#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "Blueprint/UserWidget.h" // UUserWidget ����
#include "NXGameMode.generated.h"

UCLASS()
class NXPROJECT_API ANXGameMode : public AGameMode
{
    GENERATED_BODY()

public:
    // ���ӿ��� UI �������Ʈ Ŭ���� ���� (�����Ϳ��� �Ҵ� ����)
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI")
    TSubclassOf<UUserWidget> GameOverWidgetClass;

    // ���ӿ��� UI �ν��Ͻ��� ������ ����
    UPROPERTY()
    UUserWidget* CurrentGameOverWidget;

    // ���ӿ��� UI�� ���� �� ǥ���ϴ� �Լ�
    UFUNCTION(BlueprintCallable, Category = "UI")
    void ShowGameOverUI();
};
