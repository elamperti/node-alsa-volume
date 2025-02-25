// get-mute is a small utility for ALSA mixer, written for being used
// with Conky.
//
// This code is in Public Domain
//
// Reference:
//  http://www.alsa-project.org/alsa-doc/alsa-lib/index.html
//
// Author:
//  2021 Enrico Lamperti

#include "alsa-getmute.h"

const snd_mixer_selem_channel_id_t CHANNEL = SND_MIXER_SCHN_FRONT_LEFT;

static void error_close_exit(char *errmsg, int err, snd_mixer_t *h_mixer)
{
	if (err == 0)
		fprintf(stderr, errmsg);
	else
		fprintf(stderr, errmsg, snd_strerror(err));
	if (h_mixer != NULL)
		snd_mixer_close(h_mixer);
	exit(EXIT_FAILURE);
}

NAN_METHOD(getMute)
{
	int err;
	int mute_state;
	snd_mixer_t *h_mixer;
	snd_mixer_selem_id_t *sid;
	snd_mixer_elem_t *elem;

	Nan::Utf8String nan_device(info[0]);
	std::string str_device(*nan_device);
	const char* device = str_device.c_str();

	Nan::Utf8String nan_selem_name(info[1]);
	std::string str_selem_name(*nan_selem_name);
	const char* selem_name = str_selem_name.c_str();

	if ((err = snd_mixer_open(&h_mixer, 1)) < 0)
		error_close_exit("Mixer open error: %s\n", err, NULL);

	if ((err = snd_mixer_attach(h_mixer, device)) < 0)
		error_close_exit("Mixer attach error: %s\n", err, h_mixer);

	if ((err = snd_mixer_selem_register(h_mixer, NULL, NULL)) < 0)
		error_close_exit("Mixer simple element register error: %s\n", err, h_mixer);

	if ((err = snd_mixer_load(h_mixer)) < 0)
		error_close_exit("Mixer load error: %s\n", err, h_mixer);

	snd_mixer_selem_id_alloca(&sid);
	snd_mixer_selem_id_set_index(sid, 0);
	snd_mixer_selem_id_set_name(sid, selem_name);

	if ((elem = snd_mixer_find_selem(h_mixer, sid)) == NULL)
		error_close_exit("Cannot find simple element\n", 0, h_mixer);

	snd_mixer_selem_get_playback_switch(elem, CHANNEL, &mute_state);

	snd_mixer_close(h_mixer);
	info.GetReturnValue().Set((bool)!mute_state);
}
