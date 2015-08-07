------------------------------------
System programming for cohort9 team4
------------------------------------

Phase 1

Simple server and client program using IPC/Sockets which displays
messages sent from each client periodically. Clients join a group 
on server and display of messages would be per group. [Data is 
sent from client only at this stage].

Usage:

Compiling the code:

$make

Running the multicast server:
$./bin/server <multicast_ipv4_group_address> <port_number>

Running the client:
$./bin/client <multicast_ipv4_group_address> <server_port_number> <client_port_number>