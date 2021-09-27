
__all__ = ['GPIO256']

class GPIO256:
    def __init__(self, up):
        self.up = up
        self.gpio_oe = bytearray(32)
        self.gpio_out = bytearray(32)
        self.gpio_in = None


