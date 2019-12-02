//---------------------------------------------------------------------------


#include "stdafx.h"
#pragma comment(lib, "ws2_32.lib")
#include <winsock2.h>
#include <iostream>
#include <stdio.h>
#include <string>
#include <sstream>
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
SOCKET ConnectSocket = INVALID_SOCKET;

const int TLG = 0;
const int TLF = 1;

enum Packet
{
	P_KEY,
	P_AUDIO
};

bool isEnabled = false;
bool isCalling = false;

int currentMod = 2;
int frequency = 30000;

//Number of available devices
int gRecordingDeviceCount = 0;

//Recieved audio spec
SDL_AudioSpec gReceivedRecordingSpec;
SDL_AudioSpec gReceivedPlaybackSpec;

//Recording data buffer
Uint8* gRecordingBuffer = NULL;

//Size of data buffer
Uint32 gBufferByteSize = 0;

//Position in data buffer
Uint32 gBufferBytePosition = 0;

//Maximum position in data buffer for recording
Uint32 gBufferByteMaxPosition = 0;

//Audio device IDs
SDL_AudioDeviceID recordingDeviceId = 0;
SDL_AudioDeviceID playbackDeviceId = 0;

//Maximum number of supported recording devices
const int MAX_RECORDING_DEVICES = 10;

//Maximum recording time
const int MAX_RECORDING_SECONDS = 1;

//Maximum recording time plus padding
const int RECORDING_BUFFER_SECONDS = MAX_RECORDING_SECONDS;

//The various recording actions we can take
enum RecordingState
{
	SETTING_DEVICE,
	START_RECORDING,
	RECORDING,
	START_PLAYBACK,
	PLAYBACK,
	AUDIO_ERROR
};

void audioRecordingCallback( void* userdata, Uint8* stream, int len )
{
	//Copy audio from stream
	memcpy( &gRecordingBuffer[ gBufferBytePosition ], stream, len );

	//Move along buffer
	gBufferBytePosition += len;
}

void audioPlaybackCallback( void* userdata, Uint8* stream, int len )
{
	//Copy audio to stream
	memcpy( stream, &gRecordingBuffer[ gBufferBytePosition ], len );

	//Move along buffer
	gBufferBytePosition += len;
}

bool loadMedia()
{
	//Loading success flag
	bool success = true;

	//Get capture device count
	gRecordingDeviceCount = SDL_GetNumAudioDevices( SDL_TRUE );

	//No recording devices
	if( gRecordingDeviceCount < 1 )
	{
		printf( "Unable to get audio capture device! SDL Error: %s\n", SDL_GetError() );
		success = false;
	}
	else //At least one device connected
	{
		//Cap recording device count
		if( gRecordingDeviceCount > MAX_RECORDING_DEVICES )
		{
			gRecordingDeviceCount = MAX_RECORDING_DEVICES;
		}
	}

	return success;
}

