/*
#if 0
#include "lib_record.h"
#include "route.h"
#include "lib_io.h"
#endif
#define MAX_SEARCH_TIME 9
*/
const int K = 1;
const int KK = 1;
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
        id = atoi(tmpv[0].c_str()); from = atoi(tmpv[1].c_str()); to = atoi(tmpv[2].c_str()); value = atoi(tmpv[3].c_str());
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
vii raw_mirror,st_raw_mirror;

struct Edge
{
    int rid;
    int to,cap,rev;
    bool isrev;
};
struct Ford
{
    vector<Edge> G[MAX_V];
    // bool used[MAX_V];
    // int used[MAX_V]; 
    SI used[MAX_V];


    pii headtail[MAX_V]; //
    pii prenxt[MAX_V];
    int curS,curT;
    int forbid1,forbid2,forbid3;
    set<int> affectS,affectT;

    void AddEdge(int rid, int from, int to, int cap){
        G[from].push_back((Edge){rid, to, cap, (int)G[to].size(),false});
        G[to].push_back((Edge){-1, from, 0, (int)G[from].size()-1,true});
    }
    pii findhead(int x){
        assert(x>=0);
        if(0) printf("findhead %d  head:%d  tail:%d tailhead:%d\n",x,headtail[x].X,headtail[x].Y , headtail[mirrorl2r[x]].X);
        if(x == headtail[mirrorl2r[x]].X){
            if(K){
                rep(i,st_raw_mirror.size()) printf("(%3d:%3d)",st_raw_mirror[i].X,headtail[st_raw_mirror[i].X].Y); puts("");
                rep(i,st_raw_mirror.size()) printf("(%3d:%3d)",st_raw_mirror[i].Y,headtail[st_raw_mirror[i].Y].X); puts("");
            }
            assert(0);
        }
        if(!mustPoint[x]) return mkp(x,0);
        if(headtail[mirrorl2r[x]].X >= 0){
            pii tmp = findhead(headtail[mirrorl2r[x]].X);
            return mkp(tmp.X,tmp.Y+1);
        }
        else return mkp(x,0);
    }
    void trigger(int v){
        if(KK) printf("trigger %d\n", v);
        if(v < 0){
            curS = -1; forbid1=forbid2=forbid3=-2;
        }else if(v == rawS){
            curS = rawS;
            if(mustPoints.size() != 0) forbid1 = rawT, forbid3 = rawT;
            forbid2 = rawS;
        }else if(mustPoint[v]){
            curS = v;
            assert(curS>=0);
            forbid1 = mirrorl2r[curS];
            pii tmp = findhead(curS);
            assert(tmp.X >= 0);
            // printf("tmp %d %d",tmp.X,tmp.Y);
            forbid2 = mirrorl2r[tmp.X];
            if(tmp.Y < mustPoints.size()) forbid3 = rawT;

        }
        if(forbid1 < 0) forbid1 = -2;
        if(forbid2 < 0) forbid2 = -2;
        if(forbid3 < 0) forbid3 = -2;
        if(KK) printf("trigger end\n");
    }
    void throughT(int v, int S){
        if(KK) printf("throughT v:%d S:%d  head:%d tail:%d\n",v,S,headtail[v].X,headtail[v].Y);
        if(KK) printf("forbid1:%d forbid2:%d\n",forbid1,forbid2 );
        int tmp = v;
        while(tmp >= 0 && tmp != superT){
            //printf("\t%d\n",tmp );
            headtail[tmp].X = S;
            tmp = prenxt[tmp].Y;
        }
    }
    void throughS(int v, int T){
        if(KK) printf("throughS v:%d T:%d  head:%d tail:%d\n",v,T,headtail[v].X,headtail[v].Y);
        int tmp = v;
        while(tmp >= 0 && tmp != superS){

            headtail[tmp].Y = T;
            tmp = prenxt[tmp].X;
        }
    }
    int dfs(int v,int t,int f,int pre = -1, bool preEisrev = false){
        if(KK) printf("\tv:%d G[v].size():%lu t:%d f:%d pre:%d preEisrev:%d\n",v,G[v].size(),t,f,pre,preEisrev );
        affectS.insert(headtail[v].X);
        affectT.insert(headtail[v].Y);
        if(v == t){
            curT = pre;
            // connectl2r[curS] = curT; connectr2l[curT] = curS;
            affectS.insert(curS);
            affectT.insert(curT);
            return f;
        }
        if(KK) puts("a");
        // used[v] = true;
        used[v].insert(curS);
        if(KK) printf("\t\tG[v].size:%lu\n",G[v].size());
        for(int i = 0; i < G[v].size(); i++){
            Edge &e = G[v][i];
            if(e.to == pre || e.to == superS) continue;
            // if(KK) printf("\t\te: %d %d %d s:%d t:%d used:%d forbid1:%d 2:%d v:%d\n",e.to,e.cap,e.isrev, headtail[e.to].X, headtail[e.to].Y, used[e.to], forbid1, forbid2, v);
            if(KK) printf("\t\te: %d %d %d s:%d t:%d used:%d forbid1:%d 2:%d v:%d\n",e.to,e.cap,e.isrev, headtail[e.to].X, headtail[e.to].Y, used[e.to].find(curS) != used[e.to].end(), forbid1, forbid2, v);


            //不能走到禁止节点,也不能掐断流向禁止节点的流
            if(e.to == forbid1 || e.to == forbid2 ) continue;
            if(headtail[e.to].Y == forbid1 || headtail[e.to].Y == forbid2 ) continue;

            if(KK) puts("aa");
            // if(!used[e.to] && e.cap > 0){
            if(used[e.to].find(curS) == used[e.to].end() && e.cap > 0){
                if(KK) puts("b");
                //trigger
                if(e.to == rawS || mustPoint[e.to]) trigger(e.to);
                
                //掐断之前一条流. 需要更新curS,forbid
                if(e.isrev){
                    if(KK) puts("c");
                    int precurS = curS, nxtcurS = headtail[e.to].X, nxtT = headtail[e.to].Y;
                    throughT(v, curS);    
                    throughS(e.to, -3);
                    curS = nxtcurS;
                    trigger(curS);
                    if(KK) puts("cc");
                    int d = dfs(e.to, t, min(f, e.cap),v,true);
                    if(d > 0){
                        if(preEisrev) prenxt[v] = mkp(-1,-1);
                        else prenxt[v].X = pre;

                        e.cap -= d;
                        G[e.to][e.rev].cap += d;
                        return d;
                    }else{
                        curS = precurS;
                        trigger(curS);
                        throughT(v, nxtcurS);
                        throughS(e.to, nxtT);
                    }
                }else{
                    if(KK) puts("d");
                    int d = dfs(e.to, t, min(f, e.cap),v, false);
                    if(d > 0){
                        prenxt[v].Y = e.to;
                        if(!preEisrev) prenxt[v].X = pre;

                        e.cap -= d;
                        G[e.to][e.rev].cap += d;
                        return d;
                    }
                }
                if(e.to == rawS || mustPoint[e.to]) trigger(-1);
            }
        }
        if(KK) printf("\tv:%d return\n",v);
        return 0;
    }
    void getthrough(){
        if(KK) puts("getthrough:");
        rep(i,st_raw_mirror.size()){
            int k = st_raw_mirror[i].X, tmp = k;
            if(prenxt[tmp].Y < 0) continue;
            if(KK) printf("k:%d\n",k );
            while(tmp != superT){
                if(KK) printf("\ttmp:%d\n",tmp);
                assert(tmp>=0);
                headtail[tmp].X = k;
                tmp = prenxt[tmp].Y;
            }
        }
        rep(i,st_raw_mirror.size()){
            int k = st_raw_mirror[i].Y, tmp = k;
            if(prenxt[tmp].X < 0) continue;
            if(KK) printf("k:%d\n",k );
            while(tmp != superS){
                if(KK) printf("\ttmp:%d\n",tmp);
                assert(tmp>=0);
                headtail[tmp].Y = k;
                tmp = prenxt[tmp].X;
            }
        }
    }
    int MaxFlow(int s = superS,int t = superT){
        int flow = 0;
        rep(i,MAX_V) 
            // connectl2r[i]=connectr2l[i]=i,
            headtail[i]=prenxt[i]=mkp(-1,-1);
        while(true){
            if(K) printf("curMaxFlow:%d\n", flow);
            // memset(used, 0, sizeof used);
            rep(i,MAX_V) used[i].clear();
            affectS.clear();
            affectT.clear();
            trigger(-1);
            forbid1 = forbid2 = forbid3 = -2;

            int f = dfs(s, t, INF);

            if(K){
                printf("\tf:%d curS:%d curT:%d\n",f,curS,curT);
            }
            getthrough();
            if(f == 0) return flow;
            flow += f;

            
            puts("a");
            if(K){
                rep(i,st_raw_mirror.size()) printf("(%3d:%3d)",st_raw_mirror[i].X,headtail[st_raw_mirror[i].X].Y); puts("");
                rep(i,st_raw_mirror.size()) printf("(%3d:%3d)",st_raw_mirror[i].Y,headtail[st_raw_mirror[i].Y].X); puts("");
            }
            puts("Loop end");
        }
    }

