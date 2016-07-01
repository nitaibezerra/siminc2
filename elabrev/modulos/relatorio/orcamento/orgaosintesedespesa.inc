<?php

	/*
		Sistema Simec
		Setor responsável: SPO-MEC
		Desenvolvedor: Equipe Consultores Simec
		Analista: Gilberto Arruda Cerqueira Xavier, Cristiano Cabral (cristiano.cabral@gmail.com)
		Programador: Henrique Xavier Couto (e-mail: henriquexcouto@gmail.com), Renan de Lima Barbosa (e-mail: renandelima@gmail.com), Fabrício Mendonça (e-mail: fabriciomendonca@gmail.com)
		Módulo: relatorioSintese.inc
		Finalidade: relatório dos quadros orçamentarios, detalha os gastos da unidade por program, função, subfunção, grupo de despesa e fonte
	*/


//include APPRAIZ . "includes/cabecalho.inc";


// define as unidades
$dados_unidade = $_REQUEST['unidades_sintese'];
for ( $i = count( $dados_unidade ) - 1; $i >= 0 && $dados_unidade[$i] != ''; $i--  )
{
	$unidades .= ", '" . $dados_unidade[$i] . "'";
}
$unidades = substr( $unidades, 1 );
if ( $unidades ) $unidades = " a.unicod in ( " . $unidades . " )  and ";
// FIM define as unidades

// define as ações
$dados_acao = $_REQUEST['acoes_sintese'];
for ( $i = count( $dados_acao ) - 1; $i >= 0 && $dados_acao[$i] != ''; $i--  )
{
	$acoes .= ", '" . $dados_acao[$i] . "'";
}
$acoes = substr( $acoes, 1 );
if ( $acoes ) $acoes = " a.acacod in ( " . $acoes . " )  and ";
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

?>
	
