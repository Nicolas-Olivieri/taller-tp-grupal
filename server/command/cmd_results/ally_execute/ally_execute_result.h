#ifndef ALLY_EXECUTE_RESULT_H
#define ALLY_EXECUTE_RESULT_H

#include "buy_result.h"
#include "heal_result.h"
#include "list_items_result.h"
#include "resurrect_result.h"
#include "sell_result.h"


// Resultado devuelto por ally.execute()
// TODO tiene que contener todos los results de las interacciones con aliados y constructores que convengan
struct AllyExecuteResult {
    // TODO agregar campos faltantes
    BuyResult buy;
    HealResult heal;
    ListItemsResult list_items;
    ResurrectResult resurrect;
    SellResult sell;

    // Se sobrecarga en función al aliado invocado
    explicit AllyExecuteResult(const bool was_bounded);

    explicit AllyExecuteResult(const BuyResult& result);

    explicit AllyExecuteResult(const HealResult& result);

    explicit AllyExecuteResult(const ListItemsResult& result);

    explicit AllyExecuteResult(const ResurrectResult& result);

    explicit AllyExecuteResult(const SellResult& result);
};


#endif  // ALLY_EXECUTE_RESULT_H
