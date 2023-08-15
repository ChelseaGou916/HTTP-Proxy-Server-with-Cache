all: main

main: main.cpp helper.h proxy.cpp proxy.h request.cpp request.h response.cpp response.h cache.cpp cache.h
	g++ --std=c++11 -g  -o main main.cpp helper.h proxy.cpp proxy.h request.cpp request.h response.cpp response.h cache.cpp cache.h -lpthread

.PHONY:
	clean
clean:
	rm -rf *.o main *~ *#