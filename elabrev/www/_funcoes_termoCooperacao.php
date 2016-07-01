<?php
//ini_set('memory_limit', -1); 

function verificaSessao() {
    if (!isset($_SESSION['elabrev']['tcpid']) || empty($_SESSION['elabrev']['tcpid'])) {
        header('Location:/elabrev/elabrev.php?modulo=inicio&acao=C');
    }
}


function pegaDocidTermo(){

    global $db;

    $sql = "SELECT
				docid
			FROM
				monitora.termocooperacao
			WHERE
				tcpid = ".$_SESSION['elabrev']['tcpid'];

    return $db->pegaUm($sql);
}

function gravarTermoAnexo($request) {
    global $db;

    extract( $request );

    $notArqid = Array();

    require_once APPRAIZ . "includes/classes/fileSimec.class.inc";

    if (is_array($anexoCod)) {
        foreach( $anexoCod as $k => $d ){
            $arpdsc[$k] = substr($arpdsc[$k], 0, 255);
            ## UPLOAD DE ARQUIVO
            $campos	= array("tcpid"	=> $tcpid, "arpdsc" => "'".$arpdsc[$k]."'", "arptipo" => "'".$arptipo[$k]."'");
            $file = new FilesSimec("arquivoprevorcamentaria", $campos, 'monitora');

            if( $_FILES["anexo_".$d]['name'] != '' && $anexoCod[$k] != '' ){
                $arquivoSalvo = $file->setUpload($arpdsc[$k], "anexo_".$d);
            }
        }
        /*if( $arqid[0] != '' ){
            $sql = "UPDATE monitora.arquivoprevorcamentaria SET arpstatus = 'I' WHERE tcpid = $tcpid AND arqid NOT IN (".implode(',',$arqid).") ;".$sql;
        }*/
    }elseif( is_array($linha) ){
        $sql = "UPDATE monitora.arquivoprevorcamentaria SET arpstatus = 'I' WHERE tcpid = $tcpid AND arqid NOT IN (".implode(',',$arqid).") ;".$sql;
        //$sql = "UPDATE monitora.arquivoprevorcamentaria SET arpstatus = 'I' WHERE tcpid = $tcpid ;".$sql;
    }

    if( $sql != '' ){
        $db->executar($sql);
        $db->commit();
    }
}

function gravarTermoCronograma( $request ){
    global $db;

    extract( $request );

    $notCrdid = Array();

    if( is_array($linha) ){
        foreach( $linha as $k => $d ){
            $crdparcela[$k]			= $crdparcela[$k] 		? $crdparcela[$k] 						: "null";
            $acaid[$k]				= $acaid[$k] 			? $acaid[$k] 							: "null";
            $crdmesliberacao[$k]	= $crdmesliberacao[$k] 	? $crdmesliberacao[$k] 					: "null";
            $crdvalor[$k]			= $crdvalor[$k] 		? formata_valor_sql($crdvalor[$k])		: "null";
            $crdmesexecucao[$k]		= $crdmesexecucao[$k] 	? $crdmesexecucao[$k] 					: "null";

            if($linha[$k] != ''){
                $notCrdid[] = $d;
                $sql .= "
					UPDATE monitora.cronogramadesembolso SET
							crdparcela 		= ".$crdparcela[$k].",
							acaid 			= ".$acaid[$k].",
							crdmesliberacao = ".$crdmesliberacao[$k].",
							crdvalor 		= ".$crdvalor[$k].",
							crdmesexecucao	= ".$crdmesexecucao[$k]."
					WHERE crdid = $d;
				";
            }elseif($linha[$k] == ''){
                $sql .= "
					INSERT INTO monitora.cronogramadesembolso(
											tcpid,
											crdparcela,
											acaid,
											crdmesliberacao,
											crdvalor,
											crdmesexecucao
									)VALUES(
											$tcpid,
											$crdparcela[$k],
											".$acaid[$k].",
											".$crdmesliberacao[$k].",
											".$crdvalor[$k].",
											".$crdmesexecucao[$k].");
				";
            }
        }
        if( $notCrdid[0] != '' ){
            $sql = "
				UPDATE monitora.cronogramadesembolso SET crdstatus = 'I' WHERE tcpid = $tcpid AND crdid NOT IN (".implode(',',$notCrdid).");
			".$sql;
        }
    }else{
        $sql = "
			UPDATE monitora.cronogramadesembolso SET crdstatus = 'I' WHERE tcpid = $tcpid;
		".$sql;
    }
    $db->executar($sql);
    $db->commit();
}

function gravarPrevisaoOrcamentaria($request) {

    global $db;

    extract( $request );
    $notProid = Array();

// 	ver($_REQUEST['ptrid_temp'], d);

    if( is_array($linha) ){
        foreach( $linha as $k => $d ){
            $ptrid[$k]		= $ptrid[$k] 	? $ptrid[$k] 						: "null";
            $pliid[$k]		= $pliid[$k] 	? $pliid[$k] 						: "null";
            $ndpid[$k]		= $ndpid[$k] 	? $ndpid[$k] 						: "null";
            $provalor[$k]	= $provalor[$k] ? formata_valor_sql($provalor[$k]) 	: "null";

            if( ($linha[$k] != '' && $ptrid[$k] !='') || ($linha[$k] != '' && $ndpid[$k] != '') ){
                $notProid[] = $d;
                $sql .= "
					UPDATE monitora.previsaoorcamentaria SET
							ptrid = ".$ptrid[$k].",
							pliid = ".$pliid[$k].",
							ndpid = ".$ndpid[$k].",
							provalor = ".$provalor[$k].",
							crdmesexecucao = ".($crdmesexecucao[$k] ? $crdmesexecucao[$k] : 'null').",
							crdmesliberacao = ".($crdmesliberacao[$k] ? $crdmesliberacao[$k] : 'null').",
							proanoreferencia = ".($proanoreferencia[$k] ? $proanoreferencia[$k] : 'null')."
					WHERE proid = $d;
				";
            }elseif($linha[$k] == ''){
                $sql .= "
					INSERT INTO monitora.previsaoorcamentaria
						(tcpid, ptrid, pliid, ndpid, provalor, crdmesexecucao, crdmesliberacao, proanoreferencia)
					VALUES
						($tcpid, ".$ptrid[$k].", ".$pliid[$k].", ".$ndpid[$k].", ".$provalor[$k].", ".($crdmesexecucao[$k] ? $crdmesexecucao[$k] : 'null').", ".($crdmesliberacao[$k] ? $crdmesliberacao[$k] : 'null').", ".($proanoreferencia[$k] ? $proanoreferencia[$k] : 'null').");
				";
            }
        }
        if( $notProid[0] != '' ){
            $sql = "
				UPDATE monitora.previsaoorcamentaria SET prostatus = 'I' WHERE tcpid = $tcpid AND proid NOT IN (".implode(',',$notProid).");
			".$sql;
        }
    }else{
        $sql = "
			UPDATE monitora.previsaoorcamentaria SET prostatus = 'I' WHERE tcpid = $tcpid ;
		".$sql;
    }

    $db->executar($sql);
    $db->commit();
}

/**
 * Verifica se natureza de despesa já foi enviado para NC (Nota de crédito)
 * @param $proid
 * @return bool
 */
function nc_ja_enviada($proid) {
    if (!$proid) {
        return false;
    }

    global $db;
    $strSQL = "select codsigefnc, tcpnumtransfsiafi, codncsiafi from elabrev.previsaoparcela where proid = $proid";
    $retorno = $db->pegaLinha($strSQL);
    if ($retorno) {
        if ((!empty($retorno['codncsiafi']) || !empty($retorno['codsigefnc'])) && !empty($retorno['tcpnumtransfsiafi'])) {
            return true;
        } else {
            return false;
        }
    }
    return false;
}

