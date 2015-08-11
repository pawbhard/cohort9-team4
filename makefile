#The make file to build the project

#Compiler used
COMPILER = gcc

#Compiler Version
VERSION = gnu99

#Required flags for compilation
C_FLAGS = -g -Wall

#Output target for client
CLIENT = multicast_client

#Output target for server
SERVER = multicast_server

#Client and Server compilation
all:$(SERVER) $(CLIENT)

$(CLIENT): src/$(CLIENT).c
	$(COMPILER) -std=$(VERSION) $(C_FLAGS) -pthread -o bin/$(CLIENT) src/$(CLIENT).c src/multicast_util.c

$(SERVER): src/$(SERVER).c
	$(COMPILER) -std=$(VERSION) $(C_FLAGS) -pthread -o bin/$(SERVER) src/$(SERVER).c src/multicast_util.c src/multicast_db.c
