#include "menus.h"
#include <string>
#include "sha256.h"
#include "bankaccount.h"
#include "json.hpp"
#include "transaction.h"
#include <fstream>
#include <filesystem>
#include <random>
#include <chrono>
#include <ctime>
#include <sstream>

using json = nlohmann::json;

namespace fs = std::filesystem;

//Printed when the user enters an invalid input/command.
void Menus::invalid_command() {
	system("cls");
	std::cout << "<<====== Invalid Command ======>>" << std::endl;

	//Fixes bug that creates a never ending loop when a non-int is entered.
	std::cin.clear();
	std::cin.ignore(100, '\n');
}

int Menus::find_space(std::string& string) {
	for (unsigned int i = 0; i < size(string); i++) {
		if (string[i] == ' ') {
			return i;
		}
	}
	return NULL;
}

void Menus::split(std::string& string, std::string& front, std::string& back, int& index) {
	for (int i = 0; i < index; i++) {
		front += string[i];
	}
	for (unsigned int i = index+1; i < size(string); i++) {
		back += string[i];
	}
}

long long Menus::randNum(long long lower, long long upper) {
	std::random_device rd; // obtain a random number from hardware
	std::mt19937 gen(rd()); // seed the generator
	std::uniform_int_distribution<long long> distr(lower, upper); // define the range

	//generates a random number
	return distr(gen);
}

std::string Menus::money_commas(double amount) {
	auto str_amount = std::to_string(amount);
	str_amount = str_amount.substr(0, str_amount.size() - 4);
	int n = str_amount.length() - 6;
	while (n > 0) {
		str_amount.insert(n, ",");
		n -= 3;
	}
	return str_amount;
}

void Menus::main_menu() {
	while (true) {
		std::cout << "<<===== Awesome Code Bank =====>>\n" << std::endl;
		std::cout << "<< 1 >> Login" << std::endl;
		std::cout << "<< 2 >> Signup" << std::endl;
		std::cout << "<< 3 >> Exit" << std::endl;

		std::string selection;
		std::cin >> selection;
		std::cin.ignore();

		if (selection == "1") {
			Menus::login_menu();
		}
		else if (selection == "2") {
			Menus::signup_menu();
		}
		else if (selection == "3") {
			system("cls");
			std::cout << "<<==== Exiting the program... ====>>" << std::endl;
			break;
		}
	}
}

void Menus::login_menu() {
	system("cls");
	std::cout << "<<==== Account Login ====>>" << std::endl;
	std::string get_username;
	std::cout << "Email or Username: ";
	std::getline(std::cin, get_username);

	std::string get_password;
	std::cout << "Password: ";
	std::getline(std::cin, get_password);

	std::string password_hash = sha256(get_password);

	std::ifstream load_accounts("data/accounts.json");
	json accounts;
	load_accounts >> accounts;
	load_accounts.close();

	bool login_success = false;
	for (unsigned int i = 0; i < size(accounts["Accounts"]); i++) {
		if (accounts["Accounts"][i]["Email"] == get_username || accounts["Accounts"][i]["Username"] == get_username) {
			if (accounts["Accounts"][i]["Password"] == password_hash) {
				json account = accounts["Accounts"][i];
				login_success = true;

				long long a_num = account["Account Number"].get<long long>();
				std::vector<Transaction> transaction_list;
				for (unsigned int i = 0; i < size(account["Transactions"]); i++) {
					if (!account["Transactions"][i].is_null()) {
						int id = account["Transactions"][i]["ID"].get<int>();
						std::string description = account["Transactions"][i]["Description"].get<std::string>();
						std::string type = account["Transactions"][i]["Type"].get<std::string>();
						double amount = account["Transactions"][i]["Amount"].get<double>();
						std::string date = account["Transactions"][i]["Date"].get<std::string>();
						Transaction transaction(id, description, type, amount, date, a_num);
						transaction_list.emplace_back(transaction);
					}
				}
				std::string username = account["Username"].get<std::string>();
				std::string name = account["Name"].get<std::string>();
				std::string email = account["Email"].get<std::string>();
				std::string type = account["Type"].get<std::string>();
				double bal = account["Balance"].get<double>();
				BankAccount user_account(a_num, username, name, email, type, bal, transaction_list);
				Menus::account_menu(user_account);
			}
		}
	}
	if (!login_success) {
		system("cls");
		std::cout << "<<=== Invalid Login Credentials ===>>\n" << std::endl;
	}
}

