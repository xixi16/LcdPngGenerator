import argparse
import os
import png

# The `mod_base_number` variable is set to 97. It is used in the `addChecksum` function to calculate a
# checksum for the ID string. The checksum is obtained by taking the remainder of the reversed ID
# string divided by `mod_base_number`. The purpose of the checksum is to add an additional digit to
# the ID string for error detection.
mod_base_number = 97

# The `char_to_number` dictionary maps each character from '0' to '9' to a corresponding hexadecimal
# number. These hexadecimal numbers represent the LCD pattern for each digit when displayed on a
# 7-segment display. For example, '0' is mapped to 0x77, which represents the LCD pattern for
# displaying the digit 0.
char_to_number = {
    '0': 0x77,
    '1': 0x42,
    '2': 0xB6,
    '3': 0xD6,
    '4': 0xC3,
    '5': 0xD5,
    '6': 0xF5,
    '7': 0x46,
    '8': 0xF7,
    '9': 0xD7,
}

def addChecksum(id_str):
    """
    The function `addChecksum` takes a string `id_str`, reverses it, calculates a checksum number based
    on a `mod_base_number`, and returns the checksum number concatenated with the original string.
    
    :param id_str: The `id_str` parameter is a string representing an identification number
    :return: a string that consists of the checksum number followed by the original id_str.
    """
    reversed_id_str = id_str[::-1]
    checksum_number = int(reversed_id_str) % mod_base_number
    return f"{checksum_number:02d}" + id_str

def convertToLcdPartern(idWithChecksum_str):
    """
    The function takes a string as input and converts it to a number by mapping each character to a
    corresponding number and combining them using bitwise operations.
    
    :param idWithChecksum_str: The parameter `idWithChecksum_str` is a string that represents an ID with
    a checksum
    :return: the result_number, which is an integer value.
    """
    result_number = 0
    for char in idWithChecksum_str:
        if char in char_to_number:
            result_number = (result_number << 8) | char_to_number[char]
        else:
            return 0 
    return result_number   

def converForPngDataArray(png_data, lcdPartern_str):
    """
    The function `converForPngDataArray` takes in a PNG data array and an LCD pattern string, and
    appends a new row to the PNG data array based on the LCD pattern string.
    
    :param png_data: The `png_data` parameter is a list that contains the rows of pixel data for a PNG
    image. Each row is represented as a list of integers, where each integer represents the color value
    of a pixel
    :param lcdPartern_str: The lcdPartern_str parameter is a string that represents a pattern of LCD
    pixels. Each character in the string represents a pixel, where '0' represents an off pixel and any
    other character represents an on pixel
    """
    png_row = []
    png_data.append(png_row)
    
    for c in reversed(lcdPartern_str):
        if c == '0':
            png_row.append(0xFF)
        else:
            png_row.append(0x0)

def process_file(filename, output_dir):
    """
    The `process_file` function reads a file line by line, checks if each line is a valid ID, converts
    the ID to an LCD pattern, and saves the pattern as a PNG file in the specified output directory.
    
    :param filename: The name of the file that contains the IDs to be processed
    :param output_dir: The output directory where the generated PNG files will be saved
    """
    try:
        directory_path = os.path.dirname(output_dir)
        # Ensure the directory exists, creating it if necessary
        os.makedirs(directory_path, exist_ok=True)    
        
        # Open the file for reading
        with open(filename, "r") as file:
            # Read each line in the file
            for line in file:
                # Remove leading and trailing whitespace (e.g., newline characters)
                id = line.strip()
                
                # Check if the ID has exactly 4 digits
                if len(id) == 4 and id.isdigit():
                    idWithChecksum_str = addChecksum(id)
                    lcdPartern_number = convertToLcdPartern(idWithChecksum_str)
                    if lcdPartern_number > 0:
                        lcdPartern_number <<= 8
                        lcdPartern_str = format(lcdPartern_number, '0256b')
                        
                        #let's pad 256 characters into a list                    
                        png_data = []
                        converForPngDataArray(png_data, lcdPartern_str)
                        png_writer = png.Writer(width=256, height=1, greyscale=True, bitdepth=8)
                        
                        file_path = directory_path + '/' + id + '.png'
                        with open(file_path, "wb") as file:
                            png_writer.write(file, png_data)
                            file.close()
                    else:
                        print(f"failed to convert ID: {id}, to LCD partern, skip it")
                else:
                    print(f"Invalid ID: {id}, skip")
    except Exception as e:
        print(f"An error occured when create file: {e} with path: {output_dir}" )


def main():
    """
    The main function processes a text file with 4-digit IDs and saves the output in a specified
    directory.
    """
    # Create an argument parser
    parser = argparse.ArgumentParser(description="Process a text file with 4-digit IDs")

    # Add arguments for the filename, help file, and output directory
    parser.add_argument("-f", "--filename", help="The name of the text file containing 4-digit IDs")
    parser.add_argument("-o", "--output", help="The output directory")

    # Parse the command-line arguments
    args = parser.parse_args()

    # Call the function to process the file
    process_file(args.filename, args.output)

if __name__ == "__main__":
    main()
