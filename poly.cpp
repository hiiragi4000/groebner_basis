#include<cassert>
#include<vector>
#include<map>
#include<string>
#include<iostream>
#include"fraction.h"

#define pb push_back
#define mp make_pair

template<class TOS> // TOS: totally ordered set
bool operator <(const vector<TOS> &a, const vector<TOS> &b){
    if(a.empty()) return !b.empty();
    for(int i=0; i<=(int)a.size()-1; i++){
        if(i == (int)b.size()) return false;
        if(a[i] < b[i]) return true;
        else if(b[i] < a[i]) return false;
    }
    return a.size() < b.size();
}

template<class R, unsigned int n> // R: a ring, n: the number of the variables
class polynomial{
typedef vector<unsigned int> index;
typedef pair<index, R> monomial;
private:
    map<index, R> p;
    void refresh(){
        for(auto it=p.begin(); it!=p.end(); ){
            if(it->second == 0) it = p.erase(it);
            else it++;
        }
    }
public:
    polynomial(){
        p.clear();
    }
    polynomial(const monomial &m){
        p.clear();
        assert(m.first.size() == n);
        assert(m.second != 0);
        p.insert(m);
    }
    polynomial(const vector<monomial> &v){
        p.clear();
        for(int i=0; i<=(int)v.size()-1; i++){
            assert(v[i].first.size() == n);
            if(p.find(v[i].first) == p.end()){
                p[v[i].first] = 0;
            }
            p[v[i].first] += v[i].second;
        }
        refresh();
    }
    polynomial(const polynomial<R, n> &another){
        p.clear();
        for(auto it=another.p.begin(); it!=another.p.end(); it++) p.insert(*it);
    }
    polynomial(const R &c){
        p.clear();
        if(c == 0) return;
        index zeros(n, 0u);
        p[zeros] = c;
    }
    ~polynomial(){
        p.clear();
    }
    polynomial<R, n> operator +(const polynomial<R, n> &another) const{
        vector<monomial> v;
        for(auto it=p.begin(); it!=p.end(); it++) v.pb(*it);
        for(auto it=another.p.begin(); it!=another.p.end(); it++) v.pb(*it);
        return v;
    }
    polynomial<R, n> operator -() const{
        polynomial<R, n> addinv(*this);
        for(auto it=addinv.p.begin(); it!=addinv.p.end(); it++) it->second = -it->second;
        return addinv;
    }
    polynomial<R, n> operator -(const polynomial<R, n> &another) const{
        return *this + (-another);
    }
    polynomial<R, n> operator *(const polynomial<R, n> &another) const{
        vector<monomial> v;
        for(auto it=p.begin(); it!=p.end(); it++){
            for(auto jt=another.p.begin(); jt!=another.p.end(); jt++){
                v.pb(mp(it->first, it->second * jt->second));
                for(int k=0; k<=(int)n-1; k++) v.back().first[k] += jt->first[k];
            }
        }
        return v;
    }
    polynomial<R, n> operator +=(const polynomial<R, n> &another){
        return *this = *this + another;
    }
    polynomial<R, n> operator -=(const polynomial<R, n> &another){
        return *this = *this - another;
    }
    polynomial<R, n> operator *=(const polynomial<R, n> &another){
        return *this = *this * another;
    }
    bool operator ==(const polynomial<R, n> &another) const{
        auto it = p.begin(), jt = another.p.begin();
        for(; it!=p.end() && jt!=another.p.end(); it++, jt++){
            for(int k=0; k<=(int)n-1; k++){
                if(it->first[k] != jt->first[k]) return false;
            }
            if(it->second != jt->second) return false;
        }
        return it == p.end() && jt == another.p.end();
    }
    bool operator !=(const polynomial<R, n> &another) const{
        return !(*this == another);
    }
    void print(const string &s) const{
        assert(s.size() == n);
        if(p.empty()){
            cout << 0 << endl;
            return;
        }
        for(auto it=p.end(); it!=p.begin(); ){
            if(it != p.end()) cout << " + ";
            it--;
            cout << '(' << it->second << ')';
            for(int i=0; i<=(int)n-1; i++){
                if(it->first[i] == 0u) continue;
                cout << s[i];
                if(it->first[i] >= 2u) cout << '^' << it->first[i];
            }
        }
        cout << endl;
    }
    monomial LT() const{
        assert(!p.empty());
        auto it = p.end(); it--;
        return *it;
    }
};
// F is required to be a field in the following functions.
template<class F, unsigned int n>
polynomial<F, n> poly_S(const polynomial<F, n> &f, const polynomial<F, n> &g){
    assert(f != F(0)); assert(g != F(0));
    auto lt1 = f.LT(), lt2 = g.LT();
    vector<unsigned int> deg(n);
    for(int i=0; i<=(int)n-1; i++){
        deg[i] = max(lt1.first[i], lt2.first[i]);
    }
    pair<vector<unsigned int>, F> m1, m2;
    m1.first.resize(n); m2.first.resize(n);
    for(int i=0; i<=(int)n-1; i++){
        m1.first[i] = deg[i] - lt1.first[i];
        m2.first[i] = deg[i] - lt2.first[i];
    }
    m1.second = 1/lt1.second;
    m2.second = 1/lt2.second;
    return f*m1 - g*m2;
}
template<class F, unsigned int n>
void poly_division(const polynomial<F, n> &f, const vector<polynomial<F, n>> &g, vector<polynomial<F, n>> &q, polynomial<F, n> &r){
    assert(!g.empty());
    int m = g.size();
    for(int i=0; i<=m-1; i++){
        assert(g[i] != F(0));
    }
    polynomial<F, n> ff = f;
    q.resize(m); r = F(0);
    for(int i=0; i<=m-1; i++) q[i] = F(0);
    while(ff != F(0)){
        bool chg = false;
        for(int i=0; i<=m-1; i++){
            auto lt = ff.LT(), lt2 = g[i].LT();
            bool chg2 = true;
            vector<unsigned int> deg(n);
            for(int j=0; j<=(int)n-1; j++){
                if(lt.first[j] < lt2.first[j]){
                    chg2 = false; break;
                }
                deg[j] = lt.first[j] - lt2.first[j];
            }
            if(!chg2) continue;
            chg = true;
            auto dqi = mp(deg, lt.second/lt2.second);
            q[i] += dqi; ff -= g[i] * dqi;
            break;
        }
        if(!chg){
            r += ff.LT(); ff -= ff.LT();
        }
    }
}
template<class F, unsigned int n>
void groebner_basis(const vector<polynomial<F, n>> &f, vector<polynomial<F, n>> &g){
    assert(!f.empty());
    for(int i=0; i<=(int)f.size()-1; i++) assert(f[i] != F(0));
    g = f;
    while(1){
        bool chg = false;
        for(int i=1; i<=(int)g.size()-1; i++) for(int j=0; j<=i-1; j++){
            vector<polynomial<F, n>> q;
            polynomial<F, n> r;
            poly_division(poly_S(g[i], g[j]), g, q, r);
            if(r != F(0)){
                g.pb(r); chg = true;
            }
        }
        if(!chg) return;
    }
}

