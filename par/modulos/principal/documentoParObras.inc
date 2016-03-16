<?php
if ($_REQUEST ['reqGeraEx'] == 'termoEx') {
	
	global $db;
	
	$retorno = geraTermoEx ( $_REQUEST ['dopid'], $_REQUEST ['dopdatafimvigenciaex'], 2 );
	
	if ($retorno == '1') {
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
	
	exit ();
}
function regerarTermo() {
	global $db;
	$sql = "SELECT
                    proid, dopstatus, dopdiasvigencia, dopdatainicio, dopdatafim,
                    mdoid, dopdatainclusao, usucpfinclusao, dopdataalteracao, usucpfalteracao, dopjustificativa,
                    dopdatavalidacaofnde, dopusucpfvalidacaofnde, dopdatavalidacaogestor, dopusucpfvalidacaogestor,
                    dopusucpfstatus, dopdatastatus, dopdatapublicacao, doppaginadou, dopnumportaria, proid,
                    dopreformulacao, dopidpai, dopdatainiciovigencia, dopdatafimvigencia
            FROM 
                    par.documentopar 
            WHERE 
                    dopid = {$_REQUEST['dopid']} 
                    AND dopstatus = 'A'";
	
	$arrDadosDoc = $db->pegaLinha($sql);
	$arrDadosDoc = $arrDadosDoc ? $arrDadosDoc : array ();
	
	if ($arrDadosDoc['proid']) {
		$processo = $db->pegaUm("select pronumeroprocesso from par.processoobraspar where proid = {$arrDadosDoc['proid']}");
		verificaEmpenhoSigef($processo);
	}
        
        #Se o Botão for Reformular, Devera ser setado o Termo de Compromisso - Reformulado = 103 3 o Modelo de documento 85.
        #Solicitante Thiago: 25/09 (Sexta-feira final de tarde) - Jair Foro
        if ($_REQUEST['tipoRegera'] == 'reformular') {
            $arrDadosDoc['mdoid'] = 85;
        }
	
	if ($arrDadosDoc ['mdoid']) {
            $sql = "SELECT mdoconteudo, tpdcod, mdodocumentoex, mdotipovinculado FROM par.modelosdocumentos WHERE mdostatus = 'A' AND mdoid = " . $arrDadosDoc ['mdoid'];
            $arrModelo = $db->pegaLinha($sql);

            if ($arrModelo['mdodocumentoex'] == 't') {
               $sql = "SELECT 
                           mdoconteudo, 
                           tpdcod
                       FROM par.modelosdocumentos 
                       WHERE mdostatus = 'A' AND mdoid = " . $arrModelo['mdotipovinculado'];
               $arrModeloPai = $db->pegaLinha ( $sql );
               $mdoconteudo = $arrModeloPai['mdoconteudo'];
               $tpdcod = $arrModeloPai['tpdcod'];
               $arrDadosDoc['mdoid'] = $arrModelo['mdotipovinculado'];

           } else {
               $mdoconteudo = $arrModelo['mdoconteudo'];
               $tpdcod = $arrModelo['tpdcod'];
           }
	}
	
	$sql = "SELECT preid as chk FROM par.termocomposicao WHERE dopid = " . $_REQUEST['dopid'];
	$subacoes = $db->carregarColuna($sql);
	
	$post = array (
			'mdoid' => $arrDadosDoc['mdoid'],
			'proid' => $arrDadosDoc['proid'],
			'dopdatainiciovigencia' => $arrDadosDoc['dopdatainiciovigencia'],
			'$dopdatafimvigencia' => $arrDadosDoc['dopdatafimvigencia'],
			'tpdcod' => $tpdcod,
			'dopid' => $_REQUEST['dopid'],
			'chk' => $subacoes,
                        'tipoRegera' => $_REQUEST['tipoRegera']
	);
	
	$_SESSION['par']['cronogramaFinal'] = $arrDadosDoc['dopdatafimvigencia'];
	
	$doptexto = alteraMacrosMinutaObras($mdoconteudo, $arrDadosDoc['proid'], $post);
	
	$arrDadosDoc['regerar'] = true;
	$post['regerar'] = true;
	        
        if($_SESSION['par']['totalVLR']){
            $arrDadosDoc['dopvalor'] = $_SESSION['par']['totalVLR'];
            unset($_SESSION['par']['totalVLR']);
        }
	
	$dopidNovo = salvarDadosMinutaObras($post, $doptexto, $_REQUEST['dopid']);
//	ver($_REQUEST, $arrDadosDoc, $post, $dopidNovo, d);
	
	foreach ($subacoes as $preid) {
		$sql = "INSERT INTO par.termocomposicao( preid, dopid)
                            VALUES ( $preid, $dopidNovo)";
		$db->executar($sql);
		$db->commit();
	}
	
	// Muda o Status da Obra para Aprovada
	 
	if ($_REQUEST['tipoRegera'] == 'prorrogacaoVigencia') {
		include_once APPRAIZ . 'includes/workflow.php';
		
		$sqlPreoObra = "SELECT 
                                pop.preid 	
                        FROM obras.preobraprorrogacao pop
                        INNER JOIN par.termocomposicao tc ON tc.preid = pop.preid
                        WHERE tc.dopid = {$_REQUEST['dopid']} AND pop.popstatus = 'A' AND pop.popvalidacao = 't'
                        ORDER BY popid ASC";
		
		$arrPreObra = $db->carregar ($sqlPreoObra);
		if ($arrPreObra) {
        	foreach ($arrPreObra as $obra) {
            	$docid = $db->pegaUm ( "SELECT docid FROM obras.preobra WHERE preid='" . $obra['preid'] . "'" );

                        $sql = "SELECT tooid FROM obras.preobra WHERE preid = {$obra['preid']}";
                        $tooid = $db->pegaUm($sql);

                        // Se a obra for PAC
                        if ($tooid == 1) {
                        	$esdidorigem = WF_TIPO_OBRA_AGUARDANDO_PRORROGACAO;
                        } else { // Sea obra não for PAC
                            $esdidorigem = WF_PAR_OBRA_AGUARDANDO_PRORROGACAO;
                        }

                        $esdiddestino = $db->pegaUm ( "SELECT esdidorigem FROM obras.preobraprorrogacao WHERE popstatus = 'A' AND preid=" . $obra ['preid'] );
                        $aedid = $db->pegaUm ( "SELECT aedid FROM workflow.acaoestadodoc WHERE esdidorigem = " . $esdidorigem . " AND esdiddestino = " . $esdiddestino );

                        if($aedid == '' && $esdidorigem != $esdiddestino){
                        	
                        	$sql = "SELECT esddsc FROM workflow.estadodocumento WHERE esdid = $esdiddestino";
                        	$esddsc = $db->pegaUm( $sql );
                        	
                        	$sql = "INSERT INTO workflow.acaoestadodoc
		                        		(esdidorigem, esdiddestino, aeddscrealizar, aedstatus, aeddscrealizada, esdsncomentario, aedvisivel, aedcodicaonegativa)
		                        	VALUES
		                        		($esdidorigem, $esdiddestino, 'Enviar para $esddsc', 'A', 'Enviada para $esddsc', true, false, false )
		                        	RETURNING
		                        		aedid";
                        
                        	$aedid = $db->pegaUm($sql);
                        }
                        
                        wf_alterarEstado($docid, $aedid, 'teste', $d = array (
                            'preid' => $obra ['preid'] 
                        ) );
                    }
                }
	}
	
	$db->commit();
	
	enviaEmailNovoTermoObrasPAR ($_REQUEST ['dopid'], 'regerar');
	
	if ($_REQUEST['tipoRegera'] == 'prorrogacaoVigencia') {
		echo "
		<script>
			if( window.opener ){
				window.close();
				window.opener.location.reload(); 
			}else{
				window.location.href = window.location.href;
			}
		</script>";
	}else {
            $strRegera = ($_REQUEST['tipoRegera'] == 'reformular') ? 'Reformulado' : 'Regerado';
		echo "
		<script>
                    alert('Termo $strRegera com Sucesso!');
                    window.location.href = window.location.href;
		</script>";
        }
	exit ();
}
function formRegeraTermo() {
	global $db;

	extract ( $_REQUEST );
	?>
<p><strong>Deseja <?php echo ($tipoRegera == 'reformular') ? 'Reformular' : 'Regerar'; ?> este termo?</strong></p>
<form method="post" name="formRegerar" id="formRegerar">
	<input type="hidden" name="req" id="reqRegera" /> 
        <input type="hidden" name="dopid" id="dopidRegera" value="<?= $dopidRegera ?>" /> 
        <input type="hidden" name="tipoRegera" id="tipoRegera" value="<?= $tipoRegera ?>" />
    </form>
<?php
}
function formGeraEx() {
	global $db;
	
	extract ( $_REQUEST );
	?>
<center>
	<b>Deseja gerar este Termo EX?</b>
</center>
<form method="post" name="formEx" id="formEx">
	<input type="hidden" name="reqGeraEx" id="reqGeraEx" value="" /> <input
		type="hidden" name="dopid" id="dopidex" value="<?= $dopidEx ?>" />
        Fim da Vigência: <?= campo_texto('dopdatafimvigenciaex', 'N', 'S', '', 10, 7, '##/####', ''); ?> (mm/aaaa)
    </form>
<?php
}
function formVizualizaDocumento() {
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
				dopid = " . $_REQUEST ['dopid'];
	
	$html = $db->pegaLinha ( $sql );
	
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
				dp.dopid = " . $_REQUEST ['dopid'];
	
	$dadosValidacao = $db->carregar ( $sql );
	
	$textoValidacao = "";
	
	if (is_array ( $dadosValidacao )) {
		foreach ( $dadosValidacao as $dv ) {
			$cpfvalidacao [] = $dv ['cpfgestor'];
			$textoValidacao .= "<b>Validado por " . $dv ['usunome'] . " - CPF: " . formatar_cpf ( $dv ['usucpf'] ) . " em " . $dv ['data'] . " </b><br>";
		}
	}
	
	$cpfvalidacao = $cpfvalidacao ? $cpfvalidacao : array ();
	
	$perfil = pegaArrayPerfil ( $_SESSION ['usucpf'] );
	function monta_cabecalho_relatorio_par($data = '', $largura) {
		global $db;
		
		$data = $data ? $data : date ( 'd/m/Y' );
		
		$cabecalho = '<table width="' . $largura . '%" border="0" cellpadding="0" cellspacing="0" class="notscreen1 debug">' . '	<tr bgcolor="#ffffff">' . '		<td valign="top" align="center"><img src="../imagens/brasao.gif" width="45" height="45" border="0">' . '			<br><b>MINISTÉRIO DA EDUCAÇÃO<br/>' . '			FUNDO NACIONAL DE DESENVOLVIMENTO DA EDUCAÇÃO</b> <br />' . '		</td>' . '	</tr>' . '</table><br><br>';
		
		return $cabecalho;
	}
	
	echo "<div style=\"background-color:white;\">";
	
	$cabecalhoBrasao .= "	<table width=\"95%\" cellspacing=\"1\" cellpadding=\"5\" border=\"0\" align=\"center\" >";
	$cabecalhoBrasao .= "		<tr>" . "			<td colspan=\"100\">" . ($html ['tpdcod'] == '101' ? monta_cabecalho_relatorio_par ( '29/11/2011', '100' ) : monta_cabecalho_relatorio_par ( '', '100' )) . "			</td>" . "		</tr>
						  	</table>";
	echo $cabecalhoBrasao;
	
	$doptexto = pegaTermoCompromissoArquivo( $_REQUEST['dopid'], '');
	
	?>
<table id="termo" width="95%" align="center" border="0" cellpadding="3" cellspacing="1" style="background-color: white;">
	<tr>
		<td style="font-size: 12px; font-family: arial;">
			<div>
				<?php echo simec_html_entity_decode($doptexto); ?>
			</div>
		</td>
	</tr>
</table>
<table id="termo" align="center" border="0" cellpadding="3" cellspacing="1" style="background-color: white;">
	<tr <?=$displayValidado; ?> style="text-align: center;">
		<td>
			<b>VALIDAÇÃO ELETRÔNICA DO DOCUMENTO<b><br>
			<br> <?=$textoValidacao; ?>
		</td>
	</tr>
</table>
</div>
<?php
}

if ($_REQUEST ['req']) {
	if ($_REQUEST ['req'] != 'inseriranexotermoprocesso') {
		$_REQUEST ['req'] ();
		die ();
	}
}

if ($_POST['tipodocumento']) {
    $obModelosDocumentosControle = new ModelosDocumentosControle();
    $sqlModelosDoc = $obModelosDocumentosControle->carregarModeloDocumento($_POST['tipodocumento'], TRUE, '', array('mdovisivel' => 'S'));
    echo $db->monta_combo("mdoid", $sqlModelosDoc, 'S', 'Selecione', 'verificaMoceloDoc', '', '', 400, 'N', 'mdoid', '', $_POST['mdoid']);
    exit();
}

if ($_REQUEST ['listadocumento']) {
	header ( 'content-type: text/html; charset=ISO-8859-1' );
	
	$acoes = "'<img src=\"../imagens/alterar.gif\" style=\"cursor:pointer;\" onclick=\"carregaTermoMinuta('||mi.dopid||','|| mi.proid ||');\" border=\"0\">' ";
	
	$perfil = pegaArrayPerfil ( $_SESSION ['usucpf'] );
	if (in_array ( PAR_PERFIL_SUPER_USUARIO, $perfil ) || in_array ( PAR_PERFIL_ADMINISTRADOR, $perfil ) || in_array ( PAR_PERFIL_GERADOR_DOCUMENTO, $perfil )) {
		$AcExluir = "<img src=\"../imagens/excluir.gif\" style=\"cursor:pointer;\" onclick=\"excluiTermoMinuta('||mi.dopid||','|| mi.proid ||');\" border=\"0\">";
		if (in_array ( PAR_PERFIL_SUPER_USUARIO, $perfil ) || in_array ( PAR_PERFIL_ADMINISTRADOR, $perfil )) {
			$exluirSuper = "CASE WHEN 
									(
									SELECT DISTINCT TRUE
									FROM par.vm_documentopar_ativos dop1
									INNER JOIN par.termocomposicao 	tec1 ON tec1.dopid = dop1.dopid
									INNER JOIN par.empenhoobrapar 	eop1 ON eop1.preid = tec1.preid AND eop1.eobstatus = 'A'
									INNER JOIN par.pagamento 		pag1 ON pag1.empid = eop1.empid AND pag1.pagstatus = 'A' AND pag1.pagsituacaopagamento not ilike '%CANCELADO%'
									WHERE dop1.dopid = mi.dopid
									) 
								THEN ''
								ELSE '<img src=\"../imagens/excluir.gif\" style=\"cursor:pointer;\" onclick=\"excluiTermoMinuta('||mi.dopid||','|| mi.proid ||');\" border=\"0\">'
							END";
			$AcExluir = '';
		} else {
			$exluirSuper = "''";
		}
		$ExluirInativo = "<img src=\"../imagens/excluir_01.gif\" style=\"cursor:pointer;\" border=\"0\">";
	}
	
	$imagemMais = "<img style=\"cursor:pointer\" id=\"img_dimensao_' || mi.dopid || '\" src=\"/imagens/mais.gif\" onclick=\"carregarListaDocumentoParReformulado(this.id,\'' || mi.dopid || '\');\" border=\"0\" />";
	$imagemRef = "<img src=../imagens/restricao_ico.png class=btnReformular id=' || mi.dopid || ' attr=reformular style=cursor:pointer; title=\"Termo Reformulado\">";
	
	$botaoGerarTermoEX = "''";
	if (in_array ( PAR_PERFIL_SUPER_USUARIO, $perfil ) || in_array ( PAR_PERFIL_ADMINISTRADOR, $perfil )) {
		$dadosDocumentoEX = botaoGerarTermoEX ( $_REQUEST ['proid'], 2 );
		
		if ($dadosDocumentoEX ['mdoid_ex']) {
			$botaoGerarTermoEX = "'<img style=\"cursor:pointer\" id=\"' || mi.dopid || '\" src=\"/imagens/ex-oficio.png\" class=termoEx title=\"Gerar termo Ex Ofício\" />'";
		}
	}
	
	$sql = "SELECT profinalizado FROM par.processoobraspar WHERE proid = " . $_REQUEST ['proid'];
	$finalizado = $db->pegaUm ( $sql );
	
	$regerar = "
		CASE WHEN mi.dopprorrogacaovigenciaobra IS FALSE THEN
        	'<img src=../imagens/refresh.gif style=cursor:pointer; id=' || dopid || ' class=regerarTermo title=Regerar>'
        ELSE
        	'&nbsp;<span style=\"cursor:pointer;\" attr=\"prorrogacaoVigencia\" title=\"Regerar prorrogação de vigência\" class=\"glyphicon glyphicon-calendar regerarTermo\" id=' || dopid || '></span>'
        END";
	
	if ($finalizado == true) {
		$imagemMais = "";
		$imagemRef = "";
		$exluirSuper = "''";
		$ExluirInativo = "";
		$botaoGerarTermoEX = "''";
		$AcExluir = "";
		$regerar = "''";
	}
	
	$sql = "SELECT 
				acao, documentos, 
				validado||' '||situacao||'</td></tr><tr style=\"display:none\" id=\"listaDocumentos2_' || dopid || '\" ><td id=\"trV2_' || dopid || '\" colspan=8 ></td></tr>' as situacao  
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
									INNER JOIN par.empenhoobrapar 	eop1 ON eop1.preid = tec1.preid AND eop1.eobstatus = 'A'
									INNER JOIN par.pagamento 		pag1 ON pag1.empid = eop1.empid AND pag1.pagstatus = 'A' AND pag1.pagsituacaopagamento not ilike '%CANCELADO%'
									WHERE dop1.dopid = mi.dopid
									)
								THEN '$imagemMais '||$exluirSuper||'$ExluirInativo'||$acoes||$botaoGerarTermoEX
								ELSE '$imagemMais '||$exluirSuper||'$AcExluir'||$acoes||$botaoGerarTermoEX
							END
						ELSE
							CASE WHEN 
									mo.mdoqtdvalidacao = (SELECT count(dv.dpvcpf) FROM par.documentoparvalidacao dv WHERE dv.dopid = mi.dopid AND dv.dpvstatus = 'A') OR
									(
									SELECT DISTINCT TRUE
									FROM par.documentopar dop1
									INNER JOIN par.termocomposicao 	tec1 ON tec1.dopid = dop1.dopid
									INNER JOIN par.empenhoobrapar 	eop1 ON eop1.preid = tec1.preid AND eop1.eobstatus = 'A'
									INNER JOIN par.pagamento 		pag1 ON pag1.empid = eop1.empid AND pag1.pagstatus = 'A' AND pag1.pagsituacaopagamento not ilike '%CANCELADO%'
									WHERE dop1.dopid = mi.dopid
									) 
								THEN $exluirSuper||'$ExluirInativo'||$acoes||$botaoGerarTermoEX
								ELSE $exluirSuper||'$AcExluir'||$acoes||$botaoGerarTermoEX
							END
					END ||
					$regerar || 
                                        CASE WHEN (
                                                SELECT DISTINCT 
                                                    pronumeroprocesso
                                                FROM execucaofinanceira.processos 
                                                WHERE tipid =  5 
                                                    AND pronumeroprocesso = (SELECT DISTINCT pronumeroprocesso FROM par.processoobraspar  where proid = mi.proid)) IS NOT NULL
                                            THEN  '$imagemRef'       
                                            ELSE ''
                                        END ||
					'</center>' as acao, 
				    mo.mdonome ||' - '|| dopnumerodocumento as documentos,
				    CASE WHEN (dopreformulacao = true) 
				    	THEN '<b><br>Em Reformulação</b>'||
				    		CASE WHEN (	SELECT COUNT(pop.preid) FROM par.processoobrasparcomposicao pop
										INNER JOIN obras.preobra p1 ON p1.preid = pop.preid
										WHERE pop.pocstatus = 'A' and pop.proid = mi.proid AND p1.prereformulacao = true) = 0 
								THEN ' - <input type=\"button\" value=\"Finalizar reformulação\" id=\"finalizar\" onclick=\"finaliza(\'' || mi.dopid || '\')\">'
								ELSE '' 
							END
						ELSE '<br>' 
					END as situacao,
					CASE WHEN mo.mdoqtdvalidacao = (SELECT COUNT(dv.dpvcpf) FROM par.documentoparvalidacao dv WHERE dv.dopid = mi.dopid AND dv.dpvstatus = 'A') 
						THEN 'Documento Validado'
	               	 	ELSE 'Documento não Validado' 
	               	END as validado,
	                mi.dopid
			FROM 
				par.vm_documentopar_ativos mi
		    LEFT  JOIN par.modelosdocumentos mo on mo.mdoid = mi.mdoid
		    INNER JOIN seguranca.usuario us on us.usucpf = mi.usucpfinclusao
			WHERE 
				mi.proid = " . $_REQUEST ['proid'] . " 
			) as foo";
	
	$cabecalho = array (
			"Acões",
			"Documento",
			"Situação" 
	);
	$db->monta_lista ( $sql, $cabecalho, 20, 4, 'N', 'Center' );
	
	exit ();
}

