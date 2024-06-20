#include "ConnectedComponent.h"

// Constructor
ConnectedComponent::ConnectedComponent(int id) : componentID(id), pixelCount(0) {}

// Destructor
ConnectedComponent::~ConnectedComponent() {}

// Copy constructor
ConnectedComponent::ConnectedComponent(const ConnectedComponent& other)
    : componentID(other.componentID), pixelCount(other.pixelCount), pixels(other.pixels) {}

// Move constructor
ConnectedComponent::ConnectedComponent(ConnectedComponent&& other) noexcept
    : componentID(std::move(other.componentID)), pixelCount(std::move(other.pixelCount)), pixels(std::move(other.pixels)) {}

// Copy assignment operator
ConnectedComponent& ConnectedComponent::operator=(const ConnectedComponent& other) {
    if (this != &other) {
        componentID = other.componentID;
        pixelCount = other.pixelCount;
        pixels = other.pixels;
    }
    return *this;
}

// Move assignment operator
ConnectedComponent& ConnectedComponent::operator=(ConnectedComponent&& other) noexcept {
    if (this != &other) {
        componentID = std::move(other.componentID);
        pixelCount = std::move(other.pixelCount);
        pixels = std::move(other.pixels);
    }
    return *this;
}

// Add pixel coordinates to the component
void ConnectedComponent::addPixel(int x, int y) {
    pixels.emplace_back(x, y);
    pixelCount++;
}

// Get the number of pixels in the component
int ConnectedComponent::getSize() const {
    return pixelCount;
}

// Get the unique identifier of the component
int ConnectedComponent::getID() const {
    return componentID;
}

// Get the pixel coordinates of the component
const std::vector<std::pair<int, int>>& ConnectedComponent::getPixels() const {
    return pixels;
}