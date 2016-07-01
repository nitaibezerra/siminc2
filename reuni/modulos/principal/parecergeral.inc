<?php

// monta cabeçalho


//include APPRAIZ . 'includes/cabecalho.inc';

include APPRAIZ . 'includes/workflow.php';
include APPRAIZ . 'reuni/www/funcoes.php';
include APPRAIZ . 'includes/arquivo.inc';

print '<br/>';


//monta_titulo( $titulo_modulo, '&nbsp;' );

$unicod = base64_decode($_REQUEST['unicod']);


function montaParecer($tipoParecer,$unpid){
	global $db;
	if(!isset($unpid)) return false;
	$sql = "select sitcod , pardsc , arqid from reuni.parecer where tpacod = $tipoParecer and unpid = $unpid";
	return $db->pegaLinha($sql);
}

function montaSituacao($nome='',$readOnly=false,$codParecer=''){
	global  $db;
	$sql = "select sitdsc , sitcod from reuni.situacao";
	$dado = $db->carregar($sql);
	$saida = '';
	$check = '';

	if($readOnly == false && isset($codParecer)) {
		$sql = "select sitdsc from reuni.situacao where  sitcod = $codParecer";
		return $db->pegaUm($sql);

	}

	foreach ($dado as $row)
	{

		if($row['sitcod'] == $codParecer) $check = "checked='checked'";
		$saida .= "<input type='radio' name='situacao_$nome' $read $check value='".$row['sitcod']."'>". $row['sitdsc']	. "&nbsp;";
		$check = '';
	}
	return $saida;
}



$docid = $db->pegaUm("select docid from reuni.unidadeproposta where unicod =". $unicod);
$sql = "select unpid from reuni.unidadeproposta where unicod =" . $unicod ." and docid=".$docid;

$unpid = $db->pegaUm($sql);

if(isset($_FILES["arquivo"]) and !empty($_FILES['arquivo']['name'])){

	try{

		$codArquivo = salvarArquivo($_SESSION['usucpf'],$_SESSION['sisid'],$_FILES["arquivo"],getTiposArquivoEscritorio());
		if( isset($codArquivo) )
		{
			$sql = "update reuni.parecer set arqid = $codArquivo where  unpid = $unpid and tpacod = 7 ";
			$db->executar($sql);
			$db->commit();
		}
	}catch ( Exception $objError ){
		echo '<script> alert("'.$objError->getMessage().'")</script>';
	}
}




$sesu 			= montaParecer(4,$unpid);
$adhoc 			= montaParecer(5,$unpid);
$comissao 		= montaParecer(6,$unpid);
$parecerFinal 	= montaParecer(7,$unpid);

$leituraSesu     	= reuni_podeVerParecerSesu($unicod);
$leituraAdhoc    	= reuni_podeVerParecerAdhoc($unicod);
$leituraComissao 	= reuni_podeVerParecerComissao($unicod);
$leituraParecerFinal= reuni_podeVerParecerSesuFinal($unicod);

$faseSesu 		= reuni_podeEditarParecerSesu($unicod);
$faseAdhoc 		= reuni_podeEditarParecerAdhoc($unicod);
$faseComissao 	= reuni_podeEditarParecerComissao($unicod);
$faseFinal 		= reuni_podeEditarParecerSesuFinal($unicod);

