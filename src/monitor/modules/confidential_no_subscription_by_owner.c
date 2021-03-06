//make sure that for lists with Send = Public, Confidential is not No
#include "util.h"
#include <glib.h>

char
confidential_no_subscription_by_owner_LTX_changed_list (struct listserv *l, const char const * listname) {
  if (keyword_does_contain (l, listname, "Confidential", "No")
      && keyword_does_contain (l, listname, "Subscription", "By_Owner"))
    return 1;
  return 0;
}

char*
confidential_no_subscription_by_owner_LTX_listowners_mailing (const char
							      const *listname)
{
  return g_strconcat("\
== List Visibility and Subscription Options ===================================\r\n\
Your list is visible at https://lists.aegee.org . As there are too much lists \r\n\
there, some restrictions apply to the visible lists:\r\n\
  * One list per antenna, project, working group\r\n\
  * No lists for events\r\n\
  * No lists that were not used in the last six months\r\n\
  * No lists where everybody can send messages (to limit the spam you get)\r\n\
  * Only lists where everybody can subscribe\r\n\
\r\n\
Your list does not conform to the last criteria - the subscription request \r\n\
from potential new lists members is subject to approval by the list \r\n\
owners.  Provided that your antenna, project or working group does not yet \r\n\
have a visible mailing lists you can choose between hiding the list or making \r\n\
the subscription process unconditional.  If your body prefers to have another \r\n\
list visible, then you must make this one invisible.\r\n\
\r\n\
To update the settings of ", listname, " go to \r\n\
https://lists.aegee.org/?WIZARD=", listname, "&P=WIZARD_TASK&a=2 \r\n\
and login.  At the end of this email you can find information how to login. \r\n\
\r\n\
To enable everybody to join your list, set for *Subscription* the value _Open_.\r\n\
\r\n\
To make ", listname, " invisible, find the keyword *Confidential* and set its \r\n\
value to \"Yes\".\r\n\
\r\n\
Then click on Safe at the end of the page.", NULL);
}
