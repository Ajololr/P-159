#include "stdafx.h"
#pragma comment(lib, "ws2_32.lib")
#include <winsock2.h>
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <iostream>
#include <string>
#include <sstream> 
#include <vector> 

#pragma warning(disable: 4996)

SOCKET Connections[100];
int Counter = 0;

using namespace std;

enum Packet {
	P_KEY,
	P_AUDIO
};

void sendTelegraph(int index) {
	int frequency;

	recv(Connections[index], (char*)& frequency, sizeof(int), NULL);
	for (int i = 0; i < Counter; i++) {
		if (i == index) {
			continue;
		}

		Packet msgtype = P_KEY;
		send(Connections[i], (char*)& msgtype, sizeof(Packet), NULL);
		send(Connections[i], (char*)& frequency, sizeof(int), NULL);
	}
}

void sendAudio(int index) {
	int msg_size;
	int frequency;

	recv(Connections[index], (char*)& frequency, sizeof(int), NULL);
	recv(Connections[index], (char*)& msg_size, sizeof(unsigned long int), NULL);
	vector<char> msg(msg_size + 1, 0);
	recv(Connections[index], msg.data(), msg_size, NULL);
	for (int i = 0; i < Counter; i++) {
		if (i == index) {
			continue;
		}

		Packet msgtype = P_AUDIO;
		send(Connections[i], (char*)& msgtype, sizeof(Packet), NULL);
		send(Connections[i], (char*)& frequency, sizeof(int), NULL);
		send(Connections[i], (char*)& msg_size, sizeof(unsigned long int), NULL);
		send(Connections[i], msg.data(), msg_size, NULL);
		Sleep(900);
	}
}

bool ProcessPacket(int index, Packet packettype) {
	switch (packettype) {
	case P_KEY:
	{
		sendTelegraph(index);
		break;
	}
	case P_AUDIO:
	{
		sendAudio(index);
		break;
	}
	default:
		cout << "Нераспознанный пакет: " << packettype << endl;
		break;
	}

	return true;
}


void ClientHandler(int index) {
	Packet packettype;
	while (true) {
		recv(Connections[index], (char*)& packettype, sizeof(Packet), NULL);

		if (!ProcessPacket(index, packettype)) {
			break;
		}
	}
	closesocket(Connections[index]);
}

int main(int argc, char* argv[]) {
	setlocale(LC_ALL, "Russian");
	char valueIP[15];
	int portValue;

	WSAData wsaData;
	WORD DLLVersion = MAKEWORD(2, 1);
	if(WSAStartup(DLLVersion, &wsaData) != 0) {
		cout << "Ошибка инициализации WSA!" << endl;
		exit(1);
	}

	SOCKADDR_IN addr;
	int sizeofaddr = sizeof(addr);
	cout << "Введите IP:" << endl;
	cin >> valueIP;
	addr.sin_addr.s_addr = inet_addr(valueIP);
	cout << "Введите порт:" << endl;
	cin >> portValue;
	addr.sin_port = htons(portValue);
	addr.sin_family = AF_INET;

	SOCKET sListen = socket(AF_INET, SOCK_STREAM, NULL);
	bind(sListen, (SOCKADDR*)&addr, sizeof(addr));
	listen(sListen, SOMAXCONN);

	SOCKET newConnection;
	for(int i = 0; i < 100; i++) {
		newConnection = accept(sListen, (SOCKADDR*)&addr, &sizeofaddr);
		if(newConnection == 0) {
			cout << "Ошибка подключения клиента.";
		} else {
			cout << "Клиент подключен!";
			Connections[i] = newConnection;
			Counter++;
			CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)ClientHandler, (LPVOID)(i), NULL, NULL);
		}
	}

	system("pause");
	return 0;
}