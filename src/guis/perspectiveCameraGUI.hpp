#pragma once

class PerspectiveCamera;

class PerspectiveCameraGUI
{
public:
	PerspectiveCameraGUI(PerspectiveCamera& camera);
	void update();

private:
	PerspectiveCamera& m_camera;
};
