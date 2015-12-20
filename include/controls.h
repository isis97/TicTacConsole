


class frame : public scene_object {
	private:
		int objx, objy, objw, objh;
		char defbg;
		frame_style fs;
	
	public:
		
		frame(int ax, int ay, int aw, int ah) {
			fs = default_frame_style;
			defbg = ' ';
			objx = ax;
			objy = ay;
			objw = aw;
			objh = ah;
		}
		
		inline void setFrameStyle(frame_style afs) {
			fs = afs;
		}
		
		inline void setBackground(char ch) {
			defbg = ch;
		}
		
		inline void draw(context& arg) {
			int x, y;
			for(int ity=0;ity<objh;++ity) {
				for(int itx=0;itx<objw;++itx) {
					x = itx+objx+parentX;
					y = ity+objy+parentY;
					if(defbg!=0x00) {
						arg.get().put(x, y, defbg);
					}
					if(ity==0) {
						if(itx==0) {
							arg.get().put(x, y, fs.top_left);
						} else if(itx==objw-1) {
							arg.get().put(x, y, fs.top_right);
						} else {
							arg.get().put(x, y, fs.horizontal);
						}
					} else if(ity==objh-1) {
						if(itx==0) {
							arg.get().put(x, y, fs.bottom_left);
						} else if(itx==objw-1) {
							arg.get().put(x, y, fs.bottom_right);
						} else {
							arg.get().put(x, y, fs.horizontal);
						}
					} else {
						if(itx==0||itx==objw-1) {
							arg.get().put(x, y, fs.vertical);
						}	
					}
					
				}
			}
		}
		
		inline bool isFocusable(context& arg) {
			return false;
		}
};			
	
	

class textframe : public scene_object {
	private:
		int objx, objy, objw, objh;
		char defbg;
		string text;
		frame_style fs;
	
	public:
		textframe(int ax, int ay, int aw, int ah, string atext) {
			fs = default_frame_style;
			defbg = ' ';
			objx = ax;
			objy = ay;
			objw = aw;
			objh = ah;
			text = atext;
		}
		
		inline void setText(string s) {
			text = s;
		}
		
		inline void addText(string s) {
			text += s;
		}
		
		inline void clearText(string s) {
			text = "";
		}
		
		inline string getText(string s) {
			return text;
		}
		
		inline void setFrameStyle(frame_style afs) {
			fs = afs;
		}
		
		inline void setBackground(char ch) {
			defbg = ch;
		}
		
		inline void draw(context& arg) {
			int x, y, tit, siz = text.size();
			bool lockLine = false;
			tit = 0;
			for(int ity=0;ity<objh;++ity) {
				
				for(int itx=0;itx<objw;++itx) {
					x = itx+objx+parentX;
					y = ity+objy+parentY;
					if(defbg!=0x00) {
						arg.get().put(x, y, defbg);
					}
					if(!lockLine) {
						if(ity!=0&&ity!=objh-1&&itx!=0&&itx!=objw-1&&tit<siz) {
							if(text[tit] == '\n') {
								lockLine = true;
								++tit;
							} else {
								arg.get().put(x, y, text[tit]);
								++tit;
							}
						}
					}
					
					if(ity==0) {
						if(itx==0) {
							arg.get().put(x, y, fs.top_left);
						} else if(itx==objw-1) {
							arg.get().put(x, y, fs.top_right);
						} else {
							arg.get().put(x, y, fs.horizontal);
						}
					} else if(ity==objh-1) {
						if(itx==0) {
							arg.get().put(x, y, fs.bottom_left);
						} else if(itx==objw-1) {
							arg.get().put(x, y, fs.bottom_right);
						} else {
							arg.get().put(x, y, fs.horizontal);
						}
					} else {
						if(itx==0||itx==objw-1) {
							arg.get().put(x, y, fs.vertical);
						}	
					}
					
				}
				lockLine = false;
				arg.get().moveCur(0, 1);
			}
		}
		
		inline bool isFocusable(context& arg) {
			return false;
		}
};



class imgframe : public scene_object {
	private:
		int objx, objy, objw, objh;
		char defbg;
		ascimg *img;
		frame_style fs;
	
	public:
		imgframe(int ax, int ay, int aw, int ah, ascimg *aimg) {
			fs = default_frame_style;
			defbg = ' ';
			objx = ax;
			objy = ay;
			objw = aw;
			objh = ah;
			img = aimg;
		}
		
		imgframe(int ax, int ay, ascimg *aimg) {
			fs = default_frame_style;
			defbg = ' ';
			objx = ax;
			objy = ay;
			objw = aimg->w;
			objh = aimg->h+2;
			img = aimg;
		}
		
		inline void setFrameStyle(frame_style afs) {
			fs = afs;
		}

		inline void setImage(ascimg *aimg) {
			objw = aimg->w;
			objh = aimg->h+2;
			img = aimg;
		}
		
		inline void setBackground(char ch) {
			defbg = ch;
		}
		
		inline void draw(context& arg) {
			int x, y, tit, titx, tity, siz = img->size();
			
			titx = 0;
			tity = 0;
			tit = 0;
			for(int ity=0;ity<objh;++ity) {
				
				for(int itx=0;itx<objw;++itx) {
					x = itx+objx+parentX;
					y = ity+objy+parentY;
					if(defbg!=0x00) {
						arg.get().put(x, y, defbg);
					}
					if(ity!=0&&ity!=objh-1&&itx!=0&&itx!=objw-1&&tit<siz) {
						arg.get().put(x, y, *img->get(itx-1, ity-1));
					}
					if(ity==0) {
						if(itx==0) {
							arg.get().put(x, y, fs.top_left);
						} else if(itx==objw-1) {
							arg.get().put(x, y, fs.top_right);
						} else {
							arg.get().put(x, y, fs.horizontal);
						}
					} else if(ity==objh-1) {
						if(itx==0) {
							arg.get().put(x, y, fs.bottom_left);
						} else if(itx==objw-1) {
							arg.get().put(x, y, fs.bottom_right);
						} else {
							arg.get().put(x, y, fs.horizontal);
						}
					} else {
						if(itx==0||itx==objw-1) {
							arg.get().put(x, y, fs.vertical);
						}	
					}
					
				}
				tit+=img->w-objw-2;
				arg.get().moveCur(0, 1);
			}
		}
		
