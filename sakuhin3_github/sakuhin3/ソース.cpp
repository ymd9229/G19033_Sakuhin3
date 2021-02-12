#include "DxLib.h"
#include <stdlib.h>
#include <math.h>

#define GAME_WIDTH	1280	//��ʂ̉��̃T�C�Y
#define GAME_HEIGHT	720		//��ʂ̏c�̃T�C�Y
#define GAME_COLOR	32

#define GAME_WINDOW_BAR	 0
#define GAME_WINDOW_NAME	"���̖{"	//�Q�[���^�C�g��
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
#define ENEMY_DATA_MAX 2	//�G�̃f�[�^�̐�
#define ENEMY_MAX 10		//�����ɏo�����邱�Ƃ��o����G�̐�
#define ENEMY_IMAGE_KIND 10	//�G�̉摜�̎�ނ̐�

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

#define GAME_MAP_TATE_MAX   9	//�}�b�v�̏c�̃}�X�̐�
#define GAME_MAP_YOKO_MAX   32	//�}�b�v�̉��̃}�X�̐�
#define GAME_MAP_KIND_MAX  2

#define PLAYER_TAMA_MAX 2
#define PLAYER_MAGIC_MAX 3
#define MAGIC_DATA_MAX 2


#define STAGE_MAX 1	//�X�e�[�W��

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
#define IMAGE_BOOK_PATH       �@TEXT(".\\IMAGE\\book.png")
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

enum END_TYPE	//�G���h��ʂ̎��
{
	GAME_CLEAR,//�N���A
	GAME_OVER,//�Q�[���I�[�o�[
};

enum GAME_SCENE	//�V�[���̎�� 
{
	GAME_SCENE_START,	//�X�^�[�g���
	GAME_SCENE_PLAY,	//�v���C���
	GAME_SCENE_END,		//�G���h���
	GAME_SCENE_POSE		//�|�[�Y���
};

enum PLAYER_STATUS	//�v���C���[�̏��
{
	PLAYER_STATUS_MOVE_R,	//�E�ɕ����Ă�����
	PLAYER_STATUS_MOVE_L,	//���ɕ����Ă�����
	PLAYER_STATUS_STOP,		//�����~�܂��Ă�����
	PLAYER_STATUS_JUMP,		//�W�����v���Ă�����
	PLAYER_STATUS_SQUAT,	//���Ⴊ��ł�����
};

enum MUKI	//����
{
	MUKI_R,//�E
	MUKI_L,//��
};

enum CHECK_MAP_COLL		//�}�b�v�̔��育�Ƃ̎��
{
	BLOCK,//�u���b�N�i��Q���j
	AIR,	//��C�i���薳���j
	GOAL,	//�S�[���n�_
	BOOK,	//���@�̖{
	WOOD,
};

enum SPEED_KIND		//�l�X�ȑ��x
{
	ENEMY_SPEED_SLOW = 1,		//�x���G�̑��x
	PLAYER_SPEED_NORMAL = 5,	//�v���C���[�̒ʏ펞�̑��x
	PLAYER_TAMA_SPEED = 30,		//�v���C���[�̒ʏ�U���̒e�̑��x
};

//########�\����########

typedef struct STRUCT_CNT	//�J�E���g�̍\����
{
	int cnt;
	int CntMax;
}CNT;

typedef struct STRUCT_IMAGE  //�摜�̍\����
{
	char path[PATH_MAX];		
	int handle;					
	int x;						
	int y;						
	int width;					
	int height;					
}IMAGE;	

typedef struct STRUCT_MOVIE	//����̍\����
{
	char path[PATH_MAX];
	int handle;
	int x;
	int y;
	int width;
	int height;
	BOOL IsViewed = FALSE;
}MOVIE;

typedef struct STRUCT_MUSIC	//���y�̍\����
{
	char path[PATH_MAX];		
	int handle;					
}MUSIC;

typedef struct STRUCT_CHANGE_IMAGE	//�摜��ς��鎞�Ɏg���\����
{
	int NowImage;
	int cnt;
	int CntMax;
}CHANGE_IMAGE;

