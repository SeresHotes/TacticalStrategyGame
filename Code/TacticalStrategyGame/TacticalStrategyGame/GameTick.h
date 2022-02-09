#pragma once
#include "Tickable.h"

#include <chrono>
#include <list>

class Game {
    std::list<Tickable*> objects;
public:
    void add(Tickable* object) {
        objects.push_front(object);
        object->tickable_iterator = objects.begin();
    }
    void remove(Tickable* it) {
        objects.erase(it->tickable_iterator);
    }

    void tick(std::chrono::milliseconds dt) {
        for (auto it : objects) {
            it->tick(dt);
        }
    }

    void gameLoop() {

        std::chrono::steady_clock::time_point prev = std::chrono::steady_clock::now();
        while (true) {
            std::chrono::steady_clock::time_point now = std::chrono::steady_clock::now();
            auto dt = std::chrono::duration_cast<std::chrono::milliseconds>(now - prev);
            for (auto it : objects) {
                it->tick(dt);
            }


            prev = now;
        }
    }
};