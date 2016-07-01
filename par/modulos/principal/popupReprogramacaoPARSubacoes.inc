<?php
ini_set("memory_limit", "2048M");
set_time_limit(30000);

if( $_REQUEST['requisicao'] == 'carregaDadosItens' ){
    carregaDadoItens();
    exit();
}

if( $_REQUEST['requisicao'] == 'aceita' ){
    reprogramaSubacao(1);
    exit();
}

if( $_REQUEST['requisicao'] == 'aceitaTodas' ){
    reprogramaSubacao(2);
    exit();
}

if( $_REQUEST['requisicao'] == 'cancela' ){
    cancelaReprogramaSubacao(1);
    exit();
}

if( $_REQUEST['requisicao'] == 'cancelaTodas' ){
    cancelaReprogramaSubacao(2);
    exit();
}

if( $_POST['cancelaReformulacao'] == 'cancela' ){
    global $db;

    $dado = deletaReformulacao( $_POST['sbaidCancelamento'], $_POST['anoCancelamento'] );
    echo "<script>
            alert('Reformulação cancelada com sucesso!');
            location.href='par.php?modulo=principal/popupReprogramacaoPARSubacoes&acao=A&dopid=".$_POST['dopid']."&tipo=2';
        </script>";
    exit;
}

if( !$_REQUEST['dopid'] || $_REQUEST['dopid'] == 'undefined' ){
    echo "<script>
            alert('Documento inválido.');
            window.close();
        </script>";
    die();
}

if( $_POST['sbdid'] ){
	

	ini_set("memory_limit","1000M");
	set_time_limit(0);

	$dopidOriginal = $_REQUEST['dopid'];

	if( is_array( $_POST['sbdid'] ) ){
		
		$subacoes = array();
		$numeroprotocolo = Date('YdHis').$dopidOriginal;
		
		$insereReprogramacao = "INSERT INTO par.reprogramacao (treid, repdtinicio, repstatus, dopidoriginal) VALUES ( 2, NOW(), 'A', ".$dopidOriginal.") RETURNING repid";
		$repid = $db->pegaUm($insereReprogramacao);
		
		$justificativa = $_POST['dprjustificativa'];
		$dopIdLog = $dopidOriginal;
		$cpfLog = $_SESSION['usucpf'];
		if( $dopIdLog )
		{
			$prpidLog = $db->pegaUm(" SELECT prpid from par.documentopar where dopid = {$dopIdLog}");
		}
		
		if( $numeroprotocolo && $cpfLog && $justificativa && $dopIdLog && $repid && $prpidLog)
		{
			$sqlInsert = "
				INSERT into par.historicosolicitacaoreprogramacao
					( hsrprotocolo, hsrdata, hsrtipo, hsrcpf, hsrjustificativa, dopid, repid, prpid )
				VALUES
					( '{$numeroprotocolo}', 'now()', 'S', '{$cpfLog}', '{$justificativa}', {$dopIdLog}, {$repid}, {$prpidLog} )		
				RETURNING hsrid
			";
			
			$hsrid = $db->pegaUm($sqlInsert);
		}
		
		foreach( $_POST['sbdid'] as $sbdid ){

			$dadosSub = $db->pegaLinha("SELECT par.retornacodigosubacao(s.sbaid) as codigo, s.sbaid as sbaid,  s.sbadsc as nome, sd.sbdano as ano FROM par.subacao s INNER JOIN par.subacaodetalhe sd ON sd.sbaid = s.sbaid WHERE sbdid = ".$sbdid);
			
			$subacoes[] = array('codigo' => $dadosSub['codigo'], 'nome' => $dadosSub['nome'], 'ano' => $dadosSub['ano']);
			$sbaidLog 	= $dadosSub['sbaid'];
			$sbaanoLog 	= $dadosSub['ano'];
			
			$sqlInsr .= "INSERT INTO par.documentoparreprogramacaosubacao ( dopid, usucpf, sbdid, dpsstatus, dpsdata, dpsjustificativa, repid )
						VALUES ( ".$dopidOriginal.", '".$_SESSION['usucpf']."', ".$sbdid.", 'P', 'NOW()', '{$_POST['dprjustificativa']}', ".$repid." ); ";
			
			if($hsrid && $sbaidLog && $sbaanoLog)
			{
				$sqlInsertLogSubacao = "INSERT into par.historicosolicitacaoreprogramacaosubacao
					( hsrid, sbaid, sbaano)
					VALUES
					( $hsrid , $sbaidLog, $sbaanoLog );";
				
				$db->executar($sqlInsertLogSubacao);
			}
		}

		if( $sqlInsr ){
			$db->executar($sqlInsr);
			$db->commit();
		}
	
		if($hsrid)
		{
			enviaEmailProtocoloReprogramacao($hsrid, 'S');
		}
		
		enviaEmailReprogramacao('subacao', $dopidOriginal, '', $subacoes);

		echo "<script>
				alert('Protocolo {$numeroprotocolo} Subação(ões) encaminhada(s) com sucesso! Para imprimir o protocolo dessa solicitção acesse na coluna \"Diálogo\" na aba \"Execução e Acompanhamento\".');
				location.href='par.php?modulo=principal/popupReprogramacaoPARSubacoes&acao=A&dopid=".$dopidOriginal."&tipo=2';
	 		</script>";
	}
}

