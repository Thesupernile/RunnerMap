#include "main.hpp"

struct location{
    double lat {};
    double lon {};
};

route getRoute(std::vector<location> requiredPoints){
    // TODO Link up the actual network
    network map {};

    std::vector<junction> requiredJunctions {};
    for (location point : requiredPoints){
        std::uint64_t junctionId = map.getClosestJunctionId(point.lat, point.lon);
        requiredJunctions.push_back(map.getJunction(junctionId));
    }
    
    std::unique_ptr routePtr = map.calculateRoute(std::make_unique<std::vector<junction>>(requiredJunctions));

    return *routePtr;
}


int main(){
    return 0;
}
