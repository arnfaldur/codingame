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

const double weights[] = {
        0.4584541479495468f,
        0.49527483386905563f,
        0.37846622132137486f,
        0.6152927267893537f,
        0.6658405997411934f,
        0.0028051665777753705f,
        0.48701160912925323f,
        0.4301448215532352f,
        0.17407016799401315f,
        0.39195517614226116f,
        0.40574233582520924f,
        0.3801825566690368f,
        0.24578153054337448f,
        -0.3091025411766633f,
        0.539238762361815f
};
const double bias      = 0.8940729693697819f;

struct Player {
    int  health  = 30;
    int  mana;
    int  deck    = 30;
    int  rune    = 5;
    int  draw    = 1;
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
};

struct Card { ;

    enum Location { opField = -1, hand, myField };
    enum Type { creature, greenItem, redItem, blueItem };
    enum Abilities { trample, charge, drain, guard, lethal, ward };

    int      number;
    int      id;
    Location location;
    Type     type;

    bool operator<(const Card &rhs) const;

    bool operator>(const Card &rhs) const;

    bool operator<=(const Card &rhs) const;

    bool operator>=(const Card &rhs) const;

    int cost;
    int atk;
    int def;

    bitset<6> abilities;
    int       myhc;
    int       ophc;
    int       draw;
    bool      sickness;

    friend ostream &operator<<(ostream &os, const Card &card);

    const static Card notFound;

    bool operator==(const Card &rhs) const;

    bool operator!=(const Card &rhs) const;

    const double value() const {
        double result = weights[type];
//        result += cost / 1.9f;
        result += weights[4] * atk;
        result += weights[5] * def;
        result += weights[6] * abilities[trample];
        result += weights[7] * abilities[charge];
        result += weights[8] * abilities[drain];
        result += weights[9] * abilities[guard];
        result += weights[10] * abilities[lethal];
        result += weights[11] * abilities[ward];
        result += weights[12] * myhc;
        result += weights[13] * ophc;
        result += weights[14] * draw;
        return result + bias;
    }
};

