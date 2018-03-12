ROOT=$(shell pwd)
CC=gcc
BIN=httpd
SRC=$(shell ls *.c)
OBJ=$(shell echo $(SRC) | sed 's/\.c/\.o/g')
LDFLAGS=-lpthread
FLAGS=-D_STDOUT_
CGI_PATH=$(ROOT)/wwwroot/cgi-bin #$(ROOT)/sql_connect

.PHONY:all
all:$(BIN) cgi 
$(BIN):$(OBJ)
	$(CC) -o $@ $^ $(LDFLAGS) $(FLAGS)
%.c:%.c
	$(CC) -c $< $(FLAGS)
.PHONY:cgi
cgi:
	for name in `echo $(CGI_PATH)`;\
	do\
		cd $$name;\
		make;\
		cd -;\
	done
.PHONY:clean
clean:
	rm -f *.o $(BIN)
	for name in `echo $(CGI_PATH)`;\
	do\
		cd $$name;\
		make clean;\
		cd -;\
	done
.PHONY:output
output:
	mkdir -p output/wwwroot/cgi-bin
	cp -a wwwroot/imag output/wwwroot
	cp -a log output/
	cp -a conf output/
	cp httpd output/
	cp -a wwwroot/index.html output/wwwroot
	for name in `echo $(CGI_PATH);\
	do\
		cd $$name;\
		make output;\
		cd -;\
	done
.PHONY:debug
debug:
	@echo $(SRC)
	@echo $(OBJ)
