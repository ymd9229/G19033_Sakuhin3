#pragma once
#include "main.h"

#define MAP_DIV_WIDTH     80
#define MAP_DIV_HEIGHT    80
#define MAP_DIV_TATE      8
#define MAP_DIV_YOKO      8
#define MAP_DIV_NUM  MAP_DIV_TATE * MAP_DIV_YOKO

#define GAME_MAP_TATE_MAX   9	//�}�b�v�̏c�̃}�X�̐�
#define GAME_MAP_YOKO_MAX   50	//�}�b�v�̉��̃}�X�̐�

#define STAGE_MAX 2	//�X�e�[�W��

#define IMAGE_STAGE1_BACK_PATH  TEXT(".\\IMAGE\\Stage1Back.png")
#define IMAGE_STAGE2_BACK_PATH  TEXT(".\\IMAGE\\Stage2Back.png")
#define IMAGE_MAP1_PATH         TEXT(".\\IMAGE\\map1.png")

enum GAME_MAP_KIND
{
	aa = 0, ab = 1, ac = 2, ad = 3, ae = 4, af = 5, ag = 6, ah = 7,
	ba = 8, bb = 9, bc = 10, bd = 11, be = 12,
	ga = 48,
	ha = 56,
};

enum CHECK_MAP_COLL		//�}�b�v�̔��育�Ƃ̎��
{
	BLOCK,//�u���b�N�i��Q���j
	AIR,	//��C�i���薳���j
	GOAL,	//�S�[���n�_
	BOOK,	//���@�̖{
	WOOD,
};

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

typedef struct STRUCT_MAGIC_BOOK
{
	IMAGE image;
	BOOL IsDraw;
}MAGIC_BOOK;
//�}�b�v�̃f�[�^
extern GAME_MAP_KIND stage1Data[GAME_MAP_TATE_MAX][GAME_MAP_YOKO_MAX];
extern GAME_MAP_KIND stage2Data[GAME_MAP_TATE_MAX][GAME_MAP_YOKO_MAX];

extern IMAGE StageBack[STAGE_MAX];
extern MAPCHIP mapchip;
extern MAP stage[GAME_MAP_TATE_MAX][GAME_MAP_YOKO_MAX];
extern MAGIC_BOOK book;

VOID STAGE_INIT(VOID);		//�X�e�[�W�̏�����
VOID STAGE_DRAW(VOID);		//�X�e�[�W�̕`��
VOID STAGE_COLL_INIT(VOID);
BOOL MAP_LOAD_IMAGE(VOID);
VOID MAP_DELETE_IMAGE(VOID);
BOOL MAP_LOAD_MUSIC(VOID);
VOID MAP_DELETE_MUSIC(VOID);