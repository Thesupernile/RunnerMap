#include <vector>
#include "junction.cpp"

class hashMap{
    // An open hashed container used to store nodes
    protected:
        static const int CAPACITY = 9311;           // 9311 chosen since it is a large prime
        int numJunctions {};


        std::uint64_t hash(std::uint64_t key){
            return key % CAPACITY;
        }
    public:
        hashMap(){

        }

        bool isEmpty(){
            if (numJunctions == 0){
                return true;
            }
            return false;
        }

        virtual bool containsKey(std::uint64_t targetKey) = 0;
};
