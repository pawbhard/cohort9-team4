#include "../../include/utils.h"
using namespace std;

int main(int argc, char *argv[]) {
	
	int server_sock, client_sock;
	struct sockaddr_in servaddr,client_addr;
	socklen_t client_len;
	int server_port_no, i;
	char data[BUFFER_SIZE];
	bool groups[5] = {0};
	char group_list[100], group_number[5];

	if (argc!=2) {
		cout << get_date_time() << "Enter the server port number\n";
		return 1;
	}

	server_port_no = atoi(argv[1]);
	server_sock = socket(AF_INET,SOCK_STREAM,0);

	bzero(&servaddr,sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = htons(server_port_no);
	bind(server_sock,(struct sockaddr *)&servaddr,sizeof(servaddr));
	cout << get_date_time() << " Waiting for clients\n";
	listen(server_sock,1024);

	client_len = sizeof(client_addr);
   	client_sock = accept(server_sock,(struct sockaddr *)&client_addr,&client_len);
   	if (!client_sock) {
   		cout << get_date_time() << " No client connection\n ";
   		return 1;
   	}
   	cout << get_date_time() << " Client connected\n";
   	while (1) {
   		strcpy(group_list, "");
   		cout << get_date_time() << " Waiting for data from client...\n";
   		read (client_sock, data, BUFFER_SIZE);
   		if (!strcmp(data,"")) {
   			cout << get_date_time() << " Invalid Data\n";
   			continue;
   		}
   		cout << get_date_time() << " Received from client : " << data << endl;
   		if (!strcmp(data,"Requesting list of available groups")) {
   			for (i=0;i<5;i++) {
   				if (!groups[i]) {
   					snprintf(group_number,4,"%d ",i+1);
   					strcat(group_list,group_number);
   				}
   			}
   			cout << get_date_time() << " List of available groups : " << group_list << endl;
   			write (client_sock, group_list, sizeof(group_list));
   		} else if (!strcmp(data,"Requesting list of registered groups")) {
   			for (i=0;i<5;i++) {
   				if (groups[i]) {
   					snprintf(group_number,4,"%d ",i+1);
   					strcat(group_list,group_number);
   				}
   			}
   			cout << get_date_time() << " List of available groups : " << group_list << endl;
   			write (client_sock, group_list, sizeof(group_list));
   		}
   		strcpy(data,"");
   	}
   	close(client_sock);
   	close(server_sock);
   	return 0;
}