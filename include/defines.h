
#define thread(NAME) void __cdecl NAME(void* Args)
#define beginThread(NAME)  ((HANDLE)_beginthread(NAME,0,NULL))
#define endThread() _endthread();
#define threadh HANDLE

#define uint unsigned int
#define symid short
#define locale if(true)

#define check_field_line(x,k1,k2,k3) if(x.fields[k1]!=0 && x.fields[k2]!=0 && x.fields[k3]!=0)
#define check_field_line2(x,k1,k2,k3) if((x.fields[k1]!=0 && x.fields[k2]!=0) || (x.fields[k1]!=0 && x.fields[k3]!=0) || (x.fields[k2]!=0 && x.fields[k3]!=0))

#define check_3dfield_line(sym,n1,n2,n3,k1,k2,k3) if(n1.fields[k1]==sym&&n2.fields[k2]==sym&&n3.fields[k3]==sym) {\
	add_win_marker(make_field_sym(n1, k1, sym));\
	add_win_marker(make_field_sym(n2, k2, sym));\
	add_win_marker(make_field_sym(n3, k3, sym));\
	return true;\
}

#define checkatt_3dfield_line(sym,n1,n2,n3,k1,k2,k3) if(n1.fields[k1]==sym && n2.fields[k2]==sym) {\
	attacker_checks.push_back(make_field_sym(n3, k3, sym));\
}\
if(n1.fields[k1]==sym && n3.fields[k3]==sym) {\
	attacker_checks.push_back(make_field_sym(n2, k2, sym));\
}\
if(n2.fields[k2]==sym && n3.fields[k3]==sym) {\
	attacker_checks.push_back(make_field_sym(n1, k1, sym));\
}

#define check_for_null_field_and_return(val) locale {\
	field temp_ = val;\
	if(temp_.x!=null_field.x&&temp_.y!=null_field.y) {\
		attacker_checks.push_back(temp_);\
	}\
}

#define checkout_field_line2(x,k1,k2,k3) (x.fields[k1]!=0 && x.fields[k2]!=0) ? k3 : ((x.fields[k1]!=0 && x.fields[k3]!=0) ? k2 : ((x.fields[k2]!=0 && x.fields[k3]!=0) ? k1 : -1 )))


#define ascii(code)  ((char)(code))
#define ascii_str(code) (str(ascii(code)))

#define inl inline
#define var auto
#define global static
#define YES true
#define NO false
#define cswitch(val) if(val==YES)
#define decl_thread(NAME) void __cdecl NAME (void*Args)
#define run_thread(NAME) ((HANDLE)_beginthread(NAME,0,NULL))
#define end_thread() _endthread()
#define THREAD HANDLE

#define AVAILABLE_SIGNS {ascii_str(166),ascii_str(127),ascii_str(172),ascii_str(182),ascii_str(189),ascii_str(198),ascii_str(207),ascii_str(210),ascii_str(212),ascii_str(213),ascii_str(214),ascii_str(215),ascii_str(216),ascii_str(221),ascii_str(222),ascii_str(224),ascii_str(225),ascii_str(226),ascii_str(228),ascii_str(229),ascii_str(230),ascii_str(232),ascii_str(234),ascii_str(235),ascii_str(236),ascii_str(238),ascii_str(243),ascii_str(250),ascii_str(151),ascii_str(145),ascii_str(254),ascii_str(241)}
#define AVAILABLE_SIGNS_DEFAULT ascii(166)

#ifdef ALLOW_DEBUG
	#define DEBUG(CODE) if(setting_debug_mode) CODE;
#else
	#define DEBUG(CODE)
#endif
