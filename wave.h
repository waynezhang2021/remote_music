#pragma once
#include<windows.h>
using namespace std;
typedef struct
{
	void* samples;
	void* note_count;
	int size;
	int bits;
	int sample_rate;
	int channels;
} wave_attribute;
typedef struct
{
	double freq;
	int start;
	int length;
	double volume;
	double func_min;
	double func_max;
} tone_attribute;
void wave_play(wave_attribute wa)
{
	void* samples=wa.samples;
	int size=wa.size;
	int bits=wa.bits;
	int sample_rate=wa.sample_rate;
	int channels=wa.channels;
	HWAVEOUT out;
	WAVEFORMATEX waveform;
	WAVEHDR header;
	waveform.wFormatTag = WAVE_FORMAT_PCM;
	waveform.nSamplesPerSec = sample_rate;
	waveform.wBitsPerSample = bits;
	waveform.nChannels = channels;
	waveform.nAvgBytesPerSec = bits*sample_rate;
	waveform.nBlockAlign = 2;
	waveform.cbSize = 0;
	HANDLE wait = CreateEvent(NULL,0,0,"");
	header.lpData = (LPSTR)samples;
	header.dwBufferLength = size*bits/8;
	header.dwBytesRecorded = 0;
	header.dwUser = 0;
	header.dwFlags = WAVE_ALLOWSYNC;
	header.dwLoops = 1;
	waveOutOpen(&out,WAVE_MAPPER,&waveform,(DWORD_PTR)&wait,0,CALLBACK_EVENT);
	waveOutPrepareHeader(out,&header,sizeof(WAVEHDR));
	waveOutWrite(out,&header,sizeof(WAVEHDR));
	Sleep(double(size)/sample_rate*1000);
	waveOutClose(out);
	return;
}
void add_tone(wave_attribute wa,tone_attribute ta,double func(double))
{
	if(wa.channels==1)
		if(ta.start+ta.length>=wa.size)
			throw logic_error("tone longer than buffer");
	if(wa.channels==2)
		if(ta.start+ta.length*2>=wa.size)
			throw logic_error("tone longer than buffer");
	void* samples=wa.samples;
	void* note_count=wa.note_count;
	int bits=wa.bits;
	int sample_rate=wa.sample_rate;
	int channels=wa.channels;
	int sample_max=pow(2,wa.bits)-1;
	int start=ta.start;
	int end=ta.start+ta.length;
	int coeff=sample_max*ta.volume/(ta.func_max-ta.func_min);
	int add=-ta.func_min;
	double c=3.1415926535897932384626433832795/sample_rate*ta.freq;
	if(bits==8)
	{
		if(channels==1)
			for(int i=start;i<end;i++)
			{
				((short*)(note_count))[i]++;
				((char*)(samples))[i]=((char*)(samples))[i]/(double)(((short*)(note_count))[i])*(((short*)(note_count))[i]-1)+(func(i*c)+add)*coeff/(double)((short*)(note_count))[i];
			}
		if(channels==2)
			for(int i=start;i<end;i++)
			{
				((short*)(note_count))[i]++;
				((char*)(samples))[i*2]=((char*)(samples))[i*2]/(double)(((short*)(note_count))[i])*(((short*)(note_count))[i]-1)+(func(i*c)+add)*coeff/(double)((short*)(note_count))[i];
				((char*)(samples))[i*2+1]=((char*)(samples))[i*2+1]/(double)(((short*)(note_count))[i])*(((short*)(note_count))[i]-1)+(func(i*c)+add)*coeff/(double)((short*)(note_count))[i];
			}
	}
	if(bits==16)
	{
		if(channels==1)
			for(int i=start;i<end;i++)
			{
				((short*)(note_count))[i]++;
				((short*)(samples))[i]=((short*)(samples))[i]/(double)(((short*)(note_count))[i])*(((short*)(note_count))[i]-1)+(func(i*c)+add)*coeff/(double)((short*)(note_count))[i];
			}
		if(channels==2)
			for(int i=start;i<end;i++)
			{
				((short*)(note_count))[i]++;
				((short*)(samples))[i*2]=((short*)(samples))[i*2]/(double)(((short*)(note_count))[i])*(((short*)(note_count))[i]-1)+(func(i*c)+add)*coeff/(double)((short*)(note_count))[i];
				((short*)(samples))[i*2+1]=((short*)(samples))[i*2+1]/(double)(((short*)(note_count))[i])*(((short*)(note_count))[i]-1)+(func(i*c)+add)*coeff/(double)((short*)(note_count))[i];
			}
	}
}
double note_to_hz(string note)
{
	static int tbl[]= {0,2,4,5,7,9,11};
	double base=261.6255653005986346778499935234;
	double ratio=1.0594630943592952645618252949463;
	if(note[0]>='C'&&note[0]<='G')
		base*=pow(ratio,tbl[note[0]-'C']);
	else
		base*=pow(ratio,tbl[((note[0]=='A')?5:6)]);
	if(note[1]=='#')
	{
		base*=ratio;
		base*=pow(2,atoi(note.substr(2).c_str()));
		return base;
	}
	else
	{
		base*=pow(2,atoi(note.substr(1).c_str()));
		return base;
	}
}
