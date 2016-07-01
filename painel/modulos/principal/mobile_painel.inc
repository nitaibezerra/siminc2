<?
function detalhamentoIndicador_Mobile($dados) {
	global $db;
	
	if($dados['indid']) {
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
				
				$sql = str_replace(array("{".$estor[$i]['rgaidentificador']."}", "{clausulaindicador}","{clausulaacao}", "{clausulasecretaria}", "{clausulametapne}","{ano}"),array($_REQUEST[$estor[$i]['rgaidentificador']], (($_REQUEST['indid'])?"AND ind.indid!='".$_REQUEST['indid']."'":""), (($_REQUEST['acaid'])?"AND aca.acaid='".$_REQUEST['acaid']."'":""), (($_REQUEST['secid'])?"AND ind.secid='".$_REQUEST['secid']."'":""), (($_REQUEST['mpneid'])?"AND ind.indid IN (".$indpne.")":""), date("Y")), $estor[$i]['rgasqlindicadores']);
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
		
		$qry = "SELECT * FROM painel.regagreg rga LEFT JOIN painel.regionalizacao reg ON reg.regid=rga.regid WHERE rgavisao='".$dados['detalhes']."'";
		$rga = $db->pegaLinha($qry);
		
		$sql = str_replace(array("{".$rga['rgaidentificador']."}", "{clausulaindicador}","{clausulaacao}", "{clausulasecretaria}", "{clausulametapne}", "{ano}"),array($dados[$rga['rgaidentificador']], (($dados['indid'])?"AND ind.indid!='".$dados['indid']."'":""), (($dados['acaid'])?"AND aca.acaid='".$dados['acaid']."'":""), (($dados['secid'])?"AND ind.secid='".$dados['secid']."'":""), (($_REQUEST['mpneid'])?"AND ind.indid IN (".$indpne.")":""), date("Y")),$rga['rgasqlindicadores']);
		
		$dadosreg = $db->pegaLinha(str_replace(array("{".$rga['rgaidentificador']."}"),array($dados[$rga['rgaidentificador']]),$rga['regsql']));
		
		if($rga['rgafiltroreg']) {
			$filtro1 .= str_replace(array("{".$rga['rgaidentificador']."}"),array($dados[$rga['rgaidentificador']]),$rga['rgafiltroreg']);
		} else {
			$filtro1 .= str_replace(array("{".$rga['rgaidentificador']."}"),array($dados[$rga['rgaidentificador']]),$rga['rgafiltro']);
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
			if($ind['indid'] != $dados['indid'])
				$arrIndAgrup[$ind['acaid']][] = array("indid" => $ind['indid']);
				$arrAcaInfo[$ind['acaid']] = $ind['acadsc'];
		}
	}
	
	// processando estrutura
	$html .= "<table cellSpacing=0 cellPadding=3 class=listagem style=\"width:100%;color:#888888;\" id=\"tabela\">";
	
	$html .= "<tr><td class=\"SubTituloEsquerda\" colspan=9><a style=\"cursor:pointer;\" onclick=\"controleAcoes('mais');\">Abrir todos</a> | <a style=\"cursor:pointer;\" onclick=\"controleAcoes('menos');\">Fechar todos</a></td></tr>";
	
	if($dados['indid']) {
		
		$sql = "SELECT indid, acaid, unmid, foo.regid, indcumulativo, indcumulativovalor, indnome, sum(qtde) as qtde, indqtdevalor, CASE WHEN indqtdevalor = TRUE THEN to_char(sum(valor), '999g999g999g999d99') ELSE '-' END as valor, secdsc, umedesc, regdescricao from(
					SELECT d.indid, acaid, d.unmid, d.indnome, d.secid, d.umeid, d.regid, d.indcumulativo, d.indcumulativovalor, d.indqtdevalor, CASE WHEN d.indcumulativo='S' THEN sum(d.qtde)
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
					WHERE d.indid=".$dados['indid']."
					".$filtro1." GROUP BY d.indid, acaid, d.unmid,d.indnome,d.indcumulativo,d.indcumulativovalor,d.sehstatus,d.dpeanoref,d.secid,d.umeid,d.regid,d.indqtdevalor
					) foo 
					INNER JOIN painel.secretaria sec ON sec.secid=foo.secid 
					INNER JOIN painel.unidademeta ume ON ume.umeid=foo.umeid
					INNER JOIN painel.regionalizacao reg ON reg.regid=foo.regid 
					GROUP BY indid, acaid, foo.unmid, indnome, secdsc, umedesc, regdescricao, indcumulativovalor, indcumulativo, indqtdevalor, foo.regid 
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
	
		if($dados['detalhes'])
			$rgaidentificador = $db->pegaUm("select rgaidentificador from painel.regagreg where rgavisao = '".$dados['detalhes']."'");
		
		$html .= processarLinhaDetalhamentoIndicadores($indicadorP, array('detalhes' => $dados['detalhes'], "rgaidentificador" => $rgaidentificador), true);
		
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
	
	
}


