#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Window/Mouse.hpp>
#include <vector>

using namespace std;

// PIXEL DEFINITION
unsigned int pixelHeight = 50; // pixel's height
unsigned int pixelWidth = 50; // pixel's width
//

// SCREEN
unsigned int HPixelsCount = 18; // (height) pixels per screen
unsigned int WPixelsCount = 36; // (width) pixels per screen

vector<vector<int>> pixelGrid(HPixelsCount, vector<int>(WPixelsCount, 0));
/*
	0 - empty pixel
	1 - drawen pixel
	2 - start pixel
	3 - end pixel
*/

//

static void drawScreenPixel(sf::RenderWindow* window, float x, float y, sf::Color color) {
	sf::RectangleShape pixel(sf::Vector2f((float)pixelWidth, (float)pixelHeight));
	sf::Vector2f pixelPosition({ x,y });
	pixel.setPosition(pixelPosition);
	pixel.setFillColor(color);
	window->draw(pixel);
}

int main()
{
	unsigned int cursorX = 0, cursorY = 0;
	// how many pixels
	unsigned int screenHeight = HPixelsCount * pixelHeight, screenWidth = WPixelsCount * pixelWidth;
	sf::RenderWindow window(sf::VideoMode({ screenWidth, screenHeight }), "Test");
	//sf::Vector2u vector(20, 20);
	sf::Vector2i cursorVector;

	while (window.isOpen()) {
		while (const optional event = window.pollEvent()) {
			if (event->is<sf::Event::Closed>())
				window.close();
		}

		window.clear(sf::Color::White);

		cursorVector = sf::Mouse::getPosition(window);

		cursorX = cursorVector.x;
		cursorY = cursorVector.y;

		int rowIndx = cursorY / pixelHeight;
		int colIndx = cursorX / pixelWidth;

		//if (cursorX >= 0 && cursorY >= 0 && cursorX <= screenWidth && cursorY <= screenHeight)
		if (rowIndx >= 0 && rowIndx < HPixelsCount &&
			colIndx >= 0 && colIndx < WPixelsCount) {
			cout << "X: " << cursorX << " Y: " << cursorY << endl;

			float y = rowIndx * pixelHeight;
			float x = colIndx * pixelWidth;

			if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) { pixelGrid[rowIndx][colIndx] = 1; }
			/*drawScreenPixel(&window, x, y, sf::Color::Black);*/
			else if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Right)) { pixelGrid[rowIndx][colIndx] = 0; }
			//drawScreenPixel(&window, x, y, sf::Color::White);
		}

		for (int row = 0; row < HPixelsCount; row++)
		{
			for (int col = 0; col < WPixelsCount; col++)
			{
				float y = row * pixelHeight;
				float x = col * pixelWidth;

				if (pixelGrid[row][col] == 1)
					drawScreenPixel(&window, x, y, sf::Color::Black);
				else if (pixelGrid[row][col] == 0)
					drawScreenPixel(&window, x, y, sf::Color::White);
			}
		}

		window.display();
	}
}