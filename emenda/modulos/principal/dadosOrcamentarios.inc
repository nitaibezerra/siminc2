<?php

if( $_REQUEST['reg'] == 'excluir' ){
	if( $_REQUEST['emfid'] ){
		$db->executar("UPDATE emenda.empenhofederal SET emfstatus = 'I' WHERE emfid = {$_REQUEST['emfid']}");
		$db->commit();
		$db->sucesso('principal/dadosOrcamentarios', '&aba=empenho');
	} else {
		$db->executar("UPDATE emenda.pagamentofederal SET pafstatus = 'I' WHERE pafid = {$_REQUEST['pafid']}");
		$db->commit();
		$db->sucesso('principal/dadosOrcamentarios', '&aba=pagamento');
	}
	exit();
}
if( $_POST['requisicao'] == 'salvarempenho' ){
	
	if( $_POST['emfid'] ){
		$sql = "UPDATE emenda.empenhofederal SET
					emfdescricao = '{$_POST['emfdescricao']}',
				  	emfnumero = '{$_POST['emfnumero']}',
				  	emfdata = '{$_POST['emfdata']}'
				WHERE emfid = {$_POST['emfid']}";
	} else {
		$sql = "INSERT INTO emenda.empenhofederal(emfdescricao, emfnumero, emfdata, ptrid) 
				VALUES ('{$_POST['emfdescricao']}', '{$_POST['emfnumero']}', '{$_POST['emfdata']}', {$_SESSION["emenda"]["ptrid"]})";
	}	
	$db->executar($sql);
	$db->commit();
	$db->sucesso('principal/dadosOrcamentarios', '&aba=empenho');
	exit();
}

if( $_POST['requisicao'] == 'salvarpagamento' ){
	
	if( $_POST['pafid'] ){
		$sql = "UPDATE emenda.pagamentofederal SET
					pafdescricao = '{$_POST['pafdescricao']}',
				  	pafnumero = '{$_POST['pafnumero']}',
				  	pafdata = '{$_POST['pafdata']}',
				  	emfid = {$_POST['emfid']}				  	
				WHERE pafid = {$_POST['pafid']}";
	} else {
		$sql = "INSERT INTO emenda.pagamentofederal(pafdescricao, pafnumero, pafdata, ptrid, emfid) 
				VALUES ('{$_POST['pafdescricao']}', '{$_POST['pafnumero']}', '{$_POST['pafdata']}', {$_SESSION["emenda"]["ptrid"]}, {$_POST['emfid']})";
	}	
	$db->executar($sql);
	$db->commit();
	$db->sucesso('principal/dadosOrcamentarios', '&aba=pagamento');
	exit();
}

$obPTA = new PTA();

$obPTA->validaSessionPTA( $_SESSION["emenda"]["ptrid"] );

$ptrid = $_SESSION["emenda"]["ptrid"];

include  APPRAIZ."includes/cabecalho.inc";

montaAbasPTA( $abacod_tela, $url, '', '', '' );
monta_titulo( 'Dados Orçamentários', '');
echo $obPTA->cabecalhoPlanoTrabalho($ptrid);

/*$sql = "SELECT ptrpropostasiconvano, ptridentificacaofederal, ptrobjetivofederal, ptrjustificativafederal FROM emenda.planotrabalho WHERE ptrid = $ptrid";
$arDados = $db->pegaLinha($sql);
extract($arDados);*/


if( empty($_REQUEST['aba']) || $_REQUEST['aba'] == 'empenho' ){
	$abaAtiva = 'emenda.php?modulo=principal/dadosOrcamentarios&acao=A&aba=empenho';
} else {
	$abaAtiva = 'emenda.php?modulo=principal/dadosOrcamentarios&acao=A&aba=pagamento';
}

/* Array com os itens da aba de identificação */
$menu = array (
		0 => array (
				"id" => 0,
				"descricao" => "Empenho",
				"link" => "emenda.php?modulo=principal/dadosOrcamentarios&acao=A&aba=empenho" 
		),
		1 => array (
				"id" => 1,
				"descricao" => "Pagamento",
				"link" => "emenda.php?modulo=principal/dadosOrcamentarios&acao=A&aba=pagamento" 
		) 
);

echo montarAbasArray ( $menu, $abaAtiva );
?>
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>
<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>

