CXX = g++
CXX_FLAGS = -Wall -Wpedantic -std=c++20
CPP_FLAGS = -I ./lua-5.4.7/src
LNK_FLAGS_LUA = -L ./lua-5.4.7/src -llua
EXECUTABLE = demo

#all: sandbox.o lua_wrapper.so
#	$(CXX) *.o  -o $(EXECUTABLE) $(LNK_FLAGS_WRAPPER)

#sandbox.o: ./src/sandbox.cpp
#	$(CXX) -c ./src/sandbox.cpp -o $(EXECUTABLE).o $(CXX_FLAGS)

#lua_wrapper.so: lua_wrapper.o
#	$(CXX) -shared -o libluawrapper.so lua_wrapper.o  (LNK_FLAGS_LUA)

#lua_wrapper.o: ./src/lua_wrapper.cpp
#	$(CXX) -fPIC -c ./src/lua_wrapper.cpp $(CXX_FLAGS) 

all: demo

demo:
	$(CXX) demo.cpp -o demo $(CXX_FLAGS) $(LNK_FLAGS_LUA)

clean:
	rm -f *.o *.a *.so $(EXECUTABLE)

dev:
	while inotifywait demo.cpp ; do clear; make clean ; make ; ./demo ; done
