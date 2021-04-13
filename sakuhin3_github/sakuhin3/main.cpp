#include "main.h"
#include "player.h"
#include "enemy.h"
#include "map.h"

ENEMY_DATA EnemyData[ENEMY_DATA_MAX] = {
	//�o�G�̏o���ʒuX�A�G�̏o���ʒuY�A�G�̎�ށA�o�ꂷ��X�e�[�W�A��s���邩�ǂ����p
	{700,480,0,1,FALSE},
	{3500,480,0,1,FALSE},
	{500,320,0,2,FALSE},
	{3000,240,1,2,TRUE},
	{2500,240,1,2,TRUE},
	{480,320,2,3,FALSE},
	{960,480,0,3,FALSE},
	{3520,480,2,3,FALSE},
	{3680,80,1,3,TRUE},
};	//�G�̃f�[�^
ENEMY_DATA EnemyImage[ENEMY_IMAGE_KIND];
ENEMY enemy[ENEMY_MAX];
IMAGE TitleBack;		//�^�C�g���w�i
IMAGE TitleRogo;		//�^�C�g�����S
IMAGE ClearRogo;		//�Q�[���N���A���̃��S
IMAGE OverRogo;			//�Q�[���I�[�o�[���̃��S
IMAGE GoalRogo;			//�S�[�����̃��S
IMAGE ClearBack;		//�N���A���̔w�i
IMAGE OverBack;			//�Q�[���I�[�o�[���̔w�i
IMAGE PlayerLife;
IMAGE StageBack[STAGE_MAX];
MAPCHIP mapchip;
MAP stage[GAME_MAP_TATE_MAX][GAME_MAP_YOKO_MAX];
PLAYER player;
MAGIC_BOOK book;
MAGIC magic[PLAYER_MAGIC_MAX];
MAGIC_DATA MagicData[MAGIC_DATA_MAX] = {
	{0,0,FALSE,FALSE,},
	{0,0,TRUE,FALSE,},
	{0,0,FALSE,FALSE,},
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
int NowStage;					//���X�e�[�W�ڂ�
int gravity;					//�d��
CNT precdence[256];				//��s���͂̃J�E���g�p
int WalkCheckR;					//�E�ɕ����Ă��邩���ׂ�悤
int WalkCheckL;					//���ɕ����Ă��邩���ׂ�悤
int JumpBuff = 0;				//�ǉ��̃W�����v��
int AvailableMagic = 0;	//�g�p�ł��閂�@�̐�
MUSIC TitleBGM;			//�^�C�g���̉��y
MUSIC Stage1BGM;		//�X�e�[�W�P�̉��y
MUSIC GameClearBGM;		//�N���A�������̉��y
MUSIC GameOverBGM;		//�Q�[���I�[�o�[���̉��y
MUSIC AttackSE;			//�ʏ�U�����̌��ʉ�
MUSIC DefeatSE;			//�G��|�����Ƃ��̌��ʉ�
MUSIC GoalSE;			//�S�[�������Ƃ��̌��ʉ�
MUSIC HitSE;			//�_���[�W���󂯂��Ƃ��̌��ʉ�
RECT screen;
MOVIE PoseIn;			//�|�[�Y��ʂɓ���Ƃ��̓���

//�}�b�v�̃f�[�^
GAME_MAP_KIND stage1Data[GAME_MAP_TATE_MAX][GAME_MAP_YOKO_MAX]{
	ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,af,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,
	ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,af,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,
	ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,af,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,
	ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,af,ae,ae,ae,ae,ae,af,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,
	ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,af,af,af,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,af,ae,ae,ae,ae,af,af,ae,af,ae,af,ae,ae,ae,ae,ae,ae,af,ae,ae,ae,ae,ae,ae,ae,ae,
	ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,af,af,af,ae,ae,ae,ae,ae,af,ae,ae,ae,ae,ae,ae,ae,ae,ae,ga,ae,ae,ag,
	aa,ae,ae,ae,ac,ae,ac,ae,ae,ae,ae,ae,ae,ae,ae,ad,ae,ae,ae,ha,ae,ae,ae,ae,ga,ae,ae,af,af,af,af,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ad,ae,ae,ad,
	ba,ab,ac,ab,bb,ae,bb,ab,ac,ab,ac,ab,ac,ab,ac,bc,ac,ab,ac,ab,ac,ab,ac,ab,ac,ab,ac,ac,ac,ab,ac,ae,ae,ae,ae,ae,ae,ab,ac,ab,ac,ab,ac,ab,ac,ab,bc,ab,ac,bc,
	ba,bb,bc,bb,bc,ae,bc,bb,bc,bb,bc,bb,bc,bb,bc,bb,bc,bb,bc,bb,bc,bb,bc,bb,bc,bb,bc,bb,bc,bb,bc,ae,ae,ae,ae,ae,ae,bb,bc,bb,bc,bb,bc,bb,bc,bb,bc,bb,bc,bd,
};

GAME_MAP_KIND stage2Data[GAME_MAP_TATE_MAX][GAME_MAP_YOKO_MAX]{
	ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,
	ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ag,ae,
	ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,af,ae,
	ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,af,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,
	ae,ae,ae,ae,ae,ae,af,af,af,af,af,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,af,ae,ae,ae,ae,ae,ae,ae,af,ae,ae,ae,
	ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,af,ae,ae,ae,af,ae,ae,ae,ae,ae,ae,ae,ae,ae,ga,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,
	aa,ae,ae,ae,ae,ae,ga,ae,ae,ha,af,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ac,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ac,ae,ae,ae,ae,ga,ae,ae,ae,ae,ae,ae,ac,ae,
	ba,ab,ac,ab,ac,ab,ac,ab,ac,ab,ac,ab,ac,ab,ac,ab,ae,ae,ae,ab,ac,ab,ae,ae,bb,ab,ac,ab,ac,ab,ac,ae,ab,ac,ab,ac,bc,ac,ab,ac,ab,ac,ab,ae,ae,ac,ab,ac,bc,ac,
	ba,bb,bc,bb,bc,bb,bc,bb,bc,bb,bc,bb,bc,bb,bc,bb,ae,ae,ae,bb,bc,bb,ae,ae,bc,bb,bc,bb,bc,bb,bc,ae,bb,bc,bb,bc,bb,bc,bb,bc,bb,bc,bb,ae,ae,bc,bb,bc,bb,bc,
};

GAME_MAP_KIND stage3Data[GAME_MAP_TATE_MAX][GAME_MAP_YOKO_MAX]{
	ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,af,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,af,af,af,af,af,af,af,af,ae,ae,ae,ae,ae,ae,ae,ae,af,
	ae,ae,ae,ae,ae,ae,ae,ga,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,af,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,af,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,af,
	ae,ae,ae,ae,ae,ae,ae,af,ae,ae,ha,ae,ae,ae,ae,ae,ae,ae,af,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,af,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,af,ae,ae,af,
	ae,ae,ae,af,ae,ae,ae,af,af,af,af,ae,ae,ae,ae,ae,ae,ae,af,ae,ae,af,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,af,ae,bf,ae,ae,ae,ae,af,ae,ae,ae,ae,ae,bf,ae,ae,af,
	ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,af,ae,ae,af,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,af,af,af,af,af,ae,ae,af,ae,ae,ae,ae,ae,bf,ae,ae,af,
	ae,ae,ae,ae,ae,ac,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,af,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,af,ae,af,ae,ae,ae,bf,ae,af,af,
	ae,ae,ae,ae,ab,bc,ae,ae,ae,ae,ae,ae,af,ae,ae,ae,ae,ae,bf,ae,ae,af,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ga,ae,ae,af,ae,ae,ae,ae,ae,bf,ae,ae,af,
	ab,ac,ab,ac,bc,bb,ab,ac,ab,ac,ab,ac,ab,ae,ae,ac,ab,ac,ab,ac,ab,ac,ab,ac,ae,bf,ae,ae,ae,ae,ae,ae,ab,ac,ab,ac,ab,ac,ab,ac,ab,ac,ab,ac,ab,ac,ab,ae,ag,ac,
	bb,bc,bb,bc,bb,bc,bb,bc,bb,bc,bb,bc,bb,ae,ae,bc,bb,bc,bb,bc,bb,bc,bb,bc,ab,ae,ae,ae,ae,ae,ae,ae,bb,bc,bb,bc,bb,bc,bb,bc,bb,bc,bb,bc,bb,bc,bb,ac,ab,bc,
};


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

		MY_ALL_KEYDOWN_UPDATE();	//�����Ă���L�[��Ԃ��擾
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
	int resultTime = GetNowCount() - StartTimeFps;	//������������
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

BOOL MY_KEY_DOWN_PRECEDENCE(int key,int cnt)
{
	//��s���͉\�ɂȂ�����J�E���g���n�߂�
	if (IsPrecedence[key] == TRUE)
	{
		precdence[key].CntMax = cnt;
		precdence[key].cnt = CNT_CHECK(precdence[key].cnt, precdence[key].CntMax);
		if (precdence[key].cnt != 0)
		{
			return TRUE;
		}
		else
		{
			IsPrecedence[key] = FALSE;//��s���͂̎��Ԃ��I����
			return FALSE;
		}
	}
	return FALSE;
}

VOID MY_GAME_INIT(VOID)
{
	//������	
	NowStage = 1;
	
	player.AttackInterval.CntMax = 40;
	player.jump.CntMax = 20;
	player.life.max = 3;
	player.life.InvincibleCnt.CntMax = 180;
	player.change.CntMax = 20;
	player.MagicInterval.CntMax = 60;
	AvailableMagic = 0;

	book.IsDraw = TRUE;

	//��ʉE��ɖ��@�̃A�C�R��
	MagicIcon.x = GAME_WIDTH - MagicIcon.width;
	MagicIcon.y = 0;

	MY_PLAY_INIT();//�v���C��ʂɓ���Ƃ��݂̂̏�����
}

VOID MY_PLAY_INIT(VOID)
{
	player.IsDraw = TRUE;

	player.x = 1;
	player.y = 0;
	player.CenterX = player.x + player.width / 2;
	player.CenterY = player.y + player.height / 2;
	player.life.now = player.life.max;
	player.EquipMagic = -1;
	player.life.InvincibleCnt.cnt = 0;
	player.CanJumpTimes = 1;
	player.NowJumpTimes = 0;

	gravity = 10;

	screen.left = 0;
	screen.right = GAME_WIDTH;
	screen.top = 0;
	screen.bottom = GAME_HEIGHT;

	for (int n = 0; n < ENEMY_MAX; n++)
	{
		enemy[n].IsDraw = FALSE;
		enemy[n].CanLeftMove = TRUE;
		enemy[n].CanRightMove = FALSE;
	}
	
	for (int m = 0; m < ENEMY_DATA_MAX; m++)
	{
		EnemyData[m].IsDraw = FALSE;
	}
	
	if (EndKind == STAGE_CLEAR)
	{
		book.IsDraw = TRUE;
	}
	STAGE_INIT();
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
		PlaySoundMem(TitleBGM.handle, DX_PLAYTYPE_LOOP);
	}
	//�G���^�[�L�[����������A�v���C�V�[���ֈړ�����
	if (MY_KEY_DOWN_MOMENT(KEY_INPUT_RETURN) == TRUE)
	{
		if (CheckSoundMem(TitleBGM.handle) != 0)
		{
			StopSoundMem(TitleBGM.handle);	//BGM���~�߂�
		}
		
		MY_GAME_INIT();

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
	ENEMY_PROC();		
	STAGE_SCROLL();		
	COLL_PROC();		

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

	return;
}

//�v���C��ʂ̕`��
VOID MY_PLAY_DRAW(VOID)
{
	DrawGraph(0, 0, StageBack[NowStage - 1].handle, true);
	PLAYER_DRAW();
	STAGE_DRAW();
	MAGIC_ICON_DRAW();
	PLAYER_LIFE_DRAW();
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
	case STAGE_CLEAR:
		if (CheckSoundMem(GameClearBGM.handle) == 0)
		{
			ChangeVolumeSoundMem(255 * 50 / 100, GameClearBGM.handle);
			PlaySoundMem(GameClearBGM.handle, DX_PLAYTYPE_LOOP);
		}
		break;
	case GAME_OVER:
		if (CheckSoundMem(GameOverBGM.handle) == 0)
		{
			ChangeVolumeSoundMem(255 * 50 / 100, GameOverBGM.handle);
			PlaySoundMem(GameOverBGM.handle, DX_PLAYTYPE_LOOP);
		}
		break;
	}
	if (MY_KEY_DOWN_MOMENT(KEY_INPUT_RETURN) == TRUE)
	{
		if (CheckSoundMem(GameClearBGM.handle) != 0)
		{
			StopSoundMem(GameClearBGM.handle);	//BGM���~�߂�
		}
		if (CheckSoundMem(GameOverBGM.handle) != 0)
		{
			StopSoundMem(GameOverBGM.handle);	//BGM���~�߂�
		}
		if (EndKind == GAME_OVER)
		{
			MY_PLAY_INIT();
			GameScene = GAME_SCENE_PLAY;
		}
		else if (EndKind == STAGE_CLEAR)
		{
			NowStage++;
			MY_PLAY_INIT();
			GameScene = GAME_SCENE_PLAY;
		}
		else
		{
			GameScene = GAME_SCENE_START;
		}
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
	case STAGE_CLEAR:
		DrawGraph(0, 0, ClearBack.handle, true);
		DrawGraph(GAME_WIDTH / 2 - GoalRogo.width / 2, GoalRogo.height, GoalRogo.handle, true);
		break;
	case GAME_OVER:
		DrawGraph(0, 0, OverBack.handle, true);
		DrawGraph(GAME_WIDTH / 2 - OverRogo.width / 2, OverRogo.height, OverRogo.handle, true);
		break;
	}

	DrawString(0, 0, "�G���h���(�G���^�[�L�[�������ĉ�����)", GetColor(255, 255, 255));
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

VOID MY_POSE_DRAW(VOID)		
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

	strcpy_s(GoalRogo.path, IMAGE_STAGECLEAR_PATH);
	GoalRogo.handle = LoadGraph(GoalRogo.path);
	GetGraphSize(GoalRogo.handle, &GoalRogo.width, &GoalRogo.height);

	strcpy_s(ClearBack.path, IMAGE_CLEAR_BK_PATH);
	ClearBack.handle = LoadGraph(ClearBack.path);

	strcpy_s(OverBack.path, IMAGE_OVER_BK_PATH);
	OverBack.handle = LoadGraph(OverBack.path);
	
	strcpy_s(PoseIn.path, MOVIE_POSE_IN_PATH);
	PoseIn.handle = LoadGraph(PoseIn.path);
	PLAYER_LOAD_IMAGE();
	ENEMY_LOAD_IMAGE();
	MAP_LOAD_IMAGE();
	return TRUE;
}

