/* Copyright (C) 2026, Mikhail Nikonov <michael.n.nikonov@gmail.com>
 *
 * This Source Code Form is subject to the terms of the
 * Mozilla Public License (MPL), v. 2.0.
 * If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef DLT_MCP_SERVER_UTILITY_HASH_H_
#define DLT_MCP_SERVER_UTILITY_HASH_H_

#include <cstdint>
#include <string_view>

inline uint32_t fnv1a32(std::string_view data) {
  uint32_t hash = 0x811c9dc5u;
  for (const unsigned char c : data) {
    hash ^= c;
    hash *= 0x01000193u;
  }
  return hash;
}

inline uint64_t fnv1a64(std::string_view data) {
  uint64_t hash = 0xcbf29ce484222325u;
  for (const unsigned char c : data) {
    hash ^= c;
    hash *= 0x100000001b3u;
  }
  return hash;
}

#endif  // DLT_MCP_SERVER_UTILITY_HASH_H_
