<?php

/*** Seta mais memória para carregar a página e sem tempo limite ***/
ini_set( "memory_limit", "512M" );
set_time_limit(0);

/*** Recupera o perfil do usuário ***/
$usuPerfil = arrayPerfil();

/*** Se tiver perfil administrativo ***/
if( in_array(PDEESC_PERFIL_ADMINISTRADOR_ESCOLA_ABERTA, $usuPerfil) || in_array(PDEESC_PERFIL_CONSULTA_ESCOLA_ABERTA, $usuPerfil) || in_array(PDEESC_PERFIL_SUPER_USUARIO, $usuPerfil) )
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
	$escolas = $db->carregarColuna("SELECT eabid FROM pdeescola.eabloteimpressao WHERE blicodigolote = " . $_GET["lote"]);
}
/*** Se for perfil Estadual ***/
else if( in_array(PDEESC_PERFIL_SEC_ESTADUAL_ESCOLA_ABERTA, $usuPerfil) )
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
            	mun.estuf ='".$estuf."' AND
            	entprefeito.entstatus = 'A' AND
            	entprefeitura.entstatus = 'A'";
	$entidadeDirigente = $db->carregar($sql);
	
	/*** Recupera as escolas do lote ***/
	$escolas = $db->carregarColuna("SELECT eabid FROM pdeescola.eabloteimpressao WHERE blicodigolote = " . $_GET["lote"]);
}
/*** Se for perfil Municipal ***/
else if( in_array(PDEESC_PERFIL_SEC_MUNICIPAL_ESCOLA_ABERTA, $usuPerfil) )
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
            	mun.muncod ='".$muncod."' AND
            	entprefeito.entstatus = 'A' AND
            	entprefeitura.entstatus = 'A'";
	$entidadeDirigente = $db->carregar($sql);
	
	/*** Recupera as escolas do lote ***/
	$escolas = $db->carregarColuna("SELECT eabid FROM pdeescola.eabloteimpressao WHERE blicodigolote = " . $_GET["lote"]);
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
				FNDE - Fundo Nacional de Desenvolvimento da Educação<br />
				<?php echo NOME_SISTEMA; ?><br/>
				Programa Dinheiro Direto na Escola(PDDE)<br />
				Funcionamento das Escolas nos Finais de Semana
			</font>
		</td>
		<td align="right" valign="top" height="1" style="padding:5px 0 0 0;">
			<font style="font-size:11px;">
				Impresso por: <b><?= $_SESSION['usunome'] ?></b><br/>
				Data/Hora da Impressão: <b><?= date( 'd/m/Y - H:i:s' ) ?></b><br/>
			</font>
		</td>
		<tr><td>&nbsp;</td></tr>
	</tr>
</table>

<table class="tabelaRelatorio" border="0" width="100%" align="center" cellspacing="0" cellpadding="2">
	<tr>
		<td align="center" colspan="10" class="bordaDireita"><font size="3"><b>PLANO CONSOLIDADO DE ATIVIDADES - EXERCÍCIO <?= $_SESSION["exercicio"] ?>/2012</b></font></td>
	</tr>
	<tr>
		<td align="left" colspan="8" class="tituloRelatorio"><b>ENTIDADE EXECUTORA</b></td>
	</tr>
	<tr>
		<td align="left" colspan="1" class="bordaDireita">CNPJ<br /><?= ($entidadeExecutora[0]["entnumcpfcnpj"]) ? ($entidadeExecutora[0]["entnumcpfcnpj"]) : "NÃO CADASTRADO" ?></td>
		<td align="left" colspan="3" class="bordaDireita">NOME DA EEx<br /><?= ($entidadeExecutora[0]["entnome"]) ? $entidadeExecutora[0]["entnome"] : "NÃO CADASTRADO" ?></td>
		<td align="left" colspan="3" class="bordaDireita">MUNICÍPIO<br /><?= ($entidadeExecutora[0]["mundescricao"]) ? $entidadeExecutora[0]["mundescricao"] : "NÃO CADASTRADO" ?></td>
		<td align="left" colspan="1"><font style="font-size:8px;">UF<br /><?= ($entidadeExecutora[0]["estuf"]) ? $entidadeExecutora[0]["estuf"] : "NÃO CADASTRADO" ?></font></td>
	</tr>
	<tr>
		<td align="left" colspan="8" class="tituloRelatorio"><b>DIRIGENTE DA EEx</b></td>
	</tr>
	<tr>
		<td align="left" colspan="1" class="bordaDireita">CPF<br /><?= ($entidadeDirigente[0]["entnumcpfcnpj"]) ? $entidadeDirigente[0]["entnumcpfcnpj"] : "NÃO CADASTRADO" ?></td>
		<td align="left" colspan="7" class="bordaDireita"><font style="font-size:8px;">NOME DO DIRIGENTE<br /><?= ($entidadeDirigente[0]["entnome"]) ? $entidadeDirigente[0]["entnome"] : "NÃO CADASTRADO" ?></font></td>
	</tr>
