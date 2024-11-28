#include <semaphore>
#include <thread>
#include <vector>

#include "util/sml.hpp"
#include "comm/rs232_uart/rs232_uart.hpp"
#include "magic/results/deserializer.hpp"
#include "comm/rs232_uart/test.hpp"

namespace comm {
namespace rs232_uart {
    class TestSemaphore : public SemaphoreBase<TestSemaphore> {
    private:
        std::binary_semaphore sem { 0 };
    public:
        TestSemaphore() = default;
        void release() {
            sem.release();
    

        void acquire() {
            //sem.acquire();
        }

        bool try_acquire_for(const uint32_t timeout_ms) {
            //return sem.try_acquire_for(std::chrono::milliseconds(timeout_ms));
            return false;
        }
    };

    std::vector<typename magic::results::Deserializer::Variant> output;

    void actions::Connect::operator()() const {
        output.push_back(magic::results::Connect());
    }

    void actions::Disconnect::operator()() const {
        output.push_back(magic::results::Disconnect());
    }

    void actions::Nop::operator()() const {
        output.push_back(magic::results::Nop());
    }

    void actions::ReadTempCtl::operator()() const {
        output.push_back(magic::results::ReadTempCtl());
    }
namespace test {
    int returns_zero() {
        return 0;
    }

    using TestChannel = Channel<TestSemaphore>;
    using ConnectionStateMachine = boost::sml::sm<Connection<TestChannel>, boost::sml::testing, boost::sml::logger<util::sml::Logger>>;
    //using ConnectionStateMachine = boost::sml::sm<Connection<TestChannel>, boost::sml::testing>;
    ConnectionStateMachine* connection_ptr { nullptr };

    void worker(std::stop_token st, TestChannel& channel) {
        util::sml::Logger logger {};
        ConnectionStateMachine connection { channel, logger };
        connection_ptr = &connection;
        while(st.stop_requested() == false) {
            if(std::visit([&connection](const auto& event) {
                std::cout
                    << "TestChannel::EventVariant(event).index(): "
                    << TestChannel::EventVariant(event).index()
                    << std::endl;
                return connection.process_event(event);
            }, channel.pop_for(5'000).value_or(magic::commands::Disconnect())) == false) {
                std::cout << "wrong_transition: processing magic::commands::Disconnect()\n";
                connection.process_event(magic::commands::Disconnect());
            }
            std::cout << "worker tick\n";
        }
    }

    TestChannel channel {};

    bool is_channel_being_emptied() {
        for(size_t i = 0; i < 10; i++) {
            if(channel.empty()) {
                return true;
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }
        return false;
    }

    int normal_sequence() {
        using namespace boost::sml;
        std::jthread worker_thread(worker, std::ref(channel));

        channel.push(magic::commands::Connect());
        if(is_channel_being_emptied() == false) {
            worker_thread.detach();
            return 1;
        }
        for(int i = 0; (i <= 10) && (connection_ptr == nullptr); i++) {
            if(i == 10) {
                worker_thread.detach();
                return 2;
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        if(connection_ptr->is(state<states::Connected>) == false) {
            worker_thread.detach();
            return 3;
        }

        channel.push(magic::commands::Nop());
        if(is_channel_being_emptied() == false) {
            worker_thread.detach();
            return 4;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        if(connection_ptr->is(state<states::Connected>) == false) {
            worker_thread.detach();
            return 5;
        }

        channel.push(magic::commands::ReadTempCtl());
        if(is_channel_being_emptied() == false) {
            worker_thread.detach();
            return 6;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        if(connection_ptr->is(state<states::Connected>) == false) {
            worker_thread.detach();
            return 7;
        }

        channel.push(magic::commands::Disconnect());
        if(is_channel_being_emptied() == false) {
            worker_thread.detach();
            return 8;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        if(connection_ptr->is(state<states::Disconnected>) == false) {
            worker_thread.detach();
            return 9;
        }
        worker_thread.request_stop();
        channel.push(magic::commands::Disconnect());

        if(output.size() != 4) {
            worker_thread.detach();
            return 10;
        }

        if(std::holds_alternative<magic::results::Connect>(output[0]) == false) {
            worker_thread.detach();
            return 11;
        }

        if(std::holds_alternative<magic::results::Nop>(output[1]) == false) {
            worker_thread.detach();
            return 12;
        }

        if(std::holds_alternative<magic::results::ReadTempCtl>(output[2]) == false) {
            worker_thread.detach();
            return 13;
        }

        if(std::holds_alternative<magic::results::Disconnect>(output[3]) == false) {
            worker_thread.detach();
            return 14;
        }

        return 0;
    }

    int unexpected_sequence() {
        using namespace boost::sml;
        std::jthread worker_thread(worker, std::ref(channel));

        channel.push(magic::commands::Disconnect());
        std::cout << "channel.push(magic::commands::Disconnect())\n";
        if(is_channel_being_emptied() == false) {
            worker_thread.detach();
            return 1;
        }
        for(int i = 0; (i <= 10) && (connection_ptr == nullptr); i++) {
            if(i == 10) {
                worker_thread.detach();
                return 2;
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        if(connection_ptr->is(state<states::Disconnected>) == false) {
            worker_thread.detach();
            return 3;
        }

        channel.push(magic::commands::Connect());
        std::this_thread::sleep_for(std::chrono::milliseconds(100));

        /*
        std::cout << "channel.push(magic::commands::Connect())\n";
        if(is_channel_being_emptied() == false) {
            worker_thread.detach();
            return 4;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        if(connection_ptr->is(state<states::Connected>) == false) {
            worker_thread.detach();
            return 5;
        }

        channel.push(magic::commands::Connect());
        if(is_channel_being_emptied() == false) {
            worker_thread.detach();
            return 6;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        if(connection_ptr->is(state<states::Disconnected>) == false) {
            worker_thread.detach();
            return 7;
        }

        channel.push(magic::commands::Connect());
        if(is_channel_being_emptied() == false) {
            worker_thread.detach();
            return 8;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        if(connection_ptr->is(state<states::Connected>) == false) {
            worker_thread.detach();
            return 9;
        }

        channel.push(magic::commands::Nop());
        if(is_channel_being_emptied() == false) {
            worker_thread.detach();
            return 10;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        if(connection_ptr->is(state<states::Connected>) == false) {
            worker_thread.detach();
            return 11;
        }

        channel.push(magic::commands::Disconnect());
        if(is_channel_being_emptied() == false) {
            worker_thread.detach();
            return 12;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        if(connection_ptr->is(state<states::Disconnected>) == false) {
            worker_thread.detach();
            return 13;
        }

        worker_thread.request_stop();
        channel.push(magic::commands::Disconnect());

        if(output.size() != 5) {
            worker_thread.detach();
            return 14;
        }

        if(std::holds_alternative<magic::results::Connect>(output[0]) == false) {
            worker_thread.detach();
            return 15;
        }

        if(std::holds_alternative<magic::results::Disconnect>(output[1]) == false) {
            worker_thread.detach();
            return 16;
        }

        if(std::holds_alternative<magic::results::Connect>(output[2]) == false) {
            worker_thread.detach();
            return 17;
        }

        if(std::holds_alternative<magic::results::Nop>(output[3]) == false) {
            worker_thread.detach();
            return 18;
        }

        if(std::holds_alternative<magic::results::Disconnect>(output[4]) == false) {
            worker_thread.detach();
            return 19;
        }
        */

        return 0;
    }
}
}
}
}