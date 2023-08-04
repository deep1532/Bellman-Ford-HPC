#include<bits/stdc++.h>
#include <omp.h>
#define NUM_THREADS 2 // change this 1 2 4 8, 1 for serial

using namespace std;

struct edge {
  int start;
  int end;
  int weight;
};

void bf(int N, int e_cnt, int start_v, vector<edge> gr) {

    int inf = 10000000; 
    N++;
    vector<int> d(N); 
    //#pragma omp parallel for num_threads(NUM_THREADS)
     for(int i=0;i<N;i++)
     d[i]=inf;

    d[start_v] = 0;
    omp_lock_t writelock;
    omp_init_lock(&writelock);
     int p=0;
     #pragma omp parallel num_threads(NUM_THREADS)
        #pragma omp single
        p=omp_get_num_threads();

    chrono::high_resolution_clock::time_point start_t = std::chrono::high_resolution_clock::now();;
    
     
    for (int i = 1; i <= N - 1; i++) 
      {
      //guided dynamic static
        #pragma omp parallel for num_threads(NUM_THREADS) schedule(guided,16)
        for (int j = 0; j < e_cnt; j++)
             
            if (d[ gr[j].start ] < inf) 
              {
                if (d[ gr[j].end ] > d[ gr[j].start ] + gr[j].weight) 
                  {
                    omp_set_lock(&writelock);
                    d[ gr[j].end ] = d[ gr[j].start ] + gr[j].weight;
                    omp_unset_lock(&writelock);
                  }
              }
      }
      omp_destroy_lock(&writelock);	 	   
  chrono::high_resolution_clock::time_point end_t = std::chrono::high_resolution_clock::now();

  chrono::duration<double> time_span = chrono::duration_cast<chrono::duration<double>>(end_t - start_t);
 
  cout<<p<< "Thread Elapsed time: " << time_span.count() << endl; 

  for(int i = 0;i<N;i++) {
       cout << i << " " << d[i] << endl;
    }

}

int main() {

    #ifndef ONLINE_JUDGE
    freopen("input300_44k.txt", "r", stdin);
    freopen("outputP8.txt", "w", stdout);
    #endif   
    int N,m;
    cin >> N >> m;
    vector<edge> edges; 
    for(int i = 0;i<m;i++) {
        int u, v, wt;
        cin >> u >> v >> wt; 
        edges.push_back({u, v, wt}); 
    }
    bf(N, m, 1, edges);

  return 0;
}


//new code 

#include <bits/stdc++.h>
#include <omp.h>
#define NUM_THREADS 2// change this 1 2 4 8, 1 for serial
using namespace std;

#define fast_io ios::sync_with_stdio(0),cin.tie(0),cout.tie(0)
#define int long long

bool test_case() {
    int n,m;
    cin>>n>>m;
    n++;
    int s=1;
  std::vector<vector<pair<int,int>>> adj(n);
  vector<int> d(n,INT_MAX);
  for(int i=0;i<m;i++)
  {
    int a,b,w;
    cin>>a>>b>>w;
    adj[a].push_back({b,w});
  }
  // for(int i=0;i<n;i++)
  // {
  //   cout<<i<<"=>>";
  //   for(auto i:v[i])
  //     cout<<i.first<<"->";
  //   cout<<endl;
  // }
  vector<int> cnt(n, 0);
    vector<bool> inqueue(n, false);
    queue<int> q;
  chrono::high_resolution_clock::time_point start_t = std::chrono::high_resolution_clock::now();
    d[s] = 0;
    q.push(s);
    inqueue[s] = true;
    omp_lock_t writelock;
    omp_init_lock(&writelock);
    while (!q.empty()) {
        int v = q.front();
        q.pop();
        inqueue[v] = false;
        int n=adj[v].size();
        //vector<pair<int,int>>::iterator edge;
        //edge=adj[v].begin();
        #pragma omp parallel for num_threads(NUM_THREADS)
        for(int i=0;i<n;i++){
            int to = adj[v][i].first;
            int len = adj[v][i].second;
          
            if (d[v] + len < d[to]) {
                omp_set_lock(&writelock);
                    d[to] = d[v] + len;
                if (!inqueue[to]) {
                    q.push(to);
                    inqueue[to] = true;
                    cnt[to]++;
                omp_unset_lock(&writelock);
                
                    //if (cnt[to] > n)
                        //return false;  // negative cycle
                }
            }
        }
        
    }
    omp_destroy_lock(&writelock);	
    chrono::high_resolution_clock::time_point end_t = std::chrono::high_resolution_clock::now();

  chrono::duration<double> time_span = chrono::duration_cast<chrono::duration<double>>(end_t - start_t);
    cout<< "Thread Elapsed time: " << time_span.count() << endl; 
    for(int i=0;i<n;i++)
      cout<<i<<"->"<<d[i]<<endl;
    return true;

}

signed main() {
#ifndef ONLINE_JUDGE
    freopen("input900_250k.txt", "r", stdin);
    freopen("outputP8.txt", "w", stdout);
    #endif 
    fast_io;
    int T = 1;
    //cin >> T;
    while(T--) bool b=test_case();
}
