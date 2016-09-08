<?php

/*** Seta mais memória para carregar a página e sem tempo limite ***/
ini_set( "memory_limit", "512M" );
set_time_limit(0);

/*** Recupera o perfil do usuário ***/
$usuPerfil = arrayPerfil();

/*** Se tiver perfil administrativo ***/
if( in_array(PDEESC_PERFIL_ADMINISTRADOR_ESCOLA_ACESSIVEL, $usuPerfil) || in_array(PDEESC_PERFIL_CONSULTA_ESCOLA_ACESSIVEL, $usuPerfil) || in_array(PDEESC_PERFIL_SUPER_USUARIO, $usuPerfil) )
{
	if( isset($_REQUEST["muncod"]) && $_REQUEST["muncod"] != "" )
	{
		$funid = 1;
		
		$whereEntExecutora = "en.muncod = '".$_REQUEST["muncod"]."'";
		
		$whereEntDirigente = "funprefeito.funid = 2 AND 
				              funprefeitura.funid = 1 AND 
				              mun.muncod ='".$_REQUEST["muncod"]."'";
	}
	if( isset($_REQUEST["estuf"]) && $_REQUEST["estuf"] != "" )
	{
		$funid = 6;
		
		$whereEntExecutora = "en.estuf = '".$_REQUEST["estuf"]."'";
		
		$whereEntDirigente = "funprefeito.funid = 25 AND 
	            			  funprefeitura.funid = 6 AND 
			            	  mun.estuf ='".$_REQUEST["estuf"]."'";
	}
	
	$sql = "SELECT 
				ent.entnumcpfcnpj, 
				ent.entnome,
				mun.mundescricao,
				mun.estuf
			FROM 
				entidade.entidade ent
			INNER JOIN 
				entidade.endereco en ON en.entid = ent.entid
			INNER JOIN
				entidade.funcaoentidade fue ON fue.entid = ent.entid AND
											   fue.funid = ".$funid."
			INNER JOIN
				territorios.municipio mun ON mun.muncod = en.muncod
			WHERE
				".$whereEntExecutora;
	$entidadeExecutora = $db->carregar($sql);
	
	$sql = "SELECT 
				entprefeito.entnumcpfcnpj,
				entprefeito.entnome
            FROM 
            	entidade.entidade entprefeito
           	INNER JOIN 
           		entidade.funcaoentidade funprefeito ON entprefeito.entid = funprefeito.entid
            INNER JOIN 
            	entidade.funentassoc feaprefeito ON feaprefeito.fueid = funprefeito.fueid
            INNER JOIN 
            	entidade.entidade entprefeitura ON entprefeitura.entid = feaprefeito.entid
            INNER JOIN 
            	entidade.funcaoentidade funprefeitura ON funprefeitura.entid = entprefeitura.entid
            INNER JOIN 
            	entidade.endereco entd ON entd.entid = entprefeitura.entid
            INNER JOIN 
            	territorios.municipio mun ON entd.muncod = mun.muncod
            WHERE 
            	".$whereEntDirigente."
			AND
            	entprefeito.entstatus = 'A' AND
            	entprefeitura.entstatus = 'A'";
	$entidadeDirigente = $db->carregar($sql);
	
	/*** Recupera as escolas do lote ***/
	$escolas = $db->carregarColuna("SELECT eacid FROM pdeescola.eacloteimpressao WHERE elicodigolote = " . $_GET["lote"]);
}
/*** Se for perfil Estadual ***/
else if( in_array(PDEESC_PERFIL_SEC_ESTADUAL_ESCOLA_ACESSIVEL, $usuPerfil) )
{
	/*** Recupera o estado associado ao usuário ***/
	$estuf = $db->pegaUm("SELECT estuf FROM pdeescola.usuarioresponsabilidade WHERE usucpf = '".$_SESSION["usucpf"]."' AND rpustatus = 'A'");
	
	/*** Recupera a entidade executora ***/
	$sql = "SELECT 
				ent.entnumcpfcnpj, 
				ent.entnome,
				mun.mundescricao,
				mun.estuf
			FROM 
				entidade.entidade ent
			INNER JOIN 
				entidade.endereco en ON en.entid = ent.entid
			INNER JOIN
				entidade.funcaoentidade fue ON fue.entid = ent.entid AND
											   fue.funid = 6
			INNER JOIN
				territorios.municipio mun ON mun.muncod = en.muncod
			WHERE
				en.estuf = '".$estuf."'";
	$entidadeExecutora = $db->carregar($sql);
	
	/*** Recupera a entidade dirigente ***/
	$sql = "SELECT 
				entprefeito.entnumcpfcnpj,
				entprefeito.entnome
            FROM 
            	entidade.entidade entprefeito
           	INNER JOIN 
           		entidade.funcaoentidade funprefeito ON entprefeito.entid = funprefeito.entid
            INNER JOIN 
            	entidade.funentassoc feaprefeito ON feaprefeito.fueid = funprefeito.fueid
            INNER JOIN 
            	entidade.entidade entprefeitura ON entprefeitura.entid = feaprefeito.entid
            INNER JOIN 
            	entidade.funcaoentidade funprefeitura ON funprefeitura.entid = entprefeitura.entid
            INNER JOIN 
            	entidade.endereco entd ON entd.entid = entprefeitura.entid
            INNER JOIN 
            	territorios.municipio mun ON entd.muncod = mun.muncod
            WHERE 
            	funprefeito.funid = 25 AND 
            	funprefeitura.funid = 6 AND 
            	mun.estuf ='".$estuf."'AND
               	entprefeito.entstatus = 'A' AND
            	entprefeitura.entstatus = 'A'";
	$entidadeDirigente = $db->carregar($sql);
	
	/*** Recupera as escolas do lote ***/
	$escolas = $db->carregarColuna("SELECT eacid FROM pdeescola.eacloteimpressao WHERE elicodigolote = " . $_GET["lote"]);
}
/*** Se for perfil Municipal ***/
else if( in_array(PDEESC_PERFIL_SEC_MUNICIPAL_ESCOLA_ACESSIVEL, $usuPerfil) )
{
	/*** Recupera o município associado ao usuário ***/
	$muncod = $db->pegaUm("SELECT muncod FROM pdeescola.usuarioresponsabilidade WHERE usucpf = '".$_SESSION["usucpf"]."' AND rpustatus = 'A'");
	
	/*** Recupera a entidade executora ***/
	$sql = "SELECT 
				ent.entnumcpfcnpj, 
				ent.entnome,
				mun.mundescricao,
				mun.estuf
			FROM 
				entidade.entidade ent
			INNER JOIN 
				entidade.endereco en ON en.entid = ent.entid
			INNER JOIN
				entidade.funcaoentidade fue ON fue.entid = ent.entid AND
											   fue.funid = 1
			INNER JOIN
				territorios.municipio mun ON mun.muncod = en.muncod
			WHERE
				en.muncod = '".$muncod."'";
	$entidadeExecutora = $db->carregar($sql);
	
	/*** Recupera a entidade dirigente ***/
	$sql = "SELECT 
				entprefeito.entnumcpfcnpj,
				entprefeito.entnome
            FROM 
            	entidade.entidade entprefeito
           	INNER JOIN 
           		entidade.funcaoentidade funprefeito ON entprefeito.entid = funprefeito.entid
            INNER JOIN 
            	entidade.funentassoc feaprefeito ON feaprefeito.fueid = funprefeito.fueid
            INNER JOIN 
            	entidade.entidade entprefeitura ON entprefeitura.entid = feaprefeito.entid
            INNER JOIN 
            	entidade.funcaoentidade funprefeitura ON funprefeitura.entid = entprefeitura.entid
            INNER JOIN 
            	entidade.endereco entd ON entd.entid = entprefeitura.entid
            INNER JOIN 
            	territorios.municipio mun ON entd.muncod = mun.muncod
            WHERE 
            	funprefeito.funid = 2 AND 
            	funprefeitura.funid = 1 AND 
            	mun.muncod ='".$muncod."'AND
            	entprefeito.entstatus = 'A' AND
            	entprefeitura.entstatus = 'A'";
	$entidadeDirigente = $db->carregar($sql);
	
	/*** Recupera as escolas do lote ***/
	$escolas = $db->carregarColuna("SELECT eacid FROM pdeescola.eacloteimpressao WHERE elicodigolote = " . $_GET["lote"]);
}
/*** Se não for um dos perfis permitidos ***/
else
{
	echo "<script>
			alert('Não há nenhum Munícipio ou Estado associado ao seu perfil.');
			window.close();
		  </script>";
	exit;
}

