#include "stdafx.h"
#pragma comment(lib, "ws2_32.lib")
#include <winsock2.h>
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <iostream>
#include <string>
#include <sstream> 
#include <vector> 

#pragma warning(disable: 4996)

SOCKET connections[100];
int counter = 0;

using namespace std;

enum Packet 
{
	P_KEY,
	P_AUDIO
};

bool sendTelegraph(int index) 
{
	int frequency;

	try
	{
		recv(connections[index], (char*)& frequency, sizeof(int), NULL);
	}
	catch (...)
	{
		return false;
	}
	for (int i = 0; i < counter; i++) 
	{
		if (i == index) 
		{
			continue;
		}

		Packet msgtype = P_KEY;
		send(connections[i], (char*)& msgtype, sizeof(Packet), NULL);
		send(connections[i], (char*)& frequency, sizeof(int), NULL);
	}
	return true;
}

bool sendAudio(int index) 
{
	int msg_size;
	int frequency;

	try
	{
		recv(connections[index], (char*)& frequency, sizeof(int), NULL);
		recv(connections[index], (char*)& msg_size, sizeof(unsigned long int), NULL);
	}
	catch(...)
	{
		return false;
	}
	vector<char> msg(msg_size + 1, 0);
	try
	{
		recv(connections[index], msg.data(), msg_size, NULL);
	}
	catch (...)
	{
		return false;
	}
	for (int i = 0; i < counter; i++) 
	{
		if (i == index) 
		{
			continue;
		}

		Packet msgtype = P_AUDIO;
		send(connections[i], (char*)& msgtype, sizeof(Packet), NULL);
		send(connections[i], (char*)& frequency, sizeof(int), NULL);
		send(connections[i], (char*)& msg_size, sizeof(unsigned long int), NULL);
		send(connections[i], msg.data(), msg_size, NULL);
		Sleep(900);
	}
	return true;
}

bool processPacket(int index, Packet packettype) 
{
	switch (packettype) 
	{
		case P_KEY:
		{
			return sendTelegraph(index);
		}
		case P_AUDIO:
		{
			return sendAudio(index);
		}
		default:
		{
			cout << "Нераспознанный пакет: " << packettype << endl;
			return false;
		}
	}
}

void сlientHandler(int index) 
{
	Packet packettype;
	while (true) 
	{
		recv(connections[index], (char*)& packettype, sizeof(Packet), NULL);

		if (!processPacket(index, packettype)) 
		{
			cout << "Ошибка передачи данных. Разрыв соединения." << endl;
			break;
		}
	}
	closesocket(connections[index]);
}

bool checkParams(int argc, char* argv[])
{
	for (int i = 0; i < argc; i++)
	{
		if (!strcmp(argv[i], "/?"))
		{
			cout << "Для старта сервера передайте первыми двумя параметрами данные IP и порта.";
			return false;
		}
	}

	if (argc < 3)
	{
		cout << "Ощибка. Введите два параметра.";
		return false;
	}
	return true;
}

bool initWSA()
{
	WSAData wsaData;
	WORD DLLVersion = MAKEWORD(2, 1);
	if (WSAStartup(DLLVersion, &wsaData) != 0)
	{
		cout << "Ошибка инициализации WSA!" << endl;
		return false;
	}
	return true;
}

int main(int argc, char* argv[]) 
{
	setlocale(LC_ALL, "Russian");
	if (!checkParams(argc, argv) || !initWSA())
	{
		return 0;
	}

	SOCKADDR_IN addr;
	int sizeofaddr = sizeof(addr);
	try
	{
		addr.sin_addr.s_addr = inet_addr(argv[1]);
		addr.sin_port = htons(stoi(argv[2]));
	}
	catch (...)
	{
		cout << "Ошибка. Введите корректные данные IP и порта." << endl;
		return 0;
	}
	addr.sin_family = AF_INET;

	SOCKET sListen = socket(AF_INET, SOCK_STREAM, NULL);
	bind(sListen, (SOCKADDR*)&addr, sizeof(addr));
	listen(sListen, SOMAXCONN);

	SOCKET newConnection;
	for(int i = 0; i < 100; i++) 
	{
		newConnection = accept(sListen, (SOCKADDR*)&addr, &sizeofaddr);
		if(newConnection == 0) 
		{
			cout << "Ошибка подключения клиента.";
		} 
		else 
		{
			cout << "Клиент подключен!\n";
			connections[i] = newConnection;
			counter++;
			CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)сlientHandler, (LPVOID)(i), NULL, NULL);
		}
	}

	system("pause");
	return 0;
}