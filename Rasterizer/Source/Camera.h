#pragma once

#include <Core/Time.h>
#include <Input/Event.h>
#include <Input/MouseEvent.h>

#include <Math/Matrix.h>
#include <Math/Vector.h>


namespace Raster
{
	class OrthographicCamera
	{
	public:
		OrthographicCamera() = default;
		OrthographicCamera(float left, float right, float bottom, float top, bool rotation = false);
		OrthographicCamera(float aspectRatio, bool rotation = false);

		Matrix4 GetViewProjectionMatrix() { return m_ViewMatrix * m_ProjectionMatrix; }

		void SetProjection(float left, float right, float bottom, float top);

		void EnableRotation(bool enable) { m_EnableRotation = enable; }

		Vector3 GetPosition() const { return m_Position; }
		inline float GetRotation() const { return m_Rotation; }

		inline void SetPosition(const Vector3& position) { m_Position = position; RecalculateView(); }
		inline void SetRotation(float rotation) { m_Rotation = rotation; RecalculateView(); }

		void OnUpdate(Core::Time frameTime);
		void OnEvent(Core::Event& event);

		void SetViewportSize(uint32 width, uint32 height);

		inline void SetZoomLevel(float level) { m_ZoomLevel = level; RecalculateProjection(); }
		inline float GetZoomLevel() const { return m_ZoomLevel; }

		inline void SetCameraSpeed(float speed) { m_CameraSpeed = speed; }
		inline void SetCameraRotationSpeed(float speed) { m_CameraRotationSpeed = speed; }

	private:
		void RecalculateProjection();
		void RecalculateView();

		bool OnMouseScrolled(Core::MouseScrolledEvent& event);

	private:
		Matrix4 m_ViewMatrix;
		Matrix4 m_ProjectionMatrix;

		float m_NearClip;
		float m_FarClip;

		Vector3 m_Position = Vector3(0);
		float m_Rotation = 0;
		bool m_EnableRotation = false;

		float m_AspectRatio = 1.778f;
		float m_ZoomLevel = 1.f;

		float m_CameraSpeed = 3.f;
		float m_CameraRotationSpeed = 2.f;
	};
}
