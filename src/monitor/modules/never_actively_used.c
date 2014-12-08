#include <glib.h>
#include <liblistserv.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "util.h"
GHashTable* never_actively_used_hashtable;

void
never_actively_used_LTX_load ()
{
  never_actively_used_hashtable = g_hash_table_new (g_str_hash, g_str_equal);
}

void never_acively_used_LTX_unload ()
{
  g_hash_table_destroy (never_actively_used_hashtable);
}

char*
never_actively_used_LTX_listowners_mailing (const char const * listname)
{
  return g_strconcat("\
== Not yet active ==========================================================\r\n\
Your newly created list is not active yet.  Do you need some assitance\r\n\
to manage the list?  You can reply to this email, and we will help you to \r\n\
start!  At the end of this email you can check how to login at\r\n\
  https://lists.aegee.org/cgi-bin/wa?A0=", listname, "\r\n\
Then you can choose (from the top) List Management -> Subscriber Management to\r\n\
add new subscribers.  It's easy!", NULL);
}

static void remove_mail(struct listserv *l, const char const *listname) {
  if (strcmp(listname, "LISTOWNERS-L") == 0) return;//don't remove mail@ from LISTOWNERS-L
  char * cmd = malloc (strlen(listname) + 40);
  sprintf(cmd, "QUIET DEL %s mail@aegee.org", listname);
  listserv_command (l, cmd);
  free (cmd);
}

char
never_actively_used_LTX_changed_list (struct listserv *l, const char const * listname) {
  int num_postings = 0;
  if (keyword_doesnot_contain (l, listname, "Change-Log", "No")) {
    char *changelog = listserv_get(l, listname, "CHANGELOG");
    if (changelog[0] != 'F') {
      //char* last_date = NULL;
      while ((changelog = g_strstr_len (changelog, -1, " POST "))) {
	if (changelog) {
	  changelog += 6;
	} else break;
	num_postings++;
	if (num_postings > 6) {
	  g_hash_table_steal (never_actively_used_hashtable, listname);
	  remove_mail (l, listname);
	  return 0;
	}
      }
    }
  }
  char **files = listserv_list_filelist (l, listname);
  num_postings = 0;
  int num_logfiles = 0;
  while (files[num_logfiles]) {
    if (files[num_logfiles][0] == 'L') {
      char *logfile = listserv_get(l, listname, files[num_logfiles]);
      while ((logfile = g_strstr_len (logfile, -1, "=========================================================================\n"))) {
	if (logfile == NULL) break;
	logfile+= 10;
	num_postings++;
	if (num_postings > 6) {
	  g_hash_table_steal (never_actively_used_hashtable, listname);
	  remove_mail (l, listname);
	  return 0;
	}
      }
    }
    num_logfiles++;
  }
  num_logfiles--;
  while (num_logfiles >= 0 && files[num_logfiles][0] != 'L')
    num_logfiles--;
  g_hash_table_insert (never_actively_used_hashtable,
		       (gpointer) listname, NULL);

  if (num_logfiles >= 0) {
    int year = (files[num_logfiles][3] - 48) * 10
      + (files[num_logfiles][4] - 48);
    int month = (files[num_logfiles][5] - 48) * 10
      + (files[num_logfiles][6] - 48);
    time_t _time = time (NULL);
    struct tm lt;
    localtime_r (&_time, &lt);
    if ( ((month > 1) && ((lt.tm_year % 100 == year)
			 && (month == lt.tm_mon + 1 || month == lt.tm_mon)))
	 || (lt.tm_year % 100 == year - 1 && lt.tm_mon == 11)) {
      g_hash_table_steal (never_actively_used_hashtable, listname);
      remove_mail (l, listname);
      return 0;
    } else
      return 1;
  }
  return 1;
}