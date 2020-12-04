#include "DxLib.h"

#define GAME_WIDTH	1280	
#define GAME_HEIGHT	720
#define GAME_COLOR	32

#define GAME_WINDOW_BAR	 0
#define GAME_WINDOW_NAME	"GAME TITLE"
#define GAME_FPS			60
#define PATH_MAX			255
#define NAME_MAX			255
#define PLAYER_WALK_DIV_WIDTH  80
#define PLAYER_WALK_DIV_HEIGHT  120
#define PLAYER_WALK_DIV_TATE   8
#define PLAYER_WALK_DIV_YOKO   1
#define PLAYER_WALK_DIV_NUM   PLAYER_WALK_DIV_TATE * PLAYER_WALK_DIV_YOKO

#define MAP_DIV_WIDTH     80
#define MAP_DIV_HEIGHT    80
#define MAP_DIV_TATE      8
#define MAP_DIV_YOKO      8
#define MAP_DIV_NUM  MAP_DIV_TATE * MAP_DIV_YOKO

#define GAME_MAP_TATE_MAX   9
#define GAME_MAP_YOKO_MAX   16
#define GAME_MAP_KIND_MAX  2

#define STAGE_MAX 5

#define IMAGE_PLAYER_PATH       TEXT(".\\IMAGE\\player.png")
#define IMAGE_TITLE_BACK_PATH   TEXT(".\\IMAGE\\TitleBack.png")
#define IMAGE_TITLE_ROGO_PATH   TEXT(".\\IMAGE\\TitleRogo.png")
#define IMAGE_STAGE1_BACK_PATH  TEXT(".\\IMAGE\\Stage1Back.png")
#define IMAGE_MAP1_PATH         TEXT(".\\IMAGE\\map1.png")

enum GAME_MAP_KIND
{
	aa = 0,
	ab = 1,
	ac = 2,
	ad = 3,
	ae = 4,
};

typedef struct STRUCT_IMAGE  
{
	char path[PATH_MAX];		//�p�X
	int handle;					//�n���h��
	int x;						//X�ʒu
	int y;						//Y�ʒu
	int width;					//��
	int height;					//����
}IMAGE;	//�摜�\����

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
}MAP;

typedef struct STRUCT_PLAYER
{
	int x;
	int y;
	int width;
	int height;
	char path[PATH_MAX];
	int handle[PLAYER_WALK_DIV_NUM];
	int CenterX;
	int CenterY;
	int status;
	BOOL IsDraw;
	RECT coll;
	CHANGE_IMAGE change;
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

GAME_MAP_KIND stage1Data[GAME_MAP_TATE_MAX][GAME_MAP_YOKO_MAX]{
	ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,
	ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,
	ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,
	ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,
	ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,
	ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,
	ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,
	aa,ab,ac,ab,ad,ae,aa,ab,ac,ab,ac,ab,ad,ae,ae,ae,
	ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,
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

VOID COLL_PROC(VOID);
BOOL MY_CHECK_RECT_COLL(RECT, RECT);

enum GAME_SCENE {
	GAME_SCENE_START,
	GAME_SCENE_PLAY,
	GAME_SCENE_END,
};	//�Q�[���̃V�[��

//########## �v���O�����ōŏ��Ɏ��s�����֐� ##########
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	ChangeWindowMode(TRUE);			//�E�B���h�E���[�h�ɐݒ�
	SetGraphMode(GAME_WIDTH, GAME_HEIGHT, GAME_COLOR);	//�w��̐��l�ŃE�B���h�E��\������
	SetWindowStyleMode(GAME_WINDOW_BAR);	//�^�C�g���o�[�̓f�t�H���g�ɂ���
	SetMainWindowText(TEXT(GAME_WINDOW_NAME));	//�E�B���h�E�̃^�C�g���̕���

	if (DxLib_Init() == -1) { return -1; }	//�c�w���C�u��������������
	SetDrawScreen(DX_SCREEN_BACK);	//Draw�n�֐��͗���ʂɕ`��
	if (MY_LOAD_IMAGE() == FALSE) { return -1; }

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
	char TempKey[256];	//�ꎞ�I�ɁA���݂̃L�[�̓��͏�Ԃ��i�[����

	//���O�̃L�[���͂��Ƃ��Ă���
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
	//�G���^�[�L�[����������A�v���C�V�[���ֈړ�����
	if (MY_KEY_DOWN(KEY_INPUT_RETURN) == TRUE)
	{
		player.IsDraw = TRUE;
		player.x = 0;
		player.y = 380;
		player.CenterX = player.x + player.width / 2;
		player.CenterY = player.y + player.height / 2;
		stage = 1;
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
	player.CenterX = player.x + player.width / 2;
	player.CenterY = player.y + player.height / 2;
	if (MY_KEY_DOWN(KEY_INPUT_UP) == TRUE)
	{
	}
	if (MY_KEY_DOWN(KEY_INPUT_RIGHT) == TRUE)
	{
		player.x += 5;
	}
	if (MY_KEY_DOWN(KEY_INPUT_LEFT) == TRUE)
	{
		player.x -= 5;
	}
	if (MY_KEY_DOWN(KEY_INPUT_DOWN) == TRUE)
	{
	}
	if (MY_KEY_DOWN(KEY_INPUT_SPACE) == TRUE)
	{
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
			player.CenterX,
			player.CenterY,
			player.handle[player.change.NowImage],
			TRUE);
		player.change.CntMax = 20;
		if (player.change.cnt < player.change.CntMax)
		{
			player.change.cnt++;
		}
		else
		{
			if (player.change.NowImage < PLAYER_WALK_DIV_NUM - 1)
			{
				player.change.NowImage++;
			}
			else
			{
				player.change.NowImage = 0;
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
		}
	}
	return;
}

//�G���h���
VOID MY_END(VOID)
{
	MY_END_PROC();	//�G���h��ʂ̏���
	MY_END_DRAW();	//�G���h��ʂ̕`��
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

//�G���h��ʂ̕`��
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
		PLAYER_WALK_DIV_NUM, PLAYER_WALK_DIV_TATE, PLAYER_WALK_DIV_YOKO,
		PLAYER_WALK_DIV_WIDTH, PLAYER_WALK_DIV_HEIGHT,
		&player.handle[0]);
	GetGraphSize(player.handle[0], &player.width, &player.height);

	for (int i = 1; i < PLAYER_WALK_DIV_NUM; i++)
	{
		player.handle[i] = player.handle[i];
	}

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
	return;
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
	player.coll.right = player.x + player.width;
	player.coll.left = player.x;
	player.coll.top = player.y;
	player.coll.bottom = player.y + player.height;


}