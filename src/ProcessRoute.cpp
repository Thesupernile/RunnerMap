#include <string>
#include "tests/testData.hpp"
#include "JSONConversion.cpp"
#include "fileReader.cpp"

std::string ProcessRoute(const std::string &requiredPointsJSON, double desiredRteLen){
    std::vector<junction> reqPointsList;
    convertJSONToRoute(requiredPointsJSON, reqPointsList);

    // Create the network
    network* map = new network();     // Network object used for testing
    fileReader reader;
    reader.getStoredMap(map);

    // Convert to a list of points in the map
    for (int i = 0; i < reqPointsList.size(); i++){
        junction point = reqPointsList[i];
        std::uint64_t pointId = map->getClosestJunctionId(point.lat, point.lon);
        point = map->getJunction(pointId);
        reqPointsList[i] = point;
    }
    
    std::string JSONToReturn;
    // Calculate the route
    if (desiredRteLen == 0){
        std::unique_ptr<route> calculatedRoutePtr = std::move(map->calculateRoute(reqPointsList));   // Pointer to calculated route needing to be returned to user
        // TODO Convert this function to use a pointer as a parameter
        JSONToReturn = convertRouteToJSON(*calculatedRoutePtr);
    }
    else{
        route dlsRoute;
        map->findDLS(dlsRoute, reqPointsList, desiredRteLen);
        // TODO Convert this function to use a pointer as a parameter
        JSONToReturn = convertRouteToJSON(dlsRoute);
    }
    delete(map);

    return JSONToReturn;
}
