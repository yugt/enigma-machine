#include "enigma.cpp"
#include "parser.cpp"

int main(int argc, char* argv[])
{
    Enigma em;
    if(argc==1)
    {// forward, CLI for human
        std::printf("Enigma interactive mode\n");
        string line;
        while(std::getline(std::cin, line)){
            Parser p(line);
            if(p.parse()){
                switch (p.mode)
                {
                case 'a':
                case 'b':
                case 'c':
                    std::fprintf(stderr, "TODO\n");
                    break;
                case 'd':
                    line = em.decrypt(p.key, p.comb, p.msg);
                    break;
                case 'e':
                    line = em.encrypt(p.key, p.comb, p.msg);
                    break;
                default:
                    assert(false);
                }
                std::cout << line << std::endl;
            }
            else{
                std::fprintf(stderr,
                "usage: m(ode) wheel(0-5) key msg\n");
            }
        }
    }
    else
    {// backward, brute-force cracking
        std::vector<std::string> arguments(argv + 1, argv + argc);
        // std::cout << em.encrypt("HOT", 0, "COMEBACKSOON") << std::endl;
        // std::cout << em.decrypt("HOT", 0, "Q96YRBQJD99C") << std::endl;
    }
    return 0;
}