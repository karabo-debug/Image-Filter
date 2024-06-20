#ifndef CONNECTEDCOMPONENT_H
#define CONNECTEDCOMPONENT_H

#include <vector>
#include <utility> // For std::pair

class ConnectedComponent {
public:
    // Constructor
    ConnectedComponent(int id);

    // Destructor
    ~ConnectedComponent();

    // Copy constructor
    ConnectedComponent(const ConnectedComponent& other);

    // Move constructor
    ConnectedComponent(ConnectedComponent&& other) noexcept;

    // Copy assignment operator
    ConnectedComponent& operator=(const ConnectedComponent& other);

    // Move assignment operator
    ConnectedComponent& operator=(ConnectedComponent&& other) noexcept;

    // Add pixel coordinates to the component
    void addPixel(int x, int y);

    // Get the number of pixels in the component
    int getSize() const;

    // Get the unique identifier of the component
    int getID() const;

    // Get the pixel coordinates of the component
    const std::vector<std::pair<int, int>>& getPixels() const;

private:
    // Private member variables
    int componentID;
    int pixelCount;
    std::vector<std::pair<int, int>> pixels;
};

#endif // CONNECTEDCOMPONENT_H
