<?php
/*
   Sistema Simec
   Setor responsável: SPO-MEC
   Desenvolvedor: Equipe Simec
   Analista: Vitor
   Programador: Wescley Guedes
   Módulo:cadastrofontefin.inc
   Finalidade: permitir o cadastro de Fontes de Fianciamento
*/  

if ($_REQUEST['insereFonteFin'])
{
	$sql = "SELECT * FROM assint.fontefinanciamento 
	WHERE fofdesc = '".$_REQUEST[fonteFinInsere]."'";
	$verifica = $db->pegaUm($sql, 2);	
	
	if($verifica != 'A')
	{
		$sql = "INSERT INTO assint.fontefinanciamento (fofdesc, fofstatus) VALUES ('".$_REQUEST[fonteFinInsere]."','A')";
		$db->executar($sql);
		$db->commit();		
		alert("Fonte de Fianciamento cadastrada com Sucesso!");
		echo "<script>document.location.href='assint.php?modulo=sistema/tabelaapoio/cadastrofontefin&acao=A'</script>";
		
	} else {
		alert("A Fonte de Financiamento já existe! Por favor cadastre outra.");
		echo "<script>document.location.href='assint.php?modulo=sistema/tabelaapoio/cadastrofontefin&acao=A'</script>";
	}
}

if ($_REQUEST['removeFonteFin'])
{
	$where = "WHERE fofid = '".$_REQUEST['fofid']."'";
	
	$sql = "SELECT * FROM assint.beneficiariofontefin $where";
	$beneficiarioFontefin = $db->pegaUm($sql, 2);
	
	$sql = "SELECT * FROM assint.programafontefin $where";
	$programaFontefin = $db->pegaUm($sql, 2);
	
	$sql = "SELECT * FROM assint.acordofontefin $where";
	$acordoFontefin = $db->pegaUm($sql, 2);
	
	if($beneficiarioFontefin || $programaFontefin || $acordoFontefin) {
			
		//$sql = "UPDATE assint.fontefinanciamento SET fofstatus = 'I' $where";
		//$db->executar($sql);
		//$db->commit();
		alert("Fonte de Financiamento não pode ser removida! Existem vinculos.");
		echo "<script>document.location.href='assint.php?modulo=sistema/tabelaapoio/cadastrofontefin&acao=A'</script>";
	} else {
		
		$sql = "DELETE FROM assint.fontefinanciamento $where";
		$db->executar($sql);
		$db->commit();
		alert("Fonte de Financiamento removida com sucesso!");
		echo "<script>document.location.href='assint.php?modulo=sistema/tabelaapoio/cadastrofontefin&acao=A'</script>";
	}
}

if ($_REQUEST['pegaFonteFin'])
{
	$sql = "SELECT * FROM assint.fontefinanciamento WHERE fofid = '".$_REQUEST['fofid']."'";
	$fonteFin = $db->carregar($sql);		
}

if ($_REQUEST['alteraFonteFin'])
{
	$sql = "UPDATE assint.fontefinanciamento SET fofdesc = '".$_REQUEST['fofdesc']."' WHERE fofid = '".$_REQUEST['fofid']."'";
	$fonteFin = $db->executar($sql);
	alert('Registro atualizado com sucesso!');
	echo "<script>document.location.href='assint.php?modulo=sistema/tabelaapoio/cadastrofontefin&acao=A'</script>";		
}

include  APPRAIZ."includes/cabecalho.inc";

echo "<br>";

monta_titulo( "Fontes de Financiamento", "");
?>
<script language="JavaScript" src="../includes/wz_tooltip.js"></script>
<form method="POST"  name="formulario" action="">
	<table  class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
		<tr>
			<td align='right' class="SubTituloDireita">Fonte de Financiamento:</td>
		    <td><?php echo campo_texto('fontefin','N','S','',100,255,'','','','','','id="fontefin"','',$fonteFin[0]['fofdesc']); ?></td>
		</tr>
		<tr bgcolor="#cccccc">
			<td></td>
			<td>
				<input type="button" class="botao" name="bta" value="Pesquisar" onclick="submeter();">
				<?php if(!$_REQUEST['pegaFonteFin']) { ?>
				<input type="button" class="botao" name="bt" value="Cadastrar" onclick="insereFonteFin();">
				<?php } else { ?>
				<input type="button" class="botao" name="bt" value="Alterar" onclick="alteraFonteFin(<?=$fonteFin[0]['fofid'];?>);">
				<input type="button" class="botao" name="bt" value="Novo" onclick="novoRegistro();">
				<?php } ?>
			</td>
		</tr>
	</table>
</form>
<?php

if(!empty($_REQUEST['fontefin'])){
	$where = " WHERE ff.fofstatus = 'A' and upper(ff.fofdesc) like upper('%$_REQUEST[fontefin]%') ";
} else {
	$where = "where ff.fofstatus = 'A'";
}
$sql = "SELECT '<center>
			<a style=\"cursor:pointer;\" onclick=\"pegaFonteFin(\''||ff.fofid||'\');\">
			<img src=\"/imagens/alterar.gif \" border=0 title=\"Alterar\">
			</a>
			<a  style=\"cursor:pointer;\" onclick=\"removerFonteFin(\''||ff.fofid||'\');\">
			<img src=\"/imagens/excluir.gif \" border=0 title=\"Excluir\">
			</a>
			</center>' as acao, 
			ff.fofdesc
		FROM assint.fontefinanciamento ff 
		$where
		ORDER BY ff.fofdesc ASC";
		
$cabecalho = array("","Título");
$db->monta_lista($sql,$cabecalho,100,5,'N','95%',$par2);
	
?>
<script type="text/javascript">
function submeter(){
	document.formulario.submit();	
}

function insereFonteFin(){
	var fontefin = document.getElementById('fontefin').value;
	
	if(fontefin == '')
	{	
		alert("O campo FONTE DE FINANCIAMENTO é obrigatório!");
		fontefin.focus();
	} else {	
		document.location.href='assint.php?modulo=sistema/tabelaapoio/cadastrofontefin&acao=A&insereFonteFin=true&fonteFinInsere='+fontefin;
	}
}

function removerFonteFin(fofid){
	var conf = confirm("Você realmente deseja excluir esta Fonte de Financiamento?");	
	if(conf) {
		location.href="assint.php?modulo=sistema/tabelaapoio/cadastrofontefin&acao=A&removeFonteFin=true&fofid="+fofid;	
	}
}

function pegaFonteFin(fofid){
	document.location.href='assint.php?modulo=sistema/tabelaapoio/cadastrofontefin&acao=A&pegaFonteFin=true&fofid='+fofid;	
}

function alteraFonteFin(fofid){
	var fontefin = document.getElementById('fontefin').value;
	document.location.href='assint.php?modulo=sistema/tabelaapoio/cadastrofontefin&acao=A&alteraFonteFin=true&fofid='+fofid+'&fofdesc='+fontefin;	
}

function novoRegistro(){
	document.location.href='assint.php?modulo=sistema/tabelaapoio/cadastrofontefin&acao=A';
}
</script>