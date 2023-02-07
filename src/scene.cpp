#include "scene.h"
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
#include <string>
#include <GLFW/glfw3.h>
#include <iostream>

#include "Tutorials.h"
#include "CompletedTutorials/CompletedTutorials.h"

PXR_NAMESPACE_USING_DIRECTIVE

void Scene::_setupCamera(CameraParams params)
{
    GfMatrix4d transform;
    transform.SetIdentity();
    mCamera = GfCamera(transform);
    cameraController = std::make_unique<CameraController>(&mCamera, params);
}

Scene::Scene() :
    mWidth(0),
    mHeight(0)
{
    Tutorials::mScene = this;
    CompletedTutorials::mScene = this;

    mStage = Tutorials::runTutorial();

    srand(time(NULL));

    GLint major = 0;
    GLint minor = 0;
    glGetIntegerv(GL_MAJOR_VERSION, &major);
    glGetIntegerv(GL_MINOR_VERSION, &minor);
    printf("OpenGL version is %i.%i\n", major, minor);

    mParams.frame = 1.0;
    mParams.complexity = 1.1f;

    mParams.enableLighting = true;


}

void Scene::prepare(float dt)
{
    cameraController->update(dt);
    mParams.frame = mParams.frame.GetValue() + 1;
}

void Scene::draw(int width, int height)
{
    mWidth = width;
    mHeight = height;

    auto frustum = mCamera.GetFrustum();
    GfMatrix4d viewMat = frustum.ComputeViewMatrix();
    GfMatrix4d projMat = frustum.ComputeProjectionMatrix();
    
    const GfVec4d viewport(0, 0, width, height);
    mRenderer.SetRenderViewport(viewport);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    float position[4] = { 0,0.5,2,0 };
    glLightfv(GL_LIGHT0, GL_POSITION, position);


    GlfSimpleLightingContextRefPtr lightingContext = GlfSimpleLightingContext::New();
    lightingContext->SetStateFromOpenGL();
    mRenderer.SetLightingState(lightingContext);
    mRenderer.SetCameraState(
            viewMat,
            projMat
            );
   
    // USD render.
    for (UsdPrim prim : mStage->Traverse()) {
        mRenderer.Render(prim, mParams);
    }
    
    // Clear OpenGL errors. Because UsdImagingGL::TestIntersection prints them.
    while (glGetError() != GL_NO_ERROR) {
        printf("GL ERROR");
    }
}

void Scene::click()
{
}

void Scene::cursor(float x, float y)
{
 
}

void Scene::scroll(double dx, double dy)
{

}

void Scene::keypress(int key, int scancode, int action)
{
    CameraController::keypress(key, scancode, action);
}
