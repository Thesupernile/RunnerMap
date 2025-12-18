#include <string>
#include <node_api.h> 
// #include "network.hpp"
#include "tests/testData.hpp"

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
    size_t argc = 1;
    napi_value args[1];
    napi_value testReturn;

    // Check that the argument is valid
    status = napi_get_cb_info(env, info, &argc, args, NULL, NULL);
    if (argc != 1){
        napi_throw_type_error(env, NULL, "Incorrect number of arguments");
        return NULL;
    }

    napi_valuetype arg1Type;
    status = napi_typeof(env, args[1], &arg1Type);
    if (arg1Type != napi_string){
        napi_throw_type_error(env, NULL, "Argument has invalid type");
        return NULL;
    }
    // Convert the argument into a usable format
    

    // Calculate the route
    route testRoute = route();

    // Return the route
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
