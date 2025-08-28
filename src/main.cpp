#include <Window.h>
#include <Game.h>
#include <Renderer.h>
#include <Event.h>
#include "integration/SFML/Renderer.h"
#include "integration/SFML/Window.h"



int main() {
    gld::Event::Broker broker;
    gld::Renderer<gld::integration::SFML::Renderer> renderer(broker);
    gld::Window<gld::integration::SFML::Window> window(broker, 800, 600, "gld");

    gld::Game game(broker, window, renderer);

    game.run();
    
    return 0;
}
