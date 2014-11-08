/**
 * This file is part of the "FnordMetric" project
 *   Copyright (c) 2014 Paul Asmuth, Google Inc.
 *
 * FnordMetric is free software: you can redistribute it and/or modify it under
 * the terms of the GNU General Public License v3.0. You should have received a
 * copy of the GNU General Public License along with this program. If not, see
 * <http://www.gnu.org/licenses/>.
 */
#include <fnordmetric/metricdb/sample.h>
#include <fnordmetric/net/udpserver.h>
#include <fnordmetric/thread/taskscheduler.h>

namespace fnordmetric {
namespace metricdb {

class StatsdServer {
public:

  StatsdServer(
      fnord::thread::TaskScheduler* server_scheduler,
      fnord::thread::TaskScheduler* work_scheduler);

  void listen(int port);

protected:
  /*
  void parseStatsdSample(
      const std::string& src,
      std::string* key,
      Sample<std::string>* sample);
  */

  fnord::net::UDPServer udp_server_;
};


}
}
