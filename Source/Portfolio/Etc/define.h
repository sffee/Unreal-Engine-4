#pragma once

DECLARE_LOG_CATEGORY_EXTERN(Client, Log, All);

#define LOG(Verbose, String) UE_LOG(Client, Verbose, TEXT("%s"),\
					 *FString::Printf(TEXT("%s : { %d } : %s"), *FString(__FUNCTION__), __LINE__, String));

UENUM(BlueprintType)
enum class EPLAYER_STATE : uint8
{
	IDLE,
	RUN,
	RUN_END,
	COMBO_A_1,
	COMBO_A_2,
	COMBO_A_3,
	COMBO_A_4,
	COMBO_A_5,
	COMBO_A_6,
	JUMP,
	DAMAGE,
	FALL,
	DEAD
};