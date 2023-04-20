#include "talker.h"

#include "character_id.h"
#include "effect.h"
#include "item.h"
#include "units.h"

talker::~talker() = default;

Character *talker::get_character()
{
    return nullptr;
}

const Character *talker::get_character() const
{
    return nullptr;
}

npc *talker::get_npc()
{
    return nullptr;
}

npc *talker::get_npc() const
{
    return nullptr;
}

item_location *talker::get_item()
{
    return nullptr;
}

item_location *talker::get_item() const
{
    return nullptr;
}

monster *talker::get_monster()
{
    return nullptr;
}

const monster *talker::get_monster() const
{
    return nullptr;
}

Creature *talker::get_creature()
{
    return nullptr;
}

const Creature *talker::get_creature() const
{
    return nullptr;
}

computer *talker::get_computer()
{
    return nullptr;
}

const computer *talker::get_computer() const
{
    return nullptr;
}

std::string talker::disp_name() const
{
    return "";
}

character_id talker::getID() const
{
    return character_id( 0 );
}

bool talker::is_male() const
{
    return false;
}

std::vector<std::string> talker::get_grammatical_genders() const
{
    return {};
}

int talker::posx() const
{
    return 0;
}

int talker::posy() const
{
    return 0;
}

int talker::posz() const
{
    return 0;
}

tripoint talker::pos() const
{
    return {};
}

tripoint_abs_ms talker::global_pos() const
{
    return {};
}

tripoint_abs_omt talker::global_omt_location() const
{
    return {};
}

std::string talker::distance_to_goal() const
{
    return "";
}

bool talker::will_talk_to_u( const Character &, bool )
{
    return false;
}

std::vector<std::string> talker::get_topics( bool )
{
    return {};
}

bool talker::check_hostile_response( int ) const
{
    return false;
}

int talker::parse_mod( const std::string &, int ) const
{
    return 0;
}

int talker::trial_chance_mod( const std::string & ) const
{
    return 0;
}

int talker::get_cur_hp( const bodypart_id & ) const
{
    return 0;
}

int talker::get_cur_part_temp( const bodypart_id & ) const
{
    return 0;
}

int talker::str_cur() const
{
    return 0;
}

int talker::dex_cur() const
{
    return 0;
}

int talker::int_cur() const
{
    return 0;
}

int talker::per_cur() const
{
    return 0;
}

int talker::get_str_max() const
{
    return 0;
}

int talker::get_dex_max() const
{
    return 0;
}

int talker::get_int_max() const
{
    return 0;
}

int talker::get_per_max() const
{
    return 0;
}

int talker::get_str_bonus() const
{
    return 0;
}

int talker::get_dex_bonus() const
{
    return 0;
}

int talker::get_int_bonus() const
{
    return 0;
}

int talker::get_per_bonus() const
{
    return 0;
}

int talker::get_skill_level( const skill_id & ) const
{
    return 0;
}

int talker::get_spell_level( const trait_id & ) const
{
    return 0;
}

int talker::get_spell_level( const spell_id & ) const
{
    return 0;
}

int talker::get_spell_exp( const spell_id & ) const
{
    return 0;
}

int talker::get_highest_spell_level() const
{
    return 0;
}

bool talker::has_trait( const trait_id & ) const
{
    return false;
}

bool talker::has_recipe( const recipe_id & ) const
{
    return false;
}

bool talker::has_flag( const json_character_flag & ) const
{
    return false;
}

bool talker::crossed_threshold() const
{
    return false;
}

int talker::num_bionics() const
{
    return 0;
}

bool talker::has_max_power() const
{
    return false;
}

bool talker::has_bionic( const bionic_id & ) const
{
    return false;
}

bool talker::knows_spell( const spell_id & ) const
{
    return false;
}

bool talker::knows_proficiency( const proficiency_id & ) const
{
    return false;
}

time_duration talker::proficiency_practiced_time( const proficiency_id & ) const
{
    return 0_seconds;
}

std::vector<skill_id> talker::skills_offered_to( const talker & ) const
{
    return {};
}

std::vector<skill_id> talker::skills_teacheable() const
{
    return {};
}

std::string talker::skill_seminar_text( const skill_id & ) const
{
    return {};
}

std::string talker::skill_training_text( const talker &, const skill_id & ) const
{
    return {};
}

std::vector<proficiency_id> talker::proficiencies_offered_to( const talker & ) const
{
    return {};
}

std::string talker::proficiency_training_text( const talker &, const proficiency_id & ) const
{
    return {};
}

std::vector<matype_id> talker::styles_offered_to( const talker & ) const
{
    return {};
}

std::string talker::style_training_text( const talker &, const matype_id & ) const
{
    return {};
}

std::vector<spell_id> talker::spells_offered_to( talker & )
{
    return {};
}

std::string talker::spell_training_text( talker &, const spell_id & )
{
    return {};
}

bool talker::has_effect( const efftype_id &, const bodypart_id & ) const
{
    return false;
}

effect talker::get_effect( const efftype_id &, const bodypart_id & ) const
{
    return effect::null_effect;
}

bool talker::is_deaf() const
{
    return false;
}

bool talker::can_see() const
{
    return false;
}

bool talker::is_mute() const
{
    return false;
}

std::string talker::get_value( const std::string & ) const
{
    return "";
}

bool talker::is_wearing( const itype_id & ) const
{
    return false;
}

int talker::charges_of( const itype_id & ) const
{
    return 0;
}

bool talker::has_charges( const itype_id &, int ) const
{
    return false;
}

bool talker::has_charges( const itype_id &, int, bool ) const
{
    return false;
}

std::list<item> talker::use_charges( const itype_id &, int )
{
    return {};
}

