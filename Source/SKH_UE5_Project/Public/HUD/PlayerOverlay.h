#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerOverlay.generated.h"

UCLASS()
class SKH_UE5_PROJECT_API UPlayerOverlay : public UUserWidget
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadOnly)
	bool IsHumanType;

	void SetHealthBarPercent(float Percent);
	void SetStaminaBarPercent(float Percent);
	void SetChangeBarPercent(float Percent);
	void SetGold(int32 Gold);
	void SetSouls(int32 Souls);
	void SetChangeProgressOpacity();
	void SetSkillBar1Percent(float Percent);
	void SetSkillBar2Percent(float Percent);
	void SetSkillBar3Percent(float Percent);
	void SetSkillBar4Percent(float Percent);
	void SetSkillBar5Percent(float Percent);
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void SetNum1On();
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void SetNum1Off();
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void SetNum2On();
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void SetNum2Off();
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void SetNum3On();
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void SetNum3Off();
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void SetNum4On();
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void SetNum4Off();
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void SetNum5On();
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void SetNum5Off();
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void OverlaySetHuman();
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void OverlaySetDragon();

	// ¹Ì»ç¿ë
	void SetOverlayHide();
	void SetOverlayShow();

	UFUNCTION(BlueprintImplementableEvent , BlueprintCallable)
	void SetSwordSkill();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void SetMagicSkill();

private:
	UPROPERTY(meta = (BindWidget))
	class UProgressBar* HealthProgressBar;

	UPROPERTY(meta = (BindWidget))
	class UProgressBar* StaminaProgressbar;

	UPROPERTY(meta = (BindWidget))
	class UProgressBar* ChangeProgressBar;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* GoldText;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* SoulsText;

	UPROPERTY(meta = (BindWidget))
	class UImage* OverlayCrack;

	UPROPERTY(meta = (BindWidget))
	class UProgressBar* SkillProgressBar1;

	UPROPERTY(meta = (BindWidget))
	class UProgressBar* SkillProgressBar2;

	UPROPERTY(meta = (BindWidget))
	class UProgressBar* SkillProgressBar3;

	UPROPERTY(meta = (BindWidget))
	class UProgressBar* SkillProgressBar4;

	UPROPERTY(meta = (BindWidget))
	class UProgressBar* SkillProgressBar5;

};
