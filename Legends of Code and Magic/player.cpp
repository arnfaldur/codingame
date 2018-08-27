#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <bitset>
#include <cmath>
#include <array>
#include <random>
#include <future>

using namespace std;

//const double weights[] = {
//        0.4584541479495468f,
//        0.49527483386905563f,
//        0.37846622132137486f,
//        0.6152927267893537f,
//        0.6658405997411934f,
//        0.0028051665777753705f,
//        0.48701160912925323f,
//        0.4301448215532352f,
//        0.17407016799401315f,
//        0.39195517614226116f,
//        0.40574233582520924f,
//        0.3801825566690368f,
//        0.24578153054337448f,
//        -0.3091025411766633f,
//        0.539238762361815f
//};
//const double bias     = 0.6748886773194405f;
const double weights[] = {
		0.103741125135865,
		0.5815862231594002,
		1.0011422083122006,
		1.0463757424876121,
		0.7582732533917496,
		0.013207002615374563,
		0.27299195602540044,
		0.7063593608101006,
		0.3212432505078901,
		0.5108082490777912,
		0.5102398366213459,
		0.32949509489751694,
		0.2776166056652229,
		-0.3501591411620892,
		0.8128242632427777
};

const double weigt[] = {
		-0.8286463871234114,
		0.11437568086308485,
		0.005416222839380229,
		-0.11699038765316574,
		0.8657133566706613,
		0.6758092940940308,
		-0.48777960618680166,
		-0.6289715551054854,
		-0.026245728467361167,
		-0.0323342072493089,
		1.2390637084085403,
		0.3960403685520665,
		0.7771226288539821,
		-1.2254518622613986,
		1.9870044157192972,
		-0.44636860216618507,
		0.036000617474930156,
		-0.12097392051163303,
		1.019000619050217,
		0.718946064683587,
		0.9421096420305118,
		2.731806469877956,
		0.9476540494392619,
		0.3773282282332373,
		1.9115736196734632,
		1.931766780880596,
		-0.3732141886308028,
		0.05013236813968261,
		2.28224294870229,
		0.3931942752543838,
		0.09120935911662732,
		-0.8618722696403827,
		-0.6995199163084821,
		-0.06919365272045586,
		0.2479646349411113,
		0.22184060920221407,
		0.2319285257627829,
		0.32076248256685075,
		-0.06761059057373123,
		-0.16600636646583997,
		0.48489340279852783,
		0.9666986428634307,
		-4.010691080047097,
		-0.0770140003545358,
		-1.9608571988022563,
		0.08942668136781434,
		0.12590060053187818,
		0.20334559738084657,
		-0.07894887347689855,
		-0.025633215603277373,
		-0.2165378441394038,
		3.8140779368049893,
		-1.1786640759642573,
		1.2583026130992903,
		-0.03198735534162023,
		-0.008663590335200543,
		0.14018931181555142,
		0.40338783929292804,
		0.4660361305596869,
		-0.021177788549802117,
		-0.3592847563036973,
		0.2189444903003017,
		-0.16107841250354546,
		0.12846077126173008,
		-0.14652275228093145,
		-0.009451009276857379,
		0.005230193833568855,
		0.09771033681432252,
		-0.1372734047629854,
		0.07107932767146494,
		-0.0014499700870322135,
		-0.3209804624780682,
		0.4420567918245979,
		0.29163520437916535,
		-0.2577699622045818,
		-0.1668554777799797,
		0.5267092928316072,
		-0.4219162824110354,
		0.15079161674070707,
		-4.648946339215971,
		-1.2773128681239123,
		0.1440680242677218,
		-0.4803346196841491,
		1.19814668015662,
		0.5598930541721847,
		0.2767307252992091,
		-0.7221971058899181,
		2.100120154117222,
		-1.4649599766637222,
		1.1242056225821417,
		0.1103218419793947,
		0.29896939682632034,
		-0.053003956815517506,
		0.3707587193527466,
		-0.002187563460456215,
		1.7443685053217628,
		-3.2693592498732365,
		0.005093674869705073,
		0.1724104084308126,
		0.2207234110717828,
		-0.4144401814293555,
		1.3796295996661807,
		0.5460206726896603,
		-0.7242946802220243,
		-0.5123744764815111,
		0.7621966153795461,
		0.03174809369964273,
		-0.02015243933940482,
		-0.12677488972023274,
		0.24327236265410673,
		0.6446480191115467,
		-0.047324232795244586,
		-0.8092631245413439,
		0.21030791594818035,
		-0.48742400078086134,
		0.5123407124928375,
		1.1115772228693934,
		0.20474703354312562,
		-1.0145430420647301,
		0.392292567727351
};
const double bias = 0.8940729693697819f;

struct Player {
	int health = 30;
	int mana;
	int deck = 30;
	int rune = 5;
	int draw = 1;
	bool guarded = false;

	friend ostream &operator<<(ostream &os, const Player &player) {
		os << "health: " << player.health << " mana: " << player.mana << " deck: " << player.deck << " rune: "
		   << player.rune << " draw: " << player.draw << " guarded: " << player.guarded;
		return os;
	}
};

struct Card;

struct Cards : vector<Card> {
	bool exists(int) const;

	Card &getById(int);

	const Card &getById(int) const;

	void removeDead();
};

struct Card { ;

	enum Location {
		OpField = -1, Hand, MyField, Nowhere
	};
	enum Type {
		Creature, GreenItem, RedItem, BlueItem
	};
//	enum Abilities {
//		Ward, Lethal, Guard, Drain, Charge, Trample
//	};
	enum Abilities {
		Trample, Charge, Drain, Guard, Lethal, Ward
	};

	int number;
	int id;
	Location location;
	Type type;

	int cost;
	int atk;
	int def;

	bitset<6> abilities;
	int myhc;
	int ophc;
	int draw;
	bool sickness;

	bool operator<(const Card &rhs) const;

	bool operator>(const Card &rhs) const;

	bool operator<=(const Card &rhs) const;

	bool operator>=(const Card &rhs) const;

	friend ostream &operator<<(ostream &os, const Card &card);

	const static Card notFound;

	bool operator==(const Card &rhs) const;

	bool operator!=(const Card &rhs) const;

	const double value() const {
//        double result = weights[type];
////        result += cost / 1.9f;
//        result += weights[4] * atk;
//        result += weights[5] * def;
//        result += weights[6] * abilities[trample];
//        result += weights[7] * abilities[charge];
//        result += weights[8] * abilities[drain];
//        result += weights[9] * abilities[guard];
//        result += weights[10] * abilities[lethal];
//        result += weights[11] * abilities[ward];
//        result += weights[12] * myhc;
//        result += weights[13] * ophc;
//        result += weights[14] * draw;
//        result += bias;

		double result = -0.8452627390053186;
//        result += cost / 1.9f;
		const int vals[] = {
				type == Creature,
				type == GreenItem,
				type == RedItem,
				type == BlueItem,
				atk, def,
				abilities[Trample],
				abilities[Charge],
				abilities[Drain],
				abilities[Guard],
				abilities[Lethal],
				abilities[Ward],
				myhc, ophc, draw
		};
		int k = 0;
		for (int i = 0; i < 15; ++i) {
			for (int j = i; j < 15; ++j) {
				result += vals[i] * vals[j] * weigt[k];
				k++;
			}
		}
		return result;
	}
};

