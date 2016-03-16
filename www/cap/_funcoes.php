<?php
function pegarPerfil($usucpf){
	global $db;

	$sql = "SELECT          pu.pflcod
            FROM            seguranca.perfilusuario pu
            INNER JOIN      seguranca.perfil p ON p.pflcod = pu.pflcod
            AND             pu.usucpf = '{$usucpf}'
            AND             p.sisid = {$_SESSION['sisid']}
            AND             pflstatus = 'A'";

	$arrPflcod = $db->carregar($sql);
	!$arrPflcod? $arrPflcod = array() : $arrPflcod = $arrPflcod;
	$arrPerfil = array();
	foreach($arrPflcod as $pflcod){
		$arrPerfil[] = $pflcod['pflcod'];
	}
	return $arrPerfil;
}

function exibirListaDocAnexo($tipo,$perfil,$habilitado){
	global $db;

	$aryWhere[] = "af.arastatus = 'A'";

	if($_SESSION['cap']['afpid']){
		$aryWhere[] = "af.afpid = {$_SESSION['cap']['afpid']}";
		$docid = criarDocumento($_SESSION['cap']['afpid']);
		$esdid = pegarEstadoDocumento($docid);
	}  else {
		$aryWhere[] = "af.afpid = 0";
	}

	if(in_array(CAP_PERFIL_SUPER_USUARIO,$perfil) || in_array(CAP_PERFIL_ADMINISTRADOR,$perfil) || (in_array(CAP_PERFIL_SERVIDOR,$perfil) && $esdid != WF_VIAGEM_FINALIZADA)){
		$acao = "'<a href=\"cap.php?modulo=principal/documentos&acao=A&download=S&arqid='|| ar.arqid ||'\" ><img src=\"../imagens/anexo.gif\" border=\"0\"></a>&nbsp;&nbsp;
				  <img border=\"0\" src=\"../imagens/excluir.gif\" id=\"'|| af.arqid ||'\" onclick=\"excluirDocAnexo('|| af.arqid ||');\" style=\"cursor:pointer;\"/>' AS acao,";
	} else {
		$acao = "'<a href=\"cap.php?modulo=principal/documentos&acao=A&download=S&arqid='|| ar.arqid ||'\" ><img src=\"../imagens/anexo.gif\" border=\"0\"></a>' AS acao,";
	}

	if($habilitado == 'N'){
		$acao = "'<a href=\"cap.php?modulo=principal/documentos&acao=A&download=S&arqid='|| ar.arqid ||'\" ><img src=\"../imagens/anexo.gif\" border=\"0\"></a>' AS acao,";
	}

	if($tipo == 'documentos'){
		$cabecalho = array('Ação', 'Data Inclusão', 'Tipo Documento', 'Número do Documento', 'Nome Arquivo', 'Descrição', 'Responsável');
		$select = "to_char(aradtinclusao,'DD/MM/YYYY'),
				   td.tpddsc,
				   ar.arqid,";
	} else {
		$cabecalho = array('Ação', 'Nome Arquivo', 'Descrição', 'Responsável');
		$select = "";
		$aryWhere[] = "td.tpdid = 6";
	}

	$sql = "SELECT 		$acao
						$select
						ar.arqnome||'.'||ar.arqextensao as nome_arquivo,
						ar.arqdescricao,
						us.usunome
		    FROM 		cap.arquivoafastamento af
		    INNER JOIN  public.arquivo ar ON ar.arqid = af.arqid
		    INNER JOIN  cap.tipodocumento td ON td.tpdid = af.tpdid
		    INNER JOIN	seguranca.usuario us ON us.usucpf = ar.usucpf
		    			".(is_array($aryWhere) ? ' WHERE '.implode(' AND ', $aryWhere) : '')."
		   	ORDER BY 	af.aradtinclusao DESC";
	//ver($sql,d);
	$db->monta_lista($sql, $cabecalho, '50', '10', '', '', '', '');
}

function excluirDocAnexo($arqid) {
	global $db;
	if ($arqid != '') {
		$sql = "UPDATE cap.arquivoafastamento SET arastatus = 'I' WHERE arqid = {$arqid} ";
	}

	if( $db->executar($sql) ){
		$db->commit();
	}
}

function salvarDocAnexo($file,$post){
	global $db;
	include_once APPRAIZ . "includes/classes/fileSimec.class.inc";

	extract($post);

	if( $file['arquivo']['tmp_name'] ){
		$aryCampos = array("afpid" => $afpid,"arastatus" => "'A'","aradtinclusao" => "now()","tpdid" => $tpdid);
		$file = new FilesSimec("arquivoafastamento",$aryCampos,"cap");
		$file->setUpload($arqdescricao,"arquivo");
		header("Location: cap.php?modulo=principal/documentos&acao=A");
		exit();
	} else {
		$_SESSION['cap']['mgs'] = "Não foi possível realizar a operação!";
		header("Location: cap.php?modulo=principal/documentos&acao=A");
		exit();
	}
}

function exibirListaPublicacao($perfil){
	global $db;

	$aryWhere[] = "ap.arpstatus = 'A'";

	if($_SESSION['cap']['afpid']){
		$aryWhere[] = "pu.afpid = {$_SESSION['cap']['afpid']}";
		$docid = criarDocumento($_SESSION['cap']['afpid']);
		$esdid = pegarEstadoDocumento($docid);
	} else {
		$aryWhere[] = "pu.afpid = 0";
	}

	if((in_array(CAP_PERFIL_SUPER_USUARIO,$perfil) || in_array(CAP_PERFIL_ADMINISTRADOR,$perfil)) && $esdid != WF_VIAGEM_FINALIZADA){
		$acao = "'<a href=\"cap.php?modulo=principal/documentos&acao=A&download=S&arqid='|| ap.arqid ||'\" ><img src=\"../imagens/anexo.gif\" border=\"0\"></a>&nbsp;&nbsp;
				  <img border=\"0\" src=\"../imagens/excluir.gif\" id=\"'|| ap.arqid ||'\" onclick=\"excluirDocAnexo('|| ap.arqid ||');\" style=\"cursor:pointer;\"/>' AS acao,";
	} else {
		$acao = "'<a href=\"cap.php?modulo=principal/documentos&acao=A&download=S&arqid='|| ar.arqid ||'\" ><img src=\"../imagens/anexo.gif\" border=\"0\"></a>' AS acao,";
	}

	$cabecalho = array('Ação', 'Descrição', 'Nome do Arquivo', 'Data da Inclusão');

	$sql = "SELECT 		$acao
						ar.arqnome||'.'||ar.arqextensao as nome_arquivo,
						ar.arqdescricao,
						to_char(ap.arpdtinclusao,'DD/MM/YYYY') AS arpdtinclusao
		    FROM 		cap.publicacao pu
		    INNER JOIN	cap.arquivopublicacao ap ON pu.pblid = ap.pblid
		    INNER JOIN  public.arquivo ar ON ar.arqid = ap.arqid
		    			".(is_array($aryWhere) ? ' WHERE '.implode(' AND ', $aryWhere) : '')."
		   	ORDER BY 	ap.arpdtinclusao DESC";

	$db->monta_lista($sql, $cabecalho, '50', '10', '', '', '', '');
}

function excluirPublicacao($arqid) {
	global $db;
	if ($arqid != '') {
		$sql = "UPDATE cap.arquivopublicacao SET arpstatus = 'I' WHERE arqid = {$arqid}";
	}

	if( $db->executar($sql) ){
		$db->commit();
	}
}

