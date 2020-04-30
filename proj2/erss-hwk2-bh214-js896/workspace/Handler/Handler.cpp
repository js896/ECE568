#include "Handler.h"

#include <iostream>
#include <thread>

#include "../Exceptions/Cache_Exceptions.h"
#include "../Exceptions/Parser_Exceptions.h"
#include "../Exceptions/Socket_Exceptions.h"
#include "../LRU_Cache/Cache_Node.h"
#include "../LRU_Cache/LRU_Cache.h"
#include "../LRU_Cache/Resource_Block.h"
#include "../Logger/Logger.h"
#include "../Parser/Response_Parser.h"
#include "../Tools/String_Tools.h"

Handler::Handler(const std::string & port) {
  this->id = 0;
  mainSocket = new Socket_Ver1;
  const std::string ip = "0.0.0.0";
  mainSocket->bind_to(ip, port);
}

void Handler::run() {
  mainSocket->set_listen(BACK_LOG);
  while (1) {
    Proxy_Socket * child = mainSocket->accept_connection();
    std::thread t1(thread_handler, child, id++);
    t1.detach();
  }
}

void Handler::thread_handler(Proxy_Socket * childSocket, int id) {
  std::string id_str = std::to_string(id);
  std::vector<char> recv;
  try {
    recv = childSocket->recv_str(SERVER_BUF_SIZE);
  }
  catch (connection_break_exception * e) {
    childSocket->close_socket();
    return;
  }

  try {
    Request_Message msg(recv);

    Logger::write_to_file(id_str + ": \"" + msg.map.find("line1")->second + "\" from " +
                              childSocket->get_peer_ip() + " @ " +
                              String_Tools::get_nowtime(),
                          LOG_PATH);  // initial log

    std::string method = msg.map.find("method")->second;
    if (method.compare("CONNECT") == 0) {
      Socket_Ver1 skt;
      skt.connect_to(msg.map.find("ip")->second, msg.map.find("port")->second);
      try {
        Tunnel_Ver1 tunnel(childSocket, &skt);
        tunnel.run();
      }
      catch (recv_str_exception * e) {
        Logger::write_to_file(id_str + ": Tunnel Closed\n", LOG_PATH);
        return;
      }
    }
    else if (method.compare("GET") == 0 || method.compare("POST") == 0) {
      hd_get(msg, recv, childSocket, id_str);
    }
  }
  catch (parse_request_exception * e) {
    return;
  }
}

void Handler::hd_get(const Request_Message & msg,
                     std::vector<char> & recv,
                     Proxy_Socket * childSocket,
                     const std::string & id_str) {
  Socket_Ver1 skt;
  skt.connect_to(msg.map.find("ip")->second, msg.map.find("port")->second);

  // try look up in cache
  LRU_Cache * my_cache = LRU_Cache::get_instance();
  Cache_Node * cache_node = NULL;
  std::string uri = msg.map.find("uri")->second;
  try {
    cache_node = my_cache->get(uri);
  }
  catch (cache_not_found_exception * e) {
    //Could not find in cache
    Logger::write_to_file(id_str + ": not in cache\n", LOG_PATH);
  }

  if (cache_node == NULL) {  // content not in cache: request from server
    skt.send_str(recv);
    //std::cout << "GET from proxy to server:" << recv.data() << "\n";
    std::unordered_map<std::string, std::string> rqs_map = msg.map;
    std::string request_str = rqs_map.find("line1")->second;
    Logger::write_to_file(
        id_str + ": Requesting \"" + request_str + "\" from " + skt.get_peer_ip() + "\n",
        LOG_PATH);

    std::vector<char> response;
    try {
      response = skt.recv_str(SERVER_BUF_SIZE);
    }
    catch (connection_break_exception * e) {
      return;
    }
    std::unordered_map<std::string, std::string> rsp_map;

    // parse the response
    Response_Parser::parse(rsp_map, response);

    std::string rsp_str = rsp_map.find("line1")->second;

    Logger::write_to_file(
        id_str + ": Received \"" + rsp_str + "\" from " + skt.get_peer_ip() + "\n",
        LOG_PATH);

    // can cache?
    bool can_cache = Handler::can_cache(rqs_map, rsp_map, id_str);

    // is chunk?
    bool is_chunk = Handler::is_chunk(rsp_map);
    if (is_chunk) {  //chunk
      Handler::send_chunk(
          childSocket, &skt, response, can_cache, my_cache, rqs_map, rsp_map, uri);
    }
    else {  // normal: one-time
      Handler::send_normal(
          childSocket, my_cache, rqs_map, rsp_map, response, uri, can_cache);
    }
  }
  else {  // content in cache: send back to client
    std::string rsp_str = (cache_node->rsp_map).find("line1")->second;

    Handler::ensure_fresh(cache_node,
                          cache_node->rqs_map,
                          cache_node->rsp_map,
                          id_str);  // ensure cache is fresh

    Logger::write_to_file(id_str + ": Responding \"" + rsp_str + "\"\n", LOG_PATH);
    Handler::send_cache(cache_node->rsc_head, childSocket);
  }

  childSocket->close_socket();
  skt.close_socket();
}

