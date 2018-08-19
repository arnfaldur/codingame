#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

const float weights[] = {0.4584541479495468,
						 0.49527483386905563,
						 0.37846622132137486,
						 0.6152927267893537,
						 0.6658405997411934,
						 0.0028051665777753705,
						 0.48701160912925323,
						 0.4301448215532352,
						 0.17407016799401315,
						 0.39195517614226116,
						 0.40574233582520924,
						 0.3801825566690368,
						 0.24578153054337448,
						 -0.3091025411766633,
						 0.539238762361815};
const float bias = 0.8940729693697819;

struct Player {
	int health;
	int mana;
	int deck;
	int rune;
	int draw = 1;
	bool guarded;
};
struct Card;

struct Cards : vector<Card> {
	bool exists(const int) const;
	Card &getById(int);
	const Card &getById(const int) const;
};

struct Card {

	int number;
	int id;
	enum Type {
		creature, greenItem, redItem, blueItem
	} type;
	int cost;
	int atk;
	int def;
	int myhc;

	int ophc;
	int draw;
	bool trample;
	bool charge;
	bool drain;
	bool guard;
	bool lethal;
	bool ward;

	void attackSomething(Cards &bad);

	bool operator==(const Card &rhs) const;

	bool operator!=(const Card &rhs) const;

	const float value() const {
		float result = weights[type];
		result += weights[4] * atk;
		result += weights[5] * def;
		result += weights[6] * trample;
		result += weights[7] * charge;
		result += weights[8] * drain;
		result += weights[9] * guard;
		result += weights[10] * lethal;
		result += weights[11] * ward;
		result += weights[12] * myhc;
		result += weights[13] * ophc;
		result += weights[14] * draw;
		return result + bias;
	}
};

Card notFound{0, 0}; // a dummy card that represents a a card that isn't found

void Card::attackSomething(Cards &bad) {
	bool attacked = false;
	bool guard = false;
	for (Card b: bad) {
		guard |= b.guard;
	}
	for (Card &b: bad) {
		if ((guard && b.guard) ||
			(!guard && (atk >= b.def && b.atk < def || atk >= b.def && cost < b.cost))) {
			b.def -= atk;
			attacked = true;
			cout << "ATTACK " << id << " " << b.id << ';';
			break;
		}
	}
	if (!attacked && !guard) {
		cout << "ATTACK " << id << " " << -1 << ';';
	}
}

bool Card::operator==(const Card &rhs) const {
	return number == rhs.number &&
		   id == rhs.id;
}

bool Card::operator!=(const Card &rhs) const {
	return !(rhs == *this);
}

bool Cards::exists(const int id) const {
	return any_of(begin(), end(), [id](Card c){ return c.id == id; });
}

Card &Cards::getById(int id) {
	for (Card &card: *this) {
		if (card.id == id) {
			return card;
		}
	}
	return notFound;
}

const Card &Cards::getById(const int id) const {
	for (const Card &card: *this) {
		if (card.id == id) {
			return card;
		}
	}
	return notFound;
}

struct State;

struct Action {
	enum Type {
		pick, summon, attack, use, pass
	} type;
	int doer;
	int target = -1;

	friend ostream &operator<<(ostream &os, const Action &action) {
		if (action.type == pick) {
			os << "PICK " << action.doer << ';';
		} else if (action.type == summon) {
			os << "SUMMON " << action.doer << ';';
		} else if (action.type == attack) {
			os << "ATTACK " << action.doer << ' ' << action.target << ';';
		} else if (action.type == use) {
			os << "USE " << action.doer << ' ' << action.target << ';';
		} else if (action.type == pass) {
			os << "PASS;";
		}
		return os;
	}
};

struct State {
	Cards hand;
	Cards myField;
	Cards opField;
	Player me;
	Player op;

	const bool isValid(const Action &) const;
	bool act(const Action &); // returns a success boolean
};

const bool State::isValid(const Action& action) const {
	if (action.type == Action::pick) {
	    return action.doer >= 0 && action.doer <= 2;
	} else if (action.type == action.summon) {
		if (myField.size() >= 6) { return false; }
		return hand.exists(action.doer);
	} else if (action.type == Action::attack) {
		if (myField.exists(action.doer)) {
			if (action.target == -1 && !op.guarded) {
			} else if (action.target != -1 && opField.exists(action.target)) {
				const Card &enemy = opField.getById(action.target);
				if (enemy.def > 0 && ((op.guarded && enemy.guard) || !op.guarded)) {
				} else { return false; }
			} else { return false; }
		}
	} else if (action.type == Action::use) {
		const Card &item = hand.getById(action.doer);
		if (item == notFound) { return false; }
		me.draw += item.draw;
		if (action.target == -1 && item.type == Card::blueItem) {
		} else if (action.target != -1) {
			Card &target = notFound;
			if (item.type == Card::greenItem) {
				target = myField.getById(action.target);
				if (target == notFound) { return false; }
			} else if (item.type == Card::redItem || item.type == Card::blueItem) {
				target = opField.getById(action.target);
				if (target == notFound) { return false; }
			}
		} else { return false; }
	}
	return true;
}

