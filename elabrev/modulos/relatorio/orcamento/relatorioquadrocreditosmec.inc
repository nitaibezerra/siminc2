<?php

	/*
		Sistema Simec
		Setor responsável: SPO-MEC
		Desenvolvedor: Equipe Consultores Simec
		Analista: Gilberto Arruda Cerqueira Xavier, Cristiano Cabral (cristiano.cabral@gmail.com)
		Programador: Henrique Xavier Couto (e-mail: henriquexcouto@gmail.com), Renan de Lima Barbosa (e-mail: renandelima@gmail.com), Fabrício Mendonça (e-mail: fabriciomendonca@gmail.com)
		Módulo: relatorioquadrocreditosmec.inc
		Finalidade: relatório dos quadros orçamentarios, detalha os gastos da unidade por ação
	*/
	
	set_time_limit( 0 );
	error_reporting( E_ALL );
	
	$db->executar( "set client_encoding to LATIN1" );
	
	$wid_1 = 80;
	$wid_2 = 340;
	$wid_3 = 50;
	$wid_4 = 15;
	$wid_5 = 45;
	$wid_6 = 15;
	$wid_7 = 20;
	$wid_8 = 70;

	// define as unidades
	$unidades = $_REQUEST['unidades'];
	if ( !count( $unidades ) || $unidades[0] == '' )
	{
		$unidades = array();
		$sql_unidade = "select unicod from unidade where unitpocod='U' and orgcod='26000' and unicod<>'26100' and unistatus = 'A' group by unicod, unidsc order by unicod DESC";
		$dados_unidade = $db->carregar( $sql_unidade );
		for ( $i = count( $dados_unidade ) - 1; $i >= 0; $i--  )
		{
			$unidades[] = $dados_unidade[$i]['unicod'];
		}
	}
	// FIM define as unidades

	// define os programas
	$programa_restricao = '';
	if ( count( $_REQUEST['prgcod'] ) && $_REQUEST['prgcod'][0] != '' )
	{
		for ( $i = count( $_REQUEST['prgcod'] ) - 1; $i >= 0; $i-- )
		{
			$programa_restricao .= ",'" . $_REQUEST['prgcod'][$i] . "'";
		}
		$programa_restricao = ' and ppa.prgcod in ( ' . substr( $programa_restricao, 1 ) . ' ) ' ;
	}
	// FIM define os programas
	
	// define as ações
	$acao_restricao = '';
	if ( count( $_REQUEST['acacod'] ) && $_REQUEST['acacod'][0] != '' )
	{
		for ( $i = count( $_REQUEST['acacod'] ) - 1; $i >= 0; $i-- )
		{
			$acao_restricao .= ",'" . $_REQUEST['acacod'][$i] . "'";
		}
		$acao_restricao = ' and a.acacod in (' . substr( $acao_restricao, 1 ) . ') ' ;
	}
	// FIM define as ações
	
	// define os idocs
	$idoc_restricao = '';
	$idoc_join = '';
	if ( count( $_REQUEST['idocod'] ) && $_REQUEST['idocod'][0] != '' )
	{
		for ( $i = count( $_REQUEST['idocod'] ) - 1; $i >= 0; $i-- )
		{
			$idoc_restricao .= ",'" . $_REQUEST['idocod'][$i] . "'";
		}
		$idoc_restricao = ' and i.idocod in (' . substr( $idoc_restricao, 1 ) . ') ' ;
		$idoc_join = ' inner join idoc i using ( idoid ) ';
	}
	// FIM define os idocs
	
	// define as natureza
	$natureza_restricao = '';
	$natureza_join = '';
	if ( count( $_REQUEST['ndpcod'] ) && $_REQUEST['ndpcod'][0] != '' )
	{
		for ( $i = count( $_REQUEST['ndpcod'] ) - 1; $i >= 0; $i-- )
		{
			$natureza_restricao .= ",'" . $_REQUEST['ndpcod'][$i] . "'";
		}
		$natureza_restricao = ' and n.ndpcod in (' . substr( $natureza_restricao, 1 ) . ') ' ;
		$natureza_join = ' inner join naturezadespesa n using ( ndpid ) ';
	}
	// FIM define as natureza
	
	// define as fontes de recurso
	$fonte_restricao = '';
	if ( count( $_REQUEST['fonte'] ) && $_REQUEST['fonte'][0] != '' )
	{
		for ( $i = count( $_REQUEST['fonte'] ) - 1; $i >= 0; $i-- )
		{
			$fonte_restricao .= ",'" . $_REQUEST['fonte'][$i] . "'";
		}
		$fonte_restricao = ' and ac.foncod in (' . substr( $fonte_restricao, 1 ) . ') ' ;
	}
	// FIM define as fontes de recurso
	
	// define os gnds
	$gnd_restricao = '';
	if ( count( $_REQUEST['gnd'] ) && $_REQUEST['gnd'][0] != '' )
	{
		for ( $i = count( $_REQUEST['gnd'] ) - 1; $i >= 0; $i-- )
		{
			$gnd_restricao .= "," . $_REQUEST['gnd'][$i];
		}
		$gnd_restricao = ' and n.gndcod in (' . substr( $gnd_restricao, 1 ) . ') ' ;
		// obriga join com natureza devido ao campo gndcod
		$natureza_join = ' inner join naturezadespesa n using ( ndpid ) ';
	}
	// FIM define os gnds
	// define os tipos de detalhamento
	$detalhamento_restricao = '';
	$detalhamento_join = '';
	if ( count( $_REQUEST['detalhamento'] ) && $_REQUEST['detalhamento'][0] != '' )
	{
		for ( $i = count( $_REQUEST['detalhamento'] ) - 1; $i >= 0; $i-- )
		{
			$detalhamento_restricao .= "," . $_REQUEST['detalhamento'][$i];
		}
		// captura as ações que possuem alguma despesa que por sua vez possui o tipo de detalhamento filtrado
		$detalhamento_restricao = ' and tda.tpdid in (' . substr( $detalhamento_restricao, 1 ) . ') ' ;
		// esse join abaixo é utilizado em mais de um lugar
		// por isso deve ser usado sptrintf pra especificar qual coluna faz join com a tabela
		$detalhamento_join = ' inner join elabrev.tipodetalhamentoacao tda on tda.acaid = %s ';
	}
	// FIM define os gnds
	
