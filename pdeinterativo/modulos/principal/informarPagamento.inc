<?php
if( !$_GET['usucpf'] &&  !$_GET['pdeid'])
{
	echo '<script>
			alert("Escola não encontrada!");
			window.location.href = "pdeinterativo.php?modulo=principal/principal&acao=A";
		  </script>';
	exit;
}

if($_POST['requisicao']){
	$arrDados = $_POST['requisicao']();
}

if($_POST['requisicaoAjax']){
	$_POST['requisicaoAjax']();
	exit;
}
if($_GET['pdeid']) {
	echo '<script language="JavaScript" src="../includes/funcoes.js"></script>';
	echo '<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>';
	echo "<link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>";
} else {
	include APPRAIZ . 'includes/cabecalho.inc';
	print '<br/>';
}

$db->cria_aba($abacod_tela,$url,'');
$titulo = str_replace("...","",$_SESSION['mnudsc']);
monta_titulo( $titulo, '&nbsp' );

if($_GET['usucpf']) {
	$arrDados = recuperaDadosEscolaPorCPFDiretor($_GET['usucpf']);
	$_SESSION['pdeinterativo_vars']['usucpfdiretor'] = $arrDados['usucpf'];
	$_SESSION['pdeinterativo_vars']['pdicodinep'] = $arrDados['pdicodinep'];
	$_SESSION['pdeinterativo_vars']['pdeid'] = $arrDados['pdeid'];
	$_SESSION['pdeinterativo_vars']['pdenome'] = $arrDados['pdenome'];
	$_SESSION['pdeinterativo_vars']['btn_cancelar'] = 'pdeinterativo.php?modulo=principal/principal&acao=A';
	
	$arrDados = recuperaDadosEscolaPorCPFDiretor($_SESSION['pdeinterativo_vars']['usucpfdiretor']);
} elseif($_GET['pdeid']) {
	
	$arrDados = $db->pegaLinha("SELECT pdi.*,
									   mun.mundescricao,
									   mun.muncod,
									   CASE WHEN (select distinct id_ens_fundamental_ciclos from educacenso_2010.tab_dado_escola where fk_cod_entidade::bigint = pdicodinep::bigint) = 1
									        THEN 'Sim'
											ELSE 'Não'
											END as ciclo
								FROM pdeinterativo.pdinterativo pdi 
								INNER JOIN territorios.municipio mun ON pdi.muncod = mun.muncod 
								WHERE pdeid='".$_GET['pdeid']."' AND pdistatus = 'A'");
	
	$_SESSION['pdeinterativo_vars']['pdicodinep'] = $arrDados['pdicodinep'];
	$_SESSION['pdeinterativo_vars']['pdeid'] = $arrDados['pdeid'];
	$_SESSION['pdeinterativo_vars']['pdenome'] = $arrDados['pdenome'];
}

cabecalhoPDEInterativo($arrDados);

$sql = "select * from pdeinterativo.situacaopagamento where pdeid = {$_SESSION['pdeinterativo_vars']['pdeid']} and spastatus = 'A'";
$arrDadosPag = $db->pegaLinha($sql);
$arrDadosPag = !$arrDadosPag ? array() : $arrDadosPag;
extract($arrDadosPag);
?>
<script language="javascript" type="text/javascript" src="../includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-1.4.2.min.js"></script>
<script type="text/javascript" src="../pdeinterativo/js/pdeinterativo.js"></script>
<script>	
	
	jQuery(function() {
		<?php if($_SESSION['pdeinterativo']['msg']): ?>
			alert('<?php echo $_SESSION['pdeinterativo']['msg'] ?>');
			<?php unset($_SESSION['pdeinterativo']['msg']) ?>
		<?php endif; ?>
	});
	
	function carregaOpcoes(opcao)
	{
		if(!opcao){
			jQuery('#tr_spamotivo').hide();
			jQuery('#tr_spadatapagamento').hide();
		}else{
			if(opcao == "t"){
				jQuery('#tr_spamotivo').hide();
				jQuery('#tr_spadatapagamento').show();
			}else{
				jQuery('#tr_spadatapagamento').hide();
				jQuery('#tr_spamotivo').show();
			}
		}
	}
	
	function salvarInfoPag()
	{
		if(!jQuery('[name=spasituacao]').val()){
			alert('Selecione a situação do pagamento!')
			return false;
		}
		if(jQuery('[name=spasituacao]').val() == "t" && !jQuery('[name=spadatapagamento]').val()){
			alert('Informe a data do pagamento!')
			return false;
		}
		if(jQuery('[name=spasituacao]').val() == "f" && !jQuery('[name=mopid]').val()){
			alert('Selecione o motivo da pendência!')
			return false;
		}
		jQuery('#form_info_pag').submit();
	}
	
</script>
<!-- (INÍCIO) BIBLIOTECAS - PARA USO DOS COMPONENTES (CALENDÁRIO E SLIDER) -->
<script	language="javascript" type="text/javascript" src="../includes/blendtrans.js"></script>
<script language="javascript" type="text/javascript" src="../includes/JsLibrary/_start.js"></script>
<script language="javascript" type="text/javascript" src="../includes/JsLibrary/slider/slider.js"></script>
<script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/dateFunctions.js"></script>
<script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>

<link rel="stylesheet" type="text/css" href="../pdeinterativo/css/pdeinterativo.css"/>
<link rel="stylesheet" type="text/css" href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css"/>

<form name="form_info_pag" id="form_info_pag"  method="post" action="" >
	<input type="hidden" name="requisicao" value="salvarInformePagamento" />
	<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
		<tr>
			<td width="25%" class="SubTituloDireita" >Situação do Pagamento:</td>
			<td>
				<?php $arrDados = array( 0 => array( "codigo" => "t", "descricao" => "Pago" ), 1 => array( "codigo" => "f", "descricao" => "Pendente" ) ) ?>
				<?php echo $db->monta_combo("spasituacao",$arrDados,"S","Selecione","carregaOpcoes","","",200) ?>
			</td>
		</tr>
		<tr id="tr_spadatapagamento" style="display:<?php echo $spasituacao == "t" ? "" : "none" ?>" >
			<td width="25%" class="SubTituloDireita" >Data de Pagamento:</td>
			<td>
				<?php echo campo_data2('spadatapagamento','S', "S", 'Data de Pagamento', 'S' ); ?>
			</td>
		</tr>
		<tr id="tr_spamotivo" style="display:<?php echo $spasituacao == "f" ? "" : "none" ?>" >
			<td width="25%" class="SubTituloDireita" >Motivo:</td>
			<td>
				<?php $sql = "	select
									mopid as codigo,
									mopdesc as descricao
								from
									pdeinterativo.motivopagamento
								where
									mopstatus = 'A'
								order by
									descricao" ?>
				<?php echo $db->monta_combo("mopid",$sql,"S","Selecione","","","",200) ?>
			</td>
		</tr>
		<tr class="SubtituloTabela" >
			<td></td>
			<td>
				<input type="button" name="btn_salvar" value="Salvar" onclick="salvarInfoPag()" />
				<input type="button" name="btn_cancelar" value="Cancelar" onclick="window.location='<?=$_SESSION['pdeinterativo_vars']['btn_cancelar'] ?>'" />
			</td>
		</tr>
	</table>
</form>