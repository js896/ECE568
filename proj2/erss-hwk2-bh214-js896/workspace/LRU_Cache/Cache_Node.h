#ifndef CANODE
#define CANODE
#include <unordered_map>
#include <vector>

#include "Resource_Block.h"

class Cache_Node {
 public:
  std::string uri;    // the key of this node
  Cache_Node * prev;  // points to prev node
  Cache_Node * next;  // points to next node

  Resource_Block * rsc_head;  // rsc chain head pointer
  Resource_Block * rsc_tail;  // rsc chain tail pointer

  std::unordered_map<std::string, std::string> rqs_map;  // request info
  std::unordered_map<std::string, std::string> rsp_map;  // response info

  // constructor
  Cache_Node(const std::string & uri,
             const std::unordered_map<std::string, std::string> & rqs_map,
             const std::unordered_map<std::string, std::string> & rsp_map);

  // destructor: destroy the whole resource chain
  ~Cache_Node();

  // add a resource block to the chain
  void add_rsc(const std::vector<char> & content);
};

#endif
