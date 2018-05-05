<?php
/**
 * Classe de mapeamento da entidade monitora.ptres
 *
 * @category Class
 * @package  A1
 * @author   ORION TELES DE MESQUITA <orion.mesquita@cultura.gov.br>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 23-11-2017
 * @link     no link
 */

require_once APPRAIZ .'includes/classes/Modelo.class.inc';
require_once(APPRAIZ . 'wssof/classes/Importador.inc');
require_once(APPRAIZ . 'wssof/classes/Ws_AcoesDto.inc');
require_once(APPRAIZ . 'monitora/classes/model/Acao.inc');


/**
 * Monitora_Model_Ptres
 *
 * @category Class
 * @package  A1
 * @author    <>
 * @license  GNU simec.mec.gov.br
 * @version  Release:
 * @link     no link
 */
class Monitora_Model_Ptres extends Modelo
{
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = 'monitora.ptres';

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array(
        'ptrid',
    );
    /**
     * Chaves estrangeiras.
     * @var array
     */
    protected $arChaveEstrangeira = array(
        'acaid' => array('tabela' => 'acao', 'pk' => 'acaid'),
    );

    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributos = array(
        'ptrid' => null,
        'ptres' => null,
        'acaid' => null,
        'ptrano' => null,
        'funcod' => null,
        'sfucod' => null,
        'prgcod' => null,
        'acacod' => null,
        'loccod' => null,
        'unicod' => null,
        'irpcod' => null,
        'ptrdotacao' => null,
        'ptrstatus' => null,
        'ptrdata' => null,
        'plocod' => null,
        'esfcod' => null,
        'ptrdotacaocapital' => null,
        'ptrdotacaocusteio' => null,
        'plodsc' => null,
    );

    public function importarSiop($exercicio, $momento) {
        $exercicio = $exercicio ? $exercicio : date('Y');
        $momento = $momento ? $momento : 3000;

        $map = [
            'retornarAcoes' => 'acoesDTO',
            'retornarLocalizadores' => 'localizadoresDTO',
            'retornarPlanosOrcamentarios' => 'planosOrcamentariosDTO',
        ];

        $mImportador = new Wssof_Importador();
        $mImportador->obterProgramacaoCompleta($exercicio, $momento, $map);
        $this->atualizarFuncionaisSiop($exercicio, $momento);
        $this->commit();
    }

    public function atualizarFuncionaisSiop($exercicio, $momento) {
        $sql = "select  aca.identificadorunico,  aca.exercicio, aca.codigomomento, aca.baselegal, aca.detalhamentoimplementacao,
                        aca.codigoacao, aca.codigoorgao, aca.codigoprograma, aca.codigoobjetivo, 
                        aca.descricao acadescricao, aca.titulo acatitulo, loc.codigolocalizador, 
                        plo.planoorcamentario, loc.descricao locdescricao, plo.titulo plotitulo
                from wssof.ws_acoesdto aca
                        inner join wssof.ws_localizadoresdto loc on loc.identificadorunicoacao = aca.identificadorunico
                        inner join wssof.ws_planosorcamentariosdto plo on plo.identificadorunicoacao = aca.identificadorunico
                where aca.exercicio = '$exercicio'
                and aca.codigomomento = '$momento'
                ";
        $aAcoesDto = $this->carregar($sql);
        $aAcoesDto = $aAcoesDto ? $aAcoesDto : [];

        foreach($aAcoesDto as $acaoDto){
            $acaid = $this->atualizarAcaoSiop($acaoDto);
            $ptrid = $this->atualizarPtresSiop($acaoDto, $acaid);
        }
    }

