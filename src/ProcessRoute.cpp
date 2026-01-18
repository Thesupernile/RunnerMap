#include <string>
#include "tests/testData.hpp"
#include "JSONConversion.cpp"
#include "fileReader.cpp"

std::string ProcessRoute(const std::string &requiredPointsJSON, double desiredRteLen){
    std::shared_ptr<std::vector<junction>> reqPointsListPtr = std::move(convertJSONToRoute(requiredPointsJSON));

    // Create the network
    network* map = new network();     // Network object used for testing
    fileReader reader;
    reader.getStoredMap(map);

    // Convert to a list of points in the map
    for (int i = 0; i < reqPointsListPtr->size(); i++){
        junction point = (*reqPointsListPtr)[i];
        int pointId = map->getClosestJunctionId(point.lat, point.lon);
        point = map->getJunction(pointId);
        (*reqPointsListPtr)[i] = point;
    }
    
    std::string JSONToReturn;
    // Calculate the route
    if (desiredRteLen == 0){
        std::unique_ptr<route> calculatedRoutePtr = std::move(map->calculateRoute(reqPointsListPtr));   // Pointer to calculated route needing to be returned to user
        // TODO Convert this function to use a pointer as a parameter
        JSONToReturn = convertRouteToJSON(*calculatedRoutePtr);
    }
    else{
        std::shared_ptr<route> dlsRoutePtr;
        map->findDLS(dlsRoutePtr, *reqPointsListPtr, desiredRteLen);
        // TODO Convert this function to use a pointer as a parameter
        JSONToReturn = convertRouteToJSON(*dlsRoutePtr);
    }
    delete(map);

    return JSONToReturn;
}
