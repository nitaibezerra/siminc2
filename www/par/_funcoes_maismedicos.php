<?php
function verificarSessaoMaisMedicos(){

	$erro = false;
	if( !$_SESSION['par']['muncod']){
		$erro = true;
	}

	if( !$_SESSION['par']['prgid'] ){
		$erro = true;
	}

	if( $erro ){
		echo "
			<script>
				alert('Erro de sessão.');
				window.location.href = 'par.php?modulo=principal/planoTrabalho&acao=A&tipoDiagnostico=programa';
			</script>";
		die();
	}
}

function anexarTermoPerceria($dados, $files){
	global $db;

    include_once APPRAIZ . "includes/classes/file.class.inc";
    include_once APPRAIZ . "includes/classes/fileSimec.class.inc";

    $muncod_anexo = $dados['muncod_anexo'];
    $rqmid  = $dados['rqmid'];

    if( $files['arquivo_'.$muncod_anexo]['tmp_name'] ){

    	$arqdescricao = "Mais Médico #{$muncod_anexo}";
        $arquivo = 'arquivo_'.$muncod_anexo;

        $file = new FilesSimec("parcamaismedico");
        $file->setUpload($arqdescricao, $arquivo, false, false);

        $arqid = $file->getIdArquivo();
    }

    if( $arqid != '' ){
        $sql = " UPDATE par.parcamaismedico SET arqid = {$arqid} WHERE rqmid = {$rqmid} AND muncod = '{$muncod_anexo}' RETURNING pmmid;";
        $pmmid = $db->pegaUm($sql);

        if( $pmmid > 0 ){
   			$db->commit();
            $db->sucesso('principal/programas/feirao_programas/maisMedicosInforMunicipio&acao=A');
        } else {
            $db->insucesso('Não foi possivél anexar o Arquivo, tente novamente mais tarde!', '', 'principal/programas/feirao_programas/maisMedicosInforMunicipio&acao=A');
        }
    } else {
        $db->insucesso('Não foi possivél anexar o Arquivo, tente novamente mais tarde!', '', 'principal/programas/feirao_programas/maisMedicosInforMunicipio&acao=A');
    }
}

function recuperarMunicipio(){
	global $db;

	$sql = "SELECT 		mundescricao || ' - ' || estuf AS municipio
			FROM		territorios.municipio
			WHERE		muncod = '{$_SESSION['par']['muncod']}'";

	$rs = $db->pegaUm($sql);
	return $rs;
}

function recuperarRegiaoSaude(){
	global $db;

	$ibge = substr($_SESSION['par']['muncod'],0,6);

	$sql = "SELECT 		rgsnome
			FROM		par.regiaosaude
			WHERE		muncod = '{$ibge}'";

	$rs = $db->pegaUm($sql);
	return $rs;
}

function verificarRQMID(){
	global $db;

	$sql = "SELECT 		rqmid
			FROM		par.respquestaomaismedico
			WHERE		muncod = '{$_SESSION['par']['muncod']}' AND prgid = {$_SESSION['par']['prgid']}";
	
	$rs = $db->pegaUm($sql);
	return $rs;
}

function verificarMunicipioLiberado(){
	global $db;

	$sql = "SELECT 		l.muncod
			FROM		maismedicomec.municipioliberado l
			INNER JOIN territorios.municipio AS m ON substr(m.muncod, 1, 6) = l.muncod
			WHERE		l.muncod = '".substr($_SESSION['par']['muncod'],0,6)."' AND statusliberacao = 't'";

	$rs = $db->pegaUm($sql);
	return $rs;
}

function verificarPopulacao(){
	global $db;

	$ibge = substr($_SESSION['par']['muncod'],0,6);

	$sql = "SELECT 		popnumpopulacao
			FROM		par.populacao
			WHERE		popcodigomunicipio = '{$ibge}'";

	$rs = $db->pegaUm($sql);
	return $rs;
}

function verificarVagas(){
	global $db;

	$sql = "SELECT 		vgmnumvagashab, vgmmedicohab
			FROM		par.vagasmedicos
			WHERE		estuf = '{$_SESSION['par']['estuf']}'";

	$rs = $db->pegaLinha($sql);
	return $rs;
}

function verificarCapitalEstado(){
	global $db;

	$sql = "SELECT 		estuf
			FROM		territorios.estado
			WHERE		muncodcapital = '{$_SESSION['par']['muncod']}'";

	$rs = $db->pegaUm($sql);
	return $rs;
}

function visualizarInfor($rqmid){
	global $db;

	$sql = "SELECT 		*
			FROM		par.respquestaomaismedico
			WHERE		rqmid = {$rqmid}";

	$rs = $db->pegaLinha($sql);
	return $rs;
}

function verificarDadosMunicipios(){
	global $db;

	$ibge = substr($_SESSION['par']['muncod'],0,6);

	$sql = "SELECT 		leitos_sus::numeric AS leitos_sus,
						vagas_pleiteadas,
       					leitos_abertura,
       					REPLACE(grau_comprometimento, ',', '.')::numeric AS grau_comprometimento,
       					leito_sus_vaga_municipio,
       					hospital_ensino,
       					residencia_medica::numeric AS residencia_medica,
       					hospital_100_leitos,
       					pronto_socorro,
       					adesao_pmaq,
       					caps,
       					REPLACE(equipe_atencao_basica,',','.')::numeric AS equipe_atencao_basica,
       					vagas_equipe
			FROM		par.dadosmunicipios
			WHERE		codigo_ibge = '{$ibge}'";

	$rs = $db->pegaLinha($sql);
	return $rs;
}

function salvarInfor($post){
	global $db;
	extract($post);

	$rqmquestao03 = $rqmquestao03 ? str_replace('.','',$rqmquestao03) : 'null';
	$rqmquestao06 = $rqmquestao06 ? $rqmquestao06 : 'null';

	$sql = "INSERT INTO par.respquestaomaismedico(
	            		muncod,
	            		prgid,
	            		rqmquestao03,
	            		rqmquestao04,
	            		rqmquestao05,
	            		rqmquestao06,
	            		usucpf)
	    	VALUES 		('{$muncod}',
						 {$prgid},
						 '{$rqmquestao03}',
						 '{$rqmquestao04}',
						 '{$rqmquestao05}',
						 {$rqmquestao06},
						 '{$_SESSION['usucpf']}')
			RETURNING 	 rqmid";

	$rqmid = $db->pegaUm($sql);

	if($rqmid){
		$db->commit();
		return $rqmid;
	} else {
		return false;
	}
}

function alterarInfor($post){
	global $db;
	extract($post);

	$rqmquestao01 = $rqmquestao01 ? "'{$rqmquestao01}'" : 'null';
	$rqmquestao02 = $rqmquestao02 ? "'{$rqmquestao02}'" : 'null';
	$rqmquestao07 = $rqmquestao07 ? $rqmquestao07 : 'null';

	$rqmquestao08 = $rqmquestao08 ? $rqmquestao08 : "f";
	$rqmquestao09 = $rqmquestao09 ? str_replace('.','',$rqmquestao09) : 'null';
	$rqmquestao10 = $rqmquestao10 ? $rqmquestao10 : "f";

	$rqmquestao10item1 = $rqmquestao10item1 ? $rqmquestao10item1 : "f";
	$rqmquestao10item2 = $rqmquestao10item2 ? $rqmquestao10item2 : "f";
	$rqmquestao10item3 = $rqmquestao10item3 ? $rqmquestao10item3 : "f";
	$rqmquestao10item4 = $rqmquestao10item4 ? $rqmquestao10item4 : "f";
	$rqmquestao10item5 = $rqmquestao10item5 ? $rqmquestao10item5 : "f";

	$rqmquestao11 = $rqmquestao11 ? $rqmquestao11 : "f";
	$rqmquestao12 = $rqmquestao12 ? $rqmquestao12 : "f";
	$rqmquestao13 = $rqmquestao13 ? $rqmquestao13 : "f";
	$rqmquestao14 = $rqmquestao14 ? $rqmquestao14 : "f";
	$rqmparecermec = $rqmparecermec ? "'{$rqmparecermec}'" : 'null';

	$sql = "UPDATE 	par.respquestaomaismedico
			SET		rqmquestao01 = {$rqmquestao01},
				 	rqmquestao02 = {$rqmquestao02},
				 	rqmquestao07 = {$rqmquestao07},
				 	rqmquestao08 = '{$rqmquestao08}',
				 	rqmquestao09 = {$rqmquestao09},
				 	rqmquestao10 = '{$rqmquestao10}',
                 	rqmquestao10item1 = '{$rqmquestao10item1}',
                 	rqmquestao10item2 = '{$rqmquestao10item2}',
            		rqmquestao10item3 = '{$rqmquestao10item3}',
            		rqmquestao10item4 = '{$rqmquestao10item4}',
            		rqmquestao10item5 = '{$rqmquestao10item5}',
            		rqmquestao11 = '{$rqmquestao11}',
            		rqmquestao12 = '{$rqmquestao12}',
            		rqmquestao13 = '{$rqmquestao13}',
            		rqmquestao14 = '{$rqmquestao14}',
            		rqmparecermec = $rqmparecermec,
            		rqmparecermectexto = '{$rqmparecermectexto}'
			WHERE	rqmid = {$rqmid}";

	$db->executar($sql);

    if($db->commit()){
       	$db->sucesso( 'principal/programas/feirao_programas/maisMedicosInforMunicipio');
    }
}

