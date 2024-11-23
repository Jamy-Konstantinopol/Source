// Crime Life Project, All Rights NOT Reserved.

#include "AI/EQS/CLPEnemyEnvQueryContext.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "EnvironmentQuery/EnvQueryTypes.h"
#include "EnvironmentQuery/Items/EnvQueryItemType_Actor.h"

void UCLPEnemyEnvQueryContext::ProvideContext(FEnvQueryInstance& QueryInstance, FEnvQueryContextData& ContextData) const
{
	AActor* QueryOwner = Cast<AActor>(QueryInstance.Owner.Get());

	if (const UBlackboardComponent* Blackboard = UAIBlueprintHelperLibrary::GetBlackboard(QueryOwner))
	{
		const UObject* ContextActor = Blackboard->GetValueAsObject(EnemyActorKeyName);
		UEnvQueryItemType_Actor::SetContextHelper(ContextData, Cast<AActor>(ContextActor));
	}
}
