// Copyright (C) 2023, IKinder

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "P11ChatOnScreen.generated.h"

class UP11ChatBox;

UCLASS()
class PROJECT11_API UP11ChatOnScreen : public UUserWidget
{
	GENERATED_BODY()

public:
	FORCEINLINE UP11ChatBox* GetChatBox() const { return ChatBox; }
	void ExtendChat(const bool bExtended) const;
	
protected:
	virtual void NativeConstruct() override;

protected:
	UPROPERTY(meta = (BindWidget))
	UP11ChatBox* ChatBox;
};
