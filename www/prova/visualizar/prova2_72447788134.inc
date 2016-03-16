<?php
include  APPRAIZ."includes/cabecalho.inc";
echo'<br>';
monta_titulo( "Prova SIMEC", 'Questão 02 - Crie uma tela de cadastro de novas turmas' );

/*
 * DICAS:
 * 
 * Com a função "executar" você executa uma query, por exemplo para inseri-la no banco. Ex:
 * 
 * 		$sql = "INSERT INTO data (dia, mes, ano) VALUES ('24','03','1984')";
 * 		$db->executar($sql); 
 * 		$db->commit(); 
 * 
 */
echo $_POST['requisicao'];

if( $_POST['requisicao'] == 'Salvar' ){
	global $db;

	$dtaAbertura = $_POST['turdtabr'];
	$dia = substr($dtaAbertura,0,2);	
	$mes = substr($dtaAbertura,3,2);
	$ano = substr($dtaAbertura,6,4);
	
	$dtaFechamento = $_POST['turdtfec'];
	$diaa = substr($dtaFechamento,0,2);	
	$mesa = substr($dtaFechamento,3,2);
	$anoa = substr($dtaFechamento,6,4);
	
	echo $dtaAbertura = $ano.'-'.$mes.'-'.$dia.' '.date("H:i:s");
	echo $dtaFechamento = $anoa.'-'.$mesa.'-'.$diaa.' '.date("H:i:s");
	
	$sql = "INSERT INTO prova.turma (turnome, turdtabr, turdtfec) VALUES ('".$_POST['turnome']."','".$dtaAbertura."','".$dtaFechamento."')";
	$db->executar($sql);
	$db->commit();
	
	echo '<script>alert("Dados salvos com sucesso!"); window.location.href="prova.php?modulo=principal/prova2&acao=A";</script>';
	exit;
}

?>
<script language="javascript" type="text/javascript" src="../includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-1.4.2.min.js"></script>
<script type="text/javascript">
	function navega(questao){
		location.href = "/prova/prova.php?modulo=principal/prova"+questao+"&acao=A";
	}
	function excluir(){
		alert("Tem certeza que deseja excluir a turma?");
	}
	function alterar(){
		alert("Operação realizada com sucesso!");
	}
	function validar(){
	
		if(document.getElementById('turdsc').value == ""){
			alert("Favor preencher com o nome da turma!");
			document.getElementById('turdsc').focus();
			return;
		}
		if(document.getElementById('turabr').value == ""){
			alert("Favor preencher com a data de abertura da turma!");
			document.getElementById('turabr').focus();
			return;
		}
		if(document.getElementById('turfec').value == ""){
			alert("Favor preencher com a data de fechamento da turma!");
			document.getElementById('turfec').focus();
			return;
		}
		salvar();	
	}
	function salvar(){
		document.getElementById('requisicao').value = 'Salvar';
		document.getElementById('formulario').submit();
	}
	function Formatadata(Campo, teclapres)
	{
		var tecla = teclapres.keyCode;
		var vr = new String(Campo.value);
		vr = vr.replace("/", "");
		vr = vr.replace("/", "");
		vr = vr.replace("/", "");
		tam = vr.length + 1;
		if (tecla != 8 && tecla != 8)
		{
			if (tam > 0 && tam < 2)
				Campo.value = vr.substr(0, 2) ;
			if (tam > 2 && tam < 4)
				Campo.value = vr.substr(0, 2) + '/' + vr.substr(2, 2);
			if (tam > 4 && tam < 7)
				Campo.value = vr.substr(0, 2) + '/' + vr.substr(2, 2) + '/' + vr.substr(4, 7);
		}
	}
	
</script>
<form name="formulario" id="formulario" method="post" action="prova.php?modulo=principal/prova2&acao=A" >
	<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
	<input type="hidden" id="requisicao" name="requisicao" value="salvar">
		<colgroup>
			<col width="25%" />
			<col width="75%" />
		</colgroup>
		<tr>
			<td class="SubTituloDireita">Nome da Turma:</td>
			<td><input type="text" name="turnome" id="turdsc" size="50"></td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Data de abertura da Turma:</td>
			<td><input type="text" name="turdtabr" id="turabr" size="50" maxlength="10" onkeyup="Formatadata(this,event)"></td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Data de fechamento da Turma:</td>
			<td><input type="text" name="turdtfec" id="turfec" size="50" maxlength="10" onkeyup="Formatadata(this,event)"></td>
		</tr>
	</table>
</form>
<table align ="center">
<tr>
		<td align="center" colspan="2"><strong>Lista de Turmas</strong></td>
</tr>
</table>
<?
$sql = "SELECT 
			'<img title=\"Alterar\" src=\"../imagens/alterar.gif\" onClick=\"javascript: alterar()\" />&nbsp;<img title=\"Excluir\" src=\"../imagens/excluir.gif\" onClick=\"javascript: excluir()\" />' as acao, 
			turnome, 
			TO_CHAR(turdtabr,'dd/mm/YYYY') as tur_dt_abr, 
			TO_CHAR(turdtfec,'dd/mm/YYYY') as tur_dt_fec 
		FROM 
			prova.turma
		ORDER BY
			turnome";

$cabecalho = array("Ações","Nome da Turma","Data de abertura da Turma","Data de fechamento da Turma");
$db->monta_lista($sql,$cabecalho,50,5,'N','100%');
?>

<table cellspacing="1" cellpadding="3" bgcolor="#f5f5f5" align="center" width="100%" class="tabela">
	<tbody>
		<tr bgcolor="#cccccc">
			<td>
				<div style="float:left; width:30%; text-align: left;">
					<input value="Questão anterior" type="button" onclick="navega(1)" id="anterior">
				</div>
				<div style="float:left; width:30%; text-align: center">
					<input value="Salvar" type="button" id="salvar"  onclick="validar()">
				</div>
				<div style="float:right; width:30%; text-align: right;">
					<input disabled="disabled" value="Próxima Questão" type="button" id="proximo">
				</div>
			</td>
		</tr>
	</tbody>
</table>