		inline bool isFocusable(context& arg) {
			return false;
		}
};
	
	
	
class button : public scene_object {
	private:
		int objx, objy, objw, objh;
		char defbg;
		string text;
		frame_style fs;
		frame_style mfs;
		action act;
	
	public:
		button() {
			
		}
		
		button(int ax, int ay, int aw, int ah, string atext) {
			act = null_action;
			fs = default_frame_style;
			mfs = default_dbframe_style;
			defbg = ' ';
			text = atext;
			objx = ax;
			objy = ay;
			objw = aw;
			objh = ah;
		}
		
		button(int ax, int ay, string atext) {
			act = null_action;
			fs = default_frame_style;
			mfs = default_dbframe_style;
			defbg = ' ';
			text = atext;
			objx = ax;
			objy = ay;
			objw = atext.size()+4;
			objh = 3;
		}
		
		inline void setAction(action aact) {
			act = aact;
		}
		
		inline void setText(string atext) {
			text = atext;
		}
		
		inline void setFrameStyle(frame_style afs) {
			fs = afs;
		}
		
		inline void setMarkedFrameStyle(frame_style afs) {
			mfs = afs;
		}
		
		inline void setBackground(char ch) {
			defbg = ch;
		}
		
		inline ME_STATE onMouseEvent(context& arg, mouse_info& mouse_data) {
			if(mouse_data.x>=objx+parentX && mouse_data.x<=objx+parentX+objw) {
				if(mouse_data.y>=objy+parentY && mouse_data.y<=objy+parentY+objh) {
					return ME_FOCUSED;
				}
			}
			return ME_NOT_FOCUSED;
		}

		inline void draw(bool state, context& arg) {
			int x, y;
			for(int ity=0;ity<objh;++ity) {
				for(int itx=0;itx<objw;++itx) {
					x = itx+objx+parentX;
					y = ity+objy+parentY;
					arg.get().put(x, y, defbg);
					if(state) {
						if(ity==0) {
							if(itx==0) {
								arg.get().put(x, y, mfs.top_left);
							} else if(itx==objw-1) {
								arg.get().put(x, y, mfs.top_right);
							} else {
								arg.get().put(x, y, mfs.horizontal);
							}
						} else if(ity==objh-1) {
							if(itx==0) {
								arg.get().put(x, y, mfs.bottom_left);
							} else if(itx==objw-1) {
								arg.get().put(x, y, mfs.bottom_right);
							} else {
								arg.get().put(x, y, mfs.horizontal);
							}
						} else {
							if(itx==0||itx==objw-1) {
								arg.get().put(x, y, mfs.vertical);
							}	
						}
					} else {
						if(ity==0) {
							if(itx==0) {
								arg.get().put(x, y, fs.top_left);
							} else if(itx==objw-1) {
								arg.get().put(x, y, fs.top_right);
							} else {
								arg.get().put(x, y, fs.horizontal);
							}
						} else if(ity==objh-1) {
							if(itx==0) {
								arg.get().put(x, y, fs.bottom_left);
							} else if(itx==objw-1) {
								arg.get().put(x, y, fs.bottom_right);
							} else {
								arg.get().put(x, y, fs.horizontal);
							}
						} else {
							if(itx==0||itx==objw-1) {
								arg.get().put(x, y, fs.vertical);
							}	
						}
					}
				}
				
				arg.get().setCurPos(objx+parentX+(objw-2-text.size())/2+1, objy+parentY+((objh-objh%2)/2));
				arg.get() << text;
				arg.get().moveCur(0, 1);
			}
		}
		
		inline void whenClicked(context& arg) {
			act(arg, this);
		}
};		



class selbox : public scene_object {
	private:
		int objx, objy, objw, objh;
		char defbg;
		string text;
		frame_style fs;
		frame_style mfs;
		string * options_names;
		int options_num;
		int sel_option;
		action act;
	
	public:
		
		selbox(int ax, int ay, int aw, int ah, int argnum, string atext[]) {
			act = null_action;
			sel_option=0;
			options_num = argnum;
			options_names = new string[options_num+1];
			for(int it=0;it<options_num;++it) {
				options_names[it] = atext[it];
			}
			fs = default_frame_style;
			mfs = default_dbframe_style;
			defbg = ' ';
			text = atext[0];
			objx = ax;
			objy = ay;
			objw = aw;
			objh = ah;
		}
		
		selbox(int ax, int ay, int argnum, string atext[]) {
			act = null_action;
			sel_option = 0;
			options_num = argnum;
			options_names = new string[options_num+1];
			for(int it=0;it<options_num;++it) {
				options_names[it] = atext[it];
			}
			fs = default_frame_style;
			mfs = default_dbframe_style;
			defbg = ' ';
			text = atext[0];
			objx = ax;
			objy = ay;
			objw = text.size()+4;
			objh = 3;
		}
		
		inline void moveOptions(context& arg, int delta) {
			if(delta>0) {
				for(int it=0;it<delta;++it) {
					nextOption();
					act(arg, this);
				}
			} else {
				for(int it=0;it<-delta;++it) {
					prevOption();
					act(arg, this);
				}
			}
		}

		inline void onMouseEventFocused(context& arg, mouse_info& mouse_data) {
			moveOptions(arg, mouse_data.wheel_pos_delta);
			mouse_data.wheel_pos_delta = 0;
		}

		inline ME_STATE onMouseEvent(context& arg, mouse_info& mouse_data) {
			if(mouse_data.x>=objx+parentX && mouse_data.x<=objx+parentX+objw) {
				if(mouse_data.y>=objy+parentY && mouse_data.y<=objy+parentY+objh) {
					return ME_FOCUSED;
				}
			}
			return ME_NULL;
		}

		inline string* getOptions() {
			return options_names;
		}
		
		inline int getSelectionNumber() {
			return sel_option;
		}
		
		inline string getSelectedOption() {
			return options_names[sel_option];
		}
		
		inline int optionsNumber() {
			return options_num;
		}
		
		inline void setAction(action aact) {
			act = aact;
		}
		
