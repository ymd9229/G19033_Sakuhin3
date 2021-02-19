#pragma once
#include "main.h"

#define PLAYER_DIV_WIDTH  80
#define PLAYER_DIV_HEIGHT  120
#define PLAYER_DIV_TATE   8
#define PLAYER_DIV_YOKO   3
#define PLAYER_DIV_NUM   PLAYER_DIV_TATE * PLAYER_DIV_YOKO
#define PLAYER_DIV_STOP_R   16
#define PLAYER_DIV_STOP_L   17
#define PLAYER_DIV_SQUAT_R   18
#define PLAYER_DIV_SQUAT_L   19
#define PLAYER_DIV_WALK_R 0
#define PLAYER_DIV_WALK_L 8

#define MAGIC_DIV_WIDTH		80
#define MAGIC_DIV_HEIGHT	80
#define MAGIC_DIV_KIND   4
#define MAGIC_DIV_STATE	 2
#define MAGIC_DIV_TATE   4
#define MAGIC_DIV_YOKO	 1
#define MAGIC_DIV_NUM   MAGIC_DIV_TATE * MAGIC_DIV_YOKO

#define MAGIC_ICON_DIV_WIDTH	80
#define MAGIC_ICON_DIV_HEIGHT	80
#define MAGIC_ICON_DIV_TATE   4
#define MAGIC_ICON_DIV_YOKO	1
#define MAGIC_ICON_DIV_NUM    MAGIC_ICON_DIV_TATE * MAGIC_ICON_DIV_YOKO

#define PLAYER_TAMA_MAX 2
#define PLAYER_MAGIC_MAX 3
#define MAGIC_DATA_MAX 2

#define IMAGE_PLAYER_PATH       TEXT(".\\IMAGE\\player.png")
#define IMAGE_PLAYER_TAMA_PATH  TEXT(".\\IMAGE\\tama.png")
#define IMAGE_PLAYER_HEART_PATH TEXT(".\\IMAGE\\heart.png")
#define IMAGE_MAGIC_ICON_PATH   TEXT(".\\IMAGE\\MagicIcon.png")
#define IMAGE_MAGIC_PATH		TEXT(".\\IMAGE\\magic.png")

enum PLAYER_STATUS	//プレイヤーの状態
{
	PLAYER_STATUS_MOVE_R,	//右に歩いている状態
	PLAYER_STATUS_MOVE_L,	//左に歩いている状態
	PLAYER_STATUS_STOP,		//立ち止まっている状態
	PLAYER_STATUS_JUMP,		//ジャンプしている状態
	PLAYER_STATUS_SQUAT,	//しゃがんでいる状態
};

enum MUKI	//向き
{
	MUKI_R,//右
	MUKI_L,//左
};

enum SPEED_KIND		//様々な速度
{
	ENEMY_SPEED_SLOW = 1,		//遅い敵の速度
	PLAYER_SPEED_NORMAL = 5,	//プレイヤーの通常時の速度
	PLAYER_TAMA_SPEED = 30,		//プレイヤーの通常攻撃の弾の速度
};

typedef struct STRUCT_PLAYER_ATTACK	//プレイヤーの攻撃の構造体
{
	IMAGE image;
	BOOL IsDraw;
	CNT cnt;
	int muki;
	RECT coll;
}PLAYER_ATTACK;

typedef struct STRUCT_LIFE	//体力の構造体
{
	int max;
	int now = max;
	BOOL invincible;
	CNT InvincibleCnt;
	IMAGE image;
}LIFE;

