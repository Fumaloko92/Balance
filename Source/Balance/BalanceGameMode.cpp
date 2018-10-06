// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

#include "Balance.h"
#include "BalanceBall.h"
#include "BalanceGameMode.h"

ABalanceGameMode::ABalanceGameMode()
{
	// set default pawn class to our ball

	DefaultPawnClass = ABalanceBall::StaticClass();
}
