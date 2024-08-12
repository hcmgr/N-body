#include <cstdlib>

/**
 * Collection of utility functions
 */
namespace Utils {

    /**
     * Returns rand() bounded to interval [min, max]
     */
    int bounded_rand(int min, int max)  {
        return rand() % (max-min+1) + min;
    }
}