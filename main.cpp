#include <iostream>
#include <syncstream>
#include <format>
#include <thread>
#include <latch>
using namespace std;
void f(char letter, int n) {
    osyncstream(cout) << format("З набору {} виконано дiю {}.\n", letter, n);
}
void task_a(latch& latch_bcd) {
    for (int i = 1; i <= 4; ++i) f('a', i);
    latch_bcd.count_down();
}
void task_bcd(latch& latch_bcd, latch& latch_e, latch& latch_f, latch& latch_g, latch& latch_h) {
    latch_bcd.wait();
    for (int i = 1; i <= 5; ++i) f('b', i);
    latch_e.count_down();
    for (int i = 1; i <= 9; ++i) f('c', i);
    latch_f.count_down();
    latch_g.count_down();
    for (int i = 1; i <= 9; ++i) f('d', i);
    latch_h.count_down();
}
void task_ef(latch& latch_e, latch& latch_f, latch& latch_i) {
    latch_e.wait();
    for (int i = 1; i <= 4; ++i) f('e', i);
    latch_i.count_down();
    latch_f.wait();
    for (int i = 1; i <= 4; ++i) f('f', i);
    latch_i.count_down();
}
void task_gh(latch& latch_g, latch& latch_h, latch& latch_j) {
    latch_g.wait();
    for (int i = 1; i <= 4; ++i) f('g', i);
    latch_j.count_down();
    latch_h.wait();
    for (int i = 1; i <= 9; ++i) f('h', i);
    latch_j.count_down();
}
void task_ij(latch& latch_i, latch& latch_j) {
    latch_i.wait();
    for (int i = 1; i <= 4; ++i) f('i', i);
    latch_j.count_down();
    latch_j.wait();
    for (int i = 1; i <= 6; ++i) f('j', i);
}
int main() {
    setlocale(LC_ALL, "ukr");
    latch latch_bcd(1);
    latch latch_e(1);
    latch latch_f(1);
    latch latch_g(1);
    latch latch_h(1);
    latch latch_i(2);
    latch latch_j(3);
    osyncstream(cout) << "Обчислення розпочато.\n";
    jthread t_a(task_a, ref(latch_bcd));
    jthread t_bcd(task_bcd, ref(latch_bcd), ref(latch_e), ref(latch_f), ref(latch_g), ref(latch_h));
    jthread t_ef(task_ef, ref(latch_e), ref(latch_f), ref(latch_i));
    jthread t_gh(task_gh, ref(latch_g), ref(latch_h), ref(latch_j));
    jthread t_ij(task_ij, ref(latch_i), ref(latch_j));
    t_a.join();
    t_bcd.join();
    t_ef.join();
    t_gh.join();
    t_ij.join();
    osyncstream(cout) << "Обчислення завершено.\n";
    return 0;
}