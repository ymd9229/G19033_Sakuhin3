#pragma once
#include "main.h"

#define MAP_DIV_WIDTH     80	//一マスの幅
#define MAP_DIV_HEIGHT    80	//一マスの高さ
#define MAP_DIV_TATE      8		//画像の縦の分割数
#define MAP_DIV_YOKO      8		//画像の横の分割数
#define MAP_DIV_NUM  MAP_DIV_TATE * MAP_DIV_YOKO

#define GAME_MAP_TATE_MAX   9	//マップの縦のマスの数
#define GAME_MAP_YOKO_MAX   50	//マップの横のマスの数

#define STAGE_MAX 3		//ステージ数

#define IMAGE_STAGE1_BACK_PATH  TEXT(".\\IMAGE\\BackImage\\Stage1Back.png")
#define IMAGE_STAGE2_BACK_PATH  TEXT(".\\IMAGE\\BackImage\\Stage2Back.png")
#define IMAGE_STAGE3_BACK_PATH  TEXT(".\\IMAGE\\BackImage\\Stage3Back.png")
#define IMAGE_MAP1_PATH         TEXT(".\\IMAGE\\map1.png")

enum GAME_MAP_KIND
{
	aa = 0, ab = 1, ac = 2, ad = 3, ae = 4, af = 5, ag = 6, ah = 7,
	ba = 8, bb = 9, bc = 10, bd = 11, be = 12,bf = 13,
	ga = 48,
	ha = 56,
};

enum CHECK_MAP_COLL		//マップの判定ごとの種類
{
	BLOCK,		//ブロック（障害物）
	AIR,		//空気（判定無し）
	GOAL,		//ゴール地点
	BOOK,		//魔法の本
	WOOD,		//木
	MOVE_BLOCK	//動くブロック
};

//########構造体########

typedef struct STRUCT_MAP_IMAGE	//マップチップの構造体
{
	char path[PATH_MAX];
	int handle[MAP_DIV_NUM];
	int kind[MAP_DIV_NUM];
	int x;
	int y;
	int width;
	int height;
}MAPCHIP;

typedef struct STRUCT_MAP	//マップの構造体
{
	GAME_MAP_KIND kind;
	int x;
	int y;
	int width;
	int height;
	RECT coll;
}MAP;

typedef struct STRUCT_MAGIC_BOOK
{
	IMAGE image;
	BOOL IsDraw;
}MAGIC_BOOK;

//########グローバル変数########

//マップのデータ
extern GAME_MAP_KIND stage1Data[GAME_MAP_TATE_MAX][GAME_MAP_YOKO_MAX];
extern GAME_MAP_KIND stage2Data[GAME_MAP_TATE_MAX][GAME_MAP_YOKO_MAX];
extern GAME_MAP_KIND stage3Data[GAME_MAP_TATE_MAX][GAME_MAP_YOKO_MAX];

extern IMAGE StageBack[STAGE_MAX];
extern MAPCHIP mapchip;
extern MAP stage[GAME_MAP_TATE_MAX][GAME_MAP_YOKO_MAX];
extern MAGIC_BOOK book;

VOID STAGE_INIT(VOID);			//ステージの初期化
VOID STAGE_DRAW(VOID);			//ステージの描画
VOID STAGE_COLL_INIT(VOID);		//判定の初期化
BOOL MAP_LOAD_IMAGE(VOID);		//マップの画像を読み込む
VOID MAP_DELETE_IMAGE(VOID);	//マップの画像の削除
BOOL MAP_LOAD_MUSIC(VOID);		//マップ関連の音楽を読み込む
VOID MAP_DELETE_MUSIC(VOID);	//マップ関連の音楽の削除