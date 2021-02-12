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

#define ENEMY_S_DIV_WIDTH  80
#define ENEMY_S_DIV_HEIGHT  80
#define ENEMY_S_DIV_TATE   4
#define ENEMY_S_DIV_YOKO   1
#define ENEMY_S_DIV_NUM   ENEMY_S_DIV_TATE * ENEMY_S_DIV_YOKO
#define ENEMY_DATA_MAX 2	//敵のデータの数
#define ENEMY_MAX 10		//同時に出現することが出来る敵の数
#define ENEMY_IMAGE_KIND 10	//敵の画像の種類の数

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

#define MAP_DIV_WIDTH     80
#define MAP_DIV_HEIGHT    80
#define MAP_DIV_TATE      8
#define MAP_DIV_YOKO      8
#define MAP_DIV_NUM  MAP_DIV_TATE * MAP_DIV_YOKO

#define GAME_MAP_TATE_MAX   9	//マップの縦のマスの数
#define GAME_MAP_YOKO_MAX   32	//マップの横のマスの数
#define GAME_MAP_KIND_MAX  2

#define PLAYER_TAMA_MAX 2
#define PLAYER_MAGIC_MAX 3
#define MAGIC_DATA_MAX 2


#define STAGE_MAX 1	//ステージ数

#define IMAGE_PLAYER_PATH       TEXT(".\\IMAGE\\player.png")
#define IMAGE_PLAYER_TAMA_PATH  TEXT(".\\IMAGE\\tama.png")
#define IMAGE_PLAYER_HEART_PATH TEXT(".\\IMAGE\\heart.png")
#define IMAGE_TITLE_BACK_PATH   TEXT(".\\IMAGE\\TitleBack.png")
#define IMAGE_TITLE_ROGO_PATH   TEXT(".\\IMAGE\\TitleRogo.png")
#define IMAGE_CLEAR_ROGO_PATH   TEXT(".\\IMAGE\\ClearRogo.png")
#define IMAGE_OVER_ROGO_PATH    TEXT(".\\IMAGE\\OverRogo.png")
#define IMAGE_CLEAR_BK_PATH		TEXT(".\\IMAGE\\gameclearBK.png")
#define IMAGE_OVER_BK_PATH		TEXT(".\\IMAGE\\gameoverBK.png")
#define IMAGE_STAGE1_BACK_PATH  TEXT(".\\IMAGE\\Stage1Back.png")
#define IMAGE_MAP1_PATH         TEXT(".\\IMAGE\\map1.png")
#define IMAGE_ENEMY1_PATH       TEXT(".\\IMAGE\\enemy1.png")
#define IMAGE_ENEMY2_PATH       TEXT(".\\IMAGE\\enemy2.png")
#define IMAGE_BOOK_PATH       　TEXT(".\\IMAGE\\book.png")
#define IMAGE_MAGIC_ICON_PATH   TEXT(".\\IMAGE\\MagicIcon.png")
#define IMAGE_MAGIC_PATH		TEXT(".\\IMAGE\\magic.png")


#define MUSIC_TITLE_BGM_PATH    TEXT(".\\MUSIC\\bgm_maoudamashii_healing01.mp3")
#define MUSIC_STAGE1_BGM_PATH	TEXT(".\\MUSIC\\bgm_maoudamashii_piano25.mp3")
#define MUSIC_CLEAR_BGM_PATH	TEXT(".\\MUSIC\\game_maoudamashii_7_event37.mp3")

#define MUSIC_ATTACK_SE_PATH    TEXT(".\\MUSIC\\se_maoudamashii_retro16.mp3")
#define MUSIC_DEFEAT_SE_PATH    TEXT(".\\MUSIC\\se_maoudamashii_retro04.mp3")
#define MUSIC_GOAL_SE_PATH		TEXT(".\\MUSIC\\se_maoudamashii_onepoint09.mp3")
#define MUSIC_HIT_SE_PATH		TEXT(".\\MUSIC\\se_maoudamashii_battle07.mp3")

#define MOVIE_POSE_IN_PATH      TEXT(".\\MOVIE\\pose.mp4")


enum GAME_MAP_KIND
{
	aa = 0,ab = 1,ac = 2,ad = 3,ae = 4,af = 5,ag = 6,ah = 7,
	ba = 8,bb = 9,bc = 10,bd = 11,be = 12,
	ga = 48,
	ha = 56,
};

enum END_TYPE	//エンド画面の種類
{
	GAME_CLEAR,//クリア
	GAME_OVER,//ゲームオーバー
};

enum GAME_SCENE	//シーンの種類 
{
	GAME_SCENE_START,	//スタート画面
	GAME_SCENE_PLAY,	//プレイ画面
	GAME_SCENE_END,		//エンド画面
	GAME_SCENE_POSE		//ポーズ画面
};

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

enum CHECK_MAP_COLL		//マップの判定ごとの種類
{
	BLOCK,//ブロック（障害物）
	AIR,	//空気（判定無し）
	GOAL,	//ゴール地点
	BOOK,	//魔法の本
	WOOD,
};

enum SPEED_KIND		//様々な速度
{
	ENEMY_SPEED_SLOW = 1,		//遅い敵の速度
	PLAYER_SPEED_NORMAL = 5,	//プレイヤーの通常時の速度
	PLAYER_TAMA_SPEED = 30,		//プレイヤーの通常攻撃の弾の速度
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
	BOOL IsViewed = FALSE;
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

typedef struct STRUCT_COLL	//判定の構造体
{
	RECT base;
	RECT CheckItem;
	RECT CheckLeft;
	RECT CheckRight;
	RECT CheckTop;
	RECT CheckBottom;
}COLL;

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
	BOOL IsFly;
	BOOL IsDraw = FALSE;
	char path[PATH_MAX];
	int handle[ENEMY_S_DIV_NUM];
	int width;
	int height;
}ENEMY_DATA;

//########グローバル変数########

ENEMY_DATA EnemyData[ENEMY_DATA_MAX] = {
	//｛敵の出現位置X、敵の出現位置Y、敵の種類、飛行するかどうか｝
	{700,480,0,FALSE},
	{1000,240,1,TRUE},
};	//敵のデータ
ENEMY_DATA EnemyImage[ENEMY_IMAGE_KIND];
ENEMY enemy[ENEMY_MAX];
IMAGE TitleBack;		//タイトル背景
IMAGE TitleRogo;		//タイトルロゴ
IMAGE ClearRogo;		//クリア時のロゴ
IMAGE OverRogo;			//ゲームオーバー時のロゴ
IMAGE ClearBack;		//クリア時の背景
IMAGE OverBack;			//ゲームオーバー時の背景
IMAGE PlayerLife;
IMAGE StageBack[STAGE_MAX];
MAPCHIP mapchip;
MAP stage1[GAME_MAP_TATE_MAX][GAME_MAP_YOKO_MAX];
PLAYER player;
MAGIC magic[PLAYER_MAGIC_MAX];
MAGIC_DATA MagicData[MAGIC_DATA_MAX] = {
	{0,0,0,FALSE,FALSE,},
	{0,0,0,TRUE,FALSE,},
};
MAGIC_ICON MagicIcon;
int EndKind;                    //クリアかゲームオーバーか
int StartTimeFps;				//測定開始時刻
int CountFps;					//カウンタ
float CalcFps;					//計算結果
int SampleNumFps = GAME_FPS;	//平均を取るサンプル数
int AllKeyState[256] = { 0 };		//すべてのキーの状態が入る
int OldAllKeyState[256] = { 0 };	//すべてのキーの状態(直前)が入る
BOOL IsPrecedence[256];
int GameScene;					//ゲームシーンを管理
int stage;						//何ステージ目か
int gravity;					//重力
CNT FallTime;					//落下している時間をカウントする用
CNT precdence[256];
int WalkCheckR;					//右に歩いているか調べるよう
int WalkCheckL;					//左に歩いているか調べるよう
int JumpBuff = 0;
int AvailableMagic = 0;	//使用できる魔法の数
MUSIC TitleBGM;			//タイトルの音楽
MUSIC Stage1BGM;		//ステージ１の音楽
MUSIC GameClearBGM;		//クリアした時の音楽
MUSIC AttackSE;			//通常攻撃時の効果音
MUSIC DefeatSE;			//敵を倒したときの効果音
MUSIC GoalSE;			//ゴールしたときの効果音
MUSIC HitSE;			//ダメージを受けたときの効果音
RECT screen;
MOVIE PoseIn;			//ポーズ画面に入るときの動画


