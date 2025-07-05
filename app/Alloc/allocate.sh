#Run this to allocate rooms.
psql -d hrc_db -U postgres -c "\COPY choices TO './data/choices.csv' CSV HEADER"
psql -d hrc_db -U postgres -c "\COPY pod_1_table TO './data/pod_1_table.csv' CSV HEADER"
psql -d hrc_db -U postgres -c "\COPY pod_2_table TO './data/pod_2_table.csv' CSV HEADER"
psql -d hrc_db -U postgres -c "\COPY pod_3_table TO './data/pod_3_table.csv' CSV HEADER"
psql -d hrc_db -U postgres -c "\COPY pod_4_table TO './data/pod_4_table.csv' CSV HEADER"
psql -d hrc_db -U postgres -c "\COPY pod_5_table TO './data/pod_5_table.csv' CSV HEADER"
psql -d hrc_db -U postgres -c "\COPY pod_6_table TO './data/pod_6_table.csv' CSV HEADER"

echo "Data export complete. Running allocation algorithm..."
g++ basicscc.cpp -o basicscc -std=c++17
./basicscc
echo "Room allocation complete! Check data/rooms.csv for results."o allocate rooms.
psql -d hrc_db -U postgres -c "\COPY choices TO './data/choices.csv' CSV HEADER"
psql -d hrc_db -U postgres -c "\COPY pod_1_table TO './data/pod_1_table.csv' CSV HEADER"
psql -d hrc_db -U postgres -c "\COPY pod_2_table TO './data/pod_2_table.csv' CSV HEADER"
psql -d hrc_db -U postgres -c "\COPY pod_3_table TO './data/pod_3_table.csv' CSV HEADER"
psql -d hrc_db -U postgres -c "\COPY pod_4_table TO './data/pod_4_table.csv' CSV HEADER"
psql -d hrc_db -U postgres -c "\COPY pod_5_table TO './data/pod_5_table.csv' CSV HEADER"
psql -d hrc_db -U postgres -c "\COPY pod_6_table TO './data/pod_6_table.csv' CSV HEADER"

g++ basicscc.cpp
./a.out