?>
<html>
	<head>
		<style type="text/css">
			html { margin: 10px 2px 10px 2px; }
			body { text-align: center; }
			table { border 0; }
			td{ font-size: 6pt; font-family: verdana; }
			td.italico { font-style: italic; }
			td.negrito { font-weight: bold; }
			td.sublinhado { text-decoration: underline; }
			td.bordaDireita { border-right: 1px solid #808080; }
			td.bordaCima{ border-top: 1px solid #000000; }
			td.bordaPontilhadaCima{ border-top: 2px dotted #909090; }
			td.bordaBaixo{ border-bottom: 1px solid #0000; }
			td.fundo{ background-color: #f3f3f3; }
			td.fundoEscuro{ background-color: #dadada; }
			@media print {.imprimir { display: none }}
			span.imprimir { position: absolute; top: 3px; margin: 0; padding: 5px; position: fixed; background-color: #f0f0f0; border: 1px solid #909090; cursor:pointer; }
			span.imprimir:hover { background-color: #d0d0d0; }
			div { margin:0 auto; padding: 0; text-align:left; page-break-after: always; width: <?= $wid_1 + $wid_2 + $wid_3 + $wid_4 + $wid_5 + $wid_6 + $wid_7 + $wid_8 ?>; }
		</style>
		<title>Ministério da Educação - QDD</title>
		<script type="text/javascript">
			self.focus();
		</script>
	</head>
	<body>
		<div>
			<span class="imprimir" onclick="window.print();"><img src="/imagens/print.gif"/></span>
			<p align="center" style="font-size: 18px; font-family: Arial; margin: 0;">Ministério da Educação</p>
			<p align="center" style="font-size: 14px; font-family: Arial; margin: 0;">Quadro de Detalhamento de Despesa</p>
			<p align="center" style="font-size: 12px; font-family: Arial; margin: 10px;">
				<br/><br/>
				<b>Unidades filtradas</b><br/>
				<? if ( $_REQUEST['unidades'][0] != '' ) : ?>
					<? $sql_mostra_filtro = "select unicod || ' - ' || unidsc as descricao from unidade where unicod in ( '" . implode( "','", $_REQUEST['unidades'] ) . "' ) order by unicod || ' - ' || unidsc "; ?>
					<table border="0" cellpadding="0" cellspacing="0" align="center" width="1" >
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
					<? $sql_mostra_filtro = "select acacod || ' - ' || acadsc as descricao from elabrev.ppaacao_orcamento a where 1 = 1 " . $acao_restricao . " group by acacod || ' - ' || acadsc order by acacod || ' - ' || acadsc "; ?>
					<table border="0" cellpadding="0" cellspacing="0" align="center" width="1" >
						<tr><td><div style="text-align:left;font-size:8pt;">
						<? foreach ( $db->carregar( $sql_mostra_filtro ) as $acao ) : ?>
							<?= $acao['descricao'] ?><br/>
						<? endforeach; ?>
						</div></td></tr>
					</table>
				<? else : ?>Todas<? endif; ?>
				<br/><br/>
				<b>IDOC</b><br/>
				<? if ( $idoc_restricao ) : ?>
					<? $sql_mostra_filtro = "select idocod || ' - ' || idodsc as descricao from idoc i where 1 = 1 " . $idoc_restricao . " group by idocod || ' - ' || idodsc order by idocod || ' - ' || idodsc "; ?>
					<table border="0" cellpadding="0" cellspacing="0" align="center" width="1" >
						<tr><td><div style="text-align:left;font-size:8pt;">
						<? foreach ( $db->carregar( $sql_mostra_filtro ) as $idoc ) : ?>
							<?= $idoc['descricao'] ?><br/>
						<? endforeach; ?>
						</div></td></tr>
					</table>
				<? else : ?>Todos<? endif; ?>
				<br/><br/>
				<b>Natureza</b><br/>
				<? if ( $natureza_restricao ) : ?>
					<? $sql_mostra_filtro = "select ndpcod || ' - ' || ndpdsc as descricao from naturezadespesa n where 1 = 1 " . $natureza_restricao . " order by ndpcod || ' - ' || ndpdsc "; ?>
					<table border="0" cellpadding="0" cellspacing="0" align="center" width="1" >
						<tr><td><div style="text-align:left;font-size:8pt;">
						<? foreach ( $db->carregar( $sql_mostra_filtro ) as $natureza ) : ?>
							<?= $natureza['descricao'] ?><br/>
						<? endforeach; ?>
						</div></td></tr>
					</table>
				<? else : ?>Todas<? endif; ?>
				<br/><br/>
				<b>Fonte</b><br/>
				<? if ( $fonte_restricao ) : ?>
					<? $sql_mostra_filtro = "select foncod || ' - ' || fondsc as descricao from fonterecurso ac where 1 = 1 " . $fonte_restricao . " order by foncod || ' - ' || fondsc "; ?>
					<table border="0" cellpadding="0" cellspacing="0" align="center" width="1" >
						<tr><td><div style="text-align:left;font-size:8pt;">
						<? foreach ( $db->carregar( $sql_mostra_filtro ) as $fonte ) : ?>
							<?= $fonte['descricao'] ?><br/>
						<? endforeach; ?>
						</div></td></tr>
					</table>
				<? else : ?>Todas<? endif; ?>
				<br/><br/>
				<b>GND</b><br/>
				<? if ( $gnd_restricao ) : ?>
					<? $sql_mostra_filtro = "select gndcod || ' - ' || gnddsc as descricao from gnd n where 1 = 1 " . $gnd_restricao . " order by gndcod || ' - ' || gnddsc "; ?>
					<table border="0" cellpadding="0" cellspacing="0" align="center" width="1" >
						<tr><td><div style="text-align:left;font-size:8pt;">
						<? foreach ( $db->carregar( $sql_mostra_filtro ) as $gnd ) : ?>
							<?= $gnd['descricao'] ?><br/>
						<? endforeach; ?>
						</div></td></tr>
					</table>
				<? else : ?>Todos<? endif; ?>
				<br/><br/>
				<b>Tipo de Detalhamento</b><br/>
				<? if ( $detalhamento_restricao ) : ?>
					<? $sql_mostra_filtro = "select tpdcod || ' - ' || tpddsc as descricao from elabrev.tipodetalhamento tda where 1 = 1 " . $detalhamento_restricao . " order by tpdcod || ' - ' || tpddsc "; ?>
					<table border="0" cellpadding="0" cellspacing="0" align="center" width="1" >
						<tr><td><div style="text-align:left;font-size:8pt;">
						<? foreach ( $db->carregar( $sql_mostra_filtro ) as $tipodetalhamento ) : ?>
							<?= $tipodetalhamento['descricao'] ?><br/>
						<? endforeach; ?>
						</div></td></tr>
					</table>
				<? else : ?>Todos<? endif; ?>
			</p>
		</div>
<?

	$registros_encontrados = false;
	foreach ( $unidades as $unidade )
	{
		if ( !$unidade )
		{
			continue;
		}
		$ppoid = $db->pegaUm( "select ppoid as codigo from elabrev.propostaorcamento where ppostatus = 'A' and ppoanoexercicio = '" . $_SESSION['exercicio'] . "'" );
		if ( !$ppoid )
		{
			continue;
		}
		// define filtro fixo para todas as querys
		$filtro_fixo = " a.prgano = '" . $_SESSION['exercicio'] . "' and a.acastatus = 'A' and a.unicod = '" . $unidade . "' and a.acasnrap = false ";
		// FIM define filtro fixo para todas as querys
		// captura os programas
		$Query =
			"select " .
			 	" ppa.prgcod, ppa.prgdsc, " .
			 	" coalesce( SUM( ac.dpavalor ), 0 ) as valor, ppa.prgid " .
			 " from elabrev.despesaacao ac " .
			 	" inner join elabrev.ppaacao_orcamento a using ( acaid ) " .
			 	" inner join elabrev.ppaprograma_orcamento ppa using ( prgid ) " .
			 	$idoc_join .
			 	$natureza_join .
			 	sprintf( $detalhamento_join, ' a.acaid ' ) .
			 " where " .
			 	$filtro_fixo .
			 	$programa_restricao .
			 	$acao_restricao .
			 	$idoc_restricao .
			 	$natureza_restricao .
			 	$fonte_restricao .
			 	$gnd_restricao .
			 	$detalhamento_restricao .
			 " group by ppa.prgid, ppa.prgcod, ppa.prgdsc " .
			 " order by ppa.prgid, ppa.prgcod, ppa.prgdsc ";
			 
		$RS1 = $db->record_set($Query);
		$nlinhas1 = $db->conta_linhas($RS1);
		// FIM captura os programas
		if ( $nlinhas1 < 0 )
		{
			continue;
		}
		$registros_encontrados = true;
		$nomeUnidade = $db->pegaUm( "select unicod || ' - ' || unidsc from unidade where unicod = '" . $unidade . "'" );
		?>
		<div>
			<p align="center" style="font-size: 14px; margin: 50px 0 30px 0; font-weight: bold;"><?= $nomeUnidade ?></p>
			<table border="0" align="center" cellpadding="1" cellspacing="0">
			<tr>
				<td width="<?= $wid_1 ?>" align="center" class="negrito bordaCima bordaBaixo fundo">Programática</td>
				<td width="<?= $wid_2 ?>" align="center" class="negrito bordaCima bordaBaixo fundo">Programa Ação Produto Localização Natureza</td>
				<td width="<?= $wid_3 ?>" align="center" class="negrito bordaCima bordaBaixo fundo">Funcional</td>
				<td width="<?= $wid_4 ?>" align="center" class="negrito bordaCima bordaBaixo fundo">Esf</td>
				<td width="<?= $wid_5 ?>" align="center" class="negrito bordaCima bordaBaixo fundo">Nat</td>
				<td width="<?= $wid_6 ?>" align="center" class="negrito bordaCima bordaBaixo fundo">IU</td>
				<td width="<?= $wid_7 ?>" align="center" class="negrito bordaCima bordaBaixo fundo">Fte</td>
				<td width="<?= $wid_8 ?>" align="right" class="negrito bordaCima bordaBaixo fundo">Valor</td>
			</tr>
			<?
				// percorre os programas
				$vlTotalPrograma = 0;
				for ( $i = 0 ; $i <= $nlinhas1; $i++ )
				{
					/*
						$resPrograma[0] => prgcod
						$resPrograma[1] => prgdsc
						$resPrograma[2] => valor
						$resPrograma[3] => prgid
						$resPrograma[4] => prgid
					*/
				  	$resPrograma = $db->carrega_registro( $RS1, $i );
				  	$vlTotalPrograma += $resPrograma[2];
					?>
					<tr>
						<td align="left" class="negrito fundoEscuro"><?= $resPrograma[0] ?></td>
						<td align="left" class="negrito fundoEscuro"><?= $resPrograma[1] ?></td>
						<td align="left" class="fundoEscuro">&nbsp;</td>
						<td align="left" class="fundoEscuro">&nbsp;</td>
						<td align="left" class="fundoEscuro">&nbsp;</td>
						<td align="left" class="fundoEscuro">&nbsp;</td>
						<td align="left" class="fundoEscuro">&nbsp;</td>
						<td align="right" class="negrito fundoEscuro"><?= number_format( $resPrograma[2], 0, ',', '.' ) ?></td>
					</tr>
					<?
					// captura as ações
					$Query =
						" select " .
							" a.acacod, a.acadsc, a.funcod, a.sfucod, " .
							" coalesce( SUM(ac.dpavalor) , 0) as valor, t.taccod, t.tacdsc  " .
						" from elabrev.despesaacao ac " .
							" inner join elabrev.ppaacao_orcamento a on a.acaid = ac.acaid " .
							" inner join elabrev.ppafuncao f on f.funcod = a.funcod " .
							" inner join elabrev.ppasubfuncao s on s.sfucod = a.sfucod " .
							" left join tipoacao t on t.taccod = a.taccod 	".
							sprintf( $detalhamento_join, ' a.acaid ' ) .
						$natureza_join .						
						" where " .	$filtro_fixo . " and " .
							" a.prgid = " . $resPrograma[3] .
							sprintf( $detalhamento_restricao, ' a.acaid ' ) .
					 	$natureza_restricao .
					 	$gnd_restricao .
						" group by a.acacod, a.acadsc, a.funcod, a.sfucod, t.taccod, t.tacdsc " .
						" order by t.tacdsc, a.acacod, a.acadsc, a.funcod, a.sfucod";
				    $RS2 = $db->record_set( $Query );
				    $nlinhas2 = (integer) $db->conta_linhas( $RS2 );
				    // FIM captura as ações
					// percorre as ações
					$taccod = null;
				  	for ( $j = 0; $j <= $nlinhas2; $j++ )
					{
						/*
							$resAcao[0] => acacod
							$resAcao[1] => acadsc
							$resAcao[2] => funcod
							$resAcao[3] => sfucod
							$resAcao[4] => valor
						*/
					  	$resAcao = $db->carrega_registro( $RS2, $j );
					  	// imprime título do tipo de ação
					  	if ( $taccod != $resAcao[5] || $j == 0 )
					  	{
					  		$taccod = $resAcao[5];
					  		$Query = " select coalesce( SUM( ac.dpavalor ) , 0) as valor " .
								" from elabrev.despesaacao ac " .
									" inner join elabrev.ppaacao_orcamento a on a.acaid = ac.acaid " .
									sprintf( $detalhamento_join, ' a.acaid ' ) .
								" where " .	$filtro_fixo . " and " .
									" a.prgid = " . $resPrograma[3] . " and " .
									" a.taccod = '" . $taccod . "' " .
									sprintf( $detalhamento_restricao, ' a.acaid ' );
							$resTac = $db->recuperar( $Query );
						  	?>
						  		<tr>
						  			<td colspan="7" align="center" class="negrito fundo">
						  				<?= $resAcao['tacdsc'] ?>
						  			</td>
						  			<td align="right" class="negrito fundo">
						  				<?= number_format( $resTac['valor'], 0, ',', '.' ); ?>
						  			</td>
						  		</tr>
						  	<?
				    		
					  	}
					  	// FIM imprime título do tipo de ação
					  	$borda = $j == 0 || $_REQUEST['tipo_relatorio_qdd'] == 'simplificado' ? '' : 'bordaPontilhadaCima ' ;
					  	?>
						<tr>
							<td class="<?= $borda ?>bordaDireita" align="left" class="bordaPontilhadaCima negrito"><?= $resPrograma[0] ?>.<?= $resAcao[0] ?></td>
							<td align="left" class="<?= $borda ?>negrito bordaDireita"><?= $resAcao[1] ?></td>
							<td align="center" class="<?= $borda ?>negrito italico bordaDireita"><?= $resAcao[2] ?>&nbsp;&nbsp;<?= $resAcao[3] ?></td>
							<td class="<?= $borda ?>bordaDireita">&nbsp;</td>
							<td class="<?= $borda ?>bordaDireita">&nbsp;</td>
							<td class="<?= $borda ?>bordaDireita">&nbsp;</td>
							<td class="<?= $borda ?>bordaDireita">&nbsp;</td>
							<td align="right" class="<?= $borda ?>negrito italico"><?= number_format( $resAcao[4], 0, ',', '.' ) ?></td>
						</tr>
						<?
						// captura os localizadores
						$Query =
							"select ".
								" a.loccod, a.sacdsc, " .
								" coalesce( SUM( ac.dpavalor ) , 0 ) as valor " .
							" from elabrev.despesaacao ac " .
								" inner join elabrev.ppaacao_orcamento a using ( acaid ) " .
							" where " . $filtro_fixo . " and " .
								" a.prgid = " . $resPrograma[3] . " and " .
								" a.acacod = '" . $resAcao[0] . "' " .
							" group by a.loccod, a.sacdsc ".
							" order by a.loccod, a.sacdsc ";
						$RS3 = $db->record_set( $Query );
						$nlinhas3 = (integer) $db->conta_linhas( $RS3 );
						// FIM captura os localizadores
						// percorre os localizadores
					  	for ( $k = 0; $k <= $nlinhas3; $k++ )
						{
							/*
								$resLocalizador[0] => loccod
								$resLocalizador[1] => sacdsc
								$resLocalizador[2] => valor
							*/
						  	$resLocalizador = $db->carrega_registro( $RS3, $k );
						  	?>
							<tr>
								<td class="bordaDireita" align="left"><?= $resPrograma[0] ?>.<?= $resAcao[0] ?>.<?= $resLocalizador[0] ?></td>
								<td class="bordaDireita" align="left" style="padding: 0 0 0 10px;"><img src="/imagens/seta_filho.gif" align="absmiddle"/><?= $resLocalizador[1] ?>&nbsp;</td>
								<td class="bordaDireita">&nbsp;</td>
								<td class="bordaDireita">&nbsp;</td>
								<td class="bordaDireita">&nbsp;</td>
								<td class="bordaDireita">&nbsp;</td>
								<td class="bordaDireita">&nbsp;</td>
								<td align="right" class="negrito"><?= number_format( $resLocalizador[2], 0, ',', '.' ) ?></td>
							</tr>
							<?
							if ( $_REQUEST['tipo_relatorio_qdd'] == 'simplificado' )
							{
								continue;
							}
							// captura produtos e unidades de medida
							$Query = "
										select 
									  		a.acadscunmsof as unmdsc, a.acadscprosof as prodsc, 
									 		e.esfdsc, n.ndpcod, n.ndpdsc, a.acaqtdefisico, ac.iducod, ac.foncod, 
									 		coalesce( SUM( ac.dpavalor ) , 0 ) as valor, a.acaid 
									 	from elabrev.despesaacao ac 
									 		inner join elabrev.ppaacao_orcamento a using ( acaid )
									 		inner join esfera e using ( esfcod ) 
									 		inner join naturezadespesa n using ( ndpid ) 
									 	where " . $filtro_fixo . " and 
									 		a.prgid = " . $resPrograma[3] . " and 
									 		a.acacod = '" . $resAcao[0] . "' and 
									 		a.loccod = '" . $resLocalizador[0] . "' 
								 	" . $natureza_restricao . "
								 	" . $gnd_restricao . "
									 	group by 
									 		a.acadscunmsof, a.acadscprosof, e.esfdsc, n.ndpcod, n.ndpdsc, a.acaqtdefisico, ac.iducod, ac.foncod, a.acaid 
									 	order by 
									 		a.acadscunmsof, a.acadscprosof, e.esfdsc, n.ndpcod, n.ndpdsc, a.acaqtdefisico, ac.iducod, ac.foncod ";
							$RS4 = $db->record_set( $Query );
					    	$nlinhas4 = (integer) $db->conta_linhas( $RS4 );
							//Alguns produtos não aparecerão de acordo com os filtros de natureza e GND
					    	if ( $nlinhas4 < 1 ) continue;
					    	/*
								$resProdutoUnidade[0] => unmdsc
								$resProdutoUnidade[1] => prodsc
								$resProdutoUnidade[2] => esfdsc
								$resProdutoUnidade[3] => ndpcod
								$resProdutoUnidade[4] => ndpdsc
								$resProdutoUnidade[5] => acaqtdefisico
								$resProdutoUnidade[6] => iducod
								$resProdutoUnidade[7] => foncod
								$resProdutoUnidade[8] => valor
								$resProdutoUnidade[9] => acaid
							*/
					    	$resProdutoUnidade = $db->carrega_registro( $RS4, 0 );
							// FIM captura produtos e unidades de medida
							//Mostra o produto
							?>
							<tr>
								<td class="bordaDireita" align="left">&nbsp;</td>
								<td class="bordaDireita" align="left" style="padding: 0 0 0 20px;"><img src="/imagens/seta_filho.gif" align="absmiddle"/><?= $resProdutoUnidade[1] ?> (<?= $resProdutoUnidade[0] ?>)&nbsp;&nbsp;Qtd. Físico: <?= $resProdutoUnidade[5] ?></td>
								<td class="bordaDireita" align="left">&nbsp;</td>
								<td class="bordaDireita">&nbsp;</td>
								<td class="bordaDireita">&nbsp;</td>
								<td class="bordaDireita">&nbsp;</td>
								<td class="bordaDireita">&nbsp;</td>
								<td align="right"><?= number_format( $resLocalizador[2], 0, ',', '.' ) ?></td>
							</tr>
							<?
							// percorre os produtos/unidadesmedida
							for ( $l = 0; $l <= $nlinhas4; $l++ )
							{
								/*
									$resProdutoUnidade[0] => unmdsc
									$resProdutoUnidade[1] => prodsc
									$resProdutoUnidade[2] => esfdsc
									$resProdutoUnidade[3] => ndpcod
									$resProdutoUnidade[4] => ndpdsc
									$resProdutoUnidade[5] => acaqtdefisico
									$resProdutoUnidade[6] => iducod
									$resProdutoUnidade[7] => foncod
									$resProdutoUnidade[8] => valor
									$resProdutoUnidade[9] => acaid
								*/
							  	$resProdutoUnidade = $db->carrega_registro( $RS4, $l );
								?>
								<tr>
									<td align="left" class="bordaDireita">&nbsp;</td>
									<td align="left" class="bordaDireita" style="padding: 0 0 0 30px;">
										<img src="/imagens/seta_filho.gif" align="absmiddle"/>
										<?=
											substr( $resProdutoUnidade[3], 0, 1 ) . '.' .
											substr( $resProdutoUnidade[3], 1, 1 ) . '.' .
											substr( $resProdutoUnidade[3], 2, 2 ) . '.' .
											substr( $resProdutoUnidade[3], 4, 2 ) . '.' .
											substr( $resProdutoUnidade[3], 6, 2 )
										?> - <?= $resProdutoUnidade[4] ?>
									</td>
									<td class="bordaDireita">&nbsp;</td>
									<td align="center" class="bordaDireita"><?= $resProdutoUnidade[2]{0} ?></td>
									<td align="center" class="bordaDireita"><?= $resProdutoUnidade[3] ?></td>
									<td align="center" class="bordaDireita"><?= $resProdutoUnidade[6] ?></td>
									<td align="center" class="bordaDireita"><?= $resProdutoUnidade[7] ?></td>
									<td align="right"><?= number_format( $resProdutoUnidade[8], 0, ',', '.' ) ?></td>
								</tr>
								<? 
							}
							// FIM percorre os produtos/unidadesmedida
						}
						// FIM percorre os localizadores
					}	    
					// FIM percorre as ações
				}
				// FIM percorre os programas
				?>
				<tr>
					<td colspan="7" style=" font-weight: bold; text-align: right;">Total Geral (Despesas Programadas):</td>
					<td style=" font-weight: bold; text-align: right;">&nbsp;<?= formata_valor( $vlTotalPrograma, 0 ) ?></td>
				</tr>
			</table>
		</div>
<?
	}
	if ( $registros_encontrados === false )
	{
		?>
		<div>
			<p align="center" style="color:#ff2020;">
				Não existe nenhuma despesa de detalhamento com o(s) filtro(s) realizado(s).
			</p>
		</div>
		<?
	}
	ob_flush();
	exit();
?>
	</body>
</html>