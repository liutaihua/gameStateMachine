#include <iostream>
#include <stdio.h>
#include <ctime>
#include <unistd.h>
#include "Player.h"
#include "Utility.h"

static const int kFrameCount = 60;
static const float kSecondsPerFrame = 1.0f/kFrameCount;


int main(){
    clock_t frameStart;
    Player* player = new Player(1001, "pis");
    frameStart = std::clock();
    uint64 frameCount = 0;
    double diff;
    double interval = 0.05;
    std::cout<< "CLOCKS_PER_SEC:"<< std::to_string(CLOCKS_PER_SEC)<<"\n";
    while(true){
        if(frameCount % 600000 == 0){
            //std::cout<< "now frameCount:"<< std::to_string(frameCount)<<"\n";
            float detTime = frameCount / CLOCKS_PER_SEC;
            player->Update(frameCount);
            if(player->IsDie()){
                std::cout<< "player Die\n";
                //player->Reborn();
            }
        }
        diff = (std::clock() - frameStart) / static_cast<double>(CLOCKS_PER_SEC);
        double timeLeftForNextUpdate = interval - diff;
        // timeLeftForNextUpdate 是当前时钟里开始时刻相差之后， 大于每帧的时间，即需要睡眠来跳过的， 直到下一帧来临
        if (timeLeftForNextUpdate > 0){
            //std::cout<< "now usleep:"<< std::to_string(timeLeftForNextUpdate*1000)<<"\n";
            usleep(timeLeftForNextUpdate*1000);
        }
        frameCount++;
    }
    delete player;
}
