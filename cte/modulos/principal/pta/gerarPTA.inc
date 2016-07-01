<?php 

ini_set('memory_limit','16M');

$width = 'width="100%"';
$bgColor = 'bgcolor="silver"';

require_once( APPRAIZ . 'includes/classes/dateTime.inc');
include_once APPRAIZ . "includes/classes/fileSimec.class.inc";
include_once APPRAIZ . "includes/classes/RequestHttp.class.inc";

$dados = '
<html>
<head>
<style type="">
	.fot{
		text-transform: uppercase; 
		font-family: arial black; 
		font-size: 20px;
		text-align: center;
		}
	.lista{
		font-size: 11px;
		padding: 3px;
		border-top: 2px solid #000;
		border-collapse: collapse;
	}
	.lista1{
		font-size: 11px;
		padding: 3px;
		border-top: 2px solid #000;
		border-collapse: collapse;
	}
	table.lista td{
		border-style: solid;
		border-width: 1px;
		border-color: #000;
		border-collapse: collapse;
	}
	.folha {
    	page-break-after: always;
	}
	@media print {.notprint { display: none } .div_rolagem{display: none} }	
	@media screen {.notscreen { display: none; }
	
	.div_rolagem{ overflow-x: auto; overflow-y: auto; height: 50px;}
	
</style>
</head>
<body>';


	$prsid = $_REQUEST['prsid'];
	
	$sql = "select distinct sbaid, prsjustificativapta, prsano
			from  cte.projetosapesubacao p
				inner join cte.projetosape ps on ps.prsid = p.prsid 
			where p.prsid = ".$prsid;

	$arDados = $db->carregar( $sql );
	$arDados = $arDados ? $arDados : array();
	
	$ano = '0';
	$arSbaid = array();
	foreach( $arDados as $sbaid ){
		$ano = $sbaid['prsano'];
		$arSbaid[] = $sbaid['sbaid'];
		$stJustificativa = $sbaid['prsjustificativapta'];
	}

	if( count($arSbaid) == 0 ){
		echo '<script>alert("Faltam dados.");
				history.back(-1);</script>';
		die();
	}
	
	gerarPdfPta( $arSbaid, $stJustificativa, $ano, $width, $bgColor, $dados );
	
	function gerarPdfPta( $idsSubacao, $stJustificativa, $ano, $width, $bgColor, $dados ){
		
		global $db;
		$itrid = cte_pegarItrid( $_SESSION['inuid'] );
		
		$stSbaid = "'" . implode( "', '", array_unique( $idsSubacao ) ). "'";
		
		if( $itrid == INSTRUMENTO_DIAGNOSTICO_ESTADUAL ){
			
			$sql = "select distinct 
						e.estdescricao as mundescricao, e.estuf,  
						s.sbaid, d.dimid, d.dimcod, ad.ardcod, i.indcod, d.dimdsc, ad.arddsc, 
					s.sbaordem, s.sbadsc, u.unddsc, i.inddsc, spt.*, s.sbaporescola
					from cte.subacaoindicador s
						inner join cte.subacaoparecertecnico spt on spt.sbaid = s.sbaid
						inner join cte.unidademedida u on u.undid = s.undid
						inner join cte.acaoindicador a on a.aciid = s.aciid
						inner join cte.pontuacao p on p.ptoid = a.ptoid
						inner join cte.instrumentounidade iu on iu.inuid = p.inuid	
						inner join territorios.estado e on e.estuf = iu.estuf
						inner join cte.indicador i on p.indid = p.indid
						inner join cte.criterio c on c.crtid = p.crtid and c.indid = i.indid
						inner join cte.areadimensao ad on ad.ardid = i.ardid
						inner join cte.dimensao d on d.dimid = ad.dimid	
					where s.sbaid in ( $stSbaid )
					and p.ptostatus = 'A'
					and sptano = '$ano'
					order by d.dimcod, ad.ardcod, i.indcod, s.sbaordem";
		}
		elseif( $itrid == INSTRUMENTO_DIAGNOSTICO_MUNICIPAL ){
			
			$sql = "select distinct m.mundescricao, m.estuf, s.sbaid, d.dimid, d.dimcod, ad.ardcod, i.indcod, d.dimdsc, ad.arddsc, 
					s.sbaordem, s.sbadsc, u.unddsc, i.inddsc, spt.*, s.sbaporescola
					from cte.subacaoindicador s
						inner join cte.subacaoparecertecnico spt on spt.sbaid = s.sbaid
						inner join cte.unidademedida u on u.undid = s.undid
						inner join cte.acaoindicador a on a.aciid = s.aciid
						inner join cte.pontuacao p on p.ptoid = a.ptoid
						inner join cte.instrumentounidade iu on iu.inuid = p.inuid	
						inner join territorios.municipio m on m.muncod = iu.muncod
						inner join cte.indicador i on p.indid = p.indid
						inner join cte.criterio c on c.crtid = p.crtid and c.indid = i.indid
						inner join cte.areadimensao ad on ad.ardid = i.ardid
						inner join cte.dimensao d on d.dimid = ad.dimid	
					where s.sbaid in ( $stSbaid )
					and p.ptostatus = 'A'
					and sptano = '$ano'
					order by d.dimcod, ad.ardcod, i.indcod, s.sbaordem";
		}

		$arDados = $db->carregar( $sql );
//ver($sql, d);
		if( is_array( $arDados ) ){

			if( $itrid == INSTRUMENTO_DIAGNOSTICO_ESTADUAL ){
				$sql = "SELECT
							to_char(to_number(pre.entnumcpfcnpj,'000'), '00\".\"000\".\"000\"/\"0000\"-\"00') as cnpjorgao,
							est.estdescricao as mundescricao,
							e.entnome as dirigente,
							pre.entnome as nomeorgao
						FROM entidade.entidade e
							inner join entidade.funcaoentidade 		fe on fe.entid = e.entid
							inner join entidade.funentassoc 		fea on fe.fueid = fea.fueid
							inner join entidade.entidade 			pre on pre.entid = fea.entid
							inner join entidade.endereco en on pre.entid = en.entid
							inner join territorios.estado est on est.estuf = en.estuf
							inner join cte.instrumentounidade iu on (iu.estuf = est.estuf)
						WHERE fe.funid = 25 
						AND iu.inuid = {$_SESSION['inuid']}";				
			}
			elseif( $itrid == INSTRUMENTO_DIAGNOSTICO_MUNICIPAL ){
				
				$sql = "SELECT
							to_char(to_number(pre.entnumcpfcnpj,'000'), '00\".\"000\".\"000\"/\"0000\"-\"00') as cnpjorgao,
							m.mundescricao,
							e.entnome as dirigente,
							pre.entnome as nomeorgao
						FROM entidade.entidade e
							inner join entidade.funcaoentidade 		fe on fe.entid = e.entid
							inner join entidade.funentassoc 		fea on fe.fueid = fea.fueid
							inner join entidade.entidade 			pre on pre.entid = fea.entid
							inner join entidade.endereco en on pre.entid = en.entid
							INNER JOIN territorios.municipio m ON m.muncod = en.muncod
							inner join cte.instrumentounidade iu on (iu.muncod = m.muncod)
						WHERE  fe.funid = 2 
						AND iu.inuid = {$_SESSION['inuid']}";			
			}			
			
			$arOrgao = $db->carregar( $sql );
			$arOrgao = $arOrgao ? $arOrgao : array();
			
			$arAgrupado = array();
			foreach( $arDados as $arSubacao ){
				$arAgrupado[$arSubacao['dimdsc']][$arSubacao['sbaid']] = $arSubacao;
			}
			
			$arDadosCabecalho = array();
			foreach( $arAgrupado as $dimcod => $arDados ){
				foreach( $arDados as $arSubacao ){
					$arDadosCabecalho['nrExercicio'] = $arSubacao['sptano'];
					$arDadosCabecalho['estuf'] = $arSubacao['estuf'];
					$arDadosCabecalho['mundescricao'] = $arSubacao['mundescricao'];
					$arDadosCabecalho['dimcod'][$dimcod] = $arSubacao['dimdsc'];
				}
			}
						
			$arDadosCabecalho['cnpjOrgao'] = $arOrgao[0]['cnpjorgao'];
			$arDadosCabecalho['nomeOrgao'] = $arOrgao[0]['nomeorgao'];
			$arDadosCabecalho['dirigente'] = $arOrgao[0]['dirigente'];
			
			$_SESSION['arDadosCabecalho'] = $arDadosCabecalho;
	
			$dados .= '<div class="folha">';
			$dados .= gerarAnexo1( $width, $bgColor, $arAgrupado, $stJustificativa, $itrid ); 
			$dados .= '</div>';
			$dados .= '<div class="folha">'; 
			$dados .= gerarAnexo2( $width, $bgColor, $arAgrupado, $itrid ); 
			$dados .= '</div>';
			$dados .= gerarAnexo3( $width, $bgColor, $arAgrupado ); 
			$dados .= '<div class="folha">';
			$dados .= gerarAnexo4( $width, $bgColor, $arAgrupado ); 
			$dados .= '</div>';
			$dados .= gerarAnexo5( $width, $bgColor, $arAgrupado ); 
			$dados .= '<div class="folha">';
			$dados .= gerarAnexo6( $width, $bgColor, $arAgrupado ); 
			$dados .= '</div>';
			
			$dados .= '</body></html>';

			ob_clean();

			$http = new RequestHttp();
			$http->toPdfDownload( iconv('ISO-8859-1', 'UTF-8', $dados), 'PTA' );
			
		} else {
			echo '<script>alert("Nenhum dado para ser exibido.");
				history.back(-1);</script>';
			die();
		}
	}

	function footer($width, $bgColor){
		$html = '
		<table '.$width.' class="lista1" align="center" '.$bgColor.' cellspacing="1" cellpadding="4">
			</tr>
				<td colspan="2">&nbsp;</td>
			</tr>
			<tr>
				<td>&nbsp;</td>
				<td colspan="3" style="text-align: center; border-style: solid; border-width: 1px; border-color: #000; border-collapse: collapse;" align="center" bgcolor="white"></td>
				<td>&nbsp;</td>
			</tr>
			<tr>
				<td>&nbsp;</td>
				<td colspan="3" style="text-align: center;"><b>LOCALIDADE, UF E DATA</b></td>
				<td>&nbsp;</td>
			</tr>
			<tr>
				<td>&nbsp;</td>
				<td style="text-align: center; border-style: solid; border-width: 1px; border-color: #000; border-collapse: collapse;" align="center" bgcolor="white"><b>'.strtoupper($_SESSION['arDadosCabecalho']['dirigente']).'</b></td>
				<td>&nbsp;</td>
				<td style="text-align: center; border-style: solid; border-width: 1px; border-color: #000; border-collapse: collapse;" align="center" bgcolor="white"></td>
				<td>&nbsp;</td>
			</tr>
			<tr>
				<td>&nbsp;</td>
				<td style="text-align: center;"><b>NOME DO DIRIGENTE OU REPRESENTANTE LEGAL</b></td>
				<td>&nbsp;</td>
				<td style="text-align: center;"><b>ASSINATURA DO DIRIGENTE OU REPRESENTANTE LEGAL</b></td>
				<td>&nbsp;</td>
			</tr>
		</table>
		<table '.$width.' class="lista" align="center" cellspacing="1"
			cellpadding="4">
			<tr>
				<td colspan="2" align="center"><b>Formul�rio confeccionado obedecendo aos preceitos
				da IN/STN/MF n� 01, de 15.1.1997 e as suas altera��es.</b></td>
			</tr>
		</table>';
		return $html;
	}
	
	function gerarAnexo1( $width, $bgColor, $arAgrupado, $stJustificativa, $itrid ){
		$dadosCabecalho = $_SESSION['arDadosCabecalho'];

		$html = '<table '.$width.' align="center" cellspacing="0" cellpadding="0">
		<tr>
			<td>
			<table '.$width.' class="lista" align="center" '.$bgColor.' cellspacing="1" cellpadding="4">
				<tr>
					<td class="fot" valign="top">MEC / FNDE</td>
					<td valign="top" style="text-align: center;"><span class="fot">PLANO
					DE TRABALHO</span> <br>
					<b><span style="text-align: center; font-size: 9px">DESCRI��O DO PROJETO</span></b></td>
					<td class="fot">ANEXO<br>
					1</td>
				</tr>
			</table>
			<table '.$width.' class="lista" align="center" cellspacing="1"
				cellpadding="4">
				<tr>
					<td valign="top" style="text-align: center;"><b>PLANO DE A��ES ARTICULADAS - PAR</b></td>
				</tr>
			</table>
			<table '.$width.' class="lista" align="center" cellspacing="1"
				cellpadding="4">
				<tr>
					<td><b>1 - EXERC�CIO</b><BR>
						&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;'.$dadosCabecalho['nrExercicio'].'</td>
					<td><b>2 - N�VEL DE ENSINO</b><br>
					&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Educa��o B�sica</td>
					<td><b>3 - ABRANG�NCIA DO PROJETO</b><BR>
					&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;PAR</td>
				</tr>
				<tr>
					<td><b>4 - CNPJ</b><BR>
						&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;'.$dadosCabecalho['cnpjOrgao'].'</td>
					<td colspan="2"><b>5 - NOME DO �RG�O OU ENTIDADE</b><br>
					&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;'.$dadosCabecalho['nomeOrgao'].'</td>
				</tr>
				<tr>
					<td colspan="2"><b>6 - '.($itrid == 2 ? "MUNIC�PIO" : "ESTADO").'</b><BR>
						&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;'.$dadosCabecalho['mundescricao'].'</td>
					<td style="width: 25%;"><b>7 - UF</b><br>
					&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;'.$dadosCabecalho['estuf'].'</td>
				</tr>
				<tr>
					<td colspan="3"><b>8 - A��O A SER EXECUTADA</b><BR>
						&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;'.implode( "\n", $dadosCabecalho['dimcod'] ).'</td>
				</tr>
				<tr>
					<td colspan="3"><b>9 - JUSTIFICATIVA DO PROJETO</b>
						<BR>
						&nbsp;&nbsp;&nbsp;&nbsp; '.nl2br($stJustificativa).'
					</td>
				</tr>
			</table>
			'.footer($width, $bgColor).'				
			</td>
			</tr>
		</table>';
		return $html;		
	}
	
	function gerarAnexo2( $width, $bgColor, $arAgrupado, $itrid ){

		global $db;
		$_SESSION['nrAnexo'] = 2;
		$dadosCabecalho = $_SESSION['arDadosCabecalho'];

		$html .= '<table '.$width.' align="center" cellspacing="0" cellpadding="0">
			<tr>
				<td>
				<table '.$width.' class="lista" align="center" '.$bgColor.' cellspacing="1" cellpadding="4">
					<tr>
						<td class="fot" valign="top">MEC / FNDE</td>
						<td valign="top" style="text-align: center;"><span class="fot">PLANO
						DE TRABALHO</span> <br>
						<b><span style="text-align: center; font-size: 9px">DESCRI��O DO PROJETO</span></b></td>
						<td class="fot">ANEXO<br>
						2</td>
					</tr>
				</table>
				<table '.$width.' class="lista" align="center" cellspacing="1"
					cellpadding="4">
					<tr>
						<td valign="top" style="text-align: center;"><b>PLANO DE A��ES ARTICULADAS - PAR</b></td>
					</tr>
				</table>
				<table '.$width.' class="lista" align="center" cellspacing="1"
					cellpadding="4">
					<tr>
						<td colspan="4"><b>1 - NOME DO �RG�O OU ENTIDADE</b><BR>
							&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;'.$dadosCabecalho['nomeOrgao'].'</td>
					</tr>
					<tr>
						<td colspan="3"><b>2 - '.($itrid == 2 ? "MUNIC�PIO" : "ESTADO").'</b><BR>
							&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;'.$dadosCabecalho['mundescricao'].'</td>
						<td style="width: 25%;"><b>3 - UF</b><br>
						&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;'.$dadosCabecalho['estuf'].'</td>
					</tr>
					<tr>
						<td colspan="4"><b>4 - A��O A SER EXECUTADA</b><BR>
							&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;'.implode( "\n", $dadosCabecalho['dimcod'] ).'</td>
					</tr>
					<tr>
						<td colspan="4"><b>5 - BENEFICI�RIOS DA A��O</b></td>
					</tr>';
						foreach( $arAgrupado as $stDimensao => $arDimensao ){
							foreach( $arDimensao as $count => $arSubacao ){
									
								$stDetalhamento .= "<p>Dimens�o: {$arSubacao['dimcod']} - {$arSubacao['dimdsc']}</p>";
								$stDetalhamento .= "<p>�rea: {$arSubacao['dimcod']}.{$arSubacao['ardcod']} - {$arSubacao['arddsc']}</p>";
								$stDetalhamento .= "<p>Indicador: {$arSubacao['dimcod']}.{$arSubacao['ardcod']}.{$arSubacao['indcod']} - {$arSubacao['inddsc']}</p>";
								$stDetalhamento .= "<p>Suba��o: {$arSubacao['dimcod']}.{$arSubacao['ardcod']}.{$arSubacao['indcod']} ({$arSubacao['sbaordem']}) - {$arSubacao['sbadsc']}</p>";
								$stDetalhamento .= "<p>Unidade de Medida: {$arSubacao['unddsc']}</p>";
								
								$stDetalhamento .= nl2br($arSubacao['sptuntdsc']);
									
							}
							$arSbaid = array_keys( $arDimensao );
							$stSbaid = "'" . implode( "', '", $arSbaid ). "'";
							
							// Quando � Onibus pequeno entao sao 23 alunos, medio 31 e grande 44. Ele soma todos e coloca como beneficiario o aluno.
							$sql = "select bendsc, urbano, rural, urbano + rural as total
									from (
										select b.benid, b.bendsc, sum( bs.vlrurbano ) as urbano, sum( bs.vlrrural ) as rural
										from cte.subacaoindicador s
											inner join cte.subacaobeneficiario bs on bs.sbaid = s.sbaid
											inner join cte.beneficiario b on b.benid = bs.benid
										where s.sbaid in ( $stSbaid )
										and sabano = {$dadosCabecalho['nrExercicio']}
										group by b.benid, b.bendsc
										order by b.bendsc
									) as foo";

							$arBeneficiarios = $db->carregar( $sql );
							$arBeneficiarios = $arBeneficiarios ? $arBeneficiarios : array();
						}
					$html .='<tr>
						<td>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<b>5.1 - BENEFICI�RIOS DA A��O</b></td>
						<td align="center"><b>5.2 - ZONA RURAL</b></td>
						<td align="center"><b>5.3 - ZONA URBANA</b></td>
						<td align="center"><b>5.4 - TOTAL</b></td>
					</tr>';
					foreach( $arBeneficiarios as $arBeneficiario ){
						$html .= '<tr>
							<td>Alunos</td>
							<td align="center">'.$arBeneficiario['rural'].'</td>
							<td align="center">'.$arBeneficiario['urbano'].'</td>
							<td align="center">'.$arBeneficiario['total'].'</td>
						</tr>';
					}
					$html .= '<tr>
						<td colspan="4"><b>6 - DETALHAMENTO DA A��O</b>
							'.$stDetalhamento.'
						</td>
					</tr>
				</table>
				'.footer($width, $bgColor).'				
				</td>
			</tr>
		</table>';
		return $html;	
	}

	/*
	function gerarAnexo2( $obPdf, $arAgrupado ){
//		
		global $db;
		$_SESSION['nrAnexo'] = 2;
		$arDadosCabecalho = $_SESSION['arDadosCabecalho'];
		
		foreach( $arAgrupado as $stDimensao => $arDimensao ){
			
			unset( $obPdf->aligns );
			$obPdf->AddPage(); //adicionando um nova p�gina
			$arFormatacao = array( 'Arial', '', 7, 'B' );
			
			// Terceira Linha
			$arWidth = array( 19 );
			$arTitulo[] = '4 - A��O A SER EXECUTADA';
			
			$arDados[] = $stDimensao;
			
			$obPdf->montarRowComTitulo( $arTitulo, $arDados, $arWidth, $arFormatacao );
			unset( $arDados, $arTitulo, $arWidth );

			// Detalhamento da Suba��o
				
			$stDetalhamento = '';
			foreach( $arDimensao as $count => $arSubacao ){
					
				$stDetalhamento .= "\n\nDimens�o: {$arSubacao['dimcod']} - {$arSubacao['dimdsc']}\n";
				$stDetalhamento .= "�rea: {$arSubacao['dimcod']}.{$arSubacao['ardcod']} - {$arSubacao['arddsc']}\n";
				$stDetalhamento .= "Indicador: {$arSubacao['dimcod']}.{$arSubacao['ardcod']}.{$arSubacao['indcod']} - {$arSubacao['inddsc']}\n";
				$stDetalhamento .= "Suba��o: {$arSubacao['dimcod']}.{$arSubacao['ardcod']}.{$arSubacao['indcod']} ({$arSubacao['sbaordem']}) - {$arSubacao['sbadsc']}\n";
				$stDetalhamento .= "Unidade de Medida: {$arSubacao['unddsc']}\n";
					
				$stDetalhamento .= "\n{$arSubacao['sptuntdsc']}\n";
					
			}
			
			$arSbaid = array_keys( $arDimensao );
			$stSbaid = "'" . implode( "', '", $arSbaid ). "'";
			
			$sql = "select bendsc, urbano, rural, urbano + rural as total
					from (
						select b.benid, b.bendsc, sum( bs.vlrurbano ) as urbano, sum( bs.vlrrural ) as rural
						from cte.subacaoindicador s
							inner join cte.subacaobeneficiario bs on bs.sbaid = s.sbaid
							inner join cte.beneficiario b on b.benid = bs.benid
						where s.sbaid in ( $stSbaid )
						and sabano = {$arDadosCabecalho['nrExercicio']}
						group by b.benid, b.bendsc
						order by b.bendsc
					) as foo";
			
			$arBeneficiarios = $db->carregar( $sql );
			$arBeneficiarios = $arBeneficiarios ? $arBeneficiarios : array();
			
			$obPdf->SetFont( 'Arial', 'B', 7 );
			$obPdf->SetWidths( array( 19 ) );
			$obPdf->Row( array( '5 - BENEFICI�RIOS DA A��O' ) );
						
			$obPdf->SetFont( 'Arial', 'B', 7 );
			$obPdf->SetWidths( array( 7, 4, 4, 4 ) );
			$obPdf->SetAligns( array( 'L', 'C', 'C', 'C' ) );
			$obPdf->Row( array( '     5.1 - BENEFICI�RIOS DA A��O', '5.2 - ZONA RURAL', '5.3 - ZONA URBANA', '5.4 - TOTAL' ) );			
			
			$obPdf->SetFont( 'Arial', '', 7 );
			foreach( $arBeneficiarios as $arBeneficiario ){
				$obPdf->SetWidths( array( 7, 4, 4, 4 ) );
				$obPdf->SetAligns( array( 'L', 'C', 'C', 'C' ) );
				$obPdf->Row( array( "     " . $arBeneficiario['bendsc'], $arBeneficiario['rural'], $arBeneficiario['urbano'], $arBeneficiario['total'] ) );			
			}
			
			$yAntes = $obPdf->getY();
			
			$obPdf->SetFont('Arial', 'B', 7);
			$obPdf->cell( 19, 0.4, '6 - Detalhamento da A��o', 0, 0 );
			
			$yDepois = $obPdf->getY();
			$obPdf->setY( $yAntes );	
				
			$obPdf->SetFont('Arial', '', 7);
			$obPdf->PageBreakTrigger = 25;
			$obPdf->multiCell( 19, 0.4, $stDetalhamento, 1 );
			
			$obPdf->setY( $yDepois );	
			
		}
		
	}
	*/
	
	function gerarAnexo3( $width, $bgColor, $arAgrupado ){
		
		global $db;
		$_SESSION['nrAnexo'] = 3;
		
		$dadosCabecalho = $_SESSION['arDadosCabecalho'];

		$valorTotalProjeto = 0;
		//ver($arAgrupado, d);
		foreach( $arAgrupado as $stDimensao => $arDimensao ){
			$valorTotal = 0;
			$html .= '<div class="folha">';
			$html .= '<table '.$width.' align="center" cellspacing="0" cellpadding="0">
				<tr>
					<td>
					<table '.$width.' class="lista" align="center" '.$bgColor.' cellspacing="1" cellpadding="4">
						<tr>
							<td class="fot" valign="top">MEC / FNDE</td>
							<td valign="top" style="text-align: center;"><span class="fot">PLANO
							DE TRABALHO</span> <br>
							<b><span style="text-align: center; font-size: 9px">DESCRI��O DO PROJETO</span></b></td>
							<td class="fot">ANEXO<br>
							3</td>
						</tr>
					</table>
					<table '.$width.' class="lista" align="center" cellspacing="1"
						cellpadding="4">
						<tr>
							<td valign="top" style="text-align: center;"><b>PLANO DE A��ES ARTICULADAS - PAR</b></td>
						</tr>
					</table>
					<table '.$width.' class="lista" align="center" cellspacing="1" cellpadding="4">
						<tr>
							<td><b>1 - EXERC�CIO</b><BR>
								&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;'.$dadosCabecalho['nrExercicio'].'</td>
							<td colspan="2"><b>2 - NOME DO �RG�O OU ENTIDADE</b><br>
							&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;'.$dadosCabecalho['nomeOrgao'].'</td>
							<td><b>3 - '.($itrid == 2 ? "MUNIC�PIO" : "ESTADO").'</b><BR>
							&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;'.$dadosCabecalho['mundescricao'].'</td>
							<td><b>4 - DF</b><BR>
							&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;'.$dadosCabecalho['estuf'].'</td>
						</tr>';
						$html .= '<tr>
							<td colspan="5"><b>5 - A��O A SER EXECUTADA</b><BR>
								&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;'.$stDimensao.'</td>
						</tr>';
						
						$arSbaid = array_keys( $arDimensao );
						$stSbaid = "'" . implode( "', '", $arSbaid ). "'";
						
						// Detalhamento da Suba��o
						$sql = "select sbaordem, sbadsc, unddsc, qtd, sum( total ) as valorTotal
								from (	
										select distinct cos.cosid, s.sbaordem, s.sbadsc, u.unddsc, 
											case 
												when s.sbaporescola = 'f' then sptunt
												else qfaqtd
											end as qtd,
											cosqtd * cosvlruni as total
										from cte.subacaoindicador s
											inner join cte.subacaoparecertecnico spt on spt.sbaid = s.sbaid
											left join cte.composicaosubacao cos on cos.sbaid = s.sbaid and cosano = {$dadosCabecalho['nrExercicio']}
											left join ( 
												select distinct sbaid, qfaano, sum(qfaqtd) as qfaqtd
												from cte.qtdfisicoano 
												group by sbaid, qfaano 
											) as q on q.sbaid = s.sbaid and qfaano = {$dadosCabecalho['nrExercicio']}
											inner join cte.unidademedida u on u.undid = s.undid
										where s.sbaid in ( $stSbaid )
										and sptano = {$dadosCabecalho['nrExercicio']}
										) as foo
								group by sbaordem, sbadsc, unddsc, qtd
								order by sbaordem";
						
						$arItens = $db->carregar( $sql );
						$arItens = $arItens ? $arItens : array();
							$html .= '<tr>
							<td rowspan="2"><b>6 - ORDEM</b></td>
							<td rowspan="2"><b>6.1 - ESPECIFICA��O DA A��O</b></td>
							<td align="center"><b>6.2 - INDICADOR F�SICO</b></td>
							<td align="center" colspan="2"><b>6.3 - CUSTO</b></td>
						</tr>
						<tr>
							<td align="center"><b>QUANTIDADE</b></td>
							<td align="center"><b>VALOR UNIT�RIO</b></td>
							<td align="center"><b>VALOR TOTAL</b></td>
						</tr>';
						foreach( $arItens as $arItem ){ 
							$valorTotal += $arItem['valortotal'];
							$valorItem = ( $arItem['valortotal'] / ( $arItem['qtd'] ? $arItem['qtd'] : 1 ) );
							$html .='<tr>
								<td align="right">'.$arItem['sbaordem'].'</td>
								<td>'.$arItem['sbadsc'].'</td>
								<td align="right">'.$arItem['qtd'].'</td>
								<td align="right">'.tratarValor( $valorItem ).'</td>
								<td align="right">'.tratarValor( $arItem['valortotal'] ).'</td>
							</tr>';
						} 
							$_SESSION['cte']['pta']['valortotal'] = $valorTotal;
							
						$nrValorProponente = tratarValor( $valorTotal * 0.01 );
						$nrValorConcedente = tratarValor( $valorTotal * 0.99 );
						
						$html.='<tr>
							<td colspan="5">&nbsp;</td>
						</tr>
						<tr>
							<td colspan="4" align="right"><b>7 - VALOR TOTAL DA A��O</b></td>
							<td align="right">'.tratarValor( $valorTotal ).'</td>
						</tr>
						<tr>
							<td colspan="4" align="right"><b>7 - VALOR TOTAL DO PROPONENTE</b></td>
							<td align="right">'.$nrValorProponente.'</td>
						</tr>
						<tr>
							<td colspan="4" align="right"><b>7 - VALOR TOTAL DO CONCEDENTE</b></td>
							<td align="right">'.$nrValorConcedente.'</td>
						</tr>';
					$html.='</table>
					'.footer($width, $bgColor).'				
					</td>
				</tr>
			</table>';
			$html .= '</div>';
		}
		return $html;
	}
	
	
	/*
	function gerarAnexo3( $obPdf, $arAgrupado ){
		
		global $db;
		$_SESSION['nrAnexo'] = 3;
		
		$arDadosCabecalho = $_SESSION['arDadosCabecalho'];
		
		$valorTotalProjeto = 0;
		
		foreach( $arAgrupado as $stDimensao => $arDimensao ){
			
			unset( $obPdf->aligns );
			$obPdf->AddPage(); //adicionando um nova p�gina
			$arFormatacao = array( 'Arial', '', 7, 'B' );
			
			// Terceira Linha
			$arWidth = array( 27.5 );
			$arTitulo[] = '5 - A��O A SER EXECUTADA';
			
			$arDados[] = $stDimensao;
			
			$obPdf->montarRowComTitulo( $arTitulo, $arDados, $arWidth, $arFormatacao );
			unset( $arDados, $arTitulo, $arWidth );

			$arSbaid = array_keys( $arDimensao );
			$stSbaid = "'" . implode( "', '", $arSbaid ). "'";

			// Detalhamento da Suba��o
			$sql = "select sbaordem, sbadsc, unddsc, qtd, sum( total ) as valorTotal
					from (	
							select distinct cos.cosid, s.sbaordem, s.sbadsc, u.unddsc, 
								case 
									when s.sbaporescola = 'f' then sptunt
									else qfaqtd
								end as qtd,
								cosqtd * cosvlruni as total
							from cte.subacaoindicador s
								inner join cte.subacaoparecertecnico spt on spt.sbaid = s.sbaid
								left join cte.composicaosubacao cos on cos.sbaid = s.sbaid and cosano = {$arDadosCabecalho['nrExercicio']}
								left join ( 
									select distinct sbaid, qfaano, sum(qfaqtd) as qfaqtd
									from cte.qtdfisicoano 
									group by sbaid, qfaano 
								) as q on q.sbaid = s.sbaid and qfaano = {$arDadosCabecalho['nrExercicio']}
								inner join cte.unidademedida u on u.undid = s.undid
							where s.sbaid in ( $stSbaid )
							and sptano = {$arDadosCabecalho['nrExercicio']}
							) as foo
					group by sbaordem, sbadsc, unddsc, qtd
					order by sbaordem";
			
			$arItens = $db->carregar( $sql );
			$arItens = $arItens ? $arItens : array();
			
			$obPdf->cell( 3, 0.8, '6 - ORDEM', 1, 0 );
			$obPdf->cell( 13.5, 0.8, '6.1 - ESPECIFICA��O DA A��O', 1, 0 );
			$obPdf->cell( 5, 0.4, '6.2 - INDICADOR F�SICO', 1, 0, 'C' );
			$obPdf->cell( 6, 0.4, '6.3 - CUSTO', 1, 1, 'C' );
			
			$obPdf->setX( 17.5 );
			$obPdf->cell( 3, 0.4, 'UNIDADE DE MEDIDA', 1, 0, 'C' );
			$obPdf->cell( 2, 0.4, 'QUANTIDADE', 1, 0, 'C' );
			$obPdf->cell( 3, 0.4, 'VALOR UNIT�RIO', 1, 0, 'C' );
			$obPdf->cell( 3, 0.4, 'VALOR TOTAL', 1, 1, 'C' );
			
			$obPdf->SetFont( 'Arial', '', 7 );
			$valorTotal = 0;
			
			if( $_REQUEST['victor'] ){
			
			foreach( $arItens as $arItem ){
				//ver($arItem,d);
				$valorTotal += $arItem['valortotal'];
				$valorItem = ( $arItem['valortotal'] / ( $arItem['qtd'] ? $arItem['qtd'] : 1 ) );
				
				$obPdf->SetWidths( array( 3, 13.5, 3, 2, 3, 3 ) );
				$obPdf->SetAligns( array( 'R', 'L', 'L', 'R', 'R', 'R' ) );
				//$obPdf->Row( array( $arItem['sbaordem'], $arItem['sbadsc'] , $arItem['unddsc'], $arItem['qtd'], tratarValor( 20 ), tratarValor( 30 ) ) );
				//$obPdf->Row( array( 5, 'Construir unidade de educa��o infantil - projeto do Proinf�ncia.', 'unidade(s) escolar(es)', 1, tratarValor(1229336.81), tratarValor(1229336.81) ) );
				$obPdf->Row( array( 5, "Construir unidade de educa��o infantil - projeto do Proinf�ncia.", "unidade(s) escolar(es)", 1, tratarValor(1229336.81), tratarValor(1229336.81) ) );
			}
			
			
/*			
			$obPdf->cell( 27.5, 0.4, '', 1, 1 );
			
			$valorTotalProjeto += $valorTotal;
			
			$nrValorProponente = tratarValor( $valorTotal * 0.01 );
			$nrValorConcedente = tratarValor( $valorTotal * 0.99 );
			
			$obPdf->SetFont('Arial', 'B', 7);
			$obPdf->cell( 24.5, 0.4, '7 - VALOR TOTAL DA A��O', 1, 0, 'R' );
			$obPdf->SetFont('Arial', '', 7);
			$obPdf->cell( 3, 0.4, tratarValor( $valorTotal ), 1, 1, 'R' );
			
			$obPdf->SetFont('Arial', 'B', 7);
			$obPdf->cell( 24.5, 0.4, '7 - VALOR TOTAL DO PROPONENTE', 1, 0, 'R' );
			$obPdf->SetFont('Arial', '', 7);
			$obPdf->cell( 3, 0.4, $nrValorProponente, 1, 1, 'R' );
			
			$obPdf->SetFont('Arial', 'B', 7);
			$obPdf->cell( 24.5, 0.4, '7 - VALOR TOTAL DO CONCEDENTE', 1, 0, 'R' );
			$obPdf->SetFont('Arial', '', 7);
			$obPdf->cell( 3, 0.4, $nrValorConcedente, 1, 1, 'R' );
*/			
/*		
			} elseif( $_REQUEST['teste'] ){
				foreach( $arItens as $arItem ){
					$valorTotal += $arItem['valortotal'];
					$valorItem = ( $arItem['valortotal'] / ( $arItem['qtd'] ? $arItem['qtd'] : 1 ) );
					$obPdf->SetWidths( array( 3, 13.5, 3, 2, 3, 3 ) );
					$obPdf->SetAligns( array( 'R', 'L', 'L', 'R', 'R', 'R' ) );
					$obPdf->Row( array( 5, "Testando PDF.", "unidade(s) escolar(es)", 1, tratarValor(1229336.81), tratarValor(1229336.81) ) );
				}
			}
			 elseif( $_REQUEST['teste2'] ){
				foreach( $arItens as $arItem ){
					$valorTotal += $arItem['valortotal'];
					$valorItem = ( $arItem['valortotal'] / ( $arItem['qtd'] ? $arItem['qtd'] : 1 ) );
					$obPdf->SetWidths( array( 3, 13.5, 3, 2, 3, 3 ) );
					$obPdf->SetAligns( array( 'R', 'L', 'L', 'R', 'R', 'R' ) );
					$obPdf->Row( array( 5, "Construir unidade de educa��o infantil - projeto do Proinf�ncia. Construir unidade de educa��o infantil - projeto do Proinf�ncia. Construir unidade de educa��o infantil - projeto do Proinf�ncia. Construir unidade de educa��o infantil - projeto do Proinf�ncia. Construir unidade de educa��o infantil - projeto do Proinf�ncia. Construir unidade de educa��o infantil - projeto do Proinf�ncia. Construir unidade de educa��o infantil - projeto do Proinf�ncia.", "unidade(s) escolar(es)", 1, tratarValor(1229336.81), tratarValor(1229336.81) ) );
				}
			}
		 	elseif( $_REQUEST['teste3'] ){
				foreach( $arItens as $arItem ){
					$valorTotal += $arItem['valortotal'];
					$valorItem = ( $arItem['valortotal'] / ( $arItem['qtd'] ? $arItem['qtd'] : 1 ) );
					$obPdf->SetWidths( array( 3, 13.5, 3, 2, 3, 3 ) );
					$obPdf->SetAligns( array( 'R', 'L', 'L', 'R', 'R', 'R' ) );
					var_dump($obPdf->Row( array( 5, "Construir unidade de educa��o infantil - projeto do Proinf�ncia. Construir unidade de educa��o infantil - projeto do Proinf�ncia. Construir unidade de educa��o infantil - projeto do Proinf�ncia.", "unidade(s) escolar(es)", 1, tratarValor(1229336.81), tratarValor(1229336.81))));exit;
				}
			}
		 	elseif( $_REQUEST['teste4'] ){
				try {
			 		foreach( $arItens as $arItem ){
						$valorTotal += $arItem['valortotal'];
						$valorItem = ( $arItem['valortotal'] / ( $arItem['qtd'] ? $arItem['qtd'] : 1 ) );
						$obPdf->SetWidths( array( 3, 13.5, 3, 2, 3, 3 ) );
						$obPdf->SetAligns( array( 'R', 'L', 'L', 'R', 'R', 'R' ) );
						$obPdf->Row( array( 5, "Construir unidade de educa��o infantil - projeto do Proinf�ncia. Construir unidade de educa��o infantil - projeto do Proinf�ncia. Construir unidade de educa��o infantil - projeto do Proinf�ncia.", "unidade(s) escolar(es)", 1, tratarValor(1229336.81), tratarValor(1229336.81)));
					}
					
				} catch (Exception $e) {
					echo $e->getMessage();
				}
			}
			
			else {
			
			foreach( $arItens as $arItem ){
				//ver($arItem,d);
				$valorTotal += $arItem['valortotal'];
				$valorItem = ( $arItem['valortotal'] / ( $arItem['qtd'] ? $arItem['qtd'] : 1 ) );
				
				$obPdf->SetWidths( array( 3, 13.5, 3, 2, 3, 3 ) );
				$obPdf->SetAligns( array( 'R', 'L', 'L', 'R', 'R', 'R' ) );
				$obPdf->Row( array( $arItem['sbaordem'], $arItem['sbadsc'] , $arItem['unddsc'], $arItem['qtd'], tratarValor( $valorItem ), tratarValor( $arItem['valortotal'] ) ) );
			}
			
			
				$obPdf->cell( 27.5, 0.4, '', 1, 1 );
				
				$valorTotalProjeto += $valorTotal;
				
				$nrValorProponente = tratarValor( $valorTotal * 0.01 );
				$nrValorConcedente = tratarValor( $valorTotal * 0.99 );
				
				$obPdf->SetFont('Arial', 'B', 7);
				$obPdf->cell( 24.5, 0.4, '7 - VALOR TOTAL DA A��O', 1, 0, 'R' );
				$obPdf->SetFont('Arial', '', 7);
				$obPdf->cell( 3, 0.4, tratarValor( $valorTotal ), 1, 1, 'R' );
				
				$obPdf->SetFont('Arial', 'B', 7);
				$obPdf->cell( 24.5, 0.4, '7 - VALOR TOTAL DO PROPONENTE', 1, 0, 'R' );
				$obPdf->SetFont('Arial', '', 7);
				$obPdf->cell( 3, 0.4, $nrValorProponente, 1, 1, 'R' );
				
				$obPdf->SetFont('Arial', 'B', 7);
				$obPdf->cell( 24.5, 0.4, '7 - VALOR TOTAL DO CONCEDENTE', 1, 0, 'R' );
				$obPdf->SetFont('Arial', '', 7);
				$obPdf->cell( 3, 0.4, $nrValorConcedente, 1, 1, 'R' );
			}
		}		
		$obPdf->valorTotalProjeto = $valorTotalProjeto;
	}
*/	
	
	function gerarAnexo4( $width, $bgColor, $arAgrupado ){
		global $db;
		$_SESSION['nrAnexo'] = 4;
		$dadosCabecalho = $_SESSION['arDadosCabecalho'];

		$html = '<table '.$width.' align="center" cellspacing="0" cellpadding="0">
		<tr>
			<td>
			<table '.$width.' class="lista" align="center" '.$bgColor.' cellspacing="1" cellpadding="4">
				<tr>
					<td class="fot" valign="top">MEC / FNDE</td>
					<td valign="top" style="text-align: center;"><span class="fot">PLANO
					DE TRABALHO</span> <br>
					<b><span style="text-align: center; font-size: 9px">DESCRI��O DO PROJETO</span></b></td>
					<td class="fot">ANEXO<br>
					4</td>
				</tr>
			</table>
			<table '.$width.' class="lista" align="center" cellspacing="1"
				cellpadding="4">
				<tr>
					<td valign="top" style="text-align: center;"><b>PLANO DE A��ES ARTICULADAS - PAR</b></td>
				</tr>
			</table>
			<table '.$width.' class="lista" align="center" cellspacing="1"
				cellpadding="4">
				<tr>
					<td><b>1 - EXERC�CIO</b><BR>
						&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;'.$dadosCabecalho['nrExercicio'].'</td>
					<td><b>2 - N�VEL DE ENSINO</b><br>
					&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Educa��o B�sica</td>
					<td><b>3 - ABRANG�NCIA DO PROJETO</b><BR>
					&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;PAR</td>
				</tr>
				<tr>
					<td><b>4 - CNPJ</b><BR>
						&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;'.$dadosCabecalho['cnpjOrgao'].'</td>
					<td colspan="2"><b>5 - NOME DO �RG�O OU ENTIDADE</b><br>
					&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;'.$dadosCabecalho['nomeOrgao'].'</td>
				</tr>
				<tr>
					<td colspan="2"><b>6 - '.($itrid == 2 ? "MUNIC�PIO" : "ESTADO").'</b><BR>
						&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;'.$dadosCabecalho['mundescricao'].'</td>
					<td style="width: 25%;"><b>7 - UF</b><br>
					&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;'.$dadosCabecalho['estuf'].'</td>
				</tr>
				<tr>
					<td colspan="3"><b>8 - A��O A SER EXECUTADA</b><BR>
						&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;'.implode( "\n", $dadosCabecalho['dimcod'] ).'</td>
				</tr>
			</table>';

			$stSbaid = array();
			foreach( $arAgrupado as $stDimensao => $arDimensao ){
				
				$stSbaid[] = "'" . implode( "', '", array_keys( $arDimensao ) ). "'";
			}
			
			$stSbaid = implode( ",", $stSbaid );
			
			// Detalhamento da Suba��o			
			$sql = "select min( a.acidtinicial ) AS cronogramaexecucaoinicial, max( a.acidtfinal ) AS cronogramaexecucaofinal
					from cte.subacaoindicador s
						inner join cte.acaoindicador a on a.aciid = s.aciid
					where s.sbaid in ( $stSbaid )";
		
			$arDado = $db->carregar( $sql );
			$arDado = $arDado ? $arDado : array();
			
			$obData = new Data();
			$nrQuantidadeDias = $obData->quantidadeDeDiasEntreDuasDatas( $arDado[0]['cronogramaexecucaoinicial'], $arDado[0]['cronogramaexecucaofinal'] );
		
			$valorTotal = $_SESSION['cte']['pta']['valortotal'];
			
			$nrValorProponente = tratarValor( $valorTotal * 0.01 );
			$nrValorConcedente = tratarValor( $valorTotal * 0.99 );
			
			$html.='<table '.$width.' class="lista" align="center" '.$bgColor.' cellspacing="1" cellpadding="4">
				<tr>
					<td align="center" colspan="6"><b>9 - CRONOGRAMA DE EXECU��O</b></td>
				</tr>
				<tr>
					<td><b>9.1 IN�CIO - DIA/M�S/ANO</b></td>
					<td bgcolor="white" style="width: 10%;">'.$obData->formataData($arDado[0]['cronogramaexecucaoinicial']).'</td>
					<td><b>9.2 T�RMINO - DIA/M�S/ANO</b></td>
					<td bgcolor="white" style="width: 10%;">'.$obData->formataData($arDado[0]['cronogramaexecucaofinal']).'</td>
					<td><b>9.3 - QUANTIDADE DE DIAS</b></td>
					<td bgcolor="white" style="width: 10%;">'.$nrQuantidadeDias.'</td>
				</tr>
				<tr>
					<td colspan="6"><b>10 - CRONOGRAMA DE DESEMBOLSO - VALORES CONCEDENTES</b></td>
				</tr>
				<tr>
					<td colspan="6"><b>M�S/ANO&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;VALOR</b></td>
				</tr>
				<tr>
					<td colspan="6" bgcolor="white"><b>'.date( 'm/Y' ).'&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;'.$nrValorConcedente.'</b></td>
				</tr>
				<tr>
					<td colspan="5"><b>VALOR TOTAL A SER DESEMBOLSADO PELO CONCEDENTE</b></td>
					<td style="width: 10%;" align="right">'.$nrValorConcedente.'</td>
				</tr>
				<tr>
					<td colspan="6"><b>11 - CRONOGRAMA DE DESEMBOLSO - VALORES CONCEDENTES</b></td>
				</tr>
				<tr>
					<td colspan="6"><b>M�S/ANO&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;VALOR</b></td>
				</tr>
				<tr>
					<td colspan="6" bgcolor="white"><b>'.date( 'm/Y' ).'&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;'.$nrValorConcedente.'</b></td>
				</tr>
				<tr>
					<td colspan="5"><b>VALOR TOTAL A SER DESEMBOLSADO PELO PROPONENTE (valor m�nimo de 1%)</b></td>
					<td style="width: 10%;" align="right">'.$nrValorProponente.'</td>
				</tr>
				<tr>
					<td colspan="5"><b>VALOR TOTAL DO PROJETO</b></td>
					<td style="width: 10%;" align="right">'.tratarValor( $valorTotal ).'</td>
				</tr>
			</table>
			</td>
			</tr>
			</table>
			'.footer($width, $bgColor);
			return $html;
	}
	
	
	
	
/*	
	function gerarAnexo4( $obPdf, $arAgrupado ){
		
		global $db;
		$_SESSION['nrAnexo'] = 4;
		unset( $obPdf->aligns );
		$obPdf->AddPage(); //adicionando um nova p�gina
		
		$stSbaid = array();
		foreach( $arAgrupado as $stDimensao => $arDimensao ){
			
			$stSbaid[] = "'" . implode( "', '", array_keys( $arDimensao ) ). "'";
		}
		
		$stSbaid = implode( ",", $stSbaid );
		
		// Detalhamento da Suba��o			
		$sql = "select min( a.acidtinicial ) AS cronogramaexecucaoinicial, max( a.acidtfinal ) AS cronogramaexecucaofinal
				from cte.subacaoindicador s
					inner join cte.acaoindicador a on a.aciid = s.aciid
				where s.sbaid in ( $stSbaid )";
		
		$arDado = $db->carregar( $sql );
		$arDado = $arDado ? $arDado : array();
		
		$obData = new Data();
		$nrQuantidadeDias = $obData->quantidadeDeDiasEntreDuasDatas( $arDado[0]['cronogramaexecucaoinicial'], $arDado[0]['cronogramaexecucaofinal'] );
		
		$nrValorProponente = tratarValor( $obPdf->valorTotalProjeto * 0.01 );
		$nrValorConcedente = tratarValor( $obPdf->valorTotalProjeto * 0.99 );
		
		$obPdf->SetFillColor( 200, 200, 200 );
		$obPdf->SetFont('Arial', 'B', 7);
		$obPdf->cell( 19, 0.4, '8 - CRONOGRAMA DE EXECU��O', 									1, 1, 'C', 1 );
		
		$obPdf->cell( 4, 0.4, '8.1 IN�CIO - M�S/ANO', 											1, 0, 'L', 1 );
		$obPdf->SetFont('Arial', '', 7);
		$obPdf->cell( 2, 0.4, $obData->formataData( $arDado[0]['cronogramaexecucaoinicial'] ), 	1, 0, 'L' );
		$obPdf->SetFont('Arial', 'B', 7);
		$obPdf->cell( 4, 0.4, '8.2 T�RMINO - M�S/ANO', 											1, 0, 'L', 1 );
		$obPdf->SetFont('Arial', '', 7);
		$obPdf->cell( 2, 0.4, $obData->formataData( $arDado[0]['cronogramaexecucaofinal'] ), 	1, 0, 'L' );
		$obPdf->SetFont('Arial', 'B', 7);
		$obPdf->cell( 5, 0.4, '8.3 - QUANTIDADE DE DIAS', 										1, 0, 'L', 1 );
		$obPdf->SetFont('Arial', '', 7);
		$obPdf->cell( 2, 0.4, $nrQuantidadeDias, 												1, 1, 'L' );
		
		$obPdf->SetFont('Arial', 'B', 7);
		$obPdf->cell( 19, 0.4, '9 - CRONOGRAMA DE DESEMBOLSO -  VALORES CONCEDENTES', 									1, 1, 'L', 1 );
		$obPdf->cell( 19, 0.4, 'M�S/ANO          VALOR', 											1, 1, 'L', 1 );
		$obPdf->SetFont('Arial', '', 7);
		$obPdf->cell( 19, 0.4, date( 'm/Y' ) . '              ' . $nrValorConcedente,	1, 1, 'L' );
		$obPdf->SetFont('Arial', 'B', 5);
		$obPdf->cell( 16, 0.3, 'VALOR TOTAL A SER DESEMBOLSADO PELO CONCEDENTE', 1, 0, 'L', 1 );
		$obPdf->SetFont('Arial', '', 5);
		$obPdf->cell( 03, 0.3, $nrValorConcedente, 1, 1, 'R', 1 );
		
		$obPdf->SetFont('Arial', 'B', 7);
		$obPdf->cell( 19, 0.4, '10 - CRONOGRAMA DE DESEMBOLSO - VALORES PROPONENTES', 									1, 1, 'L', 1 );
		$obPdf->cell( 19, 0.4, 'M�S/ANO          VALOR', 											1, 1, 'L', 1 );
		$obPdf->SetFont('Arial', '', 7);
		$obPdf->cell( 19, 0.4, date( 'm/Y' ) . '              ' . $nrValorConcedente,	1, 1, 'L' );
		$obPdf->SetFont('Arial', 'B', 5);
		$obPdf->cell( 16, 0.3, 'VALOR TOTAL A SER DESEMBOLSADO PELO PROPONENTE (valor m�nimo de 1%)', 1, 0, 'L', 1 );
		$obPdf->SetFont('Arial', '', 5);
		$obPdf->cell( 03, 0.3, $nrValorProponente, 1, 1, 'R', 1 );
		
		$obPdf->cell( 16, 0.3, 'VALOR TOTAL DO PROJETO', 1, 0, 'L', 1 );
		$obPdf->SetFont('Arial', '', 5);
		$obPdf->cell( 03, 0.3, tratarValor( $obPdf->valorTotalProjeto ), 1, 1, 'R', 1 );
		
		$obPdf->SetFont('Arial', 'B', 7);
	}
*/	
	
	function gerarAnexo5( $width, $bgColor, $arAgrupado ){
		
		global $db;
		$_SESSION['nrAnexo'] = 5;
		$dadosCabecalho = $_SESSION['arDadosCabecalho'];
		$itrid = cte_pegarItrid( $_SESSION['inuid'] );
		foreach( $arAgrupado as $stDimensao => $arDimensao ){ 
			foreach( $arDimensao as $arSubacao ){
				$html .= '<div class="folha">';
				$html .= '<table '.$width.' align="center" cellspacing="0" cellpadding="0">
					<tr>
						<td>
							<table '.$width.' class="lista" align="center" '.$bgColor.' cellspacing="1" cellpadding="4">
								<tr>
									<td class="fot" valign="top">MEC / FNDE</td>
									<td valign="top" style="text-align: center;"><span class="fot">PLANO
									DE TRABALHO</span> <br>
									<b><span style="text-align: center; font-size: 9px">DESCRI��O DO PROJETO</span></b></td>
									<td class="fot">ANEXO<br>
									5</td>
								</tr>
							</table>
							<table '.$width.' class="lista" align="center" cellspacing="1" cellpadding="4">
								<tr>
									<td valign="top" style="text-align: center;">Utilize quantos formul�rios forem necess�rios para a complementa��o deste anexo.</td>
								</tr>
							</table>
							<table '.$width.' class="lista" align="center" cellspacing="1" cellpadding="4">
								<tr>
									<td><b>1 - EXERC�CIO</b><BR>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;'.$dadosCabecalho['nrExercicio'].'</td>
									<td colspan="4"><b>2 - A��O A SER EXECUTADA</b><br>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;'.$arSubacao['dimdsc'].'</td>
								</tr>
								<tr>
									<td><b>3 - CNPJ</b><BR>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;'.$dadosCabecalho['cnpjOrgao'].'</td>
									<td colspan="3"><b>4 - NOME DO �RG�O OU ENTIDADE</b><br>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;'.$dadosCabecalho['nomeOrgao'].'</td>
									<td>&nbsp;</td>
								</tr>
								<tr>
									<td colspan="4"><b>5 - '.($itrid == 2 ? "MUNIC�PIO" : "ESTADO").'</b><BR>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;'.$dadosCabecalho['mundescricao'].'</td>
									<td ><b>6 - UF</b><br>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;'.$dadosCabecalho['estuf'].'</td>
								</tr>';
								$html.='<tr>
									<td colspan="5"><b>7 - ESPECIFICA��O DA A��O</b><BR>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;'.$arSubacao['sbadsc'].'</td>
								</tr>
								<tr>
									<td colspan="5">8 - DETALHAMENTO DOS ITENS QUE COMP�EM A ESPECIFICA��O</td>
								</tr>
								<tr>
									<td rowspan="2"><b>8.1 IDENTIFICA��O DOS ITENS</b></td>
									<td rowspan="2"><b>8.2 UNIDADE DE MEDIDA</b></td>
									<td rowspan="2"><b>8.3 QUANTIDADE</b></td>
									<td colspan="2" align="center"><b>8.4 ESTIMATIVA DE CUSTO</b></td>
								</tr>
								<tr>
									<td align="center"><b>VALOR UNIT�RIO</b></td>
									<td align="center"><b>VALOR TOTAL</b></td>
								</tr>';
								
									$sql = "select cos.cosdsc, u.undddsc, cos.cosqtd, cos.cosvlruni, ( cos.cosqtd * cos.cosvlruni ) as valortotal
											from cte.subacaoindicador s
												inner join cte.composicaosubacao cos on cos.sbaid = s.sbaid
												inner join cte.unidademedidadetalhamento u on u.unddid = cos.unddid
											where s.sbaid = {$arSubacao['sbaid']}
											and cosano = {$dadosCabecalho['nrExercicio']}
											order by u.undddsc";
								
									$arDado = $db->carregar( $sql );
									$arDado = $arDado ? $arDado : array();
									
									$valorTotal = 0;
									foreach( $arDado as $count => $arItem ){
							
										$valorTotal += $arItem['valortotal'];
		
										$html.='<tr>
													<td>'.( $count +1 ) . ' - ' . $arItem['cosdsc'].'</td>
													<td>'.$arItem['undddsc'].'</td>
													<td>'.round( $arItem['cosqtd'] ).'</td>
													<td>'.tratarValor( $arItem['cosvlruni'] ).'</td>
													<td>'.tratarValor( $arItem['valortotal'] ).'</td>
												</tr>';
									}
														
									$html.='<tr bgcolor="silver">
										<td colspan="4">TOTAL DESTE ANEXO</td>
										<td>'.tratarValor( $valorTotal ).'</td>
									</tr>
									</table>
								</td>
							</tr>
						</table>
						'.footer($width, $bgColor);
						$html .= '</div>';
			} 
		}
		return $html;
	}
	
	
/*	
	function gerarAnexo5( $obPdf, $arAgrupado ){
		
		global $db;
		$_SESSION['nrAnexo'] = 5;
		$arDadosCabecalho = $_SESSION['arDadosCabecalho'];
		$itrid = cte_pegarItrid( $_SESSION['inuid'] );
				
		unset( $obPdf->aligns );
		
		foreach( $arAgrupado as $stDimensao => $arDimensao ){
			
			foreach( $arDimensao as $arSubacao ){
			
				$obPdf->AddPage(); //adicionando um nova p�gina
				$arFormatacao = array( 'Arial', '', 7, 'B' );
				
				// Primeira Linha
				$arWidth = array( 3, 16 );
				
				$arTitulo[] = '1 - EXERC�CIO';
				$arTitulo[] = '2 - A��O A SER EXECUTADA';
				
				
				$arDados[] = $arDadosCabecalho['nrExercicio'];
				$arDados[] = $arSubacao['dimdsc'];
				
				$obPdf->montarRowComTitulo( $arTitulo, $arDados, $arWidth, $arFormatacao );
				unset( $arDados, $arTitulo, $arWidth );
				
				// Segunda Linha
				$arWidth = array( 3, 16 );
				
				$arTitulo[] = '3 - CNPJ';
				$arTitulo[] = '4 - NOME DO �RG�O OU ENTIDADE';
				
				$arDados[] = $arDadosCabecalho['cnpjOrgao'];
				$arDados[] = $arDadosCabecalho['nomeOrgao'];
				
				$obPdf->montarRowComTitulo( $arTitulo, $arDados, $arWidth, $arFormatacao );
				unset( $arDados, $arTitulo, $arWidth );
				
				// Terceira Linha
				$arWidth = array( 17, 2 );
				
				
				$arTitulo[] = $itrid == INSTRUMENTO_DIAGNOSTICO_MUNICIPAL ? '5 - MUNIC�PIO' : '5 - ESTADO';
				$arTitulo[] = '6 - UF';
				
				$arDados[] = $arDadosCabecalho['mundescricao'];
				$arDados[] = $arDadosCabecalho['estuf'];
				
				$obPdf->montarRowComTitulo( $arTitulo, $arDados, $arWidth, $arFormatacao );
				unset( $arDados, $arTitulo, $arWidth );
				
				// Quarta Linha
				$arWidth = array( 19 );
				
				$arTitulo[] = '7 - ESPECIFICA��O DA A��O';
				
				$arDados[] = $arSubacao['sbadsc'];
					
				$obPdf->montarRowComTitulo( $arTitulo, $arDados, $arWidth, $arFormatacao );
				unset( $arDados, $arTitulo, $arWidth );				
				
				// Detalhamento da Suba��o			
				$sql = "select cos.cosdsc, u.undddsc, cos.cosqtd, cos.cosvlruni, ( cos.cosqtd * cos.cosvlruni ) as valortotal
						from cte.subacaoindicador s
							inner join cte.composicaosubacao cos on cos.sbaid = s.sbaid
							inner join cte.unidademedidadetalhamento u on u.unddid = cos.unddid
						where s.sbaid = {$arSubacao['sbaid']}
						and cosano = {$arDadosCabecalho['nrExercicio']}
						order by u.undddsc";
				
				$arDado = $db->carregar( $sql );
				$arDado = $arDado ? $arDado : array();
	
				$obPdf->SetFillColor( 200, 200, 200 );
				$obPdf->SetFont('Arial', 'B', 7);
				$obPdf->cell( 19, 0.4, '8 - DETALHAMENTO DOS ITENS QUE COMP�EM A ESPECIFICA��O',	1, 1 );
				
				$obPdf->cell( 6.5, 0.8, '8.1 IDENTIFICA��O DOS ITENS', 								1, 0 );
				$obPdf->cell( 4, 0.8, '8.2 UNIDADE DE MEDIDA', 										1, 0 );
				$obPdf->cell( 2.5, 0.8, '8.3 QUANTIDADE', 											1, 0 );
				$obPdf->cell( 6, 0.4, '8.4 ESTIMATIVA DE CUSTO', 									1, 1, 'C' );
				
				$obPdf->setX( 14 );
				$obPdf->cell( 3, 0.4, 'VALOR UNIT�RIO', 											1, 0, 'C' );
				$obPdf->cell( 3, 0.4, 'VALOR TOTAL', 												1, 1, 'C' );
	
				$obPdf->SetFont( 'Arial', '', 7 );
				$valorTotal = 0;
				foreach( $arDado as $count => $arItem ){
					
					$valorTotal += $arItem['valortotal'];
					
					$obPdf->SetWidths( array( 6.5, 4, 2.5, 3, 3 ) );
					$obPdf->SetAligns( array( 'L', 'L', 'C', 'R', 'R' ) );
					$obPdf->Row( array( ( $count +1 ) . ' - ' . $arItem['cosdsc'], $arItem['undddsc'], round( $arItem['cosqtd'] ), tratarValor( $arItem['cosvlruni'] ), tratarValor( $arItem['valortotal'] ) ) );			
				}
				
				$obPdf->cell( 16, 0.4, 'TOTAL DESTE ANEXO', 										1, 0, 'L', 1 );
				$obPdf->cell(  3, 0.4, tratarValor( $valorTotal ), 									1, 1, 'R', 1 );							
			}			
		}
	}
	
*/
	
	function gerarAnexo6( $width, $bgColor, $arAgrupado ){
		
		global $db;
		$_SESSION['nrAnexo'] = 6;
		$dadosCabecalho = $_SESSION['arDadosCabecalho'];
		$itrid = $_SESSION['par']['itrid'];
		$vazio = 0;
		
		$html .= '<div class="folha">';
		$html = '<table '.$width.' align="center" cellspacing="0" cellpadding="0">
			<tr>
				<td>
					<table '.$width.' class="lista" align="center" '.$bgColor.' cellspacing="1" cellpadding="4">
						<tr>
							<td class="fot" valign="top">MEC / FNDE</td>
							<td valign="top" style="text-align: center;"><span class="fot">PLANO
							DE TRABALHO</span> <br>
							<b><span style="text-align: center; font-size: 9px">DESCRI��O DO PROJETO</span></b></td>
							<td class="fot">ANEXO<br>
							6</td>
						</tr>
					</table>
					<table '.$width.' class="lista" align="center" cellspacing="1" cellpadding="4">
						<tr>
							<td valign="top" style="text-align: center;">Utilize quantos formul�rios forem necess�rios para a complementa��o deste anexo.</td>
						</tr>
					</table>
					<table '.$width.' class="lista" align="center" cellspacing="1" cellpadding="4">';
		//				foreach( $arAgrupado as $stDimensao => $arDimensao ){ 
		//					foreach( $arDimensao as $arSubacao ){
						$html.='<tr>
							<td><b>1 - EXERC�CIO</b><BR>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;'.$dadosCabecalho['sbdano'].'</td>
							<td colspan="3"><b>2 - N�VEL DE ENSINO</b><br>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Educa��o B�sica</td>
							<td><b>3 - ABRANG�NCIA DO PROJETO</b><br>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;PAR</td>
						</tr>
						<tr>
							<td><b>4 - CNPJ</b><BR>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;'.$dadosCabecalho['cnpjOrgao'].'</td>
							<td colspan="4"><b>5 - NOME DO �RG�O OU ENTIDADE</b><br>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;'.$dadosCabecalho['nomeOrgao'].'</td>
						</tr>
						<tr>
							<td colspan="4"><b>6 - '.($itrid == 2 ? "MUNIC�PIO" : "ESTADO").'</b><BR>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;'.$dadosCabecalho['mundescricao'].'</td>
							<td ><b>7 - UF</b><br>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;'.$dadosCabecalho['estuf'].'</td>
						</tr>
						<tr>
							<td colspan="5"><b>8 - ESCOLAS BENEFICIADAS</b><BR>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;'.$arSubacao['sbadsc'].'</td>
						</tr>';
						$stSbaid = array();
						foreach( $arAgrupado as $stDimensao => $arDimensao ){
							
							$stSbaid[] = "'" . implode( "', '", array_keys( $arDimensao ) ). "'";
						}
						
						$stSbaid = implode( ",", $stSbaid );
						
						$sql = "SELECT  e.entid, e.entnome, e.entcodent, cs.cosdsc, to_char(cs.cosvlruni, '99999999.99') AS cosvlruni, und.undddsc,
										sum( ec.ecsqtd ) as ecsqtd, 
										sum( ec.ecsqtd*cs.cosvlruni ) AS total 
								FROM cte.composicaosubacao AS cs
									INNER JOIN cte.subacaoindicador si ON si.sbaid = cs.sbaid --AND sbaporescola = 't'
									INNER JOIN cte.escolacomposicaosubacao AS ec ON ec.cosid = cs.cosid
									INNER JOIN cte.qtdfisicoano qfa on qfa.qfaid = ec.qfaid
									INNER JOIN entidade.entidade e on e.entid = qfa.entid
									INNER JOIN cte.unidademedidadetalhamento und ON cs.unddid = und.unddid 
								WHERE cs.sbaid in( $stSbaid )
								AND ecsqtd != 0 
								and qfaano  = '{$dadosCabecalho['nrExercicio']}'
								group by e.entid, cs.cosdsc, cosvlruni, und.undddsc, e.entnome, e.entcodent
								ORDER BY e.entnome, cs.cosdsc, und.undddsc";

							$arDado = $db->carregar( $sql );
							$arDado = $arDado ? $arDado : array();
							
							foreach( $arDado as $dados ){
								$arEscolas[$dados['entcodent']. '-' .$dados['entnome']][] = $dados;
							}
							if( is_array($arEscolas) ){
								foreach( $arEscolas as $entnome => $arEscola ){
					
									$arDadosEscola = explode( '-', $entnome );
									
									$html .= '<table '.$width.' class="lista" align="center" cellspacing="1" cellpadding="4">
												<tr>
													<td width="20%"><b>C�DIGO IBGE</b><BR>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;'.$arDadosEscola[0].'</td>
													<td colspan="5"><b>ESCOLA</b><br>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;'.$arDadosEscola[1].'</td>
												</tr>
												<tr>
													<td colspan=2><b>Itens</b></td>
													<td><b>Unidade de Medida</b></td>
													<td><b>Quantidade</b></td>
													<td><b>Pre�o Unit�rio</b></td>
													<td><b>Total (R$)</b></td>
												</tr>';
									
									$nrQtdTotal = 0;
									$nrValorTotal = 0;
									foreach( $arEscola as $arEscolaDetalhe ){
										$nrQtdTotal += $arEscolaDetalhe['ecsqtd'];
										$nrValorTotal += $arEscolaDetalhe['total'];
										
										$html .= '<tr>
													<td colspan=2 width=40%>'.$arEscolaDetalhe['cosdsc'].'</td>
													<td>'.$arEscolaDetalhe['undddsc'].'</td>
													<td>'.$arEscolaDetalhe['ecsqtd'].'</td>
													<td>'.tratarValor( $arEscolaDetalhe['cosvlruni'] ).'</td>
													<td>'.tratarValor( $arEscolaDetalhe['total'] ).'</td>
												</tr>';
									}
									
									$html .= '<tr>
													<td colspan=3><b>Total</b></td>
													<td><b>'.$nrQtdTotal.'</b></td>
													<td></td>
													<td><b>'.tratarValor( $nrValorTotal ).'</b></td>
												</tr>
											</table><br>';
									/*
									$obPdf->SetFont('Arial', 'B', 7);
									$obPdf->cell( 3, 0.4, 'C�DIGO IBGE', 														1, 0 );
									$obPdf->cell( 16, 0.4, 'ESCOLA', 															1, 1 );
									$obPdf->SetFont('Arial', '', 7);
									$obPdf->cell( 3, 0.4, $arDadosEscola[0], 													1, 0 );
									$obPdf->cell( 16, 0.4, $arDadosEscola[1], 													1, 1 );
									
									$obPdf->SetFont('Arial', 'B', 7);
									$obPdf->cell( 8, 0.4, 'Itens', 																1, 0 );
									$obPdf->cell( 5, 0.4, 'Unidade de Medida', 													1, 0 );
									$obPdf->cell( 2, 0.4, 'Quantidade', 														1, 0 );
									$obPdf->cell( 2, 0.4, 'Pre�o Unit�rio', 													1, 0 );
									$obPdf->cell( 2, 0.4, 'Total (R$)', 														1, 1 );
									
									$obPdf->SetFont('Arial', '', 7);
									$nrQtdTotal = 0;
									$nrValorTotal = 0;
									foreach( $arEscola as $arEscolaDetalhe ){
										
										if( $obPdf->getY() > 23 ){
											$obPdf->AddPage();					
										}
										
										$nrQtdTotal += $arEscolaDetalhe['ecsqtd'];
										$nrValorTotal += $arEscolaDetalhe['total'];
										
										$obPdf->SetWidths( array( 8, 5, 2, 2, 2 ) );
										$obPdf->SetAligns( array( 'L', 'L', 'L', 'L', 'L' ) );
										$obPdf->Row( array( $arEscolaDetalhe['cosdsc'], $arEscolaDetalhe['undddsc'], $arEscolaDetalhe['ecsqtd'], tratarValor( $arEscolaDetalhe['cosvlruni'] ), tratarValor( $arEscolaDetalhe['total'] ) ) );				
										
									}
									
									$obPdf->SetFont('Arial', 'B', 7);
									$obPdf->SetWidths( array( 13, 2, 2, 2 ) );
									$obPdf->SetAligns( array( 'L', 'L', 'L', 'L', 'L' ) );
									$obPdf->Row( array( 'Total', $nrQtdTotal, '', tratarValor( $nrValorTotal ) ) );
									$obPdf->ln(0.2);
	*/
								}
							} else {
								$vazio = 1;
							}
		//				}							
		//			}
					$html.='</table>
				</td>
			</tr>
		</table>
		'.footer($width, $bgColor);
		$html.='</div>';
		if( $vazio == 1 ){
			return '';	
		}else{
			return $html;
		}
	}
	
	/*
	
	function gerarAnexo6( $obPdf, $arAgrupado, $stSbaid ){
		
		global $db;
		$_SESSION['nrAnexo'] = 6;
		$arDadosCabecalho = $_SESSION['arDadosCabecalho'];
		$itrid = cte_pegarItrid( $_SESSION['inuid'] );
		
		
		
		$sql = "SELECT  e.entid, e.entnome, e.entcodent, cs.cosdsc, to_char(cs.cosvlruni, '99999999.99') AS cosvlruni, und.undddsc,
						sum( ec.ecsqtd ) as ecsqtd, 
						sum( ec.ecsqtd*cs.cosvlruni ) AS total 
				FROM cte.composicaosubacao AS cs
					INNER JOIN cte.escolacomposicaosubacao AS ec ON ec.cosid = cs.cosid
					INNER JOIN cte.qtdfisicoano qfa on qfa.qfaid = ec.qfaid
					INNER JOIN entidade.entidade e on e.entid = qfa.entid
					INNER JOIN cte.unidademedidadetalhamento und ON cs.unddid = und.unddid 
				WHERE cs.sbaid in( $stSbaid )
				AND ecsqtd != 0 
				and qfaano  = '{$arDadosCabecalho['nrExercicio']}'
				group by e.entid, cs.cosdsc, cosvlruni, und.undddsc, e.entnome, e.entcodent
				ORDER BY e.entnome, cs.cosdsc, und.undddsc";
		
		$arDado = $db->carregar( $sql );
		$arDado = $arDado ? $arDado : array();
		
		if( count( $arDado ) ){
			
			$obPdf->AddPage(); //adicionando um nova p�gina
			$arFormatacao = array( 'Arial', '', 7, 'B' );		
			
			foreach( $arDado as $dados ){
				$arEscolas[$dados['entcodent']. '-' .$dados['entnome']][] = $dados;
			}
	
			
			$obPdf->SetFont('Arial', 'B', 7);
			$obPdf->cell( 19, 0.4, '8 - ESCOLAS BENEFICIADAS', 												1, 1 );
			
			foreach( $arEscolas as $entnome => $arEscola ){
				
				$arDadosEscola = explode( '-', $entnome );
				
				$obPdf->SetFont('Arial', 'B', 7);
				$obPdf->cell( 3, 0.4, 'C�DIGO IBGE', 														1, 0 );
				$obPdf->cell( 16, 0.4, 'ESCOLA', 															1, 1 );
				$obPdf->SetFont('Arial', '', 7);
				$obPdf->cell( 3, 0.4, $arDadosEscola[0], 													1, 0 );
				$obPdf->cell( 16, 0.4, $arDadosEscola[1], 													1, 1 );
				
				$obPdf->SetFont('Arial', 'B', 7);
				$obPdf->cell( 8, 0.4, 'Itens', 																1, 0 );
				$obPdf->cell( 5, 0.4, 'Unidade de Medida', 													1, 0 );
				$obPdf->cell( 2, 0.4, 'Quantidade', 														1, 0 );
				$obPdf->cell( 2, 0.4, 'Pre�o Unit�rio', 													1, 0 );
				$obPdf->cell( 2, 0.4, 'Total (R$)', 														1, 1 );
				
				$obPdf->SetFont('Arial', '', 7);
				$nrQtdTotal = 0;
				$nrValorTotal = 0;
				foreach( $arEscola as $arEscolaDetalhe ){
					
					if( $obPdf->getY() > 23 ){
						$obPdf->AddPage();					
					}
					
					$nrQtdTotal += $arEscolaDetalhe['ecsqtd'];
					$nrValorTotal += $arEscolaDetalhe['total'];
					
					$obPdf->SetWidths( array( 8, 5, 2, 2, 2 ) );
					$obPdf->SetAligns( array( 'L', 'L', 'L', 'L', 'L' ) );
					$obPdf->Row( array( $arEscolaDetalhe['cosdsc'], $arEscolaDetalhe['undddsc'], $arEscolaDetalhe['ecsqtd'], tratarValor( $arEscolaDetalhe['cosvlruni'] ), tratarValor( $arEscolaDetalhe['total'] ) ) );				
					
				}
				
				$obPdf->SetFont('Arial', 'B', 7);
				$obPdf->SetWidths( array( 13, 2, 2, 2 ) );
				$obPdf->SetAligns( array( 'L', 'L', 'L', 'L', 'L' ) );
				$obPdf->Row( array( 'Total', $nrQtdTotal, '', tratarValor( $nrValorTotal ) ) );
				$obPdf->ln(0.2);
			}
		}
	}	
	*/
	
	function tratarValor( $nrValor ){
		return "R$ " . number_format( round( ( $nrValor ), 2 ) , 2, ",", "." );
	}
	
?>