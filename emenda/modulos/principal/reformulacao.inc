<?php
include_once APPRAIZ . "includes/classes/fileSimec.class.inc";
include_once APPRAIZ . 'includes/workflow.php';

if( $_GET['acao'] == 'C' ){
	$ptridAnalise = ($_REQUEST['ptrid'] ? $_REQUEST['ptrid'] : $_SESSION["emenda"]["ptrid"]);
	$_SESSION["emenda"]["ptridAnalise"] = $ptridAnalise;
	$_SESSION["emenda"]["ptrid"] = $ptridAnalise;
	$obEmenda = new Emenda();
	$federal = $obEmenda->buscaEmendaDescentraliza( $ptridAnalise );
	$_SESSION['emenda']['federal'] = $federal;
} else {
	$_SESSION["emenda"]["ptridAnalise"] = !empty( $_REQUEST["ptridAnalise"] ) ? $_REQUEST["ptridAnalise"] : $_SESSION["emenda"]["ptridAnalise"];	
	if (!$_SESSION["emenda"]["ptridAnalise"] && $_REQUEST[ptrid]) $_SESSION["emenda"]["ptridAnalise"] = $_REQUEST[ptrid];	
	$ptridAnalise = $_SESSION["emenda"]["ptridAnalise"];
}

validaSessionPTA( $_SESSION["emenda"]["ptridAnalise"] );

verificaPermissao();

$usucpf = $_SESSION[usucpf];

if( $_GET['action'] == 'E' && !empty($_GET['refid']) ){
	if( $_GET['refsituacao'] ){
		if( $_GET['refsituacao'] == 'A' && $estadoAtual == EM_SOLICITACAO_REFORMULACAO ){
			$sql = "UPDATE emenda.planotrabalho SET 
					refid = NULL, ptrdataalteracao = NULL
					, usucpfalteracao = NULL 
					WHERE refid = ".$_GET['refid'];
			$db->executar($sql);
			$sql = "DELETE FROM emenda.ptminreformulacao WHERE refid = ".$_GET['refid']." and refsituacao = 'A'";
		} else {	
			$sql = "UPDATE emenda.ptminreformulacao SET refstatus = 'I', refdscanalise = '{$_POST['refdscanalise']}', refsituacaoanalise = '{$_POST['refsituacaoanalise']}', refdataalteracao = now(), usucpfalteracao = '". $usucpf ."'
					WHERE refid = ".$_GET['refid']." and refsituacao = 'A'";
		}		
		$db->executar( $sql );
		
		if( empty($_FILES["arquivo"]["name"]) ){
			$db->commit();
			$db->sucesso( 'principal/reformulacao' );
		}
	} else {
		echo "<script>
				alert('Não é permitido realizar a exclução de uma reformulação com situação de GERAÇÃO DO TERMO ADITIVO');
				window.location.href = 'emenda.php?modulo=principal/reformulacao&acao=".$_GET['acao']."';
			  </script>";
		exit();
	}	
}

