//---------------------------------------------------------------------------

#include "stdafx.h"
#pragma comment(lib, "ws2_32.lib")
#include <winsock2.h>
#include <iostream>

#pragma warning(disable: 4996)
//---------------------------------------------------------------------------
#include <fmx.h>
#pragma hdrstop

#include "InterfaceUnit.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.fmx"
TMainForm *MainForm;
SOCKET Connection;
//---------------------------------------------------------------------------
void ClientHandler() {
	char msg[256];
	while(true) {
		recv(Connection, msg, sizeof(msg), NULL);
		MainForm->Result->Text = 'asd';
	}
}

__fastcall TMainForm::TMainForm(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------

SOCKET ConnectSocket = INVALID_SOCKET;


void __fastcall TMainForm::Button3Click(TObject *Sender)
{
	char msg1[256] = "test";
	send(Connection, msg1, sizeof(msg1), NULL);
}
//---------------------------------------------------------------------------

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
	addr.sin_addr.s_addr = inet_addr("192.168.100.6");
	addr.sin_port = htons(1111);
	addr.sin_family = AF_INET;

	Connection = socket(AF_INET, SOCK_STREAM, NULL);
	if(connect(Connection, (SOCKADDR*)&addr, sizeof(addr)) != 0) {
		std::cout << "Error: failed connect to server.\n";
	}
	std::cout << "Connected!\n";

	CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)ClientHandler, NULL, NULL, NULL);
}
//---------------------------------------------------------------------------

