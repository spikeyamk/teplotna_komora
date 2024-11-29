#include <mutex>
#include <condition_variable>
#include <thread>
#include <vector>

#include <trielo/trielo.hpp>

#include "magic/results/deserializer.hpp"
#include "util/sml.hpp"
#include "comm/rs232_uart/transmitter_base.hpp"
#include "comm/rs232_uart/actions.hpp"
#include "comm/rs232_uart/connection.hpp"

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
        friend CRTP;
    private:
        std::mutex mutex;
        std::condition_variable condition_variable;
    public:
        TestSemaphore() = default;
    private:
        void release() {
            condition_variable.notify_one();
        }

        void acquire() {
            std::unique_lock lock(mutex);
            condition_variable.wait(lock);
        }

        bool try_acquire_for(const std::chrono::milliseconds timeout) {
            std::unique_lock lock(mutex);
            return condition_variable.wait_for(lock, timeout) == std::cv_status::no_timeout;
        }
    };
namespace test {
    //using ConnectionStateMachine = boost::sml::sm<Connection<Receiver<TestSemaphore>, TestTransmitter>, boost::sml::testing, boost::sml::logger<util::sml::Logger>>;
    using ConnectionStateMachine = boost::sml::sm<Connection<TestSemaphore, TestTransmitter>, boost::sml::testing>;

    void pusher(std::stop_token st, Receiver<TestSemaphore>& reciever, std::optional<Receiver<TestSemaphore>::EventVariant>& event_variant) {
        while(st.stop_requested() == false) {
            if(event_variant.has_value()) {
                std::visit([&](const auto& event) {
                    const auto event_serialized { magic::commands::Serializer::run(event) };
                    std::copy(event_serialized.begin(), event_serialized.end(), reciever.rx_buf.begin());
                    reciever.push(static_cast<uint16_t>(event_serialized.size()));
                }, event_variant.value());
                event_variant = std::nullopt;
            }
        }
    }

    void worker(std::stop_token st, Receiver<TestSemaphore>& reciever, TestTransmitter& transmitter, ConnectionStateMachine*& connection_ptr){
        util::sml::Logger logger {};
        ConnectionStateMachine connection { reciever, static_cast<TestTransmitter::CRTP&>(transmitter), logger };
        connection_ptr = &connection;
        while(st.stop_requested() == false) {
            std::visit([&connection](const auto& event) {
                connection.process_event(event);
            }, reciever.pop_for(std::chrono::milliseconds(10'000)).value_or(magic::commands::Disconnect()));
        }
    }

    bool is_reciever_being_emptied(Receiver<TestSemaphore>& reciever) {
        for(size_t i = 0; i < 10; i++) {
            if(reciever.empty()) {
                return true;
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
        return false;
    }

    int expected_sequence() {
        using namespace boost::sml;
        Receiver<TestSemaphore> reciever {};
        std::vector<TestTransmitter::ResultVariant> output {};
        TestTransmitter transmitter { output };
        ConnectionStateMachine* connection_ptr { nullptr };
        std::jthread worker_thread(worker, std::ref(reciever), std::ref(transmitter), std::ref(connection_ptr));
        std::optional<decltype(reciever)::EventVariant> event_variant;
        std::jthread pusher_thread(pusher, std::ref(reciever), std::ref(event_variant));

        {
            event_variant = magic::commands::Connect();
            if(is_reciever_being_emptied(reciever) == false) {
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
            event_variant = magic::commands::Nop();
            if(is_reciever_being_emptied(reciever) == false) {
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
            event_variant = magic::commands::ReadTempCtl();
            if(is_reciever_being_emptied(reciever) == false) {
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
            event_variant = magic::commands::Disconnect();
            if(is_reciever_being_emptied(reciever) == false) {
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

        event_variant = magic::commands::Disconnect();
        worker_thread.request_stop();
        pusher_thread.request_stop();
        return 0;
    }

    int unexpected_sequence() {
        using namespace boost::sml;
        Receiver<TestSemaphore> reciever {};
        std::vector<TestTransmitter::ResultVariant> output {};
        TestTransmitter transmitter { output };
        ConnectionStateMachine* connection_ptr { nullptr };
        std::jthread worker_thread(worker, std::ref(reciever), std::ref(transmitter), std::ref(connection_ptr));
        std::optional<decltype(reciever)::EventVariant> event_variant;
        std::jthread pusher_thread(pusher, std::ref(reciever), std::ref(event_variant));

        {
            event_variant = magic::commands::Disconnect();
            if(is_reciever_being_emptied(reciever) == false) {
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
            event_variant = magic::commands::Connect();
            std::this_thread::sleep_for(std::chrono::milliseconds(1000));
            if(is_reciever_being_emptied(reciever) == false) {
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
            event_variant = magic::commands::Connect();
            if(is_reciever_being_emptied(reciever) == false) {
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
            event_variant = magic::commands::Connect();
            if(is_reciever_being_emptied(reciever) == false) {
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
            event_variant = magic::commands::Nop();
            if(is_reciever_being_emptied(reciever) == false) {
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
            event_variant = magic::commands::Disconnect();
            if(is_reciever_being_emptied(reciever) == false) {
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

        event_variant = magic::commands::Disconnect();
        worker_thread.request_stop();
        pusher_thread.request_stop();
        return 0;
    }
}
}
}