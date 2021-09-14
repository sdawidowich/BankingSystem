#pragma once
#include <iostream>
#include "bankaccount.h"

class Menus {
private:
	void login_menu();
	void signup_menu();
	void account_menu(BankAccount& user_account);
	void withdraw_menu(BankAccount& user_account);
	void deposit_menu(BankAccount& user_account);
	void trans_hist_menu(BankAccount& user_account);
	void acc_settings_menu(BankAccount& user_account);
	std::string get_date();
public:
	void invalid_command();
	int find_space(std::string& string);
	void split(std::string& string, std::string& front, std::string& back, int& index);
	long long randNum(long long lower, long long upper);
	std::string money_commas(double amount);
	void main_menu();
};