#include<iostream>
#include<stdio.h>
#include<cstring>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<sys/stat.h>
#include<sys/types.h>
#include<dirent.h>
#include<fcntl.h>
#include<ftw.h>
using namespace std;

/*
Displays the path name of the current directory.
*/
void pwd() {
    char path[100];
    getcwd(path,100);//Get path
    cout<<"current directory: "<<path<<endl;
}

/*
List all directories and files in the specified directory name.
*/
bool list(string dir) {
    DIR* d = opendir(dir.c_str());
    if(d==NULL) {
        return false;
    } else {
	struct dirent *dirent;
        while(dirent=readdir(d)) {
	    cout<<endl;
	    cout<<"  "<<dirent->d_name<<"  "<<dirent->d_type<<"  "<<dirent->d_reclen<<endl;
	    cout<<endl;
	}
	closedir(d);
        return true;
    }
}

/*
Change Directory.
*/
bool changedir(string path) {
    if(chdir(path.c_str())==0) {
        return true;
    } else {
        return false;
    }
}

/*
New directory.
*/
bool makedir(string dir) {
    if(mkdir(dir.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH)==0) {
        return true;
    } else {
        return false;
    }
}

/*
Delete directory.
*/
bool deldir(string dir) {
    if(rmdir(dir.c_str())==0) {
        return true;
    } else {
        return false;
    }
}

/*
Rename a file or directory.
*/
bool rename(string lastname,string newname) {
    if(rename(lastname.c_str(),newname.c_str())==0) {
        return true;
    } else {
        return false;
    }
}

/*
Copy an existing file.
*/
bool copy(string existname,string newname) {
    int fo1,fo2;
    char buf[1024];
    fo1=open(existname.c_str(),O_RDONLY);
    if(fo1==-1) {
	
	return false;
    } else {
	fo2=open(newname.c_str(),O_RDONLY);
	if(fo2!=-1) {
	    int i;
	    cout<<"Overwrite original file??"<<endl;
	    cout<<"----1 is yes,not 1 is no.";
	    cin>>i;
	    if(i!=1) {
		newname+="(1)";
	    }
	    close(fo2);
	}
        fo2=open(newname.c_str(),O_WRONLY|O_CREAT,S_IRWXU);
	int size = read(fo1,buf,sizeof(buf));
	write(fo2,buf,size);
	close(fo1);
	close(fo2);
	return true;
    }
}

int num=0;
string file;
int fn(const char *fpath, const struct stat *st, int typeflag) {
    for(int i=strlen(fpath)-1,j=file.length()-1;;i--,j--) {
	if(j==-1&&fpath[i]=='/') {
	    cout<<"  "<<fpath;
	    if(typeflag==FTW_F) cout<<"   FILE"<<endl;
	    else if(typeflag==FTW_D) cout<<"  DIRECTORY"<<endl;
	    num++;
	    break;
	}
	if(fpath[i]=='/') break;
	if(j==-1) break;
	if(fpath[i]!=file[j]) {
	    break;
	} 
    }
    return 0;
}

/*
Find the specified file in the specified directory and its subdirectories,
And output the absolute path to the file.
*/
bool find(string dir,string filename) {
    file=filename;
    ftw(dir.c_str(),fn,500);
    if(num==0)
        return false;
    else
	return true;
}

/*
Displays menu
*/
void menu() {
    system("clear");
    cout<<"*********************************************"<<endl;
    cout<<"  1.List all directories and files in the specified directory name"<<endl;
    cout<<"  2.Change Directory"<<endl;
    cout<<"  3.New directory"<<endl;
    cout<<"  4.Delete directory"<<endl;
    cout<<"  5.Exit command interpreter."<<endl;
    cout<<"  6.Rename a file or directory"<<endl;
    cout<<"  7.Copy an existing file"<<endl;
    cout<<"  8.Find the specified file in the specified directory and its subdirectories,And output the absolute path to the file"<<endl;
    cout<<"*********************************************"<<endl;
    pwd();
    cout<<endl;
}

int main() {
    menu();
    string s;
    while(1) {
	cout<<"Please enter menu number:";
        cin>>s;
        if(s=="1") {//List all directories and files in the specified directory name.
	    menu();
            cout<<"Please enter a specified directory name:";
            string dir;
            cin>>dir;
            if(!list(dir)) {
 	        cout<<"Failed to open or do not exist in this directory!!"<<endl;
            }
        } else if(s=="2") {//Change Directory
	    menu();
	    cout<<"Please enter a specified directory name or path:";
	    string path;
	    cin>>path;
	    if(!changedir(path)) {
 	        cout<<"--->Failed to open or do not exist in this directory!!"<<endl;
            } else {
		cout<<"--->The current directory has been changed to "<<path<<endl;
	    }
        } else if(s=="3") {//New directory
	    menu();
	    cout<<"Please enter a new directory name:";
	    string dir;
	    cin>>dir;
	    if(!makedir(dir)) {
 	        cout<<"New directory failed!!"<<endl;
            } else {
		cout<<"--->Directory created successfully"<<endl;
   	    }
        } else if(s=="4") {//Delete directory
	    menu();
	    cout<<"Please enter a existing directory name:";
	    string dir;
	    cin>>dir;
	    if(!deldir(dir)) {
 	        cout<<"Directory name does not exist or delete failed!!"<<endl;
            } else {
		cout<<"--->Directory deleted successfully"<<endl;
	    }
        } else if(s=="5") {//Exit command interpreter
	    menu();
	    cout<<"Sign out!"<<endl;
	    return 0;
        } else if(s=="6") {//Rename a file or directory
	    menu();
	    string lastname,newname;
	    cout<<"Please enter an old directory name or file name:";
	    cin>>lastname;
	    cout<<"Please enter an new directory name or file name:";
	    cin>>newname;
	    if(!rename(lastname,newname)) {
		cout<<"Rename failed!!"<<endl;
    	    } else {
	        cout<<"--->Rename success"<<endl;
	    }
        } else if(s=="7") {//Copy an existing file
	    menu();
	    string existname,newname;
	    cout<<"Please enter an old directory name or file name:";
	    cin>>existname;
	    cout<<"Please enter an new directory name or file name:";
	    cin>>newname;
	    if(!copy(existname,newname)) {
		cout<<"Copy failed!!"<<endl;
    	    } else {
	        cout<<"--->Copy success"<<endl;
	    }
        } else if(s=="8") {//Find the specified file in the specified directory and its subdirectories,And output the absolute path to the file
	    menu();
	    string dir,filename;
	    cout<<"Please enter an directory name:";
	    cin>>dir;
	    cout<<"Please enter an directory name or file name:";
	    cin>>filename;
	    if(!find(dir,filename)) {
		cout<<"Find failure!!"<<endl;
    	    }
        } else {//error!!!!!!!!!!!!!!!
	    menu();
	    cout<<"Input error please re-enter!"<<endl;
	}
    }
    return 0;
}
