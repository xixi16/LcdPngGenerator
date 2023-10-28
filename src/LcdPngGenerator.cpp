/**
 * @file LcdPngGenerator.cpp
 * @author Xing
 * @brief This is the entry point of the application. Main purpose of this application is to: 
 * 1. Retrieve 4 digit IDs from a text file, prefix 2 digit checksum to makeup a 6 digits
 * 2. Convert the 6 digit number to a bit partern which can be recognized by LCD model
 * 3. Save the bit partern to a png file named after the 4 digit ID
 * 4. the png file formate is 1 bit depth(0 as white and 1 as black), 256 pixel width, 1 pixel height 
 * 
 * Dependency: libpng and its dependency zlib(http://www.libpng.org/pub/png/libpng.html)           
 * 
 * @version 0.1
 * @date 2023-10-27
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#include <iostream>
#include <fstream>
#include <string>
#include <set>
#include "CUtility.hpp"

using namespace std;

/**
 * @brief Constants for ID conversion and display partern
 * 
 * @param I_ASSET_ID_LEN The length of input asset ID
 * @param I_CHECKSUM_MOD The number to be used in mod for checksum
 * @param I_CHECKSUM_LEN The restricted length of checksum
 * @param I_DISPLAY_LEN  The length of the LCD bit partern   
 */
const unsigned int  I_ASSET_ID_LEN      = 4;
const unsigned int  I_CHECKSUM_MOD      = 97;
const unsigned int  I_CHECKSUM_LEN      = 2;
const unsigned int  I_DISPLAY_LEN       = 6;

/**
 * @brief Constants for PNG formate
 * 
 * @param I_PNG_DATA_LEN        Length of PNG binary data to be saved
 * @param I_PNG_DATA_OFFSET     Byte offset for LCD display bit partern
 * @param I_PNG_WIDTH           The width of PNG pixel
 * @param I_PNG_HEIGHT          The height of PNG pixel
 * 
 */
const unsigned int  I_PNG_DATA_LEN      = 32; //32 bytes = 256 bits
const unsigned int  I_PNG_DATA_OFFSET   = 1; 
const unsigned int  I_PNG_WIDTH         = 256;
const unsigned int  I_PNG_HEIGHT         = 1;

/**
 * @brief Main entry function, expecting one input argument as the ID text file along with excutable  
 * 
 * @param argc  number of input arguments 
 * @param argv  the set of input argument in string
 * @return int  1 as success, 0 when incorrect argument is found
 */
int main(int argc, char* argv[])
{
    //Parsing input argument, the first one is for input text file
    if(argc < 2)
    {
        //missing file name in the input
        cerr << "Usage: idToPngTest <filename>" << endl;
        return 0; // Exit with an error code
    }

    string txtFilePath = argv[1];
    ifstream txtFile(txtFilePath);
    if (!txtFile.is_open()) 
    {
        cerr << "Failed to open the file." << std::endl;
        return 0; // Exit with an error code
    }

    //local string variables
    string idLineStr, checkSumStr, finalDigitStr, decDisplayStr, pngNameStr; 

    //Initialize image data with null bytes
    string pngImageData(I_PNG_DATA_LEN, '\0'); 

    set<string> idSet; //set to check id uniqueness

    while (getline(txtFile, idLineStr)) 
    {
        // Process each line from the input txt file
        if(idSet.find(idLineStr) != idSet.end())
        {
            //duplicated id found in the file, skip to next id
            cout << "Found a duplicate id: " << idLineStr << endl; 
            continue;
        }

        idSet.insert(idLineStr);

        // Verify if the id is valid
        if(!CUtility::isValidId(idLineStr, I_ASSET_ID_LEN))
        {
            //invalid id, ignore it and skip to next line
            cerr << "Found a wrong formated id: " << idLineStr << endl; 
            continue;
        }

        // Get the checksum code based on requirement defined in constants
        if(!CUtility::getChecksumCode(idLineStr, checkSumStr, I_CHECKSUM_MOD, I_CHECKSUM_LEN))
        {
            //getting wrong in checksum calculation
            cerr << "Failed to generate checksum for id: " << idLineStr << endl; 
            continue;
        }

        finalDigitStr = checkSumStr + idLineStr;  //makeup the final digit

        // Convert the digit to LCD bit partern
        if(!CUtility::convertStringToDecDisplay(finalDigitStr, decDisplayStr))
        {
            //error found during converting from digit to decimal display formate
            cerr << "Couldn't convert the id to display digits: " << idLineStr << endl; 
            continue;
        }

        // Insert the LCD bit partern to image binary data
        pngImageData.assign(pngImageData.size(), '\0'); //cleanup the image data       
        pngImageData.replace(I_PNG_DATA_OFFSET, decDisplayStr.length(), decDisplayStr);

        // Now we are ready to generate the png file, finger crossed!
        pngNameStr = idLineStr + ".png";
        if(!CUtility::createPngImage1BitDepth(pngNameStr, I_PNG_WIDTH, I_PNG_HEIGHT, pngImageData))
        {
            //error found during converting from digit to decimal display formate
            cerr << "Failed to create png file: " << pngNameStr << endl; 
            continue;
        }
    }

    txtFile.close(); // Close the txt file
    return 0;
}