function verificarRegiaoSaude(){
	global $db;
	$ibge = substr($_SESSION['par']['muncod'],0,6);

	$sql = "SELECT  rgscodigo
			FROM 	par.regiaosaude
			WHERE	muncod = '{$ibge}'";

	$rs = $db->pegaUm($sql);
	return $rs;
}

function pesquisarRegiaoSaude($post = null){
    global $db;

    $ibge = substr($_SESSION['par']['muncod'],0,6);
    $rgscodigo = verificarRegiaoSaude();
    $post ? extract($post) : '';

    $aryWhere[] = "rgscodigo = trim('{$rgscodigo}')";
    $aryWhere[] = "r.muncod <> '{$ibge}'";

    if($descricao){
        $descricao = utf8_decode($descricao);
        $aryWhere[] = "rgsmuncir ILIKE '%{$descricao}%'";
    }

    $sql = "
        SELECT  CASE WHEN rgsnumleitosus < 50
                    THEN '<input type=\"checkbox\" name=\"muncod[]\" id=\"muncod\" value=\"'||r.muncod||'\" disabled>'
                    ELSE
                        CASE WHEN p.muncod <> ''
                            THEN '<input type=\"checkbox\" name=\"muncod[]\" id=\"muncod\" value=\"'||r.muncod||'\" checked=\"checked\">'
                            ELSE '<input type=\"checkbox\" name=\"muncod[]\" id=\"muncod\" value=\"'||r.muncod||'\">'
                        END
                END AS acao,


                rgsmuncir,

                CASE WHEN rgsnumleitosus < 50
                    THEN '<font color=\"#FF0000\"><b>'||rgsnumleitosus||'</b></font>'
                    ELSE '<font color=\"#000000\">'||rgsnumleitosus||'</font>'
                END AS rgsnumleitosus,

                rgsnumequipeAB,

                CASE WHEN rgsnumleitosus < 50
                    THEN '<input id=\"pmmnumleitos'||r.muncod||'\" type=\"text\" name=\"pmmnumleitos'||r.muncod||'\" id=\"pmmnumleitos\" value=\"\" size=\"5\" onkeyup=\"this.value=mascaraglobal(\'[#]\',this.value);\" disabled>'
                    ELSE
                        CASE WHEN p.muncod <> ''
                            THEN '<input id=\"pmmnumleitos'||r.muncod||'\" type=\"text\" name=\"pmmnumleitos'||r.muncod||'\" id=\"pmmnumleitos\" value=\"'||P.PMMNUMLEITOS||'\" size=\"5\" onkeyup=\"this.value=mascaraglobal(\'[#]\',this.value);\">'
                            ELSE '<input id=\"pmmnumleitos'||r.muncod||'\" type=\"text\" name=\"pmmnumleitos'||r.muncod||'\" id=\"pmmnumleitos\" value=\"\" size=\"5\" onkeyup=\"this.value=mascaraglobal(\'[#]\',this.value);\">'
                        END
                END AS pmmnumleitos

        FROM par.regiaosaude as r

        LEFT JOIN par.parcamaismedico AS p ON p.muncod = r.muncod AND p.rqmid = {$_SESSION['par']['rqmid']}

        ".(is_array($aryWhere) ? ' WHERE '.implode(' AND ', $aryWhere) : '')."

        ORDER BY rgsmuncir
    ";
    $alinhamento = array('center','','center','center','center');
    $cabecalho = array('Ação','Município', 'Nº Leito SUS', 'Nº Leito AB', 'Nº Leitos da Parceira');
    $db->monta_lista($sql, $cabecalho, '50','10', '', '', '', 'formulario_regiao','',$alinhamento);
}

function exibirRegiaoMunicipio( $pmmid ){
    global $db;

    $sql = "
        SELECT  CASE WHEN rgsnumleitosus < 50
                    THEN '<input type=\"checkbox\" name=\"muncod[]\" id=\"muncod\" value=\"'||pm.muncod||'\" disabled>'
                    ELSE
                        CASE WHEN pm.muncod <> ''
                            THEN '<input type=\"checkbox\" name=\"muncod[]\" id=\"muncod\" value=\"'||pm.muncod||'\" checked=\"checked\">'
                            ELSE '<input type=\"checkbox\" name=\"muncod[]\" id=\"muncod\" value=\"'||pm.muncod||'\">'
                        END
                END AS acao,

                rgsmuncir,
                CASE WHEN rgsnumleitosus < 50
                    THEN '<font color=\"#FF0000\"><b>'||rgsnumleitosus||'</b></font>'
                    ELSE '<font color=\"#000000\">'||rgsnumleitosus||'</font>'
                END,

                rgsnumequipeAB,

                CASE WHEN rgsnumleitosus < 50
                    THEN '<input id=\"pmmnumleitos'||pm.muncod||'\" type=\"text\" name=\"pmmnumleitos'||pm.muncod||'\" id=\"pmmnumleitos\" value=\"'||pm.pmmnumleitos||'\" size=\"5\" onkeyup=\"this.value=mascaraglobal(\'[#]\',this.value);\" disabled>'
                    ELSE '<input id=\"pmmnumleitos'||pm.muncod||'\" type=\"text\" name=\"pmmnumleitos'||pm.muncod||'\" id=\"pmmnumleitos\" value=\"'||pm.pmmnumleitos||'\" size=\"5\" onkeyup=\"this.value=mascaraglobal(\'[#]\',this.value);\">'
                END AS pmmnumleitos

        FROM par.parcamaismedico pm

        LEFT JOIN par.regiaosaude rs ON pm.muncod = rs.muncod
        LEFT JOIN public.arquivo ar ON ar.arqid = pm.arqid

        WHERE pmmid = {$pmmid}
    ";
    $alinhamento = array('','','center','center','center');
    $cabecalho = array('Ação','Município', 'Nº Leito SUS', 'Nº Leito AB', 'Nº Leitos da Parceira');
    $db->monta_lista($sql, $cabecalho, '50','10', '', '', '', 'formulario_regiao','',$alinhamento);
}

function salvarRegiaoMunicipio( $dados ){
    global $db;

    foreach($dados['muncod'] as $muncod){

        $sql    = "SELECT 	pmmid
        		   FROM 	par.parcamaismedico
        		   WHERE 	rqmid = {$dados['rqmid']} AND muncod = '{$muncod}'";
        $pmmid  = $db->pegaUm($sql);

        if($pmmid == ''){
            $sql = "INSERT INTO par.parcamaismedico(
                    	rqmid,
                    	muncod,
                   		pmmnumleitos
	                )VALUES (
	                    {$dados['rqmid']},
	                    '{$muncod}',
	                    {$dados['pmmnumleitos'.$muncod]}
	                ) RETURNING pmmid";
            $pmmid = $db->pegaUm($sql);
        } else {
            $sql = "UPDATE 	par.parcamaismedico
                    SET 	pmmnumleitos = '{$dados['pmmnumleitos'.$muncod]}'
                	WHERE 	pmmid = {$pmmid} RETURNING pmmid";
            $pmmid = $db->pegaUm($sql);
        }
    }

    if( $pmmid > 0 ){
        $db->commit();
        $db->sucesso('principal/programas/feirao_programas/maisMedicosInforMunicipio', '', 'Operação realizada com sucesso!', 'S', 'S');
    }
}