GAME_MAP_KIND stage1Data[GAME_MAP_TATE_MAX][GAME_MAP_YOKO_MAX]{
	ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,af,ae,ae,ae,ae,ae,ae,ae,
	ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,af,ae,ae,ae,ae,ae,ae,ae,
	ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,af,ae,ae,ae,ae,ae,ae,ae,
	ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,af,ae,ae,ae,ae,ae,ae,ae,
	ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,af,af,af,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,af,ae,ae,ae,ae,ae,ae,ae,
	ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ag,
	aa,ae,ae,ae,ac,ae,ac,ae,ae,ae,ae,ae,ae,ae,ae,ad,ae,ha,ae,ha,ae,ae,ae,ae,ga,ae,ae,ae,ae,ae,ae,ad,
	aa,ab,ac,ab,bb,ae,bb,ab,ac,ab,ac,ab,ac,ab,ac,bc,ac,ab,ac,ab,ac,ab,ac,ab,ac,ab,ac,ab,ac,ab,ac,bd,
	ba,bb,bc,bb,bc,ae,bc,bb,bc,bb,bc,bb,bc,bb,bc,bb,bc,bb,bc,bb,bc,bb,bc,bb,bc,bb,bc,bb,bc,bb,bc,bd,
};

VOID MY_FPS_UPDATE(VOID);			//FPS値を計測、更新する関数
VOID MY_FPS_DRAW(VOID);				//FPS値を描画する関数
VOID MY_FPS_WAIT(VOID);				//FPS値を計測し、待つ関数

VOID MY_ALL_KEYDOWN_UPDATE(VOID);	//キーの入力状態を更新する
BOOL MY_KEY_DOWN(int);				//キーを押しているか、キーコードで判断する
BOOL MY_KEY_UP(int);				//キーを押し上げたか、キーコードで判断する
BOOL MY_KEY_DOWN_KEEP(int, int);	//キーを押し続けているか、キーコードで判断する
BOOL MY_KEY_DOWN_MOMENT(int);		//キーを押し始めたか、キーコードで判断する
BOOL MY_KEY_DOWN_PRECEDENCE(int);	//キーの先行入力用

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

VOID PLAYER_MOVE(VOID);		//プレイヤーの動きの処理
VOID PLAYER_DRAW(VOID);		//プレイヤーの描画
VOID PLAYER_JUMP(VOID);		//プレイヤーのジャンプの処理
VOID PLAYER_ATTACK_PROC(VOID);	//プレイヤーの通常攻撃の処理
VOID PLAYER_ATTACK_DRAW(VOID);	//プレイヤーの通常攻撃の描画
VOID PLAYER_LIFE_DRAW(VOID);

VOID STAGE_SCROLL(VOID);	//スクロールの処理

VOID COLL_PROC(VOID);		//当たり判定の処理
BOOL MY_CHECK_RECT_COLL(RECT, RECT);	
INT MY_CHECK_ENEMY_COLL(RECT);
INT MY_CHECK_MAP1_COLL(RECT,int*,int*);		//マップの障害物　対　敵かプレイヤーの判定
INT MY_CHECK_MAP1_ACT_COLL(RECT);			//マップのアイテム　対　プレイヤーの判定

VOID STAGE_DRAW(VOID);

VOID ENEMY_PROC(VOID);		//敵の処理
VOID ENEMY_DRAW(VOID);		//敵の描画
INT ENEMY_CHECK(VOID);		//
VOID ENEMY_MOVE(INT);		//敵の動きの処理

VOID MAGIC_PROC(VOID);			//魔法の処理
VOID MAGIC_DRAW(VOID);			//魔法の描画
VOID MAGIC_SELECT(VOID);		//魔法の選択の処理
INT MAGIC_CHECK(VOID);			//未使用の要素を探す
VOID MAGIC_EFFECT(int);			//魔法ごとの効果の処理
VOID MAGIC_ICON_DRAW(VOID);		//魔法のアイコンの描画

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	ChangeWindowMode(TRUE);			//ウィンドウモードに設定
	SetGraphMode(GAME_WIDTH, GAME_HEIGHT, GAME_COLOR);	//指定の数値でウィンドウを表示する
	SetWindowStyleMode(GAME_WINDOW_BAR);	//タイトルバーはデフォルトにする
	SetMainWindowText(TEXT(GAME_WINDOW_NAME));	//ウィンドウのタイトルの文字

	if (DxLib_Init() == -1) { return -1; }	//ＤＸライブラリ初期化処理
	SetDrawScreen(DX_SCREEN_BACK);	//Draw系関数は裏画面に描画
	if (MY_LOAD_IMAGE() == FALSE) { return -1; }
	if (MY_LOAD_MUSIC() == FALSE) { return -1; }
	while (TRUE)
	{
		if (ProcessMessage() != 0) { break; }	//メッセージ処理の結果がエラーのとき、強制終了

		if (ClearDrawScreen() != 0) { break; }	//画面を消去できなかったとき、強制終了

		MY_ALL_KEYDOWN_UPDATE();				//押しているキー状態を取得
		MY_FPS_UPDATE();	//FPSの処理[更新]

		switch (GameScene)
		{
		case GAME_SCENE_START:
			MY_START();	//スタート画面
			break;
		case GAME_SCENE_PLAY:
			MY_PLAY();	//プレイ画面
			break;
		case GAME_SCENE_END:
			MY_END();	//エンド画面
			break;
		case GAME_SCENE_POSE:
			MY_POSE();  //ポーズ画面
			break;
		}

		ScreenFlip();		//モニタのリフレッシュレートの速さで裏画面を再描画
	}

	MY_DELETE_IMAGE();
	MY_DELETE_MUSIC();
	DxLib_End();	//ＤＸライブラリ使用の終了処理

	return 0;
}

VOID MY_FPS_UPDATE(VOID)
{
	if (CountFps == 0) //1フレーム目なら時刻を記憶
	{
		StartTimeFps = GetNowCount();
	}

	if (CountFps == SampleNumFps) //60フレーム目なら平均を計算
	{
		int now = GetNowCount();
		//現在の時間から、0フレーム目の時間を引き、FPSの数値で割る＝1FPS辺りの平均時間が計算される
		CalcFps = 1000.f / ((now - StartTimeFps) / (float)SampleNumFps);
		CountFps = 0;
		StartTimeFps = now;
	}
	CountFps++;
	return;
}

VOID MY_FPS_DRAW(VOID)
{
	//文字列を描画
	DrawFormatString(0, GAME_HEIGHT - 20, GetColor(255, 255, 255), "FPS:%.1f", CalcFps);
	return;
}

VOID MY_FPS_WAIT(VOID)
{
	int resultTime = GetNowCount() - StartTimeFps;					//かかった時間
	int waitTime = CountFps * 1000 / GAME_FPS - resultTime;	//待つべき時間

	if (waitTime > 0)		//指定のFPS値よりも早い場合
	{
		WaitTimer(waitTime);	//待つ
	}
	return;
}

VOID MY_ALL_KEYDOWN_UPDATE(VOID)
{
	char TempKey[256];	
	
	for (int i = 0; i < 256; i++)
	{
		OldAllKeyState[i] = AllKeyState[i];
	}

	GetHitKeyStateAll(TempKey); // 全てのキーの入力状態を得る

	for (int i = 0; i < 256; i++)
	{
		if (TempKey[i] != 0)	//押されているキーのキーコードを押しているとき
		{
			AllKeyState[i]++;	//押されている
		}
		else
		{
			AllKeyState[i] = 0;	//押されていない
		}
	}
	return;
}


BOOL MY_KEY_DOWN(int KEY_INPUT_)
{
	//キーコードのキーを押している時
	if (AllKeyState[KEY_INPUT_] != 0)
	{
		return TRUE;	//キーを押している
	}
	else
	{
		return FALSE;	//キーを押していない
	}
}


BOOL MY_KEY_UP(int KEY_INPUT_)
{
	if (OldAllKeyState[KEY_INPUT_] >= 1	
		&& AllKeyState[KEY_INPUT_] == 0)	
	{
		return TRUE;	
	}
	else
	{
		return FALSE;	
	}
}


BOOL MY_KEY_DOWN_KEEP(int KEY_INPUT_, int DownTime)
{
	int UpdateTime = DownTime * GAME_FPS;

	if (AllKeyState[KEY_INPUT_] > UpdateTime)
	{
		return TRUE;	//押し続けている
	}
	else
	{
		return FALSE;	//押し続けていない
	}
}

BOOL MY_KEY_DOWN_MOMENT(int KEY_INPUT_)
{
	if (MY_KEY_DOWN(KEY_INPUT_) == TRUE)
	{
		if (OldAllKeyState[KEY_INPUT_] == 0)
		{
			return TRUE;	//押し始めた
		}
	}
	return FALSE;	//押し始めていない
}

BOOL MY_KEY_DOWN_PRECEDENCE(int key)
{
	if (IsPrecedence[key] == TRUE)
	{
		precdence[key].CntMax = 30;
		precdence[key].cnt = CNT_CHECK(precdence[key].cnt, precdence[key].CntMax);
		if (precdence[key].cnt != 0)
		{
			return TRUE;
		}
		else
		{
			IsPrecedence[key] = FALSE;
			return FALSE;
		}
	}
	return FALSE;
}

