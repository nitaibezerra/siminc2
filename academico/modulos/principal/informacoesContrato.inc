<?php

if($_REQUEST['requisicao'] == 'excluirObjeto'){
	$sql = "delete from academico.objetocontrato where obcid = ".$_REQUEST['obcid'];
	$db->executar($sql);
	if($db->commit()){
		die('true');
	}
	die('false');	
}

if($_REQUEST['requisicao'] == 'salvaDados'){
	
	$valor = str_replace(array(".",","),array("","."),$_REQUEST['obcvalor']);
	$dtinicio = formata_data_sql($_REQUEST['obcdtinicio']);
	$dtfim = formata_data_sql($_REQUEST['obcdtfim']);
	
	if(empty($_REQUEST['obcid'])){
	
		$sql = "insert into academico.objetocontrato 
				(
					obcdsc,
					obcstatus, 
					tocid, 
					stcid, 
					obcvalor, 
					obcdtinicio, 
					obcdtfim, 
					entid
				)
				values
				(
					'{$_REQUEST['obcdsc']}',
					'A',
					{$_REQUEST['tocid']},
					{$_REQUEST['stcid']},
					'{$valor}',
					'{$dtinicio}',
					'{$dtfim}',
					{$_SESSION['academico']['entid']}
				)";
	}else{
		
		$sql = "update academico.objetocontrato 
					set obcdsc = '{$_REQUEST['obcdsc']}',					 
						tocid = {$_REQUEST['tocid']}, 
						stcid = {$_REQUEST['stcid']}, 
						obcvalor = '{$valor}', 
						obcdtinicio = '{$dtinicio}', 
						obcdtfim = '{$dtfim}', 
						entid = {$_SESSION['academico']['entid']}
				where obcid = ".$_REQUEST['obcid'];
	}
				
	$db->executar($sql);
	if($db->commit()){
		$db->sucesso('principal/informacoesContrato');	
	}	
}

if($_REQUEST['requisicao'] == 'recuperaDados'){
	$sql = "select 
				* 
			from 
				academico.objetocontrato obj
			inner join 
				 academico.tipoobjetocontrato toc on toc.tocid = obj.tocid
			where 
				obcid = ".$_REQUEST['obcid'];
	$rsDados = $db->pegaLinha($sql);
	echo simec_json_encode($rsDados);
	die;
}

function permissaoAlterar() {
	global $db;	
	$arrPerfil = pegaPerfilGeral();
	if( in_array(PERFIL_SUPERUSUARIO,$arrPerfil)  || 
		in_array(PERFIL_ADMINISTRADOR,$arrPerfil) || 
		in_array(PERFIL_REITOR,$arrPerfil)        || 
		in_array(PERFIL_IFESCADBOLSAS,$arrPerfil) || 
		in_array(PERFIL_IFESCADCURSOS,$arrPerfil) || 
		in_array(PERFIL_IFESCADASTRO,$arrPerfil)  ||
		in_array(PERFIL_CADASTROGERAL,$arrPerfil)){
		return true;
	}else{
		return false;
	}	
}

function pegarMomento() {
	global $db;
	$sql = "SELECT slfid FROM academico.sldfase WHERE slfdatainicio<='".date("Y-m-d")."' AND slfdatafim>='".date("Y-m-d")."'";
	$slfid = $db->pegaUm($sql);
	return $slfid;
}

if($_REQUEST['requesicao'] == 'populaCombo'){
	
	$sql = "select 
				tocid as codigo,
				tocdsc as descricao  
			from 
				academico.tipoobjetocontrato
			where tpcid = ".$_REQUEST['tpcid'];
	
	$db->monta_combo('tocid', $sql, 'S', 'Selecione..', 'verificaOutros', '');
	
	echo '&nbsp;<span id="sp_outros" style="display:none">';
	echo campo_texto('obcdsc','N','S','',20,50, '', '', '', '', 'S');
	echo '</span>';
	die;
}

require_once APPRAIZ . "includes/cabecalho.inc";
echo '<br/>';

$permissaoAlterar = permissaoAlterar();

$abacod_tela = $_SESSION['academico']['orgid'] == '2' ? '57334' : '57335';

$db->cria_aba($abacod_tela,$url,$parametros);
academico_monta_cabecalho_sig($_SESSION['academico']['entid'], 'Informações sobre o contrato');
?>
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<script>

jQuery.noConflict();

jQuery(function(){
	
});

function popupaObjeto(id, tocid)
{
	jQuery('#td_tocid').html('Carregando...');
	jQuery.ajax({
		url: 'academico.php?modulo=principal/informacoesContrato&acao=A',
		type: 'post',
		data: 'requesicao=populaCombo&tpcid='+id,
		success: function(e){
			jQuery('#td_tocid').html(e);
			if(tocid){
				jQuery('[name=tocid]').val(tocid);
			}
		}
	});
}

function verificaOutros(id)
{	
	if(id == '7' || id == '11'){		
		jQuery('#sp_outros').show();
	}else{
		jQuery('#sp_outros').hide();
	}	
}

