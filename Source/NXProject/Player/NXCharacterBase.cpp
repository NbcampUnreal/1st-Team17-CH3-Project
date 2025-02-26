#include "Player/NXCharacterBase.h"

ANXCharacterBase::ANXCharacterBase()
{
	
	MaxHealth = 100.0f;
	Health = MaxHealth;
	
}



int ANXCharacterBase::GetHealth() const
{
	return 0;
}

void ANXCharacterBase::AddHealth(float Amount)
{
	Health = FMath::Clamp(Health + Amount, 0.0f, MaxHealth);

}

