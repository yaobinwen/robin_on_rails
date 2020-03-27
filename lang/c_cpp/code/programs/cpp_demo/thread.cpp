#include <chrono>
#include <condition_variable>
#include <mutex>
#include <thread>
#include <utility>
#include <vector>

#include <gtest/gtest.h>

// Forward declaration.
class DataConsumer;

typedef void (*Callback)(DataConsumer *, int);

typedef std::pair<DataConsumer *, Callback> CallbackEntry;

class DataProducer
{
public:
    DataProducer()
      : m_flag_produce(false),
        m_flag_quit(false),
        m_thread(&DataProducer::worker, this)
    {
    }

    void
    addCallback(DataConsumer *c, Callback cb)
    {
        std::lock_guard<std::mutex> g(m_mtx_callbacks);
        m_callbacks.push_back(std::make_pair(c, cb));
    }

    void
    start()
    {
        {
            std::lock_guard<std::mutex> g(m_mtx_flag_produce_or_quit);
            m_flag_produce = true;
        }
        m_cv_produce_or_quit.notify_one();
    }

    void
    stop()
    {
        {
            std::lock_guard<std::mutex> g(m_mtx_flag_produce_or_quit);
            m_flag_produce = false;
        }
        m_cv_produce_or_quit.notify_one();
    }

    void
    quit()
    {
        {
            std::lock_guard<std::mutex> g(m_mtx_flag_produce_or_quit);
            m_flag_quit = true;
        }
        m_cv_produce_or_quit.notify_one();
    }

    void
    waitUntilThreadStarted()
    {
        std::unique_lock lock(m_mtx_cv_thrd_started);
        m_cv_thrd_started.wait(lock);
    }

    void
    waitUntilThreadStopped()
    {
        m_thread.join();
    }

private:
    /// The worker thread has the following states:
    /// STARTED: Right after the thread is started and before it enters the
    ///     `WAITING` state. This is a transient state.
    /// WAITING: The thread is waiting for the user to start production.
    ///     - `produce_flag == false` && `quit_flag == false`
    /// PRODUCING: The thread is producing data.
    ///     - `produce_flag == true` && `quit_flag == false`
    /// QUITTING: The thread stops producing and is quitting.
    ///     - `quit_flag == true`
    void
    worker()
    {
        // Notify the waiter that the worker thread has been started.
        // We don't have to lock the mutext because entering this worker
        // function itself has already guaranteed the condition has been met.
        m_cv_thrd_started.notify_one();

        int n = 0;

        while (true)
        {
            std::unique_lock lock(m_mtx_flag_produce_or_quit);
            if (!m_flag_produce)
            {
                m_cv_produce_or_quit.wait(lock);
            }

            if (m_flag_quit)
            {
                break;
            }

            // Produce data for all the callbacks.
            std::lock_guard g(m_mtx_callbacks);
            for (auto cb : m_callbacks)
            {
                cb.second(cb.first, n);
            }
            n++;
        }
    }

    std::mutex m_mtx_cv_thrd_started;
    std::condition_variable m_cv_thrd_started;

    std::mutex m_mtx_flag_produce_or_quit;
    std::condition_variable m_cv_produce_or_quit;
    bool m_flag_produce;
    bool m_flag_quit;

    std::mutex m_mtx_callbacks;
    std::vector<CallbackEntry> m_callbacks;

    std::thread m_thread;
};

class DataConsumer
{
public:
    static void
    cb(DataConsumer *c, int n)
    {
        c->addData(n);
    }

    DataConsumer() {}

    void
    addData(int d)
    {
        std::lock_guard<std::mutex> g(m_mtx_data);
        m_data.push_back(d);
    }

    bool
    empty() const
    {
        std::lock_guard<std::mutex> g(m_mtx_data);
        return m_data.empty();
    }

    void
    getData(std::vector<int> &data) const
    {
        std::lock_guard<std::mutex> g(m_mtx_data);
        data = m_data;
    }

private:
    mutable std::mutex m_mtx_data;
    std::vector<int> m_data;
};

TEST(ThreadTest, c)
{
    using namespace std::chrono_literals;

    DataProducer prod;
    DataConsumer cons;

    prod.addCallback(&cons, DataConsumer::cb);

    // Wait until the producer's worker thread has been started.
    prod.waitUntilThreadStarted();

    // We wait for 1 second to give the producer thread enough time to run.
    // If the producer thread is not implemented correctly, it may have produced
    // some data that we can catch in the next EXPECT assertion.
    std::this_thread::sleep_for(1s);

    // Before we start, the consumer has received nothing.
    EXPECT_TRUE(cons.empty());

    // Now we start the producer and wait for some time. During this time, the
    // producer should be able to produce many data.
    prod.start();
    std::this_thread::sleep_for(50ms);
    prod.stop();

    // After we stop, the consumer should receive some data.
    std::vector<int> data;
    cons.getData(data);
    EXPECT_GT(data.size(), 1U);

    std::cout << "data.size() = " << data.size() << std::endl;
    for (size_t i = 0; i < data.size(); ++i)
    {
        EXPECT_EQ(data[i], static_cast<int>(i));
    }

    // Quit the producer.
    prod.quit();

    prod.waitUntilThreadStopped();
}
