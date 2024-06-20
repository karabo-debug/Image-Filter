#include "PGMimageProcessor.h"
#include "ConnectedComponent.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <queue>
#include <limits>

// Default constructor
MSKLEH001::PGMimageProcessor::PGMimageProcessor(){}

// Constructor
MSKLEH001::PGMimageProcessor::PGMimageProcessor(const std::string& filename) : inputFileName(filename) {}

// Destructor
MSKLEH001::PGMimageProcessor::PGMimageProcessor::~PGMimageProcessor() {}

// Copy constructor
MSKLEH001::PGMimageProcessor::PGMimageProcessor(const PGMimageProcessor& other) 
    : components(other.components), inputFileName(other.inputFileName) {}

// Move constructor
MSKLEH001::PGMimageProcessor::PGMimageProcessor(PGMimageProcessor&& other) noexcept 
    : components(std::move(other.components)), inputFileName(std::move(other.inputFileName)) {}

// Copy assignment operator
MSKLEH001::PGMimageProcessor& MSKLEH001::PGMimageProcessor::operator=(const PGMimageProcessor& other) {
    if (this != &other) {
        components = other.components;
        inputFileName = other.inputFileName;
    }
    return *this;
}

// Move assignment operator
MSKLEH001::PGMimageProcessor& MSKLEH001::PGMimageProcessor::operator=(PGMimageProcessor&& other) noexcept {
    if (this != &other) {
        components = std::move(other.components);
        inputFileName = std::move(other.inputFileName);
    }
    return *this;
}

int MSKLEH001::PGMimageProcessor::extractComponents(unsigned char threshold, int minValidSize) {
    // Read the PGM image
    readPGMImage();

    // Initialize component ID
    int componentID = 0;

    // Define a lambda function to check if a pixel is foreground
    auto isForeground = [threshold](unsigned char pixelValue) {
        return pixelValue >= threshold;
    };

    // Define a lambda function to get the index of a pixel
    auto getIndex = [this](int x, int y) {
        return y * imageWidth + x;
    };

    // Define a lambda function to process connected component starting from a given pixel
    auto processComponent = [&](int startX, int startY) {
        // Start BFS to extract connected component
        std::queue<std::pair<int, int>> pixelQueue;
        pixelQueue.push({startX, startY});

        // Create a new connected component object
        auto component = std::make_shared<ConnectedComponent>(componentID++);

        // Perform BFS
        while (!pixelQueue.empty()) {
            int curX, curY;
            std::tie(curX, curY) = pixelQueue.front();
            pixelQueue.pop();

            // Add the current pixel to the component
            component->addPixel(curX, curY);

            // Mark the current pixel as visited by setting it to 0
            image[getIndex(curX, curY)] = 0;

            // Check 4-connected neighbors (N/S/E/W)
            std::pair<int, int> neighbors[] = {{curX, curY - 1}, {curX, curY + 1},
                                                {curX - 1, curY}, {curX + 1, curY}};
            for (const auto& neighbor : neighbors) {
                int nx = neighbor.first;
                int ny = neighbor.second;
                if (nx >= 0 && nx < imageWidth && ny >= 0 && ny < imageHeight) {
                    int neighborIndex = getIndex(nx, ny);
                    if (isForeground(image[neighborIndex])) {
                        // Add neighbor to the queue if it's a foreground pixel
                        pixelQueue.push({nx, ny});
                        // Mark the neighbor as visited
                        image[neighborIndex] = 0;
                    }
                }
            }
        }

        // Check if the component meets the minimum size requirement
        if (component->getSize() >= minValidSize) {
            // Add the component to the vector of components
            components.push_back(component);
        }
    };

    // Iterate through each pixel in the image
    for (int y = 0; y < imageHeight; ++y) {
        for (int x = 0; x < imageWidth; ++x) {
            // Calculate the index of the current pixel in the 1D array
            int index = getIndex(x, y);

            // If the pixel is foreground and has not been visited yet
            if (isForeground(image[index])) {
                // Process connected component starting from this pixel
                processComponent(x, y);
            }
        }
    }

    return components.size();
}

