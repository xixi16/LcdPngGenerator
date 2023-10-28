/**
 * @file CUtility.cpp
 * @author Xing Jin
 * @brief 
 * @version 0.1
 * @date 2023-10-27
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#include <cmath>
#include <map>
#include <iostream>
#include <png.h>
#include "CUtility.hpp"

using namespace std;

/**
 * @brief The checksum calculation function based on reverted given number mod by given base
 * 
 * @param inputStr   input string of original digit for checksum calculation
 * @param outputStr  the output of checksum code
 * @param mode       The based number for MOD calculation  
 * @param outLen     The restricted length of the number
 * @return int       1 for success, 0 for failure   
 */
int CUtility::getChecksumCode(const string& inputStr, string& outputStr, unsigned int mode, unsigned int outLen)
{
    if(!isFullDigitString(inputStr))
    {
        //The input string isn't a full digit string
        return 0; 
    }

    // Reverse the string
    string reversedStr = string(inputStr.rbegin(), inputStr.rend());

    try
    {
        int checkSum = std::stoi(reversedStr) % mode; 

        //limited to the required length
        checkSum %= static_cast<unsigned int>(pow(10, outLen));

        outputStr = std::to_string(checkSum);        
        if (outputStr.length() < outLen)
        {
            //prefill 0 if the checksum is less than output length
            outputStr.insert(0, outLen - outputStr.length(), '0');
        }
    }
    catch(const std::exception& e)
    {
        // Conversion failed due to invalid argument
        return 0;
    }

    return 1;
}

/**
 * @brief The string(decimal digit only) to LCD display bit partern map
 * 
 */
const map<char, char> C_Map_DecToDisplay
{
    {'0', 0b01110111},
    {'1', 0b01000010},
    {'2', 0b10110110},
    {'3', 0b11010110},
    {'4', 0b11000011},
    {'5', 0b11010101},
    {'6', 0b11110101},
    {'7', 0b01000110},
    {'8', 0b11110111},
    {'9', 0b11010111}    
};

/**
 * @brief Convert given string to bit partern pre-defined in map
 * 
 * @param inputStr  The input string to be converted
 * @param outputStr The converted bit partern string
 * @return int      1 for success, 0 for failure
 */
int CUtility::convertStringToDecDisplay(const std::string& inputStr, string& outputStr)
{
    outputStr.clear(); //cleanup the output buffer

    for (char digit : inputStr) 
    {
        //convert each digit to bit partern defined in map
        if (auto search = C_Map_DecToDisplay.find(digit); search != C_Map_DecToDisplay.end())
        {
            outputStr += C_Map_DecToDisplay.at(digit);
        }
        else
        {
            //The given digit is undefined, return with error
            return 0;
        }
    }

    return 1;
}

/**
 * @brief Function to check if the given ID string is in valid formate
 * 
 * @param idStr     the ID string to be checked
 * @param idLength  the defined length of ID
 * @return true     if the string is in valid formate
 * @return false    The string is in wrong formate
 */
bool CUtility::isValidId(const std::string& idStr, unsigned int idLength)
{
    if(idLength != idStr.length())
    {
        //we don't support the different length of digit string.
        return false;
    }

    if(!isFullDigitString(idStr))
    {
        //The input string isn't a full digit string
        return false; 
    }

    return true;
}

/**
 * @brief The function is to generate an 1 bit width(0 white, 1 black) PNG file based on given file name and data.
 * libpng is used in the function, and code is referenced to an example from libpng.org   
 * 
 * @param fileName  the file name and path of the PNG file
 * @param imgWidth  the width of the pixels 
 * @param imgHeight the height of the pixel  
 * @param data      the image data stored in a string buffer
 * @return true     file generation suceeds
 * @return false    failed to generate the png file 
 */
bool CUtility::createPngImage1BitDepth(const string& fileName, int imgWidth, int imgHeight, string& data) 
{

    // Create a file for writing the PNG image
    FILE* pngFilePtr = fopen(fileName.c_str(), "wb");
    if (!pngFilePtr) 
    {
        cerr << "Failed to create the file for writing the PNG image." << std::endl;
        return false;
    }

    // Initialize the PNG write structure
    png_structp pngStructPtr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!pngStructPtr) 
    {
        cerr << "Failed to initialize the PNG write structure." << std::endl;
        return false;
    }

    // Initialize the PNG info structure
    png_infop pngInfo = png_create_info_struct(pngStructPtr);
    if (!pngInfo) 
    {
        cerr << "Failed to initialize the PNG info structure." << std::endl;
        return false;
    }

    // Set the error handling
    if (setjmp(png_jmpbuf(pngStructPtr))) 
    {
        cerr << "Error during PNG creation." << std::endl;
        return false;
    }

    // Initialize the PNG image data
    png_init_io(pngStructPtr, pngFilePtr);
    png_set_IHDR(
        pngStructPtr,
        pngInfo,
        imgWidth,
        imgHeight,
        1,  // 1-bit depth
        PNG_COLOR_TYPE_GRAY,
        PNG_INTERLACE_NONE,
        PNG_COMPRESSION_TYPE_DEFAULT,
        PNG_FILTER_TYPE_DEFAULT
    );
    png_write_info(pngStructPtr, pngInfo);

    //by default monochrome as black being zero and white being one, revert it
    png_set_invert_mono(pngStructPtr); 

    png_bytep rowData = reinterpret_cast<png_bytep>(data.data());

    for (int y = 0; y < imgHeight; y++) {
        png_write_row(pngStructPtr, rowData);
        rowData += imgWidth;
    }

    // End writing
    png_write_end(pngStructPtr, NULL);
    fclose(pngFilePtr);

    // Clean up resource
    if (pngStructPtr && pngInfo)
    {
        png_destroy_write_struct(&pngStructPtr, &pngInfo);
    }

    std::cout << "PNG image created: " << fileName << std::endl;

    return true;
}
