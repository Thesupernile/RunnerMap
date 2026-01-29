#ifndef NETWORK_CPP
#define NETWORK_CPP

#include <memory>
#include <fstream>
#include "astarHashMap.cpp"

class network : public nodeObject{
    private:
        junctionHashMap nodeList;
        double heuristic(junction currentJunction, junction targetJunction){
            // Uses haversine to calculate aprox distance from current node to target
            return haversine(currentJunction.lat, currentJunction.lon, targetJunction.lat, targetJunction.lon);
        }

    public:
        network(){
            
        }

        void createJunction(std::uint64_t junctionId, double nodeLatitude, double nodeLongitude){
            // First node id will be paired with first arc weight
            junction newNode = {junctionId, nodeLatitude, nodeLongitude};
            addJunction(newNode);
        }

        bool isEmpty(){
            return nodeList.isEmpty();
        }

        void addJunction(junction newNode){
            nodeList.insertValue(newNode);
        }

        void addNewNodeConnection(std::uint64_t targetjunctionId, connection newConnection){
            // Used to add a new arc to an existing node in the network
            if (nodeList.containsKey(targetjunctionId)){
                nodeList.addJunctionConnection(targetjunctionId, newConnection);
            }
            else{
                throw std::invalid_argument("Node not in array");
            }
        }
    
        bool containsJunction(std::uint64_t junctionId){
            return nodeList.containsKey(junctionId);
        }

        junction getJunction(std::uint64_t junctionId){
            return nodeList.accessValue(junctionId);
        }

        void cullIsolatedJunctions(){
            // TODO Removes junctions with no connections (aimed to improve performance)
            nodeList.cull();
        }

        void astar(junction &start, junction &end, route &optimalPath){
            // Use A* to calculate a route. Desired Route length assumed to be zero unless specified (shortest route possible)
            bool end_reached = false;
            astarHashMap* unvisitedNodes = new astarHashMap();
            astarHashMap* visitedNodes = new astarHashMap();

            unvisitedNodes->insertValue(astarjunction(start, 0, heuristic(start, end)));
            while (!end_reached){
                // Find node with lowest f-score
                double lowestFScore = INFINITY;
                std::uint64_t currentNodeId = unvisitedNodes->getLowestFScore();
                astarjunction currentNode = unvisitedNodes->accessValue(currentNodeId);
                if (currentNodeId == end.id){
                    // If program reaches the end it is done
                    end_reached = true;
                }
                else{
                    for (auto connection : currentNode.node.connectionsList){
                        bool nodeVisited = false;
                        if (visitedNodes->containsKey(currentNodeId)){
                            nodeVisited = true;
                        }
                        if (!nodeVisited){
                            // We only look at a node if we've not visited it yet. If we have we already know that we've found the optimal path to that node
                            bool nodeInUnvisited = false;
                            astarjunction nextNode;
                            if (unvisitedNodes->containsKey(connection.connectedNodeId)){
                                // If we've seen the node before we just need to check if the gscore from the new connection is higher than the previous one
                                nextNode = unvisitedNodes->accessValue(connection.connectedNodeId);
                                double gScore = currentNode.g_score + connection.connectionLength;
                                if (nextNode.g_score > gScore){
                                    nextNode.g_score = gScore;
                                    double fScore = gScore + heuristic(nextNode.node, end);
                                    nextNode.f_score = fScore;
                                    nextNode.previousNodeId = currentNode.node.id;
                                }

                            }
                            else{
                                // If we've not seen the node before, we need to create it and add it to the unvisited list
                                junction targetNode = nodeList.accessValue(connection.connectedNodeId);
                                double gScore = currentNode.g_score + connection.connectionLength;
                                double fScore = gScore + heuristic(targetNode, end);

                                astarjunction newJunction = astarjunction(targetNode, gScore, fScore);
                                newJunction.previousNodeId = currentNode.node.id;
                                unvisitedNodes->insertValue(newJunction);
                            }
                        }
                    }
                }
                visitedNodes->insertValue(currentNode);
                unvisitedNodes->removeValue(currentNodeId);
            }
            // Traverse the route backwards by starting at the end and following the previous node ids
            std::uint64_t targetId = end.id;
            while(targetId != start.id){
                optimalPath.addJunction(visitedNodes->accessValue(targetId).node);
                targetId = visitedNodes->accessValue(targetId).previousNodeId;
            }
            optimalPath.addJunction(start);
            // Path is traversed backwards during creation process so reversing the route is necessary
            optimalPath.reverseRoute();

            // Cleanup
            delete(visitedNodes);
            delete(unvisitedNodes);
        }


