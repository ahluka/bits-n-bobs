#include "tb_shared.h"
#include "commands.h"

typedef struct command_s {
    str_t *name;
    cmdfunc_t func;

    struct command_s *next;
} command_t;

static command_t *commands = NULL;

void cmdlist_c()
{
    command_t *iter = NULL;
    size_t total = 0;

    if (!commands)
        return;

    printf("==== Commands ====\n");
    for (iter = commands; iter; iter = iter->next) {
        printf("\t%s\n", iter->name);
        total++;
    }

    printf("\nTotal commands: %u\n", total);
}

void commands_init()
{
    register_command("cmdlist", cmdlist_c);
}

void commands_shutdown()
{
    command_t *iter, *dead;

    if (!commands)
        panic("shutdown_commands: NULL command list");

    for (iter = commands; iter; ) {
        dead = iter;
        iter = iter->next;

        tb_free(dead);
    }
}

void register_command(str_t *name, cmdfunc_t func)
{
    command_t *c = NULL;

    if (!name)
        panic("create_command: NULL name");

    if (!func)
        panic("create_command: NULL func");

    for (c = commands; c; c = c->next) {
        if (strncmp(name, c->name, strlen(name)) == 0) {
            plog("register_command: %s already registered", name);
            return;
        }
    }

    c = tb_malloc(sizeof(*c));
    c->func = func;
    c->name = name;
    c->next = commands;
    commands = c;
}

void execute_command(str_t *str)
{
    command_t *iter = NULL;

    if (!str)
        panic("execute_command: NULL str");

    if (!commands) {
        plog("execute_command: no commands registered");
        return;
    }

    for (iter = commands; iter; iter = iter->next) {
        if (strncmp(str, iter->name, strlen(str)) == 0)
            iter->func();
    }
}