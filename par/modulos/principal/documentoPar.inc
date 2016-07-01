<?php
set_time_limit(30000);
ini_set("memory_limit", "3000M");

//ver($_REQUEST['req'], $_REQUEST, $_POST, d);

//function termoEx(){
if( $_REQUEST['reqGeraEx'] == 'termoEx' ){

	global $db;
	
	$sql = "SELECT 
				prpnumeroprocesso
			FROM
				par.processopar prp
			INNER JOIN par.documentopar dop ON dop.prpid = prp.prpid
			WHERE
				dopid = {$_REQUEST['dopid']} ";
	
	$prpnumeroprocesso = $db->pegaUm($sql);
	
	
	$retorno = geraTermoEx($_REQUEST['dopid'], $_REQUEST['dopdatafimvigenciaex']);
	
	if( $retorno == '1' ){
		
		if($prpnumeroprocesso)
		{
			$db->executar("UPDATE par.emailreprogramacaobloqueada SET erbstatus = 'I' where erbnumeroprocesso = '{$prpnumeroprocesso}' AND erbstatus = 'A'");
			$db->commit();
		}
		echo "
		<script>
			alert('Operação realizada com sucesso!');
			window.location.href = window.location;
		</script>";
	} else {
		echo "
		<script>
			alert('Tipo EX não implementado!');
			window.location.href = window.location;
		</script>";
	}
	
	exit();
}

function regerarTermo(){
	
    global $db;
    try {
        
	$sql = "SELECT
                    iueid, prpid, dopstatus, dopdiasvigencia, dopdatainicio, dopdatafim, dopacompanhamento,
                    dp.mdoid as modelo, dopdatainclusao, usucpfinclusao, dopdataalteracao, usucpfalteracao, dopjustificativa,
                    dopdatavalidacaofnde, dopusucpfvalidacaofnde, dopdatavalidacaogestor, dopusucpfvalidacaogestor,
                    dopusucpfstatus, dopdatastatus, dopdatapublicacao, doppaginadou, dopnumportaria, proid,
                    dopreformulacao, dopidpai, tpdcod, dopdatainiciovigencia, '".$_REQUEST['dopdatafimvigencia']."' as dopdatafimvigencia
                FROM
                    par.documentopar dp
                INNER JOIN par.modelosdocumentos md ON md.mdoid = dp.mdoid
                WHERE dp.dopid = {$_REQUEST['dopid']}";

	$arrDadosDoc = $db->pegaLinha( $sql );
	$arrDadosDoc = $arrDadosDoc ? $arrDadosDoc : array();
        
        if ($arrDadosDoc['dopstatus'] != 'A') {
            unset($_REQUEST);
            echo "<script>window.location = 'par.php?modulo=principal/documentoPar&acao=A'</script>";
            exit;
        }

	if( $arrDadosDoc['prpid'] ){
		$processo = $db->pegaUm("select prpnumeroprocesso from par.processopar where prpid = {$arrDadosDoc['prpid']}");
		verificaEmpenhoSigef( $processo );
	}

	/* Modificado dia 12/05/2014
	 * A pedido de: Thiago
	* Programador: Eduardo Dunice Neto
	* */
	$arrDadosDoc['dopdatainiciovigencia'] = pegaDataInicioDocumentoPar( $_REQUEST['dopid'] );
	
	/* Modificado dia 13/01/2014
	 * A pedido de: Isabel
	* Programador: Eduardo Dunice Neto
	* */
	$tipoDocumento = $arrDadosDoc['modelo'];

	$sql = "SELECT mdoconteudo, tpdcod FROM par.modelosdocumentos WHERE mdostatus = 'A' AND mdoid = ".$tipoDocumento;
	$arrModelo = $db->pegaLinha($sql);

	$mdoconteudo = $arrModelo['mdoconteudo'];
	$tpdcod = $arrModelo['tpdcod'];

	$sql = "SELECT sbdid as chk FROM par.termocomposicao WHERE dopid = ".$_REQUEST['dopid'];
	$subacoes = $db->carregarColuna($sql);
	
	$post = array('mdoid' => $arrDadosDoc['modelo'], 'dopid' => $_REQUEST['dopid'], 'tpdcod' => $tpdcod, 'chk' => $subacoes);

	$_SESSION['par']['cronogramainicial'] 	= $arrDadosDoc['dopdatainiciovigencia'];
	$_SESSION['par']['cronogramaFinal'] 	= $arrDadosDoc['dopdatafimvigencia'];

	$doptexto = alteraMacrosMinuta($mdoconteudo, $arrDadosDoc['prpid'], $post);

	$arrDadosDoc['mdoid'] = $tipoDocumento;
        
        if($_SESSION['par']['totalVLR']){
            $arrDadosDoc['dopvalor'] = $_SESSION['par']['totalVLR'];
            unset($_SESSION['par']['totalVLR']);
        }

	$dopidNovo = salvarDadosMinuta($arrDadosDoc, $doptexto, $_REQUEST['dopid']);
	
	$sql = "INSERT INTO par.termocomposicao(sbdid, dopid) 
			SELECT sbdid, $dopidNovo 
			FROM par.termocomposicao
			WHERE dopid = ".$_REQUEST['dopid'];
	$db->executar($sql);
	$db->commit();
	echo "<script>window.location.href = window.location.href;</script>";
    } catch (Exception $exc) {
        $db->rollback();
        echo $exc->getTraceAsString();
    }
}

function formGeraEx(){
	
	global $db;
	
	extract($_REQUEST);
	
	?>
	<center><b>Deseja gerar este Termo EX?</b></center>
	<form method="post" name="formEx" id="formEx">
		<input type="hidden" name="reqGeraEx"	id="reqGeraEx"  value="" />
		<input type="hidden" name="dopid" 		id="dopidex"	value="<?=$dopidEx ?>"/>
		Fim da Vigência: <?=campo_texto( 'dopdatafimvigenciaex', 'N', 'S', '', 10, 7, '##/####', '');  ?> (mm/aaaa)
	</form>
<?php 
}

function formRegeraTermo(){
	global $db;
	
	extract($_REQUEST);
	
	$sql = "SELECT 
    			dopdatainiciovigencia 
			FROM par.documentopar dp 
            WHERE 
            	prpid = ( select prpid from par.documentopar where dopid = {$dopidRegera} ) 
                AND dopstatus <> 'E'
			ORDER BY dp.dopid asc LIMIT 1";
	
	$datas = $db->pegaLinha( $sql );
	
	/* Verifica se, em caso de reformulação, está aguardando geração de termo Demanda PAR 348 */
	if( $dopidRegera != '' ){
	
		$sql = "SELECT DISTINCT
					TRUE
				FROM
					par.processopar prp
				INNER JOIN par.documentopar 					dop ON dop.prpid = prp.prpid
				INNER JOIN par.modelosdocumentos  				mdo ON mdo.mdoid = dop.mdoid
				INNER JOIN par.documentoparreprogramacaosubacao	dpr ON dpr.dopid = dop.dopid
				WHERE
					dopreformulacao IS TRUE
					AND dpr.dpsstatus = 'A'
					AND dopstatus = 'A'
					AND dop.dopid = $dopidRegera";
	
		$emReformulacao = $db->pegaUm($sql);
	
		$sql = "SELECT TRUE FROM par.documentoparreprogramacao WHERE dopid = $dopidRegera AND dprstatus = 'P'  AND dprvalidacao NOT IN ('f')";
	
		$emReformulacaoPrazo = $db->pegaUm($sql);
	}
	
	if( $emReformulacao == 't' || $emReformulacaoPrazo == 't' ){
	
	?>
		<center>
			<img src="../imagens/par/carimbo-reformulacao.png" border="0">
		</center>
		<input type="hidden" id="refREG" value="TRUE"/>
<?php 
	}else{
?>
	<center><b>Deseja regerar este termo?</b></center>
	<form method="post" name="formRegerar" id="formRegerar">
		<input type="hidden" 						id="refREG"			value="FALSE"/>
		<input type="hidden" name="req" 			id="reqRegera"  	/>
		<input type="hidden" name="dopid" 			id="dopidRegera"	value="<?=$dopidRegera ?>"/>
		<input type="hidden" name="datainicial" 	id="datainicial" 	value="<?=$datas['dopdatainiciovigencia'] ?>"	/>
		Inicio da Vigência: <?php echo $datas['dopdatainiciovigencia'] ?><br>
		Fim da Vigência: <?php echo campo_texto( 'dopdatafimvigencia', 'N', 'S', '', 10, 7, '##/####', '');  ?> (mm/aaaa)
	</form>
<?php 
	}
}

function formVizualizaDocumento(){

	global $db;

	$sql = "select * from (
				SELECT dpv.dpvcpf as cpfgestor,
							to_char(dpvdatavalidacao, 'DD/MM/YYYY HH24:MI:SS') as data,
							us.usunome, us.usucpf, d.tpdcod, itrid, dopstatus, dp.dopid, d.mdoqtdvalidacao, ( SELECT count(dpv2.dpvid) FROM par.documentoparvalidacao dpv2 WHERE dpv2.dopid = dp.dopid and dpv2.dpvstatus = 'A') as qtdvalidado
						FROM par.documentopar  dp
						INNER JOIN par.modelosdocumentos   d ON d.mdoid = dp.mdoid
						INNER JOIN par.processopar pp ON pp.prpid = dp.prpid
						INNER JOIN par.instrumentounidade iu ON iu.inuid = pp.inuid
						LEFT JOIN par.documentoparvalidacao dpv ON dpv.dopid = dp.dopid and dpv.dpvstatus = 'A'
						left join seguranca.usuario us on us.usucpf = dpv.dpvcpf
				union
				SELECT dpv.dpvcpf as cpfgestor,
							to_char(dpvdatavalidacao, 'DD/MM/YYYY HH24:MI:SS') as data,
							us.usunome, us.usucpf, d.tpdcod, itrid, dopstatus, dp.dopid, d.mdoqtdvalidacao, ( SELECT count(dpv2.dpvid) FROM par.documentoparvalidacao dpv2 WHERE dpv2.dopid = dp.dopid and dpv2.dpvstatus = 'A') as qtdvalidado
						FROM par.documentopar  dp
						INNER JOIN par.modelosdocumentos   d ON d.mdoid = dp.mdoid
						INNER JOIN par.processoobraspar pp ON pp.proid = dp.proid and pp.prostatus = 'A'
						INNER JOIN par.instrumentounidade iu ON iu.inuid = pp.inuid
						LEFT JOIN par.documentoparvalidacao dpv ON dpv.dopid = dp.dopid and dpv.dpvstatus = 'A'
						left join seguranca.usuario us on us.usucpf = dpv.dpvcpf
				) as foo
			 WHERE 
				dopid = ".$_REQUEST['dopid'];

	$html = $db->pegaLinha($sql);

	$sql = "SELECT
				dpv.dpvcpf as cpfgestor,
				to_char(dpvdatavalidacao, 'DD/MM/YYYY HH24:MI:SS') as data,
				us.usunome,
				us.usucpf
			FROM par.documentopar  dp
			INNER JOIN par.documentoparvalidacao dpv ON dpv.dopid = dp.dopid
			INNER JOIN seguranca.usuario us ON us.usucpf = dpv.dpvcpf
			WHERE
				dpv.dpvstatus = 'A' and
				dp.dopid = ".$_REQUEST['dopid'];

	$dadosValidacao = $db->carregar($sql);

	$textoValidacao = "";

	if(is_array($dadosValidacao)){
		foreach( $dadosValidacao as $dv ){
			$cpfvalidacao[] = $dv['cpfgestor'];
			$textoValidacao .= "<b>Validado por ".$dv['usunome']." - CPF: ".formatar_cpf($dv['usucpf'])." em ".$dv['data']." </b><br>";
		}
	}

	$cpfvalidacao = $cpfvalidacao ? $cpfvalidacao : array();
	$perfil = pegaArrayPerfil($_SESSION['usucpf']);
	
	$doptexto = pegaTermoCompromissoArquivo( $_REQUEST['dopid'], '');
?>

    <div style="background: #FFF; white-space: nowrap">
        <table id="termo" width="95%" align="center" border="0" cellpadding="3" cellspacing="1" style="background-color:white;">
            <tr>
                <td style="font-size: 12px; font-family:arial;">
                    <div id="divImpressao">
                        <?php echo ($html['tpdcod'] == '101') ? monta_cabecalho_relatorio_par('100') : monta_cabecalho_relatorio_par('100'); ?>
                        <?php echo simec_html_entity_decode($doptexto);?>
                    </div>
                </td>
            </tr>
        </table>
        <table id="termo" align="center" border="0" cellpadding="3" cellspacing="1" style="background-color:white;">
            <tr <?php echo $displayValidado; ?> style="text-align: center;">
                <td><b>VALIDAÇÃO ELETRÔNICA DO DOCUMENTO<b><br><br>
                            <?php echo $textoValidacao; ?>
                </td>
            </tr>
        </table>
        <script type="text/javascript" src="../par/js/jquery-1.11.1.min.js"></script>
        <script>jQuery('div#divimpressao p:nth-child(3)').css('page-break-before','auto');</script>
    </div>
<?php 	
}

function monta_cabecalho_relatorio_par($largura){
    $strCabecalho = "<table width='{$largura}%' border='0' cellpadding='0' cellspacing='0'>
                        <tr bgcolor='#FFF'>
                            <td valign='top' align='center'>
                                <img src='../imagens/brasao.gif' width='45' height='45' border='0'>
                                <br><b>MINISTÉRIO DA EDUCAÇÃO<br/>
                                FUNDO NACIONAL DE DESENVOLVIMENTO DA EDUCAÇÃO</b> <br />
                            </td>
                        </tr>
                    </table>";

    return $strCabecalho;
}


if ($_POST['req'] == 'verificaPendenciaSubacaoEscola'){
    $strHtml = verificaPendenciaSubacaoEscola($_POST['dopid']);
    echo $strHtml;
    die();
}

if( $_REQUEST['req'] ){
	if($_REQUEST['req'] != "inseriranexotermoprocesso")
	{
		$_REQUEST['req']();
		die();
	}
}

if ($_POST['tipodocumento']) {
    $obModelosDocumentosControle = new ModelosDocumentosControle();
    $sqlModelosDoc = $obModelosDocumentosControle->carregarModeloDocumento($_POST['tipodocumento'], TRUE, 'NOT');
    echo $db->monta_combo("mdoid", $sqlModelosDoc, 'S', 'Selecione', 'verificaMoceloDoc', '', '', 400, 'N', 'mdoid', '', $_POST['mdoid']);
    exit();
}

