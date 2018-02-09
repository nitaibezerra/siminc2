<?php
$_SESSION["emenda"]["ptridAnalise"] = !empty( $_REQUEST["ptridAnalise"] ) ? $_REQUEST["ptridAnalise"] : $_SESSION["emenda"]["ptridAnalise"];
$ptrid = $_SESSION["emenda"]["ptridAnalise"];

if( $_POST['requisicaoComp'] == 'vincularComplemento' ){
	//ver($_REQUEST);
	
	$enbid 		= $_POST['enbid']; 
	$emecod 	= $_POST['emecod']; 
	$emdid 		= $_POST['emdid'];
	$emeid 	= $_POST['chk'];
	
	$sql = "SELECT gndcod, foncod, mapcod, emdvalor FROM emenda.emendadetalhe WHERE emeid = $emeid";
	$arrDetalhe = $db->pegaLinha($sql);
	
	$sql = "UPDATE emenda.emendadetalhe SET 
			  emeid = $emeid,
			  gndcod = {$arrDetalhe['gndcod']},
			  foncod = {$arrDetalhe['foncod']},
			  mapcod = {$arrDetalhe['mapcod']},
			  emdvalor = {$arrDetalhe['emdvalor']},
			  emddataalteracao = now(),
			  usucpfalteracao = '{$_SESSION['usucpf']}'			 
			WHERE 
			  emdid = $emdid";
	$db->executar($sql);
	$db->commit();
	
	$db->sucesso('principal/informacoesGerais');
	exit();
}

if( $_REQUEST['requisicao'] == 'alterarComplemento' ){
	header('content-type: text/html; charset=ISO-8859-1');
	
	$enbid 	= $_POST['enbid']; 
	$emecod = $_POST['emecod']; 
	$emdid = $_POST['emdid'];
	
	$sql = "SELECT distinct
				'<center><input type=\"radio\" name=\"chk\" id=\"chk\" value=\"'||ve.emeid||'\"></center>' as acoes,
				ve.emecod,
			    v.fupfuncionalprogramatica||'&nbsp;' as fupfuncionalprogramatica,
			    v.fupdsc,
			    r.resdsc,
			    ve.emeano
			FROM
				emenda.v_funcionalprogramatica v
			    inner join emenda.v_emendadetalheentidade ve on ve.acaid = v.acaid
			    inner join emenda.responsavel r on r.resid = ve.resid
			WHERE
				ve.emetipo = 'X'
			    and ve.emecod <> '$emecod'
			    and ve.ededisponivelpta = 'S'
			    and ve.entid in (select enbid from emenda.entidadebeneficiada where enbcnpj = (select enbcnpj from emenda.entidadebeneficiada where enbid = $enbid))
		order by ve.emeano";
	
	$cabecalho = array('Selecione', 'Código', 'Funcional Prg', 'Descricao', 'Responsável', 'Ano');
	//formComplemento
	echo '<input type="hidden" name="enbid" id="enbid" value="'.$enbid.'">';
	echo '<input type="hidden" name="emecod" id="emecod" value="'.$emecod.'">';
	echo '<input type="hidden" name="emdid" id="emdid" value="'.$emdid.'">';
	
	$db->monta_lista_simples($sql, $cabecalho, 10000000, 30, 'N', '100%', '', '', '', '', true);
	exit();
}

if( $_REQUEST['carregarPI'] ){
	
	if( $_POST['ptres'] ){
		if( $_POST['emetipo'] == 'X' ) $filtro = " and pi.pliano >= '".$_SESSION['exercicio']."'";
		else $filtro = " and pi.pliano = '".$_SESSION['exercicio']."'";
										
		$sql = "SELECT DISTINCT 
			    pi.plicod as codigo, 
			    pi.plicod ||' - '||pi.plititulo as descricao
			FROM monitora.pi_planointerno pi
				inner join monitora.pi_planointernoptres plpt on pi.pliid = plpt.pliid
				inner join monitora.ptres pt on pt.ptrid = plpt.ptrid
			WHERE 
			    pt.acaid = ".$_POST['acaid']."
			    and  pt.ptres = '".$_POST['ptres']."'
			    and pi.plisituacao in ('S', 'C', 'A', 'T')
			    and pi.plistatus = 'A'
			    $filtro
			order by pi.plicod";
	} else {
		$sql = array();
	}
	$filtro = '';									
	echo $db->monta_combo('pi['.$_POST['emeid'].']['.$_POST['pedid'].']', $sql, 'S', 'Selecione...', '', '', '', '', 'S', 'pi', '', '', '', 'class="sel_chosen"' );
	exit();
}

