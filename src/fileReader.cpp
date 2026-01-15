#ifndef FILEREADER_CPP
#define FILEREADER_CPP
#include <string>
#include <fstream>
#include "network.cpp"
#include "osmiumFileHandler.cpp"

class fileReader{
    private:
    std::string filePath;

    void extractDataFromOSMFile(network* mapPtr){
        network map = network();
        
        writeMapData(&map);
    }

    public:

    fileReader(std::string newFilePath = "C:/Programming/NEA/Frontend/mapData/"){
        filePath = newFilePath;
    }

    void writeMapData(network* mapPtr){
        std::ofstream writer(filePath + "mapData.bin", std::ios::binary);
        mapPtr->storeAsBinary(&writer);

        writer.close();
    }

    void readMapData(network* mapPtr){
        std::ifstream reader(filePath + "mapData.bin", std::ios::binary);

        mapPtr->readFromBinary(&reader);

        reader.close();
    }

    void getStoredMap(network* mapPtr){
        // Try to read from the mapping file. If this fails, we read from the raw OSM file
        try{
            readMapData(mapPtr);
        }
        catch(int errorCode){
            extractDataFromOSMFile(mapPtr);
        }
    }

};
#endif
