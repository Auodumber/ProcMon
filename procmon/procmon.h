#include <iostream>
#include <windows.h>
#include <tlhelp32.h>
#include <tchar.h>
#include <stdio.h>
#include <stdlib.h>
#include <cstring>
#include <io.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <tchar.h>
#include <process.h>
#include <fcntl.h>
#include <psapi.h>



using namespace std;
///////////////////////////////////////////////////////////////////////////
//	  Structure Name : createlog
//	  typedef		 : LOGFILE
//	  Discription    : structure for logfile
///////////////////////////////////////////////////////////////////////////

typedef struct createlog
{
	char pnname[200];
	unsigned int pid;
	unsigned int ppid;
	unsigned int thread_count;
	unsigned int threadsIDs[1000][50];
	char *dllid[100][100];
	
}LOGFILE;

/////////////////////////////////////////////////////////////////////////
//		ClassName  : threadinfo
//		date	   : 25/11/2019
//		auothor    : Auodumber
//		Discription: this class provides declaration of methods 
////////////////////////////////////////////////////////////////////////

class threadinfo
{
private:
		DWORD PID;
		HANDLE hThreadSnap;
		THREADENTRY32 te32;
		
public:
	unsigned int thrcnt;
	threadinfo(DWORD);
	void threaddisplay();
	void threadlog(unsigned int [][50],int);
};

/////////////////////////////////////////////////////////////////////////
//		ClassName  : DLLinfo
//		date	   : 25/11/2019
//		auothor    : Auodumber
//		Discription: this class provides declaration of methods 
////////////////////////////////////////////////////////////////////////
class DLLinfo
{
private:
	 DWORD PID;
	 HANDLE hModuleSnap;
	 MODULEENTRY32 me32;

public:
	 DLLinfo(DWORD);
	 void DLLdisplay();
	 bool dlllog(char *[][100],int);
	 
};

/////////////////////////////////////////////////////////////////////////
//		ClassName  : processinfo
//		date	   : 25/11/2019
//		auothor    : Auodumber
//		Discription: this class provides declaration of methods  and two helper function
//////////////////////////////////////////////////////////////////////////////////////////
class processinfo
{
		private:
			DWORD PID;
			threadinfo *tobj;		// object 
			DLLinfo *dobj;
			HANDLE hProcessSnap;
			HANDLE hProcess;
		    PROCESSENTRY32 pe32;
			PROCESS_MEMORY_COUNTERS_EX pmc;
			bool TerminateMyProcess(DWORD,UINT);  //helper function
			bool memusage(DWORD);				 //helper function

	   public:
		   processinfo();
		   void processdisplay(char *);
		   void threaddisplay();
		   void createlog();
		   void readlog(DWORD,DWORD,DWORD,DWORD);
		   void search(char *);
		   bool killprocess(char *);
		   bool sysinfo();
		   bool PrintMemoryInfo(char *);
};