?>
<!-- biblioteca javascript local -->
<script language="JavaScript" src="../includes/funcoes.js"></script>
		<link rel="stylesheet" type="text/css" href="/includes/Estilo.css"/>
		<link rel='stylesheet' type='text/css' href='/includes/listagem.css'/>
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
<form name="formulario" method="post" action="" enctype="multipart/form-data">
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
<?if ($faseSesu){?>
	
	<tr>
		<td class="SubTituloCentro"  colspan="2">
		<b>Parecer Geral SESU</b>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita" align="right">
			<b>Qualificação:</b>
		</td>
		<td>
			<?=  montaSituacao('Sesu',$leituraSesu,$sesu['sitcod'])?>
		</td>
	</tr>
	
	<tr>
		<td class="SubTituloDireita" align="right">
			<b>Observações:</b>
		</td>
		<td>	
			<?php
			$parecerSesu = $sesu['pardsc'];
			if($leituraSesu)
			echo campo_textarea( 'parecerSesu', 'N', '', '',130, 10,'');
			else
			echo $parecerSesu;
			?>
		</td>
		
	
		
		
		
		
			</tr>
				<tr>
				<td class="SubTituloDireita" align="right">
					
				</td>
				<td> 
					<? if($leituraSesu){?>
						<input type="button" value="Salvar" name="salvarSesu" onclick="salvarParecerGeral('<?=base64_encode($unpid)?>',4);">	&nbsp;	
					<?}?>
				</td>					
			</tr>
	
	<?}?>
			
		<?if ($faseAdhoc){?>	
		<tr>
			<td class="SubTituloCentro	" align="right" colspan="2">
				<b>Parecer Geral ADHOC</b>
			</td>
		</tr>
			<tr>
		<td class="SubTituloDireita" align="right">
			<b>Qualificação:</b>
		</td>
		<td>
			<?=  montaSituacao('Adhoc',$leituraAdhoc,$adhoc['sitcod'])?>
		</td>
	</tr>	
		<tr>
			<td class="SubTituloDireita" align="right"><b>ADHOC:</b>
			</td>
			<td>
				<?php
				$parecerAdhoc = $adhoc['pardsc'];
				if($leituraAdhoc)
				echo campo_textarea( 'parecerAdhoc', 'N', '', '',130, 10,'');
				else
				echo $parecerAdhoc;
				?>
			</td>
			
		</tr>
		<tr>
		<td class="SubTituloDireita">
		&nbsp;
		</td>
		  <td>
		  		<? if($leituraAdhoc){?>
					<input type="button" value="Salvar" onclick="salvarParecerGeral('<?=base64_encode($unpid)?>',5)">
				<?}?>
			</td>
		</tr>

	
		<?}?>
		
		<?if ($faseComissao){?>
		<tr>
			<td class="SubTituloCentro	" align="right" colspan="2">
				<b>Parecer Geral Comissão de Homologação</b>
			</td>
		</tr>
			<tr>
		<td class="SubTituloDireita" align="right">
			<b>Qualificação:</b>
		</td>
		<td>
			<?=  montaSituacao('Comissao',$leituraComissao,$comissao['sitcod'])?>
		</td>
	</tr>	
		<tr>
			<td class="SubTituloDireita" align="right"><b>Comissão:</b>
			</td>
			<td>
				<?php
				$parecerComissao = $comissao['pardsc'];
				if($leituraComissao)
				echo campo_textarea( 'parecerComissao', 'N', '', '',130, 10,'');
				else
				echo $parecerComissao;
				?>
			</td>
			
		</tr>
		<tr>
		<td class="SubTituloDireita">
		&nbsp;
		</td>
		  	<td>
		  		<? if($leituraComissao){?>
					<input type="button" value="Salvar" onclick="salvarParecerGeral('<?=base64_encode($unpid)?>',6)">
				<?}?>
			</td>
		</tr>
		<?}?>
		
		<?if ($faseFinal){?>
	
		
		<tr>
			<td class="SubTituloCentro	" align="right" colspan="2">
				<b>Parecer Geral Final</b>
			</td>
		</tr>
			<tr>
		<td class="SubTituloDireita" align="right">
			<b>Qualificação:</b>
		</td>
		<td>
			<?=  montaSituacao('Final',$leituraParecerFinal,$parecerFinal['sitcod'])?>
		</td>
	</tr>	
		<tr>
			<td class="SubTituloDireita" align="right"><b>Upload </br>
			(.doc / .pdf / .xls)</b>
			</td>
		<td>	
			<?
			if($faseFinal){
				echo '<input type="file" name="arquivo" />';

				$codArquivo = $parecerFinal['arqid'];

				$arrArquivoBanco = pegaArquivoPeloId( $codArquivo );

				if( (bool)$arrArquivoBanco['arqstatus'] == true)
				{
					?>
						<a  onclick="if(!confirm('Deseja remover o arquivo?'))return false ; else arquivo('<?= base64_encode($codArquivo)?>');">
							<img style="border:0px" src="../imagens/excluir.gif" title="<?= $arrArquivoBanco[ "arqnome" ] ?>" alt="<?= $arrArquivoBanco[ "arqdescricao" ] ?>" />
						</a>
					<?
				}


			}
			 ?>
			&nbsp;<?geraLinkParaArquivo( $parecerFinal['arqid'] , true );?>
		</td>
		</tr>
		<tr>
			<td class="SubTituloDireita" align="right"><b>Parecer Final:</b>
			</td>
			<td>
				<?php
				$parecerFinal = $parecerFinal['pardsc'];
				if($leituraParecerFinal)
				echo campo_textarea( 'parecerFinal', 'N', '', '',130, 10,'');
				else
				echo $parecerFinal;
				?>
			</td>
			
		</tr>
		<tr>
		<td class="SubTituloDireita">
		&nbsp;
		</td>
		  	<td>
		  		<? if($leituraParecerFinal){?>
					<input type="button" value="Salvar" onclick="salvarParecerGeral('<?=base64_encode($unpid)?>',7)">
				<?}?>
			</td>
		</tr>
		<?}?>
		
		
		

