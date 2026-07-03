/* Copyright (C) 2026, Mikhail Nikonov <michael.n.nikonov@gmail.com>
 *
 * This Source Code Form is subject to the terms of the
 * Mozilla Public License (MPL), v. 2.0.
 * If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "utility/dlt_id.h"
#include "utility/hash.h"
#include "utility/string.h"
#include "utility/time.h"

#include <gtest/gtest.h>

TEST(Hash, Fnv1a32EmptyString) {
  EXPECT_EQ(fnv1a32(""), 0x811c9dc5u);
}

TEST(Hash, Fnv1a64EmptyString) {
  EXPECT_EQ(fnv1a64(""), 0xcbf29ce484222325ull);
}

TEST(Hash, Fnv1a32TestVectorA) {
  EXPECT_EQ(fnv1a32("a"), 0xe40c292cu);
}

TEST(Hash, Fnv1a64TestVectorA) {
  EXPECT_EQ(fnv1a64("a"), 0xaf63dc4c8601ec8cull);
}

TEST(Hash, Fnv1a32TestVectorFoobar) {
  EXPECT_EQ(fnv1a32("foobar"), 0xbf9cf968u);
}

TEST(Hash, Fnv1a64TestVectorFoobar) {
  EXPECT_EQ(fnv1a64("foobar"), 0x85944171f73967e8ull);
}

TEST(String, CleanPayloadEmpty) {
  EXPECT_EQ(cleanPayload(""), "");
}

TEST(String, CleanPayloadRemovesNulls) {
  std::string input("hello\x00world", 11);
  EXPECT_EQ(cleanPayload(input), "helloworld");
}

TEST(String, CleanPayloadRemovesNewlines) {
  EXPECT_EQ(cleanPayload("hello\nworld"), "helloworld");
  EXPECT_EQ(cleanPayload("hello\r\nworld"), "helloworld");
  EXPECT_EQ(cleanPayload("hello\rworld"), "helloworld");
}

TEST(String, CleanPayloadPreservesOtherChars) {
  EXPECT_EQ(cleanPayload("hello world"), "hello world");
  EXPECT_EQ(cleanPayload("tabs\there"), "tabs\there");
  EXPECT_EQ(cleanPayload("special!@#"), "special!@#");
}

TEST(String, CleanPayloadOnlyRemovable) {
  EXPECT_EQ(cleanPayload("\0\n\r\0"), "");
}

TEST(String, CleanPayloadNoRemovable) {
  EXPECT_EQ(cleanPayload("unchanged"), "unchanged");
}
