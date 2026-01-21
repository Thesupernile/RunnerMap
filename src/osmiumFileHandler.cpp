#ifndef OSMIUMFILEHANDLER_CPP
#define OSMIUMFILEHANDLER_CPP

#include "network.cpp"
#include "filepathDef.cpp"

// Include relevant osmium libraries
#include <osmium/io/pbf_input.hpp>
#include <osmium/handler.hpp>
#include <osmium/util/memory.hpp>
#include <osmium/visitor.hpp>
#include <osmium/geom/coordinates.hpp>
#include <osmium/tags/taglist.hpp>
#include <osmium/tags/tags_filter.hpp>

struct CountHandler : public osmium::handler::Handler {
    network map;
    std::uint64_t nodes         = 0;
    std::uint64_t ways          = 0;
    std::uint64_t walkways      = 0;
    std::uint64_t relations     = 0;

    // This callback is called by osmium::apply for each node in the data.
    void node(const osmium::Node& extractedNode) noexcept {
        map.createJunction(extractedNode.id(), extractedNode.location().lat(), extractedNode.location().lon());
        nodes++;
    }

    // This callback is called by osmium::apply for each way in the data.
    void way(const osmium::Way& way) noexcept {
        // Create our way filter
        osmium::TagsFilter filter1{false};
        filter1.add_rule(true, "highway", "footway");
        filter1.add_rule(true, "highway", "pavement");
        filter1.add_rule(true, "foot", "yes");
        filter1.add_rule(true, "foot", "designated");
        filter1.add_rule(true, "foot", "permissive");
        filter1.add_rule(true, "foot", "use_sidepath");

        osmium::TagsFilter filter2{false};
        filter2.add_rule(true, "foot", "private");
        filter2.add_rule(true, "foot", "no");

        //if (osmium::tags::match_any_of(way.tags(), filter1) && osmium::tags::match_none_of(way.tags(), filter2)) {
            // For each node in the way, we add the node to the last node's connections list
            const osmium::WayNodeList& nodeList = way.nodes();
            for (int i = 1; i < nodeList.size(); i++){
                const junction previousNode = map.getJunction( nodeList[i - 1].ref() );
                const junction currentNode  = map.getJunction( nodeList[i].ref() );

                // We use the haversine formula to find the distance between the two nodes
                double distance = haversine(currentNode.lat, currentNode.lon, previousNode.lat, previousNode.lon);

                connection newNodeConnection1 = connection(previousNode.id, distance);
                map.addNewNodeConnection(currentNode.id, newNodeConnection1);

                connection newNodeConnection2 = connection(currentNode.id, distance);
                map.addNewNodeConnection(previousNode.id, newNodeConnection2);
            }
            
            walkways++;

        //}
        ways++;
    }

    // This callback is called by osmium::apply for each relation in the data.
    void relation(const osmium::Relation&) noexcept {
        relations++;
    }

};

class osmiumFileHandler{
    private:
    std::string mapDataPath;

    public:
    osmiumFileHandler(std::string initMapDataPath = MAPDATAPATH){
        mapDataPath = initMapDataPath;
    }

    void extractData(network* mapPtr){
        // Extracts OSM data into a map
        const osmium::io::File input_file{mapDataPath};
        osmium::io::Reader reader{input_file};

        CountHandler dataHandler;
        osmium::apply(reader, dataHandler);

        reader.close();

        dataHandler.map.cullIsolatedJunctions();
        
        // Refactor this to prevent this large copy operation
        *mapPtr = dataHandler.map;
    }

};
#endif