if($_REQUEST['listadocumento']){
	header('content-type: text/html; charset=ISO-8859-1');
	/* Regra atualizada dia 29/05/2014
	 * Analista: Thiago
	* Programador: Eduardo
	* Documento validado deve apenas ser vizualizado.
	* */
	$acoes = "CASE WHEN dpvcpf IS NOT NULL 
					THEN '<img src=../imagens/icone_lupa.png style=cursor:pointer;  class=vizualizarDocumento id='|| mi.dopid ||' >' 
					ELSE 
						CASE WHEN prpfinalizado IS NULL THEN 
							'<img src=\"../imagens/alterar.gif\" style=\"cursor:pointer;\" onclick=\"carregaTermoMinuta('||mi.dopid||','|| mi.prpid ||');\" border=\"0\">'
						ELSE '<img src=../imagens/icone_lupa.png style=cursor:pointer;  class=vizualizarDocumento id='|| mi.dopid ||' >' END 
				END";

	$perfil = pegaArrayPerfil($_SESSION['usucpf']);
	$exluirSuper = "''";
	$AcExluir = "''";
	$ExluirInativo = "<img src=\"../imagens/excluir_01.gif\" style=\"cursor:pointer;\" border=\"0\">";
        $acaoRegerar = "";
	if( in_array( PAR_PERFIL_SUPER_USUARIO, $perfil ) ||
		in_array( PAR_PERFIL_ADMINISTRADOR, $perfil ) ||
		in_array( PAR_PERFIL_GERADOR_DOCUMENTO, $perfil )
	){
		$AcExluir = retornarSqlImgExcluirColunaAcoes();
                $acaoRegerar = "<img src=../imagens/refresh.gif style=cursor:pointer; id=' || mi.dopid || ' class=regerarTermo title=Regerar>";
        
		if( in_array( PAR_PERFIL_SUPER_USUARIO, $perfil ) || in_array( PAR_PERFIL_ADMINISTRADOR, $perfil )){
			$exluirSuper = retornarSqlImgExcluirColunaAcoes();
			$AcExluir = "''";
			$ExluirInativo = '';
		}else{
			$exluirSuper = "''";
		}
	}
	
	$sql = "SELECT prpfinalizado FROM par.processopar WHERE prpid = ".$_REQUEST['prpid'];
	$finalizado = $db->pegaUm($sql);

	$imagemReprogramacaoSubacao = "''";
	$imagemMais = "<img style=\"cursor:pointer\" id=\"img_dimensao_' || mi.dopid || '\" src=\"/imagens/mais.gif\" 
						onclick=\"carregarListaDocumentoParReformulado(this.id,\'' || mi.dopid || '\');\" border=\"0\" />";
	$imagemRef  = "<img src=../imagens/restricao_ico.png style=cursor:pointer; title=\"Termo Reformulado\">";
	if(in_array( PAR_PERFIL_SUPER_USUARIO, $perfil ) ||	in_array( PAR_PERFIL_ADMINISTRADOR, $perfil )){
		$imagemReprogramacaoSubacao  = "CASE WHEN ( SELECT count(dpsid) 
													FROM par.documentoparreprogramacaosubacao dps WHERE dps.dopid = mi.dopid AND dps.dpsstatus = 'P' AND dpsvalidacao NOT IN ('f')) > 0 
											THEN '<img  src=\"../imagens/pendente.gif\" title=\"Validar Reprogramação\" style=\"cursor:pointer;\" 
														onclick=\"validaReprogramacao('||mi.dopid||');\" border=\"0\">' 
											ELSE '' 
										END";
	}
	
	$botaoGerarTermoEX = "''";
	if( in_array( PAR_PERFIL_SUPER_USUARIO, $perfil ) ||
		in_array( PAR_PERFIL_ADMINISTRADOR, $perfil ) 
		){
		$dadosDocumentoEX = botaoGerarTermoEX( $_REQUEST['prpid'] );
	
		if( $dadosDocumentoEX['mdoid_ex'] ){
			$botaoGerarTermoEX = "'<img style=\"cursor:pointer\" id=\"' || mi.dopid || '\" src=\"/imagens/ex-oficio.png\" class=termoEx title=\"Gerar termo Ex Ofício\" />'";
		} 
	
	}

	$booCaseReprogramacao_AguardaEmpenho= "
		( 	
		SELECT DISTINCT
			TRUE
		FROM 
			par.processopar prp
		
		INNER JOIN par.processoparcomposicao 		ppc ON ppc.prpid = prp.prpid and ppc.ppcstatus = 'A'
		INNER JOIN par.subacaodetalhe 			sd  ON sd.sbdid = ppc.sbdid
		INNER JOIN par.subacao 				s   ON s.sbaid = sd.sbaid
		
		INNER JOIN par.documentopar 			dop ON dop.prpid = prp.prpid
		INNER JOIN par.modelosdocumentos  		mdo ON mdo.mdoid = dop.mdoid
		INNER JOIN par.documentoparreprogramacaosubacao	dpr ON dpr.dopid = dop.dopid
		WHERE
			dop.dopid = mi.dopid
			AND (
				(
				SELECT DISTINCT
					(
						(par.recuperavalorvalidadossubacaoporano(sbd.sbaid, sbd.sbdano)-(sbdrepassevlrcomplementaraprovado+sbdrepassevlrrafaprovado))
						-
						sum(vve.vrlempenhocancelado)
					) > 0
				FROM
					par.empenhosubacao ems
				INNER join par.empenho emp on emp.empid = ems.empid and empcodigoespecie not in ('03', '13', '02', '04') and eobstatus = 'A' and empstatus = 'A'
				inner join par.v_vrlempenhocancelado vve on vve.empid = emp.empid
				inner join par.processopar prp ON prp.prpnumeroprocesso = emp.empnumeroprocesso
				left join par.documentopar dop ON dop.prpid = prp.prpid AND dop.dopstatus = 'A'
				left join par.subacaodetalhe sbd  on ems.sbaid = sbd.sbaid and sbdano = sd.sbdano
				left join (
					select ep.empnumeroprocesso, ep.empidpai, sum(ep.empvalorempenho) as vrlreforco, ep.empcodigoespecie, sd1.sbdid 
					from par.empenho ep
						inner join par.empenhosubacao es on es.empid = ep.empid and empstatus = 'A' and eobstatus = 'A'
						inner join par.subacaodetalhe sd1 on sd1.sbaid = es.sbaid and sd1.sbdano = es.eobano
					where ep.empcodigoespecie in ('02')
					group by 
						ep.empnumeroprocesso,
						ep.empcodigoespecie,
						ep.empidpai,
						sd1.sbdid
				) as emr on emr.empidpai = emp.empid and emr.sbdid = sbd.sbdid
				where
					ems.sbaid = s.sbaid  AND ems.eobano = sd.sbdano AND ems.eobstatus = 'A'
				GROUP BY
					sbd.sbaid, sbd.sbdano, sbd.sbdrepassevlrcomplementaraprovado, sbd.sbdrepassevlrrafaprovado,emr.vrlreforco
				)			
			) 
			AND dopreformulacao IS TRUE 
			AND (	SELECT count(sd2.sbdid) FROM par.processoparcomposicao ppc 
				LEFT JOIN par.subacaodetalhe sd2 ON ppc.sbdid = sd2.sbdid AND sd2.ssuid IN (20, 21, 23) 
				WHERE ppc.ppcstatus = 'A' and dop.prpid = ppc.prpid ) = 0 
			AND dpr.dpsstatus = 'A' 
			AND dopstatus = 'A'
		)";
	
	$booCaseReprogramacao_GerarTermo = "
		(
		SELECT DISTINCT
			TRUE
		FROM 
			par.processopar prp
		
		INNER JOIN par.processoparcomposicao 		ppc ON ppc.prpid = prp.prpid and ppc.ppcstatus = 'A'
		INNER JOIN par.subacaodetalhe 			sd  ON sd.sbdid = ppc.sbdid
		INNER JOIN par.subacao 				s   ON s.sbaid = sd.sbaid
		
		INNER JOIN par.documentopar 			dop ON dop.prpid = prp.prpid
		INNER JOIN par.modelosdocumentos  		mdo ON mdo.mdoid = dop.mdoid
		INNER JOIN par.documentoparreprogramacaosubacao dpr ON dpr.dopid = dop.dopid
		WHERE
			dop.dopid = mi.dopid
			AND (
				(
				SELECT DISTINCT
					(
						(par.recuperavalorvalidadossubacaoporano(sbd.sbaid, sbd.sbdano)-(sbdrepassevlrcomplementaraprovado+sbdrepassevlrrafaprovado))
						-
						sum( vve.vrlempenhocancelado )
					) <= 0
				FROM
					par.empenhosubacao ems
				INNER join par.empenho emp on emp.empid = ems.empid and empcodigoespecie not in ('03', '13', '02', '04') and eobstatus = 'A' and empstatus = 'A'
				inner join par.v_vrlempenhocancelado vve on vve.empid = emp.empid
				inner join par.processopar prp ON prp.prpnumeroprocesso = emp.empnumeroprocesso
				left join par.documentopar dop ON dop.prpid = prp.prpid AND dop.dopstatus = 'A'
				left join par.subacaodetalhe sbd  on ems.sbaid = sbd.sbaid and sbdano = sd.sbdano
				left join (
					select ep.empnumeroprocesso, ep.empidpai, sum(ep.empvalorempenho) as vrlreforco, ep.empcodigoespecie, sd1.sbdid 
					from par.empenho ep
						inner join par.empenhosubacao es on es.empid = ep.empid and empstatus = 'A' and eobstatus = 'A'
						inner join par.subacaodetalhe sd1 on sd1.sbaid = es.sbaid and sd1.sbdano = es.eobano
					where ep.empcodigoespecie in ('02')
					group by 
						ep.empnumeroprocesso,
						ep.empcodigoespecie,
						ep.empidpai,
						sd1.sbdid
				) as emr on emr.empidpai = emp.empid and emr.sbdid = sbd.sbdid
				where
					ems.sbaid = s.sbaid  AND ems.eobano = sd.sbdano AND ems.eobstatus = 'A'
				GROUP BY
					sbd.sbaid, sbd.sbdano, sbd.sbdrepassevlrcomplementaraprovado, sbd.sbdrepassevlrrafaprovado,emr.vrlreforco
				)
			) 
			AND dopreformulacao IS TRUE 
			AND (	SELECT count(sd2.sbdid) FROM par.processoparcomposicao ppc 
				LEFT JOIN par.subacaodetalhe sd2 ON ppc.sbdid = sd2.sbdid AND sd2.ssuid IN (20, 21, 23) 
				WHERE ppc.ppcstatus = 'A' and dop.prpid = ppc.prpid) = 0 
			AND dpr.dpsstatus = 'A' 
			AND dopstatus = 'A' 	
		)";

	if( $finalizado == true ){
		$imagemMais = "";
		$imagemRef = "";
		$exluirSuper = "''";
		$ExluirInativo = "";
		$imagemReprogramacaoSubacao = "''";
		$botaoGerarTermoEX = "''";
		$AcExluir = "''";
		$acaoRegerar = "";
	}
	
	$sql = "SELECT 
				acao, 
				documentos, 
				vigencia, 
				validado||' '||situacao as situacao,
				dopdataalteracao, 
				CASE WHEN mdodocumentoex = 't' THEN 
		    		'Adminstrador de sistema'
		    	ELSE
		            CASE WHEN usualtnome is not null THEN
						usualtnome
					ELSE
						''
					END
				END
		           
					||'</td></tr><tr style=display:none id=listaDocumentos2_' || dopid || ' ><td id=trV2_' || dopid || ' colspan=8 ></td></tr>'
			FROM
			(
		        SELECT	DISTINCT
		        	'<center>'
					||
					CASE WHEN mi.dopidpai IS NOT NULL
					THEN
						CASE WHEN 
									mo.mdoqtdvalidacao = (SELECT COUNT(dv.dpvcpf) FROM par.documentoparvalidacao dv WHERE dv.dopid = mi.dopid AND dv.dpvstatus = 'A') OR
									(
									SELECT DISTINCT TRUE
									FROM par.documentopar dop1
									INNER JOIN par.termocomposicao 	tec1 ON tec1.dopid = dop1.dopid
									INNER JOIN par.subacaodetalhe 	sbd1 ON sbd1.sbdid = tec1.sbdid 
									INNER JOIN par.empenhosubacao 	ems1 ON ems1.sbaid = sbd1.sbaid AND ems1.eobano = sbd1.sbdano AND ems1.eobstatus = 'A'
									INNER JOIN par.pagamento 		pag1 ON pag1.empid = ems1.empid AND pag1.pagstatus = 'A' AND pag1.pagsituacaopagamento not ilike '%CANCELADO%'
									WHERE dop1.dopid = mi.dopid AND dop1.dopstatus = 'A'
									)
							THEN '$imagemMais '||'$imagemRef '|| $exluirSuper ||'$ExluirInativo'||$acoes||$imagemReprogramacaoSubacao||$botaoGerarTermoEX
							ELSE '$imagemMais '||'$imagemRef '|| $exluirSuper || $AcExluir ||$acoes||$imagemReprogramacaoSubacao||$botaoGerarTermoEX
						END
					ELSE
						CASE WHEN 
									mo.mdoqtdvalidacao = (SELECT COUNT(dv.dpvcpf) FROM par.documentoparvalidacao dv WHERE dv.dopid = mi.dopid AND dv.dpvstatus = 'A') OR
									(
									SELECT DISTINCT TRUE
									FROM par.documentopar dop1
									INNER JOIN par.termocomposicao 	tec1 ON tec1.dopid = dop1.dopid
									INNER JOIN par.subacaodetalhe 	sbd1 ON sbd1.sbdid = tec1.sbdid 
									INNER JOIN par.empenhosubacao 	ems1 ON ems1.sbaid = sbd1.sbaid AND ems1.eobano = sbd1.sbdano AND ems1.eobstatus = 'A'
									INNER JOIN par.pagamento 		pag1 ON pag1.empid = ems1.empid AND pag1.pagstatus = 'A' AND pag1.pagsituacaopagamento not ilike '%CANCELADO%'
									WHERE dop1.dopid = mi.dopid AND dop1.dopstatus = 'A'
									) 
							THEN $exluirSuper ||'$ExluirInativo'||$acoes||$imagemReprogramacaoSubacao||$botaoGerarTermoEX
							ELSE $exluirSuper || $AcExluir ||$acoes||$imagemReprogramacaoSubacao||$botaoGerarTermoEX
						END
					 END
		        	||
					CASE WHEN (dps.dpsid IS NOT NULL) OR (re2.dopidreprogramado IS NOT NULL) THEN '' ELSE '$acaoRegerar' END
					||
					'</center>' as acao,
			   	 	mo.mdonome ||' - '|| mi.dopnumerodocumento as documentos,
				    CASE WHEN dps.dpsid IS NOT NULL 
				    	THEN
					    	CASE WHEN $booCaseReprogramacao_AguardaEmpenho
					    		THEN '<b><br>Em Reprogramação</b> - <label style=color:red ><b>Aguardando Empenho</b></label>'
					    		WHEN $booCaseReprogramacao_GerarTermo
					    		THEN '<b><br>Em Reprogramação</b> - 
					    				<input  type=\"button\" value=\"Gerar Documento\" 
					    						onclick=\"gerarTermoReprogramacao('||mi.dopid||','||mi.prpid||')\" id='||mi.dopid||' >&nbsp;'
					    		ELSE '<b><br>Em Reprogramação</b>' 
					    	END
				    	ELSE '' 
				    END ||
	               	CASE WHEN re2.repid IS NOT NULL AND re2.repdtfim IS NULL
	               		THEN '<b><br>Em Reprogramação</b> - 
	               				<input type=\"button\" value=\"Finalizar reprogramação\" onclick=\"finalizaReprogramacao('||mi.dopid||','||mi.prpid||')\" id='||mi.dopid||' >&nbsp;'
	               		ELSE ''
	               	END ||
				    CASE 
				    	WHEN mi.dopid IN ( SELECT dopid FROM par.documentoparreprogramacao WHERE dprstatus = 'P' AND dprdataprazoaprovado IS NULL AND dprvalidacao NOT IN ('f') ) 
				    		THEN '<b><br>Em Validação de Reprogramação</b> - 
				    				<input type=\"button\" value=\"Validar Reprogramação\" id=\"validaReprogramacao\" onclick=\"validarReprogramacao(\'' || mi.dopid || '\')\">' 
				    	WHEN mi.dopid IN ( SELECT dopid FROM par.documentoparreprogramacao WHERE dprstatus = 'P' AND dprdataprazoaprovado IS NOT NULL  AND dprvalidacao NOT IN ('f')) 
				    		THEN '<center><input type=button id='|| mi.dopid ||' prpid='|| mi.prpid ||' class=gerarReprogramacaoPrazo value=\"Gerar Termo\" ></center>' 
				    	ELSE '' 
				    END as situacao,
				    trim(mi.dopdatafimvigencia) as vigencia,
					CASE WHEN mo.mdoqtdvalidacao = (select count(dv.dpvcpf) FROM par.documentoparvalidacao dv WHERE dv.dopid = mi.dopid AND dv.dpvstatus = 'A') 
						THEN 'Documento Validado'
	               	 	ELSE 'Documento não Validado' 
	               	END as validado,
	                mi.dopid,
	               	usu2.usunome as usualtnome,
	                TO_CHAR( mi.dopdataalteracao , 'DD/MM/YYYY') as dopdataalteracao,
	                mo.mdodocumentoex,
	                prp.prpfinalizado
				FROM 
					par.documentopar mi
				INNER JOIN par.processoparcomposicao 			ppc ON ppc.prpid = mi.prpid and ppc.ppcstatus = 'A'
				INNER JOIN par.processopar						prp ON ppc.prpid = prp.prpid
				INNER JOIN par.subacaodetalhe 					sd  ON sd.sbdid = ppc.sbdid
				INNER JOIN par.subacao 							s   ON s.sbaid = sd.sbaid
				LEFT  JOIN par.documentoparvalidacao 			dpv ON dpv.dopid = mi.dopid AND dpvstatus = 'A'
			    LEFT  JOIN par.modelosdocumentos 				mo  ON mo.mdoid = mi.mdoid
			    INNER JOIN seguranca.usuario 					us  ON us.usucpf = mi.usucpfinclusao
			    LEFT  JOIN seguranca.usuario 					usu2 ON usu2.usucpf = mi.usucpfalteracao
			    LEFT  JOIN par.documentoparreprogramacaosubacao	dps ON dps.dopid = mi.dopid AND dps.dpsstatus = 'A'
			    LEFT  JOIN par.reprogramacao					re2 ON re2.dopidreprogramado = mi.dopid AND re2.repstatus = 'A'
				WHERE 
					mi.prpid = ".$_REQUEST['prpid']." 
					AND mi.dopstatus IN ('A')
			) as foo";
