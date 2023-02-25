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
                    em.decrypt(p.msg);
                    break;
                case 'b':
                    em.decrypt(p.comb, p.msg);
                    break;
                case 'c':
                    em.decrypt(p.key, p.msg);
                    break;
                case 'd':
                    em.decrypt(p.key, p.comb, p.msg);
                    break;
                case 'e':
                    em.encrypt(p.key, p.comb, p.msg);
                    break;
                default:
                    assert(false);
                }
                std::cout << p.msg << std::endl;
            }
            else{
                std::fprintf(stderr,
                "usage: m(ode) wheel(0-5) key msg\n");
            }
        }
    }
    else
    {// direct input are cypher
        assert(argc == 2);
        std::string cypher(argv[1]);
        em.decrypt(cypher);
        std::cout << cypher;
    }
    return 0;
}