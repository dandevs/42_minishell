#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <time.h>
#include <signal.h>
#include "ast.h"
#include "interpreter.h"
#include "node.h"
#include "ctest.h"

#define TEST_OUT_FILE    "/tmp/minishell_test_out"
#define TEST_STATUS_FILE "/tmp/minishell_test_status"
#define BUF_SIZE         4096
#define TEST_TIMEOUT     5

//------------------------------------------------------------------------------------

static t_node *cmd_node(char **args, int argc)
{
    int     i;
    t_node *node = malloc(sizeof(t_node));
    node->type = NODE_CMD;
    node->left = NULL;
    node->right = NULL;
    node->u_data.cmd.toks.values = malloc(sizeof(t_token) * argc);
    node->u_data.cmd.toks.len = argc;
    node->u_data.cmd.toks.cap = argc;
    i = 0;
    while (i < argc)
    {
        node->u_data.cmd.toks.values[i].str = strdup(args[i]);
        node->u_data.cmd.toks.values[i].type = TOKEN_LITERAL;
        i++;
    }
    return node;
}

static t_node *pipe_node(t_node *left, t_node *right)
{
    t_node *node = malloc(sizeof(t_node));
    node->type = NODE_PIPE;
    node->left = left;
    node->right = right;
    return node;
}

/* ── Test capture infrastructure ──────────────────────────── */

typedef struct s_test_result {
    char buf[BUF_SIZE];
    int  interpreter_exit;
    int  interpreter_signal;
    int  timed_out;
} t_test_result;

static int run_test(t_node *root, t_test_result *res)
{
    pid_t pid;
    int   status;
    time_t start;
    int   fd;
    ssize_t n;

    memset(res, 0, sizeof(*res));
    fflush(stdout);
    fflush(stderr);
    unlink(TEST_OUT_FILE);
    unlink(TEST_STATUS_FILE);
    pid = fork();
    if (pid < 0)
        return -1;
    if (pid == 0) {
        t_interpreter_result ir;
        int out_fd;
        int status_fd;
        char status_buf[64];
        int len;

        setpgid(0, 0);
        out_fd = open(TEST_OUT_FILE, O_WRONLY | O_CREAT | O_TRUNC, 0644);
        if (out_fd < 0)
            _exit(2);
        dup2(out_fd, STDOUT_FILENO);
        close(out_fd);
        ir = interpret(root,
            (t_interpreter_context){.pipe_fd = {-1, -1}, .is_root = 1});
        fflush(stdout);
        status_fd = open(TEST_STATUS_FILE, O_WRONLY | O_CREAT | O_TRUNC, 0644);
        if (status_fd >= 0) {
            len = snprintf(status_buf, sizeof(status_buf), "%d %d",
                ir.exit_status, ir.signal);
            write(status_fd, status_buf, len);
            close(status_fd);
        }
        _exit(0);
    }
    setpgid(pid, pid);
    start = time(NULL);
    while (time(NULL) - start < TEST_TIMEOUT) {
        if (waitpid(pid, &status, WNOHANG) != 0)
            break;
        usleep(10000);
    }
    if (waitpid(pid, &status, WNOHANG) == 0) {
        kill(-pid, SIGKILL);
        waitpid(pid, NULL, 0);
        res->timed_out = 1;
        return 0;
    }
    fd = open(TEST_OUT_FILE, O_RDONLY);
    if (fd >= 0) {
        n = read(fd, res->buf, BUF_SIZE - 1);
        if (n >= 0)
            res->buf[n] = '\0';
        close(fd);
    }
    fd = open(TEST_STATUS_FILE, O_RDONLY);
    if (fd >= 0) {
        char sbuf[64];
        n = read(fd, sbuf, sizeof(sbuf) - 1);
        if (n >= 0) {
            sbuf[n] = '\0';
            sscanf(sbuf, "%d %d", &res->interpreter_exit, &res->interpreter_signal);
        }
        close(fd);
    }
    return 0;
}

/* ── Tests ────────────────────────────────────────────────── */

static int test_echo(void)
{
    t_test_result res;
    t_node *root;

    root = cmd_node(
        (char *[]){"/bin/echo", "hello", "world", NULL}, 3);
    run_test(root, &res);
    ASSERT_FALSE(res.timed_out);
    ASSERT_EQ(0, res.interpreter_exit);
    ASSERT_STREQ("hello world\n", res.buf);
    node_free(root);
    return 0;
}

