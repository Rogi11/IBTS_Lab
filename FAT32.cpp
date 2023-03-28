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
#pragma pack (push,1)
	typedef struct
	{
		BYTE jmp[3];
		BYTE Identifier[8]; // Идентификатор фирмы-изготовителя
		UINT16 SectorFactor; // Число байт в секторе
		UINT8 SectorPerCluster; // Число секторов в кластере
		UINT16 RsvdSecCount; // Число резервных секторов
		UINT8 CountFAT; // Число таблиц(копий) FAT
		UINT16 NumberFileDescr; // Количество 32-байтных дескрипторов
		UINT16 TotalSec; // Общее число секторов в разделе
		UINT8 MediaType; // Тип носителя
		UINT16 CountSectorFAT; // Поле содержит количество секторов, занимаемых одной копией FAT
		UINT16 NumberOfSectorsPerTrack; // Число секторов на дорожке
		UINT16 NumberOfWorkingSurfaces; // Число рабочих поверхностей
		UINT32 HiddenSectors; // Число скрытых секторов перед разделом
		UINT32 TotalSec32; //Общее число секторов в разделе.
		UINT32 FATSz32; // Размер одной таблицы FAT в секторах
		UINT32 ActiveFATNumber; // Номер активной таблицы факт
		UINT32 RootCluster; // Кластер корневого каталога
		UINT16 FSINFO; // Номер сектора структуры FSINFO
		UINT16 BkBootSec; // Сектор LBA резевной копии данной записи
		BYTE Reserved[30];
		BYTE FilSysType[8]; // Тип файловой системы
	} FAT32_BootRecord;
#pragma pack(pop)
	FAT32_BootRecord* pBootRecord;
	pBootRecord = (FAT32_BootRecord*)dataBuffer;
	
	CloseHandle(fileHandle);
	return 0;
}
