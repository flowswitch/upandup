'''Socket to internal pin mapper'''

__all__ = ['DIP48']


class DIP48:
    '''DIP48 socket pin map. Smaller packages are mapped to the top'''
    _pinmap = (8,  9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31,
            62,  63, 60, 61, 58, 59, 56, 57, 54, 55, 52, 53, 50, 51, 48, 49, 46, 47, 44, 45, 42, 43, 40, 41)


    def __init__(self, n_pins=48):
        self.set_package(n_pins)


    def set_package(self, n_pins):
        '''Slice the map for a smaller package'''
        if n_pins<2 or n_pins>48 or (n_pins & 1):
            raise ValueError("Invalid number of pins for DIP48 socket: %d" % (n_pins))
        self._n_pins = n_pins
        self._map = DIP48._pinmap[0:n_pins//2]+DIP48._pinmap[48-n_pins//2:48]


    def get(self, pin):
        pin -= 1
        if pin<0 or pin>self._n_pins:
            raise ValueError("Pin %d is out of DIP%d package" % (pin+1, self._n_pins))
        return self._map[pin]


if __name__=='__main__':
    print("Testing DIP48:")
    pm = DIP48()
    for pin in range(1, 49):
        print("%d: %d" % (pin, pm.get(pin)))

    print("Testing DIP8:")
    pm.set_package(8)
    for pin in range(1, 9):
        print("%d: %d" % (pin, pm.get(pin)))