if ($_REQUEST ['listadocumentoReformulado']) {
	header ( 'content-type: text/html; charset=ISO-8859-1' );
	
	$perfil = pegaArrayPerfil ( $_SESSION ['usucpf'] );
	if (in_array ( PAR_PERFIL_SUPER_USUARIO, $perfil ) || in_array ( PAR_PERFIL_ADMINISTRADOR, $perfil ) || in_array ( PAR_PERFIL_GERADOR_DOCUMENTO, $perfil )) {
		$AcExluir = "<img src=\"../imagens/excluir.gif\" style=\"cursor:pointer;\" onclick=\"excluiTermoMinuta('||mi.dopid||','|| mi.proid ||');\" border=\"0\">";
		if (in_array ( PAR_PERFIL_SUPER_USUARIO, $perfil ) || in_array ( PAR_PERFIL_ADMINISTRADOR, $perfil )) {
			$exluirSuper = "<img src=\"../imagens/excluir.gif\" style=\"cursor:pointer;\" onclick=\"excluiTermoMinuta('||mi.dopid||','|| mi.proid ||');\" border=\"0\">";
			$AcExluir = '';
		} else {
			$exluirSuper = "''";
		}
		$ExluirInativo = "<img src=\"../imagens/excluir_01.gif\" style=\"cursor:pointer;\" border=\"0\">";
	}
	
	$imagemMais = "<img style=\"cursor:pointer\" id=\"img_dimensao_' || mi.dopid || '\" src=\"/imagens/mais.gif\" 
						onclick=\"carregarListaDocumentoParReformulado(this.id,\'' || mi.dopid || '\');\" border=\"0\" />";
	
	// $acoes = "'<img src=\"../imagens/alterar.gif\" style=\"cursor:pointer;\" onclick=\"carregaTermoMinuta('||mi.dopid||','|| mi.proid ||');\" border=\"0\">' ";
	$acoes = "'<img src=../imagens/icone_lupa.png style=cursor:pointer;  class=vizualizarDocumento id='|| mi.dopid ||' >' ";
	
	$sql = "SELECT	DISTINCT
				'<center>'||
				CASE WHEN mi.dopidpai IS NOT NULL
					THEN '$imagemMais&nbsp;'
					ELSE ''
				END
				||
				CASE WHEN dv.dpvcpf IS NOT NULL 
					THEN $acoes
					else $acoes
				END
				||'</center>' as acao, 
			    mo.mdonome ||' - '|| mi.dopnumerodocumento,
			    case when (dopreformulacao = true) then '<b>Em Reformulação</b>
			    	'||case when (select count(pop.preid) from par.processoobrasparcomposicao pop
									inner join obras.preobra p1 on p1.preid = pop.preid
								where pop.pocstatus = 'A' and pop.proid = mi.proid and p1.prereformulacao = true) = 0 then ' - <input type=\"button\" value=\"Finalizar reformulação\" id=\"finalizar\" onclick=\"finaliza(\'' || mi.dopid || '\')\">'
						else '' end
					else '' end ||  
			   '</td></tr>
		            	<tr style=\"display:none\" id=\"listaDocumentos2_' || mi.dopid || '\" >
		            		<td id=\"trV2_' || mi.dopid || '\" colspan=8 ></td>
		            </tr>'
			    as situacao
			From par.documentopar mi
		    Left Join par.modelosdocumentos mo on mo.mdoid = mi.mdoid
		    Inner Join seguranca.usuario us on us.usucpf = mi.usucpfinclusao
		    left join par.documentoparvalidacao dv on dv.dopid = mi.dopid and dv.dpvstatus = 'A'
			Where (mi.dopid IN ( SELECT dopidpai FROM par.documentopar mi2 WHERE mi2.dopid = " . $_REQUEST ['dopid'] . " )
					or mi.dopid IN ( SELECT dopidpai FROM par.documentopar mi2 WHERE mi2.dopid = " . $_REQUEST ['dopid'] . " )) 
				and mi.dopstatus IN ('I')";
	// ver(simec_htmlentities($sql),d);
	$cabecalho = array (
			"Acões",
			"Documento",
			"Situação" 
	);
	$db->monta_lista ( $sql, $cabecalho, 20, 4, 'N', 'Center' );
	
	exit ();
}

