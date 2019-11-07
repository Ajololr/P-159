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

enum Packet
{
	P_Key,
    P_Sound
};

bool ProcessPacket(Packet packettype) {
	switch(packettype) {
		case P_Key:
		{
			PlaySound(TEXT("SystemStart"), NULL, SND_ASYNC);
			break;
		}
		case P_Sound:
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

//Maximum number of supported recording devices
const int MAX_RECORDING_DEVICES = 10;

//Maximum recording time
const int MAX_RECORDING_SECONDS = 1;

//Maximum recording time plus padding
const int RECORDING_BUFFER_SECONDS = MAX_RECORDING_SECONDS + 1;

//The various recording actions we can take
enum RecordingState
{
	SELECTING_DEVICE,
	STOPPED,
	RECORDING,
	RECORDED,
	PLAYBACK,
	AUDIO_ERROR
};

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

int start()
{
	//Start up SDL and create window
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

			//Set the default recording state
			RecordingState currentState = SELECTING_DEVICE;

			//Audio device IDs
			SDL_AudioDeviceID recordingDeviceId = 0;
			SDL_AudioDeviceID playbackDeviceId = 0;

			//While application is running
			while( !quit )
			{
				//Do current state event handling
				switch( currentState )
				{
					//User is selecting recording device
					case SELECTING_DEVICE:

						//On key press
						if( true )
						{
							//Handle key press from 0 to 9
							if(true )
							{
								//Get selection index
								int index = 1;

								//Index is valid
								if( index < gRecordingDeviceCount )
								{
									//Default audio spec
									SDL_AudioSpec desiredRecordingSpec;
									SDL_zero(desiredRecordingSpec);
									desiredRecordingSpec.freq = 44100;
									desiredRecordingSpec.format = AUDIO_F32;
									desiredRecordingSpec.channels = 2;
									desiredRecordingSpec.samples = 4096;
									desiredRecordingSpec.callback = audioRecordingCallback;

									//Open recording device
									recordingDeviceId = SDL_OpenAudioDevice( SDL_GetAudioDeviceName( index, SDL_TRUE ), SDL_TRUE, &desiredRecordingSpec, &gReceivedRecordingSpec, SDL_AUDIO_ALLOW_FORMAT_CHANGE );

									//Device failed to open
									if( recordingDeviceId == 0 )
									{
										//Report error
										printf( "Failed to open recording device! SDL Error: %s", SDL_GetError() );
										printf( "Failed to open recording device!");
										currentState = ERROR;
									}
									//Device opened successfully
									else
									{
										//Default audio spec
										SDL_AudioSpec desiredPlaybackSpec;
										SDL_zero(desiredPlaybackSpec);
										desiredPlaybackSpec.freq = 44100;
										desiredPlaybackSpec.format = AUDIO_F32;
										desiredPlaybackSpec.channels = 2;
										desiredPlaybackSpec.samples = 4096;
										desiredPlaybackSpec.callback = audioPlaybackCallback;

										//Open playback device
										playbackDeviceId = SDL_OpenAudioDevice( NULL, SDL_FALSE, &desiredPlaybackSpec, &gReceivedPlaybackSpec, SDL_AUDIO_ALLOW_FORMAT_CHANGE );

										//Device failed to open
										if( playbackDeviceId == 0 )
										{
											//Report error
											printf( "Failed to open playback device! SDL Error: %s", SDL_GetError() );
											printf( "Failed to open playback device!");
											currentState = ERROR;
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

											//Go on to next state
											printf("Press 1 to record for 5 seconds.");
											currentState = STOPPED;
										}
									}
								}
							}
						}
						break;

					//User getting ready to record
					case STOPPED:

						//On key press
						if( true )
						{
							//Start recording
							if( true )
							{
								//Go back to beginning of buffer
								gBufferBytePosition = 0;

								//Start recording
								SDL_PauseAudioDevice( recordingDeviceId, SDL_FALSE );

								//Go on to next state
								printf( "Recording...");
								currentState = RECORDING;
							}
						}
						break;

					//User has finished recording
					case RECORDED:

						//On key press
						if( true )
						{
							//Start playback
							if( true )
							{
								//Reset the buffer
								gBufferBytePosition = 0;
								memset( gRecordingBuffer, 0, gBufferByteSize );

								//Start recording
								SDL_PauseAudioDevice( recordingDeviceId, SDL_FALSE );

								//Go on to next state
								printf( "Recording...");
								currentState = RECORDING;
							}
							//Record again
							if( false )
							{
								//Reset the buffer
								gBufferBytePosition = 0;
								memset( gRecordingBuffer, 0, gBufferByteSize );

								//Start recording
								SDL_PauseAudioDevice( recordingDeviceId, SDL_FALSE );

								//Go on to next state
								printf( "Recording...");
								currentState = SELECTING_DEVICE;
							}
						}
						break;
				}

				//Updating recording
				if( currentState == RECORDING )
				{
					//Lock callback
					SDL_LockAudioDevice( recordingDeviceId );

					//Finished recording
					if( gBufferBytePosition > gBufferByteMaxPosition )
					{
						//Stop recording audio
						SDL_PauseAudioDevice( recordingDeviceId, SDL_TRUE );

						//Go on to next state
						printf( "Press 1 to play back. Press 2 to record again.");
						gBufferBytePosition = 0;

						//Start playback
						SDL_PauseAudioDevice( playbackDeviceId, SDL_FALSE );

						//Go on to next state
						printf( "Playing...");
						currentState = PLAYBACK;
					}

					//Unlock callback
					SDL_UnlockAudioDevice( recordingDeviceId );
				}
				//Updating playback
				else if( currentState == PLAYBACK )
				{
					//Lock callback
					SDL_LockAudioDevice( playbackDeviceId );

					//Finished playback
					if( gBufferBytePosition > gBufferByteMaxPosition )
					{
						//Stop playing audio
						SDL_PauseAudioDevice( playbackDeviceId, SDL_TRUE );

						//Go on to next state
						printf( "Press 1 to play back. Press 2 to record again.");
						currentState = STOPPED;
					}

					//Unlock callback
					SDL_UnlockAudioDevice( playbackDeviceId );
				}
			}
		}
	}

	return 0;
}

void __fastcall TMainForm::Button1Click(TObject *Sender)
{
	start();
}
//---------------------------------------------------------------------------

