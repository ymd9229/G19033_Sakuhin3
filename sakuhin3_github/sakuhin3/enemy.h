#pragma once
#include "main.h"

#define ENEMY_S_DIV_WIDTH  80		//敵の画像の幅
#define ENEMY_S_DIV_HEIGHT  80		//敵の画像の高さ
#define ENEMY_S_DIV_TATE   4		//敵の画像の縦の分割数
#define ENEMY_S_DIV_YOKO   1		//敵の画像の横の分割数
#define ENEMY_S_DIV_NUM   ENEMY_S_DIV_TATE * ENEMY_S_DIV_YOKO
#define ENEMY_DATA_MAX 9	//敵のデータの数
#define ENEMY_MAX 10		//同時に出現することが出来る敵の数
#define ENEMY_IMAGE_KIND 3	//敵の画像の種類の数

#define IMAGE_ENEMY1_PATH       TEXT(".\\IMAGE\\EnemyImage\\enemy1.png")
#define IMAGE_ENEMY2_PATH       TEXT(".\\IMAGE\\EnemyImage\\enemy2.png")
#define IMAGE_ENEMY3_PATH       TEXT(".\\IMAGE\\EnemyImage\\enemy3.png")

//########構造体########

typedef struct STRUCT_ENEMY		//敵の構造体
{
	double x;
	double y;
	double CenterX;
	double CenterY;
	int speed;
	int muki;
	int kind;
	int width;
	int height;
	CHANGE_IMAGE change;
	COLL coll;
	BOOL IsDraw = FALSE;
	BOOL IsFly;
	BOOL CanRightMove = TRUE;
	BOOL CanLeftMove = TRUE;
}ENEMY;

typedef struct STRUCT_ENEMY_DATA	//敵のデータの構造体
{
	int StartX;
	int StartY;
	int kind;
	int stage;	//登場するステージ
	BOOL IsFly;
	BOOL IsDraw = FALSE;
	char path[PATH_MAX];
	int handle[ENEMY_S_DIV_NUM];
	int width;
	int height;
}ENEMY_DATA;

//########グローバル変数########

extern ENEMY_DATA EnemyData[ENEMY_DATA_MAX];	//敵のデータ
extern ENEMY_DATA EnemyImage[ENEMY_IMAGE_KIND];
extern ENEMY enemy[ENEMY_MAX];

extern MUSIC DefeatSE;			//敵を倒したときの効果音

VOID ENEMY_PROC(VOID);		//敵の処理
VOID ENEMY_DRAW(VOID);		//敵の描画
INT ENEMY_CHECK(VOID);		//敵が出現できる状態か調べる
VOID ENEMY_MOVE(INT);		//敵の動きの処理
VOID ENEMY_COLL(VOID);		//敵の判定の処理
VOID ENEMY_COLL_INIT(VOID);		//敵の判定の初期化

BOOL ENEMY_LOAD_IMAGE(VOID);	//敵の画像を読み込む
VOID ENEMY_DELETE_IMAGE(VOID);	//敵の画像を削除
BOOL ENEMY_LOAD_MUSIC(VOID);	//敵関連の音楽を読み込む
VOID ENEMY_DELETE_MUSIC(VOID);	//敵関連の音楽を削除