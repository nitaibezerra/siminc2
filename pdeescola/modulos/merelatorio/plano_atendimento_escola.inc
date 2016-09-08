<?

/*** Verifica as variáveis de sessão ***/
meVerificaSessao();

$memid 		= $_SESSION["memid"];
$meentid 	= $_SESSION["meentid"];

// RECUPERA O TIPO CLASSIFICAÇÃO E O MUNICÍPIO/ESTADO DA ESCOLA
$sql = "SELECT
			(CASE WHEN ent.tpcid = 1 THEN 'Estadual'
			      WHEN ent.tpcid = 3 THEN 'Municipal'
			 END) AS tipo,
			 en.muncod,
			 en.estuf
		FROM
			entidade.entidade ent
		LEFT JOIN 
			entidade.endereco en ON en.entid = ent.entid
		WHERE
			ent.entid = ".$meentid;
$tipoEscola = $db->carregar($sql);

// RECUPERA OS DADOS DA UNIDADE EXECUTORA
if($tipoEscola[0]["tipo"] == "Estadual") {
	$sql = "SELECT 
				ent.entnumcpfcnpj, 
				ent.entnome,
				mun.mundescricao,
				mun.estuf
			FROM 
				entidade.entidade ent
			INNER JOIN
				entidade.funcaoentidade fue ON fue.entid = ent.entid AND
										   	   fue.funid = 6
			INNER JOIN 
				entidade.endereco en ON en.entid = ent.entid AND
										en.estuf = '".$tipoEscola[0]["estuf"]."'
			INNER JOIN
				territorios.municipio mun ON mun.muncod = en.muncod";
} else {
	$sql = "SELECT 
				ent.entnumcpfcnpj, 
				ent.entnome,
				mun.mundescricao,
				mun.estuf
			FROM 
				entidade.entidade ent
			INNER JOIN
				entidade.funcaoentidade fue ON fue.entid = ent.entid AND
										   	   fue.funid = 1
			INNER JOIN 
				entidade.endereco en ON en.entid = ent.entid AND
										en.muncod = '".$tipoEscola[0]["muncod"]."'
			INNER JOIN
				territorios.municipio mun ON mun.muncod = en.muncod";
}
$unidadeExecutora = $db->carregar($sql);

// RECUPERA OS DADOS DA ESCOLA
$sql = "SELECT 
			ent.entcodent,
			ent.entnome,
			mun.mundescricao,
			mun.estuf
		FROM 
			entidade.entidade ent
		LEFT JOIN 
			entidade.endereco en ON en.entid = ent.entid
		LEFT JOIN
			territorios.municipio mun ON mun.muncod = en.muncod
		WHERE 
			ent.entid = ".$meentid;
$escola = $db->carregar($sql);

// RECUPERA OS DADOS DO DIRETOR DA ESCOLA
$sql = "SELECT 
			e.entnumcpfcnpj,
			e.entnome,
			en.endlog,
			en.endcom,
			en.endbai,
			mun.mundescricao,
			mun.estuf,
			en.endcep,
			e.entnumdddresidencial,
			e.entnumresidencial,
			e.entnumfax,
			e.entemail,
			e.entnumrg,
			e.entdatainiass,
			e.entorgaoexpedidor,
			e.entid
		FROM
			entidade.entidade e
		INNER JOIN
			entidade.funcaoentidade fe ON e.entid = fe.entid
		INNER JOIN 
			entidade.funentassoc fea on fea.fueid = fe.fueid
		LEFT JOIN 
			entidade.endereco en ON en.entid = e.entid
		LEFT JOIN
			territorios.municipio mun ON mun.muncod = en.muncod
		WHERE
			fea.entid = '".$meentid."' AND
			fe.funid = ".FUN_DIRETOR_ME." AND 
			fe.fuestatus = 'A'";
$diretorEscola = $db->carregar($sql);