function cancelaReprogramaSubacao($tipo){
	global $db;

	$dopid = $_REQUEST['dopid'];

	if( $tipo == 1 ){ // Apenas uma subação
		$sbdids = Array($_POST['sbdidAceite']);
	} elseif( $tipo == 2 ){
		$sbdids = $db->carregarColuna("SELECT DISTINCT sbdid FROM par.documentoparreprogramacaosubacao dpv WHERE dpv.dopid = ".$dopid." AND dpv.dpsstatus = 'P'");
	}
	$sbdids = is_array($sbdids) ? $sbdids : Array();
	
	$parecer = $_REQUEST['dprjustificativa'];
	if( $sbdids[0] != '' ){
		$repid = $db->pegaUm("SELECT repid FROM par.documentoparreprogramacaosubacao WHERE sbdid IN (".implode(", ", $sbdids).") AND dpsstatus = 'P'");

		$db->executar("UPDATE par.documentoparreprogramacaosubacao SET dpsstatus = 'I', usucpfvalidacao = '".$_SESSION['usucpf']."', dpsvalidacao = 'f', dpsparecer ='".$parecer."', dpscpfparecer = '".$_SESSION['usucpf']."', dpstipoparecer = 'R', dpsdataparecer = NOW() WHERE sbdid IN (".implode(", ", $sbdids).")");
	}
	
	$verificaPendentes = $db->pegaUm("SELECT count(sbdid) FROM par.documentoparreprogramacaosubacao WHERE dpsstatus <> 'I' AND repid = ".$repid);
	
	if( $verificaPendentes == 0 ){
		$db->executar("UPDATE par.reprogramacao SET repstatus = 'I' WHERE repid = ".$repid);
	}

	$db->commit();
	
	enviaEmailRecusaReprogramacao( $dopid ,'subacao', $parecer, $sbdids);
	
	echo "<script>
			alert('Reprogramação(ões) cancelada(s) com sucesso!');
			location.href='par.php?modulo=principal/popupReprogramacaoPARSubacoes&acao=A&dopid=".$dopid."&valida=1';
 		</script>";
}

