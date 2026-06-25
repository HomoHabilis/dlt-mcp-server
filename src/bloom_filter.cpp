/* Copyright (C) 2026, Mikhail Nikonov <michael.n.nikonov@gmail.com>
 *
 * This Source Code Form is subject to the terms of the
 * Mozilla Public License (MPL), v. 2.0.
 * If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "bloom_filter.h"

#include <functional>

#include "utility/hash.h"
#include "utility/string.h"

BloomFilter::BloomFilter(Size filter_size)
    : bytes_per_block_(static_cast<size_t>(filter_size)),
      bit_mask_((static_cast<size_t>(filter_size) * 8) - 1) {}

void BloomFilter::add(size_t index, std::string_view text) {
  if (!bytes_per_block_) {
    return;
  }
  uint8_t* block = getBlock(index);
  processTrigrams(text, [block](size_t byte_offset, uint8_t bit_mask) {
    block[byte_offset] |= bit_mask;
  });
}

BloomFilter::Key BloomFilter::makeKey(std::string_view text) const {
  Key key;
  processTrigrams(text, [&key](size_t byte_offset, uint8_t bit_mask) {
    key.probes_.push_back({byte_offset, bit_mask});
  });
  return key;
}

bool BloomFilter::contains(size_t index, Key& key) const {
  if (bytes_per_block_ == 0) {
    return true;
  }

  const auto block_id = blockId(index);

  if (block_id < key.block_states_.size()) {
    if (key.block_states_[block_id] == Key::BlockState::kNegative) {
      return false;
    }
    if (key.block_states_[block_id] == Key::BlockState::kPositive) {
      return true;
    }
  }

  if (block_id >= blocks_.size() || !blocks_[block_id]) {
    return true;
  }

  const auto& block = *blocks_[block_id];
  bool match = true;
  for (const auto& [byte_offset, mask] : key.probes_) {
    if (!(block[byte_offset] & mask)) {
      match = false;
      break;
    }
  }

  if (block_id >= key.block_states_.size()) {
    key.block_states_.resize(block_id + 1, Key::BlockState::kUnchecked);
  }
  key.block_states_[block_id] =
      match ? Key::BlockState::kPositive : Key::BlockState::kNegative;
  return match;
}

size_t BloomFilter::blockId(size_t index) { return index / kBlockSize; }

uint8_t* BloomFilter::getBlock(size_t index) {
  const size_t id = blockId(index);
  if (id >= blocks_.size()) {
    blocks_.resize(id + 1);
  }
  if (!blocks_[id]) {
    blocks_[id] = std::make_unique<std::vector<uint8_t>>(bytes_per_block_, 0);
  }
  return blocks_[id]->data();
}

template <typename Op>
void BloomFilter::processTrigrams(std::string_view text, Op&& op) const {
  const std::string normalized = toLowerCase(text);
  for (size_t i = 0; i + 2 < normalized.size(); ++i) {
    uint64_t hash = fnv1a64(std::string_view(&normalized[i], 3));
    uint32_t h1 = static_cast<uint32_t>(hash >> 32);
    uint32_t h2 = static_cast<uint32_t>(hash) | 1;  // ensure odd
    for (size_t k = 0; k < kHashCount; ++k) {
      uint32_t combined = h1 + static_cast<uint32_t>(k) * h2;
      size_t bit_offset = combined & bit_mask_;
      size_t byte_offset = bit_offset >> 3;
      uint8_t bit_mask = static_cast<uint8_t>(1 << (bit_offset & 7));
      op(byte_offset, bit_mask);
    }
  }
}
