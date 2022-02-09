#include <functional>
#include <iostream>
#include <list>
#include <memory>
#include <variant>
#include <vector>
#include <tuple>
#include <typeinfo>


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
    constexpr std::tuple<A&, Args&...> getAllComponents() {
        return std::tuple_cat(std::tie(val), ComponentArray<Args...>::getAllComponents());
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
    constexpr std::tuple<> getAllComponents() {
        return std::tie();
    }
};

template<class ...Args>
class ComponentArray2 : private ComponentArray<Args...> {
public:
    template<class T>
    T* getComponent() {
        return ComponentArray<Args...>::getComponent(OfType<T>());
    }
    constexpr int componentCount() const {
        return sizeof...(Args);
    }
    constexpr std::tuple<Args&...> getAllComponents() {
        return ComponentArray<Args...>::getAllComponents();
    }
};



template<class ...Args>
class EntityVariant {
private:
    std::unique_ptr<void> ptr;
public:

    template<class T>
    T* getComponent() {
        return components.getComponent<T>();
    }
};

using AllComponentVariantType = std::variant<int>;

class Entity {
private:
    AllComponentVariantType components;
public:
    template<class T>
    T* getComponent() {
        return components.getComponent<T>();
    }
    constexpr int componentCount() const {
        return components.componentCount();
    }
    constexpr std::tuple<Args&...> getAllComponents() {
        return ComponentArray<Args...>::getAllComponents();
    }
};


class EntityManager {
private:
    std::vector<AllComponentVariantType> arr;
public:

};





int main() {
    ComponentArray2<int, double> comp;
    auto val = comp.getAllComponents();
    *comp.getComponent<int>() = 5; 
    std::get<0>(val) = 3;
    std::cout << *comp.getComponent<int>() << std::endl;


    return 0;
}   