#include <iostream>                 //Alan Golden 9/25/2024 Operating Systems
#include <mutex>                    //purpose: the producer/consumer problem with threads and mutexes
#include <thread>
#include <condition_variable>
#include <time.h> //random
using namespace std;

#define MAX 25                                                  //how many numbers to produce
#define N 4                                                     //size of buffer
mutex m;                                                        //lock. I know its not the most creative name but "lock" already exists as a function
condition_variable_any consumerCondition, producerCondition;    //used for signaling
char buffer[N];                                                 //buffer shared between producer and consumer
int Index = 0;                                                  //place in buffer. lowecase index conflicts with global function

void ProducerFunction(){
   srand(time(0));                                    //true randomness
   for(int i = 1; i <= MAX; i++){                     //until max produced
      m.lock();                                          //get exclusive access to buffer
      while(Index == N){ producerCondition.wait(m); }    //wait if buffer is full
      char randomCharacter = rand()%26+'A';              //generate random character A-Z
      cout << (char)tolower(randomCharacter) << ' ' << Index << endl; //print lowercase to indicate generated character
      buffer[Index] = randomCharacter;                   //place randomCharacter in buffer
      Index++;                                           //move Index right
      consumerCondition.notify_all();                    //wake up consumer
      m.unlock();                                        //release access to buffer, can be interrupted by consumer
   }
}

void ConsumerFunction(){
   for(int i = 1; i <= MAX; i++){                     //until max consumed
      m.lock();                                          //get exclusive access to buffer
      while(Index == 0){ consumerCondition.wait(m); }    //wait if buffer is empty
      Index--;                                           //move Index left
      char consumedLetter = buffer[Index];               //retrieve from buffer
      cout << "\t\t" << consumedLetter << ' ' << Index << endl; //print uppercase to indicate consumed character
      buffer[Index] = '?';                               //delete letter from buffer, debug character
      producerCondition.notify_all();                    //wake up producer
      m.unlock();                                        //release access to buffer, can be interruptde by producer
   }
}

int main(){
   cout << "The Process-Consumer Problem" << endl;
   cout << "Buffer Size:  " << N << endl;
   cout << "Max produced: " << MAX << endl;
   cout << "[p]roducer / [C]onsumer \t + index" << endl;
   thread producerThread(ProducerFunction);        //begin producer thread
   this_thread::sleep_for(200ms);                  //stall so that the buffer can be initialized
   thread consumerThread(ConsumerFunction);        //begin consumer thread
   producerThread.join();                          //wait for producer thread to finish generating characters
   consumerThread.join();                          //wait for consumer thread to retrieve all characters
   system("Pause");
   return 0;
}

/*use a char array of size N as the shared buffer
1. Producer should randomly generate >N uppercase letters and insert them into the next
   available space in the buffer, then print out the letter in lowercase form to standard output.
2. The consumer should consume the uppercase characters from the buffer and print them out
   unchanged to standard output.
3. The main function should wait a short amount of time to give the producer time to partially fill the
   buffer before beginning the consumer thread. sleep_for() is the preferred C++ STL method.
4. The producer and consumer threads must only access one element at a time. Do not add or
   remove multiple items within the critical section at one time.*/