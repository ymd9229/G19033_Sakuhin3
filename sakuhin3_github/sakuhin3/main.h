#pragma once

#include "DxLib.h"
#include <stdlib.h>
#include <math.h>

#define GAME_WIDTH	1280	//画面の横のサイズ
#define GAME_HEIGHT	720		//画面の縦のサイズ
#define GAME_COLOR	32

#define GAME_WINDOW_BAR	 0
#define GAME_WINDOW_NAME	"夢の本"	//ゲームタイトル
#define GAME_FPS			60
#define PATH_MAX			255
#define NAME_MAX			255

#define IMAGE_TITLE_ROGO_PATH   TEXT(".\\IMAGE\\LogoImage\\TitleLogo.png")
#define IMAGE_CLEAR_ROGO_PATH   TEXT(".\\IMAGE\\LogoImage\\ClearLogo.png")
#define IMAGE_OVER_ROGO_PATH    TEXT(".\\IMAGE\\LogoImage\\OverLogo.png")
#define IMAGE_STAGECLEAR_PATH	TEXT(".\\IMAGE\\LogoImage\\StageClear.png")
#define IMAGE_TITLE_BACK_PATH   TEXT(".\\IMAGE\\BackImage\\TitleBack.png")
#define IMAGE_CLEAR_BK_PATH		TEXT(".\\IMAGE\\BackImage\\gameclearBK.png")
#define IMAGE_OVER_BK_PATH		TEXT(".\\IMAGE\\BackImage\\gameoverBK.png")

#define IMAGE_BOOK_PATH			TEXT(".\\IMAGE\\book.png")

#define MUSIC_TITLE_BGM_PATH    TEXT(".\\MUSIC\\bgm_maoudamashii_healing01.mp3")
#define MUSIC_STAGE1_BGM_PATH	TEXT(".\\MUSIC\\bgm_maoudamashii_piano25.mp3")
#define MUSIC_CLEAR_BGM_PATH	TEXT(".\\MUSIC\\game_maoudamashii_7_event37.mp3")
#define MUSIC_OVER_BGM_PATH	TEXT(".\\MUSIC\\game_maoudamashii_9_jingle07.mp3")

#define MUSIC_ATTACK_SE_PATH    TEXT(".\\MUSIC\\se_maoudamashii_retro16.mp3")
#define MUSIC_DEFEAT_SE_PATH    TEXT(".\\MUSIC\\se_maoudamashii_retro04.mp3")
#define MUSIC_GOAL_SE_PATH		TEXT(".\\MUSIC\\se_maoudamashii_onepoint09.mp3")
#define MUSIC_HIT_SE_PATH		TEXT(".\\MUSIC\\se_maoudamashii_battle07.mp3")

#define MOVIE_POSE_IN_PATH      TEXT(".\\MOVIE\\pose.mp4")

enum END_TYPE	//エンド画面の種類
{
	GAME_CLEAR,//クリア
	STAGE_CLEAR,//ステージクリア
	GAME_OVER,//ゲームオーバー
};

enum GAME_SCENE	//シーンの種類 
{
	GAME_SCENE_START,	//スタート画面
	GAME_SCENE_PLAY,	//プレイ画面
	GAME_SCENE_END,		//エンド画面
	GAME_SCENE_POSE		//ポーズ画面
};

//########構造体########

typedef struct STRUCT_CNT	//カウントの構造体
{
	int cnt;
	int CntMax;
}CNT;

typedef struct STRUCT_IMAGE  //画像の構造体
{
	char path[PATH_MAX];
	int handle;
	int x;
	int y;
	int width;
	int height;
}IMAGE;

typedef struct STRUCT_MOVIE	//動画の構造体
{
	char path[PATH_MAX];
	int handle;
	int x;
	int y;
	int width;
	int height;
	BOOL IsViewed;
}MOVIE;

typedef struct STRUCT_MUSIC	//音楽の構造体
{
	char path[PATH_MAX];
	int handle;
}MUSIC;

typedef struct STRUCT_CHANGE_IMAGE	//画像を変える時に使う構造体
{
	int NowImage;
	int cnt;
	int CntMax;
}CHANGE_IMAGE;

typedef struct STRUCT_COLL	//判定の構造体
{
	RECT base;
	RECT CheckItem;
	RECT CheckLeft;
	RECT CheckRight;
	RECT CheckTop;
	RECT CheckBottom;
}COLL;

//########グローバル変数########

