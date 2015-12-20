inline void validateCurPos() {
	if(markx<0) {
		markx = 0;
	}
	if(marky<0) {
		marky = 0;
	}
	if(markx>3*nx-1) {
		markx = 3*nx-1;
	}
	if(marky>3*ny-1) {
		marky = 3*ny-1;
	}
}

inline CHAR_INFO& get_board_field(int itx, int ity) {
	test.setCurPos((3+8*(itx+3-itx%3)/3)+(itx%3)*2, (1+6*(ity-ity%3)/3)+(ity%3)*2);
	return test.get();
	//return test.get((3+8*(itx+3-itx%3)/3)+(itx%3)*2, (1+6*(ity-ity%3)/3)+(ity%3)*2);
}


struct field {
	int x;
	int y;
	int move_sym;// = -1;
} null_field = {-1, -1, -1}, unknown_field = {-999, -999, -999};

inline bool operator~(field a) {
	return levmap[a.y*nx*3+a.x] > 0;
}

inline bool operator==(field a, field b) {
	return (a.x==b.x && a.y==b.y);
}

inline bool operator!=(field a, field b) {
	return !(a.x==b.x && a.y==b.y);
}

inline short int& operator!(field a) {
	lastlevmapasgn = a.y*nx*3+a.x;
	return levmap[a.y*nx*3+a.x];
}

inline vector<field> remove_same_group( int gr, vector<field>f ) {
	const int siz = f.size();
	vector<field>ret;
	for(int it=0;it<siz;++it) {
		if(f[it].move_sym != gr) {
			ret.push_back(f[it]);
		}
	}
	return ret;
}

struct sboard {
	int fields[9];
	int x = -1;
	int y = -1;
} null_board, unknown_board;//; = {int[]{-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}, -1, -1}

inline bool operator==(sboard a, sboard b) {
	return (a.x==b.x);
}

inline bool operator!=(sboard a, sboard b) {
	return (a.x!=b.x);
}
//unknown_board.fields = {-1, -1, -1, -1, -1, -1, -1, -1, -1};

vector<field> winner_checks;
vector<field> attacker_checks;

inline int get_field_priority(field arg) {
	int ret = 0;

	field f = { ((arg.x-(arg.x%3))/3), ((arg.y-(arg.y%3))/3)  };
	field z = { (arg.x%3), (arg.y%3)  };
	if(f.x+2<3*nx) {
		++ret;
	}
	if(f.x+2<3*nx&&f.y+2<3*ny) {
		++ret;
	}
	if(f.x-2>=0) {
		++ret;
	}
	if(f.x-2>=0*nx&&f.y+2<3*ny) {
		++ret;
	}
	if(f.y+2<3*ny) {
		++ret;
	}
	if(f.x+2<3*nx&&f.y-2>=0) {
		++ret;
	}
	if(f.y-2>=0) {
		++ret;
	}
	if(f.x-2>=0*nx&&f.y-2>=0) {
		++ret;
	}
	if(f.x+1<3*nx&&f.x-1>=0) {
		++ret;
	}
	if(f.y+1<3*ny&&f.y-1>=0) {
		++ret;
	}
	if(z.x==0&&z.y==0) {
		ret+=3;
	}
	if(z.x==1&&z.y==0) {
		ret+=2;
	}
	if(z.x==2&&z.y==0) {
		ret+=3;
	}
	if(z.x==0&&z.y==1) {
		ret+=2;
	}
	if(z.x==1&&z.y==1) {
		ret+=4;
	}
	if(z.x==2&&z.y==1) {
		ret+=2;
	}
	if(z.x==0&&z.y==2) {
		ret+=3;
	}
	if(z.x==1&&z.y==2) {
		ret+=2;
	}
	if(z.x==2&&z.y==2) {
		ret+=3;
	}

	return ret;
}


struct sort_operator_field_priority__ {
  inline bool operator()(const field& i, const field& j) {
  		return (get_field_priority(i)>get_field_priority(j));
  }
} sort_operator_field_priority;

