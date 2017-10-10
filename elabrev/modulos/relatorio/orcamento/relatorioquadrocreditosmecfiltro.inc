<?php

	/*
		Sistema Simec
		Setor responsável: SPO-MEC
		Desenvolvedor: Equipe Consultores Simec
		Analista: Gilberto Arruda Cerqueira Xavier, Cristiano Cabral (cristiano.cabral@gmail.com)
		Programador: Henrique Xavier Couto (e-mail: henriquexcouto@gmail.com), Renan de Lima Barbosa (e-mail: renandelima@gmail.com)
		Módulo: relatorioquadrocreditosmecfiltro.inc
		Finalidade: Manutenção na tabela matriz
	*/

/**
 * Exportação de dados para o sidor
 */
set_time_limit(0);

function getmicrotimesidor()
{list($usec, $sec) = explode(" ", microtime());
 return ((float)$usec + (float)$sec);}

require_once APPRAIZ . "includes/Snoopy.class.php";
require_once APPRAIZ . "includes/Sidor.class.php";

$inicioTx = getmicrotimesidor();
if($_REQUEST["_dadosEnvio"] &&  $_REQUEST["usuario_sidor"] && $_REQUEST["senha_sidor"]) {

	//ini_set("output_buffering", 0);
	//ini_set("implicit_flush", 1);
    ob_start();

	$usuarioSidor = $_REQUEST["usuario_sidor"];
	$senhaSidor = $_REQUEST["senha_sidor"];
	$dados = unserialize(base64_decode($_REQUEST["_dadosEnvio"]));
	array_push($dados,array());
	
	$sidor = new Sidor();
	

	try {
		$sidor->login($usuarioSidor, $senhaSidor);
	}
	catch(SidorLoginException $e) {
		$db->insucesso("Usuário ou senha inválidos!", '', true);
	}
	catch(SidorException $e) {
		$db->insucesso($e->message());
	}
	
	$idAtual = "";
	$dadosEnvio = array();
	$i=0;
	$totalAcao = 0;
	$linhaAnterior = array();
	?>
	<pre>Speed Includator Orçamentator - Módulo de exportação de dados orçamentários do <?php echo SIGLA_SISTEMA; ?> para SIDOR</pre>
	<?
	foreach($dados as $linha) {
		try {
			$linha["tipoDetalhamento"] = trim("1" . $linha["tdecod"]);
			$id = $linha["unicod"].$linha["tipoDetalhamento"].$linha["prgcod"].$linha["acacod"].$linha["loccod"];
			$prefix = sprintf("%02d", $i);
	
			if($id != $idAtual && $i > 0) {
				
				//
				// enviar dados para o sidor
				$codref = $linhaAnterior["acacodreferenciasof"];
				//$codref = $sidor->pegarCodReferenciaSidor($linhaAnterior["unicod"], $linhaAnterior["tipoDetalhamento"], $linhaAnterior["prgcod"], $linhaAnterior["acacod"], $linhaAnterior["loccod"]);
				$prod = $sidor->pegarProdutoUnidadeSidor($codref, $linhaAnterior["unicod"], $linhaAnterior["tipoDetalhamento"], $linhaAnterior["prgcod"], $linhaAnterior["acacod"], $linhaAnterior["loccod"]);
				$dadosEnvio = array_merge($dadosEnvio, array("flgAtualizacao$prefix"=>"N"
					,"natu$prefix"=>""
					,"iuso$prefix"=>""
					,"fonte$prefix"=>""
					,"idoc$prefix"=>""
					,"valor$prefix"=>""
				));
				//dbg($dadosenvio);
				$sidor->insereDadosProposta($codref, $linhaAnterior["unicod"], $linhaAnterior["tipoDetalhamento"], $linhaAnterior["prgcod"], $linhaAnterior["acacod"], $linhaAnterior["loccod"], $i+1, $dadosEnvio, $totalAcao, $linhaAnterior["acaqtdefisico"], $prod["codproduto"], $prod["desproduto"], $prod["unidademedida"], $linhaAnterior["justificativa"]);
				?>

				<pre>OK: <?=$linhaAnterior["unicod"]?> - <?=$linhaAnterior["tipoDetalhamento"]?> - <?=$linhaAnterior["prgcod"]?> - <?=$linhaAnterior["acacod"]?> - <?=$linhaAnterior["loccod"]?> - <?=$totalAcao?> - <?=$i?> registro(s) incluído(s)!</pre>
				<?
				flush();
				ob_flush();
				flush();
				ob_flush();
				flush();
				ob_flush();
				

				$i=0;
				$totalAcao = 0;
				$dadosEnvio = array();
			}
			$prefix = sprintf("%02d", $i); // calcular novamente porque o i pode ter sido zerado		
	
			$dadosEnvio = array_merge($dadosEnvio, array("flgAtualizacao$prefix"=>"N"
				,"natu$prefix"=>$linha["ndpcod"]
				,"iuso$prefix"=>$linha["iducod"]
				,"fonte$prefix"=>$linha["foncod"]
				,"idoc$prefix"=>$linha["idocod"]
				,"valor$prefix"=>$linha["valor"]
			));
			$linhaAnterior = $linha;
			$totalAcao += $linha["valor"];
			$idAtual = $id;
			$i++;
		}
		catch(SidorCodReferenciaNaoEncontradoException $e) {
			$i=0;
			$totalAcao = 0;
			$dadosEnvio = array();
			?>
			<pre>ERRO: <?=$linhaAnterior["unicod"]?> - <?=$linhaAnterior["tipoDetalhamento"]?> - <?=$linhaAnterior["prgcod"]?> - <?=$linhaAnterior["acacod"]?> - <?=$linhaAnterior["loccod"]?> - <?=$totalAcao?> - Código de referência não encontrado!</pre>
			<?
		}
		catch(SidorCargaException $e) {
			?>
			<pre>ERRO: <?=$linhaAnterior["unicod"]?> - <?=$linhaAnterior["tipoDetalhamento"]?> - <?=$linhaAnterior["prgcod"]?> - <?=$linhaAnterior["acacod"]?> - <?=$linhaAnterior["loccod"]?> - <?=$totalAcao?> - Erro ao inserir dados!</pre>
			<?
		}
		catch(SidorException $e) {
			?>
			<pre>ERRO: <?=$linhaAnterior["unicod"]?> - <?=$linhaAnterior["tipoDetalhamento"]?> - <?=$linhaAnterior["prgcod"]?> - <?=$linhaAnterior["acacod"]?> - <?=$linhaAnterior["loccod"]?> - <?=$totalAcao?> - Genérico! Chame o suporte.</pre>
			<?
		}
	}
	$tx = getmicrotimesidor() - $inicioTx;
	?>
	<pre>Dados incluídos em <?=number_format($tx, "5", ",", ".")?> segundos :: Executado em <?=date("d/m/Y H:i:s")?></pre>
	<pre><a href="Javascript:history.back(-3)">Voltar para o <?php echo SIGLA_SISTEMA; ?></a></pre>
	<?
	exit();
}

