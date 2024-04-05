#include <iostream>
#include <memory>

class MyClass
{
public:
    MyClass()
    {
        std::cout << "MyClass constructed" << std::endl;
    }

    ~MyClass()
    {
        std::cout << "MyClass destructed" << std::endl;
    }

    void doSomething()
    {
        std::cout << "Doing something" << std::endl;
    }
};

int main()
{

    std::shared_ptr<MyClass> sharedPtr(new MyClass);

    std::shared_ptr<MyClass> sharedPtr2 = sharedPtr;

    std::weak_ptr<MyClass> weakPtr = sharedPtr;

    std::shared_ptr<MyClass> lockedSharedPtr = weakPtr.lock();
    if (lockedSharedPtr)
    {
        std::cout << "Count of sharedPtr: " << lockedSharedPtr.use_count() << std::endl;
        lockedSharedPtr->doSomething();
    }
    else
    {
        std::cout << "Weak pointer expired" << std::endl;
    }

    sharedPtr.reset();

    lockedSharedPtr = weakPtr.lock();
    if (lockedSharedPtr)
    {
        std::cout << "Count of sharedPtr: " << lockedSharedPtr.use_count() << std::endl;
        lockedSharedPtr->doSomething();
    }
    else
    {
        std::cout << "Weak pointer expired" << std::endl;
    }

    return 0;
}