VOID MY_DELETE_IMAGE(VOID)
{
	PLAYER_DELETE_IMAGE();
	ENEMY_DELETE_IMAGE();
	MAP_DELETE_IMAGE();
	DeleteGraph(TitleRogo.handle);
	DeleteGraph(TitleBack.handle);
	DeleteGraph(ClearRogo.handle);
	DeleteGraph(OverRogo.handle);
	DeleteGraph(GoalRogo.handle);
	DeleteGraph(PoseIn.handle);
	return;
}

BOOL MY_LOAD_MUSIC(VOID)
{
	strcpy_s(TitleBGM.path, MUSIC_TITLE_BGM_PATH);		
	TitleBGM.handle = LoadSoundMem(TitleBGM.path);
	strcpy_s(GameClearBGM.path, MUSIC_CLEAR_BGM_PATH);
	GameClearBGM.handle = LoadSoundMem(GameClearBGM.path);
	strcpy_s(GameOverBGM.path, MUSIC_OVER_BGM_PATH);
	GameOverBGM.handle = LoadSoundMem(GameOverBGM.path);
	strcpy_s(GoalSE.path, MUSIC_GOAL_SE_PATH);
	GoalSE.handle = LoadSoundMem(GoalSE.path);
	
	PLAYER_LOAD_MUSIC();
	ENEMY_LOAD_MUSIC();
	MAP_LOAD_MUSIC();
	return TRUE;
}

