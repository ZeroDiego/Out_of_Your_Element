#pragma once

UENUM(BlueprintType)
enum class EVariableType : uint8
{
	None,
	Int,
	Float,
	Bool,
	String
};

USTRUCT(BlueprintType)
struct FGenericVariable
{
    GENERATED_BODY()
	
public:
	UPROPERTY()
	EVariableType Type = EVariableType::None;

	UPROPERTY()
	int32 IntValue = 0;

	UPROPERTY()
	float FloatValue = 0.0f;

	UPROPERTY()
	bool BoolValue = false;

	UPROPERTY()
	FString StringValue;
	
	void Set(int32 Value)
	{
		Type = EVariableType::Int;
		IntValue = Value;
	}
	void Set(float Value)
	{
		Type = EVariableType::Float;
		FloatValue = Value;
	}
	void Set(bool Value)
	{
		Type = EVariableType::Bool;
		BoolValue = Value;
	}
	void Set(const FString& Value)
	{
		Type = EVariableType::String;
		StringValue = Value;
	}
	
	bool GetInt(int32& OutValue) const
	{
		if (Type == EVariableType::Int)
		{
			OutValue = IntValue;
			return true;
		}
		return false;
	}
	bool GetFloat(float& OutValue) const
	{
		if (Type == EVariableType::Float)
		{
			OutValue = FloatValue;
			return true;
		}
		return false;
	}
	bool GetBool(bool& OutValue) const
	{
		if (Type == EVariableType::Bool)
		{
			OutValue = BoolValue;
			return true;
		}
		return false;
	}
	bool GetString(FString& OutValue) const
	{
		if (Type == EVariableType::String)
		{
			OutValue = StringValue;
			return true;
		}
		return false;
	}
};
