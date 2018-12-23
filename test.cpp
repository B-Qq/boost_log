#include "log.h"
#include "Hello.h"
int main(int, char *[])
{
    //boost::shared_ptr<sink_t> sink = init_logging();
    InitLog();
    src::severity_logger_mt<sign_severity_level>& lg = my_logger::get();
    Hello s;
    s.PrintfHello();
    //src::severity_channel_logger_mt<severity_level> lg(keywords::channel = "net");
    BOOST_LOG_SEV(lg, info) << "Hello worldinfo!";
    BOOST_LOG_SEV(lg, debug) << "Hello worlddebug!";
    BOOST_LOG_SEV(lg, warning) << "Hello world!";
    BOOST_LOG_SEV(lg, warning) << "Hello world!";
    BOOST_LOG_SEV(lg, warning) << "Hello world!";
    BOOST_LOG_SEV(lg, error) << "Hello world!";
    BOOST_LOG_SEV(lg, warning) << "Hello world!";
    BOOST_LOG_SEV(lg, warning) << "Hello world!";
    BOOST_LOG_SEV(lg, warning) << "Hello world!";
    BOOST_LOG_SEV(lg, warning) << "Hello world!";
    while(1)
    {
        BOOST_LOG_SEV(lg, warning) << "Hello world11111!";
        sleep(2);
    }
    logging::core::get()->remove_all_sinks();
    return 0;
}

