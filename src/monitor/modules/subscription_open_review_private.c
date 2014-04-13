//make sure that Subscribe is either closed, or contains Confirm
#include <glib.h>
#include "util.h"

static inline int core (struct listserv *l,
			const char const *listname) 
{
  return keyword_does_contain (l, listname, "Review", "Private")
    && keyword_does_contain (l, listname, "Subscription", "Open");
}

char
subscription_open_review_private_LTX_changed_list (struct listserv *l,
						   const char const * listname)
{
  if (core (l, listname)) {
    char** review = listserv_getlist_keyword(l, listname, "Review");
    if (review[1] == NULL) {
      keyword_try_update (l, listname, "Review", "Owners");
      return core (l, listname);
    } else return 1;
  }
  return 0;
}


char*
s_ubscription_open_review_private_LTX_listowners_mailing (const char
							 const *listname)
{
  return g_strconcat("\
== List Visibility and Subscription Options ==================================\r\n\
According to your list configuration, everybody can subscribe to the list and\r\n\
at the same time every subscriber can see who else is subscribed to the\r\n\
list.  There are good reasons to have this combination of settings, but\r\n\
unfortunately spammers can misuse the settings, subscribe to your list and\r\n\
get the email addresses of all your subscribers.  To protect the email\r\n\
addresses of your subscribers from the spammers, consider doing one of:\r\n\
\r\n\
 -- restrict who can see the emails of the subscribers.\r\n\
\r\n\
 -- restrict the conditions under which new subscribers can join your list.\r\r\n\
Practically this means, that the listowners shall proceed applications for\r\n\
joining the mailing list and stop by this way the spammers.  Please note, that\r\n\
if you go for this option, your list may not stay visible at\r\n\
https://lists.aegee.org (in case it is already there).\r\n\
\r\n\
To resolve the problem go to\r\n\
  https://lists.aegee.org/cgi-bin/wa?WIZARD=", listname, "&P=WIZARD_TASK&a=2\r\n\
At the end of this email you can find information how to login.\r\n\
\r\n\
To restrict who can see the emails of the subscribers, search for \"Review\"\r\n\
and set it to something different from \"Public\" or \"Private\".\r\n\
\r\n\
To restrict how people subscribe to the list, find the keyword \"Subscription\"\r\n\
and set it to \"By Owner\".\r\n\
\r\n\
Then click on Safe at the end of the page.", NULL);
}
