#ifdef _WIN32
#include <tchar.h>
#endif
#include <windows.h>
#pragma once
#pragma pack(push,1)

 typedef struct
{
	BYTE    jump[3];  //    jmp
	UINT name_lo;   //      oem_lo
	UINT name_hi;   // oem_hi
	USHORT  sec_size;  //         байт на сектор
	BYTE    secs_cluster;   //    секторов на кластер
	BYTE    reserved_0[7];
	BYTE    media_desc;          //
	UINT16  reserved_2;
	UINT16  secs_track;            // кол-во секторов на дорожке
	UINT16  num_heads;               //  кол-во магн головок
	BYTE    reserved_3[8];
	UINT16  reserved_4;
	UINT16  reserved_5;
	ULONGLONG  num_secs;               //  кол-во секторов в разделе
	ULONGLONG  mft_clus;                 //   номер кластера MFT
	ULONGLONG  mft2_clus;                 //   номер кластера копии MFT
	UINT  mft_rec_size;                 //    размер MFT в кластерах
	UINT  buf_size;                    //      размер буфера индекса в кластерах
	UINT  volume_id_lo;                 //  номер серийника диска
	UINT volume_id_hi;
	BYTE Padding[0x1AE];
	USHORT sign;                         //сигнатура
} NTFS_BootRecord;
# pragma pack(pop)