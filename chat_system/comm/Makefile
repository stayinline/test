
ROOT=$(shell pwd)
INCLUDE= -I$(ROOT)/../lib/include
LIB= -L$(ROOT)/../lib/lib

LDFLAGS=-ljsoncpp

test:base_json.cpp data.cpp
	g++ -o $@ $^ $(INCLUDE) $(LIB) $(LDFLAGS) -D_STYLE_
.PHONY:clean
clean:
	rm -f test
