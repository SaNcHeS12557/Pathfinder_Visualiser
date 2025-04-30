#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Window/Mouse.hpp>
#include <vector>

using namespace std;

// PIXEL DEFINITION
unsigned int pixelHeight = 25; // pixel's height
unsigned int pixelWidth = 25; // pixel's width
//

// SCREEN
unsigned int HPixelsCount = 36; // (height) pixels per screen
unsigned int WPixelsCount = 72; // (width) pixels per screen

vector<vector<int>> pixelGrid(HPixelsCount, vector<int>(WPixelsCount, 0));
bool isGridCleared = true;

bool startChoosed = false;
sf::Vector2i startPixel{ -1, -1 };
bool endChoosed = false;
sf::Vector2i endPixel{ -1, -1 };

bool clickMade = false;
/*
	0 - empty pixel
	1 - drawen pixel
	2 - start pixel
	3 - end pixel
	4 - algorithm's pixel
*/

static void drawScreenPixel(sf::RenderWindow* window, float x, float y, sf::Color color) {
	sf::RectangleShape pixel(sf::Vector2f((float)pixelWidth, (float)pixelHeight));
	sf::Vector2f pixelPosition({ x,y });
	pixel.setPosition(pixelPosition);
	pixel.setFillColor(color);
	window->draw(pixel);
}

void checkStartEndRedraw(int rowIndx, int colIndx) {
	if (startPixel.x == rowIndx && startPixel.y == colIndx) {
		startChoosed = false;
		startPixel = { -1, -1 };
	}
	else if (endPixel.x == rowIndx && endPixel.y == colIndx) {
		endChoosed = false;
		endPixel = { -1, -1 };
	}
}

void clearAll(vector<vector<int>> &grid) {
	if (isGridCleared) {
		cout << "Already Cleared!!!\n";
		return;
	}

	// clear pixel states
	for (auto &row : grid)
		fill(row.begin(), row.end(), 0);

	isGridCleared = true;
	startChoosed = false;
	endChoosed = false;

	cout << "C Pressed\n";
}

// A* ALGO:

// HEURUSTICS:
// todo manhattan distance

// todo a*

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
			else if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>())
				if (keyPressed->scancode == sf::Keyboard::Scancode::C)
					clearAll(pixelGrid);

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
			//cout << "X: " << cursorX << " Y: " << cursorY << endl;

			float y = rowIndx * pixelHeight;
			float x = colIndx * pixelWidth;

			if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left) && sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LShift)) {

				if(!clickMade)
					// drawing start / end pixels
					if (!startChoosed) {
						if (endPixel.x == rowIndx && endPixel.y == colIndx)
						{
							cout << "Here is an end, choose another point!\n";
						}
						else {
							pixelGrid[rowIndx][colIndx] = 2;
							startPixel = { rowIndx, colIndx };
							startChoosed = true;
						}
					}
					else if (!endChoosed) {
						if (startPixel.x == rowIndx && startPixel.y == colIndx)
						{
							cout << "Here is a start, choose another point!\n";
						}
						else {
							pixelGrid[rowIndx][colIndx] = 3;
							endPixel = { rowIndx, colIndx };
							endChoosed = true;
						}
					}
					else {
						cout << "Already Picked!\n";
					}
				clickMade = true;
				isGridCleared = false;
			}
			else if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) { 
				checkStartEndRedraw(rowIndx, colIndx);

				pixelGrid[rowIndx][colIndx] = 1; 
				isGridCleared = false;
			}
			/*drawScreenPixel(&window, x, y, sf::Color::Black);*/
			else if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Right)) {
				checkStartEndRedraw(rowIndx, colIndx);

				pixelGrid[rowIndx][colIndx] = 0; 
			}
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
				else
					drawScreenPixel(&window, x, y, sf::Color::Red);
			}
		}

		// single click processing
		if (!sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
			clickMade = false;
		}

		window.display();
	}
}