//const Card pool[] = {
const array<Card, 160> pool{
        //Card{0, 0, Card::hand, Card::creature, 0, 0, 0, bitset<6>("000000"), 0, 0, 0, false},
        Card{1, -1, Card::hand, Card::creature, 1, 2, 1, bitset<6>("000000"), 1, 0, 0, false},
        Card{2, -1, Card::hand, Card::creature, 1, 1, 2, bitset<6>("000000"), 0, -1, 0, false},
        Card{3, -1, Card::hand, Card::creature, 1, 2, 2, bitset<6>("000000"), 0, 0, 0, false},
        Card{4, -1, Card::hand, Card::creature, 2, 1, 5, bitset<6>("000000"), 0, 0, 0, false},
        Card{5, -1, Card::hand, Card::creature, 2, 4, 1, bitset<6>("000000"), 0, 0, 0, false},
        Card{6, -1, Card::hand, Card::creature, 2, 3, 2, bitset<6>("000000"), 0, 0, 0, false},
        Card{7, -1, Card::hand, Card::creature, 2, 2, 2, bitset<6>("000001"), 0, 0, 0, false},
        Card{8, -1, Card::hand, Card::creature, 2, 2, 3, bitset<6>("000000"), 0, 0, 0, false},
        Card{9, -1, Card::hand, Card::creature, 3, 3, 4, bitset<6>("000000"), 0, 0, 0, false},
        Card{10, -1, Card::hand, Card::creature, 3, 3, 1, bitset<6>("001000"), 0, 0, 0, false},
        Card{11, -1, Card::hand, Card::creature, 3, 5, 2, bitset<6>("000000"), 0, 0, 0, false},
        Card{12, -1, Card::hand, Card::creature, 3, 2, 5, bitset<6>("000000"), 0, 0, 0, false},
        Card{13, -1, Card::hand, Card::creature, 4, 5, 3, bitset<6>("000000"), 1, -1, 0, false},
        Card{14, -1, Card::hand, Card::creature, 4, 9, 1, bitset<6>("000000"), 0, 0, 0, false},
        Card{15, -1, Card::hand, Card::creature, 4, 4, 5, bitset<6>("000000"), 0, 0, 0, false},
        Card{16, -1, Card::hand, Card::creature, 4, 6, 2, bitset<6>("000000"), 0, 0, 0, false},
        Card{17, -1, Card::hand, Card::creature, 4, 4, 5, bitset<6>("000000"), 0, 0, 0, false},
        Card{18, -1, Card::hand, Card::creature, 4, 7, 4, bitset<6>("000000"), 0, 0, 0, false},
        Card{19, -1, Card::hand, Card::creature, 5, 5, 6, bitset<6>("000000"), 0, 0, 0, false},
        Card{20, -1, Card::hand, Card::creature, 5, 8, 2, bitset<6>("000000"), 0, 0, 0, false},
        Card{21, -1, Card::hand, Card::creature, 5, 6, 5, bitset<6>("000000"), 0, 0, 0, false},
        Card{22, -1, Card::hand, Card::creature, 6, 7, 5, bitset<6>("000000"), 0, 0, 0, false},
        Card{23, -1, Card::hand, Card::creature, 7, 8, 8, bitset<6>("000000"), 0, 0, 0, false},
        Card{24, -1, Card::hand, Card::creature, 1, 1, 1, bitset<6>("000000"), 0, -1, 0, false},
        Card{25, -1, Card::hand, Card::creature, 2, 3, 1, bitset<6>("000000"), -2, -2, 0, false},
        Card{26, -1, Card::hand, Card::creature, 2, 3, 2, bitset<6>("000000"), 0, -1, 0, false},
        Card{27, -1, Card::hand, Card::creature, 2, 2, 2, bitset<6>("000000"), 2, 0, 0, false},
        Card{28, -1, Card::hand, Card::creature, 2, 1, 2, bitset<6>("000000"), 0, 0, 1, false},
        Card{29, -1, Card::hand, Card::creature, 2, 2, 1, bitset<6>("000000"), 0, 0, 1, false},
        Card{30, -1, Card::hand, Card::creature, 3, 4, 2, bitset<6>("000000"), 0, -2, 0, false},
        Card{31, -1, Card::hand, Card::creature, 3, 3, 1, bitset<6>("000000"), 0, -1, 0, false},
        Card{32, -1, Card::hand, Card::creature, 3, 3, 2, bitset<6>("000000"), 0, 0, 1, false},
        Card{33, -1, Card::hand, Card::creature, 4, 4, 3, bitset<6>("000000"), 0, 0, 1, false},
        Card{34, -1, Card::hand, Card::creature, 5, 3, 5, bitset<6>("000000"), 0, 0, 1, false},
        Card{35, -1, Card::hand, Card::creature, 6, 5, 2, bitset<6>("100000"), 0, 0, 1, false},
        Card{36, -1, Card::hand, Card::creature, 6, 4, 4, bitset<6>("000000"), 0, 0, 2, false},
        Card{37, -1, Card::hand, Card::creature, 6, 5, 7, bitset<6>("000000"), 0, 0, 1, false},
        Card{38, -1, Card::hand, Card::creature, 1, 1, 3, bitset<6>("001000"), 0, 0, 0, false},
        Card{39, -1, Card::hand, Card::creature, 1, 2, 1, bitset<6>("001000"), 0, 0, 0, false},
        Card{40, -1, Card::hand, Card::creature, 3, 2, 3, bitset<6>("001100"), 0, 0, 0, false},
        Card{41, -1, Card::hand, Card::creature, 3, 2, 2, bitset<6>("011000"), 0, 0, 0, false},
        Card{42, -1, Card::hand, Card::creature, 4, 4, 2, bitset<6>("001000"), 0, 0, 0, false},
        Card{43, -1, Card::hand, Card::creature, 6, 5, 5, bitset<6>("001000"), 0, 0, 0, false},
        Card{44, -1, Card::hand, Card::creature, 6, 3, 7, bitset<6>("001010"), 0, 0, 0, false},
        Card{45, -1, Card::hand, Card::creature, 6, 6, 5, bitset<6>("101000"), -3, 0, 0, false},
        Card{46, -1, Card::hand, Card::creature, 9, 7, 7, bitset<6>("001000"), 0, 0, 0, false},
        Card{47, -1, Card::hand, Card::creature, 2, 1, 5, bitset<6>("001000"), 0, 0, 0, false},
        Card{48, -1, Card::hand, Card::creature, 1, 1, 1, bitset<6>("000010"), 0, 0, 0, false},
        Card{49, -1, Card::hand, Card::creature, 2, 1, 2, bitset<6>("000110"), 0, 0, 0, false},
        Card{50, -1, Card::hand, Card::creature, 3, 3, 2, bitset<6>("000010"), 0, 0, 0, false},
        Card{51, -1, Card::hand, Card::creature, 4, 3, 5, bitset<6>("000010"), 0, 0, 0, false},
        Card{52, -1, Card::hand, Card::creature, 4, 2, 4, bitset<6>("000010"), 0, 0, 0, false},
        Card{53, -1, Card::hand, Card::creature, 4, 1, 1, bitset<6>("010010"), 0, 0, 0, false},
        Card{54, -1, Card::hand, Card::creature, 3, 2, 2, bitset<6>("000010"), 0, 0, 0, false},
        Card{55, -1, Card::hand, Card::creature, 2, 0, 5, bitset<6>("000100"), 0, 0, 0, false},
        Card{56, -1, Card::hand, Card::creature, 4, 2, 7, bitset<6>("000000"), 0, 0, 0, false},
        Card{57, -1, Card::hand, Card::creature, 4, 1, 8, bitset<6>("000000"), 0, 0, 0, false},
        Card{58, -1, Card::hand, Card::creature, 6, 5, 6, bitset<6>("100000"), 0, 0, 0, false},
        Card{59, -1, Card::hand, Card::creature, 7, 7, 7, bitset<6>("000000"), 1, -1, 0, false},
        Card{60, -1, Card::hand, Card::creature, 7, 4, 8, bitset<6>("000000"), 0, 0, 0, false},
        Card{61, -1, Card::hand, Card::creature, 9, 10, 10, bitset<6>("000000"), 0, 0, 0, false},
        Card{62, -1, Card::hand, Card::creature, 12, 12, 12, bitset<6>("100100"), 0, 0, 0, false},
        Card{63, -1, Card::hand, Card::creature, 2, 0, 4, bitset<6>("000101"), 0, 0, 0, false},
        Card{64, -1, Card::hand, Card::creature, 2, 1, 1, bitset<6>("000101"), 0, 0, 0, false},
        Card{65, -1, Card::hand, Card::creature, 2, 2, 2, bitset<6>("000001"), 0, 0, 0, false},
        Card{66, -1, Card::hand, Card::creature, 5, 5, 1, bitset<6>("000001"), 0, 0, 0, false},
        Card{67, -1, Card::hand, Card::creature, 6, 5, 5, bitset<6>("000001"), 0, -2, 0, false},
        Card{68, -1, Card::hand, Card::creature, 6, 7, 5, bitset<6>("000001"), 0, 0, 0, false},
        Card{69, -1, Card::hand, Card::creature, 3, 4, 4, bitset<6>("100000"), 0, 0, 0, false},
        Card{70, -1, Card::hand, Card::creature, 4, 6, 3, bitset<6>("100000"), 0, 0, 0, false},
        Card{71, -1, Card::hand, Card::creature, 4, 3, 2, bitset<6>("110000"), 0, 0, 0, false},
        Card{72, -1, Card::hand, Card::creature, 4, 5, 3, bitset<6>("100000"), 0, 0, 0, false},
        Card{73, -1, Card::hand, Card::creature, 4, 4, 4, bitset<6>("100000"), 4, 0, 0, false},
        Card{74, -1, Card::hand, Card::creature, 5, 5, 4, bitset<6>("100100"), 0, 0, 0, false},
        Card{75, -1, Card::hand, Card::creature, 5, 6, 5, bitset<6>("100000"), 0, 0, 0, false},
        Card{76, -1, Card::hand, Card::creature, 6, 5, 5, bitset<6>("101000"), 0, 0, 0, false},
        Card{77, -1, Card::hand, Card::creature, 7, 7, 7, bitset<6>("100000"), 0, 0, 0, false},
        Card{78, -1, Card::hand, Card::creature, 8, 5, 5, bitset<6>("100000"), 0, -5, 0, false},
        Card{79, -1, Card::hand, Card::creature, 8, 8, 8, bitset<6>("100000"), 0, 0, 0, false},
        Card{80, -1, Card::hand, Card::creature, 8, 8, 8, bitset<6>("100100"), 0, 0, 1, false},
        Card{81, -1, Card::hand, Card::creature, 9, 6, 6, bitset<6>("110000"), 0, 0, 0, false},
        Card{82, -1, Card::hand, Card::creature, 7, 5, 5, bitset<6>("101001"), 0, 0, 0, false},
        Card{83, -1, Card::hand, Card::creature, 0, 1, 1, bitset<6>("010000"), 0, 0, 0, false},
        Card{84, -1, Card::hand, Card::creature, 2, 1, 1, bitset<6>("011001"), 0, 0, 0, false},
        Card{85, -1, Card::hand, Card::creature, 3, 2, 3, bitset<6>("010000"), 0, 0, 0, false},
        Card{86, -1, Card::hand, Card::creature, 3, 1, 5, bitset<6>("010000"), 0, 0, 0, false},
        Card{87, -1, Card::hand, Card::creature, 4, 2, 5, bitset<6>("010100"), 0, 0, 0, false},
        Card{88, -1, Card::hand, Card::creature, 5, 4, 4, bitset<6>("010000"), 0, 0, 0, false},
        Card{89, -1, Card::hand, Card::creature, 5, 4, 1, bitset<6>("010000"), 2, 0, 0, false},
        Card{90, -1, Card::hand, Card::creature, 8, 5, 5, bitset<6>("010000"), 0, 0, 0, false},
        Card{91, -1, Card::hand, Card::creature, 0, 1, 2, bitset<6>("000100"), 0, 1, 0, false},
        Card{92, -1, Card::hand, Card::creature, 1, 0, 1, bitset<6>("000100"), 2, 0, 0, false},
        Card{93, -1, Card::hand, Card::creature, 1, 2, 1, bitset<6>("000100"), 0, 0, 0, false},
        Card{94, -1, Card::hand, Card::creature, 2, 1, 4, bitset<6>("000100"), 0, 0, 0, false},
        Card{95, -1, Card::hand, Card::creature, 2, 2, 3, bitset<6>("000100"), 0, 0, 0, false},
        Card{96, -1, Card::hand, Card::creature, 2, 3, 2, bitset<6>("000100"), 0, 0, 0, false},
        Card{97, -1, Card::hand, Card::creature, 3, 3, 3, bitset<6>("000100"), 0, 0, 0, false},
        Card{98, -1, Card::hand, Card::creature, 3, 2, 4, bitset<6>("000100"), 0, 0, 0, false},
        Card{99, -1, Card::hand, Card::creature, 3, 2, 5, bitset<6>("000100"), 0, 0, 0, false},
        Card{100, -1, Card::hand, Card::creature, 3, 1, 6, bitset<6>("000100"), 0, 0, 0, false},
        Card{101, -1, Card::hand, Card::creature, 4, 3, 4, bitset<6>("000100"), 0, 0, 0, false},
        Card{102, -1, Card::hand, Card::creature, 4, 3, 3, bitset<6>("000100"), 0, -1, 0, false},
        Card{103, -1, Card::hand, Card::creature, 4, 3, 6, bitset<6>("000100"), 0, 0, 0, false},
        Card{104, -1, Card::hand, Card::creature, 4, 4, 4, bitset<6>("000100"), 0, 0, 0, false},
        Card{105, -1, Card::hand, Card::creature, 5, 4, 6, bitset<6>("000100"), 0, 0, 0, false},
        Card{106, -1, Card::hand, Card::creature, 5, 5, 5, bitset<6>("000100"), 0, 0, 0, false},
        Card{107, -1, Card::hand, Card::creature, 5, 3, 3, bitset<6>("000100"), 3, 0, 0, false},
        Card{108, -1, Card::hand, Card::creature, 5, 2, 6, bitset<6>("000100"), 0, 0, 0, false},
        Card{109, -1, Card::hand, Card::creature, 5, 5, 6, bitset<6>("000000"), 0, 0, 0, false},
        Card{110, -1, Card::hand, Card::creature, 5, 0, 9, bitset<6>("000100"), 0, 0, 0, false},
        Card{111, -1, Card::hand, Card::creature, 6, 6, 6, bitset<6>("000100"), 0, 0, 0, false},
        Card{112, -1, Card::hand, Card::creature, 6, 4, 7, bitset<6>("000100"), 0, 0, 0, false},
        Card{113, -1, Card::hand, Card::creature, 6, 2, 4, bitset<6>("000100"), 4, 0, 0, false},
        Card{114, -1, Card::hand, Card::creature, 7, 7, 7, bitset<6>("000100"), 0, 0, 0, false},
        Card{115, -1, Card::hand, Card::creature, 8, 5, 5, bitset<6>("000101"), 0, 0, 0, false},
        Card{116, -1, Card::hand, Card::creature, 12, 8, 8, bitset<6>("111111"), 0, 0, 0, false},
        Card{117, -1, Card::hand, Card::greenItem, 1, 1, 1, bitset<6>("100000"), 0, 0, 0, false},
        Card{118, -1, Card::hand, Card::greenItem, 0, 0, 3, bitset<6>("000000"), 0, 0, 0, false},
        Card{119, -1, Card::hand, Card::greenItem, 1, 1, 2, bitset<6>("000000"), 0, 0, 0, false},
        Card{120, -1, Card::hand, Card::greenItem, 2, 1, 0, bitset<6>("000010"), 0, 0, 0, false},
        Card{121, -1, Card::hand, Card::greenItem, 2, 0, 3, bitset<6>("000000"), 0, 0, 1, false},
        Card{122, -1, Card::hand, Card::greenItem, 2, 1, 3, bitset<6>("000100"), 0, 0, 0, false},
        Card{123, -1, Card::hand, Card::greenItem, 2, 4, 0, bitset<6>("000000"), 0, 0, 0, false},
        Card{124, -1, Card::hand, Card::greenItem, 3, 2, 1, bitset<6>("001000"), 0, 0, 0, false},
        Card{125, -1, Card::hand, Card::greenItem, 3, 1, 4, bitset<6>("000000"), 0, 0, 0, false},
        Card{126, -1, Card::hand, Card::greenItem, 3, 2, 3, bitset<6>("000000"), 0, 0, 0, false},
        Card{127, -1, Card::hand, Card::greenItem, 3, 0, 6, bitset<6>("000000"), 0, 0, 0, false},
        Card{128, -1, Card::hand, Card::greenItem, 4, 4, 3, bitset<6>("000000"), 0, 0, 0, false},
        Card{129, -1, Card::hand, Card::greenItem, 4, 2, 5, bitset<6>("000000"), 0, 0, 0, false},
        Card{130, -1, Card::hand, Card::greenItem, 4, 0, 6, bitset<6>("000000"), 4, 0, 0, false},
        Card{131, -1, Card::hand, Card::greenItem, 4, 4, 1, bitset<6>("000000"), 0, 0, 0, false},
        Card{132, -1, Card::hand, Card::greenItem, 5, 3, 3, bitset<6>("100000"), 0, 0, 0, false},
        Card{133, -1, Card::hand, Card::greenItem, 5, 4, 0, bitset<6>("000001"), 0, 0, 0, false},
        Card{134, -1, Card::hand, Card::greenItem, 4, 2, 2, bitset<6>("000000"), 0, 0, 1, false},
        Card{135, -1, Card::hand, Card::greenItem, 6, 5, 5, bitset<6>("000000"), 0, 0, 0, false},
        Card{136, -1, Card::hand, Card::greenItem, 0, 1, 1, bitset<6>("000000"), 0, 0, 0, false},
        Card{137, -1, Card::hand, Card::greenItem, 2, 0, 0, bitset<6>("000001"), 0, 0, 0, false},
        Card{138, -1, Card::hand, Card::greenItem, 2, 0, 0, bitset<6>("000100"), 0, 0, 1, false},
        Card{139, -1, Card::hand, Card::greenItem, 4, 0, 0, bitset<6>("000011"), 0, 0, 0, false},
        Card{140, -1, Card::hand, Card::greenItem, 2, 0, 0, bitset<6>("010000"), 0, 0, 0, false},
        Card{141, -1, Card::hand, Card::redItem, 0, -1, -1, bitset<6>("000000"), 0, 0, 0, false},
        Card{142, -1, Card::hand, Card::redItem, 0, 0, 0, bitset<6>("111111"), 0, 0, 0, false},
        Card{143, -1, Card::hand, Card::redItem, 0, 0, 0, bitset<6>("000100"), 0, 0, 0, false},
        Card{144, -1, Card::hand, Card::redItem, 1, 0, -2, bitset<6>("000000"), 0, 0, 0, false},
        Card{145, -1, Card::hand, Card::redItem, 3, -2, -2, bitset<6>("000000"), 0, 0, 0, false},
        Card{146, -1, Card::hand, Card::redItem, 4, -2, -2, bitset<6>("000000"), 0, -2, 0, false},
        Card{147, -1, Card::hand, Card::redItem, 2, 0, -1, bitset<6>("000000"), 0, 0, 1, false},
        Card{148, -1, Card::hand, Card::redItem, 2, 0, -2, bitset<6>("111111"), 0, 0, 0, false},
        Card{149, -1, Card::hand, Card::redItem, 3, 0, 0, bitset<6>("111111"), 0, 0, 1, false},
        Card{150, -1, Card::hand, Card::redItem, 2, 0, -3, bitset<6>("000000"), 0, 0, 0, false},
        Card{151, -1, Card::hand, Card::redItem, 5, 0, -99, bitset<6>("111111"), 0, 0, 0, false},
        Card{152, -1, Card::hand, Card::redItem, 7, 0, -7, bitset<6>("000000"), 0, 0, 1, false},
        Card{153, -1, Card::hand, Card::blueItem, 2, 0, 0, bitset<6>("000000"), 5, 0, 0, false},
        Card{154, -1, Card::hand, Card::blueItem, 2, 0, 0, bitset<6>("000000"), 0, -2, 1, false},
        Card{155, -1, Card::hand, Card::blueItem, 3, 0, -3, bitset<6>("000000"), 0, -1, 0, false},
        Card{156, -1, Card::hand, Card::blueItem, 3, 0, 0, bitset<6>("000000"), 3, -3, 0, false},
        Card{157, -1, Card::hand, Card::blueItem, 3, 0, -1, bitset<6>("000000"), 1, 0, 1, false},
        Card{158, -1, Card::hand, Card::blueItem, 3, 0, -4, bitset<6>("000000"), 0, 0, 0, false},
        Card{159, -1, Card::hand, Card::blueItem, 4, 0, -3, bitset<6>("000000"), 3, 0, 0, false},
        Card{160, -1, Card::hand, Card::blueItem, 2, 0, 0, bitset<6>("000000"), 2, -2, 0, false}
};