bool ProcessPacket(Packet packettype) {
	int incomFreq;

	switch(packettype) {
		case P_KEY:
		{
			recv(Connection, (char*)&incomFreq, sizeof(int), NULL);

			if (currentMod == 0 && isEnabled && incomFreq == frequency)
			{
				PlaySound(TEXT("SystemStart"), NULL, SND_ASYNC);
				break;
			}
		}
		case P_AUDIO:
		{
			recv(Connection, (char*)&incomFreq, sizeof(int), NULL);

			if (currentMod == 1 && !isCalling  && isEnabled && incomFreq == frequency)
			{
				int msg_size;
				recv(Connection, (char*)&msg_size, sizeof(unsigned long int), NULL);
				recv(Connection, gRecordingBuffer, msg_size, NULL);

				//Main loop flag
				bool quit = false;

				//Go on to next state
				gBufferBytePosition = 0;

				//Start playback
				SDL_PauseAudioDevice( playbackDeviceId, SDL_FALSE );

				while (!quit)
				{
					//Lock callback
					SDL_LockAudioDevice( playbackDeviceId );

					//Finished playback
					if( gBufferBytePosition > gBufferByteMaxPosition )
					{
						//Stop playing audio
						SDL_PauseAudioDevice( playbackDeviceId, SDL_TRUE );

						//Go on to next message
						quit = true;
					}

					//Unlock callback
					SDL_UnlockAudioDevice( playbackDeviceId );
				}

				break;
			}
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
        Sleep(100);
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

int start()
{
	//Main loop flag
	bool quit = false;

	//Set the default recording state
	RecordingState currentState = START_RECORDING;

	//While application is running
	while( !quit )
	{
		//Do current state event handling
		switch( currentState )
		{
			//User getting ready to record
			case START_RECORDING:
			{
				//Go back to beginning of buffer
				gBufferBytePosition = 0;

				//Start recording
				SDL_PauseAudioDevice( recordingDeviceId, SDL_FALSE );

				//Go on to next state
				currentState = RECORDING;
				break;
			}

			case RECORDING:
			{
				//Lock callback
				SDL_LockAudioDevice( recordingDeviceId );

				//Finished recording
				if( gBufferBytePosition > gBufferByteMaxPosition )
				{
					//Stop recording audio
					SDL_PauseAudioDevice( recordingDeviceId, SDL_TRUE );

					//Go on to next state
					gBufferBytePosition = 0;

					//Start playback
					//SDL_PauseAudioDevice( playbackDeviceId, SDL_FALSE );

					if (currentMod == 1 && isCalling && isEnabled)
					{
						Packet packettype = P_AUDIO;

						int msg_size = gBufferByteSize;
						send(Connection, (char*)&packettype, sizeof(Packet), NULL);
						Sleep(10);
						send(Connection, (char*)&frequency, sizeof(int), NULL);
						send(Connection, (char*)&msg_size, sizeof(unsigned long int), NULL);
						Sleep(10);
						send(Connection, gRecordingBuffer, msg_size, NULL);
						Sleep(10);
					}

					//Go on to next state
					currentState = START_RECORDING;
				}

				//Unlock callback
				SDL_UnlockAudioDevice( recordingDeviceId );
				break;
			}


			//Updating playback
			case PLAYBACK:
			{
				//Lock callback
				SDL_LockAudioDevice( playbackDeviceId );

				//Finished playback
				if( gBufferBytePosition > gBufferByteMaxPosition )
				{
					//Stop playing audio
					SDL_PauseAudioDevice( playbackDeviceId, SDL_TRUE );

					//Go on to next state
					currentState = START_RECORDING;
				}

				//Unlock callback
				SDL_UnlockAudioDevice( playbackDeviceId );
				break;
			}
		}
	}
	return 0;
}

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

	if(SDL_Init( SDL_INIT_VIDEO | SDL_INIT_AUDIO ) < 0)
	{
		printf( "Failed to initialize!\n" );
	}
	else
	{
		//Load media
		if( !loadMedia() )
		{
			printf( "Failed to load media!\n" );
		}
		else
		{
			//Main loop flag
			bool quit = false;

			//Get selection index
			int index = 0;

			//Default audio spec
			SDL_AudioSpec desiredRecordingSpec;
			SDL_zero(desiredRecordingSpec);
			desiredRecordingSpec.freq = 4000;
			desiredRecordingSpec.format = AUDIO_F32;
			desiredRecordingSpec.channels = 2;
			desiredRecordingSpec.samples = 500;
			desiredRecordingSpec.callback = audioRecordingCallback;

			//Open recording device
			recordingDeviceId = SDL_OpenAudioDevice( SDL_GetAudioDeviceName( index, SDL_TRUE ), SDL_TRUE, &desiredRecordingSpec, &gReceivedRecordingSpec, SDL_AUDIO_ALLOW_FORMAT_CHANGE );

			//Device failed to open
			if( recordingDeviceId == 0 )
			{
				//Report error
				printf( "Failed to open recording device! SDL Error: %s", SDL_GetError() );
				printf( "Failed to open recording device!");
			}
			//Device opened successfully
			else
			{
				//Default audio spec
				SDL_AudioSpec desiredPlaybackSpec;
				SDL_zero(desiredPlaybackSpec);
				desiredPlaybackSpec.freq = 4000;
				desiredPlaybackSpec.format = AUDIO_F32;
				desiredPlaybackSpec.channels = 2;
				desiredPlaybackSpec.samples = 500;
				desiredPlaybackSpec.callback = audioPlaybackCallback;

				//Open playback device
				playbackDeviceId = SDL_OpenAudioDevice( NULL, SDL_FALSE, &desiredPlaybackSpec, &gReceivedPlaybackSpec, SDL_AUDIO_ALLOW_FORMAT_CHANGE );

				//Device failed to open
				if( playbackDeviceId == 0 )
				{
					//Report error
					printf( "Failed to open playback device! SDL Error: %s", SDL_GetError() );
					printf( "Failed to open playback device!");
				}
				//Device opened successfully
				else
				{
					//Calculate per sample bytes
					int bytesPerSample = gReceivedRecordingSpec.channels * ( SDL_AUDIO_BITSIZE( gReceivedRecordingSpec.format ) / 8 );

					//Calculate bytes per second
					int bytesPerSecond = gReceivedRecordingSpec.freq * bytesPerSample;

					//Calculate buffer size
					gBufferByteSize = RECORDING_BUFFER_SECONDS * bytesPerSecond;

					//Calculate max buffer use
					gBufferByteMaxPosition = MAX_RECORDING_SECONDS * bytesPerSecond;

					//Allocate and initialize byte buffer
					gRecordingBuffer = new Uint8[ gBufferByteSize ];
					memset( gRecordingBuffer, 0, gBufferByteSize );
				}
			}
		}
	}
	CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)start, NULL, NULL, NULL);
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::ButtonClick(TObject *Sender)
{
	if (currentMod == 0 && isEnabled) {
		Packet packettype = P_KEY;
		send(Connection, (char*)&packettype, sizeof(Packet), NULL);
		send(Connection, (char*)&frequency, sizeof(int), NULL);
		Sleep(10);
	}
}

//---------------------------------------------------------------------------

void __fastcall TMainForm::rgModClick(TObject *Sender)
{
	switch (rgMod->ItemIndex)
	{
		case TLG:
		{
			currentMod = TLG;
			break;
		}

		case TLF:
		{
			currentMod = TLF;
			break;
		}
	}
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::cbEnableClick(TObject *Sender)
{
	isEnabled = !isEnabled;
}
//---------------------------------------------------------------------------


void __fastcall TMainForm::CheckBox2Click(TObject *Sender)
{
    isCalling = !isCalling;
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::btnSetFrequencyClick(TObject *Sender)
{
	frequency = cbMTenth->Text.ToInt() * 10000 + cbMOnes->Text.ToInt() * 1000 + cbKHundred->Text.ToInt() * 100 + cbKTenth->Text.ToInt() * 10 + cbKOnes->Text.ToInt() * 1;
}
//---------------------------------------------------------------------------