    public function atualizarAcaoSiop($acaoDto)
    {
        $sql = "select acaid
                from monitora.acao aca
                where aca.prgano = '{$acaoDto['exercicio']}'
                and aca.acacod = '{$acaoDto['codigoacao']}'
                and aca.prgcod = '{$acaoDto['codigoprograma']}'
                and aca.loccod = '{$acaoDto['codigolocalizador']}'
                and aca.unicod = '{$acaoDto['codigoorgao']}'
                ";
        $acaid = $this->pegaUm($sql);

        $mAcao = new Monitora_Model_Acao($acaid);

        $mAcao->ididentificadorunicosiop = $acaoDto['identificadorunico'];
        $mAcao->acatitulo = $mAcao->acadsc = $acaoDto['acatitulo'];
        $mAcao->acadescricao = $acaoDto['acadescricao'];
        $mAcao->acabaselegal = $acaoDto['baselegal'];
        $mAcao->acadetalhamento = $acaoDto['detalhamentoimplementacao'];
        $mAcao->prgano = $acaoDto['exercicio'];
        $mAcao->acacod = $acaoDto['codigoacao'];
        $mAcao->prgcod = $acaoDto['codigoprograma'];
        $mAcao->loccod = $acaoDto['codigolocalizador'];
        $mAcao->saccod = $acaoDto['codigolocalizador'];
        $mAcao->unicod = $acaoDto['codigoorgao'];

        $mAcao->salvar();
        $acaid = $mAcao->acaid;
        unset($mAcao);

        return $acaid;
    }

    public function atualizarPtresSiop($acaoDto, $acaid)
    {
        $sql = "select ptrid
                from monitora.ptres ptr
                where ptr.ptrano = '{$acaoDto['exercicio']}'
                and ptr.acacod = '{$acaoDto['codigoacao']}'
                and ptr.prgcod = '{$acaoDto['codigoprograma']}'
                and ptr.loccod = '{$acaoDto['codigolocalizador']}'
                and ptr.unicod = '{$acaoDto['codigoorgao']}'
                and ptr.plocod = '{$acaoDto['planoorcamentario']}'
                ";
        $ptrid = $this->pegaUm($sql);

        $mPtres = new Monitora_Model_Ptres($ptrid);

        $mPtres->ptres = 0;
        $mPtres->acaid = $acaid;
        $mPtres->ptrano = $acaoDto['exercicio'];
        $mPtres->prgcod = $acaoDto['codigoprograma'];
        $mPtres->acacod = $acaoDto['codigoacao'];
        $mPtres->loccod = $acaoDto['codigolocalizador'];
        $mPtres->unicod = $acaoDto['codigoorgao'];
        $mPtres->plocod = $acaoDto['planoorcamentario'];
        $mPtres->plodsc = $acaoDto['plotitulo'];

        $mPtres->salvar();
        $ptrid = $mPtres->ptrid;

        unset($mPtres);

        return $ptrid;
    }

    public function recuperarSqlCombo($descricao = null, $where = array(), $order = '') {
        $pk = $this->arChavePrimaria[0];

        foreach (array_keys($this->arAtributos) as $atributo) {
            if (substr($atributo, -6) == 'status') {
                $where[] = "{$atributo} = 'A'";
            } elseif (substr($atributo, -3) == 'ano') {
                $where[] = "$atributo = '{$_SESSION['exercicio']}'";
            }
            if ($descricao) {
                $descricao = implode(" || ' - ' || ", (array)$descricao);
            } else {
                $aPossibilidades = ['dsc', 'desc', 'nome', 'descricao'];
                foreach ($aPossibilidades as $possibilidade) {
                    if (substr($atributo, -(strlen($possibilidade))) == $possibilidade) {
                        $descricao = $atributo;
                        break;
                    }
                }
            }
        }
        $order = $order ? $order : $descricao;
        $where = count($where) ? " where  " . implode(' and ', $where) : '';

        $sql = "
            SELECT
                {$pk} AS codigo,
                {$descricao} AS descricao
            FROM {$this->stNomeTabela}
                JOIN monitora.acao ON ptres.acaid = acao.acaid
                JOIN public.unidadeorcamentaria ON acao.unicod = unidadeorcamentaria.unocod AND acao.prgano = unidadeorcamentaria.prsano
            $where
            ORDER BY
                $order
        ";

        return $sql;
    }

