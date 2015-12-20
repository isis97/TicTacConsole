#define ME_STATE unsigned short
#define ME_FOCUSED 1
#define ME_TOPMOST_FOCUSED 2
#define ME_NULL 0
#define ME_DEFAULT 0
#define ME_NOT_FOCUSED 3

class scene_object {
	public:
		bool visibleState;
		int parentX;
		int parentY;
		void* parent = nullptr;
		objectid id;
		
		drawer post_drawer = null_drawer, pre_drawer = null_drawer;

		inline void predraw(context* scn) {
			if(pre_drawer.isActive()) {
				pre_drawer.draw(scn);
			}
		}

		inline void postdraw(context* scn) {
			if(post_drawer.isActive()) {
				post_drawer.draw(scn);
			}
		}

	public:

		inline void setPredrawer(drawer &dr) {
			pre_drawer = dr;
		}

		inline void setPostdrawer(drawer &dr) {
			post_drawer = dr;
		}

		inline void removePredrawer() {
			pre_drawer = null_drawer;
		}

		inline void removePostdrawer() {
			post_drawer = null_drawer;
		}

		scene_object() {
			id = nullid;
			visibleState = true;
			parentX = 0;
			parentY = 0;
		}
		
		inline void updateParentFocus() {
			if(parent!=nullptr) {
				scene_object* so = (scene_object*)parent;
				//if(so->isContainer()) {
					so->containerRefocus();
				//}
			}
		}
		
		inline bool isVisible() {
			return visibleState;
		}
		
		inline void show(bool state) {
			visibleState = state;
		}
		
		inline void* getParent() {
			return parent;
		}
		
		inline void registerParent(void* arg) {
			parent = arg;
		}
		
		virtual inline void close() {
			visibleState = false;
			updateParentFocus();
		}
		
		virtual inline void open() {
			visibleState = true;
			updateParentFocus();
		}
		
		void updateParentPosition(int x, int y) {
			parentX = x;
			parentY = y;		
		}
		
		virtual void draw(context& arg) {}
		virtual void draw(bool state, context& arg) {}
		virtual bool isFocusable(context& arg) {return true;}
		virtual void whenClicked(context& arg) {}
		virtual void onKeyEvent(context& arg, char keycode) {};
		virtual void onKeyEventAny(context& arg, char keycode) {};
		virtual ME_STATE onMouseEvent(context& arg, mouse_info& mouse_data) {return ME_DEFAULT;};
		virtual void onMouseEventFocused(context& arg, mouse_info& mouse_data) {}
		virtual bool tryTakeFocus(context& arg, bool direction) {return true;};
		virtual void whenFocused(context& arg) {};
		//virtual bool isContainer() {return false;}
		virtual void containerRefocus() {}
		virtual void containerFocus(void* tgt) {}

		
		virtual inline void flush() {}
		
};



/*class scene : public scene_object {
	private:
		vector<scene_object*>objs;
		int objs_num;
		int mark = 0;
		int posx = 0;
		int posy = 0;
		context * scn;
		
	public:
		scene(int x, int y);
		
		inline void flush();
		
		inline int getX();
		
		inline int getY();
		
		inline scene_object* find( objectid id );
		
		inline void markNext();
		
		inline void containerFocus(void* tgt);
		
		inline void focus(void* x);
		
		inline void click();
		
		inline void markPrev();
		
		inline void add(scene_object& so);
		
		inline void draw(context& arg);
		
		inline void onKeyEventAny(context& arg, char keycode);
		
		inline void empty();
		
		inline bool isFocusable(context& arg);
		
		inline void containerRefocus();
};*/

#include "animation.h"

class context {
	private:
		vector<scene_object*>objs;
		int objs_num;
		console *cons = NULL;
		int mark = 0;
		bool process_key_input__ = true;

		chanim* changeAnimation = nullptr;

	public:

		context(console& arg_cons) {
			objs_num = 0;
			cons = &arg_cons;
		}
		
		/*inline void showOnly(void* x) {
			for(int it=0;it<objs_num;++it) {
				if(objs[it] == x) {
					objs[it]->show(true);
				} else {
					objs[it]->show(false);
				}
			}
		}*/

		inline void clear() {
			cons->clear();
		}

		inline void clearKeyboardInput() {
			cons->readKeyboardInput();
		}

		inline char readKeyboardInput() {
			return cons->readKeyboardInput();
		}

		inline void processKeyInput(bool state = true) {
			process_key_input__ = state;
		}

		inline void update(char key) {
			updateMouseInput();
			//char key = cons->getKeyboardInput();
			if(process_key_input__) onKeyEvent(key);
		}

