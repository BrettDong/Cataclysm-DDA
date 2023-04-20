#pragma once
#ifndef CATA_SRC_TALKER_H
#define CATA_SRC_TALKER_H

#include "coordinates.h"
#include "type_id.h"
#include "units_fwd.h"
#include <list>

class Character;
class Creature;
class character_id;
class computer;
class effect;
class faction;
class item;
class item_location;
class mission;
class monster;
class npc;
struct npc_opinion;
class recipe;
struct tripoint;
class vehicle;

/*
 * Talker is an entity independent way of providing a participant in a dialogue.
 * Talker is a virtual abstract class and should never really be used.  Instead,
 * entity specific talker child classes such as character_talker should be used.
 */
class talker
{
    public:
        virtual ~talker();
        // virtual member accessor functions
        virtual Character *get_character();
        virtual const Character *get_character() const;
        virtual npc *get_npc();
        virtual npc *get_npc() const;
        virtual item_location *get_item();
        virtual item_location *get_item() const;
        virtual monster *get_monster();
        virtual const monster *get_monster() const;
        virtual Creature *get_creature();
        virtual const Creature *get_creature() const;
        virtual computer *get_computer();
        virtual const computer *get_computer() const;
        // identity and location
        virtual std::string disp_name() const;
        virtual character_id getID() const;
        virtual bool is_male() const;
        virtual std::vector<std::string> get_grammatical_genders() const;
        virtual int posx() const;
        virtual int posy() const;
        virtual int posz() const;
        virtual tripoint pos() const;
        virtual tripoint_abs_ms global_pos() const;
        virtual tripoint_abs_omt global_omt_location() const;
        virtual void set_pos( tripoint ) {}
        virtual std::string distance_to_goal() const;

        // mandatory functions for starting a dialogue
        virtual bool will_talk_to_u( const Character &, bool );
        virtual std::vector<std::string> get_topics( bool );
        virtual void check_missions() {}
        virtual void update_missions( const std::vector<mission *> & ) {}
        virtual bool check_hostile_response( int ) const;
        virtual int parse_mod( const std::string &, int ) const;
        virtual int trial_chance_mod( const std::string & ) const;
        virtual int get_cur_hp( const bodypart_id & ) const;
        virtual int get_cur_part_temp( const bodypart_id & ) const;

        // stats, skills, traits, bionics, and magic
        virtual int str_cur() const;
        virtual int dex_cur() const;
        virtual int int_cur() const;
        virtual int per_cur() const;
        virtual void set_str_max( int ) {}
        virtual void set_dex_max( int ) {}
        virtual void set_int_max( int ) {}
        virtual void set_per_max( int ) {}
        virtual void set_str_bonus( int ) {}
        virtual void set_dex_bonus( int ) {}
        virtual void set_int_bonus( int ) {}
        virtual void set_per_bonus( int ) {}
        virtual int get_str_max() const;
        virtual int get_dex_max() const;
        virtual int get_int_max() const;
        virtual int get_per_max() const;
        virtual int get_str_bonus() const;
        virtual int get_dex_bonus() const;
        virtual int get_int_bonus() const;
        virtual int get_per_bonus() const;
        virtual int get_skill_level( const skill_id & ) const;
        virtual int get_spell_level( const trait_id & ) const;
        virtual int get_spell_level( const spell_id & ) const;
        virtual int get_spell_exp( const spell_id & ) const;
        virtual int get_highest_spell_level() const;
        virtual void set_spell_level( const spell_id &, int ) {}
        virtual void set_spell_exp( const spell_id &, int ) {}
        virtual void set_skill_level( const skill_id &, int ) {}
        virtual bool has_trait( const trait_id & ) const;
        virtual bool has_recipe( const recipe_id & ) const;
        virtual void learn_recipe( const recipe_id & ) {}
        virtual void mutate( const int &, const bool & ) {}
        virtual void mutate_category( const mutation_category_id &, const bool & ) {}
        virtual void set_mutation( const trait_id & ) {}
        virtual void unset_mutation( const trait_id & ) {}
        virtual void set_fatigue( int ) {};
        virtual bool has_flag( const json_character_flag & ) const;
        virtual bool crossed_threshold() const;
        virtual int num_bionics() const;
        virtual bool has_max_power() const;
        virtual bool has_bionic( const bionic_id & ) const;
        virtual bool knows_spell( const spell_id & ) const;
        virtual bool knows_proficiency( const proficiency_id & ) const;
        virtual time_duration proficiency_practiced_time( const proficiency_id & ) const;
        virtual void set_proficiency_practiced_time( const proficiency_id &, int ) {}
        virtual std::vector<skill_id> skills_offered_to( const talker & ) const;
        virtual std::vector<skill_id> skills_teacheable() const;
        virtual std::string skill_seminar_text( const skill_id & ) const;
        virtual std::string skill_training_text( const talker &, const skill_id & ) const;
        virtual std::vector<proficiency_id> proficiencies_offered_to( const talker & ) const;
        virtual std::string proficiency_training_text( const talker &, const proficiency_id & ) const;
        virtual std::vector<matype_id> styles_offered_to( const talker & ) const;
        virtual std::string style_training_text( const talker &, const matype_id & ) const;
        virtual std::vector<spell_id> spells_offered_to( talker & );
        virtual std::string spell_training_text( talker &, const spell_id & );
        virtual void store_chosen_training( const skill_id &, const matype_id &,
                                            const spell_id &, const proficiency_id & ) {
        }