// RECUPERA OS DADOS DO COORDENADOR DA ESCOLA
$sql = "SELECT 
			e.entnumcpfcnpj,
			e.entnome,
			en.endlog,
			en.endcom,
			en.endbai,
			mun.mundescricao,
			mun.estuf,
			en.endcep,
			e.entnumdddresidencial,
			e.entnumresidencial,
			e.entnumfax,
			e.entemail,
			e.entnumrg,
			e.entdatainiass,
			e.entorgaoexpedidor,
			e.entid
		FROM
			entidade.entidade e
		INNER JOIN
			entidade.funcaoentidade fe ON e.entid = fe.entid
		INNER JOIN 
			entidade.funentassoc fea on fea.fueid = fe.fueid
		LEFT JOIN 
			entidade.endereco en ON en.entid = e.entid
		LEFT JOIN
			territorios.municipio mun ON mun.muncod = en.muncod
		WHERE
			fea.entid = '".$meentid."' AND
			fe.funid = ".FUN_COORDENADOR_ME." AND 
			fe.fuestatus = 'A'";
$coordenadorEscola = $db->carregar($sql);

?>

<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
<style>

table.tabelaRelatorio {
	border:1px solid #000000;
}

table.tabelaRelatorio2 {
	border-bottom:1px solid #000000;
	border-right:1px solid #000000;
	border-left:1px solid #000000;
}

td.tituloRelatorio {
	background-color:#c0c0c0;
	border-top: 1px solid #000000;
	border-bottom: 1px solid #000000;
}

td.tituloRelatorio2 {
	background-color:#c0c0c0;
	border-bottom: 1px solid #000000;
}

td.bordaDireita {
	border-right: 1px solid #000000;
}

td.bordaBaixo {
	border-bottom: 1px solid #000000;
}

td.bordaDireitaBaixo {
	border-right: 1px solid #000000;
	border-bottom: 1px solid #000000; 
}

