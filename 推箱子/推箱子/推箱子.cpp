#include<stdio.h>
#include<graphics.h>//EasyX图形库
#include<stdlib.h>//设置黑窗口的大小
#include<conio.h>//获取键盘输入——无需回车
#include<time.h>//计算时间
#include<string.h>

#define ROW 10
#define COL 10

/*
	1.绘制地图
	2.设置地图上不同的物品，包括:空气，墙，箱子，任务，箱子要放的位置(目的地)
		对应数字
		空气				0
		墙					1
		目的地				2
		箱子				3
		玩家				4
		//下面两个是动态发生变化的
		箱子被推到了目的地	5

	3.
*/

/*
	小功能

	1.重置关卡
	2.跳过关卡
	4.计步器

*/

#define Space  0		//空气
#define Wall   1		//墙
#define Final  2		//目的地
#define Box	    3		//箱子
#define Player  4      //玩家
//2+3 = 5——箱子到达目的地 
//玩家到了目的地和箱子到了目的地不同定义，因为是动态变化的



/*
	存储-排序
	1.创建一个结构体数组
	2.遍历保存到文件中
	3.读取
*/

#define RecordPlayerNums 10 //能够记录的玩家数量(次数)

//定义一个结构体用来存储每次玩家的数据——为排序做准备
typedef struct RpArry
{
	char name[64];//姓名
	int steps;//步数
}RpArry;

//定义一个图片数组，用来贴图
IMAGE img[6];

//加载图片
void Loadimage()
{
	for (int i = 0; i < 6; i++)
	{
		char temFileName[50] = { 0 };
		sprintf(temFileName, "./images/%d.bmp", i);
		loadimage(img + i, temFileName, 63, 63);
	}
}

RpArry rparry[RecordPlayerNums];//直接创建一个全局的结构体数组

int RankPerson = 0;//当期记录的人数

int LEVEL = 0;//当前关卡，地图数量

int Step = 0;//计步器

//先读取文件中的数据，告诉新加入的数据RankPerson从哪开始
int FilePersonNums = 0;

//主菜单
void MainMenu()
{
	printf("------------------------\n");
	printf("------》推箱子《-------\n");
	printf("------0.退出-----------\n");
	printf("------1.开始游戏-------\n");
	printf("------2.查看排行榜------\n");
	printf("------------------------\n");
}

//用二维数组来存储地图
//用三维数组来存储不同的地图

