#include "DxLib.h"

#define GAME_WIDTH	1280	
#define GAME_HEIGHT	720
#define GAME_COLOR	32

#define GAME_WINDOW_BAR	 0
#define GAME_WINDOW_NAME	"GAME TITLE"
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
#define PLAYER_DIV_WALK_R 0
#define PLAYER_DIV_WALK_L 8

#define MAP_DIV_WIDTH     80
#define MAP_DIV_HEIGHT    80
#define MAP_DIV_TATE      8
#define MAP_DIV_YOKO      8
#define MAP_DIV_NUM  MAP_DIV_TATE * MAP_DIV_YOKO

#define GAME_MAP_TATE_MAX   9
#define GAME_MAP_YOKO_MAX   32
#define GAME_MAP_KIND_MAX  2

#define PLAYER_TAMA_MAX 10

#define STAGE_MAX 5

#define IMAGE_PLAYER_PATH       TEXT(".\\IMAGE\\player.png")
#define IMAGE_PLAYER_TAMA_PATH  TEXT(".\\IMAGE\\tama.png")
#define IMAGE_TITLE_BACK_PATH   TEXT(".\\IMAGE\\TitleBack.png")
#define IMAGE_TITLE_ROGO_PATH   TEXT(".\\IMAGE\\TitleRogo.png")
#define IMAGE_STAGE1_BACK_PATH  TEXT(".\\IMAGE\\Stage1Back.png")
#define IMAGE_MAP1_PATH         TEXT(".\\IMAGE\\map1.png")

#define MUSIC_TITLE_BGM_PATH    TEXT(".\\MUSIC\\bgm_maoudamashii_healing01.mp3")
#define MUSIC_STAGE1_BGM_PATH	TEXT(".\\MUSIC\\bgm_maoudamashii_piano25.mp3")

enum GAME_MAP_KIND
{
	aa = 0,
	ab = 1,
	ac = 2,
	ad = 3,
	ae = 4,
	af = 5,
	ag = 6,
	ah = 7,
	ba = 8,
};

enum GAME_SCENE {
	GAME_SCENE_START,
	GAME_SCENE_PLAY,
	GAME_SCENE_END,
};

enum PLAYER_STATUS
{
	PLAYER_STATUS_MOVE_R,
	PLAYER_STATUS_MOVE_L,
	PLAYER_STATUS_STOP,
	PLAYER_STATUS_JUMP,
};

enum MUKI
{
	MUKI_R,
	MUKI_L,
};

enum CHECK_MAP_COLL
{
	BLOCK,
	AIR,
	GOAL,
};

typedef struct STRUCT_I_POINT
{
	int x = -1;
	int y = -1;
}iPOINT;

typedef struct STRUCT_CNT
{
	int cnt;
	int CntMax;
}CNT;

typedef struct STRUCT_IMAGE  
{
	char path[PATH_MAX];		
	int handle;					
	int x;						
	int y;						
	int width;					
	int height;					
}IMAGE;	

typedef struct STRUCT_MUSIC
{
	char path[PATH_MAX];		
	int handle;					
}MUSIC;

typedef struct STRUCT_CHANGE_IMAGE
{
	int NowImage;
	int cnt;
	int CntMax;
}CHANGE_IMAGE;

typedef struct STRUCT_MAP_IMAGE
{
	char path[PATH_MAX];
	int handle[MAP_DIV_NUM];
	int kind[MAP_DIV_NUM];
	int x;
	int y;
	int width;
	int height;
}MAPCHIP;

typedef struct STRUCT_MAP
{
	GAME_MAP_KIND kind;
	int x;
	int y;
	int width;
	int height;
	RECT coll;
}MAP;

typedef struct STRUCT_PLAYER_ATTACK
{
	IMAGE image;
	BOOL IsDraw;
	CNT cnt;
	int muki;
}PLAYER_ATTACK;

