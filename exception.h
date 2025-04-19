#pragma once

#include "error-code.h"

namespace ffilesys
{
    class Exception : public std::exception 
    {
    public:
        Exception(errcode_t errcode)
        {
            code = errcode;
        }
        
        const char* what() const noexcept override
        {
            return ERRCODE_DESCR_TABLE[code];
        }

        errcode_t code;
    private:
    };   
}