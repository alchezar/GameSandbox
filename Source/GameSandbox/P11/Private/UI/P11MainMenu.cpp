// Copyright (C) 2023, IKinder

#include "P11/Public/UI/P11MainMenu.h"

#include "Components/Button.h"
#include "Components/EditableTextBox.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"
#include "P11/Public/Game/P11GameInstance.h"
#include "P11/Public/Game/P11GameModeBase.h"
#include "P11/Public/Game/P11SavePlayerInfo.h"
#include "P11/Public/UI/P11CharUI.h"

// DEFINE_LOG_CATEGORY_STATIC(LogP11MainMenu, All, All)

void UP11MainMenu::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	SetIsFocusable(true);
}

void UP11MainMenu::NativeConstruct()
{
	Super::NativeConstruct();
	
	GameInstance = Cast<UP11GameInstance>(GetGameInstance());
	check(GameInstance)
	GameMode = Cast<AP11GameModeBase>(GetWorld()->GetAuthGameMode());
	check(GameMode);
	OnVisibilityChanged.AddDynamic(this, &ThisClass::OnVisibilityChangedHandle);

	/* Middle section */
	if (CreateBtn && SingleBtn && JoinBtn && QuitBtn && AddressBox)
	{
		CreateBtn->OnClicked.AddDynamic(this, &ThisClass::OnCreateHandle);
		SingleBtn->OnClicked.AddDynamic(this, &ThisClass::OnSingleHandle);
		JoinBtn->OnClicked.AddDynamic(this, &ThisClass::OnJoinHandle);
		QuitBtn->OnClicked.AddDynamic(this, &ThisClass::OnQuitHandle);
	}
	
	/* Right section */
	if (JediBtn && SythBtn && ApplyBtn && NameBox)
	{
		JediBtn->OnClicked.AddDynamic(this, &ThisClass::OnJediHandle);
		SythBtn->OnClicked.AddDynamic(this, &ThisClass::OnSythHandle);
		ApplyBtn->OnClicked.AddDynamic(this, &ThisClass::OnApplyHandle);
	}
}

void UP11MainMenu::OnVisibilityChangedHandle(ESlateVisibility InVisibility)
{
	/* Update saved params every time when widget is visible. */
	const auto SavedInfo = Cast<UP11SavePlayerInfo>(UGameplayStatics::LoadGameFromSlot(GameInstance->GetSlotName(), 0));
	if (SavedInfo && InVisibility == ESlateVisibility::Visible)
	{
		NameBox->SetText(SavedInfo->PlayerName);
		PlayerSide = SavedInfo->PlayerSide;
		ActivateButton(JediBtn, PlayerSide == EP11PlayerSide::Jedi);
		ActivateButton(SythBtn, PlayerSide == EP11PlayerSide::Syth);
	}

	/* Spawn Char preview when widget is visible.  */
	if (CharUIClass && InVisibility == ESlateVisibility::Visible)
	{
		const FTransform SpawnTransform = FTransform(FRotator(0.f, 0.f, -90.f), FVector(1000.f, 0.f, -500));
		CharUI = Cast<AP11CharUI>(GetWorld()->SpawnActor(CharUIClass, &SpawnTransform));
		check(CharUI)
		CharUI->ChoseSide(PlayerSide);
	}
	else if (CharUI)
	{
		CharUI->Destroy();
		CharUI = nullptr;
	}
}

void UP11MainMenu::OnCreateHandle()
{
	UGameplayStatics::OpenLevelBySoftObjectPtr(GetWorld(), GameMode->GetStartupMap(), true, "listen");
}

void UP11MainMenu::OnSingleHandle()
{
	UGameplayStatics::OpenLevelBySoftObjectPtr(GetWorld(), GameMode->GetStartupMap());
}

void UP11MainMenu::OnJoinHandle()
{
	const FName LevelAddress = FName(AddressBox->GetText().ToString());
	UGameplayStatics::OpenLevel(GetWorld(), LevelAddress);
}

void UP11MainMenu::OnQuitHandle()
{
	if (APlayerController* PlayerController = GetOwningPlayer())
	{
		PlayerController->ConsoleCommand("quit");
	}
}

void UP11MainMenu::OnJediHandle()
{
	PlayerSide = EP11PlayerSide::Jedi;
	CharUI->ChoseSide(PlayerSide);
	ActivateButton(JediBtn, true);
	ActivateButton(SythBtn, false);
}

void UP11MainMenu::OnSythHandle()
{
	PlayerSide = EP11PlayerSide::Syth;
	CharUI->ChoseSide(PlayerSide);
	ActivateButton(SythBtn, true);
	ActivateButton(JediBtn, false);
}

void UP11MainMenu::OnApplyHandle()
{
	if (UP11SavePlayerInfo* SavePlayerInfo = GameInstance->GetSavePlayerInfo())
	{
		SavePlayerInfo->PlayerName = NameBox->GetText();
		SavePlayerInfo->PlayerSide = PlayerSide;
		UGameplayStatics::SaveGameToSlot(SavePlayerInfo, GameInstance->GetSlotName(), 0);
	}
}

void UP11MainMenu::ActivateButton(UButton* Button, const bool bActive)
{
	/* Pick all colors variations. */
	const FLinearColor ColorBackground = bActive ? FLinearColor::Black : FLinearColor::Yellow;
	const FLinearColor ColorForeground = bActive ?  FLinearColor::Yellow : FLinearColor::Black;

	/* Set background and outline colors depending on bActive. */
	FButtonStyle ButtonStyle = Button->GetStyle();
	ButtonStyle.Normal.OutlineSettings.Color  = ColorForeground;
	ButtonStyle.Hovered.OutlineSettings.Color = ColorForeground;
	ButtonStyle.Pressed.OutlineSettings.Color = ColorForeground;
	Button->SetStyle(ButtonStyle);
	Button->SetBackgroundColor(ColorBackground);

	/* All buttons have text block as a child slot. So we can find it without binding. */
	if (UTextBlock* ButtonText = Cast<UTextBlock>(Button->GetSlots()[0]->Content))
	{
		ButtonText->SetColorAndOpacity(ColorForeground);
	}
}
