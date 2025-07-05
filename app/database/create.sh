#Run this first to inititate your database

psql -U postgres -c "DROP DATABASE IF EXISTS hrc_db;"

# Create new DB
psql -U postgres -c "CREATE DATABASE hrc_db;"
#Run this first to inititate your database
 
psql -U postgres -d hrc_db -f database.sql
psql -U postgres -d hrc_db -f sampleusers.sql
