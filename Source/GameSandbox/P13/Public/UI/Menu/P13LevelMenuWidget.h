// Copyright © 2024, IKinder

#pragma once

#include "CoreMinimal.h"
#include "P13MenuWidget.h"
#include "P13LevelMenuWidget.generated.h"

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                Level Menu                                 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

class UCircularThrobber;
class UCheckBox;

UCLASS()
class GAMESANDBOX_API UP13LevelMenuWidget : public UP13MenuWidget
{
	GENERATED_BODY()

	/* ------------------------------- Super ------------------------------- */
protected:
	virtual void NativeConstruct() override;

	/* ------------------------------- This -------------------------------- */
protected:
	
	/* ----------------------------- Variables ----------------------------- */
protected:
	UPROPERTY(meta = (BindWidget))
	UButton* HostButton;
	UPROPERTY(meta = (BindWidget))
	UButton* FindButton;
	UPROPERTY(meta = (BindWidget))
	UCheckBox* IsLanCheckBox;
	UPROPERTY(meta = (BindWidget))
	UCircularThrobber* SearchingThrobber;
	UPROPERTY(meta = (BindWidget))
	UHorizontalBox* ServersHorizontalBox;
};