//地图
int Map[6][ROW][COL] =
{
	//第一关
	{
		{0,0,0,0,0,0,0,0,0,0},
		{0,0,0,1,1,1,0,0,0,0},
		{0,0,0,1,2,1,0,0,0,0},
		{0,0,0,1,3,1,1,1,1,0},
		{0,1,1,1,0,0,3,2,1,0},
		{0,1,2,3,0,4,1,1,1,0},
		{0,1,1,1,1,3,1,0,0,0},
		{0,0,0,0,1,2,1,0,0,0},
		{0,0,0,0,1,1,1,0,0,0},
		{0,0,0,0,0,0,0,0,0,0},
	},
	//第二关
	{
		{0,0,0,0,0,0,0,0,0,0},
		{0,0,1,1,0,0,1,1,0,0},
		{0,1,0,2,1,1,2,0,1,0},
		{1,0,0,0,0,0,0,0,0,1},
		{1,0,0,3,0,3,0,0,0,1},
		{1,0,0,0,4,0,0,0,0,1},
		{0,1,0,3,0,3,0,0,1,0},
		{0,0,1,2,0,0,2,1,0,0},
		{0,0,0,1,0,0,1,0,0,0},
		{0,0,0,0,1,1,0,0,0,0}
	},
	//第三关
	{
		{0,0,0,0,0,0,0,0,0,0},
		{0,1,1,1,1,1,1,1,1,0},
		{0,1,2,0,0,0,0,2,1,0},
		{0,1,0,0,0,0,0,0,1,0},
		{0,1,0,3,0,3,0,0,1,0},
		{0,1,0,0,4,0,0,0,1,0},
		{0,1,0,3,0,3,0,0,1,0},
		{0,1,2,0,0,0,0,2,1,0},
		{0,1,1,1,1,1,1,1,1,0},
		{0,0,0,0,0,0,0,0,0,0}
	},
	//第四关
	{
		{0,0,0,0,0,0,0,0,0,0},
		{0,0,0,1,1,1,1,0,0,0},
		{0,1,1,1,0,2,1,0,0,0},
		{0,1,0,0,0,0,1,1,1,0},
		{0,1,2,0,0,3,3,0,1,0},
		{0,1,1,2,0,0,3,4,1,0},
		{0,0,1,1,1,1,1,1,1,0},
		{0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0}
	},

	//第五关
	{
		{0,0,0,0,0,0,0,0,0,0},
		{0,0,1,1,1,1,1,1,0,0},
		{0,1,2,2,3,0,0,2,1,0},
		{0,1,3,1,0,0,1,3,1,0},
		{0,1,0,0,0,0,0,0,1,0},
		{0,1,0,1,1,1,1,0,1,0},
		{0,1,0,0,0,0,0,0,1,0},
		{0,0,1,0,4,0,0,1,0,0},
		{0,0,0,1,1,1,1,0,0,0},
		{0,0,0,0,0,0,0,0,0,0}
	},
	//第六关
	{
		{0,0,0,0,0,0,0,0,0,0},
		{0,1,1,1,1,1,1,1,1,0},
		{0,1,2,3,0,0,3,2,1,0},
		{0,1,0,1,0,0,1,0,1,0},
		{0,1,0,1,0,0,1,0,1,0},
		{0,1,0,1,4,0,1,0,1,0},
		{0,1,0,1,0,0,1,0,1,0},
		{0,1,2,3,0,0,3,2,1,0},
		{0,1,1,1,1,1,1,1,1,0},
		{0,0,0,0,0,0,0,0,0,0}
		
	},
};

//备用地图用来重置
int FakeMap[6][ROW][COL] =
{
	//第一关
	{
		{0,0,0,0,0,0,0,0,0,0},
		{0,0,0,1,1,1,0,0,0,0},
		{0,0,0,1,2,1,0,0,0,0},
		{0,0,0,1,3,1,1,1,1,0},
		{0,1,1,1,0,0,3,2,1,0},
		{0,1,2,3,0,4,1,1,1,0},
		{0,1,1,1,1,3,1,0,0,0},
		{0,0,0,0,1,2,1,0,0,0},
		{0,0,0,0,1,1,1,0,0,0},
		{0,0,0,0,0,0,0,0,0,0},
	},
	//第二关
	{
		{0,0,0,0,0,0,0,0,0,0},
		{0,0,1,1,0,0,1,1,0,0},
		{0,1,0,2,1,1,2,0,1,0},
		{1,0,0,0,0,0,0,0,0,1},
		{1,0,0,3,0,3,0,0,0,1},
		{1,0,0,0,4,0,0,0,0,1},
		{0,1,0,3,0,3,0,0,1,0},
		{0,0,1,2,0,0,2,1,0,0},
		{0,0,0,1,0,0,1,0,0,0},
		{0,0,0,0,1,1,0,0,0,0}
	},
	//第三关
	{
		{0,0,0,0,0,0,0,0,0,0},
		{0,1,1,1,1,1,1,1,1,0},
		{0,1,2,0,0,0,0,2,1,0},
		{0,1,0,0,0,0,0,0,1,0},
		{0,1,0,3,0,3,0,0,1,0},
		{0,1,0,0,4,0,0,0,1,0},
		{0,1,0,3,0,3,0,0,1,0},
		{0,1,2,0,0,0,0,2,1,0},
		{0,1,1,1,1,1,1,1,1,0},
		{0,0,0,0,0,0,0,0,0,0}
	},
	//第四关
	{
		{0,0,0,0,0,0,0,0,0,0},
		{0,0,0,1,1,1,1,0,0,0},
		{0,1,1,1,0,2,1,0,0,0},
		{0,1,0,0,0,0,1,1,1,0},
		{0,1,2,0,0,3,3,0,1,0},
		{0,1,1,2,0,0,3,4,1,0},
		{0,0,1,1,1,1,1,1,1,0},
		{0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0}
	},

	//第五关
	{
		{0,0,0,0,0,0,0,0,0,0},
		{0,0,1,1,1,1,1,1,0,0},
		{0,1,2,2,3,0,0,2,1,0},
		{0,1,3,1,0,0,1,3,1,0},
		{0,1,0,0,0,0,0,0,1,0},
		{0,1,0,1,1,1,1,0,1,0},
		{0,1,0,0,0,0,0,0,1,0},
		{0,0,1,0,4,0,0,1,0,0},
		{0,0,0,1,1,1,1,0,0,0},
		{0,0,0,0,0,0,0,0,0,0}
	},
	//第六关
	{
		{0,0,0,0,0,0,0,0,0,0},
		{0,1,1,1,1,1,1,1,1,0},
		{0,1,2,3,0,0,3,2,1,0},
		{0,1,0,1,0,0,1,0,1,0},
		{0,1,0,1,0,0,1,0,1,0},
		{0,1,0,1,4,0,1,0,1,0},
		{0,1,0,1,0,0,1,0,1,0},
		{0,1,2,3,0,0,3,2,1,0},
		{0,1,1,1,1,1,1,1,1,0},
		{0,0,0,0,0,0,0,0,0,0}

	},
};


