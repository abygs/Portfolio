#!/bin/sh
#
# Script to set-up the product database
#

read -p "Enter mariadb username: " username
read -p "Enter mariadb password (you will be prompted for this again): " password

mariadb -h courses -u $username -p $username < product_DB_script.sql

echo $username > credentials
echo $password >> credentials

make

./inventory

make clean

rm credentials

echo "credentials file deleted."
echo "finished."
