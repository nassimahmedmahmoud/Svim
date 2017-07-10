#ifndef FUNCTIONS_HPP
#define FUNCTIONS_HPP

#include <iostream>
#include <string>
#include <regex>
#include <openssl/sha.h>
#include <openssl/md5.h>

using namespace std;


const string SPECIAL_CHARACTERS= " !\"#$%&'()*+,-./:;<=>?@[\\]^_`{|}~";

bool is_number(const std::string& s);
string sha_str(string& word, int size);
string md5_str(string word, int size);
string number_only(string word);
string character_only(string word);
string special_char(string word);
string add_capital(string word);

#endif