if( $_POST['submetido'] == '1' ){
	
	$_POST['prorrogacaooficio'] = 'N';
	if( is_array($_POST['reformulacao']) ){
		if( in_array( PRORROGRACAO_DE_OFICIO, $_POST['reformulacao'] ) ) $_POST['prorrogacaooficio'] = 'S';
	}
	
	if( !empty($_POST['refid']) ){
		$sql = "UPDATE emenda.ptminreformulacao SET 
					usucpfalteracao = '". $usucpf ."',
					refdataalteracao = now(),
					refsituacao = 'A',
					refsituacaoanalise = '{$_POST['refsituacaoanalise']}',
					refdscanalise = '{$_POST['refdscanalise']}',
					refdsc = '". $_POST['refdsc'] ."',
					refprorrogacaooficio = '". $_POST['prorrogacaooficio'] ."'
				WHERE refid = ". $_REQUEST['refid'];
		
		$db->executar($sql);
		$refid = $_POST['refid'];
	} else {
		$db->executar("update emenda.ptminreformulacao set refstatus = 'I' where ptrid = {$_SESSION["emenda"]["ptridAnalise"]}");
		$sql = "INSERT INTO emenda.ptminreformulacao(
							ptrid, refstatus, usucpfinclusao, refdatainclusao, refsituacaoanalise, refdscanalise,
							pmcid, refsituacao, refdsc, refprorrogacaooficio) 
				VALUES ('$ptridAnalise', 'A', '$usucpf', now(), '{$_POST['refsituacaoanalise']}', '{$_POST['refdscanalise']}',
							(SELECT pmcid FROM 
								emenda.ptminutaconvenio 
							WHERE ptrid = '$ptridAnalise' 
								AND pmcstatus = 'A'), 'A', '$_POST[refdsc]', '{$_POST['prorrogacaooficio']}') RETURNING refid";						
		
		$refid = $db->pegaUm($sql);		
	}
	
	$db->executar("DELETE FROM emenda.reformulatipos WHERE refid = $refid");
			
	if( is_array($_POST['reformulacao']) ){
		foreach ($_POST['reformulacao'] as $tipo) {
			$sql = "INSERT INTO emenda.reformulatipos(trefid, refid) 
					VALUES ($tipo, $refid)";
			
			$db->executar($sql);
		}
	}
	
	if( empty($_FILES["arquivo"]["name"]) ){
		$db->commit();
		$db->sucesso( 'principal/reformulacao' );
	}
}

if($_REQUEST['download'] == 'S'){
	$file = new FilesSimec();
	$arqid = $_REQUEST['arqid'];
    $arquivo = $file->getDownloadArquivo($arqid);
    $db->sucesso( 'principal/reformulacao' );
    /*echo"<script>window.location.href = 'emenda.php?modulo=principal/reformulacao&acao=".$_GET['acao']."';</script>";
    exit;*/
}

if($_GET['arqidDel']){
    $sql = "DELETE FROM emenda.anexo where arqid=".$_REQUEST['arqidDel'];
    $db->executar($sql);
    $sql = "UPDATE public.arquivo SET arqstatus = 'I' where arqid=".$_REQUEST['arqidDel'];
    $db->executar($sql);
    $db->commit();
    
    $file = new FilesSimec();
	$file->excluiArquivoFisico($_GET['arqidDel']);
	
	$db->sucesso( 'principal/reformulacao' );
    /*echo '<script type="text/javascript"> 
    		alert("Operação realizada com sucesso!");
    		window.location.href="emenda.php?modulo=principal/reformulacao&acao='.$_GET['acao'].'";
    	  </script>';
    die;*/
}

if( $_FILES["arquivo"] && $_FILES["arquivo"]["name"] && !$_POST['arqid']){
	
	$refid = ( empty($refid) ? $_POST['refid'] : $refid );
	
	$campos	= array("ptrid"  => $_SESSION['emenda']['ptridAnalise'],
				"anxdsc" => "'{$_POST['anxdsc']}'",
				"anxtipo" => "'R'",
				"refid" => "'{$refid}'"
				);
	
	$file = new FilesSimec("anexo", $campos ,"emenda");
	$arquivoSalvo = $file->setUpload();
	if($arquivoSalvo){
		echo '<script type="text/javascript"> alert(" Operação realizada com sucesso!");</script>';
		echo"<script>window.location.href = 'emenda.php?modulo=principal/reformulacao&acao=".$_GET['acao']."';</script>";
		die;	
	}
} elseif( $_POST['arqid'] ) {
	$anxdsc = $_POST['anxdsc'];
	$arqid = $_POST['arqid'];
	
    $sql = "UPDATE emenda.anexo SET anxdsc = '{$anxdsc}' where arqid=".$arqid;
    $db->executar($sql);
    $db->commit();
    $db->sucesso( 'principal/reformulacao' );
  /*  echo '<script type="text/javascript"> 
    		alert("Operação realizada com sucesso!");
    		window.location.href = "emenda.php?modulo=principal/reformulacao&acao='.$_GET['acao'].'";
    	  </script>';
    die;*/
}

$sql = "SELECT pr.refid, pr.refdsc, pr.refsituacaoanalise, pr.refdscanalise, pr.refprorrogacaooficio
		FROM emenda.ptminreformulacao pr
		WHERE pr.ptrid = $ptridAnalise 
			and pr.refsituacaoreformulacao = 'C' 
		    and pr.refstatus = 'A'";

