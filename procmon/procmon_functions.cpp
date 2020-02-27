#include "procmon.h";

using namespace std;
//////////////////////////////////////////////////////////////////////////////////////////////////
//		Function    : Constructor(Parameterised) (of class threadinfo)
//		parameter   : DWORD(unsigned long int)
//		date	    : 25/11/2019
//		author      : Auodumber
//		discription : take snapshot of threads associated with processes
/////////////////////////////////////////////////////////////////////////////////////////////////////

 threadinfo::threadinfo(DWORD dwPID)
{
	PID = dwPID;
	hThreadSnap = CreateToolhelp32Snapshot( TH32CS_SNAPTHREAD, 0 ); 
	if( hThreadSnap == INVALID_HANDLE_VALUE ) 
	{
		cout<<"invalid handle"<<endl;
		return;
	}
		 te32.dwSize = sizeof(THREADENTRY32);    //set size of structure
}
 
//////////////////////////////////////////////////////////////////////////////////////////////////
//		Function    : threaddisplay(of class threadinfo)
//		Parameter   : none
//		Return      : none
//		Date	    : 25/11/2019
//		Author      : Auodumber
//		Discription : Display threads ID Associated with process
/////////////////////////////////////////////////////////////////////////////////////////////////////
 void threadinfo::threaddisplay()
 {
		int i=1;
		if( !Thread32First( hThreadSnap, &te32 ) ) 
		 {
		 cout<<"unable to capture thread"<<endl;  // show cause of failure
		 CloseHandle( hThreadSnap );          // clean the snapshot object
			return;
		}
		cout<<"/////////////////////////////////////////////"<<endl;
	  do 
		{ 
			if( te32.th32OwnerProcessID == PID)
			{
			cout<<i <<"."<<"THREAD ID: "<<te32.th32ThreadID<<endl;
			i++;
		    }
		} while( Thread32Next(hThreadSnap, &te32 ) ); 
		cout<<"/////////////////////////////////////////////"<<endl;
	    cout<<"---------------------------------------------"<<endl;

		 CloseHandle( hThreadSnap );
  }
 
//////////////////////////////////////////////////////////////////////////////////////////////////
//		Function    : Constructor(parameterised)(of class DLLinfo)
//		Parameter   : DWORD(unsigned long int)(processid)
//		Return      : none
//		Date	    : 25/11/2019
//		Author      : Auodumber
//		Discription : take the snapshot of modules(DLL) associated with process
/////////////////////////////////////////////////////////////////////////////////////////////////////
 DLLinfo::DLLinfo(DWORD dwPID)
 {
   PID = dwPID;
   hModuleSnap = CreateToolhelp32Snapshot( TH32CS_SNAPMODULE|TH32CS_SNAPMODULE32, PID );
	if( hModuleSnap == INVALID_HANDLE_VALUE)
	 {
	 cout<<"INVALID_HANDLE_";
	 CloseHandle(hModuleSnap);
	 return;
	}
		//cout<<"snapshot of module created successfully";
		// Set the size of the structure before using it.
		me32.dwSize = sizeof( MODULEENTRY32 );
}
 
//////////////////////////////////////////////////////////////////////////////////////////////////
//		Function    : DLLdisplay(of class DLLinfo)
//		Parameter   : none
//		Return      : none
//		Date	    : 25/11/2019
//		Author      : Auodumber
//		Discription : Display's of modules(DLL) associated with process
/////////////////////////////////////////////////////////////////////////////////////////////////////
 void DLLinfo::DLLdisplay()
 {
		char arr[200];
		if( !Module32First( hModuleSnap, &me32 ) ) 
		{
		 cout<<"unable to capture module"<<endl;  // show cause of failure
		 cout<<"///////////////////////////////////////////////////////////"<<endl;
		 CloseHandle( hModuleSnap );          // clean the snapshot object
			return;
		}
	 cout<<"////////////////dependant DLL of this process////////////////"<<endl;
	 do 
	 {
	   wcstombs_s(NULL,arr,200,me32.szModule,200);
	   cout<<arr<<endl;
	 }while(Module32Next( hModuleSnap, &me32 ));
	 cout<<"---------------------------------------------"<<endl;

	  CloseHandle( hModuleSnap );
 }
 
