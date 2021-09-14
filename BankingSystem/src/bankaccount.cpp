#include "bankaccount.h"
#include "json.hpp"
#include <fstream>
#include <filesystem>

using json = nlohmann::json;

namespace fs = std::filesystem;

BankAccount::BankAccount(long long& p_account_num, std::string& p_username, std::string& p_name, std::string& p_email, std::string& p_account_type, double& p_balance, std::vector<Transaction>& p_transactions) 
	: account_num(p_account_num), account_username(p_username), account_name(p_name), account_email(p_email), account_type(p_account_type), account_bal(p_balance), account_transactions(p_transactions) {

}

void BankAccount::set_account_num(long long& p_account_num) {
	BankAccount::account_num = p_account_num;
}

void BankAccount::set_username(std::string& p_username) {
	BankAccount::account_username = p_username;
}

void BankAccount::set_name(std::string& p_name) {
	BankAccount::account_name = p_name;
}

void BankAccount::set_email(std::string& p_email) {
	BankAccount::account_email = p_email;
}

void BankAccount::set_account_type(std::string& p_account_type) {
	BankAccount::account_type = p_account_type;
}

// Sets balance to specified amount.
void BankAccount::set_account_bal(double& p_balance) {
	BankAccount::account_bal = p_balance;
}

void BankAccount::set_transactions(std::vector<Transaction>& p_transactions) {
	BankAccount::account_transactions = p_transactions;
}

void BankAccount::add_transaction(Transaction& p_transaction) {
	BankAccount::account_transactions.emplace_back(p_transaction);
	std::ifstream load_accounts("data/accounts.json");
	json accounts;
	load_accounts >> accounts;
	load_accounts.close();
	for (unsigned int i = 0; i < size(accounts["Accounts"]); i++) {
		if (accounts["Accounts"][i]["Account Number"] == BankAccount::account_num) {
			json transaction_json = {
				{"ID", p_transaction.get_id()},
				{"Description", p_transaction.get_description()},
				{"Type", p_transaction.get_type()},
				{"Amount", p_transaction.get_amount()},
				{"Date", p_transaction.get_date()}
			};

			accounts["Accounts"][i]["Transactions"].emplace_back(transaction_json);
			
		}
	}
	std::ofstream save_data("data/accounts.json");

	save_data << std::setw(4) << accounts << std::endl;
	save_data.close();
}

// Modifies the existing balance according to specified amount. Ex. Balance of 100 and specified amount 10 turns balance into 110.
void BankAccount::change_balance(double& p_amount) {
	BankAccount::account_bal += p_amount;
}

long long BankAccount::get_account_num() {
	return BankAccount::account_num;
}

std::string BankAccount::get_username() {
	return BankAccount::account_username;
}

std::string BankAccount::get_name() {
	return BankAccount::account_name;
}

std::string BankAccount::get_email() {
	return BankAccount::account_email;
}

std::string BankAccount::get_account_type() {
	return BankAccount::account_type;
}

double BankAccount::get_account_bal() {
	return BankAccount::account_bal;
}

std::vector<Transaction> BankAccount::get_transactions() {
	return BankAccount::account_transactions;
}

void BankAccount::save_data() {
	std::ifstream load_accounts("data/accounts.json");
	json accounts;
	load_accounts >> accounts;
	load_accounts.close();
	for (unsigned int i = 0; i < size(accounts["Accounts"]); i++) {
		if (accounts["Accounts"][i]["Account Number"] == BankAccount::account_num) {
			accounts["Accounts"][i]["Balance"] = BankAccount::account_bal;
			accounts["Accounts"][i]["Name"] = BankAccount::account_name;
			accounts["Accounts"][i]["Email"] = BankAccount::account_email;
		}
	}

	std::ofstream save_data("data/accounts.json");

	save_data << std::setw(4) << accounts << std::endl;
	save_data.close();
}
