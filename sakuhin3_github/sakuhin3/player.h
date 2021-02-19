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

enum SPEED_KIND		//�l�X�ȑ��x
{
	ENEMY_SPEED_SLOW = 1,		//�x���G�̑��x
	PLAYER_SPEED_NORMAL = 5,	//�v���C���[�̒ʏ펞�̑��x
	PLAYER_TAMA_SPEED = 30,		//�v���C���[�̒ʏ�U���̒e�̑��x
};

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

extern MUSIC AttackSE;			//�ʏ�U�����̌��ʉ�
extern MUSIC HitSE;			//�_���[�W���󂯂��Ƃ��̌��ʉ�
extern IMAGE PlayerLife;
extern PLAYER player;
extern MAGIC magic[PLAYER_MAGIC_MAX];
extern MAGIC_DATA MagicData[MAGIC_DATA_MAX];
extern MAGIC_ICON MagicIcon;
extern int JumpBuff;
extern int AvailableMagic;	//�g�p�ł��閂�@�̐�
extern int gravity;					//�d��
extern CNT FallTime;					//�������Ă��鎞�Ԃ��J�E���g����p
extern int WalkCheckR;					//�E�ɕ����Ă��邩���ׂ�悤
extern int WalkCheckL;					//���ɕ����Ă��邩���ׂ�悤

VOID PLAYER_MOVE(VOID);		//�v���C���[�̓����̏���
VOID PLAYER_DRAW(VOID);		//�v���C���[�̕`��
VOID PLAYER_JUMP(VOID);		//�v���C���[�̃W�����v�̏���
VOID PLAYER_ATTACK_PROC(VOID);	//�v���C���[�̒ʏ�U���̏���
VOID PLAYER_ATTACK_DRAW(VOID);	//�v���C���[�̒ʏ�U���̕`��
VOID PLAYER_LIFE_DRAW(VOID);	//�v���C���[�̃��C�t�̕`��

VOID MAGIC_PROC(VOID);			//���@�̏���
VOID MAGIC_DRAW(VOID);			//���@�̕`��
VOID MAGIC_SELECT(VOID);		//���@�̑I���̏���
INT MAGIC_CHECK(VOID);			//���g�p�̗v�f��T��
VOID MAGIC_EFFECT(int);			//���@���Ƃ̌��ʂ̏���
VOID MAGIC_ICON_DRAW(VOID);		//���@�̃A�C�R���̕`��

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