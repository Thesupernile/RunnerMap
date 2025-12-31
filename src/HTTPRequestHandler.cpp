#include <node_api.h> 
#include "JSONConversion.cpp"

napi_value JavaScriptBindings(napi_env env, napi_callback_info info){
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
    status = napi_typeof(env, args[0], &arg1Type);
    if (arg1Type != napi_string){
        napi_throw_type_error(env, NULL, "Argument has invalid type");
        return NULL;
    }

    // Convert the argument into a usable format
    size_t stringLength;
    status = napi_get_value_string_utf8(env, args[0], NULL, NULL, &stringLength);
    char* requiredPointsStr = (char*)malloc(stringLength + 1);
    status = napi_get_value_string_utf8(env, args[0], requiredPointsStr, stringLength + 1, NULL);


    std::shared_ptr<std::vector<junction>> reqPointsListPtr = convertJSONToRoute(requiredPointsStr);
    free(requiredPointsStr);

    // Create the network
    
    // Calculate the route
    route testRoute = route();
    testRoute.setRoute(*reqPointsListPtr);

    // Return the route
    std::string testValue = convertRouteToJSON(testRoute);

    status = napi_create_string_utf8(env, testValue.c_str(), testValue.length(), &testReturn);

    return testReturn;
};

napi_value Init(napi_env env, napi_value exports) {
    napi_status status;
    napi_property_descriptor desc = { "CalculateRoute", NULL, JavaScriptBindings, NULL, NULL, NULL, napi_configurable, NULL };
    status = napi_define_properties(env, exports, 1, &desc);
    if (status != napi_ok) return NULL;
    return exports;
};

NAPI_MODULE(MappingComponent, Init);