bool State::act(const Action &action) {
	if (action.type == Action::summon) {
		if (myField.size() == 6) { return false; }
		for (int i = 0; i < hand.size(); ++i) {
			if (hand[i].id == action.doer && hand[i].cost <= me.mana) {
				me.health += hand[i].myhc;
				op.health += hand[i].ophc;
				myField.push_back(hand[i]);
				hand.erase(hand.begin() + i);
				return true;
			}
		}
		return false;
	} else if (action.type == Action::attack) {
		Card &attacker = myField.getById(action.doer);
		if (attacker == notFound) { return false; }
		if (action.target == -1 && !op.guarded) {
			op.health -= attacker.atk;
		} else if (action.target != -1) {
			Card &enemy = opField.getById(action.target);
			if (enemy == notFound) { return false; }
			if (enemy.def > 0 && ((op.guarded && enemy.guard) || !op.guarded)) {
				if (!enemy.ward) {
					enemy.def -= attacker.lethal && attacker.atk > 0 ? enemy.def : attacker.atk;
				} else {
					enemy.ward = false;
				}
				if (!attacker.ward) {
					attacker.def -= enemy.lethal && enemy.atk > 0 ? attacker.def : enemy.atk;
				} else {
					attacker.ward = false;
				}
				if (enemy.def < 0 && attacker.trample) {
					op.health += enemy.def;
				}
			} else { return false; }
		} else { return false; }
	} else if (action.type == Action::use) {
		Card &item = hand.getById(action.doer);
		if (item == notFound) { return false; }
		me.draw += item.draw;
		if (action.target == -1 && item.type == Card::blueItem) {
			op.health += item.def;
		} else if (action.target != -1) {
			Card &target = notFound;
			if (item.type == Card::greenItem) {
				target = myField.getById(action.target);
				if (target == notFound) { return false; }
				target.trample |= item.trample;
				target.charge |= item.charge;
				target.drain |= item.drain;
				target.guard |= item.guard;
				target.lethal |= item.lethal;
				target.ward |= item.ward;
			} else if (item.type == Card::redItem || item.type == Card::blueItem) {
				target = opField.getById(action.target);
				if (target == notFound) { return false; }
				target.trample &= !item.trample;
				target.charge &= !item.charge;
				target.drain &= !item.drain;
				target.guard &= !item.guard;
				target.lethal &= !item.lethal;
				target.ward &= !item.ward;
			}
			target.def += item.def;
			target.atk += item.atk;
			me.health += item.myhc;
			op.health += item.ophc;
		} else { return false; }
	}
	return true;
}

float fieldValue(const Cards &good, const Cards &bad) {
	float result = 0;
	for (const Card &c: good) {
		if (c.def > 0) {
			result += c.value();
		}
	}
	for (const Card &c: bad) {
		if (c.def > 0) {
			result -= c.value();
		}
	}
	return result;
}

void removeDead(Cards &cards) {
	for (int i = 0; i < cards.size(); ++i) {
		if (cards[i].def <= 0) {
			if (i != cards.size() - 1) {
				Card tmp = cards[i];
				cards[i] = cards.back();
				cards.back() = tmp;
			}
			cards.pop_back();
		}
	}
}

int main() {

	Player player;
	Player opponent;
	vector<int> curve{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

	// game loop
	for (int iterations = 0; iterations < 100000; iterations++) {
		cin >> player.health >> player.mana >> player.deck >> player.rune;
		cin.ignore();
		cin >> opponent.health >> opponent.mana >> opponent.deck >> opponent.rune;
		cin.ignore();
		int opponentHand;
		cin >> opponentHand;
		cin.ignore();
		int cardCount;
		cin >> cardCount;
		cin.ignore();
		auto hand = Cards();
		auto good = Cards();
		auto bad = Cards();
		for (int i = 0; i < cardCount; i++) {
			int location;
			string abilities;
			Card card;
			cin >> card.number >> card.id >> location >> card.type >> card.cost >> card.atk >> card.def
				>> abilities >> card.myhc >> card.ophc >> card.draw;
			cin.ignore();
			for (char &a: abilities) {
				if (a == 'B') card.trample = true;
				if (a == 'C') card.charge = true;
				if (a == 'D') card.drain = true;
				if (a == 'G') card.guard = true;
				if (a == 'L') card.lethal = true;
				if (a == 'W') card.ward = true;
			}
			if (location == 0) {
				hand.push_back(card);
			} else if (location == 1) {
				good.push_back(card);
			} else {
				bad.push_back(card);
			}
		}
		if (player.mana != 0) { // Battle phase
			for (Card c: hand) {
				if (player.mana >= c.cost) {
					if (c.type == Creature) {
						if (good.size() < 6) {
							player.mana -= c.cost;
							cout << "SUMMON " << c.id << ';';
							if (c.charge) {
								c.attackSomething(bad);
							}
						}
					} else if (good.size() > 0 && c.type == GreenItem) {
						cout << "USE " << c.id << ' ' << good[rand() % good.size()].id << ';';
					} else if (bad.size() > 0 && c.type == RedItem) {
						cout << "USE " << c.id << ' ' << bad[rand() % bad.size()].id << ';';
					} else if (c.type == BlueItem) {
						if (c.def < 0) {
							if (bad.size() > 0) {
								cout << "USE " << c.id << ' ' << bad[rand() % bad.size()].id << ';';
							}
						} else {
							cout << "USE " << c.id << " -1" << ';';
						}
					}
					removeDead(bad);
				}
			}
			for (Card g: good) {
				for (auto c: bad) {
					cerr << c.id << ", ";
				}
				g.attackSomething(bad);
				removeDead(bad);
			}
			cout << "PASS";
		} else { // Draft Phase
			int bestPick = 0;
			int minCards = 100;
			for (int i = 0; i < 3; ++i) {
				int cost = min(hand[i].cost, 7);
				if (curve[cost] < minCards) {
					minCards = curve[hand[i].cost];
					bestPick = i;
				}
			}
			int cost = min(hand[bestPick].cost, 7);
			curve[cost]++;
			cout << "PICK " << bestPick;
		}
		cout << endl;
	}
}