function alterarRegiaoMunicipio($post,$files){
	global $db;
	include_once APPRAIZ . "includes/classes/fileSimec.class.inc";

	foreach($post['muncod'] as $m){
		if($files['arquivo'.$m]['tmp_name']){
			$sql = "DELETE FROM par.parcamaismedico WHERE pmmid = {$post['pmmid']}";
			$db->executar($sql);

			$arqdescricao = "Mais Médico #{$muncod}";
			$arquivo = 'arquivo'.$m;
			$aryCampos = array("rqmid" => $post['rqmid'],"muncod" => $m,"pmmnumleitos" => $post['pmmnumleitos'.$m]);
			$file = new FilesSimec("parcamaismedico",$aryCampos,"par");
			$file->setUpload($arqdescricao,$arquivo);
	       	echo "<script language=\"javascript\" type=\"text/javascript\">
	       				window.opener.location.href='par.php?modulo=principal/programas/feirao_programas/maisMedicosInforMunicipio&acao=A';
	       				window.close();
	       	     </script>";
			exit();
		} else {
			$sql = "UPDATE 	par.parcamaismedico
					SET		pmmnumleitos = '{$post['pmmnumleitos'.$m]}'
					WHERE	pmmid = {$post['pmmid']}";

			$db->executar($sql);
		}
	}

    if($db->commit()){
       	echo "
            <script language=\"javascript\" type=\"text/javascript\">
                window.opener.location.href='par.php?modulo=principal/programas/feirao_programas/maisMedicosInforMunicipio&acao=A';
                window.close();
       	     </script>
        ";
    }
}

function listarParceiro(){
    global $db;

    $acao = "<img src=\"../imagens/alterar.gif\" id=\"' || pm.pmmid ||'\" class=\"alterar\" onclick=\"alterarParceiro('|| pm.pmmid ||');\" style=\"cursor:pointer;\"/>
        	 <img border=\"0\" src=\"../imagens/excluir.gif\" id=\"'|| pm.pmmid ||'\" onclick=\"excluirParceiro('|| pm.pmmid ||');\" style=\"cursor:pointer;\"/>";

    $acao_anexo = "<img src=\"../imagens/alterar.gif\" id=\"' || pm.pmmid ||'\" class=\"alterar\" onclick=\"alterarParceiro('|| pm.pmmid ||');\" style=\"cursor:pointer;\"/>
        		   <img border=\"0\" src=\"../imagens/excluir.gif\" id=\"'|| pm.pmmid ||'\" onclick=\"excluirParceiro('|| pm.pmmid ||');\" style=\"cursor:pointer;\"/>
        		   <a href=\"par.php?modulo=principal/programas/feirao_programas/maisMedicosInforMunicipio&acao=A&download=S&arqid='|| ar.arqid ||'\"/><img src=\"../imagens/anexo.gif\" border=\"0\"></a>";

    $sql = "SELECT  CASE WHEN pm.arqid IS NULL
                    THEN '$acao'
                    ELSE '$acao_anexo'
                END AS acao,

                '<img border=\"0\" style=\"vertical-align:middle;cursor:pointer;\" src=\"../imagens/consultar.gif\" onclick=\"abrirTermo('|| pm.pmmid ||');\">' AS termo,

                tm.mundescricao,
                pmmnumleitos,

                CASE WHEN pm.arqid IS NOT NULL
                    THEN ar.arqnome||'.'||ar.arqextensao
                    ELSE '<input type=\"file\" name=\"arquivo_'||pm.muncod||'\" id=\"arquivo_'||pm.muncod||'\" />'
                END AS nome_arquivo,

                CASE WHEN pm.arqid IS NULL
                    THEN '<input type=\"button\" name=\"anexar_termo_'|| pm.muncod ||'\" id=\"anexar_termo_'|| pm.muncod ||'\" value=\"Anexar\" onclick=\"anexarTermoPerceria(\''|| pm.muncod ||'\');\" />'
                    ELSE '<input type=\"button\" name=\"anexar_termo\" id=\"anexar_termo\" value=\"Anexar\" disabled=\"disabled\"/>'
                END AS botao

        FROM par.parcamaismedico pm

        LEFT JOIN public.arquivo ar ON ar.arqid = pm.arqid
        LEFT JOIN territorios.municipio tm ON substr(tm.muncod,1,6) = pm.muncod
        WHERE rqmid = {$_SESSION['par']['rqmid']}

        ORDER BY tm.mundescricao
    ";
    $alinhamento = array('center','center','','center','center', 'center');
    $tamanho = Array('5%', '10%', '40%', '10%', '20%', '10%');
    $cabecalho = array('Ação','Termo de Parceria','Município','Nº Leitos da Parceria', 'Anexo', '');
    //$db->monta_lista($sql, $cabecalho, '50','10', '', '', '', 'formulario_regiao', $tamanho, $alinhamento);
    $db->monta_lista($sql, $cabecalho, '50','10', '', '', '', '', $tamanho, $alinhamento);
}

function totalLeitos($pmmid = null){
	global $db;

	if($pmmid){
		$aryWhere[] = "pmmid = {$pmmid}";
	} else {
		if($_SESSION['par']['rqmid']){
			$aryWhere[] = "rqmid = {$_SESSION['par']['rqmid']}";
		}
	}

	$sql = "SELECT 		SUM(pmmnumleitos) AS total_leitos
			FROM		par.parcamaismedico
						".(is_array($aryWhere) ? ' WHERE '.implode(' AND ', $aryWhere) : '')."";

	$rs = $db->pegaUm($sql);
	return $rs;
}

function recuperarMunicipioParceiro($pmmid){
	global $db;

	$sql = "SELECT 		mundescricao || ' - ' || estuf AS municipio
			FROM		par.parcamaismedico pm
			INNER JOIN	territorios.municipio tm ON substr(tm.muncod,1,6) = pm.muncod
			WHERE		pmmid = {$pmmid}";

	$rs = $db->pegaUm($sql);
	return $rs;
}

function recuperaDadosPrefeitura(){
	global $db;
/*
	$aryWhere[] = "(ent.entstatus = 'A' OR ent.entstatus IS NULL)";

	if($_SESSION['par']['muncod']){
		$aryWhere[] = "eed2.muncod = '{$_SESSION['par']['muncod']}'";
	}

	if($_SESSION['par']['estuf']){
		$aryWhere[] = "eed2.estuf = '{$_SESSION['par']['estuf']}'";
	}

	$sql = "SELECT				ent.entnome AS prefeito,
								ent2.entnumcpfcnpj AS cnpjmunicipio,
								mun.mundescricao AS municipio,
								est.estdescricao AS estado,
								mun.estuf AS estuf,
								eed2.endlog || ' ' || endnum || ' ' || endbai || ' ' || 'CEP:' || endcep || ' ' || mun.mundescricao || '-' || mun.estuf AS endereco
			FROM 				entidade.entidade ent
			INNER JOIN 			entidade.funcaoentidade fue ON fue.entid = ent.entid AND fue.funid = 2 AND fue.fuestatus = 'A'
			INNER JOIN 			entidade.funcao fun ON fun.funid = fue.funid
			LEFT JOIN 			entidade.funentassoc fea ON fea.fueid = fue.fueid
			LEFT JOIN 			entidade.entidade ent2 ON ent2.entid = fea.entid
			LEFT JOIN 			entidade.endereco eed2 ON eed2.entid = ent2.entid
			LEFT JOIN 			entidade.funcaoentidade fue2 ON fue2.entid = ent2.entid AND fue2.funid = 1 AND fue2.fuestatus = 'A'
			LEFT JOIN 			entidade.funcao fun2 ON fun2.funid = fue2.funid
			INNER JOIN 			territorios.municipio mun ON mun.muncod = eed2.muncod
			INNER JOIN			territorios.estado est ON est.estuf = mun.estuf
								".(is_array($aryWhere) ? ' WHERE '.implode(' AND ', $aryWhere) : '')."";
	*/

	$sql = "SELECT
		ent1.entnome AS prefeito,
		ent2.entnumcpfcnpj AS cnpjmunicipio,
		mun.mundescricao AS municipio,
		est.estdescricao AS estado,
		mun.estuf AS estuf,
		ent2.endlog || ' ' || ent2.endnum || ' ' || ent2.endbai || ' ' || 'CEP:' || ent2.endcep || ' ' || mun.mundescricao || '-' || mun.estuf AS endereco
	FROM
		par.entidade ent1
	INNER JOIN par.entidade ent2 ON ent1.inuid = ent2.inuid AND ent2.entstatus = 'A' AND ent2.dutid = ".DUTID_PREFEITURA."
	INNER JOIN territorios.municipio mun ON mun.muncod = ent2.muncod
	INNER JOIN territorios.estado est ON est.estuf = mun.estuf
	WHERE
		ent1.entstatus = 'A' AND
		ent1.dutid = ".DUTID_PREFEITO." AND
		ent2.muncod='{$_SESSION['par']['muncod']}'";

	$rs = $db->pegaLinha($sql);
	return $rs;
}

