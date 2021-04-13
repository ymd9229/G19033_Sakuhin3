#pragma once

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

#define IMAGE_TITLE_ROGO_PATH   TEXT(".\\IMAGE\\LogoImage\\TitleLogo.png")
#define IMAGE_CLEAR_ROGO_PATH   TEXT(".\\IMAGE\\LogoImage\\ClearLogo.png")
#define IMAGE_OVER_ROGO_PATH    TEXT(".\\IMAGE\\LogoImage\\OverLogo.png")
#define IMAGE_STAGECLEAR_PATH	TEXT(".\\IMAGE\\LogoImage\\StageClear.png")
#define IMAGE_TITLE_BACK_PATH   TEXT(".\\IMAGE\\BackImage\\TitleBack.png")
#define IMAGE_CLEAR_BK_PATH		TEXT(".\\IMAGE\\BackImage\\gameclearBK.png")
#define IMAGE_OVER_BK_PATH		TEXT(".\\IMAGE\\BackImage\\gameoverBK.png")

#define IMAGE_BOOK_PATH			TEXT(".\\IMAGE\\book.png")

#define MUSIC_TITLE_BGM_PATH    TEXT(".\\MUSIC\\bgm_maoudamashii_healing01.mp3")
#define MUSIC_STAGE1_BGM_PATH	TEXT(".\\MUSIC\\bgm_maoudamashii_piano25.mp3")
#define MUSIC_CLEAR_BGM_PATH	TEXT(".\\MUSIC\\game_maoudamashii_7_event37.mp3")
#define MUSIC_OVER_BGM_PATH	TEXT(".\\MUSIC\\game_maoudamashii_9_jingle07.mp3")

#define MUSIC_ATTACK_SE_PATH    TEXT(".\\MUSIC\\se_maoudamashii_retro16.mp3")
#define MUSIC_DEFEAT_SE_PATH    TEXT(".\\MUSIC\\se_maoudamashii_retro04.mp3")
#define MUSIC_GOAL_SE_PATH		TEXT(".\\MUSIC\\se_maoudamashii_onepoint09.mp3")
#define MUSIC_HIT_SE_PATH		TEXT(".\\MUSIC\\se_maoudamashii_battle07.mp3")

#define MOVIE_POSE_IN_PATH      TEXT(".\\MOVIE\\pose.mp4")

enum END_TYPE	//�G���h��ʂ̎��
{
	GAME_CLEAR,//�N���A
	STAGE_CLEAR,//�X�e�[�W�N���A
	GAME_OVER,//�Q�[���I�[�o�[
};

enum GAME_SCENE	//�V�[���̎�� 
{
	GAME_SCENE_START,	//�X�^�[�g���
	GAME_SCENE_PLAY,	//�v���C���
	GAME_SCENE_END,		//�G���h���
	GAME_SCENE_POSE		//�|�[�Y���
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
	BOOL IsViewed;
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

typedef struct STRUCT_COLL	//����̍\����
{
	RECT base;
	RECT CheckItem;
	RECT CheckLeft;
	RECT CheckRight;
	RECT CheckTop;
	RECT CheckBottom;
}COLL;

//########�O���[�o���ϐ�########

extern IMAGE TitleRogo;		//�^�C�g�����S
extern IMAGE ClearRogo;		//�Q�[���N���A���̃��S
extern IMAGE OverRogo;		//�Q�[���I�[�o�[���̃��S
extern IMAGE GoalRogo;		//�S�[�����̃��S
extern IMAGE TitleBack;		//�^�C�g���w�i
extern IMAGE ClearBack;		//�N���A���̔w�i
extern IMAGE OverBack;		//�Q�[���I�[�o�[���̔w�i

extern int EndKind;						//�N���A���Q�[���I�[�o�[��
extern int StartTimeFps;				//����J�n����
extern int CountFps;					//�J�E���^
extern float CalcFps;					//�v�Z����
extern int SampleNumFps;				//���ς����T���v����
extern int AllKeyState[256];			//���ׂẴL�[�̏�Ԃ�����
extern int OldAllKeyState[256];			//���ׂẴL�[�̏��(���O)������
extern CNT precdence[256];				//���ׂẴL�[���Ƃ̐�s���͂̃J�E���g
extern BOOL IsPrecedence[256];			//���ׂẴL�[�̐�s���͒����ǂ����̏�Ԃ�����
extern int GameScene;					//�Q�[���V�[�����Ǘ�
extern int NowStage;					//���X�e�[�W�ڂ�
extern MUSIC TitleBGM;			//�^�C�g���̉��y
extern MUSIC Stage1BGM;			//�X�e�[�W�P�̉��y
extern MUSIC GameClearBGM;		//�N���A�������̉��y
extern MUSIC GameOverBGM;		//�Q�[���I�[�o�[���̉��y
extern MUSIC GoalSE;			//�S�[�������Ƃ��̌��ʉ�
extern RECT screen;				//��ʂ͈̔�
extern MOVIE PoseIn;			//�|�[�Y��ʂɓ���Ƃ��̓���

VOID MY_FPS_UPDATE(VOID);			//FPS�l���v���A�X�V����֐�
VOID MY_FPS_DRAW(VOID);				//FPS�l��`�悷��֐�
VOID MY_FPS_WAIT(VOID);				//FPS�l���v�����A�҂֐�

VOID MY_ALL_KEYDOWN_UPDATE(VOID);	//�L�[�̓��͏�Ԃ��X�V����
BOOL MY_KEY_DOWN(int);				//�L�[�������Ă��邩�A�L�[�R�[�h�Ŕ��f����
BOOL MY_KEY_UP(int);				//�L�[�������グ�����A�L�[�R�[�h�Ŕ��f����
BOOL MY_KEY_DOWN_KEEP(int, int);	//�L�[�����������Ă��邩�A�L�[�R�[�h�Ŕ��f����
BOOL MY_KEY_DOWN_MOMENT(int);		//�L�[�������n�߂����A�L�[�R�[�h�Ŕ��f����
BOOL MY_KEY_DOWN_PRECEDENCE(int,int);	//�L�[�̐�s���͗p

VOID MY_GAME_INIT(VOID);	//������
VOID MY_PLAY_INIT(VOID);    //�X�e�[�W���n�߂鎞�̏�����

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

VOID STAGE_SCROLL(VOID);	//�X�N���[���̏���

VOID COLL_PROC(VOID);		//�����蔻��̏���
VOID COLL_INIT(VOID);		//�����蔻��̏�����

BOOL MY_CHECK_RECT_COLL(RECT, RECT);			//���蓯�m���d�Ȃ��Ă��邩
INT MY_CHECK_ENEMY_COLL(RECT);					//�G�@�΁@�v���C���[�̔���
INT MY_CHECK_MAP1_COLL(RECT, int*, int*);		//�}�b�v�̏�Q���@�΁@�G���v���C���[�̔���
INT MY_CHECK_MAP1_ACT_COLL(RECT, int*, int*);	//�}�b�v�̃A�C�e���@�΁@�v���C���[�̔���

