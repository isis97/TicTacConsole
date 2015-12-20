#include <windows.h>
#include <iostream>
#include <cstdlib>
#include <string.h>
#include <conio.h>
#include <vector>
#include <fstream>
#include <process.h>
#include <time.h>  
#include <algorithm>
#include <stdio.h>
#include <sstream>
#include "include/curl/curl.h"
#include "include/curl/easy.h"
#include "zlib/zlib.h"
#include "zlib/unzip.h"

#define ascii(code)  ((char)(code))
#define ascii_str(code) (str(ascii(code)))
#define inl inline
#define var auto
#define global static
#define YES true
#define NO false
#define cswitch(val) if(val==YES)
#define uint unsigned int
#define symid short
#define locale if(true)

#define HRS_TO_SEC(HRS) (HRS*3600)
#define MIN_UPDATE_INTERVAL HRS_TO_SEC(12)

using namespace std;

string getexepath() {
	char result[ MAX_PATH ];
	string path( result, GetModuleFileName( NULL, result, MAX_PATH ) );
	const int siz = path.size();
	string ret = "";
	string pom = "x";
	int it;
	for(it=siz-1;it>=0;--it) {
		if(path[it]=='\\') {
			break;
		} 
	}
	for(int j=0;j<=it;++j) {
		pom = "x";
		pom[0] = path[j];
		ret+=pom;
	}
	return ret;
}


string path = getexepath();//"C:\\Users\\Piotr\\Documents\\Projects\\Temporary\\SCGL - Copy\\";
string __VERSION_NAME = "";
float __VERSION = 0.0f;
int __VERSION_BUILD = 0000;

unsigned char * file_data_uc_buffer;

bool runapp = true;
int menu_state = 0;
bool update_menu_state = false;

bool is_game_made = false;


inline bool file_exists (const std::string& name) {
  	if (FILE *file = fopen(name.c_str(), "r")) {
        fclose(file);
        return true;
    } else {
        return false;
    }  
}

template <typename T> 
inline bool contains(vector<T> container, T el) {
	const int siz = container.size();
	for(int it=0;it<siz;++it) {
		if(container[it] == el) {
			return true;
		}
	}
	return false;
}

template <typename T>
inline vector<T> multiply_sets(vector<T> a, vector<T> b) {
	const int siz = a.size();
	vector<T> ret;
	for(int it=0;it<siz;++it) {
		if(contains(b, a[it])) {
			ret.push_back(a[it]);
		}
	}
	return ret;
}

inline string str(char ch) {
	string ret = "x";
	ret[0] = ch;
	return ret;
}

inline int min(int a, int b) {
	if(a<b) {
		return a;
	}
	return b;
}

BOOL WINAPI SetConsoleIcon(HICON hIcon) {
	typedef BOOL (WINAPI *PSetConsoleIcon)(HICON);
	static PSetConsoleIcon pSetConsoleIcon = NULL;
	if(pSetConsoleIcon == NULL)
		pSetConsoleIcon = (PSetConsoleIcon)GetProcAddress(GetModuleHandle(("kernel32")), "SetConsoleIcon");
	if(pSetConsoleIcon == NULL)
		return FALSE;
	return pSetConsoleIcon(hIcon);
}

#include "include/waccgl.h"




clock_t timer;

size_t write_data(void *ptr, size_t size, size_t nmemb, FILE *stream) {
    size_t written = fwrite(ptr, size, nmemb, stream);
    return written;
}