        // effects and values
        virtual bool has_effect( const efftype_id &, const bodypart_id & ) const;
        virtual effect get_effect( const efftype_id &, const bodypart_id & ) const;
        virtual bool is_deaf() const;
        virtual bool can_see() const;
        virtual bool is_mute() const;
        virtual void add_effect( const efftype_id &, const time_duration &, const std::string &, bool, bool,
                                 int ) {}
        virtual void remove_effect( const efftype_id & ) {}
        virtual void add_bionic( const bionic_id & ) {}
        virtual void remove_bionic( const bionic_id & ) {}
        virtual std::string get_value( const std::string & ) const;
        virtual void set_value( const std::string &, const std::string & ) {}
        virtual void remove_value( const std::string & ) {}

        // inventory, buying, and selling
        virtual bool is_wearing( const itype_id & ) const;
        virtual int charges_of( const itype_id & ) const;
        virtual bool has_charges( const itype_id &, int ) const;
        // bool = match tool containing charges of itype_id
        virtual bool has_charges( const itype_id &, int, bool ) const;
        virtual std::list<item> use_charges( const itype_id &, int );
        // bool = match tool containing charges of itype_id
        virtual std::list<item> use_charges( const itype_id &, int, bool );
        virtual bool has_amount( const itype_id &, int ) const;
        virtual int get_amount( const itype_id & ) const;
        virtual std::list<item> use_amount( const itype_id &, int );
        virtual int value( const item & ) const;
        virtual int cash() const;
        virtual int debt() const;
        virtual void add_debt( int ) {}
        virtual int sold() const;
        virtual void add_sold( int ) {}
        virtual std::vector<item *> items_with( const std::function<bool( const item & )> & ) const;
        virtual std::vector<const item *> const_items_with( const std::function<bool( const item & )> & )
        const;
        virtual void i_add( const item & ) {}
        virtual void i_add_or_drop( item & ) {}
        virtual void remove_items_with( const std::function<bool( const item & )> & ) {}
        virtual bool unarmed_attack() const;
        virtual bool can_stash_weapon() const;
        virtual bool has_stolen_item( const talker & ) const;
        virtual int cash_to_favor( int ) const;
        virtual std::string give_item_to( bool );
        virtual bool buy_from( int );
        virtual bool buy_monster( talker &, const mtype_id &, int, int, bool,
                                  const translation & );

        // missions
        virtual std::vector<mission *> available_missions() const;
        virtual std::vector<mission *> assigned_missions() const;
        virtual mission *selected_mission() const;
        virtual void select_mission( mission * );
        virtual void add_mission( const mission_type_id & ) {}
        virtual void set_companion_mission( const std::string & ) {}

        // factions and alliances
        virtual faction *get_faction() const;
        virtual void set_fac( const faction_id & ) {}
        virtual void add_faction_rep( int ) {}
        virtual bool is_following() const;
        virtual bool is_friendly( const Character & )  const;
        virtual bool is_player_ally()  const;
        virtual bool turned_hostile() const;
        virtual bool is_enemy() const;
        virtual void make_angry() {}

        // ai rules
        virtual bool has_ai_rule( const std::string &, const std::string & ) const;
        virtual void toggle_ai_rule( const std::string &, const std::string & ) {}
        virtual void set_ai_rule( const std::string &, const std::string & ) {}
        virtual void clear_ai_rule( const std::string &, const std::string & ) {}

