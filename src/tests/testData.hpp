#ifndef TESTDATA_HPP
#define TESTDATA_HPP

#include <vector>
#include "../network.hpp"

// TEST DATA (Manually entered data for a small part of Cambridge
const struct{
    int startJunctionIndex = 2;
    int endJunctionIndex = 8;
    double node1Lat = 52.18788;
    double node1Lon = 0.13678;
    double node2Lat = 52.18933;
    double node2Lon = 0.1359;
    double node3Lat = 52.18643;
    double node3Lon = 0.13772;
    double node4Lat = 52.18705;
    double node4Lon = 0.14172;
    double node5Lat = 52.18854;
    double node5Lon = 0.14128;
    double node6Lat = 52.19241;
    double node6Lon = 0.13371;
    double node7Lat = 52.19231;
    double node7Lon = 0.13554;
    double node8Lat = 52.19358;
    double node8Lon = 0.13649;
    double node9Lat = 52.19504;
    double node9Lon = 0.13128;
    
    double node0Lat = 52.19426;
    double node0Lon = 0.1369;
    std::vector<connection> node1ConnectedNodes = {
        connection(2, haversine(node1Lat, node1Lon, node2Lat, node2Lon)), 
        connection(3, haversine(node1Lat, node1Lon, node3Lat, node3Lon))
    };
    junction node1 = {1, node1Lat, node1Lon, node1ConnectedNodes};
    
    const std::vector<connection> node2ConnectedNodes = {
        connection(1, haversine(node2Lat, node2Lon, node1Lat, node1Lon)), 
        connection(5, haversine(node2Lat, node2Lon, node5Lat, node5Lon)),
        connection(6, haversine(node2Lat, node2Lon, node6Lat, node6Lon))
    };
    junction node2 = {2, node2Lat, node2Lon, node2ConnectedNodes};
    const std::vector<connection> node3ConnectedNodes = {
        connection(1, haversine(node3Lat, node3Lon, node1Lat, node1Lon)), 
        connection(4, haversine(node3Lat, node3Lon, node4Lat, node4Lon))
    };
    junction node3 = {3, node3Lat, node3Lon, node3ConnectedNodes};
    const std::vector<connection> node4ConnectedNodes = {
        connection(3, haversine(node4Lat, node4Lon, node3Lat, node3Lon)), 
        connection(5, haversine(node4Lat, node4Lon, node5Lat, node5Lon))
    };
    junction node4 = {4, node4Lat, node4Lon, node4ConnectedNodes};
    const std::vector<connection> node5ConnectedNodes = {
        connection(2, haversine(node5Lat, node5Lon, node2Lat, node2Lon)), 
        connection(4, haversine(node5Lat, node5Lon, node4Lat, node4Lon))
    };
    junction node5 = {5, node5Lat, node5Lon, node5ConnectedNodes};
    const std::vector<connection> node6ConnectedNodes = {
        connection(2, haversine(node6Lat, node6Lon, node2Lat, node2Lon)), 
        connection(7, haversine(node6Lat, node6Lon, node7Lat, node7Lon)),
        connection(9, haversine(node6Lat, node6Lon, node9Lat, node9Lon))
    };
    junction node6 = {6, node6Lat, node6Lon, node6ConnectedNodes};
    const std::vector<connection> node7ConnectedNodes = {
        connection(6, haversine(node7Lat, node7Lon, node6Lat, node6Lon)), 
        connection(8, haversine(node7Lat, node7Lon, node8Lat, node8Lon))
    };
    junction node7 = {7, node7Lat, node7Lon, node7ConnectedNodes};
    const std::vector<connection> node8ConnectedNodes = {
        connection(7, haversine(node8Lat, node8Lon, node7Lat, node7Lon)), 
        connection(0, haversine(node8Lat, node8Lon, node0Lat, node0Lon))
    };
    junction node8 = {8, node8Lat, node8Lon, node8ConnectedNodes};
    const std::vector<connection> node9ConnectedNodes = {
        connection(6, haversine(node9Lat, node9Lon, node6Lat, node6Lon)), 
        connection(0, haversine(node9Lat, node9Lon, node0Lat, node0Lon))
    };
    junction node9 = {9, node9Lat, node9Lon, node9ConnectedNodes};
    const std::vector<connection> node0ConnectedNodes = {
        connection(8, haversine(node0Lat, node0Lon, node8Lat, node8Lon)), 
        connection(9, haversine(node0Lat, node0Lon, node9Lat, node9Lon))
    };
    junction node0 = {0, node0Lat, node0Lon, node0ConnectedNodes};
    std::vector<junction> junctions = {node0, node1, node2, node3, node4, node5, node6, node7, node8, node9};
    
    std::vector<junction> expectedRouteJunctions = {node2, node6, node7, node8};
    std::vector<junction> expectedRouteJunctionsDesiredLength = {node2, node5, node4, node3, node4, node5, node2, node6, node7, node8};
    int expectedOptimalRouteLength = 654;
    double expectedDLSRouteLength = 2308;
} testData;

#endif
