<?php
include_once APPRAIZ . "includes/classes/fileSimec.class.inc";
include_once APPRAIZ . 'includes/workflow.php';

verificaPermissao();

if( $_REQUEST['submeter'] == '1' ){
	$data = formata_data_sql($_POST['anxdata']);
	
	if($_FILES["arquivo"] && !$_POST['arqid']){
		$campos	= array("ptrid"  => $_SESSION['emenda']['ptrid'],
						"anxdsc" => "'{$_POST['anxdsc']}'",
						"anxdata" => "'now()'",
						"anxprotocolo" => "'{$_POST['anxprotocolo']}'",
						"anxtipo" => "'C'"
						);	
		$file = new FilesSimec("anexo", $campos ,"emenda");
		$arquivoSalvo = $file->setUpload();
		if($arquivoSalvo){
			echo '<script type="text/javascript"> alert(" Operação realizada com sucesso!");</script>';
			echo"<script>window.location.href = 'emenda.php?modulo=principal/documentosPTA&acao=A';</script>";
			die;	
		}
	} elseif($_POST['arqid']) {
	    $sql = "UPDATE emenda.anexo SET anxdsc = '{$_POST['anxdsc']}', anxprotocolo ='{$_POST['anxprotocolo']}' where arqid=".$_POST['arqid']." and anxtipo = 'C'";
	    $db->executar($sql);
	    $db->commit();
	    echo '<script type="text/javascript"> 
	    		alert("Operação realizada com sucesso!");
	    		window.location.href = "emenda.php?modulo=principal/documentosPTA&acao=A";
	    	  </script>';
	    die;
	}
}

if($_GET['arqidDel']){
    $sql = "DELETE FROM emenda.anexo where arqid=".$_REQUEST['arqidDel']." and anxtipo = 'C'";
    $db->executar($sql);
    $sql = "UPDATE public.arquivo SET arqstatus = 'I' where arqid=".$_REQUEST['arqidDel'];
    $db->executar($sql);
    $db->commit();
    
    $file = new FilesSimec();
	$file->excluiArquivoFisico($_GET['arqidDel']);
	
    echo '<script type="text/javascript"> 
    		alert("Operação realizada com sucesso!");
    		window.location.href="emenda.php?modulo=principal/documentosPTA&acao=A";
    	  </script>';
    die;
}

#manipular anexos de arquivos para envio de email
if($_REQUEST['download'] == 'S'){
	$file = new FilesSimec();
	$arqid = $_REQUEST['arqid'];
    $arquivo = $file->getDownloadArquivo($arqid);
    echo"<script>window.location.href = 'emenda.php?modulo=principal/documentosPTA&acao=A';</script>";
    exit;
}

$_SESSION["emenda"]["ptrid"] = !empty( $_REQUEST["ptrid"] ) ? $_REQUEST["ptrid"] : $_SESSION["emenda"]["ptrid"];

validaSessionPTA( $_SESSION["emenda"]["ptrid"] );

if( $_SESSION['emenda']['tpopup'] != 'popup' ){
	include  APPRAIZ."includes/cabecalho.inc";
} else {
	echo '<script type="text/javascript" src="/includes/funcoes.js"></script>
		  <script type="text/javascript" src="/includes/prototype.js"></script>
		  <link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
		  <link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>';
}

validaSessionPTA( $_SESSION['emenda']['federal'] );

$estadoAtual = pegarEstadoAtual( $_SESSION["emenda"]["ptrid"] );

print "<br/>";
echo exibeInstituicaoBenefiada( $_SESSION["emenda"]["enbid"] );
print "<br/>";
montaAbasPTA( $abacod_tela, $url, '', $estadoAtual );
monta_titulo( 'Documentos', '');

echo cabecalhoPlanoTrabalho($_SESSION["emenda"]["ptrid"]);

echo '<table class="tabela" width="95%" bgcolor="#f5f5f5" cellspacing="1" cellpadding="2" align="center">
			<tr bgcolor="#D0D0D0">
				<td>
					<img src="/imagens/consultar.gif" style="cursor:pointer;" onclick="visualizarPTA( '. $_SESSION["emenda"]["ptrid"] .' );">
					<a style="cursor:pointer;" onclick="visualizarPTA( '. $_SESSION["emenda"]["ptrid"] .' );"> Visualizar PTA </a>
				</td>
			</tr>
		  </table>';