void Menus::signup_menu() {
	system("cls");
	std::cout << "<<==== Account Signup ====>>" << std::endl;
	std::string get_name;
	std::cout << "Name: ";
	std::getline(std::cin, get_name);

	std::string get_email;
	std::cout << "Email: ";
	std::getline(std::cin, get_email);

	while (true) {
		if (get_email.find('@') != std::string::npos)
		{
			break;
		}
		else {
			std::cout << "\n<<== Invalid Email ==>> \n" << std::endl;
			std::cout << "Email: ";
			std::getline(std::cin, get_email);
		}
	}

	std::string get_type;
	std::cout << "Checking or Savings: ";
	std::getline(std::cin, get_type);
	transform(get_type.begin(), get_type.end(), get_type.begin(), ::tolower);
	while (get_type != "checking" && get_type != "savings") {
		std::cout << "\n<<== Invalid Account Type ==>> \n" << std::endl;
		std::cout << "Checking or Savings: ";
		std::getline(std::cin, get_type);
		transform(get_type.begin(), get_type.end(), get_type.begin(), ::tolower);
	}
	get_type[0] = toupper(get_type[0]);

	std::string get_password;
	std::cout << "Password: ";
	std::getline(std::cin, get_password);

	std::string get_retype_password;
	std::cout << "Retype Password: ";
	std::getline(std::cin, get_retype_password);

	while (get_password != get_retype_password) {
		std::cout << "\n<<== Passwords Do Not Match ==>> \n" << std::endl;
		std::cout << "Password: ";
		std::getline(std::cin, get_password);

		std::cout << "Retype Password: ";
		std::getline(std::cin, get_retype_password);
	}

	long long account_num = randNum(100000000000, 999999999999);
	std::string username;
	std::string password_hash = sha256(get_password);

	std::ifstream load_accounts("data/accounts.json");
	json accounts;
	load_accounts >> accounts;
	load_accounts.close();

	bool used_account_num = false;
	while (true) {
		for (unsigned int i = 0; i < size(accounts["Accounts"]); i++) {
			if (accounts["Accounts"][i]["Account Number"] == account_num) {
				used_account_num = true;
			}
		}
		if (!used_account_num) {
			break;
		}
		else {
			account_num = randNum(100000000000, 999999999999);
		}
	}

	std::string str_account_num = std::to_string(account_num);
	std::string first_name;
	std::string last_name;
	int space = find_space(get_name);
	if (space != NULL) {
		split(get_name, first_name, last_name, space);
		username += first_name[0];
		if (size(last_name) > 8) {
			for (unsigned int i = 0; i < 8; i++) {
				username += last_name[i];
			}
		}
		else {
			for (unsigned int i = 0; i < size(last_name); i++) {
				username += last_name[i];
			}
		}
		username += str_account_num.substr(str_account_num.length() - 4);;
	}
	else {
		if (size(get_name) > 9) {
			for (unsigned int i = 0; i < 9; i++) {
				username += get_name[i];
			}
		}
		else {
			for (unsigned int i = 0; i < size(get_name); i++) {
				username += get_name[i];
			}
		}
		username += str_account_num.substr(str_account_num.length() - 4);;
	}
	transform(username.begin(), username.end(), username.begin(), ::tolower);

	std::ofstream add_account("data/accounts.json");
	json new_account_json;

	new_account_json = {
			{ "Account Number", account_num },
			{ "Name", get_name },
			{ "Username",  username},
			{ "Email", get_email },
			{ "Type", get_type },
			{ "Balance", 0 },
			{ "Password", password_hash },
			{ "Transactions", { {} } }
	};
	accounts["Accounts"].emplace_back(new_account_json);

	add_account << std::setw(4) << accounts << std::endl;
	add_account.close();

	system("cls");

	std::cout << "<<=== Successfully Created New Account ===>>\n" << std::endl;
}

