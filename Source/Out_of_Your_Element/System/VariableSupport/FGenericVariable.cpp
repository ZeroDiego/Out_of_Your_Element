#include "FGenericVariable.h"

void FGenericVariable::Set(int32 Value)
{
	Type = EVariableType::Int;
	IntValue = Value;
	
	FloatValue = 0.0f;
	BoolValue = false;
	StringValue.Reset();
    StringArrayValue.Reset();
}

void FGenericVariable::Set(float Value)
{
	Type = EVariableType::Float;
	FloatValue = Value;
	
	IntValue = 0;
	BoolValue = false;
	StringValue.Reset();
    StringArrayValue.Reset();
}

void FGenericVariable::Set(bool Value)
{
	Type = EVariableType::Bool;
	BoolValue = Value;
	
	IntValue = 0;
	FloatValue = 0.0f;
	StringValue.Reset();
    StringArrayValue.Reset();
}

void FGenericVariable::Set(const FString& Value)
{
	Type = EVariableType::String;
	StringValue = Value;
	
	IntValue = 0;
	FloatValue = 0.0f;
	BoolValue = false;
    StringArrayValue.Reset();
}

void FGenericVariable::Set(const TArray<FString>& Value)
{
	Type = EVariableType::StringArray;
	StringArrayValue = Value;

	IntValue = 0;
	FloatValue = 0.0f;
	BoolValue = false;
	StringValue.Reset();
}

void FGenericVariable::Set(const TCHAR* Value)
{
	Set(FString(Value));
}

bool FGenericVariable::GetInt(int32& OutValue) const
{
	if (Type == EVariableType::Int)
	{
		OutValue = IntValue;
		return true;
	}
	return false;
}

bool FGenericVariable::GetFloat(float& OutValue) const
{
	if (Type == EVariableType::Float)
	{
		OutValue = FloatValue;
		return true;
	}
	return false;
}

bool FGenericVariable::GetBool(bool& OutValue) const
{
	if (Type == EVariableType::Bool)
	{
		OutValue = BoolValue;
		return true;
	}
	return false;
}

bool FGenericVariable::GetString(FString& OutValue) const
{
	if (Type == EVariableType::String)
	{
		OutValue = StringValue;
		return true;
	}
	return false;
}

bool FGenericVariable::GetStringArray(TArray<FString>& OutValue) const
{
	if (Type == EVariableType::StringArray)
	{
		OutValue = StringArrayValue;
		return true;
	}
	return false;
}