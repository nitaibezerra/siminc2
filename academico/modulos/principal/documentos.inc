<?php
pegaSessoes(1);
//dbg($_SESSION["academico"]);
$orgid 			= $_SESSION["academico"]["orgid"];
$prtid 			= $_SESSION["academico"]["prtid"];

$ano 			= $_SESSION['academico']['ano'];
$entidentidade	= $_SESSION['academico']["entid"];
$entidcampus 	= $_SESSION["academico"]["entidcampus"];

$arqid 	 		= $_REQUEST['arqid'];
$anxid		 	= $_REQUEST['anxid'];

$orgdesc 	= ($orgid == 1) ? "Educação Superior" : "Educação Profissional";

$autoriazacaoconcursos = new autoriazacaoconcursos();

$sql = "SELECT prtnumero FROM academico.portarias WHERE prtid = ".$prtid."" ;
$prtnumero = $db->pegaUm($sql);

if(isset($_REQUEST[evento_arquivo]) && ($_REQUEST[evento_arquivo] != '')){
	switch($_REQUEST[evento_arquivo]){
		case 'EA':
			$dados_arq = array('arqid' => $arqid, 'anxid' => $anxid);
			DeletarDocumento($dados_arq);
			echo "<script>
					alert('Dados salvos com sucesso.');					
					window.location = '?modulo=principal/documentos&acao=C&evento=A&prtid=".$prtid."&orgid=".$orgid."';
				  </script>";
			exit;
		break;
		case 'DA':
			$dados_arq['arqid'] =$_REQUEST['arqid']; 
			DownloadArquivo($dados_arq);
			exit;
		break;
	
	case 'SA':
		$arquivo 			 	= $_FILES['arquivo'];
		$arqdescricao 		 	= $_REQUEST['arqdescricao'];
		$tpaid			 		= $_REQUEST['tpaid'];
		$dados_arq = array('arqdescricao' => $arqdescricao,'tpaid' => $tpaid,  'prtid' => $prtid);
		//dbg($dados_arq,1);
		if(isset($arquivo['name']) && $arquivo['name'] != ''){
			EnviarArquivo($arquivo, $dados_arq);
		}
		echo "<script>
				alert('Dados salvos com sucesso.');					
				window.location = '?modulo=principal/documentos&acao=C&evento=A&prtid=".$prtid."&orgid=".$orgid."';
			  </script>";
		exit;
	break;
	}
	
}
// monta cabeçalho
include APPRAIZ . 'includes/cabecalho.inc';
print("<br/>");


if($orgid = 1){
	$menu[0] = array("descricao" => "Lista de Portaria", "link"=> "/academico/academico.php?modulo=inicio&acao=C");
}else{
	$menu[0] = array("descricao" => "Lista de Portaria", "link"=> "/academico/academico.php?modulo=principal/listaProfissional&acao=A");
}

echo montarAbasArray(carregardadosmenuacademico('dadosPortaria'), '/academico/academico.php?modulo=principal/documentos&acao=' . $_REQUEST['acao']);		
//$db->cria_aba($abacod_tela,$url,'');	
monta_titulo($orgdesc,'Documentos');

// cria o cabeçalho padrão do módulo
if (!empty($prtid) && !empty($entidcampus)){
	echo $autoriazacaoconcursos->cabecalho( $prtid, $entidcampus );
}
?>
<script type="text/javascript" src="../includes/prototype.js"></script>
<script type="text/javascript" src="/includes/livesearch.js"></script>
<script type="text/javascript" src="/includes/abas.js"></script>
<script type="text/javascript">
<!--
function excluirDocumento(evento, arqid, anxid){
	if(confirm("Deseja realmente excluir este documento ?")){
		document.getElementById('evento').value = '';
		document.getElementById('evento_arquivo').value = evento;
		document.getElementById('arqid').value = arqid;
		document.getElementById('anxid').value = anxid;
		document.formulario.submit();		
	}
}
function downloadArquivo (evento, anxid, arqid){
	document.getElementById('evento').value = '';
	document.getElementById('evento_arquivo').value = evento;
	document.getElementById('anxid').value = anxid;
	document.getElementById('arqid').value = arqid;
	document.formulario.submit();		
}
function novoArq(){
	document.getElementById('evento').value = '';
	document.getElementById('arqid').value = '';	
	document.formulario.submit();		
}
function validarArquivo(){
	
	if(document.formulario.arquivo.value == ""){
		alert("Campo Arquivo obrigatório.");
		document.formulario.arquivo.focus();
		return false;
	}
	if(document.formulario.tpaid.value == ""){
		alert("Campo Tipo obrigatório.");
		document.formulario.tpaid.focus();
		return false;
	}
	if(document.formulario.arqdescricao.value == ""){
		alert("Campo Descrição obrigatório.");
		document.formulario.arqdescricao.focus();
		return false;
	}	
	return true;
}
function submeter(evento){		
	if (validarArquivo()){
		document.getElementById('evento').value = evento;
		document.getElementById('anxid').value = '';
		document.getElementById('arqid').value = '';
		document.getElementById('evento_arquivo').value = 'SA';
		document.formulario.submit();	
	}		
}
//-->
</script>
<body>
<form method="post" name="formulario" enctype="multipart/form-data">
	<input type="hidden" name="evento" id="evento" value="I">	
	<input type="hidden" name="evento_arquivo" id="evento_arquivo" value=""/>
	<input type="hidden" name="tipo" id="tipo" value="<?=$tipo?>">	
	<input type="hidden" name="arqid" id="arqid" value="<?=$arqid?>">
	<input type="hidden" name="anxid" id="anxid" value="<?=$anxid?>">
	<input type="hidden" name="prtid" id="prtid" value="<?=$prtid?>">
	<input type="hidden" name="mostrarAbas" id="mostrarAbas">
	
