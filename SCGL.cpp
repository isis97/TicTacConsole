/*
	Compiled with TDM-GCC 4.7.1: "-lm -std=gnu++0x -O2" options - see makefile
	This files are distributed under MIT license.
*/

#define WACCGL_NO_KERNEL_API

#define ALLOW_DEBUG
#define ALLOW_LOG
#include "include/headers.h"
#include "include/defines.h"
#include "switches.c"

/*
"Ka�da wystarczaj�co zaawansowana technologia jest nieodr��nialna od magii."
    - Arthur C. Clarke

- Code designed by Piotr Aleksander Styczynski

	*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=*
	|       _.-^^---....,,--                                                                 |
	|   _--        _         --_                                           __                |
	|  <          \_/           >)       _____ __                         /__/       __   _  |
	|  |       .--.O.--,         |      / ___// /___  ___________  __  ______  _____/ /__(_) |
	|   \._     \/   \/       _./       \__ \/ __/ / / / ___/_  / / / / / __ \/ ___/ //_/ /  |
	|      ```--. . , ; .--'''         ___/ / /_/ /_/ / /__  / /_/ /_/ / / / (__  ) ,< / /   |
	|            | |   |              /____/\__/\__, /\___/ /___/\__, /_/ /_/____/_/|_/_/    |
	|         .-=||  | |=-.                    /____/           /____/                       |
	|         `-=#$%&%$#=-'                                                                  |
	|            | ;  :|                                                                     |
	|      __.,-#%&$@%#&#~,.___                                                              |
	|                                                                                        |
	*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=*
*/

using namespace std;

#include "include/utils.h"
#include "include/global_vars.h"
#include "include/waccgl.h"

#ifdef ALLOW_LOG
ofstream LOGSTREAM("log.txt");
inline void log(const string message) {
	time_t t = time(0);   // get time now
	struct tm * now = localtime( & t );
	LOGSTREAM << "\n[" <<  (now->tm_year + 1900) << '-' << (now->tm_mon + 1) << '-' <<  now->tm_mday << "] " << message << "   ";
}
inline void log_result(const string result) {
	LOGSTREAM << ".......  "<<result;
}
inline void log_start() {
	LOGSTREAM << "=================NORMAL STARTUP=================\n";
}
inline void log_end() {
    LOGSTREAM << "=================NORMAL END=================\n";
	LOGSTREAM.close();
}
#else
#define log(X)
#define log_result(X)
#define log_start()
#define log_end()
#endif


context *mcont = nullptr;

typedef vector< pair<string,string> > comlog;


inline string getLogKey(comlog& log, string key_name) {
	const int len = log.size();
	for(int it=0;it<len;++it) {
		if(log[it].first == key_name) {
			return log[it].second;
		}
	}
	return "";
}