function gravarPrevisaoOrcamentariaTeste($request) {

    global $db;
    extract( $request );
    $notProid = Array();
    //ver($request, d);
    $estadoAtual = pegarEstadoAtual($_SESSION['elabrev']['tcpid']);

    $totalPrevisao = $db->pegaUm("
        select count(*) as total from monitora.previsaoorcamentaria
        where tcpid = {$_SESSION['elabrev']['tcpid']} and prostatus = 'A'
    ");

    if ($totalPrevisao == 1 && $estadoAtual == 641 && !array_key_exists('checkSaldoRemanejado', $request)) {
        $db->executar("update monitora.previsaoorcamentaria set crdmesexecucao = {$crdmesexecucao[0]} where proid = {$linha[0]}");
        $result = $db->commit();
        if ($result) {
            return array(
                'msg' => 'Prazo de cumprimento do objeto, alterado com sucesso!',
                'erro' => false
            );
            die;
        }
    }

    /**
     * Se estiver usando saldo remanejado, verifica se saldo > notaNaturezaCredito criada
     */
    if (array_key_exists('checkSaldoRemanejado', $request)) {
        $saldoRemanejado = existeSaldoRemanejado($_SESSION['elabrev']['tcpid'], true);
        $formatedValues = array();
        foreach ($request['checkSaldoRemanejado'] as $i => $v) {
            $_valor = str_replace('.', '', $request['provalor'][$i]);
            $_valor = str_replace(',', '.', $_valor);
            array_push($formatedValues, $_valor);
        }

        $total = array_sum($formatedValues);

        if ($saldoRemanejado < $total) {
            return array(
                'msg' => 'O(s) valor(es) inserido(s) é(são) maior(es) do que o saldo remanejado disponível!',
                'erro' => true
            );
            die;
        }
    }

    if (is_null($linha) && is_null($ptrid) && is_null($ndpid) && is_null($provalor) && is_array($crdmesexecucao)) {
        $updateStr = "update monitora.previsaoorcamentaria set crdmesexecucao = {$crdmesexecucao[0]}
                      where tcpid = {$tcpid} and crdmesexecucao is not null";
        //ver($updateStr, d);
        $db->executar($updateStr);
        $result = $db->commit();
        if ($result) {
            return array(
                'msg' => 'Prazo de cumprimento do objeto, alterado com sucesso!',
                'erro' => false
            );
            die;
        }
    }

    if (is_array($linha)) {

        $_crdmesexecucao = $crdmesexecucao[0];

        foreach ($linha as $k => $d) {
            $ptrid[$k]	   = $ptrid[$k]    ? $ptrid[$k] 					  : 'null';
            $pliid[$k]	   = $pliid[$k]    ? $pliid[$k] 					  : 'null';
            $ndpid[$k]	   = $ndpid[$k]    ? $ndpid[$k] 					  : 'null';
            $provalor[$k]  = $provalor[$k] ? formata_valor_sql($provalor[$k]) : 'null';

            if (($linha[$k] != '' && $ptrid[$k] !='') || ($linha[$k] != '' && $ndpid[$k] != '')) {

                /**
                 * Regra:
                 * Quando o termo já passou por alguma retirada de valores,
                 * o mesmo não pode ser atualizado editando o campo,
                 * por conta do relacionamento criado na hora do remanejamento de crédito
                 * tabela: elabrev.creditoremanejado
                 */
                /*if ($db->pegaUm("SELECT proid FROM elabrev.creditoremanejado where proid = {$linha[$k]}")) {
                    ver($linha[$k], d);
                    continue;
                }*/

                if (!teste_superUser()) {
                    /**
                     * verifica se a nd já foi enviada
                     */
                    if (nc_ja_enviada($d)) {
                        continue;
                    }
                }

                if ($db->pegaUm("select true from monitora.previsaoorcamentaria where proid = {$linha[$k]} and creditoremanejado = 't'") ||
                    $db->pegaUm("SELECT proid FROM elabrev.creditoremanejado where proid = {$linha[$k]}")) {
                    $sql .= "
                        UPDATE monitora.previsaoorcamentaria SET
                                ptrid = ".$ptrid[$k].",
                                pliid = ".$pliid[$k].",
                                ndpid = ".$ndpid[$k].",
                                crdmesexecucao = ".($_crdmesexecucao ? $_crdmesexecucao : 'null').",
                                crdmesliberacao = ".($crdmesliberacao[$k] ? $crdmesliberacao[$k] : 'null').",
                                proanoreferencia = ".($proanoreferencia[$k] ? $proanoreferencia[$k] : 'null')."
                        WHERE proid = $d;
                    ";
                } else {
                    $sql .= "
                        UPDATE monitora.previsaoorcamentaria SET
                                ptrid = ".$ptrid[$k].",
                                pliid = ".$pliid[$k].",
                                ndpid = ".$ndpid[$k].",
                                provalor = ".$provalor[$k].",
                                crdmesexecucao = ".($_crdmesexecucao ? $_crdmesexecucao : 'null').",
                                crdmesliberacao = ".($crdmesliberacao[$k] ? $crdmesliberacao[$k] : 'null').",
                                proanoreferencia = ".($proanoreferencia[$k] ? $proanoreferencia[$k] : 'null')."
                        WHERE proid = $d;
                    ";
                }

            } elseif($linha[$k] == '') {

                //Se a natureza utilizar crédito remanejado
                if (!empty($checkSaldoRemanejado[$k])) {
                    $sql .= "
                        INSERT INTO monitora.previsaoorcamentaria
                            (tcpid, ptrid, pliid, ndpid, provalor, crdmesexecucao, crdmesliberacao, proanoreferencia, creditoremanejado)
                        VALUES
                            ($tcpid, ".$ptrid[$k].", ".$pliid[$k].", ".$ndpid[$k].", ".$provalor[$k].", ".($_crdmesexecucao ? $_crdmesexecucao : 'null').", ".($crdmesliberacao[$k] ? $crdmesliberacao[$k] : 'null').", ".($proanoreferencia[$k] ? $proanoreferencia[$k] : 'null').", TRUE);
                    ";
                } else {
                    $sql .= "
                        INSERT INTO monitora.previsaoorcamentaria
                            (tcpid, ptrid, pliid, ndpid, provalor, crdmesexecucao, crdmesliberacao, proanoreferencia)
                        VALUES
                            ($tcpid, ".$ptrid[$k].", ".$pliid[$k].", ".$ndpid[$k].", ".$provalor[$k].", ".($_crdmesexecucao ? $_crdmesexecucao : 'null').", ".($crdmesliberacao[$k] ? $crdmesliberacao[$k] : 'null').", ".($proanoreferencia[$k] ? $proanoreferencia[$k] : 'null').");
                    ";
                }
            }
        }
    }

    if (!empty($sql)) {
        //ver($sql, d);

        $db->executar($sql);
        $db->commit();
        $message = 'Dados gravados!';
    } else {
        $message = 'Nenhuma alteração foi feita.';
    }

    return array(
        'msg' => $message,
        'erro' => false
    );
    die;
}

function gravarTermoDescentralizacao( $request ){
    global $db;

    extract( $request );

    if( $tcpid != '' ){

        if( $tcptipoemenda == 'S' && $emeid ){
            $ungcod = $_SESSION['elabrev']['ungcodlista'];

            $db->executar("DELETE FROM emenda.emendatermocooperacao WHERE tcpid = $tcpid");
            //foreach ($emeid as $emenda) {
            $sql = "INSERT INTO emenda.emendatermocooperacao(emeid, tcpid, ungcod)
						VALUES ($emeid, $tcpid, '$ungcod')";
            $db->executar($sql);
            //}
        }

        $tcpdscobjetoidentificacao 	= $tcpdscobjetoidentificacao 	? "'".$tcpdscobjetoidentificacao."'" 	: "null";
        $tcpobjetivoobjeto 		  	= $tcpobjetivoobjeto 			? "'".$tcpobjetivoobjeto."'" 			: "null";
        $tcpobjetojustificativa	  	= $tcpobjetojustificativa		? "'".$tcpobjetojustificativa."'" 		: "null";
        //$ungcoddes 					= $ungcoddes 					? $ungcoddes 							: "null";
        $ungcodrec 					= $ungcodrec 					? $ungcodrec 							: "null";
        $tcptipoemenda	  			= $tcptipoemenda				? "'".$tcptipoemenda."'" 				: "null";

        $sql = "UPDATE monitora.termocooperacao SET
					tcpdscobjetoidentificacao 	= $tcpdscobjetoidentificacao,
					tcpobjetivoobjeto 			= $tcpobjetivoobjeto,
					tcpobjetojustificativa		= $tcpobjetojustificativa,
					--ungcoddes = $ungcoddes,
					ungcodgestaorecebedora 		= $ungcodrec,
					tcptipoemenda				= $tcptipoemenda
				WHERE
					tcpid = $tcpid";

        $db->executar($sql);
        $db->commit();
    }
}

function gravarTermoConcedente($request) {
    global $db;
    extract($request);

    if (!empty($tcpid)) {

        if ($codpoliticafnde) {
            $arCodpoliticafnde = explode('_', $codpoliticafnde);
            if ($arCodpoliticafnde[1] == 'ungcod') {
                $ungcodpoliticafnde = $arCodpoliticafnde[0];
            } else if($arCodpoliticafnde[1] == 'dircod') {
                $dircodpoliticafnde = $arCodpoliticafnde[0];
            }
        }

        #Se o concedente for FNDE gere numero de processo
        $sql = "UPDATE monitora.termocooperacao
				SET	usucpfconcedente ='".$usucpf."',
					ungcodconcedente = '".$ungcod."',
					ungcodpoliticafnde = ".($ungcodpoliticafnde ? "'".$ungcodpoliticafnde."'" : 'null').",
					dircodpoliticafnde = ".($dircodpoliticafnde ? "'".$dircodpoliticafnde."'" : 'null')."
				WHERE tcpid = ".$tcpid."
				RETURNING tcpid";

        $db->executar($sql);
        $db->commit();
    }
    return $tcpid;
}

function gerarProcessoFNDE($request) {
    include_once APPRAIZ.'includes/classes/ProcessoFNDE.class.php';
    global $db;

    $login = $_POST['login'];
    $senha = $_POST['senha'];

    $arrProc = $db->pegaLinha("
      select ug.ungcnpj, tcpnumprocessofnde from monitora.termocooperacao tc
      inner join public.unidadegestora ug on ug.ungcod = tc.ungcodproponente
      where tc.tcpid = {$request['tcpid']}
    ");

    $ungcnpj 			= $arrProc['ungcnpj'];
    $tcpnumprocessofnde = $arrProc['tcpnumprocessofnde'];

    if( $ungcnpj && empty($tcpnumprocessofnde)) {
        $arrParamProcesso = array(
            'nu_cpf' 	=> '',
            'co_assunto'=> '051.21',
            'ds_resumo' => '051.21 - DESCENTRALIZAÇÃO DE RECURSOS',
            'nu_cnpj' 	=> $ungcnpj
        );

        $obProcesso = new ProcessoFNDE( $login, $senha );
        $arrProcesso = $obProcesso->gerarProcessoFNDE( $arrParamProcesso );
        if($arrProcesso){
            $numeroProcesso = $arrProcesso['processo'];
            $nu_banco 		= $arrProcesso['banco'];
            $nu_agencia		= $arrProcesso['agencia'];

            $sql = "UPDATE monitora.termocooperacao SET
						tcpbancofnde = '{$nu_banco}',
						tcpagenciafnde = '{$nu_agencia}',
						tcpnumprocessofnde = '{$numeroProcesso}'
					WHERE tcpid = {$request['tcpid']}";

            $db->executar($sql);
            $retorno = $db->commit();
            if( $request['tipo'] ){
                echo "<script>
						alert('Processo Gerado com Sucesso.');
						window.location = 'elabrev.php?modulo=principal/termoCooperacao/cadTermoCooperacao&acao=A&tcpid={$request['tcpid']}&aba={$request['aba']}';
				  </script>";
                exit();
            } else {
                return $retorno;
            }
        } else {
            if( $request['tipo'] ){
                echo "<script>
						alert('Falha ao Gerar o Processo.');
						window.location = 'elabrev.php?modulo=principal/termoCooperacao/cadTermoCooperacao&acao=A&tcpid={$request['tcpid']}&aba={$request['aba']}';
				  </script>";
                exit();
            } else {
                return 'erro';
            }
        }
    } else {
        if( $request['tipo'] ){
            echo "<script>
					alert('Falha ao Gerar o Processo. CNPJ da Unidade Gestora não Encontrado.');
					window.location = 'elabrev.php?modulo=principal/termoCooperacao/cadTermoCooperacao&acao=A&tcpid={$request['tcpid']}&aba={$request['aba']}';
			  </script>";
            exit();
        }
    }
}

function gravarTermoProponente( $request )
{
    global $db;

    extract( $request );

    //verifica se o proponente possui ug
    if(!$ungcod){
        echo '<script>
				alert("Usuário sem Unidade Gestora. Entre em contato com o Gestor do sistema.");
				history.back();
			  </script>';
        die();
    }

    //verifica se existe representante legal
    $sql = "select cpf from elabrev.representantelegal where ug = '$ungcod'";
    $rep = $db->pegaUm($sql);
    if($rep==0){
        echo '<script>
				alert("Unidade Gestora sem Representante Legal. Entre em contato com o Gestor do sistema.");
				history.back();
			  </script>';
        die();
    }

    if( $tcpid != ''){
        $sql = "UPDATE monitora.termocooperacao SET usucpfproponente = '$usucpf' ".($entid ? ", entid = {$entid}" : '' )." WHERE tcpid = $tcpid and ungcodproponente = '".$ungcod."' RETURNING tcpid";
    }else{
        require_once APPRAIZ . 'includes/workflow.php';
        $docid = wf_cadastrarDocumento(WF_TPDID_DESCENTRALIZACAO, 'Termo Cooperacao');
        $sql = "INSERT INTO monitora.termocooperacao(docid, ungcodproponente, usucpfproponente, entid)VALUES(".$docid.",".$ungcod.", '".$usucpf."', ".($entid ? $entid : 'null').") RETURNING tcpid";
    }

    if($sql != ''){
        $tcpid = $db->pegaUm($sql);
        $db->commit();
    }
    return $tcpid;
}

function gravarParecerTecnico( $request ){
    global $db;

    extract( $request );

    if( $tcpid != ''){

        $sql = "UPDATE monitora.termocooperacao SET
					tcpparecertecnico 		= '{$tcpparecertecnico}',
					tcpconsidentproponente 	= '{$tcpconsidentproponente}',
					tcpconsidproposta 		= '{$tcpconsidproposta}',
					tcpconsidobjeto 		= '{$tcpconsidobjeto}',
					tcpconsidobjetivo 		= '{$tcpconsidobjetivo}',
					tcpconsidjustificativa 	= '{$tcpconsidjustificativa}',
					tcpconsidvalores 		= '{$tcpconsidvalores}',
					tcpconsidcabiveis 		= '{$tcpconsidcabiveis}',
					tcpusucpfparecer		= '".str_replace(array('.','-',','), '', $_SESSION['usucpf'])."'
				WHERE tcpid = ".$tcpid." RETURNING tcpid";

        $db->executar($sql);
        $db->commit();
    }

    return $tcpid;
}


function atualizaDadosUg( $dados ){
    global $db;

    extract($dados);

    $ungcnpj 		= $ungcnpj ? "'".$ungcnpj."'" : 'null';
    $ungendereco 	= $ungendereco ? "'".$ungendereco."'" : 'null';
    $ungbairro 		= $ungbairro ? "'".$ungbairro."'" : 'null';
    $muncod 		= $muncod ? "'".$muncod."'" : 'null';
    $ungcep 		= $ungcep ? "'".$ungcep."'" : 'null';
    $ungfone 		= $ungfone ? "'".$ungfone."'" : 'null';
    $ungemail 		= $ungemail ? "'".$ungemail."'" : 'null';

    $sql = "UPDATE public.unidadegestora SET
				/*ungcnpj 	= $ungcnpj,*/
				ungendereco = $ungendereco,
				ungbairro 	= $ungbairro,
				muncod 		= $muncod,
				ungcep 		= $ungcep,
				ungfone 	= $ungfone,
				ungemail 	= $ungemail
			WHERE ungcod = '$ungcod'";
    $db->executar($sql);
    $db->commit();
}


function removeRespUg($ungcod, $entidadesFicam){
    global $db;

    // remove as referencias de entidades que foram excluidas da lista pelo botao excluir
    $sql = 		"update monitora.termocooperacao  set unridproponente = null
				where unridproponente in (
				select unrid from monitora.unidaderepresentante
				where ungcod = '{$ungcod}' and entid not in (".$entidadesFicam."))";

    $db->executar($sql);

    // remove as referencias de entidades que foram excluidas da lista pelo botao excluir
    $sql = 		"update monitora.termocooperacao  set unridconcedente = null
	where unridconcedente in (
	select unrid from monitora.unidaderepresentante
	where ungcod = '{$ungcod}' and entid not in (".$entidadesFicam."))";
    $db->executar($sql);

    //remove as entidades que foram excluidas da lista pelo botao excluir
    $sql = "delete from monitora.unidaderepresentante
			where ungcod = '{$ungcod}' and entid not in (".$entidadesFicam.")";

    $db->executar($sql);
}

function gravaRespUg( $dados ){
    global $db;

    extract($dados);

    //entidades e armazenado num campo hiddem com valores separados por virgula
    $entidades =  trim($entidades,',' );
    $arrEntidades = explode(',',$entidades);

    if ($entidades){
        if( $ungcod ){
            removeRespUg($ungcod, $entidades);
            foreach ($arrEntidades as $ent){
                $sql = "select * from monitora.unidaderepresentante where ungcod = '{$ungcod}' and entid = {$ent}";
                $registro = $db->pegaUm($sql);

                if ($registro['conta'] == 0){
                    $sql = "INSERT INTO monitora.unidaderepresentante(ungcod,entid)
						VALUES($ungcod,$ent)";
                    $db->executar($sql);
                }
            }
        }
    }
}

function gravarTramiteRelatorio( $dados ){
    global $db;

    //if ( $dados['tcpobsrelatorio']){
    $sql = "update monitora.termocooperacao set tcpobsrelatorio ='".$_REQUEST['tcpobsrelatorio']."' where tcpid = ".$_SESSION['elabrev']['tcpid'];
    $db->executar($sql);
    $db->commit();
    //}
}


function salvarTermo( $request ){
    global $db;

    switch ($_REQUEST['aba']){
        case 'proponente':

            atualizaDadosUg( $request );
            gravaRespUg( $request );
            extract($request);
            $tcpid = gravarTermoProponente( $request );
            echo "<script>
					alert('Dados gravados.');
					window.location = 'elabrev.php?modulo=principal/termoCooperacao/cadTermoCooperacao&acao=A&tcpid=$tcpid&aba=$acaoAba';
				  </script>";
            break;
        case 'concedente':
            gravaRespUg( $request );
            extract($request);
            $tcpid = gravarTermoConcedente( $request );
            if( $request['ungcod'] == '153173' ){
                //$retorno = gerarProcessoFNDE($request);
                /*if( $retorno ){
                    echo "<script>
                                alert('Dados gravados.');
                                window.location = 'elabrev.php?modulo=principal/termoCooperacao/cadTermoCooperacao&acao=A&tcpid=$tcpid&aba=$acaoAba';
                          </script>";
                } else {
                    echo "<script>
                                alert('Erro ao Gerar o Processo. Entre em contato com a área responsável.');
                                window.location = 'elabrev.php?modulo=principal/termoCooperacao/cadTermoCooperacao&acao=A&tcpid=$tcpid&aba=$acaoAba';
                          </script>";
                }*/
            }
            echo "<script>
					alert('Dados gravados.');
					window.location = 'elabrev.php?modulo=principal/termoCooperacao/cadTermoCooperacao&acao=A&tcpid=$tcpid&aba=$acaoAba';
				  </script>";
            break;
        case 'descentralizacao':
            gravarTermoDescentralizacao( $request );
            extract($request);
            echo "<script>
					alert('Dados gravados.');
					window.location = 'elabrev.php?modulo=principal/termoCooperacao/cadTermoCooperacao&acao=A&tcpid=$tcpid&aba=$acaoAba';
				  </script>";
            break;
        case 'previsao':

            /*
            gravarPrevisaoOrcamentaria($request);
            extract($request);
            die("<script>
                    alert('Dados gravados.');
                    window.location = 'elabrev.php?modulo=principal/termoCooperacao/cadTermoCooperacao&acao=A&tcpid=$tcpid&aba=$acaoAba';
                </script>");
            */

            $return = gravarPrevisaoOrcamentariaTeste( $request );
            extract($request);

            if ($return['erro']) {
                echo "<script>
                    alert('".$return['msg']."');
                </script>";
            } else {
                echo "<script>
                    alert('".$return['msg']."');
                    window.location = '/elabrev/elabrev.php?modulo=principal/termoCooperacao/cadTermoCooperacao&acao=A&tcpid=$tcpid&aba=$acaoAba';
                </script>";
            }

            break;
        case 'cronograma':
            gravarTermoCronograma( $request );
            extract($request);
            echo "<script>
					alert('Dados gravados.');
					window.location = 'elabrev.php?modulo=principal/termoCooperacao/cadTermoCooperacao&acao=A&tcpid=$tcpid&aba=$acaoAba';
				</script>";
            break;
        case 'parecertecnico':
            gravarParecerTecnico( $request );
            gravarTermoAnexo( $request );
            extract($request);
            echo "<script>
					alert('Dados gravados.');
					window.location = 'elabrev.php?modulo=principal/termoCooperacao/cadTermoCooperacao&acao=A&tcpid=$tcpid&aba=$acaoAba';
				</script>";
            break;
        case 'anexo':
            gravarTermoAnexo( $request );
            extract($request);
            echo "<script>
					alert('Dados gravados.');
					window.location = 'elabrev.php?modulo=principal/termoCooperacao/cadTermoCooperacao&acao=A&tcpid=$tcpid&aba=$acaoAba';
				</script>";
            break;
        case 'tramite':
            gravarTermoAnexo( $request );
            gravarTramiteRelatorio( $request );
            extract($request);
            echo "<script>
					alert('Dados gravados.');
					window.location = 'elabrev.php?modulo=principal/termoCooperacao/cadTermoCooperacao&acao=A&tcpid=$tcpid&aba=$acaoAba';
				</script>";
            break;
    }
}

function litaResponsavelUgProp($ungcod)
{
    global $db;

    if (is_array($ungcod) && isset($ungcod['ungcod'])) {
        $ungcod = $ungcod['ungcod'];
    }

    if ($_SESSION['elabrev']['tcpid']) {

        $sql = "select
				ug,
				cpf as usucpf,
				nome as usunome,
				email as usuemail,
				status as rpustatus
			from
				elabrev.representantelegal
			where
				status = 'A'
			".(is_array($ungcod) ? " AND ug IN ('".implode("','", $ungcod)."') " : " AND ug = '{$ungcod}' ");
        $retorno = $db->pegaLinha($sql);

        if ($retorno) {
            return $retorno;
        } else {
            $sql = "
				select
					hd.hstid,
					us.usucpf,
					us.usunome,
					us.usuemail
				from workflow.historicodocumento hd
					inner join workflow.acaoestadodoc ac on
						ac.aedid = hd.aedid
					inner join workflow.estadodocumento ed on
						ed.esdid = ac.esdidorigem
					inner join seguranca.usuario us on
						us.usucpf = hd.usucpf
					left join workflow.comentariodocumento cd on
						cd.hstid = hd.hstid
				where
					hd.docid = (select docid from monitora.termocooperacao where tcpid = {$_SESSION['elabrev']['tcpid']})
				and ac.esdidorigem = " . EM_APROVACAO_DA_REITORIA . " -- Representante legal do proponente / Aguardando aprovacao do proponente
                                and ac.esdiddestino = " . EM_ANALISE_DA_SECRETARIA . " -- Em analise do Gabinete da secretaria/autarquia
			";
            $rsRepProp = $db->pegaLinha($sql);

            $sql = "
				select
					hd.hstid,
					us.usucpf,
					us.usunome,
					us.usuemail
				from workflow.historicodocumento hd
					inner join workflow.acaoestadodoc ac on
						ac.aedid = hd.aedid
					inner join workflow.estadodocumento ed on
						ed.esdid = ac.esdidorigem
					inner join seguranca.usuario us on
						us.usucpf = hd.usucpf
					left join workflow.comentariodocumento cd on
						cd.hstid = hd.hstid
				where
					hd.docid = (select docid from monitora.termocooperacao where tcpid = {$_SESSION['elabrev']['tcpid']})
				and ac.esdidorigem = " . EM_EXECUCAO . " -- Em execução
				and ac.esdiddestino = " . ALTERAR_TERMO_COOPERACAO . " -- Solicitação de alteração
			";
            $rsRepAlt = $db->pegaLinha($sql);

            if ($rsRepProp['hstid'] > $rsRepAlt['hstid'] || empty($rsRepProp['hstid'])) {
                if ($rsRepProp) {
                    return $rsRepProp;
                }
            }
        }
    }

    $sql = "select
                    ug,
                    cpf as usucpf,
                    nome as usunome,
                    email as usuemail,
                    status as rpustatus
                from
                    elabrev.representantelegal
                where
                    status = 'A'
                ".(is_array($ungcod) ? " AND ug IN ('".implode("','", $ungcod)."') " : " AND ug = '{$ungcod}' ");
    //ver($sql, d);
    return $db->pegaLinha($sql);
}

function litaResponsavelUgConc($ungcod)
{
    global $db;

    if(is_array($ungcod) && isset($ungcod['ungcod'])) $ungcod = $ungcod['ungcod'];

    ?>

    <?php if(empty($ungcod)): ?>

    <tr align="center">
        <td align="center">
            <b style="color:#cc0000;">Não há Unidade Gestora associada.</b>
        </td>
    </tr>

<?php else: ?>

<?php

// 		if($_SESSION['elabrev']['tcpid']){
// 			$sql = "
// 				select
// 					us.usucpf,
// 					us.usunome,
// 					us.usuemail
// 				from workflow.historicodocumento hd
// 				inner join workflow.acaoestadodoc ac on
// 					ac.aedid = hd.aedid
// 				inner join workflow.estadodocumento ed on
// 					ed.esdid = ac.esdidorigem
// 				inner join seguranca.usuario us on
// 					us.usucpf = hd.usucpf
// 				left join workflow.comentariodocumento cd on
// 					cd.hstid = hd.hstid
// 				where
// 					hd.docid = (select docid from monitora.termocooperacao where tcpid = {$_SESSION['elabrev']['tcpid']})
// 				and
// 					ed.esdid = 635
// 			";

// 			$rsRep = $db->pegaLinha($sql);
// 		}

    $resp = null;
    if($_SESSION['elabrev']['tcpid']){

        $sql = "select
                    ug,
                    cpf as usucpf,
                    nome as usunome,
                    email as usuemail,
                    status as rpustatus,
                    funcao as usufuncao
                from
                    elabrev.representantelegal
                where
                    1=1 ".(is_array($ungcod) ? " AND ug IN ('".impode("','", $ungcod)."') " : " AND ug = '{$ungcod}' ");
        $resp = $db->pegaLinha($sql);
    }

    if (!$resp) {
        $sql = "
            select
                us.usucpf,
                us.usunome,
                us.usuemail
            from workflow.historicodocumento hd
                inner join workflow.acaoestadodoc ac on
                    ac.aedid = hd.aedid
                inner join workflow.estadodocumento ed on
                    ed.esdid = ac.esdidorigem
                inner join seguranca.usuario us on
                    us.usucpf = hd.usucpf
                left join workflow.comentariodocumento cd on
                    cd.hstid = hd.hstid
            where
                hd.docid = (select docid from monitora.termocooperacao where tcpid = {$_SESSION['elabrev']['tcpid']})
            and hd.aedid in (1612, 2442)
        ";
        $resp = $db->pegaLinha($sql);
    }

    ?>

    <?php if( $resp ): ?>
        <tr>
            <td id="tr_usucpf" value="1" class="subtitulocentro" width="15%">CPF</td>
            <td class="subtitulocentro" width="15%">Nome</td>
            <td class="subtitulocentro" width="30%">Perfil</td>
            <td class="subtitulocentro" width="55%">Função</td>
            <td class="subtitulocentro" width="55%">E-mail</td>
            <td class="subtitulocentro" width="55%">Status</td>
        </tr>
        <tr>
            <td align="center">
                <? echo formatar_cpf($resp['usucpf']); ?>
                <input type="hidden" id="usucpf" name="usucpf" value="<?=$resp['usucpf']?>" />
            </td>
            <td align="center"> <?=$resp['usunome'] ?> </td>
            <td align="center"> Representante Legal </td>
            <td align="center"> <?=$resp['usufuncao'] ?> </td>
            <td align="center"> <?=$resp['usuemail'] ?> </td>
            <td align="center"> <?=$resp['rpustatus'] ?> </td>
        </tr>
    <?php else: ?>
        <tr align="center">
            <td align="center">
                <b style="color:#cc0000;">Não há Perfil associado.</b>
            </td>
        </tr>
    <?php endif; ?>

<?php endif; ?>

<?php
}


function novaLinhaAnexo( $request ){

    ?>
    <tr id="tr_<?=$request['cod'] ?>">
        <td align="center">
            <img border="0" id="" class="excluirLinha" title="Excluir" style="cursor: pointer" src="/imagens/excluir.gif">
            <input type="hidden" name="linha[]" value="<?=$request['cod'] ?>"/>
        </td>
        <td align="center">
            <input type="hidden" name="anexoCod[]" value="<?=$request['cod'] ?>"/>
            <input type="file" name="anexo_<?=$request['cod'] ?>"/>
        </td>
        <td align="center">
            <?=campo_textarea( 'arpdsc[]', 'N', 'S', '', 150, 2, 255, '', '', '', 'id="arpdsc"', '', $dados['arpdsc']) ?>
        </td>
    </tr>
<?php
}


function novaLinhaAnexo2( $request ){

    ?>
    <tr id="tr_<?=$request['cod'] ?>">
        <td align="center">
            <img border="0" id="<?=$request['cod'] ?>" class="excluirLinha" title="Excluir" style="cursor: pointer" src="/imagens/excluir.gif">
            <input type="hidden" name="linha[]" value="<?=$request['cod'] ?>"/>
        </td>
        <td align="left">
            <input type="hidden" name="anexoCod[]" value="<?=$request['cod'] ?>"/>
            <input type="file" name="anexo_<?=$request['cod'] ?>"/>
        </td>
    </tr>
<?php
}


function novaLinhaCronograma( $request ){
    global $db;

    $perfis = array();
    $perfis = pegaPerfilGeral($_SESSION['usucpf'], $_SESSION['sisid']);

    #PERFIL UO E SECRETARIA - NIVEL - 5 E 2
    if( in_array(PERFIL_SECRETARIA, $perfis) || in_array(UO_CONSULTA_ORCAMENTO, $perfis) || in_array(UO_COORDENADOR_EQUIPE_TECNICA, $perfis ) || in_array(UO_EQUIPE_TECNICA, $perfis) ){
        $habilita_Parc = 'S';
        $habilita_Acao = 'N';
    }

    #PERFIL REITOR E SECRETARIO - NIVEL - 4 E 3
    $habilita_botao = 'S';
    if( in_array(PERFIL_REITOR, $perfis) || in_array(PERFIL_SECRETARIO, $perfis) || in_array(PERFIL_SECRETARIA, $perfis) ){
        $habilita_Parc = 'N';
        $habilita_Acao = 'N';
    }

    #PERFIL SUPER SUSÁRIO NIVEL - 1
    if( in_array(PERFIL_SUPER_USUARIO, $perfis) || in_array(PERFIL_CGSO, $perfis) || in_array(PERFIL_DIRETORIA, $perfis) ){
        $habilita_Parc = 'S';
        $habilita_Acao = 'S';
    }
    ?>
    <tr id="tr_<?=$request['cod'] ?>">
        <td align="center">
            <img border="0" id="<?=$request['cod'] ?>" class="excluirLinha" title="Excluir" style="cursor: pointer" src="/imagens/excluir.gif">
            <input type="hidden" name="linha[]" value=""/>
        </td>
        <?php if($habilita_Parc == 'S'){?>
            <td align="center">
                <?=campo_texto('crdparcela[]','S','S','',3,2,'[.###],##','', '', '', '', 'id="crdparcela"', '', $dados['crdparcela']);?>
            </td>
        <?php }else{?>
            <td align="center"> - </td>
        <?php }?>

        <?php if($habilita_Acao == 'S'){?>
            <td align="center">
                <?php
                $sql="
			SELECT 	DISTINCT a.acaid as codigo,
					a.acacod||'-'|| coalesce(a.acatitulo, '_') as descricao
			FROM monitora.ptres p
			INNER JOIN monitora.acao a ON a.acaid = p.acaid
			INNER JOIN public.unidadegestora u ON u.unicod = p.unicod
			INNER JOIN monitora.pi_planointernoptres pt on pt.ptrid = p.ptrid
			--WHERE ptrano = '".$_SESSION['exercicio']."' AND
			WHERE
				  p.ptrid in ( select ptrid from monitora.previsaoorcamentaria where tcpid = ".$_SESSION['elabrev']['tcpid']." )
			ORDER BY 1;
		";
                $db->monta_combo('acaid[]',$sql,'S','Selecione...','',$opc,'','100','S', 'acaid', '', '', $title= null);
                ?>
            </td>
        <?php }else{?>
            <td align="center"> - </td>
        <?php }?>

        <?php if($habilita_Acao == 'S'){?>
            <td align="center">
                <?php
                $sql = Array(Array('codigo'=>1,'descricao'=>'Janeiro'),
                    Array('codigo'=>2,'descricao'=>'Fevereiro'),
                    Array('codigo'=>3,'descricao'=>'Março'),
                    Array('codigo'=>4,'descricao'=>'Abril'),
                    Array('codigo'=>5,'descricao'=>'Maio'),
                    Array('codigo'=>6,'descricao'=>'Junho'),
                    Array('codigo'=>7,'descricao'=>'Julho'),
                    Array('codigo'=>8,'descricao'=>'Agosto'),
                    Array('codigo'=>9,'descricao'=>'Setembro'),
                    Array('codigo'=>10,'descricao'=>'Outubro'),
                    Array('codigo'=>11,'descricao'=>'Novembro'),
                    Array('codigo'=>12,'descricao'=>'Dezembro')
                );
                //	$db->monta_combo('crdmesliberacao[]',$sql,'S','Selecione...','',$opc,'','100','S', 'crdmesliberacao', '', '', $title= null);
                ?>
            </td>
        <?php }else{?>
            <td align="center"> - </td>
        <?php }?>

        <?php if($habilita_Parc == 'S'){?>
            <td align="center"><?=campo_texto('crdvalor[]','S','S','',30,21,'[.###],##','', '', '', '', 'id="crdvalor"', '', $dados['crdvalor']);?></td>
        <?php }else{?>
            <td align="center"> - </td>
        <?php }?>

        <?php if($habilita_Parc == 'S'){?>
            <td align="center">
                <?php
                //			$sql = array();
                //			for($i = 1; $i <= 50; $i++){
                //				$sql[$i-1]['codigo'] = $i;
                //				$sql[$i-1]['descricao'] = $i.' Mês(s)';
                //			}
                //			array_push($sql, $sql);
                //			$db->monta_combo('crdmesexecucao[]', $sql,'S','Selecione...','',$opc,'','100','S', 'crdmesexecucao', '', $dado['crdmesexecucao'], $title= null);
                ?>
            </td>
        <?php }else{?>
            <td align="center"> - </td>
        <?php }?>

    </tr>
<?php
}

function atualizaPI( $request ){
    global $db;

    If($_SESSION['elabrev']['tcpid'] != ''){
        $estadoAtual = pegarEstadoAtual( $_SESSION['elabrev']['tcpid'] );
    }

    $perfis = array();
    $perfis = pegaPerfilGeral($_SESSION['usucpf'], $_SESSION['sisid']);

    #PERFIL UO E SECRETARIA - NIVEL - 5 E 2
    if( in_array(PERFIL_SECRETARIA, $perfis) || in_array(UO_CONSULTA_ORCAMENTO, $perfis) || in_array(UO_COORDENADOR_EQUIPE_TECNICA, $perfis ) || in_array(UO_EQUIPE_TECNICA, $perfis) ){
        $habilita_Plano = 'N';
    }

    #PERFIL REITOR E SECRETARIO - NIVEL - 4 E 3
    if( in_array(PERFIL_REITOR, $perfis) || in_array(PERFIL_SECRETARIO, $perfis) || in_array(PERFIL_SECRETARIA, $perfis) ){
        $habilita_Plano = 'N';
    }

    #PERFIL SUPER SUSÁRIO NIVEL - 1
    if( in_array(PERFIL_SUPER_USUARIO, $perfis) || in_array(PERFIL_CGSO, $perfis) || in_array(PERFIL_DIRETORIA, $perfis) ){
        $habilita_Plano = 'S';
    }

    if( $estadoAtual == ALTERAR_TERMO_COOPERACAO && ( in_array(PERFIL_SUPER_USUARIO, $perfis) || in_array(UO_EQUIPE_TECNICA, $perfis) ) ){
        $habilita_Plano = 'S';
    }

    if( testaRespUGConcedente() && $_SESSION['elabrev']['termo']['aba'] == 'previsao' && $estadoAtual == EM_ANALISE_OU_PENDENTE){
        $habilita_Plano = 'S';
    }

    if($estadoAtual == EM_ANALISE_OU_PENDENTE && ( in_array(PERFIL_COORDENADOR_SEC, $perfis) || in_array(PERFIL_SUPER_USUARIO, $perfis) ) ){
        $habilita_Plano = 'S';
    }

    $sql = "
		SELECT unicod
		FROM public.unidadegestora
		WHERE ungcod = '".$_SESSION['elabrev']['ungcod']."'
	";
    $uncod = $db->pegaUm($sql);

    if ($uncod != '26101'){
        $stwhere2 = " and p.unicod = '".$uncod."'";
    }else{
        $stwhere2 = " and p.ungcod = '".$_SESSION['elabrev']['ungcod']."'";
    }

    $sql = "SELECT
				p.pliid as codigo,
				plicod||' - '||plidsc as descricao
			FROM
				monitora.pi_planointerno p
			INNER JOIN monitora.pi_planointernoptres pt on pt.pliid = p.pliid
			WHERE
				pt.ptrid = ".$request['ptrid']."
				AND p.pliano = '{$_SESSION['exercicio']}'
                AND p.plistatus = 'A'
                AND p.unicod IN ('26101','26298','26291','26290','26443')
			ORDER by
				2";

    echo $db->monta_combo('pliid[]',$sql,$habilita_Plano,'Selecione...','',$opc,'','100','S', 'pliid', '', '', $title= null);
}

function atualizaDescAcao( $request )
{
    global $db;

    if($request['ptrid']){

        $sql = "SELECT
					--substr(acatitulo, 1, 70)||'...' as acatitulo
					case when acatitulo is null then substr(acadsc, 1, 70)||'...'
					else substr(acatitulo, 1, 70)||'...' end as acatitulo
				FROM
					monitora.ptres p
				INNER JOIN monitora.acao a ON a.acaid = p.acaid
				INNER JOIN public.unidadegestora u ON u.unicod = p.unicod
				WHERE
					ptrid = ".$request['ptrid'];

        echo $db->pegaUm($sql);
    }
}

function atualizaNomeAcao( $request )
{
    global $db;

    $sql = "SELECT
				a.acacod
			FROM
				monitora.ptres p
			INNER JOIN monitora.acao a ON a.acaid = p.acaid
			INNER JOIN public.unidadegestora u ON u.unicod = p.unicod
			WHERE
				ptrid = ".$request['ptrid'];

    if(empty($request['ptrid'])){
        echo "erro";
    }else{
        echo $db->pegaUm($sql);
    }
}


function novaLinhaPrevisao( $request )
{
    global $db;

    #Estado atual do documento - tabela monitora.termocooperacao
    if($_SESSION['elabrev']['tcpid'] != ''){
        $estadoAtual = pegarEstadoAtual( $_SESSION['elabrev']['tcpid'] );
    }

    $perfis = array();
    $perfis = pegaPerfilGeral($_SESSION['usucpf'], $_SESSION['sisid']);

    #PERFIL UO E SECRETARIA - NIVEL - 5 E 2
    if( ( in_array($estadoAtual, array(EM_CADASTRAMENTO, EM_DILIGENCIA)) ) && ( in_array(UO_CONSULTA_ORCAMENTO, $perfis) || in_array(UO_COORDENADOR_EQUIPE_TECNICA, $perfis ) || in_array(UO_EQUIPE_TECNICA, $perfis) ) ){
        $habilita_Plano = 'N';
        $habilita_Natur = 'S';
    }

    #PERFIL REITOR E SECRETARIO - NIVEL - 4 E 3
    $habilita_botao = 'S';
    if( in_array(PERFIL_REITOR, $perfis) || in_array(PERFIL_SECRETARIO, $perfis) || in_array(PERFIL_SECRETARIA, $perfis) ){
        $habilita_Plano = 'N';
        $habilita_Natur = 'N';
    }

    #PERFIL SUPER SUSÁRIO NIVEL - 1
    if( in_array(PERFIL_SUPER_USUARIO, $perfis) || in_array(PERFIL_CGSO, $perfis) || in_array(PERFIL_DIRETORIA, $perfis) ){
        $habilita_Plano = 'S';
        $habilita_Natur = 'S';
    }

    if( $estadoAtual == ALTERAR_TERMO_COOPERACAO && ( in_array(UO_EQUIPE_TECNICA, $perfis) || in_array(PERFIL_SUPER_USUARIO, $perfis) ) ){
        $habilita_Plano = 'N';
        $habilita_Natur = 'S';
    }

    if( testaRespUGConcedente() && $_SESSION['elabrev']['termo']['aba'] == 'previsao' && $estadoAtual == EM_ANALISE_OU_PENDENTE){
        $habilita_Plano = 'S';
        $habilita_Natur = 'S';
    }

    if( in_array(PERFIL_SUPER_USUARIO, $perfis) ){
        $habilita_Plano = 'S';
        $habilita_Natur = 'S';
    }

    $existeSaldo = existeSaldoRemanejado($_SESSION['elabrev']['tcpid']);
    $usarSaldo = array_key_exists('index', $request);
    $_index = array_key_exists('index', $request) ? $request['index'] : '';

    $style = '';
    if ($existeSaldo && $usarSaldo) {
        $style = 'background-color:#CCFFCC';
    }
    ?>
    <tr id="tr_<?=$request['cod'] ?>" style="<?=$style;?>">

    <td>
        <?php if ($existeSaldo && $usarSaldo) : ?>
            <input checked="checked" class="checkSaldoRemanejado" name='checkSaldoRemanejado[<?=$_index;?>]' style="display: none;" id='checkSR_<?=$request['index'] ?>' value='<?=$request['cod'] ?>' type='checkbox'/>
        <?php else: ?>
            &nbsp;
        <?php endif; ?>
    </td>

    <td align="center">
        <?php if( $request['cod'] <> '1' ){?>
            <img border="0" id="<?=$request['cod'] ?>" class="excluirPrevisao" title="Excluir" style="cursor: pointer" src="/imagens/excluir.gif">
        <?php }?>
        <input type="hidden" name="linha[<?=$_index;?>]" value=""/>
    </td>

    <td align="center" id="td_anoref_<?=$k ?>">
        <?php
        for($z=0;$z<=10;$z++){
            $arAnosRef[$z]['codigo']	= 2013+$z;
            $arAnosRef[$z]['descricao']	= 2013+$z;
        }

        $db->monta_combo('proanoreferencia['.$_index.']',$arAnosRef, 'S', 'Selecione...','',$opc,'','','S', 'proanoreferencia_'.$request['cod'], '', null, $title= null);
        ?>
    </td>

    <td align="center" id="td_acao_<?=$request['cod'] ?>">
        &nbsp;
    </td>

    <?php if($habilita_Plano == 'S'){?>
        <td align="center" id="td_prg_<?=$request['cod'] ?>">
            <?php
            $sql = "
						SELECT unicod
						FROM public.unidadegestora
						WHERE ungcod = '".$_SESSION['elabrev']['ungcod']."'
					";
            $uncod = $db->pegaUm($sql);

            if ($uncod != '26101') $stwhere = " or u.unicod = '26101'";

            if ($uncod != '26101'){
                $stwhere2 = " and p.unicod = '".$uncod."'";
            }else{
                $stwhere2 = " and p.ungcod = '".$_SESSION['elabrev']['ungcod']."'";
            }

            $sql = "
						SELECT 	distinct p.ptrid as codigo,
								ptres || ' - ' || p.funcod||'.'||p.sfucod||'.'||p.prgcod||'.'||p.acacod||'.'||p.unicod||'.'||p.loccod as descricao
						FROM monitora.ptres p
						INNER JOIN monitora.acao a ON a.acaid = p.acaid
						INNER JOIN public.unidadegestora u ON u.unicod = p.unicod
						INNER JOIN monitora.pi_planointernoptres pt on pt.ptrid = p.ptrid
						WHERE ptrano = '".$_SESSION['exercicio']."' AND ( u.ungcod = '".$_SESSION['elabrev']['ungcod']."' ".$stwhere." )
						ORDER BY 1;
					";

            // 					$db->monta_combo('ptrid[]',$sql,$habilita_Plano,'Selecione...','',$opc,'','200','S', 'ptrid', '', '', $title= null);
            ?>
            <script type="text/javascript" src="../includes/jquery-autocomplete/jquery.autocomplete.js"></script>
            <link rel="stylesheet" type="text/css" href="../includes/jquery-autocomplete/jquery.autocomplete.css" />
            <script type="text/javascript">
                $(function(){
                    $("[name^='ptrid_temp']").autocomplete('elabrev.php?modulo=principal/termoCooperacao/cadTermoCooperacao&acao=A&aba=previsao&requisicao=carregaPlanosTrabalho', {
                        matchContains: true,
                        minChars: 0,
                        cacheLength:1000,
                        width: 440,
                        autoFill: false,
                        max: 1000
                    }).result(function(event, data, formatted) {

                        id_temp = event.target.id
                        arTemp = id_temp.split('_');
                        $("#ptrid_"+arTemp[1]).val(data[1]);

                        var ptrid = data[1];
                        $.ajax({
                            type: "POST",
                            url: "elabrev.php?modulo=principal/termoCooperacao/cadTermoCooperacao&acao=A",
                            data: "req=atualizaNomeAcao&ptrid="+ptrid,
                            async: false,
                            success: function(msg){
                                jQuery('#td_acao_'+arTemp[1]).html(msg);
                            }
                        });
                        $.ajax({
                            type: "POST",
                            url: "elabrev.php?modulo=principal/termoCooperacao/cadTermoCooperacao&acao=A",
                            data: "req=atualizaPI&ptrid="+ptrid,
                            async: false,
                            success: function(msg){
                                jQuery('#td_pi_'+arTemp[1]).html(msg);
                            }
                        });
                        $.ajax({
                            type: "POST",
                            url: "elabrev.php?modulo=principal/termoCooperacao/cadTermoCooperacao&acao=A",
                            data: "req=atualizaDescAcao&ptrid="+ptrid,
                            async: false,
                            success: function(msg){
                                jQuery('#td_acaodsc_'+arTemp[1]).html(msg);
                            }
                        });
                    });
                });
            </script>
            <?=campo_texto('ptrid_temp['.$_index.']','S','S','',20,27,'','', 'left', '', '', 'id="ptridtemp_'.$request['cod'].'"', '', '');?>
            <input type="hidden" name="ptrid[<?=$_index;?>]" id="ptrid_<?php echo $request['cod']; ?>" value="" />
        </td>
    <?php }else{?>
        <td align="center" id="td_prg_<?=$request['cod'] ?>">
            &nbsp;-&nbsp;
        </td>
    <?php }?>

    <?php if($habilita_Plano == 'S'){?>
        <td align="center" id="td_pi_<?=$request['cod'] ?>">
            &nbsp;
        </td>
    <?php }else{?>
        <td align="center" id="td_pi_<?=$request['cod'] ?>">
            &nbsp;-&nbsp;
        </td>
    <?php }?>

    <td align="center" id="td_acaodsc_<?=$request['cod'] ?>">
        &nbsp;
    </td>

    <?php if($habilita_Natur == 'S'){?>
        <td align="center" >
            <?php
            $sql = "
						SELECT	DISTINCT	ndpid as codigo,
								substr(ndpcod, 1, 6) || ' - ' || ndpdsc as descricao
						FROM public.naturezadespesa
						WHERE ndpstatus = 'A' and sbecod = '00' and edpcod != '00' and substr(ndpcod,1,2) not in ( '31', '32', '46' )
						and ( substr(ndpcod, 3, 2) in ('80', '90', '91') or substr(ndpcod, 1, 6) in ('335041','339147','335039', '445041', '333041') )
						order by 2
					";
            $db->monta_combo('ndpid['.$_index.']',$sql,'S','Selecione...','',$opc,'','250','S', 'ndpid', '', '', $title= null);
            ?>
        </td>
    <?php }else{?>
        <td align="center">
            &nbsp;-&nbsp;
        </td>
    <?php }?>

    <?php if($habilita_Natur == 'S'){?>
        <td align="center">
            <?=campo_texto('provalor['.$_index.']','S','S','',18,17,'[.###],##','', 'right', '', '', 'id="provalor_'.$request['cod'].'"', '', $dados['provalor']);?>
        </td>
    <?php }else{?>
        <td align="center">
            &nbsp;-&nbsp;
        </td>
    <?php }?>

    <?php if(in_array(PERFIL_SUPER_USUARIO, $perfis) || in_array(PERFIL_SECRETARIA, $perfis) || in_array(PERFIL_COORDENADOR_SEC, $perfis)):?>
        <td align="center" >
            <?php
            $sql = Array(Array('codigo'=>1,'descricao'=>'Janeiro'),
                Array('codigo'=>2,'descricao'=>'Fevereiro'),
                Array('codigo'=>3,'descricao'=>'Março'),
                Array('codigo'=>4,'descricao'=>'Abril'),
                Array('codigo'=>5,'descricao'=>'Maio'),
                Array('codigo'=>6,'descricao'=>'Junho'),
                Array('codigo'=>7,'descricao'=>'Julho'),
                Array('codigo'=>8,'descricao'=>'Agosto'),
                Array('codigo'=>9,'descricao'=>'Setembro'),
                Array('codigo'=>10,'descricao'=>'Outubro'),
                Array('codigo'=>11,'descricao'=>'Novembro'),
                Array('codigo'=>12,'descricao'=>'Dezembro')
            );
            $db->monta_combo('crdmesliberacao['.$_index.']',$sql,'S','Selecione...','',$opc,'','85','S', 'crdmesliberacao', '', $dado['crdmesliberacao'], $title= null);
            ?>
        </td>
    <?php else: ?>
        <td align="center">
            &nbsp;-&nbsp;
        </td>
    <?php endif; ?>

    <td align="center">
        <?php
        $sql = array();
        for($i = 1; $i <= 50; $i++){
            $sql[$i-1]['codigo'] = $i;
            $sql[$i-1]['descricao'] = $i.' Mês(s)';
        }
        array_push($sql, $sql);
        $db->monta_combo('crdmesexecucao['.$_index.']', $sql,'S','Selecione...','',$opc,'','100','S', 'crdmesexecucao', '', $dado['crdmesexecucao'], $title= null);
        echo '<script type="text/javascript"> $("select[name=\'crdmesexecucao['.$_index.']\']").addClass("crdmesexecucao"); </script>';
        ?>
    </td>

    </tr>
<?php
}

function validarValorCronograma(){
    global $db;
    $sql = "
		Select	tcp.tcpid,
				valor_prev,
				valor_crono
		From monitora.termocooperacao tcp

		Left Join (
			Select 	sum(provalor) as valor_prev,
				 tcpid
			From monitora.previsaoorcamentaria
			Where prostatus = 'A'
			Group by tcpid
		) prev on prev.tcpid = tcp.tcpid

		Left join (
			Select 	sum(crdvalor) as valor_crono,
				tcpid
			From monitora.cronogramadesembolso
			Where crdstatus = 'A'
			Group by tcpid
		) crono on crono.tcpid = tcp.tcpid

		Where tcp.tcpid = ".$_SESSION['elabrev']['tcpid'];
    $valor = $db->pegaLinha($sql);

    echo simec_json_encode($valor);
}

function listaPendencias() {

    require_once APPRAIZ . 'includes/workflow.php';

    global $db;

    $perfis = pegaPerfilGeral($_SESSION['usucpf'], $_SESSION['sisid']);
    $estadoAtual = pegarEstadoAtual( $_SESSION['elabrev']['tcpid'] );

    // Regra para perfil gestor e representante legal concedente e proponente
    //$arEsdidPropConc = array(PERFIL_PROREITOR_ADM, PERFIL_SUBSECRETARIO);
    $arEsdidPropConc = array(TERMO_AGUARDANDO_APROVACAO_GESTOR_PROP,
        EM_APROVACAO_DA_REITORIA,
        AGUARDANDO_APROVACAO_SECRETARIO,
        EM_ANALISE_PELA_SPO);

    $boMostraWorkflow = true;

    if (in_array($estadoAtual, $arEsdidPropConc)) {

        $boMostraWorkflow = false;

        $sql = "select
					ungcodproponente,
					ungcodconcedente
				from monitora.termocooperacao
				where tcpid = {$_SESSION['elabrev']['tcpid']}";

        $rsUgConcPropTermo = $db->pegaLinha($sql);

        if (in_array(PERFIL_SUPER_USUARIO, $perfis)) {
            $boMostraWorkflow = true;
        }

        $rco = new RelatorioCumprimentoObjeto();
        if (in_array(UO_EQUIPE_TECNICA, $perfis) && $rco->termoVencido($_SESSION['elabrev']['tcpid'])) {
            $boMostraWorkflow = true;
        }

        // esdid = Termo aguardando aprovação do Representante Legal do Proponente && pflcod = Representante Legal do Proponente
        else if ($estadoAtual == EM_APROVACAO_DA_REITORIA && in_array(PERFIL_REITOR, $perfis)) {
            if(is_array($_SESSION['elabrev']['proponente']['ungcod'])){
                if(in_array($rsUgConcPropTermo['ungcodproponente'], $_SESSION['elabrev']['proponente']['ungcod']))
                    $boMostraWorkflow = true;
            }else{
                if($rsUgConcPropTermo['ungcodproponente'] == $_SESSION['elabrev']['proponente']['ungcod'])
                    $boMostraWorkflow = true;
            }
        }

        // esdid = Termo aguardando aprovação pelo  Representante Legal do Concedente && pflcod = Representante Legal do Concedente
        else if ($estadoAtual == AGUARDANDO_APROVACAO_SECRETARIO && in_array(PERFIL_SECRETARIO, $perfis)) {
            if(is_array($_SESSION['elabrev']['concedente']['ungcod'])){
                if(in_array($rsUgConcPropTermo['ungcodconcedente'], $_SESSION['elabrev']['concedente']['ungcod']))
                    $boMostraWorkflow = true;
            }else{
                if($rsUgConcPropTermo['ungcodconcedente'] == $_SESSION['elabrev']['concedente']['ungcod'])
                    $boMostraWorkflow = true;
            }
        }

        // esdid = Termo aguardando aprovação do Gestor Orçamentário do Proponente && pflcod = Gestor Orçamentário do Proponente
        else if ($estadoAtual == TERMO_AGUARDANDO_APROVACAO_GESTOR_PROP && in_array(PERFIL_PROREITOR_ADM, $perfis)) {
            if(is_array($_SESSION['elabrev']['proponente']['ungcod'])){
                if(in_array($rsUgConcPropTermo['ungcodproponente'], $_SESSION['elabrev']['proponente']['ungcod']))
                    $boMostraWorkflow = true;
            }else{
                if($rsUgConcPropTermo['ungcodproponente'] == $_SESSION['elabrev']['proponente']['ungcod'])
                    $boMostraWorkflow = true;
            }
        }

        // esdid = Termo em Análise pelo Gestor Orçamentário do Concedente && pflcod = Gestor Orçamentário do Concedente
        else if ($estadoAtual == EM_ANALISE_PELA_SPO && in_array(PERFIL_SUBSECRETARIO, $perfis)) {
            if(is_array($_SESSION['elabrev']['concedente']['ungcod'])){
                if(in_array($rsUgConcPropTermo['ungcodconcedente'], $_SESSION['elabrev']['concedente']['ungcod']))
                    $boMostraWorkflow = true;
            }else{
                if($rsUgConcPropTermo['ungcodconcedente'] == $_SESSION['elabrev']['concedente']['ungcod'])
                    $boMostraWorkflow = true;
            }
        }
    }

    $sql = "SELECT DISTINCT

				CASE WHEN ungcodproponente IS NOT NULL
					THEN true
					ELSE false
				END as abaproponente,

				CASE WHEN ungcodconcedente IS NOT NULL
					THEN true
					ELSE false
				END as abaconcedente,

				CASE WHEN tcpdscobjetoidentificacao IS NOT NULL OR tcpobjetivoobjeto IS NOT NULL
					THEN true
					ELSE false
				END as abadescentralizacao,

				CASE WHEN
					( select count(*) from monitora.previsaoorcamentaria po06 where po06.tcpid = tcp.tcpid AND po06.prostatus = 'A'
									and po06.ndpid is not null
									and po06.provalor is not null)
					=
					( select count(*) from monitora.previsaoorcamentaria po05 where po05.tcpid = tcp.tcpid AND po05.prostatus = 'A' )
					THEN true
					ELSE false
				END as abaprevisao,

				CASE WHEN
						tcpconsidentproponente 	IS NOT NULL AND
						tcpconsidproposta  		IS NOT NULL AND
						tcpconsidobjeto  		IS NOT NULL AND
						tcpconsidobjetivo  		IS NOT NULL AND
						tcpconsidjustificativa  IS NOT NULL AND
						tcpconsidvalores  		IS NOT NULL AND
						tcpconsidcabiveis  		IS NOT NULL
					THEN true
					ELSE false
				END as abaparecertecnico,

				CASE WHEN
					( select count(*) from monitora.previsaoorcamentaria po03 where po03.tcpid = tcp.tcpid
									AND po03.prostatus = 'A'
									and po03.ptrid is not null
									and po03.pliid is not null
									and po03.crdmesliberacao is not null )
					=
					( select count(*) from monitora.previsaoorcamentaria po02 where po02.tcpid = tcp.tcpid AND po02.prostatus = 'A' )
					THEN true
					ELSE false
				END as abaprevisaoanalise,

				( select count(recid) from elabrev.relatoriocumprimento rec  where rec.tcpid = tcp.tcpid ) as relcumprimento,

				CASE WHEN apo.arqid IS NOT NULL
					THEN true
					ELSE false
				END as abaanexo
			FROM
				monitora.termocooperacao tcp
			LEFT  JOIN monitora.arquivoprevorcamentaria apo ON apo.tcpid = tcp.tcpid AND apo.arptipo = 'A'
			WHERE
				tcp.tcpid = ".$_SESSION['elabrev']['tcpid'];

    $arrValida = $db->pegaLinha($sql);

    $arrValida['abaproponente']			= $arrValida['abaproponente'] == 't' 		? true : false;
    $arrValida['abaconcedente'] 		= $arrValida['abaconcedente'] == 't' 		? true : false;
    $arrValida['abadescentralizacao'] 	= $arrValida['abadescentralizacao'] == 't' 	? true : false;
    $arrValida['abaprevisao']			= $arrValida['abaprevisao'] == 't' 			? true : false;
    $arrValida['abaparecertecnico']		= $arrValida['abaparecertecnico'] == 't' 	? true : false;
    $arrValida['abaprevisaoanalise']	= $arrValida['abaprevisaoanalise'] == 't' 	? true : false;
    $arrValida['relcumprimento']		= $arrValida['relcumprimento']>0	 		? true : false;
    $arrValida['abaanexo']				= $arrValida['abaanexo'] == 't'				? true : false;

    $verificaEmcadastramento = false;
    if ($arrValida['abaproponente'] && $arrValida['abaconcedente'] && $arrValida['abadescentralizacao']
        && $arrValida['abaprevisao'] && $arrValida['abaanexo'] &&
        ($estadoAtual == EM_CADASTRAMENTO || $estadoAtual == EM_DILIGENCIA))
    {
        $verificaEmcadastramento = true;
    }

    $verificaEmanalise = false;
    if ($arrValida['abaprevisaoanalise'] && $arrValida['abaparecertecnico'] && $estadoAtual == EM_ANALISE_OU_PENDENTE) {
        $verificaEmanalise = true;
    }

    $verificaOutroestado = false;
    if (!in_array($estadoAtual, array(EM_CADASTRAMENTO, EM_DILIGENCIA, EM_ANALISE_OU_PENDENTE, EM_EXECUCAO))) {
        $verificaOutroestado = true;
    }

    $verificaEmexecucao = false;
    if ($arrValida['relcumprimento'] && $estadoAtual == EM_EXECUCAO) {
        $verificaEmexecucao = true;
    }

    $docid = pegaDocidTermo();
    ?>

    <?php if($verificaOutroestado || $verificaEmanalise || $verificaEmcadastramento || $verificaEmexecucao) : ?>
        <tr>
            <td align="center">
                <br/>
                <b> Não possui pendências </b>
                <br/>
                <br/>
                <?php

                // Monta combo das coordenações
                if($estadoAtual == EM_ANALISE_DA_SECRETARIA && ( in_array(PERFIL_SECRETARIA, $perfis) || in_array(PERFIL_SUPER_USUARIO, $perfis) ) ){
                    $sql = "select dircod, ungcodconcedente, cooid, dircodpoliticafnde, ungcodpoliticafnde from monitora.termocooperacao where tcpid = {$_SESSION['elabrev']['tcpid']}";
                    $dado = $db->pegalinha($sql);

                    if($dado['ungcodconcedente'] == UG_FNDE && !empty($dado['dircodpoliticafnde'])){
                        $sql = "select cooid as codigo, coodsc as descricao from elabrev.coordenacao where dircod = '{$dado['dircodpoliticafnde']}' order by coodsc";
                    }else if($dado['ungcodconcedente'] == UG_FNDE && !empty($dado['ungcodpoliticafnde'])){
                        $sql = "select cooid as codigo, coodsc as descricao from elabrev.coordenacao where ungcodconcedente = '{$dado['ungcodpoliticafnde']}' order by coodsc";
                    }else{
                        $sql = "select cooid as codigo, coodsc as descricao from elabrev.coordenacao where ungcodconcedente = '{$dado['ungcodconcedente']}' order by coodsc";
                    }

                    if($db->pegaUm($sql)){
                        echo '<b>Selecione uma Coordenação </b><br/>';
                        $db->monta_combo('cooid',$sql,'S', 'Selecione','salvaCoordenacao','','','200','N','dircod','',$dado['cooid']);
                        echo "<br><br>";
                    }
                    else{
                        echo '<b><font color=red>É necessário preencher a aba Concedente para selecionar uma Coordenação.</font></b><br><br>';
                        echo '<b>Selecione uma Coordenação </b><br/>';
                        $db->monta_combo('cooid',$sql,'S', 'Selecione','salvaCoordenacao','','','200','N','dircod','',$dado['cooid']);
                        echo "<br><br>";
                    }
                }
                ?>
                <?php if($boMostraWorkflow): ?>
                    <?php echo wf_desenhaBarraNavegacao( $docid , array( 'docid' => $docid ),  array('historico'=>false) ); ?>
                <?php else: ?>
                    <script type="text/javascript">

                        function wf_atualizarTela( mensagem, janela )
                        {
                            janela.close();
                            enviarFormulario();
                        }

                        function wf_alterarEstado( aedid, docid, esdid, acao )
                        {
                            if ( !confirm( 'Deseja realmente Salvar e ' + acao + ' ?' ) ){
                                return;
                            }
                            if(!validarFormularioPrincipal()){
                                return;
                            }
                            var url = 'http://<?php echo $_SERVER['SERVER_NAME'] ?>/geral/workflow/alterar_estado.php' +
                                '?aedid=' + aedid +
                                '&docid=' + docid +
                                '&esdid=' + esdid +
                                '&verificacao=<?php echo urlencode( $dadosHtml ); ?>';
                            var janela = window.open(
                                url,
                                'alterarEstado',
                                'width=550,height=500,scrollbars=no,scrolling=no,resizebled=no'
                            );
                            janela.focus();
                        }

                        function wf_exibirHistorico( docid )
                        {
                            var url = 'http://<?php echo $_SERVER['SERVER_NAME'] ?>/geral/workflow/historico.php' +
                                '?modulo=principal/tramitacao' +
                                '&acao=C' +
                                '&docid=' + docid;
                            window.open(
                                url,
                                'alterarEstado',
                                'width=675,height=500,scrollbars=yes,scrolling=no,resizebled=no'
                            );
                        }

                    </script>
                    <table border="0" cellspacing="0" cellpadding="3" style="background-color: #f5f5f5; border: 2px solid #c9c9c9; width: 80px;">
                        <tbody>
                        <tr style="background-color: #c9c9c9; text-align:center;">
                            <td style="font-size:7pt; text-align:center;">
								<span title="estado atual">
									<b>estado atual</b>
								</span>
                            </td>
                        </tr>
                        <tr style="text-align:center;">
                            <td style="font-size:7pt; text-align:center;">
								<span title="estado atual">
									<?php
                                    $sql = "select esddsc from monitora.termocooperacao tcp
									join workflow.documento doc on doc.docid = tcp.docid
									join workflow.estadodocumento esd on esd.esdid = doc.esdid
									where tcp.tcpid = {$_SESSION['elabrev']['tcpid']}";
                                    echo $db->pegaUm($sql);
                                    ?>
								</span>
                            </td>
                        </tr>
                        <tr style="background-color: #c9c9c9; text-align:center;">
                            <td style="font-size:7pt; text-align:center;">
								<span title="estado atual">
									<b>ações</b>
								</span>
                            </td>
                        </tr>
                        <tr>
                            <td style="font-size: 7pt; text-align: center; border-top: 2px solid #d0d0d0;">
                                nenhuma ação disponível para o documento
                            </td>
                        </tr>
                        <tr style="background-color: #c9c9c9; text-align:center;">
                            <td style="font-size:7pt; text-align:center;">
								<span title="estado atual">
									<b>histórico</b>
								</span>
                            </td>
                        </tr>
                        <tr style="text-align:center;">
                            <td style="font-size:7pt; border-top: 2px solid #d0d0d0;">
                                <img onclick="wf_exibirHistorico( '<?php echo $docid; ?>' );" title="" src="http://simec-local/imagens/fluxodoc.gif" style="cursor: pointer;">
                            </td>
                        </tr>
                        </tbody>
                    </table>
                    <br/><br/>
                <?php endif; ?>
            </td>
        </tr>
    <?php else: ?>
        <?php $boPendencia = false; ?>
        <?php if(!$arrValida['relcumprimento'] && $estadoAtual == EM_EXECUCAO): ?>
            <?php $boPendencia = true; ?>
            <tr class="botao">
                <td>
                    <div style="color:red;text-align:center;" class="botao" onclick="window.location = 'elabrev.php?modulo=principal/termoCooperacao/cadTermoCooperacao&acao=A&aba=cumprimento'">
                        <b>- Relatório de cumprimento.</b>
                    </div>
                </td>
            </tr>
        <?php endif; ?>
        <?php if(!$arrValida['abaparecertecnico'] && $estadoAtual == EM_ANALISE_OU_PENDENTE && ( in_array(PERFIL_COORDENADOR_SEC, $perfis) || in_array(PERFIL_SUPER_USUARIO, $perfis) ) ): ?>
            <?php $boPendencia = true; ?>
            <tr class="botao">
                <td>
                    <div style="color:red;text-align:center;" class="botao" onclick="window.location = 'elabrev.php?modulo=principal/termoCooperacao/cadTermoCooperacao&acao=A&aba=parecertecnico'">
                        <b>- Aba Parecer Técnico.</b>
                    </div>
                </td>
            </tr>
        <?php endif; ?>
        <?php if(!$arrValida['abaprevisaoanalise'] && $estadoAtual == EM_ANALISE_OU_PENDENTE && ( in_array(PERFIL_COORDENADOR_SEC, $perfis) || in_array(PERFIL_SUPER_USUARIO, $perfis) ) ): ?>
            <?php $boPendencia = true; ?>
            <tr class="botao">
                <td>
                    <div style="color:red;text-align:center;" class="botao" onclick="window.location = 'elabrev.php?modulo=principal/termoCooperacao/cadTermoCooperacao&acao=A&aba=previsao'">
                        <b>- Aba Previsão Orçamentária.</b>
                    </div>
                </td>
            </tr>
        <?php endif; ?>
        <?php if( !$arrValida['abaproponente'] ): ?>
            <?php $boPendencia = true; ?>
            <tr class="botao">
                <td>
                    <div style="color:red;text-align:center;" class="botao" onclick="window.location = 'elabrev.php?modulo=principal/termoCooperacao/cadTermoCooperacao&acao=A&aba=proponente'">
                        <b>- Aba Proponente.</b>
                    </div>
                </td>
            </tr>
        <?php endif; ?>
        <?php if( !$arrValida['abaconcedente'] ): ?>
            <?php $boPendencia = true; ?>
            <tr class="botao">
                <td>
                    <div style="color:red;text-align:center;" class="botao" onclick="window.location = 'elabrev.php?modulo=principal/termoCooperacao/cadTermoCooperacao&acao=A&aba=concedente'">
                        <b>- Aba Concedente.</b>
                    </div>
                </td>
            </tr>
        <?php endif; ?>
        <?php if( !$arrValida['abadescentralizacao'] ): ?>
            <?php $boPendencia = true; ?>
            <tr class="botao">
                <td>
                    <div style="color:red;text-align:center;" class="botao" onclick="window.location = 'elabrev.php?modulo=principal/termoCooperacao/cadTermoCooperacao&acao=A&aba=descentralizacao'">
                        <b>- Aba Objeto e Justificativa da Descentralização do Crédito.</b>
                    </div>
                </td>
            </tr>
        <?php endif; ?>
        <?php if( !$arrValida['abaprevisao'] && ($estadoAtual == EM_CADASTRAMENTO || $estadoAtual == EM_DILIGENCIA) ): ?>
            <?php $boPendencia = true; ?>
            <tr class="botao">
                <td>
                    <div style="color:red;text-align:center;" class="botao" onclick="window.location = 'elabrev.php?modulo=principal/termoCooperacao/cadTermoCooperacao&acao=A&aba=previsao'">
                        <b>- Aba Previsão Orçamentária.</b>
                    </div>
                </td>
            </tr>
        <?php endif; ?>
        <?php if( !$arrValida['abaanexo'] && ($estadoAtual == EM_CADASTRAMENTO || $estadoAtual == EM_DILIGENCIA) ): ?>
            <?php $boPendencia = true; ?>
            <tr class="botao">
                <td>
                    <div style="color:red;text-align:center;" class="botao" onclick="window.location = 'elabrev.php?modulo=principal/termoCooperacao/cadTermoCooperacao&acao=A&aba=anexo'">
                        <b>- Aba Anexo.</b>
                    </div>
                </td>
            </tr>
        <?php endif; ?>
        <?php if($boMostraWorkflow): ?>
            <tr>
                <td align="center">
                    <?php echo wf_desenhaBarraNavegacao( $docid , array( 'docid' => $docid ),  array('historico'=>false) ); ?>
                </td>
            </tr>
        <?php endif; ?>
    <?php
    endif;
}

function listaAnexos( $tipo_anexo = null )
{
    global $db, $habilitaInserir;

    if($tipo_anexo){
        $where = " and m.arptipo = '".$tipo_anexo."' ";
    }

    $sql = "
		Select 	DISTINCT m.arqid,
				m.arpdsc,
				a.arqnome as arqnome,
				a.arqdescricao as arqdescricao,
				su.usunome,
                to_char(a.arqdata, 'DD/MM/YYYY') as criado
		From monitora.arquivoprevorcamentaria m
		inner Join public.arquivo a on a.arqid = m.arqid
        JOIN seguranca.usuario su ON (su.usucpf = a.usucpf)
		Where arqstatus = 'A' and tcpid = ".$_SESSION['elabrev']['tcpid']." $where
		Order by 1
	";
    //ver($sql, d);
    $dados = !empty($_SESSION['elabrev']['tcpid']) ? $db->carregar($sql) : array();

    $strConefere = "
        select esdid
        from workflow.documento
        where docid = (
          select docid from monitora.termocooperacao where tcpid = {$_SESSION['elabrev']['tcpid']}
        )
    ";

    if ($dados[0] != '') {
        foreach( $dados as $k => $dado ){
            if( $k % 2 == 0){
                $cor = '#CFCFCF';
            }else{
                $cor = '#F5F5F5';
            }
            ?>
            <tr id="tr_<?=$dado['arqid'] ?>">
                <td align="center" style="background-color: <?=$cor ?>;">
                    <?php if (teste_superUser()) { ?>
                        <img border="0" id="<?=$dado['arqid'] ?>" class="excluirLinha" title="Excluir" style="cursor: pointer" src="/imagens/excluir.gif">
                    <?php } elseif ($habilitaInserir == 'S' && emDilegenciaSemExecucao($_SESSION['elabrev']['tcpid'])) { ?>
                        <img border="0" id="<?=$dado['arqid'] ?>" class="excluirLinha" title="Excluir" style="cursor: pointer" src="/imagens/excluir.gif">
                    <?php } else { ?>
                        <?php if ($habilitaInserir == 'S' && (EM_EXECUCAO != $db->pegaUm($strConefere))) { ?>
                            <img border="0" id="<?=$dado['arqid'] ?>" class="excluirLinha" title="Excluir" style="cursor: pointer" src="/imagens/excluir.gif">
                        <?php } else { ?>
                            <img border="0" id="<?=$dado['arqid'] ?>" title="Excluir" src="/imagens/excluir_01.gif">
                        <?php } ?>
                    <?php } ?>
                </td>
                <td align="left" style="background-color: <?=$cor ?>;"><b><?=$dado['arqnome'] ?></b></td>
                <td align="left" style="background-color: <?=$cor ?>;"><?if($dado['arqdescricao']!='Termo Execução Descentralizada: '){ echo $dado['arqdescricao'];} ?></td>
                <td align="center" style="background-color: <?=$cor ?>;"><b><?=$dado['usunome'] ?></b></td>
                <td align="center" style="background-color: <?=$cor ?>;"><b><?=$dado['criado'] ?></b></td>
                <td align="center" style="background-color: <?=$cor ?>;">
                    <a class="baixar" id="<?=$dado['arqid'] ?>">Visualizar arquivo</a>
                </td>
            </tr>
        <?php
        }
    }
}

function listaCronograma(){
    global $db;

    $habilita_botao = 'N';
    #Estado atual do documento - tabela monitora.termocooperacao
    If($_SESSION['elabrev']['tcpid'] != ''){
        $estadoAtual = pegarEstadoAtual( $_SESSION['elabrev']['tcpid'] );
    }

    $perfis = array();
    $perfis = pegaPerfilGeral($_SESSION['usucpf'], $_SESSION['sisid']);

    #PERFIL UO E SECRETARIA - NIVEL - 5 E 2
    if( in_array(PERFIL_SECRETARIA, $perfis) || ( ( $estadoAtual == EM_CADASTRAMENTO ) && ( in_array(UO_CONSULTA_ORCAMENTO, $perfis) || in_array(UO_COORDENADOR_EQUIPE_TECNICA, $perfis ) || in_array(UO_EQUIPE_TECNICA, $perfis) ) ) ){
        $habilita_Parc = 'S';
        $habilita_Acao = 'N';
        $habilita_botao = 'S';
    }elseif( ( ( $estadoAtual == EM_APROVACAO_DA_REITORIA ) && ( in_array(UO_CONSULTA_ORCAMENTO, $perfis) || in_array(UO_COORDENADOR_EQUIPE_TECNICA, $perfis) || in_array(UO_EQUIPE_TECNICA, $perfis) ) ) ){
        $habilita_Parc = 'N';
        $habilita_Acao = 'N';
        $habilita_botao = 'N';
    }

    #PERFIL REITOR E SECRETARIO - NIVEL - 4 E 3
    if( in_array(PERFIL_REITOR, $perfis) || in_array(PERFIL_SECRETARIO, $perfis)  || in_array(PERFIL_SECRETARIA, $perfis) ){
        $habilita_Parc = 'N';
        $habilita_Acao = 'N';
        $habilita_botao = 'N';
    }

    #PERFIL SUPER SUSÁRIO NIVEL - 1
    if( in_array(PERFIL_SUPER_USUARIO, $perfis) || in_array(PERFIL_CGSO, $perfis) || ( in_array(PERFIL_DIRETORIA, $perfis) && $estadoAtual == EM_ANALISE_OU_PENDENTE ) ){
        $habilita_Parc = 'S';
        $habilita_Acao = 'S';
        $habilita_botao = 'S';
    }

    $sql = "
		SELECT	DISTINCT crdid,
				crdparcela,
				c.acaid as acaid,
				crdmesliberacao,
				to_char(crdvalor, '999G999G999G999G999D99') as crdvalor,
				crdmesexecucao,
				a.acacod||'-'||coalesce(a.acatitulo,'_') as acaid_descricao
		FROM monitora.cronogramadesembolso c

		Left Join monitora.acao a ON a.acaid = c.acaid

		WHERE crdstatus = 'A' AND tcpid = ".$_SESSION['elabrev']['tcpid']."
		Order by crdid
	";
    //ver($sql, d);
    $dados = $db->carregar($sql);

    if( $dados[0] != '' ){
        foreach( $dados as $k => $dado ){
            if(	$habilita_botao == 'S' ){
                ?>
                <tr id="tr_<?=$k ?>">
                <td align="center">
                    <img border="0" id="<?=$k ?>" class="excluirLinha" title="Excluir" style="cursor: pointer" src="/imagens/excluir.gif">
                    <input type="hidden" name="linha[]" value="<?=$dado['crdid'] ?>"/>
                </td>
            <?php }else{?>
                <td align="center">
                    <img border="0" id="<?=$k ?>" title="Excluir" src="/imagens/excluir_01.gif">
                    <input type="hidden" name="linha[]" value="<?=$dado['crdid'] ?>"/>
                </td>
            <?php }?>
            <?php if($habilita_Parc == 'S'){?>
                <td align="center">
                    <?=campo_texto('crdparcela[]','S','S','',3,2,'[.###],##','', '', '', '', 'id="crdparcela"', '', $dado['crdparcela']);?>
                </td>
            <?php }else{?>
                <td align="center">
                    <input type="hidden" name="crdparcela[]" id="crdparcela[]" value="<?=$dado['crdparcela']?>">
                    <?=$dado['crdparcela']?>
                </td>
            <?php }?>

            <?php if($habilita_Acao == 'S'){?>
                <td align="center">
                    <?php
                    $sql1 = "
					SELECT 	DISTINCT a.acaid as codigo,
							a.acacod||'-'||coalesce(a.acatitulo,'_') as descricao
					FROM monitora.ptres p
					INNER JOIN monitora.acao a ON a.acaid = p.acaid
					INNER JOIN public.unidadegestora u ON u.unicod = p.unicod
					INNER JOIN monitora.pi_planointernoptres pt on pt.ptrid = p.ptrid
					WHERE ptrano = '".$_SESSION['exercicio']."' AND
						  p.ptrid in ( select ptrid from monitora.previsaoorcamentaria where tcpid = ".$_SESSION['elabrev']['tcpid']." )
					ORDER BY 1;
				";
                    $db->monta_combo('acaid[]',$sql1,'S','Selecione...','','','','100','S', 'acaid', '', $dado['acaid'], $title= null);
                    ?>
                </td>
            <?php }else{?>
                <td align="center">
                    <input type="hidden" name="acaid[]" id="acaid[]" value="<?=$dado['acaid']?>">
                    <?=$dado['acaid_descricao'];?>
                </td>
            <?php }?>

            <?php if($habilita_Acao == 'S'){?>
                <td align="center">
                    <?php
                    $sql = Array(Array('codigo'=>1,'descricao'=>'Janeiro'),
                        Array('codigo'=>2,'descricao'=>'Fevereiro'),
                        Array('codigo'=>3,'descricao'=>'Março'),
                        Array('codigo'=>4,'descricao'=>'Abril'),
                        Array('codigo'=>5,'descricao'=>'Maio'),
                        Array('codigo'=>6,'descricao'=>'Junho'),
                        Array('codigo'=>7,'descricao'=>'Julho'),
                        Array('codigo'=>8,'descricao'=>'Agosto'),
                        Array('codigo'=>9,'descricao'=>'Setembro'),
                        Array('codigo'=>10,'descricao'=>'Outubro'),
                        Array('codigo'=>11,'descricao'=>'Novembro'),
                        Array('codigo'=>12,'descricao'=>'Dezembro')
                    );
                    //$db->monta_combo('crdmesliberacao[]',$sql,'S','Selecione...','',$opc,'','100','S', 'crdmesliberacao', '', $dado['crdmesliberacao'], $title= null);
                    ?>
                </td>
            <?php }else{?>
                <td align="center">
                </td>
            <?php }?>

            <?php if($habilita_Parc == 'S'){?>
                <td align="center">
                    <?=campo_texto('crdvalor[]','S','S','',30,21,'[.###],##','', '', '', '', 'id="crdvalor"', '', $dado['crdvalor']);?>
                </td>
            <?php }else{?>
                <td align="center">
                    <input type="hidden" name="crdvalor[]" id="'crdvalor[]" value="<?=$dado['crdvalor']?>">
                    <?=$dado['crdvalor'];?>
                </td>
            <?php }?>

            </tr>
        <?php
        }
    }
}

function montaCabecalhoUG($param = Array())
{
    global $db;
    $stInner = $stWhere = "";

    if ($_GET['ungcod']) {

        $stWhere .= "where u.ungcod = '".$_GET['ungcod']."'";

    } elseif ($_SESSION['elabrev']['tcpid']) {

        $stInner .= "inner join monitora.termocooperacao t on t.ungcodproponente = u.ungcod";
        $stWhere .= "where t.tcpid = {$_SESSION['elabrev']['tcpid']}";

    } elseif ($_SESSION['elabrev']['ungcod']) {

        $stWhere .= "where u.ungcod = '".$_SESSION['elabrev']['ungcod']."'";

    } elseif ($_SESSION['elabrev']['unicod']) {

        if (is_array($_SESSION['elabrev']['unicod'])) {
            $stWhere .= " where u.unicod in ('".implode("','", $_SESSION['elabrev']['unicod'])."') ";
        } else {
            $stWhere .= " where u.unicod = '{$_SESSION['elabrev']['unicod']}' ";
        }
    }

    if (!empty($stWhere)) {

        $sql = "SELECT u.ungabrev||' / '||u.ungdsc as descricao
				FROM public.unidadegestora u
				{$stInner}
				{$stWhere}";

        $UG = $db->pegaUm($sql);
        $rsCountSolAlt = false;

        if ($_SESSION['elabrev']['tcpid']) {
            $sqlCountSolAteracao = " select count(*) from workflow.historicodocumento where aedid = ".WF_ACAO_SOL_ALTERACAO." and docid = (select docid from monitora.termocooperacao where tcpid = {$_SESSION['elabrev']['tcpid']}) ";
            $rsCountSolAlt = $db->pegaUm($sqlCountSolAteracao);
        }

        if ($_SESSION['elabrev']['tcpid']) {
            $ungcod = $db->pegaUm("select tc.ungcodconcedente from monitora.termocooperacao tc where tc.tcpid = {$_SESSION['elabrev']['tcpid']}");
            $estadoAtual = pegarEstadoAtual($_SESSION['elabrev']['tcpid']);
        }

        ?>
        <script type="text/javascript" src="../../includes/jquery-ui/jquery-ui-1.8.20.custom.min.js"></script>
        <link rel="stylesheet" type="text/css" href="../../includes/jquery-ui/jquery-ui-1.8.22.custom.css"/>

        <table align="center" bgcolor="#f5f5f5" border="0" class="tabela" cellpadding="3" cellspacing="1">
            <tr>
                <td class="SubTituloDireita" valign="bottom" width="15%">Unidade Gestora:</td>
                <td><?php echo $UG; ?></td>
            </tr>
            <?php if ($param['termo']) : ?>
                <tr>
                    <td class="SubTituloDireita" valign="bottom" width="15%">Número do Termo:</td>
                    <td>
                        <?php
                        if ($_SESSION['elabrev']['tcpid']) {
                            if ($rsCountSolAlt > 0) {
                                echo "{$_SESSION['elabrev']['tcpid']}.$rsCountSolAlt";
                            } else {
                                echo $_SESSION['elabrev']['tcpid'];
                            }
                        } else {
                            echo 'Novo';
                        }
                        ?>
                    </td>
                </tr>
            <?php endif ; ?>
            <!-- #Unidade FNDE -->

            <?php
            $setPflCod = array(PERFIL_AREA_TECNICA_FNDE, PERFIL_DIRETORIA_FNDE, PERFIL_SUPER_USUARIO);
            $isAbaConcedente = ($_SESSION['elabrev']['termo']['aba'] == 'concedente');
            if (($ungcod == UG_FNDE) && ($estadoAtual == TERMO_EM_ANALISE_ORCAMENTARIA_FNDE) && possuiPerfil($setPflCod) && $isAbaConcedente) :
                ?>
                <tr>
                    <td class="SubTituloDireita" valign="bottom" width="15%">Número do Processo FNDE:</td>
                    <td>
                        <?php
                        $tcpnumprocessofnde = $db->pegaUm("select tc.tcpnumprocessofnde from monitora.termocooperacao tc where tc.tcpid = {$_SESSION['elabrev']['tcpid']}");
                        if ($tcpnumprocessofnde) {
                            $tcpnumprocessofnde = substr($tcpnumprocessofnde,0,5) . "." .
                                substr($tcpnumprocessofnde,5,6) . "/" .
                                substr($tcpnumprocessofnde,11,4) . "-" .
                                substr($tcpnumprocessofnde,15,2);
                            echo $tcpnumprocessofnde;
                        } else {
                            ?>
                            <input type="button" name="btnGerarProcesso" value="Gerar Nº Processo FNDE" onclick="gerarProcessoFNDE(<?=$_SESSION['elabrev']['tcpid']; ?>);">
                        <?
                        }
                        ?>
                    </td>
                </tr>
                <!-- unidade FNDE -->
            <?php endif;  ?>

            <?php
            if ($_SESSION['elabrev']['tcpid']) {
                list($estadoAtual, $dscEstadoAtual) = pegarEstadoAtual($_SESSION['elabrev']['tcpid'], $retornarDescricao = true);
                $sql = "select cmddsc from monitora.termocooperacao tcp
                        inner join workflow.historicodocumento hst on hst.docid = tcp.docid
                        inner join workflow.comentariodocumento cmd on cmd.hstid = hst.hstid
                        where tcp.tcpid = {$_SESSION['elabrev']['tcpid']}
                        order by cmd.cmdid desc, hst.hstid desc limit 1";
                $rsJustificativa = $db->pegaUm($sql);
                if ($rsJustificativa && ($estadoAtual == ALTERAR_TERMO_COOPERACAO || $estadoAtual == EM_DILIGENCIA || $estadoAtual == TERMO_EM_DILIGENCIA_RELATORIO)): ?>
                    <tr>
                        <td class="SubTituloDireita" valign="top">Justificativa:</td>
                        <td><?php echo $rsJustificativa; ?></td>
                    </tr>
                <?php endif; // -- end justificativa
                if ($dscEstadoAtual): ?>
                    <tr>
                        <td class="SubTituloDireita">Situação:</td>
                        <td><?php echo $dscEstadoAtual; ?></td>
                    </tr>
                <?php endif; // -- situação
            } // -- end tcpid
            ?>
        </table>
    <?
    }
}

function montaCabecalhoGeral( $param = Array()){
    global $db;

    $perfis = array();
    $perfis = pegaPerfilGeral($_SESSION['usucpf'], $_SESSION['sisid']);

    if( in_array(PERFIL_SECRETARIA, $perfis) || in_array(PERFIL_SECRETARIO, $perfis) || in_array(PERFIL_CGSO, $perfis) || in_array(PERFIL_SUBSECRETARIO, $perfis) ){
        $titulo = "Secretaria(s) / UG(s):";
        $sql = "
			Select und.ungabrev as descricao
			From  public.unidadegestora und
			Join elabrev.usuarioresponsabilidade u on u.ungcod = und.ungcod
			Where u.rpustatus = 'A' and u.usucpf = '".$_SESSION['usucpf']."'
		";
    }elseif( in_array(PERFIL_DIRETORIA, $perfis) ){
        $titulo = "Diretoria(s):";
        $sql = "
			Select	distinct '('||ug.ungabrev||') / '||d.dirdsc as descricao
			From monitora.termocooperacao tcp
			Join elabrev.usuarioresponsabilidade usu_r on usu_r.dircod = tcp.dircod
			Join elabrev.diretoria d on d.dircod = tcp.dircod
			Join public.unidadegestora ug on ug.ungcod = d.ungcod
			Where usu_r.rpustatus = 'A' and usu_r.usucpf = '".$_SESSION['usucpf']."'
		";
    }elseif( in_array(PERFIL_SUPER_USUARIO, $perfis) ){
        $titulo = "Super Usuário - Listagem Geral:";
        $sql = "";
    }
    if($sql != ''){
        $dados = $db->carregar($sql);
    }

    #Bloco para cocatenar os nomes da ung(s), caso o usuário tem mais que uma.
    if($dados != ''){
        $k=0;
        foreach($dados as $valor){
            $unidades .= $valor['descricao'];
            if( $k % 2 == 1){
                $unidades .= ' / ';
            }
            $k=$k+1;
        }
        $unidades .= '.';
    }else{
        $unidades = '';
    }
    ?>
    <table align="center" bgcolor="#f5f5f5" border="0" class="tabela" cellpadding="3" cellspacing="1">
        <tr>
            <td class="SubTituloDireita" valign="bottom" width="15%"><b><?=$titulo ?></b></td>
            <td><?=$unidades ?></td>
        </tr>
    </table>
<?
}

function recuperaDadosUG($ungcod) {
    global $db;

    $sql = "
		SELECT	ungcod,
				ungcnpj,
				ungdsc,
				ungendereco,
				ungbairro,
				mun.estuf,
				est.estdescricao as estado,
				mun.muncod,
				mun.mundescricao as municipio,
				ungcep,
				ungfone,
				ungemail,
				gescod,
				unicod
		FROM public.unidadegestora ung
		LEFT JOIN territorios.municipio mun ON mun.muncod = ung.muncod
		LEFT JOIN territorios.estado est ON est.estuf = mun.estuf
		WHERE ungstatus = 'A' AND ungcod = '".$ungcod."'
	";
    return $db->pegaLinha($sql);
}

function dadosUGAjax( $request ){

    global $db;

    $sql = "SELECT
				ungcod,
				--codigo da gestao
				ungcnpj,
				ungdsc,
				ungendereco,
				ungbairro,
				estuf,
				mun.muncod,
				ungcep,
				ungfone,
				ungemail,
				gescod
			FROM
				public.unidadegestora ung
			LEFT JOIN territorios.municipio mun ON mun.muncod = ung.muncod
			WHERE
				ungstatus = 'A'
				AND ungcod = '".$request['ungcod']."'";
    //ver($sql, d);
    $dados = $db->pegaLinha($sql);
    $dados['ungendereco'] = utf8_encode($dados['ungendereco']);
    $dados['ungbairro'] = utf8_encode($dados['ungbairro']);
    echo simec_json_encode($dados);
}

function montaAbaTermoCooperacao()
{
    global $boPopup, $db;

    $perfis = array();
    $perfis = pegaPerfilGeral($_SESSION['usucpf'], $_SESSION['sisid']);

    if ($boPopup) {
        $pagina = 'visualizarTermo';
    } else {
        $pagina = 'cadTermoCooperacao';
    }

    if($_SESSION['elabrev']['tcpid'] != ''){
        $estadoAtual = pegarEstadoAtual( $_SESSION['elabrev']['tcpid'] );
    }

    if ($_SESSION['elabrev']['tcpid'] != '') {

        $tcptipoemenda = $db->pegaUm( "select tcptipoemenda from monitora.termocooperacao t where tcpid = {$_SESSION['elabrev']['tcpid']}" );

        require_once APPRAIZ . 'elabrev/classes/modelo/RelatorioCumprimentoObjeto.class.inc';
        $ted = new RelatorioCumprimentoObjeto();

        $abas = array();
        if ($estadoAtual == EM_EXECUCAO) {
            array_push($abas, array( "descricao" => "Relatório de Cumprimento",
                "link"	 	=> "elabrev.php?modulo=principal/termoCooperacao/{$pagina}&acao=A&aba=cumprimento" ));

        } elseif ($estadoAtual == RELATORIO_OBJ_AGUARDANDO_ANALISE_COORD ||
            $estadoAtual == TERMO_EM_DILIGENCIA_RELATORIO ||
            $estadoAtual == RELATORIO_OBJ_AGUARDANDO_APROV_GESTOR ||
            $estadoAtual == RELATORIO_OBJ_AGUARDANDO_APROV_REITORIA ||
            termoVencido($_SESSION['elabrev']['tcpid'])) {
            array_push($abas, array( "descricao" => "Relatório de Cumprimento",
                "link"	 	 => "elabrev.php?modulo=principal/termoCooperacao/{$pagina}&acao=A&aba=cumprimento" ));
        }

        if( in_array(PERFIL_DIRETORIA, $perfis) || in_array(PERFIL_SUPER_USUARIO, $perfis) || in_array(PERFIL_DIRETORIA_FNDE, $perfis) || in_array(PERFIL_AREA_TECNICA_FNDE, $perfis)){

            array_push($abas, array( "descricao" => "Proponente",
                "link"	  	 => "elabrev.php?modulo=principal/termoCooperacao/{$pagina}&acao=A&aba=proponente" ));
            array_push($abas, array( "descricao" => "Concedente",
                "link"	 	 => "elabrev.php?modulo=principal/termoCooperacao/{$pagina}&acao=A&aba=concedente" ));
            array_push($abas, array( "descricao" => "Objeto e Justificativa da Descentralização do Crédito",
                "link"	 	 => "elabrev.php?modulo=principal/termoCooperacao/{$pagina}&acao=A&aba=descentralizacao" ));
            if( $tcptipoemenda == 'S' && (in_array(PERFIL_SECRETARIO, $perfis) || in_array(PERFIL_SUPER_USUARIO, $perfis) ) ){
                array_push($abas, array( "descricao" => "Emenda Impositivo",
                    "link"	 	 => "elabrev.php?modulo=principal/termoCooperacao/{$pagina}&acao=A&aba=EmendaImpositivo" ));
            }
            array_push($abas, array( "descricao" => "Previsão Orçamentaria",
                "link"	 	 => "elabrev.php?modulo=principal/termoCooperacao/{$pagina}&acao=A&aba=previsao" ));
            array_push($abas, array( "descricao" => "Parecer técnico (Diretoria)",
                "link"	 	 => "elabrev.php?modulo=principal/termoCooperacao/{$pagina}&acao=A&aba=parecertecnico" ));
            array_push($abas, array( "descricao" => "Anexos",
                "link"	 	 => "elabrev.php?modulo=principal/termoCooperacao/{$pagina}&acao=A&aba=anexo" ));

        }else{

            array_push($abas, array( "descricao" => "Proponente",
                "link"	  	 => "elabrev.php?modulo=principal/termoCooperacao/{$pagina}&acao=A&aba=proponente" ));
            array_push($abas, array( "descricao" => "Concedente",
                "link"	 	 => "elabrev.php?modulo=principal/termoCooperacao/{$pagina}&acao=A&aba=concedente" ));
            array_push($abas, array( "descricao" => "Objeto e Justificativa da Descentralização do Crédito",
                "link"	 	 => "elabrev.php?modulo=principal/termoCooperacao/{$pagina}&acao=A&aba=descentralizacao" ));
            if( $tcptipoemenda == 'S' && (in_array(PERFIL_SECRETARIO, $perfis) || in_array(PERFIL_SUPER_USUARIO, $perfis) ) ){
                array_push($abas, array( "descricao" => "Emenda Impositivo",
                    "link"	 	 => "elabrev.php?modulo=principal/termoCooperacao/{$pagina}&acao=A&aba=EmendaImpositivo" ));
            }
            array_push($abas, array( "descricao" => "Previsão Orçamentaria",
                "link"	 	 => "elabrev.php?modulo=principal/termoCooperacao/{$pagina}&acao=A&aba=previsao" ));

// 			if( ( ( in_array(PERFIL_COORDENADOR_SEC, $perfis) ||
// 					in_array(PERFIL_SUPER_USUARIO, $perfis) ) &&
// 					in_array($estadoAtual, array(EM_ANALISE_OU_PENDENTE)) ) ||
// 					in_array($estadoAtual, array(AGUARDANDO_APROVACAO_SECRETARIO,
// 												EM_ANALISE_PELA_CGSO,
// 												EM_ANALISE_PELA_SPO,
// 												EM_DESCENTRALIZACAO,
// 												EM_EXECUCAO,
// 												TERMO_FINALIZADO)) ||
// 					testaRespUGConcedente() &&
// 					in_array($estadoAtual, array(EM_ANALISE_OU_PENDENTE))
// 					){

            $arFasesExibeParecer = array(
                EM_ANALISE_DA_SECRETARIA,
                EM_ANALISE_OU_PENDENTE,
                AGUARDANDO_APROVACAO_DIRETORIA,
                AGUARDANDO_APROVACAO_SECRETARIO,
                EM_ANALISE_PELA_CGSO,
                EM_ANALISE_PELA_SPO,
                EM_DESCENTRALIZACAO,
                EM_EMISSAO_NOTA_CREDITO,
                EM_EXECUCAO,
                RELATORIO_OBJ_AGUARDANDO_APROV_GESTOR,
                RELATORIO_OBJ_AGUARDANDO_APROV_REITORIA,
                RELATORIO_OBJ_AGUARDANDO_ANALISE_COORD,
                TERMO_FINALIZADO,
                TERMO_AGUARDANDO_VALIDACAO_DIRETORIA_FNDE
            );

            if( in_array($estadoAtual, $arFasesExibeParecer) ){

                array_push($abas, array( "descricao" => "Parecer técnico (Diretoria)",
                    "link"	 	 => "elabrev.php?modulo=principal/termoCooperacao/{$pagina}&acao=A&aba=parecertecnico" ));
            }

            array_push($abas, array( "descricao" => "Anexos",
                "link"	 	 => "elabrev.php?modulo=principal/termoCooperacao/{$pagina}&acao=A&aba=anexo" ));
        }

        if(!$boPopup){
            array_push($abas, array( "descricao" => "Trâmite",
                "link"	 	 => "elabrev.php?modulo=principal/termoCooperacao/{$pagina}&acao=A&aba=tramite" ));

            array_push($abas, array( "descricao" => "Gerar Termo em PDF",
                "link"	 	 => "elabrev.php?modulo=principal/termoCooperacao/{$pagina}&acao=A&aba=gerapdf" ));
        }

    }else{

        $abas = array( 0 => array(  "descricao" => "Proponente",
            "link"	  	=> "elabrev.php?modulo=principal/termoCooperacao/{$pagina}&acao=A&aba=proponente" ) );
    }

    if($_SESSION['elabrev']['tcpid']){
        $sql = "select * from monitora.termocooperacao where tcpid = '{$_SESSION['elabrev']['tcpid']}' and ungcodconcedente = '".UG_FNDE."'";
        $rsFNDE = $db->pegaLinha($sql);
    }else{
        $rsFNDE = false;
    }

    if( (in_array(PERFIL_CGSO, $perfis) ||
            in_array(PERFIL_SUPER_USUARIO, $perfis)) &&
        in_array($estadoAtual, array(EM_DESCENTRALIZACAO, EM_EMISSAO_NOTA_CREDITO)) &&
        $rsFNDE){
        array_push($abas, array( "descricao" => "Enviar Nota de Crédito",
            "link"	 	 => "elabrev.php?modulo=principal/termoCooperacao/{$pagina}&acao=A&aba=enviarNCfnde" ));
    }

    if(strstr($_SESSION['favurl'], '&ungcod=')){
        $arTemp = explode('&ungcod', $_SESSION['favurl']);
        $_SESSION['favurl'] = $arTemp[0];
    }

    if ($_SESSION['elabrev']['tcpid']) {

        $historicoAlteracao = $db->pegaUm("
            select count(*) from workflow.historicodocumento hst
            where
            hst.aedid = ".WF_ACAO_SOL_ALTERACAO." and hst.docid = (
                select docid from monitora.termocooperacao where tcpid = {$_SESSION['elabrev']['tcpid']}
            )
        ");

        /**
         * Quando o relatório de cumprimento do termo em questão
         * está com o prazo de preechimento vencido, a aba relatório de cumprimento do objeto
         * deve aparecer para o uo equipe tecnica do proponente
         */
        if (termoVencido($_SESSION['elabrev']['tcpid'])
            && (in_array(PERFIL_SUPER_USUARIO, $perfis) || verificaEquipeTecnicaProponente())
            && $historicoAlteracao) {
            if (!existeAba($abas, 'Relatório de Cumprimento')) {
                array_unshift($abas, array(
                    "descricao"=> "Relatório de Cumprimento",
                    "link" => "elabrev.php?modulo=principal/termoCooperacao/{$pagina}&acao=A&aba=cumprimento"
                ));
            }
        }
    }

    echo montarAbasArray($abas, $_SESSION['favurl']);
}

/**
 * @param $arrayAba
 * @param $legenda
 * @return bool
 */
function existeAba($arrayAba, $legenda) {
    foreach ($arrayAba as $aba) {
        if (in_array($legenda, $aba))
            return true;
    }
    return false;
}

function montaAbaTermoCooperacaoListaUGs()
{
    $abas = array();
    $perfis = pegaPerfilGeral($_SESSION['usucpf'], $_SESSION['sisid']);

    array_push($abas, array( "descricao" => "Lista de UG",
        "link"	  	 => "elabrev.php?modulo=principal/termoCooperacao/listaUG&acao=A" ));

    if(in_array(PERFIL_SUPER_USUARIO, $perfis) || in_array(UO_CONSULTA_ORCAMENTO, $perfis)){
        array_push($abas, array( "descricao" => "Lista de Termos de Execução Descentralizada",
            "link"	  	 => "elabrev.php?modulo=principal/termoCooperacao/listaTermoCooperacaoGeral&acao=A" ));
    }

    if(in_array(PERFIL_SUPER_USUARIO, $perfis) ||
        in_array(PERFIL_CGSO, $perfis) ||
        in_array(PERFIL_DIRETORIA, $perfis) ||
        in_array(PERFIL_SECRETARIO, $perfis) ||
        in_array(PERFIL_SUBSECRETARIO, $perfis)
    ){
        array_push($abas, array( "descricao" => "Tramita Termos de Execução Descentralizada em Lote",
            "link"	  	 => "elabrev.php?modulo=principal/termoCooperacao/tramitaTermos&acao=A" ));

        array_push($abas, array( "descricao" => "Lista Guias de Tramitação",
            "link"	  	 => "elabrev.php?modulo=principal/termoCooperacao/listaGuiasTramitacao&acao=A" ));
    }

    echo montarAbasArray($abas, $_SESSION['favurl']);
}

function montaAbaTermoCooperacaoListaTermos()
{
    $abas = array();
    $perfis = pegaPerfilGeral($_SESSION['usucpf'], $_SESSION['sisid']);

    if(in_array(PERFIL_SUPER_USUARIO, $perfis) || is_array($_SESSION['elabrev']['ungcod'])){
        array_push($abas, array( "descricao" => "Lista de UG",
            "link"	  	 => "elabrev.php?modulo=principal/termoCooperacao/listaUG&acao=A" ));
    }

    array_push($abas, array( "descricao" => "Lista de Termos de Execução Descentralizada",
        "link"	  	 => "elabrev.php?modulo=principal/termoCooperacao/listaTermoCooperacao&acao=A&ungcod={$_SESSION['elabrev']['ungcodlista']}" ));


    echo montarAbasArray($abas, $_SESSION['favurl']);
}

function atualizaComboMunicipio( $request )
{
    global $db;

    extract($request);

    if( $estuf != '' ) $whereMuncod = " WHERE estuf = '".$estuf."' ";

    $sql = "SELECT
				muncod as codigo,
				mundescricao||' - '||estuf as descricao
			FROM
				territorios.municipio
			$whereMuncod
			ORDER BY
				2";
    echo $db->monta_combo('muncod',$sql, 'S','Selecione...','','','',200,'N', 'muncod', '', '', '');
}

//ANTIGOS
function mostraUg( $request ){

    global $db;

    extract($request);

    $sql = "SELECT
				ung.ungcod,
				ungcnpj,
				ungendereco,
				mun.muncod,
				estuf,
				ungfone
			FROM
				public.unidadegestora ung
			LEFT JOIN territorios.municipio mun ON mun.muncod = ung.muncod
			WHERE
				ungcod = '".$ungcod."'";
    $dados = $db->pegaLinha($sql);

    if( $dados['estuf'] != '' ) $whereMuncod = " WHERE estuf = '".$dados['estuf']."' ";
    $sql_uf  = "SELECT
					estuf as codigo,
					estdescricao as descricao
				FROM
					territorios.estado
				ORDER BY
					2";

    $sql_mun = "SELECT
					muncod as codigo,
					mundescricao||' - '||estuf as descricao
				FROM
					territorios.municipio
				$whereMuncod
				ORDER BY
					estuf,2";
    ?>
    <div>
        <img src="/imagens/excluir_2.gif" align="absmiddle" title="Excluir" class="excluir_ug" id="<?=$tipo ?>" style="float:left;margin-left:5px;position:relative;cursor:pointer"/>
        <div style="float:left;margin-left:5px;">
            <b>Cód. Unid. Gestora: </b><?=$dados['ungcod']?><input type="hidden" name="ungcod_<?=$tipo ?>" value="<?=$dados['ungcod']?>"/></div>
        <div style="float:left;margin-left:5px;">
            <b>Cód. Gestão: </b>?????</div>
        <div style="float:left;margin-left:5px;">
            <b>CNPJ: </b><?=campo_texto('ungcnpj_'.$tipo,'N','S','',30,30,'[#]','', '', '', '', '', '', $dados['ungcnpj']); ?>&nbsp;</div>
        <div style="margin-left:5px;">
            <b>Razão Social: </b>?????</div>
        </br>
        <div style="float:left;margin-left:5px;">
            <b>Endereço: </b><?=campo_texto('ungendereco_'.$tipo,'N','S','',100,200,'[#]','', '', '', '', '', '', $dados['ungendereco']); ?></div>
        <div style="float:left;margin-left:5px;">
            <b>Bairro ou Distrito: </b>?????&nbsp;</div>
        <div id="<?=($tipo == 'P' ? 'div_municipio_ugp' : 'div_municipio_ugc' ) ?>">
            <b>Município: </b><?=$db->monta_combo('muncod_'.$tipo,$sql_mun,'S','Selecione...','','','',200,'N', '', '', $dados['muncod'], ''); ?></div></br>
        <div style="float:left;margin-left:42px;">
            <b>UF: </b><?=$db->monta_combo('estuf_'.$tipo,$sql_uf,'S','Selecione...','atualizaComboMunicipio','','',200,'N', $tipo, '', $dados['estuf'], ''); ?></div>
        <div style="float:left;margin-left:5px;">
            <b>CEP: </b>?????</div>
        <div style="float:left;margin-left:5px;">
            <b>Telefone: </b><?=campo_texto('ungfone_'.$tipo,'N','S','',100,200,'(###) ####-####','', '', '', '', '', '', $dados['ungfone']); ?></div>
        <div style="float:left;margin-left:5px;">
            <b>Fax: </b>?????</div>
        <div style="margin-left:5px;">
            <b>E-mail: </b>?????</div>
    </div>
<?php
}
function lista_unidadeGestora( $tipo ){

    global $db;

    $sql = "SELECT
				'<input type=\"radio\" class=\"ug\" name=\"ungcod\" value=\"'|| ungcod ||'\"/>' as acao,
				ungcod,
				ungabrev||' - '||ungdsc as descricao
			FROM
				public.unidadegestora
			WHERE
				ungstatus = 'A'
			ORDER BY
				2";
    $cabecalho = Array("&nbsp;","Numero da</br> Unidade Gestora","Unidade Gestora");
    $db->monta_lista_simples($sql,$cabecalho,50,10,'N','95%','N','','','','');
}

function recupera_ugp( $request ){

    global $db;

    echo "<label style=\"font-size:15px;\"><center><b>Lista de Unidades Gestoras</b></center></label></br>";
    echo "<div style=\"overflow:auto;height:85%;\" >";
    lista_unidadeGestora( 'P' );
    echo "</div></br>";
    echo "<input type=\"button\" value=\"Definir Unidade Gestora Proponente\" class=\"define_ug\" id=\"P\"/>";
}
function recupera_rugp( $request ){
    echo 'teste_rugp';
}
function recupera_ugc( $request ){

    echo "<label style=\"font-size:15px;\"><center><b>Lista de Unidades Gestoras</b></center></label>";
    echo 'teste_ugc';
}
function recupera_rugc( $request ){
    echo 'teste_rugc';
}

/** Formata o valor numeric para ser inserido no banco
 * @name formata_valor_sql
 * @author Luciano F. Ribeiro
 * @access public
 * @return float
 */
function formata_valor_sql($valor){
    $valor = str_replace('.', '', $valor);
    $valor = str_replace(',', '.', $valor);
    return $valor;
}

function formataDataBanco($valor){
    $data = explode("/",$valor);
    $dia = $data[0];
    $mes = $data[1];
    $ano = $data[2];
    return $ano."-".$mes."-".$dia;
}

function buscarRepreProponente($ungcod){
    global $db;

    if($ungcod != ''){
        $sql = "
			Select	rpuid
			From elabrev.usuarioresponsabilidade
			Where rpustatus = 'A' and ungcod = '".$ungcod."' and pflcod = ".PERFIL_REITOR;
// 		$repProp = $db->pegaLinha($sql);

        $sql = "SELECT DISTINCT
					fun.funid,
					fun.fundsc as pfldsc,
					ent.entnome as usunome,
					ent.entnumcpfcnpj as usucpf,
					ent.entemail as usuemail,
					ent.entid,
					ent.entstatus as rpustatus,
					ent.entnumcpfcnpj as usucpf,
					ent.entid as rpuid
				FROM entidade.funcao fun
				LEFT JOIN entidade.funcaoentidade fen ON fen.funid = fun.funid
				AND fen.entid IN (SELECT
									fen2.entid
									FROM entidade.funentassoc fea2

									JOIN entidade.entidade ent2 on ent2.entid = fea2.entid
									JOIN unidadegestora ung2 on ent2.entunicod = ung2.unicod

									LEFT JOIN entidade.funcaoentidade fen2 on fea2.fueid = fen2.fueid
									WHERE
									--fea2.entid='388679'
									ung2.ungcod = '{$ungcod}'
									AND fun.funid = fen2.funid)
				LEFT JOIN entidade.entidade ent ON fen.entid = ent.entid
				WHERE fun.funid IN('21');";

        $repProp = $db->pegaLinha($sql);
    }

    return $repProp;
}

function buscarRepreConcedente($ungcod){
    global $db;

    if($_SESSION['elabrev']['tcpid'] != ''){
        $sql = "
			Select	usucpfconcedente
			From monitora.termocooperacao
			Where ungcodconcedente = '".$ungcod."' and tcpid = ".$_SESSION['elabrev']['tcpid'];
        $repConc = $db->pegaLinha($sql);
    }

    return $repConc;
}

function buscarDadosUndProp( $tcpid = null ){
    global $db;

    if( $tcpid == null ){
        $tcpid = $_SESSION['elabrev']['tcpid'];
    }else{
        $tcpid = $tcpid;
    }

    $sql = "Select ungcodproponente From monitora.termocooperacao where tcpid = ".$tcpid;
    $ungcod = $db->pegaUm($sql);

    $sql = "
		Select 	ungcod,
				ungcnpj,
				ungdsc,
				ungendereco,
				ungbairro,
				estuf,
				mun.muncod,
				ungcep,
				ungfone,
				ungemail,
				gescod
		From public.unidadegestora ung
		Left Join territorios.municipio mun on mun.muncod = ung.muncod
		Where ungstatus = 'A' and ungcod = '".$ungcod."'
	";
    return $db->pegaLinha($sql);
}

function buscarDadosUndConc( $tcpid = null ){
    global $db;

    if( $tcpid == null ){
        $tcpid = $_SESSION['elabrev']['tcpid'];
    }else{
        $tcpid = $tcpid;
    }

    $sql = "Select ungcodconcedente From monitora.termocooperacao where tcpid = ".$tcpid;
    $ungcod = $db->pegaUm($sql);

    $sql = "
		Select 	ungcod,
				ungcnpj,
				ungdsc,
				ungendereco,
				ungbairro,
				estuf,
				mun.muncod,
				ungcep,
				ungfone,
				ungemail,
				gescod
		From public.unidadegestora ung
		Left Join territorios.municipio mun on mun.muncod = ung.muncod
		Where ungstatus = 'A' and ungcod = '".$ungcod."'
	";
    return $db->pegaLinha($sql);
}

function buscaResponsavelProp( $tcpid = null ){
    global $db;

    if( $tcpid == null ){
        $tcpid = $_SESSION['elabrev']['tcpid'];
    }else{
        $tcpid = $tcpid;
    }

    $sql = "Select ungcodproponente From monitora.termocooperacao where tcpid = ".$tcpid;
    $ungcod = $db->pegaUm($sql);

    $sql = "
		Select 	distinct Coalesce( ur.usucpf, 'Não informado')  as usucpf,
				Coalesce( usu.usunome, 'Não informado')  as usunome,
				Coalesce( ed.endlog, 'Não informado')  as endereco,
				Coalesce( ed.endbai, 'Não informado')  as bairro,
				Coalesce( m.mundescricao, 'Não informado')  as municipio,
				Coalesce( ed.estuf, 'Não informado')  as estado,
				Coalesce( ed.endcep, 'Não informado')  as endcep,
				Coalesce( usu.usufoneddd||'-'||usufonenum, 'Não informado')  as fone,
				Coalesce( et.entnumrg, 'Não informado') as numeroidentidade,
				Coalesce( et.entorgaoexpedidor, 'Não informado')  as entorgaoexpedidor,
				Coalesce( usu.usufuncao, 'Não informado')  as usufuncao,
				Coalesce( usu.usuemail, 'Não informado')  as usuemail
		From elabrev.usuarioresponsabilidade ur
		Left Join seguranca.usuario usu on usu.usucpf = ur.usucpf
		Left Join seguranca.perfil p on p.pflcod = ur.pflcod
		Left Join entidade.entidade et on et.entid = usu.entid
		Left Join entidade.endereco ed on ed.entid = et.entid
		Left Join territorios.municipio m on m.muncod = ed.muncod
		Where ur.rpustatus = 'A' and ur.ungcod = '".$ungcod."' and p.pflcod = ".PERFIL_REITOR."
	";
    $propon = $db->pegaLinha($sql);
    return $propon;
}

function recuperarResponsavelProponente($tcpid = null)
{
    global $db;

    if (!$tcpid) {
        $tcpid = $_SESSION['elabrev']['tcpid'];
    }

    if ($tcpid) {

        $sql = "Select ungcodproponente From monitora.termocooperacao where tcpid = ".$tcpid;
        $ungcod = $db->pegaUm($sql);

        if ($ungcod) {

            $sql = "SELECT DISTINCT
                            COALESCE( rpl.cpf, 'Não informado') AS usucpf,
                            COALESCE( usu.usunome, 'Não informado') AS usunome,
                            COALESCE( edu.endlog, COALESCE( ed.endlog, 'Não informado') ) AS endereco,
                            COALESCE( edu.endbai, COALESCE( ed.endbai, 'Não informado') ) AS bairro,
                            COALESCE( m.mundescricao, 'Não informado') AS municipio,
                            COALESCE( edu.estuf, COALESCE( ed.estuf, 'Não informado') ) AS estado,
                            COALESCE( edu.endcep, COALESCE( ed.endcep, 'Não informado') ) AS endcep,
                            COALESCE( usu.usufoneddd||'-'||usufonenum, 'Não informado') AS fone,
                            COALESCE( etu.entnumrg, COALESCE( et.entnumrg, 'Não informado') ) AS numeroidentidade,
                            COALESCE( etu.entorgaoexpedidor, COALESCE( et.entorgaoexpedidor, 'Não informado') ) AS entorgaoexpedidor,
                            COALESCE( usu.usufuncao, 'Não informado') AS usufuncao,
                            COALESCE( usu.usuemail, 'Não informado') AS usuemail
                        FROM
                            elabrev.representantelegal rpl
                        LEFT JOIN seguranca.usuario usu ON usu.usucpf = rpl.cpf
                        LEFT JOIN public.unidadegestora ung ON ung.ungcod = rpl.ug
                        LEFT JOIN entidade.entidade et ON et.entid = usu.entid
                        LEFT JOIN entidade.endereco ed ON ed.entid = et.entid
                        LEFT JOIN entidade.entidade etu ON etu.entnumcpfcnpj = usu.usucpf
                        LEFT JOIN entidade.endereco edu ON edu.entid = etu.entid
                        LEFT JOIN territorios.municipio m ON m.muncod = edu.muncod
                        WHERE rpl.ug = '{$ungcod}'";
            return $db->pegaLinha($sql);
        } else {

            $sql = "SELECT
                    hd.hstid,
                    COALESCE( us.usucpf, 'Não informado') AS usucpf
                    , COALESCE( us.usunome, 'Não informado') AS usunome
                    , COALESCE( us.usuemail, 'Não informado') AS usuemail
                    , COALESCE( edu.endlog, COALESCE( ende.endlog, 'Não informado') ) AS endereco
                    , COALESCE( edu.endbai, COALESCE( ende.endbai, 'Não informado') ) AS bairro
                    , COALESCE( m.mundescricao, 'Não informado') AS municipio
                    , COALESCE( edu.estuf, COALESCE( ende.estuf, 'Não informado') ) AS estado
                    , COALESCE( edu.endcep, COALESCE( ende.endcep, 'Não informado') ) AS endcep
                    , COALESCE( us.usufoneddd||'-'||usufonenum, 'Não informado') AS fone
                    , COALESCE( etu.entnumrg, COALESCE( et.entnumrg, 'Não informado') ) AS numeroidentidade
                    , COALESCE( etu.entorgaoexpedidor, COALESCE( et.entorgaoexpedidor, 'Não informado') ) AS entorgaoexpedidor
                    , COALESCE( us.usufuncao, 'Não informado') AS usufuncao

                FROM workflow.historicodocumento hd
                    INNER JOIN workflow.acaoestadodoc ac on ac.aedid = hd.aedid
                    INNER JOIN workflow.estadodocumento ed on ed.esdid = ac.esdidorigem
                    INNER JOIN seguranca.usuario us on us.usucpf = hd.usucpf
                    LEFT JOIN workflow.comentariodocumento cd on cd.hstid = hd.hstid
                    LEFT JOIN entidade.entidade et ON et.entid = us.entid
                    LEFT JOIN entidade.endereco ende ON ende.entid = et.entid

                    LEFT JOIN entidade.entidade etu ON etu.entnumcpfcnpj = us.usucpf
                    LEFT JOIN entidade.endereco edu ON edu.entid = etu.entid

                    LEFT JOIN territorios.municipio m ON m.muncod = edu.muncod
            where
                hd.docid = (select docid from monitora.termocooperacao where tcpid = {$tcpid})
                and ac.esdidorigem = " . EM_APROVACAO_DA_REITORIA . " -- Representante legal do proponente / Aguardando aprovacao do proponente
                and ac.esdiddestino = " . EM_ANALISE_DA_SECRETARIA . " -- Em analise do Gabinete da secretaria/autarquia
            ORDER BY hd.hstid";

            $representante = $db->pegaLinha($sql);

            $sql = "
                select
                    hd.hstid,
                    us.usucpf,
                    us.usunome,
                    us.usuemail
                from workflow.historicodocumento hd
                inner join workflow.acaoestadodoc ac on
                    ac.aedid = hd.aedid
                inner join workflow.estadodocumento ed on
                    ed.esdid = ac.esdidorigem
                inner join seguranca.usuario us on
                    us.usucpf = hd.usucpf
                left join workflow.comentariodocumento cd on
                    cd.hstid = hd.hstid
                where
                    hd.docid = (select docid from monitora.termocooperacao where tcpid = {$tcpid})
                and ac.esdidorigem = " . EM_EXECUCAO . " -- Em execução
                and ac.esdiddestino = " . ALTERAR_TERMO_COOPERACAO . " -- Solicitação de alteração
            ";
            $solicitacao = $db->pegaLinha($sql); //solicitação de alteração

            if ($representante && ($representante['hstid'] > $solicitacao['hstid'] || empty($representante['hstid']))) {
                return $representante;
            }
        }
    }
    return array();
}


function buscaResponsavelCons( $tcpid = null ){
    global $db;

    if( $tcpid == null ){
        $tcpid = $_SESSION['elabrev']['tcpid'];
    }else{
        $tcpid = $tcpid;
    }

    $sql = "Select ungcodconcedente From monitora.termocooperacao where tcpid = ".$tcpid;
    $ungcod = $db->pegaUm($sql);

    $sql = "
		Select 	distinct Coalesce( ur.usucpf, 'Não informado')  as usucpf,
				Coalesce( usu.usunome, 'Não informado')  as usunome,
				Coalesce( ed.endlog, 'Não informado')  as endereco,
				Coalesce( ed.endbai, 'Não informado')  as bairro,
				Coalesce( m.mundescricao, 'Não informado')  as municipio,
				Coalesce( ed.estuf, 'Não informado')  as estado,
				Coalesce( ed.endcep, 'Não informado')  as endcep,
				Coalesce( usu.usufoneddd||'-'||usufonenum, 'Não informado')  as fone,
				Coalesce( et.entnumrg, 'Não informado') as numeroidentidade,
				Coalesce( et.entorgaoexpedidor, 'Não informado')  as entorgaoexpedidor,
				Case when usu.usufuncao = '' or usu.usufuncao is null then 'Não informado' else usu.usufuncao end as usufuncao,
				Coalesce( usu.usuemail, 'Não informado')  as usuemail
		From elabrev.usuarioresponsabilidade ur
		Left Join seguranca.usuario usu on usu.usucpf = ur.usucpf
		Left Join seguranca.perfil p on p.pflcod = ur.pflcod
		Left Join entidade.entidade et on et.entid = usu.entid
		Left Join entidade.endereco ed on ed.entid = et.entid
		Left Join territorios.municipio m on m.muncod = ed.muncod
		Where ur.rpustatus = 'A' and ur.ungcod = '".$ungcod."' and p.pflcod = ".PERFIL_SECRETARIO."
	";
    $conced = $db->pegaLinha($sql);
    return $conced;
}

function recuperarResponsavelConcedente($tcpid = null)
{
    global $db;

    if( $tcpid == null ){
        $tcpid = $_SESSION['elabrev']['tcpid'];
    }else{
        $tcpid = $tcpid;
    }


    if($tcpid){

        $sql = "SELECT
                            COALESCE( us.usucpf, 'Não informado') AS usucpf
                            , COALESCE( us.usunome, 'Não informado') AS usunome
                            , COALESCE( us.usuemail, 'Não informado') AS usuemail
                            , COALESCE( edu.endlog, COALESCE( ende.endlog, 'Não informado') ) AS endereco
                            , COALESCE( edu.endbai, COALESCE( ende.endbai, 'Não informado') ) AS bairro
                            , COALESCE( m.mundescricao, 'Não informado') AS municipio
                            , COALESCE( edu.estuf, COALESCE( ende.estuf, 'Não informado') ) AS estado
                            , COALESCE( edu.endcep, COALESCE( ende.endcep, 'Não informado') ) AS endcep
                            , COALESCE( us.usufoneddd||'-'||usufonenum, 'Não informado') AS fone
                            , COALESCE( etu.entnumrg, COALESCE( et.entnumrg, 'Não informado') ) AS numeroidentidade
                            , COALESCE( etu.entorgaoexpedidor, COALESCE( et.entorgaoexpedidor, 'Não informado') ) AS entorgaoexpedidor
                            , COALESCE( us.usufuncao, 'Não informado') AS usufuncao

                        FROM workflow.historicodocumento hd
                            INNER JOIN workflow.acaoestadodoc ac on ac.aedid = hd.aedid
                            INNER JOIN workflow.estadodocumento ed on ed.esdid = ac.esdidorigem
                            INNER JOIN seguranca.usuario us on us.usucpf = hd.usucpf
                            LEFT JOIN workflow.comentariodocumento cd on cd.hstid = hd.hstid
                            LEFT JOIN entidade.entidade et ON et.entid = us.entid
                            LEFT JOIN entidade.endereco ende ON ende.entid = et.entid

                            LEFT JOIN entidade.entidade etu ON etu.entnumcpfcnpj = us.usucpf
                            LEFT JOIN entidade.endereco edu ON edu.entid = etu.entid

                            LEFT JOIN territorios.municipio m ON m.muncod = edu.muncod
				where
					hd.docid = (select docid from monitora.termocooperacao where tcpid = {$tcpid})
				and ac.esdidorigem = " . AGUARDANDO_APROVACAO_SECRETARIO . " -- Representante legal do proponente / Aguardando aprovacao do proponente
                                and ac.esdiddestino = " . EM_ANALISE_PELA_CGSO . " -- Em analise do Gabinete da secretaria/autarquia
                            ORDER BY hd.hstid";
        $representante = $db->pegaLinha($sql);
        if($representante){

            return $representante;

        } else {

            $sql = "Select ungcodconcedente From monitora.termocooperacao where tcpid = ".$tcpid;
            $ungcod = $db->pegaUm($sql);

            if($ungcod){
                $sql = "SELECT DISTINCT
                                            COALESCE( rpl.cpf, 'Não informado') AS usucpf,
                                            COALESCE( usu.usunome, 'Não informado') AS usunome,
                                            COALESCE( edu.endlog, COALESCE( ed.endlog, 'Não informado') ) AS endereco,
                                            COALESCE( edu.endbai, COALESCE( ed.endbai, 'Não informado') ) AS bairro,
                                            COALESCE( m.mundescricao, 'Não informado') AS municipio,
                                            COALESCE( edu.estuf, COALESCE( ed.estuf, 'Não informado') ) AS estado,
                                            COALESCE( edu.endcep, COALESCE( ed.endcep, 'Não informado') ) AS endcep,
                                            COALESCE( usu.usufoneddd||'-'||usufonenum, 'Não informado') AS fone,
                                            COALESCE( etu.entnumrg, COALESCE( et.entnumrg, 'Não informado') ) AS numeroidentidade,
                                            COALESCE( etu.entorgaoexpedidor, COALESCE( et.entorgaoexpedidor, 'Não informado') ) AS entorgaoexpedidor,
                                            COALESCE( usu.usufuncao, 'Não informado') AS usufuncao,
                                            COALESCE( usu.usuemail, 'Não informado') AS usuemail
                                    FROM
                                            elabrev.representantelegal rpl
                                    LEFT JOIN seguranca.usuario usu ON usu.usucpf = rpl.cpf
                                    LEFT JOIN public.unidadegestora ung ON ung.ungcod = rpl.ug
                                    LEFT JOIN entidade.entidade et ON et.entid = usu.entid
                                    LEFT JOIN entidade.endereco ed ON ed.entid = et.entid

                                    LEFT JOIN entidade.entidade etu ON etu.entnumcpfcnpj = usu.usucpf
                                    LEFT JOIN entidade.endereco edu ON edu.entid = etu.entid

                                    LEFT JOIN territorios.municipio m ON m.muncod = ed.muncod
                                    WHERE rpl.ug = '{$ungcod}'";

                return $db->pegaLinha($sql);
            }
        }
        return array();
    }
}

function buscarObjetoTermo( $tcpid = null ){
    global $db;

    if( $tcpid == null ){
        $tcpid = $_SESSION['elabrev']['tcpid'];
    }else{
        $tcpid = $tcpid;
    }

    $sql = "
		Select 	distinct Coalesce( tcpdscobjetoidentificacao, 'Não informado')  as tcpdscobjetoidentificacao,
				Coalesce( tcpobjetivoobjeto, 'Não informado')  as tcpobjetivoobjeto,
				Coalesce( tcpobjetojustificativa, 'Não informado')  as tcpobjetojustificativa,
				ungcodpoliticafnde,
				ungcodconcedente,
				tcpnumprocessofnde
		From monitora.termocooperacao
		Where tcpid = ".$tcpid;
    $objeto = $db->pegaLinha($sql);
    return $objeto;
}

function buscarPrevisaoOrca($tcpid = null) {
    global $db;

    if( $tcpid == null ){
        $tcpid = $_SESSION['elabrev']['tcpid'];
    }else{
        $tcpid = $tcpid;
    }

    $sql = "
		Select distinct
		        pro.proid,
		        ptres||'-'|| p.funcod||'.'||p.sfucod||'.'||p.prgcod||'.'||p.acacod||'.'||p.unicod||'.'||p.loccod as plano_trabalho,
				a.acacod as acao,
				crdmesliberacao,
				proanoreferencia,
				case when a.acatitulo is not null then substr(a.acatitulo, 1, 70)||'...' else substr(a.acadsc, 1, 70)||'...' end as acao_loa,
				substr(pi.plicod||' - '||pi.plidsc, 1, 45)||'...' as plano_interno,
				substr(ndp.ndpcod, 1, 6) || ' - ' || ndp.ndpdsc as nat_despesa,
				to_char(pro.provalor, '999G999G999G999G999D99') as provalor,
				-- pro.provalor as valor
				case
                        when (select sum(cr.valor) from elabrev.creditoremanejado cr where cr.proid = pro.proid) IS NOT NULL then
                            to_char(pro.provalor - (select sum(cr.valor) from elabrev.creditoremanejado cr where cr.proid = pro.proid), '999G999G999G999G999D99')
                        else
                            to_char(pro.provalor, '999G999G999G999G999D99')
                        end as valor_f,
                case
                    when (select sum(cr.valor) from elabrev.creditoremanejado cr where cr.proid = pro.proid) IS NOT NULL then
                        pro.provalor - (select sum(cr.valor) from elabrev.creditoremanejado cr where cr.proid = pro.proid)
                    else
                        pro.provalor
                    end as valor,
                to_char((select sum(vTable.valor) from (
                    Select
                        case
                            when (select sum(cr.valor) from elabrev.creditoremanejado cr where cr.proid = pro.proid) IS NOT NULL then
                              pro.provalor - (select sum(cr.valor) from elabrev.creditoremanejado cr where cr.proid = pro.proid)
                            else
                              pro.provalor
                            end as valor
                    From monitora.previsaoorcamentaria pro where prostatus = 'A'
                    and tcpid = {$tcpid}
                ) as vTable), '999G999G999G999G999D99') as total,
                pro.proid
		From monitora.previsaoorcamentaria pro

		Left Join monitora.pi_planointerno pi on pi.pliid = pro.pliid
		Left Join monitora.pi_planointernoptres pts on pts.pliid = pi.pliid
		Left Join public.naturezadespesa ndp on ndp.ndpid = pro.ndpid

		Left Join monitora.ptres p on p.ptrid = pro.ptrid
		Left Join monitora.acao a on a.acaid = p.acaid
		Left Join public.unidadegestora u on u.unicod = p.unicod
		Left Join monitora.pi_planointernoptres pt on pt.ptrid = p.ptrid

		Where pro.prostatus = 'A' and pro.tcpid = ".$tcpid."
		ORDER BY proanoreferencia, crdmesliberacao
		";
    //ver($sql, d);
    $previsao = $db->carregar($sql);
    return $previsao;
}

function buscarCronograma(){
    global $db;

    $sql = "
		Select 	crdparcela,
				c.acaid as acaid,
			Case
				when crdmesliberacao = '1' then 'Janeiro'
				when crdmesliberacao = '2' then 'fevereiro'
				when crdmesliberacao = '3' then 'Março'
				when crdmesliberacao = '4' then 'Abril'
				when crdmesliberacao = '5' then 'Maio'
				when crdmesliberacao = '6' then 'Junho'
				when crdmesliberacao = '7' then 'Julho'
				when crdmesliberacao = '8' then 'Agosto'
				when crdmesliberacao = '9' then 'Setembro'
				when crdmesliberacao = '10' then 'Outubro'
				when crdmesliberacao = '11' then 'Novembro'
				when crdmesliberacao = '12' then 'Dezembro'
			end as crdmesliberacao,
				to_char(crdvalor, '999G999G999G999G999D99') as crdvalor,
				crdmesexecucao,
				a.acacod||'-'||coalesce(a.acatitulo,'_') as acaid_descricao,
				to_char(t.total, '999G999G999G999G999D99') as total
		From monitora.cronogramadesembolso c
		Left Join monitora.acao a on a.acaid = c.acaid

		Join (
			Select	sum(crdvalor) as total,
				tcpid
			From monitora.cronogramadesembolso
			Where crdstatus = 'A'
			Group by tcpid
		) as t on t.tcpid = c.tcpid

		Where crdstatus = 'A' and c.tcpid = ".$_SESSION['elabrev']['tcpid']."
		Order by crdid
	";
    $cronograma = $db->carregar($sql);
    return $cronograma;
}

function verificaEnvioDiligencia()
{
    return false;
}

function enviarParaCoordenacao($tcpid = null)
{
    global $db;

    $tcpid = $tcpid ? $tcpid : $_SESSION['elabrev']['tcpid'];
    if($tcpid){
        $sql = "select cooid from monitora.termocooperacao where tcpid = {$tcpid}";
        $cooid = $db->pegaUm($sql);

        $sql = "select count(*) as total from
				monitora.termocooperacao t
				inner join elabrev.coordenacao c on c.ungcodconcedente = t.ungcodconcedente
				where t.tcpid = {$tcpid}";

        $dado = $db->pegaUm($sql);

        if($cooid>0 || $dado==0){
            return true;
        }
    }
    return false;
}

function verificaTermoDiligencia($tcpid = null)
{
    global $db;

    $tcpid = $tcpid ? $tcpid : $_SESSION['elabrev']['tcpid'];
    if($tcpid){
        $sql = "select docid from monitora.termocooperacao where tcpid = {$tcpid}";
        $docid = $db->pegaUm($sql);
    }

    $sql = "select hstid from workflow.historicodocumento h
			inner join workflow.acaoestadodoc a on a.aedid = h.aedid
			where h.docid = {$docid}
			and a.esdiddestino in ( ".EM_DILIGENCIA." ) limit 1 ";

    $rs = $db->pegaUm($sql);

    if($rs){
        return true;
    }
    return false;
}

function verificaTermoSemDiligencia($tcpid = null)
{
    global $db;

    $tcpid = $tcpid ? $tcpid : $_SESSION['elabrev']['tcpid'];
    if($tcpid){
        $sql = "select docid from monitora.termocooperacao where tcpid = {$tcpid}";
        $docid = $db->pegaUm($sql);
    }

    $sql = "select hstid from workflow.historicodocumento h
			inner join workflow.acaoestadodoc a on a.aedid = h.aedid
			where h.docid = {$docid}
			and a.esdiddestino in ( ".EM_DILIGENCIA." ) limit 1 ";

    $rs = $db->pegaUm($sql);

    if($rs){
        return false;
    }
    return true;
}

function verificaTermoAlteracao($tcpid = null)
{
    global $db;

    $tcpid = $tcpid ? $tcpid : $_SESSION['elabrev']['tcpid'];

    if(!$tcpid){
        return false;
    }

    $return = verificaPendenciaAbaPrevisaoOrcamentaria($tcpid);
    if (count($return)) {
        foreach ($return as $linha) {
            if (empty($linha['ptrid_descricao']) ||
                empty($linha['pliid_descricao']) ||
                empty($linha['crdmesliberacao']))
            {
                return 'Verifique a pendência na Previsão Orçamentária';
            }
        }
        $previsao = true;
    }

    $linha = verificaPendenciaAbaParecer($tcpid);
    //ver($linha);
    if (count($linha)) {
        if (empty($linha['tcpconsidentproponente']) ||
            empty($linha['tcpconsidproposta']) ||
            empty($linha['tcpconsidobjeto']) ||
            empty($linha['tcpconsidobjetivo']) ||
            empty($linha['tcpconsidjustificativa']) ||
            empty($linha['tcpconsidvalores']) ||
            empty($linha['tcpconsidcabiveis']) ||
            empty($linha['tcpusucpfparecer']))
        {
            return 'Verifique a pendência no Parecer Técnico';
        }
        $parecer = true;
    }

    $sql = "select docid from monitora.termocooperacao where tcpid = {$tcpid}";
    $docid = $db->pegaUm($sql);

    $sql = "select hstid from workflow.historicodocumento h
			inner join workflow.acaoestadodoc a on a.aedid = h.aedid
			where h.docid = {$docid}
			and a.esdiddestino in ( ".ALTERAR_TERMO_COOPERACAO." ) limit 1 ";

    $rs = $db->pegaUm($sql);

    if ($rs && $previsao && $parecer) {
        return true;
    }

    return false;
}

function verificaPendenciaAbaPrevisaoOrcamentaria($tcpid) {

    global $db;

    $sql = "SELECT DISTINCT
                pro.proid,
                ptres || ' - ' || p.funcod||'.'||p.sfucod||'.'||p.prgcod||'.'||p.acacod||'.'||p.unicod||'.'||p.loccod as ptrid_descricao,
                substr(pi.plicod||' - '||pi.plidsc, 1, 45)||'...' as pliid_descricao,
                substr(ndp.ndpcod, 1, 6) || ' - ' || ndp.ndpdsc as ndp_descricao,
                pro.ptrid,
                a.acacod,
                pro.pliid,
                case when a.acatitulo is not null then substr(a.acatitulo, 1, 70)||'...' else substr(a.acadsc, 1, 70)||'...' end as acatitulo,
                pro.ndpid,
                to_char(pro.provalor, '999G999G999G999G999D99') as provalor,
                coalesce(pro.provalor, 0) as valor,
                crdmesliberacao,
                crdmesexecucao,
                pro.proid,
                pro.proanoreferencia,
                pro.prodata,
                (select ppa2.codncsiafi from elabrev.previsaoparcela ppa2 where ppa2.ppaid = (select max(ppa1.ppaid) from elabrev.previsaoparcela ppa1 where ppa1.proid = pro.proid) and ppa2.ppacancelarnc = 'f') as lote,
                pp.codsigefnc,
                pp.codncsiafi,
                tc.ungcodconcedente
            FROM monitora.previsaoorcamentaria pro
            LEFT JOIN monitora.pi_planointerno pi 		ON pi.pliid = pro.pliid
            LEFT JOIN monitora.pi_planointernoptres pts ON pts.pliid = pi.pliid
            LEFT JOIN public.naturezadespesa ndp 		ON ndp.ndpid = pro.ndpid
            LEFT JOIN monitora.ptres p 					ON p.ptrid = pro.ptrid
            LEFT JOIN monitora.acao a 					ON a.acaid = p.acaid
            LEFT JOIN public.unidadegestora u 			ON u.unicod = p.unicod
            LEFT JOIN monitora.pi_planointernoptres pt 	ON pt.ptrid = p.ptrid
            LEFT JOIN elabrev.previsaoparcela pp		ON (pp.proid = pro.proid)
            LEFT JOIN monitora.termocooperacao tc       ON (tc.tcpid = pro.tcpid)
            LEFT JOIN public.unidadegestora unc         ON (unc.ungcod = tc.ungcodconcedente)
            WHERE pro.prostatus = 'A'
            AND pro.tcpid = {$tcpid}
            ORDER BY lote, pro.proanoreferencia DESC, crdmesliberacao";
    //ver($sql,d);
    return !empty($tcpid) ? $db->carregar($sql) : array();
}

function verificaPendenciaAbaParecer($tcpid) {

    global $db;

    $sql = "select
			tcpparecertecnico,
			tcpconsidentproponente,
			tcpconsidproposta,
			tcpconsidobjeto,
			tcpconsidobjetivo,
			tcpconsidjustificativa,
			tcpconsidvalores,
			tcpconsidcabiveis,
			tcpusucpfparecer
		from
			monitora.termocooperacao
		where
			tcpid = ". $_SESSION['elabrev']['tcpid'];

    return !empty($tcpid) ? $db->pegaLinha($sql) : array();
}

/**
 * Verifica se existe pendencia no relatorio de cumprimento do objeto
 * @param null $tcpid
 * @return bool
 */
function verificar_pendencia_relatorio_cumprimento($tcpid = null) {

    $tcpid = $tcpid ? $tcpid : $_SESSION['elabrev']['tcpid'];
    if (!$tcpid) {
        return false;
    }

    $objeto = new RelatorioCumprimentoObjeto();
    $objeto->_carregaTermosVencidos();
    if ($objeto->getPendenciaTermoRelacionado() && !teste_superUser()) {
        return 'Unidade proponente com pendência em preenchimento do relatório de cumprimento do objeto.';
    }

    $return = verificaAprovacaoGestor();
    if (is_bool($return)) {
        return verificaTermoSemAlteracao($tcpid);
    } else {
        return $return;
    }
}

function verificaTermoSemAlteracao($tcpid = null) {
    global $db;

    $tcpid = $tcpid ? $tcpid : $_SESSION['elabrev']['tcpid'];
    if (!$tcpid) {
        return false;
    }

    $sql = "select docid from monitora.termocooperacao where tcpid = {$tcpid}";
    $docid = $db->pegaUm($sql);
    $estadoAtual = pegarEstadoAtual($tcpid);

    if ($estadoAtual != TERMO_AGUARDANDO_APROVACAO_GESTOR_PROP) {
        $sql = "select hstid from workflow.historicodocumento h
				inner join workflow.acaoestadodoc a on a.aedid = h.aedid
				where h.docid = {$docid}
				and a.esdiddestino in ( ".ALTERAR_TERMO_COOPERACAO." ) limit 1 ";

        if ($db->pegaUm($sql)) {
            return false;
        }
    }

    if ($estadoAtual == EM_ANALISE_OU_PENDENTE) {
        if (!verificaPreenchimentoEmAnaliseCoordenacao()) {
            return false;
        }
    }

    return true;
}

function verificaTermoVazio($tcpid = null)
{
    global $db;

    $tcpid = $tcpid ? $tcpid : $_SESSION['elabrev']['tcpid'];
    $sql = "SELECT recid FROM elabrev.relatoriocumprimento WHERE tcpid = $tcpid";

    $rs = $db->pegaUm($sql);

    if(!$rs){
        return false;
    }
    return true;
}


function verificaTermoAlteracaoDiligencia($tcpid = null)
{
    global $db;

    $tcpid = $tcpid ? $tcpid : $_SESSION['elabrev']['tcpid'];
    if($tcpid){
        $sql = "select docid from monitora.termocooperacao where tcpid = {$tcpid}";
        $docid = $db->pegaUm($sql);
    }

    $sql = "select hstid from workflow.historicodocumento h
			inner join workflow.acaoestadodoc a on a.aedid = h.aedid
			where h.docid = {$docid}
			and a.esdiddestino in ( ".ALTERAR_TERMO_COOPERACAO.", ".EM_DILIGENCIA." ) limit 1 ";

    $rs = $db->pegaUm($sql);

    if($rs){
        return true;
    }
    return false;

}

function verificaTermoSemAlteracaoDiligencia($tcpid = null)
{
    global $db;

    $tcpid = $tcpid ? $tcpid : $_SESSION['elabrev']['tcpid'];
    if($tcpid){
        $sql = "select docid from monitora.termocooperacao where tcpid = {$tcpid}";
        $docid = $db->pegaUm($sql);
    }

    $sql = "select hstid from workflow.historicodocumento h
			inner join workflow.acaoestadodoc a on a.aedid = h.aedid
			where h.docid = {$docid}
			and a.esdiddestino in ( ".ALTERAR_TERMO_COOPERACAO.",".EM_DILIGENCIA." ) limit 1 ";

    $rs = $db->pegaUm($sql);

    if($rs){
        return false;
    }
    return true;
}

function verificaPreenchimentoRelCumprimento($tcpid = null) {
    global $db;

    $perfis = pegaPerfilGeral($_SESSION['usucpf'], $_SESSION['sisid']);
    if (in_array(PERFIL_SUPER_USUARIO, $perfis)) {
        return true;
    }

    $tcpid = $tcpid ? $tcpid : $_SESSION['elabrev']['tcpid'];

    if ($tcpid) {

        $retorno = uoEquipeTecnicaProponente();
        if (is_string($retorno) && !in_array(PERFIL_COORDENADOR_SEC, $perfis)) {
            return $retorno;
        }

        $sql = "
          select rel.recid
          from elabrev.relatoriocumprimento rel
		  join elabrev.relcumprimentonc nc on nc.recid = rel.recid
		  where
		    rel.tcpid = {$tcpid} AND
		    rel.recstatus = 'A' AND
		    nc.rpustatus = 'A'
		";

        $recid = $db->pegaUm($sql);
        if($recid && in_array(UO_EQUIPE_TECNICA, $perfis)){
            return true;
        }

        //Estado 656 == "Relatório de cumprimento do objeto em análise pela Coordenação"
        $estadoAtual = _pegaEstadoAtual($tcpid);
        if ($estadoAtual == 656 && in_array(PERFIL_COORDENADOR_SEC, $perfis)) {
            return true;
        }
    }

    return 'Falta preencher o relatório de cumprimento11.';
}

/**
 * Verifica perfil que pode finalizar o termo
 * apenas coordenador secretaria atuarquia e super usuário
 * @return bool
 */
function verificaFinalizaTermo() {
    global $db;

    $perfis = pegaPerfilGeral($_SESSION['usucpf'], $_SESSION['sisid']);
    if (in_array(PERFIL_SUPER_USUARIO, $perfis) || in_array(PERFIL_COORDENADOR_SEC, $perfis)) {

        $tcpid = $tcpid ? $tcpid : $_SESSION['elabrev']['tcpid'];

        $sql = "
          select rel.recid
          from elabrev.relatoriocumprimento rel
		  join elabrev.relcumprimentonc nc on nc.recid = rel.recid
		  where
		    rel.tcpid = {$tcpid} AND
		    rel.recstatus = 'A' AND
		    nc.rpustatus = 'A'
		";
        $recid = $db->pegaUm($sql);
        return ($recid) ? true : false;
    }

    return false;
}

function verificaTermoEmCadastramento($tcpid = null) {
    global $db;

    if (possui_perfil(array(PERFIL_SUPER_USUARIO))) {
        return true;
    }

    $tcpid = $tcpid ? $tcpid : $_SESSION['elabrev']['tcpid'];
    if ($tcpid) {

        $retorno = uoEquipeTecnicaProponente();
        if (is_string($retorno)) {
            return $retorno;
        }

        $sql = "select
					t.tcpid as termo,
					(select count(proid) from monitora.previsaoorcamentaria p where p.tcpid = t.tcpid) as totalprevisao,
					(select count(arptipo) from monitora.arquivoprevorcamentaria apo where apo.tcpid = t.tcpid AND apo.arptipo = 'A') as totalanexos,
					t.ungcodproponente,
					t.ungcodconcedente,
					t.tcpdscobjetoidentificacao
				from monitora.termocooperacao t
				where t.tcpid = {$tcpid}";

        $dados = $db->pegaLinha($sql);

        $arErro = array();
        if (!$dados['ungcodproponente']) 			$arErro[] = 'Proponente(1)';
        if (!$dados['ungcodconcedente']) 			$arErro[] = 'Concedente(2)';
        if (!$dados['tcpdscobjetoidentificacao']) 	$arErro[] = 'Objeto e Justificativa da Descentralização do Crédito(3)';
        if (!$dados['totalprevisao']) 				$arErro[] = 'Previsão Orçamentária(4)';
        if (!$dados['totalanexos']) 				$aErro[] = 'Anexo(5)';

        if (empty($arErro)) {
            return true;
        } else {
            return 'É necessário preencher a(s) aba(s) '.implode(', ', $arErro);
        }
    }

    return 'Somente o UO/Equipe Técnica do Proponente pode tramitar.';
}

function testaRespUGConcedente(){

    global $db;

    if($_SESSION['elabrev']['tcpid']){

        $sql = "SELECT
					true
				FROM
					monitora.termocooperacao tcp
				INNER JOIN elabrev.usuarioresponsabilidade rpu ON (rpu.ungcod = tcp.ungcodconcedente or rpu.ungcod = tcp.ungcodpoliticafnde)
				WHERE
					pflcod = ".UO_EQUIPE_TECNICA."
				AND tcp.tcpid = {$_SESSION['elabrev']['tcpid']}
				AND usucpf = '{$_SESSION['usucpf']}'";

        $teste = $db->pegaUm($sql);

        return $teste == 't';
    }
    return false;
}

function mostraAcaoDigap()
{
    global $db;

    if($_SESSION['elabrev']['tcpid']){
        $sql = "select
					true
				from
					monitora.termocooperacao
				where
					tcpid = {$_SESSION['elabrev']['tcpid']}
				and ungcodconcedente = '".UG_FNDE."'";

        $rs = $db->pegaUm($sql);

        return $rs == 't';
    }
    return false;

}

function mostraAcaoConcedenteDigap()
{
    global $db;

    if($_SESSION['elabrev']['tcpid']){
        $sql = "select
					true
				from
					monitora.termocooperacao
				where
					tcpid = {$_SESSION['elabrev']['tcpid']}
				and ungcodconcedente = '".UG_FNDE."'";

        $rs = $db->pegaUm($sql);

        if($rs == 't'){
            return false;
        }
    }
    return true;

}

function verificaPerfilDigap()
{
    global $db;

    $perfis = pegaPerfilGeral($_SESSION['usucpf'], $_SESSION['sisid']);

    if(in_array(PERFIL_ANALISTA_DIGAP, $perfis)){
        return false;
    }
    return true;
}

/*function verificaCodigoTermoSessao()
{
	if(!$_SESSION['elabrev']['tcpid']){
		echo "<script>
				alert('Não foi possível encontrar o código do termo, tente novamente.');
				document.location.href = 'elabrev.php?modulo=principal/termoCooperacao/listaTermoCooperacao&acao=A';
			</script>";
	}
}*/
function verificaCodigoTermoSessao()
{
    if(!$_SESSION['elabrev']['tcpid']){
        alertlocation(array(
            'alert' => 'Não foi possível encontrar o código do termo, tente novamente.'
        ,   'location' => 'elabrev.php?modulo=principal/termoCooperacao/listaTermoCooperacao&acao=A'
        ));
    }

}

function alertlocation($dados) {

    die("<script>
		".(($dados['alert'])?"alert('".$dados['alert']."');":"")."
		".(($dados['location'])?"window.location='".$dados['location']."';":"")."
		".(($dados['javascript'])?$dados['javascript']:"")."
		 </script>");
}

function verificaPossuiPerfilConcProp()
{
    $perfis = pegaPerfilGeral($_SESSION['usucpf'], $_SESSION['sisid']);

    $boConcedente = false;
    $boProponente = false;

    if(is_array($perfis)){
        foreach($perfis as $perfil){
            if(in_array($perfil, arrayPerfisConcedente())){
                $boConcedente = true;
            }else if(in_array($perfil, arrayPerfisProponente())){
                $boProponente = true;
            }else if(in_array($perfil, arrayPerfisPropConc())){
                $boProponente = true;
                $boConcedente = true;
            }
        }
    }

    if($boConcedente && $boProponente){
        return true;
    }
    return false;
}

function verificaPossuiPerfilConcedente()
{
    $perfis = pegaPerfilGeral($_SESSION['usucpf'], $_SESSION['sisid']);

    if(is_array($perfis)){
        foreach($perfis as $perfil){
            if(in_array($perfil, arrayPerfisConcedente())){
                return true;
            }
        }
    }
    return false;
}

function verificaPossuiPerfilProponente()
{
    $perfis = pegaPerfilGeral($_SESSION['usucpf'], $_SESSION['sisid']);

    if(is_array($perfis)){
        foreach($perfis as $perfil){
            if(in_array($perfil, arrayPerfisProponente())){
                return true;
            }
        }
    }
    return false;
}

function verificaTipoPerfilConcedenteProponente($pflcod = null)
{

    $arPerfilProponente = arrayPerfisProponente();
    $arPerfilConcedente = arrayPerfisConcedente();
    $arPerfilConcProp 	= arrayPerfisPropConc();

    if(in_array($pflcod, $arPerfilConcedente)){
        return 'concedente';
    }else

        if(in_array($pflcod, $arPerfilProponente)){
            return 'proponente';
        }else
            if(in_array($pflcod, $arPerfilConcProp)){
                return 'prop_conc';
            }else{
                return 'outro';
            }
}

function arrayPerfisProponente()
{
    return array(
        PERFIL_PROREITOR_ADM,
        PERFIL_REITOR
    );
}

function arrayPerfisConcedente()
{
    return array(
        CGO_COORDENADOR_ORCAMENTO,
        CGO_EQUIPE_ORCAMENTARIA,
        UO_COORDENADOR_EQUIPE_TECNICA,
        MEC_CONSULTA_ORCAMENTO_GERAL,
        UO_CONSULTA_ORCAMENTO,
        AUDITOR_EXTERNO,
        AUDITOR_INTERNO,
        PERFIL_DIRETOR_ADMIM,
        PERFIL_SECRETARIA,
        PERFIL_DIRETORIA,
        PERFIL_SECRETARIO,
        PERFIL_COORDENADOR_SEC,
        PERFIL_CONSULTA_ESPLANADA_SUSTENTAVEL,
        PERFIL_ANALISTA_DIGAP,
        PERFIL_CGSO,
        PERFIL_SUBSECRETARIO
    );
}

function arrayPerfisPropConc() {
    return array(UO_EQUIPE_TECNICA);
}

function recuperarLoteTermos() {
    global $db;

    if(is_array($_REQUEST['tcpid'])){
        $where[] = "tcp.tcpid in (".implode(",", $_REQUEST['tcpid']).")";
    }else{
        if($_REQUEST['lotid']){
            $where[] = "tcp.tcpid in (
			select tcp.tcpid from monitora.termocooperacao tcp
			join elabrev.lotemacroitens loi on tcp.tcpid = loi.tcpid
			where loi.lotid = {$_REQUEST['lotid']}
			)";
        }else{
            $where[] = "tcp.tcpid not in (
				select distinct tcp.tcpid from monitora.termocooperacao tcp
				join elabrev.lotemacroitens loi on tcp.tcpid = loi.tcpid
			)";
// 			$where[] = "tcp.tcpid in (
// 								select distinct pro.tcpid from elabrev.previsaoparcela ppa
// 								join monitora.previsaoorcamentaria pro on pro.proid = ppa.proid
// 								where pro.proid not in (select proid from elabrev.lotemacroitens where loistatus = 'A')
// 						)";

            $where[] = "doc.esdid in ( ".EM_DESCENTRALIZACAO.", ".EM_ANALISE_PELA_CGSO." )";
        }
    }

    $sql = "SELECT DISTINCT
				tcp.tcpid,
				tcp.ungcodproponente,
				unp.gescod as gescodproponente,
				rep.cpf as cpfreplegalproponente,
				tcp.ungcodconcedente,
				unc.gescod as gescodconcedente,
				rec.cpf as cpfreplegalconcedente,
				tcp.tcpdscobjetoidentificacao as tcptitulo,
				tcp.tcpobjetojustificativa as tcpjustificativa,
				tcp.tcpobjetivoobjeto,

				(select htddata from workflow.historicodocumento hst
				join workflow.acaoestadodoc aed on aed.aedid = hst.aedid
				where hst.docid = doc.docid
				and aed.esdiddestino = ".EM_ANALISE_PELA_CGSO."
				and aed.esdidorigem = ".AGUARDANDO_APROVACAO_SECRETARIO."
				order by hst.hstid desc limit 1) as data_vigencia,

				(select total from (select	sum(provalor) as total, tcpid
				From monitora.previsaoorcamentaria prot
				where prostatus = 'A'
				and prot.tcpid = tcp.tcpid
				group by tcpid) as foo
				) as valor_total,

				--loi.loiid,

				(select distinct crdmesexecucao from monitora.previsaoorcamentaria promes where tcp.tcpid = promes.tcpid limit 1) as crdmesexecucao

			FROM monitora.termocooperacao tcp
			JOIN workflow.documento doc ON doc.docid = tcp.docid
			LEFT JOIN public.unidadegestora unp ON tcp.ungcodproponente = unp.ungcod
			LEFT JOIN public.unidadegestora unc ON tcp.ungcodconcedente = unc.ungcod
			LEFT JOIN elabrev.representantelegal rep ON tcp.ungcodproponente = rep.ug
			LEFT JOIN elabrev.representantelegal rec ON tcp.ungcodconcedente = rec.ug
			LEFT JOIN elabrev.lotemacroitens loi ON loi.tcpid = tcp.tcpid AND loistatus = 'A'
			".(is_array($where) ? ' WHERE '.implode(' AND ',$where) : '')."
			ORDER BY tcp.tcpid";
// 	ver($sql,d);
    return $db->carregar($sql);
}


function recuperarLoteTermosTeste() {
    global $db;

    if (is_array($_REQUEST['proid'])) {

        $_tcpid = array();
        $_proid = array();
        foreach ($_REQUEST['proid'] as $tcpidProid) {
            $_param = explode('-', $tcpidProid);

            if (!in_array($_param[0], $_tcpid))
                array_push($_tcpid, $_param[0]);

            if (!in_array($_param[1], $_proid))
                array_push($_proid, $_param[1]);
        }

        $where[] = "tcp.tcpid in (".implode(",", $_tcpid).")";

        $conditionWhere = "AND prot.proid in (".implode(",", $_proid).")";

    } else {
        if ($_REQUEST['lotid']) {

            //Pega um lote que já foi gerado
            $where[] = "po.proid in (
			select distinct po1.proid from monitora.previsaoorcamentaria po1
            join elabrev.lotemacroitens loi on po1.proid = loi.proid
            where loi.lotid = {$_REQUEST['lotid']}
			)";

            $conditionWhere = "AND lotid = {$_REQUEST['lotid']}";
            $_lotid = (int) $_REQUEST['lotid'];

        } else {
            //Pega todas as celular orçamentárias que ainda não foram gerados lotes
            $where[] = "tcp.tcpid in (
                select distinct tcpid from monitora.previsaoorcamentaria
                where prostatus = 'A' AND proid not in (select proid from elabrev.lotemacroitens where loistatus = 'A')
			)";
            $where[] = "doc.esdid in ( ".EM_DESCENTRALIZACAO.", ".EM_ANALISE_PELA_CGSO." )";

            //Excluir as celulas orçamentarias que já foram descentralizadas em outro momento
            $where[] = "po.proid in (select proid from monitora.previsaoorcamentaria where prostatus = 'A' and proid not in (select proid from elabrev.previsaoparcela pp where pp.ppacancelarnc = 'f'))";

            $conditionWhere = "AND prot.tcpid = tcp.tcpid";
            $_lotid = "(select lotid from elabrev.lotemacro order by lotid desc limit 1)";
        }
    }

    $emAnalisePelaCGSO = EM_ANALISE_PELA_CGSO;
    $aguardandoAprovacaoSecretario = AGUARDANDO_APROVACAO_SECRETARIO;

    $whereStr = '';
    if (is_array($where)) {
        $whereStr = ' WHERE ' . implode(' AND ',$where);
    }

    $sql = <<<DML
SELECT DISTINCT
       tcp.tcpid,
       tcp.ungcodproponente,
       unp.gescod AS gescodproponente,
       rep.cpf AS cpfreplegalproponente,
       tcp.ungcodconcedente,
       unc.gescod AS gescodconcedente,
       rec.cpf AS cpfreplegalconcedente,
       tcp.tcpdscobjetoidentificacao AS tcptitulo,
       tcp.tcpobjetojustificativa AS tcpjustificativa,
       tcp.tcpobjetivoobjeto,
       (SELECT htddata
          FROM workflow.historicodocumento hst
            JOIN workflow.acaoestadodoc aed ON aed.aedid = hst.aedid
           WHERE hst.docid = doc.docid
             AND aed.esdiddestino = {$emAnalisePelaCGSO}
             AND aed.esdidorigem = {$aguardandoAprovacaoSecretario}
           ORDER BY hst.hstid DESC LIMIT 1) AS data_vigencia,
       (SELECT
            sum(provalor) AS total
        FROM monitora.previsaoorcamentaria pro
            LEFT JOIN monitora.ptres ptr ON ptr.ptrid = pro.ptrid
            LEFT JOIN monitora.pi_planointerno pi on pi.pliid = pro.pliid
            LEFT JOIN public.naturezadespesa ndp ON ndp.ndpid = pro.ndpid
        WHERE
            pro.tcpid = tcp.tcpid AND
            pro.proid IN (select lmi.proid from elabrev.lotemacroitens lmi where lmi.lotid = {$_lotid})) AS valor_total,
       -- Valor utilizado apenas na planilha, onde são incluídos apenas células presentes no lote atual
       (SELECT total
          FROM (SELECT SUM(provalor) AS total
                  FROM monitora.previsaoorcamentaria prot
                    INNER JOIN elabrev.lotemacroitens loi USING(proid)
                  WHERE prostatus = 'A'
                    {$conditionWhere}
                    AND loi.loistatus = 'A'
               ) AS foo) AS valor_no_lote,
       (SELECT DISTINCT crdmesexecucao
          FROM monitora.previsaoorcamentaria promes
          WHERE tcp.tcpid = promes.tcpid LIMIT 1) AS crdmesexecucao
  FROM monitora.termocooperacao tcp
    JOIN workflow.documento doc ON doc.docid = tcp.docid
    LEFT JOIN public.unidadegestora unp ON tcp.ungcodproponente = unp.ungcod
    LEFT JOIN public.unidadegestora unc ON tcp.ungcodconcedente = unc.ungcod
    LEFT JOIN elabrev.representantelegal rep ON tcp.ungcodproponente = rep.ug
    LEFT JOIN elabrev.representantelegal rec ON tcp.ungcodconcedente = rec.ug
    JOIN monitora.previsaoorcamentaria po ON (po.tcpid = tcp.tcpid)
  {$whereStr}
  ORDER BY tcp.tcpid
DML;

    //ver($sql, d);
    return $db->carregar($sql);
}

