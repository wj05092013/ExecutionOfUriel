// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ExecutionOfUriel.h"
#include "BehaviorTree/BTService.h"
#include "BTService_CheckRange.generated.h"

/**
 * 
 */
UCLASS()
class EXECUTIONOFURIEL_API UBTService_CheckRange : public UBTService
{
	GENERATED_BODY()
	
public:
	UBTService_CheckRange();
	
protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
