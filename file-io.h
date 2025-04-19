#pragma once

#include <stdio.h>
#include <string>
#include <filesystem>

#include "exception.h"

#ifdef FFS_FIO_HANDLE_NULLFH
    #define FFS_FIO_NULLFH_HANDLER() \
        if(!fileHandler) { return EC_NoActiveFile; }
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

    static constexpr int NUM_FILE_OPEN_FLAGS = 4;

    // A Input/Output file manager, based on stdio.h FILE.
    // Mostly doesnt supports multithreading(but need to test).
    class File
    {
    public:
                
        // Openning a new file. 
        // You can specify flags in ffilesys::FileOpenFlags, for example
        // .open(ffilesys::FOF_Read | ffilesys::FOF_Binary)
        // will open a binary input stream.
        void open(const std::string& filename, unsigned char flags = 0);

        // Closing a file. 
        // Does nothing if the file is not open.
        inline void close()
        {
            fclose(fileHandler);
        }

        // Reading a some binary units from the file data.
        // Note that 'outputBuffer' must be pre-allocated with size of 'requiredUnitsNum'.
        template<typename unit_type>
        void readSome(unit_type*& outputBuffer, const size_t& requiredUnitsNum)
        {
            FFS_FIO_NULLFH_HANDLER();

            const size_t readedSize = fread(outputBuffer, sizeof(unit_type), requiredUnitsNum, fileHandler);
            if(readedSize != requiredUnitsNum)
            {
#ifdef FFS_DEBUG
                printf("ffilesys: Wrong readedSize value: (rs)%lu vs (run)%lu\n", readedSize, requiredUnitsNum);
#endif
                throw Exception(errnoToErrorCode());
            }
        }

        // Reading a one binary unit of data from the file.
        template<typename unit_type>
        inline void readOne(unit_type& unit)
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
        inline void writeOne(const unit_type unit)
        {
            writeSome<unit_type>(&unit, 1);
        }

        // Reading a one character from the file data.
        inline void readChar(char& c) 
        {
            readOne<char>(c);
        }

        // Writing a one character into the file data.
        inline void writeChar(const char c)
        {
            writeOne<char>(c);
        }

        // Writing a string characters into the file data.
        template<typename string_char_t>
        inline void writeString(const std::basic_string<string_char_t>& str)
        {
            writeSome<string_char_t>(str.c_str(), str.size());
        }

    private:

        // Generating a fopen flags string from bitflags.
        void createStrFlags(unsigned char flags);

        FILE* fileHandler = 0;
    };

    // Reading the whole file at once into the std::string output.
    // Do NOT use it for large files.
    // Its createn for very tiny files that dont need a their own file handler.
    void readFullFile(const std::string& filename, std::string& output);

    // Writing/rewriting a whole file at once from the std::string output. 
    void writeFullFile(const std::string& filename, const std::string& input);
}
