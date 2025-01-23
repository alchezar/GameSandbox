// Copyright © 2024, Ivan Kinder

#pragma once

#include "AutomationBlueprintFunctionLibrary.h"
#include "BufferVisualizationData.h"
#include "Utils/P14Utils.h"

namespace P14::Test
{
	/// @class FTakeScreenshotLatentCommand
	/// @brief Base class for taking screenshots.
	/// @detauls Extends the base class with the ability to subscribe for the screenshot delegate.
	class FTakeScreenshotLatentCommand : public IAutomationLatentCommand
	{
	public:
		explicit FTakeScreenshotLatentCommand(const FString& InScreenshotName) : ScreenshotName(InScreenshotName)
		{
			FAutomationTestFramework::Get().OnScreenshotTakenAndCompared.AddRaw(this, &FTakeScreenshotLatentCommand::OnScreenshotTakenAndCompared);
		}

		virtual ~FTakeScreenshotLatentCommand() override
		{
			FAutomationTestFramework::Get().OnScreenshotTakenAndCompared.RemoveAll(this);
		}

	protected:
		virtual void OnScreenshotTakenAndCompared()
		{
			bCommandCompleted = true;
		}

	protected:
		FString ScreenshotName;
		bool    bScreenshotRequested = false;
		bool    bCommandCompleted    = false;
	};

	/// @class FTakeGameScreenshotLatentCommand
	/// @brief Latent command for taking game screenshots.
	/// @detauls Extends the base class with the ability to take the screenshot of the game.
	class FTakeGameScreenshotLatentCommand : public FTakeScreenshotLatentCommand
	{
	public:
		explicit FTakeGameScreenshotLatentCommand(const FString& InScreenshotName) : FTakeScreenshotLatentCommand(InScreenshotName) {}

		virtual bool Update() override
		{
			// Do once.
			if (!bScreenshotRequested)
			{
				const FAutomationScreenshotOptions Options = UAutomationBlueprintFunctionLibrary::GetDefaultScreenshotOptionsForRendering();
				UAutomationBlueprintFunctionLibrary::TakeAutomationScreenshotInternal(GetTestGameWorld(), ScreenshotName, FString{}, Options);
				bScreenshotRequested = true;
			}

			return bCommandCompleted;
		}
	};

	/// @class FTakeUIScreenshotLatentCommand
	/// @brief Latent command for taking UI screenshots.
	/// @details Extends the base class with the ability to take the screenshot of the user interface.
	class FTakeUIScreenshotLatentCommand : public FTakeScreenshotLatentCommand
	{
	public:
		explicit FTakeUIScreenshotLatentCommand(const FString& InScreenshotName, const FName& InBufferName = NAME_None)
			: FTakeScreenshotLatentCommand(InScreenshotName)
			, BufferName(InBufferName) {}

		virtual bool Update() override
		{
			// Set buffer visualization and skip the first frame.
			if (!bScreenshotSetupDone)
			{
				bScreenshotSetupDone = true;
				SetBufferVisualization(BufferName);
				return false;
			}

			// Take screenshot of UI once.
			if (!bScreenshotRequested)
			{
				const FAutomationScreenshotOptions Options = UAutomationBlueprintFunctionLibrary::GetDefaultScreenshotOptionsForRendering();
				UAutomationBlueprintFunctionLibrary::TakeAutomationScreenshotOfUI_Immediate(GetTestGameWorld(), ScreenshotName, Options);
				bScreenshotRequested = true;
			}

			return bCommandCompleted;
		}

		virtual void OnScreenshotTakenAndCompared() override
		{
			FTakeScreenshotLatentCommand::OnScreenshotTakenAndCompared();
			SetBufferVisualization(NAME_None);
		}

	private:
		void SetBufferVisualization(const FName& VisualizeBuffer) const
		{
			UGameViewportClient*     ViewportClient = AutomationCommon::GetAnyGameViewportClient();
			static IConsoleVariable* IConsoleVar    = IConsoleManager::Get().FindConsoleVariable(FBufferVisualizationData::GetVisualizationTargetConsoleCommandName());
			if (!ViewportClient || !IConsoleVar)
			{
				return;
			}

			FEngineShowFlags* EngineShowFlags = ViewportClient->GetEngineShowFlags();
			if (!EngineShowFlags)
			{
				return;
			}

			EngineShowFlags->SetVisualizeBuffer(VisualizeBuffer != NAME_None);
			EngineShowFlags->SetTonemapper(VisualizeBuffer == NAME_None);
			IConsoleVar->Set(VisualizeBuffer == NAME_None ? TEXT("") : *VisualizeBuffer.ToString());
		}

	private:
		FName BufferName;
		bool  bScreenshotSetupDone = false;
	};
}
