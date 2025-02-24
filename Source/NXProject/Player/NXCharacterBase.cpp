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

void ANXCharacterBase::OnDeath()
{
	UE_LOG(LogTemp, Error, TEXT("Dead"));

}

float ANXCharacterBase::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float ActualDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	Health = FMath::Clamp(Health - DamageAmount, 0.0f, MaxHealth);
	UE_LOG(LogTemp, Warning, TEXT("Health decreased to: %f"), Health);

	if (Health <= 0.0f)
	{
		OnDeath();
	}

	return ActualDamage;
}

void ANXCharacterBase::BeginPlay()
{
	Super::BeginPlay();
	
}



void ANXCharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

}
