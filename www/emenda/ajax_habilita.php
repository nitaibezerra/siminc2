<?php
include_once '_funcoes.php';
include_once "config.inc";
include_once APPRAIZ . "includes/classes_simec.inc";
include_once APPRAIZ . "includes/funcoes.inc";

header('content-type: text/html; charset=ISO-8859-1');

$db = new cls_banco();

/*
 * Autores
*/

if(isset($_REQUEST['habilida'])) {
	$entid = $_REQUEST['entid'];

	switch($_REQUEST['habilida']) {
		case 'docnaoApresentado': 			
			$arColunas = array(
							 array("campo" => "Descrição",
								   "width" => "50%"),
							 array("campo" => "Descrição Reduzida",
								   "width" => "30%"),
							 array("campo" => "Data da Validade",
								   "width" => "10%"),
							 array("campo" => "Data da Atualização",
								   "width" => "10%")
					 );
			monta_titulo('','<b>Dados do Documentos Habilitacao</b>');
			mostraDocumentoHabilitaNaoApresentado($entid, 'naoApresentado', $arColunas);
			break;
		case 'doccomErro' :			
			$arColunas = array(
							 array("campo" => "Descrição",
								   "width" => "40%"),
							 array("campo" => "Descrição Reduzida",
								   "width" => "20%"),
							 array("campo" => "Descrição do Erro",
								   "width" => "20%"),
							 array("campo" => "Data da Validade",
								   "width" => "10%"),
							 array("campo" => "Data da Atualização",
								   "width" => "10%")
					 );
			monta_titulo('','<b>Dados do Documentos Habilitacao</b>');
			mostraDocumentoHabilitaComErro($entid, 'documentoComErro', $arColunas);
			break;		
		case 'docsemErro' :
			$arColunas = array(
							 array("campo" => "Descrição",
								   "width" => "50%"),
							 array("campo" => "Descrição Reduzida",
								   "width" => "30%"),
							 array("campo" => "Data da Validade",
								   "width" => "10%"),
							 array("campo" => "Data da Atualização",
								   "width" => "10%")
					 );
			monta_titulo('','<b>Dados do Documentos Habilitacao</b>');
			mostraDocumentoHabilitaSemErro($entid, 'documentoSemErro', $arColunas);
			break;		
		case 'diligencia' :
		
			$arColunas = array( 
							 array("campo" => "Numero da Diligência",
								   "width" => "10%"),
							 array("campo" => "Data da Emissão",
								   "width" => "10%"),
							 array("campo" => "Responsável",
								   "width" => "30%"),
							 array("campo" => "Documento",
								   "width" => "20%"),
							 array("campo" => "Erro",
								   "width" => "20%"),
							 array("campo" => "Data da Atualização",
								   "width" => "10%")
					 );
			diligencia($entid, $arColunas);
			break;		
	}
}

