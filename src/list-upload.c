#include <liblistserv.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/stat.h>
#include <glib.h>
#include <argp.h>
#include <glib/gstdio.h>
#include <unistd.h>

//arguments
// -i listname -- installs a listname
// -m listname -- sends welcome mail over the list
// no params -- print help
// -h print help
// -s list subscriber name(s)
// -v print version

static inline int
sendmail (const char* const rcpt,
	  const char* const subject,
	  const char* const body)
{
  const char *month[] = { "Jan", "Feb", "Mar", "Apr", "May", "Jun",
				 "Jul", "Aug", "Sep", "Oct", "Nov", "Dec" };

  const char *wday[] = { "Sun", "Mon", "Tue", "Wed",
				"Thu", "Fri", "Sat" };
  GString *cmdline = g_string_new ("/usr/bin/sendmail -fmail@aegee.org ");
  g_string_append_printf (cmdline, "%s", rcpt);
  GString *message = g_string_new ("\
From: AEGEE Mail (listcreator) <mail@aegee.org>\r\n");
  g_string_append_printf( message, "To: %s\r\n", rcpt);
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
			  "Auto-Submitted: yes\r\n");
  g_string_append_printf (message,
			  "User-Agent: list-upload 1.0\r\n");
  g_string_append_printf (message, "MIME-Version: 1.0\r\n");
  g_string_append_printf (message,
  	  "Content-Type: text/plain; charset=UTF-8; format=flowed\r\n");
  g_string_append_printf (message, "Content-Transfer-Encoding: 8bit\r\n");
  g_string_append_printf (message, "\r\n%s\r\n", body);
  FILE *sm = popen ( cmdline->str, "w");
  if (sm == NULL) return -1;
  g_string_free (cmdline, TRUE);
  g_fprintf (sm, "%s", message->str);
  g_string_free (message, TRUE);
  gmtneg = pclose (sm);
  return gmtneg;
}

static inline char*
get_listname (struct listserv *l, char *listname /* argv[2] */)
{
  char* temp = listserv_get (l, listname, "HDR");
  if (temp[0] == 'T') {
    g_fprintf (stderr, "List %s does not exist.\n", listname);
    return NULL;
  };
  return g_ascii_strup (listname, -1);
}

static const char* welcome_begin = "Hello,\r\n\
\r\n\
This is the first mail sent over %s@aegee.org mailing list.  To manage the list you need a listserv password that can be obtained from https://lists.aegee.org/password and is valid for the email address you are subscribed to the concrete mailing list.  All the mails sent on the list will be archived and are available after login under https://lists.aegee.org/%s.html .  In order to access them, or the moderation interface of the mailing list, please go to the above address and select \"List management\" or the appropriate period to read the archives.  The \"List Management\" menu gives you the possibilities to administer the list. E.g. to add new members choose the item \"Subscriber Management\", to remove or review the subscribers, choose \"List Management\" -> \"List Reports\" -> \"Subscriber Reports\".\r\n\
\r\n\
The complete listserv owner's manual for managing mailing lists can be found at http://www.lsoft.com -> Support -> Documentation -> Listserv, List owner's manual and the introductory Listowner's guide. As list subscriber, throw a look at the Listserv General Users' Guide.\r\n\
\r\n\
The mails are distributed, using Listserv 16.0 software, donated by LSoft - global IT pertner of AEGEE. If you have any questions concerning the mailing list management, adjusting the settings, etc, please contact mail@aegee.org .\r\n\
\r\n\
In order to unsubscribe, send an e-mail to listserv@aegee.org with the text (no subject, no html, just plain text) \"SIGNOFF %s\" or visit https://lists.aegee.org/cgi-bin/wa?SUBED1=%s&A=1 .\r\n\
\r\n\
To add an email address to the list, go to the webinterface List Management -> Subscriber Management . Always include the name of the person you add, followed by the antennae in brackets, following the email address. To manage the list, try to find a second person and share with him/her the task. In case one of you is unavailable, the other can still react in urgent situations.\r\n\
\r\n\
Don't forget to write a mail to mail@aegee.org once you decide not to use the list any more, so that it can be archived and space for other lists gets free.  If you experience problems with this list, write to %s-request@aegee.org in order to reach the persons responsible for the %s@aegee.org mailing list.\r\n\
\r\n\
The address mail@aegee.org is added now in the list for administrative purposes only. Once the list start functioning properly, it will disappear by itself.\r\n\
\r\n\
Special remarks for the %s@aegee.org mailing list (YOUR CURRENT SETTINGS):\r\n";

