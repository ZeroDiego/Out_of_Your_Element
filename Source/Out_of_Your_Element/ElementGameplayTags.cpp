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

	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Abilities_Casting, "Abilities.Casting", "Tag when owner is casting a spell");
	UE_DEFINE_GAMEPLAY_TAG(Abilities_Casting_Fireball, "Abilities.Casting.Fireball");
	UE_DEFINE_GAMEPLAY_TAG(Abilities_Casting_WaterGun, "Abilities.Casting.WaterGun");
	UE_DEFINE_GAMEPLAY_TAG(Abilities_Casting_RockThrow, "Abilities.Casting.RockThrow");
	UE_DEFINE_GAMEPLAY_TAG(Abilities_Casting_FireZone, "Abilities.Casting.FireZone");
	UE_DEFINE_GAMEPLAY_TAG(Abilities_Casting_IceLance, "Abilities.Casting.IceLance");
	UE_DEFINE_GAMEPLAY_TAG(Abilities_Casting_RockWall, "Abilities.Casting.RockWall");
	UE_DEFINE_GAMEPLAY_TAG(Abilities_Casting_Meteor, "Abilities.Casting.Meteor");
	UE_DEFINE_GAMEPLAY_TAG(Abilities_Casting_Freeze, "Abilities.Casting.Freeze");
	UE_DEFINE_GAMEPLAY_TAG(Abilities_Casting_Thorns, "Abilities.Casting.Thorns");

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

	UE_DEFINE_GAMEPLAY_TAG(Status_Frozen, "Status.Frozen");
	UE_DEFINE_GAMEPLAY_TAG(Status_Burning, "Status.Burning");
	UE_DEFINE_GAMEPLAY_TAG(GameplayCue_Status_Frozen, "GameplayCue.Status.Frozen");
	UE_DEFINE_GAMEPLAY_TAG(GameplayCue_Status_Burning, "GameplayCue.Status.Burning");
}
