/**
 * cmd.h
 *
 * ftp command handler
 *
 * Guo Jian
 * 2014.06.24
 */

#ifndef _HF_CMD_H_
#define _HF_CMD_H_

#include "hvsftpd.h"
#include "session.h"

typedef int (*cmd_handler)(struct session *s, char *param);

struct command_map
{
    char *cmd;
    cmd_handler handler;
};

int cmd_disptach(char *cmd, struct session *s);

#endif /* _HF_CMD_H_ */
