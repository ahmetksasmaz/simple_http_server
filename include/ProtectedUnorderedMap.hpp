#ifndef PROTECTED_UNORDERED_MAP_H_
#define PROTECTED_UNORDERED_MAP_H_

#include <condition_variable>
#include <mutex>
#include <unordered_map>

namespace http {

template <typename K, typename V>
class ProtectedUnorderedMap {
 public:
  ProtectedUnorderedMap();
  ProtectedUnorderedMap(ProtectedUnorderedMap& unordered_map) =
      delete;  // Popping rhs unordered_map means move
  ProtectedUnorderedMap(ProtectedUnorderedMap&& unordered_map);
  ProtectedUnorderedMap& operator=(ProtectedUnorderedMap& unordered_map) =
      delete;  // Popping rhs unordered_map means move
  ProtectedUnorderedMap& operator=(ProtectedUnorderedMap&& unordered_map);
  ~ProtectedUnorderedMap();
  void Push(std::pair<K, V> element);
  V FindAndRemove(K key);

 private:
  std::mutex mutex_;
  std::condition_variable condition_variable_;
  std::unordered_map<K, V> unordered_map_;
};

}  // namespace http

#endif