if ($_REQUEST ['excluidocumento']) {
	
// 	$sql = "update par.documentopar set dopstatus = 'I', dopusucpfstatus = '" . $_SESSION ['usucpf'] . "', dopdatastatus = NOW() WHERE dopid = " . $_REQUEST ['dopid'];
	$sql = "UPDATE par.documentopar SET dopstatus = 'E', dopusucpfstatus = '".$_SESSION['usucpf']."', dopdatastatus = NOW(), dopdataalteracao = now(), usucpfalteracao = '".$_SESSION['usucpf']."'
			WHERE dopnumerodocumento = (SELECT dopnumerodocumento FROM par.documentopar WHERE dopid = {$_REQUEST['dopid']} );";
	$db->executar ( $sql );
	$db->commit ();
	
	header ( 'content-type: text/html; charset=ISO-8859-1' );
	$acoes = "'<center><img src=\"../imagens/alterar.gif\" style=\"cursor:pointer;\" onclick=\"carregaTermoMinuta('||mi.dopid||','|| mi.proid ||');\" border=\"0\"> <img src=\"../imagens/excluir.gif\" style=\"cursor:pointer;\" onclick=\"excluiTermoMinuta('||mi.dopid||','|| mi.prpid ||');\" border=\"0\"></center>'";
	
	$sql = "SELECT
				$acoes as acao,
			    mo.mdonome
			FROM
				par.vm_documentopar_ativos mi
			    left join par.modelosdocumentos mo on mo.mdoid = mi.mdoid
			    inner join seguranca.usuario us on us.usucpf = mi.usucpfinclusao
			WHERE
				mi.proid = " . $_REQUEST ['proid'];
	
	$cabecalho = array (
			"Acões",
			"Documento" 
	);
	$dado = $db->pegaLinha ( $sql );
	
	if (! $dado) {
		echo 'recarrega';
	} else {
		$db->monta_lista ( $sql, $cabecalho, 20, 4, 'N', 'Center' );
	}
	
	exit ();
}

if ($_REQUEST ['requisicao'] == 'telaanexo') {
	telaAnexoObrasPar ( $_REQUEST );
	exit ();
}

if ($_REQUEST ['requisicao'] == 'inseriranexotermoprocesso') {
	inserirAnexoProcessoParObras ( $_REQUEST );
	exit ();
}

if ($_REQUEST ['requisicao'] == 'downloadanexotermo') {
	downloadAnexoTermoParObras ( $_REQUEST );
	exit ();
}