function mostraDocumentoHabilitaNaoApresentado($entid, $tipo, $arColunas){
global $db;
	
	$sql = "SELECT 
				hdo.hdocnpj,
				hdo.hdodsc,
				hdo.hdodescricaoerro,
				hdodscreduzido,
				hdo.hdopossuivalidade,
	            hdo.hdosituacao,
				to_char(hdo.hdodatavalidade,'DD/MM/YYYY') as validade,
				to_char(hdo.hdodataimportacao, 'DD/MM/YYYY HH24:MI:SS') as importacao  
			FROM 
			  emenda.habilitadocumento hdo 
			  inner join entidade.entidade ent
			  	on ent.entnumcpfcnpj = hdo.hdocnpj
			WHERE
			  ent.entid = $entid 
			  AND hdo.hdosituacao = 'Documento não apresentado'";
	
	$arDadosDoc = $db->carregar($sql);
	
	echo '<center><div style="overflow: auto; height: 300px; width: 95%;">';
	if($arDadosDoc){
	?>
	<table width="100%" id="tbListDocumento" align="center" border="0" cellspacing="0" cellpadding="2" class="listagem">
	<thead>
	<tr>
		<?
		foreach ($arColunas as $valor) {
			echo '<td align="Center" class="title" width="'.$valor['width'].'"
			style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;"
			onmouseover="this.bgColor=\'#c0c0c0\';" onmouseout="this.bgColor=\'\';"><strong>'.$valor['campo'].'</strong></td>';
		}
		
		?>
	</tr>
	</thead>
	<?
		foreach ($arDadosDoc as $chave => $valor) {
			$chave % 2 ? $cor = "#f7f7f7" : $cor = "";
			?>
			<tr bgcolor="<?=$cor ?>" id="tr_<?=$chave; ?>" onmouseout="this.bgColor='<?=$cor?>';" onmouseover="this.bgColor='#ffffcc';">
				<td><?=$valor['hdodsc']; ?></td>
				<td><?=$valor['hdodscreduzido']; ?></td>
				<td><?=$valor['validade']; ?></td>
				<td><?=$valor['importacao']; ?></td>
			</tr>
			<?
		}
	?>
	</table>
	<?
		echo '</div></center>';
		print "<table width='95%' align='center' border='0' cellspacing='0' cellpadding='2' class='listagem'>";
		print "<tr bgcolor='#ffffff'>";
		print "	<td><b>Total de Registros: ".count($arDadosDoc)."</b></td>";
		print "	<td></td>";
		print "	</tr>";
		print "</table>";
	} else {
		print '<table width="100%" align="center" border="0" cellspacing="0" cellpadding="2" class="listagem">';
		print '<tr><td align="center" style="color:#cc0000;">Nenhum registro encontrado.</td></tr>';
		print '</table>';
	}
}
function mostraDocumentoHabilitaComErro($entid, $tipo, $arColunas){
	global $db;

	$sql = "SELECT 
				hdo.hdocnpj,
				hdo.hdodsc,
				hdo.hdodescricaoerro,
				hdo.hdopossuivalidade,
	            hdo.hdosituacao,
				to_char(hdo.hdodatavalidade,'DD/MM/YYYY') as validade,
				to_char(hdo.hdodataimportacao, 'DD/MM/YYYY HH24:MI:SS') as importacao  
			FROM 
			  emenda.habilitadocumento hdo 
			  inner join entidade.entidade ent
			  	on ent.entnumcpfcnpj = hdo.hdocnpj
			WHERE
			  ent.entid = $entid 
			  AND hdo.hdosituacao = 'Documento com erro'";
	
	$arDadosDoc = $db->carregar($sql);
	
	echo '<center><div style="overflow: auto; height: 300px; width: 95%;">';
	if($arDadosDoc){
	?>
	<table width="100%" id="tbListDocumento" align="center" border="0" cellspacing="0" cellpadding="2" class="listagem">
	<thead>
	<tr>
		<?
		foreach ($arColunas as $valor) {
			echo '<td align="Center" class="title" width="'.$valor['width'].'"
			style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;"
			onmouseover="this.bgColor=\'#c0c0c0\';" onmouseout="this.bgColor=\'\';"><strong>'.$valor['campo'].'</strong></td>';
		}
		
		?>
	</tr>
	</thead>
	<?
		foreach ($arDadosDoc as $chave => $valor) {
			$chave % 2 ? $cor = "#f7f7f7" : $cor = "";
			?>
			<tr bgcolor="<?=$cor ?>" id="tr_<?=$chave; ?>" onmouseout="this.bgColor='<?=$cor?>';" onmouseover="this.bgColor='#ffffcc';">
				<td><?=$valor['hdodsc']; ?></td>
				<td><?=$valor['hdodscreduzido']; ?></td>
				<td><?=$valor['hdodescricaoerro']; ?></td>
				<td><?=$valor['validade']; ?></td>
				<td><?=$valor['importacao']; ?></td>
			</tr>
			<?
		}
	?>
	</table>
	<?
		echo '</div></center>';
		print "<table width='95%' align='center' border='0' cellspacing='0' cellpadding='2' class='listagem'>";
		print "<tr bgcolor='#ffffff'>";
		print "	<td><b>Total de Registros: ".count($arDadosDoc)."</b></td>";
		print "	<td></td>";
		print "	</tr>";
		print "</table>";
	} else {
		print '<table width="100%" align="center" border="0" cellspacing="0" cellpadding="2" class="listagem">';
		print '<tr><td align="center" style="color:#cc0000;">Nenhum registro encontrado.</td></tr>';
		print '</table>';
	}
}
function mostraDocumentoHabilitaSemErro($entid, $tipo, $arColunas){
	global $db;
	
	$sql = "SELECT 
				hdo.hdocnpj,
				hdo.hdodsc,
				hdo.hdodescricaoerro,
				hdodscreduzido,
				hdo.hdopossuivalidade,
	            hdo.hdosituacao,
				to_char(hdo.hdodatavalidade,'DD/MM/YYYY') as validade,
				to_char(hdo.hdodataimportacao, 'DD/MM/YYYY HH24:MI:SS') as importacao  
			FROM 
			  emenda.habilitadocumento hdo 
			  inner join entidade.entidade ent
			  	on ent.entnumcpfcnpj = hdo.hdocnpj
			WHERE
			  ent.entid = $entid 
			  AND hdo.hdosituacao = 'Documento sem erro'";
	
	$arDadosDoc = $db->carregar($sql);
	
	echo '<center><div style="overflow: auto; height: 300px; width: 95%;">';
	if($arDadosDoc){
	?>
	<table width="100%" id="tbListDocumento" align="center" border="0" cellspacing="0" cellpadding="2" class="listagem">
	<thead>
	<tr>
		<?
		foreach ($arColunas as $valor) {
			echo '<td align="Center" class="title" width="'.$valor['width'].'"
			style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;"
			onmouseover="this.bgColor=\'#c0c0c0\';" onmouseout="this.bgColor=\'\';"><strong>'.$valor['campo'].'</strong></td>';
		}
		
		?>
	</tr>
	</thead>
	<?
		foreach ($arDadosDoc as $chave => $valor) {
			$chave % 2 ? $cor = "#f7f7f7" : $cor = "";
			?>
			<tr bgcolor="<?=$cor ?>" id="tr_<?=$chave; ?>" onmouseout="this.bgColor='<?=$cor?>';" onmouseover="this.bgColor='#ffffcc';">
				<td><?=$valor['hdodsc']; ?></td>
				<td><?=$valor['hdodscreduzido']; ?></td>
				<td><?=$valor['validade']; ?></td>
				<td><?=$valor['importacao']; ?></td>
			</tr>
			<?
		}
	?>
	</table>
	<?
		echo '</div></center>';
		print "<table width='95%' align='center' border='0' cellspacing='0' cellpadding='2' class='listagem'>";
		print "<tr bgcolor='#ffffff'>";
		print "	<td><b>Total de Registros: ".count($arDadosDoc)."</b></td>";
		print "	<td></td>";
		print "	</tr>";
		print "</table>";
	} else {
		print '<table width="100%" align="center" border="0" cellspacing="0" cellpadding="2" class="listagem">';
		print '<tr><td align="center" style="color:#cc0000;">Nenhum registro encontrado.</td></tr>';
		print '</table>';
	}
}

