// Crime Life Project, All Rights NOT Reserved.

#include "Components/CLPAIWeaponComponent.h"

#include "EnhancedInputComponent.h"
#include "Weapons/CLPWeapon.h"

DEFINE_LOG_CATEGORY_STATIC(LogCLPAIWeaponComponent, All, All);

void UCLPAIWeaponComponent::Fire()
{
	if (IsEquippedWeaponAmmoEmpty())
	{
		SwitchWeapon(1.f);
	}
	else
	{
		Super::Fire();
	}
}

void UCLPAIWeaponComponent::SwitchWeapon(const FInputActionValue& InInputValue)
{
	if (!CanEquip())
	{
		return;
	}

	for (const TPair<FName, ACLPWeapon*>& SocketWeapon : SocketWeaponMap)
	{
		if (!IsWeaponAmmoEmpty(SocketWeapon.Value))
		{
			EquipWeapon(SocketWeapon.Key);
			break;
		}
	}
}