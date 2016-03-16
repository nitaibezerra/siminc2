<?php


if($_REQUEST['prpid'])
{
	$sql = "SELECT
		prpnumeroprocesso
	FROM
		par.processopar prp
	WHERE
		prpid = {$_REQUEST['prpid']}";

	$prpnumeroprocesso = $db->pegaUm($sql);

	if($prpnumeroprocesso)
	{
		$db->executar("UPDATE par.emailreprogramacaobloqueada SET erbstatus = 'I' where erbnumeroprocesso = '{$prpnumeroprocesso}' AND erbstatus = 'A'");
		$db->commit();
	}
}

//4.2.12.2 - 3209947
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
		
		$repid = $db->pegaUm("SELECT repid FROM par.reprogramacao WHERE repstatus = 'A' AND dopidoriginal = ".$dopid);
		
		if( !$repid ){
			$insereReprogramacao = "INSERT INTO par.reprogramacao (treid, repdtinicio, repstatus, dopidoriginal) VALUES ( 2, NOW(), 'A', ".$dopid.") RETURNING repid";
			$repid = $db->pegaUm($insereReprogramacao);
		}
		
		foreach( $_POST['sbdid'] as $sbdid ){
			
			$sql = "";
			$sql = "SELECT sbaid, sbdano FROM par.subacaodetalhe WHERE sbdid = ".$sbdid;			
			$dado = $db->pegaLinha( $sql );
			
//			if( $_POST['tiporeprogramacao'][0] == 2 ){
//				$sql = "UPDATE par.subacaodetalhe SET sbdrepitemvencido = TRUE WHERE sbdid = ".$sbdid."; ";
//				$db->executar( $sql );
//			}
			
			$sqlInsr = "INSERT INTO par.documentoparreprogramacaosubacao ( dopid, usucpf, usucpfvalidacao, sbdid, dpsstatus, dpsdata, dpsjustificativa, repid, dpsvalidacao )
						VALUES ( ".$dopid.", '".$_SESSION['usucpf']."', '".$_SESSION['usucpf']."', ".$sbdid.", 'A', 'NOW()', 'Reprogramação Técnica', ".$repid.", 't' ); ";
			$db->executar( $sqlInsr );
			
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
                        
                        $sql5 = "UPDATE par.subacaodetalhe SET  "
                                . "sbdrepassevlrcomplementar = '0',  "
                                . "sbdrepassevlrempenho = '0',  "
                                . "sbdrepassevlrraf = '0',  "
                                . "sbdrepassevlrcomplementaraprovado = '0',  "
                                . "sbdrepassevlrempenhoaprovado = '0',  "
                                . "sbdrepassevlrrafaprovado = '0' "              
                                . "WHERE sbaid = " . $dado['sbaid'] ;
                        $db->executar($sql5);

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
									icovalortotal, icostatus, unddid, icodetalhe, usucpf, dtatualizacao, picid, 
									icoquantidadetecnico, icovalidatecnico, dicid )
									SELECT
										$novoidsubacao, icoano, icoordem, icodescricao, icoquantidade, icovalor,
										icovalortotal, icostatus, unddid, icodetalhe, usucpf, dtatualizacao, picid, 
										icoquantidadetecnico, icovalidatecnico, dicid
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
				location.href='par.php?modulo=principal/projetos&acao=A&abas=reformularProjeto&prpid={$_REQUEST['prpid']}';
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
			$dado = $db->pegaUm("SELECT sbdid FROM par.processoparcomposicao WHERE ppcstatus = 'A' and  prpid = ".$_REQUEST['prpid']." AND sbdid = ".$sbdid);
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

echo montarAbasArray( $obProjeto->criaAbaPar(2), "par.php?modulo=principal/projetos&acao=A&abas=reformularProjeto&prpid={$_REQUEST['prpid']}" );

