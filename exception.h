#pragma once

#include <cstring>

#include "error-code.h"

namespace ffilesys
{
    class Exception : public std::exception 
    {
    public:
        explicit Exception(const errcode_t errcode)
        {
            info.assign(reinterpret_cast<const char *>(&errcode), sizeof(errcode_t));
            info += ERRCODE_DESCR_TABLE[errcode];
        }
        Exception(const errcode_t errcode, const std::string& details) {
            info.assign(reinterpret_cast<const char *>(&errcode), sizeof(errcode_t));
            info += std::string(ERRCODE_DESCR_TABLE[errcode]) + ": " + details;
        }
        Exception(const errcode_t errcode, std::string&& details) {
            info.assign(reinterpret_cast<const char *>(&errcode), sizeof(errcode_t));
            info += std::string(ERRCODE_DESCR_TABLE[errcode]) + ": " + std::move(details);
        }
        
        [[nodiscard]] const char* what() const noexcept override
        {
            return info.c_str() + sizeof(errcode_t);
        }
        [[nodiscard]] errcode_t code() const noexcept {
            return *(reinterpret_cast<const errcode_t*>(info.data()));
        }

        // First bytes of info is raw errcode_t, after them goes description of error.
        std::string info;
    private:
    };   
}