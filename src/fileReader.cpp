#ifndef FILEREADER_CPP
#define FILEREADER_CPP
#include <string>
#include <fstream>
#include "network.cpp"
#include "osmiumFileHandler.cpp"

class fileReader{
    private:
    std::string filePath;

    public:

    void extractDataFromOSMFile(network* mapPtr){
        network map = network();
        osmiumFileHandler osmFileReader = osmiumFileHandler();
        osmFileReader.extractData(&map);
        
        writeMapData(&map);
    }

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
        // Try to read from the mapping file. If this fails, we read from the raw OSM file if both fail we return an error
        try{
            readMapData(mapPtr);
            if (mapPtr->isEmpty()){
                throw std::runtime_error("Map extracted from file is empty");
            }
        }
        catch(int errorCode){
            try{
                extractDataFromOSMFile(mapPtr);
            }
            catch(int errorCode){
                throw std::runtime_error("OSM File cannot be read");
            }
        }
    }
};
#endif
