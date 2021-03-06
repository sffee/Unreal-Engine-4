#pragma once

DECLARE_LOG_CATEGORY_EXTERN(Client, Log, All);

#define LOG(Verbose, String) UE_LOG(Client, Verbose, TEXT("%s"),\
					 *FString::Printf(TEXT("%s : { %d } : %s"), *FString(__FUNCTION__), __LINE__, String));

UENUM(BlueprintType)
enum class EWEAPON_TYPE : uint8
{
	SWORD
};

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
	SWORD_JUMP_COMBO_A_1,
	SWORD_JUMP_COMBO_A_2,
	SWORD_JUMP_COMBO_A_3,
	SWORD_JUMP_COMBO_B_1,
	SWORD_JUMP_DOWNATTACK_START,
	SWORD_JUMP_DOWNATTACK_LOOP,
	SWORD_JUMP_DOWNATTACK_END,
	SWORD_UPPER_START,
	SWORD_UPPER,
	SWORD_UPPER_JUMP,
	SWORD_DASHATTACK_START,
	SWORD_DASHATTACK_RUN_START,
	SWORD_DASHATTACK_RUN_LOOP,
	SWORD_DASHATTACK_FINISH_START,
	SWORD_DASHATTACK_FINISH_END,
	JUMP,
	JUMP_SECOND,
	JUMP_LOOP,
	JUMP_LANDING,
	DAMAGE,
	JUMP_DAMAGE_START,
	JUMP_DAMAGE_LOOP,
	JUMP_DAMAGE_LANDING,
	FALL,
	DEATH,
	EVADE
};

UENUM(BlueprintType)
enum class EENEMY_STATE : uint8
{
	SPAWN,
	SPAWN_LANDING,
	IDLE,
	RUN,
	ATTACK1,
	ATTACK2,
	ATTACK3,
	ATTACK4,
	ATTACK5,
	ATTACK6,
	ATTACK7,
	ATTACK8,
	ATTACK9,
	ATTACK10,
	ATTACK11,
	ATTACK12,
	ETC1,
	ETC2,
	ETC3,
	ETC4,
	JUMP,
	DAMAGE_AIR,
	DAMAGE_KNOCKBACK_GROUND,
	DAMAGE_KNOCKBACK_FLY,
	DOWN,
	FALL,
	DEATH
};

UENUM(BlueprintType)
enum class EITEM_TYPE : uint8
{
	CONSUMABLE,
	END
};

UENUM(BlueprintType)
enum class EITEM_ID : uint8
{
	HP_POTION,
	END
};