void setNext(String filepath) {
  if (filepath.endsWith(".mp3")) {
    print("next:" + filepath);
    playNext = filepath;
    state = not_ready;
  }
}

void startPlaying() {
  if (playNext != empty) {
    audioFile = SD.open(playNext);
    playNext = empty;
    copier.begin(decoder, audioFile);
  }
  state = ready;
}

void stopPlaying() {
  playNext = empty;
  copier.end();
  audioFile.close();
  state = ready;
}
