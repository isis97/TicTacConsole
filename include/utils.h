inline int getPlayerMoveRelativeTime(char sym);
inline bool moveAllPlayers();
inline bool isServerRunning();
class aienemy;
inline char getPlayerAfterMe(int playlev);

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
