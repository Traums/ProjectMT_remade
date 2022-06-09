//---------------------------------------------------------------------------

#include <System.hpp>
#pragma hdrstop

#include "Unit1.h"
#include "Unit2.h"
#include "Unit3.h"
#pragma package(smart_init)
//---------------------------------------------------------------------------
__fastcall ProcessThread::ProcessThread(TEvent* myEvent, bool CreateSuspended)
	: TThread(CreateSuspended), myEvent(myEvent)
{
	FreeOnTerminate = true;
}
//---------------------------------------------------------------------------
void __fastcall ProcessThread::Execute()
{
	i = 0;
	while(true)
	{
		if(myEvent->WaitFor(0) == wrSignaled)
		{
			Sleep(50);

			//ׁבנמס פכאדא
			myEvent->ResetEvent();
		}
		Sleep(200);
		if(Terminated) break;
		Synchronize(&UpdateLabel);
		i++;
	}
}
//---------------------------------------------------------------------------
void __fastcall ProcessThread::UpdateLabel()
{
	Form2->Label2->Caption = UnicodeString(i);
}