function salvarPublicacao($file,$post){
	global $db;
	include_once APPRAIZ . "includes/classes/fileSimec.class.inc";

	extract($post);

	$arqdescricao = "Diário ".$pbldtdiario;
	$pbldtdiario = formata_data_sql($pbldtdiario);
	$pbldtdiario = $pbldtdiario ? "'{$pbldtdiario}'" : "null";

	$sql = "INSERT INTO 	cap.publicacao
				(afpid,
	             pblnumsecao,
	             pblpagina,
	             pbldtdiario)
    	 	VALUES
    	 		 ({$afpid},
            	 '{$pblnumsecao}',
            	 '{$pblpagina}',
            	 {$pbldtdiario})
            RETURNING pblid";

	$pblid = $db->pegaUm($sql);
	$db->commit();

	if($file['arquivo']['tmp_name'] && $pblid){
		$aryCampos = array("pblid" => $pblid,"arpstatus" => "'A'","arpdtinclusao" =>'now()');
		$file = new FilesSimec("arquivopublicacao",$aryCampos,"cap");
		$file->setUpload($arqdescricao,"arquivo");
		header("Location: cap.php?modulo=principal/publicacoes&acao=A");
		exit();
	} else {
		$_SESSION['cap']['mgs'] = "Não foi possível realizar a operação!";
		header("Location: cap.php?modulo=principal/publicacoes&acao=A");
		exit();
	}
}

    function pesquisarViagem($perfil, $post = null, $afpid = null,$flag_sublista = null){
	global $db;

        $docid = criarDocumento($_SESSION['cap']['afpid']);
        $estado_doc = pegarEstadoDocumento($docid);
        
	if($post){
            extract($post);
	}

	$aryWhere[] = "afa.afpstatus = 'A'";

	if(!empty($afpnumsiape)){
            $aryWhere[] = "afa.afpnumsiape = '{$afpnumsiape}'";
	}

	if(!empty($fdpnome)){
            $aryWhere[] = "sim.no_servidor ILIKE '%{$fdpnome}%'";
	}

	if(!empty($dataini) && !empty($datafim)){
            $dataini = formata_data_sql($dataini);
            $datafim = formata_data_sql($datafim);

            $aryWhere[] = "('{$dataini}' BETWEEN afa.afpdtrealizacaoinicial AND afa.afpdtrealizacaofinal)";
            $aryWhere[] = "('{$datafim}' BETWEEN afa.afpdtrealizacaoinicial AND afa.afpdtrealizacaofinal)";
	}

	if(!empty($uamid)){
            $aryWhere[] = "afa.uamid = {$uamid}";
	}

	if(!empty($esdid)){
            $aryWhere[] = "doc.esdid = {$esdid}";
	}

	if(!empty($cidade)){
            $cidade = utf8_decode($cidade);
            $join_cidade = "INNER JOIN (SELECT at.afpid FROM cap.afastamentotrecho at INNER JOIN cap.cidadepais cp ON at.cdpid = cp.cdpid WHERE cp.cdpdsc ILIKE '%{$cidade}%' GROUP BY cp.cdpdsc, at.afpid) as cidade ON cidade.afpid = afa.afpid";
	} else {
            $join_cidade = "";
	}

	if(!empty($paiid)){
            $join_pais = "INNER JOIN (SELECT afpid FROM cap.afastamentotrecho WHERE paiid = {$paiid} GROUP BY paiid, afpid) as pais ON pais.afpid = afa.afpid";
	} else {
            $join_pais = "";
	}

	if(!empty($expressao_chave)){
            $aryWhere[] = "afa.afttxtobjetivoviagem ILIKE '%{$expressao_chave}%'";
            $aryWhere[] = "afa.afpvincservico ILIKE '%{$expressao_chave}%'";
            $aryWhere[] = "afa.afprelevancia ILIKE '%{$expressao_chave}%'";
            $aryWhere[] = "afa.afppertinencia ILIKE '%{$expressao_chave}%'";
	}

	if(in_array(CAP_PERFIL_SUPER_USUARIO,$perfil) || in_array(CAP_PERFIL_ADMINISTRADOR,$perfil) || in_array(CAP_PERFIL_GABINETE,$perfil)){
            $cabecalho = array('Ação', 'Seq', 'Nº do Processo', 'Nº do SIAPE', 'Nome do Servidor', 'Países', 'Cidades', 'Situação', 'Rel. Viagem', 'Período', 'Responsável','Data da Inclusão','');
            $select = "afa.afpnumsiape, sim.no_servidor,";
	} else {
            $cabecalho = array('Ação', 'Seq', 'Nº do Processo', 'Países', 'Cidades', 'Situação', 'Rel. Viagem', 'Período', 'Responsável','Data da Inclusão','');
            $select = "";
            $aryWhere[] = "afa.fdpcpf = '{$_SESSION['usucpf']}'";
	}

	if(in_array(CAP_PERFIL_SUPER_USUARIO,$perfil) || in_array(CAP_PERFIL_ADMINISTRADOR,$perfil) || in_array(CAP_PERFIL_SERVIDOR,$perfil)){
            $acao = "
                <img src=\"../imagens/alterar.gif\" id=\"' || afa.afpid ||'\" class=\"alterar\" onclick=\"alterarFormulario('|| afa.afpid ||');\" style=\"cursor:pointer;\"/>
                <img src=\"../imagens/excluir.gif\" id=\"' || afa.afpid ||'\" class=\"excluir\" onclick=\"excluirFormulario('|| afa.afpid ||');\" style=\"cursor:pointer;\"/>
            ";
	} else {
            $acao = "
                <img src=\"../imagens/alterar.gif\" id=\"' || afa.afpid ||'\" class=\"alterar\" onclick=\"alterarFormulario('|| afa.afpid ||');\" style=\"cursor:pointer;\"/> 
                <img src=\"../imagens/excluir_01.gif\" id=\"' || afa.afpid ||'\" class=\"excluir\" style=\"cursor:pointer;\"/>
            ";           
	}
        
        $acao_d = "
            <img src=\"../imagens/alterar.gif\" id=\"' || afa.afpid ||'\" class=\"alterar\" onclick=\"alterarFormulario('|| afa.afpid ||');\" style=\"cursor:pointer;\"/>
            <img src=\"../imagens/excluir_01.gif\" id=\"' || afa.afpid ||'\" class=\"excluir\" style=\"cursor:pointer;\"/>
        ";

	$colspan_filhos = count($cabecalho);

	if(!$flag_sublista){
		$aryWhere[] = "afa.afpidorigem is null";
	}else{
		$aryWhere[] = "afa.afpidorigem = $afpid";
	}

	$sql = "
            SELECT  CASE WHEN esd.esdid = ".WF_CADASTRO_FORMULARIO_VAIAGEM."
                        THEN '{$acao}'
                        ELSE '{$acao_d}'
                    END AS acao,
                    
                    CASE WHEN afac.contador > 0
                        THEN afa.afpid || ' <a href=\'javascript:void(0);\' onclick=\'montaSubLista('|| afa.afpid ||')\'><img id=\'img_mais_'|| afa.afpid ||'\' src=\'../imagens/mais.gif\' border=\'0\' style=\'vertical-align: baseline;\'></a> <a href=\'javascript:void(0);\' onclick=\'desmontaSubLista('|| afa.afpid ||')\'><img id=\'img_menos_'|| afa.afpid ||'\' src=\'../imagens/menos.gif\' border=\'0\' style=\'vertical-align: baseline;display:none\'></a>'
                        ELSE afa.afpid || ' '
                    END as afpid,
                    
                    afa.afpnumprocesso || ' ',

                    {$select}

                    array_to_string(array(SELECT DISTINCT te.prddsc FROM cap.afastamentotrecho at INNER JOIN cap.paisdiarias te ON at.idpdr = te.idpdr WHERE at.afpid = afa.afpid),',') AS paises,
                    array_to_string(array(SELECT DISTINCT cp.cdpdsc FROM cap.afastamentotrecho at INNER JOIN cap.cidadepais cp ON at.cdpid = cp.cdpid WHERE at.afpid = afa.afpid AND cp.cdpdsc ILIKE '%{$cidade}%' GROUP BY cp.cdpdsc, at.afpid),',') AS cidades,
                    esd.esddsc,
                    '' as rel_viagem,
                    afa.fdpcpf,
                    afa.afpcargofuncao,
                    afa.afptelefone,
                    afa.afppertraninicial, 
                    afa.afppertranfinal,
                    raf.rafatividadesfatos,
                    raf.rafconclusao,
                    raf.rafsugestao,
                    raf.rafobservacao,
                    array_to_string(array(SELECT DISTINCT te.prddsc FROM cap.afastamentotrecho at INNER JOIN cap.paisdiarias te ON at.idpdr = te.idpdr  WHERE at.afpid = afa.afpid),',') AS pais,
                    array_to_string(array(SELECT DISTINCT cp.cdpdsc FROM cap.afastamentotrecho at INNER JOIN cap.cidadepais cp ON at.cdpid = cp.cdpid WHERE at.afpid = afa.afpid AND cp.cdpdsc ILIKE '%{$cidade}%' GROUP BY cp.cdpdsc, at.afpid),',') AS cidade,
                    to_char(afa.afpdtrealizacaoinicial,'DD/MM/YYYY') || ' a ' || to_char(afa.afpdtrealizacaofinal,'DD/MM/YYYY') AS periodo,
                    usu.usunome,
                    to_char(afa.afpdtinclusao, 'DD/MM/YYYY') as afpdtinclusao,
                    '<tr style=\"background-color:#F7F7F7\" ><td colspan={$colspan_filhos} style=\"padding-left:20px;\" id=\"td_' || afa.afpid || '\" ></td></tr>' AS origem

            FROM cap.afastamento afa

            LEFT JOIN (SELECT nu_cpf, no_servidor from siape.tb_servidor_simec group by nu_cpf, no_servidor ) sim ON sim.nu_cpf = afa.fdpcpf
            LEFT JOIN (SELECT afpid, rafatividadesfatos, rafconclusao, rafsugestao, rafobservacao FROM cap.relatorioafastamento ORDER BY rafid DESC LIMIT 1) AS raf ON raf.afpid = afa.afpid
            LEFT JOIN (SELECT afpidorigem,  count(afpid) as contador from cap.afastamento group by afpidorigem ) afac ON afac.afpidorigem = afa.afpid
            LEFT JOIN workflow.documento doc on doc.docid = afa.docid
            LEFT JOIN workflow.estadodocumento esd on esd.esdid = doc.esdid
            LEFT JOIN seguranca.usuario AS usu ON usu.usucpf = afa.afpusucpf

            {$join_cidade}
            {$join_pais}

            ".(is_array($aryWhere) ? ' WHERE '.implode(' AND ', $aryWhere) : '')."

            ORDER BY afa.afpid
        ";
	$aResultado = $db->carregar($sql);

	if (!empty($aResultado)) {
            if ($aResultado) {
                $aVerificacao = array('fdpcpf', 'afpcargofuncao', 'afptelefone', 'afppertraninicial', 'afppertranfinal', 'rafatividadesfatos', 'rafconclusao', 'rafsugestao', 'rafobservacao', 'pais', 'cidade');

                foreach ($aResultado as $count => $linha) {
                    $status = '<div align="center"><img src="../imagens/icones/bg.png"/ title="Relatório de Viagem - Preenchimento Completo "></div>';
                    foreach ($aVerificacao as $campo){
                        if (!trim($linha[$campo])) {
                            $status = '<div align="center"><img src="../imagens/icones/br.png"/ title="Relatório de Viagem - Preenchimento Parcial"></div>';
                            break;
                        }
                    }
                    foreach ($aVerificacao as $campo) {
                        unset($aResultado[$count][$campo]);
                    }
                    $aResultado[$count]['rel_viagem'] = $status;
                }
            }
        } else {
            $aResultado = array();
        }
        
        if (!$flag_sublista) {
            $db->monta_lista($aResultado, $cabecalho, '50', '10', '', '', '', '');
        } else {
            $db->monta_lista_simples($aResultado, $cabecalho, '50', '10', 'N', '', '');
        }
    }


function montaSubLista($afpid){
	global $db;

	$perfil = pegarPerfil($_SESSION['usucpf']);

	pesquisarViagem($perfil, $post = null, $afpid, $flag_sublista = 1);

}


function salvarFormularioID($post){
	global $db;

	extract($post);

	$fdpcpf = corrige_cpf($fdpcpf);

	$sql = "
            INSERT INTO cap.afastamento(
                    afpcargofuncao, afptelefone, afpstatus, afpdtinclusao, afpusucpf, afpemailservidor, fdpcpf, afpnumsiape, uamid, afpnivelcargo, afpfuncao
                ) VALUES (
                    '{$afpcargofuncao}', '{$afptelefone}', 'A', now(), '{$_SESSION['usucpf']}', '{$afpemailservidor}', '{$fdpcpf}', {$afpnumsiape}, {$uamid}, '{$afpnivelcargo}', '{$afpfuncao}'
            ) RETURNING afpid
        ";

	$afpid = $db->pegaUm($sql);
	$db->commit();

    if($afpid){
		gerarNumeroProcesso($afpid);
    	header("Location:cap.php?modulo=principal/formulario&acao=A&afpid=$afpid");
    } else {
    	header("Location:cap.php?modulo=principal/formulario&acao=A");
    }
}

