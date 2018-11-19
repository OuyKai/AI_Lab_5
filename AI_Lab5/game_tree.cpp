#include <fstream> //文件流的输入输出
#include <string>
#include <assert.h> //断言，这个主要是为了调试的方便
#include <iostream>
using namespace std;

ifstream fin("stone.in"); //文件读入
ofstream fout("stone.out"); //文件输出

int grid[8][8]; //定义整个棋盘，其中的grid就是网格的意思
int num, mark[10][2], enable[10]; //由于问题中说明了10步以内下完全局，enable[]这里代表的是还可以装填的数目
const int path[8][2] = { 0,1,0,-1,1,0,-1,0,1,1,1,-1,-1,1,-1,-1 }; //这里用path[8][2]标识了八个方向

																  //计算黑白的数目并且判断输赢，其中，白赢则定义为1，黑赢则定义为-1,如果是平局则定义为0
int cal()
{
	int i, j, whitecount = 0, blackcount = 0;
	for (i = 0; i<8; i++)
		for (j = 0; j<8; j++)
		{
			if (grid[i][j] == 1) whitecount++;
			else if (grid[i][j] == -1) blackcount++;
			else assert(grid[i][j] == 0);//这个可以不管，只是为了方便调试，向调试器发送的信号                
		}
	if (whitecount>blackcount) return 1;
	else if (whitecount == blackcount) return 0;
	else return -1;
}

int white()
{
	int i, j, k, u, v, loop, temp1, temp2, res = -1, tempres;
	int rem[32][2], remnum;
	int tag = 0;
	//考虑每一个可能的空位，虽然是指数级别的，但是由于在10步以内，空间还是比较小
	for (i = 0; i<num; i++)
		if (enable[i])
		{ 
			//重新标记回来
			enable[i] = 0;
			//获取空位点的x,y的坐标
			u = mark[i][0]; 
			v = mark[i][1];
			grid[u][v] = 1;
			remnum = 0;//统计吃子的个数
					   //朝着八个方向开始搜索
			for (loop = 0; loop<8; loop++)
			{
				//棋子对该位置不超过棋盘，而且，朝向的前面一个格子不为空(否则无法吃掉)
				for (j = u, k = v; j + path[loop][0] >= 0 && j + path[loop][0]<8 && k + path[loop][1] >= 0 && k + path[loop][1]<8 && grid[j + path[loop][0]][k + path[loop][1]];)
				{
					j += path[loop][0];
					k += path[loop][1];
					//判断是否可以吃子,这里我还是很看了一下才看懂的！是这样的，注意到前面那个for循环，最后的执行是一个空的，就是说，如果遇到朝着那个方向不相同
					//的子的话，就直接加过去,直到遇到了一个与之颜色相同的棋子为止。这个时候，还没有完，我们还需要判断一下这个中间是否"夹着"一些棋子，这里可以将
					//只朝着方向移动一格的子和终点比较，如果坐标不同的话，就说明符合条件了，并统计可以吃的子的个数，并最后以remnum作为统计
					if (grid[j][k] == grid[u][v])
					{
						for (temp1 = u + path[loop][0], temp2 = v + path[loop][1]; (temp1 != j || temp2 != k); temp1 += path[loop][0], temp2 += path[loop][1])
						{
							rem[remnum][0] = temp1;
							rem[remnum][1] = temp2;
							remnum++;
						}
						break;
					}
				}
			}
			if (remnum>0)
			{
				for (loop = 0; loop<remnum; loop++)
				{
					j = rem[loop][0];
					k = rem[loop][1];
					//调整每个子的颜色，这也就是我们为什么要设置1表示白子而-1表示黑子了
					grid[j][k] = -grid[j][k];
				}
				tempres = black();
				tag = 1;
				if (tempres>res) res = tempres; //白棋希望博弈值越大越好，如果过小的话，白方不满意
												//这里返回到初始的状态
				for (loop = 0; loop<remnum; loop++)
				{
					j = rem[loop][0];
					k = rem[loop][1];
					grid[j][k] = -grid[j][k];
				}
			}
			grid[u][v] = 0;
			enable[i] = 1;
		}
	if (tag == 0) return cal();
	else return res;
}

