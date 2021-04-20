#ifndef ARA_UCM_PKGMGR_UCM_PARSING_INFO_H
#define ARA_UCM_PKGMGR_UCM_PARSING_INFO_H

#include <iostream>
#include <vector>
#include <sstream>
#include <string>
#include <fstream>
#include"ara/core/string.h"

#define MAX_LINE_PARSER_NEEDED 6
#define MIN_STRINGS_IN_LINE 0

using namespace std;
using vecString_t = vector<string>;



namespace ara
{
    namespace ucm
    {
        namespace pkgmgr
        {
            class ucmParseInfo
            {
            private:
                ara::core::String pathSpecFile;
                bool fGetFileContent(ara::core::String fileName, vecString_t &vecOfStrs);
                vecString_t &fParsSpecOfUcm(const vecString_t &vecOfString, char delim);

            public:
                ucmParseInfo(ara::core::String P);
                ~ucmParseInfo();
                vecString_t ucmInfo();
            };

        } // namespace pkgmgr
    }     // namespace ucm
} // namespace ara
#endif