function alterarFormulario($file,$post){
	global $db;
    #ver($post,d);
	extract($post);

    $afpdtrealizacaoinicial     = formata_data_sql($afpdtrealizacaoinicial);
    $afpdtrealizacaofinal       = formata_data_sql($afpdtrealizacaofinal);
    $afppertraninicial          = formata_data_sql($afppertraninicial);
    $afppertranfinal            = formata_data_sql($afppertranfinal);
    $afpdtrealizacaoinicial     = $afpdtrealizacaoinicial ? "'{$afpdtrealizacaoinicial}'" : "null";
    $afpdtrealizacaofinal       = $afpdtrealizacaofinal ? "'{$afpdtrealizacaofinal}'" : "null";
    $afppertraninicial          = $afppertraninicial ? "'{$afppertraninicial}'" : "null";
    $afppertranfinal            = $afppertranfinal ? "'{$afppertranfinal}'" : "null";

    $afpvlrpassagem             = $afpvlrpassagem ? MoedaToBd($afpvlrpassagem) : "null";
    $afpvlrtotalbolsa           = $afpvlrtotalbolsa ? MoedaToBd($afpvlrtotalbolsa) : "null";
    $afpvlrtotaldiarias         = $afpvlrtotaldiarias ? MoedaToBd($afpvlrtotaldiarias) : "null";

    $afpnumdiarias              = $afpnumdiarias ? $afpnumdiarias : "null";
    $afpnumsiape                = $afpnumsiape ? $afpnumsiape : "null";

    $afpsitdiarias              = $afpsitdiarias ? "'{$afpsitdiarias}'" : "null";
    $afpsitpassagem             = $afpsitpassagem ? "'{$afpsitpassagem}'" : "null";

    $afttxtobjetivoviagem       = addslashes($afttxtobjetivoviagem);
    $afpvincservico             = addslashes($afpvincservico);
    $afprelevancia              = addslashes($afprelevancia);
    $afppertinencia             = addslashes($afppertinencia);
    $afpjustifespecial          = addslashes($afpjustifespecial);

	if($tonid != 1 && $tonid != 4){
        $afpsitdiarias = "null";
        $afpsitpassagem = "null";
        $afpnumdiarias = "null";
        $afpvlrtotaldiarias = "null";
        $afpvlrpassagem = "null";
        $afpclasse = "";
        $afptrecho = "";
	}

    if($tonid == 2 || $tonid == 3){
        $afporgaofinanciador = "";
    }

	$fdpcpf = corrige_cpf($fdpcpf);
	$tpdid = '6';

	$sql = "
        UPDATE cap.afastamento
        	SET tonid = {$tonid},
            afpcargofuncao = '{$afpcargofuncao}',
            afptelefone = '{$afptelefone}',
            afpdtrealizacaoinicial = $afpdtrealizacaoinicial,
            afpdtrealizacaofinal = $afpdtrealizacaofinal,
            afppertraninicial = $afppertraninicial,
            afppertranfinal = $afppertranfinal,
            afporgaofinanciador = '{$afporgaofinanciador}',
            afpvlrpassagem = {$afpvlrpassagem},
            afpclasse = '{$afpclasse}',
            afptrecho = '{$afptrecho}',
            afpnumdiarias = {$afpnumdiarias},
            afpvlrtotaldiarias = {$afpvlrtotaldiarias},
            afpvlrtotalbolsa = {$afpvlrtotalbolsa},
            afttxtobjetivoviagem = '{$afttxtobjetivoviagem}',
            afpvincservico = '{$afpvincservico}',
            afprelevancia = '{$afprelevancia}',
            afppertinencia = '{$afppertinencia}',
            afpjustifespecial = '{$afpjustifespecial}',
            afpnumsiape = {$afpnumsiape},
            uamid = {$uamid},
            afpsitdiarias = $afpsitdiarias,
            afpsitpassagem = $afpsitpassagem,
            afpemailservidor = '{$afpemailservidor}'
        WHERE afpid = {$afpid}
    ";

	$db->executar($sql);
	$db->commit();

    if( $file['arquivo']['tmp_name'] ){
        $aryCampos = array("afpid" => $afpid,"arastatus" => "'A'","aradtinclusao" => "now()","tpdid" => $tpdid);
        $file = new FilesSimec("arquivoafastamento",$aryCampos,"cap");
        $file->setUpload($arqdescricao,"arquivo");
        $arqid = $file->getIdArquivo();
    } else {
        $_SESSION['cap']['mgs'] = "Não foi possível realizar o Upload da cópia da passagem!";
    }

    if($afpid){
         header("Location:cap.php?modulo=principal/formulario&acao=A&afpid=$afpid");
    } else {
         header("Location:cap.php?modulo=principal/formulario&acao=A");
    }
}

function excluirFormulario($afpid){
	global $db;

	if ($afpid != '') {
		$sql = "UPDATE cap.afastamento SET afpstatus = 'I' WHERE afpid = {$afpid} ";
	}

	if($db->executar($sql)){
		$db->commit();
	}
}

    function exibirDadosFormulario($afpid){
	global $db;

	$sql = "
            SELECT  afpid,
                    afpidorigem,
                    tonid,
                    afpcargofuncao,
                    afptelefone,
                    afpdtrealizacaoinicial,
                    afpdtrealizacaofinal,
                    afppertraninicial,
                    afppertranfinal,
                    afporgaofinanciador,
                    trim(to_char(afpvlrpassagem, '9G999G999D99')) AS afpvlrpassagem,
                    afpclasse,
                    afptrecho,
                    afpnumdiarias,
                    trim(to_char(afpvlrtotaldiarias, '9G999G999D99')) AS afpvlrtotaldiarias,
                    trim(to_char(afpvlrtotalbolsa, '9G999G999D99')) AS afpvlrtotalbolsa,
                    afttxtobjetivoviagem,
                    afpvincservico,
                    afprelevancia,
                    afppertinencia,
                    afpjustifespecial,
                    trim(replace(to_char(cast(sim.nu_cpf AS bigint), '000:000:000-00'), ':', '.')) as fdpcpf,
                    afpnumsiape,
                    trim(sim.no_servidor) AS fdpnome,
                    afa.uamid,
                    to_char(afpdtinclusao,'DD/MM/YYYY') as afpdtinclusao,
                    uam.uamdsc,
                    afpnumprocesso,
                    afpsitdiarias,
                    afpsitpassagem,
                    afpnivelcargo,
                    afpfuncao,
                    afpemailservidor
            FROM cap.afastamento afa
            LEFT JOIN siape.tb_servidor_simec sim ON sim.nu_cpf = afa.fdpcpf
            LEFT JOIN public.unidadeareamec uam ON uam.uamid = afa.uamid
            WHERE afpid = {$afpid}
        ";
        $aryDados = $db->pegaLinha($sql);

	if(empty($aryDados)){
            return array();
	} else {
            return $aryDados;
	}
    }

    function salvarEvento($post){
	global $db;

	extract($post);

	$aftdtinicio    = formata_data_sql($aftdtinicio);
        $aftdtfinal     = formata_data_sql($aftdtfinal);
        $cdpdsc         = strtolower(trim($cdpdsc));

        $existeevento = verificarPeriodoEvento($post);
        if( $existeevento == "S" ){
	    if( $idpdr != '' ){
	    	$cdpdsc = ucfirst($cdpdsc);

	    	$cdpdsc = $cdpdsc ? utf8_decode($cdpdsc) : '';
                $sql = "INSERT INTO cap.cidadepais(idpdr, cdpdsc) VALUES ({$idpdr}, '{$cdpdsc}') RETURNING cdpid";
		$cdpid = $db->pegaUm($sql);
	    }

	    $aftobjetivo = $aftobjetivo ? utf8_decode($aftobjetivo) : '';

            if( $cdpid > 0 ){
                $sql = "
                    INSERT INTO cap.afastamentotrecho(
                        afpid, idpdr, cdpid, aftdtinicio, aftdtfinal, aftobjetivo
                    )VALUES(
                        {$afpid}, {$idpdr}, {$cdpid}, '{$aftdtinicio}', '{$aftdtfinal}', '{$aftobjetivo}'
                    ) RETURNING aftid
                ";
                $aftid = $db->pegaUm($sql);
            }
            
	    if( $aftid > 0 ){
                $db->commit();
	    	echo "S";
	    } else {
                $db->rollback();
	    	echo "N";
	    }
    } else {
    	echo "E";
    }
}

function alterarEvento($post){
	global $db;

	extract($post);

	$aftdtinicio = formata_data_sql($aftdtinicio);
    $aftdtfinal = formata_data_sql($aftdtfinal);
    $cdpdsc = strtolower(trim($cdpdsc));

    $sql = "SELECT cdpid FROM cap.cidadepais WHERE idpdr = {$idpdr} AND LOWER(cdpdsc) = '{$cdpdsc}'";
    $cdpid = $db->pegaUm($sql);

    if(empty($cdpid)){
	    $cdpdsc = ucfirst($cdpdsc);

	    $cdpdsc = $cdpdsc ? utf8_decode($cdpdsc) : '';

	    $sql = "INSERT INTO cap.cidadepais(idpdr, cdpdsc) VALUES ({$idpdr}, '{$cdpdsc}') RETURNING cdpid";
	    $cdpid = $db->pegaUm($sql);
		$db->commit();
    }

    $aftobjetivo = $aftobjetivo ? utf8_decode($aftobjetivo) : '';

	$sql = "UPDATE 	cap.afastamentotrecho
			SET		idpdr = {$idpdr},
				    cdpid = {$cdpid},
				 	aftdtinicio = '{$aftdtinicio}',
				 	aftdtfinal = '{$aftdtfinal}',
				 	aftobjetivo = '{$aftobjetivo}'
			WHERE	aftid = {$aftid}";

	$db->executar($sql);

    if($db->commit()){
    	echo "S";
    } else {
    	echo "N";
    }
}

function excluirEvento($aftid){
	global $db;

	if ($aftid != '') {
		$sql = "DELETE FROM cap.afastamentotrecho WHERE aftid = {$aftid} ";
	}

	if( $db->executar($sql) ){
		$db->commit();
	}
}

function exibirDadosEvento($aftid){
	global $db;

	$sql = "SELECT 		aft.aftid,
						aft.afpid,
						aft.idpdr,
						pai.prddsc,
						aft.aftdtinicio,
						aft.aftdtfinal,
						aft.aftobjetivo
  			FROM 		cap.afastamentotrecho aft
  			INNER JOIN	cap.paisdiarias pai ON pai.idpdr = aft.idpdr
	  		WHERE		aftid = {$aftid}";

	$aryDados = $db->pegaLinha($sql);

	if(empty($aryDados)){
		return array();
	} else {
		return $aryDados;
	}
}

function exibirListaEvento($perfil, $habilitado = null, $afpfuncao = null){
	global $db;

	switch ($afpfuncao) {
            case 6:
                $campoFuncao = "pdrdas6";
                break;
            case 5:
                $campoFuncao = "pdrdas5";
                break;
            case 4:
                $campoFuncao = "pdrdas43";
                break;
            case 2:
                $campoFuncao = "pdrdas21nivsup";
                break;
            case 'N':
                $campoFuncao = "pdrdasnivmedio";
                break;
            default :
                $campoFuncao = "pdrdasnivmedio";
                break;
        }

	if($_SESSION['cap']['afpid']){
            if(in_array(CAP_PERFIL_SUPER_USUARIO,$perfil) || in_array(CAP_PERFIL_ADMINISTRADOR,$perfil) || in_array(CAP_PERFIL_SERVIDOR,$perfil)){
                $cabecalho = array('Ação', 'País', 'Cidade','Período', 'Atividade','Nº de Diárias', 'Valor Unitário da Diária');
                $acao = "'<img src=\"../imagens/alterar.gif\" id=\"' || aft.aftid ||'\" class=\"alterar\" onclick=\"alterarEvento('|| aft.aftid ||');\" style=\"cursor:pointer;\"/>&nbsp;<img src=\"../imagens/excluir.gif\" id=\"' || aft.aftid ||'\" class=\"excluir\" onclick=\"excluirEvento('|| aft.aftid ||');\" style=\"cursor:pointer;\"/>' as acao,";
            } else {
                $cabecalho = array('País', 'Cidade', 'Período', 'Atividade','Nº de Diárias', 'Valor Unitário da Diária');
                $acao = "";
            }

            if($habilitado == 'N'){
                $cabecalho = array('País', 'Cidade', 'Período', 'Atividade','Nº de Diárias', 'Valor Unitário da Diária');
                $acao = "";
            }

            $sql_pais = "
                SELECT
                    aft.idpdr,
                    pai.prddsc
                FROM cap.afastamentotrecho aft
                INNER JOIN cap.paisdiarias pai ON pai.idpdr = aft.idpdr
                WHERE aft.afpid = {$_SESSION['cap']['afpid']}
                GROUP BY 1,2
                ORDER BY pai.prddsc
            ";
            $result = $db->carregar($sql_pais);
            if($result){
                foreach($result as $rs){
                    $sql = "
                        SELECT  {$acao}
                            pai.prddsc,
                            cidade.cdpdsc,
                            to_char(aft.aftdtinicio,'DD/MM/YYYY') || ' a ' || to_char(aft.aftdtfinal,'DD/MM/YYYY') AS periodo,
                            aft.aftobjetivo,
                            sum( DATE(aft.aftdtfinal) - DATE(aft.aftdtinicio) ) + 1  as numero_diaria,
                            'R$ ' || {$campoFuncao}
                        FROM cap.afastamentotrecho aft
                        INNER JOIN cap.paisdiarias pai ON pai.idpdr = aft.idpdr
                        INNER JOIN cap.cidadepais cidade ON cidade.cdpid = aft.cdpid
                        WHERE aft.afpid = {$_SESSION['cap']['afpid']} AND aft.idpdr = {$rs['idpdr']} AND aft.afpid = {$_SESSION['cap']['afpid']}
                        GROUP BY {$campoFuncao} ,aft.aftid, aft.aftobjetivo, pai.prddsc, cidade.cdpdsc, aft.aftdtfinal, aft.aftdtinicio
                        ORDER BY pai.prddsc
                    ";
                    $db->monta_lista($sql, $cabecalho, '50', '10', '', '', '', '');
                }
            }else{
                $sql = array();
                $db->monta_lista($sql, $cabecalho, '50', '10', '', '', '', '');
            }
	}
}

