#pragma once
#ifndef CATA_SRC_NPC_OPINION_H
#define CATA_SRC_NPC_OPINION_H

class JsonObject;
class JsonOut;

struct npc_opinion {
    int trust;
    int fear;
    int value;
    int anger;
    int owed; // Positive when the npc owes the player. Negative if player owes them.
    int sold; // Total value of goods sold/donated by player to the npc. Cannot be negative.

    npc_opinion();

    npc_opinion &operator+=( const npc_opinion &rhs );

    npc_opinion operator+( const npc_opinion &rhs );

    void serialize( JsonOut &json ) const;
    void deserialize( const JsonObject &data );
};

#endif // CATA_SRC_NPC_OPINION_H
