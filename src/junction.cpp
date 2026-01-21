#ifndef JUNCTION_CPP
#define JUNCTION_CPP

#include <vector>
#include <fstream>
#include <cstdint>

struct connection{
    std::uint64_t connectedNodeId   {0};
    double connectionLength         {0};

    connection(std::uint64_t initConnectedNodeId, double initConnectionLength){
        connectedNodeId = initConnectedNodeId;
        connectionLength = initConnectionLength;
    }

    connection(){

    }
};

class junction{
    public:
    std::uint64_t id    {0};                        // Unique node identifier
    double lat          {0};                        // Latitude of the node
    double lon          {0};                        // Longtitude of the node
    std::vector<connection> connectionsList; // List of arcs, first number in array is node id and second is weight of arc
    junction(std::uint64_t initId = 0, double initLat = 0, double initLon = 0, std::vector<connection> initConnectionsList = std::vector<connection>()){
        id = initId;
        lat = initLat;
        lon = initLon;
        connectionsList = initConnectionsList;
    }

    void storeAsBinary(std::ofstream *fileWriter){
        fileWriter->write(reinterpret_cast<const char*>(&id), sizeof id);
        fileWriter->write(reinterpret_cast<const char*>(&lat), sizeof lat);
        fileWriter->write(reinterpret_cast<const char*>(&lon), sizeof lon);

        size_t connectionListLen = connectionsList.size();
        fileWriter->write(reinterpret_cast<const char*>(&connectionListLen), sizeof connectionListLen);
        for (size_t i = 0; i < connectionListLen; i++){
            connection connectionToWrite = connectionsList[i];
            fileWriter->write(reinterpret_cast<const char*>(&(connectionToWrite.connectedNodeId)), sizeof (connectionToWrite.connectedNodeId));
            fileWriter->write(reinterpret_cast<const char*>(&(connectionToWrite.connectionLength)), sizeof (connectionToWrite.connectionLength));
        }
    }

    void readFromBinary(std::ifstream *fileReader){
        std::vector<connection> newConnectionsList;

        fileReader->read(reinterpret_cast<char*>(&id), sizeof id);
        fileReader->read(reinterpret_cast<char*>(&lat), sizeof lat);
        fileReader->read(reinterpret_cast<char*>(&lon), sizeof lon);


        size_t connectionListLen;
        fileReader->read(reinterpret_cast<char*>(&connectionListLen), sizeof connectionListLen);
        for (size_t i = 0; i < connectionListLen; i++){
            connection newConnection = connection();

            fileReader->read(reinterpret_cast<char*>(&newConnection.connectedNodeId), sizeof newConnection.connectedNodeId);
            fileReader->read(reinterpret_cast<char*>(&newConnection.connectionLength), sizeof newConnection.connectionLength);
            newConnectionsList.push_back(newConnection);
        }

        // Set attributes of this junction to those of the junction read
        connectionsList = newConnectionsList;
    }
};  

#endif