// 	ver(simec_htmlentities($sql),d);
	$cabecalho = array("Acões", "Documento", "Vigência", "Situação", "Data de alteração", "Alterado Por");
	$db->monta_lista($sql, $cabecalho, 20, 4, 'N','Center');

	exit;
}

/**
 * Cria sql referente ao botao da coluna excluir
 * 
 * @return string sql referente ao botao da coluna excluir
 */
function retornarSqlImgExcluirColunaAcoes(){
    $excluir = "
        -- Regra: Só permite excluir caso não esteja na situação validado
        CASE WHEN 
					dpvcpf IS NOT NULL OR dps.dopid IS NOT NULL  -- está em reformulacao 
					OR
					(
					SELECT DISTINCT TRUE
					FROM par.documentopar dop1
					INNER JOIN par.termocomposicao 	tec1 ON tec1.dopid = dop1.dopid
					INNER JOIN par.subacaodetalhe 	sbd1 ON sbd1.sbdid = tec1.sbdid 
					INNER JOIN par.empenhosubacao 	ems1 ON ems1.sbaid = sbd1.sbaid AND ems1.eobano = sbd1.sbdano AND ems1.eobstatus = 'A'
					INNER JOIN par.pagamento 		pag1 ON pag1.empid = ems1.empid AND pag1.pagstatus = 'A' AND pag1.pagsituacaopagamento not ilike '%CANCELADO%'
					WHERE dop1.dopid = mi.dopid AND dop1.dopstatus = 'A'
					)
			THEN 
	            ''
	        ELSE
	            '<img src=\"../imagens/excluir.gif\" style=\"cursor:pointer;\" onclick=\"excluiTermoMinuta('||mi.dopid||','|| mi.prpid ||');\" border=\"0\">'
        END
    ";

    return $excluir;
}

if($_REQUEST['listadocumentoReformulado']){
	
	header('content-type: text/html; charset=ISO-8859-1');
	$acoes = "'<img src=../imagens/icone_lupa.png style=cursor:pointer;  class=vizualizarDocumento id='|| mi.dopid ||' >' ";
	
	$perfil = pegaArrayPerfil($_SESSION['usucpf']);
	$exluirSuper = "''";
	$AcExluir = "''";
	$ExluirInativo = "<img src=\"../imagens/excluir_01.gif\" style=\"cursor:pointer;\" border=\"0\">";
	if( in_array( PAR_PERFIL_SUPER_USUARIO, $perfil ) ||
		in_array( PAR_PERFIL_ADMINISTRADOR, $perfil ) ||
		in_array( PAR_PERFIL_GERADOR_DOCUMENTO, $perfil )
	){
		$AcExluir = "<img src=\"../imagens/excluir.gif\" style=\"cursor:pointer;\" onclick=\"excluiTermoMinuta('||mi.dopid||','|| mi.prpid ||');\" border=\"0\">";
		if( in_array( PAR_PERFIL_SUPER_USUARIO, $perfil ) || in_array( PAR_PERFIL_ADMINISTRADOR, $perfil ) ){
			$exluirSuper = "<img src=\"../imagens/excluir.gif\" style=\"cursor:pointer;\" onclick=\"excluiTermoMinuta('||mi.dopid||','|| mi.prpid ||');\" border=\"0\">";
			$AcExluir = '';
			$ExluirInativo = '';
		}
	}

	$imagemMais = "'<img style=\"cursor:pointer\" id=\"img_dimensao_' || mi.dopid || '\" src=\"/imagens/mais.gif\" onclick=\"carregarListaDocumentoParReformulado(this.id,\'' || mi.dopid || '\');\" border=\"0\" />'";

	$sql = "SELECT DISTINCT
				CASE WHEN mi.dopidpai IS NOT NULL
				THEN
					case when dv.dpvcpf is not null then '<center>'||$imagemMais||'&nbsp;'||$acoes||'</center>'
						else '<center>'||$imagemMais||'&nbsp;'||$acoes||'</center>'
					 END
				ELSE
					case when dv.dpvcpf is not null then '<center>'||$acoes||'</center>'
						else '<center>'||$acoes||'</center>'
					 END
				END as acao,
			    mo.mdonome ||' - '|| mi.dopnumerodocumento,
			    trim(mi.dopdatafimvigencia) as vigencia,
			    --case when (dopreformulacao = true AND (SELECT count(sd2.sbdid) FROM par.processoparcomposicao ppc LEFT JOIN par.subacaodetalhe sd2 ON ppc.sbdid = sd2.sbdid AND sd2.ssuid IN (20,21) WHERE ppc.ppcstatus = 'A' and mi.prpid = ppc.prpid) = 0 ) then '<b>Em Reformulação</b> - <input type=\"button\" value=\"Finalizar reformulação\" id=\"finalizar\" onclick=\"finaliza(\'' || mi.dopid || '\')\">' else '' end 
			    '' as situacao,
		    	TO_CHAR( mi.dopdataalteracao , 'DD/MM/YYYY') as dopdataalteracao ,
		    	
		    	CASE WHEN mo.mdodocumentoex = 't' THEN 
		    		'Adminstrador de sistema'
		    	ELSE
			    	CASE WHEN usu2.usunome is not null THEN
						usu2.usunome
					ELSE
						''
					END
				END
		             ||
					'</td></tr>
		            	<tr style=\"display:none\" id=\"listaDocumentos2_' || mi.dopid || '\" >
		            		<td id=\"trV2_' || mi.dopid || '\" colspan=8 ></td>
		            </tr>'
		        as usucpfalteracao
		        
			FROM 
				par.documentopar mi
		    LEFT  JOIN par.modelosdocumentos 		mo ON mo.mdoid = mi.mdoid
		    INNER JOIN seguranca.usuario 			us ON us.usucpf = mi.usucpfinclusao
		    LEFT  JOIN par.documentoparvalidacao 	dv ON dv.dopid = mi.dopid AND dv.dpvstatus = 'A'
		    LEFT JOIN seguranca.usuario 			usu2 ON usu2.usucpf = mi.usucpfalteracao
		    
			WHERE 
				(mi.dopid IN ( SELECT dopidpai FROM par.documentopar mi2 WHERE mi2.dopid = ".$_REQUEST['dopid']." )
				OR mi.dopid IN ( SELECT dopidpai FROM par.documentopar mi2 WHERE mi2.dopid = ".$_REQUEST['dopid']." ))
				AND mi.dopstatus IN ('I')";
	//ver(simec_htmlentities($sql),d);
	$cabecalho = array("Acões", "Documento", "Vigência", "Situação", "Data de alteração", "Alterado Por");
	$db->monta_lista($sql, $cabecalho, 20, 4, 'N','Center');

	exit;
}

if($_REQUEST['excluidocumento']){

	
	$sql = "UPDATE par.documentopar SET dopstatus = 'E', dopusucpfstatus = '".$_SESSION['usucpf']."', dopdatastatus = NOW(), dopdataalteracao = now(), usucpfalteracao = '".$_SESSION['usucpf']."' 
			WHERE dopnumerodocumento = (SELECT dopnumerodocumento FROM par.documentopar WHERE dopid = {$_REQUEST['dopid']} );";
	
	$db->executar( $sql );

	$sql = "DELETE FROM par.termocomposicao WHERE dopid = ".$_REQUEST['dopid'];
	$db->executar( $sql );

	$db->commit();

	header('content-type: text/html; charset=ISO-8859-1');
	$acoes = "'<center><img src=\"../imagens/alterar.gif\" style=\"cursor:pointer;\" onclick=\"carregaTermoMinuta('||mi.dopid||','|| mi.prpid ||');\" border=\"0\"> <img src=\"../imagens/excluir.gif\" style=\"cursor:pointer;\" onclick=\"excluiTermoMinuta('||mi.dopid||','|| mi.prpid ||');\" border=\"0\"></center>'";

	$sql = "SELECT
				$acoes as acao,
			    mo.mdonome
			FROM
				par.documentopar mi
		    LEFT  JOIN par.modelosdocumentos mo on mo.mdoid = mi.mdoid
		    INNER JOIN seguranca.usuario us on us.usucpf = mi.usucpfinclusao
			WHERE
				mi.prpid = {$_REQUEST['prpid']} 
				AND mi.dopstatus = 'A'";

	$cabecalho = array("Acões", "Documento");
	$dado = $db->pegaLinha( $sql );

	if( !$dado ){
		echo 'recarrega';
	} else {
		$db->monta_lista($sql, $cabecalho, 20, 4, 'N','Center');
	}

	exit;
}


if( $_REQUEST['verificaValorReformulacao'] ){

	$ano = $db->pegaUm( "select date_part('year',dopdatainclusao) from par.documentopar where dopid = ".$_REQUEST['dopid'] );

	$sql = "SELECT
				s.sbaid as sbaidpai, s2.sbaid as sbaidfilho
			FROM
				par.processoparcomposicao ppc
			INNER JOIN par.subacaodetalhe sd ON sd.sbdid = ppc.sbdid
			INNER JOIN par.subacao s ON s.sbaid = sd.sbaid
			INNER JOIN par.documentopar dop ON dop.prpid = ppc.prpid
			INNER JOIN par.subacao s2 ON s2.sbaidpai = s.sbaid
			WHERE
				ppc.ppcstatus = 'A' and
				dop.dopid = {$_REQUEST['dopid']} and s.sbareformulacao = true";

	$sbaids = $db->pegaLinha( $sql );

	$sbaids = $sbaids ? $sbaids : array();

	if( count($sbaids) < 1  ){
		echo 1;
		exit;
	} else {

		$sql = "SELECT
					s.sbaid,
					CASE WHEN sbacronograma = 1
					THEN ( SELECT sum(foo.vlrsubacao) FROM ( SELECT DISTINCT
								CASE WHEN sic.icovalidatecnico IS NULL -- antigos
									THEN sum(coalesce(sic.icoquantidade,0) * coalesce(sic.icovalor,0))::numeric(20,2)
									ELSE -- novos
										CASE WHEN sic.icovalidatecnico = 'S' THEN -- validado (caso não o item não é contado)
											sum(coalesce(sic.icoquantidadetecnico,0) * coalesce(sic.icovalor,0))::numeric(20,2)
										END
									END
								as vlrsubacao
						FROM par.subacaoitenscomposicao sic
						WHERE sic.sbaid = s.sbaid
						AND sic.icoano = sd.sbdano
						GROUP BY sic.sbaid, sic.icovalidatecnico ) as foo )
					ELSE ( SELECT sum(foo.vlrsubacao) FROM ( SELECT DISTINCT
							CASE WHEN (s.frmid = 2) OR ( s.frmid = 4 AND s.ptsid = 42 ) OR ( s.frmid = 12 AND s.ptsid = 46 )
								THEN -- escolas sem itens
									CASE WHEN se.sesvalidatecnico IS NULL -- antigos
									THEN
										sum(coalesce(se.sesquantidade,0) * coalesce(sic.icovalor,0))::numeric(20,2)
									ELSE -- novos
										sum(coalesce(se.sesquantidadetecnico,0) * coalesce(sic.icovalor,0))::numeric(20,2)
									END
								ELSE -- escolas com itens
									CASE WHEN sic.icovalidatecnico IS NULL -- antigos
									THEN
										sum(coalesce(ssi.seiqtd,0) * coalesce(sic.icovalor,0))::numeric(20,2)
									ELSE -- novos
										CASE WHEN sic.icovalidatecnico = 'S' THEN -- validado (caso não o item não é contado)
											sum(coalesce(ssi.seiqtdtecnico,0) * coalesce(sic.icovalor,0))::numeric(20,2)
										END
									END
								END
							as vlrsubacao
						FROM entidade.entidade t
						inner join entidade.funcaoentidade f on f.entid = t.entid
						left join entidade.entidadedetalhe ed on t.entid = ed.entid
						inner join entidade.endereco d on t.entid = d.entid
						left join territorios.municipio m on m.muncod = d.muncod
						left join par.escolas e on e.entid = t.entid
						INNER JOIN par.subacaoescolas se ON se.escid = e.escid
						INNER JOIN par.subacaoitenscomposicao sic on se.sbaid = sic.sbaid AND se.sesano = sic.icoano
						LEFT JOIN  par.subescolas_subitenscomposicao ssi ON ssi.sesid = se.sesid AND ssi.icoid = sic.icoid
						WHERE sic.sbaid = s.sbaid AND sic.icoano = sd.sbdano
						and (t.entescolanova = false or t.entescolanova is null) AND t.entstatus = 'A' and f.funid = 3 and t.tpcid = 3
						GROUP BY sic.sbaid, se.sesvalidatecnico, sic.icovalidatecnico ) as foo )
				END AS valorsubacao
			FROM
				par.subacao s
			INNER JOIN par.subacaodetalhe sd ON sd.sbaid = s.sbaid AND sd.sbdano = {$ano}
			WHERE
				s.sbaid IN ( ".implode(',', $sbaids)." )";

		$dados = $db->carregar($sql);

		if( is_array($dados) ){
			if( $dados[0]['valorsubacao'] > $dados[1]['valorsubacao'] ){
				echo 2;
			} else {
				echo 1;
			}
		}
	}

	exit;

}

