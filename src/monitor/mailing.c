#include "listserv-monitor.h"
#include <glib/gstdio.h>

//static char *sendmail = "/usr/bin/sendmail";

//-----------------------------------------------------------------------------
//return values: != 0 is error, == 0 is OK

int
sendmail (char** const rcpt,
	  const char* const subject,
          const char* const content_type,
	  const char* const body)
{
  static const char *month[] = { "Jan", "Feb", "Mar", "Apr", "May", "Jun",
				 "Jul", "Aug", "Sep", "Oct", "Nov", "Dec" };

  static const char *wday[] = { "Sun", "Mon", "Tue", "Wed",
				"Thu", "Fri", "Sat" };
  GString *cmdline = g_string_new ("/usr/bin/sendmail -fmail@aegee.org mail@aegee.org");
  GString *message = g_string_new ("\
From: AEGEE Mail (listserv-monitor) <mail@aegee.org>\r\n\
To:");
  int i;
  for (i = 0; rcpt[i]; i++) {
    if (g_ascii_strcasecmp (rcpt[i], "mail@aegee.org"))
      g_string_append_printf (cmdline, " %s", rcpt[i]);
    g_string_append_printf (message, " %s,", rcpt[i]);
  }
  message->str[message->len - 1] = '\r';
  g_string_append_c (message, '\n');
  time_t _time = time (NULL);
  struct tm lt;
  long gmtoff = -0;
  int gmtneg = 0;
  localtime_r (&_time, &lt);
  g_string_append_printf (message, "Date: %s, %02d %s %4d %02d:%02d:%02d \
%c%.2lu%.2lu\r\n",
    	  wday[lt.tm_wday], lt.tm_mday, month[lt.tm_mon],
	  lt.tm_year + 1900, lt.tm_hour, lt.tm_min, lt.tm_sec,
	  gmtneg ? '-' : '+', gmtoff / 60, gmtoff % 60);
  g_string_append_printf (message, "Subject: %s\r\n", subject);
  g_string_append_printf (message,
			  "Auto-Submitted: auto-generated\r\n");
  g_string_append_printf (message,
			  "User-Agent: listserv-monitor 1.0\r\n");
  g_string_append_printf (message, "MIME-Version: 1.0\r\n");
  g_string_append_printf (message,
  	  "Content-Type: %s\r\n", content_type);
  g_string_append_printf (message, "Content-Transfer-Encoding: 8bit\r\n");
  g_string_append_printf (message, "\r\n%s\r\n", body);
  FILE *sm = popen ( cmdline->str, "w");
  if (sm == NULL) return -1;
  g_string_free (cmdline, TRUE);
  fprintf (sm, "%s", message->str);
  g_string_free (message, TRUE);
  gmtneg = pclose (sm);
  return gmtneg;
}

/*
static char*
insert_new_lines (char *text, int format)
//format 0 - text, 1 - html
{
  GString* str = g_string_new(text);
  unsigned int pos = 0;
  while (pos < str->len) {
    int new_line_found = 0;
    int this_line;
    //    sleep(1);
    //    printf("A %d %d\n", pos, str->len);
    for (this_line = 0; this_line < 80; this_line++) {
      if (pos + this_line >= str->len) {
	//	printf("A1 %d %d\n", pos, str->len);
	this_line = str->len - pos;
	break;
      }
      if (str->str[pos + this_line] == '\n') {
	//	printf("A2 %d %d\n", pos, str->len);
	new_line_found = 1;
	g_string_insert_c (str, pos + this_line, '\r');
	if (format) {
	  pos += 2;
	  g_string_insert (str, pos + this_line, "<br/>");
	}
	break;
      }
    }
    this_line = 80;
    //    printf("B %d %d\n", pos, str->len);
    if (new_line_found == 0) {
      //      printf("C %d %d\n", pos, str->len);
      pos += 80;
      if (str->len < pos)
	break;
      while (str->str[pos] != ' ')
	pos--;
      g_string_erase (str, pos, 1);
      g_string_insert (str, pos, "\r\n");
      if (format) {
	pos += 2;
	g_string_insert(str, pos, "<br/>");
      }
    } else {
      pos+= this_line;
    }
    pos++;
  }
  return g_string_free(str, FALSE);
}
*/

static void
mailing_onelist (gpointer p1,
		 gpointer p2,
		 gpointer p3)
{
  char *listname = (char*)p1;
  GByteArray *gb = (GByteArray*)p2;
  struct listserv *l = (struct listserv*)p3;
  const char *kw[] = {"Owner", NULL};
  char **rcpts = listserv_getemails_fromkeywords (l, listname, kw);
  if (strcasecmp(rcpts[0], "nichts@aegee.org") == 0)
    return;
  int do_mail = 0;
  GString *mailtext = g_string_new ("");
  g_string_append_printf (mailtext, "Hello,\r\n\
\r\n\
You manage the mailing list %s!\r\n\r\n\
This is an automated message to help you improve and secure your list against\r\n\
spam or make it conform to the AEGEE policies for mailing lists.  Please read\r\n\
each section which helps you improve one detail.\r\n\r\n", listname);
  unsigned int i;
  for (i = 0; i < modules->len; i++)
    if (gb->data[i] == 2) {
      struct module *mod = g_ptr_array_index (modules, i);
      char *temp =  mod->listowners_mailing (listname);
      //      char *temp_new_lines = insert_new_lines (temp, 0);
      g_string_append_printf (mailtext, "%s\r\n\r\n", temp);
      g_free (temp);
      //      g_free (temp_new_lines);
      do_mail = 1;
    }
  if (do_mail) {
    g_printf ("%s\n", listname);
    g_string_append_printf (mailtext, "\r\n\
Thanks in advance for your cooperation.\r\n\
\r\n\
The AEGEE Mail Team\r\n\
  Christophe, Clemens, Дилян, Patrik, Richard, Sorin\r\n\
  http://mail.aegee.org\r\n\
  email: mail@aegee.org\r\n\
  sip:   8372@aegee.org\r\n\
\r\n\
== How to Login ===============================================================\r\n\
You manage this list with the email address, where this email is sent (check\r\n\
the To: header).  If you don't have yet a password associated within listserv\r\n\
for this email address, obtain a new one from https://lists.aegee.org/pw . Once\r\n\
a new password is requested you will get an email.  Click on the link in this\r\n\
email to activate the password.  Please note, that this password is used\r\n\
solely to manage your lists in AEGEE, it does not have to be the password you\r\n\
use to login in your mailbox.\r\n");
    char* subj = g_strconcat ("Your list ", listname, NULL);
    sendmail (rcpts, subj, "text/plain; charset=UTF-8", mailtext->str);
    g_free (subj);
  }
  g_string_free (mailtext, TRUE);
  return;
}

void
mailing_alllists (GHashTable *gh)
{
  struct listserv *l = listserv_init (NULL, NULL, NULL);
  char **owned_lists = listserv_getowned_lists (l);
  int i = 0;
  while (owned_lists[i]) {
    GByteArray* gb = g_hash_table_lookup (gh, owned_lists[i]);
    if (gb != NULL)
      mailing_onelist (owned_lists[i], gb, l);
    i++;
  }
  mail_report (gh, l);
  listserv_destroy (l);
}
