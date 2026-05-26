// nhom 6 (B24DCAT264-B24DCAT269-B24DCAT077-B24DCAT275)
    
#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <algorithm>
#include <cmath>
#include <random>   
#include <chrono>   

using namespace std;

// Lop Bit: Bieu dien don vi nhi phan trong GF(2)
class Bit {
public:
    int val;

    Bit(int v = 0) : val(v % 2) {} 

    // Phep add trong GF(2) tuong duong XOR
    Bit operator+(const Bit& b) const {
        return Bit(val ^ b.val);
    }

    // Phep mul trong GF(2) tuong duong AND
    Bit operator*(const Bit& b) const {
        return Bit(val & b.val);
    }

    bool isOne() const { return val == 1; }
};

//Lop Vector nhi phan - Tu ma
class BiVec {
protected:
    vector<Bit> bits;

public:
    BiVec() {}
    BiVec(int size) : bits(size, Bit(0)) {}
    BiVec(vector<int> v) {
        for (int x : v) bits.push_back(Bit(x));
    }

    int size() const { return bits.size(); }

    void setBit(int index, Bit b) {
        if (index >= 0 && index < bits.size()) bits[index] = b;
    }

    Bit getBit(int index) const {
        return bits[index];
    }

    void print() const {
        for (const auto& b : bits) {
            cout << b.val;
        }
    }
};

// Lop Da thuc tren GF(2)
class Da_thuc : public BiVec {
public:
    Da_thuc() : BiVec() {}
    Da_thuc(int degree) : BiVec(degree + 1) {}
    Da_thuc(vector<int> v) : BiVec(v) {}

    int degree() const {
        for (int i = bits.size() - 1; i >= 0; i--) {
            if (bits[i].isOne()) return i;
        }
        return -1; 
    }

    static string termStr(int deg) {
        if (deg == 0) return "1";
        if (deg == 1) return "x";
        return "x^" + to_string(deg);
    }

    static string format_dathuc(Da_thuc p) {
        string res = "";
        bool first = true;
        for (int i = p.degree(); i >= 0; i--) {
            if (p.getBit(i).isOne()) {
                if (!first) res += " + ";
                res += termStr(i);
                first = false;
            }
        }
        return res.empty() ? "0" : res;
    }

    static string align_dathuc(Da_thuc p, int max_deg) {
        string res = "";
        bool first = true;
        for (int i = max_deg; i >= 0; i--) {
            if (p.getBit(i).isOne()) {
                string s = (first ? "  " : "+ ") + termStr(i);
                while(s.length() < 7) s += " "; 
                res += s;
                first = false;
            } else {
                res += string(7, ' '); 
            }
        }
        res.erase(res.find_last_not_of(" ") + 1); 
        if (res.empty()) return "  0";
        return res;
    }

    // Thuat toan chia da thuc 
    static Da_thuc Mod_da_thuc(Da_thuc dividend, Da_thuc divisor, bool tu_luan) {
        int d_deg = dividend.degree();
        int g_deg = divisor.degree();
        int start_deg = d_deg;

        if (g_deg == -1) return Da_thuc(); 

        Da_thuc temp_div = dividend;
        Da_thuc quo;
        if (d_deg >= g_deg) {
            quo = Da_thuc(d_deg - g_deg);
            for(int i = start_deg; i >= g_deg; i--) {
                if(temp_div.getBit(i).isOne()) {
                    quo.setBit(i - g_deg, Bit(1));
                    for(int j = 0; j <= g_deg; j++) {
                        Bit curr = temp_div.getBit(i - g_deg + j);
                        temp_div.setBit(i - g_deg + j, curr + divisor.getBit(j));
                    }
                }
            }
        } else {
            quo = Da_thuc(0); 
        }

        int print_line_num = 0;
        int width = (start_deg + 1) * 7 + 4; 

        auto print_with_right_panel = [&](string left_str) {
            if (!tu_luan) return; 
            
            string right_str = "";
            if (print_line_num == 0) {
                right_str = "| " + format_dathuc(divisor);
            } else if (print_line_num == 1) {
                right_str = "|-";
                int len1 = format_dathuc(divisor).length();
                int len2 = format_dathuc(quo).length();
                for(int k = 0; k <= max(len1, len2); k++) right_str += "-";
            } else if (print_line_num == 2) {
                right_str = "| " + format_dathuc(quo);
            }
            
            string pad = "";
            if (print_line_num < 3) {
                int pad_len = width - left_str.length();
                if (pad_len < 2) pad_len = 2;
                pad = string(pad_len, ' ');
            }
            cout << left_str << pad << right_str << endl;
            print_line_num++;
        };

        if (tu_luan) {
            cout << "\n=== QUA TRINH CHIA DA THUC ===" << endl;
            cout << "--------------------------------------------------\n" << endl;
        }

        for (int i = start_deg; i >= g_deg; i--) {
            if (dividend.getBit(i).isOne()) {
                Da_thuc shifted_divisor(i);
                for (int j = 0; j <= g_deg; j++) {
                    shifted_divisor.setBit(i - g_deg + j, divisor.getBit(j));
                }

                if (tu_luan) {
                    string line1 = align_dathuc(dividend, start_deg);
                    print_with_right_panel(line1);

                    string line2 = align_dathuc(shifted_divisor, start_deg);
                    size_t first_char = line2.find_first_not_of(" ");
                    if (first_char != string::npos && first_char >= 2) {
                        line2[first_char - 2] = '^'; 
                    }
                    print_with_right_panel(line2);

                    size_t start = line2.find_first_not_of(" ");
                    size_t end = line2.length() - 1;
                    string line3 = string(start_deg * 7 + 7, ' ');
                    if (start != string::npos) {
                        for(int j = start; j <= end; j++) line3[j] = '-';
                    }
                    print_with_right_panel(line3);
                }

                for (int j = 0; j <= g_deg; j++) {
                    Bit current = dividend.getBit(i - g_deg + j);
                    dividend.setBit(i - g_deg + j, current + divisor.getBit(j));
                }
            }
        }

        if (tu_luan) {
            string final_line = align_dathuc(dividend, start_deg);
            print_with_right_panel(final_line); 
            
            while(print_line_num < 3) print_with_right_panel("");
            cout << "\n==================================================\n" << endl;
        }

        return dividend;
    }
};