<head>
	<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
	<link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>
	<title>Quadro Síntese</title>
	<style type="text/css">
		span.imprimir { position: absolute; top: 3px; margin: 0; padding: 5px; position: fixed; background-color: #f0f0f0; border: 1px solid #909090; cursor:pointer; }
		span.imprimir:hover { background-color: #d0d0d0; }
	</style>
	<script type="text/javascript">
		self.focus();
	</script>
</head>	
<br/>
<? // $db->cria_aba( $abacod_tela, $url, '' ); ?>
<form method="POST" name="formulario" style="margin: 0;">
<input type="hidden" name="ppoid" value="<?=$ppoid?>">
<!-- <input type="hidden" name="unidade" value="<?=$unidade?>"> -->
<input type="hidden" name="acao" value="">
		<div>
			<span class="imprimir" onclick="window.print();"><img src="/imagens/print.gif"/></span>
			<p align="center" style="font-size: 18px; font-family: Arial; margin: 0;">Ministério da Educação</p>
			<p align="center" style="font-size: 14px; font-family: Arial; margin: 0;">Quadro Síntese</p>
			<p align="center" style="font-size: 12px; font-family: Arial; margin: 10px;">
				<br/><br/>
				<b>Unidades filtradas</b><br/>
				<? if ( $_REQUEST['unidades'][0] != '' ) : ?>
					<? $sql_mostra_filtro = "select unicod || ' - ' || unidsc as descricao from unidade where unicod in ( '" . implode( "','", $_REQUEST['unidades'] ) . "' ) "; ?>
					<table border="0" cellpadding="0" cellspacing="0" align="center" >
						<tr><td><div style="text-align:left;font-size:8pt;">
						<? foreach ( $db->carregar( $sql_mostra_filtro ) as $unidade ) : ?>
							<?= $unidade['descricao'] ?></br>
						<? endforeach; ?>
						</div></td></tr>
					</table>
				<? else : ?>Todas<? endif; ?>
				<br/><br/>
				<b>Ações</b><br/>
				<? if ( $acao_restricao ) : ?>
					<? $sql_mostra_filtro = "select acacod || ' - ' || acadsc as descricao from elabrev.ppaacao_orcamento a where 1 = 1 " . $acao_restricao; ?>
					<table border="0" cellpadding="0" cellspacing="0" align="center" >
						<tr><td><div style="text-align:left;font-size:8pt;">
						<? foreach ( $db->carregar( $sql_mostra_filtro ) as $acao ) : ?>
							<?= $acao['descricao'] ?><br/>
						<? endforeach; ?>
						</div></td></tr>
					</table>
				<? else : ?>Todas<? endif; ?>
				<br/><br/>
				<b>Tipo de Detalhamento</b><br/>
				<? if ( $detalhamento_restricao ) : ?>
					<? $sql_mostra_filtro = "select tpdcod || ' - ' || tpddsc as descricao from elabrev.tipodetalhamento tda where 1 = 1 " . $detalhamento_restricao; ?>
					<table border="0" cellpadding="0" cellspacing="0" align="center">
						<tr><td><div style="text-align:left;font-size:8pt;">
						<? foreach ( $db->carregar( $sql_mostra_filtro ) as $tipodetalhamento ) : ?>
							<?= $tipodetalhamento['descricao'] ?><br/>
						<? endforeach; ?>
						</div></td></tr>
					</table>
				<? else : ?>Todos<? endif; ?>
			</p>
		</div>
	<table class="tabela" cellSpacing="0" width="95%" bgcolor="#f5f5f5" cellPadding="2" border="0" align="center">	
<?

		$Query = "select ".
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
				 "		a.prgano = '".$_SESSION['exercicio']."' and ".
				 "		a.acastatus='A' and " . $unidades . $acoes . $detalhamentos .
				 "		a.acasnrap = false  ".
				 "	group by ".
				 "		ppa.prgcod, ".
				 "		ppa.prgdsc ".
				 "	order by ".
				 "		ppa.prgcod, ".
				 "		ppa.prgdsc ";
		$RS = $db->record_set($Query);
	    $nlinhas0 = $db->conta_linhas($RS);

	    
		if ( $nlinhas0 < 0 )
		{
			?>
			</table>
			<p align="center" style="color: #904040;">
				<br />
				Não existe detalhamento orçamentário para esta unidade.
			</p>
			<?
		}
		else
		{

?>

	

<?
		$Query = "select ".
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
				 "		a.prgano = '".$_SESSION['exercicio']."' and ".
				 "		a.acastatus='A' and " . $unidades . $acoes . $detalhamentos .
		  		"		a.acasnrap = false  ".
				 "	group by ".
				 "		ppa.prgcod, ".
				 "		ppa.prgdsc ".
				 "	order by ".
				 "		ppa.prgcod, ".
				 "		ppa.prgdsc ";

        $RS = $db->record_set($Query);
	    $nlinhas1 = $db->conta_linhas($RS);

	    if ( $nlinhas1 >= 0 )
		{
?>
		<tr bgcolor="#DCDCDC">
			<td width="70%" align="left">&nbsp;&nbsp;</td>
			<td align="right" width="30%" colspan="2"><b>Valor</b></td>
		</tr>		
<?
		  	$grupo = 0;
		  	$grupoNovo = 0;
		  	$vlTotal = 0;
		  	$vlDespesa = 0;
		
		  	for ($i=0; $i<=$nlinhas1;$i++)
			{
		  	$res = $db->carrega_registro($RS,$i);
		  	$vlTotal = $vlTotal + $res[2];
			}		  	
?>
		<tr onmouseover="this.bgColor='#ffffcc';" onmouseout="this.bgColor='';">
			<td align="left" class="SemBordaCimaBaixoDireita">Total</td>
			<td align="right" class="SemBordaCimaBaixo" colspan="2"><?=number_format(($vlTotal), 0, ',', '.')?></td>
		</tr>
<?
		}

?>
	


<?
		$Query = "select ". 
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
				 "		a.prgano = '".$_SESSION['exercicio']."' and ".
				 "		a.acastatus='A' and " . $unidades . $acoes . $detalhamentos .
				 "		a.acasnrap = false  ".
				 "	group by ".
				 "		f.funcod, ".
				 "		f.fundsc ".
				 "	order by ".
				 "		f.funcod, ".
				 "		f.fundsc ";

        $RS = $db->record_set($Query);
	    $nlinhas2 = $db->conta_linhas($RS);
	    if ($nlinhas2>=0 && isset( $_REQUEST['filtro_sintese'] ) && in_array( 'funcao', $_REQUEST['filtro_sintese'] ) )
		{
?>
		<tr bgcolor="#DCDCDC">
			<td width="70%" align="left"><b>Função</b></td>
			<td align="right" width="30%" colspan="2"><b>Valor</b></td>
		</tr>
<?
		  	for ($i=0; $i<=$nlinhas2;$i++)
			{
		  	$res = $db->carrega_registro($RS,$i);
		  	
?>
		<tr onmouseover="this.bgColor='#ffffcc';" onmouseout="this.bgColor='';">
			<td align="left" class="SemBordaCimaBaixoDireita"><?=str_to_upper( $res[0] )?> - <?=str_to_upper( $res[1] )?></td>
			<td align="right" class="SemBordaCimaBaixo" colspan="2"><?=number_format(($res[2]), 0, ',', '.')?></td>
		</tr>
<?
			}
		}
?>


	
<?
		$Query = "select ".
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
				 "		a.prgano = '".$_SESSION['exercicio']."' and ".
				 "		a.acastatus='A' and " . $unidades . $acoes . $detalhamentos .
				 "		a.acasnrap = false  ".
				 "	group by ".
				 "		f.sfucod, ".
				 "		f.sfudsc ".
				 "	order by ".
				 "		f.sfucod, ".
				 "		f.sfudsc ";

        $RS = $db->record_set($Query);
	    $nlinhas3 = $db->conta_linhas($RS);
	    if ($nlinhas3>=0 && isset( $_REQUEST['filtro_sintese'] ) && in_array( 'subfuncao', $_REQUEST['filtro_sintese'] ) )
		{
		?>
		<tr bgcolor="#DCDCDC">
			<td width="70%" align="left"><b>SubFunção</b></td>
			<td align="right" width="30%" colspan="2"><b>Valor</b></td>
		</tr>	
	<?
		  	for ($i=0; $i<=$nlinhas3;$i++)
			{
		  	$res = $db->carrega_registro($RS,$i);
		  	
?>
		<tr onmouseover="this.bgColor='#ffffcc';" onmouseout="this.bgColor='';">
			<td align="left" class="SemBordaCimaBaixoDireita"><?=str_to_upper( $res[0] )?> - <?=str_to_upper( $res[1] )?></td>
			<td align="right" class="SemBordaCimaBaixo" colspan="2"><?=number_format(($res[2]), 0, ',', '.')?></td>
		</tr>
<?
			}
		}
?>


<?
		$Query = "select ".
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
				 "		a.prgano = '".$_SESSION['exercicio']."' and ".
				 "		a.acastatus='A' and " . $unidades . $acoes . $detalhamentos .
		 		 "		a.acasnrap = false  ".
				 "	group by ".
				 "		ppa.prgcod, ".
				 "		ppa.prgdsc ".
				 "	order by ".
				 "		ppa.prgcod, ".
				 "		ppa.prgdsc ";

        $RS = $db->record_set($Query);
	    $nlinhas4 = $db->conta_linhas($RS);

	    if ($nlinhas4>=0 && isset( $_REQUEST['filtro_sintese'] ) && in_array( 'programa', $_REQUEST['filtro_sintese'] ))
		{
	?>
		<tr bgcolor="#DCDCDC">
			<td width="70%" align="left"><b>Programa</b></td>
			<td align="right" width="30%" colspan="2"><b>Valor</b></td>
		</tr>
	<?
		  	for ($i=0; $i<=$nlinhas4;$i++)
			{
		  	$res = $db->carrega_registro($RS,$i);
		  	
?>
		<tr onmouseover="this.bgColor='#ffffcc';" onmouseout="this.bgColor='';">
			<td align="left" class="SemBordaCimaBaixoDireita"><?=str_to_upper( $res[0] )?> - <?=str_to_upper( $res[1] )?></td>
			<td align="right" class="SemBordaCimaBaixo" colspan="2"><?=number_format(($res[2]), 0, ',', '.')?></td>
		</tr>
<?
			}

		}
?>
	

	
	
<?

		$Query = "select ".
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
				 "		a.prgano = '".$_SESSION['exercicio']."' and ".
				 "		a.acastatus='A' and " . $unidades . $acoes . $detalhamentos .
				 "		a.acasnrap = false  ".
				 "	group by ".
				 "		a.acacod, ".
				 "		a.acadsc ".
				 "	order by ".
				 "		a.acacod, ".
				 "		a.acadsc ";

		$RS = $db->record_set($Query);
	    $nlinhas5 = $db->conta_linhas($RS);


		if ($nlinhas>=0 && isset( $_REQUEST['filtro_sintese'] ) && in_array( 'acao', $_REQUEST['filtro_sintese'] ))
		{
		  	$grupo = 0;
		  	$grupoNovo = 0;
		  	$vlTotal = 0;
		  	$vlDespesa = 0;
		?>
		<tr bgcolor="#DCDCDC">
			<td width="70%" align="left"><b>Ação</b></td>
			<td align="right" width="30%" colspan="2"><b>Valor</b></td>
		</tr>	
		<?
		  	for ($i=0; $i<=$nlinhas5;$i++)
			{
		  	$res = $db->carrega_registro($RS,$i);
?>
		<tr onmouseover="this.bgColor='#ffffcc';" onmouseout="this.bgColor='';">
			<td align="left" class="SemBordaCimaBaixoDireita"><?=str_to_upper( $res[0] )?> - <?=str_to_upper( $res[1] )?></td>
			<td align="right" class="SemBordaCimaBaixo" colspan="2"><?=number_format(($res[2]), 0, ',', '.')?></td>
		</tr>
<?
			}		  	
		}
?>



	
	

	

	
	
	
	
<?
		$Query = "select ".	
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
				 "		inner join elabrev.tipodetalhamentoacao tda on ".
				 "			tda.acaid = a.acaid ".
				 "	where ".
				 "		a.prgano = '".$_SESSION['exercicio']."' and ".
				 "		a.acastatus='A' and " . $unidades . $acoes . $detalhamentos .
				 "		a.acasnrap = false  ".
				 "	group by ".
				 "		gnd.gndcod, ".
				 "		gnd.gnddsc ".
				 "	order by ".
				 "		gnd.gndcod, ".
				 "		gnd.gnddsc ";

        $RS = $db->record_set($Query);
	    $nlinhas6 = $db->conta_linhas($RS);
	    if ($nlinhas6>=0 && isset( $_REQUEST['filtro_sintese'] ) && in_array( 'gnd', $_REQUEST['filtro_sintese'] ))
		{
		?>
		<tr bgcolor="#DCDCDC">
			<td width="70%" align="left"><b>Grupo de Natureza de Despesa (GND)</b></td>
			<td align="right" width="30%" colspan="2"><b>Valor</b></td>
		</tr>	
		<?
		  	for ($i=0; $i<=$nlinhas6;$i++)
			{
		  	$res = $db->carrega_registro($RS,$i);
		  	$vlTotal = $vlTotal + $res[2];
		  	$vlDespesa = $vlDespesa + $res[3];
		  	
?>
		<tr onmouseover="this.bgColor='#ffffcc';" onmouseout="this.bgColor='';">
			<td align="left" class="SemBordaCimaBaixoDireita"><?=str_to_upper( $res[0] )?> - <?=str_to_upper( $res[1] )?></td>
			<td align="right" class="SemBordaCimaBaixo" colspan="2"><?=number_format(($res[2]), 0, ',', '.')?></td>
		</tr>
<?
			}
		}
?>
	

	
	
	
<?
		$Query = "select ".
                 "			f.foncod, ".
				 "			f.fondsc,  ".
				 "		coalesce( SUM(ac.dpavalor) , 0) as valor ".
				 "	from elabrev.despesaacao ac ".
				 "		inner join elabrev.ppaacao_orcamento a ON ".
				 "			a.acaid = ac.acaid ".
				 "		inner join public.fonterecurso f ON ".
				 "			f.foncod = ac.foncod ".
				 "		inner join elabrev.ppaprograma_orcamento ppa ON ".
				 "			ppa.prgid = a.prgid ".
				 "		inner join unidade b on ".
				 "			a.unicod = b.unicod ".
				 "		inner join elabrev.tipodetalhamentoacao tda on ".
				 "			tda.acaid = a.acaid ".
				 "	where ".
				 "		a.prgano = '".$_SESSION['exercicio']."' and ".
				 "		a.acastatus='A' and " . $unidades . $acoes . $detalhamentos .
				 "		a.acasnrap = false  ".
				 "	group by ".
				 "		f.foncod, ".
				 "		f.fondsc ".
				 "	order by ".
				 "		f.foncod, ".
				 "		f.fondsc ";

        $RS = $db->record_set($Query);
        $nlinhas7 = $db->conta_linhas($RS);
	    if ($nlinhas7>=0 && isset( $_REQUEST['filtro_sintese'] ) && in_array( 'fonte', $_REQUEST['filtro_sintese'] ) )
		{
		?>
		<tr bgcolor="#DCDCDC">
			<td width="70%" align="left"><b>Fonte</b></td>
			<td align="right" width="30%" colspan="2"><b>Valor</b></td>
		</tr>
		<?
		  	for ($i=0; $i<=$nlinhas7;$i++)
			{
		  	$res = $db->carrega_registro($RS,$i);
		  	
?>
		<tr onmouseover="this.bgColor='#ffffcc';" onmouseout="this.bgColor='';">
			<td align="left" class="SemBordaCimaBaixoDireita"><?=str_to_upper( $res[0] )?> - <?=str_to_upper( $res[1] )?></td>
			<td align="right" class="SemBordaCimaBaixo" colspan="2"><?=number_format(($res[2]), 0, ',', '.')?></td>
		</tr>
<?
			}
		}

// força exibição por natureza
if ( !isset( $_REQUEST['filtro_sintese'] ) )
{
	$_REQUEST['filtro_sintese'] = array( 'natureza' );
}		

if ( isset( $_REQUEST['filtro_sintese'] ) && in_array( 'natureza', $_REQUEST['filtro_sintese'] ) )
{

	$Query = "select codigo, descricao, SUM(valor) as valor from 
			 		(select  nd.ndpcod as codigo, nd.ndpdsc as descricao,  
					case when unicod is null then 0 else coalesce( sum( dpavalor ), 0 ) end as valor  
					from naturezadespesa nd  
					left join elabrev.despesaacao da ON da.ndpid = nd.ndpid and da.ppoid = (select ppoid from elabrev.propostaorcamento p where ppoanoexercicio = '".$_SESSION['exercicio']."' and tppid = 1 and ppostatus = 'A')  
					left join elabrev.ppaacao_orcamento a ON 
						$unidades 
					a.acaid = da.acaid and a.prgano = '{$_SESSION['exercicio']}'
					group by nd.ndpcod, nd.ndpdsc , a.unicod
					order by nd.ndpcod) as foo
					group by codigo, descricao
					order by codigo"; 

/*	$Query = " select " .
   			" nd.ndpcod as codigo, " .
			" nd.ndpdsc as descricao, " .
			" coalesce( sum( dpavalor ), 0 ) as valor " .
		" from naturezadespesa nd " .
			" left join elabrev.despesaacao da ON da.ndpid = nd.ndpid " .
			" left join elabrev.ppaacao_orcamento a ON " . $unidades . " a.acaid = da.acaid " . 
			" group by nd.ndpcod, nd.ndpdsc order by nd.ndpcod ";*/

	// CAPTURA DADOS DE DESPESA POR NATUREZA
	$despesa_por_natureza = $db->carregar( $Query );
	$estrutura = array();
	$valor_total_natureza = 0;
	foreach ( $despesa_por_natureza as $dados )
	{
		$codigo = $dados['codigo'];
		$descricao = $dados['descricao'];
		$valor = $dados['valor'];
		$indice_0 = substr( $codigo, 0, 1 );
		$indice_1 = substr( $codigo, 1, 1 );
		$indice_2 = substr( $codigo, 2, 2 );
		$indice_3 = substr( $codigo, 4, 2 );
		$indice_4 = substr( $codigo, 6, 2 );
		$nova_estrutura = array(
			'codigo' => $codigo,			// gndcod
			'descricao' => $descricao,		// descrição da natureza
			'valor' => $valor,				// valor gasto pela natureza
			'valor_filhos' => 0,			// soma dos valores gastos pelos filhos
			'visivel' => false,				// indica se algum filho possui valor (caso nivel 4 indica se o valor da natureza em questão é maior que zero)
			'filhos' => array()				// lista com os subitens da natureza (nível 4 sempre vazio)
		);
		$valor_total_natureza += $valor;
		switch ( true )
		{
			case substr( $codigo, 1 ) == '0000000': // nivel 0
				$estrutura[$indice_0] = $nova_estrutura;
				if ( $valor > 0 )
				{
					$estrutura[$indice_0]['visivel'] = true;
				}
				break;
			case substr( $codigo, 2 ) == '000000': // nivel 1
				$estrutura[$indice_0]['filhos'][$indice_1] = $nova_estrutura;
				if ( $valor > 0 )
				{
					$estrutura[$indice_0]['filhos'][$indice_1]['visivel'] = true;
					$estrutura[$indice_0]['visivel'] = true;
				}
				$estrutura[$indice_0]['valor_filhos'] += $valor;
				break;
			case substr( $codigo, 4 ) == '0000': // nivel 2
				$estrutura[$indice_0]['filhos'][$indice_1]['filhos'][$indice_2] = $nova_estrutura;
				if ( $valor > 0 )
				{
					$estrutura[$indice_0]['filhos'][$indice_1]['filhos'][$indice_2]['visivel'] = true;
					$estrutura[$indice_0]['filhos'][$indice_1]['visivel'] = true;
					$estrutura[$indice_0]['visivel'] = true;
				}
				$estrutura[$indice_0]['filhos'][$indice_1]['valor_filhos'] += $valor;
				$estrutura[$indice_0]['valor_filhos'] += $valor;
				break;
			case substr( $codigo, 6 ) == '00': // nivel 3
				$estrutura[$indice_0]['filhos'][$indice_1]['filhos'][$indice_2]['filhos'][$indice_3] = $nova_estrutura;
				if ( $valor > 0 )
				{
					$estrutura[$indice_0]['filhos'][$indice_1]['filhos'][$indice_2]['filhos'][$indice_3]['visivel'] = true;
					$estrutura[$indice_0]['filhos'][$indice_1]['filhos'][$indice_2]['visivel'] = true;
					$estrutura[$indice_0]['filhos'][$indice_1]['visivel'] = true;
					$estrutura[$indice_0]['visivel'] = true;
				}
				$estrutura[$indice_0]['filhos'][$indice_1]['filhos'][$indice_2]['valor_filhos'] += $valor;
				$estrutura[$indice_0]['filhos'][$indice_1]['valor_filhos'] += $valor;
				$estrutura[$indice_0]['valor_filhos'] += $valor;
				break;
			default: // nivel 4
				$estrutura[$indice_0]['filhos'][$indice_1]['filhos'][$indice_2]['filhos'][$indice_3]['filhos'][$indice_4] = $nova_estrutura;
				if ( $valor > 0 )
				{
					$estrutura[$indice_0]['filhos'][$indice_1]['filhos'][$indice_2]['filhos'][$indice_3]['filhos'][$indice_4]['visivel'] = true;
					$estrutura[$indice_0]['filhos'][$indice_1]['filhos'][$indice_2]['filhos'][$indice_3]['visivel'] = true;
					$estrutura[$indice_0]['filhos'][$indice_1]['filhos'][$indice_2]['visivel'] = true;
					$estrutura[$indice_0]['filhos'][$indice_1]['visivel'] = true;
					$estrutura[$indice_0]['visivel'] = true;
				}
				$estrutura[$indice_0]['filhos'][$indice_1]['filhos'][$indice_2]['filhos'][$indice_3]['valor_filhos'] += $valor;
				$estrutura[$indice_0]['filhos'][$indice_1]['filhos'][$indice_2]['valor_filhos'] += $valor;
				$estrutura[$indice_0]['filhos'][$indice_1]['valor_filhos'] += $valor;
				$estrutura[$indice_0]['valor_filhos'] += $valor;
				break;
		}
	}
	// FIM CAPTURA DADOS DE DESPESA POR NATUREZA
	// EXIBE DADOS DE DESPESA POR NATUREZA
	$cor = 1;
	if ( $estrutura )
	{
		?>
		<tr bgcolor="#DCDCDC">
			<td width="70%" align="left"><b>Natureza de Despesa</b> (Plano de Contas)</td>
			<td align="right" width="15%"><b>Distribuído</b></td>
			<td align="right" width="15%"><b>Total</b></td>
		</tr>
		<?
		foreach ( $estrutura as $dados_0 )
		{
			if ( $dados_0['visivel'] == false )
			{
				continue;
			}
			$cor = $cor == 'ebebeb' ? 'f5f5f5' : 'ebebeb';
			?>
			<tr bgcolor="#<?= $cor ?>" onmouseover="this.bgColor='#ffffcc';" onmouseout="this.bgColor='#<?= $cor ?>';">
				<td align="left" class="SemBordaCimaBaixoDireita">
					<?=
						substr( $dados_0['codigo'], 0, 1 ) . '.' .
						substr( $dados_0['codigo'], 1, 1 ) . '.' .
						substr( $dados_0['codigo'], 2, 2 ) . '.' .
						substr( $dados_0['codigo'], 4, 2 ) . '.' .
						substr( $dados_0['codigo'], 6, 2 )
					?> - <?= strtoupper( $dados_0['descricao'] ) ?>
				</td>
				<td align="right">&nbsp;</td>
				<td align="right">
					<?= number_format( $dados_0['valor_filhos'] , 0, ',', '.') ?>
				</td>
			</tr>
			<?
			foreach ( $dados_0['filhos'] as $dados_1 )
			{
				if ( $dados_1['visivel'] == false )
				{
					continue;
				}
				$cor = $cor == 'ebebeb' ? 'f5f5f5' : 'ebebeb';
				?>
				<tr bgcolor="#<?= $cor ?>" onmouseover="this.bgColor='#ffffcc';" onmouseout="this.bgColor='#<?= $cor ?>';">
					<td align="left" class="SemBordaCimaBaixoDireita" style="padding: 0 0 0 15px;">
						<img src="/imagens/seta_filho.gif" align="top"/>
						<?=
							substr( $dados_1['codigo'], 0, 1 ) . '.' .
							substr( $dados_1['codigo'], 1, 1 ) . '.' .
							substr( $dados_1['codigo'], 2, 2 ) . '.' .
							substr( $dados_1['codigo'], 4, 2 ) . '.' .
							substr( $dados_1['codigo'], 6, 2 )
						?> - <?= strtoupper( $dados_1['descricao'] ) ?>
					</td>
					<td align="right" class="SemBordaCimaBaixo">&nbsp;</td>
					<td align="right" class="SemBordaCimaBaixo">
						<?= number_format( $dados_1['valor_filhos'] , 0, ',', '.') ?>
					</td>
				</tr>
				<?
				foreach ( $dados_1['filhos'] as $dados_2 )
				{
					if ( $dados_2['visivel'] == false )
					{
						continue;
					}
					$cor = $cor == 'ebebeb' ? 'f5f5f5' : 'ebebeb';
					?>
					<tr bgcolor="#<?= $cor ?>" onmouseover="this.bgColor='#ffffcc';" onmouseout="this.bgColor='#<?= $cor ?>';">
						<td align="left" class="SemBordaCimaBaixoDireita" style="padding: 0 0 0 30px;">
							<img src="/imagens/seta_filho.gif" align="top"/>
							<?=
								substr( $dados_2['codigo'], 0, 1 ) . '.' .
								substr( $dados_2['codigo'], 1, 1 ) . '.' .
								substr( $dados_2['codigo'], 2, 2 ) . '.' .
								substr( $dados_2['codigo'], 4, 2 ) . '.' .
								substr( $dados_2['codigo'], 6, 2 )
							?>
							<font color="#333333"> - <?= $dados_2['descricao'] ?></font>
						</td>
						<td align="right" class="SemBordaCimaBaixo">&nbsp;</td>
						<td align="right" class="SemBordaCimaBaixo">
							<font color="#333333">
								<?= number_format( $dados_2['valor_filhos'] , 0, ',', '.') ?>
							</font>
						</td>
					</tr>
					<?
					foreach ( $dados_2['filhos'] as $dados_3 )
					{
						if ( $dados_3['visivel'] == false )
						{
							continue;
						}
						$cor = $cor == 'ebebeb' ? 'f5f5f5' : 'ebebeb';
						?>
						<tr bgcolor="#<?= $cor ?>" onmouseover="this.bgColor='#ffffcc';" onmouseout="this.bgColor='#<?= $cor ?>';">
							<td align="left" class="SemBordaCimaBaixoDireita" style="padding: 0 0 0 45px;">
								<img src="/imagens/seta_filho.gif" align="top"/>
								<?=
									substr( $dados_3['codigo'], 0, 1 ) . '.' .
									substr( $dados_3['codigo'], 1, 1 ) . '.' .
									substr( $dados_3['codigo'], 2, 2 ) . '.' .
									substr( $dados_3['codigo'], 4, 2 ) . '.' .
									substr( $dados_3['codigo'], 6, 2 )
								?>
								<font color="#555555"> - <?= $dados_3['descricao'] ?></font>
							</td>
							<td align="right" class="SemBordaCimaBaixo">
								<font color="#555555">
									<?= number_format( $dados_3['valor'] , 0, ',', '.') ?>
								</font>
							</td>
							<td align="right" class="SemBordaCimaBaixo">
								<font color="#555555">
									<?= number_format( $dados_3['valor_filhos'] + $dados_3['valor'] , 0, ',', '.') ?>
								</font>
							</td>
						</tr>
						<?
						foreach ( $dados_3['filhos'] as $dados_4 )
						{
							if ( $dados_4['visivel'] == false )
							{
								continue;
							}
							$cor = $cor == 'ebebeb' ? 'f5f5f5' : 'ebebeb';
							?>
							<tr bgcolor="#<?= $cor ?>" onmouseover="this.bgColor='#ffffcc';" onmouseout="this.bgColor='#<?= $cor ?>';">
								<td align="left" class="SemBordaCimaBaixoDireita" style="padding: 0 0 0 60px;">
									<img src="/imagens/seta_filho.gif" align="top"/>
									<?=
										substr( $dados_4['codigo'], 0, 1 ) . '.' .
										substr( $dados_4['codigo'], 1, 1 ) . '.' .
										substr( $dados_4['codigo'], 2, 2 ) . '.' .
										substr( $dados_4['codigo'], 4, 2 ) . '.' .
										substr( $dados_4['codigo'], 6, 2 )
									?>
									<font color="#777777"> - <?= $dados_4['descricao'] ?></font>
								</td>
								<td align="right" class="SemBordaCimaBaixo">
									<font color="#777777">
										<?= number_format( $dados_4['valor'], 0, ',', '.') ?>
									</font>
								</td>
								<td align="right" class="SemBordaCimaBaixo">&nbsp;</td>
							</tr>
							<?
						}
					}
				}
			}
		}
		$cor = $cor == 'ebebeb' ? 'f5f5f5' : 'ebebeb';
		?>
		<tr bgcolor="#<?= $cor ?>" onmouseover="this.bgColor='#ffffcc';" onmouseout="this.bgColor='#<?= $cor ?>';">
			<td align="right" class="SemBordaCimaBaixoDireita" style="padding: 0 0 0 60px;">
				<font color="#666666">Total Geral</font>
			</td>
			<td align="right" class="SemBordaCimaBaixo">
				<?= number_format( $valor_total_natureza, 0, ',', '.' ) ?>
			</td>
			<td align="right" class="SemBordaCimaBaixo">&nbsp;</td>
		</tr>
		<?
	}
}
	// FIM EXIBE DADOS DE DESPESA POR NATUREZA

?>




	</table>
<?
}
?>
<a href="#" onclick="document.location='?modulo=inicio&acao=C';return false;" >
    <img src="../imagens/larrow_all.gif" alt="Voltar" />
</a>

</form>

