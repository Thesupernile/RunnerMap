#include <gtest/gtest.h>
#include "../src/JSONConversion.cpp"
#include "../src/network.cpp"
#include <string>

const struct {
    double node1Lat = 52.18788;
    double node1Lon = 0.13678;
    double node2Lat = 52.18933;
    double node2Lon = 0.1359;

    junction requiredPoint1 = junction(0, node1Lat, node1Lon);
    junction requiredPoint2 = junction(1, node2Lat, node2Lon);

    std::vector<junction> requiredPoints = {requiredPoint1, requiredPoint2};
    std::string JSONString = "{\"requiredPoints\":[{\"lat\":52.187880,\"lng\":0.136780},{\"lat\":52.189330,\"lng\":0.135900}]}";
} JSONTestData;


TEST(JSONParsing, JSONToList){
    // Checks that the function to convert JSON to a list of junctions works correctly
    std::shared_ptr requiredPointsPtr = convertJSONToRoute(JSONTestData.JSONString);
    
    EXPECT_EQ(requiredPointsPtr->size(), JSONTestData.requiredPoints.size());
    for (int i = 0; i < requiredPointsPtr->size(); i++){
        EXPECT_EQ((*requiredPointsPtr)[i].lat, JSONTestData.requiredPoints[i].lat);
        EXPECT_EQ((*requiredPointsPtr)[i].lon, JSONTestData.requiredPoints[i].lon);
    }
}


TEST(JSONParsing, ListToJSON){
    // Checks that the function to convert a list of junctions to JSON is working
    std::string requiredPointsString {};
    route testRoute;
    testRoute.setRoute(JSONTestData.requiredPoints);

    requiredPointsString = convertRouteToJSON(testRoute);
    EXPECT_STREQ(requiredPointsString.c_str(), JSONTestData.JSONString.c_str());
}
