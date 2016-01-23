/*
 * 
 */
#pragma once

typedef void (*cmdfunc_t)();

void commands_init();
void commands_shutdown();

void register_command(str_t *name, cmdfunc_t func);

void execute_command(str_t *str);