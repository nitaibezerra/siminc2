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

    public function recuperarValoresPorSubnidadeEPtres($suoid, $ptrid)
    {
        $sql = "select  sum(pli.plivalorcapital) plivalorcapital, sum(pli.plivalorcusteio) plivalorcusteio, 
                        sum(pli.plivalorcapitaladicional) plivalorcapitaladicional, sum(pli.plivalorcusteioadicional) plivalorcusteioadicional
                from proposta.preplanointerno pli
                where suoid = " . (int) $suoid . "
                and ptrid = " . (int) $ptrid;

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

}//end Class
?>