const Card Card::notFound{
        0, 0, Card::hand, Card::creature, 0, 0, 0, bitset<6>(), 0, 0, 0, false
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

struct State;

struct Action {
    enum Type { pick, summon, attack, use, pass };
    Type type;
    int  doer;
    int  target = -1;

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
    Cards  cards;
    Player me;
    Player op;

    void clear() {
        cards.clear();
    }

    void readState();

    const bool isValid(const Action &) const;

    void act(const Action &);

    void act(const Actions &);

    const double fieldValue() const;

    void checkGuards();

    const int cardCount(Card::Location loc);
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
            return doer.location == Card::hand && doer.type == Card::creature &&
                   count_if(cards.begin(), cards.end(), [](const Card &c) { return c.location == Card::myField; }) < 6;
        } else {
            if (action.type == Action::attack) {
                if (doer.location != Card::myField || doer.sickness) { return false; }
                if (action.target == -1) { return !op.guarded; }
                const Card &target = cards.getById(action.target);
                return ((op.guarded && target.abilities[Card::guard]) || !op.guarded) &&
                       target != Card::notFound && target.location == Card::opField;
            } else if (action.type == Action::use) {
                if (action.target == -1) { return doer.type == Card::blueItem; }
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
        if (action.target == -1) {
            op.health -= doer.atk;
        } else {
            Card &enemy = cards.getById(action.target);
            if (doer.atk > 0) {
                if (enemy.abilities[Card::ward]) {
                    enemy.abilities[Card::ward] = false;
                } else {
                    enemy.def -= doer.abilities[Card::lethal] ? enemy.def : doer.atk;
                    me.health += doer.abilities[Card::drain] ? doer.atk : 0;
                }
            }
            if (enemy.atk > 0) {
                if (doer.abilities[Card::ward]) {
                    doer.abilities[Card::ward] = false;
                } else {
                    doer.def -= enemy.abilities[Card::lethal] ? doer.def : enemy.atk;
                    op.health += enemy.abilities[Card::drain] ? enemy.atk : 0;
                }
            }
            if (enemy.def < 0) {
                if (doer.abilities[Card::trample]) {
                    op.health += enemy.def;
                }
                swap(enemy, cards.back());
                cards.pop_back();
            }
            if (doer.def < 0) {
                swap(doer, cards.back());
                cards.pop_back();
            }
            checkGuards();
        }
        doer.sickness = true;
    } else if (action.type == Action::summon || action.type == Action::use) {
        me.mana -= doer.cost;
        me.health += doer.myhc;
        op.health += doer.ophc;
        me.draw += doer.draw;
        if (action.type == Action::summon) {
            doer.location = Card::myField;
            doer.sickness = !doer.abilities[Card::charge];
            doer.myhc     = doer.ophc = doer.draw = 0;
            checkGuards();
        } else if (action.type == Action::use) {
            if (action.target == -1) {
                op.health += doer.def;
            } else if (action.target != -1) {
                Card &target = cards.getById(action.target);
                if (doer.type == Card::greenItem) {
                    bool wasCharge = target.abilities[Card::charge];
                    target.abilities |= doer.abilities;
                    if (!wasCharge && target.abilities[Card::charge]) {
                        target.sickness = false;
                    }
                } else if (doer.type == Card::redItem) {
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
    me.draw = min(8 - cardCount(Card::hand), me.draw);
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
    int opponentHand;
    cin >> opponentHand;
    cin.ignore();
    int cardCount;
    cin >> cardCount;
    cin.ignore();
    for (int i = 0; i < cardCount; i++) {
        Card   card;
        int    location;
        int    type;
        string abilities;
        cin >> card.number >> card.id >> location >> type >> card.cost >> card.atk >> card.def
            >> abilities >> card.myhc >> card.ophc >> card.draw;
        if (me.mana == 0) {
            card.id = i;
        }
        cin.ignore();
        card.location = (Card::Location) location;
        card.type     = (Card::Type) type;
        card.sickness = location == Card::hand;
        if (card.location == Card::hand) {
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
    double          result   = 0;
    int             opAttack = 0;
    for (const Card &card: cards) {
        if (card.location == Card::hand && card.type != Card::creature) {
            result += bias;
        } else if (card.location == Card::myField) {
            result += card.value();
            if (card.abilities[Card::guard]) {
                opAttack -= card.def;
            }
        } else if (card.location == Card::opField) {
            result -= card.value();
            opAttack += card.atk;
        }
    }
    result += (double) me.health / 4.0f - (double) op.health / 2.0f;
    if (op.health <= 0) { return result + 10000; }
    else if (me.health <= opAttack) { return result - 10000; }
    return result;
}

void State::checkGuards() {
    me.guarded = op.guarded = false;
    for (Card &card : cards) {
        if (card.location == Card::myField) {
            me.guarded |= card.abilities[Card::guard];
        } else if (card.location == Card::opField) {
            op.guarded |= card.abilities[Card::guard];
        }
    }
}

const int State::cardCount(const Card::Location loc) {
    int             result = 0;
    for (const Card &card : cards) {
        if (card.location == loc) {
            result += 1;
        }
    }
    return result;
}

Actions findAllActions(const State &state) {
    Actions         result;
    for (const Card &card: state.cards) {
        if (card.location == Card::hand && card.cost <= state.me.mana) {
            if (card.type == Card::creature && state.isValid(summon(card))) {
                result.push_back(summon(card));
            } else if (card.type == Card::greenItem) {
                for (const Card &target: state.cards) {
                    if (target.location == Card::myField && state.isValid(use(card, target))) {
                        result.push_back(use(card, target));
                    }
                }
            } else if (card.type == Card::redItem) {
                for (const Card &target: state.cards) {
                    if (target.location == Card::opField && state.isValid(use(card, target))) {
                        result.push_back(use(card, target));
                    }
                }
            } else if (card.type == Card::blueItem) {
                if (state.isValid(use(card))) {
                    result.push_back(use(card));
                }
                for (const Card &target: state.cards) {
                    if (target.location == Card::opField && state.isValid(use(card, target))) {
                        result.push_back(use(card, target));
                    }
                }
            }
        } else if (card.location == Card::myField) {
            if (state.isValid(attack(card))) {
                result.push_back(attack(card));
            }
            for (const Card &target: state.cards) {
                if (target.location == Card::opField && state.isValid(attack(card, target))) {
                    result.push_back(attack(card, target));
                }
            }
        }
    }
    result.push_back(pass());
    return result;
}

Action highestValueAction(const State &state) {
    Actions     toTry     = findAllActions(state);
    Action      result;
    double      bestValue = -10000000;
    for (Action &action : toTry) {
        State s(state);
        s.act(action);
        double val = s.fieldValue();
        if (val > bestValue) {
            bestValue = val;
            result    = action;
        }
    }
    return result;
}

Action findBestAction(const State &state) {
    Actions     toTry     = findAllActions(state);
    Action      result;
    double      bestValue = -10000000;
    for (Action &action : toTry) {
        State s(state);
        s.act(action);
        s.act(highestValueAction(s));
        s.act(highestValueAction(s));
        s.act(highestValueAction(s));
        double val = s.fieldValue();
        if (val > bestValue) {
            bestValue = val;
            result    = action;
        }
    }
    return result;
}

Actions findBestActions(const State &state) {
    Actions result;
    State   s(state);
    result.push_back(findBestAction(state));
    while (result.back().type != Action::pass) {
        s.act(result.back());
        result.push_back(findBestAction(s));
    }
    return result;
}

struct Agent {
    Cards deck;
    State state;
    int   mana = 0;

    array<int, 160> scores = {};
};

struct Game {
    array<Card, 60> pickable;
    Agent           murder;
    Agent           killer;
    bool            murderTurn = false;
    //bool            drafting   = true;
    int             nextId     = 0;

    random_device rd;
    mt19937 mt;

    Game() {
        mt.seed(rd());
        array<Card, 160> picks(pool);
        shuffle(picks.begin(), picks.end(), mt);
        for (int i                   = 0; i < pickable.size(); ++i) {
            pickable[i] = picks[i];
        }
        murder.deck.reserve(30);
        killer.deck.reserve(30);
        uniform_int_distribution<> dis(0, 2);
        for (int                   i = 0; i < 30; ++i) {
            murder.deck.push_back(pickable[dis(mt)]);
            killer.deck.push_back(pickable[dis(mt)]);
            shuffle(pickable.begin(), pickable.end(), mt);
        }
        for (int                   i = 29; i >= 0; --i) {
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
        Agent &next    = murderTurn ? killer : murder;
        sort(current.state.cards.begin(), current.state.cards.end());
        sort(next.state.cards.begin(), next.state.cards.end());
        Cards           nextCards;
        for (const Card &card : next.state.cards) {
            if (card.location == Card::hand) {
                nextCards.push_back(card);
            }
        }
        for (const Card &card : current.state.cards) {
            if (card.location != Card::hand) {
                Card tmp(card);
                tmp.location = card.location == Card::myField ? Card::opField : Card::myField;
                tmp.sickness = false;
                tmp.myhc     = tmp.ophc = tmp.draw = 0;

                nextCards.push_back(tmp);
            }
        }
        next.state.cards   = nextCards;
        next.state.me      = current.state.op;
        next.state.op      = current.state.me;
        while (next.state.me.draw > 0) {
            if (next.deck.empty()) {
                next.state.me.health = min(next.state.me.health, --next.state.me.rune * 5);
            }
            if (next.state.cardCount(Card::hand) >= 8) { break; }
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
        updateScores(actions);
        for (const Action &action : actions) {
            act(action);
        }
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

    void updateScores(const Actions &actions) {
        Agent             &current = murderTurn ? murder : killer;
        for (const Action &action : actions) {
            if (action.type != Action::pass) {
                const Card &doer = current.state.cards.getById(action.doer);
                if (action.type == Action::summon) {
                    current.scores.at(doer.number - 1) += 1;
                } else if (action.type == Action::use) {
                    current.scores.at(doer.number - 1) += 1;
                } else if (action.type == Action::attack) {
                    //current.scores[doer.number-1] += 1;
                }
            }
        }
    }
};

array<int, 160> simulate() {
    array<int, 160> scores = {};
    for (int  iters = 0; iters < 10000; ++iters) {
        Game game;
        while (game.winner() == 0) {
            game.turn();
            auto bla = findBestActions(game.currentState());
            //cout << bla << endl;
            game.act(bla);\

        }
        for (int i     = 0; i < 160; ++i) {
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

    const bool testing = true;

    //Game            game;
    State           init;
    State           &state = init;
    vector<int>     curve{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

    if (testing) {
        array<int, 160> scores = {};
        vector<future<array<int,160>>> futures;
        futures.reserve(8);
        for (int i = 0; i < 8; ++i) {
            futures.push_back(async(launch::async, simulate));
        }
        for (int i = 0; i < 8; ++i) {
            auto res = futures[i].get();
            for (int i     = 0; i < 160; ++i) {
                scores[i] += res[i];
            }
        }
        for (auto &&e : scores) {
            cout << e << ",";
        }
        cout << endl;
    } else {
        state.readState();

        // game loop
        for (int iterations = 0; iterations < 100000; iterations++) {
            if (state.me.mana != 0) { // Battle phase
                cout << findBestActions(state);
            } else { // Draft Phase
                // Selects the most undervalued card according to card.value() with some curve adhesion.
                Card      &bestPick = state.cards[0];
                double    bestValue = -100000;
                for (Card &card : state.cards) {
                    double push = log((double) card.cost + 1.0);
                    if ((card.value() - card.cost - (curve[card.cost] / 2.0f)) * push > bestValue) {
                        bestValue = (card.value() - card.cost - curve[card.cost] / 2.0f) * push;
                        bestPick  = card;
                    }
                }
                int       cost      = bestPick.cost;
                curve[cost]++;
                cout << pick(bestPick);
            }
            cout << endl;
        }
    }
}