void Menus::account_menu(BankAccount& user_account) {
	system("cls");
	std::cout << "<<==== Login Successful ====>>\n" << std::endl;
	while (true) {
		std::cout << "Welcome, " << user_account.get_name() << "\n" << std::endl;
		std::cout << " > Username: " << user_account.get_username() << std::endl;
		std::cout << std::fixed << std::setprecision(2);
		std::cout << " > Account Number: " << user_account.get_account_num() << std::endl;
		std::cout << "\n \n > Balance: $" << money_commas(user_account.get_account_bal()) << std::endl;
		std::cout << "\n \n<<== Recent Transactions ==>>\n" << std::endl;

		std::vector<Transaction> user_trans = user_account.get_transactions();
		int num_trans = size(user_account.get_transactions());
		if (num_trans >= 3) {
			for (int i = 0; i < 3; i++) {
				std::cout << " > [" << user_trans[num_trans -1 - i].get_type() << "] " << money_commas(user_trans[num_trans - 1 - i].get_amount()) << " - " << user_trans[num_trans -1 - i].get_date() << std::endl;
			}
		}
		else {
			for (int i = 0; i < num_trans; i++) {
				std::cout << " > [" << user_trans[num_trans - 1 - i].get_type() << "] " << money_commas(user_trans[num_trans - 1 - i].get_amount()) << " - " << user_trans[num_trans - 1 - i].get_date() << std::endl;
			}
		}

		std::cout << "\n \n \n<< 1 >> Withdraw Money"  << std::endl;
		std::cout << "<< 2 >> Deposit Money" << std::endl;
		std::cout << "<< 3 >> Transaction History" << std::endl;
		std::cout << "<< 4 >> Account Settings" << std::endl;
		std::cout << "<< 5 >> Logout" << std::endl;

		std::string option;
		getline(std::cin, option);
		if (option == "1") {
			Menus::withdraw_menu(user_account);
		}
		else if (option == "2") {
			Menus::deposit_menu(user_account);
		}
		else if (option == "3") {
			Menus::trans_hist_menu(user_account);
		}
		else if (option == "4") {
			Menus::acc_settings_menu(user_account);
		}
		else if (option == "5") {
			system("cls");
			break;
		}
		else {
			system("cls");
		}
	}
}

void Menus::withdraw_menu(BankAccount& user_account) {
	system("cls");
	while (true) {
		std::cout << "<<==== Withdraw ====>>\n" << std::endl;
		std::cout << " > Account Number: " << user_account.get_account_num() << std::endl;
		std::cout << "\n > Balance: $" << money_commas(user_account.get_account_bal()) << std::endl;
		std::cout << "\n > How much money would you like to withdraw?" << std::endl;
		double withdrawl;
		std::cin >> withdrawl;
		//Fixes bug that creates a never ending loop when a non-int is entered.
		std::cin.clear();
		std::cin.ignore(100, '\n');

		while (withdrawl < 0 || withdrawl > user_account.get_account_bal()) {
			std::cout << "\n<<== Invalid Amount ==>>" << std::endl;
			std::cout << "\n > How much money would you like to withdraw?" << std::endl;
			std::cin >> withdrawl;
			//Fixes bug that creates a never ending loop when a non-int is entered.
			std::cin.clear();
			std::cin.ignore(100, '\n');
		}

		withdrawl *= -1;
		user_account.change_balance(withdrawl);
		Transaction new_transaction(0, "$" + std::to_string(-1*withdrawl).substr(0, size(std::to_string(withdrawl)) - 5) + " withdrawl", "Withdrawl", withdrawl, Menus::get_date(), user_account.get_account_num());
		user_account.add_transaction(new_transaction);
		user_account.save_data();
		system("cls");
		std::cout << "<<==== Successfully withdrew $" << money_commas(-1 * withdrawl) << " ====>>\n" << std::endl;
		break;
	}
}

