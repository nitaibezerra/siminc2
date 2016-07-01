<?php
/*
   Sistema Simec
   Setor responsável: SPO-MEC
   Desenvolvedor: Equipe Simec
   Analista: Vitor
   Programador: Wescley Guedes
   Módulo:cadastroatividade.inc
   Finalidade: permitir o cadastro de Atividades
*/  

if ($_REQUEST['insereAtividade'])
{
	$sql = "SELECT * FROM assint.atividade 
	WHERE atidesc = '".$_REQUEST[atividadeInsere]."'";
	$verifica = $db->pegaUm($sql, 2);	
	
	if($verifica != 'A')
	{
		$sql = "INSERT INTO assint.atividade (atidesc, atistatus) VALUES ('".$_REQUEST[atividadeInsere]."','A')";
		$db->executar($sql);
		$db->commit();		
		alert("Atividade cadastrada com Sucesso!");
		echo "<script>document.location.href='assint.php?modulo=sistema/tabelaapoio/cadastroatividade&acao=A'</script>";
		
	} else {
		alert("A Atividade já existe! Por favor cadastre outra.");
		echo "<script>document.location.href='assint.php?modulo=sistema/tabelaapoio/cadastroatividade&acao=A'</script>";
	}
}

if ($_REQUEST['removeAtividade'])
{
	$where = "WHERE atiid = '".$_REQUEST['atiid']."'";
	
	$sql = "SELECT * FROM assint.acordoatividade $where";
	$acordoAtividade = $db->pegaUm($sql, 2);	
	
	if($acordoAtividade) {
			
		//$sql = "UPDATE assint.atividade SET fofstatus = 'I' $where";
		//$db->executar($sql);
		//$db->commit();
		alert("Atividade não pode ser removida! Existem vinculos.");
		echo "<script>document.location.href='assint.php?modulo=sistema/tabelaapoio/cadastroatividade&acao=A'</script>";
	} else {
		
		$sql = "DELETE FROM assint.atividade $where";
		$db->executar($sql);
		$db->commit();
		alert("Atividade removida com sucesso!");
		echo "<script>document.location.href='assint.php?modulo=sistema/tabelaapoio/cadastroatividade&acao=A'</script>";
	}
}

if ($_REQUEST['pegaAtividade'])
{
	$sql = "SELECT * FROM assint.atividade WHERE atiid = '".$_REQUEST['atiid']."'";
	$fonteFin = $db->carregar($sql);		
}

if ($_REQUEST['alteraAtividade'])
{
	$sql = "UPDATE assint.atividade SET atidesc = '".$_REQUEST['atidesc']."' WHERE atiid = '".$_REQUEST['atiid']."'";
	$fonteFin = $db->executar($sql);
	alert('Registro atualizado com sucesso!');
	echo "<script>document.location.href='assint.php?modulo=sistema/tabelaapoio/cadastroatividade&acao=A'</script>";		
}

include  APPRAIZ."includes/cabecalho.inc";

echo "<br>";

monta_titulo( "Atividades", "");
?>
<script language="JavaScript" src="../includes/wz_tooltip.js"></script>
<form method="POST"  name="formulario" action="">
	<table  class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
		<tr>
			<td align='right' class="SubTituloDireita">Atividade:</td>
		    <td><?php echo campo_texto('atividade','N','S','',100,255,'','','','','','id="atividade"','',$fonteFin[0]['atidesc']); ?></td>
		</tr>
		<tr bgcolor="#cccccc">
			<td></td>
			<td>
				<input type="button" class="botao" name="bta" value="Pesquisar" onclick="submeter();">
				<?php if(!$_REQUEST['pegaAtividade']) { ?>
				<input type="button" class="botao" name="bt" value="Cadastrar" onclick="insereAtividade();">
				<?php } else { ?>
				<input type="button" class="botao" name="bt" value="Alterar" onclick="alteraAtividade(<?=$fonteFin[0]['atiid'];?>);">
				<input type="button" class="botao" name="bt" value="Novo" onclick="novoRegistro();">
				<?php } ?>
			</td>
		</tr>
	</table>
</form>
<?php

if(!empty($_REQUEST['atividade'])){
	$where = " WHERE at.atistatus = 'A' and upper(at.atidesc) like upper('%$_REQUEST[atividade]%') ";
} else {
	$where = "where at.atistatus = 'A'";
}
$sql = "SELECT '<center>
			<a style=\"cursor:pointer;\" onclick=\"pegaAtividade(\''||at.atiid||'\');\">
			<img src=\"/imagens/alterar.gif \" border=0 title=\"Alterar\">
			</a>
			<a  style=\"cursor:pointer;\" onclick=\"removerFonteFin(\''||at.atiid||'\');\">
			<img src=\"/imagens/excluir.gif \" border=0 title=\"Excluir\">
			</a>
			</center>' as acao, 
			at.atidesc
		FROM assint.atividade at 
		$where
		ORDER BY at.atiid ASC";
		
$cabecalho = array("","Título");
$db->monta_lista($sql,$cabecalho,100,5,'N','95%',$par2);
	
?>
<script type="text/javascript">
function submeter(){
	document.formulario.submit();	
}

function insereAtividade(){
	var atividade = document.getElementById('atividade').value;
	
	if(atividade == '')
	{	
		alert("O campo ATIVIDADE é obrigatório!");
		atividade.focus();
	} else {	
		document.location.href='assint.php?modulo=sistema/tabelaapoio/cadastroatividade&acao=A&insereAtividade=true&atividadeInsere='+atividade;
	}
}

function removerFonteFin(atiid){
	var conf = confirm("Você realmente deseja excluir esta ATIVIDADE?");	
	if(conf) {
		location.href="assint.php?modulo=sistema/tabelaapoio/cadastroatividade&acao=A&removeAtividade=true&atiid="+atiid;	
	}
}

function pegaAtividade(atiid){
	document.location.href='assint.php?modulo=sistema/tabelaapoio/cadastroatividade&acao=A&pegaAtividade=true&atiid='+atiid;	
}

function alteraAtividade(atiid){
	var atividade = document.getElementById('atividade').value;
	document.location.href='assint.php?modulo=sistema/tabelaapoio/cadastroatividade&acao=A&alteraAtividade=true&atiid='+atiid+'&atidesc='+atividade;	
}

function novoRegistro(){
	document.location.href='assint.php?modulo=sistema/tabelaapoio/cadastroatividade&acao=A';
}
</script>