<?php
$ano = $_REQUEST ['ano'];
if ($ano != date('Y') ) {
	$visivelEmenda = 'disabled="disabled"';
}

ini_set ( "memory_limit", "1000M" );
set_time_limit ( 0 );

global $db;
// Busca perfil
$perfil = pegaPerfilGeral ();
// CAso seja um dos perfis abaixo poderar aceitar a emenda
if (in_array ( PAR_PERFIL_EQUIPE_ESTADUAL, $perfil ) || in_array ( PAR_PERFIL_EQUIPE_MUNICIPAL, $perfil ) || in_array ( PAR_PERFIL_PREFEITO, $perfil ) || in_array ( PAR_PERFIL_EQUIPE_ESTADUAL_APROVACAO, $perfil ) || in_array ( PAR_PERFIL_EQUIPE_MUNICIPAL_APROVACAO, $perfil ) || in_array ( PAR_PERFIL_SUPER_USUARIO, $perfil ) || in_array ( PAR_PERFIL_ADMINISTRADOR, $perfil )) {
	$boolAceitar = true;
} else {
	$boolAceitar = false;
}

if( date('Y-m-d') >= date('Y').'04-01'){
	$boolAceitar = false;
}

if ($_REQUEST ['carregaEmendaDetalhePTA']) {
	header ( 'content-type: text/html; charset=ISO-8859-1' );
	
	$sql = "SELECT DISTINCT vede.emecod, aut.autnome, fup.fupdsc, vede.gndcod||' - '||gn.gnddsc as gndcod1, vede.mapcod||' - '||map.mapdsc as modalidade, 
				vede.foncod||' - '||fon.fondsc as fonte, ped.pedvalor
			FROM emenda.planotrabalho ptr
			    INNER JOIN emenda.ptemendadetalheentidade  ped ON ptr.ptrid = ped.ptrid
			    INNER JOIN emenda.v_emendadetalheentidade vede ON vede.edeid = ped.edeid
			    left JOIN emenda.v_funcionalprogramatica fup ON (vede.acaid = fup.acaid)
			    inner JOIN emenda.autor aut ON aut.autid = vede.autid
                left join public.gnd gn on gn.gndcod = vede.gndcod and gn.gndstatus = 'A'
			    left join public.modalidadeaplicacao map on map.mapcod = vede.mapcod
			    left join public.fonterecurso fon on fon.foncod = vede.foncod and fon.fonstatus = 'A'
			WHERE ptr.ptrid = {$_REQUEST['ptrid']}
			    AND vede.edestatus = 'A' 
			    and vede.emetipo = 'E'";
	
	echo '<table class="tabela" cellspacing="0" cellpadding="3" border="0" bgcolor="#dcdcdc" align="center" style="border-top: medium none; border-bottom: medium none;">
				<tr>
					<td bgcolor="#e9e9e9" align="center" style=""><b>Recurso(s) Vinculado(s) ao Plano de Trabalho</b></td>
				</tr>
			</table>';
	$cabecalho = array (
			"Recurso",
			"Autor",
			"Funcional Programática",
			"GND",
			"Mod",
			"Fonte",
			"Valor" 
	);
	echo $db->monta_lista_simples ( $sql, $cabecalho, 20, 5 );
	exit ();
}

$inuid = $_SESSION ['par'] ['inuid'];

if (! $inuid || $inuid == '') {
	echo "<script>alert('Entidade não encontrada!');window.close();</script>";
	exit ();
}