		inline void nextOption() {
			++sel_option;
			if(sel_option>=options_num) {
				sel_option=0;
			}
		}
		
		inline void prevOption() {
			--sel_option;
			if(sel_option<0) {
				sel_option=options_num-1;
			}
		}
		
		inline void setFrameStyle(frame_style afs) {
			fs = afs;
		}
		
		inline void setMarkedFrameStyle(frame_style afs) {
			mfs = afs;
		}
		
		inline void setBackground(char ch) {
			defbg = ch;
		}
		
		inline void draw(bool state, context& arg) {
			int x, y;
			for(int ity=0;ity<objh;++ity) {
				for(int itx=0;itx<objw;++itx) {
					x = itx+objx+parentX;
					y = ity+objy+parentY;
					arg.get().put(x, y, defbg);
					if(state) {
						if(ity==0) {
							if(itx==0) {
								arg.get().put(x, y, mfs.top_left);
							} else if(itx==objw-1) {
								arg.get().put(x, y, mfs.top_right);
							} else {
								arg.get().put(x, y, mfs.horizontal);
							}
						} else if(ity==objh-1) {
							if(itx==0) {
								arg.get().put(x, y, mfs.bottom_left);
							} else if(itx==objw-1) {
								arg.get().put(x, y, mfs.bottom_right);
							} else {
								arg.get().put(x, y, mfs.horizontal);
							}
						} else {
							if(itx==0||itx==objw-1) {
								arg.get().put(x, y, mfs.vertical);
							}	
						}
					} else {
						if(ity==0) {
							if(itx==0) {
								arg.get().put(x, y, fs.top_left);
							} else if(itx==objw-1) {
								arg.get().put(x, y, fs.top_right);
							} else {
								arg.get().put(x, y, fs.horizontal);
							}
						} else if(ity==objh-1) {
							if(itx==0) {
								arg.get().put(x, y, fs.bottom_left);
							} else if(itx==objw-1) {
								arg.get().put(x, y, fs.bottom_right);
							} else {
								arg.get().put(x, y, fs.horizontal);
							}
						} else {
							if(itx==0||itx==objw-1) {
								arg.get().put(x, y, fs.vertical);
							}	
						}
					}
				}
				/*
				string options_names;
				int options_num;
				int sel_option;
				*/
				text = options_names[sel_option];
				
				arg.get().setCurPos(objx+parentX+(objw-2-text.size())/2+1, objy+parentY+((objh-objh%2)/2));
				arg.get() << text;
				arg.get().moveCur(0, 1);
				
				if(state) {
					arg.get().setCurPos(objx+parentX+(objw-2-text.size())/2, objy+parentY+((objh-objh%2)/2)+1);
					arg.get() << "/|\\";
					arg.get().setCurPos(objx+parentX+(objw-2-text.size())/2, objy+parentY+((objh-objh%2)/2)-1);
					arg.get() << "\\|/";
				} else {
					arg.get().setCurPos(objx+parentX+(objw-2-text.size())/2+1, objy+parentY+((objh-objh%2)/2)+1);
					arg.get() << "|";
					arg.get().setCurPos(objx+parentX+(objw-2-text.size())/2+1, objy+parentY+((objh-objh%2)/2)-1);
					arg.get() << "|";
				}
			
				
				
			}
		}
		
		inline void onKeyEvent(context& arg, char keycode) {
			//test.setCurPos(3,3);
			//test<<"EXEC";
			//test.flush();
			switch(keycode) {
				/*up-arrow*/   case 80: nextOption(); act(arg, this); break;
				/*down-arrow*/ case 72: prevOption(); act(arg, this); break;
			}
		}
		
		inline void whenClicked(context& arg) {
			
		}
};		



class checkbox : public scene_object {
	private:
		int objx, objy, objw, objh;
		char defbg;
		string text;
		frame_style fs;
		frame_style mfs;
		bool cstate;
		action act;
	
	public:
		checkbox(int ax, int ay, int aw, int ah, string atext) {
			cstate=false;
			fs = default_frame_style;
			mfs = default_dbframe_style;
			defbg = ' ';
			text = atext;
			objx = ax;
			objy = ay;
			objw = aw;
			objh = ah;
			act = null_action;
		}
		
		checkbox(int ax, int ay, string atext) {
			cstate=false;
			fs = default_frame_style;
			mfs = default_dbframe_style;
			defbg = ' ';
			text = atext;
			objx = ax;
			objy = ay;
			objw = atext.size()+4;
			objh = 3;
			act = null_action;
		}
		
		inline void onMouseEventFocused(context& arg, mouse_info& mouse_data) {
			if(mouse_data.wheel_pos_delta!=0) {
				whenClicked(arg);
			}
			mouse_data.wheel_pos_delta = 0;
		}

		inline ME_STATE onMouseEvent(context& arg, mouse_info& mouse_data) {
			if(mouse_data.x>=objx+parentX && mouse_data.x<=objx+parentX+objw) {
				if(mouse_data.y>=objy+parentY && mouse_data.y<=objy+parentY+objh) {
					return ME_FOCUSED;
				}
			}
			return ME_NULL;
		}

		inline void setState(bool state) {
			cstate = state;
		}
		
		inline void setFrameStyle(frame_style afs) {
			fs = afs;
		}
		
		inline void setMarkedFrameStyle(frame_style afs) {
			mfs = afs;
		}
		
		inline void setBackground(char ch) {
			defbg = ch;
		}
		
		inline void setAction(action aact) {
			act = aact;
		}
		
