#include "ProtectedUnorderedMap.hpp"

template <typename K, typename V>
http::ProtectedUnorderedMap<K, V>::ProtectedUnorderedMap() {}

template <typename K, typename V>
http::ProtectedUnorderedMap<K, V>::ProtectedUnorderedMap(
    http::ProtectedUnorderedMap<K, V>&& protected_unordered_map) {
  if (this != &protected_unordered_map) {
    mutex_.lock();
    unordered_map_.clear();
    protected_unordered_map.mutex_.lock();
    for (auto kv_pair : protected_unordered_map.unordered_map_) {
      unordered_map_.insert(kv_pair);
    }
    protected_unordered_map.unordered_map_.clear();
    protected_unordered_map.mutex_.unlock();
    mutex_.unlock();
  }
}

template <typename K, typename V>
http::ProtectedUnorderedMap<K, V>& http::ProtectedUnorderedMap<K, V>::operator=(
    http::ProtectedUnorderedMap<K, V>&& protected_unordered_map) {
  if (this != &protected_unordered_map) {
    mutex_.lock();
    unordered_map_.clear();
    protected_unordered_map.mutex_.lock();
    for (auto kv_pair : protected_unordered_map.unordered_map_) {
      unordered_map_.insert(kv_pair);
    }
    protected_unordered_map.unordered_map_.clear();
    protected_unordered_map.mutex_.unlock();
    mutex_.unlock();
  }
  return *this;
}

template <typename K, typename V>
http::ProtectedUnorderedMap<K, V>::~ProtectedUnorderedMap() {
  mutex_.lock();
  unordered_map_.clear();
  mutex_.unlock();
}

template <typename K, typename V>
void http::ProtectedUnorderedMap<K, V>::Push(std::pair<K, V> element) {
  std::unique_lock<std::mutex> lock(mutex_);
  unordered_map_.insert(element);
  condition_variable_.notify_one();
}

template <typename K, typename V>
V http::ProtectedUnorderedMap<K, V>::FindAndRemove(K key) {
  std::unique_lock<std::mutex> lock(mutex_);
  condition_variable_.wait(lock, [this, key]() {
    return unordered_map_.find(key) != unordered_map_.end();
  });

  V element = unordered_map_.at(key);
  unordered_map_.erase(key);

  return element;
}

template class http::ProtectedUnorderedMap<int, std::string>;