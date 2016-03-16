<?php
include_once APPRAIZ . 'includes/workflow.php';
include (APPRAIZ.'www/painel/_funcoes_metas.php');

if($_POST['requisicaoAjax']){
	header('content-type: text/html; charset=ISO-8859-1');
	$_POST['requisicaoAjax']();
	exit();
}
if($_POST['requisicao']){
	$_POST['requisicao']();
}

$_SESSION['estrategico']['nao_importar_indicador'] = true;

if(!$_GET['micid'] && !$_SESSION['estrategico']['nao_importar_indicador']):
	include "importa_check_painel.inc";
else:
	unset($_SESSION['estrategico']['nao_importar_indicador']);

include_once APPRAIZ . 'includes/workflow.php';

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

if($_REQUEST['micid']){
	extract(recuparMonitoraItemCheckList($_REQUEST['micid']));
	$acaid_indicador = $acaid;

	if($indid){
		$sql = "select mtiid from painel.indicador where indid = $indid";
		$mtiid = $db->pegaUm($sql);
	}
}
$permissao = $micimportado == "t" ? "N" : "S";

if($mnmid){
	extract(recuparMonitoraMeta($mnmid));
	extract(recuparMonitoraMetaExecutor($mnmid));
	extract(recuparMonitoraMetaValidador($mnmid));
}else{
	extract(recuperaDadosProjeto($_SESSION['projeto']));
}

if($indid){
	$sql = "select mtiid from painel.indicador where indid = $indid";
	$mtiid = $db->pegaUm($sql);
}

// ----- CABEÇALHO
include APPRAIZ . 'includes/cabecalho.inc';
print '<br/>';
if(!$mnmid){
	$db->cria_aba( $abacod_tela, $url, '&atiid=' . $atividade['atiid']  );
}else{
	$menu = array(0 => array("id" => 1, "descricao" => "Listar itens de checklist", "link" => "estrategico.php?modulo=principal/atividade_estrategico/listar_checklist&acao=A&atiid=$atiid"),
				  1 => array("id" => 2, "descricao" => "Item de Checklist", 		"link" => "estrategico.php?modulo=principal/atividade_estrategico/item_checklist&acao=A&atiid=$atiid&micid=$micid"),
				  2 => array("id" => 3, "descricao" => "Metas do Checklist",    	"link" => "estrategico.php?modulo=principal/atividade_estrategico/meta_checklist&acao=A&atiid=$atiid&micid=$micid"),
		  	  );
	echo montarAbasArray($menu,"estrategico.php?modulo=principal/atividade_estrategico/item_checklist&acao=A&atiid=$atiid&micid=$micid");
}
montar_titulo_projeto( "Item de Checklist" );

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
function salvarMonitoraItemChecklist()
{
	var erro = 0;
		jQuery("[class~=obrigatorio]").each(function() {
			if(this.disabled == false && !this.value || this.value == "Selecione..."){
				erro = 1;
				alert('Favor preencher todos os campos obrigatórios.');
				this.focus();
				return false;
			}
		});

	selectAllOptions( document.getElementById( 'aggid' ) );
//	selectAllOptions( document.getElementById( 'indid_vinculado' ) );

	if(erro == 0){
		jQuery("[name='requisicao']").val("cadastrarMonitoraItemCheckList");
		jQuery("[name='form_item_check_list']").submit();
	}
}

function adicionarMonitoraMeta(micid)
{
	window.location.href='estrategico.php?modulo=principal/atividade_estrategico/cadastro_meta&acao=A&atiid=<?php echo $atiid ?>&micid=' + micid;
}

function editarMonitoraItemChecklist(micid)
{
	window.location.href='estrategico.php?modulo=principal/atividade_estrategico/cadastro_checklist&acao=A&atiid=<?php echo $atiid ?>&micid=' + micid;
}

function excluirMonitoraItemChecklist(micid)
{
	if(confirm("Deseja realmente excluir o Item de Check List e todas as suas Metas?")){
		jQuery("[name='micid']").val(micid);
		jQuery("[name='requisicao']").val("excluirMonitoraItemChecklist");
		jQuery("[name='form_item_check_list']").submit();
	}
}