typedef struct STRUCT_PLAYER
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
	int JumpMax = 20;
	int JumpCnt = 0;
	int JumpRise = 10;
	BOOL IsDraw;
	RECT coll;
	RECT CheckBottomColl;
	RECT CheckRightColl;
	RECT CheckLeftColl;
	CHANGE_IMAGE change;
	BOOL CanMove = TRUE;
	BOOL CanAttack = TRUE;
	CNT AttackInterval;
	BOOL CanRightMove = TRUE;
	BOOL CanLeftMove = TRUE;
	BOOL CanJump = FALSE;
	BOOL IsMove = FALSE;
	BOOL IsScroll = FALSE;
	PLAYER_ATTACK attack[PLAYER_TAMA_MAX];
}PLAYER;




IMAGE TitleBack;
IMAGE TitleRogo;
IMAGE StageBack[STAGE_MAX];
MAPCHIP mapchip;
MAP stage1[GAME_MAP_TATE_MAX][GAME_MAP_YOKO_MAX];;
PLAYER player;
int StartTimeFps;				//����J�n����
int CountFps;					//�J�E���^
float CalcFps;					//�v�Z����
int SampleNumFps = GAME_FPS;	//���ς����T���v����
int AllKeyState[256] = { 0 };		//���ׂẴL�[�̏�Ԃ�����
int OldAllKeyState[256] = { 0 };	//���ׂẴL�[�̏��(���O)������
int GameScene;		//�Q�[���V�[�����Ǘ�
int stage;
int gravity;
CNT FallTime;
int WalkCheckR;
int WalkCheckL;
MUSIC TitleBGM;
MUSIC Stage1BGM;

GAME_MAP_KIND stage1Data[GAME_MAP_TATE_MAX][GAME_MAP_YOKO_MAX]{
	ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,
	ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,
	ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,
	ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,
	ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,
	ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ba,
	aa,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ad,
	aa,ab,ac,ab,ad,aa,aa,ab,ac,ab,ac,ab,ad,aa,aa,aa,aa,ab,ac,ab,ad,aa,aa,ab,ac,ab,ac,ab,ad,aa,aa,aa,
	ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,
};

VOID MY_FPS_UPDATE(VOID);			//FPS�l���v���A�X�V����֐�
VOID MY_FPS_DRAW(VOID);				//FPS�l��`�悷��֐�
VOID MY_FPS_WAIT(VOID);				//FPS�l���v�����A�҂֐�
VOID MY_ALL_KEYDOWN_UPDATE(VOID);	//�L�[�̓��͏�Ԃ��X�V����
BOOL MY_KEY_DOWN(int);				//�L�[�������Ă��邩�A�L�[�R�[�h�Ŕ��f����
BOOL MY_KEY_UP(int);				//�L�[�������グ�����A�L�[�R�[�h�Ŕ��f����
BOOL MY_KEYDOWN_KEEP(int, int);	//�L�[�����������Ă��邩�A�L�[�R�[�h�Ŕ��f����
VOID MY_START(VOID);		//�X�^�[�g���
VOID MY_START_PROC(VOID);	//�X�^�[�g��ʂ̏���
VOID MY_START_DRAW(VOID);	//�X�^�[�g��ʂ̕`��

VOID MY_PLAY(VOID);			//�v���C���
VOID MY_PLAY_PROC(VOID);	//�v���C��ʂ̏���
VOID MY_PLAY_DRAW(VOID);	//�v���C��ʂ̕`��

VOID MY_END(VOID);			//�G���h���
VOID MY_END_PROC(VOID);		//�G���h��ʂ̏���
VOID MY_END_DRAW(VOID);		//�G���h��ʂ̕`��

BOOL MY_LOAD_IMAGE(VOID);		//�摜���܂Ƃ߂ēǂݍ��ފ֐�
VOID MY_DELETE_IMAGE(VOID);		//�摜���܂Ƃ߂č폜����֐�
VOID PLAYER_MOVE(VOID);
VOID PLAYER_JUMP(VOID);
VOID COLL_PROC(VOID);
VOID STAGE_SCROLL(VOID);
BOOL MY_CHECK_RECT_COLL(RECT, RECT);
INT MY_CHECK_MAP1_COLL(RECT,int*,int*);
BOOL MY_LOAD_MUSIC(VOID);
VOID MY_DELETE_MUSIC(VOID);
VOID PLAYER_ATTACK_PROC(VOID);
VOID PLAYER_ATTACK_DRAW(VOID);



