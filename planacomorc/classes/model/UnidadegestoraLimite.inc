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

    public function recuperarLimitePorSuoid($suoid)
    {
        $sql = "
            SELECT
                suo.suoid,
                suo.unoid,
                suo.suosigla,
                suo.unosigla,
                suo.unocod,
                suo.suonome,
                suo.unonome,
                suo.suocod,
                suo.unofundo,
                lmu.*,
                coalesce(vlr.totalunidade, 0) totalunidade,
                coalesce(lmu.lmuvlr, 0) - coalesce(vlr.totalunidade, 0) disponivelunidade
            FROM public.vw_subunidadeorcamentaria suo
                JOIN planacomorc.unidadegestora_limite lmu ON(
                    suo.suocod = lmu.ungcod
                    AND suo.prsano = lmu.prsano
                )
                LEFT JOIN (
                    SELECT
                        sum(plivalorcapital + plivalorcusteio) totalunidade,
                        suoid
                    FROM proposta.preplanointerno
                    WHERE
                        suoid = '". (int)$suoid. "'
                    GROUP BY
                        suoid
                ) vlr ON vlr.suoid = suo.suoid
            WHERE
                suo.unostatus = 'A'
                AND suo.prsano = '{$_SESSION['exercicio']}'
                AND suo.suoid = '". (int)$suoid. "'
        ";
        $limite = $this->pegaLinha($sql);
        return $limite;
    }

}//end Class
?>