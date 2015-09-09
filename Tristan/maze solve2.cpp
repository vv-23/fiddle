

#include <iostream>
#include <math.h>
#include <stdlib.h>
#include <vector>
#include <random>
#include <time.h> 
#include <assert.h>
#include <unistd.h>

#define NYI {cout << "NYI " << __LINE__<<"\n"; exit(1);}
 

using namespace std;
/////////////////////////////////////////////////////////////////////////////
// Structures
//
// All Structures
/////////////////////////////////////////////////////////////////////////////
struct point{
	int x;
	int y;
};

struct list{
point pt;
point prev;
bool v;
};
//////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
// operatior point
//
// allow the use of diffrent things with the structure point
/////////////////////////////////////////////////////////////////////////////
bool operator!=(point a,point b){
	return (a.x != b.x || a.y != b.y);
}
ostream&operator<<(ostream& o, point a){
	
	o<< a.x << "," << a.y << endl;
	
	return o;
}

bool operator==(point a,point b){
	return (a.x == b.x && a.y == b.y);
}
/////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////
// lastpointvalid
//
// returns true if the given point is not in the vector
/////////////////////////////////////////////////////////////////////////////
bool lastpointvalid(point p,vector<point> lp)
{
	bool valid = true;

	for (int i=0; i< (int)lp.size(); i++){
		if ( p == lp[i]){
			valid = false;
		}
	}
	return valid;
}


/////////////////////////////////////////////////////////////////////////////
// bounderies
//
// returns true if p is in the grid
/////////////////////////////////////////////////////////////////////////////
bool bounderies(point p){
	bool xvalid = p.x>=1 && p.x<=4;
	bool yvalid = p.y>=1 && p.y<=3;

	return xvalid && yvalid;
}

//////////////////////////////////////////////////////////////////////////////
// Walls
//
// returns true if p is not a wall
//////////////////////////////////////////////////////////////////////////////
bool Walls(point p){

	return !((p.y==1 && p.x==3) || (p.y==2 && p.x==3));

}

/////////////////////////////////////////////////////////////////////////////
// valid																   
//																		   
// returns true if p is not a wall or boundry                              
/////////////////////////////////////////////////////////////////////////////
bool valid(point p){
	return (Walls(p) && bounderies(p));
}

/////////////////////////////////////////////////////////////////////////////
// validpoint
//
// returns true if the given point is not a wall or boundry and has not been visited
/////////////////////////////////////////////////////////////////////////////
bool validpoint(vector<list> v,point q){
	int i;
	int max;
	bool p ;

	p=false;
	max = v.size();
	for(i=0; i<max; i++){
		if(v[i].pt == q){
			p = true;
			break;
		}
		
	}
	return valid(q) && !p;
}

/////////////////////////////////////////////////////////////////////////////
// [dir]p
//
// returns adjacent points
/////////////////////////////////////////////////////////////////////////////
point leftp(point p){
	return point{p.x - 1,p.y};
}

point rightp(point p){
	return point{p.x + 1,p.y};
}

point upp(point p){
	return point{p.x,p.y + 1};
}

point downp(point p){
	return point{p.x,p.y - 1};
}
/////////////////////////////////////////////////////////////////////////////
// nextp
//
//returns next point that has not been visited
////////////////////////////////////////////////////////////////////////////
int nextp(vector<list> v){
	int max;
	int i;

	max = v.size();

	for (i=0; i<max; i++){
		if( !v[i].v){
			break;
		}
	}
	assert(i<max);
	return i;
}
/////////////////////////////////////////////////////////////////////////////
// getpoint
//
// given a point will return a vector of points that are valid
/////////////////////////////////////////////////////////////////////////////
vector<point> getpoint(vector<list> v,point p){
	point a;
	point b;
	point c;
	point d;
	vector<point> validpoints;
	
	a = leftp(p);
	b = rightp(p);
	c = upp(p);
	d = downp(p);

	if(validpoint(v,a)){
		validpoints.push_back(a);		
	}

	if(validpoint(v,b)){
		validpoints.push_back(b);		
	}

	if(validpoint(v,c)){
		validpoints.push_back(c);		
	}

	if(validpoint(v,d)){
		validpoints.push_back(d);		
	}
	
	return validpoints; 

}

/////////////////////////////////////////////////////////////////////////////
//Find Point
//
//finds point
////////////////////////////////////////////////////////////////////////////
int findpoint(vector<list> v,point q){
	int max;
	int i;

	max = v.size();

	for (i=0; i<max; i++){
		if( v[i].pt == q){
			break;
		}
	}
	assert(i<max);
	return i;
}
string dir(point p, point q){
	string s;

	if(q.< p.x){
		s = "Left";
	}
	else if(q.x > p.x){
	
		s = "Right";
	}
	else if(q.y < p.y){
		s = "Up";

	}	
	else if(q.y > p.y){
		s = "Down";
	}
	return s;
}	
int main(){
	//declarations	
	int lineofvector;
	point p;
	point e;
	vector<list> info;
	vector<point> nextpoint;
	list log;
	point f;
	bool endpoint;
	int lastline;
	vector<point> path;

endpoint = false;
f.x = -1;//set
f.y = -1;//inital location for previus

p.x = 1;//set
p.y = 1;//location of start point

e.x = 4;//set
e.y = 1;//location of en/d point

log.prev = f; //sets a prev point 
log.pt = p; //set the original point
log.v = false; // set the visited to not visited
info.push_back(log); //Push the info to the vector of structures 


	while (!endpoint){
		lineofvector = nextp(info); //find the line of stored data that is next in line

		p = info[lineofvector].pt;//save that loacation that you want to go to going to


		nextpoint = getpoint(info,p); // set a vector to have all posable points that are legal
		
		
		for (unsigned int i = 0; i < nextpoint.size(); i++){//loop tell for the size of the vector nextpoint
			log.prev = p; //store prev point
			log.v = false; //set the visisted to not visited
			log.pt = nextpoint[i]; // the point that you are storing
			if(log.pt == e){
				endpoint = true;
			}
			info.push_back(log); // store all the stored point
		}
	
		info[lineofvector].v = true; // set the visited status to visited

	}
	
	cout << "Path" << endl;

	lastline = findpoint(info,e);
	while(lastline != 0){
		path.push_back(info[lastline].pt);
		lastline = findpoint(info,info[lastline].prev);
	}

	path.push_back(info[0].pt);

	for(int i = path.size() - 1; i >= 0;i--){
		cout << path[i] << "and" << path[i-1]
 << dir(path[i],path[i-1]) << endl;
	}
	
}