function salvarRelatorio($post){
	global $db;

	extract($post);

	$sql = "INSERT INTO cap.relatorioafastamento
				(afpid,
				 rafatividadesfatos,
				 rafconclusao,
				 rafsugestao,
				 rafobservacao,
				 rafdtinclusao,
				 rafstatus)
   			 VALUES
   			 	({$afpid},
   			 	 '{$rafatividadesfatos}',
   			 	 '{$rafconclusao}',
   			 	 '{$rafsugestao}',
   			 	 '{$rafobservacao}',
   			 	 now(),
   			 	 'A')";

	$db->executar($sql);
	$db->commit();
}

function alterarRelatorio($post){
	global $db;

	extract($post);

	$sql = "UPDATE 	cap.relatorioafastamento
			SET		rafatividadesfatos = '{$rafatividadesfatos}',
				    rafconclusao = '{$rafconclusao}',
				 	rafsugestao = '{$rafsugestao}',
				 	rafobservacao = '{$rafobservacao}'
			WHERE	rafid = {$rafid}";

	$db->executar($sql);
	$db->commit();
}

function exibirDadosRelatorioCabecalho($afpid){
	global $db;

	$sql = "SELECT 		afpid,
						afpcargofuncao,
						afptelefone,
						to_char(afppertraninicial,'DD/MM/YYYY') AS afpdtrealizacaoinicial,
	       				to_char(afppertranfinal,'DD/MM/YYYY') AS afpdtrealizacaofinal,
	       				afptrecho,
	       				afpnumsiape,
	       				trim(sim.no_servidor) AS fdpnome,
	       				uam.uamsigla || ' - ' || uam.uamdsc as orgao
  		FROM			cap.afastamento afa
  		LEFT JOIN		siape.tb_servidor_simec sim ON sim.nu_cpf = afa.fdpcpf
  		LEFT JOIN 		public.unidadeareamec uam ON uam.uamid = afa.uamid
  		WHERE			afpid = {$afpid}";

	$aryDados = $db->pegaLinha($sql);

	if($aryDados){
		return $aryDados;
	} else {
		return array();
	}
}

function exibirDadosRelatorio($afpid){
	global $db;

	$sql = "SELECT 			raf.rafid,
							raf.afpid,
							raf.rafatividadesfatos,
							raf.rafconclusao,
							raf.rafsugestao,
       						raf.rafobservacao
 			FROM  			cap.relatorioafastamento raf
 			LEFT JOIN		cap.afastamento afa ON raf.afpid = afa.afpid
	  		WHERE			raf.rafstatus = 'A' AND raf.afpid = {$afpid}
	  		ORDER BY 		raf.rafid DESC
	  		LIMIT 			1";

	$aryDados = $db->pegaLinha($sql);

	if($aryDados){
		return $aryDados;
	} else {
		return array();
	}
}

function buscarDadosServidor($dados){
	global $db;

	$fdpsiape   = trim($dados['fdpsiape']);
	$fdpcpf     = trim(corrige_cpf($dados['fdpcpf']));

	if(empty($fdpsiape) && empty($fdpcpf)){
		return false;
	} else {
		if( $fdpsiape != '' ){
			$aryWhere[] = "s.nu_matricula_siape = '{$fdpsiape}'";
		} elseif( $fdpcpf != '' ){
			$aryWhere[] = "s.nu_cpf = '{$fdpcpf}'";
		}

		$sql = "SELECT		trim( replace( to_char( cast(s.nu_cpf as bigint), '000:000:000-00'), ':', '.' ) ) AS fdpcpf,
               				trim( s.no_servidor ) AS fdpnome,
							s.nu_matricula_siape AS afpnumsiape
       			FROM 		siape.tb_servidor_simec s
       						".(is_array($aryWhere) ? ' WHERE '.implode(' AND ', $aryWhere) : '')."
       			ORDER BY	1";

	     $aryDados = $db->pegaLinha($sql);

	     if(empty($aryDados)){
	     	return "";
	     } else {
	     	echo simec_json_encode($aryDados);
	     }
	}
}

function exibirNomeServidor($afpid){
	global $db;

	$sql = "SELECT 		trim(sim.no_servidor) AS fdpnome
  			FROM		cap.afastamento afa
  			INNER JOIN	siape.tb_servidor_simec sim ON sim.nu_cpf = afa.fdpcpf
  			WHERE		afpid = {$afpid}";

	return $db->pegaUm($sql);
}

    function verificarPreenchimentoRelatorio($afpid){
	global $db;

	$sql = "
            SELECT  afa.fdpcpf,
                    afa.afpcargofuncao,
                    afa.afptelefone,
                    --afa.afpdtrealizacaoinicial,
                    --afa.afpdtrealizacaofinal,
                    afa.afppertraninicial,
                    afa.afppertranfinal,
                    raf.rafatividadesfatos,
                    raf.rafconclusao,
                    raf.rafsugestao,
                    raf.rafobservacao,
                    array_to_string(array(SELECT DISTINCT te.prddsc FROM cap.afastamentotrecho at INNER JOIN cap.paisdiarias te ON at.idpdr= te.idpdr WHERE at.afpid = afa.afpid),',') AS paises,
                    array_to_string(array(SELECT DISTINCT cp.cdpdsc FROM cap.afastamentotrecho at INNER JOIN cap.cidadepais cp ON at.cdpid = cp.cdpid WHERE at.afpid = afa.afpid GROUP BY cp.cdpdsc, at.afpid),',') AS cidades
            FROM cap.afastamento afa

            LEFT JOIN cap.relatorioafastamento raf ON raf.afpid = afa.afpid

            WHERE afa.afpid = {$afpid}
        ";
	$aryDados = $db->carregar($sql);
        
	if(empty($aryDados)){
            return false;
	} else {
            foreach($aryDados as $dados){
                if(empty($dados['fdpcpf']) || empty($dados['afpcargofuncao']) || empty($dados['afptelefone']) || empty($dados['afppertraninicial']) || empty($dados['afppertranfinal']) ||
                   empty($dados['paises']) || empty($dados['cidades']) || empty($dados['rafatividadesfatos']) || empty($dados['rafconclusao']) || empty($dados['rafsugestao']) || empty($dados['rafobservacao'])){
                        return 'N';
                }
            }
            return 'S';
	}
    }

function criarDocumento($afpid) {
	global $db;

	if(empty($afpid)){
		return false;
	}

	$docid = pegarDocid($afpid);
	if(!$docid){
		$docdsc = "Cadastramento Afastamento do País";
		$docid = wf_cadastrarDocumento(WF_TPDID_CONTROLE_AFASTAMENTO, $docdsc );
		if($afpid) {

			$sql = "UPDATE 	cap.afastamento
					SET		docid = {$docid}
					WHERE 	afpid = {$afpid}";

			$db->executar($sql);
			$db->commit();
			return $docid;
		} else {
			return false;
		}
	} else {
		return $docid;
	}
}

