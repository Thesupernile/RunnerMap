#include <gtest/gtest.h>
#include "testData.hpp"
#include "../fileReader.cpp"

TEST (FileManipulation, ReadWriteSequence){
    network testNetwork {};     // Network object used for testing
    for (auto junction : testData.junctions){
        testNetwork.addJunction(junction);
    }

    fileReader fr = fileReader();
    fr.writeMapData(&testNetwork);


    network readingTestNetwork{};
    fr.readMapData(&readingTestNetwork);

    for(auto junction : testData.junctions){
        EXPECT_EQ(junction.lat, readingTestNetwork.getJunction(junction.id).lat);
        EXPECT_EQ(junction.lon, readingTestNetwork.getJunction(junction.id).lon);
    }
}

TEST (FileManipulation, HashMapReadWrite){
    const std::string filePath = "C:/Programming/NEA/Frontend/mapData/";

    junctionHashMap testHashMap;
    for (auto junction : testData.junctions){
        testHashMap.insertValue(junction);
    }

    std::ofstream fileWriter(filePath + "mapDataHashTest.bin", std::ios::binary);
    testHashMap.storeAsBinary(&fileWriter);
    fileWriter.close();

    testHashMap = junctionHashMap();
    std::ifstream fileReader(filePath + "mapDataHashTest.bin", std::ios::binary);
    testHashMap.readFromBinary(&fileReader);
    fileReader.close();

}
