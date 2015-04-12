// checks that the entered emails in the configuration are valid 
// emails of the subscribers are not check
// emails are checked in the keywords Owner, Configuration-Owner,
// Editor, Moderator, Send, Errors-To
// an email is valid if the domain is registered in DNS, and
// before @ come only A-Z0-9./_ characters (at least once)
// if no @ is contained in the value, but it has () make sure the lists in parenthesis does exist

#include "util.h"
#include <glib.h>
#include <string.h>
#include <netinet/in.h> 
#include <arpa/nameser.h>
#include <resolv.h>
#include <glib/gstdio.h>

res_state statp;
GRegex *regex;
static GHashTable *invalid_emails_hashtable;
static GHashTable *invalid_emails_good_domains_hashtable;

void invalid_emails_LTX_load () 
{
  //  res_ninit (statp);
  regex = g_regex_new("^[-.\\w+%]+@[-.\\w]+$",
		      G_REGEX_OPTIMIZE, 0, NULL);
  invalid_emails_hashtable = g_hash_table_new_full (g_str_hash, g_str_equal,
						    g_free, g_free);
  invalid_emails_good_domains_hashtable =
    g_hash_table_new_full( g_str_hash, g_str_equal, g_free, g_free);
}

void invalid_emails_LTX_unload () 
{
  //  res_ndestroy (statp);
  g_regex_unref(regex);
  g_hash_table_destroy (invalid_emails_hashtable);
  g_hash_table_destroy (invalid_emails_good_domains_hashtable);
}

char*
invalid_emails_LTX_comment (char *listname)
{
  return g_strdup (g_hash_table_lookup (invalid_emails_hashtable, listname));
}

static inline char*
check_value(struct listserv *l, char *value)
{
  if (strchr (value, '@')) {//check if the email is valid
    if (g_regex_match(regex, value, 0, NULL) == FALSE)
      return " is an invalid email address";
    //char *domain = strchr(value, '@') + 1;
    //res_query (domain, 1, 
    //type 1 - A, 5 - CNAME, 15 - MX
  } else if (strchr (value, '(')) {//check if the list is valid
    char *listname = g_strdup(strchr (value, '(') + 1);
    if (listname[strlen (listname) -1] != ')')
      return " does not end with parenthesis";
    listname[strlen(listname) - 1] = '\0';
    char* cmd = g_alloca(strlen (listname) + 21);
    g_sprintf(cmd, "GET %s (MSG HDR NOLOCK", listname);
    char *list = listserv_command (l, cmd);
    g_free(listname);
    if ('T' == list[0])
      return " referenced list does not exist";
  }
  return NULL;
}

static inline void
check_keyword(struct listserv *l, const char const * listname, char* keyword)
{
  char **kw = listserv_getlist_keyword(l, listname, keyword);
  int i = 0;
  while (kw[i]) {
    char *err = check_value(l, kw[i]);
    if (err) {
      char *elem = g_hash_table_lookup (invalid_emails_hashtable, listname);
      if (elem == NULL)
	g_hash_table_insert (invalid_emails_hashtable, g_strdup (listname),
			     g_strconcat(keyword, " = ", kw[i], err, NULL));
      else
	g_hash_table_insert (invalid_emails_hashtable, g_strdup (listname),
			     g_strconcat (elem, ";", keyword,
					  " = ", kw[i], err, NULL));
    }
    i++;
  }
}

char*
invalid_emails_LTX_listowners_mailing (const char* const listname)
{
  return g_strconcat("\
== Invalid Email Addresses ==================================================\r\n\
Some email addresses in the configuration of ", listname, " \r\n\
seem to be misspelled.  Namely:\r\n  ",
		     g_hash_table_lookup(invalid_emails_hashtable, listname),
"\r\n\r\nPlease log in at \r\n\
https://lists.aegee.org/?HDRED1=", listname, " \r\n\
and correct the addresses.  At the end of this email you can find \r\n\
information how to login.", NULL);
}

char
invalid_emails_LTX_changed_list (struct listserv *l,
				 const char * const listname)
{
  check_keyword(l, listname, "Configuration-Owner");
  check_keyword(l, listname, "Editor");
  check_keyword(l, listname, "Errors-To");
  check_keyword(l, listname, "Moderator");
  check_keyword(l, listname, "Owner");
  check_keyword(l, listname, "Review");
  check_keyword(l, listname, "Send");
  return (NULL != g_hash_table_lookup (invalid_emails_hashtable, listname));
}
