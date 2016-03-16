<?php 

ini_set('memory_limit','16M');

$width = 'width="100%"';
$bgColor = 'bgcolor="silver"';

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

require_once( APPRAIZ . 'includes/classes/dateTime.inc');
include_once APPRAIZ . "includes/classes/fileSimec.class.inc";

	$prpid = $_REQUEST['prpid'];

	$sql = "SELECT 
				ems.sbaid,
				prpanoconveniofnde 
			FROM 
				par.processopar prp
			INNER JOIN par.empenho 		  emp ON emp.empnumeroprocesso = prpnumeroprocesso 
			INNER JOIN par.empenhosubacao ems ON ems.empid = emp.empid and eobstatus = 'A' and empstatus = 'A'
			WHERE 
				prp.prpid = ".$prpid;
	
	
	$arDados = $db->carregar( $sql );
	$arDados = $arDados ? $arDados : array();

	$ano = '0';
	$arSbaid = array();
	foreach( $arDados as $sbaid ){
		$ano = $sbaid['prpanoconveniofnde'];
		$arSbaid[] = $sbaid['sbaid'];
	}
	
	if( count($arSbaid) == 0 ){
		echo '<script>alert("Faltam dados.");
				history.back(-1);</script>';
		die();
	}
	
	gerarPdfPta( $arSbaid, $stJustificativa, $ano, $width, $bgColor, $dados );
	
	function gerarPdfPta( $idsSubacao, $stJustificativa, $ano, $width, $bgColor, $dados ){
		
		global $db;
		$itrid = $_SESSION['par']['itrid'];
		
		$stSbaid = "'" . implode( "', '", array_unique( $idsSubacao ) ). "'";
		
		if( $itrid == 1 ){
			
			$sql = "SELECT 
						e.estdescricao as mundescricao, 
						e.estuf,
						s.sbaid, 
						d.dimid, 
						d.dimcod, 
						are.arecod, 
						i.indcod, 
						d.dimdsc, 
						are.aredsc, 
						s.sbaordem, 
						s.sbadsc, 
						u.unddsc, 
						i.inddsc,
						sbd.*, 
						CASE WHEN s.sbacronograma = 2 THEN 't' ELSE 'f' END as sbaporescola
					FROM par.subacao s
						inner join par.subacaodetalhe sbd on sbd.sbaid = s.sbaid
						inner join par.unidademedida u on u.undid = s.undid
						inner join par.acao a on a.aciid = s.aciid
						inner join par.pontuacao p on p.ptoid = a.ptoid
						inner join par.instrumentounidade iu on iu.inuid = p.inuid	
						inner join territorios.estado e on e.estuf = iu.estuf
						inner join par.criterio c on c.crtid = p.crtid
						inner join par.indicador i on c.indid = i.indid
						inner join par.area are on are.areid = i.areid
						inner join par.dimensao d on d.dimid = are.dimid	
					WHERE
						s.sbaid in ( $stSbaid )
						AND p.ptostatus = 'A'
						AND sbdano = $ano
					ORDER BY
						d.dimcod, are.arecod, i.indcod, s.sbaordem";
			
		} elseif( $itrid == 2 ){
			
			$sql = "SELECT 
						m.mundescricao, 
						m.estuf,
						s.sbaid, 
						d.dimid, 
						d.dimcod, 
						are.arecod, 
						i.indcod, 
						d.dimdsc, 
						are.aredsc, 
						s.sbaordem, 
						s.sbadsc, 
						u.unddsc, 
						i.inddsc,
						sbd.*, 
						CASE WHEN s.sbacronograma = 2 THEN 't' ELSE 'f' END as sbaporescola
					FROM par.subacao s
						inner join par.subacaodetalhe sbd on sbd.sbaid = s.sbaid
						inner join par.unidademedida u on u.undid = s.undid
						inner join par.acao a on a.aciid = s.aciid
						inner join par.pontuacao p on p.ptoid = a.ptoid
						inner join par.instrumentounidade iu on iu.inuid = p.inuid	
						inner join territorios.municipio m on m.muncod = iu.muncod
						inner join par.criterio c on c.crtid = p.crtid
						inner join par.indicador i on c.indid = i.indid
						inner join par.area are on are.areid = i.areid
						inner join par.dimensao d on d.dimid = are.dimid	
					WHERE
						s.sbaid in ( $stSbaid )
						AND p.ptostatus IN ('A', 'E')
						AND sbdano = $ano
					ORDER BY
						d.dimcod, are.arecod, i.indcod, s.sbaordem";
		}
		
		$arDados = $db->carregar( $sql );

		if( is_array( $arDados ) ){

			if( $itrid == 1 ){
				$sql = "SELECT
							to_char(to_number(ent.entnumcpfcnpj,'000'), '00\".\"000\".\"000\"/\"0000\"-\"00') as cnpjorgao,
							est.estdescricao as mundescricao,
							e.entnome as dirigente,
							ent.entnome as nomeorgao
						FROM
							par.entidade e
						INNER JOIN par.instrumentounidade iu ON iu.inuid = e.inuid
						INNER JOIN territorios.estado est ON est.estuf = iu.estuf
						INNER JOIN par.entidade ent ON ent.inuid = e.inuid AND ent.dutid = ".DUTID_SECRETARIA_ESTADUAL."
						WHERE 
							iu.inuid = {$_SESSION['par']['inuid']} AND e.dutid = ".DUTID_SECRETARIO_ESTADUAL;
				/*
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
							inner join par.instrumentounidade iu on (iu.estuf = est.estuf)
						WHERE fe.funid = 25 
						AND iu.inuid = {$_SESSION['par']['inuid']}";
				*/				
			}
			elseif( $itrid == 2 ){
				
				$sql = "SELECT
							to_char(to_number(ent.entnumcpfcnpj,'000'), '00\".\"000\".\"000\"/\"0000\"-\"00') as cnpjorgao,
							m.mundescricao,
							e.entnome as dirigente,
							ent.entnome as nomeorgao
						FROM
							par.entidade e
						INNER JOIN par.instrumentounidade iu ON iu.inuid = e.inuid
						INNER JOIN territorios.municipio m ON m.muncod = iu.muncod
						INNER JOIN par.entidade ent ON ent.inuid = e.inuid AND ent.dutid = ".DUTID_PREFEITURA."
						WHERE 
							e.inuid = {$_SESSION['par']['inuid']} AND e.dutid = ".DUTID_PREFEITO;
				/*
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
							inner join par.instrumentounidade iu on (iu.muncod = m.muncod)
						WHERE  fe.funid = 2 
						AND iu.inuid = {$_SESSION['par']['inuid']}";
				*/			
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
					$arDadosCabecalho['sbdano'] = $arSubacao['sbdano'];
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
			$dados .= gerarAnexo1( $width, $bgColor, $arAgrupado ); 
			$dados .= '</div>';
			$dados .= '<div class="folha">'; 
			$dados .= gerarAnexo2( $width, $bgColor, $arAgrupado ); 
			$dados .= '</div>';
			$dados .= '<div class="folha">';
			$dados .= gerarAnexo3( $width, $bgColor, $arAgrupado ); 
			$dados .= '</div>';
			$dados .= '<div class="folha">';
			$dados .= gerarAnexo4( $width, $bgColor, $arAgrupado ); 
			$dados .= '</div>'; 
			$dados .= '<div class="folha">';
			$dados .= gerarAnexo5( $width, $bgColor, $arAgrupado ); 
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
	
	function gerarAnexo1( $width, $bgColor, $arAgrupado ){
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
						&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;'.$dadosCabecalho['sbdano'].'</td>
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
					<td colspan="2"><b>6 - MUNIC�PIO</b><BR>
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
						<BR><BR>
						&nbsp;&nbsp;&nbsp;&nbsp; A Organiza��o das Na��es Unidas para a Educa��o, a Ci�ncia e a Cultura (UNESCO) reconhece a educa��o como um fator determinante no desenvolvimento socioecon�mico de qualquer na��o. A complexa realidade do Brasil, por sua dimens�o continental e sua diversidade cultural, dificulta a elabora��o e execu��o de pol�ticas p�blicas condizentes com realidade de cada local sem a efetiva colabora��o de todos os entes federados.
						<BR><BR>
						A Lei de Diretrizes e Bases da Educa��o - Lei n� 9.394, de 1996 -, define e regulariza o sistema de educa��o brasileiro. Em seu artigo 9�, inciso III, determina a Uni�o "prestar assist�ncia t�cnica e financeira aos Estados, ao Distrito Federal e aos Munic�pios para o desenvolvimento de seus sistemas de ensino e o atendimento priorit�rio � escolaridade obrigat�ria, exercendo sua fun��o redistributiva e supletiva".
						<BR><BR>
						Nesse contexto, consoante disposto no Decreto n� 6.768, de 2009, que disciplina o Programa Caminho da Escola, a Uni�o, por interm�dio do Minist�rio da Educa��o, apoiar� os entes federativos na aquisi��o de ve�culos para transporte dos estudantes, preferencialmente, da zona rural. 
						<BR><BR>
						O Programa Caminho da Escola compreende a aquisi��o, por meio de ades�o � ata de preg�o eletr�nico, para registro de pre�os - disciplinado pelo FNDE -, de ve�culos padronizados para o transporte escolar. O servi�o, apontado, dentre outros objetivos, como fundamental para o acesso e a perman�ncia dos estudantes moradores da zona rural nas escolas p�blicas da educa��o b�sica, reduzindo a evas�o escolar, em observ�ncia �s metas do Plano Nacional de Educa��o, serve para deslocar o aluno de sua resid�ncia ou local espec�fico previamente acordado at� a institui��o de ensino. Em fun��o do n�mero de alunos desta municipalidade que residem na zona rural e que utilizam o transporte escolar entendemos como fundamental a assist�ncia financeira da uni�o para aquisi��o de ve�culos escolares, como forma de contribuir para o acesso e perman�ncia dos alunos do campo nas escolas e reduzir os �ndices de evas�o escolar.
						<BR><BR>
					</td>
				</tr>
			</table>
			'.footer($width, $bgColor).'				
			</td>
			</tr>
		</table>';
		return $html;		
	}

	function gerarAnexo2( $width, $bgColor, $arAgrupado ){

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
									
								$stDetalhamento .= "<BR><BR>Dimens�o: {$arSubacao['dimcod']} - {$arSubacao['dimdsc']}<BR>";
								$stDetalhamento .= "�rea: {$arSubacao['dimcod']}.{$arSubacao['arecod']} - {$arSubacao['aredsc']}<BR>";
								$stDetalhamento .= "Indicador: {$arSubacao['dimcod']}.{$arSubacao['arecod']}.{$arSubacao['indcod']} - {$arSubacao['inddsc']}<BR>";
								$stDetalhamento .= "Suba��o: {$arSubacao['dimcod']}.{$arSubacao['arecod']}.{$arSubacao['indcod']} ({$arSubacao['sbaordem']}) - {$arSubacao['sbadsc']}<BR>";
								$stDetalhamento .= "Unidade de Medida: {$arSubacao['unddsc']}<BR>";
								$stDetalhamento .= "\n{$arSubacao['sptuntdsc']}<BR><BR>";
									
							}
							$arSbaid = array_keys( $arDimensao );
							$stSbaid = "'" . implode( "', '", $arSbaid ). "'";
							
							// Quando � Onibus pequeno entao sao 23 alunos, medio 31 e grande 44. Ele soma todos e coloca como beneficiario o aluno.
							$sql = "SELECT sum(
										CASE WHEN
											picid = 362 OR picid = 359 OR picid = 361
										THEN
											23
										WHEN
											picid = 363 OR picid = 364
										THEN
											31
										WHEN
											picid = 365 OR picid = 366
										THEN
											44
										END ) as qtd
									FROM 
										par.subacao s
									INNER JOIN par.subacaoitenscomposicao sic ON sic.sbaid = s.sbaid
									WHERE 
										s.sbaid IN ( $stSbaid )
										AND icoano = {$dadosCabecalho['sbdano']}";
							
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
							<td align="center">'.$arBeneficiario['qtd'].'</td>
							<td align="center"></td>
							<td align="center">'.$arBeneficiario['qtd'].'</td>
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
	
	function gerarAnexo3( $width, $bgColor, $arAgrupado ){
		
		global $db;
		$_SESSION['nrAnexo'] = 3;
		
		$dadosCabecalho = $_SESSION['arDadosCabecalho'];
		
		$valorTotalProjeto = 0;
		
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
							&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;'.$dadosCabecalho['sbdano'].'</td>
						<td><b>2 - NOME DO �RG�O OU ENTIDADE</b><br>
						&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;'.$dadosCabecalho['nomeOrgao'].'</td>
						<td><b>3 - '.($itrid == 2 ? "MUNIC�PIO" : "ESTADO").'</b><BR>
						&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;'.$dadosCabecalho['mundescricao'].'</td>
						<td><b>4 - DF</b><BR>
						&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;'.$dadosCabecalho['estuf'].'</td>
					</tr>';
					foreach( $arAgrupado as $stDimensao => $arDimensao ){
						$html .= '<tr>
							<td colspan="4"><b>5 - A��O A SER EXECUTADA</b><BR>
								&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;'.implode( "\n", $dadosCabecalho['dimcod'] ).'</td>
						</tr>';
						
						$arSbaid = array_keys( $arDimensao );
						$stSbaid = "'" . implode( "', '", $arSbaid ). "'";
						
						$sql = "SELECT 
									sbaordem, sbadsc,
									CASE WHEN s.sbacronograma = 1
									THEN
										( SELECT sum(coalesce(sic.icoquantidade,0))::numeric as qtd
								                        FROM par.subacaoitenscomposicao sic 
								                        WHERE sic.sbaid = s.sbaid
								                        AND sic.icoano = sd.sbdano
								                        GROUP BY sic.sbaid )
									ELSE
										(SELECT sum(coalesce(se.sesquantidade,0))::numeric as qtd
								                        FROM par.subacaoitenscomposicao sic 
								                        INNER JOIN par.subacaoescolas se ON se.sbaid = sic.sbaid AND se.sesano = sic.icoano
								                        WHERE sic.sbaid = s.sbaid AND sic.icoano = sd.sbdano
								                        GROUP BY sic.sbaid )
									END as qtd,
									CASE WHEN s.sbacronograma = 1
									THEN
										( SELECT sum(coalesce(sic.icoquantidade,0) * coalesce(sic.icovalor,0))/sum(coalesce(sic.icoquantidade,0))::numeric as valor
								                        FROM par.subacaoitenscomposicao sic 
								                        WHERE sic.sbaid = s.sbaid
								                        AND sic.icoano = sd.sbdano
								                        GROUP BY sic.sbaid )
									ELSE
										(SELECT sum(coalesce(se.sesquantidade,0) * coalesce(sic.icovalor,0))/sum(coalesce(se.sesquantidade,0))::numeric as valor
								                        FROM par.subacaoitenscomposicao sic 
								                        INNER JOIN par.subacaoescolas se ON se.sbaid = sic.sbaid AND se.sesano = sic.icoano
								                        WHERE sic.sbaid = s.sbaid AND sic.icoano = sd.sbdano
								                        GROUP BY sic.sbaid )
									END as valor,
									CASE WHEN s.sbacronograma = 1
									THEN ( SELECT sum(coalesce(sic.icoquantidade,0) * coalesce(sic.icovalor,0))::numeric as vlrsubacao
										FROM par.subacaoitenscomposicao sic 
										WHERE sic.sbaid = s.sbaid
										AND sic.icoano = sd.sbdano
										GROUP BY sic.sbaid )
									ELSE ( SELECT sum(coalesce(se.sesquantidade,0) * coalesce(sic.icovalor,0))::numeric as vlrsubacao
										FROM par.subacaoitenscomposicao sic 
										INNER JOIN par.subacaoescolas se ON se.sbaid = sic.sbaid AND se.sesano = sic.icoano
										WHERE sic.sbaid = s.sbaid AND sic.icoano = sd.sbdano
										GROUP BY sic.sbaid ) 
									END AS totalprojeto
								FROM par.subacao s
								INNER JOIN par.subacaodetalhe sd ON sd.sbaid = s.sbaid AND sbdano = {$dadosCabecalho['sbdano']}
								WHERE s.sbaid IN ($stSbaid)";
						
						$arItens = $db->carregar( $sql );
						$arItens = $arItens ? $arItens : array();

						$html .= '<tr>
							<td rowspan="2"><b>6 - ORDEM</b></td>
							<td rowspan="2"><b>6.1 - ESPECIFICA��O DA A��O</b></td>
							<td align="center"><b>6.2 - INDICADOR F�SICO</b></td>
							<td align="center"><b>6.3 - CUSTO</b></td>
						</tr>
						<tr>
							<td align="center"><b>QUANTIDADE</b></td>
							<td align="center"><b>VALOR TOTAL</b></td>
						</tr>';
						foreach( $arItens as $arItem ){ 
							$valorTotal += $arItem['totalprojeto'];
							$html .='<tr>
								<td align="right">'.$arItem['sbaordem'].'</td>
								<td>'.$arItem['sbadsc'].'</td>
								<td align="right">'.$arItem['qtd'].'</td>
								<td align="right">'.tratarValor( $arItem['totalprojeto'] ).'</td>
							</tr>';
						} 

						$_SESSION['par']['pta']['valortotal'] = $valorTotal;
 						
						$nrValorProponente = tratarValor( $valorTotal * 0.01 );
						$nrValorConcedente = tratarValor( $valorTotal * 0.99 );
						
						$html.='<tr>
							<td colspan="4">&nbsp;</td>
						</tr>
						<tr>
							<td colspan="3" align="right"><b>7 - VALOR TOTAL DA A��O</b></td>
							<td align="right">'.tratarValor( $valorTotal ).'</td>
						</tr>
						<tr>
							<td colspan="3" align="right"><b>7 - VALOR TOTAL DO PROPONENTE</b></td>
							<td align="right">'.$nrValorProponente.'</td>
						</tr>
						<tr>
							<td colspan="3" align="right"><b>7 - VALOR TOTAL DO CONCEDENTE</b></td>
							<td align="right">'.$nrValorConcedente.'</td>
						</tr>';
						
					}
				$html.='</table>
				'.footer($width, $bgColor).'				
				</td>
			</tr>
		</table>';
		return $html;
	}
	
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
						&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;'.$dadosCabecalho['sbdano'].'</td>
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
			$sql = "select 
						'01/' || sbdinicio || '/' || sbdano AS cronogramaexecucaoinicial, 
						'/' || sbdfim || '/' || CASE WHEN sbdanotermino IS NULL THEN sbdano ELSE sbdanotermino END AS cronogramaexecucaofinal,
						sbdfim as mes
					from 
						par.subacao s
					inner join par.subacaodetalhe sbd on sbd.sbaid = s.sbaid
					where s.sbaid in ( $stSbaid )";
			
			$arDado = $db->carregar( $sql );
			$arDado = $arDado ? $arDado : array();
			
			$meses = array(
							1 => 31,
							2 => 28,
							3 => 31,
							4 => 30,
							5 => 31,
							6 => 30,
							7 => 31,
							8 => 31,
							9 => 30,
							10 => 31,
							11 => 30,
							12 => 31
					);
			
			$termino = $meses[$arDado[0]['mes']].$arDado[0]['cronogramaexecucaofinal'];
			$obData = new Data();
			$nrQuantidadeDias = $obData->quantidadeDeDiasEntreDuasDatas( $arDado[0]['cronogramaexecucaoinicial'], $termino );

			$valorTotal = $_SESSION['par']['pta']['valortotal'];
			
			$nrValorProponente = tratarValor( $valorTotal * 0.01 );
			$nrValorConcedente = tratarValor( $valorTotal * 0.99 );
			
			$html.='<table '.$width.' class="lista" align="center" '.$bgColor.' cellspacing="1" cellpadding="4">
				<tr>
					<td align="center" colspan="6"><b>9 - CRONOGRAMA DE EXECU��O</b></td>
				</tr>
				<tr>
					<td><b>9.1 IN�CIO - M�S/ANO</b></td>
					<td bgcolor="white" style="width: 10%;">'.$arDado[0]['cronogramaexecucaoinicial'].'</td>
					<td><b>9.2 T�RMINO - M�S/ANO</b></td>
					<td bgcolor="white" style="width: 10%;">'.$termino.'</td>
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
	
	function gerarAnexo5( $width, $bgColor, $arAgrupado ){
		
		global $db;
		$_SESSION['nrAnexo'] = 5;
		$dadosCabecalho = $_SESSION['arDadosCabecalho'];
		$itrid = $_SESSION['par']['itrid'];
		
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
							5</td>
						</tr>
					</table>
					<table '.$width.' class="lista" align="center" cellspacing="1" cellpadding="4">
						<tr>
							<td valign="top" style="text-align: center;">Utilize quantos formul�rios forem necess�rios para a complementa��o deste anexo.</td>
						</tr>
					</table>
					<table '.$width.' class="lista" align="center" cellspacing="1" cellpadding="4">';
						foreach( $arAgrupado as $stDimensao => $arDimensao ){ 
							foreach( $arDimensao as $arSubacao ){
						$html.='<tr>
							<td><b>1 - EXERC�CIO</b><BR>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;'.$dadosCabecalho['sbdano'].'</td>
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
						</tr>
						<tr>
							<td colspan="5"><b>7 - ESPECIFICA��O DA A��O</b><BR>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;'.$arSubacao['sbadsc'].'</td>
						</tr>
						<tr>
							<td colspan="5">8.1 IDENTIFICA��O DOS ITENS8 - DETALHAMENTO DOS ITENS QUE COMP�EM A ESPECIFICA��O</td>
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
							$sql = "SELECT 
										sic.icodescricao, u.umidescricao, sic.icoquantidade, sic.icovalor, ( sic.icoquantidade * sic.icovalor ) as valortotal
									FROM 
										par.subacao s
									INNER JOIN par.subacaoitenscomposicao sic 		ON sic.sbaid = s.sbaid
									INNER JOIN par.unidademedidadetalhamentoitem u 	ON u.umiid = sic.unddid
									WHERE 
										s.sbaid = {$arSubacao['sbaid']}
										AND icoano = {$dadosCabecalho['sbdano']}
									ORDER by
										u.umidescricao";
							
							$arDado = $db->carregar( $sql );
							$arDado = $arDado ? $arDado : array();
							
							foreach( $arDado as $count => $arItem ){
					
								$valorTotal += $arItem['valortotal'];
								
								$html.='<tr>
											<td>'.( $count +1 ) . ' - ' . $arItem['icodescricao'].'</td>
											<td>'.$arItem['umidescricao'].'</td>
											<td>'.round( $arItem['icoquantidade'] ).'</td>
											<td>'.tratarValor( $arItem['icovalor'] ).'</td>
											<td>'.tratarValor( $arItem['valortotal'] ).'</td>
										</tr>';
							}
							
							$html.='<tr bgcolor="silver">
								<td colspan="4">TOTAL DESTE ANEXO</td>
								<td>'.tratarValor( $valorTotal ).'</td>
							</tr>';
						} 
					}
					$html.='</table>
				</td>
			</tr>
		</table>
		'.footer($width, $bgColor);
		return $html;
	}
	
	function tratarValor( $nrValor ){
		return "R$ " . number_format( round( ( $nrValor ), 2 ) , 2, ",", "." );
	}
	
?>