function pegarDocid($afpid) {
	global $db;

	$afpid = (integer) $afpid;
	$sql = "SELECT docid FROM cap.afastamento WHERE afpid = {$afpid}";
	return (integer) $db->pegaUm($sql);
}

    function validarAfastamento($post){
	global $db;

	extract($post);
	$dataHora = date("d/m/Y H:i:s");

	$validado = verificarValidacaoAfastamento($afpid);

	if(empty($validado)){
            if($_SESSION['usunome'] && $_SESSION['usucpf']){
                $txt_validacao = "<b>Validado por ".$_SESSION['usunome']." - CPF: ".formatar_cpf($_SESSION['usucpf'])." em ".$dataHora."</b><br>";

		$sql = "
                    UPDATE cap.relatorioafastamento
                        SET raftxtvalidacao = '{$txt_validacao}'
                    WHERE rafid = {$rafid}
                ";
                $db->executar($sql);
                $db->commit();
            }
            return 'S';
	} else {
            return 'N';
	}
    }

    function verificarValidacaoAfastamento($afpid){
	global $db;

	$sql = "
            SELECT raftxtvalidacao
            FROM cap.relatorioafastamento
            WHERE afpid = {$afpid}
            ORDER BY rafid DESC
            LIMIT 1
        ";
	$aryDados = $db->pegaUm($sql);

	if($aryDados){
            return $aryDados;
	} else {
            return false;
	}
    }

    function exibirTrecho($afpid){
	global $db;

	//recupera o pais e periodo
	$sql_pais = "
            SELECT  idp.prddsc as pais,
                    to_char(aft.aftdtinicio,'DD/MM/YYYY') as afpdtinicio,
                    to_char(aft.aftdtfinal,'DD/MM/YYYY') as afpdtfinal

            FROM cap.afastamentotrecho aft
            JOIN cap.paisdiarias idp ON idp.idpdr = aft.idpdr

            WHERE aft.afpid = {$afpid}
            ORDER BY aft.aftdtinicio
        ";
	$result = $db->carregar($sql_pais);

	if($result){
            foreach($result as $rs){
                $trecho .= $rs["pais"] . ' - ' . $rs["afpdtinicio"] . ' à ' . $rs["afpdtfinal"]. '<br>';
            }
	}
	return $trecho;
    }

    function imprimirAfastamento($tipo,$validado = null){
	global $db;

	include_once APPRAIZ . "includes/classes/fileSimec.class.inc";
	include_once APPRAIZ ."includes/dompdf/dompdf_config.inc.php";

	$data           = dataExtenso(date("d/m/Y"));
	$servidor       = exibirDadosFormulario($_SESSION['cap']['afpid']);
	$textoValidacao = verificarValidacaoAfastamento($_SESSION['cap']['afpid']);
	$relatorio      = exibirDadosRelatorio($_SESSION['cap']['afpid']);
	$trecho         = exibirTrecho($_SESSION['cap']['afpid']);

	extract($servidor);
	extract($relatorio);

	header('Content-type: text/html; charset=ISO-8859-1');

	ob_clean();

	if($validado){
            $arqdescricao = "Relatório de Afastamento do País - ".$fdpnome." - ".$afpdtinclusao." - Validado";
            $arqnome = "relatorio_afastamento_".str_replace(" ","_",$fdpnome)."_".date('dmY_His')."_validado";
            $tpdid = 23;
	} else {
            $arqdescricao = "Relatório de Afastamento do País - ".$fdpnome." - ".$afpdtinclusao;
            $arqnome = "relatorio_afastamento_".str_replace(" ","_",$fdpnome)."_".date('dmY_His');
            $tpdid = 24;
	}

	$html .= '
            <html>
                <head>
                    <title>Relatório Afastamento do País</title>
                    <style>
                        .notprint { display: none }
                        .div_rolagem{display: none} }
                        .notscreen { display: none; }
                        .div_rolagem{ overflow-x: auto; overflow-y: auto; height: 50px;}
                        .bordaarredonda {
                            background:#FFFFFF;
                            color:#000; border: #000 1px solid;
                            padding: 10px;
                            -moz-border-radius:10px 10px;
                            -webkit-border-radius:10px 10px;
                            border-radius:10px 10px;
                            width:95%;
                            text-align:left;
                        }
                        .quebra{
                            page-break-after: always !important;
                            height: 0px;
                            clear: both;
                        }
                    </style>
                </head>
                <body>
                <table border="0" width="85%" cellspacing="1" cellpadding="5" border="0" align="center">
                    <tr>
                        <td>
                            <table width="95%" border="0" cellpadding="0" cellspacing="0" class="notscreen1 debug">
                                <tr bgcolor="#ffffff">
                                    <td valign="top" align="center">
                                        <img src="../imagens/brasao.gif" width="45" height="45" border="0">
                                        <br><b>MINISTÉRIO DA EDUCAÇÃO<br/>
                                        RELATÓRIO DE VIAGEM INTERNACIONAL</b> <br />
                                    </td>
                                </tr>
                            </table>
                        </td>
                    </tr>
                </table>
                <br>
                <table id="termo" width="95%" align="center" border="0" cellpadding="3" cellspacing="1">
                    <tr>
                        <td style="font-size: 12px; font-family:arial;">
                            <table align="center" border="0" cellspacing="1" cellpadding="3" width="85%">
                                <tr>
                                    <td bgcolor="#CCCCCC">&nbsp;<b>1. Órgão</b></td>
                                </tr>
                                <tr>
                                    <td align="center"><div class="bordaarredonda">&nbsp;' . $servidor['uamdsc'] . '</div></td>
                                </tr>
                                <tr>
                                    <td bgcolor="#CCCCCC">&nbsp;<b>2. Identificação do Servidor:</b></td>
                                </tr>
                                <tr>
                                    <td align="center">
                                        <div class="bordaarredonda">
                                            <table width="100%" border="0" align="center" cellspacing="1" cellpadding="3">
                                                <tr>
                                                    <td width="50%"><b>Nome:</b>&nbsp;&nbsp;' . $servidor['fdpnome'] . '</td>
                                                    <td width="50%"><b>Matrícula/SIAPE:</b>&nbsp;&nbsp;' . $servidor['afpnumsiape'] . '</td>
                                                </tr>
                                                <tr>
                                                    <td width="50%"><b>Cargo/Função:</b>&nbsp;&nbsp;' . $servidor['afpcargofuncao'] . '</td>
                                                    <td width="50%"><b>Ramal:</b>&nbsp;&nbsp;' . $servidor['afptelefone'] . '</td>
                                                </tr>
                                            </table>
                                        </div>
                                    </td>
                                </tr>
                                <tr>
                                    <td bgcolor="#CCCCCC">&nbsp;<b>3. Período de Afastamento:</b></td>
                                </tr>
                                <tr>
                                    <td align="center">
                                        <div class="bordaarredonda">
                                            <table width="100%" border="0" align="center" cellspacing="1" cellpadding="3">
                                                <tr>
                                                    <td width="50%"><b>Data de Saída:</b>&nbsp;&nbsp;' . formata_data($servidor['afppertraninicial']) . '</td>
                                                    <td width="50%"><b>Data de Chegada:</b>&nbsp;&nbsp;' . formata_data($servidor['afppertranfinal']) . '</td>
                                                </tr>
                                                <tr>
                                                    <td colspan="2"><b>Trecho:</b>&nbsp;&nbsp;' . $trecho . '</td>
                                                </tr>
                                            </table>
                                        </div>
                                    </td>
                                </tr>
                                <tr>
                                    <td bgcolor="#CCCCCC">&nbsp;<b>4. Atividades/Fatos Transcorridos:</b></td>
                                </tr>
                                <tr>
                                    <td align="center"><div class="bordaarredonda" style="height:150px;">' . $relatorio['rafatividadesfatos'] . '</div></td>
                                </tr>
                                <tr>
                                    <td bgcolor="#CCCCCC">&nbsp;<b>5. Conclusões Alcançadas:</b></td>
                                </tr>
                                <tr>
                                    <td align="center"><div class="bordaarredonda" style="height:150px;">' . $relatorio['rafconclusao'] . '</div></td>
                                </tr>
                            </table>
                        </td>
                    </tr>
                </table>

                <div class="quebra"></div>

                <table border="0" width="85%" cellspacing="1" cellpadding="5" border="0" align="center">
                    <tr>
                        <td>
                            <table width="95%" border="0" cellpadding="0" cellspacing="0" class="notscreen1 debug">
                                <tr bgcolor="#ffffff">
                                    <td valign="top" align="center">
                                        <img src="../imagens/brasao.gif" width="45" height="45" border="0">
                                        <br><b>MINISTÉRIO DA EDUCAÇÃO<br/>
                                        RELATÓRIO DE VIAGEM INTERNACIONAL</b> <br>
                                    </td>
                                </tr>
                            </table>
                        </td>
                    </tr>
                </table>
                <br>
                <table id="termo" width="95%" align="center" border="0" cellpadding="3" cellspacing="1">
                    <tr>
                        <td style="font-size: 12px; font-family:arial;">
                        <div>
                            <table align="center" border="0" cellspacing="1" cellpadding="3" width="85%">
                                <tr>
                                    <td bgcolor="#CCCCCC">&nbsp;<b>6. Sugestões em relação aos benefícios que podem ser auferidos para a área da Educação:</b></td>
                                </tr>
                                <tr>
                                    <td align="center"><div class="bordaarredonda" style="height:150px;">'.$relatorio['rafsugestao'].'</div></td>
                                </tr>
                                <tr>
                                    <td bgcolor="#CCCCCC">&nbsp;<b>7. Observações:</b></td>
                                </tr>
                                <tr>
                                    <td align="center"><div class="bordaarredonda" style="height:150px;">'.$relatorio['rafobservacao'].'</div></td>
                                </tr>
                                <tr>
                                    <td align="center">
                                        <div class="bordaarredonda">
                                            <p align="justify">
                                                Este relatório deverá ser encaminhado ao Setor de Afastamento do País GM/MEC, no prazo de 5 (cinco) dias úteis,
                                                contados do retorno (de acordo com o Parágrafo Único do Art. 5 da Portaria 2.016 de 07 de julho de 2004).
                                            </p>
                                        </div>
                                    </td>
                                </tr>
                                <tr>
                                    <td><p style="text-align: right;">'.$data.'&nbsp;</p></td>
                                </tr>
                                <tr>
                                    <td><p style="text-align: right;">&nbsp;</p></td>
                                </tr>
                                <tr>
                                    <td><p style="text-align: center;">______________________________________________</p></td>
                                </tr>
                                <tr>
                                    <td><p style="font-family: Arial, verdana; font-size: 11px; text-align: center;">'.$servidor['fdpnome'].'</p></td>
                                </tr>
                                <tr>
                                    <td><p style="text-align: right;">&nbsp;</p></td>
                                </tr>';
                                    if($validado){
                                        $html .= '
                                            <tr style="text-align: center;">
                                                <td><b>VALIDAÇÃO ELETRÔNICA DO DOCUMENTO<b><br><br>'.$textoValidacao.'</td>
                                            </tr>
                                        ';
                                    }
                    $html .='</table>
                        </div>
                        </td>
                    </tr>
                </table>
                <table id="termo" width="95%" align="center" border="0" cellpadding="3" cellspacing="1">
                    <tr>
                        <td class="SubtituloDireita, div_rolagem" style="text-align: center;">
                            <input type="button" name="impressao" id="impressao" value="Impressão" onclick="window.print();" />
                            <input type="button" name="fechar" id="fechar" value="Fechar" onclick="window.close();" />
                        </td>
                    </tr>
                </table>
                </body>
            </html>
        ';

	if(trim($tipo) == 'pdf'){
            ob_clean();
            $dompdf = new DOMPDF();
            $dompdf->load_html($html);
            $dompdf->set_paper('A4','portrait');
            $dompdf->render();

            if( isset($_SERVER['WINDIR']) && $_SERVER['WINDIR'] == "C:\windows" ){
                $caminhoArq = "C:/tmp/".$arqnome.".pdf";
            } else {
                $caminhoArq = "/tmp/".$arqnome.".pdf";
            }

            $aryCampos = array("afpid" => $afpid,"arastatus" => "'A'","aradtinclusao" => "now()","tpdid" => $tpdid);
            try {
                if ( empty($caminhoArq) || empty($aryCampos)){
                    throw new Exception("Faltam parametros no SAVE (" . get_class($this) . ")");
                }
                file_put_contents($caminhoArq, $dompdf->output());
                $obFile = new FilesSimec("arquivoafastamento", $aryCampos, "cap");
                $obFile->setMover($caminhoArq, "pdf");
            }catch(Exception $e){
    		echo $e->getMessage();
            }
	}else{
            print($html);
	}
    }

    function dataExtenso($data){
	$data = explode("/",$data);
	$dia = $data[0];
	$mes = $data[1];
	$ano = $data[2];

	switch( $mes ){
            case 1:
                $mes = "Janeiro";
                break;
            case 2:
                $mes = "Fevereiro";
                break;
            case 3:
                $mes = "Março";
                break;
            case 4:
                $mes = "Abril";
                break;
            case 5:
                $mes = "Maio";
                break;
            case 6:
                $mes = "Junho";
                break;
            case 7:
                $mes = "Julho";
                break;
            case 8:
                $mes = "Agosto";
                break;
            case 9:
                $mes = "Setembro";
                break;
            case 10:
                $mes = "Outubro";
                break;
            case 11:
                $mes = "Novembro";
                break;
            case 12:
                $mes = "Dezembro";
                break;
	}
	$dataExtenso = "Brasília/DF, $dia de $mes de $ano.";
	return $dataExtenso;
    }

    function verificarRelatorioPDFExiste( $afpid, $perfil ){
        global $db;

        if( in_array( CAP_PERFIL_GABINETE,$perfil ) ){
            $tpdid = 23;
        } else {
            $tpdid = 24;
        }

        $sql = "
            SELECT MAX(arqid)
            FROM cap.arquivoafastamento
            WHERE afpid = {$afpid} AND tpdid = {$tpdid}
        ";
        $aryDados = $db->pegaUm($sql);

        if($aryDados){
            return $aryDados;
        } else {
            return '';
        }
    }

function gerarNumeroProcesso($afpid){
	global $db;

	$dia = date("d");
	$mes = date("m");
	$ano = date("Y");

	$numprocesso = $ano.$mes.$dia.$afpid;

	$sql = "UPDATE 	cap.afastamento
			SET 	afpnumprocesso = {$numprocesso}
			WHERE	afpid = {$afpid}";

	$db->executar($sql);
	$db->commit();
}

