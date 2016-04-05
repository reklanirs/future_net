//74.41
#include "lib_record.h"
#include "route.h"
#include "lib_io.h"
#define MAX_SEARCH_TIME 15
using namespace std;
#define MAX_POINT_NUM 700
#define MP make_pair
typedef pair<int, int> PII;
const int INF = INT_MAX / 10;
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
vector<bool> isMustPoint(MAX_POINT_NUM, false);
//set<int> inPath;

vector<bool> inRoad(MAX_POINT_NUM, false);

int edge_num;
int inPathVPCnt = 0;


map<PII,Edge> edgeIdMap;

vector<int> ans;
int bestLen = 1000;
vector<int> finalAns;
struct FastDij
{

    int dis[MAX_POINT_NUM];
    int pre[MAX_POINT_NUM];
    //int vis[MAX_POINT_NUM];
    void run(int cur)
    {

        //cout<<"start DIJ"<<cur<<endl;
        //cout<<inPathVPCnt<<endl;
        priority_queue< PII, vector<PII>, greater<PII> > que;

        memset(pre,-1,sizeof pre);
        // memset(vis,false,sizeof vis);
        fill(dis, dis + MAX_POINT_NUM, INF);

        que.push(MP(0,cur));
        dis[cur] = 0;

        bool visDis = false;
        int visVp = 0;
        while(!que.empty())
        {

            if(visDis && visVp == mustPoints.size() - inPathVPCnt)
            {
                break;
            }
            PII curPoint = que.top();
            que.pop();
            int cp = curPoint.second;
            int cd = curPoint.first;

            if(dis[cp] == cd)
                //if(!vis[cp])
            {
                //vis[cp] = true;
                if(cp == endPoint)
                {
                    visDis = true;
                    continue;
                }
                if(isMustPoint[cp])
                {

                    visVp++;
                    //out<<"got V'"<<cp<<" "<<mustPoints.size()<<" "<<visVp<<endl;
                }
                for(int i=0; i<G[cp].size(); i++)
                {
                    const Edge& e = G[cp][i];
                    if(inRoad[e.v])
                    {
                        continue;
                    }
                    int oDis = dis[e.v];
                    int nDis = cd + e.w;
                    if(nDis < oDis)
                    {
                        dis[e.v] = nDis;
                        pre[e.v] = cp;
                        que.push(MP(nDis,e.v));

                    }

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
    //cout<<"dfs"<<cur<<" "<<dis<<endl;
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
        cout<<"find "<<dis<<endl;
        bestLen = dis;
        finalAns = ans;
        return;
    }



    //Dij dij;
    FastDij dij;
    dij.run(cur);
    if(dij.dis[endPoint] == INF)
    {
        return;
    }

    //order by dis
    vector<PII> candidates;
    bool allCanGo = true;
    //todo remove and add
    for(set<int>::iterator i = mustPoints.begin(); i!=mustPoints.end(); ++i)
    {
        if(inRoad[*i] == false && *i != cur)
        {
            candidates.push_back(MP(dij.dis[*i],*i));
            if(dij.dis[*i] == INF)
            {
                allCanGo = false;
                break;
            }
        }
    }
    if(!allCanGo)
    {
        return;
    }

    if(candidates.size() == 0)
    {

        candidates.push_back(MP(dij.dis[endPoint],endPoint));
    }
    //if(candidates.size() != 0)
    //{
    sort(candidates.begin(),candidates.end());
    inRoad[cur] = true;
    if(cur != startPoint)
        inPathVPCnt++;
    int cSize = candidates.size();
    for(int i=0; i<cSize; i++)
    {
        vector<int> path;
        int curPoint = candidates[i].second; // next point V' to go
        vector<int> vp = dij.endWith(curPoint);
        int cnt = vp.size();
        for(int i=0; i<cnt; i++)
        {
            inRoad[vp[i]] = true;
            ans.push_back(vp[i]);
        }

        dfs(curPoint,dis + dij.dis[curPoint]);

        while(cnt--)
        {
            inRoad[ans.back()] = false;
            ans.pop_back();
        }

    }
    if(cur != startPoint)
        inPathVPCnt--;
    inRoad[cur] = false;
    return;
}

void bruteForce(int cur,int dis)
{
    //cout<<cur<<endl;
    if(dis > bestLen )
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

    if(isMustPoint[cur])
    {
        ++inPathVPCnt;
    }
    else if (cur == endPoint )
    {
        if(inPathVPCnt == mustPoints.size())
        {
            cout<<"find "<<dis<<endl;
            bestLen = dis;
            finalAns = ans;
        }

        return;
    }
    inRoad[cur] = true;
    ans.push_back(cur);
    int n = G[cur].size();
    for(int i=0; i<n; i++)
    {
        Edge e = G[cur][i];
        int nxPoint = e.v;
        if(!inRoad[nxPoint])
        {
            bruteForce(nxPoint,dis + e.w);
        }
    }
    ans.pop_back();
    inRoad[cur] = false;
    if(isMustPoint[cur])
    {
        --inPathVPCnt;
    }

}

void solve()
{
    dfs(startPoint,0 );
    //assert(inPathVPCnt == 0);
    //assert(ans.size() == 0);
    bruteForce(startPoint,0);
    if(finalAns.size() != 0)
    {
        finalAns.push_back(endPoint);
        for(int i=0; i<finalAns.size()-1; i++)
        {
            //cout<<finalAns[i]<<","<<finalAns[i+1]<<endl;
            assert(edgeIdMap.count(MP(finalAns[i],finalAns[i+1])) !=0);
            int eid = edgeIdMap[MP(finalAns[i],finalAns[i+1])].id;
            record_result(eid);
            //cout<<ans[i]<<endl;
        }
    }
    //}

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
        int v = atoi(cur);
        mustPoints.insert(v);
        isMustPoint[v] = true;
        cur = strtok(NULL, "|");
    }

    ::edge_num = edge_num;


    solve();

}