// Lop CyclicCode
class CyclicCode {
    int l, k;
    Da_thuc g_x;

public:
    CyclicCode(int _l, int _k, Da_thuc _g) : l(_l), k(_k), g_x(_g) {}

    void encode(string messageStr, bool tu_luan) {
        vector<int> a_coeffs;
        for (char c : messageStr) {
            a_coeffs.push_back(c - '0');
        }
        Da_thuc a_x(a_coeffs);

        if (tu_luan) {
            cout << "\n(1) Mo ta khoi tin bang bieu dien da thuc tuong ung a(x) = ";
            cout << Da_thuc::format_dathuc(a_x) << endl;
        }

        int n_k = l - k;
        Da_thuc shifted_a(l - 1);
        for (int i = 0; i < k; i++) {
            shifted_a.setBit(i + n_k, a_x.getBit(i));
        }
        if (tu_luan) {
            cout << "(2) Tinh a^(" << n_k << ")(x) = x^" << n_k << " * a(x) = ";
            cout << Da_thuc::format_dathuc(shifted_a) << endl;
        }

        if (tu_luan) {
            cout << "(3) Chia x^" << n_k << " * a(x) cho da thuc sinh g(x) cua bo ma, thu duoc phan du p(x)." << endl;
        }
        
        Da_thuc p_x = Da_thuc::Mod_da_thuc(shifted_a, g_x, tu_luan);
        
        if (tu_luan) {
            cout << "=> Phan du p(x) = " << Da_thuc::format_dathuc(p_x) << "\n" << endl;
        }

        Da_thuc c_x = shifted_a; 
        for(int i = 0; i < n_k; i++) {
            c_x.setBit(i, p_x.getBit(i)); 
        }

        if (tu_luan) {
            cout << "(4) Thanh lap da thuc ma c(x) = p(x) + x^" << n_k << " * a(x)" << endl;
            cout << "    c(x) = " << Da_thuc::format_dathuc(c_x) << endl;
            cout << "In ra tu ma tuong ung voi da thuc ma c(x):" << endl;
        }

        if (!tu_luan) {
             cout << "Tu ma he thong: ";
        } else {
             cout << "=> ";
        }
        
        for (int i = 0; i < n_k; i++) cout << p_x.getBit(i).val;
        for (int i = 0; i < a_x.size(); i++) cout << a_x.getBit(i).val;
        cout << endl;
    }
};

// Ham ho tro phan tich chuoi da thuc 
vector<int> phan_tich_da_thuc_input(string s) {
    vector<int> coeffs(1, 0); 
    
    s.erase(remove(s.begin(), s.end(), ' '), s.end());
    
    stringstream ss(s);
    string term;
    while (getline(ss, term, '+')) {
        int deg = 0;
        if (term == "1") deg = 0;
        else if (term == "x") deg = 1;
        else if (term.find("x^") != string::npos) {
            deg = stoi(term.substr(2));
        }
        
        if (deg >= coeffs.size()) {
            coeffs.resize(deg + 1, 0);
        }
        coeffs[deg] = 1;
    }
    return coeffs;
}



// Cau truc luu tru cau hoi
struct quiz {
    string question;
    vector<pair<string, bool>> options; // pair<Noi dung dap an, Co phai dap an dung khong>
};

