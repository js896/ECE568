#include "Resource_Block.h"

#include "../Tools/String_Tools.h"
Resource_Block::Resource_Block(const std::vector<char> & content) {
  this->prev = NULL;
  this->next = NULL;
  String_Tools::copy_vector(content, this->content);
}

Resource_Block::Resource_Block(const std::vector<char> & content,
                               Resource_Block * prev,
                               Resource_Block * next) {
  this->prev = prev;
  this->next = next;
  String_Tools::copy_vector(content, this->content);
}
