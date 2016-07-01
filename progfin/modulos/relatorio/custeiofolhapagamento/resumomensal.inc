<?php
/**
 * Arquivo de carga de dados da programação financeira.
 * $Id: resumomensal.inc 103017 2015-09-30 12:29:56Z werteralmeida $
 */
$fm = new Simec_Helper_FlashMessage('progfin/custeiofolhapagamento');

/* Montando a Pesquisa */
#ver($_REQUEST);
$dados = $_REQUEST['dados'];

if (chaveTemValor($_REQUEST, 'mes') && $_REQUEST['mes'] != 'undefined') {
    $mes = $_REQUEST ['mes'];
    $where[] = " dcfmes = '{$mes}' ";
} else {
    $mes = $db->pegaUm("SELECT lpad(MAX(cfpmes::integer)::text, 2, '0') FROM progfin.cargacusteiofolhapagamento WHERE cfpano::integer =  {$_SESSION['exercicio']} ");
    $where[] = " dcfmes = '{$mes}' ";
}
if (isset($_REQUEST['dados'])) {
    if (chaveTemValor($_REQUEST['dados'], 'unicod') && $_REQUEST['dados']['unicod'] != 'undefined') {
        $where[] = "uni.unicod = '{$_REQUEST['dados']['unicod']}'";
    }
}

if (!empty($whereAdicionalUO)) {
    $where[] = 'uni.' . $whereAdicionalUO[0];
}

if (!empty($where)) {
    $where = implode(' AND ', $where);
} else {
    $where = '';
}
$sqlPesquisa = "
SELECT DISTINCT CASE dcfp.dcfmes WHEN '01' THEN 'Janeiro' WHEN '02' THEN 'Fevereiro' WHEN '03' THEN 'Março' WHEN '04' THEN 'Abril' WHEN '05' THEN 'Maio' WHEN '06' THEN 'Junho' WHEN '07' THEN 'Julho'
                                 WHEN '08' THEN 'Agosto' WHEN '09' THEN 'Setembro' WHEN '10' THEN 'Outubro' WHEN '11' THEN 'Novembro' ELSE 'Dezembro' END AS mes ,
 uni.unicod                                                                                                                                                      ,
 uni.unidsc                                                                                                                                                      ,
 SUM( regra_15 ) AS regra_15                                                                                                                                     ,
 SUM( regra_12 ) AS regra_12                                                                                                                                     ,
 SUM( regra_21 ) AS regra_21                                                                                                                                     ,
 SUM( regra_17 ) AS regra_17                                                                                                                                     ,
 SUM( regra_8 )  AS regra_8                                                                                                                                      ,
 SUM( regra_9 )  AS regra_9                                                                                                                                      ,
 SUM( regra_10 ) AS regra_10                                                                                                                                     ,
 SUM( regra_11 ) AS regra_11                                                                                                                                     ,
 SUM( regra_13 ) AS regra_13                                                                                                                                     ,
 SUM( regra_14 ) AS regra_14                                                                                                                                     ,
 SUM( regra_16 ) AS regra_16                                                                                                                                     ,
 SUM( regra_20 ) AS regra_20                                                                                                                                     ,
 SUM( regra_18 ) AS regra_18                                                                                                                                     ,
 SUM( regra_19 ) AS regra_19,
 SUM( regra_22 ) AS regra_22
FROM
 progfin.dadoscusteiofolhapagamento dcfp
INNER JOIN
 progfin.relacaounidadesigep rus
ON
 dcfp.dcforgao = rus.cfporgao
INNER JOIN
 public.unidade uni
ON
 uni.unicod = rus.unicod
WHERE
 dcfp.dcfano = '{$_SESSION['exercicio']}'
AND $where
GROUP BY
 dcfp.dcfmes ,
 uni.unicod  ,
 uni.unidsc
ORDER BY
 uni.unicod ,
 uni.unidsc";

