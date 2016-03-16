<?php
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

if($_REQUEST['micid']){
	extract(recuparMonitoraItemCheckList($_REQUEST['micid']));
}

?>

<script type="text/javascript" src="/includes/JQuery/jquery-1.4.2.min.js"></script>
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
jQuery.noConflict();

function salvarMonitoraItemChecklist()
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
	
	selectAllOptions( document.getElementById( 'aggid' ) );
	
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
							<td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%">Descriçao do item de Checklist:</td>
							<td>
								<?php echo campo_texto('indnome','S',($trava ? 'N' : 'S'),'',60, 500,'','',null,null,null,'id="indnome"'); ?>
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
									WHERE aggstatus = 'A'
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
							}
							combo_popup( 'aggid', $Sql, 'Selecione a(s) Agenda(s) de Governo(s)', '360x460', 0, array(), "", "S", false, false, 5, 400);
							?>
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
												 descricao
											";
									$db->monta_combo("exoid",$sql,"S","Selecione...","","","","200","S","","",$exoid);
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
											and
												acaid in ( select acaid from painel.acaosecretaria where secid = 18)";
									$db->monta_combo("acaid",$sql,"S","Selecione...","","","","200","S","","",$acaid);
								?>
							</td>
						</tr>
						<tr>
							<td colspan="2" class="SubtituloTabela bold esquerda" >Dados de Exibição</td>
						</tr>
						<tr>
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
									$db->monta_combo("unmid",$sql,"S","Selecione...","","","","200","S","","",$unmid);
								?>
							</td>
						</tr>
						<tr>
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
									$db->monta_combo("umeid",$sql,"S","Selecione...","","","","200","S","","",$umeid);
								?>
							</td>
						</tr>
						<tr>
							<td align='right' class="SubTituloDireita" >Coletar valor monetário?</td>
							<td>
								<input type="radio" <?php echo !$indqtdevalor || $indqtdevalor == "f" ? "checked='checked'" : "" ?> name="indqtdevalor" value="false" /> Não 
								<input type="radio" <?php echo $indqtdevalor == "t" ? "checked='checked'" : "" ?> name="indqtdevalor" value="true" /> Sim 
							</td>
						</tr>
						<tr>
							<td align='right' class="SubTituloDireita" >Exibir dados acumulados entre os períodos?</td>
							<td>
								<input type="radio" <?php echo !$indcumulativo || $indcumulativo == "N" ? "checked='checked'" : "" ?> name="indcumulativo" value="N" /> Não 
								<input type="radio" <?php echo $indcumulativo == "S" ? "checked='checked'" : "" ?> name="indcumulativo" value="S" /> Sim 
								<input type="radio" <?php echo $indcumulativo == "A" ? "checked='checked'" : "" ?> name="indcumulativo" value="A" /> Por Ano
							</td>
						</tr>
						<tr>
							<td align='right' class="SubTituloDireita" >Fórmula:</td>
							<td>
								<?php echo campo_textarea("indformula","N","S","",80,5,255,"","") ?>
							</td>
						</tr>
						<tr>
							<td align='right' class="SubTituloDireita" >Termos:</td>
							<td>
								<?php echo campo_textarea("indtermos","N","S","",80,5,255,"","") ?>
							</td>
						</tr>
						<tr>
							<td align='right' class="SubTituloDireita" >Fontes por Termo:</td>
							<td>
								<?php echo campo_textarea("indfontetermo","N","S","",80,5,255,"","") ?>
							</td>
						</tr>
						<tr>
							<td colspan="2" class="SubtituloTabela bold esquerda" >Dados sobre a Coleta</td>
						</tr>
						<tr>
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
									$db->monta_combo("estid",$sql,"S","Selecione...","","","","200","S","","",$estid);
								?>
							</td>
						</tr>
						<tr>
							<td align='right' class="SubTituloDireita" >Coleta:</td>
							<td>
								<?php
									$arrColeta[0] = array("codigo" => 2, "descricao" => "Automática");
									$arrColeta[1] = array("codigo" => 1, "descricao" => "Manual");
									$db->monta_combo("colid",$arrColeta,"S","Selecione...","","","","200","S","","",$colid);
								?>
							</td>
						</tr>
						<tr>
							<td colspan="2" class="SubtituloTabela bold esquerda" >Cronograma de Atualização</td>
						</tr>
						<tr>
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
									$db->monta_combo("peridatual",$sql,"S","Selecione...","","","","200","S","","",$peridatual);
								?>
							</td>
						</tr>
							<td class="SubtituloTabela center" colspan="2"  >
								<input type="button" name="btn_salvar" value="Salvar" onclick="salvarMonitoraItemChecklist()" />
							</td>
						</tr>
					</table>
			</form>
		</td>
	</tr>
</table>
<?php listarMonitoraItemCheckList($_REQUEST['atiid']) ?>