#include <iostream>
#include <thread>
#include <chrono>
#include <vector>

void f1() {
    std::this_thread::sleep_for(std::chrono::milliseconds(200));
    std::cout << "Mi primer hilo\n";
}

void ejemplo_1_fork_join() {
    std::thread t1;
    t1 = std::thread(f1);
    std::cout << "Hello, World!" << std::endl;
    t1.join();
}

void ejemplo_2_fork_detach() {
    std::thread t1;
    t1 = std::thread(f1);
    std::cout << "Hello, World!" << std::endl;
    t1.detach();
}

void ejemplo_3_fork_join() {
    std::jthread t1;
    t1 = std::jthread(f1);
    t1.join();
    std::cout << "Hello, World!" << std::endl;
}

void f2(int par1) {
    std::cout << "El valor recepcionado del hilo es: "<< par1 << std::endl;
}

void ejemplo_4_fork_join() {
    int x = 10;
    std::jthread t1;
    t1 = std::jthread(f2, x);
    std::cout << "El valor original del hilo principal es: " << x << std::endl;
}

void f3(int& par1) {
    par1 += 10;
}
void ejemplo_5_fork_join() {
    int x = 10;
    std::jthread t1;
    t1 = std::jthread(f3, std::ref(x));
    t1.join();
    std::cout << "El valor original del hilo principal es: " << x << std::endl;
}


void ejemplo_6_fork_join() {
    int x = 10;
    std::jthread t1([](auto par){ par += 10;}, x);
    t1.join();
    std::cout << "El valor original del hilo principal es: " << x << std::endl;
    std::jthread t2([&x](){ x += 10; });
    t2.join();
    std::cout << "El valor original del hilo principal es: " << x << std::endl;
}

class Rectangle {
    int base {};
    int altura {};
public:
    Rectangle() = default;
    Rectangle(int base, int altura): base(base), altura(altura) {}
    void calcular_area(int& area) {  area = base * altura; }
    void calcular_perimetro(int& perimetro) { perimetro = (base + altura) * 2; }
};

class Rectangle2 {
    int base {};
    int altura {};
public:
    Rectangle2() = default;
    Rectangle2(int base, int altura): base(base), altura(altura) {}
    [[nodiscard]] int calcular_area() const {return base * altura; }
    [[nodiscard]] int calcular_perimetro() const {return (base + altura) * 2; }
};

void ejemplo_7_hilos_con_clases() {
    Rectangle r(20, 30);
    int a2 = 0;
    std::jthread t1(&Rectangle::calcular_area, &r, std::ref(a2));

    Rectangle2 r2(20, 30);
    int a3 = 0;
    std::jthread t2([r2, &a3](){ a3 = r2.calcular_area(); });

    t1.join();
    t2.join();
    std::cout << " " << a2 << std::endl;
    std::cout << " " << a3 << std::endl;
}

void sumar(int a, int b, int& result) { result = a + b; }
void restar(int a, int b, int& result) { result = a - b; }

void ejemplo_8_coleccion_de_hilos() {
    std::vector<std::jthread> vt;
    int x = 10;
    int y = 20;
    int r1 = 0;
    int r2 = 0;
    vt.emplace_back(sumar, x, y, std::ref(r1));
    vt.push_back(std::jthread(sumar, x, y, std::ref(r2)));

    for (auto& t: vt) t.join();
    std::cout << " " << r1 << std::endl;
    std::cout << " " << r2 << std::endl;
}



int main() {
//    ejemplo_1_fork_join();
//    ejemplo_2_fork_detach();
//    ejemplo_3_fork_join();
//    ejemplo_4_fork_join();
//    ejemplo_5_fork_join();
//    ejemplo_6_fork_join();
//    ejemplo_7_hilos_con_clases();
    ejemplo_8_coleccion_de_hilos();
    return 0;
}