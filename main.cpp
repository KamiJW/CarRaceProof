#include <QCoreApplication>
#include <QDebug>

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <algorithm>
#include <chrono>
#include <map>

using namespace std;

pair<char,char> race(map<char, int> &race_map){
    vector<pair<char, int>> race_vector;
    for(auto race_pair : race_map){
        race_vector.push_back(race_pair);
    }
    sort(race_vector.begin(),race_vector.end(),
         [](pair<char, int> p1, pair<char, int> p2){
        return p1.second > p2.second;
    });
    auto iter = race_vector.begin();
    auto first = race_vector.begin()->first;
    ++iter;
    return {first, iter->first};
}

pair<char,char> get_fastest_2(map<char, int> &car_map){
    map<char, int> race_1;
    race_1.insert({'A',car_map['A']});
    race_1.insert({'B',car_map['B']});
    race_1.insert({'C',car_map['C']});
    auto race_1_result = race(race_1);

    map<char, int> race_2;
    race_2.insert({'D',car_map['D']});
    race_2.insert({'E',car_map['E']});
    race_2.insert({'F',car_map['F']});
    auto race_2_result = race(race_2);

    map<char, int> race_3;
    race_3.insert({'G',car_map['G']});
    race_3.insert({'H',car_map['H']});
    race_3.insert({race_1_result.first, car_map[race_1_result.first]});
    auto race_3_result = race(race_3);

    if (race_3_result.first == race_1_result.first) {
        map<char, int> race_4;
        race_4.insert({'I',car_map['I']});
        race_4.insert({race_2_result.first, car_map[race_2_result.first]});
        race_4.insert({race_3_result.first, car_map[race_3_result.first]});
        auto race_4_result = race(race_4);

        map<char, int> race_5;
        if (race_4_result.first == race_1_result.first) {
            race_5.insert({race_1_result.second, car_map[race_1_result.second]});
            race_5.insert({race_4_result.second, car_map[race_4_result.second]});
            race_5.insert({race_3_result.second, car_map[race_3_result.second]});
            auto race_5_result = race(race_5);
            return {race_4_result.first, race_5_result.first};
        }else{
            map<char, int> race_5;
            race_5.insert({race_2_result.second, car_map[race_2_result.second]});
            race_5.insert({race_4_result.second, car_map[race_4_result.second]});
            auto race_5_result = race(race_5);
            return {race_4_result.first, race_5_result.first};
        }
    }

    map<char, int> race_4;
    race_4.insert({race_2_result.first, car_map[race_2_result.first]});
    race_4.insert({race_3_result.first, car_map[race_3_result.first]});
    auto race_4_result = race(race_4);

    //第五场如果I胜出,则满足I和第四场冠军最快，但是无法分辨谁快
    map<char, int> race_5;
    race_5.insert({'I',car_map['I']});
    if (race_4_result.first == race_2_result.first){
        race_5.insert({race_2_result.second, car_map[race_2_result.second]});
        race_5.insert({race_3_result.first, car_map[race_3_result.first]});
    }else{
        race_5.insert({race_3_result.second, car_map[race_3_result.second]});
        race_5.insert({race_4_result.second, car_map[race_4_result.second]});
    }
    auto race_5_result = race(race_5);
    return {race_4_result.first, race_5_result.first};
}

void print_map(map<char, int> &car_map){
    QString print_line{""};
    for(auto car_pair : car_map){
         print_line += QString() + car_pair.first + ":" + QString::number(car_pair.second) + ", ";
    }
    qDebug() << print_line;
}

void test1(){
    char cars[9]{'A','B','C','D','E','F','G','H','I'};
    map<char, int> car_map;
    vector<int> speed;
    for (int var = 1; var < 10; ++var) {
        speed.emplace_back(var);
    }
    int count = 0;
    do {
        for (int i = 0; i < 9; ++i){
            car_map[cars[i]] = speed[i];
        }
        auto ret = get_fastest_2(car_map);
        if((car_map[ret.first] == 9 && car_map[ret.second] == 8))
            ++count;
        else if((car_map[ret.first] == 8 && car_map[ret.second] == 9)){
            qDebug() << "got it";
            ++count;
            print_map(car_map);
            break;
        }else{
            print_map(car_map);
            qDebug() << "First: " << car_map[ret.first] << ", Second: " << car_map[ret.second];
            break;
        }
    } while (next_permutation(speed.begin(), speed.end()));
    qDebug() << "count " << count << ", " << (count == 362880);
}

void single_test(){
    map<char, int> car_map;
    char cars[9]{'A','B','C','D','E','F','G','H','I'};
    int speeds[9]{1,  2,  3,  4,  5,  6,  7,  8,  9};
    for (int i = 0; i < 9; ++i){
        car_map[cars[i]] = speeds[i];
    }
    auto ret = get_fastest_2(car_map);
    qDebug() << "First: " << car_map[ret.first] << ", Second: " << car_map[ret.second];
}

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    qDebug() << "Program Start" << endl;
    auto start = chrono::high_resolution_clock::now();
    single_test();
    test1();
    auto end = std::chrono::high_resolution_clock::now();
    qDebug() << "Program End" << endl;
    std::chrono::duration<double, std::milli> tm = end - start;	// 毫秒
    // std::chrono::duration<double, std::micro> tm = end - start; 微秒
    qDebug() << "Time Cost: " << tm.count() << "ms" << endl;

    return a.exec();
}
