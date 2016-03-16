<?php
include (APPRAIZ.'www/painel/_funcoes_metas.php');
include_once APPRAIZ . 'includes/workflow.php';

if($_POST['requisicaoAjax']){
	header('content-type: text/html; charset=ISO-8859-1');
	$_POST['requisicaoAjax']();
	exit();
}

if($_POST['requisicao']){
	$_POST['requisicao']();
}

if($_REQUEST['micid']){
	extract(recuparMonitoraItemCheckList($_REQUEST['micid']));
	$permissao = $micimportado == "t" ? "N" : "S";
}

if($mnmid){
	extract(recuparMonitoraMeta($mnmid));
	extract(recuparMonitoraMetaExecutor($mnmid));
	extract(recuparMonitoraMetaValidador($mnmid));
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

$permissao = atividade_verificar_responsabilidade( $atividade['atiid'], $_SESSION['usucpf'] );
$permissao_formulario = $permissao ? 'S' : 'N'; # S habilita e N desabilita o formulário

// ----- VERIFICA SE PROJETO ESTÁ SELECIONADO
projeto_verifica_selecionado();

// ----- CABEÇALHO
include APPRAIZ . 'includes/cabecalho.inc';
print '<br/>';
$menu = array(0 => array("id" => 1, "descricao" => "Listar itens de checklist", "link" => "estrategico.php?modulo=principal/atividade_estrategico/listar_checklist&acao=A&atiid=$atiid"),
				  1 => array("id" => 2, "descricao" => "Item de Checklist", 		"link" => "estrategico.php?modulo=principal/atividade_estrategico/item_checklist&acao=A&atiid=$atiid&micid=$micid"),
				  2 => array("id" => 3, "descricao" => "Metas do Checklist",    	"link" => "estrategico.php?modulo=principal/atividade_estrategico/meta_checklist&acao=A&atiid=$atiid&micid=$micid"),
		  	  );
	echo montarAbasArray($menu,"estrategico.php?modulo=principal/atividade_estrategico/meta_checklist&acao=A&atiid=$atiid&micid=$micid");
montar_titulo_projeto( "Meta do Checklist" );
//extract( $atividade ); # mantém o formulário preenchido
?>

<script type="text/javascript" src="../includes/jquery-ui-1.8.18.custom/js/jquery-1.7.1.min.js"></script>
<script type="text/javascript" src="../includes/jquery-ui-1.8.18.custom/js/jquery-ui-1.8.18.custom.min.js"></script>
<link href="../includes/jquery-ui-1.8.18.custom/development-bundle/themes/base/jquery.ui.all.css" type="text/css" rel="stylesheet"></link>
<script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>
<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>
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
<script language="javascript" type="text/javascript">
function inserirExecutor()
{
	janela("estrategico.php?modulo=principal/atividade_estrategico/inserir_executor_meta&acao=A&atiid=<?=$_REQUEST['atiid']?>&entid_executor="+document.getElementById('entid_executor').value,800,580,"att_exec");
}

function removerExecutor()
{
	if (confirm('Deseja remover o executor ?'))
	{
		jQuery("[name='requisicao']").val("removerExecutor");
		jQuery("[name='form_item_check_list']").submit();
	}
}

function removerValidador()
{
	if (confirm('Deseja remover o validador ?'))
	{
		jQuery("[name='requisicao']").val("removerValidador");
		jQuery("[name='form_item_check_list']").submit();
	}
}

function inserirValidador()
{
	janela("estrategico.php?modulo=principal/atividade_estrategico/inserir_validador_meta&acao=A&atiid=<?=$_REQUEST['atiid']?>&entid_validador="+document.getElementById('entid_validador').value,800,580,"att_valid");
}

function addMeta(metid)
{
	janela("estrategico.php?modulo=principal/atividade_estrategico/valor_meta&acao=A&metid="+metid,600,400,"att_exec");
}

jQuery(document).ready(function($)
{

	// ao optar por uma opção de evidência ou não...
		$('.opcao_evidencia').click(function()
		{
			// execução
			if( $(this).attr('name') == 'opcao_evidencia_execucao' )
			{
				if( $(this).val() == 'S' )
					$('#evidencia_execucao').show();
				else
					$('#evidencia_execucao').hide();
			}
			// validação
			if( $(this).attr('name') == 'opcao_evidencia_validacao' )
			{
				if( $(this).val() == 'S' )
					$('#evidencia_validacao').show();
				else
					$('#evidencia_validacao').hide();
			}
			// certificação
			if( $(this).attr('name') == 'opcao_evidencia_certificacao' )
			{
				if( $(this).val() == 'S' )
					$('#evidencia_certificacao').show();
				else
					$('#evidencia_certificacao').hide();
			}
		});
});

function editarValorMeta(perid,dmiid,dpeid,dmiqtde,dmivalor)
{	
	janela("estrategico.php?modulo=principal/atividade_estrategico/valor_meta&acao=A&metid=<?php echo $metid ?>&dmiid=" + dmiid,600,400,"att_exec");	
}

function excluirValorMeta(dmiid)
{
	if(confirm("Deseja realmente excluir?")){
		var params = "requisicaoAjax=excluirValorMeta&dmiid=" + dmiid;
		$.ajax({
		   type: "POST",
		   url: window.location,
		   data: params,
		   success: function(msg){
		   		window.location.href = window.location;
		   }
		 });
	}
}

function salvarMonitoraMeta()
{
	var erro = 0;
	
	if(erro == 0 && !jQuery("#entid_executor").val()){
		alert('Favor informar o executor.');
		erro = 1;
		return false;
	}
	
	/*if(erro == 0 && !jQuery("#entid_validador").val()){
		alert('Favor informar o validador.');
		erro = 1;
		return false;
	}*/

	if(erro == 0){
		jQuery("[name='requisicao']").val("cadastrarMonitoraMeta");
		jQuery("[name='form_item_check_list']").submit();
	}
}

function excluirValidador(){
	jQuery("#nome_validador").html("");
	jQuery("#img_excluir_validador").remove();
	jQuery("#entid_validador").val("");
}

</script>

<table class="tabela" bgcolor="#fbfbfb" cellspacing="0" cellpadding="10" align="center">
	<tr>
		<td>
			<?= montar_resumo_atividade( $atividade ) ?>
				<form method="post" name="form_item_check_list" id="form_item_check_list" >
					<input type="hidden" id="micid" name="micid" value="<?php echo $micid; ?>" />
					<input type="hidden" id="mnmid" name="mnmid" value="<?php echo $mnmid; ?>" />
					<input type="hidden" id="atiid" name="atiid" value="<?php echo $atiid ?>" />
					<input type="hidden" id="requisicao" name="requisicao" value=""/>
					<table class="tabela" bgcolor="#f5f5f5" cellspacing="1" cellpadding="5" style="width: 100%;">
						<tr>
							<td class="SubtituloDireita" >Descrição:</td>
							<td><?php echo $mnmdsc ?></td>
						</tr>
						<tr>
							<td colspan="2" class="SubtituloTabela bold esquerda" >Etapa de Execução</td>
						</tr>
						<tr id="linha_evidencia_execucao" >
							<td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%;">
								<img src="/imagens/seta_filho.gif" />
								<b>Coletar evidência na Execução?</b>
							</td>
							<td>
								<input type="radio" value="S" class="opcao_evidencia" id="opcao_evidencia_execucao_sim" name="opcao_evidencia_execucao" <?php if($evidencia_execucao=='t') echo 'checked="checked"'; ?> /> Sim
								&nbsp;&nbsp;
								<input type="radio" value="N" class="opcao_evidencia" id="opcao_evidencia_execucao_nao" name="opcao_evidencia_execucao" <?php if(!$evidencia_execucao || $evidencia_execucao=='f') echo 'checked="checked"'; ?> /> Não
								&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
								<span id="evidencia_execucao" style="display:<?php echo $evidencia_execucao == "t" ? "" : "none" ?>">
									<b>Evidência</b>: <?php echo campo_texto('etcevidenciaexecucao','N','S','',30, 500,'','',null,null,null,'id="iclevidenciaexecucao"'); ?>
								</span>
							</td>
						</tr>
						<tr id="linha_executores" >
							<td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%;">
								<img src="/imagens/seta_filho.gif" />
								<b>Executores</b>
							</td>
							<td>
							<?php $dadosExecutor = $celular_executor ? $nome_executor . ' - ' . $celular_executor : $nome_executor; ?>
							<span id="nome_executor"><? echo $dadosExecutor; ?></span> 
							<a style="cursor:pointer;" onclick="removerExecutor(); return false;"><img src="../imagens/excluir.gif" align="absmiddle"></a>
							<a style="cursor:pointer;" onclick="inserirExecutor(); return false;"><img src="../imagens/gif_inclui.gif" align="absmiddle"> Executor</a>
							<input type="hidden" name="entid_executor" id="entid_executor" value="<? echo $entid_executor; ?>">
							</td>
						</tr>
						<tr>
							<td colspan="2" class="SubtituloTabela bold esquerda" >Etapa de Validação</td>
						</tr>
						<tr id="linha_evidencia_validacao" >
							<td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%;">
								<img src="/imagens/seta_filho.gif" />
								<b>Coletar evidência na Validação?</b>
							</td>
							<td>
								<input type="radio" value="S" class="opcao_evidencia" id="opcao_evidencia_validacao_sim" name="opcao_evidencia_validacao" <?php if($evidencia_validacao=='t') echo 'checked="checked"'; ?> /> Sim
								&nbsp;&nbsp;
								<input type="radio" value="N" class="opcao_evidencia" id="opcao_evidencia_validacao_nao" name="opcao_evidencia_validacao" <?php if(!$evidencia_validacao || $evidencia_validacao=='f') echo 'checked="checked"'; ?> /> Não
								&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
								<span id="evidencia_validacao" style="display:<?php echo $evidencia_validacao == "t" ? "" : "none" ?>;">
									<b>Evidência</b>: <?php echo campo_texto('etcevidenciavalidacao','N','S','',30, 500,'','',null,null,null,'id="iclevidenciavalidacao"'); ?>
								</span>
							</td>
						</tr>
						<tr id="linha_validadores" >
							<td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%;">
								<img src="/imagens/seta_filho.gif" />
								<b>Validadores</b>
							</td>
							<td>
							<span id="nome_validador"><? echo $nome_validador; ?></span>
							<?php if($nome_validador): ?>
								<a style="cursor:pointer;" id="img_excluir_validador" title="Excluir validador" onclick="excluirValidador(); return false;"><img src="../imagens/excluir.gif" align="absmiddle"></a>
							<?php endif; ?> 
							<a style="cursor:pointer;" onclick="removerValidador(); return false;"><img src="../imagens/excluir.gif" align="absmiddle"></a>
							<a style="cursor:pointer;" onclick="inserirValidador(); return false;"><img src="../imagens/gif_inclui.gif" align="absmiddle"> Validador</a>
							<input type="hidden" name="entid_validador" id="entid_validador" value="<? echo $entid_validador; ?>">
							</td>
						</tr>
						<?php if($mnmid && $entid_executor && $metid): ?>
							<tr>
								<td colspan="2" class="SubtituloTabela bold esquerda" >Metas</td>
							</tr>
							<tr>
								<td colspan="2" class="SubtituloEsquerda" ><img src="../imagens/gif_inclui.gif" onclick="addMeta(<?php echo $metid ?>)"  class="link img_middle" title="Meta" onclick="Cadastrar Meta" /> <a href="javascript:addMeta(<?php echo $metid ?>)" >Cadastrar Meta</a></td>
							</tr>
							<tr>
								<td colspan="2" class="center" ><?php listarValorMetas($metid); ?></td>
							</tr>
						<?php endif; ?>
						</tr>
							<td class="SubtituloTabela center" colspan="2"  >
								<input type="button" name="btn_salvar" value="Salvar" onclick="salvarMonitoraMeta()" />
								<input type="button" name="btn_voltar" value="Voltar" onclick="window.location.href='estrategico.php?modulo=principal/atividade_estrategico/item_checklist&acao=A&atiid=<?php echo $atiid ?>&micid=<?php echo $micid ?>'" />
							</td>
						</tr>
					</table>
			</form>
		</td>
	</tr>
</table>