/**
 * Copyright (c) 2015 - The CM Authors <legal@clickmatcher.com>
 *   All Rights Reserved.
 *
 * This file is CONFIDENTIAL -- Distribution or duplication of this material or
 * the information contained herein is strictly forbidden unless prior written
 * permission is obtained.
 */
#include "logjoin/stages/BuildSessionAttributes.h"
#include "logjoin/common.h"

using namespace stx;

namespace cm {

void BuildSessionAttributes::process(RefPtr<SessionContext> ctx) {
  auto first_seen = firstSeenTime(ctx->session);
  auto last_seen = lastSeenTime(ctx->session);
  if (first_seen.isEmpty() || last_seen.isEmpty()) {
    RAISE(kRuntimeError, "session: time isn't set");
  }

  ctx->session.set_first_seen_time(
      first_seen.get().unixMicros() / kMicrosPerSecond);

  ctx->session.set_last_seen_time(
      last_seen.get().unixMicros() / kMicrosPerSecond);

  ctx->session.set_customer(ctx->customer_key);
  for (const auto& ev : ctx->events) {
    if (ev.evtype != "__sattr") {
      continue;
    }

    URI::ParamList logline;
    URI::parseQueryString(ev.data, &logline);

    //for (const auto& p : logline) {
    //  if (p.first == "x") {
    //    experiments.emplace(p.second);
    //    continue;
    //  }
    //}

    std::string r_url;
    if (stx::URI::getParam(logline, "r_url", &r_url)) {
      ctx->session.set_referrer_url(r_url);
    }

    std::string r_cpn;
    if (stx::URI::getParam(logline, "r_cpn", &r_cpn)) {
      ctx->session.set_referrer_campaign(r_cpn);
    }

    std::string r_nm;
    if (stx::URI::getParam(logline, "r_nm", &r_nm)) {
      ctx->session.set_referrer_name(r_nm);
    }

    std::string cs;
    if (stx::URI::getParam(logline, "cs", &cs)) {
      ctx->session.set_customer_session_id(cs);
    }
  }
}

Option<UnixTime> BuildSessionAttributes::firstSeenTime(const JoinedSession& sess) {
  uint64_t t = std::numeric_limits<uint64_t>::max();

  for (const auto& e : sess.search_queries()) {
    if (e.time() * kMicrosPerSecond < t) {
      t = e.time() * kMicrosPerSecond;
    }
  }

  for (const auto& e : sess.page_views()) {
    if (e.time() * kMicrosPerSecond < t) {
      t = e.time() * kMicrosPerSecond;
    }
  }

  for (const auto& e : sess.cart_items()) {
    if (e.time() * kMicrosPerSecond < t) {
      t = e.time() * kMicrosPerSecond;
    }
  }

  if (t == std::numeric_limits<uint64_t>::max()) {
    return None<UnixTime>();
  } else {
    return Some(UnixTime(t));
  }
}

Option<UnixTime> BuildSessionAttributes::lastSeenTime(const JoinedSession& sess) {
  uint64_t t = std::numeric_limits<uint64_t>::min();

  for (const auto& e : sess.search_queries()) {
    if (e.time() * kMicrosPerSecond > t) {
      t = e.time() * kMicrosPerSecond;
    }
  }

  for (const auto& e : sess.page_views()) {
    if (e.time() * kMicrosPerSecond > t) {
      t = e.time() * kMicrosPerSecond;
    }
  }

  for (const auto& e : sess.cart_items()) {
    if (e.time() * kMicrosPerSecond > t) {
      t = e.time() * kMicrosPerSecond;
    }
  }

  if (t == std::numeric_limits<uint64_t>::min()) {
    return None<UnixTime>();
  } else {
    return Some(UnixTime(t));
  }
}

} // namespace cm

