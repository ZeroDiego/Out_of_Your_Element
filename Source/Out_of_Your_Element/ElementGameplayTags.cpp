#include "ElementGameplayTags.h"

namespace ElementGameplayTags
{
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Damage_Type_Fire, "Damage.Type.Fire", "Fire Damage");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Damage_Type_Water, "Damage.Type.Water", "Water Damage");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Damage_Type_Nature, "Damage.Type.Nature", "Nature Damage");

	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Abilities_Parameters_Damage,
	                               "Abilities.Parameters.Damage",
	                               "Raw damage amount from an attack");

	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Abilities_Parameters_Duration,
	                               "Abilities.Parameters.Duration",
	                               "Duration of applied Gameplay Effect");

	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Abilities_Parameters_Resistance_Percent,
	                               "Abilities.Parameters.Resistance.Percent",
	                               "Amount of resistance to an element in (0.0 - 1.0)");

	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Abilities_Parameters_Resistance_Fixed,
	                               "Abilities.Parameters.Resistance.Fixed",
	                               "Fixed amount of resistance to an element");

	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Abilities_Parameters_Heal_Percent,
	                               "Abilities.Parameters.Heal.Percent",
	                               "Healing from an element relative to the damage received (1.0 = 100%)");

	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Abilities_Parameters_Heal_Fixed,
	                               "Abilities.Parameters.Heal.Fixed",
	                               "Healing received when hit by an element");

	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Abilities_Fireball,
	                               "Abilities.Fireball",
	                               "Tag on Fireball");

	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Abilities_WaterGun,
	                               "Abilities.WaterGun",
	                               "Tag on WaterGun");

	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Abilities_RockThrow,
	                               "Abilities.RockThrow",
	                               "Tag on RockThrow");

	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Abilities_FireZone,
	                               "Abilities.FireZone",
	                               "Tag on FireZone");

	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Abilities_IceLance,
	                               "Abilities.IceLance",
	                               "Tag on IceLance");

	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Abilities_RockWall,
	                               "Abilities.RockWall",
	                               "Tag on RockWall");

	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Abilities_Cooldowns,
								   "Abilities.Cooldowns",
								   "Parent for cooldown tags");

	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Abilities_Cooldowns_Fireball,
	                               "Abilities.Cooldowns.Fireball",
	                               "Cooldown for Fireball");

	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Abilities_Cooldowns_WaterGun,
	                               "Abilities.Cooldowns.WaterGun",
	                               "Cooldown for WaterGun");

	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Abilities_Cooldowns_RockThrow,
	                               "Abilities.Cooldowns.RockThrow",
	                               "Cooldown for RockThrow");

	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Abilities_Cooldowns_FireZone,
	                               "Abilities.Cooldowns.FireZone",
	                               "Cooldown for FireZone");

	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Abilities_Cooldowns_IceLance,
	                               "Abilities.Cooldowns.IceLance",
	                               "Cooldown for IceLance");

	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Abilities_Cooldowns_RockWall,
	                               "Abilities.Cooldowns.RockWall",
	                               "Cooldown for RockWall");
	
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Abilities_Cooldowns_Meteor,
								   "Abilities.Cooldowns.Meteor",
								   "Cooldown for Meteor");

	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Abilities_Cooldowns_Freeze,
								   "Abilities.Cooldowns.Freeze",
								   "Cooldown for Freeze");

	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Abilities_Cooldowns_Thorns,
								   "Abilities.Cooldowns.Thorns",
								   "Cooldown for Thorns");

	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Abilities_Casting,
								   "Abilities.Casting",
								   "Parent for casting tags");

	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Abilities_Casting_BaseAttack,
								   "Abilities.Casting.BaseAttack",
								   "Applied when casting base attacks");

	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Abilities_Casting_SpecialAttack,
								   "Abilities.Casting.SpecialAttack",
								   "Applied when casting special attacks");

	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Abilities_Casting_HeavyAttack,
								   "Abilities.Casting.HeavyAttack",
								   "Applied when casting heavy attacks");

	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Abilities_Casting_Fireball,
								   "Abilities.Casting.Fireball",
								   "Applied when casting Fireball");

	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Abilities_Casting_WaterGun,
								   "Abilities.Casting.WaterGun",
								   "Applied when casting WaterGun");

	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Abilities_Casting_RockThrow,
								   "Abilities.Casting.RockThrow",
								   "Applied when casting RockThrow");

	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Abilities_Casting_FireZone,
								   "Abilities.Casting.FireZone",
								   "Applied when casting FireZone");

	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Abilities_Casting_IceLance,
								   "Abilities.Casting.IceLance",
								   "Applied when casting IceLance");

	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Abilities_Casting_RockWall,
								   "Abilities.Casting.RockWall",
								   "Applied when casting RockWall");

	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Abilities_Casting_Meteor,
								   "Abilities.Casting.Meteor",
								   "Applied when casting Meteor");

	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Abilities_Casting_Freeze,
								   "Abilities.Casting.Freeze",
								   "Applied when casting Freeze");

	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Abilities_Casting_Thorns,
								   "Abilities.Casting.Thorns",
								   "Applied when casting Thorns");

	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Abilities_Fire,
	                               "Abilities.Fire",
	                               "Tag granted when being hit by fire attacks");

	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Abilities_Water,
	                               "Abilities.Water",
	                               "Tag granted when being hit by water attacks");

	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Abilities_Nature,
	                               "Abilities.Nature",
	                               "Tag granted when being hit by nature attacks");
	
	UE_DEFINE_GAMEPLAY_TAG(Status_Frozen, "Status.Frozen");
	UE_DEFINE_GAMEPLAY_TAG(Status_Burning, "Status.Burning");
}
