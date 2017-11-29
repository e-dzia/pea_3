#include <iostream>
#include <ctime>
#include <thread>
#include "TravellingSalesmanProblem.h"

void test();
void test2();
void test_both();

void func(){
    /*Timer t;
    t.start();
    double time = 0;
    do {
        time = t.getWithoutStopping();
    }while(time < 5);*/
    std::this_thread::sleep_for (std::chrono::seconds(3));
    exit(0);
}

int main() {
    srand(time(NULL));

    //std::thread wateczek(func);

   // static std::thread::native_handle();

   /* boost::thread t(boost::bind(func1, arr1));
    if(!t.timed_join(boost::posix_time::minutes(5))){
        // thread still running, use interrupt or detach
    }

    boost::thread api_caller(::func);
    if (api_caller.timed_join(boost::posix_time::milliseconds(500)))
    {
        // API call returned within 500ms
    }
    else
    {
        // API call timed out
    }*/

    TravellingSalesmanProblem * tsp = new TravellingSalesmanProblem;
    tsp->loadFromFile("dane.txt");


    tsp->menu();

    //std::cout << tsp->bruteForce();

    std::cout << tsp->tabuSearch();
    //tsp->generateRandom(4);
    //tsp->saveToFile("data_salesman.txt");
    //std::cout << tsp->testTime(3);
    //tsp->loadFromFile("dane.txt");
    //std::cout << tsp->localSearch();
    delete tsp;

    //test();
    //test_both();

    //wateczek.join();
    return 0;
}


void test2(){
    TravellingSalesmanProblem *tsp = new TravellingSalesmanProblem();
    std::ofstream fout;
    fout.open("results_test.txt");
    //int size[] = {8, 10, 12, 14, 16, 18, 20, 22};
    for (int i = 8; i < 50; i++){ //rozmiar
        //for (int j = 0; j < 100; j++){
            tsp->generateRandom(i);
            fout << i << " " << tsp->testTime(3)<< std::endl;
       // }
    }

    fout.close();
    delete tsp;
}


void test(){
    TravellingSalesmanProblem *tsp = new TravellingSalesmanProblem();
    std::ofstream fout;
    fout.open("results_dponly.txt");
    int size[] = {8, 10, 12, 14, 16, 18, 20, 22};
    for (int i = 0; i < 8; i++){ //rozmiar
        for (int j = 0; j < 100; j++){
            tsp->generateRandom(size[i]);
            fout << size[i] << " " << tsp->testTime(3)<< std::endl;
        }
    }

    fout.close();
    delete tsp;
}

void test_both(){
    TravellingSalesmanProblem *tsp = new TravellingSalesmanProblem();
    std::ofstream fout;
    fout.open("results_both.txt");
    int size[] = {8,9,10,11,12};
    for (int i = 0; i < 5; i++){ //rozmiar
        for (int j = 0; j < 100; j++){
            tsp->generateRandom(size[i]);
            fout << size[i] << " " << tsp->testTime(0) << " " << tsp->testTime(3)<< std::endl;
        }
    }

    fout.close();
    delete tsp;
}