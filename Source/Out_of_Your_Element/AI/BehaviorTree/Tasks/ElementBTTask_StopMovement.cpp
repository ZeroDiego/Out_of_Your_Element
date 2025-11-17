#include "ElementBTTask_StopMovement.h"
#include "Out_of_Your_Element/AI/ElementalAIController.h"
#include "Out_of_Your_Element/AI/ElementAICharacterBase.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Navigation/PathFollowingComponent.h"
#include "Kismet/KismetSystemLibrary.h"

UElementBTTask_StopMovement::UElementBTTask_StopMovement()
{
	NodeName = TEXT("Stop Movement");
}

EBTNodeResult::Type UElementBTTask_StopMovement::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (AAIController* Cont = Cast<AAIController>(OwnerComp.GetAIOwner()))
	{
		//  stoppar chase movement utan att påverka physics och navlink moves
		if (Cont->GetPathFollowingComponent())
		{
			//Avbryter dens aktiva "path request"
			Cont->StopMovement();
			//Move to playerlocation från chase blir cancelled om man clearar playerlocation
			OwnerComp.GetBlackboardComponent()->ClearValue(FName("TargetLocation"));

		}

		return EBTNodeResult::Succeeded;
	}
	return EBTNodeResult::Failed;
}