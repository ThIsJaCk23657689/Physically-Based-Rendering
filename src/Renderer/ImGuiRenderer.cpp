#include "Renderer/ImGuiRenderer.hpp"

#include <imgui.h>
#include <imgui_impl_opengl3.h>
#include <imgui_impl_sdl3.h>

#include "App/Application.hpp"

ImGuiRenderer::ImGuiRenderer( Application* app ) : IRenderPass( app )
{
    // ui create
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
}

ImGuiRenderer::~ImGuiRenderer()
{
    // ui destroy
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL3_Shutdown();
    ImGui::DestroyContext();
}

bool ImGuiRenderer::Init()
{
    // ui initialize
    ImGui::StyleColorsDark();
    ImGui_ImplSDL3_InitForOpenGL( GetApplication()->GetWindow(), GetApplication()->GetContext() );
    ImGui_ImplOpenGL3_Init();
    return true;
}

bool ImGuiRenderer::LoadFont( const std::filesystem::path& fontFile, float fontSize )
{
    auto& io = ImGui::GetIO();
    ImFont* font = io.Fonts->AddFontFromFileTTF( fontFile.string().c_str(), fontSize );
    return ( font != nullptr );
}

void ImGuiRenderer::Animate( const float& deltaTime )
{
    // Start the Dear ImGui Frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL3_NewFrame();
    ImGui::NewFrame();
}

void ImGuiRenderer::Render()
{
    BuildUI();

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData( ImGui::GetDrawData() );
}

void ImGuiRenderer::BackBufferResizing() {}

void ImGuiRenderer::BeginFullScreenWindow()
{
    int width, height;
    GetApplication()->GetWindowSize( width, height );
    ImGui::SetNextWindowPos( ImVec2( 0.0f, 0.0f ), ImGuiCond_Always );
    ImGui::SetNextWindowSize( ImVec2( float( width ), float( height ) ), ImGuiCond_Always );
    ImGui::PushStyleVar( ImGuiStyleVar_WindowBorderSize, 0.0f );
    ImGui::SetNextWindowBgAlpha( 0.0f );
    ImGui::Begin(
        "Loading Screen", 0, ImGuiWindowFlags_NoInputs | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoScrollbar );
}

void ImGuiRenderer::DrawScreenCenteredText( const char* text )
{
    int width, height;
    GetApplication()->GetWindowSize( width, height );
    ImVec2 textSize = ImGui::CalcTextSize( text );
    ImGui::SetCursorPosX( ( float( width ) - textSize.x ) * 0.5f );
    ImGui::SetCursorPosY( ( float( height ) - textSize.y ) * 0.5f );
    ImGui::TextUnformatted( text );
}

void ImGuiRenderer::EndFullScreenWindow()
{
    ImGui::End();
    ImGui::PopStyleVar();
}

bool ImGuiRenderer::ProcessEvent( const SDL_Event& event )
{
    return ImGui_ImplSDL3_ProcessEvent( &event );
}

bool ImGuiRenderer::OnKeyboardEvent( const SDL_KeyboardEvent& event )
{
    auto& io = ImGui::GetIO();
    return io.WantCaptureKeyboard;
}

bool ImGuiRenderer::OnTextInputEvent( const SDL_TextInputEvent& event )
{
    auto& io = ImGui::GetIO();
    return io.WantCaptureKeyboard;
}

bool ImGuiRenderer::OnMouseButtonEvent( const SDL_MouseButtonEvent& event )
{
    auto& io = ImGui::GetIO();
    return io.WantCaptureMouse;
}

bool ImGuiRenderer::OnMouseMotionEvent( const SDL_MouseMotionEvent& event )
{
    auto& io = ImGui::GetIO();
    return io.WantCaptureMouse;
}

bool ImGuiRenderer::OnMouseWheelEvent( const SDL_MouseWheelEvent& event )
{
    auto& io = ImGui::GetIO();
    return io.WantCaptureMouse;
}
