#include "NXGameMode.h"
#include "Kismet/GameplayStatics.h"

void ANXGameMode::ShowGameOverUI()
{
    if (GameOverWidgetClass)
    {
        CurrentGameOverWidget = CreateWidget<UUserWidget>(GetWorld(), GameOverWidgetClass);
        if (CurrentGameOverWidget)
        {
            CurrentGameOverWidget->AddToViewport();

            // 플레이어 입력을 UI 모드로 변경 (예시)
            APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
            if (PC)
            {
                FInputModeUIOnly InputMode;
                InputMode.SetWidgetToFocus(CurrentGameOverWidget->TakeWidget());
                PC->SetInputMode(InputMode);
                PC->bShowMouseCursor = true;
            }
        }
    }
}
