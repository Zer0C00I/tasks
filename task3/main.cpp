#include <iostream>
#include <unordered_map>
#include <memory>
#include <random>

class MyClass
{
public:
    int data;

    MyClass(int data) : data(data) {}

    struct Hash
    {
        int operator()(const MyClass &obj) const
        {
            return std::hash<int>()(obj.data);
        }
    };

    bool operator==(const MyClass &other) const
    {
        return data == other.data;
    }
};

int main()
{

    std::unordered_map<MyClass, int, MyClass::Hash> Map;

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(1, 100);

    for (int i = 0; i < 5; ++i)
    {

        MyClass obj(dis(gen));

        Map[obj] = i;
    }

    for (const auto &pair : Map)
    {
        std::cout << "Key: " << pair.first.data << ", Value: " << pair.second << std::endl;
    }

    return 0;
}
