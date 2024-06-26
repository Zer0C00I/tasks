#include <iostream>
#include <unordered_map>
#include <string>
#include <memory>
#include <random>

class CustomKey
{
public:
    std::string part1;
    std::string part2;

    CustomKey(const std::string &p1, const std::string &p2) : part1(p1), part2(p2) {}

    bool operator==(const CustomKey &other) const
    {
        return part1 == other.part1 && part2 == other.part2;
    }
};
class CustomKeyHash
{
public:
    int operator()(const CustomKey &key) const
    {
        return std::hash<std::string>{}(key.part1) ^ (std::hash<std::string>{}(key.part2) << 1);
    }
};

class SharedPtrCustomKeyHash
{
public:
    int operator()(const std::shared_ptr<CustomKey> &key) const
    {

        return CustomKeyHash{}(*key);
    }
};

int main()
{

    std::unordered_map<CustomKey, int, CustomKeyHash> customMap;

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(1, 100);

    for (int i = 0; i < 5; ++i)
    {
        std::string part1 = "Part1_" + std::to_string(i);
        std::string part2 = "Part2_" + std::to_string(i);
        customMap[CustomKey(part1, part2)] = dis(gen);
    }

    for (const auto &pair : customMap)
    {
        std::cout << "Key: {" << pair.first.part1 << ", " << pair.first.part2 << "}, Value: " << pair.second << std::endl;
    }

    std::unordered_map<std::shared_ptr<CustomKey>, int, SharedPtrCustomKeyHash> sharedPtrMap;

    for (const auto &pair : sharedPtrMap)
    {
        std::cout << "Key: {" << pair.first->part1 << ", " << pair.first->part2 << "}, Value: " << pair.second << std::endl;
    }

    return 0;
}