function exibeMapaRegionalizador_Mobile($dados) {
	global $db;
	
	$sql = "select
				cxpid,
				cxpsql2 as sql,
				cxpcabecalho,
				(CASE WHEN cxp.regid is not null
					THEN
					(select regunidade from painel.regionalizacao r where cxp.regid = r.regid)
				 ELSE
				 	cxpunidade
				 END) as cxpunidade,
				cxpicone
			from
				painel.caixapesquisa cxp
			where
				cxpstatus = 'A'
			and
				cxpid = '".$dados['cxpid']."'";

	$arrDados = $db->pegaLinha($sql);
	
	?>
	<script>

	function filtrar(letra,busca) {
		$.mobile.changePage( "painel.php?modulo=principal/mobile_painel&acao=A", {
			type: "post",
			data: "busca="+busca+"&requisicao=exibeMapaRegionalizador_Mobile&cxpid=<?=$dados['cxpid'] ?>&letra="+letra
		});
	}
	</script>
	<div data-role="header" role="banner"><h1 class="ui-title" role="heading"><?=$arrDados['cxpunidade']; ?></h1></div>
	<br />
	<input type="search" name="busca_" id="busca_" value="<?=$dados['busca'] ?>" /><button type="button" data-theme="a" onclick="filtrar('',$('#busca_').val());">Buscar</button>
	<? $arAlfabeto = array( "A", "B", "C", "D", "E", "F", "G", "H", "I", "J", "K", "L", "M", "N", "O", "P", "Q", "R", "S", "T", "U", "V", "W", "X", "Y", "Z", "Todos" ); ?>
	<? if($dados['letra']=="") $dados['letra']="Todos"; ?>
	<div data-role="fieldcontain">
    <fieldset data-role="controlgroup" data-type="horizontal" data-mini="true">
	<? foreach($arAlfabeto as $key => $letra) : ?>
   	<input type="radio" name="letra" id="radio-choice-<?=$letra ?>" value="<?=$letra ?>" onclick="filtrar('<?=$letra ?>','');" <?=(($dados['letra']==$letra)?"checked":"") ?> />
   	<label for="radio-choice-<?=$letra ?>" style="font-size:8px;"><?=$letra ?></label>	
	<? endforeach; ?>
	</fieldset>
	<?
	if($dados['letra']=="Todos") $dados['letra']="";
	$sql = str_replace( array("{busca}","{letra}","{selectregiao}") , array($dados['busca'],$dados['letra'],"select estuf from territorios.estado") ,$arrDados['sql'] );
	$dados_ = $db->carregar($sql);
	?>
	
	<? if($dados_[0]) : ?>
	<ul data-role="listview" data-inset="true">
	<? foreach($dados_ as $dado) : ?>
	<li><a href="index.html" class="ui-link-inherit"><?=$dado['descricao'] ?></a></li>
	<? endforeach; ?>
	</ul>
	<? else : ?>
	<p align="center"><b>Não existem registros</b></p>
	<? endif; ?>
	
	<?
	if(count($dados_) >= 50){
		echo "<center><span style=\"color:#990000\" >Foram encontrados mais de 50 registros, favor refinar sua busca.</span></center>";
	}
	?>
	</div>

	<?
	
}


