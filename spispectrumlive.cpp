/*
 * Copyright (c) 2010-2016 Stephane Poirier
 *
 * stephane.poirier@oifii.org
 *
 * Stephane Poirier
 * 3532 rue Ste-Famille, #3
 * Montreal, QC, H2X 2L1
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

/*
	portaudio live spectrum analyser example

	2015may08, created by stephane.poirier@oifii.org (or spi@oifii.org)
			   from BASS spectrum analyser example
*/

#include <windows.h>
#include <stdio.h>
#include <math.h>
#include <malloc.h>
//#include "bass.h"
#include "portaudio.h"

#ifdef WIN32
#if PA_USE_ASIO
#include "pa_asio.h"
#endif
#endif

#include "resource.h"

#include <string>
#include <map>
using namespace std;

//fourier.h is the addon from audio programming book to the rfftw library,
//see audio programming book page 536 for details. in short, fourier.cpp
//wraps the rfftw by providing 2 functions: fft() and ifft().
//fourier.h also depends on libsndfile so it makes rfftw.lib depends on
//libsndfile for compiling (no need for linking it if you don't use it
//elsewhere).
//fft() function can only be called always with the same sample size N,
//this because within fft() implementation rfftw_create_plan() is called
//only once (the first time fft() is called).
#include <fourier.h> //in rfftw.lib (static library)

#include <ctime> //for random number initialization seed

#include "direct.h"

#define SAMPLE_RATE  (44100)
#define FRAMES_PER_BUFFER (1024)
#define NUM_CHANNELS    (1)
//#define NUM_CHANNELS    (2)

// Select sample format. 
#if 1
#define PA_SAMPLE_TYPE  paFloat32
typedef float SAMPLE;
#define SAMPLE_SILENCE  (0.0f)
#define PRINTF_S_FORMAT "%.8f"
#elif 1
#define PA_SAMPLE_TYPE  paInt16
typedef short SAMPLE;
#define SAMPLE_SILENCE  (0)
#define PRINTF_S_FORMAT "%d"
#elif 0
#define PA_SAMPLE_TYPE  paInt8
typedef char SAMPLE;
#define SAMPLE_SILENCE  (0)
#define PRINTF_S_FORMAT "%d"
#else
#define PA_SAMPLE_TYPE  paUInt8
typedef unsigned char SAMPLE;
#define SAMPLE_SILENCE  (128)
#define PRINTF_S_FORMAT "%d"
#endif

/*
//#define SPECWIDTH 368	// display width
//#define SPECHEIGHT 127	// height (changing requires palette adjustments too)
#define SPECWIDTH 1000	// display width
#define SPECHEIGHT 64	// height (changing requires palette adjustments too)
*/
int SPECWIDTH=500;	// display width
int SPECHEIGHT=250;	// display height 

BYTE global_alpha=200;

//char global_buffer[1024];
//string global_filename="testwav.wav";
//string global_devicename="";
map<string,int> global_devicemap;
//int global_deviceid=-1;

PaStream* global_stream;
PaStreamParameters global_inputParameters;
PaError global_err;
string global_audiodevicename="";
int global_inputAudioChannelSelectors[2];
PaAsioStreamInfo global_asioInputInfo;

FILE* pFILE= NULL;

float buf[2*1920*2]; //max 2 channels and 2 screens 1920x1080
float buf2[2*1920*2]; //max 2 channels and 2 screens 1920x1080
float fftbuf[2*1920*2]; //max 2 channels and 2 screens 1920x1080

string global_outputfoldername = "frames"; 

float global_fSecondsRecord; //negative for always recording
DWORD global_timer=0;
int global_x=200;
int global_y=200;

HWND win=NULL;
DWORD timer=0;

//DWORD chan;
//HRECORD chan;	// recording channel

HDC specdc=0;
HBITMAP specbmp=0;
BYTE *specbuf;

int specmode=0,specpos=0; // spectrum mode (and marker pos for 2nd mode)

//new parameters
string global_classname="BASS-Spectrum-Live-Frames";
string global_title="spispectrumlive-frames (click to toggle mode)";
string global_begin="begin.ahk";
string global_end="end.ahk";
int global_idcolorpalette=0;
int global_bands=20;