//////////////////////////////////////////////////////////////////////////////////////////////////
//		Function    : Constructor(parameterised)(of class Process)
//		Parameter   : none
//		Return      : none
//		Date	    : 25/11/2019
//		Author      : Auodumber
//		Discription : take the snapshot of modules(DLL) process
/////////////////////////////////////////////////////////////////////////////////////////////////////
processinfo::processinfo()
{
		hProcessSnap = CreateToolhelp32Snapshot( TH32CS_SNAPPROCESS, 0 );
		if( hProcessSnap == INVALID_HANDLE_VALUE )
		{
			cout<<"unable to create snapshot running process"<<endl;
			  return;
		}
		pe32.dwSize = sizeof( PROCESSENTRY32 );
}

//////////////////////////////////////////////////////////////////////////////////////////////////
//		Function    : processdisplay(of class processinfo)
//		Parameter   : string
//		Return      : none
//		Date	    : 25/11/2019
//		Author      : Auodumber
//		Discription : displays process name,id,parent processid,no of threads
/////////////////////////////////////////////////////////////////////////////////////////////////////
void processinfo::processdisplay(char *option)
{
		char arr[200];
		if( !Process32First( hProcessSnap, &pe32 ) )
		{
			cout<<"ERROR: in finding first process"<<endl; // show cause of failure
			CloseHandle( hProcessSnap );				  // clean the snapshot object
			 return;
		 }
		
			  // Now walk the snapshot of processes, and
			 // display information about each process in turn
			do
			{
				wcstombs_s(NULL,arr,200,pe32.szExeFile,200);
				cout<<"PROCESS NAME: "<<arr<<endl;
				cout<<"PID: "<<pe32.th32ProcessID<<endl;
				cout<<"PPID: "<<pe32.th32ParentProcessID<<endl;
				cout<<"Count of thread's: "<<pe32.cntThreads<<endl;
				cout<<"---------------------------------------------"<<endl;
				if(stricmp(option,"-t") == 0)
				{
					tobj = new threadinfo(pe32.th32ProcessID);
					tobj->threaddisplay();
					delete tobj;
				}
				else if(stricmp(option,"-d") == 0)
				{
					dobj = new DLLinfo(pe32.th32ProcessID);
					dobj->DLLdisplay();
					delete dobj;
				}

			}while( Process32Next( hProcessSnap, &pe32 ));
			CloseHandle(hProcessSnap);		
}

//////////////////////////////////////////////////////////////////////////////////////////////////
//		Function    : createlog(of class processinfo)
//		Parameter   : none
//		Return      : none
//		Date	    : 25/11/2019
//		Author      : Auodumber
//		Discription : creates logfile which contains process name,id,ppid,thread count and threads id's
/////////////////////////////////////////////////////////////////////////////////////////////////////
void processinfo::createlog()
{

	unsigned int pop[1000][50] = {{0},{0}};
	int i=0,j=0;
	char *cpop[100][100];
	bool ret = false;
	char arr[200];
	char *montharr[] = {"JAN","FEB","MAR","APR","MAY","JUN","JUL","AUG","SEP","OCT","NOV","DEC"};
	char fname[50];
	SYSTEMTIME lt;
	GetLocalTime(&lt);
	LOGFILE fobj;
	FILE *fp;
	sprintf_s(fname,"D://procmon%02d_%02d_%02d_%s.log",lt.wHour,lt.wMinute,lt.wDay,montharr[lt.wMonth-1]);

	fp = fopen(fname,"wb");
	if(fp == NULL)
	{
		cout<<"unable to create logfile"<<endl;
		return;
	}
	
		if( !Process32First(hProcessSnap, &pe32))
		{
			cout<<"ERROR: in finding first process"<<endl; // show cause of failure
			CloseHandle( hProcessSnap );			// clean the snapshot object
			return;
		 }
		cout<<"wait..."<<endl;
		do
		{
			wcstombs_s(NULL,arr,200,pe32.szExeFile,200);
			strcpy_s(fobj.pnname,arr);
			fobj.pid = pe32.th32ProcessID;
			fobj.ppid = pe32.th32ParentProcessID;
			fobj.thread_count = pe32.cntThreads;
		

			tobj = new threadinfo(pe32.th32ProcessID);
			tobj->threadlog(pop,i);
			for(int j=0;j<=pe32.cntThreads;j++)
			{
				 
				fobj.threadsIDs[i][j] = pop[i][j];
				//cout<<fobj.threadsIDs[i][j]<<endl;
			}
			i++;
			
			//dll modules in log file needs to be implemented
			/*dobj = new DLLinfo(pe32.th32ProcessID);
			ret = dobj->dlllog(cpop,i);
					if(ret == true)
					{
						for(j = 0;j<no_ofthreads;j++)
						{
							cout<<cpop[i][j]<<endl;
						}
					  i++;
					}*/
		
		fwrite(&fobj,sizeof(fobj),1,fp);
		}while( Process32Next( hProcessSnap, &pe32 ));
		cout<<"log created successfully"<<endl;
		cout<<"File Path: D://procmon"<<lt.wHour<<"_"<<lt.wMinute<<"_"<<lt.wDay<<"_"<<montharr[lt.wMonth-1]<<".pmon"<<endl;
		CloseHandle(hProcessSnap);
		fclose(fp);
		delete tobj;
}