if ($_REQUEST ['requisicao'] == 'excluiranexotermo') {
	excluirAnexoParObras ( $_REQUEST );
	exit ();
}
function telaAnexoObrasPar($dados) {
	global $db;
	
	$sql = "SELECT a.arqid, a.arqnome||'.'||a.arqextensao as arquivo FROM par.processoobraspar p
			INNER JOIN public.arquivo a ON a.arqid = p.arqidanexodoc   
			WHERE proid = '" . $dados ['proid'] . "' AND p.prostatus = 'A' ";
	
	$arquivos = $db->carregar ( $sql );
	echo "<form method=post name=formulario id=formularioanexo enctype=multipart/form-data>";
	echo "<input type=hidden name=requisicao value=" . $dados ['req'] . ">";
	echo "<input type=hidden name=urlgo value=" . $dados ['urlgo'] . ">";
	echo "<input type=hidden name=proid value=" . $dados ['proid'] . ">";
	echo "<table align=\"center\" border=\"0\" class=\"tabela\" cellpadding=\"3\" cellspacing=\"1\">";
	echo "<tr>";
	echo "<td class=\"SubTituloDireita\">Anexo:</td>";
	echo "<td><input type=file name=arquivo id=arquivo " . (($arquivos [0]) ? "disabled" : "") . "></td>";
	echo "</tr>";
	if ($arquivos [0]) {
		foreach ( $arquivos as $arquivo ) {
			echo "<tr>";
			echo "<td class=\"SubTituloDireita\">Nome do arquivo:</td>";
			echo "<td>" . $arquivo ['arquivo'] . " <input type=button name=download value=Download onclick=window.location='par.php?modulo=principal/documentoParObras&acao=A&arqid=" . $arquivo ['arqid'] . "&requisicao=downloadanexotermo';> <input type=button name=excluir value=Excluir onclick=excluirAnexoTermo(" . $dados ['proid'] . ");></td>";
			echo "</tr>";
		}
	}
	echo "<tr>";
	echo "<td class=\"SubTituloCentro\" colspan=2>";
	if (! $arquivos [0] ['arqid'])
		echo "<input type=button name=salvar value=Salvar onclick=enviarAnexoTermo();>";
	echo "<input type=button name=fechar value=Fechar onclick=closeMessage();></td>";
	echo "</tr>";
	echo "</table>";
	echo "</form>";
}
function inserirAnexoProcessoParObras($dados) {
	global $db;
	
	if ($_FILES ['arquivo'] ['name']) {
		include_once APPRAIZ . "includes/classes/fileSimec.class.inc";
		$arrCampos = array ();
		$file = new FilesSimec ( "processoobraspar", $arrCampos, "par" );
		$file->setUpload ( "", "arquivo", false );
		$arqid = $file->getIdArquivo ();
	}
	
	if ($arqid) {
		
		$sql = "UPDATE par.processoobraspar SET arqidanexodoc = $arqid WHERE proid = {$dados['proid']}";
		
		$db->executar ( $sql );
		$db->commit ();
	}
	
	die ( "<script>
			alert('Anexado com sucesso.');
			window.location='" . md5_decrypt ( $dados ['urlgo'] ) . "';
		 </script>" );
}
function downloadAnexoTermoParObras($dados) {
	global $db;
	include_once APPRAIZ . "includes/classes/fileSimec.class.inc";
	$arrCampos = array ();
	$file = new FilesSimec ( "processoobraspar", $arrCampos, "par" );
	$file->getDownloadArquivo ( $dados ['arqid'] );
	die ();
}
function excluirAnexoParObras($dados) {
	global $db;
	
	$sql = "UPDATE par.processoobraspar SET arqidanexodoc = NULL WHERE proid = {$dados['proid']}";
	$db->executar ( $sql );
	$db->commit ();
	
	die ( "<script>
			alert('Anexo removido');
			window.location='" . md5_decrypt ( $dados ['urlgo'] ) . "';
		 </script>" );
}

if ($_REQUEST ['finalizaReformulacao']) {
	
	$sql = "SELECT
			  prpid, dopstatus, dopdiasvigencia, dopdatainicio, dopdatafim,
			  mdoid, dopdatainclusao, usucpfinclusao, dopdataalteracao, usucpfalteracao, dopjustificativa,
			  dopdatavalidacaofnde, dopusucpfvalidacaofnde, dopdatavalidacaogestor, dopusucpfvalidacaogestor,
			  dopusucpfstatus, dopdatastatus, dopdatapublicacao, doppaginadou, dopnumportaria, proid,
			  dopreformulacao, dopidpai
			FROM 
			  par.documentopar WHERE dopid = {$_REQUEST['dopid']} AND dopstatus = 'A' AND dopreformulacao = true";
	
	$arrDadosDoc = $db->pegaLinha ( $sql );
	$arrDadosDoc = $arrDadosDoc ? $arrDadosDoc : array ();
	
	$sql = "SELECT mdoconteudo, tpdcod FROM par.modelosdocumentos WHERE mdostatus = 'A' AND mdoid = " . $arrDadosDoc ['mdoid'];
	$arrModelo = $db->pegaLinha ( $sql );
	
	$mdoconteudo = $arrModelo ['mdoconteudo'];
	$tpdcod = $arrModelo ['tpdcod'];
	
	$sql = "SELECT preid as chk FROM par.termocomposicao WHERE dopid = " . $_REQUEST ['dopid'];
	$subacoes = $db->carregarColuna ( $sql );
	
	$post = array (
			'mdoid' => $arrDadosDoc ['mdoid'],
			'tpdcod' => $tpdcod,
			'dopid' => $_REQUEST['dopid'],
			'chk' => $subacoes 
	);
	
	$doptexto = alteraMacrosMinutaObras ( $mdoconteudo, $arrDadosDoc ['proid'], $post );
	
	$dopid = salvarDadosMinutaObras ( $arrDadosDoc, $doptexto );
	
	$sql = "UPDATE par.documentopar SET dopreformulacao = false WHERE proid = {$arrDadosDoc['proid']}";
	$db->executar ( $sql );
	echo $db->commit ();
	
	exit ();
}

if ($_REQUEST ['tipoacao'] != 'popup') {
	include APPRAIZ . "includes/cabecalho.inc";
	echo '<br>';
	$db->cria_aba ( $abacod_tela, $url, '' );
} else {
	echo '<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
		<link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>
		<script type="text/javascript" src="/includes/funcoes.js"></script>';
}

if ($_REQUEST ['popup'] != 'true' || $_REQUEST) {
	$_SESSION ['par'] ['documentospar'] = $_REQUEST;
} else {
	$_REQUEST = $_SESSION ['par'] ['documentospar'];
}
//monta_titulo ( $titulo_modulo, '&nbsp;' );
$tabela_execucao = monta_tabela_execucao ( TIPO_OBRAS_PAR );
monta_titulo ( $titulo_modulo, $tabela_execucao );
?>
<style>
#pesquisas thead tr td {
	background-color: #F7F7F7;
}

#pesquisas thead tr td.textosTabela {
	width: 200px;
}

#pesquisas thead tr td.camposTabela {
	width: 300px;
}

#pesquisas tbody tr td {
	background-color: #F7F7F7;
}

#pesquisas tbody tr td.SubTituloDireita {
	FONT-SIZE: 8pt;
	COLOR: black;
	FONT-FAMILY: Arial, Verdana;
	TEXT-ALIGN: right;
	BACKGROUND-COLOR: #dcdcdc;
}

#pesquisas tbody tr td input.botao {
	border: 1px solid #757575;
	background-color: #DCDCDC;
	font-size: 11px;
	font-family: arial;
	font-weight: bold;
}
</style>
<!-- INICIO INCLUDE PROTOTYPE -->
<script type="text/javascript" src="/includes/prototype.js"></script>
<!-- FIM INCLUDE PROTOTYPE -->
<!-- INICIO INCLUDE jQuery -->
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<script type="text/javascript"
	src="../includes/jquery-ui-1.8.18.custom/js/jquery-ui-1.8.18.custom.min.js"></script>
<link rel="stylesheet" type="text/css"
	href="../includes/jquery-ui-1.8.18.custom/css/ui-lightness/jquery-ui-1.8.18.custom.css" />
<link rel="stylesheet" href="/includes/ModalDialogBox/modal-message.css"
	type="text/css" media="screen" />
<script type="text/javascript"
	src="../includes/ModalDialogBox/modal-message.js"></script>
<script type="text/javascript"
	src="../includes/ModalDialogBox/ajax-dynamic-content.js"></script>
<script type="text/javascript" src="../includes/ModalDialogBox/ajax.js"></script>
<!-- FIM INCLUDE jQuery -->
<!-- INICIO INCLUDE PAR -->
<script type="text/javascript" src="js/par.js"></script>
<!-- FIM INCLUDE PAR -->

<!--JS para o componente de exibir o tolltip de dados do processo-->
<script type="text/javascript" src="../includes/remedial.js"></script>
<script type="text/javascript" src="../includes/superTitle.js"></script>
<link rel="stylesheet" type="text/css" href="../includes/superTitle.css" />
<script type="text/javascript">

jQuery.noConflict();