struct sort_operator_time_and_field_priority__ {
  inline bool operator()(const field& i, const field& j) {

		if(getPlayerMoveRelativeTime(i.move_sym)<getPlayerMoveRelativeTime(j.move_sym)) {
			return true;
		}
  		return false;

	  	/*if(i.x>=0&&i.y>=0&&i.x<3*nx&&i.y<3*ny) {
	  		return false;
	  	}
	  	if(j.x>=0&&j.y>=0&&j.x<3*nx&&j.y<3*ny) {
	  		return true;
	  	}*/
		  /*test.setCurPos(50,1);
		  test<<"( "<<i.x<<" x "<<i.y<<" )\n";
		  test<<"( "<<j.x<<" x "<<j.y<<" )\n";

		  test<<" checking time of "<<i.move_sym<<" and "<<j.move_sym<<" ("<<getPlayerMoveRelativeTime(i.move_sym)<<", "<<getPlayerMoveRelativeTime(j.move_sym)<<")\n";
		  test<<"<[PRESS ANY KEY]>\n";
		  getch();
		  test.flush();*/

		/*if(getPlayerMoveRelativeTime(i.move_sym)>0 && getPlayerMoveRelativeTime(j.move_sym)>0) {
			if(getPlayerMoveRelativeTime(i.move_sym)<getPlayerMoveRelativeTime(j.move_sym)) {
				return true;
			}
	  	}
  		if(get_field_priority(i)>get_field_priority(j)) {
  			return true;
  		}*/
  		//return false;
  }
} sort_operator_time_and_field_priority;

inline void sort_by_field_priority(vector<field>& arg) {
	sort(arg.begin(), arg.end(), sort_operator_field_priority);
}

inline void sort_by_time_and_field_priority(vector<field>& arg) {
	sort(arg.begin(), arg.end(), sort_operator_time_and_field_priority);
}

/*inline field make_field(sboard board, int a) {
	if(board==unknown_board) {
		return unknown_field;
	}
	field ret;
	ret.x = 3*board.x+(a%3);
	ret.y = 3*board.y+((a-a%3)/3);
	return ret;
}*/

inline field make_field(sboard board, int a) {
	if(board==unknown_board) {
		return unknown_field;
	}
	field ret;
	ret.x = 3*board.x+(a%3);
	ret.y = 3*board.y+((a-a%3)/3);
	//ret.move_sym = sym;
	return ret;
}

inline field make_field_sym(sboard board, int a, char sym) {
	if(board==unknown_board) {
		return unknown_field;
	}
	field ret;
	ret.x = 3*board.x+(a%3);
	ret.y = 3*board.y+((a-a%3)/3);
	ret.move_sym = sym;
	return ret;
}

inline field make_field(sboard board, int x, int y) {
	if(board == unknown_board) {
		return unknown_field;
	}
	field ret;
	ret.x = 3*board.x+x;
	ret.y = 3*board.y+y;
	return ret;
}

inline field make_field(int x, int y) {
	field ret;
	ret.x = x;
	ret.y = y;
	return ret;
}

inline short levmap_get(int x, int y) {
	return levmap[y*(nx*3)+x];
}

inline void levmap_set(int x, int y, int a) {
	levmap[y*(nx*3)+x] = a;
}

inline void add_win_marker(field f) {
	if(f.x!=-1&&f.y!=-1) {
		winner_checks.push_back(f);
	}
}

inline sboard get_sboard(int x, int y) {
	sboard ret;
	if(x<0||y<0||x>nx||y>ny) {
		/*ret.x = -1;
		ret.y = -1;
		ret.fields[0] = 0;
		ret.fields[1] = 0;
		ret.fields[2] = 0;
		ret.fields[3] = 0;
		ret.fields[4] = 0;
		ret.fields[5] = 0;
		ret.fields[6] = 0;
		ret.fields[7] = 0;
		ret.fields[8] = 0;*/
		return unknown_board;
	}
	ret.x = x;
	ret.y = y;
	ret.fields[0] = levmap_get(x*3+0, y*3+0);
	ret.fields[1] = levmap_get(x*3+1, y*3+0);
	ret.fields[2] = levmap_get(x*3+2, y*3+0);
	ret.fields[3] = levmap_get(x*3+0, y*3+1);
	ret.fields[4] = levmap_get(x*3+1, y*3+1);
	ret.fields[5] = levmap_get(x*3+2, y*3+1);
	ret.fields[6] = levmap_get(x*3+0, y*3+2);
	ret.fields[7] = levmap_get(x*3+1, y*3+2);
	ret.fields[8] = levmap_get(x*3+2, y*3+2);
	return ret;
}

inline sboard rem_sboard(short sym, sboard arg) {
	sboard ret;
	for(int it=0;it<9;++it) {
		if(arg.fields[it]!=sym) {
			ret.fields[it]=0;
		} else {
			ret.fields[it]=sym;
		}
	}
	return ret;
}

inline sboard sum_sboards(short sym, sboard a, sboard b, sboard c) {
	sboard ret;
	for(int it=0;it<9;++it) {
		if(a.fields[it]==sym || b.fields[it]==sym || c.fields[it]==sym) {
			ret.fields[it] = sym;
		} else {
			ret.fields[it] = 0;
		}
	}
	return ret;
}

