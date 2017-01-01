#include "stdafx.h"

void windowing_example() {
    int counter = 0, count = 0;

    auto values = rxcpp::observable<>::range(1, 1000000)
        .window_with_time_or_count(std::chrono::seconds(1), 100000);
    
    values.
        subscribe(
            [&counter, &count](rxcpp::observable<int> window) {
                int id = counter++;
                fmt::print("[window {}] Created window\n", id);

                window.count()
                    .subscribe([](int c) {
                    fmt::print("Count in window: {}\n", c);
                });
                
                window.scan(std::make_shared<fmt::MemoryWriter>(), [](std::shared_ptr<fmt::MemoryWriter> const& w, int v) {
                    *w << v;
                    return w;
                })
                .start_with(std::make_shared<fmt::MemoryWriter>())
                .last()
                .subscribe(
                    [](std::shared_ptr<fmt::MemoryWriter> const& w) {
                        fmt::print("Len: {} ({}...)\n", w->size(), w->str().substr(0, 42).c_str());
                    },
                    [](std::exception_ptr ep) {
                        try { std::rethrow_exception(ep); }
                        catch (const std::runtime_error& ex) {
                            std::cerr << ex.what() << std::endl;
                        }
                    },
                    [] {}
                );
                
                window.subscribe(
                    [id, &count](int v) {
                      count++;
                    },
                    [id, &count]() {
                        printf("[window %d] OnCompleted: %d\n", id, count);
                    }
                );
            }
        );
}
