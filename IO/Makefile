  1 cc=g++
  2 LDFLAGS=-std=c++11
  3 
  4 .PHONY:all
  5 
  6 all:server client
  7 server:epoll_server.cc                                                   
  8     $(cc) -o $@ $^ $(LDFLAGS)
  9 client:_client.cc
 10     $(cc) -o $@ $^ $(LDFLAGS)
 11 
 12 .PHONY:clean
 13 clean:
 14     rm -f client server
 15 
