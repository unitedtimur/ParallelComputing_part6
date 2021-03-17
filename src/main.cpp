#include <iostream>
#include <random>
#include <algorithm>
#include <experimental/iterator>
//#include <omp.h>

template <typename T>
void printVector(const std::vector<T> &vector);

template <typename T = double>
double scalarProduct(const std::vector<T> &firstVector, const std::vector<T> &secondVector);

int main()
{
    size_t sizeVector = 0;

    std::cout << "Enter the size of vectors: " << std::flush;
    std::cin >> sizeVector;

    std::default_random_engine dre(0);
    std::uniform_real_distribution<> uid(-228, 322);

    std::vector<double> firstVector(sizeVector);
    std::vector<double> secondVector(sizeVector);

    for (size_t i = 0; i < sizeVector; ++i) {
        firstVector[i] = uid(dre);
        secondVector[i] = uid(dre);
    }

    printVector<double>(firstVector);
    printVector<double>(secondVector);

    std::cout << "Scalar product is equal: " << scalarProduct<double>(firstVector, secondVector) << std::endl;

    return 0;
}

template <typename T>
void printVector(const std::vector<T> &vector)
{
    std::copy(std::begin(vector),
              std::end(vector),
              std::experimental::make_ostream_joiner(std::cout, ", "));

    std::cout << std::endl;
}

template <typename T>
double scalarProduct(const std::vector<T> &firstVector, const std::vector<T> &secondVector)
{
    double result = 0;
    size_t i = 0;

    // shared - все потоки видят одну и ту же переменную
    // private, локальные - каждый поток видит свой экземпляр данной переменной, но не инициализирует её
    #pragma omp parallel shared(firstVector, secondVector) num_threads(4)
    {
        #pragma omp for private(i) reduction(+:result)
        for (i = 0; i < std::max(firstVector.size(), secondVector.size()); i++) {
            result += firstVector[i] * secondVector[i];
        }
    }
    return result;
}
