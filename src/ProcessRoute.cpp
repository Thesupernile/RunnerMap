#include <string>
#include "tests/testData.hpp"
#include "JSONConversion.cpp"

std::string ProcessRoute(std::string requiredPointsJSON){
    std::shared_ptr<std::vector<junction>> reqPointsListPtr = std::move(convertJSONToRoute(requiredPointsJSON));

    // Create the network
    // TEMP CODE
    network map {};     // Network object used for testing
    for (auto junction : testData.junctions){
        map.addJunction(junction);
    }
    // END TEMP CODE

    // Convert to a list of points in the map
    for (int i = 0; i < reqPointsListPtr->size(); i++){
        junction point = (*reqPointsListPtr)[i];
        int pointId = map.getClosestJunctionId(point.lat, point.lon);
        point = map.getJunction(pointId);
        (*reqPointsListPtr)[i] = point;
    }
    
    // Calculate the route
    std::unique_ptr<route> calculatedRoutePtr = std::move(map.calculateRoute(reqPointsListPtr));   // Pointer to calculated route needing to be returned to user

    // Return the route
    // TODO Convert this function to use a pointer as a parameter
    std::string JSONToReturn = convertRouteToJSON(*calculatedRoutePtr);
    
    return JSONToReturn;
}