<table class="tabela" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" align="center">
		<tr>
			<td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%">Arquivo:</td>
			<td>
				<input type="file" name="arquivo" <? if (!$habilitado){ echo 'disabled="disabled"'; } ?>/>							
			</td>
		</tr>
		<tr>
			<td align='right' class="SubTituloDireita" style="vertical-align:top; width:30%">Tipo:</td>
			<td><?php			
			$sql = "
				SELECT tpaid AS codigo, tpadsc AS descricao 
					FROM academico.tipoarquivo
			";			
			$db->monta_combo('tpaid', $sql, $habil, "Selecione...", '', '', '', '100');
		?></td>
		</tr>
		<tr>
			<td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%">Descrição:</td>
			<td><?= campo_textarea( 'arqdescricao', 'N', $habil, '', 60, 2, 100 );?></td>
		</tr>
		<tr style="background-color: #cccccc">
			<td align='right' style="vertical-align:top; width:25%">&nbsp;</td>
			<td>
			    <input type="button" class="botao" name="btassociar" value="Gravar" onclick="submeter('I', '<?=$anxid?>', '<?=$arqid?>');" <? echo $disabled; ?>>
			</td>
		</tr>
</table>
<?
if($prtid){ 

	if ( !$habilitado ){
		$btexcluirdoc = "<img src=\"/imagens/excluir_01.gif\" border=0 title=\"Excluir\">";
	}else{
		$btexcluirdoc = "<a href=\"#\" onclick=\"javascript:excluirDocumento(\'EA\',' || ar.arqid || ',' || ax.anxid || ');\"><img src=\"/imagens/excluir.gif\" border=0 title=\"Excluir\"></a>";
	}
	
	$sql = "SELECT 
				'<center>{$btexcluirdoc}</center>' as acao,
				to_char(ax.anxdtinclusao, 'DD/MM/YYYY') as data,
				ta.tpadsc as tipo,
				'<a style=\"cursor: pointer; color: blue;\" onclick=\"javascript: downloadArquivo(\'DA\',' ||ax.anxid|| ',' || ar.arqid || ');\" />' || ar.arqnome || '.'|| ar.arqextensao ||'</a>' as t1,
				ar.arqtamanho as tamanho,
				ar.arqnome as nome,
				s.usunome as responsavel
			FROM
				academico.anexos as ax
			INNER JOIN
				public.arquivo as ar ON ar.arqid = ax.arqid
			INNER JOIN
				academico.portarias as p ON p.prtid = ax.prtid
			INNER JOIN
				academico.tipoarquivo as ta ON ta.tpaid = ax.tpaid
			INNER JOIN
				seguranca.usuario as s ON s.usucpf = ar.usucpf
			WHERE
				anxstatus = 'A' AND
				p.prtid = ".$prtid." AND
				prtano = '$ano'";

	$cabecalho = array( "Ação", 
						"Data Inclusão",
						"Tipo Arquivo",
						"Nome Arquivo",
						"Tamanho (kbs)",
						"Descrição Arquivo",
						"Responsável");
	//$sql = $db->carregar($sql);
	$db->monta_lista( $sql, $cabecalho, 50, 10, 'N', '', '' );
}
?>
</form>
</body>