int global_frameid=0;
void saveframetobmpfile(HWND hwnd, HDC hdc)
{
    //HDC hdcScreen;
    HDC hdcWindow;
    HDC hdcMemDC = NULL;
    HBITMAP hbmScreen = NULL;
    BITMAP bmpScreen;

    // Retrieve the handle to a display device context for the client 
    // area of the window. 
    //hdcScreen = GetDC(NULL);
    hdcWindow = hdc; //hdcWindow = GetDC(g_hWnd);

    // Create a compatible DC which is used in a BitBlt from the window DC
    hdcMemDC = CreateCompatibleDC(hdcWindow); 
    if(!hdcMemDC)
    {
		return;
    }
    RECT rcClient;
    GetClientRect(hwnd, &rcClient); //GetClientRect(g_hWnd, &rcClient);
    hbmScreen = CreateCompatibleBitmap(hdcWindow, rcClient.right-rcClient.left, 
                                       rcClient.bottom-rcClient.top);
    if(!hbmScreen)
    {
		DeleteObject(hdcMemDC);
		return;
    }

    HBITMAP oldhbitmap = (HBITMAP)SelectObject(hdcMemDC,hbmScreen);

    if(!BitBlt(hdcMemDC, 
               0,0, 
               rcClient.right-rcClient.left, rcClient.bottom-rcClient.top, 
               hdcWindow, 
               0,0,
               SRCCOPY))
    {
		SelectObject(hdcMemDC, oldhbitmap);
		DeleteObject(hbmScreen);
		DeleteObject(hdcMemDC);
        return;
    }
	GetObject(hbmScreen,sizeof(BITMAP),&bmpScreen);

    BITMAPFILEHEADER   bmfHeader;    
    BITMAPINFOHEADER   bi;

    bi.biSize = sizeof(BITMAPINFOHEADER);    
    bi.biWidth = bmpScreen.bmWidth;    
    bi.biHeight = bmpScreen.bmHeight;  
    bi.biPlanes = 1;    
    bi.biBitCount = 32;    
    bi.biCompression = BI_RGB;    
    bi.biSizeImage = 0;  
    bi.biXPelsPerMeter = 0;    
    bi.biYPelsPerMeter = 0;    
    bi.biClrUsed = 0;    
    bi.biClrImportant = 0;

    DWORD dwBmpSize = ((bmpScreen.bmWidth * bi.biBitCount + 31) / 32) * 4 * 
                      bmpScreen.bmHeight;

    // Starting with 32-bit Windows, GlobalAlloc and LocalAlloc are implemented as wrapper functions that 
    // call HeapAlloc using a handle to the process's default heap. Therefore, GlobalAlloc and LocalAlloc 
    // have greater overhead than HeapAlloc.
    HANDLE hDIB = GlobalAlloc(GHND,dwBmpSize); 
    char *lpbitmap = (char *)GlobalLock(hDIB);    

    // Gets the "bits" from the bitmap and copies them into a buffer 
    // which is pointed to by lpbitmap.
    GetDIBits(hdcWindow, hbmScreen, 0,
              (UINT)bmpScreen.bmHeight,
              lpbitmap,
              (BITMAPINFO *)&bi, DIB_RGB_COLORS);

    // A file is created, this is where we will save the screen capture.
	string filename = global_outputfoldername;
	filename += "\\frame_";
	global_frameid++;
	char buf[256];
	sprintf(buf, "%06d", global_frameid);
	filename += buf;
	filename += ".bmp";
    HANDLE hFile = CreateFile(filename.c_str(),
                              GENERIC_WRITE,
                              0,
                              NULL,
                              CREATE_ALWAYS,
                              FILE_ATTRIBUTE_NORMAL, NULL);   

    // Add the size of the headers to the size of the bitmap to get the total file size
    DWORD dwSizeofDIB = dwBmpSize + sizeof(BITMAPFILEHEADER) + 
                        sizeof(BITMAPINFOHEADER);

    //Offset to where the actual bitmap bits start.
    bmfHeader.bfOffBits = (DWORD)sizeof(BITMAPFILEHEADER) + 
                          (DWORD)sizeof(BITMAPINFOHEADER); 

    //Size of the file
    bmfHeader.bfSize = dwSizeofDIB; 

    //bfType must always be BM for Bitmaps
    bmfHeader.bfType = 0x4D42; //BM   

    DWORD dwBytesWritten = 0;
    WriteFile(hFile, (LPSTR)&bmfHeader, sizeof(BITMAPFILEHEADER), &dwBytesWritten,  NULL);
    WriteFile(hFile, (LPSTR)&bi, sizeof(BITMAPINFOHEADER), &dwBytesWritten, NULL);
    WriteFile(hFile, (LPSTR)lpbitmap, dwBmpSize, &dwBytesWritten, NULL);

    //Unlock and Free the DIB from the heap
    GlobalUnlock(hDIB);    
    GlobalFree(hDIB);

    //Close the handle for the file that was created
    CloseHandle(hFile);

    //Clean up
	/*
done:
    DeleteObject(hbmScreen);
    DeleteObject(hdcMemDC);
    //ReleaseDC(NULL,hdcScreen);
    //ReleaseDC(g_hWnd,hdcWindow);
	*/
	SelectObject(hdcMemDC, oldhbitmap);
	DeleteObject(hbmScreen);
	DeleteObject(hdcMemDC);
    return;

}

