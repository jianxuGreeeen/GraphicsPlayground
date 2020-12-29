#include "GameGraphics.h"
#include "App.h"
#include "Graphics.h"
#include "MathTypes.h"

void GameGraphics::Init(App& arApp, Graphics& arGfx)
{
    LoadResources(arGfx);
}

void GameGraphics::LoadResources(Graphics& arGfx)
{
    ShaderMgr.Load(arGfx);

	auto& rdevice = arGfx.GetDevice();
	spTriangle = Model::NewTriangle();
	spTriangle->Init(arGfx);

	spQuad = Model::NewQuad();
	spQuad->Init(arGfx);

    spCube = Model::NewCube();
    spCube->Init(arGfx);

    spbraynzar = std::make_unique<Texture>();
    spbraynzar->Init(arGfx, L"Textures/braynzar.jpg");

    spLight1 = std::make_unique<PointLight>();
    spLight1->Pos = { 0.0f, -1.0f, 3.25f, 10.0f };
    spLight1->Color = { 1.0f, 0.0f, 0.0f, 1.0f };
    spLight1->Attenuation = { 0.0f, 0.2f, 0.0f };
}

void GameGraphics::Shutdown(App& arApp, Graphics& arGfx)
{
    spbraynzar.release();
    spTriangle.release();
    spQuad.release();
    spCube.release();

    ShaderMgr.Shutdown(arGfx);
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
    angle1 += 0.0075f;

    static float angle2 = 0.0f;
    angle2 -= 0.005f;

    using namespace DirectX;
    const auto rotationAxis1 = XMVectorSet(0, 1, 1, 0);
    const auto rotationAxis2 = XMVectorSet(0, 1, 0, 0);
    const auto worldMatrix1 = XMMatrixRotationAxis(rotationAxis1, XMConvertToRadians(angle1)) * XMMatrixTranslation(1.0f, 0.0f, 2.75f);
    const auto worldMatrix2 = XMMatrixRotationAxis(rotationAxis2, XMConvertToRadians(angle2)) * XMMatrixTranslation(-1.0f, 0.0f, 2.75f);

    ModelInstance worldInstance1{};
    ModelInstance worldInstance2{};
    worldInstance1.WorldMatrix = worldMatrix1;
    worldInstance1.pTexture = spbraynzar.get();
    worldInstance2.WorldMatrix = worldMatrix2;
    worldInstance2.pTexture = spbraynzar.get();

    GraphicsDrawState drawState;
    drawState.pShader = ShaderMgr.GetShader(ShaderKey::BasicShader);
    drawState.RasterizerState = RasterizerStates::Default;
    drawState.pModel = spCube.get();
	arGfx.AddItemToDraw(drawState, worldInstance1);

    drawState.RasterizerState = RasterizerStates::Default;//WireFrame;
    arGfx.AddItemToDraw(drawState, worldInstance2);

    arGfx.AddPointLight(*spLight1);
}