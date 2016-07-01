<?
include_once "_funcoesdetalhamentoindicador.php";
// carrega as funções gerais
include_once "config.inc";
include ("../../includes/funcoes.inc");
include ("../../includes/classes_simec.inc");
include "_constantes.php";

// abre conexão com o servidor de banco de dados
$db = new cls_banco();

/* configurações */
ini_set("memory_limit", "1024M");
set_time_limit(0);
/* FIM configurações */

if(!$_SESSION['usucpf'] || $_SESSION['sisid'] != 48) die("<script>
															alert('Não autenticado corretamente');
															window.location='../login.php';
														  </script>");

?>
<html>
<head>
<meta http-equiv="Cache-Control" content="no-cache">
<meta http-equiv="Pragma" content="no-cache">
<meta http-equiv="Connection" content="Keep-Alive">
<meta http-equiv="Expires" content="Mon, 26 Jul 1997 05:00:00 GMT">
<title><?= $titulo ?></title>
<script type="text/javascript" src="../includes/JQuery/jquery2.js"></script>
<script type="text/javascript" src="../includes/funcoes.js"></script>
<script language="JavaScript" src="../includes/prototype.js"></script>
<script language="JavaScript" src="./js/painel.js"></script>
<script language="JavaScript" src="./js/detalhamentoindicador.js"></script>
<script language="javascript" type="text/javascript" src="/includes/open_flash_chart/swfobject.js"></script>
<script>
function controleAcoes(ac) {
	for(i=0;i<document.getElementById("tabela").rows.length;i++) {
		if(document.getElementById("tabela").rows[i].id.substr(0,5) == "tr_m_") {
			document.getElementById("tabela").rows[i].cells[0].childNodes[0].title=ac;
			document.getElementById("tabela").rows[i].cells[0].childNodes[0].onclick();
		}
	}
}

function carregarAcao(acaid, indids, obj) {

	var tabela = obj.parentNode.parentNode.parentNode;
	var linha  = obj.parentNode.parentNode;
	if(obj.title == "mais") {
		obj.title="menos";
		obj.src="../imagens/menos.gif";
		var nlinha = tabela.insertRow(linha.rowIndex+1);
		nlinha.style.background = '#f5f5f5';
		var col0 = nlinha.insertCell(0);
		col0.colSpan=tabela.rows[0].cells.length;
		col0.id="colid_"+acaid;
		divCarregando(col0);
		ajaxatualizar('requisicao=listaIndicadores&detalhes=<? echo $_REQUEST['detalhes']; ?>&acaid='+acaid+'&indids='+indids, 'colid_'+acaid);
		divCarregado(col0);
	} else {
		obj.title="mais";
		obj.src="../imagens/mais.gif"
		tabela.deleteRow(linha.rowIndex+1);
	}

}

</script>
<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
<link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>
<style>
.boxindicador {
	width:400px;
	height:100px;
	position: absolute;
	background-color: #FCFCFC;
	border: solid 1px #000000;
	padding: 3px;
	margin-left:30px;
	display:none;
}
</style>
<?
if($_REQUEST['requisicao']) {
	$_REQUEST['requisicao']($_REQUEST);
	exit;
}
?>
<script type="text/javascript">
this._closeWindows = false;
</script>
</head>
<body style="margin:10px; padding:0; background-color: #fff; background-image: url(../imagens/fundo.gif); background-repeat: repeat-y;">
<?php

if($_REQUEST['indid']) {
	$sql = "SELECT * FROM painel.indicador WHERE indid='".$_REQUEST['indid']."'";
	$dadosi = $db->pegaLinha($sql);
	$estrutura = getEstruturaRegionalizacao($dadosi['regid']);
	$estloop = $estrutura;
}

if($estloop) {
	
	$html .= "<table cellSpacing=0 cellPadding=3 style=\"width:100%;background-color:#FCFCFC;\">";
	
	do {
		$estor[] = $estloop['atu'];
		$estloop = $estloop['sub'];
	} while ($estloop['atu']);
	
	for($i=(count($estor)-1);$i >= 0;$i--) {
		$estruturanomeordenada[] = "<b>".$estor[$i]['regdescricao']."</b>";
		
		if($estor[$i]['rgavisao'] == $_REQUEST['detalhes']) {
			
			if($estor[$i]['rgafiltroreg']) {
				$filtro1 .= str_replace(array("{".$estor[$i]['rgaidentificador']."}"),array($_REQUEST[$estor[$i]['rgaidentificador']]),$estor[$i]['rgafiltroreg']);
			} else {
				$filtro1 .= str_replace(array("{".$estor[$i]['rgaidentificador']."}"),array($_REQUEST[$estor[$i]['rgaidentificador']]),$estor[$i]['rgafiltro']);
			}
			
			$sql = str_replace(array("{".$estor[$i]['rgaidentificador']."}", "{clausulaindicador}","{clausulaacao}", "{clausulasecretaria}","{ano}"),array($_REQUEST[$estor[$i]['rgaidentificador']], (($_REQUEST['indid'])?"AND ind.indid!='".$_REQUEST['indid']."'":""), (($_REQUEST['acaid'])?"AND aca.acaid='".$_REQUEST['acaid']."'":""), (($_REQUEST['secid'])?"AND ind.secid='".$_REQUEST['secid']."'":""), date("Y")), $estor[$i]['rgasqlindicadores']);
			$dadosreg = $db->pegaLinha(str_replace(array("{".$estor[$i]['rgaidentificador']."}"),array($_REQUEST[$estor[$i]['rgaidentificador']]),$estor[$i]['regsql']));
			$icones = str_replace(array("{ano}","{estuf}","{municipiocod}","{entnumcpfcnpj}","{estcod}","{anos}","{mundescricao}","{muncod}","{muncodcompleto}","{unicod}","{entid}","{estdescricao}","{entcodent}"),array(date("Y"),$dadosreg['estuf'],substr($dadosreg['muncod'],0,6),$dadosreg['entnumcpfcnpj'],$dadosreg['estcod'],(date("Y")-1),$dadosreg['mundescricao'],$dadosreg['muncod'],$dadosreg['muncodcompleto'],$dadosreg['unicod'],$dadosreg['entid'],$dadosreg['estdescricao'], $dadosreg['entcodent']),stripslashes($estor[$i]['regicones']));
		}
	}
	
	$html .= "<tr>
				<td rowspan='4' align='center' width='140'><img src=\"../painel/images/".$dadosi['regid'].".gif\"></td>
				<td>".implode(" >> ", $estruturanomeordenada)."</td></tr>";
	$html .= "<tr><td style=\"font-size:12px;\"><b>".str_replace(array("{indid}"),array($_REQUEST['indid']),$dadosreg['descricao'])."</b></td></tr>";
	$html .= "<tr><td>".$icones."</td></tr>";
	$html .= "</table>";
	
} else {
	
	$qry = "SELECT * FROM painel.regagreg rga LEFT JOIN painel.regionalizacao reg ON reg.regid=rga.regid WHERE rgavisao='".$_REQUEST['detalhes']."'";
	$rga = $db->pegaLinha($qry);
	
	$sql = str_replace(array("{".$rga['rgaidentificador']."}", "{clausulaindicador}","{clausulaacao}", "{clausulasecretaria}", "{ano}"),array($_REQUEST[$rga['rgaidentificador']], (($_REQUEST['indid'])?"AND ind.indid!='".$_REQUEST['indid']."'":""), (($_REQUEST['acaid'])?"AND aca.acaid='".$_REQUEST['acaid']."'":""), (($_REQUEST['secid'])?"AND ind.secid='".$_REQUEST['secid']."'":""), date("Y")),$rga['rgasqlindicadores']);
	
	$dadosreg = $db->pegaLinha(str_replace(array("{".$rga['rgaidentificador']."}"),array($_REQUEST[$rga['rgaidentificador']]),$rga['regsql']));
	
	if($rga['rgafiltroreg']) {
		$filtro1 .= str_replace(array("{".$rga['rgaidentificador']."}"),array($_REQUEST[$rga['rgaidentificador']]),$rga['rgafiltroreg']);
	} else {
		$filtro1 .= str_replace(array("{".$rga['rgaidentificador']."}"),array($_REQUEST[$rga['rgaidentificador']]),$rga['rgafiltro']);
	}
	
	$estrutura = getEstruturaRegionalizacao($rga['regid']);

	$icones = str_replace(array("{ano}","{estuf}","{municipiocod}","{entnumcpfcnpj}","{estcod}","{anos}","{mundescricao}","{muncod}","{muncodcompleto}","{unicod}","{entid}","{estdescricao}", "{entcodent}"),array(date("Y"),$dadosreg['estuf'],substr($dadosreg['muncod'],0,6),$dadosreg['entnumcpfcnpj'],$dadosreg['estcod'],(date("Y")-1),$dadosreg['mundescricao'],$dadosreg['muncod'],$dadosreg['muncodcompleto'],$dadosreg['unicod'],$dadosreg['entid'],$dadosreg['estdescricao'], $dadosreg['entcodent']),stripslashes($rga['regicones']));
	
	$estloop = $estrutura;
	
	do {
		$estor[] = $estloop['atu'];
		$estloop = $estloop['sub'];
	} while ($estloop['atu']);

	for($i=(count($estor)-1);$i >= 0;$i--) {
		$estruturanomeordenada[] = "<b>".$estor[$i]['regdescricao']."</b>";
	}
	
	$html .= "<table cellSpacing=0 cellPadding=3 style=\"width:100%;background-color:#FCFCFC;\">";
	$html .= "<tr>
				<td rowspan='3' align='center' width='140'><img src=\"../painel/images/".$rga['regid'].".gif\"></td>
				<td>".implode(" >> ", $estruturanomeordenada)."</td></tr>";
	$html .= "<tr><td style=\"font-size:12px;\"><b>".str_replace(array("{indid}"),array($_REQUEST['indid']),$dadosreg['descricao'])."</b></td></tr>";
	$html .= "<tr><td>".$icones."</td></tr>";
	$html .= "</table>";
	
}

$inds = $db->carregar($sql);

// agrupando indices por eixo
if($inds[0]) {
	foreach($inds as $ind) {
		if($ind['indid'] != $_REQUEST['indid'])
			$arrIndAgrup[$ind['acaid']][] = array("indid" => $ind['indid']);
			$arrAcaInfo[$ind['acaid']] = $ind['acadsc'];
	}
}

// processando estrutura
$html .= "<table cellSpacing=0 cellPadding=3 class=listagem style=\"width:100%;color:#888888;\" id=\"tabela\">";

$html .= "<tr><td class=\"SubTituloEsquerda\" colspan=9><a style=\"cursor:pointer;\" onclick=\"controleAcoes('mais');\">Abrir todos</a> | <a style=\"cursor:pointer;\" onclick=\"controleAcoes('menos');\">Fechar todos</a></td></tr>";

if($_REQUEST['indid']) {
	
	$sql = "SELECT indid, unmid, foo.regid, indcumulativo, indcumulativovalor, indnome, sum(qtde) as qtde, indqtdevalor, CASE WHEN indqtdevalor = TRUE THEN to_char(sum(valor), '999g999g999g999d99') ELSE '-' END as valor, secdsc, umedesc, regdescricao from(
				SELECT d.indid, d.unmid, d.indnome, d.secid, d.umeid, d.regid, d.indcumulativo, d.indcumulativovalor, d.indqtdevalor, CASE WHEN d.indcumulativo='S' THEN sum(d.qtde)
					WHEN d.indcumulativo='N' THEN
						CASE WHEN d.sehstatus='A' THEN sum(d.qtde)
						ELSE 0 END
					WHEN d.indcumulativo='A' THEN
						CASE when d.dpeanoref=( SELECT dd.dpeanoref FROM painel.seriehistorica ss 
									   INNER JOIN painel.detalheperiodicidade dd ON dd.dpeid=ss.dpeid 
									   WHERE ss.indid = d.indid AND ss.sehstatus='A') THEN sum(d.qtde)
						ELSE 0 END
					END as qtde,
				CASE 	WHEN d.indcumulativovalor='S' THEN sum(d.valor)
					WHEN d.indcumulativovalor='N' THEN
						CASE when d.sehstatus='A' THEN sum(d.valor)
						ELSE 0 END
					WHEN d.indcumulativovalor='A' then
						CASE when d.dpeanoref=( SELECT dd.dpeanoref FROM painel.seriehistorica ss 
									   INNER JOIN painel.detalheperiodicidade dd ON dd.dpeid=ss.dpeid 
									   WHERE ss.indid = d.indid AND ss.sehstatus='A') THEN sum(d.valor)
						ELSE 0 end
					END as valor
				FROM painel.v_detalheindicadorsh d 
				WHERE d.indid=".$_REQUEST['indid']."
				".$filtro1." GROUP BY d.indid,d.unmid,d.indnome,d.indcumulativo,d.indcumulativovalor,d.sehstatus,d.dpeanoref,d.secid,d.umeid,d.regid,d.indqtdevalor
				) foo 
				INNER JOIN painel.secretaria sec ON sec.secid=foo.secid 
				INNER JOIN painel.unidademeta ume ON ume.umeid=foo.umeid
				INNER JOIN painel.regionalizacao reg ON reg.regid=foo.regid 
				GROUP BY indid, foo.unmid, indnome, secdsc, umedesc, regdescricao, indcumulativovalor, indcumulativo, indqtdevalor, foo.regid 
				ORDER BY indid";
	
	$indicadorP = $db->pegaLinha($sql);
	
	$html .= "<tr>";
	$html .= "<td style=\"width:100%;text-align:center;font-weight:bold;background-color:#DBDBDB;font-size:14px;color: rgb(0, 85, 0);\" colspan=9 >";
	$html .= $indicadorP['acadsc'];
	$html .= "</td></tr>";
	
	$html .= "<tr>";
	$html .= "<td>";
	
	
	$html .= "<table cellSpacing=0 cellPadding=3 class=listagem style=\"width:100%;color:#888888;\">";
	
	$html .= "<tr>";
	$html .= "<td class=\"SubTituloCentro\">&nbsp;</td>";
	$html .= "<td class=\"SubTituloCentro\">Cod</td>";
	$html .= "<td class=\"SubTituloCentro\">Nome do indicador</td>";
	$html .= "<td class=\"SubTituloCentro\">Secretaria</td>";
	$html .= "<td class=\"SubTituloCentro\">Regionalização</td>";
	$html .= "<td class=\"SubTituloCentro\">Produto</td>";
	$html .= "<td class=\"SubTituloCentro\">Qtde</td>";
	$html .= "<td class=\"SubTituloCentro\">R$</td>";
	$html .= "</tr>";

	if($_REQUEST['detalhes'])
		$rgaidentificador = $db->pegaUm("select rgaidentificador from painel.regagreg where rgavisao = '".$_REQUEST['detalhes']."'");
	
	$html .= processarLinhaDetalhamentoIndicadores($indicadorP, array('detalhes' => $_REQUEST['detalhes'], "rgaidentificador" => $rgaidentificador), true);
	
	$html .= "</table>";
	
	$html .= "</td>";
	$html .= "</tr>";
	
	$html .= "<tr>";
	$html .= "<td class=\"SubTituloEsquerda\">Acesse outros indicadores...</td>";
	$html .= "</tr>";
	

}

if($arrAcaInfo) {
	foreach($arrAcaInfo as $acaid => $acadsc) {
		$html .= "<tr id=\"tr_m_".$acaid."\">";
		$html .= "<td style=\"width:100%;text-align:left;font-weight:bold;background-color:#F3F3F3;font-size:14px;color: rgb(0, 85, 0);\" colspan=8 >";
		$html .= "<img src=\"../imagens/mais.gif\" style=\"cursor:pointer;\" title=\"mais\" id=\"imgc_".$acaid."\" onclick=\"carregarAcao('".$acaid."', '".md5_encrypt(serialize($arrIndAgrup[$acaid]))."', this);\"> ";
		$html .= "<span style=\"cursor:pointer;\" onclick=\"document.getElementById('imgc_".$acaid."').onclick();\">".$acadsc."</span>";
		$html .= "</td></tr>";
	}
} else {
	$html .= "<tr>";
	$html .= "<td align=\"center\" colspan=\"9\">Não existem indicadores.</td>";
	$html .= "</tr>";
}

$html .= "</table>";

if(count($indagrup) === 1) {
	$html .= "<script>controleAcoes('mais');</script>";
}

echo $html;

?>
<script>self.focus();</script>
</body>
</html>