#include<algorithm>
#include<iostream>
#include<cassert>
#include<string>
#include<vector>
#include<array>

using std::array, std::string, std::vector;

class Enigma
{
    const string wA = ".ABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890";
    const string wB = ".A1YZD8BX2VLF7IOK0TW3EUMC9SRP4GJN6H5Q";
    const string wC = ".YXZACB1ED2GF3IH4KJ5ML6ON7QP8SR9UT0WV";
    enum wheel { ABC, ACB, BAC, BCA, CAB, CBA };
    std::unordered_map<string, array<array<int,3>, CBA+1>> lookup_table;
    const int teeth = wA.length();
    string const *wl, *wm, *wr; 

    int modulo(int a) { return (teeth + (a % teeth)) % teeth; }
    
    auto& load_state(string& key, int wheel_pos)
    {
        assert(lookup_table.find(key) != lookup_table.end());
        assert(put_wheel(wheel_pos));
        return lookup_table[key][wheel_pos];
    }
    
    bool put_wheel(int comb)
    {
        if(comb<(int)ABC || comb>(int)CBA){
            std::fprintf(stderr, "Error: wheel config\n");
            return false;
        }
        switch (comb)
        {
            case ABC:
                wl = &wA; wm = &wB; wr = &wC; break;
            case ACB:
                wl = &wA; wm = &wC; wr = &wB; break;
            case BAC:
                wl = &wB; wm = &wA; wr = &wC; break;
            case BCA:
                wl = &wB; wm = &wC; wr = &wA; break;
            case CAB:
                wl = &wC; wm = &wA; wr = &wB; break;
            case CBA:
                wl = &wC; wm = &wB; wr = &wA; break;
            default:
                assert(false);
        }
        return true;
    }

    void rotate(int w, array<int,3>& start, int target) // bind physics
    {
        int sign = (w%2 == 0) ? 1 : -1;
        int dist = modulo(target - start[w]);
        for(auto& c : start){
            c = modulo(c + sign * dist);
            sign *= -1;
        }
    }

    bool validate(string& key)
    {
        bool valid = (key.length()==3);
        valid &= (lookup_table.find(key) != lookup_table.end());
        if(!valid) std::fprintf(stderr, "Error: key\n");
        return valid;
    }

    bool verify(string& msg)
    {
        bool valid = true;
        for(auto c : msg)
            valid &= (wA.find_first_of(c) != string::npos);
        if(!valid) std::fprintf(stderr, "Error: message\n");
        return valid;
    }

public:
    Enigma()
    {
        assert(teeth == wB.length());
        assert(teeth == wC.length());
        lookup_table.reserve(teeth * teeth * teeth);
        for(int comb=ABC; comb<=CBA; comb++){
            assert(put_wheel(comb));
            for(int i=0; i<teeth; i++){
                for(int j=0; j<teeth; j++){
                    for(int k=0; k<teeth; k++){
                        string key({wl->at(i), wm->at(j), wr->at(k)});
                        array<int, 3> state({i, j, k});
                        lookup_table[key].at(comb) = state;
                    }
                }
            }
        }
        wl = nullptr; wm = nullptr; wr = nullptr;
    }

    string encrypt(string key, int wheel_pos, string msg)
    {
        if(!put_wheel(wheel_pos) || !validate(key) || !verify(msg))
            return "";
        auto state = load_state(key, wheel_pos);
        string const* readers[2] = {wl, wm};
        string cypher;
        for(int p=0; p<msg.length(); p++){
            rotate(2, state, wr->find_first_of(msg.at(p)));
            cypher += readers[p%2]->at(state[p%2]);
        }
        return cypher;
    }

    string decrypt(string key, int wheel_pos, string msg)
    {
        if(!put_wheel(wheel_pos) || !validate(key) || !verify(msg))
            return "";
        auto state = load_state(key, wheel_pos);
        string const* readers[2] = {wl, wm};
        string plaintext;
        for(int p=0; p<msg.length(); p++){
            rotate(p%2, state, readers[p%2]->find_first_of(msg.at(p)));
            plaintext += wr->at(state[2]);
        }
        return plaintext;
    }
};