    void output(){
        return;
        if(K){
            printf("\ndetail graph:\n");
            rep(i,st_raw_mirror.size()){
                int tmp = st_raw_mirror[i].X;
                printf("%d",tmp);
                while(tmp >= 0 && tmp != superT){
                    tmp = prenxt[tmp].Y;
                    // if(tmp < rawN)
                        printf(" - %d", tmp);
                }
                puts("");
            }
        }
        vi ans;
        int tmp = rawS,nxt;
        while(tmp != rawT){
            if(tmp>=rawN && mustPoint[mirrorr2l[tmp]]){
                tmp = mirrorr2l[tmp];
                continue;
            }
            nxt = prenxt[tmp].Y;
            rep(i,G[tmp].size()){
                if(G[tmp][i].to == nxt){
                    if(G[tmp][i].rid >= 0) ans.pb(G[tmp][i].rid);
                    break;
                }
            }
            tmp = nxt;
        }
        SI tmps;
        if(K){
            printf("route:\n");
            rep(i,ans.size()) printf("%d ", ans[i]); puts("");
        }
        if(K){
            rep(i,ans.size()){
                printf("%d %d\n",rawedges[ans[i]].from,rawedges[ans[i]].to );
                tmps.insert(rawedges[ans[i]].from);
                tmps.insert(rawedges[ans[i]].to);
            }
            for(int i:mustPoints){
                if(tmps.find(i) == tmps.end()) printf("Error! no node:%d\n",i );
            }
        }
    }
}ford;


