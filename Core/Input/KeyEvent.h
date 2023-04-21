#pragma once

#include "Input/Event.h"
#include "Input/Keyboard.h"

#include <sstream>


namespace Core
{
	class KeyEvent: public Event
	{
	public:
		inline Keyboard::Key GetKeyCode() const { return m_KeyCode; }

		EVENT_CLASS_CATEGORY(EventCategoryKeyboard | EventCategoryInput)

	protected:
		KeyEvent(Keyboard::Key keycode)
			: m_KeyCode(keycode) {}

		Keyboard::Key m_KeyCode;
	};


	class KeyPressedEvent: public KeyEvent
	{
	public:
		KeyPressedEvent(Keyboard::Key keycode, bool isRepeat)
			: KeyEvent(keycode), m_IsRepeat(isRepeat) {}

		inline bool IsRepeat() const { return m_IsRepeat; }

		virtual String ToString() const override
		{
			std::stringstream stream;
			stream << "KeyPressedEvent: " << m_KeyCode << " (Is repeat - " << m_IsRepeat << ")";
			return stream.str();
		}

		EVENT_CLASS_TYPE(KeyPressed)

	private:
		bool m_IsRepeat;
	};


	class KeyReleasedEvent : public KeyEvent
	{
	public:
		KeyReleasedEvent(Keyboard::Key keycode)
			: KeyEvent(keycode) {}

		virtual String ToString() const override
		{
			std::stringstream stream;
			stream << "KeyReleasedEvent: " << m_KeyCode;
			return stream.str();
		}

		EVENT_CLASS_TYPE(KeyReleased)
	};


	class KeyTypedEvent : public KeyEvent
	{
	public:
		KeyTypedEvent(Keyboard::Key keycode)
			: KeyEvent(keycode) {}


		virtual String ToString() const override
		{
			std::stringstream stream;
			stream << "KeyTypedEvent: " << m_KeyCode;
			return stream.str();
		}

		EVENT_CLASS_TYPE(KeyTyped)
	};
}
