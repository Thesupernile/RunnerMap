#include <gtest/gtest.h>
#include "../ProcessRoute.cpp"

TEST(RouteProcessing, ProcessRoute){
    const std::string INPUTJSON = "{\"requiredPoints\":[{\"lat\":52.188708,\"lng\":0.136308},{\"lat\":52.193626,\"lng\":0.136534}]}";
    const std::string EXPECTED_OUTPUT_JSON = "{\"requiredPoints\":[{\"lat\":52.189330,\"lng\":0.135900},{\"lat\":52.192410,\"lng\":0.133710},{\"lat\":52.192310,\"lng\":0.135540},{\"lat\":52.193580,\"lng\":0.136490}]}";
    const double desiredRouteLength = 0.0;

    std::string outputJSON = ProcessRoute(INPUTJSON, desiredRouteLength);

    EXPECT_STREQ(outputJSON.c_str(), EXPECTED_OUTPUT_JSON.c_str());
}
