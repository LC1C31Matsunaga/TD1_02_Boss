#include <Novice.h>
#include <stdio.h>
#pragma region class
#include "Game.h"
#include "Stage.h"
#include "Player.h"
#include "Shield.h"
#include "Boss.h"
#include "Bullet.h"
#include "Collision.h"
#include "Effect.h"
#pragma endregion

// タイトル
const char kWindowTitle[] = "1249_TRINITY_BREAK";

///========================================
/// 構造体
///========================================

// プレイヤー
struct Player
{
	Vector2 position;
	Vector2 speed;
	Vector2 velocity;
	Vector2 acceleration;
	Vector2 size;
	int hp, maxHp;
	bool isGrounded;			// 地面にいるか
	int jumpCount;
	int dropThroughTimer;		// 足場すり抜け時間

	// 盾
	bool isShielding;			// 盾を構えているか
	int shieldFrames;
	float shieldEnergy;			// 展開エネルギー
	float maxShieldEnergy;		// 最大展開エネルギー
	int stunTimer;				// 行動不能タイマー

	BulletColor stock[3];		// 吸収した弾
	bool stockJust[3];			// ジャスト吸収した弾
	int stockCount;				// ストック内の弾の数
};

// 弾
struct Bullet
{
	bool isAlive;
	Vector2 position;
	Vector2 velocity;
	float radius;
	BulletColor color;
	RecipeType recipe;
	float damage;
	bool isJust;			// ジャスト吸収したか
	bool isHitting;			// 当たったか
	int respawnTimer;
	int hitCooldown;
};

// エフェクト
struct Effect
{
	bool isAlive;
	Vector2 position;
	float radius;
	float maxRadius;
	unsigned int color;
	int timer;
	int maxTimer;
};

// ボス
struct Boss
{
	Vector2 position;
	Vector2 velocity;
	Vector2 size;
	float hp, maxHp;
	BossForm currentForm;		// 現在のフォーム
	BossForm nextForm;			// 次のフォーム

	int formTimer;
	int shiftTimer;				//フォームチェンジ
	int attackTimer;
	int freezeTimer;
	int poisonDamage;

	bool hasIceShield;
	int iceShieldHp;
	int phase;
};

