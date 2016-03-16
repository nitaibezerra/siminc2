<?php
include_once APPRAIZ . "includes/classes/fileSimec.class.inc";

$empid = $_SESSION['obras2']['empid'];
$obrid = $_REQUEST['obrid'];
$pdoid = trim($_REQUEST['pdoid']);

$pflcods = array(PFLCOD_SUPER_USUARIO, PFLCOD_ADMINISTRADOR, PFLCOD_SUPERVISOR_UNIDADE, PFLCOD_GESTOR_UNIDADE);

$habilita = 'N';
if(!possui_perfil(PFLCOD_CALL_CENTER) && possui_perfil( $pflcods )){
	$habilita = 'S';
}

if($_REQUEST['carregarAnexo']){
	$sql = "SELECT 
				a.anxid,
			    a.arqid,
				ar.arqnome||'.'||ar.arqextensao as arquivo,
				a.anxdsc as descricao
			FROM 
				obras2.anexo a
			    inner join public.arquivo ar on ar.arqid = a.arqid
			WHERE
				a.pdoid = $pdoid
				and a.arqid = {$_POST['arqid']}
			    and a.anxtipo = 'PD'";
	
	$arrDados = $db->pegaLinha($sql);
	$arrDados['descricao'] = utf8_encode($arrDados['descricao']);	
	echo simec_json_encode($arrDados);
	die;
}

if($_GET['arqidDel']){
    $sql = "DELETE FROM obras2.anexo where arqid=".$_REQUEST['arqidDel']." and anxtipo = 'PD'";
    $db->executar($sql);
    $sql = "UPDATE public.arquivo SET arqstatus = 'I' where arqid=".$_REQUEST['arqidDel'];
    $db->executar($sql);
    $db->commit();
    
    $file = new FilesSimec();
	$file->excluiArquivoFisico($_GET['arqidDel']);
	
    $db->sucesso('principal/pedidoDesbloqueio', '&obrid='.$_REQUEST['obrid'].'&pdoid='.$_REQUEST['pdoid']);
    die;
}

#manipular anexos de arquivos para envio de email
if($_REQUEST['download'] == 'S'){
	$file = new FilesSimec();
	$arqid = $_REQUEST['arqid'];
    $arquivo = $file->getDownloadArquivo($arqid);
    echo"<script>window.location.href = 'obras2.php?modulo=principal/pedidoDesbloqueio&acao=A';</script>";
    exit;
}

if( $_POST['requisicao'] == 'excluirPedido' ){
	$sql = "UPDATE obras2.pedidodesbloqueioobra SET
  				pdostatus = 'I'
  			WHERE pdoid = {$_POST['pdoid']}";
	$db->executar($sql);
	
	$sql = "UPDATE obras2.anexo SET
  				anxstatus = 'I'
  			WHERE pdoid = {$_POST['pdoid']}";
	$db->executar($sql);
	
    $db->commit();
    $db->sucesso('principal/pedidoDesbloqueio', '&obrid='.$_REQUEST['obrid']);
    die;	
}

if( $_POST['requisicao'] == 'salvarPedido' ){
	$pdojustificativa = $_POST['pdojustificativa'] ? "'".substr ( $_POST['pdojustificativa'] , 0 , 4000)."'" : 'null';
	$pdoobservacao = $_POST['pdoobservacao'] ? "'".substr ( $_POST['pdoobservacao'] , 0 , 4000)."'" : 'null';
	$arqdescricao = $_POST['arqdescricao'] ? "'".$_POST['arqdescricao']."'" : 'null';
	$pdoid = $_POST['pdoid'];
	
	if( empty($pdoid) ){
            
            $pdoobservacao    = (strlen($pdoobservacao) > 4000) ? substr($pdoobservacao, 0, 3998) : $pdoobservacao;
            $pdojustificativa = (strlen($pdojustificativa) > 4000) ? substr($pdojustificativa, 0, 3998) : $pdojustificativa;
            
		$sql = "INSERT INTO obras2.pedidodesbloqueioobra(obrid, pdoobservacao, pdojustificativa, usucpf) 
				VALUES ($obrid, $pdoobservacao, $pdojustificativa, '{$_SESSION['usucpf']}') returning pdoid";
		$pdoid = $db->pegaUm($sql);
        
        enviarEmailPedidoDesbloqueio($pdoid);
	} else {
		$sql = "UPDATE obras2.pedidodesbloqueioobra SET
  					pdoobservacao = $pdoobservacao,
  					pdojustificativa = $pdojustificativa 
				WHERE 
				  	pdoid = $pdoid";
		$db->executar($sql);
	}
	
	if($_FILES["arquivo"]['name'] && !$_POST['arqid']){
		$campos	= array("pdoid"  => $pdoid,
						"anxdsc" => $arqdescricao,
						"usucpf" => "'{$_SESSION['usucpf']}'",
						"anxtipo" => "'PD'"
						);
		$file = new FilesSimec("anexo", $campos ,"obras2");
		$arquivoSalvo = $file->setUpload();
	} elseif($_POST['arqid']) {
	    $sql = "UPDATE obras2.anexo SET anxdsc = {$arqdescricao} where arqid=".$_POST['arqid']." and anxtipo = 'PD'";
	    $db->executar($sql);
	}
	
	$db->commit();
	if( $_POST['btAcao'] == 'anexo' || $pdoid ){
		$db->sucesso('principal/pedidoDesbloqueio', '&obrid='.$_REQUEST['obrid'].'&pdoid='.$pdoid);
	} else {
		$db->sucesso('principal/pedidoDesbloqueio', '&obrid='.$_REQUEST['obrid']);
    	die;
	}	 
}

