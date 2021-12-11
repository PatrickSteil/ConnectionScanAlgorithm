#include <iostream>
#include <list>
#include <bitset>

#define GET_ROUNDED_ARR_TIME(x) (((0xfFFFFE000) & (x)) >> 5)
#define GET_EXACT_ARR_TIME(x) (GET_ROUNDED_ARR_TIME(x) + (0xFF & (x)))
#define GET_NUMBER_OF_LEGS(x) (((0b1111100000000) & (x)) >> 8)
#define INSERT_LEG_COUNTER(x) ((0xFF & (x)) + ((0xFFFFFF00 & (x)) << 5) + (1<<8))
#define INCREASE_LEG_COUNTER(x) ( ((GET_NUMBER_OF_LEGS(x)+1) << 8) + ((x) & 0xFFFFE0F))


struct profile_array {
    unsigned int dep_time;
    unsigned int arr_time;
};

bool incorperateIntoList(struct profile_array &new_prof, std::list<struct profile_array> &liste) {
    std::list<struct profile_array>::iterator itr = liste.begin();
    while ((*itr).dep_time < new_prof.dep_time && itr != liste.end()) ++itr;
    liste.insert(itr, new_prof);
    if ((*itr).dep_time == new_prof.dep_time) {
        if ((*itr).arr_time <= new_prof.arr_time) {
            itr = liste.erase(--itr);
            return false;
        } else {
            itr = liste.erase(itr);
        }
    }
    --itr;
    if ((*itr).dep_time > new_prof.dep_time && (*itr).arr_time > new_prof.arr_time && (*(std::prev(itr))).arr_time < new_prof.arr_time) {
        return true;
    }
    while (itr != liste.begin() && (*(std::prev(itr))).arr_time >= new_prof.arr_time) {
        itr = liste.erase(--itr);
    }
    if ((*(std::next(itr))).dep_time != (unsigned int) (~0) && (*(std::next(itr))).dep_time >= new_prof.dep_time && (*(std::next(itr))).arr_time <= new_prof.arr_time) {
        // (*(std::next(itr))).dep_time = new_prof.dep_time;
        itr = liste.erase(itr);
        return false;
    }
    return true;
}


std::bitset<64> showBits(unsigned int number) {
    return std::bitset<64>(number);
}
int main() {
    struct profile_array time1, time2, time3, time4, infty;
    time1 = {(unsigned int) 5, INSERT_LEG_COUNTER(36000)};				// dep: 5 -> arr: 10:00:00 (1)
    time2 = {(unsigned int) 6, INCREASE_LEG_COUNTER(INSERT_LEG_COUNTER(36120))};	// dep: 6 -> arr: 10:02:00 (2)
    time3 = {(unsigned int) 8, INSERT_LEG_COUNTER(36180)};				// dep: 8 -> arr: 10:03:00 (1)
    time4 = {(unsigned int) 10, INSERT_LEG_COUNTER(36420)};				// dep: 10 -> arr: 10:07:00 (1)

    std::cout << (time1.dep_time) << "\t" << showBits(time1.arr_time) << std::endl;
    std::cout << (time2.dep_time) << "\t" << showBits(time2.arr_time) << std::endl;
    std::cout << (time3.dep_time) << "\t" << showBits(time3.arr_time) << std::endl;
    std::cout << (time4.dep_time) << "\t" << showBits(time4.arr_time) << std::endl;
    
    infty = {(unsigned int) (~0), (unsigned int) (~0)};

    std::list<struct profile_array> profile;
    
    if (incorperateIntoList(time1, profile)) {
        std::cout << "Added profile: " << time1.dep_time << "\t" << GET_EXACT_ARR_TIME(time1.arr_time) << " (" << GET_NUMBER_OF_LEGS(time1.arr_time) << ")\n";
    }
    if (incorperateIntoList(time3, profile)) {
        std::cout << "Added profile: " << time3.dep_time << "\t" << GET_EXACT_ARR_TIME(time3.arr_time) << " (" << GET_NUMBER_OF_LEGS(time3.arr_time) << ")\n";
    }
    if (incorperateIntoList(time2, profile)) {
        std::cout << "Added profile: " << time2.dep_time << "\t" << GET_EXACT_ARR_TIME(time2.arr_time) << " (" << GET_NUMBER_OF_LEGS(time2.arr_time) << ")\n";
    }
    if (incorperateIntoList(time4, profile)) {
        std::cout << "Added profile: " << time4.dep_time << "\t" << GET_EXACT_ARR_TIME(time4.arr_time) << " (" << GET_NUMBER_OF_LEGS(time4.arr_time) << ")\n";
    }
    
    for (auto i : profile) {
        std::cout << (i.dep_time) << "\t" << GET_EXACT_ARR_TIME(i.arr_time) << " (" << GET_NUMBER_OF_LEGS(i.arr_time) << ")\n";
    }
    return 0;
}

