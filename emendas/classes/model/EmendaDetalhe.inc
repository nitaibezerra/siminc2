<?php
/**
 * Classe de mapeamento da entidade emendas.emendadetalhe
 *
 * @category Class
 * @package  A1
 * @author   ORION TELES DE MESQUITA <orion.mesquita@cultura.gov.br>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 13-11-2017
 * @link     no link
 */


require_once APPRAIZ .'includes/classes/Modelo.class.inc';


/**
 * Emendas_Model_EmendaDetalhe
 *
 * @category Class
 * @package  A1
 * @author    <>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 
 * @link     no link
 */
class Emendas_Model_EmendaDetalhe extends Modelo
{
    const GND_COD_CUSTEIO_PESSOAL = 1;
    const GND_COD_CUSTEIO_JUROS = 2;
    const GND_COD_CUSTEIO_DESPESAS = 3;
    const GND_COD_CAPITAL_INVESTIMENTO = 4;
    const GND_COD_CAPITAL_INVERSOES = 5;
    const GND_COD_CAPITAL_AMORTIZACAO = 6;
    const GND_COD_CAPITAL_CONTIGENCIA = 9;

    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = 'emendas.emendadetalhe';

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array(
        'emdid',
    );
    /**
     * Chaves estrangeiras.
     * @var array
     */
    protected $arChaveEstrangeira = array(
        'foncod' => array('tabela' => 'public.fonterecurso', 'pk' => 'foncod'),
        'mapcod' => array('tabela' => 'modalidadeaplicacao', 'pk' => 'mapcod'),
        'gndcod' => array('tabela' => 'gnd', 'pk' => 'gndcod'),
    );

    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributos = array(
        'emdid' => null,
        'emeid' => null,
        'gndcod' => null,
        'mapcod' => null,
        'foncod' => null,
        'emdvalor' => null,
    );

    public function recuperarSqlCombo($descricao = null, $where = [], $order = '') {
        $pk = $this->arChavePrimaria[0];

        foreach (array_keys($this->arAtributos) as $atributo) {
            if (substr($atributo, -6) == 'status') {
                $where[] = "{$atributo} = 'A'";
            } elseif (substr($atributo, -3) == 'ano') {
                $where[] = "$atributo = '{$_SESSION['exercicio']}'";
            }
            if ($descricao) {
                $descricao = implode(" || ' - ' || ", (array)$descricao);
            } else {
                $descricao = '*, gndcod';
                $order = 'gndcod';
            }
        }
        $order = $order ? $order : $descricao;
        $where = count($where) ? " where  " . implode(' and ', $where) : '';

        $sql = "select {$pk} AS codigo, {$descricao} AS descricao
                from {$this->stNomeTabela}
                $where
                order by $order   
        ";

        $dados = $this->carregar($sql);

        $dados = $dados ? $dados : [];
        foreach ($dados as $linha) {
            $array[$linha['codigo']] = 'GND: ' . $linha['gndcod'] . ' / Valor: ' . number_format($linha['emdvalor'], 2, ',', '.');
        }

        return $array;
    }

}//end Class
?>