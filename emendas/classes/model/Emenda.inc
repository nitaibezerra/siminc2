<?php
/**
 * Classe de mapeamento da entidade emendas.emenda
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
 * Emendas_Model_Emenda
 *
 * @category Class
 * @package  A1
 * @author    <>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 
 * @link     no link
 */
class Emendas_Model_Emenda extends Modelo
{
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = 'emendas.emenda';

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array(
        'emeid',
    );
    /**
     * Chaves estrangeiras.
     * @var array
     */
    protected $arChaveEstrangeira = array(
        'prsano' => array('tabela' => 'emendas.programacaoexercicio', 'pk' => 'prsano'),
        'autid' => array('tabela' => 'emendas.autor', 'pk' => 'autid'),
        'acaid' => array('tabela' => 'acao', 'pk' => 'acaid'),
        'unoid' => array('tabela' => 'unidadeorcamentaria', 'pk' => 'unoid'),
    );

    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributos = array(
        'emeid' => null,
        'emenumero' => null,
        'unoid' => null,
        'acaid' => null,
        'emejustificativa' => null,
        'emeimpositiva' => null,
        'autid' => null,
        'prsano' => null,
        'emestatus' => null,
    );

    public function recuperarListagem($exercicio)
    {
        $sql = "select  eme.emeid, eme.emenumero, eme.unoid, eme.acaid, eme.emeimpositiva, eme.autid, eme.prsano, uno.unocod || ' - ' || uno.unonome unonome, 
                        aut.autnome, aut.autcod, aca.acacod, aca.prgcod || '.' || aca.acacod || '.' || aca.acaobjetivocod || '.' || aca.loccod || ' - ' || aca.acatitulo funcional
                from emendas.emenda eme
                    inner join public.unidadeorcamentaria uno on uno.unoid = eme.unoid
                    inner join emendas.autor aut on aut.autid  = eme.autid
                    inner join monitora.acao aca on aca.acaid = eme.acaid
                where eme.emestatus = 'A'        
                and eme.prsano = '$exercicio'
                order by aut.autnome";

        $dados = $this->carregar($sql);
        return $dados ? $dados : [];
    }

}//end Class
?>