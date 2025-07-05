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
    vector<string> preferences;
};

struct Room {
    int room_no;
    int user1_id;
    int user2_id;
    string user1_name;
    string user2_name;
    double x_coord;
    double y_coord;
};

struct Group {
    int group_no;
    int room1;
    int room2;
    double proximity_score;
};

class WeightedAllocationSystem {
private:
    vector<Student> students;
    vector<PodPreference> pod_preferences;
    unordered_map<string, int> username_to_id;
    unordered_map<int, string> id_to_username;
    vector<Room> rooms;
    vector<Group> groups;
    
    // Room coordinate system (simulate hostel layout)
    map<int, pair<double, double>> room_coordinates;
    
    void initializeRoomCoordinates() {
        // Simulate a hostel layout: 10 rooms per floor, 3 floors
        for (int floor = 0; floor < 3; ++floor) {
            for (int room_on_floor = 0; room_on_floor < 10; ++room_on_floor) {
                int room_no = floor * 10 + room_on_floor + 1;
                double x = room_on_floor * 5.0; // 5 meters between rooms
                double y = floor * 10.0; // 10 meters between floors
                room_coordinates[room_no] = {x, y};
            }
        }
    }
    
    double calculateDistance(int room1, int room2) {
        auto coord1 = room_coordinates[room1];
        auto coord2 = room_coordinates[room2];
        return sqrt(pow(coord1.first - coord2.first, 2) + pow(coord1.second - coord2.second, 2));
    }
    
    double calculatePreferenceScore(const Room& room1, const Room& room2) {
        double score = 0.0;
        
        // Check preferences between room1 and room2 students
        for (const auto& p : pod_preferences) {
            if (p.user_id == room1.user1_id || p.user_id == room1.user2_id) {
                for (size_t i = 0; i < p.preferences.size(); ++i) {
                    if (p.preferences[i] == room2.user1_name || p.preferences[i] == room2.user2_name) {
                        score += (6.0 - i) * 10.0; // Higher weight for higher preference
                    }
                }
            }
            if (p.user_id == room2.user1_id || p.user_id == room2.user2_id) {
                for (size_t i = 0; i < p.preferences.size(); ++i) {
                    if (p.preferences[i] == room1.user1_name || p.preferences[i] == room1.user2_name) {
                        score += (6.0 - i) * 10.0;
                    }
                }
            }
        }
        
        return score;
    }
    