function recuperaGestorSusMunicipio(){
	global $db;

	$sql = "SELECT 		dmmnome
			FROM		par.dadosmaismedicos
			WHERE		muncod = '{$_SESSION['par']['muncod']}' AND dmmtipo = 'S'";

	$rs = $db->pegaUm($sql);
	return $rs;
}


function excluirParceiro($pmmid) {
    global $db;

    $SQL = " SELECT arqid FROM par.parcamaismedico WHERE pmmid = {$pmmid}; ";
    $arqid = $db->pegaUm($SQL);

    if (pmmid != '') {
        $sql = "DELETE FROM par.parcamaismedico WHERE pmmid = {$pmmid}";
    }

    if( $db->executar($sql) ){

        if($arqid != ''){
            include_once APPRAIZ . "includes/classes/file.class.inc";
            include_once APPRAIZ . "includes/classes/fileSimec.class.inc";

            $file = new FilesSimec("parcamaismedico");
            $file->excluiArquivoFisico($arqid);
        }

        $db->commit();
    }
}

#VERIFICA SE O MUNICIPIPO OFERECE CURSO DE MEDICINA, SE CASO SIM, "SE O MUNICIPÍO FAZER PARTE DOS QUE ESTAO NA BASE", ELE NÃO TEM CONDIÇÕES DE PARTICIPAÇÕES.
function naoPossuiOfertaCurso(){
    global $db;

    $sql = "SELECT  	cmemuncod
        	FROM 		par.cursosmedicinaemec
        	WHERE 		cmemuncod = '{$_SESSION['par']['muncod']}'";

    $muncod = $db->pegaUm($sql);

    if($muncod > 0){
        $oferta = 'S';
    } else {
        $oferta = 'N';
    }
    return $oferta;
}

function salvarTermoResidencia($post){
	global $db;

	extract($post);

	$sql = "UPDATE 		par.respquestaomaismedico
   			SET 		rqmaceitetermoresidencia = '{$rqmaceitetermoresidencia}'
			WHERE 		rqmid = {$rqmid}";
	$db->executar($sql);

	if($db->commit()){
		echo 'S';
	} else {
		echo 'N';
	}
}

function verificarLeitosDisp(){
	global $db;

	$ibge = substr($_SESSION['par']['muncod'],0,6);
	$rgscodigo = verificarRegiaoSaude();

	$sql = "SELECT 	COUNT(muncod) as qtd_ndisp
			FROM	par.regiaosaude
			WHERE	rgscodigo = trim('{$rgscodigo}') AND muncod <> '{$ibge}' AND rgsnumleitosus < 50";

	$qtd_ndisp = $db->pegaUm($sql);

	$sql = "SELECT 	COUNT(muncod) as qtd_disp
			FROM	par.regiaosaude
			WHERE	rgscodigo = trim('{$rgscodigo}') AND muncod <> '{$ibge}'";

	$qtd_disp = $db->pegaUm($sql);

	if($qtd_ndisp == $qtd_disp){
		return true;
	} else {
		return false;
	}
}

function salvarAdesao($post = null){
	global $db;

	extract($post);

	$sql = "SELECT 		adpid
			FROM	 	par.pfadesaoprograma
			WHERE	 	pfaid = {$_SESSION['par']['pfaid']}
			AND 		inuid = {$_SESSION['par']['inuid']}";

	$adpid = $db->pegaUm($sql);

	if(!$adpid){
		$tapid = $tapid ? $tapid : 'null';
		$adpano = date('Y');

		$sql = "INSERT INTO 	par.pfadesaoprograma(
								pfaid,
								adpano,
								inuid,
								adpdataresposta,
								adpresposta,
								usucpf,
								tapid)
				VALUES (
								{$_SESSION['par']['pfaid']},
								'{$adpano}',
								{$_SESSION['par']['inuid']},
								now(),
								'{$adpresposta}',
								'{$_SESSION['usucpf']}',
							    {$tapid}
					    ) returning adpid ";

			$adpid = $db->pegaUm($sql);
			$_SESSION['par']['adpid'] = $adpid;
		} else {
			$sql = "UPDATE 		par.pfadesaoprograma
					SET	   	   	adpdataresposta = now(),
					   	   		adpresposta = '{$adpresposta}',
					       		usucpf = '{$_SESSION['usucpf']}'
					WHERE 		adpid = {$adpid}";
			$db->executar($sql);
		}

		if($db->commit()){
			$sql = "INSERT INTO 	par.pfadesaoprogramahistorico(
		            				adpid,
		            				aphano,
		            				inuid,
		            				aphdataresposta,
		           					aphresposta,
		           					usucpf,
		           					tapid,
		           					pfaid)
				    VALUES 			($adpid,
						    		'{$adpano}',
						    		{$_SESSION['par']['inuid']},
						    		now(),
									'{$resposta}',
									'{$_SESSION['usucpf']}',
								    {$tapid},
									{$_SESSION['par']['pfaid']})";
			$db->executar($sql);

			if($db->commit()){
				echo "S";
			} else {
				echo "N";
			}
		} else {
			echo "N";
		}
}

function verificarAdesao(){
	global $db;

	if($_SESSION['par']['adpid']){
		$sql = "SELECT		adpresposta
				FROM		par.pfadesaoprograma
				WHERE		adpid = {$_SESSION['par']['adpid']}
				GROUP BY	adpid, adpresposta
				ORDER BY 	adpid DESC";

		$adpresposta = $db->pegaUm($sql);
		return $adpresposta;
	} else {
		return 'N';
	}
}

function gerarRelatorio($post = null) {
    global $db;

    $post ? extract($post) : '';

    if ($prgid){
        $aryWhere[] = "rm.prgid = {$prgid}";
    }
    
    if ($esdid) {
        $aryWhere[] = "es.esdid = {$esdid}";
    }

    if ($estuf) {
        $aryWhere[] = "tm.estuf = '{$estuf}'";
    }

    if ($mundescricao) {
        $mundescricao = removeAcentos(str_replace("-", " ", (trim(utf8_decode($mundescricao)))));
        $aryWhere[] = "UPPER(public.removeacento(tm.mundescricao)) ILIKE '%{$mundescricao}%'";
    }

    if ($parceria) {
        if ($parceria == 'S') {
            $aryWhere[] = "ls.total_leitos IS NOT NULL";
        } else {
            $aryWhere[] = "ls.total_leitos IS NULL";
        }
    }

    if ($adesao) {
        if ($adesao == 'f') {
            $aryWhere[] = "rm.rqmaceitetermoresidencia IS NULL";
        } else {
            $aryWhere[] = "rm.rqmaceitetermoresidencia = '{$adesao}'";
        }
    }

    if($tipo_relatorio == 'XLS'){
        $acao = "tm.mundescricao";
        $acao_novo = "tm.mundescricao";
    }else{
        $acao = "'<a href=\"par.php?modulo=principal/programas/feirao_programas/maisMedicosInforMunicipio&acao=A&muncod='||tm.muncod||'&rqmid='||rm.rqmid||'\">'||tm.mundescricao||'</a>'";
        $acao_novo = "'<a href=\"par.php?modulo=principal/programas/feirao_programas/mais_medicos_2015/condicoes_participacao&acao=A&rel_muncod='||tm.muncod||'&rel_prgid='||rm.prgid||'\">'||tm.mundescricao||'</a>'";
    }

    $sql = "
        SELECT  tm.estuf,
		CASE WHEN prgid = 251
                    THEN {$acao_novo}
                    ELSE {$acao} 
                END AS municipio,                
                CASE WHEN prgid = 251
                    THEN 'Novo Edital 2015'
                    ELSE 'Mais Médico'
                END AS prgid, 
                es.esddsc,
                rqmquestao06,
                ls.total_leitos,
                (rqmquestao06 + ls.total_leitos) AS total_geral,
                CASE WHEN rm.rqmaceitetermoresidencia = 't' THEN 'Sim' ELSE 'Não' END AS aceite,
                us.usunome
        FROM par.respquestaomaismedico rm

        INNER JOIN territorios.municipio tm ON tm.muncod = rm.muncod
        LEFT JOIN (SELECT  SUM(pmmnumleitos) AS total_leitos, rqmid FROM par.parcamaismedico GROUP BY rqmid) AS ls ON ls.rqmid = rm.rqmid
        LEFT JOIN workflow.documento dc ON dc.docid = rm.docid
        LEFT JOIN workflow.estadodocumento es ON es.esdid = dc.esdid
        LEFT JOIN seguranca.usuario us ON us.usucpf = rm.usucpf

        ".(is_array($aryWhere) ? ' WHERE ' .implode(' AND ', $aryWhere) : '')."

        ORDER BY prgid, tm.estuf, tm.mundescricao
    ";
    $alinhamento = array('', '', '', 'center', 'center', 'center', 'center');
    $cabecalho = array('UF', 'Município', 'Programa', 'Estado Workflow', 'Nº Leitos Município', 'Nº Leitos Parceria', 'Total Geral Leitos', 'Adesão (Termo de Residência)', 'Responsável');
    $db->monta_lista($sql, $cabecalho, '50', '10', '', '', '', '', '', $alinhamento);


    if($tipo_relatorio == 'XLS'){
        ob_clean();
	header('content-type: text/html; charset=ISO-8859-1');

	$db->sql_to_excel($sql, 'Relatório_Controle_Financeiro_EJA', $cabecalho);
    }
}

