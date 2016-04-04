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


const int MAX_V = 700;
struct Edge
{
    int rid;
    int to,cap,rev;
};
struct Ford
{
    vector<Edge> G[MAX_V];
    bool used[MAX_V];
    void AddEdge(int rid, int from, int to, int cap){
        G[from].push_back((Edge){rid, to, cap, (int)G[to].size()});
        G[to].push_back((Edge){-1, from, 0, (int)G[from].size()-1});
    }
    int dfs(int v,int t,int f){
        if(v == t) return f;
        used[v] = true;
        for(int i = 0; i < G[v].size(); i++){
            Edge &e = G[v][i];
            if(!used[e.to] && e.cap > 0){
                int d = dfs(e.to, t, min(f, e.cap));
                if(d > 0){
                    e.cap -= d;
                    G[e.to][e.rev].cap += d;
                    return d;
                }
            }
        }
        return 0;
    }
    int MaxFlow(int s,int t){
        int flow = 0;
        while(true){
            memset(used, 0, sizeof used);
            int f = dfs(s, t, INF);
            if(f == 0) return flow;
            flow += f;
        }
    }
}ford;

int mirrorl2r[MAX_V],mirrorr2l[MAX_V];
int superS,superT;
vii raw_mirror;
void makeGraph(){
    raw_mirror.clear();
    int indx = rawN;
    memset(mirrorl2r, -1, sizeof mirrorl2r);
    memset(mirrorr2l, -1, sizeof mirrorr2l);
    for(SI::iterator ite = mustPoints.begin(); ite!=mustPoints.end(); ite++){
        mirrorl2r[*ite] = indx++;
        mirrorr2l[indx-1] = *ite;
        raw_mirror.pb(mkp(*ite,indx-1));
    }
    superS = indx++;
    superT = indx++;
    // raw_mirror.pb(mkp(superS,superT));

    ford.AddEdge(-1,superS,rawS,1);
    rep(i,raw_mirror.size()){
        ford.AddEdge(-1,superS,raw_mirror[i].X,1);
    }
    //原先点中,V'到V'的点需要改动到V'到mirror V'上去
    rep(i,rawedges.size()){
        RawEdge& e = rawedges[i];
        if(mirrorl2r[e.from]!=-1 && mirrorl2r[e.to]!=-1){
            ford.AddEdge(e.id, e.from, mirrorl2r[e.to], 1);
        }else if(mirrorl2r[e.from]!=-1){
            ford.AddEdge(e.id,e.from,e.to,1);
            // ford.AddEdge(e.id,mirrorl2r[e.from],e.to,1);  //右侧点不需要superT以外的出度
        }else if(mirrorl2r[e.to]!=-1){
            // ford.AddEdge(e.id,e.from,e.to,1); //左侧点不需要superS以外的入度
            ford.AddEdge(e.id,e.from,mirrorl2r[e.to],1);
        }else if(e.from == rawS && e.to == rawT){
            if(mustPoints.size() == 0) ford.AddEdge(e.id,e.from,e.to,1);
        }else if(e.to == rawS || e.from == rawT){
            //do nothing
        }else{
            ford.AddEdge(e.id,e.from,e.to,1);
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
    printf("%d\n",ford.MaxFlow(superS,superT));
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
    rep(i,v.size()){
        mustPoints.insert(stoi(v[i]));
    }
    mustPoints.erase(rawS);
    mustPoints.erase(rawT);
    solve();
}

char* topo[5000];
char demand[5000];
int main(int argc, char *argv[])
{

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