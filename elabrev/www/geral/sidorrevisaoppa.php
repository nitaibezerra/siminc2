<pre>Extreme Speed Includator Revisator PPAtor - Módulo de exportação de dados da Revisão Orçamentária do <?php echo SIGLA_SISTEMA; ?> para SIDOR</pre>
<?php
/**
 * Script de comunicao com o SIDOR para carga da revisão do PPA
 * @author Adonias Malosso <malosso@gmail.com>
 * @version 1.0
 */
require_once "config.inc";
require_once APPRAIZ . "/includes/classes_simec.inc";
require_once APPRAIZ . "/includes/funcoes.inc";
require_once APPRAIZ . "/includes/Snoopy.class.php";
require_once APPRAIZ . "/includes/Sidor.class.php";

set_time_limit(0);

function getmicrotimesidor()
{
	list($usec, $sec) = explode(" ", microtime());
	return ((float)$usec + (float)$sec);
}

ini_set("implicit_flush", 1);
ob_implicit_flush();

$usuarioSidor = "IYV";
$senhaSidor = "SD3822";
//$usuarioSidor = "ewg";
//$senhaSidor = "orl001";
if ($_REQUEST['fisico']) $carregarFisico = true; else $carregarFinanceiro = true;

$inicioTx = getmicrotimesidor();

$db = new cls_banco();
$sidor = new SidorPPA();

