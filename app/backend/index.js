import express from 'express';
import bodyParser from 'body-parser';//not needed try default does node stream parse
// import cors from 'cors'; //When using react diff port cross origin resource sharing
import pg from 'pg';
import dotenv from 'dotenv';
import path from 'path';
import { fileURLToPath } from 'url';

dotenv.config();
const __filename = fileURLToPath(import.meta.url);
const __dirname = path.dirname(__filename);
const app = express();
const port = 3000;

app.use(bodyParser.urlencoded({ extended: true }));//uses qs library obj in obj for []
app.use(express.json()); // For JSON data
app.use(express.static('../frontend/public'));
app.set("view engine", "ejs");
app.set("views", path.join(__dirname, "../frontend/views"));

const pool = new pg.Pool({ //default 100 connections
    user: process.env.DB_USER,
    host: process.env.DB_HOST,
    database: process.env.DB_NAME,
    password: process.env.DB_PASSWORD,
    port: process.env.DB_PORT,
});

pool.connect()
    .then(() => console.log("Connected to PostgreSQL"))
    .catch(err => console.error("Database connection error", err));

//homepage rendering
app.get('/', async (req, res) => {
    res.render("login.ejs");
});
//on all dashboard add a logout button
// Login Route
app.post("/login/submit", async (req, res) => {
    const { username, password, role } = req.body;

    try {
        const result = await pool.query(
            'SELECT * FROM "user" WHERE username = $1 AND password = $2',
            [username, password]
        );

        if (result.rows.length === 0) {
            return res.render("login.ejs", { status: "false" });
        }

        const user = result.rows[0];

        return res.render("choices.ejs", { userData: user });
    } catch (err) {
        console.error("Error during login:",err);
        res.status(500).send("Server error");
    }
});

app.post("/choices/submit",async (req,res)=>{
    let {userData,userId,username,gender,degree,branch,studyYear,roomMate,floor,lift,mess,window,pod1Preference,pod2Preference,pod3Preference,pod4Preference,pod5Preference,pod6Preference,}=req.body;

    try {
        userData = typeof
        userData === "string" ? JSON.parse(userData): userData;
    }
    catch (error) {
        console.error("Error parsing the entry:", error);
        return res.render("login.ejs",{
            userData:{},
            newEntryStatus: "false"
        });
    }

    const client = await pool.connect();
    try {
        await client.query("BEGIN");

        //insert into choices table
        const userInsertQuery='INSERT INTO choices (user_id,gender,degree,branch,study_year,room_mate,floor,lift,mess,window) VALUES ($1,$2,$3,$4,$5,$6,$7,$8,$9,$10)';

        await client.query(userInsertQuery,[
            userId,
            gender,
            degree,
            branch,
            studyYear,
            roomMate,
            floor,
            lift,
            mess,
            window
        ]);

        if(pod1Preference!=="No Preference"){
            const pod1Query='INSERT INTO pod_1_table (user_id,pod1Preference) VALUES ($1,$2)';

            await client.query(pod1Query,[
                userId,
                pod1Preference
            ]);
        }
        else if(pod2Preference!=="No Preference"){
            
            const pod2Query='INSERT INTO pod_2_table (user_id,pod2Preference) VALUES ($1,$2)';

            await client.query(pod2Query,[
                userId,
                pod2Preference
            ]);
        }
        else if(pod3Preference!=="No Preference"){
            
            const pod3Query='INSERT INTO pod_3_table (user_id,pod3Preference) VALUES ($1,$2)';

            await client.query(pod3Query,[
                userId,
                pod3Preference
            ]);
        }
        else if(pod4Preference!=="No Preference"){
            
            const pod4Query='INSERT INTO pod_4_table (user_id,pod4Preference) VALUES ($1,$2)';

            await client.query(pod4Query,[
                userId,
                pod4Preference
            ]);
        }
        else if(pod5Preference!=="No Preference"){
            
            const pod5Query='INSERT INTO pod_5_table (user_id,pod5Preference) VALUES ($1,$2)';

            await client.query(pod5Query,[
                userId,
                pod5Preference
            ]);
        }
        else if(pod6Preference!=="No Preference"){
            
            const pod6Query='INSERT INTO pod_6_table (user_id,pod6Preference) VALUES ($1,$2)';

            await client.query(pod6Query,[
                userId,
                pod6Preference
            ]);
        }

        await client.query("COMMIT");

        res.render("login.ejs", {
            userData: userData ||
            {}, newEntryStatus: "true"
        });

    }
    catch (error){
        await client.query("ROLLBACK");
        console.error("Transaction failed:",error);
        res.render("login.ejs",{
            userData:userData || {},
            newEntryStatus: "false"
        });
    }
    finally{
        client.release();
    }
});

// Start the server
app.listen(port, () => {
    console.log(`Server running on http://localhost:${port}`);
});