<form method="post" name="formulario" id="formulario" action="">
	<input type="hidden" name="requisicao" id="requisicao" value=""> 
	<table id="tblform" class="tabela" width="95%" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
		<?if( empty($_REQUEST['aba']) || $_REQUEST['aba'] == 'empenho' ){
			if( $_REQUEST['emfid'] ){
				$sql = "SELECT 
						  emfid,
						  emfdescricao,
						  emfnumero,
						  emfdata,
						  emfstatus,
						  ptrid
						FROM 
						  emenda.empenhofederal WHERE emfid = ".$_REQUEST['emfid'];
				$arrEmpenho = $db->pegaLinha($sql);
				$arrEmpenho = $arrEmpenho ? $arrEmpenho : array();
				extract($arrEmpenho);
				$emfdata = formata_data($emfdata);
			}
				?>
			<tr>
				<td class="SubTituloDireita" width="20%">Descrição do Empenho:</td>
				<td>
				<input type="hidden" name="emfid" id="emfid" value="<?=$emfid; ?>">
				<?=campo_texto( 'emfdescricao', 'S', 'S', 'Descrição do Empenho', 50, 100, '', '','','','','id="emfdescricao"', '', '', ""); ?>
			</tr>
			<tr>
				<td class="SubTituloDireita" width="20%">Número do Empenho:</td>
				<td><?=campo_texto( 'emfnumero', 'S', 'S', 'Número do Empenho', 15, 20, '', '','','','','id="emfnumero"', '', '', ""); ?>
			</tr>
			<tr>
				<td class="SubTituloDireita" width="20%">Data do Empenho:</td>
				<td><?=campo_data2('emfdata', 'S','S','Data do Empenho','','',''); ?>
			</tr>
			<tr bgcolor="#D0D0D0">
				<td colspan="4" align="center">
					<input type="button" value="Salvar" id="btnSalvar" name="btnSalvar" onclick="salvarDadosEmpenho();" style="cursor: pointer;"/>
				</td>
			</tr>
		<?} else { 
			if( $_REQUEST['pafid'] ){
				$sql = "SELECT 
						  pafid,
						  pafdescricao,
						  pafnumero,
						  pafdata,
						  pafstatus,
						  ptrid
						FROM 
						  emenda.pagamentofederal WHERE pafid = ".$_REQUEST['pafid'];
				$arrEmpenho = $db->pegaLinha($sql);
				$arrEmpenho = $arrEmpenho ? $arrEmpenho : array();
				extract($arrEmpenho);
				$pafdata = formata_data($pafdata);
			}
			?>
			<tr>
				<td class="SubTituloDireita" width="20%">Número do Empenho:</td>
				<td><?
				$sql = "SELECT 
						  emfid as codigo,
						  emfnumero as descricao						  
						FROM 
						  emenda.empenhofederal WHERE ptrid = $ptrid and emfstatus = 'A'";
				$db->monta_combo("emfid", $sql, 'S', 'Selecione...', '',"","","345","S","","");
				?>
			</tr>
			<tr>
				<td class="SubTituloDireita" width="20%">Descrição do Pagamento:</td>
				<td><?=campo_texto( 'pafdescricao', 'S', 'S', 'Descrição do Pagamento', 50, 100, '', '','','','','id="pafdescricao"', '', '', ""); ?>
			</tr>
			<tr>
				<td class="SubTituloDireita" width="20%">Número do Pagamento:</td>
				<td><?=campo_texto( 'pafnumero', 'S', 'S', 'Número do Pagamento', 15, 20, '', '','','','','id="pafnumero"', '', '', ""); ?>
			</tr>
			<tr>
				<td class="SubTituloDireita" width="20%">Data do Pagamento:</td>
				<td><?=campo_data2('pafdata', 'S','S','Data do Pagamento','','',''); ?>
			</tr>
			<tr bgcolor="#D0D0D0">
				<td colspan="4" align="center">
					<input type="button" value="Salvar" id="btnSalvar" name="btnSalvar" onclick="salvarDadosPagamento();" style="cursor: pointer;"/>
				</td>
			</tr>
		<?} ?>
	</table>
