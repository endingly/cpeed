#include <getopt.h>
#include <libgen.h>
#include <signal.h>
#include <unistd.h>

#include <algorithm>
#include <cstring>
#include <iostream>
#include <utility>
#include <vector>

#include "DefaultDiskWriter.hpp"
#include "DownloadEngine.hpp"
#include "Gflags_modules.hpp"
#include "HttpInitiateConnectionCommand.hpp"
#include "InitiateConnectionCommandFactory.hpp"
#include "SegmentManager.hpp"
#include "SimpleLogger.hpp"
#include "Util.hpp"
#include "config.hpp"
using namespace std;

extern char* optarg;
extern int   optind, opterr, optopt;

/// @brief 清空请求
/// @details 释放请求内存
/// @param req
void clearRequest(Request* req) { delete (req); }

DownloadEngine* e;

void handler(int signal) {
  cout << "\nSIGINT signal received." << endl;
  e->segmentMan->save();
  if (e->diskWriter != NULL) {
    e->diskWriter->closeFile();
  }
  exit(0);
}

void addCommand(int cuid, const char* url, vector<Request*> requests) {
  Request* req = new Request();
  if (req->setUrl(url)) {
    e->commands.push(
        InitiateConnectionCommandFactory::createInitiateConnectionCommand(cuid, req, e));
    requests.push_back(req);
  } else {
    cerr << "Unrecognized URL or unsupported protocol: " << req->getUrl() << endl;
    delete (req);
  }
}

void showVersion() { cout << PACKAGE_NAME << " version " << PACKAGE_VERSION << endl; }

void showUsage() {
  cout << "Usage: " << PACKAGE_NAME << " [options] URL ..." << endl;
  cout << "Options:" << endl;
  cout << " -d, --dir=DIR              The directory to store downloaded file." << endl;
  cout << " -o, --out=FILE             The file name for downloaded file." << endl;
  cout << " -l, --log=LOG              The file path to store log. If '-' is specified," << endl;
  cout << "                            log is written to stdout." << endl;
  cout << " -D, --daemon               Run as daemon." << endl;
  cout << " -s, --split=N              Download a file using s connections. s must be" << endl;
  cout << "                            between 1 and 5. If this option is specified the" << endl;
  cout << "                            first URL is used, and the other URLs are ignored." << endl;
  cout << " --http-proxy=HOST:PORT     Use HTTP proxy server. This affects to all" << endl;
  cout << "                            URLs." << endl;
  cout << " --http-user=USER           Set HTTP user. This affects to all URLs." << endl;
  cout << " --http-passwd=PASSWD       Set HTTP password. This affects to all URLs." << endl;
  cout << " --http-proxy-user=USER     Set HTTP proxy user. This affects to all URLs" << endl;
  cout << " --http-proxy-passwd=PASSWD Set HTTP proxy password. This affects to all URLs." << endl;
  cout << " --http-auth-scheme=SCHEME  Set HTTP authentication scheme. Currently, BASIC" << endl;
  cout << "                            is the only supported scheme." << endl;
  cout << " -v, --version              Print the version number and exit." << endl;
  cout << " -h, --help                 Print this message and exit." << endl;
  cout << "URL:" << endl;
  cout << " You can specify multiple URLs. All URLs must point to the same file" << endl;
  cout << " or a download fails." << endl;
  cout << "Examples:" << endl;
  cout << " Download a file by 1 connection:" << endl;
  cout << "  cpeed http://AAA.BBB.CCC/file.zip" << endl;
  cout << " Download a file by 2 connections:" << endl;
  cout << "  cpeed -s 2 http://AAA.BBB.CCC/file.zip" << endl;
  cout << " Download a file by 2 connections, each connects to a different server." << endl;
  cout << "  cpeed http://AAA.BBB.CCC/file.zip http://DDD.EEE.FFF/GGG/file.zip" << endl;
}

int main(int argc, char* argv[]) {
  gflags::SetVersionString(PACKAGE_NAME + "\n" + PACKAGE_VERSION);
  bool   stdoutLog = false;
  string logfile;
  string dir;
  string ufilename;
  int    split      = 0;
  bool   daemonMode = false;

  int     c;
  Option* op = new Option();

#pragma region get commandline options
  gflags::ParseCommandLineFlags(&argc, &argv, true);
  if (FLAGS_dir.size()) {
    dir = FLAGS_dir;
  }
  if (FLAGS_out.size()) {
    ufilename = FLAGS_out;
  }
  if (FLAGS_log.size()) {
    if (FLAGS_log == "-") {
      stdoutLog = true;
    } else {
      logfile = FLAGS_log;
    }
  }
  if (FLAGS_daemon) {
    daemonMode = true;
  }
  if (FLAGS_http_proxy.size()) {
    pair<string, string> proxy;
    Util::split(proxy, FLAGS_http_proxy, ':');
    int port = (int)strtol(proxy.second.c_str(), NULL, 10);
    op->put("http_proxy_host", proxy.first);
    op->put("http_proxy_port", proxy.second);
    op->put("http_proxy_enabled", "true");
  }
  if (FLAGS_http_user.size()) {
    op->put("http_user", FLAGS_http_user);
  }
  if (FLAGS_http_passwd.size()) {
    op->put("http_passwd", FLAGS_http_passwd);
  }
  if (FLAGS_http_proxy_user.size()) {
    op->put("http_proxy_user", FLAGS_http_proxy_user);
    op->put("http_proxy_auth_enabled", "true");
  }
  if (FLAGS_http_proxy_passwd.size()) {
    op->put("http_proxy_passwd", FLAGS_http_proxy_passwd);
  }
  if (FLAGS_http_auth_scheme.size()) {
    op->put("http_auth_scheme", "BASIC");
  }
  if (FLAGS_split > 0) {
    split = FLAGS_split;
  }
#pragma endregion

  if (optind == argc) {
    cerr << "specify at least one URL" << endl;
    // showUsage();
    exit(1);
  }
  if (daemonMode) {
    if (daemon(1, 1) < 0) {
      perror("daemon failed");
      exit(1);
    }
  }
  SimpleLogger* logger;
  if (stdoutLog) {
    logger = new SimpleLogger(stdout);
  } else if (logfile.size()) {
    logger = new SimpleLogger(logfile);
  } else {
    logger = new SimpleLogger("/dev/null");
  }

  e                        = new DownloadEngine();
  e->logger                = logger;
  e->option                = op;
  e->diskWriter            = new DefaultDiskWriter();
  e->segmentMan            = new SegmentMan();
  e->segmentMan->dir       = dir;
  e->segmentMan->ufilename = ufilename;
  e->segmentMan->logger    = logger;
  vector<Request*> requests;
  if (split > 0) {
    for (int i = 1; i <= split; i++) {
      addCommand(i, argv[optind], requests);
    }
  } else {
    for (int i = 1; optind < argc; i++) {
      addCommand(i, argv[optind++], requests);
    }
  }

  struct sigaction sigact;
  sigact.sa_handler = handler;
  sigact.sa_flags   = 0;
  sigemptyset(&sigact.sa_mask);
  sigaction(SIGINT, &sigact, NULL);

  e->run();

  for_each(requests.begin(), requests.end(), clearRequest);
  requests.clear();
  delete (logger);
  delete (e->segmentMan);
  delete (e->option);
  delete (e->diskWriter);
  delete (e);
  return 0;
}
