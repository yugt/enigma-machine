from itertools import product

class Enigma:
    def __init__(self):
        self.wA = ".ABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890"
        self.wB = ".A1YZD8BX2VLF7IOK0TW3EUMC9SRP4GJN6H5Q"
        self.wC = ".YXZACB1ED2GF3IH4KJ5ML6ON7QP8SR9UT0WV"
        self.teeth = len(self.wA)
        self.wheels = [ None, None, None ]
        self.wheel_order = {"ABC": 1, "ACB": 2, "BAC": 3, "BCA": 4, "CAB": 5, "CBA": 6}
        self.lookup_table = {f"{self.wA[i]}{self.wB[j]}{self.wC[k]}":
            [] for i, j, k in product(range(self.teeth), repeat=3)}
        self.init_lookup_table()


    def help(self):
        print("""
usage: 
a: cyphertext (decrypt 6 * 37^2 times)
b: wheel cyphertext (decrypt 37^2 times)
c: key cyphertext (decrypt 6 times)
d: decrypt, wheel key cyphertext
e: encrypt, wheel key plaintext
h: print this help message
q: quit
        """)

    def all(self, args):
        usage = "Usage: a <cyphertext>"
        if len(args) < 1:
            print("Invalid number of arguments.\n" + usage)
            return
        result = [self._shared(" ".join(args),
            self.parse_order_key(order+1, key)) for key in
            list(self.lookup_table.keys())[:self.teeth * self.teeth]
            for order in range(6)]
        result = list(set(result))
        if len((result)) != self.teeth * self.teeth * 6:
            print(f"{len(result)} != {self.teeth * self.teeth * 6}.")
        print(result)

    def hint_wheel(self, args):
        usage = "Usage: b <wheel_order> <cyphertext>"
        if len(args) < 2:
            print("Invalid number of arguments.\n" + usage)
            return
        order = self.validate_wheel(args[0])
        if order is None:
            return
        result = [self._shared(" ".join(args[1:]),
            self.parse_order_key(order+1, key)) for key in
            list(self.lookup_table.keys())[:self.teeth*self.teeth]]
        result = list(set(result))
        if len((result)) != self.teeth * self.teeth:
            print(f"{len(result)} != {self.teeth * self.teeth}.")
        print(result)

    def hint_key(self, args):
        usage = "Usage: c <key> <cyphertext>"
        if len(args) < 2:
            print("Invalid number of arguments.\n" + usage)
            return
        key = self.validate_key(args[0])
        if key is None:
            return
        print([self._shared(" ".join(args[1:]),
                self.parse_order_key(wheel_order+1, key))
                for wheel_order in range(6)])

    def decrypt(self, args):
        usage = "Usage: d <wheel_order> <key> <cyphertext>"
        if len(args) < 3:
            print("Invalid number of arguments.\n" + usage)
            return
        state = self.parse_order_key(args[0], args[1])
        if state is None:
            return
        print(self._shared(" ".join(args[2:]), state))

    def encrypt(self, args):
        usage = "Usage: e <wheel_order> <key> <plaintext>"
        if len(args) < 3:
            print("Invalid number of arguments.\n" + usage)
            return
        state = self.parse_order_key(args[0], args[1])
        if state is None:
            return
        print(self._shared(" ".join(args[2:]), state, decrypt=False))

    def _shared(self, cyphertext, state, decrypt=True):
        # Record the positions of characters that are not in the wheel
        positions = [ i for i, char in enumerate(cyphertext)
                        if char.upper() not in self.wA ]
        # Remove characters that not in the wheel from the message
        msg = [char.upper() for char in cyphertext if char.upper() in self.wA]

        ### simulate rotation of wheels ###
        if decrypt:
            for i in range(len(msg)):
                self.rotate(i%2, state, self.wheels[i%2].index(msg[i]))
                msg[i] = self.wheels[2][state[2]]
        else:# encrypt
            for i in range(len(msg)):
                self.rotate(2, state, self.wheels[2].index(msg[i]))
                msg[i] = self.wheels[i%2][state[i%2]]

        # Insert characters not in the wheel back into the message
        [ msg.insert(_, cyphertext[_]) for _ in positions ]
        return "".join(msg)

    def parse_order_key(self, order, key):
        order = self.validate_wheel(order)
        if order is None:
            return None

        key = self.validate_key(key)
        if key is None:
            return None

        self.put_wheel(order)
        return list(self.lookup_table[key][order])

    def init_lookup_table(self):
        for comb in range(6):
            self.put_wheel(comb)
            for i, j, k in product(range(self.teeth), repeat=3):
                key = f"{self.wheels[0][i]}{self.wheels[1][j]}{self.wheels[2][k]}"
                self.lookup_table[key].append((i, j, k))

    def validate_wheel(self, order):
        if isinstance(order, int) and 1 <= order <= 6:
            return order - 1
        elif order.isdigit() and 1 <= int(order) <= 6:
            return int(order) - 1
        elif order.upper() in self.wheel_order:
            return self.wheel_order[order.upper()] - 1
        else:
            print(f"Invalid wheel order {order} (case insensitive).\n"+\
            "Must be one of ABC, ACB, BAC, BCA, CAB, CBA or an integer from 1 to 6.")
            return

    def validate_key(self, key):
        if key.upper() not in self.lookup_table:
            print(f"Invalid key {key}.\n"+\
            "Must be a 3-character key from the wheel characters.")
            return
        return key.upper()

    def put_wheel(self, comb):
        if comb == 0:
            self.wheels = [ self.wA, self.wB, self.wC ]
        elif comb == 1:
            self.wheels = [ self.wA, self.wC, self.wB ]
        elif comb == 2:
            self.wheels = [ self.wB, self.wA, self.wC ]
        elif comb == 3:
            self.wheels = [ self.wB, self.wC, self.wA ]
        elif comb == 4:
            self.wheels = [ self.wC, self.wA, self.wB ]
        elif comb == 5:
            self.wheels = [ self.wC, self.wB, self.wA ]
        else:
            raise ValueError("Unknown wheel combination")
        
    def rotate(self, w, start, target):
        sign = 1 if w % 2 == 0 else -1
        dist = (target - start[w]) % self.teeth
        start[0] = (start[0] + sign * dist) % self.teeth
        start[1] = (start[1] - sign * dist) % self.teeth
        start[2] = (start[2] + sign * dist) % self.teeth