inline sboard gremb(int a, int b, int c) {
	sboard ret;
	for(int it=0;it<9;++it) {
		if(it==a||it==b||it==c) {
			ret.fields[it]=1;
		} else {
			ret.fields[it]=0;
		}
	}
	return ret;
}

inline pair<bool,sboard> check_win(sboard brd) {
	bool ret = false;

	check_field_line(brd, 0, 3, 6) {
		return make_pair(true, gremb(0, 3, 6));
	}
	check_field_line(brd, 0, 1, 2) {
		return make_pair(true, gremb(0, 1, 2));
	}
	check_field_line(brd, 2, 5, 8) {
		return make_pair(true, gremb(2, 5, 8));
	}
	check_field_line(brd, 6, 7, 8) {
		return make_pair(true, gremb(6, 7, 8));
	}
	check_field_line(brd, 0, 4, 8) {
		return make_pair(true, gremb(0, 4, 8));
	}
	check_field_line(brd, 2, 4, 6) {
		return make_pair(true, gremb(2, 4, 6));
	}
	check_field_line(brd, 1, 4, 7) {
		return make_pair(true, gremb(1, 4, 7));
	}
	check_field_line(brd, 3, 4, 5) {
		return make_pair(true, gremb(3, 4, 5));
	}
	return make_pair(false, null_board);
}


inline bool chceck_hlwin(short sym, sboard a, sboard b, sboard c) {
	if(sym==0) {
		return false;
	}
	for(int it=0;it<9;++it) {
		if(a.fields[it]==sym && b.fields[it]==sym && c.fields[it]==sym) {
			test.setCurPos(3,3);
			field f = make_field_sym(a,it, sym);
			add_win_marker(make_field_sym(a, it, sym));
			add_win_marker(make_field_sym(b, it, sym));
			add_win_marker(make_field_sym(c, it, sym));

			return true;
		}
	}
	return false;
}

inline void chceckatt_hlwin(short sym, sboard a, sboard b, sboard c) {
	if(sym==0) {
		return;
	}
	for(int it=0;it<9;++it) {
		if(a.fields[it]==sym&&b.fields[it]==sym) {
			attacker_checks.push_back(make_field_sym(c, it, sym));
		}
		if(a.fields[it]==sym&&c.fields[it]==sym) {
			attacker_checks.push_back(make_field_sym(b, it, sym));
		}
		if(b.fields[it]==sym&&c.fields[it]==sym) {
			attacker_checks.push_back(make_field_sym(a, it, sym));
		}
	}
	return;
}

inline bool check_win(short sym, sboard a, sboard b, sboard c) {
	if(a==unknown_board || b==unknown_board || c==unknown_board) {
		return false;
	}

	if(sym==0) {
		return false;
	}

	check_3dfield_line(sym,a,b,c,0,1,2);
	check_3dfield_line(sym,a,b,c,3,4,5);
	check_3dfield_line(sym,a,b,c,6,7,8);
	check_3dfield_line(sym,a,b,c,0,3,6);
	check_3dfield_line(sym,a,b,c,1,4,7);
	check_3dfield_line(sym,a,b,c,2,5,8);
	check_3dfield_line(sym,a,b,c,2,1,0);
	check_3dfield_line(sym,a,b,c,5,4,3);
	check_3dfield_line(sym,a,b,c,8,7,6);
	check_3dfield_line(sym,a,b,c,6,3,0);
	check_3dfield_line(sym,a,b,c,7,4,1);
	check_3dfield_line(sym,a,b,c,8,5,2);
	check_3dfield_line(sym,a,b,c,0,4,8);
	check_3dfield_line(sym,a,b,c,6,4,2);
	check_3dfield_line(sym,a,b,c,8,4,0);
	check_3dfield_line(sym,a,b,c,2,4,6);

	check_3dfield_line(sym,a,a,a,0,1,2);
	check_3dfield_line(sym,a,a,a,3,4,5);
	check_3dfield_line(sym,a,a,a,6,7,8);
	check_3dfield_line(sym,a,a,a,0,3,6);
	check_3dfield_line(sym,a,a,a,1,4,7);
	check_3dfield_line(sym,a,a,a,2,5,8);
	check_3dfield_line(sym,a,a,a,0,4,8);
	check_3dfield_line(sym,a,a,a,6,4,2);

	check_3dfield_line(sym,b,b,b,0,1,2);
	check_3dfield_line(sym,b,b,b,3,4,5);
	check_3dfield_line(sym,b,b,b,6,7,8);
	check_3dfield_line(sym,b,b,b,0,3,6);
	check_3dfield_line(sym,b,b,b,1,4,7);
	check_3dfield_line(sym,b,b,b,2,5,8);
	check_3dfield_line(sym,b,b,b,0,4,8);
	check_3dfield_line(sym,b,b,b,6,4,2);

	check_3dfield_line(sym,c,c,c,0,1,2);
	check_3dfield_line(sym,c,c,c,3,4,5);
	check_3dfield_line(sym,c,c,c,6,7,8);
	check_3dfield_line(sym,c,c,c,0,3,6);
	check_3dfield_line(sym,c,c,c,1,4,7);
	check_3dfield_line(sym,c,c,c,2,5,8);
	check_3dfield_line(sym,c,c,c,0,4,8);
	check_3dfield_line(sym,c,c,c,6,4,2);

	return chceck_hlwin(sym, a, b, c);
}


