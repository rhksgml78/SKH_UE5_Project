#include "HUD/PlayerOverlay.h"
#include "Components/ProgressBar.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"

void UPlayerOverlay::SetHealthBarPercent(float Percent)
{
	if (HealthProgressBar)
	{
		HealthProgressBar->SetPercent(Percent);
	}
	if (OverlayCrack)
	{
		FLinearColor Color = FLinearColor(1.f, 1.f, 1.f, (1.f - Percent));
		OverlayCrack->SetColorAndOpacity(Color);
	}
}

void UPlayerOverlay::SetStaminaBarPercent(float Percent)
{
	if (StaminaProgressbar)
	{
		StaminaProgressbar->SetPercent(Percent);
	}
}

void UPlayerOverlay::SetChangeBarPercent(float Percent)
{
	if (ChangeProgressBar)
	{
		ChangeProgressBar->SetPercent(Percent);
	}
}

void UPlayerOverlay::SetGold(int32 Gold)
{
	if (GoldText)
	{
		GoldText->SetText(FText::FromString(FString::Printf(TEXT("%d"), Gold)));
	}
}

void UPlayerOverlay::SetSouls(int32 Souls)
{
	if (SoulsText)
	{
		SoulsText->SetText(FText::FromString(FString::Printf(TEXT("%d"), Souls)));
	}
}

void UPlayerOverlay::SetChangeProgressOpacity()
{
	float RandomNum = FMath::RandRange(0.5f, 1.f);
	ChangeProgressBar->SetFillColorAndOpacity(FLinearColor(1.f, 0.4f, 0.f, RandomNum));
}

void UPlayerOverlay::SetSkillBar1Percent(float Percent)
{
	if (SkillProgressBar1)
	{
		SkillProgressBar1->SetPercent(Percent);
	}
}

void UPlayerOverlay::SetSkillBar2Percent(float Percent)
{
	if (SkillProgressBar2)
	{
		SkillProgressBar2->SetPercent(Percent);
	}
}

void UPlayerOverlay::SetSkillBar3Percent(float Percent)
{
	if (SkillProgressBar3)
	{
		SkillProgressBar3->SetPercent(Percent);
	}
}

void UPlayerOverlay::SetSkillBar4Percent(float Percent)
{
	if (SkillProgressBar4)
	{
		SkillProgressBar4->SetPercent(Percent);
	}
}

void UPlayerOverlay::SetSkillBar5Percent(float Percent)
{
	if (SkillProgressBar5)
	{
		SkillProgressBar5->SetPercent(Percent);
	}
}

void UPlayerOverlay::SetOverlayHide()
{
	this->SetVisibility(ESlateVisibility::Hidden);
}

void UPlayerOverlay::SetOverlayShow()
{
	this->SetVisibility(ESlateVisibility::Visible);
}
