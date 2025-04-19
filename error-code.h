#pragma once

#include <errno.h>
#include <stdio.h>
#include <exception>

namespace ffilesys
{
    enum ErrorCodes
    {
        // Common codes
        EC_UnknownError,

        // File's codes
        EC_NoActiveFile,
        EC_EOF,
        EC_WrongFileFlags,
        EC_NoSuchFileOrDir,

        EC__MAX,
    };

    extern const char* ERRCODE_DESCR_TABLE[EC__MAX];

    typedef unsigned char errcode_t;

    ErrorCodes errnoToErrorCode();

    // Prints description of error code.
    inline void verboseCode(errcode_t code)
    {
        printf("ffilesys error: %s\n", ERRCODE_DESCR_TABLE[code]);
    }
}