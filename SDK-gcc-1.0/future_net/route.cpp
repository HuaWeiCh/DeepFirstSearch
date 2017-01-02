#include "route.h"
#include "lib_io.h"
//#include "lp_lib.h"
#include <stdio.h>

#define debug 1

class line
{
    public:
    int id;
    int start;
    int end;
    int length;
};
int	 pNum;
int  lNum;
bool *ifVisited;
bool *ifmustP;
bool *ifmustPvisited;
int  startP;
int  endP;
int  mustNum;
int  mustP[51];
line l[602][8];
int  common_start_num[602]={0};
int  path[4801];
int  path_end[4801];
int  successPath[4801];
int  plength=260;//65535;				//>600*20=12000
int	 nowLength=0;
bool success;
int  pathPNum;
int  lastMustPtoEnd_deep=0;
int  ifmustPvisited_num=0;				//记录访问过的mustP的个数

extern int len;
extern char *Result_file;
//********************************************dfs函数******************************************//
void dfs(int cur,int deep,int EndP,bool ifrecord)	//cur搜索深度char
{
    if(cur>deep)	return;

    for(int i=0;i<common_start_num[path_end[cur-1]];i++)
    {
        if(ifVisited[l[path_end[cur-1]][i].end]==false)
        {
            if(l[path_end[cur-1]][i].end==EndP)
            {
//*******************当只要求从一点开始到一点结束**************
            	if(false==ifrecord)
            	{
					path_end[cur]=l[path_end[cur-1]][i].end;
					path[cur]	 =l[path_end[cur-1]][i].id;
					nowLength	+=l[path_end[cur-1]][i].length;
					pathPNum=cur;
					success=true;
                    return;
            	}
//*********************************************************
                if(cur>=mustNum)		//经过顶点数小于必过点数，必不符合要求 
                {
                    for(int j=0;j<mustNum;j++)	//必过点检测
                    {
                        if(ifVisited[mustP[j]]==false)
                        {
                            goto fall;
                        }
                    }

		    path_end[cur]=l[path_end[cur-1]][i].end;
		    path[cur]	 =l[path_end[cur-1]][i].id;
		    nowLength	+=l[path_end[cur-1]][i].length;

                    if(nowLength<plength)
                    {
                        plength=nowLength;
                        pathPNum=cur;
                        len=0;
                        for(int j=1;j<=cur;j++)
                        {
#if debug
                            successPath[j]=path[j];
#endif
                            //record_result(path[j]);
                        }
                        write_result(Result_file);
						success=true;
                    }
                    nowLength-=l[path_end[cur-1]][i].length;//释放权值,为继续搜索准备
                }
                else
                {
                    fall:
                    continue;
                }
            }
            else
            {
            	path_end[cur]=l[path_end[cur-1]][i].end;
                path[cur]	 =l[path_end[cur-1]][i].id;
                nowLength	+=l[path_end[cur-1]][i].length;
                ifVisited[path_end[cur]]=true;
                if(nowLength<plength)	dfs(cur+1,deep,EndP,ifrecord);
                if(false==ifrecord)	if(true==success)	return;//不记录的情况下直接返回
                ifVisited[path_end[cur]]=false;				//道路不通，返回过程中释放此点
                nowLength	-=l[path_end[cur-1]][i].length; //道路不通，返回过程中释放权值
            }
        }
        continue;
    }
}
//************************************DefinetheSearchRange**********************************//
int path_END[600]={0};
void DefinetheSearchRange(int cur,int deep)
{
	if(cur>deep)	return;
	for(int i=0;i<common_start_num[path_END[cur-1]];i++)
	{
		if(true==ifmustP[l[path_END[cur-1]][i].end])
		{
			//printf("here!\n");
			return;
		}
		else
		{
			path_end[cur]=l[path_END[cur-1]][i].end;
			ifVisited[path_END[cur]]=false;
			DefinetheSearchRange(cur+1,deep);
		}
	}
}
//*************************************GreedForOneAnswer***********************************//
void GreedForOneAnswer(int cur,int deep)
{
	int define_search_num=0;
	int temp_length;
	if(cur>deep)	return;
//****************start开始,搜索任一必过点*********************
    for(int i=0;i<common_start_num[path_end[cur-1]];i++)
    {
        if(false==ifVisited[l[path_end[cur-1]][i].end])
        {//printf("here!\n");
			if(true==ifmustP[l[path_end[cur-1]][i].end])
			{
				printf("here!\n");
				if(false==ifmustPvisited[l[path_end[cur-1]][i].end])	ifmustPvisited_num++;
				ifmustPvisited[l[path_end[cur-1]][i].end]=true;
            	path_end[cur]=l[path_end[cur-1]][i].end;
				path[cur]	 =l[path_end[cur-1]][i].id;
				nowLength	+=l[path_end[cur-1]][i].length;
				temp_length	 =nowLength;
				ifVisited[path_end[cur]]=true;
//********************************待完成***********************************
				for(int m=0;m<=20;m++)
				{
					define_search_num++;
					for(int j=0;j<mustNum;j++)
					{
						if(path_end[cur] != mustP[i])
						{
							path_end[4780]=mustP[i];
							dfs(4781,4800,endP,false);
							if(true==success)
							{
								lastMustPtoEnd_deep=pathPNum-pNum+21;
								success=false;
	//*********************定义搜索范围**************************
								for(int ii=0;ii<=pNum;ii++)	ifVisited[ii]=true;
								path_END[0]=path_end[cur];
								DefinetheSearchRange(1,define_search_num);
						
								for(int ii=1;ii<=cur;ii++)	ifVisited[path_end[cur]]=true;
								for(int ii=4781;ii<lastMustPtoEnd_deep+4781;ii++)
									ifVisited[path_end[cur]]=true;
	//********************************************************
								dfs(cur+1,pNum-21,mustP[i],false);
								if(true==success)
								{
									for(int k=0;k<lastMustPtoEnd_deep;k++)
									{
										path[pathPNum+k+1]=path[4781+k];
									}
									pathPNum+=lastMustPtoEnd_deep;
									plength=nowLength;
	//********************************************************
					                len=0;
					                for(int k=1;k<=pathPNum;k++)
					                {
	#if debug
					                    successPath[k]=path[k];
	#endif
					                    //record_result(path[k]);
					                }
					                write_result(Result_file);
	//********************************************************
									return;
								}
								else
								{	//不成功,则恢复原有标记点
									for(int ii=0;ii<=pNum;ii++)	ifVisited[ii]=true;
									//DefinetheSearchRange();
						
									for(int ii=1;ii<=cur;ii++)	ifVisited[path_end[cur]]=true;
									//for(int ii=4781;ii<lastMustPtoEnd_deep+4781;ii++)
									//	ifVisited[path_end[cur]]=true;
									//for(int v=pathPNum;v>cur+1;v--)	//释放必过点检测过程中的标记
									//	ifVisited[path_end[v]]=false;
								}
							}
							else
							{
								lastMustPtoEnd_deep=pathPNum-pNum+21;
							}
							for(int u=4781;u<4781+lastMustPtoEnd_deep;u++)
								ifVisited[path_end[u]]=false;	//不成功，释放必过点到结尾过程中的标记
					
							//需要考虑标记的返回！！！
							//lastMustPtoEnd_deep--;
							//success=false;
							nowLength=temp_length;				//权值返回到起始到比过点的搜索截止处
						}
					}
				}
//*************************************************************************             
            }
            else
            {
            	path_end[cur]=l[path_end[cur-1]][i].end;
                path[cur]	 =l[path_end[cur-1]][i].id;
                nowLength	+=l[path_end[cur-1]][i].length; 
                ifVisited[path_end[cur]]=true;
                GreedForOneAnswer(cur,deep);
                if(true==success)	return;			//成功搜获一个解即退出
                ifVisited[path_end[cur]]=false;			//道路不通，返回过程中释放此点
                nowLength	-=l[path_end[cur-1]][i].length; //道路不通，返回过程中释放权值
            }
        }
        continue;
    }
}
//*****************************************CatchData***************************************//
void CatchData(char *graph[5000], int edge_num, char *condition)
{
//******************************读取有向图********************************************
	int id,start,end,length;
	lNum=edge_num;
	for(int i=0;i<edge_num;i++)
	{
		sscanf(graph[i],"%d,%d,%d,%d",&id,&start,&end,&length);
		for(int j=0;j<common_start_num[start];j++)	//去重复，并保留权值最小
		{
			if(l[start][j].end==end)
			{
				if(length<l[start][j].length)
				{
					l[start][j].id=id;
					l[start][j].length=length;
				}
				goto repeat;
			}
		}
		l[start][common_start_num[start]].id	=id;
		l[start][common_start_num[start]].start	=start;
		l[start][common_start_num[start]].end	=end;
		l[start][common_start_num[start]].length=length;
		common_start_num[start]++;

		if(pNum<start)						//寻点数
            		pNum=start;
        	if(pNum<end)
            		pNum=start;
        	repeat:	;
	}
	lNum=edge_num;							//记录线数

#if debug
	for(int i=0;i<pNum;i++)	printf("The common_start_num is:%d\n",common_start_num[i]);	
#endif
//**********************************读取必过点****************************************
	ifmustP=new bool[pNum+1]; 				//根据顶点数初始化标志长度
	for(int i=0;i<=pNum;i++)				//初始化顶点标记
	{
        	ifmustP[i]=false;
    	}

	int temp0=0;							//起始点
	int temp1=0;
	temp0=*condition;
	while((temp0>=48)&&(temp0<=57))
	{
		temp1=temp1*10+temp0-48;
		condition++;
		temp0=*condition;
	}
	startP=temp1;
//***************************************
	temp1=0;								//终止点
	condition++;
	temp0=*condition;
	while((temp0>=48)&&(temp0<=57))
	{
		temp1=temp1*10+temp0-48;
		condition++;
		temp0=*condition;
	}
	endP=temp1;
//***************************************
	condition++;							//必过点
	mustNum=0;
	temp0=*condition;
	while(10!=temp0)
	{
		while((temp0>=48)&&(temp0<=57))
		{
			mustP[mustNum]=mustP[mustNum]*10+temp0-48;
			condition++;
			temp0=*condition;
			if((124!=temp0)&&((temp0<48)||(temp0>57)))	goto theEnd;
		}
		condition++;
		temp0=*condition;
		ifmustP[mustP[mustNum]]=true;
		mustNum++;
	}
	theEnd:
		mustNum++;

#if debug
	printf("\n");
	printf("The mustNum is:%d\n",mustNum);
	for(int i=0;i<mustNum;i++)	printf("The mustP is:%d\n",mustP[i]);
	
	printf("\n");
	printf("The pNum is %d\n",pNum);
	printf("The lNum is %d\n",lNum);
	printf("\n");
	printf("The startP is %d\n",startP);
	printf("The endP is %d\n",endP);
#endif
}
//**************************************refresh_order**************************************//
void refresh_order(void)//提前带必过点的路径
{
	int temNum;
	int id,start,end,length;
	
	for(int i=0;i<pNum;i++)
	{
		temNum=0;
		for(int j=common_start_num[i];j>temNum;)
		{
			if(j>0)j--;
			while(true==ifmustP[l[i][temNum].end])
			{
				if(temNum<(common_start_num[i]-1))	temNum++;
				if(temNum==(common_start_num[i]-1))	break;
			}
			if((true==ifmustP[l[i][j].end])&&(temNum<j))
			{
				id		=l[i][temNum].id;
				start	=l[i][temNum].start;
				end		=l[i][temNum].end;
				length	=l[i][temNum].length;

				l[i][temNum].id		=l[i][j].id;
				l[i][temNum].start	=l[i][j].start;
				l[i][temNum].end	=l[i][j].end;
				l[i][temNum].length	=l[i][j].length;

				l[i][j].id		=id;
				l[i][j].start	=start;
				l[i][j].end		=end;
				l[i][j].length	=length;
				if(temNum<(common_start_num[i]-1))	temNum++;
			}
		}
	}

#if debug
	for(int i=0;i<=pNum;i++)
	for(int j=0;j<common_start_num[i];j++)
	printf("The line is:%4d %4d %4d %4d\n",l[i][j].id,l[i][j].start,l[i][j].end,l[i][j].length);
#endif
}
//***************************************search_route**************************************//
//你要完成的功能总入口---相比于main函数
void search_route(char *Graph[5000], int Edge_num, char *Condition)
{
	CatchData(Graph,Edge_num,Condition);			//读取文件中数据并规律存储

    	//初始化数据
	ifVisited=new bool[pNum+1]; 				//根据顶点数初始化标志长度
	for(int i=0;i<=pNum;i++)				//初始化顶点标记
	{
		ifVisited[i]=false;
	}
	ifmustPvisited=new bool[pNum+1];			//初始化标志,用于记录mustP是否检索过
	for(int i=0;i<mustNum;i++)				//初始化标记
	{
		ifmustPvisited[mustP[i]]=false;
	}

	ifVisited[startP]=true;
	path_end[0]=startP;					//索引起始线
	pathPNum=0;
	success=false;
    
	refresh_order();					//重新排序，将含必过点的 有向线 提前
	
	GreedForOneAnswer(1,20);
	//dfs(1,pNum,endP,true);  				//深度搜索&&&

#if debug
	if(success==true)					//调试时的结果输出
	{
		printf("plength is %d\n",plength);		//打印权值
		printf("Path is :");				//打印最后路径
		for(int i=1;i<=pathPNum;i++)
			printf("%4d",successPath[i]);
		printf("\n");
	}
	else
	{
		printf("Path is : NA!\n");
	}
#endif
}
//*******************************************The end!**************************************//