		inline void draw(bool state, context& arg) {
			int x, y;
			for(int ity=0;ity<objh;++ity) {
				for(int itx=0;itx<objw;++itx) {
					x = itx+objx+parentX;
					y = ity+objy+parentY;
					arg.get().put(x, y, defbg);
					if(state) {
						if(ity==0) {
							if(itx==0) {
								arg.get().put(x, y, mfs.top_left);
							} else if(itx==objw-1) {
								arg.get().put(x, y, mfs.top_right);
							} else {
								arg.get().put(x, y, mfs.horizontal);
							}
						} else if(ity==objh-1) {
							if(itx==0) {
								arg.get().put(x, y, mfs.bottom_left);
							} else if(itx==objw-1) {
								arg.get().put(x, y, mfs.bottom_right);
							} else {
								arg.get().put(x, y, mfs.horizontal);
							}
						} else {
							if(itx==0||itx==objw-1) {
								arg.get().put(x, y, mfs.vertical);
							}	
						}
					} else {
						if(ity==0) {
							if(itx==0) {
								arg.get().put(x, y, fs.top_left);
							} else if(itx==objw-1) {
								arg.get().put(x, y, fs.top_right);
							} else {
								arg.get().put(x, y, fs.horizontal);
							}
						} else if(ity==objh-1) {
							if(itx==0) {
								arg.get().put(x, y, fs.bottom_left);
							} else if(itx==objw-1) {
								arg.get().put(x, y, fs.bottom_right);
							} else {
								arg.get().put(x, y, fs.horizontal);
							}
						} else {
							if(itx==0||itx==objw-1) {
								arg.get().put(x, y, fs.vertical);
							}	
						}
					}
				}
				
				arg.get().setCurPos(2,2);
				string pom;
				for(int it=128;it<255;++it) {
					
					pom+=intToStr(it)+"="+"?";
					pom[pom.size()-1] = (char)it;
				}
				//arg.get() << pom;
				
				arg.get().setCurPos(objx+parentX+(objw-2-text.size())/2+1, objy+parentY+((objh-objh%2)/2));
				arg.get() << text;
				//arg.get().moveCur(0, 1);
				
				arg.get().setCurPos(objx+parentX, objy+((objh-objh%2)/2)+parentY);
				string p = " ";
				if(cstate) {
					p[0] = (char)220;
				} else {
					p[0] = (char)'O';
				}
				arg.get() << p;
				
				/*if(cstate) {
					arg.get() << (char)93;
				} else {
					arg.get() << (char)93;
				}*/
			}
		}
	
		void whenClicked(context& arg) {
			cstate = !cstate;
			act(arg, this);
		}
		
		inline bool isChecked() {
			return cstate;
		}

};		






class slistbox : public scene_object {
	private:
		int objx, objy, objw, objh;
		char defbg;
		string text;
		frame_style fs;
		frame_style mfs;
		string * options_names;
		int options_num;
		int sel_option;
		action act;
		bool sprop_roundedList;
	
	public:
		
		slistbox(int ax, int ay, int aw, int ah, int argnum, string atext[]) {
			setDefaultSideProperties();
			act = null_action;
			sel_option=0;
			options_num = argnum;
			options_names = new string[options_num+1];
			for(int it=0;it<options_num;++it) {
				options_names[it] = atext[it];
			}
			fs = default_frame_style;
			mfs = default_dbframe_style;
			defbg = ' ';
			text = atext[0];
			objx = ax;
			objy = ay;
			objw = aw;
			objh = ah;
		}
		
		slistbox(int ax, int ay, int argnum, string atext[]) {
			setDefaultSideProperties();
			act = null_action;
			sel_option = 0;
			options_num = argnum;
			options_names = new string[options_num+1];
			for(int it=0;it<options_num;++it) {
				options_names[it] = atext[it];
			}
			fs = default_frame_style;
			mfs = default_dbframe_style;
			defbg = ' ';
			text = atext[0];
			objx = ax;
			objy = ay;
			objw = text.size()+4;
			objh = 3;
		}

				inline void moveOptions(context& arg, int delta) {
			if(delta>0) {
				for(int it=0;it<delta;++it) {
					nextOption();
					act(arg, this);
				}
			} else {
				for(int it=0;it<-delta;++it) {
					prevOption();
					act(arg, this);
				}
			}
		}

		inline void onMouseEventFocused(context& arg, mouse_info& mouse_data) {
			moveOptions(arg, mouse_data.wheel_pos_delta);
			mouse_data.wheel_pos_delta = 0;
		}

		inline ME_STATE onMouseEvent(context& arg, mouse_info& mouse_data) {
			if(mouse_data.x>=objx+parentX && mouse_data.x<=objx+parentX+objw) {
				if(mouse_data.y>=objy+parentY && mouse_data.y<=objy+parentY+objh) {
					return ME_FOCUSED;
				}
			}
			return ME_NULL;
		}
		
		inline void setDefaultSideProperties() {
			sprop_roundedList = false;
		}
		
		inline void setRoundedList(bool state) {
			sprop_roundedList = state;
		}
		
		inline string* getOptions() {
			return options_names;
		}
		
		inline int getSelectionNumber() {
			return sel_option;
		}
		
		inline string getSelectedOption() {
			return options_names[sel_option];
		}
		
		inline int optionsNumber() {
			return options_num;
		}
		
		inline void setAction(action aact) {
			act = aact;
		}
		
		inline void nextOption() {
			++sel_option;
			if(sel_option>=options_num) {
				if(sprop_roundedList) {
					sel_option= 0;
				} else {
					sel_option= options_num-1;	
				}
			}
		}
		
		inline void prevOption() {
			--sel_option;
			if(sel_option<0) {
				if(sprop_roundedList) {
					sel_option = options_num-1;
				} else {
					sel_option = 0;	
				}
			}
		}
		
		inline void setFrameStyle(frame_style afs) {
			fs = afs;
		}
		
		inline void setMarkedFrameStyle(frame_style afs) {
			mfs = afs;
		}
		
		inline void setBackground(char ch) {
			defbg = ch;
		}
		