$entidadePar = ($_SESSION['par']['itrid'] == 1) ? $_SESSION['par']['estuf'] : $_SESSION['par']['muncod'];
$nmEntidadePar = EntidadeParControle::recuperaDescricaoEntidadePar($entidadePar,$_SESSION['par']['itrid']);
monta_titulo( $nmEntidadePar, 'Reformular Projeto' );

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
			<?php $entidadeDesc = ($_SESSION['par']['itrid'] == 1) ? 'estado' : 'município' ?>
			<td>
			<table cellSpacing="0" cellPadding="0" align="center" style="width: 100%;">
				<tr>
					<td class="subtitulodireita" width="10%" style="font-weight: bold" valign="top">Legenda:</td> 
					<td valign="top" style="font-weight: bold" width="20%">&nbsp;<img src='/imagens/transferencia-empenho.gif'>&nbsp;-&nbsp;Alterar dados empenho</td> 
					<td valign="top" style="font-weight: bold" width="20%">&nbsp;<img src='/imagens/editar_nome.gif'>&nbsp;-&nbsp;Editar Reformulação</td>
					<td valign="top" style="font-weight: bold" width="15%">&nbsp;<img src='/imagens/excluir.gif'>&nbsp;-&nbsp;Cancelar Reformulação</td>
					<td valign="top" style="font-weight: bold" width="35%">&nbsp;<img src='/imagens/atencaoVermelho.png'>&nbsp;-&nbsp;Subação em Diligência da Reprogramação, em poder do <?= $entidadeDesc ?>.</td>
				</tr>
			</table>
			</td>
		</tr>
		<tr id="tr_div">
			<td> 
			<?
			$img = "'&nbsp;'";
			
			$img = "'&nbsp;<img src=\"/imagens/icone_lupa.png\" title=\"Subacao\" onclick=\"w = window.open(\'par.php?modulo=principal/subacao&acao=A&sbaid='||s.sbaid||'&anoatual='||sd.sbdano||'\',\'Subacao\',\'scrollbars=yes,location=no,toolbar=no,menubar=no,width=800,height=600\'); w.focus();\">'";
			
			// Quando eu reformulo novamente ele se perde mostrando todas as subações reformuladas!
			$sql = "SELECT DISTINCT
			
							CASE WHEN ( s.sbareformulacao IS TRUE AND dop.dopreformulacao IS TRUE AND (sd.ssuid = 20 OR sd.ssuid = 21) ) THEN 
								'<span style=\"white-space: nowrap;\">
									<img style=\"cursor:pointer\" id=\"img_subacao_' || sd.sbdid || '\" src=\"/imagens/mais.gif\" onclick=\"carregarListaItens(this.id,\'' || sd.sbdid || '\');\" border=\"0\" />
									'||
	                                case when (select count(es.eobid) from par.empenhosubacao es where sbaid = s.sbaid and es.eobstatus = 'A') > 0 then
	                                	'&nbsp;<img style=\"cursor:pointer\" title=\"Alterar dados empenho\" src=\"/imagens/transferencia-empenho.gif\" onclick=\"alterarDadosEmpenho('||sd.sbdano||', '||sd.sbaid||', '||".$_REQUEST['prpid']."||', '||sd.ssuid||' )\" border=\"0\" />'
	                                else
	                                ''
	                                end 
	                                ||'
									&nbsp;<img style=\"cursor:pointer\" title=\"Editar Reformulação\" src=\"/imagens/editar_nome.gif\" onclick=\"janela(\'par.php?modulo=principal/subacao&acao=A&sbaid=' || sd.sbaid || '&anoatual='||sd.sbdano||'\',800,600,\'Subação\');\" border=\"0\" />
									&nbsp;<img style=\"cursor:pointer\" title=\"Cancelar Reformulação\" src=\"/imagens/excluir.gif\" onclick=\"cancelaReformulacao(\'' || sd.sbaid || '\', \'' || sd.sbdano || '\');\" border=\"0\" /></span>'
									 
							WHEN ( s.sbareformulacao IS TRUE AND dop.dopreformulacao IS TRUE AND (sd.ssuid = 23) ) THEN 		
									
								'<span style=\"white-space: nowrap;\">
									<img style=\"cursor:pointer\" id=\"img_subacao_' || sd.sbdid || '\" src=\"/imagens/mais.gif\" onclick=\"carregarListaItens(this.id,\'' || sd.sbdid || '\');\" border=\"0\" />
									<img style=\"cursor:pointer\" title=\"Subação em Diligência da Reprogramação, em poder do {$entidadeDesc}.\" id=\"img_subacao_alert' || sd.sbdid || '\" src=\"/imagens/atencaoVermelho.png\" border=\"0\" />'
								||'
									&nbsp;<img style=\"cursor:pointer\" title=\"Editar Reformulação\" src=\"/imagens/editar_nome.gif\" onclick=\"janela(\'par.php?modulo=principal/subacao&acao=A&sbaid=' || sd.sbaid || '&anoatual='||sd.sbdano||'\',800,600,\'Subação\');\" border=\"0\" />
									&nbsp;<img style=\"cursor:pointer\" title=\"Cancelar Reformulação\" src=\"/imagens/excluir.gif\" onclick=\"cancelaReformulacao(\'' || sd.sbaid || '\', \'' || sd.sbdano || '\');\" border=\"0\" />	
								</span>'
									
							ELSE '<img style=\"cursor:pointer\" id=\"img_subacao_' || sd.sbdid || '\" src=\"/imagens/mais.gif\" onclick=\"carregarListaItens(this.id,'||sd.sbdid||');\" border=\"0\" /> 
								&nbsp;<input type=\"checkbox\" name=\"sbdid[]\" id=\"sbdid_' || sd.sbdid || '\" value=\"' || sd.sbdid || '\">
								      <input type=\"hidden\" name=\"tiporeprogramacao[]\" id=\"tiporeprogramacao\" value=\"\">' || $img END  as acao,
							par.retornacodigosubacao(s.sbaid),
							s.sbadsc,
							CASE WHEN ( s.sbareformulacao IS TRUE AND dop.dopreformulacao IS TRUE AND (sd.ssuid = 21) ) THEN 
								'<center><b> Em Reprogramação - Aguardando Aprovação do FNDE</b></center>' 
							WHEN ( s.sbareformulacao IS TRUE AND dop.dopreformulacao IS TRUE AND (sd.ssuid = 20) ) THEN 
								'<center><b>Em Reprogramação – Aguardando Conclusão do {$entidadeDesc}</b></center>' 
							WHEN ( s.sbareformulacao IS TRUE AND dop.dopreformulacao IS TRUE AND (sd.ssuid = 23) ) THEN
								'<center><b>Em Diligência de Reprogramação</b></center>'
							ELSE '' END as situacao,
							sd.sbdano,
							TO_CHAR( coalesce((SELECT par.recuperavalorvalidadossubacaoporano(s.sbaid, sd.sbdano)), 0), '999G999G999D99') as vrlsubacao,
							TO_CHAR( coalesce((select sum(es.eobvalorempenho) from par.empenhosubacao es
													inner join par.empenho e on e.empid = es.empid and empstatus = 'A' and eobstatus = 'A'  
												where es.sbaid = s.sbaid and es.eobstatus = 'A'), 0), '999G999G999D99') || '</td></tr>
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
							ppc.ppcstatus = 'A' and
							ppc.prpid = ".$_REQUEST['prpid']." 
						ORDER BY
							par.retornacodigosubacao(s.sbaid), s.sbadsc";
			//ver( simec_htmlentities($sql), d);
				$cabecalho = array("&nbsp;&nbsp;Ações&nbsp;&nbsp;", "Localizador", "Descrição da Subação", "Situação da Subação", "Ano da Subação", "Valor da Subação", "Valor do Empenho" );
				$db->monta_lista($sql,$cabecalho,50000,5,'N','95%','N','formulariomontalista');
			?></td>
		</tr>
		<tr>
			<td><input type="button" onclick="javascript: reformularSubacao(1);" value="Reformular Selecionadas">&nbsp;
			<input type="button" onclick="javascript: historicoReformulacao( <?=$_REQUEST['prpid'] ?> );" value="Histórico de Reformulações"></td>
		</tr>
		<tr>
			<td class="SubTituloDireita"><center><b>Subações a serem adicionadas ao projeto</b></center></td>
		</tr>
		<tr id="tr_div">
			<td> 
			<?
			$sql = "SELECT DISTINCT
						'<center><input type=\"checkbox\" name=\"sbdidSubacao[]\" id=\"sbdidSubacao_' || sd.sbdid || '\" value=\"' || sd.sbdid || '\"></center>' as acao,
						d.dimcod || '.' || ar.arecod || '.' || i.indcod || '.' || s.sbaordem || ' - ' || s.sbadsc,
						sd.sbdano,
						TO_CHAR( (SELECT par.recuperavalorvalidadossubacaoporano(s.sbaid, sd.sbdano)), '999G999G999D99') as valor
					FROM 
						par.subacao s
					INNER JOIN par.subacaodetalhe sd ON sd.sbaid = s.sbaid
					INNER JOIN par.acao 	 a  ON a.aciid  = s.aciid AND a.acistatus IN ('A','E')
					INNER JOIN par.pontuacao p  ON p.ptoid  = a.ptoid AND p.ptostatus IN ('A','E')
					INNER JOIN par.criterio  c  ON c.crtid  = p.crtid AND c.crtstatus IN ('A','E') 
					INNER JOIN par.indicador i  ON i.indid  = c.indid AND i.indstatus = 'A'
					INNER JOIN par.area 	 ar ON ar.areid = i.areid AND ar.arestatus = 'A'
					INNER JOIN par.dimensao  d  ON d.dimid  = ar.dimid AND d.dimstatus = 'A'
					INNER JOIN par.instrumentounidade iu ON iu.inuid = p.inuid
					WHERE
						sd.sbdid NOT IN ( select sbdid from par.processoparcomposicao where ppcstatus = 'A' ) AND
						sd.ssuid IN ( 2, 12 ) AND
						s.sbastatus = 'A' AND
						sd.sbdano >= '".$arProcesso['ano']."' and
						iu.inuid = ".$_SESSION['par']['inuid'];
