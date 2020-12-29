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

    spGoldAlbedo = std::make_unique<Texture>();
    spGoldAlbedo->Init(arGfx, L"Textures/Gold_Albedo.png");

    spGoldNormal = std::make_unique<Texture>();
    spGoldNormal->Init(arGfx, L"Textures/Gold_Normal.png");

    spGoldRoughness = std::make_unique<Texture>();
    spGoldRoughness->Init(arGfx, L"Textures/Gold_Roughness.png");

    spGoldMetallic = std::make_unique<Texture>();
    spGoldMetallic->Init(arGfx, L"Textures/Gold_Metallic.png");

    spLight1 = std::make_unique<PointLight>();
    spLight1->Pos = { 0.0f, -1.0f, 3.25f, 10.0f };
    spLight1->Color = { 1.0f, 0.0f, 0.0f, 1.0f };
    spLight1->Attenuation = { 0.0f, 0.2f, 0.0f };
}

void GameGraphics::Shutdown(App& arApp, Graphics& arGfx)
{
    spLight1.release();

    spGoldMetallic.release();
    spGoldRoughness.release();
    spGoldNormal.release();
    spGoldAlbedo.release();
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
    worldInstance1.pTextures.emplace(std::make_pair(TextureKey::ModelTex1, spbraynzar.get()));
    worldInstance2.WorldMatrix = worldMatrix2;
    worldInstance2.pTextures.emplace(std::make_pair(TextureKey::ModelTex1, spbraynzar.get()));

    GraphicsDrawState drawState;
    drawState.pModel = spCube.get();
    drawState.RasterizerState = RasterizerStates::Default;
    /*drawState.pShader = ShaderMgr.GetShader(ShaderKey::BasicShader);
    drawState.RasterizerState = RasterizerStates::Default;
	arGfx.AddItemToDraw(drawState, worldInstance1);*/

    drawState.pShader = ShaderMgr.GetShader(ShaderKey::PbrShader);
    arGfx.AddItemToDraw(drawState, worldInstance2);

    arGfx.AddPointLight(*spLight1);
}