#include <iostream>
#include <string>
#include <vector>
#include <iomanip>
#include <fstream>
#include <mysql.h>
#include <cstdlib>

int main(int argc, const char** argv) {
	
	srand(467);

	// username and password
	std::string user;
	std::string pass;	

	std::fstream file("credentials", std::fstream::in);

	file >> user;
	file >> pass;
	
	file.close();

	std::cout << "initializing..." << std::endl;
	
	// initialize mysql lib
	int ret = mysql_library_init(0, nullptr, nullptr);
	if(ret != 0) { // error
		std::cerr << "Error initializing the library." << std::endl;
		exit(-1);
	}

	// initialize mysql object
	MYSQL *new_db = mysql_init(nullptr);
	if(!new_db) { // error
		std::cerr << mysql_error(new_db);
		exit(-1);
	}
	
	std::cout << "initialized new db." << std::endl;
	std::cout << "connecting to new db...";
	
	// connect to db
	MYSQL *conn_new = mysql_real_connect(new_db,
					 "courses",
					 user.c_str(),
					 pass.c_str(),
				         user.c_str(), 
					 0,
					 nullptr,
					 0);
	if(!conn_new) { // error
		std::cerr << mysql_error(conn_new) << std::endl;
		exit(-1);
	}
	std::cout << " connected to new db" << std::endl;
	std::cout << "connected to new db. using db " << user << "." << std::endl;

	std::cout << "creating entries in new product db." << std::endl;
	
	std::string p_id;

	std::fstream file2("ids.parts", std::fstream::in);
	// create new entry for each product id in the DB 
	while(file2 >> p_id) {
		
		std::string qty = std::to_string(rand() % 100);

		// prep the query 
		std::string query = "INSERT INTO Inventory \
					(id, qty) \
				     VALUES \
				     	("+p_id+", "+qty+");";
		
		// query to create rows
		ret = mysql_query(new_db, query.c_str());
		if(ret != 0) { // erro
			std::cerr << mysql_error(new_db) << std::endl;
			exit(-1);
		}

		else
			std::cout << "\r"
				  << "successfully inserted product " << p_id
				  << std::flush;
	}

	// make some room
	std::cout << std::endl;

	// clean up
	mysql_close(conn_new);
//	mysql_close(conn_legacy);
	mysql_library_end();

	// done
	std::cout << "end." << std::endl;
	return 0;
}