$arRef = $db->pegaLinha( $sql );

$refid = $arRef['refid'];
$arTrefid = array();
if( $refid ) $arTrefid = $db->carregarColuna("select trefid from emenda.reformulatipos where refid = $refid");

$refdsc = $arRef['refdsc'];
$refdscanalise = $arRef['refdscanalise'];
$refsituacaoanalise = trim($arRef['refsituacaoanalise']);
$refprorrogacaooficio = trim($arRef['refprorrogacaooficio']);

include  APPRAIZ."includes/cabecalho.inc";
print "<br/>";

$boReformulacao = pegaPaiPTA( $_SESSION["emenda"]["ptridAnalise"] );
$boReformulacao = ( !empty($boReformulacao) ? true : false );

if( $_GET['acao'] == 'C' ){
	echo exibeInstituicaoBenefiada( $_SESSION["emenda"]["enbid"] );
	print "<br/>";
}

$estadoAtual = pegarEstadoAtual( $_SESSION["emenda"]["ptridAnalise"] );
montaAbasAnalise( $abacod_tela, $url, '' , $estadoAtual, $boReformulacao);
monta_titulo( 'Reformulação', '');

echo cabecalhoPlanoTrabalho($_SESSION["emenda"]["ptridAnalise"], true);

montaVisualizarPTA( $_SESSION["emenda"]["ptridAnalise"], true, $boReformulacao );

$titulo = ("<b>Reformulação do Convênio</b>");

monta_titulo( '', $titulo);

if( $refsituacaoanalise == 'F' || empty($refsituacaoanalise) || $refsituacaoanalise == 'F' ){
	$display = "display: none";
} else {
	$display = "display: ''";
}

$arPerfil = pegaPerfilArray( $_SESSION['usucpf'] );

$desabilitaAnalise = '';
$desablitaJust = 'S';
if( !in_array( INSTITUICAO_BENEFICIADA, $arPerfil ) ){
	$bohabilita = 'true';
} else {
	if( $refsituacaoanalise == 'E' ){
		$bohabilita = 'true';
		$desabilitaAnalise = 'disabled="disabled"';
		$desablitaJust = 'N';
	} else {
		$bohabilita = 'false';
	}
}

/*if( (!in_array( INSTITUICAO_BENEFICIADA, $arPerfil ) || in_array( SUPER_USUARIO, $arPerfil )) && ($estadoAtual == EM_SOLICITACAO_REFORMULACAO || $estadoAtual == EM_ACEITACAO_REFORMULACAO) || $refsituacaoanalise == 'E' ){
	$bohabilita = 'true';
	if( $refsituacaoanalise == 'E' ){
		$desabilitaAnalise = 'disabled="disabled"';
	}
} else {
	if( $refsituacaoanalise == 'E' ){
		$desabilitaAnalise = 'disabled="disabled"';
	}
	$bohabilita = 'false';
}*/

?>
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<script type="text/javascript" src="/includes/prototype.js"></script>
<script type="text/javascript" src="js/emenda.js"></script>
<form id="formReformulacao" name="formReformulacao" method="post" action="emenda.php?modulo=principal/reformulacao&acao=<?=$_GET['acao'];?>."" enctype="multipart/form-data">
<input type="hidden" name="submetido" id="submetido" value="1" />
<input type="hidden" name="refid" id="refid" value="<?echo $refid;?>" />
<input type="hidden" name="arqid" id="arqid" value="<?php echo $_GET['arqid'] ?>" />
<input type="hidden" name="bohabilita" id="bohabilita" value="<?=$bohabilita; ?>" />

