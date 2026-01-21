#include "haversine.cpp"
#include "hashMap.cpp"
#include "junction.cpp"
#include <vector>
#include <fstream>
#include <tuple>
#include <stdexcept>
#include <cstdint>
#include <iostream>

class junctionHashMap : public hashMap{
    // An open hashed container used to store nodes
    private:
        std::vector<junction> mapList[CAPACITY];

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
                numJunctions++;
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
                    numJunctions++;
                }
            }

        }

        bool containsKey(std::uint64_t targetKey){
            // Used to check if the hashmap contains the given key
            if (numJunctions == 0){ return false; }

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
                        numJunctions--;
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

        void addJunctionConnection(std::uint64_t key, connection newConnection){
            // Accesses a junction and adds a new connection to it
            std::uint64_t index = hash(key);
            std::vector<junction> *keyLine = &(mapList[index]);
            for (int i = 0; i < keyLine->size(); i++){
                if ((*keyLine)[i].id == key){
                    (*keyLine)[i].connectionsList.push_back(newConnection);
                }
            }
        }


        std::uint64_t getJunctionByPosition(double reqLat, double reqLon){
            // Uses a linear search to find the closest junction to the input coords
            double shortestDistance = INFINITY;
            std::uint64_t bestJunctionId {};
            if (isEmpty()){ throw std::invalid_argument("Map is empty"); }

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
                    numJunctions++;
                }
            }
        }
};
