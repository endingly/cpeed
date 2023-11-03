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

void setUsageMessage() {
  std::string s1 = "Usage: \n";
  std::string s2 = " You can specify multiple URLs. All URLs must point to the same file\n";
  std::string s3 = " or a download fails.\n";
  std::string s4 = "Example:\n";
  std::string s5 = " Download a file by 1 connection:\n";
  std::string s6 = "  $ cpeed http://example.com/file.zip\n";
  std::string s7 = " Download a file by 2 connections:\n";
  std::string s8 = "  $ cpeed http://example.com/file.zip http://example.com/file.zip\n";
  auto        s  = s1 + s2 + s3 + s4 + s5 + s6 + s7 + s8;
  gflags::SetUsageMessage(s.c_str());
}

int main(int argc, char* argv[]) {
  gflags::SetVersionString(PACKAGE_VERSION);
  setUsageMessage();
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
    std::cout << gflags::ProgramUsage() << std::endl;
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