function pegarEstadoDocumento($docid){
	global $db;
	if($docid) {
		$sql = "SELECT		ed.esdid
				FROM		workflow.documento d
				INNER JOIN	workflow.estadodocumento ed ON ed.esdid = d.esdid
				WHERE		d.docid = {$docid}";
		$estado = $db->pegaUm( $sql );
		return $estado;
	} else {
		return false;
	}
}

    function verificarPeriodoEvento($post){
	global $db;

	extract($post);

	$aftdtinicio = formata_data_sql($aftdtinicio);
	$aftdtfinal = formata_data_sql($aftdtfinal);

	$sql = "
            SELECT  COUNT(afpid)
            FROM cap.afastamento
            WHERE afpid = {$afpid}
            AND ( ('{$aftdtinicio}' BETWEEN afppertraninicial AND afppertranfinal) AND ( '{$aftdtinicio}' <> afppertranfinal ) )
            OR ( ('{$aftdtfinal}' BETWEEN afppertraninicial AND afppertranfinal) AND ( '{$aftdtfinal}' <> afppertraninicial ) )
        ";
	$periodo = $db->pegaUm($sql);

	if($periodo > 0 ){
            $sql = "
                SELECT  COUNT(aftid)
                FROM cap.afastamentotrecho
                WHERE afpid = {$afpid}
                AND ( ('{$aftdtinicio}' = aftdtinicio OR aftdtfinal =  '{$aftdtfinal}')
                OR ( ( ('{$aftdtinicio}' BETWEEN aftdtinicio AND aftdtfinal) AND ( '{$aftdtinicio}' <> aftdtfinal ) ) OR ( ('{$aftdtfinal}' BETWEEN aftdtinicio AND aftdtfinal) AND ( '{$aftdtfinal}' <> aftdtinicio ) ) ) )
            ";
            $qtd = $db->pegaUm($sql);

            if($qtd == 0){
                return "S";
            } else {
                return "N";
            }
	} else {
            return "N";
	}
}

function salvarDataViagem($post){
	global $db;

	extract($post);

	$afppertraninicial = formata_data_sql($afppertraninicial);
    $afppertranfinal = formata_data_sql($afppertranfinal);
	$afppertraninicial = $afppertraninicial ? "'{$afppertraninicial}'" : "null";
	$afppertranfinal = $afppertranfinal ? "'{$afppertranfinal}'" : "null";

	$sql = "UPDATE 	cap.afastamento
			SET	 	afppertraninicial = {$afppertraninicial },
				 	afppertranfinal  = {$afppertranfinal}
			WHERE	afpid = {$afpid}";

	$db->executar($sql);

    if($db->commit()){
    	echo "S";
    } else {
    	echo "N";
    }
}

function wfEnviaSolicitacaoViagem(){
	global $db;
	$mensagemRetorno = false;

	if(!$_SESSION['cap']['afpid']) return false;


	// valida se é afpidorigem
	$sql = "SELECT afpidorigem
			FROM cap.afastamento
			WHERE afpid = {$_SESSION['cap']['afpid']} AND afpstatus = 'A'";
	$afpidorigem = $db->pegaUm( $sql );


	// valida evento
	$sql = "SELECT count(afpid) as total
			FROM cap.afastamentotrecho
			WHERE afpid = {$_SESSION['cap']['afpid']}";
	$total = $db->pegaUm($sql);

	if( $total == 0 )
		$mensagemRetorno = "<li>É necessário inserir pelo menos um evento na aba Formulário de Autorização</li>";

	// valida documentos na aba documentos - pelo menos 1 é necessário
	$sql = "SELECT COUNT(afpid) AS total
			FROM cap.arquivoafastamento
			WHERE afpid = {$_SESSION['cap']['afpid']} AND arastatus = 'A' AND tpdid in (4,5) ";
	$totalDocumentos = $db->pegaUm( $sql );

	if( $totalDocumentos == 0 && !$afpidorigem){
		// if( $mensagemRetorno !== false )
		// 	$mensagemRetorno .= " e ";
		$mensagemRetorno .= "<li>É necessário inserir o Memorando ou o Ofício na aba Documentos</li>";
	}

	if( $mensagemRetorno !== false ){
		return '<ul style=list-style:disc;>'.$mensagemRetorno.'</ul>';
		return false;
	}

	return true;
}

function wfAutorizaSolicitacaoViagem(){
	global $db;

	if(!$_SESSION['cap']['afpid']) return false;

	$sql = "SELECT 		raftxtvalidacao
			FROM 		cap.relatorioafastamento
			WHERE 		raftxtvalidacao IS NOT NULL
			AND 		afpid = {$_SESSION['cap']['afpid']}";
	$raftxtvalidacao = $db->pegaUm($sql);

	if(!$raftxtvalidacao){
		return "É necessário preencher todos os campos e validar o formulário da aba Relatório Viagem";
		return false;
	}

	return true;
}


function wfLiberarViagem(){
	global $db;

	if(!$_SESSION['cap']['afpid']) return false;

	$sql = "SELECT 		raftxtvalidacao
			FROM 		cap.relatorioafastamento
			WHERE 		raftxtvalidacao IS NOT NULL
			AND 		afpid = {$_SESSION['cap']['afpid']}";
	$raftxtvalidacao = $db->pegaUm($sql);

	if($raftxtvalidacao){
		return "A viagem já foi liberada. Não é possível liberar novamente!";
		return false;
	}

	return true;
}

function wfVerificaTermo(){
	global $db;

	if(!$_SESSION['cap']['afpid']) return false;

	$sql = "SELECT 		tvgid
			FROM 		cap.termoviagem
			WHERE 		afpid = {$_SESSION['cap']['afpid']}";
	$tvgid = $db->pegaUm($sql);

	if(!$tvgid){
		return "É necessário a existência do termo";
		return false;
	}

	return true;
}

function wfVerificaTermoInsercao(){
	global $db;

	if(!$_SESSION['cap']['afpid']) return false;

	// valida se não é afpidorigem
	$sql = "SELECT afpidorigem
			FROM cap.afastamento
			WHERE afpid = {$_SESSION['cap']['afpid']} AND afpstatus = 'A'";
	$afpidorigem = $db->pegaUm( $sql );

	if($afpidorigem){
		return "Esta Ação é somente para a primeira autorização do termo";
		return false;
	}

	return true;
}

function wfVerificaTermoAlteracao(){
	global $db;

	if(!$_SESSION['cap']['afpid']) return false;

	// valida se é afpidorigem
	$sql = "SELECT afpidorigem
			FROM cap.afastamento
			WHERE afpid = {$_SESSION['cap']['afpid']} AND afpstatus = 'A'";
	$afpidorigem = $db->pegaUm( $sql );

	if(!$afpidorigem){
		return "Esta Ação é somente para alteração do termo";
		return false;
	}

	return true;
}

function wfCriaTermoCancela(){
	wfCriaTermo('C');
}