jQuery(document).ready(function () {

	jQuery('.vizualizarDocumento').live('click', function () {

    	var dopid = jQuery(this).attr('id');

        window.open(window.location.href + '&req=formVizualizaDocumento&dopid=' + dopid, 'minuta', 'scrollbars=yes,status=no,toolbar=no,menubar=no,location=no,fullscreen=yes');

	});

     jQuery('.regerarTermo, .btnReformular').live('click', function () {

        var dopid = jQuery(this).attr('id');
        var tipoRegera = jQuery(this).attr('attr');
        
        jQuery.ajax({
        	type: "POST",
            url: window.location.href,
            data: "req=formRegeraTermo&dopidRegera=" + dopid + "&tipoRegera=" + tipoRegera,
            async: false,
            success: function (msg) {
            	jQuery("#dialog-regerar-termo").html(msg);
                jQuery("#dialog-regerar-termo").dialog({
                	resizable: true,
                    width: 400,
                    modal: true,
                    buttons: {
                    	"Sim": function () {
							if (jQuery('[name="dopdatafimvigencia"]').val() == '') {
                            	alert('Campo obrigatório.');
                                jQuery('[name="dopdatafimvigencia"]').focus();
                                return false;
							}
							jQuery('#reqRegera').val('regerarTermo');
							jQuery('#formRegerar').submit();
                            jQuery(this).dialog("close");
						},
                        "Não": function () {
                        	jQuery(this).dialog("close");
						}
					}
				});
			}
		});
	});

	jQuery('.termoEx').live('click', function () {

		var dopid = jQuery(this).attr('id');

		jQuery.ajax({
			type: "POST",
			url: window.location.href,
			data: "req=formGeraEx&dopidEx=" + dopid,
			async: false,
			success: function (msg) {
				jQuery("#dialog-termo-ex").html(msg);
				jQuery("#dialog-termo-ex").dialog({
					resizable: true,
					width: 400,
					modal: true,
					buttons: {
						"Sim": function () {
							if (jQuery('[name="dopdatafimvigenciaex"]').val() == '') {
								alert('Campo obrigatório.');
								jQuery('[name="dopdatafimvigenciaex"]').focus();
								return false;
							}
							if (confirm('Deseja gerar o termo?')) {
								divCarregando();
								jQuery('#reqGeraEx').val('termoEx');
								jQuery('#formEx').submit();
								jQuery(this).dialog("close");
							}
						},
						"Não": function () {
							jQuery(this).dialog("close");
						}
					}
				});
			}
		});
	});
});

messageObj = new DHTML_modalMessage();	// We only create one object of this class
messageObj.setShadowOffset(5);	// Large shadow

function displayMessage(url) {
	messageObj.setSource(url);
	messageObj.setCssClassMessageBox(false);
	messageObj.setSize(560, 140);
	messageObj.setShadowDivVisible(true);	// Enable shadow for these boxes
	messageObj.display();
}

function closeMessage() {
	messageObj.close();
	window.location.href = window.location;
}
</script>
<div id="dialog-termo-ex" title="Gerar Termo EX" style="display: none;">
</div>
<div id="dialog-regerar-termo" title="Termo de Compromisso" style="display: none;"></div>
<form method="post" name="formulario" id="formulario">
	<input type="hidden" name="tipoacao" id="tipoacao" value="<?= $_REQUEST['tipoacao']; ?>">
	<table border="0" class="tabela" align="center" width="95%" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3">
		<tbody>
			<tr>
				<td class="SubTituloDireita">Preid:</td>
				<td><?php echo campo_texto('preid', 'N', 'S', 'Preid', 10, 200, '##########', '', '', '', '', '', '', $_REQUEST['preid']); ?></td>
			</tr>
			<tr>
				<td class="SubTituloDireita">Obrid:</td>
				<td><?php echo campo_texto('obrid', 'N', 'S', 'Obrid', 10, 200, '##########', '', '', '', '', '', '', $_REQUEST['obrid']); ?></td>
			</tr>
			<tr>
				<td class="SubTituloDireita" width="20%">Número do Processo:</td>
				<td colspan="3">
                	<?php
						$filtro = simec_htmlentities ( $_REQUEST ['numeroprocesso'] );
						$numeroprocesso = $filtro;
						echo campo_texto ( 'numeroprocesso', 'N', 'S', '', 50, 200, '#####.######/####-##', '' );
					?>
				</td>
			</tr>
			<tr>
				<td class="SubTituloDireita">Número do Termo:</td>
				<td colspan="3">
					<?php
						$dopid = $_REQUEST ['dopid'];
						echo campo_texto ( 'dopid', 'N', 'S', '', 20, 50, '[#]', '' );
					?>
				</td>
			</tr>
			<tr>
				<td class="SubTituloDireita">Município:</td>
				<td colspan="3">
					<?php
						$filtro = simec_htmlentities ( $_REQUEST ['municipio'] );
						$municipio = $filtro;
						echo campo_texto ( 'municipio', 'N', 'S', '', 50, 200, '', '' );
					?>
				</td>
			</tr>
			<tr>
				<td class="SubTituloDireita">Estado:</td>
				<td colspan="3">
					<?php
						$estuf = $_REQUEST ['estuf'];
						$sql = "select e.estuf as codigo, e.estdescricao as descricao from territorios.estado e order by e.estdescricao asc";
						$db->monta_combo ( "estuf", $sql, 'S', 'Todas as Unidades Federais', '', '' );
					?>
				</td>
			</tr>
			<tr>
				<td class="subtitulodireita">Esfera:</td>
				<td>
					<?php
						$esfera = $_POST ['esfera'];
						$arrEsfera = array (
								0 => array (
									"codigo" => "Municipal",
									"descricao" => "Municipal" 
								),
								1 => array (
									"codigo" => "Estadual",
									"descricao" => "Estadual" 
								) 
							);
						$db->monta_combo ( "esfera", $arrEsfera, 'S', 'Todas as esferas', '', '' );
					?>
				</td>
			</tr>
			<tr>
				<td class="subtitulodireita">Ano do Processo:</td>
				<td>
					<?php
						$anoprocesso = $_REQUEST ['anoprocesso'];
						$sql = "select distinct  substring(pronumeroprocesso,12,4) as codigo,  substring(pronumeroprocesso,12,4) as descricao from par.processoobraspar where substring(pronumeroprocesso,12,4) <> '' AND prostatus = 'A'";
						$db->monta_combo ( "anoprocesso", $sql, 'S', 'Todos', '', '' );
					?>
				</td>
			</tr>
			<tr>
				<td class="SubTituloDireita">Empenho Solicitado:</td>
				<td colspan="3"><input
					<?php echo $_REQUEST['empenhado'] == "1" ? "checked='checked'" : ""?>
					type="radio" name="empenhado" value="1">Sim <input
					<?php echo $_REQUEST['empenhado'] == "2" ? "checked='checked'" : ""?>
					type="radio" name="empenhado" value="2">Não <input
					<?php echo empty($_REQUEST['empenhado']) ? "checked='checked'" : ""?>
					type="radio" name="empenhado" value="">Todos</td>
			</tr>
			<tr>
				<td class="SubTituloDireita">Termo:</td>
				<td>
					<fieldset style="width: 380px">
						<legend>Filtros Termo:</legend>
						<input
							<?php echo $_REQUEST['termogerado'] == "1" ? "checked='checked'" : ""?>
							type="radio" name="termogerado" id="termogerado_1" value="1">Gerado
						<input
							<?php echo $_REQUEST['termogerado'] == "2" ? "checked='checked'" : ""?>
							type="radio" name="termogerado" id="termogerado_2" value="2">Não Gerado 
						<input
							<?php echo empty($_REQUEST['termogerado']) ? "checked='checked'" : ""?>
							type="radio" name="termogerado" value="">Todos <br>
						<table border="0" align="left" bgcolor="#f5f5f5" cellpadding="0" cellspacing="0" width="380px">
							<tr>
								<td class="SubTituloDireita" style="width: 60px; text-align: center;">Validação:</td>
								<td>
									<table border="0" style="width: 100%; background-color: #f5f5f5;">
										<tr>
											<td class="SubTituloDireita" style="width: 40%;">Gestor:</td>
											<td>
												<input
												<?php echo $_REQUEST['dopusucpfvalidacaogestor'] == "t" ? "checked='checked'" : ""?>
												type="radio" value="t" id="dopusucpfvalidacaogestor"
												name="dopusucpfvalidacaogestor" />Sim 
												<input
												<?php echo $_REQUEST['dopusucpfvalidacaogestor'] == "f" ? "checked='checked'" : ""?>
												type="radio" value="f" id="dopusucpfvalidacaogestor"
												name="dopusucpfvalidacaogestor" />Não 
												<input
												<?php echo empty($_REQUEST['dopusucpfvalidacaogestor']) ? "checked='checked'" : ""?>
												type="radio" name="dopusucpfvalidacaogestor" value="">Todos
											</td>
										</tr>
										<tr>
											<td class="SubTituloDireita" style="width: 40%;">De outras Entidades:</td>
											<td>
												<input
												<?php echo $_REQUEST['dopusucpfvalidacaofnde'] == "t" ? "checked='checked'" : ""?>
												type="radio" value="t" id="dopusucpfvalidacaofnde"
												name="dopusucpfvalidacaofnde" />Sim 
												<input
												<?php echo $_REQUEST['dopusucpfvalidacaofnde'] == "f" ? "checked='checked'" : ""?>
												type="radio" value="f" id="dopusucpfvalidacaofnde"
												name="dopusucpfvalidacaofnde" />Não 
												<input
												<?php echo empty($_REQUEST['dopusucpfvalidacaofnde']) ? "checked='checked'" : ""?>
												type="radio" name="dopusucpfvalidacaofnde" value="">Todos
											</td>
										</tr>
									</table>
								</td>
							</tr>
						</table>
					</fieldset>
				</td>
			</tr>
			<tr>
				<td class="subtitulodireita">Anexo de publicação:</td>
				<td>
					<input
					<?php echo ($_REQUEST['anexopublicacao'] == "1" ? "checked='checked'" : "")?>
					type="radio" name="anexopublicacao" value="1" />Sim 
					<input
					<?php echo ($_REQUEST['anexopublicacao'] == "2" ? "checked='checked'" : "")?>
					type="radio" name="anexopublicacao" value="2" />Não 
					<input
					<?php echo ($_REQUEST['anexopublicacao'] == "3" || empty($_REQUEST['anexopublicacao']) ? "checked='checked'" : "")?>
					type="radio" name="anexopublicacao" value="3" />Todos
				</td>
			</tr>
			<tr>
				<td class="SubTituloDireita">Tipo de Documento:</td>
				<td colspan="3">
                                     <?php
                                        $obTipoDocumento = new TipoDocumentoControle();
                                        $sqlTipoDocumento = $obTipoDocumento->carregarTipoDocumento(TRUE);
                                        $db->monta_combo("tpdcod", $sqlTipoDocumento, 'S', 'Selecione', 'verificaTipoDocumento', '', '', 400, 'N', 'tpdcod', '', $_REQUEST['tpdcod']);
                                    ?>
				</td>
			</tr>
			<tr id="tr_modelodoc" style="display: none">
				<td class="SubTituloDireita">Modelo de Documento:</td>
				<td colspan="3">
					<div id="div_modelodoc">
						<?php
                                                $db->monta_combo("mdoid", array(), 'S', 'Selecione', '', '', '', 400, 'N', 'mdoid');
                                                ?>
					</div>
				</td>
			</tr>
			<tr>
				<td class="SubTituloDireita">Filtrar Obras Brasil PRO:</td>
				<td colspan="3"><input type="checkbox" name="bp" value="true" /> Sim
				</td>
			</tr>
			<tr>
				<td class="SubTituloDireita">Obras MUNICIPAIS com pendências:</td>
				<td>
					<input
					<?php echo $_REQUEST['pendenciaObras'] == "1" ? "checked='checked'" : ""?>
					type="radio" name="pendenciaObras" id="pendenciaObras_1" value="1">Sim
					<input
					<?php echo $_REQUEST['pendenciaObras'] == "2" ? "checked='checked'" : ""?>
					type="radio" name="pendenciaObras" id="pendenciaObras_2" value="2">Não
					<input
					<?php echo empty($_REQUEST['pendenciaObras']) ? "checked='checked'" : ""?>
					type="radio" name="pendenciaObras" value="" />Todos
				</td>
			</tr>
			<tr>
				<td class="SubTituloDireita">Obras ESTADUAIS com pendências:</td>
				<td>
					<input
					<?php echo $_REQUEST['pendenciaObrasUF'] == "1" ? "checked='checked'" : ""?>
					type="radio" name="pendenciaObrasUF" id="pendenciaObras_uf_1"
					value="1">Sim 
					<input
					<?php echo $_REQUEST['pendenciaObrasUF'] == "2" ? "checked='checked'" : ""?>
					type="radio" name="pendenciaObrasUF" id="pendenciaObras_uf_2"
					value="2">Não 
					<input
					<?php echo empty($_REQUEST['pendenciaObrasUF']) ? "checked='checked'" : ""?>
					type="radio" name="pendenciaObrasUF" value="" />Todos
				</td>
			</tr>
			<tr>
				<td class="SubTituloDireita">Aguardando prorrogação de Vigência:</td>
				<td>
					<input
					<?php echo $_REQUEST['dopprorrogacaovigenciaobra'] == "true" ? "checked='checked'" : ""?>
					type="radio" name="dopprorrogacaovigenciaobra" value="true">Sim 
					<input
					<?php echo $_REQUEST['dopprorrogacaovigenciaobra'] == "false" ? "checked='checked'" : ""?>
					type="radio" name="dopprorrogacaovigenciaobra" value="false">Não 
					<input
					<?php echo (empty($_REQUEST['dopprorrogacaovigenciaobra']) || $_REQUEST['dopprorrogacaovigenciaobra'] == "T") ? "checked='checked'" : ""?>
					type="radio" name="dopprorrogacaovigenciaobra" value="T" />Todos
				</td>
			</tr>
			<tr>
				<td class="subtitulodireita"></td>
				<td colspan="3">
					<input class="botao" type="button" name="pesquisar" value="Pesquisar" onclick="submeterTela('nao')" /> 
					<input class="botao" type="button" name="todos" value="Ver todos" onclick="window.location.href = 'par.php?modulo=principal/documentoParObras&acao=A';" />
					<input class="botao" type="button" name="excel" value="Exportar Excel" onclick="submeterTela('sim')" /> 
					<input type="hidden" name="exportarexcel" id="exportarexcel" value="nao" />
				</td>
			</tr>
		</tbody>
	</table>