inline void check_attack(short sym, sboard a, sboard b, sboard c) {

	if(sym==0) {
		return;
	}

	checkatt_3dfield_line(sym,a,b,c,0,1,2);
	checkatt_3dfield_line(sym,a,b,c,3,4,5);
	checkatt_3dfield_line(sym,a,b,c,6,7,8);
	checkatt_3dfield_line(sym,a,b,c,0,3,6);
	checkatt_3dfield_line(sym,a,b,c,1,4,7);
	checkatt_3dfield_line(sym,a,b,c,2,5,8);
	checkatt_3dfield_line(sym,a,b,c,2,1,0);
	checkatt_3dfield_line(sym,a,b,c,5,4,3);
	checkatt_3dfield_line(sym,a,b,c,8,7,6);
	checkatt_3dfield_line(sym,a,b,c,6,3,0);
	checkatt_3dfield_line(sym,a,b,c,7,4,1);
	checkatt_3dfield_line(sym,a,b,c,8,5,2);
	checkatt_3dfield_line(sym,a,b,c,0,4,8);
	checkatt_3dfield_line(sym,a,b,c,6,4,2);
	checkatt_3dfield_line(sym,a,b,c,8,4,0);
	checkatt_3dfield_line(sym,a,b,c,2,4,6);

	checkatt_3dfield_line(sym,a,a,a,0,1,2);
	checkatt_3dfield_line(sym,a,a,a,3,4,5);
	checkatt_3dfield_line(sym,a,a,a,6,7,8);
	checkatt_3dfield_line(sym,a,a,a,0,3,6);
	checkatt_3dfield_line(sym,a,a,a,1,4,7);
	checkatt_3dfield_line(sym,a,a,a,2,5,8);
	checkatt_3dfield_line(sym,a,a,a,0,4,8);
	checkatt_3dfield_line(sym,a,a,a,6,4,2);

	checkatt_3dfield_line(sym,b,b,b,0,1,2);
	checkatt_3dfield_line(sym,b,b,b,3,4,5);
	checkatt_3dfield_line(sym,b,b,b,6,7,8);
	checkatt_3dfield_line(sym,b,b,b,0,3,6);
	checkatt_3dfield_line(sym,b,b,b,1,4,7);
	checkatt_3dfield_line(sym,b,b,b,2,5,8);
	checkatt_3dfield_line(sym,b,b,b,0,4,8);
	checkatt_3dfield_line(sym,b,b,b,6,4,2);

	checkatt_3dfield_line(sym,c,c,c,0,1,2);
	checkatt_3dfield_line(sym,c,c,c,3,4,5);
	checkatt_3dfield_line(sym,c,c,c,6,7,8);
	checkatt_3dfield_line(sym,c,c,c,0,3,6);
	checkatt_3dfield_line(sym,c,c,c,1,4,7);
	checkatt_3dfield_line(sym,c,c,c,2,5,8);
	checkatt_3dfield_line(sym,c,c,c,0,4,8);
	checkatt_3dfield_line(sym,c,c,c,6,4,2);

	chceckatt_hlwin(sym, a, b, c);
	return;
}