        void findDLSRecurse(route &route, uint64_t startNodeId, uint64_t endNodeId, double remainingRouteLength = 0){
            // Finds a route of a desired length (DLS) based on a start and end point
            junction start = nodeList.accessValue(startNodeId);
            junction end = nodeList.accessValue(endNodeId);
            double closestDistance = INFINITY;
            double estimatedRemainingDistance = heuristic(start, end);
            double bestConnectionDistance = 0;
            junction bestJunction;

            if (remainingRouteLength <= estimatedRemainingDistance){
                // AStar to the end junction
                astar(start, end, route);
                route.reverseRoute();
                route.dropLastNode();
            }
            else{
                // We need to look for nodes that are approximately the right distance away
                for(connection connection : start.connectionsList){
                    junction connectedJunction = nodeList.accessValue(connection.connectedNodeId);
                    double distanceDiff = abs(remainingRouteLength - (connection.connectionLength + heuristic(connectedJunction, nodeList.accessValue(endNodeId))));
                    if (distanceDiff < closestDistance){
                        closestDistance = distanceDiff;
                        bestConnectionDistance = connection.connectionLength;
                        bestJunction = connectedJunction;
                    }
                }
                remainingRouteLength -= bestConnectionDistance;
                if (remainingRouteLength < 0){ remainingRouteLength = 0; }
                if (bestJunction.id == endNodeId){
                    route.addJunction(bestJunction);
                    return;
                }

                uint64_t nextNodeid = bestJunction.id;
                findDLSRecurse(route, nextNodeid, endNodeId, remainingRouteLength);
                route.addJunction(bestJunction);
            }
        }

        void findDLS(route &fullRoute, std::vector<junction> requiredJunctions, double remainingRouteLength){
            // Find the expected length
            double expectMinRteLen {};
            std::vector<junction> fullRouteRoute = fullRoute.getRoute();
            route subroute {};

            for (int i = 1; i < requiredJunctions.size(); i++){
                expectMinRteLen += heuristic(requiredJunctions[i-1], requiredJunctions[i]);
            }

            for (int i = 0; i < requiredJunctions.size() - 1; i++){
                // Each leg gets allocated approximately the right amount of the desired route length depending on
                // it's proportion of the distance in an optimal route.
                double estimatedMinLegRteLen = heuristic(requiredJunctions[i], requiredJunctions[i+1]);
                double routeLengthForThisLeg = (estimatedMinLegRteLen/expectMinRteLen) * remainingRouteLength;
                subroute = route();

                findDLSRecurse(subroute, requiredJunctions[i].id, requiredJunctions[i+1].id, routeLengthForThisLeg);
                subroute.addJunction(nodeList.accessValue(requiredJunctions[i].id));
                subroute.reverseRoute();
                std::vector<junction> subrouteRoute = subroute.getRoute();

                fullRouteRoute.insert(fullRouteRoute.end(), subrouteRoute.begin(), subrouteRoute.end());
            }

            fullRoute.setRoute(fullRouteRoute);
        }

        std::uint64_t getClosestJunctionId(double lat, double lon){
            std::uint64_t junctionId {};
            junctionId = nodeList.getJunctionByPosition(lat, lon);

            return junctionId;
        }


        void calculateRoute(std::vector<junction> &requiredJunctions, route &fullRoute){
            route subroute;
            std::vector<junction> fullRouteRoute = fullRoute.getRoute();

            for (int i = 0; i < requiredJunctions.size() - 1; i++){
                junction currentJunction = requiredJunctions[i];
                junction nextJunction = requiredJunctions[i+1];
                subroute = route();

                astar(currentJunction, nextJunction, subroute);
                std::vector<junction> subrouteRoute = subroute.getRoute();

                fullRouteRoute.insert(fullRouteRoute.end(), subrouteRoute.begin(), subrouteRoute.end());
                // Consider using pointers here?
            }
            fullRoute.setRoute(fullRouteRoute);
        }

        void storeAsBinary(std::ofstream *fileWriter){
            nodeList.storeAsBinary(fileWriter);
        }

        void readFromBinary(std::ifstream *fileWriter){
            junctionHashMap newNodeList;

            newNodeList.readFromBinary(fileWriter);
            nodeList = newNodeList;
        }

};

/*
We should store a list of points that are part of the network.
These points will then store their own connections to other nodes on the network
Nodes will need an ID in order to differentiate them
They will also store thier own latitude and longtitude so can be easily plotted on the map

*/

#endif
