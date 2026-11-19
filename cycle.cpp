//#include <iostream>
//#include <vector>
//#include <intrin.h> 
//
//inline uint64_t read_cycle_count() {
//    _mm_lfence();
//    return __rdtsc();
//}
//
//// std::cout KESİNLİKLE FONKSİYON DIŞINDA OLMALI
//// Parametre dışarıdan referansla geliyor ki derleyici döngüyü silemesin
//int optimize_edilecek_algoritma(const std::vector<int>& data) {
//    int sum = 0;
//    for (size_t i = 0; i < data.size(); i++) {
//        sum += data[i];
//    }
//    return sum;
//}
//
//int optimize_edilecek_algoritma1(const std::vector<int>& data) {
//    int sum1 = 0, sum2 = 0;
//    for (size_t i = 0; i < data.size(); i += 2) {
//        sum1 += data[i];
//        sum2 += data[i + 1];
//    }
//    return sum1 + sum2;
//}
//
//int main() {
//    // 20 yerine 1 Milyon elemanlık bir test verisi oluşturalım
//    std::vector<int> data(1000000, 1);
//
//    // 1. ISINMA TURU (Warm-up): Cache'i ve işlemciyi uyandırır
//    volatile int dummy = optimize_edilecek_algoritma(data);
//
//    // BİRİNCİ ALGORİTMA ÖLÇÜMÜ
//    uint64_t start_cycles = read_cycle_count();
//    volatile int res1 = optimize_edilecek_algoritma(data); // std::cout yok, sadece işlem!
//    uint64_t end_cycles = read_cycle_count();
//
//    std::cout << "Standart For Dongusu: " << (end_cycles - start_cycles) << " cycle\n";
//
//    // 2. ISINMA TURU
//    dummy = optimize_edilecek_algoritma1(data);
//
//    // İKİNCİ ALGORİTMA ÖLÇÜMÜ (ILP Uygulanmış)
//    start_cycles = read_cycle_count();
//    volatile int res2 = optimize_edilecek_algoritma1(data);
//    end_cycles = read_cycle_count();
//
//    std::cout << "ILP (Ikili) For Dongusu: " << (end_cycles - start_cycles) << " cycle\n";
//
//    // Derleyici 'res1' ve 'res2' hesaplamalarını gereksiz görüp silmesin diye 
//    // ekrana en son, ölçüm bittikten sonra yazdırıyoruz.
//    std::cout << "Dogrulama Toplami: " << (res1 + res2) << "\n";
//
//    return 0;
//}