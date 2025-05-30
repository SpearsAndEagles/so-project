#include "../helpers.h"
#define CMD_FILE "/tmp/treasure_cmd"
#define ARG_FILE "/tmp/treasure_arg"

#define IF_MONITOR_NOT_RUNNING()                    \
    if (!monitor_running)                           \
    {                                               \
        printf("Error: monitor is not running.\n"); \
    }

int monitor_running = 0;
pid_t monitor_pid = 0;
static volatile sig_atomic_t got_command = 0;
static int pipe_fds[2];

void sigchld_handler(int signum)
{
    (void)signum;
    int status;
    pid_t pid;
    while ((pid = waitpid(-1, &status, WNOHANG)) > 0)
    {
        if (pid == monitor_pid)
        {
            printf("Monitor child (PID %d) terminated with status %d.\n", pid, status);
            monitor_running = 0;
            monitor_pid = 0;
        }
    }
}

void setup_sigchld_handler()
{
    struct sigaction sa;
    sa.sa_handler = sigchld_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART;
    if (sigaction(SIGCHLD, &sa, NULL) < 0)
    {
        perror("sigaction SIGCHLD");
        _exit(1);
    }
}

void send_command(const char *cmd, const char *arg)
{
    printf("Sending command '%s' with arg '%s'\n", cmd, arg ? arg : "(null)");
    int fd;
    // Write command to CMD_FILE
    fd = open(CMD_FILE, O_WRONLY | O_CREAT | O_TRUNC, 0666);
    if (fd < 0)
    {
        perror("open CMD_FILE");
        return;
    }
    if (write(fd, cmd, strlen(cmd) + 1) < 0)
        perror("write CMD_FILE");
    close(fd);

    // Write arg if present
    if (arg)
    {
        fd = open(ARG_FILE, O_WRONLY | O_CREAT | O_TRUNC, 0666);
        if (fd < 0)
        {
            perror("open ARG_FILE");
            return;
        }
        if (write(fd, arg, strlen(arg) + 1) < 0)
            perror("write ARG_FILE");
        close(fd);
    }

    // Signal monitor
    if (kill(monitor_pid, SIGUSR1) < 0)
    {
        perror("kill SIGUSR1");
    }
    {
        char buf[256];
        ssize_t n;
        while ((n = read(pipe_fds[0], buf, sizeof(buf))) > 0)
        {
            if (write(STDOUT_FILENO, buf, n) < 0)
                perror("write to stdout");
        }
        if (n < 0 && errno != EAGAIN && errno != EWOULDBLOCK)
            perror("read pipe");
    }
}

void sigusr1_handler(int signum)
{
    (void)signum;
    got_command = 1;
}

void setup_sigusr1_handler()
{
    struct sigaction sa;
    sa.sa_handler = sigusr1_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART;
    if (sigaction(SIGUSR1, &sa, NULL) < 0)
    {
        perror("sigaction SIGUSR1");
        _exit(1);
    }
}

