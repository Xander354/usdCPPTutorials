#include "CompletedTutorials.h"
#include "../scene.h"
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

PXR_NAMESPACE_USING_DIRECTIVE

#pragma region HelloWorld
UsdStageRefPtr CompletedTutorials::_tutorial_HelloWorld(CameraParams camParams)
{
  UsdStageRefPtr stage = UsdStage::CreateInMemory();
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
UsdStageRefPtr CompletedTutorials::_tutorial_HelloWorldRedux(CameraParams camParams)
{
  UsdStageRefPtr stage = UsdStage::CreateInMemory();
  UsdPrim xform = stage->DefinePrim(SdfPath("/hello"), TfToken("Xform"));
  UsdPrim sphere = stage->DefinePrim(SdfPath("/hello/world"), TfToken("Sphere"));

  mScene->_setupCamera(camParams);
  return std::move(stage);
}
#pragma endregion

#pragma region Properties
UsdStageRefPtr CompletedTutorials::_tutorial_InspectingAndAuthoringProperties(CameraParams camParams)
{
  UsdStageRefPtr stage = UsdStage::Open("./CompletedTutorials/HelloWorld.usda");
  UsdPrim xform = stage->GetPrimAtPath(SdfPath("/hello"));
  UsdPrim sphere = stage->GetPrimAtPath(SdfPath("/hello/world"));

  std::cout << "Xform Properties" << std::endl;
  for (auto& token : xform.GetPropertyNames())
    std::cout << "    " << token << std::endl;

  std::cout << "Sphere Properties" << std::endl;
  for (auto& token : sphere.GetPropertyNames())
    std::cout << "    " << token << std::endl;

  UsdAttribute extentAttr = sphere.GetAttribute(TfToken("extent"));
  VtVec3fArray extent;
  extentAttr.Get(&extent);
  std::cout << "Sphere Extent" << std::endl;
  std::cout << extent << std::endl;

  UsdAttribute radiusAttr = sphere.GetAttribute(TfToken("radius"));
  radiusAttr.Set(2.0f);

  extentAttr.Set(extent * 2);

  UsdGeomSphere sphereSchema = UsdGeomSphere(sphere);
  VtArray<GfVec3f> color;
  color.push_back({ 0, 0, 1 });
  sphereSchema.GetDisplayColorAttr().Set(color);

  mScene->_setupCamera(camParams);
  stage->Save();
  return std::move(stage);
}
#pragma endregion

#pragma region Layers
UsdStageRefPtr CompletedTutorials::_tutorial_ReferencingLayers(CameraParams camParams)
{
  UsdStageRefPtr stage = UsdStage::Open("./CompletedTutorials/HelloWorld.usda");
  UsdPrim hello = stage->GetPrimAtPath(SdfPath("/hello"));
  stage->SetDefaultPrim(hello);
  UsdGeomXformCommonAPI(hello).SetTranslate(GfVec3d(4, 5, 6));
  std::string rootAsString;
  stage->GetRootLayer()->ExportToString(&rootAsString);
  std::cout << rootAsString << std::endl;

  UsdStageRefPtr refStage = UsdStage::CreateNew("RefExample.usda");
  UsdPrim refSphere = refStage->OverridePrim(SdfPath("/refSphere"));
  refStage->GetRootLayer()->ExportToString(&rootAsString);
  std::cout << rootAsString << std::endl;

  refSphere.GetReferences().AddReference(SdfReference("./CompletedTutorials/HelloWorld.usda"));
  refStage->GetRootLayer()->ExportToString(&rootAsString);
  std::cout << rootAsString << std::endl;

  UsdGeomXformable refXform(refSphere);
  refXform.SetXformOpOrder({});
  refStage->GetRootLayer()->ExportToString(&rootAsString);
  std::cout << rootAsString << std::endl;

  UsdPrim refSphere2 = refStage->OverridePrim(SdfPath("/refSphere2"));
  refSphere2.GetReferences().AddReference(SdfReference("./CompletedTutorials/HelloWorld.usda"));
  refStage->GetRootLayer()->ExportToString(&rootAsString);
  std::cout << rootAsString << std::endl;

  UsdGeomSphere overSphere = UsdGeomSphere::Get(refStage, SdfPath("/refSphere2/world"));
  VtArray<GfVec3f> color;
  color.push_back({ 1, 0, 0 });
  overSphere.GetDisplayColorAttr().Set(color);
  refStage->GetRootLayer()->ExportToString(&rootAsString);
  std::cout << rootAsString << std::endl;

  mScene->_setupCamera(camParams);
  return std::move(refStage);
}
#pragma endregion

#pragma region TraversingAStage
UsdStageRefPtr CompletedTutorials::_tutorial_TraversingAStage(CameraParams camParams)
{
  UsdStageRefPtr stage = UsdStage::Open("./CompletedTutorials/RefExample.usda");
  for (UsdPrim prim : stage->Traverse()) {
    std::cout << prim.GetPath().GetString() << std::endl;
  }

  mScene->_setupCamera(camParams);
  return std::move(stage);
}
#pragma endregion

#pragma region Variants
UsdStageRefPtr CompletedTutorials::_tutorial_AuthoringVariants(CameraParams camParams)
{
  UsdStageRefPtr stage = UsdStage::Open("./CompletedTutorials/HelloWorld.usda");
  UsdAttribute colorAttr = UsdGeomGprim::Get(stage, SdfPath("/hello/world")).GetDisplayColorAttr();
  colorAttr.Clear();

  UsdPrim rootPrim = stage->GetPrimAtPath(SdfPath("/hello"));
  UsdVariantSet vset = rootPrim.GetVariantSets().AddVariantSet("shadingVariant");
  vset.AddVariant("red");
  vset.AddVariant("blue");
  vset.AddVariant("green");

  {
    vset.SetVariantSelection("red");
    UsdEditContext ctxt(vset.GetVariantEditContext());
    VtArray<GfVec3f> color;
    color.push_back({ 1, 0, 0 });
    colorAttr.Set(color);
  }

  {
    vset.SetVariantSelection("blue");
    UsdEditContext ctxt(vset.GetVariantEditContext());
    VtArray<GfVec3f> color;
    color.push_back({ 0, 0, 1 });
    colorAttr.Set(color);
  }

  {
    vset.SetVariantSelection("green");
    UsdEditContext ctxt(vset.GetVariantEditContext());
    VtArray<GfVec3f> color;
    color.push_back({ 0, 1, 0 });
    colorAttr.Set(color);
  }

  std::string rootAsString;
  stage->GetRootLayer()->ExportToString(&rootAsString);
  std::cout << rootAsString << std::endl;

  std::string stageAsString;
  stage->ExportToString(&stageAsString, false);
  std::cout << stageAsString << std::endl;

  stage->GetRootLayer()->Export("./CompletedTutorials/HelloWorldWithVariants.usda");
  mScene->_setupCamera(camParams);
  return std::move(stage);
}

#pragma endregion

#pragma region Transformations
namespace CompletedTransform
{
  UsdStageRefPtr MakeInitialStage(std::string path)
  {
    UsdStageRefPtr stage = UsdStage::CreateNew(path);
    UsdGeomSetStageUpAxis(stage, UsdGeomTokens->z);
    stage->SetStartTimeCode(1.0);
    stage->SetEndTimeCode(192.0);
    return std::move(stage);
  }

  void Step1()
  {
    UsdStageRefPtr stage = MakeInitialStage("./Step1.usda");
    stage->SetMetadata(TfToken("comment"), "Step 1: Start and end time codes");
    stage->Save();
  }

  UsdGeomXform AddReferenceToGeometry(UsdStageRefPtr stage, std::string path)
  {
    UsdGeomXform geom = UsdGeomXform::Define(stage, SdfPath(path));
    geom.GetPrim().GetReferences().AddReference("./top.geom.usd");
    return std::move(geom);
  }

  void Step2()
  {
    UsdStageRefPtr stage = MakeInitialStage("./Step2.usda");
    stage->SetMetadata(TfToken("comment"), "Step 2: Geometry reference");
    UsdGeomXform top = AddReferenceToGeometry(stage, "/Top");
    stage->Save();
  }

  void AddSpin(UsdGeomXform top)
  {
    UsdGeomXformOp spin = top.AddRotateZOp(UsdGeomXformOp::PrecisionFloat, TfToken("spin"));
    spin.Set(0.0f, 1);
    spin.Set(1440.f, 192);
  }

  void Step3()
  {
    UsdStageRefPtr stage = MakeInitialStage("./Step3.usda");
    stage->SetMetadata(TfToken("comment"), "Step 3: Adding Spin Animation");
    UsdGeomXform top = AddReferenceToGeometry(stage, "/Top");
    AddSpin(top);
    stage->Save();
  }

  void AddTilt(UsdGeomXform top)
  {
    UsdGeomXformOp tile = top.AddRotateXOp(UsdGeomXformOp::PrecisionFloat, TfToken("tile"));
    tile.Set(12.f);
  }

  void Step4()
  {
    UsdStageRefPtr stage = MakeInitialStage("./Step4.usda");
    stage->SetMetadata(TfToken("comment"), "Step 4: Adding Tilt");
    UsdGeomXform top = AddReferenceToGeometry(stage, "/Top");
    AddTilt(top);
    AddSpin(top);
    stage->Save();
  }

  void AddOffset(UsdGeomXform top)
  {
    top.AddTranslateOp(UsdGeomXformOp::PrecisionFloat, TfToken("offset")).Set(GfVec3f(0.0f, 0.1f, 0.0f));
  }

  void AddPrecession(UsdGeomXform top)
  {
    UsdGeomXformOp precess = top.AddRotateZOp(UsdGeomXformOp::PrecisionFloat, TfToken("precess"));
    precess.Set(0.f, 1.0);
    precess.Set(360.f, 192.0);
  }

  void Step5()
  {
    UsdStageRefPtr stage = MakeInitialStage("./Step5.usda");
    stage->SetMetadata(TfToken("comment"), "Step 5: Adding precession and offset");
    UsdGeomXform top = AddReferenceToGeometry(stage, "/Top");
    AddPrecession(top);
    AddOffset(top);
    AddTilt(top);
    AddSpin(top);
    stage->Save();
  }

  void Step6()
  {
    // Use animated layer from Step5
    std::string anim_layer_path("./Step5.usda");

    UsdStageRefPtr stage = MakeInitialStage("./Step6.usda");
    stage->SetMetadata(TfToken("comment"), "Step 6: Layer offsets and animation");

    UsdGeomXform left = UsdGeomXform::Define(stage, SdfPath("/Left"));
    UsdGeomXform left_top = UsdGeomXform::Define(stage, SdfPath("/Left/Top"));
    left_top.GetPrim().GetReferences().AddReference(anim_layer_path, SdfPath("/Top"));

    UsdGeomXform middle = UsdGeomXform::Define(stage, SdfPath("/Middle"));
    middle.AddTranslateOp().Set(GfVec3d(2.0, 0.0, 0.0));
    UsdGeomXform middle_top = UsdGeomXform::Define(stage, SdfPath("/Middle/Top"));
    middle_top.GetPrim().GetReferences().AddReference(anim_layer_path, SdfPath("/Top"), SdfLayerOffset(96));

    UsdGeomXform right = UsdGeomXform::Define(stage, SdfPath("/Right"));
    right.AddTranslateOp().Set(GfVec3d(4.0, 0.0, 0.0));
    UsdGeomXform right_top = UsdGeomXform::Define(stage, SdfPath("/Right/Top"));
    right_top.GetPrim().GetReferences().AddReference(anim_layer_path, SdfPath("/Top"), SdfLayerOffset(0.0, 0.25));

    stage->Save();
  }
}
UsdStageRefPtr CompletedTutorials::_tutorial_Transformations_TimeSamplesAnimations_LayerOffsets(CameraParams camParams)
{
  CompletedTransform::Step1();
  CompletedTransform::Step2();
  CompletedTransform::Step3();
  CompletedTransform::Step4();
  CompletedTransform::Step5();
  CompletedTransform::Step6();
  UsdStageRefPtr stage = UsdStage::Open("Step6.usda");

  mScene->_setupCamera(camParams);
  return std::move(stage);
}
#pragma endregion

#pragma region Shading
UsdStageRefPtr CompletedTutorials::_tutorial_SimpleShading(CameraParams camParams)
{
  UsdStageRefPtr stage = UsdStage::CreateNew("simpleShading.usda");
  UsdGeomSetStageUpAxis(stage, UsdGeomTokens->y);

  UsdGeomXform modelRoot = UsdGeomXform::Define(stage, SdfPath("/TexModel"));
  UsdModelAPI(modelRoot).SetKind(KindTokens->component);

  UsdGeomMesh billboard = UsdGeomMesh::Define(stage, SdfPath("/TexModel/card"));
  billboard.CreatePointsAttr(VtValue(VtVec3fArray({ {-430.0, -145.0, 0.0}, {430.0, -145.0, 0.0}, {430.0, 145.0, 0.0}, {-430.0, 145.0, 0.0} })));
  billboard.CreateFaceVertexCountsAttr(VtValue(VtArray<int>(1, 4)));
  billboard.CreateFaceVertexIndicesAttr(VtValue(VtArray<int>({ 0, 1, 2, 3 })));
  UsdGeomPrimvar texCoords = UsdGeomPrimvarsAPI(billboard).CreatePrimvar(TfToken("st"), SdfValueTypeNames->TexCoord2fArray, UsdGeomTokens->varying);
  texCoords.Set(VtVec2fArray({ {0, 0}, {1, 0}, {1,1}, {0, 1} }));

  UsdShadeMaterial material = UsdShadeMaterial::Define(stage, SdfPath("/TexModel/boardMat"));
  UsdShadeShader pbrShader = UsdShadeShader::Define(stage, SdfPath("/TexModel/boardMat/PBRShader"));
  pbrShader.CreateIdAttr(VtValue(TfToken("UsdPreviewSurface")));
  pbrShader.CreateInput(TfToken("roughness"), SdfValueTypeNames->Float).Set(0.4f);
  pbrShader.CreateInput(TfToken("metallic"), SdfValueTypeNames->Float).Set(0.0f);
  material.CreateSurfaceOutput().ConnectToSource(pbrShader.ConnectableAPI(), TfToken("surface"));

  UsdShadeShader stReader = UsdShadeShader::Define(stage, SdfPath("/TexModel/boardMat/stReader"));
  stReader.CreateIdAttr(VtValue(TfToken("UsdPrimvarReader_float2")));

  UsdShadeShader diffuseTextureSampler = UsdShadeShader::Define(stage, SdfPath("/TexModel/boardMat/diffuseTexture"));
  diffuseTextureSampler.CreateIdAttr(VtValue(TfToken("UsdUVTexture")));
  diffuseTextureSampler.CreateInput(TfToken("file"), SdfValueTypeNames->Asset).Set(SdfAssetPath("USDLogoLrg.png"));
  diffuseTextureSampler.CreateInput(TfToken("st"), SdfValueTypeNames->Float2).ConnectToSource(stReader.ConnectableAPI(), TfToken("result"));
  diffuseTextureSampler.CreateOutput(TfToken("rgb"), SdfValueTypeNames->Float3);
  pbrShader.CreateInput(TfToken("diffuseColor"), SdfValueTypeNames->Color3f).ConnectToSource(diffuseTextureSampler.ConnectableAPI(), TfToken("rgb"));

  UsdShadeInput stInput = material.CreateInput(TfToken("frame:stPrimvarName"), SdfValueTypeNames->Token);
  stInput.Set(VtValue(TfToken("st")));
  stReader.CreateInput(TfToken("varname"), SdfValueTypeNames->Token).ConnectToSource(stInput);

  billboard.GetPrim().ApplyAPI<UsdShadeMaterialBindingAPI>();
  UsdShadeMaterialBindingAPI(billboard).Bind(material);

  stage->Save();

  mScene->_setupCamera(camParams);
  return std::move(stage);
}
#pragma endregion


pxr::UsdStageRefPtr CompletedTutorials::runTutorial()
{
  UsdStageRefPtr result = _tutorial_ReferencingLayers();
  return std::move(result);
}