inline comlog parse_string_com(string com) {
    log("[WEB] Parse requested command = {"+com+"}");
	const int siz = com.size();
	short state = 0;
	bool blockstate = false;
	bool datablock = false;
	string val_buf = "";
	string key_buf = "";
	comlog ret;

	for(int it=0;it<siz;++it) {
		if(com[it]!='\r') {
			if(com[it]=='\n'||com[it]==';') {
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

inline void load_version_info() {
    log("Load version info TRIGGERED.");
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

#include "include/player_movement.h"
#include "include/enemies.h"
#include "include/sound.h"

void onExitUpdate(void) {
	 log("NORMAL TERMINATION WAS TRIGGERED (WITH AUTOUPDATE)");
	 log_end();
     system(string("start cmd /c \""+path+"update_script.bat\"").c_str());
     return;
}

void onExit(void) {
	 log("NORMAL TERMINATION WAS TRIGGERED (NORMAL 0 CODE)");
	 log_end();
    //system("SCGLLauncher.exe");
    return;
}


context con(test);
textframe fr(25,43,45,7,"?");
unsigned char * file_data_uc_buffer;

clock_t timer;

size_t write_data(void *ptr, size_t size, size_t nmemb, FILE *stream) {
    size_t written = fwrite(ptr, size, nmemb, stream);
    return written;
}

string newversion_url = "";
inline void update_version();


inline bool check_updates() {
    log("Check for updates...");

	//return false;

	HWND hwnd = GetConsoleWindow();
	if (CreateDirectory((path+"downloads").c_str(), NULL) ||
    ERROR_ALREADY_EXISTS == GetLastError()) {

    } else {
        MessageBox( hwnd, "Failed to access the directory. Check the permissions, because this application cannot work properly witout disc access.", "Directory Access Error", MB_ICONERROR );
    	log("DIRACCESS ERROR WHILE UPDATING");
		return false;
    }

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
    string url = (VERSION_CONTROL_FILE_URL);
		
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
    	log("CURL_EASY_INIT ERROR WHILE UPDATING");
		return false;
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

	if(version_name==""||version_build==0||version==0) {
		log_result("NO_VALID_UPDATE_ACCESSIBLE (NO_UPDATE) => GOOD");
		return false;
	}

	if(version_name!=__VERSION_NAME||__VERSION<version||__VERSION_BUILD<version_build) {
		log_result("UPDATE_AVAILABLE (UPDATE) => GOOD");
		update_version();
        return true;
	}

	log_result("ALREADY_NEWEST_UPDATE (NO_UDPATE) => GOOD");
	return false;
}

inline void drawEndGameCom(string text) {
	context tempcont(test);
	textframe fr(15,10,112,11,"");
	fr.setText(text);
	fr.draw(tempcont);
}


int fog_pos = 30;
bool to_menu = false;

inline void redraw() {
	test.clear();

	if(!to_menu) {
		--fog_pos;
		if(fog_pos<0) {
			fog_pos = 0;
		}
	}

	for(int y=0; y<ny; ++y) {
		for(int x=1;x<=nx;++x) {
			test.exec( 3+8*x, 1+6*y, [](console& src, int x, int y) {
				src<<" \xBA \xBA \n\xCD\xCE\xCD\xCE\xCD\n \xBA \xBA \n\xCD\xCE\xCD\xCE\xCD\n \xBA \xBA \n";
			});
		}
	}

	markfx = (markx+3-markx%3)/3;
	markfy = (marky-marky%3)/3;

	test.setCurPos(0,0);
	//test<<markx<<";"<<marky<<";\n";

	int itx = 0;
	int ity = 0;

	int lastmov = playerRegMove-1;
	if(lastmov<0) {
		lastmov = playersNum-1;
	}

	for(int it=0;it<levmap_size;++it) {
		if(levmap[it]!=0) {

			ity = (it-it%(nx*3))/(nx*3);
			itx = it%(nx*3);
			test.setCurPos((3+8*(itx+3-itx%3)/3)+(itx%3)*2, (1+6*(ity-ity%3)/3)+(ity%3)*2);
			test<<(char)levmap[it];
			if(lastlevmapasgn==it) {
				test.setColor((3+8*(itx+3-itx%3)/3)+(itx%3)*2, (1+6*(ity-ity%3)/3)+(ity%3)*2, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | BACKGROUND_GREEN );
			}
			//FOREGROUND_BLUE | FOREGROUND_GREEN |
		}
	}

	test.setCurPos((3+8*markfx)+(markx%3)*2, (1+6*markfy)+(marky%3)*2);
	test.setColor((3+8*markfx)+(markx%3)*2, (1+6*markfy)+(marky%3)*2,  FOREGROUND_BLUE | BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE);
	//test<<"X";

	context tempcont(test);
	textframe fr(70,1,32,20,"");
	textframe chatfr(5,20,80,12,"");

	string frtext = "";

	if(is_game_made) {
		frtext += "[GAME WAS FINISHED!]\n";
		if(is_game_made_with_draw) {
			frtext += "\nDraw\n";
		} else {
			frtext += "\nWinner: player ";

			int last = playerRegMove-1;
			if(last<0) {
				last = playersNum-1;
			}
			//playerAiReg[last]->marker

			frtext += ascii_str(playerAiReg[last]->marker);
			if(playerAiReg[last]->isUserEnemy()) {
				frtext += " (USER)";
			} else if(playerAiReg[last]->isNetEnemy()) {
				frtext += " (NET)";
			} else {
				frtext += " (AI)";
			}
			frtext += "\n";
		}

	}

	frtext += "It's "+ascii_str(playerAiReg[playerRegMove]->marker)+" turn";
	if(playerAiReg[playerRegMove]->isUserEnemy()) {
		frtext += " (USER)";
	} else if(playerAiReg[playerRegMove]->isNetEnemy()) {
		frtext += " (NET)";
	} else {
		frtext += " (AI)";
	}
	frtext+="\n\n";

	for(int it=playerRegMove-1;it>=playerRegMove-4;--it) {
		if(it<0) {
			frtext += "\n";
		}
	}

	for(int it=playerRegMove-1;it>=playerRegMove-4;--it) {
		if(it>=0) {
			frtext += "     " + intToStr((playerRegMove-1)-(it-1)) + ". player " + ascii_str(playerAiReg[it]->marker);
			if(playerAiReg[it]->isUserEnemy()) {
				frtext += " (USER)";
			} else if(playerAiReg[it]->isNetEnemy()) {
				frtext += " (NET)";
			} else {
				frtext += " (AI)";
			}
			frtext += "\n";
		}
	}

	frtext += " --> " + intToStr(playerRegMove+1) + ". [" + ascii_str(playerAiReg[playerRegMove]->marker) + "]";
	if(playerAiReg[playerRegMove]->isUserEnemy()) {
		frtext += " (USER)";
	} else if(playerAiReg[playerRegMove]->isNetEnemy()) {
		frtext += " (NET)";
	} else {
		frtext += " (AI)";
	}
	frtext+="\n";

	for(int it=playerRegMove+1;it<playerRegMove+4;++it) {
		if(it<playersNum) {
			frtext += "     " + intToStr(it+1) + ". player " + ascii_str(playerAiReg[it]->marker);
			if(playerAiReg[it]->isUserEnemy()) {
				frtext += " (USER)";
			} else if(playerAiReg[it]->isNetEnemy()) {
				frtext += " (NET)";
			} else {
				frtext += " (AI)";
			}
			frtext += "\n";
		}
	}

	frtext += "\n";
	frtext += "Move: " + intToStr(playerRegMove+1) + "/" + intToStr(playersNum) + " [";
	int roundpartfl = playerRegMove+1;
	if(playerAiReg[playerRegMove]->isUserEnemy()) {
		--roundpartfl;
	}
	const int roundpart = (roundpartfl*15/playersNum);
	for(int it=0;it<15;++it) {
		if(it<roundpart) {
			frtext += "#";
		} else {
			frtext += ".";
		}
	}
	frtext += "]";
	frtext += "\n";


	/*
	vector<aienemy*> playerAiReg;
	int playerRegMove = 0;
	int playersNum = 0;
	*/

	fr.setText(frtext);
	fr.draw(tempcont);

	//ifdef INCLUDE_CLIENT_H__
		if(is_net_game_client) {
			while(group_chat.size()>5) {
				group_chat.pop_front();
			}
			string group_chat_val = "";
			const int len = group_chat.size();
			for(int it=0;it<len;++it) {
				group_chat_val += group_chat[it] + "\n";
			}
			chatfr.setText("GROUP CHAT: (. to post)\n"+group_chat_val+"\n> "+combuf+"|");
			chatfr.draw(tempcont);
		} else if(is_net_game_server) {
			while(server_group_chat.size()>5) {
				server_group_chat.pop_front();
			}
			string group_chat_val = "";
			const int len = server_group_chat.size();
			for(int it=0;it<len;++it) {
				group_chat_val += server_group_chat[it] + "\n";
			}
			chatfr.setText("GROUP CHAT: (type . to post)\n"+group_chat_val+"\n> "+combuf+"|");
			chatfr.draw(tempcont);
		}

		if(is_net_game_server) {
			test.setCurPos(0, 0);
			test<<"Online players: "+intToStr(netclients)+" / "+intToStr(playerAiReg.size())+"     Local server \""+curiphostname+"\" IP "+curip;
			test.flush();
		}

	//endif


	int randchar = 0;
	const string arrchar[] = { "$", "#", "\\", "/", "@", "*" };
	if(fog_pos>1) {
		for(int y=0; y<fog_pos; ++y) {
			for(int x=1;x<=100;++x) {
				randchar = rand() % 6;
				test.setCurPos(x, y);
				test << arrchar[randchar];
			}
		}
	}

	if(to_menu) {
		if(fog_pos<30) {
			++fog_pos;
			Sleep(50);
			test.flush();
			cout.flush();
			redraw();
		} else {
			fog_pos = 0;
			menu_state = 1;
			update_menu_state = true;
			return;
		}
	} else {
		if(fog_pos>0) {
			Sleep(30);
			test.flush();
			cout.flush();
			redraw();
		}
	}

}



listbox ec_sl_enemies(23,5,45,10);

inline void defaultPlayerList() {
    log("Default player list TRIGGERED.");
	char mrk = rndNewEnemyMarker();

	ec_sl_enemies.addOption( string("User player {")+ascii_str(mrk)+"}" );
	putNewPlayer( bfs_s_selenemy_types_initializers[0] );
	playerAiReg[playerAiReg.size()-1]->marker = mrk;

	for(int it=0;it<3;++it) {
		mrk = rndNewEnemyMarker();
		ec_sl_enemies.addOption( string("Realistic enemy {")+ascii_str(mrk)+"}" );
		putNewPlayer( bfs_s_selenemy_types_initializers[5] );
		playerAiReg[playerAiReg.size()-1]->marker = mrk;
	}
}

vector< pair<int,int> > availableBoardSizes;
vector< pair<int,int> > availableBoardSizesEx;


inline void recreate_levmap() {
   log("Recreate levmap TRIGGERED.");
	delete levmap;
	levmap_size = (nx*3)*(ny*3);
	levmap = new short[(nx*3)*(ny*3)+7];
	for(int it=0;it<levmap_size;++it) {
		levmap[it] = 0;
	}
}


#include "include/server.h"
#include "include/client.h"

scene before_play_scene(0,0);
scene nullscene(0,0);


inline bool isServerRunning() {
	return runserver;
}


int main( void ) {
	srand (time(NULL));
	log_start();
	log("Entry point found, starts execution.");

	log("Enabling reversed video mode...");
	test.enableReversedVideoMode();
	//test.setMouseSensivityX(0.5777);
	//test.setMouseSensivityY(0.12775);
	log_result("DONE");

	log("Initializing game hardcoded functions...");
	availableBoardSizes.push_back(make_pair(3,3));
	availableBoardSizes.push_back(make_pair(2,2));
	availableBoardSizes.push_back(make_pair(4,4));

	availableBoardSizesEx.push_back(make_pair(5,5));
	availableBoardSizesEx.push_back(make_pair(1,1));
	availableBoardSizesEx.push_back(make_pair(2,3));
	availableBoardSizesEx.push_back(make_pair(3,1));
	availableBoardSizesEx.push_back(make_pair(3,4));
	log_result("OK");

	//system("CLS");
	//cout<< "STATE = "<< test.SetConsoleFontSize(4) << "\n";
	//cout<<"It's some piece of text.\nJust some text...";
	//cout<<"[FONT] : #"<<intToStr(24)<<"\n";
	//cout.flush();

	/*for(int it=5;it<100;++it) {
		SetConsoleFontSize(hn, it);
		system("cls");
		cout<<"It's some piece of text.\nJust some text...";
		cout.flush();
		Sleep(1000);
	}*/

	log("Loading version info...");
	load_version_info();
	log_result("DONE");
	log("Loading default player list...");
	defaultPlayerList();
	log_result("DONE");

	log("Initializing boards' models...");
	//int uknown_board_fields[9] = {-1, -1, -1, -1, -1, -1, -1, -1, -1};
	unknown_board.x = -999;
	unknown_board.y = -999;
	unknown_board.fields[0] = -1;
	unknown_board.fields[1] = -1;
	unknown_board.fields[2] = -1;
	unknown_board.fields[3] = -1;
	unknown_board.fields[4] = -1;
	unknown_board.fields[5] = -1;
	unknown_board.fields[6] = -1;
	unknown_board.fields[7] = -1;
	unknown_board.fields[8] = -1;
	log_result("OK");

	//system("chcp 850");
	//std::cout.imbue ( std::locale ( "greek" ) );

	#define APPLICATION_INSTANCE_MUTEX_NAME "{BA49C45E-B29A-4359-A07C-51B65B5571AD}"

	log("Selecting single instance application model (determine if there's another app instance somewhere else running)...");
  	cswitch(SWITCH_SINGLE_INSTANCE) {

	    HANDLE hMutexOneInstance(::CreateMutex( NULL, TRUE, APPLICATION_INSTANCE_MUTEX_NAME));
	    bool bAlreadyRunning((::GetLastError() == ERROR_ALREADY_EXISTS));
	    if (hMutexOneInstance == NULL || bAlreadyRunning) {
	    	if(hMutexOneInstance) {
	    		::ReleaseMutex(hMutexOneInstance);
	    		::CloseHandle(hMutexOneInstance);
	    	}
	    	log_result("INSTANCE_DETECTED(BAD)");
	    	log("Termination must occur.");
	    	PostQuitMessage( 0 );
	    	return 0;
	    }
	}
	log_result("NO_INSTANCE(OK)");

	log("Getting cursor info...");
    CONSOLE_CURSOR_INFO cci;
    cci.bVisible = false;
    cci.dwSize = 100;
    SetConsoleCursorInfo(
  		test.getHandle(),
  		&cci
	);
	log_result("OK");


	log("Obtain HWND and HINSTANCE / set icon...");
  	HWND hwndConsole = GetConsoleWindow();

  	HINSTANCE hInstance = (HINSTANCE)GetWindowLong(hwndConsole, -6);

	SetConsoleIcon(LoadIcon( hInstance , MAKEINTRESOURCE(32518)));
	log_result("OK");

	log("Create WACCGL context...");
	null_action = &null_action__;

	levmap_size = (nx*3)*(ny*3);
	levmap = new short[(nx*3)*(ny*3)+7];
	for(int it=0;it<levmap_size;++it) {
		levmap[it] = 0;
	}


	context cont(test);
	mcont = &cont;
	cont.setChangeAnimation(&chanim_slide);
	log_result("OK");

	log("Prepare splash window scene...");
	test.SetConsoleFontSize(6);
	//test.SetConsoleFont(0);

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

	ascimg splashimg(117, 35);
	splashimg.createFromFile("splash.txt");
	imgframe splash_image(1,1,&splashimg);

	context tempcont(test);
	splash_image.draw(tempcont);
	test.flush();
	cout.flush();

	test.setCurPos(5,35);
	test<<"LOADING...";

	test.setCurPos(9,36);
	test<<"                     ";


	test.setCurPos(14,40); test<<"[";
	test.setCurPos(116,40); test<<"]";
	for(int prog=0;prog<100;++prog) {
		test.setCurPos(15+prog,40);
		test<<".";
	}
	log_result("OK");
	log("Display splash.");
	cswitch(!SWITCH_SKIP_UPDATES) {
		for(int prog=0;prog<100;++prog) {
			if(prog==0) {
				test.setCurPos(9,36);
				log("Checking updates?");
				test<<"Checking updates...";
				bool good = check_updates();
				if(good) {
					getch();
					atexit(onExit);
					return 0;
				}
			}
			test.setCurPos(15+prog,40);
			test<<"#";
			test.flush();
			Sleep(10);
			test.setCurPos(9,36);
			test<<"   Running application...   ";

		}
	}
	log("STARTING CORE APPLICATION");

    Sleep(800);
	//test.SetConsoleFont(7);

	//SetConsoleCodePage( 869 );
	log("Set console CP and do init cls...");
	SetConsoleCP( 869 );
	SetConsoleOutputCP( 869 );

	system("cls");
	test.clear();
	test.clearScreen();
	cout.flush();
	test.init();

	//Sleep(4000);
	//test.clear();
	//test.clearScreen();

	test.SetConsoleFontSize( 15 );
	SetConsoleCP( 869 );
	SetConsoleOutputCP( 869 );
	log_result("DONE");

	log("Setup scenes.");

	log("Setup MULTIPLAYER_INFO scene...");
	scene multiplayer_info_scene(0,0);
	button multiplayer_info_scene_back(2,2,"Back to menu");
	button multiplayer_info_scene_runserver(12,10,"RUN SERVER!");
	button multiplayer_info_scene_runclient(30,10,"RUN CLIENT!");


	multiplayer_info_scene.add(multiplayer_info_scene_back);
	multiplayer_info_scene.add(multiplayer_info_scene_runserver);
	multiplayer_info_scene.add(multiplayer_info_scene_runclient);
	multiplayer_info_scene.show(false);
	cont.add(multiplayer_info_scene);


	assignAction(multiplayer_info_scene_back, {
		menu_state = 0;
		update_menu_state = true;
	});

	assignAction(multiplayer_info_scene_runserver, {
		runServer();
	});

	assignAction(multiplayer_info_scene_runclient, {
		is_net_game_client = true;
		menu_state = 99;
		fog_pos = 30;
		to_menu = false;
		update_menu_state = true;
		system("cls");
		runClient();
	});
	log_result("OK");

	log("Setup UPDATES_INFO scene...");
	scene updates_info_scene(0,0);
	string updinfo_sign =  string("--------------------------\nApplication version is:\n[version '")+__VERSION_NAME+"' no. "+doubleToStr(__VERSION)+", build #"+intToStr(__VERSION_BUILD)+"]\n";
	updinfo_sign += "\n";
	updinfo_sign += "Application checks updates every time when you run it,\nbut you can force it to do update, by\nusing button below.\n                  ;)";

	textframe updinfo_text(5,1,93,20, updinfo_sign);
	button uis_back(3,25,"back to menu");
	button uis_force_update(25,25,"force update");

	updates_info_scene.add(updinfo_text);
	updates_info_scene.add(uis_back);
	updates_info_scene.add(uis_force_update);
	cont.add(updates_info_scene);
	updates_info_scene.show(false);

	window uis_force_update_dlg(30,8,67,12);
	uis_force_update_dlg.setTopmost(true);

	textframe uis_force_update_dlg_textbox(1,1,65,6,"Do you sure you want to perform force update?\nForce update cannot affect your saved games or settings\nbut it will force application to download the latest version.\nWon't work if no internet connection is detected.");
	button uis_force_update_dlg_cancel(10,7,"Cancel");
	button uis_force_update_dlg_ok(20,7,"OK");

	window uis_force_update_dlg2(30,8,67,12);
	textframe uis_force_update_dlg_textbox2(1,1,65,6,"To update application please restart it.\nUpdates are managed automatically.");
	button uis_force_update_dlg_ok2(20,7,"Understood.");

	uis_force_update_dlg.add(uis_force_update_dlg_textbox);
	uis_force_update_dlg.add(uis_force_update_dlg_cancel);
	uis_force_update_dlg.add(uis_force_update_dlg_ok);

	uis_force_update_dlg2.add(uis_force_update_dlg_textbox2);
	uis_force_update_dlg2.add(uis_force_update_dlg_ok2);

	updates_info_scene.add(uis_force_update_dlg);


	assignid(uis_force_update_dlg);

	assignAction(uis_force_update_dlg_cancel, {
		window *parent = getActionParent(window);
		parent->close();
	});

	assignAction(uis_force_update_dlg_ok, {
		window *parent = getActionParent(window);
		//parent->close();
		//atexit(onExit);
		//runapp=false;
		check_updates();
	});

	assignAction(uis_back, {
		menu_state = 0;
		update_menu_state = true;
	});

	assignAction(uis_force_update, {
		scene* parent = getActionParent(scene);
		window *dlg = (window*) parent->find(getid(uis_force_update_dlg));
		dlg->open();
	});
	log_result("OK");

	log("Setup INFO scene...");
	scene info_scene(0,0);
	ascimg img_info(70,25);
	img_info.createFromFile("img.txt");
	imgframe info_image(5,11,&img_info);
	textframe info_text(5,1,93,10,string("[ver. ")+__VERSION_NAME+";"+doubleToStr(__VERSION)+";"+intToStr(__VERSION_BUILD)+"]\n"+"Application made using WACC-GL (Windows Ascii Code Console Graphic Library)\nmade by Styczynsky Digital Systems (Piotr Aleksander Styczynski).\n\nThis product is distributed under \"as is\" license. You can modify, use and share this\napplication and its code without using for commercial purposes (you cannot sell this\nproduct).\n      Styczynsky Digital Systems\n      ver. 1.1 (alpha)");
	//textframe info2_text//(5,25,93,4,"To return to main menu press ESC button.");
	button info_exit_button(5,35,"Back to main menu");
	info_scene.add(info_image);
	info_scene.add(info_text);
	//info_scene.add(info2_text);
	info_scene.add(info_exit_button);

	assignAction(info_exit_button, {
		menu_state = 0;
		update_menu_state = true;
	});

	cont.add(info_scene);
	info_scene.show(false);
	log_result("OK");

	log("Setup ENEMY_CHOOSE scene...");
	scene enemy_choose(0,0);
	button ec_b_back(34,2,"back to menu");
	button ec_b_playnewgame(55,2,"play new game");
	button ec_b_new_enemy(23,15,"new enemy...");
	button ec_b_remove(40,15,"remove");
	button ec_b_edit(50,15,"edit marker");
	button ec_b_editboard(75,15,"board options");
	button ec_b_runserver(5,30,"run local server");
	textframe ec_b_gameinfo(70,5,50,3, "TROLOLL!");
	ec_b_gameinfo.setText(string("")+"Board dimensions: "+intToStr(nx)+" x "+intToStr(ny));
	assignid(ec_b_gameinfo);

	window boardedit_dlg(30,8,55,17);
	boardedit_dlg.setTopmost(true);
	assignid(boardedit_dlg);
	button boardedit_dlg_ok(5,10,"Ok");
	button boardedit_dlg_cancel(13,10,"Cancel");

	vector<string> boardedit_dlg_sizesel_opt;

	const int sizbs = availableBoardSizes.size();
	for(int it=0;it<sizbs;++it) {
		boardedit_dlg_sizesel_opt.push_back(intToStr(availableBoardSizes[it].first)+"x"+intToStr(availableBoardSizes[it].second));
	}

	exlistbox boardedit_dlg_sizesel(1,1,30,5,boardedit_dlg_sizesel_opt);
	boardedit_dlg_sizesel.setDefaultOption(0);

	textframe boardedit_dlg_tip(2,13,45,3,"Please choose board dimensions.");

	checkbox boardedit_dlg_allownotstd(1,6,"Allow non standard sizes.");

	assignid(boardedit_dlg_sizesel);

	assignAction(boardedit_dlg_ok, {
		window *wnd = getActionParent(window);
		exlistbox *ths = (exlistbox*)(wnd->find(getid(boardedit_dlg_sizesel)));

		const int opt = ths->getSelectionNumber();
		if(opt!=-1) {
			const int sizbs = availableBoardSizes.size();
			const int sizbsx = availableBoardSizesEx.size();
			if(opt<sizbs) {
				nx = availableBoardSizes[opt].first;
				ny = availableBoardSizes[opt].second;
				recreate_levmap();
			} else {
				nx = availableBoardSizesEx[opt-sizbs].first;
				ny = availableBoardSizesEx[opt-sizbs].second;
				recreate_levmap();
			}
		}

		textframe *tf = (textframe*)((scene*)wnd->getParent())->find(getid(ec_b_gameinfo));
		tf->setText(string("")+"Board dimensions: "+intToStr(nx)+" x "+intToStr(ny));

		wnd->close();
	});

	assignAction(boardedit_dlg_cancel, {
		window *wnd = getActionParent(window);
		wnd->close();
	});

	assignAction(boardedit_dlg_allownotstd, {
		checkbox *ths = getActionSrc(checkbox);

		window *parent = getActionParent(window);
		exlistbox* elb = (exlistbox*)parent->find(getid(boardedit_dlg_sizesel));

		if(ths->isChecked()) {
			const int sizbsx = availableBoardSizesEx.size();
			for(int it=0;it<sizbsx;++it) {
				elb->addOption(intToStr(availableBoardSizesEx[it].first)+"x"+intToStr(availableBoardSizesEx[it].second));
			}
		} else {
			const int sizbsx = availableBoardSizesEx.size();
			for(int it=0;it<sizbsx;++it) {
				elb->removeOption(intToStr(availableBoardSizesEx[it].first)+"x"+intToStr(availableBoardSizesEx[it].second));
			}
			for(int it=0;it<sizbsx;++it) elb->prevOption();
		}
	});

	boardedit_dlg.add(boardedit_dlg_ok);
	boardedit_dlg.add(boardedit_dlg_cancel);
	boardedit_dlg.add(boardedit_dlg_allownotstd);
	boardedit_dlg.add(boardedit_dlg_sizesel);
	boardedit_dlg.add(boardedit_dlg_tip);

	/*assignAction(boardedit_dlg_ok, {
		window *parent = getActionParent(window);
		parent->close();
	});*/

	assignAction(ec_b_editboard, {

		scene *parent = getActionParent(scene);
		window *dlg = (window*) parent->find(getid(boardedit_dlg));

		//((exlistbox*)dlg->find(getid(boardedit_dlg_sizesel)))->setDefaultOption(0);
		dlg->open();

	});

	assignAction(ec_b_runserver, {
		runServer();
	});

	assignAction(ec_b_edit, {

		scene* parent = getActionParent(scene);

		if(((listbox*) parent->find(getid(ec_sl_enemies)))->optionsNumber() > 0) {
			//((textframe*) parent->find(getid(ec_info_text)))->setText(bfs_s_selenemy_types_descr[source->getSelectionNumber()]);
			listbox* lst = (listbox*)parent->find(getid(ec_sl_enemies));
			playerAiReg[lst->getSelectionNumber()]->editProperties();
		}

	});

	assignAction(ec_b_remove, {

		scene* parent = getActionParent(scene);

		if(((listbox*) parent->find(getid(ec_sl_enemies)))->optionsNumber() > 0) {
			//listbox* lst = (listbox*)parent->find(getid(ec_sl_enemies));
			//playerAiReg[lst->getSelectionNumber()]->editProperties();
			int index = ((listbox*) parent->find(getid(ec_sl_enemies)))->getSelectionNumber();
			((listbox*) parent->find(getid(ec_sl_enemies)))->removeOption(index);
			playerAiReg.erase(playerAiReg.begin()+index, playerAiReg.begin()+index+1);
			--playersNum;
		}

	});

	assignid(ec_sl_enemies);

	textframe ec_info_text(5,22,93,6,"...");
	assignid(ec_info_text);

	enemy_choose.add(ec_b_back);
	enemy_choose.add(ec_b_playnewgame);
	enemy_choose.add(ec_b_new_enemy);
	enemy_choose.add(ec_b_remove);
	enemy_choose.add(ec_b_edit);
	enemy_choose.add(ec_b_editboard);
	enemy_choose.add(ec_b_runserver);
	enemy_choose.add(ec_sl_enemies);
	enemy_choose.add(ec_info_text);
	enemy_choose.add(ec_b_gameinfo);

	enemy_choose_error.show(false);
	enemy_choose_error.setTopmost(true);
	enemy_choose_error.add(enemy_choose_error_abort);
	assignAction(enemy_choose_error_abort, {
		window *parent = getActionParent(window);
		parent->close();
	});
	enemy_choose_error.add(enemy_choose_error_textbox);
	enemy_choose.add(enemy_choose_error);



	enemy_choose_error2.show(false);
	enemy_choose_error2.setTopmost(true);
	enemy_choose_error2.add(enemy_choose_error2_abort);
	enemy_choose_error2.add(enemy_choose_error2_ok);
	assignAction(enemy_choose_error2_abort, {
		window *parent = getActionParent(window);
		parent->close();
	});
	assignAction(enemy_choose_error2_ok, {
		window *parent = getActionParent(window);
		parent->close();

		attacker_checks.clear();
		winner_checks.clear();
		playerRegMove = 0;

		markch = markchsel;
		is_game_made = false;
		is_game_made_with_draw = false;
		wait_each_ai_enemy = true;
		menu_state = 99;

		fog_pos = 30;
		to_menu = false;

		for(int it=0;it<levmap_size;++it) {
			levmap[it] = 0;
		}
		markx = marky = 0;
		update_menu_state = true;

		redraw();
	});
	enemy_choose_error2.add(enemy_choose_error2_textbox);
	enemy_choose.add(enemy_choose_error2);



	assignAction(enemy_edit_dlg_b_ok, {
		//enemy_choose_dlg_sel
		window* parent = getActionParent(window);
		scene* scn = (scene*)parent->getParent();
		listbox* enemy_log = (listbox*)scn->find(getid(ec_sl_enemies));
		listbox* sel_char = (listbox*)parent->find(getid(enemy_edit_dlg_selmarker));
		int index = enemy_log->getSelectionNumber();
		char mark = sel_char->getSelectedOption()[0];

		playerAiReg[index]->marker = mark;
		enemy_log->overrideOption(index, playerAiReg[index]->enemy_typename+" {"+ascii_str(mark)+"}");

		parent->close();

	});

	enemy_edit_dlg.add(enemy_edit_dlg_b_ok);
	assignid(enemy_edit_dlg_selmarker);
	enemy_edit_dlg.add(enemy_edit_dlg_selmarker);
	assignAction(enemy_edit_dlg_selmarker, {

	});
	enemy_edit_dlg.show(false);
	enemy_choose.add(enemy_edit_dlg);

	cont.add(enemy_choose);
	enemy_choose.show(false);

	window enemy_choose_dlg(30,8,55,7);
	enemy_choose_dlg.setTopmost(true);

	listbox enemy_choose_dlg_sel(19,1,35,5,bfs_s_selenemy_types);
	assignid(enemy_choose_dlg_sel);
	assignAction(enemy_choose_dlg_sel, {
		listbox* source = getActionSrc(listbox);
		scene* parent = (scene*) getActionParent(window)->getParent();
		((textframe*) parent->find(getid(ec_info_text)))->setText(bfs_s_selenemy_types_descr[source->getSelectionNumber()]);

	});

	button enemy_choose_dlg_abort(1,1,"abort");
	assignAction(enemy_choose_dlg_abort, {
		window* parent = getActionParent(window);
		parent->close();
	});

	button enemy_choose_dlg_ok(13,1,"ok");
	assignAction(enemy_choose_dlg_ok, {
		window* parent = getActionParent(window);
		parent->close();

		scene* parent_scene = (scene*) getActionParent(window)->getParent();
		char mrk = rndNewEnemyMarker();

		((listbox*) parent_scene->find(getid(ec_sl_enemies)))->addOption( ((listbox*)parent->find(getid(enemy_choose_dlg_sel)))->getSelectedOption()+" {"+ascii_str(mrk)+"}" );

		//removeAllPlayers(), registerPlayer(...)
		//registerPlayer();
		registerPlayer( bfs_s_selenemy_types_initializers[((listbox*)parent->find(getid(enemy_choose_dlg_sel)))->getSelectionNumber()]->factory_produceEnemy() );
		playerAiReg[playerAiReg.size()-1]->marker = mrk;


		/*cout<<"@ Player Ai reg:\n";
		for(int it=0;it<playersNum;++it) {
			cout<<"#PlayerAiReg: "<<(playerAiReg[it]->enemy_typename)<<"; marker="<<(playerAiReg[it]->marker)<<"\n";
		}
		cout<<"--- END ---\n";
		getch();*/

		//ec_sl_enemies

	});

	assignid(ec_b_back);
	assignid(enemy_choose_dlg);

	assignAction(ec_b_new_enemy, {
		scene *parent = getActionParent(scene);
		//button* but = (button*)parent->find(getid(ec_b_back));
		//but->setText("TROL!");
		window *dlg = (window*) parent->find(getid(enemy_choose_dlg));
		dlg->open();
		//dlg->setTopmost(true);
		//parent->focus(dlg);
		//dlg->open();
		//dlg->setTopmost(true);
		//parent->focus(dlg);
	});


	enemy_choose_dlg.add(enemy_choose_dlg_abort);
	enemy_choose_dlg.add(enemy_choose_dlg_ok);
	enemy_choose_dlg.add(enemy_choose_dlg_sel);
	//enemy_choose_dlg.add(enemy_choose_dlg_ok);

	enemy_choose.add(enemy_choose_dlg);
	enemy_choose_dlg.show(false);


	enemy_choose.add(boardedit_dlg);
	boardedit_dlg.show(false);

	//enemy_choose.focus(&enemy_choose_dlg);

	//selbox bfs_s_selchar(15,5,32,bfs_s_selchar_opt);
	//listbox bfs_s_selchar(15,8,15,10,32,bfs_s_selchar_opt);
	//bfs_s_selchar.setRoundedList(false);

	ascimg bfs_img = ascimg::fromFile("menu_background_play.txt");

	button bfs_b_back(55,5,"back to menu");
	button bfs_b_play(20,5,"play now");
	button bfs_b_resume(32,5,"resume");
	imgframe bfs_imgf_image(20, 15, &bfs_img);


	bfs_imgf_image.setFrameStyle(null_frame_style);
	bfs_imgf_image.show(true);
	bfs_b_resume.show(false);
	//before_play_scene.add(bfs_s_selchar);
	before_play_scene.add(bfs_b_play);
	before_play_scene.add(bfs_b_resume);
	before_play_scene.add(bfs_b_back);
	before_play_scene.add(bfs_imgf_image);
	cont.add(before_play_scene);
	before_play_scene.show(false);
	log_result("OK");


	log("Setup SETTINGS scene...");
	scene settings_scene(0,0);
	button settings_scene_save(55,20,"Save and return");
	checkbox settings_scene_play_music(6,6,"Play music...");
	checkbox settings_scene_debug_mode(6,9,"Use debug mode...");
	checkbox settings_scene_invert_colours(6,12,"Invert colours...");
	checkbox settings_scene_lower_mouse_sensivity(6,15,"Decrease mouse sensivity...");

	settings_scene_play_music.setState(setting_play_music);
	settings_scene_debug_mode.setState(setting_debug_mode);
	settings_scene_invert_colours.setState(setting_invert_colours);
	settings_scene_lower_mouse_sensivity.setState(setting_lower_mouse_sensivity);

	debug_confirm_dlg_textbox.setText("Are you sure you want to\nTURN ON DEBUG MODE. If you\nknow what you're doing there's no problem...");

	assignAction(debug_confirm_dlg_no,{
		setting_debug_mode = false;
		settings_scene_debug_mode.setState(setting_debug_mode);
		debug_confirm_dlg.close();
	});

	assignAction(debug_confirm_dlg_yes,{
		setting_debug_mode = true;
		settings_scene_debug_mode.setState(setting_debug_mode);
		debug_confirm_dlg.close();
	});

	debug_confirm_dlg.add(debug_confirm_dlg_textbox);
	debug_confirm_dlg.add(debug_confirm_dlg_yes);
	debug_confirm_dlg.add(debug_confirm_dlg_no);

	settings_scene.add(debug_confirm_dlg);


	assignAction(settings_scene_debug_mode, {
		if(!setting_debug_mode) {
			settings_scene_debug_mode.setState(!settings_scene_debug_mode.isChecked());
			debug_confirm_dlg.open();
		} else {
			setting_debug_mode = false;
		}
	});

	assignActionEx(settings_scene_save,&,{

		setting_lower_mouse_sensivity = settings_scene_lower_mouse_sensivity.isChecked();
		if(setting_lower_mouse_sensivity) {
			test.setMouseSensivityX(0.5777);
			test.setMouseSensivityY(0.12775);
		} else {
			test.setMouseSensivityX(1.0);
			test.setMouseSensivityY(1.0);
		}

		setting_invert_colours = settings_scene_invert_colours.isChecked();
		test.enableReversedVideoMode(setting_invert_colours);

		setting_play_music = settings_scene_play_music.isChecked();
		if(setting_play_music) {
			enableMusic();
		} else {
			disableMusic();
		}

		menu_state = 0;
		update_menu_state = true;
	});

	//settings_scene_back.show(true);
	settings_scene_save.show(true);
	settings_scene_play_music.show(true);
	settings_scene_debug_mode.show(true);
	settings_scene_invert_colours.show(true);
	settings_scene_lower_mouse_sensivity.show(true);

	//settings_scene.add(settings_scene_back);
	settings_scene.add(settings_scene_save);
	settings_scene.add(settings_scene_play_music);
	settings_scene.add(settings_scene_debug_mode);
	settings_scene.add(settings_scene_invert_colours);
	settings_scene.add(settings_scene_lower_mouse_sensivity);

	cont.add(settings_scene);
	log_result("OK");

	log("Setup MAIN scene...");
	scene main_scene(0,0);
	frame main_frame(1,1,100,35);
	textframe main_descrpt_box(1,37,100,3,"");
	main_descrpt_box.show(true);
	main_frame.setFrameStyle(default_frame_style);
	main_frame.setBackground('#');
	main_frame.show(true);

	button b_exit(5,5,"exit");
	button b_play(15,5,"let's play a game");
	button b_info(40,5,"info!");
	button b_updates(50,5,"updates");
	button b_multiplayer(70,5,"multiplayer");
	button b_settings(60,25,"Settings... ;)");
	ascimg img(70,25);
	img.createFromFile("img.txt");
	imgframe mainmanu_image(5,8,&img);

	ascimg static_mainmenu_img(70,25);
	static_mainmenu_img.createFromFile("static_img.txt");
	imgframe static_mainmanu_image(0,0,&static_mainmenu_img);
    static_mainmanu_image.show(true);
    static_mainmanu_image.setFrameStyle(null_frame_style);

	//ascimg menubg(70,25);
	//menubg.createFromFile("imgbg.txt");
	//imgframe mainmanu_bg(5,8,&menubg);

	//main_scene.add(mainmanu_bg);
	main_scene.add(main_frame);
	main_scene.add(static_mainmanu_image);
	main_scene.add(mainmanu_image);
	main_scene.add(b_exit);
	main_scene.add(b_play);
	main_scene.add(b_info);
	main_scene.add(b_updates);
	main_scene.add(b_multiplayer);
	main_scene.add(b_settings);
	main_scene.add(main_descrpt_box);
	//main_scene.add(mainmanu_image);
	cont.add(main_scene);
	main_scene.show(true);


	assignAction(b_exit, {
		runapp = false;
	});

	cont.draw();
	test.flush();
	//HANDLE hThread;
	//cswitch(SWITCH_PLAY_SOUND) enableMusic();
	enableMusic();
	if(!setting_play_music) {
		disableMusic();
	}

	mainmanu_image.show(false);
	cont.draw();
	test.flush();

	bool update = true;
	winner_checks.clear();

	cont.draw();
	test.flush();

	assignAction(ec_b_back, {
		menu_state = 0;
		update_menu_state = true;
	});

	assignAction(b_play, {
		//removeAllPlayers();
		menu_state = 1;
		update_menu_state = true;
	});

	assignAction(b_settings, {
		menu_state = 6;
		update_menu_state = true;
	});

	assignAction(b_info, {
		menu_state = 2;
		update_menu_state = true;
	});

	assignAction(b_updates, {
		menu_state = 4;
		update_menu_state = true;
	});

	assignAction(b_multiplayer, {
		menu_state = 5;
		update_menu_state = true;
	});
	//assignAction(bfs_s_selchar, {
		//selbox * srcn = getActionSrc(selbox);
		//markchsel = srcn->getSelectedOption()[0];
	//});

	assignAction(bfs_b_back, {
		menu_state = 0;
		update_menu_state = true;
	});

	assignAction(bfs_b_play, {
		menu_state = 3;
		update_menu_state = true;
	});

	/*assignAction(bfs_b_play, {
		playerAiReg.clear();
		attacker_checks.clear();
		winner_checks.clear();
		playerRegMove = 0;
		playersNum = 0;

		markch = markchsel;
		is_game_made = false;
		menu_state = 99;
		for(int it=0;it<levmap_size;++it) {
			levmap[it] = 0;
		}
		markx = marky = 0;
		update_menu_state = true;

		mainplayer.setMarker(markch);
		myenemy.setMarker('1');
		myenemy2.setMarker('2');
		myenemy3.setMarker('3');
		mainplayer.setGroup(PLAYER0_GROUP);
		myenemy.setGroup(PLAYERAI1_GROUP);
		myenemy2.setGroup(PLAYERAI2_GROUP);
		myenemy3.setGroup(PLAYERAI3_GROUP);

		registerPlayer(&mainplayer);
		cswitch(SWITCH_ACTIVATE_FIRST_SMART_ENEMY) registerPlayer(&myenemy);
		cswitch(SWITCH_ACTIVATE_SECOND_SMART_ENEMY) registerPlayer(&myenemy2);
		cswitch(SWITCH_ACTIVATE_THIRD_SMART_ENEMY) registerPlayer(&myenemy3);
	});*/

	//ec_b_playnewgame

	assignAction(ec_sl_enemies, {});

	assignAction(ec_b_playnewgame, {

		scene* parent_scene = getActionParent(scene);
		if(((listbox*) parent_scene->find(getid(ec_sl_enemies)))->optionsNumber() > 0) {

			if(((listbox*) parent_scene->find(getid(ec_sl_enemies)))->isAnyOptionContains("User player")==false) {
				enemy_choose_error2_textbox.setText("That's recommended to choose at least one user player to start new game.\nDo you wish to continue?");
				enemy_choose_error2.open();
			} else {
				attacker_checks.clear();
				winner_checks.clear();
				playerRegMove = 0;

				markch = markchsel;
				is_game_made = false;
				is_game_made_with_draw = false;
				wait_each_ai_enemy = true;
				menu_state = 99;

				fog_pos = 30;
				to_menu = false;

				for(int it=0;it<levmap_size;++it) {
					levmap[it] = 0;
				}
				markx = marky = 0;
				update_menu_state = true;
				redraw();
			}
		} else {
			enemy_choose_error_textbox.setText("You must choose at least one player to start the game!\nIt can be AI or user player.");
			enemy_choose_error.open();
		}
	});

	assignAction(bfs_b_resume, {
		menu_state = 99;
		fog_pos = 30;
		to_menu = false;
		update_menu_state = true;
	});
	log_result("OK");

	cswitch(SWITCH_SKIP_TO_GAME) menu_state = 99;
	cswitch(SWITCH_SKIP_TO_GAME) cont.showNothing();

	cont.showNothing();
	cont.showOnlyEx((void*)&main_scene);

	int local_board_dmx = 0;
	int local_board_dmy = 0;

	log("Run main app loop.");

	while(runapp) {

		char key = cont.readKeyboardInput();
		cont.clear();

		if(update_menu_state) {
			log("Menu state changed to menu_state="+intToStr(menu_state));
			switch(menu_state) {
				case 0: cont.showOnly((void*)&main_scene); break;
				case 1: cont.showOnly((void*)&before_play_scene); break;
				case 2: cont.showOnly((void*)&info_scene); break;
				case 3: cont.showOnly((void*)&enemy_choose); break;
				case 4: cont.showOnly((void*)&updates_info_scene); break;
				case 5: cont.showOnly((void*)&multiplayer_info_scene); break;
				case 6: cont.showOnly((void*)&settings_scene); break;
				case 99: cont.showNothing(); break;
				//default: cont.showNothing(); break;
			}
		}


		cont.processKeyInput(true);
		if(!update_menu_state) {
			log("KEY_ACTION: Activated key="+intToStr(key));
			if(key==27) {
				//cont.processKeyInput(false);
				if(menu_state==99) {

					fog_pos = 0;
					to_menu = true;
					if(is_net_game_client) {
						exitClient();
					}
					if(is_game_made||is_net_game_client) {
						bfs_b_resume.show(false);
					} else {
						bfs_b_resume.show(true);
					}
				} else /*if(menu_state==2) {
					menu_state = 0;
					update_menu_state = true;
					cont.showOnly((void*)&main_scene); break;
				} else*/ {
 		            atexit(onExit);
			        exit(0);
					return 0;
				}
			} else {
				//cont.processKeyInput(true);
			}
		} else {
			update_menu_state=false;
		}

		/*
		vector<char> playerMoveRegistry;
		vector<enemy*> playerAiReg;
		int playerRegMove;
		int playersNum;
		playerAiReg[playerRegMove].marker
		*/

		cont.update(key);
		bool forceupdate_key = false;

		switch(menu_state) {
			case 99:
				forceupdate_key = false;
				#ifdef INCLUDE_CLIENT_H__
				if(runserver) {
					//can_client_move = false;
					if(key=='.') {
						string pom = server::serverInterpretCommand(combuf);
						if(pom!="") server_group_chat.push_back( "[SERVER/COM] "+pom );
						combuf = "";
					} else {
						if(key!=0x00&&((key>='a'&&key<='z')||(key>='0'&&key<='9')||(key=='.'||key=='`'||key==' '||key==','||key=='.'||key==';'||key==8))) {
							if(key==8) {
								if(combuf.size()>0) combuf = combuf.substr(0, combuf.size()-1);
							} else {
								combuf += key;
							}
							forceupdate_key = true;
							redraw();
						}
					}

				} else if(!is_net_game_client) {
					can_client_move = true;
				} else {

					if(key=='.') {
						clientPostMessage(combuf);
						combuf = "";
					} else {
						if(key!=0x00) {
							if(key==8) {
								if(combuf.size()>0) combuf = combuf.substr(0, combuf.size()-1);
							} else {
								combuf += key;
							}
							redraw();
							test.flush();
							cout.flush();
						}
					}

					clientStep();
				}
				#endif
				//test.blockMouse(true);
				redraw();
				update = true;
				if(test.isMouseEnabled()) {

					//markfx = (markx+3-markx%3)/3;
					//markfy = (marky-marky%3)/3;
					//markmsx = (3+8*markfx)+(markx%3)*2
					//markmsy = (1+6*markfy)+(marky%3)*2

					local_board_dmx += test.getMouseDeltaX();
					local_board_dmy += test.getMouseDeltaY();

					if(abs(local_board_dmx)>1 || abs(local_board_dmy)>1) {
						#ifdef INCLUDE_CLIENT_H__
							if(can_client_move) {
								markx += (local_board_dmx-local_board_dmx%2) / 2;
								marky += (local_board_dmy-local_board_dmy%2) / 2;
							}
						#else
							markx += (local_board_dmx-local_board_dmx%2 / 2;
							marky += (local_board_dmy-local_board_dmy%2) / 2;
						#endif
						local_board_dmx = 0;
						local_board_dmy = 0;
					} else {
						update = false;
					}

				} else {
					switch(key) {
						case 75:
						#ifdef INCLUDE_CLIENT_H__
							if(can_client_move) --markx;
						#else
							--markx;
						#endif
						break;
						case 72:
						#ifdef INCLUDE_CLIENT_H__
							if(can_client_move) --marky;
						#else
							--marky;
						#endif
						break;
						case 77:
						#ifdef INCLUDE_CLIENT_H__
							if(can_client_move) ++markx;
						#else
							++markx;
						#endif
						break;
						case 80:
						#ifdef INCLUDE_CLIENT_H__
							if(can_client_move) ++marky;
						#else
							++marky;
						#endif
						break;
						default:
						if(!runserver) update=false;
					}
				}


				#ifdef INCLUDE_SERVER_H__
					if(true) {
				#endif
				if(key==13 || (test.getMouseInfo().lbutton && test.isMouseEnabled())) {
 					#ifdef INCLUDE_CLIENT_H__
						#ifndef INCLUDE_SERVER_H__
							if(is_net_game_client) {
								clientStep();
							}
						#else
							if(!runserver) {
								if(is_net_game_client) {
									clientStep();
								}
							}
						#endif
					#endif
					if(
					#ifdef INCLUDE_CLIENT_H__
					can_client_move&&
					#endif
					!update_menu_state&&(!is_game_made)&&levmap[marky*(nx*3)+markx]==0) { wait_each_ai_enemy = true; cswitch(!SWITCH_USER_CANNOT_MOVE) {
						levmap[marky*(nx*3)+markx]=playerAiReg[playerRegMove]->marker;
						if(is_net_game_client) {
							client_user_move_x = markx;
							client_user_move_y = marky;
							client_user_move_marker = playerAiReg[playerRegMove]->marker;
							can_client_move = false;
						}
						playerAiReg[playerRegMove]->userPlayerMoveNotification();
					} redraw();
					if(!(is_net_game_client||is_net_game_server)) {
						update=moveAllPlayers();
					} else {
						//update=moveAllPlayers_server();
						moveNextPlayer();
						can_client_move = false;
					}
					redraw(); }
				} else if(key==/*32*/126/*&&(!runserver)*/) {
					if(!update_menu_state&&(!is_game_made)&&levmap[marky*(nx*3)+markx]==0) { wait_each_ai_enemy = true; redraw();
					#ifndef INCLUDE_SERVER_H__
						update=moveAllPlayers();
					#else
						update=moveAllPlayers_server();
						can_client_move = false;
					#endif
					redraw(); }
				}
				#ifdef INCLUDE_SERVER_H__
					}
				#endif


				validateCurPos();
				if(forceupdate_key) {
					update = true;
				}
				if(update) {

					redraw();
					demark_winner();
					winner_checks.clear();
					short win = getWinner();
					if(win!=0) {

						int last = playerRegMove+1;
						if(last>=playersNum) {
							last-=playersNum;
						}

						if(!is_game_made) {
							if(win==playerAiReg[last]->marker) {
								drawEndGameCom(string("")+"  _______      ___      .___  ___.  _______         ______   ____    ____  _______ .______               \n /  _____|    /   \\     |   \\/   | |   ____|       /  __  \\  \\   \\  /   / |   ____||   _  \\              \n|  |  __     /  ^  \\    |  \\  /  | |  |__         |  |  |  |  \\   \\/   /  |  |__   |  |_)  |             \n|  | |_ |   /  /_\\  \\   |  |\\/|  | |   __|        |  |  |  |   \\      /   |   __|  |      /              \n|  |__| |  /  _____  \\  |  |  |  | |  |____       |  `--'  |    \\    /    |  |____ |  |\\  \\----.__ __ __ \n \\______| /__/     \\__\\ |__|  |__| |_______|       \\______/      \\__/     |_______|| _| `._____(__|__|__)\n\n"+"             Winner of the game is player: "+ascii_str(win)+" ");
								test.flush();
								Sleep(1000);
								test.waitForUserInput();
								test.clear();
							} else {
								drawEndGameCom(string("")+"   ____    ____  ______    __    __     ____    __    ____  ______   .__   __.  __  \n   \\   \\  /   / /  __  \\  |  |  |  |    \\   \\  /  \\  /   / /  __  \\  |  \\ |  | |  | \n    \\   \\/   / |  |  |  | |  |  |  |     \\   \\/    \\/   / |  |  |  | |   \\|  | |  | \n     \\_    _/  |  |  |  | |  |  |  |      \\            /  |  |  |  | |  . `  | |  | \n       |  |    |  `--'  | |  `--'  |       \\    /\\    /   |  `--'  | |  |\\   | |__| \n       |__|     \\______/   \\______/         \\__/  \\__/     \\______/  |__| \\__| (__) \n\n"+"             Winner of the game is player: "+ascii_str(win)+" ");
								test.flush();
								Sleep(1000);
								test.waitForUserInput();
								test.clear();
							}
						}

						is_game_made = true;
						redraw();

						mark_winner();

						test.flush();
					}
					test.flush();
				}

				//playerAiReg[playerRegMove]
				//playerAiReg[playerRegMove]->move();

				/*if(playerAiReg[playerRegMove]->isUserEnemy()) {
					context tempcont(test);
					game_youre_moving_textbox.show(true);
					game_youre_moving_textbox.draw(tempcont);
					game_youre_moving_textbox.setText("Now it's "+ascii_str(playerAiReg[playerRegMove]->marker)+" player turn!");
					test.flush();
				}*/

			break;
			default:
				if(menu_state==0) {
					mainmanu_image.show(true);
					static ascimg timg;

					if(main_scene.isFocused(&b_exit)) {
						timg = ascimg::fromFile("menu_option_exit.txt");
						main_descrpt_box.setText("Exit the application.");
					} else if(main_scene.isFocused(&b_play)) {
						timg = ascimg::fromFile("menu_option_play.txt");
						main_descrpt_box.setText("Play a new game.\nSelect enemies types and see how good you are...");
					} else if(main_scene.isFocused(&b_info)) {
						timg = ascimg::fromFile("menu_option_info.txt");
						main_descrpt_box.setText("About the application.");
					} else if(main_scene.isFocused(&b_updates)) {
						timg = ascimg::fromFile("menu_option_updates.txt");
						main_descrpt_box.setText("Checkout incoming updates!");
					} else if(main_scene.isFocused(&b_multiplayer)) {
						timg = ascimg::fromFile("menu_option_multiplayer.txt");
						main_descrpt_box.setText("Play multiplayer game.\nYou can be host a server or join running game by IP adress.\nWorks using C/C++ sockets interface.");
				    } else if(main_scene.isFocused(&b_settings)) {
                        main_descrpt_box.setText("Let's change some stuff over here!");
                    } else {
						timg = ascimg::fromFile("menu_option_null.txt");
						main_descrpt_box.setText("");
					}
					mainmanu_image.setImage(&timg);
					mainmanu_image.setFrameStyle(null_frame_style);
				} else if(menu_state==1) {
					static ascimg timg;

					if(before_play_scene.isFocused(&bfs_b_play)) {
						timg = ascimg::fromFile("menu_background_play_o.txt");
					} else {
						timg = ascimg::fromFile("menu_background_play.txt");
					}
					bfs_imgf_image.setImage(&timg);

				}
				//test.blockMouse(false);
				cont.draw();
				test.flush();
			break;
		}

	}
	atexit(onExit);
	exit(0);
	return 0;
}


int ftp_progress(void *clientp, double dltotal, double dlnow, double ultotal, double ulnow) {

    /**
    test.setCurPos(15+prog,40);
			test<<"#";
			test.flush();
			Sleep(10);
			test.setCurPos(9,36);
    */

	clock_t t = clock() - timer;
	if(((float)t)/CLOCKS_PER_SEC >= 1.0f) {
		timer = clock();
		if(dlnow<=0||dltotal<=0) {
			return 0;
		}
		test.setCurPos(25,4);
		for(int it=0;it<20;++it) {
			if(it<((int)(dlnow*100/dltotal))/5) {
				test.setCurPos(15+it,40);
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

#if SWITCH_NEVER_PHYSICALLY_INSTALL_UPDATE==NO
inline void update_version() {
    log("Updating version TRIGGERED.");

	//file_data_uc_buffer = new unsigned char[409600];

    fr.setText("Chcecking updates...\n");
    con.draw();test.flush();
    Sleep(500);

	CURL *curl;
    FILE *fp;
    CURLcode res;
    string url = newversion_url;
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
    		Sleep(3000);
    		return;
	    }
	//}

	string version_name = "";
	float version = 0;
	int version_build = 0;
	bool force_update = false;
	int files_in_zip = 0;
	int made_files = 0;

	fr.setText("Configuring update...\n");

	ofstream bat("update_script.bat", ofstream::out);
	bat<<"@ECHO OFF\n";
	bat<<"timeout /t 5\n";
	bat<<"fbzip -e -p \"downloads\\scgl_download.zip\" \""+path+""+"\""<<"\n";
	bat<<"SCGL.exe\n";
	bat<<"@ECHO ON\n";
	bat.close();

    atexit(onExitUpdate);
    exit(0);
	return;

}
#else
inline void update_version() {
       return;
}
#endif