function reprogramaSubacao($tipo){
	global $db;

	$dopid = $_REQUEST['dopid'];

	if( $tipo == 1 ){ // Apenas uma subação
		$sbdids = array('sbdid' => $_POST['sbdidAceite']);
	} elseif( $tipo == 2 ){
		$sbdids = $db->carregarColuna("SELECT sbdid FROM par.documentoparreprogramacaosubacao dpv WHERE dpv.dopid = ".$dopid." AND dpv.dpsstatus = 'P'");
	}

	if( is_array($sbdids) ){

		//Atualizo o termo para em reformulação
		$db->executar( "UPDATE par.documentopar SET dopreformulacao = TRUE WHERE dopid = ".$dopid );

		foreach( $sbdids as $sbdid ){

			$sql = "SELECT
							s.sbaid,
							sbdano,
							par.retornacodigosubacao(s.sbaid) as codigo,
							s.sbadsc as nome,
							p.inuid
						FROM
							par.subacaodetalhe sd
						INNER JOIN par.subacao s ON s.sbaid = sd.sbaid
						INNER JOIN par.acao a ON a.aciid = s.aciid
						INNER JOIN par.pontuacao p ON p.ptoid = a.ptoid
						WHERE
							sbdid = ".$sbdid;

			$dado = $db->pegaLinha( $sql );

			$inuid = $dado['inuid'];

			$subacoes[] = array('nome'=>$dado['nome'],'codigo'=>$dado['codigo'],'ano'=>$dado['sbdano']);

			$db->executar( "UPDATE par.subacao SET sbareformulacao = TRUE WHERE sbaid = ".$dado['sbaid'] );
                        
                        $parecer = $_REQUEST['dprjustificativa'];
			$db->executar( "UPDATE par.documentoparreprogramacaosubacao SET dpsstatus = 'A', usucpfvalidacao = '".$_SESSION['usucpf']."', dpsvalidacao = 't', dpsparecer ='".$parecer."', dpscpfparecer = '".$_SESSION['usucpf']."', dpstipoparecer = 'A', dpsdataparecer = NOW() WHERE sbdid = ".$sbdid );

			//SUBAÇÃO
			$sqlInsertSubacao = " INSERT INTO par.subacao
                        		( aciid, sbadsc, sbaordem, sbaobra, sbaestrategiaimplementacao,
								sbaptres, sbanaturezadespesa, sbamonitoratecnico, docid, frmid,
								indid, foaid, undid, ppsid, prgid, ptsid, sbacronograma, sbappspeso,
								sbaobjetivo, sbatexto, sbacobertura, usucpf, sbadataalteracao,
								sbastatus, sbaidpai, sbadatareformulacao )
								SELECT
									aciid, sbadsc, sbaordem, sbaobra, sbaestrategiaimplementacao,
									sbaptres, sbanaturezadespesa, sbamonitoratecnico, docid, frmid,
									indid, foaid, undid, ppsid, prgid, ptsid, sbacronograma, sbappspeso,
									sbaobjetivo, sbatexto, sbacobertura, usucpf, sbadataalteracao,
									'R' as sbastatus, sbaid, NOW()
								FROM
									par.subacao
								WHERE
									sbastatus = 'A' AND
									sbaid = {$dado['sbaid']}
								RETURNING
									sbaid";

			$novoidsubacao = $db->pegaUm($sqlInsertSubacao);

			//DETALHE
			$sqlInsertSubacaoDetalhe = "INSERT INTO par.subacaodetalhe
										(sbaid, sbdparecer, sbdquantidade, sbdano, sbdinicio, sbdfim,
										ssuid, sbdanotermino, sbdnaturezadespesa, sbddetalhamento, prpid,
            							sbdplanointerno, sbdparecerdemerito, sbdplicod, sbdptres, sbdrepassevlrcomplementar, sbdrepassevlrempenho, sbdrepassevlrraf, sbdrepassevlrcomplementaraprovado, sbdrepassevlrempenhoaprovado, sbdrepassevlrrafaprovado)
										SELECT
											$novoidsubacao, sbdparecer, sbdquantidade, sbdano, sbdinicio, sbdfim,
											ssuid, sbdanotermino, sbdnaturezadespesa, sbddetalhamento, prpid,
            								sbdplanointerno, sbdparecerdemerito, sbdplicod, sbdptres, sbdrepassevlrcomplementar, sbdrepassevlrempenho, sbdrepassevlrraf, sbdrepassevlrcomplementaraprovado, sbdrepassevlrempenhoaprovado, sbdrepassevlrrafaprovado
										FROM
											par.subacaodetalhe
										WHERE
											sbdid = ".$sbdid;
			$db->carregar($sqlInsertSubacaoDetalhe);
                          
                        
			$sql5 = "UPDATE par.subacaodetalhe SET  
						sbdrepassevlrcomplementar = '0',  
						sbdrepassevlrempenho = '0',  
						sbdrepassevlrraf = '0',  
						sbdrepassevlrcomplementaraprovado = '0',  
						sbdrepassevlrempenhoaprovado = '0',  
						sbdrepassevlrrafaprovado = '0' 
					WHERE sbaid = " . $dado['sbaid'];
			$db->executar($sql5);
                        
			//MODIFICAÇÃO SOLICITADA DIA 07/08/2014
            // Ao criar o BKP (no joinha e tela de projeto) não copiar os valores do complemento.
            //DEMANDA 807- 808
                        
			//ITENS
			$sqlItensAntigos = "SELECT
									icoid
								FROM
									par.subacaoitenscomposicao
								WHERE
									sbaid = ".$dado['sbaid']." AND icoano = ".$dado['sbdano'];
			$arrIcoid = $db->carregar( $sqlItensAntigos );

			$arrIcoAntigoNovo = array();
			if( is_array( $arrIcoid ) ){
				foreach( $arrIcoid as $icoid ){
					$sqlItens = "
								INSERT INTO par.subacaoitenscomposicao (
									sbaid, icoano, icoordem, icodescricao, icoquantidade, icovalor,
									icovalortotal, icostatus, unddid, icodetalhe, usucpf, dtatualizacao, picid, dicid,
									icoquantidadetecnico, icovalidatecnico/*, icoidpai*/ )
								SELECT
									$novoidsubacao, icoano, icoordem, icodescricao, icoquantidade, icovalor,
									icovalortotal, icostatus, unddid, icodetalhe, usucpf, dtatualizacao, picid, dicid,
									icoquantidadetecnico, icovalidatecnico/*, icoid*/
								FROM
									par.subacaoitenscomposicao
								WHERE
									icoid = ".$icoid['icoid']." AND icoano = ".$dado['sbdano']."
								RETURNING
									icoid";
					$icoidNovo = $db->pegaUm($sqlItens);

					$arrIcoAntigoNovo[$icoid['icoid']] = $icoidNovo;

				}
			}

			//ESCOLAS
			$sqlEscolasAntigas = "SELECT
									sesid
								FROM
									par.subacaoescolas
								WHERE
									sbaid=".$dado['sbaid']." AND sesano = ".$dado['sbdano'];
			$arrSesid = $db->carregar( $sqlEscolasAntigas );

			$arrSesAntigoNovo = array();
			if( is_array( $arrSesid ) ){
				foreach( $arrSesid as $sesid ){
					$sqlEscolas = "INSERT INTO par.subacaoescolas
									( sbaid, sesano, escid, sesquantidade, sesstatus, sesquantidadetecnico, sesvalidatecnico )
									SELECT
										$novoidsubacao, sesano, escid, sesquantidade, sesstatus, sesquantidadetecnico, sesvalidatecnico
									FROM
										par.subacaoescolas
									WHERE
										sesid = {$sesid['sesid']} AND sesano = {$dado['sbdano']}
									RETURNING
										sesid";
					$sesidNovo = $db->pegaUm($sqlEscolas);

					// Pego todos os itens relacionados a escola antiga
					$sqlEscIt = "SELECT
									icoid
								FROM
									par.subescolas_subitenscomposicao
								WHERE
									sesid = ".$sesid['sesid'];
					$itensVelhos = $db->carregar( $sqlEscIt );

					if( is_array( $itensVelhos ) ){
						foreach( $itensVelhos as $i => $it ){
							if( $arrIcoAntigoNovo[$it['icoid']] ){
								$sqlSubEsc = "INSERT INTO par.subescolas_subitenscomposicao
											( sesid, icoid, seiqtd, seiqtdtecnico )
											SELECT
												$sesidNovo, {$arrIcoAntigoNovo[$it['icoid']]}, seiqtd, seiqtdtecnico
											FROM
												par.subescolas_subitenscomposicao
											WHERE
												sesid = ".$sesid['sesid']." AND icoid = ".$it['icoid'];
								$db->carregar($sqlSubEsc);
							}
						}
					}
				}
			}

			//OBRAS
			$sqlObras = "INSERT INTO par.subacaoobra
						( sbaid, preid, sobano )
						SELECT
							$novoidsubacao, preid, sobano
						FROM
							par.subacaoobra
						WHERE
							sbaid = ".$dado['sbaid']." AND sobano = ".$dado['sbdano'];
			$db->carregar($sqlObras);

			$db->executar( "UPDATE par.subacaodetalhe SET ssuid = 20, sbdreformulacao = TRUE WHERE sbdid = ".$sbdid );
		}

		$db->commit();
			//enviaEmailPlanodeMetasSubacaoReformulacao( $_SESSION['par']['inuid'], $dado['sbaid'] );

		if( trim($inuid) != '' && is_array($subacoes) ){
			enviaEmailLiberacaoReprogramacao( $inuid, $subacoes );
		}

		echo "<script>
				alert('Subação(ões) aceita(s) com sucesso!');
				location.href='par.php?modulo=principal/popupReprogramacaoPARSubacoes&acao=A&dopid=".$dopid."&valida=1';
	 		</script>";

	}

}