VOID MY_START(VOID)
{
	MY_START_PROC();	//スタート画面の処理
	MY_START_DRAW();	//スタート画面の描画

	return;
}

//スタート画面の処理
VOID MY_START_PROC(VOID)
{
	if (CheckSoundMem(TitleBGM.handle) == 0)
	{
		ChangeVolumeSoundMem(255 * 50 / 100, TitleBGM.handle);	//50%の音量にする

		//BGMを流す
		//DX_PLAYTYPE_NORMAL:　ノーマル再生
		//DX_PLAYTYPE_BACK  : バックグラウンド再生
		//DX_PLAYTYPE_LOOP  : ループ再生
		PlaySoundMem(TitleBGM.handle, DX_PLAYTYPE_LOOP);
	}
	//エンターキーを押したら、プレイシーンへ移動する
	if (MY_KEY_DOWN(KEY_INPUT_RETURN) == TRUE)
	{
		if (CheckSoundMem(TitleBGM.handle) != 0)
		{
			StopSoundMem(TitleBGM.handle);	//BGMを止める
		}
		//初期化
		player.IsDraw = TRUE;

		player.x = 1;
		player.y = 0;
		player.CenterX = player.x + player.width / 2;
		player.CenterY = player.y + player.height / 2;
		player.life.max = 3;
		player.life.now = player.life.max;
		player.EquipMagic = -1;
		player.life.InvincibleCnt.CntMax = 180;
		player.life.InvincibleCnt.cnt = 0;
		stage = 1;

		gravity = 10;

		player.AttackInterval.CntMax = 40;
		player.jump.CntMax = 20;
		FallTime.CntMax = 10;
		player.change.CntMax = 20;
		player.MagicInterval.CntMax = 60;
		player.CanJumpTimes = 1;
		player.NowJumpTimes = 0;
		screen.left = 0;
		screen.right = GAME_WIDTH;
		screen.top = 0;
		screen.bottom = GAME_HEIGHT;

		for (int m = 0; m < ENEMY_DATA_MAX; m++)
		{
			EnemyData[m].IsDraw = FALSE;
		}
		for (int n = 0; n < ENEMY_MAX; n++)
		{
			enemy[n].IsDraw = FALSE;
			enemy[n].CanLeftMove = TRUE;
			enemy[n].CanRightMove = FALSE;
		}

		for (int tate = 0; tate < GAME_MAP_TATE_MAX; tate++)
		{
			for (int yoko = 0; yoko < GAME_MAP_YOKO_MAX; yoko++)
			{
				//stage1[tate][yoko].kind = stage1Data[tate][yoko];
				stage1[tate][yoko].width = mapchip.width;
				stage1[tate][yoko].height = mapchip.height;
				stage1[tate][yoko].x = yoko * stage1[tate][yoko].width;
				stage1[tate][yoko].y = tate * stage1[tate][yoko].height;
			}
		}
		//画面右上に魔法のアイコン
		MagicIcon.x = GAME_WIDTH - MagicIcon.width;
		MagicIcon.y = 0;

		GameScene = GAME_SCENE_PLAY;
	}
	

	return;
}

//スタート画面の描画
VOID MY_START_DRAW(VOID)
{
	DrawGraph(0, 0, TitleBack.handle, true);
	DrawGraph(GAME_WIDTH / 2 - TitleRogo.width / 2,
			  GAME_HEIGHT / 2 - TitleRogo.height / 2,
			  TitleRogo.handle, true);
	DrawString(0, 0, "スタート画面(エンターキーを押して下さい)", GetColor(255, 255, 255));
	return;
}

//プレイ画面
VOID MY_PLAY(VOID)
{
	MY_PLAY_PROC();	//プレイ画面の処理
	MY_PLAY_DRAW();	//プレイ画面の描画

	return;
}

//プレイ画面の処理
VOID MY_PLAY_PROC(VOID)
{
	if (CheckSoundMem(Stage1BGM.handle) == 0)
	{
		ChangeVolumeSoundMem(255 * 50 / 100, Stage1BGM.handle);	//50%の音量にする
		PlaySoundMem(Stage1BGM.handle, DX_PLAYTYPE_LOOP);
	}
	PLAYER_MOVE();		//プレイヤーの動きの処理
	ENEMY_PROC();		//敵の処理
	STAGE_SCROLL();		//ステージのスクロールの処理
	COLL_PROC();		//判定の処理

	if (MY_KEY_DOWN_MOMENT(KEY_INPUT_TAB) == TRUE)
	{
		PoseIn.IsViewed = FALSE;
		GameScene = GAME_SCENE_POSE;
	}

	if (player.y > GAME_HEIGHT)
	{
		EndKind = GAME_OVER;
		GameScene = GAME_SCENE_END;
	}

	if (MY_KEY_DOWN(KEY_INPUT_SPACE) == TRUE)
	{
		if (CheckSoundMem(Stage1BGM.handle) != 0)
		{
			StopSoundMem(Stage1BGM.handle);	//BGMを止める
		}
		GameScene = GAME_SCENE_END;
	}

	return;
}

//プレイ画面の描画
VOID MY_PLAY_DRAW(VOID)
{
	DrawGraph(0, 0, StageBack[stage-1].handle, true);
	PLAYER_DRAW();
	STAGE_DRAW();
	MAGIC_ICON_DRAW();
	PLAYER_LIFE_DRAW();
	DrawBox(player.coll.base.left, player.coll.base.top, player.coll.base.right, player.coll.base.bottom, GetColor(0, 0, 255), FALSE);
	DrawBox(player.coll.CheckRight.left, player.coll.CheckRight.top,
		player.coll.CheckRight.right, player.coll.CheckRight.bottom,
		GetColor(255, 0, 0), FALSE);
	DrawBox(player.coll.CheckLeft.left, player.coll.CheckLeft.top,
		player.coll.CheckLeft.right, player.coll.CheckLeft.bottom,
		GetColor(0, 255, 0), FALSE);
	PLAYER_ATTACK_DRAW();
	MAGIC_DRAW();
	ENEMY_DRAW();

	return;
}


VOID MY_END(VOID)
{
	MY_END_PROC();
	MY_END_DRAW();
	return;
}

//エンド画面の処理
VOID MY_END_PROC(VOID)
{
	if (CheckSoundMem(Stage1BGM.handle) != 0)
	{
		StopSoundMem(Stage1BGM.handle);
	}
	switch (EndKind)
	{
	case GAME_CLEAR:
		if (CheckSoundMem(GameClearBGM.handle) == 0)
		{
			ChangeVolumeSoundMem(255 * 50 / 100, GameClearBGM.handle);
			PlaySoundMem(GameClearBGM.handle, DX_PLAYTYPE_LOOP);
		}
		break;
	case GAME_OVER:
		break;
	}
	if (MY_KEY_DOWN(KEY_INPUT_ESCAPE) == TRUE)
	{
		if (CheckSoundMem(GameClearBGM.handle) != 0)
		{
			StopSoundMem(GameClearBGM.handle);	//BGMを止める
		}
		GameScene = GAME_SCENE_START;
	}
	return;
}


VOID MY_END_DRAW(VOID)
{
	switch (EndKind)
	{
	case GAME_CLEAR:
		DrawGraph(0, 0, ClearBack.handle, true);
		DrawGraph(GAME_WIDTH / 2 - ClearRogo.width / 2, ClearRogo.height, ClearRogo.handle, true);
		break;
	case GAME_OVER:
		DrawGraph(0, 0, OverBack.handle, true);
		DrawGraph(GAME_WIDTH / 2 - OverRogo.width / 2, OverRogo.height, OverRogo.handle, true);
		break;
	}

	DrawString(0, 0, "エンド画面(エスケープキーを押して下さい)", GetColor(255, 255, 255));
	return;
}

VOID MY_POSE(VOID)
{
	MY_POSE_PROC();
	MY_POSE_DRAW();
	return;
}

VOID MY_POSE_PROC(VOID)
{
	if (GetMovieStateToGraph(PoseIn.handle) == 1)
	{
		PoseIn.IsViewed = TRUE;
	}
	if (MY_KEY_DOWN_MOMENT(KEY_INPUT_TAB) == TRUE)
	{
		PauseMovieToGraph(PoseIn.handle);
		GameScene = GAME_SCENE_PLAY;
	}
	return;
}

VOID MY_POSE_DRAW(VOID)		//ポーズ画面の描画
{
	if (GetMovieStateToGraph(PoseIn.handle) == 0 &&
		PoseIn.IsViewed == FALSE)
	{
		SeekMovieToGraph(PoseIn.handle, 0);
		PlayMovieToGraph(PoseIn.handle);
	}
	DrawGraph(0, 0, PoseIn.handle, FALSE);
	DrawString(0, 0, "ポーズ画面", GetColor(255, 255, 255));
	return;
}

