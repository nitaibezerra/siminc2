<?php
if($_POST['requisicaoAjax']){
	header('content-type: text/html; charset=ISO-8859-1');
	$_POST['requisicaoAjax']();
	exit();
}

$atividade = atividade_pegar( $_REQUEST['atiid'] );
if ( !$atividade ) {
	redirecionar( 'principal/atividade_estrategico/arvore', 'A' );
}
$atiid = $atividade['atiid'];

$parametros = array(
	'aba' => $_REQUEST['aba'], # mantém a aba ativada
	'atiid' => $_REQUEST['atiid']
);

if( $_REQUEST['requisicao'] ){
	$_REQUEST['requisicao']();
}

$permissao = atividade_verificar_responsabilidade( $atividade['atiid'], $_SESSION['usucpf'] );
$permissao_formulario = $permissao ? 'S' : 'N'; # S habilita e N desabilita o formulário

// ----- VERIFICA SE PROJETO ESTÁ SELECIONADO
projeto_verifica_selecionado();

// ----- CABEÇALHO
include APPRAIZ . 'includes/cabecalho.inc';
print '<br/>';
$db->cria_aba( $abacod_tela, $url, '&atiid=' . $atividade['atiid']  );
montar_titulo_projeto( $atividade['atidescricao'] );
?>
<style>
.SubtituloTabela{background-color:#cccccc}
.negrito{font-weight:bold}
.bold{font-weight:bold}
.normal{font-weight:normal}
.center{text-align: center;}
.direita{text-align: right;}
.esquerda{text-align: left;}
.msg_erro{color:#990000}
.link{cursor: pointer}
.mini{width:12px;height:12px}
.sucess_msg{color: blue;}
.img_middle{vertical-align:middle}
.hidden{display:none}
.absolute{position:absolute;padding-top:5px;padding-bottom:5px;padding-left:5px;margin-top:5px;margin-left:50px;border:solid 1px black;background-color: #FFFFFF}
.fechar{position:relative;right:-5px;top:-26px;}
.img{background-color:#FFFFFF}
.red{color:#990000}
.mensagem{text-align: center; font-weight: bold;background-color:#cccccc;color: blue;}
.field_ano{width:100%;padding:5px;margin:3px}
</style>
<script type="text/javascript" src="../includes/jquery-ui-1.8.18.custom/js/jquery-1.7.1.min.js"></script>
<script>
	function addItem()
	{
		window.location.href='estrategico.php?modulo=principal/atividade_estrategico/item_checklist&acao=A&atiid=<?php echo $atiid ?>';
	}
	function editarMonitoraItemChecklist(micid)
	{
		window.location.href='estrategico.php?modulo=principal/atividade_estrategico/item_checklist&acao=A&atiid=<?php echo $atiid ?>&micid=' + micid;
	}
	function excluirMonitoraItemChecklist(micid)
	{
		if(confirm("Deseja realmente excluir o Item de Check List e todas as suas Metas?")){
			window.location.href='estrategico.php?modulo=principal/atividade_estrategico/listar_checklist&acao=A&requisicao=excluirMonitoraItemChecklist&atiid=<?php echo $atiid ?>&micid=' + micid;
		}
	}
	
	function ordemMonitoraItemChecklist(obj,micid,tipo){
		var linha = (obj.parentNode.parentNode.rowIndex);
		var tabela = obj.parentNode.parentNode.parentNode;
		var total_linhas = jQuery(tabela).find('tr').length;
		var var_linha = jQuery(tabela).find("tr:eq(" + (linha-1) + ")");
		if(tipo == "cima" && linha != 1){
			jQuery(var_linha).prev().before(jQuery(var_linha));
			alteraOrdemItemChecklist(micid,tipo);
			//jQuery(tabela).filter(":odd").attr("bgcolor","#FFFFFF");
			//jQuery(tabela).filter(":even").attr("bgcolor","#F7F7F7");
		}
		if(tipo == "baixo" && linha != total_linhas ){
			jQuery(var_linha).next().after(jQuery(var_linha));
			alteraOrdemItemChecklist(micid,tipo);
			//jQuery(tabela).filter(":odd").attr("bgcolor","#FFFFFF");
			//jQuery(tabela).filter(":even").attr("bgcolor","#F7F7F7");
		}
	}
	
	function alteraOrdemItemChecklist(micid,tipo)
	{
		var params = "requisicaoAjax=alteraOrdemItemChecklist&micid=" + micid + "&tipo=" + tipo;
		jQuery.ajax({
			   type: "POST",
			   url: window.location,
			   data: params,
			   success: function(msg){
			   }
			 });
	}
</script>
<table class="tabela" bgcolor="#fbfbfb" cellspacing="0" cellpadding="10" align="center">
	<tr>
		<td>
			<?= montar_resumo_atividade( $atividade ) ?>
			<img src="../imagens/gif_inclui.gif" onclick="addItem()"  class="link img_middle" title="Adicionar Meta" onclick="Cadastrar Item de Checklist " /> <a href="javascript:addItem()" >Cadastrar Item de Checklist</a>
		</td>
	</tr>
</table>
<?php listarMonitoraItemCheckList($_REQUEST['atiid']) ?>