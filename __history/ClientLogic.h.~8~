//---------------------------------------------------------------------------
#include "MainUnit.h"

#ifndef ClientLogicH
#define ClientLogicH
//---------------------------------------------------------------------------
SOCKET Connection;
SOCKET ConnectSocket = INVALID_SOCKET;

const int TLG = 0;
const int TLF = 1;
bool isEnabled = false;
bool isCalling = false;
int currentMod = 2;
int frequency = 30000;

int connectToServer(char *IP, int Port);
bool loadMedia();
bool loadRecordingDevice();
bool loadPlaybackDevice();
void sendTLG();
void initBuffer();
void ClientHandler();
#endif
