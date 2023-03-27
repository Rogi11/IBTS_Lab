#include <iostream>
#include <Windows.h>
using namespace std;
int main()
{
    setlocale(LC_ALL, "Russian");
    const WCHAR *fileName = L"\\\\.\\E:";
	HANDLE fileHandle = CreateFileW(
		fileName, // Имя файла (WCHAR*)
		GENERIC_READ,	  // Режим доступа
		FILE_SHARE_READ | FILE_SHARE_WRITE, // Режим совместной работы
		NULL, // Атрибуты безопасности
		OPEN_EXISTING, // Способ открытия
		FILE_ATTRIBUTE_NORMAL, // Флаги и атрибуты
		NULL // Описатель (идентификатор) файла шаблона с правами доступа GENERIC_READ.
	);
	if (fileHandle == INVALID_HANDLE_VALUE)
	{
		cout << "Ошибка чтения диска" << endl;
		return 1;
	}
	BYTE dataBuffer[512];
	ULONGLONG startOffset = 0x0;
	DWORD bytesToRead = 512;
	DWORD bytesRead;
	LARGE_INTEGER sectorOffset;
	sectorOffset.QuadPart = startOffset;
	unsigned long currentPosition = SetFilePointer(
		fileHandle,
		sectorOffset.LowPart,
		&sectorOffset.HighPart,
		FILE_BEGIN // Точка в файле, относительно которой необходимо позиционироваться (FILE_BEGIN, FILE_CURRENT, FILE_END)
	);

	if (currentPosition != sectorOffset.LowPart)
	{
		cout << "Курсор не установлен в начале" << endl;
		return 2;
	}
	bool readResult = ReadFile(
		fileHandle,
		dataBuffer,
		bytesToRead,
		&bytesRead,
		NULL
	);
	if (!readResult || bytesRead != bytesToRead)
	{
		cout << "Чтение данных выполнено с ошибкой" << endl;
		return 3;
	}
	CloseHandle(fileHandle);
	return 0;
#pragma pack (push,1)
	typedef struct
	{	
		BYTE jmp[3];
		BYTE Identifier[8]; //Идентификатор фирмы-изготовителя
		BYTE BytePerSector[2]; //Число байт в секторе
		BYTE SectorPerCluster[1]; //Число секторов в кластере
		BYTE RsvdSecCount[2]; //Число резервных секторов
		BYTE CountFAT[1];
		BYTE NumberFileDescr[2];
		BYTE TotalSec[2];
		BYTE MediaType[1];
		BYTE CountSectorFAT[2];
		BYTE HiddenSectors[4];
		BYTE NumberOfSectorsPerTrack[2];
		BYTE NumberOfWorkingSurfaces[2];
		BYTE TotalSec32[4];
		BYTE FATSz32[4];
		BYTE ActiveFATNumber[4];
		BYTE RootCluster[4];
		BYTE FSINFO[2];
		BYTE BkBootSec[2];
	} FAT32_BootRecord;
#pragma pack(pop)
	FAT32_BootRecord* pBootRecord;
}
