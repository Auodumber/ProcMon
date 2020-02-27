#include "procmon.h"
 
//////////////////////////////////////////////////////////////////////////////////////////////////
//		Function    : TerminateMyProcess(of class processinfo)
//		Parameter   : DWORD,UINT
//		Return      : bool
//		Date	    : 25/11/2019
//		Author      : Auodumber
//		Discription : it is a helper function which takes processid kill the process and return if success
/////////////////////////////////////////////////////////////////////////////////////////////////////
bool processinfo::TerminateMyProcess(DWORD dwProcessId, UINT uExitCode)
{
	PID = dwProcessId;
	DWORD dwDesiredAccess = PROCESS_TERMINATE;
    BOOL  bInheritHandle  = FALSE;
	hProcess = OpenProcess(dwDesiredAccess,bInheritHandle,PID);
	if (hProcess == NULL)
	{
		cout<<"ERROR: there is no access to terminate"<<endl;
        return false;
	}
	BOOL result = TerminateProcess(hProcess, uExitCode);
    if( result == true)
		return true;
	else
		return false;
}

//////////////////////////////////////////////////////////////////////////////////////////////////
//		Function    : memusage(of class processinfo)
//		Parameter   : DWORD
//		Return      : none
//		Date	    : 25/11/2019
//		Author      : Auodumber
//		Discription : it is helper function to provide memory usageinformation and return if success
/////////////////////////////////////////////////////////////////////////////////////////////////////
bool processinfo::memusage(DWORD processID)
{
	PID = processID;
	PROCESS_MEMORY_COUNTERS pmc;
	printf( "\nProcess ID: %u\n", PID );

	hProcess = OpenProcess(  PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, processID );
	 if (NULL == hProcess)
	 {
		 cout<<"ERROR: Failed to open Process"<<endl;
		 return false;
	 }
		if(GetProcessMemoryInfo( hProcess, &pmc, sizeof(pmc)))
		{
			printf( "\tPageFaultCount: %d\n", pmc.PageFaultCount);
			printf( "\tPeakWorkingSetSize: %d\n", pmc.PeakWorkingSetSize/1024 );
			printf( "\tAmout of physical memory is in use by process(WorkingSet): %d KB\n", pmc.WorkingSetSize/1024 );
			printf( "\tQuotaPeakPagedPoolUsage: %d\n", pmc.QuotaPeakPagedPoolUsage/1024 );
			printf( "\tQuotaPagedPoolUsage: %d\n",pmc.QuotaPagedPoolUsage/1024 );
			printf( "\tQuotaPeakNonPagedPoolUsage: %d\n",pmc.QuotaPeakNonPagedPoolUsage/1024 );
			printf( "\tQuotaNonPagedPoolUsage: %d\n",pmc.QuotaNonPagedPoolUsage/1024  );
			printf( "\tAmout of VM reserved by OS for the process(Commit): %d KB\n", pmc.PagefileUsage/1024 ); 
			printf( "\tPeakPagefileUsage: %d\n", pmc.PeakPagefileUsage/1024  );
			 
		return true;
		}
		else 
		{
			return false;
		}
CloseHandle( hProcess );
} 

//////////////////////////////////////////////////////////////////////////////////////////////////
//		Function    : threadlog(of class processinfo)
//		Parameter   : unsigned int,int
//		Return      : none
//		Date	    : 25/11/2019
//		Author      : Auodumber
//		Discription : this function helps createlog function to add threadID's
/////////////////////////////////////////////////////////////////////////////////////////////////////
void threadinfo::threadlog( unsigned int top[][50],int i)
{
	 
		int j=0;
		
		if( !Thread32First( hThreadSnap, &te32 ) ) 
		 {
		 cout<<"unable to capture thread"<<endl;  // show cause of failure
		 CloseHandle( hThreadSnap );          // clean the snapshot object
			return;
		}
		do 
		{ 
			if( te32.th32OwnerProcessID == PID)
			{ 
				top[i][j] = te32.th32ThreadID;
				j++;	
			}
		} while( Thread32Next(hThreadSnap, &te32 ) ); 
		CloseHandle(hThreadSnap);
		 
} 

//////////////////////////////////////////////////////////////////////////////////////////////////
//		Function    : dlllog
//		Parameter   : 2D Array of char pointres,int
//		Return      : Done
//		Date	    : 25/11/2019
//		Author      : Auodumber
//		Discription : fuction yet to be devloped contains some bugs
/////////////////////////////////////////////////////////////////////////////////////////////////////
/*bool DLLinfo::dlllog(char *dup[][100],int i)
{
		int j = 0;
		char *arr;
	
		if( !Module32First( hModuleSnap, &me32 ) ) 
		{
		 cout<<"unable to capture module"<<endl;  // show cause of failure
		 CloseHandle( hModuleSnap );             // clean the snapshot object
		 return false;
		}
	  
	 do 
	 {
		arr = (char*)malloc(200*sizeof(char));
	    wcstombs_s(NULL,arr,200,me32.szModule,200);
		dup[i][j] = arr;
		//cout<<dup[i][j]<<endl;
	
	 }while(Module32Next( hModuleSnap, &me32 ));
	 cout<<"------------------------------------------------"<<endl;
	 CloseHandle( hModuleSnap );
	 return true;
}*/

