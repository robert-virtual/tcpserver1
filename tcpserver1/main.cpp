#include <iostream>
#include <WS2tcpip.h>
#pragma comment (lib,"ws2_32.lib")


using namespace std;


void main() {
	// inicializar winsock
	WSADATA wsData;
	WORD ver = MAKEWORD(2, 2);
	int wsOk = WSAStartup(ver,&wsData);
	if (wsOk != 0)
	{
		cerr << "no se pudo inicializar winsock" << endl;
		return;
	}


	// create a win socket
	// socket(AF = address family,type,protocol)
	// 
	SOCKET listening = socket(AF_INET,SOCK_STREAM,0);
	if (listening == INVALID_SOCKET)
	{
		cerr << "no se pudo crear el socket " << endl;
		return;

	}
	// bind  ip address and port to a socket
	sockaddr_in hint;
	hint.sin_family = AF_INET; // ipv4
	hint.sin_port = htons(54000); // host to network short
	hint.sin_addr.S_un.S_addr = INADDR_ANY; // colud also use inet_pton
	bind(listening, (sockaddr*)&hint, sizeof(hint));

	// tell the socket iis for listening 
	listen(listening, SOMAXCONN);
	
	// wait for connections
	sockaddr_in client;
	int clientSize = sizeof(client);
	SOCKET clientSocket = accept(listening, (sockaddr*)&client,&clientSize);
	if (clientSocket == INVALID_SOCKET)
	{
		cerr << "no se pudo crear el socket cliente " << endl;
		return;
	}
	char host[NI_MAXHOST]; // nombre remoto del cliente 
	char service[NI_MAXSERV]; // servicio al cual es cliente esta conectado
	
	ZeroMemory(host, NI_MAXHOST); // same as memset(host,0,NI_MAXHOST)
	ZeroMemory(service, NI_MAXSERV);

	if (getnameinfo((sockaddr*)&client,clientSize,host,NI_MAXHOST,service,NI_MAXSERV,0) == 0)
	{
		cout << host << " connected on port " << service << endl;
	}
	else {
		inet_ntop(AF_INET,&client.sin_addr,host,NI_MAXHOST);
		cout << host << " connected on port " << ntohs(client.sin_port) << endl;

	}
	// close listener socket
	closesocket(listening);
	// while loop accesp and echo message to client
	char buf[4096];
	while (true)
	{
		ZeroMemory(buf,4096);
		// esperar q los clientes envien datos
		int bytesRecived = recv(clientSocket,buf,4096,0);
		if (bytesRecived == SOCKET_ERROR)
		{	
			cerr << "Error al recibir bytes del cliente" << endl;
			break;
		}
		if (bytesRecived == 0 )
		{

			cerr << "Cliente desconectado" << endl;
			
		}
		// responser con un mensaje al cliente	
		send(clientSocket,buf,bytesRecived+1,0);

	}
	  
	// close a win socket 
	closesocket(clientSocket);
	// cleanup winsock
	WSACleanup();
}