//输出地图-符号-图形
void ShowMap()
{
	//利用循环与条件判断语句的嵌套来打印地图
	//注意i j的顺序，顺序不同，贴图不同！
	for (int i = 0; i < 10; i++)
	{
		for (int j = 0; j < 10; j++)
		{
			if (Map[LEVEL][i][j] == Space)
			{
				putimage(j * 63, i * 63, img + 0);
				printf("  ");
			}

			else if (Map[LEVEL][i][j] == Wall)
			{

				putimage(j * 63, i * 63, img + 1);
				printf("▊");
			}
			else if (Map[LEVEL][i][j] == Final)
			{
				putimage(j * 63, i * 63, img + 2);
				printf("☆");
			}
			else if (Map[LEVEL][i][j] == Box)
			{
				putimage(j * 63, i * 63, img + 3);
				printf("★");
			}
			else if (Map[LEVEL][i][j] == Player)
			{
				putimage(j * 63, i * 63, img + 4);
				printf("♀");
			}
			else if (Map[LEVEL][i][j] == Box + Final)
			{
				putimage(j * 63, i * 63, img + 5);
				printf("★");
			}
			else if (Map[LEVEL][i][j] == Final + Player)
			{
				putimage(j * 63, i * 63, img + 4);
				printf("♂");
			}
			else
			{
				printf("  ");
			}
		}
		printf("\n");
	}
}

//重置关卡
void ReSetLevel()
{
	for (int s = 0; s < 10; s++)
	{
		for (int b = 0; b < 10; b++)
		{
			Map[LEVEL][s][b] = FakeMap[LEVEL][s][b];
		}
	}
}

