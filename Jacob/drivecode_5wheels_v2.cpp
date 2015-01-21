#include<iostream>
#include<assert.h>
#include<vector>

using namespace std;

struct Joystick{
	float j1x;
	float j1y;
	float j2x;
	float j2y;
};

int main(){
	Joystick joy;
	vector<Joystick> v;
	
	joy.j1x = 0;
	joy.j1y = 0;
	joy.j2x = 0;
	joy.j2y = 0;
	v.push_back(joy);
	joy.j1x = 0;
	joy.j1y = 1;
	joy.j2x = 0;
	joy.j2y = 1;
	v.push_back(joy);
	joy.j1x = 0;
	joy.j1y = -1;
	joy.j2x = 0;
	joy.j2y = -1;
	v.push_back(joy);
	joy.j1x = 0;
	joy.j1y = -1;
	joy.j2x = 0;
	joy.j2y = 1;
	v.push_back(joy);
	joy.j1x = 0;
	joy.j1y = 1;
	joy.j2x = 0;
	joy.j2y = -1;
	v.push_back(joy);
	joy.j1x = -1;
	joy.j1y = 0;
	joy.j2x = -1;
	joy.j2y = 0;
	v.push_back(joy);
	joy.j1x = 1;
	joy.j1y = 0;
	joy.j2x = 1;
	joy.j2y = 0;
	v.push_back(joy);

	cout<<"\n\nJ1x	J1y	J2x	J2y	Left	Right	Mid";
	int count=v.size();
	int count2 = 0;
	
while(count != 0){
		float left_track_out = v[count2].j1y;
		float right_track_out = v[count2].j2y;
		float mid_out = (v[count2].j1x + v[count2].j2x)/2;
		cout<<"\n"<<v[count2].j1x<<"	"<<v[count2].j1y<<"	"<<v[count2].j2x<<"	"<<v[count2].j2y<<"	"<<left_track_out<<"	"<<right_track_out<<"	"<<mid_out;
		count--;
		count2++;
	}
	cout<<"\n\n";

}
