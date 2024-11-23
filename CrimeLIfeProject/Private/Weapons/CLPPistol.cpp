// Crime Life Project, All Rights NOT Reserved.

#include "Weapons/CLPPistol.h"

#include "DrawDebugHelpers.h"
#include "Engine/World.h"

DEFINE_LOG_CATEGORY_STATIC(LogCLPPistol, All, All);

ACLPPistol::ACLPPistol()
{
	WeaponType = EWeaponType::Pistol;
	Damage	   = 30;
}
