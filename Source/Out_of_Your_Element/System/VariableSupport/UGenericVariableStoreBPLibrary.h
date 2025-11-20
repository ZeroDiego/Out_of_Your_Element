#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "FGenericVariableStore.h"
#include "UGenericVariableStoreBPLibrary.generated.h"

/**
 * Blueprint helper functions for working with FGenericVariableStore.
 *
 * These are thin wrappers around the C++ methods on FGenericVariableStore,
 * so you can manipulate named variables from Blueprints.
 *
 * Usage example (Blueprint):
 *   - Have a UPROPERTY of type FGenericVariableStore (e.g. GlobalVariables on GameInstance).
 *   - Drag it into the graph (Get GlobalVariables).
 *   - Call GV_SetInt / GV_GetString / etc. on it.
 */
UCLASS()
class OUT_OF_YOUR_ELEMENT_API UGenericVariableStoreBPLibrary : public UBlueprintFunctionLibrary
{
    GENERATED_BODY()
	
public:
	
    // ---------- Int ----------
	
    /** Set an int variable in the store under the given Name key. */
	UFUNCTION(BlueprintCallable, Category = "Generic Variable Store")
	static void GV_SetInt(UPARAM(ref) FGenericVariableStore& Store, const FString& Name, int32 Value);

	/**
	 * Get an int variable from the store.
	 * Returns true if the variable exists AND currently stores an int.
	 */
	UFUNCTION(BlueprintCallable, Category = "Generic Variable Store")
	static bool GV_GetInt(UPARAM(ref) FGenericVariableStore& Store, const FString& Name, int32& OutValue);

	// ---------- Float ----------
	
	/** Set a float variable in the store under the given Name key. */
	UFUNCTION(BlueprintCallable, Category = "Generic Variable Store")
	static void GV_SetFloat(UPARAM(ref) FGenericVariableStore& Store, const FString& Name, float Value);

	/**
	 * Get a float variable from the store.
	 * Returns true if the variable exists AND currently stores a float.
	 */
	UFUNCTION(BlueprintCallable, Category = "Generic Variable Store")
	static bool GV_GetFloat(UPARAM(ref) FGenericVariableStore& Store, const FString& Name, float& OutValue);

	// ---------- Bool ----------

	/** Set a bool variable in the store under the given Name key. */
	UFUNCTION(BlueprintCallable, Category = "Generic Variable Store")
	static void GV_SetBool(UPARAM(ref) FGenericVariableStore& Store, const FString& Name, bool Value);

	/**
	 * Get a bool variable from the store.
	 * Returns true if the variable exists AND currently stores a bool.
	 */
	UFUNCTION(BlueprintCallable, Category = "Generic Variable Store")
	static bool GV_GetBool(UPARAM(ref) FGenericVariableStore& Store, const FString& Name, bool& OutValue);

	// ---------- String ----------

	/** Set a string variable in the store under the given Name key. */
	UFUNCTION(BlueprintCallable, Category = "Generic Variable Store")
	static void GV_SetString(UPARAM(ref) FGenericVariableStore& Store, const FString& Name, const FString& Value);

	/**
	 * Get a string variable from the store.
	 * Returns true if the variable exists AND currently stores a string.
	 */
	UFUNCTION(BlueprintCallable, Category = "Generic Variable Store")
	static bool GV_GetString(UPARAM(ref) FGenericVariableStore& Store, const FString& Name, FString& OutValue);

	// ---------- String Array ----------

	/** Set a string array variable in the store under the given Name key. */
	UFUNCTION(BlueprintCallable, Category = "Generic Variable Store")
	static void GV_SetStringArray(UPARAM(ref) FGenericVariableStore& Store, const FString& Name, const TArray<FString>& Value);

	/**
	 * Get a string array variable from the store.
	 * Returns true if the variable exists AND currently stores a string array.
	 */
	UFUNCTION(BlueprintCallable, Category = "Generic Variable Store")
	static bool GV_GetStringArray(UPARAM(ref) FGenericVariableStore& Store, const FString& Name, TArray<FString>& OutValue);

    // ---------- Misc ----------

    /** Returns true if the store contains a variable with the given Name (regardless of type). */
    UFUNCTION(BlueprintCallable, Category = "Generic Variable Store")
    static bool GV_HasVariable(UPARAM(ref) FGenericVariableStore& Store, const FString& Name);

    /** Removes the variable with the given Name from the store (if it exists). */
    UFUNCTION(BlueprintCallable, Category = "Generic Variable Store")
    static void GV_RemoveVariable(UPARAM(ref) FGenericVariableStore& Store, const FString& Name);

    /** Removes all variables from the store. */
    UFUNCTION(BlueprintCallable, Category = "Generic Variable Store")
    static void GV_Clear(UPARAM(ref) FGenericVariableStore& Store);
};
