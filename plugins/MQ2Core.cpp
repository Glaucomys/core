//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=//
// Projet: MQ2Core.cpp
// Author: exspes007
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=//
//
#include "../MQ2Plugin.h"


PreSetup("MQ2Core");
PLUGIN_VERSION(1.0000);


HMODULE EQWhMod=0; 
typedef HWND   (__stdcall *fEQW_GetDisplayWindow)(VOID); 
fEQW_GetDisplayWindow EQW_GetDisplayWindow=0; 





///////////////////////////////////////////////////////////////////////////////

void DestroyMyWindow(void);
void CreateMyWindow(void);
void DoPlayWindow(void);

///////////////////////////////////////////////////////////////////////////////
////
//
//      Window Class - Uses custom XLM Window - MQUI_CoreWnd.xml 
//
////
///////////////////////////////////////////////////////////////////////////////


// Window Declarations
class CMyWnd : public CCustomWnd
{
public:
	CCheckBoxWnd *APW_EvalChecked;
	CCheckBoxWnd *APW_LoopChecked;
	CCheckBoxWnd *APW_PauseChecked;
	CCheckBoxWnd *APW_ReverseChecked;
	CCheckBoxWnd *APW_SlowChecked;

	CCheckBoxWnd *APW_DoorChecked;
	CCheckBoxWnd *APW_SmartChecked;
	CCheckBoxWnd *APW_ZoneChecked;

	CCheckBoxWnd *APW_StuckChecked;
	CCheckBoxWnd *APW_BreakPathChecked;
	CCheckBoxWnd *APW_BreakFollowChecked;


	CButtonWnd   *APW_PlayButton;
	CButtonWnd   *APW_CancelButton;
	CButtonWnd   *APW_RecordButton;

	CTextEntryWnd*APW_EditBox;

	CListWnd	 *APW_PathList;
	CListWnd	 *APW_INIList;
	char		  szList[MAX_STRING];

	CMyWnd():CCustomWnd("CoreWnd")
	{
		APW_PlayButton	        =(CButtonWnd*)   GetChildItem("APW_PlayButton");
		APW_CancelButton        =(CButtonWnd*)   GetChildItem("APW_CancelButton");
		APW_RecordButton        =(CButtonWnd*)   GetChildItem("APW_RecordButton");
		APW_EditBox				=(CTextEntryWnd*)GetChildItem("APW_EditBox");

		APW_PathList			=(CListWnd*)     GetChildItem("APW_PathList");
		APW_INIList				=(CListWnd*)     GetChildItem("APW_INIList");

		APW_EvalChecked         =(CCheckBoxWnd*) GetChildItem("APW_EvalButton");
		APW_LoopChecked         =(CCheckBoxWnd*) GetChildItem("APW_LoopButton");
		APW_PauseChecked        =(CCheckBoxWnd*) GetChildItem("APW_PauseButton");
		APW_ReverseChecked      =(CCheckBoxWnd*) GetChildItem("APW_ReverseButton");
		APW_SlowChecked         =(CCheckBoxWnd*) GetChildItem("APW_SlowButton");

		APW_DoorChecked         =(CCheckBoxWnd*) GetChildItem("APW_DoorButton");
		APW_SmartChecked        =(CCheckBoxWnd*) GetChildItem("APW_SmartButton");
		APW_ZoneChecked         =(CCheckBoxWnd*) GetChildItem("APW_ZoneButton");
		APW_StuckChecked        =(CCheckBoxWnd*) GetChildItem("APW_StuckButton");		
		APW_BreakPathChecked    =(CCheckBoxWnd*) GetChildItem("APW_BreakPathButton");		
		APW_BreakFollowChecked  =(CCheckBoxWnd*) GetChildItem("APW_BreakFollowButton");		

		SetWndNotification(CMyWnd);
	}

	void SetCheckMarks(void)
	{
		APW_EvalChecked->Checked		= iShowEval;
		APW_PauseChecked->Checked		= iShowPause;
		APW_LoopChecked->Checked		= iShowLoop;
		APW_ReverseChecked->Checked		= iShowReverse;
		APW_SlowChecked->Checked		= iShowSlow;
		APW_DoorChecked->Checked		= iShowDoor;
		APW_SmartChecked->Checked		= iShowSmart;
		APW_ZoneChecked->Checked		= iShowZone;
		APW_StuckChecked->Checked		= iShowStuck;
		APW_BreakPathChecked->Checked	= iShowBreakPath;
		APW_BreakFollowChecked->Checked	= iShowBreakFollow;
	}