		inline void updateMouseInput() {
			cons->updateMousePosition();
			if(!((cons->use_mouse__)&&(!cons->block_mouse__))) {
				return;
			}
			for(int it=0;it<objs_num;++it) {
				if(objs[it]->isVisible()) {
					objs[it]->onMouseEvent(*this, cons->getMouseInfo());
				}
			}
		}

		inline void showOnlyEx(void* x) {
			int lastshown = -1;
			int lasthidden = -1;
			for(int it=0;it<objs_num;++it) {
				if(objs[it] == x) {
					objs[it]->show(true);
				} else {
					objs[it]->show(false);
				}
			}
		}

		inline void executeFreeAnimation(scene_object* emptyscene, chanim* anm) {
			add(*emptyscene);
			anm->run(emptyscene);
			remove(*emptyscene);
		}
		
		inline void showOnlyEx(void* x, chanim * anm) {
			int lastshown = -1;
			int lasthidden = -1;
			for(int it=0;it<objs_num;++it) {
				if(objs[it] == x) {
					if(objs[it]->isVisible()) {
						
					} else {
						//objs[it]->show(true);
						if(lastshown==-1) lastshown = it;
					}
					objs[it]->show(false);
				} else {
					if(objs[it]->isVisible()) {
						//objs[it]->show(false);
						if(lasthidden==-1) lasthidden = it;
					} else {
						
					}
					objs[it]->show(false);
				}
			}
			
			if(lasthidden>=0 && lastshown>=0) {
				objs[lasthidden]->show(true);
				objs[lastshown]->show(true);
				anm->run( objs[lasthidden], objs[lastshown] );
				/*objs[lasthidden]->show(false);
				showNothing();
				objs[lastshown]->show(true);
				objs[lastshown]->flush();
				cons->flush();
				cout.flush();*/
				return;
			} else {
				objs[lastshown]->show(true);
				anm->run( objs[lastshown] );
			}
			cons->flush();
			cout.flush();
			/* else if(lastshown>=0) {
				objs[lastshown]->show(true);
				anm->run( objs[lastshown] );
				objs[lastshown]->show(true);
			}*/
			
		}
		
		inline void setChangeAnimation(chanim *anim) {
			changeAnimation = anim;
		}

		inline void removeChangeAnimation() {
			changeAnimation = nullptr;
		}

		inline void showOnly(void* x) {
			if(changeAnimation == nullptr) {
				showOnlyEx(x);
			} else {
				showOnlyEx(x, changeAnimation);
			}
			
		}
		
		inline void showNothing() {
			for(int it=0;it<objs_num;++it) {
				objs[it]->show(false);
			}
		}
		
		inline void showAll() {
			for(int it=0;it<objs_num;++it) {
				objs[it]->show(true);
			}
		}
		
		inline console& get() {
			return *cons;
		}
		
		inline void markNext() {
			if(!objs[mark]->tryTakeFocus(*this, true)) {
				return;
			}
			if(mark+1<objs_num) {
				if(objs[mark+1]->isFocusable(*this)&&objs[mark+1]->isVisible()) {
					objs[mark+1]->whenFocused(*this);
					++mark;
				} else {
					if(mark+2<objs_num) {
						++mark;
						markNext();	
					}
				}
			}
		}
		
		inline void click() {
			if(objs[mark]->isFocusable(*this)) {
				objs[mark]->whenClicked(*this);
			}
		}
		
		inline void markPrev() {
			if(!objs[mark]->tryTakeFocus(*this, false)) {
				return;
			}
			if(mark-1>=0) {
				if(objs[mark-1]->isFocusable(*this)&&objs[mark-1]->isVisible()) {
					objs[mark-1]->whenFocused(*this);
					--mark;
				} else {
					if(mark-2>=0) {
						--mark;
						markPrev();	
					}
				}
			}
		}
		
		inline void add(scene_object& so) {
			so.registerParent(this);
			objs.push_back(&so);
			++objs_num;
		}

		inline void remove(scene_object& so) {
			for(int it=0;it<objs_num;++it) {
				if((objs[it]) == (&so)) {
					 objs.erase(objs.begin()+it);
					 --objs_num;
					 return;
				}
			}
		}
		
