import json
import time

I2C_ADDRESS = 0x35

class Directory:
    def __init__(self, path):
        self.path = path
        self.subdirectories = []
        self.files = []

    def __repr__(self):
        str = self.path + '\n' + '\n'.join(self.files)
        for dir in self.subdirectories:
            str += f'\n{dir}'
        return str

class I2CAudio:
    def __init__(self, i2c, address=I2C_ADDRESS):
        self._bus = i2c;
        self._address = address;

        self._files = None

        # maintain state so we only make calls to get_state when needed
        self._started = False

    def _str_to_int(self, input_str):
        if input_str[0] == "-":
            is_negative = True
            input_str = input_str[1:]
        else:
            is_negative = False

        output_int = 0

        for i in input_str:
            output_int = output_int * 10 + ord(i) - ord("0")

        if is_negative:
            output_int *= -1

        return output_int

    def _command(self, command):
        return bytes(command.encode())

    def _send_command(self, command):
        try:
            self._bus.try_lock()
            self._bus.writeto(0x35, self._command(command))
        finally:
            self._bus.unlock()

    def _get_data(self, command, length):
        try:
            self._bus.try_lock()
            response = bytearray(length)
            self._bus.writeto_then_readfrom(
                self._address,
                self._command(command),
                response)
            return response.decode()
        finally:
            self._bus.unlock()

    def _get_state(self):
        return self._get_data('{ "command": "get_state" }', 14)

    def _get_files(self):
        l = self._get_data('{ "command": "get_files_length" }', 10)
        length = self._str_to_int(l)
        if length > 0:
            return self._get_data('{ "command": "list_files" }', length)

    def connect(self):
        while True:
            files = self._get_files()
            if files:
                break
            else:
                time.sleep(0.5)
        
        self._files = Directory('/')
        for file in json.loads(files):
            dir = self._files
            dir_path = []
            for path in file.split('/')[:-1]:
                if path:
                    dir_path.append(path)
                    dir_path_string = '/' + '/'.join(dir_path)
                    if path not in dir._subdirectories:
                        dir._subdirectories.append(dir_path_string)
                    dir = [sd for sd in dir._subdirectories if sd.path == path][0]
            dir.files.append(file)

    def get_files(self):
        return self._files

    def ready_for_next_track(self):
        return not self._started or '1' in self._get_state()

    def play_next(self, path):
        self._send_command(f'{{ "command": "play_next", "file": "{path}" }}')
        self._started = True

    def stop_playing(self):
        self._send_command('{ "command": "stop_playing" }')
        self._started = False

    def set_volume(self, volume):
        self._send_command(f'{{ "command": "set_volume", "volume": "{volume}" }}')
