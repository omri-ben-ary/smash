#include <unistd.h>
#include <cstring>
#include <iostream>
#include <utility>
#include <vector>
#include <sstream>
#include <sys/wait.h>
#include <iomanip>
#include <algorithm>
#include <fcntl.h>
#include "Commands.h"
#include "signals.h"
#include "printStatements.h"

using namespace std;
using namespace GeneralStatements;
const string WHITESPACE = " \n\r\t\f\v";

#if 0
#define FUNC_ENTRY()  \
  cout << __PRETTY_FUNCTION__ << " --> " << endl;

#define FUNC_EXIT()  \
  cout << __PRETTY_FUNCTION__ << " <-- " << endl;
#else
#define FUNC_ENTRY()
#define FUNC_EXIT()
#endif

namespace SyscallErrorMessages
{
    static const char* const CWD_FAILED = "smash error: getcwd failed" ;
    static const char* const CHDIR_FAILED = "smash error: chdir failed";
    static const char* const KILL_FAILED = "smash error: kill failed" ;
    static const char *FORK_FAILED = "smash error: fork failed";
    static const char *EXECVP_FAILED = "smash error: execvp failed";
    static const char *EXECV_FAILED = "smash error: execvp failed";
    static const char *WAITPID_FAILED = "smash error: waitpid failed";
    static const char *TIME_ERROR = "smash error: time failed";
    static const char *SETPGRP_FAILED = "smash error: setpgrp failed";
    static const char *OPEN_FAILED = "smash error: open failed";
    static const char *DUP_FAILED = "smash error: dup failed";
    static const char *CLOSE_FAILED = "smash error: close failed";
    /*
    static const char *SEEK_FAILED = "smash error: seek failed";
    static const char *READ_ERROR = "smash error: read failed";
    static const char *TOUCH_ERROR = "smash error: utime failed";
    static const char *OPEN_FAILED = "smash error: open failed";
     */
}

string _ltrim(const string& s)
{
    size_t start = s.find_first_not_of(WHITESPACE);
    return (start == string::npos) ? "" : s.substr(start);
}

string _rtrim(const string& s)
{
    size_t end = s.find_last_not_of(WHITESPACE);
    return (end == string::npos) ? "" : s.substr(0, end + 1);
}

string _trim(const string& s)
{
    return _rtrim(_ltrim(s));
}

int _parseCommandLine(const char* cmd_line, char** args) {
    FUNC_ENTRY()
    int i = 0;
    std::istringstream iss(_trim(string(cmd_line)).c_str());
    for(string s; iss >> s; ) {
        args[i] = (char*)malloc(s.length()+1);
        memset(args[i], 0, s.length()+1);
        strcpy(args[i], s.c_str());
        args[++i] = NULL;
    }
    return i;

    FUNC_EXIT()
}

bool _isBackgroundComamnd(const char* cmd_line) {
    const string str(cmd_line);
    return str[str.find_last_not_of(WHITESPACE)] == '&';
}

void _removeBackgroundSign(char* cmd_line) {
    const string str(cmd_line);
    // find last character other than spaces
    unsigned int idx = str.find_last_not_of(WHITESPACE);
    // if all characters are spaces then return
    if (idx == string::npos) {
        return;
    }
    // if the command line does not end with & then return
    if (cmd_line[idx] != '&') {
        return;
    }
    // replace the & (background sign) with space and then remove all tailing spaces.
    cmd_line[idx] = ' ';
    // truncate the command line string up to the last non-space character
    cmd_line[str.find_last_not_of(WHITESPACE, idx) + 1] = 0;
}

// TODO: Add your implementation for classes in Commands.h
/* ~~~~~~~~~~~~~~~~~~~~ SMASH methods implementation ~~~~~~~~~~~~~~~~~~~"*/

const char* const SmallShell::DEFAULT_PROMPT = "smash";     //why not in h file?

bool isRedirection(std::string cmd_line)
{
    return (cmd_line.find('>') != std::string::npos);
}

