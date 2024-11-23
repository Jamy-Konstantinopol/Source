#pragma once

#include "CoreMinimal.h"

class CCLPAnimUtils
{
public:
	// clang-format off
	template <typename InAnimNotifyEventType>
	static InAnimNotifyEventType* FindNotifyEvent(UAnimSequenceBase* Animation)
	{
		if (!Animation)
		{
			return nullptr;
		}

		for (const FAnimNotifyEvent& InNotifyEvent : Animation->Notifies)
		{
			if (InAnimNotifyEventType* Notify = Cast<InAnimNotifyEventType>(InNotifyEvent.Notify))
			{
				return Notify;
			}
		}
		return nullptr;
	};
	// clang-format on
};
