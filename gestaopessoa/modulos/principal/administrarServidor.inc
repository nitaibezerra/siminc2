<?php

function controleAtivo($dados) {
	global $db;
	$sql = "UPDATE gestaopessoa.servidor SET serstatus=".$dados['status']." WHERE sercpf='".$dados['cpf']."' AND seranoreferencia='".$_SESSION['exercicio']."'";
	$db->executar($sql);
	$db->commit();
}

function controleEquipe($dados) {
	global $db;
	$sql = "UPDATE gestaopessoa.servidor SET sercomequipe=".$dados['status']." WHERE sercpf='".$dados['cpf']."' AND seranoreferencia='".$_SESSION['exercicio']."'";
	$db->executar($sql);
	$db->commit();
	
	$wherenovo = str_replace("\\","",$dados['wheresql']);
	
	$sql = "SELECT '<center><input type=checkbox name=ativo value='|| sercpf ||' onclick=controleativo(this); '|| CASE WHEN serstatus=TRUE THEN 'checked' ELSE '' END ||'><center>' as chk, 
			   sercpf, 
			   sernome, 
			   sercargo,
			   '<center><input type=checkbox name=equipe value='|| sercpf ||' onclick=controleequipe(this); '|| CASE WHEN sercomequipe=TRUE THEN 'checked' ELSE '' END ||'><center>' || CASE WHEN sercomequipe=TRUE THEN 'Com equipe' ELSE 'Sem equipe' END as equipe
		FROM gestaopessoa.servidor 
		".$wherenovo." 
		ORDER BY sernome";

	$cabecalho = array("&nbsp;","CPF","Nome do Servidor","Cargo","Equipe");
	$db->monta_lista($sql,$cabecalho,100,5,'N','center',$par2);
}

if($_REQUEST['requisicao']) {
	$_REQUEST['requisicao']($_REQUEST);
	exit;
}
  
include  APPRAIZ."includes/cabecalho.inc"; 
echo '<br>';

monta_titulo( 'Administrar Servidores', 'Ativar/Desativar servidores' );

?>
<script language="javascript" type="text/javascript" src="../includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-1.4.2.min.js"></script>
<script>

function controleativo(obj) {
	
	jQuery.ajax({
   		type: "POST",
   		url: "gestaopessoa.php?modulo=principal/administrarServidor&acao=A",
   		data: "requisicao=controleAtivo&status="+obj.checked+"&cpf="+obj.value,
   		async: false,
   		success: function(msg){}
 		});
 		
}

function controleequipe(obj) {

	where = $("[name='wh']").val();

	jQuery.ajax({
   		type: "POST",
   		url: "gestaopessoa.php?modulo=principal/administrarServidor&acao=A",
   		data: "requisicao=controleEquipe&status="+obj.checked+"&cpf="+obj.value+"&wheresql="+where,
   		async: false,
   		success: function(msg){
   			$("#listaEquipe").html(msg);
   		}
 		});
 		
}

</script>

<?php
extract($_POST);

$where[] = "seranoreferencia='".$_SESSION['exercicio']."'";

if($sercpf) $where[] = "sercpf='".str_replace(array(".","-"),array("",""),$sercpf)."'";
if($sernome) $where[] = "removeacento(sernome) ilike '%'||removeacento('".$sernome."')||'%'";
if($sercargo) $where[] = "sercargo='".$sercargo."'";

$w = ($where)?"WHERE ".implode(" AND ", $where):"";

?>

<form method="post" id="formpesquisa" name="formpesquisa">
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
<input type="hidden" name="wh" id="wh"  value="<?=$w ?>">
<tr>
	<td class="SubTituloCentro" colspan="2">Argumentos de Pesquisa</td>
</tr>
<tr>
	<td class="SubTituloDireita">CPF:</td>
	<td><? echo campo_texto('sercpf', "N", "S", "CPF", 14, 11, "", "", '', '', 0, '' ); ?></td>
</tr>
<tr>
	<td class="SubTituloDireita">Nome:</td>
	<td><? echo campo_texto('sernome', "N", "S", "Nome do servidor", 30, 60, "", "", '', '', 0, '' ); ?></td>
</tr>
<tr>
	<td class="SubTituloDireita">Cargo:</td>
	<td><?
	
	$sql = "SELECT trim(sercargo) as codigo, trim(sercargo) as descricao 
			FROM gestaopessoa.servidor 
			WHERE trim(sercargo)!=''
		    GROUP BY trim(sercargo) ORDER BY trim(sercargo)";
	
	$db->monta_combo('sercargo', $sql, 'S', "Selecione...", '', '', '', '300', 'N', 'sercargo'); 
	
	?>
	</td>
</tr>
<tr>
	<td class="SubTituloCentro" colspan="2"><input type="submit" value="Pesquisar"> <input type="button" value="Ver todos" onclick="window.location='gestaopessoa.php?modulo=principal/administrarServidor&acao=A';"></td>
</tr>
</table>
</form>
<div id="listaEquipe">
<?

$sql = "SELECT '<center><input type=checkbox name=ativo value='|| sercpf ||' onclick=controleativo(this); '|| CASE WHEN serstatus=TRUE THEN 'checked' ELSE '' END ||'><center>' as chk, 
			   sercpf, 
			   sernome, 
			   sercargo,
			   '<center><input type=checkbox name=equipe value='|| sercpf ||' onclick=controleequipe(this); '|| CASE WHEN sercomequipe=TRUE THEN 'checked' ELSE '' END ||'><center>' || CASE WHEN sercomequipe=TRUE THEN 'Com equipe' ELSE 'Sem equipe' END as equipe
		FROM gestaopessoa.servidor 
		".$w." 
		ORDER BY sernome";

$cabecalho = array("&nbsp;","CPF","Nome do Servidor","Cargo","Equipe");
$db->monta_lista($sql,$cabecalho,100,5,'N','center',$par2);


?>
</div>