VOID MY_DELETE_MUSIC(VOID)
{
	PLAYER_DELETE_MUSIC();
	ENEMY_DELETE_MUSIC();
	MAP_DELETE_MUSIC();
	DeleteSoundMem(TitleBGM.handle);
	DeleteSoundMem(GameClearBGM.handle);
	DeleteSoundMem(GameOverBGM.handle);
	DeleteSoundMem(GoalSE.handle);
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
	STAGE_COLL_INIT();
	PLAYER_COLL_INIT();
	PLAYER_ATTACK_COLL_INIT();
	MAGIC_COLL_INIT();
	ENEMY_COLL_INIT();
	ENEMY_COLL();
	PLAYER_COLL();
	PLAYER_ATTACK_COLL();
	MAGIC_COLL();

	if (MY_CHECK_MAP1_ACT_COLL(player.coll.CheckItem, &x, &y) == BOOK)
	{
		if (book.IsDraw == TRUE)
		{
			AvailableMagic++;
			book.IsDraw = FALSE;
		}
	}
}

INT MY_CHECK_MAP1_COLL(RECT a,int *x, int *y)
{	
	for (int tate = 0; tate < GAME_MAP_TATE_MAX; tate++)
	{
		for (int yoko = 0; yoko < GAME_MAP_YOKO_MAX; yoko++)
		{
			if (MY_CHECK_RECT_COLL(stage[tate][yoko].coll, a) == TRUE)
			{
				if (stage[tate][yoko].kind != ae &&
					stage[tate][yoko].kind != ag &&
					stage[tate][yoko].kind != ha)
				{
					//���݂̃}�X�̈ʒu
					*y = yoko;
					*x = tate;

					return BLOCK;
				}
			}
		}
	}
	return -1;
}

