#include "UGenericVariableStoreBPLibrary.h"

// ---------- Int ----------

void UGenericVariableStoreBPLibrary::GV_SetInt(UPARAM(ref) FGenericVariableStore& Store, const FString& Name, int32 Value)
{
    Store.SetInt(Name, Value);
}

bool UGenericVariableStoreBPLibrary::GV_GetInt(UPARAM(ref) FGenericVariableStore& Store, const FString& Name, int32& OutValue)
{
    return Store.GetInt(Name, OutValue);
}

// ---------- Float ----------

void UGenericVariableStoreBPLibrary::GV_SetFloat(UPARAM(ref) FGenericVariableStore& Store, const FString& Name, float Value)
{
    Store.SetFloat(Name, Value);
}

bool UGenericVariableStoreBPLibrary::GV_GetFloat(UPARAM(ref) FGenericVariableStore& Store, const FString& Name, float& OutValue)
{
    return Store.GetFloat(Name, OutValue);
}

// ---------- Bool ----------

void UGenericVariableStoreBPLibrary::GV_SetBool(UPARAM(ref) FGenericVariableStore& Store, const FString& Name, bool Value)
{
    Store.SetBool(Name, Value);
}

bool UGenericVariableStoreBPLibrary::GV_GetBool(UPARAM(ref) FGenericVariableStore& Store, const FString& Name, bool& OutValue)
{
    return Store.GetBool(Name, OutValue);
}

// ---------- String ----------

void UGenericVariableStoreBPLibrary::GV_SetString(UPARAM(ref) FGenericVariableStore& Store, const FString& Name, const FString& Value)
{
    Store.SetString(Name, Value);
}

bool UGenericVariableStoreBPLibrary::GV_GetString(UPARAM(ref) FGenericVariableStore& Store, const FString& Name, FString& OutValue)
{
    return Store.GetString(Name, OutValue);
}

// ---------- String Array ----------

void UGenericVariableStoreBPLibrary::GV_SetStringArray(UPARAM(ref) FGenericVariableStore& Store, const FString& Name, const TArray<FString>& Value)
{
    Store.SetStringArray(Name, Value);
}

bool UGenericVariableStoreBPLibrary::GV_GetStringArray(UPARAM(ref) FGenericVariableStore& Store, const FString& Name, TArray<FString>& OutValue)
{
    return Store.GetStringArray(Name, OutValue);
}

// ---------- Misc ----------

bool UGenericVariableStoreBPLibrary::GV_HasVariable(UPARAM(ref) FGenericVariableStore& Store, const FString& Name)
{
    return Store.HasVariable(Name);
}

void UGenericVariableStoreBPLibrary::GV_RemoveVariable(UPARAM(ref) FGenericVariableStore& Store, const FString& Name)
{
    Store.RemoveVariable(Name);
}

void UGenericVariableStoreBPLibrary::GV_Clear(UPARAM(ref) FGenericVariableStore& Store)
{
    Store.Clear();
}

FGenericVariableStore UGenericVariableStoreBPLibrary::GV_Where(FGenericVariableStore& Store,
    const FString& NameContains)
{
    return Store.Where(NameContains);
}

bool UGenericVariableStoreBPLibrary::GV_FindFirstByNameContains(FGenericVariableStore& Store,
    const FString& NameContains, FGenericVariable& OutVariable, FString& OutFoundName, bool bIgnoreCase)
{
    return Store.FindFirstByNameContains(
        NameContains,
        OutVariable,
        OutFoundName,
        bIgnoreCase ? ESearchCase::IgnoreCase : ESearchCase::CaseSensitive);
}

void UGenericVariableStoreBPLibrary::GV_SortByName(FGenericVariableStore& Store, bool bIgnoreCase)
{
    Store.SortByName(bIgnoreCase ? ESearchCase::IgnoreCase : ESearchCase::CaseSensitive);
}

TArray<FString> UGenericVariableStoreBPLibrary::GV_GetSortedNames(FGenericVariableStore& Store, bool bIgnoreCase)
{
    return Store.GetSortedNames(bIgnoreCase ? ESearchCase::IgnoreCase : ESearchCase::CaseSensitive);
}

// ---------- Modify / Increment ----------

int32 UGenericVariableStoreBPLibrary::GV_AddInt(
    UPARAM(ref) FGenericVariableStore& Store,
    const FString& Name,
    int32 Delta)
{
    return Store.AddInt(Name, Delta);
}

float UGenericVariableStoreBPLibrary::GV_AddFloat(
    UPARAM(ref) FGenericVariableStore& Store,
    const FString& Name,
    float Delta)
{
    return Store.AddFloat(Name, Delta);
}