</form>
<?
if( empty($_REQUEST['aba']) || $_REQUEST['aba'] == 'empenho' ){
	monta_titulo('', 'Lista de Empenhos');
	$sql = "SELECT  
				'<center><img src=\"/imagens/alterar.gif\" border=0 title=\"Alterar\" style=\"cursor:pointer;\" onclick=\"alterarEmpenho(' || emfid || ');\">&nbsp;
				<img src=\"/imagens/excluir.gif\" border=0 title=\"Excluir\" style=\"cursor:pointer;\" onclick=\"excluirEmpenho(' || emfid || ');\"></center>' as acao,			
				emfdescricao, emfnumero, to_char(emfdata, 'DD/MM/YYYY') as data 
			FROM emenda.empenhofederal WHERE ptrid = $ptrid and emfstatus = 'A'";
	$cabecalho = array('Ação', 'Descrição do Empenho', 'Número do Empenho', 'Data do Empenho');	
} else {
	monta_titulo('', 'Lista de Pagamentos');
	$sql = "SELECT  
				'<center><img src=\"/imagens/alterar.gif\" border=0 title=\"Alterar\" style=\"cursor:pointer;\" onclick=\"alterarPagamento(' || pafid || ');\">&nbsp;
				<img src=\"/imagens/excluir.gif\" border=0 title=\"Excluir\" style=\"cursor:pointer;\" onclick=\"excluirPagamento(' || pafid || ');\"></center>' as acao,			
				ef.emfnumero, pf.pafdescricao, pf.pafnumero, to_char(pf.pafdata, 'DD/MM/YYYY') as data 
			FROM emenda.pagamentofederal pf
				inner join emenda.empenhofederal ef on ef.emfid = pf.emfid 
			WHERE pf.ptrid = $ptrid and pf.pafstatus = 'A'";
	$cabecalho = array('Ação', 'Empenho', 'Descrição do Pagamento', 'Número do Pagamento', 'Data do Pagamento');
}

$db->monta_lista_simples( $sql, $cabecalho, 100000, 1, 'N', '100%');
?>
<script>
	function salvarDadosEmpenho(){
		if( $('[name="emfdescricao"]').val() == '' ){
			alert('O campo "Descrição do Empenho" é de preenchimento obrigatório.');
			$('[name="emfdescricao"]').focus();
			return false;
		}
		if( $('[name="emfnumero"]').val() == '' ){
			alert('O campo "Número do Empenho" é de preenchimento obrigatório.');
			$('[name="emfnumero"]').focus();
			return false;
		}
		if( $('[name="emfdata"]').val() == '' ){
			alert('O campo "Data do Empenho" é de preenchimento obrigatório.');
			$('[name="emfdata"]').focus();
			return false;
		}
		$('[name="requisicao"]').val('salvarempenho');
		$('[name="formulario"]').submit();
	}
	function salvarDadosPagamento(){
		if( $('[name="emfid"]').val() == '' ){
			alert('O campo "Número do Empenho" é de preenchimento obrigatório.');
			$('[name="emfid"]').focus();
			return false;
		}
		if( $('[name="pafdescricao"]').val() == '' ){
			alert('O campo "Descrição do Pagamento" é de preenchimento obrigatório.');
			$('[name="pafdescricao"]').focus();
			return false;
		}
		if( $('[name="pafnumero"]').val() == '' ){
			alert('O campo "Número do Pagamento" é de preenchimento obrigatório.');
			$('[name="pafnumero"]').focus();
			return false;
		}
		if( $('[name="pafdata"]').val() == '' ){
			alert('O campo "Data do Pagamento" é de preenchimento obrigatório.');
			$('[name="pafdata"]').focus();
			return false;
		}
		$('[name="requisicao"]').val('salvarpagamento');
		$('[name="formulario"]').submit();
	}
	
	function alterarEmpenho( emfid ){
		window.location.href = 'emenda.php?modulo=principal/dadosOrcamentarios&acao=A&aba=empenho&emfid='+emfid;
	}
	function excluirEmpenho( emfid ){
		window.location.href = 'emenda.php?modulo=principal/dadosOrcamentarios&acao=A&aba=empenho&emfid='+emfid+'&reg=excluir';
	}
	
	function alterarPagamento( pafid ){
		window.location.href = 'emenda.php?modulo=principal/dadosOrcamentarios&acao=A&aba=pagamento&pafid='+pafid;
	}
	function excluirPagamento( pafid ){
		window.location.href = 'emenda.php?modulo=principal/dadosOrcamentarios&acao=A&aba=pagamento&pafid='+pafid+'&reg=excluir';
	}
</script>