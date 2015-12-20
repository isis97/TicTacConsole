
class enemy_linear : public aienemy {
	public:
		enemy_linear() {
			initClass("Linear enemy", "[Linear enemy]\nAI enemy that is moving by straight line.", this);
		}

		inline void nextStep() {
			for(int it=0;it<levmap_size;++it) {
				if(levmap[it]==0) {
					levmap[it] = marker;
					lastlevmapasgn = it;
					return;
				}
			}
		}

		vector<enemy_linear*> reg;
		inline aienemy* factory_produceEnemy() {
			enemy_linear* pom;
			reg.push_back(pom);
			reg[reg.size()-1] = new enemy_linear();
			return reg[reg.size()-1];
		}
} enemy_linear_initializer;

class enemy_random : public aienemy {
	public:
		enemy_random() {
			initClass("Random enemy", "[Random enemy]\nAI enemy that is moving randomly.", this);
		}
		inline void nextStep() {
			srand (time(NULL));
			int rnd = 0;
			while(levmap[rnd]!=0) {
				rnd = rand() % (levmap_size);
			}
			levmap[rnd] = marker;
			lastlevmapasgn = rnd;
		}

		vector<enemy_random*> reg;
		inline aienemy* factory_produceEnemy() {
			enemy_random* pom;
			reg.push_back(pom);
			reg[reg.size()-1] = new enemy_random();
			return reg[reg.size()-1];
		}
} enemy_random_initializer;


class enemy_random_defensive : public aienemy {
	public:
		enemy_random_defensive() {
			initClass("Random-defensive enemy", "[Random-defensive enemy]\nAI enemy that is moving randomly, but it\ncan prevent any other player by winning the game.", this);
		}
		inline void nextStep() {
			test.setCurPos(0,0);
			int siz;

			attacker_checks.clear();
			getMeAttacker(marker);
			siz = attacker_checks.size();
			for(int it=0;it<siz;++it) {
				if(attacker_checks[it].x>=0&&attacker_checks[it].y>=0&&attacker_checks[it].x<3*nx&&attacker_checks[it].y<3*ny) {
					if(attacker_checks[it]!=null_field&&attacker_checks[it]!=unknown_field) {
						if((!attacker_checks[it])==0) {
							!attacker_checks[it] = marker;
							test<<"Good pos is "<<attacker_checks[it].x<<" x "<<attacker_checks[it].y;
							test.flush();
							attacker_checks.clear();
							return;
						}
					}
				}
			}

			attacker_checks.clear();
			getAttacker();
			siz = attacker_checks.size();
			for(int it=0;it<siz;++it) {
				if(attacker_checks[it].x>=0&&attacker_checks[it].y>=0&&attacker_checks[it].x<3*nx&&attacker_checks[it].y<3*ny) {
					if(attacker_checks[it]!=null_field&&attacker_checks[it]!=unknown_field) {
						if((!attacker_checks[it])==0) {
							!attacker_checks[it] = marker;
							test<<"Good pos is "<<attacker_checks[it].x<<" x "<<attacker_checks[it].y;
							test.flush();
							attacker_checks.clear();
							return;
						}
					}
				}
			}
			attacker_checks.clear();

			srand (time(NULL));
			int rnd = 0;
			while(levmap[rnd]!=0) {
				rnd = rand() % (levmap_size);
			}
			levmap[rnd] = marker;
			lastlevmapasgn = rnd;
		}


		vector<enemy_random_defensive*> reg;
		inline aienemy* factory_produceEnemy() {
			enemy_random_defensive* pom;
			reg.push_back(pom);
			reg[reg.size()-1] = new enemy_random_defensive();
			return reg[reg.size()-1];
		}
} enemy_random_defensive_initializer;