if( $_REQUEST['pdoid'] ){
	$urlP = "/obras2/obras2.php?modulo=principal/pedidoDesbloqueio&acao=A&obrid=".$_REQUEST['obrid']."&pdoid=".$_REQUEST['pdoid'];
} else {
	$urlP = "/obras2/obras2.php?modulo=principal/pedidoDesbloqueio&acao=A&obrid=".$_REQUEST['obrid'];
}
print carregaAbaDesbloqueio($urlP, $_REQUEST['obrid'], $_REQUEST['pdoid']);

monta_titulo( "Pedido de Desbloqueio", "" );

if( !empty($pdoid) ){
	$sql = "SELECT 
			  	pdoid,
			  	obrid,
			  	pdoobservacao,
			  	pdojustificativa,
			  	pdodatainclusao,
			  	usucpf
			FROM 
				obras2.pedidodesbloqueioobra    
			WHERE
				pdoid = $pdoid
			    and pdostatus = 'A'";
	$arrPedido = $db->pegaLinha($sql);
        $arrPedido = empty($arrPedido) ? array() : $arrPedido;
	extract($arrPedido);
}

if ( $empid ){
	$empreendimento = new Empreendimento( $_SESSION['obras2']['empid'] );
	$empreendimento->montaCabecalho();
}
?>
<html>
<script type="text/javascript" src="/includes/funcoes.js"></script>
<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
<link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>

<script type="text/javascript">
	$(document).ready(function(){
		$('#tr_obras1').css('display', 'none');
		$('#td_altera').css('display', 'none');
		$('#td_insere').css('display', '');
		
		if( $('#habilita').val() == 'N' ){ 
			$('input, select, textarea').attr('disabled', true);
			$('.img_add_foto').hide();
		}		
		
		$("[name='btn_cancelar']").click(function() {
			$('[id=div_sub_foto]').hide();
			$('#td_altera').css('display', 'none');
			$('#td_insere').css('display', '');
		});
		
		$('[name=btn_upload]').click(function() {
		 	var erro = 0;
		 	if(!$('#formulario [name=arquivo]').val() && $('#td_insere').css('display') != 'none' ){
		 		alert('Favor selecionar o anexo!');
		 		erro = 1;
		 		return false;
		 	}
		 	if(!$('#formulario [name=arqdescricao]').val()){
		 		alert('Favor informar uma descrição!');
		 		$('#[name=arqdescricao]').focus()
		 		erro = 1;
		 		return false;
		 	}
		 	if(erro == 0 && validaCampos() ){
		 		$('[name=btn_upload]').val("Carregando...");
		 		$('[name=btn_cancelar]').val("Carregando...");
		 		$('[name=btn_upload]').attr("disabled","disabled");
		 		$('[name=btn_cancelar]').attr("disabled","disabled");
		 		$('#requisicao').val('salvarPedido');
		 		$('#btAcao').val('anexo');
		 		$('[name=formulario]').submit();
		 	}
		 });
	});
	function addAnexos( arqid, pdoid ){
		if( arqid ){
			$.ajax({
					type: "POST",
					dataType: "json",
					url: 'obras2.php?modulo=principal/pedidoDesbloqueio&acao=A',
					data: { carregarAnexo: "true", arqid: arqid, pdoid: pdoid},
					async: false,
					success: function(retornoajax) {
						$('[name="arqdescricao"]').val(retornoajax.descricao);
						$('#td_altera').html(retornoajax.arquivo);
  				}
  			});
  			
  			$('#td_altera').css('display', '');
			$('#td_insere').css('display', 'none');
		} else {
			$('#td_altera').css('display', 'none');
			$('#td_insere').css('display', '');
            $('.popup_alerta').removeClass('hidden');

			$('[name="arqdescricao"]').val('');
			$('[name="arqid"]').val('');
		}
		$('[name=arqid]').val(arqid);
		$('[id=div_sub_foto]').show();
		$( 'html, body' ).animate( { scrollTop: 0 }, 'slow' ); 
	}
	
	function validaCampos(){
		if( $('#pdojustificativa').val() == '' ){
			alert('Informe o campo "Justificativa"!');
			$('#pdojustificativa').focus();
			return false;
		}
		/*if( $('#pdoobservacao').val() == '' ){
			alert('Informe o campo "Observação"!');
			$('#pdoobservacao').focus();
			return false;
		}*/
		return true;
	}
	
	function salvarPedido(){
		if( validaCampos() ){
			$('#requisicao').val('salvarPedido');
			$('[name=formulario]').submit();
		}
	}
	
	function alterarPedido( pdoid, obrid ){
		window.location.href = 'obras2.php?modulo=principal/pedidoDesbloqueio&acao=A&obrid='+obrid+'&pdoid='+pdoid;
	}
	function excluirPedido( pdoid, obrid ){
		$('#requisicao').val('excluirPedido');
		$('#pdoid').val(pdoid);
		$('[name=formulario]').submit();
	}
	
	function excluirAnexo( arqid, pdoid, obrid ){
	 	if ( confirm( 'Deseja excluir o Anexo?' ) ) {
	 		window.location.href = 'obras2.php?modulo=principal/pedidoDesbloqueio&acao=A&obrid='+obrid+'&pdoid='+pdoid+'&arqidDel='+arqid;
	 	}
	}
	
