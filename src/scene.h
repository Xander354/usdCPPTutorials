
#pragma once

#include <GL/glew.h>
#include <windows.h>
#include <GL/gl.h>
#include <pxr/usd/usd/stage.h>
#include <pxr/usd/usd/prim.h>
#include <pxr/usdImaging/usdImagingGL/engine.h>
#include <pxr/base/gf/camera.h>
#include <pxr/base/gf/vec3d.h>
#include "CameraController.h"

PXR_NAMESPACE_USING_DIRECTIVE

class CameraController;

class Scene
{
public:
    Scene();
    void prepare(float dt);
    void draw(int width, int height);
    void click();
    void cursor(float x, float y);
    void scroll(double dx, double dy);
    void keypress(int key, int scancode, int action);

private:
    void _setupCamera(CameraParams params);

    pxr::  UsdStageRefPtr mStage;
    pxr::SdfPathVector mExcludePaths;
    pxr::UsdImagingGLEngine mRenderer;
    pxr::UsdImagingGLRenderParams mParams;
   
    pxr::GfCamera mCamera;

    int mWidth;
    int mHeight;

    std::unique_ptr<CameraController> cameraController;
    friend class Tutorials;
    friend class CompletedTutorials;

};