typedef struct STRUCT_MAP_IMAGE	//�}�b�v�`�b�v�̍\����
{
	char path[PATH_MAX];
	int handle[MAP_DIV_NUM];
	int kind[MAP_DIV_NUM];
	int x;
	int y;
	int width;
	int height;
}MAPCHIP;

typedef struct STRUCT_MAP	//�}�b�v�̍\����
{
	GAME_MAP_KIND kind;
	int x;
	int y;
	int width;
	int height;
	RECT coll;
}MAP;

typedef struct STRUCT_COLL	//����̍\����
{
	RECT base;
	RECT CheckItem;
	RECT CheckLeft;
	RECT CheckRight;
	RECT CheckTop;
	RECT CheckBottom;
}COLL;

typedef struct STRUCT_PLAYER_ATTACK	//�v���C���[�̍U���̍\����
{
	IMAGE image;
	BOOL IsDraw;
	CNT cnt;
	int muki;
	RECT coll;
}PLAYER_ATTACK;

typedef struct STRUCT_LIFE	//�̗͂̍\����
{
	int max;
	int now = max;
	BOOL invincible;
	CNT InvincibleCnt;
	IMAGE image;
}LIFE;

typedef struct STRUCT_PLAYER	//�v���C���[�̍\����
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

typedef struct STRUCT_MAGIC	//���@�̍\����
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

typedef struct STRUCT_MAGIC_DATA	//���@�̃f�[�^�̍\����
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

typedef struct STRUCT_MAGIC_ICON	//���@�̃A�C�R���̍\����
{
	char path[PATH_MAX];
	int handle[MAGIC_ICON_DIV_NUM];
	int x;
	int y;
	int width;
	int height;
}MAGIC_ICON;

typedef struct STRUCT_ENEMY		//�G�̍\����
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

typedef struct STRUCT_ENEMY_DATA	//�G�̃f�[�^�̍\����
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

//########�O���[�o���ϐ�########

ENEMY_DATA EnemyData[ENEMY_DATA_MAX] = {
	//�o�G�̏o���ʒuX�A�G�̏o���ʒuY�A�G�̎�ށA��s���邩�ǂ����p
	{700,480,0,FALSE},
	{1000,240,1,TRUE},
};	//�G�̃f�[�^
ENEMY_DATA EnemyImage[ENEMY_IMAGE_KIND];
ENEMY enemy[ENEMY_MAX];
IMAGE TitleBack;		//�^�C�g���w�i
IMAGE TitleRogo;		//�^�C�g�����S
IMAGE ClearRogo;		//�N���A���̃��S
IMAGE OverRogo;			//�Q�[���I�[�o�[���̃��S
IMAGE ClearBack;		//�N���A���̔w�i
IMAGE OverBack;			//�Q�[���I�[�o�[���̔w�i
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
int EndKind;                    //�N���A���Q�[���I�[�o�[��
int StartTimeFps;				//����J�n����
int CountFps;					//�J�E���^
float CalcFps;					//�v�Z����
int SampleNumFps = GAME_FPS;	//���ς����T���v����
int AllKeyState[256] = { 0 };		//���ׂẴL�[�̏�Ԃ�����
int OldAllKeyState[256] = { 0 };	//���ׂẴL�[�̏��(���O)������
BOOL IsPrecedence[256];
int GameScene;					//�Q�[���V�[�����Ǘ�
int stage;						//���X�e�[�W�ڂ�
int gravity;					//�d��
CNT FallTime;					//�������Ă��鎞�Ԃ��J�E���g����p
CNT precdence[256];
int WalkCheckR;					//�E�ɕ����Ă��邩���ׂ�悤
int WalkCheckL;					//���ɕ����Ă��邩���ׂ�悤
int JumpBuff = 0;
int AvailableMagic = 0;	//�g�p�ł��閂�@�̐�
MUSIC TitleBGM;			//�^�C�g���̉��y
MUSIC Stage1BGM;		//�X�e�[�W�P�̉��y
MUSIC GameClearBGM;		//�N���A�������̉��y
MUSIC AttackSE;			//�ʏ�U�����̌��ʉ�
MUSIC DefeatSE;			//�G��|�����Ƃ��̌��ʉ�
MUSIC GoalSE;			//�S�[�������Ƃ��̌��ʉ�
MUSIC HitSE;			//�_���[�W���󂯂��Ƃ��̌��ʉ�
RECT screen;
MOVIE PoseIn;			//�|�[�Y��ʂɓ���Ƃ��̓���


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