function contaQtdMaxPrevisaoOrcamentaria()
{
    global $db;

    if(is_array($_REQUEST['tcpid'])){

        $where[] = "tcp.tcpid in (".implode(',', $_REQUEST['tcpid']).")";
    }
    if($_REQUEST['lotid']){
        $where[] = "tcp.tcpid in (select tcpid from elabrev.lotemacroitens where lotid = {$_REQUEST['lotid']})";
        $where[] = "pro.proid in (select loi.proid from elabrev.lotemacroitens loi where loi.proid = pro.proid and loi.loistatus = 'A')";
    }
    if(empty($_GET['lotid'])){
        $where[] = "doc.esdid in ( ".EM_DESCENTRALIZACAO.", ".EM_ANALISE_PELA_CGSO." )";
    }

    $sql = "SELECT max(total) as qtd FROM (
				SELECT
					count(*) AS total,
					tcp.tcpid
				FROM monitora.termocooperacao tcp
				JOIN monitora.previsaoorcamentaria pro ON pro.tcpid = tcp.tcpid
				JOIN workflow.documento doc ON doc.docid = tcp.docid
				".(is_array($where) ? ' WHERE '.implode(' AND ',$where) : '')."
				GROUP BY tcp.tcpid
			) AS foo";

    //ver($sql, d);
    return $db->pegaUm($sql);
}

