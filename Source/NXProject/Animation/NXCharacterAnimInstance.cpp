#include "Animation/NXCharacterAnimInstance.h"

void UNXCharacterAnimInstance::AnimNotify_CheckHit()
{
	if (OnCheckHit.IsBound() == true) // �ش� ��������Ʈ�� 1���� �Լ��� ���ε� �Ǿ� �ִٸ� true�� ��ȯ�ϴ� IsBound() �Լ� 
	{
		OnCheckHit.Broadcast();
	}

}
