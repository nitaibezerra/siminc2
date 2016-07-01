<?php
ini_set('memory_limit', '1024M');

include_once APPRAIZ . "includes/classes/fileSimec.class.inc";
include_once APPRAIZ . 'includes/workflow.php';

verificaPermissao();

$usucpf = $_SESSION[usucpf];
$obConvenio = new MinutaConvenio($_SESSION["emenda"]["ptridAnalise"]);

$_SESSION["emenda"]["ptridAnalise"] = !empty( $_REQUEST["ptridAnalise"] ) ? $_REQUEST["ptridAnalise"] : $_SESSION["emenda"]["ptridAnalise"];

validaSessionPTA( $_SESSION["emenda"]["ptridAnalise"] );

$strAssunto = "TERMO DE CONVENIO e outros DOCUMENTOS para assinatura.";
	
$strMensagem = '
		Senhor Prefeito,<br><br>
		1.	<b>O CONVENIO</b>, deve ser assinado em <b>03 (três) vias</b>, colher assinatura de uma testemunha nas <b>3 (três) vias</b>, e os demais documentos <b>02 (duas)</b> vias de cada; <b><u>SEM DATAR</u></b>.<br> 

		2.	As <b>DECLARAÇÕES</b> devem ser assinadas e devolvidas juntamente com o referido CONVENIO, <b><u>SEM DATAR</u></b>.<br> 

		3.	Os demais documentos devem ser assinados em 2 (duas) vias, <b><u>SEM DATAR</u></b>.<br>

		<b>4.	Toda a documentação deve ser devolvida ao SETOR DE CONVÊNIOS/FNDE, no seguinte endereço:<br>
			&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;SBS, Quadra 02, Bloco "F",Ed. FNDE, sobreloja, sala 06 / Setor de Convênios - CEP: 70.070-929 - BRASÍLIA/DF;<br> 
			&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Aos cuidados da Srª: VERA LÚCIA THEOBALDO DE ASSIS - COORDENADORA DE CONVENIOS.<br><br>

 	Aguardamos a remessa dos documentos a esta autarquia, para darmos continuidade aos trâmites do processo no prazo de 05 (CINCO) DIAS.</b>';

////// SALVAR DOS ANEXOS
if( $_REQUEST['submeter'] != 'salvar' ){
	#manipular anexos de arquivos para envio de email
	if($_REQUEST['download'] == 'S'){
		$file = new FilesSimec();
		$arqid = $_REQUEST['arqid'];
	    $arquivo = $file->getDownloadArquivo($arqid);
	    echo"<script>window.location.href = 'emenda.php?modulo=principal/documentosAnalisePTA&acao=A';</script>";
	    exit;
	} 
	if($_FILES["arquivo"] && !$_POST['arqid']){	

		$campos	= array("ptrid"  => $_SESSION['emenda']['ptridAnalise'],
						"anxdsc" => "'{$_POST['anxdsc']}'",
						"anxtipo" => "'D'"
						);
		$file = new FilesSimec("anexo", $campos ,"emenda");
		$arquivoSalvo = $file->setUpload();
		if($arquivoSalvo){
			echo '<script type="text/javascript"> alert(" Operação realizada com sucesso!");</script>';
			echo"<script>window.location.href = 'emenda.php?modulo=principal/documentosAnalisePTA&acao=A';</script>";
			die;	
		}
	} elseif($_POST['arqid']) {
	    $sql = "UPDATE emenda.anexo SET anxdsc = '{$_POST['anxdsc']}' where arqid=".$_POST['arqid']." and anxtipo = 'D'";
	    $db->executar($sql);
	    $db->commit();
	    echo '<script type="text/javascript"> 
	    		alert("Operação realizada com sucesso!");
	    		window.location.href = "emenda.php?modulo=principal/documentosAnalisePTA&acao=A";
	    	  </script>';
	    die;
	}
	 
	if($_GET['arqidDel']){
	    $sql = "DELETE FROM emenda.anexo where arqid=".$_REQUEST['arqidDel']." and anxtipo = 'D'";
	    $db->executar($sql);
	    $sql = "UPDATE public.arquivo SET arqstatus = 'I' where arqid=".$_REQUEST['arqidDel'];
	    $db->executar($sql);
	    $db->commit();
	    
	    $file = new FilesSimec();
		$file->excluiArquivoFisico($_GET['arqidDel']);
		
	    echo '<script type="text/javascript"> 
	    		alert("Operação realizada com sucesso!");
	    		window.location.href="emenda.php?modulo=principal/documentosAnalisePTA&acao=A";
	    	  </script>';
	    die;
	}
}
////// SALVAR DOS DADOS DOS DOCUMENTOS EXTRAS 
if ($_POST['salvar1'] == '1') {
	
	if ($_REQUEST['excluir']){
		
		// Exclui o documento	
		$sql = "update emenda.documentosextras set dexstatus = 'I' where dexid = ".$_REQUEST['excluir'];
		$db->executar($sql);
	    if($db->commit()){
	    	$db->sucesso('principal/documentosAnalisePTA');
	    } else {
	    	$db->insucesso( 'Falha na operação' );
	    }
	    
	}else{
		if( $_POST['imprimirentidade'] == 'S' ){
			$campos	= array("ptrid"  => $_SESSION['emenda']['ptridAnalise'],
							"anxdsc" => "'{$_POST['dexnome']}'",
							"anxtipo" => "'D'"
							);
						
			$arParam = array( 'tabela' => 'anexo',
							  'campo' => $campos,
							  'extensao' => 'pdf',
							  'nome' => $_POST['dexnome']
						 );
			$http = new RequestHttp();
			$http->toPdfSave( $_POST['reftexto'], $arParam );
		}
		//imprimirentidade
		
		// 	Salva informações do texto
		$sql = "INSERT INTO emenda.documentosextras
					(
						ptrid,
						dextexto,
						dexnome,
						dexstatus,
						usucpfinclusao,
						dexdatainclusao,
						mdoid
					)
					VALUES
					(
						".$_SESSION['emenda']['ptridAnalise'].",
						'$_POST[reftexto]',
						'$_POST[dexnome]',
						'A',
						'$usucpf',
						NOW(),
						'$_POST[mdoid]'
					)";

		$db->executar($sql);
		if($db->commit()){
	    	$db->sucesso('principal/documentosAnalisePTA');
	    } else {
	    	$db->insucesso( 'Falha na operação' );
	    }
	}	
} else if($_REQUEST['mdoid'] != "") {
	
	$sql = "SELECT mdoconteudo FROM emenda.modelosdocumentos WHERE mdostatus = 'A' AND mdoid = ".$_REQUEST['mdoid'];
	
	$dados = $db->carregar($sql);
	$imitexto = $dados[0]['mdoconteudo'];
	
	if($imitexto){
		$obConvenio->alteraMacrosConvenio($imitexto, 0);
	}
	else{
		$imitexto = "Não existem informações sobre esta minuta.";
	}
	
	echo $imitexto;
	
	exit;
}


