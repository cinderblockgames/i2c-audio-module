String listFilesAsJson() {
  String directory = "/";
  int fileCount = countFiles(directory);

  String files[fileCount];
  int order[fileCount];
  fillFiles(directory, files, fileCount);

  String result = "[ ";
  for (int i = 0; i < fileCount; i++) {
    result += "\"" + files[i] + "\", ";
  }
  result += "]";
  return result;
}

void fillFiles(String path, String* files, int count) {
  File dir = SD.open(path);
  for (int i = 0; i < count; i++) {
    files[i] = path + nextFile(dir).name();
  }
  dir.close();
}

int countFiles(String path) {
  File dir = SD.open(path);
  int count = 0;
  while (true) {
    File next = nextFile(dir);
    if (!next) {
      break;
    }
    count++;
  }
  dir.close();
  return count;
}

File nextFile(File &dir) {
  File entry = dir.openNextFile();
  if (!entry) {
    return entry;
  }

  while (entry.name()[0] == '.' || entry.isDirectory()) {
    // Skip hidden files/folders.
    entry.close();
    entry = dir.openNextFile();
    if (!entry) {
      return entry;
    }
  }
  return entry;
}
