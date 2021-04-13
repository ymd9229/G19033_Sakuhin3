#include "player.h"
#include "enemy.h"
#include "map.h"

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

		if (MY_KEY_DOWN_PRECEDENCE(KEY_INPUT_UP,30) == TRUE)
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

	if (player.life.now == 0)
	{
		EndKind = GAME_OVER;	//�G���h�̎�ނ̓Q�[���I�[�o�[
		GameScene = GAME_SCENE_END;	//�V�[�����G���h��ʂɑJ��
	}

	if (player.life.invincible == TRUE)	//���G��Ԃ̎�
	{
		player.life.InvincibleCnt.cnt = CNT_CHECK(player.life.InvincibleCnt.cnt, player.life.InvincibleCnt.CntMax);
		if (player.life.InvincibleCnt.cnt == 0)
		{
			player.life.invincible = FALSE;
		}
		//�_�ł�����
		if (player.life.InvincibleCnt.cnt % 2 == 1)
		{
			player.IsDraw = FALSE;
		}
		else
		{
			player.IsDraw = TRUE;
		}
	}

	player.x = player.CenterX - player.width / 2;
	player.y = player.CenterY - player.height / 2;

	player.CanMove = TRUE;
}



VOID PLAYER_JUMP(VOID)
{
	player.jump.cnt = CNT_CHECK(player.jump.cnt, player.jump.CntMax);
	if (player.jump.cnt == 0)
	{
		player.status = PLAYER_STATUS_STOP;
		player.jump.cnt = 0;
		player.JumpRise = 10;	//�㏸�̒l��߂�
	}
	else
	{
		player.CenterY -= player.JumpRise;	//�v���C���[�̈ʒu���㏸�̒l��������
		if (player.jump.cnt > 8 && player.JumpRise >= 0)
		{
			player.JumpRise--;	//�㏸�̒l��������
		}
		player.status = PLAYER_STATUS_JUMP;
	}
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
		if (player.AttackInterval.cnt == 0)
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

				magic[i].x = magic[i].CenterX - magic[i].width / 2;
				magic[i].y = magic[i].CenterY - magic[i].height / 2;

				magic[i].IsDraw = TRUE;
				if (player.IsBuff == TRUE && magic[i].IsBuff == TRUE)
				{
					magic[i].IsDraw = FALSE;
				}

				player.CanMagic = FALSE;
			}
		}
	}
	if (player.CanMagic == FALSE)
	{
		player.MagicInterval.cnt = CNT_CHECK(player.MagicInterval.cnt, player.MagicInterval.CntMax);
		if (player.MagicInterval.cnt == 0)
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
		{
			player.EquipMagic--;	//��O�̖��@��I��
		}
		else						//�I�𖂖@��-1(�I���Ȃ�)�̂Ƃ�
		{
			player.EquipMagic = AvailableMagic - 1;	//�Ō���̖��@��I��
		}
	}
	else if (MY_KEY_DOWN_MOMENT(KEY_INPUT_X) == TRUE)
	{
		if (AvailableMagic - 1 > player.EquipMagic)	//�I�𒆂̖��@�̔ԍ����擾�ςݖ��@���̖����̂Ƃ�
		{
			player.EquipMagic++;
		}
		else							//�I�𖂖@���Ō���̂Ƃ�
		{
			player.EquipMagic = -1;		//-1(�I���Ȃ�)�ɖ߂�	
		}
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
		if (magic[i].muki == MUKI_R)
			magic[i].CenterX += 10;
		if (magic[i].muki == MUKI_L)
			magic[i].CenterX -= 10;
		break;
	case 1:
		JumpBuff = 1;
		player.IsBuff = TRUE;
		if (player.EquipMagic != 1)
		{
			magic[i].IsDraw = FALSE;
			player.IsBuff = FALSE;
		}
		break;
	case 2:
		if (magic[i].muki == MUKI_R)
			magic[i].CenterX += 10;
		if (magic[i].muki == MUKI_L)
			magic[i].CenterX -= 10;
		break;
	}
}

