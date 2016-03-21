//41.5
#include "lib_record.h"
#include "route.h"
#include "lib_io.h"
#define MAX_SEARCH_TIME 9

using namespace std;
#define MAX_POINT_NUM 600
#define MP make_pair
typedef pair<int, int> PII;
const int INF = INT_MAX / 2;
int startTime =(int)time(NULL);

struct Edge
{
    int id;
    int u, v, w;
    Edge()
    {
        id=u=v=-1;
        w = INF;
    }

    Edge(char *s)
    {
        char *cur = strtok(s, ",");
        id = atoi(cur);
        cur = strtok(NULL, ",");
        u = atoi(cur);
        cur = strtok(NULL, ",");
        v = atoi(cur);
        cur = strtok(NULL, ",");
        w = atoi(cur);
    }
};

vector<Edge> G[MAX_POINT_NUM];

int startPoint, endPoint;
set<int> mustPoints;
set<int> inPath;
int edge_num;
int inPathVPCnt = 0;


map<PII,Edge> edgeIdMap;

vector<int> ans;
int bestLen = INF;
vector<int> finalAns;
struct Dij
{
    bool vis[MAX_POINT_NUM];
    int dis[MAX_POINT_NUM];
    int pre[MAX_POINT_NUM];
    void run(int cur)
    {
        memset(vis, 0, sizeof vis);
        memset(pre,-1,sizeof pre);
        fill(dis, dis + MAX_POINT_NUM, INF);
        dis[cur] = 0;

        set<PII> que;
        que.insert(MP(0, cur));
        int cnt = 0;
        bool foundEnd = false;
        while (!que.empty())
        {
            PII p = *que.begin();
            que.erase(p);
            int curPoint = p.second;
            if (vis[curPoint])
            {
                continue;
            }
            vis[curPoint] = true;

            if(curPoint == endPoint){ //不能从终点扩展
                foundEnd = true;
                continue;
            }
            if(mustPoints.count(curPoint) != 0){
                ++cnt;
                if(cnt >= mustPoints.size() - inPathVPCnt && foundEnd){ //所有v'都搜过了，剩下就不用了

                    break;
                }
            }

            int n = G[curPoint].size();
            for (int i = 0; i < n; i++)
            {
                int nextPoint = G[curPoint][i].v;
                if (inPath.count(nextPoint) != 0)
                {
                    continue;
                }
                int oldDis = dis[nextPoint];
                int newDis = p.first + G[curPoint][i].w;
                if (newDis < oldDis)
                {
                    if(que.count(MP(oldDis, nextPoint)) != 0)
                    {
                        que.erase(MP(oldDis, nextPoint));
                    }

                    que.insert(MP(newDis, nextPoint));
                    pre[nextPoint] = curPoint;
                    dis[nextPoint] = newDis;
                }
            }
        }
    }

    vector<int> endWith(int t)
    {
        vector<int> result;
        while(pre[t]!=-1)
        {
            result.push_back(pre[t]);
            t = pre[t];
        }
        reverse(result.begin(),result.end());
        return result;
    }
};




void dfs(int cur,int dis)
{

    if(dis >= bestLen )
    {
        return;
    }
    int now = (int)time(NULL);
    int diff = now - startTime;
    //cout<<diff<<endl;
    if(diff > MAX_SEARCH_TIME)
    {
        return ;
    }

    if (cur == endPoint)
    {
        bestLen = dis;
        finalAns = ans;
        return;
    }


    inPath.insert(cur);
    Dij dij;
    dij.run(cur);


    //order by dis
    vector<PII> candidates;
    //todo remove and add
    for(set<int>::iterator i = mustPoints.begin(); i!=mustPoints.end(); ++i)
    {
        if(inPath.count(*i) == 0)
        {
            candidates.push_back(MP(dij.dis[*i],*i));
        }
    }
    if(candidates.size() == 0)
    {
        candidates.push_back(MP(dij.dis[endPoint],endPoint));
    }
    //if(candidates.size() != 0)
    //{
    sort(candidates.begin(),candidates.end());
    inPathVPCnt++;
    for(int i=0; i<candidates.size(); i++)
    {
        vector<int> path;
        int curPoint = candidates[i].second; // next point V' to go
        if(dij.dis[curPoint] == INF)
        {
            continue;
        }
        vector<int> vp = dij.endWith(curPoint);
        int cnt = vp.size();
        for(int i=0; i<cnt; i++)
        {
            inPath.insert(vp[i]);
            ans.push_back(vp[i]);
        }

        dfs(curPoint,dis + dij.dis[curPoint]);

        while(cnt--)
        {
            inPath.erase(ans.back());
            ans.pop_back();
        }
    }
    inPathVPCnt--;
    inPath.erase(cur);
    return;
    //}
    //else
    //{
    //goto end;
    //if(dij.dis[endPoint] != INF)
    //{
    //vector<int> vp = dij.endWith(endPoint);
    //int cnt = vp.siz
    //ans.insert(ans.end(),vp.begin(),vp.end());
    //ans.push_back(endPoint);
    //dfs(endPoint,dis + dij.dis[endPoint]);
    //for(int i=0;i<vp.size()+1;i++){
    //    ans.pop_back();
    //}
    //}
    //return ;
    //}


}


void solve()
{
    dfs(startPoint,0 );

    if(finalAns.size() != 0 )
    {
        finalAns.push_back(endPoint);
        for(int i=0; i<finalAns.size()-1; i++)
        {
            cout<<finalAns[i]<<","<<finalAns[i+1]<<endl;
            //assert(edgeIdMap.count(MP(finalAns[i],finalAns[i+1])) !=0);
            int eid = edgeIdMap[MP(finalAns[i],finalAns[i+1])].id;
            record_result(eid);
            //cout<<ans[i]<<endl;
        }
    }

}

//你要完成的功能总入口
void search_route(char *topo[5000], int edge_num, char *demand)
{

    for (int i = 0; i < edge_num; i++)
    {
        Edge e = Edge(topo[i]);
        //G[e.u].push_back(e);
        if(edgeIdMap.count(MP(e.u,e.v)) == 0 ||edgeIdMap[MP(e.u,e.v)].w > e.w )
            edgeIdMap[MP(e.u,e.v)] = e;
    }
    for(map<PII,Edge>::iterator i = edgeIdMap.begin(); i!=edgeIdMap.end(); i++)
    {
        const Edge& e = i->second;
        G[e.u].push_back(e);
    }


    char *cur = strtok(demand, ",");
    startPoint = atoi(cur);
    cur = strtok(NULL, ",");
    endPoint = atoi(cur);
    cur = strtok(NULL, "|");
    while (cur)
    {
        mustPoints.insert(atoi(cur));
        cur = strtok(NULL, "|");
    }

    ::edge_num = edge_num;
    solve();

}


