#ifndef __THREADPOOL_H__
#define __THREADPOOL_H__

#include <boost/asio.hpp>
#include <vector>
#include <boost/thread.hpp>

using namespace std;
using namespace boost;

class ThreadPool{
 private:
  int num;
  bool stopped_;
  vector<std::unique_ptr<boost::thread>>pool;
  asio::io_service my_io_;
  asio::io_service::work my_work_;
  
 public:
 ThreadPool(int n):num(n),stopped_(false),my_work_(my_io_){
    for(int i=0; i<num; i++) {
      pool.push_back(std::unique_ptr<boost::thread>(new boost::thread([&](){my_io_.run();})));
    }
  }

  ~ThreadPool(){
    stop();
  }
 
  template<class F, class...Args>
   void post(F f, Args&&...args){
   my_io_.post(std::bind(f, std::forward<Args>(args)...));
 }

 void stop(){
   if(!stopped_) {
     my_io_.stop();    
     for(auto& t : pool) t->join();
     stopped_ = true;
   }
 }
};


#endif
