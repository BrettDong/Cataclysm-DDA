#include "npc_opinion.h"

#include "json.h"

npc_opinion::npc_opinion()
{
    trust = 0;
    fear  = 0;
    value = 0;
    anger = 0;
    owed  = 0;
    sold = 0;
}

npc_opinion &npc_opinion::operator+=( const npc_opinion &rhs )
{
    trust += rhs.trust;
    fear  += rhs.fear;
    value += rhs.value;
    anger += rhs.anger;
    owed  += rhs.owed;
    sold  += rhs.sold;
    return *this;
}

npc_opinion npc_opinion::operator+( const npc_opinion &rhs )
{
    return npc_opinion( *this ) += rhs;
}

void npc_opinion::deserialize( const JsonObject &data )
{
    data.allow_omitted_members();
    data.read( "trust", trust );
    data.read( "fear", fear );
    data.read( "value", value );
    data.read( "anger", anger );
    data.read( "owed", owed );
    data.read( "sold", sold );
}

void npc_opinion::serialize( JsonOut &json ) const
{
    json.start_object();
    json.member( "trust", trust );
    json.member( "fear", fear );
    json.member( "value", value );
    json.member( "anger", anger );
    json.member( "owed", owed );
    json.member( "sold", sold );
    json.end_object();
}
