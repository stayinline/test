ROOT=$(shell pwd)
SERVER=$(ROOT)/server
CLIENT=$(ROOT)/client
DATA_POOL=$(ROOT)/data_pool
COMM=$(ROOT)/comm
LIB=$(ROOT)/lib
WINDOW=$(ROOT)/window

CC=g++
LDFLAGS=-lpthread -ljsoncpp

INCLUDE=-I $(DATA_POOL) -I $(COMM) -I $(LIB)/include -I $(WINDOW)
LIB_PATH=-L $(LIB)/lib

server_bin=chat_system
client_bin=chat_client

SERVER_SRC=$(shell ls $(SERVER)/*.cpp)
SERVER_SRC+=$(shell ls $(DATA_POOL)/*.cpp)
SERVER_SRC+=$(shell ls $(COMM)/*.cpp)
SERVER_OBJ=$(notdir $(SERVER_SRC:.cpp=.o))
CLIENT_SRC=$(shell ls $(CLIENT)/*.cpp)
CLIENT_SRC+=$(shell ls $(COMM)/*.cpp)
CLIENT_SRC+=$(shell ls $(WINDOW)/*.cpp)
CLIENT_OBJ=$(notdir $(CLIENT_SRC:.cpp=.o))

.PHONY:all
all:$(server_bin) $(client_bin)

$(server_bin):$(SERVER_OBJ)
	@$(CC) -o $@ $^ $(LDFLAGS) $(LIB_PATH)
	@echo "Linking [$^] to [$@] .. done"
$(client_bin):$(CLIENT_OBJ)
	@$(CC) -o $@ $^ $(LDFLAGS) $(LIB_PATH) -lncurses -static
	@echo "Linking [$^] to [$@] .. done"
%.o:$(SERVER)/%.cpp
	@$(CC) -c $< $(INCLUDE)
	@echo "Compling [$<] to [$@] ... done"
%.o:$(CLIENT)/%.cpp
	@$(CC) -c $< $(INCLUDE)
	@echo "Compling [$<] to [$@] ... done"
%.o:$(DATA_POOL)/%.cpp
	@$(CC) -c $<
	@echo "Compling [$<] to [$@] ... done"
%.o:$(COMM)/%.cpp
	@$(CC) -c $< $(INCLUDE)
	@echo "Compling [$<] to [$@] ... done"
%.o:$(WINDOW)/%.cpp
	@$(CC) -c $< $(INCLUDE)
	@echo "Compling [$<] to [$@] ... done"

.PHONY:clean
clean:
	@rm -rf $(server_bin) $(client_bin) *.o output

.PHONY:output
output:
	mkdir -p output/server
	mkdir -p output/client
	cp log output/server -a
	cp conf output/server -a
	cp plugin/server_ctl.sh output/server
	cp $(server_bin) output/server
	cp $(client_bin) output/client


.PHONY:debug
debug:
	@echo $(SERVER_OBJ)
	@echo $(CLIENT_OBJ)