</table>
<br />
<table class="tabelaRelatorio" border="0" width="100%" align="center" cellspacing="0" cellpadding="2">
	<tr>
		<td align="left" class="tituloRelatorio2" style="border-bottom:0px;" colspan="17"><b>LISTAGEM DAS ESCOLAS E ATIVIDADES</b></td>
	</tr>
</table>
<br />

<table class="tabelaRelatorio" border="0" width="100%" align="center" cellspacing="0" cellpadding="2">
	<tr>
		<td align="center" style="width:2%;font-weight:bold;font-size:9px;"  class="tituloRelatorio2 bordaDireitaBaixo" rowspan="2" >N°Ord.</td>
		<td align="center" style="width:5%;font-weight:bold;font-size:9px;"  class="tituloRelatorio2 bordaDireitaBaixo" rowspan="2" >Cod.da Escola Censo(INEP)</td>
		<td align="center" style="width:25%;font-weight:bold;font-size:9px;" class="tituloRelatorio2 bordaDireitaBaixo" rowspan="2" >Nomes das Escolas</td>
		<td align="center" style="width:8%;font-weight:bold;font-size:9px;"  class="tituloRelatorio2 bordaDireitaBaixo" colspan="2" >Dias de Funcionamento</td>
		<td align="center" style="width:40%;font-weight:bold;font-size:9px;" class="tituloRelatorio2 bordaDireitaBaixo" colspan="5" >Quantidade de Atividades Planejadas por Área Temática</td>
		<td align="center" style="width:5%;font-weight:bold;font-size:9px;"  class="tituloRelatorio2 bordaDireitaBaixo" colspan="2" >Ressarcimento PDDE-FEFS(Indicar a quantidade)</td>
		<td align="center" style="width:5%;font-weight:bold;font-size:9px;"  class="tituloRelatorio2 bordaDireitaBaixo" colspan="2" >UEx Central</td>
		<td align="center" style="width:5%;font-weight:bold;font-size:9px;"  class="tituloRelatorio2 bordaDireitaBaixo" rowspan="2" >Participantes(Indicar a quantidade)</td>
		<td align="center" style="width:5%;font-weight:bold;font-size:9px;"  class="tituloRelatorio2 bordaDireitaBaixo" rowspan="2" >Funcionamento Anterior a 2011/2012</td>
	</tr>
	<tr>
		<td align="center" style="width:2.5%;font-size:9px;" class=" bordaDireitaBaixo" >01(um dia)</td>
		<td align="center" style="width:2.5%;font-size:9px;" class=" bordaDireitaBaixo" >02(dois dias)</td>
		<td align="center" style="width:10%; font-size:9px;" class=" bordaDireitaBaixo" >(A)Cultura e Arte</td>
		<td align="center" style="width:15%; font-size:9px;" class=" bordaDireitaBaixo" >(B)Esporte/Lazer/Recreação </td>
		<td align="center" style="width:15%; font-size:9px;" class=" bordaDireitaBaixo" >(C)Qualificação para o Trabalho/Geração de Renda)</td>
		<td align="center" style="width:15%; font-size:9px;" class=" bordaDireitaBaixo" >(D)Formação Educativa Complementar</td>
		<td align="center" style="width:10%; font-size:9px;" class=" bordaDireitaBaixo" >Total</td>
		<td align="center" style="width:5%;  font-size:9px;" class=" bordaDireitaBaixo" >SIM</td>
		<td align="center" style="width:5%;  font-size:9px;" class=" bordaDireitaBaixo" >NÃO</td>
		<td align="center" style="width:10%; font-size:9px;" class=" bordaDireitaBaixo" >UEx Central</td>
		<td align="center" style="width:10%; font-size:9px;" class=" bordaDireitaBaixo" >Indicar Quantas Escola Atende </td>
		
	</tr>