inline vector< pair<string,string> > parse_string_com(string com) {
	const int siz = com.size();
	short state = 0;
	bool blockstate = false;
	bool datablock = false;
	string val_buf = "";
	string key_buf = "";
	vector< pair<string,string> > ret;
	
	for(int it=0;it<siz;++it) {
		if(com[it]!='\r') {
			if(com[it]=='\n') {
				ret.push_back(make_pair(key_buf, val_buf));
				state=0;
				key_buf="";
				val_buf="";
				datablock=false;
				blockstate=false;
			} else {
				if(com[it]=='['||com[it]==']') {
					if(datablock) {
						datablock = false;
					} else {
						datablock = true;	
					}
				} else {
					if(com[it]=='('||com[it]==')') {
						if(blockstate) {
							blockstate = false;
						} else {
							blockstate = true;	
						}
					} else {
						if((!blockstate)) {
							if(com[it]=='=') {
								if(datablock) {
									if(state==0) {
										key_buf+=ascii_str(com[it]);
									} else {
										val_buf+=ascii_str(com[it]);
									}
								} else {
									state=1;
								}
							} else if(com[it]==',') {
								if(datablock) {
									if(state==0) {
										key_buf+=ascii_str(com[it]);
									} else {
										val_buf+=ascii_str(com[it]);
									}
								} else {
									ret.push_back(make_pair(key_buf, val_buf));
									state=0;
									key_buf="";
									val_buf="";
								}
							} else if(state==0) {
								key_buf+=ascii_str(com[it]);
							} else {
								val_buf+=ascii_str(com[it]);
							}
						}
					}
				}
			}
		}
	}
	ret.push_back(make_pair(key_buf, val_buf));
	return ret;
}




context con(test);
textframe fr(25,5,45,7,"?");

int ftp_progress(void *clientp, double dltotal, double dlnow, double ultotal, double ulnow) {

	clock_t t = clock() - timer;
	if(((float)t)/CLOCKS_PER_SEC >= 1.0f) {
		timer = clock();
		if(dlnow<=0||dltotal<=0) {
			return 0;
		}
		test.setCurPos(25,4);
		for(int it=0;it<20;++it) {
			if(it<((int)(dlnow*100/dltotal))/5) {
				test.setCurPos(25+it,4);
				test<<"X";
				test.flush();
			}
		}
		
		fr.setText( intToStr((int)(dlnow*100/dltotal))+"% \n- Downloading updates..." );
		con.draw();
		test.flush();
		
		return 0;
	}

	return 0;
}


void onExit( void )
{
    system("SCGL.exe");
    return;
}

inline void update_version();
string newversion_url = "";

inline void load_version_info() {
	string str = "";
	string str2 = "";
	ifstream loads(path+"version_.info");
	while(loads.good()&&(!loads.eof())) {
		getline(loads, str);
		str2 += str+"\n";
	}
	loads.close();
	
	vector< pair<string,string> > parsed_com = parse_string_com(string(str2));
	
	for(int it=0;it<parsed_com.size();++it) {
		if(parsed_com[it].first=="version_name") {
			__VERSION_NAME = parsed_com[it].second;
		}
		if(parsed_com[it].first=="version") {
			__VERSION = (float)strToDouble(parsed_com[it].second);
		}
		if(parsed_com[it].first=="version_build") {
			__VERSION_BUILD = strToInt(parsed_com[it].second);
		}
		if(parsed_com[it].first=="version_date") {
			
		}	
	}
}

inline string serializeTM(tm * ptr) {
	
	return intToStr(ptr->tm_sec)+
	":"+
	intToStr(ptr->tm_min)+
	":"+
	intToStr(ptr->tm_hour)+
	":"+
	intToStr(ptr->tm_mday)+
	":"+
	intToStr(ptr->tm_mon)+
	":"+
	intToStr(ptr->tm_year)+
	":"+
	intToStr(ptr->tm_wday)+
	":"+
	intToStr(ptr->tm_yday)+
	":"+
	intToStr(ptr->tm_isdst);
}

