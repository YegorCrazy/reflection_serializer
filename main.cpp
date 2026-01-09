#include <iostream>

#include "serializer.h"

class InnerObj
{
public:
    InnerObj() = default;
    InnerObj(int a, std::string b) : _a(a), _b(b) {}

    void printInternals()
    {
        std::cout << _a << " " << _b << std::endl;
    }

private:
    int _a;
    std::string _b;
};

struct Point
{
    Point() = default;
    Point(int x, int y, double z, std::string st, std::vector<int> v,
        std::unordered_map<std::string, long long> m, InnerObj obj)
        : _x(x), _y(y), _z(z), _st(st), _v(v), _m(m), _obj(obj) {}

    void printInternals()
    {
        std::cout << _x << " " << _y << " " << _z << " " << _st << std::endl;

        for (const auto& el : _v)
            std::cout << el << " ";
        std::cout << std::endl;

        for (const auto& [k, v] : _m)
            std::cout << k << "->" << v << " ";
        std::cout << std::endl;

        _obj.printInternals();
    }

private:
    int _x;
    int _y;
    double _z;
    std::string _st;
    std::vector<int> _v;
    std::unordered_map<std::string, long long> _m;
    InnerObj _obj;
};

int main()
{
    Point a(
        10, 20, 16.67, "success",
        {66, 67, 68}, {{"abc", 10000000000}, {"bcd", 10000000010}},
        InnerObj(42, "good_one")
    );

    auto j = serializer::serialize(a);

    std::cout << j << std::endl;

    Point b = serializer::deserialize<Point>(j);
    b.printInternals();
}
