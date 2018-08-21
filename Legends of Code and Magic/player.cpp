#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <bitset>

using namespace std;

const float weights[] = {
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
const float bias      = 0.8940729693697819f;

struct Player {
    int  health;
    int  mana;
    int  deck;
    int  rune;
    int  draw = 1;
    bool guarded;

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

    int       number;
    int       id;
    Location  location;
    Type      type;
    int       cost;
    int       atk;
    int       def;
    bitset<6> abilities;
    int       myhc;
    int       ophc;
    int       draw;
    bool      sickness;

    friend ostream &operator<<(ostream &os, const Card &card);

    const static Card notFound;

    bool operator==(const Card &rhs) const;

    bool operator!=(const Card &rhs) const;

    const float value() const {
        float result = weights[type];
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

bool Cards::exists(const int id) const {
    return any_of(begin(), end(), [id](Card c) { return c.id == id; });
}

Card &Cards::getById(int id) {
    for (Card &card: *this) {
        if (card.id == id) {
            return card;
        }
    }
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

    void act(const Action &); // returns a success boolean

    const float fieldValue() const;

    void checkGuards();
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
                return target.def > 0 && ((op.guarded && target.abilities[Card::guard]) || !op.guarded) &&
                       target != Card::notFound && target.location == Card::opField;
            } else if (action.type == Action::use) {
                if (action.target == -1) { return doer.type == Card::blueItem; }
                const Card &target = cards.getById(action.target);
                return target != Card::notFound && target.def > 0;
            }
        }
    }
    return true;
}

void State::act(const Action &action) {
    Card &doer = cards.getById(action.doer);
    if (action.type == Action::attack) {
        if (action.target == -1) {
            op.health -= doer.atk;
        } else {
            Card &enemy = cards.getById(action.target);
            if (enemy.abilities[Card::ward]) {
                enemy.abilities[Card::ward] = false;
            } else {
                enemy.def -= doer.abilities[Card::lethal] && doer.atk > 0 ? enemy.def : doer.atk;
            }
            if (doer.abilities[Card::ward]) {
                doer.abilities[Card::ward] = false;
            } else {
                doer.def -= enemy.abilities[Card::lethal] && enemy.atk > 0 ? doer.def : enemy.atk;
            }
            if (enemy.def < 0 && doer.abilities[Card::trample]) {
                op.health += enemy.def;
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
            doer.myhc = doer.ophc = doer.draw = 0;
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
                checkGuards();
            }
            swap(doer, cards.back());
            cards.pop_back();
        }
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

const float State::fieldValue() const {
    float           result = 0;
    for (const Card &c: cards) {
        if (c.location != Card::hand && c.def > 0) {
            if (c.location == Card::myField) {
                result += c.value();
            } else {
                result -= c.value();
            }
        }
    }
    return result + (float) me.health / 8.0f - (float) op.health / 2.0f;
}

void State::checkGuards() {
    me.guarded = op.guarded = false;
    for (Card &card : cards) {
        if (card.location == Card::myField && card.def > 0) {
            me.guarded |= card.abilities[Card::guard];
        } else if (card.location == Card::opField && card.def > 0) {
            op.guarded |= card.abilities[Card::guard];
        }
    }
}

Actions findAllHandActions(const State&state) {

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
    if (result.empty()) {
        result.push_back(pass());
    }
    return result;
}

Action highestValueAction(const State &state) {
    Actions     toTry     = findAllActions(state);
    Action      result;
    float       bestValue = -10000000;
    for (Action &action : toTry) {
        State s(state);
        s.act(action);
        float val = s.fieldValue();
        if (val > bestValue) {
            bestValue = val;
            result    = action;
        }
    }
    return result;
}

Action findBestAction(const State &state) {
    Actions toTry         = findAllActions(state);
    cerr << toTry.size() << " possibilities" << endl;
    Action      result;
    float       bestValue = -10000000;
    for (Action &action : toTry) {
        State s(state);
        s.act(action);
        s.act(highestValueAction(s));
        s.act(highestValueAction(s));
        s.act(highestValueAction(s));
        float val = s.fieldValue();
        if (val > bestValue) {
            bestValue = val;
            result    = action;
        }
    }
    return result;
}

int main() {

    State       state;
    Player      player;
    Player      opponent;
    vector<int> curve{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

    // game loop
    for (int iterations = 0; iterations < 100000; iterations++) {
        state.readState();

        if (state.me.mana != 0) { // Battle phase

            Action action = findBestAction(state);
            while (action.type != Action::pass) {
                state.act(action);
                cout << action;
                action = findBestAction(state);
            }
            cout << action;

        } else { // Draft Phase
            // Selects the most undervalued card according to card.value() with some curve adhesion.
            Card      &bestPick = state.cards[1];
            float     bestValue = 100;
            for (Card &card : state.cards) {
                int cost = min(card.cost, 70);
//                if ((card.value() - cost - (curve[cost] / 1.2f)) < bestValue) {
//                bestValue = card.value() - card.cost - curve[cost];
                if ((cost) > bestValue) {
                    bestValue = cost;
                    bestPick  = card;
                }
            }
            int       cost      = min(bestPick.cost, 7);
            curve[cost]++;
            cout << pick(bestPick);
        }
        cout << endl;
    }
}