/**
 * Monta where da consulta
 *
 * @param array $campo array de valores para o campo
 * @param string $nmcampo nome do campo na consulta
 * @param string $alias alias da tabela
 * @param string $clausula
 * @return string
 */
function monta_where($campo, $nmcampo, $alias, $clausula)
{
	$wh = ""; 
	if (count($campo) > 0 && $campo[0] != '')
		{
			for($i=0; $i<count($campo); $i++)
			{
				if (!$clausula)
				if ($i==0) $wh .= " and $alias.$nmcampo in ('".$campo[$i]; 	else $wh .= "','".$campo[$i];
				else
				if ($i==0) $wh .= " and $clausula in ('".$campo[$i]; 	else $wh .= "','".$campo[$i];
				
			}
			$wh .= "') ";	
		}
		
	return $wh;
}

$ppoid =$db->pegaUm( "select ppoid as codigo from elabrev.propostaorcamento where ppostatus = 'A' and tppid=1 and ppoanoexercicio = '".$_SESSION['exercicio']."'");

$where = "";
$where .= monta_where($_REQUEST['unidades'], 'unicod', 'acao', '');
$where .= monta_where($_REQUEST['prgcod'], 'prgcod', 'acao', '');
$where .= monta_where($_REQUEST['acacod'], 'acacod', 'acao', '');
$where .= monta_where($_REQUEST['idocod'], 'idocod', 'idoc', '');
$where .= monta_where($_REQUEST['ndpcod'], 'ndpcod', 'nd', '');
$where .= monta_where($_REQUEST['fonte'], 'foncod', 'da', '');
$where .= monta_where($_REQUEST['gnd'], 'gndcod', 'nd', '');
$where .= 'and da.ppoid = '	. $ppoid;