static const char* welcome_end = "\r\n\
  Best wishes, good luck and success with the newest AEGEE mailing list!\r\n\
\r\n\
   Christophe, Clemens, Дилян, Patrik, Richard, Sorin\r\n\
   AEGEE Mail Team\r\n\
   mail@aegee.org\r\n\
   sip: 8372@aegee.org\r\n\
   http://mail.aegee.org/\r\n\
   http://lists.aegee.org/\r\n";

int
main (int argc, char *argv[])
{
  //  error_t err = argp_parse(ARGP, argv, argv, FLAGS,
  //			   NULL /*ARG_INDEX */, INPUT);
  /*  if (argc > 5 || argv[1][0] != '-' || argc < 3) {
    fprintf (stderr, "%s must be called with exactly two arguments: -i (for installing new list), -m (for subscribing mail@aeee.org, recreating the aliases and for sending welcome message over the list) and the name of the mailing list\n", argv[0]);
    return 1;
    } */

//SUID
  setuid (geteuid());
  struct listserv *l = listserv_init (NULL, NULL, NULL);
  char *listname, *temp, *lsv_resp;
  int i;
  if (argc > 1)
  switch (argv[1][1]) {
  case 'i':
    listname = g_alloca (strlen(argv[2])+1);
    i = 0;
    g_sprintf (listname, "%s", argv[2]);
    while (listname[i]) {
      listname[i] = toupper (listname[i]);
      i++;
    }
    //INSTALL THE LIST HEADERS
    lsv_resp = listserv_get (l, listname, "HDR");
    if (lsv_resp[0] != 'T') {
      g_fprintf (stderr, "List %s already exists.\n", listname);
      return 2;
    }
    char *stdinput = g_malloc (5000);
    int read_chars = 0, stdinput_size = 5000;
    int len = 0;
    while (! feof (stdin)) {
      len = fread (stdinput + read_chars, 1,
		   stdinput_size - len - 2, stdin);
      read_chars += len;
      if (len + 10 > stdinput_size) {
	stdinput_size += 1000;
	stdinput = g_realloc (stdinput, stdinput_size);
      }
    }
    temp = g_alloca (strlen (listname) + 28);
    g_sprintf (temp, "/home/listserv/%s", listname);
    i = 15;
    while (temp[i]) {
      temp[i] = tolower (temp[i]);
      i++;
    }
    mkdir (temp, S_IRWXU | S_IRWXG);
    chown (temp, 15, 53);
    g_sprintf (temp, "/home/listserv/public_html/%s", listname);
    i = 26;
    while (temp[i]) {
      temp[i] = tolower (temp[i]);
      i++;
    }
    mkdir (temp, S_IRWXU | S_IRWXG);
    chown (temp, 15, 53);
    char *output = listserv_putheader (l, listname, stdinput);
    GString *subj = g_string_new ("Installed ");
    g_string_append_printf (subj, "%s", listname);
    GString *body = g_string_new ("Reply from Listserv:\r\n");
    g_string_append_printf (body, "%s\r\n\r\nInstalled list headers:\r\n%s",
			    output, stdinput);
    sendmail ("mail@aegee.org", subj->str, body->str);
    g_string_free (subj, TRUE);
    g_string_free (body, TRUE);
    g_free (stdinput);
    g_printf ("%s", output);
    break;
  case 's':
    listname = get_listname (l, argv[2]);
    if (listname == NULL) {
      listserv_destroy (l);
      return 1;
    }
    temp = g_alloca (strlen (argv[3]) + strlen (listname) + 7);
    g_sprintf (temp, "ADD %s %s", listname, argv[3]);
    g_printf ("%s\n", listserv_command (l, temp));
    g_free (listname);
    break;
  case 'm':
    listname = get_listname (l, argv[2]);
    if (listname == NULL) {
      listserv_destroy (l);
      return 2;
    }
    temp = g_alloca (strlen (listname) + 16);
    g_sprintf (temp, "QUIET ADD %s mail", listname);
    listserv_command (l, temp);
    //UPDATE THE ALIASES
    system ("/home/listserv/home/GENERATE > /dev/null");
    //SUBSCRIBE THE LISTONWERS
    char **keyword;
    //    char ** owners = listserv_getlist_keyword (l, listname, "Owner");
    GString *message = g_string_sized_new(1000);
    char *listname_lower = g_ascii_strdown (listname, -1);
    g_string_append_printf(message, welcome_begin, listname,
			   listname_lower, listname, listname,
			   listname, listname, listname);
    g_free (listname_lower);

    keyword = listserv_getlist_keyword (l, listname, "Confidential");
    g_string_append_printf (message, "  * The list will %sbe visible under lists.aeee.org\r\n", (strcasecmp (keyword[0], "Yes") == 0) ? "not " : "");

    keyword = listserv_getlist_keyword (l, listname, "Notify");
    g_string_append_printf (message, "  * The listowners will %sreceive notifications for new subscriptions\r\n", (strcasecmp (keyword[0], "No") == 0) ? "not ": "");

    keyword = listserv_getlist_keyword (l, listname, "Review");
    g_string_append_printf (message, "  * The %s can see who is subscribed to the list\r\n", (strcasecmp (keyword[0], "Owner") == 0) ? "listowners" : "subscribers");

    keyword = listserv_getlist_keyword (l, listname, "Reply-To");
    g_string_append_printf (message, "  * Replies to sent emails will go by default to the %s\r\n", (strcasecmp (keyword[0], "List") == 0) ? "whole list": (strcmp(keyword[0], "Both") == 0 ? "whole list and the original sender, if he is not on the list" : "original sender"));

    keyword = listserv_getlist_keyword (l, listname, "Subscription");
    g_string_append_printf (message, "  * %s\r\n", (strcasecmp (keyword[0], "Open") == 0) ? "Everybody can subscribe to the list": "New subscribers can only be added");

    keyword = listserv_getlist_keyword (l, listname, "Ack");
    g_string_append_printf (message, "  * There is %sconfirmation sent to the sender, when mails are distributed\r\n", (strcasecmp (keyword[0], "Yes") == 0) ? "": "no ");
    keyword = listserv_getlist_keyword (l, listname, "Send");
    if (strcasecmp (keyword[0], "Public") == 0) {
      g_string_append_printf (message, "  * Everybody can post to the list\r\n");
      g_string_append_printf (message, "  * No custom spam filter is installed.  If you start getting spam, contact the\r\n\
AEGEE Mail Team to set up a filter for you.\r\n");
    } else {
      if (strcasecmp (keyword[0], "Private") == 0) {
	g_string_append_printf (message, "  * Only list subscribers can post to the list\r\n");
      }
    }

    keyword = listserv_getlist_keyword (l, listname, "Attachments");
    g_string_append_printf (message, "  * Attachments are %sallowed\r\n", (strcasecmp (keyword[0], "No") == 0) ? "not ": "");
    g_string_append_printf (message, "  * Messages are limited up to ");
    // Size-Lim
    keyword = listserv_getlist_keyword (l, listname, "Sizelim");
    int size = atol (keyword[0]);
    if (size % 1073741824 == 0)  // 2^30
      g_string_append_printf (message, "%dG", size / 1073741824);
    else if (size % 1048576 == 0)        // 2^20
      g_string_append_printf (message, "%dM", size / 1048576);
    else if (size % 1024 == 0)   // 2^10
      g_string_append_printf (message, "%dK", size / 1024);
    else
      g_string_append_printf (message, "%d", size);
    g_string_append_printf (message, "%s\r\n",
			    (message->str[message->len - 2] == '1'
			    && (message->str[message->len - 1] == 'K'
			     || message->str[message->len - 1] == 'M'
			     || message->str[message->len - 1] == 'G')) ? "Byte" : "Bytes");

    g_string_append_printf(message, welcome_end);
    // generate the message
    // considerations
    // Spam filter
    listname_lower = g_alloca (strlen (listname) + 12);
    g_sprintf (listname_lower, "Welcome to %s", listname);
    char *listname_aegee_org = g_alloca (strlen (listname) + 17);
    g_sprintf (listname_aegee_org, "%s@lists.aegee.org", listname);
    sendmail (listname_aegee_org, listname_lower, message->str);
    g_string_free (message, TRUE);
    g_free (listname);
    break;
  default:
    g_fprintf (stderr, "Unrecognized parameter %s\n", argv[1]);
  }
  listserv_destroy (l);
  //SEND A WELCOME EMAIL
  return 0;
}
