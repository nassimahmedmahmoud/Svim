#include "../hpp/functions.hpp"

namespace 
{ 
  const size_t ERROR_IN_COMMAND_LINE = 1; 
  const size_t SUCCESS = 0; 
  const size_t ERROR_UNHANDLED_EXCEPTION = 2; 
 
} 
int psw_size;
//Si options a changé que faire ?
int main(int argc, char ** argv){

    /*int *psw_size;
    const int **ppx = &psw_size;
    *ppx = &psw_size_const;*/
    psw_size = DEFAULT_SIZE_PASSWORD;
    string url;
    string pw;
    string results = "";

    po::options_description desc("Options"); 
    desc.add_options()
        ("capital,m","Set capitals (useful for the option 'characters')")
        ("characters,c", "only characters (can't use with numbers)")
        ("help", "Print help messages")
        ("numbers,n", "only numbers (can't use with characters)")
        ("size,s", po::value<int>(&psw_size),"size of password")
        (",+","recreating passwords (without the minus) and at the end of the call.");

    po::variables_map vm;

    try 
    { 
      po::store(po::parse_command_line(argc, argv, desc), vm);
 
      if ( vm.count("help")  )
      { 
        cout << "Utilisation de SVIM : " << endl 
                  << endl << "./svim --options -n=NB URL password"
                  << endl << "Le programme Svim vous retournera un mot de passe générique."
                  << endl << endl << "Svim est un programme qui va générer un mot de passe en fonction d'une URL et d'un password."
                  << "Ce programme a pour utilité de pense-bête et de générateur de mot de passe."
                  << "Svim se fait donc par ligne de commande et prend différents options afin d'améliorer la capacité à générer un mot de passe en fonction de la demande du site."
                  << "Il prend aussi en paramètre une URL, une clé qui sera un de vos mots de passe générique et enfin un nombre si vous devez changer de mot passe pour un même site régulièrement."
                  << endl << endl << desc << endl;
        return SUCCESS; 
      }
      po::notify(vm); 
    } 
    catch(po::error& e) 
    { 
      cerr << "ERROR: " << e.what() << endl << endl; 
      cerr << desc << endl; 
      return ERROR_IN_COMMAND_LINE; 
    } 
    catch(exception& e) 
    {
        cerr << "Unhandled Exception reached the top of main: " 
              << e.what() << ", application will now exit" << endl; 
        return ERROR_UNHANDLED_EXCEPTION; 
    }

    bool plus = false;
    if(strcmp(argv[argc-1],"+") == 0){
        url = argv[argc-3];
        pw = argv[argc-2];
        plus = true;
    }
    else{
        url = argv[argc-2];
        pw = argv[argc-1];
    }

    cout << "[*] Génération du mot de passe" << endl;

    cout << "[*] Mise en forme de l'URL" << endl;

    regex r("(?:[a-z0-9]+://)?((?:[0-9a-z]+\\.)*[a-z0-9]+\\.[a-z]+)(?:/.*)?");
    smatch m;
    if(!regex_search(url, m, r)){
        cout << "URL incorrect" << endl;
        cerr << desc << endl;
        return ERROR_IN_COMMAND_LINE; 
    }

    url = m[1];

    cout << "[*] URL : " << url << endl;

    cout << "[*] Verification du password" << endl;

    cout << "[*] Mot de passe de bonne taille" << endl;

    cout << "[*] Verification du NB" << endl;

    int nb = 1;

    bool read_file = read(url,nb,desc,vm);

    if(plus && read_file){
        deleteLine(url);
        nb++;
    }
    cout << "size : " << psw_size << endl;

    string tohash = to_string(nb)+url+pw;

    cout << "[*] mot de passe concatene : " << tohash << endl;

    string hashed = md5_str(tohash);

    if(vm.count("numbers") && vm.count("characters")){
        cerr << "Can't use the options numbers with the options characters. It does not make sense dude." << endl;
        return ERROR_IN_COMMAND_LINE; 
    }
    if(vm.count("numbers")){
        results = convert_to_digit(hashed);

        if((unsigned)psw_size < results.size())
            results = results.substr(0,psw_size);
        
        cout << "Mot de passe contenant seulement des nombres :" << results << endl;
    }

    if(vm.count("characters") && !vm.count("capital") && !vm.count("numbers")){
        cout << "ICIII" << endl;
        results = convert_to_character(hashed);

        if((unsigned)psw_size < results.size())
            results = results.substr(0,psw_size);
        
        cout << "Mot de passe contenant seulement des caractères :" << results << endl;
    }
    if(vm.count("characters") && vm.count("capital") && !vm.count("numbers")){
        cout << "ISSOUUUUU" << endl;
        results = convert_to_character(hashed);

        if((unsigned)psw_size < results.size())
            results = results.substr(0,psw_size);
        results = add_capital(results);
        cout << "Mot de passe contenant seulement des caractères + majuscule :" << results << endl;
    }
    if(results == ""){
        cout << "Mot de passe hashé : " << hashed << endl;

        results = special_char(hashed);

        cout << "Hashé + caractères spéciales :" << results << endl;

        if((unsigned)psw_size < results.size())
            results = results.substr(0,psw_size);

        cout << "Mot de passe splité :" << results << endl;

        cout << "Hashé + Majuscule :" << add_capital(results) << endl;
    }

    if(plus){
        if(argc > 4){
            string argstring = "";
            for(int i = 1; i < argc - 3; i++){
                if(strcmp(argv[i],url.c_str()) != 0 && strcmp(argv[i],pw.c_str()) != 0 && strcmp(argv[i],"+") != 0){
                argstring.append(argv[i]);
                argstring+= " ";
                }
            }
            write(url + " " +  to_string(nb) + " " + argstring);
        }
        else
            write(url + " " +  to_string(nb) + " ");
    }
    else if(!read_file){
        if(argc > 3){
            string argstring = "";
            for(int i = 1; i < argc - 2; i++){
                if(strcmp(argv[i],url.c_str()) != 0 && strcmp(argv[i],pw.c_str()) != 0 && strcmp(argv[i],"+") != 0){
                argstring.append(argv[i]);
                argstring+= " ";
                }
            }
            write(url + " " +  to_string(nb) + " " + argstring);
        }
        else
            write(url + " " +  to_string(nb) + " ");
    }

    return SUCCESS; 
}