//////////////////////////////////////////////////////////////////////////////////////////////////
//		Function    : readlog(class processinfo)
//		Parameter   : DWORD,DWORD,DWORD,DWORD
//		Return      : none
//		Date	    : 25/11/2019
//		Author      : Auodumber
//		Discription : takes day month hour minute as parameter and read log file of that date
/////////////////////////////////////////////////////////////////////////////////////////////////////
void processinfo::readlog(DWORD hour,DWORD min,DWORD day,DWORD month)
{
	int i=0;
	char arr[512];
	char *montharr[] = {"JAN","FEB","MAR","APR","MAY","JUN","JUL","AUG","SEP","OCT","NOV","DEC"};
	char fname[50];
	LOGFILE robj;
	FILE *fp;
	sprintf_s(fname,"D://procmon%02d_%02d_%02d_%s.log",hour,min,day,montharr[month-1]);

	fp = fopen(fname,"rb");
	if(fp == NULL)
	{
		cout<<"unable to open logfile"<<endl;
		return;
	}
	while(fread(&robj,sizeof(robj),1,fp)>0)
	{
		cout<<"PROCESS NAME: "<<robj.pnname<<endl;
		cout<<"PID: "<<robj.pid<<endl;
		cout<<"PPID: "<<robj.ppid<<endl;
		cout<<"Thread Count: "<<robj.thread_count<<endl;
		cout<<"****************************************************"<<endl;
		for(int j=0;j<robj.thread_count;j++)
		{ 
			cout<<"TIS's: "<<robj.threadsIDs[i][j]<<endl;
		}
		i++;
	    cout<<"---------------------------------------------"<<endl;
		 
	}
fclose(fp);
}

//////////////////////////////////////////////////////////////////////////////////////////////////
//		Function    : search(of class processinfo)
//		Parameter   : string(char pointer)
//		Return      : none
//		Date	    : 25/11/2019
//		Author      : Auodumber
//		Discription : searches for given process
/////////////////////////////////////////////////////////////////////////////////////////////////////
void processinfo::search(char *name)
{
	    char arr[200];
		if( !Process32First( hProcessSnap, &pe32 ) )
		{
			cout<<"ERROR: in finding first process"<<endl; // show cause of failure
			CloseHandle( hProcessSnap );			// clean the snapshot object
			 return;
		 }
		
			// Now walk the snapshot of processes, and
			 // display information about each process in turn
			do
			{
				wcstombs_s(NULL,arr,200,pe32.szExeFile,200);
				if(_stricmp(arr,name) == 0)
				{
					cout<<"\nPROCESS NAME: \n"<<arr<<endl;
					cout<<"\nPID:\n"<<pe32.th32ProcessID<<endl;
					cout<<"\nPPID:\n"<<pe32.th32ParentProcessID<<endl;
					cout<<"\nCount of threads:\n"<<pe32.cntThreads<<endl;
					
					/*for associated DLL information*/
					dobj = new DLLinfo(pe32.th32ProcessID);
					dobj->DLLdisplay();
					delete dobj;
					
					break;
				}
			}while(Process32Next( hProcessSnap, &pe32 ));
}