bool SelectAudioDevice()
{
	const PaDeviceInfo* deviceInfo;
    int numDevices = Pa_GetDeviceCount();
    for( int i=0; i<numDevices; i++ )
    {
        deviceInfo = Pa_GetDeviceInfo( i );
		string devicenamestring = deviceInfo->name;
		global_devicemap.insert(pair<string,int>(devicenamestring,i));
		if(pFILE) fprintf(pFILE,"id=%d, name=%s\n", i, devicenamestring.c_str());
	}

	int deviceid = Pa_GetDefaultInputDevice(); // default input device 
	map<string,int>::iterator it;
	it = global_devicemap.find(global_audiodevicename);
	if(it!=global_devicemap.end())
	{
		deviceid = (*it).second;
		//printf("%s maps to %d\n", global_audiodevicename.c_str(), deviceid);
		deviceInfo = Pa_GetDeviceInfo(deviceid);
		//assert(inputAudioChannelSelectors[0]<deviceInfo->maxInputChannels);
		//assert(inputAudioChannelSelectors[1]<deviceInfo->maxInputChannels);
	}
	else
	{
		/*
		for(it=global_devicemap.begin(); it!=global_devicemap.end(); it++)
		{
			printf("%s maps to %d\n", (*it).first.c_str(), (*it).second);
		}
		*/
		//Pa_Terminate();
		//return -1;
		//printf("error, audio device not found, will use default\n");
		MessageBox(win,"error, audio device not found, will use default\n",0,0);
		deviceid = Pa_GetDefaultInputDevice();
	}


	global_inputParameters.device = deviceid; 
	if (global_inputParameters.device == paNoDevice) 
	{
		/*
		fprintf(stderr,"Error: No default input device.\n");
		//goto error;
		Pa_Terminate();
		fprintf( stderr, "An error occured while using the portaudio stream\n" );
		fprintf( stderr, "Error number: %d\n", global_err );
		fprintf( stderr, "Error message: %s\n", Pa_GetErrorText( global_err ) );
		return Terminate();
		*/
		MessageBox(win,"error, no default input device.\n",0,0);
		return false;
	}
	//global_inputParameters.channelCount = 2;
	global_inputParameters.channelCount = NUM_CHANNELS;
	global_inputParameters.sampleFormat =  PA_SAMPLE_TYPE;
	global_inputParameters.suggestedLatency = Pa_GetDeviceInfo( global_inputParameters.device )->defaultLowOutputLatency;
	//inputParameters.hostApiSpecificStreamInfo = NULL;

	//Use an ASIO specific structure. WARNING - this is not portable. 
	//PaAsioStreamInfo asioInputInfo;
	global_asioInputInfo.size = sizeof(PaAsioStreamInfo);
	global_asioInputInfo.hostApiType = paASIO;
	global_asioInputInfo.version = 1;
	global_asioInputInfo.flags = paAsioUseChannelSelectors;
	global_asioInputInfo.channelSelectors = global_inputAudioChannelSelectors;
	if(deviceid==Pa_GetDefaultInputDevice())
	{
		global_inputParameters.hostApiSpecificStreamInfo = NULL;
	}
	else if(Pa_GetHostApiInfo(Pa_GetDeviceInfo(deviceid)->hostApi)->type == paASIO) 
	{
		global_inputParameters.hostApiSpecificStreamInfo = &global_asioInputInfo;
	}
	else if(Pa_GetHostApiInfo(Pa_GetDeviceInfo(deviceid)->hostApi)->type == paWDMKS) 
	{
		global_inputParameters.hostApiSpecificStreamInfo = NULL;
	}
	else
	{
		//assert(false);
		global_inputParameters.hostApiSpecificStreamInfo = NULL;
	}
	return true;
}


/*
void CALLBACK StopPlayingFile(UINT uTimerID, UINT uMsg, DWORD dwUser, DWORD dw1, DWORD dw2)
{
	PostMessage(win, WM_DESTROY, 0, 0);
}
*/
void CALLBACK StopRecording(UINT uTimerID, UINT uMsg, DWORD dwUser, DWORD dw1, DWORD dw2)
{
	PostMessage(win, WM_DESTROY, 0, 0);
}

/*
// display error messages
void Error(const char *es)
{
	char mes[200];
	sprintf(mes,"%s\n(error code: %d)",es,BASS_ErrorGetCode());
	MessageBox(win,mes,0,0);
}
*/

/*
BOOL PlayFile(const char* filename)
{
	if (!(chan=BASS_StreamCreateFile(FALSE,filename,0,0,BASS_SAMPLE_LOOP))
		&& !(chan=BASS_MusicLoad(FALSE,filename,0,0,BASS_MUSIC_RAMP|BASS_SAMPLE_LOOP,1))) 
	{
		Error("Can't play file");
		return FALSE; // Can't load the file
	}
	if(global_fSecondsRecord<=0)
	{
		QWORD length_byte=BASS_ChannelGetLength(chan,BASS_POS_BYTE);
		global_fSecondsPlay=BASS_ChannelBytes2Seconds(chan,length_byte);
	}
	global_timer=timeSetEvent(global_fSecondsPlay*1000,25,(LPTIMECALLBACK)&StopPlayingFile,0,TIME_ONESHOT);

	BASS_ChannelPlay(chan,FALSE);
	return TRUE;
}
*/

/*
// select a file to play, and play it
BOOL PlayFile()
{
	char file[MAX_PATH]="";
	OPENFILENAME ofn={0};
	ofn.lStructSize=sizeof(ofn);
	ofn.hwndOwner=win;
	ofn.nMaxFile=MAX_PATH;
	ofn.lpstrFile=file;
	ofn.Flags=OFN_FILEMUSTEXIST|OFN_HIDEREADONLY|OFN_EXPLORER;
	ofn.lpstrTitle="Select a file to play";
	ofn.lpstrFilter="playable files\0*.mo3;*.xm;*.mod;*.s3m;*.it;*.mtm;*.umx;*.mp3;*.mp2;*.mp1;*.ogg;*.wav;*.aif\0All files\0*.*\0\0";
	if (!GetOpenFileName(&ofn)) return FALSE;
	
	return PlayFile(file);
}
*/