if ($_REQUEST ['requisicao'] == 'salvar') {
	// ver($_POST,d);
	
	$ptridpai = 'null';
	$ptrcod = ($db->pegaUm ( "SELECT max(ptrcod) + 1 FROM emenda.planotrabalho" ));
	
	$resid = $_POST ['resid'] [0];
	$enbid = $_POST ['enbid'] [0];
	
	if ($resid == 3) {
		$mdeid = 3;
	} else {
		$mdeid = $db->pegaUm ( "SELECT mdeid FROM emenda.modalidadeensino where resid = $resid" );
	}
	
	$sql = "INSERT INTO emenda.planotrabalho( enbid, ptrexercicio, ptrstatus, resid, mdeid, ptridpai, ptrcod, ptrsituacao, sisid )
			VALUES ( {$enbid}, " . date ( 'Y' ) . ", 'A', {$resid}, {$mdeid}, " . $ptridpai . ", " . $ptrcod . ", 'E', 23) RETURNING ptrid";
	$ptrid = $db->pegaUm ( $sql );
	
	if ($_POST ['emdid'] && $_POST ['emdid'] [0] != "") {
		
		foreach ( $_POST ['emdid'] as $emdid ) {
			$edeid = $_POST ['edeid'] [$emdid];
			
			$sql = "INSERT INTO emenda.ptemendadetalheentidade(ptrid, edeid, pedvalor) 
					VALUES ($ptrid, $edeid, (select edevalor from emenda.emendadetalheentidade where edeid = " . $edeid . "))";
			$db->executar ( $sql );
		}
	}
	ob_clean ();
	if ($db->commit ()) {
		include_once APPRAIZ . 'includes/workflow.php';
		// cria o docid (workflow) do PTA
		$tpdid = 8;
		$docdsc = "Cadastro de PTA (emendas) - n°" . $ptrid;
		$docid = wf_cadastrarDocumento ( $tpdid, $docdsc );
		
		$sql = "UPDATE emenda.planotrabalho SET docid = " . $docid . " WHERE ptrid = " . $ptrid;
		$db->executar ( $sql );
		$db->commit ();
		
		$pta = $ptrcod . '/' . $_SESSION ['exercicio'];
		
		$db->sucesso ( 'principal/propostaEmenda', '&aba=emenda&ano='.$ano );
	} else {
		echo "<script>
				alert('Falha na Operação');
				window.location.href = window.location;
			</script>";
	}
	exit ();
}
function pegaCnpj($inuid) {
	global $db;
	
	return $db->pegaUm ( "SELECT iue.iuecnpj 
	                     FROM par.instrumentounidade iu
	                     inner join par.instrumentounidadeentidade iue on iue.inuid = iu.inuid
	                     WHERE
	                     	iu.inuid = {$inuid}
	                     	and iue.iuestatus = 'A'
	                    order by (iue.iuedefault = true) desc" );
}

?>
<html>
<head>
<meta http-equiv="Cache-Control" content="no-cache">
<meta http-equiv="Pragma" content="no-cache">
<meta http-equiv="Connection" content="Keep-Alive">
<meta http-equiv="Expires" content="Fri, 9 Oct 1998 05:00:00 GMT">
<title>PAR 2010 - Plano de Metas - Subação</title>

<link rel="stylesheet" type="text/css" href="../includes/Estilo.css" />
<link rel="stylesheet" type="text/css" href="../includes/listagem.css" />

<script type="text/javascript" src="../includes/funcoes.js"></script>
<script language="javascript" type="text/javascript" src="../includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-1.4.2.min.js"></script>
</head>
<body>
<?php
$abaAtiva = 'par.php?modulo=principal/propostaEmenda&acao=A&aba=emenda&ano=' . $_REQUEST ['ano'];

/* Array com os itens da aba de identificação */
$menu = array (
		0 => array (
				"id" => 0,
				"descricao" => "2013",
				"link" => "par.php?modulo=principal/propostaEmenda&acao=A&aba=emenda&ano=2013" 
		),
		1 => array (
				"id" => 1,
				"descricao" => "2014",
				"link" => "par.php?modulo=principal/propostaEmenda&acao=A&aba=emenda&ano=2014" 
		), 
		2 => array (
				"id" => 2,
				"descricao" => "2015",
				"link" => "par.php?modulo=principal/propostaEmenda&acao=A&aba=emenda&ano=2015" 
		) 
);

echo montarAbasArray ( $menu, $abaAtiva );

$sql = "SELECT
			vede.emecod,
			enb.enbnome,
			enb.enbid,
			(CASE WHEN vede.emetipo = 'E' THEN 'Emenda' ELSE 'Complemento' END) as tipoemenda,
			a.autnome,
			vfun.fupfuncionalprogramatica,
			vfun.fupdsc,
			sum(vede.edevalor) as valor,
			vede.emeid,
			vede.emedescentraliza,
			(select ptrid from emenda.ptemendadetalheentidade where edeid = vede.edeid limit 1) as pta
		FROM
			emenda.v_emendadetalheentidade vede
		inner join emenda.autor a on a.autid = vede.autid
		left join emenda.v_funcionalprogramatica vfun on vfun.acaid = vede.acaid and vfun.prgano = '$ano' and vfun.acastatus = 'A'
		inner join emenda.entidadebeneficiada enb on enb.enbid = vede.entid
		WHERE
			vede.edestatus = 'A'
			and vede.ededisponivelpta = 'S'
			and vede.emeano = '$ano'
			and enb.enbcnpj = '" . pegaCnpj ( $inuid ) . "'
			and vede.emetipo = 'E'
			and vede.edeid in ( select edeid from emenda.emendapariniciativa )
			and vede.edeid not in (select pte.edeid from emenda.planotrabalho ptr
									inner join emenda.ptemendadetalheentidade pte on pte.ptrid = ptr.ptrid
								where ptrstatus = 'A' and ptrexercicio = '" . (( int ) $ano) . "') 
		GROUP BY
          vede.emecod,
          enb.enbnome,
          enb.enbid,
          tipoemenda,
          a.autnome,
          vfun.fupfuncionalprogramatica,
          vfun.fupdsc,
          vede.emeid,
          vede.emedescentraliza,
          pta";

$arEmenda = $db->carregar ( $sql );
$arEmenda = $arEmenda ? $arEmenda : array ();

$entidadePar = ($_SESSION ['par'] ['itrid'] == 1) ? $_SESSION ['par'] ['estuf'] : $_SESSION ['par'] ['muncod'];
$nmEntidadePar = EntidadeParControle::recuperaDescricaoEntidadePar ( $entidadePar, $_SESSION ['par'] ['itrid'] );
monta_titulo ( $nmEntidadePar, "Emendas Propostas" );
echo '<br>';

if ($_REQUEST ['aba'] == 'emenda') {
		$abaAtivaGeral = 'par.php?modulo=principal/propostaEmenda&acao=A&aba=emenda&ano=' . $_REQUEST ['ano'];
	} elseif ($_REQUEST ['aba'] == 'subacao') {
		$abaAtivaGeral = 'par.php?modulo=principal/propostaEmendaSubacao&acao=A&aba=subacao&ano=' . $_REQUEST ['ano'];
	} elseif ($_REQUEST ['aba'] == 'orcamento') {
		$abaAtivaGeral = 'par.php?modulo=principal/orcamentoImpositivoPar&acao=A&aba=orcamento&ano=' . $_REQUEST ['ano'];
	}

$boImpositivo = verificaEmendaImpositiva($inuid, $_REQUEST ['ano']);
/* Array com os itens da aba de identificação */
if( $boImpositivo < 0 ){
	$menuGeral = array (
			0 => array (
					"id" => 0,
					"descricao" => "Proposta Emenda",
					"link" => "par.php?modulo=principal/propostaEmenda&acao=A&aba=emenda&ano=$ano"
			),
			1 => array (
					"id" => 1,
					"descricao" => "Proposta Subação",
					"link" => "par.php?modulo=principal/propostaEmendaSubacao&acao=A&aba=subacao&ano=$ano"
			) 		
	);	
} else {
	$menuGeral = array (
			0 => array (
					"id" => 0,
					"descricao" => "Proposta Emenda",
					"link" => "par.php?modulo=principal/propostaEmenda&acao=A&aba=emenda&ano=$ano"
			),
			1 => array (
					"id" => 1,
					"descricao" => "Proposta Subação",
					"link" => "par.php?modulo=principal/propostaEmendaSubacao&acao=A&aba=subacao&ano=$ano"
			),
			2 => array (
					"id" => 2,
					"descricao" => "Orçamento Impositivo",
					"link" => "par.php?modulo=principal/orcamentoImpositivoPar&acao=A&aba=orcamento&ano=$ano" 
			) 		
	);
}

echo montarAbasArray ( $menuGeral, $abaAtivaGeral );

//$db->cria_aba ( $abacod_tela, $url, '' );

$btnSalvar = '';
?>


	<form method="post" name="formulario" id="formulario" action="">
		<input type="hidden" name="requisicao" id="requisicao" value=""> 
		<input type="hidden" name="ptrid" id="ptrid" value="<?=$arEmenda[0]['pta']; ?>">
		<table id="tblform" class="tabela" width="95%" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
			<tr>
				<td class="SubTituloDireita" style="text-align: center;" colspan="2"><b>Recursos Disponíveis</b></td>
			</tr>
			<tr>
				<td>	
	<?if($arEmenda){ ?>
					<table id="tblform" class="listagem" width="100%" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
						<thead>
							<tr>
								<td align="Center" class="title" width="10%"
									style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;"
									onmouseover="this.bgColor='#c0c0c0';"
									onmouseout="this.bgColor='';"><strong>Código</strong></td>
								<td align="Center" class="title" width="10%"
									style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;"
									onmouseover="this.bgColor='#c0c0c0';"
									onmouseout="this.bgColor='';"><strong>Tipo</strong></td>
								<td align="Center" class="title" width="20%"
									style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;"
									onmouseover="this.bgColor='#c0c0c0';"
									onmouseout="this.bgColor='';"><strong>Autor</strong></td>
								<td align="Center" class="title" width="15%"
									style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;"
									onmouseover="this.bgColor='#c0c0c0';"
									onmouseout="this.bgColor='';"><strong>Funcional Programática</strong></td>
								<td align="Center" class="title" width="45%"
									style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;"
									onmouseover="this.bgColor='#c0c0c0';"
									onmouseout="this.bgColor='';"><strong>SubTítulo</strong></td>
								<td align="Center" class="title" width="10%"
									style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;"
									onmouseover="this.bgColor='#c0c0c0';"
									onmouseout="this.bgColor='';"><strong>Valor</strong></td>
							</tr>
						</thead>
		<?
		$html = '';
		foreach ( $arEmenda as $key => $valor ) {
			$key % 2 ? $cor = "#dedfde" : $cor = "";
			$emedescentraliza = $valor ['emedescentraliza'];
			$html .= '
				<tr bgcolor="' . $cor . '" id="tr_' . $key . '" onmouseout="this.bgColor=\'' . $cor . '\';" onmouseover="this.bgColor=\'#ffffcc\';">
					<td><img src="/imagens/mais.gif" style="cursor:pointer" border="0" id="img_dimensao_' . $valor ['emeid'] . '" onClick="carregaDetalheEmenda(this.id, ' . $valor ['emeid'] . ', \'listaEmendaDetalhe_' . $valor ['emeid'] . '\', \'trV_' . $valor ['emeid'] . '\');"> ' . $valor ['emecod'] . '</td>
				  	<td>' . $valor ['tipoemenda'] . '</td>
				  	<td>' . $valor ['autnome'] . '</td>
				  	<td style="text-align: center;">' . $valor ['fupfuncionalprogramatica'] . '</td>
				  	<td>' . $valor ['fupdsc'] . '</td>
					<td style="text-align: right;">R$ ' . number_format ( $valor ['valor'], 2, ',', '.' ) . '</td>
				</tr>
				<tr id="trV_' . $valor ['emeid'] . '" style="display: none">
			  	   <td colspan="6"><div id="listaEmendaDetalhe_' . $valor ['emeid'] . '" style="display: none">' . carregaDetalheEmendaAjax ( $valor ['emeid'], $valor ['enbid'], $visivelEmenda, $ano ) . '</div></td>
			  	</tr>';
		}
		$html .= '
			    <tr>
			    	<td colspan="6">&nbsp;</td>
			    </tr>
				<tr bgcolor="#dedfde">
					<td colspan="6">
					  	<table id="tblform" class="tabela" width="95%" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
							<tr>
								<td class="SubTituloDireita" style="width:20%;"><b>Valor Total do Concedente:</b></td>
								<td><div id="emdvalorTotalDiv">R$ ' . ($total ? number_format ( $total, 2, ',', '.' ) : '0,00') . '</div><input type="hidden" name="emdvalorTotal" id="emdvalorTotal" value="' . ($total ? $total : '0,00') . '"></td>
							</tr>
						</table>
					</td>
				</tr>';
		
		if ($boolAceitar) {
			if ($ano == date('Y')) {
				$btnSalvar = '<input type="button" name="btnSalvar" id="btnSalvar" value="Aceitar" onclick="salvarPropostaEmenda();">';
			}
		}
	} else {
		$html .= '<table width="95%" align="center" border="0" cellspacing="0" cellpadding="2" class="listagem">';
		$html .= '<tr><td align="center" style="color:#cc0000;">Não foram encontrados registros.</td></tr>';
		$html .= '</table>';
	}
	$html .= '	<tr bgcolor="#dedfde">
					<td colspan="6" align="center" valign="bottom" style="text-align: center;">
						'.$btnSalvar.'
						<input type="button" name="btnFechar" id="btnFechar" value="Fechar" onclick="fecharPopUp();">
					</td>
				</tr>
			</table>';
	echo $html;

	?>
			</td>
		</tr>
	</table>
</form>
<? carregarPTA ( $inuid, $ano ); ?>
</body>
<script>
	function fecharPopUp(){
		window.opener.location.href = window.opener.location; 
		window.close();
	}
	function salvarPropostaEmenda(){
		if( $('[type="checkbox"]:checked').length ){
			$('#requisicao').val('salvar');
			$('#formulario').submit();
		} else {
			alert('Selecione pelo menos uma emenda!');
			return false;
		}
	}
	function carregaDetalheEmenda(idImg, emeid, div_nome, tr_nome){	
		var img = document.getElementById( idImg );

		if(document.getElementById(div_nome).style.display == 'none'){
			document.getElementById(div_nome).style.display = '';
			document.getElementById(tr_nome).style.display = '';
			img.src = '../imagens/menos.gif';
		} else {
			document.getElementById(div_nome).style.display = 'none';
			document.getElementById(tr_nome).style.display = 'none';
			img.src = '/imagens/mais.gif';
		}
	}
	
	function somaEmendaDetalhe(check, valor, emeid){
		var soma = 0;
		if(check.checked){
			var total = document.getElementById('emdvalorTotal').value;
			soma = parseFloat(valor) + parseFloat(total);
			document.getElementById('emdvalorTotal').value = soma;
			document.getElementById('emdvalorTotalDiv').innerHTML = 'R$ ' + mascaraglobal('###.###.###.###,##', soma.toFixed(2));
		} else {
			var total = document.getElementById('emdvalorTotal').value;
			soma = parseFloat(total) - parseFloat(valor);
			document.getElementById('emdvalorTotal').value = soma;
			document.getElementById('emdvalorTotalDiv').innerHTML = 'R$ ' + mascaraglobal('###.###.###.###,##', soma.toFixed(2));
		}
	}
	
	function carregaEmendaDetalhePTA(idImg, ptrid){
		var img 	 = $('#'+idImg );
		var tr_nome = 'listaEmendaDetalhePTA_'+ ptrid;
		var td_nome  = 'trV_'+ ptrid;
		
		if($('#'+tr_nome).css('display') == 'none' && $('#'+td_nome).html() == ''){
			$('#'+td_nome).html('Carregando...');
			$('#'+idImg ).attr('src', '../imagens/menos.gif');
			carregaEmendaDetalhe(ptrid, td_nome);
		}if($('#'+tr_nome).css('display') == 'none' && $('#'+td_nome).html() != ""){
			$('#'+tr_nome).css('display', '');
			$('#'+idImg ).attr('src', '../imagens/menos.gif');
		} else {
			$('#'+tr_nome).css('display', 'none');
			$('#'+idImg ).attr('src', '../imagens/mais.gif');
		}
	}
	function carregaEmendaDetalhe(ptrid, td_nome){
				
		$.ajax({
				type: "POST",
				url: 'par.php?modulo=principal/propostaEmenda&acao=A&carregaEmendaDetalhePTA=true&ptrid='+ptrid,
				async: false,
				success: function(retornoajax) {
					$('#'+td_nome).html(retornoajax);
				}
			});
	}
</script>
