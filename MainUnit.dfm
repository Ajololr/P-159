object MainForm: TMainForm
  Left = 0
  Top = 0
  Caption = #1056'-159'
  ClientHeight = 184
  ClientWidth = 723
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  OnCreate = FormCreate
  PixelsPerInch = 96
  TextHeight = 13
  object GroupBox1: TGroupBox
    Left = 8
    Top = 8
    Width = 377
    Height = 161
    Caption = #1042#1099#1073#1086#1088' '#1088#1072#1073#1086#1095#1077#1081' '#1095#1072#1089#1090#1086#1090#1099
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -19
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentFont = False
    TabOrder = 0
    object GroupBox2: TGroupBox
      Left = 25
      Top = 40
      Width = 136
      Height = 105
      Caption = #1052#1043#1094
      TabOrder = 0
      object ComboBox2: TComboBox
        Left = 16
        Top = 51
        Width = 40
        Height = 26
        HelpType = htKeyword
        HelpContext = 3
        ParentCustomHint = False
        AutoDropDown = True
        Style = csOwnerDrawFixed
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -16
        Font.Name = 'Tahoma'
        Font.Style = []
        ItemHeight = 20
        ParentFont = False
        ParentShowHint = False
        ShowHint = False
        TabOrder = 0
        Items.Strings = (
          '3'
          '4'
          '5'
          '6'
          '7')
      end
      object ComboBox3: TComboBox
        Left = 73
        Top = 51
        Width = 40
        Height = 26
        AutoDropDown = True
        Style = csOwnerDrawFixed
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -16
        Font.Name = 'Tahoma'
        Font.Style = []
        ItemHeight = 20
        ParentFont = False
        ParentShowHint = False
        ShowHint = False
        TabOrder = 1
        Items.Strings = (
          '0'
          '1'
          '2'
          '3'
          '4'
          '5'
          '6'
          '7'
          '8'
          '9')
      end
    end
    object GroupBox3: TGroupBox
      Left = 175
      Top = 40
      Width = 185
      Height = 105
      Caption = #1050#1043#1094
      TabOrder = 1
      object ComboBox4: TComboBox
        Left = 17
        Top = 51
        Width = 40
        Height = 26
        AutoDropDown = True
        Style = csOwnerDrawFixed
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -16
        Font.Name = 'Tahoma'
        Font.Style = []
        ItemHeight = 20
        ParentFont = False
        ParentShowHint = False
        ShowHint = False
        TabOrder = 0
        Items.Strings = (
          '0'
          '1'
          '2'
          '3'
          '4'
          '5'
          '6'
          '7'
          '8'
          '9')
      end
      object ComboBox5: TComboBox
        Left = 75
        Top = 51
        Width = 40
        Height = 26
        AutoDropDown = True
        Style = csOwnerDrawFixed
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -16
        Font.Name = 'Tahoma'
        Font.Style = []
        ItemHeight = 20
        ParentFont = False
        ParentShowHint = False
        ShowHint = False
        TabOrder = 1
        Items.Strings = (
          '0'
          '1'
          '2'
          '3'
          '4'
          '5'
          '6'
          '7'
          '8'
          '9')
      end
      object ComboBox6: TComboBox
        Left = 129
        Top = 51
        Width = 40
        Height = 26
        AutoDropDown = True
        Style = csOwnerDrawFixed
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -16
        Font.Name = 'Tahoma'
        Font.Style = []
        ItemHeight = 20
        ParentFont = False
        ParentShowHint = False
        ShowHint = False
        TabOrder = 2
        Items.Strings = (
          '0'
          '1'
          '2'
          '3'
          '4'
          '5'
          '6'
          '7'
          '8'
          '9')
      end
    end
  end
  object RadioGroup1: TRadioGroup
    Left = 391
    Top = 8
    Width = 209
    Height = 161
    Caption = #1056#1077#1078#1080#1084' '#1088#1072#1073#1086#1090#1099
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -19
    Font.Name = 'Tahoma'
    Font.Style = []
    Items.Strings = (
      #1058#1051#1043
      #1058#1051#1060)
    ParentFont = False
    TabOrder = 1
  end
  object Button: TButton
    Left = 606
    Top = 20
    Width = 109
    Height = 41
    Caption = 'T'#1051#1043' '#1082#1083#1102#1095
    TabOrder = 2
    OnClick = ButtonClick
  end
  object Button1: TButton
    Left = 606
    Top = 80
    Width = 109
    Height = 33
    Caption = #1053#1072#1095#1072#1090#1100
    TabOrder = 3
    OnClick = Button1Click
  end
  object Button2: TButton
    Left = 606
    Top = 136
    Width = 109
    Height = 33
    Caption = #1054#1089#1090#1072#1085#1086#1074#1080#1090#1100
    TabOrder = 4
  end
  object MediaPlayer1: TMediaPlayer
    Left = 656
    Top = 184
    Width = 253
    Height = 30
    TabOrder = 5
  end
end
