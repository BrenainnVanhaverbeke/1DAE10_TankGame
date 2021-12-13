// Vanhaverbeke, Brenainn - 1DAE10
// Duterte, Tom - 1DAE11

#pragma once
using namespace utils;
#pragma region gameInformation
// Set your name and group in the title here
std::string g_WindowTitle{ "Tank Game - Brenainn Vanhaverbeke - Tom Duterte - 1DAE10-11" };

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
	Point2f tankCenter;
	Point2f barrelEnd;
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
const int g_ProjectileSpeed{ 750 };
const float g_BarrelLength{ 60.0f };

const Color4f g_White{ 1.0f, 1.0f, 1.0f, 1.0f };
const Index2D g_StartingPositions[g_AmountOfPlayers]{ Index2D{1, g_GridRows - 2}, Index2D{g_GridColumns - 2, 1} };

int g_TurnCounter{ 0 };
bool g_IsShooting{ false };
bool g_IsGameOver{ false };
bool g_IsIPressed{ false };
bool g_IsLoggedToConsole{ false };

Texture g_Tile{};
Texture g_Background{};
Point2f g_MousePosition{};
Circlef g_Projectile{ Point2f{0.0f, 0.0f}, 3.0f };
Vector2f g_ProjectileVector{};

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
void UpdateBarrelEnd();
void UpdateProjectile(float elapsedSec);
int CheckTankHit();
int CheckObstacleHit();
void ResolveTankHit(int tankIndex);
void ResolveObstacleHit(int obstacleIndex);

#pragma endregion Logic

#pragma region DrawFunctions

void DrawBackground();
void DrawGrid();
void DrawObstacles();
void DrawTanks();
void DrawBarrels();
void DrawProjectile();
void DrawEndScreen();
void DrawInstructions();
Point2f DrawInstructionsBackground();

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
