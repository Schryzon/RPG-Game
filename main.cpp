#include <iostream>
#include "game.hpp"
#include "bot.hpp"
#include "player.hpp"
#include <string>
#include <fstream>
#include <nlohmann/json.hpp>
using json = nlohmann::json;
using namespace std;

int main(){
    /* code */
    string aiChoice;
    cout<<"BATTLE OF RE:VOLUTION"<<endl;
    cout<<"Type the difficulty: "; cin>>aiChoice;
    ifstream file(PATH_TO_JSON);
    json j = json::parse(file);
    
    Game game(aiChoice, j);
    game.start();

    return 0;
}