</form>
<div id="debug"></div>
<script type="text/javascript">

jQuery(document).ready(function () {
	if (jQuery('#tpdcod').val() != '') {
		verificaTipoDocumento(jQuery('#tpdcod').val());
	}
});

function verificaTipoDocumento(tipo) {
    if (tipo !== '') {
	jQuery.ajax({
		type: "POST",
		url: "par.php?modulo=principal/documentoParObras&acao=A",
		data: "tipodocumento=" + tipo + '&mdoid=<?= $_REQUEST['mdoid'] ?>',
		async: false,
		success: function (msg) {
			jQuery('#tr_modelodoc').show();
			jQuery('#div_modelodoc').html(msg);
		}
	});
    } else {
        jQuery('#tr_modelodoc').hide();
    }
}

function submeterTela(valor) {
	document.getElementById('exportarexcel').value = valor;
	
	if (document.getElementById('termogerado_1').checked == true) {
	
	if (jQuery('#tpdcod').val() == '') {
	alert('Informe um Tipo de Documento!');
	jQuery('#tpdcod').focus();
	return false;
	}
	if (jQuery('#mdoid').val() == '') {
	alert('Informe um Modelo de Documento!');
	jQuery('#mdoid').focus();
	return false;
	}
	} else {
		if (jQuery('#tpdcod').val() != '') {
			if (jQuery('#mdoid').val() == '') {
				alert('Informe um Modelo de Documento!');
				jQuery('#mdoid').focus();
				return false;
			}
		}
	}
	jQuery('#formulario').submit();
}

function carregarListaDocumentoPar(idImg, proid) {
	var img = $(idImg);
	var tr_nome = 'listaDocumentos_' + proid;
	var td_nome = 'trV_' + proid;
	
	if ($(tr_nome).style.display == 'none' && $(td_nome).innerHTML == "") {
		$(td_nome).update('Carregando...');
		img.src = '../imagens/menos.gif';
		carregaListaDocumento(proid, td_nome);
	}
	if ($(tr_nome).style.display == 'none' && $(td_nome).innerHTML != "") {
		$(tr_nome).style.display = '';
		img.src = '../imagens/menos.gif';
	} else {
		$(tr_nome).style.display = 'none';
		img.src = '/imagens/mais.gif';
	}
}

function carregarListaDocumentoParReformulado(idImg, dopid) {
	var img = $(idImg);
	var tr_nome = 'listaDocumentos2_' + dopid;
	var td_nome = 'trV2_' + dopid;
	
	if ($(tr_nome).style.display == 'none' && $(td_nome).innerHTML == "") {
		$(td_nome).update('Carregando...');
		img.src = '../imagens/menos.gif';
		carregaListaDocumentoReformulado(dopid, td_nome);
	}
	if ($(tr_nome).style.display == 'none' && $(td_nome).innerHTML != "") {
		$(tr_nome).style.display = '';
		img.src = '../imagens/menos.gif';
	} else {
		$(tr_nome).style.display = 'none';
		img.src = '/imagens/mais.gif';
	}
}

function carregaListaDocumento(proid, td_nome) {
	var myajax = new Ajax.Request('par.php?modulo=principal/documentoParObras&acao=A', {
		method: 'post',
		parameters: '&listadocumento=true&proid=' + proid,
		asynchronous: false,
		onComplete: function (res) {
			$(td_nome).update(res.responseText);
		}
	});
}

function carregaListaDocumentoReformulado(dopid, td_nome) {
	var myajax = new Ajax.Request('par.php?modulo=principal/documentoParObras&acao=A', {
		method: 'post',
		parameters: '&listadocumentoReformulado=true&dopid=' + dopid,
		asynchronous: false,
		onComplete: function (res) {
			$(td_nome).update(res.responseText);
		}
	});
}

function carregaTermoMinuta(dopid, proid) {
	window.open('par.php?modulo=principal/minutaObras&acao=A&dopid=' + dopid + '&proid=' + proid, 'minuta', 'scrollbars=yes,status=no,toolbar=no,menubar=no,location=no,fullscreen=yes');
}

