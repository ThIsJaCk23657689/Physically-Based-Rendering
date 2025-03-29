#include <memory>

#include "App/AppConfig.hpp"
#include "App/Application.hpp"
#include "Renderer/MainRenderer.hpp"
#include "Renderer/UIRenderer.hpp"
#include "UIData.hpp"

int main( int argc, char** argv )
{
    AppConfig config;
    config.width = 1280;
    config.height = 720;
    config.title = "Physically Based Rendering";

    UIData uiData;

    auto app = std::make_unique< Application >();
    if ( !app->CreateContextAndWindow( config ) )
    {
        return -1;
    }

    {
        auto main = std::make_shared< MainRenderer >( app.get(), uiData );
        auto gui = std::make_shared< UIRenderer >( app.get(), main, uiData );

        gui->Init();
        gui->LoadFont( "assets/fonts/Fantasque Sans Mono Nerd Font.ttf", 14.0f );

        app->AddRenderPassToBack( main.get() );
        app->AddRenderPassToBack( gui.get() );
        app->RunMainLoop();
    }

    app->Shutdown();
    return 0;
}
