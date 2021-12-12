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

struct Tank
{
	int health;
	float barrelAngle;
	//int rowIndex;
	//int columnIndex;
	Index2D index;
	TankOrientation orientation;
};

struct GridCell
{
	bool isOccupied;
	Index2D index;
	Rectf cell;
};

struct Index2D
{
	int row;
	int column;
};

#pragma endregion Structs

const float g_SideLength{ 80.0f };
const int g_GridRows{ (int)(g_WindowHeight / g_SideLength) };
const int g_GridColumns{ (int)(g_WindowWidth / g_SideLength) };
const int g_AmountOfGridCells{ g_GridRows * g_GridColumns };
const int g_AmountOfPlayers{ 2 };

const Color4f g_White{ 1.0f, 1.0f, 1.0f, 1.0f };
const Index2D g_StartingPositions[g_AmountOfPlayers]{ Index2D{1, g_GridColumns - 2}, Index2D{g_GridColumns - 2, 1} };

int g_TurnCounter{ 0 };

Texture g_Tile{};
Texture g_Background{};
Point2f g_MousePosition{};

bool* g_IsCellFree{ new bool[g_AmountOfGridCells] };
Tank* g_Tanks{ new Tank[g_AmountOfPlayers] };
GridCell* g_Grid{ new GridCell[g_AmountOfGridCells] };

// Declare your own functions here

#pragma region Initialisers

void InitialiseBorder();
void InitialiseTanks();
void InitialiseBackground();
void InitialiseTiles();
void InitialiseObstacles(Tank* pTanks);

#pragma endregion Initialisers

#pragma region Logic

void MoveTank(TankOrientation direction);
void CalculateBarrelAngle(const Point2f& mousePosition);
int GetLinearIndexFrom2D(int rowIndex, int columnIndex, int nrOfColumns);

#pragma endregion Logic

#pragma region DrawFunctions

void DrawBackground();
void DrawGrid();
void DrawTanks();
void DrawInstructions();
void DrawTile();

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
