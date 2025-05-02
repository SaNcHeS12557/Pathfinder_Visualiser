#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Window/Mouse.hpp>
#include <vector>
#include <queue>
#include <climits>
#include <unordered_map>

using namespace std;
using pii = pair<int, int>;

// PIXEL (NODE REPRESENTATION) "A*"
struct Node {
	int f, g;
	int row, col;

	bool operator>(const Node &other) const {
		return f > other.f;
	}

};

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
bool gridChanged = false;

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
	gridChanged = true;

	cout << "C Pressed\n";
}

// A* ALGO:

// HEURUSTICS:
int h(int x1, int x2, int y1, int y2) {
	return abs(x1 - x2) + abs(y1 - y2);
}

// clear algo's path
void clearAlgPath() {
	for (auto &row : pixelGrid)
		replace(row.begin(), row.end(), 4, 0);
}

void runAStar(vector<vector<int>>& grid, sf::Vector2i start, sf::Vector2i goal) {
	cout << "AStar started!\n";
	// clear recent algo path
	clearAlgPath();

	//check board borders
	auto checkBorders = [&](int r, int c) {
		return r >= 0 && r < HPixelsCount && c >= 0 && c < WPixelsCount;
	};
	
	// directions
	auto neighbors = vector<pii>{ {0,1},{1,0},{-1,0},{0,-1} };

	priority_queue<Node, vector<Node>, greater<Node>> openSet;
	vector<vector<int>> GScore(HPixelsCount, vector<int>(WPixelsCount, INT_MAX));
	vector<vector<pii>> camePath(HPixelsCount, vector<pii>(WPixelsCount, { -1, -1 }));

	// start
	GScore[start.x][start.y] = 0;
	int f = h(start.x, goal.x, start.y, goal.y);
	openSet.push({ f, 0, start.x, start.y });

	while (!openSet.empty()) {
		Node curr = openSet.top(); openSet.pop();

		if (curr.row == goal.x && curr.col == goal.y) {
			int r = goal.x, c = goal.y;

			while (!(r == start.x && c == start.y)) {
				if (grid[r][c] != 3) grid[r][c] = 4;
				auto [pr, pc] = camePath[r][c];
				r = pr; c = pc;
			}

			return;
		}
		for (auto [dr, dc] : neighbors) {
			int nr = curr.row + dr, nc = curr.col + dc;

			if (!checkBorders(nr, nc)) continue;
			if (grid[nr][nc] == 1) continue;

			int G = GScore[curr.row][curr.col] + 1;
			if (G < GScore[nr][nc]) {
				camePath[nr][nc] = { curr.row, curr.col };
				GScore[nr][nc] = G;
				int f = G + h(nr, goal.x, nc, goal.y);

				openSet.push({ f, G, nr, nc });
			}
		}
	}

	cout << "No path!!!\n";
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
			else if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>())
				if (keyPressed->scancode == sf::Keyboard::Scancode::C)
					clearAll(pixelGrid);
				//else if (keyPressed->scancode == sf::Keyboard::Scancode::A)
				//	if (startChoosed && endChoosed)
				//		runAStar(pixelGrid, startPixel, endPixel);
				//	else
				//		cout << "Pick start and end first!\n";

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
							gridChanged = true;
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
							gridChanged = true;
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
				gridChanged = true;

			}
			/*drawScreenPixel(&window, x, y, sf::Color::Black);*/
			else if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Right)) {
				checkStartEndRedraw(rowIndx, colIndx);

				pixelGrid[rowIndx][colIndx] = 0;
				gridChanged = true;

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
				else if(pixelGrid[row][col] == 2 || pixelGrid[row][col] == 3)
					drawScreenPixel(&window, x, y, sf::Color::Red);
				else
					drawScreenPixel(&window, x, y, sf::Color::Magenta);
			}
		}

		// single click processing
		if (!sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
			clickMade = false;
		}

		if (startChoosed && endChoosed && gridChanged) {
			gridChanged = false;
			runAStar(pixelGrid, startPixel, endPixel);
		}

		window.display();
	}
}