inline void deserializeTM(tm &ptr, string ser) {
	
	ptr.tm_sec = 0;
	ptr.tm_min = 0;
	ptr.tm_hour = 0;
	ptr.tm_mday = 0;
	ptr.tm_mon = 0;
	ptr.tm_year = 0;
	ptr.tm_wday = 0;
	ptr.tm_yday = 0;
	ptr.tm_isdst = 0;
	
	string buf = "";
	int section = 0;
	int siz = ser.size();
	for(int it=0;it<siz;++it) {
		cout<<ser[it];cout.flush();
		if(ser[it]==':') {
			switch(section) {
				case 0: ptr.tm_sec=strToInt(buf); break;
				case 1: ptr.tm_min=strToInt(buf); break;
				case 2: ptr.tm_hour=strToInt(buf); break;
				case 3: ptr.tm_mday=strToInt(buf); break;
				case 4: ptr.tm_mon=strToInt(buf); break;
				case 5: ptr.tm_year=strToInt(buf); break;
				case 6: ptr.tm_wday=strToInt(buf); break;
				case 7: ptr.tm_yday=strToInt(buf); break;
				case 8: ptr.tm_isdst=strToInt(buf); break;
			}
			buf="";
			++section;
		} else {
			buf+=ascii_str(ser[it]);
		}
	}
	switch(section) {
		case 0: ptr.tm_sec=strToInt(buf); break;
		case 1: ptr.tm_min=strToInt(buf); break;
		case 2: ptr.tm_hour=strToInt(buf); break;
		case 3: ptr.tm_mday=strToInt(buf); break;
		case 4: ptr.tm_mon=strToInt(buf); break;
		case 5: ptr.tm_year=strToInt(buf); break;
		case 6: ptr.tm_wday=strToInt(buf); break;
		case 7: ptr.tm_yday=strToInt(buf); break;
		case 8: ptr.tm_isdst=strToInt(buf); break;
	}
	
}


inline void check_updates() {
	
	HWND hwnd = GetConsoleWindow();
	/*HKEY hkSoftware;
	LONG result;
	result = RegOpenKeyEx( HKEY_CURRENT_USER, "software", 0, KEY_ALL_ACCESS, & hkSoftware );
	
	HKEY hkTest;
	DWORD dwDisp;
	
	result = RegCreateKeyEx( hkSoftware, "SCGLLauncher__", 0, NULL, REG_OPTION_NON_VOLATILE,
	KEY_ALL_ACCESS, NULL, & hkTest, & dwDisp );
	

	char buffer[50] = "";
	DWORD dwBufSize = 50;
	DWORD dwRegsz = REG_SZ;
	
	result = RegQueryValueEx( hkTest, "last_update_check_time", NULL, & dwRegsz,( LPBYTE ) buffer, & dwBufSize );
	
    tm ptr;
    tm * locptr;
    time_t czas;
    time(&czas);
    locptr = localtime(&czas);
	deserializeTM(ptr, string(buffer));
	
	time_t tcurrent;
	time_t tlast;
	
	tcurrent = mktime(locptr);
	tlast = mktime(&ptr);
	
	double last_updtime = difftime( tcurrent, tlast );
	
	
	if(last_updtime<MIN_UPDATE_INTERVAL) {
		return;
	}
	
	locale {
		
		time_t czas;
 	    struct tm * ptr;
   	    time(&czas);
   		ptr = localtime(&czas);
		
		string tosave = serializeTM(ptr);
		
		DWORD dwBufSize = 20;
		DWORD dwRegsz = REG_SZ;
		result = RegSetValueEx( hkTest, "last_update_check_time", 0, REG_SZ,( LPBYTE ) tosave.c_str(), lstrlen( tosave.c_str() ) + 1 );
	}*/
	
	scene main_scene(0,0);
	
	main_scene.add(fr);
	main_scene.show(true);
	
	con.add(main_scene);
	con.draw();
	test.flush();
	
	file_data_uc_buffer = new unsigned char[409600000];
	
	CURL *curl;
    FILE *fp;
    CURLcode res;
    string url = "https://dl.dropboxusercontent.com/s/prosmraojublq8l/version.info?token_hash=AAGz7pubYR9sz6iJ7Atnr0OqNtf3fiUMdOf3Tk5V4I5JxQ";
    
    curl = curl_easy_init();

	if(curl) {
    	fr.setText("Downloading file... \n");
    	con.draw();test.flush();
    	Sleep(1);
        fp = fopen((path+"downloads\\update.info").c_str(),"wb");
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0);
        timer = clock();
        res = curl_easy_perform(curl);
        curl_easy_cleanup(curl);
        
        fclose(fp);
    } else {
    	MessageBox( hwnd, "Failed to check updates. You must do it manually.", "Unknown Error", MB_ICONERROR );
    	return;
    }
	    
	string str = "";
	string str2 = "";
	ifstream loads(path+"downloads\\update.info");
	while(loads.good()&&(!loads.eof())) {
		getline(loads, str);
		str2 += str+"\n";
	}
	loads.close();
	
	string version_name = "";
	float version = 0;
	int version_build = 0;
	bool force_update = false;
	vector< pair<string,string> > parsed_com = parse_string_com(string(str2));
	
	for(int it=0;it<parsed_com.size();++it) {
		if(parsed_com[it].first=="version_name") {
			version_name = parsed_com[it].second;
		}
		if(parsed_com[it].first=="version") {
			version = (float)strToDouble(parsed_com[it].second);
		}
		if(parsed_com[it].first=="version_build") {
			version_build = strToInt(parsed_com[it].second);
		}
		if(parsed_com[it].first=="version_date") {
			
		}
		if(parsed_com[it].first=="url") {
			newversion_url = parsed_com[it].second;
		}
		if(parsed_com[it].first=="force_update") {
			if(parsed_com[it].second=="true") {
				force_update = true;
			} else {
				force_update = false;
			}
		}	
	}
	fr.addText("Update: ["+version_name+" : "+doubleToStr(version)+" : "+intToStr(version_build)+"]\n");
	con.draw();test.flush();

	if(version_name!=__VERSION_NAME||__VERSION<version||__VERSION_BUILD<version_build) {
		update_version();
	}
}



