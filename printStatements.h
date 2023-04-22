//
// Created by DELL on 12/04/2023.
//

#ifndef HW1WET_PRINTSTATEMENTS_H
#define HW1WET_PRINTSTATEMENTS_H


#include <string.h>
#include <iostream>

namespace GeneralStatements
{
    void printGeneralErrorToCERR(const std::string &command_line);
    void printTooManyArguments(const std::string &command_name);
    void printOldPwdNotSet(const std::string &command_name);
    void printSendingSigKill(unsigned long num);
    void printKillJobMessage(pid_t pid, const std::string &command_name);
    void printInvalidArgs(const std::string &command_name);
    void printJobNotExist(const std::string& command_name, int job_id);
    void printSignalSend(int sig_num, int job_pid);
    void printSmashPID(pid_t pid);
    void printJobForJobsCMD(int job_id, const std::string& cmd_line, pid_t pid, double seconds, const std::string& stopped_print);
    void printEmptyJobsList(const std::string& command_name);
    void printJobDetails(pid_t pid, const std::string& command_line);
    void printCurrentDir(const std::string& current_dir);
    void printJobAlreadyRunning(int job_id);
    void printNoJobsToResume(const std::string& command_name);
    void printGotCtrlC();
    void printGotCtrlZ();
    void printGotAlarm();
    void printProcessWasKilled(pid_t pid);
    void printProcessWasStopped(pid_t pid);
    void printTimedOut(const std::string& command_name, int time);
    void printInvalidCoreNumber();
    void printFileInfo(const std::string& path, const std::string& filetype, long size);
}
/*

void bgfgPrint(const std::string& command_name, int pid);



*/


#endif //HW1WET_PRINTSTATEMENTS_H