/**
 * Alterar visibilidade de um bloco.
 * @param string indica o bloco a ser mostrado/escondido
 * @return void
 */
function onOffBloco( bloco )
{
	var div_on = document.getElementById( bloco + '_div_filtros_on' );
	var div_off = document.getElementById( bloco + '_div_filtros_off' );
	var img = document.getElementById( bloco + '_img' );
	var input = document.getElementById( bloco + '_flag' );
	if ( div_on.style.display == 'none' )
	{
		div_on.style.display = 'block';
		div_off.style.display = 'none';
		input.value = '0';
		img.src = '/imagens/menos.gif';
	}
	else
	{
		div_on.style.display = 'none';
		div_off.style.display = 'block';
		input.value = '1';
		img.src = '/imagens/mais.gif';
	}
}

/**
 * Alterar visibilidade de um campo.
 * @param string indica o campo a ser mostrado/escondido
 * @return void
 */
function onOffCampo( campo )
{
	var div_on = document.getElementById( campo + '_campo_on' );
	var div_off = document.getElementById( campo + '_campo_off' );
	var input = document.getElementById( campo + '_campo_flag' );
	if ( div_on.style.display == 'none' )
	{
		div_on.style.display = 'block';
		div_off.style.display = 'none';
		input.value = '1';
	}
	else
	{
		div_on.style.display = 'none';
		div_off.style.display = 'block';
		input.value = '0';
	}
}

function inserirExecutor()
{
	janela("estrategico.php?modulo=principal/atividade_estrategico/inserir_executor_meta&acao=A&atiid=<?=$_REQUEST['atiid']?>&entid_executor="+document.getElementById('entid_executor').value,800,580,"att_exec");
}

function inserirValidador()
{
	janela("estrategico.php?modulo=principal/atividade_estrategico/inserir_validador_meta&acao=A&atiid=<?=$_REQUEST['atiid']?>&entid_validador="+document.getElementById('entid_validador').value,800,580,"att_valid");
}

function addMeta(metid)
{
	janela("estrategico.php?modulo=principal/atividade_estrategico/valor_meta&acao=A&metid="+metid,400,250,"att_exec");
}

function alteraTipoMeta(valor)
{
	if(valor == 1){ //Prazo
		jQuery("[name='unmid']").val(6);jQuery("[name='unmid']").attr("class","normal");
		jQuery("[name='umeid']").val(82);jQuery("[name='umeid']").attr("class","normal");
		jQuery("#tr_unmid").hide();
		jQuery("#tr_acumulado").hide();
		jQuery("#tr_dados_coleta").hide();
		jQuery("[name='estid']").val(3);jQuery("[name='estid']").attr("class","normal");
		jQuery("#tr_estid").hide();
		jQuery("[name='colid']").val(1);jQuery("[name='colid']").attr("class","normal");
		jQuery("#tr_colid").hide();
		jQuery("#tr_dados_exibicao").hide();
		jQuery("#tr_umeid").hide();
		jQuery("#tr_periodicidade").hide();
		jQuery("[name='perid']").val(5);
	}else{
		jQuery("[name='unmid']").val("");jQuery("[name='unmid']").attr("class","CampoEstilo obrigatorio");
		jQuery("[name='umeid']").val("");jQuery("[name='umeid']").attr("class","CampoEstilo obrigatorio");
		jQuery("#tr_unmid").show()
		jQuery("#tr_acumulado").show();
		jQuery("#tr_dados_coleta").hide();
		jQuery("[name='estid']").val(1);jQuery("[name='estid']").attr("class","CampoEstilo obrigatorio");
		jQuery("#tr_estid").hide();
		jQuery("[name='colid']").val(1);jQuery("[name='colid']").attr("class","CampoEstilo obrigatorio");
		jQuery("#tr_colid").hide();
		jQuery("#tr_dados_exibicao").show();
		jQuery("#tr_umeid").show();
		jQuery("#tr_periodicidade").hide();
	}

}

