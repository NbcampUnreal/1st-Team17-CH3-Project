#include "MyUserWidget.h"
#include "Components/TextBlock.h"
#include "TimerManager.h"

void UMyUserWidget::NativeConstruct()
{
    Super::NativeConstruct();
    CurrentText = TEXT("");
}

void UMyUserWidget::StartTypingEffect(const FString& FullText, float Delay)
{
    if (!TextBlock) return;

    CurrentText = TEXT("");
    OriginalText = FullText;
    CurrentIndex = 0;

    GetWorld()->GetTimerManager().SetTimer(TypingTimerHandle, this, &UMyUserWidget::UpdateTypingText, Delay, true);
}

void UMyUserWidget::UpdateTypingText()
{
    if (CurrentIndex < OriginalText.Len())
    {
        CurrentText.AppendChar(OriginalText[CurrentIndex]);
        TextBlock->SetText(FText::FromString(CurrentText));

        CurrentIndex++;
    }
    else
    {
        GetWorld()->GetTimerManager().ClearTimer(TypingTimerHandle);
    }
}