void Handler::hd_connect(Proxy_Socket * child) {
}

bool Handler::can_cache(std::unordered_map<std::string, std::string> rqs_map,
                        std::unordered_map<std::string, std::string> rsp_map,
                        std::string id_str) {
  // can't cache: rqs has "no-store"
  if (Handler::find_in_map(rqs_map, "Cache-Control", "no-store")) {
    Logger::write_to_file(id_str + ": not cacheable because Request has \"no-store\"\n",
                          LOG_PATH);
    return false;
  }

  // can't cache: rsp has "no-store"
  if (Handler::find_in_map(rsp_map, "Cache-Control", "no-store")) {
    Logger::write_to_file(id_str + ": not cacheable because Response has \"no-store\"\n",
                          LOG_PATH);
    return false;
  }

  if (Handler::find_in_map(rqs_map, "Cache-Control", "must-revalidate") ||
      Handler::find_in_map(rsp_map, "Cache-Control", "must-revalidate")) {
    Logger::write_to_file(id_str + ": cached, but requires re-validation\n", LOG_PATH);
    return true;
  }

  // can cache
  std::unordered_map<std::string, std::string>::iterator it;
  it = rqs_map.find("Expires");
  if (it != rqs_map.end()) {  // rqs expries date
    std::string exp_date = it->second;
    Logger::write_to_file(id_str + ": cached, expires at " + exp_date + "\n", LOG_PATH);
    return true;
  }

  it = rsp_map.find("Expires");
  if (it != rsp_map.end()) {  // rsp expires date
    std::string exp_date = it->second;
    Logger::write_to_file(id_str + ": cached, expires at " + exp_date + "\n", LOG_PATH);
    return true;
  }

  Logger::write_to_file(id_str + ": cached, expire date time not specified\n", LOG_PATH);
  return true;
}

bool Handler::find_in_map(std::unordered_map<std::string, std::string> map,
                          const std::string & key,
                          const std::string & target) {
  std::unordered_map<std::string, std::string>::iterator it = map.find(key);

  if (it == map.end()) {
    return false;
  }
  std::string value = it->second;
  if (value.find(target) == std::string::npos) {
    return false;
  }
  else {
    return true;
  }
}

void Handler::send_chunk(Proxy_Socket * client,
                         Proxy_Socket * server,
                         std::vector<char> & rsp,
                         bool can_cache,
                         LRU_Cache * my_cache,
                         const std::unordered_map<std::string, std::string> & rqs_map,
                         const std::unordered_map<std::string, std::string> & rsp_map,
                         const std::string & key) {
  do {
    client->send_str(rsp);  // send
    if (can_cache) {        // cache
      my_cache->add(rqs_map, rsp_map, rsp, key);
    }
    rsp = server->recv_str(SERVER_BUF_SIZE);  // recv
  } while (!(rsp[0] == '0' && rsp[1] == '\r' && rsp[2] == '\n'));

  client->send_str(rsp);  // send 0\r\n

  if (can_cache) {
    my_cache->add(rqs_map, rsp_map, rsp, key);  // cache 0\r\n
  }
}

void Handler::send_normal(Proxy_Socket * client,
                          LRU_Cache * my_cache,
                          const std::unordered_map<std::string, std::string> & rqs_map,
                          const std::unordered_map<std::string, std::string> & rsp_map,
                          const std::vector<char> & rsp,
                          const std::string & key,
                          bool can_cache) {
  client->send_str(rsp);  // send

  if (can_cache) {  // cache
    my_cache->add(rqs_map, rsp_map, rsp, key);
  }
}

