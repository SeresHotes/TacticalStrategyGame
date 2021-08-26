#include <iostream>
#include <memory>
#include <vector>
#include <list>


template<class T>
class valid_wrapper {
private:
    T obj;
public:
    template<class ...Args>
    valid_wrapper(Args ...args) : obj(args...), is_valid(true) {}
    valid_wrapper(const T& obj) : obj(obj), is_valid(true) {}
    operator T&() {
        return obj;
    }
    bool is_valid;
};
template<class T>
using sticky_ptr2 = std::shared_ptr<valid_wrapper<T>>; 
template <class T, typename... Args>
auto make_sticky2(Args&&... args) -> decltype(std::make_shared<valid_wrapper<T>>(std::forward<Args>(args)...)) {
    return std::make_shared<valid_wrapper<T>>(std::forward<Args>(args)...);
}

#include <typeinfo>


template<typename T>
class OfType {};

template<class T>
class Component {
public:
    void print(OfType<T>) {
        std::cout << typeid(T).name() << std::endl;
    }
};


template<class ...Args>
int sum(Args ...args);
template<class A, class ...Args>
int sum(A a, Args ...args) {
    return a + sum(args...);
}
template<>
int sum() {
    return 0;
}

template<class ...Args>
class ComponentArray;



template<class A, class ...Args>
class ComponentArray<A, Args...> : public ComponentArray<Args...> {
private:
    A val;
public:
    using ComponentArray<Args...>::print;
    using ComponentArray<Args...>::getComponent;
    A* getComponent(OfType<A>) {
        return &val;
    }
    void print(OfType<A>) {
        std::cout << typeid(A).name() << std::endl;
    }
};

template<>
class ComponentArray<> {
public:
    template<class T>
    T* getComponent(OfType<T>) {
        return nullptr;
    }
    template<class T>
    void print(OfType<T>) {
        std::cout << "NULL" << std::endl;
    }
};

template<class ...Args>
class ComponentArray2 : private ComponentArray<Args...> {
public:
    template<class T>
    T* getComponent() {
        return ComponentArray<Args...>::getComponent(OfType<T>());
    }
};

class A {
public:
    template<class T>
    static constexpr T big_value = 5;
};
    
int main() {
    ComponentArray2<int, double, std::string> comp;
    *comp.getComponent<int>() = 5; 
    *comp.getComponent<double>() = 7.3;
    *comp.getComponent<std::string>() = "Hello";
    std::cout << *comp.getComponent<int>() << std::endl;
    std::cout << *comp.getComponent<double>() << std::endl;
    std::cout << *comp.getComponent<std::string>() << std::endl;

    return 0;
}   