int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	ChangeWindowMode(TRUE);			//�E�B���h�E���[�h�ɐݒ�
	SetGraphMode(GAME_WIDTH, GAME_HEIGHT, GAME_COLOR);	//�w��̐��l�ŃE�B���h�E��\������
	SetWindowStyleMode(GAME_WINDOW_BAR);	//�^�C�g���o�[�̓f�t�H���g�ɂ���
	SetMainWindowText(TEXT(GAME_WINDOW_NAME));	//�E�B���h�E�̃^�C�g���̕���

	if (DxLib_Init() == -1) { return -1; }	//�c�w���C�u��������������
	SetDrawScreen(DX_SCREEN_BACK);	//Draw�n�֐��͗���ʂɕ`��
	if (MY_LOAD_IMAGE() == FALSE) { return -1; }
	if (MY_LOAD_MUSIC() == FALSE) { return -1; }
	while (TRUE)
	{
		if (ProcessMessage() != 0) { break; }	//���b�Z�[�W�����̌��ʂ��G���[�̂Ƃ��A�����I��

		if (ClearDrawScreen() != 0) { break; }	//��ʂ������ł��Ȃ������Ƃ��A�����I��

		MY_ALL_KEYDOWN_UPDATE();				//�����Ă���L�[��Ԃ��擾
		MY_FPS_UPDATE();	//FPS�̏���[�X�V]

		switch (GameScene)
		{
		case GAME_SCENE_START:
			MY_START();	//�X�^�[�g���
			break;
		case GAME_SCENE_PLAY:
			MY_PLAY();	//�v���C���
			break;
		case GAME_SCENE_END:
			MY_END();	//�G���h���
			break;
		}

		ScreenFlip();		//���j�^�̃��t���b�V�����[�g�̑����ŗ���ʂ��ĕ`��
	}

	MY_DELETE_IMAGE();
	MY_DELETE_MUSIC();
	DxLib_End();	//�c�w���C�u�����g�p�̏I������

	return 0;
}

VOID MY_FPS_UPDATE(VOID)
{
	if (CountFps == 0) //1�t���[���ڂȂ玞�����L��
	{
		StartTimeFps = GetNowCount();
	}

	if (CountFps == SampleNumFps) //60�t���[���ڂȂ畽�ς��v�Z
	{
		int now = GetNowCount();
		//���݂̎��Ԃ���A0�t���[���ڂ̎��Ԃ������AFPS�̐��l�Ŋ��遁1FPS�ӂ�̕��ώ��Ԃ��v�Z�����
		CalcFps = 1000.f / ((now - StartTimeFps) / (float)SampleNumFps);
		CountFps = 0;
		StartTimeFps = now;
	}
	CountFps++;
	return;
}

VOID MY_FPS_DRAW(VOID)
{
	//�������`��
	DrawFormatString(0, GAME_HEIGHT - 20, GetColor(255, 255, 255), "FPS:%.1f", CalcFps);
	return;
}

