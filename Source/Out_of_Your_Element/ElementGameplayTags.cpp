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

	UE_DEFINE_GAMEPLAY_TAG(Abilities_Cooldowns, "Abilities.Cooldowns");
	UE_DEFINE_GAMEPLAY_TAG(Abilities_Cooldowns_Fireball, "Abilities.Cooldowns.Fireball");
	UE_DEFINE_GAMEPLAY_TAG(Abilities_Cooldowns_WaterGun, "Abilities.Cooldowns.WaterGun");
	UE_DEFINE_GAMEPLAY_TAG(Abilities_Cooldowns_RockThrow, "Abilities.Cooldowns.RockThrow");
	UE_DEFINE_GAMEPLAY_TAG(Abilities_Cooldowns_FireZone, "Abilities.Cooldowns.FireZone");
	UE_DEFINE_GAMEPLAY_TAG(Abilities_Cooldowns_IceLance, "Abilities.Cooldowns.IceLance");
	UE_DEFINE_GAMEPLAY_TAG(Abilities_Cooldowns_RockWall, "Abilities.Cooldowns.RockWall");
	UE_DEFINE_GAMEPLAY_TAG(Abilities_Cooldowns_Meteor, "Abilities.Cooldowns.Meteor");
	UE_DEFINE_GAMEPLAY_TAG(Abilities_Cooldowns_Freeze, "Abilities.Cooldowns.Freeze");
	UE_DEFINE_GAMEPLAY_TAG(Abilities_Cooldowns_Thorns, "Abilities.Cooldowns.Thorns");

	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Abilities_Fire,
	                               "Abilities.Fire",
	                               "Tag granted when being hit by fire attacks");

	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Abilities_Water,
	                               "Abilities.Water",
	                               "Tag granted when being hit by water attacks");

	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Abilities_Nature,
	                               "Abilities.Nature",
	                               "Tag granted when being hit by nature attacks");

	UE_DEFINE_GAMEPLAY_TAG(GameplayCue_Status_Frozen, "GameplayCue.Status.Frozen");
	UE_DEFINE_GAMEPLAY_TAG(GameplayCue_Status_Burning, "GameplayCue.Status.Burning");
}
