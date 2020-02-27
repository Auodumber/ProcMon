#include <iostream>
#include <stdio.h>
#include <string.h>
#include "procmon.h"

using namespace std;

void displayhelp()
{
	cout<<"=============== Procmon(process monitor) =============================="<<endl;
	cout<<"ps          Display all process currently running "<<endl;
	cout<<"ps -t       Display all thread "<<endl;
	cout<<"ps -d       Display all dll files "<<endl;
	cout<<"memusg      Display memory usage of given process"<<endl;
	cout<<"log         Create log file "<<endl;
	cout<<"readlog     Read log file   "<<endl;
	cout<<"search      Search for Specific process "<<endl;
	cout<<"cls         clear screen "<<endl;
	cout<<"sysinfo     Hardware configration of system "<<endl;
	cout<<"help        menu "<<endl;
	cout<<"exit        EXIT "<<endl;
	cout<<"========================================================================="<<endl;
	cout<<"->"<<endl;
}

int main()
{
	cout<<"=============== Procmon(process monitor) =============================="<<endl;
	cout<<"ps          Display all process currently running "<<endl;
	cout<<"ps -t       Display all thread "<<endl;
	cout<<"ps -d       Display all dll files "<<endl;
	cout<<"memusg      Display memory usage of given process"<<endl;
	cout<<"log         Create log file "<<endl;
	cout<<"readlog     Read log file   "<<endl;
	cout<<"search      Search for Specific process "<<endl;
	cout<<"cls         clear screen "<<endl;
	cout<<"sysinfo     Hardware configration of system "<<endl;
	cout<<"help        menu "<<endl;
	cout<<"exit        EXIT "<<endl;
	cout<<"========================================================================="<<endl;
	cout<<"->"<<endl;
	char command[4][80],str[80];
	int count;
	processinfo *pobj = NULL;
	bool result;
   while(1)
   {
	   int day;
	   int month;
	   int hour;
	   int min;
	   fflush(stdin);
	   strcpy_s(str,"");
	   fgets(str,80,stdin);
	   count = sscanf(str,"%s %s %s %s",command[0],command[1],command[2],command[3]);
	  //cout<<"Count is: "<<count;
	   if(count == 1)
	   {
				if(strcmp(command[0],"ps") == 0)
				{
				pobj = new processinfo();
				pobj->processdisplay("-a");
				delete pobj;
				}
				else if(strcmp(command[0],"log") == 0)
				{
					pobj = new processinfo();
					pobj->createlog();
					delete pobj;
				}
				else if(strcmp(command[0],"readlog") == 0)
				{
					pobj = new processinfo();
					cout<<"enter file details"<<endl;
					cout<<"Day: ";cin>>day;
					cout<<"Month: ";cin>>month;
					cout<<"Hour " ;cin>>hour;
					cout<<"Minute: ";cin>>min;
					pobj->readlog(hour,min,day,month);
					delete pobj;
				}
				else if(_stricmp(command[0],"cls") == 0)
				{
				  system("cls");
				  continue;
				}
				else if(_stricmp(command[0],"exit") == 0)
				{
				  break;
				}
				else if(strcmp(command[0],"help") == 0)
				{
					 displayhelp();
				}
				 else if(_stricmp(command[0],"sysinfo") == 0)
				{
					bool reult = pobj->sysinfo();
					 
					if(reult == false)
					{
						cout<<"ERROR:Uanble Retrive System Information"<<endl;
					}
					delete pobj;
				}
				
	   }
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	   else if(count == 2)
	   {
			if(stricmp(command[0],"ps") == 0)
			{
				pobj = new processinfo();
				pobj->processdisplay(command[1]);
				delete pobj;
			}
			else if(_stricmp(command[0],"search") == 0)
			{
				pobj = new processinfo();
				pobj->search(command[1]);
				delete pobj;
			}
			else if(_stricmp(command[0],"kill") == 0)
			{
				  pobj = new processinfo();
				  result = pobj->killprocess(command[1]);
				    if(result == true)
				    {
					  cout<<command[1]<<" "<<"Terminated Successfully"<<endl;
				    }
				   else
				   {
					  cout<<"ERROR:there is no such process"<<endl;
				   }
				delete pobj;
			}
			else if(_stricmp(command[0],"memusg") == 0)
			{
				pobj = new processinfo();
			    bool bret = pobj->PrintMemoryInfo(command[1]);
				if(bret == false)
				{
					cout<<"ERROR:there is no such process"<<endl;
				}
				delete pobj;
			}
			
	  }
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	else
	{
		cout<<" wrong command "<<endl;
		cout<<"->"<<endl;
	}
}   
	   
	   
	    
return 0;
}
