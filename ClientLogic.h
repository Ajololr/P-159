//---------------------------------------------------------------------------
#include "MainUnit.h"

#ifndef ClientLogicH
#define ClientLogicH
//---------------------------------------------------------------------------
SOCKET Connection;
SOCKET ConnectSocket = INVALID_SOCKET;

const int TLG = 0;
const int TLF = 1;
int isEnabled = 0;
int isCalling = 0;
int currentMod = 2;
int frequency = 30000;

int connectToServer(char *IP, int Port);
bool loadMedia();
bool loadRecordingDevice();
bool loadPlaybackDevice();
void sendTLG();
void initBuffer();
void ClientHandler();
void toggleEnabled();
void toggleCalling();
void toggleMod(int mod);
#endif