inline bool checkWinner(int x, int y) {
	short sym = levmap_get(x,y);
	int actsbrdx = (x-x%3)/3;
	int actsbrdy = (y-y%3)/3;

	if(sym==0) {
		return false;
	}

	sboard brd = get_sboard(actsbrdx, actsbrdy);
	sboard db, ub, lb, rb, flb, frb, fub, fdb, rucb, lucb, rdcb, ldcb, frucb, flucb, frdcb, fldcb;

	db = get_sboard(actsbrdx, actsbrdy-1);
	ub = get_sboard(actsbrdx, actsbrdy+1);
	lb = get_sboard(actsbrdx-1, actsbrdy);
	rb = get_sboard(actsbrdx+1, actsbrdy);
	ldcb = get_sboard(actsbrdx-1, actsbrdy-1);
	rdcb = get_sboard(actsbrdx+1, actsbrdy-1);
	lucb = get_sboard(actsbrdx-1, actsbrdy+1);
	rucb = get_sboard(actsbrdx+1, actsbrdy+1);

	fdb = get_sboard(actsbrdx, actsbrdy-2);
	fub = get_sboard(actsbrdx, actsbrdy+2);
	flb = get_sboard(actsbrdx-2, actsbrdy);
	frb = get_sboard(actsbrdx+2, actsbrdy);
	fldcb = get_sboard(actsbrdx-2, actsbrdy-2);
	frdcb = get_sboard(actsbrdx+2, actsbrdy-2);
	flucb = get_sboard(actsbrdx-2, actsbrdy+2);
	frucb = get_sboard(actsbrdx+2, actsbrdy+2);

	bool ret = false;

	ret = ret || check_win(sym, brd, ub, fub);
	ret = ret || check_win(sym, db, brd, ub);
	ret = ret || check_win(sym, brd, db, fdb);

	ret = ret || check_win(sym, brd, lb, flb);
	ret = ret || check_win(sym, lb, brd, rb);
	ret = ret || check_win(sym, brd, rb, frb);

	ret = ret || check_win(sym, brd, rucb, frucb);
	ret = ret || check_win(sym, ldcb, brd, rucb);
	ret = ret || check_win(sym, brd, ldcb, fldcb);

	ret = ret || check_win(sym, brd, lucb, flucb);
	ret = ret || check_win(sym, lucb, brd, rdcb);
	ret = ret || check_win(sym, brd, rdcb, frdcb);

	ret = ret || check_win(sym, rem_sboard(sym, brd), null_board, null_board);

	return ret;
}


inline void checkAttacker(int x, int y) {
	short sym = levmap_get(x,y);
	int actsbrdx = (x-x%3)/3;
	int actsbrdy = (y-y%3)/3;

	if(sym==0) {
		return;
	}

	sboard brd = get_sboard(actsbrdx, actsbrdy);
	sboard db, ub, lb, rb, flb, frb, fub, fdb, rucb, lucb, rdcb, ldcb, frucb, flucb, frdcb, fldcb;

	db = get_sboard(actsbrdx, actsbrdy-1);
	ub = get_sboard(actsbrdx, actsbrdy+1);
	lb = get_sboard(actsbrdx-1, actsbrdy);
	rb = get_sboard(actsbrdx+1, actsbrdy);
	ldcb = get_sboard(actsbrdx-1, actsbrdy-1);
	rdcb = get_sboard(actsbrdx+1, actsbrdy-1);
	lucb = get_sboard(actsbrdx-1, actsbrdy+1);
	rucb = get_sboard(actsbrdx+1, actsbrdy+1);

	fdb = get_sboard(actsbrdx, actsbrdy-2);
	fub = get_sboard(actsbrdx, actsbrdy+2);
	flb = get_sboard(actsbrdx-2, actsbrdy);
	frb = get_sboard(actsbrdx+2, actsbrdy);
	fldcb = get_sboard(actsbrdx-2, actsbrdy-2);
	frdcb = get_sboard(actsbrdx+2, actsbrdy-2);
	flucb = get_sboard(actsbrdx-2, actsbrdy+2);
	frucb = get_sboard(actsbrdx+2, actsbrdy+2);

	check_attack(sym, brd, ub, fub);
	check_attack(sym, db, brd, ub);
	check_attack(sym, brd, db, fdb);

	check_attack(sym, brd, lb, flb);
	check_attack(sym, lb, brd, rb);
	check_attack(sym, brd, rb, frb);

	check_attack(sym, brd, rucb, frucb);
	check_attack(sym, ldcb, brd, rucb);
	check_attack(sym, brd, ldcb, fldcb);

	check_attack(sym, brd, lucb, flucb);
	check_attack(sym, lucb, brd, rdcb);
	check_attack(sym, brd, rdcb, frdcb);

	(check_attack(sym, rem_sboard(sym, brd), null_board, null_board));

	return;
}

inline field getAttacker() {
	field pom;
	for(int id=0;id<playersNum-1;++id) {
		for(int x=0;x<nx*3;++x) {
			for(int y=0;y<ny*3;++y) {
				if(levmap_get(x,y) == getPlayerAfterMe(id)) {
					checkAttacker(x, y);
				}
			}
		}
	}
	return null_field;
}

inline bool isDraw() {
	for(int x=0;x<nx*3;++x) {
		for(int y=0;y<ny*3;++y) {
			if(levmap_get(x,y)==0) {
				return false;
			}
		}
	}
	is_game_made_with_draw = true;
	return true;
}

inline field getMeAttacker(char sym) {
	field pom;
	for(int x=0;x<nx*3;++x) {
		for(int y=0;y<ny*3;++y) {
			if(levmap_get(x,y)==sym) {
				checkAttacker(x, y);
			}
		}
	}
	return null_field;
}

