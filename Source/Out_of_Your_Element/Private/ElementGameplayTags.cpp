#include "ElementGameplayTags.h"

namespace ElementGameplayTags
{
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Damage_Type_Fire, "Damage.Type.Fire", "Fire Damage");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Damage_Type_Water, "Damage.Type.Water", "Water Damage");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Damage_Type_Nature, "Damage.Type.Nature", "Nature Damage");

	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Abilities_Parameters_Damage,
	                               "Abilities.Parameters.Damage",
	                               "Raw damage amount from an attack");

	const FGameplayTagContainer DamageTypes = FGameplayTagContainer::CreateFromArray(TArray<FGameplayTag>{
		Damage_Type_Fire, Damage_Type_Water, Damage_Type_Nature
	});
}
