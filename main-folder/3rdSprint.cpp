#include <cassert>
#include <iostream>
#include <map>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

enum class QueryType {
    NewBus,
    BusesForStop,
    StopsForBus,
    AllBuses,
};

struct Query {
    QueryType type;
    string bus;
    string stop;
    vector<string> stops;
};

ostream& operator<<(ostream& os, const vector<string>& vec) {
    bool check = true;
    for (const string& str : vec) {
        if (check == true) {
            os << str;
            check = false;
        }
        else os << " "s << str;
    }
    return os;
}

istream& operator>>(istream& is, Query& q) {
    string query;
    is >> query;
    if (query == "NEW_BUS"s) {
        q.type = QueryType::NewBus;
        is >> q.bus;
        int stop_count;
        is >> stop_count;
        q.stops.resize(stop_count);
        for (string& stop : q.stops) {
            is >> stop;
        }
        return is;
    }
    else if (query == "BUSES_FOR_STOP"s) {
        q.type = QueryType::BusesForStop;
        is >> q.stop;
        return is;
    }
    else if (query == "STOPS_FOR_BUS"s) {
        q.type = QueryType::StopsForBus;
        is >> q.bus;
        return is;
    }
    else if (query == "ALL_BUSES"s) {
        q.type = QueryType::AllBuses;
        return is;
    }
    else {
        return is;
    }
}


struct BusesForStopResponse {
    string stop;
    vector<string> buses;
};

ostream& operator<<(ostream& os, const BusesForStopResponse& r) {
    if (r.buses.empty()) {
        os << "No stop"s;
        return os;
    }
    bool check = true;
    for (auto& element : r.buses) {
        if (check == true) {
            os << element;
            check = false;
        }
        else os << " "s << element;
    }
    return os;
}

struct StopsForBusResponse {
    string bus;
    vector<pair<string, vector<string>>> stops;
};

ostream& operator<<(ostream& os, const StopsForBusResponse& r) {
    if (r.stops.empty()) {
        os << "No bus"s;
        return os;
    }
    bool is_first = true;
    for (auto& [stop, stopss] : r.stops) {
        if (!is_first) {
            os << endl;
            os << "Stop "s << stop << ": "s;
            vector<string> buses;
            for (auto& bus : stopss) {
                if (bus != r.bus) {
                    buses.push_back(bus);
                }
            }
            if (!buses.empty()) {
                for (const string& word : buses) {
                    os << word << " "s;
                }
            }
            else os << "no interchange"s;
        }
        else {
            os << "Stop "s << stop << ": "s;
            vector<string> buses;
            for (auto& bus : stopss) {
                if (bus != r.bus) {
                    buses.push_back(bus);
                }
            }
            if (!buses.empty()) {
                for (const string& word : buses) {
                    os << word << " "s;
                }
            }
            else os << "no interchange"s;
        }
        is_first = false;
    }
    return os;
}

struct AllBusesResponse {
    map<string, vector<string>> buses;
};

ostream& operator<<(ostream& os, const AllBusesResponse& r) {
    if (r.buses.empty()) {
        cout << "No buses"s;
        return os;
    }
    bool check = true;
    for (const auto& [route, stops] : r.buses) {
        if (check == true) {
            os << "Bus "s << route << ": "s;
            check = false;
        }
        else os << endl << "Bus "s << route << ": "s;
        for (const auto& stop : stops) {
            os << stop << " "s;
        }
    }
    return os;
}

struct Route {
    string bus;
    vector<string> stops;
};

class BusManager {
public:
    void AddBus(const string& bus, const vector<string>& stops) {
        Route value_b;
        value_b.bus = bus;
        value_b.stops = stops;
        bus_info_.push_back(value_b);
        for (const string& word : stops) {
            stop_info_[word].push_back(bus);
        }
    }

    BusesForStopResponse GetBusesForStop(const string& stop) const {
        BusesForStopResponse answer;
        answer.stop = stop;
        if (stop_info_.count(stop) > 0) {
            answer.buses = stop_info_.at(stop);
        }
        return answer;
    }

    StopsForBusResponse GetStopsForBus(const string& bus) const {
        StopsForBusResponse answer;
        vector<string> stops;
        answer.bus = bus;
        for (const auto& [key, value] : bus_info_) {
            if (key == bus) {
                for (const string& word : value) {
                    stops.push_back(word);
                }
            }
        }
        for (const string& word : stops) {
            vector<string> stops_a;
            for (const string& buss : stop_info_.at(word)) {

                if (buss != bus) {
                    stops_a.push_back(buss);
                }
            }
            answer.stops.push_back(make_pair(word, stops_a));
        }
        return answer;
    }

    AllBusesResponse GetAllBuses() const {
        AllBusesResponse answer;
        if (bus_info_.empty()) {
            return answer;
        }
        for (const auto& [key, value] : bus_info_) {
            answer.buses.emplace(make_pair(key, value));
        }
        return answer;
    }

private:
    vector <Route> bus_info_;
    map <string, vector<string>> stop_info_;
};

int main() {
    int query_count;
    Query q;

    cin >> query_count;

    BusManager bm;
    for (int i = 0; i < query_count; ++i) {
        cin >> q;
        switch (q.type) {
        case QueryType::NewBus:
            bm.AddBus(q.bus, q.stops);
            break;
        case QueryType::BusesForStop:
            cout << bm.GetBusesForStop(q.stop) << endl;
            break;
        case QueryType::StopsForBus:
            cout << bm.GetStopsForBus(q.bus) << endl;
            break;
        case QueryType::AllBuses:
            cout << bm.GetAllBuses() << endl;
            break;
        }
    }
}
