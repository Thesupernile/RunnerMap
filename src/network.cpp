#ifndef NETWORK_CPP
#define NETWORK_CPP

#include "route.cpp"
#include <memory>
#include <fstream>

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

        void addJunction(junction newNode){
            nodeList.insertValue(newNode);
        }

        void addNewNodeConnection(std::uint64_t targetjunctionId, connection newConnection){
            // Used to add a new arc to an existing node in the network
            if (nodeList.containsKey(targetjunctionId)){
                nodeList.accessValue(targetjunctionId).connectionsList.push_back(newConnection);
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

        std::unique_ptr<route> astar(std::shared_ptr<junction> start, std::shared_ptr<junction> end, double desiredRouteLength = 0){
            // Use A* to calculate a route. Desired Route length assumed to be zero unless specified (shortest route possible)
            struct astarjunction{
                junction node;
                uint64_t previousNodeId = 0;
                double g_score = INFINITY;
                double f_score = INFINITY;

                astarjunction(junction initNode, double initg_score, double initf_score){
                    node = initNode;
                    g_score = initg_score;
                    f_score = initf_score;
                }

                astarjunction(){

                }
            };
            bool end_reached = false;
            std::vector<astarjunction> unvisitedNodes;
            std::vector<astarjunction> visitedNodes;

            // Might be worth using a heap to speed this up in the future
            unvisitedNodes.push_back(astarjunction(*start, 0, heuristic(*start, *end)));
            while (!end_reached){
                // Find node with lowest f-score
                double lowestFScore = INFINITY;
                int currentNodeIndex = -1;
                astarjunction currentNode;
                for (int i = 0; i < unvisitedNodes.size(); i++){
                    astarjunction node = unvisitedNodes[i];
                    if (node.f_score < lowestFScore){
                        lowestFScore = node.f_score;
                        currentNode = node;
                        currentNodeIndex = i;
                    }
                }
                if (currentNode.node.id == end->id){
                    // Copy values for final node into the visited list
                    end_reached = true;
                }
                else{
                    for (auto connection : currentNode.node.connectionsList){
                        bool nodeVisited = false;
                        for (auto node : visitedNodes){
                            // Check id of node in visited nodes against node in the connection list
                            if (node.node.id == connection.connectedNodeId){
                                nodeVisited = true;
                            }
                        }
                        if (!nodeVisited){
                            bool nodeInUnvisited = false;
                            astarjunction nextNode;
                            for (auto node : unvisitedNodes){
                                if (node.node.id == connection.connectedNodeId){
                                    nodeInUnvisited = true;
                                    nextNode = node;
                                }
                            }
                            if (nodeInUnvisited){
                                double gScore = currentNode.g_score + connection.connectionLength;
                                if (nextNode.g_score > gScore){
                                    nextNode.g_score = gScore;
                                    double fScore = gScore + heuristic(nextNode.node, *end);
                                    nextNode.f_score = fScore;
                                    nextNode.previousNodeId = currentNode.node.id;
                                }

                            }
                            else{
                                junction targetNode = nodeList.accessValue(connection.connectedNodeId);
                                double gScore = currentNode.g_score + connection.connectionLength;
                                double fScore = gScore + heuristic(targetNode, *end);

                                astarjunction newJunction = astarjunction(targetNode, gScore, fScore);
                                newJunction.previousNodeId = currentNode.node.id;
                                unvisitedNodes.push_back(newJunction);
                            }
                        }
                    }
                }
                visitedNodes.push_back(currentNode);
                unvisitedNodes.erase(unvisitedNodes.begin() + currentNodeIndex);
            }
            route optimalPath = route();
            std::uint64_t targetId = end->id;
            while(targetId != start->id){
                for(int i = 0; i < visitedNodes.size(); i++){
                    astarjunction *currentJunction = &(visitedNodes[i]);
                    if ((*currentJunction).node.id == targetId){
                        targetId = (*currentJunction).previousNodeId;
                        optimalPath.addJunction((*currentJunction).node);
                    }
                }
            }
            optimalPath.addJunction(*start);
            optimalPath.reverseRoute();

            // Return the path calculated
            // TODO, return the actual route
            // Worth rewriting a lot of this to use ptrs so we avoid unnecessary memory allocation and deallocation
            return std::make_unique<route>(optimalPath);
        }


        void findDLSRecurse(std::shared_ptr<route> route, uint64_t startNodeId, uint64_t endNodeId, double remainingRouteLength = 0){
            // Finds a route of a desired length (DLS) based on a start and end point
            junction start = nodeList.accessValue(startNodeId);
            double closestDistance = INFINITY;
            double bestConnectionDistance = 0;
            junction bestJunction;

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
                route->addJunction(bestJunction);
                return;
            }

            uint64_t nextNodeid = bestJunction.id;
            findDLSRecurse(route, nextNodeid, endNodeId, remainingRouteLength);
            route->addJunction(bestJunction);
        }

        void findDLS(std::shared_ptr<route> route, std::vector<junction> requiredJunctions, double remainingRouteLength){
            // Find the expected length
            double expectMinRteLen {};
            for (int i = 1; i < requiredJunctions.size(); i++){
                expectMinRteLen += heuristic(requiredJunctions[i-1], requiredJunctions[i]);
            }
            

            findDLSRecurse(route, requiredJunctions[0].id, requiredJunctions[1].id, remainingRouteLength);
            route->addJunction(nodeList.accessValue(requiredJunctions[0].id));
            route->reverseRoute();
        }

        std::uint64_t getClosestJunctionId(double lat, double lon){
            std::uint64_t junctionId {};
            junctionId = nodeList.getJunctionByPosition(lat, lon);

            return junctionId;
        }


        std::unique_ptr<route> calculateRoute(std::shared_ptr<std::vector<junction>> requiredJunctions){
            route fullRoute {};
            std::unique_ptr<route> subroute;
            std::vector<junction> fullRouteRoute = fullRoute.getRoute();

            for (int i = 0; i < requiredJunctions->size() - 1; i++){
                std::shared_ptr<junction> currentJunctionPtr = std::make_shared<junction>((*requiredJunctions)[i]);
                std::shared_ptr<junction> nextJunctionPtr = std::make_shared<junction>((*requiredJunctions)[i+1]);

                subroute = std::move(astar(currentJunctionPtr, nextJunctionPtr));
                std::vector<junction> subrouteRoute = subroute->getRoute();

                fullRouteRoute.insert(fullRouteRoute.end(), subrouteRoute.begin(), subrouteRoute.end());
                // Consider using pointers here?
            }
            fullRoute.setRoute(fullRouteRoute);
            return std::make_unique<route>(fullRoute);
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