BOOL MY_LOAD_IMAGE(VOID)
{
	strcpy_s(TitleRogo.path, IMAGE_TITLE_ROGO_PATH);		
	TitleRogo.handle = LoadGraph(TitleRogo.path);
	GetGraphSize(TitleRogo.handle, &TitleRogo.width, &TitleRogo.height);

	strcpy_s(TitleBack.path, IMAGE_TITLE_BACK_PATH);
	TitleBack.handle = LoadGraph(TitleBack.path);

	strcpy_s(ClearRogo.path, IMAGE_CLEAR_ROGO_PATH);
	ClearRogo.handle = LoadGraph(ClearRogo.path);
	GetGraphSize(ClearRogo.handle, &ClearRogo.width, &ClearRogo.height);

	strcpy_s(OverRogo.path, IMAGE_OVER_ROGO_PATH);
	OverRogo.handle = LoadGraph(OverRogo.path);
	GetGraphSize(OverRogo.handle, &OverRogo.width, &OverRogo.height);

	strcpy_s(ClearBack.path, IMAGE_CLEAR_BK_PATH);
	ClearBack.handle = LoadGraph(ClearBack.path);

	strcpy_s(OverBack.path, IMAGE_OVER_BK_PATH);
	OverBack.handle = LoadGraph(OverBack.path);

	strcpy_s(PlayerLife.path, IMAGE_PLAYER_HEART_PATH);
	PlayerLife.handle = LoadGraph(PlayerLife.path);
	GetGraphSize(PlayerLife.handle, &PlayerLife.width, &PlayerLife.height);

	strcpy_s(player.attack[0].image.path, IMAGE_PLAYER_TAMA_PATH);
	player.attack[0].image.handle = LoadGraph(player.attack[0].image.path);
	GetGraphSize(player.attack[0].image.handle, &player.attack[0].image.width, &player.attack[0].image.height);

	for (int i = 0; i < STAGE_MAX; i++)
	{
		switch (i)
		{
		case 0:
			strcpy_s(StageBack[i].path, IMAGE_STAGE1_BACK_PATH);
			StageBack[i].handle = LoadGraph(StageBack[i].path);
			break;
		}
		
	}

	LoadDivGraph(
		IMAGE_PLAYER_PATH,
		PLAYER_DIV_NUM, PLAYER_DIV_TATE, PLAYER_DIV_YOKO,
		PLAYER_DIV_WIDTH, PLAYER_DIV_HEIGHT,
		&player.handle[0]);
	GetGraphSize(player.handle[0], &player.width, &player.height);

	LoadDivGraph(
		IMAGE_MAP1_PATH,
		MAP_DIV_NUM, MAP_DIV_TATE, MAP_DIV_YOKO,
		MAP_DIV_WIDTH, MAP_DIV_HEIGHT,
		&mapchip.handle[0]);
	GetGraphSize(mapchip.handle[0], &mapchip.width, &mapchip.height);

	for (int tate = 0; tate < GAME_MAP_TATE_MAX; tate++)
	{
		for (int yoko = 0; yoko < GAME_MAP_YOKO_MAX; yoko++)
		{
			stage1[tate][yoko].kind = stage1Data[tate][yoko];
			stage1[tate][yoko].width = mapchip.width;
			stage1[tate][yoko].height = mapchip.height;
			stage1[tate][yoko].x = yoko * stage1[tate][yoko].width;
			stage1[tate][yoko].y = tate * stage1[tate][yoko].height;
		}
	}

	for (int k = 0; k < ENEMY_IMAGE_KIND; k++)
	{
		switch (k)
		{
		case 0:
			LoadDivGraph(
				IMAGE_ENEMY1_PATH,
				ENEMY_S_DIV_NUM, ENEMY_S_DIV_TATE, ENEMY_S_DIV_YOKO,
				ENEMY_S_DIV_WIDTH, ENEMY_S_DIV_HEIGHT,
				&EnemyImage[k].handle[0]);
			GetGraphSize(EnemyImage[k].handle[0], &EnemyImage[k].width, &EnemyImage[k].height);
			break;
		case 1:
			LoadDivGraph(
				IMAGE_ENEMY2_PATH,
				ENEMY_S_DIV_NUM, ENEMY_S_DIV_TATE, ENEMY_S_DIV_YOKO,
				ENEMY_S_DIV_WIDTH, ENEMY_S_DIV_HEIGHT,
				&EnemyImage[k].handle[0]);
			GetGraphSize(EnemyImage[k].handle[0], &EnemyImage[k].width, &EnemyImage[k].height);
			break;
		}
	}
	for (int m = 0; m < ENEMY_DATA_MAX; m++)
	{
		EnemyData[m].width = EnemyImage[EnemyData[m].kind].width;
		EnemyData[m].height = EnemyImage[EnemyData[m].kind].height;
	}

	strcpy_s(PoseIn.path, MOVIE_POSE_IN_PATH);
	PoseIn.handle = LoadGraph(PoseIn.path);

	LoadDivGraph(
		IMAGE_MAGIC_ICON_PATH,
		MAGIC_ICON_DIV_NUM, MAGIC_ICON_DIV_TATE, MAGIC_ICON_DIV_YOKO,
		MAGIC_ICON_DIV_WIDTH, MAGIC_ICON_DIV_HEIGHT,
		&MagicIcon.handle[0]);
	GetGraphSize(MagicIcon.handle[0], &MagicIcon.width, &MagicIcon.height);

	LoadDivGraph(
		IMAGE_MAGIC_PATH,
		MAGIC_DIV_NUM, MAGIC_DIV_TATE, MAGIC_DIV_YOKO,
		MAGIC_DIV_WIDTH, MAGIC_DIV_HEIGHT,
		&MagicData[0].handle[0]);
	GetGraphSize(MagicData[0].handle[0], &MagicData[0].width, &MagicData[0].height);

	return TRUE;
}

VOID MY_DELETE_IMAGE(VOID)
{
	DeleteGraph(TitleRogo.handle);
	DeleteGraph(TitleBack.handle);
	DeleteGraph(ClearRogo.handle);
	DeleteGraph(OverRogo.handle);
	
	for (int i = 0; i < STAGE_MAX; i++)
	{
		DeleteGraph(StageBack[i].handle);
	}
	DeleteGraph(player.handle[0]);
	DeleteGraph(player.attack[0].image.handle);
	for (int k = 0; k < ENEMY_IMAGE_KIND; k++)
	{
		DeleteGraph(EnemyImage[k].handle[0]);
	}
	DeleteGraph(PoseIn.handle);
	return;
}

BOOL MY_LOAD_MUSIC(VOID)
{
	strcpy_s(TitleBGM.path, MUSIC_TITLE_BGM_PATH);		
	TitleBGM.handle = LoadSoundMem(TitleBGM.path);
	strcpy_s(Stage1BGM.path, MUSIC_STAGE1_BGM_PATH);		
	Stage1BGM.handle = LoadSoundMem(Stage1BGM.path);
	strcpy_s(GameClearBGM.path, MUSIC_CLEAR_BGM_PATH);
	GameClearBGM.handle = LoadSoundMem(GameClearBGM.path);
	strcpy_s(AttackSE.path, MUSIC_ATTACK_SE_PATH);
	AttackSE.handle = LoadSoundMem(AttackSE.path);
	strcpy_s(DefeatSE.path, MUSIC_DEFEAT_SE_PATH);
	DefeatSE.handle = LoadSoundMem(DefeatSE.path);
	strcpy_s(GoalSE.path, MUSIC_GOAL_SE_PATH);
	GoalSE.handle = LoadSoundMem(GoalSE.path);
	strcpy_s(HitSE.path, MUSIC_HIT_SE_PATH);
	HitSE.handle = LoadSoundMem(HitSE.path);
	return TRUE;
}

VOID MY_DELETE_MUSIC(VOID)
{
	DeleteSoundMem(TitleBGM.handle);
	DeleteSoundMem(Stage1BGM.handle);
	DeleteSoundMem(GameClearBGM.handle);
	DeleteSoundMem(AttackSE.handle);
	DeleteSoundMem(DefeatSE.handle);
	DeleteSoundMem(GoalSE.handle);
	DeleteSoundMem(HitSE.handle);
}

INT CNT_CHECK(int cnt, int CntMax)
{
	if (cnt == CntMax)
	{
		return 0;
	}
	cnt++;
	return cnt;
}

VOID STAGE_DRAW(VOID)
{
	for (int tate = 0; tate < GAME_MAP_TATE_MAX; tate++)
	{
		for (int yoko = 0; yoko < GAME_MAP_YOKO_MAX; yoko++)
		{
			DrawGraph(
				stage1[tate][yoko].x,
				stage1[tate][yoko].y,
				mapchip.handle[stage1[tate][yoko].kind],
				TRUE);
			DrawBox(stage1[tate][yoko].x, stage1[tate][yoko].y,
				stage1[tate][yoko].x + stage1[tate][yoko].width,
				stage1[tate][yoko].y + stage1[tate][yoko].height,
				GetColor(0, 255, 255), FALSE);
		}
	}
}

