<?php

if( $_POST['sbdid'] ){
	
	ini_set("memory_limit","1000M");
	set_time_limit(0);

	$sql = "SELECT 
				dop.dopid 
			FROM 
				par.processopar  prp 
			INNER JOIN par.vm_documentopar_ativos dop ON dop.prpid = prp.prpid
			WHERE
				prp.prpid = ".$_REQUEST['prpid'];
	
	$dopid = $db->pegaUm($sql);
	
	
	if( is_array( $_POST['sbdid'] ) ){
		
		//Atualizo o termo para em reformulação
		
		$sql = "UPDATE par.documentopar SET dopreformulacao = TRUE WHERE dopid = ".$dopid;
		
		$db->executar( $sql );
		
		foreach( $_POST['sbdid'] as $sbdid ){
			
			$sql = "SELECT sbaid, sbdano FROM par.subacaodetalhe WHERE sbdid = ".$sbdid;
			
			$dado = $db->pegaLinha( $sql );

			$sql = "UPDATE par.subacao SET sbareformulacao = TRUE WHERE sbaid = ".$dado['sbaid'] ;
			
			$db->executar( $sql );
			
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
            							sbdplanointerno, sbdparecerdemerito, sbdplicod, sbdptres)
										SELECT
											$novoidsubacao, sbdparecer, sbdquantidade, sbdano, sbdinicio, sbdfim,
											ssuid, sbdanotermino, sbdnaturezadespesa, sbddetalhamento, prpid, 
            								sbdplanointerno, sbdparecerdemerito, sbdplicod, sbdptres
										FROM
											par.subacaodetalhe
										WHERE
											sbdid = ".$sbdid;
			$db->carregar($sqlInsertSubacaoDetalhe);

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
					$sqlItens = "INSERT INTO par.subacaoitenscomposicao
									(sbaid, icoano, icoordem, icodescricao, icoquantidade, icovalor,
									icovalortotal, icostatus, unddid, icodetalhe, usucpf, dtatualizacao, picid, dicid,
									icoquantidadetecnico, icovalidatecnico )
									SELECT
										$novoidsubacao, icoano, icoordem, icodescricao, icoquantidade, icovalor,
										icovalortotal, icostatus, unddid, icodetalhe, usucpf, dtatualizacao, picid, dicid, 
										icoquantidadetecnico, icovalidatecnico
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
		
		
		if($db->commit()){
			enviaEmailPlanodeMetasSubacaoReformulacao( $_SESSION['par']['inuid'], $dado['sbaid'] );
		}
		echo "<script>
				alert('Subação(ões) reformulada(s) com sucesso!');
				location.href='par.php?modulo=principal/projetos&acao=A';
	 		</script>";
	}
}

