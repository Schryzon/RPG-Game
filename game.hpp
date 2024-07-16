#ifndef GAME_HPP
#define GAME_HPP
#include <math.h>
#include <fstream>
#include <vector>
#include <random>
#include <nlohmann/json.hpp>
#include <iostream>
#include <conio.h>
#include <ctime>
#include <algorithm>
#include <cstdlib>
#include <random>
#include "bot.hpp"
#include "player.hpp"

using json = nlohmann::json;
using namespace std;

// A trick by gpt

class Game{
    private:
        bool playing = true;
        string aiChoice;
        json enemyData;
        Player player;
        AIClass ai;
        string winner;

    // Using template to accept any types
    template<typename Attacker, typename Defender>
    void attack(Attacker attacker, Defender defender){
        double dynamicDamage;
        double dodgeChance = static_cast<double>(defender.agl) / (defender.agl + attacker.agl);

        // Check if defender dodges the attack
        if (static_cast<double>(rand()) / RAND_MAX < dodgeChance) {
            dynamicDamage = 0; // Defender dodges the attack
            cout<<defender.name<<" dodged the attack!"<<endl;
        }

        // Calculate base damage
        double baseDamage = attacker.atk - defender.def;

        if(defender.defending == true){
            defender.defending = false;
            defender.def -= defender.defIncrement;
            defender.defIncrement = 0;
        }

        // Ensure there's at least a base damage multiplier
        if (baseDamage <= 0) {
            baseDamage = 1;
        }

        // Scale damage based on HP
        double hpRatio = static_cast<double>(attacker.hp) / (attacker.hp + defender.hp);
        dynamicDamage = baseDamage * hpRatio;
        int finalDamage = static_cast<int>(round(dynamicDamage));
        defender.hp -= finalDamage;

        cout<<attacker.name<<" dealt "<<to_string(finalDamage)<<" damage to "<<defender.name<<"!"<<endl<<endl;
    }

    public:
        Game(string aiChoice, json enemyData) : aiChoice(aiChoice), enemyData(enemyData){
            cout<<"Starting game!"<<endl<<"Difficulty: "<<aiChoice;
            cout<<endl;
        }

        void start(){
            ai.setup(enemyData);
            player.show();
            ai.show();
            cout<<endl;

            // Main game
            while(playing == true){
                string playerAction;
                string aiAction;

                cout<<"= PLAYER TURN ="<<endl;
                cout<<"Decision: "; cin>>playerAction;
                if(playerAction == "attack"){
                    attack(player, ai);
                }
                else if(playerAction == "defend"){
                    player.defend();
                }
                else if(playerAction == "run"){
                    player.run();
                }
                else if(playerAction == "skip"){
                    player.skip();
                }
                else{
                    cout<<"Invalid action, turn skipped."<<endl;
                    player.skip();
                }

                // Check AI health
                if(ai.hp <= 0){
                    winner = "You";
                    playing = false;
                    break;
                }

                // AI
                aiAction = ai.decide(player);
                cout<<"= ENEMY TURN ="<<endl;
                cout<<"Decision: "<<aiAction<<endl;
                if(aiAction == "attack"){
                    attack(ai, player);
                }
                else if(aiAction == "defend"){
                    ai.defend();
                }
                else if(aiAction == "run"){
                    ai.run();
                }
                else if(aiAction == "skip"){
                    ai.skip();
                }

                if(player.hp <= 0){
                    winner = "The enemy";
                    playing = false;
                    break;
                }
            }
            cout<<winner<<"won the fight!";
            getch();
        }
};

#endif