int MSKLEH001::PGMimageProcessor::filterComponentsBySize(int minSize, int maxSize){

    // Define a lambda function to filter components based on size criteria
    auto filterFunc = [minSize, maxSize](const std:: shared_ptr<ConnectedComponent>& component){
        int componentSize = component->getSize();
        return (componentSize >= minSize && (maxSize == -1 || componentSize <= maxSize));
    };

    // Use erase-remove idiom to remove components that do not meet the size criteria
    auto newEnd = std::remove_if(components.begin(), components.end(), filterFunc);

    // Calculate the number of components remaining after filtering
    int remainingComponents = std::distance(components.begin(), newEnd);

    // Erase the removed components from the container
    components.erase(newEnd, components.end());

    return remainingComponents;
}

bool MSKLEH001::PGMimageProcessor::writeComponents(const std::string& outFileName) {
    // Open the output file
    std::ofstream outFile(outFileName, std::ios::binary);
    if (!outFile.is_open()) {
        std::cerr << "Error: Failed to open output file: " << outFileName << std::endl;
        return false;
    }

    // Write the PGM header
    outFile << "P5\n" << imageWidth << " " << imageHeight << "\n255\n";

    // Create a copy of the image data to modify for writing components
    std::unique_ptr<unsigned char[]> imageData(new unsigned char[imageWidth * imageHeight]);
    std::copy(image.get(), image.get() + imageWidth * imageHeight, imageData.get());

    // Write the component pixels to the output file
    for (const auto& component : components) {
        for (const auto& pixel : component->getPixels()) {
            int x = pixel.first;
            int y = pixel.second;

            // Calculate the index of the pixel in the 1D array
            int index = y * imageWidth + x;

            // Set pixel intensity to 255 for component pixels
            imageData[index] = 255;
        }
    }

    // Write the modified image data to the output file
    outFile.write(reinterpret_cast<char*>(imageData.get()), imageWidth * imageHeight);

    // Close the output file
    outFile.close();

    return true;
}


void MSKLEH001::PGMimageProcessor::readPGMImage() {
    std::ifstream file(inputFileName, std::ios::binary);

    if (!file) {
        std::cerr << "Error opening file: " << inputFileName << std::endl;
        return;
    }

    std::string magic;
    getline(file >> std::ws, magic); // Read and discard whitespace, then read magic number

    if (magic != "P5") {
        std::cerr << "Invalid PGM file format." << std::endl;
        return;
    }

    std::string comment;
    while (file.peek() == '#') {
        std::getline(file >> std::ws, comment); // Read and discard comment lines
    }

    file >> imageWidth >> imageHeight;

    // Allocate memory for image data using new[]
    image.reset(new unsigned char[imageWidth * imageHeight]);

    // Read binary data and store in the 1D array
    file.read(reinterpret_cast<char*>(image.get()), imageWidth * imageHeight);

    file.close();
}

int MSKLEH001::PGMimageProcessor::getComponentCount() const {
    return components.size();
}

int MSKLEH001::PGMimageProcessor::getLargestSize() const {
    int largestSize = 0;

    // Iterate through components to find the largest size
    for (const auto& component : components) {
        int componentSize = component->getSize();
        if (componentSize > largestSize) {
            largestSize = componentSize;
        }
    }

    return largestSize;
}

int MSKLEH001::PGMimageProcessor::getSmallestSize() const {
    if (components.empty()) {
        return 0; // No components, return 0
    }

    // Initialize smallestSize with the size of the first component
    int smallestSize = components[0]->getSize();

    // Iterate through components to find the smallest size
    for (const auto& component : components) {
        int componentSize = component->getSize();
        if (componentSize < smallestSize) {
            smallestSize = componentSize;
        }
    }

    return smallestSize;
}

void MSKLEH001::PGMimageProcessor::printComponentData(const ConnectedComponent &theComponent) const {
    std::cout << "Component ID: " << theComponent.getID() << std::endl;
    std::cout << "Number of pixels: " << theComponent.getSize() << std::endl;
}

// Get component by index
const ConnectedComponent& MSKLEH001::PGMimageProcessor::getComponent(int index) const {
    if (index < 0 || index >= components.size()) {
        throw std::out_of_range("Index out of range");
    }
    return *components[index];
}