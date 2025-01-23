// Copyright (C) 2023, IKinder

#include "UI/Chat/P11ChatBox.h"

#include "Components/Button.h"
#include "Components/EditableTextBox.h"
#include "Components/WidgetSwitcher.h"
#include "Player/P11PlayerController.h"
#include "Player/P11PlayerState.h"
#include "UI/Chat/P11Chat.h"

DEFINE_LOG_CATEGORY_STATIC(LogP11ChatBox, All, All);

void UP11ChatBox::NativeConstruct()
{
	Super::NativeConstruct();
	MessageBox->OnTextCommitted.AddDynamic(this, &ThisClass::OnMessageCommittedHandle);
	Controller = Cast<AP11PlayerController>(GetOwningPlayer());
	if (!Controller)
	{
		UE_LOG(LogP11ChatBox, Warning, TEXT("Player controller was not found!"));
		return;
	}
	SendBtn->OnPressed.AddDynamic(this, &ThisClass::OnSendPressedHandle);
	ExtendChat(false);
}

int32 UP11ChatBox::GetSwitcherIndex() const
{
	return ChatSwitcher->GetActiveWidgetIndex();
}

void UP11ChatBox::ExtendChat(const bool bExtended)
{
	ChatSwitcher->SetActiveWidgetIndex(bExtended ? 1 : 0);

	/* Get sender name from player state. */
	const AP11PlayerState* PlayerState = Controller->GetPlayerState<AP11PlayerState>();
	if (!PlayerState)
	{
		UE_LOG(LogP11ChatBox, Warning, TEXT("Player state was not found!"));
		return;
	}
	Sender = PlayerState->GetThePlayerName().ToString();

}

void UP11ChatBox::OnMessageCommittedHandle(const FText& Text, ETextCommit::Type CommitMethod)
{
	if (CommitMethod == ETextCommit::OnEnter)
	{
		if (Text.IsEmpty())
		{
			return;
		}
		if (!Controller)
		{
			return;
		}
		Controller->Server_SendToPlayerControllerGameState({Sender, Text.ToString(), EP11MessageState::Player});
		/* Hide chat. */
		MessageBox->SetText(FText::FromString(""));
		ChatSwitcher->SetActiveWidgetIndex(0);
		Controller->AfterCloseChat();
	}
	if (CommitMethod == ETextCommit::OnCleared)
	{
		/* Don`t hide chat. */
		MessageBox->SetText(FText::FromString(""));
	}
}

void UP11ChatBox::OnSendPressedHandle()
{
	const FText Message = MessageBox->GetText();
	if (!Message.IsEmpty())
	{
		Controller->Server_SendToPlayerControllerGameMode({Sender, Message.ToString(), EP11MessageState::Player});
		MessageBox->SetText(FText::FromString(""));
	}
}