// update the spectrum display - the interesting bit :)
void CALLBACK UpdateSpectrum(UINT uTimerID, UINT uMsg, DWORD dwUser, DWORD dw1, DWORD dw2)
{
	HDC dc;
	int x,y,y1;

	if (specmode==3 || specmode==4 || specmode==5 || specmode==6 ||
		specmode==7 || specmode==8 || specmode==9 || specmode==10 ||
		specmode==11 || specmode==12 || specmode==13 || specmode==14 || 
		specmode==15 || specmode==16 || specmode==17 || specmode==18) 
	{ // waveform and filled waveform
		int c;
		//float *buf;
		
		//black background
		if(specmode==3 || specmode==4 || specmode==5 || specmode==6)
		{
			memset(specbuf,0,SPECWIDTH*SPECHEIGHT);
		}
		//noisy background
		else if(specmode==7 || specmode==8 || specmode==9 || specmode==10)
		{	
			for(int i=0; i<SPECWIDTH; i++)
			{
				for (int j=0; j<SPECHEIGHT; j++)
				{
					
					int random_integer;
					//int lowest=1, highest=127;
					int lowest=64, highest=96; //good
					int range=(highest-lowest)+1;
					random_integer = lowest+int(range*rand()/(RAND_MAX + 1.0));
					specbuf[j*SPECWIDTH+i]=random_integer;
				}
			}
		}
		//solid slightly shifting background
		else if(specmode==11 || specmode==12 || specmode==13 || specmode==14)
		{
			int random_integer;
			//int lowest=1, highest=127;
			//int lowest=1, highest=255;
			int lowest=64, highest=96; //good
			int range=(highest-lowest)+1;
			random_integer = lowest+int(range*rand()/(RAND_MAX + 1.0));
			memset(specbuf,random_integer,SPECWIDTH*SPECHEIGHT);
		}
		//solid radically shifting background
		else //specmode==15 || specmode==16 || specmode==17 || specmode==18
		{
			int random_integer;
			//int lowest=1, highest=127;
			int lowest=1, highest=255;
			//int lowest=64, highest=96; //good
			int range=(highest-lowest)+1;
			random_integer = lowest+int(range*rand()/(RAND_MAX + 1.0));
			memset(specbuf,random_integer,SPECWIDTH*SPECHEIGHT);
		}

		//global allocation from now on
		//buf=(float*)alloca(NUM_CHANNELS*SPECWIDTH*sizeof(float)); // allocate buffer for data

        //global_err = Pa_ReadStream( global_stream, buf, FRAMES_PER_BUFFER );
        global_err = Pa_ReadStream( global_stream, buf, NUM_CHANNELS*SPECWIDTH );
        if( global_err != paNoError ) 
		{
			/*
			char errorbuf[2048];
			sprintf(errorbuf, "Error reading stream: %s\n", Pa_GetErrorText(global_err));
			MessageBox(0,errorbuf,0,MB_ICONERROR);
			*/
			return;
		}

		//under waveform filled down to bottom
		if (specmode==6 || specmode==10 || specmode==14 || specmode==18)
		{
			for (c=0;c<NUM_CHANNELS;c++) 
			{
				for (x=0;x<SPECWIDTH;x++) 
				{
					int v=(1-buf[x*NUM_CHANNELS+c])*SPECHEIGHT/2; // invert and scale to fit display
					if (v<0) v=0;
					else if (v>=SPECHEIGHT) v=SPECHEIGHT-1;
					//under waveform filled down to bottom
					y=v;
					while(--y>=0) specbuf[y*SPECWIDTH+x]=c&1?127:1; 
				}
			}
		}
		//waveform filled towards center
		else if (specmode==5 || specmode==9 || specmode==13 || specmode==17)
		{
			for (c=0;c<NUM_CHANNELS;c++) 
			{
				for (x=0;x<SPECWIDTH;x++) 
				{
					int v=(1-buf[x*NUM_CHANNELS+c])*SPECHEIGHT/2; // invert and scale to fit display
					if (v<0) v=0;
					else if (v>=SPECHEIGHT) v=SPECHEIGHT-1;
					//waveform filled towards center
					y=v;
					if(y>(SPECHEIGHT/2))
						while(--y>=(SPECHEIGHT/2)) specbuf[y*SPECWIDTH+x]=c&1?127:1; 
					else if(y<(SPECHEIGHT/2))
						while(++y<=(SPECHEIGHT/2)) specbuf[y*SPECWIDTH+x]=c&1?127:1; 
					else specbuf[y*SPECWIDTH+x]=c&1?127:1; 
				}
			}
		}
		//waveform filled towards opposite
		else if (specmode==4 || specmode==8 || specmode==12 || specmode==16)
		{
			for (c=0;c<NUM_CHANNELS;c++) 
			{
				for (x=0;x<SPECWIDTH;x++) 
				{
					int v=(1-buf[x*NUM_CHANNELS+c])*SPECHEIGHT/2; // invert and scale to fit display
					if (v<0) v=0;
					else if (v>=SPECHEIGHT) v=SPECHEIGHT-1;
					//waveform filled towards opposite
					y=v;
					if(y>(SPECHEIGHT/2))
						while(--y>=(SPECHEIGHT/2-(v-(SPECHEIGHT/2)))) specbuf[y*SPECWIDTH+x]=c&1?127:1; 
					else if(y<(SPECHEIGHT/2))
						while(++y<=(SPECHEIGHT/2+((SPECHEIGHT/2)-v))) specbuf[y*SPECWIDTH+x]=c&1?127:1; 
					else specbuf[y*SPECWIDTH+x]=c&1?127:1; 
				}
			}
		}
		//waveform (original)
		else if(specmode==3 || specmode==7 || specmode==11 || specmode==15)
		{
			for (c=0;c<NUM_CHANNELS;c++) 
			{
				for (x=0;x<SPECWIDTH;x++) 
				{
					int v=(1-buf[x*NUM_CHANNELS+c])*SPECHEIGHT/2; // invert and scale to fit display
					if (v<0) v=0;
					else if (v>=SPECHEIGHT) v=SPECHEIGHT-1;
					if (!x) y=v;
					do 
					{ // draw line from previous sample...
						if (y<v) y++;
						else if (y>v) y--;
						specbuf[y*SPECWIDTH+x]=c&1?127:1; // left=green, right=red (could add more colours to palette for more chans)
					} while (y!=v);
				}
			}
		}
	} 
	else 
	{
		//float fftbuf[1024];
		//float *buf2;
		int numberofsamples=SPECWIDTH;
		//if(numberofsamples<1024) numberofsamples=1024;
		//buf2=(float*)alloca(NUM_CHANNELS*numberofsamples*sizeof(float)); // allocate buffer for data
        global_err = Pa_ReadStream( global_stream, buf2, NUM_CHANNELS*numberofsamples );
        if( global_err != paNoError ) 
		{
			/*
			char errorbuf[2048];
			sprintf(errorbuf, "Error reading stream (2): %s\n", Pa_GetErrorText(global_err));
			MessageBox(0,errorbuf,0,MB_ICONERROR);
			*/
			return;
		}
		/*
		for(int i=0; i<1024; i++)
		{
			fftbuf[i]=0.0f;
			//fftbuf[i]=abs(buf2[i]);
		}
		*/
		/*
		fft(buf2, fftbuf, 1024);
		for(int i=0; i<1024; i++)
		*/
		fft(buf2, fftbuf, numberofsamples);
		for(int i=0; i<numberofsamples; i++)
		{
			fftbuf[i]=abs(fftbuf[i]);
		}

		if (!specmode) 
		{ // "normal" FFT
			memset(specbuf,0,SPECWIDTH*SPECHEIGHT);
			for (x=0;x<SPECWIDTH/2;x++) 
			{
#if 1
				y=sqrt(fftbuf[x+1])*3*SPECHEIGHT-4; // scale it (sqrt to make low values more visible)
#else
				y=fftbuf[x+1]*10*SPECHEIGHT; // scale it (linearly)
#endif
				if (y>SPECHEIGHT) y=SPECHEIGHT; // cap it
				if (x && (y1=(y+y1)/2)) // interpolate from previous to make the display smoother
					//while (--y1>=0) specbuf[y1*SPECWIDTH+x*2-1]=y1+1;
					while (--y1>=0) specbuf[y1*SPECWIDTH+x*2-1]=(127*y1/SPECHEIGHT)+1;
				y1=y;
				//while (--y>=0) specbuf[y*SPECWIDTH+x*2]=y+1; // draw level
				while (--y>=0) specbuf[y*SPECWIDTH+x*2]=(127*y/SPECHEIGHT)+1; // draw level
			}
		} 
		else if (specmode==1) 
		{ // logarithmic, acumulate & average bins
			int b0=0;
			memset(specbuf,0,SPECWIDTH*SPECHEIGHT);
//#define BANDS 28
//#define BANDS 80
//#define BANDS 12
			for (x=0;x<global_bands;x++) 
			{
				float peak=0;
				int b1=pow(2,x*10.0/(global_bands-1));
				if (b1>1023) b1=1023;
				if (b1<=b0) b1=b0+1; // make sure it uses at least 1 FFT bin
				for (;b0<b1;b0++)
					if (peak<fftbuf[1+b0]) peak=fftbuf[1+b0];
				y=sqrt(peak)*3*SPECHEIGHT-4; // scale it (sqrt to make low values more visible)
				if (y>SPECHEIGHT) y=SPECHEIGHT; // cap it
				while (--y>=0)
					//memset(specbuf+y*SPECWIDTH+x*(SPECWIDTH/global_bands),y+1,SPECWIDTH/global_bands-2); // draw bar
					memset(specbuf+y*SPECWIDTH+x*(SPECWIDTH/global_bands),(127*y/SPECHEIGHT)+1,SPECWIDTH/global_bands-2); // draw bar
			}
		} 
		else 
		{ // "3D"
			for (x=0;x<SPECHEIGHT;x++) 
			{
				y=sqrt(fftbuf[x+1])*3*127; // scale it (sqrt to make low values more visible)
				if (y>127) y=127; // cap it
				specbuf[x*SPECWIDTH+specpos]=128+y; // plot it
			}
			// move marker onto next position
			specpos=(specpos+1)%SPECWIDTH;
			for (x=0;x<SPECHEIGHT;x++) specbuf[x*SPECWIDTH+specpos]=255;
		}
	}

	// update the display
	dc=GetDC(win);
	BitBlt(dc,0,0,SPECWIDTH,SPECHEIGHT,specdc,0,0,SRCCOPY);
	saveframetobmpfile(win, dc);
	ReleaseDC(win,dc);
}

