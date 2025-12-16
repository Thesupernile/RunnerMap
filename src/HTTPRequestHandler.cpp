#include <string>
#include <node_api.h> 

napi_value SendHTTPRequest(napi_env env, napi_callback_info info){
    napi_status status;
    napi_value testReturn;
    status = napi_create_string_utf8(env, "Hello, world!", 13, &testReturn);
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
