#include "Cache_Node.h"

Cache_Node::Cache_Node(const std::string & uri,
                       const std::unordered_map<std::string, std::string> & rqs_map,
                       const std::unordered_map<std::string, std::string> & rsp_map) {
  this->uri = std::string(uri);
  this->prev = NULL;
  this->next = NULL;
  this->rsc_head = NULL;
  this->rsc_tail = NULL;
  this->rqs_map = std::unordered_map<std::string, std::string>(rqs_map);
  this->rsp_map = std::unordered_map<std::string, std::string>(rsp_map);
}

Cache_Node::~Cache_Node() {
  Resource_Block * ptr = this->rsc_head;
  while (ptr != NULL) {
    Resource_Block * temp = ptr->next;
    delete (ptr);
    ptr = temp;
  }
}

void Cache_Node::add_rsc(const std::vector<char> & content) {
  Resource_Block * blk = new Resource_Block(content);
  if (this->rsc_head == NULL) {
    this->rsc_head = blk;
    this->rsc_tail = blk;
  }
  else {
    this->rsc_tail->next = blk;
    blk->prev = this->rsc_tail;
    this->rsc_tail = blk;
  }
}
