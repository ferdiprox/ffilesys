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
        "Invalid file name"
    };

    ErrorCodes errnoToErrorCode()
    {
        switch(errno)
        {
            default:
#ifdef FFS_DEBUG
                printf("ffilesys: An unknown error handled: %i\n", errno);
#endif
                return EC_UnknownError;
            case EINVAL:
                // need to check if this code can be raised by another ffilesys errors
                return EC_InvalidFileName;
            case ENOENT:
                return EC_NoSuchFileOrDir;
            case EBADF:
                return EC_WrongFileFlags;
        }
    }
}