//获取键盘输入，进行箱子和玩家的移动——推箱子
void PushBox()
{
	//应该先找到玩家在哪，然后再进行控制
	//使用一个循环嵌套来找到玩家Player
	//i是行，j是列
	//利用i和j来保存找到玩家的下标
	int i = 0;//行-上下
	int j = 0;//列-左右
	//添加标记跳出嵌套循环
	int flag = 0;
	for (i = 0; i < 10; i++)
	{
		for (j = 0; j < 10; j++)
		{
			//什么是玩家？
			//玩家个体或者玩家进入到目的地里
			if (Map[LEVEL][i][j] == Player || Map[LEVEL][i][j] == Player + Final)
			{
				flag = 1;
				break;
			}
		}
		if (flag == 1)
		{
			break;
		}
	}

	//提示
	if (LEVEL < 4)
	{
		printf("按空格重置关卡\n");
		printf("按回车跳过关卡\n");
		printf("当前步数%d", Step);
	}

	//_getch接收键盘输入
	char KeyBorad = _getch();
	//移动操作
	switch (KeyBorad)//利用方向键和wasd来进行操作
	{
		/*
			什么时候可以进行移动？
			前面是空气，或者是箱子，并且箱子后面不能是墙
		*/

		//向上移动
	case 'w':
	case 'W':
	case  72:
	{
		//如果玩家前面是-空气或者-目的地就可以移动
		if (Map[LEVEL][i - 1][j] == Space || Map[LEVEL][i - 1][j] == Final)
		{
			//玩家进行移动——加减操作——宏定义中已经给出都代表的是什么
			Map[LEVEL][i - 1][j] += Player;
			//之前那个位置同样发生了变化
			Map[LEVEL][i][j] -= Player;
			Step++;
		}
		//如果玩家的前面是箱子
		else if (Map[LEVEL][i - 1][j] == Box)
		{
			//并且箱子的前面是目的地或者空气
			if (Map[LEVEL][i - 2][j] == Space || Map[LEVEL][i - 2][j] == Final)
			{
				//箱子向上移动
				Map[LEVEL][i - 2][j] += Box;
				//玩家跟着移动——覆盖之前箱子所在的位置
				Map[LEVEL][i - 1][j] -= Box;
				Map[LEVEL][i - 1][j] += Player;
				//玩家之前的位置发生变化
				Map[LEVEL][i][j] -= Player;
				Step++;
			}
		}
		break;
	}

	//向下移动
	case 's':
	case 'S':
	case 80:
	{
		//如果下面是空气或者是目的地就可以进行移动
		if (Map[LEVEL][i + 1][j] == Space || Map[LEVEL][i + 1][j] == Final)
		{
			Map[LEVEL][i + 1][j] += Player;
			Map[LEVEL][i][j] -= Player;
			Step++;
		}
		//如果下面是箱子
		else if (Map[LEVEL][i + 1][j] == Box)
		{
			//并且箱子下面是空气或者是目的地
			if (Map[LEVEL][i + 2][j] == Space || Map[LEVEL][i + 2][j] == Final)
			{
				//箱子往下面移动一个位置——其实并不是发生移动，而是要移动方向的位置上面显示的内容发生了变化,道理同上
				Map[LEVEL][i + 2][j] += Box;
				//人往下面移动一个位置
				Map[LEVEL][i + 1][j] -= Box;
				Map[LEVEL][i + 1][j] += Player;
				//人之前位置上变成空气
				Map[LEVEL][i][j] -= Player;
				Step++;
			}
		}
		break;
	}

	//判断同上不做解释
	//向左移动
	case 'a':
	case'A':
	case 75:
	{
		if (Map[LEVEL][i][j - 1] == Space || Map[LEVEL][i][j - 1] == Final)
		{
			Map[LEVEL][i][j - 1] += Player;
			Map[LEVEL][i][j] -= Player;
			Step++;
		}
		else if (Map[LEVEL][i][j - 1] == Box)
		{
			if (Map[LEVEL][i][j - 2] == Space || Map[LEVEL][i][j - 2] == Final)
			{
				Map[LEVEL][i][j - 2] += Box;
				Map[LEVEL][i][j - 1] -= Box;
				Map[LEVEL][i][j - 1] += Player;
				Map[LEVEL][i][j] -= Player;
				Step++;
			}
		}
		break;
	}

	//向右移动
	case 'd':
	case 'D':
	case 77:
	{
		if (Map[LEVEL][i][j + 1] == Space || Map[LEVEL][i][j + 1] == Final)
		{
			Map[LEVEL][i][j + 1] += Player;
			Map[LEVEL][i][j] -= Player;
			Step++;
		}
		else  if (Map[LEVEL][i][j + 1] == Box)
		{
			if (Map[LEVEL][i][j + 2] == Space || Map[LEVEL][i][j + 2] == Final)
			{
				Map[LEVEL][i][j + 2] += Box;
				Map[LEVEL][i][j + 1] -= Box;
				Map[LEVEL][i][j + 1] += Player;
				Map[LEVEL][i][j] -= Player;
				Step++;
			}
		}
		break;
	}

	//空格重置关卡
	case 32:
	{
		ReSetLevel();
		ShowMap();
		break;
	}

	//跳关
	case 13:
		if (LEVEL < 5)
		{
			LEVEL++;
		}
		break;
	}
}

