// Fill out your copyright notice in the Description page of Project Settings.


#include "EUPhaseCharacterAnimInstance.h"

UEUPhaseCharacterAnimInstance::UEUPhaseCharacterAnimInstance()
{
	// Attack montage
	//

	static ConstructorHelpers::FObjectFinder<UAnimMontage> ATTACK_MONTAGE(TEXT("/Game/ExecutionOfUriel/Animations/Phase_Attack_Montage.Phase_Attack_Montage"));
	EUCHECK(ATTACK_MONTAGE.Succeeded());
	AttackMontage = ATTACK_MONTAGE.Object;

	AttackMontageSectionNames.Reserve(4);
	AttackMontageSectionNames.Add(FName(TEXT("Attack_A")));
	AttackMontageSectionNames.Add(FName(TEXT("Attack_B")));
	AttackMontageSectionNames.Add(FName(TEXT("Attack_C")));
	AttackMontageSectionNames.Add(FName(TEXT("Attack_D")));
}