VOID MAGIC_DRAW(VOID)
{
	for (int i = 0; i < PLAYER_MAGIC_MAX; i++)
	{
		if (magic[i].IsDraw == TRUE)
		{
			if (magic[i].IsBuff == FALSE)
			{
				magic[i].x = magic[i].CenterX - magic[i].width / 2;
				magic[i].y = magic[i].CenterY - magic[i].height / 2;
			}
			else
			{
				magic[i].x = player.x;
				magic[i].y = player.y;
			}

			magic[i].change.CntMax = 20;
			magic[i].change.cnt = CNT_CHECK(magic[i].change.cnt, magic[i].change.CntMax);

			if (magic[i].change.cnt == 0)
			{
				if (magic[i].change.NowImage >= magic[i].No * MAGIC_DIV_KIND && 
					magic[i].change.NowImage < magic[i].No * MAGIC_DIV_KIND + MAGIC_DIV_KIND - 1)
				{
					magic[i].change.NowImage++;
				}
				else
				{
					magic[i].change.NowImage = magic[i].No * MAGIC_DIV_KIND;
				}
			}

			DrawGraph(magic[i].x, magic[i].y, MagicData[0].handle[magic[i].change.NowImage], true);
			
			//��ʊO�ɏo����`�����߂�
			if (magic[i].CenterX < 0 || magic[i].CenterX > GAME_WIDTH)
			{
				magic[i].IsDraw = FALSE;
			}
		}
		else
		{
			magic[i].change.cnt = magic[i].change.CntMax;
		}
	}
};

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
		if (player.change.cnt == 0)	//���J�E���g���Ƃɉ摜��ς���
		{
			if (player.status == PLAYER_STATUS_MOVE_R)	//�E�ɕ����Ă���Ƃ�
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
			if (player.status == PLAYER_STATUS_MOVE_L)	//���ɕ����Ă���Ƃ�
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
			//�������Ƃɏ���
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



VOID PLAYER_LIFE_DRAW(VOID)
{
	for (int i = 0; i < player.life.now; i++)
	{
		DrawGraph(PlayerLife.width * i - 1, 0, PlayerLife.handle, TRUE);
	}
}



VOID MAGIC_ICON_DRAW(VOID)
{
	DrawGraph(MagicIcon.x, MagicIcon.y, MagicIcon.handle[player.EquipMagic + 1], TRUE);
}

VOID PLAYER_COLL(VOID)
{
	int x, y;

	player.CanRightMove = TRUE;
	player.CanLeftMove = TRUE;

	if (MY_CHECK_MAP1_COLL(player.coll.CheckBottom, &x, &y) == -1)	//�v���C���[�������Ă���Ƃ�
	{
		if (player.status != PLAYER_STATUS_JUMP)	//�v���C���[�̏�Ԃ��W�����v�ȊO�̎�
		{
			player.CenterY += gravity;		//�d�͂�������
			if (player.NowJumpTimes == 0)
			{
				player.CanJumpTimes = 0;
			}
		}
	}
	if (MY_CHECK_MAP1_COLL(player.coll.CheckTop, &x, &y) == BLOCK)//��ɏ�Q�����������Ƃ�
	{
		player.CenterY = stage[x][y].y + stage[x][y].height + player.height / 2 + 10;
		player.jump.cnt = player.jump.CntMax;
	}
	if (MY_CHECK_MAP1_COLL(player.coll.CheckBottom, &x, &y) == BLOCK)//���ɒn�ʂ��������璅�n
	{
		player.CenterY = stage[x][y].y - player.height / 2 - 1;	//���̒n�ʂ̏�ɗ�������
		
		if(player.EquipMagic != 1)
		{
			JumpBuff = 0;
		}
		player.NowJumpTimes = 0;			//���݂̃W�����v�����O�ɖ߂�
		player.CanJumpTimes = 1 + JumpBuff;	//�\�ȃW�����v�����߂�
	}
	if (MY_CHECK_MAP1_COLL(player.coll.CheckRight, &x, &y) == BLOCK)//�E�ɏ�Q��
	{
		player.CanRightMove = FALSE;//�E�ɐi�߂Ȃ��悤�ɂ���
	}
	if (MY_CHECK_MAP1_COLL(player.coll.CheckLeft, &x, &y) == BLOCK)//���ɏ�Q��
	{
		player.CanLeftMove = FALSE;//���ɐi�߂Ȃ��悤�ɂ���
	}
	if (MY_CHECK_MAP1_ACT_COLL(player.coll.CheckItem, &x, &y) == GOAL)//�S�[���ɓ��������Ƃ�
	{
		if (NowStage == STAGE_MAX)
		{
			EndKind = GAME_CLEAR;	//�G���h�̎�ނ̓Q�[���N���A
		}
		else
		{
			EndKind = STAGE_CLEAR;  //�G���h�̎�ނ̓X�e�[�W�N���A
		}
		//�S�[�������ʉ�
		if (CheckSoundMem(GoalSE.handle) == 0)
		{
			ChangeVolumeSoundMem(255 * 50 / 100, GoalSE.handle);//50%�̃{�����[��
			PlaySoundMem(GoalSE.handle, DX_PLAYTYPE_BACK);
		}
		GameScene = GAME_SCENE_END;	//�V�[�����G���h��ʂɑJ��
	}
	

	if (MY_CHECK_ENEMY_COLL(player.coll.base) != -1 && player.life.invincible == FALSE)//�G�ɓ��������Ƃ�
	{
		player.life.invincible = TRUE;	//���G��Ԃɂ���
		player.life.now--;		//���C�t������炷
		if (CheckSoundMem(HitSE.handle) == 0)
		{
			ChangeVolumeSoundMem(255 * 30 / 100, HitSE.handle);//30%�̃{�����[��
			PlaySoundMem(HitSE.handle, DX_PLAYTYPE_BACK);
		}
	}

	player.x = player.CenterX - player.width / 2;
	player.y = player.CenterY - player.height / 2;
}

VOID PLAYER_ATTACK_COLL(VOID)
{
	int x, y;
	
	for (int i = 0; i < PLAYER_TAMA_MAX; i++)
	{
		if (player.attack[i].IsDraw == TRUE)
		{
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

}

VOID MAGIC_COLL_INIT(VOID)
{
	for (int i = 0; i < PLAYER_MAGIC_MAX; i++)
	{
		if (magic[i].IsDraw == TRUE && magic[i].IsBuff == FALSE)
		{
			magic[i].coll.right = magic[i].x + magic[i].width;
			magic[i].coll.left = magic[i].x;
			magic[i].coll.top = magic[i].y;
			magic[i].coll.bottom = magic[i].y + magic[i].height;
		}
	}
}

VOID MAGIC_COLL(VOID)
{
	int x, y;

	for (int i = 0; i < PLAYER_MAGIC_MAX; i++)
	{
		if (magic[i].IsDraw == TRUE && magic[i].IsBuff == FALSE)
		{
			if (MY_CHECK_MAP1_ACT_COLL(magic[i].coll, &x, &y) == WOOD &&
				magic[i].No == 0)		//���̖��@���؂ɓ��������Ƃ�
			{
				stage[x][y].kind = ae;
			}
			if (MY_CHECK_MAP1_ACT_COLL(magic[i].coll, &x, &y) == MOVE_BLOCK &&
				magic[i].No == 2)		//���̖��@�������u���b�N�ɓ��������Ƃ�
			{
				magic[i].IsDraw = FALSE;
				//��}�X�u���b�N�̈ʒu�����炷
				if (magic[i].muki == MUKI_R && stage[x][y + 1].kind == ae)
				{
					stage[x][y].kind = ae;
					stage[x][y + 1].kind = bf;
				}
				if (magic[i].muki == MUKI_L && stage[x][y - 1].kind == ae)
				{
					stage[x][y].kind = ae;
					stage[x][y - 1].kind = bf;
				}
			}
			if (MY_CHECK_MAP1_COLL(magic[i].coll, &x, &y) == BLOCK &&	//�u���b�N�ɖ��@�����������Ƃ�
				magic[i].IsBuff == FALSE)
			{
				magic[i].IsDraw = FALSE;	//�`�����߂�
			}
		}
	}

}

VOID PLAYER_COLL_INIT(VOID)
{
	player.coll.base.right = player.x + player.width - 25;
	player.coll.base.left = player.x + 25;
	player.coll.base.top = player.y;
	player.coll.base.bottom = player.y + player.height;

	if (player.status == PLAYER_STATUS_SQUAT)//���Ⴊ��ł���Ƃ�
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
}



VOID PLAYER_ATTACK_COLL_INIT(VOID)
{
	for (int i = 0; i < PLAYER_TAMA_MAX; i++)
	{
		if (player.attack[i].IsDraw == TRUE)
		{
			//�ʏ�U���̔���
			player.attack[i].coll.left = player.attack[i].image.x;
			player.attack[i].coll.right = player.attack[i].image.x + player.attack[i].image.width;
			player.attack[i].coll.top = player.attack[i].image.y;
			player.attack[i].coll.bottom = player.attack[i].image.y + player.attack[i].image.height;
		}
	}
}



BOOL PLAYER_LOAD_IMAGE(VOID)
{
	LoadDivGraph(
		IMAGE_PLAYER_PATH,
		PLAYER_DIV_NUM, PLAYER_DIV_TATE, PLAYER_DIV_YOKO,
		PLAYER_DIV_WIDTH, PLAYER_DIV_HEIGHT,
		&player.handle[0]);
	GetGraphSize(player.handle[0], &player.width, &player.height);

	strcpy_s(PlayerLife.path, IMAGE_PLAYER_HEART_PATH);
	PlayerLife.handle = LoadGraph(PlayerLife.path);
	GetGraphSize(PlayerLife.handle, &PlayerLife.width, &PlayerLife.height);

	strcpy_s(player.attack[0].image.path, IMAGE_PLAYER_TAMA_PATH);
	player.attack[0].image.handle = LoadGraph(player.attack[0].image.path);
	GetGraphSize(player.attack[0].image.handle, &player.attack[0].image.width, &player.attack[0].image.height);

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

VOID PLAYER_DELETE_IMAGE(VOID)
{
	DeleteGraph(player.handle[0]);
	DeleteGraph(player.attack[0].image.handle);
	DeleteGraph(PlayerLife.handle);
	DeleteGraph(MagicIcon.handle[0]);
	DeleteGraph(MagicData[0].handle[0]);
	
}

BOOL PLAYER_LOAD_MUSIC(VOID)
{
	strcpy_s(AttackSE.path, MUSIC_ATTACK_SE_PATH);
	AttackSE.handle = LoadSoundMem(AttackSE.path);

	strcpy_s(HitSE.path, MUSIC_HIT_SE_PATH);
	HitSE.handle = LoadSoundMem(HitSE.path);
	return TRUE;
}
VOID PLAYER_DELETE_MUSIC(VOID)
{
	DeleteSoundMem(AttackSE.handle);
	DeleteSoundMem(HitSE.handle);
}