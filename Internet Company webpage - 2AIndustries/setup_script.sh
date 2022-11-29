#!/bin/sh

read -p "Enter your MariaDB username: " username
read -p "Enter your MariaDB password (you will be prompted for this again): " password

# set up order database
echo "Setting up order database..."
cd order_DB
mariadb -h courses -u $username -p $username < ./order_DB_script.sql
cd -
echo "Done."

# set up product database
echo "Setting up product database..."
cd product_DB

# define the table
mariadb -h courses -u $username -p $username < ./product_DB_script.sql

# for cpp program
echo $username > credentials
echo $password >> credentials

# run cpp program to fill the product database
make
./inventory
make clean
rm credentials
cd -
echo "Done."

# set up bracket database
echo "Setting up bracket database"
cd ./bracket_db
mariadb -h courses -u $username -p $username < ./shipping_DB_script.sql
cd -
echo "Done."

# set up login database
echo "Setting up login database..."
cd ./login_db
mariadb -h courses -u $username -p $username < ./login_db_script.sql 
cd -
echo "Done."

echo "All databases setup."