inline void update_version() {
	
	//file_data_uc_buffer = new unsigned char[409600];

	scene main_scene(0,0);
	
	main_scene.add(fr);
	main_scene.show(true);
	
	con.add(main_scene);
	con.draw();
	test.flush();
	
	file_data_uc_buffer = new unsigned char[409600000];
	
	CURL *curl;
    FILE *fp;
    CURLcode res;
    string url = newversion_url;//"https://dl.dropboxusercontent.com/s/ue3rztkji79nnhf/SCGL.zip?token_hash=AAEAEsMpzDCKLwcrzmKKbIL9HgNFJUwNfUACcOowBe3hDg";
    //string verurl = "https://dl.dropboxusercontent.com/s/prosmraojublq8l/version.info?token_hash=AAGz7pubYR9sz6iJ7Atnr0OqNtf3fiUMdOf3Tk5V4I5JxQ";
    
    curl = curl_easy_init();
    /*if(file_exists(path+"downloads\\scgl_download.zip")) {
    	fr.setText("File was downloaded.\n");
    	con.draw();test.flush();
	} else {*/
		if(curl) {
	    	fr.setText("Downloading file "+url+"\n");
	    	con.draw();test.flush();
	    	Sleep(1);
	        fp = fopen((path+"downloads\\scgl_download.zip").c_str(),"wb");
	        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
	        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
	        curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);
	        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0);
	        curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 0);
	        curl_easy_setopt(curl, CURLOPT_PROGRESSFUNCTION, ftp_progress);
	        timer = clock();
	        res = curl_easy_perform(curl);
	        curl_easy_cleanup(curl);
	        
	        fclose(fp);
	    } else {
	    	fr.setText("Cannot download file.\n");
	    	con.draw();test.flush();
	    	
	    	return;
	    }
	//}
	
	string version_name = "";
	float version = 0;
	int version_build = 0;
	bool force_update = false;
	int files_in_zip = 0;
	int made_files = 0;
	
	unzFile uf = unzOpen((path+"downloads\\scgl_download.zip").c_str());
	if (unzGoToFirstFile(uf) == UNZ_OK) {
		do {
			unz_file_info unzfi;
			char dos_fn[MAX_PATH];
			char* extrafield = (char*) malloc(unzfi.size_file_extra);
			if (extrafield == NULL) {
				fr.setText("Problem ocurred\n");
				con.draw();test.flush();
				return;
			}
			char* commentary = (char*) malloc(unzfi.size_file_comment);
			unzGetCurrentFileInfo(uf, &unzfi, dos_fn, MAX_PATH,
				extrafield, unzfi.size_file_extra,
				commentary, unzfi.size_file_comment);
			++files_in_zip;
		} while (unzGoToNextFile(uf) == UNZ_OK);
	} else {
		fr.setText("Problem reading first file\n");
		con.draw();test.flush();
	}
	unzGoToNextFile(uf);
	unzClose(uf);
	
	
	uf = unzOpen((path+"downloads\\scgl_download.zip").c_str());
	timer = clock();
	if (unzGoToFirstFile(uf) == UNZ_OK) {
		do {
			
			clock_t t = clock() - timer;
			if(((float)t)/CLOCKS_PER_SEC >= 1.0f) {
				if(files_in_zip>0&&made_files>0) {
					fr.setText("Unzipping "+intToStr((made_files*100)/(files_in_zip))+"%\n- Downloading updates...");
					con.draw();test.flush();
				}
				timer = clock();
			}
			++made_files;
			
			unz_file_info unzfi;
			char dos_fn[MAX_PATH];
			char* extrafield = (char*) malloc(unzfi.size_file_extra);
			if (extrafield == NULL) {
				fr.setText("[ERROR!] Problem ocurred\n");
				con.draw();test.flush();
				return;
			}

			char* commentary = (char*) malloc(unzfi.size_file_comment);
			unzGetCurrentFileInfo(uf, &unzfi, dos_fn, MAX_PATH,
				extrafield, unzfi.size_file_extra,
				commentary, unzfi.size_file_comment);

			unzOpenCurrentFile( uf );

        	int readBytes = unzReadCurrentFile(uf, file_data_uc_buffer, unzfi.uncompressed_size);
        	
			ofstream output_file(path+string(dos_fn), ofstream::binary);
			for(int it=0;it<unzfi.uncompressed_size;++it) {
				output_file<<file_data_uc_buffer[it];	
			}
			output_file.close();
			Sleep(1);
			
			unzCloseCurrentFile( uf );
			
		} while (unzGoToNextFile(uf) == UNZ_OK);
	} else {
		fr.setText("Problem reading first file\n");
		con.draw();test.flush();
	}
	unzGoToNextFile(uf);
	
	delete file_data_uc_buffer;
}

