<?php
if($_REQUEST['requisicaoAjax']){
	header('content-type: text/html; charset=ISO-8859-1');
	$_REQUEST['requisicaoAjax']();
	exit;
}

include APPRAIZ."fabrica/classes/Contrato.class.inc";
include APPRAIZ."fabrica/classes/ContratoPenalidade.class.inc";

$ctrid = $_REQUEST['ctrid'];
$ctpid = $_REQUEST['ctpid'];

if($_REQUEST['requisicao']){
	$penalidade = new ContratoPenalidade($ctpid);
	$penalidade->popularDadosObjeto($_REQUEST);
	
	if($_POST['ctpid'] && $_POST['ctpid'] != "")
		$penalidade->$_REQUEST['requisicao']();
	else
		$ctpid = $penalidade->$_REQUEST['requisicao']();
	
	$penalidade->commit();
	
	if($_REQUEST['reload']){
		Header("Location: fabrica.php?modulo=principal/penalidadeContrato&acao=A&ctrid=".$_REQUEST['ctrid']);
	}
	
	$msg = "Operação Realizada com Sucesso!";
}

if($ctrid){
	$contrato = new Contrato($ctrid);
	$arrDados = $contrato->getDados();
	extract($arrDados);
}
if($ctpid){
	$penalidade = new ContratoPenalidade($ctpid);
	$arrDados = $penalidade->getDados();
	extract($arrDados);
}else{
	$penalidade = new ContratoPenalidade();
}

// monta cabeçalho 
include APPRAIZ . 'includes/cabecalho.inc';
print '<br/>';

// carregando o menu
$menu[0] = array("descricao" => "Listar Contratos", "link"=> "fabrica.php?modulo=principal/listarContrato&acao=A");
$menu[1] = array("descricao" => "Cadastrar Contrato", "link"=> "fabrica.php?modulo=principal/cadContrato&acao=A&ctrid=$ctrid");
if($ctrid){
	$menu[2] = array("descricao" => "Penalidades do Contrato", "link"=> "fabrica.php?modulo=principal/penalidadeContrato&acao=A&ctrid=$ctrid");
}
echo montarAbasArray($menu, "fabrica.php?modulo=principal/penalidadeContrato&acao=A&ctrid=$ctrid");

$titulo = "Penalidades do Contratos";
monta_titulo( $titulo, obrigatorio().'Indica campo obrigatório.' );