bool isPipe(std::string cmd_line)
{
    return (cmd_line.find('|') != std::string::npos);
}

SmallShell::SmallShell() : current_prompt(DEFAULT_PROMPT), last_working_directory()
{
    is_shell_in_fg = true;
    jobs_list = new JobsList;
    fg_process = nullptr;
// TODO: add your implementation
    updateCurrentWorkingDirectory();
}

SmallShell::~SmallShell() {
    delete jobs_list;
    delete fg_process; // deleting nullptr has no effect

// TODO: add your implementation
}


/**
* Creates and returns a pointer to Command class which matches the given command line (cmd_line)
*/

Command * SmallShell::CreateCommand(const char* cmd_line)
{
    string cmd_s = _trim(string(cmd_line));
    string firstWord = cmd_s.substr(0, cmd_s.find_first_of(" \n"));
    this->jobs_list->removeFinishedJobs();
    if(isRedirection(cmd_s))
    {
        return new RedirectionCommand(cmd_line);
    }
    if(isPipe(cmd_s))
    {
        //return new PipeCommand(cmd_line);
    }
    if ((firstWord == "chprompt") || (firstWord == "chprompt&"))
    {
        return new ChangePromptCommand(cmd_line);
    }
    else if ((firstWord == "showpid") || (firstWord == "showpid&"))
    {
        return new ShowPidCommand(cmd_line);
    }
    else if ((firstWord == "pwd") || (firstWord == "pwd&"))
    {
        return new GetCurrDirCommand(cmd_line);
    }
    else if ((firstWord == "cd") || (firstWord == "cd&"))
    {
        return new ChangeDirCommand(cmd_line);
    }
    else if ((firstWord == "jobs") || (firstWord == "jobs&"))
    {
        return new JobsCommand(cmd_line, jobs_list);
    }
    else if ((firstWord == "fg") || (firstWord == "fg&"))
    {
        return new ForegroundCommand(cmd_line, jobs_list);
    }
    else if ((firstWord == "bg") || (firstWord == "bg&"))
    {
        return new BackgroundCommand(cmd_line, jobs_list);
    }
    else if ((firstWord == "quit") || (firstWord == "quit&"))
    {
        return new QuitCommand(cmd_line, jobs_list);
    }
    else if ((firstWord == "kill") || (firstWord == "kill&"))
    {
        return new KillCommand(cmd_line, jobs_list);
    }
    else
    {
        return new ExternalCommand(cmd_line);
    }
}

void SmallShell::executeCommand(const char *cmd_line)
{
    Command* cmd = CreateCommand(cmd_line);
    jobs_list->removeFinishedJobs();
    cmd->execute();
}

string SmallShell::getCurrentPrompt() const
{
    return this->current_prompt;
}

void SmallShell::setCurrentPrompt(const string& prompt)
{
    this->current_prompt.assign(prompt);
}

string SmallShell::getDefaultPrompt() {
    return DEFAULT_PROMPT;
}

string SmallShell::getCurrentWorkingDirectory() const {
    return this->current_working_directory;
}

void SmallShell::updateCurrentWorkingDirectory()
{
    char cwd[COMMAND_ARGS_MAX_LENGTH];
    if (getcwd(cwd, sizeof(cwd)) == nullptr)
    {
        perror(SyscallErrorMessages::CWD_FAILED);
        return;
    }
    this->current_working_directory.assign(cwd);
}

string SmallShell::getLastWorkingDirectory() const {
    return this->last_working_directory;
}

void SmallShell::updateLastWorkingDirectory() {
    // in case we need to change only when they are different, a simple 'if' will resolve it
    this->last_working_directory = this->getCurrentWorkingDirectory();
}

JobsList* SmallShell::getJobsListPtr() {
    return this->jobs_list;
}

void SmallShell::loadFGProcess(pid_t pid, const std::string& command)
{
    this->is_shell_in_fg = false;
    this->fg_process = new ForegroundProcess(pid, command);
}

pid_t SmallShell::getFGProcessPID()
{
    return this->fg_process->getFGProcessPID();
}

