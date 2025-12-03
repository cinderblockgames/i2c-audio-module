import board
import time
from i2c_audio import I2CAudio

#i2c = board.STEMMA_I2C()  # Adafruit QT Py RP2040
i2c = board.I2C()  # Seeed XIAO RP2040

audio = I2CAudio(i2c)
audio.connect()

audio.set_volume(30)
print(audio.get_files())
while True:
    if (audio.ready_for_next_track()):
        print('ready')
        audio.play_next('/mp3.mp3')
    print('sleep')
    time.sleep(10)
