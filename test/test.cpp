
#include <iostream>

#include "file-io.h"

void testFullTextWriting()
{
    std::string filedata = "I was b0rn in th3 USA.";

    try 
    {
        ffilesys::writeFullFile("inter-test.txt", filedata);
    }
    catch(ffilesys::Exception const& exception)
    {
        ffilesys::verboseCode(exception.code);
        return;
    }

    std::cout << "Writen text data\n";
}

void testFullTextReading()
{
    std::string filedata;
    
    try
    {
        ffilesys::readFullFile("inter-test.txt", filedata);
    } 
    catch(ffilesys::Exception const& exception)
    {
        ffilesys::verboseCode(exception.code);
        return;
    }

    std::cout << "inter-test.txt data:" << filedata << '\n';
}

void testStreamBinWriting()
{
    ffilesys::File outputFile;

    try 
    {
        outputFile.open("inter-test.bin", ffilesys::FOF_Write | ffilesys::FOF_Binary);

        int worstNumberEver = 42;

        outputFile.writeOne(worstNumberEver);

        outputFile.close();
        
        std::cout << "Writen the worst number ever.\n";
    }
    catch(ffilesys::Exception const& exception)
    {
        ffilesys::verboseCode(exception.code);
        return;
    }    
}

void testStreamBinReading()
{
    ffilesys::File inputFile;

    try 
    {
        inputFile.open("inter-test.bin", ffilesys::FOF_Read | ffilesys::FOF_Binary);

        int answer2universe;

        inputFile.readOne(answer2universe);

        inputFile.close();

        std::cout << "Answer to universe: " << answer2universe << '\n';
    }
    catch(ffilesys::Exception const& exception)
    {
        ffilesys::verboseCode(exception.code);
        return;
    }
}

int main()
{
    testFullTextWriting();
    testFullTextReading();

    testStreamBinWriting();
    testStreamBinReading();

    return 0;
}