if( $_REQUEST['requisicao'] == 'salvarorcamento' ){
	//ver($_POST,d);
	$ptrid = $_SESSION["emenda"]["ptridAnalise"];

	foreach ($_POST['emeid'] as $emeid => $arPedid) {
		foreach ($arPedid as $pedid => $v) {
			//$pedid		= $_POST['pedid'][$emeid][$tpeid];
			$acaid 		= $_POST['acaid'][$emeid][$pedid];
			$pervalor 	= $_POST['pervalor'][$emeid][$pedid];
			$ptres 		= $_POST['ptres'][$emeid][$pedid];
			$pi 		= $_POST['pi'][$emeid][$pedid];
			$exfvalor 	= $_POST['exfvalor'][$emeid][$pedid];
			$exfid 		= $_POST['exfid'][$emeid][$pedid];
			$tpeid 		= $_POST['tpeid'][$emeid][$pedid];
			
			$exfvalor 	= ($exfvalor ? $exfvalor : '0,00');
			
			if( ($ptres && $pi) || $exfvalor ) {
				if( $exfid ){
					$sql = "UPDATE emenda.execucaofinanceira SET 
							plicod = '{$pi}',
							exfdataalteracao = now(),
							usucpf = '".$_SESSION['usucpf']."',
							exfvalor = ".retiraPontos($exfvalor).",
							tpeid = ".$tpeid.",
							ptres = '".$ptres."'
						WHERE
							exfid = ".$exfid;
					$db->executar($sql);
					//ver($sql);
				} else {
					$sql = "INSERT INTO emenda.execucaofinanceira(tpeid, plicod, ptrid, pedid, exfvalor, exfdatainclusao, usucpf, ptres) 
							VALUES ({$tpeid}, '{$pi}', {$ptrid}, {$pedid}, ".retiraPontos($exfvalor).", now(), '".$_SESSION['usucpf']."', '{$ptres}')";
					$db->executar($sql);
					//ver($sql);
				}
			}			
		}		
	}
	
	$db->commit();
	$db->sucesso( 'principal/informacoesGerais', '' );
	exit();
}

if( $_REQUEST['requisicao'] == 'salvaranalise' ){
	
	$ptrid		= $_SESSION["emenda"]["ptridAnalise"];	
	
	$_POST['ptrnumprocessoempenho'] = str_replace(".","", $_POST['ptrnumprocessoempenho']);
	$_POST['ptrnumprocessoempenho'] = str_replace("/","", $_POST['ptrnumprocessoempenho']);
	$_POST['ptrnumprocessoempenho'] = str_replace("-","", $_POST['ptrnumprocessoempenho']);
	$_POST['ptrnumdocumenta'] = str_replace("/","", $_POST['ptrnumdocumenta']);
	$_POST['ptrnumdocumenta'] = str_replace("-","", $_POST['ptrnumdocumenta']);

	$sql = "UPDATE emenda.planotrabalho SET ptrnumprocessoempenho = '{$_POST["ptrnumprocessoempenho"]}', ptrnumdocumenta = '{$_POST["ptrnumdocumenta"]}' WHERE ptrid = {$ptrid}";
	$db->executar( $sql );
	
	$arrUnidades = $_POST["ckb_unidade"];
	
	if($arrUnidades && is_array($arrUnidades) ){
		$sql = "delete from emenda.analise where ptrid = {$ptrid} and anatipo = 'T' and anastatus = 'A' and anadataconclusao is null";
		$db->executar($sql);

		$analote = $db->pegaUm("select max(analote) from emenda.analise where ptrid = {$ptrid} and anastatus = 'A' and anatipo = 'T'");
		$analote = !empty($analote) ? $analote + 1 : 1;

		foreach($arrUnidades as $unidade){

			$sql = "select anaid from emenda.analise where ptrid = {$ptrid} and anatipo = 'T' and uniid = {$unidade} and anastatus = 'A' and anadataconclusao is null";
			$anaid = $db->pegaUm($sql);

			if( empty($anaid) ){
				$sql = "insert into emenda.analise(ptrid, uniid, anatipo, analote, anadatainclusao, anastatus) 
						 values({$ptrid}, {$unidade}, 'T', {$analote}, now(), 'A')";

				$db->executar($sql);
			}

		}
	}
	
	$db->commit();
	$db->sucesso( 'principal/informacoesGerais', '' );
	exit();
}

if ( !$ptrid ){
	echo '<script>
			alert("A sessão do Plano de Trabalho foi perdida!");
			window.location = "emenda.php?modulo=principal/listaPtaAnalise&acao=A";
		  </script>';
	die;
}

include_once APPRAIZ . 'includes/workflow.php';
include  APPRAIZ."includes/cabecalho.inc";
print "<br/>";

$boReformulacao = pegaPaiPTA( $_SESSION["emenda"]["ptridAnalise"] );
$boReformulacao = ( !empty($boReformulacao) ? true : false );

$estadoAtual = pegarEstadoAtual( $_SESSION["emenda"]["ptridAnalise"] );