#include <tchar.h>
BOOL WINAPI SetConsoleFont(HANDLE hOutput, DWORD fontIndex) {
	typedef BOOL (WINAPI *PSetConsoleFont)(HANDLE, DWORD);
	static PSetConsoleFont pSetConsoleFont = NULL;

	if(pSetConsoleFont == NULL)
		pSetConsoleFont = (PSetConsoleFont)::GetProcAddress(::GetModuleHandle(_T("kernel32")), "SetConsoleFont");
	if(pSetConsoleFont == NULL) return FALSE;

	return pSetConsoleFont(hOutput, fontIndex);
}

int main( void ) {
	
	SetConsoleFont(GetStdHandle(STD_OUTPUT_HANDLE), 0);

	CONSOLE_SCREEN_BUFFER_INFO info;
	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &info);
	SMALL_RECT rc;
	rc.Left = rc.Top = 0;
	rc.Right = (short)(min(info.dwMaximumWindowSize.X, info.dwSize.X) - 1)/2+25;
	rc.Bottom = (short)(min(info.dwMaximumWindowSize.Y, info.dwSize.Y) - 1)/2;
	HWND hConsole = GetConsoleWindow();
	MoveWindow(hConsole, rc.Left+(rc.Right-25)*4, rc.Top+(rc.Bottom)*4, rc.Right, rc.Bottom, TRUE);
	SetConsoleWindowInfo(GetStdHandle(STD_OUTPUT_HANDLE), true, &rc);
	
	system("cls");
	cout.flush();
	
	check_updates();
	update_version();
	atexit(onExit);
	
	return 0;	
}
