#pragma once
#ifndef CATA_SRC_DAMAGE_TYPE_H
#define CATA_SRC_DAMAGE_TYPE_H

enum class damage_type : int {
    NONE = 0, // null damage, doesn't exist
    PURE, // typeless damage, should always go through
    BIOLOGICAL, // internal damage, like from smoke or poison
    COLD, // e.g. heatdrain, cryogrenades
    ELECTRIC, // e.g. electrical discharge
    ACID, // corrosive damage, e.g. acid
    // Damage types above are not intended to damage items
    BASH,  // bash damage
    CUT, // cut damage
    STAB, // stabbing/piercing damage
    HEAT, // e.g. fire, plasma
    BULLET, // bullets and other fast moving projectiles
    NUM
};

#endif // CATA_SRC_DAMAGE_TYPE_H
