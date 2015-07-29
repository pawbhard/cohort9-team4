#The make file to build the project

#Compiler used
COMPILER = gcc

#Compiler Version
VERSION = c99

#Required flags for compilation
C_FLAGS = -g -Wall

#Output target for client
CLIENT = client

#Output target for server
SERVER = server

#Client and Server compilation
all: $(SERVER)

#$(CLIENT): src/$(CLIENT).c
#	$(COMPILER) -std=$(VERSION) $(C_FLAGS) -o bin/$(CLIENT) src/$(CLIENT).c

$(SERVER): src/$(SERVER).c
	$(COMPILER) -std=$(VERSION) $(C_FLAGS) -o bin/$(SERVER) src/$(SERVER).c src/utils.c