<?php

include APPRAIZ."fabrica/classes/ControleWorkFlow.class.inc";

$ctwid = $_REQUEST['ctwid'];

if($_REQUEST['requisicao']){
	$controleWorkFlow = new ControleWorkFlow($ctwid);
	$controleWorkFlow->popularDadosObjeto($_REQUEST);
	
	if($_POST['ctwid'] && $_POST['ctwid'] != "")
		$controleWorkFlow->$_REQUEST['requisicao']();
	else
		$ctwid = $controleWorkFlow->$_REQUEST['requisicao']();
	
	$controleWorkFlow->commit();
	
	$msg = "Operação Realizada com Sucesso!";
}

if($ctwid){
	$controleWorkFlow = new ControleWorkFlow($ctwid);
	$arrDados = $controleWorkFlow->getDados();
	extract($arrDados);
}

// monta cabeçalho 
include APPRAIZ . 'includes/cabecalho.inc';
print '<br/>';

// carregando o menu
$menu[0] = array("descricao" => "Listar Controles do WorkFlow", "link"=> "fabrica.php?modulo=sistema/geral/listarControleWorkflow&acao=A");
$menu[1] = array("descricao" => "Cadastrar Controles do WorkFlow", "link"=> "fabrica.php?modulo=sistema/geral/cadControleWorkflow&acao=A");
$menu[2] = array("descricao" => "Executar Script de Controles do WorkFlow", "link"=> "fabrica.php?modulo=sistema/geral/controleWorkflow&acao=A");
echo montarAbasArray($menu, "fabrica.php?modulo=sistema/geral/cadControleWorkflow&acao=A");

$titulo = "Cadastro de Controle de WorkFlow";
monta_titulo( $titulo, obrigatorio().'Indica campo obrigatório.' );

?>
<script language="javascript" type="text/javascript" src="../includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-1.4.2.min.js"></script>
<script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/dateFunctions.js"></script>
<script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>
<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>
<style>
	.TituloTabela{background-color: #C5C5C5;font-weight:bold}
	.center{text-align:center}
	.link{cursor: pointer;}
	.middle{vertical-align: middle;}
</style>
<script>
	
	
	function limparCampos()
	{
		$('input:not(:hidden,:button,[readonly=""],[readonly="readonly"]),textarea,select').val("");
	}
	
	function salvarControleWorkFlow()
	{
		if(validaFormulario()){
			$('#form_cad_controle_work_flow').submit();
		}
	}
	
	function validaFormulario()
	{
		var erro = 0;
		$("[class~=obrigatorio]").each(function() { 
			if(!this.value){
				erro = 1;
				alert('Favor preencher todos os campos obrigatórios!');
				this.focus();
				return false;
			}
		});
		
		if($("[name=ctwtempoexec]").val() && !$("[name=ctwfuncao]").val()){ 
			erro = 1;
			alert('Favor informar o nome da Função!');
			this.focus();
			return false;
		}
		
		if(erro == 0){
			return true;
		}
	}
		
	function carregando()
	{
		return "<center><img src=\"../imagens/wait.gif\" class=\"middle\" />Aguarde...Carregando!</center>";
	}
	
</script>
<form name="form_cad_controle_work_flow" id="form_cad_controle_work_flow" method="post" action="" >
	<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
		<tr>
			<td width="25%" class="SubtituloDireita">Estado do WorkFlow:</td>
			<td>
				<?php $sql = "	select
									esd.esdid as codigo,
									tpd.tpddsc || ' - ' || esd.esddsc as descricao
								from
									workflow.estadodocumento esd
								inner join
									workflow.tipodocumento tpd ON tpd.tpdid = esd.tpdid
								where
									esd.esdstatus = 'A'
								and
									esd.esdid not in (select distinct esdid from fabrica.controleworkflow where ctwstatus = 'A' ".($ctwid ? " and ctwid != $ctwid " : "").")
								and
									sisid = ".$_SESSION['sisid']."
								order by
									tpd.tpddsc,
									esd.esddsc";?>
				<?=$db->monta_combo("esdid",$sql,"S","Selecione...","","","","","S") ?>
				<input type="hidden" name="requisicao" value="salvar" />
				<input type="hidden" name="ctwid" value="<?=$ctwid?>" />
			</td>
		</tr>
		<tr>
			<td class="SubtituloDireita">Tempo de Resposta (em horas):</td>
			<td><?=campo_texto("ctwtemporesp","S","S","Tempo de Resposta","20","20","[.###]","") ?></td>
		</tr>
		<tr>
			<td class="SubtituloDireita">Tempo mínimo para execução da função (em horas):</td>
			<td><?=campo_texto("ctwtempoexec","N","S","Tempo para execução da função","20","20","[.###]","") ?></td>
		</tr>
		<tr>
			<td class="SubtituloDireita">Nome da Função:</td>
			<td><?=campo_texto("ctwfuncao","N","S","Nome da Função","60","60","","") ?></td>
		</tr>
		<tr>
			<td colspan="2" class="TituloTabela center">
				<input type="button" onclick="salvarControleWorkFlow()" value="Salvar" >
				<input type="button" onclick="limparCampos()" value="Limpar Campos" >
			</td>
		</tr>
	</table>
</form>
<?php if($msg): ?>
<script>
	alert('<?php echo $msg ?>');
</script>
<?php endif; ?>