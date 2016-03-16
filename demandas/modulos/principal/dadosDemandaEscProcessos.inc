<?php
include_once APPRAIZ . "includes/classes/questionario/Tela.class.inc";
include_once APPRAIZ . "includes/classes/questionario/GerenciaQuestionario.class.inc";
header('Content-type: text/html; charset="iso-8859-1"',true);
header("Cache-Control: no-store, no-cache, must-revalidate");// HTTP/1.1
header("Cache-Control: post-check=0, pre-check=0", false);
header("Pragma: no-cache");// HTTP/1.0 Canhe Livre

$qrpid = pegaQrpid( DEMANDA_QUESTIONARIO, $_SESSION['dmdid'] );

include_once APPRAIZ . 'includes/workflow.php';
include APPRAIZ ."includes/cabecalho.inc";
print '<br>';

$db->cria_aba( $abacod_tela, $url, '');

monta_titulo( 'Solicitacao de Demanda - Formulario','' );
?>
<script src="../fabrica/js/autocomplete/jquery-1.7.2.js"></script>
<script type="text/javascript">
jQuery.noConflict();
jQuery(document).ready(function(){
<?php 
    $estadoAtualDaDemanda =  wf_pegarEstadoAtual($docid);
    if( in_array($estadoAtualDaDemanda['esdid'], array(
    DEMANDA_ESTADO_EM_ANALISE
    ,DEMANDA_ESTADO_EM_ATENDIMENTO
    ,DEMANDA_GENERICO_ESTADO_EM_ANALISE	       				
    ,DEMANDA_GENERICO_ESTADO_EM_ATENDIMENTO			   				
    ) ) ) { ?>
        jQuery('a[title^="Avaliação da demanda"]').removeAttr("href").css('color', '#B5B5B5').css('text-decoration', 'none');
<?php } ?>
});
</script>
<?=cabecalhoDemanda();

$dmdid = $_SESSION['dmdid']; 
$sql = 'SELECT
				od.ordid
			FROM
				demandas.demanda d
						 INNER JOIN demandas.tiposervico ts ON ts.tipid = d.tipid
						 INNER JOIN demandas.origemdemanda od ON od.ordid = ts.ordid
						 INNER JOIN demandas.demandaorigemquest quest ON quest.ordid = od.ordid
			WHERE
				quest.doqacesso = TRUE
			AND
				d.dmdid = '.$dmdid;

$origem = $db->pegaUm($sql);
if ($origem == ORIGEM_DEMANDA_ESCRITORIO_PROCESSOS){
?>

<form id="formEscProc" name="formEscProc" action="" method="post">
<table bgcolor="#f5f5f5" align="center" class="tabela" >
	<tr>
		<td>
		<fieldset style="width: 94%; background: #fff;"  >
			<legend>Questionario</legend>
			<?php
				$tela = new Tela( array("qrpid" => $qrpid, 'tamDivArvore' => 25, 'tamDivPx' => 250 ) );
			?>
		</fieldset>
	</tr>
</table>
</form>
<?php } else {?>
<form id="formEscProc" name="formEscProc" action="" method="post">
<table bgcolor="#f5f5f5" align="center" class="tabela" >
	<tr>
		<td class="SubTituloEsquerda" colspan='3'>
				<br>
			     <b>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; Formulário com informações complementares para solicitação de demandas. </b>
			     <br><br>
			</td>
	</tr>
	<tr>
			<td style="background: rgb(238, 238, 238)" colspan='3'>
				<br>
			     <b>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; ATENÇÃO! Nenhum formulário disponível para o setor de origem da demanda!</b>
			     <br><br>
			</td>
		</tr>
</table>
</form>
<?php }?>