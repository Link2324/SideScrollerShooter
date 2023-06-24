#ifndef SOUND_H_INCLUDED
#define SOUND_H_INCLUDED

void initSounds(void);
void loadMusic(char *filename);
void playMusic(int loop);
void playSound(int id, int channel);

#endif // SOUND_H_INCLUDED
