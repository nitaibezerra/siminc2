<?php
/**
 * Classe de mapeamento da entidade emendas.beneficiario
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
 * Emendas_Model_Beneficiario
 *
 * @category Class
 * @package  A1
 * @author    <>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 
 * @link     no link
 */
class Emendas_Model_Beneficiario extends Modelo
{
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = 'emendas.beneficiario';

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array(
        'benid',
    );
    /**
     * Chaves estrangeiras.
     * @var array
     */
    protected $arChaveEstrangeira = array(
        'esfid' => array('tabela' => 'territorios.esfera', 'pk' => 'esfid'),
        'proid' => array('tabela' => 'emendas.proponente', 'pk' => 'proid'),
        'suoid' => array('tabela' => 'subunidadeorcamentaria', 'pk' => 'suoid'),
    );

    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributos = array(
        'benid' => null,
        'emeid' => null,
        'suoid' => null,
        'proid' => null,
        'esfid' => null,
        'benformaexecucao' => null,
        'bennumeroprocesso' => null,
        'beninicio' => null,
        'benpprogramanumero' => null,
        'benprogramatitulo' => null,
        'benprogramaobjeto' => null,
        'benstatus' => null,
        'benpropostanumero' => null,
        'benpropostatitulo' => null,
        'benpropostaobjeto' => null,
        'benempenhado' => null,
        'benconveniado' => null,
        'benpago' => null,
        'benobservacao' => null,
        'sicid' => null,
        'impid' => null,
    );


    public function recuperarTodos($stCampos = '*', $arClausulas = null, $stOrdem = null, array $opcoes = array()) {

        $sql = "select  ben.*, replace(to_char(pro.procnpj::numeric, '00:000:000/0000-00'), ':', '.') procnpj, pro.pronome, suo.suocod, suo.suonome, suo.unocod, suo.unonome, suo.unoid, esf.esfdsc,  
                        case 
                                when benformaexecucao = 'S' then 'SICONV'
                                when benformaexecucao = 'D' then 'DIRETA'
                                when benformaexecucao = 'T' then 'TED'
                        end formaexecucao
                from  emendas.beneficiario ben
                        left join emendas.proponente pro on pro.proid = ben.proid
                        left join public.vw_subunidadeorcamentaria suo on suo.suoid = ben.suoid
                        left join territorios.esfera esf on esf.esfid = ben.esfid";

        $sql .= $arClausulas ? " WHERE " . implode(" AND ", $arClausulas) : "";
        $sql .= $stOrdem ? " ORDER BY $stOrdem" : "";

        if ($opcoes['query']) {
            return $sql;
        }

        $resultado = $this->carregar($sql);
        return $resultado ? $resultado : array();
    }

}//end Class
?>