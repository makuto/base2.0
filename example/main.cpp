#include <SFML/System.hpp>
#include <iostream>

#include "graphics/graphics.hpp"
#include "imgui-SFML.h"
#include "imgui.h"
#include "input/input.hpp"

int WindowWidth = 1920;
int WindowHeight = 1080;
#define WIN_BACKGROUND_COLOR 20, 20, 20, 255

void initializeWindow(window& win)
{
	{
		sf::ContextSettings settings = win.getBase()->getSettings();

		std::cout << "depth bits:" << settings.depthBits << "\n";
		std::cout << "stencil bits:" << settings.stencilBits << "\n";
		std::cout << "antialiasing level:" << settings.antialiasingLevel << "\n";
		std::cout << "version:" << settings.majorVersion << "." << settings.minorVersion << "\n";
	}

	win.setBackgroundColor(WIN_BACKGROUND_COLOR);

	// Show the window as soon as we can
	// shouldClose manages some state
	win.shouldClose();
	win.update();

	win.shouldClear(true);
	win.getBase()->setVerticalSyncEnabled(true);
	win.getBase()->setFramerateLimit(60);
}

void windowResizeCB(float width, float height)
{
	WindowWidth = width;
	WindowHeight = height;
}

void initializeImGui(window& win)
{
	ImGui::SFML::Init(*(win.getBase()));

	// For high-DPI screens
	// These don't actually work
	// ImGuiStyle& style = ImGui::GetStyle();
	// style.ScaleAllSizes(1.7f);
	// style.Alpha = 0.75f;
}

int main()
{
	std::cout << "Base2.0 example project\n";

	window mainWindow(WindowWidth, WindowHeight, "Base2.0 Example", &windowResizeCB);
	initializeWindow(mainWindow);

	inputManager input(&mainWindow);

	// If you're painting the whole window, you should uncomment this
	// mainWindow.shouldClear(false);

	sf::Clock imguiDeltaClock;
	initializeImGui(mainWindow);

	// A made up but sane first frame
	float previousFrameTime = 0.016f;
	timer frameTimer;
	frameTimer.start();

	while (!input.isPressed(inputCode::Escape))
	{
		// Poll events for ImGui and close state
		bool shouldClose =
		    mainWindow.pollEventsUpdateState([](sf::RenderWindow* win, const sf::Event& event) {
			    ImGui::SFML::ProcessEvent(event);
		    });
		if (shouldClose)
			break;

		ImGui::SFML::Update(*mainWindow.getBase(), imguiDeltaClock.restart());
		ImGui::ShowDemoWindow();
		ImGui::SFML::Render(*mainWindow.getBase());

		mainWindow.update();

		previousFrameTime = frameTimer.getTime();
		frameTimer.start();
	}

	return 0;
}
