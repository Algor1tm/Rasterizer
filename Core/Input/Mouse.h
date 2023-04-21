#pragma once

#include "Core/Core.h"


namespace Core
{
	namespace Mouse
	{
		enum Button : uint16
		{
			XButton1 = 0x05,
			XButton2 = 0x06,

			Left = 0x01,
			Right = 0x02,
			Middle = 0x04
		};
	}
}
