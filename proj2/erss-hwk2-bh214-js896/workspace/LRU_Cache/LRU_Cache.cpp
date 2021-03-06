#include "LRU_Cache.h"

#include <iostream>

#include "../Exceptions/Cache_Exceptions.h"
#include "../global_settings.h"

LRU_Cache * LRU_Cache::my_cache = NULL;

LRU_Cache::LRU_Cache() {
  this->head = NULL;
  this->tail = NULL;
}

void LRU_Cache::pull_node(Cache_Node * node) {
  if (node != NULL) {
    if (node == this->head) {
      this->head = node->next;
    }
    else if (node == this->tail) {
      this->tail = node->prev;
    }
    if (node->prev != NULL) {
      node->prev->next = node->next;
    }
    if (node->next != NULL) {
      node->next->prev = node->prev;
    }
    node->prev = NULL;
    node->next = NULL;
  }
}

void LRU_Cache::mv_to_head(Cache_Node * node) {
  this->pull_node(node);
  if (this->tail == NULL) {
    this->tail = node;
  }
  node->next = this->head;
  if (this->head != NULL) {
    this->head->prev = node;
  }
  this->head = node;
}

void LRU_Cache::ensure_len() {
  //  std::unique_lock lock(this->mutex_);
  unsigned int cache_len = CACHE_LEN;
  while (this->map.size() > cache_len) {
    this->rm(this->tail);
  }
  //  std::unique_lock unlock(this->mutex_);
}

LRU_Cache * LRU_Cache::get_instance() {
  if (LRU_Cache::my_cache == NULL) {
    LRU_Cache::my_cache = new LRU_Cache;
  }
  return LRU_Cache::my_cache;
}

// remove by pointer
void LRU_Cache::rm(Cache_Node * node) {
  //  std::unique_lock lock(this->mutex_);
  if (node != NULL) {
    // rm from map
    std::string uri = node->uri;
    this->map.erase(uri);
    // rm from list
    this->pull_node(node);
    delete (node);
  }
  //  std::unique_lock unlock(this->mutex_);
}

// remove by key
void LRU_Cache::rm(const std::string & uri) {
  //  std::unique_lock lock(this->mutex_);
  Cache_Node * node;
  try {
    node = this->get(uri);
  }
  catch (cache_not_found_exception * e) {
    return;
  }
  this->rm(node);
  //  std::unique_lock unlock(this->mutex_);
}

Cache_Node * LRU_Cache::get(const std::string & uri) {
  std::shared_lock lock(this->mutex_);
  std::unordered_map<std::string, Cache_Node *>::iterator res = this->map.find(uri);

  if (res == map.end()) {
    std::cout << "Cannot find resource in cache\n";
    throw new cache_not_found_exception;
  }
  std::cout << "Found resource in cache\n";
  // move the found node to head
  this->mv_to_head(res->second);
  //  std::shared_lock unlock(this->mutex_);
  return res->second;
}

void LRU_Cache::add(const std::unordered_map<std::string, std::string> & rqs_map,
                    const std::unordered_map<std::string, std::string> & rsp_map,
                    std::vector<char> resource,
                    const std::string & uri) {
  std::unique_lock lock(this->mutex_);
  Cache_Node * node;
  // try to find the node in map
  std::unordered_map<std::string, Cache_Node *>::iterator it = this->map.find(uri);
  if (it != this->map.end()) {  // found
    node = it->second;
    this->mv_to_head(node);
  }
  else {  // not found
    node = new Cache_Node(uri, rqs_map, rsp_map);
    //when the LL has nothing
    if (this->head == NULL && this->tail == NULL) {
      this->head = node;
      this->tail = node;
    }
    //LL is not empty
    else {
      node->next = this->head;
      node->prev = NULL;
      this->head->prev = node;
      this->head = node;
    }
    //also add to the unordered_map
    this->map.insert(std::make_pair(uri, node));
  }

  // add resource to the node
  node->add_rsc(resource);

  // ensure the length <= CACHE_LEN
  this->ensure_len();
  //  std::unique_lock unlock(this->mutex_);
}
