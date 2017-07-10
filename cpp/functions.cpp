#include "../hpp/functions.hpp"

bool is_number(const std::string& s)
{
    return !s.empty() && std::find_if(s.begin(),
        s.end(), [](char c) { return !std::isdigit(c); }) == s.end();
}

string sha_str(string& word, int size){
	unsigned char obuf[SHA_DIGEST_LENGTH];
	unsigned char *val=new unsigned char[word.size()+1];
	strcpy((char *)val,word.c_str());
	
	SHA256(val,word.size(),obuf);

	char mdString[size];

	for(int i = 0; i < size/2; i++)
		sprintf(&mdString[i*2], "%02x", (unsigned int)obuf[i]);
	return string(mdString);
}

string md5_str(string word, int size){
	unsigned char digest[MD5_DIGEST_LENGTH];
	unsigned char *val=new unsigned char[word.size()+1];
	strcpy((char *)val,word.c_str());
	
	MD5(val, word.size()+1, (unsigned char*)&digest);

	char mdString[size];

	for(int i = 0; i < size/2; i++)
		sprintf(&mdString[i*2], "%02x", (unsigned int)digest[i]);
	return string(mdString);	
}

string number_only(string word){
	regex r("\\D");
	string s = regex_replace(word,r, "");
	return s;
}

string character_only(string word){
	regex r("\\d");
	string s = regex_replace(word,r, "");
	return s;
}

string special_char(string word){
        string numbers = number_only(word);
        string characters = character_only(word);
        if(numbers != "" && characters != ""){
		int len = (numbers.size() < characters.size()) ? numbers.size() : characters.size();
		string str = "";
		for(int i = 0; i < len; i++){
			int ch;
			if(numbers.size() > characters.size()){
				ch = (int)(characters[i] + numbers[numbers.size() - numbers.size()/(i+1)]);
			}
			else if(numbers.size() < characters.size()){
				ch = (int)(characters[characters.size() - characters.size()/(i+1)] + numbers[i]);
			}
			else
				ch = (int)(characters[i] + numbers[i]);
				
			str= SPECIAL_CHARACTERS[ch%SPECIAL_CHARACTERS.size()];
	      		word.insert((int)(str[0])%word.size(),str);
		}
        }
        return word;
}

string add_capital(string word){
        string numbers = number_only(word);
        string characters = character_only(word);
        int num,totalNumbers = 0, totalCharacters = 0;
        for(unsigned int i = 0; i < numbers.size(); i++){
		num = numbers[i] - '0';
        	totalNumbers += num;
        }
        for(unsigned int i = 0; i < characters.size(); i++){
        	totalCharacters += (int)(characters[i] - 'a');
        }
        cout << "Total numbers : " << totalNumbers/numbers.size() << " | total characters : " << totalCharacters/(characters.size()) << endl;
        char chNum = characters[(totalNumbers/numbers.size())%characters.size()];
        char chCha = characters[(totalCharacters/characters.size())%characters.size()];
        bool boolNum = false, boolCha = false;
	for(unsigned int i = 0; i < word.size(); i++){
		if(word[i] == chNum && !boolNum){
			word[i] = toupper(chNum);
			boolNum = true;
		}
		if(word[i] == chCha && !boolCha){
			word[i] = toupper(chCha);
			boolCha = true;
		}
		 
	}
        return word;
}
