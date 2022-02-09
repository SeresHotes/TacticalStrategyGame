#pragma once


#include <chrono>
#include <list>

class Tickable {
public:
    std::list<Tickable*>::iterator tickable_iterator;
    virtual void tick(std::chrono::milliseconds dt) = 0;
};