//const Card pool[] = {
const static array<Card, 160> pool{
		//Card{0, 0, Card::Hand, Card::Creature, 0, 0, 0, bitset<6>("000000"), 0, 0, 0, false},
		Card{1, -1, Card::Hand, Card::Creature, 1, 2, 1, bitset<6>("000000"), 1, 0, 0, false},
		Card{2, -1, Card::Hand, Card::Creature, 1, 1, 2, bitset<6>("000000"), 0, -1, 0, false},
		Card{3, -1, Card::Hand, Card::Creature, 1, 2, 2, bitset<6>("000000"), 0, 0, 0, false},
		Card{4, -1, Card::Hand, Card::Creature, 2, 1, 5, bitset<6>("000000"), 0, 0, 0, false},
		Card{5, -1, Card::Hand, Card::Creature, 2, 4, 1, bitset<6>("000000"), 0, 0, 0, false},
		Card{6, -1, Card::Hand, Card::Creature, 2, 3, 2, bitset<6>("000000"), 0, 0, 0, false},
		Card{7, -1, Card::Hand, Card::Creature, 2, 2, 2, bitset<6>("000001"), 0, 0, 0, false},
		Card{8, -1, Card::Hand, Card::Creature, 2, 2, 3, bitset<6>("000000"), 0, 0, 0, false},
		Card{9, -1, Card::Hand, Card::Creature, 3, 3, 4, bitset<6>("000000"), 0, 0, 0, false},
		Card{10, -1, Card::Hand, Card::Creature, 3, 3, 1, bitset<6>("001000"), 0, 0, 0, false},
		Card{11, -1, Card::Hand, Card::Creature, 3, 5, 2, bitset<6>("000000"), 0, 0, 0, false},
		Card{12, -1, Card::Hand, Card::Creature, 3, 2, 5, bitset<6>("000000"), 0, 0, 0, false},
		Card{13, -1, Card::Hand, Card::Creature, 4, 5, 3, bitset<6>("000000"), 1, -1, 0, false},
		Card{14, -1, Card::Hand, Card::Creature, 4, 9, 1, bitset<6>("000000"), 0, 0, 0, false},
		Card{15, -1, Card::Hand, Card::Creature, 4, 4, 5, bitset<6>("000000"), 0, 0, 0, false},
		Card{16, -1, Card::Hand, Card::Creature, 4, 6, 2, bitset<6>("000000"), 0, 0, 0, false},
		Card{17, -1, Card::Hand, Card::Creature, 4, 4, 5, bitset<6>("000000"), 0, 0, 0, false},
		Card{18, -1, Card::Hand, Card::Creature, 4, 7, 4, bitset<6>("000000"), 0, 0, 0, false},
		Card{19, -1, Card::Hand, Card::Creature, 5, 5, 6, bitset<6>("000000"), 0, 0, 0, false},
		Card{20, -1, Card::Hand, Card::Creature, 5, 8, 2, bitset<6>("000000"), 0, 0, 0, false},
		Card{21, -1, Card::Hand, Card::Creature, 5, 6, 5, bitset<6>("000000"), 0, 0, 0, false},
		Card{22, -1, Card::Hand, Card::Creature, 6, 7, 5, bitset<6>("000000"), 0, 0, 0, false},
		Card{23, -1, Card::Hand, Card::Creature, 7, 8, 8, bitset<6>("000000"), 0, 0, 0, false},
		Card{24, -1, Card::Hand, Card::Creature, 1, 1, 1, bitset<6>("000000"), 0, -1, 0, false},
		Card{25, -1, Card::Hand, Card::Creature, 2, 3, 1, bitset<6>("000000"), -2, -2, 0, false},
		Card{26, -1, Card::Hand, Card::Creature, 2, 3, 2, bitset<6>("000000"), 0, -1, 0, false},
		Card{27, -1, Card::Hand, Card::Creature, 2, 2, 2, bitset<6>("000000"), 2, 0, 0, false},
		Card{28, -1, Card::Hand, Card::Creature, 2, 1, 2, bitset<6>("000000"), 0, 0, 1, false},
		Card{29, -1, Card::Hand, Card::Creature, 2, 2, 1, bitset<6>("000000"), 0, 0, 1, false},
		Card{30, -1, Card::Hand, Card::Creature, 3, 4, 2, bitset<6>("000000"), 0, -2, 0, false},
		Card{31, -1, Card::Hand, Card::Creature, 3, 3, 1, bitset<6>("000000"), 0, -1, 0, false},
		Card{32, -1, Card::Hand, Card::Creature, 3, 3, 2, bitset<6>("000000"), 0, 0, 1, false},
		Card{33, -1, Card::Hand, Card::Creature, 4, 4, 3, bitset<6>("000000"), 0, 0, 1, false},
		Card{34, -1, Card::Hand, Card::Creature, 5, 3, 5, bitset<6>("000000"), 0, 0, 1, false},
		Card{35, -1, Card::Hand, Card::Creature, 6, 5, 2, bitset<6>("100000"), 0, 0, 1, false},
		Card{36, -1, Card::Hand, Card::Creature, 6, 4, 4, bitset<6>("000000"), 0, 0, 2, false},
		Card{37, -1, Card::Hand, Card::Creature, 6, 5, 7, bitset<6>("000000"), 0, 0, 1, false},
		Card{38, -1, Card::Hand, Card::Creature, 1, 1, 3, bitset<6>("001000"), 0, 0, 0, false},
		Card{39, -1, Card::Hand, Card::Creature, 1, 2, 1, bitset<6>("001000"), 0, 0, 0, false},
		Card{40, -1, Card::Hand, Card::Creature, 3, 2, 3, bitset<6>("001100"), 0, 0, 0, false},
		Card{41, -1, Card::Hand, Card::Creature, 3, 2, 2, bitset<6>("011000"), 0, 0, 0, false},
		Card{42, -1, Card::Hand, Card::Creature, 4, 4, 2, bitset<6>("001000"), 0, 0, 0, false},
		Card{43, -1, Card::Hand, Card::Creature, 6, 5, 5, bitset<6>("001000"), 0, 0, 0, false},
		Card{44, -1, Card::Hand, Card::Creature, 6, 3, 7, bitset<6>("001010"), 0, 0, 0, false},
		Card{45, -1, Card::Hand, Card::Creature, 6, 6, 5, bitset<6>("101000"), -3, 0, 0, false},
		Card{46, -1, Card::Hand, Card::Creature, 9, 7, 7, bitset<6>("001000"), 0, 0, 0, false},
		Card{47, -1, Card::Hand, Card::Creature, 2, 1, 5, bitset<6>("001000"), 0, 0, 0, false},
		Card{48, -1, Card::Hand, Card::Creature, 1, 1, 1, bitset<6>("000010"), 0, 0, 0, false},
		Card{49, -1, Card::Hand, Card::Creature, 2, 1, 2, bitset<6>("000110"), 0, 0, 0, false},
		Card{50, -1, Card::Hand, Card::Creature, 3, 3, 2, bitset<6>("000010"), 0, 0, 0, false},
		Card{51, -1, Card::Hand, Card::Creature, 4, 3, 5, bitset<6>("000010"), 0, 0, 0, false},
		Card{52, -1, Card::Hand, Card::Creature, 4, 2, 4, bitset<6>("000010"), 0, 0, 0, false},
		Card{53, -1, Card::Hand, Card::Creature, 4, 1, 1, bitset<6>("010010"), 0, 0, 0, false},
		Card{54, -1, Card::Hand, Card::Creature, 3, 2, 2, bitset<6>("000010"), 0, 0, 0, false},
		Card{55, -1, Card::Hand, Card::Creature, 2, 0, 5, bitset<6>("000100"), 0, 0, 0, false},
		Card{56, -1, Card::Hand, Card::Creature, 4, 2, 7, bitset<6>("000000"), 0, 0, 0, false},
		Card{57, -1, Card::Hand, Card::Creature, 4, 1, 8, bitset<6>("000000"), 0, 0, 0, false},
		Card{58, -1, Card::Hand, Card::Creature, 6, 5, 6, bitset<6>("100000"), 0, 0, 0, false},
		Card{59, -1, Card::Hand, Card::Creature, 7, 7, 7, bitset<6>("000000"), 1, -1, 0, false},
		Card{60, -1, Card::Hand, Card::Creature, 7, 4, 8, bitset<6>("000000"), 0, 0, 0, false},
		Card{61, -1, Card::Hand, Card::Creature, 9, 10, 10, bitset<6>("000000"), 0, 0, 0, false},
		Card{62, -1, Card::Hand, Card::Creature, 12, 12, 12, bitset<6>("100100"), 0, 0, 0, false},
		Card{63, -1, Card::Hand, Card::Creature, 2, 0, 4, bitset<6>("000101"), 0, 0, 0, false},
		Card{64, -1, Card::Hand, Card::Creature, 2, 1, 1, bitset<6>("000101"), 0, 0, 0, false},
		Card{65, -1, Card::Hand, Card::Creature, 2, 2, 2, bitset<6>("000001"), 0, 0, 0, false},
		Card{66, -1, Card::Hand, Card::Creature, 5, 5, 1, bitset<6>("000001"), 0, 0, 0, false},
		Card{67, -1, Card::Hand, Card::Creature, 6, 5, 5, bitset<6>("000001"), 0, -2, 0, false},
		Card{68, -1, Card::Hand, Card::Creature, 6, 7, 5, bitset<6>("000001"), 0, 0, 0, false},
		Card{69, -1, Card::Hand, Card::Creature, 3, 4, 4, bitset<6>("100000"), 0, 0, 0, false},
		Card{70, -1, Card::Hand, Card::Creature, 4, 6, 3, bitset<6>("100000"), 0, 0, 0, false},
		Card{71, -1, Card::Hand, Card::Creature, 4, 3, 2, bitset<6>("110000"), 0, 0, 0, false},
		Card{72, -1, Card::Hand, Card::Creature, 4, 5, 3, bitset<6>("100000"), 0, 0, 0, false},
		Card{73, -1, Card::Hand, Card::Creature, 4, 4, 4, bitset<6>("100000"), 4, 0, 0, false},
		Card{74, -1, Card::Hand, Card::Creature, 5, 5, 4, bitset<6>("100100"), 0, 0, 0, false},
		Card{75, -1, Card::Hand, Card::Creature, 5, 6, 5, bitset<6>("100000"), 0, 0, 0, false},
		Card{76, -1, Card::Hand, Card::Creature, 6, 5, 5, bitset<6>("101000"), 0, 0, 0, false},
		Card{77, -1, Card::Hand, Card::Creature, 7, 7, 7, bitset<6>("100000"), 0, 0, 0, false},
		Card{78, -1, Card::Hand, Card::Creature, 8, 5, 5, bitset<6>("100000"), 0, -5, 0, false},
		Card{79, -1, Card::Hand, Card::Creature, 8, 8, 8, bitset<6>("100000"), 0, 0, 0, false},
		Card{80, -1, Card::Hand, Card::Creature, 8, 8, 8, bitset<6>("100100"), 0, 0, 1, false},
		Card{81, -1, Card::Hand, Card::Creature, 9, 6, 6, bitset<6>("110000"), 0, 0, 0, false},
		Card{82, -1, Card::Hand, Card::Creature, 7, 5, 5, bitset<6>("101001"), 0, 0, 0, false},
		Card{83, -1, Card::Hand, Card::Creature, 0, 1, 1, bitset<6>("010000"), 0, 0, 0, false},
		Card{84, -1, Card::Hand, Card::Creature, 2, 1, 1, bitset<6>("011001"), 0, 0, 0, false},
		Card{85, -1, Card::Hand, Card::Creature, 3, 2, 3, bitset<6>("010000"), 0, 0, 0, false},
		Card{86, -1, Card::Hand, Card::Creature, 3, 1, 5, bitset<6>("010000"), 0, 0, 0, false},
		Card{87, -1, Card::Hand, Card::Creature, 4, 2, 5, bitset<6>("010100"), 0, 0, 0, false},
		Card{88, -1, Card::Hand, Card::Creature, 5, 4, 4, bitset<6>("010000"), 0, 0, 0, false},
		Card{89, -1, Card::Hand, Card::Creature, 5, 4, 1, bitset<6>("010000"), 2, 0, 0, false},
		Card{90, -1, Card::Hand, Card::Creature, 8, 5, 5, bitset<6>("010000"), 0, 0, 0, false},
		Card{91, -1, Card::Hand, Card::Creature, 0, 1, 2, bitset<6>("000100"), 0, 1, 0, false},
		Card{92, -1, Card::Hand, Card::Creature, 1, 0, 1, bitset<6>("000100"), 2, 0, 0, false},
		Card{93, -1, Card::Hand, Card::Creature, 1, 2, 1, bitset<6>("000100"), 0, 0, 0, false},
		Card{94, -1, Card::Hand, Card::Creature, 2, 1, 4, bitset<6>("000100"), 0, 0, 0, false},
		Card{95, -1, Card::Hand, Card::Creature, 2, 2, 3, bitset<6>("000100"), 0, 0, 0, false},
		Card{96, -1, Card::Hand, Card::Creature, 2, 3, 2, bitset<6>("000100"), 0, 0, 0, false},
		Card{97, -1, Card::Hand, Card::Creature, 3, 3, 3, bitset<6>("000100"), 0, 0, 0, false},
		Card{98, -1, Card::Hand, Card::Creature, 3, 2, 4, bitset<6>("000100"), 0, 0, 0, false},
		Card{99, -1, Card::Hand, Card::Creature, 3, 2, 5, bitset<6>("000100"), 0, 0, 0, false},
		Card{100, -1, Card::Hand, Card::Creature, 3, 1, 6, bitset<6>("000100"), 0, 0, 0, false},
		Card{101, -1, Card::Hand, Card::Creature, 4, 3, 4, bitset<6>("000100"), 0, 0, 0, false},
		Card{102, -1, Card::Hand, Card::Creature, 4, 3, 3, bitset<6>("000100"), 0, -1, 0, false},
		Card{103, -1, Card::Hand, Card::Creature, 4, 3, 6, bitset<6>("000100"), 0, 0, 0, false},
		Card{104, -1, Card::Hand, Card::Creature, 4, 4, 4, bitset<6>("000100"), 0, 0, 0, false},
		Card{105, -1, Card::Hand, Card::Creature, 5, 4, 6, bitset<6>("000100"), 0, 0, 0, false},
		Card{106, -1, Card::Hand, Card::Creature, 5, 5, 5, bitset<6>("000100"), 0, 0, 0, false},
		Card{107, -1, Card::Hand, Card::Creature, 5, 3, 3, bitset<6>("000100"), 3, 0, 0, false},
		Card{108, -1, Card::Hand, Card::Creature, 5, 2, 6, bitset<6>("000100"), 0, 0, 0, false},
		Card{109, -1, Card::Hand, Card::Creature, 5, 5, 6, bitset<6>("000000"), 0, 0, 0, false},
		Card{110, -1, Card::Hand, Card::Creature, 5, 0, 9, bitset<6>("000100"), 0, 0, 0, false},
		Card{111, -1, Card::Hand, Card::Creature, 6, 6, 6, bitset<6>("000100"), 0, 0, 0, false},
		Card{112, -1, Card::Hand, Card::Creature, 6, 4, 7, bitset<6>("000100"), 0, 0, 0, false},
		Card{113, -1, Card::Hand, Card::Creature, 6, 2, 4, bitset<6>("000100"), 4, 0, 0, false},
		Card{114, -1, Card::Hand, Card::Creature, 7, 7, 7, bitset<6>("000100"), 0, 0, 0, false},
		Card{115, -1, Card::Hand, Card::Creature, 8, 5, 5, bitset<6>("000101"), 0, 0, 0, false},
		Card{116, -1, Card::Hand, Card::Creature, 12, 8, 8, bitset<6>("111111"), 0, 0, 0, false},
		Card{117, -1, Card::Hand, Card::GreenItem, 1, 1, 1, bitset<6>("100000"), 0, 0, 0, false},
		Card{118, -1, Card::Hand, Card::GreenItem, 0, 0, 3, bitset<6>("000000"), 0, 0, 0, false},
		Card{119, -1, Card::Hand, Card::GreenItem, 1, 1, 2, bitset<6>("000000"), 0, 0, 0, false},
		Card{120, -1, Card::Hand, Card::GreenItem, 2, 1, 0, bitset<6>("000010"), 0, 0, 0, false},
		Card{121, -1, Card::Hand, Card::GreenItem, 2, 0, 3, bitset<6>("000000"), 0, 0, 1, false},
		Card{122, -1, Card::Hand, Card::GreenItem, 2, 1, 3, bitset<6>("000100"), 0, 0, 0, false},
		Card{123, -1, Card::Hand, Card::GreenItem, 2, 4, 0, bitset<6>("000000"), 0, 0, 0, false},
		Card{124, -1, Card::Hand, Card::GreenItem, 3, 2, 1, bitset<6>("001000"), 0, 0, 0, false},
		Card{125, -1, Card::Hand, Card::GreenItem, 3, 1, 4, bitset<6>("000000"), 0, 0, 0, false},
		Card{126, -1, Card::Hand, Card::GreenItem, 3, 2, 3, bitset<6>("000000"), 0, 0, 0, false},
		Card{127, -1, Card::Hand, Card::GreenItem, 3, 0, 6, bitset<6>("000000"), 0, 0, 0, false},
		Card{128, -1, Card::Hand, Card::GreenItem, 4, 4, 3, bitset<6>("000000"), 0, 0, 0, false},
		Card{129, -1, Card::Hand, Card::GreenItem, 4, 2, 5, bitset<6>("000000"), 0, 0, 0, false},
		Card{130, -1, Card::Hand, Card::GreenItem, 4, 0, 6, bitset<6>("000000"), 4, 0, 0, false},
		Card{131, -1, Card::Hand, Card::GreenItem, 4, 4, 1, bitset<6>("000000"), 0, 0, 0, false},
		Card{132, -1, Card::Hand, Card::GreenItem, 5, 3, 3, bitset<6>("100000"), 0, 0, 0, false},
		Card{133, -1, Card::Hand, Card::GreenItem, 5, 4, 0, bitset<6>("000001"), 0, 0, 0, false},
		Card{134, -1, Card::Hand, Card::GreenItem, 4, 2, 2, bitset<6>("000000"), 0, 0, 1, false},
		Card{135, -1, Card::Hand, Card::GreenItem, 6, 5, 5, bitset<6>("000000"), 0, 0, 0, false},
		Card{136, -1, Card::Hand, Card::GreenItem, 0, 1, 1, bitset<6>("000000"), 0, 0, 0, false},
		Card{137, -1, Card::Hand, Card::GreenItem, 2, 0, 0, bitset<6>("000001"), 0, 0, 0, false},
		Card{138, -1, Card::Hand, Card::GreenItem, 2, 0, 0, bitset<6>("000100"), 0, 0, 1, false},
		Card{139, -1, Card::Hand, Card::GreenItem, 4, 0, 0, bitset<6>("000011"), 0, 0, 0, false},
		Card{140, -1, Card::Hand, Card::GreenItem, 2, 0, 0, bitset<6>("010000"), 0, 0, 0, false},
		Card{141, -1, Card::Hand, Card::RedItem, 0, -1, -1, bitset<6>("000000"), 0, 0, 0, false},
		Card{142, -1, Card::Hand, Card::RedItem, 0, 0, 0, bitset<6>("111111"), 0, 0, 0, false},
		Card{143, -1, Card::Hand, Card::RedItem, 0, 0, 0, bitset<6>("000100"), 0, 0, 0, false},
		Card{144, -1, Card::Hand, Card::RedItem, 1, 0, -2, bitset<6>("000000"), 0, 0, 0, false},
		Card{145, -1, Card::Hand, Card::RedItem, 3, -2, -2, bitset<6>("000000"), 0, 0, 0, false},
		Card{146, -1, Card::Hand, Card::RedItem, 4, -2, -2, bitset<6>("000000"), 0, -2, 0, false},
		Card{147, -1, Card::Hand, Card::RedItem, 2, 0, -1, bitset<6>("000000"), 0, 0, 1, false},
		Card{148, -1, Card::Hand, Card::RedItem, 2, 0, -2, bitset<6>("111111"), 0, 0, 0, false},
		Card{149, -1, Card::Hand, Card::RedItem, 3, 0, 0, bitset<6>("111111"), 0, 0, 1, false},
		Card{150, -1, Card::Hand, Card::RedItem, 2, 0, -3, bitset<6>("000000"), 0, 0, 0, false},
		Card{151, -1, Card::Hand, Card::RedItem, 5, 0, -99, bitset<6>("111111"), 0, 0, 0, false},
		Card{152, -1, Card::Hand, Card::RedItem, 7, 0, -7, bitset<6>("000000"), 0, 0, 1, false},
		Card{153, -1, Card::Hand, Card::BlueItem, 2, 0, 0, bitset<6>("000000"), 5, 0, 0, false},
		Card{154, -1, Card::Hand, Card::BlueItem, 2, 0, 0, bitset<6>("000000"), 0, -2, 1, false},
		Card{155, -1, Card::Hand, Card::BlueItem, 3, 0, -3, bitset<6>("000000"), 0, -1, 0, false},
		Card{156, -1, Card::Hand, Card::BlueItem, 3, 0, 0, bitset<6>("000000"), 3, -3, 0, false},
		Card{157, -1, Card::Hand, Card::BlueItem, 3, 0, -1, bitset<6>("000000"), 1, 0, 1, false},
		Card{158, -1, Card::Hand, Card::BlueItem, 3, 0, -4, bitset<6>("000000"), 0, 0, 0, false},
		Card{159, -1, Card::Hand, Card::BlueItem, 4, 0, -3, bitset<6>("000000"), 3, 0, 0, false},
		Card{160, -1, Card::Hand, Card::BlueItem, 2, 0, 0, bitset<6>("000000"), 2, -2, 0, false}
};