function enviarAnaliseMEC(){
	global $db;

	$leitos_parceria = totalLeitos();

	$sql = "SELECT 		rqmquestao06, rqmaceitetermoresidencia
			FROM 		par.respquestaomaismedico
			WHERE		rqmid = {$_SESSION['par']['rqmid']}";

	$dados = $db->pegaLinha($sql);

	$total_leitos = $leitos_parceria + $dados['rqmquestao06'];

	if($total_leitos < 250){
		return 'Número de leitos menor que 250!';
	} elseif($dados['rqmaceitetermoresidencia'] == 'f' || $dados['rqmaceitetermoresidencia'] == '') {
		return 'Termo de Compromisso Residência Médica não aderido!';
	} else {
		return true;
	}
}

function salvarDoc($file,$post){
    global $db;

    extract($post);

    if($file['arquivo']['tmp_name']){
        $aryCampos = array(
            "rqmid" => $rqmid,
            "aqmsituacao" => "'A'",
            "aqmdtinclusao" => "now()",
            "tpaid" => $tpaid
        );
        $file = new FilesSimec("arquivosmunicipio", $aryCampos, "par");
        $file->setUpload(substr($arqdescricao, 0, 255),"arquivo");

        if( isset($tipo_parecer) ){
            return true;
        }else{
            header("Location: par.php?modulo=principal/programas/feirao_programas/maisMedicosDocumentos&acao=A");
        }

        exit();
    } else {
        $_SESSION['cap']['mgs'] = "Não foi possível realizar a operação!";
        header("Location: par.php?modulo=principal/programas/feirao_programas/maisMedicosDocumentos&acao=A");
        exit();
    }
}

function excluirDoc($dados){
    global $db;
    
    include_once APPRAIZ . "includes/classes/fileSimec.class.inc";
    
    $arryModulo = explode("/", $dados['modulo']);
    $url = $arryModulo[3];
    
    if( $url == 'maisMedicosRecursoQuest' ){
        $arqid = $dados['arqid'];
    }else{
        $arqid = $dados;
    }    
    
    if ($arqid != '') {
        $sql = "UPDATE par.arquivosmunicipio SET aqmsituacao = 'I' WHERE arqid = {$arqid} ";
    }
    
    if( $db->executar($sql) ){
        $file = new FilesSimec('arquivosmunicipio', $campos, 'par');
        $file->excluiArquivoFisico( $arqid );
        
        $db->commit();
        
        if( $url == 'maisMedicosRecursoQuest' ){
            $db->sucesso('principal/programas/feirao_programas/maisMedicosRecursoQuest');
        }
    }
}

function exibirListaDoc($tipo = NULL){
    // header('Content-Type: text/html; charset=iso-8859-1');

    global $db;

    $arrayPerfil = pegaArrayPerfil($_SESSION['usucpf']);

    $where = "am.aqmsituacao = 'A'";

    if ($_SESSION['par']['rqmid']) {
        $where .= " AND am.rqmid = {$_SESSION['par']['rqmid']}";
    }

    if ($_SESSION['par']['muncod']) {
        $where .= " AND rm.muncod = '{$_SESSION['par']['muncod']}'";
    }

    if( $tipo == 'R'){
        $where .= " AND am.tpaid = 21";
    }

    if( $tipo == 'M'){
        $where .= " AND am.tpaid = 22";
    }

    if(in_array(PAR_PERFIL_CONSULTA_MUNICIPAL, $arrayPerfil) || in_array(PAR_PERFIL_CONTROLE_SOCIAL_MUNICIPAL, $arrayPerfil) || in_array(PAR_PERFIL_PREFEITO, $arrayPerfil) || in_array(PAR_PERFIL_AVAL_INSTITUCIONAL_MM, $arrayPerfil) ){
        $acao = "
            <a href=\"par.php?modulo=principal/programas/feirao_programas/maisMedicosDocumentos&acao=A&download=S&arqid='|| am.arqid ||'\" >
                <img src=\"../imagens/anexo.gif\" border=\"0\">
            </a>
            <img border=\"0\" src=\"../imagens/excluir_01.gif\" id=\"'|| am.arqid ||'\"/>
        ";
    } else {
        $acao = "
            <a href=\"par.php?modulo=principal/programas/feirao_programas/maisMedicosDocumentos&acao=A&download=S&arqid='|| am.arqid ||'\" >
                <img src=\"../imagens/anexo.gif\" border=\"0\">
            </a>
            <img border=\"0\" src=\"../imagens/excluir.gif\" id=\"'|| am.arqid ||'\" onclick=\"excluirDoc('|| am.arqid ||');\" style=\"cursor:pointer;\"/>
        ";
    }

    $sql = "
        SELECT  '{$acao}' AS acao,
            	ar.arqnome||'.'||ar.arqextensao AS nome_arquivo,
                ar.arqdescricao,
                ta.tpadsc,
                us.usunome
        FROM par.arquivosmunicipio am
        JOIN par.respquestaomaismedico rm ON am.rqmid = rm.rqmid
        JOIN public.arquivo ar ON ar.arqid = am.arqid
        JOIN par.tipoarquivo ta ON ta.tpaid = am.tpaid
        JOIN seguranca.usuario us ON us.usucpf = ar.usucpf

        WHERE {$where}

        ORDER BY am.aqmdtinclusao DESC
    ";
    $alinhamento = array('center','left','left','left','left');
    $tamanho = array('5%','18%','18%','18%','18%');
    $cabecalho = array('Ação', 'Nome Arquivo','Descrição','Tipo de Arquivo','Responsável');
    $db->monta_lista($sql, $cabecalho, '50', '10', '', '', '', '',$tamanho, $alinhamento);
}

function verificarExisteGestorRepres(){
	global $db;

	$sql = "SELECT 		dmmnome
			FROM 		par.dadosmaismedicos
			WHERE		muncod = '{$_SESSION['par']['muncod']}' AND dmmtipo = 'S'";

	$sus = $db->pegaLinha($sql);

	$sql = "SELECT 		dmmnome
			FROM 		par.dadosmaismedicos
			WHERE		muncod = '{$_SESSION['par']['muncod']}' AND dmmtipo = 'M'";

	$gestor = $db->pegaLinha($sql);

	if($sus && $gestor){
		return 't';
	} else {
		return 'f';
	}
}

