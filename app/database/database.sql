-- Drop existing if already exist
DROP TABLE IF EXISTS choices , pod_1_table, pod_2_table, pod_3_table, pod_4_table, pod_5_table, pod_6_table, "user" CASCADE;


-- 1) proximity to lift.
-- 2) proximity to exit 
-- 3) proximity to washroom
-- 4) proximity to mess.
-- 5) windows preference facing outside or inside
-- 6) floor 3 choices
-- 7) pod(6ppl) preference
-- 8) roomate preference
-- 9) student year of study
-- 10) branch 
-- 11) becharlors or Masters


-- Drop ENUM types if already exists
DROP TYPE IF EXISTS  gender_type, degree_type, branch_type, branch_type, study_year_type, floor_type ,proximity_type, window_type;

--Create ENUM types 
CREATE TYPE gender_type AS ENUM ('MALE', 'FEMALE');
CREATE TYPE degree_type AS ENUM ('Bachelors', 'Masters');
CREATE TYPE branch_type AS ENUM ('Computer Science', 'Electronics and Communication', 'Electrical', 'Mechanical', 'Civil', 'Chemical', 'Bio Technology', 'Production', 'Material Science', 'Engineering Physics', 'Architecture' );
CREATE TYPE study_year_type AS ENUM ('No Preference' ,'First' ,'Second' ,'Third' ,'Fourth' ,'Fifth' );
CREATE TYPE floor_type AS ENUM ('No Preference' ,'Basement' ,'Ground' ,'First' );
CREATE TYPE proximity_type AS ENUM ('No Preference' ,'very near' ,'near' ,'far' ,'very far' );
CREATE TYPE window_type AS ENUM ('No Preference' ,'inside facing' ,'outside facing' );

--User Table
CREATE TABLE "user" (
    user_id SERIAL PRIMARY KEY,
    username VARCHAR(15) UNIQUE NOT NULL,
    password VARCHAR(10) NOT NULL
);

--Choices table
CREATE TABLE choices (
    choice_id SERIAL PRIMARY KEY,
    user_id INT UNIQUE NOT NULL,
    gender gender_type NOT NULL,
    degree degree_type NOT NULL,
    branch branch_type NOT NULL,
    study_year study_year_type NOT NULL,
    floor floor_type NOT NULL,
    lift proximity_type NOT NULL,
    mess proximity_type NOT NULL,
    window_p window_type NOT NULL,
    FOREIGN KEY (user_id) REFERENCES "user" (user_id) ON DELETE CASCADE
);

CREATE TABLE pod_1_table (
    pod_1_id SERIAL PRIMARY KEY,
    username VARCHAR(15) UNIQUE NOT NULL,
    pod_1_preference VARCHAR(15) NOT NULL
);
CREATE TABLE pod_2_table (
    pod_2_id SERIAL PRIMARY KEY,
    username VARCHAR(15) UNIQUE NOT NULL,
    pod_2_preference VARCHAR(15) NOT NULL
);
CREATE TABLE pod_3_table (
    pod_3_id SERIAL PRIMARY KEY,
    username VARCHAR(15) UNIQUE NOT NULL,
    pod_3_preference VARCHAR(15) NOT NULL
);
CREATE TABLE pod_4_table (
    pod_4_id SERIAL PRIMARY KEY,
    username VARCHAR(15) UNIQUE NOT NULL,
    pod_4_preference VARCHAR(15) NOT NULL
);
CREATE TABLE pod_5_table (
    pod_5_id SERIAL PRIMARY KEY,
    username VARCHAR(15) UNIQUE NOT NULL,
    pod_5_preference VARCHAR(15) NOT NULL
);
CREATE TABLE pod_6_table (
    pod_6_id SERIAL PRIMARY KEY,
    username VARCHAR(15) UNIQUE NOT NULL,
    pod_6_preference VARCHAR(15) NOT NULL
);