<?php

$dataInicial = date( "Y-m-d H:i:s" );
header( "Content-Type: text/plain;" );

function pegarAcoesSemParecerMes( $ano, $mes )
{
	global $db;
	$ano = (integer) $ano;
	$mes = sprintf( "%02d", $mes );
	$sql = "
		select
			a.acaid,
			a.unicod, uni.unidsc,
			a.prgcod, pro.prgdsc,
			a.acacod, a.acadsc,
			a.loccod, a.sacdsc as locdsc,
			u.usunome, u.usuemail, u.usucpf, u.ususexo
		from monitora.referencia r
			left join monitora.acao a on
				a.acasnrap = false and
				a.prgano = r.refano_ref
			left join monitora.avaliacaoparecer p on
				p.refcod = r.refcod and
				p.acaid = a.acaid and
				p.avpliberada = true and
				p.tpaid = 1
			inner join monitora.usuarioresponsabilidade ur on
				ur.acaid = a.acaid
			inner join seguranca.usuario u on
				u.usucpf = ur.usucpf
			inner join public.unidade uni on
				uni.unicod = a.unicod and
				uni.unitpocod = 'U' and
				uni.unistatus = 'A'
			inner join monitora.programa pro on
				pro.prgcod = a.prgcod and
				pro.prgano = a.prgano and
				pro.prgid = a.prgid
		where
			r.refdata_limite_parecer_aca is null and
			r.refsngrupo = false and
			a.acastatus = 'A' and
			p.avpid is null and
			r.refano_ref = '" . $ano . "' and
			r.refmes_ref = '" . $mes . "' and
			ur.rpustatus = 'A' and
			ur.pflcod = 1 and
			u.usustatus = 'A' and a.procod not in ( 0,1,3 )
	";
// retirado o filtro -- a.acadscproduto is not null and da consulta acima, 
// pois para o ano de 2008 todas as ações estão com esse campo nulo

	
	$dados = $db->carregar( $sql );
	return $dados ? $dados : array();
}

function pegarAcoesSemParecer()
{
	// carrega os meses/anos que ainda é possível fazer análise
	$sql = "
		select refmes_ref, refano_ref
		from monitora.referencia
		where refsnmonitoramento = true and refdata_limite_avaliacao_aca >= now()
		order by refano_ref, refmes_ref
	";
	global $db;
	$periodos = $db->carregar( $sql );
	$periodos = $periodos ? $periodos : array();
	$dados = array();
	foreach ( $periodos as $periodo )
	{
		$mesAtual = (integer) $periodo['refmes_ref'];
		$anoAtual = $periodo['refano_ref'];
		$chaveMesAno = $anoAtual . "-" . $mesAtual;
		// carrega as ações sem parecer de um determinado mês/ano
		$dadosMes = pegarAcoesSemParecerMes( $anoAtual, $mesAtual );
		foreach ( $dadosMes as $dadosMesLinha )
		{
			// agrupa ações por usuário
			$usucpf = $dadosMesLinha['usucpf'];
			if ( !array_key_exists( $usucpf, $dados ) )
			{
				$dados[$usucpf]             = array();
				$dados[$usucpf]['usucpf']   = $usucpf;
				$dados[$usucpf]['usunome']  = $dadosMesLinha['usunome'];
				$dados[$usucpf]['usuemail'] = $dadosMesLinha['usuemail'];
				$dados[$usucpf]['ususexo']  = $dadosMesLinha['ususexo'];
				$dados[$usucpf]['periodos'] = array();
			}
			// agrupa por data mes/ano para cada usuário
			if ( !array_key_exists( $chaveMesAno, $dados[$usucpf]['periodos'] ) )
			{
				$dados[$usucpf]['periodos'][$chaveMesAno] = array();
			}
			$acao = array(
				"acaid"  => $dadosMesLinha['acaid'],
				"prgcod" => $dadosMesLinha['prgcod'],
				"acacod" => $dadosMesLinha['acacod'],
				"unicod" => $dadosMesLinha['unicod'],
				"loccod" => $dadosMesLinha['loccod'],
				"acadsc" => $dadosMesLinha['acadsc'],
				"locdsc" => $dadosMesLinha['locdsc']
			);
			array_push( $dados[$usucpf]['periodos'][$chaveMesAno], $acao );
		}
	}
	return $dados;
}

require_once "config.inc";
include APPRAIZ . "includes/classes_simec.inc";
include APPRAIZ . "includes/funcoes.inc";

error_reporting( E_ALL );

$_SESSION['mnuid'] = 1;
$_SESSION['sisid'] = 1;
$_SESSION['usucpforigem'] = "";

$db = new cls_banco();

$pendencias = pegarAcoesSemParecer();

$meses = array(
	"1" => "Janeiro",	"2" => "Favereiro",	"3" => "Março",		"4" => "Abril",
	"5" => "Maio",		"6" => "Junho",		"7" => "Julho",		"8" => "Agosto",
	"9" => "Setembro",	"10" => "Outubro",	"11" => "Novembro",	"12" => "Dezembro"
);

