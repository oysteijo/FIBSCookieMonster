/*--- clip.h -------------------------------
 * CLIP message IDs
 *
 * Modified by Fergy for use with FIBSCookieMonster.
 * Further modified by Øystein Schønning-Johansen
 * ------------------------------------------ */

#define CLIP_VERSION 1009  /* Latest standard of February 2016 */
enum {
	CLIP_WELCOME = 1,
	CLIP_OWN_INFO,
	CLIP_MOTD_BEGIN,
	CLIP_MOTD_END,
	CLIP_WHO_INFO,
	CLIP_WHO_END,
	CLIP_LOGIN,
	CLIP_LOGOUT,
	CLIP_MESSAGE,
	CLIP_MESSAGE_DELIVERED,
	CLIP_MESSAGE_SAVED,
	CLIP_SAYS,
	CLIP_SHOUTS,
	CLIP_WHISPERS,
	CLIP_KIBITZES,
	CLIP_YOU_SAY,
	CLIP_YOU_SHOUT,
	CLIP_YOU_WHISPER,
	CLIP_YOU_KIBITZ,
	CLIP_ALERT
};
#define CLIP_LAST_CLIP_ID CLIP_ALERT /* Remember to updater at new revisions of CLIP */
