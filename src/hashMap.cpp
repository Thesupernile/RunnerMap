#include "haversine.cpp"
#include <vector>
#include <fstream>
#include <tuple>
#include <stdexcept>
#include <cstdint>
#include <iostream>

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

class junctionHashMap{
    // An open hashed container used to store nodes
    public:
        static const int CAPACITY = 9311;           // 9311 chosen since it is a large prime
        std::vector<junction> mapList[CAPACITY];


        std::uint64_t hash(std::uint64_t key){
            // TK INSERT A REAL HASH FUNCTION
            return key % CAPACITY;
        }

    public:
        junctionHashMap(){

        }

        void insertValue(junction value){
            // Insert a value into the hash map
            const std::uint64_t key = value.id;
            const std::uint64_t index = hash(key);

            std::vector<junction> *keyLine = &(mapList[index]);
            if (keyLine->size() < 1){
                keyLine->push_back(value);
            }
            else{
                bool keyFound = false;
                for(int i = 0; i < keyLine->size(); i++){
                    if ((*keyLine)[i].id == key){
                        keyFound = true;
                        break;
                    }
                }
                if (!keyFound){
                    keyLine->push_back(value);
                }
            }

        }

        bool containsKey(std::uint64_t targetKey){
            // Used to check if the hashmap contains the given key
            std::uint64_t index = hash(targetKey);
            std::vector<junction> *keyLine = &(mapList[index]);
            for (auto &key : *keyLine){
                if (key.id == targetKey){
                    return true;
                }
            }
            return false;
        }

        void cull(){
            // Used to delete all nodes that are "empty" (have no neighbours)
            for (int i = 0; i < CAPACITY; i++){
                std::vector<junction> *keyLine = &(mapList[i]);

                for (int j = 0; j < keyLine->size(); j++){
                    if (((*keyLine)[j]).connectionsList.size() < 1){
                        keyLine->erase (keyLine->begin()+j);
                        j--;
                    }
                }
            }
        }

        junction accessValue(std::uint64_t key){
            // Accesses a value from the hashmap given a junctionID
            std::uint64_t index = hash(key);
            std::vector<junction> *keyLine = &(mapList[index]);
            for (int i = 0; i < keyLine->size(); i++){
                if ((*keyLine)[i].id == key){
                    return (*keyLine)[i];
                }
            }

            throw std::invalid_argument("Key not in array");
        }


        std::uint64_t getJunctionByPosition(double reqLat, double reqLon){
            // Uses a linear search to find the closest junction to the input coords
            double shortestDistance = INFINITY;
            std::uint64_t bestJunctionId {};

            for (int i = 0; i < CAPACITY; i++){
                std::vector<junction> *keyLine = &(mapList[i]);
                for (int j = 0; j < keyLine->size(); j++){
                    double distance = haversine(reqLat, reqLon, (*keyLine)[j].lat, (*keyLine)[j].lon);
                    if (distance < shortestDistance){
                        shortestDistance = distance;
                        bestJunctionId = (*keyLine)[j].id;
                    }
                }
            }

            return bestJunctionId;
        }

        void storeAsBinary(std::ofstream *fileWriter){
            // Writes each element of the hashMap to binary using the fileReader passed in (as pointer)
            for (int i = 0; i < CAPACITY; i++){
                std::vector<junction> *keyLine = &(mapList[i]);
                size_t keyLineSize = keyLine->size();
                fileWriter->write(reinterpret_cast<const char*>(&keyLineSize), sizeof &keyLineSize);
                for (int j = 0; j < keyLine->size(); j++){
                    junction* junctionPtr = &((*keyLine)[j]);
                    junctionPtr->storeAsBinary(fileWriter);
                }
            }
        }

        void readFromBinary(std::ifstream *fileReader){
            // Reads each element of the hashMap from binary using the fileReader passed in (as pointer)
            for (int i = 0; i < CAPACITY; i++){
                std::vector<junction> *keyLine = &(mapList[i]);
                size_t lineLength;
                fileReader->read(reinterpret_cast<char*>(&lineLength), sizeof &lineLength);
                for (int j = 0; j < lineLength; j++){
                    junction junctionRead;
                    junctionRead.readFromBinary(fileReader);
                    keyLine->push_back(junctionRead);
                }
            }
        }
};
