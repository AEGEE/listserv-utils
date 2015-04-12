#include <liblistserv.h>
#include <glib.h>
#include <string.h>

static const char const * keywords[] = {"Owner", "Configuration-Owner", NULL};

char
one_listowner_LTX_changed_list (struct listserv *l, const char const * listname)
{
  char **emails = listserv_getemails_fromkeywords(l, listname, keywords);
  int i = 0, j = 0;
  while (emails[i]) {
    if (g_ascii_strcasecmp (emails[i],  "nichts@aegee.org") == 0)
      return 0;
    if (g_ascii_strcasecmp (emails[i], "owner@aegee.org") != 0)
      j++;
    if (j > 1) return 0;
    i++;
  }
  if (j == 1) return 1;
  return 0;
}

char*
one_listowner_LTX_listowners_mailing (const char const *listname)
{
  return g_strconcat ("\
== Only one listowner ======================================================\r\n\
The list ", listname, " has only one listowner.  For every AEGEE \r\n\
member comes a moment when her/his mailbox gets invalid.  This is irrespecitve \r\n\
of the fact how involved this person was in projects, or how many years has \r\n\
s/he spent working for the common good.  At some moment in the future you will \r\n\
choose another email provider and your mailbox will get invalid.  While you as \r\n\
listowner are free to change to another email provider, it comes very often \r\n\
that the listowners forget to update their email address that manages their \r\n\
AEGEE mailing lists.  In turn the lists don't have anymore a listowner and it \r\n\
is a lot of work for the AEGEE Mail Team to find the current addresses of the \r\n\
listonwers or take the right decision what to do with the mailing list.\r\n\
\r\n\
To avoid this situation, which is real and does happen, all AEGEE mailing \r\n\
lists must have at least two listowners.  Please agree with a friend to \r\n\
co-manage the mailing list and add her/his email address as second listowner. \r\n\
To do this, go to \r\n\
https://lists.aegee.org/?HDRED1=", listname, ". \r\n\
(At the end of this email is explained how to login.)  Find the line with \r\n\
\"Owner=\", and write after it:\r\n\
  Owner = email@address (Name Surname)\r\n\
where email@address is the email address of your friend, and Name Surname are \r\n\
his/her names.  Click on Safe at the end of the page.  Don't forget to explain \r\n\
your friend how to manage the mailing list.", NULL);
}
