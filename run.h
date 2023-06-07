#ifndef RUN_H
#define RUN_H

#include"ls.h"
#include"cd.h"
#include"prompt.h"
#include"echo.h"
#include"pwd.h"
#include"history.h"
#include"queue.h"
#include"discover.h"
#include"pinfo.h"
#include"exec.h"
#include"redirection.h"
#include"jobs.h"
#include"sig.h"
#include"fg.h"
#include"bg.h"
#include"signals.h"
#include"auto.h"

void run(int argc, char *argv[]);

#endif