#include "nodeObject.cpp"

class route : public nodeObject{
    private:
        std::vector<junction> junctionList {};
    public:
        route(){

        }

        void createJunction(std::uint64_t junctionID, double junctionLat, double junctionLon){
            // Add a new junction by first creating it
            junction newJunction = junction(junctionID, junctionLat, junctionLon);
            addJunction(newJunction);
        }

        void addJunction(junction junctionToAdd){
            // Add an already constructed junction
            junctionList.push_back(junctionToAdd);
        }

        bool containsJunction(std::uint64_t junctionID){
            // Find whether the route contains a junction with the given ID
            for (junction junction : junctionList){
                if (junction.id == junctionID){
                    return true;
                }
            }
            return false;
        }

        junction getJunction(std::uint64_t junctionID){
            // Get a specific junction from a route
            for (junction junction : junctionList){
                if (junction.id == junctionID){
                    return junction;
                }
            }
            throw std::runtime_error("Junction not in route");
        }

        double calculateLength(){
            // Calculate the length of the whole route
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

        void dropLastNode(){
            junctionList.erase(--junctionList.end());
        }
};
