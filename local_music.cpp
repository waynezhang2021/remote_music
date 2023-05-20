#include<bits/stdc++.h>
#include<windows.h>
#include<pthread.h>
#include<time.h>
#include"wave.h"
using namespace std;
const int volume_divider=3;
int sample_rate=44100;
int bufsize=sample_rate*10;
short *buf1=nullptr,*buf2=nullptr;
short *note_count1=nullptr,*note_count2=nullptr; 
const double pi=3.1415926535897932384626433832795;
const double c=pi/sample_rate;
short main_buffer=1;
int start=0;
wave_attribute wa={nullptr,nullptr,bufsize,16,sample_rate,1};
CRITICAL_SECTION cs;
unsigned long long t0=0;
double high_precision_clock()
{
	return (chrono::high_resolution_clock::now().time_since_epoch().count()-t0)/1000000000.0;
}
inline double sqr(double n)
{
	return (((n-int(n/6.283185307179586476925286766559)*6.283185307179586476925286766559)>(3.14159265358979324846264338327960288))?1:-1);
}
inline double tri(double n)
{
	return abs((n-int(n/3.14159265358979324846264338327960288)*3.14159265358979323846264338327950288)-1.5707963267948966242313216916398);
}
inline double saw(double n)
{
	return (n-int(n/6.283185307179586476925286766559)*6.283185307179586476925286766559);
}
typedef enum
{
	SINE=1,
	SQUARE=2,
	TRIANGLE=3,
	SAWTOOTH=4
} synth_func;
void add_note(int main_buffer,double freq,int start,int duration,int velocity,string key,synth_func s)
{
	tone_attribute ta;
	printf("%s ",key.c_str());
	if(main_buffer==1)
	{
		if(start+duration<bufsize)
		{
			wa.samples=buf1;
			wa.note_count=note_count1;
			if(s==SINE)
			{
				ta={freq,start,duration,velocity/127.0/volume_divider,-1,1};
				add_tone(wa,ta,sin);	
			}
			if(s==SQUARE)
			{
				ta={freq,start,duration,velocity/127.0/volume_divider,-1,1};
				add_tone(wa,ta,sqr);	
			}
			if(s==TRIANGLE)
			{
				ta={freq,start,duration,velocity/127.0/volume_divider,0,1.5707963267948966242313216916398};
				add_tone(wa,ta,tri);
			}
			if(s==SAWTOOTH)
			{
				ta={freq,start,duration,velocity/127.0/volume_divider,0,3.14159265358979323846264338327950288};
				add_tone(wa,ta,saw);
			}
		}
		else
		{
				if(start+duration<bufsize*2)
				{
					wa.samples=buf1;
					wa.note_count=note_count1;
				if(s==SINE)
				{
					ta={freq,start,duration,velocity/127.0/volume_divider,-1,1};
					add_tone(wa,ta,sin);	
				}
				if(s==SQUARE)
				{
					ta={freq,start,duration,velocity/127.0/volume_divider,-1,1};
					add_tone(wa,ta,sqr);	
				}
				if(s==TRIANGLE)
				{
					ta={freq,start,duration,velocity/127.0/volume_divider,0,1.5707963267948966242313216916398};
					add_tone(wa,ta,tri);
				}
				if(s==SAWTOOTH)
				{
					ta={freq,start,duration,velocity/127.0/volume_divider,0,3.14159265358979323846264338327950288};
					add_tone(wa,ta,saw);
				}
				wa.samples=buf2;
				wa.note_count=note_count2;
				if(s==SINE)
				{
					ta={freq,start,duration,velocity/127.0/volume_divider,-1,1};
					add_tone(wa,ta,sin);	
				}
				if(s==SQUARE)
				{
					ta={freq,start,duration,velocity/127.0/volume_divider,-1,1};
					add_tone(wa,ta,sqr);	
				}
				if(s==TRIANGLE)
				{
					ta={freq,start,duration,velocity/127.0/volume_divider,0,1.5707963267948966242313216916398};
					add_tone(wa,ta,tri);
				}
				if(s==SAWTOOTH)
				{
					ta={freq,start,duration,velocity/127.0/volume_divider,0,3.14159265358979323846264338327950288};
					add_tone(wa,ta,saw);
				}
			}
		}
	}
	if(main_buffer==2)
	{
		if(start+duration<bufsize)
		{
			wa.samples=buf2;
			wa.note_count=note_count2;
			if(s==SINE)
			{
				ta={freq,start,duration,velocity/127.0/volume_divider,-1,1};
				add_tone(wa,ta,sin);	
			}
			if(s==SQUARE)
			{
				ta={freq,start,duration,velocity/127.0/volume_divider,-1,1};
				add_tone(wa,ta,sqr);	
			}
			if(s==TRIANGLE)
			{
				ta={freq,start,duration,velocity/127.0/volume_divider,0,1.5707963267948966242313216916398};
				add_tone(wa,ta,tri);
			}
			if(s==SAWTOOTH)
			{
				ta={freq,start,duration,velocity/127.0/volume_divider,0,3.14159265358979323846264338327950288};
				add_tone(wa,ta,saw);
			}
		}
		else
		{
			if(start+duration<bufsize*2)
			{
				wa.samples=buf2;
				wa.note_count=note_count2;
				if(s==SINE)
				{
					ta={freq,start,duration,velocity/127.0/volume_divider,-1,1};
					add_tone(wa,ta,sin);	
				}
				if(s==SQUARE)
				{
					ta={freq,start,duration,velocity/127.0/volume_divider,-1,1};
					add_tone(wa,ta,sqr);	
				}
				if(s==TRIANGLE)
				{
					ta={freq,start,duration,velocity/127.0/volume_divider,0,1.5707963267948966242313216916398};
					add_tone(wa,ta,tri);
				}
				if(s==SAWTOOTH)
				{
					ta={freq,start,duration,velocity/127.0/volume_divider,0,3.14159265358979323846264338327950288};
					add_tone(wa,ta,saw);
				}
				wa.samples=buf1;
				wa.note_count=note_count1;
				if(s==SINE)
				{
					ta={freq,start,duration,velocity/127.0/volume_divider,-1,1};
					add_tone(wa,ta,sin);	
				}
				if(s==SQUARE)
				{
					ta={freq,start,duration,velocity/127.0/volume_divider,-1,1};
					add_tone(wa,ta,sqr);	
				}
				if(s==TRIANGLE)
				{
					ta={freq,start,duration,velocity/127.0/volume_divider,0,1.5707963267948966242313216916398};
					add_tone(wa,ta,tri);
				}
				if(s==SAWTOOTH)
				{
					ta={freq,start,duration,velocity/127.0/volume_divider,0,3.14159265358979323846264338327950288};
					add_tone(wa,ta,saw);
				}
			}
		}
	}
}
void* clock_thread(void*)
{
	int last_main_buffer=1;
	unsigned long long t0=chrono::high_resolution_clock::now().time_since_epoch().count();
	while(1)
	{
		if(main_buffer!=last_main_buffer)
		{
			last_main_buffer=main_buffer;
			t0=chrono::high_resolution_clock::now().time_since_epoch().count();
		}
		start=(chrono::high_resolution_clock::now().time_since_epoch().count()-t0)/1000000000.0*sample_rate;
	}
}
void* play_thread(void*)
{
	while(1)
	{
		main_buffer=1;
		wave_play({buf1,note_count1,bufsize,16,sample_rate,1});
		EnterCriticalSection(&cs);
		memset(buf1,0,bufsize*2);
		memset(note_count1,0,bufsize*2);
		LeaveCriticalSection(&cs);
		main_buffer=2;
		wave_play({buf2,note_count2,bufsize,16,sample_rate,1});
		EnterCriticalSection(&cs);
		memset(buf2,0,bufsize*2);
		memset(note_count2,0,bufsize*2);
		LeaveCriticalSection(&cs);
	}
	return nullptr;
}
int main()
{
	InitializeCriticalSection(&cs);
	while(buf1==nullptr)
		buf1=new(nothrow) short[bufsize];
	while(buf2==nullptr)
		buf2=new(nothrow) short[bufsize];
	while(note_count1==nullptr)
		note_count1=new(nothrow) short[bufsize];
	while(note_count2==nullptr)
		note_count2=new(nothrow) short[bufsize];
	pthread_create(0,0,play_thread,0);
	pthread_create(0,0,clock_thread,0);
start:
	stringstream ss;
	string pitch;
	int velocity;
	cout<<"play CSV file:";
	string file;
	cin>>file;
	int trans;
	cout<<"transpose:";
	cin>>trans;
	double time_multiplier;
	cout<<"time multiplier:";
	cin>>time_multiplier;
	synth_func s;
	string st;
	cout<<"synth function:";
	cin>>st;
	s=((st=="sqr")?SQUARE:((st=="sin")?SINE:((st=="tri")?TRIANGLE:((st=="saw")?SAWTOOTH:SINE))));
	double begin,end,duration;
	ifstream fin(file.c_str());
	getline(fin,file);//file is just used as a useless string here(to skip the header)
	t0=chrono::high_resolution_clock::now().time_since_epoch().count(); 
	while(fin>>pitch>>begin>>end>>duration>>velocity)
	{
		while(begin>high_precision_clock()*time_multiplier);
		EnterCriticalSection(&cs);
		add_note(main_buffer,note_to_hz(pitch,trans),start+2205,duration*sample_rate/time_multiplier,velocity,pitch,s);
		LeaveCriticalSection(&cs);
	}
	system("cls");
	fin.close();
	goto start;
	return 0;
}
