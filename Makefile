CXX = g++
CXX_FLAGS = -Wall -Wpedantic -std=c++20
CPP_FLAGS = -I ./lua-5.4.7/src
LNK_FLAGS_LUA = -L ./lua-5.4.7/src -llua
EXECUTABLE = demo

all: lua-5.4.7 demo

lua-5.4.7:
	wget https://www.lua.org/ftp/lua-5.4.7.tar.gz
	tar xzvf lua-5.4.7.tar.gz
	make -C lua-5.4.7

demo:
	$(CXX) demo.cpp -o demo $(CXX_FLAGS) $(LNK_FLAGS_LUA)

.PHONY: clean
clean:
	rm -f *.o *.a *.so $(EXECUTABLE)

.PHONY: dev
dev:
	while inotifywait demo.cpp ; do clear; make clean ; make ; ./demo ; done
