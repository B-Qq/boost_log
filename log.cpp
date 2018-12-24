//
// Created by 边琪 on 2018-12-23.
//

#include "log.h"

std::ostream &operator<<(std::ostream &strm, sign_severity_level level)
{
    static const char *strings[] =
            {
                    "trace",
                    "debug",
                    "info",
                    "warning",
                    "error",
                    "fatal"
            };

    if (static_cast<std::size_t>(level) < sizeof(strings) / sizeof(*strings))
        strm << strings[level];
    else
        strm << static_cast<int>(level);

    return strm;
}

/*boost::shared_ptr<sink_t> init_logging()
{
    logging::add_common_attributes(); //日志属性初始化
    //日志输出到文件中
    logging::add_file_log(
            keywords::open_mode = std::ios::app,
            keywords::file_name = "../sample%N_20%y-%m-%d.log", //日志文件名字 以年月日的形式输出
            keywords::rotation_size = 5 * 1024 * 1024,//文件大小
            keywords::time_based_rotation = sinks::file::rotation_at_time_point(0, 0, 0),//每日零点新建文件
            keywords::format = expr::stream
                    << "["
                    << expr::format_date_time<boost::posix_time::ptime>("TimeStamp", "%Y-%m-%d %H:%M:%S.%f") << "] ["
                    << expr::attr<attrs::current_thread_id::value_type>("ThreadID") << ":" //线程号
                    << expr::attr<unsigned int>("LineID") << "]["
                    << expr::attr<std::string>("Process")
                    << "] [" << expr::attr<severity_level>("Severity") //日志等级
                    << "] "
                    << ":" << expr::smessage)->locked_backend()->auto_flush(true);

    boost::shared_ptr<logging::core> core = logging::core::get();
    boost::shared_ptr<sinks::text_ostream_backend> backend = boost::make_shared<sinks::text_ostream_backend>();
    boost::shared_ptr<sink_t> sink(new sink_t(backend));
    core->add_sink(sink);
    core->add_global_attribute("ThreadID", attrs::current_thread_id());
    core->add_global_attribute("Process", attrs::current_process_name());
    sink->set_filter(expr::attr<severity_level>("Severity") >= warning);
    sink->set_formatter(
            expr::stream
                    << "["
                    << expr::format_date_time<boost::posix_time::ptime>("TimeStamp", "%Y-%m-%d %H:%M:%S.%f") << "]["
                    << expr::attr<attrs::current_thread_id::value_type>("ThreadID") << ":"
                    << expr::attr<unsigned int>("LineID") << "]["
                    << expr::attr<std::string>("Process")
                    << "][" << expr::attr<severity_level>("Severity")
                    << "] "
                    << ":" << expr::smessage);
    {
        sink_t::locked_backend_ptr p = sink->locked_backend();//线程锁
        p->add_stream(boost::shared_ptr<std::ostream>(&std::clog, boost::null_deleter()));
    }
    return sink;
}

void stop_logging(boost::shared_ptr<sink_t> &sink)
{
    boost::shared_ptr<logging::core> core = logging::core::get();
    // Remove the sink from the core, so that no records are passed to it
    core->remove_sink(sink);
    // Break the feeding loop
    sink->stop();
    // Flush all log records that may have left buffered
    sink->flush();
    sink.reset();
}*/

//boost::shared_ptr<sink_t> init_logging()
void InitLog()
{
    boost::shared_ptr<logging::core> core = logging::core::get();

    typedef sinks::asynchronous_sink<sinks::text_file_backend, sinks::bounded_fifo_queue<1000, sinks::drop_on_overflow>> TextSink;//将日志输入到队列中,减少对程序的影响
    typedef sinks::asynchronous_sink<sinks::text_ostream_backend, sinks::bounded_fifo_queue<1000, sinks::drop_on_overflow>> OSink;//将日志输入到队列中,减少对程序的影响

    // init sink1
    boost::shared_ptr<sinks::text_file_backend> backend1 = boost::make_shared<sinks::text_file_backend>(
            keywords::file_name = "../shore_Power_%Y-%m-%d_%H-%M-%S.%N.log",
            keywords::rotation_size = 3 * 1024 * 1024,
            keywords::time_based_rotation = sinks::file::rotation_at_time_point(0, 0, 0),
            keywords::min_free_space = 30 * 1024 * 1024);
    backend1->auto_flush(true);
    boost::shared_ptr<TextSink> sink1(new TextSink(backend1));
    sink1->set_formatter(  //日志输出格式
            expr::format("[%1%] [ThreadID: %2%] [%3%]: %4%")
            //% expr::attr<unsigned int>("LineID") //日志行数
            % expr::format_date_time<boost::posix_time::ptime>("TimeStamp", "%Y-%m-%d %H:%M:%S")
            % expr::attr<attrs::current_thread_id::value_type>("ThreadID")
            % expr::attr<sign_severity_level>("Severity")//日志等级
            % expr::smessage
    );
    sink1->set_filter(expr::attr<sign_severity_level>("Severity") >= trace);
    core->add_sink(sink1);

    // init sink2
    boost::shared_ptr<sinks::text_ostream_backend> backend2 = boost::make_shared<sinks::text_ostream_backend>();
    backend2->auto_flush(true);
    boost::shared_ptr<OSink> sink2(new OSink(backend2));
    core->add_sink(sink2);
    sink2->set_filter(expr::attr<sign_severity_level>("Severity") >= trace);
    sink2->set_formatter(
            expr::format("[%1%] [ThreadID: %2%] [%3%]: %4%")
            % expr::format_date_time<boost::posix_time::ptime>("TimeStamp", "%Y-%m-%d %H:%M:%S")
            % expr::attr<attrs::current_thread_id::value_type>("ThreadID")
            % expr::attr<sign_severity_level>("Severity")//日志等级
            % expr::smessage);
    logging::add_common_attributes();
    {
        OSink::locked_backend_ptr p = sink2->locked_backend();//线程锁
        p->add_stream(boost::shared_ptr<std::ostream>(&std::clog, boost::null_deleter()));
    }
}

