#include "helpers.h"

#define CMD_FILE "/tmp/treasure_cmd"
#define ARG_FILE "/tmp/treasure_arg"

#define IF_MONITOR_NOT_RUNNING()                    \
    if (!monitor_running)                           \
    {                                               \
        printf("Error: monitor is not running.\n"); \
    }

int monitor_running = 0;
pid_t monitor_pid = 0;

void send_command(const char *cmd, const char *arg)
{
    printf("Sending command '%s' with arg '%s'\n", cmd, arg ? arg : "(null)");
}

void monitor()
{
    printf("Monitor process [%d] started.\n", getpid());
    sleep(5);
    printf("Monitor process [%d] exiting.\n", getpid());
}

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
        perror("sigaction");
        _exit(1);
    }
}

void stop_monitor()
{
    if (monitor_running && monitor_pid > 0)
    {
        monitor_running = 0;
        printf("Stopping monitor (PID %d)\n", monitor_pid);
        kill(monitor_pid, SIGTERM);
        waitpid(monitor_pid, NULL, 0);
        monitor_pid = 0;
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

    pid_t pid = fork();
    if (pid < 0)
    {
        perror("fork");
    }
    else if (pid == 0)
    {
        // Child process: run the monitor loop
        monitor();
        _exit(0);
    }
    else
    {
        // Parent process
        monitor_running = 1;
        monitor_pid = pid;
        printf("Started monitor with PID %d\n", monitor_pid);
    }
}

void print_help()
{
    printf("Available commands:\n");
    printf("  start_monitor           - Start the monitoring service (forks a child process)\n");
    printf("  stop_monitor            - Stop the monitoring service\n");
    printf("  list_hunts              - List all hunts (requires monitor)\n");
    printf("  list_treasures <id>     - List treasures for hunt <id> (requires monitor)\n");
    printf("  view_treasure <id>      - View details for treasure <id> (requires monitor)\n");
    printf("  help                    - Show this help message\n");
    printf("  exit                    - Exit the application (monitor must be stopped)\n");
}

int main()
{
    // Setup SIGCHLD handler
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
                char *arg = line + 14;
                if (arg[0] == ' ' || arg[0] == '\0')
                    printf("Error: no treasure ID provided.\n");
                else
                    send_command("view_treasure", arg);
            }
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