void makeGraph(){
    raw_mirror.clear(); st_raw_mirror.clear();
    memset(mirrorl2r, -1, sizeof mirrorl2r);
    memset(mirrorr2l, -1, sizeof mirrorr2l);
    memset(mirror2Raw, -1, sizeof mirror2Raw);
    rep(i,rawN){
        mirror2Raw[i]=mirror2Raw[i+rawN]=i;
        if(i == rawS || i == rawT) continue;
        mirrorl2r[i] = i+rawN; mirrorr2l[i+rawN] = i;
        if(!mustPoint[i]) ford.AddEdge(-1, i, i+rawN, 1);
    }
    superS = 2*rawN;
    superT = 2*rawN+1;
    st_raw_mirror.pb(mkp(rawS,rawT));
    for(SI::iterator ite = mustPoints.begin(); ite!=mustPoints.end(); ite++){
        raw_mirror.pb(mkp(*ite,mirrorl2r[*ite]));
        st_raw_mirror.pb(mkp(*ite,mirrorl2r[*ite]));
    }

    ford.AddEdge(-1,superS,rawS,1);
    rep(i,raw_mirror.size()){
        ford.AddEdge(-1,superS,raw_mirror[i].X,1);
    }
    //原先点中,V'到V'的点需要改动到V'到mirror V'上去
    rep(i,rawedges.size()){
        RawEdge& e = rawedges[i];
        if(e.from == rawS && e.to == rawT){
            if(mustPoints.size() == 0) ford.AddEdge(e.id,e.from,e.to,1);
        }else if(e.to == rawS || e.from == rawT){
            //do nothing
        }else if(e.from == rawS){
            if(mustPoint[e.to]) ford.AddEdge(e.id, e.from, mirrorl2r[e.to], 1);
            else ford.AddEdge(e.id, e.from, e.to, 1);
        }else if(e.to == rawT){
            if(mustPoint[e.from]) ford.AddEdge(e.id, e.from, e.to, 1);
            else ford.AddEdge(e.id, mirrorl2r[e.from], e.to, 1);
        }else if(mustPoint[e.from] && mustPoint[e.to]){
            ford.AddEdge(e.id, e.from, mirrorl2r[e.to], 1);
        }else if(mustPoint[e.from]){
            ford.AddEdge(e.id,e.from,e.to,1);
            // ford.AddEdge(e.id,mirrorl2r[e.from],e.to,1);  //右侧点不需要superT以外的出度
        }else if(mustPoint[e.to]){
            // ford.AddEdge(e.id,e.from,e.to,1); //左侧点不需要superS以外的入度
            ford.AddEdge(e.id,mirrorl2r[e.from],mirrorl2r[e.to],1);
        }else {
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
    if(K){
        puts("makeGraph end");
        printf("superS:%d superT:%d mustnodenum:%lu\n", superS, superT, mustPoints.size());
        printf("left : %2d",rawS ); for(int i:mustPoints) printf(" %2d",i ); puts("");
        printf("right: %2d",rawT ); for(int i:mustPoints) printf(" %2d",mirrorl2r[i] ); puts("");
    }
    int f = ford.MaxFlow(superS,superT);
    if(K) printf("MaxFlow:%d\n",f);
    if(f != mustPoints.size() + 1){
        puts("Error");
        return;
    }else{
        ford.output();
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
        if(KK){
            printf("%d %d %d %d\n",e.id,e.from,e.to,e.value );
        }
        smax(rawN, e.from); smax(rawN, e.to);
        rawedges.pb(e);
    }
    rawN += 1;
    rawM = rawedges.size();

    vector<string> v = split(demand,',');
    rawS = atoi(v[0].c_str());
    rawT = atoi(v[1].c_str());
    v = split(v[v.size()-1], '|');
    memset(mustPoint, 0, sizeof mustPoint);
    rep(i,v.size()){
        int tmp = atoi(v[i].c_str());
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
    // freopen("out.txt","w",stdout);
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