class enemy_smart : public aienemy {
	public:
		enemy_smart() {
			initClass("Smart enemy", "[Smart enemy]\nAI enemy that is moving using the best\nalgorithm invented by me (can be bugged!).", this);
		}
		inline void nextStep() {


			vector<field> defense_fields;
			vector<field> attack_fields;

			test.setCurPos(0,0);
			attacker_checks.clear();
			getAttacker();
			int siz = attacker_checks.size();
			for(int it=0;it<siz;++it) {
				if(attacker_checks[it].x>=0&&attacker_checks[it].y>=0&&attacker_checks[it].x<3*nx&&attacker_checks[it].y<3*ny) {
					if(attacker_checks[it]!=null_field&&attacker_checks[it]!=unknown_field) {
						if((!attacker_checks[it])==0) {
							defense_fields.push_back(attacker_checks[it]);
						}
					}
				}
			}
			attacker_checks.clear();


			getMeAttacker(marker);
			siz = attacker_checks.size();
			for(int it=0;it<siz;++it) {
				if(attacker_checks[it].x>=0&&attacker_checks[it].y>=0&&attacker_checks[it].x<3*nx&&attacker_checks[it].y<3*ny) {
					if(attacker_checks[it]!=null_field&&attacker_checks[it]!=unknown_field) {
						if((!attacker_checks[it])==0) {
							attack_fields.push_back(attacker_checks[it]);
						}
					}
				}
			}
			attacker_checks.clear();

			defense_fields = remove_same_group(group, defense_fields);
			attack_fields = remove_same_group(group, attack_fields);

			if(attack_fields.size()>0 || defense_fields.size()>0) {
				vector<field> attack_and_defense = multiply_sets(attack_fields, defense_fields);
				attack_and_defense = remove_same_group(group, attack_and_defense);

				siz = attack_and_defense.size();
				if(siz>0) {
					sort_by_time_and_field_priority(attack_and_defense);
					!attack_and_defense[0] = marker;
					return;
				} else {
					if(defense_fields.size()>0) {
						sort_by_time_and_field_priority(defense_fields);
						!defense_fields[0] = marker;
						return;
					} else {
						sort_by_field_priority(attack_fields);
						!attack_fields[0] = marker;
						return;
					}
				}
			}

			//sort_by_field_priority, multiply_sets

			vector<field> potattack_fields;

			field f = {0, 0};
			for(int x=0;x<nx*3;++x) {
				for(int y=0;y<ny*3;++y) {
					f.x = x;
					f.y = y;
					if((!f) == 0) {
						!f = marker;
						attacker_checks.clear();
						if(isMeAttacking(marker)) {
							 potattack_fields.push_back(f);
						}
						!f = 0;
					}
				}
			}

			if(potattack_fields.size()>0) {
				sort_by_field_priority(potattack_fields);
				!potattack_fields[0] = marker;
				return;
			}

			for(int x=0;x<nx*3;++x) {
				for(int y=0;y<ny*3;++y) {
					f.x = x;
					f.y = y;
					if((!f) == 0) {
						potattack_fields.push_back(f);
					}
				}
			}

			sort_by_field_priority(potattack_fields);
			!potattack_fields[0] = marker;
			attacker_checks.clear();
			return;

			attacker_checks.clear();

			srand (time(NULL));
			int rnd = 0;
			while(levmap[rnd]!=0) {
				rnd = rand() % (levmap_size);
			}
			levmap[rnd] = marker;
			lastlevmapasgn = rnd;
			attacker_checks.clear();
			return;
		}


		vector<enemy_smart*> reg;
		inline aienemy* factory_produceEnemy() {
			enemy_smart* pom;
			reg.push_back(pom);
			reg[reg.size()-1] = new enemy_smart();
			return reg[reg.size()-1];
		}
} enemy_smart_initializer, myenemy, myenemy2, myenemy3;


template <typename T>
inline T randomElement( int nozero_chance, vector<T> vect ) {
	int rnd = 0;
	for(int it=0;it<nozero_chance;++it) {
		rnd = rand() % vect.size();
		if(rnd==0) {
			return vect[0];
		}
	}
	rnd = rand()*rand() % vect.size();
	return vect[rnd];
}

class enemy_realistic : public aienemy {
	private:
		int nozero_chance1;
		int nozero_chance2;
		int nozero_chance3;