montaAbasAnalise( $abacod_tela, $url, '' , $estadoAtual, $boReformulacao);

monta_titulo( 'Análise do Gestor', $subtitulo_modulo );


echo cabecalhoPlanoTrabalho($ptrid, true);
$docid = criarDocumento( $ptrid );

$arDados = $db->pegaLinha("SELECT ptrnumprocessoempenho, ptrnumdocumenta FROM emenda.planotrabalho WHERE ptrid = {$ptrid}");
	
if( $arDados['ptrnumprocessoempenho'] && $arDados['ptrnumdocumenta'] ){
	$arDados['ptrnumprocessoempenho'] = substr($arDados['ptrnumprocessoempenho'],0,5) . "." .
										substr($arDados['ptrnumprocessoempenho'],5,6) . "/" .
										substr($arDados['ptrnumprocessoempenho'],11,4) . "-" .
										substr($arDados['ptrnumprocessoempenho'],15,2);
								 
	$arDados['ptrnumdocumenta'] = substr($arDados['ptrnumdocumenta'],0,7) . "/" .
								  substr($arDados['ptrnumdocumenta'],7,4) . "-" .
								  substr($arDados['ptrnumdocumenta'],11,2);
}
						

extract($arDados);

montaVisualizarPTA( $ptrid, true, $boReformulacao );
abrePTAAnaliseRelatorio($ptrid, 'T', $boReformulacao, false);

if( $_SESSION['exercicio'] > 2014 )	$filtroImp = ' and ei.edeid = vede.edeid';

$sql = "SELECT
			vede.acaid,
		    COALESCE(fup.fupfuncionalprogramatica, 'Dados Incompletos') as fupfuncionalprogramatica, 
		    vede.emecod,
		   	vede.emeid,
		   	vede.emetipo,
		   	vede.entid,
		   	vede.emdid,
		    (CASE WHEN vede.emetipo = 'E' THEN 'Emenda' ELSE 'Complemento' END) as tipoemenda, 
		    case when vede.emerelator = 'S' then aut.autnome||' - Relator Geral' else aut.autnome end as autnome,
		    vede.gndcod as gnd, 
		    vede.gndcod||' - '||gn.gnddsc as gndcod,
		    vede.mapcod||' - '||map.mapdsc as mapcod,
		    vede.foncod||' - '||fon.fondsc as foncod, 
		    per.pedid, 
		    tpe.tpedsc,
		    tpe.tpeid, 
		    (sum(per.pervalor) - coalesce(ei.edivalor, 0)) as pervalor,
		    pti.ptrid,
		    case when vede.emdliberado = 'S' then 'Sim' else 'Não' end as liberado
		FROM emenda.ptiniciativa pti
			inner join emenda.ptiniciativaespecificacao pte on pte.ptiid = pti.ptiid and pte.ptestatus = 'A'
		 	inner join emenda.ptemendadetalheentidade ped on ped.ptrid = pti.ptrid
		 	inner join emenda.ptiniesprecurso per on per.pteid = pte.pteid and per.pedid = ped.pedid
		   	inner join emenda.tipoensino tpe on pti.tpeid = tpe.tpeid
		   	inner join emenda.v_emendadetalheentidade vede on vede.edeid = ped.edeid
		   	inner join emenda.autor aut on aut.autid = vede.autid
		   	left join emenda.v_funcionalprogramatica fup on fup.acaid = vede.acaid
		  	left join public.gnd gn on gn.gndcod = vede.gndcod and gn.gndstatus = 'A'
		  	left join public.modalidadeaplicacao map on map.mapcod = vede.mapcod
		  	left join public.fonterecurso fon on fon.foncod = vede.foncod and fon.fonstatus = 'A'
		  	left join emenda.emendadetalheimpositivo ei on ei.emdid = vede.emdid and ei.edistatus = 'A' $filtroImp
		WHERE 
		    pti.ptrid = $ptrid
		    and per.pervalor <> 0
		GROUP BY 
		    per.pedid, tpe.tpedsc, tpe.tpeid, vede.emecod,
		    vede.emeid, aut.autnome, vede.emerelator, vede.gndcod, vede.mapcod, 
		    vede.foncod, fup.fupfuncionalprogramatica, vede.acaid,
		    vede.emetipo, gn.gnddsc, map.mapdsc, fon.fondsc,
		    vede.emdliberado, pti.ptrid, vede.entid, vede.emdid, ei.edivalor
		ORDER BY
			vede.emeid, vede.gndcod, fup.fupfuncionalprogramatica,
			per.pedid, tpe.tpedsc, vede.emecod, 
		    vede.mapcod, vede.foncod, vede.acaid";

$arDados = $db->carregar($sql);
$arDados = ($arDados ? $arDados : array());

$arPerfil = pegaPerfilArray($_SESSION['usucpf']);

