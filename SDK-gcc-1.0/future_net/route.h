#ifndef __ROUTE_H__
#define __ROUTE_H__

void refresh_order(void);
void GreedForOneAnswer(int cur,int deep);
void DefinetheSearchRange(int cur,int deep);
void dfs(int cur,int deep,int EndP,bool ifrecord);
void CatchData(char *graph[5000], int edge_num, char *condition);
void search_route(char *graph[5000], int edge_num, char *condition);

#endif