		inline void draw() {	
			(*cons).setCurPos(3,3);
			for(int it=0;it<objs_num;++it) {
				if(objs[it]->isVisible()) {	
					if(objs[it]->isFocusable(*this)) {
						if(mark==it) {
							objs[it]->draw(true, *this);
						} else {
							objs[it]->draw(false, *this);	
						}
					} else {
						(*objs[it]).draw(*this);
					}
				}
			}

			if(cons->getMouseInfo().x>=0 && cons->getMouseInfo().y>=0 && cons->getMouseInfo().x<cons->getScreenBufferW() && cons->getMouseInfo().y<cons->getScreenBufferH()) {
				cons->setCurPos(cons->getMouseInfo().x, cons->getMouseInfo().y);
				(*cons) << " __\n";
				(*cons) << "/\\ \n";
				(*cons) << "  \\\n";
			}
			cons->flush();
			cout.flush();
		}
		
		inline void onKeyEvent(char keycode) {
			if((cons->use_mouse__)&&(!cons->block_mouse__)) {
				return;
			}
			for(int it=0;it<objs_num;++it) {
				if(it==mark) objs[it]->onKeyEvent(*this, keycode);
				if(objs[it]->isVisible()) {
					objs[it]->onKeyEventAny(*this, keycode);
				}
			}
		}
		
		inline void empty() {
			objs.clear();
		}
};



class scene : public scene_object {
	private:
		vector<scene_object*>objs;
		int objs_num;
		int mark = 0;
		int posx = 0;
		int posy = 0;
		context * scn = NULL;

	public:

		scene(int x, int y) {
			posx = x;
			posy = y;
			objs_num = 0;
		}
		
		inline void flush() {
			test.clear();
			if(scn!=NULL) {
				scn->draw();
				scn->get().flush();
			}
			cout.flush();
		}
		
		inline int getX() {
			return posx;
		}
		
		inline int getY() {
			return posy;
		}


		inline scene_object* find( objectid id ) {
			for(int it=0;it<objs_num;++it) {
				if(objs[it]->id == id) {	
					return objs[it];
				}
			}
			return nullptr;
		}
		
		inline void markNext() {
			if(!objs[mark]->tryTakeFocus(*scn, true)) {
				return;
			}
			if(mark+1<objs_num) {
				if(objs[mark+1]->isFocusable(*scn)&&objs[mark+1]->isVisible()) {
					objs[mark+1]->whenFocused(*scn);
					++mark;
				} else {
					if(mark+2<objs_num) {
						++mark;
						markNext();	
					}
				}
			}
		}
		
		inline void containerFocus(void* tgt) {
			focus(tgt);
		}
		
		inline bool isFocused(void* x) {
			if(mark>=0 && mark<objs_num) {
				return ( ((void*)objs[mark]) == (x));
			} else {
				return false;
			}
		}

		inline int getFocusIndex() {
			return mark;
		}

		inline void focus(void* x) {
			int index = -1;
			for(int it=0;it<objs_num;++it) {
				if(objs[it] == x) {
					if(objs[it]->isFocusable(*scn)) {
						index = it;
					}
				}
			}
			if(index<0) {
				return;
			}
			mark = index;
		}
		
		inline void click() {
			if(mark>=0&&mark<objs_num) {
				if(objs[mark]->isFocusable(*scn)) {
					objs[mark]->whenClicked(*scn);
				}
			}
		}
		
		inline void markPrev() {
			if(!objs[mark]->tryTakeFocus(*scn, false)) {
				return;
			}
			if(mark-1>=0) {
				if(objs[mark-1]->isFocusable(*scn)&&objs[mark-1]->isVisible()) {
					objs[mark-1]->whenFocused(*scn);
					--mark;
				} else {
					if(mark-2>=0) {
						--mark;
						markPrev();	
					}
				}
			}
		}
		
		inline void add(scene_object& so) {
			so.registerParent(this);
			objs.push_back(&so);
			++objs_num;
		}
		
		inline void draw(context& arg) {
			if(visibleState==false) return;
			scn = &arg;
			predraw(scn);
			for(int it=0;it<objs_num;++it) {
				if(objs[it]->isVisible()) {	
					objs[it]->updateParentPosition(posx+parentX, posy+parentY);
					if(objs[it]->isFocusable(*scn)) {
						if(mark==it) {
							objs[it]->draw(true, *scn);
						} else {
							objs[it]->draw(false, *scn);	
						}
					} else {
						(*objs[it]).draw(*scn);
					}
				}
			}
			postdraw(scn);
		}
		
		inline void validateMarker() {
			if(mark<0) mark = 0;
			if(mark>=objs_num) mark = objs_num-1;
		}

