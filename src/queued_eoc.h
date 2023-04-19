#pragma once
#ifndef CATA_SRC_QUEUED_EOC_H
#define CATA_SRC_QUEUED_EOC_H

#include "calendar.h"
#include "type_id.h"

struct queued_eoc {
    public:
        effect_on_condition_id eoc;
        time_point time;
};

struct eoc_compare {
    bool operator()( const queued_eoc &lhs, const queued_eoc &rhs ) const {
        return lhs.time > rhs.time;
    }
};

#endif // CATA_SRC_QUEUED_EOC_H
