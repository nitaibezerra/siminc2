<?php

// monta cabeçalho



include APPRAIZ . 'includes/cabecalho.inc';
include APPRAIZ . 'includes/arquivo.inc';
include APPRAIZ . 'includes/workflow.php';
include APPRAIZ . 'reuni/www/funcoes.php';

print '<br/>';

$db->cria_aba( $abacod_tela, $url, '' );
monta_titulo( $titulo_modulo, '&nbsp;' );

$unicod = base64_decode($_REQUEST['unicod']);
$partes = explode('_',md5_decrypt($_REQUEST['id'],''));

$codPergunta = $partes[0];
$pai 	  	 = $partes[1];
$filho 		 = $partes[2];


$editaresposta	 = reuni_podeEditarResposta( $codPergunta );
$leituraResposta = reuni_podeVerResposta( $codPergunta );
$leituraSesu     = reuni_podeVerParecerRespostaSesu( $codPergunta );
$leituraAdhoc    = reuni_podeVerParecerRespostaAdhoc( $codPergunta );
$leituraComissao = reuni_podeVerParecerRespostaComissao( $codPergunta );

$faseSesu 		= reuni_podeEditarParecerRespostaSesu( $codPergunta );
$faseAdhoc 		= reuni_podeEditarParecerRespostaAdhoc( $codPergunta );
$faseComissao 	= reuni_podeEditarParecerRespostaComissao( $codPergunta );

$sql 			= "select *, rspdsc as resposta , rsparqcod as arquivo from reuni.resposta where unicod = '".base64_decode($_REQUEST["unicod"])."' and  prgcod = ".$codPergunta;
$dado 			= $db->pegaLinha($sql);

if ($dado['rspcod'] == '') {
	$sql = "insert into reuni.resposta (
				prgcod,
				unicod,
				unitpocod,
				rspdsc
			) values (
				$codPergunta,
				" . base64_decode($_REQUEST["unicod"]) . ",
				'U',
				'')";

	$db->executar($sql);
	$db->commit();

	$sql  = "select rspcod, unicod, rspdsc as resposta , rsparqcod as arquivo from reuni.resposta where unicod = '".base64_decode($_REQUEST["unicod"])."' and  prgcod = ".$codPergunta;
	$dado = $db->pegaLinha($sql);
}

$codResposta 	= $dado['rspcod'];
$resposta 		= $dado['resposta'];
$intArquivoId 	= $dado['arquivo'];
$unicod 		= $dado['unicod'];
$max 			= (int)base64_decode($_REQUEST['len']);
$unicod 		= (int)base64_decode($_REQUEST['unicod']);


$sesu 		= montaParecer(1,$codResposta);
$adhoc 		= montaParecer(2,$codResposta);
$comissao 	= montaParecer(3,$codResposta);

?>
<!-- biblioteca javascript local -->
<script type="text/javascript">
</script>
<script language="JavaScript" src="../includes/tiny_mce.js"></script>
<script language="JavaScript">
//Editor de textos
tinyMCE.init({
	mode: "specific_textareas",
	editor_selector : "text_editor_simple",
	theme : "advanced",
	plugins : "table,save,advhr,advimage,advlink,emotions,iespell,insertdatetime,preview,zoom,flash,searchreplace,print,contextmenu,paste,directionality,fullscreen",
	theme_advanced_buttons1 : "cut,copy,paste,pastetext,pasteword,separator,search,replace,separator,tablecontrols, separator, preview",
	theme_advanced_buttons2 : "undo,redo,separator,bold,italic,underline,separator,justifyleft,justifycenter,justifyright,justifyfull,separator,outdent,indent,bullist,numlist,separator,forecolor,fontselect,fontsizeselect ",
	theme_advanced_buttons3 : "",
	theme_advanced_toolbar_location : "top",
	theme_advanced_toolbar_align : "left",
	valid_elements : "" +
	"-strong/-b[class|style]," +
	"-ol[class|style]," +
	"-ul[class|style]," +
	"-li[class|style]," +
	"#p[id|style|dir|class|align]," +
	"br," +
	"-blockquote[dir|style]," +
	"-table[border=0|cellspacing|cellpadding|width|height|class|align|summary|style|dir|id|lang|bgcolor|background|bordercolor]," +
	"-tr[id|lang|dir|class|rowspan|width|height|align|valign|style|bgcolor|background|bordercolor]," +
	"tbody[id|class]," +
	"thead[id|class]," +
	"tfoot[id|class]," +
	"-td[id|lang|dir|class|colspan|rowspan|width|height|align|valign|style|bgcolor|background|bordercolor|scope]," +
	"-th[id|lang|dir|class|colspan|rowspan|width|height|align|valign|style|scope]" ,
	language : "pt_br",
	entity_encoding : "raw"
});
</script>

