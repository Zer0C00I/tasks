#include <iostream>

class Person
{
private:
    std::string name;
    int age;

public:
    Person(const std::string &name, int age) : name(name), age(age) {}

    void display() const
    {
        std::cout << "Name: " << name << ", Age: " << age << std::endl;
    }
};

template <typename T>
class mySharedPtr
{
private:
    T *ptr;
    size_t *ref_count;

public:
    mySharedPtr(T *value) : ptr(value), ref_count(new size_t(1)) {}

    mySharedPtr(const mySharedPtr<T> &other) : ptr(other.ptr), ref_count(other.ref_count)
    {
        (*ref_count)++;
    }

    mySharedPtr<T> &operator=(const mySharedPtr<T> &other)
    {

        if (this != &other)
        {

            if (--(*ref_count) == 0)
            {
                delete ptr;
                delete ref_count;
            }

            ptr = other.ptr;
            ref_count = other.ref_count;
            (*ref_count)++;
        }
        return *this;
    }

    T &operator*() const
    {
        return *ptr;
    }

    T *operator->() const
    {
        return ptr;
    }

    ~mySharedPtr()
    {
        if (--(*ref_count) == 0)
        {
            delete ptr;
            delete ref_count;
        }
    }
};

int main()
{

    mySharedPtr<Person> personPtr(new Person("John", 30));

    personPtr->display();

    mySharedPtr<Person> anotherPtr = personPtr;

    anotherPtr->display();

    return 0;
}