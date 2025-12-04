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
    copier.setActive(true);
    copier.begin(decoder, audioFile);
  }
  state = ready;
}

void stopPlaying() {
  playNext = empty;
  cleanUp();
  state = ready;
  stopFlag = false;
}

void cleanUp() {
  copier.setActive(false);
  copier.end();
  audioFile.close();
}