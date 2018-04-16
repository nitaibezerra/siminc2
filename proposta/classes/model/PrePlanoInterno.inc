<?php
/**
 * Classe de mapeamento da entidade proposta.prePlanoInterno
 *
 * @category Class
 * @package  A1
 * @author   ORION TELES DE MESQUITA <teste@teste.com.br>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 09-04-2018
 * @link     no link
 */


require_once APPRAIZ .'includes/classes/Modelo.class.inc';


/**
 * Proposta_Model_PrePlanoInterno
 *
 * @category Class
 * @package  A1
 * @author    <>
 * @license  GNU simec.mec.gov.br
 * @version  Release:
 * @link     no link
 */
class Proposta_Model_PrePlanoInterno extends Modelo
{
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = 'proposta.preplanointerno';

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array(
        'pliid',
    );
    /**
     * Chaves estrangeiras.
     * @var array
     */
    protected $arChaveEstrangeira = array(
        'docid' => array('tabela' => 'workflow.documento', 'pk' => 'docid'),
        'neeid' => array('tabela' => 'pi_niveletapaensino', 'pk' => 'neeid'),
        'mdeid' => array('tabela' => 'pi_modalidadeensino', 'pk' => 'mdeid'),
        'pumid' => array('tabela' => 'pi_unidade_medida', 'pk' => 'pumid'),
        'pprid' => array('tabela' => 'pi_produto', 'pk' => 'pprid'),
        'ipnid' => array('tabela' => 'indicadorpnc', 'pk' => 'ipnid'),
        'mpnid' => array('tabela' => 'metapnc', 'pk' => 'mpnid'),
        'ippid' => array('tabela' => 'iniciativappa', 'pk' => 'ippid'),
        'mppid' => array('tabela' => 'metappa', 'pk' => 'mppid'),
        'oppid' => array('tabela' => 'objetivoppa', 'pk' => 'oppid'),
        'ptrid' => array('tabela' => 'ptres', 'pk' => 'ptrid'),
        'masid' => array('tabela' => 'planacomorc.manutencaosubitem', 'pk' => 'masid'),
        'maiid' => array('tabela' => 'planacomorc.manutencaoitem', 'pk' => 'maiid'),
        'eqdid' => array('tabela' => 'pi_enquadramentodespesa', 'pk' => 'eqdid'),
        'suoid' => array('tabela' => 'subunidadeorcamentaria', 'pk' => 'suoid'),
    );

    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributos = array(
        'pliid' => null,
        'plititulo' => null,
        'plidsc' => null,
        'suoid' => null,
        'eqdid' => null,
        'maiid' => null,
        'masid' => null,
        'ptrid' => null,
        'oppid' => null,
        'mppid' => null,
        'ippid' => null,
        'mpnid' => null,
        'ipnid' => null,
        'pprid' => null,
        'pumid' => null,
        'pliquantidade' => null,
        'mdeid' => null,
        'neeid' => null,
        'plivalorcusteio' => null,
        'plivalorcapital' => null,
        'docid' => null,
        'prsano' => null,
        'plistatus' => null,
        'plivalorcusteioadicional' => null,
        'plivalorcapitaladicional' => null,
        'pliquantidadeadicional' => null,
        'plijustificativaadicional' => null,
    );

    public function recuperarListagem()
    {
        $sql = "select ptr.funcional, ptr.plodsc, eqd.eqddsc, suo.unosigla || ' - ' || suo.suonome subunidade,  pli.* 
                from proposta.preplanointerno pli
                    inner join monitora.vw_ptres ptr on pli.ptrid = ptr.ptrid
                    inner join public.vw_subunidadeorcamentaria suo on suo.suoid = pli.suoid
                    inner join monitora.pi_enquadramentodespesa eqd on eqd.eqdid = pli.eqdid
                where pli.prsano = '{$_SESSION['exercicio']}'
                and plistatus = 'A'";

        return $this->carregar($sql);
    }

}//end Class
?>