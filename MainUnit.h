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
#include "SDL/include/SDL.h"
#include <Vcl.MPlayer.hpp>
//---------------------------------------------------------------------------
class TMainForm : public TForm
{
__published:	// IDE-managed Components
	TGroupBox *GroupBox1;
	TRadioGroup *rgMod;
	TButton *Button;
	TGroupBox *gbMH;
	TGroupBox *gbKH;
	TComboBox *cbMTenth;
	TComboBox *cbMOnes;
	TComboBox *cbKHundred;
	TComboBox *cbKTenth;
	TComboBox *cbKOnes;
	TButton *btnVoltage;
	TButton *btnSetFrequency;
	TCheckBox *cbEnable;
	TLabel *Label1;
	TLabel *Label2;
	TLabel *Label3;
	TCheckBox *CheckBox2;
	void __fastcall FormCreate(TObject *Sender);
	void __fastcall ButtonClick(TObject *Sender);
	void __fastcall rgModClick(TObject *Sender);
	void __fastcall cbEnableClick(TObject *Sender);
	void __fastcall CheckBox2Click(TObject *Sender);
	void __fastcall btnSetFrequencyClick(TObject *Sender);

private:	// User declarations
public:		// User declarations
	__fastcall TMainForm(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TMainForm *MainForm;
//---------------------------------------------------------------------------
#endif