<!-- corpo da página -->

<?php

function montaBarraDeNavegacao($unicod,$codPergunta){
	global $db;

	$sql = "select
			codPai,
			codFilho,
			codPergunta,
			quantidade
		from
			(
				select
					gp.grpcod as codPai,
					gp2.grpcod as codFilho,
					p.prgcod as codPergunta,
					p.prgqtdcar as quantidade
				from reuni.grupopergunta gp
					inner join reuni.grupopergunta gp2 ON gp2.grpcodfil = gp.grpcod
					inner join reuni.pergunta p ON p.grpcod = gp2.grpcod
				group by gp.grpcod,
					gp2.grpcod,
					p.prgcod,
					p.prgqtdcar
				having p.prgcod > $codPergunta
			union all
				select
					gp.grpcod as codPai,
					null as codFilho,
					p.prgcod as codPergunta,
					p.prgqtdcar as quantidade
				from reuni.grupopergunta gp
					inner join reuni.pergunta p ON p.grpcod = gp.grpcod
				where 	
					gp.grpcodfil is null
				group by gp.grpcod,
					p.prgcod,
					p.prgqtdcar
				having p.prgcod > $codPergunta
		
			) as foo
		group by 
			codPai,
			codFilho,
			codPergunta,
			quantidade
		order by
			codPergunta limit 1
		
		";


	$sql2 = "select
			codPai,
			codFilho,
			codPergunta,
			quantidade
		from
			(
				select
					gp.grpcod as codPai,
					gp2.grpcod as codFilho,
					p.prgcod as codPergunta,
					p.prgqtdcar as quantidade
				from reuni.grupopergunta gp
					inner join reuni.grupopergunta gp2 ON gp2.grpcodfil = gp.grpcod
					inner join reuni.pergunta p ON p.grpcod = gp2.grpcod
				group by gp.grpcod,
					gp2.grpcod,
					p.prgcod,
					p.prgqtdcar
				having p.prgcod < $codPergunta
			union all
				select
					gp.grpcod as codPai,
					null as codFilho,
					p.prgcod as codPergunta,
					p.prgqtdcar as quantidade
				from reuni.grupopergunta gp
					inner join reuni.pergunta p ON p.grpcod = gp.grpcod
				where 	
					gp.grpcodfil is null
				group by gp.grpcod,
					p.prgcod,
					p.prgqtdcar
				having p.prgcod < $codPergunta
		
			) as foo
		group by 
			codPai,
			codFilho,
			codPergunta,
			quantidade
		order by
			codPergunta desc limit 1
		
		";


	$proximo = $db->pegaLinha($sql);
	$linkProximo =  'window.location="/'.$_SESSION['sisdiretorio'].'/'.$_SESSION['sisarquivo'].'.php?modulo=principal/cadastroproposta&acao=C&id='.md5_encrypt($proximo['codpergunta'] .'_'.$proximo['codpai'] .'_'.$proximo['codfilho'] ,'').'&unicod='.base64_encode($unicod).'&len='.base64_encode($proximo['quantidade']).' "';

	$anterior = $db->pegaLinha($sql2);
	$linkAnterior =  'window.location="/'.$_SESSION['sisdiretorio'].'/'.$_SESSION['sisarquivo'].'.php?modulo=principal/cadastroproposta&acao=C&id='.md5_encrypt($anterior['codpergunta'] .'_'.$anterior['codpai'] .'_'.$anterior['codfilho'] ,'').'&unicod='.base64_encode($unicod).'&len='.base64_encode($anterior['quantidade']).' "';


	$saida = "";

	if( $anterior )
	$saida .= "<input class='botao' type='button' onclick='$linkAnterior' value='Anterior'>&nbsp;";


	if( $proximo )
	$saida .= "<input class='botao' type='button' onclick='$linkProximo' value='Proximo'>";


	return $saida;
}

function montaParecer($tipoParecer,$codResposta){
	global $db;
	if(!isset($codResposta)) return false;
	$sql = "select sitcod , pardsc , arqid from reuni.parecer where tpacod = $tipoParecer and rspcod = $codResposta";
	return $db->pegaLinha($sql);
}