		inline void draw(bool state, context& arg) {
			int x, y;
			for(int ity=0;ity<objh;++ity) {
				for(int itx=0;itx<objw;++itx) {
					x = itx+objx+parentX;
					y = ity+objy+parentY;
					arg.get().put(x, y, defbg);
					if(state) {
						if(ity==0) {
							if(itx==0) {
								arg.get().put(x, y, mfs.top_left);
							} else if(itx==objw-1) {
								arg.get().put(x, y, mfs.top_right);
							} else {
								arg.get().put(x, y, mfs.horizontal);
							}
						} else if(ity==objh-1) {
							if(itx==0) {
								arg.get().put(x, y, mfs.bottom_left);
							} else if(itx==objw-1) {
								arg.get().put(x, y, mfs.bottom_right);
							} else {
								arg.get().put(x, y, mfs.horizontal);
							}
						} else {
							if(itx==0||itx==objw-1) {
								arg.get().put(x, y, mfs.vertical);
							}	
						}
					} else {
						if(ity==0) {
							if(itx==0) {
								arg.get().put(x, y, fs.top_left);
							} else if(itx==objw-1) {
								arg.get().put(x, y, fs.top_right);
							} else {
								arg.get().put(x, y, fs.horizontal);
							}
						} else if(ity==objh-1) {
							if(itx==0) {
								arg.get().put(x, y, fs.bottom_left);
							} else if(itx==objw-1) {
								arg.get().put(x, y, fs.bottom_right);
							} else {
								arg.get().put(x, y, fs.horizontal);
							}
						} else {
							if(itx==0||itx==objw-1) {
								arg.get().put(x, y, fs.vertical);
							}	
						}
					}
				}
				
				text = options_names[sel_option];
				int textidbeg = sel_option-((objh-2)/2);
				
				if(textidbeg>options_num-1) {
					textidbeg=options_num-1;
				}
				if(textidbeg<0) {
					textidbeg=0;
				}
		
				for(int ity=1;ity<objh-1;++ity) {
					if(ity-1+textidbeg>=0 && ity-1+textidbeg<options_num) {
						arg.get().setCurPos(objx+(objw-5-text.size())/2+1+parentX, objy+ity+parentY);
						if(sel_option == ity-1+textidbeg) {
							if(state) {
								arg.get() << "-> ";
								arg.get() << options_names[ity-1+textidbeg];
								arg.get() << " <-";
							} else {
								arg.get() << " [ ";
								arg.get() << options_names[ity-1+textidbeg];
								arg.get() << " ] ";
							}
						} else {
							arg.get() << "   ";
							arg.get() << options_names[ity-1+textidbeg];
							arg.get() << "   ";	
						}	
					}	
				}
				
				/*arg.get().setCurPos(objx+(objw-2-text.size())/2+1, objy+((objh-objh%2)/2));
				arg.get() << text;
				arg.get().moveCur(0, 1);*/
				
				/*if(state) {
					arg.get().setCurPos(objx+(objw-2-text.size())/2, objy+((objh-objh%2)/2)+1);
					arg.get() << "/|\\";
					arg.get().setCurPos(objx+(objw-2-text.size())/2, objy+((objh-objh%2)/2)-1);
					arg.get() << "\\|/";
				} else {
					arg.get().setCurPos(objx+(objw-2-text.size())/2+1, objy+((objh-objh%2)/2)+1);
					arg.get() << "|";
					arg.get().setCurPos(objx+(objw-2-text.size())/2+1, objy+((objh-objh%2)/2)-1);
					arg.get() << "|";
				}*/
			
				
				
			}
		}
		
		inline void onKeyEvent(context& arg, char keycode) {
			//test.setCurPos(3,3);
			//test<<"EXEC";
			//test.flush();
			switch(keycode) {
				/*up-arrow*/   case 80: nextOption(); act(arg, this); break;
				/*down-arrow*/ case 72: prevOption(); act(arg, this); break;
			}
		}
		
		inline void whenClicked(context& arg) {
			
		}
};		






class listbox : public scene_object {
	private:
		int objx, objy, objw, objh;
		char defbg;
		string text;
		frame_style fs;
		frame_style mfs;
		vector<string> options_names;
		int options_num;
		int sel_option;
		action act;
		bool sprop_roundedList;
	
	public:
		
		listbox(int ax, int ay, int aw, int ah, vector<string> atext) {
			setDefaultSideProperties();
			act = null_action;
			sel_option=0;
			options_num = atext.size();
			options_names = atext;
			fs = default_frame_style;
			mfs = default_dbframe_style;
			defbg = ' ';
			text = atext[0];
			objx = ax;
			objy = ay;
			objw = aw;
			objh = ah;
		}
		
		listbox(int ax, int ay, int aw, int ah) {
			setDefaultSideProperties();
			act = null_action;
			sel_option=0;
			options_num = 0;
			fs = default_frame_style;
			mfs = default_dbframe_style;
			defbg = ' ';
			text = "";
			objx = ax;
			objy = ay;
			objw = aw;
			objh = ah;
		}
		
		listbox(int ax, int ay, int aw, int ah, int len, string atext[]) {
			setDefaultSideProperties();
			act = null_action;
			sel_option=0;
			setOptions(len, atext);
			fs = default_frame_style;
			mfs = default_dbframe_style;
			defbg = ' ';
			text = atext[0];
			objx = ax;
			objy = ay;
			objw = aw;
			objh = ah;
		}
		
		listbox(int ax, int ay, int len, string atext[]) {
			setDefaultSideProperties();
			act = null_action;
			sel_option = 0;
			setOptions(len, atext);
			fs = default_frame_style;
			mfs = default_dbframe_style;
			defbg = ' ';
			text = atext[0];
			objx = ax;
			objy = ay;
			objw = text.size()+4;
			objh = 3;
		}
		
		listbox(int ax, int ay, vector<string> atext) {
			setDefaultSideProperties();
			act = null_action;
			sel_option = 0;
			options_num = atext.size();
			options_names = atext;
			fs = default_frame_style;
			mfs = default_dbframe_style;
			defbg = ' ';
			text = atext[0];
			objx = ax;
			objy = ay;
			objw = text.size()+4;
			objh = 3;
		}
		
		listbox(int ax, int ay) {
			setDefaultSideProperties();
			act = null_action;
			sel_option = 0;
			options_num = 0;
			fs = default_frame_style;
			mfs = default_dbframe_style;
			defbg = ' ';
			text = "";
			objx = ax;
			objy = ay;
			objw = text.size()+4;
			objh = 3;
		}


		inline void moveOptions(context& arg, int delta) {
			if(delta>0) {
				for(int it=0;it<delta;++it) {
					nextOption();
					act(arg, this);
				}
			} else {
				for(int it=0;it<-delta;++it) {
					prevOption();
					act(arg, this);
				}
			}
		}

		inline void onMouseEventFocused(context& arg, mouse_info& mouse_data) {
			moveOptions(arg, mouse_data.wheel_pos_delta);
			mouse_data.wheel_pos_delta = 0;
		}

