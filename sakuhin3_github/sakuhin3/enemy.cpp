#include"enemy.h"
#include "player.h"
#include "map.h"
VOID ENEMY_PROC(VOID)
{
	for (int m = 0; m < ENEMY_DATA_MAX; m++)//“G‚Ìƒf[ƒ^‚Ì”‚Ü‚Å
	{
		//“G‚ÌƒXƒ^[ƒgˆÊ’u‚ª‰æ–Ê“à‚É‚ ‚èA‚Ü‚¾ˆê“x‚à‚»‚Ì“G‚ð•`‰æ‚µ‚Ä‚È‚¢‚Æ‚«
		if (EnemyData[m].StartX > screen.left && EnemyData[m].StartX < screen.right &&
			EnemyData[m].StartY > screen.top && EnemyData[m].StartY < screen.bottom &&
			EnemyData[m].stage == NowStage && EnemyData[m].IsDraw == FALSE)
		{
			int n = ENEMY_CHECK();
			if (n != -1)
			{
				//“G‚Ìƒf[ƒ^‚ð“o˜^‚·‚é
				enemy[n].x = EnemyData[m].StartX - (double)screen.left;
				enemy[n].y = EnemyData[m].StartY;
				enemy[n].kind = EnemyData[m].kind;
				enemy[n].width = EnemyData[m].width;
				enemy[n].height = EnemyData[m].height;
				enemy[n].CenterX = enemy[n].x + enemy[n].width / 2;
				enemy[n].CenterY = enemy[n].y + enemy[n].height / 2;
				enemy[n].IsFly = EnemyData[m].IsFly;

				enemy[n].IsDraw = TRUE;		//“G‚ð•`‰æ‚Å‚«‚é‚æ‚¤‚É‚·‚é
				EnemyData[m].IsDraw = TRUE;		//‚”Ô–Ú‚Ìƒf[ƒ^‚Ì“G‚Í•`‰æÏ‚Ý
			}
		}
	}
	for (int n = 0; n < ENEMY_MAX; n++)
	{
		if (enemy[n].IsDraw == TRUE)//“G‚ª•`‰æ‚³‚ê‚Ä‚¢‚é‚Æ‚«
		{
			ENEMY_MOVE(n);//“G‚Ì“®‚«‚Ìˆ—
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
	switch (enemy[n].kind)//“G‚ÌŽí—Þ‚É‚æ‚Á‚Ä“®‚«‚ðŒˆ‚ß‚é
	{
	case 0:	//^‚Á’¼‚®i‚Ý•Ç‚É“–‚½‚é‚Æ”½‘Î‚Ì•ûŒü‚ÖŒü‚«‚ð•Ï‚¦‚é
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
	case 1:	//ƒvƒŒƒCƒ„[‚ªˆê’è‚Ì‹——£“à‚É‚¢‚é‚ÆƒvƒŒƒCƒ„[‚ð’Ç”ö‚·‚é
	{
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

		int Distance = sqrt(pow(DistanceX, 2) + pow(DistanceY, 2));
		if (Distance < enemy[n].width * 5)
		{
			enemy[n].CenterX += (player.CenterX - enemy[n].CenterX) / Distance * 2;
			enemy[n].CenterY += (player.CenterY - enemy[n].CenterY) / Distance * 2;
		}
	}
		break;
	case 2:	//^‚Á’¼‚®i‚Ý•Ç‚É“–‚½‚é‚Æ”½‘Î‚Ì•ûŒü‚ÖŒü‚«‚ð•Ï‚¦‚é
		if (enemy[n].CanLeftMove == TRUE)
		{
			enemy[n].muki = MUKI_L;
			enemy[n].speed = ENEMY_SPEED_NORMAL;
			enemy[n].CenterX -= enemy[n].speed;
		}
		if (enemy[n].CanRightMove == TRUE)
		{
			enemy[n].muki = MUKI_R;
			enemy[n].speed = ENEMY_SPEED_NORMAL;
			enemy[n].CenterX += enemy[n].speed;
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
			//ƒJƒEƒ“ƒg‚µ‚Ä‰æ‘œ‚ð“ü‚ê‘Ö‚¦‚é
			enemy[n].change.CntMax = 20;
			enemy[n].change.cnt = CNT_CHECK(enemy[n].change.cnt, enemy[n].change.CntMax);
			if(enemy[n].change.cnt == 0)
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

			//‰æ–ÊŠO‚És‚­‚ÆÁ‚¦‚é
			if (enemy[n].x + enemy[n].width < -GAME_WIDTH || enemy[n].x > GAME_WIDTH)
			{
				enemy[n].IsDraw = FALSE;
			}
		}
	}
}

BOOL ENEMY_LOAD_IMAGE(VOID)
{
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
		case 2:
			LoadDivGraph(
				IMAGE_ENEMY3_PATH,
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
	return TRUE;
}

VOID ENEMY_DELETE_IMAGE(VOID)
{
	for (int k = 0; k < ENEMY_IMAGE_KIND; k++)
	{
		DeleteGraph(EnemyImage[k].handle[0]);
	}
}

BOOL ENEMY_LOAD_MUSIC(VOID)
{
	strcpy_s(DefeatSE.path, MUSIC_DEFEAT_SE_PATH);
	DefeatSE.handle = LoadSoundMem(DefeatSE.path);
	return TRUE;
}

VOID ENEMY_DELETE_MUSIC(VOID)
{
	DeleteSoundMem(DefeatSE.handle);
}

VOID ENEMY_COLL(VOID)
{
	int x, y;
	for (int n = 0; n < ENEMY_MAX; n++)
	{
		if (enemy[n].IsDraw == TRUE)//“G‚ª•`‰æ‚³‚ê‚Ä‚¢‚é‚Æ‚«
		{
			if (MY_CHECK_MAP1_COLL(enemy[n].coll.CheckLeft, &x, &y) == BLOCK)//¶‚ÉáŠQ•¨
			{
				enemy[n].CanLeftMove = FALSE;
				enemy[n].CanRightMove = TRUE;
			}
			if (MY_CHECK_MAP1_COLL(enemy[n].coll.CheckRight, &x, &y) == BLOCK)//‰E‚ÉáŠQ•¨
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
				enemy[n].CenterY = (double)stage[x][y].y - enemy[n].height / 2 - 1;
			}

			enemy[n].x = enemy[n].CenterX - enemy[n].width / 2;
			enemy[n].y = enemy[n].CenterY - enemy[n].height / 2;
		}
	}
}

VOID ENEMY_COLL_INIT(VOID)
{
	for (int n = 0; n < ENEMY_MAX; n++)
	{
		if (enemy[n].IsDraw == TRUE)//“G‚ª•`‰æ‚³‚ê‚Ä‚¢‚é‚Æ‚«
		{
			enemy[n].x = enemy[n].CenterX - enemy[n].width / 2;
			enemy[n].y = enemy[n].CenterY - enemy[n].height / 2;

			//“G‚Ì“–‚½‚è”»’è

			//Šî–{‚Ì”»’è
			enemy[n].coll.base.left = enemy[n].x + 15;
			enemy[n].coll.base.right = enemy[n].x + enemy[n].width - 15;
			enemy[n].coll.base.top = enemy[n].y + 15;
			enemy[n].coll.base.bottom = enemy[n].y + enemy[n].height;
			//ã‘¤‚É­‚µ‘å‚«‚¢”»’è
			enemy[n].coll.CheckTop.left = enemy[n].coll.base.left;
			enemy[n].coll.CheckTop.right = enemy[n].coll.base.right;
			enemy[n].coll.CheckTop.top = enemy[n].coll.base.top + 10;
			enemy[n].coll.CheckTop.bottom = enemy[n].coll.base.bottom;
			//‰º‘¤‚É­‚µ‘å‚«‚¢”»’è
			enemy[n].coll.CheckBottom.left = enemy[n].coll.base.left;
			enemy[n].coll.CheckBottom.right = enemy[n].coll.base.right;
			enemy[n].coll.CheckBottom.top = enemy[n].coll.base.top;
			enemy[n].coll.CheckBottom.bottom = enemy[n].coll.base.bottom + 10;
			//¶‘¤‚É­‚µ‘å‚«‚¢”»’è
			enemy[n].coll.CheckLeft.left = enemy[n].coll.base.left - 5;
			enemy[n].coll.CheckLeft.right = enemy[n].coll.base.right - 5;
			enemy[n].coll.CheckLeft.top = enemy[n].coll.base.top;
			enemy[n].coll.CheckLeft.bottom = enemy[n].coll.base.bottom;
			//‰E‘¤‚É­‚µ‘å‚«‚¢”»’è
			enemy[n].coll.CheckRight.left = enemy[n].coll.base.left + 5;
			enemy[n].coll.CheckRight.right = enemy[n].coll.base.right + 5;
			enemy[n].coll.CheckRight.top = enemy[n].coll.base.top;
			enemy[n].coll.CheckRight.bottom = enemy[n].coll.base.bottom;
		}
	}
}