<?php

$culturaArteGeral = 0;
$esporteLazerGeral = 0;
$qualificacaoTrabalhoGeral = 0;
$formacaoEducativaGeral = 0;

$qtdParticipanteGeral = 0;
$qtdRessarcimentoSimGeral = 0;
$qtdRessarcimentoNaoGeral = 0;

$qtdFuncionamento1dia = 0;
$qtdFuncionamento2dias = 0;
$qtdUexCentralGeral = 0;
$qtdEscolasUexGeral = 0;
$qtdFuncionamentoAnteriorGeral = 0;
$somaX = 0;

for($i=0; $i<count($escolas); $i++)
{
	$sql = "SELECT
				eab.entcodent as codigo_escola,
				ent.entnome as nome_escola,
				eab.edrid as dia_funcionamento,
				eab.eabqtduexatende as uex_central,
				eab.eabanoatual as funcionamento_ano_anterior
			FROM
				pdeescola.eabescolaaberta eab
			INNER JOIN
				entidade.entidade ent ON ent.entid = eab.entid
			WHERE
				eab.eabid = ".$escolas[$i];
	$eabEscola = $db->carregar($sql);
	
	if( $eabEscola )
	{
		$sql = "SELECT
					eaa.eatid as tipo_atividade,
					eaa.eaaqtdparticipante as qtd_participante,
					eaa.eabressarcida as ressarcimento
				FROM
					pdeescola.eabatividade eaa
				WHERE
					eaa.eabid = ".$escolas[$i];
		$eabAtividades = $db->carregar($sql);
						
		$culturaArte = 0;
		$esporteLazer = 0;
		$qualificacaoTrabalho = 0;
		$formacaoEducativa = 0;
		
		$qtdParticipante = 0;
		$qtdRessarcimentoSim = 0;
		$qtdRessarcimentoNao = 0;
		
		
		if( $eabAtividades )
		{
			for($j=0; $j<count($eabAtividades); $j++)
			{
				switch( $eabAtividades[$j]['tipo_atividade'] )
				{
					case '1':
						$culturaArte++;
						break;
					case '2':
						$esporteLazer++;
						break;
					case '3':
						$qualificacaoTrabalho++;
						break;
					case '4':
						$formacaoEducativa++;
						break;
				}
				
				$qtdParticipante += $eabAtividades[$j]['qtd_participante'];
				
				if( $eabAtividades[$j]['ressarcimento'] == 't' )
				{
					$qtdRessarcimentoSim++;
				}
				else
				{
					$qtdRessarcimentoNao++;
				}
			}
		}
		$culturaArteGeral += $culturaArte;
		$esporteLazerGeral += $esporteLazer;
		$qualificacaoTrabalhoGeral += $qualificacaoTrabalho;
		$formacaoEducativaGeral += $formacaoEducativa;
		
		$qtdParticipanteGeral += $qtdParticipante;
		$qtdRessarcimentoSimGeral += $qtdRessarcimentoSim;
		$qtdRessarcimentoNaoGeral += $qtdRessarcimentoNao;
		
		if($eabEscola[0]['dia_funcionamento']==1 OR $eabEscola[0]['dia_funcionamento']==2){
			$qtdFuncionamento1dia ++;
		}
		if($eabEscola[0]['dia_funcionamento']==3){
			$qtdFuncionamento2dias ++;
		}
		if($eabEscola[0]['uex_central']){
			$qtdEscolasUexGeral += $eabEscola[0]['uex_central'];
			$qtdUexCentralGeral++;
		}
		
		$sql = "SELECT
						eaa.eaaid
					FROM
						pdeescola.eabtipoanoanterior eaa
					INNER JOIN
						pdeescola.eabescolaanoanterior eea ON eea.eaaid = eaa.eaaid
														  AND eea.eabid = ".$escolas[$i]."
					WHERE
						eaa.eaastatus = 'A'";
			$anosEab = $db->carregarColuna($sql);
			$anosEab = $anosEab ? $anosEab : array();
			$boMarcaX = false;
			if( !empty($anosEab) ){
				$boMarcaX = true;
				$somaX++;
			}
			
echo"<tr>
		<td align='center' style='font-size:9px;' class='bordaDireitaBaixo'>".($i+1)."</td>
		<td align='center' style='font-size:9px;' class='bordaDireitaBaixo'>".$eabEscola[0]['codigo_escola']."</td>
		<td align='center' style='font-size:9px;' class='bordaDireitaBaixo'>".$eabEscola[0]['nome_escola']."</td>
		<td align='center' style='font-size:9px;' class='bordaDireitaBaixo'>".(($eabEscola[0]['dia_funcionamento']==1 || $eabEscola[0]['dia_funcionamento']==2) ? 'X' : '&nbsp;')."</td>
		<td align='center' style='font-size:9px;' class='bordaDireitaBaixo'>".(($eabEscola[0]['dia_funcionamento']==3) ? 'X' : '&nbsp;')."</td>
		<td align='center' style='font-size:9px;' class='bordaDireitaBaixo'>".$culturaArte."</td>
		<td align='center' style='font-size:9px;' class='bordaDireitaBaixo'>".$esporteLazer."</td>
		<td align='center' style='font-size:9px;' class='bordaDireitaBaixo'>".$qualificacaoTrabalho."</td>
		<td align='center' style='font-size:9px;' class='bordaDireitaBaixo'>".$formacaoEducativa."</td>
		<td align='center' style='font-size:9px;' class='bordaDireitaBaixo'>".($culturaArte + $esporteLazer + $qualificacaoTrabalho + $formacaoEducativa)."</td>
		<td align='center' style='font-size:9px;' class='bordaDireitaBaixo'>".$qtdRessarcimentoSim."</td>
		<td align='center' style='font-size:9px;' class='bordaDireitaBaixo'>".$qtdRessarcimentoNao."</td>
		<td align='center' style='font-size:9px;' class='bordaDireitaBaixo'>".($eabEscola[0]['uex_central'] ? 'X' : '&nbsp;')."</td>
		<td align='center' style='font-size:9px;' class='bordaDireitaBaixo'>".($eabEscola[0]['uex_central'] ? $eabEscola[0]['uex_central'] : '&nbsp;')."</td>
		<td align='center' style='font-size:9px;' class='bordaDireitaBaixo'>".$qtdParticipante."</td>
		<td align='center' style='font-size:9px;' class='bordaDireitaBaixo'>".( $boMarcaX ? 'X' : '&nbsp;')."</td>
	</tr>";
	}
}