//判断是否过关
bool PassLevel()
{
	//如果里面所有的箱子全到了目的地，就通关
	//嵌套循环，地图里面没有箱子了就是通关了
	for (int i = 0; i < 10; i++)
	{
		for (int j = 0; j < 10; j++)
		{
			if (Map[LEVEL][i][j] == Box)
			{
				return false;
			}
		}
	}
	//system()
	//过了就重置关卡(有点重复)
	ReSetLevel();
	return true;
}

void PopWindow();//前置声明

//开始游戏
void StartGame()
{
	//system("cls");
	ShowMap();
	settextstyle(30, 0, NULL);
	settextcolor(RGB(0, 0, 0));
	setbkmode(TRANSPARENT);
	outtextxy(0, 0, "当前步数:");
	outtextxy(200,0,"第");
	char q[5];
	sprintf(q, "%d", LEVEL+1);
	outtextxy(300,0, q);
	outtextxy(380,0,"关");
	char s[5];
	sprintf(s, "%d", Step);
	outtextxy(0, 30, s);
	outtextxy(0, 570, "空格键重置当前关卡");
	outtextxy(0, 600, "回车键跳过当前关卡");


	if (PassLevel())//判断是否过关，过关就切换下一张地图
	{
		//过关——切换关卡
		LEVEL++;

		//通关
		if (LEVEL > 5)
		{
			//进入到这里说明所有关卡都已经通过
			//int End = clock();//结束——计时
			//int TimeResult = End - Begin;
			printf("恭喜你%s，完成了游戏\n", rparry[RankPerson].name);
			//printf("用时%d秒\n", TimeResult / 600);

			//将步数存储进结构体数组
			printf("走了%d步\n", Step);
			PopWindow();//恭喜过关的弹窗提示

		}
	}
	PushBox();
}