include  APPRAIZ."includes/cabecalho.inc";
print "<br/>";

$boReformulacao = pegaPaiPTA( $_SESSION["emenda"]["ptridAnalise"] );
$boReformulacao = ( !empty($boReformulacao) ? true : false );

montaAbasAnalise( $abacod_tela, $url, '' , $estadoAtual, $boReformulacao);

monta_titulo( 'Documentos', '');

echo cabecalhoPlanoTrabalho($_SESSION["emenda"]["ptridAnalise"], true);

montaVisualizarPTA( $_SESSION["emenda"]["ptridAnalise"], false, $boReformulacao );

$arPerfil = pegaPerfilArray( $_SESSION['usucpf'] );

$permissao = verificaPermissaoPerfil( 'analise', 'boolean' );
?>

<script type="text/javascript" src="/includes/prototype.js"></script>
<script type="text/javascript" src="js/emenda.js"></script>
<script language="javascript" type="text/javascript" src="../includes/tinymce/tiny_mce.js"></script>
<script type="text/javascript">
	tinyMCE.init({
		// General options
		mode : "textareas",
		theme : "advanced",
		language: "pt",
		editor_selector : "emendatinymce",
		plugins : "safari,pagebreak,style,layer,table,save,advhr,advimage,advlink,emotions,iespell,inlinepopups,insertdatetime,preview,media,searchreplace,print,contextmenu,paste,directionality,fullscreen,noneditable,visualchars,nonbreaking,xhtmlxtras,template,wordcount",

		// Theme options
		theme_advanced_buttons1 : "bold,italic,underline,strikethrough,|,justifyleft,justifycenter,justifyright,justifyfull,|,formatselect,fontselect,fontsizeselect",
		theme_advanced_buttons2 : "cut,copy,paste,pastetext,pasteword,|,search,replace,|,bullist,numlist,|,outdent,indent,blockquote,|,undo,redo,|,link,unlink,anchor,image,cleanup,help,code,|,insertdate,inserttime,preview,|,forecolor,backcolor",
		theme_advanced_buttons3 : "tablecontrols,|,hr,removeformat,visualaid,|,sub,sup,|,charmap,emotions,iespell,media,advhr,|,print,|,ltr,rtl,|,fullscreen",
		theme_advanced_buttons4 : "insertlayer,moveforward,movebackward,absolute,|,styleprops,|,cite,abbr,acronym,del,ins,attribs,|,visualchars,nonbreaking,template,pagebreak",
		theme_advanced_toolbar_location : "top",
		theme_advanced_toolbar_align : "left",
		theme_advanced_statusbar_location : "",
		theme_advanced_resizing : true,

		// Example content CSS (should be your site CSS)
		content_css : "css/content.css",

		// Drop lists for link/image/media/template dialogs
		template_external_list_url : "lists/template_list.js",
		external_link_list_url : "lists/link_list.js",
		external_image_list_url : "lists/image_list.js",
		media_external_list_url : "lists/media_list.js",

		// Replace values for the template plugin
		template_replace_values : {
			username : "Some User",
			staffid : "991234"
		}
	});