</script>
<style>
	.field_foto {text-align:right}
	.field_foto legend{font-weight:bold;}
	.img_add_foto{cursor:pointer}
	.hidden{display:none}
	.img_foto{border:0;margin:5px;cursor:pointer;margin-top:-5px}
	.div_foto{width:110px;height:90px;margin:3px;border:solid 1px black;float:left;text-align:center;background-color:#FFFFFF}
	.fechar{position:relative;margin-left:104px;top:-6px;cursor:pointer}
	
	.tabela{
		width: 95%
	}
	#calendarDiv{
		z-index: 1 !important;
	}
</style>
<body>
<form id="formulario" name="formulario" method=post enctype="multipart/form-data" >
<input type="hidden" name="requisicao" id="requisicao" value="" />
<input type="hidden" name="habilita" id="habilita" value="<?=$habilita; ?>" />
<input type="hidden" name="btAcao" id="btAcao" value="" />
<input type="hidden" name="pdoid" id="pdoid" value="<?=$pdoid; ?>" />
<input type="hidden" name="arqid" id="arqid" value="" />
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center" style="border-bottom:0px;">
	<tr>
		<td class="SubTituloDireita" width="20%">Justificativa:</td>
		<td><?=campo_textarea('pdojustificativa', 'S', 'S', 'Justificativa', 98, 5, 4000, '', '', '', '', 'Justificativa');?></td>
	</tr>
	<tr>
		<td class="SubTituloDireita" width="20%">Observação:</td>
		<td><?=campo_textarea('pdoobservacao', 'N', 'S', 'Observação', 98, 5, 4000, '', '', '', '', 'Observação');?></td>
	</tr>
	<tr>
		<td class="SubTituloDireita" width="20%">Anexos:</td>
		<td valign="top">
				<img class="img_add_foto" src="../imagens/gif_inclui.gif" title="Adicionar Anexo" onclick="addAnexos()"  /><b><span onclick="addAnexos()" class="img_add_foto" style="cursor: pointer;"> Inserir Anexos</span></b><br>
			<table class="listagem" cellSpacing="1" width="80%" cellPadding="3" align="left" id="tabelaAnexo">
				<tr>
					<th>Ação</th>
					<th>Arquivo</th>
					<th>Descrição</th>
				</tr>
				<?
				if( $pdoid ){
					$sql = "SELECT 
								a.anxid,
							    a.arqid,
								ar.arqnome||'.'||ar.arqextensao as arquivo,
								a.anxdsc as descricao
							FROM 
								obras2.anexo a
							    inner join public.arquivo ar on ar.arqid = a.arqid
							WHERE
								a.pdoid = $pdoid
							    and a.anxtipo = 'PD'";
					$arrArquivo = $db->carregar($sql);
					$arrArquivo = $arrArquivo ? $arrArquivo : array();
					
					$count = 1;
					foreach ($arrArquivo as $v) { ?>
						<tr>
							<td>
								<?if( $habilita == 'N' ){ ?>
									<img src="../imagens/alterar_01.gif" style="border:0; cursor:pointer;" title="Alterar Descrição Anexo">
			        				<img src="../imagens/excluir_01.gif" style="border:0; cursor:pointer;" title="Excluir Documento Anexo">
								<?} else { ?>
									<img src="../imagens/alterar.gif" onclick="addAnexos(<?php echo $v['arqid'];?>, <?=$pdoid ?>)" style="border:0; cursor:pointer;" title="Alterar Descrição Anexo">
			        				<img src="../imagens/excluir.gif" onClick="excluirAnexo('<?php echo $v['arqid']; ?>', <?=$pdoid ?>, <?=$obrid ?>);" style="border:0; cursor:pointer;" title="Excluir Documento Anexo">
		        				<?} ?>
							</td>
							<td><?php echo $count.' - '; ?><a style="cursor: pointer; color: blue;" onclick="window.location='?modulo=principal/pedidoDesbloqueio&acao=A&download=S&arqid=<?php echo $v['arqid'];?>&tipo=D'"><?php echo $v['arquivo'];?></a></td>
							<td><?=$v['descricao'] ?></td>
						</tr>
					<?
						$count++;
					}
				}
				?>
			</table>
		</td>
	</tr>
	<tr bgcolor="#DEDEDE">
		<td colspan="2" align="center">
			<input name="btn_salvar" value="Salvar Pedido" type="button" onclick="salvarPedido();">
			<input name="btn_fechar" value="Fechar" type="button" onclick="window.close()">
			</td>
	</tr>
