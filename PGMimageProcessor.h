#ifndef PGMIMAGEPROCESSOR_H
#define PGMIMAGEPROCESSOR_H

#include <vector>
#include <string>
#include <memory>
#include "ConnectedComponent.h"

namespace MSKLEH001{

    class PGMimageProcessor {

    private:
        std::unique_ptr<unsigned char[]> image; // For 1D array
        int imageWidth; // Image width
        int imageHeight;  // Image height

        // Private member variable to store connected components
        std::vector<std::shared_ptr<ConnectedComponent>> components;

        // Private member variable to store the input file name
        std::string inputFileName;

        // Private helper function to read the PGM image
        void readPGMImage();

    public:
        // Default constructor
        PGMimageProcessor();

        // Constructor
        PGMimageProcessor(const std::string& filename);

        // Destructor
        ~PGMimageProcessor();

        // Copy constructor
        PGMimageProcessor(const PGMimageProcessor& other);

        // Move constructor
        PGMimageProcessor(PGMimageProcessor&& other) noexcept;

        // Copy assignment operator
        PGMimageProcessor& operator=(const PGMimageProcessor& other);

        // Move assignment operator
        PGMimageProcessor& operator=(PGMimageProcessor&& other) noexcept;

        // Process the input image to extract connected components
        int extractComponents(unsigned char threshold, int minValidSize);

        // Filter out components by size
        int filterComponentsBySize(int minSize, int maxSize);

        // Write components to a new PGM file
        bool writeComponents(const std::string& outFileName);

        // Get the number of components
        int getComponentCount() const;

        // Get the size of the largest component
        int getLargestSize() const;

        // Get the size of the smallest component
        int getSmallestSize() const;

        // Print component data
        void printComponentData(const ConnectedComponent& theComponent) const;

        const ConnectedComponent& getComponent(int index) const;
    };
}

#endif  // PGMIMAGEPROCESSOR_H