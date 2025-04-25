#include "key_value.h"

#include <string>
#include <random>
#include <algorithm>
#include <cassert>

static std::random_device rd;
static std::mt19937 gen(rd());
static std::uniform_int_distribution<> distrib(32, 126); // ASCII range for printable characters

KV_Pair generateRandomPair() {
  KV_Pair ret;
  auto randchar = []() -> char {
    return (char)(distrib(gen));
  };

  ret.first.resize(KEY_LENGTH);
  std::generate_n(ret.first.begin(), KEY_LENGTH, randchar);
  assert(ret.first.size() == KEY_LENGTH);

  ret.second.resize(VALUE_LENGTH);
  std::generate_n(ret.second.begin(), VALUE_LENGTH, randchar);
  assert(ret.second.size() == VALUE_LENGTH);

  return ret;
}
