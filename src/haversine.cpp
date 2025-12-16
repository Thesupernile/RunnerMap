#include <cmath>

double haversine(double lat1, double lon1, double lat2, double lon2){
    const int EARTH_RADIUS {6371000};      // Radius of the earth
    const double PI        {3.1415926535};      // PI (the constant)

    const double lat1Rad = lat1 * PI/180;
    const double lat2Rad = lat2 * PI/180;
    const double deltaLat = (lat2-lat1) * PI/180;
    const double deltaLon = (lon2-lon1) * PI/180;

    // Apply haversine formula
    double distance = 2 * EARTH_RADIUS * asin(sqrt(pow(sin(deltaLat/2), 2) + cos(lat1Rad) * cos(lat2Rad) * pow(sin(deltaLon/2), 2)));
    
    return distance;
}
