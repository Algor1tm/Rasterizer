#include "Camera.h"

#include <Input/Input.h>

#include <Math/Projections.h>
#include <Math/Transforms.h>


namespace Raster
{
	OrthographicCamera::OrthographicCamera(float left, float right, float bottom, float top, bool rotation)
		: m_Position(0.f), m_Rotation(0.f), m_EnableRotation(rotation)
	{
		RecalculateView();
		RecalculateProjection();
	}

	OrthographicCamera::OrthographicCamera(float aspectRatio, bool rotation)
		: m_AspectRatio(aspectRatio), m_EnableRotation(rotation)
	{
		RecalculateView();
		RecalculateProjection();
	}

	void OrthographicCamera::SetProjection(float left, float right, float bottom, float top)
	{
		m_NearClip = -1.f;
		m_FarClip = 1.f;
		m_ProjectionMatrix = Math::Ortho(left, right, bottom, top, m_NearClip, m_FarClip);
	}

	void OrthographicCamera::RecalculateView()
	{
		Matrix4 transform = Math::RotateMatrix(m_Rotation, Vector3(0, 0, 1)) * Math::TranslateMatrix(m_Position);
		m_ViewMatrix = Math::AffineInverse(transform);
	}

	void OrthographicCamera::RecalculateProjection()
	{
		SetProjection(-m_ZoomLevel * m_AspectRatio, m_ZoomLevel * m_AspectRatio, -m_ZoomLevel, m_ZoomLevel);
	}

	void OrthographicCamera::OnUpdate(Core::Time frameTime)
	{
		float seconds = frameTime.AsSeconds();

		if (Core::Input::IsKeyPressed(Core::Keyboard::A))
			m_Position.x -= m_CameraSpeed * seconds;
		else if (Core::Input::IsKeyPressed(Core::Keyboard::D))
			m_Position.x += m_CameraSpeed * seconds;
		else if (Core::Input::IsKeyPressed(Core::Keyboard::W))
			m_Position.y += m_CameraSpeed * seconds;
		else if (Core::Input::IsKeyPressed(Core::Keyboard::S))
			m_Position.y -= m_CameraSpeed * seconds;

		if (m_EnableRotation)
		{
			if (Core::Input::IsKeyPressed(Core::Keyboard::Q))
				m_Rotation += m_CameraRotationSpeed * seconds;
			else if (Core::Input::IsKeyPressed(Core::Keyboard::E))
				m_Rotation -= m_CameraRotationSpeed * seconds;
		}

		RecalculateView();
	}

	void OrthographicCamera::OnEvent(Core::Event& event)
	{
		Core::EventDispatcher dispatcher(event);
		dispatcher.Dispatch<Core::MouseScrolledEvent>(APP_BIND_EVENT_FN(OrthographicCamera::OnMouseScrolled));
	}

	void OrthographicCamera::SetViewportSize(uint32 width, uint32 height)
	{
		m_AspectRatio = (float)width / (float)height;
		SetProjection(-m_ZoomLevel * m_AspectRatio, m_ZoomLevel * m_AspectRatio, -m_ZoomLevel, m_ZoomLevel);
	}

	bool OrthographicCamera::OnMouseScrolled(Core::MouseScrolledEvent& event)
	{
		m_ZoomLevel -= event.GetYOffset() * 0.4f;
		m_ZoomLevel = Math::Max(m_ZoomLevel, 0.2f);
		m_CameraSpeed = m_ZoomLevel * 1.5f;

		RecalculateProjection();

		return false;
	}
}
