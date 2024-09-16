#ifndef HW1WET_COMMAND_H_
#define HW1WET_COMMAND_H_

#include <vector>

#define COMMAND_ARGS_MAX_LENGTH (200)
#define COMMAND_MAX_ARGS (20)

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
    void printTimedOut(const std::string& command_name);
    void printInvalidCoreNumber();
    void printFileInfo(const std::string& path, const std::string& filetype, long size);
}

class Command{
    std::string cmd_line;
    bool is_timeout;
    int timeout_duration;
    std::string full_cmd_line;
    bool is_quit;
public:
    explicit Command(const char* cmd_line); //maybe we should change to std::string
    virtual ~Command();
    virtual void execute() = 0;
    std::string getCmdLine() const;
    void setTimeout();
    bool isTimeout();
    void setTimeoutDuration(int seconds);
    int getTimeoutDuration();
    void setFullPath(const std::string& full_path);
    std::string getFullPath();
    static bool isValidInt(int int_to_check, const char* str_before_convert);
    void setIsQuit() {is_quit = true;};
    bool getIsQuit() {return is_quit;};
};

class BuiltInCommand : public Command {
public:
    BuiltInCommand(const char* cmd_line);
    virtual ~BuiltInCommand() {}
};

class ExternalCommand : public Command {
public:
    ExternalCommand(const char* cmd_line);
    virtual ~ExternalCommand() {}
    void execute() override;
    bool isComplexExternalCommand();
    void executeExternalCommandInFG();
    void executeExternalCommandInBG();
    void executeSimpleExternalCommandInFG();
    void executeComplexExternalCommandInFG();
    void executeSimpleExternalCommandInBG();
    void executeComplexExternalCommandInBG();

};

class PipeCommand : public Command {
    std::string command1;
    std::string command2;
    bool toError;
public:
    PipeCommand(const char* cmd_line);
    virtual ~PipeCommand() {}
    void execute() override;
};

class RedirectionCommand : public Command {
    std::string command;
    std::string filename;
    bool toAppend{};
public:
    explicit RedirectionCommand(const char* cmd_line);
    ~RedirectionCommand() {}
    void execute() override;
    //void prepare() override;
    //void cleanup() override;
};

class ChangePromptCommand : public BuiltInCommand {
public:
    explicit ChangePromptCommand(const char* cmd_line);
    ~ChangePromptCommand() override = default; // should think about it
    void execute() override;
};

class ShowPidCommand : public BuiltInCommand {
public:
    explicit ShowPidCommand(const char* cmd_line);
    ~ShowPidCommand() override = default;
    void execute() override;
};

class GetCurrDirCommand : public BuiltInCommand {
public:
    explicit GetCurrDirCommand(const char* cmd_line);
    ~GetCurrDirCommand() override = default;
    void execute() override;
};

class ChangeDirCommand : public BuiltInCommand {
// TODO: Add your data members public:
public:
    explicit ChangeDirCommand(const char* cmd_line);
    ~ChangeDirCommand() override = default;
    void execute() override;
};

class JobsList;

class QuitCommand : public BuiltInCommand {
// TODO: Add your data members
    JobsList* jobs;
public:
    QuitCommand(const char* cmd_line, JobsList* jobs);
    virtual ~QuitCommand() {} // we dont want to delete the jobs - shell should do it
    void execute() override;
};

class KillCommand : public BuiltInCommand {
    // TODO: Add your data members
    JobsList* jobs;
public:
    KillCommand(const char* cmd_line, JobsList* jobs);
    virtual ~KillCommand() {} // we dont want to delete the jobs - shell should do it
    void execute() override;
};

class TimeoutJobEntry
{
private:
    const pid_t job_pid;
    int timeout_duration;
    std::string cmd_line;
    time_t job_time;
public:
    TimeoutJobEntry(int process_pid, int duration, std::string command);
    ~TimeoutJobEntry() = default;
    void restartJobTime();
    pid_t getJobPID() const;
    std::string getJobCMD();
    time_t getJobStartTime();
    int getSecondsUntilTimeout() const;
    bool is_job_timed_out() const;
};

class TimeoutJobsList
{
private:
    std::vector<TimeoutJobEntry *> timeout_jobs_vector;
public:
    TimeoutJobsList();
    ~TimeoutJobsList() = default;
    void addJob(int pid, int duration, std::string cmd_line);
    void removeTimedOutJobs();
    static bool isJobTimedOut(TimeoutJobEntry* job);
    static bool killTimedOutJob(pid_t pid);
    void sortTimeOutVector();
    int getTimeLeftToClosestAlarm();
    bool isEmptyList();
    void cleanTimeOutedJobsVector();
};

class JobEntry
{
private:
    const int job_id;
    const pid_t job_pid;
    std::string job_cmd_line;
    bool is_stopped;
    time_t job_time;
public:
    JobEntry(int job_id, int process_pid, std::string command, bool is_stopped = false);
    ~JobEntry() = default;
    int getJobID() const;
    pid_t getJobPID() const;
    std::string getJobCMD();
    bool isJobStopped() const;
    time_t getJobTime() const;
    void restartJobTime();
    void stopJob();
    void moveJobToBG();
    void killJob();
    void sendSignal(int sig_num, bool to_print) const;
};

