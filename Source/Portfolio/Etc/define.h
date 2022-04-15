#pragma once

DECLARE_LOG_CATEGORY_EXTERN(Client, Log, All);

#define LOG(Verbose, String) UE_LOG(Client, Verbose, TEXT("%s"),\
					 *FString::Printf(TEXT("%s : { %d } : %s"), *FString(__FUNCTION__), __LINE__, String));

UENUM(BlueprintType)
enum class EPLAYER_STATE : uint8
{
	SWORD_IDLE_L,
	SWORD_IDLE_R,
	SWORD_RUN,
	SWORD_RUN_END,
	SWORD_COMBO_A_1,
	SWORD_COMBO_A_2,
	SWORD_COMBO_A_3,
	SWORD_COMBO_A_4,
	SWORD_COMBO_A_5,
	SWORD_COMBO_B_1,
	SWORD_COMBO_B_2,
	SWORD_COMBO_B_3,
	SWORD_COMBO_B_4,
	SWORD_COMBO_B_5,
	SWORD_JUMP,
	SWORD_DAMAGE,
	SWORD_FALL,
	SWORD_DEAD
};