//保存记录-文件
void SavePlayerRecord()
{

	//文件操作
	FILE* fp = fopen("PlayerRecord.txt", "a");
	if (fp == NULL)
	{
		printf("读取文件失败\n");
		return;
	}
	fprintf(fp, "\t%s\t%d\n", rparry[RankPerson].name, rparry[RankPerson].steps);
	fclose(fp);
	system("pause");
	system("cls");
}
/*
	要实现一个按照步数大小的排行榜
	也可以不往文件里面写，而是再读取文件的时候读出来，先输出步数小的
*/
////读取记录-文件
//void ReadPlayerRecord()
//{
//	FILE* fp = fopen("PlayerRecord.txt", "r");
//	if (fp == NULL)
//	{
//		printf("当前记录为空!\n");
//		return;
//	}
//
//	//左侧侧边栏
//	int i = 1;
//
//	//先将原来的文件读取到结构体数组中——主函数第一行已经实现
//	//读取完毕，现在数据已经被保存到结构体数组中
//	//按照从小到大的顺序输出即可
//
//	//读取器
//	char Gets[164];
//	while (!feof(fp))
//	{
//		fgets(Gets, 164, fp);
//		//feop的问题——需要第二次检查结尾
//		if (feof(fp))
//		{
//			break;
//		}
//		else
//		{
//
//		}
//	}
//}
//将文件读取到当前结构体数组——得出文件中有几个人
void ReadRecordFile()
{
	FILE* fp = fopen("PlayerRecord.txt", "r");//另一个文件
	if (fp == NULL)
	{
		//printf("打开文件失败\n");
		return;
	}
	for (int i = 0; i < RecordPlayerNums; i++)
	{
		fscanf(fp, "%s%d", rparry[i].name, &rparry[i].steps);
	}
	//读取出文件中数据的人数个数
	for (int i = 0; i < RecordPlayerNums; i++)
	{
		if (rparry[i].steps != 0)
		{
			//当前人数
			RankPerson = ++FilePersonNums;
		}
	}
	fclose(fp);
}
//排序&更新到文件中
void RecordBubbSort()
{
	//冒泡排序
	//交换的时候注意是结构体
	for (int i = 0; i < RankPerson; i++)
	{
		RpArry Temprparry;
		for (int j = 1; j < RankPerson - i; j++)
		{
			if (rparry[j - 1].steps >= rparry[j].steps)
			{
				Temprparry = rparry[j];
				rparry[j] = rparry[j - 1];
				rparry[j - 1] = Temprparry;
			}
		}
	}

	//printf("排名\t玩家\t\t步数\t\n");
	//for (int i = 0; i < RankPerson; i++)
	//{
	//	printf("%d", i + 1);
	//	printf("\t%s\t\t%d\n", rparry[i].name, rparry[i].steps);
	//}
	//FILE* fp = fopen("PlayerRecord.txt", "w");
	//for (int i = 0; i < RankPerson; i++)
	//{
	//	if (rparry[i].steps != 0)
	//	{
	//		fprintf(fp,"\t%s\t%d\n",rparry[i].name,rparry[i].steps);
	//	}
	//}
}
/*
	排行榜思路
	1.自动读取文件中的内容，保存当前的结构体数组中
	2.对结构体数组按照步数的大小进行重新排序
	3.打印输出
*/
//删除文件&清空用户记录
void DeletePlayerRecord()
{
	//提示
	//printf("确定清空？\n");
	//printf("1.确定 2.取消\n");
	//int select = 0;
	//scanf("%d", &select);
	//if (select == 1)
	//{
	//	//直接删除记录文件
	//	remove("PlayerRecord.txt");
	//	//将当前人数置空
	//	RankPerson = 0;
	//	printf("清空完毕!\n");
	//}
	//else
	//{
	//	printf("记录并未清除。\n");
	//}

	//直接删除记录文件
	remove("PlayerRecord.txt");
	//将当前人数置空
	RankPerson = 0;
	//测试
	printf("清空完毕!\n");
}

void GraphInputName();
void GraphRankPage();