if( $_SESSION['exercicio'] == '2009' || $boReformulacao ){
	$liberaCasa = 'display: none;';
}
?>
<script type="text/javascript" src="js/emenda.js"></script>
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<script type="text/javascript" src="../includes/JQuery/jquery-ui-1.8.14.custom.min.js"></script>
<link rel="stylesheet" href="../includes/JQuery/jquery-ui-1.8.4.custom/css/jquery-ui.css" type="text/css" media="all" />
<style>
.button{
	width: 120px; 
	height: 25px;
	cursor:pointer;
}

.btn{
	cursor:pointer; 
	width:90px;
	height: 35px;
	white-space: normal;
}
tr th{
	text-align: center;
}
</style>
<form action="" method="post" id="formulario" name="formulario">
	<input type="hidden" name="requisicao" id="requisicao" value="">
	<input type="hidden" name="ptrid" id="ptrid" value="<?=$ptrid ?>">
	<?php abrePTAconsolidado($ptrid, '', 'entidade'); ?>
	<table class="tabela" width="95%" bgcolor="#f5f5f5" cellspacing="1" cellpadding="2" align="center" border="0">
	<tr>
		<td>
		<table class="tabela" style="width: 100%" cellspacing="1" cellpadding="2" align="center" border="0">
			<tr><th colspan="2">Programação Orçamentária </th></tr>
			<tr>
				<td style="text-align: center;" colspan="2"> <img border='0' src='../imagens/obrig.gif' title='Indica campo obrigatório.' /> Indica os campos obrigatórios </td>
			</tr>
			<tr>
				<td colspan="2"><?
