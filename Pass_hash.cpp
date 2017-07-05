#include <iostream>
#include <string>
#include <regex>

using namespace std;

bool is_number(const std::string& s)
{
    return !s.empty() && std::find_if(s.begin(),
        s.end(), [](char c) { return !std::isdigit(c); }) == s.end();
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
}