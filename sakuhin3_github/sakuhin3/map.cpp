#include "map.h"

VOID STAGE_INIT(VOID)
{
	for (int tate = 0; tate < GAME_MAP_TATE_MAX; tate++)
	{
		for (int yoko = 0; yoko < GAME_MAP_YOKO_MAX; yoko++)
		{
			switch (NowStage)
			{
			case 1:
				stage[tate][yoko].kind = stage1Data[tate][yoko];
				break;
			case 2:
				stage[tate][yoko].kind = stage2Data[tate][yoko];
				break;
			case 3:
				stage[tate][yoko].kind = stage3Data[tate][yoko];
				break;
			}

			stage[tate][yoko].width = mapchip.width;
			stage[tate][yoko].height = mapchip.height;
			stage[tate][yoko].x = yoko * stage[tate][yoko].width;
			stage[tate][yoko].y = tate * stage[tate][yoko].height;

			if (stage[tate][yoko].kind == ha)
			{
				book.image.x = stage[tate][yoko].x;
				book.image.y = stage[tate][yoko].y;
			}
		}
	}
}

VOID STAGE_DRAW(VOID)
{
	for (int tate = 0; tate < GAME_MAP_TATE_MAX; tate++)
	{
		for (int yoko = 0; yoko < GAME_MAP_YOKO_MAX; yoko++)
		{
			DrawGraph(
				stage[tate][yoko].x,
				stage[tate][yoko].y,
				mapchip.handle[stage[tate][yoko].kind],
				TRUE);
		}
	}
	if (book.IsDraw == TRUE)
	{
		DrawGraph(book.image.x, book.image.y, book.image.handle, TRUE);
	}
}

BOOL MAP_LOAD_IMAGE(VOID)
{
	strcpy_s(book.image.path, IMAGE_BOOK_PATH);
	book.image.handle = LoadGraph(book.image.path);
	GetGraphSize(book.image.handle, &book.image.width, &book.image.height);

	for (int i = 0; i < STAGE_MAX; i++)
	{
		switch (i)
		{
		case 0:
			strcpy_s(StageBack[i].path, IMAGE_STAGE1_BACK_PATH);
			StageBack[i].handle = LoadGraph(StageBack[i].path);
			break;
		case 1:
			strcpy_s(StageBack[i].path, IMAGE_STAGE2_BACK_PATH);
			StageBack[i].handle = LoadGraph(StageBack[i].path);
			break;
		case 2:
			strcpy_s(StageBack[i].path, IMAGE_STAGE3_BACK_PATH);
			StageBack[i].handle = LoadGraph(StageBack[i].path);
			break;
		}
	}

	LoadDivGraph(
		IMAGE_MAP1_PATH,
		MAP_DIV_NUM, MAP_DIV_TATE, MAP_DIV_YOKO,
		MAP_DIV_WIDTH, MAP_DIV_HEIGHT,
		&mapchip.handle[0]);
	GetGraphSize(mapchip.handle[0], &mapchip.width, &mapchip.height);

	return TRUE;
}

VOID MAP_DELETE_IMAGE(VOID)
{
	DeleteGraph(book.image.handle);
	DeleteGraph(mapchip.handle[0]);
	for (int i = 0; i < STAGE_MAX; i++)
	{
		DeleteGraph(StageBack[i].handle);
	}

}

BOOL MAP_LOAD_MUSIC(VOID)
{
	strcpy_s(Stage1BGM.path, MUSIC_STAGE1_BGM_PATH);
	Stage1BGM.handle = LoadSoundMem(Stage1BGM.path);
	return TRUE;
}

VOID MAP_DELETE_MUSIC(VOID)
{
	DeleteSoundMem(Stage1BGM.handle);
}

VOID STAGE_COLL_INIT(VOID)
{
	for (int tate = 0; tate < GAME_MAP_TATE_MAX; tate++)
	{
		for (int yoko = 0; yoko < GAME_MAP_YOKO_MAX; yoko++)
		{
			stage[tate][yoko].coll.left = stage[tate][yoko].x;
			stage[tate][yoko].coll.right = stage[tate][yoko].x + stage[tate][yoko].width;
			stage[tate][yoko].coll.top = stage[tate][yoko].y;
			stage[tate][yoko].coll.bottom = stage[tate][yoko].y + stage[tate][yoko].height;
		}
	}
}