inline bool isMeAttacking(char sym) {
	getMeAttacker(sym);
	const int siz = attacker_checks.size();
	for(int it=0;it<siz;++it) {
		if(attacker_checks[it].x>=0&&attacker_checks[it].y>=0&&attacker_checks[it].x<3*nx&&attacker_checks[it].y<3*ny) {
			if(attacker_checks[it]!=null_field&&attacker_checks[it]!=unknown_field) {
				if((!attacker_checks[it])==0) {
					return true;
				}
			}
		}
	}
	return false;

}

inline short getWinner() {
	for(int x=0;x<nx*3;++x) {
		for(int y=0;y<ny*3;++y) {
			if(checkWinner(x, y)&&levmap_get(x,y)!=0) {
				return levmap_get(x,y);
			}
		}
	}
	return 0;
}

inline void mark_winner() {
	const int siz = winner_checks.size();
	for(int it=0;it<siz;++it) {
		if(winner_checks[it].x>=0&&winner_checks[it].y>=0&&winner_checks[it].x<3*nx&&winner_checks[it].y<3*ny){
			  get_board_field(winner_checks[it].x, winner_checks[it].y).Attributes = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | BACKGROUND_RED | BACKGROUND_INTENSITY | COMMON_LVB_UNDERSCORE;
		}
	}
}

inline void demark_winner() {
	const int siz = winner_checks.size();
	for(int it=0;it<siz;++it) {
		if(winner_checks[it].x>=0&&winner_checks[it].y>=0&&winner_checks[it].x<3*nx&&winner_checks[it].y<3*ny) {
			get_board_field(winner_checks[it].x, winner_checks[it].y).Char.AsciiChar = '!';
			get_board_field(winner_checks[it].x, winner_checks[it].y).Attributes = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE;
		}
	}
}



/////////////////////////////////////////////////////////////////////////////////


int ec_enemynum = 0;
button ec_enemybuts[10];
string bfs_s_selchar_opt[] = AVAILABLE_SIGNS;
vector<string> bfs_s_selenemy_types;
vector<string> bfs_s_selenemy_types_descr;
vector<aienemy*> bfs_s_selenemy_types_initializers;

//string bfs_s_selenemy_types[] = { "A", "B", "C" };
//string bfs_s_selenemy_types_descr[] = { "@description A", "@description B", "@description C" };



inline void registerEnemyType( string name, string description, aienemy* initializer, bool listable ) {
	const int size = bfs_s_selenemy_types.size();
	for(int it=0;it<size;++it) {
		if(bfs_s_selenemy_types[it] == name) {
			return;
		}
	}
	if(!listable) {
		return;
	}
	bfs_s_selenemy_types.push_back(name);
	bfs_s_selenemy_types_descr.push_back(description);
	bfs_s_selenemy_types_initializers.push_back(initializer);
}

window debug_confirm_dlg(30,8,58,9);
textframe debug_confirm_dlg_textbox(1,1,56,6,"[???]");
button debug_confirm_dlg_yes(10,7,"Yes, turn on debug mode.");
button debug_confirm_dlg_no(35, 7,"No");

window enemy_edit_dlg(30,8,55,10);
button enemy_edit_dlg_b_ok(1,1,"OK!");
listbox enemy_edit_dlg_selmarker(10,1,30,8,32,bfs_s_selchar_opt);

window enemy_choose_error(30,8,58,9);
textframe enemy_choose_error_textbox(1,1,56,6,"[???]");
button enemy_choose_error_abort(10,7,"Understood.");

window enemy_choose_error2(30,8,58,9);
textframe enemy_choose_error2_textbox(1,1,56,6,"[???]");
button enemy_choose_error2_abort(10,7,"Understood.");
button enemy_choose_error2_ok(25,7,"Ok.");
//bool enemy_choose_error2_okstate = false;


window game_youre_moving(30,8,58,9);
textframe game_youre_moving_textbox(1,1,56,6,"[???]");
button game_youre_moving_abort(10,7,"Understood.");


class aienemy {

	public:
		char marker;
		bool running;
		int group = 0;
		bool listable;

		string enemy_typename;

		aienemy() {
			enemy_typename = "";
			listable = true;
		}

		inline void initClass(aienemy* ths) {
			initClass("", "", ths, false);
		}

		inline void initClass(string name, string description, aienemy* ths) {
			listable = true;
			enemy_typename = name;
			registerEnemyType(name, description, ths, ths->listable);
		}

		inline void initClass(string name, string description, aienemy* ths, const bool isListable) {
			listable = isListable;
			enemy_typename = name;
			registerEnemyType(name, description, ths, ths->listable);
		}

