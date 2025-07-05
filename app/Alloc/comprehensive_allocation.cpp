#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <algorithm>
#include <string>
#include <map>
#include <cmath>
#include <queue>

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
    
    // Preference weights
    int lift_weight;
    int mess_weight;
    int floor_weight;
    int window_weight;
};

struct PodPreference {
    int user_id;
    string username;
    vector<string> preferences;
};

struct Room {
    int room_no;
    int user1_id;
    int user2_id;
    string user1_name;
    string user2_name;
    double compatibility_score;
    string assigned_floor;
    int lift_proximity;
    int mess_proximity;
    bool has_outside_window;
};

struct Group {
    int group_no;
    int room1;
    int room2;
    double overall_satisfaction;
    double preference_match;
    double compatibility_score;
};

class ComprehensiveAllocationSystem {
private:
    vector<Student> students;
    vector<PodPreference> pod_preferences;
    unordered_map<string, int> username_to_id;
    unordered_map<int, string> id_to_username;
    vector<Room> rooms;
    vector<Group> groups;
    
    // Preference value mappings
    map<string, int> proximity_values = {
        {"very near", 5}, {"near", 4}, {"No Preference", 3}, {"far", 2}, {"very far", 1}
    };
    
    map<string, int> floor_values = {
        {"Ground", 4}, {"First", 3}, {"Basement", 2}, {"No Preference", 1}
    };
    
    int getProximityValue(const string& proximity) {
        return proximity_values.count(proximity) ? proximity_values[proximity] : 3;
    }
    
    int getFloorValue(const string& floor) {
        return floor_values.count(floor) ? floor_values[floor] : 1;
    }
    
    double calculateStudentCompatibility(const Student& s1, const Student& s2) {
        double score = 0.0;
        
        // Gender compatibility (mandatory)
        if (s1.gender != s2.gender) return -1000.0; // Invalid pairing
        
        // Branch compatibility
        if (s1.branch == s2.branch) score += 20.0;
        else if (s1.branch.find("Engineering") != string::npos && 
                 s2.branch.find("Engineering") != string::npos) score += 10.0;
        
        // Study year compatibility
        if (s1.study_year == s2.study_year) score += 15.0;
        else {
            // Adjacent years get partial credit
            int year1 = (s1.study_year == "First") ? 1 : 
                       (s1.study_year == "Second") ? 2 :
                       (s1.study_year == "Third") ? 3 :
                       (s1.study_year == "Fourth") ? 4 : 5;
            int year2 = (s2.study_year == "First") ? 1 : 
                       (s2.study_year == "Second") ? 2 :
                       (s2.study_year == "Third") ? 3 :
                       (s2.study_year == "Fourth") ? 4 : 5;
            if (abs(year1 - year2) == 1) score += 8.0;
        }
        
        // Floor preference compatibility
        if (s1.floor == s2.floor) score += 25.0;
        else if (s1.floor == "No Preference" || s2.floor == "No Preference") score += 10.0;
        
        // Lift preference compatibility
        int lift_diff = abs(getProximityValue(s1.lift) - getProximityValue(s2.lift));
        score += max(0, 15 - lift_diff * 3);
        
        // Mess preference compatibility
        int mess_diff = abs(getProximityValue(s1.mess) - getProximityValue(s2.mess));
        score += max(0, 15 - mess_diff * 3);
        
        // Window preference compatibility
        if (s1.window_p == s2.window_p) score += 10.0;
        else if (s1.window_p == "No Preference" || s2.window_p == "No Preference") score += 5.0;
        
        // Degree compatibility
        if (s1.degree == s2.degree) score += 10.0;
        
        return score;
    }
    
    double calculatePreferenceScore(int user1, int user2) {
        double score = 0.0;
        string name1 = id_to_username[user1];
        string name2 = id_to_username[user2];
        
        // Check user1's preferences for user2
        for (const auto& p : pod_preferences) {
            if (p.user_id == user1) {
                for (size_t i = 0; i < p.preferences.size(); ++i) {
                    if (p.preferences[i] == name2) {
                        score += (6.0 - i) * 15.0; // Higher weight for higher preference
                        break;
                    }
                }
                break;
            }
        }
        
        // Check user2's preferences for user1
        for (const auto& p : pod_preferences) {
            if (p.user_id == user2) {
                for (size_t i = 0; i < p.preferences.size(); ++i) {
                    if (p.preferences[i] == name1) {
                        score += (6.0 - i) * 15.0;
                        // Mutual preference bonus
                        if (score > 15.0) score += 50.0;
                        break;
                    }
                }
                break;
            }
        }
        
        return score;
    }
    
