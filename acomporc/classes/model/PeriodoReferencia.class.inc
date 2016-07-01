<?php
/**
 * Implementação da classe de abstração de períodos de referência.
 *
 * $Id$
 */

/**
 * @see Spo_Model_Periodoreferencia
 */
class Acomporc_Model_PeriodoReferencia extends Spo_Model_Periodoreferencia_Abstract
{
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "acomporc.periodoreferencia";

    /**
     * @inheritdoc
     */
    protected function init()
    {
        // -- A - Ação, S - Subação, T - Tcu
        $this->arAtributos['prftipo'] = null;
    }

    /**
     * @param array $whereAdicional, desconsiderar.
     * @inheritdoc
     */
    public function carregarUltimo(array $whereAdicional = array())
    {
        $whereAdicional = array('prftipo' => $this->arAtributos['prftipo']);
        return parent::carregarUltimo($whereAdicional);
    }

    /**
     * @param array $whereAdicional, desconsiderar.
     * @inheritdoc
     */
    public function carregarAtual(array $whereAdicional = array())
    {
        $whereAdicional = array('prftipo' => $this->arAtributos['prftipo']);
        return parent::carregarAtual($whereAdicional);
    }
}
