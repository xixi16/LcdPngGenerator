/**
 * @file CUtility.hpp
 * @author Xing Jin
 * @brief  The header file for the utility class CUtility
 * @version 0.1
 * @date 2023-10-27
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#include <string>

/**
 * @brief The common utility class for re-use purpose
 * 
 */
class CUtility
{
public:
    //CheckSum based on MOD generation
    static int getChecksumCode(const std::string&, std::string&, unsigned int, unsigned int);

    //Convert whole data of a buffer to decimal lcd display format
    static int convertStringToDecDisplay(const std::string&, std::string&);

    //Check if the string contains valid digits
    static bool isValidId(const std::string&, unsigned int);

    //Create PNG file based on given file name and image data
    static bool createPngImage1BitDepth(const std::string&, int, int, std::string&);

    //Check if a string contains digits only
    static inline bool isFullDigitString(const std::string& inputStr)
    {
        for (char c : inputStr) 
        {
            // return if there is non-digit charater found
            if (!std::isdigit(c)) return false; 
        }
        return true;
    }    
    
};