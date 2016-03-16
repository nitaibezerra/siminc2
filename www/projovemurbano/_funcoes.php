<?php
function buscarEscolaPorINEP($dados) {
	global $db;
	$sql = "SELECT
				ent.entid, ent.entnome, tpc.tpcdesc, tpl.tpldesc, ent.entnumdddcomercial, ent.entnumcomercial
			FROM entidade.entidade ent
			INNER JOIN	educacenso_2014.tab_entidade tent ON ent.entcodent::int = tent.pk_cod_entidade
			LEFT JOIN entidade.tipoclassificacao tpc ON tpc.tpcid=ent.tpcid
			LEFT JOIN entidade.tipolocalizacao tpl ON tpl.tplid=ent.tplid
			WHERE entcodent='" . $dados ['codinep'] . "'";
	$entidade = $db->pegaLinha ( $sql );

	if ($entidade ['entid']) {

		$sql = "SELECT ende.endlog, ende.endnum, ende.endcom, ende.endbai, ende.endcep, mun.mundescricao, mun.estuf FROM entidade.endereco ende
				INNER JOIN territorios.municipio mun ON mun.muncod = ende.muncod
				WHERE entid='" . $entidade ['entid'] . "' AND tpeid='1'";
		$entidade ['endereco'] = $db->pegaLinha ( $sql );
	}

	echo simec_json_encode ( $entidade );
}
function cancelarAdesao() {
	global $db;
	
	$sql = "UPDATE projovemurbano.projovemurbano
	SET pjustatus = 'I'
	WHERE pjuid = {$_REQUEST['pjuid']};	";
	$docid = $db->pegaUm ( $sql );
	$db->commit ();
	
	echo "<script>
			alert('Adesão cancelada com sucesso!');
    			window.location.href = window.location.href
		  </script>";
	die ();
}
function reativarAdesao() {
	global $db;
	
	$sql = "UPDATE projovemurbano.projovemurbano
	SET pjustatus = 'A'
	WHERE pjuid = {$_REQUEST['pjuid']};	";
	$docid = $db->pegaUm ( $sql );
	$db->commit ();
	
	echo "<script>
				alert('Adesão reativada com sucesso!');
	    			window.location.href = window.location.href
			  </script>";
	die ();
}
function checkAno() {
	if (! $_SESSION ['projovemurbano'] ['ppuid'])
		die ( "
            <script>
                alert('Selecione primeiro o ano do projeto!');
                window.location='projovemurbano.php?modulo=inicio&acao=C';
            </script>" );
}
function pegarenderecoPorCEP($dados) {
	global $db;
	
	include_once APPRAIZ . "includes/classes/EnderecoCEP.class.inc";
	
	$cp = str_replace ( array (
			'.',
			'-' 
	), '', $_REQUEST ['endcep'] );
	
	$endereco = new enderecoCEP ( $cp );
// 	ver($endereco->tipo_logradouro." ".$endereco->no_logradouro,d);
	echo $endereco->tipo_logradouro." ".$endereco->no_logradouro . "||" . $endereco->no_bairro . "||" . $endereco->co_municipio . "||" . $endereco->sg_uf . "||" . $endereco->co_ibge;
	
	exit ();
}
function validacaoCompletaPlanoImplementacao2013() {
	global $db, $retornarTotalMaximoDemaisAcoes;
	
	$msg = '';
	$tpridTemp = '';
	$max = 4;
	if (isset ( $_SESSION ['projovemurbano'] ['tprid'] )) {
		$tpridTemp = $_SESSION ['projovemurbano'] ['tprid'];
		$max = 2;
	}
	
	for($x = 1; $x < $max; $x ++) {
		
		if ($tpridTemp != '') {
			$_SESSION ['projovemurbano'] ['tprid'] = $tpridTemp;
		} else {
			$_SESSION ['projovemurbano'] ['tprid'] = $x;
		}
		$sql = "SELECT
				tprdesc
			FROM
				projovemurbano.tipoprograma
			WHERE
				tprid = {$_SESSION['projovemurbano']['tprid']}";
		
		$tprdesc = $db->pegaUm ( $sql );
		
		if ($_SESSION ['projovemurbano'] ['muncod']) {
			$sugestaoampliacao = $db->pegaLinha ( "SELECT suaverdade, suametaajustada FROM projovemurbano.sugestaoampliacao WHERE pjuid='" . $_SESSION ['projovemurbano'] ['pjuid'] . "'" );
			$meta = $db->pegaUm ( "SELECT
									mtpvalor as valor,
									mtp.tpmid as tipo
								FROM
									projovemurbano.metasdoprograma mtp
								INNER JOIN projovemurbano.tipometadoprograma tpr ON tpr.tpmid = mtp.tpmid
								WHERE
									pjuid = {$_SESSION['projovemurbano']['pjuid']}
									AND tprid = {$_SESSION['projovemurbano']['tprid']}
								ORDER BY
									tipo DESC" );
			// if( $meta < 1 ){
			// return '';
			// }
			if ($sugestaoampliacao ['suaverdade'] == "t") {
				if ($sugestaoampliacao ['suametaajustada'])
					$meta = $sugestaoampliacao ['suametaajustada'];
			}
		}
		
		if ($_SESSION ['projovemurbano'] ['estuf']) {
			$sugestaoampliacao = $db->pegaLinha ( "SELECT suaverdade, suametaajustada FROM projovemurbano.sugestaoampliacao WHERE pjuid='" . $_SESSION ['projovemurbano'] ['pjuid'] . "'" );
			$sql = "SELECT 
						mtpvalor as valor,
						mtp.tpmid as tipo
					FROM
						projovemurbano.metasdoprograma mtp
					INNER JOIN projovemurbano.tipometadoprograma tpr ON tpr.tpmid = mtp.tpmid
					WHERE
						pjuid = {$_SESSION['projovemurbano']['pjuid']}
						AND tprid = {$_SESSION['projovemurbano']['tprid']}
					ORDER BY
						tipo DESC ";
			$meta = $db->pegaUm ( $sql );
			
			// if( $meta < 1 ){
			// return '';
			// }
			if ($sugestaoampliacao ['suaverdade'] == "t") {
				if ($sugestaoampliacao ['suametaajustada'])
					$meta = $sugestaoampliacao ['suametaajustada'];
			}
		}
		
		$meta = carregarMeta ( $sugestaoampliacao );
		if ($meta < 1) {
			continue;
		}
		/*
		 * VALIDANDO NÚMERO DE NÚCLEOS Se o núcleo for igual a 1 (um), o nº de alunos deve ser necessariamente 200. Se o nº de núcleo for maior que 1 (um), o nº de alunos no núcleo poderá variar entre 150 a 200.
		 */
		
		$sql = "SELECT pmupossuipolo FROM projovemurbano.polomunicipio WHERE pjuid='" . $_SESSION ['projovemurbano'] ['pjuid'] . "'";
		$pmupossuipolo = $db->pegaUm ( $sql );
		
		if ($pmupossuipolo == "t") {
			
			$nucleos = $db->carregar ( "SELECT mun.munid, nuc.nucid, nuc.nucqtdestudantes FROM projovemurbano.nucleo nuc
									INNER JOIN projovemurbano.municipio mun ON mun.munid = nuc.munid
									INNER JOIN projovemurbano.associamucipiopolo amp ON amp.munid = mun.munid
									INNER JOIN projovemurbano.polo pol ON pol.polid = amp.polid
									INNER JOIN projovemurbano.polomunicipio plm ON plm.pmuid = pol.pmuid
									WHERE 
										nuc.nucstatus='A' 
										AND mun.munstatus='A' 
										AND plm.pmustatus='A' 
										AND pol.polstatus='A' 
										AND pjuid='" . $_SESSION ['projovemurbano'] ['pjuid'] . "'
										AND nuc.tprid = {$_SESSION['projovemurbano']['tprid']}" );
		} else {
			
			$nucleos = $db->carregar ( "SELECT mun.munid, nuc.nucid, nuc.nucqtdestudantes FROM projovemurbano.nucleo nuc
									INNER JOIN projovemurbano.municipio mun ON mun.munid = nuc.munid
									INNER JOIN projovemurbano.polomunicipio plm ON plm.pmuid = mun.pmuid
									WHERE 
										nuc.nucstatus='A' 
										AND mun.munstatus='A' 
										AND plm.pmustatus='A' 
										AND pjuid='" . $_SESSION ['projovemurbano'] ['pjuid'] . "'
										AND nuc.tprid = {$_SESSION['projovemurbano']['tprid']}" );
		}
		
		$totalestudantes = 0;
		if ($nucleos [0]) {
			$_CHK = Array ();
			foreach ( $nucleos as $nucleo ) {
				$_CHK [$nucleo ['munid']] [$nucleo ['nucid']] = $nucleo ['nucqtdestudantes'];
				$totalestudantes += $nucleo ['nucqtdestudantes'];
			}
		} else {
			$msg [] = "<br>" . $tprdesc . ": Não existe núcleo não cadastrado.";
		}
		
		if ($totalestudantes != $meta) {
			$msg [] = "<br>" . $tprdesc . ": Quantidade de estudantes/Núcleo diferente da meta (Meta:" . $meta . ",Utilizado:" . $totalestudantes . ").";
		}
		
		if ($_CHK) {
			foreach ( array_keys ( $_CHK ) as $munid ) {
				if (count ( $_CHK [$munid] ) == 1) {
					$qtde = current ( $_CHK [$munid] );
					if ($qtde != '200' && $_SESSION ['projovemurbano'] ['tprid'] != 2) {
						$msg [] = "<br>" . $tprdesc . ": Se o núcleo for igual a 1 (um), o nº de alunos deve ser necessariamente 200.";
					}
					if ($qtde < '60' && $qtde > '150' && $_SESSION ['projovemurbano'] ['tprid'] == 2) {
						$msg [] = "<br>" . $tprdesc . ": Se o núcleo for igual a 1 (um), o nº de alunos deve ser necessariamente entre 60 e 150.";
					}
				}
			}
		}
		
		/* FIM validando número de núcleos */
		$montante = calcularMontante ( $meta );
		
		/*
		 * VALIDANDO PROFISSIONAIS - verifica se a aba profissionais foi gravado pelo menos uma vez; - verifica se o valor total de profissionais é maior que o percentual previsto; - atualiza o percentual utilizado (caso tenha ocorrido alguma falha)
		 */
		
		criaSessaoProfissionais ();
		
		unset ( $dirassistentes );
		unset ( $casoprisionais2 );
		unset ( $coordgera );
		unset ( $assitente_A2 );
		
		$profissionais = $db->pegaLinha ( "SELECT * FROM projovemurbano.profissionais WHERE proid = " . $_SESSION ['projovemurbano'] ['proid'] . " " );
		
		$_SESSION ['projovemurbano'] ['proid'] = $profissionais ['proid'];
		$proid = $_SESSION ['projovemurbano'] ['proid'];
		$coordgeral = pegarCoordenadorGeral ( $_SESSION ['projovemurbano'] ['proid'] );
		$assistenteadministrativo_A = pegarAssistentesCoordenadorGeral ( $_SESSION ['projovemurbano'] ['proid'], 'A' );
		$assistenteadministrativo_P = pegarAssistentesCoordenadorGeral ( $_SESSION ['projovemurbano'] ['proid'], 'P' );
		$assistenteadministrativo_M = pegarAssistentesCoordenadorGeral ( $_SESSION ['projovemurbano'] ['proid'], 'M', $_SESSION ['projovemurbano'] ['tprid'] );
		$assistenteadministrativo_AP = pegarAssistentesCoordenadorGeral ( $_SESSION ['projovemurbano'] ['proid'], 'AP', $_SESSION ['projovemurbano'] ['tprid'] );
		$assistenteadministrativo_PP = pegarAssistentesCoordenadorGeral ( $_SESSION ['projovemurbano'] ['proid'], 'PP', $_SESSION ['projovemurbano'] ['tprid'] );
		
		if ($_SESSION ['projovemurbano'] ['tprid'] == 2) {
			$casoprisionais2 = $assistenteadministrativo_AP ['coavlrtotal'] + $assistenteadministrativo_PP ['coavlrtotal'];
		}
		if ($coordgeral ['tprid'] == $_SESSION ['projovemurbano'] ['tprid'] && $coordgeral ['tprid'] != '') {
			$coordgera = $coordgeral ['cgevlrtotal'];
		}
		
		if ($coordgeral ['tprid'] == $_SESSION ['projovemurbano'] ['tprid'] && $coordgeral ['tprid'] != '') {
			$assitente_A2 = $assistenteadministrativo_A ['coavlrtotal'] + $assistenteadministrativo_P ['coavlrtotal'];
		}
		
		$coordassistentes = array (
				"coaqtd" => $assitente_A1 + $assistenteadministrativo_M ['coaqtd'] + $casoprisionais1,
				"coavlrtotal" => $assitente_A2 + $assistenteadministrativo_M ['coavlrtotal'] + $casoprisionais2 
		);
		
		$diretorpolo = pegarDiretorPolo ( $_SESSION ['projovemurbano'] ['proid'] );
		$dirassistentes_A = pegarAssistentesDiretorPolo ( $_SESSION ['projovemurbano'] ['proid'], 'A' );
		$dirassistentes_P = pegarAssistentesDiretorPolo ( $_SESSION ['projovemurbano'] ['proid'], 'P' );
		$diretorpol = '';
		if ($coordgeral ['tprid'] == $_SESSION ['projovemurbano'] ['tprid'] && $coordgeral ['tprid'] != '') {
			$diretorpol = pegarDiretorPolo ( $_SESSION ['projovemurbano'] ['proid'] );
		}
		// dbg( $coordgeral['tprid'] == $_SESSION['projovemurbano']['tprid'] && $coordgeral['tprid'] != '');
		
		if ($coordgeral ['tprid'] == $_SESSION ['projovemurbano'] ['tprid'] && $coordgeral ['tprid'] != '') {
			$dirassistentes = array (
					"dasqtdefetivo40hr" => $dirassistentes_A ['dasqtdefetivo40hr'] + $dirassistentes_P ['dasqtdefetivo40hr'],
					"dasqtdrecursoproprio" => $dirassistentes_A ['dasqtdrecursoproprio'] + $dirassistentes_P ['dasqtdrecursoproprio'],
					"creqtd" => $dirassistentes_A ['creqtd'] + $dirassistentes_P ['creqtd'],
					"crevlrtotal" => $dirassistentes_A ['crevlrtotal'] + $dirassistentes_P ['crevlrtotal'] 
			);
		}
		
		$educadores_F = pegarEducadores ( $_SESSION ['projovemurbano'] ['proid'], 'F' );
		$educadores_Q = pegarEducadores ( $_SESSION ['projovemurbano'] ['proid'], 'Q' );
		$educadores_P = pegarEducadores ( $_SESSION ['projovemurbano'] ['proid'], 'P' );
		$educadores_M = pegarEducadores ( $_SESSION ['projovemurbano'] ['proid'], 'M' );
		$educadores_T = pegarEducadores ( $_SESSION ['projovemurbano'] ['proid'], 'T' );
		$educadores_E = pegarEducadores ( $_SESSION ['projovemurbano'] ['proid'], 'E' );
		
		$totalUtilizado = $educadores_F ['crevlrtotal'] + $educadores_Q ['crevlrtotal'] + $educadores_P ['crevlrtotal'] + $educadores_M ['crevlrtotal'] + $educadores_T ['crevlrtotal'] + $educadores_E ['ccmvlrtotal'] + $educadores_F ['ccmvlrtotal'] + $educadores_Q ['ccmvlrtotal'] + $educadores_P ['ccmvlrtotal'];
		// ver($profissionais['propercmax'],$montante,d);
		$provlrmaximo = round ( ($profissionais ['propercmax'] * $montante) / 100, 2 );
		// $totalutilizado_profissionais = round($coordgeral['cgevlrtotal']+$coordassistentes['coavlrtotal']+$diretorpolo['ccmvlrtotal']+$dirassistentes['crevlrtotal']+$totalUtilizado,2);
		
		// ver($diretorpolo);
		$totalutilizado_profissionais = '';
		
		$totalutilizado_profissionais = $coordgera + $coordassistentes ['coavlrtotal'] + (is_array($diretorpol)?$diretorpol['ccmvlrtotal']:'') + $dirassistentes['crevlrtotal'] + $totalUtilizado;
		// ver($coordgera,
		// $coordassistentes['coavlrtotal'],
		// $diretorpol['ccmvlrtotal'],
		// $dirassistentes['crevlrtotal'],
		// $totalUtilizado);
		// ver($totalutilizado_profissionais,$provlrmaximo,$_SESSION['projovemurbano']['tprid']);
		
		if ($_SESSION ['projovemurbano'] ['muncod']) {
			$mun = "AND m.muncod = '{$_SESSION['projovemurbano']['muncod']}'";
		}
		// $sql = "SELECT count(p.polid)
		// FROM projovemurbano.polo p
		// INNER JOIN projovemurbano.polomunicipio pm ON pm.pmuid = p.pmuid
		// INNER JOIN projovemurbano.associamucipiopolo ass ON ass.polid = p.polid
		// INNER JOIN projovemurbano.municipio m ON m.munid = ass.munid
		// WHERE pm.pjuid='341'
		// AND p.polstatus='A'
		// $mun";
		// ver($sql, d);
		// $Npolos = $db->pegaLinha($sql);
		// $educadores_F['ccmvlrtotal']+$educadores_Q['ccmvlrtotal']+$educadores_P['ccmvlrtotal']+
		// $educadores_F['crevlrtotal']+$educadores_Q['crevlrtotal']+$educadores_P['crevlrtotal']+
		// $educadores_M['crevlrtotal']+$educadores_T['crevlrtotal']
		// dbg($diretorpolo['dipqtd']);
		// dbg($Npolos['qtdpolos']);
		$contagemPolos = pegarNumeroPolos ( true );
		
		if ($contagemPolos) {
			foreach ( $contagemPolos as $poloTotal ) {
				// if ($_SESSION['projovemurbano']['tprid'] == $poloTotal['tprid']) {
				$tenhoPolo = true;
				// }
				$numeropolos += ( int ) $poloTotal ['count'];
			}
		}
		$numeronucleos = pegarNumeroNucleos ( $tenhoPolo );
		
		$qtdDiretor = $diretorpolo ['dipeqtdefetivo40hr'] + $diretorpolo ['dipqtdrecursoproprio'] + $diretorpolo ['ccmqtd'];
		$qtdEducador = $educadores_T ['eduefetivo30hr'] + $educadores_T ['eduqtdrecursoproprio'] + $educadores_T ['creqtd'];
		
		// if( $numeropolos < $qtdDiretor || $numeronucleos < $qtdEducador ){
		// $msg[] = "<br>".$tprdesc.": O valor utilizado é maior que o permitido.";
		// }
		if ($numeropolos < $qtdDiretor || $numeronucleos < $qtdEducador) {
			$msg [] = "<br>" . $tprdesc . ": O número de profissionais a ser contratado não condiz com a necessidade.";
		}
		
		// if($dirassistentes_A['dasqtd'] != $Npolos['qtdpolos']){
		// $msg[] = "<br>".$tprdesc.": O número de assistentes administrativos a ser contratado não condiz com a necessidade.";
		// }
		//
		// if($dirassistentes_P['dasqtd'] != $Npolos['qtdpolos']){
		// $msg[] = "<br>".$tprdesc.": O número de assistentes pedagógicos a ser contratado não condiz com a necessidade.";
		// }
		//
		if (! $profissionais ['propercmax']) {
			$msg [] = "<br>" . $tprdesc . ": A Tela de Profissionais não foi gravada.";
		}
		// dbg(round($totalutilizado_profissionais));
		if (round ( $totalutilizado_profissionais ) > $provlrmaximo) {
			// dbg(round($totalutilizado_profissionais) > $provlrmaximo); d
			$msg [] = "<br>" . $tprdesc . ": O valor utilizado em profissionais é maior que a percentagem prevista.";
		}
		
		if ($totalutilizado_profissionais > 0 && $montante > 0) {
			$vlr = round ( ($totalutilizado_profissionais / $montante) * 100, 1 );
		} else {
			$vlr = 0;
		}
		
		// if( $_SESSION['projovemurbano']['proid'] ){
		// $db->executar("UPDATE projovemurbano.profissionais
		// SET propercutilizado='".$vlr."'
		// WHERE proid='".$_SESSION['projovemurbano']['proid']."'");
		// }
		
		/* FIM validando profissionais */

		/*
		 * VALIDANDO FORMAÇÃO
		*
		* - verifica se a aba formação de educadores foi gravado pelo menos uma vez;
		* - verifica se o valor total dos recursos com a formação é maior que o percentual previsto;
		* - atualiza o percentual utilizado dos recursos gastos com formação (caso tenha ocorrido alguma falha)
		* - verifica se o valor total dos recursos com a formação é maior que o percentual previsto;
		* - atualiza o percentual utilizado dos recursos gastos com formação (caso tenha ocorrido alguma falha)
		*
		*/

		criaSessaoFormacaoEducadores ();
		
		$sql = "SELECT * FROM projovemurbano.formacaoeducadores
                          WHERE fedid='" . $_SESSION ['projovemurbano'] ['fedid'] . "'
                            AND tprid = {$_SESSION['projovemurbano']['tprid']}";
		$formacaoeducadores = $db->pegaLinha ( $sql );
		
		$fedvlrmaximo = round ( ($formacaoeducadores ['fedpercmax'] * $montante) / 100, 2 );
		
		$sql = "SELECT SUM(rgavalor) FROM projovemurbano.recursosgastos
		WHERE rgastatus='A' AND fedid='" . $_SESSION ['projovemurbano'] ['fedid'] . "'";
		
		$totalutilizado_formacaoeducadores = $db->pegaUm ( $sql );
		// ver($formacaoeducadores['fedpercmax']);
		if (! $formacaoeducadores ['fedpercmax']) {
			$msg [] = "<br>" . $tprdesc . ": A Tela de Formação de Educadores não foi gravada.";
		}
		
		if ($totalutilizado_formacaoeducadores > $fedvlrmaximo) {
			$msg [] = "<br>" . $tprdesc . ": Recursos gastos com a formação é maior que a percentagem prevista.";
		}
		
		if ($totalutilizado_formacaoeducadores > 0 && $montante > 0) {
			$vlr = round ( ($totalutilizado_formacaoeducadores / $montante) * 100, 1 );
		} else {
			$vlr = 0;
		}
		
		if ($_SESSION ['projovemurbano'] ['fedid']) {
			$db->executar ( "UPDATE projovemurbano.formacaoeducadores 
							SET fedperutilizado='" . $vlr . "' 
							WHERE fedid='" . $_SESSION ['projovemurbano'] ['fedid'] . "'
                                                          AND tprid = {$_SESSION['projovemurbano']['tprid']}" );
		}
		
		$auxiliofinanceiro = $db->pegaLinha ( "SELECT aufavlrauxilio, aufpercmax, (aufqtdeducador*aufavlrauxilio) as total1etapa FROM projovemurbano.auxiliofinanceiro WHERE fedid='" . $_SESSION ['projovemurbano'] ['fedid'] . "' " );
		
		$aufvlrmaximo = round ( ($auxiliofinanceiro ['aufpercmax'] * $montante) / 100, 2 );
		$totalutilizado_auxiliofinanceiro = $auxiliofinanceiro ['total1etapa'];
		
		$educadores_F ['crevlrbrutorem'] = $educadores_F ['crevlrbrutorem'] == 'null' ? '0' : $educadores_F ['crevlrbrutorem'];
		$educadores_Q ['crevlrbrutorem'] = $educadores_Q ['crevlrbrutorem'] == 'null' ? '0' : $educadores_Q ['crevlrbrutorem'];
		$educadores_P ['crevlrbrutorem'] = $educadores_P ['crevlrbrutorem'] == 'null' ? '0' : $educadores_P ['crevlrbrutorem'];
		
		if (round ( $auxiliofinanceiro ['aufavlrauxilio'], 2 ) > round ( ($educadores_F ['crevlrbrutorem'] + $educadores_Q ['crevlrbrutorem'] + $educadores_P ['crevlrbrutorem']), 2 )) {
			$msg [] = "<br>" . $tprdesc . ": Auxílio financeiro a ser pago(R$) maior do que o permitido.";
		}
		
		if ($totalutilizado_auxiliofinanceiro > $aufvlrmaximo) {
			$msg [] = "<br>" . $tprdesc . ": Valor destinado ao pagamento de auxílio financeiro para a primeira etapa da formação é maior que a percentagem prevista.";
		}
		
		if ($totalutilizado_auxiliofinanceiro > 0 && $montante > 0) {
			$vlr = number_format ( ($totalutilizado_auxiliofinanceiro / $montante) * 100, 1 );
		} else {
			$vlr = 0;
		}
		
		if ($_SESSION ['projovemurbano'] ['fedid']) {
			$db->executar ( "UPDATE projovemurbano.auxiliofinanceiro 
							SET aufpercutilizado='" . $vlr . "' 
							WHERE fedid='" . $_SESSION ['projovemurbano'] ['fedid'] . "'" );
		}
		
		/* FIM validando formação */

		/*
		 * VALIDANDO GENÊRO ALIMENTICIOS
		*
		* - verifica se a aba genero alimenticios foi gravado pelo menos uma vez;
		* - atualiza o percentual utilizado dos generos alimenticios (caso tenha ocorrido alguma falha)
		*
		*/

		criaSessaoGeneroAlimenticios ();
		
		$generoalimenticio = $db->pegaLinha ( "SELECT * FROM projovemurbano.generoalimenticio 
											WHERE galid='" . $_SESSION ['projovemurbano'] ['galid'] . "'" );
		
		$lancherefeicao = $db->pegaLinha ( "SELECT * FROM projovemurbano.lancherefeicao WHERE galid='" . $_SESSION ['projovemurbano'] ['galid'] . "'" );
		
		$galvlrmaximo = round ( ($generoalimenticio ['galpercmax'] * $montante) / 100, 2 );
		$totalutilizado_generoalimenticio = $lancherefeicao ['lrevlrtotal'];
		
		if (! $generoalimenticio ['galpercmax']) {
			$msg [] = "<br>" . $tprdesc . ": A Tela de Gêneros Alimenticios não foi gravada.";
		}
		
		if ($totalutilizado_generoalimenticio > $galvlrmaximo) {
			$msg [] = "<br>" . $tprdesc . ": Valor do Lanche ou Refeição é maior que a percentagem prevista.";
		}
		
		if ($totalutilizado_generoalimenticio > 0 && $montante > 0) {
			$vlr = number_format ( ($totalutilizado_generoalimenticio / $montante) * 100, 1 );
		} else {
			$vlr = 0;
		}
		
		if ($_SESSION ['projovemurbano'] ['galid']) {
			$db->executar ( "UPDATE projovemurbano.generoalimenticio 
							SET galpercutilizado='" . $vlr . "'
							WHERE galid='" . $_SESSION ['projovemurbano'] ['galid'] . "'" );
		}
		
		/* FIM validando genêro alimenticios */

		/*
		 * VALIDANDO QUALIFICAÇÃO PROFISSIONAL
		*
		* - verifica se a aba qualificação profissional foi gravado pelo menos uma vez;
		* - atualiza o percentual utilizado na qualificação profissional (caso tenha ocorrido alguma falha)
		*
		*/

		criaSessaoQualificacaoProfissional ();
		
		$qualificacaoprofissional = $db->pegaLinha ( "SELECT * FROM projovemurbano.qualificacaoprofissional WHERE qprid='" . $_SESSION ['projovemurbano'] ['qprid'] . "'" );
		
		$qprvlrmaximo = round ( ($qualificacaoprofissional ['qprpercmax'] * $montante) / 100, 2 );
		
		$sql = "SELECT 
					SUM(pgavlrmes*pgaqtdmeses), 
					nucid 
				FROM 
					projovemurbano.previsaogasto 
				WHERE 
					qprid = '" . $_SESSION ['projovemurbano'] ['qprid'] . "' 
					AND pgastatus='A' 
				GROUP BY 
					nucid";
		
		$totalutilizado_qualificacaoprofissional = $db->pegaUm ( $sql );
		;
		
		if (! $qualificacaoprofissional ['qprpercmax']) {
			$msg [] = "<br>" . $tprdesc . ": A Tela de Qualificação profissional não foi gravada.";
		}
		
		if ($totalutilizado_qualificacaoprofissional > $qprvlrmaximo) {
			$msg [] = "<br>" . $tprdesc . ": Despesas com qualificação profissional é maior que a percentagem prevista.";
		}
		
		//
		if ($nucleos [0]) {
			foreach ( $nucleos as $n ) {
				$nucleo [] = $n ['nucid'];
			}
		}
		if ($nucleo) {
			$arcids = $db->carregarColuna ( "SELECT arcid FROM projovemurbano.arcoqualificacao WHERE nucid IN('" . implode ( "','", $nucleo ) . "') AND qprid='" . $_SESSION ['projovemurbano'] ['qprid'] . "' AND arqstatus='A'" );
		}
		
		if ($qualificacaoprofissional ['qprarco'] == "t") {
			if ($arcids) {
				if (in_array ( '', $arcids )) {
					$msg [] = "<br>" . $tprdesc . ": Não fizeram vinculação de arco.";
				}
			}
		}
		
		if ($totalutilizado_generoalimenticio > 0 && $montante > 0) {
			$vlr = number_format ( ($totalutilizado_generoalimenticio / $montante) * 100, 1 );
		} else {
			$vlr = 0;
		}
		
		if ($_SESSION ['projovemurbano'] ['qprid']) {
			$db->executar ( "UPDATE projovemurbano.qualificacaoprofissional 
							SET qprpercutilizado='" . $vlr . "' 
							WHERE qprid='" . $_SESSION ['projovemurbano'] ['qprid'] . "'" );
		}
		
		/* FIM validando qualificação profissional */
		
		if ($_SESSION ['projovemurbano'] ['estuf']) :
			
			/*
			 * VALIDANDO TRANSPORTE DIDATICO - verifica se a aba transporte didatico foi gravado pelo menos uma vez; - atualiza o percentual utilizado em transporte didatico (caso tenha ocorrido alguma falha)
			 */
			
			$sql = "SELECT * FROM projovemurbano.transportematerial WHERE pjuid='" . $_SESSION ['projovemurbano'] ['pjuid'] . "' AND tprid = {$_SESSION['projovemurbano']['tprid']}";
			$transportematerial = $db->pegaLinha ( $sql );
			
			$tmavlrmaximo = round ( ($transportematerial ['tmapercmax'] * $montante) / 100, 2 );
			$totalutilizado_transportematerial = $transportematerial ['tmarecursoutilizado'];
			
			if (! $transportematerial ['tmapercmax']) {
				$msg [] = "<br>" . $tprdesc . ": A Tela de Transporte Didático não foi gravada.";
			}
			
			if ($totalutilizado_transportematerial > $tmavlrmaximo) {
				$msg [] = "<br>" . $tprdesc . ": Recursos Utilizados em transporte didático é maior que a percentagem prevista.";
			}
			
			if ($transportematerial ['tmaid']) {
				$db->executar ( "UPDATE projovemurbano.transportematerial 
							SET tmaperutilizado='" . number_format ( ($totalutilizado_transportematerial / $montante) * 100, 1 ) . "' 
							WHERE tmaid='" . $transportematerial ['tmaid'] . "'" );
			}
		
		
			
		/* FIM validando transporte didatico */
			
		endif;
		
		/*
		 * VALIDANDO DEMAIS AÇÕES - verifica se a aba demais ações foi gravado pelo menos uma vez; - atualiza o percentual utilizado em demais ações (caso tenha ocorrido alguma falha)
		 */
		
		criaSessaoDemaisAcoes ();
		
		$totalutilizado_profissionais = $totalutilizado_profissionais > 0 ? $totalutilizado_profissionais : '0';
		$totalutilizado_formacaoeducadores = $totalutilizado_formacaoeducadores > 0 ? $totalutilizado_formacaoeducadores : '0';
		$totalutilizado_auxiliofinanceiro = $totalutilizado_auxiliofinanceiro > 0 ? $totalutilizado_auxiliofinanceiro : '0';
		$totalutilizado_generoalimenticio = $totalutilizado_generoalimenticio > 0 ? $totalutilizado_generoalimenticio : '0';
		$totalutilizado_qualificacaoprofissional = $totalutilizado_qualificacaoprofissional > 0 ? $totalutilizado_qualificacaoprofissional : '0';
		$totalutilizado_transportematerial = $totalutilizado_transportematerial > 0 ? $totalutilizado_transportematerial : '0';
		
		$totalmaxdemaisacoes = $montante - ($totalutilizado_profissionais + $totalutilizado_formacaoeducadores + $totalutilizado_auxiliofinanceiro + $totalutilizado_generoalimenticio + $totalutilizado_qualificacaoprofissional + $totalutilizado_transportematerial);
		$totalmaxdemaisacoes = $totalmaxdemaisacoes > 0 ? $totalmaxdemaisacoes : 0;
		
		// Gatilho pra não precisar refazer todos os calculos, reaproveitando o código
		
		if ($retornarTotalMaximoDemaisAcoes)
			return $totalmaxdemaisacoes;
		
		$totalutilizado_demaisacoes = $db->pegaUm ( "SELECT SUM(idaqtdmeses*idavlrmes) as x FROM projovemurbano.itemdemaisacoes WHERE deaid='" . $_SESSION ['projovemurbano'] ['deaid'] . "'" );
		
		$totalUtil_demaisacoes = str_replace ( '.', '', number_format ( $totalutilizado_demaisacoes, 2, '', '.' ) );
		$totalMax_demaisacoes = str_replace ( '.', '', number_format ( $totalmaxdemaisacoes, 2, '', '.' ) );
		
		if ($totalUtil_demaisacoes > $totalMax_demaisacoes) {
			$msg [] = "<br>" . $tprdesc . ": Gastos com Demais ações é maior que a percentagem prevista.";
		}
		
		if ($totalutilizado_demaisacoes > 0 && $montante > 0) {
			$vlr = number_format ( ($totalutilizado_demaisacoes / $montante) * 100, 1 );
		} else {
			$vlr = 0;
		}
		
		if ($_SESSION ['projovemurbano'] ['deaid']) {
			$db->executar ( "UPDATE projovemurbano.demaisacoes 
							SET deapercutilizado='" . $vlr . "' 
							WHERE deaid='" . $_SESSION ['projovemurbano'] ['deaid'] . "'" );
		}
		
		/* FIM validando demais ações */
		
		$db->commit ();
		
		// return "Validação completa ainda não esta disponivel. Em breve poderá enviar o Plano de Implementação para analise do MEC";
		
		// if(date("Y-m-d")>"2012-01-23") return "Prazo para envio do Plano de implementação terminou. Obrigado!";
	}
	
	unset ( $_SESSION ['projovemurbano'] ['tprid'] );
	
	if ($tpridTemp != '') {
		$_SESSION ['projovemurbano'] ['tprid'] = $tpridTemp;
	}
	
	return $msg;
}
function validacaoCompletaPlanoImplementacao2014() {
	global $db, $retornarTotalMaximoDemaisAcoes;
	
	$msg = '';
	$tpridTemp = '';
	$max = 4;
	if (isset ( $_SESSION ['projovemurbano'] ['tprid'] )) {
		$tpridTemp = $_SESSION ['projovemurbano'] ['tprid'];
		$max = 2;
	}
	
	for($x = 1; $x < $max; $x ++) {
		
		if ($tpridTemp != '') {
			$_SESSION ['projovemurbano'] ['tprid'] = $tpridTemp;
		} else {
			$_SESSION ['projovemurbano'] ['tprid'] = $x;
		}
		$sql = "SELECT
		tprdesc
		FROM
		projovemurbano.tipoprograma
		WHERE
		tprid = {$_SESSION['projovemurbano']['tprid']}";
		
		$tprdesc = $db->pegaUm ( $sql );
		
		if ($_SESSION ['projovemurbano'] ['muncod']) {
			$sugestaoampliacao = $db->pegaLinha ( "SELECT suaverdade, suametaajustada FROM projovemurbano.sugestaoampliacao WHERE pjuid='" . $_SESSION ['projovemurbano'] ['pjuid'] . "'" );
			$meta = $db->pegaUm ( "SELECT
								mtpvalor as valor,
								mtp.tpmid as tipo
								FROM
								projovemurbano.metasdoprograma mtp
								INNER JOIN projovemurbano.tipometadoprograma tpr ON tpr.tpmid = mtp.tpmid
								WHERE
								pjuid = {$_SESSION['projovemurbano']['pjuid']}
										AND tprid = {$_SESSION['projovemurbano']['tprid']}
												ORDER BY
												tipo DESC" );
			// if( $meta < 1 ){
			// return '';
			// }
			if ($sugestaoampliacao ['suaverdade'] == "t") {
				if ($sugestaoampliacao ['suametaajustada'])
					$meta = $sugestaoampliacao ['suametaajustada'];
			}
		}
		
		if ($_SESSION ['projovemurbano'] ['estuf']) {
			$sugestaoampliacao = $db->pegaLinha ( "SELECT suaverdade, suametaajustada FROM projovemurbano.sugestaoampliacao WHERE pjuid='" . $_SESSION ['projovemurbano'] ['pjuid'] . "'" );
			$sql = "SELECT
		mtpvalor as valor,
		mtp.tpmid as tipo
		FROM
		projovemurbano.metasdoprograma mtp
		INNER JOIN projovemurbano.tipometadoprograma tpr ON tpr.tpmid = mtp.tpmid
		WHERE
		pjuid = {$_SESSION['projovemurbano']['pjuid']}
		AND tprid = {$_SESSION['projovemurbano']['tprid']}
		ORDER BY
		tipo DESC ";
			$meta = $db->pegaUm ( $sql );
			
			// if( $meta < 1 ){
			// return '';
			// }
			if ($sugestaoampliacao ['suaverdade'] == "t") {
				if ($sugestaoampliacao ['suametaajustada'])
					$meta = $sugestaoampliacao ['suametaajustada'];
			}
		}
		
		$meta = carregarMeta ( $sugestaoampliacao );
		if ($meta < 1) {
			continue;
		}
		/*
		 * VALIDANDO NÚMERO DE NÚCLEOS Se o núcleo for igual a 1 (um), o nº de alunos deve ser necessariamente 200. Se o nº de núcleo for maior que 1 (um), o nº de alunos no núcleo poderá variar entre 150 a 200.
		 */
		
		$sql = "SELECT pmupossuipolo FROM projovemurbano.polomunicipio WHERE pjuid='" . $_SESSION ['projovemurbano'] ['pjuid'] . "'";
		$pmupossuipolo = $db->pegaUm ( $sql );
		
		if ($pmupossuipolo == "t") {
			
			$nucleos = $db->carregar ( "SELECT mun.munid, nuc.nucid, nuc.nucqtdestudantes FROM projovemurbano.nucleo nuc
									INNER JOIN projovemurbano.municipio mun ON mun.munid = nuc.munid
									INNER JOIN projovemurbano.associamucipiopolo amp ON amp.munid = mun.munid
									INNER JOIN projovemurbano.polo pol ON pol.polid = amp.polid
									INNER JOIN projovemurbano.polomunicipio plm ON plm.pmuid = pol.pmuid
									WHERE
										nuc.nucstatus='A'
										AND mun.munstatus='A'
										AND plm.pmustatus='A'
										AND pol.polstatus='A'
										AND pjuid='" . $_SESSION ['projovemurbano'] ['pjuid'] . "'
															AND nuc.tprid = {$_SESSION['projovemurbano']['tprid']}" );
		} else {
			
			$nucleos = $db->carregar ( "SELECT mun.munid, nuc.nucid, nuc.nucqtdestudantes FROM projovemurbano.nucleo nuc
									INNER JOIN projovemurbano.municipio mun ON mun.munid = nuc.munid
									INNER JOIN projovemurbano.polomunicipio plm ON plm.pmuid = mun.pmuid
									WHERE
										nuc.nucstatus='A'
										AND mun.munstatus='A'
										AND plm.pmustatus='A'
														AND pjuid='" . $_SESSION ['projovemurbano'] ['pjuid'] . "'
														AND nuc.tprid = {$_SESSION['projovemurbano']['tprid']}" );
		}
		
		$totalestudantes = 0;
		if ($nucleos [0]) {
			$_CHK = Array ();
			foreach ( $nucleos as $nucleo ) {
				$_CHK [$nucleo ['munid']] [$nucleo ['nucid']] = $nucleo ['nucqtdestudantes'];
				$totalestudantes += $nucleo ['nucqtdestudantes'];
			}
		} else {
			$msg [] = "<br>" . $tprdesc . ": Não existe núcleo não cadastrado.";
		}
		
		if ($totalestudantes != $meta) {
			$msg [] = "<br>" . $tprdesc . ": Quantidade de estudantes/Núcleo diferente da meta (Meta:" . $meta . ",Utilizado:" . $totalestudantes . ").";
		}
		
		if ($_CHK) {
			foreach ( array_keys ( $_CHK ) as $munid ) {
				if (count ( $_CHK [$munid] ) == 1) {
					$qtde = current ( $_CHK [$munid] );
					if ($qtde != '200' && $_SESSION ['projovemurbano'] ['tprid'] != 2) {
						$msg [] = "<br>" . $tprdesc . ": Se o núcleo for igual a 1 (um), o nº de alunos deve ser necessariamente 200.";
					}
					if ($qtde < '60' && $qtde > '150' && $_SESSION ['projovemurbano'] ['tprid'] == 2) {
						$msg [] = "<br>" . $tprdesc . ": Se o núcleo for igual a 1 (um), o nº de alunos deve ser necessariamente entre 60 e 150.";
					}
				}
			}
		}
		
		/* FIM validando número de núcleos */
		$montante = calcularMontante ( $meta );
		
		/*
		 * VALIDANDO PROFISSIONAIS - verifica se a aba profissionais foi gravado pelo menos uma vez; - verifica se o valor total de profissionais é maior que o percentual previsto; - atualiza o percentual utilizado (caso tenha ocorrido alguma falha)
		 */
		
		criaSessaoProfissionais ();
		
		unset ( $dirassistentes );
		unset ( $casoprisionais2 );
		unset ( $coordgera );
		unset ( $assitente_A2 );
		
		$profissionais = $db->pegaLinha ( "SELECT * FROM projovemurbano.profissionais WHERE proid = " . $_SESSION ['projovemurbano'] ['proid'] . " " );
		
		$_SESSION ['projovemurbano'] ['proid'] = $profissionais ['proid'];
		$proid = $_SESSION ['projovemurbano'] ['proid'];
		$coordgeral = pegarCoordenadorGeral ( $_SESSION ['projovemurbano'] ['proid'] );
		$assistenteadministrativo_A = pegarAssistentesCoordenadorGeral ( $_SESSION ['projovemurbano'] ['proid'], 'A' );
		$assistenteadministrativo_P = pegarAssistentesCoordenadorGeral ( $_SESSION ['projovemurbano'] ['proid'], 'P' );
		$assistenteadministrativo_M = pegarAssistentesCoordenadorGeral ( $_SESSION ['projovemurbano'] ['proid'], 'M', $_SESSION ['projovemurbano'] ['tprid'] );
		$assistenteadministrativo_AP = pegarAssistentesCoordenadorGeral ( $_SESSION ['projovemurbano'] ['proid'], 'AP', $_SESSION ['projovemurbano'] ['tprid'] );
		$assistenteadministrativo_PP = pegarAssistentesCoordenadorGeral ( $_SESSION ['projovemurbano'] ['proid'], 'PP', $_SESSION ['projovemurbano'] ['tprid'] );
		
		if ($_SESSION ['projovemurbano'] ['tprid'] == 2) {
			$casoprisionais2 = $assistenteadministrativo_AP ['coavlrtotal'] + $assistenteadministrativo_PP ['coavlrtotal'];
			$casoprisionais3 = $assistenteadministrativo_AP ['ccmvlrtotal'] + $assistenteadministrativo_PP ['ccmvlrtotal'];
			
		}
		if ($coordgeral ['tprid'] == $_SESSION ['projovemurbano'] ['tprid'] && $coordgeral ['tprid'] != '') {
			$coordgera = $coordgeral ['cgevlrtotal'];
			$coordgera2 = $coordgeral ['crevlrtotal'];
		}
		if ($coordgeral ['tprid'] == $_SESSION ['projovemurbano'] ['tprid'] && $coordgeral ['tprid'] != '') {
			$assitente_A2 = $assistenteadministrativo_A ['coavlrtotal'] + $assistenteadministrativo_P ['coavlrtotal'];
			$assitente_A3 = $assistenteadministrativo_A ['ccmvlrtotal'] + $assistenteadministrativo_P ['ccmvlrtotal'];
			$assitente_M1  = $assistenteadministrativo_M ['coaqtd'] ;
			$assitente_M2  = $assistenteadministrativo_M ['coavlrtotal'] ;
		}
		
		$coordassistentes = array (
				"coaqtd" => $assitente_A1 + $assitente_M1 + $casoprisionais1,
				"coavlrtotal" => $assitente_A2 + $assitente_M2 + $casoprisionais2,
				"ccmvlrtotal" =>$assitente_A3 + $casoprisionais3
		);
		
		$diretorpolo = pegarDiretorPolo ( $_SESSION ['projovemurbano'] ['proid'] );
		$dirassistentes_A = pegarAssistentesDiretorPolo ( $_SESSION ['projovemurbano'] ['proid'], 'A' );
		$dirassistentes_P = pegarAssistentesDiretorPolo ( $_SESSION ['projovemurbano'] ['proid'], 'P' );
		$diretorpol = '';
		if ($coordgeral ['tprid'] == $_SESSION ['projovemurbano'] ['tprid'] && $coordgeral ['tprid'] != '') {
			$diretorpol = pegarDiretorPolo ( $_SESSION ['projovemurbano'] ['proid'] );
		}
		// dbg( $coordgeral['tprid'] == $_SESSION['projovemurbano']['tprid'] && $coordgeral['tprid'] != '');
		
		if ($coordgeral ['tprid'] == $_SESSION ['projovemurbano'] ['tprid'] && $coordgeral ['tprid'] != '') {
			$dirassistentes = array (
					"dasqtdefetivo40hr" => $dirassistentes_A ['dasqtdefetivo40hr'] + $dirassistentes_P ['dasqtdefetivo40hr'],
					"dasqtdrecursoproprio" => $dirassistentes_A ['dasqtdrecursoproprio'] + $dirassistentes_P ['dasqtdrecursoproprio'],
					"creqtd" => $dirassistentes_A ['creqtd'] + $dirassistentes_P ['creqtd'],
					"crevlrtotal" => $dirassistentes_A ['crevlrtotal'] + $dirassistentes_P ['crevlrtotal'],
					"ccmvlrtotal" => $dirassistentes_A ['ccmvlrtotal'] + $dirassistentes_P ['ccmvlrtotal']
			);
		}
		
		$educadores_F = pegarEducadores ( $_SESSION ['projovemurbano'] ['proid'], 'F' );
		$educadores_Q = pegarEducadores ( $_SESSION ['projovemurbano'] ['proid'], 'Q' );
		$educadores_P = pegarEducadores ( $_SESSION ['projovemurbano'] ['proid'], 'P' );
		$educadores_M = pegarEducadores ( $_SESSION ['projovemurbano'] ['proid'], 'M' );
		$educadores_T = pegarEducadores ( $_SESSION ['projovemurbano'] ['proid'], 'T' );
		$educadores_E = pegarEducadores ( $_SESSION ['projovemurbano'] ['proid'], 'E' );
		
		$totalUtilizado = $educadores_F ['crevlrtotal'] + $educadores_Q ['crevlrtotal'] + $educadores_P ['crevlrtotal'] + $educadores_M ['crevlrtotal'] + $educadores_T ['crevlrtotal'] + (is_array($educadores_E)?$educadores_E['ccmvlrtotal']:0) + (is_array($educadores_F)?$educadores_F['ccmvlrtotal']:0) + (is_array($educadores_Q)?$educadores_Q['ccmvlrtotal']:0) + (is_array($educadores_P)?$educadores_Q['ccmvlrtotal']:0);
		// ver($profissionais['propercmax'],$montante,d);
		$provlrmaximo = round ( ($profissionais ['propercmax'] * $montante) / 100, 2 );
		// $totalutilizado_profissionais = round($coordgeral['cgevlrtotal']+$coordassistentes['coavlrtotal']+$diretorpolo['ccmvlrtotal']+$dirassistentes['crevlrtotal']+$totalUtilizado,2);
		
		// ver($diretorpolo);
		$totalutilizado_profissionais = '';
		
		$totalutilizado_profissionais = $coordgera+$coordgera2 + $coordassistentes ['coavlrtotal'] +$coordassistentes['ccmvlrtotal']+ (is_array($diretorpol)?$diretorpol['ccmvlrtotal']:'') + $dirassistentes ['crevlrtotal'] + $dirassistentes ['ccmvlrtotal']+ $totalUtilizado;
		// ver($coordgera,
		// $coordassistentes['coavlrtotal'],
		// $diretorpol['ccmvlrtotal'],
		// $dirassistentes['crevlrtotal'],
		// $totalUtilizado);
		// ver($totalutilizado_profissionais,$provlrmaximo,$_SESSION['projovemurbano']['tprid']);
		
		if ($_SESSION ['projovemurbano'] ['muncod']) {
			$mun = "AND m.muncod = '{$_SESSION['projovemurbano']['muncod']}'";
		}
		// $sql = "SELECT count(p.polid)
		// FROM projovemurbano.polo p
		// INNER JOIN projovemurbano.polomunicipio pm ON pm.pmuid = p.pmuid
		// INNER JOIN projovemurbano.associamucipiopolo ass ON ass.polid = p.polid
		// INNER JOIN projovemurbano.municipio m ON m.munid = ass.munid
		// WHERE pm.pjuid='341'
		// AND p.polstatus='A'
		// $mun";
		// ver($sql, d);
		// $Npolos = $db->pegaLinha($sql);
		// $educadores_F['ccmvlrtotal']+$educadores_Q['ccmvlrtotal']+$educadores_P['ccmvlrtotal']+
		// $educadores_F['crevlrtotal']+$educadores_Q['crevlrtotal']+$educadores_P['crevlrtotal']+
		// $educadores_M['crevlrtotal']+$educadores_T['crevlrtotal']
		// dbg($diretorpolo['dipqtd']);
		// dbg($Npolos['qtdpolos']);
		$contagemPolos = pegarNumeroPolos ( true );
		
		if ($contagemPolos) {
			foreach ( $contagemPolos as $poloTotal ) {
				// if ($_SESSION['projovemurbano']['tprid'] == $poloTotal['tprid']) {
				$tenhoPolo = true;
				// }
				$numeropolos += ( int ) $poloTotal ['count'];
			}
		}
		$numeronucleos = pegarNumeroNucleos ( $tenhoPolo );
		
		$qtdDiretor = $diretorpolo ['dipeqtdefetivo40hr'] + $diretorpolo ['dipqtdrecursoproprio'] + $diretorpolo ['ccmqtd'];
		$qtdEducador = $educadores_T ['eduefetivo30hr'] + $educadores_T ['eduqtdrecursoproprio'] + $educadores_T ['creqtd'];
		
		// if( $numeropolos < $qtdDiretor || $numeronucleos < $qtdEducador ){
		// $msg[] = "<br>".$tprdesc.": O valor utilizado é maior que o permitido.";
		// }
		if ($numeropolos < $qtdDiretor || $numeronucleos < $qtdEducador) {
			$msg [] = "<br>" . $tprdesc . ": O número de profissionais a ser contratado não condiz com a necessidade.";
		}
		
		// if($dirassistentes_A['dasqtd'] != $Npolos['qtdpolos']){
		// $msg[] = "<br>".$tprdesc.": O número de assistentes administrativos a ser contratado não condiz com a necessidade.";
		// }
		//
		// if($dirassistentes_P['dasqtd'] != $Npolos['qtdpolos']){
		// $msg[] = "<br>".$tprdesc.": O número de assistentes pedagógicos a ser contratado não condiz com a necessidade.";
		// }
		//
		if (! $profissionais ['propercmax']) {
			$msg [] = "<br>" . $tprdesc . ": A Tela de Profissionais não foi gravada.";
		}
		// dbg(round($totalutilizado_profissionais));
		if (round ( $totalutilizado_profissionais ) > $provlrmaximo) {
			// dbg(round($totalutilizado_profissionais) > $provlrmaximo); d
			$msg [] = "<br>" . $tprdesc . ": O valor utilizado em profissionais é maior que a percentagem prevista.";
		}
		
		if ($totalutilizado_profissionais > 0 && $montante > 0) {
			$vlr = round ( ($totalutilizado_profissionais / $montante) * 100, 1 );
		} else {
			$vlr = 0;
		}
		
		// if( $_SESSION['projovemurbano']['proid'] ){
		// $db->executar("UPDATE projovemurbano.profissionais
		// SET propercutilizado='".$vlr."'
		// WHERE proid='".$_SESSION['projovemurbano']['proid']."'");
		// }
		
		/* FIM validando profissionais */

/*
* VALIDANDO FORMAÇÃO
*
* - verifica se a aba formação de educadores foi gravado pelo menos uma vez;
* - verifica se o valor total dos recursos com a formação é maior que o percentual previsto;
* - atualiza o percentual utilizado dos recursos gastos com formação (caso tenha ocorrido alguma falha)
* - verifica se o valor total dos recursos com a formação é maior que o percentual previsto;
* - atualiza o percentual utilizado dos recursos gastos com formação (caso tenha ocorrido alguma falha)
*
*/

criaSessaoFormacaoEducadores ();
		
		$sql = "SELECT * FROM projovemurbano.formacaoeducadores
WHERE fedid='" . $_SESSION ['projovemurbano'] ['fedid'] . "'
AND tprid = {$_SESSION['projovemurbano']['tprid']}";
		$formacaoeducadores = $db->pegaLinha ( $sql );
		
		$fedvlrmaximo = round ( ($formacaoeducadores ['fedpercmax'] * $montante) / 100, 2 );
		
		$sql = "SELECT SUM(rgavalor) FROM projovemurbano.recursosgastos
WHERE rgastatus='A' AND fedid='" . $_SESSION ['projovemurbano'] ['fedid'] . "'";
		
		$totalutilizado_formacaoeducadores = $db->pegaUm ( $sql );
		// ver($formacaoeducadores['fedpercmax']);
		if (! $formacaoeducadores ['fedpercmax']) {
			$msg [] = "<br>" . $tprdesc . ": A Tela de Formação de Educadores não foi gravada.";
		}
		
		if ($totalutilizado_formacaoeducadores > $fedvlrmaximo) {
			$msg [] = "<br>" . $tprdesc . ": Recursos gastos com a formação é maior que a percentagem prevista.";
		}
		
		if ($totalutilizado_formacaoeducadores > 0 && $montante > 0) {
			$vlr = round ( ($totalutilizado_formacaoeducadores / $montante) * 100, 1 );
		} else {
			$vlr = 0;
		}
		
		if ($_SESSION ['projovemurbano'] ['fedid']) {
			$db->executar ( "UPDATE projovemurbano.formacaoeducadores
	SET fedperutilizado='" . $vlr . "'
	WHERE fedid='" . $_SESSION ['projovemurbano'] ['fedid'] . "'
	AND tprid = {$_SESSION['projovemurbano']['tprid']}" );
		}
		
		$auxiliofinanceiro = $db->pegaLinha ( "SELECT aufavlrauxilio, aufpercmax, (aufqtdeducador*aufavlrauxilio) as total1etapa FROM projovemurbano.auxiliofinanceiro WHERE fedid='" . $_SESSION ['projovemurbano'] ['fedid'] . "' " );
		
		$aufvlrmaximo = round ( ($auxiliofinanceiro ['aufpercmax'] * $montante) / 100, 2 );
		$totalutilizado_auxiliofinanceiro = $auxiliofinanceiro ['total1etapa'];
		
		$educadores_F ['crevlrbrutorem'] = $educadores_F ['crevlrbrutorem'] == 'null' ? '0' : $educadores_F ['crevlrbrutorem'];
		$educadores_Q ['crevlrbrutorem'] = $educadores_Q ['crevlrbrutorem'] == 'null' ? '0' : $educadores_Q ['crevlrbrutorem'];
		$educadores_P ['crevlrbrutorem'] = $educadores_P ['crevlrbrutorem'] == 'null' ? '0' : $educadores_P ['crevlrbrutorem'];
		
		if (round ( $auxiliofinanceiro ['aufavlrauxilio'], 2 ) > round ( ($educadores_F ['crevlrbrutorem'] + $educadores_Q ['crevlrbrutorem'] + $educadores_P ['crevlrbrutorem']), 2 )) {
			$msg [] = "<br>" . $tprdesc . ": Auxílio financeiro a ser pago(R$) maior do que o permitido.";
		}
		
		if ($totalutilizado_auxiliofinanceiro > $aufvlrmaximo) {
			$msg [] = "<br>" . $tprdesc . ": Valor destinado ao pagamento de auxílio financeiro para a primeira etapa da formação é maior que a percentagem prevista.";
		}
		
		if ($totalutilizado_auxiliofinanceiro > 0 && $montante > 0) {
			$vlr = number_format ( ($totalutilizado_auxiliofinanceiro / $montante) * 100, 1 );
		} else {
			$vlr = 0;
		}
		
		if ($_SESSION ['projovemurbano'] ['fedid']) {
			$db->executar ( "UPDATE projovemurbano.auxiliofinanceiro
			SET aufpercutilizado='" . $vlr . "'
			WHERE fedid='" . $_SESSION ['projovemurbano'] ['fedid'] . "'" );
		}
		
		/* FIM validando formação */

/*
* VALIDANDO GENÊRO ALIMENTICIOS
*
* - verifica se a aba genero alimenticios foi gravado pelo menos uma vez;
		* - atualiza o percentual utilizado dos generos alimenticios (caso tenha ocorrido alguma falha)
*
*/

 criaSessaoGeneroAlimenticios ();
		
		$generoalimenticio = $db->pegaLinha ( "SELECT * FROM projovemurbano.generoalimenticio
 		WHERE galid='" . $_SESSION ['projovemurbano'] ['galid'] . "'" );
		
		$lancherefeicao = $db->pegaLinha ( "SELECT * FROM projovemurbano.lancherefeicao WHERE galid='" . $_SESSION ['projovemurbano'] ['galid'] . "'" );
		
		$galvlrmaximo = round ( ($generoalimenticio ['galpercmax'] * $montante) / 100, 2 );
		$totalutilizado_generoalimenticio = $lancherefeicao ['lrevlrtotal'];
		
		if (! $generoalimenticio ['galpercmax']) {
			$msg [] = "<br>" . $tprdesc . ": A Tela de Gêneros Alimenticios não foi gravada.";
		}
		
		if ($totalutilizado_generoalimenticio > $galvlrmaximo) {
			$msg [] = "<br>" . $tprdesc . ": Valor do Lanche ou Refeição é maior que a percentagem prevista.";
		}
		
		if ($totalutilizado_generoalimenticio > 0 && $montante > 0) {
			$vlr = number_format ( ($totalutilizado_generoalimenticio / $montante) * 100, 1 );
		} else {
			$vlr = 0;
		}
		
		if ($_SESSION ['projovemurbano'] ['galid']) {
			$db->executar ( "UPDATE projovemurbano.generoalimenticio
							SET galpercutilizado='" . $vlr . "'
							WHERE galid='" . $_SESSION ['projovemurbano'] ['galid'] . "'" );
		}
		
		/* FIM validando genêro alimenticios */
		
/*
* VALIDANDO QUALIFICAÇÃO PROFISSIONAL
*
* - verifica se a aba qualificação profissional foi gravado pelo menos uma vez;
		* - atualiza o percentual utilizado na qualificação profissional (caso tenha ocorrido alguma falha)
		*
		*/

		criaSessaoQualificacaoProfissional ();
		
		$qualificacaoprofissional = $db->pegaLinha ( "SELECT * FROM projovemurbano.qualificacaoprofissional WHERE qprid='" . $_SESSION ['projovemurbano'] ['qprid'] . "'" );
		
		$qprvlrmaximo = round ( ($qualificacaoprofissional ['qprpercmax'] * $montante) / 100, 2 );
		
		$sql = "SELECT
SUM(pgavlrmes*pgaqtdmeses),
nucid
FROM
projovemurbano.previsaogasto
WHERE
qprid = '" . $_SESSION ['projovemurbano'] ['qprid'] . "'
					AND pgastatus='A'
				GROUP BY
nucid";
		
		$totalutilizado_qualificacaoprofissional = $db->pegaUm ( $sql );
		;
		
		if (! $qualificacaoprofissional ['qprpercmax']) {
			$msg [] = "<br>" . $tprdesc . ": A Tela de Qualificação profissional não foi gravada.";
		}
		
		if ($totalutilizado_qualificacaoprofissional > $qprvlrmaximo) {
			$msg [] = "<br>" . $tprdesc . ": Despesas com qualificação profissional é maior que a percentagem prevista.";
		}
		
		//
		if ($nucleos [0]) {
			foreach ( $nucleos as $n ) {
				$nucleo [] = $n ['nucid'];
			}
		}
		if ($nucleo) {
			$arcids = $db->carregarColuna ( "SELECT arcid FROM projovemurbano.arcoqualificacao WHERE nucid IN('" . implode ( "','", $nucleo ) . "') AND qprid='" . $_SESSION ['projovemurbano'] ['qprid'] . "' AND arqstatus='A'" );
		}
		
		if ($qualificacaoprofissional ['qprarco'] == "t") {
			if ($arcids) {
				if (in_array ( '', $arcids )) {
					$msg [] = "<br>" . $tprdesc . ": Não fizeram vinculação de arco.";
				}
			}
		}
		
		if ($totalutilizado_generoalimenticio > 0 && $montante > 0) {
			$vlr = number_format ( ($totalutilizado_generoalimenticio / $montante) * 100, 1 );
		} else {
			$vlr = 0;
		}
		
		if ($_SESSION ['projovemurbano'] ['qprid']) {
			$db->executar ( "UPDATE projovemurbano.qualificacaoprofissional
		SET qprpercutilizado='" . $vlr . "'
		WHERE qprid='" . $_SESSION ['projovemurbano'] ['qprid'] . "'" );
		}
		
		/* FIM validando qualificação profissional */
		
		if ($_SESSION ['projovemurbano'] ['estuf']) :
			
			/*
			 * VALIDANDO TRANSPORTE DIDATICO - verifica se a aba transporte didatico foi gravado pelo menos uma vez; - atualiza o percentual utilizado em transporte didatico (caso tenha ocorrido alguma falha)
			 */
			
			$sql = "SELECT * FROM projovemurbano.transportematerial WHERE pjuid='" . $_SESSION ['projovemurbano'] ['pjuid'] . "' AND tprid = {$_SESSION['projovemurbano']['tprid']}";
			$transportematerial = $db->pegaLinha ( $sql );
			
			$tmavlrmaximo = round ( ($transportematerial ['tmapercmax'] * $montante) / 100, 2 );
			$totalutilizado_transportematerial = $transportematerial ['tmarecursoutilizado'];
			
			if (! $transportematerial ['tmapercmax']) {
				$msg [] = "<br>" . $tprdesc . ": A Tela de Transporte Didático não foi gravada.";
			}
			
			if ($totalutilizado_transportematerial > $tmavlrmaximo) {
				$msg [] = "<br>" . $tprdesc . ": Recursos Utilizados em transporte didático é maior que a percentagem prevista.";
			}
			
			if ($transportematerial ['tmaid']) {
				$db->executar ( "UPDATE projovemurbano.transportematerial
				SET tmaperutilizado='" . number_format ( ($totalutilizado_transportematerial / $montante) * 100, 1 ) . "'
							WHERE tmaid='" . $transportematerial ['tmaid'] . "'" );
			}
		
		
	
/* FIM validando transporte didatico */
	
		endif;
		
		/*
		 * VALIDANDO DEMAIS AÇÕES - verifica se a aba demais ações foi gravado pelo menos uma vez; - atualiza o percentual utilizado em demais ações (caso tenha ocorrido alguma falha)
		 */
		
		criaSessaoDemaisAcoes ();
		
		$totalutilizado_profissionais = $totalutilizado_profissionais > 0 ? $totalutilizado_profissionais : '0';
		$totalutilizado_formacaoeducadores = $totalutilizado_formacaoeducadores > 0 ? $totalutilizado_formacaoeducadores : '0';
		$totalutilizado_auxiliofinanceiro = $totalutilizado_auxiliofinanceiro > 0 ? $totalutilizado_auxiliofinanceiro : '0';
		$totalutilizado_generoalimenticio = $totalutilizado_generoalimenticio > 0 ? $totalutilizado_generoalimenticio : '0';
		$totalutilizado_qualificacaoprofissional = $totalutilizado_qualificacaoprofissional > 0 ? $totalutilizado_qualificacaoprofissional : '0';
		$totalutilizado_transportematerial = $totalutilizado_transportematerial > 0 ? $totalutilizado_transportematerial : '0';
		
		$totalmaxdemaisacoes = $montante - ($totalutilizado_profissionais + $totalutilizado_formacaoeducadores + $totalutilizado_auxiliofinanceiro + $totalutilizado_generoalimenticio + $totalutilizado_qualificacaoprofissional + $totalutilizado_transportematerial);
		$totalmaxdemaisacoes = $totalmaxdemaisacoes > 0 ? $totalmaxdemaisacoes : 0;
		
		// Gatilho pra não precisar refazer todos os calculos, reaproveitando o código
		
		if ($retornarTotalMaximoDemaisAcoes)
			return $totalmaxdemaisacoes;
		
		$totalutilizado_demaisacoes = $db->pegaUm ( "SELECT SUM(idaqtdmeses*idavlrmes) as x FROM projovemurbano.itemdemaisacoes WHERE deaid='" . $_SESSION ['projovemurbano'] ['deaid'] . "'" );
		
		$totalUtil_demaisacoes = str_replace ( '.', '', number_format ( $totalutilizado_demaisacoes, 2, '', '.' ) );
		$totalMax_demaisacoes = str_replace ( '.', '', number_format ( $totalmaxdemaisacoes, 2, '', '.' ) );
		
		if ($totalUtil_demaisacoes > $totalMax_demaisacoes) {
			$msg [] = "<br>" . $tprdesc . ": Gastos com Demais ações é maior que a percentagem prevista.";
		}
		
		if ($totalutilizado_demaisacoes > 0 && $montante > 0) {
			$vlr = number_format ( ($totalutilizado_demaisacoes / $montante) * 100, 1 );
		} else {
			$vlr = 0;
		}
		
		if ($_SESSION ['projovemurbano'] ['deaid']) {
			$db->executar ( "UPDATE projovemurbano.demaisacoes
		SET deapercutilizado='" . $vlr . "'
		WHERE deaid='" . $_SESSION ['projovemurbano'] ['deaid'] . "'" );
		}
		
		/* FIM validando demais ações */
		$sql = "
            SELECT true FROM seguranca.usuario u
            INNER JOIN projovemurbano.enderecoentregadematerial i ON i.eemcpfresponsavel = u.usucpf
            WHERE i.pjuid='" . $_SESSION ['projovemurbano'] ['pjuid'] . "'";
		$enderecoentrega = $db->pegaUm ( $sql );
		
		if (! $enderecoentrega) {
			$msg [] = "<br>A Tela de Endereço Entrega Mat. Didático não foi gravada.";
		}
		
		/* FIM validando Endereço de Entrega */
		
		$db->commit ();
		
		// return "Validação completa ainda não esta disponivel. Em breve poderá enviar o Plano de Implementação para analise do MEC";
		
		// if(date("Y-m-d")>"2012-01-23") return "Prazo para envio do Plano de implementação terminou. Obrigado!";
	}
	
	/* VALIDANDO Endereço de Entrega */
	unset ( $_SESSION ['projovemurbano'] ['tprid'] );
	
	if ($tpridTemp != '') {
		$_SESSION ['projovemurbano'] ['tprid'] = $tpridTemp;
	}
	
	return $msg;
}
function validacaoCompletaPlanoImplementacao() {
	global $db, $retornarTotalMaximoDemaisAcoes;
	if ($_SESSION ['projovemurbano'] ['ppuid'] == '3') {
		$msg = validacaoCompletaPlanoImplementacao2014 ();
	} elseif ($_SESSION ['projovemurbano'] ['ppuid'] == '2') {
		$msg = validacaoCompletaPlanoImplementacao2013 ();
	} else {
		if ($_SESSION ['projovemurbano'] ['muncod']) {
			$sugestaoampliacao = $db->pegaLinha ( "SELECT suaverdade, suametaajustada FROM projovemurbano.sugestaoampliacao WHERE pjuid='" . $_SESSION ['projovemurbano'] ['pjuid'] . "'" );
			$meta = $db->pegaUm ( "SELECT cmemeta FROM projovemurbano.cargameta WHERE cmecodibge='" . $_SESSION ['projovemurbano'] ['muncod'] . "' AND ppuid = '" . $_SESSION ['projovemurbano'] ['ppuid'] . "'" );
			if ($sugestaoampliacao ['suaverdade'] == "t") {
				if ($sugestaoampliacao ['suametaajustada'])
					$meta = $sugestaoampliacao ['suametaajustada'];
			}
		}
		
		if ($_SESSION ['projovemurbano'] ['estuf']) {
			$sugestaoampliacao = $db->pegaLinha ( "SELECT suaverdade, suametaajustada FROM projovemurbano.sugestaoampliacao WHERE pjuid='" . $_SESSION ['projovemurbano'] ['pjuid'] . "'" );
			$meta = $db->pegaUm ( "SELECT cmemeta FROM projovemurbano.cargameta c INNER JOIN territorios.estado e ON e.estcod::numeric=c.cmecodibge WHERE c.cmetipo='E' AND e.estuf='" . $_SESSION ['projovemurbano'] ['estuf'] . "' AND c.ppuid = '" . $_SESSION ['projovemurbano'] ['ppuid'] . "'" );
			if ($sugestaoampliacao ['suaverdade'] == "t") {
				if ($sugestaoampliacao ['suametaajustada'])
					$meta = $sugestaoampliacao ['suametaajustada'];
			}
		}
		
		/*
		 * VALIDANDO NÚMERO DE NÚCLEOS Se o núcleo for igual a 1 (um), o nº de alunos deve ser necessariamente 200. Se o nº de núcleo for maior que 1 (um), o nº de alunos no núcleo poderá variar entre 150 a 200.
		 */
		
		$sql = "SELECT pmupossuipolo FROM projovemurbano.polomunicipio WHERE pjuid='" . $_SESSION ['projovemurbano'] ['pjuid'] . "'";
		$pmupossuipolo = $db->pegaUm ( $sql );
		
		if ($pmupossuipolo == "t") {
			
			$nucleos = $db->carregar ( "SELECT mun.munid, nuc.nucid, nuc.nucqtdestudantes FROM projovemurbano.nucleo nuc
									  INNER JOIN projovemurbano.municipio mun ON mun.munid = nuc.munid 
									  INNER JOIN projovemurbano.associamucipiopolo amp ON amp.munid = mun.munid    
									  INNER JOIN projovemurbano.polo pol ON pol.polid = amp.polid 
									  INNER JOIN projovemurbano.polomunicipio plm ON plm.pmuid = pol.pmuid 
									  WHERE nuc.nucstatus='A' AND mun.munstatus='A' AND plm.pmustatus='A' 
									  		AND pol.polstatus='A' AND pjuid='" . $_SESSION ['projovemurbano'] ['pjuid'] . "'" );
		} else {
			
			$nucleos = $db->carregar ( "SELECT mun.munid, nuc.nucid, nuc.nucqtdestudantes FROM projovemurbano.nucleo nuc
									  INNER JOIN projovemurbano.municipio mun ON mun.munid = nuc.munid 
									  INNER JOIN projovemurbano.polomunicipio plm ON plm.pmuid = mun.pmuid 
									  WHERE 
									  		nuc.nucstatus='A' AND mun.munstatus='A' AND plm.pmustatus='A' 
									  		AND pjuid='" . $_SESSION ['projovemurbano'] ['pjuid'] . "'" );
		}
		
		$totalestudantes = 0;
		if ($nucleos [0]) {
			foreach ( $nucleos as $nucleo ) {
				$_CHK [$nucleo ['munid']] [$nucleo ['nucid']] = $nucleo ['nucqtdestudantes'];
				$totalestudantes += $nucleo ['nucqtdestudantes'];
			}
		} else {
			$msg [] = "Não existe núcleo não cadastrado.";
		}
		
		if ($totalestudantes != $meta) {
			$msg [] = "Quantidade de estudantes/Núcleo diferente da meta (Meta:" . $meta . ",Utilizado:" . $totalestudantes . ").";
		}
		
		if ($_CHK) {
			foreach ( array_keys ( $_CHK ) as $munid ) {
				if (count ( $_CHK [$munid] ) == 1) {
					$qtde = current ( $_CHK [$munid] );
					if ($qtde != '200') {
						$msg [] = "Se o núcleo for igual a 1 (um), o nº de alunos deve ser necessariamente 200.";
					}
				}
			}
		}
		
		/* FIM validando número de núcleos */
		
		$montante = calcularMontante ( $meta );
		
		/*
		 * VALIDANDO PROFISSIONAIS - verifica se a aba profissionais foi gravado pelo menos uma vez; - verifica se o valor total de profissionais é maior que o percentual previsto; - atualiza o percentual utilizado (caso tenha ocorrido alguma falha)
		 */
		
		criaSessaoProfissionais ();
		// ver($montante,d);
		$profissionais = $db->pegaLinha ( "SELECT * FROM projovemurbano.profissionais WHERE proid='" . $_SESSION ['projovemurbano'] ['proid'] . "'" );
		
		$coordgeral = pegarCoordenadorGeral ( $_SESSION ['projovemurbano'] ['proid'] );
		$assistenteadministrativo_A = pegarAssistentesCoordenadorGeral ( $_SESSION ['projovemurbano'] ['proid'], 'A' );
		$assistenteadministrativo_P = pegarAssistentesCoordenadorGeral ( $_SESSION ['projovemurbano'] ['proid'], 'P' );
		$coordassistentes = array (
				"coaqtd" => $assistenteadministrativo_A ['coaqtd'] + $assistenteadministrativo_P ['coaqtd'],
				"coavlrtotal" => $assistenteadministrativo_A ['coavlrtotal'] + $assistenteadministrativo_P ['coavlrtotal'] 
		);
		$diretorpolo = pegarDiretorPolo ( $_SESSION ['projovemurbano'] ['proid'] );
		$dirassistentes_A = pegarAssistentesDiretorPolo ( $_SESSION ['projovemurbano'] ['proid'], 'A' );
		$dirassistentes_P = pegarAssistentesDiretorPolo ( $_SESSION ['projovemurbano'] ['proid'], 'P' );
		$dirassistentes = array (
				"dasqtdefetivo40hr" => $dirassistentes_A ['dasqtdefetivo40hr'] + $dirassistentes_P ['dasqtdefetivo40hr'],
				"dasqtdrecursoproprio" => $dirassistentes_A ['dasqtdrecursoproprio'] + $dirassistentes_P ['dasqtdrecursoproprio'],
				"creqtd" => $dirassistentes_A ['creqtd'] + $dirassistentes_P ['creqtd'],
				"crevlrtotal" => $dirassistentes_A ['crevlrtotal'] + $dirassistentes_P ['crevlrtotal'] 
		);
		
		$educadores_F = pegarEducadores ( $_SESSION ['projovemurbano'] ['proid'], 'F' );
		$educadores_Q = pegarEducadores ( $_SESSION ['projovemurbano'] ['proid'], 'Q' );
		$educadores_P = pegarEducadores ( $_SESSION ['projovemurbano'] ['proid'], 'P' );
		$educadores_M = pegarEducadores ( $_SESSION ['projovemurbano'] ['proid'], 'M' );
		$educadores_T = pegarEducadores ( $_SESSION ['projovemurbano'] ['proid'], 'T' );
		$educadores_E = pegarEducadores ( $_SESSION ['projovemurbano'] ['proid'], 'E' );
		
		$provlrmaximo = round ( ($profissionais ['propercmax'] * $montante) / 100, 2 );
		$totalutilizado_profissionais = round ( $coordgeral ['cgevlrtotal'] + $coordassistentes ['coavlrtotal'] + $diretorpolo ['ccmvlrtotal'] + $educadores_F ['ccmvlrtotal'] + $educadores_Q ['ccmvlrtotal'] + $educadores_P ['ccmvlrtotal'] + $dirassistentes ['crevlrtotal'] + $educadores_F ['crevlrtotal'] + $educadores_Q ['crevlrtotal'] + $educadores_P ['crevlrtotal'] + $educadores_M ['crevlrtotal'] + $educadores_T ['crevlrtotal'], 2 );
		
		if (! $profissionais ['propercmax']) {
			$msg [] = "A Tela de Profissionais não foi gravada.";
		}
		if ($totalutilizado_profissionais > $provlrmaximo) {
			$msg [] = "O valor utilizado em profissionais é maior que a percentagem prevista.";
		}
		
		if ($montante == 0) {
			return false;
		}
		if ($_SESSION ['projovemurbano'] ['proid']) {
			$db->executar ( "UPDATE projovemurbano.profissionais 
							SET propercutilizado='" . round ( ($totalutilizado_profissionais / $montante) * 100, 1 ) . "' 
							WHERE proid='" . $_SESSION ['projovemurbano'] ['proid'] . "'" );
		}
		
		/* FIM validando profissionais */
		
		/* 
		 * VALIDANDO FORMAÇÃO 
		 * 
		 * - verifica se a aba formação de educadores foi gravado pelo menos uma vez;
		 * - verifica se o valor total dos recursos com a formação é maior que o percentual previsto;
		 * - atualiza o percentual utilizado dos recursos gastos com formação (caso tenha ocorrido alguma falha)
		 * - verifica se o valor total dos recursos com a formação é maior que o percentual previsto;
		 * - atualiza o percentual utilizado dos recursos gastos com formação (caso tenha ocorrido alguma falha)  
		 *    
		 */
		
		criaSessaoFormacaoEducadores ();
		
		$sql = "SELECT * FROM projovemurbano.formacaoeducadores
                          WHERE fedid='" . $_SESSION ['projovemurbano'] ['fedid'] . "'";
		$formacaoeducadores = $db->pegaLinha ( $sql );
		
		$fedvlrmaximo = round ( ($formacaoeducadores ['fedpercmax'] * $montante) / 100, 2 );
		
		$sql = "SELECT SUM(rgavalor) FROM projovemurbano.recursosgastos    
				WHERE rgastatus='A' AND fedid='" . $_SESSION ['projovemurbano'] ['fedid'] . "'";
		
		$totalutilizado_formacaoeducadores = $db->pegaUm ( $sql );
		
		if (! $formacaoeducadores ['fedpercmax']) {
			$msg [] = "A Tela de Formação de Educadores não foi gravada.";
		}
		
		if ($totalutilizado_formacaoeducadores > $fedvlrmaximo) {
			$msg [] = "Recursos gastos com a formação é maior que a percentagem prevista.";
		}
		
		if ($_SESSION ['projovemurbano'] ['fedid']) {
			$db->executar ( "UPDATE projovemurbano.formacaoeducadores 
							SET fedperutilizado='" . round ( ($totalutilizado_formacaoeducadores / $montante) * 100, 1 ) . "' 
							WHERE fedid='" . $_SESSION ['projovemurbano'] ['fedid'] . "'
                                                          AND tprid = {$_SESSION['projovemurbano']['tprid']}" );
		}
		
		$auxiliofinanceiro = $db->pegaLinha ( "SELECT aufavlrauxilio, aufpercmax, (aufqtdeducador*aufavlrauxilio) as total1etapa FROM projovemurbano.auxiliofinanceiro WHERE fedid='" . $_SESSION ['projovemurbano'] ['fedid'] . "'" );
		
		$aufvlrmaximo = round ( ($auxiliofinanceiro ['aufpercmax'] * $montante) / 100, 2 );
		$totalutilizado_auxiliofinanceiro = $auxiliofinanceiro ['total1etapa'];
		
		if ($auxiliofinanceiro ['aufavlrauxilio'] > round ( ($educadores_F ['crevlrbrutorem'] + $educadores_Q ['crevlrbrutorem'] + $educadores_P ['crevlrbrutorem']) / 3, 2 )) {
			$msg [] = "Auxílio financeiro a ser pago(R$) maior do que o permitido.";
		}
		
		if ($totalutilizado_auxiliofinanceiro > $aufvlrmaximo) {
			$msg [] = "Valor destinado ao pagamento de auxílio financeiro para a primeira etapa da formação é maior que a percentagem prevista.";
		}
		
		if ($_SESSION ['projovemurbano'] ['fedid']) {
			$db->executar ( "UPDATE projovemurbano.auxiliofinanceiro 
							SET aufpercutilizado='" . number_format ( ($totalutilizado_auxiliofinanceiro / $montante) * 100, 1 ) . "' 
							WHERE fedid='" . $_SESSION ['projovemurbano'] ['fedid'] . "'" );
		}
		
		/* FIM validando formação */
		
		/* 
		 * VALIDANDO GENÊRO ALIMENTICIOS 
		 * 
		 * - verifica se a aba genero alimenticios foi gravado pelo menos uma vez;
		 * - atualiza o percentual utilizado dos generos alimenticios (caso tenha ocorrido alguma falha)  
		 *  
		*/
		
		criaSessaoGeneroAlimenticios ();
		
		$generoalimenticio = $db->pegaLinha ( "SELECT * FROM projovemurbano.generoalimenticio WHERE galid='" . $_SESSION ['projovemurbano'] ['galid'] . "'" );
		
		$lancherefeicao = $db->pegaLinha ( "SELECT * FROM projovemurbano.lancherefeicao WHERE galid='" . $_SESSION ['projovemurbano'] ['galid'] . "'" );
		
		$galvlrmaximo = round ( ($generoalimenticio ['galpercmax'] * $montante) / 100, 2 );
		$totalutilizado_generoalimenticio = $lancherefeicao ['lrevlrtotal'];
		
		if (! $generoalimenticio ['galpercmax']) {
			$msg [] = "A Tela de Gêneros Alimenticios não foi gravada.";
		}
		
		if ($totalutilizado_generoalimenticio > $galvlrmaximo) {
			$msg [] = "Valor do Lanche ou Refeição é maior que a percentagem prevista.";
		}
		
		if ($_SESSION ['projovemurbano'] ['galid']) {
			$db->executar ( "UPDATE projovemurbano.generoalimenticio 
							SET galpercutilizado='" . number_format ( ($totalutilizado_generoalimenticio / $montante) * 100, 1 ) . "' 
							WHERE galid='" . $_SESSION ['projovemurbano'] ['galid'] . "'" );
		}
		
		/* FIM validando genêro alimenticios */
		
		/*
		 * VALIDANDO QUALIFICAÇÃO PROFISSIONAL 
		 * 
		 * - verifica se a aba qualificação profissional foi gravado pelo menos uma vez;
		 * - atualiza o percentual utilizado na qualificação profissional (caso tenha ocorrido alguma falha)  
		 *   
		 */
		
		criaSessaoQualificacaoProfissional ();
		
		$qualificacaoprofissional = $db->pegaLinha ( "SELECT * FROM projovemurbano.qualificacaoprofissional WHERE qprid='" . $_SESSION ['projovemurbano'] ['qprid'] . "'" );
		
		$qprvlrmaximo = round ( ($qualificacaoprofissional ['qprpercmax'] * $montante) / 100, 2 );
		$totalutilizado_qualificacaoprofissional = $db->pegaUm ( "SELECT SUM(pgavlrmes*pgaqtdmeses) FROM projovemurbano.previsaogasto WHERE qprid='" . $_SESSION ['projovemurbano'] ['qprid'] . "' AND pgastatus='A'" );
		;
		
		if (! $qualificacaoprofissional ['qprpercmax']) {
			$msg [] = "A Tela de Qualificação profissional não foi gravada.";
		}
		
		if ($totalutilizado_qualificacaoprofissional > $qprvlrmaximo) {
			$msg [] = "Despesas com qualificação profissional é maior que a percentagem prevista.";
		}
		
		//
		if ($nucleos [0]) {
			foreach ( $nucleos as $n ) {
				$nucleo [] = $n ['nucid'];
			}
		}
		if ($nucleo) {
			$arcids = $db->carregarColuna ( "SELECT arcid FROM projovemurbano.arcoqualificacao WHERE nucid IN('" . implode ( "','", $nucleo ) . "') AND qprid='" . $_SESSION ['projovemurbano'] ['qprid'] . "' AND arqstatus='A'" );
		}
		
		if ($qualificacaoprofissional ['qprarco'] == "t") {
			if ($arcids) {
				if (in_array ( '', $arcids )) {
					$msg [] = "Não fizeram vinculação de arco.";
				}
			}
		}
		//
		
		if ($_SESSION ['projovemurbano'] ['qprid']) {
			$db->executar ( "UPDATE projovemurbano.qualificacaoprofissional 
							SET qprpercutilizado='" . number_format ( ($totalutilizado_qualificacaoprofissional / $montante) * 100, 1 ) . "' 
							WHERE qprid='" . $_SESSION ['projovemurbano'] ['qprid'] . "'" );
		}
		
		/* FIM validando qualificação profissional */
		
		if ($_SESSION ['projovemurbano'] ['estuf']) :
			
			/*
			 * VALIDANDO TRANSPORTE DIDATICO - verifica se a aba transporte didatico foi gravado pelo menos uma vez; - atualiza o percentual utilizado em transporte didatico (caso tenha ocorrido alguma falha)
			 */
			
			$sql = "SELECT * FROM projovemurbano.transportematerial WHERE pjuid='" . $_SESSION ['projovemurbano'] ['pjuid'] . "'";
			$transportematerial = $db->pegaLinha ( $sql );
			
			$tmavlrmaximo = round ( ($transportematerial ['tmapercmax'] * $montante) / 100, 2 );
			$totalutilizado_transportematerial = $transportematerial ['tmarecursoutilizado'];
			
			if (! $transportematerial ['tmapercmax']) {
				$msg [] = "A Tela de Transporte Didático não foi gravada.";
			}
			
			if ($totalutilizado_transportematerial > $tmavlrmaximo) {
				$msg [] = "Recursos Utilizados em transporte didático é maior que a percentagem prevista.";
			}
			
			if ($transportematerial ['tmaid']) {
				$db->executar ( "UPDATE projovemurbano.transportematerial 
								SET tmaperutilizado='" . number_format ( ($totalutilizado_transportematerial / $montante) * 100, 1 ) . "' 
								WHERE tmaid='" . $transportematerial ['tmaid'] . "'" );
			}
		
		
			
			/* FIM validando transporte didatico */
			
		endif;
		
		/*
		 * VALIDANDO DEMAIS AÇÕES - verifica se a aba demais ações foi gravado pelo menos uma vez; - atualiza o percentual utilizado em demais ações (caso tenha ocorrido alguma falha)
		 */
		
		criaSessaoDemaisAcoes ();
		
		$totalmaxdemaisacoes = $montante - ($totalutilizado_profissionais + $totalutilizado_formacaoeducadores + $totalutilizado_auxiliofinanceiro + $totalutilizado_generoalimenticio + $totalutilizado_qualificacaoprofissional + $totalutilizado_transportematerial);
		
		// Gatilho pra não precisar refazer todos os calculos, reaproveitando o código
		if ($retornarTotalMaximoDemaisAcoes)
			return $totalmaxdemaisacoes;
		
		$totalutilizado_demaisacoes = $db->pegaUm ( "SELECT SUM(idaqtdmeses*idavlrmes) as x FROM projovemurbano.itemdemaisacoes WHERE deaid='" . $_SESSION ['projovemurbano'] ['deaid'] . "'" );
		
		if ($totalutilizado_demaisacoes > $totalmaxdemaisacoes) {
			$msg [] = "Gastos com Demais ações é maior que a percentagem prevista.";
		}
		
		if ($_SESSION ['projovemurbano'] ['deaid']) {
			$db->executar ( "UPDATE projovemurbano.demaisacoes 
							SET deapercutilizado='" . number_format ( ($totalutilizado_demaisacoes / $montante) * 100, 1 ) . "' 
							WHERE deaid='" . $_SESSION ['projovemurbano'] ['deaid'] . "'" );
		}
		
		/* FIM validando demais ações */
		
		$db->commit ();
		
		// return "Validação completa ainda não esta disponivel. Em breve poderá enviar o Plano de Implementação para analise do MEC";
		
		// if(date("Y-m-d")>"2012-01-23") return "Prazo para envio do Plano de implementação terminou. Obrigado!";
	}
	if ($retornarTotalMaximoDemaisAcoes)
		return $msg;
	
	if ($msg) {
		return "Ainda constam algumas pendências no plano de implementação:" . '\n\n' . implode ( '\n', $msg );
	} else {
		return true;
	}
}
function criaDocumento() {
	global $db;
	
	$esdid = $db->pegaUm ( "SELECT esdid FROM workflow.estadodocumento WHERE tpdid='" . TPD_PROJOVEMURBANO . "' ORDER BY esdordem ASC LIMIT 1" );
	
	$sql = "INSERT INTO workflow.documento(
            tpdid, esdid, docdsc)
    		VALUES ('" . TPD_PROJOVEMURBANO . "', '" . $esdid . "', 'Projovem Urbano " . $_SESSION ['projovemurbano'] ['pjuid'] . "') RETURNING docid;";
	
	$docid = $db->pegaUm ( $sql );
	$db->commit ();
	
	return $docid;
}
function listaApoioReferenciaFormacao($dados) {
	global $db;
	
	$sql = "SELECT DISTINCT refdesc,refunidademedida,refvalor FROM projovemurbano.apoioreferenciaformacao WHERE refstatus='A' ORDER BY refdesc";
	$cabecalho = array (
			"Descrição",
			"Unidade",
			"Valor(R$)" 
	);
	
	echo "<script language='JavaScript' src='../includes/funcoes.js'></script>
		  <link rel='stylesheet' type='text/css' href='../includes/Estilo.css'/>
		  <link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>";
	
	$db->monta_lista_simples ( $sql, $cabecalho, 50, 5, 'N', '100%', $par2 );
}
function excluirNucleo($dados) {
	global $db;
	
	$db->executar ( "DELETE FROM projovemurbano.educadores WHERE proid IN(SELECT proid FROM projovemurbano.profissionais 
																		WHERE pjuid='" . $_SESSION ['projovemurbano'] ['pjuid'] . "' AND tprid = {$_SESSION['projovemurbano']['tprid']})" );
	
	$db->executar ( "UPDATE projovemurbano.nucleo SET nucstatus='I' WHERE nucid='" . $dados ['nucid'] . "'" );
	$db->commit ();
	$db->executar ( "UPDATE projovemurbano.nucleoescola SET nuestatus='I' WHERE nucid='" . $dados ['nucid'] . "'" );
	$db->commit ();
	$db->executar ( "UPDATE projovemurbano.turma SET turstatus='I' WHERE nucid='" . $dados ['nucid'] . "'" );
	$db->commit ();
	$db->executar ( "UPDATE projovemurbano.cadastroestudante SET caestatus='I' WHERE nucid='" . $dados ['nucid'] . "'" );
	$db->commit ();
	
	if ($_SESSION ['projovemurbano'] ['ppuid'] == '2') {
		$link = "projovemurbano.php?modulo=principal/planoImplementacao2013&acao=A&aba={$_REQUEST['aba']}&aba2=poloNucleo&aba3=poloNucleoGerenciar&munid=" . $dados ['munid'];
	} elseif ($_SESSION ['projovemurbano'] ['ppuid'] == '3') {
		$link = "projovemurbano.php?modulo=principal/indexPoloNucleo2014&acao=A&aba={$_REQUEST['aba']}&aba2=poloNucleo&aba3=poloNucleoGerenciar&munid=" . $dados ['munid'];
	} else {
		$link = "projovemurbano.php?modulo=principal/planoImplementacao&acao=A&aba=poloNucleo&aba2=poloNucleoGerenciar&munid=" . $dados ['munid'];
	}
	if ($_SESSION ['projovemurbano'] ['ppuid'] != 3) {
		echo "<script>
				alert('Nucleo excluido com sucesso, as abas de profisionais e formação serão modificadas');
				window.location='$link';
			  </script>";
	} else {
		echo "<script>
				alert('Nucleo excluido com sucesso.');
				window.location='$link';
			  </script>";
	}
}
function excluirPolo($dados) {
	global $db;
	
	$db->executar ( "DELETE FROM projovemurbano.educadores WHERE proid IN(
					SELECT proid FROM projovemurbano.profissionais WHERE pjuid='" . $_SESSION ['projovemurbano'] ['pjuid'] . "' AND tprid = {$_SESSION['projovemurbano']['tprid']})" );
	$db->executar ( "DELETE FROM projovemurbano.dirassistentes WHERE proid IN(
					SELECT proid FROM projovemurbano.profissionais WHERE pjuid='" . $_SESSION ['projovemurbano'] ['pjuid'] . "' AND tprid = {$_SESSION['projovemurbano']['tprid']})" );
	$db->executar ( "DELETE FROM projovemurbano.diretorpolo WHERE proid IN(
					SELECT proid FROM projovemurbano.profissionais WHERE pjuid='" . $_SESSION ['projovemurbano'] ['pjuid'] . "' AND tprid = {$_SESSION['projovemurbano']['tprid']})" );
	
	$db->executar ( "UPDATE projovemurbano.municipio SET munstatus='I' WHERE munid IN(
					SELECT munid FROM projovemurbano.associamucipiopolo WHERE polid='" . $dados ['polid'] . "')" );
	$db->executar ( "UPDATE projovemurbano.polo SET polstatus='I' WHERE polid='" . $dados ['polid'] . "'" );
	
	$db->executar ( " UPDATE projovemurbano.nucleo 
					SET nucstatus='I' 
					WHERE nucid IN 
						(
						SELECT nuc.nucid 
						FROM projovemurbano.nucleo nuc
						INNER JOIN projovemurbano.municipio mu ON mu.munid = nuc.munid
						INNER JOIN territorios.municipio mun ON mu.muncod = mun.muncod
						WHERE nuc.munid in (SELECT munid FROM projovemurbano.associamucipiopolo WHERE polid = " . $dados ['polid'] . ")
						)" );
	$db->executar ( " UPDATE projovemurbano.nucleoescola 
					SET nuestatus='I' 
					WHERE nucid IN 
						(
						SELECT nuc.nucid 
						FROM projovemurbano.nucleo nuc
						INNER JOIN projovemurbano.municipio mu ON mu.munid = nuc.munid
						INNER JOIN territorios.municipio mun ON mu.muncod = mun.muncod
						WHERE nuc.munid in (SELECT munid FROM projovemurbano.associamucipiopolo WHERE polid = " . $dados ['polid'] . ")
						)" );
	$db->commit ();
	$db->executar ( " UPDATE projovemurbano.turma 
					SET turstatus='I' 
					WHERE nucid IN 
						(
						SELECT nuc.nucid 
						FROM projovemurbano.nucleo nuc
						INNER JOIN projovemurbano.municipio mu ON mu.munid = nuc.munid
						INNER JOIN territorios.municipio mun ON mu.muncod = mun.muncod
						WHERE nuc.munid in (SELECT munid FROM projovemurbano.associamucipiopolo WHERE polid = " . $dados ['polid'] . ")
						)" );
	$db->commit ();
	$db->executar ( " UPDATE projovemurbano.cadastroestudante 
					SET caestatus='I' 
					WHERE nucid IN 
						(
						SELECT nuc.nucid 
						FROM projovemurbano.nucleo nuc
						INNER JOIN projovemurbano.municipio mu ON mu.munid = nuc.munid
						INNER JOIN territorios.municipio mun ON mu.muncod = mun.muncod
						WHERE nuc.munid in (SELECT munid FROM projovemurbano.associamucipiopolo WHERE polid = " . $dados ['polid'] . ")
						)" );
	$db->commit ();
	
	if ($_SESSION ['projovemurbano'] ['ppuid'] == '2') {
		$link = "projovemurbano.php?modulo=principal/planoImplementacao2013&acao=A&aba={$_REQUEST['aba']}&aba2=poloNucleo&aba3=poloNucleoCadastro";
	} elseif ($_SESSION ['projovemurbano'] ['ppuid'] == '3') {
		$link = "projovemurbano.php?modulo=principal/indexPoloNucleo2014&acao=A&aba={$_REQUEST['aba']}&aba2=poloNucleo&aba3=poloNucleoCadastro";
	} else {
		$link = "projovemurbano.php?modulo=principal/planoImplementacao&acao=A&aba=poloNucleo&aba2=poloNucleoCadastro";
	}
	
	echo "<script>
			alert('Polo excluido com sucesso');
			window.location='$link';
		  </script>";
}
function excluirMunicipio($dados) {
	global $db;
	
	$db->executar ( "UPDATE projovemurbano.municipio SET munstatus='I' WHERE munid='" . $dados ['munid'] . "'" );
	
	$db->executar ( " UPDATE projovemurbano.nucleo 
					SET nucstatus='I' 
					WHERE nucid IN 
						(
						SELECT nuc.nucid 
						FROM projovemurbano.nucleo nuc
						INNER JOIN projovemurbano.municipio mu ON mu.munid = nuc.munid
						INNER JOIN territorios.municipio mun ON mu.muncod = mun.muncod
						WHERE nuc.munid = " . $dados ['munid'] . "
						)" );
	$db->executar ( " UPDATE projovemurbano.nucleoescola 
					SET nuestatus='I' 
					WHERE nucid IN 
						(
						SELECT nuc.nucid 
						FROM projovemurbano.nucleo nuc
						INNER JOIN projovemurbano.municipio mu ON mu.munid = nuc.munid
						INNER JOIN territorios.municipio mun ON mu.muncod = mun.muncod
						WHERE nuc.munid = " . $dados ['munid'] . "
						)" );
	$db->commit ();
	$db->executar ( " UPDATE projovemurbano.turma 
					SET turstatus='I' 
					WHERE nucid IN 
						(
						SELECT nuc.nucid 
						FROM projovemurbano.nucleo nuc
						INNER JOIN projovemurbano.municipio mu ON mu.munid = nuc.munid
						INNER JOIN territorios.municipio mun ON mu.muncod = mun.muncod
						WHERE nuc.munid = " . $dados ['munid'] . "
						)" );
	$db->commit ();
	$db->executar ( " UPDATE projovemurbano.cadastroestudante 
					SET caestatus='I' 
					WHERE nucid IN 
						(
						SELECT nuc.nucid 
						FROM projovemurbano.nucleo nuc
						INNER JOIN projovemurbano.municipio mu ON mu.munid = nuc.munid
						INNER JOIN territorios.municipio mun ON mu.muncod = mun.muncod
						WHERE nuc.munid = " . $dados ['munid'] . "
						)" );
	
	$db->commit ();
	
	if ($_SESSION ['projovemurbano'] ['ppuid'] == '2') {
		$link = "projovemurbano.php?modulo=principal/planoImplementacao2013&acao=A&aba={$_GET['aba']}&aba2=poloNucleo&aba3=poloNucleoCadastro";
	} elseif ($_SESSION ['projovemurbano'] ['ppuid'] == '3') {
		$link = "projovemurbano.php?modulo=principal/indexPoloNucleo2014&acao=A&aba={$_REQUEST['aba']}&aba2=poloNucleo&aba3=poloNucleoCadastro";
	} else {
		$link = "projovemurbano.php?modulo=principal/planoImplementacao&acao=A&aba=poloNucleo&aba2=poloNucleoCadastro";
	}
	
	echo "<script>
			alert('Município excluido com sucesso');
			window.location='$link';
		  </script>";
}
function pegaValorPercapta() {
	global $db;
	
	if ($_SESSION ['projovemurbano'] ['muncod']) {
		$sql = "SELECT
					tprvlrpercaptamunicipal
				FROM
					projovemurbano.tipoprograma
				WHERE
					tprid = {$_SESSION['projovemurbano']['tprid']}";
	} else {
		$sql = "SELECT
					tprvlrpercaptaestadual
				FROM
					projovemurbano.tipoprograma
				WHERE
					tprid = {$_SESSION['projovemurbano']['tprid']}";
	}
	return $db->pegaUm ( $sql );
}
function calcularMontante($meta = 0) {
	global $db;
	
	if ($_SESSION ['projovemurbano'] ['ppuid'] != 1) {
		// ver(d);
		$vlr = pegaValorPercapta ();
		if ($vlr != '') {
			return $vlr * $meta * 18;
		}
	} else {
		if ($_SESSION ['projovemurbano'] ['muncod'])
			return ((165 * $meta * 18));
		if ($_SESSION ['projovemurbano'] ['estuf'])
			return ((170 * $meta * 18));
	}
}
function inserirMunicipio($dados) {
	global $db;
	
	if ($dados ['muncod']) {
		foreach ( $dados ['muncod'] as $muncod ) {
			$sql = "INSERT INTO projovemurbano.municipio(
		            muncod, pmuid, munstatus)
		    		VALUES ('" . $muncod . "', '" . $dados ['pmuid'] . "', 'A');";
			$db->executar ( $sql );
		}
	}
	
	$db->commit ();
	
	if ($_SESSION ['projovemurbano'] ['ppuid'] == '2') {
		$link = "projovemurbano.php?modulo=principal/planoImplementacao2013&acao=A&aba={$_GET['aba']}&aba2=poloNucleo";
	} elseif ($_SESSION ['projovemurbano'] ['ppuid'] == '3') {
		$link = "projovemurbano.php?modulo=principal/indexPoloNucleo2014&acao=A&aba={$_REQUEST['aba']}&aba2=poloNucleo";
	} else {
		$link = "projovemurbano.php?modulo=principal/planoImplementacao&acao=A&aba=poloNucleo";
	}
	
	echo "<script>
			alert('Gravado com sucesso');
			window.location='$link';
		  </script>";
}
function inserirPolo($dados) {
	global $db;
	
	$sql = "INSERT INTO projovemurbano.polo(
            pmuid, polqtdmunicipio, polcep, polendereco, polbairro, 
            polcomplemento, polnumero, poltelefone, polcelular, polemail, 
            polstatus)
    		VALUES ('" . $dados ['pmuid'] . "', 
    				NULL, 
    				" . (($dados ['polcep']) ? "'" . str_replace ( array (
			"-" 
	), array (
			"" 
	), $dados ['polcep'] ) . "'" : "NULL") . ", 
    				" . (($dados ['polendereco']) ? "'" . $dados ['polendereco'] . "'" : "NULL") . ", 
    				" . (($dados ['polbairro']) ? "'" . $dados ['polbairro'] . "'" : "NULL") . ", 
            		" . (($dados ['polcomplemento']) ? "'" . $dados ['polcomplemento'] . "'" : "NULL") . ", 
            		" . (($dados ['polnumero']) ? "'" . trim ( $dados ['polnumero'] ) . "'" : "NULL") . ", 
            		" . (($dados ['poltelefone']) ? "'" . str_pad ( $dados ['poltelefoneddd'], 2, "0", STR_PAD_LEFT ) . str_replace ( array (
			"-" 
	), array (
			"" 
	), $dados ['poltelefone'] ) . "'" : "NULL") . ", 
            		" . (($dados ['polcelular']) ? "'" . $dados ['polcelular'] . "'" : "NULL") . ", 
            		" . (($dados ['polemail']) ? "'" . $dados ['polemail'] . "'" : "NULL") . ", 
            		'A') RETURNING polid;";
	
	$polid = $db->pegaUm ( $sql );
	
	if ($dados ['muncod']) {
		foreach ( $dados ['muncod'] as $muncod ) {
			$sql = "INSERT INTO projovemurbano.municipio(
            		muncod, pmuid, munsede, munqtdnucleo, munstatus)
    				VALUES ('" . $muncod . "', 
    						NULL, 
    						NULL, 
    						NULL, 
    						'A') RETURNING munid;";
			
			$munid = $db->pegaUm ( $sql );
			
			$sql = "INSERT INTO projovemurbano.associamucipiopolo(
            		munid, polid, ampstatus)
    				VALUES ('" . $munid . "', '" . $polid . "', 'A');";
			
			$db->executar ( $sql );
		}
	}
	
	if ($_SESSION ['projovemurbano'] ['ppuid'] == '2') {
		$db->executar ( "DELETE FROM projovemurbano.educadores 
						WHERE proid IN(SELECT proid 
										FROM projovemurbano.profissionais 
										WHERE pjuid='" . $_SESSION ['projovemurbano'] ['pjuid'] . "'
											AND tprid = {$_SESSION['projovemurbano']['tprid']})" );
		$db->executar ( "DELETE FROM projovemurbano.dirassistentes 
						WHERE proid IN(SELECT proid 
										FROM projovemurbano.profissionais 
										WHERE pjuid='" . $_SESSION ['projovemurbano'] ['pjuid'] . "'
											AND tprid = {$_SESSION['projovemurbano']['tprid']})" );
		$db->executar ( "DELETE FROM projovemurbano.diretorpolo 
						WHERE proid IN(SELECT proid 
										FROM projovemurbano.profissionais 
										WHERE pjuid='" . $_SESSION ['projovemurbano'] ['pjuid'] . "'
											AND tprid = {$_SESSION['projovemurbano']['tprid']})" );
	} else {
		$db->executar ( "DELETE FROM projovemurbano.educadores WHERE proid IN(SELECT proid FROM projovemurbano.profissionais WHERE pjuid='" . $_SESSION ['projovemurbano'] ['pjuid'] . "')" );
		$db->executar ( "DELETE FROM projovemurbano.dirassistentes WHERE proid IN(SELECT proid FROM projovemurbano.profissionais WHERE pjuid='" . $_SESSION ['projovemurbano'] ['pjuid'] . "')" );
		$db->executar ( "DELETE FROM projovemurbano.diretorpolo WHERE proid IN(SELECT proid FROM projovemurbano.profissionais WHERE pjuid='" . $_SESSION ['projovemurbano'] ['pjuid'] . "')" );
	}
	
	$db->commit ();
	
	if ($_SESSION ['projovemurbano'] ['ppuid'] == '2') {
		$link = "projovemurbano.php?modulo=principal/planoImplementacao2013&acao=A&aba={$_GET['aba']}&aba2=poloNucleo";
	} elseif ($_SESSION ['projovemurbano'] ['ppuid'] == '3') {
		$link = "projovemurbano.php?modulo=principal/indexPoloNucleo2014&acao=A&aba={$_REQUEST['aba']}&aba2=poloNucleo";
	} else {
		$link = "projovemurbano.php?modulo=principal/planoImplementacao&acao=A&aba=poloNucleo";
	}
	echo "<script>
			alert('Polo gravado com sucesso.');
			window.location='$link';
		  </script>";
}
function atualizarPolo($dados) {
	global $db;
	
	$sql = "UPDATE projovemurbano.polo
			SET
            polcep=" . (($dados ['polcep']) ? "'" . str_replace ( array (
			"-" 
	), array (
			"" 
	), $dados ['polcep'] ) . "'" : "NULL") . ", 
            polendereco=" . (($dados ['polendereco']) ? "'" . $dados ['polendereco'] . "'" : "NULL") . ", 
            polbairro=" . (($dados ['polbairro']) ? "'" . $dados ['polbairro'] . "'" : "NULL") . ", 
            polcomplemento=" . (($dados ['polcomplemento']) ? "'" . $dados ['polcomplemento'] . "'" : "NULL") . ", 
            polnumero=" . (($dados ['polnumero']) ? "'" . $dados ['polnumero'] . "'" : "NULL") . ", 
            poltelefone=" . (($dados ['poltelefone']) ? "'" . str_pad ( $dados ['poltelefoneddd'], 2, "0", STR_PAD_LEFT ) . str_replace ( array (
			"-" 
	), array (
			"" 
	), $dados ['poltelefone'] ) . "'" : "NULL") . ", 
            polcelular=" . (($dados ['polcelular']) ? "'" . $dados ['polcelular'] . "'" : "NULL") . ", 
            polemail=" . (($dados ['polemail']) ? "'" . $dados ['polemail'] . "'" : "NULL") . "
            WHERE polid='" . $dados ['polid'] . "';";
	
	$db->executar ( $sql );
	
	$muncods = $db->carregarColuna ( "SELECT mun.muncod 
								   FROM projovemurbano.municipio mu 
								   INNER JOIN projovemurbano.associamucipiopolo amp ON amp.munid = mu.munid 
								   INNER JOIN projovemurbano.polo pol ON amp.polid = pol.polid 
								   INNER JOIN territorios.municipio mun ON mun.muncod = mu.muncod
								   WHERE pol.polid='" . $dados ['polid'] . "'" );
	
	$db->executar ( "UPDATE projovemurbano.municipio SET munstatus='I' WHERE munid IN(SELECT munid FROM projovemurbano.associamucipiopolo WHERE polid='" . $dados ['polid'] . "')" );
	
	if ($dados ['muncod']) {
		foreach ( $dados ['muncod'] as $m ) {
			if (! in_array ( $m, $muncods )) {
				$munid = $db->pegaUm ( "INSERT INTO projovemurbano.municipio(muncod) VALUES ('" . $m . "') RETURNING munid;" );
				$db->executar ( "INSERT INTO projovemurbano.associamucipiopolo(munid, polid) VALUES ('" . $munid . "', '" . $dados ['polid'] . "');" );
			} else {
				$db->executar ( "UPDATE projovemurbano.municipio SET munstatus='A' WHERE muncod='" . $m . "' AND munid IN(SELECT munid FROM projovemurbano.associamucipiopolo WHERE polid='" . $dados ['polid'] . "')" );
			}
		}
	}
	
	$db->commit ();
	
	if ($_SESSION ['projovemurbano'] ['ppuid'] == '2') {
		$link = "projovemurbano.php?modulo=principal/planoImplementacao2013&acao=A&aba={$_REQUEST['aba']}&aba2=poloNucleo";
	} elseif ($_SESSION ['projovemurbano'] ['ppuid'] == '3') {
		$link = "projovemurbano.php?modulo=principal/indexPoloNucleo2014&acao=A&aba={$_REQUEST['aba']}&aba2=poloNucleo";
	} else {
		$link = "projovemurbano.php?modulo=principal/planoImplementacao&acao=A&aba=poloNucleo";
	}
	
	echo "<script>
			alert('Atualizado com sucesso');
			window.location='$link';
		  </script>";
}
function gravarDemaisAcoes($dados) {
	global $db;
	$sql = "UPDATE projovemurbano.demaisacoes
			   SET deapercmax=" . (($dados ['deapercmax']) ? "'" . $dados ['deapercmax'] . "'" : "NULL") . ", 
			   	   deapercutilizado=" . (($dados ['deapercutilizado']) ? "'" . $dados ['deapercutilizado'] . "'" : "NULL") . "
			 WHERE deaid='" . $_SESSION ['projovemurbano'] ['deaid'] . "';";
	
	$db->executar ( $sql );
	
	if ($dados ['idavlrmes']) {
		foreach ( $dados ['idavlrmes'] as $acoid => $idavlrmes ) {
			$idaid = $db->pegaUm ( "SELECT idaid FROM projovemurbano.itemdemaisacoes WHERE acoid='" . $acoid . "' AND deaid='" . $_SESSION ['projovemurbano'] ['deaid'] . "'" );
			
			if ($idaid) {
				$sql = "UPDATE projovemurbano.itemdemaisacoes 
						SET idavlrmes=" . (($dados ['idavlrmes'] [$acoid]) ? "'" . str_replace ( array (
						".",
						"," 
				), array (
						"",
						"." 
				), $dados ['idavlrmes'] [$acoid] ) . "'" : "NULL") . ", 
							idaqtdmeses=" . (($dados ['idaqtdmeses'] [$acoid]) ? "'" . $dados ['idaqtdmeses'] [$acoid] . "'" : "NULL") . ",
							idapercmaxprevisto=" . (($dados ['idapercmaxprevisto'] [$acoid]) ? "'" . $dados ['idapercmaxprevisto'] [$acoid] . "'" : "NULL") . "
							WHERE idaid='" . $idaid . "'";
			} else {
				$sql = "INSERT INTO projovemurbano.itemdemaisacoes(
	            		acoid, idavlrmes, idaqtdmeses, idapercmaxprevisto, deaid)
	    				VALUES ('" . $acoid . "', 
	    						" . (($dados ['idavlrmes'] [$acoid]) ? "'" . str_replace ( array (
						".",
						"," 
				), array (
						"",
						"." 
				), $dados ['idavlrmes'] [$acoid] ) . "'" : "NULL") . ", 
	    						" . (($dados ['idaqtdmeses'] [$acoid]) ? "'" . $dados ['idaqtdmeses'] [$acoid] . "'" : "NULL") . ", 
	    						" . (($dados ['idapercmaxprevisto'] [$acoid]) ? "'" . $dados ['idapercmaxprevisto'] [$acoid] . "'" : "NULL") . ",
	    						'" . $_SESSION ['projovemurbano'] ['deaid'] . "');";
			}
			$db->executar ( $sql );
		}
	}
	
	$db->commit ();
	
	if ($_SESSION ['projovemurbano'] ['ppuid'] == '2') {
		echo "<script>
                alert('Gravado com sucesso');
                window.location='projovemurbano.php?modulo=principal/planoImplementacao2013&acao=A&aba={$_GET['aba']}&aba2=demaisAcoes';
              </script>";
	} elseif ($_SESSION ['projovemurbano'] ['ppuid'] == '3') {
		echo "<script>
                alert('Gravado com sucesso');
                window.location='projovemurbano.php?modulo=principal/planoImplementacao2014&acao=A&aba={$_GET['aba']}&aba2=demaisAcoes';
              </script>";
	} else {
		echo "<script>
                alert('Gravado com sucesso');
                window.location='projovemurbano.php?modulo=principal/planoImplementacao&acao=A&aba=demaisAcoes';
              </script>";
	}
}
function gravarTransporteDidatico($dados) {
	global $db;
	
	$sql = "UPDATE projovemurbano.transportematerial
   			SET tmapercmax=" . (($dados ['tmapercmax']) ? "'" . $dados ['tmapercmax'] . "'" : "NULL") . ", 
   				tmaperutilizado=" . (($dados ['tmaperutilizado']) ? "'" . $dados ['tmaperutilizado'] . "'" : "NULL") . ", 
   				tmarecursoutilizado=" . (($dados ['tmarecursoutilizado']) ? "'" . str_replace ( array (
			".",
			"," 
	), array (
			"",
			"." 
	), $dados ['tmarecursoutilizado'] ) . "'" : "NULL") . " 
 			WHERE tmaid='" . $_SESSION ['projovemurbano'] ['tmaid'] . "';";
	
	$db->executar ( $sql );
	$db->commit ();
	
	if ($_SESSION ['projovemurbano'] ['ppuid'] == '2') {
		echo "<script>
                alert('Gravado com sucesso');
                window.location='projovemurbano.php?modulo=principal/planoImplementacao2013&acao=A&aba={$_GET['aba']}&aba2=transporteDidatico';
              </script>";
	}elseif($_SESSION ['projovemurbano'] ['ppuid'] == '3'){
		echo "<script>
				alert('Gravado com sucesso');
				window.location='projovemurbano.php?modulo=principal/planoImplementacao2014&acao=A&aba={$_GET['aba']}&aba2=transporteDidatico';
				</script>";
	}else{
		echo "<script>
                alert('Gravado com sucesso');
                window.location='projovemurbano.php?modulo=principal/planoImplementacao&acao=A&aba=transporteDidatico';
              </script>";
	}
}
function gravarQualificacaoProfissional2013($dados) {
	global $db;
	
	$sql = "UPDATE projovemurbano.qualificacaoprofissional SET 
				qprpercmax = " . (($dados ['qprpercmax']) ? "'" . $dados ['qprpercmax'] . "'" : "NULL") . ",
				qprpercutilizado = " . (($dados ['qprpercutilizado']) ? "'" . $dados ['qprpercutilizado'] . "'" : "NULL") . "--,
				--qprarco = " . (($dados ['qprarco']) ? $dados ['qprarco'] : "NULL") . ",
				--qpredutecnica = " . (($dados ['qpredutecnica']) ? $dados ['qpredutecnica'] : "NULL") . "
			WHERE 
				qprid = '" . $_SESSION ['projovemurbano'] ['qprid'] . "';
			DELETE FROM projovemurbano.qualificacaoprofissional_nucleo WHERE qprid = '{$_SESSION['projovemurbano']['qprid']}';";
	
	$db->executar ( $sql );
	
	$sql = "";
	// ver($dados,d);
	
	if ($dados ['pgavlrmes'] && is_array ( $dados ['nucid'] )) {
		foreach ( $dados ['nucid'] as $nucid ) {
			$sql .= "INSERT INTO projovemurbano.qualificacaoprofissional_nucleo(qprid, nucid, qpnarco, qpnedutecnica) 
					 VALUES({$_SESSION['projovemurbano']['qprid']}, $nucid, 
							" . ($dados ['qpnarco'] [$nucid] == TRUE ? 'TRUE' : 'FALSE') . ",
							" . ($dados ['qpnedutecnica'] [$nucid] == TRUE ? 'TRUE' : 'FALSE') . ");";
		}
	}
	if (is_array ( $dados ['pgavlrmes'] ) && is_array ( $dados ['nucid'] )) {
		$sql .= "DELETE FROM projovemurbano.previsaogasto 
				WHERE 
					qprid='" . $_SESSION ['projovemurbano'] ['qprid'] . "';";
		foreach ( $dados ['pgavlrmes'] as $desid => $valormes ) {
			foreach ( $dados ['nucid'] as $nucid ) {
				
				$valor = (($dados ['qpnarco'] [$nucid] == "TRUE") ? $dados ['pgavlrmes'] [$desid] : "");
				
				$sql2 = "SELECT 
							pgaid 
						FROM 
							projovemurbano.previsaogasto 
						WHERE 
							desid= $desid	 
							AND nucid = '$nucid' 
							AND qprid='" . $_SESSION ['projovemurbano'] ['qprid'] . "'";
				$pgaid = $db->pegaUm ( $sql2 );
				
				// if($pgaid) {
				// $sql .= "UPDATE projovemurbano.previsaogasto SET
				// pgavlrmes = ".(($valor)?"'".str_replace(array(".",","),array("","."),$valor)."'":"NULL").",
				// pgaqtdmeses = ".(($dados['pgaqtdmeses'][$desid])?"'".$dados['pgaqtdmeses'][$desid]."'":"NULL")."
				// WHERE pgaid='".$pgaid."';";
				// } else {
				$sql .= "INSERT INTO projovemurbano.previsaogasto(desid, pgavlrmes, pgaqtdmeses, qprid, nucid)
							VALUES ('" . $desid . "', 
									" . (($valor) ? "'" . str_replace ( array (
						".",
						"," 
				), array (
						"",
						"." 
				), $valor ) . "'" : "NULL") . ", 
									" . (($dados ['pgaqtdmeses'] [$desid]) ? "'" . $dados ['pgaqtdmeses'] [$desid] . "'" : "NULL") . ", 
									'" . $_SESSION ['projovemurbano'] ['qprid'] . "', $nucid);";
				// }
			}
		}
	}
	// ver($dados,$sql,d);
	if ($sql) {
		$db->executar ( $sql );
		$db->commit ();
	}
	
	if ($_SESSION ['projovemurbano'] ['muncod'])
		$ab = "qualificacaoProfissionalMunicipio";
	if ($_SESSION ['projovemurbano'] ['estuf'])
		$ab = "qualificacaoProfissionalEstado";
	
	if ($_SESSION ['projovemurbano'] ['ppuid'] == '2') {
		echo "<script>
				alert('Gravado com sucesso');
				window.location='projovemurbano.php?modulo=principal/planoImplementacao2013&acao=A&aba={$_GET['aba']}&aba2={$ab}';
			</script>";
	} else {
		echo "<script>
				alert('Gravado com sucesso');
				window.location='projovemurbano.php?modulo=principal/planoImplementacao&acao=A&aba=" . $ab . "';
			</script>";
	}
}
function gravarQualificacaoProfissional2014($dados) {
	global $db;
	$sql = "UPDATE projovemurbano.qualificacaoprofissional SET
				qprpercmax = " . (($dados ['qprpercmax']) ? "'" . $dados ['qprpercmax'] . "'" : "NULL") . ",
				qprpercutilizado = " . (($dados ['qprpercutilizado']) ? "'" . $dados ['qprpercutilizado'] . "'" : "NULL") . ",
				qprarco = " . (($dados ['qprarco']) ? $dados ['qprarco'] : "NULL") . ",
				qpredutecnica = " . (($dados ['qpredutecnica']) ? $dados ['qpredutecnica'] : "NULL") . "
			WHERE
				qprid = '" . $_SESSION ['projovemurbano'] ['qprid'] . "';
				DELETE FROM projovemurbano.qualificacaoprofissional_nucleo WHERE qprid = '{$_SESSION['projovemurbano']['qprid']}';";
	
	$db->executar ( $sql );
	$sql = "";
	if ($dados ['pgavlrmes'] && is_array ( $dados ['arcidnucleo'] )) {
		foreach ( $dados ['arcidnucleo'] as $nucid => $arcid ) {
			$arcid = (($dados ['qprarco'] == "TRUE") ? $arcid : "");
			$sql = "SELECT arqid FROM projovemurbano.arcoqualificacao WHERE nucid='" . $nucid . "'";
			$arqid = $db->pegaUm ( $sql );
			if ($arqid) {
				$sql = "UPDATE projovemurbano.arcoqualificacao SET arcid=" . (($arcid) ? "'" . $arcid . "'" : "NULL") . " WHERE arqid='" . $arqid . "'";
			} else {
				if ($arcid) {
					$sql = "INSERT INTO projovemurbano.arcoqualificacao(
            				nucid, arcid, qprid)
	    					VALUES ('" . $nucid . "', '" . $arcid . "', '" . $_SESSION ['projovemurbano'] ['qprid'] . "');";
				}
			}
			$db->executar ( $sql );
			$sql = "";
			$sql .= "INSERT INTO projovemurbano.qualificacaoprofissional_nucleo(qprid, nucid, qpnarco, qpnedutecnica)
			VALUES({$_SESSION['projovemurbano']['qprid']}, $nucid,
			" . ($dados ['qpnarco'] [$nucid] == TRUE ? 'TRUE' : 'FALSE') . ",
					" . ($dados ['qpnedutecnica'] [$nucid] == TRUE ? 'TRUE' : 'FALSE') . ");";
			$db->executar ( $sql );
		}
	}
	if (is_array ( $dados ['pgavlrmes'] ) && is_array ( $dados ['arcidnucleo'] )) {
		$sql .= "DELETE FROM projovemurbano.previsaogasto
				WHERE
					qprid='" . $_SESSION ['projovemurbano'] ['qprid'] . "';";
		$db->executar ( $sql );
		foreach ( $dados ['pgavlrmes'] as $desid => $valormes ) {
			foreach ( $dados ['arcidnucleo'] as $nucid => $arcos ) {
				$valor = (($dados ['qpnarco'] [$nucid] == "TRUE") ? $dados ['pgavlrmes'] [$desid] : "");
				
				$sql2 = "SELECT
					pgaid
					FROM
					projovemurbano.previsaogasto
					WHERE
					desid= $desid
					AND nucid = '$nucid'
					AND qprid='" . $_SESSION ['projovemurbano'] ['qprid'] . "'";
				$pgaid = $db->pegaUm ( $sql2 );
				
				if ($pgaid) {
					$sql .= "UPDATE projovemurbano.previsaogasto SET
								pgavlrmes = " . (($valormes) ? "'" . str_replace ( array (
							".",
							"," 
					), array (
							"",
							"." 
					), $valormes ) . "'" : "NULL") . ",
								pgaqtdmeses = " . (($dados ['pgaqtdmeses'] [$desid]) ? "'" . $dados ['pgaqtdmeses'] [$desid] . "'" : "NULL") . "
							 WHERE pgaid='" . $pgaid . "';";
				} else {
					$sql .= "INSERT INTO projovemurbano.previsaogasto(desid, pgavlrmes, pgaqtdmeses, qprid, nucid)
								VALUES ('" . $desid . "',
										" . (($valormes) ? "'" . str_replace ( array (
							".",
							"," 
					), array (
							"",
							"." 
					), $valormes ) . "'" : "NULL") . ",
										" . (($dados ['pgaqtdmeses'] [$desid]) ? "'" . $dados ['pgaqtdmeses'] [$desid] . "'" : "NULL") . ",
										'" . $_SESSION ['projovemurbano'] ['qprid'] . "', $nucid);";
				}
				$db->executar ( $sql );
			}
		}
	}
	if ($sql) {
		$db->commit ();
	}
	
	if ($_SESSION ['projovemurbano'] ['muncod'])
		$ab = "qualificacaoProfissionalMunicipio";
	if ($_SESSION ['projovemurbano'] ['estuf'])
		$ab = "qualificacaoProfissionalEstado";
	if ($_SESSION ['projovemurbano'] ['ppuid'] == '3') {
		echo "<script>
				alert('Gravado com sucesso');
				window.location='projovemurbano.php?modulo=principal/planoImplementacao2014&acao=A&aba={$_GET['aba']}&aba2={$ab}';
				</script>";
	}
}
function gravarQualificacaoProfissional($dados) {
	global $db;
	
	$sql = "UPDATE projovemurbano.qualificacaoprofissional
			   SET qprpercmax=" . (($dados ['qprpercmax']) ? "'" . $dados ['qprpercmax'] . "'" : "NULL") . ", 
			   	   qprpercutilizado=" . (($dados ['qprpercutilizado']) ? "'" . $dados ['qprpercutilizado'] . "'" : "NULL") . ", 
		       	   qprarco=" . (($dados ['qprarco']) ? $dados ['qprarco'] : "NULL") . ", 
			       qpredutecnica=" . (($dados ['qpredutecnica']) ? $dados ['qpredutecnica'] : "NULL") . "
			 WHERE qprid='" . $_SESSION ['projovemurbano'] ['qprid'] . "';";
	
	$db->executar ( $sql );
	
	if ($dados ['arcidnucleo']) {
		foreach ( $dados ['arcidnucleo'] as $nucid => $arcid ) {
			$arcid = (($dados ['qprarco'] == "TRUE") ? $arcid : "");
			$sql = "SELECT arqid FROM projovemurbano.arcoqualificacao WHERE nucid='" . $nucid . "'";
			$arqid = $db->pegaUm ( $sql );
			if ($arqid) {
				$sql = "UPDATE projovemurbano.arcoqualificacao SET arcid=" . (($arcid) ? "'" . $arcid . "'" : "NULL") . " WHERE arqid='" . $arqid . "'";
			} else {
				if ($arcid) {
					$sql = "INSERT INTO projovemurbano.arcoqualificacao(
	            			nucid, arcid, qprid)
	    					VALUES ('" . $nucid . "', '" . $arcid . "', '" . $_SESSION ['projovemurbano'] ['qprid'] . "');";
				}
			}
			$db->executar ( $sql );
		}
	}
	
	if ($dados ['arcidmunicipio']) {
		foreach ( $dados ['arcidmunicipio'] as $muncod => $arcid ) {
			$arcid = (($dados ['qprarco'] == "TRUE") ? $arcid : "");
			$sql = "SELECT arqid FROM projovemurbano.arcoqualificacao WHERE muncod='" . $muncod . "'";
			$arqid = $db->pegaUm ( $sql );
			if ($arqid) {
				$sql = "UPDATE projovemurbano.arcoqualificacao SET arcid=" . (($arcid) ? "'" . $arcid . "'" : "NULL") . " WHERE arqid='" . $arqid . "'";
			} else {
				if ($arcid) {
					$sql = "INSERT INTO projovemurbano.arcoqualificacao(
	            			muncod, arcid, qprid)
	    					VALUES ('" . $muncod . "', '" . $arcid . "', '" . $_SESSION ['projovemurbano'] ['qprid'] . "');";
				}
			}
			$db->executar ( $sql );
		}
	}
	
	if ($dados ['pgavlrmes']) {
		foreach ( $dados ['pgavlrmes'] as $desid => $valormes ) {
			
			$dados ['pgavlrmes'] [$desid] = (($dados ['qprarco'] == "TRUE") ? $dados ['pgavlrmes'] [$desid] : "");
			$pgaid = $db->pegaUm ( "SELECT pgaid FROM projovemurbano.previsaogasto WHERE desid='" . $desid . "' AND qprid='" . $_SESSION ['projovemurbano'] ['qprid'] . "'" );
			
			if ($pgaid) {
				$sql = "UPDATE projovemurbano.previsaogasto 
						SET pgavlrmes=" . (($dados ['pgavlrmes'] [$desid]) ? "'" . str_replace ( array (
						".",
						"," 
				), array (
						"",
						"." 
				), $dados ['pgavlrmes'] [$desid] ) . "'" : "NULL") . ", 
							pgaqtdmeses=" . (($dados ['pgaqtdmeses'] [$desid]) ? "'" . $dados ['pgaqtdmeses'] [$desid] . "'" : "NULL") . " 
							WHERE pgaid='" . $pgaid . "'";
			} else {
				$sql = "INSERT INTO projovemurbano.previsaogasto(
	            		desid, pgavlrmes, pgaqtdmeses, qprid)
	    				VALUES ('" . $desid . "', " . (($dados ['pgavlrmes'] [$desid]) ? "'" . str_replace ( array (
						".",
						"," 
				), array (
						"",
						"." 
				), $dados ['pgavlrmes'] [$desid] ) . "'" : "NULL") . ", " . (($dados ['pgaqtdmeses'] [$desid]) ? "'" . $dados ['pgaqtdmeses'] [$desid] . "'" : "NULL") . ", '" . $_SESSION ['projovemurbano'] ['qprid'] . "');";
			}
			$db->executar ( $sql );
		}
	}
	
	$db->executar ( "UPDATE projovemurbano.cursoprojovemurbano SET cupstatus='I' WHERE pjuid='" . $_SESSION ['projovemurbano'] ['pjuid'] . "'" );
	
	if ($dados ['qpredutecnica'] == "TRUE") {
		if ($dados ['instituicao']) {
			foreach ( $dados ['instituicao'] as $nucid => $instituicao ) {
				if ($instituicao) {
					$db->executar ( "UPDATE projovemurbano.nucleo SET nucinstituicoes='" . $instituicao . "' WHERE nucid='" . $nucid . "'" );
					if ($dados ['cupqtdestudantes'] [$nucid]) {
						foreach ( $dados ['cupqtdestudantes'] [$nucid] as $ccuid => $cupqtdestudantes ) {
							if ($cupqtdestudantes) {
								$db->executar ( "INSERT INTO projovemurbano.cursoprojovemurbano(ccuid, pjuid, nucid, cupturma, cupqtdestudantes)
		    								   VALUES ('" . $ccuid . "', 
		    								   		   '" . $_SESSION ['projovemurbano'] ['pjuid'] . "', 
		    								   		   '" . $nucid . "', 
		    								   		   '" . $dados ['cupturma'] [$nucid] [$ccuid] . "', 
		    								   		   '" . $cupqtdestudantes . "' 
		    								   		   );" );
							}
						}
					}
				}
			}
		}
	}
	
	$db->commit ();
	
	if ($_SESSION ['projovemurbano'] ['muncod'])
		$ab = "qualificacaoProfissionalMunicipio";
	if ($_SESSION ['projovemurbano'] ['estuf'])
		$ab = "qualificacaoProfissionalEstado";
	
	if ($_SESSION ['projovemurbano'] ['ppuid'] == '2') {
		echo "<script>
                alert('Gravado com sucesso');
                window.location='projovemurbano.php?modulo=principal/planoImplementacao2013&acao=A&aba={$_GET['aba']}&aba2={$ab}';
              </script>";
	} else {
		echo "<script>
                alert('Gravado com sucesso');
                window.location='projovemurbano.php?modulo=principal/planoImplementacao&acao=A&aba=" . $ab . "';
              </script>";
	}
}
function gravarGeneroAlimenticio($dados) {
	global $db;
	
	$sql = "UPDATE projovemurbano.generoalimenticio
   			SET galpercmax=" . (($dados ['galpercmax']) ? "'" . $dados ['galpercmax'] . "'" : "NULL") . ", 
   				galpercutilizado=" . (($dados ['galpercutilizado']) ? "'" . $dados ['galpercutilizado'] . "'" : "NULL") . "
 			WHERE galid='" . $_SESSION ['projovemurbano'] ['galid'] . "';";
	
	$db->executar ( $sql );
	
	$sql = "SELECT lreid FROM projovemurbano.lancherefeicao WHERE galid='" . $_SESSION ['projovemurbano'] ['galid'] . "'";
	$lreid = $db->pegaUm ( $sql );
	
	if ($lreid) {
		$sql = "UPDATE projovemurbano.lancherefeicao
	   			SET lremeta=" . (($dados ['lremeta']) ? "'" . $dados ['lremeta'] . "'" : "NULL") . ", 
	   				lreqtdcrianca=" . (($dados ['lreqtdcrianca']) ? "'" . $dados ['lreqtdcrianca'] . "'" : "NULL") . ", 
	   				lreqtdmeses=" . (($dados ['lreqtdmeses']) ? "'" . $dados ['lreqtdmeses'] . "'" : "NULL") . ", 
	       			lrevlrreflanche=" . (($dados ['lrevlrreflanche']) ? "'" . str_replace ( array (
				".",
				"," 
		), array (
				"",
				"." 
		), $dados ['lrevlrreflanche'] ) . "'" : "NULL") . ", 
	       			lrevlrtotal=" . (($dados ['lrevlrtotal']) ? "'" . str_replace ( array (
				".",
				"," 
		), array (
				"",
				"." 
		), $dados ['lrevlrtotal'] ) . "'" : "NULL") . "
	 			WHERE lreid='" . $lreid . "';";
	} else {
		$sql = "INSERT INTO projovemurbano.lancherefeicao(
	            galid, lremeta, lreqtdcrianca, lreqtdmeses, lrevlrreflanche, 
	            lrevlrtotal, lrestatus)
	    		VALUES ('" . $_SESSION ['projovemurbano'] ['galid'] . "', 
	    				" . (($dados ['lremeta']) ? "'" . $dados ['lremeta'] . "'" : "NULL") . ", 
	    				" . (($dados ['lreqtdcrianca']) ? "'" . $dados ['lreqtdcrianca'] . "'" : "NULL") . ", 
	    				" . (($dados ['lreqtdmeses']) ? "'" . $dados ['lreqtdmeses'] . "'" : "NULL") . ", 
	    				" . (($dados ['lrevlrreflanche']) ? "'" . str_replace ( array (
				".",
				"," 
		), array (
				"",
				"." 
		), $dados ['lrevlrreflanche'] ) . "'" : "NULL") . ", 
	            		" . (($dados ['lrevlrtotal']) ? "'" . str_replace ( array (
				".",
				"," 
		), array (
				"",
				"." 
		), $dados ['lrevlrtotal'] ) . "'" : "NULL") . ", 
	            		'A');";
	}
	
	$db->executar ( $sql );
	
	$db->commit ();
	
	if ($_SESSION ['projovemurbano'] ['ppuid'] == '2') {
		echo "<script>
                alert('Gravado com sucesso');
                window.location='projovemurbano.php?modulo=principal/planoImplementacao2013&acao=A&aba={$_GET['aba']}&aba2=generoAlimenticios';
              </script>";
	} elseif ($_SESSION ['projovemurbano'] ['ppuid'] == '3') {
		echo "<script>
                alert('Gravado com sucesso');
                window.location='projovemurbano.php?modulo=principal/planoImplementacao2014&acao=A&aba={$_GET['aba']}&aba2=generoAlimenticios';
              </script>";
	} else {
		echo "<script>
                alert('Gravado com sucesso');
                window.location='projovemurbano.php?modulo=principal/planoImplementacao&acao=A&aba=generoAlimenticios';
              </script>";
	}
}
function gravarFormacaoEducadores($dados) {
	global $db;
	$sql = "UPDATE projovemurbano.formacaoeducadores
                  SET fedpercmax=" . (($dados ['fedpercmax']) ? "'" . $dados ['fedpercmax'] . "'" : "NULL") . ",
		      fedperutilizado=" . (($dados ['fedperutilizado']) ? "'" . $dados ['fedperutilizado'] . "'" : "NULL") . ",
		      fedqtd=" . (($dados ['fedqtd']) ? "'" . $dados ['fedqtd'] . "'" : "NULL") . "
                  WHERE fedid='" . $_SESSION ['projovemurbano'] ['fedid'] . "'
                    AND tprid = {$_SESSION['projovemurbano']['tprid']}";
	$db->executar ( $sql );
	
	if ($dados ['rgavalor']) {
		foreach ( $dados ['rgavalor'] as $refid => $valor ) {
			
			$sql = "SELECT rgaid FROM projovemurbano.recursosgastos
                                  WHERE refid='" . $refid . "' AND fedid='" . $_SESSION ['projovemurbano'] ['fedid'] . "'
                                    AND tprid = {$_SESSION['projovemurbano']['tprid']}";
			$rgaid = $db->pegaUm ( $sql );
			
			$tprid = 'NULL';
			
			if ($_SESSION ['projovemurbano'] ['ppuid'] != '1') {
				$tprid = $_SESSION ['projovemurbano'] ['tprid'];
			}
			
			if ($rgaid) {
				
				$sql = "UPDATE projovemurbano.recursosgastos
			   	 SET rgavalor=" . (($valor) ? "'" . str_replace ( array (
						".",
						"," 
				), array (
						"",
						"." 
				), $valor ) . "'" : "NULL") . "
			 	 WHERE rgaid='" . $rgaid . "';";
				$db->executar ( $sql );
			} else {
				$sql = "INSERT INTO projovemurbano.recursosgastos(
			            fedid, rgavalor, rgastatus, refid, tprid)
					     VALUES ('" . $_SESSION ['projovemurbano'] ['fedid'] . "', 
					     		  " . (($valor) ? "'" . str_replace ( array (
						".",
						"," 
				), array (
						"",
						"." 
				), $valor ) . "'" : "NULL") . ", 
					     		  'A', 
					     		  '" . $refid . "',
									$tprid);";
				
				$db->executar ( $sql );
			}
		}
	}
	
	$sql = "SELECT aufid FROM projovemurbano.auxiliofinanceiro WHERE fedid='" . $_SESSION ['projovemurbano'] ['fedid'] . "'";
	$aufid = $db->pegaUm ( $sql );
	
	if ($aufid) {
		$sql = "UPDATE projovemurbano.auxiliofinanceiro
   				SET aufqtdeducador=" . (($dados ['aufqtdeducador']) ? "'" . $dados ['aufqtdeducador'] . "'" : "NULL") . ", 
   					aufavlrauxilio=" . (($dados ['aufavlrauxilio']) ? "'" . str_replace ( array (
				".",
				"," 
		), array (
				"",
				"." 
		), $dados ['aufavlrauxilio'] ) . "'" : "NULL") . ", 
   					aufpercmax=" . (($dados ['aufpercmax']) ? "'" . $dados ['aufpercmax'] . "'" : "NULL") . ", 
       				aufpercutilizado=" . (($dados ['aufpercutilizado']) ? "'" . $dados ['aufpercutilizado'] . "'" : "NULL") . ", 
       				aufpercutilizadoper=" . (($dados ['aufpercutilizadoper']) ? "'" . $dados ['aufpercutilizadoper'] . "'" : "NULL") . "
 				WHERE aufid='" . $aufid . "';";
	} else {
		$sql = "INSERT INTO projovemurbano.auxiliofinanceiro(
	            fedid, aufqtdeducador, aufavlrauxilio, aufpercmax, aufpercutilizado, 
	            aufpercutilizadoper, aufstatus)
	    VALUES ('" . $_SESSION ['projovemurbano'] ['fedid'] . "', 
	    		" . (($dados ['aufqtdeducador']) ? "'" . $dados ['aufqtdeducador'] . "'" : "NULL") . ", 
	    		" . (($dados ['aufavlrauxilio']) ? "'" . str_replace ( array (
				".",
				"," 
		), array (
				"",
				"." 
		), $dados ['aufavlrauxilio'] ) . "'" : "NULL") . ", 
	    		" . (($dados ['aufpercmax']) ? "'" . $dados ['aufpercmax'] . "'" : "NULL") . ", 
	    		" . (($dados ['aufpercutilizado']) ? "'" . $dados ['aufpercutilizado'] . "'" : "NULL") . ", 
	            " . (($dados ['aufpercutilizadoper']) ? "'" . $dados ['aufpercutilizadoper'] . "'" : "NULL") . ", 'A');";
	}
	
	$db->executar ( $sql );
	
	$db->commit ();
	
	if ($_SESSION ['projovemurbano'] ['ppuid'] == '2') {
		echo "<script>
                alert('Gravado com sucesso');
                window.location='projovemurbano.php?modulo=principal/planoImplementacao2013&acao=A&aba={$_GET['aba']}&aba2=formacaoEducadores';
              </script>";
	} elseif ($_SESSION ['projovemurbano'] ['ppuid'] == '3') {
		echo "<script>
		    	alert('Gravado com sucesso');
		    	window.location='projovemurbano.php?modulo=principal/planoImplementacao2014&acao=A&aba={$_GET['aba']}&aba2=formacaoEducadores';
    			</script>";
	} else {
		echo "<script>
				alert('Gravado com sucesso');
				window.location='projovemurbano.php?modulo=principal/planoImplementacao&acao=A&aba=formacaoEducadores';
			  </script>";
	}
}
function pesquisarEscolas($dados) {
	global $db;
	
	$whereEntnome = '';
	if (isset ( $dados ['entnome'] ) && ! empty ( $dados ['entnome'] )) {
		$dados ['entnome'] = strtoupper ( $dados ['entnome'] );
		$whereEntnome = " AND no_entidade LIKE '%{$dados['entnome']}%' ";
	}
	
	$sql = "SELECT 
				'<img src=../imagens/alterar.gif border=0 style=cursor:pointer; onclick=\"marcarCodigoInep(\''||pk_cod_entidade||'\');\">' as acao, 
				pk_cod_entidade, no_entidade 
			FROM 
				educacenso_2014.tab_entidade
			WHERE 
				fk_cod_municipio='" . $dados ['muncod'] . "' AND id_dependencia_adm='" . $dados ['id_dependencia_adm'] . "'
                                {$whereEntnome}
                        ORDER BY no_entidade";
	
	$db->monta_lista_simples ( $sql, $cabecalho, 5000, 5, 'N', '100%', $par2 );
}
function montarCombosMunicipioRede($dados) {
	global $db;
	?>
<table class="listagem" bgcolor="#f5f5f5" cellSpacing="1"
	cellPadding="3" align="center" width="100%">
	<tr>
		<td class="SubTituloDireita">Município</td>
		<td><? $db->monta_combo('muncod', "SELECT muncod as codigo, mundescricao as descricao FROM territorios.municipio WHERE estuf='".$dados['estuf']."' ORDER BY mundescricao", (($_SESSION['projovemurbano']['muncod'])?'N':'S'), 'Selecione', '', '', '', '', 'S', 'muncod', '', $_SESSION['projovemurbano']['muncod']); ?></td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Rede</td>
		<td>
			<?
	$op [] = array (
			'codigo' => '1',
			'descricao' => 'FEDERAL' 
	);
	$op [] = array (
			'codigo' => '2',
			'descricao' => 'ESTADUAL' 
	);
	$op [] = array (
			'codigo' => '3',
			'descricao' => 'MUNICIPAL' 
	);
	$op [] = array (
			'codigo' => '4',
			'descricao' => 'PRIVADA' 
	);
	$db->monta_combo ( 'id_dependencia_adm', $op, 'S', 'Selecione', '', '', '', '', 'S', 'id_dependencia_adm', '' );
	?></td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Nome</td>
		<td>
                    <?php echo campo_texto('entnome', 'N', 'S', '', 20, 50, '', '', '', '', null, 'id="entnome"'); ?>
                    <input type="hidden" name="escolatipo"
			value="<?php echo $dados['escolatipo']; ?>" />
		</td>
	</tr>
	<tr>
		<td colspan="2" class="SubTituloCentro"><input type="button"
			value="Pesquisar"
			onclick="pesquisarEscolas(document.getElementById('muncod').value,document.getElementById('id_dependencia_adm').value,document.getElementById('entnome').value);"></td>
	</tr>
	<tr>
		<td colspan="2">
			<div id="div_escolas"
				style="position: absolute; left: 6px; margin: 13px; width: 415px; height: 180px; overflow: auto"></div>
		</td>
	</tr>
</table>
<?
}
function buscarEscolas($dados) {
	global $db;
	?>
<html>
<body>
	<script>
	function selecionarMunicipio(estuf) {
		if(estuf) {
		
	 		document.getElementById('tr_filtros').style.display='';
			jQuery.ajax({
		   		type: "POST",
		   		url: "projovemurbano.php?modulo=principal/planoImplementacao&acao=A",
		   		data: "requisicao=montarCombosMunicipioRede&estuf="+estuf+'&escolatipo='+'<?php echo $dados['escolatipo']; ?>',
		   		async: false,
		   		success: function(msg){document.getElementById('td_filtros').innerHTML=msg;}
		 		});
	 		
	 	} else {
	 	
	 		document.getElementById('tr_filtros').style.display='none';
	 		document.getElementById('td_filtros').innerHTML='Carregando...';
	 		
	 	}
	}
	
	function pesquisarEscolas(muncod, id_dependencia_adm, entnome) {
	
		if(muncod=='') {
			alert('Selecione um Município');
			return false;
		}
	
		if(id_dependencia_adm=='') {
			alert('Selecione uma Rede');
			return false;
		}

		jQuery.ajax({
	   		type: "POST",
	   		url: "projovemurbano.php?modulo=principal/planoImplementacao&acao=A",
	   		data: "requisicao=pesquisarEscolas&muncod="+muncod+"&id_dependencia_adm="+id_dependencia_adm+"&entnome="+entnome,
	   		async: false,
	   		success: function(msg){
	   			document.getElementById('div_escolas').innerHTML=msg;
	   			}
	 		});

	}
	
	function marcarCodigoInep(codinep) {
		document.getElementById('entcodent<?=$dados['escolatipo'] ?>').value=codinep;
		document.getElementById('entcodent<?=$dados['escolatipo'] ?>').onblur();
		closeMessage();
	}

	</script>
	<?
	if ($_SESSION ['projovemurbano'] ['estuf']) {
		$estuf = $_SESSION ['projovemurbano'] ['estuf'];
	}
	if ($_SESSION ['projovemurbano'] ['muncod']) {
		$estuf = $db->pegaUm ( "SELECT estuf FROM territorios.municipio WHERE muncod='" . $_SESSION ['projovemurbano'] ['muncod'] . "'" );
	}
	
	if ($estuf) :
		?>
	<script>
	jQuery(document).ready(function() {
		selecionarMunicipio('<?=$estuf ?>');
	});
	</script>
	
	
	<?
	endif;
	?>
	<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"
		align="center">
		<tr>
			<td class="SubTituloDireita">UF</td>
			<td><? $db->monta_combo('estuf', "SELECT estuf as codigo, estdescricao as descricao FROM territorios.estado", (($estuf)?'N':'S') , 'Selecione', 'selecionarMunicipio', '', '', '', 'S', '', '', $estuf); ?>  <input
				type="button" value="Fechar" onclick="closeMessage();"></td>
		</tr>
		<tr style="display: none;" id="tr_filtros">
			<td id="td_filtros" colspan="2"></td>
		</tr>
	</table>
</body>
	<?
}
function zerarCoord($dados) {
	global $db;
	
	$sql = "DELETE FROM projovemurbano.coordgeral 
			WHERE proid='" . $_SESSION ['projovemurbano'] ['proid'] . "' AND tprid = '{$_SESSION['projovemurbano']['tprid']}';";
	$sql .= "DELETE FROM projovemurbano.coordassistentes
			WHERE proid='" . $_SESSION ['projovemurbano'] ['proid'] . "' AND tprid = '{$_SESSION['projovemurbano']['tprid']}';";
	$sql .= "DELETE FROM projovemurbano.diretorpolo
			WHERE proid='" . $_SESSION ['projovemurbano'] ['proid'] . "' AND tprid = '{$_SESSION['projovemurbano']['tprid']}';";
	$sql .= "DELETE FROM projovemurbano.dirassistentes
			WHERE proid='" . $_SESSION ['projovemurbano'] ['proid'] . "' AND tprid = '{$_SESSION['projovemurbano']['tprid']}';";
	
	$db->executar ( $sql );
	
	$db->commit ();
	
	$link = "projovemurbano.php?modulo=principal/planoImplementacao2013&acao=A&aba={$_REQUEST['aba']}&aba2=profissionais";
	
	echo "<script>
			alert('Zerado com sucesso');
			window.location='$link';
		  </script>";
}
function gravarProfissionais($dados) {
	global $db;
	
	include_once APPRAIZ . "includes/classes/fileSimec.class.inc";
	// -- Processar upload do arquivo
	if ($_FILES ['arquivo']['name']) {
		$campos = array (
				"angdsc" => "'" . $_POST ['descricao'] . "'",
				"ppuid" => $_SESSION ['projovemurbano'] ['ppuid'],
				"angtip" => "'IL'" 
		);
		$file = new FilesSimec ( "anexogeral", $campos, "projovemurbano" );
		$file->setUpload ( $_POST ['descricao'], '', true, 'angid' );
		$angid = $file->getCampoRetorno ();
		// -- Salvando a referência do anexo geral para o público do programa
		if ($dados ['inldatainstlegal']) {
			$inldatainstlegal = "'" . formata_data_sql ( $dados ['inldatainstlegal'] ) . "'";
		} else {
			$inldatainstlegal = 'NULL';
		}
		$sql = "INSERT INTO projovemurbano.instrumentolegal(proid, tprid, angid, inlnuminstlegal, inldatainstlegal, tpdid)
				  VALUES({$_SESSION['projovemurbano']['proid']},
				         {$_SESSION['projovemurbano']['tprid']},
				         {$angid}, {$dados['inlnuminstlegal']}, {$inldatainstlegal}, {$dados['tpdid']})";
		$db->executar ( $sql );
		$sql = "UPDATE projovemurbano.profissionais 
	                    SET propercmax=" . (($dados ['profissionais'] ['propercmax']) ? "'" . $dados ['profissionais'] ['propercmax'] . "'" : "NULL") . ",
	                        propercutilizado=" . (($dados ['profissionais'] ['propercutilizado']) ? "'" . $dados ['profissionais'] ['propercutilizado'] . "'" : "NULL") . ",
	                        tpdid = {$dados['tpdid']},
	                        tprid = '" . $_SESSION ['projovemurbano'] ['tprid'] . "',
	                        pronuminstlegal=" . (($dados ['inlnuminstlegal']) ? "'" . $dados ['inlnuminstlegal'] . "'" : "NULL") . ", 
							prodatainstlegal=" . (($dados ['prodatainstlegal']) ? "'" . formata_data_sql ( $dados ['prodatainstlegal'] ) . "'" : "NULL") . "
	                    WHERE proid='" . $_SESSION ['projovemurbano'] ['proid'] . "'";
		$db->executar ( $sql );
	}
	$sql = "SELECT cgeid,creid FROM projovemurbano.coordgeral 
						WHERE proid='" . $_SESSION ['projovemurbano'] ['proid'] . "' AND tprid = '{$_SESSION['projovemurbano']['tprid']}' AND cgestatus='A'" ;
	$cgeid = $db->pegaLinha ($sql);
	$qtdCgeid = $db->pegaUm ( "SELECT count(cgeid) FROM projovemurbano.coordgeral WHERE proid='" . $_SESSION ['projovemurbano'] ['proid'] . "' AND cgestatus='A'" );
	
	if ($cgeid && $dados ['tprid_origem'] == $_SESSION ['projovemurbano'] ['tprid']) {
		
		$sql = "UPDATE projovemurbano.contratadorecurso
   				SET creqtd=" . (($dados ['contratadorecurso']['creqtd']) ? "'" . $dados ['contratadorecurso']['creqtd'] . "'" : "NULL") . ",
   					crevlrbrutorem=" . (($dados ['contratadorecurso']['crevlrbrutorem']) ? "'" . str_replace ( array (".",","), array ("","."), $dados ['contratadorecurso']['crevlrbrutorem'] ) . "'" : "NULL") . ",
   					creqtdmeses=" . (($dados ['contratadorecurso']['creqtdmeses']) ? "'" . $dados ['contratadorecurso']['creqtdmeses'] . "'" : "NULL") . ",
   					creencargos=" . (($dados ['contratadorecurso']['creencargos']) ? "'" . $dados ['contratadorecurso']['creencargos'] . "'" : "NULL") . ",
			        crevlrtotal=" . (($dados ['contratadorecurso']['crevlrtotal']) ? "'" . str_replace ( array (".",","), array ("","."), $dados ['contratadorecurso']['crevlrtotal'] ) . "'" : "NULL") . "
				WHERE creid='" . $cgeid ['creid'] . "';";
		
		$db->executar ( $sql );
		
		$db->commit ();
		
		$sql = "UPDATE projovemurbano.coordgeral
   				SET oppid=" . (($dados ['coordgeral'] ['oppid']) ? "'" . $dados ['coordgeral'] ['oppid'] . "'" : "NULL") . ", 
   					cgeqtd=" . (($dados ['coordgeral'] ['cgeqtd']) ? "'" . $dados ['coordgeral'] ['cgeqtd'] . "'" : "NULL") . ", 
   					cgehorpagrec=" . (($dados ['coordgeral'] ['cgehorpagrec']) ? "'" . $dados ['coordgeral'] ['cgehorpagrec'] . "'" : "NULL") . ", 
   					cgevlrhora=" . (($dados ['coordgeral'] ['cgevlrhora']) ? "'" . str_replace ( array (
				".",
				"," 
		), array (
				"",
				"." 
		), $dados ['coordgeral'] ['cgevlrhora'] ) . "'" : "NULL") . ", 
       				cgevlrcomp=" . (($dados ['coordgeral'] ['cgevlrcomp']) ? "'" . str_replace ( array (
				".",
				"," 
		), array (
				"",
				"." 
		), $dados ['coordgeral'] ['cgevlrcomp'] ) . "'" : "NULL") . ", 
       				cgeqtdmeses=" . (($dados ['coordgeral'] ['cgeqtdmeses']) ? "'" . $dados ['coordgeral'] ['cgeqtdmeses'] . "'" : "NULL") . ", 
       				cgeencargos=" . (($dados ['coordgeral'] ['cgeencargos']) ? "'" . $dados ['coordgeral'] ['cgeencargos'] . "'" : "NULL") . ", 
       				cgevlrtotal=" . (($dados ['coordgeral'] ['cgevlrtotal']) ? "'" . str_replace ( array (
				".",
				"," 
		), array (
				"",
				"." 
		), $dados ['coordgeral'] ['cgevlrtotal'] ) . "'" : "NULL") . "
 				WHERE cgeid='" . $cgeid['cgeid'] . "';";
	} elseif (! $qtdCgeid) {
		$sql = "INSERT INTO projovemurbano.contratadorecurso(
		            creqtd, crevlrbrutorem, creqtdmeses, creencargos, crevlrtotal,
		            crestatus)
	    		VALUES (" . (($dados ['contratadorecurso']['creqtd']) ? "'" . $dados ['contratadorecurso']['creqtd'] . "'" : "NULL") . ",
	    				" . (($dados ['contratadorecurso']['crevlrbrutorem']) ? "'" . str_replace ( array (".",","), array ("","."), $dados ['contratadorecurso']['crevlrbrutorem'] ) . "'" : "NULL") . ",
	    				" . (($dados ['contratadorecurso']['creqtdmeses']) ? "'" . $dados ['contratadorecurso']['creqtdmeses'] . "'" : "NULL") . ",
	    				" . (($dados ['contratadorecurso']['creencargos']) ? "'" . $dados ['contratadorecurso']['creencargos'] . "'" : "NULL") . ",
	    				" . (($dados ['contratadorecurso']['crevlrtotal']) ? "'" . str_replace ( array (".",","), array ("","."), $dados ['contratadorecurso']['crevlrtotal'] ) . "'" : "NULL") . ", 'A') RETURNING creid;";

		$creid = $db->pegaUm ( $sql );
		
		$sql = "INSERT INTO projovemurbano.coordgeral(
	            oppid, proid, tprid, cgeqtd, cgehorpagrec, cgevlrhora, cgevlrcomp, 
	            cgeqtdmeses, cgeencargos, cgevlrtotal, cgestatus,creid)
	    		VALUES (" . (($dados ['coordgeral'] ['oppid']) ? "'" . $dados ['coordgeral'] ['oppid'] . "'" : "NULL") . ", 
	    				'" . $_SESSION ['projovemurbano'] ['proid'] . "', 
	    				'{$_SESSION['projovemurbano']['tprid']}',
	    				" . (($dados ['coordgeral'] ['cgeqtd']) ? "'" . $dados ['coordgeral'] ['cgeqtd'] . "'" : "NULL") . ", 
	    				" . (($dados ['coordgeral'] ['cgehorpagrec']) ? "'" . $dados ['coordgeral'] ['cgehorpagrec'] . "'" : "NULL") . ", 
	    				" . (($dados ['coordgeral'] ['cgevlrhora']) ? "'" . str_replace ( array (".",","), array ("","."), $dados ['coordgeral'] ['cgevlrhora'] ) . "'" : "NULL") . ", 
	    				" . (($dados ['coordgeral'] ['cgevlrcomp']) ? "'" . str_replace ( array (".",","), array ("","."), $dados ['coordgeral'] ['cgevlrcomp'] ) . "'" : "NULL") . ", 
	            		" . (($dados ['coordgeral'] ['cgeqtdmeses']) ? "'" . $dados ['coordgeral'] ['cgeqtdmeses'] . "'" : "NULL") . ", 
	            		" . (($dados ['coordgeral'] ['cgeencargos']) ? "'" . $dados ['coordgeral'] ['cgeencargos'] . "'" : "NULL") . ", 
	            		" . (($dados ['coordgeral'] ['cgevlrtotal']) ? "'" . str_replace ( array (".",","), array ("","."), $dados ['coordgeral'] ['cgevlrtotal'] ) . "'" : "NULL") . ", 
	            		'A',
						$creid);";
	}
	
	if($sql){
		$db->executar ( $sql );
		
		$db->commit ();
	}
	$coaid_A = $db->pegaLinha ( "SELECT coaid,ccmrid FROM projovemurbano.coordassistentes 
							WHERE proid='" . $_SESSION ['projovemurbano'] ['proid'] . "' AND tprid = '{$_SESSION['projovemurbano']['tprid']}' AND coatipo='A' AND coastatus='A'" );
	$coaid_A_qtd = $db->pegaUm ( "SELECT count(coaid) FROM projovemurbano.coordassistentes WHERE proid='" . $_SESSION ['projovemurbano'] ['proid'] . "' AND coatipo='A' AND coastatus='A'" );
	
	if ($coaid_A && $dados ['tprid_origem'] == $_SESSION ['projovemurbano'] ['tprid']) {
		
		$sql = "UPDATE projovemurbano.contratadocomp
				   SET ccmqtd=" . (($dados ['coordassistentes'] ['A'] ['coaqtd']) ? "'" . $dados ['coordassistentes'] ['A'] ['coaqtd'] . "'" : "NULL") . ",
				   	   ccmqtdhoras=" . (($dados ['contratadocomp'] ['A'] ['ccmqtdhoras']) ? "'" . $dados ['contratadocomp'] ['A'] ['ccmqtdhoras'] . "'" : "NULL") . ",
				   	   ccmvlrhora=" . (($dados ['contratadocomp'] ['A'] ['ccmvlrhora']) ? "'" . str_replace ( array (
				".",
				"," 
		), array (
				"",
				"." 
		), $dados ['contratadocomp'] ['A'] ['ccmvlrhora'] ) . "'" : "NULL") . ",
				   	   ccmvlrcomp=" . (($dados ['contratadocomp'] ['A'] ['ccmvlrcomp']) ? "'" . str_replace ( array (
				".",
				"," 
		), array (
				"",
				"." 
		), $dados ['contratadocomp'] ['A'] ['ccmvlrcomp'] ) . "'" : "NULL") . ",
				       ccmqtdmeses=" . (($dados ['contratadocomp'] ['A'] ['ccmqtdmeses']) ? "'" . $dados ['contratadocomp'] ['A'] ['ccmqtdmeses'] . "'" : "NULL") . ",
				       ccmencargos=" . (($dados ['contratadocomp'] ['A'] ['ccmencargos']) ? "'" . $dados ['contratadocomp'] ['A'] ['ccmencargos'] . "'" : "NULL") . ",
				       ccmvlrtotal=" . (($dados ['contratadocomp'] ['A'] ['ccmvlrtotal']) ? "'" . str_replace ( array (
				".",
				"," 
		), array (
				"",
				"." 
		), $dados ['contratadocomp'] ['A'] ['ccmvlrtotal'] ) . "'" : "NULL") . "
				 WHERE ccmrid='" . $coaid_A ['ccmrid'] . "';";
		$db->executar ( $sql );
		
		$sql = "UPDATE projovemurbano.coordassistentes
   				SET oppid=" . (($dados ['coordassistentes'] ['a'] ['oppid']) ? "'" . $dados ['coordassistentes'] ['a'] ['oppid'] . "'" : "NULL") . ", 
   					coaqtd=" . (($dados ['coordassistentes'] ['A'] ['coaqtd']) ? "'" . $dados ['coordassistentes'] ['A'] ['coaqtd'] . "'" : "NULL") . ", 
   					coavlrbrutorem=" . (($dados ['coordassistentes'] ['A'] ['coavlrbrutorem']) ? "'" . str_replace ( array (
				".",
				"," 
		), array (
				"",
				"." 
		), $dados ['coordassistentes'] ['A'] ['coavlrbrutorem'] ) . "'" : "NULL") . ", 
       				coaqtdmeses=" . (($dados ['coordassistentes'] ['A'] ['coaqtdmeses']) ? "'" . $dados ['coordassistentes'] ['A'] ['coaqtdmeses'] . "'" : "NULL") . ", 
       				coaencargos=" . (($dados ['coordassistentes'] ['A'] ['coaencargos']) ? "'" . $dados ['coordassistentes'] ['A'] ['coaencargos'] . "'" : "NULL") . ",
       				coavlrtotal=" . (($dados ['coordassistentes'] ['A'] ['coavlrtotal']) ? "'" . str_replace ( array (
				".",
				"," 
		), array (
				"",
				"." 
		), $dados ['coordassistentes'] ['A'] ['coavlrtotal'] ) . "'" : "NULL") . "
 				WHERE coaid='" . $coaid_A ['coaid'] . "';";
	} elseif (! $coaid_A_qtd) {
		$sql = "INSERT INTO projovemurbano.contratadocomp(
					            ccmqtd, ccmqtdhoras, ccmvlrhora, ccmvlrcomp, ccmqtdmeses,
					            ccmencargos, ccmvlrtotal, ccmstatus)
					    VALUES (" . (($dados ['coordassistentes'] ['A'] ['coaqtd']) ? "'" . $dados ['coordassistentes'] ['A'] ['coaqtd'] . "'" : "NULL") . ",
					    		" . (($dados ['contratadocomp'] ['A'] ['ccmqtdhoras']) ? "'" . $dados ['contratadocomp'] ['A'] ['ccmqtdhoras'] . "'" : "NULL") . ",
					    		" . (($dados ['contratadocomp'] ['A'] ['ccmvlrhora']) ? "'" . str_replace ( array (
				".",
				"," 
		), array (
				"",
				"." 
		), $dados ['contratadocomp'] ['A'] ['ccmvlrhora'] ) . "'" : "NULL") . ",
					    		" . (($dados ['contratadocomp'] ['A'] ['ccmvlrcomp']) ? "'" . str_replace ( array (
				".",
				"," 
		), array (
				"",
				"." 
		), $dados ['contratadocomp'] ['A'] ['ccmvlrcomp'] ) . "'" : "NULL") . ",
					    		" . (($dados ['contratadocomp'] ['A'] ['ccmqtdmeses']) ? "'" . $dados ['contratadocomp'] ['A'] ['ccmqtdmeses'] . "'" : "NULL") . ",
					    		" . (($dados ['contratadocomp'] ['A'] ['ccmencargos']) ? "'" . $dados ['contratadocomp'] ['A'] ['ccmencargos'] . "'" : "NULL") . ",
					            " . (($dados ['contratadocomp'] ['A'] ['ccmvlrtotal']) ? "'" . str_replace ( array (
				".",
				"," 
		), array (
				"",
				"." 
		), $dados ['contratadocomp'] ['A'] ['ccmvlrtotal'] ) . "'" : "NULL") . ",
					            'A') RETURNING ccmrid;";
		$ccmrid = $db->pegaUm ( $sql );
		
		$sql = "INSERT INTO projovemurbano.coordassistentes(
	            proid, tprid,oppid, coatipo, coaqtd, coahorpagrec, coavlrhora, coavlrbrutorem, coaqtdmeses, 
	            coaencargos, coavlrtotal, coastatus,ccmrid)
	    		VALUES ('" . $_SESSION ['projovemurbano'] ['proid'] . "', '{$_SESSION['projovemurbano']['tprid']}',
	    				" . (($dados ['coordassistentes'] ['a'] ['oppid']) ? "'" . $dados ['coordassistentes'] ['a'] ['oppid'] . "'" : "NULL") . ", 
	    				'A', 
	    				" . (($dados ['coordassistentes'] ['A'] ['coaqtd']) ? "'" . $dados ['coordassistentes'] ['A'] ['coaqtd'] . "'" : "NULL") . ",
				 		" . (($dados ['coordassistentes'] ['A'] ['coahorpagrec']) ? "'" . $dados ['coordassistentes'] ['A'] ['coahorpagrec'] . "'" : "NULL") . ", 
	    				" . (($dados ['coordassistentes'] ['A'] ['coavlrhora']) ? "'" . str_replace ( array (
				".",
				"," 
		), array (
				"",
				"." 
		), $dados ['coordassistentes'] ['A'] ['coavlrhora'] ) . "'" : "NULL") . ", 
	    				" . (($dados ['coordassistentes'] ['A'] ['coavlrbrutorem']) ? "'" . str_replace ( array (
				".",
				"," 
		), array (
				"",
				"." 
		), $dados ['coordassistentes'] ['A'] ['coavlrbrutorem'] ) . "'" : "NULL") . ", 
	    				" . (($dados ['coordassistentes'] ['A'] ['coaqtdmeses']) ? "'" . $dados ['coordassistentes'] ['A'] ['coaqtdmeses'] . "'" : "NULL") . ", 
	            		" . (($dados ['coordassistentes'] ['A'] ['coaencargos']) ? "'" . $dados ['coordassistentes'] ['A'] ['coaencargos'] . "'" : "NULL") . ", 
	            		" . (($dados ['coordassistentes'] ['A'] ['coavlrtotal']) ? "'" . str_replace ( array (
				".",
				"," 
		), array (
				"",
				"." 
		), $dados ['coordassistentes'] ['A'] ['coavlrtotal'] ) . "'" : "NULL") . ", 'A',$ccmrid);";
	}
	$db->executar ( $sql );
	
	$db->commit ();
	
	$coaid_P = $db->pegaLinha ( "SELECT coaid,ccmrid FROM projovemurbano.coordassistentes WHERE proid='" . $_SESSION ['projovemurbano'] ['proid'] . "' AND coatipo='P' AND coastatus='A' AND tprid = '{$_SESSION['projovemurbano']['tprid']}'" );
	$coaid_P_qtd = $db->pegaUm ( "SELECT count(coaid) FROM projovemurbano.coordassistentes WHERE proid='" . $_SESSION ['projovemurbano'] ['proid'] . "' AND coatipo='P' AND coastatus='A'" );
	
	if ($coaid_P && $dados ['tprid_origem'] == $_SESSION ['projovemurbano'] ['tprid']) {
		
		$sql = "UPDATE projovemurbano.contratadocomp
				   SET ccmqtd=" . (($dados ['coordassistentes'] ['P'] ['coaqtd']) ? "'" . $dados ['coordassistentes'] ['P'] ['coaqtd'] . "'" : "NULL") . ",
				   	   ccmqtdhoras=" . (($dados ['contratadocomp'] ['P'] ['ccmqtdhoras']) ? "'" . $dados ['contratadocomp'] ['P'] ['ccmqtdhoras'] . "'" : "NULL") . ",
				   	   ccmvlrhora=" . (($dados ['contratadocomp'] ['P'] ['ccmvlrhora']) ? "'" . str_replace ( array (
				".",
				"," 
		), array (
				"",
				"." 
		), $dados ['contratadocomp'] ['P'] ['ccmvlrhora'] ) . "'" : "NULL") . ",
				   	   ccmvlrcomp=" . (($dados ['contratadocomp'] ['P'] ['ccmvlrcomp']) ? "'" . str_replace ( array (
				".",
				"," 
		), array (
				"",
				"." 
		), $dados ['contratadocomp'] ['P'] ['ccmvlrcomp'] ) . "'" : "NULL") . ",
				       ccmqtdmeses=" . (($dados ['contratadocomp'] ['P'] ['ccmqtdmeses']) ? "'" . $dados ['contratadocomp'] ['P'] ['ccmqtdmeses'] . "'" : "NULL") . ",
				       ccmencargos=" . (($dados ['contratadocomp'] ['P'] ['ccmencargos']) ? "'" . $dados ['contratadocomp'] ['P'] ['ccmencargos'] . "'" : "NULL") . ",
				       ccmvlrtotal=" . (($dados ['contratadocomp'] ['P'] ['ccmvlrtotal']) ? "'" . str_replace ( array (
				".",
				"," 
		), array (
				"",
				"." 
		), $dados ['contratadocomp'] ['P'] ['ccmvlrtotal'] ) . "'" : "NULL") . "
				 WHERE ccmrid='" . $coaid_P ['ccmrid'] . "';";
		$db->executar ( $sql );
		
		$sql = "UPDATE projovemurbano.coordassistentes
				SET oppid=" . (($dados ['coordassistentes'] ['p'] ['oppid']) ? "'" . $dados ['coordassistentes'] ['p'] ['oppid'] . "'" : "NULL") . ",
					coaqtd=" . (($dados ['coordassistentes'] ['P'] ['coaqtd']) ? "'" . $dados ['coordassistentes'] ['P'] ['coaqtd'] . "'" : "NULL") . ",
					coavlrbrutorem=" . (($dados ['coordassistentes'] ['P'] ['coavlrbrutorem']) ? "'" . str_replace ( array (
				".",
				"," 
		), array (
				"",
				"." 
		), $dados ['coordassistentes'] ['P'] ['coavlrbrutorem'] ) . "'" : "NULL") . ",
					coaqtdmeses=" . (($dados ['coordassistentes'] ['P'] ['coaqtdmeses']) ? "'" . $dados ['coordassistentes'] ['P'] ['coaqtdmeses'] . "'" : "NULL") . ",
					coaencargos=" . (($dados ['coordassistentes'] ['P'] ['coaencargos']) ? "'" . $dados ['coordassistentes'] ['P'] ['coaencargos'] . "'" : "NULL") . ",
					coahorpagrec=" . (($dados ['coordassistentes'] ['P'] ['coahorpagrec']) ? "'" . $dados ['coordassistentes'] ['P'] ['coahorpagrec'] . "'" : "NULL") . ", 
   					coavlrhora=" . (($dados ['coordassistentes'] ['P'] ['coavlrhora']) ? "'" . str_replace ( array (
				".",
				"," 
		), array (
				"",
				"." 
		), $dados ['coordassistentes'] ['P'] ['coavlrhora'] ) . "'" : "NULL") . ",
					coavlrtotal=" . (($dados ['coordassistentes'] ['P'] ['coavlrtotal']) ? "'" . str_replace ( array (
				".",
				"," 
		), array (
				"",
				"." 
		), $dados ['coordassistentes'] ['P'] ['coavlrtotal'] ) . "'" : "NULL") . "
				WHERE coaid='" . $coaid_P ['coaid'] . "';";
	} elseif (! $coaid_P_qtd) {
		
		$sql = "INSERT INTO projovemurbano.contratadocomp(
					            ccmqtd, ccmqtdhoras, ccmvlrhora, ccmvlrcomp, ccmqtdmeses,
					            ccmencargos, ccmvlrtotal, ccmstatus)
					    VALUES (" . (($dados ['coordassistentes'] ['P'] ['coaqtd']) ? "'" . $dados ['coordassistentes'] ['P'] ['coaqtd'] . "'" : "NULL") . ",
					    		" . (($dados ['contratadocomp'] ['P'] ['ccmqtdhoras']) ? "'" . $dados ['contratadocomp'] ['P'] ['ccmqtdhoras'] . "'" : "NULL") . ",
					    		" . (($dados ['contratadocomp'] ['P'] ['ccmvlrhora']) ? "'" . str_replace ( array (
				".",
				"," 
		), array (
				"",
				"." 
		), $dados ['contratadocomp'] ['P'] ['ccmvlrhora'] ) . "'" : "NULL") . ",
					    		" . (($dados ['contratadocomp'] ['P'] ['ccmvlrcomp']) ? "'" . str_replace ( array (
				".",
				"," 
		), array (
				"",
				"." 
		), $dados ['contratadocomp'] ['P'] ['ccmvlrcomp'] ) . "'" : "NULL") . ",
					    		" . (($dados ['contratadocomp'] ['P'] ['ccmqtdmeses']) ? "'" . $dados ['contratadocomp'] ['P'] ['ccmqtdmeses'] . "'" : "NULL") . ",
					    		" . (($dados ['contratadocomp'] ['P'] ['ccmencargos']) ? "'" . $dados ['contratadocomp'] ['P'] ['ccmencargos'] . "'" : "NULL") . ",
					            " . (($dados ['contratadocomp'] ['P'] ['ccmvlrtotal']) ? "'" . str_replace ( array (
				".",
				"," 
		), array (
				"",
				"." 
		), $dados ['contratadocomp'] ['P'] ['ccmvlrtotal'] ) . "'" : "NULL") . ",
					            'A') RETURNING ccmrid;";
		$ccmrid = $db->pegaUm ( $sql );
		
		$sql = "INSERT INTO projovemurbano.coordassistentes(
					proid, tprid, oppid, coatipo, coaqtd, coahorpagrec, coavlrhora, coavlrbrutorem, coaqtdmeses,
					coaencargos, coavlrtotal, coastatus,ccmrid)
				VALUES ('" . $_SESSION ['projovemurbano'] ['proid'] . "', '{$_SESSION['projovemurbano']['tprid']}',
					" . (($dados ['coordassistentes'] ['p'] ['oppid']) ? "'" . $dados ['coordassistentes'] ['p'] ['oppid'] . "'" : "NULL") . ",
					'P',
					" . (($dados ['coordassistentes'] ['P'] ['coaqtd']) ? "'" . $dados ['coordassistentes'] ['P'] ['coaqtd'] . "'" : "NULL") . ",
					" . (($dados ['coordassistentes'] ['P'] ['coahorpagrec']) ? "'" . $dados ['coordassistentes'] ['P'] ['coahorpagrec'] . "'" : "NULL") . ", 
	    			" . (($dados ['coordassistentes'] ['P'] ['coavlrhora']) ? "'" . str_replace ( array (
				".",
				"," 
		), array (
				"",
				"." 
		), $dados ['coordassistentes'] ['P'] ['coavlrhora'] ) . "'" : "NULL") . ", 
					" . (($dados ['coordassistentes'] ['P'] ['coavlrbrutorem']) ? "'" . str_replace ( array (
				".",
				"," 
		), array (
				"",
				"." 
		), $dados ['coordassistentes'] ['P'] ['coavlrbrutorem'] ) . "'" : "NULL") . ",
					" . (($dados ['coordassistentes'] ['P'] ['coaqtdmeses']) ? "'" . $dados ['coordassistentes'] ['P'] ['coaqtdmeses'] . "'" : "NULL") . ",
					" . (($dados ['coordassistentes'] ['P'] ['coaencargos']) ? "'" . $dados ['coordassistentes'] ['P'] ['coaencargos'] . "'" : "NULL") . ",
					" . (($dados ['coordassistentes'] ['P'] ['coavlrtotal']) ? "'" . str_replace ( array (
				".",
				"," 
		), array (
				"",
				"." 
		), $dados ['coordassistentes'] ['P'] ['coavlrtotal'] ) . "'" : "NULL") . ", 'A',$ccmrid);";
	}
	
	$db->executar ( $sql );
	
	$db->commit ();
	
	$sql = "SELECT 
				coaid,ccmrid
			FROM 
				projovemurbano.coordassistentes 
			WHERE 
				proid='" . $_SESSION ['projovemurbano'] ['proid'] . "' 
				AND tprid = '{$_SESSION['projovemurbano']['tprid']}' 
				AND coatipo='AP' 
				AND coastatus='A'";
	
	$coaid_PA = $db->pegaLinha ( $sql );
	
	$sql = "SELECT 
				count(coaid) 
			FROM 
				projovemurbano.coordassistentes 
			WHERE 
				proid='" . $_SESSION ['projovemurbano'] ['proid'] . "' 
				AND tprid = '{$_SESSION['projovemurbano']['tprid']}' 
				AND coatipo='AP' 
				AND coastatus='A'";
	
	$coaid_PA_qtd = $db->pegaUm ( $sql );
	if ($coaid_PA) {
		
		$sql = "UPDATE projovemurbano.contratadocomp
				   SET ccmqtd=" . (($dados ['coordassistentes'] ['PA'] ['coaqtd']) ? "'" . $dados ['coordassistentes'] ['PA'] ['coaqtd'] . "'" : "NULL") . ",
				   	   ccmqtdhoras=" . (($dados ['contratadocomp'] ['PA'] ['ccmqtdhoras']) ? "'" . $dados ['contratadocomp'] ['PA'] ['ccmqtdhoras'] . "'" : "NULL") . ",
				   	   ccmvlrhora=" . (($dados ['contratadocomp'] ['PA'] ['ccmvlrhora']) ? "'" . str_replace ( array (
				".",
				"," 
		), array (
				"",
				"." 
		), $dados ['contratadocomp'] ['PA'] ['ccmvlrhora'] ) . "'" : "NULL") . ",
				   	   ccmvlrcomp=" . (($dados ['contratadocomp'] ['PA'] ['ccmvlrcomp']) ? "'" . str_replace ( array (
				".",
				"," 
		), array (
				"",
				"." 
		), $dados ['contratadocomp'] ['PA'] ['ccmvlrcomp'] ) . "'" : "NULL") . ",
				       ccmqtdmeses=" . (($dados ['contratadocomp'] ['PA'] ['ccmqtdmeses']) ? "'" . $dados ['contratadocomp'] ['PA'] ['ccmqtdmeses'] . "'" : "NULL") . ",
				       ccmencargos=" . (($dados ['contratadocomp'] ['PA'] ['ccmencargos']) ? "'" . $dados ['contratadocomp'] ['PA'] ['ccmencargos'] . "'" : "NULL") . ",
				       ccmvlrtotal=" . (($dados ['contratadocomp'] ['PA'] ['ccmvlrtotal']) ? "'" . str_replace ( array (
				".",
				"," 
		), array (
				"",
				"." 
		), $dados ['contratadocomp'] ['PA'] ['ccmvlrtotal'] ) . "'" : "NULL") . "
				 WHERE ccmrid='" . $coaid_PA ['ccmrid'] . "';";
		$db->executar ( $sql );
		
		$sql = "UPDATE projovemurbano.coordassistentes
   				SET oppid=" . (($dados ['coordassistentes'] ['ap'] ['oppid']) ? "'" . $dados ['coordassistentes'] ['ap'] ['oppid'] . "'" : "NULL") . ", 
   					coaqtd=" . (($dados ['coordassistentes'] ['PA'] ['coaqtd']) ? "'" . $dados ['coordassistentes'] ['PA'] ['coaqtd'] . "'" : "NULL") . ", 
   					coavlrbrutorem=" . (($dados ['coordassistentes'] ['PA'] ['coavlrbrutorem']) ? "'" . str_replace ( array (
				".",
				"," 
		), array (
				"",
				"." 
		), $dados ['coordassistentes'] ['PA'] ['coavlrbrutorem'] ) . "'" : "NULL") . ", 
       				coaqtdmeses=" . (($dados ['coordassistentes'] ['PA'] ['coaqtdmeses']) ? "'" . $dados ['coordassistentes'] ['PA'] ['coaqtdmeses'] . "'" : "NULL") . ", 
       				coaencargos=" . (($dados ['coordassistentes'] ['PA'] ['coaencargos']) ? "'" . $dados ['coordassistentes'] ['PA'] ['coaencargos'] . "'" : "NULL") . ", 
       				coavlrtotal=" . (($dados ['coordassistentes'] ['PA'] ['coavlrtotal']) ? "'" . str_replace ( array (
				".",
				"," 
		), array (
				"",
				"." 
		), $dados ['coordassistentes'] ['PA'] ['coavlrtotal'] ) . "'" : "NULL") . "
 				WHERE coaid='" . $coaid_PA ['coaid'] . "';";
	} elseif (! $coaid_PA_qtd) {
// 		ver($dados['contratadocomp']['PA']['ccmqtdhoras'],$dados['contratadocomp']['PA']['ccmvlrhora'],d);
		$sql = "INSERT INTO projovemurbano.contratadocomp(
					            ccmqtd, ccmqtdhoras, ccmvlrhora, ccmvlrcomp, ccmqtdmeses,
					            ccmencargos, ccmvlrtotal, ccmstatus)
					    VALUES (" . (($dados ['coordassistentes'] ['PA'] ['coaqtd']) ? "'" . $dados ['coordassistentes'] ['PA'] ['coaqtd'] . "'" : "NULL") . ",
					    		" . (($dados ['contratadocomp'] ['PA'] ['ccmqtdhoras']) ? "'" . $dados ['contratadocomp'] ['PA'] ['ccmqtdhoras'] . "'" : "NULL") . ",
					    		" . (($dados ['contratadocomp'] ['PA'] ['ccmvlrhora']) ? "'" . str_replace ( array (".",","), array ("","."), $dados ['contratadocomp'] ['PA'] ['ccmvlrhora'] ) . "'" : "NULL") . ",
					    		" . (($dados ['contratadocomp'] ['PA'] ['ccmvlrcomp']) ? "'" . str_replace ( array (
				".",
				"," 
		), array (
				"",
				"." 
		), $dados ['contratadocomp'] ['PA'] ['ccmvlrcomp'] ) . "'" : "NULL") . ",
					    		" . (($dados ['contratadocomp'] ['PA'] ['ccmqtdmeses']) ? "'" . $dados ['contratadocomp'] ['PA'] ['ccmqtdmeses'] . "'" : "NULL") . ",
					    		" . (($dados ['contratadocomp'] ['PA'] ['ccmencargos']) ? "'" . $dados ['contratadocomp'] ['PA'] ['ccmencargos'] . "'" : "NULL") . ",
					            " . (($dados ['contratadocomp'] ['PA'] ['ccmvlrtotal']) ? "'" . str_replace ( array (
				".",
				"," 
		), array (
				"",
				"." 
		), $dados ['contratadocomp'] ['PA'] ['ccmvlrtotal'] ) . "'" : "NULL") . ",
					            'A') RETURNING ccmrid;";
		$ccmrid = $db->pegaUm ( $sql );
		
		$sql = "INSERT INTO projovemurbano.coordassistentes(
	            proid, tprid, oppid, coatipo, coaqtd, coavlrbrutorem, coaqtdmeses, 
	            coaencargos, coavlrtotal, coastatus,ccmrid)
	    		VALUES ('" . $_SESSION ['projovemurbano'] ['proid'] . "', 
	    				'{$_SESSION['projovemurbano']['tprid']}',	    				
	    				" . (($dados ['coordassistentes'] ['ap'] ['oppid']) ? "'" . $dados ['coordassistentes'] ['ap'] ['oppid'] . "'" : "NULL") . ", 
	    				'AP', 
	    				" . (($dados ['coordassistentes'] ['PA'] ['coaqtd']) ? "'" . $dados ['coordassistentes'] ['PA'] ['coaqtd'] . "'" : "NULL") . ", 
	    				" . (($dados ['coordassistentes'] ['PA'] ['coavlrbrutorem']) ? "'" . str_replace ( array (
				".",
				"," 
		), array (
				"",
				"." 
		), $dados ['coordassistentes'] ['PA'] ['coavlrbrutorem'] ) . "'" : "NULL") . ", 
	    				" . (($dados ['coordassistentes'] ['PA'] ['coaqtdmeses']) ? "'" . $dados ['coordassistentes'] ['PA'] ['coaqtdmeses'] . "'" : "NULL") . ", 
	            		" . (($dados ['coordassistentes'] ['PA'] ['coaencargos']) ? "'" . $dados ['coordassistentes'] ['PA'] ['coaencargos'] . "'" : "NULL") . ", 
	            		" . (($dados ['coordassistentes'] ['PA'] ['coavlrtotal']) ? "'" . str_replace ( array (
				".",
				"," 
		), array (
				"",
				"." 
		), $dados ['coordassistentes'] ['PA'] ['coavlrtotal'] ) . "'" : "NULL") . ", 'A',$ccmrid);";
	}
	$db->executar ( $sql );
	
	$db->commit ();
	
	$sql = "SELECT 
				coaid,ccmrid
			FROM 
				projovemurbano.coordassistentes 
			WHERE 
				proid='" . $_SESSION ['projovemurbano'] ['proid'] . "' 
				AND tprid = '{$_SESSION['projovemurbano']['tprid']}' 
				AND coatipo='PP' AND coastatus='A'";
	
	$coaid_PP = $db->pegaLinha ( $sql );
	
	$sql = "SELECT 
				count(coaid) 
			FROM 
				projovemurbano.coordassistentes 
			WHERE 
				proid='" . $_SESSION ['projovemurbano'] ['proid'] . "' 
				AND coatipo='PP' 
				AND coastatus='A' 
				AND tprid = '{$_SESSION['projovemurbano']['tprid']}'";
	
	$coaid_PP_qtd = $db->pegaUm ( $sql );
// 	ver($dados ['contratadocomp'] ['PP'],d);
	if ($coaid_PP) {
		
		$sql = "UPDATE projovemurbano.contratadocomp
				   SET ccmqtd=" . (($dados ['coordassistentes'] ['PP'] ['coaqtd']) ? "'" . $dados ['coordassistentes'] ['PP'] ['coaqtd'] . "'" : "NULL") . ",
				   	   ccmqtdhoras=" . (($dados ['contratadocomp'] ['PP'] ['ccmqtdhoras']) ? "'" . $dados ['contratadocomp'] ['PP'] ['ccmqtdhoras'] . "'" : "NULL") . ",
				   	   ccmvlrhora=" . (($dados ['contratadocomp'] ['PP'] ['ccmvlrhora']) ? "'" . str_replace ( array (
				".",
				"," 
		), array (
				"",
				"." 
		), $dados ['contratadocomp'] ['PP'] ['ccmvlrhora'] ) . "'" : "NULL") . ",
				   	   ccmvlrcomp=" . (($dados ['contratadocomp'] ['PP'] ['ccmvlrcomp']) ? "'" . str_replace ( array (
				".",
				"," 
		), array (
				"",
				"." 
		), $dados ['contratadocomp'] ['PP'] ['ccmvlrcomp'] ) . "'" : "NULL") . ",
				       ccmqtdmeses=" . (($dados ['contratadocomp'] ['PP'] ['ccmqtdmeses']) ? "'" . $dados ['contratadocomp'] ['PP'] ['ccmqtdmeses'] . "'" : "NULL") . ",
				       ccmencargos=" . (($dados ['contratadocomp'] ['PP'] ['ccmencargos']) ? "'" . $dados ['contratadocomp'] ['PP'] ['ccmencargos'] . "'" : "NULL") . ",
				       ccmvlrtotal=" . (($dados ['contratadocomp'] ['PP'] ['ccmvlrtotal']) ? "'" . str_replace ( array (
				".",
				"," 
		), array (
				"",
				"." 
		), $dados ['contratadocomp'] ['PP'] ['ccmvlrtotal'] ) . "'" : "NULL") . "
				 WHERE ccmrid='" . $coaid_PP ['ccmrid'] . "';";
		$db->executar ( $sql );
		
		$sql = "UPDATE projovemurbano.coordassistentes
				SET oppid=" . (($dados ['coordassistentes'] ['pp'] ['oppid']) ? "'" . $dados ['coordassistentes'] ['pp'] ['oppid'] . "'" : "NULL") . ",
					coaqtd=" . (($dados ['coordassistentes'] ['PP'] ['coaqtd']) ? "'" . $dados ['coordassistentes'] ['PP'] ['coaqtd'] . "'" : "NULL") . ",
					coavlrbrutorem=" . (($dados ['coordassistentes'] ['PP'] ['coavlrbrutorem']) ? "'" . str_replace ( array (
				".",
				"," 
		), array (
				"",
				"." 
		), $dados ['coordassistentes'] ['PP'] ['coavlrbrutorem'] ) . "'" : "NULL") . ",
					coaqtdmeses=" . (($dados ['coordassistentes'] ['PP'] ['coaqtdmeses']) ? "'" . $dados ['coordassistentes'] ['PP'] ['coaqtdmeses'] . "'" : "NULL") . ",
					coaencargos=" . (($dados ['coordassistentes'] ['PP'] ['coaencargos']) ? "'" . $dados ['coordassistentes'] ['PP'] ['coaencargos'] . "'" : "NULL") . ",
					coavlrtotal=" . (($dados ['coordassistentes'] ['PP'] ['coavlrtotal']) ? "'" . str_replace ( array (
				".",
				"," 
		), array (
				"",
				"." 
		), $dados ['coordassistentes'] ['PP'] ['coavlrtotal'] ) . "'" : "NULL") . "
				WHERE coaid='" . $coaid_PP ['coaid'] . "';";
	} elseif (! $coaid_PP_qtd) {
		
		$sql = "INSERT INTO projovemurbano.contratadocomp(
					            ccmqtd, ccmqtdhoras, ccmvlrhora, ccmvlrcomp, ccmqtdmeses,
					            ccmencargos, ccmvlrtotal, ccmstatus)
					    VALUES (" . (($dados ['coordassistentes'] ['PP'] ['coaqtd']) ? "'" . $dados ['coordassistentes'] ['PP'] ['coaqtd'] . "'" : "NULL") . ",
					    		" . (($dados ['contratadocomp'] ['PP'] ['ccmqtdhoras']) ? "'" . $dados ['contratadocomp'] ['PP'] ['ccmqtdhoras'] . "'" : "NULL") . ",
					    		" . (($dados ['contratadocomp'] ['PP'] ['ccmvlrhora']) ? "'" . str_replace ( array (
				".",
				"," 
		), array (
				"",
				"." 
		), $dados ['contratadocomp'] ['PP'] ['ccmvlrhora'] ) . "'" : "NULL") . ",
					    		" . (($dados ['contratadocomp'] ['PP'] ['ccmvlrcomp']) ? "'" . str_replace ( array (
				".",
				"," 
		), array (
				"",
				"." 
		), $dados ['contratadocomp'] ['PP'] ['ccmvlrcomp'] ) . "'" : "NULL") . ",
					    		" . (($dados ['contratadocomp'] ['PP'] ['ccmqtdmeses']) ? "'" . $dados ['contratadocomp'] ['PP'] ['ccmqtdmeses'] . "'" : "NULL") . ",
					    		" . (($dados ['contratadocomp'] ['PP'] ['ccmencargos']) ? "'" . $dados ['contratadocomp'] ['PP'] ['ccmencargos'] . "'" : "NULL") . ",
					            " . (($dados ['contratadocomp'] ['PP'] ['ccmvlrtotal']) ? "'" . str_replace ( array (
				".",
				"," 
		), array (
				"",
				"." 
		), $dados ['contratadocomp'] ['PP'] ['ccmvlrtotal'] ) . "'" : "NULL") . ",
					            'A') RETURNING ccmrid;";
		$ccmrid = $db->pegaUm ( $sql );
		
		$sql = "INSERT INTO projovemurbano.coordassistentes(
					proid, tprid, oppid, coatipo, coaqtd, coavlrbrutorem, coaqtdmeses,
					coaencargos, coavlrtotal, coastatus,ccmrid)
				VALUES ('" . $_SESSION ['projovemurbano'] ['proid'] . "', '{$_SESSION['projovemurbano']['tprid']}',
					" . (($dados ['coordassistentes'] ['pp'] ['oppid']) ? "'" . $dados ['coordassistentes'] ['pp'] ['oppid'] . "'" : "NULL") . ",
					'PP',
					" . (($dados ['coordassistentes'] ['PP'] ['coaqtd']) ? "'" . $dados ['coordassistentes'] ['PP'] ['coaqtd'] . "'" : "NULL") . ",
					" . (($dados ['coordassistentes'] ['PP'] ['coavlrbrutorem']) ? "'" . str_replace ( array (
				".",
				"," 
		), array (
				"",
				"." 
		), $dados ['coordassistentes'] ['PP'] ['coavlrbrutorem'] ) . "'" : "NULL") . ",
					" . (($dados ['coordassistentes'] ['PP'] ['coaqtdmeses']) ? "'" . $dados ['coordassistentes'] ['PP'] ['coaqtdmeses'] . "'" : "NULL") . ",
					" . (($dados ['coordassistentes'] ['PP'] ['coaencargos']) ? "'" . $dados ['coordassistentes'] ['PP'] ['coaencargos'] . "'" : "NULL") . ",
					" . (($dados ['coordassistentes'] ['PP'] ['coavlrtotal']) ? "'" . str_replace ( array (
				".",
				"," 
		), array (
				"",
				"." 
		), $dados ['coordassistentes'] ['PP'] ['coavlrtotal'] ) . "'" : "NULL") . ", 'A',$ccmrid);";
	}
	
	$db->executar ( $sql );
	
	$db->commit ();
	
	$sql = "SELECT 
				coaid 
			FROM 
				projovemurbano.coordassistentes 
			WHERE 
				proid='" . $_SESSION ['projovemurbano'] ['proid'] . "' 
				AND tprid = '{$_SESSION['projovemurbano']['tprid']}' 
				AND coatipo='M' 
				AND coastatus='A'";
	
	$coaid_M = $db->pegaUm ( $sql );
	
	$sql = "SELECT 
				count(coaid) 
			FROM 
				projovemurbano.coordassistentes 
			WHERE 
				proid='" . $_SESSION ['projovemurbano'] ['proid'] . "' 
				AND tprid = '{$_SESSION['projovemurbano']['tprid']}' 
				AND coatipo='M' 
				AND coastatus='A'";
	
	$coaid_M_qtd = $db->pegaUm ( $sql );
	
	if ($coaid_M) {
		
		$sql = "UPDATE projovemurbano.coordassistentes
   				SET oppid=" . (($dados ['coordassistentes'] ['m'] ['oppid']) ? "'" . $dados ['coordassistentes'] ['m'] ['oppid'] . "'" : "NULL") . ", 
   					coaqtd=" . (($dados ['coordassistentes'] ['m'] ['coaqtd']) ? "'" . $dados ['coordassistentes'] ['m'] ['coaqtd'] . "'" : "NULL") . ", 
   					coavlrbrutorem=" . (($dados ['coordassistentes'] ['M'] ['coavlrbrutorem']) ? "'" . str_replace ( array (
				".",
				"," 
		), array (
				"",
				"." 
		), $dados ['coordassistentes'] ['M'] ['coavlrbrutorem'] ) . "'" : "NULL") . ", 
       				coaqtdmeses=" . (($dados ['coordassistentes'] ['M'] ['coaqtdmeses']) ? "'" . $dados ['coordassistentes'] ['M'] ['coaqtdmeses'] . "'" : "NULL") . ", 
       				coaencargos=" . (($dados ['coordassistentes'] ['M'] ['coaencargos']) ? "'" . $dados ['coordassistentes'] ['M'] ['coaencargos'] . "'" : "NULL") . ", 
       				coavlrtotal=" . (($dados ['coordassistentes'] ['M'] ['coavlrtotal']) ? "'" . str_replace ( array (
				".",
				"," 
		), array (
				"",
				"." 
		), $dados ['coordassistentes'] ['M'] ['coavlrtotal'] ) . "'" : "NULL") . "
 				WHERE coaid='" . $coaid_M . "';";
	} elseif (! $coaid_M_qtd) {
		
		$sql = "INSERT INTO projovemurbano.coordassistentes(
	            proid, tprid, oppid, coatipo, coaqtd, coavlrbrutorem, coaqtdmeses, 
	            coaencargos, coavlrtotal, coastatus)
	    		VALUES ('" . $_SESSION ['projovemurbano'] ['proid'] . "', '{$_SESSION['projovemurbano']['tprid']}',
	    				" . (($dados ['coordassistentes'] ['m'] ['oppid']) ? "'" . $dados ['coordassistentes'] ['m'] ['oppid'] . "'" : "NULL") . ", 
	    				'M', 
	    				" . (($dados ['coordassistentes'] ['m'] ['coaqtd']) ? "'" . $dados ['coordassistentes'] ['m'] ['coaqtd'] . "'" : "NULL") . ", 
	    				" . (($dados ['coordassistentes'] ['M'] ['coavlrbrutorem']) ? "'" . str_replace ( array (
				".",
				"," 
		), array (
				"",
				"." 
		), $dados ['coordassistentes'] ['M'] ['coavlrbrutorem'] ) . "'" : "NULL") . ", 
	    				" . (($dados ['coordassistentes'] ['M'] ['coaqtdmeses']) ? "'" . $dados ['coordassistentes'] ['M'] ['coaqtdmeses'] . "'" : "NULL") . ", 
	            		" . (($dados ['coordassistentes'] ['M'] ['coaencargos']) ? "'" . $dados ['coordassistentes'] ['M'] ['coaencargos'] . "'" : "NULL") . ", 
	            		" . (($dados ['coordassistentes'] ['M'] ['coavlrtotal']) ? "'" . str_replace ( array (
				".",
				"," 
		), array (
				"",
				"." 
		), $dados ['coordassistentes'] ['M'] ['coavlrtotal'] ) . "'" : "NULL") . ", 'A');";
	}
	// ver($sql,d);
	$db->executar ( $sql );
	
	$db->commit ();
	
	$diretorpolo = $db->pegaLinha ( "SELECT dipid, ccmrid FROM projovemurbano.diretorpolo 
									WHERE proid='" . $_SESSION ['projovemurbano'] ['proid'] . "' AND tprid = '{$_SESSION['projovemurbano']['tprid']}' AND dipstatus='A'" );
	$diretorpolo_qtd = $db->pegaUm ( "SELECT count(dipid) FROM projovemurbano.diretorpolo WHERE proid='" . $_SESSION ['projovemurbano'] ['proid'] . "' AND dipstatus='A'" );
	
	if ($diretorpolo ['dipid'] && $dados ['tprid_origem'] == $_SESSION ['projovemurbano'] ['tprid']) {
		
		$sql = "UPDATE projovemurbano.contratadocomp
				   SET ccmqtd=" . (($dados ['contratadocomp'] ['DP'] ['ccmqtd']) ? "'" . $dados ['contratadocomp'] ['DP'] ['ccmqtd'] . "'" : "NULL") . ", 
				   	   ccmqtdhoras=" . (($dados ['contratadocomp'] ['DP'] ['ccmqtdhoras']) ? "'" . $dados ['contratadocomp'] ['DP'] ['ccmqtdhoras'] . "'" : "NULL") . ", 
				   	   ccmvlrhora=" . (($dados ['contratadocomp'] ['DP'] ['ccmvlrhora']) ? "'" . str_replace ( array (
				".",
				"," 
		), array (
				"",
				"." 
		), $dados ['contratadocomp'] ['DP'] ['ccmvlrhora'] ) . "'" : "NULL") . ", 
				   	   ccmvlrcomp=" . (($dados ['contratadocomp'] ['DP'] ['ccmvlrcomp']) ? "'" . str_replace ( array (
				".",
				"," 
		), array (
				"",
				"." 
		), $dados ['contratadocomp'] ['DP'] ['ccmvlrcomp'] ) . "'" : "NULL") . ", 
				       ccmqtdmeses=" . (($dados ['contratadocomp'] ['DP'] ['ccmqtdmeses']) ? "'" . $dados ['contratadocomp'] ['DP'] ['ccmqtdmeses'] . "'" : "NULL") . ", 
				       ccmencargos=" . (($dados ['contratadocomp'] ['DP'] ['ccmencargos']) ? "'" . $dados ['contratadocomp'] ['DP'] ['ccmencargos'] . "'" : "NULL") . ", 
				       ccmvlrtotal=" . (($dados ['contratadocomp'] ['DP'] ['ccmvlrtotal']) ? "'" . str_replace ( array (
				".",
				"," 
		), array (
				"",
				"." 
		), $dados ['contratadocomp'] ['DP'] ['ccmvlrtotal'] ) . "'" : "NULL") . " 
				 WHERE ccmrid='" . $diretorpolo ['ccmrid'] . "';";
		
		$db->executar ( $sql );
		
		$sql = "UPDATE projovemurbano.diretorpolo
   				SET dipeqtdefetivo40hr=" . (($dados ['diretorpolo'] ['dipeqtdefetivo40hr']) ? "'" . $dados ['diretorpolo'] ['dipeqtdefetivo40hr'] . "'" : "NULL") . ", 
   					dipqtdrecursoproprio=" . (($dados ['diretorpolo'] ['dipqtdrecursoproprio']) ? "'" . $dados ['diretorpolo'] ['dipqtdrecursoproprio'] . "'" : "NULL") . ",
   					dipqtd=" . (($dados ['diretorpolo'] ['dipqtd']) ? "'" . $dados ['diretorpolo'] ['dipqtd'] . "'" : "NULL") . "
 				WHERE dipid='" . $diretorpolo ['dipid'] . "';";
		
		$db->executar ( $sql );
		
		$db->commit ();
	} elseif (! $diretorpolo_qtd) {
		
		$sql = "INSERT INTO projovemurbano.contratadocomp(
			            ccmqtd, ccmqtdhoras, ccmvlrhora, ccmvlrcomp, ccmqtdmeses, 
			            ccmencargos, ccmvlrtotal, ccmstatus)
			    VALUES (" . (($dados ['contratadocomp'] ['DP'] ['ccmqtd']) ? "'" . $dados ['contratadocomp'] ['DP'] ['ccmqtd'] . "'" : "NULL") . ", 
			    		" . (($dados ['contratadocomp'] ['DP'] ['ccmqtdhoras']) ? "'" . $dados ['contratadocomp'] ['DP'] ['ccmqtdhoras'] . "'" : "NULL") . ", 
			    		" . (($dados ['contratadocomp'] ['DP'] ['ccmvlrhora']) ? "'" . str_replace ( array (
				".",
				"," 
		), array (
				"",
				"." 
		), $dados ['contratadocomp'] ['DP'] ['ccmvlrhora'] ) . "'" : "NULL") . ", 
			    		" . (($dados ['contratadocomp'] ['DP'] ['ccmvlrcomp']) ? "'" . str_replace ( array (
				".",
				"," 
		), array (
				"",
				"." 
		), $dados ['contratadocomp'] ['DP'] ['ccmvlrcomp'] ) . "'" : "NULL") . ", 
			    		" . (($dados ['contratadocomp'] ['DP'] ['ccmqtdmeses']) ? "'" . $dados ['contratadocomp'] ['DP'] ['ccmqtdmeses'] . "'" : "NULL") . ", 
			    		" . (($dados ['contratadocomp'] ['DP'] ['ccmencargos']) ? "'" . $dados ['contratadocomp'] ['DP'] ['ccmencargos'] . "'" : "NULL") . ", 
			            " . (($dados ['contratadocomp'] ['DP'] ['ccmvlrtotal']) ? "'" . str_replace ( array (
				".",
				"," 
		), array (
				"",
				"." 
		), $dados ['contratadocomp'] ['DP'] ['ccmvlrtotal'] ) . "'" : "NULL") . ", 
			            'A') RETURNING ccmrid;";
		
		$ccmrid = $db->pegaUm ( $sql );
		
		$sql = "INSERT INTO projovemurbano.diretorpolo(
	            proid, tprid, ccmrid, dipeqtdefetivo40hr, dipqtdrecursoproprio, dipqtd,
	            dipstatus)
	    		VALUES ('" . $_SESSION ['projovemurbano'] ['proid'] . "', '{$_SESSION['projovemurbano']['tprid']}',
	    				'" . $ccmrid . "', 
	    				" . (($dados ['diretorpolo'] ['dipeqtdefetivo40hr']) ? "'" . $dados ['diretorpolo'] ['dipeqtdefetivo40hr'] . "'" : "NULL") . ", 
	    				" . (($dados ['diretorpolo'] ['dipqtdrecursoproprio']) ? "'" . $dados ['diretorpolo'] ['dipqtdrecursoproprio'] . "'" : "NULL") . ", 
	    				" . (($dados ['diretorpolo'] ['dipqtd']) ? "'" . $dados ['diretorpolo'] ['dipqtd'] . "'" : "NULL") . ",
	            		'A');";
		
		$db->executar ( $sql );
		
		$db->commit ();
	}
	
	$dirassistentes_A = $db->pegaLinha ( "SELECT dasid, creid, ccmrid FROM projovemurbano.dirassistentes 
										WHERE proid='" . $_SESSION ['projovemurbano'] ['proid'] . "' AND tprid = '{$_SESSION['projovemurbano']['tprid']}' AND dasstatus='A' AND dastipo='A'" );
	$dirassistentes_A_qtd = $db->pegaUm ( "SELECT count(dasid) FROM projovemurbano.dirassistentes WHERE proid='" . $_SESSION ['projovemurbano'] ['proid'] . "' AND dasstatus='A' AND dastipo='A'" );
	
	if ($dirassistentes_A && $dados ['tprid_origem'] == $_SESSION ['projovemurbano'] ['tprid']) {
		
		$sql = "UPDATE projovemurbano.contratadocomp
				   SET ccmqtd=" . (($dados ['contratadocomp'] ['AA'] ['ccmqtd']) ? "'" . $dados ['contratadocomp'] ['AA'] ['ccmqtd'] . "'" : "NULL") . ",
				   	   ccmqtdhoras=" . (($dados ['contratadocomp'] ['AA'] ['ccmqtdhoras']) ? "'" . $dados ['contratadocomp'] ['AA'] ['ccmqtdhoras'] . "'" : "NULL") . ",
				   	   ccmvlrhora=" . (($dados ['contratadocomp'] ['AA'] ['ccmvlrhora']) ? "'" . str_replace ( array (
				".",
				"," 
		), array (
				"",
				"." 
		), $dados ['contratadocomp'] ['AA'] ['ccmvlrhora'] ) . "'" : "NULL") . ",
				   	   ccmvlrcomp=" . (($dados ['contratadocomp'] ['AA'] ['ccmvlrcomp']) ? "'" . str_replace ( array (
				".",
				"," 
		), array (
				"",
				"." 
		), $dados ['contratadocomp'] ['AA'] ['ccmvlrcomp'] ) . "'" : "NULL") . ",
				       ccmqtdmeses=" . (($dados ['contratadocomp'] ['AA'] ['ccmqtdmeses']) ? "'" . $dados ['contratadocomp'] ['AA'] ['ccmqtdmeses'] . "'" : "NULL") . ",
				       ccmencargos=" . (($dados ['contratadocomp'] ['AA'] ['ccmencargos']) ? "'" . $dados ['contratadocomp'] ['AA'] ['ccmencargos'] . "'" : "NULL") . ",
				       ccmvlrtotal=" . (($dados ['contratadocomp'] ['AA'] ['ccmvlrtotal']) ? "'" . str_replace ( array (
				".",
				"," 
		), array (
				"",
				"." 
		), $dados ['contratadocomp'] ['AA'] ['ccmvlrtotal'] ) . "'" : "NULL") . "
				 WHERE ccmrid='" . $dirassistentes_A ['ccmrid'] . "';";
		$db->executar ( $sql );
		
		$sql = "UPDATE projovemurbano.contratadorecurso
   				SET creqtd=" . (($dados ['contratadorecurso'] ['AA'] ['creqtd']) ? "'" . $dados ['contratadorecurso'] ['AA'] ['creqtd'] . "'" : "NULL") . ", 
   					crevlrbrutorem=" . (($dados ['contratadorecurso'] ['AA'] ['crevlrbrutorem']) ? "'" . str_replace ( array (
				".",
				"," 
		), array (
				"",
				"." 
		), $dados ['contratadorecurso'] ['AA'] ['crevlrbrutorem'] ) . "'" : "NULL") . ", 
   					creqtdmeses=" . (($dados ['contratadorecurso'] ['AA'] ['creqtdmeses']) ? "'" . $dados ['contratadorecurso'] ['AA'] ['creqtdmeses'] . "'" : "NULL") . ", 
   					creencargos=" . (($dados ['contratadorecurso'] ['AA'] ['creencargos']) ? "'" . $dados ['contratadorecurso'] ['AA'] ['creencargos'] . "'" : "NULL") . ", 
			        crevlrtotal=" . (($dados ['contratadorecurso'] ['AA'] ['crevlrtotal']) ? "'" . str_replace ( array (
				".",
				"," 
		), array (
				"",
				"." 
		), $dados ['contratadorecurso'] ['AA'] ['crevlrtotal'] ) . "'" : "NULL") . " 
				WHERE creid='" . $dirassistentes_A ['creid'] . "';";
		
		$db->executar ( $sql );
		
		$sql = "UPDATE projovemurbano.dirassistentes
   				SET dasqtd=" . (($dados ['dirassistentes'] ['A'] ['dasqtd']) ? "'" . $dados ['dirassistentes'] ['A'] ['dasqtd'] . "'" : "NULL") . ", 
   					dasqtdefetivo40hr=" . (($dados ['dirassistentes'] ['A'] ['dasqtdefetivo40hr']) ? "'" . $dados ['dirassistentes'] ['A'] ['dasqtdefetivo40hr'] . "'" : "NULL") . ", 
       			dasqtdrecursoproprio=" . (($dados ['dirassistentes'] ['A'] ['dasqtdrecursoproprio']) ? "'" . $dados ['dirassistentes'] ['A'] ['dasqtdrecursoproprio'] . "'" : "NULL") . "
 				WHERE dasid='" . $dirassistentes_A ['dasid'] . "';";
		
		$db->executar ( $sql );
		
		$db->commit ();
	} elseif (! $dirassistentes_A_qtd) {
		
		$sql = "INSERT INTO projovemurbano.contratadocomp(
			            ccmqtd, ccmqtdhoras, ccmvlrhora, ccmvlrcomp, ccmqtdmeses,
			            ccmencargos, ccmvlrtotal, ccmstatus)
			    VALUES (" . (($dados ['contratadocomp'] ['AA'] ['ccmqtd']) ? "'" . $dados ['contratadocomp'] ['AA'] ['ccmqtd'] . "'" : "NULL") . ",
			    		" . (($dados ['contratadocomp'] ['AA'] ['ccmqtdhoras']) ? "'" . $dados ['contratadocomp'] ['AA'] ['ccmqtdhoras'] . "'" : "NULL") . ",
			    		" . (($dados ['contratadocomp'] ['AA'] ['ccmvlrhora']) ? "'" . str_replace ( array (".",","), array ("","."), $dados ['contratadocomp'] ['AA'] ['ccmvlrhora'] ) . "'" : "NULL") . ",
			    		" . (($dados ['contratadocomp'] ['AA'] ['ccmvlrcomp']) ? "'" . str_replace ( array (".",","), array ("","."), $dados ['contratadocomp'] ['AA'] ['ccmvlrcomp'] ) . "'" : "NULL") . ",
			    		" . (($dados ['contratadocomp'] ['AA'] ['ccmqtdmeses']) ? "'" . $dados ['contratadocomp'] ['AA'] ['ccmqtdmeses'] . "'" : "NULL") . ",
			    		" . (($dados ['contratadocomp'] ['AA'] ['ccmencargos']) ? "'" . $dados ['contratadocomp'] ['AA'] ['ccmencargos'] . "'" : "NULL") . ",
			            " . (($dados ['contratadocomp'] ['AA'] ['ccmvlrtotal']) ? "'" . str_replace ( array (
				".",
				"," 
		), array (
				"",
				"." 
		), $dados ['contratadocomp'] ['AA'] ['ccmvlrtotal'] ) . "'" : "NULL") . ",
			            'A') RETURNING ccmrid;";
		$ccmrid = $db->pegaUm ( $sql );
		
		$sql = "INSERT INTO projovemurbano.contratadorecurso(
		            creqtd, crevlrbrutorem, creqtdmeses, creencargos, crevlrtotal, 
		            crestatus)
	    		VALUES (" . (($dados ['contratadorecurso'] ['AA'] ['creqtd']) ? "'" . $dados ['contratadorecurso'] ['AA'] ['creqtd'] . "'" : "NULL") . ", 
	    				" . (($dados ['contratadorecurso'] ['AA'] ['crevlrbrutorem']) ? "'" . str_replace ( array (
				".",
				"," 
		), array (
				"",
				"." 
		), $dados ['contratadorecurso'] ['AA'] ['crevlrbrutorem'] ) . "'" : "NULL") . ", 
	    				" . (($dados ['contratadorecurso'] ['AA'] ['creqtdmeses']) ? "'" . $dados ['contratadorecurso'] ['AA'] ['creqtdmeses'] . "'" : "NULL") . ", 
	    				" . (($dados ['contratadorecurso'] ['AA'] ['creencargos']) ? "'" . $dados ['contratadorecurso'] ['AA'] ['creencargos'] . "'" : "NULL") . ", 
	    				" . (($dados ['contratadorecurso'] ['AA'] ['crevlrtotal']) ? "'" . str_replace ( array (
				".",
				"," 
		), array (
				"",
				"." 
		), $dados ['contratadorecurso'] ['AA'] ['crevlrtotal'] ) . "'" : "NULL") . ", 'A') RETURNING creid;";
		
		$creid = $db->pegaUm ( $sql );
		
		$sql = "INSERT INTO projovemurbano.dirassistentes(
            	creid, proid, tprid, dastipo, dasqtd, dasqtdefetivo40hr, dasqtdrecursoproprio, 
    	        dasstatus,ccmrid)
    			VALUES ('" . $creid . "', 
    					'" . $_SESSION ['projovemurbano'] ['proid'] . "', 
    					'{$_SESSION['projovemurbano']['tprid']}',
    					'A', 
    					" . (($dados ['dirassistentes'] ['A'] ['dasqtd']) ? "'" . $dados ['dirassistentes'] ['A'] ['dasqtd'] . "'" : "NULL") . ", 
    					" . (($dados ['dirassistentes'] ['A'] ['dasqtdefetivo40hr']) ? "'" . $dados ['dirassistentes'] ['A'] ['dasqtdefetivo40hr'] . "'" : "NULL") . ", 
    					" . (($dados ['dirassistentes'] ['A'] ['dasqtdrecursoproprio']) ? "'" . $dados ['dirassistentes'] ['A'] ['dasqtdrecursoproprio'] . "'" : "NULL") . ", 
            			'A',
						'" . $ccmrid . "');";
		
		$db->executar ( $sql );
		
		$db->commit ();
	}
	
	$dirassistentes_P = $db->pegaLinha ( "SELECT dasid, creid, ccmrid
										FROM projovemurbano.dirassistentes 
										WHERE proid='" . $_SESSION ['projovemurbano'] ['proid'] . "' AND tprid = '{$_SESSION['projovemurbano']['tprid']}' AND dasstatus='A' AND dastipo='P'" );
	$dirassistentes_P_qtd = $db->pegaUm ( "SELECT count(dasid) FROM projovemurbano.dirassistentes WHERE proid='" . $_SESSION ['projovemurbano'] ['proid'] . "' AND dasstatus='A' AND dastipo='P'" );
	
	if ($dirassistentes_P && $dados ['tprid_origem'] == $_SESSION ['projovemurbano'] ['tprid']) {
		
		$sql = "UPDATE projovemurbano.contratadocomp
				   SET ccmqtd=" . (($dados ['contratadocomp'] ['AP'] ['ccmqtd']) ? "'" . $dados ['contratadocomp'] ['AP'] ['ccmqtd'] . "'" : "NULL") . ",
				   	   ccmqtdhoras=" . (($dados ['contratadocomp'] ['AP'] ['ccmqtdhoras']) ? "'" . $dados ['contratadocomp'] ['AP'] ['ccmqtdhoras'] . "'" : "NULL") . ",
				   	   ccmvlrhora=" . (($dados ['contratadocomp'] ['AP'] ['ccmvlrhora']) ? "'" . str_replace ( array (
				".",
				"," 
		), array (
				"",
				"." 
		), $dados ['contratadocomp'] ['AP'] ['ccmvlrhora'] ) . "'" : "NULL") . ",
				   	   ccmvlrcomp=" . (($dados ['contratadocomp'] ['AP'] ['ccmvlrcomp']) ? "'" . str_replace ( array (
				".",
				"," 
		), array (
				"",
				"." 
		), $dados ['contratadocomp'] ['AP'] ['ccmvlrcomp'] ) . "'" : "NULL") . ",
				       ccmqtdmeses=" . (($dados ['contratadocomp'] ['AP'] ['ccmqtdmeses']) ? "'" . $dados ['contratadocomp'] ['AP'] ['ccmqtdmeses'] . "'" : "NULL") . ",
				       ccmencargos=" . (($dados ['contratadocomp'] ['AP'] ['ccmencargos']) ? "'" . $dados ['contratadocomp'] ['AP'] ['ccmencargos'] . "'" : "NULL") . ",
				       ccmvlrtotal=" . (($dados ['contratadocomp'] ['AP'] ['ccmvlrtotal']) ? "'" . str_replace ( array (
				".",
				"," 
		), array (
				"",
				"." 
		), $dados ['contratadocomp'] ['AP'] ['ccmvlrtotal'] ) . "'" : "NULL") . "
				 WHERE ccmrid='" . $dirassistentes_P ['ccmrid'] . "';";
		$db->executar ( $sql );
		
		$sql = "UPDATE projovemurbano.contratadorecurso
   				SET creqtd=" . (($dados ['contratadorecurso'] ['AP'] ['creqtd']) ? "'" . $dados ['contratadorecurso'] ['AP'] ['creqtd'] . "'" : "NULL") . ", 
   					crevlrbrutorem=" . (($dados ['contratadorecurso'] ['AP'] ['crevlrbrutorem']) ? "'" . str_replace ( array (
				".",
				"," 
		), array (
				"",
				"." 
		), $dados ['contratadorecurso'] ['AP'] ['crevlrbrutorem'] ) . "'" : "NULL") . ", 
   					creqtdmeses=" . (($dados ['contratadorecurso'] ['AP'] ['creqtdmeses']) ? "'" . $dados ['contratadorecurso'] ['AP'] ['creqtdmeses'] . "'" : "NULL") . ", 
   					creencargos=" . (($dados ['contratadorecurso'] ['AP'] ['creencargos']) ? "'" . $dados ['contratadorecurso'] ['AP'] ['creencargos'] . "'" : "NULL") . ", 
			        crevlrtotal=" . (($dados ['contratadorecurso'] ['AP'] ['crevlrtotal']) ? "'" . str_replace ( array (
				".",
				"," 
		), array (
				"",
				"." 
		), $dados ['contratadorecurso'] ['AP'] ['crevlrtotal'] ) . "'" : "NULL") . " 
				WHERE creid='" . $dirassistentes_P ['creid'] . "';";
		
		$db->executar ( $sql );
		
		$sql = "UPDATE projovemurbano.dirassistentes
   				SET dasqtd=" . (($dados ['dirassistentes'] ['P'] ['dasqtd']) ? "'" . $dados ['dirassistentes'] ['P'] ['dasqtd'] . "'" : "NULL") . ", 
   					dasqtdefetivo40hr=" . (($dados ['dirassistentes'] ['P'] ['dasqtdefetivo40hr']) ? "'" . $dados ['dirassistentes'] ['P'] ['dasqtdefetivo40hr'] . "'" : "NULL") . ", 
       			dasqtdrecursoproprio=" . (($dados ['dirassistentes'] ['P'] ['dasqtdrecursoproprio']) ? "'" . $dados ['dirassistentes'] ['P'] ['dasqtdrecursoproprio'] . "'" : "NULL") . "
 				WHERE dasid='" . $dirassistentes_P ['dasid'] . "';";
		
		$db->executar ( $sql );
		
		$db->commit ();
	} elseif (! $dirassistentes_P_qtd) {
		
		$sql = "INSERT INTO projovemurbano.contratadocomp(
			            ccmqtd, ccmqtdhoras, ccmvlrhora, ccmvlrcomp, ccmqtdmeses,
			            ccmencargos, ccmvlrtotal, ccmstatus)
			    VALUES (" . (($dados ['contratadocomp'] ['AP'] ['ccmqtd']) ? "'" . $dados ['contratadocomp'] ['AP'] ['ccmqtd'] . "'" : "NULL") . ",
			    		" . (($dados ['contratadocomp'] ['AP'] ['ccmqtdhoras']) ? "'" . $dados ['contratadocomp'] ['AP'] ['ccmqtdhoras'] . "'" : "NULL") . ",
			    		" . (($dados ['contratadocomp'] ['AP'] ['ccmvlrhora']) ? "'" . str_replace ( array (
				".",
				"," 
		), array (
				"",
				"." 
		), $dados ['contratadocomp'] ['AP'] ['ccmvlrhora'] ) . "'" : "NULL") . ",
			    		" . (($dados ['contratadocomp'] ['AP'] ['ccmvlrcomp']) ? "'" . str_replace ( array (
				".",
				"," 
		), array (
				"",
				"." 
		), $dados ['contratadocomp'] ['AP'] ['ccmvlrcomp'] ) . "'" : "NULL") . ",
			    		" . (($dados ['contratadocomp'] ['AP'] ['ccmqtdmeses']) ? "'" . $dados ['contratadocomp'] ['AP'] ['ccmqtdmeses'] . "'" : "NULL") . ",
			    		" . (($dados ['contratadocomp'] ['AP'] ['ccmencargos']) ? "'" . $dados ['contratadocomp'] ['AP'] ['ccmencargos'] . "'" : "NULL") . ",
			            " . (($dados ['contratadocomp'] ['AP'] ['ccmvlrtotal']) ? "'" . str_replace ( array (
				".",
				"," 
		), array (
				"",
				"." 
		), $dados ['contratadocomp'] ['AP'] ['ccmvlrtotal'] ) . "'" : "NULL") . ",
			            'A') RETURNING ccmrid;";
		
		$ccmrid = $db->pegaUm ( $sql );
		
		$sql = "INSERT INTO projovemurbano.contratadorecurso(
            creqtd, crevlrbrutorem, creqtdmeses, creencargos, crevlrtotal, 
            crestatus)
    		VALUES (" . (($dados ['contratadorecurso'] ['AP'] ['creqtd']) ? "'" . $dados ['contratadorecurso'] ['AP'] ['creqtd'] . "'" : "NULL") . ", 
    				" . (($dados ['contratadorecurso'] ['AP'] ['crevlrbrutorem']) ? "'" . str_replace ( array (
				".",
				"," 
		), array (
				"",
				"." 
		), $dados ['contratadorecurso'] ['AP'] ['crevlrbrutorem'] ) . "'" : "NULL") . ", 
    				" . (($dados ['contratadorecurso'] ['AP'] ['creqtdmeses']) ? "'" . $dados ['contratadorecurso'] ['AP'] ['creqtdmeses'] . "'" : "NULL") . ", 
    				" . (($dados ['contratadorecurso'] ['AP'] ['creencargos']) ? "'" . $dados ['contratadorecurso'] ['AP'] ['creencargos'] . "'" : "NULL") . ", 
    				" . (($dados ['contratadorecurso'] ['AP'] ['crevlrtotal']) ? "'" . str_replace ( array (
				".",
				"," 
		), array (
				"",
				"." 
		), $dados ['contratadorecurso'] ['AP'] ['crevlrtotal'] ) . "'" : "NULL") . ", 'A') RETURNING creid;";
		
		$creid = $db->pegaUm ( $sql );
		
		$sql = "INSERT INTO projovemurbano.dirassistentes(
            	creid, proid, tprid, dastipo, dasqtd, dasqtdefetivo40hr, dasqtdrecursoproprio, 
    	        dasstatus,ccmrid)
    			VALUES ('" . $creid . "', 
    					'" . $_SESSION ['projovemurbano'] ['proid'] . "', 
    					'{$_SESSION['projovemurbano']['tprid']}',
    					'P', 
    					" . (($dados ['dirassistentes'] ['P'] ['dasqtd']) ? "'" . $dados ['dirassistentes'] ['P'] ['dasqtd'] . "'" : "NULL") . ", 
    					" . (($dados ['dirassistentes'] ['P'] ['dasqtdefetivo40hr']) ? "'" . $dados ['dirassistentes'] ['P'] ['dasqtdefetivo40hr'] . "'" : "NULL") . ", 
    					" . (($dados ['dirassistentes'] ['P'] ['dasqtdrecursoproprio']) ? "'" . $dados ['dirassistentes'] ['P'] ['dasqtdrecursoproprio'] . "'" : "NULL") . ", 
            			'A',
						'" . $ccmrid . "');";
		
		$db->executar ( $sql );
		
		$db->commit ();
	}
	
	$educadores_F = $db->pegaLinha ( "SELECT eduid, creid, ccmrid FROM projovemurbano.educadores WHERE proid='" . $_SESSION ['projovemurbano'] ['proid'] . "' AND tprid = '{$_SESSION['projovemurbano']['tprid']}' AND edustatus='A' AND edutipo='F'" );
	
	if ($educadores_F) {
		
		$sql = "UPDATE projovemurbano.contratadocomp
				   SET ccmqtd=" . (($dados ['contratadocomp'] ['EF'] ['ccmqtd']) ? "'" . $dados ['contratadocomp'] ['EF'] ['ccmqtd'] . "'" : "NULL") . ", 
				   	   ccmqtdhoras=" . (($dados ['contratadocomp'] ['EF'] ['ccmqtdhoras']) ? "'" . $dados ['contratadocomp'] ['EF'] ['ccmqtdhoras'] . "'" : "NULL") . ", 
				   	   ccmvlrhora=" . (($dados ['contratadocomp'] ['EF'] ['ccmvlrhora']) ? "'" . str_replace ( array (
				".",
				"," 
		), array (
				"",
				"." 
		), $dados ['contratadocomp'] ['EF'] ['ccmvlrhora'] ) . "'" : "NULL") . ", 
				   	   ccmvlrcomp=" . (($dados ['contratadocomp'] ['EF'] ['ccmvlrcomp']) ? "'" . str_replace ( array (
				".",
				"," 
		), array (
				"",
				"." 
		), $dados ['contratadocomp'] ['EF'] ['ccmvlrcomp'] ) . "'" : "NULL") . ", 
				       ccmqtdmeses=" . (($dados ['contratadocomp'] ['EF'] ['ccmqtdmeses']) ? "'" . $dados ['contratadocomp'] ['EF'] ['ccmqtdmeses'] . "'" : "NULL") . ", 
				       ccmencargos=" . (($dados ['contratadocomp'] ['EF'] ['ccmencargos']) ? "'" . $dados ['contratadocomp'] ['EF'] ['ccmencargos'] . "'" : "NULL") . ", 
				       ccmvlrtotal=" . (($dados ['contratadocomp'] ['EF'] ['ccmvlrtotal']) ? "'" . str_replace ( array (
				".",
				"," 
		), array (
				"",
				"." 
		), $dados ['contratadocomp'] ['EF'] ['ccmvlrtotal'] ) . "'" : "NULL") . " 
				 WHERE ccmrid='" . $educadores_F ['ccmrid'] . "';";
		
		$db->executar ( $sql );
		
		$sql = "UPDATE projovemurbano.contratadorecurso
   				SET creqtd=" . (($dados ['contratadorecurso'] ['EF'] ['creqtd']) ? "'" . $dados ['contratadorecurso'] ['EF'] ['creqtd'] . "'" : "NULL") . ", 
   					crevlrbrutorem=" . (($dados ['contratadorecurso'] ['EF'] ['crevlrbrutorem']) ? "'" . str_replace ( array (
				".",
				"," 
		), array (
				"",
				"." 
		), $dados ['contratadorecurso'] ['EF'] ['crevlrbrutorem'] ) . "'" : "NULL") . ", 
   					creqtdmeses=" . (($dados ['contratadorecurso'] ['EF'] ['creqtdmeses']) ? "'" . $dados ['contratadorecurso'] ['EF'] ['creqtdmeses'] . "'" : "NULL") . ", 
   					creencargos=" . (($dados ['contratadorecurso'] ['EF'] ['creencargos']) ? "'" . $dados ['contratadorecurso'] ['EF'] ['creencargos'] . "'" : "NULL") . ", 
			        crevlrtotal=" . (($dados ['contratadorecurso'] ['EF'] ['crevlrtotal']) ? "'" . str_replace ( array (
				".",
				"," 
		), array (
				"",
				"." 
		), $dados ['contratadorecurso'] ['EF'] ['crevlrtotal'] ) . "'" : "NULL") . " 
				WHERE creid='" . $educadores_F ['creid'] . "';";
		
		$db->executar ( $sql );
		
		$sql = "UPDATE projovemurbano.educadores
				   SET eduqtd=" . (($dados ['educadores'] ['F'] ['eduqtd']) ? "'" . $dados ['educadores'] ['F'] ['eduqtd'] . "'" : "NULL") . ", 
				   	   eduefetivo=" . (($dados ['educadores'] ['F'] ['eduefetivo']) ? "'" . $dados ['educadores'] ['F'] ['eduefetivo'] . "'" : "NULL") . ", 
				       eduefetivo30hr=" . (($dados ['educadores'] ['F'] ['eduefetivo30hr']) ? "'" . $dados ['educadores'] ['F'] ['eduefetivo30hr'] . "'" : "NULL") . ", 
				       eduqtdrecursoproprio=" . (($dados ['educadores'] ['F'] ['eduqtdrecursoproprio']) ? "'" . $dados ['educadores'] ['F'] ['eduqtdrecursoproprio'] . "'" : "NULL") . "
				 WHERE eduid='" . $educadores_F ['eduid'] . "';";
		
		$db->executar ( $sql );
		
		$db->commit ();
	} else {
		
		$sql = "INSERT INTO projovemurbano.contratadocomp(
			            ccmqtd, ccmqtdhoras, ccmvlrhora, ccmvlrcomp, ccmqtdmeses, 
			            ccmencargos, ccmvlrtotal, ccmstatus)
			    VALUES (" . (($dados ['contratadocomp'] ['EF'] ['ccmqtd']) ? "'" . $dados ['contratadocomp'] ['EF'] ['ccmqtd'] . "'" : "NULL") . ", 
			    		" . (($dados ['contratadocomp'] ['EF'] ['ccmqtdhoras']) ? "'" . $dados ['contratadocomp'] ['EF'] ['ccmqtdhoras'] . "'" : "NULL") . ", 
			    		" . (($dados ['contratadocomp'] ['EF'] ['ccmvlrhora']) ? "'" . str_replace ( array (
				".",
				"," 
		), array (
				"",
				"." 
		), $dados ['contratadocomp'] ['EF'] ['ccmvlrhora'] ) . "'" : "NULL") . ", 
			    		" . (($dados ['contratadocomp'] ['EF'] ['ccmvlrcomp']) ? "'" . str_replace ( array (
				".",
				"," 
		), array (
				"",
				"." 
		), $dados ['contratadocomp'] ['EF'] ['ccmvlrcomp'] ) . "'" : "NULL") . ", 
			    		" . (($dados ['contratadocomp'] ['EF'] ['ccmqtdmeses']) ? "'" . $dados ['contratadocomp'] ['EF'] ['ccmqtdmeses'] . "'" : "NULL") . ", 
			    		" . (($dados ['contratadocomp'] ['EF'] ['ccmencargos']) ? "'" . $dados ['contratadocomp'] ['EF'] ['ccmencargos'] . "'" : "NULL") . ", 
			            " . (($dados ['contratadocomp'] ['EF'] ['ccmvlrtotal']) ? "'" . str_replace ( array (
				".",
				"," 
		), array (
				"",
				"." 
		), $dados ['contratadocomp'] ['EF'] ['ccmvlrtotal'] ) . "'" : "NULL") . ", 
			            'A') RETURNING ccmrid;";
		
		$ccmrid = $db->pegaUm ( $sql );
		
		$sql = "INSERT INTO projovemurbano.contratadorecurso(
            creqtd, crevlrbrutorem, creqtdmeses, creencargos, crevlrtotal, 
            crestatus)
    		VALUES (" . (($dados ['contratadorecurso'] ['EF'] ['creqtd']) ? "'" . $dados ['contratadorecurso'] ['EF'] ['creqtd'] . "'" : "NULL") . ", 
    				" . (($dados ['contratadorecurso'] ['EF'] ['crevlrbrutorem']) ? "'" . str_replace ( array (
				".",
				"," 
		), array (
				"",
				"." 
		), $dados ['contratadorecurso'] ['EF'] ['crevlrbrutorem'] ) . "'" : "NULL") . ", 
    				" . (($dados ['contratadorecurso'] ['EF'] ['creqtdmeses']) ? "'" . $dados ['contratadorecurso'] ['EF'] ['creqtdmeses'] . "'" : "NULL") . ", 
    				" . (($dados ['contratadorecurso'] ['EF'] ['creencargos']) ? "'" . $dados ['contratadorecurso'] ['EF'] ['creencargos'] . "'" : "NULL") . ", 
    				" . (($dados ['contratadorecurso'] ['EF'] ['crevlrtotal']) ? "'" . str_replace ( array (
				".",
				"," 
		), array (
				"",
				"." 
		), $dados ['contratadorecurso'] ['EF'] ['crevlrtotal'] ) . "'" : "NULL") . ", 'A') RETURNING creid;";
		
		$creid = $db->pegaUm ( $sql );
		
		$sql = "INSERT INTO projovemurbano.educadores(
	            creid, ccmrid, proid, tprid, edutipo, eduqtd, eduefetivo, eduefetivo30hr, 
	            eduqtdrecursoproprio, edustatus)
	    		VALUES ('" . $creid . "', 
	    				'" . $ccmrid . "', 
	    				'" . $_SESSION ['projovemurbano'] ['proid'] . "', 
	    				'{$_SESSION['projovemurbano']['tprid']}',
	    				'F', 
	    				" . (($dados ['educadores'] ['F'] ['eduqtd']) ? "'" . $dados ['educadores'] ['F'] ['eduqtd'] . "'" : "NULL") . ", 
	    				" . (($dados ['educadores'] ['F'] ['eduefetivo']) ? "'" . $dados ['educadores'] ['F'] ['eduefetivo'] . "'" : "NULL") . ", 
	    				" . (($dados ['educadores'] ['F'] ['eduefetivo30hr']) ? "'" . $dados ['educadores'] ['F'] ['eduefetivo30hr'] . "'" : "NULL") . ", 
	            		" . (($dados ['educadores'] ['F'] ['eduqtdrecursoproprio']) ? "'" . $dados ['educadores'] ['F'] ['eduqtdrecursoproprio'] . "'" : "NULL") . ", 
	            		'A');";
		
		$db->executar ( $sql );
		
		$db->commit ();
	}
	
	$educadores_Q = $db->pegaLinha ( "SELECT eduid, creid, ccmrid FROM projovemurbano.educadores WHERE proid='" . $_SESSION ['projovemurbano'] ['proid'] . "' AND tprid = '{$_SESSION['projovemurbano']['tprid']}' AND edustatus='A' AND edutipo='Q'" );
	
	if ($educadores_Q) {
		
		$sql = "UPDATE projovemurbano.contratadocomp
				   SET ccmqtd=" . (($dados ['contratadocomp'] ['EQ'] ['ccmqtd']) ? "'" . $dados ['contratadocomp'] ['EQ'] ['ccmqtd'] . "'" : "NULL") . ", 
				   	   ccmqtdhoras=" . (($dados ['contratadocomp'] ['EQ'] ['ccmqtdhoras']) ? "'" . $dados ['contratadocomp'] ['EQ'] ['ccmqtdhoras'] . "'" : "NULL") . ", 
				   	   ccmvlrhora=" . (($dados ['contratadocomp'] ['EQ'] ['ccmvlrhora']) ? "'" . str_replace ( array (
				".",
				"," 
		), array (
				"",
				"." 
		), $dados ['contratadocomp'] ['EQ'] ['ccmvlrhora'] ) . "'" : "NULL") . ", 
				   	   ccmvlrcomp=" . (($dados ['contratadocomp'] ['EQ'] ['ccmvlrcomp']) ? "'" . str_replace ( array (
				".",
				"," 
		), array (
				"",
				"." 
		), $dados ['contratadocomp'] ['EQ'] ['ccmvlrcomp'] ) . "'" : "NULL") . ", 
				       ccmqtdmeses=" . (($dados ['contratadocomp'] ['EQ'] ['ccmqtdmeses']) ? "'" . $dados ['contratadocomp'] ['EQ'] ['ccmqtdmeses'] . "'" : "NULL") . ", 
				       ccmencargos=" . (($dados ['contratadocomp'] ['EQ'] ['ccmencargos']) ? "'" . $dados ['contratadocomp'] ['EQ'] ['ccmencargos'] . "'" : "NULL") . ", 
				       ccmvlrtotal=" . (($dados ['contratadocomp'] ['EQ'] ['ccmvlrtotal']) ? "'" . str_replace ( array (
				".",
				"," 
		), array (
				"",
				"." 
		), $dados ['contratadocomp'] ['EQ'] ['ccmvlrtotal'] ) . "'" : "NULL") . " 
				 WHERE ccmrid='" . $educadores_Q ['ccmrid'] . "';";
		
		$db->executar ( $sql );
		
		$sql = "UPDATE projovemurbano.contratadorecurso
   				SET creqtd=" . (($dados ['contratadorecurso'] ['EQ'] ['creqtd']) ? "'" . $dados ['contratadorecurso'] ['EQ'] ['creqtd'] . "'" : "NULL") . ", 
   					crevlrbrutorem=" . (($dados ['contratadorecurso'] ['EQ'] ['crevlrbrutorem']) ? "'" . str_replace ( array (
				".",
				"," 
		), array (
				"",
				"." 
		), $dados ['contratadorecurso'] ['EQ'] ['crevlrbrutorem'] ) . "'" : "NULL") . ", 
   					creqtdmeses=" . (($dados ['contratadorecurso'] ['EQ'] ['creqtdmeses']) ? "'" . $dados ['contratadorecurso'] ['EQ'] ['creqtdmeses'] . "'" : "NULL") . ", 
   					creencargos=" . (($dados ['contratadorecurso'] ['EQ'] ['creencargos']) ? "'" . $dados ['contratadorecurso'] ['EQ'] ['creencargos'] . "'" : "NULL") . ", 
			        crevlrtotal=" . (($dados ['contratadorecurso'] ['EQ'] ['crevlrtotal']) ? "'" . str_replace ( array (
				".",
				"," 
		), array (
				"",
				"." 
		), $dados ['contratadorecurso'] ['EQ'] ['crevlrtotal'] ) . "'" : "NULL") . " 
				WHERE creid='" . $educadores_Q ['creid'] . "';";
		
		$db->executar ( $sql );
		
		$sql = "UPDATE projovemurbano.educadores
				   SET eduqtd=" . (($dados ['educadores'] ['Q'] ['eduqtd']) ? "'" . $dados ['educadores'] ['Q'] ['eduqtd'] . "'" : "NULL") . ", 
				   	   eduefetivo=" . (($dados ['educadores'] ['Q'] ['eduefetivo']) ? "'" . $dados ['educadores'] ['Q'] ['eduefetivo'] . "'" : "NULL") . ", 
				       eduefetivo30hr=" . (($dados ['educadores'] ['Q'] ['eduefetivo30hr']) ? "'" . $dados ['educadores'] ['Q'] ['eduefetivo30hr'] . "'" : "NULL") . ", 
				       eduqtdrecursoproprio=" . (($dados ['educadores'] ['Q'] ['eduqtdrecursoproprio']) ? "'" . $dados ['educadores'] ['Q'] ['eduqtdrecursoproprio'] . "'" : "NULL") . "
				 WHERE eduid='" . $educadores_Q ['eduid'] . "';";
		
		$db->executar ( $sql );
		
		$db->commit ();
	} else {
		
		$sql = "INSERT INTO projovemurbano.contratadocomp(
			            ccmqtd, ccmqtdhoras, ccmvlrhora, ccmvlrcomp, ccmqtdmeses, 
			            ccmencargos, ccmvlrtotal, ccmstatus)
			    VALUES (" . (($dados ['contratadocomp'] ['EQ'] ['ccmqtd']) ? "'" . $dados ['contratadocomp'] ['EQ'] ['ccmqtd'] . "'" : "NULL") . ", 
			    		" . (($dados ['contratadocomp'] ['EQ'] ['ccmqtdhoras']) ? "'" . $dados ['contratadocomp'] ['EQ'] ['ccmqtdhoras'] . "'" : "NULL") . ", 
			    		" . (($dados ['contratadocomp'] ['EQ'] ['ccmvlrhora']) ? "'" . str_replace ( array (
				".",
				"," 
		), array (
				"",
				"." 
		), $dados ['contratadocomp'] ['EQ'] ['ccmvlrhora'] ) . "'" : "NULL") . ", 
			    		" . (($dados ['contratadocomp'] ['EQ'] ['ccmvlrcomp']) ? "'" . str_replace ( array (
				".",
				"," 
		), array (
				"",
				"." 
		), $dados ['contratadocomp'] ['EQ'] ['ccmvlrcomp'] ) . "'" : "NULL") . ", 
			    		" . (($dados ['contratadocomp'] ['EQ'] ['ccmqtdmeses']) ? "'" . $dados ['contratadocomp'] ['EQ'] ['ccmqtdmeses'] . "'" : "NULL") . ", 
			    		" . (($dados ['contratadocomp'] ['EQ'] ['ccmencargos']) ? "'" . $dados ['contratadocomp'] ['EQ'] ['ccmencargos'] . "'" : "NULL") . ", 
			            " . (($dados ['contratadocomp'] ['EQ'] ['ccmvlrtotal']) ? "'" . str_replace ( array (
				".",
				"," 
		), array (
				"",
				"." 
		), $dados ['contratadocomp'] ['EQ'] ['ccmvlrtotal'] ) . "'" : "NULL") . ", 
			            'A') RETURNING ccmrid;";
		
		$ccmrid = $db->pegaUm ( $sql );
		
		$sql = "INSERT INTO projovemurbano.contratadorecurso(
            creqtd, crevlrbrutorem, creqtdmeses, creencargos, crevlrtotal, 
            crestatus)
    		VALUES (" . (($dados ['contratadorecurso'] ['EQ'] ['creqtd']) ? "'" . $dados ['contratadorecurso'] ['EQ'] ['creqtd'] . "'" : "NULL") . ", 
    				" . (($dados ['contratadorecurso'] ['EQ'] ['crevlrbrutorem']) ? "'" . str_replace ( array (
				".",
				"," 
		), array (
				"",
				"." 
		), $dados ['contratadorecurso'] ['EQ'] ['crevlrbrutorem'] ) . "'" : "NULL") . ", 
    				" . (($dados ['contratadorecurso'] ['EQ'] ['creqtdmeses']) ? "'" . $dados ['contratadorecurso'] ['EQ'] ['creqtdmeses'] . "'" : "NULL") . ", 
    				" . (($dados ['contratadorecurso'] ['EQ'] ['creencargos']) ? "'" . $dados ['contratadorecurso'] ['EQ'] ['creencargos'] . "'" : "NULL") . ", 
    				" . (($dados ['contratadorecurso'] ['EQ'] ['crevlrtotal']) ? "'" . str_replace ( array (
				".",
				"," 
		), array (
				"",
				"." 
		), $dados ['contratadorecurso'] ['EQ'] ['crevlrtotal'] ) . "'" : "NULL") . ", 'A') RETURNING creid;";
		
		$creid = $db->pegaUm ( $sql );
		
		$sql = "INSERT INTO projovemurbano.educadores(
	            creid, ccmrid, proid, tprid, edutipo, eduqtd, eduefetivo, eduefetivo30hr, 
	            eduqtdrecursoproprio, edustatus)
	    		VALUES ('" . $creid . "', 
	    				'" . $ccmrid . "', 
	    				'" . $_SESSION ['projovemurbano'] ['proid'] . "', 
	    				'{$_SESSION['projovemurbano']['tprid']}',
	    				'Q', 
	    				" . (($dados ['educadores'] ['Q'] ['eduqtd']) ? "'" . $dados ['educadores'] ['Q'] ['eduqtd'] . "'" : "NULL") . ", 
	    				" . (($dados ['educadores'] ['Q'] ['eduefetivo']) ? "'" . $dados ['educadores'] ['Q'] ['eduefetivo'] . "'" : "NULL") . ", 
	    				" . (($dados ['educadores'] ['Q'] ['eduefetivo30hr']) ? "'" . $dados ['educadores'] ['Q'] ['eduefetivo30hr'] . "'" : "NULL") . ", 
	            		" . (($dados ['educadores'] ['Q'] ['eduqtdrecursoproprio']) ? "'" . $dados ['educadores'] ['Q'] ['eduqtdrecursoproprio'] . "'" : "NULL") . ", 
	            		'A');";
		
		$db->executar ( $sql );
		
		$db->commit ();
	}
	
	$educadores_P = $db->pegaLinha ( "SELECT eduid, creid, ccmrid FROM projovemurbano.educadores WHERE proid='" . $_SESSION ['projovemurbano'] ['proid'] . "' AND tprid = '{$_SESSION['projovemurbano']['tprid']}' AND edustatus='A' AND edutipo='P'" );
	
	if ($educadores_P) {
		
		$sql = "UPDATE projovemurbano.contratadocomp
				   SET ccmqtd=" . (($dados ['contratadocomp'] ['EP'] ['ccmqtd']) ? "'" . $dados ['contratadocomp'] ['EP'] ['ccmqtd'] . "'" : "NULL") . ", 
				   	   ccmqtdhoras=" . (($dados ['contratadocomp'] ['EP'] ['ccmqtdhoras']) ? "'" . $dados ['contratadocomp'] ['EP'] ['ccmqtdhoras'] . "'" : "NULL") . ", 
				   	   ccmvlrhora=" . (($dados ['contratadocomp'] ['EP'] ['ccmvlrhora']) ? "'" . str_replace ( array (
				".",
				"," 
		), array (
				"",
				"." 
		), $dados ['contratadocomp'] ['EP'] ['ccmvlrhora'] ) . "'" : "NULL") . ", 
				   	   ccmvlrcomp=" . (($dados ['contratadocomp'] ['EP'] ['ccmvlrcomp']) ? "'" . str_replace ( array (
				".",
				"," 
		), array (
				"",
				"." 
		), $dados ['contratadocomp'] ['EP'] ['ccmvlrcomp'] ) . "'" : "NULL") . ", 
				       ccmqtdmeses=" . (($dados ['contratadocomp'] ['EP'] ['ccmqtdmeses']) ? "'" . $dados ['contratadocomp'] ['EP'] ['ccmqtdmeses'] . "'" : "NULL") . ", 
				       ccmencargos=" . (($dados ['contratadocomp'] ['EP'] ['ccmencargos']) ? "'" . $dados ['contratadocomp'] ['EP'] ['ccmencargos'] . "'" : "NULL") . ", 
				       ccmvlrtotal=" . (($dados ['contratadocomp'] ['EP'] ['ccmvlrtotal']) ? "'" . str_replace ( array (
				".",
				"," 
		), array (
				"",
				"." 
		), $dados ['contratadocomp'] ['EP'] ['ccmvlrtotal'] ) . "'" : "NULL") . " 
				 WHERE ccmrid='" . $educadores_P ['ccmrid'] . "';";
		
		$db->executar ( $sql );
		
		$sql = "UPDATE projovemurbano.contratadorecurso
   				SET creqtd=" . (($dados ['contratadorecurso'] ['EP'] ['creqtd']) ? "'" . $dados ['contratadorecurso'] ['EP'] ['creqtd'] . "'" : "NULL") . ", 
   					crevlrbrutorem=" . (($dados ['contratadorecurso'] ['EP'] ['crevlrbrutorem']) ? "'" . str_replace ( array (
				".",
				"," 
		), array (
				"",
				"." 
		), $dados ['contratadorecurso'] ['EP'] ['crevlrbrutorem'] ) . "'" : "NULL") . ", 
   					creqtdmeses=" . (($dados ['contratadorecurso'] ['EP'] ['creqtdmeses']) ? "'" . $dados ['contratadorecurso'] ['EP'] ['creqtdmeses'] . "'" : "NULL") . ", 
   					creencargos=" . (($dados ['contratadorecurso'] ['EP'] ['creencargos']) ? "'" . $dados ['contratadorecurso'] ['EP'] ['creencargos'] . "'" : "NULL") . ", 
			        crevlrtotal=" . (($dados ['contratadorecurso'] ['EP'] ['crevlrtotal']) ? "'" . str_replace ( array (
				".",
				"," 
		), array (
				"",
				"." 
		), $dados ['contratadorecurso'] ['EP'] ['crevlrtotal'] ) . "'" : "NULL") . " 
				WHERE creid='" . $educadores_P ['creid'] . "';";
		
		$db->executar ( $sql );
		
		$sql = "UPDATE projovemurbano.educadores
				   SET eduqtd=" . (($dados ['educadores'] ['P'] ['eduqtd']) ? "'" . $dados ['educadores'] ['P'] ['eduqtd'] . "'" : "NULL") . ", 
				   	   eduefetivo=" . (($dados ['educadores'] ['P'] ['eduefetivo']) ? "'" . $dados ['educadores'] ['P'] ['eduefetivo'] . "'" : "NULL") . ", 
				       eduefetivo30hr=" . (($dados ['educadores'] ['P'] ['eduefetivo30hr']) ? "'" . $dados ['educadores'] ['P'] ['eduefetivo30hr'] . "'" : "NULL") . ", 
				       eduqtdrecursoproprio=" . (($dados ['educadores'] ['P'] ['eduqtdrecursoproprio']) ? "'" . $dados ['educadores'] ['P'] ['eduqtdrecursoproprio'] . "'" : "NULL") . "
				 WHERE eduid='" . $educadores_P ['eduid'] . "';";
		
		$db->executar ( $sql );
		
		$db->commit ();
	} else {
		
		$sql = "INSERT INTO projovemurbano.contratadocomp(
			            ccmqtd, ccmqtdhoras, ccmvlrhora, ccmvlrcomp, ccmqtdmeses, 
			            ccmencargos, ccmvlrtotal, ccmstatus)
			    VALUES (" . (($dados ['contratadocomp'] ['EP'] ['ccmqtd']) ? "'" . $dados ['contratadocomp'] ['EP'] ['ccmqtd'] . "'" : "NULL") . ", 
			    		" . (($dados ['contratadocomp'] ['EP'] ['ccmqtdhoras']) ? "'" . $dados ['contratadocomp'] ['EP'] ['ccmqtdhoras'] . "'" : "NULL") . ", 
			    		" . (($dados ['contratadocomp'] ['EP'] ['ccmvlrhora']) ? "'" . str_replace ( array (
				".",
				"," 
		), array (
				"",
				"." 
		), $dados ['contratadocomp'] ['EP'] ['ccmvlrhora'] ) . "'" : "NULL") . ", 
			    		" . (($dados ['contratadocomp'] ['EP'] ['ccmvlrcomp']) ? "'" . str_replace ( array (
				".",
				"," 
		), array (
				"",
				"." 
		), $dados ['contratadocomp'] ['EP'] ['ccmvlrcomp'] ) . "'" : "NULL") . ", 
			    		" . (($dados ['contratadocomp'] ['EP'] ['ccmqtdmeses']) ? "'" . $dados ['contratadocomp'] ['EP'] ['ccmqtdmeses'] . "'" : "NULL") . ", 
			    		" . (($dados ['contratadocomp'] ['EP'] ['ccmencargos']) ? "'" . $dados ['contratadocomp'] ['EP'] ['ccmencargos'] . "'" : "NULL") . ", 
			            " . (($dados ['contratadocomp'] ['EP'] ['ccmvlrtotal']) ? "'" . str_replace ( array (
				".",
				"," 
		), array (
				"",
				"." 
		), $dados ['contratadocomp'] ['EP'] ['ccmvlrtotal'] ) . "'" : "NULL") . ", 
			            'A') RETURNING ccmrid;";
		
		$ccmrid = $db->pegaUm ( $sql );
		
		$sql = "INSERT INTO projovemurbano.contratadorecurso(
            creqtd, crevlrbrutorem, creqtdmeses, creencargos, crevlrtotal, 
            crestatus)
    		VALUES (" . (($dados ['contratadorecurso'] ['EP'] ['creqtd']) ? "'" . $dados ['contratadorecurso'] ['EP'] ['creqtd'] . "'" : "NULL") . ", 
    				" . (($dados ['contratadorecurso'] ['EP'] ['crevlrbrutorem']) ? "'" . str_replace ( array (
				".",
				"," 
		), array (
				"",
				"." 
		), $dados ['contratadorecurso'] ['EP'] ['crevlrbrutorem'] ) . "'" : "NULL") . ", 
    				" . (($dados ['contratadorecurso'] ['EP'] ['creqtdmeses']) ? "'" . $dados ['contratadorecurso'] ['EP'] ['creqtdmeses'] . "'" : "NULL") . ", 
    				" . (($dados ['contratadorecurso'] ['EP'] ['creencargos']) ? "'" . $dados ['contratadorecurso'] ['EP'] ['creencargos'] . "'" : "NULL") . ", 
    				" . (($dados ['contratadorecurso'] ['EP'] ['crevlrtotal']) ? "'" . str_replace ( array (
				".",
				"," 
		), array (
				"",
				"." 
		), $dados ['contratadorecurso'] ['EP'] ['crevlrtotal'] ) . "'" : "NULL") . ", 'A') RETURNING creid;";
		
		$creid = $db->pegaUm ( $sql );
		
		$sql = "INSERT INTO projovemurbano.educadores(
	            creid, ccmrid, proid, tprid, edutipo, eduqtd, eduefetivo, eduefetivo30hr, 
	            eduqtdrecursoproprio, edustatus)
	    		VALUES ('" . $creid . "', 
	    				'" . $ccmrid . "', 
	    				'" . $_SESSION ['projovemurbano'] ['proid'] . "', 
	    				'{$_SESSION['projovemurbano']['tprid']}',
	    				'P', 
	    				" . (($dados ['educadores'] ['P'] ['eduqtd']) ? "'" . $dados ['educadores'] ['P'] ['eduqtd'] . "'" : "NULL") . ", 
	    				" . (($dados ['educadores'] ['P'] ['eduefetivo']) ? "'" . $dados ['educadores'] ['P'] ['eduefetivo'] . "'" : "NULL") . ", 
	    				" . (($dados ['educadores'] ['P'] ['eduefetivo30hr']) ? "'" . $dados ['educadores'] ['P'] ['eduefetivo30hr'] . "'" : "NULL") . ", 
	            		" . (($dados ['educadores'] ['P'] ['eduqtdrecursoproprio']) ? "'" . $dados ['educadores'] ['P'] ['eduqtdrecursoproprio'] . "'" : "NULL") . ", 
	            		'A');";
		
		$db->executar ( $sql );
		
		$db->commit ();
	}
	
	$educadores_M = $db->pegaLinha ( "SELECT eduid, creid, ccmrid FROM projovemurbano.educadores WHERE proid='" . $_SESSION ['projovemurbano'] ['proid'] . "' AND tprid = '{$_SESSION['projovemurbano']['tprid']}' AND edustatus='A' AND edutipo='M'" );
	
	if ($educadores_M) {
		
		$sql = "UPDATE projovemurbano.contratadorecurso
   				SET creqtd=" . (($dados ['contratadorecurso'] ['EM'] ['creqtd']) ? "'" . $dados ['contratadorecurso'] ['EM'] ['creqtd'] . "'" : "NULL") . ", 
   					crevlrbrutorem=" . (($dados ['contratadorecurso'] ['EM'] ['crevlrbrutorem']) ? "'" . str_replace ( array (
				".",
				"," 
		), array (
				"",
				"." 
		), $dados ['contratadorecurso'] ['EM'] ['crevlrbrutorem'] ) . "'" : "NULL") . ", 
   					creqtdmeses=" . (($dados ['contratadorecurso'] ['EM'] ['creqtdmeses']) ? "'" . $dados ['contratadorecurso'] ['EM'] ['creqtdmeses'] . "'" : "NULL") . ", 
   					creencargos=" . (($dados ['contratadorecurso'] ['EM'] ['creencargos']) ? "'" . $dados ['contratadorecurso'] ['EM'] ['creencargos'] . "'" : "NULL") . ", 
			        crevlrtotal=" . (($dados ['contratadorecurso'] ['EM'] ['crevlrtotal']) ? "'" . str_replace ( array (
				".",
				"," 
		), array (
				"",
				"." 
		), $dados ['contratadorecurso'] ['EM'] ['crevlrtotal'] ) . "'" : "NULL") . " 
				WHERE creid='" . $educadores_M ['creid'] . "';";
		
		$db->executar ( $sql );
		
		$sql = "UPDATE projovemurbano.educadores
				   SET eduqtd=" . (($dados ['educadores'] ['M'] ['eduqtd']) ? "'" . $dados ['educadores'] ['M'] ['eduqtd'] . "'" : "NULL") . ", 
				   	   eduefetivo=" . (($dados ['educadores'] ['M'] ['eduefetivo']) ? "'" . $dados ['educadores'] ['M'] ['eduefetivo'] . "'" : "NULL") . ", 
				       eduefetivo30hr=" . (($dados ['educadores'] ['M'] ['eduefetivo30hr']) ? "'" . $dados ['educadores'] ['M'] ['eduefetivo30hr'] . "'" : "NULL") . ", 
				       eduqtdrecursoproprio=" . (($dados ['educadores'] ['M'] ['eduqtdrecursoproprio']) ? "'" . $dados ['educadores'] ['M'] ['eduqtdrecursoproprio'] . "'" : "NULL") . "
				 WHERE eduid='" . $educadores_M ['eduid'] . "';";
		
		$db->executar ( $sql );
		
		$db->commit ();
	} else {
		
		$sql = "INSERT INTO projovemurbano.contratadorecurso(
            creqtd, crevlrbrutorem, creqtdmeses, creencargos, crevlrtotal, 
            crestatus)
    		VALUES (" . (($dados ['contratadorecurso'] ['EM'] ['creqtd']) ? "'" . $dados ['contratadorecurso'] ['EM'] ['creqtd'] . "'" : "NULL") . ", 
    				" . (($dados ['contratadorecurso'] ['EM'] ['crevlrbrutorem']) ? "'" . str_replace ( array (
				".",
				"," 
		), array (
				"",
				"." 
		), $dados ['contratadorecurso'] ['EM'] ['crevlrbrutorem'] ) . "'" : "NULL") . ", 
    				" . (($dados ['contratadorecurso'] ['EM'] ['creqtdmeses']) ? "'" . $dados ['contratadorecurso'] ['EM'] ['creqtdmeses'] . "'" : "NULL") . ", 
    				" . (($dados ['contratadorecurso'] ['EM'] ['creencargos']) ? "'" . $dados ['contratadorecurso'] ['EM'] ['creencargos'] . "'" : "NULL") . ", 
    				" . (($dados ['contratadorecurso'] ['EM'] ['crevlrtotal']) ? "'" . str_replace ( array (
				".",
				"," 
		), array (
				"",
				"." 
		), $dados ['contratadorecurso'] ['EM'] ['crevlrtotal'] ) . "'" : "NULL") . ", 'A') RETURNING creid;";
		
		$creid = $db->pegaUm ( $sql );
		
		$sql = "INSERT INTO projovemurbano.educadores(
	            creid, ccmrid, proid, tprid, edutipo, eduqtd, eduefetivo, eduefetivo30hr, 
	            eduqtdrecursoproprio, edustatus)
	    		VALUES ('" . $creid . "', 
	    				NULL, 
	    				'" . $_SESSION ['projovemurbano'] ['proid'] . "', 
	    				'{$_SESSION['projovemurbano']['tprid']}',
	    				'M', 
	    				" . (($dados ['educadores'] ['M'] ['eduqtd']) ? "'" . $dados ['educadores'] ['M'] ['eduqtd'] . "'" : "NULL") . ", 
	    				" . (($dados ['educadores'] ['M'] ['eduefetivo']) ? "'" . $dados ['educadores'] ['M'] ['eduefetivo'] . "'" : "NULL") . ", 
	    				" . (($dados ['educadores'] ['M'] ['eduefetivo30hr']) ? "'" . $dados ['educadores'] ['M'] ['eduefetivo30hr'] . "'" : "NULL") . ", 
	            		" . (($dados ['educadores'] ['M'] ['eduqtdrecursoproprio']) ? "'" . $dados ['educadores'] ['M'] ['eduqtdrecursoproprio'] . "'" : "NULL") . ", 
	            		'A');";
		
		$db->executar ( $sql );
		
		$db->commit ();
	}
	
	$educadores_T = $db->pegaLinha ( "SELECT eduid, creid, ccmrid FROM projovemurbano.educadores WHERE proid='" . $_SESSION ['projovemurbano'] ['proid'] . "' AND tprid = '{$_SESSION['projovemurbano']['tprid']}' AND edustatus='A' AND edutipo='T'" );
	
	if ($educadores_T) {
		
		$sql = "UPDATE projovemurbano.contratadorecurso
   				SET creqtd=" . (($dados ['contratadorecurso'] ['ET'] ['creqtd']) ? "'" . $dados ['contratadorecurso'] ['ET'] ['creqtd'] . "'" : "NULL") . ", 
   					crevlrbrutorem=" . (($dados ['contratadorecurso'] ['ET'] ['crevlrbrutorem']) ? "'" . str_replace ( array (
				".",
				"," 
		), array (
				"",
				"." 
		), $dados ['contratadorecurso'] ['ET'] ['crevlrbrutorem'] ) . "'" : "NULL") . ", 
   					creqtdmeses=" . (($dados ['contratadorecurso'] ['ET'] ['creqtdmeses']) ? "'" . $dados ['contratadorecurso'] ['ET'] ['creqtdmeses'] . "'" : "NULL") . ", 
   					creencargos=" . (($dados ['contratadorecurso'] ['ET'] ['creencargos']) ? "'" . $dados ['contratadorecurso'] ['ET'] ['creencargos'] . "'" : "NULL") . ", 
			        crevlrtotal=" . (($dados ['contratadorecurso'] ['ET'] ['crevlrtotal']) ? "'" . str_replace ( array (
				".",
				"," 
		), array (
				"",
				"." 
		), $dados ['contratadorecurso'] ['ET'] ['crevlrtotal'] ) . "'" : "NULL") . " 
				WHERE creid='" . $educadores_T ['creid'] . "';";
		
		$db->executar ( $sql );
		
		$sql = "UPDATE projovemurbano.educadores
				   SET eduqtd=" . (($dados ['educadores'] ['T'] ['eduqtd']) ? "'" . $dados ['educadores'] ['T'] ['eduqtd'] . "'" : "NULL") . ", 
				   	   eduefetivo=" . (($dados ['educadores'] ['T'] ['eduefetivo']) ? "'" . $dados ['educadores'] ['T'] ['eduefetivo'] . "'" : "NULL") . ", 
				       eduefetivo30hr=" . (($dados ['educadores'] ['T'] ['eduefetivo30hr']) ? "'" . $dados ['educadores'] ['T'] ['eduefetivo30hr'] . "'" : "NULL") . ", 
				       eduqtdrecursoproprio=" . (($dados ['educadores'] ['T'] ['eduqtdrecursoproprio']) ? "'" . $dados ['educadores'] ['T'] ['eduqtdrecursoproprio'] . "'" : "NULL") . "
				 WHERE eduid='" . $educadores_T ['eduid'] . "';";
		
		$db->executar ( $sql );
		
		$db->commit ();
	} else {
		
		$sql = "INSERT INTO projovemurbano.contratadorecurso(
            creqtd, crevlrbrutorem, creqtdmeses, creencargos, crevlrtotal, 
            crestatus)
    		VALUES (" . (($dados ['contratadorecurso'] ['ET'] ['creqtd']) ? "'" . $dados ['contratadorecurso'] ['ET'] ['creqtd'] . "'" : "NULL") . ", 
    				" . (($dados ['contratadorecurso'] ['ET'] ['crevlrbrutorem']) ? "'" . str_replace ( array (
				".",
				"," 
		), array (
				"",
				"." 
		), $dados ['contratadorecurso'] ['ET'] ['crevlrbrutorem'] ) . "'" : "NULL") . ", 
    				" . (($dados ['contratadorecurso'] ['ET'] ['creqtdmeses']) ? "'" . $dados ['contratadorecurso'] ['ET'] ['creqtdmeses'] . "'" : "NULL") . ", 
    				" . (($dados ['contratadorecurso'] ['ET'] ['creencargos']) ? "'" . $dados ['contratadorecurso'] ['ET'] ['creencargos'] . "'" : "NULL") . ", 
    				" . (($dados ['contratadorecurso'] ['ET'] ['crevlrtotal']) ? "'" . str_replace ( array (
				".",
				"," 
		), array (
				"",
				"." 
		), $dados ['contratadorecurso'] ['ET'] ['crevlrtotal'] ) . "'" : "NULL") . ", 'A') RETURNING creid;";
		
		$creid = $db->pegaUm ( $sql );
		
		$sql = "INSERT INTO projovemurbano.educadores(
	            creid, ccmrid, proid, tprid, edutipo, eduqtd, eduefetivo, eduefetivo30hr, 
	            eduqtdrecursoproprio, edustatus)
	    		VALUES ('" . $creid . "', 
	    				NULL, 
	    				'" . $_SESSION ['projovemurbano'] ['proid'] . "', 
	    				'{$_SESSION['projovemurbano']['tprid']}',
	    				'T', 
	    				" . (($dados ['educadores'] ['T'] ['eduqtd']) ? "'" . $dados ['educadores'] ['T'] ['eduqtd'] . "'" : "NULL") . ", 
	    				" . (($dados ['educadores'] ['T'] ['eduefetivo']) ? "'" . $dados ['educadores'] ['T'] ['eduefetivo'] . "'" : "NULL") . ", 
	    				" . (($dados ['educadores'] ['T'] ['eduefetivo30hr']) ? "'" . $dados ['educadores'] ['T'] ['eduefetivo30hr'] . "'" : "NULL") . ", 
	            		" . (($dados ['educadores'] ['T'] ['eduqtdrecursoproprio']) ? "'" . $dados ['educadores'] ['T'] ['eduqtdrecursoproprio'] . "'" : "NULL") . ", 
	            		'A');";
		
		$db->executar ( $sql );
		
		$db->commit ();
	}
	
	$educadores_E = $db->pegaLinha ( "SELECT eduid, creid, ccmrid FROM projovemurbano.educadores WHERE proid='" . $_SESSION ['projovemurbano'] ['proid'] . "' AND tprid = '{$_SESSION['projovemurbano']['tprid']}' AND edustatus='A' AND edutipo='E'" );
	
	if ($educadores_E) {
		
		$sql = "UPDATE projovemurbano.educadores
				   SET eduqtd=" . (($dados ['educadores'] ['E'] ['eduqtd']) ? "'" . $dados ['educadores'] ['E'] ['eduqtd'] . "'" : "NULL") . ", 
				   	   eduefetivo=" . (($dados ['educadores'] ['E'] ['eduefetivo']) ? "'" . $dados ['educadores'] ['E'] ['eduefetivo'] . "'" : "NULL") . ", 
				       eduefetivo30hr=" . (($dados ['educadores'] ['E'] ['eduefetivo30hr']) ? "'" . $dados ['educadores'] ['E'] ['eduefetivo30hr'] . "'" : "NULL") . ", 
				       eduqtdrecursoproprio=" . (($dados ['educadores'] ['E'] ['eduqtdrecursoproprio']) ? "'" . $dados ['educadores'] ['E'] ['eduqtdrecursoproprio'] . "'" : "NULL") . "
				 WHERE eduid='" . $educadores_E ['eduid'] . "';";
		
		$db->executar ( $sql );
	} else {
		
		$sql = "INSERT INTO projovemurbano.educadores(
	            creid, ccmrid, proid, tprid, edutipo, eduqtd, eduefetivo, eduefetivo30hr, 
	            eduqtdrecursoproprio, edustatus)
	    		VALUES (NULL, 
	    				NULL, 
	    				'" . $_SESSION ['projovemurbano'] ['proid'] . "', 
	    				'{$_SESSION['projovemurbano']['tprid']}',
	    				'E', 
	    				" . (($dados ['educadores'] ['E'] ['eduqtd']) ? "'" . $dados ['educadores'] ['E'] ['eduqtd'] . "'" : "NULL") . ", 
	    				" . (($dados ['educadores'] ['E'] ['eduefetivo']) ? "'" . $dados ['educadores'] ['E'] ['eduefetivo'] . "'" : "NULL") . ", 
	    				" . (($dados ['educadores'] ['E'] ['eduefetivo30hr']) ? "'" . $dados ['educadores'] ['E'] ['eduefetivo30hr'] . "'" : "NULL") . ", 
	            		" . (($dados ['educadores'] ['E'] ['eduqtdrecursoproprio']) ? "'" . $dados ['educadores'] ['E'] ['eduqtdrecursoproprio'] . "'" : "NULL") . ", 
	            		'A');";
		
		$db->executar ( $sql );
	}
	
	$db->commit ();
	
	if ($_SESSION ['projovemurbano'] ['ppuid'] == '2') {
		$link = "projovemurbano.php?modulo=principal/planoImplementacao2013&acao=A&aba={$_REQUEST['aba']}&aba2=profissionais";
	} elseif ($_SESSION ['projovemurbano'] ['ppuid'] == '3') {
		$link = "projovemurbano.php?modulo=principal/planoImplementacao2014&acao=A&aba={$_REQUEST['aba']}&aba2=profissionais";
	} else {
		$link = "projovemurbano.php?modulo=principal/planoImplementacao&acao=A&aba=profissionais";
	}
	
	echo "<script>
			alert('Gravado com sucesso');
			window.location='$link';
		  </script>";
}
function excluirAnexo($dados) {
	global $db;
	include_once APPRAIZ . "includes/classes/fileSimec.class.inc";
	// ver($dados,d);
	if ($dados ['arquivo']) {
		$sql = "UPDATE projovemurbano.profissionais
                  SET angid = null,
                  tpdid = null,
                  pronuminstlegal = null,
                  prodatainstlegal = null
                WHERE proid = {$_SESSION['projovemurbano']['proid']}";
		$db->executar ( $sql );
		$sql = <<<DML
DELETE FROM projovemurbano.instrumentolegal
  WHERE tprid = {$_SESSION['projovemurbano']['tprid']}
    AND proid = {$_SESSION['projovemurbano']['proid']}
    AND angid = (SELECT angid
                   FROM projovemurbano.anexogeral
                   WHERE ppuid = {$_SESSION['projovemurbano']['ppuid']}
                     AND arqid = {$dados['arquivo']})
DML;
		$db->executar ( $sql );
		$sql = "DELETE FROM projovemurbano.anexogeral WHERE ppuid = {$_SESSION['projovemurbano']['ppuid']} AND arqid = {$dados['arquivo']}";
		$db->executar ( $sql );
		$sql = "UPDATE public.arquivo SET arqstatus = 'I' WHERE arqid={$dados['arquivo']}";
		$db->executar ( $sql );
		$db->commit ();
		
		// -- Excluíndo arquivo do fs
		$file = new FilesSimec ();
		$file->excluiArquivoFisico ( $_POST ['arquivo'] );
		echo '<script type="text/javascript">alert("Arquivo excluído com sucesso!");</script>';
	} else {
		echo '<script type="text/javascript">alert("Nenhum arquivo foi informado para exclusão.");</script>';
	}
	if ($_SESSION ['projovemurbano'] ['ppuid'] == '2') {
		echo '<script type="text/javascript">
	              window.location.href="projovemurbano.php?modulo=principal/planoImplementacao2013&acao=A&aba=' . $_REQUEST ['aba'] . '&aba2=profissionais";
	    	  </script>';
		die ();
	} elseif ($_SESSION ['projovemurbano'] ['ppuid'] == '3') {
		echo '<script type="text/javascript">
	              window.location.href="projovemurbano.php?modulo=principal/planoImplementacao2014&acao=A&aba=' . $_REQUEST ['aba'] . '&aba2=profissionais";
	    	  </script>';
		die ();
	}
}
function montaMenuProfissionais() {
	$menu [] = array (
			"id" => 1,
			"descricao" => "Profissionais",
			"link" => "/projovemurbano/projovemurbano.php?modulo=principal/planoImplementacao&acao=A&aba=profissionais&aba2=profissionaisCadastro" 
	);
	$menu [] = array (
			"id" => 2,
			"descricao" => "Profissionais - Resumo",
			"link" => "/projovemurbano/projovemurbano.php?modulo=principal/planoImplementacao&acao=A&aba=profissionais&aba2=profissionaisResumo" 
	);
	$menu [] = array (
			"id" => 3,
			"descricao" => "Resumo geral de educadores",
			"link" => "/projovemurbano/projovemurbano.php?modulo=principal/planoImplementacao&acao=A&aba=profissionais&aba2=resumoGeralEducadores" 
	);
	
	return $menu;
}
function montaMenuFormacaoEducadores() {
	global $db;
	$menu [] = array (
			"id" => 1,
			"descricao" => "Formação de Educadores",
			"link" => "/projovemurbano/projovemurbano.php?modulo=principal/planoImplementacao&acao=A&aba=formacaoEducadores&aba2=formacaoEducadoresCadastro" 
	);
	$menu [] = array (
			"id" => 2,
			"descricao" => "Formação de Educadores - Resumo",
			"link" => "/projovemurbano/projovemurbano.php?modulo=principal/planoImplementacao&acao=A&aba=formacaoEducadores&aba2=formacaoEducadoresResumo" 
	);
	
	return $menu;
}
function atualizarProjovemMunicipio($dados) {
	global $db;
	
	$sql = "UPDATE projovemurbano.municipio
			   SET muncod=" . (($dados ['muncod']) ? "'" . $dados ['muncod'] . "'" : "NULL") . "
			 WHERE munid='" . $dados ['munid'] . "';";
	$db->executar ( $sql );
	$db->commit ();
}
function gravarNucleo($dados) {
	global $db;
	// if( $_SESSION['projovemurbano']['ppuid'] == '2' ){
	// $sql = "SELECT * FROM projovemurbano.nucleoescola WHERE entid::character Varying IN ('{$_POST['entid']}', '{$_POST['entid2']}')";
	// $result = $db->carregar($sql);
	
	// if($result){
	// die("<script>
	// alert('Ja existe nucleo cadastrado com um desses codigos INEP.');
	// window.location='projovemurbano.php?modulo=principal/planoImplementacao&acao=A&requisicao=gerenciarNucleos&munid=".$dados['munid']."'
	// </script>");
	// }
	// }
// 	ver($dados,d);
	if ($dados ['nucid'] != '') {
		$sql = "UPDATE projovemurbano.nucleo
				   	SET nucqtdestudantes= 0
				    WHERE nucid = {$dados['nucid']};";
		$db->executar ( $sql );
		// $db->commit ();
	}
	
	// aplicando regras
	if ($_SESSION ['projovemurbano'] ['muncod']) {
		
		if ($_SESSION ['projovemurbano'] ['ppuid'] != '1') {
			$sql = "SELECT
			mtpvalor as valor,
			mtp.tpmid as tipo
			FROM
			projovemurbano.metasdoprograma mtp
			INNER JOIN projovemurbano.tipometadoprograma tpr ON tpr.tpmid = mtp.tpmid
			WHERE
			pjuid = {$_SESSION['projovemurbano']['pjuid']}
			AND tprid = {$_SESSION['projovemurbano']['tprid']}
			ORDER BY
						tipo DESC ";
			$meta = $db->pegaUm ( $sql );
		} else {
			$sugestaoampliacao = $db->pegaLinha ( "SELECT suaverdade, suametaajustada FROM projovemurbano.sugestaoampliacao WHERE pjuid='" . $_SESSION ['projovemurbano'] ['pjuid'] . "'" );
			$meta = $db->pegaUm ( "SELECT cmemeta FROM projovemurbano.cargameta WHERE cmecodibge='" . $_SESSION ['projovemurbano'] ['muncod'] . "' AND ppuid='" . $_SESSION ['projovemurbano'] ['ppuid'] . "'" );
			if ($sugestaoampliacao ['suaverdade'] == "t") {
				if ($sugestaoampliacao ['suametaajustada'])
					$meta = $sugestaoampliacao ['suametaajustada'];
			}
		}
	}
	if ($_SESSION ['projovemurbano'] ['estuf']) {
		$sugestaoampliacao = $db->pegaLinha ( "SELECT suaverdade, suametaajustada FROM projovemurbano.sugestaoampliacao WHERE pjuid='" . $_SESSION ['projovemurbano'] ['pjuid'] . "'" );
		if ($_SESSION ['projovemurbano'] ['ppuid'] != '1') {
			$sql = "SELECT 
						mtpvalor as valor,
						mtp.tpmid as tipo
					FROM
						projovemurbano.metasdoprograma mtp
					INNER JOIN projovemurbano.tipometadoprograma tpr ON tpr.tpmid = mtp.tpmid
					WHERE
						pjuid = {$_SESSION['projovemurbano']['pjuid']}
						AND tprid = {$_SESSION['projovemurbano']['tprid']}
					ORDER BY
						tipo DESC ";
			$meta = $db->pegaUm ( $sql );
		} else {
			$meta = $db->pegaUm ( "SELECT cmemeta FROM projovemurbano.cargameta c INNER JOIN territorios.estado e ON e.estcod::numeric=c.cmecodibge WHERE c.cmetipo='E' AND e.estuf='" . $_SESSION ['projovemurbano'] ['estuf'] . "'AND ppuid='" . $_SESSION ['projovemurbano'] ['ppuid'] . "'" );
			if ($sugestaoampliacao ['suaverdade'] == "t") {
				if ($sugestaoampliacao ['suametaajustada'])
					$meta = $sugestaoampliacao ['suametaajustada'];
			}
		}
	}
	
	$pmupossuipolo = $db->pegaUm ( "SELECT pmupossuipolo
                                        FROM projovemurbano.polomunicipio 
                                        WHERE pjuid='" . $_SESSION ['projovemurbano'] ['pjuid'] . "'
                                          AND tprid = {$_SESSION['projovemurbano']['tprid']}
                                          AND pmustatus='A'" );
	if (2 != $_SESSION ['projovemurbano'] ['tprid']) { // -- Limite de 200 alunos não é válido para unidades prisionais
	                                                   // ver($meta,$dados,d);
		if ($meta == 200) {
			if ($pmupossuipolo == "t") {
				$numeronuc = pegarNumeroNucleos ( true );
			} else {
				$numeronuc = pegarNumeroNucleos ( false );
			}
			// ver($numeronuc>0 , $dados['nucqtdestudantes'],d);
			if (($numeronuc > 0 && ! $dados ['nucid']) || ($dados ['nucqtdestudantes'] != 200)) {
				die ( "<script>
                                            alert('Meta igual a 200, somente 1(um) núcleo pode ser cadastrado com 200 alunos.');
                                            window.location='projovemurbano.php?modulo=principal/planoImplementacao&acao=A&requisicao=gerenciarNucleos&munid=" . $dados ['munid'] . "'
                                     </script>" );
			}
		}
		
		if ($meta > 200) {
			if ($pmupossuipolo == "t") {
				$estudantes = pegarNumeroEstudantes ( true );
			} else {
				$estudantes = pegarNumeroEstudantes ( false );
			}
			
			if (((($estudantes + $dados ['nucqtdestudantes']) > $meta) && ! $dados ['nucid']) || $dados ['nucqtdestudantes'] > 200 || $dados ['nucqtdestudantes'] < 150) {
				die ( "<script>
                                            alert('Se a meta for maior que 200, poderá incluir núcleos de 150 a 200, limitando, conforme o número da meta.');
                                            window.location='projovemurbano.php?modulo=principal/planoImplementacao&acao=A&requisicao=gerenciarNucleos&munid=" . $dados ['munid'] . "'
                                     </script>" );
			}
		}
	} else {
		$estudantes = pegarNumeroEstudantes ( 't' == $pmupossuipolo );
		$redir = "window.location='projovemurbano.php?modulo=principal/planoImplementacao&acao=A&requisicao=gerenciarNucleos&munid=" . $dados ['munid'] . "'";
		// ver($estudantes + $dados ['nucqtdestudantes'] > $meta,d);
		if ($estudantes + $dados ['nucqtdestudantes'] > $meta) {
			die ( "<script>alert('A quantidade de estudantes informada ultrapassa a meta deste núcleo.'); {$redir}</script>" );
		}
		if (($dados ['nucqtdestudantes'] > 150) || ($dados ['nucqtdestudantes'] < 60)) {
			die ( "<script>alert('A quantidade de estudantes cadastrados por turma dever maior que 60 e menor que 150.'); {$redir}</script>" );
		}
	}
	// ver(d);
	// fim regras
	if ($dados ['nucid'] != '') {
	$sqldadosturmasantigos = "SELECT DISTINCT nueqtdturma as qtd, nuetipo FROM projovemurbano.nucleoescola WHERE nucid = {$dados['nucid']} ORDER BY nuetipo desc";
	$dadosturmasantigos = $db->carregar($sqldadosturmasantigos);
		$sqlnueid = "SELECT
						nueid
					FROM
						 projovemurbano.nucleoescola 
					WHERE
						 nucid = {$dados['nucid']}
					ORDER BY
						nueid";
		
		$nueid = $db->carregarColuna( $sqlnueid );
		
		$sql = "UPDATE projovemurbano.nucleo
				   	SET nucqtdestudantes='" . $dados ['nucqtdestudantes'] . "'
				    WHERE nucid = {$dados['nucid']};";
		$db->executar ( $sql );
		$sql = '';
		
		if ($nueid ['0']) {
			$sqlturma = "SELECT DISTINCT
							turid 
						FROM projovemurbano.turma tur
					 	INNER JOIN projovemurbano.nucleoescola nue ON nue.entid = tur.entid
						WHERE 
							nueid = {$nueid ['0']}
						AND turstatus = 'A'";
			
// 			$sql.= "UPDATE projovemurbano.turma
// 						SET entid='" . $dados ['entid'] . "'
// 						WHERE turid in($sqlturma);";
			
			$sql.= "UPDATE projovemurbano.nucleoescola
						SET entid='" . $dados ['entid'] . "', nueqtdturma='" . $dados ['nueqtdturma'] . "'
						WHERE nueid = {$nueid['0']};";
			$db->executar ( $sql );
		}
		// ver($nueid ['1'],$dados ['entid2'],$dados['nueqtdturma2'],d);
		if ($nueid ['1'] && $dados ['entid2'] && ($dados ['nueqtdturma2'] != '')) {
			$sqlturma2 = "SELECT DISTINCT
							turid
						FROM projovemurbano.turma tur
						INNER JOIN projovemurbano.nucleoescola nue ON nue.entid = tur.entid
						WHERE
							nueid = {$nueid ['1']}";
			
// 			$sql.= "UPDATE projovemurbano.turma
// 						SET entid={$dados['entid2']}
// 					WHERE turid in ($sqlturma2);";
			
		
			$sql.= "UPDATE projovemurbano.nucleoescola
						SET entid={$dados['entid2']}, nueqtdturma={$dados['nueqtdturma2']}
						WHERE nueid = {$nueid['1']};";
			$db->executar ( $sql );
			
		} elseif ($dados ['entid2'] && $dados ['entcodent2'] && ! $nueid ['1']) {
			
			$sql = "INSERT INTO projovemurbano.nucleoescola(
		            	nucid, entid, nueqtdturma, nuetipo, nuestatus)
		    			VALUES ({$dados['nucid']}, '" . $dados ['entid2'] . "', {$dados['nueqtdturma2']}, 'A', 'A');";
			
			$db->executar ( $sql );
		} elseif (($nueid ['1'] && ! $dados ['entid2'] && ($dados ['nueqtdturma2'] == '')) || ($nueid ['1'] && $dados ['entid2'] && ($dados ['nueqtdturma2'] == ''))) {
			
			$sql = "DELETE FROM projovemurbano.nucleoescola
	 				WHERE nueid = {$nueid['1']};";
			$db->executar ( $sql );
		}
		$sqltemturma = "SELECT DISTINCT true  FROM projovemurbano.turma WHERE nucid = {$dados['nucid']} ";
		$temturma = $db->pegaUm($sqltemturma);
		$sqldadosturmas = "SELECT DISTINCT entid as id, nueqtdturma as qtd, nuetipo FROM projovemurbano.nucleoescola WHERE nucid = {$dados['nucid']} ORDER BY nuetipo desc";
		$dadosturmas = $db->carregar($sqldadosturmas);
		if($temturma){
			if(is_array($dadosturmas)){
				$sql = '';
				$y = 1;
				foreach($dadosturmas as $dadosturma){
					if($dadosturma['nuetipo']=='S'){
							$valor = $dadosturma['qtd'];
					}else{
						if($dadosturmasantigos['1']['qtd']>$dadosturmas['qtd']){
							$valor = $dadosturmasantigos['1']['qtd'];
						}else{
							$dadosturmas['qtd'];
						}
					}
					for($x=1;$x<=$valor;$x++){
						if(($dadosturmasantigos['0']['qtd']+$dadosturmasantigos['1']['qtd'])>=$y){	
							$sqlturma1 = "SELECT turid FROM projovemurbano.turma WHERE nucid = {$dados['nucid']} AND turdesc = 'Turma $y'";
							$turma = $db->pegaUm($sqlturma1);
							
							$sql .= "UPDATE projovemurbano.turma
							SET 
								entid = {$dadosturma['id']},
								tursedeanexo='{$dadosturma['nuetipo']}'
							WHERE turid = {$turma} AND turdesc = 'Turma $y';";
							$y++;
						}else{
								
							$sql2 .= "INSERT INTO projovemurbano.turma(
								            nucid, turdesc, turstatus, tursedeanexo, entid)
								    VALUES ({$dados['nucid']},'Turma $y','A', '{$dadosturma['nuetipo']}','{$dadosturma['id']}');
                		;";
									$y++;
						}
					}
				}
			}
			if($sql!=''){
				$db->executar($sql);
			}
			if($sql2!=''){
				$db->executar($sql2);
			}
		}
		$db->commit();
	} else {
		$db->executar ( "DELETE FROM projovemurbano.educadores WHERE proid IN(SELECT proid FROM projovemurbano.profissionais 
						WHERE pjuid='" . $_SESSION ['projovemurbano'] ['pjuid'] . "' AND tprid = {$_SESSION['projovemurbano']['tprid']} )" );
		if ($_SESSION['projovemurbano']['ppuid'] != 1) {
			$sql = "INSERT INTO projovemurbano.nucleo(
	                    entid, munid, nucqtdestudantes, nucstatus,ppuid,tprid)
	                    VALUES (NULL, '" . $dados ['munid'] . "', '" . $dados ['nucqtdestudantes'] . "', 'A',{$_SESSION['projovemurbano']['ppuid']} ,{$_SESSION['projovemurbano']['tprid']})RETURNING nucid;";
		} else {
			$sql = "INSERT INTO projovemurbano.nucleo(
	                    entid, munid, nucqtdestudantes, nucstatus)
	                    VALUES (NULL, '" . $dados ['munid'] . "', '" . $dados ['nucqtdestudantes'] . "', 'A')RETURNING nucid;";
		}
		// ver($sql);
		$nucid = $db->pegaUm ( $sql );
		
		if ($dados ['entid']) {
			
			$sql = "INSERT INTO projovemurbano.nucleoescola(
	            	nucid, entid, nueqtdturma, nuetipo, nuestatus)
	    			VALUES ('" . $nucid . "', '" . $dados ['entid'] . "', '" . $dados ['nueqtdturma'] . "', 'S', 'A');";
			
			$db->executar ( $sql );
		}
		
		if ($dados ['entid2'] && $dados ['entcodent2']) {
			
			$sql = "INSERT INTO projovemurbano.nucleoescola(
	            	nucid, entid, nueqtdturma, nuetipo, nuestatus)
	    			VALUES ('" . $nucid . "', '" . $dados ['entid2'] . "', '" . $dados ['nueqtdturma2'] . "', 'A', 'A');";
			
			$db->executar ( $sql );
		}
	}
	// ver($sql,d);
	$db->commit ();
	if ($_SESSION ['projovemurbano'] ['ppuid'] == '2') {
		echo "<script>
				alert('Núcleo gravado com sucesso, as abas de profisionais e formação foram modificadas');
				window.opener.carregarNucleos('" . $dados ['munid'] . "');
				window.close();
			  </script>";
	} elseif ($_SESSION ['projovemurbano'] ['ppuid'] == '3') {
		echo "<script>
				alert('Núcleo gravado com sucesso.');
				window.opener.carregarNucleos('" . $dados ['munid'] . "');
				window.close();
			  </script>";
	} else {
		echo "<script>
				alert('Núcleo gravado com sucesso, as abas de profisionais e formação foram modificadas');
				window.opener.carregarNucleos('" . $dados ['munid'] . "');
				window.close();
			  </script>";
	}
}

function gerenciarNucleos($dados) {
	global $db;
	if ($dados ['nucid']) {
		$sql = "SELECT DISTINCT
					nuc.nucid,
					nes.nueid,
					nes.entid,
					nueqtdturma,
					nucqtdestudantes,
					entcodent
				FROM 
					projovemurbano.nucleo nuc
				LEFT JOIN projovemurbano.nucleoescola nes ON nes.nucid = nuc.nucid AND nes.nuestatus = 'A'
				LEFT JOIN projovemurbano.turma tur ON tur.entid = nes.entid 
				LEFT JOIN entidade.entidade ent ON ent.entid = nes.entid
				WHERE 
					munid={$dados['munid']}
				AND nuc.nucid = {$dados['nucid']}
				AND nuc.nucstatus='A'
				ORDER BY
					nes.nueid";
		
		$dadosescolas = $db->carregar ( $sql );
		
		$entcodent = $dadosescolas [0] ['entcodent'];
		$entcodent2 = $dadosescolas [1] ['entcodent'];
		$nucqtdestudantes = $dadosescolas [0] ['nucqtdestudantes'];
		$nueqtdturma = $dadosescolas [0] ['nueqtdturma'];
		$nueqtdturma2 = $dadosescolas [1] ['nueqtdturma'];
		$nueid = $dadosescolas [0] ['nueid'];
		$nueid2 = $dadosescolas [1] ['nueid'];
// 		ver($dadosescolas,$dados);
	}
	if ($_REQUEST ['mnuid']) {
		$munid = $_REQUEST ['mnuid'];
	} else {
		$munid = $dados ['munid'];
	}
	?>
	


<html>
<head>
<script language="JavaScript" src="../includes/funcoes.js"></script>
<script language="javascript" type="text/javascript"
	src="../includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-1.4.2.min.js"></script>
<link rel="stylesheet" type="text/css" href="../includes/Estilo.css" />
<link rel="stylesheet" type="text/css" href="../includes/listagem.css" />
<script type="text/javascript"
	src="../includes/ModalDialogBox/modal-message.js"></script>
<script type="text/javascript"
	src="../includes/ModalDialogBox/ajax-dynamic-content.js"></script>
<script type="text/javascript" src="../includes/ModalDialogBox/ajax.js"></script>
<link rel="stylesheet" href="/includes/ModalDialogBox/modal-message.css"
	type="text/css" media="screen" />

</head>
<body>
	<script>
	jQuery(document).ready(function() {
		if( '<?=$dadosescolas[0]['nucid'] ?>' != '' ){
			jQuery('#entcodent').blur();
		}
		if( '<?=$dadosescolas[1]['nucid'] ?>' != '' ){
			jQuery('#entcodent2').blur();
		}
	});
	function gravarNucleo() {
		var qtd_1 = <?=$nueqtdturma?$nueqtdturma:0?>;
		var qtd_2 = <?=$nueqtdturma2?$nueqtdturma2:0?>;
		var qtd_total = qtd_1+qtd_2;

		var qtdescrito1 ='' ;
		if(isNaN(parseInt(jQuery('#nueqtdturma').val()))){
			qtdescrito1 = 0;
		}else{
			qtdescrito1 = parseInt(jQuery('#nueqtdturma').val());
		}
		
		var qtdescrito2 = '';
		if(isNaN(parseInt(jQuery('#nueqtdturma2').val()))){
			qtdescrito2 =0;
		}else{
			qtdescrito2 =parseInt(jQuery('#nueqtdturma2').val());
		}
		var qtd_tela_total = qtdescrito1 + qtdescrito2;
		
		if(qtd_tela_total!=0){	
			if(qtd_total > qtd_tela_total){
				alert('Não é permitido diminuir  o numero total de turmas.');
				return false;
			}	
		}
		
        var qtdEstudates = jQuery('#nucqtdestudantes').val();
		if ('' == qtdEstudates) {
			alert('Preencha a quantidades de estudantes');
			return false;
		} 
		/*
		else if ((parseInt(qtdEstudates) > 60) || (parseInt(qtdEstudates) < 40)) {
                    alert('A quantidade de alunos por turma deve ser entre 40 e 60 alunos.');
                    jQuery('#nucqtdestudantes').focus();
                    return false;
                }*/

		if(jQuery('#entcodent').val()=='') {
			alert('Preencha o código INEP');
			return false;
		}
		
		if(jQuery('#nueqtdturma').val()=='') {
			alert('Preencha a quantidade de turma');
			return false;
		}

		/*
		if(jQuery('#nueqtdturma2').val()=='') {
			alert('Preencha a quantidade de turma');
			return false;
		}
		*/

		var turmas1 = parseInt(jQuery('#nueqtdturma').val());
		var turmas2 = parseInt(jQuery('#nueqtdturma2').val());
                <?php
	if (2 == $_SESSION ['projovemurbano'] ['tprid']) : // -- Unidade prisional		?>
                    if ((turmas1 + turmas2) > 4) {
                            alert('A quantidade total de turmas não deve ser maior que 4.');
                            return false;
                    } else if ((turmas1 + turmas2) < 3) {
                        alert('A quantidade total de turmas não deve ser menor que 3.');
                        return false;
                    }
                <?php else: ?>
                    if( (turmas1+turmas2)>5 ) {
                            alert('A quantidade total de turmas não deve ser maior que 5.');
                            return false;
                    }
                <?php endif; ?>

		if(jQuery('#entcodent2').val()!='') {
			if(jQuery('#nueqtdturma2').val()=='') {
				alert('Preencha a quantidade de turma (Escola Anexo)');
				return false;
			}
		}


		
		$('#form').submit();
	}
	
	function buscarEscolaPorINEP(codinep, escolatipo) {
		if(codinep=='') {
			alert('Digite um código INEP');
			return false;
		}
		var outroTipo = '';
		if( escolatipo == '' ){ outrotipo = '2' }else{ outrotipo = '' }
		jQuery.ajax({
	   		type: "POST",
	   		url: "projovemurbano.php?modulo=principal/planoImplementacao&acao=A",
	   		data: "requisicao=buscarEscolaPorINEP&codinep="+codinep,
	   		async: false,
	   		success: function(msg){
   				var myObject = eval('(' + msg + ')');
   				if(myObject.entid) {
   					if( myObject.entid == $('#entid'+outrotipo).val()  ){
   						alert('Escola repetida. Escolha outra.');
   						$('#entcodent'+escolatipo).val('');
   	   					$('#entid'+escolatipo).val('');
   	   					$('#td_entnome'+escolatipo).html('');
   	   					$('#td_tpcdesc'+escolatipo).html('');
   	   					$('#td_tpldesc'+escolatipo).html('');
   	   					
   	   					$('#td_endlog'+escolatipo).html('');
   	   					$('#td_endnum'+escolatipo).html('');
   	   					$('#td_endcom'+escolatipo).html('');
   	   					$('#td_endbai'+escolatipo).html('');
   	   					$('#td_endcep'+escolatipo).html('');
   	   					$('#td_mundescricao'+escolatipo).html('');
   	   					$('#td_estuf'+escolatipo).html('');
   	   					$('#td_enttelefone'+escolatipo).html('');
   						return false;
	   				}
   					$('#entid'+escolatipo).val(myObject.entid);
   					$('#td_entnome'+escolatipo).html(myObject.entnome);
   					$('#td_tpcdesc'+escolatipo).html(myObject.tpcdesc);
   					$('#td_tpldesc'+escolatipo).html(myObject.tpldesc);
   					
   					$('#td_endlog'+escolatipo).html(myObject['endereco'].endlog);
   					$('#td_endnum'+escolatipo).html(myObject['endereco'].endnum);
   					$('#td_endcom'+escolatipo).html(myObject['endereco'].endcom);
   					$('#td_endbai'+escolatipo).html(myObject['endereco'].endbai);
   					$('#td_endcep'+escolatipo).html(myObject['endereco'].endcep);
   					$('#td_mundescricao'+escolatipo).html(myObject['endereco'].mundescricao);
   					$('#td_estuf'+escolatipo).html(myObject['endereco'].estuf);
   					$('#td_enttelefone'+escolatipo).html('('+myObject.entnumdddcomercial+')'+myObject.entnumcomercial);
   				} else {
   					alert('Escola não identificada');
   					jQuery('#entcodent'+escolatipo).val('');
   				}
	   		}
	 	});
	}
	
	function validarQuantidadeTurma() {
		var total = 0;
		if(jQuery('#nueqtdturma').val()!='') {
			total += parseInt(jQuery('#nueqtdturma').val());
		}
		if(jQuery('#nueqtdturma2').val()!='') {
			total += parseInt(jQuery('#nueqtdturma2').val());
		}
                <?php if (2 == $_SESSION['projovemurbano']['tprid']): // -- Unidade prisional ?>
                    if (total > 4) {
                            alert('A quantidade turmas não pode ser maior do que 4.');
                            jQuery('#nueqtdturma').val('');
                            jQuery('#nueqtdturma2').val('');
                            jQuery('#nueqtdturma').focus();
                    }
                <?php else: ?>
                    if(total > 5) {
                            alert('A quantidade turmas não pode ser maior do que 5.');
                            jQuery('#nueqtdturma').val('');
                            jQuery('#nueqtdturma2').val('');
                    }
                <?php endif; ?>
	}
	</script>
	<form id="form" name="form" method="POST">
		<input type="hidden" name="requisicao" value="gravarNucleo"> <input
			type="hidden" name="entid" id="entid" value=""> <input type="hidden"
			name="entid2" id="entid2" value=""> <input type="hidden" name="mnuid"
			id="mnuid" value="<?=$munid ?>">
		<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1"
			cellPadding="3" align="center">
			<tr>
				<td class="SubTituloDireita" width="30%">Orientações</td>
				<td><font color=blue>
                        <?php if (2 == $_SESSION['projovemurbano']['tprid']): // -- Unidade prisional ?>
			- O Núcleo da Unidade Prisional é composto de no mínimo 60 estudantes e no máximo 150.
                        <?php else: ?>
			- Se a meta for 200, inserir somente 1(UM) núcleo de 200 alunos.<br />
			- Se a meta for maior que 200, poderá incluir núcleos de 150 ou 200, limitando, conforme o número da meta.
                        <?php endif; ?>
			</font></td>
			</tr>
			<tr>
				<td class="SubTituloDireita" width="30%">Quantidades de estudantes</td>
				<td><? echo campo_texto('nucqtdestudantes', 'S', 'S', 'Quantidades de estudantes', 8, 7, "########", "", '', '', 0, 'id="nucqtdestudantes"', '', $nucqtdestudantes,'' ); ?></td>
			</tr>
			<tr>
				<td class="SubTituloCentro" colspan="2">Escola Sede</td>
			</tr>
			<tr>
				<td class="SubTituloDireita">Código INEP</td>
				<td><? echo campo_texto('entcodent', 'S', 'S', 'Código INEP', 11, 10, "##########", "", '', '', 0, 'id="entcodent"', '', $entcodent, 'buscarEscolaPorINEP(this.value,\'\');' ); ?>
                            <input type="button" name="buscar"
					value="Buscar"
					onclick="var today = new Date();
                                            displayMessage('projovemurbano.php?modulo=principal/planoImplementacao&acao=A&requisicao=buscarEscolas&nocache='+today);">
				</td>
			</tr>
			<tr>
				<td class="SubTituloDireita">Quantidade de turmas</td>
				<td><? echo campo_texto('nueqtdturma', 'S', 'S', 'Quantidade de turmas', 8, 7, "##########", "", '', '', 0, 'id="nueqtdturma"', '', $nueqtdturma, 'validarQuantidadeTurma();' ); ?></td>
			</tr>
			<tr>
				<td class="SubTituloDireita">Nome da escola</td>
				<td id="td_entnome"></td>
			</tr>
			<tr>
				<td class="SubTituloDireita">Tipo de orgão</td>
				<td id="td_tpcdesc"></td>
			</tr>
			<tr>
				<td class="SubTituloDireita">Localização</td>
				<td id="td_tpldesc"></td>
			</tr>
			<tr>
				<td class="SubTituloDireita">Endereço</td>
				<td id="td_endlog"></td>
			</tr>
			<tr>
				<td class="SubTituloDireita">Número</td>
				<td id="td_endnum"></td>
			</tr>
			<tr>
				<td class="SubTituloDireita">Complemento</td>
				<td id="td_endcom"></td>
			</tr>
			<tr>
				<td class="SubTituloDireita">Bairro</td>
				<td id="td_endbai"></td>
			</tr>
			<tr>
				<td class="SubTituloDireita">CEP</td>
				<td id="td_endcep"></td>
			</tr>
			<tr>
				<td class="SubTituloDireita">Município</td>
				<td id="td_mundescricao"></td>
			</tr>
			<tr>
				<td class="SubTituloDireita">UF</td>
				<td id="td_estuf"></td>
			</tr>
			<tr>
				<td class="SubTituloDireita">Telefone</td>
				<td id="td_enttelefone"></td>
			</tr>
			<tr>
				<td class="SubTituloCentro" colspan="2">Escola Anexo</td>
			</tr>
			<tr>
				<td class="SubTituloDireita">Código INEP</td>
				<td><? echo campo_texto('entcodent2', 'S', 'S', 'Código INEP', 11, 10, "##########", "", '', '', 0, 'id="entcodent2"', '', $entcodent2, "buscarEscolaPorINEP(this.value,'2');" ); ?>
                            <input type="button" name="buscar"
					value="Buscar"
					onclick="var today = new Date();
                                            displayMessage('projovemurbano.php?modulo=principal/planoImplementacao&acao=A&requisicao=buscarEscolas&escolatipo=2&nocache='+today);"></td>
			</tr>
			<tr>
				<td class="SubTituloDireita">Quantidade de turmas</td>
				<td><? echo campo_texto('nueqtdturma2', 'S', 'S', 'Quantidade de turmas', 8, 7, "##########", "", '', '', 0, 'id="nueqtdturma2"', '', $nueqtdturma2, 'validarQuantidadeTurma();' ); ?></td>
			</tr>
			<tr>
				<td class="SubTituloDireita">Nome da escola</td>
				<td id="td_entnome2"></td>
			</tr>
			<tr>
				<td class="SubTituloDireita">Tipo de orgão</td>
				<td id="td_tpcdesc2"></td>
			</tr>
			<tr>
				<td class="SubTituloDireita">Localização</td>
				<td id="td_tpldesc2"></td>
			</tr>
			<tr>
				<td class="SubTituloDireita">Endereço</td>
				<td id="td_endlog2"></td>
			</tr>
			<tr>
				<td class="SubTituloDireita">Número</td>
				<td id="td_endnum2"></td>
			</tr>
			<tr>
				<td class="SubTituloDireita">Complemento</td>
				<td id="td_endcom2"></td>
			</tr>
			<tr>
				<td class="SubTituloDireita">Bairro</td>
				<td id="td_endbai2"></td>
			</tr>
			<tr>
				<td class="SubTituloDireita">CEP</td>
				<td id="td_endcep2"></td>
			</tr>
			<tr>
				<td class="SubTituloDireita">Município</td>
				<td id="td_mundescricao2"></td>
			</tr>
			<tr>
				<td class="SubTituloDireita">UF</td>
				<td id="td_estuf2"></td>
			</tr>
			<tr>
				<td class="SubTituloDireita">Telefone</td>
				<td id="td_enttelefone2"></td>
			</tr>
			<tr>
				<td class="SubTituloCentro" colspan="2"><input type="button"
					name="salvar" value="Salvar" onclick="gravarNucleo();"></td>
			</tr>
		</table>
	</form>
	<script type="text/javascript">
	messageObj = new DHTML_modalMessage();	// We only create one object of this class
	messageObj.setShadowOffset(5);	// Large shadow
	
	function displayMessage(url) {
		messageObj.setSource(url);
		messageObj.setCssClassMessageBox(false);
		messageObj.setSize(450,350);
		messageObj.setShadowDivVisible(true);	// Enable shadow for these boxes
		messageObj.display();
	}
	
	function closeMessage() {
		messageObj.close();	
	}
	
	</script>

</body>
</html>
<?
}
function carregarNucleos($dados) {
	global $db;
	// ver($dados,d);
	?>
<script>
		function gerenciarNucleos(munid) {
			window.open('projovemurbano.php?modulo=principal/planoImplementacao2014&acao=A&requisicao=gerenciarNucleos&munid='+munid,'Núcleos','scrollbars=yes,height=400,width=600,status=no,toolbar=no,menubar=no,location=no');
		}
	</script>
<?
	$esdid = $db->pegaUm ( "SELECT esdid FROM projovemurbano.projovemurbano p 
				 		  INNER JOIN workflow.documento d ON d.docid = p.docid 
				 		  WHERE pjuid='" . $_SESSION ['projovemurbano'] ['pjuid'] . "'" );
	
	if ($esdid != ESD_EMELABORACAO && ! $db->testa_superuser ()) {
		$habilitado = "disabled";
	}
	
	$complement = ($_SESSION ['projovemurbano'] ['ppuid'] == 1) ? '' : 'AND tprid = ' . $_SESSION ['projovemurbano'] ['tprid'];
	if ($_SESSION ['projovemurbano'] ['ppuid'] == 3) {
		$deletar = "" . ((! $habilitado) ? "<img src=../imagens/excluir.gif style=cursor:pointer; title=\"Excluir\"
							onclick=\"excluirPoloMunicipio(\'projovemurbano.php?modulo=principal/indexPoloNucleo2014&acao=A&aba={$_REQUEST['aba']}&requisicao=excluirNucleo&nucid='||nuc.nucid||'&munid={$dados['mnuid']}\');\">" : "&nbsp;") . "";
		$editar = "" . ((! $habilitado) ? "<img src=../imagens/alterar.gif style=cursor:pointer; title=\"Editar\"
							onclick=\"editarPoloMunicipio({$dados['mnuid']}, '|| nuc.nucid ||');\">" : "&nbsp;") . "";
	} else {
		$deletar = "" . ((! $habilitado) ? "<img src=../imagens/excluir.gif style=cursor:pointer; title=\"Excluir\"
							onclick=\"excluirPoloMunicipio(\'projovemurbano.php?modulo=principal/planoImplementacao2013&acao=A&aba={$_REQUEST['aba']}&requisicao=excluirNucleo&nucid='||nuc.nucid||'&munid={$dados['mnuid']}\');\">" : "&nbsp;") . "";
	}
	$sql = "
        SELECT acao, nucleo, inepsede, '<a href=\"javascript:detalharEndereco(' || inepsede || ');\">' ||escolasede|| '</a>' as escolasede,
               --CASE WHEN enderecosede = '<center>-</center>' THEN enderecosede
                -- ELSE '<a href=\"javascript:detalharEndereco(' || inepsede || ');\">' || enderecosede || '</a>' END AS enderecosede,
               qtdturmasede, inepanexo, '<a href=\"javascript:detalharEndereco(' || inepanexo || ');\">' ||escolaanexo || '</a>' as escolaanexo,
              -- CASE WHEN enderecoanexo = '<center>-</center>' THEN enderecoanexo
                -- ELSE '<a href=\"javascript:detalharEndereco(' || inepanexo || ');\">' || enderecoanexo || '</a>' END AS enderecoanexo,
               qtdturmaanexo, nucqtdestudantes
          FROM (SELECT DISTINCT '<center>$deletar|| $editar</center>' as acao,
				'Nucleo '||nuc.nucid as nucleo, 
                                (SELECT tent.pk_cod_entidade
                                   FROM projovemurbano.nucleoescola nes
                                     INNER JOIN entidade.entidade ent USING(entid)
                                     INNER JOIN educacenso_2014.tab_entidade tent
                                       ON ent.entcodent::int = tent.pk_cod_entidade
                                   WHERE nes.nucid = nuc.nucid
                                     AND nes.nuetipo = 'S') AS inepsede,
				(SELECT entnome FROM entidade.entidade ent INNER JOIN projovemurbano.nucleoescola nes ON nes.entid = ent.entid WHERE nes.nucid=nuc.nucid AND nes.nuetipo='S') as escolasede,
                                COALESCE((SELECT tent.desc_endereco
                                            FROM projovemurbano.nucleoescola nes
                                              INNER JOIN entidade.entidade ent USING(entid)
                                              INNER JOIN educacenso_2014.tab_entidade tent
                                                ON ent.entcodent::int = tent.pk_cod_entidade
                                            WHERE nes.nucid = nuc.nucid
                                              AND nes.nuetipo = 'S'), '<center>-</center>') AS enderecosede,
				(SELECT nueqtdturma FROM entidade.entidade ent INNER JOIN projovemurbano.nucleoescola nes ON nes.entid = ent.entid WHERE nes.nucid=nuc.nucid AND nes.nuetipo='S') as qtdturmasede,  
                                (SELECT tent.pk_cod_entidade
                                   FROM projovemurbano.nucleoescola nes
                                     INNER JOIN entidade.entidade ent USING(entid)
                                     INNER JOIN educacenso_2014.tab_entidade tent
                                       ON ent.entcodent::int = tent.pk_cod_entidade
                                   WHERE nes.nucid = nuc.nucid
                                     AND nes.nuetipo = 'A' AND  nes.nuestatus='A') AS inepanexo,
				COALESCE((SELECT entnome FROM entidade.entidade ent INNER JOIN projovemurbano.nucleoescola nes ON nes.entid = ent.entid WHERE nes.nucid=nuc.nucid AND nes.nuetipo='A' AND  nes.nuestatus='A'),'<center>-</center>') as escolaanexo,
                                COALESCE((SELECT tent.desc_endereco
                                            FROM projovemurbano.nucleoescola nes
                                              INNER JOIN entidade.entidade ent USING(entid)
                                              INNER JOIN educacenso_2014.tab_entidade tent
                                                ON ent.entcodent::int = tent.pk_cod_entidade
                                            WHERE nes.nucid = nuc.nucid
                                              AND nes.nuetipo = 'A' AND  nes.nuestatus='A'), '<center>-</center>') AS enderecoanexo,
				COALESCE((SELECT nueqtdturma FROM entidade.entidade ent INNER JOIN projovemurbano.nucleoescola nes ON nes.entid = ent.entid WHERE nes.nucid=nuc.nucid AND nes.nuetipo='A' AND  nes.nuestatus='A'),null) as qtdturmaanexo,
				nuc.nucqtdestudantes 
			FROM 
				projovemurbano.nucleo nuc
			LEFT JOIN projovemurbano.nucleoescola nes ON nes.nucid = nuc.nucid 
			LEFT JOIN projovemurbano.turma tur ON tur.entid = nes.entid
			WHERE munid='" . $dados ['mnuid'] . "' AND nuc.nucstatus='A'
            {$complement}
			ORDER BY
				3) select_dados";
// 	ver($sql);
	$cabecalho = array (
			"&nbsp;",
			"Núcleo",
			'Cód. INEP Sede',
			"Escola Sede",
			"Turmas Sede",
			'Cód. INEP Anexo',
			"Escola Anexo",
			"Turmas Anexo",
			"Qtd. Estudantes" 
	);
	$db->monta_lista_simples ( $sql, $cabecalho, 50, 5, 'N', '100%', $par2 );
}
function inserirPoloMunicipio($dados) {
	global $db;
	
	if ($_SESSION ['projovemurbano'] ['ppuid'] == '2') {
		$sql = "INSERT INTO projovemurbano.polomunicipio(
	            pjuid, tprid, pmuqtdpolo, pmuqtdmunicipio, pmupossuipolo)
	    		VALUES ('" . $_SESSION ['projovemurbano'] ['pjuid'] . "', {$_SESSION['projovemurbano']['tprid']}," . (($dados ['pmuqtdpolo']) ? "'" . $dados ['pmuqtdpolo'] . "'" : "NULL") . ", " . ((is_null ( $dados ['pmuqtdmunicipio'] )) ? "NULL" : "'" . $dados ['pmuqtdmunicipio'] . "'") . ", " . (($dados ['pmupossuipolo']) ? $dados ['pmupossuipolo'] : 'NULL') . ");";
		
		$db->executar ( $sql );
		
		$db->commit ();
		
		header ( "Cache-Control: no-cache, must-revalidate" );
		header ( "location:projovemurbano.php?modulo=principal/planoImplementacao2013&acao=A&aba={$_GET['aba']}&aba2=poloNucleo" );
	} else {
		$sql = "INSERT INTO projovemurbano.polomunicipio(
	            pjuid, tprid, pmuqtdpolo, pmuqtdmunicipio, pmupossuipolo)
	    		VALUES ('" . $_SESSION ['projovemurbano'] ['pjuid'] . "', null, " . (($dados ['pmuqtdpolo']) ? "'" . $dados ['pmuqtdpolo'] . "'" : "NULL") . ", " . ((is_null ( $dados ['pmuqtdmunicipio'] )) ? "NULL" : "'" . $dados ['pmuqtdmunicipio'] . "'") . ", " . (($dados ['pmupossuipolo']) ? $dados ['pmupossuipolo'] : 'NULL') . ");";
		
		$db->executar ( $sql );
		
		$db->commit ();
		
		header ( "Cache-Control: no-cache, must-revalidate" );
		header ( "location:projovemurbano.php?modulo=principal/planoImplementacao&acao=A&aba=poloNucleo" );
	}
}
function inserirPoloMunicipio2014($dados) {
	global $db;
	
	if ($_SESSION ['projovemurbano'] ['ppuid'] == '3') {
		$sql = "INSERT INTO projovemurbano.polomunicipio(
	            pjuid, tprid, pmuqtdpolo, pmuqtdmunicipio, pmupossuipolo)
	    		VALUES ('" . $_SESSION ['projovemurbano'] ['pjuid'] . "', {$_SESSION['projovemurbano']['tprid']}," . (($dados ['pmuqtdpolo']) ? "'" . $dados ['pmuqtdpolo'] . "'" : "NULL") . ", " . ((is_null ( $dados ['pmuqtdmunicipio'] )) ? "NULL" : "'" . $dados ['pmuqtdmunicipio'] . "'") . ", " . (($dados ['pmupossuipolo']) ? $dados ['pmupossuipolo'] : 'NULL') . ");";
		
		$db->executar ( $sql );
		
		$db->commit ();
		
		header ( "Cache-Control: no-cache, must-revalidate" );
		header ( "location:projovemurbano.php?modulo=principal/indexPoloNucleo2014&acao=A&aba={$_GET['aba']}&aba2=poloNucleo" );
	}
}
function atualizarPoloMunicipio($dados) {
	global $db;
	
	if ($dados ['pmupossuipolo'] == "TRUE") {
		$db->executar ( "UPDATE projovemurbano.municipio SET munstatus='I' WHERE pmuid='" . $dados ['pmuid'] . "'" );
	}
	
	if ($dados ['pmupossuipolo'] == "FALSE") {
		
		$tprid = $_SESSION ['projovemurbano'] ['ppuid'] != '' ? " AND tprid = " . $_SESSION ['projovemurbano'] ['ppuid'] : "";
		
		$sql = "UPDATE projovemurbano.municipio SET munstatus='I' 
				WHERE munid IN(SELECT munid FROM projovemurbano.associamucipiopolo amp 
							   INNER JOIN projovemurbano.polo pol ON amp.polid = pol.polid 
							   INNER JOIN projovemurbano.polomunicipio pmu ON pmu.pmuid=pol.pmuid 
							   WHERE pmu.pjuid='" . $_SESSION ['projovemurbano'] ['pjuid'] . "' $tprid AND pol.polstatus='A')";
		
		$db->executar ( $sql );
		
		$sql = "UPDATE projovemurbano.polo SET polstatus='I' 
				WHERE polid IN(SELECT polid FROM projovemurbano.polo pol
							   INNER JOIN projovemurbano.polomunicipio pmu ON pmu.pmuid=pol.pmuid 
							   WHERE pmu.pjuid='" . $_SESSION ['projovemurbano'] ['pjuid'] . "' $tprid )";
		
		$db->executar ( $sql );
	}
	
	$sql = "UPDATE projovemurbano.polomunicipio
   			SET pmuqtdpolo=" . (($dados ['pmuqtdpolo']) ? "'" . $dados ['pmuqtdpolo'] . "'" : "NULL") . ", 
   				pmuqtdmunicipio=" . (($dados ['pmuqtdmunicipio']) ? "'" . $dados ['pmuqtdmunicipio'] . "'" : "NULL") . ",
   				pmupossuipolo=" . (($dados ['pmupossuipolo']) ? $dados ['pmupossuipolo'] : 'NULL') . "
 			WHERE pmuid='" . $dados ['pmuid'] . "';";
	
	$db->executar ( $sql );
	
	$db->commit ();
	
	header ( "Cache-Control: no-cache, must-revalidate" );
	if ($_SESSION ['projovemurbano'] ['ppuid'] == '2') {
		header ( "location:projovemurbano.php?modulo=principal/planoImplementacao2013&acao=A&aba={$_GET['aba']}&aba2=poloNucleo" );
	} elseif ($_SESSION ['projovemurbano'] ['ppuid'] == '3') {
		header ( "location:projovemurbano.php?modulo=principal/indexPoloNucleo2014&acao=A&aba={$_GET['aba']}&aba2=poloNucleo" );
	} else {
		header ( "location:projovemurbano.php?modulo=principal/planoImplementacao&acao=A&aba=poloNucleo" );
	}
}
function atualizarPoloMunicipio2014($dados) {
	global $db;
	
	if ($dados ['pmupossuipolo'] == "TRUE") {
		$db->executar ( "UPDATE projovemurbano.municipio SET munstatus='I' WHERE pmuid='" . $dados ['pmuid'] . "'" );
	}
	
	if ($dados ['pmupossuipolo'] == "FALSE") {
		
		$tprid = $_SESSION ['projovemurbano'] ['ppuid'] != '' ? " AND tprid = " . $_SESSION ['projovemurbano'] ['ppuid'] : "";
		
		$sql = "UPDATE projovemurbano.municipio SET munstatus='I'
				WHERE munid IN(SELECT munid FROM projovemurbano.associamucipiopolo amp
							   INNER JOIN projovemurbano.polo pol ON amp.polid = pol.polid
							   INNER JOIN projovemurbano.polomunicipio pmu ON pmu.pmuid=pol.pmuid
							   WHERE pmu.pjuid='" . $_SESSION ['projovemurbano'] ['pjuid'] . "' $tprid AND pol.polstatus='A')";
		
		$db->executar ( $sql );
		
		$sql = "UPDATE projovemurbano.polo SET polstatus='I'
				WHERE polid IN(SELECT polid FROM projovemurbano.polo pol
							   INNER JOIN projovemurbano.polomunicipio pmu ON pmu.pmuid=pol.pmuid
							   WHERE pmu.pjuid='" . $_SESSION ['projovemurbano'] ['pjuid'] . "' $tprid )";
		
		$db->executar ( $sql );
	}
	
	$sql = "UPDATE projovemurbano.polomunicipio
   			SET pmuqtdpolo=" . (($dados ['pmuqtdpolo']) ? "'" . $dados ['pmuqtdpolo'] . "'" : "NULL") . ",
   				pmuqtdmunicipio=" . (($dados ['pmuqtdmunicipio']) ? "'" . $dados ['pmuqtdmunicipio'] . "'" : "NULL") . ",
   				pmupossuipolo=" . (($dados ['pmupossuipolo']) ? $dados ['pmupossuipolo'] : 'NULL') . "
 			WHERE pmuid='" . $dados ['pmuid'] . "';";
	
	$db->executar ( $sql );
	
	$db->commit ();
	
	header ( "Cache-Control: no-cache, must-revalidate" );
	if ($_SESSION ['projovemurbano'] ['ppuid'] == '2') {
		header ( "location:projovemurbano.php?modulo=principal/planoImplementacao2013&acao=A&aba={$_GET['aba']}&aba2=poloNucleo" );
	} elseif ($_SESSION ['projovemurbano'] ['ppuid'] == '3') {
		header ( "location:projovemurbano.php?modulo=principal/indexPoloNucleo2014&acao=A&aba={$_GET['aba']}&aba2=poloNucleo" );
	} else {
		header ( "location:projovemurbano.php?modulo=principal/planoImplementacao&acao=A&aba=poloNucleo" );
	}
}
function listarMunicipiosCadastrados($dados) {
	global $db;
	
	if ($dados ['adesao'])
		$adesao = " AND prj.adesaotermo=TRUE";
	
	$sql = "SELECT mun.mundescricao, usu.usunome, usu.usuemail, '('||SUBSTR(ifs.isetelefone::text,1,2)||')'||SUBSTR(ifs.isetelefone::text,3) as tel 
                FROM projovemurbano.usuarioresponsabilidade urs 
                LEFT JOIN seguranca.usuario usu ON usu.usucpf=urs.usucpf 
                LEFT JOIN projovemurbano.projovemurbano prj ON urs.muncod=prj.muncod
                INNER JOIN territorios.municipio mun ON mun.muncod=urs.muncod 
                LEFT JOIN projovemurbano.identificacaosecretario ifs ON ifs.pjuid=prj.pjuid 
                WHERE mun.muncod IS NOT NULL AND rpustatus='A'" . $adesao . "
                ORDER BY mun.mundescricao, usu.usunome
        ";
	
	$cabecalho = array (
			"Município",
			"Secretário",
			"E-mail",
			"Telefone" 
	);
	
	if (! $dados ['relatorio'])
		echo "<div style=height:370;overflow:auto;>";
	$db->monta_lista_simples ( $sql, $cabecalho, 50, 5, 'N', '100%', $par2 );
	if (! $dados ['relatorio'])
		echo "</div>";
	if (! $dados ['relatorio'])
		echo "<p align=center><input type=button value=Fechar onclick=\"closeMessage();\"></p>";
}
function listarEstadosCadastrados($dados) {
	global $db;
	
	if ($dados ['adesao'])
		$adesao = " AND prj.adesaotermo=TRUE";
	
	$sql = "SELECT est.estdescricao, usu.usunome, usu.usuemail, '('||SUBSTR(ifs.isetelefone::text,1,2)||')'||SUBSTR(ifs.isetelefone::text,3) as tel 
                FROM projovemurbano.usuarioresponsabilidade urs 
                LEFT JOIN seguranca.usuario usu ON usu.usucpf=urs.usucpf 
                LEFT JOIN projovemurbano.projovemurbano prj ON urs.estuf=prj.estuf
                INNER JOIN territorios.estado est ON est.estuf=urs.estuf 
                LEFT JOIN projovemurbano.identificacaosecretario ifs ON ifs.pjuid=prj.pjuid 
                WHERE est.estuf IS NOT NULL AND rpustatus='A'" . $adesao . "
                ORDER BY est.estdescricao, usu.usunome            
        ";
	
	$cabecalho = array (
			"Estados",
			"Secretário",
			"E-mail",
			"Telefone" 
	);
	
	echo "<div style=height:370;overflow:auto;>";
	$db->monta_lista_simples ( $sql, $cabecalho, 50, 5, 'N', '100%', $par2 );
	echo "</div>";
	echo "<p align=center><input type=button value=Fechar onclick=\"closeMessage();\"></p>";
}
function registarUltimoAcesso() {
	global $db;
	
	$db->executar ( "UPDATE projovemurbano.projovemurbano SET paginaultimoacesso='" . $_SERVER ['REQUEST_URI'] . "' WHERE pjuid='" . $_SESSION ['projovemurbano'] ['pjuid'] . "'" );
	$db->commit ();
}
function encaminharUltimoAcesso() {
	global $db;
	$sql = "SELECT paginaultimoacesso FROM projovemurbano.projovemurbano WHERE pjuid='" . $_SESSION ['projovemurbano'] ['pjuid'] . "'";
	$paginaultimoacesso = $db->pegaUm ( $sql );
	
	if ($paginaultimoacesso && $_SERVER ['REQUEST_URI'] != $paginaultimoacesso) {
		die ( "<script>window.location='{$paginaultimoacesso}';</script>" );
	}
}
function carregarProJovemUrbanoUF_MUNCOD() {
	global $db;
	
	if ($_SESSION['projovemurbano']['pjuid']) {
		$sql = "SELECT
					estuf,
					muncod
				FROM
					projovemurbano.projovemurbano
				WHERE
					pjuid = ".$_SESSION['projovemurbano']['pjuid'];
		$dados = $db->pegaLinha ( $sql );
		$_SESSION['projovemurbano']['muncod'] = $dados['muncod'];
		$_SESSION['projovemurbano']['estuf'] = $dados['estuf'];
	}
}
function montaTituloEstMun() {
	global $db;
	
	if ($_SESSION ['projovemurbano'] ['muncod']) {
		$sql = "SELECT mundescricao as descricao, estuf as uf
					FROM territorios.municipio
					WHERE muncod = '" . $_SESSION ['projovemurbano'] ['muncod'] . "'";
		$dado = $db->pegaLinha ( $sql );
	}
	
	if ($_SESSION ['projovemurbano'] ['estuf']) {
		$sql = "SELECT estdescricao as descricao, estuf as uf
			FROM territorios.estado
					WHERE estuf = '" . $_SESSION ['projovemurbano'] ['estuf'] . "'";
		$dado = $db->pegaLinha ( $sql );
	}
	
	return $dado ['descricao'];
}
function aceitarTermoAjustado($dados) {
	global $db;
	
	$sql = "UPDATE projovemurbano.projovemurbano SET adesaotermoajustadodata=NOW(), adesaotermoajustado=TRUE WHERE pjuid='" . $_SESSION ['projovemurbano'] ['pjuid'] . "'";

	$db->executar ( $sql );
	$db->commit ();
	
	echo "<script>
			alert('Gravado com sucesso');
			//window.location='projovemurbano.php?modulo=principal/planoImplementacao&acao=A';
			window.location='projovemurbano.php?modulo=principal/termoAdesaoAjustado&acao=A';
		  </script>";
}
function naoAceitarTermoAjustado() {
	global $db;
	$sql = "UPDATE projovemurbano.projovemurbano SET adesaotermoajustado=FALSE WHERE pjuid='" . $_SESSION ['projovemurbano'] ['pjuid'] . "'";
	$db->executar ( $sql );
	$db->commit ();
	
	echo "<script>
			alert('Termo Ajustado não foi aceito com sucesso');
			window.location='projovemurbano.php?modulo=principal/" . (($_SESSION ['projovemurbano'] ['estuf']) ? "listaEstados" : "") . (($_SESSION ['projovemurbano'] ['muncod']) ? "listaMunicipios" : "") . "&acao=A';
		  </script>";
}
function montaMenuProJovemUrbano() {
	global $db;
	
	include_once APPRAIZ . 'projovemurbano/modulos/principal/filtroAnoExercicio.inc';
	echo "<br>";
	
	$docid = $db->pegaUm ("SELECT docid FROM projovemurbano.projovemurbano WHERE pjuid='" . $_SESSION ['projovemurbano'] ['pjuid'] . "'" );
	if (!$docid){
		$docid = criaDocumento();
		$db->executar ("UPDATE projovemurbano.projovemurbano SET docid='" . $docid . "' WHERE pjuid='" . $_SESSION ['projovemurbano'] ['pjuid'] . "'" );
		$db->commit ();
	}
	
	$esdid = $db->pegaUm( "SELECT esdid FROM workflow.documento WHERE docid='" . $docid . "'" );
	
	$perfis = pegaPerfilGeral ();
	
	if (! $_SESSION ['projovemurbano'] ['pjuid']) {
		die ( "
            <script>
                alert('Problemas de navegação. Inicie novamente.');
                window.location='projovemurbano.php?modulo=inicio&acao=C';
            </script>" );
	}
	
	if($_SESSION['projovemurbano']['ppuid']=='3'&& !$db->testa_superuser ()){
		$sqlrapid = "SELECT
						rapid
					FROM
						projovemurbano.projovemurbano
					WHERE
						pjuid = {$_SESSION ['projovemurbano'] ['pjuid']}
					";
		$rapid = $db->pegaUm ( $sqlrapid );
	}
	$sql = "SELECT * FROM projovemurbano.coordenadorresponsavel WHERE pjuid='" . $_SESSION ['projovemurbano'] ['pjuid'] . "'";
	$coordenadorresponsavel = $db->pegaLinha ( $sql );
	// Adaptação para o perfil Diretor do Pólo
	if (! $db->testa_superuser ()) {
		if (in_array ( PFL_DIRETOR_POLO, $perfis ) || in_array ( PFL_DIRETOR_NUCLEO, $perfis )) {
			
			switch ($_SESSION ['projovemurbano'] ['ppuid']) {
				case '3' :
					if($rapid!=''){
						$urlMonitoramento = "/projovemurbano/projovemurbano.php?modulo=principal/monitoramento2014&acao=A";
					}
					break;
				case '2' :
					$urlMonitoramento = "/projovemurbano/projovemurbano.php?modulo=principal/monitoramento2013&acao=A";
					break;
				default :
					
					$urlMonitoramento = '/projovemurbano/projovemurbano.php?modulo=principal/monitoramento&acao=A';
					break;
			}
			if ($coordenadorresponsavel && $_SESSION ['projovemurbano'] ['ppuid']) {
				if($urlMonitoramento!=''){
					$menu = array (
							0 => array (
									"id" => 1,
									"descricao" => "Monitoramento",
									"link" => $urlMonitoramento 
							) 
					);
				}
			}
			return $menu;
		}
	}
	
	// Adaptação para o perfil Diretor do Pólo
	$menu = array (
			0 => array (
					"id" => 1,
					"descricao" => "Instruções",
					"link" => "/projovemurbano/projovemurbano.php?modulo=principal/instrucao&acao=A" . (($_SESSION ['projovemurbano'] ['estuf']) ? "&estuf=" . $_SESSION ['projovemurbano'] ['estuf'] : "") . (($_SESSION ['projovemurbano'] ['muncod']) ? "&muncod=" . $_SESSION ['projovemurbano'] ['muncod'] : "") 
			),
			1 => array (
					"id" => 2,
					"descricao" => "Identificação",
					"link" => "/projovemurbano/projovemurbano.php?modulo=principal/identificacao&acao=A" 
			) 
	);
	
	$identificacao = $db->pegaUm ( "SELECT isecid FROM projovemurbano.identificacaosecretario WHERE pjuid='" . $_SESSION ['projovemurbano'] ['pjuid'] . "' --AND ppuid = '{$_SESSION['projovemurbano']['ppuid']}'" );
	
	// ONTA MENU MUNICIPIO.
	// ver($_SESSION['projovemurbano']['muncod'],$identificacao,d);
	if ($_SESSION ['projovemurbano'] ['muncod'] && $identificacao) {
		if (in_array ( PFL_SUPER_USUARIO, $perfis ) || in_array ( PFL_EQUIPE_MEC, $perfis ) || in_array ( PFL_ADMINISTRADOR, $perfis ) || in_array ( PFL_COORDENADOR_MUNICIPAL, $perfis )) {
			if ($_SESSION ['projovemurbano'] ['ppuid'] == '1') {
				$menu [] = array (
						'id' => 2.1,
						'descricao' => 'Meta',
						'link' => '/projovemurbano/projovemurbano.php?modulo=principal/metaAtendimentoMun&acao=A' 
				);
			}
		}
		switch ($_SESSION ['projovemurbano'] ['ppuid']) {
			case 1 :
				$metaDirecionada = 1;
				break;
			case 2 :
				$idCargaMeta = $db->pegaUm ( "select cmeid from projovemurbano.cargameta where cmecodibge = '{$_SESSION['projovemurbano']['muncod']}' and ppuid = {$_SESSION['projovemurbano']['ppuid']}" );
				$metaDirecionada = $db->pegaUm ( "SELECT count (mtpid) as mtpid FROM projovemurbano.metasdoprograma WHERE cmeid = " . $idCargaMeta );
				break;
			case 3 :
				$idCargaMeta = $db->pegaUm ( "select cmeid from projovemurbano.cargameta where cmecodibge = '{$_SESSION['projovemurbano']['muncod']}' and ppuid = {$_SESSION['projovemurbano']['ppuid']}" );
				$metaDirecionada = $db->pegaUm ( "SELECT count (mtpid) as mtpid FROM projovemurbano.metasdoprograma WHERE cmeid = " . $idCargaMeta );
				break;
		}
		
		if ($metaDirecionada > 0) {
			
			$adesaotermo = $db->pegaLinha ( "SELECT adesaotermo, adesaotermoajustado FROM projovemurbano.projovemurbano WHERE pjuid='{$_SESSION['projovemurbano']['pjuid']}' AND ppuid = {$_SESSION['projovemurbano']['ppuid']}" );
			
			$abaTermo = podeMostrarTermosMetas ();
			// ver($abaTermo,d);
			if ($abaTermo) {
				$menu [] = array (
						"id" => 3,
						"descricao" => "Termo de Adesão",
						"link" => "/projovemurbano/projovemurbano.php?modulo=principal/termoAdesao&acao=A" 
				);
			}
			
			if ($adesaotermo ['adesaotermo'] == "t") {
				
				if (in_array ( PFL_CONSULTA, $perfis ) || in_array ( PFL_SUPER_USUARIO, $perfis ) || in_array ( PFL_EQUIPE_MEC, $perfis ) || in_array ( PFL_ADMINISTRADOR, $perfis ) || in_array ( PFL_SECRETARIO_MUNICIPAL, $perfis ) || in_array ( PFL_SECRETARIO_ESTADUAL, $perfis )) {
					$menu [] = array (
							"id" => 4,
							"descricao" => "Sugestão de Meta",
							"link" => "/projovemurbano/projovemurbano.php?modulo=principal/sugestaoAmpliacao&acao=A" 
					);
				}
				
				$suametaajustada = $db->pegaUm ( "SELECT suaverdade FROM projovemurbano.sugestaoampliacao WHERE pjuid='" . $_SESSION ['projovemurbano'] ['pjuid'] . "'" );
				
				$abaTermoAjustado = podeMostrarTermosMetas ( array (
						'ajustado' => true 
				) );
				
				if ($abaTermoAjustado && $suametaajustada == 't') {
					$menu [] = array (
							"id" => 5,
							"descricao" => "Termo de adesão ajustado",
							"link" => "/projovemurbano/projovemurbano.php?modulo=principal/termoAdesaoAjustado&acao=A" 
					);
				}
				
				$suaverdade = $db->pegaUm ( "SELECT suaverdade FROM projovemurbano.sugestaoampliacao WHERE pjuid='" . $_SESSION ['projovemurbano'] ['pjuid'] . "'" );
				
				if (($suaverdade == "t" || $suaverdade == "f") && $_SESSION ['projovemurbano'] ['ppuano'] == 2012) {
					
					$menu [] = array (
							"id" => 7,
							"descricao" => "Plano de implementação",
							"link" => "/projovemurbano/projovemurbano.php?modulo=principal/planoImplementacao&acao=A" 
					);
				}
				if (/*($suaverdade == "t" || $suaverdade == "f") && */$_SESSION ['projovemurbano'] ['ppuano'] == 2013) {
					if (in_array ( PFL_CONSULTA, $perfis ) || in_array ( PFL_COORDENADOR_MUNICIPAL, $perfis )) {
						$sql = "SELECT
				            		true
			            		FROM
			            			projovemurbano.coordenadorresponsavel
			            		WHERE
				            		corcpf = '{$_SESSION['usucpf']}'
				            		AND pjuid = {$_SESSION['projovemurbano']['pjuid']}
				            		AND corstatus = 'A' ";
						$testa = $db->pegaUm ( $sql );
						if ($testa == 't') {
							$menu [] = array (
									"id" => 7,
									"descricao" => "Plano de implementação",
									"link" => "projovemurbano.php?modulo=principal/planoImplementacao2013&acao=A" 
							);
						}
					}
					if (in_array ( PFL_CONSULTA, $perfis ) || in_array ( PFL_SECRETARIO_MUNICIPAL, $perfis ) || in_array ( PFL_ADMINISTRADOR, $perfis ) || $db->testa_superuser ()) {
						if (! in_array ( array (
								"id" => 7,
								"descricao" => "Plano de implementação",
								"link" => "projovemurbano.php?modulo=principal/planoImplementacao2013&acao=A" 
						), $menu )) {
							$menu [] = array (
									"id" => 7,
									"descricao" => "Plano de implementação",
									"link" => "projovemurbano.php?modulo=principal/planoImplementacao2013&acao=A" 
							);
						}
					}
				}
				if (($suaverdade == "t" || $suaverdade == "f") && $_SESSION ['projovemurbano'] ['ppuano'] == 2014) {
					$sql = "SELECT
		                			true
		                		FROM
		                			projovemurbano.coordenadorresponsavel
		                		WHERE
		                			--corcpf = '{$_SESSION['usucpf']}'
		                		--AND 
									pjuid = {$_SESSION['projovemurbano']['pjuid']}
		                		AND corstatus = 'A' ";
					$testa = $db->pegaUm ( $sql );
					if (in_array ( PFL_COORDENADOR_MUNICIPAL, $perfis )) {
						$menu [] = array (
								"id" => 6,
								"descricao" => "Pólo/Núcleo",
								"link" => "projovemurbano.php?modulo=principal/indexPoloNucleo2014&acao=A" 
						);
						if ($testa == 't') {
							$menu [] = array (
									"id" => 7,
									"descricao" => "Plano de implementação",
									"link" => "projovemurbano.php?modulo=principal/planoImplementacao2014&acao=A" 
							);
						}
					}
					if (! in_array ( array (
							"id" => 6,
							"descricao" => "Pólo/Núcleo",
							"link" => "projovemurbano.php?modulo=principal/indexPoloNucleo2014&acao=A" 
					), $menu )) {
						$menu [] = array (
								"id" => 6,
								"descricao" => "Pólo/Núcleo",
								"link" => "projovemurbano.php?modulo=principal/indexPoloNucleo2014&acao=A" 
						);
					}
					if (($testa == 't') && (in_array ( PFL_CONSULTA, $perfis ) || in_array ( PFL_SECRETARIO_MUNICIPAL, $perfis ) || in_array ( PFL_ADMINISTRADOR, $perfis ) || $db->testa_superuser ())) {
						// block2
						if (! in_array ( array (
								"id" => 7,
								"descricao" => "Plano de implementação",
								"link" => "projovemurbano.php?modulo=principal/planoImplementacao2014&acao=A" 
						), $menu )) {
							$menu [] = array (
									"id" => 7,
									"descricao" => "Plano de implementação",
									"link" => "projovemurbano.php?modulo=principal/planoImplementacao2014&acao=A" 
							);
						}
					}
				}
				if (in_array ( PFL_ADMINISTRADOR, $perfis ) || in_array ( PFL_CONSULTA, $perfis ) || in_array ( PFL_COORDENADOR_MUNICIPAL, $perfis ) || $db->testa_superuser ()/* && $_SESSION ['projovemurbano'] ['ppuano'] != 2014*/) {
					// block6
					$menu [] = array (
							"id" => 8,
							"descricao" => "Transferência de Aluno",
							"link" => "/projovemurbano/projovemurbano.php?modulo=principal/transferencia&acao=A" 
					);
				}
			}
		}
	}
	// MONTA MENU ESTADO.
	if ($_SESSION ['projovemurbano'] ['estuf'] && $identificacao) {
		if (in_array ( PFL_CONSULTA, $perfis ) || in_array ( PFL_SUPER_USUARIO, $perfis ) || in_array ( PFL_EQUIPE_MEC, $perfis ) || in_array ( PFL_ADMINISTRADOR, $perfis ) || in_array ( PFL_COORDENADOR_ESTADUAL, $perfis )) {
			if ($_SESSION ['projovemurbano'] ['ppuid'] == '2') {
				$menu [] = array (
						'id' => 2.1,
						'descricao' => 'Meta',
						'link' => '/projovemurbano/projovemurbano.php?modulo=principal/metaAtendimento&acao=A' 
				);
			}
		}
		
		$abaTermo = podeMostrarTermosMetas ();
		
		if ($abaTermo) {
			$menu [] = array (
					"id" => 3,
					"descricao" => "Termo de Adesão",
					"link" => "/projovemurbano/projovemurbano.php?modulo=principal/termoAdesao&acao=A" 
			);
		}
		
		$adesaotermo = $db->pegaLinha ( "SELECT adesaotermo, adesaotermoajustado FROM projovemurbano.projovemurbano WHERE pjuid='{$_SESSION['projovemurbano']['pjuid']}' AND ppuid = {$_SESSION['projovemurbano']['ppuid']}" );
		
		if ($adesaotermo ['adesaotermo'] == "t") {
			if (in_array ( PFL_CONSULTA, $perfis ) || in_array ( PFL_SUPER_USUARIO, $perfis ) || in_array ( PFL_EQUIPE_MEC, $perfis ) || in_array ( PFL_ADMINISTRADOR, $perfis ) || in_array ( PFL_SECRETARIO_MUNICIPAL, $perfis ) || in_array ( PFL_SECRETARIO_ESTADUAL, $perfis )) {
				$menu [] = array (
						"id" => 4,
						"descricao" => "Sugestão de Meta",
						"link" => "/projovemurbano/projovemurbano.php?modulo=principal/sugestaoAmpliacao&acao=A" 
				);
			}
			$suametaajustada = $db->pegaUm ( "SELECT SUM(mtpvalor) FROM projovemurbano.metasdoprograma WHERE pjuid= {$_SESSION ['projovemurbano']['pjuid']} AND ppuid = {$_SESSION ['projovemurbano']['ppuid']} AND tpmid in(15,12,9)" );
			
			$abaTermoAjustado = podeMostrarTermosMetas ( array (
					'ajustado' => true 
			) );
			if ($abaTermoAjustado && ($suametaajustada > 0)) {
				$menu [] = array (
						"id" => 5,
						"descricao" => "Termo de adesão ajustado",
						"link" => "/projovemurbano/projovemurbano.php?modulo=principal/termoAdesaoAjustado&acao=A" 
				);
			}
			
			$suaverdade = $db->pegaUm ( "SELECT suaverdade FROM projovemurbano.sugestaoampliacao WHERE pjuid='" . $_SESSION ['projovemurbano'] ['pjuid'] . "'" );
			if (($suaverdade == "t" || $suaverdade == "f") && $_SESSION ['projovemurbano'] ['ppuano'] == 2012) {
				$menu [] = array (
						"id" => 6,
						"descricao" => "Plano de implementação",
						"link" => "/projovemurbano/projovemurbano.php?modulo=principal/planoImplementacao&acao=A" 
				);
			}
			if (/*($suaverdade == "t" || $suaverdade == "f") && */$_SESSION ['projovemurbano'] ['ppuano'] == 2013) {
				$sql = "SELECT
			            		true
		            		FROM
		            			projovemurbano.coordenadorresponsavel
		            		WHERE
			            		--corcpf = '{$_SESSION['usucpf']}'
			            		--AND
					 pjuid = {$_SESSION['projovemurbano']['pjuid']}
			            		AND corstatus = 'A' ";
				$testa = $db->pegaUm ( $sql );
				if ($testa == 't') {
					$menu [] = array (
							"id" => 6,
							"descricao" => "Plano de implementação",
							"link" => "projovemurbano.php?modulo=principal/planoImplementacao2013&acao=A" 
					);
				}
				if ((in_array ( PFL_CONSULTA, $perfis ) || in_array ( PFL_CONSULTA, $perfis ) || in_array ( PFL_SECRETARIO_ESTADUAL, $perfis ) || in_array ( PFL_ADMINISTRADOR, $perfis ) || $db->testa_superuser ()) && ! in_array ( array (
						"id" => 6,
						"descricao" => "Plano de implementação",
						"link" => "projovemurbano.php?modulo=principal/planoImplementacao2013&acao=A" 
				), $menu )) {
					$menu [] = array (
							"id" => 6,
							"descricao" => "Plano de implementação",
							"link" => "projovemurbano.php?modulo=principal/planoImplementacao2013&acao=A" 
					);
				}
			}
			if (($suaverdade == "t" || $suaverdade == 'f') && $_SESSION ['projovemurbano'] ['ppuano'] == 2014) {
				
				// if (in_array ( PFL_CONSULTA, $perfis )||in_array ( PFL_SECRETARIO_ESTADUAL, $perfis ) || in_array ( PFL_ADMINISTRADOR, $perfis )||in_array ( PFL_COORDENADOR_ESTADUAL, $perfis ) || $db->testa_superuser ()) {
				$sql = "SELECT
		            			true
		            		FROM
		            			projovemurbano.coordenadorresponsavel
		            		WHERE
		            		--	corcpf = '{$_SESSION['usucpf']}'
		            		--AND 
								pjuid = {$_SESSION['projovemurbano']['pjuid']}
		            		AND corstatus = 'A' ";
				$testa = $db->pegaUm ( $sql );
				$menu [] = array (
						"id" => 6,
						"descricao" => "Pólo/Núcleo",
						"link" => "projovemurbano.php?modulo=principal/indexPoloNucleo2014&acao=A" 
				);
				if (! in_array ( array (
						"id" => 6,
						"descricao" => "Pólo/Núcleo",
						"link" => "projovemurbano.php?modulo=principal/indexPoloNucleo2014&acao=A" 
				), $menu )) {
					$menu [] = array (
							"id" => 6,
							"descricao" => "Pólo/Núcleo",
							"link" => "projovemurbano.php?modulo=principal/indexPoloNucleo2014&acao=A" 
					);
				}
				if (in_array ( PFL_COORDENADOR_ESTADUAL, $perfis )) {
					if ($testa == 't') {
						$menu [] = array (
								"id" => 7,
								"descricao" => "Plano de implementação",
								"link" => "projovemurbano.php?modulo=principal/planoImplementacao2014&acao=A" 
						);
					}
				}
				if (($testa == 't') && (in_array ( PFL_CONSULTA, $perfis ) || in_array ( PFL_SECRETARIO_ESTADUAL, $perfis ) || in_array ( PFL_ADMINISTRADOR, $perfis ) || $db->testa_superuser () && ! in_array ( array (
						"id" => 7,
						"descricao" => "Plano de implementação",
						"link" => "projovemurbano.php?modulo=principal/planoImplementacao2014&acao=A" 
				), $menu ))) {
					$menu [] = array (
							"id" => 7,
							"descricao" => "Plano de implementação",
							"link" => "projovemurbano.php?modulo=principal/planoImplementacao2014&acao=A" 
					);
				}
			}
			
			if (in_array ( PFL_ADMINISTRADOR, $perfis ) || in_array ( PFL_CONSULTA, $perfis ) || in_array ( PFL_COORDENADOR_ESTADUAL, $perfis ) || $db->testa_superuser () /*&& $_SESSION ['projovemurbano'] ['ppuano'] != 2014*/) {
				$menu [] = array (
						"id" => 8,
						"descricao" => "Transferência de Aluno",
						"link" => "/projovemurbano/projovemurbano.php?modulo=principal/transferencia&acao=A" 
				);
			}
		}
	}
	if ($db->testa_superuser () || in_array ( PFL_ADMINISTRADOR, $perfis ) || (in_array ( PFL_COORDENADOR_ESTADUAL, $perfis ) || in_array ( PFL_COORDENADOR_MUNICIPAL, $perfis ) || in_array ( PFL_EQUIPE_MEC, $perfis ) || in_array ( PFL_CONSULTA, $perfis ))
//             && $esdid != ESD_EMELABORACAO 
				) {

		switch ($_SESSION ['projovemurbano'] ['ppuid']) {
			
			case '3' :
				if (($coordenadorresponsavel&&$rapid!='')||$db->testa_superuser ()||in_array ( PFL_ADMINISTRADOR, $perfis )||in_array ( PFL_EQUIPE_MEC, $perfis )) {
					$urlMonitoramento = "/projovemurbano/projovemurbano.php?modulo=principal/monitoramento2014&acao=A";
				}
				break;
			
			case '2' :
				$urlMonitoramento = "/projovemurbano/projovemurbano.php?modulo=principal/monitoramento2013&acao=A";
				break;
			
			default :
				$urlMonitoramento = '/projovemurbano/projovemurbano.php?modulo=principal/monitoramento&acao=A';
				break;
		}
		if ($coordenadorresponsavel && $_SESSION ['projovemurbano'] ['ppuid']) {
			if($urlMonitoramento!=''||$db->testa_superuser ()|| in_array ( PFL_ADMINISTRADOR, $perfis )){
				$menu [] = array (
						"id" => 9,
						"descricao" => "Monitoramento",
						"link" => $urlMonitoramento 
				);
			}
		}
	}
	
	return $menu;
}
function montaMenuMonitoramento() {
	global $db;
	$menu = array ();
	$perfis = pegaPerfilGeral ();
	
	if (1 == $_SESSION ['projovemurbano'] ['ppuid']) { // -- projovem urbando 2012
		if ($db->testa_superuser () || in_array ( PFL_SUPER_USUARIO, $perfis ) || in_array ( PFL_EQUIPE_MEC, $perfis ) || in_array ( PFL_CONSULTA, $perfis )) {
			$menu [] = array (
					"id" => 1,
					"descricao" => "Cadastro de Estudantes",
					"link" => "/projovemurbano/projovemurbano.php?modulo=principal/monitoramento&acao=A&aba=cadastroEstudantes" 
			);
			$menu [] = array (
					"id" => 2,
					"descricao" => "Diários de Frequência e Trabalhos",
					"link" => "/projovemurbano/projovemurbano.php?modulo=principal/monitoramento&acao=A&aba=diarioFrequencia" 
			);
			$menu [] = array (
					"id" => 3,
					"descricao" => "Frequência Mensal",
					"link" => "/projovemurbano/projovemurbano.php?modulo=principal/monitoramento&acao=A&aba=frequenciaMensal" 
			);
			$menu [] = array (
					"id" => 4,
					"descricao" => "Trabalho Mensal",
					"link" => "/projovemurbano/projovemurbano.php?modulo=principal/monitoramento&acao=A&aba=trabalhoMensal" 
			);
			$menu [] = array (
					"id" => 5,
					"descricao" => "Agência Bancária",
					"link" => "/projovemurbano/projovemurbano.php?modulo=principal/monitoramento&acao=A&aba=agencias" 
			);
			$menu [] = array (
					"id" => 6,
					"descricao" => "Encaminhar Lista",
					"link" => "/projovemurbano/projovemurbano.php?modulo=principal/monitoramento&acao=A&aba=encaminharLista" 
			);
			$menu [] = array (
					"id" => 7,
					"descricao" => "Lançamento de Notas",
					"link" => "/projovemurbano/projovemurbano.php?modulo=principal/monitoramento&acao=A&aba=lancamentoNotas" 
			);
			$menu [] = array (
					"id" => 8,
					"descricao" => "Acompanhamento de Frequência e Notas",
					"link" => "/projovemurbano/projovemurbano.php?modulo=principal/monitoramento&acao=A&aba=relatorio_acompanhamento_freq" 
			);
		} else if (in_array ( PFL_DIRETOR_NUCLEO, $perfis )) {
			$menu [] = array (
					"id" => 1,
					"descricao" => "Cadastro de Estudantes",
					"link" => "/projovemurbano/projovemurbano.php?modulo=principal/monitoramento&acao=A&aba=cadastroEstudantes" 
			);
			$menu [] = array (
					"id" => 2,
					"descricao" => "Diários de Frequência e Trabalhos",
					"link" => "/projovemurbano/projovemurbano.php?modulo=principal/monitoramento&acao=A&aba=diarioFrequencia" 
			);
			$menu [] = array (
					"id" => 3,
					"descricao" => "Frequência Mensal",
					"link" => "/projovemurbano/projovemurbano.php?modulo=principal/monitoramento&acao=A&aba=frequenciaMensal" 
			);
			$menu [] = array (
					"id" => 4,
					"descricao" => "Trabalho Mensal",
					"link" => "/projovemurbano/projovemurbano.php?modulo=principal/monitoramento&acao=A&aba=trabalhoMensal" 
			);
			$menu [] = array (
					"id" => 6,
					"descricao" => "Encaminhar Lista",
					"link" => "/projovemurbano/projovemurbano.php?modulo=principal/monitoramento&acao=A&aba=encaminharLista" 
			);
			$menu [] = array (
					"id" => 7,
					"descricao" => "Lançamento de Notas",
					"link" => "/projovemurbano/projovemurbano.php?modulo=principal/monitoramento&acao=A&aba=lancamentoNotas" 
			);
			$menu [] = array (
					"id" => 8,
					"descricao" => "Acompanhamento de Frequência e Notas",
					"link" => "/projovemurbano/projovemurbano.php?modulo=principal/monitoramento&acao=A&aba=relatorio_acompanhamento_freq" 
			);
		} else if (in_array ( PFL_DIRETOR_POLO, $perfis )) {
			$menu [] = array (
					"id" => 6,
					"descricao" => "Lançamento de Notas",
					"link" => "/projovemurbano/projovemurbano.php?modulo=principal/monitoramento&acao=A&aba=lancamentoNotas" 
			);
			$menu [] = array (
					"id" => 7,
					"descricao" => "Encaminhar Lista",
					"link" => "/projovemurbano/projovemurbano.php?modulo=principal/monitoramento&acao=A&aba=encaminharLista" 
			);
			$menu [] = array (
					"id" => 8,
					"descricao" => "Acompanhamento de Frequência e Notas",
					"link" => "/projovemurbano/projovemurbano.php?modulo=principal/monitoramento&acao=A&aba=relatorio_acompanhamento_freq" 
			);
		} else if (in_array ( PFL_COORDENADOR_ESTADUAL, $perfis ) || in_array ( PFL_COORDENADOR_MUNICIPAL, $perfis )) {
			$menu [] = array (
					"id" => 1,
					"descricao" => "Cadastro de Estudantes",
					"link" => "/projovemurbano/projovemurbano.php?modulo=principal/monitoramento&acao=A&aba=cadastroEstudantes" 
			);
			$menu [] = array (
					"id" => 5,
					"descricao" => "Agência",
					"link" => "/projovemurbano/projovemurbano.php?modulo=principal/monitoramento&acao=A&aba=agencias" 
			);
			$menu [] = array (
					"id" => 6,
					"descricao" => "Encaminhar Lista",
					"link" => "/projovemurbano/projovemurbano.php?modulo=principal/monitoramento&acao=A&aba=encaminharLista" 
			);
			$menu [] = array (
					"id" => 7,
					"descricao" => "Lançamento de Notas",
					"link" => "/projovemurbano/projovemurbano.php?modulo=principal/monitoramento&acao=A&aba=lancamentoNotas" 
			);
			$menu [] = array (
					"id" => 8,
					"descricao" => "Acompanhamento de Frequência e Notas",
					"link" => "/projovemurbano/projovemurbano.php?modulo=principal/monitoramento&acao=A&aba=relatorio_acompanhamento_freq" 
			);
		}
	} elseif ($_SESSION ['projovemurbano'] ['ppuid'] == 2) {
		if ($db->testa_superuser () || in_array ( PFL_SUPER_USUARIO, $perfis ) || in_array ( PFL_EQUIPE_MEC, $perfis ) || in_array ( PFL_CONSULTA, $perfis )) {
			$menu [] = array (
					"id" => 1,
					"descricao" => "Cadastro de Estudantes",
					"link" => "/projovemurbano/projovemurbano.php?modulo=principal/monitoramento2013&acao=A&aba=cadastroEstudantes" 
			);
			$menu [] = array (
					"id" => 2,
					"descricao" => "Diários de Frequência e Trabalhos",
					"link" => "/projovemurbano/projovemurbano.php?modulo=principal/monitoramento&acao=A&aba=diarioFrequencia" 
			);
			$menu [] = array (
					"id" => 3,
					"descricao" => "Frequência Mensal",
					"link" => "/projovemurbano/projovemurbano.php?modulo=principal/monitoramento&acao=A&aba=frequenciaMensal" 
			);
			$menu [] = array (
					"id" => 4,
					"descricao" => "Trabalho Mensal",
					"link" => "/projovemurbano/projovemurbano.php?modulo=principal/monitoramento&acao=A&aba=trabalhoMensal" 
			);
			$menu [] = array (
					"id" => 5,
					"descricao" => "Agência Bancária",
					"link" => "/projovemurbano/projovemurbano.php?modulo=principal/monitoramento&acao=A&aba=agencias" 
			);
			$menu [] = array (
					"id" => 6,
					"descricao" => "Encaminhar Lista",
					"link" => "/projovemurbano/projovemurbano.php?modulo=principal/monitoramento&acao=A&aba=encaminharLista" 
			);
			$menu [] = array (
					"id" => 7,
					"descricao" => "Lançamento de Notas",
					"link" => "/projovemurbano/projovemurbano.php?modulo=principal/monitoramento&acao=A&aba=lancamentoNotas" 
			);
			$menu [] = array (
					"id" => 8,
					"descricao" => "Acompanhamento de Frequência e Notas",
					"link" => "/projovemurbano/projovemurbano.php?modulo=principal/monitoramento&acao=A&aba=relatorio_acompanhamento_freq" 
			);
		} else if (in_array ( PFL_DIRETOR_NUCLEO, $perfis )) {
			$menu [] = array (
					"id" => 1,
					"descricao" => "Cadastro de Estudantes",
					"link" => "/projovemurbano/projovemurbano.php?modulo=principal/monitoramento2013&acao=A&aba=cadastroEstudantes" 
			);
			$menu [] = array (
					"id" => 2,
					"descricao" => "Diários de Frequência e Trabalhos",
					"link" => "/projovemurbano/projovemurbano.php?modulo=principal/monitoramento&acao=A&aba=diarioFrequencia" 
			);
			$menu [] = array (
					"id" => 3,
					"descricao" => "Frequência Mensal",
					"link" => "/projovemurbano/projovemurbano.php?modulo=principal/monitoramento&acao=A&aba=frequenciaMensal" 
			);
			$menu [] = array (
					"id" => 4,
					"descricao" => "Trabalho Mensal",
					"link" => "/projovemurbano/projovemurbano.php?modulo=principal/monitoramento&acao=A&aba=trabalhoMensal" 
			);
			$menu [] = array (
					"id" => 6,
					"descricao" => "Encaminhar Lista",
					"link" => "/projovemurbano/projovemurbano.php?modulo=principal/monitoramento&acao=A&aba=encaminharLista" 
			);
			$menu [] = array (
					"id" => 7,
					"descricao" => "Lançamento de Notas",
					"link" => "/projovemurbano/projovemurbano.php?modulo=principal/monitoramento&acao=A&aba=lancamentoNotas" 
			);
			$menu [] = array (
					"id" => 8,
					"descricao" => "Acompanhamento de Frequência e Notas",
					"link" => "/projovemurbano/projovemurbano.php?modulo=principal/monitoramento&acao=A&aba=relatorio_acompanhamento_freq" 
			);
		} else if (in_array ( PFL_DIRETOR_POLO, $perfis )) {
			$menu [] = array (
					"id" => 6,
					"descricao" => "Lançamento de Notas",
					"link" => "/projovemurbano/projovemurbano.php?modulo=principal/monitoramento&acao=A&aba=lancamentoNotas" 
			);
			$menu [] = array (
					"id" => 7,
					"descricao" => "Encaminhar Lista",
					"link" => "/projovemurbano/projovemurbano.php?modulo=principal/monitoramento&acao=A&aba=encaminharLista" 
			);
			$menu [] = array (
					"id" => 8,
					"descricao" => "Acompanhamento de Frequência e Notas",
					"link" => "/projovemurbano/projovemurbano.php?modulo=principal/monitoramento&acao=A&aba=relatorio_acompanhamento_freq" 
			);
		} else if (in_array ( PFL_COORDENADOR_ESTADUAL, $perfis ) || in_array ( PFL_COORDENADOR_MUNICIPAL, $perfis )) {
			$menu [] = array (
					"id" => 1,
					"descricao" => "Cadastro de Estudantes",
					"link" => "/projovemurbano/projovemurbano.php?modulo=principal/monitoramento2013&acao=A&aba=cadastroEstudantes" 
			);
			$menu [] = array (
					"id" => 5,
					"descricao" => "Agência",
					"link" => "/projovemurbano/projovemurbano.php?modulo=principal/monitoramento&acao=A&aba=agencias" 
			);
			$menu [] = array (
					"id" => 6,
					"descricao" => "Encaminhar Lista",
					"link" => "/projovemurbano/projovemurbano.php?modulo=principal/monitoramento&acao=A&aba=encaminharLista" 
			);
			$menu [] = array (
					"id" => 7,
					"descricao" => "Lançamento de Notas",
					"link" => "/projovemurbano/projovemurbano.php?modulo=principal/monitoramento&acao=A&aba=lancamentoNotas" 
			);
			$menu [] = array (
					"id" => 8,
					"descricao" => "Acompanhamento de Frequência e Notas",
					"link" => "/projovemurbano/projovemurbano.php?modulo=principal/monitoramento&acao=A&aba=relatorio_acompanhamento_freq" 
			);
		}
	} elseif ($_SESSION ['projovemurbano'] ['ppuid'] == 3) {

		$sql = "SELECT distinct
						ordem
					FROM
							projovemurbano.rangeperiodo rap
					INNER JOIN projovemurbano.projovemurbano pju ON pju.rapid = rap.rapid
					WHERE
						pju.pjuid = {$_SESSION['projovemurbano']['pjuid']}";

		$ordem = $db->pegaUm($sql);

		if ($db->testa_superuser () || in_array ( PFL_SUPER_USUARIO, $perfis ) || in_array ( PFL_EQUIPE_MEC, $perfis ) || in_array ( PFL_CONSULTA, $perfis )) {
			$menu [] = array(
				"id" => 1,
				"descricao" => "Cadastro de Estudantes",
				"link" => "/projovemurbano/projovemurbano.php?modulo=principal/monitoramento2014&acao=A&aba={$_GET['aba']}&aba2=cadastroEstudantes"
			);
			if ($ordem!='') {
				$menu[] = array("id" => 2, "descricao" => "Diários de Frequência e Trabalhos", "link" => "/projovemurbano/projovemurbano.php?modulo=principal/monitoramento2014&acao=A&aba={$_GET['aba']}&aba2=diarioFrequencia");
				$menu[] = array("id" => 3, "descricao" => "Frequência Mensal", "link" => "/projovemurbano/projovemurbano.php?modulo=principal/monitoramento2014&acao=A&aba={$_GET['aba']}&aba2=frequenciaMensal");
				$menu[] = array("id" => 4, "descricao" => "Trabalho Mensal", "link" => "/projovemurbano/projovemurbano.php?modulo=principal/monitoramento2014&acao=A&aba={$_GET['aba']}&aba2=trabalhoMensal");
				$menu[] = array("id" => 7, "descricao" => "Lançamento de Notas", "link" => "/projovemurbano/projovemurbano.php?modulo=principal/monitoramento2014&acao=A&aba={$_GET['aba']}&aba2=lancamentoNotas");
			}
				$menu[] = array("id" => 5, "descricao" => "Agência Bancária", "link" => "/projovemurbano/projovemurbano.php?modulo=principal/monitoramento2014&acao=A&aba={$_GET['aba']}&aba2=agencias");
				$menu[] = array("id" => 6, "descricao" => "Encaminhar Lista", "link" => "/projovemurbano/projovemurbano.php?modulo=principal/monitoramento2014&acao=A&aba={$_GET['aba']}&aba2=encaminharLista");
				$menu[] = array("id" => 8, "descricao" => "Acompanhamento de Frequência e Notas", "link" => "/projovemurbano/projovemurbano.php?modulo=principal/monitoramento2014&acao=A&aba={$_GET['aba']}&aba2=relatorio_acompanhamento_freq");

		} else if (in_array ( 650, $perfis )) {

			$menu [] = array (
					"id" => 1,
					"descricao" => "Cadastro de Estudantes",
					"link" => "/projovemurbano/projovemurbano.php?modulo=principal/monitoramento2014&acao=A&aba={$_GET['aba']}&aba2=cadastroEstudantes" 
			);
			if ($ordem!='') {
				$menu[] = array("id" => 2, "descricao" => "Diários de Frequência e Trabalhos", "link" => "/projovemurbano/projovemurbano.php?modulo=principal/monitoramento2014&acao=A&aba={$_GET['aba']}&aba2=diarioFrequencia");
				$menu[] = array("id" => 3, "descricao" => "Frequência Mensal", "link" => "/projovemurbano/projovemurbano.php?modulo=principal/monitoramento2014&acao=A&aba={$_GET['aba']}&aba2=frequenciaMensal");
				$menu[] = array("id" => 4, "descricao" => "Trabalho Mensal", "link" => "/projovemurbano/projovemurbano.php?modulo=principal/monitoramento2014&acao=A&aba={$_GET['aba']}&aba2=trabalhoMensal");
				$menu[] = array("id" => 6, "descricao" => "Encaminhar Lista", "link" => "/projovemurbano/projovemurbano.php?modulo=principal/monitoramento2014&acao=A&aba={$_GET['aba']}&aba2=encaminharLista");
				$menu[] = array("id" => 7, "descricao" => "Lançamento de Notas", "link" => "/projovemurbano/projovemurbano.php?modulo=principal/monitoramento2014&acao=A&aba={$_GET['aba']}&aba2=lancamentoNotas");
				$menu[] = array("id" => 8, "descricao" => "Acompanhamento de Frequência e Notas", "link" => "/projovemurbano/projovemurbano.php?modulo=principal/monitoramento2014&acao=A&aba={$_GET['aba']}&aba2=relatorio_acompanhamento_freq");
			}
		} else if (in_array ( PFL_DIRETOR_POLO, $perfis )) {

			$menu[] = array("id" => 7, "descricao" => "Encaminhar Lista","link" => "/projovemurbano/projovemurbano.php?modulo=principal/monitoramento2014&acao=A&aba={$_GET['aba']}&aba2=encaminharLista");
			if ($ordem!='') {
				$menu[] = array("id" => 6, "descricao" => "Lançamento de Notas", "link" => "/projovemurbano/projovemurbano.php?modulo=principal/monitoramento2014&acao=A&aba={$_GET['aba']}&aba2=lancamentoNotas");
				$menu[] = array("id" => 8, "descricao" => "Acompanhamento de Frequência e Notas", "link" => "/projovemurbano/projovemurbano.php?modulo=principal/monitoramento2014&acao=A&aba={$_GET['aba']}&aba2=relatorio_acompanhamento_freq");
			}
		} else if (in_array ( PFL_COORDENADOR_ESTADUAL, $perfis ) || in_array ( PFL_COORDENADOR_MUNICIPAL, $perfis )) {
			$menu [] = array (
					"id" => 1,
					"descricao" => "Cadastro de Estudantes",
					"link" => "/projovemurbano/projovemurbano.php?modulo=principal/monitoramento2014&acao=A&aba={$_GET['aba']}&aba2=cadastroEstudantes" 
			);
			if ($ordem!='') {
				$menu[] = array("id" => 5, "descricao" => "Agência", "link" => "/projovemurbano/projovemurbano.php?modulo=principal/monitoramento2014&aba={$_GET['aba']}&aba2=A&aba=agencias");
				$menu[] = array("id" => 6, "descricao" => "Encaminhar Lista", "link" => "/projovemurbano/projovemurbano.php?modulo=principal/monitoramento2014&acao=A&aba={$_GET['aba']}&aba2=encaminharLista");
				$menu[] = array("id" => 8, "descricao" => "Acompanhamento de Frequência e Notas", "link" => "/projovemurbano/projovemurbano.php?modulo=principal/monitoramento2014&acao=A&aba={$_GET['aba']}&aba2=relatorio_acompanhamento_freq");
			}
			$menu[] = array("id" => 7, "descricao" => "Lançamento de Notas","link" => "/projovemurbano/projovemurbano.php?modulo=principal/monitoramento2014&acao=A&aba={$_GET['aba']}&aba2=lancamentoNotas");
		}
	} 
	return $menu;
}
function carregarMunicipios2($dados) {
	global $db;
	if ($dados ['estuf'])
		$sql = "SELECT muncod as codigo, mundescricao as descricao FROM territorios.municipio WHERE estuf='" . $dados ['estuf'] . "' ORDER BY mundescricao";
	else
		$sql = array ();
	$funcao = $dados ['funcao'] != '' ? $dados ['funcao'] : 'carregarPolo';
	if ($funcao == '{semfuncao}') {
		$funcao = '';
	}
	$db->monta_combo ( 'muncod', $sql, 'S', 'Selecione', $funcao, '', '', '', 'N', 'muncod' );
}
function carregarMunicipios($dados) {
	global $db;
	if ($dados ['estuf']) {
		if ($dados ['muncod']) {
			$muncod = "AND muncod = '{$dados['muncod']}'";
		}
		$sql = "SELECT muncod as codigo, mundescricao as descricao FROM territorios.municipio WHERE estuf='" . $dados ['estuf'] . "' $muncod ORDER BY mundescricao";
	} else {
		$sql = array ();
	}
	$dados ['bloq'] = $dados ['bloq'] ? $dados ['bloq'] : 'S';
	if ($dados ['nat']) {
		$db->monta_combo ( 'caemuncodnaturalidade', $sql, $dados ['bloq'], 'Selecione o Munícipio', '', '', '', '', 'S', 'caemuncodnaturalidade', null, null, null, 'required' );
	} else {
		// $db->monta_combo ( 'endmuncod', $sql, $dados ['bloq'], 'Selecione', '', '', '', '', 'S', 'endmuncod' );
		$municipio = $db->pegaLinha ( $sql );
		echo $municipio ['descricao'];
	}
}
function testaPolo($post) {
	global $db;
	
	if ($post ['estuf'] != '') {
		$filtro = "AND estuf = '" . $post ['estuf'] . "'";
	} else {
		$filtro = "AND muncod = '" . $post ['muncod'] . "'";
	}
	$sql = "SELECT
				'S'
			FROM
				projovemurbano.projovemurbano pju
			INNER JOIN projovemurbano.polomunicipio pmu ON pmu.pjuid = pju.pjuid
			WHERE
				pmupossuipolo IS TRUE 
				$filtro";
	echo $db->pegaUm ( $sql );
}
function carregarPolo($post) {
	global $db;
	
	if ($post ['estuf'] != '') {
		$filtro = "pju.estuf = '" . $post ['estuf'] . "'";
	} else {
		$filtro = "pju.muncod = '" . $post ['muncod'] . "'";
	}
	
	// Adaptação para o perfil Diretor do Pólo
	if (! $db->testa_superuser ()) {
		$perfis = pegaPerfilGeral ();
		
		if (in_array ( PFL_DIRETOR_POLO, $perfis )) {
			$inner_polo = "inner join projovemurbano.usuarioresponsabilidade ur on ur.usucpf='" . $_SESSION ['usucpf'] . "' and ur.polid=pol.polid AND rpustatus='A'";
			$inner_polo_filtro = "inner join projovemurbano.usuarioresponsabilidade ur on ur.usucpf='" . $_SESSION ['usucpf'] . "' and ur.polid=cae.polid AND rpustatus='A'";
		}
		
		if (in_array ( PFL_DIRETOR_NUCLEO, $perfis )) {
			$inner_nucleo = "inner join projovemurbano.associamucipiopolo asm on asm.polid = pol.polid 
						     inner join projovemurbano.nucleo nuc on nuc.munid = asm.munid
						     inner join projovemurbano.usuarioresponsabilidade ur on ur.usucpf='" . $_SESSION ['usucpf'] . "' and ur.nucid=nuc.nucid AND rpustatus='A'";
		}
	}
	$sql = "SELECT pol.polid as codigo, 'POLO '||pol.polid as descricao 
			FROM projovemurbano.polo pol 
			LEFT JOIN projovemurbano.polomunicipio pmu ON pmu.pmuid = pol.pmuid 
			LEFT JOIN projovemurbano.projovemurbano pju ON pju.pjuid = pmu.pjuid
			{$inner_polo} 
			{$inner_nucleo}
			WHERE $filtro AND pmustatus='A' AND polstatus='A' 
			ORDER BY pol.polid";
	// print_r($sql);die();
	$db->monta_combo ( 'polid', $sql, $bloq, 'Selecione', 'buscarNucleos', '', '', '', 'S', 'polid' );
}
function buscarNucleosSemId($post) {
	global $db;
	
	if ($post ['estuf'] != '') {
		$filtro = "pu.estuf = '" . $post ['estuf'] . "'";
		$ent = "estado";
	} else {
		$filtro = "pu.muncod = '" . $post ['muncod'] . "'";
		$ent = "município";
	}
	
	// Adaptação para o perfil Diretor do Núcleo
	if (! $db->testa_superuser ()) {
		$perfis = pegaPerfilGeral ();
		if (in_array ( PFL_DIRETOR_NUCLEO, $perfis )) {
			$inner_nucleo = "INNER JOIN projovemurbano.usuarioresponsabilidade ur ON ur.usucpf='" . $_SESSION ['usucpf'] . "' AND ur.nucid=nuc.nucid AND rpustatus='A'";
		}
	}
	// Alteração naSQL feita por Wallace 19/06/2012
	$sql = "SELECT DISTINCT
				nuc.nucid as codigo, 
				'NÚCLEO '||nuc.nucid||', SEDE: '||COALESCE((SELECT entnome 
												   FROM entidade.entidade ent 
												   INNER JOIN projovemurbano.nucleoescola nes ON nes.entid = ent.entid 
												   WHERE nes.nucid=nuc.nucid AND nes.nuetipo='S'),'NA')||COALESCE(', 
										 ANEXO:'||(SELECT entnome 
										 		   FROM entidade.entidade ent 
										 		   INNER JOIN projovemurbano.nucleoescola nes ON nes.entid = ent.entid 
										 		   WHERE nes.nucid=nuc.nucid AND nes.nuetipo='A'),'') as descricao
			FROM
				projovemurbano.projovemurbano pu
			INNER JOIN projovemurbano.polomunicipio pm ON pm.pjuid = pu.pjuid
			INNER JOIN projovemurbano.municipio mu ON mu.pmuid = pm.pmuid
			INNER JOIN projovemurbano.nucleo nuc ON nuc.munid = mu.munid 
			INNER JOIN projovemurbano.nucleoescola ne ON ne.nucid = nuc.nucid
			$inner_nucleo
			WHERE
				$filtro
				AND nucstatus = 'A'
				AND pu.pjustatus = 'A'
				AND mu.munstatus = 'A'
				AND ne.nuestatus = 'A'";
	
	// $sql = "SELECT
	// nuc.nucid as codigo,
	// 'NÚCLEO '||nuc.nucid||', SEDE: '||COALESCE((SELECT entnome
	// FROM entidade.entidade ent
	// INNER JOIN projovemurbano.nucleoescola nes ON nes.entid = ent.entid
	// WHERE nes.nucid=nuc.nucid AND nes.nuetipo='S'),'NA')||COALESCE(',
	// ANEXO:'||(SELECT entnome
	// FROM entidade.entidade ent
	// INNER JOIN projovemurbano.nucleoescola nes ON nes.entid = ent.entid
	// WHERE nes.nucid=nuc.nucid AND nes.nuetipo='A'),'') as descricao
	//
	// FROM projovemurbano.nucleo nuc
	// LEFT JOIN projovemurbano.municipio mun ON mun.munid = nuc.munid
	// LEFT JOIN projovemurbano.associamucipiopolo amp ON amp.munid = mun.munid
	// LEFT JOIN projovemurbano.polo pol ON pol.polid = amp.polid
	// LEFT JOIN projovemurbano.polomunicipio plm ON plm.pmuid = pol.pmuid
	// LEFT JOIN projovemurbano.projovemurbano pju ON pju.pjuid = plm.pjuid
	// INNER JOIN territorios.municipio tm ON tm.muncod = mun.muncod
	// {$inner_nucleo}
	// WHERE
	// nuc.nucstatus='A'
	// AND mun.munstatus='A'
	// --AND plm.pmustatus='A'
	// --AND pol.polstatus='A'
	// AND
	// $filtro";
	// FIM Alteração naSQL feita por Wallace 19/06/2012
	$nucleos = $db->carregar ( $sql );
	if ($nucleos [0] ['codigo'] == '') {
		echo "Não existe nucleo para este $ent.";
	} else {
		$db->monta_combo ( 'nucid', $nucleos, '', 'Selecione', 'buscarTurmas', '', '', '', 'S', 'nucid' );
	}
}
function montaMenuPlanoImplementacao() {
	global $db;
	if ($_SESSION ['projovemurbano'] ['ppuid'] != 3) {
		$menu [] = array (
				"id" => 1,
				"descricao" => "Coordenador Responsável",
				"link" => "/projovemurbano/projovemurbano.php?modulo=principal/planoImplementacao&acao=A&aba=coordenadorResponsavel" 
		);
	}
	$menu [] = array (
			"id" => 2,
			"descricao" => "Meta, Matrícula e Início de aula",
			"link" => "/projovemurbano/projovemurbano.php?modulo=principal/planoImplementacao&acao=A&aba=metaMatriculaInicioAula" 
	);
	$menu [] = array (
			"id" => 3,
			"descricao" => "Pólo/Núcleo",
			"link" => "/projovemurbano/projovemurbano.php?modulo=principal/planoImplementacao&acao=A&aba=poloNucleo" 
	);
	$menu [] = array (
			"id" => 4,
			"descricao" => "Profissionais",
			"link" => "/projovemurbano/projovemurbano.php?modulo=principal/planoImplementacao&acao=A&aba=profissionais" 
	);
	$menu [] = array (
			"id" => 5,
			"descricao" => "Formação de Educadores",
			"link" => "/projovemurbano/projovemurbano.php?modulo=principal/planoImplementacao&acao=A&aba=formacaoEducadores" 
	);
	$menu [] = array (
			"id" => 6,
			"descricao" => "Gêneros Alimenticios",
			"link" => "/projovemurbano/projovemurbano.php?modulo=principal/planoImplementacao&acao=A&aba=generoAlimenticios" 
	);
	if ($_SESSION ['projovemurbano'] ['estuf'])
		$ab = "qualificacaoProfissionalEstado";
	if ($_SESSION ['projovemurbano'] ['muncod'])
		$ab = "qualificacaoProfissionalMunicipio";
	$menu [] = array (
			"id" => 7,
			"descricao" => "Qualificação Profissional",
			"link" => "/projovemurbano/projovemurbano.php?modulo=principal/planoImplementacao&acao=A&aba=" . $ab 
	);
	if (! $_SESSION ['projovemurbano'] ['muncod']) {
		$menu [] = array (
				"id" => 11,
				"descricao" => "Transporte Mat. Didático",
				"link" => "/projovemurbano/projovemurbano.php?modulo=principal/planoImplementacao&acao=A&aba=transporteDidatico" 
		);
	}
	// $menu [] = array (
	// "id" => 12,
	// "descricao" => "Endereço Entrega Mat. Didático",
	// "link" => "/projovemurbano/projovemurbano.php?modulo=principal/planoImplementacao&acao=A&aba=enderecoEntrega"
	// );
	$menu [] = array (
			"id" => 8,
			"descricao" => "Demais Ações",
			"link" => "/projovemurbano/projovemurbano.php?modulo=principal/planoImplementacao&acao=A&aba=demaisAcoes" 
	);
	if ($_SESSION ['projovemurbano'] ['ppuid'] != 3) {
		$menu [] = array (
				"id" => 9,
				"descricao" => "Resumo Financeiro",
				"link" => "/projovemurbano/projovemurbano.php?modulo=principal/planoImplementacao&acao=A&aba=resumoFinanceiro" 
		);
	}
	$menu [] = array (
			"id" => 10,
			"descricao" => "Repasse de Recurso",
			"link" => "/projovemurbano/projovemurbano.php?modulo=principal/planoImplementacao&acao=A&aba=repasseRecurso" 
	);
	$menu [] = array (
			"id" => 10,
			"descricao" => "Visualizar Plano",
			"link" => "/projovemurbano/projovemurbano.php?modulo=principal/planoImplementacao&acao=A&aba=visualizarPlano" 
	);
	return $menu;
}
function montaMenuPoloNucleo() {
	global $db;
	
	$menu [] = array (
			"id" => 1,
			"descricao" => "Pólo/Núcleo",
			"link" => "/projovemurbano/projovemurbano.php?modulo=principal/planoImplementacao&acao=A&aba=poloNucleo&aba2=poloNucleoCadastro" 
	);
	if ($_REQUEST ['aba2'] == "poloNucleoGerenciar")
		$menu [] = array (
				"id" => 2,
				"descricao" => "Pólo/Núcleo - Gerenciar",
				"link" => "/projovemurbano/projovemurbano.php?modulo=principal/planoImplementacao&acao=A&aba=poloNucleo&aba2=poloNucleoGerenciar" 
		);
	$menu [] = array (
			"id" => 3,
			"descricao" => "Pólo/Núcleo - Resumo",
			"link" => "/projovemurbano/projovemurbano.php?modulo=principal/planoImplementacao&acao=A&aba=poloNucleo&aba2=poloNucleoResumo" 
	);
	
	return $menu;
}
function inserirCoordenadorResponsavel($dados) {
	global $db;
	$sql = "INSERT INTO projovemurbano.coordenadorresponsavel(
            pjuid, corcpf, cornome, corsecretario, corstatus)
    		VALUES ('" . $_SESSION ['projovemurbano'] ['pjuid'] . "', 
    				'" . str_replace ( array (
			".",
			"-" 
	), array (
			"",
			"" 
	), $dados ['corcpf'] ) . "', 
    				'" . $dados ['cornome'] . "', 
    				" . (($dados ['corsecretario'] == "sim") ? "TRUE" : "FALSE") . ", 
    				'A');";
	
	$db->executar ( $sql );
	
	$db->commit ();
	
	echo "<script>
			alert('Dados salvos com sucesso');
			window.location='projovemurbano.php?modulo=principal/planoImplementacao&acao=A&aba=coordenadorResponsavel';
		  </script>";
}
function atualizarCoordenadorResponsavel($dados) {
	global $db;
	
	$sql = "UPDATE projovemurbano.coordenadorresponsavel
   			SET corcpf='" . str_replace ( array (
			".",
			"-" 
	), array (
			"",
			"" 
	), $dados ['corcpf'] ) . "', 
   				cornome='" . $dados ['cornome'] . "', 
   				corsecretario=" . (($dados ['corsecretario'] == "sim") ? "TRUE" : "FALSE") . " 
 			WHERE pjuid='" . $_SESSION ['projovemurbano'] ['pjuid'] . "';";
	
	$db->executar ( $sql );
	
	$db->commit ();
	
	echo "<script>
			alert('Coordenador gravado com sucesso');
			window.location='projovemurbano.php?modulo=principal/planoImplementacao&acao=A&aba=coordenadorResponsavel';
		  </script>";
}
function pegarUsuarioProJovem() {
	global $db;
	
	if ($_SESSION ['projovemurbano'] ['ppuid']) {
		$sql = "
            SELECT * FROM seguranca.usuario u 
            LEFT JOIN projovemurbano.identificacaosecretario i ON i.isecpf = u.usucpf   
            WHERE i.pjuid='" . $_SESSION ['projovemurbano'] ['pjuid'] . "' AND i.ppuid = {$_SESSION['projovemurbano']['ppuid']}
        ";
		return $db->pegaLinha ( $sql );
	}
	return array ();
}
function pegarUsuarioMaterial() {
	global $db;
	
	if ($_SESSION ['projovemurbano'] ['ppuid']) {
		$sql = "
            SELECT * FROM seguranca.usuario u
            INNER JOIN projovemurbano.enderecoentregadematerial i ON i.eemcpfresponsavel = u.usucpf
            WHERE i.pjuid='" . $_SESSION ['projovemurbano'] ['pjuid'] . "' 
		";
		return $db->pegaLinha ( $sql );
	}
	return array ();
}
function carregarProJovemUrbano() {
	global $db;
	
	if ($_SESSION ['projovemurbano'] ['estuf'] && $_SESSION ['projovemurbano'] ['ppuid']) {
		$sql = "SELECT pjuid FROM projovemurbano.projovemurbano WHERE estuf='" . $_SESSION ['projovemurbano'] ['estuf'] . "' and ppuid = {$_SESSION['projovemurbano']['ppuid']} AND pjustatus = 'A'";
		$pjuid = $db->pegaUm ( $sql );
		if ($pjuid) {
			$_SESSION ['projovemurbano'] ['pjuid'] = $pjuid;
		} else {
			// pegando a secretaria de educação estadual
			$sql = "
                SELECT  en.entid 
                FROM entidade.entidade en 
                INNER JOIN entidade.funcaoentidade fe ON fe.entid = en.entid 
                INNER JOIN entidade.endereco ed ON ed.entid = en.entid  
                WHERE ed.estuf='" . $_SESSION ['projovemurbano'] ['estuf'] . "' AND fe.funid=6 AND fe.fuestatus='A' AND en.entstatus='A' AND ed.tpeid=1
            ";
			$entid = $db->pegaUm ( $sql );
			
			$sql = "INSERT INTO projovemurbano.projovemurbano( entid, pjustatus, estuf, ppuid )
                        VALUES (" . (($entid) ? "'" . $entid . "'" : "NULL") . ", 'A', '" . $_SESSION ['projovemurbano'] ['estuf'] . "', {$_SESSION['projovemurbano']['ppuid']}) RETURNING pjuid;
            ";
			$pjuid = $db->pegaUm ( $sql );
			$db->commit ();
			$_SESSION ['projovemurbano'] ['pjuid'] = $pjuid;
		}
	} else {
		if ($_SESSION ['projovemurbano'] ['muncod'] && $_SESSION ['projovemurbano'] ['ppuid']) {
			$sql = "SELECT pjuid FROM projovemurbano.projovemurbano WHERE muncod='" . $_SESSION ['projovemurbano'] ['muncod'] . "' and ppuid = {$_SESSION['projovemurbano']['ppuid']} AND pjustatus = 'A'";
			$pjuid = $db->pegaUm ( $sql );
			if ($pjuid) {
				$_SESSION ['projovemurbano'] ['pjuid'] = $pjuid;
			} else {
				// pegando a secretaria de educação estadual
				$sql = "
                    SELECT  en.entid 
                    FROM entidade.entidade en 
                    INNER JOIN entidade.funcaoentidade fe ON fe.entid = en.entid 
                    INNER JOIN entidade.endereco ed ON ed.entid = en.entid  
                    WHERE ed.muncod='" . $_SESSION ['projovemurbano'] ['muncod'] . "' AND fe.funid=7 AND fe.fuestatus='A' AND en.entstatus='A' AND ed.tpeid=1
                ";
				$entid = $db->pegaUm ( $sql );
				$sql = "INSERT INTO projovemurbano.projovemurbano(entid, pjustatus, muncod, ppuid)
                            VALUES (" . (($entid) ? "'" . $entid . "'" : "NULL") . ", 'A', '" . $_SESSION ['projovemurbano'] ['muncod'] . "', {$_SESSION['projovemurbano']['ppuid']}) RETURNING pjuid;
                ";
				$pjuid = $db->pegaUm ( $sql );
				$db->commit ();
				$_SESSION ['projovemurbano'] ['pjuid'] = $pjuid;
			}
		}
	}
}
function inserirIdentificacao($dados) {
	global $db;
	
	$perfis = pegaPerfilGeral ();
	
	$isecep = str_replace ( array (
			"-" 
	), array (
			"" 
	), $dados ['isecep'] );
	$isenumero = $dados ['isenumero'];
	$iseendereco = $dados ['iseendereco'];
	$isecomplemento = (($dados ['isecomplemento']) ? "'" . $dados ['isecomplemento'] . "'" : "NULL");
	$isebairro = $dados ['isebairro'];
	$iseuf = $dados ['iseuf'];
	$isemunicipio = $dados ['isemunicipio'];
	$isetelefone = $dados ['isetelefoneddd'] . str_replace ( array (
			"-" 
	), array (
			"" 
	), $dados ['isetelefone'] );
	$isecelular = $dados ['isecelularddd'] . str_replace ( array (
			"-" 
	), array (
			"" 
	), $dados ['isecelular'] );
	$isecpf = $dados ['isecpf'];
	$iseregistrogeral = $dados ['iseregistrogeral'];
	$iseorgaoexpedidor = $dados ['iseorgaoexpedidor'];
	$ppuid = $_SESSION ['projovemurbano'] ['ppuid'];
	
	if ($_SESSION ['projovemurbano'] ['pjuid']) {
		$sqlVerificaCodigo = "SELECT pjuid FROM projovemurbano.identificacaosecretario WHERE pjuid = " . $_SESSION ['projovemurbano'] ['pjuid'];
		$pjuid = $db->pegaUm ( $sqlVerificaCodigo );
	}
	if ($pjuid) {
		if ($dados ['isecomplemento']) {
			$complemento = "isecomplemento = " . (($dados ['isecomplemento']) ? "'" . $dados ['isecomplemento'] . "'" : "NULL") . ",";
		}
		if ($dados ['isemunicipio']) {
			$municipio = "isemunicipio = '" . $dados ['isemunicipio'] . "',";
		}
		if ($dados ['iseuf']) {
			$uf = "iseuf = '" . $dados ['iseuf'] . "',";
		}
		$sqlUpdate = "UPDATE projovemurbano.identificacaosecretario SET 
                        pjuid = '" . $_SESSION ['projovemurbano'] ['pjuid'] . "', 
                        isecep = '" . str_replace ( array (
				"-" 
		), array (
				"" 
		), $dados ['isecep'] ) . "', 
                        iseendereco = '" . $dados ['iseendereco'] . "', 
                        isenumero = '" . $dados ['isenumero'] . "',
                        $complemento 
            		isebairro = '" . $dados ['isebairro'] . "', 
            		$uf 
            		$municipio
            		isetelefone = '" . $dados ['isetelefoneddd'] . str_replace ( array (
				"-" 
		), array (
				"" 
		), $dados ['isetelefone'] ) . "', 
            		isecelular = '" . $dados ['isecelularddd'] . str_replace ( array (
				"-" 
		), array (
				"" 
		), $dados ['isecelular'] ) . "',  
            		isecpf = '" . $dados ['isecpf'] . "', 
            		iserg = '" . $dados ['iseregistrogeral'] . "', 
            		iseorgexp = '" . $dados ['iseorgaoexpedidor'] . "', 
            		isestatus = 'A', 
            		ppuid = " . $_SESSION ['projovemurbano'] ['ppuid'] . "
            		WHERE pjuid = '" . $_SESSION ['projovemurbano'] ['pjuid'] . "'";
		$db->executar ( $sqlUpdate );
		$db->commit ();
	} else {
		$sql = "
            INSERT INTO projovemurbano.identificacaosecretario(
                    pjuid, isecep, iseendereco, isenumero, isecomplemento, isebairro, iseuf, isemunicipio, isetelefone, isecelular,  
                    isecpf, iserg, iseorgexp, isestatus, ppuid)
                VALUES (
                    '" . $_SESSION ['projovemurbano'] ['pjuid'] . "', 
                    '" . $isecep . "', 
                    '" . $iseendereco . "', 
                    '" . $isenumero . "', 
                     " . $isecomplemento . ", 
                    '" . $isebairro . "', 
                    '" . $iseuf . "', 
                    '" . $isemunicipio . "', 
                    '" . $isetelefone . "', 
                    '" . $isecelular . "',  
                    '" . $isecpf . "', 
                    '" . $iseregistrogeral . "', 
                    '" . $iseorgaoexpedidor . "', 'A',
                    " . $ppuid . ");
        ";
		$db->executar ( $sql );
		$db->commit ();
	}
	
	if ($_SESSION ['projovemurbano'] ['muncod']) {
		$cmecodibge = $_SESSION ['projovemurbano'] ['muncod'];
	} else {
		$cmecodibge = $db->pegaUm ( "select estcod from territorios.estado where estuf = '{$_SESSION['projovemurbano']['estuf']}'" );
	}
	
	if ($_SESSION ['projovemurbano'] ['ppuid'] == '2' || $_SESSION ['projovemurbano'] ['ppuid'] == '3') {
		
		$sql = "select * from projovemurbano.cargameta where cmecodibge = '{$cmecodibge}' and ppuid = {$_SESSION['projovemurbano']['ppuid']}";
		
		$rsCargaMeta = $db->pegaLinha ( $sql );
		$sql = 'delete from projovemurbano.metasdoprograma where cmeid = ' . $rsCargaMeta ['cmeid'] . ' and tpmid in (7,10,13);';
		
		$rsCargaMeta ['juventude'] = $rsCargaMeta ['juventude'] ? $rsCargaMeta ['juventude'] : '0';
		$rsCargaMeta ['prisional'] = $rsCargaMeta ['prisional'] ? $rsCargaMeta ['prisional'] : '0';
		$rsCargaMeta ['geral'] = $rsCargaMeta ['geral'] ? $rsCargaMeta ['geral'] : '0';
		
		if ($_SESSION ['projovemurbano'] ['estuf']) {
			
			$sql .= "INSERT INTO projovemurbano.metasdoprograma(tpmid, pjuid, ppuid, suaid, cmeid, mtpvalor)
		    		VALUES (7, {$_SESSION['projovemurbano']['pjuid']}, {$_SESSION['projovemurbano']['ppuid']}, null, {$rsCargaMeta['cmeid']}, '{$rsCargaMeta['juventude']}');";
			$sql .= "INSERT INTO projovemurbano.metasdoprograma(tpmid, pjuid, ppuid, suaid, cmeid, mtpvalor)
		   			VALUES (10, {$_SESSION['projovemurbano']['pjuid']}, {$_SESSION['projovemurbano']['ppuid']}, null, {$rsCargaMeta['cmeid']}, '{$rsCargaMeta['prisional']}');";
			$sql .= "INSERT INTO projovemurbano.metasdoprograma(tpmid, pjuid, ppuid, suaid, cmeid, mtpvalor)
		   			VALUES (13, {$_SESSION['projovemurbano']['pjuid']}, {$_SESSION['projovemurbano']['ppuid']}, null, {$rsCargaMeta['cmeid']}, '{$rsCargaMeta['geral']}');";
			$docid = criaDocumento ();
			$sql .= "INSERT INTO projovemurbano.pj_programa_workflow(pjuid, tprid, docid)
    				VALUES ({$_SESSION['projovemurbano']['pjuid']}, 1, $docid);";
			$docid = criaDocumento ();
			$sql .= "INSERT INTO projovemurbano.pj_programa_workflow(pjuid, tprid, docid)
		   			VALUES ({$_SESSION['projovemurbano']['pjuid']}, 2, $docid);";
			$docid = criaDocumento ();
			$sql .= "INSERT INTO projovemurbano.pj_programa_workflow(pjuid, tprid, docid)
		   			VALUES ({$_SESSION['projovemurbano']['pjuid']}, 3, $docid);";
		} else {
			
			$docid = criaDocumento ();
			if ($rsCargaMeta ['juventude'] > 0) {
				$sql .= "INSERT INTO projovemurbano.metasdoprograma(tpmid, pjuid, ppuid, suaid, cmeid, mtpvalor)
	    				VALUES (7, {$_SESSION['projovemurbano']['pjuid']}, {$_SESSION['projovemurbano']['ppuid']}, null, {$rsCargaMeta['cmeid']}, '{$rsCargaMeta['juventude']}');";
				$sql .= "INSERT INTO projovemurbano.pj_programa_workflow(pjuid, tprid, docid)
	    				VALUES ({$_SESSION['projovemurbano']['pjuid']}, 1, $docid);";
			} else {
				$sql .= "INSERT INTO projovemurbano.metasdoprograma(tpmid, pjuid, ppuid, suaid, cmeid, mtpvalor)
	    				VALUES (13, {$_SESSION['projovemurbano']['pjuid']}, {$_SESSION['projovemurbano']['ppuid']}, null, {$rsCargaMeta['cmeid']}, '{$rsCargaMeta['geral']}');";
				$sql .= "INSERT INTO projovemurbano.pj_programa_workflow(pjuid, tprid, docid)
	    				VALUES ({$_SESSION['projovemurbano']['pjuid']}, 3, $docid);";
			}
		}
		
		if ($sql) {
			$db->executar ( $sql );
			$db->commit ();
		}
	}
	
	$cargaMeta = $db->pegaUm ( "select cmemeta from projovemurbano.cargameta where cmecodibge = '{$cmecodibge}' and ppuid = {$_SESSION['projovemurbano']['ppuid']}" );
	
	if ($_SESSION ['projovemurbano'] ['estuf']) {
		if (in_array ( PFL_SECRETARIO_MUNICIPAL, $perfis ) || in_array ( PFL_SECRETARIO_ESTADUAL, $perfis )) {
			$urlRedirect = "projovemurbano.php?modulo=principal/identificacao&acao=A";
		} else {
			if ($_SESSION ['projovemurbano'] ['ppuid'] == '1' && ( int ) $cargaMeta > 0) {
				
				$urlRedirect = "projovemurbano.php?modulo=principal/termoAdesao&acao=A";
			}
			if ($_SESSION ['projovemurbano'] ['ppui'] == '2') {
				$urlRedirect = "projovemurbano.php?modulo=principal/termoAdesao&acao=A";
			} else {
				$urlRedirect = "projovemurbano.php?modulo=principal/identificacao&acao=A";
			}
		}
	} elseif ($_SESSION ['projovemurbano'] ['muncod']) {
		if (in_array ( PFL_SECRETARIO_MUNICIPAL, $perfis ) || in_array ( PFL_SECRETARIO_ESTADUAL, $perfis )) {
			$urlRedirect = "projovemurbano.php?modulo=principal/identificacao&acao=A";
		} else {
			if (($_SESSION ['projovemurbano'] ['ppuid'] == '1' || $_SESSION ['projovemurbano'] ['ppuid'] == '2' || $_SESSION ['projovemurbano'] ['ppuid'] == '3') && ( int ) $cargaMeta > 0) {
				$urlRedirect = "projovemurbano.php?modulo=principal/termoAdesao&acao=A";
			} else {
				// $urlRedirect = "projovemurbano.php?modulo=principal/metaAtendimentoMun&acao=A";
				$urlRedirect = "projovemurbano.php?modulo=principal/termoAdesao&acao=A";
			}
		}
	} else {
		$urlRedirect = "projovemurbano.php?modulo=principal/identificacao&acao=A";
	}
	
	echo "
        <script>
            alert('Identificação gravada com sucesso');
            window.location='{$urlRedirect}';
        </script>
    ";
}
function alterarCpfNovo($dados) {
	global $db;
	
	$cpf = str_replace ( array (
			".",
			"-" 
	), array (
			"",
			"" 
	), $dados ['novo_cpf'] );
	
	if ($dados ['novo_cpf']) {
		$sql = "SELECT usucpf FROM seguranca.usuario WHERE usucpf='" . $cpf . "'";
		$existe_us = $db->pegaUm ( $sql );
	} else {
		echo "CPF informado não esta cadastrado na base de dados do SIMEC.";
		exit ();
	}
	
	if ($existe_us) {
		$sql = "Select * From projovemurbano.identificacaosecretario Where isecpf='" . $existe_us . "' and pjuid='" . $_SESSION ['projovemurbano'] ['pjuid'] . "'";
		$existe_pro = $db->executar ( $sql );
	}
	if ($existe_pro ['isecpf'] && $existe_pro ['pjuid']) {
		$sql .= "UPDATE projovemurbano.identificacaosecretario SET isecpf='" . $existe_us . "' WHERE pjuid='" . $_SESSION ['projovemurbano'] ['pjuid'] . "'";
		$db->executar ( $sql );
		$db->commit ();
		echo "Usuário atualizado com sucesso.";
		exit ();
	} else {
		$sql = "UPDATE projovemurbano.identificacaosecretario SET isecpf='" . $cpf . "' WHERE pjuid='" . $_SESSION ['projovemurbano'] ['pjuid'] . "'";
		$result = $db->executar ( $sql );
		$db->commit ();
		echo "Dados Gravados com sucesso!";
		exit ();
	}
}
function atualizarIdentificacao($dados) {
	global $db;
	
	$sql = "UPDATE projovemurbano.identificacaosecretario
                    SET isecep='" . str_replace ( array (
			"-" 
	), array (
			"" 
	), $dados ['isecep'] ) . "', 
                        iseendereco='" . $dados ['iseendereco'] . "', 
                        isenumero='" . $dados ['isenumero'] . "', 
                        isecomplemento=" . (($dados ['isecomplemento']) ? "'" . $dados ['isecomplemento'] . "'" : "NULL") . ", 
                        isebairro='" . $dados ['isebairro'] . "', 
                        iseuf='" . $dados ['iseuf'] . "', 
                        isemunicipio='" . $dados ['isemunicipio'] . "', 
                        isetelefone='" . $dados ['isetelefoneddd'] . str_replace ( array (
			"-" 
	), array (
			"" 
	), $dados ['isetelefone'] ) . "', 
                        isecelular='" . $dados ['isecelularddd'] . str_replace ( array (
			"-" 
	), array (
			"" 
	), $dados ['isecelular'] ) . "', 
                        iserg='" . $dados ['iseregistrogeral'] . "', 
                        iseorgexp='" . $dados ['iseorgaoexpedidor'] . "'
                  WHERE isecpf='" . $dados ['isecpf'] . "';
        ";
	$db->executar ( $sql );
	$db->commit ();
	
	if ($_SESSION ['projovemurbano'] ['muncod']) {
		$cmecodibge = $_SESSION ['projovemurbano'] ['muncod'];
	} else {
		$cmecodibge = $db->pegaUm ( "select estcod from territorios.estado where estuf = '{$_SESSION['projovemurbano']['estuf']}'" );
		
		$sql = "select * from projovemurbano.cargameta where cmecodibge = '{$cmecodibge}' and ppuid = {$_SESSION['projovemurbano']['ppuid']}";
		
		$rsCargaMeta = $db->pegaLinha ( $sql );
		
		$rsCargaMeta ['juventude'] = $rsCargaMeta ['juventude'] ? $rsCargaMeta ['juventude'] : '0';
		$rsCargaMeta ['prisional'] = $rsCargaMeta ['prisional'] ? $rsCargaMeta ['prisional'] : '0';
		$rsCargaMeta ['geral'] = $rsCargaMeta ['geral'] ? $rsCargaMeta ['geral'] : '0';
		
		$sql = 'delete from projovemurbano.metasdoprograma where cmeid = ' . $rsCargaMeta ['cmeid'] . ' and tpmid in (7,10,13);';
		
		$sql .= "INSERT INTO projovemurbano.metasdoprograma(tpmid, ppuid, suaid, cmeid, mtpvalor,pjuid)
				VALUES (7, {$_SESSION['projovemurbano']['ppuid']}, null, {$rsCargaMeta['cmeid']}, '{$rsCargaMeta['juventude']}', {$_SESSION['projovemurbano']['pjuid']});";
		$sql .= "INSERT INTO projovemurbano.metasdoprograma(tpmid, ppuid, suaid, cmeid, mtpvalor,pjuid)
				VALUES (10, {$_SESSION['projovemurbano']['ppuid']}, null, {$rsCargaMeta['cmeid']}, '{$rsCargaMeta['prisional']}', {$_SESSION['projovemurbano']['pjuid']});";
		$sql .= "INSERT INTO projovemurbano.metasdoprograma(tpmid, ppuid, suaid, cmeid, mtpvalor,pjuid)
				VALUES (13, {$_SESSION['projovemurbano']['ppuid']}, null, {$rsCargaMeta['cmeid']}, '{$rsCargaMeta['geral']}', {$_SESSION['projovemurbano']['pjuid']});";
		// ver($sql);
		if ($sql) {
			$db->executar ( $sql );
			$db->commit ();
		}
	}
	
	echo "
            <script>
                alert('Identificação gravada com sucesso');
                window.location='projovemurbano.php?modulo=principal/identificacao&acao=A';
            </script>
        ";
}
function aceitarTermo($dados) {
	global $db;
	
	$perfis = pegaPerfilGeral ();
	$sql = "INSERT INTO projovemurbano.sugestaoampliacao (pjuid, suaverdade, suametasugerida, suastatus, suametaajustada, ppuid)
					    VALUES ({$_SESSION['projovemurbano']['pjuid']}, FALSE, null, 'A', null, {$_SESSION['projovemurbano']['ppuid']});
					    ";
	$sql .= "UPDATE projovemurbano.projovemurbano SET adesaotermodata=NOW(), adesaotermo=TRUE WHERE pjuid='" . $_SESSION ['projovemurbano'] ['pjuid'] . "'";
	$db->executar ( $sql );
	$db->commit ();
	
	if (in_array ( PFL_SECRETARIO_MUNICIPAL, $perfis ) || in_array ( PFL_SECRETARIO_ESTADUAL, $perfis )) {
		$url = "projovemurbano.php?modulo=principal/sugestaoAmpliacao&acao=A";
	} else {
		$url = "projovemurbano.php?modulo=principal/sugestaoAmpliacao&acao=A";
	}
	echo "
            <script>
            alert('Termo foi aceito com sucesso');
            window.location='" . $url . "';
            </script>
        ";
}
function naoAceitarTermo() {
	global $db;
	$sql = "UPDATE projovemurbano.projovemurbano SET adesaotermo=FALSE WHERE pjuid='" . $_SESSION ['projovemurbano'] ['pjuid'] . "'";
	$db->executar ( $sql );
	$db->commit ();
	
	echo "<script>
			alert('Termo não foi aceito com sucesso');
			window.location='projovemurbano.php?modulo=principal/" . (($_SESSION ['projovemurbano'] ['estuf']) ? "listaEstados" : "") . (($_SESSION ['projovemurbano'] ['muncod']) ? "listaMunicipios" : "") . "&acao=A';
		  </script>";
}
function inserirSugestaoAmpliacao($dados) {
	global $db;
	// ver($dados,d);
	// die;
	$pjuid = $_SESSION ['projovemurbano'] ['pjuid'];
	$suaverdade = $dados ['suaverdade'] == "sim" ? "TRUE" : "FALSE";
	$ppuid = $_SESSION ['projovemurbano'] ['ppuid'];
	
	if ($dados ['suaverdade'] == "sim") {
		$suametasugerida = $dados ['suametasugerida'];
	} elseif ($dados ['suaverdade'] == "nao") {
		$suametasugerida = '0';
	} else {
		$suametasugerida = '0';
	}
	
	$sql_sug = "
        INSERT INTO projovemurbano.sugestaoampliacao(
                    pjuid, suaverdade, suametasugerida, suastatus, ppuid)
               VALUES( '" . $pjuid . "', " . $suaverdade . ", " . $suametasugerida . ", 'A', " . $ppuid . " ) returning suaid;
    ";
	$suaid = $db->pegaUm ( $sql_sug );
	$db->commit ();
	
	if ($suaverdade && $suametasugerida != '0' && $suaid > 0) {
		$sql = 'delete from projovemurbano.metasdoprograma where suaid = ' . $suaid . ' and tpmid in (8, 14);';
		
		if ($_REQUEST ['metaDestinada_sugerida'] == 'J') {
			$sql .= "
                Insert into projovemurbano.metasdoprograma (tpmid, pjuid, ppuid, suaid, cmeid, mtpvalor) values (8, $pjuid, {$_SESSION['projovemurbano']['ppuid']}, {$suaid}, null, " . $suametasugerida . ");
                Insert into projovemurbano.metasdoprograma (tpmid, pjuid, ppuid, suaid, cmeid, mtpvalor) values (14, $pjuid, {$_SESSION['projovemurbano']['ppuid']}, {$suaid}, null, 0);                
                    
            ";
		} elseif ($_REQUEST ['metaDestinada_sugerida'] == 'P') {
			$sql .= "
                Insert into projovemurbano.metasdoprograma (tpmid, pjuid, ppuid, suaid, cmeid, mtpvalor) values (8, $pjuid, {$_SESSION['projovemurbano']['ppuid']}, {$suaid}, null, 0);
                Insert into projovemurbano.metasdoprograma (tpmid, pjuid, ppuid, suaid, cmeid, mtpvalor) values (14, $pjuid, {$_SESSION['projovemurbano']['ppuid']}, {$suaid}, null, " . $suametasugerida . ");
            ";
		}
		$metasPrograma = $db->executar ( $sql );
	} else {
		$msg = "Processo Concluído com sucesso";
	}
	
	if ($metasPrograma) {
		if ($suaid > 0) {
			$msg = "Dados Gravado com sucesso";
			$db->commit ();
		} else {
			$msg = "Ocorreu algum problema com a gravação dos dados, tente novamente mais tarde ou entre em contado com o administrador do sistema";
		}
	}
	
	if ($dados ['suaverdade'] == "sim") {
		$end = "projovemurbano.php?modulo=principal/sugestaoAmpliacao&acao=A";
	} elseif ($dados ['suaverdade'] == "nao") {
		$end = "projovemurbano.php?modulo=principal/sugestaoAmpliacao&acao=A";
	}
	
	echo "
        <script>
            alert('{$msg}');
            window.location='{$end}';
		</script>
    ";
}
function atualizarSugestaoAmpliacao($dados) {
	global $db;
	$perfil = pegaPerfilGeral ();
	
	$suaid = $dados ['suaid'];
	$suaverdade = $dados ['suaverdade'] == "sim" ? "TRUE" : "FALSE";
	$suametasugerida = $dados ['suaverdade'] == "sim" ? "'" . $dados ['suametasugerida'] . "'" : "NULL";
	$suametaajustada = $dados ['suametaajustada'];
	$metaDestinada_sugerida = $dados ['metaDestinada_sugerida'] ? $dados ['metaDestinada_sugerida'] : "";
	$metaDestinada_ajustada = $dados ['metaDestinada_ajustada'] ? $dados ['metaDestinada_ajustada'] : "";
	
	if ($dados ['suametaajustada'] && ($dados ['suaverdade'] == 'sim')) {
		$updtajus = " , suametaajustada = " . (($dados ['suaverdade'] == "sim") ? "'" . $dados ['suametaajustada'] . "'" : "NULL");
	}
	
	$sql = "
        UPDATE projovemurbano.sugestaoampliacao
                SET suaverdade = " . $suaverdade . ", 
                    suametasugerida=" . $suametasugerida . " 
                    {$updtajus}
        WHERE pjuid='" . $_SESSION ['projovemurbano'] ['pjuid'] . "';
    ";
	
	if (($suaverdade == 'TRUE') && $dados ['suaid'] != '') {
		
		$sql .= 'delete from projovemurbano.metasdoprograma where suaid = ' . $suaid . ' and tpmid in (8, 14, 9, 15);';
		
		if (in_array ( PFL_ADMINISTRADOR, $perfil ) || in_array ( PFL_EQUIPE_MEC, $perfil ) || $db->testa_superuser ()) {
			
			if ($_REQUEST ['metaDestinada_ajustada'] == 'J') {
				$sql .= "Insert into projovemurbano.metasdoprograma (tpmid, ppuid, pjuid, suaid, cmeid, mtpvalor) values (9, {$_SESSION['projovemurbano']['ppuid']}, {$_SESSION['projovemurbano']['pjuid']}, {$suaid}, null, " . $suametaajustada . ");";
			} elseif ($_REQUEST ['metaDestinada_ajustada'] == 'P') {
				$sql .= "Insert into projovemurbano.metasdoprograma (tpmid, ppuid, pjuid, suaid, cmeid, mtpvalor) values (15, {$_SESSION['projovemurbano']['ppuid']}, {$_SESSION['projovemurbano']['pjuid']}, {$suaid}, null, " . $suametaajustada . ");";
			}
		}
		if ($_REQUEST ['metaDestinada_sugerida'] == 'J') {
			
			$sql .= "Insert into projovemurbano.metasdoprograma (tpmid, ppuid, pjuid, suaid, cmeid, mtpvalor) values (8, {$_SESSION['projovemurbano']['ppuid']}, {$_SESSION['projovemurbano']['pjuid']}, {$suaid}, null, " . $suametasugerida . ");";
		} elseif ($_REQUEST ['metaDestinada_sugerida'] == 'P') {
			$sql .= "Insert into projovemurbano.metasdoprograma (tpmid, ppuid, pjuid, suaid, cmeid, mtpvalor) values (14, {$_SESSION['projovemurbano']['ppuid']}, {$_SESSION['projovemurbano']['pjuid']}, {$suaid}, null, " . $suametasugerida . ");";
		}
	}
	// else{
	// $sql .= 'delete from projovemurbano.metasdoprograma where suaid = ' . $suaid . ' and tpmid in (8, 14, 9, 15);';
	// }
	
	if ($db->executar ( $sql )) {
		$sql .= "UPDATE projovemurbano.projovemurbano SET adesaotermoajustado = FALSE WHERE pjuid='" . $_SESSION ['projovemurbano'] ['pjuid'] . "'";
		if ($db->executar ( $sql )) {
			$mensagem = "Dados Gravados com sucesso!";
		} else {
			$mensagem = "Ocorreu algum problema com a gravação dos dados, tente novamente mais tarde ou entre em contado com o administrador do sistema";
		}
	} else {
		$mensagem = "Ocorreu algum problema com a gravação dos dados, tente novamente mais tarde ou entre em contado com o administrador do sistema";
	}
	$db->commit ();
	
	if ($suaverdade == 'TRUE' && ($_REQUEST ['metaDestinada_ajustada'] == 'J')) {
		echo "
	        <script>
	            alert('{$mensagem}');
	            window.location='projovemurbano.php?modulo=principal/termoAdesaoAjustado&acao=A';
	        </script>
	    ";
	} else {
		echo "
		<script>
			alert('{$mensagem}');
			window.location='projovemurbano.php?modulo=principal/termoAdesao&acao=A';
		</script>
		";
	}
}
function mascaraglobal($value, $mask) {
	$casasdec = explode ( ",", $mask );
	// Se possui casas decimais
	if ($casasdec [1])
		$value = sprintf ( "%01." . strlen ( $casasdec [1] ) . "f", $value );
	
	$value = str_replace ( array (
			"." 
	), array (
			"" 
	), $value );
	if (strlen ( $mask ) > 0) {
		$masklen = - 1;
		$valuelen = - 1;
		while ( $masklen >= - strlen ( $mask ) ) {
			if (substr ( $mask, $masklen, 1 ) == "#") {
				$valueformatado = trim ( substr ( $value, $valuelen, 1 ) ) . $valueformatado;
				$valuelen --;
			} else {
				if (trim ( substr ( $value, $valuelen, 1 ) ) != "") {
					$valueformatado = trim ( substr ( $mask, $masklen, 1 ) ) . $valueformatado;
				}
			}
			$masklen --;
		}
	}
	return $valueformatado;
}
function termoEstado($dados) {
	global $db;
	
	$rsSecretaria = recuperaSecretariaPorUfMuncod ();
	
	$rsMetas = recuperaMetasPorUfMuncod ( $dados );
	
	$dadosT = $db->pegaLinha ( "SELECT * FROM territorios.estado e 
                              JOIN projovemurbano.cargameta c ON c.cmecodibge = e.estcod::numeric 
                              WHERE estuf='" . $_SESSION ['projovemurbano'] ['estuf'] . "'
                              		AND c.ppuid='" . $_SESSION ['projovemurbano'] ['ppuid'] . "'" );
	// ver($_SESSION['projovemurbano']['tpmid']);
	?>
<table class="tabela" cellSpacing="1" cellPadding="3" align="center">
	<tr>
		<td>
			<h3 style="text-align: center">MINISTÉRIO DA EDUCAÇÃO</h3> <BR />
			<h4 style="text-align: center">GABINETE DO MINISTRO</h4> <BR />
<?if($_SESSION ['projovemurbano'] ['ppuid'] == '1'){?>
			<h4 style="text-align: center">TERMO DE ADESÃO AO PROGRAMA NACIONAL
				DE INCLUSÃO DE JOVENS - PROJOVEM URBANO</h4> <BR />
<?}else{?>
			<h4 style="text-align: center">TERMO DE ADESÃO AO PROGRAMA NACIONAL
				DE INCLUSÃO DE JOVENS - PROJOVEM URBANO E / OU PROJOVEM CAMPO</h4> <BR />
<?}?>
<?

	if ($_SESSION ['projovemurbano'] ['ppuid'] == '1') {
		?>
				<p>
				O Estado de (do) <b><?= $dadosT['estdescricao'] ?></b>/Distrito
				Federal,doravante denominado Estado/Distrito Federal, representado
				por seu (sua) Secretário(a) de Educação, <b><?= $dados['usunome'] ?></b>,
				CPF nº <b><?= mascaraglobal($dados['usucpf'], "###.###.###-##") ?></b>,
				RG nº <b><?= $dados['iserg'] ?></b>, expedido por <b><?= $dados['iseorgexp'] ?></b>,
				devidamente estabelecido à <b> <?= $dados['iseendereco'] . ", nº " . $dados['isenumero'] . ", " . $dados['isebairro'] . ", " . $db->pegaUm("SELECT mundescricao FROM territorios.municipio WHERE muncod='" . $dados['isemunicipio'] . "'") . ", " . $dados['iseuf']. ", " ?></b><b>CEP <?= mascaraglobal($dados['isecep'], "#####-###")?> </b>,
				e o Ministério da Educação, representado pelo Ministro de Estado,
				resolvem firmar o presente Termo de Adesão ao Programa Nacional de
				Inclusão de Jovens – Projovem Urbano consideradas as seguintes
				condições:
			</p> <BR />
			<h5>
				<h3>Cláusula Primeira – Do Objeto</h3>
				<p>O presente termo tem por objeto a adesão do Município ao Programa
					Nacional de Inclusão de Jovens – Projovem Urbano, instituído nos
					termos da Lei nº 11.692 de 10 de junho de 2008 , regulamentado pelo
					Decreto nº 6.629 de 04 de novembro de 2008 e em conformidade com a
					Lei Nº 8.666, de 21 de junho de 1993.</p>

				<h3>Cláusula Segunda – O Município se compromete a:</h3>

				<p>1. Atingir a seguinte meta de atendimento de jovens para o
					Projovem Urbano, no período de 2012:</p>

				<table border=1 align=center width=30%>
					<tr>
						<td align="center"><b>Anos</b></td>
						<td align="center"><b>2012</b></td>
					</tr>
					<tr>
						<td align="center"><b>Meta</b></td>
                    <? if ($dados['ajustado']) : ?>
                        <? $suametaajustada = $db->pegaUm("SELECT suametaajustada FROM projovemurbano.sugestaoampliacao WHERE pjuid='" . $_SESSION['projovemurbano']['pjuid'] . "'"); ?>
                    <td align="center"><?= (($suametaajustada) ? $suametaajustada : "Não cadastrado") ?></td>
                    <? else : ?>
                    <td align="center"><?= (($dadosT['cmemeta']) ? $dadosT['cmemeta'] : "Não cadastrado") ?></td>
                    <? endif; ?>
                </tr>
				</table>
				<p>2. Cumprir as seguintes diretrizes abaixo:</p>
				<p>I - estabelecer como foco a aprendizagem, realizando todos os
					esforços necessários para garantir a certificação em Ensino
					Fundamental – EJA e em qualificação profissional como formação
					inicial dos jovens matriculados no Projovem Urbano;</p>
				<p>II - responsabilizar-se pela divulgação do ProJovem Urbano em
					nível local, inclusive quanto aos processos de matrícula a serem
					realizados pelo Município, mobilizando a comunidade e suas
					lideranças, os jovens, pais e responsáveis, bem como os meios
					políticos e administrativos;</p>
				<p>III - empreender esforços para viabilizar a expedição dos
					documentos necessários para a matrícula dos jovens a serem
					atendidos pelo Programa;</p>
				<p>IV - matricular os estudantes por meio de Sistema de Matrícula,
					Acompanhamento de Frequência e Certificação do Projovem Urbano que
					a Secretaria de Educação Continuada, Alfabetização, Diversidade e
					Inclusão - SECADI/MEC disponibiliza online, sendo esta a única
					forma de garantir a inclusão dos jovens no Programa, bem como ser
					responsável pela fidedignidade das informações lançadas no referido
					sistema;</p>
				<p>V - garantir o acesso e as condições de permanência das pessoas
					público-alvo da educação especial ao Programa, por meio da oferta
					do Atendimento Educacional Especializado e oferta de recursos e
					serviços de acessibilidade.</p>
				<p>VI - desenvolver o Projeto Pedagógico Integrado do Programa em
					suas três dimensões, garantindo sua execução conforme legislação do
					Projovem Urbano e orientações da Secretaria de Educação Continuada,
					Alfabetização, Diversidade e Inclusão - SECADI/MEC;</p>
				<p>VII - acompanhar cada beneficiário do ProJovem Urbano,
					individualmente, mediante registro mensal de freqüência, por meio
					do Sistema de Matrícula, Acompanhamento da Freqüência e
					Certificação do Projovem Urbano;</p>
				<p>VIII - prevenir e combater a evasão pelo acompanhamento
					individual das razões para a não-freqüência do educando e implantar
					medidas para superá-las;</p>
				<p>IX - garantir o funcionamento do Comitê Gestor do Programa no
					âmbito local, sob coordenação da Secretaria de Educação, composto
					pelo Conselho de Juventude, por órgãos de políticas de juventude,
					quando existir na localidade, bem como pelas demais secretarias e
					órgãos afins, observada a intersetorialidade necessária para a
					execução das ações previstas pelo Programa;</p>
				<p>X - articular-se com as redes estaduais de ensino visando
					garantir a continuidade de estudos para os jovens atendidos pelo
					Programa;</p>
				<p>XI - concordar integralmente com os termos da Resolução CD/FNDE
					nº 60/2011, publicada no Diário Oficial da União em 10 de novembro
					de 2011, que estabelece os critérios e as normas de transferência
					automática de recursos financeiros do ProJovem Urbano para a
					execução das ações do Programa;</p>
				<p>XII - autorizar o FNDE/MEC a estornar ou bloquear valores
					creditados indevidamente na conta corrente do Programa em favor do
					Município, mediante solicitação direta ao agente financeiro
					depositário dos recursos ou procedendo ao desconto nas parcelas
					subseqüentes;</p>
				<p>XIII - restituir ao FNDE/MEC, no prazo de dez dias úteis a contar
					do recebimento da notificação e na forma prevista nos §§ 17 a 20 do
					Art. 18 da referida Resolução, os valores creditados indevidamente
					ou objeto de eventual irregularidade constatada, quando inexistir
					saldo suficiente na conta corrente e não houver repasses futuros a
					serem efetuados;</p>
				<p>XIV - atualizar permanentemente junto à Secretaria de Educação
					Continuada, Alfabetização, Diversidade e Inclusão/MEC, as
					informações prestadas no Plano de Implementação do Programa, sob
					pena de suspensão de pagamento de parcelas subseqüentes até a
					regularização da atualização dessas informações.</p>

				<h3>Cláusula Terceira – Da Rescisão</h3>
				<p>O presente instrumento poderá ser rescindido a qualquer tempo, no
					interesse das partes, ou pelo não cumprimento das cláusulas e/ou
					condições, observado o disposto nos artigos 77 a 80 da Lei Nº
					8.666, de 21 de junho de 1993, independentemente de interpelação
					judicial ou extrajudicial ou daquelas dispostas nos artigos 86 a 88
					do mesmo Diploma Legal.</p>

				<h3>Cláusula Quarta – Da Publicação</h3>
				<p>Caberá à Secretaria de Educação Continuada, Alfabetização,
					Diversidade e Inclusão - SECADI/MEC proceder à publicação do
					presente Termo de Adesão no Diário Oficial da União – DOU, conforme
					estabelecido no Parágrafo Único, do art. 61, da Lei Nº 8.666, de 21
					de junho de 1993.</p>

				<h3>Cláusula Quinta – Do Foro</h3>
				<p>O foro competente para dirimir dúvidas ou litígios oriundos deste
					instrumento é o da Justiça Federal, Foro da cidade de Brasília/DF,
					Seção Judiciária do Distrito Federal.</p>
				<br> <br> <br>
                
                <? if ($dados['ajustado']) : ?>
                    <? $adesaotermoajustadadata = $db->pegaLinha("SELECT to_char(adesaotermoajustadodata,'dd') as dia, to_char(adesaotermoajustadodata,'mm') as mes, to_char(adesaotermoajustadodata,'YYYY') as ano FROM projovemurbano.projovemurbano WHERE pjuid=" . $_SESSION['projovemurbano']['pjuid']); ?>
                <p align=center>___________________________________, <?= (($adesaotermoajustadadata['dia']) ? $adesaotermoajustadadata['dia'] : date("d")) . " de " . $db->pegaUm("SELECT mesdsc FROM public.meses WHERE mescod='" . (($adesaotermoajustadadata['mes']) ? $adesaotermoajustadadata['mes'] : date("m")) . "'") . " de " . (($adesaotermoajustadadata['ano']) ? $adesaotermoajustadadata['ano'] : date("Y")) ?></p>
                <? else : ?>
                    <? $adesaotermodata = $db->pegaLinha("SELECT to_char(adesaotermodata,'dd') as dia, to_char(adesaotermodata,'mm') as mes, to_char(adesaotermodata,'YYYY') as ano FROM projovemurbano.projovemurbano WHERE pjuid=" . $_SESSION['projovemurbano']['pjuid']); ?>
                <p align=center>___________________________________, <?= (($adesaotermodata['dia']) ? $adesaotermodata['dia'] : date("d")) . " de " . $db->pegaUm("SELECT mesdsc FROM public.meses WHERE mescod='" . (($adesaotermodata['mes']) ? $adesaotermodata['mes'] : date("m")) . "'") . " de " . (($adesaotermodata['ano']) ? $adesaotermodata['ano'] : date("Y")) ?></p>
                <? endif; ?>
                
                <br> <br>
				<p align=center>___________________________________________________________________</p>
				<p align=center>
					<b>Secretário(a) Municipal de Educação</b>
				</p>  
<?php
	} elseif ($_SESSION ['projovemurbano'] ['ppuid'] == '2') {
		?>
				<p>
					O Estado/Município do <b><?= $dadosT['estdescricao'] ?></b>,doravante
					denominado Ente Federado, por meio da sua Secretaria de Educação,
					CNPJ: <b><?= mascaraglobal($rsSecretaria['entnumcpfcnpj'], "##.###.###/####-##") ?></b>
					representado por seu (sua) Secretário(a), <b><?= $dados['usunome'] ?></b>,
					CPF nº <b><?= mascaraglobal($dados['usucpf'], "###.###.###-##") ?></b>,
					RG nº <b><?= $dados['iserg'] ?></b>, expedido por <b><?= $dados['iseorgexp'] ?></b>,
					devidamente estabelecido à <b> <?= $dados['iseendereco'] . ", nº " . $dados['isenumero'] . ", " . $dados['isebairro'] . ", " . $db->pegaUm("SELECT mundescricao FROM territorios.municipio WHERE muncod='" . $dados['isemunicipio'] . "'") . ", " . $dados['iseuf']. ", " ?></b><b>CEP <?= mascaraglobal($dados['isecep'], "#####-###")?> </b>,
					e o Ministério da Educação, representado pelo Ministro de Estado,
					resolvem firmar o presente Termo de Adesão ao Programa Nacional de
					Inclusão de Jovens – Projovem Urbano, edição 2013, em conformidade,
					no que couber, com a Lei n.º 8.666, de 21 de junho de 1993, e a
					legislação correlata, consideradas as seguintes condições:
				</p>
				<BR />
				<h5>
					<strong>Cláusula Primeira – Do Objeto</strong>
				</h5>
				<br>
				<p>O presente termo tem por objeto a adesão do Ente Federado ao
					Programa Nacional de Inclusão de Jovens – Projovem Urbano ,
					instituído nos termos da Lei nº 11.692 de 10 de junho de 2008,
					regulamentado pelo Decreto nº 6.629 de 4 de novembro de 2008 e pelo
					Decreto nº 7.649 de 21 de dezembro de 2011.</p>
				<br>

				<h5>
					<strong>Cláusula Segunda – DAS OBRIGAÇÕES DOS ENTES FEDERADOS:</strong>
				</h5>
				<br> <br>
				<p>1. Os Entes Federados se comprometem a cumprir as seguintes
					diretrizes abaixo:</p>
				<br>

				<p>I -executar o Programa, por meio da sua Secretaria de Educação,
					que deverá coordenar o desenvolvimento das ações de implementação
					do Programa, garantindo a necessária articulação com a rede de
					ensino, conforme seus Projetos Pedagógicos Integrados, as
					orientações da Secretaria de Educação Continuada, Alfabetização,
					Diversidade e Inclusão – SECADI/MEC e de acordo com Resolução
					CD/FNDE Nº de 2013;</p>
				<p>II - executar os recursos orçamentários repassados pelo Governo
					Federal exclusivamente na implementação do Programa, gerindo-os com
					eficiência, eficácia e transparência, visando a efetividade das
					ações;</p>
				<p>III - estabelecer como foco a aprendizagem, realizando todos os
					esforços necessários para garantir a certificação em Ensino
					Fundamental – EJA e em qualificação profissional como formação
					inicial dos jovens matriculados no Programa;</p>
				<p>IV - responsabilizar-se pela divulgação do Programa em nível
					local, inclusive quanto aos processos de matrícula a serem
					realizados pelo Ente Federado, mobilizando a comunidade e suas
					lideranças, os jovens, pais e responsáveis, bem como os meios
					políticos e administrativos;</p>
				<p>V - empreender esforços para viabilizar a expedição dos
					documentos necessários para a matrícula dos jovens a serem
					atendidos pelo Programa;</p>
				<p>VI -matricular os estudantes por meio de Sistema de Matrícula,
					Acompanhamento de Frequência e Certificação do Projovem Urbano e
					Campo disponibilizado pela Secretaria de Educação Continuada,
					Alfabetização, Diversidade e Inclusão - SECADI/MEC, sendo esta a
					única forma de garantir a inclusão dos jovens no Programa, bem como
					ser responsável pela fidedignidade das informações lançadas no
					referido sistema;</p>
				<p>VII - garantir o acesso e as condições de permanência das pessoas
					público-alvo da educação especial ao Programa, por meio da oferta
					do atendimento educacional especializado e oferta de recursos e
					serviços de acessibilidade;</p>
				<p>VIII - desenvolver os Projetos Pedagógicos Integrados das duas
					modalidades do Programa em suas três dimensões, garantindo sua
					execução conforme legislação do Projovem Urbano e do Projovem Campo
					– Saberes da Terra e orientações da Secretaria de Educação
					Continuada, Alfabetização, Diversidade e Inclusão - SECADI/MEC;</p>
				<p>IX - acompanhar cada beneficiário individualmente, no caso do
					Projovem Urbano, mediante registro mensal de frequência e de
					entrega de trabalhos, e no caso do Projovem Campo – Saberes da
					Terra, mediante registro mensal de frequência por meio do Sistema
					de Matrícula, Acompanhamento da Frequência e Certificação do
					Projovem Urbano e Campo;</p>
				<p>X - prevenir e combater a evasão pelo acompanhamento individual
					das razões para a não frequência do educando e implantar medidas
					para superá-las;</p>
				<p>XI - concordar integralmente com os termos da Resolução CD/FNDE
					Nº de 2013 publicada no Diário Oficial da União em, que estabelece
					os critérios e as normas de transferência automática de recursos
					financeiros do Projovem Urbano e do Projovem Campo – Saberes da
					Terra para a execução das ações do Programa;</p>
				<p>XII - autorizar o FNDE/MEC a estornar ou bloquear valores
					creditados indevidamente na conta corrente do Programa em favor do
					Ente Federado, mediante solicitação direta ao agente financeiro
					depositário dos recursos ou procedendo ao desconto nas parcelas
					subsequentes;</p>
				<p>XIII - restituir ao FNDE/MEC, no prazo de dez dias úteis a contar
					do recebimento da notificação e na forma prevista nos §§ 17 a 20 do
					art. 18 da referida Resolução, os valores creditados indevidamente
					ou objeto de eventual irregularidade constatada, quando inexistir
					saldo suficiente na conta corrente e não houver repasses futuros a
					serem efetuados;</p>
				<p>XIV - Aplica-se ao presente termo de adesão o previsto no art.
					30, § 5º e no art. 36, § 4º do Decreto n.º 6.629/2008.</p>
				<br>
				<h5>
					<strong>Cláusula Terceira – DAS OBRIGAÇÕES DO ESTADO/DISTRITO
						FEDERAL</strong>
				</h5>
				<br> <br>
				<p>1. O Estado/Distrito Federal se obriga a:</p>

				<p>1.1 Atingir a seguinte meta de atendimento de jovens para o
					Projovem Urbano, edição 2013:</p>
				<br>
<?php
		if (! $dados ['ajustado']) {
			?>
                <table border=1 align=center width=30%>
					<tr>
						<td colspan="5" align="center"><b>Meta 2013</b></td>
					</tr>
					<tr>
						<td align="center"><b>Meta Total</b></td>
						<td align="center">Público Juventude Viva (anexo II) Projovem
							Urbano</td>
						<td align="center">Público Unidades Prisionais Projovem Urbano</td>
						<td align="center">Público Geral do Projovem Urbano</td>
					</tr>
                    <?php if( $_SESSION['projovemurbano']['estuf'] ) { ?>
                     <tr>
	                    <?php
				$sql = "SELECT coalesce( ( geral + 0), 0 ) as total, coalesce(juventude, 0 ) as juventude, coalesce( prisional, 0 ) as prisional, geral as projovem, 0 as campo FROM territorios.estado e 
	                              JOIN projovemurbano.cargameta c ON c.cmecodibge = e.estcod::numeric 
	                              WHERE estuf='" . $_SESSION ['projovemurbano'] ['estuf'] . "'
	                              		AND c.ppuid=" . $_SESSION ['projovemurbano'] ['ppuid'] . "";
				$rsValoresMeta = $db->pegaLinha ( $sql );
				?>
	                    	<td><strong><?php echo $rsValoresMeta['total'];?></strong></td>
						<td><strong><?php echo $rsValoresMeta['juventude'];?></strong></td>
						<td><strong><?php echo $rsValoresMeta['prisional'];?></strong></td>
						<td><strong><?php echo $rsValoresMeta['projovem'];?></strong></td>
						<td><strong><?php echo $rsValoresMeta['campo'];?></strong></td>
					</tr>
                    <?php } ?>
                </table>
  <?php  }else{ ?>  
    				<table border=1 align=center width=30%>
					<tr>
						<td align="center" colspan="4"><b>Meta 2013</b></td>
					</tr>
					<tr>
						<td align="center"><b>Meta Total Ajustada</b></td>
						<td align="center"><b>Público Juventude Viva (anexo II) *</b></td>
						<td align="center"><b>Público Unidades Prisionais Projovem Urbano</b></td>
						<td align="center"><b>Público Geral</b></td>
					</tr>
<?php if( $_SESSION['projovemurbano']['estuf'] ) { ?>
                    <tr>
						<td align="center"><?= ( $rsMetas['juventudevivaa'] + $rsMetas['publicogerala'] + $rsMetas['prisionaisa'] ) ? ($rsMetas['juventudevivaa'] +$rsMetas['publicogerala'] + $rsMetas['prisionaisa'] )	: '0'; ?></td>
						<td align="center"><?= $rsMetas['juventudevivaa'] 	? $rsMetas['juventudevivaa'] 	: '0'; ?></td>
						<td align="center"><?= $rsMetas['prisionaisa'] 	? $rsMetas['prisionaisa'] 	: '0'; ?></td>
						<td align="center"><?= $rsMetas['publicogerala'] 	? $rsMetas['publicogerala'] 	: '0'; ?></td>
					</tr>
                    <?php } ?>
                </table>
                
<?php  } ?>    
                <br>
				<p>1.2 Cumprir as seguintes diretrizes:</p>
				<br>
				<p>I - priorizar o atendimento aos jovens residentes nos municípios
					integrantes do Plano Juventude Viva, das políticas de enfrentamento
					à violência e das regiões impactadas pelas grandes obras do Governo
					Federal, bem como aos jovens catadores de resíduos sólidos e
					egressos do Programa Brasil Alfabetizado;</p>
				<p>II - priorizar o atendimento às jovens mulheres, no caso da
					oferta em unidades do sistema prisional;</p>
				<p>III - garantir o funcionamento do comitê gestor do Projovem
					Urbano, no âmbito local, sob coordenação da Secretaria de Educação,
					composto por representação do Conselho de Juventude, quando existir
					na localidade, dos órgãos de políticas de juventude, das políticas
					para mulheres, da promoção da igualdade racial, dos jovens
					participantes no Programa, das demais secretarias afins, além da
					Agenda de Desenvolvimento Integrado de Alfabetização e Educação de
					Jovens e Adultos, para garantir efetividade ao acompanhamento e
					apoio à execução das ações do Programa, observada a
					intersetorialidade necessária para a execução dessas ações;</p>
				<p>IV - garantir o funcionamento do comitê gestor do Projovem Campo
					– Saberes da Terra, no âmbito local, sob coordenação da Secretaria
					de Educação, composto por representação do Conselho de Juventude,
					quando existir na localidade, dos órgãos locais de políticas de
					juventude, dos movimentos sociais do campo e dos colegiados
					territoriais, bem como do órgão local de políticas para mulheres,
					de promoção da igualdade racial, dos jovens participantes no
					Programa, das demais secretarias afins e da Agenda de
					Desenvolvimento Integrado de Alfabetização e Educação de Jovens e
					Adultos e dos Comitês, Fóruns e/ou Articulações Estaduais de
					Educação do Campo, para garantir efetividade ao acompanhamento e
					apoio à execução das ações do Programa, observada a
					intersetorialidade necessária para a execução dessas ações;</p>
				<p>V - assegurar que 50% dos membros do comitê gestor local do
					Projovem Campo – Saberes da Terra seja de representantes das
					entidades que compõem os Comitês, Fóruns e/ou Articulações
					Estaduais de Educação do Campo;</p>
				<p>VI - garantir a oferta de Educação de Jovens e Adultos –
					EJA/Ensino Médio aos jovens atendidos pelo Programa nas escolas de
					sua rede, proporcionando a continuidade de seus estudos.</p>
				<br> <br> <br>
<?php
		if (! $dados ['ajustado']) {
			?>
               <table border=1 align=center width=30%>
					<tr>
						<td colspan="5" align="center"><b>Meta 2013</b></td>
					</tr>
					<tr>
						<td align="center"><b>Meta Total</b></td>
						<td align="center">Público Juventude Viva (anexo II) Projovem
							Urbano</td>
						<td align="center">Público Geral do Projovem Urbano</td>
					</tr>
                     <?php if( $_SESSION['projovemurbano']['muncod'] ) { ?>
                     <tr>
                    <?php
				$sql = "SELECT *, coalesce( ( geral + juventude  ), 0 ) as total, coalesce(juventude, 0 ) as juventude, coalesce( prisional, 0 ) as prisional, geral as projovem, 0 as campo FROM territorios.municipio e 
	                              JOIN projovemurbano.cargameta c ON c.cmecodibge = e.muncod::numeric 
	                              WHERE muncod='" . $_SESSION ['projovemurbano'] ['muncod'] . "'
	                              		AND c.ppuid=" . $_SESSION ['projovemurbano'] ['ppuid'] . "";
				
				$rsValoresMeta = $db->pegaLinha ( $sql );
				?>
	                    	<td><strong><?php echo $rsValoresMeta['total'];?></strong></td>
						<td><strong><?php echo $rsValoresMeta['juventude'];?></strong></td>
						<td><strong><?php echo $rsValoresMeta['projovem'];?></strong></td>
						<td><strong><?php echo $rsValoresMeta['campo'];?></strong></td>
					</tr>
	                <?php } ?>
                </table>
  <?php  }else{ ?>              
  				<table border=1 align=center width=30%>
					<tr>
						<td align="center" colspan="4"><b>Meta 2014</b></td>
					</tr>
					<tr>
						<td align="center"><b>Meta Total Ajustada</b></td>
						<td align="center"><b>Público Juventude Viva (anexo II) *</b></td>
						<td align="center"><b>Público Geral</b></td>
					</tr>
                    <?php if( $_SESSION['projovemurbano']['muncod'] ) { ?>
                    <tr>
						<td align="center"><?= ( $rsMetas['juventudevivaa'] + $rsMetas['publicogerala']  ) ? ($rsMetas['juventudevivaa'] +$rsMetas['publicogerala'] )	: '0'; ?></td>
						<td align="center"><?= $rsMetas['juventudevivaa'] 	? $rsMetas['juventudevivaa'] 	: '0'; ?></td>
						<td align="center"><?= $rsMetas['publicogerala'] 	? $rsMetas['publicogerala'] 	: '0'; ?></td>
					</tr>
                    <?php  } ?>
                </table>
  <?php } ?>
                 <p>1.2 Cumprir as seguintes diretrizes:</p>
				<br>

				<p>I - priorizar o atendimento nas escolas localizadas nas regiões
					impactadas por grandes obras do Governo Federal, nas regiões com
					maiores índices de violência contra a juventude negra e nas áreas
					de abrangência das políticas de enfrentamento à violência, bem como
					atender aos jovens catadores de resíduos sólidos e egressos do
					Programa Brasil Alfabetizado.</p>
				<p>II - garantir o funcionamento do comitê gestor do Projovem
					Urbano, no âmbito local, sob coordenação da Secretaria de Educação,
					composto por representação do Conselho de Juventude, quando existir
					na localidade, dos órgãos de políticas de juventude, das políticas
					para mulheres, da promoção da igualdade racial, dos jovens
					participantes no Programa, das demais secretarias afins, para
					garantir efetividade ao acompanhamento e apoio à execução das ações
					do Programa, observada a intersetorialidade necessária para a
					execução dessas ações;</p>
				<p>III - garantir o funcionamento do comitê gestor do Projovem Campo
					– Saberes da Terra, no âmbito local, sob coordenação da Secretaria
					de Educação, composto por representação do Conselho de Juventude,
					quando existir na localidade, dos órgãos locais de políticas de
					juventude, dos movimentos sociais do campo e dos colegiados
					territoriais, bem como do órgão local de políticas para mulheres,
					de promoção da igualdade racial, dos jovens participantes no
					Programa, das demais secretarias afins, para garantir efetividade
					ao acompanhamento e apoio à execução das ações do Programa,
					observada a intersetorialidade necessária para a execução dessas
					ações;</p>
				<p>IV - articular-se com as redes estaduais de ensino visando
					garantir a continuidade de estudos para os jovens atendidos pelo
					Programa.</p>
				<br>
				<h5>
					<strong>Cláusula Quinta – DA RECISÃO</strong>
				</h5>
				<p>O presente instrumento poderá ser denunciado a qualquer tempo, no
					interesse das partes, ou rescindido pelo não cumprimento das
					cláusulas e/ou condições, observado o disposto nos artigos 77 a 80
					da Lei nº 8.666, de 21 de junho de 1993, e o Decreto nº 6.170, 25
					de julho de 2007, no que couber, independentemente de interpelação
					judicial ou extrajudicial ou daquelas dispostas nos artigos 86 a 88
					do mesmo diploma legal.</p>
				<br> <br>

				<h5>
					<strong>Cláusula Sexta – DA PUBLICAÇÃO</strong>
				</h5>
				<p>Caberá à Secretaria de Educação Continuada, Alfabetização,
					Diversidade e Inclusão - SECADI/MEC proceder à publicação do
					presente Termo de Adesão no Diário Oficial da União – DOU, conforme
					estabelecido no parágrafo único do art. 61 da Lei nº 8.666, de 21
					de junho de 1993.</p>
				<br> <br> <br>

				<h5>
					<strong>Cláusula Sétima– DO FORO</strong>
				</h5>
				<p>O foro competente para dirimir qualquer questão relativa a
					instrumento é o da Justiça Federal, Foro da cidade de Brasília/DF,
					Seção Judiciária do Distrito Federal.</p>
				<br> <br> <br>
                
<?
		if ($dados ['ajustado']) :
			$adesaotermoajustadadata = $db->pegaLinha ( "SELECT to_char(adesaotermoajustadodata,'dd') as dia, to_char(adesaotermoajustadodata,'mm') as mes, to_char(adesaotermoajustadodata,'YYYY') as ano FROM projovemurbano.projovemurbano WHERE pjuid=" . $_SESSION ['projovemurbano'] ['pjuid'] );
			?>
                    <p align=center>___________________________________, <?= (($adesaotermoajustadadata['dia']) ? $adesaotermoajustadadata['dia'] : date("d")) . " de " . $db->pegaUm("SELECT mesdsc FROM public.meses WHERE mescod='" . (($adesaotermoajustadadata['mes']) ? $adesaotermoajustadadata['mes'] : date("m")) . "'") . " de " . (($adesaotermoajustadadata['ano']) ? $adesaotermoajustadadata['ano'] : date("Y")) ?></p>

		 
		 <?
else :
			$adesaotermodata = $db->pegaLinha ( "SELECT to_char(adesaotermodata,'dd') as dia, to_char(adesaotermodata,'mm') as mes, to_char(adesaotermodata,'YYYY') as ano FROM projovemurbano.projovemurbano WHERE pjuid=" . $_SESSION ['projovemurbano'] ['pjuid'] );
			?>
                    <p align=center>___________________________________, <?= (($adesaotermodata['dia']) ? $adesaotermodata['dia'] : date("d")) . " de " . $db->pegaUm("SELECT mesdsc FROM public.meses WHERE mescod='" . (($adesaotermodata['mes']) ? $adesaotermodata['mes'] : date("m")) . "'") . " de " . (($adesaotermodata['ano']) ? $adesaotermodata['ano'] : date("Y")) ?></p>

		
		<?
endif;
		?>

                <br> <br>
				<p>
				
				
				<center>
					___________________________________________________________________
					</p>
					<p>
					
					
					<center>
						<b>Secretário(a) Municipal/Estadual/Distrital de Educação</b>
						</p>
						<br> <br>

						<p align=center>
							<b>JOSÉ HENRIQUE PAIM FERNANDES</b>
					
					</center>
					</p>
					<p align=center>Ministro de Estado da Educação
				
				</center>
				</p>
<?
	} else {
		?>
                <p>
					O Distrito Federal/Estado/Município de <b><?= $dadosT['estdescricao'] ?></b>,doravante
					denominado Ente Federado, por meio da sua Secretaria de Educação,
					CNPJ: <b><?= mascaraglobal($rsSecretaria['entnumcpfcnpj'], "##.###.###/####-##") ?></b>
					representado por seu (sua) Secretário(a), <b><?= $dados['usunome'] ?></b>,
					CPF nº <b><?= mascaraglobal($dados['usucpf'], "###.###.###-##") ?></b>,
					RG nº <b><?= $dados['iserg'] ?></b>, expedido por <b><?= $dados['iseorgexp'] ?></b>,
					com atribuição legal para representar o governador ou o prefeito
					neste ato e devidamente estabelecido à <b> <?= $dados['iseendereco'] . ", nº " . $dados['isenumero'] . ", " . $dados['isebairro'] . ", " . $db->pegaUm("SELECT mundescricao FROM territorios.municipio WHERE muncod='" . $dados['isemunicipio'] . "'") . ", " . $dados['iseuf']. ", " ?></b><b>CEP <?= mascaraglobal($dados['isecep'], "#####-###")?> </b>,
					e o Ministério da Educação, representado pelo Ministro de Estado,
					resolvem firmar o presente Termo de Adesão ao Programa Nacional de
					Inclusão de Jovens – Projovem Urbano e/ou Projovem Campo – Saberes
					da Terra, edição 2014, em conformidade, no que couber, com a Lei
					n.º 8.666, de 21 de junho de 1993, e a legislação correlata,
					consideradas as seguintes condições:
				</p>
				<BR />
				<h5>
					<strong>Cláusula Primeira – Do Objeto</strong>
				</h5>
				<br>
				<p>O presente termo tem por objeto a adesão do Ente Federado ao
					Programa Nacional de Inclusão de Jovens – Projovem Urbano e/ou
					Projovem Campo - Saberes da Terra, instituído nos termos da Lei nº
					11.692 de 10 de junho de 2008, regulamentado pelo Decreto nº 6.629
					de 4 de novembro de 2008 e pelo Decreto nº 7.649 de 21 de dezembro
					de 2011.</p>
				<br>

				<h5>
					<strong>Cláusula Segunda – DAS OBRIGAÇÕES DOS ENTES FEDERADOS:</strong>
				</h5>
				<br> <br>
				<p>1. Os Entes Federados se comprometem a cumprir as seguintes
					diretrizes abaixo:</p>
				<br>

				<p>I -executar o Programa, por meio da sua secretaria de Educação,
					que deverá coordenar o desenvolvimento das ações de implementação
					do Programa, garantindo a necessária articulação com a rede de
					ensino, conforme seus Projetos Pedagógicos Integrados, as
					orientações da Secretaria de Educação Continuada, Alfabetização,
					Diversidade e Inclusão – SECADI/MEC e de acordo com as Resoluções
					CD/FNDE/MEC Nº 8/2014 e Nº 11/2014;</p>
				<p>II - executar os recursos orçamentários repassados pelo Governo
					Federal exclusivamente na implementação do Programa, gerindo-os com
					eficiência, eficácia e transparência, visando a efetividade das
					ações;</p>
				<p>III - estabelecer como foco a aprendizagem, realizando todos os
					esforços necessários para garantir a certificação em Ensino
					Fundamental – EJA e em qualificação profissional como formação
					inicial dos jovens matriculados no Programa;</p>
				<p>IV - responsabilizar-se pela divulgação do Programa em nível
					local, inclusive quanto aos processos de matrícula a serem
					realizados pelo Ente Federado, mobilizando a comunidade e suas
					lideranças, os jovens, pais e responsáveis, bem como os meios
					políticos e administrativos;</p>
				<p>V - empreender esforços para viabilizar a expedição dos
					documentos necessários para a matrícula dos jovens a serem
					atendidos pelo Programa;</p>
				<p>VI -matricular os estudantes por meio de Sistema de Matrícula,
					Acompanhamento de Frequência e Certificação do Projovem Urbano e
					Campo disponibilizado pela Secretaria de Educação Continuada,
					Alfabetização, Diversidade e Inclusão - SECADI/MEC, sendo esta a
					única forma de garantir a inclusão dos jovens no Programa, bem como
					ser responsável pela fidedignidade das informações lançadas no
					referido sistema;</p>
				<p>VII - garantir o acesso e as condições de permanência das pessoas
					público-alvo da educação especial ao Programa, por meio da oferta
					do atendimento educacional especializado e oferta de recursos e
					serviços de acessibilidade;</p>
				<p>VIII - desenvolver os Projetos Pedagógicos Integrados das duas
					modalidades do Programa em suas três dimensões, garantindo sua
					execução conforme legislação do Projovem Urbano e do Projovem Campo
					– Saberes da Terra e orientações da Secretaria de Educação
					Continuada, Alfabetização, Diversidade e Inclusão - SECADI/MEC;</p>
				<p>IX - acompanhar cada beneficiário individualmente, no caso do
					Projovem Urbano, mediante registro mensal de frequência e de
					entrega de trabalhos, e no caso do Projovem Campo – Saberes da
					Terra, mediante registro mensal de frequência por meio do Sistema
					de Matrícula, Acompanhamento da Frequência e Certificação do
					Projovem Urbano e Campo;</p>
				<p>X - prevenir e combater a evasão pelo acompanhamento individual
					das razões para a não frequência do educando e implantar medidas
					para superá-las;</p>
				<p>XI - concordar integralmente com os termos das Resoluções
					CD/FNDE/MEC Nº 8/2014 e Nº 11/2014 publicadas no Diário Oficial da
					União em 16 de abril de 2014, que estabelece os critérios e as
					normas de transferência automática de recursos financeiros do
					Projovem Urbano e do Projovem Campo – Saberes da Terra para a
					execução das ações do Programa;</p>
				<p>XII - autorizar o FNDE/MEC a estornar ou bloquear valores
					creditados indevidamente na conta corrente do Programa em favor do
					Ente Federado, mediante solicitação direta ao agente financeiro
					depositário dos recursos ou procedendo ao desconto nas parcelas
					subsequentes;</p>
				<p>XIII - restituir ao FNDE/MEC, no prazo de dez dias úteis a contar
					do recebimento da notificação e na forma prevista nas Resoluções
					CD/FNDE/MEC Nº 8/2014 e Nº 11/2014, os valores creditados
					indevidamente ou objeto de eventual irregularidade constatada,
					quando inexistir saldo suficiente na conta corrente e não houver
					repasses futuros a serem efetuados;</p>
				<p>XIV - aplica-se ao presente termo de adesão o previsto no art.
					30, § 5º e no art. 36, § 4º do Decreto nº 6.629/2008.</p>
				<br>
				<h5>
					<strong>Cláusula Terceira – DAS OBRIGAÇÕES DO ESTADO/DISTRITO
						FEDERAL</strong>
				</h5>
				<br> <br>
				<p>1. O Estado/Distrito Federal se obriga a:</p>

				<p>1.1 Atingir a seguinte meta de atendimento de jovens para o
					Projovem Urbano e/ou Projovem Campo - Saberes da Terra, edição
					2014:</p>
				<br>
<?php
		if (! $dados ['ajustado']) {
			?>
                <table border=1 align=center width=30%>
					<tr>
						<td colspan="5" align="center"><b>Meta 2014</b></td>
					</tr>
					<tr>
						<td align="center"><b>Meta Total</b></td>
						<td align="center">Público Juventude Viva (anexo II) Projovem
							Urbano</td>
						<td align="center">Público Unidades Prisionais Projovem Urbano</td>
						<td align="center">Público Geral do Projovem Urbano</td>
						<td align="center">Público Projovem Campo Saberes da Terra</td>
					</tr>
                    <?php if( $_SESSION['projovemurbano']['estuf'] ) { ?>
                     <tr>
	                    <?php
				$sql = "SELECT coalesce( ( geral + 0), 0 ) as total, coalesce(juventude, 0 ) as juventude, coalesce( prisional, 0 ) as prisional, geral as projovem, 0 as campo FROM territorios.estado e 
	                              JOIN projovemurbano.cargameta c ON c.cmecodibge = e.estcod::numeric 
	                              WHERE estuf='" . $_SESSION ['projovemurbano'] ['estuf'] . "'
	                              		AND c.ppuid=" . $_SESSION ['projovemurbano'] ['ppuid'] . "";
				$rsValoresMeta = $db->pegaLinha ( $sql );
				?>
	                    	<td><strong><?php echo $rsValoresMeta['total'];?></strong></td>
						<td><strong><?php echo $rsValoresMeta['juventude'];?></strong></td>
						<td><strong><?php echo $rsValoresMeta['prisional'];?></strong></td>
						<td><strong><?php echo $rsValoresMeta['projovem'];?></strong></td>
						<td><strong><?php echo $rsValoresMeta['campo'];?></strong></td>
					</tr>
                    <?php } ?>
                </table>
  <?php  }else{ ?>  
    				<table border=1 align=center width=30%>
					<tr>
						<td align="center" colspan="4"><b>Meta 2014</b></td>
					</tr>
					<tr>
						<td align="center"><b>Meta Total Ajustada</b></td>
						<td align="center"><b>Público Juventude Viva (anexo II) *</b></td>
						<td align="center"><b>Público Unidades Prisionais Projovem Urbano</b></td>
						<td align="center"><b>Público Geral</b></td>
						<td align="center">Público Projovem Campo Saberes da Terra</td>
					</tr>
<?php if( $_SESSION['projovemurbano']['estuf'] ) { ?>
                    <tr>
						<td align="center"><?= ( $rsMetas['juventudevivaa'] + $rsMetas['publicogerala'] + $rsMetas['prisionaisa'] ) ? ($rsMetas['juventudevivaa'] +$rsMetas['publicogerala'] + $rsMetas['prisionaisa'] )	: '0'; ?></td>
						<td align="center"><?= $rsMetas['juventudevivaa'] 	? $rsMetas['juventudevivaa'] 	: '0'; ?></td>
						<td align="center"><?= $rsMetas['prisionaisa'] 	? $rsMetas['prisionaisa'] 	: '0'; ?></td>
						<td align="center"><?= $rsMetas['publicogerala'] 	? $rsMetas['publicogerala'] 	: '0'; ?></td>
					</tr>
                    <?php } ?>
                </table>
                
<?php  } ?>    
                <br>
				<p>1.2 Cumprir as seguintes diretrizes:</p>
				<br>
				<p>I - priorizar o atendimento aos jovens residentes nos municípios
					integrantes do Plano Juventude Viva, das políticas de enfrentamento
					à violência e das regiões impactadas pelas grandes obras do Governo
					Federal, bem como aos jovens catadores de resíduos sólidos e
					egressos do Programa Brasil Alfabetizado;</p>
				<p>II - priorizar o atendimento às jovens mulheres, no caso da
					oferta em unidades do sistema prisional;</p>
				<p>III - garantir o funcionamento do comitê gestor do Projovem
					Urbano, no âmbito local, sob coordenação da Secretaria de Educação,
					composto por representação do Conselho de Juventude, quando existir
					na localidade, dos órgãos de políticas de juventude, das políticas
					para mulheres, da promoção da igualdade racial, dos jovens
					participantes no Programa, das demais secretarias afins, além da
					Agenda de Desenvolvimento Integrado de Alfabetização e Educação de
					Jovens e Adultos, para garantir efetividade ao acompanhamento e
					apoio à execução das ações do Programa, observada a
					intersetorialidade necessária para a execução dessas ações;</p>
				<p>IV - garantir o funcionamento do comitê gestor do Projovem Campo
					– Saberes da Terra, no âmbito local, sob coordenação da Secretaria
					de Educação, composto por representação do Conselho de Juventude,
					quando existir na localidade, dos órgãos locais de políticas de
					juventude, dos movimentos sociais do campo e dos colegiados
					territoriais, bem como do órgão local de políticas para mulheres,
					de promoção da igualdade racial, dos jovens participantes no
					Programa, das demais secretarias afins e da Agenda de
					Desenvolvimento Integrado de Alfabetização e Educação de Jovens e
					Adultos e dos Comitês, Fóruns e/ou Articulações Estaduais de
					Educação do Campo, para garantir efetividade ao acompanhamento e
					apoio à execução das ações do Programa, observada a
					intersetorialidade necessária para a execução dessas ações;</p>
				<p>V - assegurar que 50% dos membros do comitê gestor local do
					Projovem Campo – Saberes da Terra seja de representantes das
					entidades que compõem os Comitês, Fóruns e/ou Articulações
					Estaduais de Educação do Campo;</p>
				<p>VI - garantir a oferta de Educação de Jovens e Adultos –
					EJA/Ensino Médio aos jovens atendidos pelo Programa nas escolas de
					sua rede, proporcionando a continuidade de seus estudos.</p>
				<br> <br>
				<h5>
					<strong>Cláusula Quarta – DAS OBRIGAÇÕES DO MUNICÍPIO</strong>
				</h5>
				<br> <br>

				<p>
					1. O <strong> Município </strong>se compromete a:
				</p>
				<br />
				<p>1.1 Atingir a seguinte meta de atendimento de jovens para o
					Projovem Urbano e/ou Projovem Campo - Saberes da Terra, edição
					2014:</p>
				<br>
<?php
		if (! $dados ['ajustado']) {
			?>
               <table border=1 align=center width=30%>
					<tr>
						<td colspan="5" align="center"><b>Meta 2014</b></td>
					</tr>
					<tr>
						<td align="center"><b>Meta Total</b></td>
						<td align="center">Público Juventude Viva (anexo II) Projovem
							Urbano</td>
						<td align="center">Público Geral do Projovem Urbano</td>
						<td align="center">Público Projovem Campo Saberes da Terra</td>
					</tr>
                     <?php if( $_SESSION['projovemurbano']['muncod'] ) { ?>
                     <tr>
                    <?php
				$sql = "SELECT *, coalesce( ( geral + juventude  ), 0 ) as total, coalesce(juventude, 0 ) as juventude, coalesce( prisional, 0 ) as prisional, geral as projovem, 0 as campo FROM territorios.municipio e 
	                              JOIN projovemurbano.cargameta c ON c.cmecodibge = e.muncod::numeric 
	                              WHERE muncod='" . $_SESSION ['projovemurbano'] ['muncod'] . "'
	                              		AND c.ppuid=" . $_SESSION ['projovemurbano'] ['ppuid'] . "";
				
				$rsValoresMeta = $db->pegaLinha ( $sql );
				?>
	                    	<td><strong><?php echo $rsValoresMeta['total'];?></strong></td>
						<td><strong><?php echo $rsValoresMeta['juventude'];?></strong></td>
						<td><strong><?php echo $rsValoresMeta['projovem'];?></strong></td>
						<td><strong><?php echo $rsValoresMeta['campo'];?></strong></td>
					</tr>
	                <?php } ?>
                </table>
  <?php  }else{ ?>              
  				<table border=1 align=center width=30%>
					<tr>
						<td align="center" colspan="4"><b>Meta 2014</b></td>
					</tr>
					<tr>
						<td align="center"><b>Meta Total Ajustada</b></td>
						<td align="center"><b>Público Juventude Viva (anexo II) *</b></td>
						<td align="center"><b>Público Geral</b></td>
						<td align="center">Público Projovem Campo Saberes da Terra</td>
					</tr>
                    <?php if( $_SESSION['projovemurbano']['muncod'] ) { ?>
                    <tr>
						<td><strong><?php echo $rsValoresMeta['total'];?></strong></td>
						<td><strong><?php echo $rsValoresMeta['juventude'];?></strong></td>
						<td><strong><?php echo $rsValoresMeta['projovem'];?></strong></td>
						<td><strong><?php echo $rsValoresMeta['campo'];?></strong></td>
					</tr>
                    <?php  } ?>
                </table>
  <?php } ?>
                 <p>1.2 Cumprir as seguintes diretrizes abaixo:</p>
				<br>

				<p>I - priorizar o atendimento nas escolas localizadas nas regiões
					impactadas por grandes obras do Governo Federal, nas regiões com
					maiores índices de violência contra a juventude negra e nas áreas
					de abrangência das políticas de enfrentamento à violência, bem como
					atender aos jovens catadores de resíduos sólidos e egressos do
					Programa Brasil Alfabetizado.</p>
				<p>II - garantir o funcionamento do comitê gestor do Projovem
					Urbano, no âmbito local, sob coordenação da Secretaria de Educação,
					composto por representação do Conselho de Juventude, quando existir
					na localidade, dos órgãos de políticas de juventude, das políticas
					para mulheres, da promoção da igualdade racial, dos jovens
					participantes no Programa, das demais secretarias afins, para
					garantir efetividade ao acompanhamento e apoio à execução das ações
					do Programa, observada a intersetorialidade necessária para a
					execução dessas ações;</p>
				<p>III - garantir o funcionamento do comitê gestor do Projovem Campo
					– Saberes da Terra, no âmbito local, sob coordenação da Secretaria
					de Educação, composto por representação do Conselho de Juventude,
					quando existir na localidade, dos órgãos locais de políticas de
					juventude, dos movimentos sociais do campo e dos colegiados
					territoriais, bem como do órgão local de políticas para mulheres,
					de promoção da igualdade racial, dos jovens participantes no
					Programa, das demais secretarias afins, para garantir efetividade
					ao acompanhamento e apoio à execução das ações do Programa,
					observada a intersetorialidade necessária para a execução dessas
					ações;</p>
				<p>IV - articular-se com as redes estaduais de ensino visando
					garantir a continuidade de estudos para os jovens atendidos pelo
					Programa.</p>
				<br>
				<h5>
					<strong>Cláusula Quinta – DA RECISÃO</strong>
				</h5>
				<p>O presente instrumento poderá ser denunciado a qualquer tempo, no
					interesse das partes, ou rescindido pelo não cumprimento das
					cláusulas e/ou condições, observado o disposto nos artigos 77 a 80
					da Lei nº 8.666, de 21 de junho de 1993, no que couber,
					independentemente de interpelação judicial ou extrajudicial ou
					daquelas dispostas nos artigos 86 a 88 do mesmo diploma legal.</p>
				<br> <br>

				<h5>
					<strong>Cláusula Sexta – DA PUBLICAÇÃO</strong>
				</h5>
				<p>Caberá à Secretaria de Educação Continuada, Alfabetização,
					Diversidade e Inclusão - SECADI/MEC proceder à publicação do
					presente Termo de Adesão no Diário Oficial da União – DOU, conforme
					estabelecido no parágrafo único do art. 61 da Lei nº 8.666, de 21
					de junho de 1993.</p>
				<br> <br> <br>

				<h5>
					<strong>Cláusula Sétima– DO FORO</strong>
				</h5>
				<p>O foro competente para dirimir qualquer questão relativa a
					instrumento é o da Justiça Federal, Foro da cidade de Brasília/DF,
					Seção Judiciária do Distrito Federal.</p>
				<br> <br> <br>
                
<?
		if ($dados ['ajustado']) :
			$adesaotermoajustadadata = $db->pegaLinha ( "SELECT to_char(adesaotermoajustadodata,'dd') as dia, to_char(adesaotermoajustadodata,'mm') as mes, to_char(adesaotermoajustadodata,'YYYY') as ano FROM projovemurbano.projovemurbano WHERE pjuid=" . $_SESSION ['projovemurbano'] ['pjuid'] );
			?>
                    <p align=center>___________________________________, <?= (($adesaotermoajustadadata['dia']) ? $adesaotermoajustadadata['dia'] : date("d")) . " de " . $db->pegaUm("SELECT mesdsc FROM public.meses WHERE mescod='" . (($adesaotermoajustadadata['mes']) ? $adesaotermoajustadadata['mes'] : date("m")) . "'") . " de " . (($adesaotermoajustadadata['ano']) ? $adesaotermoajustadadata['ano'] : date("Y")) ?></p>

		 
		 <?
else :
			$adesaotermodata = $db->pegaLinha ( "SELECT to_char(adesaotermodata,'dd') as dia, to_char(adesaotermodata,'mm') as mes, to_char(adesaotermodata,'YYYY') as ano FROM projovemurbano.projovemurbano WHERE pjuid=" . $_SESSION ['projovemurbano'] ['pjuid'] );
			?>
                    <p align=center>___________________________________, <?= (($adesaotermodata['dia']) ? $adesaotermodata['dia'] : date("d")) . " de " . $db->pegaUm("SELECT mesdsc FROM public.meses WHERE mescod='" . (($adesaotermodata['mes']) ? $adesaotermodata['mes'] : date("m")) . "'") . " de " . (($adesaotermodata['ano']) ? $adesaotermodata['ano'] : date("Y")) ?></p>

		
		<?
endif;
		?>

                <br> <br>
				<p>
				
				
				<center>
					___________________________________________________________________
					</p>
					<p>
					
					
					<center>
						<b>Secretário(a) Municipal/Estadual/Distrital de Educação</b>
						</p>
						<br> <br>

						<p align=center>
							<b>JOSÉ HENRIQUE PAIM FERNANDES</b>
					
					</center>
					</p>
					<p align=center>Ministro de Estado da Educação
				
				</center>
				</p>
<?php
	}
	?>           
            
		
		</td>
	</tr>
</table>

<?
}
function termoMunicipio($dados) {
	global $db;
	
	$rsSecretaria = recuperaSecretariaPorUfMuncod ();
	
	$rsMetas = recuperaMetasPorUfMuncod ( $dados );
	
	$dadosT = $db->pegaLinha ( "SELECT * FROM territorios.municipio m
                                  LEFT JOIN projovemurbano.cargameta c ON c.cmecodibge::character(7) = m.muncod
                                  WHERE 
                                  	muncod='" . $_SESSION ['projovemurbano'] ['muncod'] . "'
                                  	AND c.ppuid = '" . $_SESSION ['projovemurbano'] ['ppuid'] . "'
                       " );
	?>
<table class="tabela" cellSpacing="1" cellPadding="3" align="center">
	<tr>
		<td>
<?php
	if ($_SESSION ['projovemurbano'] ['ppuid'] == '1') {
		?>
            <h3 style="text-align: center">MINISTÉRIO DA EDUCAÇÃO</h3>
			<h4 style="text-align: center">SECRETARIA DE EDUCAÇÃO CONTINUADA,
				ALFABETIZAÇÃO, DIVERSIDADE E INCLUSÃO</h4>
			<h4 style="text-align: center">TERMO DE ADESÃO</h4>

			<p>
				O Município de <b><?= $dadosT['mundescricao'] ?></b>, doravante
				denominado Município, representado por seu (sua) Secretário(a) de
				Educação , <b><?= $dados['usunome'] ?></b>, CPF nº <b><?= mascaraglobal($dados['usucpf'], "###.###.###-##") ?></b>,
				RG nº <b><?= $dados['iserg'] ?></b>, expedido por <b><?= $dados['iseorgexp'] ?></b>,
				devidamente estabelecido à <b><?= $dados['iseendereco'] . ", nº " . $dados['isenumero'] . ", " . $dados['isebairro'] . ", " . $db->pegaUm("SELECT mundescricao FROM territorios.municipio WHERE muncod='" . $dados['isemunicipio'] . "'") . ", " . $dados['iseuf']. ", " ?></b><b>CEP<?= mascaraglobal($dados['isecep'], "#####-###")?></b>,
				resolve firmar o presente Termo de Adesão ao Programa Nacional de
				Inclusão de Jovens – Projovem Urbano consideradas as seguintes
				condições:
			</p>

			<h3>Cláusula Primeira – Do Objeto</h3>
			<p>O presente termo tem por objeto a adesão do Município ao Programa
				Nacional de Inclusão de Jovens – Projovem Urbano, instituído nos
				termos da Lei nº 11.692 de 10 de junho de 2008 , regulamentado pelo
				Decreto nº 6.629 de 04 de novembro de 2008 e em conformidade com a
				Lei Nº 8.666, de 21 de junho de 1993.</p>

			<h3>Cláusula Segunda – O Município se compromete a:</h3>

			<p>1. Atingir a seguinte meta de atendimento de jovens para o
				Projovem Urbano, no período de 2012:</p>

			<table border=1 align=center width=30%>
				<tr>
					<td align="center"><b>Anos</b></td>
					<td align="center"><b>2012</b></td>
				</tr>
				<tr>
					<td align="center"><b>Meta</b></td>
                    <? if ($dados['ajustado']) : ?>
                        <? $suametaajustada = $db->pegaUm("SELECT suametaajustada FROM projovemurbano.sugestaoampliacao WHERE pjuid='" . $_SESSION['projovemurbano']['pjuid'] . "'"); ?>
                    <td align="center"><?= (($suametaajustada) ? $suametaajustada : "Não cadastrado") ?></td>
                    <? else : ?>
                    <td align="center"><?= (($dadosT['cmemeta']) ? $dadosT['cmemeta'] : "Não cadastrado") ?></td>
                    <? endif; ?>
                </tr>
			</table>
			<p>2. Cumprir as seguintes diretrizes abaixo:</p>
			<p>I - estabelecer como foco a aprendizagem, realizando todos os
				esforços necessários para garantir a certificação em Ensino
				Fundamental – EJA e em qualificação profissional como formação
				inicial dos jovens matriculados no Projovem Urbano;</p>
			<p>II - responsabilizar-se pela divulgação do ProJovem Urbano em
				nível local, inclusive quanto aos processos de matrícula a serem
				realizados pelo Município, mobilizando a comunidade e suas
				lideranças, os jovens, pais e responsáveis, bem como os meios
				políticos e administrativos;</p>
			<p>III - empreender esforços para viabilizar a expedição dos
				documentos necessários para a matrícula dos jovens a serem atendidos
				pelo Programa;</p>
			<p>IV - matricular os estudantes por meio de Sistema de Matrícula,
				Acompanhamento de Frequência e Certificação do Projovem Urbano que a
				Secretaria de Educação Continuada, Alfabetização, Diversidade e
				Inclusão - SECADI/MEC disponibiliza online, sendo esta a única forma
				de garantir a inclusão dos jovens no Programa, bem como ser
				responsável pela fidedignidade das informações lançadas no referido
				sistema;</p>
			<p>V - garantir o acesso e as condições de permanência das pessoas
				público-alvo da educação especial ao Programa, por meio da oferta do
				Atendimento Educacional Especializado e oferta de recursos e
				serviços de acessibilidade.</p>
			<p>VI - desenvolver o Projeto Pedagógico Integrado do Programa em
				suas três dimensões, garantindo sua execução conforme legislação do
				Projovem Urbano e orientações da Secretaria de Educação Continuada,
				Alfabetização, Diversidade e Inclusão - SECADI/MEC;</p>
			<p>VII - acompanhar cada beneficiário do ProJovem Urbano,
				individualmente, mediante registro mensal de freqüência, por meio do
				Sistema de Matrícula, Acompanhamento da Freqüência e Certificação do
				Projovem Urbano;</p>
			<p>VIII - prevenir e combater a evasão pelo acompanhamento individual
				das razões para a não-freqüência do educando e implantar medidas
				para superá-las;</p>
			<p>IX - garantir o funcionamento do Comitê Gestor do Programa no
				âmbito local, sob coordenação da Secretaria de Educação, composto
				pelo Conselho de Juventude, por órgãos de políticas de juventude,
				quando existir na localidade, bem como pelas demais secretarias e
				órgãos afins, observada a intersetorialidade necessária para a
				execução das ações previstas pelo Programa;</p>
			<p>X - articular-se com as redes estaduais de ensino visando garantir
				a continuidade de estudos para os jovens atendidos pelo Programa;</p>
			<p>XI - concordar integralmente com os termos da Resolução CD/FNDE nº
				60/2011, publicada no Diário Oficial da União em 10 de novembro de
				2011, que estabelece os critérios e as normas de transferência
				automática de recursos financeiros do ProJovem Urbano para a
				execução das ações do Programa;</p>
			<p>XII - autorizar o FNDE/MEC a estornar ou bloquear valores
				creditados indevidamente na conta corrente do Programa em favor do
				Município, mediante solicitação direta ao agente financeiro
				depositário dos recursos ou procedendo ao desconto nas parcelas
				subseqüentes;</p>
			<p>XIII - restituir ao FNDE/MEC, no prazo de dez dias úteis a contar
				do recebimento da notificação e na forma prevista nos §§ 17 a 20 do
				Art. 18 da referida Resolução, os valores creditados indevidamente
				ou objeto de eventual irregularidade constatada, quando inexistir
				saldo suficiente na conta corrente e não houver repasses futuros a
				serem efetuados;</p>
			<p>XIV - atualizar permanentemente junto à Secretaria de Educação
				Continuada, Alfabetização, Diversidade e Inclusão/MEC, as
				informações prestadas no Plano de Implementação do Programa, sob
				pena de suspensão de pagamento de parcelas subseqüentes até a
				regularização da atualização dessas informações.</p>

			<h3>Cláusula Terceira – Da Rescisão</h3>
			<p>O presente instrumento poderá ser rescindido a qualquer tempo, no
				interesse das partes, ou pelo não cumprimento das cláusulas e/ou
				condições, observado o disposto nos artigos 77 a 80 da Lei Nº 8.666,
				de 21 de junho de 1993, independentemente de interpelação judicial
				ou extrajudicial ou daquelas dispostas nos artigos 86 a 88 do mesmo
				Diploma Legal.</p>

			<h3>Cláusula Quarta – Da Publicação</h3>
			<p>Caberá à Secretaria de Educação Continuada, Alfabetização,
				Diversidade e Inclusão - SECADI/MEC proceder à publicação do
				presente Termo de Adesão no Diário Oficial da União – DOU, conforme
				estabelecido no Parágrafo Único, do art. 61, da Lei Nº 8.666, de 21
				de junho de 1993.</p>

			<h3>Cláusula Quinta – Do Foro</h3>
			<p>O foro competente para dirimir dúvidas ou litígios oriundos deste
				instrumento é o da Justiça Federal, Foro da cidade de Brasília/DF,
				Seção Judiciária do Distrito Federal.</p> <br> <br> <br>
                
                <? if ($dados['ajustado']) : ?>
                    <? $adesaotermoajustadadata = $db->pegaLinha("SELECT to_char(adesaotermoajustadodata,'dd') as dia, to_char(adesaotermoajustadodata,'mm') as mes, to_char(adesaotermoajustadodata,'YYYY') as ano FROM projovemurbano.projovemurbano WHERE pjuid=" . $_SESSION['projovemurbano']['pjuid']); ?>
                <p align=center>___________________________________, <?= (($adesaotermoajustadadata['dia']) ? $adesaotermoajustadadata['dia'] : date("d")) . " de " . $db->pegaUm("SELECT mesdsc FROM public.meses WHERE mescod='" . (($adesaotermoajustadadata['mes']) ? $adesaotermoajustadadata['mes'] : date("m")) . "'") . " de " . (($adesaotermoajustadadata['ano']) ? $adesaotermoajustadadata['ano'] : date("Y")) ?></p>
                <? else : ?>
                    <? $adesaotermodata = $db->pegaLinha("SELECT to_char(adesaotermodata,'dd') as dia, to_char(adesaotermodata,'mm') as mes, to_char(adesaotermodata,'YYYY') as ano FROM projovemurbano.projovemurbano WHERE pjuid=" . $_SESSION['projovemurbano']['pjuid']); ?>
                <p align=center>___________________________________, <?= (($adesaotermodata['dia']) ? $adesaotermodata['dia'] : date("d")) . " de " . $db->pegaUm("SELECT mesdsc FROM public.meses WHERE mescod='" . (($adesaotermodata['mes']) ? $adesaotermodata['mes'] : date("m")) . "'") . " de " . (($adesaotermodata['ano']) ? $adesaotermodata['ano'] : date("Y")) ?></p>
                <? endif; ?>
                
                <br> <br>
			<p align=center>___________________________________________________________________</p>
			<p align=center>
				<b>Secretário(a) Municipal de Educação</b>
			</p>        		

	<?php
	
} elseif ($_SESSION ['projovemurbano'] ['ppuid'] == '2') {
		?>
            <h1 style="text-align: center">MINISTÉRIO DA EDUCAÇÃO</h1>
			<h2 style="text-align: center">GABINETE DO MINISTRO</h2>
			<h2 style="text-align: center">TERMO DE ADESÃO</h2>

			<p>
				O Estado/Município de <b><?= $dadosT['mundescricao'] ?></b>,
				doravante denominado Ente Federado, por meio da sua Secretaria de
				Educação, CNPJ:­­­­­<b><?php echo formatar_cnpj($rsSecretaria['entnumcpfcnpj']); ?></b>,
				representado por seu (sua) Secretário(a), <b><?= $dados['usunome'] ?></b>,
				CPF nº <b><?= mascaraglobal($dados['usucpf'], "###.###.###-##") ?></b>,
				RG nº <b><?= $dados['iserg'] ?></b>, expedido por <b><?= $dados['iseorgexp'] ?></b>,
				devidamente estabelecido à <b><?= $dados['iseendereco'] . ", nº " . $dados['isenumero'] . ", " . $dados['isebairro'] . ", " . $db->pegaUm("SELECT mundescricao FROM territorios.municipio WHERE muncod='" . $dados['isemunicipio'] . "'") . ", " . $dados['iseuf']. ", " ?></b><b>CEP<?= mascaraglobal($dados['isecep'], "#####-###")?></b>, 
                e o Ministério da Educação, representado pelo Ministro de Estado, resolvem firmar o presente Termo de Adesão ao Programa Nacional de Inclusão de Jovens – Projovem Urbano,  
                edição <?= $_SESSION['projovemurbano']['ppuano'] ?>, em conformidade, no que couber, com a Lei n.º 8.666, de 21 de junho de 1993, e a legislação correlata, consideradas as seguintes condições:
            </p>

			<h3>CLÁUSULA PRIMEIRA – Do objeto</h3>

			<p>1. O presente termo tem por objeto a adesão do Ente Federado ao
				Programa Nacional de Inclusão de Jovens – Projovem Urbano,
				instituído nos termos da Lei nº 11.692 de 10 de junho de 2008,
				regulamentado pelo Decreto nº 6.629 de 4 de novembro de 2008 e pelo
				Decreto nº 7.649 de 21 de dezembro de 2011.</p>

			<h3>CLÁUSULA SEGUNDA – DAS OBRIGAÇÕES DOS ENTES FEDERADOS:</h3>

			<p>1. Os Entes Federados se comprometem a cumprir as seguintes
				diretrizes abaixo:</p>

			<p>I - executar o Projovem Urbano por meio da sua Secretaria de
				Educação, que deverá coordenar o desenvolvimento das ações de
				implementação do Programa, garantindo a necessária articulação com a
				rede de ensino, conforme o Projeto Pedagógico Integrado, as
				orientações da Secretaria de Educação Continuada, Alfabetização,
				Diversidade e Inclusão – SECADI/MEC e de acordo com Resolução
				CD/FNDE Nº 54 de 21 de novembro de 2012;</p>
			<p>II – executar os recursos orçamentários repassados pelo Governo
				Federal exclusivamente na implementação do Programa, gerindo-os com
				eficiência, eficácia e transparência, visando a efetividade das
				ações;</p>
			<p>III - estabelecer como foco a aprendizagem, realizando todos os
				esforços necessários para garantir a certificação em Ensino
				Fundamental – EJA e em qualificação profissional como formação
				inicial dos jovens matriculados no Projovem Urbano;</p>
			<p>IV - responsabilizar-se pela divulgação do ProJovem Urbano em
				nível local, inclusive quanto aos processos de matrícula a serem
				realizados pelo Ente Federado, mobilizando a comunidade e suas
				lideranças, os jovens, pais e responsáveis, bem como os meios
				políticos e administrativos;</p>
			<p>V - empreender esforços para viabilizar a expedição dos documentos
				necessários para a matrícula dos jovens a serem atendidos pelo
				Programa;</p>
			<p>VI - matricular os estudantes por meio de Sistema de Matrícula,
				Acompanhamento de Frequência e Certificação do Projovem Urbano
				disponibilizado pela Secretaria de Educação Continuada,
				Alfabetização, Diversidade e Inclusão - SECADI/MEC, sendo esta a
				única forma de garantir a inclusão dos jovens no Programa, bem como
				ser responsável pela fidedignidade das informações lançadas no
				referido sistema;
			
			<p>VII – garantir o acesso e as condições de permanência das pessoas
				público-alvo da educação especial ao Programa, por meio da oferta do
				atendimento educacional especializado e oferta de recursos e
				serviços de acessibilidade;</p>
			<p>VIII - desenvolver o Projeto Pedagógico Integrado do Programa em
				suas três dimensões, garantindo sua execução conforme legislação do
				Projovem Urbano e orientações da Secretaria de Educação Continuada,
				Alfabetização, Diversidade e Inclusão - SECADI/MEC;</p>
			<p>IX - acompanhar cada beneficiário do ProJovem Urbano,
				individualmente, mediante registro mensal de frequência e de entrega
				de trabalhos, por meio do Sistema de Matrícula, Acompanhamento da
				Frequência e Certificação do Projovem Urbano;</p>
			<p>X - prevenir e combater a evasão pelo acompanhamento individual
				das razões para a não-frequência do educando e implantar medidas
				para superá-las;</p>
			<p>XI - concordar integralmente com os termos da Resolução CD/FNDE Nº
				54 de 21 de novembro de 2012, publicada no Diário Oficial da União
				em 22 de novembro de 2012, que estabelece os critérios e as normas
				de transferência automática de recursos financeiros do ProJovem
				Urbano para a execução das ações do Programa;</p>
			<p>XII - autorizar o FNDE/MEC a estornar ou bloquear valores
				creditados indevidamente na conta corrente do Programa em favor do
				Ente Federado, mediante solicitação direta ao agente financeiro
				depositário dos recursos ou procedendo ao desconto nas parcelas
				subsequentes;</p>
			<p>XIII - restituir ao FNDE/MEC, no prazo de dez dias úteis a contar
				do recebimento da notificação e na forma prevista nos §§ 17 a 20 do
				art. 18 da referida Resolução, os valores creditados indevidamente
				ou objeto de eventual irregularidade constatada, quando inexistir
				saldo suficiente na conta corrente e não houver repasses futuros a
				serem efetuados;</p>
			<p>XIV - atualizar junto à Secretaria de Educação Continuada,
				Alfabetização, Diversidade e Inclusão/MEC, as informações prestadas
				no Plano de Implementação do Programa, sob pena de suspensão de
				pagamento de parcelas subsequentes até a regularização da
				atualização dessas informações;</p>
			<p>XV - Aplica-se ao presente termo de adesão o previsto no art. 30,
				§ 5º do Decreto n.º 6.629/2008.</p>


			<h3>CLÁUSULA TERCEIRA – DAS OBRIGAÇÕES DO ESTADO/DISTRITO FEDERAL:</h3>

			<p>1. O Estado/Distrito Federal se obriga a:</p>
			<p>1. Atingir a seguinte meta de atendimento de jovens para o
				Projovem Urbano, edição 2013:</p>

			<table border=1 align=center width=30%>
				<tr>
					<td colspan="4"><center>
							<b>Meta 2013</b>
						</center></td>
				</tr>
				<tr>
					<td align="center"><b>Meta Total</b></td>
					<td align="center"><b>Público Juventude Viva (anexo II)*</b></td>
					<td align="center"><b>Público Unidades Prisionais</b></td>
					<td align="center"><b>Público Geral</b></td>
				</tr>
				<tr>
					<td align="center">Não Informado</td>
					<td align="center">Não Informado</td>
					<td align="center">Não informado</td>
					<td align="center">Não Informado</td>
				</tr>
			</table>
			<p>
			
			<center>(*) Anexo II da Resolução Nº 54/2012.</center>
			</p>
			<p>1.2. Cumprir as seguintes diretrizes:</p>

			<p>I – priorizar o atendimento aos jovens residentes nos municípios
				integrantes do Plano Juventude Viva, das políticas de enfrentamento
				à violência e das regiões impactadas pelas grandes obras do Governo
				Federal, bem como aos jovens catadores de resíduos sólidos;</p>
			<p>II – priorizar o atendimento às jovens mulheres, no caso da oferta
				em unidades do sistema prisional;</p>
			<p>III - garantir o funcionamento do comitê gestor do Programa no
				âmbito local, sob coordenação da Secretaria de Educação, composto
				pelo Conselho de Juventude, por órgãos de políticas de juventude,
				quando existirem na localidade, bem como pelas demais secretarias e
				órgãos afins, além de representação da Agenda de Desenvolvimento
				Integrado de Alfabetização e Educação de Jovens e Adultos, observada
				a intersetorialidade necessária para a execução das ações previstas
				pelo Programa;</p>
			<p>IV - garantir a oferta de Educação de Jovens e Adultos –
				EJA/Ensino Médio aos jovens atendidos pelo Programa nas escolas de
				sua rede, proporcionando a continuidade de seus estudos.</p>


			<h3>CLÁUSULA QUARTA – DAS OBRIGAÇÕES DO MUNICÍPIO</h3>

			<p>1. O Município se compromete a:</p>

			<p>1.1 Atingir a seguinte meta de atendimento de jovens para o
				Projovem Urbano, edição 2013:</p>
<?php
		if (! $dados ['ajustado']) {
			?>
                <table border=1 align=center width=30%>
				<tr>
					<td align="center" colspan="2"><b>Meta 2013</b></td>
				</tr>
				<tr>
					<td align="center"><b>Público Juventude Viva (anexo II) *</b></td>
					<td align="center"><b>Público Geral</b></td>
				</tr>
				<tr>
					<td align="center"><?=$rsMetas['juventudeviva'] ? $rsMetas['juventudeviva'] : '0'; ?></td>
					<td align="center"><?= $rsMetas['publicogeral'] ? $rsMetas['publicogeral'] 	: '0'; ?></td>
				</tr>
			</table>
			<p>
			
			<center>(*) Anexo II da Resolução Nº 54/2012.</center>
			</p>
<?php
		} else {
			?>
                <table border=1 align=center width=30%>
				<tr>
					<td align="center" colspan="2"><b>Meta 2013</b></td>
				</tr>
				<tr>
					<td align="center"><b>Público Juventude Viva (anexo II) *</b></td>
					<td align="center"><b>Público Geral</b></td>
				</tr>
				<tr>
					<td align="center"><?= $rsMetas['juventudevivaa'] 	? $rsMetas['juventudevivaa'] 	: '0'; ?></td>
					<td align="center"><?= $rsMetas['publicogerala'] 	? $rsMetas['publicogerala'] 	: '0'; ?></td>
				</tr>
			</table>
			<p>
			
			<center>(*) Anexo II da Resolução Nº 54/2012.</center>
			</p>
<?php
		}
		?>
            <p>1.2. Cumprir as seguintes diretrizes abaixo:</p>
			<p>I – priorizar o atendimento nas escolas localizadas nas regiões
				impactadas por grandes obras do Governo Federal, nas regiões com
				maiores índices de violência contra a juventude negra e nas áreas de
				abrangência das políticas de enfrentamento à violência, bem como
				atender aos jovens catadores de resíduos sólidos</p>
			<p>II – priorizar o atendimento às jovens mulheres, no caso da oferta
				em unidades do sistema prisional;</p>
			<p>III - garantir o funcionamento do comitê gestor do Programa no
				âmbito local, sob coordenação da Secretaria de Educação, composto
				pelo Conselho de Juventude, por órgãos de políticas de juventude,
				quando existirem na localidade, bem como pelas demais secretarias e
				órgãos afins, observada a intersetorialidade necessária para a
				execução das ações previstas pelo Programa;</p>
			<p>IV - articular-se com as redes estaduais de ensino visando
				garantir a continuidade de estudos para os jovens atendidos pelo
				Programa.</p>


			<h3>CLÁUSULA QUINTA – DA RESCISÃO</h3>

			<p>O presente instrumento poderá ser denunciado a qualquer tempo, no
				interesse das partes, ou rescindido pelo não cumprimento das
				cláusulas e/ou condições, observado o disposto nos artigos 77 a 80
				da Lei nº 8.666, de 21 de junho de 1993, e o Decreto nº 6.170, 25 de
				julho de 2007, no que couber, independentemente de interpelação
				judicial ou extrajudicial ou daquelas dispostas nos artigos 86 a 88
				do mesmo diploma legal.</p>


			<h3>CLÁUSULA SEXTA – DA PUBLICAÇÃO</h3>

			<p>Caberá à Secretaria de Educação Continuada, Alfabetização,
				Diversidade e Inclusão - SECADI/MEC proceder à publicação do
				presente Termo de Adesão no Diário Oficial da União – DOU, conforme
				estabelecido no parágrafo único do art. 61 da Lei nº 8.666, de 21 de
				junho de 1993.</p>


			<h3>CLÁUSULA SÉTIMA – DO FORO</h3>

			<p>O foro competente para dirimir qualquer questão relativa a
				instrumento é o da Justiça Federal, Foro da cidade de Brasília/DF,
				Seção Judiciária do Distrito Federal.</p> <br> <br> <br>
<?
		if ($dados ['ajustado']) :
			$adesaotermoajustadadata = $db->pegaLinha ( "SELECT to_char(adesaotermoajustadodata,'dd') as dia, to_char(adesaotermoajustadodata,'mm') as mes, to_char(adesaotermoajustadodata,'YYYY') as ano FROM projovemurbano.projovemurbano WHERE pjuid=" . $_SESSION ['projovemurbano'] ['pjuid'] );
			?>
                <p align=center>___________________________________, <?= (($adesaotermoajustadadata['dia']) ? $adesaotermoajustadadata['dia'] : date("d")) . " de " . $db->pegaUm("SELECT mesdsc FROM public.meses WHERE mescod='" . (($adesaotermoajustadadata['mes']) ? $adesaotermoajustadadata['mes'] : date("m")) . "'") . " de " . (($adesaotermoajustadadata['ano']) ? $adesaotermoajustadadata['ano'] : date("Y")) ?></p>

		 
		 <?
else :
			$adesaotermodata = $db->pegaLinha ( "SELECT to_char(adesaotermodata,'dd') as dia, to_char(adesaotermodata,'mm') as mes, to_char(adesaotermodata,'YYYY') as ano FROM projovemurbano.projovemurbano WHERE pjuid=" . $_SESSION ['projovemurbano'] ['pjuid'] );
			?>
                <p align=center>___________________________________, <?= (($adesaotermodata['dia']) ? $adesaotermodata['dia'] : date("d")) . " de " . $db->pegaUm("SELECT mesdsc FROM public.meses WHERE mescod='" . (($adesaotermodata['mes']) ? $adesaotermodata['mes'] : date("m")) . "'") . " de " . (($adesaotermodata['ano']) ? $adesaotermodata['ano'] : date("Y")) ?></p>

		
		<?
endif;
		?>
                <br> <br>
			<p>
			
			<center>
				___________________________________________________________________
				</p>
				<p>
				
				
				<center>
					<b>Secretário(a) Municipal/Estadual/Distrital de Educação</b>
					</p>
					<br> <br>

					<p align=center>
						<b>JOSÉ HENRIQUE PAIM FERNANDES</b>
				
				</center>
				</p>
				<p align=center>Ministro de Estado da Educação
			
			</center>
			</p>

	 <?
	
} else {
		?>
		            <h1 style="text-align: center">MINISTÉRIO DA EDUCAÇÃO</h1>
			<h2 style="text-align: center">GABINETE DO MINISTRO</h2>
			<h2 style="text-align: center">TERMO DE ADESÃO</h2>

			<p>
				O Distrito Federal/Estado/Município de <b><?= $dadosT['mundescricao'] ?></b>,
				doravante denominado Ente Federado, por meio da sua Secretaria de
				Educação, CNPJ:­­­­­<b><?php echo formatar_cnpj($rsSecretaria['entnumcpfcnpj']); ?></b>,
				representado por seu (sua) Secretário(a), <b><?= $dados['usunome'] ?></b>,
				CPF nº <b><?= mascaraglobal($dados['usucpf'], "###.###.###-##") ?></b>,
				RG nº <b><?= $dados['iserg'] ?></b>, expedido por <b><?= $dados['iseorgexp'] ?></b>,
				com atribuição legal para representar o governador ou o prefeito
				neste ato e devidamente estabelecido à<b><?= $dados['iseendereco'] . ", nº " . $dados['isenumero'] . ", " . $dados['isebairro'] . ", " . $db->pegaUm("SELECT mundescricao FROM territorios.municipio WHERE muncod='" . $dados['isemunicipio'] . "'") . ", " . $dados['iseuf']. ", " ?></b><b>CEP<?= mascaraglobal($dados['isecep'], "#####-###")?></b>, 
                e o Ministério da Educação, representado pelo Ministro de Estado, resolvem firmar o presente Termo de Adesão ao Programa Nacional de Inclusão de Jovens – Projovem Urbano e/ou Projovem Campo - Saberes da Terra,  
                edição <?= $_SESSION['projovemurbano']['ppuano'] ?>, em conformidade, no que couber, com a Lei n.º 8.666, de 21 de junho de 1993, e a legislação correlata, consideradas as seguintes condições:
            </p>

			<h3>CLÁUSULA PRIMEIRA – Do objeto</h3>

			<p>1. O presente termo tem por objeto a adesão do Ente Federado ao
				Programa Nacional de Inclusão de Jovens – Projovem Urbano e/ou
				Projovem Campo - Saberes da Terra, instituído nos termos da Lei nº
				11.692 de 10 de junho de 2008, regulamentado pelo Decreto nº 6.629
				de 4 de novembro de 2008 e pelo Decreto nº 7.649 de 21 de dezembro
				de 2011.</p>

			<h3>CLÁUSULA SEGUNDA – DAS OBRIGAÇÕES DOS ENTES FEDERADOS:</h3>

			<p>1. Os Entes Federados se comprometem a cumprir as seguintes
				diretrizes abaixo:</p>

			<p>I -executar o Programa, por meio da sua secretaria de Educação,
				que deverá coordenar o desenvolvimento das ações de implementação do
				Programa, garantindo a necessária articulação com a rede de ensino,
				conforme seus Projetos Pedagógicos Integrados, as orientações da
				Secretaria de Educação Continuada, Alfabetização, Diversidade e
				Inclusão – SECADI/MEC e de acordo com as Resoluções CD/FNDE/MEC Nº
				8/2014 e Nº 11/2014;</p>
			<p>II – executar os recursos orçamentários repassados pelo Governo
				Federal exclusivamente na implementação do Programa, gerindo-os com
				eficiência, eficácia e transparência, visando a efetividade das
				ações;</p>
			<p>III - estabelecer como foco a aprendizagem, realizando todos os
				esforços necessários para garantir a certificação em Ensino
				Fundamental – EJA e em qualificação profissional como formação
				inicial dos jovens matriculados no Programa;</p>
			<p>IV - responsabilizar-se pela divulgação do Programa em nível
				local, inclusive quanto aos processos de matrícula a serem
				realizados pelo Ente Federado, mobilizando a comunidade e suas
				lideranças, os jovens, pais e responsáveis, bem como os meios
				políticos e administrativos;</p>
			<p>V - empreender esforços para viabilizar a expedição dos documentos
				necessários para a matrícula dos jovens a serem atendidos pelo
				Programa;</p>
			<p>VI -matricular os estudantes por meio de Sistema de Matrícula,
				Acompanhamento de Frequência e Certificação do Projovem Urbano e
				Campo disponibilizado pela Secretaria de Educação Continuada,
				Alfabetização, Diversidade e Inclusão - SECADI/MEC, sendo esta a
				única forma de garantir a inclusão dos jovens no Programa, bem como
				ser responsável pela fidedignidade das informações lançadas no
				referido sistema;
			
			<p>VII – garantir o acesso e as condições de permanência das pessoas
				público-alvo da educação especial ao Programa, por meio da oferta do
				atendimento educacional especializado e oferta de recursos e
				serviços de acessibilidade;</p>
			<p>VIII - desenvolver os Projetos Pedagógicos Integrados das duas
				modalidades do Programa em suas três dimensões, garantindo sua
				execução conforme legislação do Projovem Urbano e do Projovem Campo
				– Saberes da Terra e orientações da Secretaria de Educação
				Continuada, Alfabetização, Diversidade e Inclusão - SECADI/MEC;</p>
			<p>IX - acompanhar cada beneficiário individualmente, no caso do
				Projovem Urbano, mediante registro mensal de frequência e de entrega
				de trabalhos, e no caso do Projovem Campo – Saberes da Terra,
				mediante registro mensal de frequência por meio do Sistema de
				Matrícula, Acompanhamento da Frequência e Certificação do Projovem
				Urbano e Campo;</p>
			<p>X - prevenir e combater a evasão pelo acompanhamento individual
				das razões para a não frequência do educando e implantar medidas
				para superá-las;</p>
			<p>XI - concordar integralmente com os termos das Resoluções
				CD/FNDE/MEC Nº 8/2014 e Nº 11/2014 publicadas no Diário Oficial da
				União em 16 de abril de 2014, que estabelece os critérios e as
				normas de transferência automática de recursos financeiros do
				Projovem Urbano e do Projovem Campo – Saberes da Terra para a
				execução das ações do Programa;</p>
			<p>XII - autorizar o FNDE/MEC a estornar ou bloquear valores
				creditados indevidamente na conta corrente do Programa em favor do
				Ente Federado, mediante solicitação direta ao agente financeiro
				depositário dos recursos ou procedendo ao desconto nas parcelas
				subsequentes;</p>
			<p>XIII - restituir ao FNDE/MEC, no prazo de dez dias úteis a contar
				do recebimento da notificação e na forma prevista nas Resoluções
				CD/FNDE/MEC Nº 8/2014 e Nº 11/2014, os valores creditados
				indevidamente ou objeto de eventual irregularidade constatada,
				quando inexistir saldo suficiente na conta corrente e não houver
				repasses futuros a serem efetuados;</p>
			<p>XIV - aplica-se ao presente termo de adesão o previsto no art. 30,
				§ 5º e no art. 36, § 4º do Decreto nº 6.629/2008.</p> <!--             <p>XV - Aplica-se ao presente termo de adesão o previsto no art. 30, § 5º do Decreto n.º 6.629/2008.</p> -->


			<h3>CLÁUSULA TERCEIRA – DAS OBRIGAÇÕES DO ESTADO/DISTRITO FEDERAL:</h3>

			<p>1. O Estado/Distrito Federal se obriga a:</p>
			<p>1. Atingir a seguinte meta de atendimento de jovens para o
				Projovem Urbano e/ou Projovem Campo – Saberes da Terra, edição 2014:</p>

			<table border=1 align=center width=30%>
				<tr>
					<td colspan="4"><center>
							<b>Meta 2014</b>
						</center></td>
				</tr>
				<tr>
					<td align="center"><b>Meta Total</b></td>
					<td align="center"><b>Público Juventude Viva (anexo II)*</b></td>
					<td align="center"><b>Público Unidades Prisionais</b></td>
					<td align="center"><b>Público Geral</b></td>
				</tr>
				<tr>
					<td align="center">Não Informado</td>
					<td align="center">Não Informado</td>
					<td align="center">Não informado</td>
					<td align="center">Não Informado</td>
				</tr>
			</table>
			<p>
			
			<center>(*) Anexo II da Resolução Nº 54/2012.</center>
			</p>
			<p>1.2. Cumprir as seguintes diretrizes:</p>

			<p>I - priorizar o atendimento aos jovens residentes nos municípios
				integrantes do Plano Juventude Viva, das políticas de enfrentamento
				à violência e das regiões impactadas pelas grandes obras do Governo
				Federal, bem como aos jovens catadores de resíduos sólidos e
				egressos do Programa Brasil Alfabetizado;</p>
			<p>II - priorizar o atendimento às jovens mulheres, no caso da oferta
				em unidades do sistema prisional;</p>
			<p>III - garantir o funcionamento do comitê gestor do Projovem
				Urbano, no âmbito local, sob coordenação da Secretaria de Educação,
				composto por representação do Conselho de Juventude, quando existir
				na localidade, dos órgãos de políticas de juventude, das políticas
				para mulheres, da promoção da igualdade racial, dos jovens
				participantes no Programa, das demais secretarias afins, além da
				Agenda de Desenvolvimento Integrado de Alfabetização e Educação de
				Jovens e Adultos, para garantir efetividade ao acompanhamento e
				apoio à execução das ações do Programa, observada a
				intersetorialidade necessária para a execução dessas ações;</p>
			<p>IV - garantir o funcionamento do comitê gestor do Projovem Campo –
				Saberes da Terra, no âmbito local, sob coordenação da Secretaria de
				Educação, composto por representação do Conselho de Juventude,
				quando existir na localidade, dos órgãos locais de políticas de
				juventude, dos movimentos sociais do campo e dos colegiados
				territoriais, bem como do órgão local de políticas para mulheres, de
				promoção da igualdade racial, dos jovens participantes no Programa,
				das demais secretarias afins e da Agenda de Desenvolvimento
				Integrado de Alfabetização e Educação de Jovens e Adultos e dos
				Comitês, Fóruns e/ou Articulações Estaduais de Educação do Campo,
				para garantir efetividade ao acompanhamento e apoio à execução das
				ações do Programa, observada a intersetorialidade necessária para a
				execução dessas ações;</p>
			<p>V - assegurar que 50% dos membros do comitê gestor local do
				Projovem Campo – Saberes da Terra seja de representantes das
				entidades que compõem os Comitês, Fóruns e/ou Articulações Estaduais
				de Educação do Campo;</p>
			<p>VI - garantir a oferta de Educação de Jovens e Adultos –
				EJA/Ensino Médio aos jovens atendidos pelo Programa nas escolas de
				sua rede, proporcionando a continuidade de seus estudos.</p>


			<h3>CLÁUSULA QUARTA – DAS OBRIGAÇÕES DO MUNICÍPIO</h3>

			<p>1. O Município se compromete a:</p>

			<p>1.1 Atingir a seguinte meta de atendimento de jovens para o
				Projovem Urbano e/ou Projovem Campo, edição 2014:</p>
<?php
		if (! $dados ['ajustado']) {
			?>
                <table border=1 align=center width=30%>
				<tr>
					<td align="center" colspan="2"><b>Meta 2014</b></td>
				</tr>
				<tr>
					<td align="center"><b>Público Juventude Viva (anexo II) *</b></td>
					<td align="center"><b>Público Geral</b></td>
				</tr>
				<tr>
					<td align="center"><?=$rsMetas['juventudeviva'] ? $rsMetas['juventudeviva'] : '0'; ?></td>
					<td align="center"><?= $rsMetas['publicogeral'] ? $rsMetas['publicogeral'] 	: '0'; ?></td>
				</tr>
			</table>
			<p>
			
			<center>(*) Anexo II da Resolução Nº 54/2012.</center>
			</p>
<?php
		} else {
			?>
                <table border=1 align=center width=30%>
				<tr>
					<td align="center" colspan="2"><b>Meta 2014</b></td>
				</tr>
				<tr>
					<td align="center"><b>Público Juventude Viva (anexo II) *</b></td>
					<td align="center"><b>Público Geral</b></td>
				</tr>
				<tr>
					<td align="center"><?= $rsMetas['juventudevivaa'] 	? $rsMetas['juventudevivaa'] 	: '0'; ?></td>
					<td align="center"><?= $rsMetas['publicogerala'] 	? $rsMetas['publicogerala'] 	: '0'; ?></td>
				</tr>
			</table>
			<p>
			
			<center>(*) Anexo II da Resolução Nº 54/2012.</center>
			</p>
<?php
		}
		?>
            <p>1.2. Cumprir as seguintes diretrizes abaixo:</p>
			<p>I - priorizar o atendimento aos jovens residentes nos municípios
				integrantes do Plano Juventude Viva, das políticas de enfrentamento
				à violência e das regiões impactadas pelas grandes obras do Governo
				Federal, bem como aos jovens catadores de resíduos sólidos e
				egressos do Programa Brasil Alfabetizado;</p>
			<p>II - priorizar o atendimento às jovens mulheres, no caso da oferta
				em unidades do sistema prisional;</p>
			<p>III - garantir o funcionamento do comitê gestor do Projovem
				Urbano, no âmbito local, sob coordenação da Secretaria de Educação,
				composto por representação do Conselho de Juventude, quando existir
				na localidade, dos órgãos de políticas de juventude, das políticas
				para mulheres, da promoção da igualdade racial, dos jovens
				participantes no Programa, das demais secretarias afins, além da
				Agenda de Desenvolvimento Integrado de Alfabetização e Educação de
				Jovens e Adultos, para garantir efetividade ao acompanhamento e
				apoio à execução das ações do Programa, observada a
				intersetorialidade necessária para a execução dessas ações;</p>
			<p>IV - garantir o funcionamento do comitê gestor do Projovem Campo –
				Saberes da Terra, no âmbito local, sob coordenação da Secretaria de
				Educação, composto por representação do Conselho de Juventude,
				quando existir na localidade, dos órgãos locais de políticas de
				juventude, dos movimentos sociais do campo e dos colegiados
				territoriais, bem como do órgão local de políticas para mulheres, de
				promoção da igualdade racial, dos jovens participantes no Programa,
				das demais secretarias afins e da Agenda de Desenvolvimento
				Integrado de Alfabetização e Educação de Jovens e Adultos e dos
				Comitês, Fóruns e/ou Articulações Estaduais de Educação do Campo,
				para garantir efetividade ao acompanhamento e apoio à execução das
				ações do Programa, observada a intersetorialidade necessária para a
				execução dessas ações;</p>
			<p>V - assegurar que 50% dos membros do comitê gestor local do
				Projovem Campo – Saberes da Terra seja de representantes das
				entidades que compõem os Comitês, Fóruns e/ou Articulações Estaduais
				de Educação do Campo;</p>
			<p>VI - garantir a oferta de Educação de Jovens e Adultos –
				EJA/Ensino Médio aos jovens atendidos pelo Programa nas escolas de
				sua rede, proporcionando a continuidade de seus estudos.</p>


			<h3>CLÁUSULA QUINTA – DA RESCISÃO</h3>

			<p>O presente instrumento poderá ser denunciado a qualquer tempo, no
				interesse das partes, ou rescindido pelo não cumprimento das
				cláusulas e/ou condições, observado o disposto nos artigos 77 a 80
				da Lei nº 8.666, de 21 de junho de 1993, no que couber,
				independentemente de interpelação judicial ou extrajudicial ou
				daquelas dispostas nos artigos 86 a 88 do mesmo diploma legal.</p>


			<h3>CLÁUSULA SEXTA – DA PUBLICAÇÃO</h3>

			<p>Caberá à Secretaria de Educação Continuada, Alfabetização,
				Diversidade e Inclusão - SECADI/MEC proceder à publicação do
				presente Termo de Adesão no Diário Oficial da União – DOU, conforme
				estabelecido no parágrafo único do art. 61 da Lei nº 8.666, de 21 de
				junho de 1993.</p>


			<h3>CLÁUSULA SÉTIMA – DO FORO</h3>

			<p>O foro competente para dirimir qualquer questão relativa a
				instrumento é o da Justiça Federal, Foro da cidade de Brasília/DF,
				Seção Judiciária do Distrito Federal.</p> <br> <br> <br>
<?
		if ($dados ['ajustado']) :
			$adesaotermoajustadadata = $db->pegaLinha ( "SELECT to_char(adesaotermoajustadodata,'dd') as dia, to_char(adesaotermoajustadodata,'mm') as mes, to_char(adesaotermoajustadodata,'YYYY') as ano FROM projovemurbano.projovemurbano WHERE pjuid=" . $_SESSION ['projovemurbano'] ['pjuid'] );
			?>
                <p align=center>___________________________________, <?= (($adesaotermoajustadadata['dia']) ? $adesaotermoajustadadata['dia'] : date("d")) . " de " . $db->pegaUm("SELECT mesdsc FROM public.meses WHERE mescod='" . (($adesaotermoajustadadata['mes']) ? $adesaotermoajustadadata['mes'] : date("m")) . "'") . " de " . (($adesaotermoajustadadata['ano']) ? $adesaotermoajustadadata['ano'] : date("Y")) ?></p>

		 
		 <?
else :
			$adesaotermodata = $db->pegaLinha ( "SELECT to_char(adesaotermodata,'dd') as dia, to_char(adesaotermodata,'mm') as mes, to_char(adesaotermodata,'YYYY') as ano FROM projovemurbano.projovemurbano WHERE pjuid=" . $_SESSION ['projovemurbano'] ['pjuid'] );
			?>
                <p align=center>___________________________________, <?= (($adesaotermodata['dia']) ? $adesaotermodata['dia'] : date("d")) . " de " . $db->pegaUm("SELECT mesdsc FROM public.meses WHERE mescod='" . (($adesaotermodata['mes']) ? $adesaotermodata['mes'] : date("m")) . "'") . " de " . (($adesaotermodata['ano']) ? $adesaotermodata['ano'] : date("Y")) ?></p>

		
		<?
endif;
		?>
                <br> <br>
			<p>
			
			<center>
				___________________________________________________________________
				</p>
				<p>
				
				
				<center>
					<b>Secretário(a) Municipal/Estadual/Distrital de Educação</b>
					</p>
					<br> <br>

					<p align=center>
						<b>JOSÉ HENRIQUE PAIM FERNANDES</b>
				
				</center>
				</p>
				<p align=center>Ministro de Estado da Educação
			
			</center>
			</p>
            
	<?
	
}
	?>
    	</td>
	</tr>
</table>
<?
}
function popUpFormula() {
	?>
<html>
<head>
<title>SIMEC- Sistema Integrado de Monitoramento do Ministério da
	Educação</title>
<script type="text/javascript" src="../includes/funcoes.js"></script>
<script type="text/javascript" src="../includes/prototype.js"></script>
<script type="text/javascript" src="../includes/entidades.js"></script>
<script type="text/javascript" src="/includes/estouvivo.js"></script>
<script src="/emi/geral/js/emi.js"></script>
<link rel="stylesheet" type="text/css" href="../includes/Estilo.css" />
<link rel="stylesheet" type="text/css" href="../includes/listagem.css" />
</head>
<body>
		<?php monta_titulo("Fórmula",""); ?>
		<table class="tabela" cellSpacing="1" cellPadding="3" align="center">
		<tr>
			<td class="SubtituloDireita" width="25%">Legenda</td>
			<td>
				<p>VR1 = Valor da 1 parcela</p>
				<p>EM1 = Meta Prevista</p>
				<p>m = 6 meses de curso</p>
				<p>p1 = Percentual para pagamento de pessoal</p>
				<p>p2 = Percentual para formação continuada</p>
				<p>p3 = Percentual para o auxílio financeiro da 1° etapa de formação</p>
				<p>p4 = Percentual para aquisição de gêneros alimentícios</p>
				<p>p5 = Percentual para execução dos arcos ocupacionais</p>
				<? if($_SESSION['projovemurbano']['estuf']) : ?>
				<p>p7 = Percentual para transporte de material didático</p>
				<? endif; ?>
				<p>R$ 54,00 = Adicional para elaboração e aplicação das ..provas</p>
			</td>
		</tr>
		<tr>
			<td class="SubtituloDireita">Fórmula</td>
			<td>
				<?
	if ($_SESSION ['projovemurbano'] ['estuf'])
		$vlr = "170,00";
	if ($_SESSION ['projovemurbano'] ['muncod'])
		$vlr = "165,00";
	?>
				VR1 = EM1 X [(p1 X m X R$ <?=$vlr ?>) + (p2 X m X R$ <?=$vlr ?>) + (p3 X 18 X R$ <?=$vlr ?>) + (p4 X m X R$ <?=$vlr ?>) + (p5 X m X R$ <?=$vlr ?>) <? if($_SESSION['projovemurbano']['estuf']) : ?>+ (p6 X 18 X R$ <?=$vlr ?>)<? endif; ?>] + (EM1 X R$ 54,00)
				</td>
		</tr>
		<tr>
			<td colspan="2" style="text-align: center" class="SubtituloDireita"><input
				type="button" name="btn_fechar" value="Fechar"
				onclick="window.close()" /></td>
		</tr>
	</table>
</body>
</html>
<?php
}
function popUpFormula2014() {
	?>
<html>
<head>
<title>SIMEC- Sistema Integrado de Monitoramento do Ministério da
	Educação</title>
<script type="text/javascript" src="../includes/funcoes.js"></script>
<script type="text/javascript" src="../includes/prototype.js"></script>
<script type="text/javascript" src="../includes/entidades.js"></script>
<script type="text/javascript" src="/includes/estouvivo.js"></script>
<script src="/emi/geral/js/emi.js"></script>
<link rel="stylesheet" type="text/css" href="../includes/Estilo.css" />
<link rel="stylesheet" type="text/css" href="../includes/listagem.css" />
</head>
<body>
		<?php monta_titulo("Fórmula",""); ?>
		<table class="tabela" cellSpacing="1" cellPadding="3" align="center">
		<tr>
			<td class="SubtituloDireita" width="25%">Legenda</td>
			<td>
				<? if($_SESSION['projovemurbano']['muncod']) : ?>
					<p>
					<strong>Vr1</strong> = Valor da 1 parcela
				</p>
				<p>
					<strong>MP1</strong> = Meta prevista para atendimento Meta Prevista
				</p>
				<p>
					<strong>6</strong> = Meses de curso
				</p>
				<p>
					<strong>89%</strong> = Soma dos percentuais referentes a:
					percentual para pagamento de pessoal, aquisição de gêneros
					alimentícios, qualificação profissional
				</p>
				<p>
					<strong>Vpc</strong> = Valor per capita
				</p>
				<p>
					<strong>18</strong> = Meses de curso
				</p>
				<p>
					<strong>1%</strong> = Percentual para pagamento de auxílio
					financeiro para formação
				</p>
				<p>
					<strong>12</strong> = Meses de formação
				</p>
				<p>
					<strong>p5</strong> = Percentual para execução dos arcos
					ocupacionais
				</p>
				<p>
					<strong>10%</strong> = Percentual para custeio da formação
					continuada
				</p>
				<p>
					<strong>R$ 54,00</strong> = Adicional para elaboração e aplicação
					das ..provas
				</p>
				<? else: ?>
					<p>
					<strong>Vr1</strong> = Valor da 1 parcela
				</p>
				<p>
					<strong>MP1</strong> = Meta prevista para atendimento Meta Prevista
				</p>
				<p>
					<strong>6</strong> = Meses de curso
				</p>
				<p>
					<strong>87,5%</strong> = Soma dos percentuais referentes a:
					percentual para pagamento de pessoal, aquisição de gêneros
					alimentícios, qualificação profissional
				</p>
				<p>
					<strong>Vpc</strong> = Valor per capita
				</p>
				<p>
					<strong>18</strong> = Meses de curso
				</p>
				<p>
					<strong>1,5%</strong> = Percentual para transporte de material
					didático(exclusivo para estados)
				</p>
				<p>
					<strong>1%</strong> = Percentual para pagamento de auxílio
					financeiro para formação
				</p>
				<p>
					<strong>12</strong> = Meses de formação
				</p>
				<p>
					<strong>p5</strong> = Percentual para execução dos arcos
					ocupacionais
				</p>
				<p>
					<strong>10%</strong> = Percentual para custeio da formação
					continuada
				</p>
				<p>
					<strong>R$ 54,00</strong> = Adicional para elaboração e aplicação
					das ..provas
				</p>
				<? endif;?>
			</td>
		</tr>
		<tr>
			<td class="SubtituloDireita">Fórmula</td>
			<td>
				<?
	if ($_SESSION ['projovemurbano'] ['estuf'])
		$vlr = "170,00";
	if ($_SESSION ['projovemurbano'] ['muncod'])
		$vlr = "165,00";
	?>
				Vr1 = MP1 X [<?if($_SESSION['projovemurbano']['muncod']):?>(6 X 89% X Vpc) <?endif;?><? if($_SESSION['projovemurbano']['estuf']) : ?> (6 X 87,5% X Vpc)+ (18 X 1,5% X Vpc)<? endif; ?> + (18 X 1% X Vpc) + (12 X 10% X Vpc)] + (MP1 X R$ 54,00)
				</td>
		</tr>
		<tr>
			<td colspan="2" style="text-align: center" class="SubtituloDireita"><input
				type="button" name="btn_fechar" value="Fechar"
				onclick="window.close()" /></td>
		</tr>
	</table>
</body>
</html>
<?php
}
function inserirCursosQualificacaoEstado($dados) {
	global $db;
	?>
<script language="JavaScript" src="../includes/funcoes.js"></script>
<link rel="stylesheet" type="text/css" href="../includes/Estilo.css" />
<link rel="stylesheet" type="text/css" href="../includes/listagem.css" />
<script>
	function salvarCursosEstado() {
		if(document.getElementById('muncod').value=='') {
			alert('Selecione um municipio');
			return false;
		}
		
		selectAllOptions( document.getElementById( 'cofid' ) );
		
		document.getElementById( 'form' ).submit();
	}	
	</script>
<form id="form" name="form" method="POST">
	<input type="hidden" name="requisicao"
		value="inserirMunicipioCursosEstado">
	<table class="tabela" cellSpacing="1" cellPadding="3" align="center">
		<tr>
			<td class="SubtituloDireita">Município</td>
			<td><? $db->monta_combo('muncod', "SELECT muncod as codigo, mundescricao as descricao FROM territorios.municipio WHERE estuf='".$_SESSION['projovemurbano']['estuf']."'", 'S', 'Selecione', '', '', '', '200', 'S', 'muncod', '', $arcids[0]); ?></td>
		</tr>
		<tr>
			<td class="SubtituloDireita">Cursos</td>
			<td><?
	$sql = "SELECT cofid as codigo, cofdesc as descricao FROM projovemurbano.cursoofertado WHERE cofstatus='A'";
	combo_popup ( "cofid", $sql, "Cursos", "192x400", 0, array (), "", "S", false, false, 5, 400 );
	?></td>
		</tr>
		<tr>
			<td colspan="2" class="SubtituloCentro"><input type="button"
				name="salvar" value="Salvar" onclick="salvarCursosEstado();" /></td>
		</tr>
	</table>
</form>
<?
}
function inserirMunicipioCursosEstado($dados) {
	global $db;
	if ($dados ['cofid']) {
		foreach ( $dados ['cofid'] as $cof ) {
			$sql = "INSERT INTO projovemurbano.cursoqualificacao(
				            qprid, cofid, muncod, cuqstatus)
				    VALUES ('" . $_SESSION ['projovemurbano'] ['qprid'] . "', 
				    		'" . $cof . "', 
				    		'" . $dados ['muncod'] . "', 'A');";
			
			$db->executar ( $sql );
		}
		
		$db->commit ();
	}
	
	echo "<script>
			alert('Gravado com sucesso');
			window.opener.carregarListaCursosEstado();
			window.close();
		  </script>";
}
function carregarListaCursosEstado($dados) {
	global $db;
	
	$sql = "SELECT '<center><img src=../imagens/excluir.gif style=cursor:pointer; onclick=\"excluirPoloMunicipio(\'projovemurbano.php?modulo=principal/planoImplementacao&acao=A&requisicao=excluirCursoMunicipioEstado&cuqid='||cuqid||'\');\"></center>' as acao, mun.mundescricao, cof.cofdesc FROM projovemurbano.cursoqualificacao cq 
			INNER JOIN territorios.municipio mun ON mun.muncod=cq.muncod 
			INNER JOIN projovemurbano.cursoofertado cof ON cof.cofid=cq.cofid 
			WHERE qprid='" . $_SESSION ['projovemurbano'] ['qprid'] . "'";
	
	$cabecalho = array (
			"&nbsp;",
			"Município",
			"Cursos" 
	);
	$db->monta_lista_simples ( $sql, $cabecalho, 50, 5, 'N', '100%', $par2 );
}
function excluirCursoMunicipioEstado($dados) {
	global $db;
	$db->executar ( "DELETE FROM projovemurbano.cursoqualificacao WHERE cuqid='" . $dados ['cuqid'] . "'" );
	$db->commit ();
	
	echo "<script>
			alert('Curso excluido com sucesso');
			window.location='projovemurbano.php?modulo=principal/planoImplementacao&acao=A&aba=qualificacaoProfissionalEstado';
		  </script>";
}
function criaSessaoProfissionais() {
	global $db;
	
	if ($_SESSION ['projovemurbano'] ['ppuid'] == '2' || $_SESSION ['projovemurbano'] ['ppuid'] == '3') {
		// $ano2013 = "AND tprid = {$_SESSION['projovemurbano']['tprid']}";
	} else {
		$_SESSION ['projovemurbano'] ['tprid'] = 'NULL';
	}
	$sql = "SELECT * FROM projovemurbano.profissionais WHERE pjuid='" . $_SESSION ['projovemurbano'] ['pjuid'] . "' $ano2013";
	$profissionais = $db->pegaLinha ( $sql );
	// ver ($sql);
	if (! $profissionais) {
		$sql = "INSERT INTO projovemurbano.profissionais(
	            pjuid, propercmax, propercutilizado, prostatus)
	    		VALUES ('" . $_SESSION ['projovemurbano'] ['pjuid'] . "', NULL, NULL, 'A') RETURNING proid;";
		
		$_SESSION ['projovemurbano'] ['proid'] = $db->pegaUm ( $sql );
		$db->commit ();
	} else {
		$_SESSION ['projovemurbano'] ['proid'] = $profissionais ['proid'];
	}
}
function criaSessaoFormacaoEducadores() {
	global $db;
	
	if ($_SESSION ['projovemurbano'] ['ppuid'] == 2 || $_SESSION ['projovemurbano'] ['ppuid'] == 3) {
		$filtroTprid = "AND tprid = {$_SESSION['projovemurbano']['tprid']}";
		$campoTprid = ", tprid";
		$valorTprid = ", {$_SESSION['projovemurbano']['tprid']}";
	}
	
	$sql = "SELECT * FROM projovemurbano.formacaoeducadores
         	WHERE 
         		pjuid = {$_SESSION['projovemurbano']['pjuid']}
           		$filtroTprid";
	
	$formacaoeducadores = $db->pegaLinha ( $sql );
	
	if (! $formacaoeducadores) {
		
		$sql = "INSERT INTO projovemurbano.formacaoeducadores(
	            pjuid, fedpercmax, fedperutilizado, fedqtd, fedstatus $campoTprid)
	    		VALUES ('" . $_SESSION ['projovemurbano'] ['pjuid'] . "', 
	    				NULL, 
	    				NULL, 
	    				NULL, 
	    				'A' $valorTprid) RETURNING fedid;";
		
		$_SESSION ['projovemurbano'] ['fedid'] = $db->pegaUm ( $sql );
		$db->commit ();
	} else {
		
		$_SESSION ['projovemurbano'] ['fedid'] = $formacaoeducadores ['fedid'];
	}
}
function criaSessaoGeneroAlimenticios() {
	global $db;
	
	if ($_SESSION ['projovemurbano'] ['ppuid'] == '2' || $_SESSION ['projovemurbano'] ['ppuid'] == '3') {
		$sql = "SELECT * FROM projovemurbano.generoalimenticio 
				WHERE pjuid='" . $_SESSION ['projovemurbano'] ['pjuid'] . "' AND tprid = {$_SESSION['projovemurbano']['tprid']}";
		$generoalimenticio = $db->pegaLinha ( $sql );
		
		if (! $generoalimenticio) {
			$sql = "INSERT INTO projovemurbano.generoalimenticio(
		            pjuid, tprid)
		    		VALUES ('" . $_SESSION ['projovemurbano'] ['pjuid'] . "', {$_SESSION['projovemurbano']['tprid']}) RETURNING galid;";
			
			$_SESSION ['projovemurbano'] ['galid'] = $db->pegaUm ( $sql );
			$db->commit ();
		} else {
			$_SESSION ['projovemurbano'] ['galid'] = $generoalimenticio ['galid'];
		}
	} else {
		$sql = "SELECT * FROM projovemurbano.generoalimenticio WHERE pjuid='" . $_SESSION ['projovemurbano'] ['pjuid'] . "'";
		$generoalimenticio = $db->pegaLinha ( $sql );
		
		if (! $generoalimenticio) {
			$sql = "INSERT INTO projovemurbano.generoalimenticio(
		            pjuid)
		    		VALUES ('" . $_SESSION ['projovemurbano'] ['pjuid'] . "') RETURNING galid;";
			
			$_SESSION ['projovemurbano'] ['galid'] = $db->pegaUm ( $sql );
			$db->commit ();
		} else {
			$_SESSION ['projovemurbano'] ['galid'] = $generoalimenticio ['galid'];
		}
	}
}
function criaSessaoDemaisAcoes() {
	global $db;
	
	if ($_SESSION ['projovemurbano'] ['ppuid'] == '2' || $_SESSION ['projovemurbano'] ['ppuid'] == '3') {
		
		$sql = "SELECT deaid FROM projovemurbano.demaisacoes 
				WHERE pjuid='" . $_SESSION ['projovemurbano'] ['pjuid'] . "' AND tprid = {$_SESSION['projovemurbano']['tprid']}";
		
		$demaisacoes = $db->pegaUm ( $sql );
		
		if (! $demaisacoes) {
			$sql = "INSERT INTO projovemurbano.demaisacoes(
			pjuid,tprid)
			VALUES ('" . $_SESSION ['projovemurbano'] ['pjuid'] . "',{$_SESSION['projovemurbano']['tprid']}) RETURNING deaid;";
			
			$_SESSION ['projovemurbano'] ['deaid'] = $db->pegaUm ( $sql );
			$db->commit ();
		} else {
			$_SESSION ['projovemurbano'] ['deaid'] = $demaisacoes;
		}
	} else {
		$sql = "SELECT deaid FROM projovemurbano.demaisacoes WHERE pjuid='" . $_SESSION ['projovemurbano'] ['pjuid'] . "'";
		$demaisacoes = $db->pegaUm ( $sql );
		
		if (! $demaisacoes) {
			$sql = "INSERT INTO projovemurbano.demaisacoes(
			pjuid)
			VALUES ('" . $_SESSION ['projovemurbano'] ['pjuid'] . "') RETURNING deaid;";
			
			$_SESSION ['projovemurbano'] ['deaid'] = $db->pegaUm ( $sql );
			$db->commit ();
		} else {
			$_SESSION ['projovemurbano'] ['deaid'] = $demaisacoes;
		}
	}
}
function criaSessaoQualificacaoProfissional() {
	global $db;
	
	if ($_SESSION ['projovemurbano'] ['ppuid'] == '2' || $_SESSION ['projovemurbano'] ['ppuid'] == '3') {
		$sql = "SELECT * FROM projovemurbano.qualificacaoprofissional 
				WHERE pjuid='" . $_SESSION ['projovemurbano'] ['pjuid'] . "' AND tprid = {$_SESSION['projovemurbano']['tprid']} ";
		$qualificacaoprofissional = $db->pegaLinha ( $sql );
		
		if (! $qualificacaoprofissional) {
			$sql = "INSERT INTO projovemurbano.qualificacaoprofissional(
		            pjuid,tprid)
		    		VALUES ('" . $_SESSION ['projovemurbano'] ['pjuid'] . "',{$_SESSION['projovemurbano']['tprid']}) RETURNING qprid;";
			
			$_SESSION ['projovemurbano'] ['qprid'] = $db->pegaUm ( $sql );
			$db->commit ();
		} else {
			$_SESSION ['projovemurbano'] ['qprid'] = $qualificacaoprofissional ['qprid'];
		}
	} else {
		$sql = "SELECT * FROM projovemurbano.qualificacaoprofissional WHERE pjuid='" . $_SESSION ['projovemurbano'] ['pjuid'] . "'";
		$qualificacaoprofissional = $db->pegaLinha ( $sql );
		
		if (! $qualificacaoprofissional) {
			$sql = "INSERT INTO projovemurbano.qualificacaoprofissional(
		            pjuid)
		    		VALUES ('" . $_SESSION ['projovemurbano'] ['pjuid'] . "') RETURNING qprid;";
			
			$_SESSION ['projovemurbano'] ['qprid'] = $db->pegaUm ( $sql );
			$db->commit ();
		} else {
			$_SESSION ['projovemurbano'] ['qprid'] = $qualificacaoprofissional ['qprid'];
		}
	}
}
function pegarCoordenadorGeral($proid) {
	global $db;
	$sql="SELECT * FROM projovemurbano.coordgeral coo LEFT JOIN projovemurbano.contratadorecurso cre ON cre.creid = coo.creid WHERE proid='" . $proid . "'" ;
	$coordG = $db->pegaLinha ($sql);
	return $coordG;
}
function pegarAssistentesCoordenadorGeral($proid, $tipo, $tprid = null) {
	global $db;
	$sql = "SELECT 
    			*
	  		FROM 
	  			projovemurbano.coordassistentes coo
	  		LEFT JOIN projovemurbano.contratadocomp ccm ON ccm.ccmrid = coo.ccmrid
	  		WHERE 
	  			coatipo = '{$tipo}'
	  			AND coastatus = 'A'
	    		AND proid='{$proid}'";
	if (! is_null ( $tprid )) {
		$sql .= " AND tprid = {$tprid}";
	}
	$assisCoorG = $db->pegaLinha ( $sql );
	return $assisCoorG;
}
function pegarNumeroPolos($agrupado = false) {
	global $db;
	
	$sql = "SELECT COUNT(*) " . ($agrupado ? ", tprid" : "") . "
                  FROM projovemurbano.polo pol 
                    INNER JOIN projovemurbano.polomunicipio plm ON plm.pmuid = pol.pmuid
                  WHERE plm.pmustatus='A'
                    AND pol.polstatus='A'
					AND plm.pmupossuipolo = 't'
                    AND plm.pjuid='" . $_SESSION ['projovemurbano'] ['pjuid'] . "'
                    -- Polos sao validos entre publicos
                    -- AND tprid = {$_SESSION['projovemurbano']['tprid']}
                  " . ($agrupado ? "GROUP BY tprid" : '');
	if ($agrupado) {
		return $db->carregar ( $sql );
	}
	
	$numeropolos = $db->pegaUm ( $sql );
	return $numeropolos;
}
function pegarDiretorPolo($proid) {
	global $db;
	
	$sql = "SELECT * FROM projovemurbano.diretorpolo dp 
		   LEFT JOIN projovemurbano.contratadocomp cc ON cc.ccmrid=dp.ccmrid 
		   WHERE dp.proid='" . $proid . "' AND dp.dipstatus='A'" ;
	$diretorPolo = $db->pegaLinha ($sql);
	
	return $diretorPolo;
}
function pegarAssistentesDiretorPolo($proid, $tipo) {
	global $db;
		
		$sql =  "SELECT * FROM projovemurbano.dirassistentes dir 
				  LEFT JOIN projovemurbano.contratadorecurso ctr ON dir.creid = ctr.creid
				  LEFT JOIN projovemurbano.contratadocomp ccm ON ccm.ccmrid = dir.ccmrid 
				  WHERE dir.proid='" . $proid . "' AND dasstatus='A' AND dastipo='" . $tipo .  "'" ;
		$assistentesDiretorPolo = $db->pegaLinha ($sql);
	return $assistentesDiretorPolo;
}
function pegarNucleos($possuipolo, $polid = null) {
	global $db;
	// ver($possuipolo);
	// Adaptação para o perfil Diretor do Núcleo
	if (! $db->testa_superuser ()) {
		$perfis = pegaPerfilGeral ();
		if (in_array ( PFL_DIRETOR_NUCLEO, $perfis )) {
			$inner_nucleo = "inner join projovemurbano.usuarioresponsabilidade ur on ur.usucpf='" . $_SESSION ['usucpf'] . "' and ur.entid=nes.entid AND rpustatus='A'";
		}
	}
	
	if ($possuipolo == "t") {
		
		$sql = "SELECT DISTINCT
					nuc.nucid as codigo, 
					'NÚCLEO '||nuc.nucid||', SEDE: '||(SELECT entnome FROM entidade.entidade ent INNER JOIN projovemurbano.nucleoescola nes ON nes.entid = ent.entid WHERE nes.nucid=nuc.nucid AND nes.nuetipo='S')||COALESCE(', ANEXO: '||(SELECT entnome FROM entidade.entidade ent INNER JOIN projovemurbano.nucleoescola nes ON nes.entid = ent.entid WHERE nes.nucid=nuc.nucid AND nes.nuetipo='A'),'') as descricao 
				FROM projovemurbano.nucleo nuc
				INNER JOIN projovemurbano.nucleoescola nes ON nes.nucid = nuc.nucid
				INNER JOIN projovemurbano.municipio mun ON mun.munid = nuc.munid 
				INNER JOIN projovemurbano.associamucipiopolo amp ON amp.munid = mun.munid    
				INNER JOIN projovemurbano.polo pol ON pol.polid = amp.polid 
				INNER JOIN projovemurbano.polomunicipio plm ON plm.pmuid = pol.pmuid 
				{$inner_nucleo} 
				WHERE 
					nuc.nucstatus='A' AND mun.munstatus='A' AND plm.pmustatus='A' 
					/*Retirado a pedido do Wallace - 08/05/2012*/
				  	/*AND nuc.nucid NOT IN (SELECT
				  							nuc2.nucid
				  						  FROM
				  						  	projovemurbano.nucleo nuc2
				  						  WHERE
				  						  	nuc2.nucqtdestudantes <= ( SELECT count(caeid) FROM projovemurbano.cadastroestudante cae WHERE cae.nucid = nuc2.nucid  ))*/
				  	AND pol.polstatus='A' " . (($_SESSION ['projovemurbano'] ['pjuid']) ? " AND pjuid='" . $_SESSION ['projovemurbano'] ['pjuid'] . "'" : "") . (($polid) ? " AND pol.polid='" . $polid . "'" : "");
		$nucleos = $db->carregar ( $sql );
	} else {
		
		$sql = "SELECT DISTINCT
					nuc.nucid as codigo, 
					'NÚCLEO '||nuc.nucid||', 
					SEDE: '||(SELECT entnome FROM entidade.entidade ent INNER JOIN projovemurbano.nucleoescola nes ON nes.entid = ent.entid WHERE nes.nucid=nuc.nucid AND nes.nuetipo='S')||
					COALESCE(', ANEXO:'||(SELECT entnome FROM entidade.entidade ent INNER JOIN projovemurbano.nucleoescola nes ON nes.entid = ent.entid WHERE nes.nucid=nuc.nucid AND nes.nuetipo='A'),'') as descricao 
				FROM 
					projovemurbano.nucleo nuc 
				INNER JOIN projovemurbano.nucleoescola nes ON nes.nucid = nuc.nucid
			    INNER JOIN projovemurbano.municipio mun ON mun.munid = nuc.munid 
			    INNER JOIN projovemurbano.polomunicipio plm ON plm.pmuid = mun.pmuid 
			    {$inner_nucleo}
			    WHERE 
			  		nuc.nucstatus='A' AND mun.munstatus='A' AND plm.pmustatus='A'
			  		/*Retirado a pedido do Wallace - 08/05/2012*/
			  		/*AND nuc.nucid NOT IN (SELECT
				  							nuc2.nucid
				  						  FROM
				  						  	projovemurbano.nucleo nuc2
				  						  WHERE
				  						  	nuc2.nucqtdestudantes <= ( SELECT count(caeid) FROM projovemurbano.cadastroestudante cae WHERE cae.nucid = nuc2.nucid  ))*/ 
			  		AND pjuid='" . $_SESSION ['projovemurbano'] ['pjuid'] . "'";
		$nucleos = $db->carregar ( $sql );
	}
	// ver( $sql );
	return $nucleos;
}
function contaEstudantesNucleos($possuipolo, $polid = null) {
	global $db;
	
	// Adaptação para o perfil Diretor do Núcleo
	if (! $db->testa_superuser ()) {
		$perfis = pegaPerfilGeral ();
		if (in_array ( PFL_DIRETOR_NUCLEO, $perfis )) {
			$inner_nucleo = "inner join projovemurbano.usuarioresponsabilidade ur on ur.usucpf='" . $_SESSION ['usucpf'] . "' and ur.entid=nes.entid AND rpustatus='A'";
		}
	}
	
	if ($possuipolo == "t") {
		
		$sql = "SELECT DISTINCT
					count(caeid) as qtd,
					nuc.nucqtdestudantes
				FROM projovemurbano.nucleo nuc
				INNER JOIN projovemurbano.cadastroestudante cae ON cae.nucid = nuc.nucid
				INNER JOIN projovemurbano.nucleoescola nes ON nes.nucid = nuc.nucid
				INNER JOIN projovemurbano.municipio mun ON mun.munid = nuc.munid 
				INNER JOIN projovemurbano.associamucipiopolo amp ON amp.munid = mun.munid    
				INNER JOIN projovemurbano.polo pol ON pol.polid = amp.polid 
				INNER JOIN projovemurbano.polomunicipio plm ON plm.pmuid = pol.pmuid 
				{$inner_nucleo} 
				WHERE 
					nuc.nucstatus='A' AND mun.munstatus='A' AND plm.pmustatus='A' 
				  	AND pol.polstatus='A' AND plm.pjuid='" . $_SESSION ['projovemurbano'] ['pjuid'] . "'" . (($polid) ? " AND pol.polid='" . $polid . "'" : "") . "
				GROUP BY
				  	nuc.nucqtdestudantes";
		
		$qtd = $db->pegaLinha ( $sql );
	} elseif($possuipolo == "f"){
		
		$sql = "SELECT DISTINCT
					count(caeid) as qtd,
					nuc.nucqtdestudantes
				FROM 
					projovemurbano.nucleo nuc 
				INNER JOIN projovemurbano.cadastroestudante cae ON cae.nucid = nuc.nucid
				INNER JOIN projovemurbano.nucleoescola nes ON nes.nucid = nuc.nucid
			    INNER JOIN projovemurbano.municipio mun ON mun.munid = nuc.munid 
			    INNER JOIN projovemurbano.polomunicipio plm ON plm.pmuid = mun.pmuid 
			    {$inner_nucleo}
			    WHERE 
			  		nuc.nucstatus='A' AND mun.munstatus='A' AND plm.pmustatus='A'
			  		AND plm.pjuid='" . $_SESSION ['projovemurbano'] ['pjuid'] . "'
			  	GROUP BY
			  		nuc.nucqtdestudantes";
		$qtd = $db->pegaLinha ( $sql );
	}else{
		$sql = "SELECT DISTINCT
					count(caeid) as qtd,
					nuc.nucqtdestudantes
				FROM
					rojovemurbano.nucleo nuc
				INNER JOIN projovemurbano.cadastroestudante cae ON cae.nucid = nuc.nucid
				INNER JOIN projovemurbano.nucleoescola nes ON nes.nucid = nuc.nucid
				INNER JOIN projovemurbano.municipio mun ON mun.munid = nuc.munid
				INNER JOIN projovemurbano.polomunicipio plm ON plm.pmuid = mun.pmuid
				{$inner_nucleo}
				WHERE
				nuc.nucstatus='A' AND mun.munstatus='A' AND plm.pmustatus='A'
				AND plm.pjuid='" . $_SESSION ['projovemurbano'] ['pjuid'] . "'
				GROUP BY
				nuc.nucqtdestudantes";
		$qtd = $db->pegaLinha ( $sql );
	}
	
	return $qtd ['qtd'] >= $qtd ['nucqtdestudantes'];
}
function buscarNucleos2($dados) {
	global $db;
	
	if ($dados ['polid']) {
		$nucleos = pegarNucleos ( "t", $dados ['polid'] );
		if (! $nucleos [0]) {
			if (contaEstudantesNucleos ( $polomunicipio ['pmupossuipolo'] )) {
				echo "Numero máximo de estudantes atingido.";
				die ();
			} else {
				echo "Não possui Nucleo";
				die ();
			}
		}
	} else {
		$nucleos = array ();
	}
	
	$db->monta_combo ( 'nucid', $nucleos, $dados ['bloq'], 'Selecione', 'buscarTurmas2', '', '', '', 'N', 'nucid' );
}
function verificaTurmaNucleo($nucid) {
	global $db;
	$sql = "SELECT true FROM projovemurbano.turma WHERE nucid = $nucid";
	$teste = $db->pegaUm ( $sql );
	
	if ($teste != 't') {
		$sql = "SELECT DISTINCT	entid as id, nueqtdturma as qtd, nuetipo FROM projovemurbano.nucleoescola WHERE nucid = $nucid ORDER BY nuetipo desc";
		$dados = $db->carregar ( $sql );
		
		if (is_array ( $dados )) {
			$sql = '';
			$y = 1;
			foreach ( $dados as $dado ) {
				for($x = 1; $x <= $dado ['qtd']; $x ++) {
					$sql .= "INSERT INTO projovemurbano.turma( turdesc, nucid, entid ) VALUES( 'Turma $y', $nucid, " . $dado ['id'] . " );";
					$y ++;
				}
			}
			// ver($dados,d);
			$db->executar ( $sql );
			$db->commit ();
		}
	}
}
function buscarTurmas($dados) {
	global $db;
// 	ver($dados,d);
	if ($dados ['com_vaga'] == 'true') {
		
		$sql_com_vaga = "AND t.turid NOT IN ( SELECT
                                                        turid
                                                FROM
                                                (
                                                    SELECT
                                                        count(caeid) as qtd,
                                                        tur.turid
                                                    FROM
                                                        projovemurbano.turma tur
                                                    INNER JOIN projovemurbano.cadastroestudante cae ON cae.turid = tur.turid AND cae.caestatus = 'A'
                                                    WHERE cae.nucid = {$dados['nucid']}
                                                    GROUP BY tur.turid
                                                ) as foo
                                                WHERE
                                                    qtd >= 40)";
// 		ver($sql_com_vaga,d);
	}
	
	verificaTurmaNucleo ( $dados ['nucid'] );
	
	if (! $db->testa_superuser ()) {
		$perfis = pegaPerfilGeral ();
		
		if (in_array ( 650, $perfis )) {
			if ($_SESSION ['projovemurbano'] ['entid']) {
				$escola_diretor = "t.entid = " . $_SESSION ['projovemurbano'] ['entid'] . " AND ";
			} else {
				$escola_diretor = "1=0 AND";
			}
		}
	}
	
	// Query original que lista todas as turmas, inclusive as com nenhum aluno
	$sql = <<<DML
SELECT DISTINCT turid AS codigo,
                turdesc || CASE WHEN nes.nuetipo = 'S'
                             THEN ' SEDE '
                             ELSE ' ANEXO ' END || ', Total de Alunos: ' || (SELECT count(*)
                                                                               FROM projovemurbano.cadastroestudante c
                                                                               WHERE c.turid = t.turid
                                                                                 AND caestatus = 'A') AS descricao
  FROM projovemurbano.turma t
    LEFT JOIN projovemurbano.nucleoescola nes
      ON nes.entid = t.entid AND nes.nucid = {$dados['nucid']}
  WHERE {$escola_diretor} t.nucid = {$dados['nucid']}
    AND turstatus = 'A'
    $sql_com_vaga
  ORDER BY 2
    
DML;
	// $sql = "SELECT DISTINCT
	// t.turid as codigo,
	// t.turdesc||
	// CASE WHEN nes.nuetipo = 'S' THEN ' SEDE ' ELSE ' ANEXO ' END|| ', Total de Alunos: '||
	// count(c.caeid) AS descricao FROM projovemurbano.turma t
	// INNER JOIN projovemurbano.nucleoescola nes ON nes.entid = t.entid AND nes.nucid = ".$dados['nucid']."
	// INNER JOIN projovemurbano.cadastroestudante c ON c.turid = t.turid AND caestatus = 'A'
	// GROUP BY t.turid,
	// t.turdesc,
	// CASE WHEN nes.nuetipo = 'S' THEN ' SEDE ' ELSE ' ANEXO ' END
	// ORDER BY 2";
	
	$turmas = $db->carregar ( $sql );
	$dados ['bloq'] = $dados ['bloq'] ? $dados ['bloq'] : 'S';
	if ($_REQUEST ['form'] == 'M') {
		$db->monta_combo ( 'turidM', $sql, $dados ['bloq'], 'Selecione', '', '', '', '', 'N', 'turidM' );
	} else {
		$db->monta_combo ( 'turid', $sql, $dados ['bloq'], 'Selecione', '', '', '', '', 'S', 'turid' );
	}
	if (! $turmas) {
		
		echo "<label style=\"color:red\">
				<b>
					Esta escola não possui turmas ou vagas disponíveis.
				</b>
			  </label>";
	}
}

/**
 * Retorna as turmas de um núcleo
 * Verifica na sessão as turmas vinculadas ao diretor logado
 *
 * @global cls_banco $db
 * @param array $dados        	
 */
function buscarTurmasComAlunos($dados) {
	global $db;
	
	verificaTurmaNucleo ( $dados ['nucid'] );
	
	if (! $db->testa_superuser ()) {
		$perfis = pegaPerfilGeral ();
		
		if (in_array ( 650, $perfis )) {
			if ($_SESSION ['projovemurbano'] ['entid']) {
				$escola_diretor = "t.entid = " . $_SESSION ['projovemurbano'] ['entid'] . " AND ";
			} else {
				$escola_diretor = "1=0 AND";
			}
		}
	}
	
	$turid = '';
	
	if (isset ( $dados ['turid'] )) {
		$turid = $dados ['turid'];
	}
	
	$sql = "SELECT DISTINCT t.turid as codigo
                , turdesc || CASE WHEN nes.nuetipo = 'S' THEN ' SEDE ' ELSE ' ANEXO ' END 
                          ||', Total de Alunos: ' 
                          || COUNT( c.caeid ) as descricao
                
                FROM projovemurbano.turma t
                LEFT JOIN projovemurbano.nucleoescola nes 
                    ON nes.entid    = t.entid 
                    AND nes.nucid   = " . $dados ['nucid'] . "
                INNER JOIN  projovemurbano.cadastroestudante c
                    ON c.turid = t.turid                     
                WHERE $escola_diretor t.nucid = " . $dados ['nucid'] . " 
                AND turstatus = 'A'
                AND caestatus = 'A'
                GROUP BY t.turid, t.turdesc, nes.nuetipo
                ORDER BY 2";
	
// 	ver($sql,d);
	$dados = $db->carregar ( $sql );
	$dados ['bloq'] = $dados ['bloq'] ? $dados ['bloq'] : 'S';
	$db->monta_combo ( 'turid', $sql, $dados ['bloq'], 'Selecione', '', '', '', '', 'S', 'turid', '', $turid );
	
	if (empty($dados)) {
		
		echo "<label style=\"color:red\">
				<b>
					Esta escola não possui turmas.
				</b>
			  </label>";
	}
}
function buscarPeriodoEncaminharLista() {
	global $db;
	$inner='';
	if($_SESSION['projovemurbano']['ppuid'] !=3){
		$dataRange = "|| ' - '
				|| to_char(perdtinicio,'DD/MM/YYYY')
				|| ' a '
				|| to_char(perdtfim,'DD/MM/YYYY')";
	}
	$sql = "SELECT DISTINCT per.perid as codigo
            	, per.perdesc $dataRange as descricao
        	FROM projovemurbano.diario dia
        	INNER JOIN projovemurbano.periodocurso as per
        	    ON dia.perid = per.perid
        	INNER JOIN workflow.documento as doc
		        ON doc.docid = dia.docid
		    WHERE per.ppuid = " . $_SESSION ['projovemurbano'] ['ppuid'] . "
			AND per.perid !=37
        	ORDER BY per.perid";
	$dados = $db->carregar ( $sql );
	$dados = $dados ? $dados : 'S';
	
	$db->monta_combo ( 'perid', $sql, 'S', 'Selecione', '', '', '', '', 'S', 'perid' );
	
	if (! $dados) {
		
		echo "<label style=\"color:red\">
		<b>
		Esta turma não possui período.
		</b>
		</label>";
	}
}
function buscarPeriodoDiario($dados) {
	global $db;
	
	$perId = '';
	
	if (isset ( $dados ['perid'] ))
		$perId = $dados ['perid'];
	if($_SESSION['projovemurbano']['ppuid']==2){
		$filtroperiodo = "AND b.perid!=37";
	}
	if($_SESSION['projovemurbano']['ppuid'] !=3){
		$dataRange = "|| ' - '
				|| to_char(perdtinicio,'DD/MM/YYYY')
				|| ' a '
				|| to_char(perdtfim,'DD/MM/YYYY')";
	}else{
		$sql = "SELECT distinct
					ordem
				FROM
						projovemurbano.rangeperiodo rap
				INNER JOIN projovemurbano.projovemurbano pju ON pju.rapid = rap.rapid
				WHERE
					pju.pjuid = {$_SESSION['projovemurbano']['pjuid']}";
		$ordem = $db->pegaUm ( $sql );
		
		$dataRange = "|| ' - '
				|| to_char(datainicio,'DD/MM/YYYY')
				|| ' a '
				|| to_char(datafim,'DD/MM/YYYY')";
		$inner = "INNER JOIN projovemurbano.rangeperiodo rap ON rap.perid = a.perid AND rap.ordem = {$ordem}";
	}
	if (!$ordem&&($_SESSION['projovemurbano']['ppuid'] ==3)) {
		echo 'Esta localidade ainda não escolheu a data de início.';
		die;
	}
	$sql = "SELECT DISTINCT b.perid as codigo 
				, b.perdesc $dataRange as descricao
			FROM projovemurbano.diario a
			INNER JOIN projovemurbano.periodocurso b
				ON a.perid = b.perid
			$inner
			WHERE a.turid = '{$dados['turid']}'
            AND   b.ppuid = " . $_SESSION ['projovemurbano'] ['ppuid'] . "
				$filtroperiodo
                        ORDER BY b.perid";

	$dados = $db->carregar ( $sql );
	$dados ['bloq'] = $dados ['bloq'] ? $dados ['bloq'] : 'S';

	$db->monta_combo ( 'perid', $sql, $dados ['bloq'], 'Selecione', '', '', '', '', 'S', 'perid', '', $perId );
	
	if (! $dados) {
		echo "<label style=\"color:red\">
			<b>
			Esta turma não possui período.
			</b>
			</label>";
	}
}
function listaEstudantesPorTurma($turid) {
	global $db;
	
	$sql = "SELECT a.caeid, a.caenome, a.caestatus
    			FROM projovemurbano.cadastroestudante a
    			WHERE a.turid = " . $turid . "
    		--	AND a.caestatus = 'A' 
    			ORDER BY a.caenome";
	// ver($sql);
	$retorno = $db->carregar ( $sql );
	return $retorno;
}
function listaEstudantesTransferidosPorTurma($turid) {
	global $db;
	
	$sql = "SELECT DISTINCT
                    cae.caeid ,cae.caenome ,tra.turid_origem 
                FROM projovemurbano.cadastroestudante cae
                INNER JOIN projovemurbano.transferencia tra ON cae.caeid = tra.cad_caeid AND tra.turid_origem = $turid
                INNER JOIN projovemurbano.historico_transferencia htr ON htr.traid = tra.traid AND htr.shtid_status = '3'
                WHERE
                    tra.turid_origem = " . $turid . "
                AND cae.caestatus = 'A' 
                ORDER BY cae.caenome
                ";
	// ver($sql,d);
	$retorno = $db->carregar ( $sql );
	return $retorno;
}
function listaPresencaPorAluno($param) {
	global $db;
	
	$difid = ! empty ( $param ['difid'] ) ? $param ['difid'] : 0;
	$caeid = ! empty ( $param ['caeid'] ) ? $param ['caeid'] : 0;
	
	$sql = "SELECT frq.frqid, frq.frqqtdpresenca 
			FROM projovemurbano.frequenciaestudante frq
			INNER JOIN projovemurbano.diariofrequencia dif
				ON frq.difid = dif.difid
			INNER JOIN projovemurbano.gradecurricular grd
				ON dif.grdid = grd.grdid
			INNER JOIN projovemurbano.componentecurricular coc
				ON grd.cocid = coc.cocid
			WHERE frq.caeid = {$caeid}
			AND frq.difid = {$difid}";
	// ver($sql,d);
	$retorno = $db->pegaLinha ( $sql );
	
	return $retorno;
}
function listaPresencaPorAlunoTransferido($parametrosPresencaTrans) {
	global $db;
	
	$difid = ! empty ( $parametrosPresencaTrans ['difid'] ) ? $parametrosPresencaTrans ['difid'] : 0;
	$caeid = ! empty ( $parametrosPresencaTrans ['caeid'] ) ? $parametrosPresencaTrans ['caeid'] : 0;
	
	$sql = "SELECT frq.frqid, frq.frqqtdpresenca 
            FROM projovemurbano.frequenciaestudante frq
            INNER JOIN projovemurbano.diariofrequencia dif
                ON frq.difid = dif.difid
            INNER JOIN projovemurbano.gradecurricular grd
                ON dif.grdid = grd.grdid
            INNER JOIN projovemurbano.componentecurricular coc
                ON grd.cocid = coc.cocid
            WHERE frq.caeid = {$caeid}
            AND frq.difid = {$difid}";
	// ver($sql,d);
	$retorno2 = $db->pegaLinha ( $sql );
	
	return $retorno2;
}
function listarComponenteCurricular($diaid) {
	global $db;
	
	$sql = "SELECT 
                    a.cocid, 
                    a.cocnome, 
                    a.cocdesc, 
                    c.difid
		  FROM projovemurbano.componentecurricular a
                LEFT OUTER JOIN projovemurbano.gradecurricular b
                    ON a.cocid = b.cocid
		LEFT OUTER JOIN projovemurbano.diariofrequencia c
                    ON b.grdid 	= c.grdid
                   AND c.diaid = {$diaid}
		 WHERE a.cocstatus 	= 'A' 
                   AND a.cocdisciplina = 'D'";
	
	// ver($sql);
	$retorno = $db->carregar ( $sql );
	return $retorno;
}
function listarComponenteCurricularTrans($diaid) {
	global $db;
	// ver($diaid,d);
	$sql = "SELECT DISTINCT 
                    a.cocid, 
                    a.cocnome, 
                    a.cocdesc, 
                    c.difid
          FROM projovemurbano.componentecurricular a
                LEFT OUTER JOIN projovemurbano.gradecurricular b
                    ON a.cocid = b.cocid
        LEFT OUTER JOIN projovemurbano.diariofrequencia c
                    LEFT JOIN projovemurbano.frequenciaestudante frq ON frq.difid = c.difid
                    ON b.grdid  = c.grdid
                   AND c.diaid = {$diaid}
                   --AND frq.frqstatus = 'I'
         WHERE a.cocstatus  = 'A' 
                   AND a.cocdisciplina = 'D'
";
	
	// ver($sql,d);
	$retorno = $db->carregar ( $sql );
	return $retorno;
}
function montaCabecalhoDoDiarioFrequenciaMensal($parametros) {
	global $db;
	
	$sql = "SELECT
                    dif.difqtdaulaprevista,
                    dif.difqtdauladada,
                    dif.difid,
                    dia.diaid,
                    grd.grdid AS grid_id,
                    coc.cocid AS coc_id,
                    coc.cocdesc AS componente_curricular,
                    coc.cocnome AS componente_curricular_nome,
                    coalesce(pol.polid::text, 'Não consta' ) AS polo,
                    pol.polendereco AS endereco,
                    nuc.nucid as nucleo,
                    nuc.nucinstituicoes as instituicoes,
                    tur.entid AS entidade_id,
                    ent.entnome AS entidade,
                    ede.endlog AS logradouro,
                    ede.endnum AS numero,
                    ede.endcom AS endereco_comercial,
                    ede.endbai AS endbai,
                    ede.endcep AS cep,
                    ede.muncod AS codigo_municipal,
                    ede.endlog || ede.endnum || ' - ' || ede.endbai ||' - '|| pmun.mundsc ||'/'|| ede.estuf as endereco_completo,
                    pmun.mundsc AS municipio,
                    ede.estuf AS uf,
                    tur.turid AS turma_id,
                    tur.turdesc AS turma,
                    cic.cicid, cic.cicdesc AS ciclo,
                    unf.unfid, unf.unfdesc AS unidade,
                    per.perid, 
                    per.perdesc AS periodo,
                    per.perdtinicio AS dt_inicio,
                    per.perdtfim AS dt_fim,
                    soma_difqtdauladada,
                    (SELECT sum(cocqtdhoras) as cocqtdhoras 
                    FROM projovemurbano.componentecurricular 
                    WHERE cocdisciplina = 'D' AND cocstatus = 'A') as hrs_aulas_previstas
                  FROM  projovemurbano.diario dia
		  INNER JOIN projovemurbano.diariofrequencia dif on dia.diaid = dif.diaid
                  INNER JOIN projovemurbano.gradecurricular grd ON grd.grdid = dif.grdid
                  INNER JOIN projovemurbano.componentecurricular coc ON coc.cocid = grd.cocid
                  INNER JOIN projovemurbano.programaprojovemurbano ppu ON ppu.ppuid = grd.ppuid
                  INNER JOIN projovemurbano.turma tur ON dia.turid = tur.turid
                  INNER JOIN projovemurbano.periodocurso per on per.perid = dia.perid
                  INNER JOIN projovemurbano.unidadeformativa unf ON per.unfid = unf.unfid
                  INNER JOIN projovemurbano.ciclocurso cic ON unf.cicid = cic.cicid
                  INNER JOIN projovemurbano.nucleo nuc ON nuc.nucid = tur.nucid
                  INNER JOIN projovemurbano.municipio mun ON mun.munid = nuc.munid
                  LEFT OUTER JOIN projovemurbano.associamucipiopolo amp ON amp.munid = mun.munid
                  LEFT OUTER JOIN projovemurbano.polo pol ON pol.polid = amp.polid
                  LEFT OUTER JOIN projovemurbano.polomunicipio plm ON plm.pmuid = pol.pmuid
                  LEFT OUTER JOIN entidade.entidade ent ON ent.entid = tur.entid
                  LEFT OUTER JOIN entidade.endereco ede ON ede.entid = ent.entid
                  LEFT OUTER JOIN municipio pmun ON pmun.muncod = ede.muncod
                  LEFT OUTER JOIN ( SELECT
					SUM(difqtdauladada) as soma_difqtdauladada,
					diaid
                                    FROM projovemurbano.diariofrequencia a
                                    INNER JOIN projovemurbano.gradecurricular b ON b.grdid = a.grdid
                                    INNER JOIN projovemurbano.componentecurricular c ON c.cocid = b.cocid
                                    WHERE c.cocdisciplina = 'D' 
                                    GROUP BY diaid) AS total_aulas
                                        ON total_aulas.diaid = dif.diaid
					WHERE
                                            per.perid 		  = " . $parametros ['perid'] . "
                                            AND tur.turid 	  = " . $parametros ['turid'] . "
                                            AND nuc.nucid 	  = " . $parametros ['nucid'] . "
                                            AND nuc.nucstatus	  = 'A'
                                            AND mun.munstatus	  = 'A'
                                            AND coc.cocdisciplina = 'D' ";
	// ver($sql,d);
	$infoDiario = $db->carregar ( $sql );
	return $infoDiario;
}
function montaCabecalhoDoDiarioFrequenciaMensalTrans($parametros) {
	global $db;
	
	$sql = "SELECT
                    dif.difqtdaulaprevista,
                    dif.difqtdauladada,
                    dif.difid,
                    dia.diaid,
                    grd.grdid AS grid_id,
                    coc.cocid AS coc_id,
                    coc.cocdesc AS componente_curricular,
                    coc.cocnome AS componente_curricular_nome,
                    coalesce(pol.polid::text, 'Não consta' ) AS polo,
                    pol.polendereco AS endereco,
                    nuc.nucid as nucleo,
                    nuc.nucinstituicoes as instituicoes,
                    tur.entid AS entidade_id,
                    ent.entnome AS entidade,
                    ede.endlog AS logradouro,
                    ede.endnum AS numero,
                    ede.endcom AS endereco_comercial,
                    ede.endbai AS endbai,
                    ede.endcep AS cep,
                    ede.muncod AS codigo_municipal,
                    ede.endlog || ede.endnum || ' - ' || ede.endbai ||' - '|| pmun.mundsc ||'/'|| ede.estuf as endereco_completo,
                    pmun.mundsc AS municipio,
                    ede.estuf AS uf,
                    tur.turid AS turma_id,
                    tur.turdesc AS turma,
                    cic.cicid, cic.cicdesc AS ciclo,
                    unf.unfid, unf.unfdesc AS unidade,
                    per.perid, 
                    per.perdesc AS periodo,
                    per.perdtinicio AS dt_inicio,
                    per.perdtfim AS dt_fim,
                    soma_difqtdauladada,
                    (SELECT sum(cocqtdhoras) as cocqtdhoras 
                    FROM projovemurbano.componentecurricular 
                    WHERE cocdisciplina = 'D' AND cocstatus = 'A') as hrs_aulas_previstas
                  FROM  projovemurbano.diario dia
          INNER JOIN projovemurbano.diariofrequencia dif on dia.diaid = dif.diaid 
                  INNER JOIN projovemurbano.gradecurricular grd ON grd.grdid = dif.grdid
                  INNER JOIN projovemurbano.componentecurricular coc ON coc.cocid = grd.cocid
                  INNER JOIN projovemurbano.programaprojovemurbano ppu ON ppu.ppuid = grd.ppuid
                  INNER JOIN projovemurbano.turma tur ON dia.turid = tur.turid
                  INNER JOIN projovemurbano.periodocurso per on per.perid = dia.perid
                  INNER JOIN projovemurbano.unidadeformativa unf ON per.unfid = unf.unfid
                  INNER JOIN projovemurbano.ciclocurso cic ON unf.cicid = cic.cicid
                  INNER JOIN projovemurbano.nucleo nuc ON nuc.nucid = tur.nucid
                  INNER JOIN projovemurbano.municipio mun ON mun.munid = nuc.munid
                  LEFT OUTER JOIN projovemurbano.associamucipiopolo amp ON amp.munid = mun.munid
                  LEFT OUTER JOIN projovemurbano.polo pol ON pol.polid = amp.polid
                  LEFT OUTER JOIN projovemurbano.polomunicipio plm ON plm.pmuid = pol.pmuid
                  LEFT OUTER JOIN entidade.entidade ent ON ent.entid = tur.entid
                  LEFT OUTER JOIN entidade.endereco ede ON ede.entid = ent.entid
                  LEFT OUTER JOIN municipio pmun ON pmun.muncod = ede.muncod
                  LEFT OUTER JOIN ( SELECT
                    SUM(difqtdauladada) as soma_difqtdauladada,
                    diaid
                                    FROM projovemurbano.diariofrequencia a
                                    INNER JOIN projovemurbano.gradecurricular b ON b.grdid = a.grdid
                                    INNER JOIN projovemurbano.componentecurricular c ON c.cocid = b.cocid
                                    WHERE c.cocdisciplina = 'D' 
                                    GROUP BY diaid) AS total_aulas
                                        ON total_aulas.diaid = dif.diaid
                    WHERE
                                            per.perid         = " . $parametros ['perid'] . "
                                            AND tur.turid     = " . $parametros ['turid'] . "
                                            AND nuc.nucid     = " . $parametros ['nucid'] . "
                                            AND nuc.nucstatus     = 'A'
                                            AND mun.munstatus     = 'A'
                                            AND coc.cocdisciplina = 'D' ";
	// ver($sql,d);
	$infoDiario = $db->carregar ( $sql );
	return $infoDiario;
}
function buscarTurmas2($dados) {
	global $db;
	
	verificaTurmaNucleo ( $dados ['nucid'] );
	$perfis = pegaPerfilGeral ();
	
	if (! $db->testa_superuser () && ! in_array ( PFL_CONSULTA, $perfis )) {
		if ($_SESSION ['projovemurbano'] ['entid']) {
			$escola_diretor = "t.entid = " . $_SESSION ['projovemurbano'] ['entid'] . " AND ";
		} else {
			$escola_diretor = "1=0 AND ";
		}
	}
	
	$sql = "SELECT DISTINCT
				turid as codigo,
				turdesc||', Total de Alunos: '||(SELECT count(*) FROM projovemurbano.cadastroestudante c WHERE c.turid = t.turid AND caestatus = 'A') as descricao
			FROM
				projovemurbano.turma t
			WHERE
				$escola_diretor
				t.nucid = " . $dados ['nucid'] . "
			ORDER BY
				2";
	$dados ['bloq'] = $dados ['bloq'] ? $dados ['bloq'] : 'S';
	$db->monta_combo ( 'turid', $sql, $dados ['bloq'], 'Selecione um nucleo', '', '', '', '', 'N', 'turid' );
}
function adicionaHistoricoDiario($parametros, $tipo) {
	global $db;
	
	// verifica historico anterior
	$sql = "select hidid, diaid from projovemurbano.diario where turid = '" . $parametros ['turid'] . "' and perid = '" . $parametros ['perid'] . "' ";
	
	$dadosDiario = $db->pegaLinha ( $sql );
	
	$anteriorHidid = $dadosDiario ['hidid'];
	$anteriorHidid = $anteriorHidid ? $anteriorHidid : 'null';
	$pkDiario = $dadosDiario ['diaid'];
	
	// verifica se já existe algum save para o mesmo histórico. caso sim, Ignore.
	if ($anteriorHidid == $tipo) {
		return false;
	}
	// verifica qual será o tipo de encaminhamento, caso exista o polo ou não.(se tipo passado for 2 ou 3)
	if ($tipo == 2 || $tipo == 3) {
		
		if ($_SESSION ['projovemurbano'] ['pjuid'] != "") {
			
			$sql = "select * from projovemurbano.polomunicipio where pjuid = " . $_SESSION ['projovemurbano'] ['pjuid'] . "and pmupossuipolo = 't'";
			$pmuid = $db->pegaUm ( $sql );
			if ($pmuid) {
				$tipo = 2;
			} else {
				$tipo = 3;
			}
		} else {
			$tipo = 3;
		}
	}
	$sql = "INSERT INTO projovemurbano.historico_diario ( stdid, diaid, anterior_hidid, usucpfquemfez, datahora ) values ($tipo, $pkDiario, $anteriorHidid, '" . $_SESSION ['usucpf'] . "', clock_timestamp() ) RETURNING hidid";
	
	$pkHidid = $db->pegaUm ( $sql );
	$db->commit ();
	
	// update id do historico na tabela de frequencia
	$sql = "update projovemurbano.diario set hidid = $pkHidid where diaid = " . $pkDiario;
	$pkDiario = $db->pegaUm ( $sql );
	$db->commit ();
}
function adicionaHistoricoDiarioById($id, $tipo) {
	global $db;
	
	// verifica historico anterior
	$sql = "select max(hidid) from projovemurbano.diario where diaid = '" . $id . "' limit 1";
	
	$anteriorHidid = $db->pegaUm ( $sql );
	
	$anteriorHidid = $anteriorHidid ? $anteriorHidid : 'null';
	$pkDiario = $id;
	
	// verifica se já existe algum save para o mesmo histórico. caso sim, Ignore.
	// if( $anteriorHidid == $tipo ){
	// return false;
	// }
	// verifica qual será o tipo de encaminhamento, caso exista o polo ou não.(se tipo passado for 2 ou 3)
	// if( $tipo == 2 || $tipo == 3 ){
	
	// if( $_SESSION['projovemurbano']['pjuid'] != "" ) {
	
	// $sql = "select * from projovemurbano.polomunicipio where pjuid = ".$_SESSION['projovemurbano']['pjuid']. "and pmupossuipolo = 't'";
	// $pmuid = $db->pegaUm( $sql );
	// if( $pmuid ){
	// $tipo = 2;
	// }else{
	// $tipo = 3;
	// }
	// }else{
	// $tipo = 3;
	// }
	// }
	$sql = "INSERT INTO projovemurbano.historico_diario ( stdid, diaid, anterior_hidid, usucpfquemfez, datahora ) values ($tipo, $pkDiario, $anteriorHidid, '" . $_SESSION ['usucpf'] . "', clock_timestamp() ) RETURNING hidid";
	
	$pkHidid = $db->pegaUm ( $sql );
	$db->commit ();
	
	// update id do historico na tabela de frequencia
	$sql = "update projovemurbano.diario set hidid = $pkHidid where diaid = " . $pkDiario;
	$pkDiario = $db->pegaUm ( $sql );
	$db->commit ();
}
function salvarDiarioFrequenciaMensal($parametros) {
	global $db;
	// Salva as aulas dadas em "diariofrequencia"
	$parDiarios = $parametros ['qtdaulasdadas'];
	$parEstudantes = $parametros ['qtdaulas'];
	// ver($parDiarios,d);
	foreach ( $parDiarios as $chave => $valor ) {
		$valor = ( int ) $valor;
		
		$db->executar ( "UPDATE projovemurbano.diariofrequencia SET difqtdauladada = {$valor} WHERE difid = {$chave}" );
		// $db->commit();
	}
	
	$debugTotalUpdate = 0;
	$debugTotalInsert = 0;
	
	// Salva as presenças dos estudantes em "frequenciaestudante"
	foreach ( $parEstudantes as $chave => $valor ) {
		// Recupera a chave de "Diário Frequencia"
		$idDiario = $chave;
		
		if (! empty ( $idDiario ) && $idDiario != 0) {
			
			foreach ( $valor as $chaveEstudante => $valorEstudante ) {
				// Recupera a chave de "Cadastro Estudante"
				$idEstudante = $chaveEstudante;
				
				foreach ( $valorEstudante as $chaveDiarioEstudante => $valorDiarioEstudante ) {
					// Verifica se o registro já existe
					$sql = sprintf ( "SELECT caeid 
                    				FROM projovemurbano.frequenciaestudante
                        			WHERE caeid = %d
                        			AND difid = %d", $idEstudante, $idDiario );
					
					$temRegistro = $db->pegaUm ( $sql );
					
					$valorDiarioEstudante = (empty ( $valorDiarioEstudante ) ? 0 : $valorDiarioEstudante);
					// ver($valorDiarioEstudante,d);
					if ($temRegistro === false) {
						$sql = "INSERT INTO projovemurbano.frequenciaestudante(caeid, difid, frqqtdpresenca) ";
						$sql .= " VALUES( {$idEstudante}, {$idDiario}, {$valorDiarioEstudante} )";
						
						$db->executar ( $sql );
						// $db->commit();
						
						$debugTotalInsert ++;
					} else {
						$valorDiarioEstudante = ( int ) $valorDiarioEstudante;
						
						$sql = "UPDATE projovemurbano.frequenciaestudante SET ";
						
						$sql .= " frqqtdpresenca = {$valorDiarioEstudante}";
						$sql .= " WHERE difid = {$idDiario}";
						$sql .= " AND caeid = {$idEstudante}";
						$sql .= " AND frqid = {$chaveDiarioEstudante}";
						
						$db->executar ( $sql );
						// $db->commit();
						
						$debugTotalUpdate ++;
					}
				}
			}
		}
	}
	
	$db->commit ();
	// echo "debugTotalUpdate ". $debugTotalUpdate . " - debugTotalInsert: " . $debugTotalInsert;
}

/**
 *
 *
 *
 * Salva o diário de trabalho com seus lançamentos
 *
 * @global cls_banco $db
 * @param array $dados        	
 * @throws Exception
 * @return boolean
 */
function salvarDiarioTrabalho($dados) {
	global $db;
	if ($dados) {
		$sqlInsereFrequencia="";
		foreach ( $dados as $difId => $arrAluno ) {
			if (is_array ( $dados [$difId] )) {
				foreach ( $dados [$difId] as $caeId => $frqtrabalho ) {
					$sqlVerificaFrequencia = "SELECT frqid
		                                    FROM projovemurbano.frequenciaestudante
		                                    WHERE caeid   = {$caeId}
		                                    AND difid     = {$difId}";
					
					$verificaFrequencia = $db->pegaUm ( $sqlVerificaFrequencia );
					
					if ($verificaFrequencia == false) {
						$sqlInsereFrequencia .= " INSERT INTO projovemurbano.frequenciaestudante( caeid, difid, frqtrabalho ) VALUES( {$caeId}, {$difId}, '{$frqtrabalho}'); ";
					} else {
						$sqlInsereFrequencia .= " UPDATE projovemurbano.frequenciaestudante SET frqtrabalho  = '{$frqtrabalho}' WHERE caeid = {$caeId} AND difid = {$difId}; ";
					}
					
				}
			}
		}

		if($sqlInsereFrequencia) {
			$db->executar($sqlInsereFrequencia);
			$db->commit();
		}
	}
	
	return true;
}
function buscarNucleos($dados) {
	global $db;
	
	if ($dados ['polid']) {
		$nucleos = pegarNucleos ( "t", $dados ['polid'] );
		if (! $nucleos [0]) {
			if (contaEstudantesNucleos ( "t" )) {
				echo "Numero máximo de estudantes atingido.";
				die ();
			} else {
				echo "Não possui Nucleo";
				die ();
			}
		}
	} else {
		$nucleos = array ();
	}
	
	if ($_REQUEST ['form'] == 'M') {
		$db->monta_combo ( 'nucidM', $nucleos, 'S', 'Selecione', 'buscarTurmasM', '', '', '', 'N', 'nucidM' );
	} else {
		$db->monta_combo ( 'nucid', $nucleos, $dados ['bloq'], 'Selecione', 'buscarTurmas', '', '', '', 'S', 'nucid' );
	}
}
function verificaCertificadoTotalBolsa($dados) {
	global $db;
	
	$sql = "SELECT dbucertificado 
			FROM projovemurbano.dadosbatimentoum 
			WHERE 
				dbucpf='" . str_replace ( array (
			".",
			"-" 
	), array (
			"",
			"" 
	), $dados ['cpf'] ) . "' 
				AND (dbuprojeto='PROJOVEM_URBANO' OR dbuprojeto='PROJOVEM_ORIGINAL')
				AND dbucertificado > 0";
	$dbucertificado = $db->pegaUm ( $sql );
	
	if (! $dbucertificado && verifica_data ( $dados ['caedatanasc'] )) {
		$sql = "SELECT dbucertificado FROM projovemurbano.dadosbatimentoum 
				WHERE 
					UPPER(dbunomeestudante)=UPPER('" . removeacentos ( trim ( $dados ['caenome'] ) ) . "')
					AND UPPER(dbunomemae)=UPPER('" . removeacentos ( trim ( $dados ['caenomemae'] ) ) . "')
					AND dbudatanasc='" . formata_data_sql ( $dados ['caedatanasc'] ) . "'
					AND (dbuprojeto='PROJOVEM_URBANO' OR dbuprojeto='PROJOVEM_ORIGINAL')
					AND dbucertificado > 0";
		
		$dbucertificado = $db->pegaUm ( $sql );
	}
	
	$sql = "SELECT dbutotalbolsas FROM projovemurbano.dadosbatimentoum WHERE dbucpf='" . str_replace ( array (
			".",
			"-" 
	), array (
			"",
			"" 
	), $dados ['cpf'] ) . "' AND dbuprojeto='PROJOVEM_URBANO'";
	$caeqtddireitobolsa = $db->pegaUm ( $sql );
	
	if (! $caeqtddireitobolsa && verifica_data ( $dados ['caedatanasc'] )) {
		$sql = "SELECT dbutotalbolsas 
				FROM projovemurbano.dadosbatimentoum 
				WHERE 
					UPPER(dbunomeestudante)=UPPER('" . removeacentos ( trim ( $dados ['caenome'] ) ) . "') AND 
					UPPER(dbunomemae)=UPPER('" . removeacentos ( trim ( $dados ['caenomemae'] ) ) . "') AND 
					dbudatanasc='" . formata_data_sql ( $dados ['caedatanasc'] ) . "' AND
					dbuprojeto='PROJOVEM_URBANO'";
		$caeqtddireitobolsa = $db->pegaUm ( $sql );
	}
	// ver($caeqtddireitobolsa,d);
	if ($caeqtddireitobolsa == '' && $dbucertificado == '') {
		$totalbolsa = '18';
	} else {
		$totalbolsa = (($caeqtddireitobolsa > 18) ? 0 : 18 - $caeqtddireitobolsa);
		$totalbolsa = $dbucertificado ? '0' : $totalbolsa;
	}
	echo $dbucertificado . ";" . $totalbolsa;
}
function excluirEstudante($dados) {
	global $db;
	
	$sql = "DELETE FROM projovemurbano.pagamentoestudante WHERE caeid='" . $dados ['caeid'] . "'";
	$db->executar ( $sql );
	
	$sql = "DELETE FROM projovemurbano.frequenciaestudante WHERE caeid='" . $dados ['caeid'] . "'";
	$db->executar ( $sql );
	
	$_sql = "SELECT caestatus FROM projovemurbano.cadastroestudante WHERE caeid='" . $dados ['caeid'] . "'";
	$caestatus = $db->pegaUm ( $_sql );
	
	$sql = "DELETE FROM projovemurbano.cadastroestudante_recursosacessibilidade WHERE caeid='" . $dados ['caeid'] . "'";
	$db->executar ( $sql );
	
	$sql = "DELETE FROM projovemurbano.estudantefilhos WHERE caeid='" . $dados ['caeid'] . "'";
	$db->executar ( $sql );
	
	$sql = "DELETE FROM projovemurbano.cadastroestudante WHERE caeid='" . $dados ['caeid'] . "'";
	$db->executar ( $sql );
	
	
	$db->commit ();
	
	// HISTORICO - paramentros do historico.
	$caeid = $dados ['caeid'];
	$usucpf = $_SESSION ['usucpf'];
	$usucpf = str_replace ( array (
			".",
			"-" 
	), array (
			"",
			"" 
	), $usucpf );
	$hictipo = $caestatus != '' ? $caestatus : 'I';
	$hicacao = "D";
	
	historicoCadastro ( $caeid, $usucpf, $hictipo, $hicacao );
	// HOSITORICO end.
	
	echo "<script>
			alert('Estudante excluido com sucesso');
			window.location='projovemurbano.php?modulo=principal/listaEstudantesMonitoramento&acao=A';
		  </script>";
	die;
}
$possuipolo = "f";
function pegarNumeroNucleos($possuipolo) {
	global $db;
	
	if ($possuipolo == "t") {
		
		if ($_SESSION ['projovemurbano'] ['ppuid'] != 1) {
			$filtraTprid = "AND nuc.tprid = {$_SESSION['projovemurbano']['tprid']}";
			$ppuid = "AND nuc.ppuid = {$_SESSION['projovemurbano']['ppuid']}";
		}
		
		$sql = "SELECT COUNT(*)
                          FROM projovemurbano.nucleo nuc
                            INNER JOIN projovemurbano.municipio mun ON mun.munid = nuc.munid 
                            INNER JOIN projovemurbano.associamucipiopolo amp ON amp.munid = mun.munid    
                            INNER JOIN projovemurbano.polo pol ON pol.polid = amp.polid 
                            INNER JOIN projovemurbano.polomunicipio plm ON plm.pmuid = pol.pmuid 
                          WHERE nuc.nucstatus='A'
                            AND mun.munstatus='A'
                            AND plm.pmustatus='A' 
                            AND pol.polstatus='A'
                            AND pjuid = {$_SESSION['projovemurbano']['pjuid']}
                            $filtraTprid
                            $ppuid";
		$numeronucleos = $db->pegaUm ( $sql );
		// ver($sql);
	} else {
		
		if ($_SESSION ['projovemurbano'] ['ppuid'] != 1) {
			$filtraTprid = "AND plm.tprid = {$_SESSION['projovemurbano']['tprid']}";
			$ppuid = "AND nuc.ppuid = {$_SESSION['projovemurbano']['ppuid']}";
		}
		
		$sql = "SELECT COUNT(*)
                          FROM projovemurbano.nucleo nuc 
                            INNER JOIN projovemurbano.municipio mun ON mun.munid = nuc.munid 
                            INNER JOIN projovemurbano.polomunicipio plm ON plm.pmuid = mun.pmuid 
                          WHERE nuc.nucstatus='A'
                            AND mun.munstatus='A'
                            AND pjuid = {$_SESSION['projovemurbano']['pjuid']}
                            $filtraTprid
                            $ppuid";
		$numeronucleos = $db->pegaUm ( $sql );
		// ver($sql);
	}
	return $numeronucleos;
}
function pegarNumeroEstudantes($possuipolo) {
	global $db;
	
	if ($possuipolo) {
		
		$numeroestudantes = $db->pegaUm ( "SELECT sum(nucqtdestudantes)
                                                   FROM projovemurbano.nucleo nuc
                                                     INNER JOIN projovemurbano.municipio mun ON mun.munid = nuc.munid 
                                                     INNER JOIN projovemurbano.associamucipiopolo amp ON amp.munid = mun.munid    
                                                     INNER JOIN projovemurbano.polo pol ON pol.polid = amp.polid 
                                                     INNER JOIN projovemurbano.polomunicipio plm ON plm.pmuid = pol.pmuid 
                                                   WHERE nuc.nucstatus='A'
                                                     AND mun.munstatus='A'
                                                     AND plm.pmustatus='A'
                                                     AND pol.polstatus='A'
                                                     AND pjuid='" . $_SESSION ['projovemurbano'] ['pjuid'] . "'
                                                     AND nuc.tprid={$_SESSION['projovemurbano']['tprid']}" );
	} else {
		
		$numeroestudantes = $db->pegaUm ( "SELECT sum(nucqtdestudantes)
                                                   FROM projovemurbano.nucleo nuc 
                                                     INNER JOIN projovemurbano.municipio mun ON mun.munid = nuc.munid 
                                                     INNER JOIN projovemurbano.polomunicipio plm ON plm.pmuid = mun.pmuid 
                                                   WHERE nuc.nucstatus='A'
                                                     AND mun.munstatus='A'
                                                     AND plm.pmustatus='A'
                                                     AND pjuid='" . $_SESSION ['projovemurbano'] ['pjuid'] . "'
                                                     AND nuc.tprid={$_SESSION['projovemurbano']['tprid']}" );
	}
	
	return $numeroestudantes;
}
function pegarEducadores($proid, $tipo) {
	global $db;
	
	if ($_SESSION ['projovemurbano'] ['ppuid'] == 2 || $_SESSION ['projovemurbano'] ['ppuid'] == 3) {
		$filtraTprid = " AND edu.tprid = {$_SESSION['projovemurbano']['tprid']}";
	}
	
	$educadores = $db->pegaLinha ( "SELECT * FROM projovemurbano.educadores edu 
								  LEFT JOIN projovemurbano.contratadocomp ctc ON ctc.ccmrid=edu.ccmrid 
								  LEFT JOIN projovemurbano.contratadorecurso ctr ON ctr.creid=edu.creid  
								  WHERE 
									proid = $proid 
									AND edustatus='A' 
									AND edutipo='$tipo' 
									$filtraTprid" );
	return $educadores;
}
function contatipoeducadores($proid) {
	global $db;
	
	if ($_SESSION ['projovemurbano'] ['ppuid'] == 2 || $_SESSION ['projovemurbano'] ['ppuid'] == 3) {
		$filtraTprid = " AND edu.tprid = {$_SESSION['projovemurbano']['tprid']}";
	}
	
	$educadores = $db->pegaUm ( "SELECT DISTINCT count(edutipo) FROM projovemurbano.educadores edu
			LEFT JOIN projovemurbano.contratadorecurso ctr ON ctr.creid=edu.creid
			WHERE
			proid = $proid
			AND edustatus='A'
			AND edutipo in ('F','P','Q')
			AND crevlrbrutorem is not null
			$filtraTprid" );
	return $educadores;
}
function gravarCNPJEntidade($dados) {
	global $db;
	$db->executar ( "UPDATE entidade.entidade SET entnumcpfcnpj='" . str_replace ( array (
			".",
			"/",
			"-" 
	), array (
			"",
			"",
			"" 
	), $dados ['entnumcpfcnpj'] ) . "' WHERE entid='" . $dados ['entid'] . "'" );
	echo "UPDATE entidade.entidade SET entnumcpfcnpj='" . str_replace ( array (
			".",
			"/",
			"-" 
	), array (
			"",
			"",
			"" 
	), $dados ['entnumcpfcnpj'] ) . "' WHERE entid='" . $dados ['entid'] . "'";
	$db->commit ();
}
function pegarNucleosPorMunicipio($munid) {
	global $db;
	
	$nucleos = $db->carregar ( "SELECT *, (SELECT entnome FROM entidade.entidade ent INNER JOIN projovemurbano.nucleoescola nes ON nes.entid = ent.entid WHERE nes.nucid=nuc.nucid AND nes.nuetipo='S') as entnome FROM projovemurbano.nucleo nuc
						    INNER JOIN projovemurbano.municipio mun ON mun.munid = nuc.munid 
							WHERE nuc.nucstatus='A' AND mun.munstatus='A' AND mun.munid='" . $munid . "' 
							ORDER BY nuc.nucid" );
	
	return $nucleos;
}
function buscarCursosPorInstituicoesMunicipio($dados) {
	global $db;
	$sql = "SELECT '<font style=font-size:8px;>'||ccucurso||'</font>' as curso, 
				   '<select style=width:90px; class=CampoEstilo name=cupturma[{$dados['nucid']}]['||ccuid||']>
				    <option value=\"\">Selecione</option>
				    <option value=\"1\" '||COALESCE((SELECT CASE WHEN COALESCE(cupturma::text,'')='1' THEN 'selected' ELSE '' END FROM projovemurbano.cursoprojovemurbano WHERE ccuid=c.ccuid AND nucid=" . $dados ['nucid'] . " AND cupstatus='A'),'')||'>Turma 1</option>
				    <option value=\"2\" '||COALESCE((SELECT CASE WHEN COALESCE(cupturma::text,'')='2' THEN 'selected' ELSE '' END FROM projovemurbano.cursoprojovemurbano WHERE ccuid=c.ccuid AND nucid=" . $dados ['nucid'] . " AND cupstatus='A'),'')||'>Turma 2</option>
				    <option value=\"3\" '||COALESCE((SELECT CASE WHEN COALESCE(cupturma::text,'')='3' THEN 'selected' ELSE '' END FROM projovemurbano.cursoprojovemurbano WHERE ccuid=c.ccuid AND nucid=" . $dados ['nucid'] . " AND cupstatus='A'),'')||'>Turma 3</option>
				    <option value=\"4\" '||COALESCE((SELECT CASE WHEN COALESCE(cupturma::text,'')='4' THEN 'selected' ELSE '' END FROM projovemurbano.cursoprojovemurbano WHERE ccuid=c.ccuid AND nucid=" . $dados ['nucid'] . " AND cupstatus='A'),'')||'>Turma 4</option>
				    <option value=\"5\" '||COALESCE((SELECT CASE WHEN COALESCE(cupturma::text,'')='5' THEN 'selected' ELSE '' END FROM projovemurbano.cursoprojovemurbano WHERE ccuid=c.ccuid AND nucid=" . $dados ['nucid'] . " AND cupstatus='A'),'')||'>Turma 5</option>
				    </select>' as inp, 
				    '<input type=text class=normal size=6 maxlength=5 id=cupqtdestudantes_'||c.ccuid||'_{$dados['nucid']} name=cupqtdestudantes[{$dados['nucid']}]['||ccuid||'] value=\"'||COALESCE((SELECT COALESCE(cupqtdestudantes::text,'') FROM projovemurbano.cursoprojovemurbano WHERE ccuid=c.ccuid AND nucid=" . $dados ['nucid'] . " AND cupstatus='A'),'')||'\" onkeyup=\"this.value=mascaraglobal(\'######\',this.value);calcularQuantidadeCursosMunicpio(\''||ccuid||'\',\'{$dados['ccuibge']}\',this);\">' as inp2 
				    FROM projovemurbano.cargacurso c WHERE ccuibge='" . $dados ['ccuibge'] . "' AND ccuinstituicao='" . iconv ( "UTF-8", "ISO-8859-1", $dados ['ccuinstituicao'] ) . "'";
	
	$cabecalho = array (
			"Curso",
			"Turma",
			"Vagas" 
	);
	$db->monta_lista_simples ( $sql, $cabecalho, 50, 5, 'N', '100%', $par2 );
}
function testaQtdAlunoTurma($dados) {
	global $db;
	
	$sqlEstudante = '';
	$turmaatual = '';
	
	if (is_array( $dados )) {
		$dados['cpfestudante'] = is_array($dados) ? $dados['cpfestudante'] : '';
		$sqlEstudante = " AND cae.caecpf <> '" . $dados ['cpfestudante'] . "'";
		$turid = $dados ['turid'];
		if($dados['cpfestudante'] != ''){
			$sql = "SELECT 
						turid
					 FROM
						projovemurbano.cadastroestudante
					WHERE
						caecpf = '{$dados['cpfestudante']}'";
		
			$turmaatual = $db->pegaUm ( $sql );
		}
	} else {
		$turid = $dados;
	}
	
	if ($turmaatual != $turid) {
		if ($turid && $turid != 'undefined' && is_numeric ( $turid )) {
			$sql = "SELECT
							true
					FROM
					(
						SELECT
							count(caeid) as qtd
						FROM
							projovemurbano.turma tur
						INNER JOIN projovemurbano.cadastroestudante cae ON cae.turid = tur.turid AND cae.caestatus = 'A'
						WHERE tur.turid = " . $turid . " 
	                                            {$sqlEstudante}
					) as foo
					WHERE
						qtd >= 40";
			$boolean = $db->pegaUm ( $sql );
		}
	}
	return ($boolean == 't' ? true : false);
	die ();
}
function testaQtdAlunoMetaProjovem($pjuid) {
	global $db;
	$sql12 = "SELECT true FROM projovemurbano.metasdoprograma WHERE pjuid = {$_SESSION['projovemurbano']['pjuid']} AND tpmid = 3";
	$temAjuste = $db->pegaUm ( $sql12 );
	// ver($temAjuste,d);
	if ($temAjuste == 't') {
		$sql = "
            SELECT
                    true
                FROM
                (
                    SELECT 
                        pjuid,
                        mtpvalor as qtd
                    FROM projovemurbano.metasdoprograma 
                    WHERE 
                        tpmid = 3  
                    AND ppuid = {$_SESSION['projovemurbano']['ppuid']} 
                    AND pjuid = {$_SESSION['projovemurbano']['pjuid']}
                ) as foo
                WHERE
                    foo.qtd <= ( SELECT count(caeid) FROM projovemurbano.cadastroestudante cae WHERE cae.pjuid = foo.pjuid AND cae.caestatus = 'A'  )
                    AND foo.pjuid = $pjuid ";
		$boolean = $db->pegaUm ( $sql );
	} else {
		$sql = "SELECT
    				true
    			FROM
    			(
    				SELECT 
    					pu.pjuid,
    					COALESCE(sa.suametaajustada,cm.cmemeta) as qtd
    				FROM
    					projovemurbano.projovemurbano pu
    				LEFT JOIN territorios.estado est ON est.estuf = pu.estuf
    				LEFT JOIN projovemurbano.cargameta cm ON cm.cmecodibge = pu.muncod::numeric OR cm.cmecodibge = est.estcod::numeric
    				LEFT JOIN projovemurbano.sugestaoampliacao sa ON sa.pjuid = pu.pjuid 
    				WHERE
    					cm.cmemeta IS NOT NULL
    				AND pu.ppuid = {$_SESSION['projovemurbano']['ppuid']}
    				AND cm.ppuid = {$_SESSION['projovemurbano']['ppuid']}
    			) as foo
    			WHERE
    				foo.qtd <= ( SELECT count(caeid) FROM projovemurbano.cadastroestudante cae WHERE cae.pjuid = foo.pjuid AND cae.caestatus = 'A'  )
    				AND foo.pjuid = $pjuid";
		$boolean = $db->pegaUm ( $sql );
	}
	// ver($sql,d);
	return ($boolean == 't' ? true : false);
}
function testaQtdAlunoNucleo($dados) {
	global $db;
	
	$dados = is_array($dados) ? $dados : array();
	
	$sql = "SELECT
				nucid
			FROM
				projovemurbano.cadastroestudante
			WHERE
				caecpf = '" . $dados ['cpfestudante'] . "'";
	$teste = $db->pegaUm ( $sql );
	if ($teste != $dados ['nucid']) {
		$sqlEstudante = '';
		
		if (is_array ( $dados )) {
			$nucid = $dados ['nucid'];
			$sqlEstudante = "AND cae.caecpf <> '" . $dados ['cpfestudante'] . "'";
		} else {
			return false;
			// $nucid = $dados;
		}
		if ($nucid == '' || ! is_numeric ( $nucid )) {
			return false;
		}
		$sql = "SELECT
		  			true
		  		FROM
		  			projovemurbano.nucleo nuc2
		  		WHERE
		  			nuc2.nucqtdestudantes <= ( SELECT count(caeid) 
	                                                        FROM projovemurbano.cadastroestudante cae 
	                                                        WHERE 
	                                                        cae.nucid = nuc2.nucid 
	                                                        {$sqlEstudante}
	                                                        AND cae.caestatus = 'A'  )
		  			AND nuc2.nucid = $nucid";
		$boolean = $db->pegaUm ( $sql );
	} else {
		$boolean = 'f';
	}
	return ($boolean == 't' ? true : false);
}
function inserirEstudantes($dados) {
	global $db;
	
	$dados ['caenomesocial'] = $dados ['caenomesocial'] ? "'" . $dados ['caenomesocial'] . "'" : 'null';
	$dados ['egressoprogalfabetizacao'] = $dados ['egressoprogalfabetizacao'] ? $dados ['egressoprogalfabetizacao'] : 'FALSE';
	$dados ['cpf'] = str_replace ( Array (
			'.',
			'-' 
	), '', $dados ['caecpf'] );
	$dados ['caetelfixo'] = str_replace ( '-', '', $dados ['caetelfixo'] );
	$dados ['caetelcel'] = str_replace ( '-', '', $dados ['caetelcel'] );
	$dados ['caenomemae'] = empty ( $dados ['caenomemae'] ) ? 'IGNORADA' : $dados ['caenomemae'];
	$dados ['caenomepai'] = empty ( $dados ['caenomepai'] ) ? 'IGNORADO' : $dados ['caenomepai'];
	$dados ['estqtdfilhos'] = $dados ['estqtdfilhos'] < 1 ? '0' : $dados ['estqtdfilhos'];
	$dados ['ufemissaorg'] = $dados ['ufemissaorg'] ? "'" . $dados ['ufemissaorg'] . "'" : 'null';
	if ($dados ['caetipomoradia'] != 'u' || $dados ['caetipomoradia'] != 'r') {
		$dados ['caetipomoradia'] = $dados ['caetipomoradia'] == 'TRUE' ? 'u' : 'r';
	}

	if ($_SESSION ['projovemurbano'] ['ppuano'] == '2013') {
		$ufemissaorg = 'ufemissaorg';
	}

	$obrigatorios = Array (
			'craid',
			'escid',
			'ssoid',
			'nucid',
			'endestuf',
			'endmuncod',
			'caenome',
			'caenomemae',
			'caenaturalidade',
			'caedeficiencia',
			'caefilhos',
			'caebenoutroprog',
			'turid',
			'caenumrg',
			'caeorgaoexpedidorrg',
			'caedataemissaorg',
			'caecep',
			'caelogradouro',
			'caenumero',
			'caebairro',
			'caetelfixo',
			'caestatus',
			'caealtashabilidades',
			'egressoprogalfabetizacao' 
	);
	if ($ufemissaorg) {
		array_push ( $obrigatorios, $ufemissaorg );
	}
	// -- URL de redirecionamento após o processamento
	switch ($_SESSION ['projovemurbano'] ['ppuano']) {
		case 2012 :
			$urlCadastroEstudantes = 'projovemurbano.php?modulo=principal/monitoramento&acao=A&aba=cadastroEstudantes';
			break;
		default :
			$urlCadastroEstudantes = "projovemurbano.php?modulo=principal/monitoramento{$_SESSION['projovemurbano']['ppuano']}&acao=A&aba=cadastroEstudantes";
	}
	foreach ( $obrigatorios as $obrigatorio ) {
		if ($dados [$obrigatorio] == '') {

			die ( "<script>
					alert('Erro na validação das informações.');
					window.location='{$urlCadastroEstudantes}';
				  </script>" );
		}
	}
	
	if ($dados ['turid'] == '') {
		die ( "<script>
				alert('Escolha uma turma.');
				window.location='{$urlCadastroEstudantes}';
			  </script>" );
	}
	/* Regra exigida por wallace - 25/06/2012 */
	if (testaQtdAlunoMetaProjovem ( $_SESSION ['projovemurbano'] ['pjuid'] )) {
		die ( "<script>
				alert('Meta atingida.');
                                window.location='{$urlCadastroEstudantes}';
			  </script>" );
	}
	/* FIm Regra exigida por wallace - 25/06/2012 */
	/*Regra exigida por wallace - 08/05/2012*/
	if (testaQtdAlunoNucleo ( $dados ['nucid'] )) {
		die ( "<script>
				alert('Núcleo lotado. Escolha outra.');
				window.location='{$urlCadastroEstudantes}';
			  </script>" );
	}
	if (testaQtdAlunoTurma ( $dados ['turid'] )) {
		die ( "<script>
				alert('Turma lotada. Escolha outra.');
				window.location='{$urlCadastroEstudantes}';
			  </script>" );
	}
	/* Fim Regra exigida por wallace - 08/05/2012 */
	$teste = verificaCadastroCPF2 ( $dados );
	if ($teste == '1') {
		
		die ( "<script>
				alert('CPF já cadastrado.');
				window.location='{$urlCadastroEstudantes}';
			  </script>" );
	}elseif ($teste == '2') {
		
		die ( "<script>
				alert('CPF já cadastrado no Projovem Campo.');
				window.location='{$urlCadastroEstudantes}';
			  </script>" );
	}
	
	$dados ['caestatus'] = $dados ['caestatus'] ? $dados ['caestatus'] : 'A';
	// if($_SESSION['projovemurbano']['ppuid'] == 2){
	// $campo = 'ppuid,';
	// $valor = '2';
	// }
	
	$sql = "INSERT INTO projovemurbano.cadastroestudante(pjuid,
															 ppuid,
                                                             polid,
                                                             craid,
                                                             tdeid,
                                                             tdiid,
                                                             escid,
                                                             pbeid,
                                                             ocuid,
                                                             ssoid,
                                                             nucid,
                                                             estuf,
                                                             muncod,
                                                             caecpf,
                                                             caenome,
                                                             caedatanasc,
                                                             caesexo,
                                                             caenomemae,
                                                             caenomepai,
                                                             caenaturalidade, -- grava nacionalidade
                                                             caeufnaturalidade,
                                                             caemuncodnaturalidade,
                                                             caenispispasep,
                                                             caenit,
                                                             caedeficiencia,
                                                             caefilhos,
                                                             --qtdfilhos,
                                                             caebenoutroprog,
                                                             caeocupacao,
                                                             caebrasilalfa,
                                                             caehistorico,
                                                             caetestepro,
                                                             turid,
                                                             caeqtddireitobolsa,
                                                             caenumrg,
                                                             caeorgaoexpedidorrg,
                                                             ufemissaorg,
                                                             caedataemissaorg,
                                                             caecep,
                                                             caelogradouro,
                                                             caenumero,
                                                             caecomp,
                                                             caebairro,
                                                             caeemail,
                                                             caetelfixo,
                                                             caetelcel,
                                                             caestatus,
                                                             caealtashabilidades,
                                                             caejustificativainativacao,
                                                             egressoprogalfabetizacao,
                                                             turno,
                                                             minid,
                                                             descricaomotivoinativacao,
                                                             caecumpremedidasocioeduc,
                                                             caeparticipouprojovemurbano,
															 caetipomoradia)
		    VALUES ('" . $_SESSION ['projovemurbano'] ['pjuid'] . "',
		    				'" . $_SESSION ['projovemurbano'] ['ppuid'] . "',
                            " . (($dados ['polid']) ? "'" . $dados ['polid'] . "'" : "NULL") . ",
                            '" . $dados ['craid'] . "',
                            " . (($dados ['tdeid']) ? "'" . $dados ['tdeid'] . "'" : "NULL") . ",
                            " . (($dados ['tdiid']) ? "'" . $dados ['tdiid'] . "'" : "NULL") . ",
                            '" . $dados ['escid'] . "',
                            " . (($dados ['pbeid']) ? "'" . $dados ['pbeid'] . "'" : "NULL") . ",
                            " . (($dados ['ocuid']) ? "'" . $dados ['ocuid'] . "'" : "NULL") . ",
                            '" . $dados ['ssoid'] . "',
                            '" . $dados ['nucid'] . "',
                            '" . $dados ['endestuf'] . "',
                            '" . $dados ['endmuncod'] . "',
                            '" . str_replace ( array (
			".",
			"-" 
	), "", $dados ['caecpf'] ) . "',
                            '" . $dados ['caenome'] . "',
                            '" . formata_data_sql ( $dados ['caedatanasc'] ) . "',
                            '" . $dados ['caesexo'] . "',
		            '" . $dados ['caenomemae'] . "',
                            " . (($dados ['caenomepai']) ? "'" . $dados ['caenomepai'] . "'" : "NULL") . ",
                            '" . $dados ['caenaturalidade'] . "', -- grava nacionalidade
                            '" . $dados ['caeufnaturalidade'] . "',
                            '" . $dados ['caemuncodnaturalidade'] . "',
                            " . (($dados ['caenispispasep']) ? "'" . $dados ['caenispispasep'] . "'" : "NULL") . ",
		            " . (($dados ['caenit']) ? "'" . $dados ['caenit'] . "'" : "NULL") . ",
                            " . $dados ['caedeficiencia'] . ",
                            " . $dados ['caefilhos'] . ",
                            --" . $dados ['estqtdfilhos'] . ",
                            " . $dados ['caebenoutroprog'] . ",
		            " . (($dados ['caeocupacao']) ? "'" . $dados ['caeocupacao'] . "'" : "NULL") . ",
                            " . $dados ['caebrasilalfa'] . ",
                            " . $dados ['caehistorico'] . ",
                            " . $dados ['caetestepro'] . ",
                            " . $dados ['turid'] . ",
		            " . ((is_numeric ( $dados ['caeqtddireitobolsa'] )) ? "'" . $dados ['caeqtddireitobolsa'] . "'" : "NULL") . ",
                            '" .  addslashes($dados ['caenumrg']) . "',
                            '" . $dados ['caeorgaoexpedidorrg'] . "',
                            " . $dados ['ufemissaorg'] . ",
		            '" . formata_data_sql ( $dados ['caedataemissaorg'] ) . "',
                            '" . str_replace ( array (
			"-" 
	), array (
			"" 
	), $dados ['caecep'] ) . "',
                            '" . $dados ['caelogradouro'] . "',
		            '" . $dados ['caenumero'] . "',
                            " . (($dados ['caecomp']) ? "'" . addslashes($dados ['caecomp']) . "'" : "NULL") . ",
                            '" . addslashes($dados ['caebairro']) . "',
                            " . (($dados ['caeemail']) ? "'" . $dados ['caeemail'] . "'" : "NULL") . ",
                            '" . $dados ['caetelfixo'] . "',
                            " . (($dados ['caetelcel']) ? "'" . $dados ['caetelcel'] . "'" : "NULL") . ",
		            '" . $dados ['caestatus'] . "',
                            " . $dados ['caealtashabilidades'] . ",
                            " . ($dados ['caejustificativainativacao'] != '' ? "'" . $dados ['caejustificativainativacao'] . "'" : "null") . ",
                            " . $dados ['egressoprogalfabetizacao'] . ",
                            '" . $dados ['turno'] . "',
                            " . ($dados ['minid'] != '' ? "'" . $dados ['minid'] . "'" : "null") . ",
                            '" . $dados ['descricaomotivoinativacao'] . "',
                            " . (($dados ['caecumpremedidasocioeduc']) ? "'" . $dados ['caecumpremedidasocioeduc'] . "'" : "NULL") . ",
                            " . (($dados ['caeparticipouprojovemurbano']) ? "'" . $dados ['caeparticipouprojovemurbano'] . "'" : "NULL") . ",
							'" . $dados ['caetipomoradia'] . "') RETURNING caeid;";
	// ver($sql,d);
	$caeid = $db->pegaUm ( $sql );
	$sql = '';
	$sql = "DELETE FROM projovemurbano.cadastroestudante_recursosacessibilidade WHERE caeid = $caeid;";
	
	if ($dados ['estqtdfilhos'] && ($dados ['estqtdfilhos'])) {
		foreach ( $dados ['estqtdfilhos'] as $idfid => $estqtdfilhos ) {
			if ($estqtdfilhos) {
				$sql .= "INSERT INTO projovemurbano.estudantefilhos(
            			caeid, idfid, estqtdfilhos, esfstatus)
    					VALUES ('" . $caeid . "', " . $idfid . ", " . trim ( $estqtdfilhos ) . ", 'A');";
			}
		}
	} elseif ($dados ['estqtdfilhos']) {
		$sql .= "INSERT INTO projovemurbano.estudantefilhos(caeid, idfid, estqtdfilhos, esfstatus)
				 VALUES({$caeid}, 3, {$dados['estqtdfilhos']}, 'A');";
	}
	
	if ($dados ['racid']) {
		foreach ( $dados ['racid'] as $racid ) {
			$sql .= "INSERT INTO projovemurbano.cadastroestudante_recursosacessibilidade(
            			caeid, racid)
    				VALUES ('" . $caeid . "', $racid);";
		}
	}
	
	if ($sql != '') {
		$db->executar ( $sql );
	}
	$db->commit ();
	
	// HISTORICO - paramentros do historico.
	$caeid = $caeid;
	$usucpf = $_SESSION ['usucpf'];
	$usucpf = str_replace ( array (
			".",
			"-" 
	), array (
			"",
			"" 
	), $usucpf );
	$hictipo = $dados ['caestatus'];
	$hicacao = "I";
	
	historicoCadastro ( $caeid, $usucpf, $hictipo, $hicacao );
	// HOSITORICO end.
	
	echo "<script>
			alert('Estudante inserido com sucesso.');
			window.open( 'projovemurbano.php?modulo=principal/popComprovante&acao=A&caeid=" . $caeid . "', 'Comprovante', 'width=480,height=265,status=1,menubar=1,toolbar=0,scrollbars=1,resizable=1' );
			window.location='{$urlCadastroEstudantes}';
		  </script>";
	die;
}
function atualizarEstudantes($dados) {
	global $db;
	// ver($dados ['caetipomoradia'],d);

	// if($dados ['caetipomoradia']!= 'u'||$dados ['caetipomoradia']!='r'){
	// $dados ['caetipomoradia'] = $dados ['caetipomoradia'] == 'TRUE'?'u':'r';
	// }
	// -- URL de redirecionamento após o processamento
	switch ($_SESSION ['projovemurbano'] ['ppuano']) {
		case 2012 :
			$urlCadastroEstudantes = 'projovemurbano.php?modulo=principal/monitoramento&acao=A&aba=cadastroEstudantes';
			break;
		default :
			$urlCadastroEstudantes = "projovemurbano.php?modulo=principal/monitoramento{$_SESSION['projovemurbano']['ppuano']}&acao=A&aba=cadastroEstudantes";
	}
	
	if ($dados ['turid'] == '') {
		echo "<script>
            	alert('Escolha uma turma.');
            	window.location='{$urlCadastroEstudantes}';
          	</script>";
	}
	
	if (testaQtdAlunoTurma ( $dados ['turid'] ) && $dados ['turid'] != $dados ['turid_bkp']) {
		die ( "<script>
            	alert('Turma lotada. Escolha outra.');
            	window.location='{$urlCadastroEstudantes}';
         	</script>" );
	}
	$teste = verificaCadastroCPF2 ( $dados );
	if ($teste == '2') {
	
		die ( "<script>
			alert('CPF já cadastrado no Projovem Campo.');
			window.location='{$urlCadastroEstudantes}';
			</script>" );
	}
	$dados ['craid'] = $dados ['craid'] ? $dados ['craid'] : 'null';
	$dados ['tdeid'] = $dados ['tdeid'] ? $dados ['tdeid'] : 'null';
	$dados ['tdiid'] = $dados ['tdiid'] ? $dados ['tdiid'] : 'null';
	$dados ['escid'] = $dados ['escid'] ? $dados ['escid'] : 'null';
	$dados ['pbeid'] = $dados ['pbeid'] ? $dados ['pbeid'] : 'null';
	$dados ['ocuid'] = $dados ['ocuid'] ? $dados ['ocuid'] : 'null';
	$dados ['ssoid'] = $dados ['ssoid'] ? $dados ['ssoid'] : 'null';
	$dados ['nucid'] = $dados ['nucid'] ? $dados ['nucid'] : 'null';
	$dados ['turid'] = $dados ['turid'] ? $dados ['turid'] : 'null';
	$dados ['caetestepro'] = $dados ['caetestepro'] ? $dados ['caetestepro'] : 'false';
	$status = $dados ['caestatus'];
	$dados ['caestatus'] = $dados ['caestatus'] ? "'" . $dados ['caestatus'] . "'" : "'A'"; // 'NULL';
	$dados ['caenomesocial'] = $dados ['caenomesocial'] ? "'" . $dados ['caenomesocial'] . "'" : 'null';
	$dados ['caedeficiencia'] = $dados ['caedeficiencia'] ? "'" . $dados ['caedeficiencia'] . "'" : 'null';
	$dados ['caefilhos'] = $dados ['caefilhos'] ? "'" . $dados ['caefilhos'] . "'" : 'null';
	$dados ['caebenoutroprog'] = $dados ['caebenoutroprog'] ? "'" . $dados ['caebenoutroprog'] . "'" : 'null';
	$dados ['caeocupacao'] = $dados ['caeocupacao'] ? "'" . $dados ['caeocupacao'] . "'" : 'null';
	$dados ['caebrasilalfa'] = $dados ['caebrasilalfa'] ? "'" . $dados ['caebrasilalfa'] . "'" : 'null';
	$dados ['caehistorico'] = $dados ['caehistorico'] ? "'" . $dados ['caehistorico'] . "'" : 'null';
	$dados ['egressoprogalfabetizacao'] = $dados ['egressoprogalfabetizacao'] ? $dados ['egressoprogalfabetizacao'] : 'FALSE';
	$dados ['estqtdfilhos'] = $dados ['estqtdfilhos'] < 1 ? '0' : $dados ['estqtdfilhos'];
	
	$sql = "UPDATE projovemurbano.cadastroestudante
                  SET polid=" . (($dados ['polid']) ? $dados ['polid'] : "NULL") . ",
                      craid=" . $dados ['craid'] . ",
                      tdeid=" . $dados ['tdeid'] . ",
                      tdiid=" . $dados ['tdiid'] . ",
                      escid=" . $dados ['escid'] . ",
                      pbeid=" . $dados ['pbeid'] . ",
                      ocuid=" . $dados ['ocuid'] . ",
                      ssoid=" . $dados ['ssoid'] . ",
                      nucid=" . $dados ['nucid'] . ",
                      caenomesocial=" . $dados ['caenomesocial'] . ",
                      caecpf='" . str_replace ( array (
			".",
			"-" 
	), "", $dados ['caecpf'] ) . "',
                      caenome='" . $dados ['caenome'] . "',
                      caedatanasc='" . formata_data_sql ( $dados ['caedatanasc'] ) . "',
                      caenomemae='" . $dados ['caenomemae'] . "',
                      caenomepai='" . $dados ['caenomepai'] . "',
                      caenaturalidade='" . $dados ['caenaturalidade'] . "',
                      caeufnaturalidade='" . $dados ['caeufnaturalidade'] . "',
                      caemuncodnaturalidade='" . $dados ['caemuncodnaturalidade'] . "',
                      caenispispasep='" . trim ( $dados ['caenispispasep'] ) . "',
                      caenit='" . trim ( $dados ['caenit'] ) . "',
                      caedeficiencia=" . $dados ['caedeficiencia'] . ",
                      caefilhos=" . $dados ['caefilhos'] . ",
                      --qtdfilhos=" . (($dados ['estqtdfilhos']) ? "'" . $dados ['estqtdfilhos'] . "'" : "NULL") . ",
                      caebenoutroprog=" . $dados ['caebenoutroprog'] . ",
                      caeocupacao=" . $dados ['caeocupacao'] . ",
                      caebrasilalfa=" . $dados ['caebrasilalfa'] . ",
                      caehistorico=" . $dados ['caehistorico'] . ",
                      caetestepro=" . $dados ['caetestepro'] . ",
                      turid=" . $dados ['turid'] . ",
                      caeqtddireitobolsa=" . (is_numeric ( $dados ['caeqtddireitobolsa'] ) ? "'" . $dados ['caeqtddireitobolsa'] . "'" : "NULL") . ",
                      caenumrg='" . addslashes($dados ['caenumrg']) . "',
                      caeorgaoexpedidorrg='" . $dados ['caeorgaoexpedidorrg'] . "',
                      ufemissaorg=" . (($dados ['ufemissaorg']) ? "'" . $dados ['ufemissaorg'] . "'" : "NULL") . ",
                      caedataemissaorg='" . formata_data_sql ( $dados ['caedataemissaorg'] ) . "',
                      caecep='" . str_replace ( array (
			"-" 
	), array (
			"" 
	), $dados ['caecep'] ) . "',
		      caelogradouro='" . $dados ['caelogradouro'] . "',
                      caenumero='" . $dados ['caenumero'] . "',
                      caecomp=" . (($dados ['caecomp']) ? "'" . addslashes($dados ['caecomp']) . "'" : "NULL") . ",
	      			caebairro='" . addslashes($dados ['caebairro']) . "',
                      caeemail='" . $dados ['caeemail'] . "',
                      caetelfixo='" . str_replace ( '-', '', $dados ['caetelfixo'] ) . "',
                      caetelcel='" . str_replace ( '-', '', $dados ['caetelcel'] ) . "',
                      estuf='" . $dados ['endestuf'] . "',
                      muncod='" . $dados ['endmuncod'] . "',
				      caesexo='" . $dados ['caesexo'] . "',
				      caealtashabilidades = " . $dados ['caealtashabilidades'] . ",
				      caestatus = " . $dados ['caestatus'] . ",
				      caejustificativainativacao = " . ($dados ['caejustificativainativacao'] != '' ? "'" . $dados ['caejustificativainativacao'] . "'" : "null") . ",
				      egressoprogalfabetizacao = " . $dados ['egressoprogalfabetizacao'] . ",
				      turno = '" . $dados ['turno'] . "',
                      minid = " . (($dados ['minid']) ? "'" . $dados ['minid'] . "'" : "NULL") . ",
                      descricaomotivoinativacao = '" . $dados ['descricaomotivoinativacao'] . "',
                      caecumpremedidasocioeduc = " . (($dados ['caecumpremedidasocioeduc']) ? "'" . $dados ['caecumpremedidasocioeduc'] . "'" : "NULL") . ",
                      caeparticipouprojovemurbano=" . (($dados ['caeparticipouprojovemurbano']) ? "'" . $dados ['caeparticipouprojovemurbano'] . "'" : "NULL") . ",
				      caetipomoradia = '" . $dados ['caetipomoradia'] . "'
                  WHERE caeid='" . $dados ['caeid'] . "';";
	
	$sql .= "DELETE FROM projovemurbano.estudantefilhos WHERE caeid = " . $dados ['caeid'] . ";";
	
	// ver($dados,d);

	if ($dados ['estqtdfilhos'] && is_array ( $dados ['estqtdfilhos'] )) {
		foreach ( $dados ['estqtdfilhos'] as $idfid => $estqtdfilhos ) {
			if ($estqtdfilhos) {
				$sql .= "INSERT INTO projovemurbano.estudantefilhos(
	            			caeid, idfid, estqtdfilhos, esfstatus)
	    					VALUES ('" . $dados ['caeid'] . "', " . $idfid . ", " . $estqtdfilhos . ", 'A');";
				
				$db->executar ( $sql );
			}
		}
	}
	// ver($sql,d);
	$sql .= "DELETE FROM projovemurbano.cadastroestudante_recursosacessibilidade WHERE caeid='" . $dados ['caeid'] . "';";
	// $db->executar($sql);
	// ver($dados,d);
	if ($dados ['racid']) {
		foreach ( $dados ['racid'] as $racid ) {
			$sql .= "INSERT INTO projovemurbano.cadastroestudante_recursosacessibilidade(
            			caeid, racid)
    				VALUES ('" . $dados ['caeid'] . "', $racid);";
		}
	}
	// ver($sql,d);
	$db->executar ( $sql );
	$db->commit ();
	
	// HISTORICO - paramentros do historico.
	$caeid = $dados ['caeid']; // $dados['caeid']
	$usucpf = $_SESSION ['usucpf'];
	$usucpf = str_replace ( array (
			".",
			"-" 
	), array (
			"",
			"" 
	), $usucpf );
	$hictipo = $status;
	$hicacao = "A";
	
	historicoCadastro ( $caeid, $usucpf, $hictipo, $hicacao );
	// HOSITORICO end.
	
	echo "<script>
			alert('Estudante atualizado com sucesso');
			window.open( 'projovemurbano.php?modulo=principal/popComprovante&acao=A&caeid=" . $caeid . "', 'Comprovante', 'width=480,height=265,status=1,menubar=1,toolbar=0,scrollbars=1,resizable=1' );
                        window.location='{$urlCadastroEstudantes}';
		  </script>";
}
function historicoCadastro($caeid, $usucpf, $hictipo, $hicacao) {
	global $db;
	
	// hicid - id da tabela historicocadastro
	// caeid - id da tabela cadastroestudante
	// usucpf - CPF do estudante
	// hicdataacao - data da acao
	// hictipo Tipo de Status: a - ativação, i - inativação
	// hicacao Tipo da ação realizada: Insert - Update - Delete
	// hicstatus Status da ação-sistema por DEFAULT é sempre 'A'
	
	$sql = "
		Insert Into projovemurbano.historicocadastro(
            	caeid, 
            	usucpf, 
            	hicdataacao, 
            	hictipo, 
            	hicacao
        )Values(
				$caeid, 
				'$usucpf',
				'now()',
				'$hictipo', 
				'$hicacao' 
		);
	";
	if ($sql != '') {
		$db->executar ( $sql );
	}
	$db->commit ();
}
function verificaCadastroCPF($dados) {
	global $db;
	$sql = "SELECT 
				1
			FROM
				projovemurbano.cadastroestudante
			WHERE
				caecpf = '" . $dados ['cpf'] . "'";
	$teste = $db->pegaUm ( $sql );

	$sql = "SELECT
				2
			FROM
				projovemcampo.estudante
			WHERE
				estcpf = '" . $dados ['cpf'] . "'
			AND eststatus = 'A'
				";
	$teste2 = $db->pegaUm ( $sql );

	if($teste2==2){
		$teste = 2;
	}
	echo $teste; //== 't' ? 'true' : 'false';
	// return $teste == 't' ? 'true' : 'false';
}
function verificaCadastroCPF2($dados) {
	global $db;
	
	$sql = "SELECT 
				1
			FROM
				projovemurbano.cadastroestudante
			WHERE
				caecpf = '" . $dados ['cpf'] . "'";
	$teste = $db->pegaUm ( $sql );
	
	$sql = "SELECT
				2
			FROM
				projovemcampo.estudante
			WHERE
				estcpf = '" . $dados ['cpf'] . "'
			AND eststatus = 'A'";
	$teste2 = $db->pegaUm ( $sql );
	// echo $teste == 't' ? 'true' : 'false';
	if($teste2==2){
		$teste = 2;
	}
	return $teste; //== 't' ? 'true' : 'false';
}

/**
 * Monta diário de frequencia de alunos
 *
 * @param array $param
 *        	onde devem ser pespecificados todos os parametros do diário
 * @param string $param['var']
 *        	Nome da variável que possui a frequência
 * @param string|array $param['sql']
 *        	Sql ou array com os alunos do diário Array('código'=>00000,'descricao'=>'Fulano de tal')
 * @param string|array $param['sql_marcados']
 *        	Sql ou array com as frequencias já marcadas Array(Array('codigo'=>'111111','data'=>'05/03/2012','valor'=>'P','ordem'=>1),
 *        	Array('codigo'=>'111111','data'=>'05/03/2012','valor'=>'P','ordem'=>2),
 *        	Array('codigo'=>'111111','data'=>'05/03/2012','valor'=>' ','ordem'=>3),
 *        	Array('codigo'=>'222222','data'=>'05/03/2012','valor'=>' ','ordem'=>1),
 *        	Array('codigo'=>'222222','data'=>'05/03/2012','valor'=>' ','ordem'=>2),
 *        	Array('codigo'=>'222222','data'=>'05/03/2012','valor'=>' ','ordem'=>3),
 *        	Array('codigo'=>'333333','data'=>'05/03/2012','valor'=>' ','ordem'=>1),
 *        	Array('codigo'=>'333333','data'=>'05/03/2012','valor'=>'F','ordem'=>2),
 *        	Array('codigo'=>'333333','data'=>'05/03/2012','valor'=>'P','ordem'=>3)
 *        	); 'P' para presente e 'F' para falso.
 * @param string|array $param['sql_periodo']
 *        	Sql ou array com o periodo de referência da frequência Array('inicio'=>'01/01/2012','fim'=>'01/02/2012');
 * @param array $param['width']
 *        	Array com as larguras da 2 caixas, nomes e frequencia. Array('nomes'=>'50px','frequencias'=>'500px');
 * @return string
 */
function monta_diarioFrequencia($param) {
	global $db;
	
	$param ['var'] = $param ['var'] ? $param ['var'] : 'pres';
	if (! is_array ( $param ['sql'] ) && $param ['sql'] != '') {
		$param ['sql'] = $db->carregar ( $param ['sql'] );
	}
	if (! is_array ( $param ['sql_marcados'] ) && $param ['sql_marcados'] != '') {
		$param ['sql_marcados'] = $db->carregar ( $param ['sql_marcados'] );
	}
	if (! is_array ( $param ['sql_periodo'] ) && $param ['sql_periodo'] != '') {
		$param ['sql_periodo'] = $db->carregar ( $param ['sql_periodo'] );
	}
	if (! is_array ( $param ['width'] )) {
		$param ['width'] = Array (
				'nomes' => '100px',
				'frequencias' => '500px' 
		);
	}
	
	$html = '<script language="javascript" type="text/javascript" src="../includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-1.4.2.min.js"></script>
			<script language="javascript" type="text/javascript">
				jQuery(document).ready(function()
				{
					jQuery(\'#incluirAula\').click(function(){
						var ano = jQuery(\'#novaData\').val().substr(5,5);
						var ano = jQuery(\'#novaData\').val().substr(5,5);
						if( jQuery(\'#novaData\').val() != \'\' ){
							if(verificaPeriodo(jQuery(\'#novaData\').val())){
								var aula = 1;
								if( jQuery(\'[name="data[\'+jQuery(\'#novaData\').val()+\']"]\') ){
									jQuery(\'[name="data[\'+jQuery(\'#novaData\').val()+\']"]\').each(function(){
										aula = parseInt(jQuery(this).val())+1;
									});
								}
								var ex = \'\';
								jQuery(\'[name="excluir"]\').each(function(){
									ex = parseInt(jQuery(this).val())+1;
								});
								if( ex == \'\' ){ ex = 1; }
								var html = \'<td width="10px"  style="background-color:#DCDCDC" height="50px;" align="center" class="col\'+replaceAll(jQuery(\'#novaData\').val(),\'/\',\'\')+aula+\'">\'+
											\'<img onclick="excluirColuna(\'+replaceAll(jQuery(\'#novaData\').val(),\'/\',\'\')+aula+\')" style="cursor:pointer;display:none;" class="excluirImg" id="excluir_\'+ex+\'" title="Excluir" src="../imagens/excluir.gif">\'+
											\'<b><center>\'+replaceAll(jQuery(\'#novaData\').val(),\'/\',\'  \').replace(\'  \',\'/\')+
											\'</center></b><input type="hidden" name="data[]" value="\'+jQuery(\'#novaData\').val()+\'"/>\'+
											\'<input type="hidden" name="data[\'+jQuery(\'#novaData\').val()+\']" value="\'+aula+\'"/>\'+
						  					\'<input type="hidden" name="excluir" value="\'+ex+\'"/></td>\';
								jQuery(\'#tituloLast\').before(html);
								jQuery(\'.matriculas\').each(function(){
									var matricula = jQuery(this).val();
									html =  \'<td width="10px" class="col[\'+jQuery(\'#novaData\').val()+\'][\'+aula+\']">\'+
												\'<center>\'+
													\'<select id="estuf" style="width: auto" class="CampoEstilo ' . $param ['var'] . '" name="' . $param ['var'] . '[\'+matricula+\'][\'+jQuery(\'#novaData\').val()+\'][\'+aula+\']">\'+
														\'<option value=" "></option>\'+
														\'<option value="P">P</option>\'+
														\'<option value="F">F</option>\'+
													\'</select>\'+
												\'</center>\'+
											\'</td>\';
									jQuery(\'#\'+matricula+\'Last\').before(html);
								});
								mostraBotaoExlcuir()
							}
						}
					});
					
					mostraBotaoExlcuir();
				});
				
				function verificaPeriodo( data ){
					var dia = data.split(\'/\');
					dia = dia[2]+dia[1]+dia[0];
					var inicio = jQuery(\'#inicio\').val();
					if( jQuery(\'#data_maior\').val() != \'\' ){
						var inicio = jQuery(\'#data_maior\').val();
					}
					inicio = inicio.split(\'/\');
					inicio = inicio[2]+inicio[1]+inicio[0];
					var fim = jQuery(\'#fim\').val();
					fim = fim.split(\'/\');
					fim = fim[2]+fim[1]+fim[0];
					if( parseInt(inicio) <= parseInt(dia) && parseInt(fim) >= parseInt(dia) ){
						jQuery(\'#data_maior\').val(data);
						return true;
					}
				}
				
				function mostraBotaoExlcuir(){
					var botao;
					jQuery(\'.excluirImg\').hide();
					jQuery(\'[name="excluir"]\').each(function(){
						botao = jQuery(this).val();
					});
					jQuery(\'#excluir_\'+botao).show();
				}
				
				function excluirColuna( coluna ){
					jQuery(\'.col\'+coluna).each(function(){
						jQuery(this).remove();
					});
					mostraBotaoExlcuir();
				}
				
			</script>
			<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
				<tr>
					<td>' . campo_data2 ( 'novaData', 'N', 'S', 'Nova Aula', '##/##/####' ) . '
						<input value="Incluir Aula" type="button" id="incluirAula"/>';
	$html .= '</td>
				</tr>
				<tr>
					<td>
			<div style="width:' . $param ['width'] ['nomes'] . ';overflow-x:scroll;float:left;border-width:0px;" >
			<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center" boder="0" style="width:100%">
				<tr>
					<td width="20%" height="50px;" style="background-color:#DCDCDC"><b>Matricula</b></td>
					<td width="80%" style="background-color:#DCDCDC"><b>Aluno</b></td>
				</tr>';
	if (is_array ( $param ['sql'] )) {
		$i = 0;
		foreach ( $param ['sql'] as $ro ) {
			$html .= '<tr ' . ($i % 2 == 0 ? 'style="background-color:white"' : '') . '>
							<td height="25px;">' . $ro ['codigo'] . '</td>
							<td>' . $ro ['descricao'] . '</td>
						  </tr>';
			$i ++;
		}
	}
	$html .= '</table>
			</div>
			<div style="width:' . $param ['width'] ['frequencias'] . ';overflow-x:scroll;float:left;border-width:0px;">
			<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center"  style="width:100%;border-left-width:0px">
				<tr>
					';
	if (is_array ( $param ['sql_marcados'] )) {
		$atualMat = '';
		$atualDat = '';
		$marcados = Array ();
		$ex = 1;
		foreach ( $param ['sql_marcados'] as $r ) {
			if ($atualDat != $r ['data']) {
				$atualDat = $r ['data'];
			}
			if ($atualMat != $r ['codigo']) {
				$atualMat = $r ['codigo'];
			}
			$marcados [$atualMat] [$atualDat] [$r ['ordem']] = $r ['valor'];
			$data = explode ( '/', $r ['data'] );
			if ($r ['codigo'] == $param ['sql_marcados'] [0] ['codigo']) {
				$html .= '<td width="10px"  style="background-color:#DCDCDC" height="50px;" align="center" class="col' . $data [0] . '' . $data [1] . '' . $data [2] . $r ['ordem'] . '"><b>
						  <img onclick="excluirColuna(\'' . $data [0] . '' . $data [1] . '' . $data [2] . $r ['ordem'] . '\')" style="cursor:pointer;display:none;" class="excluirImg" id="excluir_' . $ex . '" title="Excluir" src="../imagens/excluir.gif">
						  <input type="hidden" name="excluir" value="' . $ex . '"/>
						  <input type="hidden" name="data[]" value="' . $data [0] . '/' . $data [1] . '/' . $data [2] . '"/>
						  <input type="hidden" name="data[' . $data [0] . '/' . $data [1] . '/' . $data [2] . ']" value="' . $r ['ordem'] . '"/>
						  ' . $data [0] . '/' . $data [1] . ' ' . $data [2] . '</b></td>';
			}
			$ex ++;
		}
	}
	$html .= '<td id="tituloLast" style="background-color:#DCDCDC" height="50px;"><b>Faltas</b></td>
				</tr>';
	if (is_array ( $param ['sql'] )) {
		$i = 0;
		foreach ( $param ['sql'] as $ro ) {
			$html .= '<tr' . ($i % 2 == 0 ? 'style="background-color:white"' : '') . '>';
			if (is_array ( $marcados [$ro ['codigo']] )) {
				foreach ( $marcados [$ro ['codigo']] as $data => $r ) {
					foreach ( $r as $k => $pres ) {
						$html .= '<td width="10px" height="25px;" class="col' . str_replace ( '/', '', $data ) . $k . '">
										<center>
											<select id="estuf" style="width: auto" class="CampoEstilo ' . $param ['var'] . '" 
												name="' . $param ['var'] . '[' . $ro ['codigo'] . '][' . $data . '][' . $k . ']">
												<option value=" " ' . ($pres == ' ' ? 'selected' : '') . '></option>
												<option value="P" ' . ($pres == 'P' ? 'selected' : '') . '>P</option>
												<option value="F" ' . ($pres == 'F' ? 'selected' : '') . '>F</option>
											</select>
										</center>
									  </td>';
						$dtLst = $data;
					}
				}
			}
			$html .= '<td id="' . $ro ['codigo'] . 'Last" height="25px;"><input type="hidden" class="matriculas" value="' . $ro ['codigo'] . '"/>0</td>
						  </tr>';
			$i ++;
		}
	}
	$html .= '</table>';
	if (is_array ( $param ['sql_periodo'] )) {
		$html .= '		<input type="hidden" id="inicio" value="' . $param ['sql_periodo'] ['inicio'] . '" />
						<input type="hidden" id="fim" value="' . $param ['sql_periodo'] ['fim'] . '" />
						<input type="hidden" id="data_maior" value="' . $dtLst . '" />';
	}
	$html .= '</div>
					</td>
				</tr>
			</table>';
	echo $html;
}
function buscaPolos($idPolo = null) {
	global $db;
	
	$complemento = (! empty ( $idPolo ) ? " and pjuid=" . $idPolo : '');
	if($_SESSION['projovemurbano']['ppuid'] == 3){
		$filtrotprid = "AND tprid='".$_SESSION['projovemurbano']['tprid']."'";
	}
	$sqlPolos = "SELECT pmupossuipolo 
        		FROM projovemurbano.polomunicipio 
        			WHERE pmustatus='A' $filtrotprid " . $complemento . " ";
	
	$resultado = $db->pegaLinha ( $sqlPolos );
	
	return $resultado;
}
function listaAgencias($paramentros) {
	
	// Definindo os valores dos argumentos do webservice
	$sgUf = $_SESSION ['projovemurbano'] ['estuf']; // Definindo como Distrito Federal
	$codIbge = $paramentros ['muncod']; // 3514403
	$nuRaioKm = $paramentros ['uraiokm']; // 10
	
	$cliente = new SoapClient ( "http://ws.mec.gov.br/AgenciasBb/wsdl" );
	$xmlDeRespostaDoServidor = $cliente->getMunicipio ( $codIbge, $nuRaioKm );
	
	$agencias = new SimpleXMLElement ( $xmlDeRespostaDoServidor );
	$retorno = array ();
	
	foreach ( $agencias->NODELIST as $agencia ) {
		
		$coAgencia = $agencia->co_agencia . '-' . $agencia->nu_dv . '-' . utf8_encode ( $agencia->no_agencia );
		$arrAgencia = array (
				'co_agencia' => $agencia->co_agencia . '',
				'co_banco' => $agencia->co_banco . '',
				'dv' => $agencia->nu_dv . '',
				'agencia_dv' => $coAgencia,
				'no_agencia' => utf8_encode ( $agencia->no_agencia . '' ) 
		);
		
		$retorno [] = $arrAgencia;
	}
	
	return $retorno;
}
function listaDeEncaminhamentoPerfilEquipeMEC($dados) {
	global $db;
	$retorno = '';
	
	$nucid = ! empty ( $dados ['nucid'] ) ? ' AND nuc.nucid  IN (' . $dados ['nucid'] . ')' : '';
	$polid = ! empty ( $dados ['polid'] ) ? ' AND pol.polid  IN (' . $dados ['polid'] . ')' : '';
	$dadosEstuf = $dados ['estuf'];
	$estuf = ! empty ( $dados ['estuf'] ) ? " AND ( tmun.estuf IN ('{$dadosEstuf}') or pju.estuf IN ('{$dadosEstuf}')  )" : "";
	$esfera = '';
	
	if ($dados ['esfera'] == 'M') {
		$esfera = " AND pju.muncod is not null";
	} elseif ($dados ['esfera'] == 'E') {
		$esfera = " AND pju.muncod is null";
	}
	
	$estudantesaptos = ! empty ( $dados ['estudantesaptos'] ) ? ' AND pge.pgeaptoreceber = true' : '';
	$estudantesinaptos = ! empty ( $dados ['estudantesinaptos'] ) ? ' AND pge.pgeaptoreceber = false' : '';
	$mundescricao = ! empty ( $dados ['mundescricao'] ) ? " AND tmun.mundescricao ilike '%" . utf8_decode ( $dados ['mundescricao'] ) . "%'" : '';
	$naopagamento = ! empty ( $dados ['naopagamento'] ) ? ' AND esd.esdid <> ' . WF_ESTADO_DIARIO_PAGAMENTO : '';
	$simpagamento = ! empty ( $dados ['simpagamento'] ) ? ' AND esd.esdid =  ' . WF_ESTADO_DIARIO_PAGAMENTO : '';
	$esdid = ! empty ( $dados ['esdid'] ) ? " AND esd.esdid in(" . $dados ['esdid'] . ")" : '';
	
	$wherefiltro = $estudantesaptos . $estudantesinaptos . $naopagamento . $simpagamento . $nucid . $polid . $mundescricao . $estuf . $esfera . $esdid;
	
	$wherefiltrotransferido = $estudantesaptos . $estudantesinaptos . $naopagamento . $simpagamento . $nucid . $polid . $mundescricao . $estuf . $esfera . $esdid;
	
	if (! empty ( $_REQUEST [''] )) {
		$parametros ['estudantesaptos'] = $_REQUEST ['estudantesaptos'];
	}
	
	$dados ['from'] = sprintf ( " esd.esdid = %d OR esd.esdid = %d ", WF_ESTADO_DIARIO_APROVACAO, WF_ESTADO_DIARIO_VALIDACAO );
	$dados ['inner'] = "";
	$dados ['where'] = " AND cae.caestatus = 'A'
						AND dia.perid     = " . $dados ['perid'] . $wherefiltro;
	
	$dados ['wheretransferidos'] = "AND cae.caestatus = 'A'
								   AND dia.perid     = " . $dados ['perid'] . $wherefiltrotransferido;
	// ver($dados['$wheretransferidos'],d);
	$sql = listaDeEncaminhamentoPerfilSQL ( $dados );
	
	// echo "PerfilEquipeMEC <pre>";print( $sql );exit;
	
	$retorno = $db->carregar ( $sql );
	$db->commit ();
	
	return $retorno;
}
function listaDeEncaminhamentoPerfilDiretorDePolo($dados) {
	global $db;
	$retorno = '';
	
	$dados ['from'] = sprintf ( " esd.esdid = %d OR esd.esdid = %d ", WF_ESTADO_DIARIO_ENCAMINHAR, WF_ESTADO_DIARIO_FECHADO );
	
	$dados ['inner'] = "--PerfilDiretorDePolo
	                   LEFT JOIN projovemurbano.usuarioresponsabilidade rpu 
								ON rpu.polid = pol.polid AND rpu.rpustatus = 'A' ";
	
	$dados ['where'] = "--PerfilDiretorDePolo
	                   AND cae.caestatus = 'A'
					   AND pju.pjuid     = " . $dados ['pjuid'] . "
					   AND dia.perid     = " . $dados ['perid'] . "
					   AND rpu.usucpf    = '" . $dados ['usucpf'] . "'";
	
	$dados ['wheretransferidos'] = "--PerfilDiretorDePolo
	                   AND cae.caestatus = 'A'
					   --AND pju.pjuid     = " . $dados ['pjuid'] . "
					   AND dia.perid     = " . $dados ['perid'] . "
					   AND rpu.usucpf    = '" . $dados ['usucpf'] . "'";
	
	$sql = listaDeEncaminhamentoPerfilSQL ( $dados );
	
	// echo "listaDeEncaminhamentoPerfilDiretorDePolo: <pre>";print( $sql );exit;
	
	$retorno = $db->carregar ( $sql );
	$db->commit ();
	
	return $retorno;
}
function listaDeEncaminhamentoPerfilCoordenadorEstadual($dados) {
	global $db;
	$retorno = '';
	
	$dados ['from'] = sprintf ( " esd.esdid = %d OR esd.esdid = %d OR esd.esdid = %d", WF_ESTADO_DIARIO_VALIDACAO, WF_ESTADO_DIARIO_FECHADO, WF_ESTADO_DIARIO_ENCAMINHAR );
	
	$dados ['inner'] = "--PerfilCoordenadorEstadual
	                   LEFT JOIN projovemurbano.coordenadorresponsavel cr
								ON pju.pjuid = cr.pjuid";
	
	$dados ['where'] = "--PerfilCoordenadorEstadual
	                   AND cae.caestatus = 'A'
					   AND pju.pjuid     = " . $dados ['pjuid'] . "
					   AND dia.perid     = " . $dados ['perid'] . "
					   AND cr.corcpf     = '" . $dados ['usucpf'] . "'";
	
	$dados ['wheretransferidos'] = "--PerfilCoordenadorEstadual
	                   AND cae.caestatus = 'A'
					   --AND pju.pjuid     = " . $dados ['pjuid'] . "
					   AND dia.perid     = " . $dados ['perid'] . "
					   AND cr.corcpf     = '" . $dados ['usucpf'] . "'";
	$sql = listaDeEncaminhamentoPerfilSQL ( $dados );
	
	// echo "listaDeEncaminhamentoPerfilSQL_Coordenador: <pre>";print( $sql );exit;
	
	$retorno = $db->carregar ( $sql );
	$db->commit ();
	
	return $retorno;
}
function listaDeEncaminhamentoPerfilCoordenadorMunicipal($dados) {
	return listaDeEncaminhamentoPerfilCoordenadorEstadual ( $dados );
}
function listaDeEncaminhamentoPerfilDiretorDeNucleo($dados) {
	global $db;
	$retorno = '';
	
	$dados ['from'] = sprintf ( " esd.esdid = %d OR esd.esdid = %d OR esd.esdid = %d ", WF_ESTADO_DIARIO_FECHADO, WF_ESTADO_DIARIO_ENCAMINHAR, WF_ESTADO_DIARIO_VALIDACAO );
	
	$dados ['inner'] = "--PerfilDiretorDeNucleo
                        LEFT JOIN projovemurbano.usuarioresponsabilidade urn1 
                            ON urn1.nucid = nuc.nucid AND urn1.rpustatus='A'
                        LEFT JOIN projovemurbano.usuarioresponsabilidade urn2 
                            ON urn2.entid = ent.entid AND urn2.rpustatus='A' ";
	
	$dados ['where'] = "--PerfilDiretorDeNucleo 
	                   AND cae.caestatus = 'A'
					   AND pju.pjuid     = " . $dados ['pjuid'] . "
					   AND dia.perid     = " . $dados ['perid'] . "
					    AND (urn1.usucpf  = '" . $dados ['usucpf'] . "' or urn2.usucpf = '" . $dados ['usucpf'] . "')";
	$dados ['wheretransferidos'] = "--PerfilDiretorDeNucleo 
	                   AND cae.caestatus = 'A'
					   --AND pju.pjuid     = " . $dados ['pjuid'] . "
					   AND dia.perid     = " . $dados ['perid'] . "
                       AND urn2.usucpf  = '" . $dados ['usucpf'] . "'";
	
	$sql = listaDeEncaminhamentoPerfilSQL ( $dados );
	
	// echo "listaDeEncaminhamentoPerfilDiretorDeNucleo: <pre>";print( $sql );exit;
	
	$retorno = $db->carregar ( $sql );
	$db->commit ();
	
	return $retorno;
}

/**
 * Verifica se os alunos de um diário
 * estão aptos para receber o benefício
 * Executa no pós-ação entre as fases:
 * Aberto -> Fechado
 *
 * @global cls_banco $db
 * @param int $diaid        	
 * @return boolean
 * @todo coloca try/catch
 * @todo verificar regra de auxílio
 * @todo testar
 */
function verificaAlunoApto($diaid) {
	global $db;
	
	$sqlTotalAulasDadas = "SELECT SUM(difqtdauladada) AS soma_difqtdauladada
                            FROM projovemurbano.diariofrequencia dif
                            INNER JOIN projovemurbano.gradecurricular grd
                                ON dif.grdid = grd.grdid
                            INNER JOIN projovemurbano.componentecurricular coc
                                ON grd.cocid = coc.cocid
                            WHERE diaid = {$diaid}";
	
	$sqlAluno = "SELECT cae.caeid, cae.caenome, cae.caenispispasep
                        , nab.nabid, presenca_estudante.total_presenca_componente
                        , trabalho_aluno.total_trabalho_entregue
                        , cae.caeqtddireitobolsa
                        ,caeqtdbolsaprojovem
                    FROM projovemurbano.diario dia
                    INNER JOIN projovemurbano.turma tur
                        ON dia.turid = tur.turid
                    INNER JOIN projovemurbano.cadastroestudante cae
                        ON tur.turid = cae.turid
                    INNER JOIN projovemurbano.nucleo nuc
                        ON tur.nucid = nuc.nucid
                    LEFT JOIN projovemurbano.nucleoagenciabancaria nab
                        ON nuc.nucid = nab.nucid
                    LEFT JOIN 
                        (
                        SELECT cae.caeid, cae.caenome	
                            , COALESCE(SUM( frq.frqqtdpresenca ), 0 )  as total_presenca_componente
                        from projovemurbano.diario dia 	
                        INNER JOIN projovemurbano.diariofrequencia dif 
                            ON dia.diaid = dif.diaid AND dia.diaid = {$diaid}
                        INNER JOIN projovemurbano.cadastroestudante cae
                            ON dia.turid =  cae.turid
                            AND cae.caestatus = 'A'
                        INNER JOIN projovemurbano.gradecurricular grd 
                            ON grd.grdid = dif.grdid
                        INNER JOIN projovemurbano.componentecurricular coc 
                            ON coc.cocid = grd.cocid AND coc.cocdisciplina = 'D'
                        LEFT JOIN projovemurbano.frequenciaestudante frq 
                            ON dif.difid  	= frq.difid  
                            AND cae.caeid	= frq.caeid
                        GROUP BY cae.caeid,cae.caenome
                        ORDER BY cae.caenome
                        ) as presenca_estudante
                        ON cae.caeid = presenca_estudante.caeid


                    LEFT JOIN 
                        (
                        SELECT cae.caeid, cae.caenome, COUNT ( frq.frqid ) as total_trabalho_entregue
                        , COUNT ( frq2.frqid ) as total_trabalho_nao_entregue
                        FROM projovemurbano.diario dia
                        INNER JOIN projovemurbano.diariofrequencia dif
                            ON dia.diaid = dif.diaid AND dia.diaid = {$diaid}	
                        INNER JOIN projovemurbano.gradecurricular grd
                            ON dif.grdid = grd.grdid
                        INNER JOIN projovemurbano.componentecurricular coc
                            ON grd.cocid = coc.cocid AND coc.cocdisciplina = 'T'
                        INNER JOIN projovemurbano.cadastroestudante cae
                            ON dia.turid = cae.turid AND cae.caestatus	= 'A'
                        LEFT JOIN projovemurbano.frequenciaestudante frq
                            ON dif.difid 		= frq.difid 
                            AND cae.caeid 		= frq.caeid	
                            AND frq.frqtrabalho 	= 't'
                            AND frq.frqstatus = 'A'
                        LEFT JOIN projovemurbano.frequenciaestudante frq2
                            ON dif.difid = frq2.difid 
                            AND cae.caeid = frq2.caeid	
                            AND frq2.frqtrabalho 	= 'f'
                            AND frq2.frqstatus = 'A'	
                        GROUP BY cae.caeid, cae.caenome
                        ) as trabalho_aluno
                        ON cae.caeid = trabalho_aluno.caeid
                    WHERE dia.diaid = {$diaid} 
                    ORDER BY caenome";
	// ver($sql,d);
	$dadosAlunos = $db->carregar ( $sqlAluno );
	$totalAulasDadas = $db->pegaUm ( $sqlTotalAulasDadas );
	
	if ($totalAulasDadas == 0) {
		throw new Exception ( 'Não foi possível fechar o diário. Informe a quantidade de aulas dadas' );
	}
	
	foreach ( $dadosAlunos as $aluno ) {
		
		$apto = "t";
		$porcentagemPresenca = (($aluno ['total_presenca_componente'] * 100) / $totalAulasDadas);
		
		// aluno com menos de 3 trabalhos
		if ($aluno ['total_trabalho_entregue'] < 3) {
			$apto = "f";
		}
		
		// aluno sem agência ou nis
		if (empty ( $aluno ['nabid'] )/* || empty ( $aluno ['caenispispasep'] ) || $aluno ['caenispispasep'] == 0*/) {
			$apto = "f";
		}
		
		// aluno sem 75% de presenca
		if ($porcentagemPresenca < 0.75) {
			$apto = "f";
		}
		
		// aluno com direito a receber a bolsa
		if ($aluno ['caeqtddireitobolsa'] - $aluno ['caeqtdbolsaprojovem'] <= 0) {
			$apto = "f";
		}
		
		$sqlVerificaPagamento = "SELECT * FROM projovemurbano.pagamentoestudante 
        						WHERE caeid = {$aluno['caeid']} AND diaid = {$diaid}";
		$VerificaPagamento = $db->pegaLinha ( $sqlVerificaPagamento );
		
		$docId = wf_cadastrarDocumento ( WORKFLOW_TIPODOCUMENTO_PAGAMENTO, 'Fluxo do pagamento de estudante' );
		
		if (! $VerificaPagamento ['pgeid']) {
			$sqlInserePagamento = "INSERT INTO projovemurbano.pagamentoestudante( caeid, diaid, pgeaptoreceber, docid ) 
            							VALUES( {$aluno['caeid']}, {$diaid}, '{$apto}', {$docId}) RETURNING pgeid";
			$db->pegaUm ( $sqlInserePagamento );
			
			$sqlAtualizaAptoAReceber = "SELECT projovemurbano.fn_situacao_aluno_pagto({$aluno['caeid']}, {$diaid});";
			$db->pegaUm ( $sqlAtualizaAptoAReceber );
		} elseif ($VerificaPagamento ['pgeid'] && $VerificaPagamento ['pgeaptoreceber'] != 't' && $VerificaPagamento ['pgeaptoreceber'] != $apto) {
			
			$atualizapagamento = "UPDATE projovemurbano.pagamentoestudante
								  SET pgeaptoreceber = 't',
									  docid = {$docId}
								  WHERE
								  	  caeid = {$aluno['caeid']}
								  AND diaid = {$diaid}";
			$db->pegaUm ( $atualizapagamento );
			
			$sqlAtualizaAptoAReceber = "SELECT projovemurbano.fn_situacao_aluno_pagto({$aluno['caeid']}, {$diaid});";
			$db->pegaUm ( $sqlAtualizaAptoAReceber );
		}
	}
	// ver($sqlInserePagamento);
	$db->commit ();
	
	return true;
}
function listaDeEncaminhamentoPerfilSQL($paramentros) {
	global $db;
	$where = $paramentros ['where'];
	$inner = $paramentros ['inner'];
	$from = $paramentros ['from'];
	$pflcod = PFL_DIRETOR_NUCLEO;
	$wheretransferidos = $paramentros ['wheretransferidos'];
	
	$perfis = pegaPerfilGeral ();
	if($_SESSION['projovemurbano']['ppuid']=='3'){
		$wheretprid =  "AND tprid = {$_SESSION['projovemurbano']['tprid']}";
	}
	$sqlpossuipolo = "SELECT DISTINCT pmupossuipolo FROM projovemurbano.polomunicipio WHERE pjuid = {$_SESSION['projovemurbano']['pjuid']} $wheretprid";
	$possuipolo = $db->pegaUm ( $sqlpossuipolo );
	if ($possuipolo == 't') {
		$wherepossuipolo = "AND pol.polid is not null";
	} else {
		$wherepossuipolo = "AND pol.polid is null";
	}
	
	$sql = "
    	select DISTINCT * from ( (SELECT	DISTINCT
		    --Por esfera estadual ou muncipal
		    CASE WHEN pju.muncod IS NULL
				THEN 'Estadual'
				ELSE 'Municipal'
			END as esfera,
	
		    --Informações do Estado
		    COALESCE(pju.estuf,tmun.estuf) as estuf, 
		    COALESCE(tmun.mundescricao,'Esfera Estadual') as mundescricao, 
		
		    --Informações do Polo
		    coalesce(pol.polid, '0' ) AS polid,
		    'Polo ' || coalesce(pol.polid::text, 'Não consta' ) AS polo,
	
			CASE WHEN pju.muncod IS NULL
				THEN 'Estadual_'  || COALESCE(pju.estuf,tmun.estuf) || '_' || coalesce(pol.polid::text, 'Nao_consta' )
				ELSE 'Municipal_' || COALESCE(pju.estuf,tmun.estuf) || '_' || coalesce(pol.polid::text, 'Nao_consta' )
			END  
			|| '_' || replace(COALESCE(tmun.mundescricao,'Esfera_Estadual'),' ','_') as chave_polo,
		
		    --Informações do Núcleo
		    nuc.nucid || usu.usucpf as cpfnucleo,
		    nuc.nucid,
		    'NÚCLEO '|| nuc.nucid || ' - DIRETOR: ' || usu.usunome as nucleo,
		    --CASE WHEN nes.nuetipo = 'S' THEN 'SEDE : ' ELSE 'ANEXO : ' END as escola,
		
		    --Informações da Turma
		    tur.turid as turid,
		    tur.turdesc,
		
		    --Informações do Estudante
		    cae.caeid as matricula,
		    cae.caenome as estudante,
		    dia.diaid,
		    
			--Trabalhos Entrgues
		    COALESCE(
		    	(
			    	select sum(case when frq.frqtrabalho = true then 1 else 0 end) x
			    	from projovemurbano.diariofrequencia dif
			    	left join projovemurbano.frequenciaestudante frq ON frq.difid = dif.difid AND frq.caeid = cae.caeid AND frq.frqtrabalho = true
			    	inner join projovemurbano.gradecurricular grd on grd.grdid = dif.grdid
			    	inner join projovemurbano.componentecurricular coc on coc.cocid = grd.cocid
			    	where dif.diaid = dia.diaid and coc.cocdisciplina = 'T'
		    	), 0 
		    ) as trabalhosentregues,
		    
		    --Frequencia.	 
			(select case 
						when coalesce( SUM(dif.difqtdauladada), 0 ) = 0 then 0
	    				when coalesce( SUM(frq.frqqtdpresenca), 0 ) = 0 then 0
	    				else coalesce( ROUND(SUM(frq.frqqtdpresenca)::NUMERIC/SUM(dif.difqtdauladada)::NUMERIC*100::NUMERIC,2) ,0)
	    			end
	    	from projovemurbano.diariofrequencia dif
	    	left join projovemurbano.frequenciaestudante frq ON frq.difid = dif.difid AND frq.caeid = cae.caeid
	    	inner join projovemurbano.gradecurricular grd on grd.grdid = dif.grdid
	    	inner join projovemurbano.componentecurricular coc on coc.cocid = grd.cocid
	    	where dif.diaid = dia.diaid and coc.cocdisciplina = 'D'
	    	) as frequencia,
	    	
			--Alteração feita em 25/11/2013 a pedido do Júlio, a mando da Hellem
			--COALESCE(cae.caeqtddireitobolsa,0)  as auxilios,
	    	CASE 
	    		WHEN (cae.caeqtddireitobolsa - cae.caeqtdbolsaprojovem) > 0
	    		THEN (cae.caeqtddireitobolsa - cae.caeqtdbolsaprojovem)
	    		ELSE 0
	    		END  as auxilios,
	    		
			CASE WHEN pge.pgeaptoreceber = true 
				THEN 'SIM'
		    	ELSE 'NÃO'
		    END as aptoreceber,
			COALESCE(nab.agbcod ||'-' || nab.agbdv,'-') as agencia,
		    CASE WHEN LENGTH(cae.caenispispasep) = 0 or LENGTH(cae.caenispispasep) = 1
		    	THEN '-'
		    	ELSE COALESCE(cae.caenispispasep,'-')
		    END AS caenispispasep,
		    esd.esdid as estadodocumento,
		    
		    (
		    	select max(cd.cmdid)
		       	from workflow.comentariodocumento cd 
		       	inner join workflow.historicodocumento ht on cd.hstid = ht.hstid
		       	where ht.docid = dia.docid 
		    ) as cmddsc,
		
		    coalesce(dia.docid,0) as docid,
		    'regular' as tipo_aluno

	    FROM projovemurbano.cadastroestudante cae
	
	    INNER JOIN projovemurbano.turma tur ON tur.turid = cae.turid AND tur.turstatus='A'
	    
	    INNER JOIN projovemurbano.nucleoescola nes ON nes.entid = tur.entid AND nes.nuestatus='A' -- and nes.nucid = tur.nucid
	    
	    INNER JOIN projovemurbano.diario dia ON tur.turid = dia.turid
	    LEFT JOIN projovemurbano.pagamentoestudante pge ON pge.diaid = dia.diaid AND pge.caeid = cae.caeid
	    INNER JOIN projovemurbano.projovemurbano pju ON pju.pjuid = cae.pjuid AND pju.pjustatus='A'
	    INNER JOIN projovemurbano.nucleo nuc ON nuc.nucid = tur.nucid AND nuc.nucstatus = 'A'
	
	    INNER JOIN entidade.entidade ent ON ent.entid = tur.entid AND ent.entstatus='A'
	    INNER JOIN projovemurbano.municipio mun ON mun.munid = nuc.munid AND mun.munstatus='A'
	    
	    --INNER JOIN territorios.municipio tmun ON tmun.muncod = mun.muncod
	    LEFT JOIN territorios.municipio tmun ON tmun.muncod = pju.muncod
	    
	    LEFT JOIN projovemurbano.associamucipiopolo amp ON amp.munid = mun.munid AND amp.ampstatus='A'
	    LEFT JOIN projovemurbano.polo pol ON pol.polid = amp.polid AND pol.polstatus='A'
	
	    LEFT JOIN projovemurbano.nucleoagenciabancaria nab on nuc.nucid = nab.nucid
	    
	    INNER JOIN projovemurbano.usuarioresponsabilidade ur ON ur.entid=ent.entid AND ur.rpustatus='A' AND ur.pflcod = {$pflcod} 
	    
	    INNER JOIN seguranca.usuario usu ON usu.usucpf = ur.usucpf
	
	    {$inner}
	
	    INNER JOIN workflow.documento dd ON dd.docid  = dia.docid
	    INNER JOIN workflow.estadodocumento esd ON esd.esdid = dd.esdid
	
	    WHERE 1=1
	    $wherepossuipolo
	    {$where}
	
	    
	    ) UNION ALL (
	    
	    SELECT	DISTINCT
		    --Por esfera estadual ou muncipal
		    CASE WHEN pju.muncod IS NULL
				THEN 'Estadual'
				ELSE 'Municipal'
			END as esfera,
	
		    --Informações do Estado
		    COALESCE(pju.estuf,tmun.estuf) as estuf, 
		    COALESCE(tmun.mundescricao,'Esfera Estadual') as mundescricao, 
		
		    --Informações do Polo
		    coalesce(pol.polid, '0' ) AS polid,
		    'Polo ' || coalesce(pol.polid::text, 'Não consta' ) AS polo,
	
			CASE WHEN pju.muncod IS NULL
				THEN 'Estadual_'  || COALESCE(pju.estuf,tmun.estuf) || '_' || coalesce(pol.polid::text, 'Nao_consta' )
				ELSE 'Municipal_' || COALESCE(pju.estuf,tmun.estuf) || '_' || coalesce(pol.polid::text, 'Nao_consta' )
			END  
			|| '_' || replace(COALESCE(tmun.mundescricao,'Esfera_Estadual'),' ','_') as chave_polo,
		
		    --Informações do Núcleo
		    nuc.nucid || usu.usucpf as cpfnucleo,
		    nuc.nucid,
		    'NÚCLEO '|| nuc.nucid || ' - DIRETOR: ' || usu.usunome as nucleo,
		    --CASE WHEN nes.nuetipo = 'S' THEN 'SEDE : ' ELSE 'ANEXO : ' END as escola,
		
		    --Informações da Turma
		    tur.turid as turid,
		    tur.turdesc ,
		
		    --Informações do Estudante
		    cae.caeid as matricula,
		    cae.caenome as estudante,
		    dia.diaid,
		    
			--Trabalhos Entrgues
		    COALESCE(
		    	(
			    	select sum(case when frq.frqtrabalho = true then 1 else 0 end) x
			    	from projovemurbano.diariofrequencia dif
			    	left join projovemurbano.frequenciaestudante frq ON frq.difid = dif.difid AND frq.caeid = cae.caeid AND frq.frqtrabalho = true
			    	inner join projovemurbano.gradecurricular grd on grd.grdid = dif.grdid
			    	inner join projovemurbano.componentecurricular coc on coc.cocid = grd.cocid
			    	where dif.diaid = dia.diaid and coc.cocdisciplina = 'T'
		    	), 0 
		    ) as trabalhosentregues,
		    
		    --Frequencia.	 
			(select case 
						when coalesce( SUM(dif.difqtdauladada), 0 ) = 0 then 0
	    				when coalesce( SUM(frq.frqqtdpresenca), 0 ) = 0 then 0
	    				else coalesce( ROUND(SUM(frq.frqqtdpresenca)::NUMERIC/SUM(dif.difqtdauladada)::NUMERIC*100::NUMERIC,2) ,0)
	    			end
	    	from projovemurbano.diariofrequencia dif
	    	left join projovemurbano.frequenciaestudante frq ON frq.difid = dif.difid AND frq.caeid = cae.caeid
	    	inner join projovemurbano.gradecurricular grd on grd.grdid = dif.grdid
	    	inner join projovemurbano.componentecurricular coc on coc.cocid = grd.cocid
	    	where dif.diaid = dia.diaid and coc.cocdisciplina = 'D'
	    	) as frequencia,
	    	
	    	--Alteração feita em 25/11/2013 a pedido do Júlio, a mando da Hellem
			--COALESCE(cae.caeqtddireitobolsa,0)  as auxilios,
	    	CASE 
	    		WHEN (cae.caeqtddireitobolsa - cae.caeqtdbolsaprojovem) > 0
	    		THEN (cae.caeqtddireitobolsa - cae.caeqtdbolsaprojovem)
	    		ELSE 0
	    		END  as auxilios,
			CASE WHEN pge.pgeaptoreceber = true 
				THEN 'SIM'
		    	ELSE 'NÃO'
		    END as aptoreceber,
			COALESCE(nab.agbcod ||'-' || nab.agbdv,'-') as agencia,
		    CASE WHEN LENGTH(cae.caenispispasep) = 0 or LENGTH(cae.caenispispasep) = 1
		    	THEN '-'
		    	ELSE COALESCE(cae.caenispispasep,'-')
		    END AS caenispispasep,
		    esd.esdid as estadodocumento,
		    
		    (
		    	select max(cd.cmdid)
		       	from workflow.comentariodocumento cd 
		       	inner join workflow.historicodocumento ht on cd.hstid = ht.hstid
		       	where ht.docid = dia.docid 
		    ) as cmddsc,
		
		    coalesce(dia.docid,0) as docid,
		    'transferido' as tipo_aluno

	    FROM        projovemurbano.transferencia tra
	    
	    INNER JOIN  projovemurbano.historico_transferencia hst ON hst.traid = tra.traid

	    INNER JOIN 	projovemurbano.cadastroestudante cae  ON cae.caeid = tra.cad_caeid
	     
	    INNER JOIN  projovemurbano.projovemurbano pju ON pju.pjuid = tra.pjuid_origem AND pju.pjustatus='A' 
	    
	    INNER JOIN  projovemurbano.turma tur ON tur.turid = tra.turid_origem AND tur.turstatus='A'
	    
	    INNER JOIN  projovemurbano.nucleoescola nes ON nes.entid = tur.entid AND nes.nuestatus='A' -- and nes.nucid = tur.nucid
	    
	    INNER JOIN  projovemurbano.diario dia ON tur.turid = dia.turid
	    LEFT JOIN   projovemurbano.pagamentoestudante pge ON pge.diaid = dia.diaid AND pge.caeid = cae.caeid
	    INNER JOIN  projovemurbano.nucleo nuc ON nuc.nucid = tra.nucid_origem AND nuc.nucstatus = 'A'
	
	    INNER JOIN  entidade.entidade ent ON ent.entid = tur.entid AND ent.entstatus='A'
	    
	    INNER JOIN  projovemurbano.municipio mun ON mun.munid = nuc.munid AND mun.munstatus='A'
	    
	    --INNER JOIN territorios.municipio tmun ON tmun.muncod = mun.muncod
	    LEFT JOIN   territorios.municipio tmun ON tmun.muncod = pju.muncod
	    
	    LEFT JOIN   projovemurbano.associamucipiopolo amp ON amp.munid = mun.munid AND amp.ampstatus='A'
	    LEFT JOIN   projovemurbano.polo pol ON pol.polid = amp.polid AND pol.polstatus='A'
	
	    LEFT JOIN   projovemurbano.nucleoagenciabancaria nab on nuc.nucid = nab.nucid
	    
	    INNER JOIN  projovemurbano.usuarioresponsabilidade ur ON ur.entid=ent.entid AND ur.rpustatus='A' AND ur.pflcod = 650 
	    
	    INNER JOIN  seguranca.usuario usu ON usu.usucpf = ur.usucpf
	
	    {$inner}
	
	    INNER JOIN workflow.documento dd ON dd.docid  = dia.docid
	    INNER JOIN workflow.estadodocumento esd ON esd.esdid = dd.esdid
	
	    WHERE 1=1
	    $wherepossuipolo
	    AND shtid_status = 3
	    {$wheretransferidos}
	
	   
	    )
	    ) as foo  ORDER BY  esfera, estuf, mundescricao, chave_polo, turid, cpfnucleo, estudante
    ";
	// echo "<pre>";print( $sql );//exit;
// 	ver($sql,d);
	return $sql;
}
function retornaNomeDaAgenciaCadastrada($paramentros) {
	global $db;
	$listaDeAgencias = array ();
	if($_SESSION['projovemurbano']['ppuid']== 3){
		$filtratprid = "AND nuc.tprid ='".$_SESSION['projovemurbano']['tprid']. "'";
	}
	$sql = "SELECT DISTINCT
				case when nab.nucid = nuc.nucid then
					" . $paramentros ['imgAgenciaVinculada'] . "
					" . $paramentros ['imgAgenciaComAcao'] . "
				else
					" . $paramentros ['imgAgenciaComAcao'] . "
				end as acao
				, nuc.nucid as nucleo
				, tmun.estuf AS uf
				, tmun.mundescricao AS municipio
				, ede.endlog ||',' ||ede.endnum|| '-' ||ede.endbai AS endereco
				, ede.endcep
				, case when nab.nabnomeagencia is null then
					nab.agbcod::varchar
				else
					nab.agbcod ||' / '|| nab.nabnomeagencia
				end as agencia
				, ede.muncod
				,case when nab.nabnomeagencia is null and nab.agbcod is not null then
					1
				else
					0
				end as corrige_agencia
				, nab.nabid
				, nab.agbcod
			FROM projovemurbano.nucleo nuc
			INNER JOIN projovemurbano.municipio pmun  					ON pmun.munid  = nuc.munid
			INNER JOIN territorios.municipio tmun 						ON tmun.muncod = pmun.muncod
			INNER JOIN projovemurbano.nucleoescola nes 					ON nes.nucid   = nuc.nucid
			INNER JOIN entidade.entidade ent 							ON nes.entid   = ent.entid
			INNER JOIN projovemurbano.cadastroestudante cae 			ON cae.nucid   = nuc.nucid
			LEFT OUTER JOIN projovemurbano.nucleoagenciabancaria nab 	ON nab.nucid   = nuc.nucid
			LEFT OUTER JOIN entidade.endereco ede 						ON ede.entid   = ent.entid
			WHERE
				nuc.nucstatus='A'
				AND pmun.munstatus='A'
       			
				AND cae.pjuid ='" . $_SESSION ['projovemurbano'] ['pjuid'] . "'
			ORDER BY
				nuc.nucid";
	$retorno = $db->carregar ( $sql );
	
	if ($retorno) {
		
		// Rotina para correção dos nomes das agências bancárias
		foreach ( $retorno as $chave => $valor ) {
			// Valida se registro precisa de correção e se tem código do muncipio
			if ($retorno [$chave] ['corrige_agencia'] == 1 && ! empty ( $retorno [$chave] ['muncod'] )) {
				// Se registro ainda não foi valido, então validá-lo
				if (! in_array ( $retorno [$chave] ['nabid'], $listaDeAgencias )) {
					// Chama Serviço de Agência
					$retornoWs = listaAgencias ( array (
							'muncod' => $retorno [$chave] ['muncod'],
							'uraiokm' => '500' 
					) );
					
					// Insere registro na lista para não ser validado novamente
					$listaDeAgencias [] = $retorno [$chave] ['nabid'];
					
					// Lista de agências retornadas pelo Serviço WS
					foreach ( $retornoWs as $agencias ) {
						if ($retorno [$chave] ['agbcod'] == $agencias ['co_agencia']) {
							$sqlCorrecaoUpdate = "update projovemurbano.nucleoagenciabancaria
												  set nabnomeagencia = '" . $agencias ['no_agencia'] . "'
												  , nabdtatualizacao = current_timestamp
												  where nabid = " . $retorno [$chave] ['nabid'];
							
							$db->carregar ( $sqlCorrecaoUpdate );
						}
					}
				}
			}
		}
		
		$db->commit ();
		
		// Retorna uma nova consulta
		$retorno = $db->carregar ( $sql );
		
		// Remove os campos que não entrarão na tabela
		foreach ( $retorno as $chave => $valor ) {
			// Remove último registro do ARRAY, senão remover aparece no componente da tabela.
			array_pop ( $retorno [$chave] ); // muncod
			array_pop ( $retorno [$chave] ); // corrige_agencia
			array_pop ( $retorno [$chave] ); // nabid
			array_pop ( $retorno [$chave] ); // agbcod
		}
	} else {
		$retorno = array ();
	}
	
	return $retorno;
}
function recuperaSecretariaPorUfMuncod() {
	global $db;
	
	if ($_SESSION ['projovemurbano'] ['estuf']) {
		$stCampo = '';
		$stInner = '';
		// $stWhere = "AND fen.funid = 25 AND fen2.funid = 6 AND ende.estuf = '{$_SESSION['projovemurbano']['estuf']}'";
		$stWhere = "AND fen.funid = 6 AND ende.estuf = '{$_SESSION['projovemurbano']['estuf']}'";
	} else {
		$stCampo = "mun.mundescricao, mun.estuf,";
		$stInner = "INNER JOIN territorios.municipio mun on mun.muncod = ende.muncod";
		// $stWhere = "AND fen.funid = 15 AND fen2.funid = 7 AND mun.muncod = '{$_SESSION['projovemurbano']['muncod']}'";
		$stWhere = "AND fen.funid = 7 AND mun.muncod = '{$_SESSION['projovemurbano']['muncod']}'";
	}
	
	$sql = "
            SELECT  DISTINCT ent.entnome, 
                    CASE 
						WHEN ent.entnumcpfcnpj is not null
						THEN  ent.entnumcpfcnpj
						ELSE pjuprefcnpj
					END as entnumcpfcnpj,  
                    ende.endlog, 
                    ende.endcep, 
                    ende.endnum, 
                    ende.endbai,
                    {$stCampo}
                    ent.entnumcpfcnpj as cpfsecretario, 
                    ent.entnome as secretario
            FROM entidade.entidade ent
            
            LEFT JOIN entidade.funcaoentidade fen ON fen.entid = ent.entid AND fen.fuestatus = 'A'
            LEFT JOIN entidade.funentassoc fea ON fea.fueid = fen.fueid
                        
            --INNER JOIN entidade.entidade ent2 ON ent2.entid = fea.entid
            --INNER JOIN entidade.funcaoentidade fen2 ON fen2.entid = ent2.entid AND fen2.fuestatus = 'A'

            INNER JOIN entidade.endereco ende ON ende.entid = ent.entid
            INNER JOIN projovemurbano.projovemurbano pju ON pju.entid = ent.entid
            {$stInner}
            WHERE ent.entstatus = 'A' AND ent.entstatus = 'A' AND ende.endstatus = 'A' AND pju.ppuid = {$_SESSION['projovemurbano']['ppuid']}
            
            --AND trim(ent.entnumcpfcnpj) IS NOT NULL
            {$stWhere}";
//             ver($sql,d);
	$rsSecretaria = $db->pegaLinha ( $sql );
	return $rsSecretaria;
}
function testaMetaPrograma($mtpvalor, $tpmid) {
	global $db;
	
	$sql = "SELECT
				mtpvalor as valor,
				mtpid as id
			FROM
				projovemurbano.metasdoprograma
			WHERE
				tpmid = $tpmid
				AND ppuid = {$_SESSION['projovemurbano']['ppuid']}
				AND pjuid = {$_SESSION['projovemurbano']['pjuid']}";
	
	$valor = $db->pegaLinha ( $sql );
	
	if ($valor ['valor'] == '') {
		$sql = "INSERT INTO projovemurbano.metasdoprograma(tpmid, ppuid, pjuid, mtpvalor) 
				VALUES ($tpmid, {$_SESSION['projovemurbano']['ppuid']}, {$_SESSION['projovemurbano']['pjuid']}, $mtpvalor);";
	} elseif ($valor ['valor'] != $mtpvalor) {
		$sql = "UPDATE projovemurbano.metasdoprograma SET
					mtpvalor = $mtpvalor
				WHERE
					tpmid = $tpmid
					AND ppuid = {$_SESSION['projovemurbano']['ppuid']}
					AND pjuid = {$_SESSION['projovemurbano']['pjuid']};";
	}
	
	if ($sql != '') {
		$db->executar ( $sql );
		$db->commit ();
	}
}
function recuperaMetasPorUfMuncod($us) {
	global $db;
	
	if ($_SESSION ['projovemurbano'] ['muncod']) {
		$cmecodibge = " and cmecodibge = '" . $_SESSION ['projovemurbano'] ['muncod'] . "'";
	} else {
		$cmecodibge = " and cmecodibge = (select estcod from territorios.estado where estuf = '" . $_SESSION ['projovemurbano'] ['estuf'] . "')::numeric";
	}
	
	$sql = "
        select * from projovemurbano.cargameta c			
        left join projovemurbano.metasdoprograma m on c.cmeid = m.cmeid
        where 
        	c.ppuid = {$_SESSION['projovemurbano']['ppuid']}			
        {$cmecodibge}
    ";
	$rsMetas = $db->carregar ( $sql );
	
	$rs ['metatotal'] = $rsMetas [0] ['cmemeta'];
	
	foreach ( $rsMetas as $dados ) {
		if ($dados ['tpmid'] == 7) {
			testaMetaPrograma ( $dados ['mtpvalor'], $dados ['tpmid'] );
			$rs ['juventudeviva'] = $dados ['mtpvalor'];
		} else if ($dados ['tpmid'] == 10) {
			testaMetaPrograma ( $dados ['mtpvalor'], $dados ['tpmid'] );
			$rs ['prisionais'] = $dados ['mtpvalor'];
		} else if ($dados ['tpmid'] == 13) {
			testaMetaPrograma ( $dados ['mtpvalor'], $dados ['tpmid'] );
			$rs ['publicogeral'] = $dados ['mtpvalor'];
		}
	}
	
	/* RECUPERA META AJUSTADA */
	if ($_SESSION ['projovemurbano'] ['muncod']) {
		$filtroestmun = " and muncod = '" . $_SESSION ['projovemurbano'] ['muncod'] . "'";
	} else {
		$filtroestmun = " and estuf = '" . $_SESSION ['projovemurbano'] ['estuf'] . "'"; // " and estuf = '".$db->pegaUm("select estuf from territorios.estado where estuf = '{$_SESSION['projovemurbano']['estuf']}'")."'";
	}
	
	$sql = "
        select a.*
        from projovemurbano.projovemurbano p
	inner join projovemurbano.sugestaoampliacao a on a.pjuid = p.pjuid and a.ppuid = {$_SESSION['projovemurbano']['ppuid']}
        where p.ppuid = {$_SESSION['projovemurbano']['ppuid']} and p.pjuid = {$_SESSION['projovemurbano']['pjuid']}
	{$filtroestmun}
    ";
	
	$rsSugestaoMeta = $db->pegaLinha ( $sql );
	
	$rsSugestaoMeta ['suaid'] = $rsSugestaoMeta ['suaid'] ? $rsSugestaoMeta ['suaid'] : 'null';
	
	$sql = "
        select  t.tpmid,
		mtpvalor,
		tpmdescricao
        from projovemurbano.tipometadoprograma t
        left join projovemurbano.metasdoprograma m on m.tpmid = t.tpmid and suaid = {$rsSugestaoMeta['suaid']} and ppuid = {$_SESSION['projovemurbano']['ppuid']}
        where t.tpmid in (5,6,8,9,11,12,14,15)
    ";
	// ver($sql,d);
	$rsMetasAjustadas = $db->carregar ( $sql );
	
	if ($rsMetasAjustadas && $us ['ajustado'] == true) {
		if ($_SESSION ['projovemurbano'] ['estuf']) {
			foreach ( $rsMetasAjustadas as $meta ) {
				if (in_array ( $meta ['tpmid'], array (
						5 
				) )) {
					$rs ['metatotals'] = $meta ['mtpvalor'] ? $meta ['mtpvalor'] : $rs ['metatotal'];
				} elseif (in_array ( $meta ['tpmid'], array (
						6 
				) )) {
					$rs ['metatotala'] = $meta ['mtpvalor'] ? $meta ['mtpvalor'] : $rs ['metatotal'];
				} elseif (in_array ( $meta ['tpmid'], array (
						8 
				) )) { // 8,9
					$rs ['juventudevivas'] = $meta ['mtpvalor'];
				} elseif (in_array ( $meta ['tpmid'], array (
						9 
				) )) { // 8,9
					$rs ['juventudevivaa'] = $meta ['mtpvalor'];
				} elseif (in_array ( $meta ['tpmid'], array (
						11 
				) )) { // 11,12
					$rs ['prisionaiss'] = $meta ['mtpvalor'];
				} elseif (in_array ( $meta ['tpmid'], array (
						12 
				) )) { // 11,12
					$rs ['prisionaisa'] = $meta ['mtpvalor'];
				} elseif (in_array ( $meta ['tpmid'], array (
						14 
				) )) { // 14,15
					$rs ['publicogerals'] = $meta ['mtpvalor'];
				} elseif (in_array ( $meta ['tpmid'], array (
						15 
				) )) { // 14,15
					$rs ['publicogerala'] = $meta ['mtpvalor'];
				}
			}
			$rs ['metatotal'] = $rsSugestaoMeta ['suametaajustada'] ? $rsSugestaoMeta ['suametaajustada'] : $rsSugestaoMeta ['suametasugerida']; // $rs['metatotala'] ? $rs['metatotala'] : $rs['metatotals'];
			$rs ['juventudeviva'] = $rs ['juventudevivaa'] ? $rs ['juventudevivaa'] : $rs ['juventudevivas'];
			$rs ['prisionais'] = $rs ['prisionaisa'] ? $rs ['prisionaisa'] : $rs ['prisionaiss'];
			$rs ['publicogeral'] = $rs ['publicogerala'] ? $rs ['publicogerala'] : $rs ['publicogerals'];
		} elseif ($_SESSION ['projovemurbano'] ['muncod']) {
			foreach ( $rsMetasAjustadas as $meta ) {
				if (in_array ( $meta ['tpmid'], array (
						5 
				) )) {
					$rs ['metatotals'] = $meta ['mtpvalor'] ? $meta ['mtpvalor'] : $rs ['metatotal'];
				} elseif (in_array ( $meta ['tpmid'], array (
						6 
				) )) {
					$rs ['metatotala'] = $meta ['mtpvalor'] ? $meta ['mtpvalor'] : $rs ['metatotal'];
				} elseif (in_array ( $meta ['tpmid'], array (
						9 
				) )) {
					$rs ['juventudevivaa'] = $meta ['mtpvalor'];
				} elseif (in_array ( $meta ['tpmid'], array (
						15 
				) )) {
					$rs ['publicogerala'] = $meta ['mtpvalor'];
				}
			}
			$rs ['juventudeviva'] = $rs ['juventudevivaa'] ? $rs ['juventudevivaa'] : $rs ['juventudevivas'];
			$rs ['publicogeral'] = $rs ['publicogerala'] ? $rs ['publicogerala'] : $rs ['publicogerals'];
			$rs ['metatotal'] = $rs ['juventudeviva'] + $rs ['publicogeral']; // $rsSugestaoMeta['suametaajustada'] ? $rsSugestaoMeta['suametaajustada'] : $rsSugestaoMeta['suametasugerida']; //$rs['metatotala'] ? $rs['metatotala'] : $rs['metatotals'];
		} else {
			$rs ['metatotal'] = $rsSugestaoMeta ['suametaajustada'] ? $rsSugestaoMeta ['suametaajustada'] : $rsSugestaoMeta ['suametasugerida']; // $rsSugestaoMeta['suametasugerida'] ? $rsSugestaoMeta['suametasugerida'] : $rs['metatotal'];
		}
	}
	
	return $rs;
}
function podeMostrarTermosMetas($dados = array()) {
	global $db;
	
	if ($_SESSION ['projovemurbano'] ['muncod']) {
		
		$sql = "
            SELECT  suametasugerida,
                    suametaajustada
            FROM projovemurbano.projovemurbano p
            JOIN projovemurbano.sugestaoampliacao a on a.pjuid = p.pjuid
            WHERE p.ppuid = {$_SESSION['projovemurbano']['ppuid']} AND p.pjuid = {$_SESSION['projovemurbano']['pjuid']} AND muncod = '{$_SESSION['projovemurbano']['muncod']}'
        ";
		$rsSugerida = $db->pegaLinha ( $sql );
		
		if ($dados ['ajustado'] == true) {
			$stNomeCampo = 'suametaajustada';
		}
		
		if ($dados ['sugerido'] == true) {
			$stNomeCampo = 'suametasugerida';
		}
		
		if ($rsSugerida [$stNomeCampo] > 0) {
			return true;
		} else if (! $dados ['sugerido'] && ! $dados ['ajustado']) {
			return true;
		}
	} else {
		$stWhere = '';
		$stInner = '';
		if (! $dados ['sugerido'] && ! $dados ['ajustado']) {
			$stWhere .= "AND m.tpmid in (7, 10, 13) ";
			$stInner .= "
                JOIN territorios.estado est on est.estuf = p.estuf
                JOIN projovemurbano.cargameta cme on est.estcod::numeric = cme.cmecodibge
                JOIN projovemurbano.metasdoprograma m on cme.cmeid = m.cmeid
            ";
		} else {
			if ($dados ['ajustado'] == true) {
				$stWhere .= " AND m.tpmid in (9, 12, 15) --ajustados ";
			}
			
			if ($dados ['sugerido'] == true) {
				$stWhere .= " AND m.tpmid in (8, 11, 14) --sugeridas ";
			}
			
			$stInner .= "
                JOIN projovemurbano.sugestaoampliacao a on a.pjuid = p.pjuid
                JOIN projovemurbano.metasdoprograma m on a.suaid = m.suaid
            ";
		}
		
		if ($_SESSION ['projovemurbano'] ['estuf'] && $_SESSION ['projovemurbano'] ['ppuid'] == '1') {
			$sql = "
                SELECT	cmemeta
                FROM projovemurbano.cargameta cme
                JOIN territorios.estado est on cast(est.estcod as integer) = cast(cme.cmecodibge as integer)
                WHERE cme.ppuid = {$_SESSION['projovemurbano']['ppuid']} 
                AND est.estuf = '{$_SESSION['projovemurbano']['estuf']}' 
                AND cmemeta IS NOT NULL
            ";
		} else {
			$sql = "
                SELECT  mtpvalor
                FROM projovemurbano.projovemurbano p
                {$stInner}
                WHERE p.ppuid = {$_SESSION['projovemurbano']['ppuid']} AND m.ppuid = {$_SESSION['projovemurbano']['ppuid']} AND p.pjuid = {$_SESSION['projovemurbano']['pjuid']} AND p.estuf = '{$_SESSION['projovemurbano']['estuf']}'
                {$stWhere}
                AND mtpvalor IS NOT NULL
            ";
		}
		$rsSugeridas = $db->carregar ( $sql );
		
		if ($rsSugeridas [0] ['cmemeta'] != '' && $_SESSION ['projovemurbano'] ['estuf'] && $_SESSION ['projovemurbano'] ['ppuid'] == '1') {
			return true;
		} elseif (count ( $rsSugeridas ) == 3) {
			return true;
		}
		return false;
	}
}
function verificaMetaDestinada($dados) {
	global $db;
	
// 	extract( $dados );
	
	if($dados['cmeid'] != '' || $dados['suaid'] != '') {
		if ($dados['metaDestinada'] == "atendida") {
			$sql = "
                    Select tpmid, mtpvalor From projovemurbano.metasdoprograma Where ppuid = " . $_SESSION ['projovemurbano'] ['ppuid'] . " and cmeid = " . $dados['cmeid'] . "
                ";
			$meta = $db->carregar ( $sql );
			$i = 0;
			foreach ( $meta as $k => $a ) {
				if ($meta [$i] ['mtpvalor'] != 0 && $meta [$i] ['tpmid'] == 7) {
					$tipo_meta = 7;
				} elseif ($meta [$i] ['mtpvalor'] != 0 && $meta [$i] ['tpmid'] == 13) {
					$tipo_meta = 13;
				}
				$i = $i + 1;
			}
		}
		
		if ($dados['metaDestinada']  == "sugerida") {
			$sql = "
                    Select tpmid, mtpvalor From projovemurbano.metasdoprograma Where ppuid = " . $_SESSION ['projovemurbano'] ['ppuid'] . " and suaid = " . $dados['suaid'] . "
                ";
			$meta = $db->carregar ( $sql );
			$i = 0;
			if (is_array ( $meta )) {
				foreach ( $meta as $k => $a ) { 
					if ($meta [$i] ['mtpvalor'] != 0 && $meta [$i] ['tpmid'] == 8) {
						$tipo_meta = 8;
					} elseif ($meta [$i] ['mtpvalor'] != 0 && $meta [$i] ['tpmid'] == 14) {
						$tipo_meta = 14;
					}
					$i = $i + 1;
				}
			}
		}
		
		if ($dados['metaDestinada'] == "ajustada") {
			$sql = "
                    Select tpmid, mtpvalor From projovemurbano.metasdoprograma Where ppuid = " . $_SESSION ['projovemurbano'] ['ppuid'] . " and suaid = " . $dados['suaid'] . "
                ";
			$meta = $db->carregar ( $sql );
			$i = 0;
			foreach ( $meta as $k => $a ) {
				if ($meta [$i] ['mtpvalor'] != 0 && $meta [$i] ['tpmid'] == 9) {
					$tipo_meta = 9;
				} elseif ($meta [$i] ['mtpvalor'] != 0 && $meta [$i] ['tpmid'] == 15) {
					$tipo_meta = 15;
				}
				$i = $i + 1;
			}
		}
		echo $tipo_meta;
	}
}
function carregarSugestaoAmpliacao() {
	global $db;
	
	$sugestaoampliacao = $db->pegaLinha ( "SELECT suaverdade, suametaajustada FROM projovemurbano.sugestaoampliacao WHERE pjuid='" . $_SESSION ['projovemurbano'] ['pjuid'] . "'" );
	
	return $sugestaoampliacao;
}
function carregarMeta($sugestaoampliacao) {
	global $db;
	
	$sql = "SELECT adesaotermoajustado FROM projovemurbano.projovemurbano WHERE pjuid = {$_SESSION['projovemurbano']['pjuid']}";
    	
	$ajustado = $db->pegaUm($sql);

	if( $ajustado == 't' ){
		$not = ",7,10,13";
	}else{
		$not = ",9,12,15";
	}
	$sql = "SELECT
			    	mtpvalor as valor,
			    	mtp.tpmid as tipo
		    	FROM
		    		projovemurbano.metasdoprograma mtp
		    	INNER JOIN projovemurbano.tipometadoprograma tpr ON tpr.tpmid = mtp.tpmid
		    	WHERE
			    	pjuid = {$_SESSION['projovemurbano']['pjuid']}
			    	AND tprid = {$_SESSION['projovemurbano']['tprid']}
			    	AND tpr.tpmid NOT IN  (2,5,8,11,14$not)
		    	ORDER BY
		    		tipo DESC ";
	
	$meta = $db->pegaUm ( $sql );
	return $meta;
}
function bloqueioadesao2014() {
	global $db;
	
	$dataAtual = mktime ( date ( 'H' ), date ( 'i' ), date ( 's' ), date ( 'm' ), date ( 'd' ), date ( 'Y' ) );
	$dataBloqueio = mktime ( 23, 59, 00, 08, 6, 2014 );
	$bloqueioHorario = ( bool ) ($dataAtual > $dataBloqueio);
	
	return $bloqueioHorario;
}
function enderecoEscola($dados) {
	global $db;
	$sql = <<<DML
SELECT no_entidade, num_cep, desc_endereco,
       num_endereco, desc_endereco_complemento,
       desc_endereco_bairro, '(' || num_ddd || ') '|| num_telefone AS num_telefone, mun.mundescricao
  FROM educacenso_2014.tab_entidade tent
    INNER JOIN territorios.municipio mun
      ON (muncod::int = fk_cod_municipio)
  WHERE tent.pk_cod_entidade = {$dados['entid']}
DML;
	$dadosEntidade = $db->pegaLinha ( $sql );
	?>
<html>
<head>
<link rel="stylesheet" type="text/css" href="../includes/Estilo.css" />
<link rel="stylesheet" type="text/css" href="../includes/listagem.css" />
</head>
<body>
	<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"
		align="center">
		<tr>
			<td class="SubTituloDireita" width="30%">Nome da escola</td>
			<td><?php echo $dadosEntidade['no_entidade']; ?></td>
		</tr>
		<tr>
			<td class="SubTituloDireita" width="30%">Código INEP</td>
			<td><?php echo $dados['entid']; ?></td>
		</tr>
		<tr>
			<td class="SubTituloDireita" width="30%">Telefone</td>
			<td><?php echo $dadosEntidade['num_telefone']; ?></td>
		</tr>
		<tr>
			<td class="SubTituloDireita" width="30%">Endereço</td>
			<td><?php echo $dadosEntidade['desc_endereco']; ?></td>
		</tr>
		<tr>
			<td class="SubTituloDireita" width="30%">Número</td>
			<td><?php echo $dadosEntidade['num_endereco']; ?></td>
		</tr>
		<tr>
			<td class="SubTituloDireita" width="30%">Complemento</td>
			<td><?php echo $dadosEntidade['desc_endereco_complemento']; ?></td>
		</tr>
		<tr>
			<td class="SubTituloDireita" width="30%">Bairro</td>
			<td><?php echo $dadosEntidade['desc_endereco_bairro']; ?></td>
		</tr>
		<tr>
			<td class="SubTituloDireita" width="30%">CEP</td>
			<td><?php echo $dadosEntidade['num_cep']; ?></td>
		</tr>
		<tr>
			<td class="SubTituloDireita" width="30%">Município</td>
			<td><?php echo $dadosEntidade['mundescricao']; ?></td>
		</tr>
		<tr>
			<td colspan="2" align="center"><button onclick="window.close();">Fechar</button></td>
		</tr>
	</table>
</body>
</html>

<?
}
function subistituiCaracteres($string) {
	$palavra = strtr ( $string, "ŠŒŽšœžŸ¥µÀÁÂÃÄÅÆÇÈÉÊËÌÍÎÏÐÑÒÓÔÕÖØÙÚÛÜÝßàáâãäåæçèéêëìíîïðñòóôõöøùúûüýÿ", "SOZsozYYuAAAAAAACEEEEIIIIDNOOOOOOUUUUYsaaaaaaaceeeeiiiionoooooouuuuyy" );
	$palavranova = str_replace ( "_", " ", $palavra );
	$pattern = '|[^a-zA-Z0-9\-]|';
	$palavranova = preg_replace ( $pattern, ' ', $palavranova );
	$string = str_replace ( ' ', ' ', $palavranova );
	$string = str_replace ( '---', '', $string );
	$string = str_replace ( '--', '', $string );
	$string = str_replace ( '-', '', $string );
	$string = str_replace ( 'http   simec mec gov br imagens obrig gif', '', $string );
	return $string;
}
/*
 * FUNÇÕES GERAIS TRANFERÊNCIA
 */
// function atualizaHistoricoStatusTranferencia($htrid, $traid) {
// 	global $db;
	
// 	$sql = "UPDATE projovemurbano.transferencia SET
//                 htrid_ultimo = $htrid
//             WHERE
//                 traid = $traid";
// 	$db->executar ( $sql );
// }
// function atualizaHistoricoAluno($turid, $caeid, $turid_origem, $ppuid) {
// 	global $db;
	
// 	$sql = "SELECT max(notaciclo1) as nota1, max(notaciclo2) as nota2, max(notaciclo3) as nota3 
//             FROM projovemurbano.notasporciclo 
//             WHERE 
//                 turid = $turid_origem
//     		AND caeid = $caeid ";
// 	// ver($sql,d);
// 	$dados = $db->pegaLinha ( $sql );
	
// 	extract ( $dados );
	
// 	if ($nota1 == '') {
// 		$notaciclo1 = "NULL";
// 	} else {
// 		$notaciclo1 = $nota1;
// 	}
	
// 	if ($nota2 == '') {
// 		$notaciclo2 = "NULL";
// 	} else {
// 		$notaciclo2 = $nota2;
// 	}
	
// 	if ($nota3 == '') {
// 		$notaciclo3 = "NULL";
// 	} else {
// 		$notaciclo3 = $nota3;
// 	}
// 	$sqlverificaturma = "SELECT 
// 							TRUE
// 						 FROM
// 							projovemurbano.notasporciclo
// 						 WHERE 
// 						 	turid = $turid_origem
//     					 AND caeid = $caeid
//     					 AND npc_status = 'I'";
// 	$verificaturma = $db->pegaUm ( $sqlverificaturma );
	
// 	if ($sqlverificaturma) {
// 		$sql = "UPDATE projovemurbano.notasporciclo  SET npc_status = 'A'
// 				WHERE
// 					turid = $turid_origem
// 				AND caeid = $caeid
// 				AND npc_status = 'I'";
		
// 		$db->executar ( $sql );
// 	} else {
// 		$sql = "INSERT INTO projovemurbano.notasporciclo (caeid, turid, notaciclo1, notaciclo2, notaciclo3, ppuid)
// 	                 VALUES ($caeid, $turid, $notaciclo1, $notaciclo2, $notaciclo3, $ppuid)
// 	                 ";
// 		// ver($sql,d);
// 		$db->executar ( $sql );
		
// 		$sql = "UPDATE projovemurbano.notasporciclo  SET npc_status = 'I'
// 	            WHERE 
// 	                turid = $turid_origem
// 	            AND caeid = $caeid
// 	    		AND npc_status = 'A'";
		
// 		$db->executar ( $sql );
// 	}
	
// 	$sql = "UPDATE projovemurbano.frequenciaestudante SET
//                  frqstatus = 'I'
//             WHERE
//                  caeid = $caeid
//                  AND frqid IN (SELECT DISTINCT
//                              frqid
//                          FROM
//                              projovemurbano.frequenciaestudante frq
//                          INNER JOIN projovemurbano.diariofrequencia dif ON dif.difid = frq.difid
//                          INNER JOIN projovemurbano.diario dia ON dia.diaid = dif.diaid
//                          WHERE
//                              caeid = $caeid
//                              AND dia.perid IN (SELECT perid FROM projovemurbano.diario WHERE turid = $turid_origem))";
// 	// ver($sql,d);
// 	$db->executar ( $sql );
// }
// function importaTrabalhoFrequencia($caeid) {
// 	global $db;
	
// 	$sql = "SELECT DISTINCT true 
// 			FROM projovemurbano.transferencia 
// 	 		WHERE 
// 	 			 cad_caeid = $caeid";
	
// 	$temtrans = $db->pegaUm ( $sql );
	
// 	if ($temtrans == 't') {
		
// 		$sql1 = "SELECT max(htrid) 
// 			FROM projovemurbano.transferencia tra 
// 			INNER JOIN projovemurbano.historico_transferencia htr ON tra.traid = htr.traid
// 			WHERE cad_caeid = $caeid";
		
// 		$htrid = $db->pegaUm ( $sql1 );
		
// 		$htridx = "AND htr.htrid = $htrid";
// 	}
// 	$sqldadostrans = "SELECT 
//     					turid_origem, 
//     					turid_destino, 
//     					ppuid_origem, 
//     					ppuid_destino 
// 					FROM projovemurbano.transferencia tra 
// 					INNER JOIN projovemurbano.historico_transferencia htr ON tra.traid = htr.traid 
//     				WHERE 
//     					cad_caeid = $caeid $htridx";
// 	$dadostrasns = $db->pegaLinha ( $sqldadostrans );
	
// 	$sqlperiodoorigem = "SELECT 
//     						max(perperiodo) 
//     					FROM projovemurbano.periodocurso 
//     					WHERE 
//     						ppuid = {$dadostrasns['ppuid_origem']}
// 						AND perid = (SELECT 
//     									max(perid) 
//     								FROM projovemurbano.diario 
//     								WHERE turid = {$dadostrasns['turid_origem']})";
// 	$periodoorigem = $db->pegaUm ( $sqlperiodoorigem );
	
// 	$sqlperiododestino = "SELECT
//     						 max(perperiodo) 
// 						FROM projovemurbano.periodocurso 
// 						WHERE 
// 							ppuid = {$dadostrasns['ppuid_destino']} 
// 						AND perid = (SELECT 
// 										max(perid) 
// 									FROM projovemurbano.diario 
// 									WHERE turid = {$dadostrasns['turid_destino']})";
// 	// ver($sqlperiododestino,d);
// 	$periododestino = $db->pegaUm ( $sqlperiododestino );
// 	// ver($dadostrasns,d);
	
// 	$sqltestaturma = "SELECT
// 							difid
// 					  FROM
// 							projovemurbano.frequenciaestudante frq
// 					  INNER JOIN projovemurbano.diariofrequencia dif ON dif.difid = frq.difid
// 					  INNER JOIN projovemurbano.diario dia ON dia.diaid = dif.diaid
// 				  	  WHERE
// 					  	dia.turid = {$dadostrasns['turid_destino']}
//     				  AND frq.caeid = $caeid
//     				  AND frq.frqstatus = 'I'";
// 	$testaturma = $db->pegarColunas ( $sqltestaturma );
// 	// ver($testaturma,d);
// 	if (is_array ( $testaturma ) && ! empty ( $testaturma )) {
// 		$sql = "UPDATE projovemurbano.frequenciaestudante  SET frqstatus = 'A'
// 				WHERE
// 					caeid = $caeid
// 				AND frqstatus = 'I'
//     			AND difid in ($testaturma)";
		
// 		$db->executar ( $sql );
// 	} else {
// 		$sql12 = "INSERT INTO projovemurbano.frequenciaestudante(caeid, difid, frqqtdpresenca, frqtrabalho)
// 					SELECT 
// 					caeid,
// 					(
// 					SELECT DISTINCT
// 						difid
// 					FROM 
// 						projovemurbano.diariofrequencia dif2 
// 					INNER JOIN projovemurbano.diario dia2 ON dia2.diaid = dif2.diaid
// 					INNER JOIN projovemurbano.periodocurso per2 ON per2.perid = dia2.perid
// 					WHERE
// 						dia2.diaid in(        
// 										SELECT 
// 											diaid
// 										FROM 
// 											projovemurbano.diario
// 										WHERE 
// 											turid = {$dadostrasns['turid_destino']}
// 										AND perid BETWEEN (SELECT perid FROM projovemurbano.periodocurso WHERE ppuid = {$dadostrasns['ppuid_destino']} AND perperiodo = 1)
// 										AND(SELECT perid FROM projovemurbano.periodocurso WHERE ppuid = {$dadostrasns['ppuid_destino']} AND perperiodo = ($periododestino - 1))
// 									)
// 					AND dif2.grdid = dif.grdid
// 					AND per2.perperiodo = per.perperiodo
// 					) as difid,
// 					frqqtdpresenca,
// 					frqtrabalho
// 					FROM 
// 						projovemurbano.frequenciaestudante fre
// 					INNER JOIN projovemurbano.diariofrequencia dif ON dif.difid = fre.difid
// 					INNER JOIN projovemurbano.diario dia ON dia.diaid = dif.diaid
// 					INNER JOIN projovemurbano.periodocurso per ON per.perid = dia.perid
// 					WHERE
// 						caeid = $caeid
// 					AND turid = {$dadostrasns['turid_origem']}
// 					AND frqstatus = 'I'
// 					AND dia.perid BETWEEN (SELECT perid FROM projovemurbano.periodocurso WHERE ppuid = {$dadostrasns['ppuid_origem']} AND perperiodo = 1)
// 					AND(SELECT perid FROM projovemurbano.periodocurso WHERE ppuid =  {$dadostrasns['ppuid_origem']} AND perperiodo = ($periododestino - 1))
// 					ORDER BY
// 					       3
// 	            ";
// 		// ver($sql12,d);
// 		$db->executar ( $sql12 );
// 	}
// }
function inserirResponsavelMaterial() {
	global $db;
	// ver($_POST,d);
	extract ( $_POST );
	
	$eemcep = str_replace ( array (
			"-" 
	), array (
			"" 
	), $eemcep );
	
	$inserir = "INSERT INTO projovemurbano.enderecoentregadematerial(
            	pjuid, eemcpfresponsavel, eemnomeresponsavel, eemcep, 
            	eemlogradouro, eemnumero, eemcomplemento, eembairro, eemuf, eemmuncod)
    		VALUES ({$_SESSION['projovemurbano']['pjuid']}, '{$eemcpfresponsavel}', '{$eemnomeresponsavel}', '{$eemcep}', 
            		'{$eemlogradouro}', {$eemnumero}, '{$eemcomplemento}', '{$eembairro}', '{$eemuf}', {$eemmuncod});
						";
	$db->executar ( $inserir );
	$db->commit ();
	
	echo "<script>
           alert('Dados Salvos com sucesso.');
            window.location.href = window.location.href;
          </script>";
}
function atualizarResponsavelMaterial() {
	global $db;
	// ver($_POST,d);
	extract ( $_POST );
	
	$eemcep = str_replace ( array (
			"-" 
	), array (
			"" 
	), $eemcep );
	
	$atualizar = "UPDATE projovemurbano.enderecoentregadematerial
	   			SET  
					eemcpfresponsavel='{$eemcpfresponsavel}', 
					eemnomeresponsavel='{$eemnomeresponsavel}', 
	       			eemcep='{$eemcep}', 
					eemlogradouro='{$eemlogradouro}', 
					eemnumero={$eemnumero}, 
					eemcomplemento='{$eemcomplemento}', 
					eembairro='{$eembairro}', 
	       			eemuf='{$eemuf}', 
					eemmuncod={$eemmuncod}
	 			WHERE 
					eemid = {$eemid}
				";
	// ver($atualizar,d);
	$db->executar ( $atualizar );
	$db->commit ();
	
	echo "<script>
			alert('Dados atualizados com sucesso.');
	        window.location.href = window.location.href;
	      </script>";
}
function mostraDetalheTransferencia() {
	global $db;
	extract ( $_POST );
	
	$coordnome = "
                    (SELECT DISTINCT 
                        cornome
                        FROM projovemurbano.coordenadorresponsavel cor
                        LEFT JOIN projovemurbano.polomunicipio pmu  
                            INNER JOIN projovemurbano.polo p ON p.pmuid = pmu.pmuid
                        ON pmu.pjuid = cor.pjuid        
                        LEFT JOIN projovemurbano.projovemurbano pju on pju.pjuid = cor.pjuid
                            LEFT JOIN projovemurbano.municipio mun  
                            INNER JOIN projovemurbano.nucleo nuc ON nuc.munid = mun.munid
                            ON mun.muncod = pju.muncod
                        WHERE
                        CASE WHEN tra.polid_destino IS NOT NULL
                            THEN p.polid = tra.polid_destino AND cor.ppuid = tra.ppuid_destino
                            ELSE nuc.nucid = tra.nucid_destino AND cor.ppuid = tra.ppuid_destino END )";
	
	$sqlano = "SELECT
                    ppuano as ano2
                FROM
                    projovemurbano.programaprojovemurbano ppu
                WHERE ";
	
	$sql = "SELECT 
                shtdescricao as status,            
                cae.caecpf as cpf,
                cae.caenome as nome,
                to_char( tra.tradataingressoorigem, 'DD/MM/YY HH:MM:SS' ) as dataingressoorigem,
                tra.*,
                (SELECT DISTINCT 
                    turdesc AS descricao
                FROM projovemurbano.turma 
                WHERE  turid = tra.turid_origem
                --AND    turstatus = 'A'
				) as turid_origem,
                (SELECT DISTINCT 
                    turdesc AS descricao
                FROM projovemurbano.turma 
                WHERE  turid = tra.turid_destino
                --AND    turstatus = 'A'
				) as turid_destino,
                ( coalesce(estO.estuf,munO.estuf||' - '||munO.mundescricao) ) as localorigem,
                ( coalesce(est.estuf,mun.estuf||' - '||mun.mundescricao) ) as localdestino,
                ( $sqlano  ppu.ppuid = tra.ppuid_origem) as AnoOrigem,
                ( $sqlano  ppu.ppuid = tra.ppuid_destino) as AnoDestino,
                CASE WHEN arqt.arqid = null
                    THEN 'Não Inserido'
                    ELSE arqt.arqid::text
                END as arquivo,
                usu.usunome as resporigem,
                $coordnome  as respdestino
            FROM 
                projovemurbano.transferencia tra 
            INNER JOIN seguranca.usuario                            usu  ON usu.usucpf  = tra.usucpfresponsavelorigem
            INNER JOIN projovemurbano.cadastroestudante             cae  ON cae.caeid   = tra.cad_caeid 
            LEFT  JOIN projovemurbano.arquivo_tranferencia          arqt ON arqt.traid  = tra.traid
            INNER JOIN projovemurbano.historico_transferencia       hst  ON hst.htrid   = tra.htrid_ultimo
            INNER JOIN projovemurbano.statushistorictransferencia   sht  ON sht.shtid   = hst.shtid_status

            LEFT  JOIN projovemurbano.polo                          polO
                INNER JOIN projovemurbano.polomunicipio             pmuO  ON pmuO.pmuid = polO.pmuid                          
            ON polO.polid  = tra.polid_origem
            LEFT  JOIN projovemurbano.nucleo                        nucO  
                INNER JOIN projovemurbano.municipio                 muniO  ON muniO.munid = nucO.munid
                INNER JOIN projovemurbano.polomunicipio             pmuO2 ON pmuO2.pmuid = muniO.pmuid  
            ON nucO.nucid  = tra.nucid_origem 
            LEFT  JOIN projovemurbano.projovemurbano                pjuO ON pjuO.pjuid = pmuO.pjuid OR pjuO.pjuid = pmuO2.pjuid
            LEFT  JOIN territorios.municipio                        munO ON munO.muncod = pjuO.muncod
            LEFT  JOIN territorios.estado                           estO ON estO.estuf = pjuO.estuf

            LEFT  JOIN projovemurbano.polo pol
                INNER JOIN projovemurbano.polomunicipio             pmu  ON pmu.pmuid = pol.pmuid                          
            ON pol.polid  = tra.polid_destino
            LEFT  JOIN projovemurbano.nucleo                        nuc  
                INNER JOIN projovemurbano.municipio                 muni  ON muni.munid = nuc.munid
                INNER JOIN projovemurbano.polomunicipio             pmu2 ON pmu2.pmuid = muni.pmuid  
            ON nuc.nucid  = tra.nucid_destino
            LEFT  JOIN projovemurbano.projovemurbano                pju ON pju.pjuid = pmu.pjuid OR pju.pjuid = pmu2.pjuid
            LEFT  JOIN territorios.municipio                        mun ON mun.muncod = pju.muncod
            LEFT  JOIN territorios.estado                           est ON est.estuf = pju.estuf
            WHERE   
                tra.traid = $traid
            ORDER BY
                nome";
	// ver($sql,d);
	$dados = $db->pegaLinha ( $sql );
	
	extract ( $dados );
	?>
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"
	align="Center">
	<tr>
		<td class="SubTituloDireita" width="20%">Aluno:</td>
		<td width="30%"><?=$nome?></td>
		<td class="SubTituloDireita" width="20%">Coordenação Destino:</td>
		<td width="30%"><?=$localdestino?></td>
	</tr>
	<tr>
		<td class="SubTituloDireita" width="20%">CPF:</td>
		<td width="30%"><?=$cpf?></td>
		<td class="SubTituloDireita" width="20%">Pólo Destino:</td>
		<td width="30%"> Polo: <?=($polid_destino!=''?$polid_destino:'Não possui')?></td>
	</tr>
	<tr>
		<td class="SubTituloDireita" width="20%">Coordenação de Origem:</td>
		<td width="30%"><?=$localorigem?></td>
		<td class="SubTituloDireita" width="20%">Núcleo Destino:</td>
		<td width="30%"> Núcleo: <?=$nucid_destino?></td>
	</tr>
	<tr>
		<td class="SubTituloDireita" width="20%">Pólo Origem:</td>
		<td width="30%"> Polo: <?=($polid_origem!=''?$polid_origem:'Não possui')?></td>
		<td class="SubTituloDireita" width="20%">Turma Destino:</td>
		<td width="30%"><?=($turid_destino!=''?$turid_destino:'Não informado')?></td>
	</tr>
	<tr>
		<td class="SubTituloDireita" width="20%">Núcleo Origem:</td>
		<td width="30%"> Núcleo: <?=$nucid_origem?></td>
		<td class="SubTituloDireita" width="20%">Ano Destino:</td>
		<td width="30%"><?=$anodestino?></td>
	</tr>
	<tr>
		<td class="SubTituloDireita" width="20%">Turma Origem:</td>
		<td width="30%"> <?=$turid_origem?></td>
		<td class="SubTituloDireita" width="20%">Status Atual:</td>
		<td width="30%"><?=$status?></td>
	</tr>
	<tr>
		<td class="SubTituloDireita" width="20%">Ano Origem</td>
		<td width="30%"><?=$anoorigem?></td>
		<td class="SubTituloDireita" width="20%">Coordenador Responsável
			Destino:</td>
		<td width="30%"><?=$respdestino?></td>
	</tr>
	<tr>
		<td class="SubTituloDireita" width="20%">Data que Ingressou na Origem:</td>
		<td width="30%"><?=$dataingressoorigem?></td>
	</tr>
	<tr>
		<td class="SubTituloDireita" width="20%">Coordenador Responsável
			Origem:</td>
		<td width="30%"><?=$resporigem?></td>
	</tr>
</table>
<?
	$sql = "SELECT 
                *
            FROM
            (
            SELECT 
                ( SELECT
                    shtdescricao
                FROM
                    projovemurbano.statushistorictransferencia
                WHERE
                    shtid = (SELECT shtid_status 
                        FROM projovemurbano.historico_transferencia
                        WHERE htrid = (SELECT max(htrid) FROM projovemurbano.historico_transferencia WHERE traid = htr.traid AND htrid < htr.htrid )
                        ) ) as anterior,
                shtdescricao as atual,
                usu.usunome,
                to_char( htrdatahoraacao, 'DD/MM/YYYY HH:MM:SS' ) as data,
                justificativa
            FROM 
                projovemurbano.historico_transferencia htr
            INNER JOIN projovemurbano.statushistorictransferencia   sht ON sht.shtid  = htr.shtid_status
            LEFT  JOIN seguranca.usuario                usu ON usu.usucpf = usucpf_fezacao
            WHERE 
                traid = $traid
            ) asfoo
            WHERE
                anterior IS NOT NULL";
	// ver($sql,d);
	$cabecalho = array (
			"Onde estava?",
			"O que aconteceu?",
			"Quem fez?",
			"Quando fez?",
			"Justificativa" 
	);
	$db->monta_lista ( $sql, $cabecalho, 100, 5, 'N', 'center', '', 'form_transferencias' );
}
function trocaRangePeriodo() {
	global $db;
	
	$perfis = pegaPerfilGeral();
	
	monta_titulo('Selecione a data de início das aulas da sua localidade para prosseguir', '');
	?>
	<form id="form" enctype="multipart/form-data" name="form" method="POST" >
		<input type="hidden" name="requisicao" id="requisicao" value="salvarRange"/>
		<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="Center">
			<tr>
				<td class="SubTituloDireita" width="20%">Data de Inicio:</td>
				<td width="30%">
					<?
						$sql = "SELECT 
									rap.rapid as codigo,
									per.perdesc||'  '|| datainicio ||' - '||datafim as descricao
								FROM
									projovemurbano.periodocurso per
								INNER JOIN projovemurbano.rangeperiodo rap ON rap.perid = per.perid
								WHERE
									per.perperiodo = 1
								ORDER BY									
									1";
						$dadosperiodo = $db->carregar ( $sql );
						$db->monta_combo ( 'rapid', $sql, 'S', 'Selecione', '', '', '', '', 'N', 'rapid', '' );
					?>
				</td>
			</tr>
			<tr>
				<td align="Center" colspan = "2" >
					<input type="button" class="inserirRange" value="OK">
				</td>
			</tr>
		</table>
	</form>
	<script language="javascript" type="text/javascript">
		jQuery(document).ready(function() {
			jQuery('.inserirRange').live('click',function(){
				if(jQuery('#rapid').val()==''){
					alert('Escolha uma opção para a data de início.');
					return false;
				}
		        jQuery('#form').submit();
		    });
		});
	</script>
<?
} 

function salvarRange(){
	global $db;
	$sql = "SELECT
				true
			FROM
				projovemurbano.projovemurbano pju
			INNER JOIN projovemurbano.coordenadorresponsavel coor ON coor.pjuid = pju.pjuid AND coor.ppuid = pju.ppuid
			WHERE
				pju.ppuid = {$_SESSION['projovemurbano']['ppuid']}
			AND corcpf = '{$_SESSION['usucpf']}'";
	$testacoord = $db->pegaUm($sql);
	
	if($testacoord!= true){
		echo "<script>
				alert('Você não é o coordenador indicado na aba de Coordenador Responsável!');
				window.location.href = window.location.href
			</script>";
		die;
   	}
   	
	$sqlrange = "UPDATE projovemurbano.projovemurbano
			SET
				rapid = {$_REQUEST['rapid']}
			WHERE
				pjuid = (SELECT DISTINCT
							pjuid
						FROM
							projovemurbano.coordenadorresponsavel
						WHERE
							corcpf = '{$_SESSION['usucpf']}'
						AND ppuid = {$_SESSION['projovemurbano']['ppuid']})";
// 	ver($sqlrange,d);
	$db->executar($sqlrange);
	$db->commit();
	echo "<script>
			alert('Dados salvo com sucesso!');
    			window.location.href = window.location.href
		  </script>";
}   
    
