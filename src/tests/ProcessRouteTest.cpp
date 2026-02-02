#include <gtest/gtest.h>
#include "../ProcessRoute.cpp"

TEST(RouteProcessing, ProcessAStarRoute){
    const std::string INPUTJSON = "{\"requiredPoints\":[{\"lat\":52.188708,\"lng\":0.136308},{\"lat\":52.193626,\"lng\":0.136534}]}";
    const std::string EXPECTED_OUTPUT_JSON = "{\"requiredPoints\":[{\"lat\":52.188768,\"lng\":0.136261},{\"lat\":52.189337,\"lng\":0.135913},{\"lat\":52.189533,\"lng\":0.135797},{\"lat\":52.189666,\"lng\":0.135725},{\"lat\":52.190106,\"lng\":0.135463},{\"lat\":52.190356,\"lng\":0.135298},{\"lat\":52.190418,\"lng\":0.135252},{\"lat\":52.190525,\"lng\":0.135173},{\"lat\":52.190905,\"lng\":0.134914},{\"lat\":52.190802,\"lng\":0.134827},{\"lat\":52.190668,\"lng\":0.134740},{\"lat\":52.191032,\"lng\":0.135039},{\"lat\":52.191148,\"lng\":0.135171},{\"lat\":52.191252,\"lng\":0.135268},{\"lat\":52.191338,\"lng\":0.135337},{\"lat\":52.191395,\"lng\":0.135380},{\"lat\":52.191459,\"lng\":0.135419},{\"lat\":52.191519,\"lng\":0.135447},{\"lat\":52.191590,\"lng\":0.135466},{\"lat\":52.191648,\"lng\":0.135474},{\"lat\":52.191706,\"lng\":0.135480},{\"lat\":52.191770,\"lng\":0.135475},{\"lat\":52.191834,\"lng\":0.135459},{\"lat\":52.191970,\"lng\":0.135411},{\"lat\":52.192022,\"lng\":0.135388},{\"lat\":52.192067,\"lng\":0.135380},{\"lat\":52.192138,\"lng\":0.135388},{\"lat\":52.192327,\"lng\":0.135581},{\"lat\":52.192882,\"lng\":0.135991},{\"lat\":52.193216,\"lng\":0.136232},{\"lat\":52.193440,\"lng\":0.136395},{\"lat\":52.193537,\"lng\":0.136462},{\"lat\":52.193578,\"lng\":0.136495}]}";
    const double desiredRouteLength = 0.0;

    std::string outputJSON = ProcessRoute(INPUTJSON, desiredRouteLength);

    EXPECT_STREQ(outputJSON.c_str(), EXPECTED_OUTPUT_JSON.c_str());
}

TEST(RouteProcessing, ProcessDLSRoute){
    const std::string INPUTJSON = "{\"requiredPoints\":[{\"lat\":52.188708,\"lng\":0.136308},{\"lat\":52.193626,\"lng\":0.136534}]}";
    const std::string EXPECTED_OUTPUT_JSON = "";
    const double desiredRouteLength = 1000.0;

    std::string outputJSON = ProcessRoute(INPUTJSON, desiredRouteLength);
}
