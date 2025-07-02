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
    room_mate VARCHAR(9) NOT NULL,
    floor floor_type NOT NULL,
    lift proximity_type NOT NULL,
    mess proximity_type NOT NULL,
    window_p window_type NOT NULL,
    FOREIGN KEY (user_id) REFERENCES "user" (user_id) ON DELETE CASCADE
);

CREATE TABLE pod_1_table (
    pod_1_id SERIAL PRIMARY KEY,
    user_id INT UNIQUE NOT NULL,
    pod_1_preference VARCHAR(9) NOT NULL
);
CREATE TABLE pod_2_table (
    pod_2_id SERIAL PRIMARY KEY,
    user_id INT UNIQUE NOT NULL,
    pod_2_preference VARCHAR(9) NOT NULL
);
CREATE TABLE pod_3_table (
    pod_3_id SERIAL PRIMARY KEY,
    user_id INT UNIQUE NOT NULL,
    pod_3_preference VARCHAR(9) NOT NULL
);
CREATE TABLE pod_4_table (
    pod_4_id SERIAL PRIMARY KEY,
    user_id INT UNIQUE NOT NULL,
    pod_4_preference VARCHAR(9) NOT NULL
);
CREATE TABLE pod_5_table (
    pod_5_id SERIAL PRIMARY KEY,
    user_id INT UNIQUE NOT NULL,
    pod_5_preference VARCHAR(9) NOT NULL
);
CREATE TABLE pod_6_table (
    pod_6_id SERIAL PRIMARY KEY,
    user_id INT UNIQUE NOT NULL,
    pod_6_preference VARCHAR(9) NOT NULL
);

INSERT INTO "user" (username,password)
VALUES
('Renish Dsouza','B231198CS'),
('Aarav Patel', 'B112367CS'),
('Ishita Sharma', 'B176521CS'),
('Mohan Kumar', 'B981543CS'),
('Sneha Reddy', 'B236178CS'),
('Rahul Verma', 'B347819CS'),
('Neha Singh', 'B451690CS'),
('Karan Mehta', 'B561901CS'),
('Priya Jain', 'B67812CS'),
('Riya Gupta', 'B7890131CS'),
('Ananya Rao', 'B890341CS'),
('Arjun Nair', 'B901145CS'),
('Meera Joshi', 'B111334CS'),
('Amit Das', 'B223234CS'),
('Divya Sen', 'B334456CS'),
('Vikram Bhat', 'B444667CS'),
('Nisha Iyer', 'B556728CS'),
('Akash Roy', 'B667579CS'),
('Ritu Agarwal', 'B858990CS'),
('Harsh Pandey', 'B884001CS'),
('Alok Singh', 'B990052CS'),
('Kavya Menon', 'B101231CS'),
('Tarun Yadav', 'B121421CS'),
('Reena Pillai', 'B131651CS'),
('Siddharth S.', 'B141761CS'),
('Pooja B.', 'B151771CS'),
('Ishan K.', 'B161881CS'),
('Swati J.', 'B171941CS'),
('Gaurav T.', 'B182401CS'),
('Deepak R.', 'B192701CS'),
('Manish M.', 'B202251CS'),
('Rashmi C.', 'B212351CS'),
('Suresh D.', 'B222471CS'),
('Bhavna E.', 'B232581CS'),
('Tejas G.', 'B242691CS'),
('Nidhi H.', 'B252741CS'),
('Sameer I.', 'B266281CS'),
('Shruti J.', 'B274291CS'),
('Yash K.', 'B282961CS'),
('Asha L.', 'B292181CS'),
('Raj M.', 'B303117CS'),
('Nikhil N.', 'B315331CS'),
('Komal O.', 'B323471CS'),
('Vivek P.', 'B333591CS'),
('Aarti Q.', 'B343641CS'),
('Nitin R.', 'B353761CS'),
('Mehul S.', 'B363891CS'),
('Varsha T.', 'B373941CS'),
('Dipti U.', 'B383013CS');
