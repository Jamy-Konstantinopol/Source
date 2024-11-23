#pragma once

#include "GameFramework/Controller.h"
#include "Player/CLPPlayerState.h"

class CLPUtils
{
public:
	bool static AreEnemies(AController* Controller1, AController* Controller2)
	{
		if (Controller1 && Controller2 && Controller1 != Controller2)
		{
			const ACLPPlayerState* PlayerState1 = Cast<ACLPPlayerState>(Controller1->PlayerState);
			const ACLPPlayerState* PlayerState2 = Cast<ACLPPlayerState>(Controller2->PlayerState);

			return PlayerState1 && PlayerState2 && PlayerState1->GetTeamID() != PlayerState2->GetTeamID();
		}
		return false;
	}
};