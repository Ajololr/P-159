object MainForm: TMainForm
  Left = 0
  Top = 0
  Caption = #1056'-159'
  ClientHeight = 272
  ClientWidth = 617
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
  object Label1: TLabel
    Left = 396
    Top = 167
    Width = 60
    Height = 24
    Caption = #1053#1072#1089#1090#1088'.'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -20
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentFont = False
  end
  object Label2: TLabel
    Left = 486
    Top = 19
    Width = 93
    Height = 13
    Caption = #1042#1074#1077#1076#1080#1090#1077' IP '#1072#1076#1088#1077#1089':'
  end
  object Label3: TLabel
    Left = 486
    Top = 74
    Width = 74
    Height = 13
    Caption = #1042#1074#1077#1076#1080#1090#1077' '#1087#1086#1088#1090':'
  end
  object GroupBox1: TGroupBox
    Left = 8
    Top = 2
    Width = 464
    Height = 161
    Caption = #1042#1099#1073#1086#1088' '#1088#1072#1073#1086#1095#1077#1081' '#1095#1072#1089#1090#1086#1090#1099
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -19
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentFont = False
    TabOrder = 0
    object gbMH: TGroupBox
      Left = 41
      Top = 40
      Width = 136
      Height = 105
      Caption = #1052#1043#1094
      TabOrder = 0
      object cbMTenth: TComboBox
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
        ItemIndex = 0
        ParentFont = False
        ParentShowHint = False
        ShowHint = False
        TabOrder = 0
        Text = '3'
        Items.Strings = (
          '3'
          '4'
          '5'
          '6'
          '7')
      end
      object cbMOnes: TComboBox
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
        ItemIndex = 0
        ParentFont = False
        ParentShowHint = False
        ShowHint = False
        TabOrder = 1
        Text = '0'
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
    object gbKH: TGroupBox
      Left = 223
      Top = 40
      Width = 185
      Height = 105
      Caption = #1050#1043#1094
      TabOrder = 1
      object cbKHundred: TComboBox
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
        ItemIndex = 0
        ParentFont = False
        ParentShowHint = False
        ShowHint = False
        TabOrder = 0
        Text = '0'
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
      object cbKTenth: TComboBox
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
        ItemIndex = 0
        ParentFont = False
        ParentShowHint = False
        ShowHint = False
        TabOrder = 1
        Text = '0'
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
      object cbKOnes: TComboBox
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
        ItemIndex = 0
        ParentFont = False
        ParentShowHint = False
        ShowHint = False
        TabOrder = 2
        Text = '0'
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
  object rgMod: TRadioGroup
    Left = 126
    Top = 167
    Width = 162
    Height = 97
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
    OnClick = rgModClick
  end
  object Button: TButton
    Left = 24
    Top = 169
    Width = 73
    Height = 48
    Caption = 'T'#1051#1043' '#1082#1083#1102#1095
    TabOrder = 2
    OnClick = ButtonClick
  end
  object btnSetFrequency: TButton
    Left = 404
    Top = 197
    Width = 43
    Height = 39
    TabOrder = 3
    OnClick = btnSetFrequencyClick
  end
  object cbEnable: TCheckBox
    Left = 315
    Top = 195
    Width = 65
    Height = 37
    Caption = #1042#1082#1083
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -20
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentFont = False
    TabOrder = 4
    OnClick = cbEnableClick
  end
  object CheckBox2: TCheckBox
    Left = 24
    Top = 231
    Width = 73
    Height = 17
    Caption = #1042#1099#1079#1086#1074
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -20
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentFont = False
    TabOrder = 5
    OnClick = CheckBox2Click
  end
  object IPEdt: TEdit
    Left = 488
    Top = 38
    Width = 121
    Height = 21
    TabOrder = 6
    Text = '192.168.100.3'
  end
  object PortEdt: TEdit
    Left = 486
    Top = 93
    Width = 121
    Height = 21
    TabOrder = 7
    Text = '5693'
  end
  object ConnectBtn: TButton
    Left = 504
    Top = 138
    Width = 89
    Height = 25
    Caption = #1055#1086#1076#1082#1083#1102#1095#1080#1090#1100#1089#1103
    TabOrder = 8
    OnClick = ConnectBtnClick
  end
end
