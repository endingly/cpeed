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
  bool   stdoutLog = false;
  string logfile;
  string dir;
  string ufilename;
  int    split      = 0;
  bool   daemonMode = false;

  int     c;
  Option* op = new Option();

  while (1) {
    int                  optIndex = 0;
    int                  lopt;
    static struct option longOpts[] = {{"daemon", no_argument, NULL, 'D'},
                                       {"dir", required_argument, NULL, 'd'},
                                       {"out", required_argument, NULL, 'o'},
                                       {"log", required_argument, NULL, 'l'},
                                       {"split", required_argument, NULL, 's'},
                                       {"http-proxy", required_argument, &lopt, 1},
                                       {"http-user", required_argument, &lopt, 2},
                                       {"http-passwd", required_argument, &lopt, 3},
                                       {"http-proxy-user", required_argument, &lopt, 4},
                                       {"http-proxy-passwd", required_argument, &lopt, 5},
                                       {"http-auth-scheme", required_argument, &lopt, 6},
                                       {"version", no_argument, NULL, 'v'},
                                       {"help", no_argument, NULL, 'h'},
                                       {0, 0, 0, 0}};
    c                               = getopt_long(argc, argv, "Dd:o:l:s:vh", longOpts, &optIndex);
    if (c == -1) {
      break;
    }
    switch (c) {
      case 0: {
        switch (lopt) {
          case 1: {
            pair<string, string> proxy;
            Util::split(proxy, optarg, ':');
            int port = (int)strtol(proxy.second.c_str(), NULL, 10);
            if (proxy.first.empty() || proxy.second.empty() || !(0 < port && port <= 65535)) {
              cerr << "unrecognized proxy format" << endl;
              showUsage();
              exit(1);
            }
            op->put("http_proxy_host", proxy.first);
            op->put("http_proxy_port", proxy.second);
            op->put("http_proxy_enabled", "true");
            break;
          }
          case 2:
            op->put("http_user", optarg);
            break;
          case 3:
            op->put("http_passwd", optarg);
            break;
          case 4:
            op->put("http_proxy_user", optarg);
            op->put("http_proxy_auth_enabled", "true");
            break;
          case 5:
            op->put("http_proxy_passwd", optarg);
            break;
          case 6:
            if (string("BASIC") == optarg) {
              op->put("http_auth_scheme", "BASIC");
            } else {
              cerr << "Currently, supported authentication scheme is BASIC." << endl;
            }
            break;
        }
        break;
      }
      case 'D':
        daemonMode = true;
        break;
      case 'd':
        dir = optarg;
        break;
      case 'o':
        ufilename = optarg;
        break;
      case 'l':
        if (strcmp("-", optarg) == 0) {
          stdoutLog = true;
        } else {
          logfile = optarg;
        }
        break;
      case 's':
        split = (int)strtol(optarg, NULL, 10);
        if (!(1 < split && split < 5)) {
          cerr << "split must be between 1 and 5." << endl;
          showUsage();
          exit(1);
        }
        break;
      case 'v':
        showVersion();
        exit(0);
      case 'h':
        showUsage();
        exit(0);
      default:
        showUsage();
        exit(1);
    }
  }
  if (optind == argc) {
    cerr << "specify at least one URL" << endl;
    showUsage();
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