std::string SmallShell::getFGProcessCMD()
{
    return this->fg_process->getFGProcessCMD();
}

bool SmallShell::killFGProcess()
{
    return (this->fg_process->sendSignal(SIGKILL));
}


bool SmallShell::stopFGProcess()
{
    return (this->fg_process->sendSignal(SIGSTOP));
}

void SmallShell::removeFGProcess()
{
    this->is_shell_in_fg = true;
    delete fg_process;
    this->fg_process = nullptr;
}

bool SmallShell::isShellInFG() const
{
    return this->is_shell_in_fg;
}

void SmallShell::updateJobList() {
    jobs_list->removeFinishedJobs();
}


/* ~~~~~~~~~~~~~~~~~~~~ Built-in Commands methods implementation ~~~~~~~~~~~~~~~~~~~"*/

Command::Command(const char *cmd_line) : cmd_line(cmd_line){
}

const char *Command::getCmdLine() const
{
    return this->cmd_line;
}

bool Command::isValidInt(int int_to_check, const char *str_before_convert)
{
    string str_to_cmp = to_string(int_to_check);
    return (strcmp(str_to_cmp.c_str(), str_before_convert) == 0);
}

Command::~Command() = default; //???


BuiltInCommand::BuiltInCommand(const char *cmd_line) : Command(cmd_line) {
}

ChangePromptCommand::ChangePromptCommand(const char *cmd_line) : BuiltInCommand(cmd_line){
}

void ChangePromptCommand::execute()
{
    char *cmd_args[COMMAND_MAX_ARGS];
    int num_of_cmd_args = _parseCommandLine(this->getCmdLine(), cmd_args);
    string new_prompt = SmallShell::getInstance().getDefaultPrompt();
    if (num_of_cmd_args > 1)
    {
        new_prompt = cmd_args[1];
    }
    SmallShell::getInstance().setCurrentPrompt(new_prompt);
}

ShowPidCommand::ShowPidCommand(const char *cmd_line) : BuiltInCommand(cmd_line){
}

void ShowPidCommand::execute()
{
    // getpid is always successful
    pid_t pid = getpid();
    printSmashPID(pid);
}

GetCurrDirCommand::GetCurrDirCommand(const char *cmd_line) : BuiltInCommand(cmd_line){
}

void GetCurrDirCommand::execute()
{
    printCurrentDir(SmallShell::getInstance().getCurrentWorkingDirectory());
}

ChangeDirCommand::ChangeDirCommand(const char *cmd_line) : BuiltInCommand(cmd_line){
}

void ChangeDirCommand::execute()
{
    char *cmd_args[COMMAND_MAX_ARGS];
    int num_of_cmd_args = _parseCommandLine(this->getCmdLine(), cmd_args);
    SmallShell& smash = SmallShell::getInstance();
    if (num_of_cmd_args == 1)
    {
        printGeneralErrorToCERR(this->getCmdLine());
        return;
    }
    else if (num_of_cmd_args > 2)
    {
        printTooManyArguments("cd");
        return;
    }
    else
    {
        const char *new_path = cmd_args[1];
        string last_working_dir = smash.getLastWorkingDirectory();
        if (strcmp(new_path, "-") == 0)
        {
            // cd argument is '-'
            if (last_working_dir.empty())
            {
                printOldPwdNotSet("cd");
                return;
            }
            new_path = last_working_dir.c_str();
        }
        if (chdir(new_path) == -1)
        {
            perror(SyscallErrorMessages::CHDIR_FAILED);
            return;
        }
        smash.updateLastWorkingDirectory();
        smash.updateCurrentWorkingDirectory();
    }
}

JobsCommand::JobsCommand(const char *cmd_line, JobsList *jobs) : BuiltInCommand(cmd_line), jobs(jobs) {
}

void JobsCommand::execute()
{
    jobs->removeFinishedJobs();
    jobs->printJobsList();
}

