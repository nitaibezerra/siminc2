<?php
include_once APPRAIZ . 'includes/workflow.php';
$micid = $_REQUEST['micid'];
$atividade = atividade_pegar( $_REQUEST['atiid'] );
if ( !$atividade ) {
	redirecionar( 'principal/atividade_estrategico/arvore', 'A' );
}
$atiid = $atividade['atiid'];
if ( !$micid ) {
	redirecionar( 'principal/atividade_estrategico/cadastro_checklist', 'A' ,array('atiid' => $atiid));
}

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
extract( $atividade ); # mantém o formulário preenchido

if($_REQUEST['mnmid']){
	extract(recuparMonitoraMeta($_REQUEST['mnmid']));
	extract(recuparMonitoraMetaExecutor($_REQUEST['mnmid']));
	extract(recuparMonitoraMetaValidador($_REQUEST['mnmid']));
}

if($_REQUEST['micid']){
	extract(recuparMonitoraItemCheckList($_REQUEST['micid']));
}

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
//jQuery.noConflict();

function inserirExecutor()
{
	janela("estrategico.php?modulo=principal/atividade_estrategico/inserir_executor_meta&acao=A&atiid=<?=$_REQUEST['atiid']?>&entid_executor="+document.getElementById('entid_executor').value,800,580,"att_exec");
}