static int test_echo_no_args(void)
{
    t_test_result res;
    t_node *root;

    root = cmd_node((char *[]){"/bin/echo", NULL}, 1);
    run_test(root, &res);
    ASSERT_FALSE(res.timed_out);
    ASSERT_EQ(0, res.interpreter_exit);
    ASSERT_STREQ("\n", res.buf);
    node_free(root);
    return 0;
}

static int test_pwd(void)
{
    t_test_result res;
    t_node *root;

    root = cmd_node((char *[]){"/bin/pwd", NULL}, 1);
    run_test(root, &res);
    ASSERT_FALSE(res.timed_out);
    ASSERT_EQ(0, res.interpreter_exit);
    ASSERT_TRUE(strstr(res.buf, "/") != NULL);
    node_free(root);
    return 0;
}

static int test_echo_many_args(void)
{
    t_test_result res;
    t_node *root;

    root = cmd_node(
        (char *[]){"/bin/echo", "a", "b", "c", "d", "e", "f", "g", NULL}, 8);
    run_test(root, &res);
    ASSERT_FALSE(res.timed_out);
    ASSERT_EQ(0, res.interpreter_exit);
    ASSERT_STREQ("a b c d e f g\n", res.buf);
    node_free(root);
    return 0;
}

static int test_true(void)
{
    t_test_result res;
    t_node *root;

    root = cmd_node((char *[]){"/bin/true", NULL}, 1);
    run_test(root, &res);
    ASSERT_FALSE(res.timed_out);
    ASSERT_EQ(0, res.interpreter_exit);
    ASSERT_STREQ("", res.buf);
    node_free(root);
    return 0;
}

static int test_cat_devnull(void)
{
    t_test_result res;
    t_node *root;

    root = cmd_node(
        (char *[]){"/usr/bin/cat", "/dev/null", NULL}, 2);
    run_test(root, &res);
    ASSERT_FALSE(res.timed_out);
    ASSERT_EQ(0, res.interpreter_exit);
    ASSERT_STREQ("", res.buf);
    node_free(root);
    return 0;
}

/* === Pipe cases === */

static int test_pipe_echo_wc(void)
{
    t_test_result res;
    t_node *root;

    root = pipe_node(
        cmd_node((char *[]){"/bin/echo", "hello", "world", NULL}, 3),
        cmd_node((char *[]){"/usr/bin/wc", "-w", NULL}, 2));
    run_test(root, &res);
    ASSERT_FALSE(res.timed_out);
    ASSERT_EQ(0, res.interpreter_exit);
    ASSERT_STREQ("2\n", res.buf);
    node_free(root);
    return 0;
}

static int test_pipe_echo_cat_wc(void)
{
    t_test_result res;
    t_node *root;

    root = pipe_node(
        pipe_node(
            cmd_node((char *[]){"/bin/echo", "hello", NULL}, 2),
            cmd_node((char *[]){"/usr/bin/cat", NULL}, 1)),
        cmd_node((char *[]){"/usr/bin/wc", "-w", NULL}, 2));
    run_test(root, &res);
    ASSERT_FALSE(res.timed_out);
    ASSERT_EQ(0, res.interpreter_exit);
    ASSERT_STREQ("1\n", res.buf);
    node_free(root);
    return 0;
}

static int test_pipe_triple(void)
{
    t_test_result res;
    t_node *root;

    root = pipe_node(
        pipe_node(
            pipe_node(
                cmd_node((char *[]){"/bin/echo", "a", "b", "c", NULL}, 4),
                cmd_node((char *[]){"/usr/bin/cat", NULL}, 1)),
            cmd_node((char *[]){"/usr/bin/cat", NULL}, 1)),
        cmd_node((char *[]){"/usr/bin/wc", "-w", NULL}, 2));
    run_test(root, &res);
    ASSERT_FALSE(res.timed_out);
    ASSERT_EQ(0, res.interpreter_exit);
    ASSERT_STREQ("3\n", res.buf);
    node_free(root);
    return 0;
}

