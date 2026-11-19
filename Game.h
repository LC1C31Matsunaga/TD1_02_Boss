#pragma once

///========================================
/// 定数
///========================================

// ウィンドウサイズ
const int kWindowWidth = 1920;
const int kWindowHeight = 1080;
// マップチップサイズ
const int kChipSize = 64;
// マップ行（横）
const int kMapCols = 30;
// マップ列（縦）
const int kMapRows = 17;

///========================================
/// 列挙型
///========================================

//シーン遷移
enum Scene
{
	TITLE,
	TUTORIAL,
	BATTLE,
	CLEAR,
	GAMEOVER
};

// マップチップタイプの種類
enum TileType
{
	NONE,		// なし
	BLOCK,		// 壁や床
	FLOOR		// すり抜けられる足場
};

// 弾の色
enum BulletColor
{
	BULLET_NONE,
	BULLET_RED,
	BULLET_BLUE,
	BULLET_YELLOW,
	BULLET_BLACK
};

// レシピの種類
enum RecipeType
{
	RECIPE_INCOMPLETE,
	RECIPE_RRR,
	RECIPE_BBB,
	RECIPE_YYY,
	RECIPE_RRB,
	RECIPE_RRY,
	RECIPE_BBR,
	RECIPE_BBY,
	RECIPE_YYR,
	RECIPE_YYB,
	RECIPE_RBY
};

// ボスフォーム
enum BossForm
{
	BOSS_RED,
	BOSS_BLUE,
	BOSS_YELLOW
};

// ベクトル
struct Vector2
{
	float x;
	float y;
};