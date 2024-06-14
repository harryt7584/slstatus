#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mpd/client.h>

#include "../util.h"

const char *
media_mpd_stat()
{
        struct mpd_connection *conn;
        struct mpd_status *status;
        struct mpd_song *song;
        char *mpc_title;
	char *mpc_artist;
        char ret[64];
	unsigned state;	

        if (!((conn = mpd_connection_new(NULL, 0, 0)) || 
                    mpd_connection_get_error(conn)))
                return NULL;

        mpd_command_list_begin(conn, true);
        mpd_send_status(conn);
        mpd_send_current_song(conn);
        mpd_command_list_end(conn);
        status = mpd_recv_status(conn);
        if (status) {
                state = mpd_status_get_state(status);
                if (state == MPD_STATE_PLAY) {
                        mpd_response_next(conn);
                        song = mpd_recv_song(conn);
                        mpc_title = (char *)mpd_song_get_tag(song,
                                MPD_TAG_TITLE, 0);
			mpc_artist = (char *)mpd_song_get_tag(song,
                                MPD_TAG_ARTIST, 0);
                        snprintf(ret, 64, "%s - %s",
                                mpc_artist, mpc_title);
                        free(mpc_title);
			free(mpc_artist);
                } else if (state == MPD_STATE_PAUSE) {
                        mpd_response_next(conn);
                        song = mpd_recv_song(conn);
                        mpc_title = (char *)mpd_song_get_tag(song,
                                MPD_TAG_TITLE, 0);
			mpc_artist = (char *)mpd_song_get_tag(song,
                                MPD_TAG_ARTIST, 0);
                        snprintf(ret, 64, "No Song Playing");
                        free(mpc_title);
			free(mpc_artist);
                } else if (state == MPD_STATE_STOP) {
                        mpd_response_next(conn);
                        if ((song = mpd_recv_song(conn))) {
                            mpc_title = (char *)mpd_song_get_tag(song,
                                    MPD_TAG_TITLE, 0);
			    mpc_artist = (char *)mpd_song_get_tag(song,
                                MPD_TAG_ARTIST, 0);
                            snprintf(ret, 64, "No Song Playing");
                            free(mpc_title);
			    free(mpc_artist);
                        } else {
                            snprintf(ret, 64, "No Song Playing");
                        }
                } else {
                        /* state == MPD_STATE_UNKNOWN */
                        snprintf(ret, 64, "No Song Playing");
                } 
        } else {
                return NULL;
	}
        mpd_response_finish(conn);
        mpd_connection_free(conn);
        strcpy(buf, ret);

        return buf;
}

