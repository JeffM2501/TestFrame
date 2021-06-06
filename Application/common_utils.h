#pragma once

namespace Base64
{
    int GetSize(const char* input);
    void* Decode(const char* input, int* size);
    void FreeBuffer(void* buffer);
}