<?php
/**
 * Classe de mapeamento da entidade proposta.proposta
 *
 * @category Class
 * @package  A1
 * @author   ORION TELES DE MESQUITA <teste@teste.com.br>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 13-04-2018
 * @link     no link
 */


require_once APPRAIZ .'includes/classes/Modelo.class.inc';


/**
 * Proposta_Model_Proposta
 *
 * @category Class
 * @package  A1
 * @author    <>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 
 * @link     no link
 */
class Proposta_Model_Proposta extends Modelo
{
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = 'proposta.proposta';

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array(
        'proid',
    );
    /**
     * Chaves estrangeiras.
     * @var array
     */
    protected $arChaveEstrangeira = array(
        'ptrid' => array('tabela' => 'ptres', 'pk' => 'ptrid'),
        'eqdid' => array('tabela' => 'pi_enquadramentodespesa', 'pk' => 'eqdid'),
        'suoid' => array('tabela' => 'subunidadeorcamentaria', 'pk' => 'suoid'),
    );

    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributos = array(
        'proid' => null,
        'suoid' => null,
        'eqdid' => null,
        'ptrid' => null,
        'proquantidade' => null,
        'proquantidadeexpansao' => null,
        'projustificativa' => null,
        'projustificativaexpansao' => null,
        'prostatus' => null,
        'prsano' => null,
    );


    public function recuperarListagem()
    {
        $sql = "select  ptr.funcional, ptr.plodsc, eqd.eqddsc, suo.unosigla || ' - ' || suo.suonome subunidade,  pro.proquantidade, pro.proquantidadeexpansao, 
                        sum(prd.prdvalor) prdvalor, sum(prd.prdvalorexpansao) prdvalorexpansao
                from proposta.proposta pro
                    inner join monitora.vw_ptres ptr on pro.ptrid = ptr.ptrid
                    inner join public.vw_subunidadeorcamentaria suo on suo.suoid = pro.suoid
                    inner join monitora.pi_enquadramentodespesa eqd on eqd.eqdid = pro.eqdid
                    left join proposta.propostadetalhe prd on prd.proid = pro.proid
                where pro.prsano = '{$_SESSION['exercicio']}'
                and prostatus = 'A'
                group by ptr.funcional, ptr.plodsc, eqd.eqddsc, subunidade,  pro.proquantidade, pro.proquantidadeexpansao";

        return $this->carregar($sql);
    }
}//end Class
?>