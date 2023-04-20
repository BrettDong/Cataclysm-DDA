#pragma once
#ifndef CATA_SRC_SPECIAL_ATTACK_H
#define CATA_SRC_SPECIAL_ATTACK_H

#include "damage.h"

struct special_attack {
    std::string text;
    damage_instance damage;
};

#endif // CATA_SRC_SPECIAL_ATTACK_H
