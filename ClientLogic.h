//---------------------------------------------------------------------------
#include "MainUnit.h"

#ifndef ClientLogicH
#define ClientLogicH
//---------------------------------------------------------------------------
SOCKET Connection;
SOCKET ConnectSocket = INVALID_SOCKET;

const int TLG = 0;
const int TLF = 1;

int connectToServer(char *IP, int Port);
bool loadMedia();
bool loadRecordingDevice();
bool loadPlaybackDevice();
void sendTLG();
void initBuffer();
void clientHandler();
void toggleEnabled();
void toggleCalling();
void toggleMod(int mod);
void setFrequency(int MTenth, int MOnes, int KHundred, int KTenth, int KOnes);

#endif