</style>
<table width="100%" border="0" cellpadding="0" cellspacing="0">
	<tr>
		<td valign="top" width="50" rowspan="2">
			<img src="../imagens/brasao.gif" width="45" height="45" border="0">
		</td>
		<td nowrap align="left" valign="top" height="1" style="padding:5px 0 0 0;">
			<font style="font-size:11px;">
				MEC - Ministério da Educação<br />
				SIMEC - Sistema Integrado de Monitoramento Execução e Controle<br/>
				PLANO DE ATENDIMENTO DA ESCOLA
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
		<td align="center" colspan="17"><font size="3"><b>PLANO DE ATENDIMENTO DA ESCOLA</b></td>
	</tr>
	<tr>
		<td align="left" colspan="17" class="tituloRelatorio"><b>UNIDADE EXECUTORA</b></td>
	</tr>
	<tr>
		<td align="left" class="bordaDireita">1 - CNPJ - UEx<br /><?= ($unidadeExecutora[0]["entnumcpfcnpj"]) ? ($unidadeExecutora[0]["entnumcpfcnpj"]) : "NÃO CADASTRADO" ?></td>
		<td align="left" colspan="2" class="bordaDireita">2 - NOME DA UNIDADE EXECUTORA<br /><?= ($unidadeExecutora[0]["entnome"]) ? ($unidadeExecutora[0]["entnome"]) : "NÃO CADASTRADO" ?></td>
		<td align="left" class="bordaDireita">3 - MUNICÍPIO<br /><?= ($unidadeExecutora[0]["mundescricao"]) ? ($unidadeExecutora[0]["mundescricao"]) : "NÃO CADASTRADO" ?></td>
		<td align="left">4 - UF<br /><?= ($unidadeExecutora[0]["estuf"]) ? ($unidadeExecutora[0]["estuf"]) : "NÃO CADASTRADO" ?></td>
	</tr>
	<tr>
		<td align="left" colspan="17" class="tituloRelatorio"><b>ESCOLA</b></td>
	</tr>
	<tr>
		<td align="left" width="10%" rowspan="3" class="bordaDireita">5 - COD. DA ESCOLA<br /><?= ($escola[0]["entcodent"]) ? ($escola[0]["entcodent"]) : "NÃO CADASTRADO" ?></td>
		<td align="left" width="20%" rowspan="3" class="bordaDireita">6 - NOME DA ESCOLA<br /><?= ($escola[0]["entnome"]) ? ($escola[0]["entnome"]) : "NÃO CADASTRADO" ?></td>
		<td align="left" width="20%" rowspan="3" class="bordaDireita">7 - MUNICÍPIO<br /><?= ($escola[0]["mundescricao"]) ? ($escola[0]["mundescricao"]) : "NÃO CADASTRADO" ?></td>
		<td align="left" width="11%" rowspan="3" class="bordaDireita">8 - UF<br /><?= ($escola[0]["estuf"]) ? ($escola[0]["estuf"]) : "NÃO CADASTRADO" ?></td>
		<td align="left" width="39%" colspan="13" class="bordaBaixo">9 - ALUNADO PARTICIPANTE</td>
	</tr>
	<tr>
		<td align="center" width="3%" class="bordaDireitaBaixo">1° ANO</td>
		<td align="center" width="3%" class="bordaDireitaBaixo">2° ANO</td>
		<td align="center" width="3%" class="bordaDireitaBaixo">3° ANO</td>
		<td align="center" width="3%" class="bordaDireitaBaixo">4° ANO</td>
		<td align="center" width="3%" class="bordaDireitaBaixo">5° ANO</td>
		<td align="center" width="3%" class="bordaDireitaBaixo">6° ANO</td>
		<td align="center" width="3%" class="bordaDireitaBaixo">7° ANO</td>
		<td align="center" width="3%" class="bordaDireitaBaixo">8° ANO</td>
		<td align="center" width="3%" class="bordaDireitaBaixo">9° ANO</td>
		<td align="center" width="3%" class="bordaDireitaBaixo">1° ANO</td>
		<td align="center" width="3%" class="bordaDireitaBaixo">2° ANO</td>
		<td align="center" width="3%" class="bordaDireitaBaixo">3° ANO</td>
		<td align="center" width="3%" class="bordaBaixo">TOTAL</td>
	</tr>
	<tr>
	<?
	$modalidadeEnsino = $db->pegaUm("SELECT memmodalidadeensino FROM pdeescola.memaiseducacao WHERE memid = ".$memid);
		
	$somaAlunado = 0;
	for($j=0; $j<12; $j++) {
		// Ensino Fundamental
		if($j>=0 && $j<=8) $inSerie = ($j + 1);
		// Ensino Médio
		if($j>=9 && $j<=11) $inSerie = ($j + 11);
		
		$sql = "SELECT
					map.mapquantidade as qtd
				FROM
					pdeescola.mealunoparticipante map
				INNER JOIN
					pdeescola.seriecicloescolar sce ON sce.sceid = map.sceid
								AND sce.sceid IN (".$inSerie.")
				WHERE
					map.memid = ".$memid." AND
					map.mapano = " . $_SESSION["exercicio"] . "
				ORDER BY
					map.sceid";
		$alunadoParticipante = $db->carregar($sql);
		
		if($modalidadeEnsino == "F") {
			if($j>=0 && $j<=8) {
				echo "<td align=\"center\" width=\"3%\" class=\"bordaDireita\">".($alunadoParticipante[0]["qtd"])."</td>";
				$somaAlunado += (integer)$alunadoParticipante[0]["qtd"];
			}
			else {
				echo "<td align=\"center\" width=\"3%\" class=\"bordaDireita\">-</td>";
			}
		}
		else if($modalidadeEnsino == "M") {
			if($j>=9 && $j<=11) {
				echo "<td align=\"center\" width=\"3%\" class=\"bordaDireita\">".($alunadoParticipante[0]["qtd"])."</td>";
				$somaAlunado += (integer)$alunadoParticipante[0]["qtd"];
			}
			else {
				echo "<td align=\"center\" width=\"3%\" class=\"bordaDireita\">-</td>";
			}
		}
		else {
			echo "<td align=\"center\" width=\"3%\" class=\"bordaDireita\">".($alunadoParticipante[0]["qtd"])."</td>";
			$somaAlunado += (integer)$alunadoParticipante[0]["qtd"];
		}
	}
	
	echo "<td align=\"center\" width=\"3%\">".$somaAlunado."</td>";
	?>
	</tr>
