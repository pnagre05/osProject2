#ifndef LOCKS_FREE_H
#define LOCKS_FREE_H

#include <atomic>
#include <vector>

struct petersonLock 
{
    std::atomic<bool> flagZero;
    std::atomic<bool> flagOne;
    std::atomic<int> turn;
};

void peterson(petersonLock* lock) 
{
    lock->flagZero = false;
    lock->flagOne = false;
    lock->turn = 0;
}

void acquirePeterson(petersonLock* lock, int threadId) 
{
    if (threadId == 0) 
    {
        lock->flagZero = true;
        lock->turn = 1;
        
        while (lock->flagOne == true && lock->turn == 1) 
        {
            //waiting 
        }
    } 
    else if (threadId == 1) 
    {
        lock->flagOne = true;
        lock->turn = 0;
        
        while (lock->flagZero == true && lock->turn == 0) {
            //waiting
        }
    }
}

//release function that accepts a pointer to lock state
void releasePeterson(petersonLock* lock, int threadId) 
{
    if (threadId == 0) {
        lock->flagZero = false;
    } else if (threadId == 1) {
        lock->flagOne = false;
    }
}

//part 2
struct tourTreeLock 
{
    std::vector<petersonLock*> tree;
    int numLeaves;
};

void tournamentTree(tourTreeLock* tLock, int n) 
{
    //find following power of 2 to handle unused pos when n is not power of 2
    tLock->numLeaves = 1;
    while (tLock->numLeaves < n) 
    {
        tLock->numLeaves = tLock->numLeaves * 2;
    }

    //make 1 index array for binary tree
    tLock->tree.resize(2 * tLock->numLeaves);
    int i = 1;
    while (i < 2 * tLock->numLeaves) 
    {
       tLock->tree[i] = new petersonLock();
       peterson(tLock->tree[i]);
       i++;
    }
}

//clean up to prevent memory leaks
void cleanTournament(tourTreeLock* tLock) 
{
    int i = 1;
    while (i < tLock->tree.size()) 
    {
       delete tLock->tree[i];
       i++;
    }
}

//Thread aquires locks along path from leaf to root
void acquireTournament(tourTreeLock* tLock, int threadId) 
{
    for (int current_node = tLock->numLeaves + threadId; current_node > 1; current_node = current_node / 2) 
    {
        int parent = current_node / 2;
        int side = current_node % 2;
        acquirePeterson(tLock->tree[parent], side);
    }
}

//thread lets locks go in reverse order from root to leaf
void releaseTournament(tourTreeLock* tLock, int threadId) 
{
    std::vector<int> path;

    //save the path from leaf to root
    for (int current_node = tLock->numLeaves + threadId; current_node > 1; current_node = current_node / 2) 
    {
        path.push_back(current_node);
    }

    //reverse traverse to release locks
    for (int i = path.size() - 1; i >= 0; i--) 
    {
        int node = path[i];
        int parent = node / 2;
        int side = node % 2;
        
        releasePeterson(tLock->tree[parent], side);
    }
}

#endif
