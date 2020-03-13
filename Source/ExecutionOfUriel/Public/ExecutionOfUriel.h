// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "EngineMinimal.h"

DECLARE_LOG_CATEGORY_EXTERN(ExecutionOfUriel, Log, All);

#define EULOG_CALLINFO (FString(__FUNCTION__) + TEXT("(") + FString::FromInt(__LINE__) + TEXT(")"))
#define EULOG_S(Verbosity) UE_LOG(ExecutionOfUriel, Verbosity, TEXT("%s"), *EULOG_CALLINFO)
#define EULOG(Verbosity, Format, ...) UE_LOG(ExecutionOfUriel, Verbosity, TEXT("%s %s"), *EULOG_CALLINFO, *FString::Printf(Format, ##__VA_ARGS__))

#define EUCHECK(Expr, ...) { if( !(Expr) ) { EULOG(Error, TEXT("ASSERTION: %s"), TEXT("'"#Expr"'")); return __VA_ARGS__; } }
