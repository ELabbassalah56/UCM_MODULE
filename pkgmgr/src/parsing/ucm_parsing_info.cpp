#include "parsing/ucm_parsing_info.h"

namespace ara
{
    namespace ucm
    {
        namespace pkgmgr
        {
<<<<<<< HEAD
            ucmParseInfo::ucmParseInfo(ara::core::String P) : pathSpecFile{P}
=======
            ucmParseInfo::ucmParseInfo(string P) : pathSpecFile{P}
>>>>>>> bf25fabc8941baef3e8d8669df567de6c3a608ef
            {
            }

            ucmParseInfo::~ucmParseInfo()
            {
            }

<<<<<<< HEAD
            bool ucmParseInfo::fGetFileContent(ara::core::String fileName, vecString_t &vecOfStrs)
=======
            bool ucmParseInfo::fGetFileContent(string fileName, std::vector<std::string> &vecOfStrs)
>>>>>>> bf25fabc8941baef3e8d8669df567de6c3a608ef
            {
                // Open the File
                std::ifstream in(fileName.c_str());
                // Check if object is valid
                if (!in)
                {
                    std::cerr << "Cannot open the File : " << fileName << std::endl;
                    return false;
                }
                std::string str;
                // Read the next line from File untill it reaches the end.
                while (std::getline(in, str))
                {
                    // Line contains string of length > 0 then save it in vector
                    if (str.size() > MIN_STRINGS_IN_LINE)
                    {
                        vecOfStrs.push_back(str);
                    }
                    if ((vecOfStrs.size() >= MAX_LINE_PARSER_NEEDED))
                        break;
                }
                //Close The File
                in.close();
                return true;
            }

<<<<<<< HEAD
            vecString_t &ucmParseInfo::fParsSpecOfUcm(const vecString_t &vecOfString, char delim)
=======
            vecString_t &ucmParseInfo::fParsSpecOfUcm(const vecString_t &vecOfString, const int argSize, char delim)
>>>>>>> bf25fabc8941baef3e8d8669df567de6c3a608ef
            {
                //data holder
                static vecString_t holdOut;

                for (const auto &info : vecOfString)
                {

                    stringstream sStream{info};
                    while (sStream.good())
                    {
                        string subStr;
                        //select data depend on delim
                        getline(sStream, subStr, delim);
                        holdOut.push_back(subStr);
                    }
                }

                return holdOut;
            }

            vecString_t ucmParseInfo::ucmInfo()
            {
                vecString_t vecOfStr;
                vecString_t tempData;
                vecString_t holdData;
                // Get the contents of file in a vector
                bool result = fGetFileContent(pathSpecFile, vecOfStr);
                if (result)
                {
<<<<<<< HEAD
                    tempData = fParsSpecOfUcm(vecOfStr,':');
=======
                    tempData = fParsSpecOfUcm(vecOfStr, vecOfStr.size(), ':');
>>>>>>> bf25fabc8941baef3e8d8669df567de6c3a608ef
                    for (int i = 0; i < tempData.size(); i++)
                    {
                        // holdData.push_back(simple_tokenizer(info));
                        if (i & 1)
                        {
                            holdData.push_back(tempData.at(i));
                        }
                    }

                    return holdData;
                }
            }
        } // namespace pkgmgr
    }     // namespace ucm
} // namespace ara