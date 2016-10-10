<?php
//
/**
 * Listagem de subações.
 *
 * @package SiMEC
 * @subpackage planejamento-acompanhamento-orcamentario
 */
include_once APPRAIZ . "includes/cabecalho.inc";
require_once APPRAIZ . "includes/library/simec/Helper/FlashMessage.php";
?>
<section class="col-md-12">
    <ol class="breadcrumb">
        <li><a href="/<?php echo $_SESSION['sisdiretorio']; ?>/<?php echo $_SESSION['sisdiretorio']; ?>.php?modulo=inicio&acao=C"><?= $_SESSION['sisdsc']; ?></a></li>
        <li class="active">Extrato completo das Ações <?= $_SESSION['exercicio']; ?></li>
    </ol>
</section>
<script>
function exibirAcao(unicod, acacod) {
 location.href = '<?php echo $_SESSION['sisdiretorio']; ?>.php?modulo=principal/dadosacoes/detalhardadosacoes&acao=A&unicod='+unicod+'&acacod='+acacod;
}
</script>
<section class="col-md-12">
    <form id="formPesquisa" name="formPesquisa" method="POST" class="form-horizontal" >
    <div class="well">
        <div class="form-group">
            <div class="col-md-2 text-right">
                <label class="control-label">Unidade Orçamentária (UO)</label>
            </div>
            <div class="col-md-10">
                <p class="form-control-static">
                    <?php
                    $sql = "
                        SELECT DISTINCT
                            unicod as codigo,
                            unicod || ' - ' ||unidsc as descricao
                           FROM
                            public.unidade
                           WHERE
                            unistatus = 'A'
                           AND
                            orgcod = '". CODIGO_ORGAO_SISTEMA. "'
                           ORDER BY
                            1";
                    inputCombo('unicod', $sql, $_REQUEST['unicod'], 'unicod', array());
                    ?>
                </p>
            </div>
        </div>
        <div class="form-group">
            <div class="col-md-2 text-right">
                <label class="control-label" for="id_acao_programatica">
                    Ação
                </label>
            </div>
            <div class="col-md-10">
                <?php
                $sql = "
                    SELECT DISTINCT
                    acacod as codigo,
                    acacod||' - '||acadsc as descricao
                   FROM
                    monitora.acao
                   WHERE
                    prgano = '{$_SESSION['exercicio']}'
                   AND
                    acastatus = 'A'
                   ORDER BY
                    1";
                inputCombo('acacod', $sql, $_REQUEST['acacod'], 'acacod', array());
                ?>
            </div>
        </div>
        <div class="form-group">
            <div class="col-lg-10 col-lg-offset-2">
                <button class="btn btn-warning" type="reset" onclick="window.location = window.location;">
                    <i class="glyphicon glyphicon-remove-circle"></i>
                    Limpar
                </button>
                <button class="btn btn-success" id="pesquisar" type="submit">
                    <i class="glyphicon glyphicon-search"></i>
                    Filtrar
                </button>
            </div>
        </div>
    </div>
    </form>
<?php

$where .= $_REQUEST['unicod'] ? "AND uni.unicod = '" . $_REQUEST['unicod'] . "'" : '';
$where .= $_REQUEST['acacod'] ? "AND aca.acacod = '" . $_REQUEST['acacod'] . "'" : '';
$whereUO = '';

$sql = <<<DML
        SELECT DISTINCT
            uni.unicod                           ,
            aca.acacod                           ,
            uni.unicod || ' - ' ||uni.unidsc AS acao ,
            aca.acacod||' - '||aca.acadsc    AS unidade,
            SUM( sex.vlrdotacaoatual ) AS dotacaoatual          ,
            SUM( sex.vlrempenhado )    AS empenhado             ,
            SUM( sex.vlrliquidado )    AS liquidado             ,
            SUM( sex.vlrpago )         AS pago
        FROM
         monitora.acao aca
        JOIN
         public.unidade uni
         ON uni.unicod = aca.unicod
        LEFT JOIN spo.siopexecucao SEX
        ON sex.unicod = aca.unicod AND sex.acacod=aca.acacod AND sex.exercicio = aca.prgano
        WHERE
         prgano = '{$_SESSION['exercicio']}'
        AND
         acastatus = 'A'
         {$where}
         {$whereUO}
        GROUP BY 1, 2 ,3, 4 
        ORDER BY
         1 , 2
DML;
#ver($sql);
?>

    <div class="col-md-12">
<?php
$listagem = new Simec_Listagem();
$listagem->setCabecalho(array(
    "Unidade Orçamentária (UO)",
    "Ação",
    "Execução (R$)"=>array(
        'Dotação Atual',
        'Empenhado',
        'Liquidado',
        'Pago')
    ));
$listagem->setQuery($sql);
$listagem->addAcao('view', array('func'=>'exibirAcao', 'extra-params'=>array('acacod')));
$listagem->esconderColunas(array('acacod', 'unicod'));
$listagem->addCallbackDeCampo(array('acao', 'unidade' ), 'alinhaParaEsquerda');
$listagem->addCallbackDeCampo(array('dotacaoatual', 'empenhado', 'liquidado', 'pago'), 'mascaraMoeda');
#$listagem->setTotalizador(Simec_Listagem::TOTAL_SOMATORIO_COLUNA, array('dotacaoatual', 'empenhado', 'liquidado', 'pago'));
$listagem->turnOnPesquisator();
$listagem->render(Simec_Listagem::SEM_REGISTROS_MENSAGEM);
?>
    </div>
</section>