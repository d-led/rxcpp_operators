#include "stdafx.h"

class Sleeper {
    rxcpp::subjects::subject<int> async_subj;
    rxcpp::subscription listener;
public:
    Sleeper() {
        listener = async_subj.get_observable()
            .observe_on(rxcpp::synchronize_new_thread())
            .subscribe([this](int) {
            sleep_sync();
        });
    }
private:
    void sleep_sync() {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        fmt::print("finished sleeping on thread: {}\n", std::this_thread::get_id());
    }
public:
    void sleep_async() {
        async_subj.get_subscriber().on_next(0);
    }
};

void active_objects() {
    std::vector<Sleeper> sloths(4);
    for (auto& sloth : sloths) {
        sloth.sleep_async();
    }
    std::this_thread::sleep_for(std::chrono::seconds(1));
}