//图形界面的首页
void GraphFirImagesPg()
{
	MOUSEMSG m;
	//设置图形窗口大小
	initgraph(ROW * 63, COL * 63);//SHOWCONSOLE显示命令行窗口
	Loadimage();

	setbkcolor(BLACK);

	cleardevice();//刷新一次 略等于那个system(cls)

	IMAGE picture1;
	loadimage(&picture1, "./topbc.jpg", 630, 630);
	putimage(0, 0, &picture1);

	setfillcolor(GREEN);

	fillrectangle(230, 405, 400, 430);

	fillrectangle(230, 445, 400, 470);

	fillrectangle(230, 485, 400, 510);


	settextstyle(85, 0, "黑体");
	char Title[] = "推箱子";
	char Select1[] = "开始游戏";
	char Select2[] = "排行榜";
	char Select3[] = "退出游戏";


	//大标题
	outtextxy(210, 150, Title);
	//下面的选项
	settextstyle(30, 0, NULL);
	setbkmode(TRANSPARENT);// 去掉文字背景
	outtextxy(252, 405, Select1);
	outtextxy(268, 445, Select2);
	outtextxy(255, 485, Select3);



	while (1)
	{
		m = GetMouseMsg();//鼠标
		if (m.x >= 230 && m.x <= 400 && m.y >= 405 && m.y <= 430)//开始游戏
		{
			setlinecolor(RED);
			rectangle(220, 400, 410, 440);
			if (m.uMsg == WM_LBUTTONDOWN)
			{
				GraphInputName();


				cleardevice();//清空绘图设备
				while (LEVEL < 6)
				{
					StartGame();

				}
				//先给对应的人赋值存进结构体
				rparry[RankPerson].steps = Step;
				//保存数据进文件
				SavePlayerRecord();
				//重置关数
				LEVEL = 0;
				//重置步数
				Step = 0;
				//数组存储下一个
				RankPerson++;
				//重置关卡
				ReSetLevel();
				//清除
				cleardevice();
				GraphFirImagesPg();
			}
		}
		else if (m.x >= 230 && m.x <= 400 && m.y >= 445 && m.y <= 470)//查看排行榜&清空加在这里面
		{
			setlinecolor(RED);
			rectangle(220, 440, 410, 480);
			if (m.uMsg == WM_LBUTTONDOWN)
			{
				cleardevice();
				GraphRankPage();

			}

		}
		else if (m.x >= 230 && m.x <= 400 && m.y >= 485 && m.y <= 510)//退出游戏
		{
			setlinecolor(RED);
			rectangle(220, 480, 410, 520);
			if (m.uMsg == WM_LBUTTONDOWN)
			{
				exit(0);
				break;
			}
		}
		//鼠标不在相应的范围内边框就消失
		else
		{
			setlinecolor(BLACK);
			rectangle(220, 400, 410, 440);
			rectangle(220, 440, 410, 480);
			rectangle(220, 480, 410, 520);
		}



	}
}


//图形界面开始游戏-输入昵称
void GraphInputName()
{

	TCHAR InputWindow[] = _T("请输入你的昵称");
	InputBox(rparry[RankPerson].name, 10, InputWindow);//输入昵称
}




//一把玩完后的弹窗
void PopWindow()
{


	//获取窗口句柄
	HWND hnd = GetHWnd();
	//弹出窗口提示用户操作
	//加上窗口句柄 强制用户先点击再进行操作

	//用一个字符数组来总结句子
	int isok = MessageBox(hnd, "恭喜通关", "提示", MB_OK);
}

void GraphDeleteRankPage();
//图形界面排行榜
void GraphRankPage()
{
	MOUSEMSG m1;//鼠标
	cleardevice();
	
	//清空按钮
	setbkcolor(BLACK);

	cleardevice();//刷新一次 略等于那个system(cls)

	setfillcolor(GREEN);
	fillrectangle(440, 90, 580, 140);
	fillrectangle(440, 150, 580, 200);
	settextstyle(30, 0, "黑体");
	setbkmode(TRANSPARENT);
	char Delete[10] = { "清空数据" };//清空按钮
	char Back[10] = { "返回" };

	outtextxy(480, 158, Back);
	outtextxy(450, 100, Delete);
	outtextxy(60, 100, "排名");
	outtextxy(220, 100, "昵称");
	outtextxy(350, 100, "步数");

	if (RankPerson == 0)
	{
		settextstyle(50, 0, "黑体");
		char TIPS[20] = { "当前记录为空!" };
		outtextxy(130, 238, TIPS);
	}

	RecordBubbSort();



	for (int i = 1; i <= RankPerson; i++)
	{
		char a[5];
		sprintf(a, "%d", rparry[i - 1].steps);
		char b[5];
		sprintf(b, "%d", i);
		outtextxy(60, 100 + (50 * i), b);
		outtextxy(220, 100 + (50 * i), rparry[i - 1].name);
		outtextxy(350, 100 + (50 * i), a);
	}

	//清空按钮
	while (1)
	{
		m1 = GetMouseMsg();
		if (m1.x >= 440 && m1.x <= 580 && m1.y >= 85 && m1.y <= 150)
		{
			rectangle(430, 85, 590, 150);
			setlinecolor(RED);
			if (m1.uMsg == WM_LBUTTONDOWN)
			{
				GraphDeleteRankPage();//清空排行榜
				cleardevice();//清屏
				GraphRankPage();
			}
		}
		else if (m1.x>=440 && m1.x<=580&&m1.y >=150 && m1.y<200)
		{
			rectangle(430, 145, 590, 210);
			setlinecolor(RED);
			if (m1.uMsg == WM_LBUTTONDOWN)
			{	
				cleardevice();
				GraphFirImagesPg();
			}
		}
		else
		{
			setlinecolor(BLACK);
			rectangle(430, 85, 590, 150);
			rectangle(430, 145, 590, 210);

		}
	}
}