function contaQtdMaxPrevisaoOrcamentariaTeste()
{
    global $db;

    if(is_array($_REQUEST['proid'])){

        $_proid = array();
        foreach ($_REQUEST['proid'] as $tcpidProid) {
            $_param = explode('-', $tcpidProid);
            if (!in_array($_param[1], $_proid))
                array_push($_proid, $_param[1]);
        }

        $where[] = "pro.proid in (".implode(',', $_proid).")";
    }
    if($_REQUEST['lotid']){
        $where[] = "pro.proid in (select proid from elabrev.lotemacroitens where lotid = {$_REQUEST['lotid']})";
        $where[] = "pro.proid in (select loi.proid from elabrev.lotemacroitens loi where loi.proid = pro.proid and loi.loistatus = 'A')";
    }
    if(empty($_GET['lotid'])){
        $where[] = "doc.esdid in ( ".EM_DESCENTRALIZACAO.", ".EM_ANALISE_PELA_CGSO." )";
    }

    $sql = "SELECT max(total) as qtd FROM (
				SELECT
					count(*) AS total,
					tcp.tcpid
				FROM monitora.termocooperacao tcp
				JOIN monitora.previsaoorcamentaria pro ON pro.tcpid = tcp.tcpid
				JOIN workflow.documento doc ON doc.docid = tcp.docid
				".(is_array($where) ? ' WHERE '.implode(' AND ',$where) : '')."
				GROUP BY tcp.tcpid
			) AS foo";

    //ver($sql, d);
    return $db->pegaUm($sql);
}