    void assignRoomAttributes(Room& room) {
        // Find students
        Student* s1 = nullptr, *s2 = nullptr;
        for (auto& s : students) {
            if (s.user_id == room.user1_id) s1 = &s;
            if (s.user_id == room.user2_id) s2 = &s;
        }
        
        if (!s1 || !s2) return;
        
        // Assign floor based on preference
        if (s1->floor == s2->floor && s1->floor != "No Preference") {
            room.assigned_floor = s1->floor;
        } else if (s1->floor != "No Preference") {
            room.assigned_floor = s1->floor;
        } else if (s2->floor != "No Preference") {
            room.assigned_floor = s2->floor;
        } else {
            room.assigned_floor = "Ground"; // Default
        }
        
        // Assign lift proximity (average of preferences)
        room.lift_proximity = (getProximityValue(s1->lift) + getProximityValue(s2->lift)) / 2;
        
        // Assign mess proximity (average of preferences)
        room.mess_proximity = (getProximityValue(s1->mess) + getProximityValue(s2->mess)) / 2;
        
        // Assign window preference
        room.has_outside_window = (s1->window_p == "outside facing" || s2->window_p == "outside facing");
        
        // Calculate room compatibility score
        room.compatibility_score = calculateStudentCompatibility(*s1, *s2);
    }
    
