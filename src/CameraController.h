#pragma once
#include <unordered_map>
#include <pxr/base/gf/camera.h>
#include <pxr/base/tf/token.h>
#include <pxr/base/gf/vec3d.h>

struct CameraParams
{
    pxr::GfVec3d camPos;
    pxr::GfVec3d camRot;
    float movementSpeed;
    float rotateSpeed;
    pxr::TfToken upAxis;
};

class Scene;

class CameraController
{
public:
	CameraController(pxr::GfCamera* camera, CameraParams camParams);
	static inline std::unordered_map<int, bool> keyDownMap;
	static void keypress(int key, int scancode, int action);

    void update(float dt);
    bool keyPressed(int keycode);

    enum CameraMovement {
        FORWARD,
        BACKWARD,
        LEFT,
        RIGHT,
        UP,
        DOWN,
    };

    void applyCameraMovement(CameraMovement direction, float dt);
    void applyCameraRotation(pxr::GfVec3d delta, float dt);
    void setCameraEulerAngles(pxr::GfVec3d rotation);
    void setCameraTranslation(pxr::GfVec3d translation);

private:

	pxr::GfCamera* mCamera;
    pxr::GfVec3d mEulerRot;
    pxr::GfVec3d mTranslation;
    float cameraMoveSpeed = 50;
    float cameraRotateSpeed = M_PI/5;
    pxr::TfToken mUpAxis;
    Scene* mScene;
};