</script>
<?php    
$sql = "select anx.anxid,
			   anx.arqid,
			   anx.ptrid,
			   anx.anxdsc,
			   arq.arqnome || '.' || arq.arqextensao as arquivo
		from emenda.anexo anx
			inner join public.arquivo arq on anx.arqid = arq.arqid
    	where ptrid = {$_SESSION['emenda']['ptridAnalise']} and anx.anxtipo = 'D'";
    
$arDados = $db->carregar($sql);
$arDados = ($arDados ? $arDados : array());
?>
<form name="formulario" id="formulario" method="post" enctype="multipart/form-data">
<input type="hidden" name="arqid" id="arqid" value="<?php echo $_GET['arqid'] ?>" />
<input type="hidden" name="strAssunto" id="strAssunto" value="<?php echo $strAssunto; ?>" />
<input type="hidden" name="strMensagem" id="strMensagem" value="<?php echo simec_htmlentities($strMensagem); ?>" />
<input type="hidden" name="ptridchave" id="ptridchave" value="<?php echo $_SESSION['emenda']['ptridAnalise']; ?>" />
<input type="hidden" name="emailentidade" id="emailentidade" value="true" />
<input type="hidden" name="submeter" id="submeter" value="1">
<input type="hidden" name="requisicao" id="requisicao" value="">

<table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="4" style="border-bottom:none;">	
	<tr>
		<td>
<table align="center" class="tabela" bgcolor="#f5f5f5" cellspacing="1" cellpadding="4">
	<?if( !empty($arDados) && $permissao ){ ?>
	<tr>
		<td><a href="" onclick="enviarEmailAnaliseEntidade();">Enviar e-mail para Responsável da Entidade</a></td>
	</tr>
	<?}?>
	<tr>
		<th colspan = 3>Lista de arquivos que estará disponível no plano de trabalho para a impressão da entidade</th>
	</tr>
	<tr>
	    <th>Arquivo</th>
	    <th>Descrição</th>
	    <th>Ação</th>
    </tr>
    <tr>
        <td align="left">
            <input type="file" name="arquivo" id="arquivo" <?=($_REQUEST['status'] == 'alterar' || !$permissao ? 'disabled="disabled"' : ''); ?> />
            <img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif"/>
        </td>
        <td align="left">
        	<?
			if($_GET['arqid']){
				$anxdsc = $db->pegaUm("select anxdsc from emenda.anexo where arqid = {$_GET['arqid']}");
			}
        	echo campo_texto( 'anxdsc', 'S', ($desabilita ? 'N' : 'S'), 'Descrição Anexo', 100, 100, '', '','','','','id="anxdsc"');
        	?>
        </td>
        <td align="center">
        	<input type="button" name="botao" value="Salvar" onclick="validaFormAnaliseAnexos();" <?=verificaPermissaoPerfil('analise', '', '', true); ?> />
        </td>
    </tr>
    <?php    
    $count = 1;    
    if($arDados) {
	    foreach($arDados as $dados){
	    ?>
		<tr>		    
	        <td align="left">
	        	<?php echo $count.' - '; ?><a style="cursor: pointer; color: blue;" onclick="window.location='?modulo=principal/documentosAnalisePTA&acao=A&download=S&arqid=<?php echo $dados['arqid'];?>&tipo=D'"><?php echo $dados['arquivo'];?></a>
	        </td>
	        <td align="left">
	        	<?php echo $dados['anxdsc']; ?>
	        </td>
	        <td align="center">
	        <? if( !in_array( CONSULTA_GERAL, $arPerfil ) && $permissao ){?>
	        	<img src="../imagens/alterar.gif" onClick="window.location.href='emenda.php?modulo=principal/documentosAnalisePTA&acao=A&arqid=<?php echo $dados['arqid'];?>&tipo=D&status=alterar'" style="border:0; cursor:pointer;" title="Alterar Descrição Anexo">
	        	<img src="../imagens/excluir.gif" onClick="excluirAnexoAnalise('<?php echo $dados['arqid']; ?>','D');" style="border:0; cursor:pointer;" title="Excluir Documento Anexo">
	        <? } else { ?>
	        	<img src="../imagens/alterar_01.gif" style="border:0; cursor:pointer;" title="Alterar Descrição Anexo">
	        	<img src="../imagens/excluir_01.gif" style="border:0; cursor:pointer;" title="Excluir Documento Anexo">
	        <? } ?>
		        </td>
		   	</tr>
	    <?php
	    $count++;
	    }
	}
    ?>
