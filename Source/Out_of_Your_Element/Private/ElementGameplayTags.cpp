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

	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Abilities_BaseAttack,
	                               "Abilities.BaseAttack",
	                               "Tag granted for all elements' level 1 attack");

	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Abilities_Cooldowns_BaseAttack,
	                               "Abilities.Cooldowns.BaseAttack",
	                               "Cooldown for all elements' level 1 attack");

	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Abilities_Fire,
	                               "Abilities.Fire",
	                               "Tag granted when using fire attacks");

	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Abilities_Water,
	                               "Abilities.Water",
	                               "Tag granted when using water attacks");

	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Abilities_Nature,
	                               "Abilities.Nature",
	                               "Tag granted when using nature attacks");

	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Abilities_Cooldowns_Fire,
	                               "Abilities.Fire",
	                               "Cooldown for fire element attacks");

	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Abilities_Cooldowns_Water,
	                               "Abilities.Water",
	                               "Cooldown for water element attacks");

	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Abilities_Cooldowns_Nature,
	                               "Abilities.Nature",
	                               "Cooldown for nature element attacks");

	const FGameplayTagContainer DamageTypes = FGameplayTagContainer::CreateFromArray(TArray<FGameplayTag>{
		Damage_Type_Fire, Damage_Type_Water, Damage_Type_Nature
	});
}
