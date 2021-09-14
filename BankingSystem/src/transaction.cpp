#include "transaction.h"
#include "json.hpp"
#include "bankaccount.h"
#include <fstream>
#include <filesystem>

using json = nlohmann::json;

namespace fs = std::filesystem;

Transaction::Transaction(int p_id, std::string p_description, std::string p_type, double& p_amount, std::string p_date, long long p_account_num)
	: id(p_id), description(p_description), type(p_type), amount(p_amount), date(p_date) {
	std::ifstream load_accounts("data/accounts.json");
	json accounts;
	load_accounts >> accounts;
	load_accounts.close();
	for (unsigned int i = 0; i < size(accounts["Accounts"]); i++) {
		if (accounts["Accounts"][i]["Account Number"] == p_account_num) {
			Transaction::id = size(accounts["Accounts"][i]["Transactions"]);
		}
	}
}

void Transaction::set_description(std::string& p_description) {
	Transaction::description = p_description;
}

void Transaction::set_type(std::string& p_type) {
	Transaction::type = p_type;
}

void Transaction::set_amount(double& p_amount) {
	Transaction::amount = p_amount;
}

void Transaction::set_date(std::string& p_date) {
	Transaction::date = p_date;
}

int Transaction::get_id() {
	return Transaction::id;
}

std::string Transaction::get_description() {
	return Transaction::description;
}

std::string Transaction::get_type() {
	return Transaction::type;
}

double Transaction::get_amount() {
	return Transaction::amount;
}

std::string Transaction::get_date() {
	return Transaction::date;
}