ForegroundCommand::ForegroundCommand(const char* cmd_line, JobsList* jobs) : BuiltInCommand(cmd_line), jobs(jobs){
}

void ForegroundCommand::execute() {
    // parse command line and check that Command format is "fg [job_id]"
    char *cmd_args[COMMAND_MAX_ARGS];
    int num_of_cmd_args = _parseCommandLine(this->getCmdLine(), cmd_args);

    if (num_of_cmd_args > 2) {
        printInvalidArgs("fg");
        return;
    }
    int job_id;
    if (num_of_cmd_args == 2) {
        try {
            job_id = stoi(string(cmd_args[1]));
            if (!(isValidInt(job_id, cmd_args[1]))) {
                printInvalidArgs("fg");
                return;
            }
        } catch (std::exception &e) {
            printInvalidArgs("fg");
            return;
        }
        if (job_id <= 0) {
            printInvalidArgs("fg");
            return;
        }
        // remove finished jobs
        jobs->removeFinishedJobs();
    } else {
        // remove finished jobs
        jobs->removeFinishedJobs();
        job_id = jobs->getMaxJobID();
    }

    // remove finished jobs

    JobEntry *requested_job;
    // get job Entry
    if (num_of_cmd_args == 1) {
        requested_job = jobs->getLastJob();
        if (requested_job == nullptr) {
            printEmptyJobsList("fg");
            return;
        }
    } else // 2 cmd line args
    {
        requested_job = jobs->getJobById(job_id);
        if (requested_job == nullptr) {
            printJobNotExist("fg", job_id);
            return;
        }
    }

    // print job entry
    pid_t c_pid = requested_job->getJobPID();
    printJobDetails(c_pid, requested_job->getJobCMD());

    // execute in fg
    SmallShell& smash = SmallShell::getInstance();
    smash.loadFGProcess(c_pid, requested_job->getJobCMD());
    jobs->sendSignalToJob(job_id, SIGCONT, false);
    jobs->removeFromListJobById(job_id);
    waitpid(c_pid, nullptr, WUNTRACED);
    smash.removeFGProcess();

    // remove job from JobsList
    jobs->removeFinishedJobs();
}

BackgroundCommand::BackgroundCommand(const char* cmd_line, JobsList* jobs) : BuiltInCommand(cmd_line), jobs(jobs){
}

void BackgroundCommand::execute()
{
    // parse command line and check that Command format is "bg [job_id]"
    char *cmd_args[COMMAND_MAX_ARGS];
    int num_of_cmd_args = _parseCommandLine(this->getCmdLine(), cmd_args);

    if (num_of_cmd_args > 2) {
        printInvalidArgs("bg");
        return;
    }
    int job_id;
    if (num_of_cmd_args == 2)
    {
        try
        {
            job_id = stoi(string(cmd_args[1]));
            if (!(isValidInt(job_id, cmd_args[1])))
            {
                printInvalidArgs("bg");
                return;
            }
        } catch (std::exception &e) {
            printInvalidArgs("bg");
            return;
        }
        if (job_id <= 0)
        {
            printInvalidArgs("bg");
            return;
            jobs->removeFinishedJobs();
        }
    }
    else
    {
        // remove finished jobs
        jobs->removeFinishedJobs();
        job_id = jobs->getMaxJobID();
    }
    // get job Entry
    JobEntry *requested_job = nullptr;

    if (num_of_cmd_args == 1)
    {
        requested_job = jobs->getLastStoppedJob();
        if (requested_job == nullptr)
        {
            printNoJobsToResume("bg");
            return;
        }
    }
    else // 2 cmd line args
    {
        requested_job = jobs->getJobById(job_id);
        if (requested_job == nullptr)
        {
            printJobNotExist("bg", job_id);
            return;
        }
        if (!(requested_job->isJobStopped()))
        {
            printJobAlreadyRunning(job_id);
            return;
        }
    }

    // print job entry
    pid_t c_pid = requested_job->getJobPID();
    printJobDetails(c_pid, requested_job->getJobCMD());

    // execute in bg
    jobs->sendSignalToJob(job_id, SIGCONT, false);

    // move job status to bg
    requested_job->moveJobToBG();
}