extern IMAGE TitleRogo;		//タイトルロゴ
extern IMAGE ClearRogo;		//ゲームクリア時のロゴ
extern IMAGE OverRogo;		//ゲームオーバー時のロゴ
extern IMAGE GoalRogo;		//ゴール時のロゴ
extern IMAGE TitleBack;		//タイトル背景
extern IMAGE ClearBack;		//クリア時の背景
extern IMAGE OverBack;		//ゲームオーバー時の背景

extern int EndKind;						//クリアかゲームオーバーか
extern int StartTimeFps;				//測定開始時刻
extern int CountFps;					//カウンタ
extern float CalcFps;					//計算結果
extern int SampleNumFps;				//平均を取るサンプル数
extern int AllKeyState[256];			//すべてのキーの状態が入る
extern int OldAllKeyState[256];			//すべてのキーの状態(直前)が入る
extern CNT precdence[256];				//すべてのキーごとの先行入力のカウント
extern BOOL IsPrecedence[256];			//すべてのキーの先行入力中かどうかの状態が入る
extern int GameScene;					//ゲームシーンを管理
extern int NowStage;					//何ステージ目か
extern MUSIC TitleBGM;			//タイトルの音楽
extern MUSIC Stage1BGM;			//ステージ１の音楽
extern MUSIC GameClearBGM;		//クリアした時の音楽
extern MUSIC GameOverBGM;		//ゲームオーバー時の音楽
extern MUSIC GoalSE;			//ゴールしたときの効果音
extern RECT screen;				//画面の範囲
extern MOVIE PoseIn;			//ポーズ画面に入るときの動画

VOID MY_FPS_UPDATE(VOID);			//FPS値を計測、更新する関数
VOID MY_FPS_DRAW(VOID);				//FPS値を描画する関数
VOID MY_FPS_WAIT(VOID);				//FPS値を計測し、待つ関数

VOID MY_ALL_KEYDOWN_UPDATE(VOID);	//キーの入力状態を更新する
BOOL MY_KEY_DOWN(int);				//キーを押しているか、キーコードで判断する
BOOL MY_KEY_UP(int);				//キーを押し上げたか、キーコードで判断する
BOOL MY_KEY_DOWN_KEEP(int, int);	//キーを押し続けているか、キーコードで判断する
BOOL MY_KEY_DOWN_MOMENT(int);		//キーを押し始めたか、キーコードで判断する
BOOL MY_KEY_DOWN_PRECEDENCE(int,int);	//キーの先行入力用

VOID MY_GAME_INIT(VOID);	//初期化
VOID MY_PLAY_INIT(VOID);    //ステージを始める時の初期化

VOID MY_START(VOID);		//スタート画面
VOID MY_START_PROC(VOID);	//スタート画面の処理
VOID MY_START_DRAW(VOID);	//スタート画面の描画

VOID MY_PLAY(VOID);			//プレイ画面
VOID MY_PLAY_PROC(VOID);	//プレイ画面の処理
VOID MY_PLAY_DRAW(VOID);	//プレイ画面の描画

VOID MY_END(VOID);			//エンド画面
VOID MY_END_PROC(VOID);		//エンド画面の処理
VOID MY_END_DRAW(VOID);		//エンド画面の描画

VOID MY_POSE(VOID);         //ポーズ画面
VOID MY_POSE_PROC(VOID);	//ポーズ画面の処理
VOID MY_POSE_DRAW(VOID);	//ポーズ画面の描画

BOOL MY_LOAD_IMAGE(VOID);		//画像をまとめて読み込む関数
VOID MY_DELETE_IMAGE(VOID);		//画像をまとめて削除する関数

BOOL MY_LOAD_MUSIC(VOID);		//音楽をまとめて読み込む関数
VOID MY_DELETE_MUSIC(VOID);		//音楽をまとめて削除する関数

INT CNT_CHECK(int, int);		//cntをインクリメントして値を返す、CntMaxと同じ値なら0を返す関数

VOID STAGE_SCROLL(VOID);	//スクロールの処理

VOID COLL_PROC(VOID);		//当たり判定の処理
VOID COLL_INIT(VOID);		//当たり判定の初期化

BOOL MY_CHECK_RECT_COLL(RECT, RECT);			//判定同士が重なっているか
INT MY_CHECK_ENEMY_COLL(RECT);					//敵　対　プレイヤーの判定
INT MY_CHECK_MAP1_COLL(RECT, int*, int*);		//マップの障害物　対　敵かプレイヤーの判定
INT MY_CHECK_MAP1_ACT_COLL(RECT, int*, int*);	//マップのアイテム　対　プレイヤーの判定

