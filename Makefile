SRC=src/plugin.cpp
LIBS=-lsysrepo -lvom #dependencies library
OUT=libcomb.so

all:
	g++ $(SRC) -o $(OUT) -fpic -shared $(LIBS)

install:
	cp $(OUT) /usr/lib/x86_64-linux-gnu/sysrepo/plugins

clean:
	rm $(OUT)