function criaLoteMacroNC() {
    global $db;

    if(is_array($_REQUEST['tcpid'])){

        $sql = "select count(*) from elabrev.lotemacroitens where tcpid in (".implode(',', $_REQUEST['tcpid']).")";
        $count = $db->pegaUm($sql);

// 		ver($sql, $_REQUEST['tcpid'], $count, d);

        if($count == count($_REQUEST['tcpid']))
            return true;

        $sql = "insert into elabrev.lotemacro (lotdsc, lotdata, lotstatus, lotcpfresponsavel)
				values ('Termos: ".implode(', ', $_REQUEST['tcpid'])."', now(), 'A', '{$_SESSION['usucpf']}') returning lotid;";

        $lotid = $db->pegaUm($sql);

        $sqlItem = '';
        foreach($_REQUEST['tcpid'] as $tcpid){

            $sql = "select pro.proid from monitora.previsaoorcamentaria pro
					where pro.tcpid = {$tcpid}
					and pro.prostatus = 'A'
					";

            $existe = $db->carregar($sql);

            if($existe){
                foreach($existe as $previsao){
                    $sqlItem .= "insert into elabrev.lotemacroitens (lotid, tcpid, loistatus, proid)
							values ($lotid, $tcpid, 'A', {$previsao['proid']}); ";
                }
            }
        }
        if($sqlItem){
            $db->executar($sqlItem);
            if($db->commit()){
                return true;
            }
        }
    }
    return false;
}