</table>
<table class="tabelaRelatorio2" border="0" width="100%" align="center" cellspacing="0" cellpadding="2">
	<tr>
		<td align="left" colspan="10" class="tituloRelatorio2"><b>DIRETOR DA ESCOLA</b></td>
	</tr>
	<tr>
		<td align="left" colspan="1" class="bordaDireitaBaixo">10 - CPF<br /><?= ($diretorEscola[0]["entnumcpfcnpj"]) ? ($diretorEscola[0]["entnumcpfcnpj"]) : "NÃO CADASTRADO" ?></td>
		<td align="left" colspan="2" class="bordaDireitaBaixo">11 - NOME DO DIRETOR<br /><?= ($diretorEscola[0]["entnome"]) ? ($diretorEscola[0]["entnome"]) : "NÃO CADASTRADO" ?></td>
		<td align="left" colspan="3" class="bordaDireitaBaixo">12 - ENDEREÇO(Rua,avenida ou praça e número)<br /><?= ($diretorEscola[0]["endlog"]) ? ($diretorEscola[0]["endlog"]) : "NÃO CADASTRADO" ?></td>
		<td align="left" colspan="2" class="bordaDireitaBaixo">13 - COMPLEMENTO DO ENDEREÇO(Andar,sala,etc...)<br /><?= ($diretorEscola[0]["endcom"]) ? ($diretorEscola[0]["endcom"]) : "NÃO CADASTRADO" ?></td>
		<td align="left" colspan="2" class="bordaBaixo">14 - BAIRRO OU DISTRITO<br /><?= ($diretorEscola[0]["endbai"]) ? ($diretorEscola[0]["endbai"]) : "NÃO CADASTRADO" ?></td>
	</tr>
	<tr>
		<td align="left" width="5%" class="bordaDireita">15 - UF<br /><?= ($diretorEscola[0]["estuf"]) ? ($diretorEscola[0]["estuf"]) : "NÃO CADASTRADO" ?></td>
		<td align="left" width="15%" class="bordaDireita">16 - MUNICÍPIO<br /><?= ($diretorEscola[0]["mundescricao"]) ? ($diretorEscola[0]["mundescricao"]) : "NÃO CADASTRADO" ?></td>
		<td align="left" width="5%" class="bordaDireita">17 - CEP<br /><?= ($diretorEscola[0]["endcep"]) ? ($diretorEscola[0]["endcep"]) : "NÃO CADASTRADO" ?></td>
		<td align="left" width="5%" class="bordaDireita">18 - DDD<br /><?= ($diretorEscola[0]["entnumdddresidencial"]) ? ($diretorEscola[0]["entnumdddresidencial"]) : "NÃO CADASTRADO" ?></td>
		<td align="left" width="10%" class="bordaDireita">19 - TELEFONE<br /><?= ($diretorEscola[0]["entnumresidencial"]) ? ($diretorEscola[0]["entnumresidencial"]) : "NÃO CADASTRADO" ?></td>
		<td align="left" width="10%" class="bordaDireita">20 - FAX<br /><?= ($diretorEscola[0]["entnumfax"]) ? ($diretorEscola[0]["entnumfax"]) : "NÃO CADASTRADO" ?></td>
		<td align="left" width="15%"class="bordaDireita">21 - EMAIL<br /><?= ($diretorEscola[0]["entemail"]) ? ($diretorEscola[0]["entemail"]) : "NÃO CADASTRADO" ?></td>
		<td align="left" width="15%" class="bordaDireita">22 - N° DA CARTEIRA DE IDENTIDADE<br /><?= ($diretorEscola[0]["entnumrg"]) ? ($diretorEscola[0]["entnumrg"]) : "NÃO CADASTRADO" ?></td>
		<td align="left" width="10%" class="bordaDireita">23 - DATA DE EMISSÃO<br /><?= ($diretorEscola[0]["entdatainiass"]) ? ($diretorEscola[0]["entdatainiass"]) : "NÃO CADASTRADO" ?></td>
		<td align="left" width="10%">24 - ÓRGÃO EXP./UF<br /><?= ($diretorEscola[0]["entorgaoexpedidor"]) ? ($diretorEscola[0]["entorgaoexpedidor"]) : "NÃO CADASTRADO" ?></td>
	</tr>
