#include <iostream>

#include "serializer.h"

struct Point
{
    Point() = default;
    Point(int x, int y, double z, std::string st)
        : _x(x), _y(y), _z(z), _st(st) {}

    void printInternals()
    {
        std::cout << _x << " " << _y << " " << _z << " " << _st << std::endl;
    }

private:
    int _x;
    int _y;
    double _z;
    std::string _st;
};

int main()
{
    Point a(10, 20, 16.67, "success");

    std::stringstream ss;
    serializer::serialize(ss, a);

    auto serialized = ss.str();
    // serialized = serialized.substr(0, serialized.size() - 6) + "s";
    std::cout << serialized << std::endl;

    Point b = serializer::deserialize<Point>(serialized);
    b.printInternals();
}