</table>
</form>


<script>


function arquivo(cod){

	var request =  window.XMLHttpRequest ? new XMLHttpRequest : new window.ActiveXObject( 'Microsoft.XmlHttp' );

	request.open( 'POST', '../reuni/ajax/arquivo.php',true) ;
	request.setRequestHeader( "Content-Type", "application/x-www-form-urlencoded; charset=iso-8859-1" );
	request.onreadystatechange=function()
	{
		if (request.readyState==4)
		{
			var x = request.responseText;
			alert(x);
			window.location.href = "reuni.php?modulo=principal/parecergeral&acao=C&unicod=<?=base64_encode($unicod)?>";

		}
	}
	request.send( 'cod=' +  cod  );

}

function pegaRadio(objRadio){

	for ( var i = 0 ; i < objRadio.length ; i++ ){
		if (objRadio[i].checked){
			return objRadio[ i ].value;
			break ;
		}
	}
	return false;
}

function salvarParecerGeral(codUnidade,tipo)
{

	

	var objRadio  	  = "";
	var objCampoTexto = "";

	switch(tipo)
	{

		case 4:
		objRadio      = document.formulario.situacao_Sesu;
		objCampoTexto =  tinyMCE.getContent('parecerSesu');
		break

		case 5:
		objRadio 	  = document.formulario.situacao_Adhoc;
		objCampoTexto =  tinyMCE.getContent('parecerAdhoc');
		break

		case 6:
		objRadio 	  = document.formulario.situacao_Comissao;
		objCampoTexto =  tinyMCE.getContent('parecerComissao');
		break

		case 7:
		objRadio 	  = document.formulario.situacao_Final;
		objCampoTexto =  tinyMCE.getContent('parecerFinal');
		break

		default:
		alert('Problemas na excução do script!');
		return false;
	}


	

	var valorRadio = pegaRadio(objRadio);




	var mensagem = "";

	if(trim(objCampoTexto) == ''){
		mensagem = "Informe o campo Descritivo. \n";
	}


	if(valorRadio == ''){
		mensagem += "Informe o campo Qualificação. \n";
	}

	if(mensagem.length > 0){
		alert(mensagem);
		return;
	}



	var request =  window.XMLHttpRequest ? new XMLHttpRequest : new window.ActiveXObject( 'Microsoft.XmlHttp' );

	request.open( 'POST', '../reuni/ajax/salvaParecerGeral.php',true);
	request.setRequestHeader( "Content-Type", "application/x-www-form-urlencoded; charset=iso-8859-1" );
	request.onreadystatechange=function()
	{
		if (request.readyState==4)
		{
			var x = request.responseText;
			alert(x);
			window.opener.reloadPage();
			document.formulario.submit();
		}
	}
	request.send( 'parecer=' + escape( objCampoTexto ) + "&codUnidade=" + codUnidade + "&tipoParecer=" + tipo + "&situacao=" + valorRadio  );

}

</script>