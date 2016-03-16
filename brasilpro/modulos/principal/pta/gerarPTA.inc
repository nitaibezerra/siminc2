<?php 

	require_once( APPRAIZ . 'includes/fpdf/pdfPTATable.inc');
	require_once( APPRAIZ . 'includes/classes/dateTime.inc');

	$prsid = $_REQUEST['prsid'];
	
	$sql = "select distinct sbaid, prsjustificativapta, prsano
			from  cte.projetosapesubacao p
				inner join cte.projetosape ps on ps.prsid = p.prsid 
			where p.prsid = $prsid";
	
	$arDados = $db->carregar( $sql );
	$arDados = $arDados ? $arDados : array();
	
	$ano = '0';
	$arSbaid = array();
	foreach( $arDados as $sbaid ){
		$ano = $sbaid['prsano'];
		$arSbaid[] = $sbaid['sbaid'];
	}
	
//	$arSbaid = array( 3181309 );
	
	$stJustificativa = $sbaid['prsjustificativapta'];
	
	gerarPdfPta( $arSbaid, $stJustificativa, $ano );
	
	function gerarPdfPta( $idsSubacao, $stJustificativa, $ano ){
		
		global $db;
		$itrid = cte_pegarItrid( $_SESSION['inuid'] );
		
		//Chamando a classe de FPDF
		$obPdf = new PDF_PTA_Table( 'P','cm','A4' ); //documento em formato de retrato, medido em cm e no tamanho A4
//		$obPdf->bMargin = 1;
		$obPdf->setEntreLinha(4);
		
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
			
			gerarAnexo1( $obPdf, $arAgrupado, $stJustificativa );
			gerarAnexo2( $obPdf, $arAgrupado );
			gerarAnexo3( $obPdf, $arAgrupado );
			gerarAnexo4( $obPdf, $arAgrupado );
			gerarAnexo5( $obPdf, $arAgrupado );
			gerarAnexo6( $obPdf, $arAgrupado, $stSbaid );
	
			$obPdf->Output( 'PTA.pdf', 'D' );
		}
	}

	function gerarAnexo1( $obPdf, $arAgrupado, $stJustificativa ){
		
		$_SESSION['nrAnexo'] = 1;
		$obPdf->AddPage(); //adicionando um nova página
		
		$arFormatacao = array( 'Arial', '', 7, 'B' );
		
		// Quinta Linha
		$arWidth = array( 19 );
		
		$arTitulo[] = '10 - JUSTIFICATIVA DO PROJETO';
		
		$arDados[] = $stJustificativa;
		
		$obPdf->montarRowComTitulo( $arTitulo, $arDados, $arWidth, $arFormatacao );
		unset( $arDados, $arTitulo, $arWidth );
		
	}

	function gerarAnexo2( $obPdf, $arAgrupado ){
//		
		global $db;
		$_SESSION['nrAnexo'] = 2;
		$arDadosCabecalho = $_SESSION['arDadosCabecalho'];
		
		foreach( $arAgrupado as $stDimensao => $arDimensao ){
			
			unset( $obPdf->aligns );
			$obPdf->AddPage(); //adicionando um nova página
			$arFormatacao = array( 'Arial', '', 7, 'B' );
			
			// Terceira Linha
			$arWidth = array( 19 );
			$arTitulo[] = '4 - AÇÃO A SER EXECUTADA';
			
			$arDados[] = $stDimensao;
			
			$obPdf->montarRowComTitulo( $arTitulo, $arDados, $arWidth, $arFormatacao );
			unset( $arDados, $arTitulo, $arWidth );

			// Detalhamento da Subação
				
			$stDetalhamento = '';
			foreach( $arDimensao as $count => $arSubacao ){
					
				$stDetalhamento .= "\n\nDimensão: {$arSubacao['dimcod']} - {$arSubacao['dimdsc']}\n";
				$stDetalhamento .= "Área: {$arSubacao['dimcod']}.{$arSubacao['ardcod']} - {$arSubacao['arddsc']}\n";
				$stDetalhamento .= "Indicador: {$arSubacao['dimcod']}.{$arSubacao['ardcod']}.{$arSubacao['indcod']} - {$arSubacao['inddsc']}\n";
				$stDetalhamento .= "Subação: {$arSubacao['dimcod']}.{$arSubacao['ardcod']}.{$arSubacao['indcod']} ({$arSubacao['sbaordem']}) - {$arSubacao['sbadsc']}\n";
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
			$obPdf->Row( array( '5 - BENEFICIÁRIOS DA AÇÃO' ) );
						
			$obPdf->SetFont( 'Arial', 'B', 7 );
			$obPdf->SetWidths( array( 7, 4, 4, 4 ) );
			$obPdf->SetAligns( array( 'L', 'C', 'C', 'C' ) );
			$obPdf->Row( array( '     5.1 - BENEFICIÁRIOS DA AÇÃO', '5.2 - ZONA RURAL', '5.3 - ZONA URBANA', '5.4 - TOTAL' ) );			
			
			$obPdf->SetFont( 'Arial', '', 7 );
			foreach( $arBeneficiarios as $arBeneficiario ){
				$obPdf->SetWidths( array( 7, 4, 4, 4 ) );
				$obPdf->SetAligns( array( 'L', 'C', 'C', 'C' ) );
				$obPdf->Row( array( "     " . $arBeneficiario['bendsc'], $arBeneficiario['rural'], $arBeneficiario['urbano'], $arBeneficiario['total'] ) );			
			}
			
			$yAntes = $obPdf->getY();
			
			$obPdf->SetFont('Arial', 'B', 7);
			$obPdf->cell( 19, 0.4, '6 - Detalhamento da Ação', 0, 0 );
			
			$yDepois = $obPdf->getY();
			$obPdf->setY( $yAntes );	
				
			$obPdf->SetFont('Arial', '', 7);
			$obPdf->PageBreakTrigger = 25;
			$obPdf->multiCell( 19, 0.4, $stDetalhamento, 1 );
			
			$obPdf->setY( $yDepois );	
			
		}
		
	}
	
	function gerarAnexo3( $obPdf, $arAgrupado ){
		
		global $db;
		$_SESSION['nrAnexo'] = 3;
		
		$arDadosCabecalho = $_SESSION['arDadosCabecalho'];
		
		$valorTotalProjeto = 0;
		foreach( $arAgrupado as $stDimensao => $arDimensao ){
			
			unset( $obPdf->aligns );
			$obPdf->AddPage( 'L' ); //adicionando um nova página
			$arFormatacao = array( 'Arial', '', 7, 'B' );
			
			// Terceira Linha
			$arWidth = array( 27.5 );
			$arTitulo[] = '5 - AÇÃO A SER EXECUTADA';
			
			$arDados[] = $stDimensao;
			
			$obPdf->montarRowComTitulo( $arTitulo, $arDados, $arWidth, $arFormatacao );
			unset( $arDados, $arTitulo, $arWidth );

			$arSbaid = array_keys( $arDimensao );
			$stSbaid = "'" . implode( "', '", $arSbaid ). "'";

			// Detalhamento da Subação
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
			$obPdf->cell( 13.5, 0.8, '6.1 - ESPECIFICAÇÃO DA AÇÃO', 1, 0 );
			$obPdf->cell( 5, 0.4, '6.2 - INDICADOR FÍSICO', 1, 0, 'C' );
			$obPdf->cell( 6, 0.4, '6.3 - CUSTO', 1, 1, 'C' );
			
			$obPdf->setX( 17.5 );
			$obPdf->cell( 3, 0.4, 'UNIDADE DE MEDIDA', 1, 0, 'C' );
			$obPdf->cell( 2, 0.4, 'QUANTIDADE', 1, 0, 'C' );
			$obPdf->cell( 3, 0.4, 'VALOR UNITÁRIO', 1, 0, 'C' );
			$obPdf->cell( 3, 0.4, 'VALOR TOTAL', 1, 1, 'C' );
			
			$obPdf->SetFont( 'Arial', '', 7 );
			$valorTotal = 0;
			foreach( $arItens as $arItem ){
				
				$valorTotal += $arItem['valortotal'];
				$valorItem = ( $arItem['valortotal'] / ( $arItem['qtd'] ? $arItem['qtd'] : 1 ) );
				
				$obPdf->SetWidths( array( 3, 13.5, 3, 2, 3, 3 ) );
				$obPdf->SetAligns( array( 'R', 'L', 'L', 'R', 'R', 'R' ) );
				$obPdf->Row( array( $arItem['sbaordem'], $arItem['sbadsc'], $arItem['unddsc'], $arItem['qtd'], tratarValor( $valorItem ), tratarValor( $arItem['valortotal'] ) ) );
			}
			
			$obPdf->cell( 27.5, 0.4, '', 1, 1 );
			
			$valorTotalProjeto += $valorTotal;
			
			$nrValorProponente = tratarValor( $valorTotal * 0.01 );
			$nrValorConcedente = tratarValor( $valorTotal * 0.99 );
			
			$obPdf->SetFont('Arial', 'B', 7);
			$obPdf->cell( 24.5, 0.4, '7 - VALOR TOTAL DA AÇÃO', 1, 0, 'R' );
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
		$obPdf->valorTotalProjeto = $valorTotalProjeto;
	}
	
	function gerarAnexo4( $obPdf, $arAgrupado ){
		
		global $db;
		$_SESSION['nrAnexo'] = 4;
		unset( $obPdf->aligns );
		$obPdf->AddPage(); //adicionando um nova página
		
		$stSbaid = array();
		foreach( $arAgrupado as $stDimensao => $arDimensao ){
			
			$stSbaid[] = "'" . implode( "', '", array_keys( $arDimensao ) ). "'";
		}
		
		$stSbaid = implode( ",", $stSbaid );
		
		// Detalhamento da Subação			
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
		$obPdf->cell( 19, 0.4, '8 - CRONOGRAMA DE EXECUÇÃO', 									1, 1, 'C', 1 );
		
		$obPdf->cell( 4, 0.4, '8.1 INÍCIO - MÊS/ANO', 											1, 0, 'L', 1 );
		$obPdf->SetFont('Arial', '', 7);
		$obPdf->cell( 2, 0.4, $obData->formataData( $arDado[0]['cronogramaexecucaoinicial'] ), 	1, 0, 'L' );
		$obPdf->SetFont('Arial', 'B', 7);
		$obPdf->cell( 4, 0.4, '8.2 TÉRMINO - MÊS/ANO', 											1, 0, 'L', 1 );
		$obPdf->SetFont('Arial', '', 7);
		$obPdf->cell( 2, 0.4, $obData->formataData( $arDado[0]['cronogramaexecucaofinal'] ), 	1, 0, 'L' );
		$obPdf->SetFont('Arial', 'B', 7);
		$obPdf->cell( 5, 0.4, '8.3 - QUANTIDADE DE DIAS', 										1, 0, 'L', 1 );
		$obPdf->SetFont('Arial', '', 7);
		$obPdf->cell( 2, 0.4, $nrQuantidadeDias, 												1, 1, 'L' );
		
		$obPdf->SetFont('Arial', 'B', 7);
		$obPdf->cell( 19, 0.4, '9 - CRONOGRAMA DE DESEMBOLSO -  VALORES CONCEDENTES', 									1, 1, 'L', 1 );
		$obPdf->cell( 19, 0.4, 'MÊS/ANO          VALOR', 											1, 1, 'L', 1 );
		$obPdf->SetFont('Arial', '', 7);
		$obPdf->cell( 19, 0.4, date( 'm/Y' ) . '              ' . $nrValorConcedente,	1, 1, 'L' );
		$obPdf->SetFont('Arial', 'B', 5);
		$obPdf->cell( 16, 0.3, 'VALOR TOTAL A SER DESEMBOLSADO PELO CONCEDENTE', 1, 0, 'L', 1 );
		$obPdf->SetFont('Arial', '', 5);
		$obPdf->cell( 03, 0.3, $nrValorConcedente, 1, 1, 'R', 1 );
		
		$obPdf->SetFont('Arial', 'B', 7);
		$obPdf->cell( 19, 0.4, '10 - CRONOGRAMA DE DESEMBOLSO - VALORES PROPONENTES', 									1, 1, 'L', 1 );
		$obPdf->cell( 19, 0.4, 'MÊS/ANO          VALOR', 											1, 1, 'L', 1 );
		$obPdf->SetFont('Arial', '', 7);
		$obPdf->cell( 19, 0.4, date( 'm/Y' ) . '              ' . $nrValorConcedente,	1, 1, 'L' );
		$obPdf->SetFont('Arial', 'B', 5);
		$obPdf->cell( 16, 0.3, 'VALOR TOTAL A SER DESEMBOLSADO PELO PROPONENTE (valor mínimo de 1%)', 1, 0, 'L', 1 );
		$obPdf->SetFont('Arial', '', 5);
		$obPdf->cell( 03, 0.3, $nrValorProponente, 1, 1, 'R', 1 );
		
		$obPdf->cell( 16, 0.3, 'VALOR TOTAL DO PROJETO', 1, 0, 'L', 1 );
		$obPdf->SetFont('Arial', '', 5);
		$obPdf->cell( 03, 0.3, tratarValor( $obPdf->valorTotalProjeto ), 1, 1, 'R', 1 );
		
		$obPdf->SetFont('Arial', 'B', 7);
	}
	
	function gerarAnexo5( $obPdf, $arAgrupado ){
		
		global $db;
		$_SESSION['nrAnexo'] = 5;
		$arDadosCabecalho = $_SESSION['arDadosCabecalho'];
		$itrid = cte_pegarItrid( $_SESSION['inuid'] );
				
		$obPdf->PageBreakTrigger = 25;
		
		unset( $obPdf->aligns );
		
		foreach( $arAgrupado as $stDimensao => $arDimensao ){
			
			foreach( $arDimensao as $arSubacao ){
			
				$obPdf->AddPage(); //adicionando um nova página
				$arFormatacao = array( 'Arial', '', 7, 'B' );
				
				// Primeira Linha
				$arWidth = array( 3, 16 );
				
				$arTitulo[] = '1 - EXERCÍCIO';
				$arTitulo[] = '2 - AÇÃO A SER EXECUTADA';
				
				
				$arDados[] = $arDadosCabecalho['nrExercicio'];
				$arDados[] = $arSubacao['dimdsc'];
				
				$obPdf->montarRowComTitulo( $arTitulo, $arDados, $arWidth, $arFormatacao );
				unset( $arDados, $arTitulo, $arWidth );
				
				// Segunda Linha
				$arWidth = array( 3, 16 );
				
				$arTitulo[] = '3 - CNPJ';
				$arTitulo[] = '4 - NOME DO ÓRGÃO OU ENTIDADE';
				
				$arDados[] = $arDadosCabecalho['cnpjOrgao'];
				$arDados[] = $arDadosCabecalho['nomeOrgao'];
				
				$obPdf->montarRowComTitulo( $arTitulo, $arDados, $arWidth, $arFormatacao );
				unset( $arDados, $arTitulo, $arWidth );
				
				// Terceira Linha
				$arWidth = array( 17, 2 );
				
				
				$arTitulo[] = $itrid == INSTRUMENTO_DIAGNOSTICO_MUNICIPAL ? '5 - MUNICÍPIO' : '5 - ESTADO';
				$arTitulo[] = '6 - UF';
				
				$arDados[] = $arDadosCabecalho['mundescricao'];
				$arDados[] = $arDadosCabecalho['estuf'];
				
				$obPdf->montarRowComTitulo( $arTitulo, $arDados, $arWidth, $arFormatacao );
				unset( $arDados, $arTitulo, $arWidth );
				
				// Quarta Linha
				$arWidth = array( 19 );
				
				$arTitulo[] = '7 - ESPECIFICAÇÃO DA AÇÃO';
				
				$arDados[] = $arSubacao['sbadsc'];
					
				$obPdf->montarRowComTitulo( $arTitulo, $arDados, $arWidth, $arFormatacao );
				unset( $arDados, $arTitulo, $arWidth );				
				
				// Detalhamento da Subação			
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
				$obPdf->cell( 19, 0.4, '8 - DETALHAMENTO DOS ITENS QUE COMPÕEM A ESPECIFICAÇÃO',	1, 1 );
				
				$obPdf->cell( 6.5, 0.8, '8.1 IDENTIFICAÇÃO DOS ITENS', 								1, 0 );
				$obPdf->cell( 4, 0.8, '8.2 UNIDADE DE MEDIDA', 										1, 0 );
				$obPdf->cell( 2.5, 0.8, '8.3 QUANTIDADE', 											1, 0 );
				$obPdf->cell( 6, 0.4, '8.4 ESTIMATIVA DE CUSTO', 									1, 1, 'C' );
				
				$obPdf->setX( 14 );
				$obPdf->cell( 3, 0.4, 'VALOR UNITÁRIO', 											1, 0, 'C' );
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
			
			$obPdf->AddPage(); //adicionando um nova página
			$arFormatacao = array( 'Arial', '', 7, 'B' );		
			
			foreach( $arDado as $dados ){
				$arEscolas[$dados['entcodent']. '-' .$dados['entnome']][] = $dados;
			}
	
			
			$obPdf->SetFont('Arial', 'B', 7);
			$obPdf->cell( 19, 0.4, '8 - ESCOLAS BENEFICIADAS', 												1, 1 );
			
			foreach( $arEscolas as $entnome => $arEscola ){
				
				$arDadosEscola = explode( '-', $entnome );
				
				$obPdf->SetFont('Arial', 'B', 7);
				$obPdf->cell( 3, 0.4, 'CÓDIGO IBGE', 														1, 0 );
				$obPdf->cell( 16, 0.4, 'ESCOLA', 															1, 1 );
				$obPdf->SetFont('Arial', '', 7);
				$obPdf->cell( 3, 0.4, $arDadosEscola[0], 													1, 0 );
				$obPdf->cell( 16, 0.4, $arDadosEscola[1], 													1, 1 );
				
				$obPdf->SetFont('Arial', 'B', 7);
				$obPdf->cell( 8, 0.4, 'Itens', 																1, 0 );
				$obPdf->cell( 5, 0.4, 'Unidade de Medida', 													1, 0 );
				$obPdf->cell( 2, 0.4, 'Quantidade', 														1, 0 );
				$obPdf->cell( 2, 0.4, 'Preço Unitário', 													1, 0 );
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
	
	function tratarValor( $nrValor ){
		return "R$ " . number_format( round( ( $nrValor ), 2 ) , 2, ",", "." );
	}
	
?>