foreach ($arDados as $chave => $valor){
					$chave % 2 ? $cor = "#dedfde" : $cor = "";
					
					$color = 'rgb(0, 102, 204)';
					if( $valor['liberado'] == 'Não' ) $color = 'red';
					
					$sql = "SELECT e.exfid, e.ptres, e.plicod, e.exfvalor, e.pliid, se.semdesc, se.semid FROM emenda.execucaofinanceira e
								left join emenda.situacaoempenho se on se.semid = e.semid and se.semstatus = 'A'
							WHERE e.ptrid = {$valor['ptrid']} and e.pedid = {$valor['pedid']} and e.tpeid = {$valor['tpeid']} and e.exfstatus = 'A'";
					
					$arPlano = $db->pegaLinha($sql);
					
					$boEmepenhado = '';
					$disabBtnComp = '';
					if( $arPlano['semid'] == 4 || $arPlano['semid'] == 3 ){
						$boEmepenhado = 'disabled="disabled"';
						
					}
					
					if($valor['emetipo'] != 'X' || !empty($boEmepenhado) || 
						(!in_array(ADMINISTRADOR_INST , $arPerfil) && 
						!in_array(ADMINISTRADOR_REFORMULACAO , $arPerfil) && 
						!in_array(SUPER_USUARIO , $arPerfil)
						)  ){
						$disabBtnComp = 'disabled="disabled"';
					}
					?>
					<input type="hidden" name="emeid[<?=$valor['emeid'] ?>][<?=$valor['pedid'] ?>]" value="<?=$valor['emeid'] ?>">
					<input type="hidden" name="tpeid[<?=$valor['emeid'] ?>][<?=$valor['pedid'] ?>]" value="<?=$valor['tpeid'] ?>">
					<input type="hidden" name="acaid[<?=$valor['emeid'] ?>][<?=$valor['pedid'] ?>]" value="<?=$valor['acaid'] ?>">
					<input type="hidden" name="pedid[]" value="<?=$valor['pedid'] ?>">
					<input type="hidden" name="pervalor[<?=$valor['emeid'] ?>][<?=$valor['pedid'] ?>]" value="<?=$valor['pervalor'] ?>">
					
					<table class="tabela" width="100%" cellspacing="1" cellpadding="2" align="center" onmouseout="this.bgColor='';" onmouseover="this.bgColor='#ffffcc';">
					<thead>
						<tr bgcolor="dedfde">
							<th align="Center" class="title" width="05%" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;"
								onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>Ações</strong></th>
							<th align="Center" class="title" width="05%" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;"
								onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>Funcional</strong></th>
							<th align="Center" class="title" width="05%" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;"
								onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>Tipo</strong></th>
							<th align="Center" class="title" width="05%" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;"
								onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>Código</strong></th>
							<th align="Center" class="title" width="15%" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;"
								onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>Autor</strong></th>
							<th align="Center" class="title" width="10%" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;"
								onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>GND</strong></th>
							<th align="Center" class="title" width="10%" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;"
								onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>Mod</strong></th>
							<th align="Center" class="title" width="30%" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;"
								onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>Fonte</strong></th>
							<th align="Center" class="title" width="10%" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;"
								onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>Nível de Ensino</strong></th>
							<th align="Center" class="title" width="10%" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;"
								onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>Valor</strong></th>
							<th align="Center" class="title" width="05%" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff; <?=$liberaCasa ?>"
								onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>Limite Autorizado</strong></th>
						</tr>
					</thead>
					<tbody>
					<tr>
						<td rowspan="2"><input type="button" name="btnComplemento" id="btnComplemento" onclick="alterarComplemento('<?=$valor['emecod'] ?>', '<?=$valor['emdid'] ?>', '<?=$valor['entid'] ?>');" value="Alterar Complemento" <?=$disabBtnComp; ?> class="btn"></td>
						<td style="text-align: center; color: rgb(0, 102, 204);"><?=$valor['fupfuncionalprogramatica'] ?></td>
						<td><?=$valor['tipoemenda'] ?></td>
						<td style="text-align: center; color: rgb(0, 102, 204);"><?=$valor['emecod'] ?></td>
						<td><?=$valor['autnome'] ?></td>
						<td><?=$valor['gndcod'] ?></td>
						<td><?=$valor['mapcod'] ?></td>
						<td><?=$valor['foncod'] ?></td>
						<td><?=$valor['tpedsc'] ?></td>
						<td style="text-align: center; color: rgb(0, 102, 204);">R$ <?=number_format($valor['pervalor'],2,',','.') ?></td>
						<td style="<?=$liberaCasa ?>; color: <?=$color ?>;"><?=$valor['liberado'] ?></td>
					</tr>
					<tr>
						<td colspan="10">
							<table width="90%" cellspacing="1" cellpadding="2" align="center">
								<thead>
								<tr>
									<th width="02%"><img border="0" src="../imagens/seta_filho.gif"></th>
									<th align="Center" class="title" width="18%"
										style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;"
										onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>PTRES</strong></th>
									<th align="Center" class="title" width="48%"
										style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;"
										onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>PI</strong></th>
									<th align="Center" class="title" width="18%"
										style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;"
										onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>Valor Utilizado</strong></th>
									<th align="Center" class="title" width="22%"
										style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;"
										onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>Situação Empenho</strong></th>
								</tr>
								</thead>
								<tbody>
								<tr>
									<td>&nbsp;</td>
									<td align="center"><?
										if( $valor['emetipo'] == 'X' ) $filtro = " and pi.pliano >= '".$_SESSION['exercicio']."'";
										else $filtro = " and pi.pliano = '".$_SESSION['exercicio']."'";
										
										if( empty($arPlano['ptres']) && !empty($arPlano['pliid']) && !empty($arPlano['plicod']) ){
											$arPlano['ptres'] = $db->pegaUm("select pliptres from monitora.planointerno where pliid = {$arPlano['pliid']} and plicod = '{$arPlano['plicod']}'");
										}
										
										$habCampo = 'S';
										if( !empty($boEmepenhado) ) $habCampo = 'N';
										
										$empenho = ( !empty($arPlano['semdesc']) ? '<span style="color: blue">'.$arPlano['semdesc'].'</span>' : '<span style="color: red">Não Solicitado</span>' );
										
										if( $valor['acaid'] ){
											$sql = "SELECT DISTINCT
														pt.ptres AS codigo,
														pt.ptres AS descricao
													FROM monitora.pi_planointerno pi
														inner join monitora.pi_planointernoptres plpt on pi.pliid = plpt.pliid
														inner join monitora.ptres pt on pt.ptrid = plpt.ptrid
													WHERE pt.acaid = {$valor['acaid']}
												    	and pi.plisituacao in ('S', 'C', 'A', 'T')
												    	$filtro";
											
											$arrPtres = $db->carregar($sql);
										}
										$arrPtres = $arrPtres ? $arrPtres : array();
										
										//echo $db->monta_combo('ptres['.$valor['emeid'].']['.$valor['tpeid'].']', $sql, 'S', 'Selecione...', 'carregarPI', '', '', 150, '', 'ptres', false, $arPlano['ptres'] ); ?>
										<select name="ptres[<?=$valor['emeid'] ?>][<?=$valor['pedid'] ?>]" <?=$boEmepenhado; ?> class="CampoEstilo obrigatorio" style="width:150px;" onchange="carregarPI(<?=$valor['emeid']; ?>, <?=$valor['tpeid']; ?>, <?=$valor['acaid']; ?>, '<?=$valor['emetipo']; ?>', <?=$valor['pedid'] ?>)" id="ptres">
											<option value="">Selecione...</option>
										<?	foreach ($arrPtres as $v) {
												$select = '';
												if( $v['codigo'] == $arPlano['ptres'] ) $select = 'selected="selected"';
												echo '<option value="'.$v['codigo'].'" '.$select.'>'.$v['descricao'].'</option>';
											}?>
										</select><?=obrigatorio(); 
										
										if( !empty($boEmepenhado) ) echo '<input type="hidden" name="ptres['.$valor['emeid'].']['.$valor['pedid'].']" id="ptres['.$valor['emeid'].']['.$valor['pedid'].']" value="'.$arPlano['ptres'].'">';
										?>										
										<input type="hidden" name="exfid[<?=$valor['emeid'] ?>][<?=$valor['pedid'] ?>]" value="<?=$arPlano['exfid'] ?>">
										</td>
									<td align="left">
										<div id="div_pi_<?=$valor['emeid'];?>_<?=$valor['pedid'] ?>">
										<?
										if( $arPlano['ptres'] && $valor['acaid'] ){
											$sql = "SELECT DISTINCT 
												    pi.plicod as codigo, 
												    pi.plicod ||' - '||pi.plititulo as descricao
												FROM monitora.pi_planointerno pi
													inner join monitora.pi_planointernoptres plpt on pi.pliid = plpt.pliid
													inner join monitora.ptres pt on pt.ptrid = plpt.ptrid
												WHERE 
												    pt.acaid = ".$valor['acaid']."
												    and  pt.ptres = '".$arPlano['ptres']."'
												    and pi.plisituacao in ('S', 'C', 'A', 'T')
												    and pi.plistatus = 'A'
												    $filtro
												order by pi.plicod";
										} else {
											$sql = array();
										}
										$filtro = '';
										$exfvalor = ( $arPlano['exfvalor'] ? number_format($arPlano['exfvalor'],2,',','.') : ($valor['pervalor'] ? number_format($valor['pervalor'],2,',','.') : '0,00') );									
										echo $db->monta_combo('pi['.$valor['emeid'].']['.$valor['pedid'].']', $sql, $habCampo, 'Selecione...', '', '', '', '', 'S', 'pi', false, $arPlano['plicod'], '', 'class="sel_chosen"' ); 
										
										if( !empty($boEmepenhado) ) echo '<input type="hidden" name="pi['.$valor['emeid'].']['.$valor['pedid'].']" id="pi['.$valor['emeid'].']['.$valor['pedid'].']" value="'.$arPlano['plicod'].'">';
										?>
										</div>
										</td>
										
									<td align="center"><?php									
									echo campo_texto('exfvalor['.$valor['emeid'].']['.$valor['pedid'].']', 'S', $habCampo, 'Valor Utilizado', 25, 13, '[###.]###,##', '','','','','id="exfvalor[]"','', $exfvalor,"this.value=mascaraglobal('[###.]###,##',this.value); validaRecursoInformado('".$valor['emeid']."', '".$valor['tpeid']."', '".$valor['pedid']."');"); ?></td>
									<td><?=$empenho; ?></td>
								</tr>
								</tbody>
							</table>
						</td>
					</tr>
					</tbody>
				</table>
				<br>
<?} ?>
				</td>
			</tr>
			<tr bgcolor="#D0D0D0">
				<td colspan="3" align="center">
					<input type="button" value="Salvar Orçamento" class="button" name="btnSalvarOrc" <?php echo verificaPermissaoPerfil('analise', '', '', true); ?> id="btnSalvarOrc" onclick="salvarOrcamento();" style="cursor:pointer; width:160px; white-space: normal;">
				</td>
			</tr>
			<tr>
				<th colspan="3">Informações Gerais</th>
			</tr>
			<tr>
				<td>
					<table class="tabela" width="95%" cellspacing="1" cellpadding="2" align="center" border="0">
					<tr>
						<td class="subtitulodireita" width="30%">Nº do Processo Empenho</td>
						<td width="70%">
							<?php 
								$boSolicitado = $db->pegaUm("select case when count(exfnumsolempenho) = 0 then 'S' else 'N' end as solicitado
															 from emenda.execucaofinanceira where exfnumsolempenho is not null and exfnumsolempenho <> '' and exfstatus = 'A' and ptrid = $ptrid");
								echo campo_texto( 'ptrnumprocessoempenho', 'S', "$boSolicitado", '', 52, 50, '#####.######/####-##', '', '', '', 0, 'id="ptrnumprocessoempenho"' );
							?>
						</td>
					</tr>
					<tr>
						<td class="subtitulodireita">Nº do Documento</td>
						<td valign="top">
							<?php 
								echo campo_texto( 'ptrnumdocumenta', 'S', 'S', '', 52, 50, '', '', '', '', 0, 'id="ptrnumdocumenta" onkeypress="return somenteNumeros(event);"', "this.value=mascaraglobal('#######/####-#',this.value);", '', "validaQuantidadeCaracter(this);" );
							?>
							&nbsp;&nbsp;&nbsp;<a href="#" onclick="tramitar('<?=$ptrnumdocumenta; ?>');"><img src="../imagens/consultar.gif" style="cursor:pointer;" title="Consultar histórico do documento no sistema Documenta do FNDE" border="0"></a>
						</td>
					</tr>
<? 
$ptridPai = pegaPaiPTA( $ptrid );
if( $_SESSION['exercicio'] != '2011' || !empty($ptridPai)  ){?>
					<tr>
						<td class="subtitulodireita">Unidades que realizarão a Análise Técnica do PTA</td>
						<td>
							<?php 
								if( !$boReformulacao )
									$filtro = 'and uniid != 11';
									
								$sql = "select uniid, uninome, unisigla from emenda.unidades where unistatus = 'A' $filtro order by uninome";
								$arrUnidades = $db->carregar($sql);
								$arrUnidades = !$arrUnidades ? array() : $arrUnidades; 
								
								$sql = "select uniid from emenda.analise 
										where ptrid = $ptrid and anatipo = 'T' and anastatus = 'A'
											and analote = (select max(analote) from emenda.analise where ptrid = {$ptrid} and anastatus = 'A' and anatipo = 'T')";
											
								$arUnidades = $db->carregar($sql);
								$arUnidades = !$arUnidades ? array() : $arUnidades;
								foreach($arUnidades as $unidade){
									$arUnidade[] = $unidade['uniid'];
								}
								$arUnidade = !$arUnidade ? array() : $arUnidade;
								
								?>
								<table>
									<tr>
								<?foreach($arrUnidades as $key => $arrUnidade){ 
									if($key == 3 || $key == $numDiv){
										$numDiv = $numDiv + 3;
										echo "</tr><tr>";
									}
									if( in_array($arrUnidade['uniid'],$arUnidade) ){
										echo '<input type="hidden" name="check[]" id="check" value="'.$arrUnidade['uniid'].'">';
									}
									?>
									<td><input <? echo in_array($arrUnidade['uniid'],$arUnidade) ? "checked='checked'" : "" ?> type="checkbox" value="<?=$arrUnidade['uniid'] ?>" name="ckb_unidade[]" title="<?=$arrUnidade['uninome'] ?>" id="unidade_<?=$arrUnidade['uniid'] ?>" /><label style="margin-right:10px" for="unidade_<?=$arrUnidade['uniid'] ?>" title="<?=$arrUnidade['uninome'] ?>" ><?=$arrUnidade['unisigla'] ?></label></td> 
								<? $cont++; }?>
									</tr>
								</table>
							
						</td>
					</tr>
<?} ?>
					</table>
				</td>
			</tr>
			<tr bgcolor="#D0D0D0">
				<td colspan="3" align="center">
					<input type="button" value="Salvar Análise" class="button" name="btnSalvar" <?php echo verificaPermissaoPerfil('analise', '', '', true); ?> id="btnSalvar" onclick="salvaAnaliseGestor();" style="cursor: pointer;">
					<input type="button" value="Voltar" class="button" onclick="history.back(-1);" style="cursor: pointer;">
				</td>
			</tr>
		</table>
		</td>
		<td valign="top" align="center">
			<?php
				wf_desenhaBarraNavegacao( $docid , array( 'url' => $_SESSION['favurl'], 'ptrid' => $ptrid, 'resid' => '' , 'tipo' => "M" ) );
				
				$arFluxo = verificaFluxo( $ptrid );
				
				if( $arFluxo ){
					$fluxo = implode(', ', $arFluxo);
				}
			?>
		</td>
	</tr>
</table>
</form>
<div id="debug"></div>
<div id="dialog_acoes" title="Complementos Orçamentários" style="display: none" >
	<form action="" method="post" id="formComplemento" name="formComplemento">
		<input type="hidden" name="requisicaoComp" id="requisicaoComp" value="">
		<div style="padding:5px;text-align:justify;" id="mostraRetorno"></div>
	</form>
</div>
<script type="text/javascript">

$(document).ready(function(){
	jq('.sel_chosen').chosen({allow_single_deselect:true});

	$.each([ <?=$fluxo ?> ], function( index, value ) {
		$('#td_acao_'+value).css('display', 'none');
		//$('#td_acao_445').css('display', 'none');
	});
	
	$('#exfvalor[]').focusin(function(){
		alert('teste');
		if( $(this).val() == 0 ){
			$(this).val('');
		}
	});
	
	$('#exfvalor[]').focusout(function(){
		if( $(this).val() == '' ){
			$(this).val('0');
		}
	});
});

function alterarComplemento(emecod, emdid, enbid){
	$.ajax({
			type: "POST",
			url: "emenda.php?modulo=principal/informacoesGerais&acao=A",
			data: "requisicao=alterarComplemento&emdid="+emdid+'&enbid='+enbid+'&emecod='+emecod,
			async: false,
			success: function(msg){
				$( "#dialog_acoes" ).show();
				$( "#mostraRetorno" ).html(msg);
				$( '#dialog_acoes' ).dialog({
						resizable: false,
						width: 800,
						modal: true,
						show: { effect: 'drop', direction: "up" },
						buttons: {
							'Salvar': function() {
								if( $('[name="chk"]:checked').length == '0'){
									alert('Selecione um complemento!');
									return false;
								}
								$('#requisicaoComp').val('vincularComplemento');
								$('#formComplemento').submit();
								$( this ).dialog( 'close' );
							},
							'Cancelar': function() {
								$( this ).dialog( 'close' );
							}
						}
				});
			}
		});
} 

function carregarPI( emeid, tpeid, acaid, emetipo, pedid ){
	var ptres = $('[name="ptres['+emeid+']['+pedid+']"]').val();
	var div_pi = $('#div_pi_'+emeid+'_'+pedid);
	
	$.ajax({
		type: "POST",
		url: 'emenda.php?modulo=principal/informacoesGerais&acao=A',
		data: "carregarPI=true&acaid="+acaid+"&emetipo="+emetipo+"&emeid="+emeid+"&tpeid="+tpeid+"&ptres="+ptres+"&pedid="+pedid,
		async: false,
		success: function(msg){
			$(div_pi).html(msg);
			jq('.sel_chosen').chosen({allow_single_deselect:true});
		}
	});
}

function salvarOrcamento(){
	var erro = false;
	var nome = '';
	
	$('.obrigatorio').each(function(){
		if( $(this).val() == '' ){
			nome = $(this).attr('name');
			erro = true;
		}
	});
	
	if( erro  ){
		if(!confirm('Existe(m) campo(s) obrigátorio(s) em branco, deseja continuar mesmo assim!')){		
			$('[name="'+nome+'"]').focus();
			return false;
		}
	}
	
	$('#requisicao').val('salvarorcamento');
	$('#formulario').submit();
}

function salvaAnaliseGestor(){
		
	if( $('[name="ptrnumprocessoempenho"]').val() == '' ){
		alert('O campo "Nº do Processo Empenho" é obrigatório.');
		$('[name="ptrnumprocessoempenho"]').focus();
		return false;
	}
	if( $('[name="ptrnumdocumenta"]').val() == '' ){
		alert('O campo "Nº do Documento" é obrigatório.');
		$('[name="ptrnumdocumenta"]').focus();
		return false;
	}
	
	if( $('[name="ckb_unidade[]"]:checked').length == '0'){
		alert('Selecione uma unidade.');
		return false;
	}
	$('#requisicao').val('salvaranalise');
	$('#formulario').submit();
}

function validaRecursoInformado(emeid, tpeid, pedid){
	
	var valorCadastrado = $('[name="pervalor['+emeid+']['+pedid+']"]').val();
	var valorInformado = $('[name="exfvalor['+emeid+']['+pedid+']"]').val();
	
	valorInformado = valorInformado.replace(/\./gi, '');
	valorInformado = valorInformado.replace(/\,/gi, '.');
	
	if( valorInformado > 0 ){
		if(parseFloat(valorInformado) > parseFloat(valorCadastrado) ){
			var v = valorInformado - valorCadastrado;
			v = 'R$ ' + mascaraglobal('###.###.###.###,##', v.toFixed(2));
			valor = 'R$ ' + mascaraglobal('###.###.###.###,##', valorCadastrado);
			alert('A soma dos valores preenchidos para os PIs é MAIOR que o valor disponibilizado pelo recurso '+valor+'.\n' + 'Valor remanescente: \t'+ v);
			$('[name="exfvalor['+emeid+']['+pedid+']"]').val('');
			return false;
		}
	} else {
		$('[name="exfvalor['+emeid+']['+pedid+']"]').val('0,00');
	}
}

function tramitar(documento){
	var num_documenta = '';
	if( documento == '' ){
		num_documenta = document.getElementById('ptrnumdocumenta').value;
	}else{
		num_documenta = documento;
	}
	if( num_documenta != '' ){
		num_documenta = num_documenta.split('/');
		var zero = '';
		if( num_documenta[0].length < 7 ){
			for(i=num_documenta[0].length; i<7; i++){
				zero = zero + '0';					
			}
			num_documenta[0] = zero+num_documenta[0];
		}
		num_documenta[1] = num_documenta[1].substr(0,4);
		window.open( "http://www.fnde.gov.br/pls/tramita_fnde/!tramita_fnde.tmtconrelatorio_pc?cha="+num_documenta[1] + num_documenta[0]+"&usu=03468", '_blank');
	}
}

function validaQuantidadeCaracter(campo){
	
	if(campo.value){
		var valor = campo.value.replace("/", "");
		valor = valor.replace("-", "");
		
		var quant = valor.length;
	
		var comp = "";
		if(quant < 12 ){
			comp = 0;
			quant++;
			for(i=quant; i<12; i++){
				comp = comp + '0';
			}
		}
		
		campo.value = mascaraglobal('#######/####-#', comp + campo.value);
	}
}
</script>