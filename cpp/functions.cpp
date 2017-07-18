#include "../hpp/functions.hpp"

bool is_number(const std::string& s)
{
	return !s.empty() && std::find_if(s.begin(),
		s.end(), [](char c) { return !std::isdigit(c); }) == s.end();
}

string sha_str(string& word){
	unsigned char obuf[SHA_DIGEST_LENGTH];
	unsigned char *val=new unsigned char[word.size()+1];
	strcpy((char *)val,word.c_str());

	SHA256(val,word.size(),obuf);

	char mdString[SHA_DIGEST_LENGTH];

	for(int i = 0; i < SHA_DIGEST_LENGTH/2; i++)
		sprintf(&mdString[i*2], "%02x", (unsigned int)obuf[i]);
	return string(mdString);
}

string md5_str(string word){
	unsigned char digest[MD5_DIGEST_LENGTH];
	unsigned char *val=new unsigned char[word.size()+1];
	strcpy((char *)val,word.c_str());

	MD5(val, word.size()+1, (unsigned char*)&digest);

	char mdString[MD5_DIGEST_LENGTH];

	for(int i = 0; i < SHA_DIGEST_LENGTH/2; i++)
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

string convert_to_digit(string word){
	string newW = "";
	for(unsigned int i = 0; i < word.size(); i++){
		if(word[i] >= '0' && word[i] <= '9'){
			newW += word[i];
		}
		else{
			newW += to_string((int)word[i]);
		}
	}
	return newW;
}

string convert_to_character(string word){
	string newW = "";
	for(unsigned int i = 0; i < word.size(); i++){
		if(word[i] < '0' && word[i] > '9')
			newW += word[i] - '0' + 'a';
		if(word[i] >= '0' && word[i] <= '9'){
			newW += word[i] - '0' + 'a';		
		}
		else{
			newW += word[i];
		}
	}
	return newW;
}

string special_char(string word){
	string numbers = number_only(word);
	string characters = character_only(word);
	if(!numbers.empty() && !characters.empty()){
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
	if(!numbers.empty()){
		for(unsigned int i = 0; i < numbers.size(); i++){
		num = numbers[i] - '0';
			totalNumbers += num;
		}
	}
	else{
		totalNumbers = word.size();
	}
	if(!characters.empty()){
		for(unsigned int i = 0; i < characters.size(); i++){
			totalCharacters += (int)(characters[i] - 'a');
		}
		char chNum, chCha;
		if(numbers.size() > 0)
			chNum = characters[(totalNumbers/numbers.size())%characters.size()];
		else			
			chNum = characters[totalNumbers%characters.size()];
		if(characters.size() > 0)
			chCha = characters[(totalCharacters/characters.size())%characters.size()];
		else
			chCha = characters[totalCharacters%characters.size()];

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
	}
	return word;
}

int write(const std::string& s)
{
    ofstream outfile;

    outfile.open("data",ofstream::out | ofstream::app);

    outfile<<s<<endl;
    outfile.close();

    cout<<"Writing Complete."<<endl;

    return 0;
}

bool read(const std::string& s, int& nb, po::options_description& desc, po::variables_map& vm)
{
	bool stringfound = false;
    ifstream input;
    input.open("data",ios::in);
    string str;
    stringstream ss(str);

    while (getline(input, str) && !stringfound){
    	if(str.compare(0,s.size(),s) == 0){
    		stringstream ss(str);
    		string tmp;
    		ss >> tmp  >> nb;

    		string options;
    		getline(ss,options);
    		if(options != ""){
    			cout << "Options utilisés sur cet URL :" << options << endl;
    		}
    		string anOption = "";
    		stringstream soptions(options);

    		vector<const char*> newargc;

    		vm = po::variables_map();
    		//cout << "ISSOUUUUUC :" << endl;
    		while(soptions >> anOption){
    			newargc.push_back(const_cast<char*>(anOption.c_str()));
    			int i = 1;
    			if(anOption[i] == '-')
    				i = 2;
    			else
    				i = 1;
    			cout << "ISSOUUUUUC : " << anOption[i] << endl;
    			switch(anOption[i]){
    				case 'n':
    					vm.insert(make_pair("numbers",po::variable_value()));
    					break;
    				case 'c':
    					vm.insert(make_pair("characters",po::variable_value()));
    					break;
    				case 'm':
    					vm.insert(make_pair("capital",po::variable_value()));
    					break;
					case 's':
						if(anOption[i+1] != 'i'){
							anOption = anOption.substr(i+1,anOption.size());
						}
						else if(anOption[i+4] != '='){
							anOption = anOption.substr(i+4,anOption.size());
						}
						else{
							anOption = anOption.substr(i+5,anOption.size());
						}
						try{
							int value = stoi(anOption);
							psw_size = value;
	    					vm.insert(make_pair("size",po::variable_value(value,true)));
    					}
    					catch(invalid_argument&){
    						cout << "Valeur saisie dans size incorrect."<< anOption << endl;
    					}
    					break;
    			}
    		}
			try 
		    { 
		      po::store(po::parse_command_line(newargc.size(), &newargc[0], desc), vm);
		      po::notify(vm); 
		    }catch(exception& e) 
		    {
		        cerr << "Unhandled Exception reached the top of main: " 
		              << e.what() << ", application will now exit" << endl; 
		        return ""; 
		    }
    		stringfound = true;
    	}
    }
    cout<<"Reading Completed. " <<endl;
    input.close();
    return stringfound;
}

void deleteLine(const std::string& s){
    ifstream fin;
    fin.open("data",ios::in);
    ofstream temp;
	temp.open("temp.txt");
    string str;
    stringstream ss(str);

    while (getline(fin, str)){
    	if(str.compare(0,s.size(),s) != 0){
			temp << str << endl;
    	}
    }
	temp.close();
	fin.close();
	remove("data");
	rename("temp.txt","data");
}