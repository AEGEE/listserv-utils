#include <liblistserv.h>
#include <glib.h>
#include <string.h>
#include "util.h"

char
no_listowners_LTX_changed_list (struct listserv *l,
				const char const * listname)
{
  return keyword_does_contain (l, listname, "Owner", "nichts@aegee.org") 
    && keyword_does_contain (l, listname, "Configuration-Owner",
			     "nichts@aegee.org");
}

char*
noone_listowner_LTX_listowners_mailing (const char const *listname)
{
  return g_strconcat ("\
== Two Listowners =============================================================\r\n\
The list " , listname, " has only one listowner. All AEGEE Mailing\r\n\
lists must have at least two listowners.  Please agree with a friend to co-\r\n\
manage the mailing list and add her/his email address as second listowner.\r\n\
To do this, go to:\r\n\
  https://lists.aegee.org/?HDRED1=", listname, "\r\n\
, find the line with \"Owner=\", and write after it:\r\n\
  Owner = email@address (Name Surname)\r\n\
where email@address is the email address of your friend, and Name Surname are\r\n\
his/her names.  Click on Safe at the end of the page.  Don't forget to explain\r\n\
your frined how to manage the mailing list.", NULL);
}