try {
	$sidor->login($usuarioSidor, $senhaSidor);
}
catch(SidorLoginException $e) {
	dbg('SIDOR: Usuário ou senha inválidos!',1);
	//$db->insucesso("Usuário ou senha inválidos!", '', true);
}
catch(SidorException $e) {
	dbg(2,1);
	$db->insucesso($e->message());
}
if($carregarFisico) {
$sqlDados =  "SELECT r.codreferenciasof, r.unicod, r.prgcod, r.acacod, r.loccod, cast(max(r.acaqtdefisicot) as varchar)
as acaqtdefisico, cast(max(r.fis2008t) as varchar) as fis2008, cast(max(r.fis2009t) as varchar) as fis2009, 
cast(max(r.fis2010t) as varchar) as fis2010 
FROM public.revisaoppa r
inner join elabrev.ppaacao_orcamento p on p.acacodreferenciasof = r.codreferenciasof and p.prgano='2007'
WHERE r.acaqtdefisico <> '0' and r.codreferenciasof is not null and trim(p.acadscprosof)<>'-' 
GROUP BY r.codreferenciasof, r.unicod, r.prgcod, r.acacod, r.loccod ORDER BY unicod, prgcod, acacod, loccod";	
//$sqlDados = "SELECT DISTINCT unicod, prgcod, acacod, loccod, acaqtdefisico, fis2008, fis2009, fis2010 FROM revisaoppa ORDER BY unicod, prgcod, acacod, loccod, acaqtdefisico";
/*$sqlDados = "select distinct a.unicod, a.prgcod, a.acacod, a.loccod,
coalesce(b.acaqtdefisico,'0') as acaqtdefisico, coalesce(b.fis2008,'0') as fis2008, 
coalesce(b.fis2009,'0') as fis2009, coalesce(b.fis2010,'0') as fis2010
from revisao_ppa a 
left join revisaoppa b using(unicod, prgcod, acacod, loccod) 
where b.unicod is null";*/
$dados = $db->carregar($sqlDados);




foreach($dados as $linha) {
	try {
		$linha["momento"] = "10";
		$linha["td"] = "3";
		
		//$codref = $sidor->pegarCodReferenciaSidor($linha["unicod"], $linha["prgcod"], $linha["acacod"], $linha["loccod"]);		
		$codref = $linha["codreferenciasof"];
		$urlInputs = "https://sidornet.planejamento.gov.br/captacao/ppa/ppaOfsFisAction.do?acao=listar&mom=%s&ref=%s&unid=%s&codAcao=%s";		
		$inputs = $sidor->pegarInputs($urlInputs, $linha["momento"], $codref, $linha["unicod"], $linha["acacod"]);
		$dadosEnvio = array(
			"operacao"=>$linha["operacao"]
			,"tipoAcao"=>""
			,"codAcao"=>$inputs["codAcao"]
			,"codProduto"=>$inputs["codProduto"]
			,"descProduto"=>$inputs["descProduto"]
			,"codUnidMedida"=>$inputs["codUnidMedida"]
			,"descUnidMedida"=>$inputs["descUnidMedida"]
			,"produto"=>$inputs["produto"]
			,"unidMedida"=>$inputs["unidMedida"]
			,"qtdFisPrevT1"=>$inputs["qtdFisPrevT1"]
			,"qtdFisPrevT2"=>$inputs["qtdFisPrevT2"]
			,"qtdFisPrevT3"=>$inputs["qtdFisPrevT3"]
			,"qtdFisPrevT4"=>$inputs["qtdFisPrevT4"]
			,"qtdTotalT1T4"=>$inputs["qtdTotalT1T4"]
			,"qtdFisRealT01"=>$inputs["qtdFisRealT01"]
			,"qtdFisRealT01"=>$inputs["qtdFisRealT01"]
			,"qtdFisPrevT"=>$inputs["qtdFisPrevT"]
			,"qtdAposT4"=>$inputs["qtdAposT4"]
			,"qtdTotalProj"=>$inputs["qtdTotalProj"]
			,"dthInicio"=>$inputs["dthInicio"]
			,"dthFim"=>$inputs["dthFim"]
			,"obsUsuario"=>""
		);

		$dadosEnvio["qtdFisPrevT1"] = $linha["acaqtdefisico"];
		$dadosEnvio["qtdFisPrevT2"] = $linha["fis2008"];
		$dadosEnvio["qtdFisPrevT3"] = $linha["fis2009"];
		$dadosEnvio["qtdFisPrevT4"] = $linha["fis2010"];
		$dadosEnvio["qtdTotalT1T4"] = 0+$linha["acaqtdefisico"]+$linha["fis2008"]+$linha["fis2009"]+$linha["fis2010"];

		$tipoSql = "SELECT taccod FROM monitora.acao WHERE unicod = '%s' and prgcod = '%s' and acacod = '%s' and loccod = '%s' AND acasnemenda = 'f'";
		$sql = sprintf($tipoSql, $linha["unicod"], $linha["prgcod"], $linha["acacod"], $linha["loccod"]);
		$taccod = $db->pegaUm($sql);
		switch($taccod) {
			case 3: case 1: $dadosEnvio["operacao"] = "Projeto"; break;
			case 2: $dadosEnvio["operacao"] = "Atividade"; break;
			case 4: $dadosEnvio["operacao"] = "Opera%E7%E3o+Especial"; break;
			default: 
				if($inputs["dthInicio"] && $inputs["dthFim"]) {
					$dadosEnvio["operacao"] = "Projeto";
				}
				else {
					$dadosEnvio["operacao"] = "Opera%E7%E3o+Especial";
				}
			break;
		}
		
/*		
		$cumulativaSql = "SELECT acasnmetanaocumulativa FROM monitora.acao WHERE unicod = '%s' and prgcod = '%s' and acacod = '%s' and loccod = '%s' AND acasnemenda = 'f'";
		$sql = sprintf($cumulativaSql, $linha["unicod"], $linha["prgcod"], $linha["acacod"], $linha["loccod"]);
		$metanaocumulativa = $db->pegaUm($sql);
		if($metanaocumulativa == 't') {
			$dadosEnvio["flgCumulatividade"] = "on";
			if($linha["fis2010"])
				$dadosEnvio["qtdTotalT1T4"] = $linha["fis2010"];
			elseif($linha["fis2009"])
				$dadosEnvio["qtdTotalT1T4"] = $linha["fis2009"];
			else
				$dadosEnvio["qtdTotalT1T4"] = $linha["fis2008"];
		}*/
		$dadosEnvio["flgCumulatividade"] = "off";
/*
		if($linha["fis2010"])
			$dadosEnvio["qtdTotalT1T4"] = $linha["fis2010"];
		elseif($linha["fis2009"])
			$dadosEnvio["qtdTotalT1T4"] = $linha["fis2009"];
		else
			$dadosEnvio["qtdTotalT1T4"] = $linha["fis2008"];
	
*/		
		
		$msg = $sidor->enviarDadosFisicos($linha["momento"], $linha["unicod"], $linha["acacod"], $dadosEnvio, $codref);
		?><pre>OK: <?=$linha["unicod"]?> - <?=$linha["prgcod"]?> - <?=$linha["acacod"]?> - <?=$linha["loccod"]?> :: <?=$msg?> :: <?=implode(",", $linha)?> :: <?=implode(",", $dadosEnvio)?></pre><?
	}
	catch(SidorCodReferenciaNaoEncontradoException $e) {
		?>
		<pre>ERRO: <?=$linha["unicod"]?> - <?=$linha["momento"]?> - <?=$taccod?> - <?=$linha["prgcod"]?> - <?=$linha["acacod"]?> - <?=$linha["loccod"]?> - <?=$totalAcao?> - Código de referência não encontrado!  :: <?=implode(",", $linha)?> :: <?=implode(",", $dadosEnvio)?></pre>
		<?
	}
	catch(SidorCargaException $e) {
		?>
		<pre>ERRO: <?=$linha["unicod"]?> - <?=$linha["momento"]?> - <?=$taccod?> - <?=$linha["prgcod"]?> - <?=$linha["acacod"]?> - <?=$linha["loccod"]?> - <?=$totalAcao?> - Erro ao inserir dados!  <?=$e->getMessage()?>:: <?=implode(",", $linha)?> :: <?=implode(",", $dadosEnvio)?></pre>
		<?
	}
	catch(SidorException $e) {
		?>
		<pre>ERRO: <?=$linha["unicod"]?> - <?=$linha["momento"]?> - <?=$taccod?>  - <?=$linha["prgcod"]?> - <?=$linha["acacod"]?> - <?=$linha["loccod"]?> - <?=$totalAcao?> - Genérico! Chame o suporte. :: <?=implode(",", $linha)?> :: <?=implode(",", $dadosEnvio)?></pre>
		<?
	}
	flush();ob_flush();flush();
}
	$tx = getmicrotimesidor() - $inicioTx;
	?>
	<pre>Dados físicos incluídos em <?=number_format($tx, "5", ",", ".")?> segundos :: Executado em <?=date("d/m/Y H:i:s")?></pre>
	<?
}

