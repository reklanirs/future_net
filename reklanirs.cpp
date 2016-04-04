/*
#if 0
#include "lib_record.h"
#include "route.h"
#include "lib_io.h"
#endif
#define MAX_SEARCH_TIME 9
*/
const int K = 1;
#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <cassert>

#include <algorithm>
#include <string>
#include <vector>
#include <set>
#include <map>
#include <unordered_map>
#include <queue>
#include <deque>
#include <stack>
#include <cmath>
#include <numeric>
#include <bitset>

#include <cstdio>
#include <cstring>

using namespace std;

#define rep(i, n) for (int i = 0, _n = (int)(n); i < _n; i++)
#define fer(i, x, n) for (int i = (int)(x), _n = (int)(n); i < _n; i++)
#define rof(i, n, x) for (int i = (int)(n), _x = (int)(x); i-- > _x; )
#define fch(i, x) for (__typeof(x.begin()) i = x.begin(); i != x.end(); i++)
#define sz(x) (int((x).size()))
#define pb push_back
#define mkp make_pair
#define all(X) (X).begin(),(X).end()

#define X first
#define Y second

template<class T> inline void smin(T &a, T b){if(b<a)a=b;}
template<class T> inline void smax(T &a, T b){if(a<b)a=b;}

typedef long long ll;
typedef long double ld;
typedef pair<int, int> pii;
typedef pair<ll,ll> pll;
typedef vector<pii> vii;
typedef vector<int> vi;
typedef vector<vi> vvi;
typedef vector<vii> vvii;


typedef vector<char> VC;
typedef vector<string> VS;
typedef vector<ll> VL;
typedef vector<double> VD;
typedef set<int> SI;
typedef set<pair<int,int> > SII;
typedef set<string> SS;
typedef map<int, int> MII;
typedef map<string, int> MSI;

template<class T> inline void RST(T &A){memset(A, 0, sizeof(A));}
template<class T> inline void FLC(T &A, int x){memset(A, x, sizeof(A));}
template<class T> inline void CLR(T &A){A.clear();}

/** Constant List .. **/ //{
const int dx4[] = {-1, 0, 1, 0};
const int dy4[] = {0, 1, 0, -1};
const int dx8[] = {-1, 0, 1, 0 , -1 , -1 , 1 , 1};
const int dy8[] = {0, 1, 0, -1 , -1 , 1 , -1 , 1};
const int mod = 1000000007;
const int INF = 0x3f3f3f3f;
const double eps = 1e-7;
const double pi = acos(-1.0);
//}

template<class T> inline T min(T a, T b, T c){return min(min(a, b), c);}
template<class T> inline T max(T a, T b, T c){return max(max(a, b), c);}
template<class T> inline T min(T a, T b, T c, T d){return min(min(a, b), min(c, d));}
template<class T> inline T max(T a, T b, T c, T d){return max(max(a, b), max(c, d));}
template<class T> inline T sqr(T a){return a*a;}
template<class T> inline T cub(T a){return a*a*a;}

////////////////////////////////////////////////////////////////////////////////////////////
/**////////////////////////////////////////////////////////////////////////////////////////////
int startTime =(int)time(NULL);

std::vector<std::string> &split(const std::string &s, char delim, std::vector<std::string> &elems) {
    std::stringstream ss(s);
    std::string item;
    while (std::getline(ss, item, delim)) {
        elems.push_back(item);
    }
    return elems;
}

std::vector<std::string> split(const std::string &s, char delim) {
    std::vector<std::string> elems;
    split(s, delim, elems);
    return elems;
}

struct RawEdge
{
    int id,from,to,value;
    RawEdge(){ id=from=to=-1; value = INF; }
    RawEdge(char *s){
        vector<string> tmpv;
        split(s,',',tmpv);
        id = stoi(tmpv[0]); from = stoi(tmpv[1]); to = stoi(tmpv[2]); value = stoi(tmpv[3]);
    }
};
vector<RawEdge> rawedges;
int rawS,rawT;
int rawN,rawM;
set<int> mustPoints;



const int MAX_V = 1400;
int mirrorl2r[MAX_V],mirrorr2l[MAX_V],mirror2Raw[MAX_V];
bool mustPoint[MAX_V];
int superS,superT;
vii raw_mirror;

