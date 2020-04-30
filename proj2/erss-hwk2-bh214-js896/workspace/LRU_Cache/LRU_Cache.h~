#ifndef LRUCA
#define LRUCA

#include <cstdlib>
#include <mutex>
#include <shared_mutex>
#include <string>
#include <thread>
#include <unordered_map>
#include <vector>

#include "Cache_Node.h"

class LRU_Cache {
 private:
  mutable std::shared_mutex mutex_;

 protected:
  static LRU_Cache * my_cache;

  Cache_Node * head;  //head of the LL
  Cache_Node * tail;  //tail of the LL

  // key: uri ; value: Cache_Node*
  std::unordered_map<std::string, Cache_Node *> map;

  //constructor
  LRU_Cache();

  // pull node off the list
  void pull_node(Cache_Node * node);

  // move a node to the head
  void mv_to_head(Cache_Node * node);

  // ensure the length of the LL is CACHE_LEN
  // if longer, continuously rm until length is equal to CACHE_LEN
  void ensure_len();

 public:
  // for Singleton Pattern
  static LRU_Cache * get_instance();

  // remove by ptr
  void rm(Cache_Node *);
  // remove by key
  void rm(const std::string & uri);

  // find cache node (and mv_to_head)
  Cache_Node * get(const std::string & uri);

  //add resource in
  // if the given uri exists in the LL: append to that node's subnode
  // else: add new node before head
  void add(const std::unordered_map<std::string, std::string> & rqs_map,
           const std::unordered_map<std::string, std::string> & rsp_map,
           std::vector<char> resource,
           const std::string & uri);
};

#endif
