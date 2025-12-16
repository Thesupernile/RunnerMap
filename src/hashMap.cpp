#include "haversine.cpp"
#include <vector>
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
};

struct junction{
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
};  

class junctionHashMap{
    // An open hashed container used to store nodes
    private:
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
};