VOID PLAYER_LIFE_DRAW(VOID)
{
	for (int i = 0; i < player.life.now;i++)
	{
		DrawGraph(PlayerLife.width * i - 1, 0, PlayerLife.handle, TRUE);
	}
}

VOID PLAYER_MOVE(VOID)
{
	player.IsMove = FALSE;

	//プレイヤーが動けるとき
	if (player.CanMove == TRUE)	
	{
		if (MY_KEY_DOWN(KEY_INPUT_RIGHT) != TRUE &&
			MY_KEY_DOWN(KEY_INPUT_LEFT) != TRUE &&
			player.status != PLAYER_STATUS_JUMP)	//右と左のキーを押していなくて、状態がジャンプ以外の時
		{
			player.status = PLAYER_STATUS_STOP;		//プレイヤーの状態を止まっている状態にする
		}

		//上のキーを押したとき
		if (MY_KEY_DOWN_MOMENT(KEY_INPUT_UP) == TRUE)	
		{
			if (player.NowJumpTimes < player.CanJumpTimes)	//現在のジャンプ回数がジャンプできる回数未満の時
			{
				if (player.status != PLAYER_STATUS_JUMP)	//状態がジャンプ以外の時
				{
					player.status = PLAYER_STATUS_JUMP;		//プレイヤーの状態をジャンプ状態にする
					player.NowJumpTimes++;					//現在のジャンプ数を増やす
				}
				else										//状態がジャンプの時
				{
					IsPrecedence[KEY_INPUT_UP] = TRUE;		//上のキーの先行入力開始
				}
			}
		}

		//下のキーを押していないとき
		if (MY_KEY_DOWN(KEY_INPUT_DOWN) != TRUE)
		{
			//左のキーを押していなく、右のキーを押しているとき
			if (MY_KEY_DOWN(KEY_INPUT_RIGHT) == TRUE && MY_KEY_DOWN(KEY_INPUT_LEFT) != TRUE)
			{
				if (player.IsScroll == FALSE &&
					player.CanRightMove == TRUE)				//スクロールしていなく、右に動けるとき
				{
					player.CenterX += PLAYER_SPEED_NORMAL;		
				}
				if (player.status != PLAYER_STATUS_JUMP)		//状態がジャンプ以外の時
				{
					player.status = PLAYER_STATUS_MOVE_R;		//プレイヤーの状態を歩き(右)状態にする
				}
				player.IsMove = TRUE;											
				player.muki = MUKI_R;							//プレイヤーの向きは右
			}

			//右のキーを押していなく、左のキーを押しているとき
			if (MY_KEY_DOWN(KEY_INPUT_LEFT) == TRUE && MY_KEY_DOWN(KEY_INPUT_RIGHT) != TRUE)
			{
				if (player.x > 0 && player.CanLeftMove == TRUE)					
				{
					player.CenterX -= PLAYER_SPEED_NORMAL;
				}
				if (player.status != PLAYER_STATUS_JUMP)		//状態がジャンプ以外の時
				{
					player.status = PLAYER_STATUS_MOVE_L;		//プレイヤーの状態を歩き(左)状態にする
				}
				player.IsMove = TRUE;
				player.muki = MUKI_L;							//プレイヤーの向きは左
			}
		}

		//下のキーを押しているとき
		if (MY_KEY_DOWN(KEY_INPUT_DOWN) == TRUE)
		{
			if (player.status != PLAYER_STATUS_JUMP)			//状態がジャンプ以外の時
			{
				player.status = PLAYER_STATUS_SQUAT;			//プレイヤーの状態をしゃがみ状態にする
			}
		}

		if (MY_KEY_DOWN_PRECEDENCE(KEY_INPUT_UP) == TRUE)		
		{
			if (player.NowJumpTimes < player.CanJumpTimes &&
				player.status != PLAYER_STATUS_JUMP && 
				player.NowJumpTimes > 0)
			{
				player.NowJumpTimes++;
				player.status = PLAYER_STATUS_JUMP;
			}
		}

		if (player.status == PLAYER_STATUS_JUMP)
		{
			PLAYER_JUMP();
		}
	}
	PLAYER_ATTACK_PROC();
	MAGIC_PROC();

	player.x = player.CenterX - player.width / 2;
	player.y = player.CenterY - player.height / 2;

	player.CanMove = TRUE;
}



VOID PLAYER_JUMP(VOID)
{
	
	if (player.jump.cnt < player.jump.CntMax)
	{
		player.CenterY -= player.JumpRise;
		if (player.jump.cnt > 7 && player.JumpRise >= 0)
		{
			player.JumpRise--;
		}
		player.status = PLAYER_STATUS_JUMP;
	}
	else
	{
		player.status = PLAYER_STATUS_STOP;
		player.jump.cnt = 0;
		player.JumpRise = 10;
	}
	player.jump.cnt++;
}

VOID PLAYER_ATTACK_PROC(VOID)
{
	if (MY_KEY_DOWN(KEY_INPUT_V) == TRUE)//Ⅴキーを押したとき
	{
		if (player.CanAttack == TRUE)//プレイヤーが攻撃可能なら
		{
			player.CanAttack = FALSE;	//攻撃を一度不能にする
			for (int i = 0; i < PLAYER_TAMA_MAX; i++)
			{
				if (player.attack[i].IsDraw == FALSE)
				{
					//プレイやーの向きから弾の向きを定める(撃った後にプレイヤーの向きを変えても弾の向きは変わらない)
					if (player.muki == MUKI_R)//プレイヤーが右向きのとき
					{
						player.attack[i].muki = MUKI_R;//弾の向きは右
						player.attack[i].image.x = player.x + player.width;//弾のスタート位置X
					}
					if (player.muki == MUKI_L)//プレイヤーが左向きの時
					{
						player.attack[i].muki = MUKI_L;//弾の向きは左
						player.attack[i].image.x = player.x;//弾のスタート位置X
					}
					player.attack[i].image.y = player.CenterY;	//弾のスタート位置Y
					player.attack[i].IsDraw = TRUE;		//弾の描画ができるようにする

					//通常攻撃の効果音
					if (CheckSoundMem(AttackSE.handle) == 0)
					{
						ChangeVolumeSoundMem(255 * 30 / 100, AttackSE.handle);//30%のボリューム
						PlaySoundMem(AttackSE.handle, DX_PLAYTYPE_BACK);
					}
					break;
				}
			}
		}
	}
	if (player.CanAttack == FALSE)
	{
		player.AttackInterval.cnt = CNT_CHECK(player.AttackInterval.cnt, player.AttackInterval.CntMax);
		if(player.AttackInterval.cnt == 0)
		{
			player.CanAttack = TRUE;
		}
	}
	return;
}

VOID MAGIC_PROC(VOID)
{
	int i = -1;
	MAGIC_SELECT();
	if (MY_KEY_DOWN(KEY_INPUT_C) == TRUE)	//Cキーを押したとき
	{
		if (player.CanMagic == TRUE)	//魔法が使える状態なら
		{
			i = MAGIC_CHECK();
			if (i != -1 && player.EquipMagic != -1)
			{
				magic[i].No = player.EquipMagic;
				if (MagicData[player.EquipMagic].fiexd == TRUE)//開始位置が固定のとき
				{
					magic[i].x = MagicData[player.EquipMagic].StartX;
					magic[i].y = MagicData[player.EquipMagic].StartY;
				}
				else	//開始位置が固定じゃないとき
				{
					//プレイヤーの前方から出る
					if (player.muki == MUKI_R)//プレイヤーが右向きのとき
					{
						magic[i].CenterX = player.x + player.width;
						magic[i].muki = MUKI_R;		//魔法の向きを右に決める
					}
					if (player.muki == MUKI_L)//プレイヤーが左向きのとき
					{
						magic[i].CenterX = player.x;
						magic[i].muki = MUKI_L;		//魔法の向きを左に決める
					}
					magic[i].CenterY = player.CenterY;
				}
				magic[i].IsBuff = MagicData[player.EquipMagic].IsBuff;
				magic[i].width = MagicData[0].width;
				magic[i].height = MagicData[0].height;
				//player.MagicInterval.CntMax = MagicData[player.EquipMagic].CoolTime;

				magic[i].x = magic[i].CenterX - magic[i].width / 2;
				magic[i].y = magic[i].CenterY - magic[i].height / 2;

				magic[i].IsDraw = TRUE;
				player.CanMagic = FALSE;
			}
		}
	}
	if (player.CanMagic == FALSE)
	{
		player.MagicInterval.cnt = CNT_CHECK(player.MagicInterval.cnt, player.MagicInterval.CntMax);
		if(player.MagicInterval.cnt == 0)
		{
			player.CanMagic = TRUE;
		}
	}
	for (int i = 0; i < PLAYER_MAGIC_MAX; i++)
	{
		if (magic[i].IsDraw == TRUE)
		{
			MAGIC_EFFECT(i);
		}
	}
	return;
};

