def s(x, y, w, h):
    if y == 0: return x
    return s(y - 1, w - 1 - x, h - 1, w) + w


def s2(x, y, w, h):
    m = min(x, y, w-1-x, h-1-y)
    outer = sum((w - m_*2) * 2 + (h - m_*2) * 2 - 4 for m_ in range(m))
    outer = m * 2 * (w + h - 2*m)
    _x = x - m;
    _y = y - m;
    _w = w - 2 * m;
    _h = h - 2 * m;
    if _y == 0: return outer + _x;
    elif _x == _w - 1: return outer + _w + _y - 1;
    elif _y == _h - 1: return outer + _w + _h + _w - 3 - _x;
    elif _x == 0: return outer + _w + _h + _w + _h - 4 - _y;
    else: return "!{}{}".format(x, y)

w = 10
h = 14

for y in range(h):
    print(*(s(x, y, w, h) for x in range(w)), sep = "\t")

print('--------------')

for y in range(h):
    print(*(s2(x, y, w, h) for x in range(w)), sep = "\t")
