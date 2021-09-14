#pragma once
#include <iostream>

class Transaction {
private:
	int id;
	std::string description;
	std::string type;
	double amount;
	std::string date;
public:
	Transaction(int p_id, std::string p_description, std::string p_type, double& p_amount, std::string p_date, long long p_account_num);
	void set_description(std::string& p_description);
	void set_type(std::string& p_type);
	void set_amount(double& p_amount);
	void set_date(std::string& p_date);
	int get_id();
	std::string get_description();
	std::string get_type();
	double get_amount();
	std::string get_date();

};