static int test_pipe_grep(void)
{
    t_test_result res;
    t_node *root;

    root = pipe_node(
        cmd_node((char *[]){"/bin/echo", "test123", NULL}, 2),
        cmd_node((char *[]){"/usr/bin/grep", "test", NULL}, 2));
    run_test(root, &res);
    ASSERT_FALSE(res.timed_out);
    ASSERT_EQ(0, res.interpreter_exit);
    ASSERT_STREQ("test123\n", res.buf);
    node_free(root);
    return 0;
}

/* === Failure cases === */

static int test_false(void)
{
    t_test_result res;
    t_node *root;

    root = cmd_node((char *[]){"/bin/false", NULL}, 1);
    run_test(root, &res);
    ASSERT_FALSE(res.timed_out);
    ASSERT_EQ(1, res.interpreter_exit);
    ASSERT_STREQ("", res.buf);
    node_free(root);
    return 0;
}

static int test_invalid_command(void)
{
    t_test_result res;
    t_node *root;

    root = cmd_node(
        (char *[]){"/nonexistent_cmd_xyz", NULL}, 1);
    run_test(root, &res);
    ASSERT_FALSE(res.timed_out);
    ASSERT_EQ(127, res.interpreter_exit);
    ASSERT_STREQ("", res.buf);
    node_free(root);
    return 0;
}

static int test_custom_exit_code(void)
{
    t_test_result res;
    t_node *root;

    root = cmd_node(
        (char *[]){"/bin/sh", "-c", "exit 42", NULL}, 3);
    run_test(root, &res);
    ASSERT_FALSE(res.timed_out);
    ASSERT_EQ(42, res.interpreter_exit);
    ASSERT_STREQ("", res.buf);
    node_free(root);
    return 0;
}

static int test_pipe_last_fails(void)
{
    t_test_result res;
    t_node *root;

    root = pipe_node(
        cmd_node((char *[]){"/bin/echo", "hello", NULL}, 2),
        cmd_node((char *[]){"/bin/false", NULL}, 1));
    run_test(root, &res);
    ASSERT_FALSE(res.timed_out);
    ASSERT_EQ(1, res.interpreter_exit);
    node_free(root);
    return 0;
}

static int test_pipe_last_succeeds(void)
{
    t_test_result res;
    t_node *root;

    root = pipe_node(
        cmd_node((char *[]){"/bin/false", NULL}, 1),
        cmd_node((char *[]){"/bin/echo", "hello", NULL}, 2));
    run_test(root, &res);
    ASSERT_FALSE(res.timed_out);
    ASSERT_EQ(0, res.interpreter_exit);
    ASSERT_STREQ("hello\n", res.buf);
    node_free(root);
    return 0;
}

static int test_pipe_exit_code(void)
{
    t_test_result res;
    t_node *root;

    root = pipe_node(
        cmd_node((char *[]){"/bin/echo", "hello", NULL}, 2),
        cmd_node((char *[]){"/bin/sh", "-c", "exit 42", NULL}, 3));
    run_test(root, &res);
    ASSERT_FALSE(res.timed_out);
    ASSERT_EQ(42, res.interpreter_exit);
    node_free(root);
    return 0;
}

static int test_grep_no_match(void)
{
    t_test_result res;
    t_node *root;

    root = pipe_node(
        cmd_node((char *[]){"/bin/echo", "hello", NULL}, 2),
        cmd_node((char *[]){"/usr/bin/grep", "xyz", NULL}, 2));
    run_test(root, &res);
    ASSERT_FALSE(res.timed_out);
    ASSERT_EQ(1, res.interpreter_exit);
    ASSERT_STREQ("", res.buf);
    node_free(root);
    return 0;
}

//------------------------------------------------------------------------------------

int main(void)
{
    int fail = 0;

    fail += test_echo();
    fail += test_echo_no_args();
    fail += test_pwd();
    fail += test_echo_many_args();
    fail += test_true();
    fail += test_cat_devnull();
    fail += test_pipe_echo_wc();
    fail += test_pipe_echo_cat_wc();
    fail += test_pipe_triple();
    fail += test_pipe_grep();
    fail += test_false();
    fail += test_invalid_command();
    fail += test_custom_exit_code();
    fail += test_pipe_last_fails();
    fail += test_pipe_last_succeeds();
    fail += test_pipe_exit_code();
    fail += test_grep_no_match();

    return fail != 0;
}
