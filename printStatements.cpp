//
// Created by DELL on 12/04/2023.
//

#include "printStatements.h"

namespace GeneralStatements
{
    void printGeneralErrorToCERR(const std::string &command_line)
    {
        std::cerr << "smash error:> \"" << command_line << "\"" << std::endl;
    }

    void printTooManyArguments(const std::string &command_name)
    {
        std::cerr << "smash error:> " << command_name << ": too many arguments" << std::endl;
    }

    void printOldPwdNotSet(const std::string &command_name)
    {
        std::cerr << "smash error: " << command_name << ": OLDPWD not set" << std::endl;
    }

    void printSendingSigKill(unsigned long num)
    {
        std::cout << "smash: sending SIGKILL signal to " << num << " jobs:" << std::endl;
    }

    void printKillJobMessage(pid_t pid, const std::string& command_line)
    {
        std::cout << pid << ": " << command_line << std::endl;
    }

    void printInvalidArgs(const std::string &command_name)
    {
        std::cerr << "smash error: " << command_name << ": invalid arguments" << std::endl;
    }

    void printJobNotExist(const std::string& command_name, int job_id)
    {
        std::cerr << "smash error: "<<command_name << ": job-id " << job_id <<" does not exist" << std::endl;
    }

    void printSignalSend(int sig_num, int job_pid)
    {
        std::cout << "signal number " << sig_num <<" was sent to pid " << job_pid << std::endl;
    }

    void printSmashPID(pid_t pid)
    {
        std::cout << "smash pid is " << pid << std::endl;
    }

    void printJobForJobsCMD(int job_id, const std::string& cmd_line, pid_t pid, double seconds,
                            const std::string& stopped_print)
    {
        std::cout << "[" << job_id << "] " << cmd_line << " : " << pid << " "
                  << seconds << " secs " << stopped_print << std::endl;
    }

    void printEmptyJobsList(const std::string& command_name)
    {
        std::cerr << "smash error: "<< command_name <<": jobs list is empty" << std::endl;
    }

    void printJobDetails(pid_t pid, const std::string& command_line)
    {
        std::cout << command_line << " : " << pid << std::endl;
    }

    void printCurrentDir(const std::string& current_dir)
    {
        std::cout << current_dir << std::endl;
    }

    void printJobAlreadyRunning(int job_id)
    {
        std::cerr << "smash error: bg: job-id "<< job_id <<" is already running in the background" << std::endl;
    }

    void printNoJobsToResume(const std::string& command_name)
    {
        std::cerr << "smash error: "<< command_name <<": there is no stopped jobs to resume" << std::endl;

    }

    void printGotCtrlC()
    {
        std::cout  << "smash: got ctrl-C"  << std::endl;
    }

    void printGotCtrlZ()
    {
        std::cout  << "smash: got ctrl-Z"  << std::endl;
    }

    void printGotAlarm()
    {
        std::cout  << "smash: got an alarm"  << std::endl;
    }

    void printProcessWasKilled(pid_t pid)
    {
        std::cout << "smash: process " << pid << " was killed" << std::endl;
    }

    void printProcessWasStopped(pid_t pid)
    {
        std::cout << "smash: process " << pid << " was stopped" << std::endl;
    }

    void printTimedOut(const std::string& command_name, int time)
    {
        std::cout << "smash: timeout "<< time << " " << command_name <<" timed out!" << std::endl;
    }
    void printInvalidCoreNumber()
    {
        std::cout << "smash error: setcore: invalid core number" << std::endl;
    }
    void printFileInfo(const std::string& path, const std::string& filetype, long size)
    {
        std::cout << path << "'s type is " << '"' << filetype << '"' << " and takes up " << size << " bytes" << std::endl;
    }
}/*



    void bgfgPrint(const std::string& command_name, int pid){
        std::cout << command_name << " : "<< pid << std::endl;
    }







*/
