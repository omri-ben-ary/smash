#include <iostream>
#include <csignal>
#include "signals.h"
#include "Commands.h"
#include "printStatements.h"

using namespace std;
using namespace GeneralStatements;


void ctrlCHandler(int sig_num)
{
    printGotCtrlC();
    SmallShell& smash = SmallShell::getInstance();
    if (!(smash.isShellInFG()))
    {
        // kill FG process
        if (smash.killFGProcess())
        {
            pid_t pid = smash.getFGProcessPID();
            printProcessWasKilled(pid);
            smash.removeFGProcess(); // maybe can be deleted
        }
    }
}

void ctrlZHandler(int sig_num)
{
    printGotCtrlZ();
    SmallShell& smash = SmallShell::getInstance();
    if (!(smash.isShellInFG()))
    {
        //add FG process to job list
        pid_t pid = smash.getFGProcessPID();
        std::string cmd = smash.getFGProcessCMD();
        smash.getJobsListPtr()->addJob(pid, cmd, true);
        //send SIGSTOP
        if (smash.stopFGProcess())
        {
            printProcessWasStopped(pid);
            smash.removeFGProcess(); // maybe can be deleted
        }

    }
}

void alarmHandler(int sig_num)
{
    /*
    printGotAlarm();
    // find which command caused the alarm
    // send SIGKILL to its process
    printTimedOut(cmd_line);*/
}

