<?php
/**
 * $Id: centralacompanhamento.inc 92099 2014-12-19 12:42:00Z lindalbertofilho $
 */
/**
 * Funções de manipulação de ações.
 * 
 * @see _funcoesacoes.php
 */
include_once '_funcoessubacoes.php';
require APPRAIZ . 'includes/library/simec/Listagem.php';

/* Funções de Gráficos do Cockpit */
include_once '../../pde/www/_funcoes_cockpit.php';

if (! isset ( $_REQUEST ['percod'] )) {    
    /* Por para funcionar */    
    $periodoSelecionado = recuperaUltimoPeriodoReferencia();
} else {
    $periodoSelecionado = $_REQUEST ['percod'];
}

if ($_REQUEST ['requisicao']) {
    $_REQUEST ['requisicao'] ( $_REQUEST );
    exit ();
}
// Chamada de programa
include APPRAIZ . "includes/cabecalho.inc";
$perfis = pegaPerfilGeral();

if (1 != $_SESSION ['superuser']) {
	if (in_array ( PFL_VALIDADORACAO, $perfis ) || in_array ( PFL_VALIDADOR_SUBSTITUTO, $perfis )) {
		die ( "<script>window.location='planacomorc.php?modulo=centralacompanhamento&acao=A';</script>" );
	} elseif (in_array ( PFL_COORDENADORACAO, $perfis ) || in_array ( PFL_CONSULTA, $perfis )) {
		die ( "<script>window.location='planacomorc.php?modulo=principal/acoes/listaunidades&acao=A';</script>" );
	} elseif (in_array ( PFL_COORDENADORSUBACAO, $perfis )) {
		die ( "<script>window.location='planacomorc.php?modulo=principal/subacoes/listasubacoes&acao=A';</script>" );
	}
}
?>
<style>
    .marcado {background-color: #C1FFC1 !important;}
    .remover {display: none;}
</style>
<script>
//Função que executa a busca textual - by Raj
    $(document).ready(function()
    {
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

    function gerenciarSubacao(id_subacao) {
        window.location = 'planacomorc.php?modulo=principal/subacoes/listasubacoes&acao=A&requisicao=carregarSubacao&id_subacao=' + id_subacao;
    }

    function ajaxatualizar(params, iddestinatario) {
        jQuery.ajax({
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

        var filtro = $("#ckh_filtro_diligenciados").attr('checked');
        window.location = "planacomorc.php?modulo=centralacompanhamento&acao=A&diligenciadas=" + filtro;
    }
    function busca() {
        document.form.submit();
    }
</script>
<script language="javascript" src="../includes/Highcharts-3.0.0/js/highcharts.js"></script>
<script language="javascript" src="../includes/Highcharts-3.0.0/js/modules/exporting.js"></script>

<section class="col-md-12">
    <ol class="breadcrumb">
        <li><a href="/planacomorc/planacomorc.php?modulo=inicio&acao=C"><?=$_SESSION['sisdsc']; ?></a></li>
        <li class="">Subações <?= $_SESSION['exercicio']; ?></li>
        <li class="active">Central de Acompanhamento</li>
    </ol>
</section>
<section class="col-md-12">
    <section class="well">
        <form class="form-horizontal" id="form" name="form" method="POST">
            <section class="form-group">			
                <label class="control-label col-md-2" for="percod">Período de referência: </label>
                <?php 
                $sql = "
                    SELECT
                        id_periodo_referencia AS codigo,
                        titulo || ' : ' || to_char(inicio_validade,'DD/MM/YYYY') ||' a ' || to_char(fim_validade,'DD/MM/YYYY') as descricao
                    FROM planacomorc.periodo_referencia p
                    WHERE id_exercicio = '" . $_SESSION ['exercicio'] . "'
                    ORDER BY id_periodo_referencia desc
                ";
                $pers = $db->carregar($sql);
                $pers = $pers ? $pers : array();
                ?>
                <section class="col-md-10">
                <?php inputCombo('percod', $pers, $periodoSelecionado, 'percod',array('acao'=>'busca();'));?>
                </section>						
            </section>
        </form>
    </section>
    <section class="col-md-7">
    <?php								
    if ($pers[0]){
        if(count( $pers ) == 1)
            $periodoreferencia = current($pers);
    ?>									
        <label for="textFind" style="margin-bottom: 3px">Pesquisa rápida: </label>
        <input class="normal form-control" type="text" id="textFind"></input>
        <br>
    <?php 
        listaSubacoes_bootstrap(array('id_periodo_referencia' => $periodoSelecionado)); 
    } else {
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
                SELECT
                    COALESCE(COALESCE(esd2.esddsc,esd.esddsc), 'Não Iniciado') AS descricao,
                    COUNT(DISTINCT s.id_subacao)         AS valor
                FROM planacomorc.snapshot_dotacao_subacao apr
                INNER JOIN planacomorc.subacao s USING (id_subacao) LEFT JOIN planacomorc.acompanhamento_subacao aco ON
                        (apr.id_subacao = aco.id_subacao AND aco.id_periodo_referencia = {$periodoSelecionado} AND apr.id_ptres = aco.id_ptres )
                LEFT JOIN workflow.documento doc ON doc.docid = aco.docid 
                LEFT JOIN workflow.estadodocumento esd ON esd.esdid = doc.esdid
                -- gato para o novo
                LEFT JOIN planacomorc.acompanhamento_subacao_tramitacao ast ON ast.id_subacao = s.id_subacao AND ast.id_periodo_referencia = 5
                LEFT JOIN workflow.documento doc2 ON ast.docid = doc2.docid
                LEFT JOIN workflow.estadodocumento esd2 ON esd2.esdid = doc2.esdid
                --
                WHERE s.st_ativo='A'
                    AND s.id_exercicio= {$_SESSION['exercicio']}
                GROUP BY esd.esddsc,esd2.esddsc
DML;
            
            if($periodoSelecionado)
                $dados = $db->carregar($sql);
            if ($dados) {
                echo geraGrafico ( $dados, $nomeUnico = 'acoes', $titulo = 'Acompanhamento de Subações', $formatoDica = "", $formatoValores = "{point.y} ({point.percentage:.2f} %)", $nomeSerie = "", $mostrapopudetalhes = true, $caminhopopupdetalhes = "", $largurapopupdetalhes = "", $alturapopupdetalhes = "", $mostrarLegenda = true, $aLegendaConfig = false, $legendaClique = false );
            }else{
                echo "<section class=\"alert alert-danger text-center\">Não existem dados a serem apresentados</section>";
            }				
            ?>

            </section>
        </section>		   
    </section>
</section>
