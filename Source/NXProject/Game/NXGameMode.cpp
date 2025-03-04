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

            // �÷��̾� �Է��� UI ���� ���� (����)
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