echo"<tr>
		<td align='center' style='font-size:9px;text-align:right;font-weight:bold;' class='bordaDireitaBaixo' colspan='3'>TOTAL:</td>
		<td align='center' style='font-size:9px;' class='bordaDireitaBaixo'>".$qtdFuncionamento1dia."</td>
		<td align='center' style='font-size:9px;' class='bordaDireitaBaixo'>".$qtdFuncionamento2dias."</td>
		<td align='center' style='font-size:9px;' class='bordaDireitaBaixo'>".$culturaArteGeral."</td>
		<td align='center' style='font-size:9px;' class='bordaDireitaBaixo'>".$esporteLazerGeral."</td>
		<td align='center' style='font-size:9px;' class='bordaDireitaBaixo'>".$qualificacaoTrabalhoGeral."</td>
		<td align='center' style='font-size:9px;' class='bordaDireitaBaixo'>".$formacaoEducativaGeral."</td>
		<td align='center' style='font-size:9px;' class='bordaDireitaBaixo'>".($culturaArteGeral + $esporteLazerGeral + $qualificacaoTrabalhoGeral + $formacaoEducativaGeral)."</td>
		<td align='center' style='font-size:9px;' class='bordaDireitaBaixo'>".$qtdRessarcimentoSimGeral."</td>
		<td align='center' style='font-size:9px;' class='bordaDireitaBaixo'>".$qtdRessarcimentoNaoGeral."</td>
		<td align='center' style='font-size:9px;' class='bordaDireitaBaixo'>".$qtdUexCentralGeral."</td>
		<td align='center' style='font-size:9px;' class='bordaDireitaBaixo'>".$qtdEscolasUexGeral."</td>
		<td align='center' style='font-size:9px;' class='bordaDireitaBaixo'>".$qtdParticipanteGeral	."</td>
		<td align='center' style='font-size:9px;' class='bordaDireitaBaixo'>".$somaX."</td>
	</tr>";

