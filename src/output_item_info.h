#pragma once
#ifndef CATA_SRC_OUTPUT_ITEM_INFO_H
#define CATA_SRC_OUTPUT_ITEM_INFO_H

#include <string>
#include <vector>

struct iteminfo;

// the extra data that item_info needs to draw
struct item_info_data {
    private:
        std::string sItemName;
        std::string sTypeName;
        std::vector<iteminfo> vItemDisplay;
        std::vector<iteminfo> vItemCompare;
        int selected = 0;

    public:
        item_info_data();
        ~item_info_data();

        item_info_data( const std::string &sItemName, const std::string &sTypeName,
                        const std::vector<iteminfo> &vItemDisplay, const std::vector<iteminfo> &vItemCompare );

        item_info_data( const std::string &sItemName, const std::string &sTypeName,
                        const std::vector<iteminfo> &vItemDisplay, const std::vector<iteminfo> &vItemCompare,
                        int &ptr_selected );

        const std::string &get_item_name() const {
            return sItemName;
        }
        const std::string &get_type_name() const {
            return sTypeName;
        }
        const std::vector<iteminfo> &get_item_display() const {
            return vItemDisplay;
        }
        const std::vector<iteminfo> &get_item_compare() const {
            return vItemCompare;
        }

        int *ptr_selected = &selected;
        bool without_getch = false;
        bool without_border = false;
        bool handle_scrolling = false;
        bool any_input = true;
        bool scrollbar_left = true;
        bool use_full_win = false;
        unsigned int padding = 1;
};

#endif // CATA_SRC_OUTPUT_ITEM_INFO_H