</table>
<table class="tabelaRelatorio2" border="0" width="100%" align="center" cellspacing="0" cellpadding="2">
	<tr>
		<td align="left" colspan="13" class="tituloRelatorio2"><b>COORDENADOR DA EDUCAÇÃO INTEGRAL NA ESCOLA</b></td>
	</tr>
	<tr>
		<td align="left" colspan="1" class="bordaDireitaBaixo">25 - CPF<br /><?= ($coordenadorEscola[0]["entnumcpfcnpj"]) ? ($coordenadorEscola[0]["entnumcpfcnpj"]) : "NÃO CADASTRADO" ?></td>
		<td align="left" colspan="2" class="bordaDireitaBaixo">26 - NOME DO COORDENADOR<br /><?= ($coordenadorEscola[0]["entnome"]) ? ($coordenadorEscola[0]["entnome"]) : "NÃO CADASTRADO" ?></td>
		<td align="left" colspan="3" class="bordaDireitaBaixo">27 - ENDEREÇO(Rua,avenida ou praça e número)<br /><?= ($coordenadorEscola[0]["endlog"]) ? ($coordenadorEscola[0]["endlog"]) : "NÃO CADASTRADO" ?></td>
		<td align="left" colspan="2" class="bordaDireitaBaixo">28 - COMPLEMENTO DO ENDEREÇO(Andar,sala,etc...)<br /><?= ($coordenadorEscola[0]["endcom"]) ? ($coordenadorEscola[0]["endcom"]) : "NÃO CADASTRADO" ?></td>
		<td align="left" colspan="2" class="bordaBaixo">29 - BAIRRO OU DISTRITO<br /><?= ($coordenadorEscola[0]["endbai"]) ? ($coordenadorEscola[0]["endbai"]) : "NÃO CADASTRADO" ?></td>
	</tr>
	<tr>
		<td align="left" width="5%" class="bordaDireita">30 - UF<br /><?= ($coordenadorEscola[0]["estuf"]) ? ($coordenadorEscola[0]["estuf"]) : "NÃO CADASTRADO" ?></td>
		<td align="left" width="15%" class="bordaDireita">31 - MUNICÍPIO<br /><?= ($coordenadorEscola[0]["mundescricao"]) ? ($coordenadorEscola[0]["mundescricao"]) : "NÃO CADASTRADO" ?></td>
		<td align="left" width="5%" class="bordaDireita">32 - CEP<br /><?= ($coordenadorEscola[0]["endcep"]) ? ($coordenadorEscola[0]["endcep"]) : "NÃO CADASTRADO" ?></td>
		<td align="left" width="5%" class="bordaDireita">33 - DDD<br /><?= ($coordenadorEscola[0]["entnumdddresidencial"]) ? ($coordenadorEscola[0]["entnumdddresidencial"]) : "NÃO CADASTRADO" ?></td>
		<td align="left" width="10%" class="bordaDireita">34 - TELEFONE<br /><?= ($coordenadorEscola[0]["entnumresidencial"]) ? ($coordenadorEscola[0]["entnumresidencial"]) : "NÃO CADASTRADO" ?></td>
		<td align="left" width="10%" class="bordaDireita">35 - FAX<br /><?= ($coordenadorEscola[0]["entnumfax"]) ? ($coordenadorEscola[0]["entnumfax"]) : "NÃO CADASTRADO" ?></td>
		<td align="left" width="15%" class="bordaDireita">36 - EMAIL<br /><?= ($coordenadorEscola[0]["entemail"]) ? ($coordenadorEscola[0]["entemail"]) : "NÃO CADASTRADO" ?></td>
		<td align="left" width="15%" class="bordaDireita">37 - N° DA CARTEIRA DE IDENTIDADE<br /><?= ($coordenadorEscola[0]["entnumrg"]) ? ($coordenadorEscola[0]["entnumrg"]) : "NÃO CADASTRADO" ?></td>
		<td align="left" width="10%" class="bordaDireita">38 - DATA DA EMISSÃO<br /><?= ($coordenadorEscola[0]["entdatainiass"]) ? ($coordenadorEscola[0]["entdatainiass"]) : "NÃO CADASTRADO" ?></td>
		<td align="left" width="10%">39 - ÓRGÃO EXP./UF<br /><?= ($coordenadorEscola[0]["entorgaoexpedidor"]) ? ($coordenadorEscola[0]["entorgaoexpedidor"]) : "NÃO CADASTRADO" ?></td>
	</tr>