QuitCommand::QuitCommand(const char* cmd_line, JobsList* jobs) : BuiltInCommand(cmd_line), jobs(jobs) {}
// need to think if we want a copy of the job list or the job list itself.
// shafran made a copy. it seems as the right thing to do because if
// we quit the program we need to kill all the jobs and not their copy
// point to the main jobs list who is pointed by the shell

void QuitCommand::execute()
{
    char *cmd_args[COMMAND_MAX_ARGS];
    int num_of_cmd_args = _parseCommandLine(this->getCmdLine(), cmd_args);
    if ((num_of_cmd_args > 1) && (strcmp(cmd_args[1],"kill") == 0))// cmd line: quit
    {
        jobs->killAllJobs();
    }
    // this syscall cannot fail
    exit(0);
}

KillCommand::KillCommand(const char * cmd_line, JobsList* jobs) : BuiltInCommand(cmd_line), jobs(jobs) {}
// point to the main jobs pointer (who is pointed by the shell)

void KillCommand::execute()
{
    char *cmd_args[COMMAND_MAX_ARGS];
    int num_of_cmd_args = _parseCommandLine(this->getCmdLine(), cmd_args);
    if (num_of_cmd_args != 3)
    {
        printInvalidArgs("kill");
        return;
    }
    int sig_num, job_id;
    try
    {
        sig_num = abs(stoi(string(cmd_args[1])));
        job_id = stoi(string(cmd_args[2]));
        if (!(isValidInt(-sig_num, cmd_args[1])) || !(isValidInt(job_id, cmd_args[2])))
        {
            printInvalidArgs("kill");
            return;
        }
    } catch (std::exception& e){
        printInvalidArgs("kill");
        return;
    }
    if ((sig_num > BIGGEST_SIG_NUM) || (sig_num < SMALLEST_SIG_NUM)){
        printInvalidArgs("kill");
        return;
    }
    // Command format is "kill -<signum> <jobid>"
    if (!(jobs->isJobExist(job_id)))
    {
        printJobNotExist("kill",job_id);
        return;
    }
    jobs->sendSignalToJob(job_id, sig_num, true);

}

//TODO
RedirectionCommand::RedirectionCommand(const char *cmd_line) : Command(cmd_line), command(), filename(), toAppend(false) {
    std::string str_cmd_line= string(cmd_line);
    if(str_cmd_line.find(">>") != std::string::npos)
    {
        toAppend = true;
    }
    size_t separate = str_cmd_line.find('>');
    command = str_cmd_line.substr(0, separate);
    command = command.substr(0, str_cmd_line.find_first_of(' '));
    filename = str_cmd_line.substr(separate);
    filename = filename.substr(filename.find_first_not_of(" >"));
    filename = filename.substr(0, filename.find_first_of(" \n"));
}

void RedirectionCommand::execute() {
    int fd;
    if(toAppend)
    {
        fd = open(filename.c_str(), O_RDWR | O_CREAT | O_APPEND, 0655);
    }
    else
    {
        fd = open(filename.c_str(), O_RDWR | O_CREAT | O_TRUNC, 0655);
    }
    if(fd == -1)
    {
        perror(SyscallErrorMessages::OPEN_FAILED);
        return;
    }
    pid_t c_pid = fork();
    if (c_pid == -1)
    {
        close(fd);  //what to do if close fails??
        perror(SyscallErrorMessages::FORK_FAILED);
        return;
    }
    if(c_pid == 0)
    {
        if (setpgrp() == -1)
        {
            perror(SyscallErrorMessages::SETPGRP_FAILED);
            return;
        }
        SmallShell& smash = SmallShell::getInstance();
        if(dup2(STDOUT_FILENO,fd) == -1)
        {
            perror(SyscallErrorMessages::DUP_FAILED);
            return;
        }
        cout << "dup" << std::endl;
        Command* cmd = smash.CreateCommand(command.c_str());
        smash.updateJobList();
        cmd->execute();
        if(close(fd) == -1)
        {
            perror(SyscallErrorMessages::CLOSE_FAILED);
            return;
        }
        exit(0);
    }
    else
    {
        wait(nullptr);
        /*int wstatus;
        SmallShell& smash = SmallShell::getInstance();
        smash.loadFGProcess(c_pid, this->getCmdLine());
        if (waitpid(c_pid, &wstatus, WUNTRACED) == -1) // maybe can delete wstatus
        {
            perror(SyscallErrorMessages::WAITPID_FAILED);
            return;
        }
        smash.removeFGProcess();*/
        if(close(fd) == -1)
        {
            perror(SyscallErrorMessages::CLOSE_FAILED);
            return;
        }
    }
}

