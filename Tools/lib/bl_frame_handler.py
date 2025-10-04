import yaml
import struct

class BLFrameHandler:
    def __init__(self, config_path: str):
        self.config_path = config_path
        self.config = self.__load_config()
        self.frames_names = list(self.config.keys())

        # Create BLFrame objects for each frame in the config
        self.frames = {name: BLFrame(name, self.config[name]) for name in self.frames_names}

    def __load_config(self)-> dict:
        with open(self.config_path, 'r') as file:
            config = yaml.safe_load(file)
        return config['frames']
    
    def get_frame(self, name: str)-> 'BLFrame':
        return self.frames.get(name, None)
    
class BLFrame(BLFrameHandler):
    def __init__(self, name:str, frame_dict: dict):
        self.name = name
        self.frame_dict = frame_dict
        self.payload = self.frame_dict['payload']

        self.data_type_dict = {
            'uint8': ('B', 1),
            'int8': ('b', 1),
            'uint16': ('H', 2),
            'int16': ('h', 2),
            'uint32': ('I', 4),
            'int32': ('i', 4),
            'float32': ('f', 4),
            'float64': ('d', 8)
        }

        self.byteorder = self.frame_dict['byteorder']
        self.lengths = []
        self.formats = []
        self.labels = []
        self.total_length = 0
        self._fields = []

        self.__parse_payload()

    def __sort_payload(self)-> list:
        # Make copy to avoid modifying original
        entries = list(self.payload)

        # Sorting function
        def get_pos(e):
            if 'pos' not in e:
                raise KeyError(f"Missing 'pos' key in entry {e}")
            else:
                return e.get('pos')
            
        entries.sort(key=get_pos)
        return entries

    def __parse_payload(self):
        sorted_payload = self.__sort_payload()

        # build labels, formats, lengths, and precomputed fields
        self._fields = []
        total = 0
        for e in sorted_payload:
            name = e.get('signal_name')
            dtype = e.get('datatype')

            if name is None:
                raise KeyError(f"Field {e} missing signal_name")

            if dtype is None:
                raise KeyError(f"Field {name} missing data type")

            # map human type to struct fmt and size
            if dtype in self.data_type_dict:
                fmt_char, size = self.data_type_dict[dtype]
            else:
                raise ValueError(f"Unsupported data type: {dtype}")

            total += size
            
            self.labels.append(name)
            self.formats.append(fmt_char)
            self.lengths.append(size)

            pos = e.get('pos')
            self._fields.append((name, fmt_char, pos))

        self.total_length = total

    def unpack(self, b: bytes) -> dict:
        out = {}
        idx = 0

        if self.byteorder == 'little':
            endian = '<'
        elif self.byteorder == 'big':
            endian = '>'
        else:
            raise ValueError(f"Invalid byteorder: {self.byteorder}")

        for i, length in enumerate(self.lengths):
            if idx + length > len(b):
                raise ValueError('Not enough binary data for frame')
            chunk = b[idx:idx+length]
            idx += length
            fmt = self.formats[i]
            if fmt == 'B':
                out[self.labels[i]] = chunk[0]
            else:
                out[self.labels[i]] = struct.unpack(endian + fmt, chunk)[0]
        return out

if __name__ == "__main__":
    handler = BLFrameHandler('Scripts/bl_frames.yaml')
    
    log_frame = handler.get_frame('log_frame')
    print(log_frame.labels)


      