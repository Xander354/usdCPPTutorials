#include "Tutorials.h"
#include "scene.h"
#include <cstdio>

#include <pxr/usd/usd/variantSets.h>
#include <pxr/usd/usd/editContext.h>
#include <pxr/usd/usd/primRange.h>
#include <pxr/usd/usdGeom/camera.h>
#include <pxr/usd/usdGeom/xformCommonAPI.h>
#include <pxr/usd/usdGeom/xform.h>
#include <pxr/usd/usdGeom/sphere.h>
#include <pxr/usd/usdGeom/metrics.h>
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
#include "CompletedTutorials/CompletedTutorials.h"

PXR_NAMESPACE_USING_DIRECTIVE

#pragma region HelloWorld
UsdStageRefPtr Tutorials::_tutorial_HelloWorld(CameraParams camParams)
{
    UsdStageRefPtr stage = UsdStage::CreateNew("HelloWorld.usda");
    
    UsdGeomXform xFormPrim = UsdGeomXform::Define(stage, SdfPath("/hello"));
    UsdGeomSphere spherePrim = UsdGeomSphere::Define(stage, SdfPath("/hello/world"));
    double r;
    auto radius = spherePrim.GetPrim().GetAttribute(TfToken("radius"));
    radius.Get(&r);
    radius.Set(r + 0.5);

    mScene->_setupCamera(camParams);
    return std::move(stage);
}
#pragma endregion

#pragma region HelloWorldRedux
UsdStageRefPtr Tutorials::_tutorial_HelloWorldRedux(CameraParams camParams)
{
    UsdStageRefPtr stage = UsdStage::Open("HelloWorld.usda");

    //HelloWorldRedux
    //https://graphics.pixar.com/usd/release/tut_helloworld_redux.html

    mScene->_setupCamera(camParams);
    return std::move(stage);
}
#pragma endregion

#pragma region Properties
UsdStageRefPtr Tutorials::_tutorial_InspectingAndAuthoringProperties(CameraParams camParams)
{
    UsdStageRefPtr stage = UsdStage::Open("HelloWorld.usda");
    
    //Inspecting and Authoring Properties
    //https://graphics.pixar.com/usd/release/tut_inspect_and_author_props.html

    mScene->_setupCamera(camParams);
    return std::move(stage);
}
#pragma endregion

#pragma region Layers
UsdStageRefPtr Tutorials::_tutorial_ReferencingLayers(CameraParams camParams)
{
    UsdStageRefPtr stage = UsdStage::Open("HelloWorld.usda");
    
    //Referencing Layers
    //https://graphics.pixar.com/usd/release/tut_referencing_layers.html

    mScene->_setupCamera(camParams);
    return std::move(stage); //This one should return a different stage but needed placeholder
}
#pragma endregion

#pragma region TraversingAStage
UsdStageRefPtr Tutorials::_tutorial_TraversingAStage(CameraParams camParams)
{
    UsdStageRefPtr stage = UsdStage::Open("RefExample.usda");
    
    //Traversing a stage
    //https://graphics.pixar.com/usd/release/tut_traversing_stage.html

    return std::move(stage);
}
#pragma endregion

#pragma region Variants
UsdStageRefPtr Tutorials::_tutorial_AuthoringVariants(CameraParams camParams)
{
    UsdStageRefPtr stage = UsdStage::Open("HelloWorld.usda");
   
    //Authoring Variants
    //https://graphics.pixar.com/usd/release/tut_authoring_variants.html

    mScene->_setupCamera(camParams);
    return std::move(stage);
}

#pragma endregion

#pragma region Transformations

UsdStageRefPtr Tutorials::_tutorial_Transformations_TimeSamplesAnimations_LayerOffsets(CameraParams camParams)
{
    UsdStageRefPtr stage;

    //Transformations, Time samples and animations, and Layer Offsets
    //https://graphics.pixar.com/usd/release/tut_xforms.html

    mScene->_setupCamera(camParams);
    return std::move(stage);
}
#pragma endregion

#pragma region Shading
UsdStageRefPtr Tutorials::_tutorial_SimpleShading(CameraParams camParams)
{
    UsdStageRefPtr stage = UsdStage::CreateNew("simpleShading.usda");
    
    //Simple Shading
    //https://graphics.pixar.com/usd/release/tut_simple_shading.html

    mScene->_setupCamera(camParams);
    return std::move(stage);
}
#pragma endregion


pxr::UsdStageRefPtr Tutorials::runTutorial()
{
    UsdStageRefPtr result = _tutorial_HelloWorld();
	  return std::move(result);
}