    public function recuperarSqlPorSubunidadeEEnquadramento($suoid, $eqdid)
    {
        $sql = "select  ptr.ptrid codigo, 
                        coalesce(ptr.prgcod, '') || ' - ' || coalesce(ptr.acacod, '') || ' - ' || coalesce(ptr.loccod, '') || ' - ' || coalesce(ptr.acaobjetivocod, '') || ' - ' || coalesce(ptr.plocod, '') || ' - ' || ptr.acatitulo  || ' - ' || ptr.plodsc descricao
                from monitora.vw_ptres ptr
                        inner join spo.ptressubunidade psu on psu.ptrid = ptr.ptrid
                        inner join monitora.enquadramentorp erp on erp.irpcod = ptr.irpcod and erp.eqdid = " . (int) $eqdid . "
                where ptr.ptrano = '{$_SESSION['exercicio']}'
                and ptr.ptrstatus = 'A'
                and ptr.plocod != 'EIND'
                and psu.suoid = " . (int) $suoid . "
                order by descricao";

        return $sql;
    }

    public function recuperarDadosView()
    {
        $sql = "select  * from monitora.vw_ptres ptr 
                where ptr.ptrano = '{$_SESSION['exercicio']}'
                and ptr.ptrstatus = 'A'
                and ptrid = " . (int) $this->ptrid;

        return $this->pegaLinha($sql);
    }

    function recuperarObjetivoPorPtres() {
        global $db;

        $sql = "SELECT o.oppid
                FROM monitora.ptres ptr
                INNER JOIN monitora.acao aca on ptr.acaid = aca.acaid and aca.prgano = ptr.ptrano
                INNER JOIN public.objetivoppa o on o.oppcod = aca.acaobjetivocod and o.prsano = ptr.ptrano
            where ptrid = " . (int) $this->ptrid;

        return $db->pegaUm($sql);
    }

    function recuperarValoresPrePi() {
        global $db;

        $sql = "select sum(pre.plivalorcapital) capitalptres, sum(pre.plivalorcusteio) custeioptres
                from proposta.preplanointerno pre
                where ptrid = " . (int) $this->ptrid;

        return json_encode($db->pegaLinha($sql));
    }

    public function recuperarTodosApoio(stdClass $param) {
        $sql = "
            SELECT DISTINCT
                ptr.ptrid,
                suo.unocod || '(' || suo.unonome || ')' AS unidade_orcamentaria,
                vptr.acacod,
                vptr.acatitulo,
                ptr.ptres,
                vptr.funcional,
                vptr.plocod,
                vptr.plodsc,
                vptr.irpcod,
                ptr.ptrdotacaocusteio,
                ptr.ptrdotacaocapital
            FROM monitora.ptres ptr
                JOIN public.vw_subunidadeorcamentaria suo ON(
                    ptr.unicod = suo.unocod
                    AND ptr.ptrano = suo.prsano
                    AND suo.suostatus = 'A'
                )
                JOIN monitora.vw_ptres vptr ON ptr.ptrid = vptr.ptrid-- SELECT * FROM monitora.vw_ptres 
            WHERE
                ptr.ptrstatus = 'A'
                AND ptr.ptrano = '". (int)$param->exercicio. "'
        ";
        
        $listaResultado = $this->carregar($sql);
        return $listaResultado? $listaResultado: [];
    }

    function recuperarProdutosPorPtres() {
        global $db;
        
        ver((int) $this->ptrid, d);

        $sql = "select sum(pre.plivalorcapital) capitalptres, sum(pre.plivalorcusteio) custeioptres
                from proposta.preplanointerno pre
                where ptrid = " . (int) $this->ptrid;

        return $db->pegaLinha($sql);
    }
    
}//end Class
?>