</table>
<?
$html = '<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
					<tr>
						<td width="30%" class="SubtituloDireita" >Anexo:</td>
						<td id="td_insere"><input type="file" name="arquivo" /></td>
						<td id="td_altera"></td>
					</tr>
					<tr>
						<td class="SubtituloDireita" >Descrição:</td>
						<td>'.campo_texto("arqdescricao","S","S","",40,60,"","","","","","","").'</td>
					</tr>
					<tr>
						<td class="SubtituloDireita" ></td>
						<td class="SubtituloEsquerda" >
							<input type="button" name="btn_upload" value="Salvar" />
							<input type="button" name="btn_cancelar" value="Cancelar" />
						</td>
					</tr>
				</table>'; 
				   
popupAlertaGeral($html,"400px","205px","div_sub_foto","hidden");
?>
</form>
<?
	
monta_titulo( "Lista Pedido de Desbloqueio", "" );

if( $habilita == 'S' ){
	$imagem = "'<img src=\"../imagens/alterar.gif\" onclick=\"alterarPedido('||pbo.pdoid||', '||pbo.obrid||')\" style=\"border:0; cursor:pointer;\" title=\"Alterar Pedido Desbloqueio\">&nbsp;'||
		    	'<img src=\"../imagens/excluir.gif\" onclick=\"excluirPedido('||pbo.pdoid||', '||pbo.obrid||')\" style=\"border:0; cursor:pointer;\" title=\"Excluir Pedido Desbloqueio\">'";
} else {
	$imagem = "'<img src=\"../imagens/alterar_01.gif\" style=\"border:0; cursor:pointer;\" title=\"Alterar Pedido Desbloqueio\">&nbsp;'||
		    	'<img src=\"../imagens/excluir_01.gif\" style=\"border:0; cursor:pointer;\" title=\"Excluir Pedido Desbloqueio\">'";
}

$sql = "SELECT
			$imagem as acoes,
			pbo.pdojustificativa, 
			to_char(pbo.pdodatainclusao, 'DD/MM/YYYY HH24:MI:SS'),
			us.usunome
		FROM 
			obras2.pedidodesbloqueioobra pbo
			inner join seguranca.usuario us on us.usucpf = pbo.usucpf
		WHERE
			pbo.pdostatus = 'A'
			and pbo.obrid = $obrid";

$cabecalho = array('Ações', 'Justificativa', 'Data', 'Usuário');
$arrHeighTds = array('05%', '60%', '15%', '20%');
$db->monta_lista($sql, $cabecalho, 1000000, 5, 'N', 'center', '', '', $arrHeighTds);
?>
</body>
</html>