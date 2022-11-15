/*
从1，2，3，...n个数中，选m个数进行排列组合，输出所有情况
实现方式采用递归，深度搜索
*/
#include<iostream>
using namespace std;
#define MAXN 30
int a[MAXN];
int temp[MAXN];
int tag[MAXN]={0};
int n,m;
 
void permutation(int n, int m, int lev)//排列数
{
	int i;
	if(lev == m)
	{
		for(i=0; i<m; ++i)
			cout<<temp[i]<<" ";
		cout<<endl;
	}
	else
	{
		for(i=0; i<n; ++i)
		{
			if(tag[i] == 0)
			{
				tag[i]=1;
				temp[lev]=a[i];
				permutation(n,m,lev+1);
				tag[i]=0;
			}
		}
	}
 
}
 
void combine(int n, int m, int lev)//组合数
{
	int i;
	if(m == 0)
	{
		for(i=0; i<lev; ++i)
			cout<<temp[i]<<" ";
		cout<<endl;
	}
	else
	{
		for(i=0; i<=n-m; ++i)
		{
			temp[lev++] = n-i;
			combine(n-i-1, m-1, lev);
			lev--;
		}
	}
}
 
int main()
{
	cin>>n>>m;
	int i;
	for(i=0; i<n; ++i)//初始化赋值
		a[i]=i+1;
	//permutation(n, m, 0);
	combine(n,m,0);
	system("pause");
	return 0;
}