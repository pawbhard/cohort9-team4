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

#Common Utils
UTILS = common_util

#Database link
DATABASE = database

#Data Buffer link
DATABUFFER = data_buffer

#Client and Server compilation
all: $(CLIENT) $(SERVER)

$(CLIENT): src/$(CLIENT)/client.cpp
	$(COMPILER) -std=$(STANDARD) $(C_FLAGS) -o bin/$(CLIENT) src/$(CLIENT)/client.cpp  src/$(CLIENT)/client_task_execute.cpp

$(SERVER): src/$(SERVER)/server_main.cpp
	$(COMPILER) -std=$(STANDARD) $(C_FLAGS) -o bin/$(SERVER) src/$(SERVER)/server_main.cpp src/$(SERVER)/server_collect.cpp src/$(SERVER)/server_distribution.cpp src/$(SERVER)/server_payload.cpp src/$(SERVER)/server_register_handler.cpp src/$(SERVER)/threadpool.cpp src/$(SERVER)/update_result.cpp src/$(SERVER)/timer.cpp src/$(DATABASE)/database_main.cpp src/$(DATABASE)/database_track_client.cpp src/$(DATABASE)/database_client_mapping.cpp src/$(DATABASE)/database_client_state.cpp src/$(DATABASE)/database_display.cpp src/$(DATABUFFER)/data_buffer_add_remove.cpp src/$(DATABUFFER)/data_buffer_init.cpp src/$(DATABUFFER)/thread_common.cpp 