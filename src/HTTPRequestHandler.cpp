#include <string>
#include <node_api.h> 
#include "network.hpp"

std::string convertRouteToJSON(route inputRoute){
    std::string testString = "{\"requiredPoints\":[{\"lat\":52.18759634914795,\"lng\":0.13465762138366702},{\"lat\":52.188398491900806,\"lng\":0.13573586940765384}]}";
    std::string JSONOutput = "{\"requiredPoints\":[";
    int counter = 0;
    for (auto junction : inputRoute.getRoute()){
        JSONOutput += "{\"lat\":";
        JSONOutput += junction.lat;
        JSONOutput += ",\"lng\":";
        JSONOutput += junction.lon;
        counter++;
        JSONOutput += "}";
        if (counter != inputRoute.getRoute().size()){
            JSONOutput += ",";
        }
    }
    
    JSONOutput = JSONOutput + "]}";
    return testString;
}

napi_value SendHTTPRequest(napi_env env, napi_callback_info info){
    napi_status status;
    napi_value testReturn;

    route testRoute = route();
    std::string testValue = convertRouteToJSON(testRoute);

    status = napi_create_string_utf8(env, testValue.c_str(), testValue.length(), &testReturn);

    return testReturn;
};

napi_value Init(napi_env env, napi_value exports) {
    napi_status status;
    napi_property_descriptor desc = { "CalculateRoute", NULL, SendHTTPRequest, NULL, NULL, NULL, napi_configurable, NULL };
    status = napi_define_properties(env, exports, 1, &desc);
    if (status != napi_ok) return NULL;
    return exports;
};

NAPI_MODULE(MappingComponent, Init);
