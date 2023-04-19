#include "output_item_info.h"

#include "item.h"

item_info_data::item_info_data() = default;
item_info_data::~item_info_data() = default;

item_info_data::item_info_data( const std::string &sItemName, const std::string &sTypeName,
                                const std::vector<iteminfo> &vItemDisplay, const std::vector<iteminfo> &vItemCompare )
    : sItemName( sItemName ), sTypeName( sTypeName ),
      vItemDisplay( vItemDisplay ), vItemCompare( vItemCompare ) {}

item_info_data::item_info_data( const std::string &sItemName, const std::string &sTypeName,
                                const std::vector<iteminfo> &vItemDisplay, const std::vector<iteminfo> &vItemCompare,
                                int &ptr_selected )
    : sItemName( sItemName ), sTypeName( sTypeName ),
      vItemDisplay( vItemDisplay ), vItemCompare( vItemCompare ),
      ptr_selected( &ptr_selected ) {}
