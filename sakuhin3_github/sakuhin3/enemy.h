#pragma once
#include "main.h"

#define ENEMY_S_DIV_WIDTH  80
#define ENEMY_S_DIV_HEIGHT  80
#define ENEMY_S_DIV_TATE   4
#define ENEMY_S_DIV_YOKO   1
#define ENEMY_S_DIV_NUM   ENEMY_S_DIV_TATE * ENEMY_S_DIV_YOKO
#define ENEMY_DATA_MAX 5	//�G�̃f�[�^�̐�
#define ENEMY_MAX 10		//�����ɏo�����邱�Ƃ��o����G�̐�
#define ENEMY_IMAGE_KIND 10	//�G�̉摜�̎�ނ̐�

#define IMAGE_ENEMY1_PATH       TEXT(".\\IMAGE\\enemy1.png")
#define IMAGE_ENEMY2_PATH       TEXT(".\\IMAGE\\enemy2.png")

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
	int stage;	//�o�ꂷ��X�e�[�W
	BOOL IsFly;
	BOOL IsDraw = FALSE;
	char path[PATH_MAX];
	int handle[ENEMY_S_DIV_NUM];
	int width;
	int height;
}ENEMY_DATA;

extern ENEMY_DATA EnemyData[ENEMY_DATA_MAX];	//�G�̃f�[�^
extern ENEMY_DATA EnemyImage[ENEMY_IMAGE_KIND];
extern ENEMY enemy[ENEMY_MAX];

extern MUSIC DefeatSE;			//�G��|�����Ƃ��̌��ʉ�

VOID ENEMY_PROC(VOID);		//�G�̏���
VOID ENEMY_DRAW(VOID);		//�G�̕`��
INT ENEMY_CHECK(VOID);		//
VOID ENEMY_MOVE(INT);		//�G�̓����̏���
VOID ENEMY_COLL(VOID);
VOID ENEMY_COLL_INIT(VOID);

BOOL ENEMY_LOAD_IMAGE(VOID);
VOID ENEMY_DELETE_IMAGE(VOID);
BOOL ENEMY_LOAD_MUSIC(VOID);
VOID ENEMY_DELETE_MUSIC(VOID);