//清空记录的图形界面
void GraphDeleteRankPage()
{
	//获取窗口句柄
	HWND hnd = GetHWnd();
	//弹出窗口提示用户操作
	//加上窗口句柄 强制用户先点击再进行操作
	//用一个字符数组来总结句子
	int isok = MessageBox(hnd, "确认清空?", "提示", MB_OKCANCEL);
	if (isok == IDOK)
	{
		int tips1 = MessageBox(hnd, "清空成功！", "提示", MB_OK);
		DeletePlayerRecord();
	}
	else if (isok == IDCANCEL)
	{
		//点击取消不做反应
	}
}


//主函数
int main(void)
{
	//先把文件中本来有的个数计算出来
	ReadRecordFile();

	//图形界面以及鼠标点击
	GraphFirImagesPg();


	//设置命令行黑窗口的大小
	//system("mode con cols=30 lines=20");//设置cols和lines数值的时候不能有空格，否则会报错。
	//while (1)
	//{

	//	MainMenu();
	//	printf("请输入你的选择:\n");

	

	//	int Select = 0;
	//	scanf_s("%d", &Select);
	//	system("cls");


	//	switch (Select)
	//	{
	//		/////////////////////////////////////////////////////////////////////退出
	//	case 0:
	//		printf("游戏结束，再见:)。\n");
	//		exit(0);
	//		/////////////////////////////////////////////////////////////////////开始游戏
	//	case 1:
	//		//图形界面输入用户名
	//		GraphInputName();
	//		//进入游戏之前会提示你输入用户名
	//		//printf("请输入你的昵称:\n");
	//		//
	//		//scanf("%s", rparry[RankPerson].name);
	//		system("cls");

	//		//cleardevice();//清空绘图设备

	//		while (LEVEL < 4)
	//		{
	//			StartGame();
	//		
	//		}
	//		//先给对应的人赋值存进结构体
	//		rparry[RankPerson].steps = Step;
	//		//保存数据进文件
	//		SavePlayerRecord();
	//		//重置关数
	//		LEVEL = 0;
	//		//重置步数
	//		Step = 0;
	//		//数组存储下一个
	//		RankPerson++;
	//		//重置关卡
	//		ReSetLevel();

	//		//清除
	//		cleardevice();
	//		break;
	//		//////////////////////////////////////////////////////////////////////排行榜
	//	case 2:

			//图形界面的排行榜

			//成功代替之前的ReadPlayerRecord
			//if (RankPerson == 0)
			//{
			//	printf("当前记录为空！\n");
			//}
			//else
			//{
			//	//当前记录数
			//	printf("当前记录数:%d\n", RankPerson);
			//	//排序&输出
			//	RecordBubbSort();
			//}
			//system("pause");
			//break;
			///////////////////////////////////////////////////////////////////////清空排行榜&清空文件
	//	case 3:
	//		//DeletePlayerRecord();
	//		GraphDeleteRankPage();
	//		break;


	//		//////////////////////////////////////////////////////////////////////输入有误
	//	default:
	//		printf("选择有误，请重新输入。\n");
	//		system("pause");
	//		break;
	//	}

	//	system("cls");
	//}


	return 0;
}