VOID MAGIC_SELECT(VOID)
{
	if (MY_KEY_DOWN_MOMENT(KEY_INPUT_Z) == TRUE)
	{
		if (-1 < player.EquipMagic)	//選択中の魔法の番号が-1より大きいとき(-1は選択なし)
			player.EquipMagic--;	//一つ前の魔法を選択
		else						//選択魔法が-1(選択なし)のとき
			player.EquipMagic = AvailableMagic - 1;	//最後尾の魔法を選択
	}
	else if (MY_KEY_DOWN_MOMENT(KEY_INPUT_X) == TRUE) 	
	{
		if (AvailableMagic - 1 > player.EquipMagic)	//選択中の魔法の番号が取得済み魔法数の未満のとき
			player.EquipMagic++;
		else							//選択魔法が最後尾のとき
			player.EquipMagic = -1;		//-1(選択なし)に戻す		
	}
}

INT MAGIC_CHECK(VOID)
{
	for (int i = 0; i < PLAYER_MAGIC_MAX; i++)
	{
		if (magic[i].IsDraw == FALSE)
		{
			return i;
		}
	}
	return -1;
}

VOID MAGIC_EFFECT(int i)
{
	switch (magic[i].No)
	{
	case 0:
		if(magic[i].muki == MUKI_R)
		magic[i].CenterX += 10;
		if(magic[i].muki == MUKI_L)
		magic[i].CenterX -= 10;
		break;
	case 1:
		JumpBuff = 1;
		magic[i].IsDraw = FALSE;
		break;
	}
}

VOID MAGIC_ICON_DRAW(VOID)
{
	DrawGraph(MagicIcon.x, MagicIcon.y, MagicIcon.handle[player.EquipMagic + 1], TRUE);
}

VOID PLAYER_DRAW(VOID)	//プレイヤーの描画
{
	if (player.IsDraw == TRUE)	//プレイヤーを描画するとき
	{
		DrawGraph(
			player.x,
			player.y,
			player.handle[player.change.NowImage],
			TRUE);
		
		switch (player.status)	//プレイヤーの状態によって処理を変える
		{
		case PLAYER_STATUS_MOVE_R:	//プレイヤーが右向きに歩いているとき
			WalkCheckR++;
			WalkCheckL = 0;
			if (WalkCheckR == 1)
			{
				player.change.cnt = player.change.CntMax;
			}
			break;

		case PLAYER_STATUS_MOVE_L:	//プレイヤーが左向きに歩いているとき
			WalkCheckR = 0;
			WalkCheckL++;
			if (WalkCheckL == 1)
			{
				player.change.cnt = player.change.CntMax;
			}
			break;

		case PLAYER_STATUS_STOP:	//プレイヤーが止まっているとき
			if (player.muki == MUKI_R)	//プレイヤーが右向きの時
				player.change.NowImage = PLAYER_DIV_STOP_R;
			if (player.muki == MUKI_L)	//プレイヤーが左向きの時
				player.change.NowImage = PLAYER_DIV_STOP_L;
			break;

		case PLAYER_STATUS_SQUAT:	//プレイヤーがしゃがんでいるとき
			if (player.muki == MUKI_R)	//プレイヤーが右向きの時
				player.change.NowImage = PLAYER_DIV_SQUAT_R;
			if (player.muki == MUKI_L)	//プレイヤーが左向きの時
				player.change.NowImage = PLAYER_DIV_SQUAT_L;
			break;
		}
		player.change.cnt = CNT_CHECK(player.change.cnt, player.change.CntMax);
		if(player.change.cnt == 0)
		{
			if (player.status == PLAYER_STATUS_MOVE_R)
			{
				if (player.change.NowImage < PLAYER_DIV_WALK_R + PLAYER_DIV_TATE - 1)
				{
					player.change.NowImage++;
				}
				else
				{
					player.change.NowImage = PLAYER_DIV_WALK_R;
				}
			}
			if (player.status == PLAYER_STATUS_MOVE_L)
			{
				if (player.change.NowImage < PLAYER_DIV_WALK_L + PLAYER_DIV_TATE - 1 &&
					player.change.NowImage >= PLAYER_DIV_WALK_L)
				{
					player.change.NowImage++;
				}
				else
				{
					player.change.NowImage = PLAYER_DIV_WALK_L;
				}
			}
		}
	}
}

VOID PLAYER_ATTACK_DRAW(VOID)
{
	for (int i = 0; i < PLAYER_TAMA_MAX; i++)
	{
		if (player.attack[i].IsDraw == TRUE)
		{
			DrawGraph(player.attack[i].image.x,
					  player.attack[i].image.y,
					  player.attack[0].image.handle, TRUE);
			if (player.attack[i].muki == MUKI_R)
			{
				if (player.attack[i].image.x + player.attack[i].image.width > GAME_WIDTH)
				{
					player.attack[i].IsDraw = FALSE;
				}
				else
				{
					player.attack[i].image.x += PLAYER_TAMA_SPEED;
				}
			}
			if (player.attack[i].muki == MUKI_L)
			{
				if (player.attack[i].image.x < 0)
				{
					player.attack[i].IsDraw = FALSE;
				}
				else
				{
					player.attack[i].image.x -= PLAYER_TAMA_SPEED;
				}
			}
			
		}
	}
}

VOID MAGIC_DRAW(VOID)
{
	
	for (int i = 0; i < PLAYER_MAGIC_MAX; i++)
	{
		if (magic[i].IsDraw == TRUE)
		{
			magic[i].x = magic[i].CenterX - magic[i].width / 2;
			magic[i].y = magic[i].CenterY - magic[i].height / 2;	

			magic[i].change.CntMax = 20;
			magic[i].change.cnt = CNT_CHECK(magic[i].change.cnt, magic[i].change.CntMax);
			if (magic[i].change.cnt == 0)
			{
				if (magic[i].change.NowImage < magic[i].No * MAGIC_DIV_KIND + MAGIC_DIV_STATE)
				{
					magic[i].change.NowImage++;
				}
				else
				{
					magic[i].change.NowImage = magic[i].No * MAGIC_DIV_KIND;
				}
			}
			if (magic[i].IsBuff == FALSE)
			{
				DrawGraph(magic[i].x, magic[i].y, MagicData[0].handle[magic[i].change.NowImage], true);
			}
			if (magic[i].CenterX < 0 || magic[i].CenterX > GAME_WIDTH)
			{
				magic[i].IsDraw = FALSE;
			}
		}
	}
};

BOOL MY_CHECK_RECT_COLL(RECT a, RECT b)
{
	if (a.left < b.right &&
		a.top < b.bottom &&
		a.right > b.left &&
		a.bottom > b.top
		)
	{
		return TRUE;
	}
	return FALSE;
}