	void GetCheckMarks(void)
	{
		iShowEval		= APW_EvalChecked->Checked;
		iShowPause  	= APW_PauseChecked->Checked;
		iShowLoop		= APW_LoopChecked->Checked;
		iShowReverse	= APW_ReverseChecked->Checked;
		iShowSlow		= APW_SlowChecked->Checked;
		iShowDoor		= APW_DoorChecked->Checked;
		iShowSmart		= APW_SmartChecked->Checked;
		iShowStuck		= APW_StuckChecked->Checked;
		iShowZone		= APW_ZoneChecked->Checked;
		iShowBreakPath  = APW_BreakPathChecked->Checked;
		iShowBreakFollow= APW_BreakFollowChecked->Checked;
	}


	void ShowWin(void)
	{
		((CXWnd*)this)->Show(1,1);
		Show = 1;
	}

	void HideWin(void)
	{
		((CXWnd*)this)->Show(0,0);
		Show = 0;
	}


	void GetPathListItem(char *s,int m)
	{
        CXStr Str;
        ((CListWnd*)APW_PathList)->GetItemText(&Str,APW_PathList->GetCurSel(),0);
        GetCXStr(Str.Ptr,s,m);
	}

	void AddINIListItem(char *s)
	{
		APW_INIList->AddString( s , 0xFFFFFFFF, 0, 0);
	}

	void GetINIListItem(char *s,int m)
	{
        CXStr Str;
        ((CListWnd*)APW_INIList)->GetItemText(&Str,APW_INIList->GetCurSel(),0);
        GetCXStr(Str.Ptr,s,m);
	}

	void SetINIListItem(char *s,int m)
	{
        //CXStr Str;
		//		SetCXStr(&Str, s);
        //((CListWnd*)APW_INIList)->SetItemText(APW_INIList->GetCurSel(),0,&Str);
	}

	void GetEditBoxItem(char *s,int m)
	{
		GetCXStr((PCXSTR)APW_EditBox->InputText, s, m);
	}


	void SetEditBoxItem(char *s)
	{
		SetCXStr(&APW_EditBox->InputText, s);
	}


	void UpdateUI(void)	{ UpdateUI(0,0,0,0,0); }

	void UpdateUI(int waypoint,float x,float y,float z,char *checkpoint)
	{
		if (StatusState == STATUS_ON && FollowState == FOLLOW_RECORDING) // Recording a path
		{
			SetCXStr( &APW_PlayButton->WindowText		 , "Save");
			SetCXStr( &APW_RecordButton->WindowText		 , "CheckPoint");
		}
		else															
		{
			SetCXStr( &APW_PlayButton->WindowText		 , "Play");
			SetCXStr( &APW_RecordButton->WindowText		 , "Record");
		}

		if (StatusState == STATUS_ON && FollowState == FOLLOW_PLAYING && checkpoint)  // Playing a path
		{
		    char szTemp[MAX_STRING];
			sprintf(szTemp,"%d=%5.2f %5.2f %5.2f %s ",waypoint,y,x,z,checkpoint);
			SetCXStr(&APW_EditBox->InputText, szTemp);
			APW_INIList->SetCurSel(waypoint-1);
			APW_INIList->EnsureVisible(waypoint-1);
		}
		if (StatusState == STATUS_OFF) 
			SetCXStr(&APW_EditBox->InputText, "");
	}


	///////////////////////////////////////////////////////////////////////////////
	////
	//
	//      WndNotification - Major place for interactions
	//
	////
	///////////////////////////////////////////////////////////////////////////////