function onOffCampo(campo)
{
	var div_on = document.getElementById(campo + '_campo_on');
	var div_off = document.getElementById(campo + '_campo_off');
	var input = document.getElementById(campo + '_campo_flag');
	if (div_on.style.display == 'none')
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

function finaliza(dopid) {
	divCarregando();
	if (confirm('Tem certeza que deseja finalizar essa reformulação?')) {
		var myajax = new Ajax.Request('par.php?modulo=principal/documentoParObras&acao=A', {
			method: 'post',
			parameters: '&finalizaReformulacao=true&dopid=' + dopid,
			asynchronous: false,
			onComplete: function (res) {
				if (Number(res.responseText) == '1') {
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

function excluiTermoMinuta(dopid, proid) {
	if (confirm('Tem certeza que gostaria de excluir esse registro?')) {
		var td_nome = 'trV_' + proid;
		var myajax = new Ajax.Request('par.php?modulo=principal/documentoParObras&acao=A', {
			method: 'post',
			parameters: '&excluidocumento=true&dopid=' + dopid + '&proid=' + proid,
			asynchronous: false,
			onComplete: function (res) {
				if (res.responseText == 'recarrega') {
					window.location.href = window.location;
				} else {
					$(td_nome).update(res.responseText);
				}
			}
		});
	}
}

function enviarAnexo2(proid) {
displayMessage('par.php?modulo=principal/documentoParObras&acao=A&urlgo=<? echo md5_encrypt('par.php?modulo=principal/documentoParObras&acao=A'); ?>&requisicao=telaanexo&req=inseriranexotermoprocesso&proid=' + proid);
}

function excluirAnexoTermo(proid) {
	var conf = confirm('Deseja excluir o anexo?');
	if (conf) {
		window.location = 'par.php?modulo=principal/documentoParObras&acao=A&urlgo=<? echo md5_encrypt('par.php?modulo=principal/documentoParObras&acao=A'); ?>&requisicao=excluiranexotermo&proid=' + proid;
	}
}

function enviarAnexo(dopid) {
	displayMessage('par.php?modulo=principal/documentoParObras&acao=A&urlgo=<? echo md5_encrypt('par.php?modulo=principal/documentoParObras&acao=A'); ?>&requisicao=telaanexo&req=inseriranexotermo&proid=' + proid);
}
</script>
<?php
if ($_REQUEST ['dopprorrogacaovigenciaobra'] != 'T' && ! empty ( $_REQUEST ['dopprorrogacaovigenciaobra'] )) {
	$where [] = "dp.dopprorrogacaovigenciaobra IS {$_REQUEST['dopprorrogacaovigenciaobra']}";
}

if ($_REQUEST ['anexopublicacao'] == "1") {
	$where [] = "p.arqidanexodoc is not null";
}
if ($_REQUEST ['anexopublicacao'] == "2") {
	$where [] = "p.arqidanexodoc is null";
}

if ($_REQUEST ['assinado'] != "3") {
	if ($_REQUEST ['assinado'] == "1") {
		$where [] = "ter.terassinado = TRUE";
	}
	if ($_REQUEST ['assinado'] == "2") {
		$where [] = "( ter.terassinado = FALSE OR ter.terassinado IS NULL )";
	}
}

if ($_REQUEST ['numeroprocesso']) {
	$_REQUEST ['numeroprocesso'] = str_replace ( ".", "", $_REQUEST ['numeroprocesso'] );
	$_REQUEST ['numeroprocesso'] = str_replace ( "/", "", $_REQUEST ['numeroprocesso'] );
	$_REQUEST ['numeroprocesso'] = str_replace ( "-", "", $_REQUEST ['numeroprocesso'] );
	$where [] = "p.pronumeroprocesso = '" . $_REQUEST ['numeroprocesso'] . "'";
}
if ($_REQUEST ['municipio']) {
	$where [] = "removeacento(m.mundescricao) ILIKE removeacento('%" . $_REQUEST ['municipio'] . "%')";
}

if ($_REQUEST ['estuf']) {
	$where [] = "( (iu.estuf='" . $_REQUEST ['estuf'] . "') or (iu.mun_estuf = '" . $_REQUEST ['estuf'] . "') )";
}

if ($_REQUEST ['frmid']) {
	// $where[] = "s.frmid = '".$_REQUEST['frmid']."'";
}

if ($_REQUEST ['ptsid']) {
	// $where[] = "pts.ptsid = '".$_REQUEST['ptsid']."'";
}

$sqlValorEmpenho = "(SELECT DISTINCT
					sum(vve.vrlempenhocancelado)
				FROM par.empenho e
				INNER JOIN par.v_vrlempenhocancelado vve on vve.empid = e.empid
				WHERE e.empstatus = 'A'
                                    AND e.empcodigoespecie NOT IN ('03', '13', '02', '04')
                                    AND e.empnumeroprocesso=p.pronumeroprocesso
                                    AND vve.vrlempenhocancelado > 0 )";
if ($_REQUEST ['empenhado'] == "1") {
	$where [] = "{$sqlValorEmpenho} > 0";
}
if ($_REQUEST ['empenhado'] == "2") {
	$where [] = "{$sqlValorEmpenho} = 0";
}

if ($_REQUEST ['tipoacao'] == 'popup') {
	$where [] = "md.tpdcod = 102";
}

if ($_REQUEST ['termogerado'] == "1") {
	if ($_REQUEST ['tpdcod']) {
		$where [] = "dp.dopid in (select dopid from par.documentopar dp1 
	            			inner join par.modelosdocumentos md on md.mdoid = dp1.mdoid and md.mdostatus = 'A'
	            			where md.tpdcod = {$_REQUEST['tpdcod']} ) ";
	}
	
	if ($_REQUEST ['mdoid']) {
		$where [] = "dp.mdoid = {$_REQUEST['mdoid']}";
	}
} else if ($_REQUEST ['termogerado'] == "2") {
	$where [] = "p.proid not in (SELECT DISTINCT proid FROM par.vm_documentopar_ativos WHERE proid is not null)";
} else {
	if ($_REQUEST ['tpdcod']) {
		$where [] = "dp.dopid in (select dopid from par.documentopar dp1 
	            			inner join par.modelosdocumentos md on md.mdoid = dp1.mdoid and md.mdostatus = 'A'
	            			where md.tpdcod = {$_REQUEST['tpdcod']} ) ";
	}
	
	if ($_REQUEST ['mdoid']) {
		$where [] = "dp.mdoid = {$_REQUEST['mdoid']}";
	}
}

if ($_REQUEST ['dopusucpfvalidacaofnde'] == 't') {
	$where [] = "dp.dopusucpfvalidacaofnde IS NOT NULL ";
}
if ($_REQUEST ['dopusucpfvalidacaofnde'] == 'f') {
	$where [] = "dp.dopusucpfvalidacaofnde IS NULL ";
}
if ($_REQUEST ['dopusucpfvalidacaogestor'] == 't') {
	$where [] = "dp.dopusucpfvalidacaogestor IS NOT NULL ";
}
if ($_REQUEST ['dopusucpfvalidacaogestor'] == 'f') {
	$where [] = "dp.dopusucpfvalidacaogestor IS NULL ";
}

if ($_REQUEST ['esfera']) {
	switch ($_REQUEST ['esfera']) {
		case 'Municipal' :
			$where [] = "iu.itrid = 2";
			break;
		case 'Estadual' :
			$where [] = "CASE WHEN iu.estuf = 'DF' THEN iu.itrid = 2 ELSE iu.itrid = 1 END AND p.muncod IS NULL";
			break;
	}
}

$imagem = "<img style=\"cursor:pointer\" id=\"img_dimensao_' || p.proid || '\" src=\"/imagens/mais.gif\" onclick=\"carregarListaDocumentoPar(this.id,\'' || p.proid || '\');\" border=\"0\" />";

$consultaDocumenta = "'<a href=\"#\" onclick=\"tramitar(\''||p.prpdocumenta||'\');\"><img src=\"../imagens/consultar.gif\" style=\"cursor:pointer;\" title=\"Consultar histórico do documento no sistema Documenta do FNDE\" border=\"0\"></a>'";
$insereDocumenta = "'<img src=\"../imagens/consultar.gif\" style=\"cursor:pointer;\" onclick=\"cadastraDocumenta(\''||p.pronumeroprocesso||'\', \'documentoPar\');\" title=\"Consultar histórico do documento no sistema Documenta do FNDE\" border=\"0\">'";

$perfil = pegaArrayPerfil ( $_SESSION ['usucpf'] );

if (in_array ( PAR_PERFIL_SUPER_USUARIO, $perfil ) || in_array ( PAR_PERFIL_ADMINISTRADOR, $perfil ) || in_array ( PAR_PERFIL_COORDENADOR_GERAL, $perfil ) || in_array ( PAR_PERFIL_GERADOR_DOCUMENTO, $perfil )) {
	$incluiMinuta = "'&nbsp;<img src=\"../imagens/gif_inclui.gif\" title=\"Incluir Documento\" style=cursor:pointer; onclick=\"window.open(\'par.php?modulo=principal/minutaObras&acao=A&proid='||p.proid||'\',\'minuta\',\'scrollbars=yes,status=no,toolbar=no,menubar=no,location=no,fullscreen=yes\');\"></center>'";
} else {
	$incluiMinuta = "''";
}

if ($_REQUEST ['anoprocesso']) {
	$condicaoProcessoPar = " and   substring(pronumeroprocesso,12,4) = '" . $_REQUEST ['anoprocesso'] . "'";
}

if ($_REQUEST ['dopusucpfvalidacaofnde'] == 't') {
	$where [] = "dp.dopid in (select distinct dp.dopid from par.modelosdocumentos md
								    inner join par.vm_documentopar_ativos dp on dp.mdoid = md.mdoid
								where
								    md.mdoqtdvalidacao > 1
								    and md.mdoqtdvalidacao = (select count(dv.dpvcpf) from par.documentoparvalidacao dv where dv.dopid = dp.dopid and dv.dpvstatus = 'A')
								    and md.mdostatus = 'A')";
}
if ($_REQUEST ['dopusucpfvalidacaofnde'] == 'f') {
	$where [] = "dp.dopid in (select distinct dp.dopid from par.modelosdocumentos md
										inner join par.vm_documentopar_ativos dp on dp.mdoid = md.mdoid
										inner join par.documentoparvalidacao dv on dv.dopid = dp.dopid and dv.dpvstatus = 'A'
									where
										md.mdoqtdvalidacao = 1
										and md.mdoqtdvalidacao = (select count(dv.dpvcpf) from par.documentoparvalidacao dv where dv.dopid = dp.dopid and dv.dpvstatus = 'A')
									    and md.mdostatus = 'A')";
}
if ($_REQUEST ['dopusucpfvalidacaogestor'] == 't') {
	$where [] = "dp.dopid IN (select dopid from par.documentoparvalidacao where dpvdatavalidacao is not null and dpvstatus = 'A')";
}
if ($_REQUEST ['dopusucpfvalidacaogestor'] == 'f') {
	$where [] = "dp.dopid not in (select dopid from par.documentoparvalidacao where dpvdatavalidacao is not null and dpvstatus = 'A')";
}

if ($_REQUEST ['dopid']) {
	$where [] = "(dopid = {$_REQUEST['dopid']} OR dopnumerodocumento = {$_REQUEST['dopid']})";
}

$cabecalho = array (
		"&nbsp;",
		"&nbsp;",
		"Nº Processo",
		"Município",
		"UF",
		"Tipo obra",
		"Valor empenhado(R$)",
		"Qtd obras no processo",
		"Esfera",
		'Sistema' 
);

if ($_REQUEST ['exportarexcel'] != 'sim') {
	$acoes = " case when dp.dopid is not null then
				'<center>$imagem</center>' else '' end as mais,
			   	CASE WHEN profinalizado IS NULL THEN '<center>'||$incluiMinuta ELSE '' END as acoes,";
	
	$montarTR = " ||
			  '</td></tr>
		            	<tr style=\"display:none\" id=\"listaDocumentos_' || p.proid || '\" >
		            		<td id=\"trV_' || p.proid || '\" colspan=8 ></td>
		            </tr>' ";
	
	$colunaAnexos = "CASE WHEN dp.dopid is not null THEN '<center><img onclick=\"enviarAnexo2('||p.proid||');\" style=\"cursor:pointer;\" src=\"../imagens/reject.png\"></center>' ELSE '<center> - </center>' END as anexopublicado,";
}
if ($_REQUEST ['bp']) {
	$where [] = "sisid = 14";
}

$pendenciaObras = $_REQUEST ['pendenciaObras'];
if ($pendenciaObras == '1') { // com pendencias
	$where [] = " iu.itrid = '2' ";
	$where [] = " p.muncod in (select coalesce(muncod, '') from obras2.vm_pendencia_obras where empesfera = 'M')  ";
}

if ($_REQUEST ['preid']) {
	$where [] = " pre.preid = '{$_REQUEST['preid']}' ";
}

if ($_REQUEST ['obrid']) {
	$where [] = " obr.obrid = '{$_REQUEST['obrid']}' ";
}

if ($pendenciaObras == '2') { // sem pendencias
	$where [] = " iu.itrid = '2' ";
	$where [] = " p.muncod not in (select coalesce(muncod, '') from obras2.vm_pendencia_obras where empesfera = 'M')  ";
}

$pendenciaObrasUF = $_REQUEST ['pendenciaObrasUF'];
if ($pendenciaObrasUF == '1') { // com pendencias
	$where [] = " iu.itrid = '1' ";
	$where [] = " p.estuf in (select coalesce(estuf, '') from obras2.vm_pendencia_obras where empesfera = 'E')  ";
}

if ($pendenciaObrasUF == '2') { // sem pendencias
	$where [] = " iu.itrid = '1' ";
	$where [] = " p.estuf not in (select coalesce(estuf, '') from obras2.vm_pendencia_obras where empesfera = 'E')  ";
}

$where [] = " p.prostatus = 'A'";


if ($_REQUEST ['exportarexcel'] == 'sim') {
	
	$nProcesso = " 
				substring(p.pronumeroprocesso from 1 for 5)||'.'||
			   	substring(p.pronumeroprocesso from 6 for 6)||'/'||
			   	substring(p.pronumeroprocesso from 12 for 4)||'-'||
			   	substring(p.pronumeroprocesso from 16 for 2) || '</span>' as numeroprocesso, ";
	

	$munAndUF = "CASE WHEN iu.estuf is null THEN m.mundescricao ELSE ' - ' END as mundescricao,
				CASE WHEN iu.estuf is null THEN iu.mun_estuf ELSE iu.estuf end as estuf,";
}
else
{
	$nProcesso = "  (
	                            CASE WHEN (select count(*) as qtd from painel.dadosfinanceirosconvenios dfi where dfi.dfiprocesso = p.pronumeroprocesso) > 0 THEN
	                                '<span class=\"processoDetalhes processo_detalhe\" >'
	                            ELSE
	                                '<span class=\"processo_detalhe\" >'
	                            END
	                        ) ||
				substring(p.pronumeroprocesso from 1 for 5)||'.'||
			   	substring(p.pronumeroprocesso from 6 for 6)||'/'||
			   	substring(p.pronumeroprocesso from 12 for 4)||'-'||
			   	substring(p.pronumeroprocesso from 16 for 2) || '</span>' as numeroprocesso, ";
	
	$munAndUF = "CASE WHEN iu.estuf is null THEN '<a onclick=\"abrePlanoTrabalho(''municipio'', '''|| iu.mun_estuf ||''', '''|| iu.muncod ||''', '''|| iu.inuid ||''')\" style=cursor:pointer; >'||m.mundescricao||'</a>' ELSE ' - ' END as mundescricao,
				CASE WHEN iu.estuf is null THEN iu.mun_estuf ELSE '<a onclick=\"abrePlanoTrabalho(''estado'', '''||iu.estuf||''', '''','''|| iu.inuid ||''')\" style=cursor:pointer; >'||iu.estuf||'</a>' end as estuf,";
	
}


$sql = "
		SELECT DISTINCT
				{$acoes}
               	{$nProcesso}    
               	{$munAndUF}  
			--m.mundescricao, 
			--CASE WHEN p.estuf IS NOT NULL THEN p.estuf ELSE m.estuf END as estuf,
			
			
				
			'' as tipoobra, -- CASE WHEN p.protipo='P' THEN 'Proinfância' ELSE 'Quadra' END as tipoobra,
			$colunaAnexos
			(SELECT DISTINCT
					sum(vve.vrlempenhocancelado)
				FROM par.empenho e
				INNER JOIN par.v_vrlempenhocancelado vve on vve.empid = e.empid
				WHERE e.empstatus = 'A'
					AND e.empcodigoespecie NOT IN ('03', '13', '02', '04')
					AND e.empnumeroprocesso=p.pronumeroprocesso
					AND vve.vrlempenhocancelado > 0) as valorempenhado,
			(select count(pc.preid) from par.processoobraspar po inner join par.processoobrasparcomposicao pc on pc.proid = po.proid where pc.pocstatus = 'A' and po.proid = p.proid) as qtdobrempenhadas,
			case when (p.muncod is null or p.muncod = '') then 'Estadual' else 'Municipal' end as esfera,
			case when p.sisid = 14 then 'Brasil-pró' else 'PAR' end $montarTR as tiposistema
		FROM par.processoobraspar p

		LEFT JOIN par.processoobrasparcomposicao poc ON(poc.proid = p.proid) and poc.pocstatus = 'A'
		LEFT JOIN obras.preobra pre ON(
            pre.preid = poc.preid 
            AND pre.prestatus = 'A')

		LEFT JOIN obras2.obras obr
		INNER JOIN workflow.documento doc2 ON(
            doc2.docid = obr.docid
            AND tpdid = " . TPDID_OBJETO . ")
		INNER JOIN workflow.estadodocumento esd2 ON esd2.esdid = doc2.esdid
            ON( obr.obrid = pre.obrid
            AND obridpai IS NULL)

		INNER JOIN par.instrumentounidade iu ON iu.inuid = p.inuid " . $condicaoProcessoPar . "
		LEFT JOIN territorios.municipio m ON m.muncod = p.muncod
		LEFT JOIN territorios.estado e ON e.estuf = p.estuf
		LEFT JOIN par.vm_documentopar_ativos dp ON dp.proid = p.proid
		LEFT JOIN par.modelosdocumentos md ON md.mdoid = dp.mdoid
		LEFT JOIN par.termocompromissopac ter ON ter.proid = p.proid AND ter.terstatus='A'
		" . (($where) ? " WHERE " . implode ( " AND ", $where ) : "");

if ($_REQUEST ['exportarexcel'] == 'sim') {
	ob_clean ();
	$cabecalho = array (
			"Nº Processo",
			"Município",
			"UF",
			"Tipo obra",
			"Valor empenhado(R$)",
			"Qtd obras no processo",
			"Esfera",
			'Sistema' 
	);
	header ( "Expires: Mon, 1 Apr 1974 05:00:00 GMT" );
	header ( "Last-Modified: " . gmdate ( "D,d M YH:i:s" ) . " GMT" );
	header ( "Pragma: no-cache" );
	header ( "Content-type: application/xls; name=SIMEC_RelatDocente" . date ( "Ymdhis" ) . ".xls" );
	header ( "Content-Disposition: attachment; filename=SIMEC_RelatDocente" . date ( "Ymdhis" ) . ".xls" );
	header ( "Content-Description: MID Gera excel" );
	$db->monta_lista_tabulado ( $sql, $cabecalho, 1000000, 5, 'N', '100%', $par2 );
	exit ();
} else {
	// Só monta lista quando clicar em pesquisar
	if (isset ( $_REQUEST ['estuf'] )) {
		$cabecalho = array (
				"&nbsp;",
				"&nbsp;",
				"Nº Processo",
				"Município",
				"UF",
				"Tipo obra",
				"Anexo de publicação",
				"Valor empenhado(R$)",
				"Qtd obras no processo",
				"Esfera",
				'Sistema' 
		);
		$db->monta_lista ( $sql, $cabecalho, 100, 5, 'N', 'center', 'N', 'formprocesso' );
	}
}
?>