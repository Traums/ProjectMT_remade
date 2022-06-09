//---------------------------------------------------------------------------

#ifndef Unit1H
#define Unit1H
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
//---------------------------------------------------------------------------
class ReadThread : public TThread
{
private:
	class ProcessThread *ProcessThreadPtr;
	TEvent* myEvent;
protected:
	void __fastcall Execute();
public:
	__fastcall ReadThread(bool CreateSuspended);
	char dataBuffer[1024*512];
};
//---------------------------------------------------------------------------
#endif
