void save_job(pid_t pid);
int get_jid(pid_t pid);
void release_job(pid_t pid);
void print_jobs();
void save_job_cmd(pid_t pid, char *argv[], int bg);
void resume_bg_job(char **argv);
void resume_fg_job(char **argv);
void terminate_fg();
void stop_fg();
void reap_terminated_children();
void reap_all_children();
