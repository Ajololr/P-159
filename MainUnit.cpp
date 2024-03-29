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

__fastcall TMainForm::TMainForm(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::FormCreate(TObject *Sender)
{
	if(SDL_Init( SDL_INIT_VIDEO | SDL_INIT_AUDIO ) < 0)
	{
		MessageDlg("������ ������������� SDL!", mtError, TMsgDlgButtons()<<mbOK, 0);
	}
	else
	{
		//Load media
		if( !loadMedia() )
		{
			MessageDlg("�� ������� ���������� ���������������!", mtError, TMsgDlgButtons()<<mbOK, 0);
		}
		else
		{
			//Device failed to open
			if( loadRecordingDevice() )
			{
				//Report error
				MessageDlg("�� ������� ���������� � ������������� ����������!", mtError, TMsgDlgButtons()<<mbOK, 0);
			}
			//Device opened successfully
			else
			{
				//Device failed to open
				if( loadPlaybackDevice()  )
				{
					//Report error
					MessageDlg("�� ������� ���������� � ���������������� ����������!", mtInformation, TMsgDlgButtons() << mbOK, 0);
				}
				//Device opened successfully
				else
				{
					initBuffer();
				}
			}
		}
	}
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::ButtonClick(TObject *Sender)
{
    sendTLG();
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::rgModClick(TObject *Sender)
{
	switch (rgMod->ItemIndex)
	{
		case TLG:
		{
			toggleMod(TLG);
			break;
		}

		case TLF:
		{
			toggleMod(TLF);
			break;
		}
	}
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::cbEnableClick(TObject *Sender)
{
	toggleEnabled();
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::CheckBox2Click(TObject *Sender)
{
    toggleCalling();
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::btnSetFrequencyClick(TObject *Sender)
{
	setFrequency(cbMTenth->Text.ToInt(), cbMOnes->Text.ToInt(), cbKHundred->Text.ToInt(), cbKTenth->Text.ToInt(), cbKOnes->Text.ToInt());
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::ConnectBtnClick(TObject *Sender)
{
	connectToServer(AnsiString(IPEdt->Text).c_str(), PortEdt->Text.ToInt());
}
//---------------------------------------------------------------------------


void __fastcall TMainForm::HelpClick(TObject *Sender)
{
	MessageDlg("��������� ���������:\n� checkBox ���� � ����������� ���������;\n� checkBox ������ � ����������� ���������.\n������� ���������:\n2. � ���� IP ������ � ����� ������ ������ � �������;\n2. ������ �� ������ ��������������;\n3. ������� �������������� �������;\n4. ������� ����� ��������;\n5. ������ �� ������ ����;\n6. ������ �� ������ ������.�", mtInformation, TMsgDlgButtons() << mbOK, 0);
}
//---------------------------------------------------------------------------