void process_command()
{
    char cmd_buf[256];
    char arg_buf[256];
    // Read command
    int fd = open(CMD_FILE, O_RDONLY);
    if (fd < 0)
    {
        perror("monitor open CMD_FILE");
        return;
    }
    ssize_t n = read(fd, cmd_buf, sizeof(cmd_buf));
    close(fd);
    if (n <= 0)
        return;

    // Try read arg
    fd = open(ARG_FILE, O_RDONLY);
    char *arg = NULL;
    if (fd >= 0)
    {
        ssize_t m = read(fd, arg_buf, sizeof(arg_buf));
        close(fd);
        if (m > 0)
            arg = arg_buf;
    }

    // Dispatch
    if (strcmp(cmd_buf, "list_hunts") == 0)
    {
        printf("[Monitor] Listing all hunts...\n");
        /* spawn treasure_manager list_hunts */
        {
            pid_t cpid = fork();
            if (cpid < 0)
            {
                perror("fork list_hunts");
            }
            else if (cpid == 0)
            {
                execlp("treasure_manager", "treasure_manager", "list_hunts", NULL);
                perror("execlp list_hunts");
                _exit(1);
            }
            else
            {
                waitpid(cpid, NULL, 0);
            }
        }
    }
    else if (strcmp(cmd_buf, "list_treasures") == 0)
    {
        printf("[Monitor] Listing treasures for hunt '%s'...\n", arg ? arg : "(none)");
        /* spawn treasure_manager list <hunt_id> */
        if (!arg)
        {
            printf("[Monitor] Error: no hunt_id provided for list_treasures.\n");
            return;
        }
        {
            pid_t cpid = fork();
            if (cpid < 0)
            {
                perror("fork list");
            }
            else if (cpid == 0)
            {
                execlp("treasure_manager", "treasure_manager", "list", arg, NULL);
                perror("execlp list");
                _exit(1);
            }
            else
            {
                waitpid(cpid, NULL, 0);
            }
        }
    }
    else if (strcmp(cmd_buf, "view_treasure") == 0)
    {
        if (!arg)
        {
            printf("[Monitor] Error: no arguments provided for view_treasure.\n");
            return;
        }
        // Parse two IDs from arg: <hunt_id> <treasure_id>
        char *hunt_id = strtok(arg_buf, " ");
        char *treasure_id = strtok(NULL, " ");
        if (!hunt_id || !treasure_id)
        {
            printf("[Monitor] Error: view_treasure requires <hunt_id> <treasure_id>.\n");
            return;
        }
        printf("[Monitor] Viewing treasure '%s' in hunt '%s'...\n", treasure_id, hunt_id);
        /* spawn treasure_manager view <hunt_id> <treasure_id> */
        {
            pid_t cpid = fork();
            if (cpid < 0)
            {
                perror("fork view");
            }
            else if (cpid == 0)
            {
                execlp("treasure_manager", "treasure_manager",
                       "view", hunt_id, treasure_id, NULL);
                perror("execlp view");
                _exit(1);
            }
            else
            {
                waitpid(cpid, NULL, 0);
            }
        }
    }
    else if (strcmp(cmd_buf, "calculate_score") == 0)
    {
        printf("[Monitor] Calculating scores for all hunts...\n");
        DIR *d = opendir(".");
        if (!d)
        {
            perror("opendir");
            return;
        }
        struct dirent *entry;
        while ((entry = readdir(d)) != NULL)
        {
            if (entry->d_type != DT_DIR ||
                strcmp(entry->d_name, ".") == 0 ||
                strcmp(entry->d_name, "..") == 0)
                continue;
            printf("[Monitor] Hunt '%s':\n", entry->d_name);
            pid_t cpid = fork();
            if (cpid < 0)
            {
                perror("fork calculate_score");
                continue;
            }
            else if (cpid == 0)
            {
                execlp("./score_calculator", "score_calculator", entry->d_name, NULL);
                perror("execlp score_calculator");
                _exit(1);
            }
            else
            {
                waitpid(cpid, NULL, 0);
            }
        }
        closedir(d);
    }
    else if (strcmp(cmd_buf, "stop") == 0)
    {
        printf("[Monitor] Received stop command exiting\n");
        _exit(0);
    }
    else
    {
        printf("[Monitor] Unknown command '%s'\n", cmd_buf);
    }
}

void monitor()
{
    printf("Monitor process [%d] started. Waiting for commands...\n", getpid());
    setup_sigusr1_handler();

    while (1)
    {
        pause(); // wait for signal
        if (got_command)
        {
            got_command = 0;
            process_command();
        }
    }
}

void stop_monitor()
{
    if (monitor_running && monitor_pid > 0)
    {
        printf("Stopping monitor (PID %d)\n", monitor_pid);
        send_command("stop", NULL);
        while (monitor_running)
            sleep(1);
    }
    else
    {
        printf("Error: monitor is not running.\n");
    }
}