</table>
<table class="tabelaRelatorio2" border="0" width="100%" align="center" cellspacing="0" cellpadding="2">
	<tr>
		<td align="left" colspan="16" class="tituloRelatorio2"><b>ATIVIDADES</b></td>
	</tr>
	<tr>
		<td align="center" rowspan="2" width="40%" class="bordaDireitaBaixo">40 - ATIVIDADES A SEREM DESENVOLVIDAS</td>
		<td align="center" rowspan="2" width="10%" class="bordaDireitaBaixo">41 - COD. DA ATIVIDADE</td>
		<td align="center" colspan="13" width="39%" class="bordaDireitaBaixo">42 - QUANTIDADE DE ALUNOS PARTICIPANTES</td>
		<td align="center" rowspan="2" width="11%" class="bordaBaixo">43 - ATIVIDADE INTERNA/EXTERNA</td>
	</tr>
	<tr>
		<td align="center" width="3%" class="bordaDireitaBaixo">1° ANO</td>
		<td align="center" width="3%" class="bordaDireitaBaixo">2° ANO</td>
		<td align="center" width="3%" class="bordaDireitaBaixo">3° ANO</td>
		<td align="center" width="3%" class="bordaDireitaBaixo">4° ANO</td>
		<td align="center" width="3%" class="bordaDireitaBaixo">5° ANO</td>
		<td align="center" width="3%" class="bordaDireitaBaixo">6° ANO</td>
		<td align="center" width="3%" class="bordaDireitaBaixo">7° ANO</td>
		<td align="center" width="3%" class="bordaDireitaBaixo">8° ANO</td>
		<td align="center" width="3%" class="bordaDireitaBaixo">9° ANO</td>
		<td align="center" width="3%" class="bordaDireitaBaixo">1° ANO</td>
		<td align="center" width="3%" class="bordaDireitaBaixo">2° ANO</td>
		<td align="center" width="3%" class="bordaDireitaBaixo">3° ANO</td>
		<td align="center" width="3%" class="bordaDireitaBaixo">TOTAL</td>
	</tr>
	<?
	$sql = "SELECT
				mea.meaid,
				mea.mealocalizacao,
				mtm.mtmid,
				mtm.mtmdescricao,
				mta.mtaid,
				mta.mtadescricao
			FROM
				pdeescola.meatividade mea
			INNER JOIN
				pdeescola.metipoatividade mta ON mta.mtaid = mea.mtaid
							AND mta.mtasituacao = 'A'
			INNER JOIN
				pdeescola.metipomacrocampo mtm ON mtm.mtmid = mta.mtmid
							AND mtm.mtmsituacao = 'A'
			WHERE
				mea.memid = ".$memid." AND
				mea.meaano = " . $_SESSION["exercicio"] . "
			ORDER BY
				mea.meaid";
	
	$dadosAtividades = $db->carregar($sql);
	
	if($dadosAtividades) {
		$somaGeralAtividades = array();
		
		for($i=0; $i<count($dadosAtividades); $i++) {
			echo "<tr>
					<td align=\"center\" width=\"40%\" class=\"bordaDireitaBaixo\">".$dadosAtividades[$i]["mtmdescricao"]." / ".$dadosAtividades[$i]["mtadescricao"]."</td>
					<td align=\"center\" width=\"10%\" class=\"bordaDireitaBaixo\">".$dadosAtividades[$i]["mtaid"]."</td>";
			$somaAtividades = 0;
			
			for($j=0; $j<12; $j++) {
				// Ensino Fundamental
				if($j>=0 && $j<=8) $inSerie = ($j + 1);
				// Ensino Médio
				if($j>=9 && $j<=11) $inSerie = ($j + 11);
				
				$sql = "SELECT
							map.mpaquantidade
						FROM
							pdeescola.mealunoparticipanteatividade map
						WHERE
							map.meaid = ".$dadosAtividades[$i]["meaid"]." AND 
							map.sceid IN (".$inSerie.")
						ORDER BY
							map.sceid";
				$numAlunosAtividade = $db->carregar($sql);
				
				if($modalidadeEnsino == "F") {
					if($j>=0 && $j<=8) {
						echo "<td align=\"center\" width=\"3%\" class=\"bordaDireitaBaixo\">".$numAlunosAtividade[0]["mpaquantidade"]."</td>";
						$somaAtividades += (integer)$numAlunosAtividade[0]["mpaquantidade"];
						$somaGeralAtividades[$j] += (integer)$numAlunosAtividade[0]["mpaquantidade"];
					}
					else {
						echo "<td align=\"center\" width=\"3%\" class=\"bordaDireitaBaixo\">-</td>";
						$somaGeralAtividades[$j] = "-";
					}
				}
				else if($modalidadeEnsino == "M") {
					if($j>=9 && $j<=11) {
						echo "<td align=\"center\" width=\"3%\" class=\"bordaDireitaBaixo\">".$numAlunosAtividade[0]["mpaquantidade"]."</td>";
						$somaAtividades += (integer)$numAlunosAtividade[0]["mpaquantidade"];
						$somaGeralAtividades[$j] += (integer)$numAlunosAtividade[0]["mpaquantidade"];
					}
					else {
						echo "<td align=\"center\" width=\"3%\" class=\"bordaDireitaBaixo\">-</td>";
						$somaGeralAtividades[$j] = "-";
					}
				}
				else {
					echo "<td align=\"center\" width=\"3%\" class=\"bordaDireitaBaixo\">".$numAlunosAtividade[0]["mpaquantidade"]."</td>";
					$somaAtividades += (integer)$numAlunosAtividade[0]["mpaquantidade"];
					$somaGeralAtividades[$j] += (integer)$numAlunosAtividade[0]["mpaquantidade"];
				}
			}
			
			$somaGeralAtividades[12] += $somaAtividades;
			echo "<td align=\"center\" width=\"3%\" class=\"bordaDireitaBaixo\">".$somaAtividades."</td>
				  <td align=\"center\" width=\"11%\" class=\"bordaBaixo\">".(($dadosAtividades[$i]["mealocalizacao"] == "I") ? "Interna" : "Externa")."</td>
				</tr>";
		}
	}
	?>
	<tr>
		<td align="left" width="40%" class="bordaDireita">44 - TOTAL DE ALUNOS PARTICIPANTES DAS ATIVIDADES</td>
		<td align="center" width="10%" class="bordaDireita" style="background-color:#c0c0c0;">&nbsp;</td>
		<?
		for($i=0; $i<count($somaGeralAtividades); $i++) {
			echo "<td align=\"center\" width=\"3%\" class=\"bordaDireita\">".$somaGeralAtividades[$i]."</td>";
		}
		?>
		<td align="center" width="11%" style="background-color:#c0c0c0;">&nbsp;</td>
	</tr>