function verificarParceria(){
	global $db;

	$sql = "SELECT 	COUNT(pmmid)
			FROM 	par.parcamaismedico
			WHERE	muncod = '{$_SESSION['par']['muncod']}'";

	$parceria = $db->pegaUm($sql);

	if($parceria > 0){
		return 't';
	} else {
		return 'f';
	}
}

function salvarParecer($post){
	global $db;
	extract($post);

	$sql = "UPDATE 		par.respquestaomaismedico
			SET	   	   	rqmparintroducao = '{$rqmparintroducao}',
			   	   		rqmparanalise = '{$rqmparanalise}',
			       		rqmparconclusao = '{$rqmparconclusao}'
			WHERE 		rqmid = {$rqmid}
			RETURNING 	rqmid";

	$parecer = $db->pegaUm($sql);

	if($parecer > 0){
		$db->commit();
		$db->sucesso('principal/programas/feirao_programas/maisMedicosAnalise','&aba=Parecer','Operação realizada com sucesso!','N','N');
	} else {
        $db->insucesso('Não foi possivel realizar a operação!', '', 'principal/programas/feirao_programas/maisMedicosAnalise&acao=A&aba=Parecer');
	}
}

function salvarFicha($post){
	global $db;
	extract($post);

	$rqmsitdoccomprobatoria = $rqmsitdoccomprobatoria ? "'{$rqmsitdoccomprobatoria}'" : 'null';
	$rqmoficiodirigente = $rqmoficiodirigente ? "'{$rqmoficiodirigente}'" : 'null';
	$rqmcopiargcpfdir = $rqmcopiargcpfdir ? "'{$rqmcopiargcpfdir}'" : 'null';
	$rqmprojetomelhoria = $rqmprojetomelhoria ? "'{$rqmprojetomelhoria}'" : 'null';
	$rqmdocitem3133 = $rqmdocitem3133 ? "'{$rqmdocitem3133}'" : 'null';
	$rqmdocitem414243 = $rqmdocitem414243 ? "'{$rqmdocitem414243}'" : 'null';
	$rqmampliacaoequipebasica = $rqmampliacaoequipebasica ? "'{$rqmampliacaoequipebasica}'" : 'null';
	$rqmampliacaoleitos = $rqmampliacaoleitos ? "'{$rqmampliacaoleitos}'" : 'null';
	$rqmleitosurgencia = $rqmleitosurgencia ? "'{$rqmleitosurgencia}'" : 'null';
	$rqmhospitalensino = $rqmhospitalensino ? "'{$rqmhospitalensino}'" : 'null';
	$rqmhospitalleitoscurso = $rqmhospitalleitoscurso ? "'{$rqmhospitalleitoscurso}'" : 'null';
	$rqmampliacaoprogsaude = $rqmampliacaoprogsaude ? "'{$rqmampliacaoprogsaude}'" : 'null';

		$sql = "UPDATE 		par.respquestaomaismedico
			SET	   		rqmobsaval70mil = '{$rqmobsaval70mil}',
						rqmobsavacapitalestado = '{$rqmobsavacapitalestado}',
						rqmobsavalcursomedicina = '{$rqmobsavalcursomedicina}',
						rqmobsaval250leitos = '{$rqmobsaval250leitos}',
						rqmobsavalparceria = '{$rqmobsavalparceria}',
						rqmobsavalalunoequipbasica = '{$rqmobsavalalunoequipbasica}',
						rqmobsavalleitourgencia = '{$rqmobsavalleitourgencia}',
 						rqmobsavalresidmedica = '{$rqmobsavalresidmedica}',
						rqmobstermoresidencia = '{$rqmobstermoresidencia}',
						rqmobsavalpmaq = '{$rqmobsavalpmaq}',
						rqmobsavalcaps = '{$rqmobsavalcaps}',
						rqmobsavalhospensino = '{$rqmobsavalhospensino}',
						rqmobsaval100leitos = '{$rqmobsaval100leitos}',
						rqmobsavalgestorlocal = '{$rqmobsavalgestorlocal}',
						rqmobsavaltermomunic = '{$rqmobsavaltermomunic}',
						rqmobsavaldocoficio = '{$rqmobsavaldocoficio}',
						rqmobsavaldocrgcpf = '{$rqmobsavaldocrgcpf}',
						rqmobsavalprojetomelhoria = '{$rqmobsavalprojetomelhoria}',
						rqmobsavalitens3133 = '{$rqmobsavalitens3133}',
						rqmobsaval414243 = '{$rqmobsaval414243}',
						rqmsitdoccomprobatoria = $rqmsitdoccomprobatoria,
						rqmoficiodirigente = $rqmoficiodirigente,
						rqmcopiargcpfdir = $rqmcopiargcpfdir,
						rqmprojetomelhoria = $rqmprojetomelhoria,
						rqmdocitem3133 = $rqmdocitem3133,
						rqmdocitem414243 = $rqmdocitem414243,
						rqmsintesesegundaetapa = '{$rqmsintesesegundaetapa}',
						rqmampliacaoequipebasica = $rqmampliacaoequipebasica,
						rqmobsampliacaoequipebasica = '{$rqmobsampliacaoequipebasica}',
						rqmampliacaoleitos = $rqmampliacaoleitos,
						rqmobsampliacaoleitos = '{$rqmobsampliacaoleitos}',
						rqmleitosurgencia =  $rqmleitosurgencia,
						rqmobsleitosurgencia = '{$rqmobsleitosurgencia}',
						rqmhospitalensino = $rqmhospitalensino,
						rqmobshospitalensino = '{$rqmobshospitalensino}',
						rqmhospitalleitoscurso = $rqmhospitalleitoscurso,
						rqmobshospitalleitoscurso = '{$rqmobshospitalleitoscurso}',
						rqmampliacaoprogsaude = $rqmampliacaoprogsaude,
						rqmobsampliacaoprogsaude = '{$rqmobsampliacaoprogsaude}',
						rqmoutrainfo = '{$rqmoutrainfo}',
						rqmsinteseterceiraetapa = '{$rqmsinteseterceiraetapa}'
			WHERE 		rqmid = {$rqmid}
			RETURNING 	rqmid";

	$parecer = $db->pegaUm($sql);

	if($parecer > 0){
		$db->commit();
		$db->sucesso('principal/programas/feirao_programas/maisMedicosAnalise','&aba=Ficha','Operação realizada com sucesso!','N','N');
	} else {
        $db->insucesso('Não foi possivel realizar a operação!', '', 'principal/programas/feirao_programas/maisMedicosAnalise&acao=A&aba=Ficha');
	}
}

function recuperaDadosParecer($rqmid){

	global $db;

	if( $rqmid != '' ){
		$sql = "SELECT 		rqmparintroducao,
							rqmparanalise,
							rqmparconclusao
				FROM		par.respquestaomaismedico
				WHERE 		rqmid = {$rqmid}";

		$parecer = $db->pegaLinha($sql);
	}

	return $parecer;
}