function inserirValidador()
{
	janela("estrategico.php?modulo=principal/atividade_estrategico/inserir_validador_meta&acao=A&atiid=<?=$_REQUEST['atiid']?>&entid_validador="+document.getElementById('entid_validador').value,800,580,"att_valid");
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

function salvarMonitoraMeta()
{
	var erro = 0;
	jQuery("[class~=obrigatorio]").each(function() { 
		if(!this.value || this.value == "Selecione..."){
			erro = 1;
			alert('Favor preencher todos os campos obrigatórios.');
			this.focus();
			return false;
		}
	});

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


function editarMonitoraMeta(mnmid)
{
	window.location.href = "estrategico.php?modulo=principal/atividade_estrategico/cadastro_meta&acao=A&atiid=<?php echo $_REQUEST['atiid']; ?>&micid=<?php echo $micid; ?>&mnmid=" + mnmid;
}

function excluirMonitoraMeta(mnmid)
{
	if(confirm("Deseja realmente excluir esta meta?")){
		jQuery("[name='mnmid']").val(mnmid);
		jQuery("[name='requisicao']").val("excluirMonitoraMeta");
		jQuery("[name='form_item_check_list']").submit();
	}
}

function alteraTipoMeta(valor)
{
	if(valor == 1){ //Prazo
		jQuery("#tr_prazo").show();jQuery("[name='mnmprazo']").attr("class","normal obrigatorio");
		jQuery("#tr_qtde").hide();jQuery("[name='mnmqtd']").attr("class","normal");
		jQuery("#tr_faixas").hide();
		jQuery("#tr_valor").hide();jQuery("[name='mnmvalor']").attr("class","normal");
	}if(valor == 2){ //Quantidade
		jQuery("#tr_prazo").hide();jQuery("[name='mnmprazo']").attr("class","normal");
		jQuery("#tr_qtde").show();jQuery("[name='mnmqtd']").attr("class","normal obrigatorio");
		jQuery("#tr_faixas").show();
		jQuery("#tr_valor").show();jQuery("[name='mnmvalor']").attr("class","normal obrigatorio");
	}if(valor == 3){ //Prazo e Quantidade
		jQuery("#tr_prazo").show();jQuery("[name='mnmprazo']").attr("class","normal obrigatorio");
		jQuery("#tr_qtde").show();jQuery("[name='mnmqtd']").attr("class","normal obrigatorio");
		jQuery("#tr_faixas").show();
		jQuery("#tr_valor").show();jQuery("[name='mnmvalor']").attr("class","normal obrigatorio");
	}
}

function addValorMonitoraMeta(metid)
{
	window.location.href='estrategico.php?modulo=principal/atividade_estrategico/valor_meta&acao=A&atiid=<?php echo $_REQUEST['atiid']; ?>&micid=<?php echo $_REQUEST['micid']; ?>&metid='+metid;
}

</script>
<table class="tabela" bgcolor="#fbfbfb" cellspacing="0" cellpadding="10" align="center">
	<tr>
		<td>
			<?= montar_resumo_atividade( $atividade ) ?>
				<form method="post" name="form_item_check_list" id="form_item_check_list" >
					<input type="hidden" id="atiid" name="atiid" value="<?php echo $_REQUEST['atiid'] ?>" />
					<input type="hidden" id="micid" name="micid" value="<?php echo $micid; ?>" />
					<input type="hidden" id="mnmid" name="mnmid" value="<?php echo $_REQUEST['mnmid'] ?>" />
					<input type="hidden" id="requisicao" name="requisicao" value=""/>
					<table class="tabela" bgcolor="#f5f5f5" cellspacing="1" cellpadding="5" style="width: 100%;">
						<tr>
							<td colspan="2" class="SubtituloTabela bold esquerda" >Dados Gerais da Meta</td>
						</tr>
						<tr>
							<td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%">Descriçao da Meta:</td>
							<td>
								<?php echo campo_texto('mnmdsc','S',($trava ? 'N' : 'S'),'',60, 500,'','',null,null,null,'id="mnmdsc"'); ?>
							</td>
						</tr>
						<tr>
							<td class="SubtituloDireita" >Exercício</td>
							<td>
								<?php 
									$ano = date("Y");
									$x=0;
									for($i=$ano;$i<($ano+10);$i++){
										$arrAnos[$x] = array("codigo" => $i, "descricao" => $i);
										$x++;
									}
								?>
								<?php $db->monta_combo("mnmexercicio",$arrAnos,"S","Selecione...","","","","","S","","",$mnmexercicio) ?>
							</td>
						</tr>
						<tr>
							<td align='right' class="SubTituloDireita" >Tipo de Meta:</td>
							<td>
								<?php
									$sql = "select
								   				mtmid as codigo,
								   				mtmdsc as descricao
								   			from
								   				pde.monitoratipometa
								   			where
								   				mtmstatus = 'A'
								   			order by
								   				mtmid";
									$db->monta_combo("mtmid",$sql,"S","Selecione...","alteraTipoMeta","","","200","S","","",$mtmid);
								?>
							</td>
						</tr>
						<?php 
						switch($mtmid){
							case 1 : //Prazo
								$display_prazo = "";
								$prazo_obrigatorio = "S";
								$display_qtde = "none";
								$qtde_obrigatorio = "N";
								$display_valor = "none";
								$valor_obrigatorio = "N";
								$display_faixas = "none";
							break;
							case 2 : //Quantidade
								$display_prazo = "none";
								$prazo_obrigatorio = "N";
								$display_qtde = "";
								$qtde_obrigatorio = "S";
								$display_valor = "";
								$valor_obrigatorio = "S";
								$display_faixas = "";
							break;
							case 3 : //Prazo e Quantidade
								$display_prazo = "";
								$prazo_obrigatorio = "S";
								$display_qtde = "";
								$qtde_obrigatorio = "S";
								$display_valor = "";
								$valor_obrigatorio = "S";
								$display_faixas = "";
							break;
							default:
								$display_prazo = "none";
								$prazo_obrigatorio = "S";
								$display_qtde = "none";
								$qtde_obrigatorio = "S";
								$display_valor = "none";
								$valor_obrigatorio = "S";
								$display_faixas = "none";
						} 
						?>
						<tr id="tr_prazo" style="display:<?php echo $display_prazo ?>" >
							<td align='right' class="SubTituloDireita" >Prazo:</td>
							<td>
								<?php $mnmprazo = $mnmprazo ? formata_data($mnmprazo) : "";echo campo_data2("mnmprazo",$prazo_obrigatorio,"S","Prazo","##/##/####","","",$mnmprazo) ?>
							</td>
						</tr>
						<?php if($estid == 2): ?>
							<tr id="tr_faixas" style="display:<?php echo $display_faixas ?>" >
								<td class="SubtituloDireita" >Faixas Avaliativas</td>
								<td>
									<style>
									#demo-frame > div.demo { padding: 10px !important; };
									</style>
									<script>
									$(function() {
										$( "#slider-range" ).slider({
											range: true,
											min: 0,
											max: 100,
											values: [ <?php echo $mnmqtdestavel ? $mnmqtdestavel : 25 ?>, <?php echo $mnmqtdcritico ? $mnmqtdcritico : 75 ?> ],
											slide: function( event, ui ) {
												$( "#valor_critico" ).html( $( "#slider-range" ).slider( "values", 1 ) + "%");
												$( "#valor_ini_estavel" ).html( $( "#slider-range" ).slider( "values", 0 ) + "%");
												$( "#valor_fim_estavel" ).html( $( "#slider-range" ).slider( "values", 1 ) + "%");
												$( "#valor_estavel" ).html( $( "#slider-range" ).slider( "values", 0 ) + "%");
												$( "#hdn_estavel" ).val($( "#slider-range" ).slider( "values", 0 ));
												$( "#hdn_critico" ).val($( "#slider-range" ).slider( "values", 1 ));
											}
										});
										$( "#valor_critico" ).html( $( "#slider-range" ).slider( "values", 1 ) + "%");
										$( "#valor_ini_estavel" ).html( $( "#slider-range" ).slider( "values", 0 ) + "%");
										$( "#valor_fim_estavel" ).html( $( "#slider-range" ).slider( "values", 1 ) + "%");
										$( "#valor_estavel" ).html( $( "#slider-range" ).slider( "values", 0 ) + "%");
										$( "#hdn_estavel" ).val($( "#slider-range" ).slider( "values", 0 ));
										$( "#hdn_critico" ).val($( "#slider-range" ).slider( "values", 1 ));
									});
									</script>
									<div class="demo">
									<div id="slider-range" style="width:300px"></div>
									</div>
									<div style="margin-top:5px;">
									<b>Estável:</b> até <span id="valor_estavel" ></span>.<br />
									<b>Atenção:</b> entre <span id="valor_ini_estavel" ></span> e <span id="valor_fim_estavel" ></span>.<br />
									<b>Crítico:</b> maior que <span id="valor_critico" ></span>.
									<input type="hidden" name="mnmqtdcritico" id="hdn_critico" />
									<input type="hidden" name="mnmqtdestavel" id="hdn_estavel" />
									</div>
								</td>
							</tr>
						<?php else: ?>
							<tr id="tr_faixas" style="display:<?php echo $display_faixas ?>" >
								<td class="SubtituloDireita" >Faixas Avaliativas</td>
								<td>
									<style>
									#demo-frame > div.demo { padding: 10px !important; };
									</style>
									<script>
									$(function() {
										$( "#slider-range" ).slider({
											range: true,
											min: 0,
											max: 100,
											values: [ <?php echo $mnmqtdcritico ? $mnmqtdcritico : 25 ?>, <?php echo $mnmqtdestavel ? $mnmqtdestavel : 75 ?> ],
											slide: function( event, ui ) {
												$( "#valor_critico" ).html( $( "#slider-range" ).slider( "values", 0 ) + "%");
												$( "#valor_ini_estavel" ).html( $( "#slider-range" ).slider( "values", 0 ) + "%");
												$( "#valor_fim_estavel" ).html( $( "#slider-range" ).slider( "values", 1 ) + "%");
												$( "#valor_estavel" ).html( $( "#slider-range" ).slider( "values", 1 ) + "%");
												$( "#hdn_estavel" ).val($( "#slider-range" ).slider( "values", 1 ));
												$( "#hdn_critico" ).val($( "#slider-range" ).slider( "values", 0 ));
											}
										});
										$( "#valor_critico" ).html( $( "#slider-range" ).slider( "values", 0 ) + "%");
										$( "#valor_ini_estavel" ).html( $( "#slider-range" ).slider( "values", 0 ) + "%");
										$( "#valor_fim_estavel" ).html( $( "#slider-range" ).slider( "values", 1 ) + "%");
										$( "#valor_estavel" ).html( $( "#slider-range" ).slider( "values", 1 ) + "%");
										$( "#hdn_estavel" ).val($( "#slider-range" ).slider( "values", 1 ));
										$( "#hdn_critico" ).val($( "#slider-range" ).slider( "values", 0 ));
									});
									</script>
									<div class="demo">
									<div id="slider-range" style="width:300px"></div>
									</div>
									<div style="margin-top:5px;">
									<b>Crítico:</b> até <span id="valor_critico" ></span>.<br />
									<b>Atenção:</b> entre <span id="valor_ini_estavel" ></span> e <span id="valor_fim_estavel" ></span>.<br />
									<b>Estável:</b> maior que <span id="valor_estavel" ></span>.
									<input type="hidden" name="mnmqtdcritico" id="hdn_critico" />
									<input type="hidden" name="mnmqtdestavel" id="hdn_estavel" />
									</div>
								</td>
							</tr>
						<?php endif; ?>
						<tr>
							<td colspan="2" class="SubtituloTabela bold esquerda" >Etapa de Execução</td>
						</tr>
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
							<span id="nome_executor"><? echo $nome_executor; ?></span> 
							<a style="cursor:pointer;" onclick="inserirExecutor(); return false;"><img src="../imagens/gif_inclui.gif" align="absmiddle"> Executor</a>
							<input type="hidden" name="entid_executor" id="entid_executor" value="<? echo $entid_executor; ?>">
							</td>
						</tr>
						<tr id="linha_prazo_execucao" >
							<td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%;">
								<img src="/imagens/seta_filho.gif" />
								<b>Prazo de Execução</b>
							</td>
							<td>
								<?php echo campo_data2('iclprazoexec', 'N', ($trava ? 'N' : 'S'), 'Prazo para Execução', 'S') ?>
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
							<a style="cursor:pointer;" onclick="inserirValidador(); return false;"><img src="../imagens/gif_inclui.gif" align="absmiddle"> Validador</a>
							<input type="hidden" name="entid_validador" id="entid_validador" value="<? echo $entid_validador; ?>">
							</td>
						</tr>
						<tr id="linha_prazo_validacao" >
							<td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%;">
								<img src="/imagens/seta_filho.gif" />
								<b>Prazo de Validação</b>
							</td>
							<td>
								<?php echo campo_data2('iclprazoval', 'N', ($trava ? 'N' : 'S'), 'Prazo para Validação', 'S') ?>
							</td>
						</tr>
						<tr>
							<td class="SubtituloTabela center" colspan="2"  >
								<input type="button" name="btn_salvar" value="Salvar" onclick="salvarMonitoraMeta()" />
								<input type="button" name="btn_voltar" value="Voltar" onclick="window.location.href='estrategico.php?modulo=principal/atividade_estrategico/cadastro_meta&acao=A&atiid=<?php echo $_REQUEST['atiid'] ?>'" />
							</td>
						</tr>
					</table>
			</form>
		</td>
	</tr>
</table>
<?php listarMonitoraMetas($_REQUEST['atiid'],$_REQUEST['micid']) ?>