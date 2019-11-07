//---------------------------------------------------------------------------

#include "stdafx.h"
#pragma comment(lib, "ws2_32.lib")
#include <winsock2.h>
#include <iostream>

#pragma warning(disable: 4996)
//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "MainUnit.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TMainForm *MainForm;
//---------------------------------------------------------------------------
SOCKET Connection;

enum Packet
{
	P_Key
};

bool ProcessPacket(Packet packettype) {
	switch(packettype) {
		case P_Key:
		{
			PlaySound(TEXT("SystemStart"), NULL, SND_ASYNC);
			break;
		}
		default:
			std::cout << "Unrecognized packet: " << packettype << std::endl;
			break;
	}

	return true;
}

void ClientHandler() {
	Packet packettype;
	while(true) {
		recv(Connection, (char*)&packettype, sizeof(Packet), NULL);

		if(!ProcessPacket(packettype)) {
			break;
		}
	}
	closesocket(Connection);
}

__fastcall TMainForm::TMainForm(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------

SOCKET ConnectSocket = INVALID_SOCKET;

void __fastcall TMainForm::FormCreate(TObject *Sender)
{
	WSAData wsaData;
	WORD DLLVersion = MAKEWORD(2, 1);
	if(WSAStartup(DLLVersion, &wsaData) != 0) {
		std::cout << "Error" << std::endl;
		exit(1);
	}

	SOCKADDR_IN addr;
	int sizeofaddr = sizeof(addr);
	addr.sin_addr.s_addr = inet_addr("192.168.100.3");
	addr.sin_port = htons(4980);
	addr.sin_family = AF_INET;

	Connection = socket(AF_INET, SOCK_STREAM, NULL);
	if(connect(Connection, (SOCKADDR*)&addr, sizeof(addr)) != 0) {
		std::cout << "Error: failed connect to server.\n";
	}
	std::cout << "Connected!\n";

	CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)ClientHandler, NULL, NULL, NULL);
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::ButtonClick(TObject *Sender)
{
	Packet packettype = P_Key;
	send(Connection, (char*)&packettype, sizeof(Packet), NULL);
	Sleep(10);
}

//---------------------------------------------------------------------------