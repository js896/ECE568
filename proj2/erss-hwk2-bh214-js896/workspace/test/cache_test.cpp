#include "../LRU_Cache/LRU_Cache.h"
#include "../Tools/String_Tools.h"
int main() {
  LRU_Cache * cache = LRU_Cache::get_instance();

  std::unordered_map<std::string, std::string> rqs_map1(
      {{"rqs_map1_key1 xxxxxx", "rqs_map1_val1kkkkk"}});
  std::unordered_map<std::string, std::string> rsp_map1(
      {{"  rsp_map1_key1  ", "rsp_map1_val1qweqwe"},
       {"    rsp_map1_key2", "rsp_map1_val2 qwe 3e3"},
       {" _rsp_map1_key3", "rsp_map1_val3 qw12 dsa"},
       {" rsp_map1_key4__", "rsp_map1_val4 32 qwd211"}});
  std::vector<char> rsc_1_1;
  std::vector<char> rsc_1_2;
  std::vector<char> rsc_1_3;
  String_Tools::load_into_vector(rsc_1_1, ")(*(YUHBJNKJH");
  String_Tools::load_into_vector(rsc_1_2, " \n asd 0 qwqwmmasd x");
  String_Tools::load_into_vector(rsc_1_3, " --UJ*as  ++0q==");
  std::string uri1("uri1 hey man");

  std::unordered_map<std::string, std::string> rqs_map2(
      {{"rqs_map2&_key1", "rqs_map2_val1"},
       {"rqs_map2*&^_key2", "rqs_map2_val2"},
       {"rqs_map2+_)(_key3", "rqs_map2+_)(_val3"},
       {"rqs_map2?>_key4", "rqs_map2@#$_val4"}});
  std::unordered_map<std::string, std::string> rsp_map2(
      {{"rsp_map2_key1", "rsp_map2_val1"}, {"rsp_map2_key2", "rsp_map2_val2"}});
  std::vector<char> rsc_2_1;
  std::vector<char> rsc_2_2;
  String_Tools::load_into_vector(rsc_2_1, " KJKJHqw klqw");
  String_Tools::load_into_vector(rsc_2_2, "  jjjnasd qw");
  std::string uri2("uri2 apple banana");

  std::unordered_map<std::string, std::string> rqs_map3(
      {{"rqs_map3_key1", "rqs_map3_val1"},
       {"rqs_map3_key2", "rqs_map3_val2"},
       {"rqs_map3_key3", "rqs_map3_val3"},
       {"rqs_map3_key4", "rqs_map3_val4"}});
  std::unordered_map<std::string, std::string> rsp_map3(
      {{"rsp_map3_key1", "rsp_map3_val1"},
       {"rsp_map3_key2", "rsp_map3_val2"},
       {"rsp_map3_key3", "rsp_map3_val3"},
       {"rsp_map3_key4", "rsp_map3_val4"}});
  std::vector<char> rsc_3_1;
  String_Tools::load_into_vector(rsc_3_1, ")78bajksd qwyi qw");
  std::string uri3(" uri3 fish_dog");

  std::unordered_map<std::string, std::string> rqs_map4(
      {{"rqs_map4 key1", "rqs_map4_val1"},
       {"rqs_map4 key2", "rqs_map4_val2"},
       {"rqs_map4 key3", "rqs_map4_val3"}});
  std::unordered_map<std::string, std::string> rsp_map4(
      {{"rsp_map4_key1", "rsp_map4_val1"}, {"rsp_map4_key2", "rsp_map4_val2"}});
  std::vector<char> rsc_4_1;
  String_Tools::load_into_vector(rsc_4_1, "we the best!! ");
  std::string uri4("!_uri4 airplane trane ?");

  std::unordered_map<std::string, std::string> rqs_map5(
      {{"rqs_map5 key1", "rqs_map5_val1"},
       {"rqs_map5 key2", "rqs_map5_val2"},
       {"rqs_map5 key3", "rqs_map5_val3"}});
  std::unordered_map<std::string, std::string> rsp_map5(
      {{"rsp_map5_key1", "rsp_map5_val1"}, {"rsp_map5_key2", "rsp_map5_val2"}});
  std::vector<char> rsc_5_1;
  String_Tools::load_into_vector(rsc_5_1, "we the best!! ");
  std::string uri5("this is  5 ?");

  std::unordered_map<std::string, std::string> rqs_map6(
      {{"rqs_map6 key1", "rqs_map6_val1"},
       {"rqs_map6 key2", "rqs_map6_val2"},
       {"rqs_map6 key3", "rqs_map6_val3"}});
  std::unordered_map<std::string, std::string> rsp_map6(
      {{"rsp_map6_key1", "rsp_map6_val1"}, {"rsp_map6_key2", "rsp_map6_val2"}});
  std::vector<char> rsc_6_1;
  String_Tools::load_into_vector(rsc_6_1, "we the best!! ");
  std::string uri6("!_uri6 final test ?");

  // add in 5 6
  cache->add(rqs_map5, rsp_map5, rsc_5_1, uri5);
  cache->add(rqs_map6, rsp_map6, rsc_6_1, uri6);

  // add in 1 2 3 4;
  cache->add(rqs_map1, rsp_map1, rsc_1_1, uri1);
  cache->add(rqs_map2, rsp_map2, rsc_2_1, uri2);
  cache->add(rqs_map3, rsp_map3, rsc_3_1, uri3);  // here deleted 5: 3 2 1 6
  cache->add(rqs_map4, rsp_map4, rsc_4_1, uri4);  // here deleted 6: 4 3 2 1

  // add more to 1
  cache->add(rqs_map1, rsp_map1, rsc_1_2, uri1);
  // add more to 2
  cache->add(rqs_map2, rsp_map2, rsc_2_2, uri2);
  // add more to 1
  cache->add(rqs_map1, rsp_map1, rsc_1_3, uri1);

  // here it should be 1 2 4 3

  // lookup 1
  Cache_Node * res1 = cache->get(uri1);
  if (res1 == NULL) {
    return 1;
  }
  // lookup 4
  Cache_Node * res4 = cache->get(uri4);
  if (res4 == NULL) {
    return 1;
  }
  // here it should be 4 1 2 3

  // delete 3: 4 1 2
  cache->rm(uri3);
  // delete 4: 1 2
  cache->rm(cache->get(uri4));
  // delete 1: 2
  cache->rm(uri1);
  // delete 2
  cache->rm(cache->get(uri2));

  return 0;
}
