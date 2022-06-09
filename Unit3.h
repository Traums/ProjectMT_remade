//---------------------------------------------------------------------------

#ifndef Unit3H
#define Unit3H
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
//---------------------------------------------------------------------------
class ProcessThread : public TThread
{
private:
	TEvent* myEvent;
	int i;
protected:
	void __fastcall Execute();
	void __fastcall UpdateLabel();
public:
	__fastcall ProcessThread(TEvent* myEvent, bool CreateSuspended);
	char *dataBuffer;
};
//---------------------------------------------------------------------------
#endif
