#include <liblistserv.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>

//paramters
// 1- HOST or --usage or --help
/*
 * Plugin Return Code	Service State	Host State
 * 0	OK	UP
 * 1	WARNING	UP or DOWN/UNREACHABLE*
 * 2	CRITICAL	DOWN/UNREACHABLE
 * 3	UNKNOWN	DOWN/UNREACHABLE
 */
int main(int argc, char* argv[]) {
  FILE *file = fopen("/var/log/check_listserv.log", "a");
  time_t _time = time(NULL);
  struct tm lt;
  localtime_r(&_time, &lt);
  fprintf(file, "%04d/%02d/%02d %02d:%02d:%02d PID: %6i START\n", 1900 + lt.tm_year, lt.tm_mon + 1, lt.tm_mday, lt.tm_hour, lt.tm_min, lt.tm_sec, getpid());
  fflush(file);
  char *host;
  if (argc == 1) host = "localhost";
  if (argc == 2) {
    if (strcmp(argv[1], "--usage") == 0) {
      printf("Usage:\n    check_listserv [host]\n    if host is missing, localhost is used");
      return 3;
    } else
    if (strcmp(argv[1], "--version") == 0 || strcmp(argv[1], "-V") == 0) {
      printf("check_listserv v1.0\n");
      return 3;
    } else
    if (strcmp(argv[1], "--help") == 0 || strcmp(argv[1], "-h") == 0) {
      printf("Help:\n\
check_listserv v1.0\n\
Copyright (c) 2009 Dilyan Palauzov <check_listserv@aegee.org>\n\
\n\
This plugin checks if Listserv is responding on TCP GUI commands.\n\
\n\
Usage: check_listserv [hostname]\n\
\n\
Options:\n\
 -h, --help\n\
    Print detailed help screen\n\
 -V, --version\n\
    Print version information\n\
\n\
If hostname is missing localhost is used.\n\
\n\
Send email to check_listserv@aegee.org if you have questions\n\
regarding use of this software, to submit patches or suggest improvements.\n");
      return 3;
    } else
    host = argv[1];
  }
  struct listserv *l = listserv_init("@", "password", host);
  int return_code = 0;
  if (strcmp( listserv_command(l, "THANKS"), "You're welcome!") != 0) {
    return_code = 2;
    //    printf("Listserv on host %s port 2306 does not respond.\n", host);
  }
  _time = time(NULL);
  localtime_r(&_time, &lt);
  fprintf(file, "%04d/%02d/%02d %02d:%02d:%02d PID: %6i END %i\n", 1900 + lt.tm_year, lt.tm_mon + 1, lt.tm_mday, lt.tm_hour, lt.tm_min, lt.tm_sec, getpid(), return_code);
  fflush(file);
  fclose(file);
  listserv_destroy(l);
  return 0;
}
