#The make file to build the project

#Compiler used
COMPILER = g++

#C++ standard used
STANDARD = gnu++11

#Required flags for compilation
C_FLAGS = -g -Wall -pthread

#Output target for client
CLIENT = client

#Output target for server
SERVER = server

STUB_SERVER = stub_server

UTILS = common_util

#Client and Server compilation
all: $(CLIENT) $(STUB_SERVER)

$(CLIENT): src/$(CLIENT)/client_infra.cpp
	$(COMPILER) -std=$(STANDARD) $(C_FLAGS) -o bin/$(CLIENT) src/$(CLIENT)/client_infra.cpp src/$(CLIENT)/client_leave_join_group.cpp src/$(CLIENT)/client_connection.cpp src/$(CLIENT)/client_task_execute.cpp

$(STUB_SERVER): src/$(CLIENT)/$(STUB_SERVER).cpp
	$(COMPILER) -std=$(STANDARD) $(C_FLAGS) -o bin/$(STUB_SERVER) src/$(CLIENT)/$(STUB_SERVER).cpp	