#ifndef ARA_UCM_PKGMGR_UCM_PARSING_INFO_H
#define ARA_UCM_PKGMGR_UCM_PARSING_INFO_H

#include <iostream>
#include <vector>
#include <sstream>
#include <string>
#include <fstream>
<<<<<<< HEAD
<<<<<<< HEAD
#include"ara/core/string.h"
=======
>>>>>>> bf25fabc8941baef3e8d8669df567de6c3a608ef
=======
>>>>>>> bf25fabc8941baef3e8d8669df567de6c3a608ef

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
<<<<<<< HEAD
<<<<<<< HEAD
                ara::core::String pathSpecFile;
                bool fGetFileContent(ara::core::String fileName, vecString_t &vecOfStrs);
                vecString_t &fParsSpecOfUcm(const vecString_t &vecOfString, char delim);

            public:
                ucmParseInfo(ara::core::String P);
=======
=======
>>>>>>> bf25fabc8941baef3e8d8669df567de6c3a608ef
                string pathSpecFile;
                bool fGetFileContent(string fileName, vecString_t &vecOfStrs);
                vecString_t &fParsSpecOfUcm(const vecString_t &vecOfString, const int argSize, char delim);

            public:
                ucmParseInfo(string P);
<<<<<<< HEAD
>>>>>>> bf25fabc8941baef3e8d8669df567de6c3a608ef
=======
>>>>>>> bf25fabc8941baef3e8d8669df567de6c3a608ef
                ~ucmParseInfo();
                vecString_t ucmInfo();
            };

        } // namespace pkgmgr
    }     // namespace ucm
} // namespace ara
#endif