<table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="4" style="border-bottom:none;">
<td>
    <table align="center" class="tabela" bgcolor="#f5f5f5" cellspacing="1" cellpadding="4">
    <?if( $estadoAtual == EM_ACEITACAO_REFORMULACAO && in_array( INSTITUICAO_BENEFICIADA, $arPerfil ) ){ ?>
    <tr>
        <td class="SubTituloDireita"><b>Status de aceitação da reformulação do projeto:</b></td>
        <td>Aguardando validação do FNDE...</td>
    </tr>
    <?} ?>
    <tr>
        <td class="SubTituloDireita" style="width: 25%;"><b>Justificativa:</b></td>
        <td><?=campo_textarea('refdsc', 'S', 'S', 'Descrição da Reformulação', 150, 15, 2000, '', '', '', '', 'Descrição da Reformulação');?></td>
    </tr>
    <?if($bohabilita == 'true'){ ?>
    <tr>
		<td class="subtitulodireita"><b>Análise:</b></td>
		<td>
			<label for="anasituacao1"><input name="refsituacaoanalise" id="refsituacaoanalise1" checked="checked" <?=($refsituacaoanalise == 'F' ? 'checked="checked"' : ''); echo $desabilitaAnalise; ?> onclick="verificaAnaliseSelecionada(this.value)" value="F" type="radio">Aprovado</label>
			<label for="anasituacao2"><input name="refsituacaoanalise" id="refsituacaoanalise2" <?=($refsituacaoanalise == 'E' ? 'checked="checked"' : ''); echo $desabilitaAnalise; ?> onclick="verificaAnaliseSelecionada(this.value)" value="E" type="radio">Em diligência</label>
			<label for="anasituacao3"><input name="refsituacaoanalise" id="refsituacaoanalise3" <?=($refsituacaoanalise == 'D' ? 'checked="checked"' : ''); echo $desabilitaAnalise; ?> onclick="verificaAnaliseSelecionada(this.value)" value="D" type="radio">Indeferido</label>
			<img title="Indica campo obrigatório." src="../imagens/obrig.gif" border="0">
		</td>
	</tr>
	<tr id="justanalise" style="<?=$display; ?>">
		<td class="subtitulodireita"><b>Justificativa da diligência:</b></td>
		<td><?=campo_textarea('refdscanalise', 'S', $desablitaJust, 'Justificativa da Análise', 80, 5, 500, '', '', '', '', 'Justificativa da Análise');?></td>
	</tr>
	<tr>
		<td class="subtitulodireita"><b>Tipos de Reformulações Pré-Definidas:</b></td>
		<td>
			<table class="tabela" align="left" cellspacing="1" cellpadding="4" style="width: 50%">
				<tr>
					<th>Ação</th>
					<th><b>Reformulações</b></th>
				</tr>
				<tr>
					<td align="center"><input type="checkbox" value="8" id="reformulacao" name="reformulacao[]" <?=( in_array(PRORROGRACAO_DE_OFICIO, $arTrefid) ? 'checked="checked"' : '');?> /></td>
					<td><b>Prorrogação de Ofício</b></td>
				</tr>
				<tr>
					<td align="center"><input type="checkbox" value="9" id="reformulacao" name="reformulacao[]" <?=(in_array(APOSTILAMENTO, $arTrefid) ? 'checked="checked"' : '');?> /></td>
					<td><b>Apostilamento</b></td>
				</tr>
				<tr>
					<td align="center"><input type="checkbox" value="11" id="reformulacao" name="reformulacao[]" <?=(in_array(ALTERACAO_DE_ITENS_DA_ESPECIFICACAO, $arTrefid) ? 'checked="checked"' : '');?> /></td>
					<td><b>Alteração de Itens da Especificação</b></td>
				</tr>
				<tr>
					<td align="center"><input type="checkbox" value="10" id="reformulacao" name="reformulacao[]" <?=(in_array(RENDIMENTO_DE_APLICACAO, $arTrefid) ? 'checked="checked"' : '');?> /></td>
					<td><b>Rendimento de Aplicação</b></td>
				</tr>
			</table>
		</td>
	</tr>
	<?} ?>
    <tr bgcolor="#DCDCDC">
        <td colspan="2"><b>Anexar Documentos</b></td>
    </tr>
	<?if( (!in_array( INSTITUICAO_BENEFICIADA, $arPerfil ) || in_array( SUPER_USUARIO, $arPerfil )) && $estadoAtual != EM_PROCESSO_REFORMULADO ){ ?>
	<tr>
		<td class="subtituloesquerda" colspan="2"><b>Necessário inseir o documento referente a solicitação da reformulação.</b></td>
	</tr>
	<tr>
		<td colspan="2" style="text-align: left"><a href="emenda.php?modulo=principal/listaDeExtraOrcamento&acao=<?=$_GET['acao'] ?>" target="_blank"><b>Deseja inserir complemento orçamentário?</b></a></td>
	</tr>
	<tr>
		<td colspan="2">&nbsp;</td>
	</tr>
	<?} ?>
    <tr>
		<td colspan="2">
			<table class="listagem" width="95%" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" align="center" style="border-left: 1px solid rgb(255, 255, 255); border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192);">
			    <tr>
				    <th>Arquivo</th>
				    <th>Descrição</th>
				    <th>Ação</th>
			    </tr>
			    <tr>
			        <td align="left">
			            <input type="file" name="arquivo" id="arquivo" />
			        </td>
			        <td align="left">
			        	<?
						if($_GET['arqid'] && empty($_GET['tipo']) ){
							$anxdsc = $db->pegaUm("select anxdsc from emenda.anexo where arqid = {$_GET['arqid']}");
						}
			        	echo campo_texto( 'anxdsc', 'N', $habilitado, 'Descrição Anexo', 100, 100, '', '','','','','id="anxdsc"'); ?>
			        </td>
			        <td align="center">
			        	<input type="button" name="botao" value="Salvar Anexos" onclick="validaForm(); <?=disabled($estadoAtual); ?>" />
			        </td>
			    </tr>
			    <? if( !empty($refid) )montaListaArquivosAnexados( $_SESSION['emenda']['ptridAnalise'], $refid, $estadoAtual ); ?>
			 </table>
		</td>
	</tr>
    </table>

	<table align="center" class="tabela" bgcolor="#f5f5f5" cellspacing="1" cellpadding="4">
	<tr>
		<td align="center" bgcolor="#c0c0c0" colspan="4">
			<?
			/*$sql = "SELECT refsituacaoreformulacao FROM emenda.ptminreformulacao WHERE refsituacaoreformulacao = 'C' AND refstatus = 'A' AND ptrid = ".$ptridAnalise;
			$testaSit = $db->pegaUm( $sql );*/
			if( $estadoAtual == EM_SOLICITACAO_REFORMULACAO || $estadoAtual == EM_REFORMULACAO_PROCESSO ){					
				echo '<input type="button" id="bt_salvar" value="Salvar" onclick="salvarRegistro();"/>&nbsp;
					  <input type="button" id="bt_cancelar" value="Cancelar" onclick="cancelarReformulacao();" />';	
			} else if( $estadoAtual == EM_ACEITACAO_REFORMULACAO && in_array( INSTITUICAO_BENEFICIADA, $arPerfil ) ){
				echo '<input type="button" id="bt_salvar" value="Salvar" disabled="disabled"/>&nbsp;
					  <input type="button" id="bt_cancelar" value="Cancelar" disabled="disabled" />';
			} else if( $estadoAtual == EM_ACEITACAO_REFORMULACAO && (in_array( ANALISTA_FNDE, $arPerfil ) || in_array( SUPER_USUARIO, $arPerfil )) ){
				echo '<input type="button" id="bt_salvar" value="Salvar" onclick="salvarRegistro();"/>&nbsp;
					  <input type="button" id="bt_cancelar" value="Cancelar" onclick="cancelarReformulacao();" />';
			} else {
			 	echo '<input type="button" id="bt_salvar" value="Salvar" disabled="disabled"/>&nbsp;
					  <input type="button" id="bt_cancelar" value="Cancelar" disabled="disabled" />';
			}			
			?>
		</td>
	</tr>
    <tr>
    	<td>
    	<table align='left'>   	
    	</table>
    	</td>
    </tr>	
	</table>

	