		inline void onKeyEventAny(context& arg, char keycode) {
			if(!visibleState) {
				return;
			}
			validateMarker();
			switch(keycode) {
				case 75: markPrev(); break;
				case 72: break;
				case 77: markNext(); break;
				case 80: break;
				case 13: click(); break;
				default: break;
			}
			for(int it=0;it<objs_num;++it) {
				if(it==mark) objs[it]->onKeyEvent(*scn, keycode);
				//objs[it]->onKeyEventAny(*scn, keycode);
				if(objs[it]->isVisible()) {
					objs[it]->onKeyEventAny(*scn, keycode);
				}
			}
		}

		inline ME_STATE onMouseEvent(context& arg, mouse_info& mouse_data) {
			if(!visibleState) {
				return ME_NULL;
			}
			bool wasfocused = false;
			for(int it=0;it<objs_num;++it) {
				if(objs[it]->isVisible()) {
					ME_STATE mestate = objs[it]->onMouseEvent(*scn, mouse_data);
					if(mestate == ME_FOCUSED && !wasfocused) {
						mark = it;
						wasfocused = true;
					}
					if(mestate == ME_NOT_FOCUSED && !wasfocused) {
						mark = -1;
					}
					if(mestate == ME_TOPMOST_FOCUSED) {
						mark = it;
						wasfocused = true;
						break;
					}
				}
			}
			if(mark>=0&&mark<objs_num) {
				objs[mark]->onMouseEventFocused(*scn, mouse_data);
			}
			if(mouse_data.lbutton&&mouse_data.lbutton_changed) {
				click();
			}
			return ME_NULL;
		}
		
		inline void empty() {
			objs.clear();
		}
		
		inline bool isFocusable(context& arg) {
			return false;
		}
		
		inline void containerRefocus() {
			if(mark<objs_num-1) {
				markNext();
			} else {
				markPrev();
			}
		}
};



class window : public scene_object {
	private:
		vector<scene_object*>objs;
		int objs_num;
		int mark = 0;
		int posx = 0;
		int posy = 0;
		int wndw = 0;
		int wndh = 0;
		context * scn;
		frame_style fs;
		char defbg;
		bool topmost;
		
	public:
		window(int x, int y, int w, int h) {
			open();
			close();
			open();
			close();
			topmost = false;
			fs = default_frame_style;
			defbg = ' ';
			posx = x;
			posy = y;
			wndw = w;
			wndh = h;
			objs_num = 0;
		}
		
		inline bool isTopmost() {
			return topmost;
		}
		
		inline void setTopmost(bool state) {
			topmost = state;
		}
		
		inline void setFrameStyle(frame_style afs) {
			fs = afs;
		}
		
		inline void setBackground(char ch) {
			defbg = ch;
		}
		
		inline int getX() {
			return posx;
		}
		
		inline int getY() {
			return posy;
		}
		
		inline int getW() {
			return wndw;
		}
		
		inline int getH() {
			return wndh;
		}
		
		inline void open() {
			mark = -1;
			visibleState = true;
			if(parent!=nullptr) {
				scene_object* so = (scene_object*)parent;
				so->containerFocus(this);
			}
		}
		
		inline void close() {
			visibleState = false;
			mark = -1;
		}
		
		inline void markNext() {
			if(!objs[mark]->tryTakeFocus(*scn, true)) {
				return;
			}
			if(mark+1<objs_num) {
				if(objs[mark+1]->isFocusable(*scn)&&objs[mark+1]->isVisible()) {
					objs[mark+1]->whenFocused(*scn);
					++mark;
				} else {
					if(mark+2<objs_num) {
						++mark;
						markNext();	
					}
				}
			}
		}
		
		inline void click() {
			if(mark>=0&&mark<objs_num) {
				if(objs[mark]->isFocusable(*scn)) {
					objs[mark]->whenClicked(*scn);
				}
			}
		}
		
		inline void markPrev() {
			if(!objs[mark]->tryTakeFocus(*scn, false)) {
				return;
			}
			if(mark-1>=0) {
				if(objs[mark-1]->isFocusable(*scn)&&objs[mark-1]->isVisible()) {
					objs[mark-1]->whenFocused(*scn);
					--mark;
				} else {
					if(mark-2>=0) {
						--mark;
						markPrev();	
					}
				}
			}
		}
		
		inline void add(scene_object& so) {
			so.registerParent(this);
			objs.push_back(&so);
			++objs_num;
		}
		
		inline void whenFocused(context& arg) {
			mark = -1;
		}
		
