/**
 * This file is part of the "libfnord" project
 *   Copyright (c) 2015 Paul Asmuth
 *
 * FnordMetric is free software: you can redistribute it and/or modify it under
 * the terms of the GNU General Public License v3.0. You should have received a
 * copy of the GNU General Public License along with this program. If not, see
 * <http://www.gnu.org/licenses/>.
 */
#ifndef _FNORD_TSDB_STREAMCHUNK_H
#define _FNORD_TSDB_STREAMCHUNK_H
#include <stx/stdtypes.h>
#include <stx/option.h>
#include <stx/UnixTime.h>
#include <stx/util/binarymessagereader.h>
#include <stx/util/binarymessagewriter.h>
#include <stx/protobuf/MessageSchema.h>
#include <tsdb/Table.h>
#include <tsdb/RecordRef.h>
#include <tsdb/TSDBNodeRef.h>
#include <tsdb/PartitionInfo.pb.h>
#include <tsdb/PartitionSnapshot.h>
#include <tsdb/PartitionWriter.h>
#include <tsdb/PartitionReader.h>
#include <cstable/CSTableReader.h>

using namespace stx;

namespace tsdb {
class Table;

class Partition : public RefCounted {
public:

  static RefPtr<Partition> create(
      const String& tsdb_namespace,
      RefPtr<Table> table,
      const SHA1Hash& partition_key,
      TSDBNodeRef* node);

  static RefPtr<Partition> reopen(
      const String& tsdb_namespace,
      RefPtr<Table> table,
      const SHA1Hash& partition_key,
      TSDBNodeRef* node);

  Partition(
      RefPtr<PartitionSnapshot> snap,
      RefPtr<Table> table);

  RefPtr<PartitionWriter> getWriter();
  RefPtr<PartitionReader> getReader();
  PartitionInfo getInfo() const;

protected:
  RefPtr<PartitionSnapshot> head_;
  RefPtr<Table> table_;
  RefPtr<PartitionWriter> writer_;
};

}
#endif