/* ~~~~~~~~~~~~~~~~~~~~ External Commands methods implementation ~~~~~~~~~~~~~~~~~~~"*/

ExternalCommand::ExternalCommand(const char* cmd_line) : Command(cmd_line){}

bool ExternalCommand::isComplexExternalCommand()
{
    char* cmd_args[COMMAND_MAX_ARGS];
    int num_of_cmd_args = _parseCommandLine(this->getCmdLine(), cmd_args);
    std::size_t found = string::npos;
    for (int i = 0; i < num_of_cmd_args ; i++)
    {
        found = string(cmd_args[i]).find_first_of("*?");
        if (found != string::npos)
        {
            return true;
        }
    }
    return (found!= string::npos);
}

void ExternalCommand::executeExternalCommandInFG()
{
    int wstatus; // might delete
    SmallShell& smash = SmallShell::getInstance();
    pid_t c_pid = fork();
    if (c_pid == -1)
    {
        perror(SyscallErrorMessages::FORK_FAILED);
        return;
    }
    else if (c_pid == 0)
    { // son
        if (setpgrp() == -1)
        {
            perror(SyscallErrorMessages::SETPGRP_FAILED);
            return;
        }
        if (isComplexExternalCommand())
        {
            char arg1[] = "/bin/bash";
            char arg2[] = "-c";
            string cmd = getCmdLine();
            char* arg3 = &(cmd[0]);
            char* args[] = {arg1, arg2, arg3, nullptr};
            if (execv(arg1, args) == -1)
            {
                perror(SyscallErrorMessages::EXECV_FAILED);
                exit(0); // we don't want the child to keep running
            }
        }
        else
        {
            char* cmd_args[COMMAND_MAX_ARGS];
            _parseCommandLine(this->getCmdLine(), cmd_args);
            if (execvp(cmd_args[0], cmd_args) == -1)
            {
                perror(SyscallErrorMessages::EXECVP_FAILED);
                exit(0);
            }
        }
    }
    else
    {
        smash.loadFGProcess(c_pid, this->getCmdLine());
        if (waitpid(c_pid, &wstatus, WUNTRACED) == -1) // maybe can delete wstatus
        {
            perror(SyscallErrorMessages::WAITPID_FAILED);
            return;
        }
        smash.removeFGProcess();

    }
}

void ExternalCommand::executeExternalCommandInBG()
{
    string cmd = getCmdLine();
    char* arg3 = &(cmd[0]);
    _removeBackgroundSign(arg3);
    int wstatus; // might delete
    pid_t c_pid = fork();
    if (c_pid == -1)
    {
        perror(SyscallErrorMessages::FORK_FAILED);
        return;
    }
    else if (c_pid == 0)
    { // son
        if (setpgrp() == -1)
        {
            perror(SyscallErrorMessages::SETPGRP_FAILED);
            return;
        }
        if (isComplexExternalCommand())
        {
            char arg1[] = "/bin/bash";
            char arg2[] = "-c";
            char* args[] = { arg1, arg2, arg3, nullptr};
            if (execv(arg1, args) == -1)
            {
                perror(SyscallErrorMessages::EXECV_FAILED);
                exit(0); // we don't want the child to keep running
            }
        }
        else
        {
            char* cmd_args[COMMAND_MAX_ARGS];
            _parseCommandLine(arg3, cmd_args);
            if (execvp(cmd_args[0], cmd_args) == -1)
            {
                perror(SyscallErrorMessages::EXECVP_FAILED);
                exit(0);
            }
        }
    }
    else
    {
        string original_cmd = getCmdLine();
        SmallShell::getInstance().getJobsListPtr()->addJob(c_pid, original_cmd, false);
    }
}
void ExternalCommand::execute()
{

    if (_isBackgroundComamnd(this->getCmdLine()))
    {
        executeExternalCommandInBG();
    }
    else
    {
        executeExternalCommandInFG();
    }

}
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ JobEntry methods ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