$iconeRegra = array(
    'regra_15' => pegaIconeRegra('regra_15'),
    'regra_12' => pegaIconeRegra('regra_12'),
    'regra_21' => pegaIconeRegra('regra_21'),
    'regra_17' => pegaIconeRegra('regra_17'),
    'regra_8' => pegaIconeRegra('regra_8'),
    'regra_9' => pegaIconeRegra('regra_9'),
    'regra_10' => pegaIconeRegra('regra_10'),
    'regra_11' => pegaIconeRegra('regra_11'),
    'regra_13' => pegaIconeRegra('regra_13'),
    'regra_14' => pegaIconeRegra('regra_14'),
    'regra_16' => pegaIconeRegra('regra_16'),
    'regra_20' => pegaIconeRegra('regra_20'),
    'regra_18' => pegaIconeRegra('regra_18'),
    'regra_19' => pegaIconeRegra('regra_19'),
    'regra_22' => pegaIconeRegra('regra_22'));

$cabecalhoTabela = array(
    'Mês',
    'Cod. Unidade Orçamentária (UO)',
    'Unidade Orçamentária (UO)',
    'ASSISTÊNCIA MÉDICA+EX.ANTERIORES' . $iconeRegra['regra_15'],
    'CONTRATO TEMPORÁRIO' . $iconeRegra['regra_12'],
    'SENTENÇA JUDICIAL' . $iconeRegra['regra_21'],
    'AUX. PRÉ-ESCOLAR+EX. ANTERIORES' . $iconeRegra['regra_17'],
    'INDENIZAÇÃO DE TRANSPORTE' . $iconeRegra['regra_8'],
    'PENSÃO INDENIZATÓRIA' . $iconeRegra['regra_9'],
    'PENSÕES GRACIOSAS' . $iconeRegra['regra_10'],
    'RESIDÊNCIA MÉDICA' . $iconeRegra['regra_11'],
    'ESTAGIÁRIOS' . $iconeRegra['regra_13'],
    'CURSO/CONCURSO+EX. ANTERIORES' . $iconeRegra['regra_14'],
    'AUXÍLIO NATALIDADE E DEFICENTE' . $iconeRegra['regra_16'],
    'INCENTIVO EDUCACIONAL' . $iconeRegra['regra_20'],
    'AUX. TRANSPORTE+EX. ANTERIORES' . $iconeRegra['regra_18'],
    'AUX. ALIMENTAÇÃO+EX. ANTERIORES' . $iconeRegra['regra_19'],
    'AJUDA DE CUSTO' . $iconeRegra['regra_22'],
);

$camposDinheiro = array(
    'regra_8',
    'regra_9',
    'regra_10',
    'regra_11',
    'regra_12',
    'regra_13',
    'regra_14',
    'regra_15',
    'regra_16',
    'regra_17',
    'regra_18',
    'regra_19',
    'regra_20',
    'regra_21',
    'regra_22',
);

$tipoRelatorio = ($_REQUEST['requisicao'] == 'xls')
    ?Simec_Listagem::RELATORIO_XLS
    :Simec_Listagem::RELATORIO_CORRIDO;

$listagem = new Simec_Listagem($tipoRelatorio);
$listagem->setCabecalho($cabecalhoTabela);
$listagem->setQuery($sqlPesquisa);
$listagem->setTotalizador(Simec_Listagem::TOTAL_SOMATORIO_COLUNA, $camposDinheiro);
$listagem->addCallbackDeCampo($camposDinheiro, 'formataDinheiro');

if ($_REQUEST['requisicao'] == 'xls') {
    $listagem->render();
    die();
} else {
    $listagem->turnOnPesquisator();
}
/**
 * Cabecalho padrão do SIMEC.
 * @see cabecalho.inc
 */
