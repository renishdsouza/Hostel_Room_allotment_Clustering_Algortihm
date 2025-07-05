#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <algorithm>
#include <string>
#include <map>

using namespace std;

struct Student {
    int user_id;
    string username;
    string gender;
    string degree;
    string branch;
    string study_year;
    string floor;
    string lift;
    string mess;
    string window_p;
};

struct PodPreference {
    int user_id;
    string username;
    vector<string> preferences; // ordered by priority
};

struct Room {
    int room_no;
    int user1_id;
    int user2_id;
    string user1_name;
    string user2_name;
};

struct Group {
    int group_no;
    int room1;
    int room2;
};

class AllocationSystem {
private:
    vector<Student> students;
    vector<PodPreference> pod_preferences;
    unordered_map<string, int> username_to_id;
    unordered_map<int, string> id_to_username;
    vector<Room> rooms;
    vector<Group> groups;
    
public:
    void loadStudentData() {
        ifstream file("data/choices.csv");
        if (!file.is_open()) {
            cerr << "Could not open choices.csv\n";
            return;
        }
        
        string line;
        getline(file, line); // skip header
        
        while (getline(file, line)) {
            stringstream ss(line);
            string val;
            Student s;
            
            getline(ss, val, ','); // choice_id
            getline(ss, val, ','); s.user_id = stoi(val);
            getline(ss, val, ','); s.gender = val;
            getline(ss, val, ','); s.degree = val;
            getline(ss, val, ','); s.branch = val;
            getline(ss, val, ','); s.study_year = val;
            getline(ss, val, ','); s.floor = val;
            getline(ss, val, ','); s.lift = val;
            getline(ss, val, ','); s.mess = val;
            getline(ss, val, ','); s.window_p = val;
            
            s.username = "User " + to_string(s.user_id);
            students.push_back(s);
            username_to_id[s.username] = s.user_id;
            id_to_username[s.user_id] = s.username;
        }
    }
    
    void loadPodPreferences() {
        for (int i = 1; i <= 6; ++i) {
            ifstream file("data/pod_" + to_string(i) + "_table.csv");
            if (!file.is_open()) {
                cerr << "Could not open pod_" << i << "_table.csv\n";
                continue;
            }
            
            string line;
            getline(file, line); // skip header
            
            while (getline(file, line)) {
                stringstream ss(line);
                string val;
                string username, preference;
                
                getline(ss, val, ','); // pod_id
                getline(ss, username, ',');
                getline(ss, preference, ',');
                
                if (username_to_id.find(username) == username_to_id.end()) continue;
                
                int user_id = username_to_id[username];
                
                auto it = find_if(pod_preferences.begin(), pod_preferences.end(),
                                [user_id](const PodPreference& p) { return p.user_id == user_id; });
                
                if (it != pod_preferences.end()) {
                    it->preferences.push_back(preference);
                } else {
                    PodPreference pp;
                    pp.user_id = user_id;
                    pp.username = username;
                    pp.preferences.push_back(preference);
                    pod_preferences.push_back(pp);
                }
            }
        }
    }
    
