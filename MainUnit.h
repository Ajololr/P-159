//---------------------------------------------------------------------------

#ifndef MainUnitH
#define MainUnitH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.ExtCtrls.hpp>
#include <Vcl.CheckLst.hpp>
#include "include/SDL.h"
#include <Vcl.MPlayer.hpp>
//---------------------------------------------------------------------------
class TMainForm : public TForm
{
__published:	// IDE-managed Components
	TGroupBox *GroupBox1;
	TRadioGroup *RadioGroup1;
	TButton *Button;
	TGroupBox *GroupBox2;
	TGroupBox *GroupBox3;
	TComboBox *ComboBox2;
	TComboBox *ComboBox3;
	TComboBox *ComboBox4;
	TComboBox *ComboBox5;
	TComboBox *ComboBox6;
	TButton *Button1;
	TButton *Button2;
	TMediaPlayer *MediaPlayer1;
	void __fastcall FormCreate(TObject *Sender);
	void __fastcall ButtonClick(TObject *Sender);
	void __fastcall Button1Click(TObject *Sender);

private:	// User declarations
public:		// User declarations
	__fastcall TMainForm(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TMainForm *MainForm;
//---------------------------------------------------------------------------
#endif