function editarValorMeta(perid,dmiid,dpeid,dmiqtde,dmivalor)
{
	janela("estrategico.php?modulo=principal/atividade_estrategico/valor_meta&acao=A&metid=<?php echo $metid ?>&dmiid=" + dmiid,400,250,"att_exec");
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

function verificaMetasCadastradas(micid){
	if(micid){
		var params = "requisicaoAjax=verificaMetasCadastradas&micid=" + micid;
		$.ajax({
		   type: "POST",
		   url: window.location,
		   data: params,
		   success: function(msg){
		   		if(msg){
		   			if(confirm("Ao alterar a periodicidade é necessário apagar todas as metas já cadastradas. Deseja prosseguir?")){
						var params = "requisicaoAjax=excluirTodasMetas&micid=" + micid;
						$.ajax({
						   type: "POST",
						   url: window.location,
						   data: params,
						   success: function(resultado){
						   }
						 });
					}else{
						$("[name='perid']").val(msg);
					}
		   		}
		   }
		 });
	}
}

function fake(id)
{

}

function filtrarIndicador( indid, idvid ){
	var micid = $('#micid').val();
	janela('?modulo=principal/atividade_estrategico/filtrar_indicador&acao=A&micid=' + micid + '&indid=' + indid + '&idvid=' + idvid, 900, 600, 'filtrarIndicador' );
}

function excluirIndicador( indid ){
	$('#td_indicador table tbody tr td input[value=' + indid + ']').parents('tr:first').remove();
}

$(document).ready(function (){
	if ( $('#td_indicador [name^=indid_vinculado]').size() > 3 ){
		$('#td_indicador').css({height : '150px', overflow : 'auto'});
	}else if ( $('#td_indicador [name^=indid_vinculado]').size() < 4 ){
		$('#td_indicador').css({height : '', overflow : ''});
	}



	//verifica se foram incluidos ou removidos indicadores vinculados e salva
    //$('#td_indicador').bind('DOMNodeInserted', function(event) {
    	//if ( $('#td_indicador [name^=indid_vinculado]').size() >= 0 );
    		//salvarMonitoraItemChecklist();

 //  window.setInterval(salvarMonitoraItemChecklist, 2000);
    		//alert($('#td_indicador [name^=indid_vinculado]').size());
 //   });

});



</script>


<table class="tabela" bgcolor="#fbfbfb" cellspacing="0" cellpadding="10" align="center">
	<tr>
		<td>
			<?= montar_resumo_atividade( $atividade ) ?>
				<form method="post" name="form_item_check_list" id="form_item_check_list" >
					<input type="hidden" id="micid" name="micid" value="<?php echo $micid; ?>" />
					<input type="hidden" id="atiid" name="atiid" value="<?php echo $_REQUEST['atiid'] ?>" />
					<input type="hidden" id="requisicao" name="requisicao" value=""/>
					<input type="hidden" name="secid" value="18" />
					<input type="hidden" name="secidgestora" value="18" />
					<input type="hidden" name="regid" value="1" />
					<input type="hidden" name="perid" value="5" />
					<table class="tabela" bgcolor="#f5f5f5" cellspacing="1" cellpadding="5" style="width: 100%;">
						<tr>
							<td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%">Descrição:</td>
							<td>
								<?php echo campo_texto('indnome','S',$permissao,'',60, 500,'','',null,null,null,'id="indnome"'); ?>
							</td>
						</tr>
						<tr>
							<td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%">Apelido:</td>
							<td>
								<?php echo campo_texto('indapelido','N',$permissao,'',25, 20,'','',null,null,null,'id="indapelido"'); ?>
							</td>
						</tr>
						<tr>
							<td align='right' class="SubTituloDireita" >Eixo:</td>
							<td>
								<?php
									$sql= "SELECT
												exodsc AS descricao,
												exoid AS codigo
											FROM
												painel.eixo
											order by
												 descricao";
									$db->monta_combo("exoid",$sql,$permissao,"Selecione...","","","","200","S","","",$exoid);
								?>
							</td>
						</tr>
						<tr>
							<td align='right' class="SubTituloDireita" >Ação:</td>
							<td>
								<?php
									$sql= "select
												acaid as codigo,
												acadsc as descricao
											from
												painel.acao
											where
												acastatus = 'A'
											order by
												acadsc";
									$acaid = $acaid_indicador;
									$db->monta_combo("acaid",$sql,$permissao,"Selecione...","","","","200","S","","",$acaid_indicador);
								?>
							</td>
						</tr>
						<tr>
							<td align='right' class="SubTituloDireita">Agenda(s) de Governo(s):</td>
							<td>
							<?php
							$Sql = "SELECT
										aggdsc AS descricao,
										aggid AS codigo
									FROM
										painel.agendagoverno
									WHERE
										aggstatus = 'A'
									order by
										 descricao";
							if($indid){
								$SqlCarregado = "SELECT
													aggdsc AS descricao,
													agi.aggid AS codigo
												FROM
													painel.agendagoverno age
												INNER JOIN
													painel.agendaindicador agi ON age.aggid = agi.aggid
												WHERE
													aggstatus = 'A'
												and
													indid = $indid
												order by
													 descricao";
								$aggid = $db->carregar($SqlCarregado);
							}else{
								if($agenda_projeto){
									$SqlCarregado = "SELECT
													aggdsc AS descricao,
													agi.aggid AS codigo
												FROM
													painel.agendagoverno age
												INNER JOIN
													pde.agendaatividade agi ON age.aggid = agi.aggid
												WHERE
													aggstatus = 'A'
												and
													atiid = $agenda_projeto
												order by
													 descricao";
								$aggid = $db->carregar($SqlCarregado);
								}
							}
							combo_popup( 'aggid', $Sql, 'Selecione a(s) Agenda(s) de Governo(s)', '360x460', 0, array(), "", $permissao, false, false, 5, 400);
							?>
							</td>
						</tr>
                        <tr>
                            <td align='right' class="SubTituloDireita">Observação:</td>
                            <td><?=campo_textarea( 'indobservacao', 'N', 'S', '', 98, 5, 2000 );?></td>
                        </tr>
                        <tr>
							<td align='right' class="SubTituloDireita" >Tipo de Checklist:</td>
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

									switch($indqtdevalor){
										case "t":
											$mtmid = 3;
											break;
										case "f":
											if($unmid == UNIDADEMEDICAO_BOLEANA){
												$mtmid = 1;
											}else{
												$mtmid = 2;
											}
											break;
									}
									if($mtmid == 3){
										$mtmid = 2;
									}
									$db->monta_combo("mtmid",$sql,($mtmid ? $permissao : "S"),"Selecione...","alteraTipoMeta","","","200","S","","",$mtmid);

									if($mtmid == 1){
										$display_campos = "none";
									}
								?>
							</td>
						</tr>
						<tr>
							<td align='right' class="SubTituloDireita" >Tipo de Indicador:</td>
							<td>
								<?php
									$sql = "select
								   				mtiid as codigo,
								   				mtidsc as descricao
								   			from
								   				pde.monitoratipoindicador
								   			where
								   				mtistatus = 'A'
								   			order by
								   				mtidsc";
									$db->monta_combo("mtiid",$sql,($mtiid ? $permissao : "S"),"Selecione...","","","","200","S","","",$mtiid);
								?>
							</td>
						</tr>
						<tr>
							<td align='right' class="SubTituloDireita" >Crítico:</td>
							<td>
								<input type='checkbox' name='micestrategico' id='micestrategico' <?php  echo $micestrategico=='t'?'checked':'' ?>>
							</td>
						</tr>
						<tr>
							<td align='right' class="SubTituloDireita" >Envia SMS Atraso:</td>
							<td>
								<input type='checkbox' name='micenviasms' id='micenviasms' <?php  echo $micenviasms=='t'?'checked':'' ?>>
								<span class="span-vincular-responsaveis" <?php echo $micenviasms=='t' ? '' : 'style="display: none;"'; ?>>
									<a href="javascript:janela('?modulo=principal/atividade_estrategico/testesms&acao=A&micid=<?php echo $micid; ?>', 400, 250, 'testarSMS' );">Enviar SMS Teste</a>
								</span>
							</td>
						</tr>

						<?php if($_REQUEST['micid']):?>
							<tr class="span-vincular-responsaveis" <?php echo $micenviasms=='t' ? '' : 'style="display: none;"'; ?>>
								<td align='right' class="SubTituloDireita">SMS - Responsáveis vinculados:</td>
								<td>
									<?php
										$sql = "select distinct usuario.usunome, '(' || dddcelular || ') ' || celular as celular
										FROM seguranca.usuario usuario
											inner join pde.notificacaometaresponsavel nmr on nmr.usucpf = usuario.usucpf
											left join pde.responsavelcelular rc on nmr.usucpf = rc.usucpf
										where nmr.micid = '{$_REQUEST['micid']}'";
									?>
									<a href="javascript:janela('?modulo=principal/atividade_estrategico/vincular_responsavel_notificacao&acao=A&micid=<?php echo $_REQUEST['micid']; ?>', 800, 600, 'vincularResponsavel' );">Vincular Responsáveis Cobrança SMS</a>
									<?php
									$cabecalho = array( "Nome", "Celular" );
									$db->monta_lista_simples($sql, $cabecalho, 100, 100, 'N');
									?>
								</td>
							</tr>
						<?php endif;?>
						<tr>
							<td align='right' class="SubTituloDireita" >Etapa:</td>
							<td>
								<select name=micetapa id=micetapa>
									<option value=''>Selecione ...</option>
									<option value='I' <?=$micetapa == 'I'? 'SELECTED' : ''?>>Implantação</option>
									<option value='E' <?=$micetapa == 'E'? 'SELECTED' : ''?>>Execução</option>
								</select>
							</td>
						</tr>
						<tr id="tr_periodicidade" style="display:<?php echo 'none'; //$display_campos ?>" >
							<td align='right' class="SubTituloDireita" >Periodicidade:</td>
							<td>
								<?php
									$sql = "select
								   				perid as codigo,
								   				perdsc as descricao
								   			from
								   				painel.periodicidade
								   			where
								   				perstatus = 'A'
								   			order by
								   				pernivel";
									$perid = 5;
									$db->monta_combo("perid",$sql,$permissao,"Selecione...","verificaMetasCadastradas($micid);fake","","","200","S","","",$perid);
								?>
							</td>
						</tr>
						<tr id="tr_dados_exibicao" style="display:<?php echo $display_campos ?>" >
							<td colspan="2" class="SubtituloTabela bold esquerda" >Dados de Exibição</td>
						</tr>
						<tr id="tr_unmid" style="display:<?php echo $display_campos ?>" >
							<td align='right' class="SubTituloDireita" >Unidade de Medição:</td>
							<td>
								<?php
									$sql = "select
								   				unmid as codigo,
								   				unmdesc as descricao
								   			from
								   				painel.unidademedicao
								   			where
								   				unmstatus = 'A'
								   			order by
								   				unmdesc";
									$db->monta_combo("unmid",$sql,$permissao,"Selecione...","","","","200","S","","",$unmid);
								?>
							</td>
						</tr>
						<tr id="tr_umeid" style="display:<?php echo $display_campos ?>" >
							<td align='right' class="SubTituloDireita" >Exibir produto da unidade como:</td>
							<td>
								<?php
									$sql = "select
								   				umeid as codigo,
								   				umedesc as descricao
								   			from
								   				painel.unidademeta
								   			where
								   				umestatus = 'A'
								   			order by
								   				umedesc";
									$db->monta_combo("umeid",$sql,$permissao,"Selecione...","","","","200","S","","",$umeid);
								?>
							</td>
						</tr>
						<tr style="display:none" >
							<td align='right' class="SubTituloDireita" >Coletar valor monetário?</td>
							<td>
								<input <?php echo $permissao == "N" ? "disabled='disabled'" : "" ?> type="radio" <?php echo !$indqtdevalor || $indqtdevalor == "f" ? "checked='checked'" : "" ?> name="indqtdevalor" value="false" /> Não
								<input <?php echo $permissao == "N" ? "disabled='disabled'" : "" ?> type="radio" <?php echo $indqtdevalor == "t" ? "checked='checked'" : "" ?> name="indqtdevalor" value="true" /> Sim
							</td>
						</tr>
						<tr id="tr_acumulado" style="display:<?php echo $display_campos ?>" >
							<td align='right' class="SubTituloDireita" >Exibir dados acumulados entre os períodos?</td>
							<td>
								<input <?php echo $permissao == "N" ? "disabled='disabled'" : "" ?> type="radio" <?php echo !$indcumulativo || $indcumulativo == "N" ? "checked='checked'" : "" ?> name="indcumulativo" value="N" /> Não
								<input <?php echo $permissao == "N" ? "disabled='disabled'" : "" ?> type="radio" <?php echo $indcumulativo == "S" ? "checked='checked'" : "" ?> name="indcumulativo" value="S" /> Sim
								<input <?php echo $permissao == "N" ? "disabled='disabled'" : "" ?> type="radio" <?php echo $indcumulativo == "A" ? "checked='checked'" : "" ?> name="indcumulativo" value="A" /> Por Ano
							</td>
						</tr>
						<tr id="tr_dados_coleta" style="display:<?php echo 'none'; $display_campos ?>" >
							<td colspan="2" class="SubtituloTabela bold esquerda" >Dados sobre a Coleta</td>
						</tr>
						<tr id="tr_estid" style="display:<?php echo 'none'; //$display_campos ?>" >
							<td align='right' class="SubTituloDireita" >Estilo:</td>
							<td>
								<?php
									$sql = "select
								   				estid as codigo,
								   				estdsc as descricao
								   			from
								   				painel.estilo
								   			where
								   				eststatus = 'A'
								   			order by
								   				estdsc";
									$estid = 1;
									$db->monta_combo("estid",$sql,$permissao,"Selecione...","","","","200","S","","",$estid);
								?>
							</td>
						</tr>
						<tr id="tr_colid" style="display:<?php echo 'none'; //$display_campos ?>" >
							<td align='right' class="SubTituloDireita" >Coleta:</td>
							<td>
								<?php
									$arrColeta[0] = array("codigo" => 2, "descricao" => "Automática");
									$arrColeta[1] = array("codigo" => 1, "descricao" => "Manual");
									$colid = 1;
									$db->monta_combo("colid",$arrColeta,$permissao,"Selecione...","","","","200","S","","",$colid);
								?>
							</td>
						</tr>
						<tr style="display:none" >
							<td colspan="2" class="SubtituloTabela bold esquerda" >Cronograma de Atualização</td>
						</tr>
						<tr style="display:none" >
							<td align='right' class="SubTituloDireita" >Periodicidade de atualização:</td>
							<td>
								<?php
									$sql = "select
								   				perid as codigo,
								   				perdsc as descricao
								   			from
								   				painel.periodicidade
								   			where
								   				perstatus = 'A'
								   			order by
								   				pernivel";
									$db->monta_combo("peridatual",$sql,$permissao,"Selecione...","","","","200","S","","",5);
								?>
							</td>
						</tr>
						<tr>
							<td colspan="2" class="SubtituloTabela bold esquerda" >Indicador(es) Vinculado(s) (Pai)&nbsp;&nbsp;<a href="javascript:janela('?modulo=principal/atividade_estrategico/vincular_indicador&acao=A', 500, 400, 'vincularIndicador' );">Vincular Indicador</a></td>
						</tr>
						<tr>
							<td colspan="2">
								<div id="td_indicador">
									<input id="indid_vinculado_meta" name="indid_vinculado_meta" type="hidden">
									<input id="indid_filtro_meta" name="indid_filtro_meta" type="hidden">
									<input id="idvid_meta" name="idvid_meta" type="hidden">
									<input id="idvdsc_meta" name="idvdsc_meta" type="hidden">
									<input id="idvmeta_meta" name="idvmeta_meta" type="hidden">
									<input id="idvdatameta_meta" name="idvdatameta_meta" type="hidden">
									<input id="nova_meta" name="nova_meta" type="hidden">
								<?php
								if ($indid){
									$sql = "SELECT
												'<div style=\"width:100%; text-align:center;\">
													<input name=\"indid_vinculado[]\" value=\"' || ind.indid || '\" type=\"hidden\">
													<input id=\"indid_filtro_' || ind.indid || '\" name=\"indid_filtro[]\" value=\"' || COALESCE(vin.idvfiltro, '') || '\" type=\"hidden\">
													<input id=\"idvdsc_' || ind.indid || '\" name=\"idvdsc[]\" value=\"' || COALESCE(vin.idvdsc, '') || '\" type=\"hidden\">
													<input id=\"idvmeta_' || ind.indid || '\" name=\"idvmeta[]\" value=\"' || REPLACE (COALESCE(vin.idvmeta::text, ''),'.',',') || '\" type=\"hidden\">
													<input id=\"idvdatameta_' || ind.indid || '\" name=\"idvdatameta[]\" type=\"hidden\">
													<img class=\"link img_middle\" src=\"../imagens/principal.gif\" title=\"Detalhar Indicador\" onclick=\"filtrarIndicador(' || ind.indid || ', ' || vin.idvid || ')\">
												 	<img class=\"link img_middle\" src=\"../imagens/excluir.gif\" title=\"Excluir Indicador\" onclick=\"excluirIndicador(' || ind.indid || ')\">
												 </div>' AS codigo,
												CASE WHEN vin.idvdsc = '' OR vin.idvdsc IS NULL
													THEN
														ind.indid || ' - ' || indnome
													ELSE
														vin.idvdsc || '<br>( ' || ind.indid || ' - ' || indnome || ' )'
												END AS descricao
											FROM
												painel.indicador ind
											INNER JOIN
												painel.indicadoresvinculados vin ON vin.indid = ind.indid
											WHERE
												idvstatus = 'A'
											and
												vin.indidvinculo = $indid
											order by
												 descricao";
								}else{
									$sql = array();
								}
								$cabecalho = array( "Ação", "Indicador" );
								$db->monta_lista_simples($sql, $cabecalho, 100, 100, 'N', '', '' );
								?>
								</div>
							</td>
						</tr>
<!--
						<tr>
							<td align='right' class="SubTituloDireita">Indicador(es) Vinculado(s): (Pai)</td>
							<td>
							<?php
//							$Sql = "SELECT
//										indid || ' - ' || indnome AS descricao,
//										indid AS codigo
//									FROM
//										painel.indicador
//									where
//										(indstatus = 'A' or (indstatus = 'I' and indhomologado is true) )
//									order by
//										 descricao";
//							if($indid){
//								$SqlCarregado = "SELECT
//													ind.indid || ' - ' || indnome AS descricao,
//													ind.indid AS codigo
//												FROM
//													painel.indicador ind
//												INNER JOIN
//													painel.indicadoresvinculados vin ON vin.indid = ind.indid
//												WHERE
//													idvstatus = 'A'
//												and
//													vin.indidvinculo = $indid
//												order by
//													 descricao";
//
//								$indid_vinculado = $db->carregar($SqlCarregado);
//							}
//							combo_popup( 'indid_vinculado', $Sql, 'Selecione o(s) Indicador(es) ', '360x460', 0, array(), "", $permissao, true, false, 5, 400,"","","",array(
//								  array(
//								  		"codigo" 	=> "indnome",
//								  		"descricao" => "Indicador"
//								  		)
//								  )	,"",true,true);
							?>
							</td>
						</tr>
-->
						</tr>
							<td class="SubtituloTabela center" colspan="2"  >
								<input type="button" name="btn_salvar" value="Salvar" onclick="salvarMonitoraItemChecklist()" />
								<input type="button" name="btn_voltar" value="Voltar" onclick="window.location.href='estrategico.php?modulo=principal/atividade_estrategico/listar_checklist&acao=A&atiid=<?php echo $atiid ?>'" />
							</td>
						</tr>
					</table>
			</form>
		</td>
	</tr>
</table>
<?php endif; ?>


<script type="text/javascript">
	$(function(){
		$('#micenviasms').click(function(){
			if($('#micenviasms').is(':checked')){
				$('.span-vincular-responsaveis').show();
			} else {
				$('.span-vincular-responsaveis').hide();
			}
		});
	});
</script>