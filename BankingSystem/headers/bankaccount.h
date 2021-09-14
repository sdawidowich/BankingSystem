#pragma once
#include <iostream>
#include <vector>
#include "transaction.h"

class BankAccount {
private:
	long long account_num = 000000000000;
	std::string account_username;
	std::string account_name;
	std::string account_email;
	std::string account_type;
	double account_bal = 0;
	std::vector<Transaction> account_transactions;
public:
	BankAccount(long long& p_account_num, std::string& p_username, std::string& p_name, std::string& p_email, std::string& p_account_type, double& p_balance, std::vector<Transaction>& p_transactions);
	void set_account_num(long long& p_account_num);
	void set_username(std::string& p_username);
	void set_name(std::string& p_name);
	void set_email(std::string& p_email);
	void set_account_type(std::string& p_account_type);
	void set_account_bal(double& p_balance);
	void set_transactions(std::vector<Transaction>& p_transactions);
	void add_transaction(Transaction& p_transaction);
	void change_balance(double& p_amount);
	long long get_account_num();
	std::string get_username();
	std::string get_name();
	std::string get_email();
	std::string get_account_type();
	double get_account_bal();
	std::vector<Transaction> get_transactions();
	void save_data();
};