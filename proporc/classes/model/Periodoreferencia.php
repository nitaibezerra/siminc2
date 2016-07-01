<?php
/**
 *
 * $Id: Periodoreferencia.php 98655 2015-06-16 13:52:40Z maykelbraz $
 */

/**
 *
 */
class Proporc_Model_Periodoreferencia extends Spo_Model_Periodoreferencia_Abstract
{
    protected $stNomeTabela = 'proporc.periodoreferencia';

    protected function init()
    {
    }

    public function queryTodosPeriodosCombo()
    {
        return <<<DML
SELECT t1.prfid AS codigo,
       t1.prsano || ' - ' || t1.prftitulo AS descricao
  FROM {$this->stNomeTabela} t1
DML;
    }

}
