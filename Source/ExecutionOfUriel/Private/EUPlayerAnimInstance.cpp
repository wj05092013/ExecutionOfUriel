// Fill out your copyright notice in the Description page of Project Settings.


#include "EUPlayerAnimInstance.h"
#include "EUPlayerCharacter.h"

void UEUPlayerAnimInstance::AnimNotify_CheckCombo()
{
	OnCheckCombo.Broadcast();
}
