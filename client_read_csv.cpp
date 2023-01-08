#include<windows.h>
#include<bits/stdc++.h>
using namespace std;
unsigned long long t0=0;
double high_precision_clock()
{
	return (chrono::high_resolution_clock::now().time_since_epoch().count()-t0)/1000000000.0;
}
string transpose(string note,int octave)
{
	if(note[1]=='#'||note[1]=='b')
		return note.substr(0,2)+to_string(stoi(note.substr(2))+octave);
	else
		return note.substr(0,1)+to_string(stoi(note.substr(1))+octave);
}
int main()
{
	WORD wVersionRequested;
	WSADATA wsaData;
	wVersionRequested = MAKEWORD(2, 2);
	WSAStartup(wVersionRequested, &wsaData);
	int PORT=440;
	string SERVER_ADDRESS;
	char c[128];
	cout<<"send to udp://";
	scanf("%s",c);
	SERVER_ADDRESS=c;
	SOCKET client = socket(AF_INET, SOCK_DGRAM, 0);
	SOCKADDR_IN addrSrv;
	addrSrv.sin_addr.S_un.S_addr = inet_addr(SERVER_ADDRESS.c_str());
	addrSrv.sin_family = AF_INET;
	addrSrv.sin_port = htons(PORT);
	cout<<"ready to send to "<<SERVER_ADDRESS<<":"<<PORT<<endl;
	string filename;
	int tsp;
	cout<<"read CSV file:";
	cin>>filename;
	cout<<"transpose:";
	cin>>tsp;
	ifstream fin(filename.c_str());
	string s;
	getline(fin,s);
	string note;
	double start,end,duration;
	int velocity;
	t0=chrono::high_resolution_clock::now().time_since_epoch().count();
	stringstream ss;
	while(fin>>note>>start>>end>>duration>>velocity)
	{
		//end is unused
		while(start>high_precision_clock());
		cout<<transpose(note,tsp)<<" ";
		ss<<transpose(note,tsp)<<" "<<int(duration*44100)<<" "<<velocity<<"\n";
		getline(ss,s);
		sendto(client,s.c_str(),s.length(),0,(SOCKADDR*)&addrSrv,sizeof(SOCKADDR));
	}
	closesocket(client);
	WSACleanup();
	return 0;
}
