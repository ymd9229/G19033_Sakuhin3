#include "player.h"
#include "enemy.h"
#include "map.h"

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
		EndKind = GAME_OVER;	//エンドの種類はゲームオーバー
		GameScene = GAME_SCENE_END;	//シーンをエンド画面に遷移
	}

	if (player.life.invincible == TRUE)	//無敵状態の時
	{
		player.life.InvincibleCnt.cnt = CNT_CHECK(player.life.InvincibleCnt.cnt, player.life.InvincibleCnt.CntMax);
		if (player.life.InvincibleCnt.cnt == 0)
		{
			player.life.invincible = FALSE;
		}
		//点滅させる
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
		player.JumpRise = 10;	//上昇の値を戻す
	}
	else
	{
		player.CenterY -= player.JumpRise;	//プレイヤーの位置を上昇の値分動かす
		if (player.jump.cnt > 8 && player.JumpRise >= 0)
		{
			player.JumpRise--;	//上昇の値を下げる
		}
		player.status = PLAYER_STATUS_JUMP;
	}
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
		if (-1 < player.EquipMagic)	//選択中の魔法の番号が-1より大きいとき(-1は選択なし)
		{
			player.EquipMagic--;	//一つ前の魔法を選択
		}
		else						//選択魔法が-1(選択なし)のとき
		{
			player.EquipMagic = AvailableMagic - 1;	//最後尾の魔法を選択
		}
	}
	else if (MY_KEY_DOWN_MOMENT(KEY_INPUT_X) == TRUE)
	{
		if (AvailableMagic - 1 > player.EquipMagic)	//選択中の魔法の番号が取得済み魔法数の未満のとき
		{
			player.EquipMagic++;
		}
		else							//選択魔法が最後尾のとき
		{
			player.EquipMagic = -1;		//-1(選択なし)に戻す	
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
			
			//画面外に出たら描画をやめる
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
		if (player.change.cnt == 0)	//一定カウントごとに画像を変える
		{
			if (player.status == PLAYER_STATUS_MOVE_R)	//右に歩いているとき
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
			if (player.status == PLAYER_STATUS_MOVE_L)	//左に歩いているとき
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
			//向きごとに処理
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

	if (MY_CHECK_MAP1_COLL(player.coll.CheckBottom, &x, &y) == -1)	//プレイヤーが浮いているとき
	{
		if (player.status != PLAYER_STATUS_JUMP)	//プレイヤーの状態がジャンプ以外の時
		{
			player.CenterY += gravity;		//重力をかける
			if (player.NowJumpTimes == 0)
			{
				player.CanJumpTimes = 0;
			}
		}
	}
	if (MY_CHECK_MAP1_COLL(player.coll.CheckTop, &x, &y) == BLOCK)//上に障害物があったとき
	{
		player.CenterY = stage[x][y].y + stage[x][y].height + player.height / 2 + 10;
		player.jump.cnt = player.jump.CntMax;
	}
	if (MY_CHECK_MAP1_COLL(player.coll.CheckBottom, &x, &y) == BLOCK)//下に地面があったら着地
	{
		player.CenterY = stage[x][y].y - player.height / 2 - 1;	//下の地面の上に立たせる
		
		if(player.EquipMagic != 1)
		{
			JumpBuff = 0;
		}
		player.NowJumpTimes = 0;			//現在のジャンプ数を０に戻す
		player.CanJumpTimes = 1 + JumpBuff;	//可能なジャンプ数決める
	}
	if (MY_CHECK_MAP1_COLL(player.coll.CheckRight, &x, &y) == BLOCK)//右に障害物
	{
		player.CanRightMove = FALSE;//右に進めないようにする
	}
	if (MY_CHECK_MAP1_COLL(player.coll.CheckLeft, &x, &y) == BLOCK)//左に障害物
	{
		player.CanLeftMove = FALSE;//左に進めないようにする
	}
	if (MY_CHECK_MAP1_ACT_COLL(player.coll.CheckItem, &x, &y) == GOAL)//ゴールに当たったとき
	{
		if (NowStage == STAGE_MAX)
		{
			EndKind = GAME_CLEAR;	//エンドの種類はゲームクリア
		}
		else
		{
			EndKind = STAGE_CLEAR;  //エンドの種類はステージクリア
		}
		//ゴール時効果音
		if (CheckSoundMem(GoalSE.handle) == 0)
		{
			ChangeVolumeSoundMem(255 * 50 / 100, GoalSE.handle);//50%のボリューム
			PlaySoundMem(GoalSE.handle, DX_PLAYTYPE_BACK);
		}
		GameScene = GAME_SCENE_END;	//シーンをエンド画面に遷移
	}
	

	if (MY_CHECK_ENEMY_COLL(player.coll.base) != -1 && player.life.invincible == FALSE)//敵に当たったとき
	{
		player.life.invincible = TRUE;	//無敵状態にする
		player.life.now--;		//ライフを一つ減らす
		if (CheckSoundMem(HitSE.handle) == 0)
		{
			ChangeVolumeSoundMem(255 * 30 / 100, HitSE.handle);//30%のボリューム
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
				magic[i].No == 0)		//炎の魔法が木に当たったとき
			{
				stage[x][y].kind = ae;
			}
			if (MY_CHECK_MAP1_ACT_COLL(magic[i].coll, &x, &y) == MOVE_BLOCK &&
				magic[i].No == 2)		//風の魔法が動くブロックに当たったとき
			{
				magic[i].IsDraw = FALSE;
				//一マスブロックの位置をずらす
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
			if (MY_CHECK_MAP1_COLL(magic[i].coll, &x, &y) == BLOCK &&	//ブロックに魔法が当たったとき
				magic[i].IsBuff == FALSE)
			{
				magic[i].IsDraw = FALSE;	//描画をやめる
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

	if (player.status == PLAYER_STATUS_SQUAT)//しゃがんでいるとき
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
			//通常攻撃の判定
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