std::list<item> talker::use_charges( const itype_id &, int, bool )
{
    return {};
}

bool talker::has_amount( const itype_id &, int ) const
{
    return false;
}

int talker::get_amount( const itype_id & ) const
{
    return 0;
}

std::list<item> talker::use_amount( const itype_id &, int )
{
    return {};
}

int talker::value( const item & ) const
{
    return 0;
}

int talker::cash() const
{
    return 0;
}

int talker::debt() const
{
    return 0;
}

int talker::sold() const
{
    return 0;
}

std::vector<item *> talker::items_with( const std::function<bool( const item & )> & ) const
{
    return {};
}

std::vector<const item *> talker::const_items_with( const std::function<bool( const item & )> & )
const
{
    return {};
}

bool talker::unarmed_attack() const
{
    return false;
}

bool talker::can_stash_weapon() const
{
    return false;
}

bool talker::has_stolen_item( const talker & ) const
{
    return false;
}

int talker::cash_to_favor( int ) const
{
    return 0;
}

std::string talker::give_item_to( bool )
{
    return _( "Nope." );
}

bool talker::buy_from( int )
{
    return false;
}

bool talker::buy_monster( talker &, const mtype_id &, int, int, bool, const translation & )
{
    return false;
}

std::vector<mission *> talker::available_missions() const
{
    return {};
}

std::vector<mission *> talker::assigned_missions() const
{
    return {};
}

mission *talker::selected_mission() const
{
    return nullptr;
}

void talker::select_mission( mission * )
{
}

faction *talker::get_faction() const
{
    return nullptr;
}

bool talker::is_following() const
{
    return false;
}

bool talker::is_friendly( const Character & ) const
{
    return false;
}

bool talker::is_player_ally() const
{
    return false;
}

bool talker::turned_hostile() const
{
    return false;
}

bool talker::is_enemy() const
{
    return false;
}

bool talker::has_ai_rule( const std::string &, const std::string & ) const
{
    return false;
}

std::string talker::get_job_description() const
{
    return "";
}

std::string talker::evaluation_by( const talker & ) const
{
    return "";
}

std::string talker::short_description() const
{
    return "";
}

bool talker::has_activity() const
{
    return false;
}

bool talker::is_mounted() const
{
    return false;
}

bool talker::is_myclass( const npc_class_id & ) const
{
    return false;
}

int talker::get_activity_level() const
{
    return 0;
}

int talker::get_fatigue() const
{
    return 0;
}

int talker::get_hunger() const
{
    return 0;
}

int talker::get_thirst() const
{
    return 0;
}

int talker::get_instant_thirst() const
{
    return 0;
}

int talker::get_stored_kcal() const
{
    return 0;
}

int talker::get_healthy_kcal() const
{
    return 0;
}

int talker::get_stim() const
{
    return 0;
}

int talker::get_addiction_intensity( const addiction_id & ) const
{
    return 0;
}

int talker::get_addiction_turns( const addiction_id & ) const
{
    return 0;
}

bool talker::is_in_control_of( const vehicle & ) const
{
    return false;
}

bool talker::enslave_mind()
{
    return false;
}

std::string talker::opinion_text() const
{
    return "";
}

bool talker::is_safe() const
{
    return true;
}

int talker::pain_cur() const
{
    return 0;
}

bool talker::worn_with_flag( const flag_id &, const bodypart_id & ) const
{
    return false;
}

bool talker::wielded_with_flag( const flag_id & ) const
{
    return false;
}

bool talker::has_item_with_flag( const flag_id & ) const
{
    return false;
}

int talker::item_rads( const flag_id &, aggregate_type ) const
{
    return 0;
}

units::energy talker::power_cur() const
{
    return 0_kJ;
}

units::energy talker::power_max() const
{
    return 0_kJ;
}

void talker::set_power_cur( units::energy ) {}

int talker::mana_cur() const
{
    return 0;
}

int talker::mana_max() const
{
    return 0;
}

int talker::morale_cur() const
{
    return 0;
}

int talker::focus_cur() const
{
    return 0;
}

int talker::get_pkill() const
{
    return 0;
}

int talker::get_stamina() const
{
    return 0;
}

int talker::get_sleep_deprivation() const
{
    return 0;
}

int talker::get_rad() const
{
    return 0;
}

int talker::get_anger() const
{
    return 0;
}

int talker::get_friendly() const
{
    return 0;
}

void talker::add_morale( const morale_type &, int, int, time_duration, time_duration, bool ) {}

int talker::get_kill_xp() const
{
    return 0;
}

int talker::get_age() const
{
    return 0;
}

int talker::get_height() const
{
    return 0;
}

int talker::get_npc_trust() const
{
    return 0;
}

int talker::get_npc_fear() const
{
    return 0;
}

int talker::get_npc_value() const
{
    return 0;
}

int talker::get_npc_anger() const
{
    return 0;
}

int talker::get_bmi_permil() const
{
    return 0;
}

const move_mode_id &talker::get_move_mode() const
{
    return move_mode_id::NULL_ID();
}

int talker::get_fine_detail_vision_mod() const
{
    return 0;
}

int talker::get_health() const
{
    return 0;
}

int talker::get_body_temp() const
{
    return 0;
}

int talker::get_body_temp_delta() const
{
    return 0;
}

std::vector<bodypart_id> talker::get_all_body_parts( bool, bool ) const
{
    return std::vector<bodypart_id>();
}

int talker::get_part_hp_cur( const bodypart_id & ) const
{
    return 0;
}

int talker::get_part_hp_max( const bodypart_id & ) const
{
    return 0;
}

bool talker::knows_martial_art( const matype_id & ) const
{
    return false;
}
