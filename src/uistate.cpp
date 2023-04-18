#include "uistate.h"

#include "json.h"

void advanced_inv_pane_save_state::serialize( JsonOut &json, const std::string &prefix ) const
{
    json.member( prefix + "sort_idx", sort_idx );
    json.member( prefix + "filter", filter );
    json.member( prefix + "area_idx", area_idx );
    json.member( prefix + "selected_idx", selected_idx );
    json.member( prefix + "in_vehicle", in_vehicle );
    json.member( prefix + "container", container );
}

void advanced_inv_pane_save_state::deserialize( const JsonObject &jo, const std::string &prefix )
{
    jo.read( prefix + "sort_idx", sort_idx );
    jo.read( prefix + "filter", filter );
    jo.read( prefix + "area_idx", area_idx );
    jo.read( prefix + "selected_idx", selected_idx );
    jo.read( prefix + "in_vehicle", in_vehicle );
    jo.read( prefix + "container", container );
}

void advanced_inv_save_state::serialize( JsonOut &json, const std::string &prefix ) const
{
    json.member( prefix + "exit_code", exit_code );
    json.member( prefix + "re_enter_move_all", re_enter_move_all );
    json.member( prefix + "aim_all_location", aim_all_location );

    json.member( prefix + "active_left", active_left );
    json.member( prefix + "last_popup_dest", last_popup_dest );

    json.member( prefix + "saved_area", saved_area );
    json.member( prefix + "saved_area_right", saved_area_right );
    pane.serialize( json, prefix + "pane_" );
    pane_right.serialize( json, prefix + "pane_right_" );
}

void advanced_inv_save_state::deserialize( const JsonObject &jo, const std::string &prefix )
{
    jo.read( prefix + "exit_code", exit_code );
    jo.read( prefix + "re_enter_move_all", re_enter_move_all );
    jo.read( prefix + "aim_all_location", aim_all_location );

    jo.read( prefix + "active_left", active_left );
    jo.read( prefix + "last_popup_dest", last_popup_dest );

    jo.read( prefix + "saved_area", saved_area );
    jo.read( prefix + "saved_area_right", saved_area_right );
    pane.area_idx = saved_area;
    pane_right.area_idx = saved_area_right;
    pane.deserialize( jo, prefix + "pane_" );
    pane_right.deserialize( jo, prefix + "pane_right_" );
}