	public:
		enemy_realistic() {
			/*nozero_chance1 = rand() % 3;
			nozero_chance2 = rand() % 3;
			nozero_chance3 = rand() % 3;
			group = rand() % max(,rand()%10);*/
			nozero_chance1 = min(1, rand() % 4);
			nozero_chance2 = min(1, rand() % 4);
			nozero_chance3 = min(1, rand() % 4);
			group = min(1, rand() % min(1,max(5,rand()%10)));
			initClass("Realistic enemy", "[Realistic enemy]\nAI enemy that is moving using the best\nalgorithm invented by me\nbut uses random probability.", this);
		}
		inline void nextStep() {


			vector<field> defense_fields;
			vector<field> attack_fields;

			test.setCurPos(0,0);
			attacker_checks.clear();
			getAttacker();
			int siz = attacker_checks.size();
			for(int it=0;it<siz;++it) {
				if(attacker_checks[it].x>=0&&attacker_checks[it].y>=0&&attacker_checks[it].x<3*nx&&attacker_checks[it].y<3*ny) {
					if(attacker_checks[it]!=null_field&&attacker_checks[it]!=unknown_field) {
						if((!attacker_checks[it])==0) {
							defense_fields.push_back(attacker_checks[it]);
						}
					}
				}
			}
			attacker_checks.clear();


			getMeAttacker(marker);
			siz = attacker_checks.size();
			for(int it=0;it<siz;++it) {
				if(attacker_checks[it].x>=0&&attacker_checks[it].y>=0&&attacker_checks[it].x<3*nx&&attacker_checks[it].y<3*ny) {
					if(attacker_checks[it]!=null_field&&attacker_checks[it]!=unknown_field) {
						if((!attacker_checks[it])==0) {
							attack_fields.push_back(attacker_checks[it]);
						}
					}
				}
			}
			attacker_checks.clear();

			defense_fields = remove_same_group(group, defense_fields);
			attack_fields = remove_same_group(group, attack_fields);


			if(attack_fields.size()>0 || defense_fields.size()>0) {
				vector<field> attack_and_defense = multiply_sets(attack_fields, defense_fields);
				attack_and_defense = remove_same_group(group, attack_and_defense);

				siz = attack_and_defense.size();
				if(siz>0) {
					sort_by_time_and_field_priority(attack_and_defense);
					!randomElement(nozero_chance1, attack_and_defense) = marker;
					return;
					//if(rand() % (nozero_chance1*3) != 0) return;
				} else {
					if(defense_fields.size()>0) {
						sort_by_time_and_field_priority(defense_fields);
						!randomElement(nozero_chance1, defense_fields) = marker;
						return;
						//if(rand() % (nozero_chance2*4) != 0) return;
					} else {
						sort_by_field_priority(attack_fields);
						!randomElement(nozero_chance1, attack_fields) = marker;
						return;
						//if(rand() % (nozero_chance3*5) != 0) return;
					}
				}
			}


			//sort_by_field_priority, multiply_sets

			vector<field> potattack_fields;

			field f = {0, 0};
			for(int x=0;x<nx*3;++x) {
				for(int y=0;y<ny*3;++y) {
					f.x = x;
					f.y = y;
					if((!f) == 0) {
						!f = marker;
						attacker_checks.clear();
						if(isMeAttacking(marker)) {
							 potattack_fields.push_back(f);
						}
						!f = 0;
					}
				}
			}

			if(potattack_fields.size()>0) {
				sort_by_field_priority(potattack_fields);
				!randomElement(nozero_chance2, potattack_fields) = marker;
				if((rand() % (min(1,nozero_chance1+nozero_chance2*5+nozero_chance3*3))) > nozero_chance1) return;
				//return;
			}

			for(int x=0;x<nx*3;++x) {
				for(int y=0;y<ny*3;++y) {
					f.x = x;
					f.y = y;
					if((!f) == 0) {
						potattack_fields.push_back(f);
					}
				}
			}

			sort_by_field_priority(potattack_fields);
			!randomElement(nozero_chance3, potattack_fields) = marker;
			attacker_checks.clear();
			return;

			attacker_checks.clear();

			srand (time(NULL));
			int rnd = 0;
			while(levmap[rnd]!=0) {
				rnd = rand() % (levmap_size);
			}
			levmap[rnd] = marker;
			lastlevmapasgn = rnd;
			attacker_checks.clear();
			return;
		}


		vector<enemy_realistic*> reg;
		inline aienemy* factory_produceEnemy() {
			enemy_realistic* pom;
			reg.push_back(pom);
			reg[reg.size()-1] = new enemy_realistic();
			return reg[reg.size()-1];
		}
} enemy_realistic_initializer;
