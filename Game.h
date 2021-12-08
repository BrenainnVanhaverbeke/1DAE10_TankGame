#pragma once
using namespace utils;
#pragma region gameInformation
// Set your name and group in the title here
std::string g_WindowTitle{ "Tank Game - Brenainn + Tom - 1DAE10-11" };

// Change the window dimensions here
float g_WindowWidth{ 1280 };
float g_WindowHeight{ 720 };
#pragma endregion gameInformation



Texture g_Background{};
void InitBackground();
void DrawBackground();
void EndBackground();

Texture g_Tile{};
void InitTile();
void DrawTile();
void EndTile();

enum class TankOrientation
{
	UP,
	DOWN,
	LEFT,
	RIGHT,
	END
};

struct Tank
{
	int rowIndex;
	int columnIndex;
	int health;
	float barrelAngle;
	TankOrientation orientation;
};

#pragma region ownDeclarations
// Declare your own global variables here

const float g_SideLength{ 80.0f };
const int g_GridRows{ (int)(g_WindowHeight / g_SideLength) };
const int g_GridColumns{ (int)(g_WindowWidth / g_SideLength) };
const int g_AmountOfGridCells{ g_GridRows * g_GridColumns };
const int g_AmountOfPlayers{ 2 };

const Color4f g_White{ 1.0f, 1.0f, 1.0f, 1.0f };

int g_TurnCounter{ 0 };

Point2f g_MousePosition{};

bool* g_IsCellFree{ new bool[g_AmountOfGridCells] };
Tank* g_Tanks{ new Tank[g_AmountOfPlayers] };

// Declare your own functions here

void InitialiseBorder();
void InitialiseTanks();
void MoveTank(TankOrientation direction);
void DrawGrid();
void DrawTanks();
void DrawBarrel();
void CalculateBarrelAngle(const Point2f& mousePosition);
void InitializeObstacle(Tank g_Tanks[]);
Point2f GetCenterOfRectangle();


int GetLinearIndexFrom2D(int rowIndex, int columnIndex, int nrOfColumns);

#pragma endregion ownDeclarations

#pragma region gameFunctions											
void Start();
void Draw();
void Update(float elapsedSec);
void End();
#pragma endregion gameFunctions

#pragma region inputHandling											
void OnKeyDownEvent(SDL_Keycode key);
void OnKeyUpEvent(SDL_Keycode key);
void OnMouseMotionEvent(const SDL_MouseMotionEvent& e);
void OnMouseDownEvent(const SDL_MouseButtonEvent& e);
void OnMouseUpEvent(const SDL_MouseButtonEvent& e);
#pragma endregion inputHandling
