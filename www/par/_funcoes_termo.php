<?php

function downloadanexotermo($dados) {
	global $db;
	include_once APPRAIZ . "includes/classes/fileSimec.class.inc";
	$arrCampos = array();
	$file = new FilesSimec("termocompromissopac",$arrCampos,"par");
	$file->getDownloadArquivo($dados['arqid']);
}

function excluiranexotermo($dados) {
	global $db;
	
	if( $dados['tipo'] == 'dou' ){
		$sql = "UPDATE par.processoobraanexo SET poastatus = 'I' WHERE proid = '".$_REQUEST['codigo']."'";
		$db->executar($sql);
		$db->commit();
	} else {	
		$sql = "UPDATE par.termocompromissopac SET arqidanexo=NULL WHERE terid='".$_REQUEST['codigo']."'";
		$db->executar($sql);
		$db->commit();
	}
	
	die("<script>
			alert('Anexo removido');
			window.location='".md5_decrypt($dados['urlgo'])."';
		 </script>");
	
}

function inseriranexotermo($dados) {
	global $db;
	
	if($_FILES['arquivo']['name']){
		include_once APPRAIZ . "includes/classes/fileSimec.class.inc";
		$arrCampos = array();
		$file = new FilesSimec("termocompromissopac",$arrCampos,"par");
		$file->setUpload("","arquivo",false);
		$arqid = $file->getIdArquivo();
	}

	$sql = "UPDATE par.termocompromissopac SET arqidanexo='".$arqid."' WHERE terid='".$_POST['terid']."'";
	$db->executar($sql);
	$db->commit();
	
	
	die("<script>
			alert('Anexado com sucesso.');
			window.location='".md5_decrypt($dados['urlgo'])."';
		 </script>");

}

function telaanexo($dados) {
	global $db;
	
	if($dados['terid']) {
		$sql = "SELECT a.arqid, a.arqnome||'.'||a.arqextensao as arquivo FROM par.termocompromissopac t
				INNER JOIN public.arquivo a ON a.arqid = t.arqidanexo   
				WHERE terid='".$dados['terid']."'";
		$tipo = 'termo';
		$codigo = $dados['terid'];
	} else {
		$sql = "SELECT a.arqid, a.arqnome||'.'||a.arqextensao as arquivo FROM par.processoobraanexo p
				INNER JOIN public.arquivo a ON a.arqid = p.arqid   
				WHERE proid='".$dados['proid']."' and poastatus = 'A'";
		$tipo = 'dou';
		$codigo = $dados['proid'];
	}
	
	$arquivos = $db->carregar($sql);
	
	echo "<form method=post name=formulario id=formularioanexo enctype=multipart/form-data>";
	echo "<input type=hidden name=requisicao value=".$dados['req'].">";
	echo "<input type=hidden name=urlgo value=".$dados['urlgo'].">";
	echo "<input type=hidden name=terid value=".$dados['terid'].">";	
	echo "<input type=hidden name=proid value=".$dados['proid'].">";
	echo "<table align=\"center\" border=\"0\" class=\"tabela\" cellpadding=\"3\" cellspacing=\"1\">";
	echo "<tr>";
	echo "<td class=\"SubTituloDireita\">Anexo:</td>";
	echo "<td><input type=file name=arquivo id=arquivo ".(($arquivos[0])?"disabled":"")."></td>";
	echo "</tr>";
	if($arquivos[0]) {
		foreach($arquivos as $arquivo) {
			echo "<tr>";
			echo "<td class=\"SubTituloDireita\">Nome do arquivo:</td>";
			echo "<td>".$arquivo['arquivo']." <input type=button name=download value=Download onclick=window.location='par.php?modulo=principal/gerarTermoObra&acao=A&arqid=".$arquivo['arqid']."&requisicao=downloadanexotermo';> 
											<input type=button name=excluir value=Excluir onclick=excluirAnexoTermo('".$codigo."','".$tipo."');></td>";
			echo "</tr>";
		}
	}
	echo "<tr>";
	echo "<td class=\"SubTituloCentro\" colspan=2><input type=button name=salvar value=Salvar onclick=enviarAnexoTermo();> <input type=button name=fechar value=Fechar onclick=closeMessage();></td>";
	echo "</tr>";
	echo "</table>";
	echo "</form>";
}

function download(){
	
	global $db;
	
	$sql = "SELECT
				arqid
			FROM
				par.termocompromissopac
			WHERE
				terid = ".$_REQUEST['terid'];

	$arqid = $db->pegaUm($sql);
//	ver($arqid,d);
	
	$documento = pegaTermoCompromissoArquivo('', $_REQUEST['terid']);
	
	if($arqid){
		$caminho = APPRAIZ."arquivos/par/".floor($arqid/1000).'/'.$arqid;
		if( is_file($caminho) ){
			include_once APPRAIZ."includes/classes/fileSimec.class.inc";
			$file 		= new FilesSimec('termocompromissopac', NULL, 'par');
			$file->getDownloadArquivo($arqid);
		} else {
			$http = new RequestHttp();
			$http->toPdfDownload($documento, 'PAC2_'.$_REQUEST['terid']);
		}
	}else{
		if( $documento ){
			$http = new RequestHttp();
			$http->toPdfDownload($documento, 'PAC2_'.$_REQUEST['terid']);
		} else {		
			echo "
				<script>
					alert('Arquivo não encontrado.');
					window.close();
				</script>";
		}
	}
	die();
}

function cabecalhoSolicitacaoTermo() {
	global $db;
	
	if( $_REQUEST['muncod'] ){
		$arrDados = $db->pegaLinha("SELECT 
										m.muncod,
										m.estuf,
										m.mundescricao,
										p.pronumeroprocesso,
										CASE WHEN p.protipo='P' THEN 'Proinfância' ELSE 'Quadra' END as tipoobra,
										p.protipo
									FROM par.processoobra p
								    INNER JOIN territorios.municipio m ON m.muncod = p.muncod
								    WHERE 
								    	p.prostatus = 'A'  and 
								    	m.muncod = '".$_REQUEST['muncod']."' AND 
								    	p.proid = '".$_SESSION['par_var']['proid']."'");
	}elseif( $_REQUEST['estuf'] ){
		$arrDados = $db->pegaLinha("SELECT 
										e.estuf,
										' - ' as mundescricao,
										p.pronumeroprocesso,
										CASE WHEN p.protipo='P' THEN 'Proinfância' ELSE 'Quadra' END as tipoobra,
										p.protipo
									FROM par.processoobra p
								    INNER JOIN territorios.estado e ON e.estuf = p.estuf
								    WHERE 
								    	p.prostatus = 'A'  and 
								    	e.estuf = '".$_REQUEST['estuf']."' AND 
								    	p.proid = '".$_SESSION['par_var']['proid']."'");
	}

	echo "<table border=0 cellpadding=3 cellspacing=0 class=listagem width=95% align=center>";
	echo "<tr>";
	echo "<td class=SubTituloDireita width=\"30%\">Número do Processo:</td>";
	echo "<td>".$arrDados['pronumeroprocesso']."</td>";
	echo "</tr>";
	echo "<tr>";
	echo "<td class=SubTituloDireita>UF:</td>";
	echo "<td>".$arrDados['estuf']."</td>";
	echo "</tr>";
	echo "<tr>";
	echo "<td class=SubTituloDireita>Município:</td>";
	echo "<td>".$arrDados['mundescricao']."</td>";
	echo "</tr>";
	echo "<tr>";
	echo "<td class=SubTituloDireita>Tipo obra:</td>";
	echo "<td>".$arrDados['tipoobra']."</td>";
	echo "</tr>";
	echo "</table>";
	
	return $arrDados['protipo'];
}

function excluirTermo() {
	
	global $db;
	
	$sql = "UPDATE par.termocompromissopac
			SET
				terstatus = 'I'
			WHERE
				terid = ".$_REQUEST['terid'];
	
	$db->executar($sql);
	$db->commit();
}

function listaTermoFilho($dados){
	
      if ($dados['icone'] == null) {
            $imgBotaoVisualizar = "'<img src=../imagens/print.gif title=\"Visualizar\" style=cursor:pointer; onclick=consultarTermo('||t.terid||');>'";
        } else {
            $imgBotaoVisualizar = "'<img onclick=\"window.open(\'par.php?modulo=principal/teladevalidacao&acao=A&terid='||t.terid||'\',\'assinatura\',\'scrollbars=yes,fullscreen=yes,status=no,toolbar=no,menubar=no,location=no\');\" style=\"cursor:pointer;\" title=\"Visualizar\" src=\"../imagens/icone_lupa.png\">'";
        }
	global $db;
	
	extract($dados);
	
	$titid = $titid ? $titid : 'NULL';
	
	$sql = "SELECT DISTINCT
				CASE WHEN teridpai IS NOT NULL
					THEN '<img src=../imagens/mais.gif title=Histórico style=cursor:pointer; id='|| t.teridpai || ' class=historicoTermo >&nbsp;'
					ELSE ''
				END ||
				$imgBotaoVisualizar as acao,
				en.entnumcpfcnpj,
				en.entnome,
				(select par.retornanumerotermopac(t.proid)) as ternum,
				u.usunome,
				(
                                    SELECT to_char(terdatainclusao,'DD/MM/YYYY') from par.termocompromissopac WHERE proid = p.proid ORDER BY terid ASC LIMIT 1
                                ) as terdata,
				'<a style=cursor:pointer onclick=\"window.location=\'par.php?modulo=principal/gerarTermoObra&acao=A&arqid='||arq.arqid||'&requisicao=downloadanexotermo\';\">'||arq.arqnome||'.'||arq.arqextensao||'</a>' as arquivo,
                                CASE WHEN t.teracaoorigem IS NULL THEN 'CRIAÇÃO' ELSE t.teracaoorigem END,
                                to_char(t.terdatainclusao,'DD/MM/YYYY')
                                
			FROM
				par.termocompromissopac t
			INNER JOIN par.processoobra 		 p ON p.proid = t.proid and p.prostatus = 'A' 
			LEFT  JOIN seguranca.usuario 	     u ON u.usucpf    = t.usucpf
			LEFT  JOIN par.entidade		    	en ON ( (en.muncod = t.muncod AND en.dutid = ".DUTID_PREFEITURA.") 
														OR
														(en.estuf = t.estuf AND en.dutid = ".DUTID_SECRETARIA_ESTADUAL.")
													  ) AND en.entstatus = 'A'
													
			LEFT  JOIN public.arquivo 		   arq ON arq.arqid   = t.arqidanexo
			WHERE
				t.terid = $terid
				--AND t.titid = $titid";
	$cabecalho = array("&nbsp;","CNPJ","Entidade","Nº Termo","Usuário criação","Data de criação do TC Original", "Anexo","Ação realizada",'Data de criação do TC Atual');
	$db->monta_lista_simples($sql,$cabecalho,500,5,'N','100%',$par2);
}

function listaTermo($dados) {
	global $db;
        
        #Resgata perfil LOGADO
        $perfil = pegaPerfilGeral();
	
	if($dados['muncod']){
		
		$queryAprovadasProcesso = "
				( SELECT
					COUNT(pre.preid)
				FROM
					par.processoobraspaccomposicao poc
				INNER JOIN obras.preobra pre ON pre.preid = poc.preid AND pre.prestatus = 'A' AND poc.pocstatus = 'A'
				INNER JOIN workflow.documento doc ON doc.docid = pre.docid AND doc.esdid <> 228
				WHERE poc.proid = {$dados['proid']} )";
		
		$queryTermoAssinado = "(SELECT terassinado FROM par.termocompromissopac WHERE proid = {$dados['proid']} AND terstatus='A' AND terassinado = TRUE LIMIT 1)";
		
		$acaoReprogramar = "CASE WHEN $queryAprovadasProcesso = 0 AND $queryTermoAssinado = 't'
								THEN '<img src=../imagens/restricao_ico.png style=cursor:pointer; id=' || t.proid || '_' || p.protipo  || '_' || t.muncod || '_' || 'null' || '_' || t.terid || ' attr=\"REPROGRAMAR\" class=aditivoTermo title=\"Reformular\">'
								ELSE ''
							END";
		
		$acaoRegerar = "CASE WHEN $queryTermoAssinado = 't'
							THEN '<img src=../imagens/refresh.gif style=cursor:pointer; id=' || t.proid || '_' || p.protipo  || '_' || t.muncod || '_' || 'null' || '_' || t.terid || ' class=aditivoTermo attr=\"REGERAR\" title=\"Regerar\">&nbsp;'
							ELSE '<img src=../imagens/refresh.gif style=cursor:pointer; id=' || t.proid || '_' || p.protipo  || '_' || t.muncod || '_' || 'null' || '_' || t.terid || ' class=regerarTermo title=\"Regerar\">'
						END";
		
		$sql = "SELECT DISTINCT
                                CASE WHEN (SELECT teridpai FROM par.termocompromissopac WHERE proid = {$dados['proid']} AND terstatus='A' LIMIT 1) IS NOT NULL
                                    THEN '<img src=../imagens/mais.gif title=Histórico style=cursor:pointer; id='|| t.teridpai || ' class=historicoTermo >'
                                    ELSE ''
                                END
                               || 
                                '<img src=../imagens/print.gif title=\"Visualizar\" style=cursor:pointer; onclick=consultarTermo('||t.terid||');>' ||
                                CASE WHEN p.profinalizado IS NULL 
                                    THEN '<img src=../imagens/reject.png title=\"Enviar anexo\" style=cursor:pointer; onclick=enviarAnexo('||t.terid||');>'
                                    ||
                                        CASE WHEN 
                                            (
                                                SELECT DISTINCT TRUE
                                                FROM par.termocompromissopac ter1
                                                INNER JOIN par.termoobra 	tob1 ON tob1.terid = ter1.terid
                                                INNER JOIN par.empenhoobra 	eob1 ON eob1.preid = tob1.preid AND eob1.eobstatus = 'A'
                                                INNER JOIN par.pagamento 	pag1 ON pag1.empid = eob1.empid AND pag1.pagstatus = 'A' AND pag1.pagsituacaopagamento not ilike '%CANCELADO%'
                                                WHERE ter1.terid = t.terid
                                            )
                                            THEN ''
                                            ELSE '<img src=../imagens/excluir_2.gif title=\"Excluir\" width=\"15px\" style=\"cursor:pointer;\" onclick=excluirTermos('||t.terid||');>'
                                        END
                                    ||
                                    $acaoRegerar
                               		||
                                	$acaoReprogramar
                                ELSE '' 
                                END as acao,
                                en.entnumcpfcnpj, 
                                en.entnome,
                                (
                                    select par.retornanumerotermopac(p.proid)
                                ) as ternum,
                                u.usunome, 
                                (
                                    SELECT to_char(terdatainclusao,'DD/MM/YYYY') from par.termocompromissopac WHERE proid = p.proid ORDER BY terid ASC LIMIT 1
                                ) as terdata,
                                '<a style=cursor:pointer onclick=\"window.location=\'par.php?modulo=principal/gerarTermoObra&acao=A&arqid='||arq.arqid||'&requisicao=downloadanexotermo\';\">'||arq.arqnome||'.'||arq.arqextensao||'</a>' as arquivo,
                                tpc.tpddsc,
                                CASE WHEN t.teracaoorigem IS NULL THEN 'CRIAÇÃO' ELSE t.teracaoorigem END,
                                to_char(t.terdatainclusao,'DD/MM/YYYY')
                        FROM 
                                par.termocompromissopac t
                        INNER JOIN par.processoobra 		 p ON p.proid = t.proid and p.prostatus = 'A' 
                        LEFT  JOIN seguranca.usuario 	     u ON u.usucpf    = t.usucpf
                        LEFT  JOIN par.entidade		    	en ON en.muncod = t.muncod AND en.dutid = ".DUTID_PREFEITURA." AND en.entstatus = 'A'
                        --LEFT  JOIN entidade.endereco 	  ende ON ende.muncod = t.muncod
                        --LEFT  JOIN entidade.entidade 	    en ON en.entid    = ende.entid
                        --LEFT  JOIN entidade.funcaoentidade fun ON fun.entid   = en.entid
                        LEFT JOIN par.termocompromissopac tc2  ON t.teridpai = tc2.terid
                        LEFT  JOIN public.arquivo arq ON arq.arqid   = t.arqidanexo
                        left join public.tipodocumento tpc on tpc.tpdcod = t.tpdcod and tpc.tpdstatus = 'A'
                        WHERE 
                                en.muncod = '".$dados['muncod']."'
                                AND t.proid = '".$dados['proid']."'
                                AND t.terstatus = 'A'
                        ORDER BY ternum desc, terdata desc";
	}elseif($dados['estuf']){
           
            $acaoBotao = "'<img src=../imagens/restricao_ico.png style=cursor:pointer; id=' || t.proid || '_' || p.protipo  || '_' || 'null' || '_' || t.estuf || '_' || t.terid || ' attr=\"REGERAR\" class=aditivoTermo title=\"Regerar\"><span id=' || t.proid || '_' || p.protipo  || '_' || 'null' || '_' || t.estuf || '_' || t.terid || '  title=\"Reprogramar\" class=\"glyphicon glyphicon-share aditivoTermo\" attr=\"REPROGRAMAR\" style=\"cursor:pointer; color:#228B22;\"></span>'";
            
            $sql = "SELECT DISTINCT
                        CASE WHEN (SELECT teridpai FROM par.termocompromissopac WHERE proid = {$dados['proid']} AND terstatus='A' LIMIT 1) IS NOT NULL
                            THEN '<img src=../imagens/mais.gif title=Histórico style=cursor:pointer; id='|| t.teridpai || ' class=historicoTermo >&nbsp;'
                            ELSE ''
                        END
                         || 
                        '<img src=../imagens/print.gif title=\"Visualizar\" style=cursor:pointer; onclick=consultarTermo('||t.terid||');>' ||
                        CASE WHEN p.profinalizado IS NULL 
                            THEN '<img src=../imagens/reject.png title=\"Enviar anexo\" style=cursor:pointer; onclick=enviarAnexo('||t.terid||');>'
                            ||
                                CASE WHEN 
                                    (
                                        SELECT DISTINCT TRUE
                                        FROM par.termocompromissopac ter1
                                        INNER JOIN par.termoobra 	tob1 ON tob1.terid = ter1.terid
                                        INNER JOIN par.empenhoobra 	eob1 ON eob1.preid = tob1.preid AND eob1.eobstatus = 'A'
                                        INNER JOIN par.pagamento 	pag1 ON pag1.empid = eob1.empid AND pag1.pagstatus = 'A' AND pag1.pagsituacaopagamento not ilike '%CANCELADO%'
                                        WHERE ter1.terid = t.terid
                                    )
                                    THEN ''
                                    ELSE '<img src=../imagens/excluir_2.gif title=\"Excluir\" width=\"15px\" style=\"cursor:pointer;\" onclick=excluirTermos('||t.terid||');>'
                                END
                            ||
                                CASE WHEN (SELECT DISTINCT terassinado FROM par.termocompromissopac WHERE proid = {$dados['proid']} AND terstatus='A' AND terassinado = false limit 1) = 'f'
                                    THEN '<img src=../imagens/refresh.gif style=cursor:pointer; id=' || t.proid || '_' || p.protipo  || '_' || 'null'  || '_' || t.estuf || '_' || t.terid || ' class=regerarTermo title=\"Recarregar\">'
                                    ELSE $acaoBotao
                                END
                        ELSE '' 
                        END as acao,
                        en.entnumcpfcnpj, 
                        en.entnome, 
                        (
                            select par.retornanumerotermopac(p.proid)
                        ) as ternum,
                        u.usunome, 
                        (
                            SELECT to_char(terdatainclusao,'DD/MM/YYYY') from par.termocompromissopac WHERE proid = p.proid ORDER BY terid ASC LIMIT 1
                        ) as terdata,
                        '<a style=cursor:pointer onclick=\"window.location=\'par.php?modulo=principal/gerarTermoObra&acao=A&arqid='||arq.arqid||'&requisicao=downloadanexotermo\';\">'||arq.arqnome||'.'||arq.arqextensao||'</a>' as arquivo,
                        tpc.tpddsc,
                        CASE WHEN t.teracaoorigem IS NULL THEN 'CRIAÇÃO' ELSE t.teracaoorigem END,
                        to_char(t.terdatainclusao,'DD/MM/YYYY')
                    FROM 
                            par.termocompromissopac t
                    INNER JOIN par.processoobra 		 p ON p.proid = t.proid and p.prostatus = 'A' 
                    LEFT  JOIN seguranca.usuario 	     u ON u.usucpf    = t.usucpf
                    LEFT  JOIN par.entidade		    	en ON en.estuf = t.estuf AND en.dutid = ".DUTID_SECRETARIA_ESTADUAL." AND en.entstatus = 'A'
                    LEFT  JOIN public.arquivo arq ON arq.arqid   = t.arqidanexo
                    left join public.tipodocumento tpc on tpc.tpdcod = t.tpdcod and tpc.tpdstatus = 'A'
                    WHERE 
                        en.estuf = '".$dados['estuf']."'
                        AND t.proid = '".$dados['proid']."'
                        AND t.terstatus = 'A'
                    LIMIT 1";
	}
	//ver(simec_htmlentities($sql),d);
	$cabecalho = array("&nbsp;","CNPJ","Entidade","Nº Termo","Usuário criação","Data de criação do TC Original", "Anexo", 'Tipo de Documento','Ação realizada', 'Data de criação do TC Atual');
	$db->monta_lista_simples($sql,$cabecalho,500,5,'N','100%',$par2);
}

function listaTermoHistorico($dados) {
    
	global $db;
        
        #Resgata perfil LOGADO
        $perfil = pegaPerfilGeral();
	
	if($dados['muncod']){
		$sql = "SELECT DISTINCT
                                CASE WHEN (SELECT teridpai FROM par.termocompromissopac WHERE proid = {$dados['proid']} AND terstatus='A' LIMIT 1) IS NOT NULL
                                    THEN '<img src=../imagens/mais.gif title=Histórico style=cursor:pointer; id='|| t.teridpai || ' class=historicoTermo >
                                          <img onclick=\"window.open(\'par.php?modulo=principal/teladevalidacao&acao=A&terid='||t.terid||'\',\'assinatura\',\'scrollbars=yes,fullscreen=yes,status=no,toolbar=no,menubar=no,location=no\');\" style=\"cursor:pointer;\" title=\"Visualizar\" src=\"../imagens/icone_lupa.png\">'
                                    ELSE '<img onclick=\"window.open(\'par.php?modulo=principal/teladevalidacao&acao=A&terid='||t.terid||'\',\'assinatura\',\'scrollbars=yes,fullscreen=yes,status=no,toolbar=no,menubar=no,location=no\');\" style=\"cursor:pointer;\" title=\"Visualizar\" src=\"../imagens/icone_lupa.png\">'
                                END as acao,
                                en.entnumcpfcnpj, 
                                en.entnome,
                                (
                                    select par.retornanumerotermopac(p.proid)
                                ) as ternum,
                                u.usunome, 
                                (
                                    SELECT to_char(terdatainclusao,'DD/MM/YYYY') from par.termocompromissopac WHERE proid = p.proid ORDER BY terid ASC LIMIT 1
                                ) as terdata,
                                '<a style=cursor:pointer onclick=\"window.location=\'par.php?modulo=principal/gerarTermoObra&acao=A&arqid='||arq.arqid||'&requisicao=downloadanexotermo\';\">'||arq.arqnome||'.'||arq.arqextensao||'</a>' as arquivo,
                                tpc.tpddsc 
                        FROM 
                                par.termocompromissopac t
                        INNER JOIN par.processoobra 		 p ON p.proid = t.proid and p.prostatus = 'A' 
                        LEFT  JOIN seguranca.usuario 	     u ON u.usucpf    = t.usucpf
                        LEFT  JOIN par.entidade		    	en ON en.muncod = t.muncod AND en.dutid = ".DUTID_PREFEITURA." AND en.entstatus = 'A'
                        --LEFT  JOIN entidade.endereco 	  ende ON ende.muncod = t.muncod
                        --LEFT  JOIN entidade.entidade 	    en ON en.entid    = ende.entid
                        --LEFT  JOIN entidade.funcaoentidade fun ON fun.entid   = en.entid
                        LEFT JOIN par.termocompromissopac tc2  ON t.teridpai = tc2.terid
                        LEFT  JOIN public.arquivo arq ON arq.arqid   = t.arqidanexo
                        left join public.tipodocumento tpc on tpc.tpdcod = t.tpdcod and tpc.tpdstatus = 'A'
                        WHERE 
                                en.muncod = '".$dados['muncod']."'
                                AND t.proid = '".$dados['proid']."'
                                AND t.terstatus = 'A'
                        ORDER BY ternum desc, terdata desc";
	}elseif($dados['estuf']){
            
            $sql = "SELECT DISTINCT
                        CASE WHEN (SELECT teridpai FROM par.termocompromissopac WHERE proid = {$dados['proid']} AND terstatus='A' LIMIT 1) IS NOT NULL
                            THEN '<img src=../imagens/mais.gif title=Histórico style=cursor:pointer; id='|| t.teridpai || ' class=historicoTermo >&nbsp;
                            <img onclick=\"window.open(\'par.php?modulo=principal/teladevalidacao&acao=A&terid='||t.terid||'\',\'assinatura\',\'scrollbars=yes,fullscreen=yes,status=no,toolbar=no,menubar=no,location=no\');\" style=\"cursor:pointer;\" title=\"Visualizar\" src=\"../imagens/icone_lupa.png\">'
                            ELSE '<img onclick=\"window.open(\'par.php?modulo=principal/teladevalidacao&acao=A&terid='||t.terid||'\',\'assinatura\',\'scrollbars=yes,fullscreen=yes,status=no,toolbar=no,menubar=no,location=no\');\" style=\"cursor:pointer;\" title=\"Visualizar\" src=\"../imagens/icone_lupa.png\">'
                        END as acao,
                        en.entnumcpfcnpj, 
                        en.entnome, 
                        (
                            select par.retornanumerotermopac(p.proid)
                        ) as ternum,
                        u.usunome, 
                        (
                            SELECT to_char(terdatainclusao,'DD/MM/YYYY') from par.termocompromissopac WHERE proid = p.proid ORDER BY terid ASC LIMIT 1
                        ) as terdata,
                        '<a style=cursor:pointer onclick=\"window.location=\'par.php?modulo=principal/gerarTermoObra&acao=A&arqid='||arq.arqid||'&requisicao=downloadanexotermo\';\">'||arq.arqnome||'.'||arq.arqextensao||'</a>' as arquivo,
                        tpc.tpddsc 
                    FROM 
                            par.termocompromissopac t
                    INNER JOIN par.processoobra 		 p ON p.proid = t.proid and p.prostatus = 'A' 
                    LEFT  JOIN seguranca.usuario 	     u ON u.usucpf    = t.usucpf
                    LEFT  JOIN par.entidade		    	en ON en.estuf = t.estuf AND en.dutid = ".DUTID_SECRETARIA_ESTADUAL." AND en.entstatus = 'A'
                    LEFT  JOIN public.arquivo arq ON arq.arqid   = t.arqidanexo
                    left join public.tipodocumento tpc on tpc.tpdcod = t.tpdcod and tpc.tpdstatus = 'A'
                    WHERE 
                        en.estuf = '".$dados['estuf']."'
                        AND t.proid = '".$dados['proid']."'
                        AND t.terstatus = 'A'
                    LIMIT 1";
	}
        
	$cabecalho = array("&nbsp;","CNPJ","Entidade","Nº Termo","Usuário criação","Data da Criação", "Anexo", 'Tipo de Documento');
	$db->monta_lista_simples($sql,$cabecalho,500,5,'N','100%');
}

function listaPreObrasTermo($dados) {

	global $db;
	
	// A pedido do Julio Viana (25/06/2014)
	$proid = $_REQUEST['proid'] ? $_REQUEST['proid'] : $_SESSION['par_var']['proid'];
	if( $proid == 11809 ){
		$inn = " INNER JOIN workflow.documento d ON d.docid = pre.docid and d.esdid IN (".WF_TIPO_OBRA_APROVADA.",".WF_TIPO_OBRA_APROVACAO_CONDICIONAL.") ";
	} else {
		$inn = " INNER JOIN workflow.documento d ON d.docid = pre.docid and d.esdid IN (".WF_TIPO_OBRA_APROVADA.") ";
	}
	
	$strWhere = "";
	if ($dados['estuf'] != '' && $dados['estuf'] != 'null' && $dados['estuf'] != null ) {
		if ($dados['estuf'] == 'DF') {
			$strWhere = "AND ( pre.estuf = 'DF' OR pre.muncod = '5300108') AND (pre.preesfera = 'E' OR pre.preesfera = 'M')";
		}else{
			$strWhere = "AND pre.estuf = '{$dados['estuf']}' AND pre.preesfera = 'E'";
		}
	}else {
		if ($dados['muncod'] == '5300108') {
			$strWhere = "AND ( pre.estuf = 'DF' OR pre.muncod = '5300108') AND (pre.preesfera = 'E' OR pre.preesfera = 'M')";
		}else{
			$strWhere = "AND pre.muncod = '{$dados['muncod']}' AND pre.preesfera = 'M' ";
		}
	}

	$sql = "SELECT
				'<input type=hidden name=preids[] id='|| pre.preid ||' value='|| pre.preid ||'><input type=\"checkbox\" name=\"preids_\" checked disabled />' as check,
				pre.predescricao,
				pto.ptodescricao,
				prevalorobra
			FROM
				obras.preobra pre
			INNER JOIN par.empenhoobra   emp ON emp.preid = pre.preid and eobstatus = 'A' 
			INNER JOIN par.empenho   emn ON emn.empid = emp.empid and empstatus = 'A'
			INNER JOIN par.processoobra   pro ON pro.pronumeroprocesso = emn.empnumeroprocesso and pro.prostatus = 'A' 
			INNER JOIN obras.pretipoobra pto ON pto.ptoid = pre.ptoid AND pto.ptoclassificacaoobra = pro.protipo
			".$inn."
			WHERE
				pro.proid = '".$proid."' ".$strWhere." 
			GROUP BY
				pre.preid, pre.predescricao, pto.ptodescricao, prevalorobra
			ORDER BY
				pto.ptodescricao,pre.predescricao";

	echo "<form id=\"formpreobras\">";
	$cabecalho = array("&nbsp;","Nome da obra","Tipo da Obra","Valor da obra");
	$celWidth  = array("5%","45%","25%","25%");
	$celAlign  = array("center","left","left","right");
	if( $_REQUEST['tipo'] == 'simples' ){
		$db->monta_lista_simples($sql,$cabecalho,500,1,'N','','S' );
	}else{
		$db->monta_lista($sql,$cabecalho,500,5,'N','','S',"formpreobras",$celWidth,$celAlign);
	}
	echo "</form>";

}

function assinarTermo($dados) {
	global $db;
	
	if($dados['terid']) {
		$sql = "UPDATE par.termocompromissopac SET terassinado=".$dados['terassinado']." WHERE terid='".$dados['terid']."'";
		$db->executar($sql);
		$db->commit();
	}
}

function inseriranexotermoprocesso($dados) {
	global $db;
	
	if($_FILES['arquivo']['name']){
		include_once APPRAIZ . "includes/classes/fileSimec.class.inc";
		$arrCampos = array();
		$file = new FilesSimec("processoobraanexo",$arrCampos,"par");
		$file->setUpload("","arquivo",false);
		$arqid = $file->getIdArquivo();
	}
	
	if($arqid) {
		
		$sql = "INSERT INTO par.processoobraanexo(
	            usucpf, poadatainclusao, poastatus, arqid, proid)
	    		VALUES ('".$_SESSION['usucpf']."', NOW(), 'A', '".$arqid."', '".$_POST['proid']."');";
		
		$db->executar($sql);
		$db->commit();
		
	}
	
	
	die("<script>
			alert('Anexado com sucesso.');
			window.location='".md5_decrypt($dados['urlgo'])."';
		 </script>");
}

?>