#include "LRU_Cache.h"

int main() {
  LRUCache test;
  LRUCache::node node1;
  LRUCache::node node2;
  LRUCache::node node3;
  std::string key1 = "node1";
  std::string key2 = "node2";
  std::string key3 = "node3";
  test.add(node1, key1);
  test.add(node2, key2);
  test.add(node3, key3);
  test.get("node1");
  test.remove("node2");
  test.rm_tail();
  test.remove("node3");
  return EXIT_SUCCESS;
}
