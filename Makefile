SRC=src/plugin.cpp src/model.cpp src/ietf/ietf_interfaces.cpp
LIBS=-lsysrepo -lvom #dependencies library
OUT=libcomb.so
WARN=-Wall -Wextra -std=c++11

all:
	g++ $(WARN) $(SRC) -o $(OUT) -fpic -shared $(LIBS)

install:
	cp $(OUT) /usr/lib/x86_64-linux-gnu/sysrepo/plugins

clean:
	rm $(OUT)
