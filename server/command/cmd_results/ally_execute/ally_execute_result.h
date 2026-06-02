#ifndef ALLY_EXECUTE_RESULT_H
#define ALLY_EXECUTE_RESULT_H

#include "resurrect_result.h"

// Resultado devuelto por ally.execute()
// TODO tiene que contener todos los results de las interacciones con aliados y constructores que convengan
struct AllyExecuteResult {
    ResurrectResult resurrect_result;
    // TODO agregar campos faltantes

    // Se sobrecarga en función al aliado invocado
    explicit AllyExecuteResult(const ResurrectResult& result);
};

#endif  // ALLY_EXECUTE_RESULT_H
