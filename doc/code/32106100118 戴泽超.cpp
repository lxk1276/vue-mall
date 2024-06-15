#include<iostream>
#include<stdio.h>
#include<string>
#include<string.h>
#include<cmath>
#include<ctime>
#include<stdlib.h>
using namespace std;
int seed = (int)time(0);//�����������
int cityposition[30][2] = { {87,7},{91,38},{83,46},{71,44},{64,60},{68,58},{83,69},{87,76},{74,78},{71,71},{58,69},{54,62},{51,67},{37,84},{41,94},{2,99},{7,64},{22,60},{25,62},{18,54},{4,50},{13,40},{18,40},{24,42},{25,38},{41,26},{45,21},{44,35},{58,35},{62,32} };
#define CITY_NUM 30//��������
#define ANT_NUM 100//��Ⱥ����
#define TMAC 1000//����������
#define ROU 0.5//����С
#define ALPHA 1//��Ϣ����Ҫ�̶ȵĲ���
#define BETA 4//����ʽ������Ҫ�̶ȵĲ���
#define Q 100//��Ϣ�ز�������
#define maxn 100
#define inf 0x3f3f3f3f
double dis[maxn][maxn];//�������
double info[maxn][maxn];//��Ϣ�ؾ���
bool vis[CITY_NUM][CITY_NUM];//��Ǿ���

//����ָ����Χ�ڵ��������
int rnd(int low, int upper)
{
    return low + (upper - low) * rand() / (RAND_MAX + 1);//rand()/(RAND_MAX+1)Ϊ0-1�������
}

//����ָ����Χ�ڵ����������
double rnd(double low, double upper)
{
    double temp = rand() / ((double)RAND_MAX + 1.0);//rand()/(RAND_MAX+1)Ϊ0-1�������
    return low + temp * (upper - low);
}

struct Ant//���Ͻṹ��
{
    int path[CITY_NUM];//���������ߵ�·��
    double length1;//·���ܳ���
    bool vis[CITY_NUM];//����߹��ĳ���
    int current;//��ǰ����
    int moved_cnt;//���߳�������

    //��ʼ��
    void init()
    { 
        memset(vis, false, sizeof(vis));//��ʼ��������visΪҪ�������ڴ�飬falseΪ���õ�ֵ��sizeof(vis)Ϊ���ø�ֵ���ַ���
        length1 = 0;//·���ܳ���Ϊ0
        current = rnd(0, CITY_NUM);//�������
        path[0] = current;//�߹���·��ֻ����current
        vis[current] = true;//����߹��ĳ���currentΪ��
        moved_cnt = 1;//���߳�������Ϊ1
    }

    //ѡ����һ������
    int choose()
    {
        int select_city = -1;//��ѡ���б��
        double sum = 0;//����Ϣ��
        double posb[CITY_NUM];//����ÿ�����б�ѡ�еĸ���
        for (int i = 0; i < CITY_NUM; i++)
        {
            if (!vis[i])
            {
                posb[i] = pow(info[current][i], ALPHA) * pow(1.0 / dis[current][i], BETA);//����ÿ�����б�ѡ�еĸ���
                sum = sum + posb[i];
            }
            else
            {
                posb[i] = 0;
            }
        }
        //��������ѡ��
        double temp = 0;
        if (sum > 0)//�ܵ���Ϣ�ش���0
        {
            temp = rnd(0.0, sum);//ȡ�������
            for (int i = 0; i < CITY_NUM; i++)
            {
                if (!vis[i])
                {
                    temp = temp - posb[i];//���Ҿ����iֵ��ѡ�����
                    if (temp < 0.0)
                    {
                        select_city = i;
                        break;
                    }
                }
            }
        }
        else //��Ϣ��̫�پ�����Ҹ�û�߹��ĳ���
        {
            for (int i = 0; i < CITY_NUM; i++)
            {
                if (!vis[i])
                {
                    select_city = i;
                    break;
                }
            }
        }
        return select_city;
    }

    //���ϵ��ƶ�
    void move()
    {
        int nex_cityno = choose();//��һ�����д���nex_cityno
        path[moved_cnt] = nex_cityno;
        vis[nex_cityno] = true;
        length1 = length1 + dis[current][nex_cityno];
        current = nex_cityno;
        moved_cnt++;
    }

    //���Ͻ���һ�ε���
    void search()
    {
        init();
        while (moved_cnt < CITY_NUM)
        {
            move();
        }
        //�ص�ԭ���ĳ���
        length1 = length1 + dis[path[CITY_NUM - 1]][path[0]];
    }
};

struct TSP//��Ⱥ�㷨
{
    Ant ants[ANT_NUM];//������Ⱥ
    Ant ant_best;//����·������

    void Init()
    {
        //��ʼ��Ϊ���ֵ
        ant_best.length1 = inf;
        //�����������м����
        for (int i = 0; i < CITY_NUM; i++)
        {
            for (int j = 0; j < CITY_NUM; j++)
            {
                info[i][j] = 1.0;
                double temp1 = double(cityposition[j][0]) - double(cityposition[i][0]);
                double temp2 = double(cityposition[j][1]) - double(cityposition[i][1]);
                dis[i][j] = sqrt(temp1 * temp1 + temp2 * temp2);//���빫ʽ
            }
        }
    }

    //������Ϣ��
    void updateinfo()
    {
        double temp_info[CITY_NUM][CITY_NUM];
        memset(temp_info, 0, sizeof(temp_info));
        int u, v;
        for (int i = 0; i < ANT_NUM; i++) //����ÿһֻ����
        {
            for (int j = 1; j < CITY_NUM; j++)
            {
                u = ants[i].path[j - 1];
                v = ants[i].path[j];
                temp_info[u][v] = temp_info[u][v] + Q / ants[i].length1;
                temp_info[v][u] = temp_info[u][v];
            }
            //�����кͿ�ʼ����֮�����Ϣ��
            u = ants[i].path[0];
            temp_info[u][v] = temp_info[u][v] + Q / ants[i].length1;
            temp_info[v][u] = temp_info[u][v];
        }
        for (int i = 0; i < CITY_NUM; i++)
        {
            for (int j = 0; j < CITY_NUM; j++)
            {
                info[i][j] = info[i][j] * ROU + temp_info[i][j];
            }
        }
    }
    void Search()
    {
        //����TMAC��
        for (int i = 0; i < TMAC; i++)
        {
            //�������϶�����һ�α���
            for (int j = 0; j < ANT_NUM; j++)
            {
                ants[j].search();
            }
            //�����������ϱ�������ѽ��
            for (int j = 0; j < ANT_NUM; j++)
            {
                if (ant_best.length1 > ants[j].length1)
                {
                    ant_best = ants[j];
                }
            }
            updateinfo();
            //printf("��%d�ε�������·��������%lf\n", i, ant_best.length1);
            cout << "��" << i << "�ε�������·��������" << ant_best.length1 << endl;
            cout << endl;
        }
    }
};
int main()
{
    srand(seed);
    TSP tsp;
    tsp.Init();
    tsp.Search();
    //printf("���·������\n");
    cout << "���·�����£�" << endl;
    for (int i = 0; i < CITY_NUM; i++)
    {
        //printf("%d", tsp.ant_best.path[i]);
        cout << tsp.ant_best.path[i];
        if (i < CITY_NUM - 1)
            cout << "->";
        else
            cout << endl;
    }
    return 0;
}
