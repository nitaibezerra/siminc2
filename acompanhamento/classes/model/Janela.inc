<?php
/**
 * Classe de mapeamento da entidade acompanhamento.janela
 *
 * @category Class
 * @package  A1
 * @author   ORION TELES DE MESQUITA <orion.mesquita@cultura.gov.br>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 23-01-2018
 * @link     no link
 */


require_once APPRAIZ .'includes/classes/Modelo.class.inc';


/**
 * Acompanhamento_Model_Janela
 *
 * @category Class
 * @package  A1
 * @author    <>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 
 * @link     no link
 */
class Acompanhamento_Model_Janela extends Modelo
{
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = 'acompanhamento.janela';

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array(
        'janid',
    );
    /**
     * Chaves estrangeiras.
     * @var array
     */
    protected $arChaveEstrangeira = array(
        'tipid' => array('tabela' => 'acompanhamento.tipo', 'pk' => 'tipid'),
    );

    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributos = array(
        'janid' => null,
        'jandsc' => null,
        'janinicio' => null,
        'janfim' => null,
        'janstatus' => null,
        'prsano' => null,
        'tipid' => null,
    );

    public function recuperarJanelaAtiva($tipid, $exercicio = null)
    {
        $exercicio = $exercicio ? $exercicio : $_SESSION['exercicio'];
        $sql = "select *, to_char(janinicio, 'DD/MM/YYYY') janinicio, to_char(janfim, 'DD/MM/YYYY') janfim
                from acompanhamento.janela 
                where prsano = '$exercicio'
                and to_char(now(), 'YYYYMMDD') between to_char(janinicio, 'YYYYMMDD') and coalesce(to_char(janfim, 'YYYYMMDD'), '2018-12-31') 
                and tipid = $tipid";

        return $this->pegaLinha($sql);
    }

}//end Class
?>