struct Edge
{
    int rid;
    int to,cap,rev;
    bool isrev;
};
struct Ford
{
    vector<Edge> G[MAX_V];
    bool used[MAX_V];


    int connectl2r[MAX_V],connectr2l[MAX_V];//V' -> mirror V'
    pii headtail[MAX_V]; //
    pii prenxt[MAX_V];
    int curS,curT;
    int forbid1,forbid2;
    set<int> affectS,affectT;

    void AddEdge(int rid, int from, int to, int cap){
        G[from].push_back((Edge){rid, to, cap, (int)G[to].size(),false});
        G[to].push_back((Edge){-1, from, 0, (int)G[from].size()-1,true});
    }
    int findhead(int x){
        if(x == rawS) return x;
        if(connectr2l[mirrorl2r[x]] != mirrorl2r[x]) return findhead(connectr2l[mirrorl2r[x]]);
        else return x;
    }
    int dfs(int v,int t,int f,int pre = -1, int preEisrev = false){
        if(K) printf("\t%d %d %d %d %d\n",v,t,f,pre,preEisrev );
        affectS.insert(headtail[v].X);
        affectT.insert(headtail[v].Y);
        if(v == t){
            //
            curT = pre;
            connectl2r[curS] = curT; connectr2l[curT] = curS;
            affectS.insert(curS);
            affectT.insert(curT);
            return f;
        }
        
        used[v] = true;
        for(int i = 0; i < G[v].size(); i++){
            Edge &e = G[v][i];

            //不能走到禁止节点,也不能掐断流向禁止节点的流
            if(e.to == forbid1 || e.to == forbid2) continue;
            if(headtail[e.to].Y == forbid1 || headtail[e.to].Y == forbid2) continue;
            if(!used[e.to] && e.cap > 0){

                //trigger
                if(e.to == rawS || mirrorl2r[e.to]!=-1){

                    curS = e.to;
                    forbid1 = mirrorl2r[curS];
                    forbid2 = mirrorl2r[findhead(curS)];
                }

                //掐断之前一条流. 需要更新curS,forbid
                if(e.isrev){
                    int precurS = curS;
                    curS = headtail[e.to].X;
                    forbid1 = mirrorl2r[curS];
                    forbid2 = mirrorl2r[findhead(curS)];

                    int d = dfs(e.to, t, min(f, e.cap),v,true);
                    if(d > 0){
                        if(preEisrev) prenxt[v] = mkp(-1,-1);
                        else prenxt[v].X = pre;

                        e.cap -= d;
                        G[e.to][e.rev].cap += d;
                        return d;
                    }else{
                        curS = precurS;
                        forbid1 = mirrorl2r[curS];
                        forbid2 = mirrorl2r[findhead(curS)];
                    }
                }else{
                    int d = dfs(e.to, t, min(f, e.cap),v, false);
                    if(d > 0){
                        prenxt[v].Y = e.to;
                        if(!preEisrev) prenxt[v].X = pre;

                        e.cap -= d;
                        G[e.to][e.rev].cap += d;
                        return d;
                    }
                }

                //reset trigger ?
                if(e.to == rawS || mirrorl2r[e.to]!=-1){
                    curS = e.to;
                    forbid1 = mirrorl2r[curS];
                    forbid2 = mirrorl2r[findhead(curS)];
                }
            }
        }
        return 0;
    }
    int MaxFlow(int s,int t){
        int flow = 0;
        rep(i,MAX_V) connectl2r[i]=connectr2l[i]=i,headtail[i]=prenxt[i]=mkp(-1,-1);
        while(true){
            if(K) printf("%d\n", flow);
            memset(used, 0, sizeof used);
            affectS.clear();
            affectT.clear();
            int f = dfs(s, t, INF);
            if(f == 0) return flow;
            flow += f;

            for(SI::iterator ite = affectS.begin(); ite!=affectS.end(); ite++){
                if(*ite == -1) continue;
                int tmp = *ite;
                while(tmp != superT){
                    headtail[tmp].X = *ite;
                    tmp = prenxt[tmp].Y;
                }
            }
            for(SI::iterator ite = affectT.begin(); ite!=affectT.end(); ite++){
                if(*ite == -1) continue;
                int tmp = *ite;
                while(tmp != superS){
                    headtail[tmp].Y = *ite;
                    tmp = prenxt[tmp].X;
                }
            }
        }
    }
}ford;


