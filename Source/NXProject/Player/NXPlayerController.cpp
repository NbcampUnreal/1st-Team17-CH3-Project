#include "Player/NXPlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "Blueprint/UserWidget.h"
#include "Player/NXPlayerCharacter.h"

ANXPlayerController::ANXPlayerController()
	:InputMappingContext(nullptr),
    MoveAction(nullptr),
    JumpAction(nullptr),
    LookAction(nullptr),
    SprintAction(nullptr),
    InteractAction(nullptr),
    CrouchAction(nullptr),
    UnCrouchAction(nullptr)
    
{

}

void ANXPlayerController::BeginPlay()
{
    Super::BeginPlay();



    // ���� PlayerController�� ����� Local Player ��ü�� ������    
    if (ULocalPlayer* LocalPlayer = GetLocalPlayer())
    {
        // Local Player���� EnhancedInputLocalPlayerSubsystem�� ȹ��
        if (UEnhancedInputLocalPlayerSubsystem* Subsystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
        {
            if (InputMappingContext)
            {
                // Subsystem�� ���� �츮�� �Ҵ��� IMC�� Ȱ��ȭ
                // �켱����(Priority)�� 0�� ���� ���� �켱����
                Subsystem->AddMappingContext(InputMappingContext, 0);
            }
        }
    }

    if (HUDWidgetClass)
    {
        UUserWidget* HUDWidget = CreateWidget<UUserWidget>(this, HUDWidgetClass);
        if (HUDWidget)
        {
            HUDWidget->AddToViewport();

          
            ANXPlayerCharacter* PlayerCharacter = Cast<ANXPlayerCharacter>(GetPawn());
            if (PlayerCharacter)
            {
                PlayerCharacter->SetHUDWidget(HUDWidget);
            }
        }
    }
}
