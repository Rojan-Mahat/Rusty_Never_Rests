#include<SFML/Graphics.hpp>
#include "game.hpp"

int main(){
    Game game;
    game.run();
    return 0;
}

//cmake --build build && ./build/DinoRunner

/*&&(!scoreAwarded)){
            score++;
            if(bestScore < score){
                    bestScore = score;
                }
            if(score % 2 == 0){
                enemy.increaseDifficulty(0.5);
            }
            scoreAwarded = true;
        }

        if(enemy.getRespawnedStatus()){
            scoreAwarded = false;
        }
    }*/