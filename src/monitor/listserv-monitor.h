#include <liblistserv.h>
#include <ltdl.h>
#include <glib.h>
#include <string.h>
#include <stdio.h>
struct module {
  lt_dlhandle mod;
  void  (*load)();
  char  (*changed_list)(struct listserv*, const char const *);
  char* (*changed_mailtpl)(struct listserv*, char*);
  char* (*listowners_mailing)(const char const*);
  char* (*comment)(const char*);
  char* name;
  void  (*unload)();
};

unsigned int longest_listname;
unsigned int longest_modulename;
char* disabled_module;
GPtrArray *modules;
int sendmail (char **, const char*, const char*, const char*);
void mailing_alllists (GHashTable*);
char* print_report (GHashTable *gh, struct listserv *l, int format);
void mail_report (GHashTable *gh, struct listserv *l);
