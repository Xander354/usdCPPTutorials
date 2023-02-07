#pragma once
#include <pxr/base/gf/vec3d.h>
#include <pxr/usd/usd/references.h>
#include <pxr/usd/usdGeom/tokens.h>
#include "CameraController.h"
PXR_NAMESPACE_USING_DIRECTIVE

class Scene;

class Tutorials
{
public:
    static UsdStageRefPtr _tutorial_HelloWorld(CameraParams camParams = { {0, 0, 20}, {0, 0, 0}, 30, M_PI/5, UsdGeomTokens->y});
    static UsdStageRefPtr _tutorial_HelloWorldRedux(CameraParams camParams = { {0, 0, 10}, {0, 0, 0}, 30, M_PI / 5, UsdGeomTokens->y });
    static UsdStageRefPtr _tutorial_InspectingAndAuthoringProperties(CameraParams camParams = { {0, 0, 25}, {0, 0, 0}, 30, M_PI / 5, UsdGeomTokens->y });
    static UsdStageRefPtr _tutorial_ReferencingLayers(CameraParams camParams = { {0, 0, 50}, {0, 0, 0}, 30, M_PI / 5, UsdGeomTokens->y });
    static UsdStageRefPtr _tutorial_TraversingAStage(CameraParams camParams = { {0, 0, 50}, {0, 0, 0}, 30, M_PI / 5, UsdGeomTokens->y });
    static UsdStageRefPtr _tutorial_AuthoringVariants(CameraParams camParams = { {0, 0, 50}, {0, 0, 0}, 30, M_PI / 5, UsdGeomTokens->y });
    static UsdStageRefPtr _tutorial_Transformations_TimeSamplesAnimations_LayerOffsets(CameraParams camParams = { {0, 0, 20}, {0, 0, 0}, 30, M_PI / 5, UsdGeomTokens->z });
    static UsdStageRefPtr _tutorial_SimpleShading(CameraParams camParams = { {0, 0, 2500}, {0, 0, 0}, 300, M_PI*2, UsdGeomTokens->y });

    static UsdStageRefPtr runTutorial();
    //Gets set by scene constructor
    static inline Scene* mScene;
};