bool Handler::is_chunk(const std::unordered_map<std::string, std::string> & rsp_map) {
  return Handler::find_in_map(rsp_map, "Transfer-Encoding", "chunked");
}

void Handler::send_cache(Resource_Block * blk, Proxy_Socket * client) {
  while (blk != NULL) {
    client->send_str(blk->content);
    blk = blk->next;
  }
}

bool Handler::need_refresh(std::unordered_map<std::string, std::string> rqs_map,
                           std::unordered_map<std::string, std::string> rsp_map,
                           std::string id_str) {
  // find out if need re-validation
  if (Handler::find_in_map(rqs_map, "Cache-Control", "must-revalidate") ||
      Handler::find_in_map(rsp_map, "Cache-Control", "must-revalidate")) {
    Logger::write_to_file(id_str + ": in cache, requires validation\n", LOG_PATH);
    return true;
  }

  std::unordered_map<std::string, std::string>::iterator it_date;
  std::unordered_map<std::string, std::string>::iterator it_cc;
  std::unordered_map<std::string, std::string>::iterator it_exp_rqs;
  std::unordered_map<std::string, std::string>::iterator it_exp_rsp;

  std::string max_age_str("max-age=");
  std::string cc_str("Cache-Control");
  std::string date_str("Date");
  std::string exp_str("Expires");

  it_date = rsp_map.find(date_str);
  it_cc = rsp_map.find(cc_str);
  it_exp_rqs = rqs_map.find(exp_str);
  it_exp_rsp = rsp_map.find(exp_str);

  time_t exp_time_t;

  // calculate expire time_t
  // using Date and Cache-Control(max-age:)
  if (it_date != rsp_map.end() && Handler::find_in_map(rsp_map, cc_str, max_age_str)) {
    std::string date;
    std::string cc;  // cache control value

    // get response date in time_t
    date = it_date->second;

    time_t date_time_t = String_Tools::rqs_to_time_t(date);

    // get max-age in seconds (int)
    cc = it_cc->second;
    time_t max_age_time_t = (time_t)String_Tools::find_int_after(cc, max_age_str);

    // add them up
    exp_time_t = date_time_t + max_age_time_t;
  }
  else if (it_exp_rqs != rqs_map.end() ||
           it_exp_rsp != rsp_map.end()) {  // compare with Expires
    // request singly has the expires time
    if (it_exp_rqs != rqs_map.end() && it_exp_rsp == rsp_map.end()) {
      exp_time_t = String_Tools::rqs_to_time_t(it_exp_rqs->second);
    }
    else if (it_exp_rqs == rqs_map.end() &&
             it_exp_rsp != rsp_map.end()) {  // response singly has the expires time
      exp_time_t = String_Tools::rqs_to_time_t(it_exp_rsp->second);
    }
    else {  // both exists: get the smaller one
      time_t exp_time_t_rqs = String_Tools::rqs_to_time_t(it_exp_rqs->second);
      time_t exp_time_t_rsp = String_Tools::rqs_to_time_t(it_exp_rsp->second);
      exp_time_t = exp_time_t_rqs <= exp_time_t_rsp ? exp_time_t_rqs : exp_time_t_rsp;
    }
  }
  else {  // no life-span control: return true (default need refresh)
    Logger::write_to_file(id_str + ": in cache, requires validation\n", LOG_PATH);
    return true;
  }

  // compare resource  with current time
  tm * tmptr = gmtime(&exp_time_t);
  char * dt = asctime(tmptr);

  time_t now = time(0);
  std::string exp_d_str(dt);
  if (now >= exp_time_t) {
    Logger::write_to_file(id_str + ": in cache, but expired at " + exp_d_str, LOG_PATH);
    return true;
  }
  else {
    Logger::write_to_file(id_str + ": in cache, vaild\n", LOG_PATH);
    return false;
  }
}

void Handler::ensure_fresh(Cache_Node * cache_node,
                           std::unordered_map<std::string, std::string> rqs_map,
                           std::unordered_map<std::string, std::string> rsp_map,
                           std::string id_str) {
  bool need_refresh = Handler::need_refresh(rqs_map, rsp_map, id_str);
  if (need_refresh && cache_node != NULL) {
    //TBD
    return;
  }
}
