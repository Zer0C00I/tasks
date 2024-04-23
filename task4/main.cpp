#include <iostream>
#include <memory>
#include <unordered_map>

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
    SharedPtrCustomKeyHash() = default;

    SharedPtrCustomKeyHash(const SharedPtrCustomKeyHash &other) = default;

    SharedPtrCustomKeyHash(SharedPtrCustomKeyHash &&other) noexcept = default;

    SharedPtrCustomKeyHash &operator=(const SharedPtrCustomKeyHash &other) = default;

    SharedPtrCustomKeyHash &operator=(SharedPtrCustomKeyHash &&other) noexcept = default;

    int operator()(const std::shared_ptr<CustomKey> &key) const
    {
        return CustomKeyHash{}(*key);
    }
};

int main()
{

    std::unordered_map<std::shared_ptr<CustomKey>, int, SharedPtrCustomKeyHash> sharedPtrMap;

    sharedPtrMap[std::make_shared<CustomKey>("Part1_0", "Part2_0")] = 10;
    sharedPtrMap[std::make_shared<CustomKey>("Part1_1", "Part2_1")] = 20;

    for (const auto &pair : sharedPtrMap)
    {
        std::cout << "Key: {" << pair.first->part1 << ", " << pair.first->part2 << "}, Value: " << pair.second << std::endl;
    }

    return 0;
}
