CC=g++
CFLAGS=-std=c++17 -pthread -O0 -ggdb -g -Werror -Wall -pedantic -fPIC
SOCK = ../Socket/
EXCEPTION = ../Exceptions/
PARSER = ../Parser/
MSG = ../Message/
GLB = ../
TUN = ../Tunnel/
TOOL = ../Tools/
HAND = ../Handler/
CACHE = ../LRU_Cache/
LOGGER = ../Logger/

all: receiver_test

str_split_test: str_split_test.o String_Tools.o
	$(CC) $(CFLAGS) -o $@ $< String_Tools.o

sender_test: test_send.o Socket_Ver1.o Request_Message.o Request_Parser_Ver1.o Tunnel_Ver1.o
	$(CC) $(CFLAGS) -o $@ $< Socket_Ver1.o Request_Message.o Request_Parser_Ver1.o Tunnel_Ver1.o

receiver_test: test_recv.o Socket_Ver1.o Request_Message.o Request_Parser.o Tunnel_Ver1.o String_Tools.o Handler.o LRU_Cache.o Cache_Node.o Resource_Block.o Response_Parser.o Logger.o
	$(CC) $(CFLAGS) -o $@ $< Socket_Ver1.o Request_Message.o Request_Parser.o Tunnel_Ver1.o String_Tools.o Handler.o LRU_Cache.o Cache_Node.o Resource_Block.o Response_Parser.o Logger.o

cache_test: cache_test.o LRU_Cache.o Cache_Node.o Resource_Block.o String_Tools.o
	$(CC) $(CFLAGS) -o $@ $< LRU_Cache.o Cache_Node.o Resource_Block.o String_Tools.o

cache_test.o: cache_test.cpp
	$(CC) $(CFLAGS) -c -o $@ $<

test_send.o: test_send.cpp $(GLB)global_settings.h $(EXCEPTION)Parser_Exceptions.h  $(EXCEPTION)Socket_Exceptions.h $(TUN)Tunnel.h $(TUN)Tunnel_Ver1.h
	$(CC) $(CFLAGS) -c -o $@ $<

test_recv.o: test_recv.cpp $(GLB)global_settings.h $(EXCEPTION)Parser_Exceptions.h  $(EXCEPTION)Socket_Exceptions.h $(TUN)Tunnel.h $(TUN)Tunnel_Ver1.h
	$(CC) $(CFLAGS) -c -o $@ $< 

Socket_Ver1.o: $(SOCK)Socket_Ver1.cpp $(SOCK)Proxy_Socket.h $(SOCK)Socket_Ver1.h $(EXCEPTION)Socket_Exceptions.h  $(GLB)global_settings.h
	$(CC) $(CFLAGS) -c -o $@ $<

Request_Message.o: $(MSG)Request_Message.cpp $(MSG)Request_Message.h $(GLB)global_settings.h
	$(CC) $(CFLAGS) -c -o $@ $<

Request_Parser.o: $(PARSER)Request_Parser.cpp $(PARSER)Request_Parser.h $(GLB)global_settings.h $(EXCEPTION)Parser_Exceptions.h
	$(CC) $(CFLAGS) -c -o $@ $<

Tunnel_Ver1.o:$(TUN)Tunnel_Ver1.cpp $(TUN)Tunnel.h $(TUN)Tunnel_Ver1.h $(SOCK)Proxy_Socket.h $(GLB)global_settings.h
	$(CC) $(CFLAGS) -c -o $@ $<

String_Tools.o: $(TOOL)String_Tools.cpp $(TOOL)String_Tools.h
	$(CC) $(CFLAGS) -c -o $@ $<

Handler.o: $(HAND)Handler.cpp $(HAND)Handler.h $(GLB)global_settings.h
	$(CC) $(CFLAGS) -c -o $@ $<

LRU_Cache.o: $(CACHE)LRU_Cache.cpp $(CACHE)LRU_Cache.h $(EXCEPTION)Cache_Exceptions.h
	$(CC) $(CFLAGS) -c -o $@ $<

Cache_Node.o:$(CACHE)Cache_Node.cpp $(CACHE)Cache_Node.h $(EXCEPTION)Cache_Exceptions.h
	$(CC) $(CFLAGS) -c -o $@ $<

Resource_Block.o: $(CACHE)Resource_Block.cpp $(CACHE)Resource_Block.h $(EXCEPTION)Cache_Exceptions.h
	$(CC) $(CFLAGS) -c -o $@ $<

Response_Parser.o: $(PARSER)Response_Parser.cpp $(PARSER)Response_Parser.h $(GLB)global_settings.h $(EXCEPTION)Parser_Exceptions.h
	$(CC) $(CFLAGS) -c -o $@ $<

Logger.o: $(LOGGER)Logger.cpp $(LOGGER)Logger.h $(GLB)global_settings.h
	$(CC) $(CFLAGS) -c -o $@ $<

Request_Parser_test_get: Request_Parser_test_get.o Request_Parser.o String_Tools.o
	$(CC) $(CFLAGS)  -o $@ $< Request_Parser.o String_Tools.o

Request_Parser_test_get.o: Request_Parser_test_get.cpp $(PARSER)Request_Parser.h $(TOOL)String_Tools.h
	$(CC) $(CFLAGS) -c -o $@ $<

Request_Parser_test_connect: Request_Parser_test_connect.o Request_Parser.o String_Tools.o
	$(CC) $(CFLAGS)  -o $@ $< Request_Parser.o String_Tools.o

Request_Parser_test_connect.o: Request_Parser_test_connect.cpp $(PARSER)Request_Parser.h $(TOOL)String_Tools.h
	$(CC) $(CFLAGS) -c -o $@ $<

Request_Parser_test_post: Request_Parser_test_post.o Request_Parser.o String_Tools.o
	$(CC) $(CFLAGS)  -o $@ $< Request_Parser.o String_Tools.o

Request_Parser_test_post.o: Request_Parser_test_post.cpp $(PARSER)Request_Parser.h $(TOOL)String_Tools.h
	$(CC) $(CFLAGS) -c -o $@ $<

Response_Parser_test: Response_Parser_test.o Response_Parser.o String_Tools.o
	$(CC) $(CFLAGS)  -o $@ $< Response_Parser.o String_Tools.o

Response_Parser_test.o: Response_Parser_test.cpp $(PARSER)Response_Parser.h $(TOOL)String_Tools.h
	$(CC) $(CFLAGS) -c -o $@ $<


test_rqs_to_tm: test_rqs_to_tm.o String_Tools.o
	$(CC) $(CFLAGS)  -o $@ $< String_Tools.o

test_rqs_to_tm.o: test_rqs_to_tm.cpp $(TOOL)String_Tools.h
	$(CC) $(CFLAGS) -c -o $@ $<

test_rqs_to_time_t: test_rqs_to_time_t.o String_Tools.o
	$(CC) $(CFLAGS)  -o $@ $< String_Tools.o

test_rqs_to_time_t.o: test_rqs_to_time_t.cpp $(TOOL)String_Tools.h
	$(CC) $(CFLAGS) -c -o $@ $<

test_find_int_after: test_find_int_after.o String_Tools.o
	$(CC) $(CFLAGS)  -o $@ $< String_Tools.o

test_find_int_after.o: test_find_int_after.cpp $(TOOL)String_Tools.h 
	$(CC) $(CFLAGS) -c -o $@ $<