if( $_POST['sbdidSubacao'] ){
	
	$dopid = $db->pegaUm("SELECT 
				dop.dopid 
			FROM 
				par.processopar  prp 
			INNER JOIN par.vm_documentopar_ativos dop ON dop.prpid = prp.prpid
			WHERE
				prp.prpid = ".$_REQUEST['prpid']);
	
	
	if( is_array( $_POST['sbdidSubacao'] ) ){
		
		//Atualizo o termo para em reformulação
		$db->executar( "UPDATE par.documentopar SET dopreformulacao = TRUE WHERE dopid = ".$dopid );
		
		foreach( $_POST['sbdidSubacao'] as $sbdid ){
			$dado = $db->pegaUm("SELECT sbdid FROM par.processoparcomposicao WHERE ppcstatus = 'A' and prpid = ".$_REQUEST['prpid']." AND sbdid = ".$sbdid);
			if( !$dado ){
				$sqlInsert .= "INSERT INTO par.processoparcomposicao ( prpid, sbdid ) VALUES ( ".$_REQUEST['prpid'].", ".$sbdid." );";
			}
		}
		
		$db->executar($sqlInsert);
		$db->commit();
		echo "<script>
					alert('Subação(ões) adicionada(s) com sucesso!');
					location.href='par.php?modulo=principal/projetos&acao=A&abas=reformularProjeto&prpid=".$_REQUEST['prpid']."';
		 		</script>";
		exit();
	}
}

if( $_POST['cancelaReformulacao'] == 'cancela' ){
	global $db;

	$dado = deletaReformulacao( $_POST['sbaidCancelamento'], $_POST['anoCancelamento'] );

	echo "<script>
				alert('Reformulação cancelada com sucesso!');
				location.href='par.php?modulo=principal/projetos&acao=A&abas=reformularProjeto&prpid=".$_REQUEST['prpid']."';
	 		</script>";
	exit;
}

if( !$_REQUEST['prpid'] ){
	echo '<script>alert("Escolha um Processo para reformular!");
			history.back(-1);
	</script>';
	die();
}

//include  APPRAIZ."includes/cabecalho.inc";
echo'<br>';

/*if( $_SESSION['baselogin'] == 'simec_desenvolvimento' ){
	$mnuid = array( 0 => 12185 ); // Esconde a Aba "Histórico de Reformulações"
} else {
	$mnuid = array( 0 => 11862, 1 => 11863 ); // Esconde a Aba "Histórico de Reformulações" e "Montar Projeto"
}*/
//$db->cria_aba( $abacod_tela, $url, $parametros, $mnuid );
include_once APPRAIZ.'par/classes/Projeto.class.inc';
$obProjeto = new Projeto( $_REQUEST );

echo montarAbasArray( $obProjeto->criaAbaPar(6), "par.php?modulo=principal/projetos&acao=A&abas=distribuirEmpenho&prpid={$_REQUEST['prpid']}" );

$entidadePar = ($_SESSION['par']['itrid'] == 1) ? $_SESSION['par']['estuf'] : $_SESSION['par']['muncod'];
$nmEntidadePar = EntidadeParControle::recuperaDescricaoEntidadePar($entidadePar,$_SESSION['par']['itrid']);
monta_titulo( $nmEntidadePar, 'Distribuir Valor Empenho' );

$sql = "select 
			substring(p.prpnumeroprocesso, 12, 4) as ano, 
		    p.prpnumeroprocesso as processo, 
		    case when p.sisid = 57 then 'Subações de Emendas no PAR'
		                        	else 'Subações Genérico do PAR' end as tipo
		from par.processopar p where prpid = {$_REQUEST['prpid']}";
$arProcesso = $db->pegaLinha($sql);

?>
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<form name="formulario" id="formulario" method="post">   
    <input type="hidden" name="sbaidCancelamento" id="sbaidCancelamento" value="">
    <input type="hidden" name="anoCancelamento" id="anoCancelamento" value="">
    <input type="hidden" name="cancelaReformulacao" id="cancelaReformulacao" value="">
	<?=cabecalhoProcesso($arProcesso); ?>
	<table align="center" border="0" width="95%" class="tabela" cellpadding="3" cellspacing="2">
		<tr>
			<td class="SubTituloDireita"><center><b>Subações a serem reformuladas</b></center></td>
		</tr>
		<tr>
			<td>
			<table cellSpacing="0" cellPadding="0" align="center" style="width: 100%;">
				<tr>
					<td class="subtitulodireita" width="25%" style="font-weight: bold" valign="top">Legenda:</td>
					<td valign="top" style="font-weight: bold" width="75%">&nbsp;<img src='/imagens/alteracao.gif'>&nbsp;-&nbsp;Alterar dados empenho</td>
				</tr>
			</table>
			</td>
		</tr>
		<tr id="tr_div">
			<td> 
			<?
			// Quando eu reformulo novamente ele se perde mostrando todas as subações reformuladas!
			$sql = "SELECT DISTINCT
							'<img style=\"cursor:pointer\" id=\"img_subacao_' || sd.sbdid || '\" src=\"/imagens/mais.gif\" onclick=\"carregarListaItens(this.id,\'' || sd.sbdid || '\');\" border=\"0\" />'||
								case when (select count(es.eobid) from par.empenhosubacao es where sbaid = s.sbaid and es.eobstatus = 'A') > 0 then
	                                	'&nbsp;&nbsp;<img style=\"cursor:pointer\" title=\"Alterar dados empenho\" src=\"/imagens/alteracao.gif\" onclick=\"janela(\'par.php?modulo=principal/alterarDadosEmpenho&acao=A&sbaid=' || sd.sbaid || '&prpid=".$_REQUEST['prpid']."\',1000,600,\'Subação\');\" border=\"0\" />'
	                                else
	                                ''
	                                end as acao,
							par.retornacodigosubacao(s.sbaid),
							s.sbadsc,
							CASE WHEN ( s.sbareformulacao IS TRUE AND dop.dopreformulacao IS TRUE AND (sd.ssuid = 20 OR sd.ssuid = 21) ) THEN '<center><b>Em Reformulação</b></center>' ELSE '' END as situacao,
							sd.sbdano,
							TO_CHAR( coalesce((SELECT par.recuperavalorvalidadossubacaoporano(s.sbaid, sd.sbdano)), 0), '999G999G999D99') || '</td></tr>
											            	<tr style=\"display:none\" id=\"listaDocumentos2_' || sd.sbdid || '\" >
											            		<td id=\"trI_' || sd.sbdid || '\" colspan=8 ></td>
											            </tr>' as valor
						FROM 
							par.processoparcomposicao ppc
						INNER JOIN par.subacaodetalhe sd ON sd.sbdid = ppc.sbdid
						INNER JOIN par.subacao s ON s.sbaid = sd.sbaid
						LEFT JOIN par.documentopar dop ON dop.prpid = ppc.prpid and dop.dopreformulacao = true
					--	INNER JOIN par.empenhosubacao ems ON ems.sbaid = sd.sbaid AND ems.eobano = sd.sbdano and eobstatus = 'A'
						WHERE
							ppc.prpid = ".$_REQUEST['prpid']."
							and ppc.ppcstatus = 'A' 
						ORDER BY
							par.retornacodigosubacao(s.sbaid), s.sbadsc";
			
				$cabecalho = array("&nbsp;Ações&nbsp;", "Localizador", "Descrição da Subação", "Situação da Subação", "Ano da Subação", "Valor da Subação" );
				$db->monta_lista($sql,$cabecalho,50000,5,'N','95%','S','formulariomontalista');
			?></td>
		</tr>
	</table>
<script type="text/javascript">

function carregarListaItens(idImg, sbdid){
	var img 	 = $( '#'+idImg );
	var tr_nome = 'listaDocumentos2_'+ sbdid;
	var td_nome  = 'trI_'+ sbdid;
	
	if($('#'+tr_nome).css('display') == 'none' && $('#'+td_nome).html() == ""){
		$('#'+td_nome).html('Carregando...');
		img.attr ('src','../imagens/menos.gif');
		carregaListaItens(sbdid, td_nome);
	}
	if($('#'+tr_nome).css('display') == 'none' && $('#'+td_nome).html() != ""){
		$('#'+tr_nome).css('display','');
		img.attr('src','../imagens/menos.gif');
	} else {
		$('#'+tr_nome).css('display','none');
		img.attr('src','/imagens/mais.gif');
	}
}

function carregaListaItens(sbdid, td_nome){
	divCarregando();
	$.ajax({
	   		type: "POST",
	   		url: "par.php?modulo=principal/administrarProjeto&acao=A",
	   		data: "requisicao=carregaDadoItensReformular&sbdid="+sbdid,
	   		async: false,
	   		success: function(msg){
		   		console.log(msg);
	   			$('#'+td_nome).html(msg);
	   			divCarregado();
	   		}
		});
}
</script>
</form>
