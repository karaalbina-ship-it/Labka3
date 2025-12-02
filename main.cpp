#include <iostream>
#include <syncstream>
#include <format>
#include <thread>
#include <latch>

using namespace std;

void f(char letter, int n) {
    osyncstream{cout} << format("З набору {} виконано дію {}.\n", letter, n);
}

void task_1(latch& start_c, latch& start_d, latch& start_e, latch& start_g) {  // a → b + запуск c,d,e,g
    for (int i = 1; i <= 4; ++i) f('a', i);
    for (int i = 1; i <= 5; ++i) f('b', i);
    start_c.count_down();
    start_d.count_down();
    start_e.count_down();
    start_g.count_down();
}

void task_2(latch& start_c, latch& start_f, latch& start_i) {  // тільки c → f → частина i
    start_c.wait();
    for (int i = 1; i <= 9; ++i) f('c', i);
    start_f.count_down();
    start_i.count_down();
}

void task_3(latch& start_d, latch& start_h) {  // тільки d → h
    start_d.wait();
    for (int i = 1; i <= 9; ++i) f('d', i);
    start_h.count_down();
}

void task_4(latch& start_e, latch& start_f, latch& start_i, latch& start_j) {  // e + f + i + запуск j
    start_e.wait();
    for (int i = 1; i <= 4; ++i) f('e', i);

    start_f.wait();
    for (int i = 1; i <= 4; ++i) f('f', i);

    start_i.wait();                 // чекаємо, поки task_2 зробить свій count_down
    for (int i = 1; i <= 4; ++i) f('i', i);
    start_j.count_down();
}

void task_5(latch& start_g, latch& start_h, latch& start_j) {  // g + h + j
    start_g.wait();
    for (int i = 1; i <= 4; ++i) f('g', i);

    start_h.wait();
    for (int i = 1; i <= 9; ++i) f('h', i);

    start_j.wait();                 // чекаємо i від task_4
    for (int i = 1; i <= 6; ++i) f('j', i);
}

int main() {
    setlocale(LC_ALL, "uk_UA.UTF-8");

    latch start_c(1), start_d(1), start_e(1), start_f(1), start_g(1), start_h(1);
    latch start_i(2);   // c і e+f запускають i
    latch start_j(2);   // i і h запускають j

    osyncstream{cout} << "Обчислення розпочато.\n";

    jthread t1(task_1, ref(start_c), ref(start_d), ref(start_e), ref(start_g));
    jthread t2(task_2, ref(start_c), ref(start_f), ref(start_i));
    jthread t3(task_3, ref(start_d), ref(start_h));
    jthread t4(task_4, ref(start_e), ref(start_f), ref(start_i), ref(start_j));
    jthread t5(task_5, ref(start_g), ref(start_h), ref(start_j));

    osyncstream{cout} << "Обчислення завершено.\n";
}