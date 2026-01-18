#include <gtest/gtest.h>
#include "../ProcessRoute.cpp"

TEST(RouteProcessing, ProcessRoute){
    const std::string INPUTJSON = "{\"requiredPoints\":[{\"lat\":52.188708,\"lng\":0.136308},{\"lat\":52.193626,\"lng\":0.136534}]}";
    const std::string EXPECTED_OUTPUT_JSON = "";
    const double desiredRouteLength = 0.0;


    std::cout << sizeof(network) << "\n";
    std::cout << sizeof(junctionHashMap) << "\n";
    std::cout << sizeof(route) << "\n";
    std::cout << sizeof(fileReader) << "\n";
    std::cout << sizeof(junction) << "\n";

    std::string outputJSON = ProcessRoute(INPUTJSON, desiredRouteLength);

    // EXPECT_STREQ(outputJSON.c_str(), EXPECTED_OUTPUT_JSON.c_str());
}
