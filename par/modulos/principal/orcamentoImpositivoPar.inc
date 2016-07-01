<?php

$inuid = $_SESSION ['par'] ['inuid'];

if (! $inuid || $inuid == '') {
	echo "<script>alert('Entidade não encontrada!');window.close();</script>";
	exit ();
}

$ano = $_REQUEST ['ano'];

if ($_REQUEST ['carregaEmendaDetalhePTA']) {
	header ( 'content-type: text/html; charset=ISO-8859-1' );
	
	if( $ano > 2014 ) $filtroImped = " and edi.edeid = vede.edeid ";
		
	$sql = "SELECT DISTINCT
				'<center><img border=\"0\" title=\"Impositivos\" src=\"../imagens/alterar.gif\" style=\"cursor: pointer\" onclick=\"abreEmpedimento('||vede.emdid||', \''||vede.edevalor||'\', '||vede.edeid||');\" alt=\"Ir\"/></center>' as acoes, 
				vede.emecod, 
				aut.autnome, 
				fup.fupdsc, 
				vede.gndcod||' - '||gn.gnddsc as gndcod1, 
				vede.mapcod||' - '||map.mapdsc as modalidade, 
				vede.foncod||' - '||fon.fondsc as fonte, 
				(sum(vede.edevalor) - coalesce(edi.edivalor,0)) as valortotal
			FROM emenda.planotrabalho ptr
			    INNER JOIN emenda.ptemendadetalheentidade  ped ON ptr.ptrid = ped.ptrid
			    INNER JOIN emenda.v_emendadetalheentidade vede ON vede.edeid = ped.edeid
			    left JOIN emenda.v_funcionalprogramatica fup ON (vede.acaid = fup.acaid)
			    inner JOIN emenda.autor aut ON aut.autid = vede.autid
                left join public.gnd gn on gn.gndcod = vede.gndcod and gn.gndstatus = 'A'
			    left join public.modalidadeaplicacao map on map.mapcod = vede.mapcod
			    left join public.fonterecurso fon on fon.foncod = vede.foncod and fon.fonstatus = 'A'
			    left join emenda.emendadetalheimpositivo edi on edi.emdid = vede.emdid and edi.edistatus = 'A' $filtroImped
			WHERE ptr.ptrid = {$_REQUEST['ptrid']}
			    AND vede.edestatus = 'A' 
			    and vede.emetipo = 'E'
			GROUP BY
				vede.emecod, 
				vede.edeid,
				aut.autnome, 
				fup.fupdsc, 
				vede.gndcod,
				gn.gnddsc, 
				vede.mapcod,
				map.mapdsc, 
				vede.foncod,
				fon.fondsc,
				vede.edevalor,
				vede.emdid,
				edi.edivalor";
	
	/*echo '<table class="tabela" cellspacing="0" cellpadding="3" border="0" bgcolor="#dcdcdc" align="center" style="border-top: medium none; border-bottom: medium none;">
				<tr>
					<td bgcolor="#e9e9e9" align="center" style=""><b>Recurso(s) Vinculado(s) ao Plano de Trabalho</b></td>
				</tr>
			</table>';*/
	$cabecalho = array ('Acoes',
			"Recurso",
			"Autor",
			"Funcional Programática",
			"GND",
			"Mod",
			"Fonte",
			"Valor" 
	);
	echo '<table cellspacing="0" cellpadding="3" border="0" align="right" style="border-top: medium none; border-bottom: medium none; width: 100%">
				<tr>
					<td align="right" style="width: 100%">';
	echo $db->monta_lista_simples ( $sql, $cabecalho, 20, 5 );
	ECHO '</td>
				</tr>
			</table>';
	exit ();
}

$entidadePar = ($_SESSION ['par'] ['itrid'] == 1) ? $_SESSION ['par'] ['estuf'] : $_SESSION ['par'] ['muncod'];
$nmEntidadePar = EntidadeParControle::recuperaDescricaoEntidadePar ( $entidadePar, $_SESSION ['par'] ['itrid'] );
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
<?

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

monta_titulo( $nmEntidadePar, 'Orçamento Impositivo' );
echo '<br>';

if ($_REQUEST ['aba'] == 'emenda') {
	$abaAtivaGeral = 'par.php?modulo=principal/propostaEmenda&acao=A&aba=emenda&ano=' . $_REQUEST ['ano'];
} elseif ($_REQUEST ['aba'] == 'subacao') {
	$abaAtivaGeral = 'par.php?modulo=principal/propostaEmendaSubacao&acao=A&aba=subacao&ano=' . $_REQUEST ['ano'];
} elseif ($_REQUEST ['aba'] == 'orcamento') {
	$abaAtivaGeral = 'par.php?modulo=principal/orcamentoImpositivoPar&acao=A&aba=orcamento&ano=' . $_REQUEST ['ano'];
}
/* Array com os itens da aba de identificação */
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
echo montarAbasArray ( $menuGeral, $abaAtivaGeral );

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

carregarPTA ( $inuid, $ano );

/*$sql = "select ptrid 
		from emenda.planotrabalho p
			inner join emenda.entidadebeneficiada eb on eb.enbid = p.enbid
		where
			p.sisid = 23
		    and p.ptrstatus = 'A'
		    and eb.enbcnpj = '".pegaCnpj ( $inuid )."'
		    and p.ptrexercicio = '$ano'";
$arrPtrid = $db->carregarColuna($sql);

if( empty( $arrPtrid[0] ) ){
	echo "<script>
			alert('Código do Plano de Trabalho não encontrado.');
			history.back(-1);
		</script>";
	exit();
} else {
	include_once APPRAIZ.'emenda/classes/EmendaImpositivo.class.inc';
	
	$boImpositivo = new EmendaImpositivo( $arrPtrid[0], $ano );
	$sql = $boImpositivo->carregarEmenda();
	
	include_once APPRAIZ.'www/emenda/orcamentoImpositivo.php';
}*/
?>
</body>
<script type="text/javascript">
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
			url: 'par.php?modulo=principal/orcamentoImpositivoPar&acao=A&carregaEmendaDetalhePTA=true&ptrid='+ptrid,
			async: false,
			success: function(retornoajax) {
				$('#'+td_nome).html(retornoajax);
			}
		});
}

function abreEmpedimento( emdid, valor, edeid ){
		
	var largura = 900;
	var altura = 700;
	//pega a resolução do visitante
	w = screen.width;
	h = screen.height;
	
	//divide a resolução por 2, obtendo o centro do monitor
	meio_w = w/2;
	meio_h = h/2;
	
	//diminui o valor da metade da resolução pelo tamanho da janela, fazendo com q ela fique centralizada
	altura2 = altura/2;
	largura2 = largura/2;
	meio1 = meio_h-altura2;
	meio2 = meio_w-largura2;
		
	window.open('<?=$_SERVER['SCRIPT_NAME']?>?modulo=principal/emendaImpositivo&acao=A&emdid='+emdid+'&valor='+valor+'&edeid='+edeid,'Emenda Empendimento','height=' + altura + ', width=' + largura + ', top='+meio1+', left='+meio2+',scrollbars=yes,location=no,toolbar=no,menubar=no');
}
</script>
</html>