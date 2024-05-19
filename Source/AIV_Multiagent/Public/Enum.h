#pragma once

UENUM(BlueprintType)
enum class E_AIType : uint8
{
	REGULAR		UMETA(DisplayName = "Regular"),
	ELITE		UMETA(DisplayName = "Elite")
};

UENUM(BlueprintType)
enum class E_AISense : uint8
{
	NONE		UMETA(DisplayName = "None"),
	SIGHT		UMETA(DisplayName = "Sight"),
	HEARING		UMETA(DisplayName = "Hearing"),
	TOUCH       UMETA(DisplayName = "Touch"),
};

UENUM(BlueprintType)
enum class E_AISightCone : uint8
{
	NONE		UMETA(DisplayName = "None"),
	NOTSEEN		UMETA(DisplayName = "Not Seen"),
	BACKWARD	UMETA(DisplayName = "Backward"),
	PERIPHERAL	UMETA(DisplayName = "Peripheral"),
	WIDE		UMETA(DisplayName = "Wide"),
	NARROW      UMETA(DisplayName = "Narrow")
};

UENUM(BlueprintType)
enum class E_AITag : uint8
{
	UNAWARE		UMETA(DisplayName = "Unaware"),
	SUSPICIOUS	UMETA(DisplayName = "Suspicious"),
	ALERTED		UMETA(DisplayName = "Alerted"),
	HUNTING		UMETA(DisplayName = "Hunting")
};

UENUM(BlueprintType)
enum class E_AISpeed : uint8
{
	WALK	UMETA(DisplayName = "Walk"),
	RUN		UMETA(DisplayName = "Run")
};