typedef struct STRUCT_PLAYER	//プレイヤーの構造体
{
	int x;
	int y;
	int width;
	int height;
	char path[PATH_MAX];
	int handle[PLAYER_DIV_NUM];
	int CenterX;
	int CenterY;
	int status;
	int muki;
	int EquipMagic;
	LIFE life;
	CNT jump;
	int JumpRise = 10;
	BOOL IsDraw;
	COLL coll;
	CHANGE_IMAGE change;
	BOOL CanMagic = TRUE;
	BOOL CanMove = TRUE;
	BOOL CanAttack = TRUE;
	CNT AttackInterval;
	CNT MagicInterval;
	BOOL CanRightMove = TRUE;
	BOOL CanLeftMove = TRUE;
	int CanJumpTimes = 1;
	int NowJumpTimes = 0;
	BOOL IsMove = FALSE;
	BOOL IsScroll = FALSE;
	PLAYER_ATTACK attack[PLAYER_TAMA_MAX];
}PLAYER;

typedef struct STRUCT_MAGIC	//魔法の構造体
{
	int No;
	int x;
	int y;
	int CenterX;
	int CenterY;
	BOOL IsBuff = FALSE;
	BOOL IsDraw = FALSE;
	int width;
	int height;
	int muki;
	RECT coll;
	CHANGE_IMAGE change;
}MAGIC;

typedef struct STRUCT_MAGIC_DATA	//魔法のデータの構造体
{
	int StartX;
	int StartY;
	int CoolTime;
	BOOL IsBuff = FALSE;
	BOOL fiexd = FALSE;
	char path[PATH_MAX];
	int handle[MAGIC_DIV_NUM];
	int width;
	int height;
	MUSIC SE;
}MAGIC_DATA;

typedef struct STRUCT_MAGIC_ICON	//魔法のアイコンの構造体
{
	char path[PATH_MAX];
	int handle[MAGIC_ICON_DIV_NUM];
	int x;
	int y;
	int width;
	int height;
}MAGIC_ICON;

extern MUSIC AttackSE;			//通常攻撃時の効果音
extern MUSIC HitSE;			//ダメージを受けたときの効果音
extern IMAGE PlayerLife;
extern PLAYER player;
extern MAGIC magic[PLAYER_MAGIC_MAX];
extern MAGIC_DATA MagicData[MAGIC_DATA_MAX];
extern MAGIC_ICON MagicIcon;
extern int JumpBuff;
extern int AvailableMagic;	//使用できる魔法の数
extern int gravity;					//重力
extern CNT FallTime;					//落下している時間をカウントする用
extern int WalkCheckR;					//右に歩いているか調べるよう
extern int WalkCheckL;					//左に歩いているか調べるよう

VOID PLAYER_MOVE(VOID);		//プレイヤーの動きの処理
VOID PLAYER_DRAW(VOID);		//プレイヤーの描画
VOID PLAYER_JUMP(VOID);		//プレイヤーのジャンプの処理
VOID PLAYER_ATTACK_PROC(VOID);	//プレイヤーの通常攻撃の処理
VOID PLAYER_ATTACK_DRAW(VOID);	//プレイヤーの通常攻撃の描画
VOID PLAYER_LIFE_DRAW(VOID);	//プレイヤーのライフの描画

VOID MAGIC_PROC(VOID);			//魔法の処理
VOID MAGIC_DRAW(VOID);			//魔法の描画
VOID MAGIC_SELECT(VOID);		//魔法の選択の処理
INT MAGIC_CHECK(VOID);			//未使用の要素を探す
VOID MAGIC_EFFECT(int);			//魔法ごとの効果の処理
VOID MAGIC_ICON_DRAW(VOID);		//魔法のアイコンの描画

VOID PLAYER_COLL(VOID);
VOID PLAYER_ATTACK_COLL(VOID);
VOID MAGIC_COLL(VOID);

VOID PLAYER_COLL_INIT(VOID);
VOID PLAYER_ATTACK_COLL_INIT(VOID);
VOID MAGIC_COLL_INIT(VOID);

BOOL PLAYER_LOAD_IMAGE(VOID);
VOID PLAYER_DELETE_IMAGE(VOID);
BOOL PLAYER_LOAD_MUSIC(VOID);
VOID PLAYER_DELETE_MUSIC(VOID);