		inline ME_STATE onMouseEvent(context& arg, mouse_info& mouse_data) {
			if(mouse_data.x>=objx+parentX && mouse_data.x<=objx+parentX+objw) {
				if(mouse_data.y>=objy+parentY && mouse_data.y<=objy+parentY+objh) {
					return ME_FOCUSED;
				}
			}
			return ME_NULL;
		}

		
		inline void emptyOptions() {
			options_num = 0;
			options_names.clear();
		}
		
		inline void addOptions(int len, string atext[]) {
			for(int it=0;it<len;++it) {
				options_names.push_back(atext[it]);
			}
			options_num += len;
		}
		
		inline void setOptions(int len, string atext[]) {
			emptyOptions();
			addOptions(len, atext);
		}
		
		inline void overrideOption(int pos, string atext) {
			options_names[pos] = atext;
		}
		
		inline void addOption(string atext) {
			++options_num;
			options_names.push_back(atext);
		}
		
		inline void removeOption(string atext) {
			for(int it=0;it<options_num;++it) {
				if(options_names[it] == atext) {
					options_names.erase(options_names.begin()+it, options_names.begin()+it+1);
					--options_num;
					return;
				}
			}
		}
		
		inline void addOptions(vector<string> atext) {
			const int len = atext.size();
			for(int it=0;it<len;++it) {
				options_names.push_back(atext[it]);
			}
			options_num += atext.size();
		}
		
		inline void removeOption(int index) {
			options_names.erase(options_names.begin()+index, options_names.begin()+index+1);
			--options_num;
		}
		
		inline void setOptions(vector<string> atext) {
			options_names = atext;
			options_num = atext.size();
		}
		
		inline void setDefaultSideProperties() {
			sprop_roundedList = false;
		}
		
		inline void setRoundedList(bool state) {
			sprop_roundedList = state;
		}
		
		inline vector<string> getOptions() {
			return options_names;
		}
		
		inline bool isAnyOptionContains(string val) {
			for(int it=0;it<options_num;++it) {
				if(options_names[it].find(val)!=std::string::npos) {
					return true;
				}
			}
			return false;
		}
		
		inline int getSelectionNumber() {
			return sel_option;
		}
		
		inline string getSelectedOption() {
			return options_names[sel_option];
		}
		
		inline int optionsNumber() {
			return options_num;
		}
		
		inline void setAction(action aact) {
			act = aact;
		}
		
		inline void nextOption() {
			++sel_option;
			if(sel_option>=options_num) {
				if(sprop_roundedList) {
					sel_option= 0;
				} else {
					sel_option= options_num-1;	
				}
			}
		}
		
		inline void prevOption() {
			--sel_option;
			if(sel_option<0) {
				if(sprop_roundedList) {
					sel_option = options_num-1;
				} else {
					sel_option = 0;	
				}
			}
		}
		
		inline void setFrameStyle(frame_style afs) {
			fs = afs;
		}
		
		inline void setMarkedFrameStyle(frame_style afs) {
			mfs = afs;
		}
		
		inline void setBackground(char ch) {
			defbg = ch;
		}
		
		inline void draw(bool state, context& arg) {
			int x, y;
			for(int ity=0;ity<objh;++ity) {
				for(int itx=0;itx<objw;++itx) {
					x = itx+objx+parentX;
					y = ity+objy+parentY;
					arg.get().put(x, y, defbg);
					if(state) {
						if(ity==0) {
							if(itx==0) {
								arg.get().put(x, y, mfs.top_left);
							} else if(itx==objw-1) {
								arg.get().put(x, y, mfs.top_right);
							} else {
								arg.get().put(x, y, mfs.horizontal);
							}
						} else if(ity==objh-1) {
							if(itx==0) {
								arg.get().put(x, y, mfs.bottom_left);
							} else if(itx==objw-1) {
								arg.get().put(x, y, mfs.bottom_right);
							} else {
								arg.get().put(x, y, mfs.horizontal);
							}
						} else {
							if(itx==0||itx==objw-1) {
								arg.get().put(x, y, mfs.vertical);
							}	
						}
					} else {
						if(ity==0) {
							if(itx==0) {
								arg.get().put(x, y, fs.top_left);
							} else if(itx==objw-1) {
								arg.get().put(x, y, fs.top_right);
							} else {
								arg.get().put(x, y, fs.horizontal);
							}
						} else if(ity==objh-1) {
							if(itx==0) {
								arg.get().put(x, y, fs.bottom_left);
							} else if(itx==objw-1) {
								arg.get().put(x, y, fs.bottom_right);
							} else {
								arg.get().put(x, y, fs.horizontal);
							}
						} else {
							if(itx==0||itx==objw-1) {
								arg.get().put(x, y, fs.vertical);
							}	
						}
					}
				}
				
				if(options_num<=0) {
					text = "";
				} else {
					text = options_names[sel_option];
					int textidbeg = sel_option-((objh-2)/2);
					
					if(textidbeg>options_num-1) {
						textidbeg=options_num-1;
					}
					if(textidbeg<0) {
						textidbeg=0;
					}
			
					for(int ity=1;ity<objh-1;++ity) {
						if(ity-1+textidbeg>=0 && ity-1+textidbeg<options_num) {
							arg.get().setCurPos(objx+(objw-5-text.size())/2+1+parentX, objy+ity+parentY);
							if(sel_option == ity-1+textidbeg) {
								if(state) {
									arg.get() << "-> ";
									arg.get() << options_names[ity-1+textidbeg];
									arg.get() << " <-";
								} else {
									arg.get() << " [ ";
									arg.get() << options_names[ity-1+textidbeg];
									arg.get() << " ] ";
								}
							} else {
								arg.get() << "   ";
								arg.get() << options_names[ity-1+textidbeg];
								arg.get() << "   ";	
							}	
						}	
					}
				
				}
				/*arg.get().setCurPos(objx+(objw-2-text.size())/2+1, objy+((objh-objh%2)/2));
				arg.get() << text;
				arg.get().moveCur(0, 1);*/
				
				/*if(state) {
					arg.get().setCurPos(objx+(objw-2-text.size())/2, objy+((objh-objh%2)/2)+1);
					arg.get() << "/|\\";
					arg.get().setCurPos(objx+(objw-2-text.size())/2, objy+((objh-objh%2)/2)-1);
					arg.get() << "\\|/";
				} else {
					arg.get().setCurPos(objx+(objw-2-text.size())/2+1, objy+((objh-objh%2)/2)+1);
					arg.get() << "|";
					arg.get().setCurPos(objx+(objw-2-text.size())/2+1, objy+((objh-objh%2)/2)-1);
					arg.get() << "|";
				}*/
			
				
				
			}
		}
		