require APPRAIZ . "includes/Email.php";

$enviador = new Email();

$frase =
	"<br/>".
	"Existem ações que não foram avaliadas ou liberados no sistema.<br/>" .
	"Abaixo a lista de ações (Unidade, Programa, Ação, Localizador) seguidas de seus períodos pendentes:<br/>";

$dataAtual = date( "d" ) . " de " . $meses[date( "n" )] . " de " . date( "Y" );

foreach ( $pendencias as $itemUsu )
{
	$fraseAcao = "";
	$acaids = array();
	$fraseAcao = "";
	foreach ( $itemUsu['periodos'] as $chave => $acoes )
	{
		$data = explode( "-", $chave );
		$mes = $meses[$data[1]];
		$ano = $data[0];
		$data = $mes . " / " . $ano;
		$frasePeriodo = "<font color=\"#dd3030\">" . $data . "</font><br/>";
		foreach ( $acoes as $acao )
		{
			$frasePeriodo .=
				$acao['prgcod'] . "." .
				$acao['acacod'] . "." .
				$acao['unicod'] . "." .
				$acao['loccod'] . " " .
				$acao['acadsc'] . " " .
				"(" . $acao['locdsc'] . ")<br/>";
			array_push( $acaids, $acao['acaid'] );
		}
		$fraseAcao .= "<br/>" . $frasePeriodo;
	}
	$email    = $itemUsu['usuemail'];
	$cpf      = $itemUsu['usucpf'];
	//$assunto  = "Lembrete de pendências";
	switch ( strtoupper( $itemUsu['ususexo'] ) )
	{
		case 'M':
			$nome_pre = "Coordenador";
			$nome_pos = "Prezado Coordenador";
			break;
		case 'F':
			$nome_pre = "Coordenadora";
			$nome_pos = "Prezada Coordenadora";
			break;
		default:
			$nome_pre = "Coordenador(a)";
			$nome_pos = "Prezado(a) Coordenador(a)";
			break;
	}
	$nome = $itemUsu['usunome'];
	
	$assunto = "Atualização de informações no ". SIGLA_SISTEMA;
	
	$mensagem = <<<EOT
		<p align="center">
		    <b>
		        MINISTÉRIO DA EDUCAÇÃO
		        <br/>
		        SECRETARIA EXECUTIVA
		        <br/>
		        SUBSECRETARIA DE PLANEJAMENTO E ORÇAMENTO
		    </b>
		</p>
		<br/><br/>
		<p align="left">
		    Circular Eletrônica SPO/SE/MEC
		</p>
		<p align="right">
		    Brasília, $dataAtual.
		</p>
		<p align="left">
		    DE: Subsecretaria de Planejamento e Orçamento
		    <br/>
		    PARA: $nome, $nome_pre de Ações dos Programas do MEC 
		</p>
		<p align="left">
		    Assunto: $assunto
		</p>
		<p align="left">
		    &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
		    $nome_pos de Ação,
		</p>
		<p align="left">
		    &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
		    Esta é uma mensagem eletrônica e é enviada automaticamente pelo Sistema Integrado de Planejamento, Orçamento e Finanças do Ministério da Educação (SIMEC), a todos (as) Coordenadores (as) de Ação, deste Ministério, no inicio de cada mês, com o objetivo de lembrá-lo (a) do preenchimento da execução física e avaliação das ações sob sua responsabilidade, no Módulo de Monitoramento e Avaliação do Sistema.
		</p>
		<p align="left">
		    &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
		    Caso não seja o responsável pelo preenchimento das informações, favor entrar em contato solicitando a substituição do Coordenador da referida Ação.
		    Para maiores informações entre em contato com a Unidade de Monitoramento e Avaliação (UMA) da Coordenação Geral de Planejamento da SPO, via  mensagem eletrônica para o SIMEC (<a href="http://simec.mec.gov.br">http://simec.mec.gov.br</a>) ou (<a href="mailto:spo_planejamento@mec.gov.br">spo_planejamento@mec.gov.br</a>).
		</p>
		<p align="left">
			Relação de Períodos/Ações pendentes de preenchimento:
		</p>
		<p align="left">
			$fraseAcao
		</p>
		<p align="left">
		    &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Caso já tenha efetuado os devidos registros no SIMEC, por favor, desconsidere a mensagem. 
		</p>

		<p align="left">
		    &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
		    Atenciosamente,
		</p>
		<br/><br/>
		<p align="center">
		    <b>
		        CGP - Coordenação Geral de Planejamento
		        <br/>
		        SPO - Subsecretaria de Planejamento e Orçamento
		    </b>
		</p>
EOT;
	$enviador->enviar( array( "" ), $assunto, $mensagem );
}

$db->commit();

?>