/*
// Recording callback - not doing anything with the data
BOOL CALLBACK DuffRecording(HRECORD handle, const void *buffer, DWORD length, void *user)
{
	return TRUE; // continue recording
}
*/

// window procedure
long FAR PASCAL SpectrumWindowProc(HWND h, UINT m, WPARAM w, LPARAM l)
{
	switch (m) {
		case WM_PAINT:
			if (GetUpdateRect(h,0,0)) {
				PAINTSTRUCT p;
				HDC dc;
				if (!(dc=BeginPaint(h,&p))) return 0;
				BitBlt(dc,0,0,SPECWIDTH,SPECHEIGHT,specdc,0,0,SRCCOPY);
				EndPaint(h,&p);
			}
			return 0;

		case WM_LBUTTONUP:
			//specmode=(specmode+1)%4; // swap spectrum mode
			specmode=(specmode+1)%19; // swap spectrum mode
			memset(specbuf,0,SPECWIDTH*SPECHEIGHT);	// clear display
			return 0;

		case WM_RBUTTONUP:
			specmode=(specmode-1); // swap spectrum mode
			if(specmode<0) specmode = 19-1;
			memset(specbuf,0,SPECWIDTH*SPECHEIGHT);	// clear display
			return 0;

		case WM_CREATE:
			win=h;
			//spi, avril 2015, begin
			SetWindowLong(h, GWL_EXSTYLE, GetWindowLong(h, GWL_EXSTYLE) | WS_EX_LAYERED);
			SetLayeredWindowAttributes(h, 0, global_alpha, LWA_ALPHA);
			//SetLayeredWindowAttributes(h, 0, 200, LWA_ALPHA);
			//spi, avril 2015, end
			/*
			// initialize BASS
			if (!BASS_Init(-1,44100,0,win,NULL)) {
				Error("Can't initialize device");
				return -1;
			}
			//if (!PlayFile()) { // start a file playing
			if (!PlayFile(global_filename.c_str())) { // start a file playing
				BASS_Free();
				return -1;
			}
			*/

			/*
			// initialize BASS recording
			//if (!BASS_RecordInit(-1)) { //default device
			//if (!BASS_RecordInit(0)) { //device 0
			//if (!BASS_RecordInit(1)) { //device 1
			if (!BASS_RecordInit(global_deviceid)) {
				Error("Can't initialize device");
				return -1;
			}
			// start recording (44100hz mono 16-bit)
			if (!(chan=BASS_RecordStart(44100,1,0,&DuffRecording,0))) {
				Error("Can't start recording");
				return -1;
			}
			*/
			if(global_fSecondsRecord>0)
			{
				global_timer=timeSetEvent(global_fSecondsRecord*1000,100,(LPTIMECALLBACK)&StopRecording,0,TIME_ONESHOT);
			}
			{ // create bitmap to draw spectrum in (8 bit for easy updating)
				BYTE data[2000]={0};
				BITMAPINFOHEADER *bh=(BITMAPINFOHEADER*)data;
				RGBQUAD *pal=(RGBQUAD*)(data+sizeof(*bh));
				int a;
				bh->biSize=sizeof(*bh);
				bh->biWidth=SPECWIDTH;
				bh->biHeight=SPECHEIGHT; // upside down (line 0=bottom)
				bh->biPlanes=1;
				bh->biBitCount=8;
				bh->biClrUsed=bh->biClrImportant=256;
				// setup palette
				
				if(global_idcolorpalette==0)
				{
					//original palette, green shifting to red
					for (a=1;a<128;a++) {
						pal[a].rgbGreen=256-2*a;
						pal[a].rgbRed=2*a;
					}
					for (a=0;a<32;a++) {
						pal[128+a].rgbBlue=8*a;
						pal[128+32+a].rgbBlue=255;
						pal[128+32+a].rgbRed=8*a;
						pal[128+64+a].rgbRed=255;
						pal[128+64+a].rgbBlue=8*(31-a);
						pal[128+64+a].rgbGreen=8*a;
						pal[128+96+a].rgbRed=255;
						pal[128+96+a].rgbGreen=255;
						pal[128+96+a].rgbBlue=8*a;
					}
				}
				else if(global_idcolorpalette==1)
				{
					//altered palette, red shifting to green
					for (a=1;a<128;a++) {
						pal[a].rgbRed=256-2*a;
						pal[a].rgbGreen=2*a;
					}
					for (a=0;a<32;a++) {
						pal[128+a].rgbBlue=8*a;
						pal[128+32+a].rgbBlue=255;
						pal[128+32+a].rgbGreen=8*a;
						pal[128+64+a].rgbGreen=255;
						pal[128+64+a].rgbBlue=8*(31-a);
						pal[128+64+a].rgbRed=8*a;
						pal[128+96+a].rgbGreen=255;
						pal[128+96+a].rgbRed=255;
						pal[128+96+a].rgbBlue=8*a;
					}
				}
				else if(global_idcolorpalette==2)
				{
					//altered palette, blue shifting to green
					for (a=1;a<128;a++) {
						pal[a].rgbBlue=256-2*a;
						pal[a].rgbGreen=2*a;
					}
					for (a=0;a<32;a++) {
						pal[128+a].rgbBlue=8*a;
						pal[128+32+a].rgbRed=255;
						pal[128+32+a].rgbGreen=8*a;
						pal[128+64+a].rgbGreen=255;
						pal[128+64+a].rgbRed=8*(31-a);
						pal[128+64+a].rgbBlue=8*a;
						pal[128+96+a].rgbGreen=255;
						pal[128+96+a].rgbBlue=255;
						pal[128+96+a].rgbRed=8*a;
					}
				}
				else if(global_idcolorpalette==3)
				{
					//altered palette, black shifting to white - grascale
					for (a=1;a<256;a++) {
						pal[a].rgbRed=a;
						pal[a].rgbBlue=a;
						pal[a].rgbGreen=a;
					}
				}
				else if(global_idcolorpalette==4)
				{
					//altered palette, pink
					for (a=1;a<256;a++) {
						pal[a].rgbRed=255;
						pal[a].rgbBlue=255;
						pal[a].rgbGreen=a;
					}
				}
				else if(global_idcolorpalette==5)
				{
					//altered palette, yellow
					for (a=1;a<256;a++) {
						pal[a].rgbRed=255;
						pal[a].rgbBlue=a;
						pal[a].rgbGreen=255;
					}
				}
				else if(global_idcolorpalette==6)
				{
					//altered palette, cyan
					for (a=1;a<256;a++) {
						pal[a].rgbRed=a;
						pal[a].rgbBlue=255;
						pal[a].rgbGreen=255;
					}
				}
				else if(global_idcolorpalette==7)
				{
					//altered palette, lite green
					for (a=1;a<256;a++) {
						pal[a].rgbRed=a;
						pal[a].rgbBlue=127;
						pal[a].rgbGreen=255;
					}
				}


				// create the bitmap
				specbmp=CreateDIBSection(0,(BITMAPINFO*)bh,DIB_RGB_COLORS,(void**)&specbuf,NULL,0);
				specdc=CreateCompatibleDC(0);
				SelectObject(specdc,specbmp);
			}
			// setup update timer (40hz)
			timer=timeSetEvent(25,25,(LPTIMECALLBACK)&UpdateSpectrum,0,TIME_PERIODIC);
			//timer=timeSetEvent(10,25,(LPTIMECALLBACK)&UpdateSpectrum,0,TIME_PERIODIC);
			//timer=timeSetEvent(100,100,(LPTIMECALLBACK)&UpdateSpectrum,0,TIME_PERIODIC);
			break;

		case WM_DESTROY:
			{
				if (timer) timeKillEvent(timer);
				if (global_timer) timeKillEvent(global_timer);
				//BASS_Free();
				//BASS_RecordFree();
				Sleep(100);
				global_err = Pa_StopStream( global_stream );
				if( global_err != paNoError ) 
				{
					char errorbuf[2048];
					sprintf(errorbuf, "Error stoping stream: %s\n", Pa_GetErrorText(global_err));
					MessageBox(0,errorbuf,0,MB_ICONERROR);
					return 1;
				}
				global_err = Pa_CloseStream( global_stream );
				if( global_err != paNoError ) 
				{
					char errorbuf[2048];
					sprintf(errorbuf, "Error closing stream: %s\n", Pa_GetErrorText(global_err));
					MessageBox(0,errorbuf,0,MB_ICONERROR);
					return 1;
				}
				Pa_Terminate();
				if (specdc) DeleteDC(specdc);
				if (specbmp) DeleteObject(specbmp);

				int nShowCmd = false;
				//ShellExecuteA(NULL, "open", "end.bat", "", NULL, nShowCmd);
				//ShellExecuteA(NULL, "open", "end.ahk", "", NULL, nShowCmd);
				ShellExecuteA(NULL, "open", global_end.c_str(), "", NULL, nShowCmd);
				
				PostQuitMessage(0);
			}
			break;
	}
	return DefWindowProc(h, m, w, l);
}

