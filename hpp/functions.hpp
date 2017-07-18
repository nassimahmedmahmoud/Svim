#ifndef FUNCTIONS_HPP
#define FUNCTIONS_HPP
	#ifdef FUNCTIONS_HPP
		#define EXTERN
	#else
		#define EXTERN extern
	#endif

#include <iostream>
#include <fstream>
#include <string>
#include <regex>
#include <openssl/sha.h>
#include <openssl/md5.h>
#include <boost/program_options.hpp>

using namespace std;
namespace po = boost::program_options;

const string SPECIAL_CHARACTERS= " !\"#$%&'()*+,-./:;<=>?@[\\]^_`{|}~";
const int DEFAULT_SIZE_PASSWORD = 12;
extern int psw_size;

bool is_number(const std::string& s);
string sha_str(string& word);
string md5_str(string word);
string number_only(string word);
string character_only(string word);
string special_char(string word);
string add_capital(string word);
string convert_to_digit(string word);
string convert_to_character(string word);
int write(const std::string& s);
bool read(const std::string& s, int& nb, po::options_description& desc, po::variables_map& vm);
void deleteLine(const std::string& s);

#endif
