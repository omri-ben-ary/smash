#include <iostream>
#include <csignal>
#include "signals.h"
#include "Commands.h"

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
        JobEntry* job = smash.getJobsListPtr()->getJobByPid(pid);
        if (job != nullptr)
        {
            // following scenario occurred: 1.run in bg with & 2.fg command 3.ctrl-z
            // and therefore the job is already in the jobs list
            job->restartJobTime();
            job->stopJob();
        }
        else
        {
            // job is not in the jobs list and therefore we should add him
            smash.getJobsListPtr()->addJob(pid, cmd, true);
        }

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
    printGotAlarm();
    SmallShell& smash = SmallShell::getInstance();
    smash.getJobsListPtr()->removeFinishedJobs();
    smash.getTimeoutJobsListPtr()->removeTimedOutJobs();
    smash.setNextAlarm();
}