require(APPRAIZ . 'includes/cabecalho.inc');
?>
<div class="row col-md-12">
    <ol class="breadcrumb">
        <li><a href="progfin.php?modulo=inicio&acao=C">Programação Financeira</a></li>
        <li>Extração de Custeio da Folha de Pagamento</li>
        <li class="active">Relatório resumo da Folha Mensal</li>
    </ol>
    <script type="text/javascript">
    $('documento').ready(function () {
        $('#buttonSend').click(function () {
            $('#formBusca').submit();
        });
        $('[data-toggle="popover"]').popover({
            placement:'bottom', title:'Detalhes', html:true, trigger:'hover'
        });
        $('#buttonClear').click(function () {
            url = 'progfin.php?modulo=relatorio/custeiofolhapagamento/resumomensal&acao=A';
            $(location).attr('href', url);
        });
        $('#gerar-xls').click(function(){
            $('#requisicao').val('xls');
            $('#formBusca').submit();
        });
    });
    </script>
    <div class="well">
    <form name="formBusca" id="formBusca" method="POST" role="form" class="form-horizontal">
        <input type="hidden" name="requisicao" id="requisicao" value="filtrar" />
        <label class="control-label col-md-2" for="fdsid">Mês:</label>
        <div class="col-md-10">
            <?php
            $sql = "  SELECT
                    DISTINCT cfpmes as codigo,
                    CASE WHEN cfpmes::integer = 1 THEN 'Janeiro'
                    WHEN cfpmes::integer = 2 THEN 'Fevereiro'
                    WHEN cfpmes::integer = 3 then 'Março'
                    WHEN cfpmes::integer = 4 THEN 'Abril'
                    WHEN cfpmes::integer = 5 THEN 'Maio'
                    WHEN cfpmes::integer = 6 THEN 'Junho'
                    WHEN cfpmes::integer = 7 THEN 'Julho'
                    WHEN cfpmes::integer = 8 THEN 'Agosto'
                    WHEN cfpmes::integer = 9 THEN 'Setembro'
                    WHEN cfpmes::integer = 10 THEN 'Outubro'
                    WHEN cfpmes::integer = 11 THEN 'Novembro'
                    WHEN cfpmes::integer = 12 THEN 'Dezembro' end as descricao
              FROM progfin.cargacusteiofolhapagamento
              WHERE cfpano::integer =  {$_SESSION['exercicio']}";
            inputCombo('mes', $sql, $mes, 'trocaMesCarga');
            ?>
        </div>
        <br style="clear:both" />
        <br style="clear:both" />
        <div class="form-group row">
            <label class="control-label col-md-2" for="unicod">Unidade Orçamentária (UO):</label>
            <div class="col-md-10">
                <?php
                $whereAdicionalUO = array();
                if (in_array(PFL_UO_EQUIPE_TECNICA, pegaPerfilGeral($_SESSION['usucpf']))) {
                    $whereAdicionalUO[] = 'unicod IN(' . pegaResposabilidade($_SESSION['usucpf'], PFL_UO_EQUIPE_TECNICA, 'unicod') . ')';
                }
                inputComboUnicod($dados['unicod'], $whereAdicionalUO);
                ?>
            </div>
        </div>
        <hr />
        <button onclick="divCarregando();" class="btn btn-success" id="buttonSend" type="submit"><i class="glyphicon glyphicon-search"></i> Filtrar </button>
        <button class="btn btn-warning" id="buttonClear" type="reset">Limpar</button>
        <input class="btn btn-primary start" type="button" name="" id="gerar-xls" value="Gerar XLS" />
        <br style="clear:both" />
    </form>
    </div>
    <div class="row col-md-12">
        <?php
        echo $fm->getMensagens();
        $listagem->render();
        if ($_SESSION['superuser'] == 1) {
            echo "<br/><div style=\" color:#FFF \"> {$sqlPesquisa} </div><br/><br/><br/>";
        }
        ?>
    </div>
</div>
<?php
function pegaIconeRegra($regra) {
    global $db;
    $regra = $db->pegaLinha("SELECT rcc.rccelementodespesa , rcc.rccrubrica FROM progfin.regrascargacusteiofolhapagamento rcc WHERE rccnomecoluna = '{$regra}'");
    $texto = <<<TEXTO
<b>Elemento(s) de despesa</b>: {$regra['rccelementodespesa']}<br />
<b>Rubrica(s)</b>: {$regra['rccrubrica']}
TEXTO;
    return "&nbsp <span data-toggle=\"popover\" class=\"glyphicon glyphicon-info-sign\" style=\"color:#888\" data-container=\"body\" data-content=\"$texto\"></span>";
}