</td>
<td width='100' align="center">
<?	
	$docid = criarDocumento( $_SESSION["emenda"]["ptridAnalise"] );
	$boHistorico = array();
	if( in_array( INSTITUICAO_BENEFICIADA, $arPerfil ) ){
		$boHistorico = array('historico' => true);
	}
	validaEnvioReformulacao( $_SESSION['favurl'], $_SESSION["emenda"]["ptridAnalise"]);
	wf_desenhaBarraNavegacao( $docid , array( 'url' => $_SESSION['favurl'], 'ptrid' => $_SESSION["emenda"]["ptridAnalise"], 'refid'=> $refid, 'tipo' => ""),$boHistorico );
?>

<?php
	$arFluxo = verificaFluxo( $_SESSION["emenda"]["ptridAnalise"] );
	
	if( $arFluxo ){
		$fluxo = implode(', ', $arFluxo);
	}
?>
</td>
</table>    
</form>
<table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="4" >
<tr>
	<th width="40%">
	<?if( in_array( ADMINISTRADOR_INST, $arPerfil ) || in_array( ADMINISTRADOR_REFORMULACAO, $arPerfil ) || in_array( SUPER_USUARIO, $arPerfil ) ){ ?>
		<input type="button" name="botao" value="Excluir Reformulação com Problema" onclick="excluirReformulacao(<?=$ptridAnalise; ?>);" />
	<?} else { ?>
		<input type="button" name="botao" value="Excluir Reformulação com Problema" disabled="disabled" />
	<?} ?></th>
	<th width="60%" style="text-align: left">Lista de Reformulação</th>
