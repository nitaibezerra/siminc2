<?php
/**
 * $Id: centralacompanhamento.inc 92808 2015-01-19 13:05:58Z maykelbraz $
 */
/**
 * Funções de manipulação de ações.
 * @see _funcoesacoes.php
 */
include_once '_funcoesacoes.php';
include_once(APPRAIZ . 'includes/funcoesspo.php');
/* Funções de Gráficos do Cockpit */
include_once '../../pde/www/_funcoes_cockpit.php';

if (!isset($_REQUEST['percod']) || $_REQUEST['percod'] == '') {
    $periodoSelecionado = recuperaUltimoPeriodoReferencia();
} else {
    $periodoSelecionado = $_REQUEST['percod'];
}
$percod = $periodoSelecionado;
if ($_REQUEST['requisicao']) {
    $_REQUEST['requisicao']($_REQUEST);
    exit;
}
if ($_REQUEST['diligenciadas'] && $_REQUEST['diligenciadas'] == 'true') {
    $diligenciadasMarcado = 'checked=checked';
}

$perfis = pegaPerfilGeral();
//Chamada de programa
include APPRAIZ . "includes/cabecalho.inc";
if (1 != $_SESSION['superuser']) {
    if (in_array(PFL_VALIDADORACAO, $perfis) || in_array(PFL_VALIDADOR_SUBSTITUTO, $perfis)) {
        die("<script>window.location='planacomorc.php?modulo=centralacompanhamento&acao=A';</script>");
    } elseif (in_array(PFL_COORDENADORACAO, $perfis) || in_array(PFL_CONSULTA, $perfis)) {
        die("<script>window.location='planacomorc.php?modulo=principal/acoes/listaunidades&acao=A';</script>");
    } elseif (in_array(PFL_COORDENADORSUBACAO, $perfis)) {
        die("<script>window.location='planacomorc.php?modulo=principal/subacoes/listasubacoes&acao=A';</script>");
    }
}
?>
<style>
    .marcado{background-color:#C1FFC1;}
    .remover{ display:none;}
</style>
<script type="text/javascript" lang="JavaScript" src="/includes/funcoesspo.js"></script>
<script>
    $(document).ready(function()
    {

        $('#percod').change(function() {
            window.location = "planacomorc.php?modulo=centralacompanhamento&acao=A&percod=" + this.value;
        });
    	pesquisaRapida('textFind');
    });

    function pesquisaRapida(id) {
        jQuery.expr[':'].contains = function(a, i, m) {
            return jQuery(a).text().toUpperCase().indexOf(m[3].toUpperCase()) >= 0;
        };

        $("#" + id).keyup(function()
        {
            $('table.table tbody tr td').removeClass('marcado');
            $('table.table tbody tr').removeClass('remover');
            stringPesquisa = $("#" + id).val();
            if (stringPesquisa) {
                $('table.table tbody tr td:contains(' + stringPesquisa + ')').addClass('marcado');
                $('table.table tbody tr:not(:contains(' + stringPesquisa + '))').addClass('remover');
            }
        });
    }

    function monitorarAcao(id_acao_programatica, loccod) {
        if ($('#percod').val() == '') {
            alert('Período de referência não esta selecionado');
            return false;
        }
        window.location = "planacomorc.php?modulo=principal/acoes/gerenciarunidades&acao=A&aba=monitoraracao&id_acao_programatica="
                + id_acao_programatica + "&loccod=" + loccod + "&percod=" + $('#percod').val();
    }

    function detalharDiagnosticoMonitoramentoAcao(orgcod, percod, obj) {
        alert("entrou");
        console.log('orgcod: '+ orgcod);
        console.log('percod: '+ percod);
        console.log('obj: '+ obj);
        var tabela = obj.parentNode.parentNode.parentNode.parentNode;
        var linha = obj.parentNode.parentNode.parentNode;

        if (obj.title == "mais") {
            sectionCarregando();
            obj.title = "menos";
            obj.src = "../imagens/menos.gif";
            var nlinha = tabela.insertRow(linha.rowIndex);
            var ncol0 = nlinha.insertCell(0);
            ncol0.innerHTML = "&nbsp;"

            var ncol1 = nlinha.insertCell(1);
            ncol1.colSpan = 7;
            ncol1.id = 'dtl2_coluna_' + (nlinha.rowIndex);
            ajaxatualizar('requisicao=detalharDiagnosticoMonitoramentoAcao&orgcod=' + orgcod + '&percod=' + percod, ncol1.id);
            sectionCarregado();
        } else {
            obj.title = "mais";
            obj.src = "../imagens/mais.gif";
            tabela.deleteRow(linha.rowIndex);
        }
    }

    function ajaxatualizar(params, iddestinatario) {
        $.ajax({
            type: "POST",
            url: window.location.href,
            data: params,
            async: false,
            success: function(html) {
                if (iddestinatario != '') {
                    document.getElementById(iddestinatario).innerHTML = html;
                }
            }
        });

    }
    function filtraDiligenciados() {
        if($("#ckh_filtro_diligenciados").hasClass('active')){
            filtro = false;
        }else {
            filtro = true;
        }
        //var filtro = $("#ckh_filtro_diligenciados").attr('checked');
        window.location = "planacomorc.php?modulo=centralacompanhamento&acao=A&diligenciadas=" + filtro;
    }

</script>
<section class="col-lg-12">
    <ol class="breadcrumb">
        <li><a href="/planacomorc/planacomorc.php?modulo=inicio&acao=C"><?= $_SESSION['sisdsc']; ?></a></li>
        <li class="">Ações <?= $_SESSION['exercicio']; ?></li>
        <li class="active">Painel de Diagnóstico de Acompanhamento de Ações</li>
    </ol>
    <section class="well">
        <section class="form-horizontal">
            <section class="form-group">
                <label class="control-label col-md-2" for="percod">Período</label>
                <section class="col-md-10">
                    <?php
                    $sql = "SELECT id_periodo_referencia AS codigo,
                           titulo || ' : ' || to_char(inicio_validade,'DD/MM/YYYY') ||' a ' || to_char(fim_validade,'DD/MM/YYYY') as descricao
                        FROM planacomorc.periodo_referencia p
                        WHERE id_exercicio = '" . $_SESSION['exercicio'] . "'
                        ORDER BY id_periodo_referencia desc";
                    $valor = $_REQUEST['percod'];
                    $opcoesAdicionais = array();
                    inputCombo(
                            'percod', $sql, $valor, 'percod', $opcoesAdicionais
                    );
                    ?>
                </section>
            </section>
            <section class="form-group">
                <section class="col-md-2"></section>
                <section class="col-md-10">
                    <section class="btn-group" data-toggle="buttons">
                        <label id="ckh_filtro_diligenciados" class="btn btn-default <?php echo isset($_REQUEST['diligenciadas']) && $_REQUEST['diligenciadas']!='false' ? 'active' : ''?>" onclick="filtraDiligenciados();">
                            <input   type ="checkbox"  <?php echo $diligenciadasMarcado ?> />
                            Apenas ações diligenciadas
                        </label>
                    </section>
                </section>
            </section>
        </section>
    </section>

    <section class="col-md-7">
        <?php
        /* Usa a mesma SQL da Combo */
        $pers = $db->carregar($sql);
        if ($pers[0]) {
            if (count($pers) == 1)
                $periodoreferencia = current($pers);
            	diagnosticoMonitoramentoAcao(array('percod' => $periodoreferencia['codigo'], 'diligenciadas' => $_REQUEST['diligenciadas']));
        } else {
            //echo "<p align='center'><span style=color:red;>Não existem períodos de referência cadastrados</span></p>";
            echo "<section class=\"alert alert-danger text-center\">Não existem períodos de referência cadastrados</section>";
        }
        ?>
    </section>

    <section class="col-md-5">
    	<section class="panel panel-primary">
            <section class="panel-heading"><strong>GRÁFICO</strong></section>
            <section class="panel-body" style="background-color: royalblue;">
            <?
            $sql = <<<DML
SELECT esddsc AS descricao,
       COUNT(esdid) AS valor
  FROM (SELECT DISTINCT aca.codigo AS acacod,
                        org.codigo AS unicod,
                        org.descricao AS unidsc,
                        lpr.loccod,
                        COALESCE(esddsc, 'Não Iniciado') AS esddsc,
                        COALESCE(esdid, -1) AS esdid
          FROM planacomorc.acao aca
            JOIN planacomorc.acao_programatica apr
              ON apr.id_acao = aca.id_acao
            JOIN planacomorc.dados_acao_exercicio dae
              ON dae.id_acao = aca.id_acao
                 AND dae.id_exercicio=apr.id_exercicio
            INNER JOIN planacomorc.localizador_programatica lpr
              USING (id_acao_programatica)
            LEFT JOIN planacomorc.quantitativo_sof qsf
              ON (qsf.loccod = lpr.loccod
                  AND qsf.id_acao_programatica = apr.id_acao_programatica)
            JOIN planacomorc.programa pro
              ON pro.id_programa = aca.id_programa
            JOIN planacomorc.orgao org
              ON org.id_orgao = apr.id_orgao
            LEFT JOIN planacomorc.acompanhamento_acao aac
              ON (aac.id_localizador = lpr.id_localizador
                  AND aac.id_acao_programatica = lpr.id_acao_programatica)
            LEFT JOIN workflow.documento doc
              ON(doc.docid = aac.docid)
            LEFT JOIN workflow.estadodocumento esd
              USING(esdid)
          WHERE qsf.quantidade_fisico != 0
            AND dae.id_produto IS NOT NULL
            AND apr.id_exercicio = {$_SESSION['exercicio']}
            AND aac.id_periodo_referencia = {$periodoSelecionado}
            {$filtroDiligenciadas}
            {$wh}
) AS foo
  GROUP BY esddsc, esdid
DML;
            if($periodoSelecionado)
                $dados = $db->carregar($sql);
            if($dados)
                echo geraGrafico($dados, $nomeUnico = 'acoes', $titulo = 'Acompanhamento de Ações', $formatoDica = "", $formatoValores = "{point.y} ({point.percentage:.2f} %)", $nomeSerie = "", $mostrapopudetalhes = true, $caminhopopupdetalhes = "", $largurapopupdetalhes = "", $alturapopupdetalhes = "", $mostrarLegenda = true, $aLegendaConfig = false, $legendaClique = false);
            else
                echo "<section class=\"alert alert-danger text-center\">Não existem dados a serem apresentados</section>";
            ?>
            </section>
    	</section>
    </section>
</section>