/* Essa função era chamada na pagina administrarProjeto e trouxe pra q para funcionar independentemente de inuid de sessão.
 * */
function carregaDadoItens(){
	global $db;

	$cronograma = $db->pegaUm( "SELECT sbacronograma FROM par.subacao WHERE sbaid = ".$_POST['sbaid'] );


	//        ver($_POST['sbaid'],d);

	if( $cronograma == 1 ){ // GLOBAL

		$sql = "SELECT DISTINCT
				sic.icodescricao,
				sic.icoquantidadetecnico as quantidade,
				TO_CHAR( sic.icovalor, '999G999G999D99'),
				TO_CHAR( ( sic.icoquantidadetecnico * sic.icovalor), '999G999G999D99') as valortotal,
                emi.adesao
			FROM
				par.subacaoitenscomposicao sic
			INNER JOIN par.propostaitemcomposicao pic ON pic.picid = sic.picid
			LEFT JOIN par.empenhopregaoitensenviados emi ON emi.idsigarp = pic.idsigarp AND emi.sbaid = sic.sbaid AND emi.epistatus = 'A'
            --LEFT JOIN par.processoitemadesaopregao piap ON sic.icoid = piap.icoid AND piap.piastatus = 'A'
			WHERE
				sic.icovalidatecnico = 'S' AND
				sic.sbaid = ".$_POST['sbaid'];

	} else { // ESCOLA

		$sql = "SELECT
                        sic.icodescricao,
                        SUM( sse.seiqtdtecnico ) as quantidade,
                        TO_CHAR( sic.icovalor, '999G999G999D99'),
                        TO_CHAR(SUM( sse.seiqtdtecnico * sic.icovalor ), '999G999G999D99') as valortotal,
                        emi.adesao
                FROM
                        par.subacaoitenscomposicao sic
                INNER JOIN par.subescolas_subitenscomposicao  sse ON sse.icoid = sic.icoid
                INNER JOIN par.propostaitemcomposicao pic ON pic.picid = sic.picid
				LEFT JOIN par.empenhopregaoitensenviados emi ON emi.idsigarp = pic.idsigarp AND emi.sbaid = sic.sbaid AND emi.epistatus = 'A'
                --LEFT JOIN par.processoitemadesaopregao piap ON sic.icoid = piap.icoid
                WHERE
                        sic.icovalidatecnico = 'S' AND
                        sic.sbaid = ".$_POST['sbaid']."
                GROUP BY
                        sic.icodescricao, sic.icovalor, sic.icoquantidadetecnico, emi.adesao";
		//ver($sql);
	}

	$cabecalho = array("Descrição do Item", "Quantidade", "Valor", "Valor Total", 'Número da adesão' );
	$db->monta_lista($sql,$cabecalho,50000,5,'N','90%','N');

}