</table>
<br><br>
</form>
<form name="formulariominuta" id="formulariominuta" method="post">
<input type="hidden" name="salvar1" id="salvar1" value="<?php echo $_POST['salvar1'] ?>" />
<input type="hidden" name="excluir" id="excluir" value="" />
<!-- DADOS DOS DOCUMENTOS EXTRAS  -->
<table align="center" class="tabela" bgcolor="#f5f5f5" cellspacing="1" cellpadding="4">
	<tr>
		<th colspan = 3>Geração de Documentos Extras do Plano de Trabalho</th>
	</tr>
	<tr>
        <td class="SubTituloDireita" style="width: 19%;">
            <b>Modelo do documento utilizado:</b>
        </td>
        <td>
	        <select name="mdoid" id="mdoid" onchange="carregaTextoDoc();"><option value='0'>Escolher</option>
		    <?php 
		    $arDados = array();
		    
		    	$sql = "select modelosdocumentos.mdoid, modelosdocumentos.mdonome
							from emenda.modelosdocumentos
				     		where modelosdocumentos.mdostatus = 'A'";
			    
			    $arDados = $db->carregar($sql);
		    
		    $count = 1;		    
		    if($arDados) {
		    	foreach($arDados as $dados){
		    		($dadosReformulacao[0][mdoid] == $dados[mdoid]) ? $selected = ' selected' : $selected = '';
		    		
		    		echo "<option value='{$dados[mdoid]}'$selected>{$dados[mdonome]}</option>";
		    	}
		    }
		    
		    $imitexto = "{$dadosReformulacao[0][reftexto]}";
	        ?>
	        </select>
        </td>    
    </tr>
    <tr>
    	<td class="SubTituloDireita" style="width: 19%;">
            <b>Nome documento:</b>
        </td>
    	<td>
        	<?
			echo campo_texto( 'dexnome', 'S', ($desabilita ? 'N' : 'S'), 'Nome Documento', 100, 500, '', '','','','','id="dexnome"');
        	?>
        </td>
    </tr>
    <tr>
    	<td class="SubTituloDireita" style="width: 19%;">
            <b>Documento disponível para impressão para entidade?:</b>
        </td>
    	<td><input type="checkbox" name="imprimirentidade" id="imprimirentidade" value="S" ></td>
    </tr>
    <tr>
	    <td colspan='2' height='300'>
			<div>
				<textarea id="texto" name="reftexto" rows="90" cols="80" style="width:100%;height:400px;" class="emendatinymce"><?=$imitexto?></textarea>
			</div>		
	    </td>
    </tr>
    <tr>
		<td align="center" bgcolor="#c0c0c0" colspan="4">
			<input type="button" id="bt_salvar" value="Salvar" onclick="salvarRegistro();" <?=verificaPermissaoPerfil('analise', '', '', true); ?> />
			<input type="button" id="bt_cancelar" value="Cancelar" onclick="history.back(-1);" />
		</td>
	</tr>
	<tr>
    	<td colspan=3>
	    	<table align='left'>
	    	
	    	<?php 	
	  
			$sql = "select de.dexid, de.dexnome, md.mdonome, to_char(de.dexdatainclusao,'dd/mm/YYYY HH24:MI') AS dexdatainclusao, md.mdonome 
				from emenda.documentosextras de
				inner join emenda.modelosdocumentos md on de.mdoid = md.mdoid  
				where ptrid = ".$_SESSION["emenda"]["ptridAnalise"]."
				and dexstatus = 'A'
				order by de.dexid";
			           
		    if ($arDados = $db->carregar($sql)) {
		    	foreach ($arDados as $dados) {
		    		$htmlConsulta = $dados[dexnome] ." - ". $dados[mdonome] ." - ". $dados[dexdatainclusao];
		    		if( !in_array( CONSULTA_GERAL, $arPerfil ) ){
						echo "<tr>
								<td width='50'>
									<img src='../imagens/consultar.gif' onclick=\"window.open('emenda.php?modulo=principal/exibeDocumento&acao=A&nometabela=emenda.documentosextras&nomecampo=dextexto&nomeid=dexid&id=$dados[dexid]', 'JANELA', 'height=600, width=700, status=0, menubar=1, toolbar=0, scrollbars=1')\">
									<img src='../imagens/excluir.gif' onclick='excluirDoc($dados[dexid]);'>
								</td>
								<td>$htmlConsulta</td>
							</tr>";
		    		} else {
		    			echo "<tr>
								<td width='50'><img src='../imagens/excluir_01.gif'></td>
								<td>$htmlConsulta</td>
							</tr>";
		    		}
		    	}
		    }    	
	    	?>
	    	
	    	</table>
    	</td>
    
    </tr>	