const Card Card::notFound{
		0, 0, Card::Nowhere, Card::Creature, 0, 0, 0, bitset<6>(), 0, 0, 0, false
}; // a dummy card that represents a a card that isn't found

bool Card::operator==(const Card &rhs) const {
	return number == rhs.number &&
		   id == rhs.id;
}

bool Card::operator!=(const Card &rhs) const {
	return !(rhs == *this);
}

ostream &operator<<(ostream &os, const Card &card) {
	os << "number: " << card.number << " id: " << card.id << " location: " << card.location << " type: " << card.type
	   << " cost: " << card.cost << " atk: " << card.atk << " def: " << card.def << " abilities: " << card.abilities
	   << " myhc: " << card.myhc << " ophc: " << card.ophc << " draw: " << card.draw << " sickness: " << card.sickness;
	return os;
}

bool Card::operator<(const Card &rhs) const {
	return id < rhs.id;
}

bool Card::operator>(const Card &rhs) const {
	return id > rhs.id;
}

bool Card::operator<=(const Card &rhs) const {
	return id <= rhs.id;
}

bool Card::operator>=(const Card &rhs) const {
	return id >= rhs.id;
}

bool Cards::exists(const int id) const {
	return any_of(begin(), end(), [id](Card c) { return c.id == id; });
}

