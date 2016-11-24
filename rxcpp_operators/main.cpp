#include <rx.hpp>
#include <chrono>
#include <string>
#include <memory>
#include <fmt/ostream.h>

int main() {
    int counter = 0, count = 0;
    auto values = rxcpp::observable<>::range(1, 1000000)
        .window_with_time_or_count(std::chrono::seconds(1), 100000);
    values.
        subscribe(
            [&counter, &count](rxcpp::observable<int> window) {
        int id = counter++;
        printf("[window %d] Create window\n", id);
        window.count()
            .subscribe([](int c) {printf("Count in window: %d\n", c); });
        window.scan(std::make_shared<fmt::MemoryWriter>(), [](std::shared_ptr<fmt::MemoryWriter> const& w, int v) { *w << v; return w; })
            .last()
            .subscribe([](std::shared_ptr<fmt::MemoryWriter> const& w) {printf("Len: %zd (%s...)\n", w->size(),w->str().substr(0,42).c_str()); });
        window.subscribe(
            [id, &count](int v) {count++;},
            [id, &count]() {printf("[window %d] OnCompleted: %d\n", id, count); });
    });
}
