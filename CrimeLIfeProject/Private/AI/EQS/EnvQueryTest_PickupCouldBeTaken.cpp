// Crime Life Project, All Rights NOT Reserved.

#include "AI/EQS/EnvQueryTest_PickupCouldBeTaken.h"

#include "EnvironmentQuery/Items/EnvQueryItemType_ActorBase.h"
#include "Pickups/CLPPickup.h"

UEnvQueryTest_PickupCouldBeTaken::UEnvQueryTest_PickupCouldBeTaken(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	Cost		  = EEnvTestCost::Low;
	ValidItemType = UEnvQueryItemType_ActorBase::StaticClass();
	SetWorkOnFloatValues(false);
}

void UEnvQueryTest_PickupCouldBeTaken::RunTest(FEnvQueryInstance& QueryInstance) const
{
	UObject* DataOwner = QueryInstance.Owner.Get();
	BoolValue.BindData(DataOwner, QueryInstance.QueryID);

	for (FEnvQueryInstance::ItemIterator It(this, QueryInstance); It; ++It)
	{
		const AActor* ItemActor = GetItemActor(QueryInstance, It.GetIndex());
		if (const ACLPPickup* Pickup = Cast<ACLPPickup>(ItemActor))
		{
			It.SetScore(TestPurpose, FilterType, Pickup->CanBeTaken(), BoolValue.GetValue());
		}
	}
}
