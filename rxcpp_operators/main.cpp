#include <rx.hpp>
#include <chrono>
#include <string>

int main() {
    int counter = 0, count = 0;
    auto values = rxcpp::observable<>::range(1, 1000000).window_with_time_or_count(std::chrono::seconds(1), 100000);
    values.
        subscribe(
            [&counter, &count](rxcpp::observable<int> v) {
        int id = counter++;
        printf("[window %d] Create window\n", id);
        v.count().subscribe([](int c) {printf("Count in window: %d\n", c); });
        v.scan(std::string(), [](std::string s, int v) {return s + std::to_string(v); }).last().subscribe([](std::string v) {printf("Len: %zd\n", v.length()); });
        v.subscribe(
            [id, &count](int v) {count++;/*printf("[window %d] OnNext: %d\n", id, v); */},
            [id, &count]() {printf("[window %d] OnCompleted: %d\n", id, count); });
    });
}
