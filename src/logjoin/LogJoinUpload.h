/**
 * Copyright (c) 2015 - The CM Authors <legal@clickmatcher.com>
 *   All Rights Reserved.
 *
 * This file is CONFIDENTIAL -- Distribution or duplication of this material or
 * the information contained herein is strictly forbidden unless prior written
 * permission is obtained.
 */
#ifndef _CM_LOGJOINUPLOAD_H
#define _CM_LOGJOINUPLOAD_H
#include "fnord-base/stdtypes.h"
#include "fnord-mdb/MDB.h"
#include "fnord-msg/MessageSchema.h"
#include "fnord-msg/msg.h"
#include "fnord-http/httpconnectionpool.h"
#include "fnord-feeds/BrokerClient.h"
#include "src/JoinedSession.pb.h"

using namespace fnord;

namespace cm {

class LogJoinUpload {
public:
  static const size_t kDefaultBatchSize = 10;

  LogJoinUpload(
      RefPtr<mdb::MDB> db,
      const String& tsdb_addr,
      const String& brokerd_addr,
      http::HTTPConnectionPool* http);

  void upload();

protected:

  size_t scanQueue(const String& queue_name);
  void uploadTSDBBatch(const Vector<Buffer>& batch);

  RefPtr<mdb::MDB> db_;
  String tsdb_addr_;
  InetAddr broker_addr_;
  http::HTTPConnectionPool* http_;
  feeds::BrokerClient broker_client_;
  size_t batch_size_;
};

} // namespace cm
#endif