JobEntry::JobEntry(int id, int pid, string command, bool is_stopped)
        : job_id(id), job_pid(pid), job_cmd_line(std::move(command)),  is_stopped(is_stopped)
{

    time_t current_time = time(nullptr);
    if (current_time == -1)
    {
        perror(SyscallErrorMessages::TIME_ERROR);
        return;
    }
    this->job_time = current_time;
}

int JobEntry::getJobID() const {
    return this->job_id;
}

pid_t JobEntry::getJobPID() const {
    return this->job_pid;
}

string JobEntry::getJobCMD() {
    return this->job_cmd_line;
}

bool JobEntry::isJobStopped() const {
    return this->is_stopped;
}

time_t JobEntry::getJobTime() const {
    return this->job_time;
}

void JobEntry::moveJobToBG()
{
    this->is_stopped = false;
}

void JobEntry::killJob() {
    pid_t pid = this->getJobPID();
    string job_command_line = this->getJobCMD();
    if (kill(pid, SIGKILL) != 0)
    {
        perror(SyscallErrorMessages::KILL_FAILED);
        return;
    }
    printKillJobMessage(pid, job_command_line);
}

void JobEntry::sendSignal(int sig_num, bool to_print) const{
    pid_t pid = this->getJobPID();
    if (kill(pid, sig_num) != 0)
    {
        perror(SyscallErrorMessages::KILL_FAILED);
        return;
    }
    if (to_print)
    {
        printSignalSend(sig_num, pid);
    }

}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ JobList methods ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

JobsList::JobsList() : jobs_vector(), max_job_id(0), max_stopped_job_id(0){}

void JobsList::addJob(int pid, string cmd_line, bool is_stopped)
{
    removeFinishedJobs();
    int job_id = this->getMaxJobID() + 1;
    auto* job = new JobEntry(job_id, pid, std::move(cmd_line), is_stopped);
    jobs_vector.push_back(job);
    this->setMaxJobID(job_id);
    if (is_stopped) this->setMaxStoppedJobID(job_id);
}

void JobsList::printJobsList()
{
    // jobs need to delete all finished jobs before calling that func
    for(const auto& job: jobs_vector) // we assume here that all the jobs are already in the bg. need to think
        // if we want to check if jobs are bg during the loop (like shafran)
    {
        time_t current_time = time(nullptr);
        if (current_time == -1)
        {
            perror(SyscallErrorMessages::TIME_ERROR);
            return;
        }
        string stopped_print = (job->isJobStopped()) ? "(stopped)" : "";
        printJobForJobsCMD(job->getJobID(), job->getJobCMD(), job->getJobPID(),
                           difftime(current_time, job->getJobTime()), stopped_print);
    }
}

void JobsList::killAllJobs()
{
    printSendingSigKill(jobs_vector.size());
    for (JobEntry* job : jobs_vector)
    {
        job->killJob();
    }
}

void JobsList::removeFinishedJobs()
{
    std::vector<JobEntry *> jobs_to_remove;
    for (auto &job: jobs_vector)
    {
        if (isJobFinished(job))
        {
            jobs_to_remove.push_back(job);
        }
    }
    for (auto &stopped_job: jobs_to_remove) {
        jobs_vector.erase(std::remove(jobs_vector.begin(), jobs_vector.end(), stopped_job), jobs_vector.end());
    }
    this->updateMaxJobID();
    this->updateMaxStoppedJobID();
}

