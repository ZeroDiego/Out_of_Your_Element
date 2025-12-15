#include "FGenericVariableStore.h"

// ---------- Int ----------
void FGenericVariableStore::SetInt(const FString& Name, int32 Value)
{
    FGenericVariable& Var = Variables.FindOrAdd(Name);
    Var.Set(Value);
    MaybeAutoSort();
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
    MaybeAutoSort();
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
    MaybeAutoSort();
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
    MaybeAutoSort();
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
    MaybeAutoSort();
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
    MaybeAutoSort();
}

void FGenericVariableStore::Clear()
{
    Variables.Empty();
}

// ---------- Modify / Increment ----------

int32 FGenericVariableStore::AddInt(const FString& Name, int32 Delta)
{
    FGenericVariable& Var = Variables.FindOrAdd(Name);

    int32 CurrentValue = 0;
    if (Var.Type == EVariableType::Int)
    {
        CurrentValue = Var.IntValue;
    }

    const int32 NewValue = CurrentValue + Delta;
    Var.Set(NewValue);
    MaybeAutoSort();
    return NewValue;
}

float FGenericVariableStore::AddFloat(const FString& Name, float Delta)
{
    FGenericVariable& Var = Variables.FindOrAdd(Name);

    float CurrentValue = 0.0f;
    if (Var.Type == EVariableType::Float)
    {
        CurrentValue = Var.FloatValue;
    }

    const float NewValue = CurrentValue + Delta;
    Var.Set(NewValue);
    MaybeAutoSort();
    return NewValue;
}

FGenericVariableStore FGenericVariableStore::Where(const FString& NameContains, ESearchCase::Type SearchCase) const
{
    FGenericVariableStore Result;

    // Empty substring => return a full copy (everything "contains" "")
    if (NameContains.IsEmpty())
    {
        Result.Variables = Variables;
        return Result;
    }

    for (const TPair<FString, FGenericVariable>& Pair : Variables)
    {
        if (Pair.Key.Contains(NameContains, SearchCase))
        {
            Result.Variables.Add(Pair.Key, Pair.Value);
        }
    }

    return Result;
}

void FGenericVariableStore::SortByName(ESearchCase::Type SearchCase)
{
    Variables.KeySort([SearchCase](const FString& A, const FString& B)
    {
        return A.Compare(B, SearchCase) < 0;
    });
}

TArray<FString> FGenericVariableStore::GetSortedNames(ESearchCase::Type SearchCase) const
{
    TArray<FString> Keys;
    Variables.GetKeys(Keys);

    Keys.Sort([SearchCase](const FString& A, const FString& B)
    {
        return A.Compare(B, SearchCase) < 0;
    });

    return Keys;
}

void FGenericVariableStore::MaybeAutoSort()
{
    if (bAutoSortByName)
    {
        SortByName();
    }
}