        // other descriptors
        virtual std::string get_job_description() const;
        virtual std::string evaluation_by( const talker & ) const;
        virtual std::string short_description() const;
        virtual bool has_activity() const;
        virtual bool is_mounted() const;
        virtual bool is_myclass( const npc_class_id & ) const;
        virtual void set_class( const npc_class_id & ) {}
        virtual int get_activity_level() const;
        virtual int get_fatigue() const;
        virtual int get_hunger() const;
        virtual int get_thirst() const;
        virtual int get_instant_thirst() const;
        virtual int get_stored_kcal() const;
        virtual int get_healthy_kcal() const;
        virtual int get_stim() const;
        virtual int get_addiction_intensity( const addiction_id & ) const;
        virtual int get_addiction_turns( const addiction_id & ) const;
        virtual void set_addiction_turns( const addiction_id &, int ) {}
        virtual void set_stored_kcal( int ) {}
        virtual void set_stim( int ) {}
        virtual void set_thirst( int ) {}
        virtual bool is_in_control_of( const vehicle & ) const;

        // speaking
        virtual void say( const std::string & ) {}
        virtual void shout( const std::string & = "", bool = false ) {}

        // miscellaneous
        virtual bool enslave_mind();
        virtual std::string opinion_text() const;
        virtual void add_opinion( const npc_opinion & ) {}
        virtual void set_first_topic( const std::string & ) {}
        virtual bool is_safe() const;
        virtual void mod_pain( int ) {}
        virtual void set_pain( int ) {}
        virtual int pain_cur() const;
        virtual bool worn_with_flag( const flag_id &, const bodypart_id & ) const;
        virtual bool wielded_with_flag( const flag_id & ) const;
        virtual bool has_item_with_flag( const flag_id & ) const;
        virtual int item_rads( const flag_id &, aggregate_type ) const;
        virtual units::energy power_cur() const;
        virtual units::energy power_max() const;
        virtual void set_power_cur( units::energy );
        virtual int mana_cur() const;
        virtual int mana_max() const;
        virtual void set_mana_cur( int ) {}
        virtual void mod_daily_health( int, int ) {}
        virtual int morale_cur() const;
        virtual int focus_cur() const;
        virtual void mod_focus( int ) {}
        virtual int get_pkill() const;
        virtual void set_pkill( int ) {}
        virtual int get_stamina() const;
        virtual void set_stamina( int ) {}
        virtual int get_sleep_deprivation() const;
        virtual void set_sleep_deprivation( int ) {}
        virtual int get_rad() const;
        virtual void set_rad( int ) {}
        virtual int get_anger() const;
        virtual void set_anger( int ) {}
        virtual void set_morale( int ) {}
        virtual int get_friendly() const;
        virtual void set_friendly( int ) {}
        virtual void add_morale( const morale_type &, int, int, time_duration, time_duration, bool );
        virtual void remove_morale( const morale_type & ) {}
        virtual void set_kill_xp( int ) {}
        virtual int get_kill_xp() const;
        virtual void set_age( int ) {}
        virtual int get_age() const;
        virtual void set_height( int ) {}
        virtual int get_height() const;
        virtual void set_npc_trust( int ) {}
        virtual int get_npc_trust() const;
        virtual void set_npc_fear( int ) {}
        virtual int get_npc_fear() const;
        virtual void set_npc_value( int ) {}
        virtual int get_npc_value() const;
        virtual void set_npc_anger( int ) {}
        virtual int get_npc_anger() const;
        virtual int get_bmi_permil() const;
        virtual const move_mode_id &get_move_mode() const;
        virtual int get_fine_detail_vision_mod() const;
        virtual int get_health() const;
        virtual int get_body_temp() const;
        virtual int get_body_temp_delta() const;
        virtual std::vector<bodypart_id> get_all_body_parts( bool, bool ) const;
        virtual int get_part_hp_cur( const bodypart_id & ) const;
        virtual int get_part_hp_max( const bodypart_id & ) const;
        virtual void set_part_hp_cur( const bodypart_id &, int ) const {}

        virtual void learn_martial_art( const matype_id & ) const {}
        virtual void forget_martial_art( const matype_id & ) const {}
        virtual bool knows_martial_art( const matype_id & ) const;
};

#endif // CATA_SRC_TALKER_H