//dbg($sql);
				$cabecalho = array("&nbsp;Ações&nbsp;", "Descrição da Subação", "Ano da Subação", "Valor da Subação" );
				$db->monta_lista($sql,$cabecalho,50000,5,'N','95%','N','formulariomontalista2');
			?></td>
		</tr>
		<tr>
			<td><input type="button" onclick="javascript: adicionarSubacao();" value="Adicionar Subação"></td>
		</tr>
	</table>
<script type="text/javascript">

function alterarDadosEmpenho(ano, sbaid, prpid, ssuid){
	janela('par.php?modulo=principal/alterarDadosEmpenho&acao=A&ano='+ano+'&sbaid='+sbaid+'&prpid='+prpid,1000,600,'Subação');
}

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

function reformularSubacao(tiporeprogramacao){
	erro = 0;
	$('[id^=tiporeprogramacao]').val(tiporeprogramacao);
	
	$('[id^=sbdid_]').each(function(i, v) {
		if( $(v).attr('checked') ){
			erro = 1;
		}
	});
	if( erro == 0 ){
		alert('Você precisa selecionar uma subação para reformular!');
	} else {
		if( confirm( "Tem certeza que deseja reformular essa(s) subação(ões)?" ) ){
			divCarregando('','Reformulação em Andamento<br>Isso pode levar algum tempo. Aguarde...');
			$('#formulariomontalista').submit();
		}
	}
}

function adicionarSubacao(){
	erro = 0;
	$('[id^=sbdidSubacao_]').each(function(i, v) {
		if( $(v).attr('checked') ){
			erro = 1;
		}
	});
	if( erro == 0 ){
		alert('Você precisa selecionar uma subação para reformular!');
	} else {
		if( confirm( "Tem certeza que deseja adicionar essa(s) subação(ões)?" ) ){
			divCarregando();
			$('#formulariomontalista2').submit();
		}
	}
}

function cancelaReformulacao(sbaid, ano){
	if(confirm("Você tem certeza que deseja cancelar essa reformulação?\nTodas as alterações serão perdidas.")){
		$('#sbaidCancelamento').val(sbaid)
		$('#anoCancelamento').val(ano)
		$('#cancelaReformulacao').val('cancela')
		$('#formulario').submit();
	}
}

function historicoReformulacao(prpid){
	//location.href = "par.php?modulo=principal/historicoReformulacao&acao=A&prpid=" + prpid;
	location.href = "par.php?modulo=principal/projetos&acao=A&abas=historicoReformulacao&prpid=" + prpid;
}

</script>
</form>
