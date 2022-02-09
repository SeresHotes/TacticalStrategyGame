#pragma once
#include <list>
#include "MapObject.h"
#include "Unit.h"

class Map {
public:
    std::list<MapObject> mapObjects;
    std::list<Unit> units;
};

#include <cstdlib>

void unitAttackUnit(Map& map, 
                    typename std::list<Unit>::iterator& from, 
                    typename std::list<Unit>::iterator& to) {
    if (std::rand() < from->crit_rate * RAND_MAX) {
        to->health -= 2 * from->health;
    } else {
        to->health -= from->health;
    }
    if (to->health <= 0) {
        map.units.erase(to);
    }
}
void unitMove(Map& map, typename std::list<Unit>::iterator& unit, float dx, float dy) {
    unit->x += dx;
    unit->y += dy;
}