?>
<script language="javascript" type="text/javascript" src="../includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-1.4.2.min.js"></script>
<script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/dateFunctions.js"></script>
<script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>
<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>
<style>
	.TituloTabela{background-color: #C5C5C5;font-weight:bold}
	.center{text-align:center}
	.link{cursor: pointer;}
	.middle{vertical-align: middle;}
</style>

<script>
	function limparCampos()
	{
		$('input:not(:hidden,:button,[readonly=""],[readonly="readonly"]),textarea,select').val("");
	}
	
	function salvarPenalidade()
	{
		if(validaFormulario()){
			$('#form_cad_penalidade').submit();
		}
	}
	
	function validaFormulario()
	{
		var erro = 0;
		$("[class~=obrigatorio]").each(function() { 
			if(!this.value){
				erro = 1;
				alert('Favor preencher todos os campos obrigatórios!');
				this.focus();
				return false;
			}
		});
		
		if(erro == 0 && $("[name=ctpvalor]").val() > 100){
			erro = 1;
			alert('O Percentual de Penalidade não pode ser maior que 100%!');
			$("[name=ctpvalor]").focus();
			return false;
		}
		
		if(erro == 0){
			return true;
		}
	}
	
	function exibirSolicitacaoServico(prjid)
	{
		if(prjid){
			$('#td_combo_scsid').html(carregando());
				$.ajax({
				  url: window.location + "&requisicaoAjax=showSolicitacaoServico&ctrid=" + prjid,
				  success: function(data) {
				    $('#td_combo_scsid').html(data);
			  	}
			});
		}
	}
	
	function exibirOrdemServico(scsid)
	{
		if(scsid){
			$('#td_combo_odsid').html(carregando());
				$.ajax({
				  url: window.location + "&requisicaoAjax=showOrdemServico&scsid=" + scsid,
				  success: function(data) {
				    $('#td_combo_odsid').html(data);
			  	}
			});
		}
	}
	
	function carregando()
	{
		return "<center><img src=\"../imagens/wait.gif\" class=\"middle\" />Aguarde...Carregando!</center>";
	}
	
	function alterarPenalidade(ctpid)
	{
		window.location.href = window.location + "&ctpid=" + ctpid;
	}
	
	function excluirPenalidade(ctpid)
	{
		if(confirm('Deseja realmente excluir a penalidade?')){
			window.location.href = 'fabrica.php?modulo=principal/penalidadeContrato&acao=A&requisicao=excluirPenalidade&ctpid=' + ctpid + "&reload=true&ctrid=<?=$ctrid?>";
		}
	}
	
</script>

<form name="form_cad_penalidade" id="form_cad_penalidade" method="post" action="" >
	<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
		<tr>
			<td class="SubtituloDireita">Número do Contrato:</td>
			<td><?=campo_texto("ctrnumero","N","N","Número do Contrato","20","20","","") ?></td>
		</tr>
		<tr>
			<td class="SubtituloDireita" width="25%" >Nome da Empresa:</td>
			<td>
				<?php
				if($entidcontratado){ 
					require_once APPRAIZ . "includes/classes/entidades.class.inc";
					$entidade = new Entidades();
					$entidade->carregarPorEntid($entidcontratado);
					$entnome_empresa = $entidade->getEntnome();
				}
				?>
				<?=campo_texto("entnome_empresa","N","N","Empresa do Contrato","60","120","","") ?>
				<input type="hidden" name="ctrid" value="<?=$ctrid?>" />
				<input type="hidden" name="ctpid" value="<?=$ctpid?>" />
				<input type="hidden" name="requisicao" value="salvar" />
			</td>
		</tr>
		<tr>
			<td class="SubtituloDireita">Sistema:</td>
			<td>
				<?php 
				if($odsid){
					$scsid = recuperarSolicitacaoPorOrdem($odsid);
				}
				
				/*
				if($scsid){
					$ctsid = recuperarSistemaPorSolicitacao($scsid);
				}
				*/
				?>
				<?php $sql = "	select
									sidid as codigo,
									siddescricao as descricao
								from
									demandas.sistemadetalhe sid 
								where
									sidstatus = 'A'
								order by
									siddescricao"; ?>
				<?=$db->monta_combo("sidid",$sql,"S","Selecione...","","","","","S") ?>
			</td>
		</tr>
		<tr>
			<td class="SubtituloDireita">Solicitação de Serviço:</td>
			<td id="td_combo_scsid" >
				<?php if($ctrid):
					$habil = "S";
					$sql = "	select 
									scs.scsid as codigo,
									scs.scsid || ' - ' || substr(scs.scsnecessidade,0,150) || '...' as descricao
								from
									fabrica.solicitacaoservico scs
								inner join
									fabrica.analisesolicitacao ans ON ans.scsid = scs.scsid
								where
									ctrid= $ctrid
								order by
									scs.scsnecessidade"; 
				else:
					$habil = "N";
					$sql = array();
				endif;?> 

				<?=$db->monta_combo("scsid",$sql,$habil,"Selecione...","exibirOrdemServico","","","","S") ?>
			</td>
		</tr>
		<tr>
			<td class="SubtituloDireita">Ordem de Serviço:</td>
			<td id="td_combo_odsid" >
				<?php if($scsid):
					$habil = "S";
					$sql = "	select
									odsid as codigo,
									odsdetalhamento as descricao
								from
									fabrica.ordemservico
								where
									scsid = $scsid
								order by
									odsdetalhamento"; 
				else:
					$habil = "N";
					$sql = array();
				endif;?>
				<?=$db->monta_combo("odsid",$sql,$habil,"Selecione...","","","","","S") ?>
			</td>
		</tr>
		<tr>
			<td class="SubtituloDireita">Penalidade:</td>
			<td>
				<?php $sql = "	select
									pnlid as codigo,
									pnldsc as descricao
								from
									fabrica.penalidade
								where
									pnlstatus = 'A'
								order by
									pnldsc"; ?>
				<?=$db->monta_combo("pnlid",$sql,"S","Selecione...","","","","","S") ?>
			</td>
		</tr>
		<tr>
			<td class="SubtituloDireita" width="25%" >Percentual de Penalidade (%):</td>
			<td>
				<?=campo_texto("ctpvalor","S","S","Percentual de Penalidade (%)","15","3","###","") ?>
			</td>
		</tr>
		<tr>
			<td colspan="2" class="TituloTabela center">
				<input type="button" onclick="salvarPenalidade()" value="Salvar" >
				<input type="button" onclick="limparCampos()" value="Limpar Campos" >
			</td>
		</tr>
	</table>
</form>
<?php $penalidade->showListaPenalidade() ?>
<?php if($msg): ?>
<script>
	alert('<?php echo $msg ?>');
</script>
<?php endif; ?>