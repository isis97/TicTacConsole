
struct frame_style {
	char horizontal;
	char vertical;
	char top_left;
	char top_right;
	char bottom_left;
	char bottom_right;
	
} default_frame_style = { (char)196, (char)179,(char)218, (char)191, (char)192, (char)217 },
  default_dbframe_style = { (char)205, (char)186,(char)201, (char)187, (char)200, (char)188 },
  invisible_frame_style = { ' ', ' ', ' ', ' ', ' ', ' ' },
  null_frame_style = { (char)0x00, (char)0x00, (char)0x00, (char)0x00, (char)0x00, (char)0x00 };
  
class drawer {
	public:
		inline virtual void draw(context* cont) {}
		inline virtual bool isActive() {return false;}
} null_drawer;

#include "scene.h"



class ascimg {
	public:
		char *img;
		int w, h;
		
		ascimg() {
			w = 0;
			h = 0;
			img = nullptr;
		}

		ascimg(int sizex, int sizey) {
			w = sizex;
			h = sizey;
			const int siz = w*h+7;
			img = new char[w*h+7];
			for(int it=0;it<siz;++it) {
				img[it] = ' ';
			}
		}
		
		inline char* get(int x, int y) {
			return &(img[y*w+x]);
		}
		
		inline int size() {
			return w*h;
		}

		inline void clear() {
			if(img==nullptr) return;
			for(int it=0;it<h*w;++it) {
				img[it] = ' ';
			}
		}
		
		inline void loadFromFile(string filename) {
			ifstream loads(filename.c_str());
			string buf;
			int siz;
			for(int it=0;it<h;++it) {
				getline(loads, buf);
				siz = min(buf.size(),w);
				for(int x=0;x<siz;++x) {
					img[it*w+x]=buf[x];
				}
			}
			loads.close();
		}
		
		inline void createFromFile(string filename) {
			ifstream loads(filename.c_str());
			string buf;
			int maxw = 0, totalw = 0;
			if(buf.size()+1>maxw) {
				maxw = buf.size()+1;
			}
			totalw += buf.size();
			getline(loads, buf);
			h = 1;
			while(!loads.eof()&&loads.good()) {
				++h;
				getline(loads,buf);
				if(buf.size()+1>maxw) {
					maxw = buf.size()+1;
				}
				totalw += buf.size();
			}
			w = maxw;
			delete img;
			img = new char[w*h+7];
			loads.close();
			clear();
			loadFromFile(filename);
		}

		static ascimg fromFile(string filename) {
			ascimg ret(100, 100);
			ret.createFromFile(filename);
			return ret;
		}
		
};


