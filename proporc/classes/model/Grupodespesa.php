<?php
/**
 * Classe de mapeamento da entidade proporc.grupodespesa.
 *
 * $Id: Grupodespesa.php 101146 2015-08-11 21:22:58Z maykelbraz $
 */

/**
 * Mapeamento da entidade proporc.grupodespesa.
 *
 * @see Modelo
 */
class Proporc_Model_Grupodespesa extends Modelo
{
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = 'proporc.grupodespesa';

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array(
        'gdpid',
    );

    /**
     * Chaves estrangeiras.
     * @var array
     */
    protected $arChaveEstrangeira = array(
        'prfid' => array('tabela' => 'proporc.periodoreferencia', 'pk' => 'prfid'),
    );

    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributos = array(
        'gdpid' => null,
        'gdpnome' => null,
        'gdpordem' => null,
        'gdpobservacao' => null,
        'prfid' => null,
        'vlrmontante' => null,
    );

    public function carregarGruposDespesa(array $criterio = array(), $retornarQuery = false)
    {
        $sql = <<<DML
SELECT gdpid,
       gdpordem,
       gdpnome
  FROM {$this->stNomeTabela} t1
DML;
        $where = array();
        foreach ($criterio as $campo => &$valor) {
            if ('gdpnome' == $campo) {
                $where[] = "{$campo} ILIKE :{$campo}";
                $valor = "%{$valor}%";
            } else {
                $where[] = "{$campo} = :{$campo}";
            }
        }

        if (!empty($where)) {
            $where = implode(' AND ', $where);
            $sql .= <<<DML
  WHERE {$where}
DML;
        }
        $sql .= <<<DML
  ORDER BY gdpordem
DML;

        $dml = new Simec_DB_DML($sql);
        $dml->addParams($criterio);

        if ($retornarQuery) {
            return (string)$dml;
        }

        return $this->carregar($dml);
    }

    /**
     * Antes de excluir um grupo de despesa, verifica se não há nenhuma categoria cadastrada para ele.
     * @param type $id
     * @return type
     */
    public function antesExcluir($id = null)
    {
        $despesas = new Proporc_Model_Despesa();
        $categorias = $despesas->recuperarTodos('dspid',array("gdpid = {$id}"));
        if ($categorias) {
            foreach ($categorias as $cat) {
                $despesas->clearDados();
                $despesas->dspid = $cat['dspid'];

                if (!$despesas->excluir()) {
                    return false;
                }
                $despesas->commit();
            }
        }

        return true;
    }
}