		inline virtual void nextStep() {}
		inline void move() {
			if(running) nextStep();
		}
		inline void setMarker( char arg ) {
			marker = arg;
		}
		inline void setGroup(int arg) {
			group = arg;
		}
		inline void run() {
			running = true;
		}
		inline void stop() {
			running = false;
		}
		inline virtual bool isUserEnemy() {
			return false;
		}
		inline virtual aienemy* factory_produceEnemy() {return this;}
		inline virtual void factory_clear() {}
		inline virtual void userPlayerMoveNotification() {}
		inline virtual bool isNetEnemy() {return false;}
		inline virtual void editProperties() {
			enemy_edit_dlg.setTopmost(true);
			enemy_edit_dlg.open();
			enemy_edit_dlg.show(true);
		};

};


class enemy_virtual : public aienemy {
	public:
		enemy_virtual() {
			initClass(this);
		}
		inline void nextStep() {

		}
		vector<enemy_virtual*> reg;
		inline aienemy* factory_produceEnemy() {
			enemy_virtual* pom;
			reg.push_back(pom);
			reg[reg.size()-1] = new enemy_virtual();
			return reg[reg.size()-1];
		}

} enemy_virtual_initializer;


inline aienemy* findEnemy(short marker) {
	for(int it=0;it<playersNum;++it) {
		if(playerAiReg[it]->marker==marker) {
			return playerAiReg[it];
		}
	}
	return nullptr;
}



inline char rndNewEnemyMarker() {
	string signs[] = AVAILABLE_SIGNS;
	int signs_size = 32;
	int siz = playerAiReg.size();
	for(int it=0;it<siz;++it) {
		for(int j=0;j<signs_size;++j) {
			if(playerAiReg[it]->marker==signs[j][0]) {
				signs[j] = "";
			}
		}
	}

	for(int j=0;j<signs_size;++j) {
		if(signs[j]!="") {
			return signs[j][0];
		}
	}
	string pom[] = AVAILABLE_SIGNS;
	return pom[0][0];

}

class userenemy : public aienemy {
	public:
		userenemy() {
			initClass("User player", "[User player]\nPlayer that is controlled only by user.", this);
		}
		inline void nextStep() {

		}
		inline bool isUserEnemy() {
			return true;
		}

		vector<userenemy*> reg;
		inline aienemy* factory_produceEnemy() {
			userenemy* pom;
			reg.push_back(pom);
			reg[reg.size()-1] = new userenemy();
			return reg[reg.size()-1];
		}

} mainplayer;





inline void registerPlayer(aienemy* arg) {
	playerAiReg.push_back(arg);
	++playersNum;
	arg->run();
}

inline void removeAllPlayers() {
	for(int it=0;it<playersNum;++it) {
		playerAiReg[it]->factory_clear();
	}
	playerAiReg.clear();
	playersNum = 0;
}

inline void putNewPlayer(aienemy* arg) {
	registerPlayer(arg->factory_produceEnemy());
}



inline aienemy* currentPlayer() {
	return playerAiReg[playerRegMove];
}

/*
vector<aienemy*> playerAiReg;
int playerRegMove = 0;
int playersNum = 0;
*/
inline int getPlayerMoveRelativeTime(char sym) {
	int pos = 0;
	for(int it=0;it<playersNum;++it) {
		pos = playerRegMove+it+1;
		if(pos>=playersNum) pos%=playersNum;
		if(playerAiReg[pos]->marker == sym) {
			if(currentPlayer()->group == playerAiReg[pos]->group) {
				return -9999;
			} else {
				return it+1;
			}
		}
	}
	return 9999;
}

void redraw();

inline void moveNextPlayer() {
	++playerRegMove;
	if(playerRegMove>=playersNum) {
		playerRegMove%=playersNum;
	}

	redraw();
	test.setCurPos(50,22);
	test<<"[ Player "<<playerAiReg[playerRegMove]->marker<<" is moving now... ]\n";
	test.flush();

	//test.flush();

	playerAiReg[playerRegMove]->move();
	if(!playerAiReg[playerRegMove]->isUserEnemy()) {

		if(wait_each_ai_enemy) {
			GetAsyncKeyState( VK_SHIFT );
			GetAsyncKeyState( VK_SHIFT );
			for(int t=0;t<1500;t+=10) {
				short key = GetAsyncKeyState( VK_SHIFT );
				if(key != 0) {
					wait_each_ai_enemy = false;
					GetAsyncKeyState( VK_SHIFT );
					return;
				}
				test.wait(10);
			}
			GetAsyncKeyState( VK_SHIFT );
		}
	}
}

void drawEndGameCom(string text);