int black()
{
	//黑方下棋的规则和白方类似，这里略去一些注释
	int i, j, k, u, v, loop, temp1, temp2, res = 1, tempres;
	int rem[32][2], remnum;
	int tag = 0;
	for (i = 0; i<num; i++)
		if (enable[i])
		{
			enable[i] = 0;
			u = mark[i][0], v = mark[i][1];
			grid[u][v] = -1;
			remnum = 0;
			for (loop = 0; loop<8; loop++)
			{
				for (j = u, k = v; j += path[loop][0] >= 0 && j + path[loop][0]<8 && k + path[loop][1] >= 0 && k + path[loop][1]<8 && grid[j + path[loop][0]][k + path[loop][1]];)
				{
					j += path[loop][0];
					k += path[loop][1];
					if (grid[j][k] == grid[u][v])
					{
						for (temp1 = u + path[loop][0], temp2 = v + path[loop][1]; (temp1 != j || temp2 != k); temp1 += path[loop][0], temp2 += path[loop][1])
						{
							rem[remnum][0] = temp1;
							rem[remnum][1] = temp2;
							remnum++;
						}
						break;
					}
				}
			}
			if (remnum>0)
			{
				for (loop = 0; loop<remnum; loop++)
				{
					j = rem[loop][0];
					k = rem[loop][1];
					grid[j][k] = -grid[j][k];
				}
				tempres = white();
				tag = 1;
				if (tempres<res) res = tempres; //黑方则希望博弈值越小越好，如果过大的话，则会不满意
												//跳回到原来的状态
				for (loop = 0; loop<remnum; loop++)
				{
					j = rem[loop][0];
					k = rem[loop][1];
					grid[j][k] = -grid[j][k];
				}
			}
			grid[u][v] = 0;
			enable[i] = 1;
		}
	if (tag == 0) return cal();
	else return res;
}

int main()
{
	char ch, str[100];
	int i, j;
	for (; ;)//这个，用while(1)可能看起来更舒服一些吧！
	{
		//由于是读str，这里是读一行
		fin >> str;
		if (strcmp(str, "EndOfInput") == 0) break; //读文件结束
		for (i = 0; i<8; i++)
		{
			if (str[i] == 'w') grid[0][i] = 1; //白方置1
			else if (str[i] == 'b') grid[0][i] = -1; //黑方置-1
			else if (str[i] == 'e') grid[0][i] = 0; //空白置0                 
		}
		for (i = 1; i<8; i++)
			for (j = 0; j<8; j++)
			{
				//由于是读ch，这里是一个字符一个字符地读
				fin >> ch;
				if (ch == 'w') grid[i][j] = 1;
				else if (ch == 'b') grid[i][j] = -1;
				else if (ch == 'e') grid[i][j] = 0;
			}
		num = 0;
		for (i = 0; i<8; i++)
			for (j = 0; j<8; j++)
				if (grid[i][j] == 0)
				{
					mark[num][0] = i;
					mark[num][1] = j;
					enable[num] = 1;
					num++;
				}
		cin >> str;
		if (strcmp(str, "WHITE") == 0)
		{
			i = white();
			if (i == 1) fout << "White" << endl;
			else if (i == 0) fout << "Draw" << endl;
			else if (i == -1) fout << "Black" << endl;
			else assert(0); //实在不行，就报错了，这里相当于异常处理                         
		}
		else if (strcmp(str, "Black") == 0)
		{
			i = black();
			if (i == 1) fout << "White" << endl;
			else if (i == 0) fout << "Draw" << endl;
			else if (i == -1) fout << "Black" << endl;
			else assert(0);
		}
		else assert(0);
	}
	return 0;
}