echo
"</table>

<br />

<table class=\"tabelaRelatorio\" border=\"0\" width=\"100%\" align=\"center\" cellspacing=\"0\" cellpadding=\"2\">
	<tr>
		<td align=\"left\" colspan=\"5\" class=\"tituloRelatorio2\"><b>AUTENTICAÇÃO</b></td>
	</tr>
	<tr valign=\"bottom\" style=\"height:80px;\">
		<td align=\"center\" class=\"bordaDireitaBaixo\">
		________________________________________________________________
		<br />
		LOCAL E DATA
		</td>
		<td align=\"center\" class=\"bordaBaixo\">
		___________________________________________________________________________________________________________
		<br />
		NOME DO DIRIGENTE DA EEx
		<br />
		<br />
		<br />
		___________________________________________________________________________________________________________
		<br />
		ASSINATURA DO DIRIGENTE EEx
	</tr>
	<tr>
		<td align=\"left\" colspan=\"5\" class=\"tituloRelatorio2\"><b>APROVAÇÃO DO ANALISTA DA SEB</b></td>
	</tr>
	<tr valign=\"bottom\" style=\"height:80px;\">
		<td align=\"center\" class=\"bordaDireitaBaixo\">
		________________________________________________________________		
		<br />
		LOCAL E DATA
		</td>
		<td align=\"center\" class=\"bordaBaixo\">
		___________________________________________________________________________________________________________
		<br />
		NOME DO ANALISTA
		<br />
		<br />
		<br />
		___________________________________________________________________________________________________________
		<br />
		ASSINATURA DO ANALISTA
	</tr>
	<tr>
		<td align=\"left\" colspan=\"5\" class=\"tituloRelatorio2\"><b>APROVAÇÃO DO DIRIGENTE SEB</b></td>
	</tr>
	<tr valign=\"bottom\" style=\"height:80px;\">
		<td align=\"center\" class=\"bordaDireitaBaixo\">
		________________________________________________________________
		<br />
		LOCAL E DATA
		</td>
		<td align=\"center\" class=\"bordaBaixo\">
		__________________________________________________________________________________________________________
		<br />
		NOME DO DIRIGENTE 
		<br />
		<br />
		<br />
		___________________________________________________________________________________________________________
		<br />
		ASSINATURA DO DIRIGENTE
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
</table>