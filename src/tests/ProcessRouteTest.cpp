#include <gtest/gtest.h>
#include "../ProcessRoute.cpp"

TEST(RouteProcessing, ProcessDLSRoute){
    const std::string INPUTJSON = "{\"requiredPoints\":[{\"lat\":52.188708,\"lng\":0.136308},{\"lat\":52.193626,\"lng\":0.136534}]}";
    const std::string EXPECTED_OUTPUT_JSON = "";
    const double desiredRouteLength = 1000.0;

    std::string outputJSON = ProcessRoute(INPUTJSON, desiredRouteLength);
}

TEST(RouteProcessing, ProcessAStarRoute){
    const std::string INPUTJSON = "{\"requiredPoints\":[{\"lat\":52.188708,\"lng\":0.136308},{\"lat\":52.193626,\"lng\":0.136534}]}";
    const std::string EXPECTED_OUTPUT_JSON = "{\"requiredPoints\":[{\"lat\":52.188768,\"lng\":0.136261},{\"lat\":52.189337,\"lng\":0.135913},{\"lat\":52.189533,\"lng\":0.135797},{\"lat\":52.189666,\"lng\":0.135725},{\"lat\":52.190106,\"lng\":0.135463},{\"lat\":52.190356,\"lng\":0.135298},{\"lat\":52.190418,\"lng\":0.135252},{\"lat\":52.190525,\"lng\":0.135173},{\"lat\":52.190905,\"lng\":0.134914},{\"lat\":52.191294,\"lng\":0.134619},{\"lat\":52.191524,\"lng\":0.134435},{\"lat\":52.191572,\"lng\":0.134393},{\"lat\":52.191851,\"lng\":0.134156},{\"lat\":52.191897,\"lng\":0.134300},{\"lat\":52.191989,\"lng\":0.134228},{\"lat\":52.191990,\"lng\":0.134256},{\"lat\":52.191985,\"lng\":0.134286},{\"lat\":52.191973,\"lng\":0.134317},{\"lat\":52.191740,\"lng\":0.134511},{\"lat\":52.191902,\"lng\":0.135028},{\"lat\":52.191876,\"lng\":0.135051},{\"lat\":52.191893,\"lng\":0.135110},{\"lat\":52.191898,\"lng\":0.135124},{\"lat\":52.191969,\"lng\":0.135397},{\"lat\":52.192022,\"lng\":0.135388},{\"lat\":52.192067,\"lng\":0.135380},{\"lat\":52.192138,\"lng\":0.135388},{\"lat\":52.192327,\"lng\":0.135581},{\"lat\":52.192882,\"lng\":0.135991},{\"lat\":52.193216,\"lng\":0.136232},{\"lat\":52.193440,\"lng\":0.136395},{\"lat\":52.193537,\"lng\":0.136462},{\"lat\":52.193578,\"lng\":0.136495}]}";
    const double desiredRouteLength = 0.0;

    std::string outputJSON = ProcessRoute(INPUTJSON, desiredRouteLength);

    EXPECT_STREQ(outputJSON.c_str(), EXPECTED_OUTPUT_JSON.c_str());
}
