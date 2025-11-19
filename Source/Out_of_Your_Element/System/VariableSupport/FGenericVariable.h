#pragma once

#include "CoreMinimal.h"
#include "FGenericVariable.generated.h"

/**
 * Enum describing which kind of value is currently stored in FGenericVariable.
 */
UENUM(BlueprintType)
enum class EVariableType : uint8
{
	None    UMETA(DisplayName = "None"),
	Int     UMETA(DisplayName = "Int"),
	Float   UMETA(DisplayName = "Float"),
	Bool    UMETA(DisplayName = "Bool"),
	String  UMETA(DisplayName = "String"),
	StringArray  UMETA(DisplayName = "String Array")
};

/**
 * FGenericVariable
 * 
 * Small, USTRUCT-based "variant" that can store exactly ONE value of a few basic types:
 *  - int32
 *  - float
 *  - bool
 *  - FString
 *  - TArray<FString>
 *  
 * The active type is tracked by the 'Type' field (EVariableType). Only the value
 * corresponding to 'Type' should be considered valid at any time.
 *
 * Typical use:
 *  - As the value type in a TMap<FString, FGenericVariable> to implement a simple
 *    named variable table (e.g. in GameInstance, SaveGame, config structs, etc.).
 *  - To pass around "a value that might be int/float/string" without separate maps.
 *  
 * Setting a value (C++):
 *      - FGenericVariable Var;\n
 *      Var.Set(42);                          // stores int32, Type = EVariableType::Int\n
 *      Var.Set(3.14f);                       // stores float\n
 *      Var.Set(true);                        // stores bool\n
 *      Var.Set(FString(TEXT("Name")));       // stores string\n
 *      Var.Set(TArray<FString>{TEXT("Tag")}); // stores string array (if enabled)
 * 
 * Getting a value (C++):
 *      - int32 IntValue;\n
 *      if (Var.GetInt(IntValue))\n
 *      {\n
 *          // Var currently holds an int, use IntValue...\n
 *      }
 * 
 * Each GetX(...) function:
 *  - Returns true if 'Type' matches the requested type and fills OutValue.
 *  - Returns false if the stored type does not match; OutValue is left unchanged.
 *  
 * Notes:
 *  - The Set(...) helpers automatically update 'Type' and reset other fields
 *    to neutral defaults, so callers don't need to manipulate the fields directly.
 *  - In Blueprints you typically use this via a map wrapper (UGenericVariableStoreBPLibrary)
 *    or by manually reading/writing Type + the corresponding *_Value field.
 */
USTRUCT(BlueprintType)
struct FGenericVariable
{
    GENERATED_BODY()
	
public:
    // What type of value is currently stored.
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Generic Variable")
	EVariableType Type = EVariableType::None;

	// Possible value storage slots.
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Generic Variable")
	int32 IntValue = 0;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Generic Variable")
	float FloatValue = 0.0f;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Generic Variable")
	bool BoolValue = false;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Generic Variable")
	FString StringValue;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Generic Variable")
    TArray<FString> StringArrayValue;
	
	// ----- C++ convenience setters -----
	void Set(int32 Value);
	void Set(float Value);
	void Set(bool Value);
	void Set(const FString& Value);
    void Set(const TArray<FString>& Value);
	// Overload for const TCHAR* / literal strings
	void Set(const TCHAR* Value);

	// ----- C++ convenience getters (return false if type mismatch) -----
	bool GetInt(int32& OutValue) const;
	bool GetFloat(float& OutValue) const;
	bool GetBool(bool& OutValue) const;
	bool GetString(FString& OutValue) const;
    bool GetStringArray(TArray<FString>& OutValue) const;
};