		inline scene_object* find( objectid id ) {
			for(int it=0;it<objs_num;++it) {
				if(objs[it]->id == id) {	
					return objs[it];
				}
			}
			return nullptr;
		}
		
		
		inline void draw(bool state, context& arg) {
			if(visibleState==false) return;
			
			//arg.get().print(posx+parentX, posy+parentY, "[X]");
			int x, y;
			for(int ity=0;ity<wndh;++ity) {
				for(int itx=0;itx<wndw;++itx) {
					x = itx+posx+parentX;
					y = ity+posy+parentY;
					if(defbg!=0x00) {
						arg.get().put(x, y, defbg);
					}
					if(ity==0) {
						if(itx==0) {
							arg.get().put(x, y, fs.top_left);
						} else if(itx==wndw-1) {
							arg.get().put(x, y, fs.top_right);
						} else {
							arg.get().put(x, y, fs.horizontal);
						}
					} else if(ity==wndh-1) {
						if(itx==0) {
							arg.get().put(x, y, fs.bottom_left);
						} else if(itx==wndw-1) {
							arg.get().put(x, y, fs.bottom_right);
						} else {
							arg.get().put(x, y, fs.horizontal);
						}
					} else {
						if(itx==0||itx==wndw-1) {
							arg.get().put(x, y, fs.vertical);
						}	
					}
					
				}
			}
			
			scn = &arg;
			for(int it=0;it<objs_num;++it) {
				if(objs[it]->isVisible()) {	
					objs[it]->updateParentPosition(posx+parentX, posy+parentY);
					if(objs[it]->isFocusable(*scn)) {
						if(mark==it&&state) {
							objs[it]->draw(true, *scn);
						} else {
							objs[it]->draw(false, *scn);	
						}
					} else {
						(*objs[it]).draw(*scn);
					}
				}
			}
		}
		
		inline void validateMarker() {
			if(mark<0) mark = 0;
			if(mark>=objs_num) mark = objs_num-1;
		}

		inline void onKeyEventAny(context& arg, char keycode) {
			if(visibleState&&mark<0) {
				mark = 0;
				return;
			}
			validateMarker();
			//cout<<"KEY EVENT REGISTERED!\n";
			//cout.flush();
			//getch();
			if(!visibleState) return;
			
			switch(keycode) {
				//case 75: markPrev(); break;
				//case 72: break;
				//case 77: markNext(); break;
				//case 80: break;
				case 13: click(); break;
				//default: break;
			}
			//return;
			for(int it=0;it<objs_num;++it) {
				if(it==mark) objs[it]->onKeyEvent(*scn, keycode);
				//objs[it]->onKeyEventAny(*scn, keycode);
				if(objs[it]->isVisible()) {
					objs[it]->onKeyEventAny(*scn, keycode);
				}
			}
		}

		inline ME_STATE onMouseEvent(context& arg, mouse_info& mouse_data) {
			//if(visibleState&&mark<0) {
			//	mark = -1;
			//	return ME_NOT_FOCUSED;
			//}
			if(!visibleState) {
				mark = -1;
				return ME_NOT_FOCUSED;
			}
			bool wasfocused = false;
			for(int it=0;it<objs_num;++it) {
				if(objs[it]->isVisible()) {
					ME_STATE mestate = objs[it]->onMouseEvent(*scn, mouse_data);
					if(mestate == ME_FOCUSED && !wasfocused) {
						mark = it;
						wasfocused = true;
					}
					if(mestate == ME_NOT_FOCUSED && !wasfocused) {
						mark = -1;
					}
					if(mestate == ME_TOPMOST_FOCUSED) {
						mark = it;
						wasfocused = true;
						break;
					}
				}
			}
			if(!wasfocused) {
				mark = -1;
				if(topmost) {
					return ME_TOPMOST_FOCUSED;
				}
				return ME_FOCUSED;
			}
			if(mark>=0&&mark<objs_num) {
				objs[mark]->onMouseEventFocused(*scn, mouse_data);
			}
			if(mouse_data.lbutton && mouse_data.lbutton_changed) {
				click();
			}
			if(topmost) {
				return ME_TOPMOST_FOCUSED;
			}
			return ME_FOCUSED;
		}
		
		inline void empty() {
			objs.clear();
		}
		
		inline bool isFocusable(context& arg) {
			return true;
		}
		
		/*inline void close() {
			visibleState = false;
		}*/
		
		inline bool tryTakeFocus(context& arg, bool direction) {
			if(!visibleState) {
				return true;
			}
			if(direction) {
				if(mark<objs_num-1) {
					markNext();
					return false;
				}
				if(topmost) {
					return false;
				} else {
					return true;
				}
			} else {
				if(mark>=1) {
					markPrev();
					return false;
				}	
				if(topmost) {
					return false;
				} else {
					return true;
				}
			}
			
			if(topmost) {
				return false;
			} else {
				return true;
			}
		}
};


#include "animations.h"


