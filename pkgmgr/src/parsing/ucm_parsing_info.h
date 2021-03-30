#ifndef ARA_UCM_PKGMGR_UCM_PARSING_INFO_H
#define ARA_UCM_PKGMGR_UCM_PARSING_INFO_H

#include <iostream>
#include <vector>
#include <sstream>
#include <string>
#include <fstream>

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
                string pathSpecFile;
                bool fGetFileContent(string fileName, vecString_t &vecOfStrs);
                vecString_t &fParsSpecOfUcm(const vecString_t &vecOfString, const int argSize, char delim);

            public:
                ucmParseInfo(string P);
                ~ucmParseInfo();
                vecString_t ucmInfo();
            };

        } // namespace pkgmgr
    }     // namespace ucm
} // namespace ara
#endif