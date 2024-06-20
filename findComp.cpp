#include <iostream>
#include <cstring>
#include "PGMimageProcessor.h"
#include "ConnectedComponent.h"

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " [options] <inputPGMfile>" << std::endl;
        return 1;
    }

    // Default option values
    int minSize = 3; // Minimum valid component size
    int maxSize = -1; // Maximum valid component size (unlimited)
    unsigned char threshold = 128; // Threshold for component detection
    std::string outFileName; // Output PGM file name
    bool printData = false; // Flag to print component data

    // Parse command line options
    for (int i = 1; i < argc - 1; ++i) {
        if (strcmp(argv[i], "-s") == 0 && i + 2 < argc) {
            minSize = std::stoi(argv[++i]);
            maxSize = std::stoi(argv[++i]);
        } else if (strcmp(argv[i], "-t") == 0 && i + 1 < argc) {
            threshold = static_cast<unsigned char>(std::stoi(argv[++i]));
        } else if (strcmp(argv[i], "-w") == 0 && i + 1 < argc) {
            outFileName = argv[++i];
        } else if (strcmp(argv[i], "-p") == 0) {
            printData = true;
        } else {
            std::cerr << "Invalid option: " << argv[i] << std::endl;
            return 1;
        }
    }

    // Process the input PGM file
    MSKLEH001::PGMimageProcessor processor(argv[argc - 1]);

    // Extract connected components
    int numComponents = processor.extractComponents(threshold, minSize);

    // Filter components by size
    if (maxSize != -1) {
        processor.filterComponentsBySize(minSize, maxSize);
    }

    // Write components to a new PGM file
    if (!outFileName.empty()) {
        if (processor.writeComponents(outFileName)) {
            std::cout << "Components written to " << outFileName << std::endl;
        } else {
            std::cerr << "Failed to write components to " << outFileName << std::endl;
        }
    }

    // Print component data if requested using -p
    if (printData) {
        std::cout << "Total number of components: " << processor.getComponentCount() << std::endl;
        std::cout << "Size of smallest component: " << processor.getSmallestSize() << std::endl;
        std::cout << "Size of largest component: " << processor.getLargestSize() << std::endl;
        for (int i = 0; i < numComponents; ++i) {
            processor.printComponentData(processor.getComponent(i));
        }
    }

    return 0;
}