    void allocateRooms() {
        unordered_set<int> allocated;
        int room_no = 1;
        
        // First pass: Mutual preferences
        for (const auto& p : pod_preferences) {
            if (allocated.count(p.user_id)) continue;
            
            bool paired = false;
            for (const string& pref_name : p.preferences) {
                if (username_to_id.find(pref_name) == username_to_id.end()) continue;
                
                int pref_id = username_to_id[pref_name];
                if (allocated.count(pref_id)) continue;
                
                // Check if preference is mutual
                auto it = find_if(pod_preferences.begin(), pod_preferences.end(),
                                [pref_id](const PodPreference& p2) { return p2.user_id == pref_id; });
                
                if (it != pod_preferences.end()) {
                    if (find(it->preferences.begin(), it->preferences.end(), p.username) != it->preferences.end()) {
                        Room room;
                        room.room_no = room_no++;
                        room.user1_id = p.user_id;
                        room.user2_id = pref_id;
                        room.user1_name = p.username;
                        room.user2_name = pref_name;
                        rooms.push_back(room);
                        
                        allocated.insert(p.user_id);
                        allocated.insert(pref_id);
                        paired = true;
                        break;
                    }
                }
            }
        }
        
        // Second pass: One-way preferences
        for (const auto& p : pod_preferences) {
            if (allocated.count(p.user_id)) continue;
            
            bool paired = false;
            for (const string& pref_name : p.preferences) {
                if (username_to_id.find(pref_name) == username_to_id.end()) continue;
                
                int pref_id = username_to_id[pref_name];
                if (allocated.count(pref_id)) continue;
                
                Room room;
                room.room_no = room_no++;
                room.user1_id = p.user_id;
                room.user2_id = pref_id;
                room.user1_name = p.username;
                room.user2_name = pref_name;
                rooms.push_back(room);
                
                allocated.insert(p.user_id);
                allocated.insert(pref_id);
                paired = true;
                break;
            }
        }
        
        // Third pass: Random pairing for remaining students
        vector<int> leftover;
        for (const auto& p : pod_preferences) {
            if (!allocated.count(p.user_id))
                leftover.push_back(p.user_id);
        }
        
        for (size_t i = 0; i + 1 < leftover.size(); i += 2) {
            Room room;
            room.room_no = room_no++;
            room.user1_id = leftover[i];
            room.user2_id = leftover[i+1];
            room.user1_name = id_to_username[leftover[i]];
            room.user2_name = id_to_username[leftover[i+1]];
            rooms.push_back(room);
            
            allocated.insert(leftover[i]);
            allocated.insert(leftover[i+1]);
        }
    }
    
    void allocateGroups() {
        unordered_set<int> allocated_rooms;
        int group_no = 1;
        
        // First pass: Try to group rooms with mutual preferences
        for (size_t i = 0; i < rooms.size(); ++i) {
            if (allocated_rooms.count(rooms[i].room_no)) continue;
            
            bool grouped = false;
            for (size_t j = i + 1; j < rooms.size(); ++j) {
                if (allocated_rooms.count(rooms[j].room_no)) continue;
                
                // Check if any student in room i prefers any student in room j
                bool has_preference = false;
                for (const auto& p : pod_preferences) {
                    if (p.user_id == rooms[i].user1_id || p.user_id == rooms[i].user2_id) {
                        for (const string& pref : p.preferences) {
                            if (pref == rooms[j].user1_name || pref == rooms[j].user2_name) {
                                has_preference = true;
                                break;
                            }
                        }
                        if (has_preference) break;
                    }
                }
                
                if (has_preference) {
                    Group group;
                    group.group_no = group_no++;
                    group.room1 = rooms[i].room_no;
                    group.room2 = rooms[j].room_no;
                    groups.push_back(group);
                    
                    allocated_rooms.insert(rooms[i].room_no);
                    allocated_rooms.insert(rooms[j].room_no);
                    grouped = true;
                    break;
                }
            }
        }
        
        // Second pass: Random grouping for remaining rooms
        vector<int> leftover_rooms;
        for (const auto& room : rooms) {
            if (!allocated_rooms.count(room.room_no))
                leftover_rooms.push_back(room.room_no);
        }
        
        for (size_t i = 0; i + 1 < leftover_rooms.size(); i += 2) {
            Group group;
            group.group_no = group_no++;
            group.room1 = leftover_rooms[i];
            group.room2 = leftover_rooms[i+1];
            groups.push_back(group);
        }
    }
    
    void saveResults() {
        ofstream out("data/rooms.csv");
        out << "room_no,user1_id,user2_id,user1_name,user2_name,group_no\n";
        
        for (const auto& room : rooms) {
            int group_no = 0;
            for (const auto& group : groups) {
                if (group.room1 == room.room_no || group.room2 == room.room_no) {
                    group_no = group.group_no;
                    break;
                }
            }
            out << room.room_no << "," << room.user1_id << "," << room.user2_id 
                << "," << room.user1_name << "," << room.user2_name << "," << group_no << "\n";
        }
        
        cout << "Allocation complete: data/rooms.csv\n";
        cout << "Total rooms: " << rooms.size() << "\n";
        cout << "Total groups: " << groups.size() << "\n";
    }
    
    void run() {
        loadStudentData();
        loadPodPreferences();
        allocateRooms();
        allocateGroups();
        saveResults();
    }
};

int main() {
    AllocationSystem system;
    system.run();
    return 0;
}