</table>




</td>
<td><?
	$docid = criarDocumento( $_SESSION["emenda"]["ptridAnalise"] );
	wf_desenhaBarraNavegacao( $docid , array( 'url' => $_SESSION['favurl'], 'ptrid' => $_SESSION["emenda"]["ptridAnalise"], 'tipo' => "" ) );
?></td>
</tr>
</table>
</form>
<div id="erro"></div>
</body>
<?php
	$arFluxo = verificaFluxo( $_SESSION["emenda"]["ptridAnalise"] );
	
	if( $arFluxo ){
		$fluxo = implode(', ', $arFluxo);
	}
?>
</body>
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<script type="text/javascript">

jQuery.noConflict();
jQuery(document).ready(function(){
	jQuery.each([ <?=$fluxo ?> ], function( index, value ) {
		jQuery('#td_acao_'+value).css('display', 'none');
	});
});


var objForm = document.forms["formulario"];

function validaFormAnaliseAnexos(){
	var alerta = "";
	if(objForm.arquivo.value == '' && objForm.arqid.value == ''){
		alerta += "Você deve escolher um arquivo.\n";
	}
	if(objForm.anxdsc.value == ''){
		alerta += "O campo Descrição Anexo é Obrigatório.";
	}
	
	if(alerta){
		alert(alerta);
		return false;
	}
	objForm.submit();
}
	 
function excluirAnexoAnalise( arqid, tipo ){
 	if ( confirm( 'Deseja excluir o Documento?' ) ) {
 		location.href= window.location+'&arqidDel='+arqid+'&tipo=D';
 	}
}

function carregaTextoDoc(){
	
	var myajax = new Ajax.Request('emenda.php?modulo=principal/documentosAnalisePTA&acao=A', {
		        method:     'post',
		        parameters: '&mdoid='+document.getElementById('mdoid').value,
		        asynchronous: false,
		        onComplete: function (res){
		        	var ed = tinyMCE.get('texto');
		        	ed.setContent(res.responseText);					
		        }
		  });
}

function salvarRegistro ()
{

	if (document.getElementById('mdoid').value == '0')
	{
		alert('O campo Modelo do documento é um campo obrigatório');
		return false;
	}
	if (document.getElementById('dexnome').value == '')
	{
		alert('O campo nome do documento é um campo obrigatório');
		return false;
	}
	document.getElementById('salvar1').value = '1';
	document.formulariominuta.submit();
}

function excluirDoc(dexid){
	if (confirm('Deseja realmente excluir este documento?'))
	{
		document.getElementById('salvar1').value = '1';
		document.getElementById('excluir').value = dexid;
		document.formulariominuta.submit();	
	}else{
		return false;
	}	
}

function enviarEmailAnaliseEntidade(){
	var largura = 673;
	var altura = 500;
	//pega a resolução do visitante
	w = screen.width;
	h = screen.height;
	
	//divide a resolução por 2, obtendo o centro do monitor
	meio_w = w/2;
	meio_h = h/2;
	
	//diminui o valor da metade da resolução pelo tamanho da janela, fazendo com q ela fique centralizada
	altura2 = altura/2;
	largura2 = largura/2;
	meio1 = meio_h-altura2;
	meio2 = meio_w-largura2;
	
	var form = document.getElementById('formulario');
	document.getElementById('requisicao').value = 'filtraemail';
	
	form.action = "emenda.php?modulo=principal/enviarEmailEmenda&acao=A";
	form.target = 'page';
	var janela = window.open('emenda.php?modulo=principal/enviarEmailEmenda&acao=A&tipoEmail=analisePTA','page','height=' + altura + ', width=' + largura + ', top='+meio1+', left='+meio2+'');
	janela.focus();
	form.submit();
}
</script>
</html>