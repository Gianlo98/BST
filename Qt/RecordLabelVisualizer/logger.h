#ifndef LOGGER_H
#define LOGGER_H
#include <QString>

class LoggerOutput{
   public:
    virtual void logAppend(QString l) { Q_UNUSED(l) };
};

class Logger
{
    public:
        static Logger& getInstance(){
            static Logger    instance;   // Guaranteed to be destroyed
                                         // Instantiated on first use.
            return instance;
        }

        void connect(LoggerOutput* rl);
        void log(QString qs);
    private:
        Logger() {}
        LoggerOutput* rl;

    public:
        Logger(Logger const&)                = delete;
        void operator=(Logger const&)        = delete;

        // Note: Loggercott Meyers mentions in his Effective Modern
        //       C++ book, that deleted functions should generally
        //       be public as it results in better error messages
        //       due to the compilers behavior to check accessibility
        //       before deleted status
};

#endif // LOGGER_H