?>
<form name="formulario" id="formulario" method="post">
	<link rel="stylesheet" type="text/css" href="/includes/Estilo.css"/>
	<link rel="stylesheet" type="text/css" href="/includes/listagem.css"/>
	<script type="text/javascript" src="../includes/funcoes.js"></script>
	<script type="text/javascript" src="/includes/prototype.js"></script>
	<script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/dateFunctions.js"></script>
	<script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>
	<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>
	<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
	<script type="text/javascript">
		function selecionatipo(dado){
			dopid = $('#dopid').val();

			if( dado.value == 1 ){ // Prazo
				window.location = 'par.php?modulo=principal/popupReprogramacaoPAR&acao=A&dopid='+dopid+'&tipo='+dado.value;
			} else if( dado.value == 2 ) { // Subação
				window.location = 'par.php?modulo=principal/popupReprogramacaoPARSubacoes&acao=A&dopid='+dopid+'&tipo='+dado.value;
			} else { // Termo
				alert('Não disponível no momento!');
			}
		}
		function carregarListaItens(idImg, sbaid){
			var img 	 = $( '#'+idImg );
			var tr_nome = 'listaDocumentos2_'+ sbaid;
			var td_nome  = 'trI_'+ sbaid;

			if($('#'+tr_nome).css('display') == 'none' && $('#'+td_nome).html() == ""){
				$('#'+td_nome).html('Carregando...');
				img.attr ('src','../imagens/menos.gif');
				carregaListaItens(sbaid, td_nome);
			}
			if($('#'+tr_nome).css('display') == 'none' && $('#'+td_nome).html() != ""){
				$('#'+tr_nome).css('display','');
				img.attr('src','../imagens/menos.gif');
			} else {
				$('#'+tr_nome).css('display','none');
				img.attr('src','/imagens/mais.gif');
			}
		}

		function carregaListaItens(sbaid, td_nome){
			divCarregando();
			$.ajax({
			   		type: "POST",
			   		//url: "par.php?modulo=principal/administrarProjeto&acao=A",
			   		url: window.location.href,
			   		data: "requisicao=carregaDadosItens&sbaid="+sbaid,
			   		async: false,
			   		success: function(msg){
			   			$('#'+td_nome).html(msg);
			   			divCarregado();
			   		}
				});
		}

		function cancelaReformulacao(sbaid, ano){
			divCarregando();
			if(confirm("Você tem certeza que deseja cancelar essa reformulação?\nTodas as alterações serão perdidas.")){
				$('#sbaidCancelamento').val(sbaid)
				$('#anoCancelamento').val(ano)
				$('#cancelaReformulacao').val('cancela')
				$('#formulario').submit();
			} else {
				divCarregado();
			}
		}

		function reprogramarSubacao(){
			erro = 0;
			$('[id^=sbdid_]').each(function(i, v) {
				if( $(v).attr('checked') ){
					erro = 1;
				}
			});
			if( erro == 0 ){
				alert('Você precisa selecionar uma subação para reformular!');
			} else {
				if( $('#dprjustificativa').val() == '' ){
					alert('Campo obrigatório!');
					$('#dprjustificativa').focus();
					return false;
				}
				if( confirm( "Tem certeza que deseja reprogramar essa(s) subação(ões)?" ) ){
					$('#formulariomontalista').children('table').after('<input type="hidden" name="dprjustificativa" value="'+$('#dprjustificativa').val()+'"/>');
					divCarregando('','Reprogramação em Andamento<br>Isso pode levar algum tempo. Aguarde...');
					$('#formulariomontalista').submit();
				}
			}
		}

		function aprovarReprogramacao(sbdid){
                    if( $('#dprjustificativa').val() == '' ){
                        alert('O campo Justificativa é obrigatório.');
                        $('#dprjustificativa').focus();
                        return false;
                    }
                    divCarregando('','Reprogramação em Andamento<br>Isso pode levar algum tempo. Aguarde...');
                    $('#requisicao').val('aceita');
                    $('#sbdidAceite').val(sbdid);
                    $('#formulario').submit();
		}

		function recusarReprogramacao(sbdid){
                    if( $('#dprjustificativa').val() == '' ){
                        alert('O campo Justificativa é obrigatório.');
                        $('#dprjustificativa').focus();
                        return false;
                    }
                    $('#requisicao').val('cancela');
                    $('#sbdidAceite').val(sbdid);
                    $('#formulario').submit();
		}

		function aprovarTodasReprogramacao(){
                    if( $('#dprjustificativa').val() == '' ){
                        alert('O campo Justificativa é obrigatório.');
                        $('#dprjustificativa').focus();
                        return false;
                    }
                    divCarregando('','Reprogramação em Andamento<br>Isso pode levar algum tempo. Aguarde...');
                    $('#requisicao').val('aceitaTodas');
                    $('#formulario').submit();
		}

		function recusarTodasReprogramacao(){
			if( $('#tr_justificativa').css('display') == 'none' ){
				$('#tr_justificativa').show();
				alert('O campo Justificativa é obrigatório.');
				return false;
			}
			if( $('#dprjustificativa').val() == '' ){
				alert('O campo Justificativa é obrigatório.');
				$('#dprjustificativa').focus();
				return false;
			}
			$('#requisicao').val('cancelaTodas');
			$('#formulario').submit();
		}
	</script>
	<?php
	global $db;

	$sql = "select
				dopdatafimvigencia as prazo
			from
				par.documentopar
			where
				dopid = ".$_REQUEST['dopid'];

	$prazo = $db->pegaUm($sql);

	if( substr($prazo, 0, 2) == '02' ){
		$var = '28';
	} else {
		$var = '30';
	}

	$perfil = pegaArrayPerfil($_SESSION['usucpf']);

	$display = 'disabled="disabled"';
	if( in_array( PAR_PERFIL_SUPER_USUARIO, $perfil ) || in_array( PAR_PERFIL_ADMINISTRADOR, $perfil ) || in_array( PAR_PERFIL_PREFEITO, $perfil ) || in_array( PAR_PERFIL_EQUIPE_ESTADUAL_APROVACAO, $perfil ) || in_array( PAR_PERFIL_EQUIPE_MUNICIPAL_APROVACAO, $perfil ) || in_array(PAR_PERFIL_UNIVERSIDADE_ESTADUAL, $perfil) || in_array(PAR_PERFIL_ENTIDADE_EXECUTORA, $perfil) ){
		$display = "";
	}

	$verificacao = "<input type=\"checkbox\" disabled >";
	$botoes = '<input type="button" onclick="javascript: reprogramarSubacao();" value="Solicitar Reprogramação" '.$display.' >';

	if( $_REQUEST['valida'] == 1 ){
		$display = 'disabled="disabled"';
		$verificacao = "";
		$v = "style='display: none;'";
		$w = " AND sd.sbdid IN ( SELECT sbdid FROM par.documentoparreprogramacaosubacao dpv WHERE dpv.dopid = dop.dopid AND dpv.dpsstatus = 'P' )";
		if( in_array( PAR_PERFIL_SUPER_USUARIO, $perfil ) || in_array( PAR_PERFIL_ADMINISTRADOR, $perfil ) ){
			$display = '';
			$verificacao = "<img src=\"../imagens/workflow/1.png\" style=\"cursor:pointer;\" title=\"Aprovar Reprogramação\" onclick=\"aprovarReprogramacao('|| sd.sbdid ||');\" border=\"0\"> &nbsp;<img src=\"../imagens/workflow/2.png\" style=\"cursor:pointer;\" title=\"Recusar Reprogramação\" onclick=\"recusarReprogramacao('|| sd.sbdid ||');\" border=\"0\"> ";
		}
		$botoes = '<input type="button" onclick="javascript: aprovarTodasReprogramacao();" value="Aceitar Todas" '.$display.' >&nbsp;<input type="button" onclick="javascript: recusarTodasReprogramacao();" value="Recusar Todas" '.$display.' >';
	}
	?>

	<input type="hidden" id="dataAtual" name="dataAtual" value="<?php echo formata_data_sql($var.'/'.$prazo); ?>">
	<input type="hidden" id="dopid" name="dopid" value="<?php echo $_REQUEST['dopid']; ?>">
	<input type="hidden" id="requisicao" name="requisicao" value="">
	<input type="hidden" id="dias" name="dias" value="">
	<input type="hidden" name="sbaidCancelamento" id="sbaidCancelamento" value="">
	<input type="hidden" name="sbdidAceite" id="sbdidAceite" value="">
    <input type="hidden" name="anoCancelamento" id="anoCancelamento" value="">
    <input type="hidden" name="cancelaReformulacao" id="cancelaReformulacao" value="">
	<table align="center" cellspacing="0" cellpadding="3" border="0" bgcolor="#DCDCDC" style="border-top: none; border-bottom: none;" class="tabela">
		<tr>
			<td align="center" bgcolor="#e9e9e9" style="FILTER: progid:DXImageTransform.Microsoft.Gradient(startColorStr=\'#FFFFFF\', endColorStr=\'#dcdcdc\', gradientType=\'1\')">
				<b>Reprogramação de Subação</b>
			</td>
		</tr>
	</table>
	<table align="center" border="0" width="95%" class="tabela" cellpadding="3" cellspacing="2">
		<tr>
			<td colspan="2">
			<?php
			
			$sql = "SELECT dopid FROM par.documentoparreprogramacaosubacao WHERE dpsstatus = 'P' AND dopid = ".$_REQUEST['dopid'];
			$testaRep = $db->pegaUm($sql);
			
			$disabled = "";
			if( $testaRep ){
				$disabled = "disabled";
			}
			
			$visualizaSub1 = "<img style=\"cursor:pointer\" title=\"Editar Reformulação\" src=\"/imagens/editar_nome.gif\" onclick=\"janela(\'par.php?modulo=principal/subacao&acao=A&sbaid=' || sd.sbaid || '&anoatual='||sd.sbdano||'\',800,600,\'Subação\');\" border=\"0\" />&nbsp;";
			$visualizaSub2 = "<img style=\"cursor:pointer\" title=\"Editar Reformulação\" src=\"/imagens/icone_lupa.png\" onclick=\"janela(\'par.php?modulo=principal/subacao&acao=A&sbaid=' || sd.sbaid || '&anoatual='||sd.sbdano||'\',800,600,\'Subação\');\" border=\"0\" /> &nbsp;";
			if( in_array(PAR_PERFIL_UNIVERSIDADE_ESTADUAL, $perfil) || in_array(PAR_PERFIL_ENTIDADE_EXECUTORA, $perfil) ){
				$visualizaSub1 = "<img style=\"cursor:pointer\" title=\"Editar Reformulação\" src=\"/imagens/editar_nome.gif\" onclick=\"janela(\'par.php?modulo=principal/subacaoVisualizar&acao=A&sbaid=' || sd.sbaid || '&sbdano=' || sd.sbdano || '\',800,600,\'Subação\');\" border=\"0\" />&nbsp;";
				$visualizaSub2 = "<img style=\"cursor:pointer\" title=\"Editar Reformulação\" src=\"/imagens/icone_lupa.png\" onclick=\"janela(\'par.php?modulo=principal/subacaoVisualizar&acao=A&sbaid=' || sd.sbaid || '&sbdano=' || sd.sbdano || '\',800,600,\'Subação\');\" border=\"0\" /> &nbsp;";
			}

			$sql = "SELECT DISTINCT
							CASE WHEN ( s.sbareformulacao IS TRUE AND dop.dopreformulacao IS TRUE AND (sd.ssuid = 20 OR sd.ssuid = 21) ) THEN
								'<center><span style=\"white-space: nowrap;\"><img style=\"cursor:pointer\" id=\"img_subacao_' || sd.sbaid || '\" src=\"/imagens/mais.gif\" onclick=\"carregarListaItens(this.id,\'' || sd.sbaid || '\');\" border=\"0\" />&nbsp;
								{$visualizaSub}
								<img style=\"cursor:pointer\" title=\"Cancelar Reformulação\" src=\"/imagens/excluir.gif\" onclick=\"cancelaReformulacao(\'' || sd.sbaid || '\', \'' || sd.sbdano || '\');\" border=\"0\" /></span></center>'
							ELSE
                                                        
                                 /*CASE WHEN (s.prgid != 153) THEN*/	
								'<center><span style=\"white-space: nowrap;\"><img style=\"cursor:pointer\" id=\"img_subacao_' || sd.sbaid || '\" src=\"/imagens/mais.gif\" onclick=\"carregarListaItens(this.id,\'' || sd.sbaid || '\');\" border=\"0\" /> &nbsp;
								{$visualizaSub2}'
                                                        /*ELSE '' END */||
								
								CASE WHEN ( SELECT DISTINCT true FROM par.documentoparreprogramacaosubacao dpv
											WHERE dpv.dopid = dop.dopid AND dpv.sbdid = sd.sbdid AND dpv.dpsstatus = 'P' ) = true
									THEN '$verificacao'
									ELSE 
										--CASE WHEN icc.sccid IS NOT NULL 
											--THEN '<img style=\"cursor:pointer\" title=\"Não é possivel reformular essa subação pois se encontra em monitoramento.\" 
												--		onclick=\"alert(''Não é possivel reformular essa subação pois se encontra em monitoramento.'')\" 
													--	src=\"/imagens/atencao.png\" border=\"0\" />&nbsp;' 
											--ELSE 
									/*CASE WHEN (s.prgid != 153) THEN*/	
									'<input type=\"checkbox\" ".$disabled." name=\"sbdid[]\" id=\"sbdid_' || sd.sbdid || '\" value=\"' || sd.sbdid || '\" $display >'
									 /*ELSE '' END*/

									  || '</span></center>' 
										END --END
							END as acao,
							/*CASE WHEN (s.prgid = 153) THEN '<span style=\"color: #999;\" >' || s.sbadsc || ' <span style=\"color: red;\">Esta subação foi adicionada pelo programa \"FNDE - Programa Caminho da Escola - Ônibus Escolar Acessível\", deste modo não pode ser reprogramada.</span></span>' ELSE*/ 
							s.sbadsc 
							/*END*/  
							as sbadsc,
							CASE WHEN ( s.sbareformulacao IS TRUE AND dop.dopreformulacao IS TRUE AND (sd.ssuid = 20 OR sd.ssuid = 21) ) THEN
								'<center><b>Em Reformulação</b></center>'
							ELSE
								CASE WHEN ( SELECT DISTINCT true FROM par.documentoparreprogramacaosubacao dpv WHERE dpv.dopid = dop.dopid AND dpv.sbdid = sd.sbdid AND dpv.dpsstatus = 'P' ) = true THEN '<center><b>Aguardando Reprogramação</b></center>' ELSE '' END
							END as situacao,
							sd.sbdano||'&nbsp;' as sbdano,
							TO_CHAR( (SELECT par.recuperavalorvalidadossubacaoporano(s.sbaid, sd.sbdano)), '999G999G999D99') || '</td></tr>
											            	<tr style=\"display:none\" id=\"listaDocumentos2_' || sd.sbaid || '\" >
											            		<td id=\"trI_' || sd.sbaid || '\" colspan=8 ></td>
											            </tr>' as valor
						FROM
							par.documentopar dop
						INNER JOIN par.processoparcomposicao 			ppc ON dop.prpid = ppc.prpid and ppc.ppcstatus = 'A'
						INNER JOIN par.subacaodetalhe 					sd  ON sd.sbdid = ppc.sbdid
						INNER JOIN par.subacao 							s   ON s.sbaid = sd.sbaid
						INNER JOIN par.subacaoitenscomposicao 			ico ON ico.sbaid = sd.sbaid AND ico.icoano = sd.sbdano
						LEFT  JOIN par.subacaoitenscomposicaocontratos 	icc ON icc.icoid = ico.icoid AND sccstatus = 'A'
						WHERE
							dop.dopid = ".$_REQUEST['dopid']." ".$w."
						ORDER BY sbadsc";