inline bool moveAllPlayers_server() {

	if(is_net_game_client){
		return true;
	}
		if(isDraw()) {
			is_game_made = true;
			redraw();
			drawEndGameCom(string("")+"                _______  .______          ___   ____    __    ____ \n               |       \\ |   _  \\        /   \\  \\   \\  /  \\  /   / \n               |  .--.  ||  |_)  |      /  ^  \\  \\   \\/    \\/   /  \n               |  |  |  ||      /      /  /_\\  \\  \\            /   \n               |  '--'  ||  |\\  \\----./  _____  \\  \\    /\\    /    \n               |_______/ | _| `._____/__/     \\__\\  \\__/  \\__/     \n");
			test.flush();
			return false;
		}

		demark_winner();
		winner_checks.clear();
		short win = getWinner();
		if(win!=0) {

			int last = playerRegMove+1;
			if(last>=playersNum) {
				last-=playersNum;
			}

			if(!is_game_made) {
				if(!findEnemy(win)->isUserEnemy()) {
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
			return true;
		}

		moveNextPlayer();
		if(isDraw()) {
			is_game_made = true;
			redraw();
			//drawEndGameCom("Draw in the game!");
			drawEndGameCom(string("")+"                _______  .______          ___   ____    __    ____ \n               |       \\ |   _  \\        /   \\  \\   \\  /  \\  /   / \n               |  .--.  ||  |_)  |      /  ^  \\  \\   \\/    \\/   /  \n               |  |  |  ||      /      /  /_\\  \\  \\            /   \n               |  '--'  ||  |\\  \\----./  _____  \\  \\    /\\    /    \n               |_______/ | _| `._____/__/     \\__\\  \\__/  \\__/     \n");

			test.flush();
			return false;
		}

		demark_winner();
		winner_checks.clear();
		win = getWinner();
		if(win!=0) {

			int last = playerRegMove+1;
			if(last>=playersNum) {
				last-=playersNum;
			}

			if(!is_game_made) {
				if(!findEnemy(win)->isUserEnemy()) {
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
			return true;
		}

	return true;
}

inline bool moveAllPlayers() {

	if(is_net_game_client){
		return true;
	}

	while(true) {

		if(isDraw()) {
			is_game_made = true;
			redraw();
			drawEndGameCom(string("")+"                _______  .______          ___   ____    __    ____ \n               |       \\ |   _  \\        /   \\  \\   \\  /  \\  /   / \n               |  .--.  ||  |_)  |      /  ^  \\  \\   \\/    \\/   /  \n               |  |  |  ||      /      /  /_\\  \\  \\            /   \n               |  '--'  ||  |\\  \\----./  _____  \\  \\    /\\    /    \n               |_______/ | _| `._____/__/     \\__\\  \\__/  \\__/     \n");
			test.flush();
			return false;
		}

		demark_winner();
		winner_checks.clear();
		short win = getWinner();
		if(win!=0) {

			int last = playerRegMove+1;
			if(last>=playersNum) {
				last-=playersNum;
			}

			if(!is_game_made) {
				if(!findEnemy(win)->isUserEnemy()) {
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
			return true;
		}

		moveNextPlayer();

		if(isDraw()) {
			is_game_made = true;
			redraw();
			//drawEndGameCom("Draw in the game!");
			drawEndGameCom(string("")+"                _______  .______          ___   ____    __    ____ \n               |       \\ |   _  \\        /   \\  \\   \\  /  \\  /   / \n               |  .--.  ||  |_)  |      /  ^  \\  \\   \\/    \\/   /  \n               |  |  |  ||      /      /  /_\\  \\  \\            /   \n               |  '--'  ||  |\\  \\----./  _____  \\  \\    /\\    /    \n               |_______/ | _| `._____/__/     \\__\\  \\__/  \\__/     \n");

			test.flush();
			return false;
		}

		demark_winner();
		winner_checks.clear();
		win = getWinner();
		if(win!=0) {

			int last = playerRegMove+1;
			if(last>=playersNum) {
				last-=playersNum;
			}

			if(!is_game_made) {
				if(!findEnemy(win)->isUserEnemy()) {
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
			return true;
		}

		//ifdef INCLUDE_SERVER_H__
			if( isServerRunning()) {
				if(playerAiReg[playerRegMove]->isNetEnemy()||playerAiReg[playerRegMove]->isUserEnemy()) {
					return true;
				}
			} else {
				if(playerAiReg[playerRegMove]->isUserEnemy()) {
					return true;
				}
			}
		//endif

	}
	return true;
}

inline char getPlayerAfterMe(int playlev) {
	int id = playerRegMove+1+playlev;
	if(id>=playersNum) {
		id%=playersNum;
	}
	return playerAiReg[id]->marker;
}