	int WndNotification(CXWnd *pWnd, unsigned int Message, void *unknown)
	{
		if (pWnd==0) {
			if (Message==XWM_CLOSE) {
				CreateMyWindow();
				ShowWin();
				return 0;
			}
		}

		if ((pWnd == (CXWnd*)APW_StuckChecked)   || (pWnd == (CXWnd*)APW_BreakPathChecked) || (pWnd == (CXWnd*)APW_BreakFollowChecked) ) {
			return 0;
		}

		if (pWnd->GetType()==UI_Button) GetCheckMarks();

		if (pWnd == (CXWnd*)APW_EditBox  && Message == 6) // Pressed ENTER 
		{
			char szTemp[MAX_STRING];
			GetEditBoxItem(szTemp,MAX_STRING);
			ReplaceWaypointWith(szTemp);
			GetPathListItem(szTemp,MAX_STRING);
			return 0; 
		}

		if (pWnd == (CXWnd*)APW_PathList && Message ==1) 
		{
			char szTemp[MAX_STRING];
			GetPathListItem(szTemp,MAX_STRING);
			ClearAll();
			LoadPath(szTemp);
			return 0;
		}
		if (pWnd == (CXWnd*)APW_INIList && Message ==1) 
		{
			char szTemp[MAX_STRING];
			if (StatusState == STATUS_OFF) {
				GetINIListItem(szTemp,MAX_STRING);
				SetCXStr(&APW_EditBox->InputText,szTemp);
				PlayWaypoint = APW_INIList->GetCurSel()+1;
				PlayOne = true;
				StatusState = STATUS_ON;
				FollowState = FOLLOW_PLAYING;	
			}

			return 0;
		}

		
		if (pWnd == (CXWnd*)APW_PlayButton && Message == 1) {

			if (StatusState == STATUS_ON && FollowState == FOLLOW_RECORDING) // Recording a path
			{
				MQRecordCommand(NULL,"save");
				MQPlayCommand(NULL,"show");
				UpdateUI();
				return 0;
			}


			char Buffer[MAX_STRING];
			GetPathListItem(Buffer,MAX_STRING);
			if (Buffer[0]!=0)
			{
				char cmd[1000];
				sprintf(cmd,"%s%s%s%s%s%s%s", Buffer,
					(APW_DoorChecked->Checked    ?" Door":" NoDoor"),
					(APW_LoopChecked->Checked    ?" Loop":""),
					(APW_ReverseChecked->Checked ?" Reverse":" Normal"),
					(APW_SmartChecked->Checked   ?" Smart":""),
					(APW_SlowChecked->Checked    ?" Slow":" Fast"),
					(APW_ZoneChecked->Checked    ?" Zone":""));
				MQPlayCommand(NULL,"off");
				MQPlayCommand(NULL,cmd); 
				WriteChatf("GUI cmd = [%s]",cmd);
			}
			UpdateUI();
			return 0;
		}

		if (pWnd == (CXWnd*)APW_CancelButton) {
			MQPlayCommand(NULL,"off");
			UpdateUI();
			return 0;
		}

		if (pWnd == (CXWnd*)APW_RecordButton) {
		    char szTemp[MAX_STRING];
			char cmd[MAX_STRING];

			GetCXStr((PCXSTR)APW_EditBox->InputText, szTemp, 256);
			if (szTemp[0]!=0)
			{
				SetCXStr(&APW_EditBox->InputText,"");
				if (StatusState == STATUS_OFF) 
					MQRecordCommand(NULL,szTemp);
				else if (FollowState == FOLLOW_RECORDING) // Recording a path
				{
					sprintf(cmd,"checkpoint %s",szTemp);
					MQRecordCommand(NULL,cmd);
				}
			}
			UpdateUI();
			return 0;
		}


		return CSidlScreenWnd::WndNotification(pWnd,Message,unknown);
	};

};


///////////////////////////////////////////////////////////////////////////////
////
//
//      The rest of the non Window class routines.
//
////
///////////////////////////////////////////////////////////////////////////////


CMyWnd *MyWnd=0;

void CreateMyWindow(void)
{
	DebugSpewAlways("MQ2MyWnd::CreateWindow()");
	if (MyWnd) return;
//	if (MyWnd) DestroyMyWindow();
	if (pSidlMgr->FindScreenPieceTemplate("coreWnd")) {
		MyWnd = new CMyWnd;
		if (MyWnd) {
			MyWnd->SetCheckMarks();
			MyWnd->HideWin();
		}
	}
}

void DestroyMyWindow(void)
{
	DebugSpewAlways("MQ2MyWnd::DestroyWindow()");
	if (MyWnd)
	{
		delete MyWnd;
		MyWnd=0;
	}
}

PLUGIN_API VOID OnCleanUI(VOID)		{ DestroyMyWindow(); }
PLUGIN_API VOID OnReloadUI(VOID)	{ if (gGameState == GAMESTATE_INGAME && pCharSpawn) CreateMyWindow(); }



void DoPlayWindow(void)
{
	int i;
	char *p,*q;
	CHAR INIFileNameTemp[400];

	CreateMyWindow();
	if (MyWnd) {
		MyWnd->APW_PathList->DeleteAll();

		sprintf(INIFileNameTemp,"%s\\%s\\%s.ini",gszINIPath,"MQ2AdvPath",GetShortZone(GetCharInfo()->zoneId));

		GetPrivateProfileString(NULL,NULL,NULL,MyWnd->szList,MAX_STRING,INIFileNameTemp);
		i = 0;
		p = q = MyWnd->szList;
		while ( *q!=NULL && i<99) 
		{
			if (*p==0)
			{
				MyWnd->APW_PathList->AddString( q , 0xFFFFFFFF, 0, 0);
				q = p+1;
			}
			p++;
		} 

		MyWnd->ShowWin();
	}
}
