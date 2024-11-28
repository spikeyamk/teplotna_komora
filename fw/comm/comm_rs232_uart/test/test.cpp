#include <mutex>
#include <condition_variable>
#include <thread>
#include <vector>

#include <trielo/trielo.hpp>

#include "magic/results/deserializer.hpp"
#include "util/sml.hpp"
#include "comm/rs232_uart/rs232_uart.hpp"

namespace comm {
namespace rs232_uart {
    class TestTransmitter : public TransmitterBase<TestTransmitter> {
        friend CRTP;
    public:
        std::vector<ResultVariant>& output;

        TestTransmitter(std::vector<ResultVariant>& output) :
            output { output }
        {}
    private:
        void operator()(const ResultVariant& result_variant) const {
            output.push_back(result_variant);
        }
    };

    template<>
    void actions::Connect<TestTransmitter>::operator()(const TransmitterBase<TestTransmitter>& transmitter) const {
        transmitter(magic::results::Connect());
    }

    template<>
    void actions::Disconnect<TestTransmitter>::operator()(const TransmitterBase<TestTransmitter>& transmitter) const {
        transmitter(magic::results::Disconnect());
    }

    template<>
    void actions::Nop<TestTransmitter>::operator()(const TransmitterBase<TestTransmitter>& transmitter) const {
        transmitter(magic::results::Nop());
    }

    template<>
    void actions::ReadTempCtl<TestTransmitter>::operator()(const TransmitterBase<TestTransmitter>& transmitter) const {
        transmitter(magic::results::ReadTempCtl());
    }

    class TestSemaphore : public SemaphoreBase<TestSemaphore> {
    private:
        std::mutex mutex;
        std::condition_variable condition_variable;
    public:
        TestSemaphore() = default;
        void release() {
            condition_variable.notify_one();
        }

        void acquire() {
            std::unique_lock lock(mutex);
            condition_variable.wait(lock);
        }

        bool try_acquire_for(const uint32_t timeout_ms) {
            std::unique_lock lock(mutex);
            return condition_variable.wait_for(lock, std::chrono::milliseconds(timeout_ms)) == std::cv_status::no_timeout;
        }
    };
namespace test {
    //using ConnectionStateMachine = boost::sml::sm<Connection<Channel<TestSemaphore>, TestTransmitter>, boost::sml::testing, boost::sml::logger<util::sml::Logger>>;
    using ConnectionStateMachine = boost::sml::sm<Connection<Channel<TestSemaphore>, TestTransmitter>, boost::sml::testing>;

