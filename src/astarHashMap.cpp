#include <list>
#include "route.cpp"

struct astarjunction{
    junction node;
    uint64_t previousNodeId = 0;
    double g_score = INFINITY;
    double f_score = INFINITY;

    astarjunction(junction initNode, double initg_score, double initf_score){
        node = initNode;
        g_score = initg_score;
        f_score = initf_score;
    }

    astarjunction(){

    }
};


class astarHashMap : public hashMap{
    private:
    std::vector<astarjunction> mapList[CAPACITY];

    public:

    bool containsKey(std::uint64_t targetKey){
        // Used to check if the hashmap contains the given key
        if (numJunctions == 0){ return false; }

        std::uint64_t index = hash(targetKey);
        std::vector<astarjunction> *keyLine = &(mapList[index]);
        for (auto &key : *keyLine){
            if (key.node.id == targetKey){
                return true;
            }
        }
        return false;
    }
    
    void insertValue(astarjunction junctionToAdd){
        // Inserts a junction into the hash map
        const std::uint64_t key = junctionToAdd.node.id;
        const std::uint64_t index = hash(key);

        std::vector<astarjunction> *keyLine = &(mapList[index]);
        if (keyLine->size() < 1){
            keyLine->push_back(junctionToAdd);
            numJunctions++;
        }
        else{
            bool keyFound = false;
            for(int i = 0; i < keyLine->size(); i++){
                if ((*keyLine)[i].node.id == key){
                    keyFound = true;
                    break;
                }
            }
            if (!keyFound){
                keyLine->push_back(junctionToAdd);
                numJunctions++;
            }
        }

    }

    astarjunction accessValue(std::uint64_t key){
        // Accesses a value from the hashmap given a junctionID
        std::uint64_t index = hash(key);
        std::vector<astarjunction> *keyLine = &(mapList[index]);
        for (int i = 0; i < keyLine->size(); i++){
            if ((*keyLine)[i].node.id == key){
                return (*keyLine)[i];
            }
        }

        throw std::invalid_argument("Key not in array");
    }

    std::uint64_t getLowestFScore(){
        std::uint64_t lowestFScoreId;
        std::uint64_t lowestFScore = INFINITY;

        for (int i = 0; i < CAPACITY; i++){
            std::vector<astarjunction> *keyLine = &(mapList[i]);
            for (int j = 0; j < keyLine->size(); j++){
                if ((*keyLine)[j].f_score < lowestFScore){
                    lowestFScore = (*keyLine)[j].f_score;
                    lowestFScoreId = (*keyLine)[j].node.id;
                }
            }
        }

        return lowestFScoreId;
    }

};