VOID MY_FPS_WAIT(VOID)
{
	int resultTime = GetNowCount() - StartTimeFps;					//������������
	int waitTime = CountFps * 1000 / GAME_FPS - resultTime;	//�҂ׂ�����

	if (waitTime > 0)		//�w���FPS�l���������ꍇ
	{
		WaitTimer(waitTime);	//�҂�
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

	GetHitKeyStateAll(TempKey); // �S�ẴL�[�̓��͏�Ԃ𓾂�

	for (int i = 0; i < 256; i++)
	{
		if (TempKey[i] != 0)	//������Ă���L�[�̃L�[�R�[�h�������Ă���Ƃ�
		{
			AllKeyState[i]++;	//������Ă���
		}
		else
		{
			AllKeyState[i] = 0;	//������Ă��Ȃ�
		}
	}
	return;
}


BOOL MY_KEY_DOWN(int KEY_INPUT_)
{
	//�L�[�R�[�h�̃L�[�������Ă��鎞
	if (AllKeyState[KEY_INPUT_] != 0)
	{
		return TRUE;	//�L�[�������Ă���
	}
	else
	{
		return FALSE;	//�L�[�������Ă��Ȃ�
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


BOOL MY_KEYDOWN_KEEP(int KEY_INPUT_, int DownTime)
{
	int UpdateTime = DownTime * GAME_FPS;

	if (AllKeyState[KEY_INPUT_] > UpdateTime)
	{
		return TRUE;	//���������Ă���
	}
	else
	{
		return FALSE;	//���������Ă��Ȃ�
	}
}

VOID MY_START(VOID)
{
	MY_START_PROC();	//�X�^�[�g��ʂ̏���
	MY_START_DRAW();	//�X�^�[�g��ʂ̕`��

	return;
}

//�X�^�[�g��ʂ̏���
VOID MY_START_PROC(VOID)
{
	if (CheckSoundMem(TitleBGM.handle) == 0)
	{
		//BGM�̉��ʂ�������
		ChangeVolumeSoundMem(255 * 50 / 100, TitleBGM.handle);	//50%�̉��ʂɂ���

		//BGM�𗬂�
		//DX_PLAYTYPE_NORMAL:�@�m�[�}���Đ�
		//DX_PLAYTYPE_BACK  : �o�b�N�O���E���h�Đ�
		//DX_PLAYTYPE_LOOP  : ���[�v�Đ�
		PlaySoundMem(TitleBGM.handle, DX_PLAYTYPE_LOOP);
	}
	//�G���^�[�L�[����������A�v���C�V�[���ֈړ�����
	if (MY_KEY_DOWN(KEY_INPUT_RETURN) == TRUE)
	{
		if (CheckSoundMem(TitleBGM.handle) != 0)
		{
			StopSoundMem(TitleBGM.handle);	//BGM���~�߂�
		}
		player.IsDraw = TRUE;
		player.x = 1;
		player.y = 0;
		player.CenterX = player.x + player.width / 2;
		player.CenterY = player.y + player.height / 2;
		player.AttackInterval.CntMax = 20;
		stage = 1;
		gravity = 10;
		FallTime.CntMax = 10;
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
		GameScene = GAME_SCENE_PLAY;
	}
	

	return;
}

//�X�^�[�g��ʂ̕`��
VOID MY_START_DRAW(VOID)
{
	DrawGraph(0, 0, TitleBack.handle, true);
	DrawGraph(GAME_WIDTH / 2 - TitleRogo.width / 2,
			  GAME_HEIGHT / 2 - TitleRogo.height / 2,
			  TitleRogo.handle, true);
	DrawString(0, 0, "�X�^�[�g���(�G���^�[�L�[�������ĉ�����)", GetColor(255, 255, 255));
	return;
}

//�v���C���
VOID MY_PLAY(VOID)
{
	MY_PLAY_PROC();	//�v���C��ʂ̏���
	MY_PLAY_DRAW();	//�v���C��ʂ̕`��

	return;
}

//�v���C��ʂ̏���
VOID MY_PLAY_PROC(VOID)
{
	if (CheckSoundMem(Stage1BGM.handle) == 0)
	{
		ChangeVolumeSoundMem(255 * 50 / 100, Stage1BGM.handle);	//50%�̉��ʂɂ���
		PlaySoundMem(Stage1BGM.handle, DX_PLAYTYPE_LOOP);
	}
	PLAYER_MOVE();
	STAGE_SCROLL();
	COLL_PROC();
	if (MY_KEY_DOWN(KEY_INPUT_SPACE) == TRUE)
	{
		if (CheckSoundMem(Stage1BGM.handle) != 0)
		{
			StopSoundMem(Stage1BGM.handle);	//BGM���~�߂�
		}
		GameScene = GAME_SCENE_END;
	}

	return;
}

//�v���C��ʂ̕`��
VOID MY_PLAY_DRAW(VOID)
{
	DrawGraph(0, 0, StageBack[stage].handle, true);
	if (player.IsDraw == TRUE)
	{
		DrawGraph(
			player.x,
			player.y,
			player.handle[player.change.NowImage],
			TRUE);
		player.change.CntMax = 20;

		if (player.status == PLAYER_STATUS_MOVE_R)
		{
			WalkCheckR++;
			WalkCheckL = 0;
			if (WalkCheckR == 1)
			{
				player.change.cnt = 20;
			}
		}
		if (player.status == PLAYER_STATUS_MOVE_L)
		{
			WalkCheckR = 0;
			WalkCheckL++;
			if (WalkCheckL == 1)
			{
				player.change.cnt = 20;
			}
		}

		if (player.status == PLAYER_STATUS_STOP)
		{
			if(player.muki == MUKI_R)
			player.change.NowImage = PLAYER_DIV_STOP_R;
			if (player.muki == MUKI_L)
			player.change.NowImage = PLAYER_DIV_STOP_L;
		}
		else if (player.change.cnt < player.change.CntMax)
		{
			player.change.cnt++;
		}
		else
		{
			if (player.status == PLAYER_STATUS_MOVE_R)
			{
				if (player.change.NowImage < PLAYER_DIV_WALK_R + PLAYER_DIV_TATE -1)
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
				WalkCheckR = 0;
				WalkCheckL++;
				if (WalkCheckL == 1)
				{
					player.change.cnt = 20;
				}
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
			player.change.cnt = 0;
		}
	}
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
	DrawBox(player.coll.left,player.coll.top,player.coll.right,player.coll.bottom, GetColor(0, 0, 255), FALSE);
	DrawBox(player.CheckRightColl.left, player.CheckRightColl.top,
			player.CheckRightColl.right, player.CheckRightColl.bottom,
			GetColor(255, 0, 0), FALSE);
	DrawBox(player.CheckLeftColl.left, player.CheckLeftColl.top,
			player.CheckLeftColl.right, player.CheckLeftColl.bottom,
			GetColor(0, 255, 0), FALSE);
	PLAYER_ATTACK_DRAW();
	return;
}


VOID MY_END(VOID)
{
	MY_END_PROC();	
	MY_END_DRAW();	
	return;
}

//�G���h��ʂ̏���
VOID MY_END_PROC(VOID)
{
	if (MY_KEY_DOWN(KEY_INPUT_ESCAPE) == TRUE)
	{
		GameScene = GAME_SCENE_START;
	}
	return;
}


VOID MY_END_DRAW(VOID)
{
	
	DrawBox(10, 10, GAME_WIDTH - 10, GAME_HEIGHT - 10, GetColor(0, 0, 255), TRUE);

	DrawString(0, 0, "�G���h���(�G�X�P�[�v�L�[�������ĉ�����)", GetColor(255, 255, 255));
	return;
}

BOOL MY_LOAD_IMAGE(VOID)
{
	strcpy_s(TitleRogo.path, IMAGE_TITLE_ROGO_PATH);		
	TitleRogo.handle = LoadGraph(TitleRogo.path);
	strcpy_s(TitleBack.path, IMAGE_TITLE_BACK_PATH);
	TitleBack.handle = LoadGraph(TitleBack.path);
	GetGraphSize(TitleRogo.handle, &TitleRogo.width, &TitleRogo.height);	
	strcpy_s(player.attack[0].image.path, IMAGE_PLAYER_TAMA_PATH);
	player.attack[0].image.handle = LoadGraph(player.attack[0].image.path);
	GetGraphSize(player.attack[0].image.handle, &player.attack[0].image.width, &player.attack[0].image.height);
	for (int i = 1; i < STAGE_MAX; i++)
	{
		switch (i)
		{
		case 1:
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
	return TRUE;
}

VOID MY_DELETE_IMAGE(VOID)
{
	DeleteGraph(TitleRogo.handle);
	DeleteGraph(TitleBack.handle);
	for (int i = 0; i < STAGE_MAX; i++)
	{
		DeleteGraph(StageBack[i].handle);
	}
	DeleteGraph(player.handle[0]);
	DeleteGraph(player.attack[0].image.handle);
	return;
}

BOOL MY_LOAD_MUSIC(VOID)
{
	strcpy_s(TitleBGM.path, MUSIC_TITLE_BGM_PATH);		
	TitleBGM.handle = LoadSoundMem(TitleBGM.path);
	strcpy_s(Stage1BGM.path, MUSIC_STAGE1_BGM_PATH);		
	Stage1BGM.handle = LoadSoundMem(Stage1BGM.path);
	return TRUE;
}

VOID MY_DELETE_MUSIC(VOID)
{
	DeleteSoundMem(TitleBGM.handle);
	DeleteSoundMem(Stage1BGM.handle);
}

VOID PLAYER_MOVE(VOID)
{
	player.IsMove = FALSE;
	if (player.CanMove == TRUE)
	{
		if (MY_KEY_DOWN(KEY_INPUT_UP) == TRUE)
		{
			if (player.CanJump == TRUE)
			{
				player.status = PLAYER_STATUS_JUMP;
			}
		}
		if (MY_KEY_DOWN(KEY_INPUT_RIGHT) == TRUE && MY_KEY_DOWN(KEY_INPUT_LEFT) != TRUE)
		{
			if (player.IsScroll == FALSE && player.CanRightMove == TRUE)
			{
				player.CenterX += 5;
			}
			player.IsMove = TRUE;
			if (player.status != PLAYER_STATUS_JUMP)
			{
				player.status = PLAYER_STATUS_MOVE_R;
			}
			player.muki = MUKI_R;
		}
		if (MY_KEY_DOWN(KEY_INPUT_LEFT) == TRUE && MY_KEY_DOWN(KEY_INPUT_RIGHT) != TRUE)
		{
			if (player.x > 0 && player.CanLeftMove == TRUE)
			{
				player.CenterX -= 5;
			}
			player.IsMove = TRUE;
			if (player.status != PLAYER_STATUS_JUMP)
			{
				player.status = PLAYER_STATUS_MOVE_L;
			}
			player.muki = MUKI_L;
		}
		if (MY_KEY_DOWN(KEY_INPUT_DOWN) == TRUE)
		{
		}
		if (player.status == PLAYER_STATUS_JUMP)
		{
			PLAYER_JUMP();
		}
	}
	PLAYER_ATTACK_PROC();
	player.x = player.CenterX - player.width / 2;
	player.y = player.CenterY - player.height / 2;

	if (MY_KEY_DOWN(KEY_INPUT_RIGHT) != TRUE && MY_KEY_DOWN(KEY_INPUT_LEFT) != TRUE && player.status != PLAYER_STATUS_JUMP)
	{
		player.status = PLAYER_STATUS_STOP;
	}
}



VOID PLAYER_JUMP(VOID)
{
	if (player.JumpCnt < player.JumpMax)
	{
		player.CenterY -= player.JumpRise;
		if (player.JumpCnt > 7 && player.JumpRise >= 0)
		{
			player.JumpRise--;
		}
		player.status = PLAYER_STATUS_JUMP;
	}
	else
	{
		player.status = PLAYER_STATUS_STOP;
		player.JumpCnt = 0;
		player.JumpRise = 10;
	}
	player.JumpCnt++;
}

VOID PLAYER_ATTACK_PROC(VOID)
{
	if (MY_KEY_DOWN(KEY_INPUT_V) == TRUE)
	{
		if (player.CanAttack == TRUE)
		{
			player.CanAttack = FALSE;
			for (int i = 0; i < PLAYER_TAMA_MAX; i++)
			{
				if (player.attack[i].IsDraw == FALSE)
				{
					if (player.muki == MUKI_R)
					{
						player.attack[i].muki = MUKI_R;
					}
					if (player.muki == MUKI_L)
					{
						player.attack[i].muki = MUKI_L;
					}
					if (player.attack[i].muki == MUKI_R)
					{
						player.attack[i].image.x = player.x + player.width;
					}
					if (player.attack[i].muki == MUKI_L)
					{
						player.attack[i].image.x = player.x;
					}
					player.attack[i].image.y = player.CenterY;
					player.attack[i].IsDraw = TRUE;
					break;
				}
			}
		}
	}
	if (player.CanAttack == FALSE)
	{
		if (player.AttackInterval.cnt == player.AttackInterval.CntMax)
		{
			player.AttackInterval.cnt = 0;
			player.CanAttack = TRUE;
		}
		player.AttackInterval.cnt++;
	}
	return;
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
					player.attack[i].image.x += 30;
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
					player.attack[i].image.x -= 30;
				}
			}
			
		}
	}
}

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
	player.coll.right = player.x + player.width - 25;
	player.coll.left = player.x + 25;
	player.coll.top = player.y;
	player.coll.bottom = player.y + player.height;

	player.CheckBottomColl.right = player.coll.right;
	player.CheckBottomColl.left = player.coll.left;
	player.CheckBottomColl.top = player.coll.top;
	player.CheckBottomColl.bottom = player.coll.bottom + gravity;

	player.CheckRightColl.right = player.coll.right + 1;
	player.CheckRightColl.left = player.coll.left + 5;
	player.CheckRightColl.top = player.coll.top;
	player.CheckRightColl.bottom = player.coll.bottom;

	player.CheckLeftColl.right = player.coll.right - 5;
	player.CheckLeftColl.left = player.coll.left - 1;
	player.CheckLeftColl.top = player.coll.top;
	player.CheckLeftColl.bottom = player.coll.bottom;


	player.CanMove = TRUE;
	player.CanRightMove = TRUE;
	player.CanLeftMove = TRUE;
	player.CanJump = TRUE;
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
	int x, y;
	if (MY_CHECK_MAP1_COLL(player.CheckBottomColl,&x,&y) == -1)
	{
		player.CanJump = FALSE;
		if (player.status != PLAYER_STATUS_JUMP)
		{
			player.CenterY += gravity;
			if (FallTime.cnt < FallTime.CntMax)
			{
				FallTime.cnt++;
			}
			else
			{
				gravity += 1;	
				FallTime.cnt = 0;
			}
		}
	}
	if(MY_CHECK_MAP1_COLL(player.CheckBottomColl, &x, &y) == BLOCK)
	{
		player.CenterY = stage1[x][y].y - player.height / 2 - 1;
		FallTime.cnt = 0;
		gravity = 10;
	}
	if (MY_CHECK_MAP1_COLL(player.CheckRightColl, &x, &y) == BLOCK)
	{
		player.CanRightMove = FALSE;
	}
	if (MY_CHECK_MAP1_COLL(player.CheckLeftColl, &x, &y) == BLOCK)
	{
		player.CanLeftMove = FALSE;
	}
	if (MY_CHECK_MAP1_COLL(player.coll, &x, &y) == GOAL)
	{
		if (CheckSoundMem(Stage1BGM.handle) != 0)
		{
			StopSoundMem(Stage1BGM.handle);	//BGM���~�߂�
		}
		GameScene = GAME_SCENE_END;
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
				if (stage1[tate][yoko].kind != ae && stage1[tate][yoko].kind != ba)
				{
					*x = tate;
					*y = yoko;
					return BLOCK;
				}
				if (stage1[tate][yoko].kind == ba)
				{
					return GOAL;
				}
			}
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
					stage1[tate][yoko].x -= 5;
				}
			}
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
					stage1[tate][yoko].x += 5;
				}
			}
		}
	}
	else
	{
		player.IsScroll = FALSE;
	}
}