/**
 * Em teste - para novo relatório XLS
 * Cria celular somente dos lotes selecionados
 * @return bool
 */
function criaLoteMacroNCTeste() {
    global $db;

    if (is_array($_REQUEST['proid'])) {

        $_tcpid = array();
        if (count($_REQUEST['proid'])) {
            foreach ($_REQUEST['proid'] as $value) {
                $_tmpValue = explode('-', $value);
                if (!empty($_tmpValue[0]) && !in_array($_tmpValue[0], $_tcpid)) {
                    array_push($_tcpid, $_tmpValue[0]);
                }
            }
        }

        if (count($_tcpid)) {
            $sql = "select count(*) from elabrev.lotemacroitens where tcpid in (".implode(',', $_tcpid).")";
            $count = $db->pegaUm($sql);

            if ($count == count($_tcpid))
                return true;

            $sql = "insert into elabrev.lotemacro (lotdsc, lotdata, lotstatus, lotcpfresponsavel)
                    values ('Termos: ".implode(', ', $_tcpid)."', now(), 'A', '{$_SESSION['usucpf']}') returning lotid;";
            $lotid = $db->pegaUm($sql);

            $sqlItem = '';
            foreach ($_REQUEST['proid'] as $p) {
                $param = explode('-', $p);
                $_tcpid = $param[0];
                $_proid = $param[1];

                $existe = $db->pegaUm("select pro.proid from monitora.previsaoorcamentaria pro where pro.tcpid = {$_tcpid} and pro.proid = {$_proid} and pro.prostatus = 'A'");
                if ($existe) {
                    $sqlItem .= "insert into elabrev.lotemacroitens (lotid, tcpid, loistatus, proid) values ($lotid, $_tcpid, 'A', $_proid); ";
                }
            }

            if ($sqlItem) {
                $db->executar($sqlItem);
                if ($db->commit()) {
                    return true;
                }
            }
        }
    }
    return false;
}

function verificaPreenchimentoEmAnaliseCoordenacao()
{
    global $db;

    $sql = "select
				tcpparecertecnico,
				tcpconsidentproponente,
				tcpconsidproposta,
				tcpconsidobjeto,
				tcpconsidobjetivo,
				tcpconsidjustificativa,
				tcpconsidvalores,
				tcpconsidcabiveis
			from
				monitora.termocooperacao
			where
				tcpid = ". $_SESSION['elabrev']['tcpid'];

    $dadoParecer = $db->pegaLinha($sql);

    if(empty($dadoParecer['tcpconsidentproponente']) ||
        empty($dadoParecer['tcpconsidentproponente']) ||
        empty($dadoParecer['tcpconsidproposta']) ||
        empty($dadoParecer['tcpconsidobjeto']) ||
        empty($dadoParecer['tcpconsidobjetivo']) ||
        empty($dadoParecer['tcpconsidjustificativa']) ||
        empty($dadoParecer['tcpconsidvalores']) ||
        empty($dadoParecer['tcpconsidcabiveis'])){

        return false;
    }

    $sql = "select * from monitora.previsaoorcamentaria where tcpid = {$_SESSION['elabrev']['tcpid']} and prostatus = 'A'";
    $rs = $db->carregar($sql);

    $boPrevisao = true;
    foreach($rs as $dado){
        if(empty($dado['ptrid']) || empty($dado['pliid']) || empty($dado['crdmesliberacao'])){
            $boPrevisao = false;
        }
    }

    if(!$boPrevisao){
        return false;
    }
    return true;

}

function makeDateSoma($date, $days=0, $mounths=0, $years=0)
{
    $date = explode("/", $date);
    return date('d/m/Y', mktime(0, 0, 0, $date[1] + $mounths, $date[0] +  $days, $date[2] + $years) );
}