void Menus::deposit_menu(BankAccount& user_account) {
	system("cls");
	while (true) {
		std::cout << "<<==== Deposit ====>>\n" << std::endl;
		std::cout << " > Account Number: " << user_account.get_account_num() << std::endl;
		std::cout << "\n > Balance: $" << money_commas(user_account.get_account_bal()) << std::endl;
		std::cout << "\n > How much money would you like to deposit?" << std::endl;
		double deposit;
		std::cin >> deposit;
		//Fixes bug that creates a never ending loop when a non-int is entered.
		std::cin.clear();
		std::cin.ignore(100, '\n');

		while (deposit < 0) {
			std::cout << "\n<<== Invalid Amount ==>>" << std::endl;
			std::cout << "\n > How much money would you like to deposit?" << std::endl;
			std::cin >> deposit;
			//Fixes bug that creates a never ending loop when a non-int is entered.
			std::cin.clear();
			std::cin.ignore(100, '\n');
		}

		user_account.change_balance(deposit);
		Transaction new_transaction(0, "$" + std::to_string(deposit).substr(0, size(std::to_string(deposit))-5) + " deposit", "Deposit", deposit, Menus::get_date(), user_account.get_account_num());
		user_account.add_transaction(new_transaction);
		user_account.save_data();
		system("cls");
		std::cout << "<<==== Successfully deposited $" << money_commas(deposit) << " ====>>\n" << std::endl;
		break;
	}
}

void Menus::trans_hist_menu(BankAccount& user_account) {
	system("cls");
	int page_num = 1;
	while (true) {
		std::vector<Transaction> user_trans = user_account.get_transactions();
		int num_trans = size(user_account.get_transactions());
		int available_pages = ceil(num_trans / 10.0);
		std::cout << "<<==== Transaction History ====>>\n" << std::endl;
		std::cout << " > Account Number: " << user_account.get_account_num() << std::endl;
		std::cout << " > Total Transactions: " << num_trans << std::endl;
		std::cout << "\n<<== Page " << page_num << "/" << available_pages << " ==>>" << std::endl;

		if (page_num <= available_pages) {
			if ((page_num * 10) > num_trans) {
				for (int i = (page_num - 1) * 10; i < num_trans; i++) {
					std::cout << " > [" << user_trans[i].get_type() << "] " << money_commas(user_trans[i].get_amount()) << " - " << user_trans[i].get_date() << std::endl;
				}
			}
			else {
				for (int i = (page_num - 1) * 10; i < page_num * 10; i++) {
					std::cout << " > [" << user_trans[i].get_type() << "] " << money_commas(user_trans[i].get_amount()) << " - " << user_trans[i].get_date() << std::endl;
				}
			}
		}
		else {
			std::cout << " > No Transactions Available" << std::endl;
		}

		std::cout << " > Type the page number you would like to jump to, or type 'back' to go back." << std::endl;
		std::string option;
		getline(std::cin, option);
		if (option == "back") {
			system("cls");
			break;
		}
		else {
			page_num = std::stoi(option);
			system("cls");
		}
	}
}

