#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "PGMimageProcessor.h"

TEST_CASE("Test extracting components from image") {
    // Create an instance of PGMimageProcessor
    MSKLEH001::PGMimageProcessor processor("input_images/Chess.pgm");

    // Extract components
    int actualComponentCount = processor.extractComponents(200, 3);

    // Check if components were extracted
    REQUIRE(actualComponentCount > 0);

    SECTION("Test filtering components by size") {
        // Filter components by size
        int remainingComponents = processor.filterComponentsBySize(10, 100); // Adjust minSize and maxSize as needed

        // Check if components were filtered correctly
        REQUIRE(remainingComponents > 0);
    }

    SECTION("Test writing components to file") {
        // Write components to output file
        bool success = processor.writeComponents("output.pgm");

        // Check if components were written successfully
        REQUIRE(success == true);
    }
}

TEST_CASE("Test getting component count") {
    MSKLEH001::PGMimageProcessor processor("input_images/Chess.pgm");
    int componentCount = processor.extractComponents(200, 3);

    // Check if the component count matches the expected value
    REQUIRE(processor.getComponentCount() == componentCount);
}

TEST_CASE("Test getting largest component size") {
    MSKLEH001::PGMimageProcessor processor("input_images/Chess.pgm");
    processor.extractComponents(200, 3);
    int largestSize = processor.getLargestSize();

    // Check if the largest size is greater than or equal to 0
    REQUIRE(largestSize >= 0);
}

TEST_CASE("Test getting smallest component size") {
    MSKLEH001::PGMimageProcessor processor("input_images/Chess.pgm");
    processor.extractComponents(200, 3);
    int smallestSize = processor.getSmallestSize();

    // Check if the smallest size is greater than or equal to 0
    REQUIRE(smallestSize >= 0);
}

TEST_CASE("Test getting component by index") {
    MSKLEH001::PGMimageProcessor processor("input_images/Chess.pgm");
    processor.extractComponents(200, 3);
    int componentCount = processor.getComponentCount();

    if (componentCount > 0) {
        // Check getting components by index within valid range
        for (int i = 0; i < componentCount; ++i) {
            REQUIRE_NOTHROW(processor.getComponent(i));
        }

        // Check getting component by index out of range
        REQUIRE_THROWS_AS(processor.getComponent(componentCount), std::out_of_range);
    }
}

TEST_CASE("Test copying PGMimageProcessor object") {
    MSKLEH001::PGMimageProcessor processor1("input_images/Chess.pgm");
    processor1.extractComponents(200, 3);

    // Copy constructor
    MSKLEH001::PGMimageProcessor processor2(processor1);
    REQUIRE(processor1.getComponentCount() == processor2.getComponentCount());

    // Copy assignment operator
    MSKLEH001::PGMimageProcessor processor3;
    processor3 = processor1;
    REQUIRE(processor1.getComponentCount() == processor3.getComponentCount());
}

TEST_CASE("Test moving PGMimageProcessor object") {
    MSKLEH001::PGMimageProcessor processor1("input_images/Chess.pgm");
    processor1.extractComponents(200, 3);

    // Move constructor
    MSKLEH001::PGMimageProcessor processor2(std::move(processor1));
    REQUIRE(processor2.getComponentCount() > 0);

    // Move assignment operator
    MSKLEH001::PGMimageProcessor processor3;
    processor3 = std::move(processor2);
    REQUIRE(processor3.getComponentCount() > 0);
}
