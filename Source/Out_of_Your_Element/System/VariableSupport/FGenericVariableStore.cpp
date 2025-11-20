#include "FGenericVariableStore.h"

// ---------- Int ----------
void FGenericVariableStore::SetInt(const FString& Name, int32 Value)
{
    FGenericVariable& Var = Variables.FindOrAdd(Name);
    Var.Set(Value);
}

bool FGenericVariableStore::GetInt(const FString& Name, int32& OutValue) const
{
    if (const FGenericVariable* Var = Variables.Find(Name))
    {
        return Var->GetInt(OutValue);
    }
    return false;
}

// ---------- Float ----------
void FGenericVariableStore::SetFloat(const FString& Name, float Value)
{
    FGenericVariable& Var = Variables.FindOrAdd(Name);
    Var.Set(Value);
}

bool FGenericVariableStore::GetFloat(const FString& Name, float& OutValue) const
{
    if (const FGenericVariable* Var = Variables.Find(Name))
    {
        return Var->GetFloat(OutValue);
    }
    return false;
}

// ---------- Bool ----------
void FGenericVariableStore::SetBool(const FString& Name, bool Value)
{
    FGenericVariable& Var = Variables.FindOrAdd(Name);
    Var.Set(Value);
}

bool FGenericVariableStore::GetBool(const FString& Name, bool& OutValue) const
{
    if (const FGenericVariable* Var = Variables.Find(Name))
    {
        return Var->GetBool(OutValue);
    }
    return false;
}

// ---------- String ----------
void FGenericVariableStore::SetString(const FString& Name, const FString& Value)
{
    FGenericVariable& Var = Variables.FindOrAdd(Name);
    Var.Set(Value);
}

bool FGenericVariableStore::GetString(const FString& Name, FString& OutValue) const
{
    if (const FGenericVariable* Var = Variables.Find(Name))
    {
        return Var->GetString(OutValue);
    }
    return false;
}

// ---------- String Array ----------
void FGenericVariableStore::SetStringArray(const FString& Name, const TArray<FString>& Value)
{
    FGenericVariable& Var = Variables.FindOrAdd(Name);
    Var.Set(Value); // calls FGenericVariable::Set(const TArray<FString>&)
}

bool FGenericVariableStore::GetStringArray(const FString& Name, TArray<FString>& OutValue) const
{
    if (const FGenericVariable* Var = Variables.Find(Name))
    {
        return Var->GetStringArray(OutValue);
    }
    return false;
}

// ---------- Misc ----------
bool FGenericVariableStore::HasVariable(const FString& Name) const
{
    return Variables.Contains(Name);
}

void FGenericVariableStore::RemoveVariable(const FString& Name)
{
    Variables.Remove(Name);
}

void FGenericVariableStore::Clear()
{
    Variables.Empty();
}
