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
    fileReader(std::string newFilePath = "mapData/"){
        filePath = newFilePath;
    }

    void writeMapData(network* map){

    }

    void extractDataFromOSMFile(){
        network map = network();


        writeMapData(&map);
    }

    void readMapData(network* mapPtr){
        

    }

};
#endif