    double calculateGroupCompatibility(const Room& r1, const Room& r2) {
        double score = 0.0;
        
        // Floor compatibility
        if (r1.assigned_floor == r2.assigned_floor) score += 30.0;
        else {
            // Adjacent floors get partial credit
            int floor1 = (r1.assigned_floor == "Ground") ? 1 : 
                        (r1.assigned_floor == "First") ? 2 : 0;
            int floor2 = (r2.assigned_floor == "Ground") ? 1 : 
                        (r2.assigned_floor == "First") ? 2 : 0;
            if (abs(floor1 - floor2) == 1) score += 15.0;
        }
        
        // Lift proximity compatibility
        int lift_diff = abs(r1.lift_proximity - r2.lift_proximity);
        score += max(0, 20 - lift_diff * 4);
        
        // Mess proximity compatibility
        int mess_diff = abs(r1.mess_proximity - r2.mess_proximity);
        score += max(0, 20 - mess_diff * 4);
        
        // Window preference compatibility
        if (r1.has_outside_window == r2.has_outside_window) score += 10.0;
        
        // Cross-room preferences
        vector<int> room1_users = {r1.user1_id, r1.user2_id};
        vector<int> room2_users = {r2.user1_id, r2.user2_id};
        
        for (int u1 : room1_users) {
            for (int u2 : room2_users) {
                score += calculatePreferenceScore(u1, u2) * 0.5; // Reduced weight for cross-room
            }
        }
        
        return score;
    }
    
public:
    void loadStudentData() {
        ifstream file("data/choices.csv");
        if (!file.is_open()) {
            cerr << "Could not open choices.csv\n";
            return;
        }
        
        string line;
        getline(file, line);
        
        while (getline(file, line)) {
            stringstream ss(line);
            string val;
            Student s;
            
            getline(ss, val, ',');
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
            s.lift_weight = getProximityValue(s.lift);
            s.mess_weight = getProximityValue(s.mess);
            s.floor_weight = getFloorValue(s.floor);
            s.window_weight = (s.window_p == "outside facing") ? 2 : 1;
            
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
            getline(file, line);
            
            while (getline(file, line)) {
                stringstream ss(line);
                string val;
                string username, preference;
                
                getline(ss, val, ',');
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
        // Advanced allocation considering all factors
        priority_queue<tuple<double, int, int>> pq; // score, user1, user2
        
        // Calculate comprehensive scores for all valid pairs
        for (const auto& s1 : students) {
            for (const auto& s2 : students) {
                if (s1.user_id >= s2.user_id) continue;
                
                double compatibility = calculateStudentCompatibility(s1, s2);
                if (compatibility < 0) continue; // Invalid pairing
                
                double preference = calculatePreferenceScore(s1.user_id, s2.user_id);
                double total_score = compatibility + preference;
                
                pq.push({total_score, s1.user_id, s2.user_id});
            }
        }
        
        unordered_set<int> allocated;
        int room_no = 1;
        
        // Allocate rooms based on comprehensive scores
        while (!pq.empty()) {
            auto [score, user1, user2] = pq.top();
            pq.pop();
            
            if (allocated.count(user1) || allocated.count(user2)) continue;
            
            Room room;
            room.room_no = room_no++;
            room.user1_id = user1;
            room.user2_id = user2;
            room.user1_name = id_to_username[user1];
            room.user2_name = id_to_username[user2];
            
            assignRoomAttributes(room);
            rooms.push_back(room);
            
            allocated.insert(user1);
            allocated.insert(user2);
        }
        
        // Handle remaining students with random valid pairing
        vector<int> leftover;
        for (const auto& s : students) {
            if (!allocated.count(s.user_id))
                leftover.push_back(s.user_id);
        }
        
        // Try to pair remaining students validly
        for (size_t i = 0; i < leftover.size(); ++i) {
            if (allocated.count(leftover[i])) continue;
            
            for (size_t j = i + 1; j < leftover.size(); ++j) {
                if (allocated.count(leftover[j])) continue;
                
                // Check if pairing is valid (same gender)
                Student* s1 = nullptr, *s2 = nullptr;
                for (auto& s : students) {
                    if (s.user_id == leftover[i]) s1 = &s;
                    if (s.user_id == leftover[j]) s2 = &s;
                }
                
                if (s1 && s2 && s1->gender == s2->gender) {
                    Room room;
                    room.room_no = room_no++;
                    room.user1_id = leftover[i];
                    room.user2_id = leftover[j];
                    room.user1_name = id_to_username[leftover[i]];
                    room.user2_name = id_to_username[leftover[j]];
                    
                    assignRoomAttributes(room);
                    rooms.push_back(room);
                    
                    allocated.insert(leftover[i]);
                    allocated.insert(leftover[j]);
                    break;
                }
            }
        }
    }
    
    void allocateGroups() {
        priority_queue<tuple<double, int, int>> pq; // score, room1_idx, room2_idx
        
        // Calculate comprehensive scores for all room pairs
        for (size_t i = 0; i < rooms.size(); ++i) {
            for (size_t j = i + 1; j < rooms.size(); ++j) {
                double compatibility = calculateGroupCompatibility(rooms[i], rooms[j]);
                pq.push({compatibility, i, j});
            }
        }
        
        unordered_set<int> allocated_rooms;
        int group_no = 1;
        
        // Allocate groups based on comprehensive scores
        while (!pq.empty()) {
            auto [score, room1_idx, room2_idx] = pq.top();
            pq.pop();
            
            if (allocated_rooms.count(room1_idx) || allocated_rooms.count(room2_idx)) continue;
            
            Group group;
            group.group_no = group_no++;
            group.room1 = rooms[room1_idx].room_no;
            group.room2 = rooms[room2_idx].room_no;
            group.overall_satisfaction = score;
            group.compatibility_score = calculateGroupCompatibility(rooms[room1_idx], rooms[room2_idx]);
            
            // Calculate preference match
            group.preference_match = 0.0;
            vector<int> room1_users = {rooms[room1_idx].user1_id, rooms[room1_idx].user2_id};
            vector<int> room2_users = {rooms[room2_idx].user1_id, rooms[room2_idx].user2_id};
            
            for (int u1 : room1_users) {
                for (int u2 : room2_users) {
                    group.preference_match += calculatePreferenceScore(u1, u2);
                }
            }
            
            groups.push_back(group);
            allocated_rooms.insert(room1_idx);
            allocated_rooms.insert(room2_idx);
        }
    }
    
    void saveResults() {
        ofstream out("data/comprehensive_rooms.csv");
        out << "room_no,user1_id,user2_id,user1_name,user2_name,group_no,compatibility_score,assigned_floor,lift_proximity,mess_proximity,has_outside_window\n";
        
        for (const auto& room : rooms) {
            int group_no = 0;
            for (const auto& group : groups) {
                if (group.room1 == room.room_no || group.room2 == room.room_no) {
                    group_no = group.group_no;
                    break;
                }
            }
            out << room.room_no << "," << room.user1_id << "," << room.user2_id 
                << "," << room.user1_name << "," << room.user2_name << "," 
                << group_no << "," << room.compatibility_score << "," << room.assigned_floor
                << "," << room.lift_proximity << "," << room.mess_proximity 
                << "," << (room.has_outside_window ? "yes" : "no") << "\n";
        }
        
        // Save detailed group analysis
        ofstream group_out("data/comprehensive_groups.csv");
        group_out << "group_no,room1,room2,overall_satisfaction,compatibility_score,preference_match\n";
        for (const auto& group : groups) {
            group_out << group.group_no << "," << group.room1 << "," << group.room2 
                      << "," << group.overall_satisfaction << "," << group.compatibility_score
                      << "," << group.preference_match << "\n";
        }
        
        cout << "Comprehensive allocation complete: data/comprehensive_rooms.csv\n";
        cout << "Detailed group analysis: data/comprehensive_groups.csv\n";
        cout << "Total rooms: " << rooms.size() << "\n";
        cout << "Total groups: " << groups.size() << "\n";
        
        // Calculate and display satisfaction metrics
        double total_room_satisfaction = 0.0;
        double total_group_satisfaction = 0.0;
        
        for (const auto& room : rooms) {
            total_room_satisfaction += room.compatibility_score;
        }
        
        for (const auto& group : groups) {
            total_group_satisfaction += group.overall_satisfaction;
        }
        
        cout << "Average room satisfaction: " << total_room_satisfaction / rooms.size() << "\n";
        cout << "Average group satisfaction: " << total_group_satisfaction / groups.size() << "\n";
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
    ComprehensiveAllocationSystem system;
    system.run();
    return 0;
}