function diligencia($entid, $arColunas){
	global $db;
	monta_titulo('','<b>Dados do Diligência</b>');
	
	$sql = "SELECT 
			  hdi.hdicnpj,
			  hdi.hdicod,
			  hdi.hdidescricao,
			  hdi.hdiusunome,
			  hdi.hdodsc,
			  hdi.hdidescricaoerro,
			  to_char(hdi.hdidataemissao, 'DD/MM/YYYY') as emissao,
			  to_char(hdi.hdidataimportacao, 'DD/MM/YYYY HH24:MI:SS') as importacao
			FROM 
			  emenda.habilitadiligencia hdi
			  inner join entidade.entidade ent
			  	on ent.entnumcpfcnpj = hdi.hdicnpj
			WHERE
			  ent.entid = $entid";
			  
	$arDadosDil = $db->carregar($sql);
	
	echo '<center><div style="overflow: auto; height: 300px; width: 95%;">';
	if($arDadosDil){
	?>
	<table width="100%" id="tbListaDiligencia" align="center" border="0" cellspacing="0" cellpadding="2" class="listagem">
	<thead>
	<tr>
		<?
		foreach ($arColunas as $valor) {
			echo '<td align="Center" class="title" width="'.$valor['width'].'"
			style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;"
			onmouseover="this.bgColor=\'#c0c0c0\';" onmouseout="this.bgColor=\'\';"><strong>'.$valor['campo'].'</strong></td>';
		}
		?>
	</tr>
	</thead>
	<?
		foreach ($arDadosDil as $chave => $valor) {
			$chave % 2 ? $cor = "#f7f7f7" : $cor = "";
			?>
			<tr bgcolor="<?=$cor ?>" id="tr_<?=$chave; ?>" onmouseout="this.bgColor='<?=$cor?>';" onmouseover="this.bgColor='#ffffcc';">
				<td style="text-align: center; color: rgb(0, 102, 204);"><?=$valor['hdicod']; ?></td>
				<td><?=$valor['emissao']; ?></td>
				<td><?=$valor['hdiusunome']; ?></td>
				<td><?=$valor['hdodsc']; ?></td>
				<td><?=$valor['hdidescricaoerro']; ?></td>
				<td><?=$valor['importacao']; ?></td>
			</tr>
			<?
		}
	?>
	</table>
	<?
		echo '</div></center>';
		print "<table width='95%' align='center' border='0' cellspacing='0' cellpadding='2' class='listagem'>";
		print "<tr bgcolor='#ffffff'>";
		print "	<td><b>Total de Registros: ".count($arDadosDil)."</b></td>";
		print "	<td></td>";
		print "	</tr>";
		print "</table>";
	}else{
		print '<table width="100%" align="center" border="0" cellspacing="0" cellpadding="2" class="listagem">';
		print '<tr><td align="center" style="color:#cc0000;">Nenhum registro encontrado.</td></tr>';
		print '</table>';	
	}
}

?>