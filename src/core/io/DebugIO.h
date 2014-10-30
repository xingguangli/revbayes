#ifndef DEBUG_IO_H
#define DEBUG_IO_H
void debug_bool_vec_flf(const char *filename, const size_t line_num, const char * func_name, const char * prefix, const std::vector<bool> &);
template<typename T> void debug_vec_flf(const char *filename, const size_t line_num, const char * func_name, const char * prefix, const T * ptr, size_t n);
template<typename T> void debug_mat(const char * prefix, const T * ptr, size_t n, size_t m);
template<typename T> void debug_value(const char * pref, const T &x);

#define debug_vec(p, x, s) (debug_vec_flf(__FILE__, __LINE__, __FUNCTION__, p, x, s))
#define debug_bool_vec(p, v) (debug_bool_vec_flf(__FILE__, __LINE__, __FUNCTION__, p, v))

#if defined (REVBAYES_DEBUG_OUTPUT)
inline void debug_bool_vec_flf(const char *filename, const size_t line_num, const char * func_name, const char * pref, const std::vector<bool> &v) {
    std::cerr << "file: " << filename;
    std::cerr << " line_num: " << line_num;
    std::cerr << " func_name: " << func_name;
    std::cerr << pref << " at " << (long) &(v) << " = [";
    for (size_t i = 0; i < v.size(); ++i) {
        if (i != 0) {
            std::cerr << ", ";
        }
        std::cerr << v[i];
    }
    std::cerr << "]\n";
}
template<typename T>
inline void debug_vec_flf(const char *filename, const size_t line_num, const char * func_name, const char * pref, const T *x, size_t s) {
    std::cerr << "file: " << filename;
    std::cerr << " line_num: " << line_num;
    std::cerr << " func_name: " << func_name;
    std::cerr << pref << " at " << (long) x << " = [";
    for (size_t i = 0; i < s; ++i) {
        if (i != 0) {
            std::cerr << ", ";
        }
        std::cerr << x[i];
    }
    std::cerr << "]\n";
}
template<typename T>
inline void debug_mat(const char * pref, const T *x, size_t n, size_t m) {
    std::cerr << pref << " at " << (long) x << " =  [";
    for (size_t j = 0; j < n; ++j) {
        if (j != 0) {
            std::cerr << ",";
        }
        std::cerr << "\n  ";
        for (size_t i = 0; i < m; ++i) {
            if (i != 0) {
                std::cerr << ", ";
            }
            std::cerr << x[j*m + i];
        }
    }
    std::cerr << "]\n";
}

template<typename T>
inline void debug_value(const char * pref, const T &x) {
    std::cerr << pref << " =  " << x << "\n";
}

#else

template<typename T>
inline void debug_bool_vec_flf(const char *filename, const size_t line_num, const char * func_name, const char * pref, const std::vector<bool> &v) {
}

template<typename T>
inline void debug_vec_flf(const char *filename, const size_t line_num, const char * func_name, const char * , const T *, size_t){
}

template<typename T>
inline void debug_mat(const char * , const T *, size_t , size_t ) {
}

template<typename T>
inline void debug_value(const char * pref, const T &x) {
}

#endif
#endif
