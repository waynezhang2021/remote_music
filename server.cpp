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
bool flag=false;
wave_attribute wa={nullptr,nullptr,bufsize,16,sample_rate,1};
CRITICAL_SECTION cs;
void add_note(int main_buffer,double freq,int start,int duration,int velocity,string key)
{
	tone_attribute ta;
	printf("%s ",key.c_str());
	int sample_end=start+duration;
	if(main_buffer==1)
	{
		if(start+duration<bufsize)
		{
			wa.samples=buf1;
			wa.note_count=note_count1;
			ta={freq,start,duration,velocity/127.0/volume_divider,-1,1};
			add_tone(wa,ta,sin);
		}
		else
		{
			if(start+duration<bufsize*2)
			{
				wa.samples=buf1;
				wa.note_count=note_count1;
				ta={freq,start,bufsize-start-1,velocity/127.0/volume_divider,-1,1};
				add_tone(wa,ta,sin);
				wa.samples=buf2;
				wa.note_count=note_count2;
				ta={freq,0,sample_end-bufsize,velocity/127.0/volume_divider,-1,1};
				add_tone(wa,ta,sin);
			}
		}
	}
	if(main_buffer==2)
	{
		if(start+duration<bufsize)
		{
			wa.samples=buf2;
			wa.note_count=note_count2;
			ta={freq,start,duration,velocity/127.0/volume_divider,-1,1};
			add_tone(wa,ta,sin);
		}
		else
		{
			if(start+duration<bufsize*2)
			{
				wa.samples=buf2;
				wa.note_count=note_count2;
				ta={freq,start,bufsize-start-1,velocity/127.0/volume_divider,-1,1};
				add_tone(wa,ta,sin);
				wa.samples=buf1;
				wa.note_count=note_count1;
				ta={freq,0,sample_end-bufsize,velocity/127.0/volume_divider,-1,1};
				add_tone(wa,ta,sin);
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
		start=(chrono::high_resolution_clock::now().time_since_epoch().count()-t0)/1000000000.0*44100.0;
	}
}
void* play_thread(void*)
{
	while(1)
	{
		EnterCriticalSection(&cs);
		main_buffer=1;
		LeaveCriticalSection(&cs);
		wave_play({buf1,note_count1,bufsize,16,sample_rate,1});
		EnterCriticalSection(&cs);
		memset(buf1,0,bufsize*2);
		memset(note_count1,0,bufsize*2);
		main_buffer=2;
		LeaveCriticalSection(&cs);
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
	int MSGSIZE=1048576;
	InitializeCriticalSection(&cs);
	while(buf1==nullptr)
		buf1=new(nothrow) short[bufsize];
	while(buf2==nullptr)
		buf2=new(nothrow) short[bufsize];
	while(note_count1==nullptr)
		note_count1=new(nothrow) short[bufsize];
	while(note_count2==nullptr)
		note_count2=new(nothrow) short[bufsize];
	char *szMessage=nullptr;
	while(szMessage==nullptr)
		szMessage=new(nothrow) char[MSGSIZE];
	WSADATA wsaData;
	WSAStartup(0x0202, &wsaData);
	int port=440;
	SOCKET sockSrv = socket(AF_INET, SOCK_DGRAM, 0);
	SOCKADDR_IN addrSrv;
	addrSrv.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
	addrSrv.sin_family = AF_INET;
	addrSrv.sin_port = htons(port);
	bind(sockSrv, (SOCKADDR*)&addrSrv, sizeof(SOCKADDR));
	SOCKADDR_IN addrClient;
	int len = sizeof(SOCKADDR);
	int ret;
	pthread_create(0,0,play_thread,0); 
	pthread_create(0,0,clock_thread,0);
	cout<<"receiving on port "<<port<<endl;
	stringstream ss;
	string pitch;
	int duration,velocity;
	while(1)
	{
	    ret=recvfrom(sockSrv,szMessage,MSGSIZE,0,(SOCKADDR*)&addrClient,&len);
	    if(ret<=0)
	    	continue;
	    szMessage[ret] = '\0';
	    //printf("received [%d bytes]: %s\n", ret, szMessage);
	    ss.clear();
	    ss<<string(szMessage);
	    while(ss>>pitch>>duration>>velocity)
	    {
	    	EnterCriticalSection(&cs);
	    	add_note(main_buffer,note_to_hz(pitch),start+2205,duration,velocity,pitch);
	    	LeaveCriticalSection(&cs);
		}
	}
	return 0;
}