    void worker(std::stop_token st, Channel<TestSemaphore>& channel, TestTransmitter& transmitter, ConnectionStateMachine*& connection_ptr){
        util::sml::Logger logger {};
        ConnectionStateMachine connection { channel, static_cast<TestTransmitter::CRTP&>(transmitter), logger };
        connection_ptr = &connection;
        while(st.stop_requested() == false) {
            std::visit([&connection](const auto& event) {
                connection.process_event(event);
            }, channel.pop_for(10'000).value_or(magic::commands::Disconnect()));
        }
    }

    bool is_channel_being_emptied(Channel<TestSemaphore>& channel) {
        for(size_t i = 0; i < 10; i++) {
            if(channel.empty()) {
                return true;
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
        return false;
    }

    int expected_sequence() {
        using namespace boost::sml;
        Channel<TestSemaphore> channel {};
        std::vector<TestTransmitter::ResultVariant> output {};
        TestTransmitter transmitter { output };
        ConnectionStateMachine* connection_ptr { nullptr };
        std::jthread worker_thread(worker, std::ref(channel), std::ref(transmitter), std::ref(connection_ptr));

        {
            channel.push(magic::commands::Connect());
            if(is_channel_being_emptied(channel) == false) {
                worker_thread.detach();
                return 1;
            }
            for(int i = 0; (i <= 10) && (connection_ptr == nullptr); i++) {
                if(i == 10) {
                    worker_thread.detach();
                    return 2;
                }
                std::this_thread::sleep_for(std::chrono::milliseconds(100));
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(1000));
            if(connection_ptr->is(state<states::Connected>) == false) {
                worker_thread.detach();
                return 3;
            }
            if(output.size() != 1) {
                worker_thread.detach();
                return 4;
            }
            if(std::holds_alternative<magic::results::Connect>(output[0]) == false) {
                worker_thread.detach();
                return 5;
            }
        }

        {
            channel.push(magic::commands::Nop());
            if(is_channel_being_emptied(channel) == false) {
                worker_thread.detach();
                return 6;
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(1000));
            if(connection_ptr->is(state<states::Connected>) == false) {
                worker_thread.detach();
                return 7;
            }
            if(output.size() != 2) {
                worker_thread.detach();
                return 8;
            }
            if(std::holds_alternative<magic::results::Nop>(output[1]) == false) {
                worker_thread.detach();
                return 9;
            }
        }

        {
            channel.push(magic::commands::ReadTempCtl());
            if(is_channel_being_emptied(channel) == false) {
                worker_thread.detach();
                return 10;
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(1000));
            if(connection_ptr->is(state<states::Connected>) == false) {
                worker_thread.detach();
                return 11;
            }
            if(output.size() != 3) {
                worker_thread.detach();
                return 12;
            }
            if(std::holds_alternative<magic::results::ReadTempCtl>(output[2]) == false) {
                worker_thread.detach();
                return 13;
            }
        }

        {
            channel.push(magic::commands::Disconnect());
            if(is_channel_being_emptied(channel) == false) {
                worker_thread.detach();
                return 14;
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(1000));
            if(connection_ptr->is(state<states::Disconnected>) == false) {
                worker_thread.detach();
                return 15;
            }
            if(output.size() != 4) {
                worker_thread.detach();
                return 16;
            }
            if(std::holds_alternative<magic::results::Disconnect>(output[3]) == false) {
                worker_thread.detach();
                return 17;
            }
        }

        worker_thread.request_stop();
        channel.push(magic::commands::Disconnect());
        return 0;
    }

    int unexpected_sequence() {
        using namespace boost::sml;
        Channel<TestSemaphore> channel {};
        std::vector<TestTransmitter::ResultVariant> output {};
        TestTransmitter transmitter { output };
        ConnectionStateMachine* connection_ptr { nullptr };
        std::jthread worker_thread(worker, std::ref(channel), std::ref(transmitter), std::ref(connection_ptr));

        {
            channel.push(magic::commands::Disconnect());
            if(is_channel_being_emptied(channel) == false) {
                worker_thread.detach();
                return 1;
            }
            for(int i = 0; (i <= 10) && (connection_ptr == nullptr); i++) {
                if(i == 10) {
                    worker_thread.detach();
                    return 2;
                }
                std::this_thread::sleep_for(std::chrono::milliseconds(100));
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(1000));
            if(connection_ptr->is(state<states::Disconnected>) == false) {
                worker_thread.detach();
                return 3;
            }
            if(output.size() != 0) {
                worker_thread.detach();
                return 4;
            }
        }

        {
            channel.push(magic::commands::Connect());
            std::this_thread::sleep_for(std::chrono::milliseconds(1000));
            if(is_channel_being_emptied(channel) == false) {
                worker_thread.detach();
                return 5;
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(1000));
            if(connection_ptr->is(state<states::Connected>) == false) {
                worker_thread.detach();
                return 6;
            }
            if(output.size() != 1) {
                worker_thread.detach();
                return 7;
            }
            if(std::holds_alternative<magic::results::Connect>(output[0]) == false) {
                worker_thread.detach();
                return 8;
            }
        }

        {
            channel.push(magic::commands::Connect());
            if(is_channel_being_emptied(channel) == false) {
                worker_thread.detach();
                return 9;
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(1000));
            if(connection_ptr->is(state<states::Disconnected>) == false) {
                worker_thread.detach();
                return 10;
            }
            if(output.size() != 2) {
                worker_thread.detach();
                return 11;
            }
            if(std::holds_alternative<magic::results::Disconnect>(output[1]) == false) {
                worker_thread.detach();
                return 12;
            }
        }

        {
            channel.push(magic::commands::Connect());
            if(is_channel_being_emptied(channel) == false) {
                worker_thread.detach();
                return 13;
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(1000));
            if(connection_ptr->is(state<states::Connected>) == false) {
                worker_thread.detach();
                return 14;
            }
            if(output.size() != 3) {
                worker_thread.detach();
                return 15;
            }
            if(std::holds_alternative<magic::results::Connect>(output[2]) == false) {
                worker_thread.detach();
                return 16;
            }
        }

        {
            channel.push(magic::commands::Nop());
            if(is_channel_being_emptied(channel) == false) {
                worker_thread.detach();
                return 17;
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(1000));
            if(connection_ptr->is(state<states::Connected>) == false) {
                worker_thread.detach();
                return 18;
            }
            if(output.size() != 4) {
                worker_thread.detach();
                return 19;
            }
            if(std::holds_alternative<magic::results::Nop>(output[3]) == false) {
                worker_thread.detach();
                return 20;
            }
        }

        {
            channel.push(magic::commands::Disconnect());
            if(is_channel_being_emptied(channel) == false) {
                worker_thread.detach();
                return 21;
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(1000));
            if(connection_ptr->is(state<states::Disconnected>) == false) {
                worker_thread.detach();
                return 22;
            }
            if(output.size() != 5) {
                worker_thread.detach();
                return 23;
            }
            if(std::holds_alternative<magic::results::Disconnect>(output[4]) == false) {
                worker_thread.detach();
                return 24;
            }
        }

        worker_thread.request_stop();
        channel.push(magic::commands::Disconnect());
        return 0;
    }
}
}
}