function formPeriodoFinalizarRef(){

?>
<script>
jQuery(document).ready(function(){
	jQuery('.mes').blur(function(){
		if( jQuery(this).val().length == 1 ){
			jQuery(this).val('0'+jQuery(this).val());
		}
		var mes = jQuery(this).val();
		if( parseFloat(mes) < 1 || parseFloat(mes) > 12 ){
			alert('Mês inválido.');
			jQuery(this).val(1);
		}
	});
	jQuery('.ano').blur(function(){
		var ano = jQuery(this).val();
		if( parseFloat(ano) < 2013 || parseFloat(ano) < <?=date('Y')?>  ){
			alert('Ano inválido.');
			jQuery(this).val(2013);
		}
	});
	
});
</script>
<table class="tabela" bgcolor="#f5f5f5" style="width: 100%" cellSpacing="1" cellPadding="3" align="center">
	<tr>
		<td class="SubTituloDireita" width="20%">Início:</td>
		<td>
			<?=campo_texto('mes_inicio','S','S','',3,2,'##','', 'right', '',0, 'id="mes_inicio" class="mes"','', '', '', '' )?> Mês /
			<?=campo_texto('ano_inicio','S','S','',5,4,'####','', 'right', '',0, 'id="ano_inicio" class="ano"','', '', '', '' )?> Ano
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Fim:</td>
		<td>
			<?=campo_texto('mes_fim','S','S','',3,2,'##','', 'right', '',0, 'id="mes_fim" class="mes"','', '', '', '' )?> Mês /
			<?=campo_texto('ano_fim','S','S','',5,4,'####','', 'right', '',0, 'id="ano_fim" class="ano"','', '', '', '' )?> Ano
		</td>
	</tr>
</table>
<?php

}

if($_REQUEST['requisicao'] == 'telaanexo'){
	telaAnexoPar($_REQUEST);
	exit();
}

if($_REQUEST['requisicao'] == 'inseriranexotermoprocesso'){
	inserirAnexoProcessoPar($_REQUEST);
	exit();
}

if($_REQUEST['requisicao'] == 'downloadanexotermo'){
	downloadAnexoTermoPar($_REQUEST);
	exit();
}

if($_REQUEST['requisicao'] == 'excluiranexotermo'){
	excluirAnexoPar($_REQUEST);
	exit();
}

if($_REQUEST['requisicao'] == 'formPeriodoFinalizarRef'){
	formPeriodoFinalizarRef($_REQUEST);
	exit();
}

function telaAnexoPar($dados) {
	global $db;

	$sql = "SELECT a.arqid, a.arqnome||'.'||a.arqextensao as arquivo FROM par.processopar p
				INNER JOIN public.arquivo a ON a.arqid = p.arqidanexodoc
			WHERE prpid = '".$dados['prpid']."'";

	$arquivos = $db->carregar($sql);
	echo "<form method=post name=formulario id=formularioanexo enctype=multipart/form-data>";
	echo "<input type=hidden name=requisicao value=".$dados['req'].">";
	echo "<input type=hidden name=urlgo value=".$dados['urlgo'].">";
	echo "<input type=hidden name=prpid value=".$dados['prpid'].">";
	echo "<table align=\"center\" border=\"0\" class=\"tabela\" cellpadding=\"3\" cellspacing=\"1\">";
	echo "<tr>";
	echo "<td class=\"SubTituloDireita\">Anexo:</td>";
	echo "<td><input type=file name=arquivo id=arquivo ".(($arquivos[0])?"disabled":"")."></td>";
	echo "</tr>";
	if($arquivos[0]) {
		foreach($arquivos as $arquivo) {
			echo "<tr>";
			echo "<td class=\"SubTituloDireita\">Nome do arquivo:</td>";
			echo "<td>".$arquivo['arquivo']." <input type=button name=download value=Download onclick=window.location='par.php?modulo=principal/documentoPar&acao=A&arqid=".$arquivo['arqid']."&requisicao=downloadanexotermo';> <input type=button name=excluir value=Excluir onclick=excluirAnexoTermo(".$dados['prpid'].");></td>";
			echo "</tr>";
		}
	}
	echo "<tr>";
	echo "<td class=\"SubTituloCentro\" colspan=2>";
	if(!$arquivos[0]['arqid']) echo "<input type=button name=salvar value=Salvar onclick=enviarAnexoTermo();>";
	echo "<input type=button name=fechar value=Fechar onclick=closeMessage();></td>";
	echo "</tr>";
	echo "</table>";
	echo "</form>";
}