		inline void onKeyEvent(context& arg, char keycode) {
			//test.setCurPos(3,3);
			//test<<"EXEC";
			//test.flush();
			switch(keycode) {
				/*up-arrow*/   case 80: nextOption(); act(arg, this); break;
				/*down-arrow*/ case 72: prevOption(); act(arg, this); break;
			}
		}
		
		inline void whenClicked(context& arg) {
			
		}
};		



class exlistbox : public scene_object {
	private:
		int objx, objy, objw, objh;
		char defbg;
		string text;
		frame_style fs;
		frame_style mfs;
		vector<string> options_names;
		int options_num;
		int sel_option;
		action act;
		bool sprop_roundedList;
		string main_text;
		int main_text_sel;
	
	public:
		
		exlistbox(int ax, int ay, int aw, int ah, vector<string> atext) {
			setDefaultSideProperties();
			act = null_action;
			sel_option=0;
			options_num = atext.size();
			options_names = atext;
			fs = default_frame_style;
			mfs = default_dbframe_style;
			defbg = ' ';
			text = atext[0];
			objx = ax;
			objy = ay;
			objw = aw;
			objh = ah;
			main_text = "...";
			main_text_sel = -1;
		}
		
		exlistbox(int ax, int ay, int aw, int ah) {
			setDefaultSideProperties();
			act = null_action;
			sel_option=0;
			options_num = 0;
			fs = default_frame_style;
			mfs = default_dbframe_style;
			defbg = ' ';
			text = "";
			objx = ax;
			objy = ay;
			objw = aw;
			objh = ah;
			main_text = "...";
			main_text_sel = -1;
		}
		
		exlistbox(int ax, int ay, int aw, int ah, int len, string atext[]) {
			setDefaultSideProperties();
			act = null_action;
			sel_option=0;
			setOptions(len, atext);
			fs = default_frame_style;
			mfs = default_dbframe_style;
			defbg = ' ';
			text = atext[0];
			objx = ax;
			objy = ay;
			objw = aw;
			objh = ah;
			main_text = "...";
			main_text_sel = -1;
		}
		
		exlistbox(int ax, int ay, int len, string atext[]) {
			setDefaultSideProperties();
			act = null_action;
			sel_option = 0;
			setOptions(len, atext);
			fs = default_frame_style;
			mfs = default_dbframe_style;
			defbg = ' ';
			text = atext[0];
			objx = ax;
			objy = ay;
			objw = text.size()+4;
			objh = 3;
			main_text = "...";
			main_text_sel = -1;
		}
		
		exlistbox(int ax, int ay, vector<string> atext) {
			setDefaultSideProperties();
			act = null_action;
			sel_option = 0;
			options_num = atext.size();
			options_names = atext;
			fs = default_frame_style;
			mfs = default_dbframe_style;
			defbg = ' ';
			text = atext[0];
			objx = ax;
			objy = ay;
			objw = text.size()+4;
			objh = 3;
			main_text = "...";
			main_text_sel = -1;
		}
		
		exlistbox(int ax, int ay) {
			setDefaultSideProperties();
			act = null_action;
			sel_option = 0;
			options_num = 0;
			fs = default_frame_style;
			mfs = default_dbframe_style;
			defbg = ' ';
			text = "";
			objx = ax;
			objy = ay;
			objw = text.size()+4;
			objh = 3;
			main_text = "...";
			main_text_sel = -1;
		}
		
				inline void moveOptions(context& arg, int delta) {
			if(delta>0) {
				for(int it=0;it<delta;++it) {
					nextOption();
					act(arg, this);
				}
			} else {
				for(int it=0;it<-delta;++it) {
					prevOption();
					act(arg, this);
				}
			}
		}

		inline void onMouseEventFocused(context& arg, mouse_info& mouse_data) {
			moveOptions(arg, mouse_data.wheel_pos_delta);
			mouse_data.wheel_pos_delta = 0;
		}

		inline ME_STATE onMouseEvent(context& arg, mouse_info& mouse_data) {
			if(mouse_data.x>=objx+parentX && mouse_data.x<=objx+parentX+objw) {
				if(mouse_data.y>=objy+parentY && mouse_data.y<=objy+parentY+objh) {
					return ME_FOCUSED;
				}
			}
			return ME_NULL;
		}

		inline void emptyOptions() {
			options_num = 0;
			options_names.clear();
		}
		
		inline void addOptions(int len, string atext[]) {
			for(int it=0;it<len;++it) {
				options_names.push_back(atext[it]);
			}
			options_num += len;
		}
		
		inline void setOptions(int len, string atext[]) {
			emptyOptions();
			addOptions(len, atext);
		}
		
		inline void overrideOption(int pos, string atext) {
			options_names[pos] = atext;
		}
		
		inline void addOption(string atext) {
			++options_num;
			options_names.push_back(atext);
		}
		
		inline void removeOption(string atext) {
			for(int it=0;it<options_num;++it) {
				if(options_names[it] == atext) {
					options_names.erase(options_names.begin()+it, options_names.begin()+it+1);
					--options_num;
					return;
				}
			}
		}
		
		inline void addOptions(vector<string> atext) {
			const int len = atext.size();
			for(int it=0;it<len;++it) {
				options_names.push_back(atext[it]);
			}
			options_num += atext.size();
		}
		
		inline void removeOption(int index) {
			options_names.erase(options_names.begin()+index, options_names.begin()+index+1);
			--options_num;
		}
		
		inline void setOptions(vector<string> atext) {
			options_names = atext;
			options_num = atext.size();
		}
		
		inline void setDefaultSideProperties() {
			sprop_roundedList = false;
		}
		