//function chamada estado Autorizar Termo de Viagem - workflow
function wfCriaTermo($status = null){
	global $db;

	if(!$_SESSION['cap']['afpid']) return false;

	$dia = date('d');
	$mes = strtolower(retornaMesPorExtenso(date('m')));
	$ano = date('Y');

	//pega dados do solicitante
	$sql = "
        SELECT
            s.no_servidor as nome,
            afpcargofuncao as cargo,
            uamdsc as areamec,
            to_char(afppertraninicial::TIMESTAMP,'DD/MM/YYYY') as dataini,
            to_char(afppertranfinal::TIMESTAMP,'DD/MM/YYYY') as datafim,
            afttxtobjetivoviagem as objetivo,
            afpnumprocesso as nuprocesso,
            afporgaofinanciador as orgaofinanciador,
            tonid as tiponatureza,
            afpsitdiarias,
            afpsitpassagem
        FROM cap.afastamento a
        INNER JOIN siape.tb_servidor_simec s on s.nu_cpf = a.fdpcpf
        INNER JOIN public.unidadeareamec u on u.uamid = a.uamid
        WHERE afpid = {$_SESSION['cap']['afpid']}
    ";
	$dados = $db->pegaLinha($sql);

	if($dados) extract($dados);
    $tiponaturezasaida = '';
	if($tiponatureza == '1'){
        $tiponaturezasaida = "Com Ônus";
        if($afpsitdiarias == true){
            $tiponaturezasaida .= " - Diárias";
        }
        if($afpsitpassagem == true){
            $tiponaturezasaida .= " - Passagem Aérea";
        }
	}else if($tiponatureza == '2'){
        $tiponaturezasaida = "Com Ônus Limitado";
	}else if($tiponatureza == '3'){
        $tiponaturezasaida = "Sem Ônus";
	}else if($tiponatureza == '4'){
        $tiponaturezasaida = "Com Ônus (50% da(s) diária(s))";
        if($afpsitdiarias == true){
            $tiponaturezasaida .= " - Diárias";
        }
        if($afpsitpassagem == true){
            $tiponaturezasaida .= " - Passagem Aérea";
        }
    }


	//recupera o pais e objetivos
	$sql_pais = "
        SELECT
            pai.prddsc,
            aft.aftobjetivo,
            cidade.cdpdsc
        FROM cap.afastamentotrecho aft
        INNER JOIN cap.paisdiarias pai ON pai.idpdr = aft.idpdr
        INNER JOIN cap.cidadepais cidade ON cidade.cdpid = aft.cdpid
        WHERE aft.afpid = {$_SESSION['cap']['afpid']}
        ORDER BY aft.idpdr, aft.aftdtinicio
        ";
	$result = $db->carregar($sql_pais);

	if($result){
        foreach($result as $rs){
            $tr_objetivo_pais .= '
                <tr>
                    <td align="left">'.$rs["aftobjetivo"].'</td>
                    <td align="center">'.$rs["prddsc"].'</td>
                    <td align="center">'.$rs["cdpdsc"].'</td>
                </tr>
            ';
            //$objetivosTexto = $rs["aftobjetivo"] .' - País: '.$rs["paidescricao"].',';
        }
	}

	//recupera o ultimo registro do diario oficial da uniao
	$sql = "
        select
            to_char(pbldtdiario::TIMESTAMP,'DD/MM/YYYY') as datapub,
            pblnumsecao as secao,
            pblpagina as pagina
        from cap.publicacao
        where afpid = {$_SESSION['cap']['afpid']}
        order by pblid DESC limit 1
    ";
	$diario = $db->pegaLinha($sql);

	if($diario){
        $diarioOficialUniao = "Publicada no Diário Oficial da União de ".$diario['datapub'].", Seção ".$diario['secao'].", Página ".$diario['pagina'].".";
	}

	//verifica se é 1=autorizado / 2=alterado / 3=torna sem efeito
	$sql = "
        select count(tvgid) as total
        from cap.termoviagem
        where tvgtipo = 1
        and afpid = {$_SESSION['cap']['afpid']}
    ";
	$total = $db->pegaUm($sql);

	if($total > 0){
            $autoriza_altera_cancela = "altera";
            $tvgtipo = 2;
	}else{
        //verifica se é filha
        $sql = "
            select afpidorigem
            from cap.afastamento
            where afpstatus = 'A' and afpid = {$_SESSION['cap']['afpid']}
        ";
        $afpidorigem = $db->pegaUm($sql);

        if(!$afpidorigem){
            $autoriza_altera_cancela = "autoriza";
            $tvgtipo = 1;
            unset($diarioOficialUniao);
        }else{
            $autoriza_altera_cancela = "altera";
            $tvgtipo = 2;
        }
	}

	if($status == 'C') {
        $autoriza_altera_cancela = "torna sem efeito";
        $tvgtipo = 3;
	}

	//recupera assinante do termo (modal workflow Enviar para análise do GM)
	$sqlAss = "SELECT * FROM cap.assinattermoviagem WHERE afpid = ".$_SESSION['cap']['afpid'];
	$assinanteTermo = $db->carregar($sqlAss);
    //fim recupera assinante Termo

    #TRATAMENTO SOLICITADO POR LUIZ SIQUEIRA(SELECIONAR QUEM VAI ASSINAR O TERMO DE VIAGEM)

    #MINISTRO TITULAR
	if( $assinanteTermo[0]['atvministrotitular'] == 't'){
        $interino = "";
        $assinatura = '<img width="232" height="104" src="http://simec.mec.gov.br/imagens/assinaturas_dirigentes/assinatura_ministro.png"><br>';
    }

    #MINISTRO INTERINO
	if( $assinanteTermo[0]['atvministrointerino'] == 't'){
        $interino = " Interino,";
        $assinatura = '<img width="232" height="104" src="http://simec.mec.gov.br/imagens/assinaturas_dirigentes/assinatura_ministro_interino.png"><br>';
    }

    #DEFININDO TEXTO DE ÍNICIO DO TERMO, DE ACORDO COM A ASSINATURA ESCOLHIDA NA PRÉ CONDICAO
    $iniciotrm = 'O MINISTRO DE ESTADO DA EDUCAÇÃO,'.$interino.' no uso da competência que lhe foi delegada pelo Decreto no 1.387, de 7 de fevereiro de 1995, com redação dada pelo Decreto nº 2.349, de 15 de outubro de 1997 e nº 3.025, de 12 de abril de 1999, <b>'.$autoriza_altera_cancela.'</b> o afastamento do País do(s) seguinte(s) servidor(es):';

    if( $assinanteTermo[0]['atvsecretarioexecutivo'] == 't'){
        $assinatura = '<img width="232" height="104" src="http://simec.mec.gov.br/imagens/assinaturas_dirigentes/assinatura_secretario.png"><br>';
        $iniciotrm = 'O SECRETÁRIO-EXECUTIVO DO MINISTÉRIO DA EDUCAÇÃO, no uso da competência que lhe foi subdelegada pela Portaria nº 373, de 22 de abril de 2015, <b>'.$autoriza_altera_cancela.'</b> o afastamento do País do(s) seguinte(s) servidor(es):';
    }

    $html = '
            <style>
                .cabecalho{text-align:center;font-weight:bold;font-size:18px;}
                table.tbl {border-collapse: collapse;}
                table.tbl tr td {border:1px solid black;}
                table.tbl2 {border-top:0px;}
                .texto{text-align:left;}
                table.tbl_not {border:0px;}
                *{font-family:Arial, Helvetica, sans-serif;font-size:12px;}
                .bold{font-weight:bold}
                @media print {.notprint { display: none }}
                @media screen {.notscreen { display: none }}
            </style>

            <div class="notprint" style="text-align:right;width:100%" >
                <a href="javascript:print()">Imprimir</a>
            </div>
            <div class="cabecalho" >
                <img width="100px" height="100px" src="http://simec.mec.gov.br/imagens/brasao.gif" /><br />
                MINISTÉRIO DA EDUCAÇÃO <br><br>
                Controle de Afastamento do País.<br>
            </div>
            <table width="100%" class="tbl tbl2" cellSpacing="0" cellPadding="3" align="center" >
                <tr>
                    <td>
                        <div class="texto">
                            <br> '.$iniciotrm.' <br><br>

                            <table width="95%" style="border-bottom:0px" class="tbl" cellSpacing="0" cellPadding="3" align="center" >
                                <tr>
                                    <td class="bold" width="20%" >Nome do Servidor:</td>
                                    <td colspan="3" >'.$nome.'</td>
                                </tr>
                                <tr>
                                    <td class="bold" >Cargo/Função:</td>
                                    <td colspan="3" >'.$cargo.'</td>
                                </tr>
                                <tr>
                                    <td class="bold" >Área MEC:</td>
                                    <td colspan="3" >'.$areamec.'</td>
                                </tr>
                                <tr>
                                    <td class="bold" >Período Total:</td>
                                    <td colspan="3" >'.$dataini.' à '.$datafim.'</td>
                                </tr>
                            </table>

                            <br><br>

                            Trânsito incluso, com a(s) seguinte(s) finalidade(s):

                            <br><br>

                            <table width="95%" class="tbl tbl2" cellSpacing="0" cellPadding="3" align="center" >
                                <tr>
                                    <td align="center" ><b>Objetivo</b></td>
                                    <td align="center" ><b>País</b></td>
                                    <td align="center" ><b>Cidade</b></td>
                                </tr>
                                   '.$tr_objetivo_pais.'
                            </table>

                            <br><br>

                            '.$diarioOficialUniao.'

                            <br><br><br>

                            Tipo de natureza: '.$tiponaturezasaida.'
                            <br>
                            Órgão financiador: '.$orgaofinanciador.'
                            <br>
                            Número do processo: '.$nuprocesso.'
                        </div>
                        <br><br>

                        <center>
                            Brasília, '.$dia.' de '.$mes.' de '.$ano.'.
                            <br><br>
                            '.$assinatura.'
                        </center>
                    </td>
                </tr>
            </table>
        ';

	#INSERE NA CAP.TERMOVIAGEM
	$sql = "
            INSERT INTO cap.termoviagem(afpid, tvgtipo, tvgdsc, tvgstatus, tvgdtinclusao )VALUES({$_SESSION['cap']['afpid']}, {$tvgtipo}, '{$html}', 'A', '".date('Y-m-d H:i:s')."')
        ";
	$db->carregar($sql);
	$db->commit();

	return true;
    }

    function wfAlteraViagem(){
	global $db;

	if( !$_SESSION['cap']['afpid'] ){
            return false;
        }

	#CRIA O DOCUMENTO PARA O CADASTRAMENTO DO NOVO FORMULÁRIO
	$docdsc = "Cadastramento Afastamento do País";
	$docid = wf_cadastrarDocumento(WF_TPDID_CONTROLE_AFASTAMENTO, $docdsc, WF_CADASTRO_NOVO_FORMULARIO );

	#INSERE AFASTAMENTO - AFPIDORIGEM E DOCID NULL
	$sql = "
            INSERT INTO cap.afastamento(
                    tonid, afpcargofuncao, afpdtrealizacaoinicial, afpdtrealizacaofinal, afppertraninicial, afppertranfinal, afporgaofinanciador, afpvlrpassagem,
                    afpclasse, afptrecho, afpnumdiarias, afpvlrtotaldiarias, afpvlrtotalbolsa, afpinfbolsamensal, afpinfbolsaauxalim, afpinfbolsasegurosaude,
                    afpinfbolsatxescolares, afttxtobjetivoviagem, afpvincservico, afprelevancia, afppertinencia,afpjustifespecial, afpparecerassessoria, afpstatus,
                    afpdtinclusao, afpusucpf, fdpcpf, afpnumsiape, uamid, afptelefone, docid, afpnumprocesso, afpsitpassagem, afpsitdiarias, afpidorigem,afpemailservidor
            )
            (
                SELECT  tonid, afpcargofuncao, afpdtrealizacaoinicial, afpdtrealizacaofinal, afppertraninicial, afppertranfinal, afporgaofinanciador, afpvlrpassagem,
                        afpclasse, afptrecho, afpnumdiarias, afpvlrtotaldiarias, afpvlrtotalbolsa, afpinfbolsamensal, afpinfbolsaauxalim, afpinfbolsasegurosaude,
                        afpinfbolsatxescolares, afttxtobjetivoviagem, afpvincservico, afprelevancia, afppertinencia, afpjustifespecial, afpparecerassessoria, afpstatus,
                        afpdtinclusao, afpusucpf, fdpcpf, afpnumsiape, uamid, afptelefone, {$docid}, afpnumprocesso,
                        afpsitpassagem, afpsitdiarias, {$_SESSION['cap']['afpid']}, afpemailservidor

                FROM cap.afastamento

                WHERE afpid = {$_SESSION['cap']['afpid']}
            ) returning afpid
        ";
	$afpid = $db->pegaUm($sql);

	//insere trechos de viajens
	$sql = "
            INSERT INTO cap.afastamentotrecho(
                afpid, paiid, cdpid, aftdtinicio, aftdtfinal, aftobjetivo
            )
            (
                SELECT {$afpid}, paiid, cdpid, aftdtinicio, aftdtfinal, aftobjetivo
                FROM cap.afastamentotrecho
                WHERE afpid = {$_SESSION['cap']['afpid']}
            )
        ";
	$db->executar($sql);
	$db->commit();

	$_SESSION['cap']['afpid'] = $afpid;

	return true;
    }

function cabecalhoCAP(){
	global $db;

	$afpid = $_GET['afpid'];

	if ($afpid){
		unset($_SESSION['cap']['afpid']);
		$caminho = $_SERVER['REQUEST_URI'];
		$caminho = explode("&afpid=",$caminho);
		$_SESSION['cap']['afpid'] = $_GET['afpid'];
		echo "<script>window.location.href=\"{$caminho[0]}\";</script>";
		exit;
	}
	else{
		$afpid = $_SESSION['cap']['afpid'];
	}

	if (!$afpid){
		echo "<script>window.location.href=\"cap.php?modulo=principal/listaviagem&acao=A\";</script>";
		exit;
	}

	$Pai = afpPai($afpid);
	$filhos = afpFilho($Pai,0,1);
	$sql = "select count(afpid) from cap.afastamento where afpidorigem = $Pai";
	$filho = $db->pegaUm($sql);
	($filho)? $vis="" : $vis = "none";

	$cab = "<div style=\"display:$vis\">
			 <table align=\"center\" class=\"Tabela\" >
			 <tbody>
			 	<tr bgcolor=\"#CCCCCC\">
					<td><b>Navegue:</b></td>
				</tr>
			 	<tr>
			 		<td colspan=2>$filhos</td>
			 	</tr>
			 </tbody>
			 </table>
			</div>";

	if(!$vis){
		echo $cab;
	}

}


/* Função que retorna o pai de todos os níveis de registro (cap.afastamento) corrente .*/
function afpPai ($afpid){
	global $db;
	$sql="select afpidorigem from cap.afastamento where afpid = $afpid";
	$pai = $db->pegaUm($sql);
	if($pai){
		$pai2 = afpPai($pai);
		return $pai2;
	}
	else{
		return $afpid;
	}
}

