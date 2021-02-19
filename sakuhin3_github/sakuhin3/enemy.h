#pragma once
#include "main.h"

#define ENEMY_S_DIV_WIDTH  80
#define ENEMY_S_DIV_HEIGHT  80
#define ENEMY_S_DIV_TATE   4
#define ENEMY_S_DIV_YOKO   1
#define ENEMY_S_DIV_NUM   ENEMY_S_DIV_TATE * ENEMY_S_DIV_YOKO
#define ENEMY_DATA_MAX 5	//敵のデータの数
#define ENEMY_MAX 10		//同時に出現することが出来る敵の数
#define ENEMY_IMAGE_KIND 10	//敵の画像の種類の数

#define IMAGE_ENEMY1_PATH       TEXT(".\\IMAGE\\enemy1.png")
#define IMAGE_ENEMY2_PATH       TEXT(".\\IMAGE\\enemy2.png")

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

extern ENEMY_DATA EnemyData[ENEMY_DATA_MAX];	//敵のデータ
extern ENEMY_DATA EnemyImage[ENEMY_IMAGE_KIND];
extern ENEMY enemy[ENEMY_MAX];

extern MUSIC DefeatSE;			//敵を倒したときの効果音

VOID ENEMY_PROC(VOID);		//敵の処理
VOID ENEMY_DRAW(VOID);		//敵の描画
INT ENEMY_CHECK(VOID);		//
VOID ENEMY_MOVE(INT);		//敵の動きの処理
VOID ENEMY_COLL(VOID);
VOID ENEMY_COLL_INIT(VOID);

BOOL ENEMY_LOAD_IMAGE(VOID);
VOID ENEMY_DELETE_IMAGE(VOID);
BOOL ENEMY_LOAD_MUSIC(VOID);
VOID ENEMY_DELETE_MUSIC(VOID);