monta_titulo( '', 'Lista de documentos disponíveis para impressão');
?>
<script type="text/javascript" src="js/emenda.js"></script>
<html>
<body>
<table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="4" style="border-bottom:none;">
<?
	$arDados = array();
	$ptrids = $_SESSION['emenda']['ptrid'];
    
	$sql = "select ptrid from emenda.planotrabalho  where ptrcod in (select ptrcod from emenda.planotrabalho where ptrid = {$_SESSION['emenda']['ptrid']})";
	$arptrid = $db->carregarColuna($sql);
	if(is_array($arptrid)){
		$ptrids = implode( ",", $arptrid );
	}
	
    $sql = "select anx.anxid,
				   anx.arqid,
				   anx.ptrid,
				   anx.anxdsc,
				   arq.arqnome || '.' || arq.arqextensao as arquivo
			from emenda.anexo anx
				inner join public.arquivo arq on anx.arqid = arq.arqid
	   		where ptrid in ( {$ptrids} ) and anx.anxtipo = 'D'";
	   
	$arDados = $db->carregar($sql);
    
    $count = 1;
    
    $perfil = pegaPerfil($_SESSION['usucpf']);
    
    if($arDados) {
?>
	<tr>
	    <th>Arquivo</th>
	    <th>Descrição</th>
    </tr>
    <?php 
    
    foreach($arDados as $dados){
    ?>
	<tr>		    
        <td align="left">
        	<?php echo $count.' - '; ?><a style="cursor: pointer; color: blue;" onclick="window.location='?modulo=principal/documentosPTA&acao=A&download=S&arqid=<?php echo $dados['arqid'];?>&tipo=D'"><?php echo $dados['arquivo'];?></a>
        </td>
        <td align="left">
        	<?php echo $dados['anxdsc']; ?>
        </td>
	   	</tr>
    <?php
    $count++;
    }
	} else {
		echo '<tr>
		    	<td style="color: red">Nenhum documento disponível!</td>
		    </tr>';
	}
    ?>
</table>
<? monta_titulo( '', 'Lista de documento(s) disponibilizado(s) pelo convenente ao concedente'); ?>
<script type="text/javascript" src="/includes/JQuery/jquery-1.4.2.min.js"></script>
<script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/dateFunctions.js"></script>
<script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>
<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>
<form name=formulario id=formulario method=post enctype="multipart/form-data">
<input type="hidden" name="arqid" id="arqid" value="<?php echo $_GET['arqid'] ?>" />
<input type="hidden" name="submeter" id="submeter" value="1">
<table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="4" style="border-bottom:none;">
<?
	$arDados = array();
	$ptrids = $_SESSION['emenda']['ptrid'];
    
	$sql = "select ptrid from emenda.planotrabalho  where ptrcod in (select ptrcod from emenda.planotrabalho where ptrid = {$_SESSION['emenda']['ptrid']})";
	$arptrid = $db->carregarColuna($sql);
	if(is_array($arptrid)){
		$ptrids = implode( ",", $arptrid );
	}
	$arDados = array();
	$filtro = '';
	if( $_REQUEST['arqid'] && $_REQUEST['tipo'] ){
		$sql = "select anx.anxid,
				   anx.arqid,
				   anx.ptrid,
				   anx.anxdsc,
				   arq.arqnome || '.' || arq.arqextensao as arquivo,
				   anx.anxdata,
				   anx.anxprotocolo,
				   anx.anxtipo				   
			from emenda.anexo anx
				inner join public.arquivo arq on anx.arqid = arq.arqid
	   		where ptrid in ( {$ptrids} ) and anx.anxtipo = 'C' and arq.arqid = {$_REQUEST['arqid']}";
		$arReg = $db->pegaLinha( $sql );
		$arReg = $arReg ? $arReg : array();
		extract($arReg);
		
		$anxdata = formata_data( $anxdata );
	}
    $sql = "select anx.anxid,
				   anx.arqid,
				   anx.ptrid,
				   anx.anxdsc,
				   arq.arqnome || '.' || arq.arqextensao as arquivo,
				   anx.anxdata,
				   anx.anxprotocolo,
				   anx.anxtipo				   
			from emenda.anexo anx
				inner join public.arquivo arq on anx.arqid = arq.arqid
	   		where ptrid in ( {$ptrids} ) and anx.anxtipo = 'C' ".$filtro;
	   
	$arDados = $db->carregar($sql);
    
    $count = 1;
    
    $perfil = pegaPerfil($_SESSION['usucpf']);
