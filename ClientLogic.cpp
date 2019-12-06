//---------------------------------------------------------------------------
#include "stdafx.h"
#pragma comment(lib, "ws2_32.lib")
#include <winsock2.h>
#include <iostream>
#include <stdio.h>
#include <string>
#include <sstream>
#pragma warning(disable: 4996)
#pragma hdrstop

#include "ClientLogic.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//Number of available devices

enum Packet
{
	P_KEY,
	P_AUDIO
};

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

void audioPlaybackCallback( void* userdata, Uint8* stream, int len)
{
	//Copy audio to stream
	memcpy( stream, &gRecordingBuffer[ gBufferBytePosition ], len );

	//Move along buffer
	gBufferBytePosition += len;
}

bool loadRecordingDevice()
{
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
	return recordingDeviceId == 0;
}

bool loadPlaybackDevice()
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
	return playbackDeviceId == 0;
}

void sendAudio()
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

void recording()
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
			sendAudio();
		}
	}

	//Unlock callback
	SDL_UnlockAudioDevice( recordingDeviceId );
}

void startRecording()
{
	//Go back to beginning of buffer
	gBufferBytePosition = 0;

	//Start recording
	SDL_PauseAudioDevice( recordingDeviceId, SDL_FALSE );
}

int initRecording()
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
				startRecording();
				//Go on to next state
				currentState = RECORDING;
				break;
			}

			case RECORDING:
			{
				recording();
				//Go on to next state
				currentState = START_RECORDING;
				break;
			}
		}
	}
	return 0;
}

void initBuffer()
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

	CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)startRecording, NULL, NULL, NULL);
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
		MessageDlg("Unable to get audio capture device!", mtError, TMsgDlgButtons()<<mbOK, 0);
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

void playAudio()
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
				playAudio();
				break;
			}
		}
		default:
			break;
	}

	return true;
}

void sendTLG()
{
	if (currentMod == 0 && isEnabled) {
		Packet packettype = P_KEY;
		send(Connection, (char*)&packettype, sizeof(Packet), NULL);
		send(Connection, (char*)&frequency, sizeof(int), NULL);
		Sleep(10);
	}
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

int connectToServer(char *IP, int Port)
{
	WSAData wsaData;
	WORD DLLVersion = MAKEWORD(2, 1);
	if(WSAStartup(DLLVersion, &wsaData) != 0)
	{
		//MessageDlg("Error", mtInformation, TMsgDlgButtons() << mbOK, 0);
	}

	SOCKADDR_IN addr;
	int sizeofaddr = sizeof(addr);
	addr.sin_addr.s_addr = inet_addr(IP);
	addr.sin_port = htons(Port);
	addr.sin_family = AF_INET;

	Connection = socket(AF_INET, SOCK_STREAM, NULL);
	if (connect(Connection, (SOCKADDR*)&addr, sizeof(addr)) != 0)
	{
		//MessageDlg("Error", mtInformation, TMsgDlgButtons() << mbOK, 0);
		exit(1);
	}

	CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)ClientHandler, NULL, NULL, NULL);
}

