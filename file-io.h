#pragma once

#include <cstdio>
#include <string>
#include <filesystem>

#include "exception.h"

#define FFS_FIO_HANDLE_NULLFH

#ifdef FFS_FIO_HANDLE_NULLFH
    #define FFS_FIO_NULLFH_HANDLER() \
        if(!fileHandler) { throw Exception(EC_NoActiveFile); }
#else 
    #define FFS_FIO_NULLFH_HANDLER()
#endif

namespace ffilesys
{
    enum FileOpenFlags
    {
        FOF_Read = 1 << 0,
        FOF_Write = 1 << 1,
        FOF_Append = 1 << 2,
        FOF_Binary = 1 << 3,
    };

    enum FileOrigins {
        FO_Begin = SEEK_SET,
        FO_Current = SEEK_CUR,
        FO_End = SEEK_END,
    };

    static constexpr int NUM_FILE_OPEN_FLAGS = 4;

    // An Input/Output file manager, based on cstdio FILE.
    // Mostly doesn't support multithreading(but need to test).
    class File
    {
    public:
                
        // Opening a new file.
        // You can specify flags in ffilesys::FileOpenFlags, for example
        // .open(ffilesys::FOF_Read | ffilesys::FOF_Binary)
        // will open a binary input stream.
        void open(const std::string& filename, unsigned char flags = 0);

        // Closing a file. 
        // Does nothing if the file is not open.
        void close() const;

        // Reading a some binary units from the file data.
        // Note that 'outputBuffer' must be pre-allocated with size of 'requiredUnitsNum'.
        template<typename unit_type>
        void readSome(unit_type*& outputBuffer, const size_t& requiredUnitsNum)
        {
            FFS_FIO_NULLFH_HANDLER();

            const size_t readSize = fread(outputBuffer, sizeof(unit_type), requiredUnitsNum, fileHandler);
            if(readSize != requiredUnitsNum)
            {
#ifdef FFS_DEBUG
                printf("ffilesys: Wrong readSize value: (rs)%lu vs (run)%lu\n", readSize, requiredUnitsNum);
#endif

                throw Exception(errnoToErrorCode());
            }
        }

        // Reading a one binary unit of data from the file.
        template<typename unit_type>
        void readOne(unit_type& unit)
        {
            unit_type* readPointer = &unit;

            readSome<unit_type>(readPointer, 1);
        }

        // Writing a some binary units into the file data.
        template<typename unit_type>
        void writeSome(const unit_type* inputBuffer, const size_t& inputUnitsNum)
        {
            FFS_FIO_NULLFH_HANDLER();

            const size_t writenSize = fwrite(inputBuffer, sizeof(unit_type), inputUnitsNum, fileHandler);
            if(writenSize != inputUnitsNum)
            {
                throw Exception(errnoToErrorCode());
            }
        }

        // Writing a one binary unit into the file data.
        template<typename unit_type>
        void writeOne(const unit_type unit)
        {
            writeSome<unit_type>(&unit, 1);
        }

        // Reading a one character from the file data.
        void readChar(char& c)
        {
            readOne<char>(c);
        }

        // Writing a one character into the file data.
        void writeChar(const char c)
        {
            writeOne<char>(c);
        }

        // Writing a string characters into the file data.
        template<typename string_char_t>
        void writeString(const std::basic_string<string_char_t>& str)
        {
            writeSome<string_char_t>(str.c_str(), str.size());
        }

        // Just a wrapper for fseek.
        void seek(const long offset, const int origin) const {
            if (!fseek(fileHandler, offset, origin)) {
                throw Exception(errnoToErrorCode());
            }
        }

        // Wrapper, same as seek().
        [[nodiscard]] long tell() const {
            const long result = ftell(fileHandler);

            if (result == -1L) {
                throw Exception(errnoToErrorCode());
            }

            return result;
        }

    private:

        // Generating a fopen flags string from bitflags.
        static void createStrFlags(unsigned char flags);

        FILE* fileHandler = nullptr;
    };

    // Reading the whole file at once into the std::string output.
    // Do NOT use it for large files.
    // It's made for very tiny files that don't need their own file handler.
    void readFullFile(const std::string& filename, std::string& output);

    // Writing/rewriting a whole file at once from the std::string output. 
    void writeFullFile(const std::string& filename, const std::string& input);
}