int main(){
    typedef fraction Q;
    typedef vector<unsigned int> index;
    typedef pair<index, Q> mono;
    typedef polynomial<Q, 4> poly;
    index idx(4);
    vector<mono> v0, v1, v2;
    idx[0] = 1, idx[1] = 0, idx[2] = 1, idx[3] = 0; v0.pb(mp(idx, 1));
    idx[0] = 0, idx[1] = 0, idx[2] = 0, idx[3] = 3; v0.pb(mp(idx,-1));
    idx[0] = 1, idx[1] = 0, idx[2] = 0, idx[3] = 2; v1.pb(mp(idx, 1));
    idx[0] = 0, idx[1] = 4, idx[2] = 0, idx[3] = 0; v1.pb(mp(idx,-1));
    idx[0] = 0, idx[1] = 4, idx[2] = 2, idx[3] = 0; v2.pb(mp(idx, 1));
    idx[0] = 0, idx[1] = 0, idx[2] = 0, idx[3] = 5; v2.pb(mp(idx,-1));
    vector<poly> ecchi(3);
    ecchi[0] = v0; ecchi[1] = v1; ecchi[2] = v2;
    cout << "Given generator(s):" << endl;
    ecchi[0].print("xyzw");
    ecchi[1].print("xyzw");
    ecchi[2].print("xyzw");
    vector<poly> hentai;
    groebner_basis(ecchi, hentai);
    cout << "A Groebner basis (without the reduction):" << endl;
    for(int i=0; i<=(int)hentai.size()-1; i++){
        hentai[i].print("xyzw");
    }
    return 0;
}