Card &Cards::getById(int id) {
	for (Card &card: *this) {
		if (card.id == id) {
			return card;
		}
	}
	throw out_of_range("Error: card not found");
}

const Card &Cards::getById(const int id) const {
	for (const Card &card: *this) {
		if (card.id == id) {
			return card;
		}
	}
	return Card::notFound;
}

void Cards::removeDead() {
	sort(begin(), end(), [](const Card &a, const Card &b) { return a.def > b.def; });
	while (!empty() && back().def <= 0) {
		pop_back();
	}
}

struct Scores : array<double, 160> {

	friend ostream &operator<<(ostream &os, const Scores &scores) {
		os << '[';
		for (int i = 0; i < scores.size() - 1; ++i) {
			os << scores[i] << ',';
			if (i % 10 == 9) { cout << endl; }
		}
		os << scores[scores.size() - 1] << ']';
		return os;
	}
};

struct State;

struct Action {
	enum Type {
		pick, summon, attack, use, pass
	};
	Type type = pass;
	int doer = -1;
	int target = -1;

	friend ostream &operator<<(ostream &os, const Action &action) {
		if (action.type == pick) {
			os << "PICK " << action.doer;
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

struct Actions : vector<Action> {
	friend ostream &operator<<(ostream &os, const Actions &actions) {
		for (const Action &action : actions) {
			os << action;
		}
		return os;
	}
};

const Action pick(const Card &card) { return Action{Action::pick, card.id}; }

const Action summon(const Card &card) { return Action{Action::summon, card.id}; }

const Action attack(const Card &attacker, const Card &target) { return Action{Action::attack, attacker.id, target.id}; }

const Action attack(const Card &attacker) { return Action{Action::attack, attacker.id}; }

const Action use(const Card &item, const Card &target) { return Action{Action::use, item.id, target.id}; }

const Action use(const Card &item) { return Action{Action::use, item.id}; }

const Action pass() { return Action{Action::pass}; }

struct State {
	Cards cards;
	Player me;
	Player op;

	Scores scores;

	void clear() {
		cards.clear();
	}

	void readState();

	const bool isValid(const Action &) const;

	void act(const Action &);

	void act(const Actions &);

	void flipSide();

	const double fieldValue() const;

	void checkGuards();

	const int cardCount(Card::Location loc) const;
};


const bool State::isValid(const Action &action) const {
	if (action.type == Action::pass) {
		return true;
	} else if (action.type == Action::pick) {
		return action.doer >= 0 && action.doer <= 2;
	} else {
		const Card &doer = cards.getById(action.doer);
		if (doer == Card::notFound) { return false; }
		if (action.type == Action::summon) {
			return doer.location == Card::Hand && doer.type == Card::Creature && cardCount(Card::MyField) < 6;
		} else {
			if (action.type == Action::attack) {
				if (doer.location != Card::MyField || doer.sickness) { return false; }
				if (action.target == -1) { return !op.guarded; }
				const Card &target = cards.getById(action.target);
				return ((op.guarded && target.abilities[Card::Guard]) || !op.guarded) &&
					   target != Card::notFound && target.location == Card::OpField;
			} else if (action.type == Action::use) {
				if (action.target == -1) { return doer.type == Card::BlueItem; }
				const Card &target = cards.getById(action.target);
				return target != Card::notFound;
			}
		}
	}
	return true;
}

void State::act(const Action &action) {
	if (action.type == Action::pass) {
		return;
	}
	Card &doer = cards.getById(action.doer);
	if (action.type == Action::attack) {
		doer.sickness = true;
		if (action.target == -1) {
			op.health -= doer.atk;
		} else {
			Card &enemy = cards.getById(action.target);
			if (doer.atk > 0) {
				if (enemy.abilities[Card::Ward]) {
					enemy.abilities[Card::Ward] = false;
				} else {
					enemy.def -= doer.abilities[Card::Lethal] ? max(enemy.def, doer.atk) : doer.atk;
					me.health += doer.abilities[Card::Drain] ? doer.atk : 0;
				}
			}
			if (enemy.atk > 0) {
				if (doer.abilities[Card::Ward]) {
					doer.abilities[Card::Ward] = false;
				} else {
					doer.def -= enemy.abilities[Card::Lethal] ? doer.def : enemy.atk;
					op.health += enemy.abilities[Card::Drain] ? enemy.atk : 0;
				}
			}
			if (enemy.def <= 0 && doer.abilities[Card::Trample]) {
				op.health += enemy.def;
			}
			cards.removeDead();
			checkGuards();
		}
	} else if (action.type == Action::summon || action.type == Action::use) {
		me.mana -= doer.cost;
		me.health += doer.myhc;
		op.health += doer.ophc;
		me.draw += doer.draw;
		if (action.type == Action::summon) {
			doer.location = Card::MyField;
			doer.sickness = !doer.abilities[Card::Charge];
			doer.myhc = doer.ophc = doer.draw = 0;
			checkGuards();
		} else if (action.type == Action::use) {
			if (action.target == -1) {
				op.health += doer.def;
			} else if (action.target != -1) {
				Card &target = cards.getById(action.target);
				if (doer.type == Card::GreenItem) {
					bool wasCharge = target.abilities[Card::Charge];
					target.abilities |= doer.abilities;
					if (!wasCharge && target.abilities[Card::Charge]) {
						target.sickness = false;
					}
				} else if (doer.type == Card::RedItem) {
					target.abilities &= ~doer.abilities;
				}
				target.def += doer.def;
				target.atk += doer.atk;
				if (target.def <= 0) {
					swap(target, cards.back());
					cards.pop_back();
				}
				checkGuards();
			}
			swap(doer, cards.back());
			cards.pop_back();
		}
	}
	while (me.rune * 5 >= me.health) {
		me.draw++;
		me.rune--;
	}
	while (op.rune * 5 >= op.health) {
		op.draw++;
		op.rune--;
	}
	me.draw = min(8 - cardCount(Card::Hand), me.draw);
}

void State::act(const Actions &actions) {
	for (const Action &action : actions) {
		act(action);
	}
}

void State::readState() {
	clear();
	cin >> me.health >> me.mana >> me.deck >> me.rune;
	cin.ignore();
	cin >> op.health >> op.mana >> op.deck >> op.rune;
	cin.ignore();
	int opponentHand, cardCount;
	cin >> opponentHand;
	cin.ignore();
	cin >> cardCount;
	cin.ignore();
	cards.reserve((unsigned long) cardCount);
	for (int i = 0; i < cardCount; i++) {
		Card card;
		int location, type;
		string abilities;
		cin >> card.number >> card.id >> location >> type >> card.cost >> card.atk >> card.def
			>> abilities >> card.myhc >> card.ophc >> card.draw;
		if (me.mana == 0) {
			card.id = i;
		}
		cin.ignore();
		card.location = (Card::Location) location;
		card.type = (Card::Type) type;
		card.sickness = location == Card::Hand;
		if (card.location != Card::Hand) {
			card.myhc = card.ophc = card.draw = 0;
		}
		for (int j = 0; j < abilities.size(); ++j) {
			if (abilities[j] != '-') {
				card.abilities[j] = true;
			}
		}
		cards.push_back(card);
	}
	checkGuards();
}

const double State::fieldValue() const {
	double result = 0;
	int opAttack = 0;
	for (const Card &card: cards) {
		if (card.location == Card::Hand && card.type != Card::Creature) {
			result += card.value() / 1.5;
		} else if (card.location == Card::MyField) {
			result += card.value();
			if (card.abilities[Card::Guard]) {
				opAttack -= card.def;
			}
		} else if (card.location == Card::OpField) {
			result -= card.value();
			opAttack += card.atk;
		}
	}

	result -= (me.draw - min(8 - cardCount(Card::Hand), me.draw)) * 8;
	if (op.health <= 0) { return result + 10000; }
	else if (me.health <= opAttack) { return result - 10000; }
	result += log((double) me.health) - log((double) op.health)*2;
	cerr << "hi";
	return result;
}

void State::checkGuards() {
	me.guarded = op.guarded = false;
	for (Card &card : cards) {
		if (card.location == Card::MyField) {
			me.guarded |= card.abilities[Card::Guard];
		} else if (card.location == Card::OpField) {
			op.guarded |= card.abilities[Card::Guard];
		}
	}
}

const int State::cardCount(const Card::Location loc) const {
	int result = 0;
	for (const Card &card : cards) {
		if (card.location == loc) {
			result += 1;
		}
	}
	return result;
}

void State::flipSide() {
	Cards nextCards;
	for (const Card &card : cards) {
		if (card.location != Card::Hand) {
			Card tmp(card);
			tmp.location = card.location == Card::MyField ? Card::OpField : Card::MyField;
			tmp.sickness = false;
			tmp.myhc = tmp.ophc = tmp.draw = 0;
			nextCards.push_back(tmp);
		}
	}
	cards = nextCards;
	swap(me, op);
}

const Actions findAllActions(const State &state) {
	Actions result;
	result.reserve(64);
	for (const Card &card: state.cards) {
		if (card.location == Card::Hand && card.cost <= state.me.mana) {
			if (card.type == Card::Creature && state.isValid(summon(card))) {
				result.push_back(summon(card));
			} else if (card.type == Card::GreenItem) {
				for (const Card &target: state.cards) {
					if (target.location == Card::MyField && state.isValid(use(card, target))) {
						result.push_back(use(card, target));
					}
				}
			} else if (card.type == Card::RedItem) {
				for (const Card &target: state.cards) {
					if (target.location == Card::OpField && state.isValid(use(card, target))) {
						result.push_back(use(card, target));
					}
				}
			} else if (card.type == Card::BlueItem) {
				if (state.isValid(use(card))) {
					result.push_back(use(card));
				}
				for (const Card &target: state.cards) {
					if (target.location == Card::OpField && state.isValid(use(card, target))) {
						result.push_back(use(card, target));
					}
				}
			}
		} else if (card.location == Card::MyField) {
			if (state.isValid(attack(card))) {
				result.push_back(attack(card));
			}
			for (const Card &target: state.cards) {
				if (target.location == Card::OpField && state.isValid(attack(card, target))) {
					result.push_back(attack(card, target));
				}
			}
		}
	}
	result.push_back(pass());
	return result;
}

Action highestValueAction(const State &state) {
	Actions toTry = findAllActions(state);
	Action result;
	double bestValue = -10000000;
	for (Action &action : toTry) {
		State s(state);
		s.act(action);
		double val = s.fieldValue();
		if (val > bestValue) {
			bestValue = val;
			result = action;
		}
	}
	return result;
}

Action findBestAction(const State &state) {
	Actions toTry = findAllActions(state);
	Action result;
	double bestValue = -10000000;
	for (Action &action : toTry) {
		State s(state);
		s.act(action);
		auto nact = highestValueAction(s);
		while (nact.type != Action::pass) {
			s.act(nact);
			nact = highestValueAction(s);
		}
		s.flipSide();
		nact = highestValueAction(s);
		while (nact.type != Action::pass) {
			s.act(nact);
			nact = highestValueAction(s);
		}
		s.flipSide();
		s.act(pass());
		double val = s.fieldValue();
		if (val > bestValue) {
			bestValue = val;
			result = action;
		}
	}
	return result;
}

Actions findBestActions(const State &state) {
	Actions result;
	State s(state);
	result.push_back(findBestAction(state));
	while (result.back().type != Action::pass) {
		s.act(result.back());
		result.push_back(findBestAction(s));
	}
	return result;
}

const array<double, 160> draftGuide = {
		0.45977, 0.22187, 0.56624, 0.16868, 0.74898, 0.61970, 0.29331, 0.36603, 0.45274, 0.04711, 0.73541, 0.18487,
		0.56719, 0.43182, 0.44353, 0.36201, 0.45479, 1.27507, 0.45461, 0.26474, 0.61298, 0.38477, 0.63867, 0.13433,
		0.47905, 0.75390, 0.32897, 0.06140, 0.20176, 0.60322, 0.10535, 0.21882, 0.19620, 0.07414, 0.18135, 0.01590,
		0.30371, 0.24984, 0.26994, 0.20440, 0.01425, 0.00967, 0.07367, 0.30157, 1.03179, 0.09468, 0.31005, 0.11754,
		0.23289, 0.37436, 0.39629, -0.14110, -0.37985, -0.01325, 0.13354, 0.00250, -0.14637, 0.62729, 0.50096, 0.13457,
		0.44149, 0.60580, 0.05779, 0.04306, 0.29440, 0.03854, 0.29715, 0.58277, 2.20764, 2.16930, 0.28832, 1.43484,
		1.94359, 2.72560, 1.97038, 0.96776, 0.97009, 0.28030, 0.88443, 2.35881, 0.11137, 0.51738, 0.39080, 0.04147,
		0.10494, 0.07141, 0.33050, 0.12711, 0.00983, 0.00854, 0.63672, 0.01066, 0.80907, 0.38650, 0.84453, 1.31205,
		0.91281, 0.49572, 0.73062, 0.25975, 0.45974, 0.37695, 1.04181, 1.12244, 1.03622, 1.38386, 0.18218, 0.10709,
		0.48385, -0.00848, 1.50644, 0.69526, -0.01766, 1.44426, 0.22816, 0.91617, 0.23242, 0.02268, 0.01449, 0.12162,
		-0.04564, 0.21570, 0.11422, -0.01648, -0.10309, -0.06570, -0.06816, -0.03794, -0.14922, -0.08111, -0.13985,
		0.03169, -0.34431, -0.25880, -0.01231, 0.05541, -0.17726, 0.09344, 0.02509, -0.06590, 0.34357, 0.08508, 0.07958,
		0.35447, -0.05666, -0.13071, 0.02988, 0.14867, -0.20032, 0.28060, 0.24174, -0.17413, 0.07898, 0.01828, 0.02746,
		-0.01428, -0.14497, 0.09487, -0.08558, 0.07097
};

//const array<double, 160> draftGuide = {
//		0.22849, 0.14110, 0.31307, 0.15880, 0.40403, 0.34400, 0.20107, 0.26296, 0.38562, -0.09344, 0.42202, 0.18805,
//		0.39741, 0.49459, 0.65126, 0.40484, 0.64423, 1.00909, 0.91997, 0.48282, 0.88485, 0.84665, 1.49986, 0.15834,
//		0.21000, 0.36887, 0.17120, 0.04911, 0.09789, 0.15215, 0.05590, 0.14527, 0.36421, 0.34343, 0.67929, 0.31757,
//		1.02964, 0.15631, 0.14965, -0.01925, -0.05192, -0.09270, 0.19951, 0.58423, 1.01700, 0.42459, 0.20684, 0.11533,
//		0.17586, 0.09277, 0.43908, 0.09959, 0.03845, -0.02481, 0.40635, 0.20334, -0.05982, 1.56605, 1.10321, 0.66246,
//		1.63028, 5.86517, 0.12109, 0.09551, 0.20335, 0.21529, 0.53030, 0.87223, 2.14018, 2.15219, 1.19436, 1.60233,
//		1.99482, 3.65635, 2.63488, 1.12757, 2.44287, 0.96713, 2.65750, 7.32187, 1.29846, 0.55958, 0.52475, 0.62330,
//		0.33164, 0.65816, 1.00128, 0.50847, 0.19434, 0.46248, 0.35507, -0.04450, 0.33544, 0.38937, 0.51947, 0.55157,
//		0.46857, 0.43021, 0.65281, 0.33875, 0.58101, 0.28621, 1.07249, 0.91278, 1.33913, 1.46046, 0.24836, 0.36779,
//		0.92966, 0.31226, 1.98390, 1.31035, 0.17068, 2.43928, 0.97604, 4.65451, 0.01865, -0.03758, -0.03475, 0.11906,
//		-0.19186, 0.09867, -0.10292, -0.11211, -0.17729, -0.19697, -0.10013, -0.41069, -0.39976, -0.14618, -0.31576,
//		-0.87889, -0.18010, -0.60168, -0.90637, -0.07378, 0.05495, 0.08251, 0.17773, -0.00063, -0.01832, -0.25496,
//		-0.27967, 0.11023, -0.12580, -0.22160, 0.03495, -0.03051, -0.04304, 0.13577, -0.10629, -0.17221, 0.04333,
//		0.06390, -0.09974, -0.03614, -0.02908, -0.05215, -0.04725, 0.08326
//};

struct Agent {
	Cards deck;
	State state;
	int mana = 0;

	Scores scores = {};
};

struct Game {
	array<Card, 60> pickable;
	Agent murder;
	Agent killer;
	bool murderTurn = false;
	//bool            drafting   = true;
	int nextId = 0;

	random_device rd;
	mt19937 mt;

	Game() {
		mt.seed(rd());
		array<Card, 160> picks(pool);
		shuffle(picks.begin(), picks.end(), mt);
		for (int i = 0; i < pickable.size(); ++i) {
			pickable[i] = picks[i];
		}
		murder.deck.reserve(30);
		killer.deck.reserve(30);
		uniform_int_distribution<> dis(0, 2);
		for (int i = 0; i < 30; ++i) {
			for (int j = 0; j < 3; ++j) {

			}
			murder.deck.push_back(pickable[dis(mt)]);
			killer.deck.push_back(pickable[dis(mt)]);
			shuffle(pickable.begin(), pickable.end(), mt);
		}
		for (int i = 29; i >= 0; --i) {
			murder.deck[i].id = ++nextId;
			killer.deck[i].id = ++nextId;
		}
	}

//    Cards draft() {
//        Cards    result{};
//        for (int i = 0; i < 3; ++i) {
//            result.push_back(pickable[i]);
//        }
//        shuffle(pickable.begin(), pickable.end(), mt);
//        return result;
//    }

//    void pick(const Card &card) {
//        if (murderTurn) {
//            murder.deck.push_back(card);
//        } else {
//            killer.deck.push_back(card);
//        }
//    }
	void start() {
		for (int i = 0; i < 4; ++i) {
			murder.state.cards.push_back(murder.deck.back());
			murder.deck.pop_back();
		}
		murder.state.me.deck = killer.state.op.deck = 26;
		for (int i = 0; i < 5; ++i) {
			killer.state.cards.push_back(killer.deck.back());
			killer.deck.pop_back();
		}
		killer.state.me.deck = murder.state.op.deck = 25;
	}

	void turn() {
		Agent &current = murderTurn ? murder : killer;
		Agent &next = murderTurn ? killer : murder;
		sort(current.state.cards.begin(), current.state.cards.end());
		sort(next.state.cards.begin(), next.state.cards.end());
		Cards nextCards;
		for (const Card &card : next.state.cards) {
			if (card.location == Card::Hand) {
				nextCards.push_back(card);
			}
		}
		for (const Card &card : current.state.cards) {
			if (card.location != Card::Hand) {
				Card tmp(card);
				tmp.location = card.location == Card::MyField ? Card::OpField : Card::MyField;
				tmp.sickness = false;
				tmp.myhc = tmp.ophc = tmp.draw = 0;

				nextCards.push_back(tmp);
			}
		}
		next.state.cards = nextCards;
		next.state.me = current.state.op;
		next.state.op = current.state.me;
		while (next.state.me.draw > 0) {
			if (next.deck.empty()) {
				next.state.me.health = min(next.state.me.health, --next.state.me.rune * 5);
			}
			if (next.state.cardCount(Card::Hand) >= 8) { break; }
			if (!next.deck.empty()) {
				next.state.cards.push_back(next.deck.back());
				next.deck.pop_back();
			}
			next.state.me.draw--;
		}
		next.state.me.draw = 1;
		next.state.me.deck = static_cast<int>(next.deck.size());
		if (next.mana < 12) { next.mana++; }
		next.state.me.mana = next.mana;
		murderTurn = !murderTurn;
	}

	State &currentState() {
		if (murderTurn) {
			return murder.state;
		} else {
			return killer.state;
		}
	}

	void act(const Action &action) {
		if (murderTurn) {
			murder.state.act(action);
		} else {
			killer.state.act(action);
		}
	}

	void act(const Actions &actions) {
		Agent &current = murderTurn ? murder : killer;
//        auto              val      = current.state.fieldValue();
//        vector<int>       doers;
		//updateScores(actions);
		for (const Action &action : actions) {
			int dor;
			if (action.type != Action::pass) {
				dor = (current.state.cards.getById(action.doer).number);
			}
			auto val = current.state.fieldValue();
			if (action.type == Action::summon || action.type == Action::use) {
				val += current.state.cards.getById(action.doer).cost;
			}
			act(action);
			val = current.state.fieldValue() - val;
			if (action.type != Action::pass && abs(val) < 5000) { current.scores.at(dor - 1) += val; }
		}
//        for (int dr: doers) {
//            if (abs(val) < 5000) { current.scores.at(dr - 1) += val; }
//        }
	}

	int winner() {
		if (murder.state.me.health <= 0 || killer.state.op.health <= 0) {
			return -1;
		} else if (killer.state.me.health <= 0 || murder.state.op.health <= 0) {
			return 1;
		} else {
			return 0;
		}
	}

	void updateScores(const Actions &action) {
		Agent &current = murderTurn ? murder : killer;
		for (const Card &card: current.state.cards) {
			if (card.location != Card::OpField) {
				current.scores.at(card.number - 1) = 1;
			}
		}
//        for (const Action &action : actions) {
//            if (action.type != Action::pass) {
//                const Card &doer = current.state.cards.getById(action.doer);
//                if (action.type == Action::summon) {
//                    current.scores.at(doer.number - 1) += 1;
//                } else if (action.type == Action::use) {
//                    current.scores.at(doer.number - 1) += 1;
//                } else if (action.type == Action::attack) {
//                    //current.scores[doer.number-1] += 1;
//                }
//            }
//        }
	}
};

Scores simulate(const int games) {
	Scores scores = {};
	for (int iters = 0; iters < games; ++iters) {
		Game game;
		game.start();
		while (game.winner() == 0) {
			game.turn();
			auto bla = findBestActions(game.currentState());
			//cout << bla << endl;
			game.act(bla);\

		}
		for (int i = 0; i < 160; ++i) {
			if (game.winner() == 1) {
				game.killer.scores[i] = -game.killer.scores[i];
			} else if (game.winner() == -1) {
				game.murder.scores[i] = -game.murder.scores[i];
			}
			scores[i] += game.murder.scores[i] + game.killer.scores[i];
		}
	}
	return scores;
}

int main() {

	const bool testing = false;
	const bool multithreaded = true;

	//Game            game;
	State state;
	vector<int> curve{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

	if (testing) {
		Scores scores = {};
		if (multithreaded) {
			vector<future<Scores>> futures;
			const int threadCount = 100;
			futures.reserve(threadCount);
			for (int i = 0; i < threadCount; ++i) {
				futures.push_back(async(launch::async, simulate, 10000));
			}
			int fuleft = futures.size();
			while (fuleft > 0) {
				for (auto &&future : futures) {
					if (future.valid()) {
						auto res = future.get();
						for (int j = 0; j < scores.size(); ++j) {
							scores[j] += res[j];
						}
						cout << scores << endl << --fuleft << " futures left" << endl;
					}
				}
			}
		} else {
			auto res = simulate(1000);
			for (int j = 0; j < 160; ++j) {
				scores[j] += res[j];
			}
			cout << scores << endl;
		}

	} else {
//		state.me.mana = 6;
//		state.me.health = 29;
//		state.me.guarded = true;
//		state.op.health = 25;
//		state.op.guarded = true;
//		state.cards.push_back(pool[43]);
//		state.cards.back().id = 1;
//		state.cards.push_back(pool[104]);
//		state.cards.back().id = 2;
//		state.cards.back().location = Card::MyField;
//		state.cards.push_back(pool[104]);
//		state.cards.back().id = 3;
//		state.cards.back().location = Card::OpField;
//		state.cards.push_back(pool[14]);
//		state.cards.back().id = 4;
//		state.cards.back().location = Card::OpField;
//		state.cards.push_back(pool[6]);
//		state.cards.back().id = 5;
//		state.cards.back().location = Card::OpField;
		// game loop
		for (int iterations = 0; iterations < 1000; iterations++) {
			state.readState();
			if (state.me.mana != 0) { // Battle phase
				cout << findBestActions(state);
			} else { // Draft Phase
				// Selects the most undervalued card according to card.value() with some curve adhesion.
				Card nobody;
				Card &bestPick = nobody;
				double bestValue = -100000;
				for (Card &card : state.cards) {
					//double value = (card.value() - card.cost - curve[card.cost] / 2.0f) * log((double) card.cost + 1.0);
					double value = draftGuide[card.number - 1];
					if (value > bestValue) {
						bestValue = value;
						bestPick = card;
					}
				}
				int cost = bestPick.cost;
				curve[cost]++;
				cout << pick(bestPick);
			}
			cout << endl;
//			break;
		}
	}
}