function nomePaiFilho($cod,$p=false){
	global $db;
	if (!$cod || trim($cod) == '')
        return;

	$sql = "select grpdsc from reuni.grupopergunta where grpcod = $cod";
	$dado = $db->pegaUm($sql);

	if($p){
		$item = 'Item:';
	}else{
		$item = 'Tópico:';
	}

	if($dado){
		$saida = '<tr><td class="SubTituloDireita" align="right"><b>' . $item . '</b></td><td>' .$dado . '</td></tr>';
		return $saida;
	}
}

function nomePergunta($cod,$pai){
	global $db;
	$sql = "select prgdsc from reuni.pergunta where prgcod = $cod";
	$dado = $db->pegaUm($sql);
	if($dado)
	{
		if(	$pai < 28 )
		{
			$saida = '<tr><td class="SubTituloDireita" align="right"><b>Sub-Item:</b></td><td>' .$dado . '</td></tr>';
		}else
		{
			$saida = '<tr><td class="SubTituloDireita" align="right"><b>Item:</b></td><td>' .$dado . '</td></tr>';
		}
		return $saida;
	}
}



function pesoPergunta($cod){
	global $db;
	$sql = "select prgpeso from reuni.pergunta where prgcod = $cod";
	$dado = $db->pegaUm($sql);

	$saida = '<tr><td class="SubTituloDireita" align="right"><b>Peso da questão:</b></td><td>' .$dado . '</td></tr>';
	return $saida;

}


function insertUpload($unicod,$codPergunta){
	global  $db;
	$sql = "select count(*) from reuni.resposta where unicod = '$unicod' and prgcod = $codPergunta";
	$dado = $db->pegaUm($sql);

	if($dado > 0)
	{
		return true;
	}
	else
	{
		return false;
	}
}

function montaSituacao($nome='',$readOnly=false,$codParecer=''){
	global  $db;
	$sql = "select sitdsc , sitcod from reuni.situacao";
	$dado = $db->carregar($sql);
	$saida = '';
	$check = '';

    if (trim($codParecer) == '')
        return null;

	if ($readOnly == false) {
		$sql = "select sitdsc from reuni.situacao where  sitcod = $codParecer";
		return $db->pegaUm($sql);

	}

	foreach ($dado as $row)
	{

		if($row['sitcod'] == $codParecer) $check = "checked='checked'";
		$saida .= "<input type='radio' name='situacao_" . strtolower($nome) . "' $read $check value='".$row['sitcod']."'>". $row['sitdsc']	. "&nbsp;";
		$check = '';
	}
	return $saida;
}
function parecerPreenchido($codPergunta,$unicod){
	global  $db;
	$sql = "select parcod from reuni.resposta where prgcod = $codPergunta and unicod = '$unicod'";
	$dado = $db->pegaUm($sql);
	if($dado == "") return false;
	else
	return true;
}

function mostraParecer($codPergunta,$unicod){
	global  $db;
	$sql = "select sitcod as situacao, pardscsesu from reuni.resposta inner join reuni.parecer on resposta.parcod = parecer.parcod where resposta.prgcod = $codPergunta and resposta.unicod = '$unicod' ";

	$dado = $db->pegaLinha($sql);
	return $dado;
}


function parecerEnviado($codPergunta,$unicod){
	global  $db;
	$sql = "select parstssesu as enviado from reuni.resposta inner join reuni.parecer on resposta.parcod = parecer.parcod where resposta.prgcod = $codPergunta and resposta.unicod = '$unicod' ";
	$dado = $db->pegaUm($sql);
	return $dado;
}







