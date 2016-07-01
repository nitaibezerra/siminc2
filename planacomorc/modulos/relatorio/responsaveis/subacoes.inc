<?php
/**
 * Relatório de responsáveis por subações.
 * $Id: subacoes.inc 92296 2015-01-02 18:55:21Z lindalbertofilho $
 */

/**
 * Cabeçalho base do SIMEC.
 */
include APPRAIZ . 'includes/cabecalho.inc';
require APPRAIZ . 'includes/library/simec/Listagem.php';

// -- Filtro de períodos
$sqlPeriodo = <<<DML
SELECT id_periodo_referencia AS codigo,
       titulo || ' : ' || to_char(inicio_validade,'DD/MM/YYYY') ||' a ' || to_char(fim_validade,'DD/MM/YYYY') as descricao
  FROM planacomorc.periodo_referencia p
  WHERE id_exercicio = '{$_SESSION['exercicio']}'
  ORDER BY id_periodo_referencia DESC
DML;

// @todo Fazer uma função
// -- Identificando / selecionando o periodo de consulta
if(!isset($_REQUEST['id_periodo_referencia'])){
    $_REQUEST['id_periodo_referencia'] = $db->pegaUm("SELECT id_periodo_referencia FROM planacomorc.periodo_referencia ORDER BY id_periodo_referencia DESC LIMIT 1");
}
?>
<script src="./js/planacomorc.js"></script>
<script type="text/javascript" src="../includes/funcoes.js"></script>

<section class="col-md-12">
    <ol class="breadcrumb">
        <li><a href="/planacomorc/planacomorc.php?modulo=inicio&acao=C"><?=$_SESSION['sisdsc']; ?></a></li>
        <li class="">Subações <?= $_SESSION['exercicio']?></li>
        <li class="active">Relatórios - Responsáveis</li>
    </ol>
</section>
<section class="col-md-12">
    <section class="well">
        <form class="form-horizontal" method="post" id="filtrosubacao">
            <section class="form-group">
                <label class="col-md-2 control-label" for="periodo">Período: </label>
                <section class="col-md-10">
                <?php inputCombo('id_periodo_referencia', $sqlPeriodo, $_REQUEST ['id_periodo_referencia'], 'id_periodo_referencia')?>
                </section>
            </section>			
            <section class="form-group">
                <section class="col-md-offset-2">
                    <input class="btn btn-primary" type="button" name="filtrar" value="Pesquisar" onclick="document.getElementById('filtrosubacao').submit();">
                </section>
            </section>

        </form>
    </section>

<?php
$andaco =  $_REQUEST['id_periodo_referencia'] ? "AND aco.id_periodo_referencia = {$_REQUEST['id_periodo_referencia']}" : "";
$andur  =  $_REQUEST['id_periodo_referencia'] ? "AND ur.id_periodo_referencia = {$_REQUEST['id_periodo_referencia']}" : "";
$andapr =  $_REQUEST['id_periodo_referencia'] ? "AND apr.id_periodo_referencia = {$_REQUEST['id_periodo_referencia']}" : "";
$sqlRelatorio = <<<DML
SELECT DISTINCT s.codigo as codigo,
                s.titulo AS lula,
                COALESCE(u.usucpf || ' - ' || u.usunome, '<center>-</center>') AS responsavel
  FROM planacomorc.snapshot_dotacao_subacao apr
    INNER JOIN planacomorc.subacao s USING (id_subacao)
    LEFT JOIN planacomorc.acompanhamento_subacao aco
      ON (apr.id_subacao = aco.id_subacao
          {$andaco}
          AND apr.id_ptres = aco.id_ptres)
    LEFT JOIN planacomorc.usuarioresponsabilidade ur
      ON (ur.id_subacao = s.id_subacao
          AND ur.rpustatus = 'A'
          {$andur})
    LEFT JOIN seguranca.usuario u
      ON ur.usucpf = u.usucpf
  WHERE s.st_ativo = 'A'
    AND s.id_exercicio = {$_SESSION['exercicio']}
    {$andapr}
  ORDER BY s.codigo
DML;

$listagem = new Simec_Listagem();
$listagem->setQuery($sqlRelatorio);
$listagem->turnOnPesquisator();
$listagem->setTotalizador(Simec_Listagem::TOTAL_QTD_REGISTROS);
$listagem->setCabecalho(array('Subação', 'Título', 'Coordenador'));
$listagem->render(Simec_Listagem::SEM_REGISTROS_MENSAGEM);
?>
</section>