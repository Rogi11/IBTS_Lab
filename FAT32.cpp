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
}
