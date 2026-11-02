#include <iostream>   
#include <thread>        
#include <chrono>     
#include <random>     
#include <string>     
#include <algorithm>  
#include <stdexcept>  
#include <cstdio>
#include "locks.h"
using namespace std;

int main(int argc, char* argv[]){
    if (argc != 2) {
        cerr << "Invalid number of arguments!" << endl;
        return 1;
    }
    int n;

    try {
        size_t pos = 0;
        n = stoi(argv[1], &pos);

        if (pos != string(argv[1]).size() || n < 2) {
            cerr << "Invalid number of philosophers!" << endl;
            return 1;
        }
    }
    catch (const exception&) {
        cerr << "Invalid number of philosophers!" << endl;
        return 1;
    }
    
    
    // COARSE
    tourTreeLock globalLock;
    tournamentTree(&globalLock, n);

    cout << "BEGIN COARSE" << endl;
    thread* philosophers = new thread[n];
    for (int i = 0; i < n; i++){
        
        philosophers[i] = (thread([i, &globalLock]() {
            
            mt19937 rng(random_device{}());
            uniform_int_distribution<int> delay(1, 500);

            for (int round = 0; round < 5; round++){
                printf("COARSE %d %d THINK_START\n", i, round);
                int timeThink = delay(rng);
                this_thread::sleep_for(chrono::milliseconds(timeThink));
                printf("COARSE %d %d THINK_END\n", i, round);
                //acquire TTL
                acquireTournament(&globalLock, i);

                printf("COARSE %d %d EAT_START\n", i, round);
                int timeEat =  delay(rng);
                this_thread::sleep_for(chrono::milliseconds(timeEat));
                printf("COARSE %d %d EAT_END\n", i, round);

                // unacquire lock
                releaseTournament(&globalLock, i);
            }

            
        }));

    }
    
    for (int i = 0; i < n; i++){
        philosophers[i].join();
    }

    cout << "END COARSE" << endl;
    delete[] philosophers;
    cleanTournament(&globalLock);
    
    
    // FINE
    cout << "BEGIN FINE" << endl;
    
    thread* philosophersFine = new thread[n];
    petersonLock* chopsticks = new petersonLock[n];
    for (int i = 0; i < n; i++){
        peterson(&chopsticks[i]);
    }

    for (int i = 0; i < n; i++){
        philosophersFine[i] = (thread([i, n, chopsticks]() {
                
                int left = i, right = (i+1) % n;        
                int first = min(left, right);
                int second = max(left, right);

                mt19937 rng(random_device{}());
                uniform_int_distribution<int> delay(1, 500);

                for (int round = 0; round < 5; round++){
                    printf("FINE %d %d THINK_START\n", i, round);
                    int timeThink = delay(rng);
                    this_thread::sleep_for(chrono::milliseconds(timeThink));
                    printf("FINE %d %d THINK_END\n", i, round);

                    int firstID = (i == first) ? 0 : 1;
                    int secondID = (i == second) ? 0 : 1;

                    //acquire lock
                    acquirePeterson(&chopsticks[first], firstID);
                    acquirePeterson(&chopsticks[second], secondID);

                    printf("FINE %d %d EAT_START\n", i, round);
                    int timeEat =  delay(rng);
                    this_thread::sleep_for(chrono::milliseconds(timeEat));
                    printf("FINE %d %d EAT_END\n", i, round);

                    // unacquire lock
                    releasePeterson(&chopsticks[second], secondID);
                    releasePeterson(&chopsticks[first], firstID);

                }

                
        }));
    }

    for (int i = 0; i < n; i++){
        philosophersFine[i].join();
    }
    cout << "END FINE" << endl;

    delete[] philosophersFine;
    delete[] chopsticks;
    
}
