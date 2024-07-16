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
        // The attack function cannot be made into a method as it...
        // makes a circular #include between headers and classes.

        double dynamicDamage;
        double dodgeChance = static_cast<double>(defender.agl) / (defender.agl + attacker.agl);

        // Check if defender dodges the attack
        if (static_cast<double>(rand()) / RAND_MAX < dodgeChance) {
            dynamicDamage = 0; // Defender dodges the attack
            cout<<defender.name<<" dodged the attack!"<<endl<<endl;
            return;
        }

        // Calculate base damage
        double baseDamage = attacker.atk - defender.def * 10;

        if(defender.defending == true){
            defender.defending = false;
            defender.def -= defender.defIncrement;
            defender.defIncrement = 0;
        }

        // Ensure there's at least a base damage multiplier
        if (baseDamage <= 0) {
            baseDamage = 10;
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
                    winner = ai.name;
                    break;
                }
                else if(playerAction == "skip"){
                    player.skip();
                }
                else{
                    cout<<"Invalid action, turn skipped."<<endl;
                    player.skip();
                }

                // Check AI health, on progress
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
                    winner = "You";
                    break;
                }
                else if(aiAction == "skip"){
                    ai.skip();
                }

                if(player.hp <= 0){
                    winner = ai.name;
                    playing = false;
                    break;
                }
            }
            cout<<winner<<" won the fight!";
            getch();
        }
};

#endif