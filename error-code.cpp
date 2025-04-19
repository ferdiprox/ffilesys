#include "error-code.h"

namespace ffilesys
{
    const char* ERRCODE_DESCR_TABLE[EC__MAX]
    {
        "Unknown/Unhandled error",
        "Null FILE pointer",
        "Reached end of file",
        "Bad file opening flags",
        "No such file or directory",
    };

    ErrorCodes errnoToErrorCode()
    {
        switch(errno)
        {
        default:
#ifdef FFS_DEBUG
            printf("An unknown error handled: %i\n", errno);
#endif
            return EC_UnknownError;
        case ENOENT:
            return EC_NoSuchFileOrDir;
        case EBADF:
            return EC_WrongFileFlags;
        }
    }
}