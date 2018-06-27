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
    public function getMetasPnc(stdClass $filtro){
        include_once APPRAIZ. 'public/classes/Model/SubUnidadeOrcamentaria.inc';
        $listaMetas = array();
        $subunidade = new Public_Model_SubUnidadeOrcamentaria($filtro->suoid);
        $uo = new Public_Model_UnidadeOrcamentaria($subunidade->unoid);
        $sql = "
            SELECT
                m.mpncod,
                m.mpnid,
                m.mpndsc,
                sm.suoid,
                m.prsano,
                (
                    SELECT
                        count(0) as qtd
                    FROM acompanhamento.acompanhamento_pnc a1
                    WHERE
                        a1.acpstatus = 'A'
                        and a1.mpnid = m.mpnid
                        and a1.janid = ". (int)$filtro->janid. "
                        and a1.suoid = '". (int)$filtro->suoid. "') as acompanhamento,
                (
                    SELECT
                        count(0) as qtd
                    FROM planacomorc.pi_complemento c
                        JOIN monitora.pi_planointerno p on c.pliid = p.pliid
                        JOIN public.vw_subunidadeorcamentaria su on p.ungcod = su.suocod
                    WHERE
                        c.mpnid = m.mpnid
                        and su.suoid = '". (int)$filtro->suoid. "'
                ) as pis
            FROM public.metapnc m
                JOIN spo.subunidademetapnc sm ON m.mpnid = sm.mpnid
            WHERE
                m.mpnstatus = 'A'
                AND m.prsano = '". (int)$filtro->exercicio. "'
                AND sm.suoid = '". (int)$filtro->suoid. "'
            ORDER BY
                m.mpncod::int
        ";
        $listaMetas = $this->carregar($sql);

        return $listaMetas? $listaMetas: array();
    }
    
    public function getIndicadoresPnc(stdClass $filtro)
    {
        $listaIndicadoresPnc = array();
        if($filtro->suoid){
            $sql = "
                SELECT
                    ipn.*,
                    acp.acpid,
                    sic.suoid,
                    idp.idpquantidade,
                    mpn.mpncod::int,
                    mpn.mpndsc
                FROM public.indicadorpnc ipn
                    JOIN public.metapnc mpn ON mpn.mpnid = ipn.mpnid
                    JOIN spo.subunidadeindicadorpnc sic ON sic.ipnid = ipn.ipnid
                    JOIN public.vw_subunidadeorcamentaria suo ON sic.suoid = suo.suoid
                    LEFT JOIN acompanhamento.acompanhamento_pnc acp ON(
			suo.suoid = acp.suoid
                        AND acp.acpstatus = 'A'
                    )
                    LEFT JOIN acompanhamento.indicador_pnc idp ON(
			acp.acpid = idp.acpid
			AND ipn.ipncod = idp.ipncod
			AND ipn.prsano = idp.prsano
			AND idp.idpstatus = 'A'
                    )
                WHERE
                    ipn.prsano = '". (int)$filtro->exercicio. "'
                    AND ipn.ipnstatus = 'A'
                    AND sic.suoid = ". (int)$filtro->suoid. "
                    AND mpn.mpnid = ". (int)$filtro->mpnid. "
                UNION
                SELECT
                    ipn.*,
                    acp.acpid,
                    ". (int)$filtro->suoid. ",
                    idp.idpquantidade,
                    mpn.mpncod::int,
                    mpn.mpndsc
                FROM public.indicadorpnc ipn
                    JOIN public.metapnc mpn ON mpn.mpnid = ipn.mpnid
                    LEFT JOIN spo.subunidadeindicadorpnc sic ON sic.ipnid = ipn.ipnid
                    LEFT JOIN public.vw_subunidadeorcamentaria suo ON sic.suoid = suo.suoid
                    LEFT JOIN acompanhamento.acompanhamento_pnc acp ON(
			suo.suoid = ". (int)$filtro->suoid. "
                        AND acp.acpstatus = 'A'
                    )
                    LEFT JOIN acompanhamento.indicador_pnc idp ON(
			acp.acpid = idp.acpid
			AND ipn.ipncod = idp.ipncod
			AND ipn.prsano = idp.prsano
			AND idp.idpstatus = 'A'
                    )
                WHERE
                    ipn.prsano = '". (int)$filtro->exercicio. "'
                    AND ipn.ipnstatus = 'A'
                    AND sic.suoid IS NULL
                    AND ipn.ipncod != '0'
                    AND mpn.mpnid = ". (int)$filtro->mpnid. "
                ORDER BY
                    mpncod,
                    ipncod,
                    ipndsc
            ";
//ver($sql, d);
            $listaIndicadoresPnc = $this->carregar($sql);
        }

        return $listaIndicadoresPnc;
    }
    
    /**
     * Monta consulta pra buscar acompanhamento por Janela, UO, Unidade, Indicador e Ano.
     * 
     * @param stdClass $filtro
     * @return string
     */
    public function montarSqlBuscarIndicadorPnc(stdClass $filtro){
        $sql = "
            SELECT
                idp.idpid,
                acp.janid,
                idp.unocod,
                idp.suocod,
                idp.ipncod,
                idp.prsano,
                idp.idpquantidade
            FROM acompanhamento.indicador_pnc idp
                JOIN acompanhamento.acompanhamento_pnc acp ON(idp.acpid = acp.acpid)
            WHERE
                idp.idpstatus = 'A'
                AND acp.acpstatus = 'A'
                AND acp.janid = ". (int)$filtro->janid. "
                AND idp.unocod = '". (int)$filtro->unocod. "'
                AND idp.suocod = '". (int)$filtro->suocod. "'
                AND idp.ipncod = '". $filtro->ipncod. "'
                AND idp.prsano = '". (int)$filtro->prsano. "'
        ";
        
        return $sql;
    }
    
    /**
     * Buscar acompanhamento por Janela, UO, Unidade, Indicador e Ano.
     * 
     * @param stdClass $filtro
     * @return stdClass
     */
    public function buscarIndicadorPnc(stdClass $filtro) {
        $sql = $this->montarSqlBuscarIndicadorPnc($filtro);
        $acompanhamento = $this->pegaLinha($sql);
        
        return $acompanhamento? (object)$acompanhamento: new stdClass();
    }

}