void makeGraph(){
    raw_mirror.clear();
    int indx = rawN;
    memset(mirrorl2r, -1, sizeof mirrorl2r);
    memset(mirrorr2l, -1, sizeof mirrorr2l);
    memset(mirror2Raw, -1, sizeof mirror2Raw);
    rep(i,rawN){
        if(i == rawS || i == rawT) continue;
        mirrorl2r[i] = i+rawN; mirrorr2l[i+rawN] = i;
        mirror2Raw[i]=mirror2Raw[i+rawN]=i;
        ford.AddEdge(-1, i, i+rawN, 1);
    }
    superS = 2*rawN;
    superT = 2*rawN+1;
    for(SI::iterator ite = mustPoints.begin(); ite!=mustPoints.end(); ite++){
        raw_mirror.pb(mkp(*ite,mirrorl2r[*ite]));
    }
    // raw_mirror.pb(mkp(rawS,rawT));

    ford.AddEdge(-1,superS,rawS,1);
    rep(i,raw_mirror.size()){
        ford.AddEdge(-1,superS,raw_mirror[i].X,1);
    }
    //原先点中,V'到V'的点需要改动到V'到mirror V'上去
    rep(i,rawedges.size()){
        RawEdge& e = rawedges[i];
        if(mustPoint[e.from] && mustPoint[e.to]){
            ford.AddEdge(e.id, e.from, mirrorl2r[e.to], 1);
        }else if(mustPoint[e.from]){
            ford.AddEdge(e.id,e.from,e.to,1);
            // ford.AddEdge(e.id,mirrorl2r[e.from],e.to,1);  //右侧点不需要superT以外的出度
        }else if(mustPoint[e.to]){
            // ford.AddEdge(e.id,e.from,e.to,1); //左侧点不需要superS以外的入度
            ford.AddEdge(e.id,mirrorl2r[e.from],mirrorl2r[e.to],1);
        }else if(e.from == rawS && e.to == rawT){
            if(mustPoints.size() == 0) ford.AddEdge(e.id,e.from,e.to,1);
        }else if(e.to == rawS || e.from == rawT){
            //do nothing
        }else{
            ford.AddEdge(e.id,mirrorl2r[e.from],e.to,1);
        }
    }
    rep(i,raw_mirror.size()){
        ford.AddEdge(-1,raw_mirror[i].Y,superT,1);
    }
    ford.AddEdge(-1,rawT,superT,1);
}

void solve(){
    if(K) puts("solve");
    makeGraph();
    if(K) puts("makeGraph end");
    int f = ford.MaxFlow(superS,superT);
    printf("MaxFlow:%d\n",f);
    if(f != mustPoints.size() + 1){
        pus("Error");
        return;
    }
    //output answer
}


//你要完成的功能总入口
void search_route(char *topo[5000], int edge_num, char *demand){
    if(K) puts("search_route");
    rawN = 0;
    rawedges.clear();
    for (int i = 0; i < edge_num; i++){
        RawEdge e = RawEdge(topo[i]);
        smax(rawN, e.from); smax(rawN, e.to);
        rawedges.pb(e);
    }
    rawN += 1;
    rawM = rawedges.size();

    vector<string> v = split(demand,',');
    rawS = stoi(v[0]);
    rawT = stoi(v[1]);
    v = split(v[v.size()-1], '|');
    memset(mustPoint, 0, sizeof mustPoint);
    rep(i,v.size()){
        int tmp = stoi(v[i]);
        mustPoints.insert(tmp);
        mustPoint[tmp] = true;
    }
    mustPoints.erase(rawS); mustPoint[rawS] = false;
    mustPoints.erase(rawT); mustPoint[rawT] = false;
    solve();
}

char* topo[5000];
char demand[5000];
int main(int argc, char *argv[]){

    freopen(argv[1],"r",stdin);
    rep(i,5000) topo[i] = new char[500];
    int edge_num = 0;
    while(~scanf("\n%s\n",topo[edge_num++]));
    while(strlen(topo[edge_num-1]) <= 1) edge_num--;
    freopen(argv[2],"r",stdin);
    scanf("%s\n",demand);
    if(K) puts(demand);
    search_route(topo,edge_num,demand);
    return 0;
}