?>
	<tr>
	    <th>Arquivo</th>
	    <th>Descrição</th>
	    <th>Data do Envio</th>
	    <th>Indicação do Protocolo de Envio emitido pelos correios(AR, Sedex)</th>
	    <th></th>
    </tr>
	<tr>
	    <td><input type="file" name="arquivo" id="arquivo" /></td>
	    <td><?=campo_texto( 'anxdsc', 'S', 'S', 'Descrição Anexo', 80, 100, '', '','','','','id="anxdsc"'); ?> </td>
	    <td>&nbsp;&nbsp; <? /*=campo_data2( 'anxdata', 'S', 'S','Data Inicio', '', '', '');*/ ?> </td>
	    <td><?=campo_texto( 'anxprotocolo', 'S', 'S', 'Protocolo', 50, 100, '', '','','','','id="anxprotocolo"'); ?> </td>
	    <td><input type="button" name="btnSalvar" id="btnSalvar" onclick="validaFormAnexos();" value="Salvar" <?=verificaPermissaoPerfil( 'pta', '', $estadoAtual, true ); ?>> </td>
    </tr>
    <?php 
    if($arDados) {
	    foreach($arDados as $dados){
	    ?>
		<tr>		    
	        <td align="left">
	        	<?php echo $count.' - '; ?><a style="cursor: pointer; color: blue;" onclick="window.location='?modulo=principal/documentosPTA&acao=A&download=S&arqid=<?php echo $dados['arqid'];?>&tipo=D'"><?php echo $dados['arquivo'];?></a>
	        </td>
	        <td align="left"><?php echo $dados['anxdsc']; ?></td>
	        <td align="left"><?php echo formata_data($dados['anxdata']); ?></td>
	        <td align="left"><?php echo $dados['anxprotocolo']; ?></td>
	        <td align="center">
	        <? if( verificaPermissaoPerfil( 'pta', 'boolean', $estadoAtual, true ) ) {?>
	        	<img src="../imagens/alterar.gif" onClick="window.location.href='emenda.php?modulo=principal/documentosPTA&acao=A&arqid=<?php echo $dados['arqid'];?>&tipo=<?=$dados['anxtipo'] ?>'" style="border:0; cursor:pointer;" title="Alterar Descrição Anexo">
	        	<img src="../imagens/excluir.gif" onClick="excluirAnexo('<?php echo $dados['arqid']; ?>');" style="border:0; cursor:pointer;" title="Excluir Documento Anexo">
	        <? } else { ?>
	        	<img src="../imagens/alterar_01.gif" style="border:0; cursor:pointer;" title="Alterar Descrição Anexo">
	        	<img src="../imagens/excluir_01.gif" style="border:0; cursor:pointer;" title="Excluir Documento Anexo">
	        <? } ?>
	        </td>
		   	</tr>
	    <?php
	    $count++;
	    }
	} else {
		echo '<tr>
		    	<td style="color: red">Nenhum documento disponível!</td>
		    </tr>';
	}
    ?>
</table>
</form>
</body>
<script type="text/javascript">
var objForm = document.forms["formulario"];

if(objForm.arqid.value != ''){
	objForm.arquivo.disabled = true;
}

function validaFormAnexos(){
	var alerta = "";
	if(objForm.arquivo.value == '' && objForm.arqid.value == ''){
		alert("Você deve escolher um arquivo.");
		return false;
	}
	var erro = 0;
	$("[class~=obrigatorio]").each(function() { 
		if(!this.value){
			erro = 1;
			alert('Favor preencher todos os campos obrigatórios!');
			this.focus();
			return false;
		}
	});
	if(erro == 0){
		//document.getElementById('requisicao').value = 'salvar';
		$("#formulario").submit();
	}
}
	 
function excluirAnexo( arqid, tipo ){
 	if ( confirm( 'Deseja excluir o Documento?' ) ) {
 		location.href= window.location+'&arqidDel='+arqid+'&tipo=C';
 	}
}
</script>
</html>