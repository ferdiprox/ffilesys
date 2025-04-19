#include "file-io.h"

namespace ffilesys
{
    int FILE_OPEN_FLAGS_VALUES[NUM_FILE_OPEN_FLAGS]
    {
        FOF_Read,
        FOF_Write,
        FOF_Append,
        FOF_Binary,
    };

    char FILE_OPEN_FLAGS_CHARS[NUM_FILE_OPEN_FLAGS]
    {
        'r',
        'w',
        'a',
        'b',
    };

    char fileOpenFlagsBuffer[NUM_FILE_OPEN_FLAGS + 1];

    void File::open(const std::string& filename, unsigned char flags)
    {
        createStrFlags(flags);

        fileHandler = fopen(filename.c_str(), fileOpenFlagsBuffer);

        if(!fileHandler)
        {
            throw errnoToErrorCode();
        }
    }
    
    void File::createStrFlags(unsigned char flags)
    {
        int writeOffset = 0;
        for(int i = 0; i < NUM_FILE_OPEN_FLAGS; i++)
        {
            if(flags & FILE_OPEN_FLAGS_VALUES[i])
            {
                fileOpenFlagsBuffer[writeOffset++] = FILE_OPEN_FLAGS_CHARS[i];
            }
        }

        fileOpenFlagsBuffer[writeOffset] = '\0';
    }

    void readFullFile(const std::string& filename, std::string& output)
    {
        if(!std::filesystem::exists(filename))
        {
            throw Exception(EC_NoSuchFileOrDir);
        }

        size_t targetFileSize;

        try 
        {
            targetFileSize = std::filesystem::file_size(filename);
        } catch(...)
        {
            throw Exception(EC_UnknownError);
        }

        FILE* targetFileHandler = fopen(filename.c_str(), "rb");

        if(!targetFileHandler)
        {
            throw Exception(errnoToErrorCode());
        }

        output.resize(targetFileSize);

        const size_t readedSize = fread(output.data(), 1, targetFileSize, targetFileHandler);

        fclose(targetFileHandler);

        if(readedSize != targetFileSize)
        {
            throw Exception(EC_UnknownError);
        }
    }

    void writeFullFile(const std::string& filename, const std::string& input)
    {
        FILE* targetFileHandler = fopen(filename.c_str(), "wb");

        if(!targetFileHandler)
        {
            throw Exception(errnoToErrorCode());
        }

        const size_t writenSize = fwrite(input.data(), 1, input.size(), targetFileHandler);

        fclose(targetFileHandler);

        if(writenSize != input.size())
        {
            throw Exception(EC_UnknownError);
        }
    }
}