function exibeResultadoBusca_Mobile($dados) {
	global $db;
	
	$sql = "select
				cxpid,
				cxpsql1 as sql,
				cxpsql2 as sql2,
				cxpcabecalho,
				(CASE WHEN cxp.regid is not null
					THEN
					(select regunidade from painel.regionalizacao r where cxp.regid = r.regid)
				 ELSE
				 	cxpunidade
				 END) as cxpunidade,
				cxpicone,
				regid
			from
				painel.caixapesquisa cxp
			where
				cxpstatus = 'A'
			order by
				cxpordem";
	
	$arrDados = $db->carregar($sql);
	
	if($dados['busca']) : 
	?>
	<? if($arrDados[0]) : ?>
		<? foreach($arrDados as $dado) : ?>
			<?
			$sql = str_replace( array("{busca}","{letra}","{selectregiao}") , array(iconv("UTF-8", "ISO-8859-1", $dados['busca']),$letraInicial,$whereRegiao) ,$dado['sql'] );
			$dados_ = $db->carregar($sql);
			if($dados_[0]) :
			?>
			<div data-role="collapsible" data-content-theme="c" data-collapsed=false>
			   <h3><?=$dado['cxpunidade'] ?></h3>
			   <ul data-role="listview" data-inset="true">
			   <? foreach($dados_ as $dado_) : ?>
			   <li>
			   <?
				switch($dado['regid']){
					case REGIONALIZACAO_ESCOLA:
						break;
					case REGIONALIZACAO_POLO:
						break;
					case REGIONALIZACAO_IES:
						break;
					case REGIONALIZACAO_POSGRADUACAO:
						break;
					case REGIONALIZACAO_CAMPUS_SUPERIOR:
						break;
					case REGIONALIZACAO_HOSPITAL:
						break;
					case REGIONALIZACAO_CAMPUS_PROFISSIONAL:
						$href="painel.php?modulo=principal/mobile_painel&acao=A&requisicao=detalhamentoIndicador_Mobile&detalhes=campusprofissional&entid=".$dado_['codigo'];
						break;
					case REGIONALIZACAO_UNIVERSIDADE:
						break;
					case REGIONALIZACAO_INSTITUTO:
						break;
					default:
				}
			   ?>
				<div class="ui-grid-b">
					<div class="ui-block-a" style="width:10%;"><?=$dado_['codigo'] ?></div>
					<div class="ui-block-b" style="width:70%;"><?=$dado_['descricao'] ?></div>
					<div class="ui-block-c" style="width:20%;"><?=$dado_['municipio'] ?></div>
				</div>
			   </li>
			   <? endforeach; ?>
			   </ul>
			</div>
			<?
			endif;
			?>
		<? endforeach; ?>
	<? endif; ?>
	<? else : ?>
	<ul data-role="listview" data-inset="true">
	<? if($arrDados[0]) : ?>
		<? foreach($arrDados as $dado) : ?>
			<li><a href="painel.php?modulo=principal/mobile_painel&acao=A&requisicao=exibeMapaRegionalizador_Mobile&cxpid=<?=$dado['cxpid'] ?>">
			<h1><?=$dado['cxpunidade'] ?></h1>
			</a></li>
		<? endforeach; ?>
	<? endif; ?>
	</ul>
	<?
	endif;

}


function exibePaginaInicial_Mobile($dados) {
	?>
	<form method="post" action="painel.php?modulo=principal/mobile_painel&acao=A">
	<input type="hidden" name="requisicao" value="exibePaginaInicial_Mobile">	
	<input type="search" value="<?=iconv("UTF-8", "ISO-8859-1", $dados['busca']) ?>" id="busca" name="busca" />
	<div class="ui-grid-a">
		<div class="ui-block-a"><button data-theme="a" type="submit" class="ui-btn-hidden" aria-disabled="false">Buscar</button></div>
		<div class="ui-block-b"><a href="painel.php?modulo=principal/mobile_painel&acao=A&requisicao=exibePaginaInicial_Mobile" data-role="button">Limpar</a></div>
	</div>
	</form>
	<br />
	<? 
	exibeResultadoBusca_Mobile($dados);
}

?>
<!DOCTYPE html> 
<html> 
	<head> 
	<title>SIMEC - Mobile</title> 
	<meta name="viewport" content="width=device-width, initial-scale=1"> 
	<link rel="stylesheet" href="/includes/jquery.mobile-1.0.1/jquery.mobile-1.0.1.min.css" />
	<script src="/includes/jquery.mobile-1.0.1/jquery-1.7.1.min.js"></script>	
	<script src="/includes/jquery.mobile-1.0.1/jquery.mobile-1.0.1.min.js"></script>
	<link type="text/css" href="http://dev.jtsage.com/cdn/datebox/latest/jquery.mobile.datebox.min.css" rel="stylesheet" />
	<script type="text/javascript" src="http://dev.jtsage.com/jquery.mousewheel.min.js"></script>
	<script type="text/javascript" src="http://dev.jtsage.com/cdn/datebox/latest/jquery.mobile.datebox.min.js"></script>
	<script>
