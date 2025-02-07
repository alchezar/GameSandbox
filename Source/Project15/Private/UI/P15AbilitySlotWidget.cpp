// Copyright © 2025, Ivan Kinder

#include "UI/P15AbilitySlotWidget.h"

#include "Project15.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Gameplay/Abilities/P15BaseAbility.h"

void UP15AbilitySlotWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	DynamicMaterial = AbilityImage->GetDynamicMaterial();
	check(DynamicMaterial)
}

void UP15AbilitySlotWidget::NativeConstruct()
{
	Super::NativeConstruct();

	EARLY_RETURN_IF(!DynamicMaterial || !AbilityTexture)
	DynamicMaterial->SetTextureParameterValue(TextureParameterName, AbilityTexture.Get());
}

void UP15AbilitySlotWidget::NativeTick(const FGeometry& MyGeometry, const float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	UpdateCooldownSmoothly(InDeltaTime);
}

void UP15AbilitySlotWidget::SetCooldown(const float NewCooldown)
{
	MaxCooldown = FMath::Max(NewCooldown, 1.f);
	UpdateCooldown(NewCooldown);
}

void UP15AbilitySlotWidget::UpdateCooldown(const float NewCooldown) const
{
	CooldownText->SetText(FText::AsNumber(NewCooldown));
	DynamicMaterial->SetScalarParameterValue(AlphaParameterName, NewCooldown / MaxCooldown);
}

void UP15AbilitySlotWidget::SetAbilityInfo(FP15AbilityInfo&& AbilityInfo)
{
	EARLY_RETURN_IF(!DynamicMaterial)

	// Update slot appearance.
	const bool bCost = AbilityInfo.Cost < 0.f;
	if (bCost)
	{
		CostText->SetText(FText::AsNumber(-AbilityInfo.Cost));
		CostText->SetColorAndOpacity(GetCostColor(AbilityInfo.CostType));
	}
	CostText->SetVisibility(bCost ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);

	MaxCooldown = AbilityInfo.CooldownDuration;
	CooldownText->SetVisibility(ESlateVisibility::Collapsed);
	DynamicMaterial->SetTextureParameterValue(TextureParameterName, AbilityInfo.Texture);
	AbilityClass = AbilityInfo.AbilityClass;
}

void UP15AbilitySlotWidget::OnAbilityStarted()
{
	EARLY_RETURN_IF(bActive || MaxCooldown <= 0.f)

	Cooldown = 0;
	bActive  = true;
	CooldownText->SetVisibility(ESlateVisibility::Visible);
}

FSlateColor UP15AbilitySlotWidget::GetCostColor(const EP15AbilityCostType CostType) const
{
	FColor Result = FColor::Black;

	// clang-format off
	static TMap<EP15AbilityCostType, FColor> ColorMap = {
		{EP15AbilityCostType::Health,   FColor::Red},
		{EP15AbilityCostType::Mana,     FColor::Blue},
		{EP15AbilityCostType::Strength, FColor::Green}
	};
	// clang-format on

	if (const FColor* Found = ColorMap.Find(CostType))
	{
		Result = *Found;
	}

	Result.A = 255 * 0.75f;
	return Result;
}

void UP15AbilitySlotWidget::UpdateCooldownSmoothly(const float DeltaTime)
{
	EARLY_RETURN_IF(!bActive)
	float Alpha = FMath::Min(Cooldown / MaxCooldown, 1.f);
	if (Alpha >= 1.f)
	{
		bActive  = false;
		Alpha    = 1.f;
		Cooldown = 0.f;
		CooldownText->SetVisibility(ESlateVisibility::Collapsed);
	}

	DynamicMaterial->SetScalarParameterValue(AlphaParameterName, Alpha);
	CooldownText->SetText(FText::AsNumber(FMath::Floor(MaxCooldown - Cooldown)));
	Cooldown += DeltaTime;
}
