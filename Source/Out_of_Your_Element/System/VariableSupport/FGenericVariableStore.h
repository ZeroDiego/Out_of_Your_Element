#pragma once

#include "CoreMinimal.h"
#include "FGenericVariable.h"
#include "FGenericVariableStore.generated.h"

/**
 * FGenericVariableStore
* 
 * Reusable container for a named set of FGenericVariable values.
 * Internally this is just a TMap<FString, FGenericVariable> plus
 * convenience helpers for the most common types.
* 
 * Typical use:
 *  - Embed in UGameInstance (e.g. GlobalVariables) to keep values
 *    alive across level loads.
 *  - Embed in USaveGame (e.g. SavedVariables) to serialize/restore
 *    the same named values.
*    
 * Examples (C++):
 * 
 *      // Set values
 *      - GlobalVariables.SetInt(TEXT("PlayerLevel"), 10);
 *      - GlobalVariables.SetFloat(TEXT("MasterVolume"), 0.8f);
 *      - GlobalVariables.SetBool(TEXT("HasCompletedTutorial"), true);
 *      - GlobalVariables.SetString(TEXT("PlayerName"), TEXT("Agnar"));
 *      - TArray<FString> Tags = { TEXT("Fire"), TEXT("Boss") };\n
 *      GlobalVariables.SetStringArray(TEXT("CurrentEncounterTags"), Tags);
*      
 *      // Get values
 *      - int32 Level;\n
 *      if (GlobalVariables.GetInt(TEXT("PlayerLevel"), Level))\n
 *      {\n
 *          // use Level...\n
 *      }
 *      - FString Name;\n
 *      if (GlobalVariables.GetString(TEXT("PlayerName"), Name))\n
 *      {\n
 *          // use Name...\n
 *      }
*      
 *      // Utility
 *      - if (GlobalVariables.HasVariable(TEXT("PlayerLevel")))\n
 *      {\n
 *          // variable exists (regardless of its type)\n
 *      }
*      
 *      - GlobalVariables.Remove(TEXT("TempVar")); // remove a single entry
 *      - GlobalVariables.Clear();                 // remove all entries
*      
 * Notes:
 *  - The store does NOT enforce any schema; keys and types are up to the caller.
 *  - Type safety is handled by FGenericVariable::GetX(...) returning false
 *    when the stored type does not match the requested one.
 *  - In Blueprints, expose an FGenericVariableStore as a UPROPERTY and
 *    call these methods from C++ or wrap them in BlueprintCallable functions
 *    on your GameInstance/SaveGame as needed.
 */
USTRUCT(BlueprintType)
struct FGenericVariableStore
{
    GENERATED_BODY()
	
public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Generic Variable Store")
	TMap<FString, FGenericVariable> Variables;
	
	// ---------- Int ----------
	void SetInt(const FString& Name, int32 Value);
	bool GetInt(const FString& Name, int32& OutValue) const;

	// ---------- Float ----------
	void SetFloat(const FString& Name, float Value);
	bool GetFloat(const FString& Name, float& OutValue) const;

	// ---------- Bool ----------
	void SetBool(const FString& Name, bool Value);
	bool GetBool(const FString& Name, bool& OutValue) const;

	// ---------- String ----------
	void SetString(const FString& Name, const FString& Value);
	bool GetString(const FString& Name, FString& OutValue) const;

	// ---------- String Array ----------
	void SetStringArray(const FString& Name, const TArray<FString>& Value);
	bool GetStringArray(const FString& Name, TArray<FString>& OutValue) const;

	// ---------- Misc ----------
	bool HasVariable(const FString& Name) const;
	void RemoveVariable(const FString& Name);
	void Clear();
};