function abrasil() {
	window.open('painel.php?modulo=principal/detalhamentoIndicador&acao=A&detalhes=pais&indid=&paiid=1','Indicador','scrollbars=yes,height=700,width=840,status=no,toolbar=no,menubar=no,location=no');
	void(0);
}
function amun(muncod) {
	window.open('painel.php?modulo=principal/detalhamentoIndicador&acao=A&detalhes=municipio&muncod='+muncod,'Indicador','scrollbars=yes,height=700,width=840,status=no,toolbar=no,menubar=no,location=no');
	void(0);
}
function aest(estuf) {
	window.open('painel.php?modulo=principal/detalhamentoIndicador&acao=A&detalhes=estado&estuf='+estuf,'Indicador','scrollbars=yes,height=700,width=840,status=no,toolbar=no,menubar=no,location=no');
	void(0);
}
function aesc(esccodinep) {
	window.open('painel.php?modulo=principal/detalhamentoIndicador&acao=A&detalhes=escola&esccodinep='+esccodinep,'Indicador','scrollbars=yes,height=700,width=840,status=no,toolbar=no,menubar=no,location=no');
	void(0);
}
function aies(iesid) {
	window.open('painel.php?modulo=principal/detalhamentoIndicador&acao=A&detalhes=ies&iesid='+iesid,'Indicador','scrollbars=yes,height=700,width=840,status=no,toolbar=no,menubar=no,location=no');
	void(0);
}
function auni(unicod){
	window.open('painel.php?modulo=principal/detalhamentoIndicador&acao=A&detalhes=universidade&unicod='+unicod,'Indicador','scrollbars=yes,height=700,width=840,status=no,toolbar=no,menubar=no,location=no');
	void(0);
}
function ains(unicod){
	window.open('painel.php?modulo=principal/detalhamentoIndicador&acao=A&detalhes=instituto&unicod='+unicod,'Indicador','scrollbars=yes,height=700,width=840,status=no,toolbar=no,menubar=no,location=no');
	void(0);
}
function aiep(iepid){
	window.open('painel.php?modulo=principal/detalhamentoIndicador&acao=A&detalhes=posgraduacao&iepid='+iepid,'Indicador','scrollbars=yes,height=700,width=840,status=no,toolbar=no,menubar=no,location=no');
	void(0);
}

function acampusprof(entid){
	$.mobile.changePage("painel.php?modulo=principal/mobile_painel&acao=A", {
		type: "post",
		data: "&detalhes=campusprofissional&entid="+entid
	});
}

function acampussup(entid){
	window.open('painel.php?modulo=principal/detalhamentoIndicador&acao=A&detalhes=campussuperior&entid='+entid,'Indicador','scrollbars=yes,height=700,width=840,status=no,toolbar=no,menubar=no,location=no');
	void(0);
}
function ahosp(entid){
	window.open('painel.php?modulo=principal/detalhamentoIndicador&acao=A&detalhes=hospital&entid='+entid,'Indicador','scrollbars=yes,height=700,width=840,status=no,toolbar=no,menubar=no,location=no');
	void(0);
}
function apol(polid){
	window.open('painel.php?modulo=principal/detalhamentoIndicador&acao=A&detalhes=polo&polid='+polid,'Indicador','scrollbars=yes,height=700,width=840,status=no,toolbar=no,menubar=no,location=no');
	void(0);
}
function atip(tpmid){
	window.open('painel.php?modulo=principal/detalhamentoIndicador&acao=A&detalhes=tipomunicipio&tpmid='+tpmid,'Indicador','scrollbars=yes,height=700,width=840,status=no,toolbar=no,menubar=no,location=no');
	void(0);
}
function agrp(gtmid){
	window.open('painel.php?modulo=principal/detalhamentoIndicador&acao=A&detalhes=grupotipomunicipio&gtmid='+gtmid,'Indicador','scrollbars=yes,height=700,width=840,status=no,toolbar=no,menubar=no,location=no');
	void(0);
}
function amicro(miccod){
	window.open('painel.php?modulo=principal/detalhamentoIndicador&acao=A&detalhes=microregiao&miccod='+miccod,'Indicador','scrollbars=yes,height=700,width=840,status=no,toolbar=no,menubar=no,location=no');
	void(0);
}
	</script>
</head> 
<body>
<div data-role="page">
	<div data-role="header" data-position="fixed">
		<div data-role="controlgroup" data-type="horizontal">
			<a href="painel.php?modulo=principal/mobile_painel&acao=A" data-role="button" data-icon="home" class="inicio-rodape">Busca</a>
		</div>
	</div>
 
	<div data-role="content">
	<? $_REQUEST['requisicao'] ? $_REQUEST['requisicao']($_REQUEST) : exibePaginaInicial_Mobile(array()); ?>
	</div>
	<?php if($_GET['titulo_pagina']): ?>
		<div data-role="footer" data-theme="c" data-position="fixed" >
			<div data-role="controlgroup" data-type="horizontal">
				<a href="javascript:history.back(-1)" data-role="button" data-icon="arrow-l" data-iconpos="right">Voltar</a>
		</div>
	<? endif; ?>
</div>
</body>
</html>