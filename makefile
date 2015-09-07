#The make file to build the project

#Compiler used
COMPILER = g++

#Required flags for compilation
C_FLAGS = -g -Wall

#Output target for client
CLIENT = client

#Output target for server
SERVER = server

UTILS = common_util

#Client and Server compilation
all: $(CLIENT)

$(CLIENT): src/$(CLIENT)/client_infra.cpp
	$(COMPILER) $(C_FLAGS) -o bin/$(CLIENT) src/$(CLIENT)/client_infra.cpp src/$(UTILS)/utils.cpp
