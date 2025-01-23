// Copyright (C) 2023, IKinder

#include "Player/P11PlayerController.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Components/EditableTextBox.h"
#include "Game/P11GameInstance.h"
#include "Game/P11GameModeBase.h"
#include "Game/P11GameState.h"
#include "Game/P11SavePlayerInfo.h"
#include "Kismet/GameplayStatics.h"
#include "Player/P11Character.h"
#include "Player/P11PlayerState.h"
#include "UI/P11HUD.h"
#include "UI/Chat/P11ChatBox.h"
#include "UI/Chat/P11ChatOnScreen.h"

// DEFINE_LOG_CATEGORY_STATIC(LogP11PlayerController, All, All)

void AP11PlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	if (AP11PlayerState* ThisPlayerState = GetPlayerState<AP11PlayerState>())
	{
		ThisPlayerState->SetIsAlive();
	}
	if (HasAuthority())
	{
		Client_ChangeSide();
	}
}

void AP11PlayerController::BeginPlay()
{
	Super::BeginPlay();
	SetInputMode(FInputModeGameOnly());

	if (auto* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		Subsystem->AddMappingContext(DefaultContext, 0);
	}
	HUD = Cast<AP11HUD>(GetHUD());
}

void AP11PlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	if (!InputComponent)
	{
		return;
	}
	if (UEnhancedInputComponent* EnhancedInput = CastChecked<UEnhancedInputComponent>(InputComponent))
	{
		EnhancedInput->BindAction(MainMenuAction, ETriggerEvent::Started, this, &ThisClass::MainMenuInput);
		EnhancedInput->BindAction(ScoreboardAction, ETriggerEvent::Started, this, &ThisClass::ScoreboardInput, true);
		EnhancedInput->BindAction(ScoreboardAction, ETriggerEvent::Completed, this, &ThisClass::ScoreboardInput, false);
		EnhancedInput->BindAction(ChatAction, ETriggerEvent::Started, this, &ThisClass::ChatInput);
	}
}

void AP11PlayerController::MainMenuInput()
{
	if (!HUD)
	{
		return;
	}
	bMenuVisible = !bMenuVisible;
	// SetPause(bMenuVisibility);
	HUD->ShowMainMenu(bMenuVisible);
	SetShowMouseCursor(bMenuVisible);

	FInputModeGameAndUI GameAndUIMode;
	GameAndUIMode.SetHideCursorDuringCapture(false);
	bMenuVisible ? SetInputMode(GameAndUIMode) : SetInputMode(FInputModeGameOnly());
	bMenuVisible ? GetPawn()->DisableInput(this) : GetPawn()->EnableInput(this);
}

void AP11PlayerController::SetCharSide(const EP11PlayerSide NewSide)
{
	Server_ChangeCharSide(NewSide);
}

void AP11PlayerController::Client_ChangeSide_Implementation()
{
	const UP11GameInstance* GameInstance = Cast<UP11GameInstance>(GetWorld()->GetGameInstance());
	if (!GameInstance)
	{
		return;
	}
	const UP11SavePlayerInfo* SavedInfo = Cast<UP11SavePlayerInfo>(UGameplayStatics::LoadGameFromSlot(GameInstance->GetSlotName(), 0));
	if (!SavedInfo)
	{
		return;
	}
	Server_ChangeCharSide(SavedInfo->PlayerSide);
}

void AP11PlayerController::Server_ChangeCharSide_Implementation(const EP11PlayerSide NewSide)
{
	ChangeCharSide(NewSide);
}

void AP11PlayerController::ChangeCharSide(const EP11PlayerSide NewSide) const
{
	if (AP11Character* Char = Cast<AP11Character>(GetPawn()))
	{
		Char->UpdateMeshOnServer(NewSide);
	}
	
}

void AP11PlayerController::ScoreboardInput(const bool bVisible)
{
	if (!HUD)
	{
		return;
	}
	HUD->ShowScore(bVisible);
}

void AP11PlayerController::ChatInput()
{
	if (!HUD)
	{
		return;
	}
	const UP11ChatOnScreen* ChatOnScreen = HUD->GetChatOnScreen();
	if (!ChatOnScreen)
	{
		return;
	}
	const UP11ChatBox* ChatBox = ChatOnScreen->GetChatBox();
	if (!ChatBox)
	{
		return;
	}
	UEditableTextBox* MessageBox = ChatBox->GetMessageBox();
	if (!MessageBox)
	{
		return;
	}
	APawn* ControlledPawn = GetPawn();
	if (!ControlledPawn)
	{
		return;
	}
	const int32 SwitcherIndex = ChatBox->GetSwitcherIndex();
	const bool bShowChat = SwitcherIndex == 0;
	if (bShowChat)
	{
		FInputModeGameAndUI GameAndUIMode;
		GameAndUIMode.SetHideCursorDuringCapture(false);
		GameAndUIMode.SetWidgetToFocus(MessageBox->TakeWidget());
		SetInputMode(GameAndUIMode);
		ControlledPawn->DisableInput(this);
	}
	else
	{
		AfterCloseChat();
	}
	HUD->ShowChat(bShowChat);
	SetShowMouseCursor(bShowChat);
}

void AP11PlayerController::Server_SendToPlayerControllerGameState_Implementation(const FP11MessageInfo& MessageInfo)
{
	AP11GameState* GameState = Cast<AP11GameState>(GetWorld()->GetGameState());
	if (!GameState)
	{
		return;
	}
	GameState->Multicast_MessageSendToGameState(MessageInfo);
}

void AP11PlayerController::Server_SendToPlayerControllerGameMode_Implementation(const FP11MessageInfo& MessageInfo)
{
	if (!GetWorld()->GetAuthGameMode())
	{
		return;
	}
	AP11GameModeBase* GameMode = Cast<AP11GameModeBase>(GetWorld()->GetAuthGameMode());
	if (!GameMode)
	{
		return;
	}
	GameMode->Server_SendToGameMode(MessageInfo);
}

void AP11PlayerController::AfterCloseChat()
{
	APawn* ControlledPawn = GetPawn();
	if (!ControlledPawn)
	{
		return;
	}
	bShowMouseCursor = false;
	SetInputMode(FInputModeGameOnly{});
	ControlledPawn->EnableInput(this);
}

void AP11PlayerController::Client_MessageSendToGameMode_Implementation(const FP11MessageInfo& MessageInfo)
{
	OnMessageSendToGameMode.Broadcast(MessageInfo);
}
