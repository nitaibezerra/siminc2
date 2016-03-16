<?php
if($_REQUEST['requisicaoAjax'])
{
	$_REQUEST['requisicaoAjax']();
	die;
}

header("Cache-Control: no-cache, must-revalidate");
header("Expires: Mon, 26 Jul 1997 05:00:00 GMT");
$habilitado = true;
$tpeid = $_GET['tpeid'];
if(!$tpeid){
	echo "<script>alert('Parâmetros inválidos.');this.close()</script>";
}
?>
<html>
<head>
<meta http-equiv="Cache-Control" content="no-cache">
<meta http-equiv="Pragma" content="no-cache">
<meta http-equiv="Connection" content="Keep-Alive">
<meta http-equiv="Expires" content="Mon, 26 Jul 1997 05:00:00 GMT">
<title><?= $titulo ?></title>
<script type="text/javascript" src="../includes/funcoes.js"></script>
<script type="text/javascript" src="/includes/estouvivo.js"></script>
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
<link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>
<style>
.link {
	cursor: pointer
}
</style>
<script	type="text/javascript" src="../includes/JQuery/jquery-1.10.2.min.js"></script>
<script type="text/javascript">
this._closeWindows = false;
</script>
</head>
<body style="margin:10px; padding:0; background-color: #fff;">
<div>
<?php monta_titulo("Contratante","&nbsp;")?>
<form name=formulario id=formulario method=post enctype="multipart/form-data" >
	<input type="hidden" name="requisicao" id="requisicao" value=""> 
	<input type="hidden" name="classe" id="classe" value=""> 
	<input type="hidden" name="entid" id="entid" value="<?php echo $entid ?>"> 
	<table class="tabela" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" align="center">
	    <tr>
	        <td class="SubTituloDireita" width="25%" >
	        	Pesquisa:
	        </td>
	        <td>
	        	<?php 
	        	$arrAtributos = false;
				$arrAtributos['name'] = "pesquisa";
				$arrAtributos['id'] = "pesquisa";
				$arrAtributos['obrigatorio'] = false;
				$arrAtributos['habilitado'] = $habilitado;
				$arrAtributos['size'] = 40;
				$arrAtributos['maxsize'] = 200;
				$arrAtributos['align'] = "left";
				$arrAtributos['value'] = $_POST['pesquisa'];
				echo campo_texto($arrAtributos)
	        	?>
	        	<input type="submit" value="Buscar" name="btn_buscar" />
	        	<?php if( $_POST['pesquisa']): ?>
	        		<input type="button" value="Ver Todos" name="btn_todos" onclick="window.location=window.location" />
	        	<?php endif; ?>
	        </td>
	    </tr>
	</table>
</form>
</div>
<?php 
if($_POST['pesquisa']){
	$arrWhere[] = "pejcnpj ilike ('%{$_POST['pesquisa']}%')";
	$arrWhere[] = "removeacento(pejrazaosocial) ilike removeacento(('%{$_POST['pesquisa']}%'))";
}
$sql = "select
			'<input type=\"radio\" name=\"entidcontratante\" value=\"' || entid || '\" onclick=\"selecionaContratante(\'' || entid || '\')\"  >' as acao,
			'<span id=\"pejcnpj_' || entid || '\" >' || pejcnpj || '</span>' as pejcnpj,
			'<span id=\"pejrazaosocial_' || entid || '\" >' || pejrazaosocial || '</span>' as pejrazaosocial
		from
			corporativo.pessoajuridica
		where
			pejstatus = 'A'
		".($arrWhere ? " and (".implode(" or ",$arrWhere).")" : "");
$arrCab = array("Ação","CNPJ","Razão Social");
$db->monta_lista($sql,$arrCab,30,10,"N","center");
?>
<table class="tabela" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" align="center">
    <tr>
        <td align="center">
        	<input type="button" value="Fechar" name="btn_fechar" onclick="window.close()" />
        </td>
	</tr>
</table>
<script type="text/javascript">
function selecionaContratante(entid)
{
	var pejcnpj = $("#pejcnpj_"+entid).html();
	var pejrazaosocial = $("#pejrazaosocial_"+entid).html();
	/*if(window.opener.document.getElementById("entidcontratante").value != entid){
		$.ajax({
		   type: "POST",
		   url: window.location,
		   data: "requisicaoAjax=FiltraEmpenhoPorCNPJ&cnpj="+pejcnpj,
		   success: function(msg){
			   window.opener.document.getElementById("tr_empenho_contrato").innerHTML = msg;
		   }
		 });
	}*/
	window.opener.document.getElementById("entnomecontratante").innerHTML = pejrazaosocial;
	window.opener.document.getElementById("entcnpjcontratante").innerHTML = mascaraglobal('##.###.###/####-##',pejcnpj);
	window.opener.document.getElementById("entidcontratante").value = entid;
	/*if(pejcnpj == "15126437000143"){
		window.opener.document.getElementById("tr_diretoria_ebserh").style.display = "";
	}else{
		window.opener.document.getElementById("tr_diretoria_ebserh").style.display = "none";
	}*/
	window.close();
}
</script>
</body>
</html>
