#pragma once

#include "Core/Core.h"

#include "Input/Keyboard.h"
#include "Input/Mouse.h"

#include "Math/Vector.h"


namespace Core
{
	class Input
	{
	public:
		static bool IsKeyPressed(Keyboard::Key keycode);
		static bool IsMouseButtonPressed(Mouse::Button button);
		static Vector2 GetMousePosition();
	};
}