//////////////////////////////////////////////////////////////////////////////////////////////////
//		Function    : killprocess(of class processinfo)
//		Parameter   : string (char pointer) "processname"
//		Return      : bool
//		Date	    : 25/11/2019
//		Author      : Auodumber
//		Discription : terminate given process with and return true if successed
/////////////////////////////////////////////////////////////////////////////////////////////////////
bool processinfo::killprocess(char *name)
{
		bool reuslt = false;
		char arr[200];
		if( !Process32First( hProcessSnap, &pe32 ) )
		{
			cout<<"ERROR: in finding first process"<<endl; // show cause of failure
			CloseHandle( hProcessSnap );			// clean the snapshot object
			 return false;
		 }
		
			// Now walk the snapshot of processes, and
			 // display information about each process in turn
			do
			{
				wcstombs_s(NULL,arr,200,pe32.szExeFile,200);
				if(_stricmp(arr,name) == 0)
				{
					  reuslt = TerminateMyProcess(pe32.th32ProcessID, 1);
					  break;
				}
			}while(Process32Next( hProcessSnap, &pe32 ));
 if(reuslt == true)
 {
	 return true;
 }
 else
 {
	 return false;
 }

 CloseHandle(hProcessSnap);
}

//////////////////////////////////////////////////////////////////////////////////////////////////
//		Function    : sysinfo(of class processinfo)
//		Parameter   : none
//		Return      : bool
//		Date	    : 25/11/2019
//		Author      : Auodumber
//		Discription : Display hardware information
/////////////////////////////////////////////////////////////////////////////////////////////////////
bool processinfo::sysinfo()
{
	 
	SYSTEM_INFO siSysInfo;
	// Copy the hardware information to the SYSTEM_INFO structure. 
 
	GetSystemInfo(&siSysInfo); 
   // Display the contents of the SYSTEM_INFO structure. 
	 
		printf("Hardware information: \n");  
		printf("  OEM ID: %u\n", siSysInfo.dwOemId);
		printf("  Number of processors: %u\n", siSysInfo.dwNumberOfProcessors); 
		printf("  Page size: %u\n", siSysInfo.dwPageSize); 
		printf("  Processor type: %u\n", siSysInfo.dwProcessorType); 
	   printf("  Minimum application address: %lx\n",siSysInfo.lpMinimumApplicationAddress); 
	printf("  Maximum application address: %lx\n", 
      siSysInfo.lpMaximumApplicationAddress); 
	printf("  Active processor mask: %u\n", 
      siSysInfo.dwActiveProcessorMask);
	return true;
}

//////////////////////////////////////////////////////////////////////////////////////////////////
//		Function    : PrintMemoryInfo(of class processinfo)
//		Parameter   : string (char pointer) "processname"
//		Return      : bool
//		Date	    : 25/11/2019
//		Author      : Auodumber
//		Discription : Display's memory info of given process and return true if successed
/////////////////////////////////////////////////////////////////////////////////////////////////////
bool processinfo::PrintMemoryInfo(char *name)
{
		char arr[200];
		bool result = false;
		if( !Process32First( hProcessSnap, &pe32 ) )
		{
			cout<<"ERROR: in finding first process"<<endl; // show cause of failure
			CloseHandle( hProcessSnap );			// clean the snapshot object
			return result;
		 }
			do
			{
				wcstombs_s(NULL,arr,200,pe32.szExeFile,200);
				if(_stricmp(arr,name) == 0)
				{
					result=memusage(pe32.th32ProcessID);
					result = true;
					break;
				}
			}while( Process32Next( hProcessSnap, &pe32 ));
			CloseHandle(hProcessSnap);	
			return result;
}

