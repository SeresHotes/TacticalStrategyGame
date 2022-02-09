#pragma once
#include "Map.h"
#include "Tickable.h"

#include "cmath"

class UnitAI : public Tickable {
public:
    Unit& unit;
    Map& map;
    UnitAI(Unit& unit, Map& map) : unit(unit), map(map) {}
    void tick(std::chrono::milliseconds dt) {
        Unit* closest;
        auto dist = [](Unit& a, Unit& b) -> float {
            return (a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y);
        };

        for (auto &v : map.units) {
            if (closest == nullptr) {
                if (&v != &unit) {
                    closest = &v;
                }
            } else {
                if (dist(v, unit) < dist(unit, *closest)) {
                    closest = &v;
                }
            }
        }

        if (closest != nullptr) {
            if (dist(*closest, unit) > 1) {
            }
        } 
    }

};