// ver(simec_htmlentities($sql));
				$cabecalho = array("&nbsp;Ações&nbsp;", "Descrição da Subação", "Situação da Subação", "Ano da Subação", "Valor da Subação" );
				$db->monta_lista($sql,$cabecalho,50000,5,'N','95%','S','formulariomontalista');
			?></td>
		</tr>
		<?php
                $arrSubacao = $db->carregar($sql);
                $arrSubacao = $arrSubacao ? $arrSubacao : array();
                if (count($arrSubacao)>0) {
                
		$sql = "SELECT dpsjustificativa FROM par.documentoparreprogramacaosubacao WHERE dpsstatus <> 'I' AND dopid = {$_REQUEST['dopid']}";
		$motivo = $db->pegaUm($sql);

		if( $motivo != '' ){
	 	?>
		<tr>
			<td class="SubTituloDireita" width="20%">
				Motivo:
			</td>
			<td><?=$motivo?>
			</td>
		</tr>
		<?php } ?>
		<tr id=tr_justificativa>
			<td class="SubTituloDireita">
				Justificativa:
			</td>
			<td>
				<?php echo campo_textarea('dprjustificativa', 'S', 'S', 'Justificativa', 60, 5, 1000) ?>
			</td>
		</tr>
		<tr>
			<td colspan="2"><?php echo $botoes; ?></td>
		</tr>
                <?php
                }
                ?>
	</table>
</form>