#pragma once

#include "CoreMinimal.h"                       // Engine core types / utilities
#include "AIController.h"                    // Base AIController class
#include "ElementAICharacterBase.h"                          // Forward access to the pawn class
//#include "Perception/AIPerceptionTypes.h"     // FAIStimulus, enums, etc.
//#include "Perception/AISenseConfig_Sight.h"   // Sight sense configuration

#include "ElementalAIController.generated.h"
/**
 * 
 */
UCLASS()
class OUT_OF_YOUR_ELEMENT_API AElementalAIController : public AAIController
{
	GENERATED_BODY()

public:
	explicit AElementalAIController(const FObjectInitializer& FObjectInitializer);

protected:
	virtual void OnPossess(APawn* InPawn) override;

private:
	//UAISenseConfig_Sight* SightConfig = nullptr;

	//void SetupPerceptionSystem();

	//UFUNCTION()
	//void OnTargetDetected(AActor* Actor, FAIStimulus Stimulus);
};
