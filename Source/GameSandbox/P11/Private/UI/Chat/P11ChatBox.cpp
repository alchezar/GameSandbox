// Copyright (C) 2023, IKinder

#include "P11/Public/UI/Chat/P11ChatBox.h"

#include "Components/EditableTextBox.h"
#include "Components/WidgetSwitcher.h"
#include "P11/Public/Player/P11PlayerController.h"
#include "P11/Public/Player/P11PlayerState.h"

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
		Controller->Server_SendToPlayerControllerGameState(Sender, Text.ToString());
		MessageBox->SetText(FText::FromString(""));
	}
}
