void save_job(pid_t pid);
int get_jid(pid_t pid);
void release_job(pid_t pid);
void print_jobs();
void save_job_cmd(pid_t pid, char *argv[]);
void print_finished_job(pid_t pid);
void terminate_fg();
void stop_fg();