void Menus::acc_settings_menu(BankAccount& user_account) {
	system("cls");
	while (true) {
		std::cout << "<<==== Account Settings ====>>\n" << std::endl;
		std::cout << " > Account Number: " << user_account.get_account_num() << std::endl;
		std::cout << " > Account Type: " << user_account.get_account_type() << std::endl;
		std::cout << " > Name: " << user_account.get_name() << std::endl;
		std::cout << " > Username: " << user_account.get_username() << std::endl;
		std::cout << " > Email: " << user_account.get_name() << std::endl;
		std::cout << " > Balance: " << user_account.get_account_bal() << std::endl;

		std::cout << "\n \n<< 1 >> Edit Name" << std::endl;
		std::cout << "<< 2 >> Edit Email" << std::endl;
		std::cout << "<< 3 >> Change Password" << std::endl;
		std::cout << "<< 4 >> Exit" << std::endl;
		std::string option;
		getline(std::cin, option);
		if (option == "1") {
			system("cls");
			std::cout << "<<=== Edit Name ===>>\n" << std::endl;
			std::cout << " > Enter your new name" << std::endl;

			std::string new_name;
			getline(std::cin, new_name);

			user_account.set_name(new_name);
			user_account.save_data();
			system("cls");
			std::cout << "<<=== Successfully Changed Your Name ===>>\n" << std::endl;
		}
		else if (option == "2") {
			system("cls");
			std::cout << "<<=== Edit Email ===>>\n" << std::endl;
			std::cout << " > Enter your new email" << std::endl;

			std::string new_email;
			getline(std::cin, new_email);

			user_account.set_email(new_email);
			user_account.save_data();
			system("cls");
			std::cout << "<<=== Successfully Changed Your Email ===>>\n" << std::endl;
		}
		else if (option == "3") {
			system("cls");
			std::cout << "<<=== Change Password ===>>\n" << std::endl;
			std::cout << " > Enter your current password" << std::endl;

			std::string current_password;
			getline(std::cin, current_password);

			std::string password_hash = sha256(current_password);

			std::ifstream load_accounts("data/accounts.json");
			json accounts;
			load_accounts >> accounts;
			load_accounts.close();

			bool login_success = false;
			for (unsigned int i = 0; i < size(accounts["Accounts"]); i++) {
				if (accounts["Accounts"][i]["Email"] == user_account.get_email()) {
					if (accounts["Accounts"][i]["Password"] == password_hash) {
						login_success = true;
					}
				}
			}
			if (!login_success) {
				system("cls");
				std::cout << "<<=== Incorrect Password ===>>\n" << std::endl;
			}
			else {
				std::cout << " > Enter your new password" << std::endl;
				std::string new_password;
				getline(std::cin, new_password);

				std::cout << " > Retype your new password" << std::endl;
				std::string retype_password;
				getline(std::cin, retype_password);

				if (new_password == retype_password) {
					std::string new_pass_hash = sha256(new_password);
					std::ifstream load_accounts("data/accounts.json");
					json accounts;
					load_accounts >> accounts;
					load_accounts.close();
					for (unsigned int i = 0; i < size(accounts["Accounts"]); i++) {
						if (accounts["Accounts"][i]["Account Number"] == user_account.get_account_num()) {
							accounts["Accounts"][i]["Password"] = new_pass_hash;
						}
					}
					std::ofstream save_data("data/accounts.json");

					save_data << std::setw(4) << accounts << std::endl;
					save_data.close();
					system("cls");
					std::cout << "<<=== Successfully Changed Your Password ===>>\n" << std::endl;
				}
				else {
					system("cls");
					std::cout << "<<=== Passwords Do Not Match ===>>\n" << std::endl;
				}
			}
		}
		else if (option == "4") {
			system("cls");
			break;
		}
		else {
			system("cls");
		}
	}
}

std::string Menus::get_date() {
	auto now = std::chrono::system_clock::now();
	auto in_time_t = std::chrono::system_clock::to_time_t(now);
	tm timeinfo;
	errno_t err = localtime_s(&timeinfo, &in_time_t); 
	std::string am_pm = "AM";
	if (timeinfo.tm_hour > 12)        // Set up extension.
		am_pm = "PM";
	if (timeinfo.tm_hour > 12)        // Convert from 24-hour
		timeinfo.tm_hour -= 12;        // to 12-hour clock.
	if (timeinfo.tm_hour == 0)        // Set hour to 12 if midnight.
		timeinfo.tm_hour = 12;
	return "" + std::to_string(timeinfo.tm_mon + 1) + "/" + std::to_string(timeinfo.tm_mday) + "/" + std::to_string(timeinfo.tm_year + 1900) + ", " + 
		std::to_string(timeinfo.tm_hour) + ":" + std::to_string(timeinfo.tm_min) + ":" + std::to_string(timeinfo.tm_sec) + " " + am_pm;
}