VOID MY_FPS_UPDATE(VOID);			//FPS�l���v���A�X�V����֐�
VOID MY_FPS_DRAW(VOID);				//FPS�l��`�悷��֐�
VOID MY_FPS_WAIT(VOID);				//FPS�l���v�����A�҂֐�

VOID MY_ALL_KEYDOWN_UPDATE(VOID);	//�L�[�̓��͏�Ԃ��X�V����
BOOL MY_KEY_DOWN(int);				//�L�[�������Ă��邩�A�L�[�R�[�h�Ŕ��f����
BOOL MY_KEY_UP(int);				//�L�[�������グ�����A�L�[�R�[�h�Ŕ��f����
BOOL MY_KEY_DOWN_KEEP(int, int);	//�L�[�����������Ă��邩�A�L�[�R�[�h�Ŕ��f����
BOOL MY_KEY_DOWN_MOMENT(int);		//�L�[�������n�߂����A�L�[�R�[�h�Ŕ��f����
BOOL MY_KEY_DOWN_PRECEDENCE(int);	//�L�[�̐�s���͗p

VOID MY_START(VOID);		//�X�^�[�g���
VOID MY_START_PROC(VOID);	//�X�^�[�g��ʂ̏���
VOID MY_START_DRAW(VOID);	//�X�^�[�g��ʂ̕`��

VOID MY_PLAY(VOID);			//�v���C���
VOID MY_PLAY_PROC(VOID);	//�v���C��ʂ̏���
VOID MY_PLAY_DRAW(VOID);	//�v���C��ʂ̕`��

VOID MY_END(VOID);			//�G���h���
VOID MY_END_PROC(VOID);		//�G���h��ʂ̏���
VOID MY_END_DRAW(VOID);		//�G���h��ʂ̕`��

VOID MY_POSE(VOID);         //�|�[�Y���
VOID MY_POSE_PROC(VOID);	//�|�[�Y��ʂ̏���
VOID MY_POSE_DRAW(VOID);	//�|�[�Y��ʂ̕`��

BOOL MY_LOAD_IMAGE(VOID);		//�摜���܂Ƃ߂ēǂݍ��ފ֐�
VOID MY_DELETE_IMAGE(VOID);		//�摜���܂Ƃ߂č폜����֐�

BOOL MY_LOAD_MUSIC(VOID);		//���y���܂Ƃ߂ēǂݍ��ފ֐�
VOID MY_DELETE_MUSIC(VOID);		//���y���܂Ƃ߂č폜����֐�

INT CNT_CHECK(int, int);		//cnt���C���N�������g���Ēl��Ԃ��ACntMax�Ɠ����l�Ȃ�0��Ԃ��֐�

VOID PLAYER_MOVE(VOID);		//�v���C���[�̓����̏���
VOID PLAYER_DRAW(VOID);		//�v���C���[�̕`��
VOID PLAYER_JUMP(VOID);		//�v���C���[�̃W�����v�̏���
VOID PLAYER_ATTACK_PROC(VOID);	//�v���C���[�̒ʏ�U���̏���
VOID PLAYER_ATTACK_DRAW(VOID);	//�v���C���[�̒ʏ�U���̕`��
VOID PLAYER_LIFE_DRAW(VOID);

VOID STAGE_SCROLL(VOID);	//�X�N���[���̏���

VOID COLL_PROC(VOID);		//�����蔻��̏���
BOOL MY_CHECK_RECT_COLL(RECT, RECT);	
INT MY_CHECK_ENEMY_COLL(RECT);
INT MY_CHECK_MAP1_COLL(RECT,int*,int*);		//�}�b�v�̏�Q���@�΁@�G���v���C���[�̔���
INT MY_CHECK_MAP1_ACT_COLL(RECT);			//�}�b�v�̃A�C�e���@�΁@�v���C���[�̔���

VOID STAGE_DRAW(VOID);

VOID ENEMY_PROC(VOID);		//�G�̏���
VOID ENEMY_DRAW(VOID);		//�G�̕`��
INT ENEMY_CHECK(VOID);		//
VOID ENEMY_MOVE(INT);		//�G�̓����̏���

VOID MAGIC_PROC(VOID);			//���@�̏���
VOID MAGIC_DRAW(VOID);			//���@�̕`��
VOID MAGIC_SELECT(VOID);		//���@�̑I���̏���
INT MAGIC_CHECK(VOID);			//���g�p�̗v�f��T��
VOID MAGIC_EFFECT(int);			//���@���Ƃ̌��ʂ̏���
VOID MAGIC_ICON_DRAW(VOID);		//���@�̃A�C�R���̕`��

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
		case GAME_SCENE_POSE:
			MY_POSE();  //�|�[�Y���
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


BOOL MY_KEY_DOWN_KEEP(int KEY_INPUT_, int DownTime)
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

BOOL MY_KEY_DOWN_MOMENT(int KEY_INPUT_)
{
	if (MY_KEY_DOWN(KEY_INPUT_) == TRUE)
	{
		if (OldAllKeyState[KEY_INPUT_] == 0)
		{
			return TRUE;	//�����n�߂�
		}
	}
	return FALSE;	//�����n�߂Ă��Ȃ�
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
	MY_START_PROC();	//�X�^�[�g��ʂ̏���
	MY_START_DRAW();	//�X�^�[�g��ʂ̕`��

	return;
}

//�X�^�[�g��ʂ̏���
VOID MY_START_PROC(VOID)
{
	if (CheckSoundMem(TitleBGM.handle) == 0)
	{
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
		//������
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
		//��ʉE��ɖ��@�̃A�C�R��
		MagicIcon.x = GAME_WIDTH - MagicIcon.width;
		MagicIcon.y = 0;

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
	PLAYER_MOVE();		//�v���C���[�̓����̏���
	ENEMY_PROC();		//�G�̏���
	STAGE_SCROLL();		//�X�e�[�W�̃X�N���[���̏���
	COLL_PROC();		//����̏���

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
			StopSoundMem(Stage1BGM.handle);	//BGM���~�߂�
		}
		GameScene = GAME_SCENE_END;
	}

	return;
}

//�v���C��ʂ̕`��
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

//�G���h��ʂ̏���
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
			StopSoundMem(GameClearBGM.handle);	//BGM���~�߂�
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

	DrawString(0, 0, "�G���h���(�G�X�P�[�v�L�[�������ĉ�����)", GetColor(255, 255, 255));
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