if($carregarFinanceiro) {
$inicioTx = getmicrotimesidor();
//
// DADOS FINANCEIROS
$sqlDados = "SELECT codreferenciasof, unicod, prgcod, acacod, loccod, fontesrev, 
gnd, somade2007, somade2008, somade2009, somade2010 FROM revisaoppa 
 where codreferenciasof is not null and unicod='26101' and acacod in ('0E44','0145')
ORDER BY somade2007 desc, unicod, prgcod, acacod, loccod, fontesrev, gnd";
/*$sqlDados = "select distinct a.unicod, a.prgcod, a.acacod, a.loccod, a.fontesrev, a.gnd,
coalesce(b.somade2007,'0') as somade2007, coalesce(b.somade2008,'0') as somade2008, 
coalesce(b.somade2009,'0') as somade2009, coalesce(b.somade2010,'0') as somade2010
from revisao_ppa a 
left join revisaoppa b using(unicod, prgcod, acacod, loccod) 
where b.unicod is null";*/
$dados = $db->carregar($sqlDados);

$fontes = $sidor->pegarFontesPPA();

$idAtual = "";
$dadosEnvio = array();
$i=0;
$linhaAnterior = array();
?>
<pre>DADOS ORÇAMENTÁRIOS</pre>
<?
foreach($dados as $linha) {

	try {
		$linha["momento"] = "10";
		$linha["td"] = "3";
		$id = $linha["unicod"].$linha["momento"].$linha["prgcod"].$linha["acacod"].$linha["loccod"].$linha["fontesrev"];

		if($id != $idAtual && $i > 0) {
		
			//
			// enviar dados para o sidor
			//$codref = $sidor->pegarCodReferenciaSidor($linhaAnterior["unicod"], $linhaAnterior["prgcod"], $linhaAnterior["acacod"], $linhaAnterior["loccod"]);
			$codref = $linhaAnterior["codreferenciasof"];
			$campos = $sidor->pegarCamposDetalhamento($linhaAnterior["td"], $linhaAnterior["fontesrev"], $linhaAnterior["momento"], $codref, $linhaAnterior["acacod"], $linhaAnterior["unicod"], $fontes[$linhaAnterior["fontesrev"]] );
			
			$urlInputs = "https://sidornet.planejamento.gov.br/captacao/ppa/ppaOfsFisAction.do?acao=listar&mom=%s&ref=%s&unid=%s&codAcao=%s";		
			$inputs = $sidor->pegarInputs($urlInputs, $linhaAnterior["momento"], $codref, $linhaAnterior["unicod"], $linhaAnterior["acacod"]);
			$dadosEnvio = array_merge($campos, $dadosEnvio);
			$dadosEnvio["codAcao"] = $inputs["codAcao"];
			$dadosEnvio["strDescFonte"] = $fontes[$linhaAnterior["fontesrev"]];
			$dadosEnvio["strFonte"] = $linhaAnterior["fontesrev"] . " - " . $fontes[$linhaAnterior["fontesrev"]];
			$dadosEnvio["strCodTDDis"] = $linhaAnterior["td"];
			
			$dadosEnvio["strCodNatuOutrasDesp"] = "33000000";
			$dadosEnvio["strCodNatuInvestimento"] = "44000000";
			$dadosEnvio["strCodNatuInverFin"] = "45000000";
			print  $url.'<br>';
			$msg = $sidor->enviarDadosFinanceiros($linhaAnterior["momento"], $linhaAnterior["unicod"], $dadosEnvio);
			?><pre>OK: <?=$linhaAnterior["unicod"]?> - <?=$linhaAnterior["prgcod"]?> - <?=$linhaAnterior["acacod"]?> - <?=$linhaAnterior["loccod"]?> :: <?=$msg?> :: <?=implode(",", $linhaAnterior)?> :: <?=implode(",", $dadosEnvio)?></pre><?
			$i=0;
			$dadosEnvio = array();
		}
		switch($linha["gnd"]) {
			case 3:
				$dadosEnvio["strOutrasDespT2"] = str_replace(',','.',str_replace('.','',$linha["somade2008"]));
				$dadosEnvio["strOutrasDespT3"] = str_replace(',','.',str_replace('.','',$linha["somade2009"]));
				$dadosEnvio["strOutrasDespT4"] = str_replace(',','.',str_replace('.','',$linha["somade2010"]));
			break;
			case 4:
				$dadosEnvio["strInvestimentoT2"] = str_replace(',','.',str_replace(',','.',$linha["somade2008"]));
				$dadosEnvio["strInvestimentoT3"] = str_replace(',','.',str_replace(',','.',$linha["somade2009"]));
				$dadosEnvio["strInvestimentoT4"] = str_replace(',','.',str_replace(',','.',$linha["somade2010"]));
			break;
		}
		$linhaAnterior = $linha;
		$idAtual = $id;
		$i++;
	}
	catch(SidorCodReferenciaNaoEncontradoException $e) {
		?><pre>ERRO: <?=$linhaAnterior["unicod"]?> - <?=$linhaAnterior["momento"]?> - <?=$linhaAnterior["prgcod"]?> - <?=$linhaAnterior["acacod"]?> - <?=$linhaAnterior["loccod"]?> - Código de referência não encontrado!  :: <?=implode(",", $linhaAnterior)?> :: <?=implode(",", $dadosEnvio)?></pre><?
	}
	catch(SidorCargaException $e) {
		?><pre>ERRO: <?=$linhaAnterior["unicod"]?> - <?=$linhaAnterior["momento"]?> - <?=$linhaAnterior["prgcod"]?> - <?=$linhaAnterior["acacod"]?> - <?=$linhaAnterior["loccod"]?> - Erro ao inserir dados!  <?=$e->getMessage()?>:: <?=implode(",", $linhaAnterior)?> :: <?=implode(",", $dadosEnvio)?></pre><?
	}
	catch(SidorException $e) {
		?><pre>ERRO: <?=$linhaAnterior["unicod"]?> - <?=$linhaAnterior["momento"]?> - <?=$linhaAnterior["prgcod"]?> - <?=$linhaAnterior["acacod"]?> - <?=$linhaAnterior["loccod"]?> - Genérico! Chame o suporte. :: <?=implode(",", $linhaAnterior)?> :: <?=implode(",", $dadosEnvio)?></pre><?
	}
	flush();ob_flush();flush();
}
$tx2 = getmicrotimesidor() - $inicioTx;
?>
<pre>Dados incluídos em <?=number_format($tx2, "5", ",", ".")?> segundos :: Executado em <?=date("d/m/Y H:i:s")?></pre>
<?
}
exit();
//}
?>