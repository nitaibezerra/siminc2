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
        'esfid' => null,
        'prsano' => null,
        'plistatus' => null,
        'plivalorcusteioadicional' => null,
        'plivalorcapitaladicional' => null,
        'pliquantidadeadicional' => null,
        'plijustificativaadicional' => null,
    );

    public function recuperarListagem()
    {
        $sql = "select ptr.funcional, ptr.acatitulo, ptr.plodsc, eqd.eqddsc, suo.unosigla || ' - ' || suo.suonome subunidade,  pli.*, esd.esddsc
                from proposta.preplanointerno pli
                    inner join monitora.vw_ptres ptr on pli.ptrid = ptr.ptrid
                    inner join public.vw_subunidadeorcamentaria suo on suo.suoid = pli.suoid
                    inner join monitora.pi_enquadramentodespesa eqd on eqd.eqdid = pli.eqdid
                    left join workflow.documento doc on doc.docid = pli.docid
                    left join workflow.estadodocumento esd on esd.esdid = doc.esdid
                where pli.prsano = '{$_SESSION['exercicio']}'
                and plistatus = 'A'";

        return $this->carregar($sql);
    }

    public function recuperarLocalizacao(){
        $sql = "select * 
                from  proposta.preplanointernolocalizacao
                where pliid = " . (int) $this->pliid;

        $dados = $this->carregar($sql);

        $aLocalizacoes = ['paiid'=>[], 'estuf'=>[], 'muncod'=>[]];
        foreach($dados as $dado){
            if($dado['paiid']){ $aLocalizacoes['paiid'][] = $dado['paiid']; }
            if($dado['estuf']){ $aLocalizacoes['estuf'][] = $dado['estuf']; }
            if($dado['muncod']){ $aLocalizacoes['muncod'][] = $dado['muncod']; }
        }

        return $aLocalizacoes;
    }
    
    public function recuperarLocalizacaoPi($pliid){
        $sql = "
            SELECT
                *
            FROM planacomorc.pi_localizacao
            WHERE
                pliid = " . (int)$pliid;

        $dados = $this->carregar($sql);

        $listaLocalizacoes = ['paiid'=>[], 'estuf'=>[], 'muncod'=>[]];
        foreach($dados as $dado){
            if($dado['paiid']){ $listaLocalizacoes['paiid'][] = $dado['paiid']; }
            if($dado['estuf']){ $listaLocalizacoes['estuf'][] = $dado['estuf']; }
            if($dado['muncod']){ $listaLocalizacoes['muncod'][] = $dado['muncod']; }
        }
//ver($sql, $listaLocalizacoes, d);
        return $listaLocalizacoes;
    }

    public function recuperarValoresPorSubnidadeEPtres($suoid, $ptrid)
    {
        $sql = "SELECT
                    sum(pli.plivalorcapital) plivalorcapital,
                    sum(pli.plivalorcusteio) plivalorcusteio,
                    sum(pli.plivalorcapitaladicional) plivalorcapitaladicional,
                    sum(pli.plivalorcusteioadicional) plivalorcusteioadicional
                FROM
                    proposta.preplanointerno pli
                WHERE
                    pli.suoid = " . (int) $suoid . "
                    AND pli.ptrid = " . (int) $ptrid . " 
                    AND pli.plistatus = 'A'";

        return $this->pegaLinha($sql);
    }

    function carregarGrafico($filtro = null){

        # Cores dos Graficos
        $colors = "'#FFD700', '#FF0000', '#55BF3B', '#eeaaee', '#00BFFF', '#aaeeee', '#7798BF', '#DDDF0D', '#7CCD7C', '#DF5353', '#008000', '#CD0000', '#FF4500', '#ff0066', '#4B0082', '#808000', '#800000', '#2F4F4F', '#006400', '#FFA500'";

        echo '<div class="panel-body">';
        $estatistica = $this->recuperarEstatisticaPagamento($filtro);
        $grafico = new Grafico(Grafico::K_TIPO_COLUNA, false);
        $grafico
            ->setFormatoTooltip(Grafico::K_TOOLTIP_DECIMAL_0)
            ->setColors($colors)
            ->setEvent(array('click' => "exibirModalDetalheGrafico(0, event.point.series.name, event.point.category);"))
            ->gerarGrafico($estatistica);
        echo '</div>';
    }

    /**
     * Busca Unidades com estatistica de orçamento e valores de pagamento.
     *
     * @param stdClass $filtro
     * @return array
     */
    public function recuperarEstatisticaPagamento($aFiltro = null)
    {
        
        $filtro = '';
        $filtro .= $aFiltro['unosigla'] ? " AND suo.unosigla = '{$aFiltro['unosigla']}' ": NULL;
        $filtro .= $aFiltro['unidades'] ? " AND {$aFiltro['unidades']} ": NULL;

        $descricao .= $aFiltro['unosigla'] ? "suo.suosigla": "suo.unosigla";

        $sql = "
            SELECT DISTINCT
                1 AS ordem,
                'Dotação' AS descricao,
                $descricao AS categoria,
                (sum(COALESCE (psu.ptrdotacaocusteio, 0)::DECIMAL) + sum(COALESCE (psu.ptrdotacaocapital, 0)::DECIMAL)) AS valor
            FROM
                public.vw_subunidadeorcamentaria suo
                LEFT JOIN spo.ptressubunidade psu ON psu.suoid = suo.suoid
                LEFT JOIN monitora.vw_ptres ptr ON ptr.ptrid = psu.ptrid
                AND ptr.ptrano = suo.prsano
            WHERE
                suo.prsano = '{$_SESSION['exercicio']}'
                $filtro
                AND suo.suostatus = 'A'
                AND ptr.irpcod <> '6'
            GROUP BY
                ordem,
                descricao,
                categoria
            UNION ALL
            SELECT DISTINCT
                2 AS ordem,
                'Limite' AS descricao,
                $descricao AS categoria,
                sum(COALESCE (lmu.lmuvlr, 0)::DECIMAL) AS valor
            FROM
                public.vw_subunidadeorcamentaria suo
                LEFT JOIN planacomorc.unidadegestora_limite lmu ON suo.suocod = lmu.ungcod
                AND suo.prsano = lmu.prsano
            WHERE
                suo.prsano = '{$_SESSION['exercicio']}'
                $filtro
                and suo.suostatus = 'A'
            GROUP BY
                ordem,
                descricao,
                categoria
            UNION ALL
            SELECT DISTINCT
                3 AS ordem,
                'Planejado' AS descricao,
                $descricao AS categoria,
                sum(COALESCE (pli.plivalorcapital, 0) + COALESCE (pli.plivalorcusteio, 0))::DECIMAL AS valor
            FROM
                public.vw_subunidadeorcamentaria suo
                LEFT JOIN proposta.preplanointerno pli ON suo.suoid = pli.suoid AND pli.plistatus = 'A'
            WHERE
                suo.prsano = '{$_SESSION['exercicio']}'
                $filtro
                AND suo.suostatus = 'A'    
            GROUP BY
                ordem,
                descricao,
                categoria
            ORDER BY
                ordem,
                categoria
        ";

        $dados = $this->carregar($sql);
        return $dados ? $dados : [];
    }
    
    private function montarSQlImportarPi($pliid = NULL) {
        $sql = "
            SELECT DISTINCT
                pli.pliid,
                pli.mdeid,
                mde.mdecod,
                pli.eqdid,
                ptr.ptrid,
                eqd.eqdcod,
                pli.neeid,
                nee.neecod,
                pli.capid,
                cap.capcod,
                pli.sbaid, pli.plititulo, pli.plicodsubacao, pli.plicod, pli.plilivre, pli.plidsc, pli.unicod, pli.ungcod, pli.pliano, pli.plicadsiafi, pli.docid,
                to_char(pli.plidata, 'dd/mm/YYYY') as plidata,
                pc.picid, pc.pliid, pc.oppid, pc.mppid, pc.ippid, pc.mpnid, pc.ipnid, pc.pprid, pc.pumid, pc.picquantidade, pc.picted, pc.picedital, pc.esfid, pc.picvalorcusteio, pc.picvalorcapital, pc.mescod, pc.acuid, pc.secid, pc.maiid, pc.masid, pc.picexecucao, pc.picpublico, pc.picpriorizacao, pc.pijid, pc.ptaid,
                CASE plisituacao WHEN 'A' THEN
                    'Aprovado'
                WHEN 'E' THEN
                    'Enviado para revisao'
                WHEN 'P' THEN
                    'Pendente'
                WHEN 'C' THEN
                    'Cadastrado no SIAFI'
                WHEN 'R' THEN
                    'Revisado'
                WHEN 'H' THEN
                    'Homologado'
                WHEN 'T' THEN
                    '<span style=\"color:red\">Cadastrado no SIAFI</span>'
                ELSE 'Tendencioso'
                END AS plisituacao,
                sba.sbaid, sba.sbasigla || ' - ' AS sbasigla, sba.sbacod, ben.benid, em.emenumero
            FROM monitora.pi_planointerno pli
                LEFT JOIN monitora.pi_planointernoptres ptr ON pli.pliid = ptr.pliid
                LEFT JOIN emendas.beneficiario ben ON(pli.pliid = ben.pliid)
                LEFT JOIN emendas.emenda em ON(ben.emeid = em.emeid)
                LEFT JOIN planacomorc.pi_complemento pc on pc.pliid = pli.pliid
                LEFT JOIN monitora.pi_subacao sba ON (pli.sbaid = sba.sbaid AND pli.pliano = sba.sbaano)
                LEFT JOIN monitora.pi_enquadramentodespesa eqd ON (pli.eqdid = eqd.eqdid AND pli.pliano = eqd.eqdano)
                LEFT JOIN monitora.pi_niveletapaensino nee ON (pli.neeid = nee.neeid AND pli.pliano = nee.neeano)
                LEFT JOIN monitora.pi_categoriaapropriacao cap ON (pli.capid = cap.capid AND pli.pliano = cap.capano)
                LEFT JOIN monitora.pi_modalidadeensino mde ON (pli.mdeid = mde.mdeid) --ON (pli.mdeid = mde.mdeid AND pli.pliano = mde.mdeano)
            WHERE
                pli.pliid = $pliid
        ";
//ver($sql, d);
        return $sql;
    }

    /**
     * Importa plano interno baseado do ano anterior ao de exercício
     *
     * @param $pliid
     */
    public function importarPI($pliid = NULL) {
        $sql = $this->montarSQlImportarPi($pliid);
        $pi = $this->pegaLinha($sql);

        $this->plititulo     = $pi['plititulo'];
        $this->plidsc        = $pi['plidsc'];
        $this->suoid         = $this->getSubUnidadeIDAnoAnterior($pi['unicod'], $pi['ungcod']);
        $this->eqdid         = $this->getEnquadramentoIDAnoAnterior($pi['eqdid']);
        $this->ptrid         = $this->getFuncionalIDAnoAnterior($pi['ptrid']);
        $this->pprid         = $this->getProdutoIDAnoAnterior($pi['pprid']);
        $this->pumid         = $this->getUnidadeMedidaIDAnoAnterior($pi['pumid']);
        $this->pliquantidade = $pi['picquantidade'];
        $this->oppid         = $this->getObjPPAIDAnoAnterior($pi['oppid']);
        $this->mppid         = $this->getMetasPPAIDAnoAnterior($pi['mppid']);
        $this->ippid         = $this->getIniciativaPPAIDAnoAnterior($this->oppid, $pi['ippid']);
        $this->mpnid         = $this->getMetaPNCIDAnoAnterior($pi['mpnid']);
        $this->ipnid         = $this->getIndicadorPNCIDAnoAnterior($pi['ipnid']);
        $this->mdeid         = $this->getAreaCulturalIDAnoAnterior($pi['mdeid']);
        $this->neeid         = $this->getSegmentoCulturalIDAnoAnterior($pi['neeid']);
        $this->esfid         = $pi['esfid'];
        $this->plivalorcusteio = $pi['picvalorcusteio'];
        $this->plivalorcapital = $pi['picvalorcapital'];
        $this->pliid = NULL;
    }

    /**
     * Retorna a ID do produto PI do ano atual, baseado no ano anterior
     *
     * @param $pprid
     * @return array
     */
    private function getProdutoIDAnoAnterior($pprid = NULL) {
        $anoAnterior = $_SESSION['exercicio'] - 1;
        $sql = "
            SELECT 
                pprid
            FROM 
                monitora.pi_produto
            WHERE 
                prsano = '{$_SESSION['exercicio']}'
                AND pprstatus = 'A'
                AND pprnome = (
                    SELECT 
                        pprnome
                    FROM  monitora.pi_produto
                    WHERE 
                        prsano = '{$anoAnterior}'
                        AND pprstatus = 'A'
                        AND pprid = '". (int)$pprid. "'
              )
        ";

        return $this->pegaUm($sql);
    }

    /**
     * Retorna a ID da Unidade de Medida do ano atual, baseado no ano anterior
     *
     * @param $pumid
     * @return array
     */
    private function getUnidadeMedidaIDAnoAnterior($pumid = NULL){
        $anoAnterior = $_SESSION['exercicio'] - 1;
        $pumid = $pumid ? $pumid : 0;
        $sql = "
            SELECT
                pumid
            FROM monitora.pi_unidade_medida
            WHERE
                prsano = '{$_SESSION['exercicio']}'
                AND pumstatus = 'A'
                AND pumdescricao = (
                    SELECT
                        pumdescricao
                    FROM monitora.pi_unidade_medida
                    WHERE
                        prsano = '{$anoAnterior}'
                        AND pumstatus = 'A'
                        AND pumid = '". (int)$pumid. "'
                )
        ";

        return $this->pegaUm($sql);
    }

    /**
     * Retorna a ID da Subunidade do ano atual, baseado no ano anterior
     *
     * @param $unocod
     * @param $suocod
     * @return array
     */
    private function getSubUnidadeIDAnoAnterior($unocod = NULL, $suocod = NULL){
        $sql = "
            SELECT DISTINCT
                suo.suoid
            FROM public.vw_subunidadeorcamentaria suo
            WHERE
                suo.suostatus = 'A'
                AND suo.prsano = '{$_SESSION['exercicio']}'
                AND suo.unocod = '{$unocod}'
                AND suo.suocod = '{$suocod}'
                AND suo.unofundo IS FALSE
        ";

        return $this->pegaUm($sql);
    }

    /**
     * Retorna a ID do campo Enquadramento da Despesa do ano atual, baseado no ano anterior
     *
     * @param $eqdid
     * @return bool|mixed|NULL|string
     */
    private function getEnquadramentoIDAnoAnterior($eqdid = NULL) {
        $anoAnterior = $_SESSION['exercicio'] - 1;
        $sql = "
            SELECT
                eqdid,
                eqddsc
            FROM monitora.pi_enquadramentodespesa
            WHERE
                eqdstatus = 'A'
                AND eqdano = '{$_SESSION['exercicio']}'
                AND eqddsc = (
                    SELECT
                        eqddsc
                    FROM monitora.pi_enquadramentodespesa
                    WHERE
                        eqdstatus = 'A'
                        AND eqdano = '{$anoAnterior}'
                        AND eqdid = '". (int)$eqdid. "'
                )
        ";

        return $this->pegaUm($sql);
    }
    
    /**
     * Retorna a ID da Funcional do ano atual, baseado no ano anterior
     *
     * @param $ptrid
     * @return bool|mixed|NULL|string
     */
    private function getFuncionalIDAnoAnterior($ptrid = NULL) {
        $anoAnterior = $_SESSION['exercicio'] - 1;
        $sql = "
            SELECT DISTINCT
                ptr.ptrid
            FROM monitora.ptres ptr
                JOIN monitora.acao aca ON ptr.acaid = aca.acaid
                JOIN (
                    SELECT DISTINCT
                        ptranterior.ptrid,
                        ptranterior.unicod,
                        ptranterior.prgcod,
                        ptranterior.acacod,
                        ptranterior.loccod,
                        ptranterior.plocod
                    FROM monitora.ptres ptranterior
                        JOIN monitora.acao acaanterior ON ptranterior.acaid = acaanterior.acaid
                    WHERE
                        ptranterior.ptrano = '{$anoAnterior}'
                        AND ptranterior.ptrstatus = 'A'
                ) funcionalanterior ON(
                        ptr.unicod = funcionalanterior.unicod
                        AND ptr.prgcod = funcionalanterior.prgcod
                        AND aca.acacod = funcionalanterior.acacod
                        AND ptr.loccod = funcionalanterior.loccod
                        AND ptr.plocod = funcionalanterior.plocod
                )
            WHERE
                ptr.ptrstatus = 'A'
                AND ptr.ptrano = '{$_SESSION['exercicio']}'
                AND funcionalanterior.ptrid = '". (int)$ptrid. "'
        ";

        return $this->pegaUm($sql);
    }

    /**
     * Retorna a ID do campo Objetivo PPA do ano atual, baseado no ano anterior
     *
     * @param $oppid
     * @return bool|mixed|NULL|string
     */
    private function getObjPPAIDAnoAnterior($oppid = NULL) {
        $anoAnterior = $_SESSION['exercicio'] - 1;
        $sql = "
            SELECT
                oppid,
                oppcod || ' - ' || oppnome AS descricao,
                prsano
            FROM public.objetivoppa
            WHERE
                oppstatus = 'A'
                AND prsano = '{$_SESSION['exercicio']}'
                AND oppcod = (
                    SELECT
                        oppcod
                    FROM public.objetivoppa
                    WHERE
                        oppstatus = 'A'
                        AND prsano = '{$anoAnterior}'
                        and oppid = '". (int)$oppid. "'
                )
        ";

        return $this->pegaUm($sql);
    }

    /**
     * Retorna a ID do campo Metas PPA do ano atual, baseado no ano anterior
     *
     * @param $mppid
     * @return bool|mixed|NULL|string
     */
    private function getMetasPPAIDAnoAnterior($mppid = NULL) {
        $anoAnterior = $_SESSION['exercicio'] - 1;
        $sql = "
            SELECT DISTINCT
                m.mppid
            FROM public.metappa m
                JOIN public.objetivometappa om ON m.mppid = om.mppid
            WHERE
                m.mppstatus = 'A'
                AND m.prsano = '{$_SESSION['exercicio']}'
                AND m.mppcod = (
                    SELECT DISTINCT
                        m.mppcod
                    FROM public.metappa m
                        JOIN public.objetivometappa om ON m.mppid = om.mppid
                    WHERE
                        m.mppstatus = 'A'
                        AND m.prsano = '{$anoAnterior}'
                        AND om.mppid = '". (int)$mppid. "'
                )
        ";

        return $this->pegaUm($sql);
    }

    /**
     * Retorna a ID do campo Iniciativa PPA do ano atual, baseado no ano anterior
     *
     * @param $oppid
     * @param $ippid
     * @return bool|mixed|NULL|string
     */
    private function getIniciativaPPAIDAnoAnterior($oppid = NULL, $ippid = NULL) {
        $anoAnterior = $_SESSION['exercicio'] - 1;
        $ippid = $ippid ? $ippid : 0;
        $sql = "
            SELECT
                ippid
            FROM public.iniciativappa
            WHERE
                ippstatus = 'A'
                AND prsano = '{$_SESSION['exercicio']}'
                AND oppid = '".(int)$oppid. "'
                AND ippcod = (
                    SELECT
                        ippcod
                    FROM public.iniciativappa
                    WHERE
                        ippstatus = 'A'
                        AND prsano = '{$anoAnterior}'
                        AND ippid = '". (int)$ippid. "'
                )
        ";

        return $this->pegaUm($sql);
    }

    /**
     * Retorna a ID do campo Meta PNC do ano atual, baseado no ano anterior
     *
     * @param $mpnid
     * @return bool|mixed|NULL|string
     */
    private function getMetaPNCIDAnoAnterior($mpnid = NULL) {
        $anoAnterior = $_SESSION['exercicio'] - 1;
        $sql = "
            SELECT
                mpn.mpnid AS codigo
            FROM public.metapnc mpn
            WHERE
                mpn.mpnstatus = 'A'
                AND mpn.prsano  = '{$_SESSION['exercicio']}'
                AND mpn.mpncod = (
                    SELECT
                        mpn.mpncod
                    FROM public.metapnc mpn
                    WHERE
                        mpn.mpnstatus = 'A'
                        AND mpn.prsano = '{$anoAnterior}'
                        and mpn.mpnid = '". (int)$mpnid. "'
                )	
        ";
//ver($sql, d);
        return $this->pegaUm($sql);
    }

    /**
     * Retorna a ID do campo Indicador PNC do ano atual, baseado no ano anterior
     *
     * @param $ipnid
     * @return bool|mixed|NULL|string
     */
    private function getIndicadorPNCIDAnoAnterior($ipnid = NULL) {
        $anoAnterior = $_SESSION['exercicio'] - 1;
        $sql = "
            SELECT
                ipnid
            FROM public.indicadorpnc
            WHERE
                ipnstatus = 'A'
                AND prsano = '{$_SESSION['exercicio']}'
                AND ipncod = (
                    SELECT
                        ipncod
                    FROM public.indicadorpnc
                    WHERE
                        ipnstatus = 'A'
                        AND prsano = '{$anoAnterior}'
                        AND ipnid = '". (int)$ipnid. "'
                )
        ";

        return $this->pegaUm($sql);
    }

    /**
     * Retorna a ID do campo Área Cultural do ano atual, baseado no ano anterior
     *
     * @param $mdeid
     * @return bool|mixed|NULL|string
     */
    private function getAreaCulturalIDAnoAnterior($mdeid = NULL) {
        $anoAnterior = $_SESSION['exercicio'] - 1;
        $sql = "
            SELECT
                MAX(mdeid),
                upper(public.removeacento(mdedsc)) as descricao
            FROM monitora.pi_modalidadeensino
            WHERE
                mdestatus = 'A'
                AND mdeano = '{$_SESSION['exercicio']}'
                AND upper(public.removeacento(mdedsc)) = (
                    SELECT
                        upper(public.removeacento(mdedsc))
                    FROM monitora.pi_modalidadeensino
                    WHERE
                        mdestatus = 'A'
                        AND mdeano = '{$anoAnterior}'
                        AND mdeid = '". (int)$mdeid. "'
                )
            GROUP BY
                descricao
        ";
        return $this->pegaUm($sql);
    }

    /**
     * Retorna a ID do campo Segmento Cultural do ano atual, baseado no ano anterior
     *
     * @param $neeid
     * @return bool|mixed|NULL|string
     */
    private function getSegmentoCulturalIDAnoAnterior($neeid = NULL) {
        $anoAnterior = $_SESSION['exercicio'] - 1;
        $sql = "
            SELECT
                neeid AS codigo
            FROM monitora.pi_niveletapaensino
            WHERE
                neeano = '{$_SESSION['exercicio']}'
                AND neestatus = 'A'
                AND needsc = (
                    SELECT
                        needsc AS descricao
                    FROM monitora.pi_niveletapaensino
                    WHERE
                        neeano = '{$anoAnterior}'
                        AND neestatus = 'A'
                        AND neeid = '". (int)$neeid. "'
                )
        ";

        return $this->pegaUm($sql);
    }

}//end Class
?>