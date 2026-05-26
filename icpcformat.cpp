// nhom 6 (B24DCAT264-B24DCAT269-B24DCAT077-B24DCAT275)

#include <iostream>
#include <vector>

using namespace std;

// Lop Bit: Bieu dien don vi nhi phan trong GF(2)
class Bit {
public:
    int val;
    Bit(int v = 0) : val(v % 2) {} 
    Bit operator+(const Bit& b) const { return Bit(val ^ b.val); }
    Bit operator*(const Bit& b) const { return Bit(val & b.val); }
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
    Bit getBit(int index) const { return bits[index]; }
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

    static Da_thuc Mod_da_thuc(Da_thuc dividend, Da_thuc divisor) {
        int d_deg = dividend.degree();
        int g_deg = divisor.degree();
        int start_deg = d_deg;

        if (g_deg == -1) return Da_thuc(); 

        for (int i = start_deg; i >= g_deg; i--) {
            if (dividend.getBit(i).isOne()) {
                for (int j = 0; j <= g_deg; j++) {
                    Bit current = dividend.getBit(i - g_deg + j);
                    dividend.setBit(i - g_deg + j, current + divisor.getBit(j));
                }
            }
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

    void encode(Da_thuc a_x) {
        int n_k = l - k;
        Da_thuc shifted_a(l - 1);
        
        // Buoc 1: Dinh tuyen a(x) len bac cao bang cach nhan x^(l-k)
        for (int i = 0; i < k; i++) {
            shifted_a.setBit(i + n_k, a_x.getBit(i));
        }
        
        // Buoc 2: Chia lay du de tim p(x)
        Da_thuc p_x = Da_thuc::Mod_da_thuc(shifted_a, g_x);

        // Buoc 3: In
        for (int i = 0; i < n_k; i++) cout << p_x.getBit(i).val;
        for (int i = 0; i < a_x.size(); i++) cout << a_x.getBit(i).val;
        cout << "\n";
    }
};

int main() {
    
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int l, k;
    if (!(cin >> l >> k)) return 0; 

    vector<int> g_coeffs(l - k + 1);
    for (int i = 0; i <= l - k; i++) {
        cin >> g_coeffs[i];
    }
    Da_thuc g_x(g_coeffs);

    vector<int> a_coeffs(k);
    for (int i = 0; i < k; i++) {
        cin >> a_coeffs[i];
    }
    Da_thuc a_x(a_coeffs);

    	// check error

    // Loi 1: Kiem tra bac cua g(x) co bang l - k khong
    if (g_x.degree() != (l - k)) {
        cout << "Error\n";
        return 0;
    }

    // Loi 3: Kiem tra g(x) co phai la uoc cua x^l - 1 hay khong
    Da_thuc xl_1(l); 
    xl_1.setBit(l, Bit(1)); 
    xl_1.setBit(0, Bit(1)); 
    
    Da_thuc rem = Da_thuc::Mod_da_thuc(xl_1, g_x);
    if (rem.degree() != -1) {
        cout << "Error\n";
        return 0;
    }

    // Thuc hien lap ma
    CyclicCode code(l, k, g_x);
    code.encode(a_x);

    return 0;
}