function recuperaDadosFicha($rqmid){
	global $db;

	$sql = "
            SELECT  rqmobsaval70mil,
                    rqmobsavacapitalestado,
                    rqmobsavalcursomedicina,
                    rqmobsaval250leitos,
                    rqmobsavalparceria,
                    rqmobsavalalunoequipbasica,
                    rqmobsavalleitourgencia,
                    rqmobsavalresidmedica,
                    rqmobstermoresidencia,
                    rqmobsavalpmaq,
                    rqmobsavalcaps,
                    rqmobsavalhospensino,
                    rqmobsaval100leitos,
                    rqmobsavalgestorlocal,
                    rqmobsavaltermomunic,
                    rqmobsavaldocoficio,
                    rqmobsavaldocrgcpf,
                    rqmobsavalprojetomelhoria,
                    rqmobsavalitens3133,
                    rqmobsaval414243,
                    rqmsintesesegundaetapa,
                    rqmsitdoccomprobatoria,
                    rqmoficiodirigente,
                    rqmcopiargcpfdir,
                    rqmprojetomelhoria,
                    rqmdocitem3133,
                    rqmdocitem414243,
                    rqmampliacaoequipebasica,
                    rqmobsampliacaoequipebasica,
                    rqmampliacaoleitos,
                    rqmobsampliacaoleitos,
                    rqmleitosurgencia,
                    rqmobsleitosurgencia,
                    rqmhospitalensino,
                    rqmobshospitalensino,
                    rqmhospitalleitoscurso,
                    rqmobshospitalleitoscurso,
                    rqmampliacaoprogsaude,
                    rqmobsampliacaoprogsaude,
                    rqmoutrainfo,
                    rqmsinteseterceiraetapa,
                    CASE WHEN rqmampliacaoequipebasica = 'S' THEN 'Satisfatoriamente'
                             WHEN rqmampliacaoequipebasica = 'P' THEN 'Parcialmente satisfatorio'
                             WHEN rqmampliacaoequipebasica = 'I' THEN 'Insatisfatório' END AS ampliacaoequipebasica,
                    CASE WHEN rqmampliacaoleitos = 'S' THEN 'Satisfatoriamente'
                             WHEN rqmampliacaoleitos = 'P' THEN 'Parcialmente satisfatorio'
                             WHEN rqmampliacaoleitos = 'I' THEN 'Insatisfatório' END AS ampliacaoleitos,
                    CASE WHEN rqmleitosurgencia = 'S' THEN 'Satisfatoriamente'
                             WHEN rqmleitosurgencia = 'P' THEN 'Parcialmente satisfatorio'
                             WHEN rqmleitosurgencia = 'I' THEN 'Insatisfatório' END AS leitosurgencia,
                    CASE WHEN rqmhospitalensino = 'S' THEN 'Satisfatoriamente'
                             WHEN rqmhospitalensino = 'P' THEN 'Parcialmente satisfatorio'
                             WHEN rqmhospitalensino = 'I' THEN 'Insatisfatório' END AS hospitalensino,
                    CASE WHEN rqmhospitalleitoscurso = 'S' THEN 'Satisfatoriamente'
                             WHEN rqmhospitalleitoscurso = 'P' THEN 'Parcialmente satisfatorio'
                             WHEN rqmhospitalleitoscurso = 'I' THEN 'Insatisfatório' END AS hospitalleitoscurso,
                    CASE WHEN rqmampliacaoprogsaude = 'S' THEN 'Satisfatoriamente'
                             WHEN rqmampliacaoprogsaude = 'P' THEN 'Parcialmente satisfatorio'
                             WHEN rqmampliacaoprogsaude = 'I' THEN 'Insatisfatório' END AS ampliacaoprogsaude
            FROM par.respquestaomaismedico

            WHERE rqmid = {$rqmid}
        ";
	$parecer = $db->pegaLinha($sql);
	return $parecer;
}

#BUSCA DADOS MAIS MEDICOS. GETOR SUS E GETOR LOCAL "MUNICIPAL"
function buscaRecudoMaisMedicos($rqmid) {
    header('Content-Type: text/html; charset=iso-8859-1');

    global $db;

    $sql = "
        SELECT	rcmid,
                rqmid,
                replace(to_char(cast(usucpfmunicipio as bigint), '000:000:000-00'), ':', '.') as usucpfmunicipio,
                mun.usunome AS nome_municipio,
                replace(to_char(cast(usucpfmec as bigint), '000:000:000-00'), ':', '.') as usucpfmec,
                mec.usunome AS nome_mec,
                rcmtextmunicipio,
                rcmparecermec,
                rcmsituacao,
                rcmstatus,
                to_char(rcmdtinclusaomunicipio, 'DD/MM/YYYY') AS rcmdtinclusaomunicipio,
                to_char(rcmdtinclusaomec, 'DD/MM/YYYY') AS rcmdtinclusaomec
        FROM par.recursomaismedico r
        LEFT JOIN seguranca.usuario AS mun ON mun.usucpf = usucpfmunicipio
        LEFT JOIN seguranca.usuario AS mec ON mec.usucpf = usucpfmec
        WHERE rqmid = '{$rqmid}' AND rcmstatus = 'A'
    ";
    $dados = $db->pegaLinha($sql);

    return $dados;
}

#BUSCA DADOS MAIS MEDICOS. GETOR SUS E GETOR LOCAL "MUNICIPAL"
function buscaRecusoMaisMedicosQuestAvaliacao($rqmid) {
    header('Content-Type: text/html; charset=iso-8859-1');
    global $db;

    $sql = "
        SELECT	rqaid,
                rqmid,
                replace(to_char(cast(usucpfmunicipio as bigint), '000:000:000-00'), ':', '.') as usucpfmunicipio,
                mun.usunome AS nome_municipio,
                replace(to_char(cast(usucpfmec as bigint), '000:000:000-00'), ':', '.') as usucpfmec,
                mec.usunome AS nome_mec,
                rqatextmunicipio,
                rqaparecermec,
                rqasituacao,
                rqastatus,
                to_char(rqadtinclusaomunicipio, 'DD/MM/YYYY') AS rqadtinclusaomunicipio,
                to_char(rqadtinlcusaomec, 'DD/MM/YYYY') AS rqadtinlcusaomec
        FROM maismedicomec.recursoquestavaliacao r
        LEFT JOIN seguranca.usuario AS mun ON mun.usucpf = usucpfmunicipio
        LEFT JOIN seguranca.usuario AS mec ON mec.usucpf = usucpfmec
        WHERE rqmid = '{$rqmid}' AND rqastatus = 'A'
    ";
    $dados = $db->pegaLinha($sql);

    return $dados;
}

#SALVA DADOS MAIS MEDICOS. GETOR SUS E GETOR LOCAL "MUNICIPAL"
function salvarRecursoMaisMedicos($dados, $files) {
    global $db;

    extract($dados);

    $usucpfmunicipio    = str_replace(".", "", str_replace("-", "", $usucpfmunicipio));
    $usucpfmec          = str_replace(".", "", str_replace("-", "", $usucpfmec));
    $rcmtextmunicipio   = addslashes($rcmtextmunicipio);
    $rcmparecermec      = addslashes($rcmparecermec);
    $rcmsituacao        = $rcmsituacao == 'S' ? 't' : 'f';

    if ($rcmid == '') {

        if($tipo_parecer == 'R'){
            $sql = "
                INSERT INTO par.recursomaismedico(
                    rqmid,
                    usucpfmunicipio,
                    rcmtextmunicipio,
                    rcmdtinclusaomunicipio
                )VALUES (
                    {$rqmid},
                    '{$usucpfmunicipio}',
                    '{$rcmtextmunicipio}',
                    'NOW()'
                )RETURNING rcmid;
            ";
        }else{
            $sql = "
                INSERT INTO par.recursomaismedico(
                    rqmid,
                    usucpfmec,
                    rcmparecermec,
                    rcmsituacao,
                    rcmdtinclusaomec
                )VALUES (
                    {$rqmid},
                    '{$usucpfmec}',
                    '{$rcmparecermec}',
                    '{$rcmsituacao}',
                    'NOW()'
                )RETURNING rcmid;
            ";
        }
        $rqmid = $db->pegaUm($sql);
    } else {
        if($tipo_parecer == 'R'){
            $sql = "
                UPDATE par.recursomaismedico
                    SET rqmid                   = {$rqmid},
                        usucpfmunicipio         = '{$usucpfmunicipio}',
                        rcmtextmunicipio        = '{$rcmtextmunicipio}',
                        rcmdtinclusaomunicipio  = 'NOW()'
                WHERE rcmid = {$rcmid} RETURNING rcmid;
            ";
        }else{
            $sql = "
                UPDATE par.recursomaismedico
                    SET rqmid                   = {$rqmid},
                        usucpfmec               = '{$usucpfmec}',
                        rcmparecermec           = '{$rcmparecermec}',
                        rcmsituacao             = '{$rcmsituacao}',
                        rcmdtinclusaomec        = 'NOW()'
                WHERE rcmid = {$rcmid} RETURNING rcmid;
            ";
        }
        $rqmid = $db->pegaUm($sql);
    }

    if($files['arquivo']['name'] != ''){
        $anexado = salvarDoc($files, $dados);
    }

    if ($rqmid > 0) {
        $db->commit();
        $db->sucesso('principal/programas/feirao_programas/maisMedicosRecursoMunicipio');
    }else{
        $db->insucesso('Não foi possivel realizar a operação!', '', 'principal/programas/feirao_programas/maisMedicosRecursoMunicipio');
    }
}

function verificaExisteQuestionario(){
    global $db;

    $sql = "SELECT rqmid FROM par.respquestaomaismedico WHERE muncod = '{$_SESSION['par']['muncod']}';";
    $rqmid = $db->pegaUm($sql);

    return $rqmid;
}