if ( $_REQUEST['tipo_arquivo'] > 0 )
{
	if ( $_REQUEST['tipo_relatorio_qdd'] == 'completo' )
	{
		$sqlQddXls = " select  
					acao.acacodreferenciasof,
					acao.unicod, 
					acao.prgcod, 
					acao.acacod, 
					acao.loccod, 
					nd.ndpcod, 
					da.iducod, 
					da.foncod, 
					idoc.idocod, 
					coalesce ( SUM(da.dpavalor), 0 ) as valor, 
					acao.acaqtdefisico, 
					case when acao.acacod in('09HB', '2000', '2272', '2991', '2992', '4001', '4006', '4009', '4086', '6318', '6321','00C5','0110') and substr(nd.ndpcod, 1,2)<>'31' then '1' else acao.tdecod end as tdecod, 
					acao.justificativa 
				 from elabrev.despesaacao da 
					inner join elabrev.ppaacao_orcamento acao on acao.acaid = da.acaid 
					inner join naturezadespesa nd ON nd.ndpid = da.ndpid 
					inner join idoc on idoc.idoid = da.idoid 
					where 1=1 " . $where . "
				 group by 
				 	acao.acacodreferenciasof,
					acao.unicod, 
					acao.prgcod, 
					acao.acacod, 
					acao.loccod, 
					nd.ndpcod, 
					da.iducod, 
					da.foncod, 
					idoc.idocod, 
					acao.acaqtdefisico, 
					acao.tdecod, 
					acao.justificativa 
				 order by 
					acao.unicod, 
					acao.tdecod, 	
					acao.prgcod, 
					acao.acacod, 
					acao.loccod ";
	}
	else
	{
		$sqlQddXls = " select 
					acao.acacodreferenciasof, 
					acao.unicod, 
					acao.prgcod, 
					acao.acacod, 
					acao.loccod, 
					coalesce ( SUM(da.dpavalor), 0 ) as valor 
				 from elabrev.despesaacao da 
					inner join elabrev.ppaacao_orcamento acao on acao.acaid = da.acaid 
					inner join naturezadespesa nd ON nd.ndpid = da.ndpid 
					where 1=1 " . $where . "
				 group by
				 	acao.acacodreferenciasof,
					acao.unicod, 
					acao.prgcod, 
					acao.acacod, 
					acao.loccod 
				 order by 
					acao.unicod, 
					acao.prgcod, 
					acao.acacod, 
					acao.loccod ";
	}
	switch( $_REQUEST["tipo_arquivo"] )
	{
		case 1: // html
			if ( $_REQUEST["tipo_relatorio"] == 'qdd' )
			{
				include( dirname( __FILE__ ) . '/relatorioquadrocreditosmec.inc' );
				?>
				<script language="javascript" type="text/javascript">
//					document.formulario.action='window.open("elabrev.php?modulo=relatorio/orcamento/relatorioquadrocreditosmec&acao=R", "formulario", "width=480,height=460,status=1,toolbar=0,scrollbars=1,resizable=1")';
//					document.submit();
				</script>
				
				<?
			}
			else if ( $_REQUEST["tipo_relatorio"] == 'sintese' )
			{
				include( dirname( __FILE__ ) . '/orgaosintesedespesa.inc' );
				?>
				<script language="javascript" type="text/javascript">
//					document.formulario.action='window.open("elabrev.php?modulo=relatorio/orcamento/orgaosintesedespesa&acao=R", "formulario", "width=480,height=460,status=1,toolbar=0,scrollbars=1,resizable=1")';
//					document.submit();
				</script>
		
				<?
			}
			return;
		break;
		case 2: // xls
			if ( $_REQUEST["tipo_relatorio"] == 'qdd' )
			{
				$db->sql_to_excel( $sqlQddXls, "qddMec", '', array('s','s','s','s','s','s','s','s','n','n','s') );
			}
			else
			{
				// define as unidades
				$dados_unidade = $_REQUEST['unidades_sintese'];
				for ( $i = count( $dados_unidade ) - 1; $i >= 0 && $dados_unidade[$i] != ''; $i--  )
				{
					$unidades .= ", '" . $dados_unidade[$i] . "'";
				}
				$unidades = substr( $unidades, 1 );
				if ( $unidades )
				{
					$unidades = ' a.unicod in ( ' . $unidades . ' ) and ';
				}
				// FIM define as unidades
				
				// define as ações
				$dados_acao = $_REQUEST['acoes_sintese'];
				for ( $i = count( $dados_acao ) - 1; $i >= 0 && $dados_acao[$i] != ''; $i--  )
				{
					$acoes .= ", '" . $dados_acao[$i] . "'";
				}
				$acoes = substr( $acoes, 1 );
				if ( $acoes )
				{
					$acoes = ' a.acacod in ( ' . $acoes . ' ) and ';
				}
				// FIM define as ações
				
				// define os tipos de detalhamento
				$dados_detalhamento = $_REQUEST['detalhamento_sintese'];
				for ( $i = count( $dados_detalhamento ) - 1; $i >= 0 && $dados_detalhamento[$i] != ''; $i--  )
				{
					$detalhamentos .= ", " . $dados_detalhamento[$i] . "";
				}
				$detalhamentos = substr( $detalhamentos, 1 );
				if ( $detalhamentos ) $detalhamentos = " tda.tpdid in ( " . $detalhamentos . " )  and ";
				// FIM define as ações

				header('Content-type: application/xls');
				header('Content-Disposition: attachment; filename="planilha_simec.xls"');
				$querys = array();
				$Query2 = "select ".	
				 "		f.funcod, ".
				 "		f.fundsc, ".
				 "		coalesce( SUM(ac.dpavalor) , 0) as valor ".
				 "	from elabrev.despesaacao ac ".
				 "	inner join elabrev.ppaacao_orcamento a ON ".
				 "		a.acaid = ac.acaid ".
				 "	inner join elabrev.ppafuncao f ON ".
				 "		f.funcod = a.funcod ".
				 "	inner join elabrev.ppaprograma_orcamento ppa ON ".
				 "		ppa.prgid = a.prgid ".
				 "	inner join unidade b on ".
				 "		a.unicod = b.unicod ".
				 "	inner join elabrev.tipodetalhamentoacao tda on ".
				 "		tda.acaid = a.acaid ".
				 "	where ".
				 $unidades .
				 $acoes .
				 $detalhamentos .
				 "		a.prgano = '".$_SESSION['exercicio']."' and ".
				 "		a.acastatus='A' and ".
				 "		a.acasnrap = false  ".
				 "	group by ".
				 "		f.funcod, ".
				 "		f.fundsc ".
				 "	order by ".
				 "		f.funcod, ".
				 "		f.fundsc ";
				if ( in_array( 'funcao', $_REQUEST['filtro_sintese'] ) )
				{
					array_push( $querys, $Query2 );
				}
				$Query3 = "select ".	
					 "		f.sfucod, ".
					 "		f.sfudsc, ".
					 "		coalesce( SUM(ac.dpavalor) , 0) as valor ".
					 "	from elabrev.despesaacao ac ".
					 "	inner join elabrev.ppaacao_orcamento a ON ".
					 "		a.acaid = ac.acaid ".
					 "	inner join elabrev.ppasubfuncao f ON ".
					 "		f.sfucod = a.sfucod ".
					 "	inner join elabrev.ppaprograma_orcamento ppa ON ".
					 "		ppa.prgid = a.prgid ".
					 "	inner join unidade b on ".
					 "		a.unicod = b.unicod ".
					 "	inner join elabrev.tipodetalhamentoacao tda on ".
					 "		tda.acaid = a.acaid ".
					 "	where ".
					 $unidades .
					 $acoes .
					 $detalhamentos .
					 "		a.prgano = '".$_SESSION['exercicio']."' and ".
					 "		a.acastatus='A' and ".
					 "		a.acasnrap = false  ".
					 "	group by ".
					 "		f.sfucod, ".
					 "		f.sfudsc ".
					 "	order by ".
					 "		f.sfucod, ".
					 "		f.sfudsc ";
				if ( in_array( 'subfuncao', $_REQUEST['filtro_sintese'] ) )
				{
					array_push( $querys, $Query3 );
				}
				$Query4 = "select ".
					 "		ppa.prgcod, ".
					 "		ppa.prgdsc, ".
					 "		coalesce( SUM(ac.dpavalor) , 0) as valor ".
					 "	from elabrev.despesaacao ac ".
					 "	inner join elabrev.ppaacao_orcamento a ON ".
					 "		a.acaid = ac.acaid ".
					 "	inner join elabrev.ppaprograma_orcamento ppa ON ".
					 "		ppa.prgid = a.prgid ".
					 "	inner join unidade b on ".
					 "		a.unicod = b.unicod ".
					 "	inner join elabrev.tipodetalhamentoacao tda on ".
					 "		tda.acaid = a.acaid ".
					 "	where ".
					 $unidades .
					 $acoes .
					 $detalhamentos .
					 "		a.prgano = '".$_SESSION['exercicio']."' and ".
					 "		a.acastatus='A' and ".
					 "		a.acasnrap = false  ".
					 "	group by ".
					 "		ppa.prgcod, ".
					 "		ppa.prgdsc ".
					 "	order by ".
					 "		ppa.prgcod, ".
					 "		ppa.prgdsc ";
				if ( in_array( 'programa', $_REQUEST['filtro_sintese'] ) )
				{
					array_push( $querys, $Query4 );
				}
				$Query5 = "select ".
					 "		a.acacod, ".
					 "		a.acadsc, ".
					 "		coalesce( SUM(ac.dpavalor) , 0) as valor ".
					 "	from elabrev.despesaacao ac ".
					 "	inner join elabrev.ppaacao_orcamento a ON ".
					 "		a.acaid = ac.acaid ".
					 "	inner join unidade b on ".
					 "		a.unicod = b.unicod ".
					 "	inner join elabrev.tipodetalhamentoacao tda on ".
					 "		tda.acaid = a.acaid ".
					 "	where ".
					 $unidades .
					 $acoes .
					 $detalhamentos .
					 "		a.prgano = '".$_SESSION['exercicio']."' and ".
					 "		a.acastatus='A' and ".
					 "		a.acasnrap = false  ".
					 "	group by ".
					 "		a.acacod, ".
					 "		a.acadsc ".
					 "	order by ".
					 "		a.acacod, ".
					 "		a.acadsc ";
				if ( in_array( 'acao', $_REQUEST['filtro_sintese'] ) )
				{
					array_push( $querys, $Query5 );
				}
				$Query6 = "select ".	
					 "			gnd.gndcod, ".
					 "			gnd.gnddsc,  ".
					 "		coalesce( SUM(ac.dpavalor) , 0) as valor ".
					 "	from elabrev.despesaacao ac ".
					 "		inner join elabrev.ppaacao_orcamento a ON ".
					 "			a.acaid = ac.acaid ".
					 "		inner join naturezadespesa n ON ".
					 "			n.ndpid = ac.ndpid ".
					 "		inner join gnd ON ".
					 "			gnd.gndcod = n.gndcod ".
					 "		inner join elabrev.ppaprograma_orcamento ppa ON ".
					 "			ppa.prgid = a.prgid ".
					 "		inner join unidade b on ".
					 "			a.unicod = b.unicod ".
					 "	inner join elabrev.tipodetalhamentoacao tda on ".
					 "		tda.acaid = a.acaid ".
					 "	where ".
					 $unidades .
					 $acoes .
					 $detalhamentos .
					 "		a.prgano = '".$_SESSION['exercicio']."' and ".
					 "		a.acastatus='A' and ".
					 "		a.acasnrap = false  ".
					 "	group by ".
					 "		gnd.gndcod, ".
					 "		gnd.gnddsc ".
					 "	order by ".
					 "		gnd.gndcod, ".
					 "		gnd.gnddsc ";
				if ( in_array( 'gnd', $_REQUEST['filtro_sintese'] ) )
				{
					array_push( $querys, $Query6 );
				}
				$Query7 = "select ".	
					 "			f.foncod, ".
					 "			f.fondsc,  ".
					 "		coalesce( SUM(ac.dpavalor) , 0) as valor ".
					 "	from elabrev.despesaacao ac ".
					 "		inner join elabrev.ppaacao_orcamento a ON ".
					 "			a.acaid = ac.acaid ".
					 "		inner join fonterecurso f ON ".
					 "			f.foncod = ac.foncod ".
					 "		inner join elabrev.ppaprograma_orcamento ppa ON ".
					 "			ppa.prgid = a.prgid ".
					 "		inner join unidade b on ".
					 "			a.unicod = b.unicod ".
					 "	inner join elabrev.tipodetalhamentoacao tda on ".
					 "		tda.acaid = a.acaid ".
					 "	where ".
					 $unidades .
					 $acoes .
					 $detalhamentos .
					 "		a.prgano = '".$_SESSION['exercicio']."' and ".
					 "		a.acastatus='A' and ".
					 "		a.acasnrap = false  ".
					 "	group by ".
					 "		f.foncod, ".
					 "		f.fondsc ".
					 "	order by ".
					 "		f.foncod, ".
					 "		f.fondsc ";
				if ( in_array( 'fonte', $_REQUEST['filtro_sintese'] ) )
				{
					array_push( $querys, $Query7 );
				}
				$Query8 = " select " .
					" 	substr(nd.ndpcod,1,1)||'.'||substr(nd.ndpcod,2,1)||'.'||substr(nd.ndpcod,3,2)||'.'||substr(nd.ndpcod,5,2)||'.'||substr(nd.ndpcod,7,2) as codigo,  " .
					" nd.ndpdsc as descricao, " .
					" coalesce( sum( dpavalor ), 0 ) as valor " .
					" from naturezadespesa nd " .
					" left join elabrev.despesaacao da using ( ndpid ) " .
					" left join elabrev.ppaacao_orcamento a using ( acaid ) " .
					" where " . $unidades . $acoes . " 1 = 1 " .
					" group by nd.ndpcod, nd.ndpdsc having 	coalesce( sum( dpavalor ), 0 ) > 0 order by nd.ndpcod";
				if ( in_array( 'natureza', $_REQUEST['filtro_sintese'] ) )
				{
					array_push( $querys, $Query8 );
				}
				//$querys = array($Query1,$Query2,$Query3,$Query4,$Query5,$Query6,$Query7,$Query8);
				for ( $i = 0; $i < count( $querys ); $i++ )
				{
					$RS = $db->record_set( $querys[$i] );
					$_SESSION['conteudo']=  $db->carrega_tudo($RS); 
					$_SESSION['LINHA'] = $db->conta_linhas($RS);
					$_SESSION['title'] = $tit;
					$_SESSION['TITULO']= 'Relatório Gerencial de Ação';
					$_SESSION['TIT'][0][0] = 'Código ';
					$_SESSION['TIT'][0][1] = 'Título';
					$_SESSION['data']= date('(h:m) d/m/Y');
					$_SESSION['LIN'] = array();
					$_SESSION['cab'] = Array();
					for ($a=0;$a<pg_num_fields($RS);$a++)
					{
						$_SESSION['cab'][$a] = pg_field_name($RS,$a); 
					}
					for ($a=0;$a<count($_SESSION['cab']);$a++)
					{
						if ( $a<count( $_SESSION['cab'] ) -1 )
						{
							print trim(($_SESSION['cab'][$a])). ' ' . "\t";
						}
						else
						{
							print($_SESSION['cab'][$a]);
						}
					}
					print chr(13);
					if ( $_SESSION['LINHA'] >= 0 ) 
					{
						for ( $a=0; $a<=$_SESSION['LINHA'];$a++)
						{
							$res = $db->carrega_registro($RS,$a);
							// a linha abaixo transforma em variáveis todos os campos do array
							if(is_array($res)) foreach($res as $k=>$v) ${$k}=$v;
						}
					}
					if (count($_SESSION['conteudo'][0])>1)
					{
						for ($a=0;$a<count($_SESSION['conteudo']);$a++)
						{
							foreach($_SESSION['conteudo'][$a] as $k=>$v)
							{
								if ( $k <> 'acao' )
								{
									${$k}=$v;
									if ( $v == 't' )
									{
										$v='1';
									}
									else if ( $v=='f' )
									{
										$v='0';
									}
									if ($v <> '' ) 
									{
										$v=$v;
										print $v."\t";
									}
								}
							}
							print chr(13);
						}
					}
				}
				exit();
			}
			exit;
	}
}

