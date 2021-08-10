///
/// Written by: Meynard Khel A. Yadao
///

#include <iostream>
#include "PhoneMessagesRecordParser.h"
#include <memory>

int main(int argc, char * argv[])
{
    std::unique_ptr<PhoneMessagesRecordParser> phoneMessagesParser(new PhoneMessagesRecordParser());

    // Print current params for debugging
    for (int i = 0; i < argc; i++)
    {
        printf("param:  %s", argv[i]);
    }
    try
    {
        if (argc >= 2)
        {
            phoneMessagesParser->SetInputFileName(argv[1]); // Assume second parameter as custom input file name
        }

        phoneMessagesParser->ProcessInputFile();
        phoneMessagesParser->CreateMetricFile();
    }
    catch (const char * msg)
    {
        printf("\n\nError encountered: %s\n", msg);
    }
    

    return 0;
}
