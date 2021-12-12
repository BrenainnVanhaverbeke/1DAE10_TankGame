// Vanhaverbeke, Brenainn - 1DAE10
// Duterte, Tom - 1DAE11

#pragma once
using namespace utils;
#pragma region gameInformation
// Set your name and group in the title here
std::string g_WindowTitle{ "Tank Game - Brenainn + Tom - 1DAE10-11" };

// Change the window dimensions here
float g_WindowWidth{ 1280 };
float g_WindowHeight{ 720 };
#pragma endregion gameInformation

#pragma region ownDeclarations

#pragma region Enums

enum class TankOrientation
{
	UP,
	DOWN,
	LEFT,
	RIGHT,
	END
};

#pragma endregion Enums

#pragma region Structs

struct GridCell
{
	bool isFree;
	Index2D index;
	Rectf cell;
};

struct Tank
{
	int health;
	float barrelAngle;
	Index2D tankIndex;
	TankOrientation orientation;
};

struct Obstacle
{
	int health;
	Index2D obstacleIndex;
};

#pragma endregion Structs

const float g_SideLength{ 80.0f };
const int g_GridRows{ (int)(g_WindowHeight / g_SideLength) };
const int g_GridColumns{ (int)(g_WindowWidth / g_SideLength) };
const int g_AmountOfGridCells{ g_GridRows * g_GridColumns };
const int g_AmountOfPlayers{ 2 };
const int g_NrOfObstacles{ 20 };

const Color4f g_White{ 1.0f, 1.0f, 1.0f, 1.0f };
const Index2D g_StartingPositions[g_AmountOfPlayers]{ Index2D{1, g_GridRows - 2}, Index2D{g_GridColumns - 2, 1} };

int g_TurnCounter{ 0 };

Texture g_Tile{};
Texture g_Background{};
Point2f g_MousePosition{};

Tank* g_Tanks{ new Tank[g_AmountOfPlayers] };
GridCell* g_Grid{ new GridCell[g_AmountOfGridCells] };
Obstacle* g_pObstacles{ nullptr };

// Declare your own functions here

#pragma region Initialisers

void InitialiseGrid();
void InitialiseBorder();
void InitialiseTanks();
void InitialiseBackground();
void InitialiseTiles();
void InitialiseObstacles(Tank* pTanks);

#pragma endregion Initialisers

#pragma region Logic

void MoveTank(TankOrientation direction);
void CalculateBarrelAngle(const Point2f& mousePosition);

#pragma endregion Logic

#pragma region DrawFunctions

void DrawBackground();
void DrawGrid();
void DrawObstacles();
void DrawTanks();
void DrawBarrels();
void DrawInstructions();
//void DrawTile();

#pragma endregion DrawFunctions

#pragma region CleanupFunctions

void EndTile();
void EndBackground();

#pragma endregion CleanupFunctions

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
