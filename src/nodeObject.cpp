#include "hashMap.cpp"
#include <array>

class nodeObject{
    protected:
        junctionHashMap nodeList;

    public:
        virtual void createJunction(std::uint64_t, double, double) = 0;
        virtual void addJunction(junction) = 0;
        virtual bool containsJunction(std::uint64_t) = 0;
        virtual junction getJunction(std::uint64_t) = 0;
};