function inserirAnexoProcessoPar($dados) {
	global $db;

	if($_FILES['arquivo']['name']){
		include_once APPRAIZ . "includes/classes/fileSimec.class.inc";
		$arrCampos = array();
		$file = new FilesSimec("processopar", $arrCampos, "par");
		$file->setUpload("", "arquivo", false);
		$arqid = $file->getIdArquivo();
	}

	if($arqid) {

		$sql = "UPDATE par.processopar SET arqidanexodoc = $arqid WHERE prpid = {$dados['prpid']}";

		$db->executar($sql);
		$db->commit();

	}

	die("<script>
			alert('Anexado com sucesso.');
			window.location='".md5_decrypt($dados['urlgo'])."';
		 </script>");
}

function downloadAnexoTermoPar($dados) {
	global $db;
	include_once APPRAIZ . "includes/classes/fileSimec.class.inc";
	$arrCampos = array();
	$file = new FilesSimec("processopar",$arrCampos,"par");
	$file->getDownloadArquivo($dados['arqid']);
	die;
}

function excluirAnexoPar($dados) {
	global $db;

	$sql = "UPDATE par.processopar SET arqidanexodoc = NULL WHERE prpid = {$dados['prpid']}";
	$db->executar($sql);
	$db->commit();

	die("<script>
			alert('Anexo removido');
			window.location='".md5_decrypt($dados['urlgo'])."';
		 </script>");

}

if( $_REQUEST['finalizaReprogramacao'] ){
	
	global $db;
	
	finalizaReprogramacaoSubacao();
	
// 	$sql = "UPDATE par.subacao SET 
// 				sbareformulacao = false 
// 			WHERE sbaid in (SELECT s.sbaid FROM
// 								par.processoparcomposicao ppc
// 							INNER JOIN par.subacaodetalhe sd ON sd.sbdid = ppc.sbdid
// 							INNER JOIN par.subacao s ON s.sbaid = sd.sbaid
// 							INNER JOIN par.documentopar dop ON dop.prpid = ppc.prpid
// 							WHERE
// 								dop.dopid = {$_REQUEST['dopid']} and s.sbareformulacao = true)";
// 	$db->executar( $sql );
	
// 	$sql = "SELECT dopidoriginal FROM par.reprogramacao WHERE dopidreprogramado = ".$_REQUEST['dopid'];
// 	$dopidOriginal = $db->pegaUm($sql);

// 	$sql = "UPDATE par.documentopar SET dopreformulacao = false WHERE dopid = ".$dopidOriginal;
// 	$db->executar( $sql );

// 	$sql = "UPDATE par.reprogramacao SET repstatus = 'F' WHERE dopidreprogramado = ".$_REQUEST['dopid'];
// 	$db->executar( $sql );
	
// 	$dopid = $_REQUEST['dopid'];
	
// 	$db->commit();
// 	if($dopid)
// 	{
// 		 enviaEmailNovoTermo($dopid, 'reformulacao');
// 	}
// 	echo 1;
	exit;
}

if( $_REQUEST['finalizaReformulacao'] ){
	$sql = "SELECT
			  prpid, dopstatus, dopdiasvigencia, dopdatainicio, dopdatafim,
			  dp.mdoid as modelo, dopdatainclusao, usucpfinclusao, dopdataalteracao, usucpfalteracao, dopjustificativa,
			  dopdatavalidacaofnde, dopusucpfvalidacaofnde, dopdatavalidacaogestor, dopusucpfvalidacaogestor,
			  dopusucpfstatus, dopdatastatus, dopdatapublicacao, doppaginadou, dopnumportaria, proid,
			  dopreformulacao, dopidpai, tpdcod, dopdatainiciovigencia, dopdatafimvigencia
			FROM
			  par.documentopar dp
			 INNER JOIN par.modelosdocumentos md ON md.mdoid = dp.mdoid
			 WHERE dp.dopid = {$_REQUEST['dopid']}
			 	and dopreformulacao = true";

	$arrDadosDoc = $db->pegaLinha( $sql );
	$arrDadosDoc = $arrDadosDoc ? $arrDadosDoc : array();
	
	$testeI = explode('/',$arrDadosDoc['dopdatainiciovigencia']);
	$teste = explode('/',$arrDadosDoc['dopdatafimvigencia']);
	if( $teste[0] < 1 || $teste[1] < $testeI[1] ){
		$arrDadosDoc['dopdatafimvigencia'] = str_pad($teste[0],2,'0').'/'.date('Y');
		$sql = "SELECT
					TO_CHAR(('01/'||{$arrDadosDoc['dopdatafimvigencia']})::timestamp + INTERVAL '6 MONTHS', 'MM/YYYY') as dopdatafimvigencia
				FROM
					par.documentopar dp
				INNER JOIN par.modelosdocumentos md ON md.mdoid = dp.mdoid
				WHERE 
					dp.dopid = {$_REQUEST['dopid']}
					AND dopreformulacao = true";
	
		$arrDadosDoc['dopdatafimvigencia'] = $db->pegaUm($sql);
	}
	
	$sql = "SELECT  s.sbadsc as finalidade,
					to_char(now(), 'MM/YYYY') AS cronogramainicial,
					to_char( (now() + INTERVAL '365 DAY'), 'MM/YYYY') AS cronogramafinal
			FROM par.processopar p
			INNER JOIN par.empenho e ON e.empnumeroprocesso =  p.prpnumeroprocesso
			INNER JOIN par.empenhosubacao es ON es.empid = e.empid and eobstatus = 'A' and empstatus = 'A'
			INNER JOIN par.subacao s  ON s.sbaid  = es.sbaid
			INNER JOIN par.subacaodetalhe sd ON sd.sbaid = s.sbaid AND es.eobano = sd.sbdano
			WHERE p.prpid = ".$arrDadosDoc['prpid'];

	$arCronograma = $db->pegaLinha( $sql );

	/* Modificado dia 13/01/2014
	 * A pedido de: Isabel
	 * Programador: Eduardo Dunice Neto
	 * */
	$tipoDocumento = $arrDadosDoc['modelo'];

	$sql = "SELECT mdoconteudo, tpdcod FROM par.modelosdocumentos WHERE mdostatus = 'A' AND mdoid = ".$tipoDocumento;
	$arrModelo = $db->pegaLinha($sql);

	$mdoconteudo = $arrModelo['mdoconteudo'];
	$tpdcod = $arrModelo['tpdcod'];

	$sql = "SELECT sbdid as chk FROM par.termocomposicao WHERE dopid = ".$_REQUEST['dopid'];
	$subacoes = $db->carregarColuna($sql);

	$post = array('mdoid' => $arrDadosDoc['modelo'], 'dopid' => $_REQUEST['dopid'], 'tpdcod' => $tpdcod, 'chk' => $subacoes);

	$date = explode('/',$_REQUEST['inicio']);
	$date2 = explode('/',$_REQUEST['fim']);
	
	if( is_numeric($date[0]) && is_numeric($date2[0]) ){

		$_SESSION['par']['cronogramainicial'] 	= $_REQUEST['inicio'];
		$_SESSION['par']['cronogramaFinal'] 	= $_REQUEST['fim'];
		$arrDadosDoc['dopdatainiciovigencia'] 	= $_REQUEST['inicio'];
		$arrDadosDoc['dopdatafimvigencia'] 		= $_REQUEST['fim'];
	}else{

		$_SESSION['par']['cronogramainicial'] 	= $arrDadosDoc['dopdatainiciovigencia'];
		$_SESSION['par']['cronogramaFinal'] 	= $arrDadosDoc['dopdatafimvigencia'];
	}

	$doptexto = alteraMacrosMinuta($mdoconteudo, $arrDadosDoc['prpid'], $post);

	$arrDadosDoc['mdoid'] = $tipoDocumento;

	$dopid = salvarDadosMinuta($arrDadosDoc, $doptexto, $_REQUEST['dopid']);
	
	$sql = "INSERT INTO par.termocomposicao(sbdid, dopid)
			SELECT sbdid, $dopid
			FROM par.termocomposicao
			WHERE dopid = ".$_REQUEST['dopid'];
	$db->executar($sql);
	$db->commit();

	$sql = "UPDATE par.subacao SET 
				sbareformulacao = false 
			WHERE sbaid in (SELECT s.sbaid FROM
								par.processoparcomposicao ppc
							INNER JOIN par.subacaodetalhe sd ON sd.sbdid = ppc.sbdid
							INNER JOIN par.subacao s ON s.sbaid = sd.sbaid
							INNER JOIN par.documentopar dop ON dop.prpid = ppc.prpid
							WHERE
								ppc.ppcstatus = 'A' and 
								dop.dopid = {$_REQUEST['dopid']} and s.sbareformulacao = true)";
	$db->executar( $sql );

	$sql = "UPDATE par.documentopar SET dopreformulacao = false WHERE prpid = {$arrDadosDoc['prpid']}";
	$db->executar( $sql );
	$db->commit();

	echo 1;
	exit;
}

if( $_REQUEST['popup'] != 'true' || $_POST  ){
	$_SESSION['par']['documentospar'] = $_POST;
} else {
	$_REQUEST = $_SESSION['par']['documentospar'];
}

include APPRAIZ."includes/cabecalho.inc";
echo'<br>';
$db->cria_aba( $abacod_tela, $url, '' );
$tabela_execucao = monta_tabela_execucao( TIPO_PAR );
monta_titulo( $titulo_modulo, $tabela_execucao );

if( $_REQUEST['dopid'] != '' ){

	$sql = "SELECT DISTINCT
				TRUE
			FROM
				par.processopar prp
			INNER JOIN par.documentopar 					dop ON dop.prpid = prp.prpid
			INNER JOIN par.modelosdocumentos  				mdo ON mdo.mdoid = dop.mdoid
			INNER JOIN par.documentoparreprogramacaosubacao	dpr ON dpr.dopid = dop.dopid
			WHERE
				dopreformulacao IS TRUE
				AND dpr.dpsstatus = 'A'
				AND dopstatus = 'A'
				AND dop.dopid = {$_REQUEST['dopid']}";

	$emReformulacao = $db->pegaUm($sql);

	$sql = "SELECT TRUE FROM par.documentoparreprogramacao WHERE dopid = {$_REQUEST['dopid']} AND dprstatus = 'P' AND dprvalidacao NOT IN ('f')";

	$emReformulacaoPrazo = $db->pegaUm($sql);
}

?>
<style>
	#pesquisas thead tr td
	{
		background-color: #F7F7F7;
	}

	#pesquisas thead tr td.textosTabela
	{
		width: 200px;
	}
	#pesquisas thead tr td.camposTabela
	{
		width: 300px;
	}
	#pesquisas tbody tr td
	{
		background-color: #F7F7F7;
	}

	#pesquisas tbody tr td.SubTituloDireita
	{
		FONT-SIZE: 8pt;
    	COLOR: black;
    	FONT-FAMILY: Arial, Verdana;
    	TEXT-ALIGN: right;
		BACKGROUND-COLOR: #dcdcdc;
	}

	#pesquisas tbody tr td input.botao
	{
		border: 1px solid #757575;
		background-color: #DCDCDC;
		font-size: 11px;
		font-family: arial;
		font-weight: bold;
	}




</style>
<script type="text/javascript" src="/includes/prototype.js"></script>
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<script type="text/javascript" src="../includes/jquery-ui-1.8.18.custom/js/jquery-ui-1.8.18.custom.min.js"></script>
<link rel="stylesheet" type="text/css" href="../includes/jquery-ui-1.8.18.custom/css/ui-lightness/jquery-ui-1.8.18.custom.css"/>
<script type="text/javascript" src="../par/js/par.js"></script>
<link rel="stylesheet" href="/includes/ModalDialogBox/modal-message.css" type="text/css" media="screen" />
<script type="text/javascript" src="../includes/ModalDialogBox/modal-message.js"></script>
<script type="text/javascript" src="../includes/ModalDialogBox/ajax-dynamic-content.js"></script>
<script type="text/javascript" src="../includes/ModalDialogBox/ajax.js"></script>
<!--JS para o componente de exibir o tolltip de dados do processo-->
<script type="text/javascript" src="../includes/remedial.js"></script>
<script type="text/javascript" src="../includes/superTitle.js"></script>
<link rel="stylesheet" type="text/css" href="../includes/superTitle.css" />

<script type="text/javascript">

	messageObj = new DHTML_modalMessage();	// We only create one object of this class
	messageObj.setShadowOffset(5);	// Large shadow

	function displayMessage(url){
		messageObj.setSource(url);
		messageObj.setCssClassMessageBox(false);
		messageObj.setSize(560,300);
		messageObj.setShadowDivVisible(true);	// Enable shadow for these boxes
		messageObj.display();
	}

	function closeMessage(){
		messageObj.close();
	}

	function validaReprogramacao(dopid){
		window.open("par.php?modulo=principal/popupReprogramacaoPARSubacoes&acao=A&dopid="+dopid+"&valida=1","reprogramacaoPar","height=400,width=600,scrollbars=yes,top=50,left=200");
	}

jQuery.noConflict();

jQuery(document).ready(function(){
	
	if(jQuery('#tpdcod').val() != ''){
		verificaTipoDocumento( jQuery('#tpdcod').val() );
	}

	jQuery('.gerarReprogramacaoPrazo').live('click',function(){

		var dopid = jQuery(this).attr('id');
		var prpid = jQuery(this).attr('prpid');
		
		window.open('par.php?modulo=principal/minuta&acao=A&dopid='+dopid+'&prpid='+prpid+'&geraTermoReprogramacaoPrazo=1&origem=2','minuta','scrollbars=yes,status=no,toolbar=no,menubar=no,location=no,fullscreen=yes');
	});
	
	jQuery('.finalizar').live('click', function(){

        var dopid = jQuery(this).attr('id');

        jQuery.ajax({
	   		type: "POST",
	   		url: window.location.href,
	   		data: "requisicao=formPeriodoFinalizarRef&dopid="+dopid,
	   		async: false,
	   		success: function(msg){
	   			jQuery( "#dialog" ).html(msg);
				jQuery( "#dialog" ).dialog({
					resizable: true,
					height:180,
					width:400,
					modal: true,
					buttons: {
						"Finalizar": function() {
							if( jQuery('#mes_inicio').val() == '' ){
								alert('Campo obrigatório.');
								jQuery('#mes_inicio').focus();
								return false;
							}
							if( jQuery('#mes_fim').val() == '' ){
								alert('Campo obrigatório.');
								jQuery('#mes_fim').focus();
								return false;
							}
							if( jQuery('#ano_inicio').val() == '' ){
								alert('Campo obrigatório.');
								jQuery('#ano_inicio').focus();
								return false;
							}
							if( jQuery('#ano_fim').val() == '' ){
								alert('Campo obrigatório.');
								jQuery('#ano_fim').focus();
								return false;
							}
							if( parseFloat(jQuery('#ano_fim').val()+jQuery('#mes_fim').val()) < parseFloat(jQuery('#ano_inicio').val()+jQuery('#mes_inicio').val())){
								alert('Data de fim deve ser maior que a data de início.');
								return false;
							}
							finaliza( dopid );
							jQuery( this ).dialog( "close" );
						},
						"Atender Automaticamente": function() {
							jQuery('#mes_inicio').val('');
							jQuery('#mes_fim').val('');
							jQuery('#ano_inicio').val('');
							jQuery('#ano_fim').val('');
							finaliza( dopid );
							jQuery( this ).dialog( "close" );
						}

					}
				});
	   		}
		});
	});

	function pad (str, max) {
	  str = str.toString();
	  return str.length < max ? pad("0" + str, max) : str;
	}

	jQuery('[name="dopdatafimvigencia"]').live('blur',function(){

		var datainicio = jQuery('#datainicial').val().split('/');
		var mesini = datainicio[0];
		var anoini = datainicio[1];
		
		var data = jQuery(this).val().split('/');
		var mes = data[0];
		var ano = data[1];

		if(jQuery(this).val() == '' ){
			ano = anoini;
			mes = mesini;
		}
		
		if( mes == '' ){
			mes = mesini;
		}

		if( ano == '' ){
			ano = anoini;
		}

		if( mes > 12 ){
			mes = 12;
		}
		
		if( mes < 1 ){
			mes = '01';
		}
		
		if( mes < 10 ){
			mes = pad(mes, 2);
		}

		if( ano < anoini ){
			ano = anoini;
		}

		if( ano == anoini ){
			if( mes < mesini ){
				mes = mesini;
			}
		}

		jQuery(this).val(mes+'/'+ano);
	});

	jQuery('.regerarTermo').live('click', function(){

		var dopid = jQuery(this).attr('id');
                jQuery.ajax({
                    type: "POST",
                    url: window.location.href,
                    data: "req=verificaPendenciaSubacaoEscola&dopid="+dopid,
                    async: false,
                    success: function(msg){
                        if (msg != '') {
                            jQuery( "#dialog-regerar-termo" ).html(msg);
                            jQuery( "#dialog-regerar-termo" ).dialog({
                                resizable: true,
                                width:400,
                                modal: true,
                                buttons: {
                                    "OK": function() {
                                        jQuery( this ).dialog( "close" );
                                    }
                                }
                            });
                        } else {
                            // Pode regerar Documento
                            jQuery.ajax({
                                type: "POST",
                                url: window.location.href,
                                data: "req=formRegeraTermo&dopidRegera="+dopid,
                                async: false,
                                success: function(msg){
                                    jQuery( "#dialog-regerar-termo" ).html(msg);
                                    jQuery( "#dialog-regerar-termo" ).dialog({
                                        resizable: true,
                                        width:400,
                                        modal: true,
                                        buttons: {
                                            "Sim": function() {
                                                if( jQuery('[name="dopdatafimvigencia"]').val() == '' ){
                                                    alert('Campo obrigatório.');
                                                    jQuery('[name="dopdatafimvigencia"]').focus();
                                                    return false;
                                                }
                                                if( jQuery('#refREG').val() == 'TRUE' ){
                                                    alert('Ação impossível. Termo em reformulação.');
                                                    return false;
                                                }
                                                if( confirm('Deseja regerar o termo?') ){
                                                    divCarregando();
                                                    jQuery('#reqRegera').val('regerarTermo');
                                                    jQuery('#formRegerar').submit();
                                                    jQuery( this ).dialog( "close" );
                                                }
                                            },
                                            "Não": function() {
                                                jQuery( this ).dialog( "close" );
                                            }
                                        }
                                    });
                                }
                            });
                        }
                    }
		});
	});

	jQuery('.termoEx').live('click', function(){

		var dopid = jQuery(this).attr('id');

		jQuery.ajax({
	   		type: "POST",
	   		url: window.location.href,
	   		data: "req=formGeraEx&dopidEx="+dopid,
	   		async: false,
	   		success: function(msg){
				jQuery( "#dialog-termo-ex" ).html(msg);
				jQuery( "#dialog-termo-ex" ).dialog({
					resizable: true,
					width:400,
					modal: true,
					buttons: {
						"Sim": function() {
							if( jQuery('[name="dopdatafimvigenciaex"]').val() == '' ){
								alert('Campo obrigatório.');
								jQuery('[name="dopdatafimvigenciaex"]').focus();
								return false;
							}
							if( confirm('Deseja gerar o termo?') ){
								divCarregando();
								jQuery('#reqGeraEx').val('termoEx');
								jQuery('#formEx').submit();
								jQuery( this ).dialog( "close" );
							}
						},
						"Não": function() {
							jQuery( this ).dialog( "close" );
						}
		
					}
				});
	   		}
		});
	});

	jQuery('.vizualizarDocumento').live('click', function(){

        var dopid = jQuery(this).attr('id');

//         window.open('par.php?modulo=principal/minuta&acao=A&dopid='+dopid+'&prpid='+prpid,'minuta',
//                 	'scrollbars=yes,status=no,toolbar=no,menubar=no,location=no,fullscreen=yes');
        window.open(window.location.href+'&req=formVizualizaDocumento&dopid='+dopid,'minuta',
                	'scrollbars=yes,status=no,toolbar=no,menubar=no,location=no,fullscreen=yes');
        
//         jQuery.ajax({
// 	   		type: "POST",
// 	   		url: window.location.href,
// 	   		data: "req=formVizualizaDocumento&dopid="+dopid,
// 	   		async: false,
// 	   		success: function(msg){
// 	   			jQuery( "#dialog" ).html(msg);
// 	   			jQuery( "#dialog" ).dialog({
// 					resizable: true,
// 					height:720,
// 					width:1280,
// 					modal: true,
// 					buttons: {
// 						"Fechar": function() {
// 							jQuery( this ).dialog( "close" );
// 						}
// 					}
// 				});
// 	   		}
// 		});
	});
});


</script>
<div id="dialog-termo-ex" title="Gerar Termo EX" style="display:none;">
</div>
<div id="dialog-regerar-termo" title="Regerar Termo" style="display:none;">
</div>
<form method="post" name="formulario" id="formulario">
<div id="dialog" title="Termo de Compromisso" style="display:none;">
</div>
<form method="post" name="formulario" id="formulario">
<div id="dialog" title="Termo de Compromisso" style="display:none;">
</div>
<table id="pesquisas" align="center" border="0" class="tabela" cellpadding="3" cellspacing="1">
	<tbody>
	<tr>
		<td class="SubTituloDireita" width="30%">Número de Processo:</td>
		<td colspan="3">
		<?php
			$filtro = simec_htmlentities( $_REQUEST['numeroprocesso'] );
			$numeroprocesso = $filtro;
			echo campo_texto( 'numeroprocesso', 'N', 'S', '', 50, 200, '', '');
		?>
		</td>

	</tr>
	<tr>
		<td class="SubTituloDireita">Número do Termo:</td>
		<td colspan="3">
		<?php
			$dopid = $_REQUEST['dopid'];
			echo campo_texto( 'dopid', 'N', 'S', '', 20, 50, '[#]', '');
		?>
		</td>

	</tr>
	<tr>
		<td class="SubTituloDireita" >Município:</td>
		<td colspan="3">
		<?php
			$filtro = simec_htmlentities( $_REQUEST['municipio'] );
			$municipio = $filtro;
			echo campo_texto( 'municipio', 'N', 'S', '', 50, 200, '', '');
		?>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita" >Estado:</td>
		<td colspan="3">
		<?php
			$estuf = $_REQUEST['estuf'];
			$sql = "select e.estuf as codigo, e.estdescricao as descricao from territorios.estado e order by e.estdescricao asc";
			$db->monta_combo( "estuf", $sql, 'S', 'Todas as Unidades Federais', '', '' );
		?>
		</td>
	</tr>
	<tr>
		<td class="subtitulodireita">Ano do Processo:</td>
		<td>
			<?php
			$anoprocesso = $_REQUEST['anoprocesso'];
			$sql = "select distinct  substring(prpnumeroprocesso,12,4) as codigo,  substring(prpnumeroprocesso,12,4) as descricao from par.processopar where substring(prpnumeroprocesso,12,4) <> ''";
			$db->monta_combo( "anoprocesso", $sql, 'S', 'Todos', '', '' );
			?>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita" >Empenho:</td>
		<td colspan="3">
			<?
			$empenhado = $_REQUEST['empenhado'];
			$arrEmpenho = array(
								array('codigo' => 1, 'descricao' => 'Empenhado Solicitado 100%'),
								array('codigo' => 2, 'descricao' => 'Nenhum Empenho Solicitado'),
								array('codigo' => 3, 'descricao' => 'Empenho Parcial')
							);
			$db->monta_combo( "empenhado", $arrEmpenho, 'S', 'Selecione', '', '' );
			?>
			<!--  Sim: <input <?php echo $_REQUEST['empenhado'] == "1" ? "checked='checked'" : "" ?> type="radio" name="empenhado" value="1" >
			Não: <input <?php echo $_REQUEST['empenhado'] == "2" ? "checked='checked'" : "" ?> type="radio" name="empenhado" value="2" >-->
		</td>
	</tr>
	<tr>
		<td class="subtitulodireita">Esfera:</td>
		<td>
			<?php
				$esfera = $_POST['esfera'];
				$arrEsfera = array(0 => array("codigo"=>"Municipal","descricao"=>"Municipal"), 1 => array("codigo"=>"Estadual","descricao"=>"Estadual"));
				$db->monta_combo( "esfera", $arrEsfera, 'S', 'Todas as esferas', '', '' );
			?>
		</td>
	</tr>
	<tr>
		<td class="subtitulodireita">Vigência:</td>
		<td>
			<?php
				$vigencia = $_POST['vigencia'];
				$sql = "SELECT
							DISTINCT
							CASE WHEN char_length(dopdatafimvigencia) = 7 THEN
								SUBSTRING(trim(dopdatafimvigencia) FROM 4 FOR 4) || '-' || SUBSTRING(trim(dopdatafimvigencia) FROM 1 FOR 2)
							ELSE
								SUBSTRING(trim(dopdatafimvigencia) FROM 7 FOR 4) || '-' || SUBSTRING(trim(dopdatafimvigencia) FROM 4 FOR 2)
							END as codigo,
							CASE WHEN char_length(dopdatafimvigencia) = 7 THEN
								trim(dopdatafimvigencia)
							ELSE
								SUBSTRING(trim(dopdatafimvigencia) FROM 4 FOR 9)
							END as descricao
						FROM
							par.documentopar
						WHERE
							dopdatafimvigencia is not null
						ORDER BY
							codigo";
				$db->monta_combo( "vigencia", $sql, 'S', 'Todas as Vigências', '', '' );
			?>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Empenho Solicitado:</td>
		<td colspan="3">
			<input <?php echo $_REQUEST['empenhosolicitado'] == "1" ? "checked='checked'" : "" ?> type="radio" name="empenhosolicitado" value="1" />Sim
			<input <?php echo $_REQUEST['empenhosolicitado'] == "2" ? "checked='checked'" : "" ?> type="radio" name="empenhosolicitado" value="2" />Não
			<?if(empty($_REQUEST['empenhosolicitado'])){?>
				<input <?php echo empty($_REQUEST['empenhosolicitado']) ? "checked='checked'" : "" ?> type="radio" name="empenhosolicitado" value="3" checked="checked" />Todos
			<?}else{ ?>
				<input <?php echo empty($_REQUEST['empenhosolicitado']) ? "checked='checked'" : "" ?> type="radio" name="empenhosolicitado" value="3" />Todos
			<?} ?>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Reprogramação de Prazo Solicitada:</td>
		<td colspan="3">
			<input <?php echo $_REQUEST['reprogramacaosolicitada'] == "1" ? "checked='checked'" : "" ?> type="radio" name="reprogramacaosolicitada" value="1" />Sim
			<?if(empty($_REQUEST['reprogramacaosolicitada'])){?>
				<input <?php echo empty($_REQUEST['reprogramacaosolicitada']) ? "checked='checked'" : "" ?> type="radio" name="reprogramacaosolicitada" value="3" checked="checked" />Todos
			<?}else{ ?>
				<input <?php echo empty($_REQUEST['reprogramacaosolicitada']) ? "checked='checked'" : "" ?> type="radio" name="reprogramacaosolicitada" value="3" />Todos
			<?} ?>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Reprogramação de Subação Solicitada:</td>
		<td colspan="3">
			<input <?php echo $_REQUEST['reprogramacaosubsolicitada'] == "1" ? "checked='checked'" : "" ?> type="radio" name="reprogramacaosubsolicitada" value="1" />Sim
			<?if(empty($_REQUEST['reprogramacaosubsolicitada'])){?>
				<input <?php echo empty($_REQUEST['reprogramacaosubsolicitada']) ? "checked='checked'" : "" ?> type="radio" name="reprogramacaosubsolicitada" value="3" checked="checked" />Todos
			<?}else{ ?>
				<input <?php echo empty($_REQUEST['reprogramacaosubsolicitada']) ? "checked='checked'" : "" ?> type="radio" name="reprogramacaosubsolicitada" value="3" />Todos
			<?} ?>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita" >Tipo Execução:</td>
		<td colspan="3">
			<input <?php echo $_REQUEST['tipoExecucao'] == "C" ? "checked='checked'" : "" ?> type="radio" name="tipoExecucao" value="C" >Convênio
			<input <?php echo $_REQUEST['tipoExecucao'] == "T" ? "checked='checked'" : "" ?> type="radio" name="tipoExecucao" value="T" >Termo
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Termo:</td>
		<td>
			<fieldset style="width: 380px"><legend>Filtros Termo:</legend>
			<input <?php echo $_REQUEST['termogerado'] == "1" ? "checked='checked'" : "" ?> type="radio" name="termogerado" id="termogerado_1" value="1" >Gerado
			<input <?php echo $_REQUEST['termogerado'] == "2" ? "checked='checked'" : "" ?> type="radio" name="termogerado" id="termogerado_2" value="2" >Não Gerado
			<input <?php echo empty($_REQUEST['termogerado']) ? "checked='checked'" : "" ?> type="radio" name="termogerado" id="termogerado_3" value="" >Todos
		<br>
			<table border="0"  align="left" bgcolor="#f5f5f5" cellpadding="0" cellspacing="0" width="380px">
					<tr>
						<td class="SubTituloDireita" style="width: 60px;text-align:center;" >Validação:</td>
						<td>
				<table border="0" style="width: 100%; background-color: #f5f5f5;">
					<tr>
						<td class="SubTituloDireita" style="width: 40%;" >Gestor:</td>
						<td>
							<input <?php echo $_REQUEST['dopusucpfvalidacaogestor'] == "t" ? "checked='checked'" : "" ?>  type="radio" value="t" id="dopusucpfvalidacaogestor" name="dopusucpfvalidacaogestor" />Sim
							<input <?php echo $_REQUEST['dopusucpfvalidacaogestor'] == "f" ? "checked='checked'" : "" ?>  type="radio" value="f" id="dopusucpfvalidacaogestor" name="dopusucpfvalidacaogestor" />Não
							<input <?php echo empty($_REQUEST['dopusucpfvalidacaogestor']) ? "checked='checked'" : "" ?> type="radio" name="dopusucpfvalidacaogestor" value="" >Todos
						</td>
					</tr>
					<tr>
						<td class="SubTituloDireita" style="width: 40%;" >De outras Entidades:</td>
						<td>
							<input <?php echo $_REQUEST['dopusucpfvalidacaofnde'] == "t" ? "checked='checked'" : "" ?> type="radio" value="t" id="dopusucpfvalidacaofnde" name="dopusucpfvalidacaofnde" />Sim
							<input <?php echo $_REQUEST['dopusucpfvalidacaofnde'] == "f" ? "checked='checked'" : "" ?> type="radio" value="f" id="dopusucpfvalidacaofnde" name="dopusucpfvalidacaofnde"  />Não
							<input <?php echo empty($_REQUEST['dopusucpfvalidacaofnde']) ? "checked='checked'" : "" ?> type="radio" name="dopusucpfvalidacaofnde" value="" >Todos
						</td>
					</tr>
				</table>
			</table>
			</fieldset>
		</td>
	</tr>
			<?php
			$sql = "SELECT pts.ptsid as codigo, pts.ptsdescricao || ' - ' || fe.frmdsc  AS descricao
					FROM par.propostatiposubacao pts
					INNER JOIN par.formaexecucao fe ON fe.frmid = pts.frmid
					WHERE pts.ptsstatus = 'A'
					AND fe.frmid in (6, 13)
					ORDER BY pts.ptsdescricao, fe.frmdsc";

			$stSqlCarregados = "";

			if( $_REQUEST['ptsid'][0] ){
				$stSqlCarregados = "SELECT pts.ptsid as codigo, pts.ptsdescricao || ' - ' || fe.frmdsc  AS descricao
									FROM par.propostatiposubacao pts
									INNER JOIN par.formaexecucao fe ON fe.frmid = pts.frmid
									WHERE pts.ptsstatus = 'A'
									AND fe.frmid in (6, 13)
									and pts.ptsid IN ( ". implode($_REQUEST['ptsid'], ","). ")
									ORDER BY pts.ptsdescricao, fe.frmdsc";
			}
			$ptsid_campo_excludente = $_REQUEST['ptsid_campo_excludente'];
			mostrarComboPopup( 'Tipo de Subação:', 'ptsid',  $sql, $stSqlCarregados, 'Selecione o Tipo de Subação' );
			?>
	<tr>
	<tr>
		<td class="SubTituloDireita" >Tipo de Documento:</td>
		<td colspan="3">
                <?php
                   $obTipoDocumento = new TipoDocumentoControle();
                   $sqlTipoDocumento = $obTipoDocumento->carregarTipoDocumento(TRUE, array(21, 102));
                   $db->monta_combo("tpdcod", $sqlTipoDocumento, 'S', 'Selecione', 'verificaTipoDocumento', '', '', 400, 'N', 'tpdcod', '', $_REQUEST['tpdcod']);
               ?>
		</td>
	</tr>
	<tr id="tr_modelodoc" style="display: none">
		<td class="SubTituloDireita" >Modelo de Documento:</td>
		<td colspan="3">
		<div id="div_modelodoc">
		<?php
			$sql = array();
			$db->monta_combo( "mdoid", $sql, 'S', 'Selecione', '', '', '', 400, 'N', 'mdoid' );
		?></div>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Reformulação:</td>
		<td colspan="3">
		<?php
			$aryReformulacao = array(
				array('codigo' => 't', 'descricao' => 'Termos em Reformulação'),
				array('codigo' => 'f', 'descricao' => 'Termos Reformulados')
			);
			$reformulacao = $_POST['reformulacao'];
			$db->monta_combo("reformulacao",$aryReformulacao, 'S', 'Todos', '', '', '',400,'N','reformulacao');	?>
		</td>
	</tr>
    <tr>
        <td class="SubTituloDireita">Obras MUNICIPAIS com pendências:</td>
        <td>
            <input <?php echo $_REQUEST['pendenciaObras'] == "1" ? "checked='checked'" : "" ?> type="radio" name="pendenciaObras" id="pendenciaObras_1" value="1" >Sim
            <input <?php echo $_REQUEST['pendenciaObras'] == "2" ? "checked='checked'" : "" ?> type="radio" name="pendenciaObras" id="pendenciaObras_2" value="2" >Não
            <input <?php echo empty($_REQUEST['pendenciaObras']) ? "checked='checked'" : "" ?> type="radio" name="pendenciaObras" value=""   />Todos
        </td>
    </tr>
    <tr>
        <td class="SubTituloDireita">Obras ESTADUAIS com pendências:</td>
        <td>
            <input <?php echo $_REQUEST['pendenciaObrasUF'] == "1" ? "checked='checked'" : "" ?> type="radio" name="pendenciaObrasUF" id="pendenciaObras_uf_1" value="1" >Sim
            <input <?php echo $_REQUEST['pendenciaObrasUF'] == "2" ? "checked='checked'" : "" ?> type="radio" name="pendenciaObrasUF" id="pendenciaObras_uf_2" value="2" >Não
            <input <?php echo empty($_REQUEST['pendenciaObrasUF']) ? "checked='checked'" : "" ?> type="radio" name="pendenciaObrasUF" value=""   />Todos
        </td>
    </tr>
	<tr>
		<td colspan="3" class="SubTituloDireita" style="text-align:  center;">
			<input type="button" name="pesquisar" value="Pesquisar" onclick="submeterTela('nao')" />
			<input type="button" name="todos" value="Ver todos" onclick="window.location.href='par.php?modulo=principal/documentoPar&acao=A';" />
			<input type="button" name="excel" value="Exportar Excel" onclick="submeterTela('sim')" />
			<input type="hidden" name="exportarexcel" id="exportarexcel" value="nao" />
		</td>
	</tr>
	</tbody>
</table>
</form>
<div id="debug"></div>
<script type="text/javascript">

function verificaTipoDocumento(tipo){
    if (tipo !== '') {
	jQuery.ajax({
   		type: "POST",
   		url: "par.php?modulo=principal/documentoPar&acao=A",
   		data: "tipodocumento=" + tipo + '&mdoid=<?=$_REQUEST['mdoid'] ?>',
   		async: false,
   		success: function(msg){
   			jQuery('#tr_modelodoc').show();
			jQuery('#div_modelodoc').html(msg);
   		}
   	});
    } else {
        jQuery('#tr_modelodoc').hide();
    }
}

function submeterTela(valor){
	document.getElementById('exportarexcel').value = valor;
	selectAllOptions( document.formulario.ptsid );
/*
	if( document.getElementById('termogerado_1').checked == true || document.getElementById('termogerado_2').checked == true ){

		if( jQuery('#tpdcod').val() == '' ){
			alert('Informe um Tipo de Documento!');
			jQuery('#tpdcod').focus();
			return false;
		}
		if( jQuery('#mdoid').val() == '' ){
			alert('Informe um Modelo de Documento!');
			jQuery('#mdoid').focus();
			return false;
		}
	} else {
		if( jQuery('#tpdcod').val() != '' ){
			if( jQuery('#mdoid').val() == '' ){
				alert('Informe um Modelo de Documento!');
				jQuery('#mdoid').focus();
				return false;
			}
		}
	}
	*/
	jQuery('#formulario').submit();
}

function carregarListaDocumentoPar(idImg, prpid){
	var img 	 = $( idImg );
	var tr_nome = 'listaDocumentos_'+ prpid;
	var td_nome  = 'trV_'+ prpid;

	if($(tr_nome).style.display == 'none' && $(td_nome).innerHTML == ""){
		$(td_nome).update('Carregando...');
		img.src = '../imagens/menos.gif';
		carregaListaDocumento(prpid, td_nome);
	}
	if($(tr_nome).style.display == 'none' && $(td_nome).innerHTML != ""){
		$(tr_nome).style.display = '';
		img.src = '../imagens/menos.gif';
	} else {
		$(tr_nome).style.display = 'none';
		img.src = '/imagens/mais.gif';
	}
}

function carregarListaDocumentoParReformulado(idImg, dopid){
	var img 	 = $( idImg );
	var tr_nome = 'listaDocumentos2_'+ dopid;
	var td_nome  = 'trV2_'+ dopid;

	if($(tr_nome).style.display == 'none' && $(td_nome).innerHTML == ""){
		$(td_nome).update('Carregando...');
		img.src = '../imagens/menos.gif';
		carregaListaDocumentoReformulado(dopid, td_nome);
	}
	if($(tr_nome).style.display == 'none' && $(td_nome).innerHTML != ""){
		$(tr_nome).style.display = '';
		img.src = '../imagens/menos.gif';
	} else {
		$(tr_nome).style.display = 'none';
		img.src = '/imagens/mais.gif';
	}
}

function carregaListaDocumento(prpid, td_nome, emecod, autid){
	var myajax = new Ajax.Request('par.php?modulo=principal/documentoPar&acao=A', {
		        method:     'post',
		        parameters: '&listadocumento=true&prpid='+prpid,
		        asynchronous: false,
		        onComplete: function (res){
					//$(td_nome).innerHTML = res.responseText;
					$(td_nome).update(res.responseText);
		        }
		  });
}

function carregaListaDocumentoReformulado(dopid, td_nome, emecod, autid){
	var myajax = new Ajax.Request('par.php?modulo=principal/documentoPar&acao=A', {
		        method:     'post',
		        parameters: '&listadocumentoReformulado=true&dopid='+dopid,
		        asynchronous: false,
		        onComplete: function (res){
					//$(td_nome).innerHTML = res.responseText;
					$(td_nome).update(res.responseText);
		        }
		  });
}

//function gerarTermoEx(dopid){
//	window.open('par.php?modulo=principal/minuta&acao=A&dopid='+dopid,'Termo EX','height=200,width=300,scrollbars=yes,top=50,left=200');
//}

function carregaTermoMinuta(dopid, prpid){
	window.open('par.php?modulo=principal/minuta&acao=A&dopid='+dopid+'&prpid='+prpid,'minuta','scrollbars=yes,status=no,toolbar=no,menubar=no,location=no,fullscreen=yes');
}

function gerarTermoReprogramacao(dopid, prpid){
	window.open('par.php?modulo=principal/minuta&acao=A&dopid='+dopid+'&prpid='+prpid+'&geraTermoReprogramacao=1','minuta','scrollbars=yes,status=no,toolbar=no,menubar=no,location=no,fullscreen=yes');
}

function finalizaReprogramacao(dopid, prpid){
	if( confirm('Tem certeza que gostaria de finalizar a reprogramação? O Termo de compromisso está correto?') ){
		var myajax = new Ajax.Request('par.php?modulo=principal/documentoPar&acao=A', {
			        method:     'post',
			        parameters: '&finalizaReprogramacao=true&dopid='+dopid,
			        asynchronous: false,
			        onComplete: function (res){
				       	
			        	if( res.responseText ){
		        			alert(res.responseText);
		        			window.location.reload();
		        		}else {
		        			alert('Falha na finalização da Reformulação!');
		        		}
			        }
			  });
	}
}

function onOffCampo( campo )
{
	var div_on = document.getElementById( campo + '_campo_on' );
	var div_off = document.getElementById( campo + '_campo_off' );
	var input = document.getElementById( campo + '_campo_flag' );
	if ( div_on.style.display == 'none' )
	{
		div_on.style.display = 'block';
		div_off.style.display = 'none';
		input.value = '1';
	}
	else
	{
		div_on.style.display = 'none';
		div_off.style.display = 'block';
		input.value = '0';
	}
}

function excluiTermoMinuta(dopid, prpid){
	if( confirm('Tem certeza que gostaria de excluir esse registro?') ){
		var td_nome  = 'trV_'+ prpid;
		var myajax = new Ajax.Request('par.php?modulo=principal/documentoPar&acao=A', {
			        method:     'post',
			        parameters: '&excluidocumento=true&dopid='+dopid+'&prpid='+prpid,
			        asynchronous: false,
			        onComplete: function (res){
			        	if( res.responseText == 'recarrega' ){
			        		window.location.href = window.location;
			        	} else {
							$(td_nome).update(res.responseText);
						}
			        }
			  });
	}
}

function validarReprogramacao( dopid ){
	window.open("par.php?modulo=principal/popupReprogramacaoPARAprovacao&acao=A&dopid="+dopid+"&valida=1","reprogramacaoPar","height=400,width=600,scrollbars=yes,top=50,left=200");
}

function finaliza( dopid ) {
	divCarregando();
	var myajax = new Ajax.Request('par.php?modulo=principal/documentoPar&acao=A', {
			        method:     'post',
			        parameters: '&verificaValorReformulacao=true&dopid='+dopid,
			        asynchronous: false,
			        onComplete: function (res){
			        	divCarregado();
			        	if(res.responseText == 1){
			        		return finaliza2( dopid );
			        	} else {
			        		if( confirm("Valor divergente do Termo original. Continuar?") ){
			        			return finaliza2( dopid );
			        		}
			        	}
						//return res.responseText;
			        }
			  });
}

function finaliza2( dopid ){
		divCarregando();
	if( confirm('Tem certeza que deseja finalizar essa reformulação?') ){
		var datas = '';
		if( jQuery('#mes_inicio').val() != '' ){
			var inicio	= jQuery('#mes_inicio').val()+'/'+jQuery('#ano_inicio').val();
			var fim		= jQuery('#mes_fim').val()+'/'+jQuery('#ano_fim').val();
			datas 		= "&inicio="+inicio+"&fim="+fim;
		}
		var myajax = new Ajax.Request('par.php?modulo=principal/documentoPar&acao=A', {
			        method:     'post',
			        parameters: '&finalizaReformulacao=true&dopid='+dopid+datas,
			        asynchronous: false,
			        onComplete: function (res){
			        	/*alert(res.responseText);
			        	document.getElementById('debug').innerHTML = res.responseText;
			        	return false;*/
			        	//alert(res.responseText);
		        		//document.getElementById('debug').innerHTML = res.responseText;
		        		if( res.responseText = '1' ){
		        			alert('Reformulação finalizada com sucesso!');
		        			window.location.reload();
		        		} else {
		        			alert('Falha na finalização da Reformulação!');
		        			window.location.reload();
		        		}
			        }
			  });
	}
	divCarregado();
}

function enviarAnexo2(prpid) {
	displayMessage('par.php?modulo=principal/documentoPar&acao=A&urlgo=<? echo md5_encrypt('par.php?modulo=principal/documentoPar&acao=A'); ?>&requisicao=telaanexo&req=inseriranexotermoprocesso&prpid='+prpid);
}

function excluirAnexoTermo(prpid) {
	var conf = confirm('Deseja excluir o anexo?');
	if(conf) {
		window.location='par.php?modulo=principal/documentoPar&acao=A&urlgo=<? echo md5_encrypt('par.php?modulo=principal/documentoPar&acao=A'); ?>&requisicao=excluiranexotermo&prpid='+prpid;
	}
}

function enviarAnexo(dopid) {
	displayMessage('par.php?modulo=principal/documentoPar&acao=A&urlgo=<? echo md5_encrypt('par.php?modulo=principal/documentoPar&acao=A'); ?>&requisicao=telaanexo&req=inseriranexotermo&prpid='+prpid);
}

</script>
<?php
	if($_REQUEST['empenhado'] == "3" && $_POST['estuf'] == ""){
		echo "<script language='javascript'>alert('Por favor, selecione um Estado!');</script>";
		exit;
	}

	if($_REQUEST['numeroprocesso']) {
		$where[] = "p.prpnumeroprocesso = '".str_replace(array("-",".","/"), "", $_REQUEST['numeroprocesso'])."'";
	}
	if($_REQUEST['municipio']) {
		$where[] = "removeacento(m.mundescricao) ILIKE removeacento('%".$_REQUEST['municipio']."%')";
	}

	if($_REQUEST['estuf']) {
		$where[] = " ((iu.estuf='".$_REQUEST['estuf']."') or (iu.mun_estuf = '".$_REQUEST['estuf']."'))";
	}

	if($_REQUEST['empenhosolicitado'] == "1") {
		$where[] = "(SELECT sum(vve.vrlempenhocancelado) as empvalorempenho FROM par.empenho ep2
								inner join par.v_vrlempenhocancelado vve on vve.empid = ep2.empid and empstatus = 'A'
							 WHERE ep2.empnumeroprocesso=p.prpnumeroprocesso and empcodigoespecie not in ('03', '13', '02', '04')) > 0";
	}
	if($_REQUEST['empenhosolicitado'] == "2") {
		$where[] = "(SELECT sum(vve.vrlempenhocancelado) as empvalorempenho FROM par.empenho ep2
								inner join par.v_vrlempenhocancelado vve on vve.empid = ep2.empid and empstatus = 'A'
							WHERE ep2.empnumeroprocesso=p.prpnumeroprocesso and empcodigoespecie not in ('03', '13', '02', '04')) = 0";
	}
	if($_REQUEST['reprogramacaosolicitada'] == "1") {
		$where[] = "dp.dopid IN ( SELECT dopid FROM par.documentoparreprogramacao WHERE dprstatus = 'P' AND dprvalidacao NOT IN ('f') )";
	}
	if($_REQUEST['reprogramacaosubsolicitada'] == "1") {
		$where[] = "dp.dopid IN ( SELECT dopid FROM par.documentoparreprogramacaosubacao WHERE dpsstatus = 'P' AND dpsvalidacao NOT IN ('f') )";
	}

	if($_REQUEST['empenhado'] == "1") {
	$where[] = " 1 = (SELECT 	case when pp.prptipoexecucao = 'C' then
						            -- CASE WHEN SUM(COALESCE( es.eobpercentualemp,0)) = (count(es.empid) * 99) THEN 1 --100% EMPENHADO
						             CASE WHEN SUM(COALESCE( es.eobpercentualemp,0)) = (count(e.empid) * 99) THEN 1 --100% EMPENHADO
						            ELSE
						                        CASE WHEN SUM( COALESCE( es.eobpercentualemp,0) ) <> 0   THEN 3 --EMPENHADO PARCIAL
						                        ELSE  2 --NÃO FOI EMPENHADO
						                        END
						            END
								else
						          --  CASE WHEN SUM(COALESCE( es.eobpercentualemp,0)) = (count(es.empid) * 100) THEN 1 --100% EMPENHADO
						          CASE WHEN SUM(COALESCE( es.eobpercentualemp,0)) = (count(e.empid) * 100) THEN 1 --100% EMPENHADO
						            ELSE
						                        CASE WHEN SUM( COALESCE( es.eobpercentualemp,0) ) <> 0   THEN 3 --EMPENHADO PARCIAL
						                        ELSE  2 --NÃO FOI EMPENHADO
						                        END
						            END
						       	end AS empenho

					FROM par.processoparcomposicao pc
					INNER JOIN par.processopar pp on pp.prpid = pc.prpid
					INNER JOIN par.subacaodetalhe s on s.sbdid = pc.sbdid
					LEFT JOIN par.empenho e on e.empnumeroprocesso = pp.prpnumeroprocesso AND e.empsituacao = '2 - EFETIVADO' and empstatus = 'A'
					LEFT JOIN par.empenhosubacao es on es.empid = e.empid  and es.eobstatus = 'A' AND es.eobano = s.sbdano AND es.sbaid = s.sbaid
					WHERE pc.ppcstatus = 'A' and pp.prpnumeroprocesso = p.prpnumeroprocesso
					GROUP BY pp.prpid, pp.prptipoexecucao)";
	}
	if($_REQUEST['empenhado'] == "2") {
		$where[] = " 2 = (SELECT 	case when pp.prptipoexecucao = 'C' then
							            CASE WHEN SUM(COALESCE( es.eobpercentualemp,0)) = (count(es.empid) * 99) THEN 1 --100% EMPENHADO
							            ELSE
							                        CASE WHEN SUM( COALESCE( es.eobpercentualemp,0) ) <> 0   THEN 3 --EMPENHADO PARCIAL
							                        ELSE  2 --NÃO FOI EMPENHADO
							                        END
							            END
									else
							            CASE WHEN SUM(COALESCE( es.eobpercentualemp,0)) = (count(es.empid) * 100) THEN 1 --100% EMPENHADO
							            ELSE
							                        CASE WHEN SUM( COALESCE( es.eobpercentualemp,0) ) <> 0   THEN 3 --EMPENHADO PARCIAL
							                        ELSE  2 --NÃO FOI EMPENHADO
							                        END
							            END
							        end AS empenho

						FROM par.processoparcomposicao pc
						INNER JOIN par.processopar pp on pp.prpid = pc.prpid
						INNER JOIN par.subacaodetalhe s on s.sbdid = pc.sbdid
						LEFT JOIN par.empenho e on e.empnumeroprocesso = pp.prpnumeroprocesso AND e.empsituacao = '2 - EFETIVADO' and empstatus = 'A'
						LEFT JOIN par.empenhosubacao es on es.empid = e.empid  and es.eobstatus = 'A' AND es.eobano = s.sbdano AND es.sbaid = s.sbaid
						WHERE 
						pc.ppcstatus = 'A' and
						pp.prpnumeroprocesso = p.prpnumeroprocesso
						GROUP BY pp.prpid, pp.prptipoexecucao) ";
	}
	if($_REQUEST['empenhado'] == "3"){
		$where[] = " 3 IN (select
							CASE WHEN ref.vrlreforco IS NOT NULL THEN
								CASE WHEN sum(vve.vrlempenhocancelado+ref.vrlreforco) < (SELECT par.recuperavalorvalidadossubacaoporano(es.sbaid, es.eobano))::numeric(20,2) THEN 3 --EMPENHADO PARCIAL
								END
							ELSE
								CASE WHEN sum(vve.vrlempenhocancelado) < (SELECT par.recuperavalorvalidadossubacaoporano(es.sbaid, es.eobano))::numeric(20,2) THEN 3 --EMPENHADO PARCIAL
								END
							END AS empenho
						FROM
							par.processopar prp
						INNER JOIN par.empenho emp ON emp.empnumeroprocesso = prp.prpnumeroprocesso AND emp.empcodigoespecie not in ('03', '13', '02', '04') and empstatus = 'A'
						inner join par.v_vrlempenhocancelado vve on vve.empid = emp.empid
						INNER JOIN par.empenhosubacao es ON es.empid = emp.empid and eobstatus = 'A'
						left join (select empnumeroprocesso, empidpai, sum(empvalorempenho) as vrlreforco, empcodigoespecie 
									from par.empenho
									where empcodigoespecie in ('02') and empstatus = 'A'
									group by 
										empnumeroprocesso,
										empcodigoespecie,
										empidpai) as ref on ref.empidpai = emp.empid
						WHERE
							prp.prpnumeroprocesso = p.prpnumeroprocesso
						GROUP BY
							es.sbaid, es.eobano, ref.vrlreforco)";
	}
	if($_REQUEST['tipoExecucao'] == "C") {
		$where[] = "p.prptipoexecucao = 'C'";
	}

	if($_REQUEST['tipoExecucao'] == "T") {
		$where[] = "p.prptipoexecucao = 'T'";
	}
	if($_REQUEST['ptsid'][0]) {
		$innerPropostatiposubacao = "LEFT JOIN par.subacao s 
										INNER join par.propostatiposubacao pts on pts.frmid = s.frmid
					        		ON ems.sbaid = s.sbaid";
		if( $_REQUEST['ptsid_campo_excludente'] == 1 ){
			$where[] = "ems.sbaid NOT IN ( select su.sbaid from par.propostasubacao ps
												inner join par.subacao su on su.frmid = ps.frmid
											where ps.ptsid in (". implode($_REQUEST['ptsid'], ","). ") )";
		} else {
			$where[] = "pts.ptsid IN ( ". implode($_REQUEST['ptsid'], ","). ")";
		}
	}else{
		$innerPropostatiposubacao = " inner join par.subacao s on s.sbaid = ems.sbaid ";
	}

	if($_REQUEST['termogerado'] == "1") {
		if($_REQUEST['tpdcod']) {
			$where[] = "dp.dopid in (select dopid from par.documentopar dp1
	            			inner join par.modelosdocumentos md on md.mdoid = dp1.mdoid and md.mdostatus = 'A'
	            			where md.tpdcod = {$_REQUEST['tpdcod']} ) ";
		}

		if($_REQUEST['mdoid']) {
			$where[] = "dp.mdoid = {$_REQUEST['mdoid']}";
		}
	} else if($_REQUEST['termogerado'] == "2") {
		if( $_REQUEST['tpdcod'] || $_REQUEST['mdoid'] ){
			if($_REQUEST['tpdcod']) {
				$where[] = "dp.dopid not in (select dopid from par.documentopar dp1
		            			inner join par.modelosdocumentos md on md.mdoid = dp1.mdoid and md.mdostatus = 'A'
		            			where md.tpdcod = {$_REQUEST['tpdcod']} ) ";
			}
	
			if($_REQUEST['mdoid']) {
				$where[] = "dp.mdoid not in ({$_REQUEST['mdoid']})";
			}
		} else {
			$where[] = "p.prpid not in (select distinct coalesce(prpid,0) from par.documentopar WHERE dopstatus = 'A')";
		}
	} else {
		if($_REQUEST['tpdcod']) {
			$where[] = "dp.dopid in (select dopid from par.documentopar dp1
	            			inner join par.modelosdocumentos md on md.mdoid = dp1.mdoid and md.mdostatus = 'A'
	            			where md.tpdcod = {$_REQUEST['tpdcod']} ) ";
		}

		if($_REQUEST['mdoid']) {
			$where[] = "dp.mdoid = {$_REQUEST['mdoid']}";
		}
	}
	if($_REQUEST['dopusucpfvalidacaofnde'] == 't') {
		$where[] = "dp.dopid in (select distinct dp.dopid from par.modelosdocumentos md
								    inner join par.documentopar dp on dp.mdoid = md.mdoid and dp.dopstatus = 'A'
								where
								    md.mdoqtdvalidacao > 1
								    and md.mdoqtdvalidacao = (select count(dv.dpvcpf) from par.documentoparvalidacao dv where dv.dopid = dp.dopid and dv.dpvstatus = 'A')
								    and md.mdostatus = 'A')";
	}
	if($_REQUEST['dopusucpfvalidacaofnde'] == 'f') {
		$where[] = "dp.dopid in (select distinct dp.dopid from par.modelosdocumentos md
										inner join par.documentopar dp on dp.mdoid = md.mdoid and dp.dopstatus = 'A'
										inner join par.documentoparvalidacao dv on dv.dopid = dp.dopid and dv.dpvstatus = 'A'
									where
										md.mdoqtdvalidacao = 1
										and md.mdoqtdvalidacao = (select count(dv.dpvcpf) from par.documentoparvalidacao dv where dv.dopid = dp.dopid and dv.dpvstatus = 'A')
									    and md.mdostatus = 'A')";
	}
	if($_REQUEST['dopusucpfvalidacaogestor'] == 't') {
		$where[] = "dp.dopid IN (select dopid from par.documentoparvalidacao where dpvdatavalidacao is not null and dpvstatus = 'A')";
	}
	if($_REQUEST['dopusucpfvalidacaogestor'] == 'f') {
		$where[] = "dp.dopid not in (select dopid from par.documentoparvalidacao where dpvdatavalidacao is not null and dpvstatus = 'A')";
	}

		if($_REQUEST['esfera']) {
			switch($_REQUEST['esfera']) {
				case 'Municipal':
					$where[] = "iu.itrid = 2";
					break;
				case 'Estadual':
					$where[] = "CASE WHEN iu.estuf = 'DF' THEN iu.itrid = 2 ELSE iu.itrid = 1 END AND iu.muncod IS NULL";
					break;
			}
		}

	if($_REQUEST['vigencia']){
		$where[] = "dp.dopdatafimvigencia ilike '".substr($_REQUEST['vigencia'],5, 2)."/".substr($_REQUEST['vigencia'],0, 4)."'";
	}

	$imagem = "<img style=\"cursor:pointer\" id=\"img_dimensao_' || p.prpid || '\" src=\"/imagens/mais.gif\" onclick=\"carregarListaDocumentoPar(this.id,\'' || p.prpid || '\');\" border=\"0\" />";

	$consultaDocumenta  = "'<a href=\"#\" onclick=\"tramitar(\''||p.prpdocumenta||'\');\"><img src=\"../imagens/consultar.gif\" style=\"cursor:pointer;\" title=\"Consultar histórico do documento no sistema Documenta do FNDE\" border=\"0\"></a>'";
	$insereDocumenta  = "'<img src=\"../imagens/consultar.gif\" style=\"cursor:pointer;\" onclick=\"cadastraDocumenta(\''||p.prpnumeroprocesso||'\', \'documentoPar\');\" title=\"Consultar histórico do documento no sistema Documenta do FNDE\" border=\"0\">'";

	$perfil = pegaArrayPerfil($_SESSION['usucpf']);

	if(	in_array(PAR_PERFIL_GERADOR_DOCUMENTO, $perfil) ||
		in_array(PAR_PERFIL_SUPER_USUARIO, $perfil) ||
		in_array(PAR_PERFIL_ADMINISTRADOR, $perfil)
	){
		$incluiMinuta = "'&nbsp;<img src=\"../imagens/gif_inclui.gif\" style=cursor:pointer; onclick=\"window.open(\'par.php?modulo=principal/minuta&acao=A&prpid='||p.prpid||'\',\'minuta\',\'scrollbars=yes,status=no,toolbar=no,menubar=no,location=no,fullscreen=yes\');\"></center>'";
	}else{
		$incluiMinuta = "''";
	}

	if($_REQUEST['anoprocesso']){
		$condicaoProcessoPar = " and   substring(prpnumeroprocesso,12,4) = '".$_REQUEST['anoprocesso']."'";
	}

	if($_REQUEST['dopid']){
		$where[]= "(dopnumerodocumento = {$_REQUEST['dopid']} OR dopid = {$_REQUEST['dopid']})";
	}

	if($_REQUEST['reformulacao']){
		if($_REQUEST['reformulacao'] == 't'){
			$where[]= "p.prpid IN (SELECT DISTINCT p.prpid FROM par.documentopar d INNER JOIN par.processopar p ON p.prpid = d.prpid WHERE dopreformulacao = true)";
		} elseif($_REQUEST['reformulacao'] == 'f'){
			$where[]= "p.prpid IN (SELECT DISTINCT p.prpid FROM par.documentopar d INNER JOIN par.processopar p ON p.prpid = d.prpid WHERE dopreformulacao = false)";
		}
	}

	if($_REQUEST['exportarexcel'] != 'sim'){
		$acoes = "case when dp.dopid is not null then
					'<center>$imagem</center>' else '' end as mais,
					CASE WHEN p.prpfinalizado IS NULL THEN
				   	'<center>'||
						case when p.prpdocumenta is not null then $consultaDocumenta
							else $insereDocumenta
						end ||  
						$incluiMinuta ELSE '' END as acoes,";

		$montarTR = " || '</td></tr>
		            	<tr style=\"display:none\" id=\"listaDocumentos_' || p.prpid || '\" >
		            		<td id=\"trV_' || p.prpid || '\" colspan=8 ></td>
		            </tr>' ";
		$colunaAnexos = "CASE WHEN dp.dopid is not null 
				    		THEN '<center><img onclick=\"enviarAnexo2('||p.prpid||');\" style=\"cursor:pointer;\" src=\"../imagens/reject.png\"></center>' 
				    		ELSE '<center> - </center>' 
				    	END as anexopublicado,";
	}

	$pendenciaObras = $_REQUEST['pendenciaObras'];
    if($pendenciaObras == '1'){ // com pendencias
        $where[] = " iu.itrid = '2' ";
        $where[] = " iu.muncod in (select coalesce(muncod, '') from obras2.vm_pendencia_obras where empesfera = 'M')  ";
    }

    if($pendenciaObras == '2'){ // sem pendencias
        $where[] = " iu.itrid = '2' ";
        $where[] = " iu.muncod not in (select coalesce(muncod, '') from obras2.vm_pendencia_obras where empesfera = 'M')  ";
    }

    $pendenciaObrasUF = $_REQUEST['pendenciaObrasUF'];
    if($pendenciaObrasUF == '1'){ // com pendencias
        $where[] = " iu.itrid = '1' ";
        $where[] = " iu.estuf in (select coalesce(estuf, '') from obras2.vm_pendencia_obras where empesfera = 'E')  ";
    }

    if($pendenciaObrasUF == '2'){ // sem pendencias
        $where[] = " iu.itrid = '1' ";
        $where[] = " iu.estuf not in (select coalesce(estuf, '') from obras2.vm_pendencia_obras where empesfera = 'E')  ";
    }

    if($_REQUEST['exportarexcel'] == 'sim')
	{
		$processo = "
				substring(p.prpnumeroprocesso from 1 for 5)||'.'||
		   		substring(p.prpnumeroprocesso from 6 for 6)||'/'||
		   		substring(p.prpnumeroprocesso from 12 for 4)||'-'||
		   		substring(p.prpnumeroprocesso from 16 for 2) || '' as numeroprocesso, 
				";
		
		$ufMunicipio =
		"
			CASE WHEN iu.estuf is null THEN iu.mun_estuf ELSE iu.estuf end as estuf,
			m.mundescricao as mundescricao,
		";
			
	}
	else
	{
		$processo = "(
                CASE WHEN (select count(*) as qtd from painel.dadosfinanceirosconvenios dfi where dfi.dfiprocesso = p.prpnumeroprocesso) > 0 THEN
                	'<span class=\"processoDetalhes processo_detalhe\" >'
                ELSE
                	'<span class=\"processo_detalhe\" >'
                END
                ) ||
				substring(p.prpnumeroprocesso from 1 for 5)||'.'||
		   		substring(p.prpnumeroprocesso from 6 for 6)||'/'||
		   		substring(p.prpnumeroprocesso from 12 for 4)||'-'||
		   		substring(p.prpnumeroprocesso from 16 for 2) || '</span>' as numeroprocesso, 
				";
		
		$ufMunicipio =
		"
			CASE WHEN iu.estuf is null THEN iu.mun_estuf ELSE '<a onclick=\"abrePlanoTrabalho(''estado'', '''||iu.estuf||''', '''','''|| iu.inuid ||''')\" style=cursor:pointer; >'||iu.estuf||'</a>' end as estuf,
			CASE WHEN iu.estuf is null THEN '<a onclick=\"abrePlanoTrabalho(''municipio'', '''|| iu.mun_estuf ||''', '''|| iu.muncod ||''', '''|| iu.inuid ||''')\" style=cursor:pointer; >'||m.mundescricao||'</a>' ELSE ' - ' END as mundescricao,				
		";
	}
    
	$sql = "SELECT DISTINCT
				$acoes
				{$processo}
				p.prpdocumenta,
				{$ufMunicipio}
				$colunaAnexos
				( SELECT sum(saldo) FROM par.vm_dadosempenhos  where processo =  p.prpnumeroprocesso ) as valorempenhado,
				COUNT(ems.sbaid) as qtdsubacaoempenhada,
				CASE WHEN p.prptipoexecucao = 'C' THEN 'Convênio' ELSE 'Termo' END $montarTR as tipo
		FROM par.instrumentounidade iu
		INNER JOIN 	par.processopar p ON p.inuid = iu.inuid ".$condicaoProcessoPar."
		LEFT JOIN 	territorios.municipio m ON m.muncod = iu.muncod
		LEFT JOIN 	territorios.estado e ON e.estuf = iu.estuf
		LEFT JOIN 	par.documentopar dp on dp.prpid = p.prpid AND dopstatus = 'A'
                LEFT JOIN 	par.modelosdocumentos md on md.mdoid = dp.mdoid
                INNER JOIN 	par.empenho emp on emp.empnumeroprocesso = p.prpnumeroprocesso and empstatus = 'A'
                LEFT JOIN 	par.empenhosubacao ems on emp.empid=ems.empid and eobstatus = 'A' ".$innerPropostatiposubacao."
		".(($where)?" WHERE ".implode(" AND ", $where):"").
		"
		GROUP BY dp.dopid, p.prpfinalizado, p.prpdocumenta, iu.estuf, iu.mun_estuf, m.mundescricao, p.prpnumeroprocesso, p.prptipoexecucao, p.prpid, iu.inuid
		ORDER BY estuf, mundescricao";

 	//ver(simec_htmlentities($sql),d);

	if($_REQUEST['exportarexcel'] == 'sim'){
		ob_clean();
		$cabecalho = array("Nº Processo", "Nº Documenta","UF","Município", "Valor empenhado(R$)","QTD subações","Tipo");
		header ( "Expires: Mon, 1 Apr 1974 05:00:00 GMT");
		header ( "Last-Modified: " . gmdate("D,d M YH:i:s") . " GMT" );
		header ( "Pragma: no-cache" );
		header ( "Content-type: application/xls; name=SIMEC_RelatDocente".date("Ymdhis").".xls");
		header ( "Content-Disposition: attachment; filename=SIMEC_RelatDocente".date("Ymdhis").".xls");
		header ( "Content-Description: MID Gera excel" );
		$db->monta_lista_tabulado($sql,$cabecalho,1000000,5,'N','100%',$par2);
		exit;

	} else {
        // Só monta lista quando clicar em pesquisar
        if(isset($_REQUEST['estuf'])){
            $cabecalho = array("&nbsp;","&nbsp;","Nº Processo", "Nº Documenta", "UF", "Município", "Anexo de publicação", "Valor empenhado(R$)","QTD subações","Tipo");
            $db->monta_lista($sql,$cabecalho,100,5,'N','center','N','formprocesso');
        }
    }
?>
