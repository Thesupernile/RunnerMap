// #include "network.hpp"
#include "tests/testData.hpp"
#include <string>

std::string convertRouteToJSON(route inputRoute){
    //std::string testString = "{\"requiredPoints\":[{\"lat\":52.18759634914795,\"lng\":0.13465762138366702},{\"lat\":52.188398491900806,\"lng\":0.13573586940765384}]}";
    std::string JSONOutput = "{\"requiredPoints\":[";
    int counter = 0;
    for (auto junction : inputRoute.getRoute()){
        JSONOutput += "{\"lat\":";
        JSONOutput += std::to_string(junction.lat);
        JSONOutput += ",\"lng\":";
        JSONOutput += std::to_string(junction.lon);
        counter++;
        JSONOutput += "}";
        if (counter != inputRoute.getRoute().size()){
            JSONOutput += ",";
        }
    }

    JSONOutput = JSONOutput + "]}";
    return JSONOutput;
}


std::shared_ptr<std::vector<junction>> convertJSONToRoute(std::string JSON){
    // Super simple conversion from list of required points as JSON into a list of required points
    std::vector<junction> requiredPoints {};
    std::shared_ptr reqPointsPtr = std::make_shared<std::vector<junction>>(requiredPoints);
    
    int extractedNumCount = 0;
    std::string numStr;
    for (int i = 0; i < JSON.length(); i++){
        // Check the second param is actually a number
        if (JSON[i] == ':' && JSON[i+1] >= 48 && JSON[i+1] <= 57){
            // Extract the number between the colon and the comma (or } ) (which is by standard the value of the paramater)
            for (int j = i + 1; j < JSON.length(); j++){
                if (JSON[j] == ',' || JSON[j] == '}'){
                    // Extract just the number from the substring
                    numStr = JSON.substr(i+1, j-i-1);

                    if (extractedNumCount % 2 == 0){
                        junction newJunction = junction();
                        newJunction.lat = stod(numStr);
                        reqPointsPtr->push_back(newJunction);
                    }
                    else{
                        int currentJunctionIndex = floor(extractedNumCount / 2);
                        (*reqPointsPtr)[currentJunctionIndex].lon = stod(numStr);
                    }
                    extractedNumCount++;
                    break;
                }
            }
        }
    }
    
    return reqPointsPtr;
}