VOID MY_POSE_DRAW(VOID)		//�|�[�Y��ʂ̕`��
{
	if (GetMovieStateToGraph(PoseIn.handle) == 0 &&
		PoseIn.IsViewed == FALSE)
	{
		SeekMovieToGraph(PoseIn.handle, 0);
		PlayMovieToGraph(PoseIn.handle);
	}
	DrawGraph(0, 0, PoseIn.handle, FALSE);
	DrawString(0, 0, "�|�[�Y���", GetColor(255, 255, 255));
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

	//�v���C���[��������Ƃ�
	if (player.CanMove == TRUE)	
	{
		if (MY_KEY_DOWN(KEY_INPUT_RIGHT) != TRUE &&
			MY_KEY_DOWN(KEY_INPUT_LEFT) != TRUE &&
			player.status != PLAYER_STATUS_JUMP)	//�E�ƍ��̃L�[�������Ă��Ȃ��āA��Ԃ��W�����v�ȊO�̎�
		{
			player.status = PLAYER_STATUS_STOP;		//�v���C���[�̏�Ԃ��~�܂��Ă����Ԃɂ���
		}

		//��̃L�[���������Ƃ�
		if (MY_KEY_DOWN_MOMENT(KEY_INPUT_UP) == TRUE)	
		{
			if (player.NowJumpTimes < player.CanJumpTimes)	//���݂̃W�����v�񐔂��W�����v�ł���񐔖����̎�
			{
				if (player.status != PLAYER_STATUS_JUMP)	//��Ԃ��W�����v�ȊO�̎�
				{
					player.status = PLAYER_STATUS_JUMP;		//�v���C���[�̏�Ԃ��W�����v��Ԃɂ���
					player.NowJumpTimes++;					//���݂̃W�����v���𑝂₷
				}
				else										//��Ԃ��W�����v�̎�
				{
					IsPrecedence[KEY_INPUT_UP] = TRUE;		//��̃L�[�̐�s���͊J�n
				}
			}
		}

		//���̃L�[�������Ă��Ȃ��Ƃ�
		if (MY_KEY_DOWN(KEY_INPUT_DOWN) != TRUE)
		{
			//���̃L�[�������Ă��Ȃ��A�E�̃L�[�������Ă���Ƃ�
			if (MY_KEY_DOWN(KEY_INPUT_RIGHT) == TRUE && MY_KEY_DOWN(KEY_INPUT_LEFT) != TRUE)
			{
				if (player.IsScroll == FALSE &&
					player.CanRightMove == TRUE)				//�X�N���[�����Ă��Ȃ��A�E�ɓ�����Ƃ�
				{
					player.CenterX += PLAYER_SPEED_NORMAL;		
				}
				if (player.status != PLAYER_STATUS_JUMP)		//��Ԃ��W�����v�ȊO�̎�
				{
					player.status = PLAYER_STATUS_MOVE_R;		//�v���C���[�̏�Ԃ����(�E)��Ԃɂ���
				}
				player.IsMove = TRUE;											
				player.muki = MUKI_R;							//�v���C���[�̌����͉E
			}

			//�E�̃L�[�������Ă��Ȃ��A���̃L�[�������Ă���Ƃ�
			if (MY_KEY_DOWN(KEY_INPUT_LEFT) == TRUE && MY_KEY_DOWN(KEY_INPUT_RIGHT) != TRUE)
			{
				if (player.x > 0 && player.CanLeftMove == TRUE)					
				{
					player.CenterX -= PLAYER_SPEED_NORMAL;
				}
				if (player.status != PLAYER_STATUS_JUMP)		//��Ԃ��W�����v�ȊO�̎�
				{
					player.status = PLAYER_STATUS_MOVE_L;		//�v���C���[�̏�Ԃ����(��)��Ԃɂ���
				}
				player.IsMove = TRUE;
				player.muki = MUKI_L;							//�v���C���[�̌����͍�
			}
		}

		//���̃L�[�������Ă���Ƃ�
		if (MY_KEY_DOWN(KEY_INPUT_DOWN) == TRUE)
		{
			if (player.status != PLAYER_STATUS_JUMP)			//��Ԃ��W�����v�ȊO�̎�
			{
				player.status = PLAYER_STATUS_SQUAT;			//�v���C���[�̏�Ԃ����Ⴊ�ݏ�Ԃɂ���
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
	if (MY_KEY_DOWN(KEY_INPUT_V) == TRUE)//�X�L�[���������Ƃ�
	{
		if (player.CanAttack == TRUE)//�v���C���[���U���\�Ȃ�
		{
			player.CanAttack = FALSE;	//�U������x�s�\�ɂ���
			for (int i = 0; i < PLAYER_TAMA_MAX; i++)
			{
				if (player.attack[i].IsDraw == FALSE)
				{
					//�v���C��[�̌�������e�̌������߂�(��������Ƀv���C���[�̌�����ς��Ă��e�̌����͕ς��Ȃ�)
					if (player.muki == MUKI_R)//�v���C���[���E�����̂Ƃ�
					{
						player.attack[i].muki = MUKI_R;//�e�̌����͉E
						player.attack[i].image.x = player.x + player.width;//�e�̃X�^�[�g�ʒuX
					}
					if (player.muki == MUKI_L)//�v���C���[���������̎�
					{
						player.attack[i].muki = MUKI_L;//�e�̌����͍�
						player.attack[i].image.x = player.x;//�e�̃X�^�[�g�ʒuX
					}
					player.attack[i].image.y = player.CenterY;	//�e�̃X�^�[�g�ʒuY
					player.attack[i].IsDraw = TRUE;		//�e�̕`�悪�ł���悤�ɂ���

					//�ʏ�U���̌��ʉ�
					if (CheckSoundMem(AttackSE.handle) == 0)
					{
						ChangeVolumeSoundMem(255 * 30 / 100, AttackSE.handle);//30%�̃{�����[��
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
	if (MY_KEY_DOWN(KEY_INPUT_C) == TRUE)	//C�L�[���������Ƃ�
	{
		if (player.CanMagic == TRUE)	//���@���g�����ԂȂ�
		{
			i = MAGIC_CHECK();
			if (i != -1 && player.EquipMagic != -1)
			{
				magic[i].No = player.EquipMagic;
				if (MagicData[player.EquipMagic].fiexd == TRUE)//�J�n�ʒu���Œ�̂Ƃ�
				{
					magic[i].x = MagicData[player.EquipMagic].StartX;
					magic[i].y = MagicData[player.EquipMagic].StartY;
				}
				else	//�J�n�ʒu���Œ肶��Ȃ��Ƃ�
				{
					//�v���C���[�̑O������o��
					if (player.muki == MUKI_R)//�v���C���[���E�����̂Ƃ�
					{
						magic[i].CenterX = player.x + player.width;
						magic[i].muki = MUKI_R;		//���@�̌������E�Ɍ��߂�
					}
					if (player.muki == MUKI_L)//�v���C���[���������̂Ƃ�
					{
						magic[i].CenterX = player.x;
						magic[i].muki = MUKI_L;		//���@�̌��������Ɍ��߂�
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
		if (-1 < player.EquipMagic)	//�I�𒆂̖��@�̔ԍ���-1���傫���Ƃ�(-1�͑I���Ȃ�)
			player.EquipMagic--;	//��O�̖��@��I��
		else						//�I�𖂖@��-1(�I���Ȃ�)�̂Ƃ�
			player.EquipMagic = AvailableMagic - 1;	//�Ō���̖��@��I��
	}
	else if (MY_KEY_DOWN_MOMENT(KEY_INPUT_X) == TRUE) 	
	{
		if (AvailableMagic - 1 > player.EquipMagic)	//�I�𒆂̖��@�̔ԍ����擾�ςݖ��@���̖����̂Ƃ�
			player.EquipMagic++;
		else							//�I�𖂖@���Ō���̂Ƃ�
			player.EquipMagic = -1;		//-1(�I���Ȃ�)�ɖ߂�		
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

VOID PLAYER_DRAW(VOID)	//�v���C���[�̕`��
{
	if (player.IsDraw == TRUE)	//�v���C���[��`�悷��Ƃ�
	{
		DrawGraph(
			player.x,
			player.y,
			player.handle[player.change.NowImage],
			TRUE);
		
		switch (player.status)	//�v���C���[�̏�Ԃɂ���ď�����ς���
		{
		case PLAYER_STATUS_MOVE_R:	//�v���C���[���E�����ɕ����Ă���Ƃ�
			WalkCheckR++;
			WalkCheckL = 0;
			if (WalkCheckR == 1)
			{
				player.change.cnt = player.change.CntMax;
			}
			break;

		case PLAYER_STATUS_MOVE_L:	//�v���C���[���������ɕ����Ă���Ƃ�
			WalkCheckR = 0;
			WalkCheckL++;
			if (WalkCheckL == 1)
			{
				player.change.cnt = player.change.CntMax;
			}
			break;

		case PLAYER_STATUS_STOP:	//�v���C���[���~�܂��Ă���Ƃ�
			if (player.muki == MUKI_R)	//�v���C���[���E�����̎�
				player.change.NowImage = PLAYER_DIV_STOP_R;
			if (player.muki == MUKI_L)	//�v���C���[���������̎�
				player.change.NowImage = PLAYER_DIV_STOP_L;
			break;

		case PLAYER_STATUS_SQUAT:	//�v���C���[�����Ⴊ��ł���Ƃ�
			if (player.muki == MUKI_R)	//�v���C���[���E�����̎�
				player.change.NowImage = PLAYER_DIV_SQUAT_R;
			if (player.muki == MUKI_L)	//�v���C���[���������̎�
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
		if (enemy[n].IsDraw == TRUE)//�G���`�悳��Ă���Ƃ�
		{
			enemy[n].x = enemy[n].CenterX - enemy[n].width / 2;
			enemy[n].y = enemy[n].CenterY - enemy[n].height / 2;

			//�G�̓����蔻��

			//��{�̔���
			enemy[n].coll.base.left = enemy[n].x + 15;
			enemy[n].coll.base.right = enemy[n].x + enemy[n].width - 15;
			enemy[n].coll.base.top = enemy[n].y + 15;
			enemy[n].coll.base.bottom = enemy[n].y + enemy[n].height;
			//
			enemy[n].coll.CheckTop.left = enemy[n].coll.base.left;
			enemy[n].coll.CheckTop.right = enemy[n].coll.base.right;
			enemy[n].coll.CheckTop.top = enemy[n].coll.base.top + 10;
			enemy[n].coll.CheckTop.bottom = enemy[n].coll.base.bottom;
			//�����ɏ����傫������
			enemy[n].coll.CheckBottom.left = enemy[n].coll.base.left;
			enemy[n].coll.CheckBottom.right = enemy[n].coll.base.right;
			enemy[n].coll.CheckBottom.top = enemy[n].coll.base.top;
			enemy[n].coll.CheckBottom.bottom = enemy[n].coll.base.bottom + 10;
			//�����ɏ����傫������
			enemy[n].coll.CheckLeft.left = enemy[n].coll.base.left - 5;
			enemy[n].coll.CheckLeft.right = enemy[n].coll.base.right - 5;
			enemy[n].coll.CheckLeft.top = enemy[n].coll.base.top;
			enemy[n].coll.CheckLeft.bottom = enemy[n].coll.base.bottom;
			//�E���ɏ����傫������
			enemy[n].coll.CheckRight.left = enemy[n].coll.base.left + 5;
			enemy[n].coll.CheckRight.right = enemy[n].coll.base.right + 5;
			enemy[n].coll.CheckRight.top = enemy[n].coll.base.top;
			enemy[n].coll.CheckRight.bottom = enemy[n].coll.base.bottom;


			if (MY_CHECK_MAP1_COLL(enemy[n].coll.CheckLeft, &x, &y) == BLOCK)//���ɏ�Q��
			{
				enemy[n].CanLeftMove = FALSE;
				enemy[n].CanRightMove = TRUE;
			}
			if (MY_CHECK_MAP1_COLL(enemy[n].coll.CheckRight, &x, &y) == BLOCK)//�E�ɏ�Q��
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
			//�ʏ�U���̔���
			player.attack[i].coll.left = player.attack[i].image.x;
			player.attack[i].coll.right = player.attack[i].image.x + player.attack[i].image.width;
			player.attack[i].coll.top = player.attack[i].image.y;
			player.attack[i].coll.bottom = player.attack[i].image.y + player.attack[i].image.height;

			if (MY_CHECK_MAP1_COLL(player.attack[i].coll, &x, &y) == BLOCK)//��Q���ɓ��������Ƃ�
			{
				player.attack[i].IsDraw = FALSE;	//	�`�����߂�
			}
			if (MY_CHECK_ENEMY_COLL(player.attack[i].coll) != -1)	//�G�ɓ���������
			{
				player.attack[i].IsDraw = FALSE;	//	�`�����߂�
				enemy[MY_CHECK_ENEMY_COLL(player.attack[i].coll)].IsDraw = FALSE;	//�G�̕`�����߂�
				//�G��|�����Ƃ��̌��ʉ�
				if (CheckSoundMem(DefeatSE.handle) == 0)
				{
					ChangeVolumeSoundMem(255 * 60 / 100, DefeatSE.handle);//60%�̃{�����[��
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
	
	if (MY_CHECK_MAP1_COLL(player.coll.CheckBottom,&x,&y) == -1)	//�v���C���[�������Ă���Ƃ�
	{
		if (player.status != PLAYER_STATUS_JUMP)	//�v���C���[�̏�Ԃ��W�����v�ȊO�̎�
		{
			player.CenterY += gravity;		//�d�͂�������
			//�󒆂ɂ��鎞�Ԃɂ���ďd�͂��傫���Ȃ��Ă���(�d�͉����x��)
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
	if(MY_CHECK_MAP1_COLL(player.coll.CheckBottom, &x, &y) == BLOCK)//���ɒn�ʂ��������璅�n
	{
		player.CenterY = stage1[x][y].y - player.height / 2 - 1;	//���̒n�ʂ̏�ɗ�������
		FallTime.cnt = 0;	//�������̃J�E���g�̏�����
		gravity = 10;	//�d�͂����Ƃɖ߂�
		if (player.NowJumpTimes == player.CanJumpTimes)
		{
			JumpBuff = 0;
		}
		player.NowJumpTimes = 0;
		player.CanJumpTimes = 1 + JumpBuff;
	}
	if (MY_CHECK_MAP1_COLL(player.coll.CheckRight, &x, &y) == BLOCK)//�E�ɏ�Q��
	{
		player.CanRightMove = FALSE;//�E�ɐi�߂Ȃ��悤�ɂ���
	}
	if (MY_CHECK_MAP1_COLL(player.coll.CheckLeft, &x, &y) == BLOCK)//���ɏ�Q��
	{
		player.CanLeftMove = FALSE;//���ɐi�߂Ȃ��悤�ɂ���
	}
	if (MY_CHECK_MAP1_ACT_COLL(player.coll.CheckItem) == GOAL)//�S�[���ɓ��������Ƃ�
	{
		EndKind = GAME_CLEAR;	//�G���h�̎�ނ̓N���A
		//�S�[�������ʉ�
		if (CheckSoundMem(GoalSE.handle) == 0)
		{
			ChangeVolumeSoundMem(255 * 50 / 100, GoalSE.handle);//50%�̃{�����[��
			PlaySoundMem(GoalSE.handle, DX_PLAYTYPE_BACK);
		}
		GameScene = GAME_SCENE_END;	//�V�[�����G���h��ʂɑJ��
	}
	if (MY_CHECK_ENEMY_COLL(player.coll.base) != -1 && player.life.invincible == FALSE)
	{
		player.life.invincible = TRUE;
		player.life.now--;
		if (CheckSoundMem(HitSE.handle) == 0)
		{
			ChangeVolumeSoundMem(255 * 30 / 100, HitSE.handle);//30%�̃{�����[��
			PlaySoundMem(HitSE.handle, DX_PLAYTYPE_BACK);
		}
	}
	if (player.life.now == 0)
	{
		EndKind = GAME_OVER;	//�G���h�̎�ނ̓Q�[���I�[�o�[
		GameScene = GAME_SCENE_END;	//�V�[�����G���h��ʂɑJ��
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
	for (int m = 0; m < ENEMY_DATA_MAX; m++)//�G�̃f�[�^�̐��܂�
	{
		if (EnemyData[m].StartX > screen.left && EnemyData[m].StartX < screen.right &&
			EnemyData[m].StartY > screen.top && EnemyData[m].StartY < screen.bottom &&
			EnemyData[m].IsDraw == FALSE)//�G�̃X�^�[�g�ʒu����ʓ��ɂ���A�܂���x�����̓G��`�悵�ĂȂ��Ƃ�
		{
			int n = ENEMY_CHECK();
			if(n != -1)
			{
				//�G�̃f�[�^��o�^����
				enemy[n].x = EnemyData[m].StartX;
				enemy[n].y = EnemyData[m].StartY;
				enemy[n].kind = EnemyData[m].kind;
				enemy[n].width = EnemyData[m].width;
				enemy[n].height = EnemyData[m].height;
				enemy[n].CenterX = enemy[n].x + enemy[n].width / 2;
				enemy[n].CenterY = enemy[n].y + enemy[n].height / 2;
				enemy[n].IsFly = EnemyData[m].IsFly;

				enemy[n].IsDraw = TRUE;		//�G��`��ł���悤�ɂ���
				EnemyData[m].IsDraw = TRUE;		//���Ԗڂ̃f�[�^�̓G�͕`��ς�
			}
		}
	}
	for (int n = 0; n < ENEMY_MAX; n++)
	{
		if (enemy[n].IsDraw == TRUE)//�G���`�悳��Ă���Ƃ�
		{
			ENEMY_MOVE(n);//�G�̓����̏���
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
	
	switch (enemy[n].kind)//�G�̎�ނɂ���ē��������߂�
	{
	case 0:	//�^�������i�ݕǂɓ�����Ɣ��΂̕����֌�����ς���
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

			//��ʊO�ɍs���Ə�����
			if (enemy[n].x + enemy[n].width < 0 || enemy[n].x > GAME_WIDTH)
			{
				enemy[n].IsDraw = FALSE;
			}
		}
	}
}