/* Função que monta/retorna os filhos de registro (cap.afastamento).*/
function afpFilho ($afpid,$width,$profundidade = null){
	global $db;

	if($profundidade == 1){
		$sql = "select afpid, afpnumprocesso from cap.afastamento where afpid = $afpid order by afpid desc";
		$dadosDemanda = $db->carregar($sql);
		$caminho = $_SERVER['REQUEST_URI'];
		$caminho = explode("&afpid=",$caminho);
		$caminho = $caminho[0]."&afpid={$dadosDemanda[0]['afpid']}";
		($_SESSION['cap']['afpid'] == $dadosDemanda[0]['afpid'])? $cor = "font-weight:bold" : $cor="";
		$tr_filhos .= "<div style=\"text-align: left;background: rgb(238, 238, 238);$cor\" ><a href=\"$caminho\"> Cód. # {$dadosDemanda[0]['afpid']} -  {$dadosDemanda[0]['afpnumprocesso']}</a></div>";
	}

	($profundidade)? $profundidade++ : $profundidade = $profundidade;
	$sql="select afpid, afpnumprocesso from cap.afastamento where afpidorigem = $afpid order by afpid desc";
	$filhos = $db->carregar($sql);
	if($filhos){
		$nivel = 1;
		foreach($filhos AS $fl){
			$arvore = "1.$x$nivel";
			$caminho = $_SERVER['REQUEST_URI'];
			$caminho = explode("&afpid=",$caminho);
			$caminho = $caminho[0]."&afpid={$fl['afpid']}";
			($_SESSION['cap']['afpid'] == $fl['afpid'])? $cor = "font-weight:bold" : $cor="";
			$tr_filhos .= ("
						<div style=\"text-align: left;background: rgb(238, 238, 238);padding-left:".(($nivel == 1)? $width=$width+15 : $width=$width)."px;$cor\" ><img src='../imagens/seta_filho.gif' ><a href=\"$caminho\" > Cód. # {$fl['afpid']} - {$fl['afpnumprocesso']}</a></div>
						");
			$filho = afpFilho($fl['afpid'],$width,$profundidade);
			($filho)? $profundidade = $profundidade : "" ;
			$tr_filhos .= $filho;
			$nivel++;
		}
	}
	return $tr_filhos;
}



function retornaMesPorExtenso($num)
{
	switch($num) {
		case "01": $mes = "Janeiro";   break;
		case "02": $mes = "Fevereiro"; break;
		case "03": $mes = "Março";     break;
		case "04": $mes = "Abril";     break;
		case "05": $mes = "Maio";      break;
		case "06": $mes = "Junho";     break;
		case "07": $mes = "Julho";     break;
		case "08": $mes = "Agosto";    break;
		case "09": $mes = "Setembro";  break;
		case "10": $mes = "Outubro";   break;
		case "11": $mes = "Novembro";  break;
		case "12": $mes = "Dezembro";  break;
	}
	return $mes;
}


/*
 * Funções de pré-ação de worklfow para envio para diligência e envio para reformulação
 * */

function form_assinarTermoViagem(){
	extract( $_POST );
?>
	<input type="hidden" name="afpid" value="<?=$afpid ?>"/>
	<table align="center" border="0" width="95%" class="tabela" cellpadding="3" cellspacing="2">
            <tr>
                <td class="SubTituloDireita" colspan="2"><center><b>Selecione quem vai assinar o termo de viagem:</b></center></td>
            </tr>
            <tr>
                <td>
                    <input type="radio" id="assinatura1" name="assinatura" value="1"> <b>Ministro da Educação </b> <br>
                    <input type="radio" id="assinatura2" name="assinatura" value="2"> <b>Ministro Interino </b> <br>
                    <input type="radio" id="assinatura3" name="assinatura" value="3"> <b>Secretário Executivo </b> <br>
                </td>
            </tr>
	</table>
        <script>
            jQuery(document).ready(function(){
                //jQuery('[name="assinatura"]').validate();
                if(jQuery('input[name=assinatura]').is(':checked') == false){
                    alert("Selecione quem vai assinar o termo de viagem.");
                    return false;
                }
            });
	</script>
<?PHP
}

function assinarTermoViagem(){
	global $db;

	extract($_POST);

    $atvministrotitular = 'FALSE';
    $atvministrointerino = 'FALSE';
    $atvsecretarioexecutivo = 'FALSE';

	if( $afpid && $assinatura ){
        $atvministrotitular = $assinatura == '1' ? 'TRUE' : 'FALSE';
        $atvministrointerino = $assinatura == '2' ? 'TRUE' : 'FALSE';
        $atvsecretarioexecutivo = $assinatura == '3' ? 'TRUE' : 'FALSE';
        
        //atribuindo o tipo de portaria para a sessao, para tratar na funcao que gera termo wfCriaTermo()
        if($assinatura == '3'){ $_SESSION['portAssSecExecutivo'] = 601; }
        if($assinatura == '4'){ $_SESSION['portAssSecExecutivo'] = 754; }

        $existeAfpid = $db->carregar("SELECT afpid FROM cap.assinattermoviagem WHERE afpid = {$afpid}");
        if($existeAfpid == FALSE ){
            $sql = "
                INSERT INTO cap.assinattermoviagem(
                    afpid,
                    atvtipo,
                    atvministrotitular,
                    atvministrointerino,
                    atvsecretarioexecutivo,
                    tvgstatus,
                    tvgdtinclusao
                )VALUES(
                    {$afpid},
                    NULL,
                    {$atvministrotitular},
                    {$atvministrointerino},
                    {$atvsecretarioexecutivo},
                    'A',
                    now()
                );
            ";
        }else{
            $sql = "
                UPDATE cap.assinattermoviagem
                    SET atvtipo                 = NULL,
                        atvministrotitular      = {$atvministrotitular},
                        atvministrointerino     = {$atvministrointerino},
                        atvsecretarioexecutivo  = {$atvsecretarioexecutivo},
                        tvgstatus               = 'A',
                        tvgdtinclusao           = now()
                WHERE afpid = {$afpid}
            ";
        }
        $db->executar( $sql );
        $res = $db->commit();

        $retorno = Array('boo' => $res, 'msg' => '');
	}else{
        $retorno = Array('boo' => false, 'msg' => 'ERRO!');
	}
	$retorno = simec_json_encode($retorno);
	echo $retorno;
}

/*
 * Fim fuções pré-ação
 * */

//Condição workflow Cancelar Viagem
function verificaCancelado($docid){

    global $db;

    if($docid){
        $sql = "select aedid from workflow.historicodocumento  where docid = ".$docid." AND aedid = 2564 order by htddata desc limit 1";
        $bloq = $db->pegaUm($sql);

        if( $bloq ){
            return false;
        } else {
            return true;
        }
    } else {
        return true;
    }
}

    function salvarDadosPais($dados) {
        global $db;

        if (!$dados['idpdr']) {
            $sql = "
                INSERT INTO cap.paisdiarias(
                        prddsc, pdrdas6, pdrdas5, pdrdas43, pdrdas21nivsup, pdrdasnivmedio
                    ) VALUES (
                    '{$dados['prddsc']}', '{$dados['pdrdas6']}', '{$dados['pdrdas5']}', '{$dados['pdrdas43']}', '{$dados['pdrdas21nivsup']}', '{$dados['pdrdasnivmedio']}'
                );
            ";
        } else {
            $sql = "
                UPDATE cap.paisdiarias
                    SET prddsc          = '{$dados['prddsc']}',
                        pdrdas6         = '{$dados['pdrdas6']}',
                        pdrdas5         = '{$dados['pdrdas5']}',
                        pdrdas43        = '{$dados['pdrdas43']}',
                        pdrdas21nivsup  = '{$dados['pdrdas21nivsup']}',
                        pdrdasnivmedio  = '{$dados['pdrdasnivmedio']}'
                WHERE idpdr = {$dados['idpdr']};
            ";
        }
        $db->executar($sql);
        $db->commit();
?>
        <script>
            alert('Operação realizada com sucesso');
            //location.href = "<?= $_SESSION['sisdiretorio'] ?>.php?modulo=ajuda/ajuda&acao=C&mnuid=<?= $mnuid ?>";
         </script>
<?PHP
    }

function listagemPais($params = false) {
                     global $db;

                     if($params){
                         $where = "where prddsc ~* '{$params['paisPesquisa']}'";
                     }
                     $cabecalho = array('Ação', 'País', 'DAS 6', 'DAS 5', 'DAS 4 e 3', 'DAS 2 e 1 / Nível Superior', 'Nível Médio');
                     $acao = "'<a href=\"cap.php?modulo=principal/paises_diarias&acao=A&idpdr='|| idpdr ||'\" ><img src=\"../imagens/alterar.gif\" border=\"0\"></a>&nbsp;
	<img border=\"0\" src=\"../imagens/excluir.gif\" id=\"'|| idpdr ||'\" onclick=\"excluirPais('|| idpdr ||');\" style=\"cursor:pointer;\"/>' AS acao,";

                     $sql = "SELECT 		$acao  prddsc, pdrdas6, pdrdas5, pdrdas43, pdrdas21nivsup, pdrdasnivmedio
                    FROM 		cap.paisdiarias pdi {$where}";
                     $db->monta_lista($sql, $cabecalho, '10', '10', '', '', '', '');
                 }

                 function excluirPais($dados) {
                     global $db;
                     $sql = "DELETE FROM cap.paisdiarias
 WHERE idpdr = '{$dados['idpdr']}';
";

                     $db->executar($sql);
                     $db->commit();
                     ?>
                    <script>
                              alert('Operação realizada com sucesso');
                     </script>
    <?php
}
    #não usado mais. não é feito o cauculo mais.
//    function atualizarDiaria($afpid) {
//        global $db;
//
//        $sql_pais = "
//            SELECT  aft.idpdr,
//                    pai.prddsc
//            FROM cap.afastamentotrecho aft
//            INNER JOIN cap.paisdiarias pai ON pai.idpdr = aft.idpdr
//            WHERE aft.afpid = {$afpid}
//
//            GROUP BY aft.idpdr, pai.prddsc
//            ORDER BY pai.prddsc
//        ";
//        $result = $db->carregar($sql_pais);
//
//        if ($result) {
//            $sql = "
//                SELECT  COUNT( DATE(aft.aftdtfinal)- DATE(aft.aftdtinicio) ) + 1
//                FROM cap.afastamentotrecho aft
//                WHERE aft.afpid = {$afpid}
//            ";
//            $diaria = ( $db->pegaUm($sql) );
//
//            if($diaria < 0){
//                $diaria = 0;
//            }
//            return $diaria;
//        }
//    }

function atualizarValorDiaria($afpid, $afpfuncao, $afpnivelcargo) {
    global $db;

    switch ($afpfuncao) {
        case 6:
            $campoFuncao = "pdrdas6";
            break;
        case 5:
            $campoFuncao = "pdrdas5";
            break;
        case 4:
            $campoFuncao = "pdrdas43";
            break;
        case 2:
            $campoFuncao = "pdrdas21nivsup";
            break;
        case 'N':
            $campoFuncao = "pdrdasnivmedio";
            break;
    }

      $sql = "SELECT 		{$campoFuncao}
					 FROM 			cap.afastamentotrecho aft
					 INNER JOIN		cap.paisdiarias pai ON pai.idpdr = aft.idpdr
					 WHERE 			aft.afpid = {$afpid}
					 GROUP BY 		{$campoFuncao}";


        $valordiaria = $db->carregar($sql);
        if($valordiaria){
        foreach ($valordiaria as $valor){
            $total = $total + $valor[$campoFuncao];
        }
        }
    return $total;
}

function atualizarValorTotal($afpid, $afpfuncao, $afpnivelcargo) {
    global $db;

    switch ($afpfuncao) {
        case 6:
            $campoFuncao = "pdrdas6";
            break;
        case 5:
            $campoFuncao = "pdrdas5";
            break;
        case 4:
            $campoFuncao = "pdrdas43";
            break;
        case 2:
            $campoFuncao = "pdrdas21nivsup";
            break;
        case 'N':
            $campoFuncao = "pdrdasnivmedio";
            break;
    }

      $sql = "SELECT  {$campoFuncao} ,sum(DATE(aft.aftdtfinal)-DATE(aft.aftdtinicio)) FROM cap.afastamentotrecho aft
 INNER JOIN		cap.paisdiarias pai ON pai.idpdr = aft.idpdr
						WHERE 		aft.afpid = {$afpid}
						group by   aft.idpdr,{$campoFuncao},aftdtinicio order by aftdtinicio;";

    $valortotal = $db->carregar($sql);
    $qnt = count($valortotal);
    $i = 1;
    if($valortotal){
    foreach ($valortotal as $valor) {
        if ($i == 1) {
            $total = $valor[$campoFuncao] / 2;
            $total = $total + $valor[$campoFuncao] * ($valor['sum']-1);
        } elseif ($i == $qnt) {
            $total = $total + ($valor[$campoFuncao] / 2);
            $total = $total + $valor[$campoFuncao] * ($valor['sum']-1);
        } else {
             $total = $total + $valor[$campoFuncao] * $valor['sum'];
        }
        $i++;
    }
    }
    return $total;
}

    ?>