VOID COLL_PROC(VOID)
{
	int x, y;
	for (int n = 0; n < ENEMY_MAX; n++)
	{
		if (enemy[n].IsDraw == TRUE)//敵が描画されているとき
		{
			enemy[n].x = enemy[n].CenterX - enemy[n].width / 2;
			enemy[n].y = enemy[n].CenterY - enemy[n].height / 2;

			//敵の当たり判定

			//基本の判定
			enemy[n].coll.base.left = enemy[n].x + 15;
			enemy[n].coll.base.right = enemy[n].x + enemy[n].width - 15;
			enemy[n].coll.base.top = enemy[n].y + 15;
			enemy[n].coll.base.bottom = enemy[n].y + enemy[n].height;
			//
			enemy[n].coll.CheckTop.left = enemy[n].coll.base.left;
			enemy[n].coll.CheckTop.right = enemy[n].coll.base.right;
			enemy[n].coll.CheckTop.top = enemy[n].coll.base.top + 10;
			enemy[n].coll.CheckTop.bottom = enemy[n].coll.base.bottom;
			//下側に少し大きい判定
			enemy[n].coll.CheckBottom.left = enemy[n].coll.base.left;
			enemy[n].coll.CheckBottom.right = enemy[n].coll.base.right;
			enemy[n].coll.CheckBottom.top = enemy[n].coll.base.top;
			enemy[n].coll.CheckBottom.bottom = enemy[n].coll.base.bottom + 10;
			//左側に少し大きい判定
			enemy[n].coll.CheckLeft.left = enemy[n].coll.base.left - 5;
			enemy[n].coll.CheckLeft.right = enemy[n].coll.base.right - 5;
			enemy[n].coll.CheckLeft.top = enemy[n].coll.base.top;
			enemy[n].coll.CheckLeft.bottom = enemy[n].coll.base.bottom;
			//右側に少し大きい判定
			enemy[n].coll.CheckRight.left = enemy[n].coll.base.left + 5;
			enemy[n].coll.CheckRight.right = enemy[n].coll.base.right + 5;
			enemy[n].coll.CheckRight.top = enemy[n].coll.base.top;
			enemy[n].coll.CheckRight.bottom = enemy[n].coll.base.bottom;


			if (MY_CHECK_MAP1_COLL(enemy[n].coll.CheckLeft, &x, &y) == BLOCK)//左に障害物
			{
				enemy[n].CanLeftMove = FALSE;
				enemy[n].CanRightMove = TRUE;
			}
			if (MY_CHECK_MAP1_COLL(enemy[n].coll.CheckRight, &x, &y) == BLOCK)//右に障害物
			{
				enemy[n].CanRightMove = FALSE;
				enemy[n].CanLeftMove = TRUE;
			}
			if (MY_CHECK_MAP1_COLL(enemy[n].coll.CheckBottom, &x, &y) == -1 && enemy[n].IsFly == FALSE)
			{
				enemy[n].CenterY += 10;
			}
			if (MY_CHECK_MAP1_COLL(enemy[n].coll.CheckBottom, &x, &y) == BLOCK)
			{
				enemy[n].CenterY = stage1[x][y].y - enemy[n].height / 2 - 1;
			}
			enemy[n].x = enemy[n].CenterX - enemy[n].width / 2;
			enemy[n].y = enemy[n].CenterY - enemy[n].height / 2;
		}
	}

	for (int i = 0; i < PLAYER_TAMA_MAX; i++)
	{
		if (player.attack[i].IsDraw == TRUE)
		{
			//通常攻撃の判定
			player.attack[i].coll.left = player.attack[i].image.x;
			player.attack[i].coll.right = player.attack[i].image.x + player.attack[i].image.width;
			player.attack[i].coll.top = player.attack[i].image.y;
			player.attack[i].coll.bottom = player.attack[i].image.y + player.attack[i].image.height;

			if (MY_CHECK_MAP1_COLL(player.attack[i].coll, &x, &y) == BLOCK)//障害物に当たったとき
			{
				player.attack[i].IsDraw = FALSE;	//	描画をやめる
			}
			if (MY_CHECK_ENEMY_COLL(player.attack[i].coll) != -1)	//敵に当たった時
			{
				player.attack[i].IsDraw = FALSE;	//	描画をやめる
				enemy[MY_CHECK_ENEMY_COLL(player.attack[i].coll)].IsDraw = FALSE;	//敵の描画をやめる
				//敵を倒したときの効果音
				if (CheckSoundMem(DefeatSE.handle) == 0)
				{
					ChangeVolumeSoundMem(255 * 60 / 100, DefeatSE.handle);//60%のボリューム
					PlaySoundMem(DefeatSE.handle, DX_PLAYTYPE_BACK);
				}
			}
		}
	}


	for (int i = 0; i < PLAYER_MAGIC_MAX; i++)
	{
		if (magic[i].IsDraw == TRUE && magic[i].IsBuff == FALSE)
		{
			magic[i].coll.right = magic[i].x + magic[i].width;
			magic[i].coll.left = magic[i].x;
			magic[i].coll.top = magic[i].y;
			magic[i].coll.bottom = magic[i].y + magic[i].height;

			if (MY_CHECK_MAP1_ACT_COLL(magic[i].coll) == WOOD)
			{

			}
		}
	}

	player.coll.base.right = player.x + player.width - 25;
	player.coll.base.left = player.x + 25;
	player.coll.base.top = player.y;
	player.coll.base.bottom = player.y + player.height;

	if (player.status == PLAYER_STATUS_SQUAT)
	{
		player.coll.base.top = player.y + player.height / 2;
	}

	player.coll.CheckItem.right = player.coll.base.right - 10;
	player.coll.CheckItem.left = player.coll.base.left + 10;
	player.coll.CheckItem.top = player.coll.base.top;
	player.coll.CheckItem.bottom = player.coll.base.bottom;

	player.coll.CheckTop.right = player.coll.base.right;
	player.coll.CheckTop.left = player.coll.base.left;
	player.coll.CheckTop.top = player.coll.base.top + 10;
	player.coll.CheckTop.bottom = player.coll.base.top;

	player.coll.CheckBottom.right = player.coll.base.right;
	player.coll.CheckBottom.left = player.coll.base.left;
	player.coll.CheckBottom.top = player.coll.base.bottom;
	player.coll.CheckBottom.bottom = player.coll.base.bottom + gravity;

	player.coll.CheckRight.right = player.coll.base.right + 5;
	player.coll.CheckRight.left = player.coll.base.left + 5;
	player.coll.CheckRight.top = player.coll.base.top;
	player.coll.CheckRight.bottom = player.coll.base.bottom;

	player.coll.CheckLeft.right = player.coll.base.right - 5;
	player.coll.CheckLeft.left = player.coll.base.left - 5;
	player.coll.CheckLeft.top = player.coll.base.top;
	player.coll.CheckLeft.bottom = player.coll.base.bottom;

	player.CanRightMove = TRUE;
	player.CanLeftMove = TRUE;
	
	for (int tate = 0; tate < GAME_MAP_TATE_MAX; tate++)
	{
		for (int yoko = 0; yoko < GAME_MAP_YOKO_MAX; yoko++)
		{
			stage1[tate][yoko].coll.left = stage1[tate][yoko].x;
			stage1[tate][yoko].coll.right = stage1[tate][yoko].x + stage1[tate][yoko].width;
			stage1[tate][yoko].coll.top = stage1[tate][yoko].y;
			stage1[tate][yoko].coll.bottom = stage1[tate][yoko].y + stage1[tate][yoko].height;
		}
	}
	
	if (MY_CHECK_MAP1_COLL(player.coll.CheckBottom,&x,&y) == -1)	//プレイヤーが浮いているとき
	{
		if (player.status != PLAYER_STATUS_JUMP)	//プレイヤーの状態がジャンプ以外の時
		{
			player.CenterY += gravity;		//重力をかける
			//空中にいる時間によって重力が大きくなっていく(重力加速度風)
			FallTime.cnt = CNT_CHECK(FallTime.cnt, FallTime.CntMax);
			if(FallTime.cnt == 0)
			{
				gravity += 1;	
			}
			if (player.NowJumpTimes == 0)
			{
				player.CanJumpTimes = 0;
			}
		}
	}
	if (MY_CHECK_MAP1_COLL(player.coll.CheckTop, &x, &y) == BLOCK)
	{
		player.CenterY = stage1[x][y].y + stage1[x][y].height + player.height / 2 + 10;
		player.status = PLAYER_STATUS_STOP;
		player.jump.cnt = 0;
	}
	if(MY_CHECK_MAP1_COLL(player.coll.CheckBottom, &x, &y) == BLOCK)//下に地面があったら着地
	{
		player.CenterY = stage1[x][y].y - player.height / 2 - 1;	//下の地面の上に立たせる
		FallTime.cnt = 0;	//落下時のカウントの初期化
		gravity = 10;	//重力をもとに戻す
		if (player.NowJumpTimes == player.CanJumpTimes)
		{
			JumpBuff = 0;
		}
		player.NowJumpTimes = 0;
		player.CanJumpTimes = 1 + JumpBuff;
	}
	if (MY_CHECK_MAP1_COLL(player.coll.CheckRight, &x, &y) == BLOCK)//右に障害物
	{
		player.CanRightMove = FALSE;//右に進めないようにする
	}
	if (MY_CHECK_MAP1_COLL(player.coll.CheckLeft, &x, &y) == BLOCK)//左に障害物
	{
		player.CanLeftMove = FALSE;//左に進めないようにする
	}
	if (MY_CHECK_MAP1_ACT_COLL(player.coll.CheckItem) == GOAL)//ゴールに当たったとき
	{
		EndKind = GAME_CLEAR;	//エンドの種類はクリア
		//ゴール時効果音
		if (CheckSoundMem(GoalSE.handle) == 0)
		{
			ChangeVolumeSoundMem(255 * 50 / 100, GoalSE.handle);//50%のボリューム
			PlaySoundMem(GoalSE.handle, DX_PLAYTYPE_BACK);
		}
		GameScene = GAME_SCENE_END;	//シーンをエンド画面に遷移
	}
	if (MY_CHECK_ENEMY_COLL(player.coll.base) != -1 && player.life.invincible == FALSE)
	{
		player.life.invincible = TRUE;
		player.life.now--;
		if (CheckSoundMem(HitSE.handle) == 0)
		{
			ChangeVolumeSoundMem(255 * 30 / 100, HitSE.handle);//30%のボリューム
			PlaySoundMem(HitSE.handle, DX_PLAYTYPE_BACK);
		}
	}
	if (player.life.now == 0)
	{
		EndKind = GAME_OVER;	//エンドの種類はゲームオーバー
		GameScene = GAME_SCENE_END;	//シーンをエンド画面に遷移
	}
	if (player.life.invincible == TRUE)
	{
		player.life.InvincibleCnt.cnt = CNT_CHECK(player.life.InvincibleCnt.cnt, player.life.InvincibleCnt.CntMax);
		if (player.life.InvincibleCnt.cnt == 0)
		{
			player.life.invincible = FALSE;
		}
		if (player.life.InvincibleCnt.cnt % 2 == 1)
			player.IsDraw = FALSE;
		else
			player.IsDraw = TRUE;
	}

	player.x = player.CenterX - player.width / 2;
	player.y = player.CenterY - player.height / 2;
	
}

