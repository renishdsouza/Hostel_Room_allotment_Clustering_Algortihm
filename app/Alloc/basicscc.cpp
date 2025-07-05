#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <algorithm>
#include <string>

using namespace std;

struct PodPreference {
    int user_id;
    string username;
    vector<string> preferences;
};

struct Room {
    int room_no;
    int user1_id;
    int user2_id;
};

struct Group {
    int room1;
    int room2;
};

vector<PodPreference> loadPodPreferences() {
    vector<PodPreference> pods;
    unordered_map<string, int> username_to_id;
    
    // First, create a mapping from username to user_id
    for (int i = 1; i <= 100; ++i) {
        username_to_id["User " + to_string(i)] = i;
    }
    
    for (int i = 1; i <= 6; ++i) {
        ifstream file("data/pod_" + to_string(i) + "_table.csv");
        if (!file.is_open()) {
            cerr << "Could not open pod_" << i << "_table.csv\n";
            continue;
        }
        string line;
        getline(file, line);  // skip header
        while (getline(file, line)) {
            stringstream ss(line);
            string val;
            string username, partner_name;

            getline(ss, val, ','); // pod_id
            getline(ss, username, ','); // username
            getline(ss, partner_name, ','); // partner_name

            // Convert username to user_id
            if (username_to_id.find(username) == username_to_id.end()) continue;
            int user_id = username_to_id[username];

            auto it = find_if(pods.begin(), pods.end(),
                                   [user_id](const PodPreference& p) { return p.user_id == user_id; });
            if (it != pods.end()) {
                it->preferences.push_back(partner_name);
            } else {
                PodPreference pp;
                pp.user_id = user_id;
                pp.username = username;
                pp.preferences.push_back(partner_name);
                pods.push_back(pp);
            }
        }
    }
    return pods;
}

void allocateRoomsAndGroups(const vector<PodPreference>& pods) {
    unordered_set<int> allocated;
    vector<Room> rooms;
    int room_no = 1;
    
    // Create mapping from username to user_id
    unordered_map<string, int> username_to_id;
    for (const auto& p : pods) {
        username_to_id[p.username] = p.user_id;
    }

    for (const auto& p : pods) {
        if (allocated.count(p.user_id)) continue;

        bool paired = false;
        for (const string& pref_name : p.preferences) {
            // Convert preference name to user_id
            if (username_to_id.find(pref_name) == username_to_id.end()) continue;
            int pref_id = username_to_id[pref_name];
            
            if (allocated.count(pref_id)) continue;

            auto it = find_if(pods.begin(), pods.end(),
                                   [pref_id](const PodPreference& p2) { return p2.user_id == pref_id; });
            if (it != pods.end()) {
                if (find(it->preferences.begin(), it->preferences.end(), p.username) != it->preferences.end()) {
                    rooms.push_back({room_no++, p.user_id, pref_id});
                    allocated.insert(p.user_id);
                    allocated.insert(pref_id);
                    paired = true;
                    break;
                }
            }
        }
    }

    vector<int> leftover;
    for (const auto& p : pods) {
        if (!allocated.count(p.user_id))
            leftover.push_back(p.user_id);
    }

    for (size_t i = 0; i + 1 < leftover.size(); i += 2) {
        rooms.push_back({room_no++, leftover[i], leftover[i+1]});
        allocated.insert(leftover[i]);
        allocated.insert(leftover[i+1]);
    }

    vector<Group> groups;
    for (size_t i = 0; i + 1 < rooms.size(); i += 2) {
        groups.push_back({rooms[i].room_no, rooms[i+1].room_no});
    }

    ofstream out("data/rooms.csv");
    out << "room_no,user1_id,user2_id,group_no\n";
    int group_no = 1;
    for (const auto& g : groups) {
        for (const auto& r : rooms) {
            if (r.room_no == g.room1 || r.room_no == g.room2) {
                out << r.room_no << "," << r.user1_id << "," << r.user2_id << "," << group_no << "\n";
            }
        }
        ++group_no;
    }

    cout << "Allocation complete: data/rooms.csv\n";
}

int main() {
    auto pods = loadPodPreferences();
    allocateRoomsAndGroups(pods);
    return 0;
}
