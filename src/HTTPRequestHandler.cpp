#include <node_api.h>
#include "ProcessRoute.cpp"

napi_value JavaScriptBindings(napi_env env, napi_callback_info info){
    napi_status status;
    size_t argc = 2;
    napi_value args[2];
    napi_value returnValue;

    // Check that the argument is valid
    status = napi_get_cb_info(env, info, &argc, args, NULL, NULL);
    if (argc != 2){
        napi_throw_type_error(env, NULL, "Incorrect number of arguments");
        return NULL;
    }

    napi_valuetype arg1Type;
    status = napi_typeof(env, args[0], &arg1Type);
    if (arg1Type != napi_string){
        napi_throw_type_error(env, NULL, "Argument 1 has invalid type");
        return NULL;
    }

    // Convert the argument into a usable format
    size_t stringLength;
    status = napi_get_value_string_utf8(env, args[0], NULL, NULL, &stringLength);
    char* requiredPointsStr = (char*)malloc(stringLength + 1);
    status = napi_get_value_string_utf8(env, args[0], requiredPointsStr, stringLength + 1, NULL);

    // Convert desiredLengthRoute argument
    napi_valuetype arg2Type;
    status = napi_typeof(env, args[1], &arg2Type);
    if (arg2Type != napi_number){
        napi_throw_type_error(env, NULL, "Argument 2 has invalid type");
        return NULL;
    }

    double desiredRteLen = 0;
    status = napi_get_value_double(env, args[1], &desiredRteLen);

    // Calculate the route
    std::string JSONToReturn = ProcessRoute(requiredPointsStr, desiredRteLen);
    free(requiredPointsStr);

    status = napi_create_string_utf8(env, JSONToReturn.c_str(), JSONToReturn.length(), &returnValue);

    return returnValue;
};

napi_value Init(napi_env env, napi_value exports) {
    napi_status status;
    napi_property_descriptor desc = { "CalculateRoute", NULL, JavaScriptBindings, NULL, NULL, NULL, napi_configurable, NULL };
    status = napi_define_properties(env, exports, 1, &desc);
    if (status != napi_ok) return NULL;
    return exports;
};

NAPI_MODULE(MappingComponent, Init);