</tr>
<tr>
	<td colspan="2">
<?
	if( $_SESSION['usucpf'] == '05646593638' ){
		$cabecalho = array("Opção", "PTA id", "refid", "Descrição Reformulação", "Data Inclusão", "Status", "Situação");
	} else {
		$cabecalho = array("Opção", "Descrição Reformulação", "Data Inclusão", "Status", "Situação");
	}
	$arRegistro = montaTiposReformulacao( $_SESSION["emenda"]["ptridAnalise"], 'A' );
	$db->monta_lista_array($arRegistro, $cabecalho, 20, 4, 'N','Center');
?>
	</td>
</tr>
</table>
<script type="text/javascript">
var objForm = document.forms["formReformulacao"];

jQuery.noConflict();
jQuery(document).ready(function(){
	jQuery.each([ <?=$fluxo ?> ], function( index, value ) {
		jQuery('#td_acao_'+value).css('display', 'none');
		//$('#td_acao_445').css('display', 'none');
	});
});

if(objForm.arqid.value != ''){
	objForm.arquivo.disabled = true;
}

function excluirReformulacao( ptrid ){
	window.open('emenda.php?modulo=principal/limparReformulacao&acao=A&ptrid='+ptrid+'&popup=S','Reformulação','toolbar=no,location=no,status=yes,menubar=no,scrollbars=yes,resizable=no, height=650, width=800');
}

function validaForm(){
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
	 
function excluirAnexo( arqid ){
 	if ( confirm( 'Deseja excluir o Documento?' ) ) {
 		location.href= window.location+'&arqidDel='+arqid;
 	}
}
function anexarDocumentos( ptrid, refid ){
	window.open('emenda.php?modulo=principal/popupIncluirAnexos&acao=<?=$_GET['acao'] ?>&ptrid='+ptrid+'&refid='+refid,'page','toolbar=no,location=no,status=yes,menubar=no,scrollbars=yes,resizable=no, height=150, width=1020');
}
function cancelarReformulacao(){
	window.location.href = 'emenda.php?modulo=principal/reformulacao&acao=<?=$_GET['acao'] ?>';
}
function salvarRegistro (){
	var form = document.getElementById('formReformulacao');
	var radio = "";
	var marcadoRadio = false;
	
	if( document.getElementById('refdsc').value == '' ){
		alert('A justificativa da reformulação é de preenchimento obrigatório!');
		document.getElementById('refdsc').focus();
		return false;
	}
	if( document.getElementById('bohabilita').value == 'true' ){
		for(i=0; i<form.length; i++){
			if(form.elements[i].type == "radio"){
				if(form.elements[i].checked == true ){
					radio = form.elements[i].value;
					marcadoRadio = true
				}
			}
		}
		
		if(!marcadoRadio){
			alert('Selecione uma análise!');
			return false;
		}
	}
	document.formReformulacao.submit();
}

function verificaAnaliseSelecionada( select ){
	if(select == 'E' || select == 'D'){
		document.getElementById('justanalise').style.display = '';
	} else {
		document.getElementById('justanalise').style.display = 'none';
	}
}
</script>