#include <iostream>

#include "graphics/graphics.hpp"
#include "input/input.hpp"

int WindowWidth = 1920;
int WindowHeight = 1080;
#define WIN_BACKGROUND_COLOR 20, 20, 20, 255

void windowResizeCB(float width, float height)
{
	WindowWidth = width;
	WindowHeight = height;
}

int main2();

int main()
{
	return main2();
	// window mainWindow(WindowWidth, WindowHeight, "Base 2.0 Test", &windowResizeCB);
	// {
	// 	mainWindow.setBackgroundColor(WIN_BACKGROUND_COLOR);

	// 	// shouldClose manages some state
	// 	mainWindow.shouldClose();
	// 	mainWindow.update();

	// 	mainWindow.shouldClear(true);
	// 	mainWindow.getBase()->setVerticalSyncEnabled(true);
	// 	mainWindow.getBase()->setFramerateLimit(60);
	// }

	// inputManager input(&mainWindow);

	// while (!mainWindow.shouldClose() && !input.isPressed(inputCode::Escape))
	// {
	// }
}

#include "imgui.h"
#include "imgui-SFML.h"

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Graphics/CircleShape.hpp>

int main2()
{
    sf::RenderWindow window(sf::VideoMode(640, 480), "ImGui + SFML = <3");
    window.setFramerateLimit(60);
    ImGui::SFML::Init(window);

    sf::CircleShape shape(100.f);
    shape.setFillColor(sf::Color::Green);

    sf::Clock deltaClock;
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            ImGui::SFML::ProcessEvent(event);

            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }

        ImGui::SFML::Update(window, deltaClock.restart());

		ImGui::ShowDemoWindow();

        ImGui::Begin("Hello, world!");
        ImGui::Button("Look at this pretty button");
        ImGui::End();

        window.clear();
        window.draw(shape);
        ImGui::SFML::Render(window);
        window.display();
    }

    ImGui::SFML::Shutdown();

	return 0;
}
