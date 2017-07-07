#include <iostream>
#include <string>
#include <regex>
#include <openssl/sha.h>
#include <openssl/md5.h>

using namespace std;

const string SPECIAL_CHARACTERS= " !\"#$%&'()*+,-./:;<=>?@[\]^_`{|}~";

bool is_number(const std::string& s)
{
    return !s.empty() && std::find_if(s.begin(),
        s.end(), [](char c) { return !std::isdigit(c); }) == s.end();
}

string sha1_str(string& word, int size){
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
        return word;
}

string add_capital(string word){
        string numbers = number_only(word);
        string characters = character_only(word);
        int num,totalNumbers = 0, totalCharacters = 0;
        for(int i = 0; i < numbers.size(); i++){
		num = numbers[i] - '0';
        	totalNumbers += num;
        }
        for(int i = 0; i < characters.size(); i++){
        	totalCharacters += (int)(characters[i] - 'a');
        }
        cout << "Total numbers : " << totalNumbers/numbers.size() << " | total characters : " << totalCharacters/(characters.size()) << endl;
        char chNum = characters[(totalNumbers/numbers.size())%characters.size()];
        char chCha = characters[(totalCharacters/characters.size())%characters.size()];
        bool boolNum = false, boolCha = false;
	for(int i = 0; i < word.size(); i++){
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

int main(int argc, char ** argv){

        if(argc < 3){
                cout << "Usage : password.x URL pw [ NB ]" << endl;
                return 1;
        }

        string url = argv[1];
        string pw = argv[2];
        string nb = "1";
        if( argc >= 4 ){
                nb = argv[3];
        }

        cout << "[*] Génération du mot de passe" << endl;

        cout << "[*] Mise en forme de l'URL" << endl;

        regex r("(?:[a-z0-9]+://)?((?:[0-9a-z]+\\.)*[a-z0-9]+\\.[a-z]+)(?:/.*)?");
        smatch m;
        if(!regex_search(url, m, r)){
                cout << "[x] Mauvais format d'URL." << endl;
                return 2;
        }

        url = m[1];

        cout << "[*] URL : " << url << endl;

        cout << "[*] Verification du password" << endl;

        if(pw.size() < 8){
                cout << "[x] Mot de passe trop petit ( <8 ), taille : " << pw.size() << endl;
                return 3;
        }

        cout << "[*] Mot de passe de bonne taille" << endl;

        cout << "[*] Verification du NB" << endl;

        if(!is_number(nb)){
                cout << "[x] Nb n'est pas un nombre" << endl;
                return 4;
        }

        cout << "[*] NB est bien un nombre" << endl;

        string tohash = nb+url+pw;

        cout << "[*] mot de passe concatene : " << tohash << endl;
        
        string hashed = sha1_str(tohash,10);
        
        cout << "Mot de passe hashé : " << hashed << endl;
        
        string numbers = number_only(hashed);
        string characters = character_only(hashed);
        
        cout << "Hashé + caractères spéciales :" << special_char(hashed) << endl;
        
        cout << "Hashé + Majuscule :" << add_capital(hashed) << endl;
        
      	cout << "Hashé + Majuscule(s) + caractères spéciales :" << special_char(add_capital(hashed)) << endl;
                
        return 0;
}
