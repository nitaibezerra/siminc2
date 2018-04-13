<?php
/**
 * Classe de mapeamento da entidade planacomorc.unidadegestora_limite
 *
 * @category Class
 * @package  A1
 * @author   ORION TELES DE MESQUITA <orion.mesquita@cultura.gov.br>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 29-09-2017
 * @link     no link
 */


require_once APPRAIZ .'includes/classes/Modelo.class.inc';


/**
 * Planacomorc_Model_Unidadegestora_limite
 *
 * @category Class
 * @package  A1
 * @author    <>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 
 * @link     no link
 */
class Planacomorc_Model_UnidadegestoraLimite extends Modelo
{
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = 'planacomorc.unidadegestora_limite';

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array(
        'lmuid',
    );
    /**
     * Chaves estrangeiras.
     * @var array
     */
    protected $arChaveEstrangeira = array(
    );

    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributos = array(
        'lmuid' => null,
        'lmuvlr' => null,
        'lmudtcadastro' => null,
        'usucpf' => null,
        'lmustatus' => null,
        'lmuflgliberado' => null,
        'prsano' => null,
        'ungcod' => null,
        'unoid' => null,
    );

    public function recuperarLimites($exercicio)
    {
        $sql = "select suo.suoid, suo.unoid, suo.suosigla, suo.unosigla, suo.unocod, suo.suonome, suo.unonome, suo.suocod, suo.unofundo, lmu.*
                from public.vw_subunidadeorcamentaria suo 
                        left join planacomorc.unidadegestora_limite lmu on suo.suocod = lmu.ungcod and suo.prsano = lmu.prsano
                where suo.unostatus = 'A'
                and suo.prsano = '$exercicio'
                and suo.unofundo = false
                order by suo.unofundo, suo.unonome, suo.suonome;";
        
        $dados = $this->carregar($sql);
        return $dados ? $dados : [];
    }

    public function recuperarLimitePorSuoid($suoid, $pliid = 0)
    {
        $sql = "select suo.suoid, suo.unoid, suo.suosigla, suo.unosigla, suo.unocod, suo.suonome, suo.unonome, suo.suocod, suo.unofundo, 
                       lmu.*, coalesce(vlr.valorcapital, 0) valorcapital, coalesce(vlr.valorcusteio, 0) valorcusteio,
                       coalesce(vlr.valorcapital, 0) + coalesce(vlr.valorcusteio, 0) totalunidade
                from public.vw_subunidadeorcamentaria suo 
                        inner join planacomorc.unidadegestora_limite lmu on suo.suocod = lmu.ungcod and suo.prsano = lmu.prsano
                        left join (
                                select sum(plivalorcapital) valorcapital, sum(plivalorcusteio) valorcusteio, suoid
                                from proposta.preplanointerno
                                where pliid != " . (int) $pliid . "
                                and suoid = $suoid
                                group by suoid
                        ) vlr on vlr.suoid = suo.suoid                        
                where suo.unostatus = 'A'
                and suo.prsano = '{$_SESSION['exercicio']}'
                and suo.suoid = " . (int) $suoid;

        return $this->pegaLinha($sql);
    }

}//end Class
?>