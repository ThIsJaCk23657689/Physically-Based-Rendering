#include "Renderer/ImGuiRenderer.hpp"
#include <imgui.h>
#include <imgui_impl_opengl3.h>
#include <imgui_impl_sdl.h>

ImGuiRenderer::ImGuiRenderer(Application* app)
    : IRenderPass(app) {
    // ui create
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
}

ImGuiRenderer::~ImGuiRenderer() {
    // ui destroy
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();
}

bool ImGuiRenderer::Init() {
    // ui initialize
    ImGui::StyleColorsDark();
    ImGui_ImplSDL2_InitForOpenGL(GetApplication()->GetWindow(), GetApplication()->GetContext());
    ImGui_ImplOpenGL3_Init();
    return true;
}

bool ImGuiRenderer::LoadFont(const std::filesystem::path& fontFile, float fontSize) {
    ImGuiIO& io = ImGui::GetIO();
    ImFont* font = io.Fonts->AddFontFromFileTTF(fontFile.string().c_str(), fontSize);
    return (font != nullptr);
}

void ImGuiRenderer::Animate(float deltaTime) {
    int width, height;
    GetApplication()->GetWindowSize(width, height);

    ImGuiIO& io = ImGui::GetIO();
    io.DisplaySize = ImVec2(float(width), float(height));
    io.DeltaTime = deltaTime;
    io.MouseDrawCursor = false;

    // Start the Dear ImGui Frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL2_NewFrame();
    ImGui::NewFrame();
}

void ImGuiRenderer::Render() {
    BuildUI();

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void ImGuiRenderer::BackBufferResizing() {

}

void ImGuiRenderer::BeginFullScreenWindow() {
    int width, height;
    GetApplication()->GetWindowSize(width, height);
    ImGui::SetNextWindowPos(ImVec2(0.0f, 0.0f), ImGuiCond_Always);
    ImGui::SetNextWindowSize(ImVec2(float(width), float(height)), ImGuiCond_Always);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
    ImGui::SetNextWindowBgAlpha(0.0f);
    ImGui::Begin("", 0, ImGuiWindowFlags_NoInputs | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoScrollbar);
}

void ImGuiRenderer::DrawScreenCenteredText(const char* text) {
    int width, height;
    GetApplication()->GetWindowSize(width, height);
    ImVec2 textSize = ImGui::CalcTextSize(text);
    ImGui::SetCursorPosX((float(width) - textSize.x) * 0.5f);
    ImGui::SetCursorPosY((float(height) - textSize.y) * 0.5f);
    ImGui::TextUnformatted(text);
}

void ImGuiRenderer::EndFullScreenWindow() {
    ImGui::End();
    ImGui::PopStyleVar();
}