include APPRAIZ."includes/cabecalho.inc"; 
?>
<br/>
<?php if( $_REQUEST["tipo_arquivo"] == 3 && $sql ) { ?>
	<?php
		$sqlQdd = $sqlQddXls;
		$cabecalho = array("Cod. SOF", "Unidade", "Programa", "Ação", "Localizador", "Nat. Desp.", "Id. Uso", "Fonte", "Idoc", "Valor", "Qtde Físico", "Detalhamento", "Justificativa");
		$soma = array(0,0,0,0,0,0,0,0,1,0,0);
		$dados = $db->carregaAgrupado($sqlQdd, "unicod");
		$dadosPost = $db->carregar($sqlQdd);
		monta_titulo( $titulo_modulo, 'Confirmação de dados para envio' );
		$db->monta_lista_agrupado($dados, $cabecalho, $soma);
	?>
	<br/>
	<form method="post" name="formulario">
		<input type="hidden" name="_dadosEnvio" value="<?=base64_encode(serialize($dadosPost))?>"/>
		<table  class="tabela" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" align="center">
			<tr>
				<!-- GRUPO MATRIZ -->
				<td colspan="2" align="center" bgcolor="#cccccc">
					Atenção! Confirme os dados acima antes de pressionar o botão enviar. Essas informações serão incluídas no SIDOR, substituíndo as já existentes para cada unidade, ação, localizador e tipo de detalhamento.
				</td>
			</tr>
			<tr>
				<td align='right' class="SubTituloDireita">Usuário SIDOR:</td>
				<td><?=campo_texto('usuario_sidor','S','S','',20,30,'','');?></td>
			</tr>
			<tr>
				<td align='right' class="SubTituloDireita">Senha SIDOR:</td>
				<td><?=campo_texto('senha_sidor','S','S','',20,30,'','');?></td>
			</tr>
			<tr bgcolor="#cccccc">
				<td>&nbsp;</td>
				<td>
					<input type="button" name="bsubmeter" value="Enviar Dados" class="botao" onclick="enviarDadosSidor()"/>
				</td>
			</tr>
		</table>
	</form>
	<script language="javascript">
		function enviarDadosSidor()
		{
			document.formulario.submit();
		}
	</script>	
<? } else { ?>
	<form method="post" name="formulario" action="window.open('elabrev.php?modulo=relatorio/orcamento/orgaosintesedespesa&acao=R', 'formulario', 'width=480,height=460,status=1,toolbar=0,scrollbars=1,resizable=1'); void(0);">
		<input type="hidden" name="modulo" value="<?= $modulo ?>">
		<input type="hidden" name="mtrid" value="<?= $mtrid ?>">
		<input type="hidden" name="acao" value="<?= $_REQUEST['acao'] ?>">
		<input type="hidden" name="act" value="<?= $act ?>">
		<input type='hidden' name="relat" >
		<input type='hidden' name="planilha">
		<input type='hidden' name="tipo" value="6">
		<!-- FORMULÁRIO DE INSERÇÃO / EDIÇÃO -->
		<table class="tabela" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" align="center">
			<tr>
				<!-- TIPO DE RELATÓRIO -->
				<td align='right' class="SubTituloDireita" width="250">Tipo de Relatório:</td>
				<td>
					<script type="text/javascript">
						function altera_tipo_relatorio( input )
						{
							var valor = input.value;
							var filtro_qdd = document.getElementById( 'filtro_qdd' );
							var filtro_sintese = document.getElementById( 'filtro_sintese' );
							if ( valor == 'qdd' )
							{
								filtro_qdd.style.display = 'block';
								filtro_sintese.style.display = 'none';
							}
							else if ( valor == 'sintese' )
							{
								filtro_qdd.style.display = 'none';
								filtro_sintese.style.display = 'block';
							}
							else
							{
								filtro_qdd.style.display = 'none';
								filtro_sintese.style.display = 'none';
							}
							return true;
						}
					</script>
					<?
						$opcoes_tipo = array(
							'QDD' => array(
								'valor' => 'qdd',
								'id' => 'tipo_qdd',
								'callback' => 'altera_tipo_relatorio'
							),
							'Sintese' => array(
								'valor' => 'sintese',
								'id' => 'tipo_sintese',
								'callback' => 'altera_tipo_relatorio'
							)
						);
						$tipo_relatorio = 'qdd';
					?>
					<? campo_radio( 'tipo_relatorio', $opcoes_tipo, 'h' ); ?>
				</td>
			</tr>
		</table>
		<div id="filtro_sintese" style="display:none;">
			<table class="tabela" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" align="center">
				<tr>
					<td align='right' class="SubTituloDireita" width="250">
						Campos para exibição do relatório:
					</td>
					<td>
						<table border="0" cellpadding="5" cellspacing="0">
							<tr>
								<td>
									<input type="checkbox" checked="checked" name="filtro_sintese[]" value="funcao"/> Função
								</td>
								<td>
									<input type="checkbox" checked="checked" name="filtro_sintese[]" value="subfuncao"/> Sub-Função
								</td>
								<td>
									<input type="checkbox" checked="checked" name="filtro_sintese[]" value="programa"/> Programa<br/>
								</td>
							</tr>
							<tr>
								<td>
									<input type="checkbox" checked="checked" name="filtro_sintese[]" value="acao"/> Ação
								</td>
								<td>
									<input type="checkbox" checked="checked" name="filtro_sintese[]" value="gnd"/> GND
								</td>
								<td>
									<input type="checkbox" checked="checked" name="filtro_sintese[]" value="fonte"/> Fonte<br/>
								</td>
							</tr>
							<tr>
								<td colspan="3">
									<input type="checkbox" checked="checked" name="filtro_sintese[]" value="natureza"/> Natureza de Despesa (Plano de Conta)<br/>
								</td>
							</tr>
						</table>
					</td>
				</tr>
				<tr>
					<!-- UNIDADES -->
					<td align='right' class="SubTituloDireita">Unidadesssss Orçamentárias:</td>
					<td>
						<? $sql_unidade = "select unicod as Codigo, unicod|| ' - ' ||unidsc as Descricao from unidade where unitpocod='U' and orgcod= '". CODIGO_ORGAO_SISTEMA. "' and unicod<>'26100' and unistatus = 'A' group by unicod, unidsc order by unicod|| ' - ' ||unidsc"; ?>
						<? combo_popup( 'unidades_sintese', $sql_unidade, 'Selecione a(s) Unidade(s) Orçamentárias', '400x500' ); ?>
					</td>
				</tr>
				<tr>
					<!-- AÇÃO -->
					<td align='right' class="SubTituloDireita">Ações Relacionadas:</td>
					<td>
						<? $sql_acao = "select acacod as Codigo, acacod|| ' - ' ||acadsc as Descricao from elabrev.ppaacao_orcamento group by acacod, acadsc order by acacod|| ' - ' ||acadsc"; ?>
						<? combo_popup( 'acoes_sintese', $sql_acao, 'Selecione a(s) Ação(ões)', '400x400' ); ?>
					</td>
				</tr>
				<tr>
					<!-- TIPO DE DETALHAMENTO -->
					<td align='right' class="SubTituloDireita">Tipo de Detalhamento:</td>
					<td>
						<? $sql_detalhamento = "select tpdid as Codigo, tpdcod || ' - ' || tpddsc as Descricao from elabrev.tipodetalhamento where tpdstatus='A'  order by tpdcod, tpddsc"; ?>
						<? combo_popup( 'detalhamento_sintese', $sql_detalhamento, 'Selecione o(s) Tipo(s) de Detalhamento', '400x500' ); ?>
					</td>
				</tr>
			</table>
		</div>
		<div id="filtro_qdd" style="display:none;">
			<table class="tabela" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" align="center">
				<tr>
					<td align='right' class="SubTituloDireita" width="250">
						Forma de apresentação do relatório:
					</td>
					<td>
						<?
							$opcoes_tipo_qdd = array(
								'Completo' => array(
									'valor' => 'completo',
									'id' => 'tipo_qdd_completo'
								),
								'Simplificado' => array(
									'valor' => 'simplificado',
									'id' => 'tipo_qdd_simplificado'
								)
							);
							$tipo_relatorio_qdd = 'completo';
						?>
						<? campo_radio( 'tipo_relatorio_qdd', $opcoes_tipo_qdd, 'h' ); ?>
					</td>
				</tr>
				<tr style="display:none;">
					<!-- GRUPO MATRIZ -->
					<td align='right' class="SubTituloDireita">Programas:</td>
					<td>
						<? $filtro = array(); ?>
						<? $sql_programas = " select prgcod as Codigo, prgcod|| ' - ' ||prgdsc as Descricao from elabrev.ppaprograma_orcamento order by prgcod|| ' - ' ||prgdsc "; ?>
						<? combo_popup( 'prgcod', $sql_programas, 'Selecione os Programas', '600x700', 0,  $filtro, '' ); ?>
					</td>
				</tr>
				<tr>
					<!-- AÇÃO -->
					<td align='right' class="SubTituloDireita">Ações Relacionadas:</td>
					<td>
						<? $sql_acao = "select acacod as Codigo, acacod|| ' - ' ||acadsc as Descricao from elabrev.ppaacao_orcamento group by acacod, acadsc order by acacod|| ' - ' ||acadsc"; ?>
						<? combo_popup( 'acacod', $sql_acao, 'Selecione a(s) Ação(ões)', '400x400', 0, $filtro, '' ); ?>
					</td>
				</tr>
				<tr>
					<!-- LOCALIZADOR -->
					<td align='right' class="SubTituloDireita">IDOC:</td>
					<td>
						<? $sql_idoc = "select idocod as Codigo, idocod|| ' - ' ||idodsc as Descricao from idoc order by idocod|| ' - ' ||idodsc"; ?>
						<? combo_popup( 'idocod', $sql_idoc, 'Selecione o(s) IDOC(s)', '400x400', 0, $filtro, '' ); ?>
					</td>
				</tr>
				<tr>
					<!-- PRODUTOS -->
					<td align='right' class="SubTituloDireita">Natureza:</td>
					<td>
						<? $sql_natureza = "select ndpcod as Codigo, ndpcod|| ' - ' ||ndpdsc as Descricao from naturezadespesa where edpcod <> '00' order by ndpcod|| ' - ' ||ndpdsc"; ?>
						<? combo_popup( 'ndpcod', $sql_natureza, 'Selecione a(s) Natureza(s)', '400x400', 0, $filtro, '' ); ?>
					</td>
				</tr>
				<tr>
					<!-- FONTES -->
					<td align='right' class="SubTituloDireita">Fontes de Recurso:</td>
					<td>
					<? $sql_fonte = "select foncod as codigo, foncod|| ' - ' ||fondsc as Descricao from fonterecurso order by foncod|| ' - ' ||fondsc"; ?>
						<? combo_popup( 'fonte', $sql_fonte, 'Selecione a(s) Fonte(s) de Recurso', '600x700', 0, $filtro, '' ); ?>
					</td>
				</tr>
				<tr>
					<!-- GND -->
					<td align='right' class="SubTituloDireita">GND:</td>
					<td>
						<? $sql_gnd = "select gndcod as Codigo, gndcod|| ' - ' ||gnddsc as Descricao from gnd order by gndcod|| ' - ' ||gnddsc "; ?>
						<? combo_popup( 'gnd', $sql_gnd, 'Selecione o(s) GND', '400x400', 0, $filtro, '' ); ?>
					</td>
				</tr>
				<tr>
					<!-- UNIDADES -->
					<td align='right' class="SubTituloDireita">Unidades Orçamentárias:</td>
					<td>
						<? $sql_unidade = "select unicod as Codigo, unicod || ' - ' || unidsc as Descricao from unidade where unitpocod='U' and orgcod= '". CODIGO_ORGAO_SISTEMA. "' and unicod<>'26100' and unicod<'27000' and unistatus = 'A' group by unicod, unidsc order by unicod|| ' - ' ||unidsc"; ?>
						<? combo_popup( 'unidades', $sql_unidade, 'Selecione a(s) Unidade(s) Orçamentárias', '400x500', 0, $filtro, '' ); ?>
					</td>
				</tr>
				<tr>
					<!-- TIPO DE DETALHAMENTO -->
					<td align='right' class="SubTituloDireita">Tipo de Detalhamento:</td>
					<td>
						<? $sql_detalhamento = "select tpdid as Codigo, tpdcod || ' - ' || tpddsc as Descricao from elabrev.tipodetalhamento where tpdstatus='A' and tpdano = '" . $_SESSION['exercicio'] . "' order by tpdcod, tpddsc"; ?>
						<? combo_popup( 'detalhamento', $sql_detalhamento, 'Selecione o(s) Tipo(s) de Detalhamento', '400x500', 0, $filtro, '' ); ?>
					</td>
				</tr>
			</table>
		</div>
		<?if (!$_SESSION['superuser']) $ativo = 'disabled="disabled"';?>
		<table class="tabela" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" align="center">
			<tr>
				<!-- TIPO DE ARQUIVO -->
				<td align='right' class="SubTituloDireita" width="250">Tipo de Arquivo:</td>
				<td>
					<input type="radio" name="tipo_arquivo" value="1" id="mtrmatriz_1" <?= $tipo_arquivo==1 || !$tipo_arquivo ? 'checked="checked"' : '' ?> onclick="document.formulario.tipo.value=1;">
					<label for="mtrmatriz_1">HMTL</label>
					<input type="radio" name="tipo_arquivo" value="2" id="mtrmatriz_2" <?= $tipo_arquivo==2 ? 'checked="checked"' : ''  ?> onclick="document.formulario.tipo.value=2;">
					<label for="mtrmatriz_2">XLS</label>
					<input type="radio" name="tipo_arquivo" <?=$ativo?> value="3" id="mtrmatriz_3" <?= $tipo_arquivo==3 ? 'checked="checked"' : ''  ?> onclick="document.formulario.tipo.value=3;">
					<label for="mtrmatriz_3">Exportar para o SIDOR</label>
				</td>
			</tr>
			<tr bgcolor="#cccccc">
				<td>&nbsp;</td>
				<td>
					<input type="button" name="bsubmeter" value="Gerar Dados" class="botao" onclick="imprime_rel(document.formulario.tipo.value);"/>
				</td>
			</tr>
		</table>
		<!-- FIM FORMULÁRIO DE INSERÇÃO / EDIÇÃO -->
	</form>
	<?
		switch( $_REQUEST["tipo_arquivo"] )
		{
			case 1: // html
				if ( $_REQUEST["tipo_relatorio"] == 'sintese' )
				{
					?>
					<script language="javascript">
						document.formulario.action=window.open("elabrev.php?modulo=relatorio/orcamento/orgaosintesedespesa&acao=R", "formulario", "width=480,height=460,status=1,toolbar=0,scrollbars=1,resizable=1");
						document.submit();
					</script>
					<?
				}
				return;
			break;
		}
	?>		
	<script language="javascript">
		function imprime_rel( cod )
		{
			<? if(!$ppoid) { ?>			 
				alert('Não existe orçamento previsto para este ano de exercício');
			<? } else { ?>
				var formulario = document.formulario;
				var tipos = formulario.tipo_relatorio;
			
				if ( tipos[0].checked === false && tipos[1].checked === false )
				{
					alert( 'Escolha o tipo de relatório.' );
					return;
				}
				if ( tipos[1].checked === true )
				{
					formulario.action = 'elabrev.php?modulo=relatorio/orcamento/orgaosintesedespesa&acao=R';
				}
				else
				{
					formulario.action = 'elabrev.php?modulo=relatorio/orcamento/relatorioquadrocreditosmecfiltro&acao=A';
				}
				prepara_formulario();
				if ( formulario.tipo_arquivo[0].checked === true )
				{
					var janela = window.open("", "formulariopopup", "width=700,height=460,status=1,menubar=1,toolbar=0,scrollbars=1,resizable=1");
					formulario.target = 'formulariopopup';
				}
				else
				{
					formulario.target = '_top';
				}
				formulario.planilha.value = cod;         	
				formulario.submit();
			<? } ?>
		}
		// inicia os filtros QDD abertos
		altera_tipo_relatorio( document.getElementById( 'tipo_qdd' ) );
	</script>
<? } // else ?>
