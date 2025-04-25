#ifndef KEY_VALUE_H_
#define KEY_VALUE_H_

#include <string>

#define KEY_LENGTH 24
#define VALUE_LENGTH 1000

typedef std::pair<std::string, std::string> KV_Pair;

KV_Pair generateRandomPair();

#endif // KEY_VALUE_H_