void start_monitor()
{
    if (monitor_running)
    {
        printf("Monitor is already running (PID %d).\n", monitor_pid);
        return;
    }

    if (pipe(pipe_fds) < 0)
    {
        perror("pipe");
        return;
    }
    pid_t pid = fork();
    if (pid < 0)
    {
        perror("fork");
    }
    else if (pid == 0)
    {
        close(pipe_fds[0]);
        if (dup2(pipe_fds[1], STDOUT_FILENO) < 0)
        {
            perror("dup2");
            _exit(1);
        }
        close(pipe_fds[1]);
        setvbuf(stdout, NULL, _IONBF, 0);
        monitor();
        _exit(0);
    }
    else
    {
        close(pipe_fds[1]);
        int flags = fcntl(pipe_fds[0], F_GETFL);
        fcntl(pipe_fds[0], F_SETFL, flags | O_NONBLOCK);
        monitor_running = 1;
        monitor_pid = pid;
        printf("Started monitor with PID %d\n", monitor_pid);
        {
            char buf[256];
            ssize_t n;
            while ((n = read(pipe_fds[0], buf, sizeof(buf))) > 0) {
                if (write(STDOUT_FILENO, buf, n) < 0)
                    perror("write to stdout");
            }
            if (n < 0 && errno != EAGAIN && errno != EWOULDBLOCK)
                perror("read pipe");
        }
    }
}

void print_help()
{
    printf("Available commands:\n");
    printf("  start_monitor                     - Start the monitoring service\n");
    printf("  stop_monitor                      - Stop the monitoring service\n");
    printf("  list_hunts                        - List all hunts (requires monitor)\n");
    printf("  list_treasures <hunt_id>         - List treasures for hunt <hunt_id> (requires monitor)\n");
    printf("  view_treasure <hunt_id> <treasure_id> - View details for treasure in a hunt (requires monitor)\n");
    printf("  calculate_score                   - Calculate and display scores for all users in each hunt (requires monitor)\n");
    printf("  help                              - Show this help message\n");
    printf("  exit                              - Exit the application (monitor must be stopped)\n");
}

int main()
{
    setup_sigchld_handler();

    char line[256];
    printf("Welcome to treasure_hub. Type commands (help to list commands, exit to quit).\n");
    while (1)
    {
        printf("> ");
        if (!fgets(line, sizeof(line), stdin))
            break;
        line[strcspn(line, "\n")] = '\0';

        if (strcmp(line, "start_monitor") == 0)
        {
            start_monitor();
        }
        else if (strcmp(line, "stop_monitor") == 0)
        {
            IF_MONITOR_NOT_RUNNING()
            else stop_monitor();
        }
        else if (strcmp(line, "list_hunts") == 0)
        {
            IF_MONITOR_NOT_RUNNING()
            else send_command("list_hunts", NULL);
        }
        else if (strncmp(line, "list_treasures", 14) == 0)
        {
            IF_MONITOR_NOT_RUNNING()
            else
            {
                char *arg = line + 15;
                send_command("list_treasures", arg);
            }
        }
        else if (strncmp(line, "view_treasure", 13) == 0)
        {
            IF_MONITOR_NOT_RUNNING()
            else
            {
                // Expect "view_treasure <hunt_id> <treasure_id>"
                char *args = line + 14;
                char hunt_buf[128], treasure_buf[128];
                if (sscanf(args, "%127s %127s", hunt_buf, treasure_buf) != 2)
                {
                    printf("Error: view_treasure requires two IDs.\n");
                }
                else
                {
                    char combined_arg[256];
                    snprintf(combined_arg, sizeof(combined_arg), "%s %s", hunt_buf, treasure_buf);
                    send_command("view_treasure", combined_arg);
                }
            }
        }
        else if (strcmp(line, "calculate_score") == 0)
        {
            IF_MONITOR_NOT_RUNNING()
            else send_command("calculate_score", NULL);
        }
        else if (strcmp(line, "help") == 0)
        {
            print_help();
        }
        else if (strcmp(line, "exit") == 0)
        {
            if (monitor_running)
            {
                printf("Error: cannot exit while monitor is running.\n");
            }
            else
                break;
        }
        else if (line[0] == '\0')
        {
            continue;
        }
        else
        {
            printf("Unknown command '%s'\n", line);
        }
    }

    printf("Goodbye!\n");
    return 0;
}
