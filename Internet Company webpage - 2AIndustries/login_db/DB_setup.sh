#!/bin/sh

#
# Script to set up the order database
# !! Will delete all information in database
# !! If it exists
# # # # # # # # # # # # # # # # # # # # # # #


read -p "Enter mariadb username: " username

mariadb -h courses -u $username -p $username < login_db_script.sql