PCHAR*
    CommandLineToArgvA(
        PCHAR CmdLine,
        int* _argc
        )
    {
        PCHAR* argv;
        PCHAR  _argv;
        ULONG   len;
        ULONG   argc;
        CHAR   a;
        ULONG   i, j;

        BOOLEAN  in_QM;
        BOOLEAN  in_TEXT;
        BOOLEAN  in_SPACE;

        len = strlen(CmdLine);
        i = ((len+2)/2)*sizeof(PVOID) + sizeof(PVOID);

        argv = (PCHAR*)GlobalAlloc(GMEM_FIXED,
            i + (len+2)*sizeof(CHAR));

        _argv = (PCHAR)(((PUCHAR)argv)+i);

        argc = 0;
        argv[argc] = _argv;
        in_QM = FALSE;
        in_TEXT = FALSE;
        in_SPACE = TRUE;
        i = 0;
        j = 0;

        while( a = CmdLine[i] ) {
            if(in_QM) {
                if(a == '\"') {
                    in_QM = FALSE;
                } else {
                    _argv[j] = a;
                    j++;
                }
            } else {
                switch(a) {
                case '\"':
                    in_QM = TRUE;
                    in_TEXT = TRUE;
                    if(in_SPACE) {
                        argv[argc] = _argv+j;
                        argc++;
                    }
                    in_SPACE = FALSE;
                    break;
                case ' ':
                case '\t':
                case '\n':
                case '\r':
                    if(in_TEXT) {
                        _argv[j] = '\0';
                        j++;
                    }
                    in_TEXT = FALSE;
                    in_SPACE = TRUE;
                    break;
                default:
                    in_TEXT = TRUE;
                    if(in_SPACE) {
                        argv[argc] = _argv+j;
                        argc++;
                    }
                    _argv[j] = a;
                    j++;
                    in_SPACE = FALSE;
                    break;
                }
            }
            i++;
        }
        _argv[j] = '\0';
        argv[argc] = NULL;

        (*_argc) = argc;
        return argv;
    }