function salvaDados()
{
	if(jQuery('[name=tpcid]').val() == ''){
		alert('O campo Tipo de contrato é obrigatório!');
		jQuery('[name=tpcid]').focus();
		return false;
	}
	if(jQuery('[name=stcid]').val() == ''){
		alert('O campo Situação do contrato é obrigatório!');
		jQuery('[name=stcid]').focus();
		return false;
	}	
	if(jQuery('[name=tocid]').val() == ''){
		alert('O campo Objeto é obrigatório!');
		jQuery('[name=tocid]').focus();
		return false;
	}
	/*
	if(jQuery('[name=obcvalor]').val() == ''){
		alert('O campo Valor é obrigatório!');
		jQuery('[name=obcvalor]').focus();
		return false;
	}
	*/
	if(jQuery('[name=obcdtinicio]').val() == ''){
		alert('O campo Vigência Data de Início é obrigatório!');
		jQuery('[name=obcdtinicio]').focus();
		return false;
	}
	if(jQuery('[name=obcdtfim]').val() == ''){
		alert('O campo Vigência Data de Fim é obrigatório!');
		jQuery('[name=obcdtfim]').focus();
		return false;
	}
	
	document.formulario.submit();
}

function alterarDados(obcid)
{
	jQuery.ajax({
		url: 'academico.php?modulo=principal/informacoesContrato&acao=A',
		type: 'post',
		dataType: 'json',
		data: 'requisicao=recuperaDados&obcid='+obcid,
		success: function(e){		
			jQuery('[name=obcid]').val(e.obcid);
			jQuery('[name=tpcid]').val(e.tpcid);
			jQuery('[name=stcid]').val(e.stcid);
			jQuery('[name=obcvalor]').val(MascaraMonetario(e.obcvalor));			
			jQuery('[name=obcdtinicio]').val(formataData(e.obcdtinicio));
			jQuery('[name=obcdtfim]').val(formataData(e.obcdtfim));
			popupaObjeto(e.tpcid, e.tocid);
		}
	});	
}

function excluir(obcid)
{
	alert(obcid);
}

jQuery(function(){
	jQuery('.excluir_objeto').click(function(){	
		if(confirm('Deseja deletar o registro?')){			
			obcid = this.id;
			linha = jQuery(this).parent().parent().parent().parent();			
			jQuery.ajax({
				url: 'academico.php?modulo=principal/informacoesContrato&acao=A',
				type: 'post',
				data: 'requisicao=excluirObjeto&obcid='+obcid,
				success: function(e){					
					linha.remove();
				}		
			});
		}
	});
});

function formataData(data)
{
	arData = data.split('-');
	return arData[2]+'/'+arData[1]+'/'+arData[0]
}

</script>
<script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/dateFunctions.js"></script>
<script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>
<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>
<form name="formulario" method="post">
	<input type="hidden" name="requisicao" value="salvaDados" />
	<input type="hidden" name="obcid" value="" />
	<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
		<tr>
			<td class="SubTituloDireita">Tipo de contrato:</td>
			<td>
				<?php
				$sql = "select 
							tpcid as codigo, 
							tpcdsc as descricao 
						from 
							academico.tipocontrato 
						where 
							tpcstatus = 'A' 
						order by 
							tpcdsc";
				
				$db->monta_combo('tpcid', $sql, 'S', 'Selecione..', 'popupaObjeto', '', '', '', 'S');
				?>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Situação do contrato:</td>
			<td>
			<?php
			$sql = "select 
						stcid as codigo, 
						stcdsc as descricao 
					from 
						academico.situacaocontrato 
					order by 
						stcdsc";
			$db->monta_combo('stcid', $sql, 'S', 'Selecione..', '', '', '', '', 'S');
			?>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Objeto:</td>
			<td id="td_tocid">
			<?php
			$db->monta_combo('tocid', array(), 'S', 'Selecione..', '', '', '', '', 'S');
			?>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Valor:</td>
			<td>
			<?php		
			echo campo_texto('obcvalor', "S", "S", "Valor", 30, 14, "###.###.###,##", "", '', '', 0, 'id="obcvalor"' );
			?>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Vigência:</td>
			<td>
				Início: <?php echo campo_data2('obcdtinicio','S','S','',''); ?>
				&nbsp;
				Final: <?php echo campo_data2('obcdtfim','S','S','',''); ?>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita"></td>
			<td>
				<input type="button" value="Gravar" onclick="salvaDados()" />
			</td>
		</tr>
	</table>
</form>
<?php
 
$slfid = pegarMomento();

if(!$slfid) {
	$permissaoAlterar = false;
}

$permissaoAlterar = true;

$acao = '';
if($permissaoAlterar){
	$acao = "'<center><span style=\"white-space:nowrap;\" >
				<img src=../imagens/alterar.gif style=cursor:pointer; onclick=\"alterarDados('||obj.obcid||');\"> 
				<img src=../imagens/excluir.gif style=cursor:pointer; id=\"'||obj.obcid||'\" class=\"excluir_objeto\">
			  </span></center>' as acao,";
}

$sql = "select 
			{$acao}
			obcid,
			tpcdsc,
			stcdsc,
			tocdsc,
			obcvalor,
			to_char(obcdtinicio,'DD/MM/YYYY') || ' a ' || to_char(obcdtfim,'DD/MM/YYYY') as vigencia
		from 
			academico.objetocontrato obj
		join 
			academico.tipoobjetocontrato tob on obj.tocid = tob.tocid
		join 
			academico.situacaocontrato stc on stc.stcid = obj.stcid
		join 
			academico.tipocontrato tpc on tpc.tpcid = tob.tpcid
		where 
			obj.entid = ".$_SESSION['academico']['entid'];

if($permissaoAlterar){
	$cabecalho = array("Ação","Cód.","Tipo Contrato","Situação","Objeto","Valor","Vigência");
}else{
	$cabecalho = array("Cód.","Tipo Contrato","Situação","Objeto","Valor","Vigência");	
}

$db->monta_lista_simples($sql,$cabecalho,1000,5,'N','95%');

?>