#ifndef RSC_BLK
#define RSC_BLK

#include <vector>

class Resource_Block {
 public:
  Resource_Block(const std::vector<char> & content);
  Resource_Block(const std::vector<char> & content,
                 Resource_Block * prev,
                 Resource_Block * next);
  Resource_Block * prev;
  Resource_Block * next;
  std::vector<char> content;
};

#endif
