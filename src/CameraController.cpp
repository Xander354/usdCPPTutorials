#include "CameraController.h"
#include <GLFW/glfw3.h>
#include <pxr/base/gf/vec3d.h>
#include <pxr/base/gf/quatd.h>

#include <pxr/usd/usd/variantSets.h>
#include <pxr/usd/usd/editContext.h>
#include <pxr/usd/usd/primRange.h>
#include <pxr/usd/usdGeom/camera.h>
#include <pxr/usd/usdGeom/xformCommonAPI.h>
#include <pxr/usd/usdGeom/xform.h>
#include <pxr/usd/usdGeom/sphere.h>
#include <pxr/usd/usdGeom/metrics.h>
#include <pxr/usd/usdGeom/tokens.h>
#include <pxr/usd/usdGeom/mesh.h>
#include "pxr/imaging/glf/simpleLightingContext.h"
#include <pxr/usd/usd/modelAPI.h>
#include <pxr/usd/kind/registry.h>
#include <pxr/usd/kind/api.h>
#include <pxr/usd/usdGeom/primvar.h>
#include <pxr/usd/usdGeom/primvarsAPI.h>
#include <pxr/usd/usdShade/api.h>
#include <pxr/usd/usdShade/material.h>
#include <pxr/usd/usdShade/materialBindingAPI.h>

PXR_NAMESPACE_USING_DIRECTIVE

GfVec3d eulerFromQuat(double x, double y, double z, double w) {
    double t0 = +2.0 * (w * x + y * z);
    double t1 = +1.0 - 2.0 * (x * x + y * y);
    double roll_x = atan2(t0, t1);

    double t2 = +2.0 * (w * y - z * x);
    t2 = t2 > 1.0 ? 1.0 : t2;
    t2 = t2 < -1.0 ? -1.0 : t2;
    double pitch_y = asin(t2);

    double t3 = +2.0 * (w * z + x * y);
    double t4 = +1.0 - 2.0 * (y * y + z * z);
    double yaw_z = atan2(t3, t4);

    return { roll_x, pitch_y, yaw_z }; //in radians

}

GfQuatd quatFromEuler(double roll, double pitch, double yaw) {
    float cr, cp, cy, sr, sp, sy, cpcy, spsy;
    // calculate trig identities
    cr = cos(roll / 2);
    cp = cos(pitch / 2);
    cy = cos(yaw / 2);
    sr = sin(roll / 2);
    sp = sin(pitch / 2);
    sy = sin(yaw / 2);
    cpcy = cp * cy;
    spsy = sp * sy;
    const double w = cr * cpcy + sr * spsy;
    const double x = sr * cpcy - cr * spsy;
    const double y = cr * sp * cy + sr * cp * sy;
    const double z = cr * cp * sy - sr * sp * cy;
    GfQuatd res(w, { x, y, z });
    return res;
}

CameraController::CameraController(pxr::GfCamera* camera, CameraParams camParams) : 
    mCamera(camera), 
    mUpAxis(camParams.upAxis), 
    cameraMoveSpeed(camParams.movementSpeed), 
    cameraRotateSpeed(camParams.rotateSpeed)
{
    setCameraTranslation(camParams.camPos);
    setCameraEulerAngles(camParams.camRot);
}

void CameraController::keypress(int key, int scancode, int action)
{
	//This is horrible, but I'm lazy
	if (action == GLFW_PRESS) {
		keyDownMap[key] = true;
	}
	else if (action == GLFW_RELEASE) {
		keyDownMap[key] = false;
	}
}

void CameraController::update(float dt)
{
    if (keyPressed(GLFW_KEY_W)) {
        applyCameraMovement(FORWARD, dt);
    }
    if (keyPressed(GLFW_KEY_S)) {
        applyCameraMovement(BACKWARD, dt);
    }
    if (keyPressed(GLFW_KEY_A)) {
        applyCameraMovement(LEFT, dt);
    }
    if (keyPressed(GLFW_KEY_D)) {
        applyCameraMovement(RIGHT, dt);
    }
    if (keyPressed(GLFW_KEY_SPACE)) {
        applyCameraMovement(UP, dt);
    }
    if (keyPressed(GLFW_KEY_LEFT_SHIFT)) {
        applyCameraMovement(DOWN, dt);
    }

    if (keyPressed(GLFW_KEY_UP)) {
        applyCameraRotation({ cameraRotateSpeed, 0, 0 }, dt);
    }
    if (keyPressed(GLFW_KEY_DOWN)) {
        applyCameraRotation({ -cameraRotateSpeed, 0, 0 }, dt);
    }
    if (keyPressed(GLFW_KEY_RIGHT)) {
        applyCameraRotation({ 0, -cameraRotateSpeed, 0 }, dt);
    }
    if (keyPressed(GLFW_KEY_LEFT)) {
        applyCameraRotation({ 0, cameraRotateSpeed, 0 }, dt);
    }
}

bool CameraController::keyPressed(int keycode)
{
    return keyDownMap.contains(keycode) && keyDownMap[keycode];
}

void CameraController::applyCameraMovement(CameraMovement direction, float dt)
{
    GfVec3d Position = mTranslation;
    const double* rot = mEulerRot.data();
    const double pitch = rot[0];
    const double yaw = rot[1];
    const double x = cos(yaw) * cos(pitch);
    const double z = sin(yaw) * cos(pitch);
    const double y = -sin(pitch);
    GfVec3d rightVec(-x, 0, z);
    GfVec3d frontVec(z, keyPressed(GLFW_KEY_LEFT_CONTROL) ? 0 : y, x);
    const float speed = cameraMoveSpeed * dt;

    if (direction == RIGHT)
       setCameraTranslation(Position - rightVec * speed);
    if (direction == LEFT)
        setCameraTranslation(Position + rightVec * speed);
    if (direction == FORWARD)
        setCameraTranslation(Position - frontVec * speed);
    if (direction == BACKWARD)
        setCameraTranslation(Position + frontVec * speed);
    if (direction == UP)
        setCameraTranslation(Position + GfVec3d(0, 1, 0) * speed);
    if (direction == DOWN)
        setCameraTranslation(Position - GfVec3d(0, 1, 0) * speed);
}

void CameraController::applyCameraRotation(pxr::GfVec3d delta, float dt)
{
    mEulerRot += delta * dt;
    setCameraEulerAngles(mEulerRot);
}

void CameraController::setCameraEulerAngles(pxr::GfVec3d rotation)
{
    mEulerRot = rotation;
    auto transform = mCamera->GetTransform();
    double* eData = rotation.data();

    if (mUpAxis == UsdGeomTokens->z) {
        eData[0] += M_PI / 2;
        double temp = eData[1];
        eData[1] = eData[2];
        eData[2] = temp;
    }

    GfQuatd newQ = quatFromEuler(eData[0], eData[1], eData[2]);
    transform.SetRotateOnly(newQ);
    mCamera->SetTransform(transform);
}

void CameraController::setCameraTranslation(pxr::GfVec3d translation)
{
    GfMatrix4d transform = mCamera->GetTransform();
    mTranslation = translation;

    if (mUpAxis == UsdGeomTokens->z) {
        double* tData = translation.data();
        double temp = translation[2];
        tData[2] = tData[1];
        tData[1] = -temp;
        translation.Set(tData);
    }

    transform.SetTranslateOnly(translation);
    mCamera->SetTransform(transform);
}