INT MY_CHECK_MAP1_ACT_COLL(RECT a, int* x, int* y)
{
	for (int tate = 0; tate < GAME_MAP_TATE_MAX; tate++)
	{
		for (int yoko = 0; yoko < GAME_MAP_YOKO_MAX; yoko++)
		{
			if (MY_CHECK_RECT_COLL(stage[tate][yoko].coll, a) == TRUE)
			{
				//���݂̃}�X�̈ʒu
				*x = tate;
				*y = yoko;

				//�����������̂ɂ���Ė߂�l��ς���
				if (stage[tate][yoko].kind == ag)
				{
					return GOAL;
				}
				if (stage[tate][yoko].kind == ha)
				{
					return BOOK;
				}
				if (stage[tate][yoko].kind == ga)
				{
					return WOOD;
				}
				if (stage[tate][yoko].kind == bf)
				{
					return MOVE_BLOCK;
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
	if (player.x + player.width > GAME_WIDTH / 2 && screen.right < mapchip.width * GAME_MAP_YOKO_MAX)	//�v���C���[����ʂ̔����܂œ��B������
	{
		player.IsScroll = TRUE;
		if (player.IsMove == TRUE && player.CanRightMove == TRUE && player.muki == MUKI_R)
		{
			for (int tate = 0; tate < GAME_MAP_TATE_MAX; tate++)
			{
				for (int yoko = 0; yoko < GAME_MAP_YOKO_MAX; yoko++)
				{
					stage[tate][yoko].x -= PLAYER_SPEED_NORMAL;		//�X�e�[�W�̃X�N���[��	
				}
			}
			book.image.x -= PLAYER_SPEED_NORMAL;
			for (int n = 0; n < ENEMY_MAX; n++)
			{
				enemy[n].CenterX -= PLAYER_SPEED_NORMAL;	//�G�̃X�N���[��
			}
			screen.right += PLAYER_SPEED_NORMAL;
			screen.left += PLAYER_SPEED_NORMAL;
		}
	}
	else if (player.x <= 0 && player.CanLeftMove == TRUE && player.muki == MUKI_L && screen.left > 0)
	{
		player.IsScroll = TRUE;
		if (player.IsMove == TRUE && player.CanLeftMove == TRUE)
		{
			for (int tate = 0; tate < GAME_MAP_TATE_MAX; tate++)
			{
				for (int yoko = 0; yoko < GAME_MAP_YOKO_MAX; yoko++)
				{
					stage[tate][yoko].x += PLAYER_SPEED_NORMAL;
				}
			}
			book.image.x += PLAYER_SPEED_NORMAL;
			for (int n = 0; n < ENEMY_MAX; n++)
			{
				enemy[n].CenterX += PLAYER_SPEED_NORMAL;
			}
			screen.right -= PLAYER_SPEED_NORMAL;
			screen.left -= PLAYER_SPEED_NORMAL;
		}
	}
	else
	{
		player.IsScroll = FALSE;
	}
}

