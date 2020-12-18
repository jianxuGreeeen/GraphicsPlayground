#include "GameGraphics.h"
#include "App.h"
#include "Graphics.h"
#include "MathTypes.h"
#include "ShaderKey.h"

void GameGraphics::Init(App& arApp, Graphics& arGfx)
{
    LoadResources(arGfx);
}

void GameGraphics::LoadResources(Graphics& arGfx)
{
	auto& rdevice = arGfx.GetDevice();
	spTriangle = Model::CreateTriangle();
	spTriangle->Init(rdevice);

	spQuad = Model::CreateQuad();
	spQuad->Init(rdevice);

    spCube = Model::CreateCube();
    spCube->Init(rdevice);
}

void GameGraphics::Update(App& arApp, Graphics& arGfx)
{
    using namespace DirectX;
    Cam.EyePos = XMVectorSet(0.0f, 0, 0, 0);
    Cam.FocusPos = XMVectorSet(0, 0, 100, 0);

    const Vector camUp = XMVectorSet(0, 1, 0, 0);
    auto viewMatrix = XMMatrixLookAtLH(Cam.EyePos, Cam.FocusPos, camUp);
    arGfx.SetViewMatrix(viewMatrix);

    auto& rwindowSettings = arApp.GetSettings();
    const auto width = static_cast<float>(rwindowSettings.Width);
    const auto height = static_cast<float>(rwindowSettings.Height);

    auto projMatrix = XMMatrixPerspectiveFovLH(XMConvertToRadians(Cam.Fov), width / height, Cam.Near, Cam.Far);
    arGfx.SetProjectionMatrix(projMatrix);

    static float angle1 = 0.0f;
    angle1 += 0.005f;

    static float angle2 = 0.0f;
    angle2 -= 0.005f;

    using namespace DirectX;
    const auto rotationAxis1 = XMVectorSet(0, 1, 1, 0);
    const auto rotationAxis2 = XMVectorSet(1, 1, 0, 0);
    const auto worldMatrix1 = XMMatrixRotationAxis(rotationAxis1, XMConvertToRadians(angle1)) * XMMatrixTranslation(1.0f, 0.0f, 3.0f);
    const auto worldMatrix2 = XMMatrixRotationAxis(rotationAxis2, XMConvertToRadians(angle2)) * XMMatrixTranslation(-1.0f, 0.0f, 3.0f);

    ModelInstance worldInstance1{};
    ModelInstance worldInstance2{};
    worldInstance1.WorldMatrix = worldMatrix1;
    worldInstance2.WorldMatrix = worldMatrix2;

	arGfx.AddItemToDraw(spCube.get(), worldInstance1);
    arGfx.AddItemToDraw(spCube.get(), worldInstance2);
}