// ボスタレット
struct BossTurret
{
	bool isAlive;
	Vector2 position;
	Vector2 size;
	float hp;
	BulletColor color;
	int attackTimer;
};

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(_In_ HINSTANCE, _In_opt_ HINSTANCE, _In_ LPSTR, _In_ int) {

	// ライブラリの初期化
	Novice::Initialize(kWindowTitle, kWindowWidth, kWindowHeight);

	// キー入力結果を受け取る箱
	char keys[256] = {0};
	char preKeys[256] = {0};

	Scene currentScene = TITLE;

	int map[kMapRows][kMapCols] = {
		{ 1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1 },
		{ 1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1 },
		{ 1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1 },
		{ 1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1 },
		{ 1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1 },
		{ 1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1 },
		{ 1,0,0,0,2,2,2,2,2,2,2,0,0,0,0,0,0,0,0,2,2,2,2,2,2,2,0,0,0,1 },
		{ 1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1 },
		{ 1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1 },
		{ 1,0,0,0,0,0,0,0,0,0,0,0,0,2,2,2,2,0,0,0,0,0,0,0,0,0,0,0,0,1 },
		{ 1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1 },
		{ 1,0,0,0,2,2,2,2,2,2,2,0,0,0,0,0,0,0,0,2,2,2,2,2,2,2,0,0,0,1 },
		{ 1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1 },
		{ 1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1 },
		{ 1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1 },
		{ 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1 },
		{ 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1 },
	};

	Player player{};
	player.position.x = static_cast<float>(kChipSize) * 3;
	player.position.y = static_cast<float>(kChipSize) * 14;
	player.speed = { 8.0f,8.0f };
	player.velocity = { 0.0f,0.0f };
	player.acceleration = { 0.0f,0.8f };
	player.size = { 64.0f,64.0f };
	player.jumpCount = 0;
	player.isGrounded = true;

	//const int kWhiteTexture = Novice::LoadTexture("./NoviceResources/white1x1.png");
	const int kPlayerTextureHandle = Novice::LoadTexture("./Resources/player.png");
	const int kMapChipTextureHandle = Novice::LoadTexture("./Resources/block.png");
	const int kFloorTextureHandle = Novice::LoadTexture("./Resources/floor.png");
	const int kTitleTextureHandle = Novice::LoadTexture("./Resources/title.png");
	const int kBackGroundTextureHandle = Novice::LoadTexture("./Resources/backGround.png");
	const int kClearTextureHandle = Novice::LoadTexture("./Resources/clear.png");
	const int kGameOverTextureHandle = Novice::LoadTexture("./Resources/gameOver.png");

	// ウィンドウの×ボタンが押されるまでループ
	while (Novice::ProcessMessage() == 0) {
		// フレームの開始
		Novice::BeginFrame();

		// キー入力を受け取る
		memcpy(preKeys, keys, 256);
		Novice::GetHitKeyStateAll(keys);

		///
		/// ↓更新処理ここから
		///

		switch (currentScene)
		{
			case TITLE:
				if (keys[DIK_SPACE] && !preKeys[DIK_SPACE])
				{
					currentScene = BATTLE;

					player.position.x = static_cast<float>(kChipSize) * 3;
					player.position.y = static_cast<float>(kChipSize) * 14;
					player.speed = { 8.0f,8.0f };
					player.velocity = { 0.0f,0.0f };
					player.acceleration = { 0.0f,0.8f };
					player.size = { 64.0f,64.0f };
					player.jumpCount = 0;
					player.isGrounded = true;
					player.dropThroughTimer = 0;
				}
				break;

			case BATTLE:
			{
				player.velocity.x = 0.0f;

				if (keys[DIK_A])
				{
					player.velocity.x -= player.speed.x;
				}
				if (keys[DIK_D])
				{
					player.velocity.x += player.speed.x;
				}

				if (keys[DIK_SPACE] && !preKeys[DIK_SPACE])
				{
					if (player.jumpCount < 2)
					{
						player.velocity.y = -17.0f;
						player.jumpCount++;
						player.isGrounded = false;
					}
				}

				if (player.dropThroughTimer > 0)
				{
					player.dropThroughTimer--;
				}

				if (player.isGrounded && keys[DIK_S] && !preKeys[DIK_S])
				{
					player.dropThroughTimer = 12;
					player.isGrounded = false;
				}

				player.velocity.y += player.acceleration.y;

				player.position.x += player.velocity.x;

				// X軸方向の移動と当たり判定
				{
					// X軸移動時プレイヤーの四隅を計算
					int leftTile = static_cast<int>(player.position.x) / kChipSize;
					int rightTile = static_cast<int>(player.position.x + player.size.x - 1.0f) / kChipSize;
					int topTile = static_cast<int>(player.position.y) / kChipSize;
					int bottomTile = static_cast<int>(player.position.y + player.size.y - 1.0f) / kChipSize;

					// 右移動時の判定
					if (player.velocity.x > 0.0f)
					{
						if (map[topTile][rightTile] == BLOCK || map[bottomTile][rightTile] == BLOCK)
						{
							// ブロックの左端に押し戻す
							player.position.x = static_cast<float>(rightTile * kChipSize - player.size.x);
							player.velocity.x = 0.0f;
						}
					}
					// 左移動時の判定
					else if (player.velocity.x < 0.0f)
					{
						if (map[topTile][leftTile] == BLOCK || map[bottomTile][leftTile] == BLOCK)
						{
							// ブロックの右端に押し戻す
							player.position.x = static_cast<float>((leftTile + 1) * kChipSize);
							player.velocity.x = 0.0f;
						}
					}
				}

				// Y軸方向の移動と当たり判定
				float prevBottom = player.position.y + player.size.y;	// 移動前の足元Y座標
				player.position.y += player.velocity.y;

				{
					// Y軸移動後のプレイヤーの四隅を更新
					int leftTile = static_cast<int>(player.position.x) / kChipSize;
					int rightTile = static_cast<int>(player.position.x + player.size.x - 1.0f) / kChipSize;
					int topTile = static_cast<int>(player.position.y) / kChipSize;
					int bottomTile = static_cast<int>(player.position.y + player.size.y - 1.0f) / kChipSize;

					// 下移動時判定
					if (player.velocity.y > 0.0f)
					{
						if (map[bottomTile][leftTile] == BLOCK || map[bottomTile][rightTile] == BLOCK)
						{
							// ブロックの上端に押し戻す
							player.position.y = static_cast<float>(bottomTile * kChipSize - player.size.y);
							player.velocity.y = 0.0f;
							player.isGrounded = true;
							player.jumpCount = 0;
						}
						else if (player.dropThroughTimer == 0 && (map[bottomTile][leftTile] == FLOOR || map[bottomTile][rightTile] == FLOOR))
						{
							float floorTop = static_cast<float>(bottomTile * kChipSize);	// 足場の上端Ｙ座標

							// 前フレームの足元が足場の上端以下（＋少しの許容誤差）なら着地させる
							if (prevBottom <= floorTop + player.velocity.y)
							{
								player.position.y = floorTop - player.size.y;
								player.velocity.y = 0.0f;
								player.isGrounded = true;
								player.jumpCount = 0;
							}


						}
					}
					else if (player.velocity.y < 0.0f)
					{
						if (map[topTile][leftTile] == BLOCK || map[topTile][rightTile] == BLOCK)
						{
							// ブロックの下端に押し戻す
							player.position.y = static_cast<float>((topTile + 1) * kChipSize);
							player.velocity.y = 0.0f;
						}
					}
				}

				break;
			}

			case CLEAR:
			case GAMEOVER:
			{
				if (keys[DIK_SPACE] && !preKeys[DIK_SPACE])
				{
					currentScene = TITLE;
				}
				break;
			}
		}

		///
		/// ↑更新処理ここまで
		///

		///
		/// ↓描画処理ここから
		///

		switch (currentScene)
		{
			case TITLE:
			{
				Novice::DrawSprite(
					0, 0,
					kTitleTextureHandle,
					1.0f, 1.0f,
					0.0f,
					WHITE
				);
				break;
			}

			case BATTLE:
			{
				Novice::DrawSprite(
					0, 0,
					kBackGroundTextureHandle,
					1.0f, 1.0f,
					0.0f,
					WHITE
				);

				for (int y = 0; y < kMapRows; y++)
				{
					for (int x = 0; x < kMapCols; x++)
					{
						if (map[y][x] == BLOCK)
						{
							Novice::DrawSprite(
								x* kChipSize,
								y* kChipSize,
								kMapChipTextureHandle,
								1.0f, 1.0f,
								0.0f,
								WHITE
							);
						}
						if (map[y][x] == FLOOR)
						{
							Novice::DrawSprite(
								x* kChipSize,
								y* kChipSize,
								kFloorTextureHandle,
								1.0f, 1.0f,
								0.0f,
								WHITE
							);
						}
					}
				}

				Novice::DrawSprite(
					static_cast<int>(player.position.x),
					static_cast<int>(player.position.y),
					kPlayerTextureHandle,
					1.0f, 1.0f,
					0.0f,
					WHITE
				);

				break;
			}

			case CLEAR:
			{
				Novice::DrawSprite(
					0, 0,
					kClearTextureHandle,
					1.0f, 1.0f,
					0.0f,
					WHITE
				);

				break;
			}

			case GAMEOVER:
			{
				Novice::DrawSprite(
					0, 0,
					kGameOverTextureHandle,
					1.0f, 1.0f,
					0.0f,
					WHITE
				);

				break;
			}
		}

		///
		/// ↑描画処理ここまで
		///

		// フレームの終了
		Novice::EndFrame();

		// ESCキーが押されたらループを抜ける
		if (preKeys[DIK_ESCAPE] == 0 && keys[DIK_ESCAPE] != 0) {
			break;
		}
	}

	// ライブラリの終了
	Novice::Finalize();
	return 0;
}
