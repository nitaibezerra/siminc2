<?php
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
$db->cria_aba( $abacod_tela, $url, '&atiid=' . $atividade['atiid']  );
montar_titulo_projeto( "Item de Checklist" );
extract( $atividade ); # mantém o formulário preenchido
?>

<script type="text/javascript" src="../includes/jquery-ui-1.8.18.custom/js/jquery-1.7.1.min.js"></script>
<script language="javascript" type="text/javascript" src="../painel/js/metas_indicador.js"></script>
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
function exibeImportacao()
{
	jQuery("#tr_secid").show();
	jQuery("#tr_acaid").show();
	jQuery("#tr_indid").show();
	jQuery("#tr_salvar").show();
}
function naoImporta()
{
	jQuery.ajax({
		   type: "POST",
		   url: window.location,
		   data: "requisicaoAjax=naoImportaIndicador",
		   success: function(micid){
		   		window.location.href='estrategico.php?modulo=principal/atividade_estrategico/item_checklist&acao=A&atiid=<?php echo $atiid ?>';
		   }
		 });
}
function importarIndicador()
{
	if(!jQuery("[name='indid']").val()){
		alert('Selecione o indicador.');
	}else{
		jQuery.ajax({
		   type: "POST",
		   url: window.location,
		   data: "requisicaoAjax=importaIndicador&indid=" + jQuery("[name='indid']").val(),
		   success: function(micid){
		   		window.location.href='estrategico.php?modulo=principal/atividade_estrategico/item_checklist&acao=A&atiid=<?php echo $atiid ?>&micid=' + micid;
		   }
		 });
	}
}
</script>
<table class="tabela" bgcolor="#f5f5f5" cellspacing="1" cellpadding="5" align="center" >
	<tr>
		<td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%">Deseja utilizar um indicador existente?</td>
		<td>
			<input type="radio" name="rdo_importa" onclick="exibeImportacao()" value="1" /> Sim
			<input type="radio" name="rdo_importa" onclick="naoImporta()" value="0" checked="checked"/> Não
		</td>
	</tr>
	<tr id="tr_secid" style="display:none" >
		<td width="25%" class="SubTituloDireita" >Selecione a Secretaria:</td>
		<td id="td_combo_secid" >
			<?php 
			$secid = $_POST['secid'];
			$sql = "select 
						secid as codigo,
						secdsc as descricao
					from
						painel.secretaria
					where
						secstatus = 'A'
					order by
						secdsc";
			$db->monta_combo('secid',$sql,'S','Selecione a Secretaria','filtrarAcao','');
			?>
		</td>
	</tr>
	<tr id="tr_acaid" style="display:none" >
		<td class="SubTituloDireita" >Selecione a Ação:</td>
		<td id="td_combo_acaid" >
			<?php 
			$acaid = $_POST['acaid'];
			if($secid){
				$habilitado = "S";
				$sql = "select 
							acaid as codigo,
							acadsc as descricao
						from
							painel.acao
						where
							acastatus = 'A'
						and
							acaid in ( select acaid from painel.acaosecretaria where secid = $secid)
						order by
							acadsc";
			}else{
				$habilitado = "N";
				$sql = array();
			}
			$db->monta_combo('acaid',$sql,$habilitado,'Selecione a Ação','filtrarIndicador','');
			?>
		</td>
	</tr>
	<tr id="tr_indid" style="display:none" >
		<td class="SubTituloDireita" >Selecione o Indicador:</td>
		<td id="td_combo_indid" >
			<?php 
			$indid = $_POST['indid'];
			if($secid && $acaid){
				$habilitado = "S";
				$sql = "select 
							indid as codigo,
							indid || ' - ' || indnome as descricao
						from
							painel.indicador
						where
							indstatus = 'A'
						and
							secid = $secid
						and
							acaid = $acaid
						order by
							indid,indnome";
			}else{
				$habilitado = "N";
				$sql = array();
			}
			$db->monta_combo('indid',$sql,$habilitado,'Selecione o Indicador','','');
			?>
		</td>
	</tr>
	<tr id="tr_salvar" style="display:none" >
		<td colspan="2" class="SubtituloTabela center" >
			<input type="button" name="btn_salvar" onclick="importarIndicador()" value="Salvar" />
		</td>
	</tr>
</table>