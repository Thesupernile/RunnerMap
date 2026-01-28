#include <gtest/gtest.h>
#include "testData.hpp"

TEST(Routefinding, AStarSearch){
    network testNetwork {};     // Network object used for testing
    for (auto junction : testData.junctions){
        testNetwork.addJunction(junction);
    }

    std::vector<junction> testRouteRequiredJunctions = {{
        testData.junctions[testData.startJunctionIndex], 
        testData.junctions[testData.endJunctionIndex]
    }};

    std::unique_ptr<route> calculatedRoute = testNetwork.calculateRoute(testRouteRequiredJunctions);

    std::vector<junction> returnedRoute = calculatedRoute->getRoute();
    
    EXPECT_EQ(returnedRoute.size(), testData.expectedRouteJunctions.size());
    for (int i = 0; i < returnedRoute.size(); i++){
        EXPECT_EQ(returnedRoute[i].id, testData.expectedRouteJunctions[i].id);
    }
}

TEST(Routefinding, RouteLengthCalculation){
    route testRoute;
    testRoute.setRoute(testData.expectedRouteJunctions);
    EXPECT_EQ(std::round(testRoute.calculateLength()), testData.expectedOptimalRouteLength);
}

TEST(Routefinding, DesiredLengthRouteSearch){
    network testNetwork {};     // Network object used for testing
    for (auto junction : testData.junctions){
        testNetwork.addJunction(junction);
    }

    const double desiredRouteLength = 1829;

    route calculatedRoute {};
    uint64_t startJuncID = testData.junctions[testData.startJunctionIndex].id;
    uint64_t endJuncID = testData.junctions[testData.endJunctionIndex].id;

    std::vector<junction> testRouteRequiredJunctions = {{
        testData.junctions[testData.startJunctionIndex], 
        testData.junctions[testData.endJunctionIndex]
    }};

    testNetwork.findDLS(calculatedRoute, testRouteRequiredJunctions, desiredRouteLength);
    std::vector<junction> returnedRoute = calculatedRoute.getRoute();

    EXPECT_EQ(round(calculatedRoute.calculateLength()), testData.expectedDLSRouteLength);
    EXPECT_EQ(returnedRoute.size(), testData.expectedRouteJunctionsDesiredLength.size());
    for (int i = 0; i < returnedRoute.size(); i++){
        EXPECT_EQ(returnedRoute[i].id, testData.expectedRouteJunctionsDesiredLength[i].id);
    }
}

TEST(GeoMapping, FindNodeByLocation){
    // TESTLAT and TESTLON describe a location near to node 2 (but not the same location as node 2)
    const double TESTLAT = 52.18932;
    const double TESTLON = 0.1358;
    const std::uint64_t EXPECTEDJUNCTIONID = 2;

    network testNetwork {};     // Network object used for testing
    for (auto junction : testData.junctions){
        testNetwork.addJunction(junction);
    }

    std::uint64_t returnedJunctionId = testNetwork.getClosestJunctionId(TESTLAT, TESTLON);
    EXPECT_EQ(returnedJunctionId, EXPECTEDJUNCTIONID);
}
