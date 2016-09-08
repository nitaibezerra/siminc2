<?

if($_REQUEST['req'] == 'adesao'){
	include APPRAIZ."pdeescola/modulos/meprincipal/meAdesaoConsolidado.inc";	
	die;
}

ini_set( "memory_limit", "512M" );
set_time_limit(0);

include_once APPRAIZ . 'includes/workflow.php';

$msgPagamentoNaoRecebido = "";

if( isset($_REQUEST["lista_escolas"]) )
{
	$listaEscolas = $db->carregarColuna("SELECT entid FROM pdeescola.meloteimpressao WHERE mlicodigolote in (".$_REQUEST["lista_escolas"].") ORDER BY mliid");
	if($listaEscolas){
		$listaEscolas = " AND e.entid in (" . implode(',',$listaEscolas) .")";
	}
	else{
		$listaEscolas = "";
	}
}
else
{
	$listaEscolas = "";
}

// Recupera o perfil do usuário no Mais Educação.
$usuPerfil = arrayPerfil();

// Se for só visualização, recupera as entidades com estado 'relatório emitido', senão pega os 'finalizados'.
$esdid = (!isset($_REQUEST["visualizar"])) ? ' = 34 ' : ' in (39,40) ';

if(in_array(PDEESC_PERFIL_ADMINISTRADOR_MAIS_EDUCACAO, $usuPerfil) || in_array(PDEESC_PERFIL_SUPER_USUARIO, $usuPerfil)) {
	if(isset($_REQUEST["muncod"]) && $_REQUEST["muncod"] != "") {
		$funid = 1;

		$whereEntExecutora = "en.muncod = '".$_REQUEST["muncod"]."'";

		$whereEntDirigente = "funprefeito.funid = 2 AND
				              funprefeitura.funid = 1 AND 
				              mun.muncod ='".$_REQUEST["muncod"]."'";

		$whereEntidades = "m.muncod = '".$_REQUEST["muncod"]."' AND
						   e.tpcid IN (3) AND 
						   est.esdid ".$esdid." 
						   ".$listaEscolas."";
	}else
	if(isset($_REQUEST["estuf"]) && $_REQUEST["estuf"] != "") {
		$funid = 6;

		$whereEntExecutora = "en.estuf = '".$_REQUEST["estuf"]."'";

		$whereEntDirigente = "funprefeito.funid = 25 AND
	            			  funprefeitura.funid = 6 AND 
			            	  mun.estuf ='".$_REQUEST["estuf"]."'";

		$whereEntidades = "m.estuf = '".$_REQUEST["estuf"]."' AND
						   e.tpcid IN (1) AND 
						   est.esdid ".$esdid." 
						   ".$listaEscolas."";
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
				".$whereEntExecutora." and entstatus = 'A'";
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

	$sql = "SELECT DISTINCT
				maedu.memid,
				e.entid,
			 	e.entcodent,
			 	e.entnome,
			 	maedu.memid,
			 	est.esddsc,
				maedu.memsaldo
			FROM
				entidade.entidade e
			INNER JOIN 
				entidade.endereco ee ON e.entid = ee.entid
			INNER JOIN 
				entidade.endereco endi ON endi.endid = ee.endid
			INNER JOIN 
				territorios.municipio m ON m.muncod = endi.muncod
			INNER JOIN 
				pdeescola.memaiseducacao maedu ON maedu.entid = e.entid 
										      AND maedu.memanoreferencia = " . $_SESSION["exercicio"] . "
										      AND maedu.memstatus = 'A'
			LEFT JOIN 
				workflow.documento d ON d.docid = maedu.docid
			LEFT JOIN 
				workflow.estadodocumento est ON est.esdid = d.esdid
			WHERE
				".$whereEntidades;
	$entidades = $db->carregar($sql);
}
else if(in_array(PDEESC_PERFIL_SEC_MUNICIPAL_MAIS_EDUCACAO, $usuPerfil)) {
	$sql = "SELECT * FROM pdeescola.usuarioresponsabilidade WHERE usucpf = '".$_SESSION["usucpf"]."' AND rpustatus = 'A' AND pflcod = ".PDEESC_PERFIL_SEC_MUNICIPAL_MAIS_EDUCACAO;
	$pflcod = $db->carregar($sql);

	if($listaEscolas == "") {
		$sql = "SELECT memid FROM pdeescola.meloteimpressao WHERE muncod = '".$pflcod[0]["muncod"]."'";
		$memids = $db->carregar($sql);

		if($memids) {
			$restricaoLote = array();
			for($i=0; $i<count($memids); $i++) {
				$restricaoLote[$i] = $memids[$i]["memid"];
			}
			$restricaoLote = " AND maedu.memid not in (".implode(',', $restricaoLote).")";
		} else {
			$restricaoLote = "";
		}
	} else {
		$restricaoLote = "";
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
											   fue.funid = 1
			INNER JOIN
				territorios.municipio mun ON mun.muncod = en.muncod
			WHERE
				en.muncod = '".$pflcod[0]["muncod"]."'";
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
            	funprefeito.funid = 2 AND 
            	funprefeitura.funid = 1 AND 
            	mun.muncod ='".$pflcod[0]["muncod"]."' AND
				entprefeito.entstatus = 'A' AND
            	entprefeitura.entstatus = 'A'";
	 
	$entidadeDirigente = $db->carregar($sql);

	$sql = "SELECT DISTINCT
				maedu.memid,
				e.entid,
			 	e.entcodent,
			 	e.entnome,
			 	maedu.memid,
			 	est.esddsc,
				maedu.memsaldo
			FROM
				entidade.entidade e
			INNER JOIN 
				entidade.endereco ee ON e.entid = ee.entid
			INNER JOIN 
				entidade.endereco endi ON endi.endid = ee.endid
			INNER JOIN 
				territorios.municipio m ON m.muncod = endi.muncod
			INNER JOIN 
				pdeescola.memaiseducacao maedu ON maedu.entid = e.entid 
											  AND maedu.memanoreferencia = " . $_SESSION["exercicio"] . " 
											  AND maedu.memstatus = 'A'
			LEFT JOIN 
				workflow.documento d ON d.docid = maedu.docid
			LEFT JOIN 
				workflow.estadodocumento est ON est.esdid = d.esdid
			WHERE
				m.muncod = '".$pflcod[0]["muncod"]."' AND 
				e.tpcid IN (3) AND
				est.esdid ".$esdid."
				".$listaEscolas." 
				".$restricaoLote."";
	//dbg($sql,1);
	$entidades = $db->carregar($sql);
}
else if(in_array(PDEESC_PERFIL_SEC_ESTADUAL_MAIS_EDUCACAO, $usuPerfil)) {
	$sql = "SELECT * FROM pdeescola.usuarioresponsabilidade WHERE usucpf = '".$_SESSION["usucpf"]."' AND rpustatus = 'A' AND pflcod = ".PDEESC_PERFIL_SEC_ESTADUAL_MAIS_EDUCACAO;
	$pflcod = $db->carregar($sql);

	if($listaEscolas == "") {
		$sql = "SELECT memid FROM pdeescola.meloteimpressao WHERE estuf = '".$pflcod[0]["estuf"]."'";
		$memids = $db->carregar($sql);

		if($memids) {
			$restricaoLote = array();
			for($i=0; $i<count($memids); $i++) {
				$restricaoLote[$i] = $memids[$i]["memid"];
			}
			$restricaoLote = " AND maedu.memid not in (".implode(',', $restricaoLote).")";
		} else {
			$restricaoLote = "";
		}
	} else {
		$restricaoLote = "";
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
											   fue.funid = 6
			INNER JOIN
				territorios.municipio mun ON mun.muncod = en.muncod
			WHERE
				en.estuf = '".$pflcod[0]["estuf"]."'";
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
            	funprefeito.funid = 25 AND 
            	funprefeitura.funid = 6 AND 
            	mun.estuf ='".$pflcod[0]["estuf"]."' AND
            	entprefeito.entstatus = 'A' AND
            	entprefeitura.entstatus = 'A'";
	$entidadeDirigente = $db->carregar($sql);

	$sql = "SELECT DISTINCT
				maedu.memid,
				e.entid,
			 	e.entcodent,
			 	e.entnome,
			 	maedu.memid,
			 	est.esddsc,
				maedu.memsaldo
			FROM
				entidade.entidade e
			INNER JOIN 
				entidade.endereco ee ON e.entid = ee.entid
			INNER JOIN 
				entidade.endereco endi ON endi.endid = ee.endid
			INNER JOIN 
				territorios.municipio m ON m.muncod = endi.muncod
			INNER JOIN 
				pdeescola.memaiseducacao maedu ON maedu.entid = e.entid 
											  AND maedu.memanoreferencia = " . $_SESSION["exercicio"] . "
											  AND maedu.memstatus = 'A'
			LEFT JOIN 
				workflow.documento d ON d.docid = maedu.docid
			LEFT JOIN 
				workflow.estadodocumento est ON est.esdid = d.esdid
			WHERE
				m.estuf = '".$pflcod[0]["estuf"]."' AND
				e.tpcid IN (1) AND
				est.esdid ".$esdid." 
				".$listaEscolas."
				".$restricaoLote."";
	$entidades = $db->carregar($sql);
}
else {
	echo "<script>
			alert('Não há nenhum Munícipio ou Estado associado ao seu perfil.');
			window.close();
		  </script>";
	exit;
}

// função recupera as informações referentes ao pagamento a ser realizado
function recuperaDadosPagamento($memid, $entid, $ano, &$kitCusteio, &$kitCapital, &$ressarcimentoMonitores, &$servicosMateriais, &$valorLimite, &$pagamentoProfessorPST, &$msgPagamentoNaoRecebido) {
	global $db;

	$msgPagamentoNaoRecebido = "";
	$numAtividadesCalculadas = 6;

	/*** INICIO - Regras para cálculo do pagamento de escolas 2010/2009 - INICIO ***/
	if((integer)$ano == 2010) {

		// verifica se a escola participou em 2009
		$dadosEscola2009 = $db->carregar("SELECT * FROM pdeescola.memaiseducacao WHERE entid = ".$entid." AND memstatus = 'A' AND memanoreferencia = 2009");

		// escola de 2010
		if(!$dadosEscola2009) {
			// Quantidade de meses
			$meses = 10;
			// Chama a função para o cálculo
			calculoKitsRessarcimento($memid, $ano, $kitCusteio, $kitCapital, $ressarcimentoMonitores, $pagamentoProfessorPST, $meses);
			$valorLimite = 999999;
			// Chama a função para cálculo de Serviços e Materiais
			calculoServicosMateriais($memid, $ano, $servicosMateriais, $meses, $calculoServicosMateriaisCapital, $calculoServicosMateriaisCusteio);
		}
		// participou em 2009
		else {
			// não foi paga em 2009
			if( $dadosEscola2009[0]["mempagofnde"] == 'f' )
			{
				// Quantidade de meses
				$meses = 10;

				/*** Atividades de 2010 ***/
				// Chama a função para o cálculo
				calculoKitsRessarcimento($memid, $ano, $kitCusteio, $kitCapital, $ressarcimentoMonitores, $pagamentoProfessorPST, $meses);
				$valorLimite = 999999;
				// Chama a função para cálculo de Serviços e Materiais
				calculoServicosMateriais($memid, $ano, $servicosMateriais, $meses, $calculoServicosMateriaisCapital, $calculoServicosMateriaisCusteio);
			}
			// foi feito o pagamento em 2009
			else {
				$iniciouAnoAnterior = $db->pegaUm("SELECT count(*) FROM pdeescola.meatividade WHERE memid = ".$dadosEscola2009[0]["memid"]." AND meacomecounoano = 't'");

				// Escola iniciou as atividades
				if((integer)$iniciouAnoAnterior > 0) {
					// Quantidade de meses
					$meses = 10;
					// Chama a função para o cálculo
					calculoKitsRessarcimento($memid, $ano, $kitCusteio, $kitCapital, $ressarcimentoMonitores, $pagamentoProfessorPST, $meses);
					$valorLimite = 999999;
					// Chama a função para cálculo de Serviços e Materiais
					calculoServicosMateriais($memid, $ano, $servicosMateriais, $meses, $calculoServicosMateriaisCapital, $calculoServicosMateriaisCusteio);
				}
				// Escola não iniciou as atividades
				else {
					// Quantidade de meses
					$meses = 4;
					/*** Atividades de 2009 ***/
					// Chama a função para o cálculo
					calculoKitsRessarcimento($dadosEscola2009[0]["memid"], 2009, $kitCusteio, $kitCapital, $ressarcimentoMonitores, $pagamentoProfessorPST, $meses, true, true);
					// Chama a função para cálculo do valor limite
					calculoValorLimite($dadosEscola2009[0]["memid"], 2009, $numAtividadesCalculadas, $valorLimite, $meses);
					// Chama a função para cálculo de Serviços e Materiais
					calculoServicosMateriais($dadosEscola2009[0]["memid"], 2009, $servicosMateriais, $meses, $calculoServicosMateriaisCapital, $calculoServicosMateriaisCusteio);
						
					// Quantidade de meses
					$meses = 10;
					/*** Atividades de 2010 ***/
					// Chama a função para o cálculo
					calculoKitsRessarcimento($memid, $ano, $kitCusteio, $kitCapital, $ressarcimentoMonitores, $pagamentoProfessorPST, $meses);
					// Chama a função para cálculo do valor limite
					calculoValorLimite($memid, $ano, $numAtividadesCalculadas, $valorLimite, $meses);
				}
			}
		}
	}
	// Escolas de 2008 e 2009
	else {
		$flag = true;
		$dadosEscolaAnoAnterior = $db->carregar("SELECT memid,mempagofnde FROM pdeescola.memaiseducacao WHERE entid = ".$entid." AND memstatus = 'A' AND memanoreferencia = ".((integer)$ano - 1));

		if($dadosEscolaAnoAnterior)
		{
			$temAtividades = $db->pegaUm("SELECT count(*) FROM pdeescola.meatividade WHERE memid = ".$dadosEscolaAnoAnterior[0]['memid']."");

			if( $temAtividades > 0 )
			{
				if( $temAtividades != $db->pegaUm("SELECT count(*) FROM pdeescola.meatividade WHERE memid = ".$dadosEscolaAnoAnterior[0]['memid']." AND meacomecounoano is null") )
				{
					$iniciouAnoAnterior = $db->pegaUm("SELECT count(*) FROM pdeescola.meatividade WHERE memid = ".$dadosEscolaAnoAnterior[0]['memid']." AND meacomecounoano = 't'");

					if( (integer)$iniciouAnoAnterior == 0 && $dadosEscolaAnoAnterior[0]['mempagofnde'] == 't' )
					{
						// não paga nada
						$msgPagamentoNaoRecebido = "As escolas que receberam recursos no exercício de ".($ano-1)." para o desenvolvimento do plano de atendimento e não o executaram, <br />não poderão apresentar novo plano de atendimento para o exercício de ".$ano." antes da conclusão do plano anterior.";
						$flag = false;
					}
				}
			}
		}

		if($flag)
		{
			// Quantidade de meses usada nos anos de 2008 e 2009
			$meses = 6;
			
			// Chama a função para o cálculo
			calculoKitsRessarcimento($memid, $ano, $kitCusteio, $kitCapital, $ressarcimentoMonitores, $pagamentoProfessorPST, $meses);

			if((integer)$ano == 2012) {
				if($pagamentoProfessorPST > 0){
					$ressarcimentoMonitores += $pagamentoProfessorPST;
					$pagamentoProfessorPST = 0;
				}
			}
			
			// Chama a função para cálculo de Serviços e Materiais
			calculoServicosMateriais($memid, $ano, $servicosMateriais, $meses, $calculoServicosMateriaisCapital, $calculoServicosMateriaisCusteio);
		}
	}
	/*** FIM - Regras para cálculo do pagamento de escolas 2010/2009 - FIM ***/
	if($calculoServicosMateriaisCapital || $calculoServicosMateriaisCusteio){
		
		$servicosMateriais = $calculoServicosMateriaisCapital+$calculoServicosMateriaisCusteio;

		$kitCusteioTemp = $kitCusteio;
		$kitCapitalTemp = $kitCapital;
		
		$kitCusteio = $ressarcimentoMonitores+$calculoServicosMateriaisCusteio+$kitCusteio;
		$kitCapital = $kitCapital+$calculoServicosMateriaisCapital+$escolaAbertaCapital;

		$sql = "select memjovem1517, mamescolaaberta, mempeif from pdeescola.memaiseducacao  where memid = {$memid}";
		$rsAtividades = $db->pegaLinha($sql);
		
		if($rsAtividades['memjovem1517'] == 't'){
			
			$calculoJovem15a17 = 0;
			
			calculoJovem15a17($memid, $ano, $meses, $calculoJovem15a17);
			
			$jovem1517custeio = $calculoJovem15a17>0 ? 5000 : 0;
			$jovem1517capital = $calculoJovem15a17>0 ? 2000 : 0;
			
			$kitCusteio = $kitCusteio+$calculoJovem15a17+$jovem1517custeio;
			$kitCapital = $kitCapital+$jovem1517capital;

		}
		if($rsAtividades['mamescolaaberta'] == 't'){
			
			$escolaAbertaCapital = 0;
			$escolaAbertaCusteio = 0;
			
			calculoEscolaAberta($memid, $ano, $escolaAbertaCapital, $escolaAbertaCusteio, $meses);
			
			$kitCusteio = $kitCusteio+$escolaAbertaCusteio;
			$kitCapital = $kitCapital+$escolaAbertaCapital;
				
		}
		if($rsAtividades['mempeif'] == 't'){
			
			$peifCusteio = 0;
			$peifCapital = 0;
			
			calculoPeif($memid, $peifCusteio, $peifCapital, $ano);
		}
		
	}
}


if($entidades) {
	?>

<link
	rel="stylesheet" type="text/css" href="../includes/Estilo.css" />
<style>
table.tabelaRelatorio {
	border: 1px solid #000000;
	font-size: 8px;
}

td.tituloRelatorio {
	background-color: #c0c0c0;
	border-top: 1px solid #000000;
	border-bottom: 1px solid #000000;
	font-size: 8px;
}

td.tituloRelatorio2 {
	background-color: #c0c0c0;
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
<table width="100%" border="0" cellpadding="0" cellspacing="0">
	<tr>
		<td valign="top" width="50" rowspan="2"><img
			src="../imagens/brasao.gif" width="45" height="45" border="0"></td>
		<td nowrap align="left" valign="top" height="1"
			style="padding: 5px 0 0 0;"><font style="font-size: 11px;"> MEC -
		Ministério da Educação<br />
		SIMEC - Sistema Integrado de Monitoramento Execução e Controle<br />
		PDDE EDUCAÇÃO INTEGRAL </font></td>
		<td align="right" valign="top" height="1" style="padding: 5px 0 0 0;">
		<font style="font-size: 11px;"> Impresso por: <b><?= $_SESSION['usunome'] ?></b><br />
		Data/Hora da Impressão: <?= date( 'd/m/Y - H:i:s' ) ?></b><br />
		</font></td>
	
	
	<tr>
		<td>&nbsp;</td>
	</tr>
	</tr>
</table>
<table class="tabelaRelatorio" border="0" width="100%" align="center"
	cellspacing="0" cellpadding="2">
	<tr>
		<td align="center" colspan="8"><font size="3"><b>PLANO DE ATENDIMENTO
		GERAL CONSOLIDADO - <?= $_SESSION["exercicio"] ?></b></td>
	</tr>
	<tr>
		<td align="left" colspan="8" class="tituloRelatorio"><b>ENTIDADE
		EXECUTORA</b></td>
	</tr>
	<tr>
		<td align="left" colspan="1" class="bordaDireita">CNPJ<br />
		<?= ($entidadeExecutora[0]["entnumcpfcnpj"]) ? ($entidadeExecutora[0]["entnumcpfcnpj"]) : "NÃO CADASTRADO" ?></td>
		<td align="left" colspan="3" class="bordaDireita">NOME DA ENTIDADE<br />
		<?= ($entidadeExecutora[0]["entnome"]) ? $entidadeExecutora[0]["entnome"] : "NÃO CADASTRADO" ?></td>
		<td align="left" colspan="3" class="bordaDireita">MUNICÍPIO<br />
		<?= ($entidadeExecutora[0]["mundescricao"]) ? $entidadeExecutora[0]["mundescricao"] : "NÃO CADASTRADO" ?></td>
		<td align="left" colspan="1"><font style="font-size: 8px;">UF<br />
		<?= ($entidadeExecutora[0]["estuf"]) ? $entidadeExecutora[0]["estuf"] : "NÃO CADASTRADO" ?></font></td>
	</tr>
	<tr>
		<td align="left" colspan="8" class="tituloRelatorio"><b>DIRIGENTE DA
		ENTIDADE</b></td>
	</tr>
	<tr>
		<td align="left" colspan="1" class="bordaDireita">CPF<br />
		<?= ($entidadeDirigente[0]["entnumcpfcnpj"]) ? $entidadeDirigente[0]["entnumcpfcnpj"] : "NÃO CADASTRADO" ?></td>
		<td align="left" colspan="7"><font style="font-size: 8px;">NOME DO
		DIRIGENTE<br />
		<?= ($entidadeDirigente[0]["entnome"]) ? $entidadeDirigente[0]["entnome"] : "NÃO CADASTRADO" ?></font></td>
	</tr>
</table>
<br />
<table class="tabelaRelatorio" border="0" width="100%" align="center"
	cellspacing="0" cellpadding="2">
	<tr>
		<td align="left" class="tituloRelatorio2" colspan="17"><b>LISTAGEM DAS
		ESCOLAS</b></td>
	</tr>
	<?
	if(!isset($_REQUEST["visualizar"])) {
		$codLote = $db->pegaUm("SELECT coalesce(max(mlicodigolote), 0) FROM pdeescola.meloteimpressao");
		$codLote++;
	}

	// totais
	$kitCusteioGeral 			 = 0;
	$kitCapitalGeral 			 = 0;
	$ressarcimentoMonitoresGeral = 0;
	$valorLimiteGeral			 = 0;
	$servicosMateriaisGeral		 = 0;
	$totalGeralGeral			 = 0;
	$qtdGeralAlunosGeral		 = 0;
	$totalGeralSaldo 			 = 0;
	$totalGeralRepasse			 = 0;

	for($i=0; $i<count($entidades); $i++) {
		// variáveis para uso no pagamento de escolas
		$kitCusteio 			= 0;
		$kitCapital 			= 0;
		$ressarcimentoMonitores = 0;
		$valorLimite 			= 0;
		$servicosMateriais 		= 0;
		$pagamentoProfessorPST	= 0;
			
		if(!isset($_REQUEST["visualizar"])) {
			// Recupera o 'docid' da entidade
			$sql = "SELECT docid FROM pdeescola.memaiseducacao WHERE memid = ".$entidades[$i]["memid"];
			$docid = $db->pegaUm($sql);
			// O 'aedid' da ação: finalizado -> relatório emitido. (produção)
			$aedid = 211;
			$aedid = (integer)$aedid;
			// Sem comentário
			$comentario = NULL;
			// Sem dados no array já que não há chamada de função após a ação.
			$dados = array();

			// Realiza a alteração do estado da entidade no ano corrente.
			wf_alterarEstado( $docid, $aedid, $comentario, $dados );

			// Insere as informações sobre o lote de impressão na tabela, dependendo da secretaria (estadual ou municipal).
			if($pflcod[0]["pflcod"] == PDEESC_PERFIL_SEC_MUNICIPAL_MAIS_EDUCACAO) {
				$sqlInsert = "INSERT INTO pdeescola.meloteimpressao(mlidataimpressao, memid, entid, muncod, mlicodigolote) VALUES (now(), ".$entidades[$i]["memid"].", ".$entidades[$i]["entid"].", '".$pflcod[0]["muncod"]."', ".$codLote.")";
			} else {
				$sqlInsert = "INSERT INTO pdeescola.meloteimpressao(mlidataimpressao, memid, entid, estuf, mlicodigolote) VALUES (now(), ".$entidades[$i]["memid"].", ".$entidades[$i]["entid"].", '".$pflcod[0]["estuf"]."', ".$codLote.")";
			}
			$db->executar($sqlInsert);
		}

		// verifica se a escola participou do programa no ano passado
		$existeAnoAnterior = $db->pegaUm("SELECT memid FROM pdeescola.memaiseducacao WHERE entcodent = '".$entidades[$i]["entcodent"]."' AND memstatus = 'A' AND memanoreferencia = ".($_SESSION["exercicio"] - 1)."");

		echo "<tr>
				<td align=\"left\" width=\"10%\" class=\"bordaDireitaBaixo\" rowspan=\"3\" valign=\"top\">REGISTRO<br />#".($i+1)."</td>
				<td align=\"left\" width=\"10%\" class=\"bordaDireitaBaixo\" rowspan=\"3\" valign=\"top\">COD. INEP<br />".$entidades[$i]["entcodent"]."</td>
				<td align=\"left\" width=\"20%\" class=\"bordaDireitaBaixo\" rowspan=\"3\" valign=\"top\">NOME DA ESCOLA<br />".$entidades[$i]["entnome"]."</td>
				<td align=\"left\" width=\"10%\" class=\"bordaDireitaBaixo\" rowspan=\"3\" valign=\"top\">ANO<br />".$_SESSION["exercicio"]."".(($existeAnoAnterior) ? " (participou em ".($_SESSION["exercicio"] - 1).")" : "")."</td>
				<td align=\"left\" width=\"50%\" class=\"bordaBaixo\" colspan=\"13\">ALUNADO PARTICIPANTE</td>
			</tr>
			<tr>
				<td align=\"center\" class=\"bordaDireitaBaixo\">1° ANO</td>
				<td align=\"center\" class=\"bordaDireitaBaixo\">2° ANO</td>
				<td align=\"center\" class=\"bordaDireitaBaixo\">3° ANO</td>
				<td align=\"center\" class=\"bordaDireitaBaixo\">4° ANO</td>
				<td align=\"center\" class=\"bordaDireitaBaixo\">5° ANO</td>
				<td align=\"center\" class=\"bordaDireitaBaixo\">6° ANO</td>
				<td align=\"center\" class=\"bordaDireitaBaixo\">7° ANO</td>
				<td align=\"center\" class=\"bordaDireitaBaixo\">8° ANO</td>
				<td align=\"center\" class=\"bordaDireitaBaixo\">9° ANO</td>
				<td align=\"center\" class=\"bordaDireitaBaixo\">1° ANO</td>
				<td align=\"center\" class=\"bordaDireitaBaixo\">2° ANO</td>
				<td align=\"center\" class=\"bordaDireitaBaixo\">3° ANO</td>
				<td align=\"center\" class=\"bordaBaixo\">TOTAL</td>
			</tr>
			<tr>";

		$modalidadeEnsino = $db->pegaUm("SELECT memmodalidadeensino FROM pdeescola.memaiseducacao WHERE memid = ".$entidades[$i]["memid"]);

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
						map.memid = ".$entidades[$i]["memid"]." AND
						map.mapano = " . $_SESSION["exercicio"] . "
					ORDER BY
						map.sceid";
			$alunadoParticipante = $db->carregar($sql);

			if($modalidadeEnsino == "F") {
				if($j>=0 && $j<=8) {
					echo "<td align=\"center\" class=\"bordaDireitaBaixo\">".($alunadoParticipante[0]["qtd"])."</td>";
					$somaAlunado += (integer)$alunadoParticipante[0]["qtd"];
				}
				else {
					echo "<td align=\"center\" class=\"bordaDireitaBaixo\">-</td>";
				}
			}
			else if($modalidadeEnsino == "M") {
				if($j>=9 && $j<=11) {
					echo "<td align=\"center\" class=\"bordaDireitaBaixo\">".($alunadoParticipante[0]["qtd"])."</td>";
					$somaAlunado += (integer)$alunadoParticipante[0]["qtd"];
				}
				else {
					echo "<td align=\"center\" class=\"bordaDireitaBaixo\">-</td>";
				}
			}
			else {
				echo "<td align=\"center\" class=\"bordaDireitaBaixo\">".($alunadoParticipante[0]["qtd"])."</td>";
				$somaAlunado += (integer)$alunadoParticipante[0]["qtd"];
			}

		}

		echo "<td align=\"center\" class=\"bordaBaixo\">".$somaAlunado."</td>
			</tr>
			<tr>
				<td align=\"left\" class=\"bordaDireitaBaixo\" colspan=\"2\">MACROCAMPO</td>
				<td align=\"left\" class=\"bordaDireitaBaixo\" colspan=\"2\">ATIVIDADE</td>
				<td align=\"center\" class=\"bordaDireitaBaixo\" colspan=\"5\">QUANTIDADE DE ALUNOS POR ATIVIDADE</td>
				<td align=\"center\" class=\"bordaDireitaBaixo\" colspan=\"4\">VALOR KIT (capital)</td>
				<td align=\"center\" class=\"bordaBaixo\" colspan=\"4\">VALOR KIT (custeio)</td>
			</tr>";

		$sql = "SELECT
					mea.meaid,
					mtm.mtmdescricao,
					mta.mtadescricao,
					sum(mta.mtavlrcapital) as capital,
					sum(mta.mtavlrcusteio) as custeio
				FROM
					pdeescola.meatividade mea
				INNER JOIN
					pdeescola.metipoatividade mta ON mta.mtaid = mea.mtaid
								AND mta.mtasituacao = 'A'
				INNER JOIN
					pdeescola.metipomacrocampo mtm ON mtm.mtmid = mta.mtmid
								AND mtm.mtmsituacao = 'A'
				WHERE
					mea.memid = ".$entidades[$i]["memid"]." AND
					mea.meaano = " . $_SESSION["exercicio"] . "
				GROUP BY
					mea.meaid,
					mtm.mtmdescricao,
					mta.mtadescricao
				ORDER BY
					mea.meaid";
		$dadosAtividades = $db->carregar($sql);

		$qtdGeralAlunos  = 0;
		$contAtividades  = 0;
		$qtdAlunosPST	 = 0;
		$kitCapitalPST	 = 0;
		$kitCusteioPST	 = 0;

		if($dadosAtividades) {
			$valorKitCusteio = 0;
			$valorKitCapital = 0;
			for($k=0; $k<count($dadosAtividades); $k++) {

				if($modalidadeEnsino == "F")
				$inSeries = "1,2,3,4,5,6,7,8,9";
				else if($modalidadeEnsino == "M")
				$inSeries = "20,21,22";
				else
				$inSeries = "1,2,3,4,5,6,7,8,9,20,21,22";
					
				$sql = "SELECT
							map.mpaquantidade
						FROM
							pdeescola.mealunoparticipanteatividade map
						WHERE
							map.meaid = ".$dadosAtividades[$k]["meaid"]." AND 
							map.sceid IN (".$inSeries.")
						ORDER BY
							map.sceid";
				$numAlunosAtividade = $db->carregar($sql);

				$qtdAlunos = 0;
				for($z=0; $z<count($numAlunosAtividade); $z++) {
					$qtdAlunos += (integer)$numAlunosAtividade[$z]["mpaquantidade"];
				}
					
				if( strtoupper(trim($dadosAtividades[$k]["mtadescricao"])) != "PST" ) {
					$contAtividades++;
					$qtdGeralAlunos += (integer)$qtdAlunos;
 
					echo "<tr>
							<td align=\"left\" class=\"bordaDireitaBaixo\" colspan=\"2\">".$dadosAtividades[$k]["mtmdescricao"]."</td>
							<td align=\"left\" class=\"bordaDireitaBaixo\" colspan=\"2\">".$dadosAtividades[$k]["mtadescricao"]."</td>
							<td align=\"center\" class=\"bordaDireitaBaixo\" colspan=\"5\">".$qtdAlunos."</td>
							<td align=\"center\" class=\"bordaDireitaBaixo\" colspan=\"4\">".(($dadosAtividades[$k]["capital"]) ? number_format($dadosAtividades[$k]["capital"],"2",",",".") : "0")."</td>
							<td align=\"center\" class=\"bordaBaixo\" colspan=\"4\">".(($dadosAtividades[$k]["custeio"]) ? number_format($dadosAtividades[$k]["custeio"],"2",",",".") : "0")."</td>
						</tr>";
					
					$valorKitCusteio = $dadosAtividades[$k]["custeio"]+$valorKitCusteio;
					$valorKitCapital = $dadosAtividades[$k]["capital"]+$valorKitCapital;
					
				} else {
					$qtdAlunosPST 	= $qtdAlunos;
					$kitCapitalPST	= (($dadosAtividades[$k]["capital"]) ? number_format($dadosAtividades[$k]["capital"],"2",",",".") : "0");
					$kitCusteioPST	= (($dadosAtividades[$k]["custeio"]) ? number_format($dadosAtividades[$k]["custeio"],"2",",",".") : "0");
				}
			}
		}

		// recupera as informações sobre os kits/serviços e materiais/ressarcimento
		recuperaDadosPagamento($entidades[$i]["memid"], $entidades[$i]["entid"], $_SESSION["exercicio"], $kitCusteio, $kitCapital, $ressarcimentoMonitores, $servicosMateriais, $valorLimite, $pagamentoProfessorPST, $msgPagamentoNaoRecebido);

		$valorKits = ($kitCusteio + $kitCapital);
			
		/*** Regra da Clarissa (28/03/2011) ***/
		$porcentagemCusteio = ( $servicosMateriais * 0.8 );
		$porcentagemCapital = ( $servicosMateriais * 0.2 );

		// Calcula os Totais
		$totalCusteio = ($kitCusteio + $pagamentoProfessorPST + $ressarcimentoMonitores + $porcentagemCusteio - $saldoAnoAnterior);

		//$totalCapital = $kitCapital;
		$totalCapital = ($kitCapital + $porcentagemCapital);

		// novo cálculo definido pelo Leandro
		if($totalCusteio < 0) {
			if(abs($totalCusteio) > $totalCapital)
			$totalCusteio = (($totalCapital) * (-1));
		}

		$totalGeral = ($kitCusteio + $kitCapital);		

		if($totalGeral < 0) $totalGeral = 0;

		// soma os totais gerais
		$kitCusteioGeral 			 += $kitCusteio;
		$kitCapitalGeral 			 += $kitCapital;
		$ressarcimentoMonitoresGeral += $ressarcimentoMonitores;
		$servicosMateriaisGeral		 += $servicosMateriais;
		$totalGeralGeral			 += $totalGeral;
		$qtdGeralAlunosGeral		 += $somaAlunado;
		

		echo "<tr>
				<td align=\"right\" class=\"bordaDireitaBaixo\" colspan=\"4\">TOTAL DE ATIVIDADES: ".$contAtividades."</td>
				<td align=\"center\" class=\"bordaDireitaBaixo\" colspan=\"5\">&nbsp;</td>
				<td align=\"center\" class=\"bordaDireitaBaixo\" colspan=\"4\">".number_format($valorKitCapital,"2",",",".")."</td>
				<td align=\"center\" class=\"bordaBaixo\" colspan=\"4\">".number_format($valorKitCusteio,"2",",",".")."</td>
			</tr>";

		if($qtdAlunosPST) {
			echo "<tr>
					<td align=\"center\" class=\"bordaDireitaBaixo\" colspan=\"4\" style=\"font-weight:bold;font-size:9px;\">Pagamento Professor PST: ".number_format($pagamentoProfessorPST,"2",",",".")."</td>
					<td align=\"center\" class=\"bordaDireitaBaixo\" colspan=\"5\">".$qtdAlunosPST."</td>
					<td align=\"center\" class=\"bordaDireitaBaixo\" colspan=\"4\">".number_format($kitCapitalPST,"2",",",".")."</td>
					<td align=\"center\" class=\"bordaBaixo\" colspan=\"4\">".number_format($kitCusteioPST,"2",",",".")."</td>
				</tr>
				<tr>
					<td align=\"right\" class=\"bordaDireitaBaixo\" colspan=\"4\">TOTAL GERAL ATIVIDADES: ".($contAtividades + 1)."</td>
					<td align=\"center\" class=\"bordaDireitaBaixo\" colspan=\"5\">".($qtdGeralAlunos + $qtdAlunosPST)."</td>
					<td align=\"center\" class=\"bordaDireitaBaixo\" colspan=\"4\">".number_format(($kitCapital+$kitCapitalPST),"2",",",".")."</td>
					<td align=\"center\" class=\"bordaBaixo\" colspan=\"4\">".number_format(($kitCusteio+$kitCusteioPST),"2",",",".")."</td>
				</tr>";
		}
		
		$peifCapital = 0; 
		$peifCusteio = 0;
		
		echo '<tr><td colspan="17">';		
		recuperaRelacaoEscolaComunidade($entidades[$i]["memid"]);
		echo '<br/>';
		recuperaJovem15a17anos($entidades[$i]["memid"]);
		echo '<br/>';
		recuperaPeif($entidades[$i]["memid"], $peifCapital, $peifCusteio);
		echo '<br/>';
		echo '</td></tr>';

		$saldoTotalPagar = ($totalGeral - $entidades[$i]["memsaldo"]);
		
		if($saldoTotalPagar < 0) $saldoTotalPagar = 0;
		
		$repasseCusteio = $totalGeral>0 ? (($kitCusteio/$totalGeral)*$saldoTotalPagar)+$peifCusteio : $totalGeral;
		$repasseCapital = $totalGeral>0 ? (($kitCapital/$totalGeral)*$saldoTotalPagar)+$peifCapital : $totalGeral;
		
		$repasseTotal = ($repasseCusteio+$repasseCapital); 
		
		echo "<tr>
				<td align=\"center\" style=\"font-weight:bold;font-size:9px;\" colspan=\"17\">
					***Ressarc. Monitores: ".number_format($ressarcimentoMonitores,"2",",",".")."
					&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;|&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; 
					Serviços/Materiais: ".number_format($servicosMateriais,"2",",",".")."
					&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;|&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
					Total Custeio: ".number_format($kitCusteio,"2",",",".")."
					&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;|&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
					Total Capital: ".number_format($kitCapital,"2",",",".")."
					&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;|&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; 
					Total Geral: ".number_format(($totalGeral),"2",",",".")."
					&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;|&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;	
					Saldo: ".number_format(($entidades[$i]["memsaldo"]),"2",",",".")."
					&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;|&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;	
					*Repasse (custeio): ".number_format(($repasseCusteio),"2",",",".")."
					&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;|&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;	
					**Repasse (capital): ".number_format(($repasseCapital),"2",",",".")."
					&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;|&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;	
					Total do Repasse: ".number_format(($repasseTotal),"2",",",".")."	
				</td>
			</tr>";
		
		$totalGeralRepasse			 += $repasseTotal;
		$totalGeralSaldo			 += $entidades[$i]["memsaldo"];
		
		if( $msgPagamentoNaoRecebido != '' )
		{
			echo "<tr>
					<td align=\"center\" style=\"font-weight:bold;font-size:9px;color:red;\" colspan=\"17\">
						".$msgPagamentoNaoRecebido."
					</td>
				</tr>";
		}
		
		echo "<tr>
				<td class=\"tituloRelatorio\" colspan=\"17\">&nbsp;</td>
			</tr>";

		$entidadesMemidEA[] = $entidades[$i]["memid"];
	}

	echo "</table>";
	
	echo "<br />
			<center>
			<table width='600'>
			<tr><td algin='left'>
				<span style=\"font-weight:bold;font-size:9px;background-color:#e0e0e0;border:1px solid #c0c0c0;\">
				* Fórmula para cálculo do Repasse (custeio): (Total do Repasse x Total Custeio) / Total Geral<br/>
				** Fórmula para cálculo do Repasse (capital): (Total do Repasse x Total Capital) / Total Geral<br/>
				*** Fórmula para cálculo de Ressarc. de Monitores: (n° de alunos / 30 = n° de turmas) x (n° atividades) x (6 meses) x (R$ 80,00)<br/>
				**** Apenas para as escolas que participam pela 1ª vez<br/>
				O cálculo para a atividade 'Orientação de Estudos e Leitura' deve ser feito separadamente e somado ao cálculo de Ressarc. de Monitores: (nº de alunos/15 = nº de turmas)x(6 meses)x(R$ 80,00)
				</span>
			</td></tr>
			</table>
			</center><br><hr>";

	if( in_array($_SESSION["exercicio"], array('2012', '2013')) && false ){ 

		//ESCOLA ABERTA
		if($entidadesMemidEA){

			$sql = "SELECT DISTINCT
							maedu.memid,
							e.entid,
						 	e.entcodent,
						 	e.entnome,
						 	maedu.memanoreferencia
						FROM
							entidade.entidade e
						INNER JOIN 
							pdeescola.memaiseducacao maedu ON maedu.entid = e.entid 
						WHERE
							maedu.memstatus = 'A' AND
							maedu.mamescolaaberta = 't' AND
							maedu.memid in (".implode(',',$entidadesMemidEA).")
						ORDER BY 
							e.entnome";
			$entidadesEA = $db->carregar($sql);
			
			echo "<br>
				
						<table class=\"tabelaRelatorio\" border=\"0\" width=\"100%\" align=\"center\" cellspacing=\"0\" cellpadding=\"2\">
						<tr>
							<td align=\"left\" class=\"tituloRelatorio2\" colspan=\"17\"><b>PROGRAMA ESCOLA ABERTA</b></td>
						</tr>";

			echo "<tr bgcolor=\"#f0f0f0\">
						<td align=\"left\" class=\"bordaDireitaBaixo\" rowspan=\"2\" valign=\"top\">Nº</td>
						<td align=\"left\" class=\"bordaDireitaBaixo\" rowspan=\"2\" valign=\"top\">COD. INEP</td>
						<td align=\"left\" class=\"bordaDireitaBaixo\" rowspan=\"2\" valign=\"top\">NOME DA ESCOLA</td>
						<td align=\"left\" class=\"bordaDireitaBaixo\" rowspan=\"2\" valign=\"top\">Nº DE MATRÍCULAS</td>
						<td align=\"center\" class=\"bordaDireitaBaixo\" colspan=\"5\">QUANTIDADES DE ATIVIDADES PLANEJADAS</td>
						<td align=\"center\" class=\"bordaBaixo\" colspan=\"2\">RECURSOS FINANCEIROS</td>
					</tr>
					<tr bgcolor=\"#f0f0f0\">
						<td align=\"center\" class=\"bordaDireitaBaixo\">(A) CULTURA E ARTE</td>
						<td align=\"center\" class=\"bordaDireitaBaixo\">(B) ESPORTE / LAZER / RECREAÇÃO</td>
						<td align=\"center\" class=\"bordaDireitaBaixo\">(C) QUALIFICAÇÃO PARA O TRABALHO / GERAÇÃO DE RENDA</td>
						<td align=\"center\" class=\"bordaDireitaBaixo\">(D) FORMAÇÃO EDUCATIVA COMPLEMENTAR</td>
						<td align=\"center\" class=\"bordaDireitaBaixo\">TOTAL</td>
						<td align=\"center\" class=\"bordaDireitaBaixo\">CAPITAL****</td>
						<td align=\"center\" class=\"bordaBaixo\">CUSTEIO</td>
					</tr>
					";
			
			if($entidadesEA){
				
				$i=1;
				foreach($entidadesEA as $e){
					
					//pega total de matricula censo
					$sql = "SELECT
								sum(mecquantidadealunos) as total
							FROM
								pdeescola.mecenso
							WHERE
								entid = '".$e['entid']."' AND
								mecanoreferencia = " . $e['memanoreferencia'] . " AND
								mecserie in (1,2,3,4,5,6,7,8,9)";
					$qtdMatriculaCenso = $db->pegaUm($sql);
					if(!$qtdMatriculaCenso) $qtdMatriculaCenso = 0;
					$totalMatriculaCenso += $qtdMatriculaCenso; 
					
					
					//pega total CULTURA E ARTE
					$sql = "SELECT
								count(eaaid) as total
							FROM
								pdeescola.meeabatividade eaa
							WHERE
								eatid = 1 AND
								eaa.memid = ". $e['memid'];
					$qtdC = $db->pegaUm($sql);
					if(!$qtdC) $qtdC = 0;
					$totalC += $qtdC;

					//pega total ESPORTE / LAZER / RECREAÇÃO
					$sql = "SELECT
								count(eaaid) as total
							FROM
								pdeescola.meeabatividade eaa
							WHERE
								eatid = 2 AND
								eaa.memid = ". $e['memid'];
					$qtdE = $db->pegaUm($sql);
					if(!$qtdE) $qtdE = 0;
					$totalE += $qtdE;
					
					//pega total QUALIFICAÇÃO PARA O TRABALHO
					$sql = "SELECT
								count(eaaid) as total
							FROM
								pdeescola.meeabatividade eaa
							WHERE
								eatid = 3 AND
								eaa.memid = ". $e['memid'];
					$qtdQ = $db->pegaUm($sql);
					if(!$qtdQ) $qtdQ = 0;
					$totalQ += $qtdQ;
					
					//pega total FORMAÇÃO EDUCATIVA
					$sql = "SELECT
								count(eaaid) as total
							FROM
								pdeescola.meeabatividade eaa
							WHERE
								eatid = 4 AND
								eaa.memid = ". $e['memid'];
					$qtdF = $db->pegaUm($sql);
					if(!$qtdF) $qtdF = 0;
					$totalF += $qtdF;
					
					$totalLinha = $qtdC + $qtdE + $qtdQ + $qtdF;
					
					//pega os valores capital e custeio
					$meses = 10;
					calculoEscolaAberta($e['memid'], $e['memanoreferencia'], $escolaAbertaCapital, $escolaAbertaCusteio, $meses);
					
					$totalEscolaAbertaCapital += $escolaAbertaCapital;
					$totalEscolaAbertaCusteio += $escolaAbertaCusteio;
					
					echo "<tr>
							<td align=\"center\" class=\"bordaDireitaBaixo\" valign=\"top\">".$i."</td>
							<td align=\"center\" class=\"bordaDireitaBaixo\" valign=\"top\">".$e['entcodent']."</td>
							<td align=\"left\" class=\"bordaDireitaBaixo\" valign=\"top\">".$e['entnome']."</td>
							<td align=\"center\" class=\"bordaDireitaBaixo\" valign=\"top\">".$qtdMatriculaCenso."</td>
							<td align=\"center\" class=\"bordaDireitaBaixo\" >".$qtdC."</td>
							<td align=\"center\" class=\"bordaDireitaBaixo\" >".$qtdE."</td>
							<td align=\"center\" class=\"bordaDireitaBaixo\" >".$qtdQ."</td>
							<td align=\"center\" class=\"bordaDireitaBaixo\" >".$qtdF."</td>
							<td align=\"center\" class=\"bordaDireitaBaixo\" >".$totalLinha."</td>
							<td align=\"center\" class=\"bordaDireitaBaixo\" >".($escolaAbertaCapital ? number_format($escolaAbertaCapital,2,',','.') : '0,00' )."</td>
							<td align=\"center\" class=\"bordaBaixo\">".($escolaAbertaCusteio ? number_format($escolaAbertaCusteio,2,',','.') : '0,00' )."</td>
						  </tr>";
					
					$i++;
				}
				
			}
			else{
				
				echo "<tr>
						<td align=\"center\"  class=\"bordaBaixo\" colspan=\"11\"  valign=\"top\">Não existem registros.</td>
					  </tr>";
			}

			echo "</table>";

		}
		//FIM ESCOLA ABERTA
			
		echo "<br />
			<center>
			<span style=\"font-weight:bold;font-size:9px;background-color:#e0e0e0;border:1px solid #c0c0c0;\">
			*** APENAS AS ESCOLAS NOVAS TERÃO DIREITO AOS R$ 1.000,00 DE CAPITAL.
			<BR>
			**** VALOR BASEADO NO Nº DE MATRÍCULAS (CENSO ESCOLAR). 
			</span>
			</center>";
		
		//total MAIS EDUCAÇÃO
		echo "<br />
			<table class=\"tabelaRelatorio\" border=\"0\" width=\"100%\" align=\"center\" cellspacing=\"0\" cellpadding=\"2\">
			<tr>
				<td align=\"left\" colspan=\"5\" class=\"tituloRelatorio2\"><b>TOTAIS MAIS EDUCAÇÃO</b></td>
			</tr>
			<tr valign=\"bottom\">
				<td align=\"center\" style=\"font-weight:bold;font-size:9px;\" colspan=\"17\">
					N° de Escolas: ".count($entidades)."
					&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;|&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
					Quantidade de Alunos: ".number_format($qtdGeralAlunosGeral,"0",",",".")."
					&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;|&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
					Total (custeio): ".number_format($kitCusteioGeral,"2",",",".")."
					&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;|&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
					Total (capital): ".number_format($kitCapitalGeral,"2",",",".")."
					&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;|&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
					***Ressarc. Monitores: ".number_format($ressarcimentoMonitoresGeral,"2",",",".")."
					&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;|&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
					Serviços/Materiais (custeio): ".number_format( ($servicosMateriaisGeral * 0.8 ),"2",",",".")."
					&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;|&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
					Serviços/Materiais (capital): ".number_format( ($servicosMateriaisGeral * 0.2 ),"2",",",".")."
					&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;|&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
					Total Geral: ".number_format( $totalGeralGeral ,"2",",",".")."
				</td>
			</tr>
			</table>
			";
		
		//total ESCOLA ABERTA
		echo "<br />
			<table class=\"tabelaRelatorio\" border=\"0\" width=\"100%\" align=\"center\" cellspacing=\"0\" cellpadding=\"2\">
			<tr>
				<td align=\"left\" colspan=\"5\" class=\"tituloRelatorio2\"><b>TOTAIS ESCOLA ABERTA</b></td>
			</tr>
			<tr valign=\"bottom\">
				<td align=\"center\" style=\"font-weight:bold;font-size:9px;\" colspan=\"17\">
					N° de Escolas: ".($entidadesEA ? count($entidadesEA) : '0')."
					&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;|&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
					Quantidade de Alunos: ".number_format($totalMatriculaCenso,"0",",",".")."
					&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;|&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
					Escola Aberta (custeio): ".number_format( $totalEscolaAbertaCusteio ,"2",",",".")."
					&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;|&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
					Escola Aberta (capital): ".number_format( $totalEscolaAbertaCapital ,"2",",",".")."
					&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;|&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
					Total Geral: ".number_format( ($totalEscolaAbertaCusteio + $totalEscolaAbertaCapital) ,"2",",",".")."
				</td>
			</tr>
			</table>
			";
		
		//total GERAL
		echo "<br />
			<table class=\"tabelaRelatorio\" border=\"0\" width=\"100%\" align=\"center\" cellspacing=\"0\" cellpadding=\"2\">
			<tr>
				<td align=\"left\" colspan=\"5\" class=\"tituloRelatorio2\"><b>TOTAIS GERAIS</b></td>
			</tr>
			<tr valign=\"bottom\">
				<td align=\"center\" style=\"font-weight:bold;font-size:9px;\" colspan=\"17\">
					N° de Escolas: ".($entidadesEA ? count($entidadesEA)+count($entidades) : 0+count($entidades))."
					&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;|&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
					Quantidade de Alunos: ".number_format( ($qtdGeralAlunosGeral + $totalMatriculaCenso),"0",",",".")."
					&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;|&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
					Kits(custeio): ".number_format($kitCusteioGeral,"2",",",".")."
					&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;|&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
					Kits(capital): ".number_format($kitCapitalGeral,"2",",",".")."
					&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;|&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
					***Ressarc. Monitores: ".number_format($ressarcimentoMonitoresGeral,"2",",",".")."
					&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;|&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
					Serviços/Materiais (custeio): ".number_format( ($servicosMateriaisGeral * 0.8 ),"2",",",".")."
					&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;|&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
					Serviços/Materiais (capital): ".number_format( ($servicosMateriaisGeral * 0.2 ),"2",",",".")."
					&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;|&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
					Escola Aberta (custeio): ".number_format( $totalEscolaAbertaCusteio ,"2",",",".")."
					&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;|&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
					Escola Aberta (capital): ".number_format( $totalEscolaAbertaCapital ,"2",",",".")."
					&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;|&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
					Total Geral: ".number_format( ($totalGeralGeral + $totalEscolaAbertaCusteio + $totalEscolaAbertaCapital) ,"2",",",".")."
				</td>
			</tr>
			</table>
			";
		
	}
	else{

		echo "
			<br />
			<table class=\"tabelaRelatorio\" border=\"0\" width=\"100%\" align=\"center\" cellspacing=\"0\" cellpadding=\"2\">
			<tr>
				<td align=\"left\" colspan=\"5\" class=\"tituloRelatorio2\"><b>TOTAIS GERAIS</b></td>
			</tr>
			<tr valign=\"bottom\">
				<td align=\"center\" style=\"font-weight:bold;font-size:9px;\" colspan=\"17\">
					N° de Escolas: ".count($entidades)."
					&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;|&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
					Quantidade de Alunos: ".number_format($qtdGeralAlunosGeral,"0",",",".")."
					&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;|&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
					Total (custeio): ".number_format($kitCusteioGeral,"2",",",".")."
					&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;|&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
					Total (capital): ".number_format($kitCapitalGeral,"2",",",".")."					
					&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;|&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
					Total Geral: ".number_format($totalGeralGeral,"2",",",".")."						
					&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;|&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
					Saldo a Deduzir: ".number_format( $totalGeralSaldo ,"2",",",".")."						
					&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;|&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
					Total do Repasse: ".number_format( $totalGeralRepasse ,"2",",",".")."
				</td>
			</tr>
			</table>
			<br />";

	}

	if(!isset($_REQUEST["visualizar"])) {
		$db->commit();
		echo "<script>
			  	if(window.opener && !window.opener.closed) {
					window.opener.location.reload();
				} 
			  </script>";
	}

} else {
	echo "<script>
			alert('Não existem entidades finalizadas no momento e/ou já foram gerados relatórios para\\ntodas as entidades associadas. Utilize a lista abaixo para rever algum relatório.');
			window.close();
		  </script>";
	exit;
}

?>