int PASCAL WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,LPSTR lpCmdLine, int nCmdShow)
{
	int nShowCmd = false;

	//LPWSTR *szArgList;
	LPSTR *szArgList;
	int argCount;
	//szArgList = CommandLineToArgvW(GetCommandLineW(), &argCount);
	szArgList = CommandLineToArgvA(GetCommandLine(), &argCount);
	if (szArgList == NULL)
	{
		MessageBox(NULL, "Unable to parse command line", "Error", MB_OK);
		return 10;
	}
	global_audiodevicename="E-MU ASIO"; //"Wave (2- E-MU E-DSP Audio Proce"
	if(argCount>1)
	{
		//global_filename = szArgList[1]; 
		global_audiodevicename = szArgList[1]; 
	
		/*
		int ret = wcstombs ( global_buffer, szArgList[1], sizeof(global_buffer) );
		if (ret==sizeof(global_buffer)) global_buffer[sizeof(global_buffer)-1]='\0';
		global_filename = global_buffer; 
		*/
	}
	global_inputAudioChannelSelectors[0] = 0; // on emu patchmix ASIO device channel 1 (left)
	global_inputAudioChannelSelectors[1] = 1; // on emu patchmix ASIO device channel 2 (right)
	//global_inputAudioChannelSelectors[0] = 2; // on emu patchmix ASIO device channel 3 (left)
	//global_inputAudioChannelSelectors[1] = 3; // on emu patchmix ASIO device channel 4 (right)
	//global_inputAudioChannelSelectors[0] = 8; // on emu patchmix ASIO device channel 9 (left)
	//global_inputAudioChannelSelectors[1] = 9; // on emu patchmix ASIO device channel 10 (right)
	//global_inputAudioChannelSelectors[0] = 10; // on emu patchmix ASIO device channel 11 (left)
	//global_inputAudioChannelSelectors[1] = 11; // on emu patchmix ASIO device channel 12 (right)
	if(argCount>2)
	{
		global_inputAudioChannelSelectors[0]=atoi((LPCSTR)(szArgList[2])); //0 for first asio channel (left) or 2, 4, 6, etc.
	}
	if(argCount>3)
	{
		global_inputAudioChannelSelectors[1]=atoi((LPCSTR)(szArgList[3])); //1 for second asio channel (right) or 3, 5, 7, etc.
	}
	
	global_fSecondsRecord = -1.0; //negative for always recording
	if(argCount>4)
	{
		global_fSecondsRecord = atof((LPCSTR)(szArgList[4]));
	}
	if(argCount>5)
	{
		global_x = atoi((LPCSTR)(szArgList[5]));
	}
	if(argCount>6)
	{
		global_y = atoi((LPCSTR)(szArgList[6]));
	}
	if(argCount>7)
	{
		specmode = atoi((LPCSTR)(szArgList[7]));
	}
	if(argCount>8)
	{
		global_classname = szArgList[8]; 
	}
	if(argCount>9)
	{
		global_title = szArgList[9]; 
	}
	if(argCount>10)
	{
		global_begin = szArgList[10]; 
	}
	if(argCount>11)
	{
		global_end = szArgList[11]; 
	}
	if(argCount>12)
	{
		global_idcolorpalette = atoi((LPCSTR)(szArgList[12]));
	}
	if(argCount>13)
	{
		global_bands = atoi((LPCSTR)(szArgList[13]));
	}
	if(argCount>14)
	{
		SPECWIDTH = atoi((LPCSTR)(szArgList[14]));
	}
	if(argCount>15)
	{
		SPECHEIGHT = atoi((LPCSTR)(szArgList[15]));
	}
	if(argCount>16)
	{
		global_alpha = atoi(szArgList[16]);
	}
	LocalFree(szArgList);

	/*
	//map devicename onto id
	BASS_DEVICEINFO info;
	for (int iii=0; BASS_RecordGetDeviceInfo(iii, &info); iii++)
	{
		string devicenamestring = info.name;
		global_devicemap.insert(pair<string,int>(devicenamestring,iii));
	}
	map<string,int>::iterator it;
	it = global_devicemap.find(global_audiodevicename);
	if(it!=global_devicemap.end())
	{
		global_deviceid = (*it).second;
	}
	else
	{
		global_deviceid = -1;
	}
	*/

	//////////////////////////////////////////////
	//if don't exist, create dir for output frames 
	//////////////////////////////////////////////
	_mkdir(global_outputfoldername.c_str());

	//ShellExecuteA(NULL, "open", "begin.bat", "", NULL, nShowCmd);
	//ShellExecuteA(NULL, "open", "begin.ahk", "", NULL, nShowCmd);
	ShellExecuteA(NULL, "open", global_begin.c_str(), "", NULL, nShowCmd);

	//////////////////////////
	//initialize random number
	//////////////////////////
	srand((unsigned)time(0));

	///////////////////////
	//initialize port audio
	///////////////////////
    global_err = Pa_Initialize();
    if( global_err != paNoError )
	{
		MessageBox(0,"portaudio initialization failed",0,MB_ICONERROR);
		return 1;
	}

	////////////////////////
	//audio device selection
	////////////////////////
	pFILE = fopen("devices.txt","w");
	SelectAudioDevice();
	fclose(pFILE);
	pFILE=NULL;

	//////////////
    //setup stream  
	//////////////
    global_err = Pa_OpenStream(
        &global_stream,
        &global_inputParameters,
        NULL, // &outputParameters,
        SAMPLE_RATE,
        0,
        paClipOff,      // we won't output out of range samples so don't bother clipping them
        NULL, // no callback, use blocking API 
        NULL ); // no callback, so no callback userData
    if( global_err != paNoError ) 
	{
		char errorbuf[2048];
        sprintf(errorbuf, "Unable to open stream: %s\n", Pa_GetErrorText(global_err));
		MessageBox(0,errorbuf,0,MB_ICONERROR);
        return 1;
    }

	//////////////
    //start stream  
	//////////////
    global_err = Pa_StartStream( global_stream );
    if( global_err != paNoError ) 
	{
		char errorbuf[2048];
        sprintf(errorbuf, "Unable to start stream: %s\n", Pa_GetErrorText(global_err));
		MessageBox(0,errorbuf,0,MB_ICONERROR);
        return 1;
    }


	WNDCLASS wc={0};
    MSG msg;

	/*
	// check the correct BASS was loaded
	if (HIWORD(BASS_GetVersion())!=BASSVERSION) {
		MessageBox(0,"An incorrect version of BASS.DLL was loaded",0,MB_ICONERROR);
		return 0;
	}
	*/

	// register window class and create the window
	wc.lpfnWndProc = SpectrumWindowProc;
	wc.hInstance = hInstance;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON1)); //spi, added
	wc.lpszClassName = global_classname.c_str();
	if (!RegisterClass(&wc) || !CreateWindow(global_classname.c_str(),
			//"BASS spectrum example (click to toggle mode)",
			//"spispectrumplay (click to toggle mode)",
			global_title.c_str(),
			//WS_POPUPWINDOW|WS_VISIBLE, global_x, global_y,
			WS_POPUP|WS_VISIBLE, global_x, global_y,
			
			//WS_POPUPWINDOW|WS_CAPTION|WS_VISIBLE, global_x, global_y,
			//WS_POPUPWINDOW|WS_VISIBLE, 200, 200,
			SPECWIDTH,
			//SPECWIDTH+2*GetSystemMetrics(SM_CXDLGFRAME),
			SPECHEIGHT,
			//SPECHEIGHT+GetSystemMetrics(SM_CYCAPTION)+2*GetSystemMetrics(SM_CYDLGFRAME),
			NULL, NULL, hInstance, NULL)) 
	{
		//Error("Can't create window");
		MessageBox(0,"Can't create window",0,MB_ICONERROR);
		return 0;
	}
	ShowWindow(win, SW_SHOWNORMAL);

	while (GetMessage(&msg,NULL,0,0)>0) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return 0;
}