void gen_trac_nghiem() {
    // Ngan hang 8 cau hoi ve Ma vong C(l, k)
    vector<quiz> bank = {
        {
            "Dieu kien de mot da thuc g(x) la da thuc sinh cua ma vong C(l, k) la gi?",
            {
                {"x^l + 1 (hoac x^l - 1) phai chia het cho g(x)", true},
                {"g(x) phai chia het cho x^l + 1", false},
                {"g(x) phai co bac la l", false},
                {"l phai chia het cho k", false}
            }
        },
        {
            "Trong ma vong he thong C(l, k), bac cua da thuc sinh g(x) phai bang bao nhieu?",
            {
                {"l - k", true},
                {"k", false},
                {"l", false},
                {"l + k", false}
            }
        },
        {
            "Da thuc phan du p(x) thu duoc khi chia x^(l-k)*a(x) cho g(x) dong vai tro gi?",
            {
                {"Cac bit kiem tra chan le (Check bits)", true},
                {"Cac bit ban tin (Message bits)", false},
                {"Da thuc sinh (Generator)", false},
                {"Tu ma hoan chinh (Codeword)", false}
            }
        },
        {
            "Voi ma vong C(l, k), chieu dai toi da cua khoi ban tin (message) la bao nhieu bit?",
            {
                {"k bit", true},
                {"l bit", false},
                {"l - k bit", false},
                {"l + k bit", false}
            }
        },
        {
            "Trong thuat toan chia da thuc tren truong huu han GF(2), phep tru duoc thay the bang phep toan nao?",
            {
                {"Phep XOR", true},
                {"Phep AND", false},
                {"Phep OR", false},
                {"Phep NOT", false}
            }
        },
        {
            "Neu bieu dien tu bac thap den bac cao, da thuc a(x) = 1 + x^2 + x^3 ung voi chuoi nhi phan nao?",
            {
                {"1011", true},
                {"1101", false},
                {"1001", false},
                {"1110", false}
            }
        },
        {
            "Neu tu ma c(x) thu duoc tai phia nhan chia het cho g(x) (phan du = 0) thi he thong ket luan gi?",
            {
                {"Qua trinh truyen khong phat hien thay loi", true},
                {"Qua trinh truyen chac chan co loi", false},
                {"Tu ma bi sai it nhat 1 bit", false},
                {"Ban tin ban dau toan bit 0", false}
            }
        },
        {
            "Trong bo ma Hamming (7, 4), moi tu ma se co so luong bit kiem tra (check bits) la bao nhieu?",
            {
                {"3 bit", true},
                {"4 bit", false},
                {"7 bit", false},
                {"1 bit", false}
            }
        }
    };

    // Khoi tao random engine bang thoi gian thuc he thong
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::default_random_engine engine(seed);

    cout << "\n==================================================" << endl;
    cout << "      CAU HOI TRAC NGHIEM ON TAP MA VONG" << endl;
    cout << "==================================================\n" << endl;

    // In cau hoi ra man hinh
    for (int i = 0; i < bank.size(); i++) {
        cout << "Cau " << i + 1 << ": " << bank[i].question << endl;
        
        // Xao tron vi tri cac dap an cua cau hoi nay
        std::shuffle(bank[i].options.begin(), bank[i].options.end(), engine);
        
        char label = 'A';
        char dung = 'A';
        
        // In A, B, C, D va tim xem dau la dap an dung de luu lai
        for (const auto& opt : bank[i].options) {
            cout << "  " << label << ". " << opt.first << endl;
            if (opt.second == true) {
                dung = label;
            }
            label++;
        }
        
        // In ra dap an dung cua cau (sau khi da bi random)
        cout << "  -> Dap an dung: " << dung << "\n" << endl;
    }
    
    cout << "==================================================" << endl;
}

int main() {
    int l, k;
    string g_str, m_str;
    int choice;
    int choice_quiz;

    cout << "Nhap l k: ";
    cin >> l >> k;
    cin.ignore();

    cout << "Nhap da thuc sinh g(x) (VD: 1 + x + x^3): ";
    getline(cin, g_str);

    cout << "Nhap ban tin m (chuoi nhi phan): ";
    cin >> m_str;

    Da_thuc g_x(phan_tich_da_thuc_input(g_str));

    // check error
    if (g_x.degree() != (l - k)) {
        cout << "Error\n";
        return 0;
    }

    if (m_str.length() > k) {
        cout << "Error\n";
        return 0;
    }

    Da_thuc xl_1(l); 
    xl_1.setBit(l, Bit(1)); 
    xl_1.setBit(0, Bit(1)); 
    
    Da_thuc rem = Da_thuc::Mod_da_thuc(xl_1, g_x, false);
    
    if (rem.degree() != -1) {
        cout << "Error\n";
        return 0;
    }

    cout << "\nBan co muon hien thi cac buoc tu luan khong? (1: Co, 0: Khong): ";
    cin >> choice;
    bool tu_luan = (choice == 1);

    CyclicCode code(l, k, g_x);

    // Thuc hien lap ma 
    code.encode(m_str, tu_luan);

    // Goi ham trac nghiem o cuoi cung
    cout << "\nBan co muon xem cau hoi trac nghiem on tap khong? (1: Co, 0: Khong): ";
    cin >> choice_quiz;
    if (choice_quiz == 1) {
        gen_trac_nghiem();
    }

    return 0;
}