class JobsList {
private:
    std::vector<JobEntry*> jobs_vector;
    int max_job_id; // at init - equals 0
    int max_stopped_job_id; // at init - equals 0

public:
    JobsList();
    ~JobsList() = default;
    void addJob(int pid, std::string cmd_line, bool is_stopped);
    void printJobsList();
    void killAllJobs();
    void removeFinishedJobs();
    bool isJobExist(int jobId);
    bool isJobExistByPID(pid_t pid);
    static bool isJobFinished(JobEntry* job);
    JobEntry* getJobById(int jobId); //returns nullptr if job not found
    JobEntry* getJobByPid(pid_t pid); //returns nullptr if job not found
    void removeFromListJobByPid(pid_t pid); //
    JobEntry* getLastJob(); // returns getJobById(this->max_job_id).
    // might be faster to call the last element in the vector if
    JobEntry* getLastStoppedJob(); // returns getJobById(this->max_stopped_job_id).
    void sendSignalToJob(int job_id, int sig_num, bool to_print);
    int getMaxJobID() const;
    void setMaxJobID(int job_id);
    int getMaxStoppedJobID() const;
    void setMaxStoppedJobID(int job_id);
    void updateMaxJobID();// updates this->max_job_id to be the id of the last element in the vector? (vector.back())
    void updateMaxStoppedJobID(); // updates this->max_job_id by reverse iterator who stops at first job with is_Stopped = true
    void cleanJobsVector();
};

class JobsCommand : public BuiltInCommand
{
    // TODO: Add your data members
    JobsList* jobs;
public:
    JobsCommand(const char* cmd_line, JobsList* jobs);
    virtual ~JobsCommand() {}
    void execute() override;
};

class ForegroundCommand : public BuiltInCommand
{
    JobsList* jobs;
public:
    ForegroundCommand(const char* cmd_line, JobsList* jobs);
    virtual ~ForegroundCommand() {}
    void execute() override;
};

class BackgroundCommand : public BuiltInCommand
{
    JobsList* jobs;
public:
    BackgroundCommand(const char* cmd_line, JobsList* jobs);
    ~BackgroundCommand() override = default;
    void execute() override;
};

class TimeoutCommand : public BuiltInCommand {
public:
    explicit TimeoutCommand(const char* cmd_line);
    virtual ~TimeoutCommand() {}
    void execute() override;
};

class ChmodCommand : public BuiltInCommand {
    int mode;
public:
    explicit ChmodCommand(const char* cmd_line);
    virtual ~ChmodCommand() {}
    void execute() override;
};

class GetFileTypeCommand : public BuiltInCommand {
    std::string filetype,path;
    long size;
    static std::string getFileTypeInternal(mode_t mode);
public:
    explicit GetFileTypeCommand(const char* cmd_line);
    virtual ~GetFileTypeCommand() {}
    void execute() override;
};

class SetcoreCommand : public BuiltInCommand {
    int job_id,core_num;
public:
    explicit SetcoreCommand(const char* cmd_line);
    virtual ~SetcoreCommand() {}
    void execute() override;
};

class ForegroundProcess
{
private:
    const pid_t process_pid;
    std::string process_cmd_line;
public:
    ForegroundProcess(int pid, std::string command);
    ~ForegroundProcess() = default;
    pid_t getFGProcessPID() const;
    std::string getFGProcessCMD();
    bool sendSignal(int sig_num) const;
};


class SmallShell {
private:
    // TODO: Add your data members
    SmallShell() ;
    static const char* const DEFAULT_PROMPT;
    std::string current_prompt;
    std::string last_working_directory;
    std::string current_working_directory;
    bool is_shell_in_fg;
    JobsList* jobs_list;
    ForegroundProcess* fg_process;
    TimeoutJobsList* timeout_jobs_list;

public:
    Command *CreateCommand(const char* cmd_line);
    SmallShell(SmallShell const&) = delete; // disable copy ctor
    SmallShell(SmallShell&) = delete; // disable copy ctor - gal edit
    void operator=(SmallShell const&)  = delete; // disable = operator
    void operator=(SmallShell&)  = delete; // gal - edit
    static SmallShell& getInstance() // make SmallShell singleton
    {
        static SmallShell instance; // Guaranteed to be destroyed.
        // Instantiated on first use.
        return instance;
    }
    ~SmallShell();
    void executeCommand(const char* cmd_line);
    // TODO: add extra methods as needed
    std::string getCurrentPrompt() const;
    void setCurrentPrompt(const std::string& new_prompt);
    static std::string getDefaultPrompt() ;
    std::string getCurrentWorkingDirectory() const;
    void updateCurrentWorkingDirectory();
    std::string getLastWorkingDirectory() const;
    void updateLastWorkingDirectory();
    JobsList* getJobsListPtr();
    void loadFGProcess(pid_t pid, const std::string& command);
    pid_t getFGProcessPID();
    std::string getFGProcessCMD();
    bool killFGProcess();
    bool stopFGProcess();
    void removeFGProcess();
    bool isShellInFG() const;
    void updateJobList();
    pid_t getJobPid(int job_id) const;
    TimeoutJobsList* getTimeoutJobsListPtr();
    void sortTimeOutVector();
    int getTimeLeftToClosestAlarm();
    void setNextAlarm();
    void cleanJobsVector();
    void cleanTimeOutedJobsVector();
};

#endif //HW1WET_COMMAND_H_