void JobsList::sendSignalToJob(int job_id, int sig_num, bool to_print)
{
    JobEntry* receiving_job = this->getJobById(job_id); // should we kill it in the end ?
    if (receiving_job == nullptr)
    {
        return;
    }
    receiving_job->sendSignal(sig_num, to_print);
}

bool JobsList::isJobExist(int jobId)
{
    auto it = std::find_if(jobs_vector.begin(), jobs_vector.end(),
                           [&jobId] (JobEntry* job) {return job->getJobID() ==  jobId;});
    return (it != jobs_vector.end());
}

JobEntry *JobsList::getJobById(int jobId)
{
    auto it = std::find_if(jobs_vector.begin(), jobs_vector.end(),
                           [&jobId] (JobEntry* job) {return job->getJobID() ==  jobId;});
    if (it != jobs_vector.end())
    {
        return *it;
    }
    return nullptr;
}

JobEntry* JobsList::getLastJob()
{
    if (!jobs_vector.empty())
    {
        return jobs_vector.back();
    }
    return nullptr;
}

JobEntry* JobsList::getLastStoppedJob()
{
    // can use this function just after we update the last stopped job id
    int job_id = getMaxStoppedJobID();
    if (job_id > 0)
    {
        return getJobById(job_id);
    }
    return nullptr;
}

int JobsList::getMaxJobID() const
{
    return this->max_job_id;
}

void JobsList::setMaxJobID(int job_id) //mostly in case when we add new job
{
    this->max_job_id = job_id;
}

int JobsList::getMaxStoppedJobID() const
{
    return this->max_stopped_job_id;
}

void JobsList::setMaxStoppedJobID(int job_id) //mostly in case when we add new job
{
    this->max_stopped_job_id = job_id;
}

void JobsList::updateMaxJobID() // mostly in case when we delete job
{
    // cant use getLastJob after this function and expect to get the last job
    // at first we should delete finished jobs.
    JobEntry* last_job = JobsList::getLastJob();
    if (last_job == nullptr)
    {
        setMaxJobID(0);
    }
    else
    {
        setMaxJobID(last_job->getJobID());
    }
}

void JobsList::updateMaxStoppedJobID() // mostly in case when we delete job
{
    for (auto it =  jobs_vector.rbegin(); it != jobs_vector.rend(); ++it)
    {
        if ((*it)->isJobStopped())
        {
            setMaxStoppedJobID((*it)->getJobID());
            return;
        }
    }
    setMaxStoppedJobID(0); // if jobs vector is empty or no stopped jobs
}

bool JobsList::isJobFinished(JobEntry* job)
{
    if (waitpid(job->getJobPID(), nullptr, WNOHANG) == 0)
    {
        return false;
    }
    else // res == -1 or res > 0returned pid
    {
        return true;
    }
}

void JobsList::removeFromListJobById(int jobId)
{
    auto it = std::find_if(jobs_vector.begin(), jobs_vector.end(),
                           [&jobId] (JobEntry* job) {return job->getJobID() ==  jobId;});
    if (it != jobs_vector.end())
    {
        jobs_vector.erase(it) ;
    }
    updateMaxJobID();
    updateMaxStoppedJobID();
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ ForegroundProcess methods ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ForegroundProcess::ForegroundProcess(int pid, std::string command)
        : process_pid(pid), process_cmd_line(std::move(command)){
}

pid_t ForegroundProcess::getFGProcessPID() const
{
    return this->process_pid;
}

string ForegroundProcess::getFGProcessCMD()
{
    return this->process_cmd_line;
}


bool ForegroundProcess::sendSignal(int sig_num) const
{
    pid_t pid = this->getFGProcessPID();
    if (kill(pid, sig_num) == -1)
    {
        perror(SyscallErrorMessages::KILL_FAILED);
        return false;
    }
    return true;
}
