# PGM Image Processor

# The PGM Image Processor is a C++ program designed to process images in the PGM (Portable Gray Map) file format. It provides functionality to extract connected components from PGM images, filter components based on size criteria, and write the processed components back to a PGM file.

Features

# Extract Components: The program extracts connected components from a PGM image based on a specified threshold value.
# Filter by Size: Components can be filtered based on their size, allowing users to retain only components within a specified size range.
# Write Processed Image: The program writes the processed components to a new PGM file, allowing users to visualize the extracted components.


Compilation: Compile the program using a C++ compiler (e.g., g++).

# 1. run 'make' to compile the program
# 2. Then run the executable file followed by necessary arguments: (for example)
    ./findComp -t 128 -s 50 100 -p -w output.pgm input_images/Chess.pgm
# 3. Adjust the values as needed 

Testing
# 1. run 'make test'
# 2. Then run the executable file:
    ./test 
