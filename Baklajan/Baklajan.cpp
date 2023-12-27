#include <iostream>
#include <windows.h>

#pragma pack(push,1) // выравниваем

typedef struct // заголовок
{
    WORD header_crc;
    BYTE header_type;
    WORD header_flags;
    WORD header_size;
} VOLUME_HEADER;

typedef struct //заголок 0x74
{
    DWORD PackSize;
    DWORD UnpSize;
    BYTE HostOS;
    DWORD FileCRC;
    DWORD FileTime;
    BYTE UnpVer;
    BYTE Method;
    WORD NameSize;
    DWORD FileAttr;
} FILE_HEAD;

#pragma pack(pop)

void PrintHeader(DWORD file_size, const char* file_in) {
    int i = 7;
    while (i < file_size) {
        VOLUME_HEADER* header = (VOLUME_HEADER*)&file_in[i];

        if (header->header_type == 0x74) {
            int save_point = i;
            i += sizeof(VOLUME_HEADER);
            FILE_HEAD* file = (FILE_HEAD*)&file_in[i];
            i += sizeof(FILE_HEAD);

            char* content = new char[file->NameSize];
            for (int j = 0; j < file->NameSize; j++) {
                content[j] = file_in[i];
                i++;
            }
            for (int j = 0; j < file->NameSize; j++) {
                std::cout << content[j];
            }
            std::cout << std::endl;
            delete[] content;
            i = save_point;
            i += header->header_size + file->PackSize;
        }
        else {
            i += header->header_size;
        }
    }
}


int main() {
    wchar_t* file_name(L"F:\\NSUlabs\\CPP\\dasha\\Example.rar");
    HANDLE file_handle = CreateFileW(file_name, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

    if (file_handle == INVALID_HANDLE_VALUE) {
        std::cout << "Error INVALID_HANDLE_VALUE" << std::endl;
        return 1;
    }
    DWORD file_size = GetFileSize(file_handle, NULL);
    char* file_in = new char[file_size];
    DWORD byte_read;

    if (ReadFile(file_handle, file_in, file_size, &byte_read, NULL)) {
        PrintHeader(file_size, file_in);
    }
    else {
        std::cout << "FileRead Error" << std::endl;
        return 1;
    }

    delete[] file_in;
    return 0;
}