if(isset($_REQUEST['resposta']) or $_FILES["arquivo"] )
{

	$resp = trim($_REQUEST['resposta']);

	$unicod = base64_decode($_REQUEST['unicod']);

	$unipocod = 'U';

	if(insertUpload($unicod,$codPergunta))
	{
		$sql = "update reuni.resposta set  rspdsc = '$resp' where prgcod= $codPergunta and unicod = '$unicod' and unitpocod = '$unipocod'";
	}
	else
	{
		$sql = "insert into reuni.resposta (prgcod,unicod,unitpocod,rspdsc) values ($codPergunta,$unicod,'$unipocod','$resp')";
	}

	$db->executar($sql);


	$sql = "select rspcod from reuni.resposta where prgcod = $codPergunta and unicod = '$unicod' ";
	$codResposta  = $db->pegaUm($sql);

	try{

		$codArquivo = salvarArquivo($_SESSION['usucpf'],$_SESSION['sisid'],$_FILES["arquivo"],getTiposArquivoEscritorio());
		//salvar o id na tabela;
		$sql = "update reuni.resposta set rsparqcod = $codArquivo , rspdsc = '".$_FILES["arquivo"]["name"]. "' where unicod = '$unicod' and rspcod = $codResposta ";

		if(isset($codArquivo) && $codArquivo != null){

			$db->executar($sql);

		}

		$db->commit();
		$db->sucesso('principal/cadastroproposta','&unicod='.$_REQUEST['unicod'].'&id='.$_REQUEST['id'].'&len='.$_REQUEST['len']);
	}catch ( Exception $objError ){
		echo '<script> alert("'.$objError->getMessage().'")</script>';
	}
}



?>




<form action="" method="post" name="formulario" enctype="multipart/form-data">

<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
	<?= nomePaiFilho($pai)?>
	<?= nomePaiFilho($filho,1) ?>
	<?= nomePergunta($codPergunta,$pai)?>
	
	
	<?php if($max != 0){?>
	
	
	
	
	<tr>
		<td class="SubTituloDireita" align="right"><b>Texto:</b></td>
		<td>
	<?	if ($editaresposta) {?>
			<textarea style="width:130ex;" rows="15" class="text_editor_simple" name="resposta"><?=$resposta?></textarea>
<?		}
else
{
	if($leituraResposta)
	{
		echo $resposta;
	}
	else
	{
		echo "Não Respondido / Em Elaboração";
	}
}
		?>

		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita">
		</td>
		<td>
			<b>Quantidade maxima de caracteres: <?=$max?></b>
		</td>
	</tr>
	<?php

	} else{

		?>
	<tr>
		<td class="SubTituloDireita" align="right"><b>Upload </br>
		(.doc / .pdf / .xls)</b>
		</td>
		<td>	
			<?
			if($editaresposta){
				echo '<input type="file" name="arquivo" />';
				$funcao = base64_encode("updateArquivo,null,$unicod,$codResposta");
				geraLinkParaRemoverArquivo($intArquivoId ,$funcao );
			}
			 ?>
			&nbsp;<?geraLinkParaArquivo( $intArquivoId,true );?>
		</td>
	</tr>
	<?php }?>
	
	<?if ( $leituraSesu && ( $faseSesu || $parecerSesu ) ){?>
	
	<tr>
		<td class="SubTituloCentro"  colspan="2">
		<b>Parecer SESU</b>
		</td>
	</tr>
	<!--
	<tr>
		<td class="SubTituloDireita" align="right">
			<b>Qualificação:</b>
		</td>
		<td>
			<?  //montaSituacao('Sesu',$leituraSesu,$sesu['sitcod'])?>
		</td>
	</tr>
	-->
	<tr>
		<td class="SubTituloDireita" align="right">
			<b>Observações:</b>
		</td>
			<td>	
				<?php

				$codArquivo = $sesu['arqid'];


				$parecerSesu = $sesu['pardsc'];
				if($faseSesu)
					echo campo_textarea( 'parecerSesu', 'N', '', '',130, 10,'');
				else
					echo $parecerSesu;
				?>
			</td>
		</tr>
		
			
			<?}?>
			
		<?if ($leituraAdhoc){?>	
		<tr>
			<td class="SubTituloCentro	" align="right" colspan="2">
				<b>Parecer ADHOC</b>
			</td>
		</tr>
			<tr>
		<td class="SubTituloDireita" align="right">
			<b>Qualificação:</b>
		</td>
		<td>
			<?=  montaSituacao('Adhoc',$faseAdhoc,$adhoc['sitcod'])?>
		</td>
	</tr>	
		<tr>
			<td class="SubTituloDireita" align="right"><b>ADHOC:</b>
			</td>
			<td>
				<?php
				$parecerAdhoc = $adhoc['pardsc'];
				if($faseAdhoc)
					echo campo_textarea( 'parecerAdhoc', 'N', '', '',130, 10,'');
				else
					echo $parecerAdhoc;
				?>
			</td>
		
		<?}?>
		
		<?if ($leituraComissao){?>
		<tr>
			<td class="SubTituloCentro	" align="right" colspan="2">
				<b>Parecer Comissão de Homologação</b>
			</td>
		</tr>
		<!-- 
		<tr>
		<td class="SubTituloDireita" align="right">
			<b>Qualificação:</b>
		</td>
		 -->
		<td>
			<?//=  montaSituacao('Comissao',$faseComissao,$comissao['sitcod'])?>
		</td>
	</tr>	
		<tr>
			<td class="SubTituloDireita" align="right"><b>Comissão:</b>
			</td>
			<td>
				<?php
				$parecerComissao = $comissao['pardsc'];
				if($faseComissao)
					echo campo_textarea( 'parecerComissao', 'N', '', '',130, 10,'');
				else
					echo $parecerComissao;
				?>
			</td>
			
		</tr>
	
		<?}?>
		
	
	

	<tr bgcolor="#cococo">
		<td align="right"></td>

		<td align="left">
		
		<div style="float:left">
		<input class="botao" type="button" value="Voltar" onclick="history.back();"> 
		
		<?if($db->testa_superuser()){?>
			<input class="botao" type="button" value="Salvar" onclick="salvarParecer('<?=base64_encode($codResposta)?>',1,true); salvarParecer('<?=base64_encode($codResposta)?>',2,true); salvarParecer('<?=base64_encode($codResposta)?>',3,true);">
		<?} else {?>
			
			<?if($editaresposta){?>
				<input class="botao" type="button" value="Salvar" onclick="document.formulario.submit();">
			<?}?>
			
			<? if($faseSesu){?>
						<input type="button" class="botao"  value="Salvar" name="salvarSesu" onclick="salvarParecer('<?=base64_encode($codResposta)?>',1,false);">	&nbsp;	
			<?}?>
			
			<? if($faseAdhoc){?>
					<input type="button" class="botao"  value="Salvar" onclick="salvarParecer('<?=base64_encode($codResposta)?>',2,false)">
			<?}?>
			
			<? if($faseComissao){?>
					<input type="button" class="botao"  value="Salvar" onclick="salvarParecer('<?=base64_encode($codResposta)?>',3,false)">
			<?}?>
			
		<?}?>
		</div>
		<div style="float:right"> <?=montaBarraDeNavegacao($unicod,$codPergunta); ?> </div>
		</td>
	</tr>
	
