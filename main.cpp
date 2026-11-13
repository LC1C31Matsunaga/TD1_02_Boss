#include <Novice.h>
#include <stdio.h>
#pragma region class
#include "boss.h"
#include "bullet.h"
#include "collision.h"
#include "common.h"
#include "player.h"
#include "world.h"
#pragma endregion

///=========================
/// 定数宣言
///=========================
// タイトル
const char kWindowTitle[] = "1249_ゲームタイトル";

///=========================
/// 構造体宣言
///=========================



///=========================
/// 関数宣言
///=========================


// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(_In_ HINSTANCE, _In_opt_ HINSTANCE, _In_ LPSTR, _In_ int) {

	// ライブラリの初期化
	Novice::Initialize(kWindowTitle, 1920, 1080);

	// キー入力結果を受け取る箱
	char keys[256] = {0};
	char preKeys[256] = {0};

	///=========================
	/// 変数宣言
	///=========================



	///=========================
	/// リソース読み込み
	///=========================
	const int kWhiteTexture = Novice::LoadTexture("./NoviceResources/white1x1.png");

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

		///
		/// ↑更新処理ここまで
		///

		///
		/// ↓描画処理ここから
		///

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
