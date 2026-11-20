#include <iostream>
#include <cmath>
#include <iomanip>
#include <string>
using namespace std;
const double PI = 3.1415926535; // em cho dài dài tí cho đỡ sai số ạ
const double SIGMA = 5.67e-8;

// đề cho
const double Ro_a = 1.0;
const double Cp_a = 1005.0;
const double V = 2.0;
const double Ch = 0.005;       // 0.001 - 0.01
// đất khô và đất 40% ẩm
const double Ro_kho = 1600.0;
const double Cp_kho = 800.0;
const double Ro_am = 2000.0;
const double Cp_am = 1480.0;

// em thêm hàm này để input cái lớp đất với tách trường hợp đất khô với ẩm
void nhapLopDat(double &d, double &Ro_soil, double &Cp_soil) {
    cout<<"Nhập độ dài lớp đất(m): ";
    cin>>d;
    string loai;
    cout<<"Nhập loại đất (kho / am): ";
    cin>>loai;
    if (loai == "am" || loai == "AM" || loai == "ẩm") {
        Ro_soil = Ro_am;
        Cp_soil = Cp_am;
        cout<<"Chọn đất ẩm 40% (Ro = " << Ro_soil << ", Cp = " << Cp_soil << ")\n";
    } else {
        Ro_soil = Ro_kho;
        Cp_soil = Cp_kho;
        cout<<"Chọn đất khô (Ro = " << Ro_soil << ", Cp = " << Cp_soil << ")\n";
    }
}
void giai(char cau, double d, double Ro_soil, double Cp_soil, double A) {
    double Ts =20.0; // 6h là 20 độ
    double t15 =0, t04 = 0;
    double dt = 1.0;  // cách 1 giây
    for (int giay =0; giay <=22*3600; giay++) {
        if (giay == 9*3600) t15 = Ts;
        if (giay == 22*3600) {
            t04 = Ts;
            break;
        }
        double t = giay/3600.0;
        // sw: 6h- 18h
        double SW = (t <=12.0) ? 1000.0 * sin(PI* t / 12.0):0.0;
        // biên độ 4-6
        double Ta = 23.0 +A * sin(2.0 *PI*t/ 24.0);
        double LWa = SIGMA*pow(Ta +273.15, 4);
        // lws
        double LWs = SIGMA*pow(Ts +273.15, 4);
        // qh
        double H = Ch*V*(Ta - Ts);
        double Qh = Ro_a*Cp_a*H;

        // E của các câu
        double E = 0;
        if (cau == 'a') E = SW;
        else if (cau == 'b') E = SW- LWs;
        else if (cau == 'c') E = SW +LWa - LWs;
        else if (cau == 'd') E = SW+LWa - LWs +Qh;

        Ts+= (E * dt) / (Ro_soil * Cp_soil * d);
    }
    cout<<"Cau "<< cau<< ": 15h00 = "<< t15 << " do | 04h = "<< t04 << " do\n";
}

int main() {
    double d, Ro_soil, Cp_soil;
    nhapLopDat(d, Ro_soil, Cp_soil);
    cout<<fixed << setprecision(2) <<"\n";

    // câu a và b không phụ thuộc Ta
    giai('a', d, Ro_soil, Cp_soil, 5.0);
    giai('b', d, Ro_soil, Cp_soil, 5.0);

    // câu c,d chạy từ 4-6 độ
    for (double A=4.0; A<= 6.0; A+= 1.0) {
        cout<<"\n Biên đô Ta = " << A << " độ\n";
        giai('c', d, Ro_soil, Cp_soil, A);
        giai('d', d, Ro_soil, Cp_soil, A);
    }
    return 0;
}