#SALVA DADOS MAIS MEDICOS. GETOR SUS E GETOR LOCAL "MUNICIPAL"
function salvarRecursoQuestionarioAvaliacao($dados, $files) {
    global $db;

    extract($dados);

    $usucpfmunicipio    = str_replace(".", "", str_replace("-", "", $usucpfmunicipio));
    $usucpfmec          = str_replace(".", "", str_replace("-", "", $usucpfmec));
    $rqatextmunicipio   = addslashes($rqatextmunicipio);
    $rqaparecermec      = addslashes($rqaparecermec);
    $rqasituacao        = $rqasituacao == 'S' ? 't' : 'f';

    if ($rqaid == '') {
        if($tipo_parecer == 'R'){
            $sql = "
                INSERT INTO maismedicomec.recursoquestavaliacao(
                    rqmid,
                    usucpfmunicipio,
                    rqatextmunicipio,
                    rqadtinclusaomunicipio
                )VALUES (
                    {$rqmid},
                    '{$usucpfmunicipio}',
                    '{$rqatextmunicipio}',
                    'NOW()'
                )RETURNING rqaid;
            ";
        }else{
            $sql = "
                INSERT INTO maismedicomec.recursoquestavaliacao(
                    rqmid,
                    usucpfmec,
                    rqaparecermec,
                    rqasituacao,
                    rqadtinclusaomec
                )VALUES (
                    {$rqmid},
                    '{$usucpfmec}',
                    '{$rqaparecermec}',
                    '{$rqasituacao}',
                    'NOW()'
                )RETURNING rqaid;
            ";
        }
        $rqaid = $db->pegaUm($sql);
    } else {
        if($tipo_parecer == 'R'){
            $sql = "
                UPDATE maismedicomec.recursoquestavaliacao
                    SET rqmid                   = {$rqmid},
                        usucpfmunicipio         = '{$usucpfmunicipio}',
                        rqatextmunicipio        = '{$rqatextmunicipio}',
                        rqadtinclusaomunicipio  = 'NOW()'
                WHERE rqaid = {$rqaid} RETURNING rqaid;
            ";
        }else{
            $sql = "
                UPDATE maismedicomec.recursoquestavaliacao
                    SET rqmid                   = {$rqmid},
                        usucpfmec               = '{$usucpfmec}',
                        rqaparecermec           = '{$rqaparecermec}',
                        rqasituacao             = '{$rqasituacao}',
                        rqadtinlcusaomec        = 'NOW()'
                WHERE rqaid = {$rqaid} RETURNING rqaid;
            ";
        }
        $rqmid = $db->pegaUm($sql);
    }
//ver($sql, $dados, $files, d);
    if($files['arquivo']['name'] != ''){
        $anexado = salvarDoc($files, $dados);
    }

    if ($rqmid > 0) {        
        criaDocidMaisMedicoQuestionarioAvaliacao( $rqaid );
        
        $_SESSION['maismedico']['rqaid'] = $rqaid;
        
        $db->commit();
        $db->sucesso('principal/programas/feirao_programas/maisMedicosRecursoQuest');
    }else{
        $db->insucesso('Não foi possivel realizar a operação!', '', 'principal/programas/feirao_programas/maisMedicosRecursoQuest');
    }
}


function exibirListaDocQuesAval($tipo = NULL){
    header('Content-Type: text/html; charset=iso-8859-1');

    global $db;

    $arrayPerfil = pegaArrayPerfil($_SESSION['usucpf']);

    $where = "am.aqmsituacao = 'A'";

    if ($_SESSION['par']['rqmid']) {
        $where .= " AND am.rqmid = {$_SESSION['par']['rqmid']}";
    }

    if ($_SESSION['par']['muncod']) {
        $where .= " AND rm.muncod = '{$_SESSION['par']['muncod']}'";
    }

    if( $tipo == 'R'){
        $where .= " AND am.tpaid = 27";
    }

    if( $tipo == 'M'){
        $where .= " AND am.tpaid = 28";
    }

    if( in_array(PAR_PERFIL_PREFEITO, $arrayPerfil) || in_array(PAR_PERFIL_AVAL_INSTITUCIONAL_MM, $arrayPerfil) ){        
        $acao = "
            <img align=\"absmiddle\" src=\"/imagens/anexo.gif\" style=\"cursor: pointer\" onclick=\"downloadDocumento('|| am.arqid ||');\" title=\"Download Documento\" >
            <img align=\"absmiddle\" src=\"/imagens/excluir_01.gif\" title=\"Excluir Documento\" >
        ";
    } else {
        $acao = "
            <img align=\"absmiddle\" src=\"/imagens/anexo.gif\" style=\"cursor: pointer\" onclick=\"downloadDocumento('|| am.arqid ||');\" title=\"Download Documento\" >
            <img align=\"absmiddle\" src=\"/imagens/excluir.gif\" style=\"cursor: pointer\" onclick=\"excluirDocumento('|| am.arqid ||');\" title=\"Excluir Documento\" >
        ";
    }

    $sql = "
        SELECT  '{$acao}' AS acao,
            	ar.arqnome||'.'||ar.arqextensao AS nome_arquivo,
                ar.arqdescricao,
                ta.tpadsc,
                us.usunome
        FROM par.arquivosmunicipio am
        JOIN par.respquestaomaismedico rm ON am.rqmid = rm.rqmid
        JOIN public.arquivo ar ON ar.arqid = am.arqid
        JOIN par.tipoarquivo ta ON ta.tpaid = am.tpaid
        JOIN seguranca.usuario us ON us.usucpf = ar.usucpf

        WHERE {$where}

        ORDER BY am.aqmdtinclusao DESC
    ";
    $alinhamento = array('center','left','left','left','left');
    $tamanho = array('3%','25%','25%','15%','15%');
    $cabecalho = array('Ação', 'Nome Arquivo','Descrição','Tipo de Arquivo','Responsável');
    $db->monta_lista($sql, $cabecalho, '50', '10', '', 'center', '', '',$tamanho, $alinhamento);
}


#--------------------------------------------- FUNÇÕES WORKFLOW MODULO FORÇA DE TRABALHO CESSÃO/PRORROGAÇÃO SERVIDOR ----------------------------------#


#REGRAS WORKFLOW - BUSCA DOCID VERIFICA SE O DOCUENTO JÁ EXISTE.
function buscarDocidMaisMedicoQuestionarioAvaliacao( $rqmid ){
    global $db;

    $sql = "
            SELECT  rqaid,
                    docid
            FROM maismedicomec.recursoquestavaliacao
            WHERE rqmid = {$rqmid}
    ";
    $dados = $db->pegaLinha($sql);
    return $dados['docid'];
}

#REGRAS WORKFLOW - CRIA O DOCUMENTO CASO NÃO EXISTA.
function criaDocidMaisMedicoQuestionarioAvaliacao( $rqaid ){
    global $db;

    require_once APPRAIZ ."includes/workflow.php";

    $usucpf = $_SESSION['usucpf'];

    $existeDocid = buscarDocidMaisMedicoQuestionarioAvaliacao( $rqaid );
    if($existeDocid == ''){
        $tpdid = WF_TPDID_MAIS_MEDICOS_RECURSO_QUESTIONARIO_AVALIACAO;       
        
        if($rqaid != ''){
            $docid = wf_cadastrarDocumento($tpdid, 'Mais Médico MEC - Questionário Avaliação');
            $sql = "
                UPDATE maismedicomec.recursoquestavaliacao SET docid = {$docid} WHERE rqaid = {$rqaid};
            ";

            if( $db->executar($sql) ){
                $db->commit();
            }else{
                return false;
            }
        }
    }else{
        return false;
    }
}

#PEGA ESTADO ATUAL DO DOCUMENTO DO WORKFLOW.
function pegaEstadoAtualWorkflowMaisMedicoQuestAval($docid){
    global $db;

    if($docid) {
        $docid = (integer) $docid;
        $sql = "
            SELECT  ed.esdid, ed.esddsc
            FROM workflow.documento d
            JOIN workflow.estadodocumento AS ed ON ed.esdid = d.esdid
            WHERE d.docid = $docid
        ";
        $estado = $db->pegaLinha($sql);
        return $estado;
    } else {
        return false;
    }
}

?>