		inline void setRoundedList(bool state) {
			sprop_roundedList = state;
		}
		
		inline vector<string> getOptions() {
			return options_names;
		}
		
		inline bool isAnyOptionContains(string val) {
			for(int it=0;it<options_num;++it) {
				if(options_names[it].find(val)!=std::string::npos) {
					return true;
				}
			}
			return false;
		}
		
		inline void setDefaultOption(int index) {
			main_text = options_names[index];
			main_text_sel = index;
		}
		
		inline int getTempSelectionNumber() {
			return sel_option;
		}
		
		inline string getTempSelectedOption() {
			return options_names[sel_option];
		}
		
		inline int getSelectionNumber() {
			return main_text_sel;
		}
		
		inline string getSelectedOption() {
			if(main_text_sel<0) {
				return "";
			}
			return main_text;
		}
		
		inline int optionsNumber() {
			return options_num;
		}
		
		inline void setAction(action aact) {
			act = aact;
		}
		
		inline void nextOption() {
			++sel_option;
			if(sel_option>=options_num) {
				if(sprop_roundedList) {
					sel_option= 0;
				} else {
					sel_option= options_num-1;	
				}
			}
		}
		
		inline void prevOption() {
			--sel_option;
			if(sel_option<0) {
				if(sprop_roundedList) {
					sel_option = options_num-1;
				} else {
					sel_option = 0;	
				}
			}
		}
		
		inline void setFrameStyle(frame_style afs) {
			fs = afs;
		}
		
		inline void setMarkedFrameStyle(frame_style afs) {
			mfs = afs;
		}
		
		inline void setBackground(char ch) {
			defbg = ch;
		}
		
		inline void draw(bool state, context& arg) {
			int x, y;
			int tobjh = objh+2;
			for(int ity=0;ity<tobjh;++ity) {
				for(int itx=0;itx<objw;++itx) {
					x = itx+objx+parentX;
					y = ity+objy+parentY;
					arg.get().put(x, y, defbg);
					if(state) {
						if(ity==0) {
							if(itx==0) {
								arg.get().put(x, y, mfs.top_left);
							} else if(itx==objw-1) {
								arg.get().put(x, y, mfs.top_right);
							} else {
								arg.get().put(x, y, mfs.horizontal);
							}
						} else if(ity==tobjh-1) {
							if(itx==0) {
								arg.get().put(x, y, mfs.bottom_left);
							} else if(itx==objw-1) {
								arg.get().put(x, y, mfs.bottom_right);
							} else {
								arg.get().put(x, y, mfs.horizontal);
							}
						} else {
							if(itx==0||itx==objw-1) {
								arg.get().put(x, y, mfs.vertical);
							}	
						}
					} else {
						if(ity==0) {
							if(itx==0) {
								arg.get().put(x, y, fs.top_left);
							} else if(itx==objw-1) {
								arg.get().put(x, y, fs.top_right);
							} else {
								arg.get().put(x, y, fs.horizontal);
							}
						} else if(ity==tobjh-1) {
							if(itx==0) {
								arg.get().put(x, y, fs.bottom_left);
							} else if(itx==objw-1) {
								arg.get().put(x, y, fs.bottom_right);
							} else {
								arg.get().put(x, y, fs.horizontal);
							}
						} else {
							if(itx==0||itx==objw-1) {
								arg.get().put(x, y, fs.vertical);
							}	
						}
					}
				}
				
				arg.get().setCurPos(objx+(objw-5-text.size())/2+1+parentX, objy+1+parentY);
				arg.get() << "* "+main_text+" *";
				
				if(!state) {
					tobjh = 3;
					arg.get().setCurPos(objx+objw-5+parentX, objy+1+parentY);
					arg.get() << "\\/";
				} else {
					if(options_num<=0) {
						text = "";
					} else {
						text = options_names[sel_option];
						int textidbeg = sel_option-((tobjh-2)/2);
						
						if(textidbeg>options_num-1) {
							textidbeg=options_num-1;
						}
						if(textidbeg<0) {
							textidbeg=0;
						}
				
						for(int ity=1;ity<tobjh-2;++ity) {
							if(ity-1+textidbeg>=0 && ity-1+textidbeg<options_num) {
								arg.get().setCurPos(objx+(objw-5-text.size())/2+1+parentX, objy+ity+parentY+1);
								if(sel_option == ity-1+textidbeg) {
									if(state) {
										arg.get() << "-> ";
										arg.get() << options_names[ity-1+textidbeg];
										arg.get() << " <-";
									} else {
										arg.get() << " [ ";
										arg.get() << options_names[ity-1+textidbeg];
										arg.get() << " ] ";
									}
								} else {
									arg.get() << "   ";
									arg.get() << options_names[ity-1+textidbeg];
									arg.get() << "   ";	
								}	
							}	
						}
					
					}
				}
				/*arg.get().setCurPos(objx+(objw-2-text.size())/2+1, objy+((tobjh-tobjh%2)/2));
				arg.get() << text;
				arg.get().moveCur(0, 1);*/
				
				/*if(state) {
					arg.get().setCurPos(objx+(objw-2-text.size())/2, objy+((tobjh-tobjh%2)/2)+1);
					arg.get() << "/|\\";
					arg.get().setCurPos(objx+(objw-2-text.size())/2, objy+((tobjh-tobjh%2)/2)-1);
					arg.get() << "\\|/";
				} else {
					arg.get().setCurPos(objx+(objw-2-text.size())/2+1, objy+((tobjh-tobjh%2)/2)+1);
					arg.get() << "|";
					arg.get().setCurPos(objx+(objw-2-text.size())/2+1, objy+((tobjh-tobjh%2)/2)-1);
					arg.get() << "|";
				}*/
			
				
				
			}
		}
		
		inline void onKeyEvent(context& arg, char keycode) {
			//test.setCurPos(3,3);
			//test<<"EXEC";
			//test.flush();
			switch(keycode) {
				/*up-arrow*/   case 80: nextOption(); break;
				/*down-arrow*/ case 72: prevOption(); break;
			}
		}
		
		inline void whenClicked(context& arg) {
			main_text = getTempSelectedOption();
			main_text_sel = getTempSelectionNumber();
			act(arg, this);
		}
};		