INT MY_CHECK_MAP1_COLL(RECT a,int *x, int *y)
{
	
	for (int tate = 0; tate < GAME_MAP_TATE_MAX; tate++)
	{
		for (int yoko = 0; yoko < GAME_MAP_YOKO_MAX; yoko++)
		{
			if (MY_CHECK_RECT_COLL(stage1[tate][yoko].coll, a) == TRUE)
			{
				if (stage1[tate][yoko].kind != ae &&
					stage1[tate][yoko].kind != ag &&
					stage1[tate][yoko].kind != ha)
				{
					*x = tate;
					*y = yoko;
					return BLOCK;
				}
			}
		}
	}
	return -1;
}

INT MY_CHECK_MAP1_ACT_COLL(RECT a)
{
	for (int tate = 0; tate < GAME_MAP_TATE_MAX; tate++)
	{
		for (int yoko = 0; yoko < GAME_MAP_YOKO_MAX; yoko++)
		{
			if (MY_CHECK_RECT_COLL(stage1[tate][yoko].coll, a) == TRUE)
			{
				if (stage1[tate][yoko].kind == ag)
				{
					return GOAL;
				}
				if (stage1[tate][yoko].kind == ha)
				{
					stage1[tate][yoko].kind = ae;
					AvailableMagic++;
					return BOOK;
				}
				if (stage1[tate][yoko].kind == ga)
				{
					stage1[tate][yoko].kind = ae;
					return WOOD;
				}
			}
		}
	}
	return -1;
}

INT MY_CHECK_ENEMY_COLL(RECT player)
{
	for (int n = 0; n < ENEMY_MAX; n++)
	{
		if (MY_CHECK_RECT_COLL(enemy[n].coll.base, player) == TRUE && enemy[n].IsDraw == TRUE)
		{
			return n;
		}
	}
	return -1;
}

VOID STAGE_SCROLL(VOID)
{
	if (player.CenterX + player.width > GAME_WIDTH / 2)
	{
		player.IsScroll = TRUE;
		if (player.IsMove == TRUE && player.CanRightMove == TRUE && player.muki == MUKI_R)
		{
			for (int tate = 0; tate < GAME_MAP_TATE_MAX; tate++)
			{
				for (int yoko = 0; yoko < GAME_MAP_YOKO_MAX; yoko++)
				{
					stage1[tate][yoko].x -= PLAYER_SPEED_NORMAL;
				}
			}
			for (int n = 0; n < ENEMY_MAX; n++)
			{
				enemy[n].CenterX -= PLAYER_SPEED_NORMAL;
			}
			screen.right -= PLAYER_SPEED_NORMAL;
			screen.left -= PLAYER_SPEED_NORMAL;
		}
	}
	else if (player.x <= 0 && player.CanLeftMove == TRUE && player.muki == MUKI_L)
	{
		player.IsScroll = TRUE;
		if (player.IsMove == TRUE && player.CanLeftMove == TRUE)
		{
			for (int tate = 0; tate < GAME_MAP_TATE_MAX; tate++)
			{
				for (int yoko = 0; yoko < GAME_MAP_YOKO_MAX; yoko++)
				{
					stage1[tate][yoko].x += PLAYER_SPEED_NORMAL;
				}
			}
			for (int n = 0; n < ENEMY_MAX; n++)
			{
				enemy[n].CenterX += PLAYER_SPEED_NORMAL;
			}
			screen.right += PLAYER_SPEED_NORMAL;
			screen.left += PLAYER_SPEED_NORMAL;
		}
	}
	else
	{
		player.IsScroll = FALSE;
	}
}

VOID ENEMY_PROC(VOID)
{
	for (int m = 0; m < ENEMY_DATA_MAX; m++)//敵のデータの数まで
	{
		if (EnemyData[m].StartX > screen.left && EnemyData[m].StartX < screen.right &&
			EnemyData[m].StartY > screen.top && EnemyData[m].StartY < screen.bottom &&
			EnemyData[m].IsDraw == FALSE)//敵のスタート位置が画面内にあり、まだ一度もその敵を描画してないとき
		{
			int n = ENEMY_CHECK();
			if(n != -1)
			{
				//敵のデータを登録する
				enemy[n].x = EnemyData[m].StartX;
				enemy[n].y = EnemyData[m].StartY;
				enemy[n].kind = EnemyData[m].kind;
				enemy[n].width = EnemyData[m].width;
				enemy[n].height = EnemyData[m].height;
				enemy[n].CenterX = enemy[n].x + enemy[n].width / 2;
				enemy[n].CenterY = enemy[n].y + enemy[n].height / 2;
				enemy[n].IsFly = EnemyData[m].IsFly;

				enemy[n].IsDraw = TRUE;		//敵を描画できるようにする
				EnemyData[m].IsDraw = TRUE;		//ｍ番目のデータの敵は描画済み
			}
		}
	}
	for (int n = 0; n < ENEMY_MAX; n++)
	{
		if (enemy[n].IsDraw == TRUE)//敵が描画されているとき
		{
			ENEMY_MOVE(n);//敵の動きの処理
		}
	}
}

int ENEMY_CHECK()
{
	for (int n = 0; n < ENEMY_MAX; n++)
	{
		if (enemy[n].IsDraw == 0)
		{
			return n;
		}
	}
	return -1;
}

VOID ENEMY_MOVE(int n)
{
	
	switch (enemy[n].kind)//敵の種類によって動きを決める
	{
	case 0:	//真っ直ぐ進み壁に当たると反対の方向へ向きを変える
		if (enemy[n].CanLeftMove == TRUE)
		{
			enemy[n].muki = MUKI_L;
			enemy[n].speed = ENEMY_SPEED_SLOW;
			enemy[n].CenterX -= enemy[n].speed;
		}
		if (enemy[n].CanRightMove == TRUE)
		{
			enemy[n].muki = MUKI_R;
			enemy[n].speed = ENEMY_SPEED_SLOW;
			enemy[n].CenterX += enemy[n].speed;
		}
		break;
	case 1:
		if (enemy[n].CenterX > player.CenterX)
		{
			enemy[n].muki = MUKI_L;
		}
		if (enemy[n].CenterX < player.CenterX)
		{
			enemy[n].muki = MUKI_R;
		}
		int DistanceX = player.CenterX - enemy[n].CenterX;
		int DistanceY = player.CenterY - enemy[n].CenterY;

		int Distance = sqrt(pow(DistanceX , 2) + pow(DistanceY , 2));
		if (Distance < enemy[n].width * 5)
		{
			enemy[n].CenterX += (player.CenterX - enemy[n].CenterX) / Distance * 2;
			enemy[n].CenterY += (player.CenterY - enemy[n].CenterY) / Distance * 2;
		}
		break;
	}
}

VOID ENEMY_DRAW(VOID)
{
	for (int n = 0; n < ENEMY_MAX; n++)
	{
		if (enemy[n].IsDraw == TRUE)
		{
			DrawGraph(
				enemy[n].x,
				enemy[n].y,
				EnemyImage[enemy[n].kind].handle[enemy[n].change.NowImage],
				TRUE);
			enemy[n].change.CntMax = 20;

			if (enemy[n].change.cnt < enemy[n].change.CntMax)
			{
				enemy[n].change.cnt++;
			}
			else
			{
				if (enemy[n].muki == MUKI_L)
				{
					if (enemy[n].change.NowImage < 1)
					{
						enemy[n].change.NowImage++;
					}
					else
					{
						enemy[n].change.NowImage = 0;
					}
				}
				if (enemy[n].muki == MUKI_R)
				{
					if (enemy[n].change.NowImage < 3)
					{
						enemy[n].change.NowImage++;
					}
					else
					{
						enemy[n].change.NowImage = 2;
					}
				}
				enemy[n].change.cnt = 0;
			}

			//画面外に行くと消える
			if (enemy[n].x + enemy[n].width < 0 || enemy[n].x > GAME_WIDTH)
			{
				enemy[n].IsDraw = FALSE;
			}
		}
	}
}