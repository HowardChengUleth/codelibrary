#include <iostream>
#include <algorithm>
#include <vector>
#include <cstdio>

using namespace std;

typedef pair<int,int> pii;
typedef vector< vector<pii> > vvpii;
typedef vector<int> vi;

// G[u][a]=(z,w) means u is connected to z with weight w. O(|V|.|E|). 
// s is source. dist and pre are output parameters. negative edges allowed.
// directed graphs allowed. undirected graphs (use 2 edges per edge) allowed.
bool BellmanFord(vvpii & G,int s,vi & dist, vi & pre, int dcv)
{ // ^^^ dist is the minimum distance from source. use pre to build the path.
	int a,b,u,z,w; int n=G.size(); // ^^^ dcv is disconnected value.

	for(a=0 ; a < n ; a++) { dist[a]=dcv; pre[a]=-1; }
	dist[s]=0;

	for(a=0 ; a < n ; a++)
		for(u=0 ; u < n ; u++)
			if(dist[u]!=dcv)
				for(b=0 ; b < G[u].size() ; b++)
				{
					z=G[u][b].first;
					w=G[u][b].second; // vvv modifiy for tie breaking.
					if(dist[u] + w < dist[z] || dist[z]==dcv)
					{ // vvv we have a negative cycle
						if(a == n - 1) return true;
						dist[z]=dist[u] + w; pre[z]=u;
					}
				}

	return false; 
// for negative cycle between source and a special node check if you can
// relax more using any edge connected to any node on the path.
} // returns true if there is a negative cycle reachable from source.

int main()
{
	//freopen("input.txt","r",stdin);
	int n,r,a,b,i,j,q,t=0;;

	while(cin>>n)
	{
		t++;
		cout<<"Set #"<<t<<endl;

		vvpii G;
		vector<int> bypass(n,0);

		for(i=0;i<n;i++)
		{
			G.push_back(vector<pii>());
			cin>>bypass[i];
		}

		cin>>r;

		for(i=0;i<r;i++)
		{
			cin>>a>>b;
			a--;b--;
			int w=bypass[b]-bypass[a];
			w=w*w*w;
			G[a].push_back(make_pair(b,w));
		}

		vi dist(n,0),pre(n,0);
		const int dcv=1e9;

		BellmanFord(G,0,dist,pre,dcv);

		cin>>q;

		for(i=0;i<q;i++)
		{
			cin>>a;
			a--;

			bool cycle =false;
			vector<int> path;
			int u=a;
			while(true)
			{
				if(u==-1)
					break;

				for (j=0;j<G[u].size();j++)
				{
					int z=G[u][j].first;
					int w=G[u][j].second;
					if(dist[u]+w<dist[z])
					{
						cycle=true;
						goto next;
					}
				}

				u=pre[u];
			}
			next:;

			if(dist[a]<3 || dist[a]==dcv || cycle )
				cout<<"?"<<endl;
			else
				cout<<dist[a]<<endl;
		}
	}
	return 0;
}