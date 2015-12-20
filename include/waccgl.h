#include <tchar.h>

#define getid(NAME) (MACRO_STR__(NAME))
#define assignid(NAME) NAME.id = MACRO_STR__(NAME);

#define STR_VALUE(arg)      #arg
#define MACRO_STR__(name) STR_VALUE(name)

#define objectid string
#define nullid "<null>"


class context;
class scene_object;


class context;
class scene_object;

string intToStr(int n) {
     string tmp, ret;
     if(n < 0) {
          ret = "-";
          n = -n;
     }
     do {
          tmp += n % 10 + 48;
          n -= n % 10;
     }
     while(n /= 10);
     for(int i = tmp.size()-1; i >= 0; i--)
          ret += tmp[i];
     return ret;
}


double strToDouble(string s) {
	return atof(s.c_str()); 
}

string doubleToStr(double dbl) {	
	std::ostringstream strs;
	strs << dbl;
	return strs.str();
}

int strToInt(string s) {
     int tmp = 0, i = 0;
     bool m = false;
     if(s[0] == '-') {
          m = true;
          i++;
     }
     for(; i < s.size(); i++)
          tmp = 10 * tmp + s[i] - 48;
     return m ? -tmp : tmp;   
}

#include "action.h"
#include "console.h"
#include "graphics.h"
#include "controls.h"