</table>
</form>


<script>

function pegaRadio(objRadio){
	for ( var i = 0 ; i < objRadio.length ; i++ ){
		if (objRadio[i].checked) {
			return objRadio[ i ].value;
			break ;
		}
	}
	return false;
}

function salvarParecer(codResposta,tipo,superUser)
{


	var objRadio      = null;
	var objCampoTexto = "";
	var flag = true;

	switch(tipo)
	{

		case 1:
            objRadio      = document.formulario.situacao_sesu;
            objCampoTexto = document.formulario.parecerSesu.value;
            flag = false;
            break;

		case 2:
            objRadio      = document.formulario.situacao_adhoc;
            objCampoTexto = document.formulario.parecerAdhoc.value;
            break;

		case 3:
            objRadio      = document.formulario.situacao_comissao;
            objCampoTexto = document.formulario.parecerComissao.value;
            flag = false;
            break;

		default:
            alert('Problemas na excução do script!');
            return false;

	}


	var valorRadio = flag && objRadio ? pegaRadio(objRadio) : 'null';
	var mensagem = "";

	if (valorRadio == null) {
		mensagem = "Informe o campo Qualificação.\n";
	}

	if (mensagem.length > 0) {
		alert(mensagem);
		return;
	}



	var request =  window.XMLHttpRequest ? new XMLHttpRequest : new window.ActiveXObject( 'Microsoft.XmlHttp' );

	request.open( 'POST' , '../reuni/ajax/salvaParecer.php',true);
	request.setRequestHeader( "Content-Type", "application/x-www-form-urlencoded; charset=iso-8859-1" );
	request.onreadystatechange=function()
	{
		if (request.readyState==4)
		{
			var x = request.responseText;
			if (!superUser)
			{
				alert(x);
				//document.formulario.submit();
			} else {
				if (tipo == 3){
					//document.formulario.submit();
				}
			}

		}
	}

	request.send('parecer=' + escape( objCampoTexto ) + "&codPergunta=" + codResposta + "&tipoParecer=" + tipo + "&situacao=" + valorRadio );
	document.formulario.submit();
}



</script>