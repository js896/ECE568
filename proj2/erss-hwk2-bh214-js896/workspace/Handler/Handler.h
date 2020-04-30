#ifndef HANDLER
#define HANDLER

#include "../LRU_Cache/LRU_Cache.h"
#include "../LRU_Cache/Resource_Block.h"
#include "../Message/Request_Message.h"
#include "../Socket/Proxy_Socket.h"
#include "../Socket/Socket_Ver1.h"
#include "../Tunnel/Tunnel_Ver1.h"
#include "../global_settings.h"
class Handler {
 private:
  int id;
  Proxy_Socket * mainSocket;
  static void hd_connect(Proxy_Socket * child);
  // static void hd_post(const Request_Message & msg,
  //                     std::vector<char> & recv,
  //                     Proxy_Socket * childSocket);
  static void hd_get(const Request_Message & msg,
                     std::vector<char> & recv,
                     Proxy_Socket * childSocket,
                     const std::string & id_str);

 public:
  Handler(const std::string & port);

  void run();

  static void thread_handler(Proxy_Socket * childSocket, int id);
  static bool need_refresh(std::unordered_map<std::string, std::string> rqs_map,
                           std::unordered_map<std::string, std::string> rsp_map,
                           std::string id_str);
  static bool is_chunk(const std::unordered_map<std::string, std::string> & rsp_map);
  static void send_cache(Resource_Block * blk, Proxy_Socket * client);
  static void send_normal(Proxy_Socket * client,
                          LRU_Cache * my_cache,
                          const std::unordered_map<std::string, std::string> & rqs_map,
                          const std::unordered_map<std::string, std::string> & rsp_map,
                          const std::vector<char> & rsp,
                          const std::string & key,
                          bool can_cache);

  static void send_chunk(Proxy_Socket * client,
                         Proxy_Socket * server,
                         std::vector<char> & rsp,
                         bool can_cache,
                         LRU_Cache * my_cache,
                         const std::unordered_map<std::string, std::string> & rqs_map,
                         const std::unordered_map<std::string, std::string> & rsp_map,
                         const std::string & key);
  static bool find_in_map(std::unordered_map<std::string, std::string> map,
                          const std::string & key,
                          const std::string & target);

  static bool can_cache(std::unordered_map<std::string, std::string> rqs_map,
                        std::unordered_map<std::string, std::string> rsp_map,
                        std::string id_str);

  static void ensure_fresh(Cache_Node * cache_node,
                           std::unordered_map<std::string, std::string> rqs_map,
                           std::unordered_map<std::string, std::string> rsp_map,
                           std::string id_str);
};

#endif
