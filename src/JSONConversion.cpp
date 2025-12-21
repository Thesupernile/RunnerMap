// #include "network.hpp"
#include "tests/testData.hpp"
#include <string>

std::string convertRouteToJSON(route inputRoute){
    //std::string testString = "{\"requiredPoints\":[{\"lat\":52.18759634914795,\"lng\":0.13465762138366702},{\"lat\":52.188398491900806,\"lng\":0.13573586940765384}]}";
    std::string JSONOutput = "{\"requiredPoints\":[";
    int counter = 0;
    for (auto junction : inputRoute.getRoute()){
        JSONOutput += "{\"lat\":";
        JSONOutput += junction.lat;
        JSONOutput += ",\"lng\":";
        JSONOutput += junction.lon;
        counter++;
        JSONOutput += "}";
        if (counter != inputRoute.getRoute().size()){
            JSONOutput += ",";
        }
    }

    JSONOutput = JSONOutput + "]}";
    return JSONOutput;
}


void convertJSONToRoute(std::string JSON, std::shared_ptr<std::vector<junction>> reqPointsPtr){
    // Super simple conversion from list of required points as JSON into a list of required points
    
}