function recuperaLimites($inicio, $fim)
{
	if(($inicio >= 0 && $inicio <= 199) && ($fim >= 0 && $fim <= 199)) {
		$recurso = 12000;
	}
	if(($inicio >= 200 && $inicio <= 499) && ($fim >= 200 && $fim <= 499)) {
		$recurso = 14000;
	}
	if(($inicio >= 500 && $inicio <= 1000) && ($fim >= 500 && $fim <= 1000)) {
		$recurso = 16000;
	}	
	if(($inicio > 1000) && ($fim >1000)) {
		$recurso = 18000;
	}
	
	return $recurso;
}

/*** Se houverem escolas no lote ***/
if( $escolas )
{
?>

<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>

<style>

table.tabelaRelatorio {
	border:1px solid #000000;
	font-size: 8px;
}

td.tituloRelatorio {
	background-color:#c0c0c0;
	border-top: 1px solid #000000;
	border-bottom: 1px solid #000000;
	font-size: 8px;
}

td.tituloRelatorio2 {
	background-color:#c0c0c0;
	border-bottom: 1px solid #000000;
	font-size: 8px;
}

td.bordaDireita {
	border-right: 1px solid #000000;
	font-size: 8px;
}

td.bordaBaixo {
	border-bottom: 1px solid #000000;
	font-size: 8px;
}

td.bordaDireitaBaixo {
	border-right: 1px solid #000000;
	border-bottom: 1px solid #000000;
	font-size: 8px; 
}

</style>
<!--  Cabeçalho -->
<table width="100%" border="0" cellpadding="0" cellspacing="0">
	<tr>
		<td valign="top" width="50" rowspan="2">
			<img src="../imagens/brasao.gif" width="45" height="45" border="0">
		</td>
		<td nowrap align="left" valign="top" height="1" style="padding:5px 0 0 0;">
			<font style="font-size:11px;">
				MEC - Ministério da Educação<br />
				SIMEC - Sistema Integrado de Monitoramento Execução e Controle<br/>
				SEESP - Secretaria da Educação Especial<br/>
				PDDE ESCOLA ACESSIVEL 
			</font>
		</td>
		<td align="right" valign="top" height="1" style="padding:5px 0 0 0;">
			<font style="font-size:11px;">
				Impresso por: <b><?= $_SESSION['usunome'] ?></b><br/>
				Data/Hora da Impressão: <?= date( 'd/m/Y - H:i:s' ) ?></b><br/>
			</font>
		</td>
		<tr><td>&nbsp;</td></tr>
	</tr>
</table>

<table class="tabelaRelatorio" border="0" width="100%" align="center" cellspacing="0" cellpadding="2">
	<tr>
		<td align="center" colspan="8"><font size="3"><b>PLANO DE ATENDIMENTO GLOBAL CONSOLIDADO - <?= $_SESSION["exercicio"] ?></b></td>
	</tr>
	<tr>
		<td align="left" colspan="8" class="tituloRelatorio"><b>ENTIDADE EXECUTORA</b></td>
	</tr>
	<tr>
		<td align="left" colspan="1" class="bordaDireita">CNPJ<br /><?= ($entidadeExecutora[0]["entnumcpfcnpj"]) ? ($entidadeExecutora[0]["entnumcpfcnpj"]) : "NÃO CADASTRADO" ?></td>
		<td align="left" colspan="3" class="bordaDireita">NOME DA ENTIDADE<br /><?= ($entidadeExecutora[0]["entnome"]) ? $entidadeExecutora[0]["entnome"] : "NÃO CADASTRADO" ?></td>
		<td align="left" colspan="3" class="bordaDireita">MUNICÍPIO<br /><?= ($entidadeExecutora[0]["mundescricao"]) ? $entidadeExecutora[0]["mundescricao"] : "NÃO CADASTRADO" ?></td>
		<td align="left" colspan="1"><font style="font-size:8px;">UF<br /><?= ($entidadeExecutora[0]["estuf"]) ? $entidadeExecutora[0]["estuf"] : "NÃO CADASTRADO" ?></font></td>
	</tr>
	<tr>
		<td align="left" colspan="8" class="tituloRelatorio"><b>DIRIGENTE DA ENTIDADE</b></td>
	</tr>
	<tr>
		<td align="left" colspan="1" class="bordaDireita">CPF<br /><?= ($entidadeDirigente[0]["entnumcpfcnpj"]) ? $entidadeDirigente[0]["entnumcpfcnpj"] : "NÃO CADASTRADO" ?></td>
		<td align="left" colspan="6" class="bordaDireita"><font style="font-size:8px;">NOME DO DIRIGENTE<br /><?= ($entidadeDirigente[0]["entnome"]) ? $entidadeDirigente[0]["entnome"] : "NÃO CADASTRADO" ?></font></td>
		<td align="left" colspan="1"><font style="font-size:8px;">ANO<br /><?= date('Y') ?></font></td>
	</tr>
</table>
<br />

<table class="tabelaRelatorio" border="0" width="100%" align="center" cellspacing="0" cellpadding="2">
	<tr>
		<td align="left" class="tituloRelatorio2" style="border-bottom:0px;" colspan="17"><b>LISTAGEM DAS ESCOLAS</b></td>
	</tr>
</table>
<br />

<table class="tabelaRelatorio" border="0" width="100%" align="center" cellspacing="0" cellpadding="2">
	<tr>
		<td align="center" style="width:2.5%;font-weight:bold;font-size:9px;" class="tituloRelatorio2 bordaDireitaBaixo" valign="top">N°</td>
		<td align="center" style="width:2.5%;font-weight:bold;font-size:9px;" class="tituloRelatorio2 bordaDireitaBaixo" valign="top">Cod. INEP</td>
		<td align="center" style="width:5%;font-weight:bold;font-size:9px;" class="tituloRelatorio2 bordaDireitaBaixo" valign="top">Nome da Escola</td>
		<td align="center" style="width:10%;font-weight:bold;font-size:9px;" class="tituloRelatorio2 bordaDireitaBaixo" valign="top">Adequação de sanitários; alargamento de portas e vias de acesso; construção de rampas; corrimão; sinalização tátil e visual</td>
		<td align="center" style="width:10%;font-weight:bold;font-size:9px;" class="tituloRelatorio2 bordaDireitaBaixo" valign="top">Aquisição de mobiliário acessível; cadeira de rodas; material desportivo acessível; e outros recursos de Tecnologia Assistiva.</td>
		<td align="center" style="width:10%;font-weight:bold;font-size:9px;" class="tituloRelatorio2 bordaDireitaBaixo" valign="top">Adequação estrutural e arquitetônica de espaço físico destinado à instalação e funcionamento de Salas de Recursos Mulitifuncionais</td>
		<td align="center" style="width:5%;font-weight:bold;font-size:9px;" class="tituloRelatorio2 bordaDireitaBaixo" valign="top">Valor Total</td>
		<td align="center" style="width:10%;font-weight:bold;font-size:9px;" class="tituloRelatorio2 bordaDireitaBaixo" valign="top">Valor Custeio</td>
		<td align="center" style="width:10%;font-weight:bold;font-size:9px;" class="tituloRelatorio2 bordaDireitaBaixo" valign="top">Valor Capital</td>
		<td align="center" style="width:10%;font-weight:bold;font-size:9px;" class="tituloRelatorio2 bordaDireitaBaixo" valign="top">% Parceria</td>
		<td align="center" style="width:10%;font-weight:bold;font-size:9px;" class="tituloRelatorio2 bordaDireitaBaixo" valign="top">% Parceria(custeio)</td>
		<td align="center" style="width:10%;font-weight:bold;font-size:9px;" class="tituloRelatorio2 bordaDireitaBaixo" valign="top">% Parceria(capital)</td>
		<td align="center" style="width:5%;font-weight:bold;font-size:9px;" class="tituloRelatorio2 bordaBaixo" valign="top">TOTAL DO PLANO</td>
	</tr>

<?php 

$valorGeralCusteio 	= 0;
$valorGeralCapital 	= 0;
$valorGeralTotal 	= 0;

$ttValorTotal		= 0;
$ttCusteio 			= 0;
$ttCapital 			= 0;
$ttParceria 		= 0;
$ttParceriaCusteio 	= 0;
$ttParceriaCapital 	= 0;
$ttTotalPlano 		= 0;

for($i=0; $i<count($escolas); $i++)
{
	$sql = "SELECT
				eac.entcodent,
				ent.entnome,
				eac.eapossuiparceria,
				eac.eaciniciointervalo,
				eac.eacfimintervalo
			FROM
				pdeescola.eacescolaacessivel eac
			INNER JOIN
				entidade.entidade ent ON ent.entid = eac.entid
			WHERE
				eac.eacid = ".$escolas[$i];
	$eaEscola = $db->carregar($sql);
	
	$recurso = recuperaLimites($eaEscola[0]["eaciniciointervalo"], $eaEscola[0]["eacfimintervalo"]);
	
	$sql = "SELECT
				eta.etaid,
				
				CASE WHEN eaa.eaavlrcusteio is null
				THEN 0.00
				ELSE (eaa.eaavlrcusteio * eaaqtd)
				END AS custeio,
				
				CASE WHEN eaa.eaavlrcapital is null
				THEN 0.00
				ELSE (eaa.eaavlrcapital * eaaqtd)
				END AS capital
			FROM
				pdeescola.eacacaoacessibilidade eaa
			INNER JOIN
				pdeescola.eactipoitemfinanciavel eti ON eti.etiid = eaa.etiid
													AND eti.etistatus = 'A'
													AND eti.etianoreferencia = {$_SESSION["exercicio"]} 
			INNER JOIN
				pdeescola.eactipoacaoacessibilidade eta ON eta.etaid = eti.etaid
													   AND eta.etaastatus = 'A'
													   AND eta.etaanoreferencia = {$_SESSION["exercicio"]}
			WHERE
				eaa.eacid = {$escolas[$i]}";
	$dadosAcao = $db->carregar($sql);
	
	$valorCusteio 	= 0;
	$valorCapital 	= 0;
	$valorTotal 	= 0;
	
	$acao1 = false;
	$acao2 = false;
	$acao3 = false;
	for($k=0; $k<count($dadosAcao); $k++)
	{
		if( $dadosAcao[$k]["etaid"] == 3 ) $acao1 = true;
		if( $dadosAcao[$k]["etaid"] == 5 ) $acao2 = true;
		if( $dadosAcao[$k]["etaid"] == 1 ) $acao3 = true;
		
		$valorCusteio	+= $dadosAcao[$k]["custeio"];
		$valorCapital	+= $dadosAcao[$k]["capital"];
		$valorTotal 	+= ($dadosAcao[$k]["custeio"] + $dadosAcao[$k]["capital"]);
		
		$valorGeralCusteio 	+= $dadosAcao[$k]["custeio"];
		$valorGeralCapital 	+= $dadosAcao[$k]["capital"];
		$valorGeralTotal 	+= ($dadosAcao[$k]["custeio"] + $dadosAcao[$k]["capital"]);
	}
	
	$parceria 		 = 0;
	$parceriaCusteio = 0;
	$parceriaCapital = 0;
		
	if($eaEscola[0]["eapossuiparceria"] == 't')
	{
		if($valorTotal > $recurso)
		{
			$parceria 			= ($valorTotal - $recurso);
			
			$recursoCusteio		= ($recurso * 0.6);
			$recursoCapital		= ($recurso * 0.4);
			
			$parceriaCusteio	= ($valorCusteio>$recursoCusteio) ? ($valorCusteio - $recursoCusteio) : 0;
			$parceriaCapital	= ($valorCapital>$recursoCapital) ? ($valorCapital - $recursoCapital) : 0;
		}
	}
	
	echo "<tr>
			<td align=\"center\" class=\"bordaDireitaBaixo\" valign=\"top\">".($i+1)."</td>
			<td align=\"left\" class=\"bordaDireitaBaixo\" valign=\"top\">".$eaEscola[0]["entcodent"]."</td>
			<td align=\"left\" class=\"bordaDireitaBaixo\" valign=\"top\">".$eaEscola[0]["entnome"]."</td>
			<td align=\"center\" class=\"bordaDireitaBaixo\" valign=\"top\">".(($acao1) ? 'X' : '&nbsp;')."</td>
			<td align=\"center\" class=\"bordaDireitaBaixo\" valign=\"top\">".(($acao2) ? 'X' : '&nbsp;')."</td>
			<td align=\"center\" class=\"bordaDireitaBaixo\" valign=\"top\">".(($acao3) ? 'X' : '&nbsp;')."</td>
			<td align=\"center\" class=\"bordaDireitaBaixo\" valign=\"top\">".number_format(($valorTotal - $parceria),"2",",",".")."</td>
			<td align=\"center\" class=\"bordaDireitaBaixo\" valign=\"top\">".number_format(($valorCusteio-$parceriaCusteio),"2",",",".")."</td>
			<td align=\"center\" class=\"bordaDireitaBaixo\" valign=\"top\">".number_format(($valorCapital-$parceriaCapital),"2",",",".")."</td>
			<td align=\"center\" class=\"bordaDireitaBaixo\" valign=\"top\">".number_format($parceria,"2",",",".")."</td>
			<td align=\"center\" class=\"bordaDireitaBaixo\" valign=\"top\">".number_format($parceriaCusteio,"2",",",".")."</td>
			<td align=\"center\" class=\"bordaDireitaBaixo\" valign=\"top\">".number_format($parceriaCapital,"2",",",".")."</td>
			<td align=\"center\" class=\"bordaBaixo\" valign=\"top\">".number_format($valorTotal,"2",",",".")."</td>
		</tr>";
	
	$ttValorTotal		+= ($valorTotal - $parceria);
	$ttCusteio 			+= ($valorCusteio-$parceriaCusteio);
	$ttCapital 			+= ($valorCapital-$parceriaCapital);
	$ttParceria 		+= $parceria;
	$ttParceriaCusteio 	+= $parceriaCusteio;
	$ttParceriaCapital 	+= $parceriaCapital;
	$ttTotalPlano 		+= $valorTotal;
}

echo "<tr>
		<td align=\"right\" colspan=\"6\" class=\"bordaDireitaBaixo\" valign=\"top\"><b>TOTAL</b></td>
		<td align=\"center\" class=\"bordaDireitaBaixo\" valign=\"top\">".number_format($ttValorTotal,"2",",",".")."</td>
		<td align=\"center\" class=\"bordaDireitaBaixo\" valign=\"top\">".number_format($ttCusteio,"2",",",".")."</td>
		<td align=\"center\" class=\"bordaDireitaBaixo\" valign=\"top\">".number_format($ttCapital,"2",",",".")."</td>
		<td align=\"center\" class=\"bordaDireitaBaixo\" valign=\"top\">".number_format($ttParceria,"2",",",".")."</td>
		<td align=\"center\" class=\"bordaDireitaBaixo\" valign=\"top\">".number_format($ttParceriaCusteio,"2",",",".")."</td>
		<td align=\"center\" class=\"bordaDireitaBaixo\" valign=\"top\">".number_format($ttParceriaCapital,"2",",",".")."</td>
		<td align=\"center\" class=\"bordaBaixo\" valign=\"top\">".number_format($ttTotalPlano,"2",",",".")."</td>
	</tr>";

echo
"</table>
<br />

<table class=\"tabelaRelatorio\" border=\"0\" width=\"100%\" align=\"center\" cellspacing=\"0\" cellpadding=\"2\">
	<tr>
		<td align=\"left\" class=\"tituloRelatorio2\"><b>TOTAIS GERAIS</b></td>
	</tr>
	<tr valign=\"bottom\">
		<td align=\"center\" style=\"font-weight:bold;font-size:9px;\">
			N° de Escolas: ".count($escolas)."
			&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;|&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
			Valor Total Custeio: ".number_format($valorGeralCusteio,"2",",",".")."
			&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;|&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
			Valor Total Capital: ".number_format($valorGeralCapital,"2",",",".")."
			&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;|&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
			Total Geral: ".number_format($valorGeralTotal,"2",",",".")."
		</td>
	</tr>
</table>
<br />

<table class=\"tabelaRelatorio\" border=\"0\" width=\"100%\" align=\"center\" cellspacing=\"0\" cellpadding=\"2\">
	<tr>
		<td align=\"left\" colspan=\"5\" class=\"tituloRelatorio2\"><b>AUTENTICAÇÃO</b></td>
	</tr>
	<tr valign=\"bottom\" style=\"height:80px;\">
		<td align=\"center\" class=\"bordaDireitaBaixo\">
		__________________________________________________________________________________
		<br />
		LOCAL E DATA
		</td>
		<td align=\"center\" class=\"bordaBaixo\">
		__________________________________________________________________________________
		<br />
		DIRIGENTE DA EEx
	</tr>
	<tr>
		<td align=\"left\" colspan=\"5\" class=\"tituloRelatorio2\"><b>ANÁLISE DA SEESP</b></td>
	</tr>
	<tr valign=\"bottom\" style=\"height:80px;\">
		<td align=\"center\" class=\"bordaDireitaBaixo\">
		__________________________________________________________________________________
		<br />
		LOCAL E DATA
		</td>
		<td align=\"center\" class=\"bordaBaixo\">
		De acordo, _______________________________________________________________________
		<br />
		ANALISTA
	</tr>
	<tr>
		<td align=\"left\" colspan=\"5\" class=\"tituloRelatorio2\"><b>APROVAÇÃO</b></td>
	</tr>
	<tr valign=\"bottom\" style=\"height:80px;\">
		<td align=\"center\" class=\"bordaDireita\">
		__________________________________________________________________________________
		<br />
		LOCAL E DATA
		</td>
		<td align=\"center\"><font style=\"font-size:8px;\">
		__________________________________________________________________________________
		<br />
		SECRETÁRIO DA SEESP</font>
	</tr>
</table>
<br />";

}
else
{
	echo "<script>
			alert('Não existem escolas cadastradas neste lote.');
			window.close();
		  </script>";
	exit;
}

?>