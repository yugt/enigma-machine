from enigma import Enigma
import readline

def main():
    enigma = Enigma()
    enigma.help()

    command_handlers = {
        'a': enigma.all,
        'b': enigma.hint_wheel,
        'c': enigma.hint_key,
        'd': enigma.decrypt,
        'e': enigma.encrypt,
        'q': lambda _: exit()
    }

    while True:
        try:
            user_input = input("Enigma> ").strip().split()
        except EOFError:
            print("\nExiting...")
            break
        if not user_input:
            continue

        command = user_input[0]
        args = user_input[1:]

        handler = command_handlers.get(command)
        if handler:
            handler(args)
        else:
            enigma.help()

if __name__ == "__main__":
    main()