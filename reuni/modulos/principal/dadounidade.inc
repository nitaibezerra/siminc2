<?php

// monta cabeçalho


include APPRAIZ . 'includes/cabecalho.inc';
include APPRAIZ . 'includes/workflow.php';
include APPRAIZ . 'reuni/www/funcoes.php';
print '<br/>';
$db->cria_aba( $abacod_tela, $url, '' );
monta_titulo( $titulo_modulo, '&nbsp;' );
?>



<script type="text/javascript">
</script>
<script language="JavaScript" src="../includes/tiny_mce.js"></script>
<script language="JavaScript">
//Editor de textos
tinyMCE.init({
	mode : "textareas",
	theme : "advanced",
	plugins : "table,save,advhr,advimage,advlink,emotions,iespell,insertdatetime,preview,zoom,flash,searchreplace,print,contextmenu,paste,directionality,fullscreen",
	theme_advanced_buttons1 :
	"undo,redo,separator,bold,italic,underline,separator,justifyleft,justifycenter,justifyright,justifyfull",
	theme_advanced_buttons2 : "",
	theme_advanced_buttons3 : "",
	theme_advanced_toolbar_location : "top",
	theme_advanced_toolbar_align : "left",
	extended_valid_elements :
	"a[name|href|target|title|onclick],img[class|src|border=0|alt|title|hspace|vspace|width|height|align|onmouseover|onmouseout|name],hr[class|width|size|noshade],font[face|size|color|style],span[class|align|style]",
	language : "pt_br",
	entity_encoding : "raw"
});
</script>

<!-- corpo da página -->



<?php

$trava = 'N';

if(reuni_possuiPerfisUnidade(array(REUNI_PERFIL_IFES_APR,REUNI_PERFIL_IFES_CAD), $_REQUEST['unidadeCod'] ))
{
	$trava = 'S';	
}

if($db->testa_superuser()){
	$trava = 'S';	
}




function insertUpdade($cod){
	global $db;
	$sql  = "select count(*) as qtd from reuni.unidadeproposta where unicod = '$cod' ";
	$dado = $db->pegaUm($sql);
	if($dado > 0)
	return true;
	else
	return false;

}


$unidade = $db->usuarioUnidadesPermitidas('reuni');
$unidadeCod = $_REQUEST['unidadeCod'];

if(in_array( $unidadeCod,$unidade)===false)
{
	print "unidade: $unidadeCod";
	print_r( $unidade );
	print serialize( (in_array($_REQUEST['unidadeCod'],$unidade) ) )	;
	die('morreu');
	echo "<script>
	alert('Acesso negado!');
	location.href = '/reuni/reuni.php?modulo=inicio&acao=C' 
	</script>";
}
else
{

	$codUnidade     = $_REQUEST['unidadeCod'];
	if(isset($_REQUEST["enviar"])){


		$dirigente 		= trim($_REQUEST["dirigente"]);
		$endereco 		= trim($_REQUEST["endereco"]);
		$caracteristica = trim($_REQUEST["caracteristica"]);
		$sumula 		= trim($_REQUEST["sumula"]);


		if(insertUpdade($codUnidade)){
			$sql = "update reuni.unidadeproposta set  unpend='$endereco', unpdir = '$dirigente' ,  unpcar = '$caracteristica', unpsum = '$sumula' where unicod = '$codUnidade'";

			$db->executar($sql);


		}
		else{

			$docId = wf_cadastrarDocumento( WF_TIPO_REUNI , "Proposta Reuni - " . nomeUnidade( $codUnidade ) );
						
			$sql = "insert into reuni.unidadeproposta (unpend,unpdir,unpcar,unpsum,prjcod,unicod,unitpocod,docid)  VALUES ('$endereco','$dirigente','$caracteristica','$sumula',1,'$codUnidade','U',$docId)";

			$db->executar($sql);


		}
		$db->commit();
		$db->sucesso('principal/cadastro','&unicod='.$codUnidade);

	}



	$sql = "select unpend,unpdir,unpcar,unpsum from reuni.unidadeproposta where unicod = '$codUnidade'";
	$dado = $db->pegaLinha($sql);

	$endereco 		= $dado['unpend'];
	$dirigente 		= $dado['unpdir'];
	$caracteristica = $dado['unpcar'];
	$sumula 		= $dado['unpsum'];


?>
<form name="formulario" method="POST">
<table  class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
<tr>
<td   class="SubTituloDireita" align="right"><b>Nome da Universidade</b></td>
<td><?=nomeUnidade($_REQUEST['unidadeCod'])?></td>
</tr>

<tr>
<td   class="SubTituloDireita" align="right"><b>Endereço</b></td>
<td><?=campo_texto('endereco','S',$trava,'','50','250','','','','','','')?></td>
</tr>


<tr>
<td   class="SubTituloDireita" align="right"><b>Dirigente</b></td>
<td><?=campo_texto('dirigente','S',$trava,'','50','100','','','','','','')?></td>
</tr>
</tr>
		<tr>
		<td colspan="2" align='right' class="SubTituloEsquerda">Responsabilidades</td>
	</tr>
<?=$db->mostra_resp($_SESSION['unicod'], 'unicod', true, 'reuni');?>
<script>
	    function envia_email(cpf)
    {
          e = "<?=$_SESSION['sisdiretorio']?>.php?modulo=sistema/geral/envia_email&acao=A&cpf="+cpf;
          window.open(e, "Envioemail","menubar=no,toolbar=no,scrollbars=yes,resizable=no,left=20,top=20,width=550,height=480");
    }
</script>
<tr>
<td   class="SubTituloDireita" align="right"><b>Característica atual da Instituição:</b><br> Perfil(geral, para a instituição incluindo pós-graduação)</td>
<td><?
	if($trava == 'S')
		echo campo_textarea( 'caracteristica', 'S', "", '',130, 20 ,"");
	else 
		echo $caracteristica;
	?>
</td>
</tr>

<tr>
<td   class="SubTituloDireita" align="right"><b>Súmula do plano:</b><br>Justificativa, conceitos e fundamentos</td>
<td><?
	if($trava == 'S')		
		echo campo_textarea( 'sumula', 'S', "", '',130, 20 ,"");
	else 
		echo $sumula;
	?>
	</td>
<tr bgcolor="#cococo">
<td class="" align="right">
</td>
<td class=""  align="left">
<input class="botao" type="button" value="Voltar" onclick="history.back();">
<?if($trava == 'S'){?>
	<input class="botao" type="submit" name="enviar" value="Gravar">
<? } ?>
</td>
</tr>
</table>
</form>
<?php }?>
