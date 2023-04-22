#ifndef HW1WET_COMMAND_H_
#define HW1WET_COMMAND_H_

#include <vector>

#define COMMAND_ARGS_MAX_LENGTH (200)
#define COMMAND_MAX_ARGS (20)

class Command{
// TODO: Add your data members
    const char* cmd_line;
public:
    explicit Command(const char* cmd_line); //maybe we should change to std::string
    virtual ~Command();
    virtual void execute() = 0;
    //virtual void prepare();
    //virtual void cleanup();
    // TODO: Add your extra methods if needed
    const char* getCmdLine() const;
    static bool isValidInt(int int_to_check, const char* str_before_convert);
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
    static bool isJobFinished(JobEntry* job);
    JobEntry* getJobById(int jobId); //returns nullptr if job not found
    void removeFromListJobById(int jobId); //
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

// is the max_job_id equals to the vector size - if vector deletes element so no
// we have to notice that the job with the max id is always the last job in the vector
// fastest way to update maxStoppedJobID is by holding pointers between the stopped jobs.
// however it creates another problem: when we stop bg process (dont sure that it is even possible)
// we don't change the job place in the vector and we have to waste time searching for the next and previous stopped jobs
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
/* Bonus */
// TODO: Add your data members
public:
    explicit TimeoutCommand(const char* cmd_line);
    virtual ~TimeoutCommand() {}
    void execute() override;
};

class ChmodCommand : public BuiltInCommand {
    int mode;
    mode_t parseMode(int mode);
    mode_t parseFirstDigit(int digit);
    mode_t parseSecondDigit(int digit);
    mode_t parseThirdDigit(int digit);
    mode_t parseFourthDigit(int digit);
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
};

#endif //HW1WET_COMMAND_H_
