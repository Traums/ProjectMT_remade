//---------------------------------------------------------------------------

#include <System.hpp>
#pragma hdrstop

#include "Unit1.h"
#include "Unit2.h"
#include "Unit3.h"
#include "NTFSDriverPCH1.h"

#include<iostream>
#include<Windows.h>

#pragma package(smart_init)
//---------------------------------------------------------------------------
__fastcall ReadThread::ReadThread(bool CreateSuspended)
	: TThread(CreateSuspended)
{
	FreeOnTerminate = true;
	myEvent = new TEvent(NULL, true, false, "", false);
	ProcessThreadPtr = new ProcessThread(myEvent, true);
	ProcessThreadPtr->dataBuffer = dataBuffer;
}
//---------------------------------------------------------------------------
void __fastcall ReadThread::Execute()
{

	using namespace std;

	HANDLE partition = INVALID_HANDLE_VALUE;
	PARTITION_INFORMATION partitionInfo = {0};
	DISK_GEOMETRY diskGeometry = {0};
	HANDLE file = INVALID_HANDLE_VALUE;
	BYTE* buffer = NULL;
	DWORD bufferSize = 0;
	DWORD bytesReturned = 0;
	DWORD bytesWritten = 0;
	BOOL result = FALSE;


	wchar_t Path[64];
	UnicodeString deviceName = Form2->Edit1->Text;
	wchar_t EditText = deviceName.w_str()[0];
	swprintf(Path,L"\\\\.\\PhysicalDrive%c",EditText);

	if ((partition = CreateFileW(Path,
		GENERIC_READ,
		FILE_SHARE_READ | FILE_SHARE_WRITE,
		NULL,
		OPEN_EXISTING,
        0,
		NULL)) == INVALID_HANDLE_VALUE)
	{
		Form2->Label5->Visible = True;
		Form2->Label5->Caption = "Ошибка чтения раздела";
	}

	if (!DeviceIoControl(partition,
		IOCTL_DISK_GET_DRIVE_GEOMETRY,
		NULL,
		0,
        &diskGeometry,
		sizeof (DISK_GEOMETRY),
		&bytesReturned,
        (LPOVERLAPPED)NULL))
	{
		Form2->Label5->Visible = True;
		Form2->Label5->Caption = "Ошибка определения геометрии диска";

		char errMsg[65];
		DWORD dw = GetLastError();

		FormatMessageA(
			FORMAT_MESSAGE_FROM_SYSTEM |
			FORMAT_MESSAGE_IGNORE_INSERTS,
			NULL,
			dw,
			MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
			(LPSTR)&errMsg,
			sizeof(errMsg), NULL );

		Form2->Label7->Caption = errMsg;
		CloseHandle(partition);

		ProcessThreadPtr->Terminate();
		delete ProcessThreadPtr;
	}

	if (!DeviceIoControl(partition,
		IOCTL_DISK_GET_PARTITION_INFO,
		NULL,
		0,
		&partitionInfo,
		sizeof (PARTITION_INFORMATION),
		&bytesReturned,
		(LPOVERLAPPED)NULL))
	{
		Form2->Label5->Visible = True;
		Form2->Label5->Caption = "Ошибка определения сведений о разделе";
		char errMsg[65];
		DWORD dw = GetLastError();

		FormatMessageA(
			FORMAT_MESSAGE_FROM_SYSTEM |
			FORMAT_MESSAGE_IGNORE_INSERTS,
			NULL,
			dw,
			MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
			(LPSTR)&errMsg,
			sizeof(errMsg), NULL );

		Form2->Label7->Caption = errMsg;
		CloseHandle(partition);
	}


	deviceName = Form2->Edit2->Text;
	EditText = deviceName.w_str()[0];
	swprintf(Path,L"%c:\\partition.img",EditText);

	if ((file = CreateFileW(Path,
		GENERIC_READ | GENERIC_WRITE,
		0,
		NULL,
		CREATE_ALWAYS,
		0,
		NULL)) == INVALID_HANDLE_VALUE)
	{
		Form2->Label5->Visible = True;
		Form2->Label5->Caption = GetLastError();
		CloseHandle(partition);

    }

	if (!SetFilePointerEx(file, partitionInfo.PartitionLength, NULL, FILE_BEGIN))
	{
		Form2->Label5->Visible = True;
		Form2->Label5->Caption = "Ошибка установки указателя";
		CloseHandle(file);
		CloseHandle(partition);
    }

	if (!SetEndOfFile(file))
	{
		Form2->Label5->Visible = True;
		Form2->Label5->Caption = "Ошибка определения размера файла";

		char errMsg[65];
		DWORD dw = GetLastError();

		FormatMessageA(
			FORMAT_MESSAGE_FROM_SYSTEM |
			FORMAT_MESSAGE_IGNORE_INSERTS,
			NULL,
			dw,
			MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
			(LPSTR)&errMsg,
			sizeof(errMsg), NULL );
		//Form2->Label7->Caption = errMsg;

		CloseHandle(file);
		CloseHandle(partition);
    }


	bufferSize = diskGeometry.BytesPerSector * diskGeometry.SectorsPerTrack;
	buffer = new BYTE[bufferSize];

	//Индикатор % чтения
	int p = 0;
	__int64 s = 0;
	__int64 t = (partitionInfo.PartitionLength.QuadPart / bufferSize) / 100;


	SetFilePointer(file, 0, NULL, FILE_BEGIN);

	do
	{
		result = ReadFile(partition, buffer, bufferSize, &bytesReturned, NULL);
		if (!result)
		{
			delete[] buffer;
			CloseHandle(file);
			CloseHandle(partition);
			Form2->Label5->Visible = True;
			Form2->Label5->Caption = "Ошибка чтения";
		}

		result = WriteFile(file, buffer, bytesReturned, &bytesWritten, NULL);
		if (!result)
		{
			delete[] buffer;
			CloseHandle(file);
			CloseHandle(partition);
			Form2->Label5->Visible = True;
			Form2->Label5->Caption = "Ошибка записи, недостаточно места на диске";

			char errMsg[65];
			DWORD dw = GetLastError();

			FormatMessageA(
				FORMAT_MESSAGE_FROM_SYSTEM |
				FORMAT_MESSAGE_IGNORE_INSERTS,
				NULL,
				dw,
				MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
				(LPSTR)&errMsg,
				sizeof(errMsg), NULL );
			//Form2->Label7->Caption = errMsg;
		}

		if (!(s++ % (t)))
		{
			Form2->Label1->Caption = p++;
		}
	}
	while (result && bytesReturned);

	delete[] buffer;
	CloseHandle(file);
	CloseHandle(partition);

	ProcessThreadPtr->Terminate();
	delete ProcessThreadPtr;
}
//---------------------------------------------------------------------------
