#include "nodeObject.cpp"

class route : public nodeObject{
    private:
        std::vector<junction> junctionList {};
    public:
        route(){

        }

        void createJunction(std::uint64_t junctionID, double junctionLat, double junctionLon){
            junction newJunction = junction(junctionID, junctionLat, junctionLon);
            addJunction(newJunction);
        }

        void addJunction(junction junctionToAdd){
            junctionList.push_back(junctionToAdd);
        }

        bool containsJunction(std::uint64_t junctionID){
            for (junction junction : junctionList){
                if (junction.id == junctionID){
                    return true;
                }
            }
            return false;
        }

        junction getJunction(std::uint64_t junctionID){
            for (junction junction : junctionList){
                if (junction.id == junctionID){
                    return junction;
                }
            }
            throw std::runtime_error("Junction not in route");
        }

        double calculateLength(){
            double length = 0;
            for (int i = 1; i < junctionList.size(); i++){
                junction previousJunction = junctionList[i-1];
                junction currentJunction = junctionList[i];

                length += haversine(previousJunction.lat, previousJunction.lon, currentJunction.lat, currentJunction.lon);
            }
            return length;
        }

        std::vector<junction> getRoute(){
            return junctionList;
        }

        void setRoute(std::vector<junction> newRoute){
            junctionList = newRoute;
        }

        void reverseRoute(){
            size_t end = junctionList.size() - 1;
            for(int i = 0; i < junctionList.size()/2; i++){
                junction temp = junctionList[end - i];
                junctionList[end - i] = junctionList[i];
                junctionList[i] = temp;
            }
        }
};