    double calculateCompatibilityScore(const Room& room1, const Room& room2) {
        double score = 0.0;
        
        // Get student details
        Student* s1_1 = nullptr, *s1_2 = nullptr, *s2_1 = nullptr, *s2_2 = nullptr;
        for (auto& s : students) {
            if (s.user_id == room1.user1_id) s1_1 = &s;
            if (s.user_id == room1.user2_id) s1_2 = &s;
            if (s.user_id == room2.user1_id) s2_1 = &s;
            if (s.user_id == room2.user2_id) s2_2 = &s;
        }
        
        if (!s1_1 || !s1_2 || !s2_1 || !s2_2) return 0.0;
        
        // Same gender bonus
        if (s1_1->gender == s2_1->gender) score += 10.0;
        
        // Same branch bonus
        if (s1_1->branch == s2_1->branch) score += 5.0;
        
        // Same study year bonus
        if (s1_1->study_year == s2_1->study_year) score += 5.0;
        
        // Same floor preference bonus
        if (s1_1->floor == s2_1->floor) score += 15.0;
        
        // Same lift preference bonus
        if (s1_1->lift == s2_1->lift) score += 8.0;
        
        // Same mess preference bonus
        if (s1_1->mess == s2_1->mess) score += 12.0;
        
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
        unordered_set<int> allocated;
        int room_no = 1;
        
        // Enhanced room allocation with preference weights
        vector<pair<pair<int, int>, double>> potential_pairs;
        
        // Calculate scores for all possible pairs
        for (const auto& p1 : pod_preferences) {
            if (allocated.count(p1.user_id)) continue;
            
            for (const auto& p2 : pod_preferences) {
                if (allocated.count(p2.user_id) || p1.user_id == p2.user_id) continue;
                
                double score = 0.0;
                
                // Mutual preference bonus
                bool mutual = false;
                for (size_t i = 0; i < p1.preferences.size(); ++i) {
                    if (p1.preferences[i] == p2.username) {
                        score += (6.0 - i) * 20.0;
                        for (size_t j = 0; j < p2.preferences.size(); ++j) {
                            if (p2.preferences[j] == p1.username) {
                                score += (6.0 - j) * 20.0;
                                mutual = true;
                                break;
                            }
                        }
                        break;
                    }
                }
                
                if (mutual) score += 100.0; // Big bonus for mutual preference
                
                potential_pairs.push_back({{p1.user_id, p2.user_id}, score});
            }
        }
        
        // Sort by score descending
        sort(potential_pairs.begin(), potential_pairs.end(), 
             [](const auto& a, const auto& b) { return a.second > b.second; });
        
        // Allocate rooms based on sorted scores
        for (const auto& pair : potential_pairs) {
            int user1 = pair.first.first;
            int user2 = pair.first.second;
            
            if (allocated.count(user1) || allocated.count(user2)) continue;
            
            Room room;
            room.room_no = room_no++;
            room.user1_id = user1;
            room.user2_id = user2;
            room.user1_name = id_to_username[user1];
            room.user2_name = id_to_username[user2];
            rooms.push_back(room);
            
            allocated.insert(user1);
            allocated.insert(user2);
        }
        
        // Handle remaining students
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
        }
    }
    
    void allocateGroups() {
        initializeRoomCoordinates();
        
        // Assign coordinates to rooms
        for (auto& room : rooms) {
            if (room_coordinates.find(room.room_no) != room_coordinates.end()) {
                room.x_coord = room_coordinates[room.room_no].first;
                room.y_coord = room_coordinates[room.room_no].second;
            }
        }
        
        unordered_set<int> allocated_rooms;
        int group_no = 1;
        
        // Calculate all possible group combinations with scores
        vector<tuple<int, int, double>> group_scores;
        
        for (size_t i = 0; i < rooms.size(); ++i) {
            for (size_t j = i + 1; j < rooms.size(); ++j) {
                double distance = calculateDistance(rooms[i].room_no, rooms[j].room_no);
                double preference_score = calculatePreferenceScore(rooms[i], rooms[j]);
                double compatibility_score = calculateCompatibilityScore(rooms[i], rooms[j]);
                
                // Combined score: preference + compatibility - distance penalty
                double total_score = preference_score + compatibility_score - (distance * 0.5);
                
                group_scores.push_back({i, j, total_score});
            }
        }
        
        // Sort by score descending
        sort(group_scores.begin(), group_scores.end(),
             [](const auto& a, const auto& b) { return get<2>(a) > get<2>(b); });
        
        // Allocate groups based on scores
        for (const auto& score_tuple : group_scores) {
            int room1_idx = get<0>(score_tuple);
            int room2_idx = get<1>(score_tuple);
            
            if (allocated_rooms.count(rooms[room1_idx].room_no) || 
                allocated_rooms.count(rooms[room2_idx].room_no)) continue;
            
            Group group;
            group.group_no = group_no++;
            group.room1 = rooms[room1_idx].room_no;
            group.room2 = rooms[room2_idx].room_no;
            group.proximity_score = get<2>(score_tuple);
            groups.push_back(group);
            
            allocated_rooms.insert(rooms[room1_idx].room_no);
            allocated_rooms.insert(rooms[room2_idx].room_no);
        }
    }
    
    void saveResults() {
        ofstream out("data/weighted_rooms.csv");
        out << "room_no,user1_id,user2_id,user1_name,user2_name,group_no,x_coord,y_coord\n";
        
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
                << group_no << "," << room.x_coord << "," << room.y_coord << "\n";
        }
        
        // Save group details
        ofstream group_out("data/group_details.csv");
        group_out << "group_no,room1,room2,proximity_score,distance\n";
        for (const auto& group : groups) {
            double distance = calculateDistance(group.room1, group.room2);
            group_out << group.group_no << "," << group.room1 << "," << group.room2 
                      << "," << group.proximity_score << "," << distance << "\n";
        }
        
        cout << "Weighted allocation complete: data/weighted_rooms.csv\n";
        cout << "Group details saved: data/group_details.csv\n";
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
    WeightedAllocationSystem system;
    system.run();
    return 0;
}
