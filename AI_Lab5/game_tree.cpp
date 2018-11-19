#include <fstream> //�ļ������������
#include <string>
#include <assert.h> //���ԣ������Ҫ��Ϊ�˵��Եķ���
#include <iostream>
using namespace std;

ifstream fin("stone.in"); //�ļ�����
ofstream fout("stone.out"); //�ļ����

int grid[8][8]; //�����������̣����е�grid�����������˼
int num, mark[10][2], enable[10]; //����������˵����10����������ȫ�֣�enable[]���������ǻ�����װ�����Ŀ
const int path[8][2] = { 0,1,0,-1,1,0,-1,0,1,1,1,-1,-1,1,-1,-1 }; //������path[8][2]��ʶ�˰˸�����

																  //����ڰ׵���Ŀ�����ж���Ӯ�����У���Ӯ����Ϊ1����Ӯ����Ϊ-1,�����ƽ������Ϊ0
int cal()
{
	int i, j, whitecount = 0, blackcount = 0;
	for (i = 0; i<8; i++)
		for (j = 0; j<8; j++)
		{
			if (grid[i][j] == 1) whitecount++;
			else if (grid[i][j] == -1) blackcount++;
			else assert(grid[i][j] == 0);//������Բ��ܣ�ֻ��Ϊ�˷�����ԣ�����������͵��ź�                
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
	//����ÿһ�����ܵĿ�λ����Ȼ��ָ������ģ�����������10�����ڣ��ռ仹�ǱȽ�С
	for (i = 0; i<num; i++)
		if (enable[i])
		{ 
			//���±�ǻ���
			enable[i] = 0;
			//��ȡ��λ���x,y������
			u = mark[i][0]; 
			v = mark[i][1];
			grid[u][v] = 1;
			remnum = 0;//ͳ�Ƴ��ӵĸ���
					   //���Ű˸�����ʼ����
			for (loop = 0; loop<8; loop++)
			{
				//���ӶԸ�λ�ò��������̣����ң������ǰ��һ�����Ӳ�Ϊ��(�����޷��Ե�)
				for (j = u, k = v; j + path[loop][0] >= 0 && j + path[loop][0]<8 && k + path[loop][1] >= 0 && k + path[loop][1]<8 && grid[j + path[loop][0]][k + path[loop][1]];)
				{
					j += path[loop][0];
					k += path[loop][1];
					//�ж��Ƿ���Գ���,�����һ��Ǻܿ���һ�²ſ����ģ��������ģ�ע�⵽ǰ���Ǹ�forѭ��������ִ����һ���յģ�����˵��������������Ǹ�������ͬ
					//���ӵĻ�����ֱ�Ӽӹ�ȥ,ֱ��������һ����֮��ɫ��ͬ������Ϊֹ�����ʱ�򣬻�û���꣬���ǻ���Ҫ�ж�һ������м��Ƿ�"����"һЩ���ӣ�������Խ�
					//ֻ���ŷ����ƶ�һ����Ӻ��յ�Ƚϣ�������겻ͬ�Ļ�����˵�����������ˣ���ͳ�ƿ��ԳԵ��ӵĸ������������remnum��Ϊͳ��
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
					//����ÿ���ӵ���ɫ����Ҳ��������ΪʲôҪ����1��ʾ���Ӷ�-1��ʾ������
					grid[j][k] = -grid[j][k];
				}
				tempres = black();
				tag = 1;
				if (tempres>res) res = tempres; //����ϣ������ֵԽ��Խ�ã������С�Ļ����׷�������
												//���ﷵ�ص���ʼ��״̬
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
	//�ڷ�����Ĺ���Ͱ׷����ƣ�������ȥһЩע��
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
				if (tempres<res) res = tempres; //�ڷ���ϣ������ֵԽСԽ�ã��������Ļ�����᲻����
												//���ص�ԭ����״̬
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
	for (; ;)//�������while(1)���ܿ����������һЩ�ɣ�
	{
		//�����Ƕ�str�������Ƕ�һ��
		fin >> str;
		if (strcmp(str, "EndOfInput") == 0) break; //���ļ�����
		for (i = 0; i<8; i++)
		{
			if (str[i] == 'w') grid[0][i] = 1; //�׷���1
			else if (str[i] == 'b') grid[0][i] = -1; //�ڷ���-1
			else if (str[i] == 'e') grid[0][i] = 0; //�հ���0                 
		}
		for (i = 1; i<8; i++)
			for (j = 0; j<8; j++)
			{
				//�����Ƕ�ch��������һ���ַ�һ���ַ��ض�
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
			else assert(0); //ʵ�ڲ��У��ͱ����ˣ������൱���쳣����                         
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