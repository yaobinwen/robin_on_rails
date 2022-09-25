#pragma once

extern char **_environ;

static char * next_env_entry (char ***position);
