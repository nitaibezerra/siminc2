<?php

/**
 * Classe de mapeamento da entidade acompanhamento.acompanhamento_pnc
 */

require_once APPRAIZ .'includes/classes/Modelo.class.inc';


/**
 * Acompanhamento_Model_Acompanhamentopnc
 *
 * @category Class
 * @package  A1
 * @author    <>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 
 * @link     no link
 */
class Acompanhamento_Model_Acompanhamentopnc extends Modelo
{
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = 'acompanhamento.acompanhamento_pnc';

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array(
        'acpid',
    );

    /**
     * Chaves estrangeiras.
     * @var array
     */
    protected $arChaveEstrangeira = array(
        'suoid' => array('tabela' => 'public.subunidadeorcamentaria', 'pk' => 'suoid'),
        'janid' => array('tabela' => 'acompanhamento.janela', 'pk' => 'janid'),
        'mpnid' => array('tabela' => 'public.metapnc', 'pk' => 'mpnid')
    );

    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributos = array(
        'acpid' => null,
        'suoid' => null,
        'janid' => null,
        'mpnid' => null,
        'acpenviado' => null,
        'docid' => null,
        'acpdata' => null,
        'acpstatus' => null,
    );

    /**
     * Busca Metas PNC
     * 
     * @param stdClass $filtro
     * @return array
     */
    public function getMetasPncComPiAntiga(stdClass $filtro){
        include_once APPRAIZ. 'public/classes/Model/SubUnidadeOrcamentaria.inc';
        $listaMetas = array();
        $subunidade = new Public_Model_SubUnidadeOrcamentaria($filtro->suoid);
        $uo = new Public_Model_UnidadeOrcamentaria($subunidade->unoid);
        $sql = "
            SELECT DISTINCT
                ac.prsano,
                ac.suoid,
                mpn.mpncod::int,
                mpn.mpndsc,
                mpn.mpnid,
                count(pic.pliid) pis,
                count(ac.acoid) acompanhamento
            FROM (
                SELECT
                    ipn.ipncod,
                    ipn.mpnid,
                    ipn.ipndsc,
                    ipn.ipnstatus,
                    ipn.prsano,
                    ipn.ipncod,
                    aco.acoid,
                    sic.suoid,
                    suo.unocod,
                    suo.suocod,
                    aco.acoquantidade
                FROM public.indicadorpnc ipn
                    JOIN spo.subunidadeindicadorpnc sic ON sic.ipnid = ipn.ipnid
                    JOIN public.vw_subunidadeorcamentaria suo ON sic.suoid = suo.suoid
                    LEFT JOIN acompanhamento.acompanhamento aco ON(
                        ipn.ipncod = aco.ipncod
                        AND suo.unocod = aco.unocod
                        AND suo.suocod = aco.suocod
                        AND acostatus = 'A'
                        AND aco.janid = ". (int)$filtro->janid. "
                    )
                WHERE
                    ipn.prsano = '". (int)$filtro->exercicio. "'
                    AND ipn.ipnstatus = 'A'
                    AND suo.suoid = ". (int)$filtro->suoid. "
                UNION
                SELECT
                    ipn.ipncod,
                    ipn.mpnid,
                    ipn.ipndsc,
                    ipn.ipnstatus,
                    ipn.prsano,
                    ipn.ipncod,
                    aco.acoid,
                    sic.suoid,
                    '". (int)$uo->unocod. "',
                    '". (int)$subunidade->suocod. "',
                    aco.acoquantidade
                FROM public.indicadorpnc ipn
                    LEFT JOIN spo.subunidadeindicadorpnc sic ON sic.ipnid = ipn.ipnid
                    LEFT JOIN public.vw_subunidadeorcamentaria suo ON sic.suoid = suo.suoid
                    LEFT JOIN acompanhamento.acompanhamento aco ON(
                        ipn.ipncod = aco.ipncod
                        AND sic.suoid = ". (int)$filtro->suoid. "
                        AND acostatus = 'A'
                        AND aco.janid = ". (int)$filtro->janid. "
                    )
                WHERE
                    ipn.prsano = '". (int)$filtro->exercicio. "'
                    AND ipn.ipnstatus = 'A'
                    AND suo.suoid IS NULL
                    AND ipn.ipncod != '0'
            ) ac
            JOIN public.metapnc mpn ON mpn.mpnid = ac.mpnid
            LEFT JOIN public.vw_subunidadeorcamentaria suo ON suo.suoid = ac.suoid
            LEFT JOIN monitora.pi_planointerno pli ON(
                pli.ungcod = suo.suocod
                AND pli.unicod = suo.unocod
            )
            LEFT JOIN planacomorc.pi_complemento pic ON(
                pli.pliid = pic.pliid
                AND ac.mpnid = pic.mpnid
            )
            GROUP BY
                ac.prsano,
                ac.suoid,
                mpn.mpncod,
                mpn.mpndsc,
                mpn.mpnid
            ORDER BY
                mpncod
        ";
//ver($sql, d);
        $listaMetas = $this->carregar($sql);

        return $listaMetas? $listaMetas: array();
    }

}