</table>
<table class="tabelaRelatorio2" border="0" width="100%" align="center" cellspacing="0" cellpadding="2">
	<tr>
		<td align="left" colspan="5" class="tituloRelatorio2"><b>PARCEIROS</b></td>
	</tr>
	<tr>
		<td align="left" width="6%" class="bordaDireitaBaixo">45 - NOME DO PARCEIRO</td>
		<td align="left" width="40%" class="bordaBaixo">46 - N° DE ALUNOS</td>
	</tr>
	<?
		$sql = "SELECT
					e.entnome,
					mepar.mepquantidadealuno
				FROM 
					pdeescola.meparceiro mepar
				INNER JOIN 
					entidade.entidade e ON mepar.entid = e.entid
				INNER JOIN
					entidade.funcaoentidade fe ON fe.entid = e.entid
				WHERE 
					mepar.memid = ". $memid ." AND 
					fe.funid IN ('".FUN_PARCEIRO_ME_PF."','".FUN_PARCEIRO_ME_PJ."') 
				ORDER BY
					mepar.mepid";
		$parceiros = $db->carregar($sql);
		
		$contParceiros = 0;
		for($i=0; $i<count($parceiros); $i++) {
			echo "<tr>
					<td align=\"center\" width=\"60%\" class=\"bordaDireitaBaixo\">".$parceiros[$i]["entnome"]."</td>
					<td align=\"center\" width=\"40%\" class=\"bordaBaixo\">".$parceiros[$i]["mepquantidadealuno"]."</td>
				  </tr>";
			$contParceiros += (integer)$parceiros[$i]["mepquantidadealuno"];
		}
	?>
	<tr>
		<td align="right" width="60%" class="bordaDireita">47 - TOTAL</td>
		<td align="center" width="40%"><?=$contParceiros?></td>
	</tr>
</table>
<table class="tabelaRelatorio2" border="0" width="100%" align="center" cellspacing="0" cellpadding="2">
	<tr>
		<td align="left" colspan="5" class="tituloRelatorio2"><b>AUTENTICAÇÃO</b></td>
	</tr>
	<tr valign="bottom" style="height:80px;">
		<td align="center" class="bordaDireita">
		__________________________________________________________________________________
		<br />
		LOCAL E DATA
		</td>
		<td align="center">
		__________________________________________________________________________________
		<br />
		DIRETOR DA ESCOLA
	</tr>
</table>