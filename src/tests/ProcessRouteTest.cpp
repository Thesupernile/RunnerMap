#include <gtest/gtest.h>
#include "../ProcessRoute.cpp"

TEST(RouteProcessing, ProcessRoute){
    const std::string INPUTJSON = "{\"requiredPoints\":[{\"lat\":52.188708,\"lng\":0.136308},{\"lat\":52.193626,\"lng\":0.136534}]}";
    const std::string EXPECTED_OUTPUT_JSON = "";

    std::string outputJSON = ProcessRoute(INPUTJSON);

    EXPECT_STREQ(outputJSON.c_str(), EXPECTED_OUTPUT_JSON.c_str());
}
