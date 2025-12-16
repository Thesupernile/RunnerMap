#include "NetworkTests.cpp"

#include <iomanip>
#include <sstream>

double roundDouble(double x, int numberOfDp)
{
   std::stringstream ss;
   ss << std::scientific << std::setprecision( numberOfDp - 1 ) << x; 
   return stod( ss.str() );
}

TEST(Routefinding, HaversineTest){
    // Test input
    const double testStartLat = 50.0359;
    const double testStartLon = -5.4253;
    const double testEndLat = 58.3838;
    const double testEndLon = -3.0412;

    const double expectedLength = 940900;       // Expected result
    const int precision = 4;                    // Precision, in significant figures

    double calculatedLength = haversine(testStartLat, testStartLon, testEndLat, testEndLon);
    calculatedLength = roundDouble(calculatedLength, precision);

    EXPECT_EQ(calculatedLength, expectedLength);
}