function verificaPodeEnviarDiligencia() {
    global $db;

    $perfis = pegaPerfilGeral($_SESSION['usucpf'], $_SESSION['sisid']);

    $ungcod = $db->pegaUm("
	    select
            tcp.ungcodconcedente
        from monitora.termocooperacao tcp
        join elabrev.usuarioresponsabilidade usr on ( usr.ungcod = tcp.ungcodpoliticafnde or usr.ungcod = tcp.ungcodconcedente )
            and usr.rpustatus = 'A' and usr.usucpf = '{$_SESSION['usucpf']}'
        where tcp.tcpid = {$_SESSION['elabrev']['tcpid']} and usr.pflcod = ".UO_EQUIPE_TECNICA."
	");

    if (in_array(PERFIL_COORDENADOR_SEC, $perfis) || in_array(PERFIL_SUPER_USUARIO, $perfis)) {
        return true;
    } else if ($ungcod && in_array(UO_EQUIPE_TECNICA, $perfis)) {
        return true;
    }

    return false;
}

function montaXmlNotaCreditoFnde()
{

    global $db;

    $sql = "select
				tcp.tcpid,
				unp.ungcod as unidade_gestora_favorecida,
				unp.gescod as gestao_favorecida
			from monitora.termocooperacao tcp
			join public.unidadegestora unp on unp.ungcod = tcp.ungcodproponente
			where tcp.tcpid = {$_SESSION['elabrev']['tcpid']}";

    $rsTC = $db->pegaLinha($sql);

    if($rsTC){

        $data_created = date('c');

        $usuario = $_POST['sigefusername'];
        $senha = $_POST['sigefpassword'];

        // Dados do termo
        $unidade_gestora_favorecida = $rsTC['unidade_gestora_favorecida'];
        $gestao_favorecida = $rsTC['gestao_favorecida'];
        $observacao = $_POST['tcpobsfnde'];
        $complemento = $_POST['tcpobscomplemento'] ? substr($_POST['tcpobscomplemento'], 0, 239) : '';
        $processo = $_POST['tcpnumprocessofnde'];
        $numero_documento_siafi_original = '';
        $nc_original = '';
        $especie = $_POST['especie'];
        $programa = str_pad($_POST['tcpprogramafnde'], 2, '0', STR_PAD_LEFT);
        $sistema = $_POST['sistema'];
        $termo_compromisso = $_POST['tcpnumtransfsiafi'];

        $sql = "update monitora.termocooperacao set
					tcpnumtransfsiafi 	= '{$termo_compromisso}',
					tcpnumprocessofnde 	= '{$processo}',
					tcpprogramafnde 	= '{$programa}',
					tcpobsfnde 			= '{$observacao}',
					uniid 				= '{$_POST['uniid']}',
					ungcodemitente 		= '{$_POST['ungcodemitente']}',
					gescodemitente 		= '{$_POST['gescodemitente']}',
					tcpobscomplemento 	= '{$complemento}'
				where tcpid = '{$_SESSION['elabrev']['tcpid']}'";

        $db->executar($sql);

        if($rsTC['tcpid']){
            $sql = "
				SELECT DISTINCT
					proid,
					ptres,
					provalor,
					proanoreferencia,
					plicod,
					ndpcod
				FROM monitora.previsaoorcamentaria pro
				LEFT JOIN monitora.pi_planointerno pi 		ON pi.pliid = pro.pliid
				LEFT JOIN monitora.pi_planointernoptres pts ON pts.pliid = pi.pliid
				LEFT JOIN public.naturezadespesa ndp 		ON ndp.ndpid = pro.ndpid
				LEFT JOIN monitora.ptres p 					ON p.ptrid = pro.ptrid
				LEFT JOIN monitora.acao a 					ON a.acaid = p.acaid
				LEFT JOIN public.unidadegestora u 			ON u.unicod = p.unicod
				LEFT JOIN monitora.pi_planointernoptres pt 	ON pt.ptrid = p.ptrid
				WHERE pro.prostatus = 'A'
				--AND tcpid = ".$rsTC['tcpid']."
				AND pro.proid in (".implode(',', $_POST['chekCel']).")
			";

            $rsPO = $db->carregar($sql);
            $detalhamento = '';
            if($rsPO){
// 				$x=1;
                foreach($rsPO as $po){

                    if( !$_POST['prgid'][$po['proid']] )
                        continue;

// 					if($x==1){
                    $sql = "select * from elabrev.dadosprogramasfnde where prgid = '{$_REQUEST['prgid'][$po['proid']]}'";
                    $rsPrograma = $db->pegaLinha($sql);

                    $sql = "update monitora.previsaoorcamentaria set
								prgidfnde = '{$_REQUEST['prgid'][$po['proid']]}',
								prgfonterecurso = '{$_REQUEST['prgfonterecurso'][$po['proid']]}',
								espid = '{$_REQUEST['espid'][$po['proid']]}',
								esfid = '{$_REQUEST['esfid'][$po['proid']]}'
							where proid = {$po['proid']}";
                    $db->executar($sql);

                    $evento_contabil 			= $_POST['evento_contabil'];
                    $esfera_orcamentaria 		= $_POST['esfid'][$po['proid']];
                    $unidade_orcamentaria 		= $_POST['unicod'];
                    $centro_gestao 				= $rsPrograma['gescod'];
                    $ptres 						= $po['ptres'];
                    $fonte_recurso 				= $_POST['prgfonterecurso'][$po['proid']];
                    $natureza_despesa 			= $po['ndpcod'];
                    $plano_interno 				= $po['plicod'];
                    $ano_exercicio 				= $po['proanoreferencia'];
                    $valor 						= $po['provalor'];
                    $unidade_gestora_emitente 	= $_POST['ungcodemitente'];
                    $gestao_emitente 			= $_POST['gescodemitente'];


                    //<ptres>$ptres</ptres>
                    //<ptres>000000</ptres>
                    $detalhamento .= "
						<detalhamento>
							<evento_contabil>$evento_contabil</evento_contabil>
							<esfera_orcamentaria>$esfera_orcamentaria</esfera_orcamentaria>
							<unidade_orcamentaria>$unidade_orcamentaria</unidade_orcamentaria>
							<centro_gestao>$centro_gestao</centro_gestao>
							<celula_orcamentaria>
								<ptres>$ptres</ptres>
								<fonte_recurso>$fonte_recurso</fonte_recurso>
								<natureza_despesa>$natureza_despesa</natureza_despesa>
								<plano_interno>$plano_interno</plano_interno>
							</celula_orcamentaria>
							<ano_exercicio>$ano_exercicio</ano_exercicio>
							<valor>$valor</valor>
							<unidade_gestora_emitente>$unidade_gestora_emitente</unidade_gestora_emitente>
							<gestao_emitente>$gestao_emitente</gestao_emitente>
						</detalhamento>
					";
// 					}
// 					$x++;
                }
            }
        }
        $arqXml = <<<XML
<?xml version='1.0' encoding='iso-8859-1'?>
<request>
	<header>
		<app>string</app>
		<version>string</version>
		<created>$data_created</created>
	</header>
	<body>
		<auth>
			<usuario>$usuario</usuario>
			<senha>$senha</senha>
		</auth>
		<params>
			<unidade_gestora_favorecida>$unidade_gestora_favorecida</unidade_gestora_favorecida>
			<gestao_favorecida>$gestao_favorecida</gestao_favorecida>
			<observacao>$observacao</observacao>
			<complemento>$complemento</complemento>
			<processo>$processo</processo>
			<numero_documento_siafi_original>$numero_documento_siafi_original</numero_documento_siafi_original>
			<nc_original>$nc_original</nc_original>
			<especie>$especie</especie>
			<programa>$programa</programa>
			$detalhamento
			<sistema>$sistema</sistema>
			<termo_compromisso>$termo_compromisso</termo_compromisso>
		</params>
	</body>
</request>
XML;

        if($db->commit()){
            return $arqXml;
        }
    }

    return false;
}

function montaXmlNotaCreditoFndeTeste()
{
    $data_created = date('c');

    $usuario = 'MECTIAGOT';
    $senha = 'M3135689';

    /* Paramentros */
    $unidade_gestora_favorecida = '153173'; // String 6 digitos
    $gestao_favorecida = '15253'; // String 5 digitos
    $observacao = '25'; // String maxLenght 2, maxOccurs="1" minOccurs="1"
    $complemento = 'XXXXXXX XXXXXXXXX'; // String maxLentght 240, maxOccurs="1" minOccurs="0"
    $processo = '23034252324201109'; // String 17, maxOccurs="1" minOccurs="1"
    $numero_documento_siafi_original = ''; // String, minOccurs="0"
    $nc_original = ''; // String, maxOccurs="0"
    $especie = '3'; // String 2, maxOccurs="1" minOccurs="1"
    $programa = 'C7'; // String, maxOccurs="1" minOccurs="1"

    $detalhamento = ''; // Previsões orçamentarias, maxOccurs="12" minOccurs="1"

    $evento_contabil = '300300'; // String max 6, maxOccurs="1" minOccurs="1"
    $esfera_orcamentaria = '1'; // String max 1, maxOccurs="1" minOccurs="1"
    $unidade_orcamentaria = '26298'; // String, maxOccurs="1" minOccurs="1"
    $centro_gestao = '51000000000'; // String max 11, maxOccurs="1" minOccurs="1"

    $celula_orcamentaria = ''; // Dados das previsões

    $ptres = '043930'; // String 6, pattern value="\w{6}"
    $fonte_recurso = '0100479430'; // String 10, maxOccurs="1" minOccurs="1"
    $natureza_despesa = '31909100'; // String 8, pattern value="\w{8}", teste erro 339048
    $plano_interno = 'FFF53B0101N'; // String 11, pattern value="\w{11}"

    $ano_exercicio = '2013'; // Year
    $valor = '30000.00'; // Float
    $unidade_gestora_emitente = '153173'; // String 6
    $gestao_emitente = '15253'; // String 5

    $sistema = '2'; // String, maxOccurs="1" minOccurs="1"
    $termo_compromisso = '38'; // String,  maxOccurs="1" minOccurs="0"

    $arqXml = <<<XML
<?xml version='1.0' encoding='iso-8859-1'?>
<request>
	<header>
		<app>string</app>
		<version>string</version>
		<created>$data_created</created>
	</header>
	<body>
		<auth>
			<usuario>$usuario</usuario>
			<senha>$senha</senha>
		</auth>
		<params>
			<unidade_gestora_favorecida>$unidade_gestora_favorecida</unidade_gestora_favorecida>
			<gestao_favorecida>$gestao_favorecida</gestao_favorecida>
			<observacao>$observacao</observacao>
			<complemento>$complemento</complemento>
			<processo>$processo</processo>
			<numero_documento_siafi_original>$numero_documento_siafi_original</numero_documento_siafi_original>
			<nc_original>$nc_original</nc_original>
			<especie>$especie</especie>
			<programa>$programa</programa>
			<detalhamento>
				<evento_contabil>$evento_contabil</evento_contabil>
				<esfera_orcamentaria>$esfera_orcamentaria</esfera_orcamentaria>
				<unidade_orcamentaria>$unidade_orcamentaria</unidade_orcamentaria>
				<centro_gestao>$centro_gestao</centro_gestao>
				<celula_orcamentaria>
					<ptres>$ptres</ptres>
					<fonte_recurso>$fonte_recurso</fonte_recurso>
					<natureza_despesa>$natureza_despesa</natureza_despesa>
					<plano_interno>$plano_interno</plano_interno>
				</celula_orcamentaria>
				<ano_exercicio>$ano_exercicio</ano_exercicio>
				<valor>$valor</valor>
				<unidade_gestora_emitente>$unidade_gestora_emitente</unidade_gestora_emitente>
				<gestao_emitente>$gestao_emitente</gestao_emitente>
			</detalhamento>
			<detalhamento>
				<evento_contabil>$evento_contabil</evento_contabil>
				<esfera_orcamentaria>$esfera_orcamentaria</esfera_orcamentaria>
				<unidade_orcamentaria>$unidade_orcamentaria</unidade_orcamentaria>
				<centro_gestao>$centro_gestao</centro_gestao>
				<celula_orcamentaria>
					<ptres>$ptres</ptres>
					<fonte_recurso>$fonte_recurso</fonte_recurso>
					<natureza_despesa>$natureza_despesa</natureza_despesa>
					<plano_interno>$plano_interno</plano_interno>
				</celula_orcamentaria>
				<ano_exercicio>$ano_exercicio</ano_exercicio>
				<valor>$valor</valor>
				<unidade_gestora_emitente>$unidade_gestora_emitente</unidade_gestora_emitente>
				<gestao_emitente>$gestao_emitente</gestao_emitente>
			</detalhamento>
			<sistema>$sistema</sistema>
			<termo_compromisso>$termo_compromisso</termo_compromisso>
		</params>
	</body>
</request>
XML;

    return $arqXml;
}

/**
 * Verifica se existe nota de crédito cadastrada
 * antes de permitir o envio para execucao
 * @param null $tcpid
 * @return bool|string
 */
function verificaConcedenteFndeEnviarNc($tcpid = null) {
    global $db;

    $tcpid = $tcpid ? $tcpid : $_SESSION['elabrev']['tcpid'];

    $sql = "SELECT sigefid, codsigefnc FROM monitora.previsaoorcamentaria WHERE tcpid = $tcpid AND prostatus = 'A' AND codsigefnc is not null AND sigefid is not null";
    $rsFNDE = $db->pegaLinha($sql);

    if ($rsFNDE) {
        if (!is_null($rsFNDE['sigefid'])  && !empty($rsFNDE['codsigefnc'])) {
            return true;
        }
        return 'Falta enviar ou efetivar a nota de crédito junto ao SIGEF!';
    } else {
        if (permiteEnviarExecucao($tcpid)) {
            return true;
        }
        return 'Falta cadastrar nota de crédito na aba previsão orçamentária!';
    }
}

function verificaConcedenteFnde($tcpid = null) {
    global $db;

    $tcpid = $tcpid ? $tcpid : $_SESSION['elabrev']['tcpid'];

    $sql = "
        select true from
            monitora.termocooperacao
        where tcpid = $tcpid
        and ungcodconcedente = '".UG_FNDE."'
    ";

    $rsFNDE = $db->pegaUm($sql);

    if ($rsFNDE) {
        return true;
    } else {
        return false;
    }
}

function verificaConcedenteNaoFnde( $tcpid = null )
{
    global $db;

    $tcpid = $tcpid ? $tcpid : $_SESSION['elabrev']['tcpid'];

    $sql = "select
				true
			from
				monitora.termocooperacao
			where
				tcpid = $tcpid
			and ungcodconcedente = '".UG_FNDE."'";

    $rsFNDE = $db->pegaUm($sql);

    if($rsFNDE){
        return false;
    }else{
        return true;
    }
}

function recuperaDataGeraPdf()
{
    global $db;

    $tcpid = $_SESSION['elabrev']['tcpid'] ? $_SESSION['elabrev']['tcpid'] : $_REQUEST['tcpid'];

    $sql = "select
				max(hd.htddata) as data
			from workflow.historicodocumento hd
				inner join workflow.acaoestadodoc ac on
					ac.aedid = hd.aedid
				inner join workflow.estadodocumento ed on
					ed.esdid = ac.esdidorigem
				inner join seguranca.usuario us on
					us.usucpf = hd.usucpf
				left join workflow.comentariodocumento cd on
					cd.hstid = hd.hstid
			where
				hd.docid = (select docid from monitora.termocooperacao where tcpid = {$tcpid})
			and ac.esdidorigem in (635)";

    $rs = !empty($tcpid) ? $db->pegaUm($sql) : false;

    if($rs){

        $arData = explode(' ', $rs);
        $arData = explode('-', $arData[0]);

        switch ($arData[1]){
            case 1: $mes = "Janeiro"; break;
            case 2: $mes = "Fevereiro"; break;
            case 3: $mes = "Março"; break;
            case 4: $mes = "Abril"; break;
            case 5: $mes = "Maio"; break;
            case 6: $mes = "Junho"; break;
            case 7: $mes = "Julho"; break;
            case 8: $mes = "Agosto"; break;
            case 9: $mes = "Setembro"; break;
            case 10: $mes = "Outubro"; break;
            case 11: $mes = "Novembro"; break;
            case 12: $mes = "Dezembro"; break;

        }

        return 'Brasília, '.$arData[2].' de '.$mes.' de '.$arData[0].'';
    }

    return ' ';

}

function verificaConcPropFnde()
{
    global $db;

    $sql = "select
				tcpid
			from
				monitora.termocooperacao
			where
				tcpid = {$_SESSION['elabrev']['tcpid']}
			and
				(ungcodproponente = '".UG_FNDE."' or ungcodconcedente = '".UG_FNDE."')";

    $rs = !empty($_SESSION['elabrev']['tcpid']) ? $db->pegaUm($sql) : false;

    if($rs){
        return true;
    }
    return false;
}

function verificaPodeInserirEditarEquipeTecnica()
{
    global $db;

    if($db->testa_superuser()){

        return true;

    }

    if(empty($_SESSION['elabrev']['tcpid'])){

        return true;

    }else{

        $sql = "select
					tcpid
				from elabrev.usuarioresponsabilidade ur
				join monitora.termocooperacao tc on tc.ungcodproponente = ur.ungcod
				where tc.tcpid = {$_SESSION['elabrev']['tcpid']}
				and ur.usucpf = '{$_SESSION['usucpf']}'
				and ur.pflcod = ".UO_EQUIPE_TECNICA;

        //ver($sql, d);
        $rs = $db->pegaUm($sql);

        if($rs) return true;

    }

    return false;

}

function salvarRelCumprimento()
{
    global $db;

    if (!$_REQUEST['onlyread']) {
        if (!empty($_REQUEST['recid'])) {

            $sql = "update elabrev.relatoriocumprimento set
                    tcpid = ".($_REQUEST['tcpid'] ? $_REQUEST['tcpid'] : $_SESSION['elabrev']['tcpid']).",
                    reccnpj = ".($_REQUEST['reccnpj'] ? "'".str_replace(array('.','/','-', '\\'), '', $_REQUEST['reccnpj'])."'" : "''").",
                    recnome = ".($_REQUEST['recnome'] ? "'".$_REQUEST['recnome']."'" : "''").",
                    recendereco = ".($_REQUEST['recendereco'] ? "'".$_REQUEST['recendereco']."'" : "''").",
                    muncod = ".($_REQUEST['muncod'] ? "'".$_REQUEST['muncod']."'" : "''").",
                    estuf = ".($_REQUEST['estuf'] ? "'".$_REQUEST['estuf']."'" : "''").",
                    reccep = ".($_REQUEST['reccep'] ? "'".str_replace(array('-','.'), '', $_REQUEST['reccep'])."'" : "''").",
                    rectelefone = ".($_REQUEST['rectelefone'] ? "'".$_REQUEST['rectelefoneddd'].str_replace('-', '', $_REQUEST['rectelefone'])."'" : "''").",
                    uocod = ".($_REQUEST['uocod'] ? "'".$_REQUEST['uocod']."'" : "''").",
                    ugcod = ".($_REQUEST['ugcod'] ? "'".$_REQUEST['ugcod']."'" : "''").",
                    gestaocod = ".($_REQUEST['gestaocod'] ? "'".$_REQUEST['gestaocod']."'" : "''").",
                    recnomeresponsavel = ".($_REQUEST['recnomeresponsavel'] ? "'".$_REQUEST['recnomeresponsavel']."'" : "''").",
                    reccpfresponsavel = ".($_REQUEST['reccpfresponsavel'] ? "'".$_REQUEST['reccpfresponsavel']."'" : "''").",
                    recsiaperesponsavel = ".($_REQUEST['recsiaperesponsavel'] ? "'".$_REQUEST['recsiaperesponsavel']."'" : "''").",
                    recrgresponsavel = ".($_REQUEST['recrgresponsavel'] ? "'".$_REQUEST['recrgresponsavel']."'" : "''").",
                    recdtemissaorgresposavel = ".($_REQUEST['recdtemissaorgresposavel'] ? "'".formata_data_sql($_REQUEST['recdtemissaorgresposavel'])."'" : "''").",
                    recexpedidorrgresposavel = ".($_REQUEST['recexpedidorrgresposavel'] ? "'".$_REQUEST['recexpedidorrgresposavel']."'" : "''").",
                    reccargo = ".($_REQUEST['reccargo'] ? "'".$_REQUEST['reccargo']."'" : "''").",
                    recemailresposavel = ".($_REQUEST['recemailresposavel'] ? "'".$_REQUEST['recemailresposavel']."'" : "''").",
                    recnumportaria = ".($_REQUEST['recnumportaria'] ? "'".$_REQUEST['recnumportaria']."'" : "''").",
                    recdtpublicacao = ".($_REQUEST['recdtpublicacao'] ? "'".formata_data_sql($_REQUEST['recdtpublicacao'])."'" : "''").",
                    recnumnotacredito = ".($_REQUEST['recnumnotacredito'] ? "'".$_REQUEST['recnumnotacredito']."'" : "''").",
                    recexecucaoobjeto = ".($_REQUEST['recexecucaoobjeto'] ? "'".$_REQUEST['recexecucaoobjeto']."'" : "''").",
                    recatividadesprevistas = ".($_REQUEST['recatividadesprevistas'] ? "'".$_REQUEST['recatividadesprevistas']."'" : "''").",
                    recmetaprevista = ".($_REQUEST['recmetaprevista'] ? "'".$_REQUEST['recmetaprevista']."'" : "''").",
                    recatividadesexecutadas = ".($_REQUEST['recatividadesexecutadas'] ? "'".$_REQUEST['recatividadesexecutadas']."'" : "''").",
                    recmetaexecutada = ".($_REQUEST['recmetaexecutada'] ? "'".$_REQUEST['recmetaexecutada']."'" : "''").",
                    recdificuldades = ".($_REQUEST['recdificuldades'] ? "'".$_REQUEST['recdificuldades']."'" : "''").",
                    recmetasadotadas = ".($_REQUEST['recmetasadotadas'] ? "'".$_REQUEST['recmetasadotadas']."'" : "''").",
                    reccomentarios = ".($_REQUEST['reccomentarios'] ? "'".$_REQUEST['reccomentarios']."'" : "''").",
                    recvlrrecebido = ".($_REQUEST['recvlrrecebido'] ? formata_valor_sql($_REQUEST['recvlrrecebido']) : "null").",
                    recvlrutilizado = ".($_REQUEST['recvlrutilizado'] ? formata_valor_sql($_REQUEST['recvlrutilizado']) : "null").",
                    recvlrdevolvido = ".($_REQUEST['recvlrdevolvido'] ? formata_valor_sql($_REQUEST['recvlrdevolvido']) : "null").",
                    recnumncdevolucao = ".($_REQUEST['recnumncdevolucao'] ? "'".$_REQUEST['recnumncdevolucao']."'" : "null")."
                    where recid = {$_REQUEST['recid']}";
            //ver($sql, d);
            $db->executar($sql);
            $recid = $_REQUEST['recid'];

        } else {

            $sql = "insert into elabrev.relatoriocumprimento
                (
                    tcpid,
                    reccnpj,
                    recnome,
                    recendereco,
                    muncod,
                    estuf,
                    reccep,
                    rectelefone,
                    uocod,
                    ugcod,
                    gestaocod,
                    recnomeresponsavel,
                    reccpfresponsavel,
                    recsiaperesponsavel,
                    recrgresponsavel,
                    recdtemissaorgresposavel,
                    recexpedidorrgresposavel,
                    reccargo,
                    recemailresposavel,
                    recnumportaria,
                    recdtpublicacao,
                    recnumnotacredito,
                    recexecucaoobjeto,
                    recatividadesprevistas,
                    recmetaprevista,
                    recatividadesexecutadas,
                    recmetaexecutada,
                    recdificuldades,
                    recmetasadotadas,
                    reccomentarios,
                    recvlrrecebido,
                    recvlrutilizado,
                    recvlrdevolvido,
                    recnumncdevolucao
                )
                values
                (
                    ".($_REQUEST['tcpid'] ? $_REQUEST['tcpid'] : $_SESSION['elabrev']['tcpid']).",
                    ".($_REQUEST['reccnpj'] ? "'".str_replace(array('.','/','-', '\\'), '', $_REQUEST['reccnpj'])."'" : "''").",
                    ".($_REQUEST['recnome'] ? "'".$_REQUEST['recnome']."'" : "''").",
                    ".($_REQUEST['recendereco'] ? "'".$_REQUEST['recendereco']."'" : "''").",
                    ".($_REQUEST['muncod'] ? "'".$_REQUEST['muncod']."'" : "''").",
                    ".($_REQUEST['estuf'] ? "'".$_REQUEST['estuf']."'" : "''").",
                    ".($_REQUEST['reccep'] ? "'".str_replace(array('-','.'), '', $_REQUEST['reccep'])."'" : "''").",
                    ".($_REQUEST['rectelefone'] ? "'".$_REQUEST['rectelefoneddd'].str_replace('-', '', $_REQUEST['rectelefone'])."'" : "''").",
                    ".($_REQUEST['uocod'] ? "'".$_REQUEST['uocod']."'" : "''").",
                    ".($_REQUEST['ugcod'] ? "'".$_REQUEST['ugcod']."'" : "''").",
                    ".($_REQUEST['gestaocod'] ? "'".$_REQUEST['gestaocod']."'" : "''").",
                    ".($_REQUEST['recnomeresponsavel'] ? "'".$_REQUEST['recnomeresponsavel']."'" : "''").",
                    ".($_REQUEST['reccpfresponsavel'] ? "'".$_REQUEST['reccpfresponsavel']."'" : "''").",
                    ".($_REQUEST['recsiaperesponsavel'] ? "'".$_REQUEST['recsiaperesponsavel']."'" : "''").",
                    ".($_REQUEST['recrgresponsavel'] ? "'".$_REQUEST['recrgresponsavel']."'" : "''").",
                    ".($_REQUEST['recdtemissaorgresposavel'] ? "'".formata_data_sql($_REQUEST['recdtemissaorgresposavel'])."'" : "''").",
                    ".($_REQUEST['recexpedidorrgresposavel'] ? "'".$_REQUEST['recexpedidorrgresposavel']."'" : "''").",
                    ".($_REQUEST['reccargo'] ? "'".$_REQUEST['reccargo']."'" : "''").",
                    ".($_REQUEST['recemailresposavel'] ? "'".$_REQUEST['recemailresposavel']."'" : "''").",
                    ".($_REQUEST['recnumportaria'] ? "'".$_REQUEST['recnumportaria']."'" : "''").",
                    ".($_REQUEST['recdtpublicacao'] ? "'".formata_data_sql($_REQUEST['recdtpublicacao'])."'" : "''").",
                    ".(!is_array($_REQUEST['recnumnotacredito']) ? "'".$_REQUEST['recnumnotacredito']."'" : "''").",
                    ".($_REQUEST['recexecucaoobjeto'] ? "'".$_REQUEST['recexecucaoobjeto']."'" : "''").",
                    ".($_REQUEST['recatividadesprevistas'] ? "'".$_REQUEST['recatividadesprevistas']."'" : "''").",
                    ".($_REQUEST['recmetaprevista'] ? "'".$_REQUEST['recmetaprevista']."'" : "''").",
                    ".($_REQUEST['recatividadesexecutadas'] ? "'".$_REQUEST['recatividadesexecutadas']."'" : "''").",
                    ".($_REQUEST['recmetaexecutada'] ? "'".$_REQUEST['recmetaexecutada']."'" : "''").",
                    ".($_REQUEST['recdificuldades'] ? "'".$_REQUEST['recdificuldades']."'" : "''").",
                    ".($_REQUEST['recmetasadotadas'] ? "'".$_REQUEST['recmetasadotadas']."'" : "''").",
                    ".($_REQUEST['reccomentarios'] ? "'".$_REQUEST['reccomentarios']."'" : "''").",
                    ".($_REQUEST['recvlrrecebido'] ? formata_valor_sql($_REQUEST['recvlrrecebido']) : "null").",
                    ".($_REQUEST['recvlrutilizado'] ? formata_valor_sql($_REQUEST['recvlrutilizado']) : "null").",
                    ".($_REQUEST['recvlrdevolvido'] ? formata_valor_sql($_REQUEST['recvlrdevolvido']) : "null").",
                    ".($_REQUEST['recnumncdevolucao'] ? "'".$_REQUEST['recnumncdevolucao']."'" : "null")."
                ) returning recid;";
            //ver($sql, d);
            $recid = $db->pegaUm($sql);
        }
    } else {
        $recid = $_REQUEST['recid'];
    }

    $arquivo = $_FILES['arquivo'];
    if ($_FILES['arquivo'] && $arquivo['name'] && $arquivo['type'] && $arquivo['size']) {
        include_once APPRAIZ . 'includes/classes/fileSimec.class.inc';
        $file = new FilesSimec('arquivo', null, 'public');
        $file->setPasta('elabrev');
        $file->setUpload(null, 'arquivo', false);

        $db->executar("INSERT INTO elabrev.anexos(arqid, recid, usucpf) values ({$file->getIdArquivo()}, $recid, '".$_SESSION['usucpf']."')");
    }

    if ($_REQUEST['recnumnotacredito'] && $recid) {

        if(empty($_REQUEST['recnumnotacredito']))
            $sql = "";
        else
            $sql = "delete from elabrev.relcumprimentonc where tcpid = {$_SESSION['elabrev']['tcpid']} and recid = {$recid} and rcndevolucao = false;";

        foreach($_REQUEST['recnumnotacredito'] as $recnumnotacredito){
            if(!empty($recnumnotacredito)){
                $sql .= "insert into elabrev.relcumprimentonc (tcpid, recid, rcnnumnc) values
						({$_SESSION['elabrev']['tcpid']}, {$recid}, '{$recnumnotacredito}');";
            }
        }

        //ver($sql);
        if ($sql) {
            $db->executar($sql);
        }

    }

    if ($_REQUEST['recnumnotacredito_dev'] && $recid) {

        if(empty($_REQUEST['recnumnotacredito_dev']))
            $sql = "";
        else
            $sql = "delete from elabrev.relcumprimentonc where tcpid = {$_SESSION['elabrev']['tcpid']} and recid = {$recid} and rcndevolucao = true;";

        foreach ($_REQUEST['recnumnotacredito_dev'] as $recnumnotacredito) {
            if (!empty($recnumnotacredito)) {
                $sql .= "insert into elabrev.relcumprimentonc (tcpid, recid, rcnnumnc, rcndevolucao) values
				({$_SESSION['elabrev']['tcpid']}, {$recid}, '{$recnumnotacredito}', true);";
            }
        }

        //ver($sql, d);
        if($sql) {
            $db->executar($sql);
        }

    }

// 	ver($sql, d);
    if ($db->commit()) {
        $db->sucesso('principal/termoCooperacao/cadTermoCooperacao', '&aba=cumprimento');
    } else {
        $db->insucesso('Falha na operação!', 'principal/termoCooperacao/cadTermoCooperacao', '&aba=cumprimento');
    }
}

/**
 * Consulta as notas de crédito para listagem na tela de trâmite ou para envio da NC para o SIGEF.
 * O conjunto do resultado é alterado por <$listagem>.
 *
 * @global cls_banco $db Conexão com a base de dados.
 * @param int $tcpid ID do termo de execução descentralizado.
 * @param bool $listagem Indica se os campos de listagem devem ser inclusos na query.
 * @return array|bool Listagem dos campos do XML ou do relatório de listagem.
 * @see abaTramite.inc
 * @see solicitarCadastroDeNotasDeCreditoSIGEF()
 **/
function consultarNotasDeCredito($tcpid, $listagem = false, $filtrar = false)
{
    global $db;

    $selectAdicional = '';
    if ($listagem) {
        $selectAdicional = <<<DML
, COALESCE(ppa.ppaultimoretornosigef, '-') AS ppaultimoretornosigef,
       CASE WHEN ppa.ppacadastradosigef THEN 'Sim' ELSE 'Não' END AS ppacadastradosigef
DML;
    }

    $whereAdicional = '';
    if ($filtrar) {
        $whereAdicional = <<<DML
AND ppa.ppacadastradosigef = false
DML;
    }

    // -- IMPORTANTE
    // -- Não adicionar ou remover ou reordenar os campos do select sem considerar no XML
    // -- de solicitacao da NC em solicitarCadastroDeNotasDeCreditoSIGEF().
    $sql = <<<DML
SELECT
       ppa.proid,
       ungcodemitente AS unidade_gestora_emitente,
       gescodemitente AS gestao_emitente,
       ungcodproponente as unidade_gestora_favorecida,
       ung.gescod AS gestao_favorecida,
       tcp.tcpnumprocessofnde AS processo,
       tcp.tcpprogramafnde AS programa,
       'false' AS estorno,
       -- '' AS numero_convenio,
       -- '' AS ano_convenio,
       (select sum(pp.ppavlrparcela) from elabrev.previsaoparcela pp where pp.proid in (
            select proid from monitora.previsaoorcamentaria t where t.tcpid = %d and t.prostatus = 'A' and pp.ppacadastradosigef = 'f')
       ) AS valor_custeio,
       0 AS valor_capital,
       '' AS complemento,
       pro.proanoreferencia AS exercicio,
       --ppa.tcpnumtransfsiafi AS nota_credito,
       ppa.codsigefnc AS nota_credito,
       ptr.ptres,
       pro.prgfonterecurso AS fonte_recurso,
       ndpcod AS natureza_despesa,
       pli.plicod AS plano_interno,
       pro.proanoreferencia AS ano_parcela,
       row_number() OVER (ORDER BY ppaid) AS numero_parcela,
       pro.crdmesliberacao AS mes_pagamento,
       pro.provalor AS valor_pagamento
       {$selectAdicional}
  FROM elabrev.previsaoparcela ppa
    INNER JOIN monitora.previsaoorcamentaria pro ON(pro.proid = ppa.proid)
    inner join monitora.termocooperacao tcp ON(tcp.tcpid = pro.tcpid)
    INNER JOIN public.unidadegestora ung ON (tcp.ungcodproponente = ung.ungcod)
    INNER JOIN monitora.ptres ptr USING(ptrid)
    INNER JOIN public.naturezadespesa ndp USING(ndpid)
    INNER JOIN monitora.pi_planointerno pli ON(pli.pliid = pro.pliid)
  WHERE tcp.tcpid = %d
  {$whereAdicional}
DML;
    $stmt = sprintf($sql, $tcpid, $tcpid);
    //ver($stmt, d);
    return $db->carregar($stmt);
}

/**
 * Pega todas as notas de crédito ja submetidas para pagamento
 * usando um determinado TCPID
 * @return array|null
 */
function pagamentosSolicitados($tcpid, $listagem = false, $filtrar = false) {
    global $db;

    $selectAdicional = '';
    if ($listagem) {
        $selectAdicional = <<<DML
, COALESCE(ppa.ppaultimoretornosigef, '-') AS ppaultimoretornosigef,
       CASE WHEN ppa.ppacadastradosigef THEN 'Sim' ELSE 'Não' END AS ppacadastradosigef
DML;
    }

    $whereAdicional = '';
    if ($filtrar) {
        $whereAdicional = <<<DML
AND ppa.ppacadastradosigef = true
DML;
    }

    // -- IMPORTANTE
    // -- Não adicionar ou remover ou reordenar os campos do select sem considerar no XML
    // -- de solicitacao da NC em solicitarCadastroDeNotasDeCreditoSIGEF().
    $sql = <<<DML
SELECT
       ppa.proid,
       ungcodemitente AS unidade_gestora_emitente,
       gescodemitente AS gestao_emitente,
       ungcodproponente as unidade_gestora_favorecida,
       ung.gescod AS gestao_favorecida,
       tcp.tcpnumprocessofnde AS processo,
       tcp.tcpprogramafnde AS programa,
       'false' AS estorno,
       -- '' AS numero_convenio,
       -- '' AS ano_convenio,
       (select sum(pp.ppavlrparcela) from elabrev.previsaoparcela pp where pp.proid in (
            select proid from monitora.previsaoorcamentaria t where t.tcpid = %d and t.prostatus = 'A' and pp.ppacadastradosigef = 'f')
       ) AS valor_custeio,
       0 AS valor_capital,
       '' AS complemento,
       pro.proanoreferencia AS exercicio,
       --ppa.tcpnumtransfsiafi AS nota_credito,
       ppa.codsigefnc AS nota_credito,
       ptr.ptres,
       pro.prgfonterecurso AS fonte_recurso,
       ndpcod AS natureza_despesa,
       pli.plicod AS plano_interno,
       pro.proanoreferencia AS ano_parcela,
       row_number() OVER (ORDER BY ppaid) AS numero_parcela,
       pro.crdmesliberacao AS mes_pagamento,
       pro.provalor AS valor_pagamento,
       to_char(ppa.ppadata, 'DD/MM/YYYY') as ppadata
       {$selectAdicional}
  FROM elabrev.previsaoparcela ppa
    INNER JOIN monitora.previsaoorcamentaria pro ON(pro.proid = ppa.proid)
    inner join monitora.termocooperacao tcp ON(tcp.tcpid = pro.tcpid)
    INNER JOIN public.unidadegestora ung ON (tcp.ungcodproponente = ung.ungcod)
    INNER JOIN monitora.ptres ptr USING(ptrid)
    INNER JOIN public.naturezadespesa ndp USING(ndpid)
    INNER JOIN monitora.pi_planointerno pli ON(pli.pliid = pro.pliid)
  WHERE tcp.tcpid = %d
  {$whereAdicional}
DML;
    $stmt = sprintf($sql, $tcpid, $tcpid);
    //ver($stmt, d);
    return $db->carregar($stmt);
}

/**
 * Faz o select para Pagamento FNDE, somente as celular orçamentárias indicadas
 * via checkbox
 * @return array
 */
function pegaNCPagamentos($tcpid, array $proid) {
    global $db;

    $queryCusteio = sprintf("
        select sum(po.provalor) from monitora.previsaoorcamentaria po
        inner join public.naturezadespesa nd on (po.ndpid = nd.ndpid)
        where tcpid = %d and proid in (".implode(',', $proid).") and substr(nd.ndpcod, 1, 2) = '33'
    ", $tcpid);

    $queryCapital = sprintf("
        select sum(po.provalor) from monitora.previsaoorcamentaria po
        inner join public.naturezadespesa nd on (po.ndpid = nd.ndpid)
        where tcpid = %d and proid in (".implode(',', $proid).") and substr(nd.ndpcod, 1, 2) = '44'
    ", $tcpid);

    $strSQL = "
        SELECT
            ungcodemitente AS unidade_gestora_emitente,
            gescodemitente AS gestao_emitente,
            ungcodproponente as unidade_gestora_favorecida,
            ung.gescod AS gestao_favorecida,
            tcp.tcpnumprocessofnde AS processo,
            tcp.tcpprogramafnde AS programa,
            'false' AS estorno,
            -- '' AS numero_convenio,
            -- '' AS ano_convenio,
            (%s) AS valor_custeio,
            coalesce((%s), 0) AS valor_capital,
            '' AS complemento,
            pro.proanoreferencia AS exercicio,
            --ppa.tcpnumtransfsiafi AS nota_credito,
            ppa.codsigefnc AS nota_credito,
            ptr.ptres,
            pro.prgfonterecurso AS fonte_recurso,
            ndpcod AS natureza_despesa,
            pli.plicod AS plano_interno,
            pro.proanoreferencia AS ano_parcela,
            row_number() OVER (ORDER BY ppaid) AS numero_parcela,
            pro.crdmesliberacao AS mes_pagamento,
            pro.provalor AS valor_pagamento,
            COALESCE(ppa.ppaultimoretornosigef, '-') AS ppaultimoretornosigef,
            CASE WHEN ppa.ppacadastradosigef THEN 'Sim' ELSE 'Não' END AS ppacadastradosigef,
            pro.proid
        FROM elabrev.previsaoparcela ppa
            INNER JOIN monitora.previsaoorcamentaria pro USING(proid)
            inner join monitora.termocooperacao tcp using(tcpid)
            INNER JOIN public.unidadegestora ung ON(tcp.ungcodproponente = ung.ungcod)
            INNER JOIN monitora.ptres ptr USING(ptrid)
            INNER JOIN public.naturezadespesa ndp USING(ndpid)
            INNER JOIN monitora.pi_planointerno pli ON(pli.pliid = pro.pliid)
        WHERE
            ppa.proid in (%s) AND
            ppa.ppacadastradosigef = false
    ";

    $subStmt = sprintf($strSQL, $queryCusteio, $queryCapital, '%s');
    //ver($subStmt, d);
    $stmt = sprintf($subStmt, implode(',', $proid));
    //ver($stmt, d);
    return $db->carregar($stmt);
}

function solicitarCadastroDeNotasDeCreditoSIGEF($dados)
{
    global $db;

    $listaNC = pegaNCPagamentos($dados['tcpid'], $dados['proid']);
    //ver($listaNC, d);

    if (!$listaNC) {
        return false;
    }

    $proids = array();
    foreach ($listaNC as $i => $v) {

        /**
         * Solução paleativa para linhas duplicadas
         */
        if (in_array($v['proid'], $proids)) {
            unset($listaNC[$i]);
        }
        array_push($proids, $v['proid']);

        if (strlen($v['programa']) == 1) {
            $listaNC[$i]['programa'] = str_pad($v['programa'], 2, '0', STR_PAD_LEFT);
        }

        if (!empty($listaNC[$i]['valor_custeio'])) {
            $valorCusteio = explode('.', $listaNC[$i]['valor_custeio']);

            /**
             * Remove zéro a direita de ponto flutuante
             */
            if ($valorCusteio[1] > 0) {
                $valorPontoFlutuante = rtrim($valorCusteio[1], '0');
                $value = "{$valorCusteio[0]}.$valorPontoFlutuante";
            } else {
                $value = $valorCusteio[0];
            }

            $listaNC[$i]['valor_custeio'] = $value;
        }
    }

    $xmlSolicitarSIGEF = <<<XML
<?xml version="1.0" encoding="UTF-8"?>
<request>
    <header>
        <app>%s</app>
        <version>%s</version>
        <created>%s</created>
    </header>
    <body>
        <auth>
            <usuario>%s</usuario>
            <senha>%s</senha>
        </auth>
        <params>
            <unidade_gestora_emitente>%s</unidade_gestora_emitente>
            <gestao_emitente>%s</gestao_emitente>
            <unidade_gestora_favorecida>%s</unidade_gestora_favorecida>
            <gestao_favorecida>%s</gestao_favorecida>
            <processo>%s</processo>
            <programa>%s</programa>
            <estorno>%s</estorno>
            <valor_custeio>%s</valor_custeio>
            <valor_capital>%s</valor_capital>
            <complemento>%s</complemento>
%s
            <sistema>3</sistema>
            <id_solicitante></id_solicitante>
       </params>
    </body>
</request>
XML;
    $xmlDetalhamento = <<<PARTIAL_XML
            <detalhamento>
                <ano_exercicio>%s</ano_exercicio>
                <nota_credito>%s</nota_credito>
                <celula_orcamentaria>
                    <ptres>%s</ptres>
                    <fonte_recurso>%s</fonte_recurso>
                    <natureza_despesa>%s</natureza_despesa>
                    <plano_interno>%s</plano_interno>
                </celula_orcamentaria>
                <ano_parcela>%s</ano_parcela>
                <numero_parcela>%s</numero_parcela>
                <mes_pagamento>%s</mes_pagamento>
                <valor_pagamento>%s</valor_pagamento>
            </detalhamento>
PARTIAL_XML;


    // -- Dadas da requisição comuns a todas as NC
    $dadosIdentificacaoHeader = array(
        'app' => 'SIMEC',
        'version' => 1,
        'created' => date('c'),
        'usuario' => $dados['wsusuario'],
        'senha' => $dados['wssenha'],
    );

    // -- URL da requisição
    if ($_SESSION['baselogin'] == "simec_desenvolvimento" || $_SESSION['baselogin'] == "simec_espelho_producao") {
        $urlWS = 'https://dev.fnde.gov.br/webservices/sigef/integracao/public/index.php/financeiro/pf';
    } else {
        $urlWS = 'https://www.fnde.gov.br/webservices/sigef/index.php/financeiro/pf';
    }

    /**
     * Cliente de WEBSERVICE do FNDE.
     * @see Fnde_Webservice_Client
     */
    include_once APPRAIZ . "includes/classes/Fnde_Webservice_Client.class.inc";

    $dadosDaRequisicao = array_merge(
        $dadosIdentificacaoHeader,
        array_slice($listaNC[0], 0, 10, true)
    );
    $todosDetalhamentos = '';
    foreach ($listaNC as $nc) {
        // -- criando um detalhamento para NC da lista
        $todosDetalhamentos.= vsprintf(
            $xmlDetalhamento,
            array_slice($nc, 10)
        );
//        break;
    }
    $dadosDaRequisicao[] = $todosDetalhamentos;
    $xmlRequisicaoSIGEF .= vsprintf($xmlSolicitarSIGEF, $dadosDaRequisicao);
    //ver(simec_htmlentities($xmlRequisicaoSIGEF), d);

    $paramRequest = array(
        'xml' => $xmlRequisicaoSIGEF,
        'method' => 'solicitar'
    );

    $xmlReturn = Fnde_Webservice_Client::CreateRequest()
        ->setURL($urlWS)
        ->setParams($paramRequest)
        ->execute();

    //$xml->body->row->nu_seq_pf;
    $xml = simplexml_load_string($xmlReturn);

    if (isset($xml->status->error)) {
        $statusText = $xml->status->message->code.' - '. $xml->status->message->text . "\n" .
            $xml->status->error->message->code .' - '. $xml->status->error->message->text;
    } else {
        $statusText = $xml->status->message->code.' - '. $xml->status->message->text;
    }

    // ofuscando a senha para salvar log do xml de envio no db
    $logXmlSend = new SimpleXMLElement($xmlRequisicaoSIGEF);
    $logXmlSend->body->auth->senha = '************';
    $_xmlRequisicaoSIGEF_SaveDb = str_replace('"', '\"', $logXmlSend->asXML());

    $statusText = str_replace("'", "", $statusText);
    $xmlReturn = str_replace("'", "", $xmlReturn);

    $_ppacadastradosigef = ($xml->status->result == 1) ? 'true' : 'false';

    $strSQL_insertLog = "INSERT INTO elabrev.logws_ted
    (lwsrequestcontent,
     lwsrequestheader,
     lwsrequesttimestamp,
     lwsresponsecontent,
     lwsresponseheader,
     lwsresponsetimestamp,
     lwsurl,
     lwsmetodo,
     lwserro,
     lwstcpid)
    VALUES
    ('$_xmlRequisicaoSIGEF_SaveDb',
     '',
     'NOW()',
     '$xmlReturn',
     '',
     'NOW()',
     '$urlWS',
     'solicitar',
     $_ppacadastradosigef,
     {$dados['tcpid']})";

    $db->executar($strSQL_insertLog);

    /*$updateSQL = "
      UPDATE elabrev.previsaoparcela p SET
        ppaultimoretornosigef = '$statusText',
        ppacadastradosigef = $_ppacadastradosigef
      WHERE EXISTS(
        SELECT 1 FROM monitora.termocooperacao tcp WHERE tcp.tcpid = {$dados['tcpid']} AND tcp.codsigefnc = p.codsigefnc
      )
    ";*/
    $updateSQL = "
      UPDATE elabrev.previsaoparcela p SET
        ppaultimoretornosigef = '$statusText',
        ppacadastradosigef = $_ppacadastradosigef
      WHERE proid IN(".implode(',', $dados['proid']).")
    ";
    $db->executar($updateSQL);
    $db->commit();

    $xmlFormated = new SimpleXMLElement($xmlReturn);
    $dom = dom_import_simplexml($xmlFormated)->ownerDocument;
    $dom->formatOutput = true;

    unset($_SESSION['elabrev']['xmlWs']);
    $_SESSION['elabrev']['xmlWs'] = '
    <table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding=3 align="center" width="95%">
        <tr>
            <td>
                <h4>XML</h4>
                <textarea name="xmlRetorno" cols="150" rows="12" readonly="readonly">'.$dom->saveXML().'</textarea>
            </td>
        </tr>
    </table>';


    if ($xml->status->result == 1) {
        return true;
    } else
        return false;
}

/**
 * Retorna informações referentes às notas de crédito
 * retorna no máximo 20 notas de crédito
 *
 * @string $identificador
 * @return bool
 */
function consultar_sigef_nc($identificador) {

    $xmlSigefNC = new SimpleXMLElement('<?xml version="1.0" encoding="ISO-8859-1"?><request></request>');
    $header = $xmlSigefNC->addChild('header');
    $header->addChild('app', 'SIMEC');
    $header->addChild('version', '1.4.1');
    $header->addChild('created', date('c'));
    $body = $xmlSigefNC->addChild('body');
    $auth = $body->addChild('auth');

    if ($_SESSION['baselogin'] == 'simec_desenvolvimento' || $_SESSION['baselogin'] == 'simec_espelho_producao') {
        $auth->addChild('usuario', 'luciab');
        $auth->addChild('senha', 'paulo005');
    } else {
        $auth->addChild('usuario', $_REQUEST['sigefusername']);
        $auth->addChild('senha', $_REQUEST['sigefpassword']);
    }

    $params = $body->addChild('params');
    $params->addChild('sequencial', $identificador);
    //ver(simec_htmlentities($xmlSigefNC->asXML()), d);

    require_once APPRAIZ . "includes/classes/Fnde_Webservice_Client.class.inc";
    $urlWS = 'https://www.fnde.gov.br/webservices/sigef/index.php/financeiro/nc';
    $arrayParams = array(
        'xml' => $xmlSigefNC->asXML(),
        'method' => 'consultar'
    );

    $xml = Fnde_Webservice_Client::CreateRequest()
        ->setURL($urlWS)
        ->setParams($arrayParams)
        ->execute();

    $resultXml = new SimpleXMLElement($xml);

    $domEnvio = dom_import_simplexml($xmlSigefNC)->ownerDocument;
    $domEnvio->formatOutput = true;
    //ver(simec_htmlentities($domEnvio->saveXML()));

    $domRetorno = dom_import_simplexml($resultXml)->ownerDocument;
    $domRetorno->formatOutput = true;
    //ver(simec_htmlentities($domRetorno->saveXML()), d);

    $result = (int) $resultXml->status->result;
    //$sequencial = $resultXml->body->ncs->sequencial;
    $numero_documento_siafi = (string) $resultXml->body->ncs->numero_documento_siafi;
    $termo_compromisso = (string) $resultXml->body->ncs->termo_compromisso;

    $_params = array();
    $_params[] = $domEnvio->saveXML();
    $_params[] = 'NOW()';
    $_params[] = $domRetorno->saveXML();
    $_params[] = 'NOW()';
    $_params[] = $urlWS;
    $_params[] = 'consultar';
    $_params[] = ($result) ? 'TRUE' : 'FALSE';
    $_params[] = $_SESSION['elabrev']['tcpid'];

    if ($_SESSION['baselogin'] != 'simec_desenvolvimento' && $_SESSION['baselogin'] != 'simec_espelho_producao') {
        gravaLogConsultaWsSigefNc($_params);
    }

    if ($result && !empty($numero_documento_siafi) && !empty($termo_compromisso)) {
        return array('numero_documento_siafi' => $numero_documento_siafi,
            'termo_compromisso' => $termo_compromisso);
    } else {
        return false;
    }
}

/**
 * Grava log de consulta ao WS SIGEF
 * @param array $params
 */
function gravaLogConsultaWsSigefNc(array $params) {
    global $db;

    $stmt = sprintf("INSERT INTO elabrev.logwsconsultancsigef
                (
                    lwsncrequestcontent,
                    lwsncrequesttimestamp,
                    lwsncresponsecontent,
                    lwsncresponsetimestamp,
                    lwsncurl,
                    lwsncmetodo,
                    lwsncerro,
                    lwsnctcpid
                )
                VALUES
                (
                    '%s', %s, '%s', %s, '%s', '%s', '%s', '%s'
                )",
        $params[0],
        $params[1],
        $params[2],
        $params[3],
        $params[4],
        $params[5],
        $params[6],
        $params[7]
    );

    //ver($stmt, d);
    $db->executar($stmt);
    $db->commit();
}

/**
 * Verifica se o perfil UO/Equipe Tecnica é vinculado a algum Orgão Concedente
 * @return bool
 */
function uoEquipeTecnicaConcedente() {
    global $db;

    $message = 'Apenas o UO/Equipe Técnica do Concedente pode tramitar';

    if (possui_perfil(array(PERFIL_SUPER_USUARIO))) {
        return true;
    }

    if (possui_perfil(array(UO_EQUIPE_TECNICA))) {

        $strSQL = "
            select * from elabrev.usuarioresponsabilidade
            where
                usucpf = '{$_SESSION['usucpf']}' and
                pflcod = ".UO_EQUIPE_TECNICA." and
                ungcod = (select ungcodconcedente from monitora.termocooperacao where tcpid = {$_SESSION['elabrev']['tcpid']})
        ";

        $linha = $db->pegaLinha($strSQL);
        return ($linha) ? true : $message;
    }

    return $message;
}

function uoEquipeTecnicaProponente() {
    global $db;

    $message = 'Apenas o UO/Equipe Técnica do Proponente pode tramitar';

    if (possui_perfil(array(PERFIL_SUPER_USUARIO))) {
        return true;
    }

    if (possui_perfil(array(UO_EQUIPE_TECNICA))) {

        $strSQL = "
            select * from elabrev.usuarioresponsabilidade
            where
                usucpf = '{$_SESSION['usucpf']}' and
                pflcod = ".UO_EQUIPE_TECNICA." and
                ungcod = (select ungcodproponente from monitora.termocooperacao where tcpid = {$_SESSION['elabrev']['tcpid']})
        ";

        $linha = $db->pegaLinha($strSQL);
        return ($linha) ? true : $message;
    }

    return $message;
}

/**
 * Traz os dados para preenchimento padrão(automatico)
 * do relatório de cumprimento do objeto
 * @param $tcpid
 * @return array|bool
 */
function preenchimentoPadraoDoObjeto($tcpid) {

    global $db;

    if (is_numeric($tcpid) && $tcpid > 0) {

        $strSQL = "SELECT
                        u.ungcod, u.ungdsc, u.ungstatus, u.unicod, u.unitpocod, u.ungabrev, u.ungcnpj,
                        u.ungendereco, u.ungfone, e.estuf, u.muncod, u.ungemail, u.ungbairro, u.ungcep, u.gescod
                    FROM public.unidadegestora u
                        JOIN territorios.municipio m ON (m.muncod = u.muncod)
                        JOIN territorios.estado e ON (e.estuf = m.estuf)
                    WHERE u.ungcod = (
                      SELECT ungcodproponente FROM monitora.termocooperacao tc WHERE tc.tcpid = {$tcpid}
                    )";

        $result = $db->pegaLinha($strSQL);
        if ($result) {

            $foneDddTmp = explode('-', $result['ungfone']);

            if (count($foneDddTmp) == 3) {
                $ddd = $foneDddTmp[0];
                $phone = $foneDddTmp[1].'-'.$foneDddTmp[2];
            }
            else if (count($foneDddTmp) == 2) {
                $ddd = '';
                $phone = $foneDddTmp[0].'-'.$foneDddTmp[1];
            }
            else {
                $ddd = '';
                $phone = '';
            }

            $array_retorno = array(
                'reccnpj'         => $result['ungcnpj'],
                'recnome'         => $result['ungdsc'],
                'recendereco'     => $result['ungendereco'],
                'estuf'           => $result['estuf'],
                'muncod'          => $result['muncod'],
                'reccep'          => $result['ungcep'],
                'rectelefoneddd'  => $ddd,
                'rectelefone'     => $phone,
                'uocod'           => $result['unicod'],
                'ugcod'           => $result['ungcod'],
                'gestaocod'       => $result['gescod'],
            );

            $representante = litaResponsavelUgProp($result['ungcod']);
            if ($representante) {
                $array_retorno['recnomeresponsavel'] = $representante['usunome'];
                $array_retorno['reccpfresponsavel']  = $representante['usucpf'];
                $array_retorno['recemailresposavel'] = $representante['usuemail'];
            }
        }

        return ($array_retorno) ? $array_retorno : false;
    }

    return false;
}

function mesPorExtenso($mes) {

    switch ($mes){
        case 1: $_mes = "Janeiro"; break;
        case 2: $_mes = "Fevereiro"; break;
        case 3: $_mes = "Março"; break;
        case 4: $_mes = "Abril"; break;
        case 5: $_mes = "Maio"; break;
        case 6: $_mes = "Junho"; break;
        case 7: $_mes = "Julho"; break;
        case 8: $_mes = "Agosto"; break;
        case 9: $_mes = "Setembro"; break;
        case 10: $_mes = "Outubro"; break;
        case 11: $_mes = "Novembro"; break;
        case 12: $_mes = "Dezembro"; break;
    }

    return $_mes;
}

/**
 * Download do anexo do relatorio de cumprimento do objeto
 * @param $arqid
 */
function dowloadDocAnexo($arqid) {

    include_once APPRAIZ . 'includes/classes/fileSimec.class.inc';

    if ($arqid) {
        $file = new FilesSimec('arquivo', null, 'public');
        if (!$file->getDownloadArquivo($arqid)) {
            echo '<script type="text/javascript">
                location.href="/elabrev/elabrev.php?modulo=principal/termoCooperacao/cadTermoCooperacao&acao=A&aba=cumprimento"
            </script>';
        }
    }
}

/**
 * Excluir anexo do relatório de cumprimento do objeto
 * @param $arqid
 */
function excluirDocAnexo($arqid) {
    global $db;

    include_once APPRAIZ . 'includes/classes/fileSimec.class.inc';

    if ($arqid != '') {
        $sql = "delete from elabrev.anexos WHERE arqid = {$arqid}";
    }

    if ($db->executar($sql)) {
        $file = new FilesSimec('arquivo', null, 'public');
        $file->excluiArquivoFisico($arqid);

        $db->commit();
        $db->sucesso('principal/termoCooperacao/cadTermoCooperacao', 'acao=A&aba=cumprimento');
    }
}

/**
 * Busca saldo remanejado por termo de compromisso
 * @param $tcpid
 */
function pegaSaldoRemanejado($tcpid) {
    global $db;

    $strSQL = "SELECT (SELECT sum(valor) FROM elabrev.creditoremanejado
               WHERE proid IN (SELECT proid FROM monitora.previsaoorcamentaria WHERE tcpid = {$tcpid} and (prostatus = 'A' OR statusvalorzerado = 't'))) -
               COALESCE((SELECT sum(provalor) FROM monitora.previsaoorcamentaria WHERE tcpid = {$tcpid} AND prostatus = 'A' AND creditoremanejado = 't'), 0) AS saldo";

    $result = $db->pegaLinha($strSQL);

    if ($result > 0) {
        echo '<table style="background-color:#fff">
            <tr style="background-color:#003300;color:#fff">
                <td style="font-weight:bold;">Saldo disponível a ser remanejado</td>
            </tr>
            <tr style="color:#006600;">
                <td style="font-weight:bold;">R$ '.number_format($result['saldo'], 2, ',', '.').'</td>
            </tr>
        </table>';
    }
}

function existeSaldoRemanejado($tcpid, $retornaSaldo = false) {
    global $db;

    $strSQL = "SELECT (SELECT sum(valor) FROM elabrev.creditoremanejado
               WHERE proid IN (SELECT proid FROM monitora.previsaoorcamentaria WHERE tcpid = {$tcpid} and (prostatus = 'A' OR statusvalorzerado = 't'))) -
               COALESCE((SELECT sum(provalor) FROM monitora.previsaoorcamentaria WHERE tcpid = {$tcpid} AND prostatus = 'A' AND creditoremanejado = 't'), 0) AS saldo";

    $result = $db->pegaLinha($strSQL);

    if ($retornaSaldo) {
        return ($result['saldo'] > 0) ? $result['saldo'] : false;
    } else
        return ($result['saldo'] > 0) ? true : false;
}

/**
 * Não permitir exclusão após o arquivo ter sido aprovado pelo Proponente
 * @param $tcpid
 * @return bool
 */
function alreadyBeenExecucao($tcpid) {
    global $db;

    $strSQL = "select
                    u.usunome,
                    u.usucpf,
                    to_char(h.htddata, 'DD/MM/YYYY') as htddata,
                    to_char(h.htddata, 'HH:II:SS') as hora,
                    g.ungdsc
                from monitora.termocooperacao t
                inner join workflow.historicodocumento h on h.docid = t.docid
                inner join workflow.acaoestadodoc a on a.aedid = h.aedid
                inner join seguranca.usuario u on u.usucpf = h.usucpf
                left join unidadegestora g on g.ungcod = t.ungcodconcedente
                where
                    t.tcpid = {$tcpid} and
                    a.esdiddestino in (".EM_ANALISE_DA_SECRETARIA.",".EM_ANALISE_OU_PENDENTE.")
                order by hstid asc";
    //ver($strSQL);
    $result = $db->carregar($strSQL);
    return ($result) ? true : false;
}

/**
 * Consulta uma NC a partir do identificado interno do SIGEF
 * para saber se a mesma já foi efetivada junto ao SIGEF
 *
 * @param $sequencial
 * @return string
 */
function verifica_nc_sigef($sequencial) {
    global $db;

    $retorno = consultar_sigef_nc($sequencial);

    if (is_array($retorno)) {
        $rsProid = $db->carregar("select proid, provalor from monitora.previsaoorcamentaria where sigefid = {$sequencial}");

        if (count($rsProid)) {

            $db->executar("UPDATE monitora.previsaoorcamentaria
                            SET codsigefnc = '{$retorno['numero_documento_siafi']}'
                            WHERE sigefid = {$sequencial}");
            /*echo "UPDATE monitora.previsaoorcamentaria
                            SET codsigefnc = '{$retorno['numero_documento_siafi']}'
                            WHERE sigefid = {$sequencial}<br>";*/

            foreach ($rsProid as $previsao) {
                // grava na funcionalidade de parcela
                $sql = "insert into elabrev.previsaoparcela(proid, ppavlrparcela, codsigefnc, tcpnumtransfsiafi, ppacadastradosigef)
                    values('{$previsao['proid']}', {$previsao['provalor']}, '{$retorno['numero_documento_siafi']}', '{$retorno['termo_compromisso']}', 'f')";
                //echo $sql . '<br>';
                $db->executar( $sql );
                $db->commit();
            }
            $sql = "insert into elabrev.logtermowssigef (tcpid, logdsc, logxmlenvio, logxmlretorno) values ({$_SESSION['elabrev']['tcpid']}, 'Sucesso', 'verificacao de NC - efetivada', 'verificacao de NC - efetivada')";
            $db->executar($sql);
            $db->commit();

            echo '<script type="text/javascript">alert("Nota de crédito efetivada com sucesso junto ao SIGEF!");</script>';
        } else {
            echo '<script type="text/javascript">alert("Não foi encontrado previsão orçamentária para o termo solicitado!");</script>';
        }
    } else {
        $sql = "insert into elabrev.logtermowssigef (tcpid, logdsc, logxmlenvio, logxmlretorno) values ({$_SESSION['elabrev']['tcpid']}, 'identificador da nc vazio', 'verificacao de NC - nao efetivada', 'verificacao de NC - nao efetivada')";
        $db->executar($sql);
        $db->commit();

        echo '<script type="text/javascript">alert("A nota de crédito ainda não foi efetivada junto ao SIGEF, tente mais tarde!");</script>';
    }
}

function quemPodeAnexarArquivo($habilitaInserir) {
    $quemPodeAnexar = array(UO_EQUIPE_TECNICA, PERFIL_AREA_TECNICA_FNDE, PERFIL_CGSO, PERFIL_SUPER_USUARIO);
    $perfis = pegaPerfilGeral($_SESSION['usucpf'], $_SESSION['sisid']);

    foreach ($perfis as $perfil) {
        if (in_array($perfil, $quemPodeAnexar)) {
            $habilitaInserir = 'S';
        }
    }

    return $habilitaInserir;
}

/**
 * Verifica se o termo possui pendencia do retorno do identificador
 * retorna true se sim
 * @return bool|null|string|void
 */
function verificaEfetivacaoNCSigef() {
    global $db;

    $sql = "select count(*) AS pendencia
      from monitora.previsaoorcamentaria
    where
        sigefid is not null and
        codsigefnc is null and
        tcpid = {$_SESSION['elabrev']['tcpid']}";

    /*$sql = "select count(*) from monitora.termocooperacao
            where
                codsigefnc is not null and
                retornosigefnc = false and
                tcpid = {$_SESSION['elabrev']['tcpid']}";*/

    $result = $db->pegaUm($sql);
    return ($result) ? $result : null;
}

function teste_superUser() {
    global $db;
    return $db->testa_superuser();
}

/**
 * Verifica quantas vezes o termo já foi para execucao
 * @return bool|string|void
 */
function termoJaFoiParaExecucao() {
    global $db;

    $strSQL = "select
        count(*) as qtd
    from monitora.termocooperacao t
        inner join workflow.historicodocumento h on h.docid = t.docid
        inner join workflow.acaoestadodoc a on a.aedid = h.aedid
        inner join seguranca.usuario u on u.usucpf = h.usucpf
        left join unidadegestora g on g.ungcod = t.ungcodconcedente
    where
        t.tcpid = {$_SESSION['elabrev']['tcpid']} and
        a.esdiddestino in (".EM_EXECUCAO.")";

    $value = $db->pegaUm($strSQL);
    return ($value) ? $value : false;
}

/**
 * Relatorio de Cumprimento do Objeto foi Preenchido
 * @return array|bool|void
 */
function rco_foi_preenchido() {
    global $db;
    $strSQL = "select * from elabrev.relatoriocumprimento where tcpid = {$_SESSION['elabrev']['tcpid']}";
    $result = $db->pegaLinha($strSQL);
    return ($result) ? $result : false;
}

function pega_data_ultima_execucao() {
    global $db;

    $strSQL = "select
        to_char(h.htddata, 'DD/MM/YYYY') as htddata,
        to_char(h.htddata, 'HH:II:SS') as hora
    from monitora.termocooperacao t
    inner join workflow.historicodocumento h on h.docid = t.docid
    inner join workflow.acaoestadodoc a on a.aedid = h.aedid
    inner join seguranca.usuario u on u.usucpf = h.usucpf
    left join unidadegestora g on g.ungcod = t.ungcodconcedente
    where
        t.tcpid = {$_SESSION['elabrev']['tcpid']} and
        a.esdiddestino in (".EM_EXECUCAO.")
    order by hstid desc limit 1";

    $result = $db->pegaLinha($strSQL);
    return ($result) ? $result : false;
}

/**
 * função para corrigir problemas de codificação no preenchimendo da aba
 * @param $tcpid
 */
function dumpInsertParecerTecnico($tcpid) {
    global $db;

    $sql = "select
            tcpparecertecnico,
            tcpconsidentproponente,
            tcpconsidproposta,
            tcpconsidobjeto,
            tcpconsidobjetivo,
            tcpconsidjustificativa,
            tcpconsidvalores,
            tcpconsidcabiveis,
            tcpusucpfparecer
        from monitora.termocooperacao
        WHERE tcpid = $tcpid";

    $linha = $db->pegaLinha($sql);

    $insert = "update monitora.termocooperacao set
                tcpconsidentproponente = '{$linha['tcpconsidentproponente']}',
                tcpconsidproposta = '{$linha['tcpconsidproposta']}',
                tcpconsidobjeto = '{$linha['tcpconsidobjeto']}',
                tcpconsidobjetivo = '{$linha['tcpconsidobjetivo']}',
                tcpconsidjustificativa = '{$linha['tcpconsidjustificativa']}',
                tcpconsidvalores = '{$linha['tcpconsidvalores']}',
                tcpconsidcabiveis = '{$linha['tcpconsidcabiveis']}',
                tcpusucpfparecer = '{$linha['tcpusucpfparecer']}'
            WHERE tcpid = $tcpid";

    ver($insert, d);
}

/**
 * Verifica se existe NC cadastrada para cada previsao orcamentaria, beaseado no mes de liberacao
 * @param $tpcid
 * @return bool
 */
function permiteEnviarExecucao($tcpid) {
    global $db;

    $estadoAtual = pegarEstadoAtual($tcpid);

    $sqlParcelas = "
        select
            count(proid) as qtd, proanoreferencia, crdmesliberacao
        from monitora.previsaoorcamentaria
        where
            tcpid = {$tcpid} and
            crdmesliberacao is not null and
            proanoreferencia = '{$_SESSION['exercicio']}' and
            prostatus = 'A'
        group by
            crdmesliberacao, proanoreferencia
    ";

    $previsoes = $db->carregar($sqlParcelas);

    if (is_array($previsoes)) {
        foreach ($previsoes as $previsao) {
            $sql = "
                select * from elabrev.previsaoparcela where proid in (
                    select proid from monitora.previsaoorcamentaria where tcpid = {$tcpid} and prostatus = 'A' and proanoreferencia = '{$_SESSION['exercicio']}'
                )
            ";

            $result = $db->carregar($sql);
            if ($result) return true;
        }
    }

    return false;
}

/**
 * Busca todos os termos que tem o FNDE como concedente
 * E que o responsável pela politica é alguma diretoria do FNDE
 * Se a condição for verdadeira retorna false
 * para ocultar a ação de "Enviar para representante legal do proponente"
 * @param $tcpid
 */
function verificaConcedenteFNDEpoliticaDiretoriaFnde() {

    global $db;

    $tcpid = $_SESSION['elabrev']['tcpid'];

    $rs = $db->pegaLinha("select tc.tcpid, tc.ungcodconcedente from monitora.termocooperacao tc where tc.tcpid = {$tcpid}");
    if ($rs) {
        if ($rs['ungcodconcedente'] != '153173') return false;
    }

    $strSQL = "
        select
            tc.tcpid, d.dircod, d.dirdsc, d.dirstatus, d.ungcod
        from monitora.termocooperacao tc
        INNER JOIN elabrev.diretoria d ON (dircodpoliticafnde = d.dircod)
        where
            dircodpoliticafnde is not null
            and ungcodconcedente = '153173'
            and tc.tcpid = {$tcpid}
    ";

    $rs = $db->carregar($strSQL);
    return ($rs) ? true : false;
}

/**
 * Busca todos os termos que tem o FNDE como concedente
 * E que o responsável pela politica é alguma diretoria do FNDE
 * Se a condição for verdadeira retorna false
 * para ocultar a ação de "Enviar para representante legal do proponente"
 * @param $tcpid
 */
function verificaConcedenteFNDEpoliticaSecretariaFnde() {

    global $db;

    $tcpid = $_SESSION['elabrev']['tcpid'];

    $rs = $db->pegaLinha("select tc.tcpid, tc.ungcodconcedente from monitora.termocooperacao tc where tc.tcpid = {$tcpid}");
    if ($rs) {
        if ($rs['ungcodconcedente'] != '153173') return false;
    }

    $strSQL = "
        select
          tc.tcpid, d.dircod, d.dirdsc, d.dirstatus, d.ungcod
        from monitora.termocooperacao tc
        INNER JOIN elabrev.diretoria d ON (ungcodpoliticafnde = d.ungcod)
        where
            ungcodpoliticafnde is not null
            and ungcodconcedente = '153173'
            and tc.tcpid = {$tcpid}
    ";

    $rs = $db->carregar($strSQL);
    return ($rs) ? true : false;
}

/**
 * Condição do workflow,
 * Quando o termo esta em aprovação pela Diretoria
 * E o concedente é o FNDE, não tramitar direto para
 * Representante Legal do Concedente
 * @return bool
 */
function verificaTermoFNDEcomPolitica() {

    global $db;

    $tcpid = $_SESSION['elabrev']['tcpid'];

    $strSQL = "
        SELECT DISTINCT
            tc.tcpid, tc.ungcodconcedente, tc.ungcodproponente
        FROM monitora.termocooperacao tc
        WHERE
            (ungcodpoliticafnde is not null OR dircodpoliticafnde is not null) AND
            tc.tcpstatus = 'A' AND
            tc.ungcodconcedente  = '153173' AND
            tc.tcpid = {$tcpid};
    ";

    $rs = $db->pegaLinha($strSQL);
    return ($rs) ? FALSE : TRUE;
}

function possui_historico_execucao() {
    global $db;

    $tcpid = $_SESSION['elabrev']['tcpid'];

    if ($tcpid) {

        $strSQL = "
            select
                count(*) as is_execucao
            from
                workflow.historicodocumento
            where
                docid = (select docid from monitora.termocooperacao where tcpid = {$tcpid}) and
                aedid in (1609, 1618, 1650)
        ";

        $is_execucao = $db->pegaUm($strSQL);
        return ($is_execucao > 0) ? true : false;
    }
}


function nao_possui_historico_execucao() {
    global $db;

    $tcpid = $_SESSION['elabrev']['tcpid'];

    if ($tcpid) {

        $strSQL = "
            select
                count(*) as is_execucao
            from
                workflow.historicodocumento
            where
                docid = (select docid from monitora.termocooperacao where tcpid = {$tcpid}) and
                aedid in (1609, 1618, 1650)
        ";

        $is_execucao = $db->pegaUm($strSQL);
        return ($is_execucao == 0) ? true : false;
    }
}

function emDilegenciaSemExecucao($tcpid) {
    global $db;

    if (is_null($tcpid)) {
        return false;
    }

    $strSQL = "
        select
            u.usunome,
            u.usucpf,
            to_char(h.htddata, 'DD/MM/YYYY') as htddata,
            to_char(h.htddata, 'HH:II:SS') as hora,
            g.ungdsc
        from monitora.termocooperacao t
        inner join workflow.historicodocumento h on h.docid = t.docid
        inner join workflow.acaoestadodoc a on a.aedid = h.aedid
        inner join seguranca.usuario u on u.usucpf = h.usucpf
        left join unidadegestora g on g.ungcod = t.ungcodconcedente
        where
            t.tcpid = {$tcpid} and
            a.esdiddestino in (643) and
            a.esdiddestino not in (639)
        order by hstid asc
    ";

    $result = $db->carregar($strSQL);
    return ($result) ? true : false;
}

function verificaEquipeTecnicaConcedente() {
    global $db;

    if (possui_perfil(array(PERFIL_SUPER_USUARIO))) {
        return true;
    }

    if (possui_perfil(array(UO_EQUIPE_TECNICA))) {

        /**
         * Se o termo for FNDE
         * e politica for alguma secretaria (SETEC, SECAD, SEB)
         * pega como codigo concedente o codigo da UG da secretaria
         */
        $secretarias = array(
            '150016',
            '150028',
            '150019',
        );

        $sqlComplement = "(select ungcodconcedente from monitora.termocooperacao where tcpid = {$_SESSION['elabrev']['tcpid']})";
        $rsSec = $db->pegaUm("select ungcodpoliticafnde from monitora.termocooperacao where tcpid = {$_SESSION['elabrev']['tcpid']}");
        //ver($rsSec);
        if ($rsSec) {
            if (in_array($rsSec, $secretarias)) {
                $sqlComplement = "(select ungcodpoliticafnde from monitora.termocooperacao where tcpid = {$_SESSION['elabrev']['tcpid']})";
            }
        }

        $strSQL = "
            select * from elabrev.usuarioresponsabilidade
            where
                usucpf = '{$_SESSION['usucpf']}' and
                pflcod = ".UO_EQUIPE_TECNICA." and
                rpustatus = 'A' and
                ungcod = {$sqlComplement}
        ";

        //ver($strSQL);
        $linha = $db->pegaLinha($strSQL);
        return ($linha) ? true : false;
    }

    return false;
}

function verificaEquipeTecnicaProponente() {
    global $db;


    if (possui_perfil(array(PERFIL_SUPER_USUARIO))) {
        return true;
    }

    if (possui_perfil(array(UO_EQUIPE_TECNICA))) {

        $strSQL = "
            select * from elabrev.usuarioresponsabilidade
            where
                usucpf = '{$_SESSION['usucpf']}' and
                pflcod = ".UO_EQUIPE_TECNICA." and
                rpustatus = 'A' and
                ungcod = (select ungcodproponente from monitora.termocooperacao where tcpid = {$_SESSION['elabrev']['tcpid']})
        ";

        $linha = $db->pegaLinha($strSQL);
        return ($linha) ? true : false;
    }

    return false;
}


function rco_prazo_vencido() {
    global $db;

    $rco = new RelatorioCumprimentoObjeto();

    $isFillRCO = $db->pegaUm("
        select rel.recid
        from elabrev.relatoriocumprimento rel
        join elabrev.relcumprimentonc nc on nc.recid = rel.recid
        where
        rel.tcpid = {$_SESSION['elabrev']['tcpid']} AND
        rel.recstatus = 'A' AND
        nc.rpustatus = 'A'
    ");

    $tv = $rco->termoVencido($_SESSION['elabrev']['tcpid']);
    //dbg($tv, d);
    if (possui_perfil(array(PERFIL_SUPER_USUARIO)) && $tv) {
        if ($isFillRCO)
            return true;
        else
            return 'Falta preencher o relatório de cumprimento.';
    }

    if (possui_perfil(array(UO_EQUIPE_TECNICA)) || possui_perfil(array(PERFIL_SUPER_USUARIO))) {

        $strSQL = "
            select * from elabrev.usuarioresponsabilidade
            where
                usucpf = '{$_SESSION['usucpf']}' and
                pflcod = ".UO_EQUIPE_TECNICA." and
                rpustatus = 'A' and
                ungcod = (select ungcodproponente from monitora.termocooperacao where tcpid = {$_SESSION['elabrev']['tcpid']})
        ";

        $linha = $db->pegaLinha($strSQL);
        if ($linha && $rco->termoVencido($_SESSION['elabrev']['tcpid'])) {

            if ($isFillRCO)
                return true;
            else
                return 'Falta preencher o relatório de cumprimento.';

        } else
            return false;
    }

    return false;
}

/**
 * Trava para termos que venceram a data de analise do RCO pelo coordenador
 * @return bool|string
 */
function verificaAprovacaoGestor() {
    global $db;

    if (!$_SESSION['elabrev']['tcpid']) {
        return false;
    }

    $objeto = new RelatorioCumprimentoObjeto();
    if ($objeto->termosPendenciaAprovacaoCoordenacao() && !teste_superUser()) {
        return 'Relatório de Cumprimento do Objeto, pendente de aprovação pelo Gestor Orçamentário do Proponente';
    } else {
        return true;
    }
}

function buildQuery($tcpid = null) {
    global $db;

    if (!$tcpid) return false;

    $strSQL = "
        SELECT * FROM
        (select distinct
            t.tcpid,
            t.ungcodproponente,
            t.ungcodconcedente,
            t.docid,
            to_char((select htddata from workflow.historicodocumento hst where hst.aedid IN(1609, 1618, 2440) and hst.docid = t.docid order by hstid desc limit 1), 'DD-MM-YYYY') as data_execucao,
            case
            when (select count(*) from workflow.historicodocumento hst where hst.aedid IN(1609, 1618, 2440) and hst.docid = t.docid) = 1 then
                (select crdmesexecucao from monitora.previsaoorcamentaria where tcpid = t.tcpid and prostatus = 'A' and crdmesexecucao is not null order by crdmesexecucao asc limit 1)
            when (select count(*) from workflow.historicodocumento hst where hst.aedid IN(1609, 1618, 2440) and hst.docid = t.docid) > 1 then
                (select crdmesexecucao from monitora.previsaoorcamentaria where tcpid = t.tcpid and prostatus = 'A' and crdmesexecucao is not null order by crdmesexecucao desc limit 1)
            else
                null
            end AS vigencia,
            (select count(*) from workflow.historicodocumento hst where hst.aedid IN(1609, 1618, 2440) and hst.docid = t.docid) as qtd_execucao
            from monitora.termocooperacao t
            inner join unidadegestora g on g.ungcod = t.ungcodconcedente
            where
            t.ungcodproponente = (select ungcodproponente from monitora.termocooperacao where tcpid = {$tcpid})
            and
            t.ungcodconcedente = (select ungcodconcedente from monitora.termocooperacao where tcpid = {$tcpid})
            and
            t.tcpid = {$tcpid}
        ) AS vTable
    ";

    $rs = $db->pegaLinha($strSQL);
    return ($rs) ? $rs : null;
}

function termoVencido($tcpid) {
    global $db;

    $row = buildQuery($tcpid);
    $row = (is_array($row)) ? $row : array();
    $estadoAtual = _pegaEstadoAtual($tcpid);

    if ($row && !empty($row['data_execucao']) && !empty($row['vigencia'])
        && (!rcoPreenchido($tcpid) || !rcoEmcaminhadoGestor($tcpid))
        //|| $estadoAtual == TERMO_EM_DILIGENCIA_RELATORIO)
    ) {

        $row['prazo_extra'] = 2; //60 dias = dois meses
        $row['expira'] = $row['vigencia']; // + $row['prazo_extra'];

        $data = new DateTime($row['data_execucao']);
        $data->modify("+{$row['expira']} month");
        $row['data_expira'] = $data->format('d-m-Y');
        //ver($row, d);

        $dateNow = new DateTime();
        if ($data < $dateNow)
            return $row['tcpid'];
        else
            return false;
    }
}

function rcoPreenchido($tcpid) {
    global $db;

    $stmt = sprintf("SELECT * FROM elabrev.relatoriocumprimento WHERE recstatus = '%s' AND tcpid = %d", 'A', $tcpid);
    $result = $db->pegaLinha($stmt);
    return ($result) ? true : false;
}

function rcoEmcaminhadoGestor($tcpid) {
    global $db;

    $strSQL = "
        select * from workflow.historicodocumento where aedid in (
            SELECT
                ae.aedid
            FROM workflow.acaoestadodoc ae
                INNER JOIN workflow.estadodocumento ed ON (ed.esdid = ae.esdidorigem)
                INNER JOIN workflow.estadodocumentoperfil dp ON (dp.aedid = ae.aedid)
            where
                ed.tpdid = 97 and aedstatus = 'A'
                and aeddscrealizar ilike '%Encaminhar o relatório de cumprimento do objeto%'
            order by ae.esdidorigem
        )
        and docid = (select docid from monitora.termocooperacao where tcpid = {$tcpid})
        order by htddata desc
    ";

    $resultado = $db->carregar($strSQL);
    return ($resultado) ? $resultado : false;
}