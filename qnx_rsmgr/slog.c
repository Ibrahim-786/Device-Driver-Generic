#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/slog.h>
#include <sys/slogcodes.h>

int main() {
  int i;

  for(i = 0 ; ; i++) {
    switch(rand() % 3) {
    case 0:
      slogb( _SLOG_SETCODE(_SLOGC_TEST, 0),
             _SLOG_DEBUG1, &i, sizeof(i));
      break;

    case 1:
      slogi( _SLOG_SETCODE(_SLOGC_TEST, 1),
             _SLOG_CRITICAL, 1, i);
      break;

    case 2:
      slogf( _SLOG_SETCODE(_SLOGC_TEST, 2),
              _SLOG_ERROR, 
             "This is number %d", i);
      break;
    }

    sleep(1);
  }
}
