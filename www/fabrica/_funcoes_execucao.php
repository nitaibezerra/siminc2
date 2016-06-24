<?php

function adicionaTempoPausa($odsid) { 
    global $db;

    $sqlDocId = "select
					case when tosid = " . TIPO_OS_GERAL . "
					then docid
					else docidpf
					end AS doc_id
					, tosid
				from
					fabrica.ordemservico
				where
				odsid = " . $odsid;

    $retornoSql = $db->pegaLinha($sqlDocId);

    if (!empty($retornoSql['doc_id']) && !empty($retornoSql['tosid'])) {
        $dadosVerificacao = array();
        //$esdidPausa			= (TIPO_OS_GERAL == $retornoSql['tosid'] ? WF_ESTADO_OS_PAUSA : WF_ESTADO_CPF_PAUSA) ;
        $esdidPausa = WF_ESTADO_OS_PAUSA;
        $docmentoId = (TIPO_OS_GERAL == $retornoSql['tosid'] ? 'os.docid' : 'os.docidpf');
        $tipoOrdemServico = (TIPO_OS_GERAL == $retornoSql['tosid'] ? WORKFLOW_ORDEM_SERVICO : WORKFLOW_CONTAGEM_PF);

        $sql = "update fabrica.ordemservico
					set odsdtprevtermino = date(odsdtprevtermino) + ( ( date(current_date) - date(hd2.htddata) ) + 1)
				from (
					select
					htddata
					from fabrica.ordemservico os
					inner join workflow.documento 			dc ON dc.docid = " . $docmentoId . "
					inner join workflow.historicodocumento 	hd ON dc.docid = hd.docid
					inner join workflow.acaoestadodoc 		a  ON a.aedid  = hd.aedid
					inner join workflow.estadodocumento 	ed ON ed.esdid = dc.esdid
					where
					os.odsid 	   	   = " . $odsid . "
					and ed.tpdid 	   = " . $tipoOrdemServico . "
					and a.esdiddestino = " . $esdidPausa . "
					order by hd.htddata desc
					limit 1
				) as hd2
				where odsid = " . $odsid;

        $db->executar($sql);
        $db->commit();

        return true;
    } else {

        return false;
    }
}

function adicionaTempoPausaSS($scsid) {
    global $db;

    $sqlDocId = "select docid
                 from fabrica.solicitacaoservico
				where scsid = " . $scsid;

    $retornoSql = $db->pegaLinha($sqlDocId);

    $dados = wf_pegarAcao(WF_ESTADO_SS_PAUSA, WF_ESTADO_DETALHAMENTO);
    $sqlVerificaPausaDia = "SELECT COUNT(hst.hstid) as total_pausa
                            FROM workflow.historicodocumento hst
                            WHERE hst.docid = {$retornoSql['docid']}
                            AND hst.aedid = {$dados['aedid']}
                            AND to_char(hst.htddata, 'YYYY-MM-DD') = '" . date('Y-m-d') . "'";

    $totalPausa = $db->pegaUm($sqlVerificaPausaDia);

    if ($totalPausa > 1) {
        return true;
    }

    if (!empty($retornoSql['docid'])) {
        $sql = "update fabrica.analisesolicitacao
					set ansprevtermino = date(ansprevtermino) + ( ( date(current_date) - date(hd2.htddata) ) + 1)
				from (
					select
					htddata
					from fabrica.solicitacaoservico ss
					inner join workflow.documento 			dc ON dc.docid = ss.docid
					inner join workflow.historicodocumento 	hd ON dc.docid = hd.docid
					inner join workflow.acaoestadodoc 		a  ON a.aedid  = hd.aedid
					inner join workflow.estadodocumento 	ed ON ed.esdid = dc.esdid
					where ss.scsid 	   	   = " . $scsid . "
					and ed.tpdid 	   = " . WORKFLOW_SOLICITACAO_SERVICO . "
					and a.esdiddestino = " . WF_ESTADO_SS_PAUSA . "
					order by hd.htddata desc
					limit 1
				) as hd2
				where scsid = " . $scsid;

        $db->executar($sql);
        $db->commit();

        return true;
    } else {

        return false;
    }
}

//problema svn
function removerObservacaoServico($dados) {
    global $db;
    $sql = "UPDATE
				fabrica.ordemservicoobservacao
			SET
				osostatus='I'
			WHERE osoid={$dados['osoid']};";

    $db->executar($sql);
    $db->commit();

    echo "<script>
			alert('Observação apagada com sucesso');
			window.location='fabrica.php?modulo=principal/cadOSExecucao&acao=A&odsid=" . $dados['odsid'] . "';
		  </script>";
}

function downloadAnexoExecucao($dados) {
    include_once APPRAIZ . "includes/classes/fileSimec.class.inc";
    $file = new FilesSimec("anexoordemservico", NULL, "fabrica");
    $file->getDownloadArquivo($dados['arqid']);
}

function inserirProfissionais($dados) {
    global $db;

    if ($dados['usucpf'][0]) {

        $sql = "DELETE
				FROM
				fabrica.profissionalos
	    		WHERE odsid='" . $dados['odsid'] . "';";
        $db->executar($sql);

        foreach ($dados['usucpf'] as $usucpf) {

            $sql = "INSERT INTO fabrica.profissionalos(odsid, usucpf)
		    		VALUES ('" . $dados['odsid'] . "', '" . $usucpf . "');";
            $db->executar($sql);
        }
    }

    if ($_FILES['arquivo']) {
        if ($_FILES['arquivo']['error'] == 0) {

            include_once APPRAIZ . "includes/classes/fileSimec.class.inc";

            $campos = array("taoid" => "'" . $dados['taoid'] . "'",
                "odsid" => "'" . $dados['odsid'] . "'",
                "aosdsc" => "'" . $dados['aosdsc_'] . "'",
                "aosdtinclusao" => "NOW()",
                "aosstatus" => "'A'");

            $file = new FilesSimec("anexoordemservico", $campos, "fabrica");
            $file->setUpload((($dados['aosdsc_']) ? $dados['aosdsc_'] : NULL), $key = "arquivo");
        }
    }

    //salvando a observação
    if ($dados['osodesc']) {
        $sql = "INSERT
				INTO
				fabrica.ordemservicoobservacao
					(osodesc, osodtinclusao, odsid, usucpf)
				VALUES
					('" . $dados['osodesc'] . "', NOW(), '" . $dados['odsid'] . "', '" . $_SESSION['usucpf'] . "');";
        $db->executar($sql);
    }

    $db->commit();

    echo "<script>
			alert('Registros inseridos com sucesso');
			window.location='fabrica.php?modulo=principal/cadOSExecucao&acao=A&odsid=" . $dados['odsid'] . "';
		  </script>";
}

function carregarMenuExecucaoSolicitacao() {
    // monta menu padrão contendo informações sobre as entidades
    $menu = array(0 => array("id" => 1, "descricao" => "Executar Solicitação", "link" => "/fabrica/fabrica.php?modulo=principal/abrirSolicitacao&acao=A&ansid=" . $_SESSION['fabrica_var']['ansid'] . "&scsid=" . $_SESSION['fabrica_var']['scsid']),
        1 => array("id" => 2, "descricao" => "Observações", "link" => "/fabrica/fabrica.php?modulo=principal/cadSSObservacao&acao=A&tipoobs=cadExecucao"),
        2 => array("id" => 3, "descricao" => "Anexos da ordem de serviço", "link" => "/fabrica/fabrica.php?modulo=principal/cadDetalhamentoAnexos&acao=D"),
        3 => array("id" => 4, "descricao" => "Monitoramento / Controle de riscos", "link" => "/fabrica/fabrica.php?modulo=principal/monitoramentoRiscos&acao=C"),
        4 => array("id" => 5, "descricao" => "Providências", "link" => "/fabrica/fabrica.php?modulo=principal/providencias&acao=A")
    );
    return $menu;
}

//estava no estado pendente - ação(Enviar para Execução) - pos ação
function envialEmailProfissionais($odsid, $usucpf = '') {
    global $db;
    // dados
    $sql = "SELECT d.usucpf FROM fabrica.ordemservico o
			LEFT JOIN workflow.historicodocumento d ON o.docid=d.docid
			WHERE odsid='" . $odsid . "' and aedid=" . WF_ACAO_OS_EMEXECUCAO;
    $usucpfdemandante = $db->pegaUm($sql);

    if (!$usucpfdemandante) {
        return false;
    }

    // não pode haver mais de um sistema por contrato
    $sql = "SELECT c.sidid, o.odsdetalhamento, to_char(o.odsdtprevinicio::date,'YYYY-MM-DD') as odsdtprevinicio, to_char(o.odsdtprevtermino::date,'YYYY-MM-DD') as odsdtprevtermino
			FROM fabrica.ordemservico o
			LEFT JOIN fabrica.analisesolicitacao a ON  o.scsid = a.scsid
			LEFT JOIN fabrica.solicitacaoservico c ON a.scsid = c.scsid
			WHERE odsid='" . $odsid . "' LIMIT 1";
    $os_dados = $db->pegaLinha($sql);

    if (!$os_dados['sidid']) {
        return false;
    }

    // dados
    if ($usucpf)
        $andUsucpf = " and prf.usucpf = '$usucpf' ";
    $sql = "SELECT usu.usucpf, usu.usunome, usu.usuemail FROM seguranca.usuario usu
			INNER JOIN fabrica.profissionalos prf ON usu.usucpf = prf.usucpf
			WHERE odsid='" . $odsid . "' $andUsucpf ";


    $profissionais = $db->carregar($sql);
    //dbg($profissionais,1);

    if ($profissionais[0]) {
        foreach ($profissionais as $prof) {

            $sql = "INSERT INTO demandas.demanda
					(
						usucpfdemandante,
						usucpfanalise,
						usucpfclassificador,
						usucpfexecutor,
						usucpfinclusao,
						tipid,
						sidid,
						dmdtitulo,
						dmddsc,
						dmdreproducao,
						dmdstatus,
						laaid,
						dmdsalaatendimento,
						unaid,
						dmdqtde,
						dmdhorarioatendimento,
						dmdatendremoto,
						dmddatainclusao,
						dmddataclassificacao,
						dmdatendurgente,
						dmdjusturgente,
						dmdclassificacao,
						dmdclassificacaosistema,
						priid,
						dmddatainiprevatendimento,
			         	dmddatafimprevatendimento,
						odsid
					)VALUES(
						'" . $usucpfdemandante . "',
						'" . $usucpfdemandante . "',
						'" . $usucpfdemandante . "',
						'" . $prof['usucpf'] . "',
						'" . $_SESSION['usucpf'] . "',
						1,
						{$os_dados['sidid']},
						'" . substr($os_dados['odsdetalhamento'], 0, 250) . "',
						'" . substr($os_dados['odsdetalhamento'], 0, 250) . "',
						'',
						'A',
						NULL,
						'',
						1,
						1,
						'C',
						'f',
						'" . date("Y-m-d H:i:s") . "',
						'" . date("Y-m-d H:i:s") . "',
						'f',
						'',
						'S',
						'5',
						1,
						'" . $os_dados['odsdtprevinicio'] . " 08:00:00',
						'" . $os_dados['odsdtprevtermino'] . " 18:00:00',
						'" . $odsid . "'
					) RETURNING dmdid";

            $dmdid = $db->pegaUm($sql);

            if ($dmdid) {


                /*
                 * Pega nome da demanda
                 */
                $sqlDescricao = "SELECT
								  REPLACE (dmdtitulo, chr(92), chr(47)) AS titulo,
								  docid
								 FROM
								  demandas.demanda
								 WHERE
								  dmdid = $dmdid ";

                $descricao = $db->pegaLinha($sqlDescricao);

                $docdsc = "Cadastramento DEMANDAS - " . $descricao['titulo'];

                if (!$descricao['docid']) {
                    /*
                     * cria documento WORKFLOW
                     */
                    $tpdid = DEMANDA_WORKFLOW_GENERICO;
                    $docid = wf_cadastrarDocumento($tpdid, $docdsc);

                    /*
                     * Atualiza docid na demanda
                     */
                    //if($docid){
                    $sql = "UPDATE demandas.demanda SET
							 docid = '" . $docid . "'
							WHERE
							 dmdid = " . $dmdid;

                    $db->executar($sql);


                    $cmddsc = "";
                    $aedid = WF_DEMANDA_ACAO_ANALISE_PARA_ATENDIMENTO; //envia para atendimento
                    $dadosVerificacao = (array) "a:1:{s:6:\"unicod\";s:0:\"\";}";

                    //tramita o documento
                    wf_alterarEstado($docid, $aedid, $cmddsc, $dadosVerificacao);
                }
            }
        }

        $db->commit();
    }


    if ($_SERVER['HTTP_HOST'] == "localhost" || $_SERVER['HTTP_HOST'] == "simec-local") {
        return true;
    }


    require_once APPRAIZ . 'includes/phpmailer/class.phpmailer.php';
    require_once APPRAIZ . 'includes/phpmailer/class.smtp.php';

    $mensagem = new PHPMailer();
    $mensagem->persistencia = $db;
    $mensagem->Host = "localhost";
    $mensagem->Mailer = "smtp";
    $mensagem->FromName = "Fábrica de softwares";
    $mensagem->From = $_SESSION['email_sistema'];
    $mensagem->Body = "Você foi atribudo como profissional responsavel pela ordem de serviço #" . $odsid . "";
    $mensagem->Subject = "Atribuição na ordem de serviço (Em execução)";

    if ($profissionais[0]) {
        foreach ($profissionais as $prof) {
            $mensagem->AddAddress($prof['usuemail'], $prof['usunome']);
        }
    }

    $mensagem->IsHTML(true);
    $mensagem->Send();

    return true;
}

function gravarAvaliacaoOS($dados) {
    global $db;
    $avoid = $db->pegaUm("SELECT avoid FROM fabrica.avaliacaoos WHERE odsid='" . $dados['odsid'] . "'");

    if ($avoid) {

        $sql = "UPDATE fabrica.avaliacaoos
		   		SET avsprobres=" . $dados['avsprobres'] . ", avstempo='" . $dados['avstempo'] . "', avsgeral='" . $dados['avsgeral'] . "', avsobs='" . $dados['avsobs'] . "'
		 		WHERE avoid='" . $avoid . "'";
        $db->executar($sql);
    } else {

        $sql = "INSERT INTO fabrica.avaliacaoos(
	            odsid, avsprobres, avstempo, avsgeral, avsobs, avsstatus,
	            avsdata)
	    		VALUES ('" . $dados['odsid'] . "', " . $dados['avsprobres'] . ", '" . $dados['avstempo'] . "', '" . $dados['avsgeral'] . "',
	    				'" . $dados['avsobs'] . "', 'A', NOW());";
        $db->executar($sql);
    }

    //salvando a observação
    if ($dados['osodesc']) {
        $sql = "INSERT
				INTO
				fabrica.ordemservicoobservacao
					(osodesc, osodtinclusao, odsid, usucpf)
				VALUES
					('" . $dados['osodesc'] . "', NOW(), '" . $dados['odsid'] . "', '" . $_SESSION['usucpf'] . "');";
        $db->executar($sql);
    }

    // gravando os anexos
    if ($_FILES['arquivo']['error'] == 0) {

        include_once APPRAIZ . "includes/classes/fileSimec.class.inc";

        $campos = array("taoid" => "'" . $dados['taoid'] . "'",
            "odsid" => "'" . $dados['odsid'] . "'",
            "aosdsc" => "'" . $dados['aosdsc_'] . "'",
            "aosdtinclusao" => "NOW()",
            "aosstatus" => "'A'");

        $file = new FilesSimec("anexoordemservico", $campos, "fabrica");
        $file->setUpload((($dados['aosdsc_']) ? $dados['aosdsc_'] : NULL), $key = "arquivo");
    }

    $db->commit();

    echo "<script>
			alert('Avaliação da OS efetuada com sucesso');
			window.location='fabrica.php?modulo=principal/cadOSExecucao&acao=A&odsid=" . $dados['odsid'] . "';
		  </script>";
}

function gravarAtestoOS($dados) {
    global $db;
    $atoid = $db->pegaUm("SELECT atoid FROM fabrica.atestoos WHERE odsid='" . $dados['odsid'] . "'");

    if ($atoid) {
        $sql = "UPDATE fabrica.atestoos
			   		SET atoatendimento=" . $dados['atoatendimento'] . ", atoresposta='" . $dados['atoresposta'] . "', atorequisito='" . $dados['atorequisito'] . "', atolayout='" . $dados['atolayout'] . "', atoobs='" . $dados['atoobs'] . "'
		 		WHERE atoid='" . $atoid . "'";
        $db->executar($sql);
    } else {

        $sql = "INSERT INTO fabrica.atestoos(
					odsid, atoatendimento, atoresposta, atorequisito, atolayout, usucpf, atoobs, atostatus, atodata
	            )
	    		VALUES ('" . $dados['odsid'] . "', " . $dados['atoatendimento'] . ", '" . $dados['atoresposta'] . "', '" . $dados['atorequisito'] . "','" . $dados['atolayout'] . "', '" . $_SESSION['usucpf'] . "',
	    				'" . $dados['atoobs'] . "', 'A', NOW());";
        $db->executar($sql);
    }
    $db->commit();

    echo "<script>
			alert('Atesto da OS efetuada com sucesso');
			window.location='fabrica.php?modulo=principal/cadOSExecucao&acao=A&odsid=" . $dados['odsid'] . "';
		  </script>";
}

function gravarVerificaoOS($dados) {

    global $db;

    $veoid = $db->pegaUm("SELECT veoid FROM fabrica.verificacaoos WHERE odsid='" . $dados['odsid'] . "'");

    if ($veoid) {
        $sql = "UPDATE fabrica.verificacaoos
			   		SET veoprodutos=" . $dados['veoprodutos'] . ", veoniveis='" . $dados['veoniveis'] . "', veoocorrencias='" . $dados['veoocorrencias'] . "', veoobs='" . $dados['veoobs'] . "'
		 		WHERE veoid='" . $veoid . "'";
        $db->executar($sql);
    } else {

        $sql = "INSERT INTO fabrica.verificacaoos(
					odsid, veoprodutos, veoniveis, veoocorrencias, usucpf, veoobs, veostatus, veodata
	            )
	    		VALUES ('" . $dados['odsid'] . "', " . $dados['veoprodutos'] . ", '" . $dados['veoniveis'] . "', '" . $dados['veoocorrencias'] . "', '" . $_SESSION['usucpf'] . "',
	    				'" . $dados['veoobs'] . "', 'A', NOW());";
        $db->executar($sql);
    }

    $sql_OS = "UPDATE fabrica.ordemservico
		   		SET ctrid=" . ($dados['ctrid']) . " WHERE odsid=" . $dados['odsid'] . "";


    $ordemServico = new OrdemServico();
    $os = $ordemServico->recuperePorId($dados['odsid']);
    $qtdPFDetalhadaOS = $os->getQtdePfDetalhada();

    $db->executar($sql_OS);
    $db->commit();

    echo "<script>
			alert('Verificação da OS efetuada com sucesso');
			window.location='fabrica.php?modulo=principal/cadOSExecucao&acao=A&odsid=" . $dados['odsid'] . "';
		  </script>";
}

function tramitarOSPFEmpresaItem1($odsid) {
    calcularGlosaOSEmpresaItem1($odsid);
    enviaEmailFluxoHistoricoOS($odsid);
    return true;
}

function tramitarOSPFEmpresaItem2($odsid) {
    calcularGlosaOSEmpresaItem2($odsid);
    enviaEmailFluxoHistoricoOS($odsid);
    return true;
}

function calcularGlosaOSEmpresaItem2($odsid) {
    $ordemServico = new OrdemServico();
    $os = $ordemServico->recuperePorId($odsid);
    $histPrevisaoTermino = new HistoricoPrevisaoTermino();
    $arrDadosSituacaoStatusOS = $histPrevisaoTermino->retornarDataPrevisaoTerminoInicialEmpresaItem2($odsid);
    $qtdPFDetalhadaOS = $os->getQtdePfDetalhada();

    if (strtoupper($arrDadosSituacaoStatusOS['status_os']) == 'OS EM ATRASO') {

        $valorIDP = ( ( $arrDadosSituacaoStatusOS['dias_em_atraso'] * 100) / $arrDadosSituacaoStatusOS['dias_previstos'] );

        $percentualGlosa = 0;

        if ($valorIDP > 81) {
            $percentualGlosa = 20;
        } elseif ($valorIDP >= 51 && $valorIDP <= 80) {
            $percentualGlosa = 15;
        } elseif ($valorIDP >= 31 && $valorIDP <= 50) {
            $percentualGlosa = 10;
        } elseif ($valorIDP >= 11 && $valorIDP <= 30) {
            $percentualGlosa = 5;
        }

        $qtdPFGlosa = ($qtdPFDetalhadaOS * ( $percentualGlosa / 100 ));
        $glosa = new Glosa();

        if ($os->possuiGlosa()) {
            $glosa->setId($os->getIdGlosa());
        }

        $glosa->setDataInclusao(date('Y-m-d'));
        $glosa->setJustificativa('Demanda entregue fora do prazo');
        $glosa->setUsuarioResponsavel($_SESSION['usucpf']);
        $glosa->setValorEmPfComMascara($qtdPFGlosa);
        $glosa->salvar($os->getId());
    }
    $ordemServico->commit();
    return true;
}

function calcularGlosaOSEmpresaItem1($odsid) {

    global $db;

    $ordemServico = new OrdemServico();
    $os = $ordemServico->recuperePorId($odsid);
    $histPrevisaoTermino = new HistoricoPrevisaoTermino();
    $arrDadosSituacaoStatusOS = $histPrevisaoTermino->retornarDataPrevisaoTerminoInicialEmpresaItem1($odsid);
    $qtdPFDetalhadaOS = $os->getQtdePfDetalhada();

    if (strtoupper($arrDadosSituacaoStatusOS['status_os']) == 'OS EM ATRASO') {

        $valorIDP = ( ( $arrDadosSituacaoStatusOS['dias_em_atraso'] * 100) / $arrDadosSituacaoStatusOS['dias_previstos'] );
        $porcentagemDisciplina = $ordemServico->recuperarPorcentagemDisciplinasContratadas($odsid);

        $percentualGlosa = 0;

        if ($valorIDP > 81) {
            $percentualGlosa = 20;
        } elseif ($valorIDP >= 51 && $valorIDP <= 80) {
            $percentualGlosa = 15;
        } elseif ($valorIDP >= 31 && $valorIDP <= 50) {
            $percentualGlosa = 10;
        } elseif ($valorIDP >= 11 && $valorIDP <= 30) {
            $percentualGlosa = 5;
        }

        $qtdPFGlosa = ( ( $qtdPFDetalhadaOS * ( $percentualGlosa / 100 )) * ( $porcentagemDisciplina / 100 ) );
        $glosa = new Glosa();

        if ($os->possuiGlosa()) {
            $glosa->setId($os->getIdGlosa());
        }

        $glosa->setDataInclusao(date('Y-m-d'));
        $glosa->setJustificativa('Demanda entregue fora do prazo');
        $glosa->setUsuarioResponsavel($_SESSION['usucpf']);
        $glosa->setValorEmPfComMascara($qtdPFGlosa);
        $glosa->salvar($os->getId());

        $sql = "SELECT hd.docid, cmdid, max(hd.hstid) as hstid, max(hd.htddata) as httdata 
                    FROM  workflow.historicodocumento hd
                    INNER JOIN fabrica.ordemservico os
                        ON hd.docid = os.docid
                    LEFT JOIN workflow.comentariodocumento cdoc
                        ON hd.hstid = cdoc.hstid 
                        AND hd.docid = cdoc.docid
                    WHERE os.odsid = {$odsid}
                    GROUP BY hd.docid, cmdid";

        $dadoHistorico = $db->pegaLinha($sql);

        $sqlComentario = "insert into workflow.comentariodocumento
                    ( docid, hstid, cmddsc, cmddata, cmdstatus )

        values ( " . $dadoHistorico['docid'] . ", " . $dadoHistorico['hstid'] . ", 'Glosa incluída automaticamente ao tramitar', now(), 'A' )";

        $db->executar($sqlComentario);
        $db->commit();
    }

    $ordemServico->commit();
    return true;
}

function executarOS($dados) {
    global $db;

    $sql = "UPDATE fabrica.ordemservico
		   		SET odsqtdpfdetalhada=" . ($dados['odsqtdpfdetalhada'] ? "'" . str_replace(',', '.', str_replace('.', '', $_POST['odsqtdpfdetalhada'])) . "'" : 'null') . ",
	 				odssubtotalpfdetalhada=" . ($dados['odssubtotalpfdetalhada'] ? "'" . str_replace(',', '.', str_replace('.', '', $_POST['odssubtotalpfdetalhada'])) . "'" : 'null') . "
		   	WHERE odsid=" . $dados['odsid'] . "";

    $db->executar($sql);
    $db->commit();

    echo "<script>
			alert('Operação efetuada com sucesso');
			window.location='fabrica.php?modulo=principal/cadOSExecucao&acao=A&odsid=" . $dados['odsid'] . "';
		  </script>";
    exit;
}

function verificarFinalizacao($odsid) {
    global $db;

    $sql = "SELECT distinct
					odsid
					/*,
					case when tosid = 1 then
						doc.esdid
					    else
						docpf.esdid
					end as esdid,
					tosid
					*/
			FROM fabrica.ordemservico os
			LEFT JOIN workflow.documento doc ON doc.docid = os.docid
			LEFT JOIN workflow.documento docpf ON docpf.docid = os.docidpf
			WHERE scsid=" . $_SESSION['fabrica_var']['scsid'] . "
			AND (doc.esdid not in(" . WF_ESTADO_OS_FINALIZADA . "," . WF_ESTADO_OS_CANCELADA_SEM_CUSTO . "," . WF_ESTADO_OS_CANCELADA_COM_CUSTO . ") OR doc.esdid is null)
			AND (docpf.esdid not in(" . WF_ESTADO_CPF_FINALIZADA . "," . WF_ESTADO_CPF_CANCELADA . ") OR docpf.esdid is null)
			limit 1";
    /*
      $sql = "SELECT esdid FROM fabrica.ordemservico os
      INNER JOIN workflow.documento doc ON doc.docid = os.docid
      WHERE scsid='".$_SESSION['fabrica_var']['scsid']."'";
     */
    $oss = $db->pegaUm($sql);

    if (!$oss) {
        $docid = $db->pegaUm("SELECT docid FROM fabrica.solicitacaoservico WHERE scsid='" . $_SESSION['fabrica_var']['scsid'] . "'");
        $ok = wf_alterarEstado($docid, WF_ACAO_SOL_EXECUCAOFINAL, $cmddsc = '', $dados = array('scsid' => $_SESSION['fabrica_var']['scsid']));

        if ($ok) {
            enviaEmailFluxoHistorico($_SESSION['fabrica_var']['scsid']);
        }
    }

    if ($odsid) {
        $db->executar("UPDATE fabrica.contrato
				   SET ctrqtdpfutilizado=(ctrqtdpfutilizado-(SELECT odsqtdpfestimada FROM fabrica.ordemservico WHERE odsid='" . $odsid . "'))
				   WHERE ctrid IN(select
								  	ctrid
							      from
								    fabrica.solicitacaoservico scs
								   inner join
								    fabrica.analisesolicitacao ans ON ans.scsid = scs.scsid
								   where
								    scs.scsid = " . $_SESSION['fabrica_var']['scsid'] . "
								   and
								    scsstatus = 'A'
								   and
								   	ctsstatus = 'A'
								   )");
    }
    $db->commit();

    return true;
}

function regraFinalizaOS($odsid) {
    global $db;

    //pega dados da OS
    $os = $db->pegaLinha("SELECT scsid, tosid FROM fabrica.ordemservico WHERE odsid=" . $odsid);


    $scsid = $os['scsid'];

    if ($os['tosid'] == '2' || $os['tosid'] == '3') {
        $tpeid = 2;
    } else {
        $tpeid = 1;
    }


    //Contagem de P.F.
    $porcentoPf = 0;
    //$valorTotal = 0;
    //pega odsqtdpfestimada das OSs
    if ($tpeid == 1) { // empresa 1
        $sql = "SELECT COALESCE(sum(os.odsqtdpfestimada),0) as qtdpf
					FROM fabrica.ordemservico os
					where os.odsid = {$odsid}
					and os.tosid in (1)";
    } else {
        $sql = "SELECT COALESCE(sum(os.odsqtdpfestimada),0) as qtdpf
					FROM fabrica.ordemservico os
					where os.odsid = {$odsid}
					and os.tosid in (2,3)";
    }
    $qtdpf = $db->pegaUm($sql);

    $porcentoPf = 100;
    $valorPf = $qtdpf / 100;
    $valorPfFinal = $porcentoPf * $valorPf;

    //pega id do contrato
    $ctrid = $db->pegaUm("select ctrid from fabrica.analisesolicitacao where scsid =" . $scsid);

    //Atualiza P.F. utilizado do contrato
    if ($ctrid && $valorPfFinal) {
        $qtd = $valorPfFinal;
        
        // Chama função responsavel por realizar o calculo de PF Utilizado
        $db->pegaUm("select fabrica.fn_calculo_pf_utilizado()");
        
        $db->executar("UPDATE fabrica.contrato SET ctrqtdpfutilizado = ctrqtdpfutilizado + {$qtd} WHERE ctrid = {$ctrid}");
        $db->commit();
    }

    //envia email avisando a mudança de status da OS
    enviaEmailFluxoHistoricoOS($odsid);

    return true;
}

function verificarAtribuicaoProfissionais($odsid) {
    global $db;
    return (($db->pegaUm("SELECT usucpf FROM fabrica.profissionalos WHERE odsid='" . $odsid . "'")) ? true : false);
}

function verificarAtesto($odsid) {
    global $db;
    $atesto = $db->pegaUm("SELECT atoid FROM fabrica.atestoos WHERE odsid='" . $odsid . "'");
    //$odsqtdpfestimada = $db->pegaUm("SELECT odsqtdpfestimada FROM fabrica.ordemservico WHERE odsid='".$odsid."'");
    return (($atesto) ? true : 'Atesto Técnico deve ser preenchido!');
}

function verificarAvaliacao($odsid) {
    global $db;
    
	//se for UST, retorna true
	//recupera sigla do item da metrica
	$sigla = recuperaMetrica( $_SESSION['fabrica_var']['ansid'] );
	if($sigla == 'UST'){
		return true;
	}
    
    $avaliacao = $db->pegaUm("SELECT veoid FROM fabrica.verificacaoos WHERE odsid='" . $odsid . "'");
    //$odsqtdpfestimada = $db->pegaUm("SELECT odsqtdpfestimada FROM fabrica.ordemservico WHERE odsid='".$odsid."'");
    if (!$avaliacao) {
        return 'Avaliação deve ser preenchida!';
    } else {
        return validaDivergencia($odsid, 'OS', 'Detalhada');
    }
}

function validarExecucaoOS($odsid) {
    global $db;

    $sql = "SELECT esdid FROM workflow.documento d
			INNER JOIN demandas.demanda dd ON dd.docid = d.docid
			WHERE odsid='" . $odsid . "'";

    $estadosdoc = $db->carregar($sql);

    $validacao = true;

    if ($estadosdoc[0]) {
        foreach ($estadosdoc as $ed) {
            // verifcnado se o estado é finalizado
            if ($ed['esdid'] != 111) {
                $validacao = false;
            }
        }
    }

    if ($validacao && $estadosdoc[0])
        return true;
    else
        return "Demandas não finalizadas";
}

function validarPFEstimada($scsid) {
    return validaDivergencia($scsid, 'SS', 'Estimada');
}

function validaDivergencia($id, $tipo, $contagem) {
    global $db;
    // DETALHADA EMPRESA DO ITEM 1
    $sql = "Select os.scsid, os.odssubtotalpf, os.odssubtotalpfdetalhada, ans.mensuravel, os.odsqtdpfdetalhada
                                        FROM fabrica.ordemservico os 
                                        LEFT JOIN fabrica.analisesolicitacao ans ON ans.scsid = os.scsid
                                        WHERE tosid = 1";
    if ($tipo == 'SS') {
        $sql = $sql . " AND os.scsid = '" . $id . "' ";
    } else {
        $sql = $sql . " AND os.odsid = '" . $id . "' ";
    }
    
    $sql_tosid_1 = $db->carregar($sql);

    $scsid = $sql_tosid_1[0]['scsid'];
    $mensuravel = $sql_tosid_1[0]['mensuravel'];
    //FIM DETALHADA EMPRSA DO ITEM 1
    
    if ($contagem == "Estimada") {
        $sql_tosid_2 = $db->carregar("Select odsid, odssubtotalpf, odsqtdpfdetalhada FROM fabrica.ordemservico os WHERE scsid='" . $scsid . "' AND tosid = 2");
        $valorEmpresaItem2 = $sql_tosid_2[0]['odssubtotalpf'];
        $osEmpresa2 = $sql_tosid_2[0]['odsid'];
        $valorEmpresaItem1 = $sql_tosid_1[0]['odssubtotalpf'];
    } else {
        $sql_tosid_3 = $db->carregar("Select odsid, odssubtotalpf, odsqtdpfdetalhada FROM fabrica.ordemservico os WHERE scsid='" . $scsid . "' AND tosid = 3");
        $sql_tosid_4 = $db->carregar("Select odsid, odssubtotalpf, odsqtdpfdetalhada FROM fabrica.ordemservico os WHERE scsid='" . $scsid . "' AND tosid = 1");
        $valorEmpresaItem2 = $sql_tosid_3[0]['odssubtotalpf'];
        $valorEmpresaItem1 = $sql_tosid_1[0]['odssubtotalpfdetalhada'];
        $valorQtdEmpresaItem3 = $sql_tosid_3[0]['odsqtdpfdetalhada'];
        $valorQtdEmpresaItem1 = $sql_tosid_4[0]['odsqtdpfdetalhada'];
    }

    $divergente = true;
	
	if(!$valorEmpresaItem2) $valorEmpresaItem2 = 0;
	
	$divPF = true;
	$divQT = true;
	
    if ( $valorEmpresaItem2 >= ($valorEmpresaItem1 * 0.95) && $valorEmpresaItem2 <= ($valorEmpresaItem1 * 1.05) ) {
        $divPF = false;
    }

	if ( $valorQtdEmpresaItem3 >= ($valorQtdEmpresaItem1 * 0.95) && $valorQtdEmpresaItem3 <= ($valorQtdEmpresaItem1 * 1.05) ) {
		$divQT = false;
	}
    
	if ($divPF || $divQT) {
		$divergente = true; 
	} else {
		$divergente = false;
	}
	
    // Quando a OS não for mensuravel e ja tiver contagem detalhada
    if ($mensuravel != t && $valorEmpresaItem1 > 0 && $contagem == "Detalhada") {
        $divergente = false;
    }
    // NO CASO DE CONTAGEM ESTIMADA NÃO E NECESSÁRIO UMA VALIDAÇÃO DA EMPRESA DO ITEM 2
    if (($contagem == "Estimada" && !$osEmpresa2) || ($contagem == "Estimada" && $mensuravel != t)) {
        $divergente = false;
    }
	
    //ver($divergente ? "t" : "f");
    
    return !$divergente;
}

function carregarMenuOS() {
    // monta menu padrão contendo informações sobre as entidades
    $menu = array(0 => array("id" => 1, "descricao" => "OS", "link" => "/fabrica/fabrica.php?modulo=principal/cadOSExecucao&acao=A&odsid=" . $_REQUEST['odsid']),
        1 => array("id" => 2, "descricao" => "Observações", "link" => "/fabrica/fabrica.php?modulo=principal/cadOSObservacao&acao=A&odsid=" . $_REQUEST['odsid'] . "&fecharTela=nao"),
        2 => array("id" => 3, "descricao" => "Termos", "link" => "/fabrica/fabrica.php?modulo=principal/termo&acao=A&odsid=" . $_REQUEST['odsid'] . "&odsidAba=" . $_REQUEST['odsid'] . "&fecharTela=nao")
    );
    return $menu;
}

function regraFinalizarOSPF($odsid) {

    global $db;

    $sql = "SELECT
                    count(*)

                        FROM fabrica.ordemservico as os

                            inner join  workflow.documento as wdc
                                on wdc.docid = os.docidpf
                            inner join  workflow.estadodocumento as wed
                                on wed.esdid = wdc.esdid

                            WHERE   os.odsid = {$odsid} and
                                    os.tosid = " . TIPO_OS_CONTAGEM_ESTIMADA . " and
                                    wed.esdid = " . WF_ESTADO_CPF_FINALIZADA;

    $SOPFEstimadaFinalizada = $db->pegaUm($sql);

    if ($SOPFEstimadaFinalizada > 0) {
        return true;
    } else {
        return false;
    }
}

function regraPagamentoSS($scsid) {
    global $db;

    // Regra 1 de Tramitação
    $regra1 = regraAprovarSS($scsid);

    // Regra 2 de Tramitação
    $sql = "select
                os.odsid

                from fabrica.ordemservico as os
                    inner join fabrica.tipoordemservico as tos
                        on tos.tosid=os.tosid
                where os.scsid = {$scsid} and os.tosid = " . TIPO_OS_GERAL;

    $odsidGenerica = $db->pegaUm($sql);


    $regra2 = validarPFDetalhada($odsidGenerica);

    // Regra 3 de Tramitação
    $sql = "select
                count(*)

                from fabrica.ordemservico as os
                    inner join workflow.documento as wdc
                        on wdc.docid = os.docid
                    inner join workflow.estadodocumento as wed
                        on wed.esdid = wdc.esdid

                where   os.scsid = {$scsid} and
                        os.tosid = " . TIPO_OS_GERAL . " and
                        wed.esdid = " . WF_ESTADO_OS_FINALIZADA;

    $OSGerericaFinalizada = $db->pegaUm($sql);

    $regra3 = false;
    if ($OSGerericaFinalizada > 0) {
        $regra3 = true;
    }


    if ($regra1 == true && $regra2 == true && $regra3 == true) {
        return true;
    } else {
        return false;
    }
}

function regraFinalizarSS($scsid) {
    global $db;

    // Regra 1 de Tramitação
    $regra1 = regraAprovarSS($scsid);

    // Regra 2 de Tramitação
    $sql = "select
                os.odsid

                from fabrica.ordemservico as os
                    inner join fabrica.tipoordemservico as tos
                        on tos.tosid=os.tosid
                where os.scsid = {$scsid} and os.tosid = " . TIPO_OS_GERAL;

    $odsidGenerica = $db->pegaUm($sql);


    $regra2 = validarPFDetalhada($odsidGenerica);

    // Regra 3 de Tramitação
    $sql = "select
                count(*)

                from fabrica.ordemservico as os
                    inner join workflow.documento as wdc
                        on wdc.docid = os.docid
                    inner join workflow.estadodocumento as wed
                        on wed.esdid = wdc.esdid

                where   os.scsid = {$scsid} and
                        os.tosid = " . TIPO_OS_GERAL . " and
                        wed.esdid = " . WF_ESTADO_OS_FINALIZADA;

    $OSGerericaFinalizada = $db->pegaUm($sql);

    $regra3 = false;
    if ($OSGerericaFinalizada > 0) {
        $regra3 = true;
    }


    if ($regra1 == true && $regra2 == true && $regra3 == true) {
        return true;
    } else {
        return false;
    }
}

function criaOrdemDeServicoDeContagemEstimada($odsid) {
    global $db;

    $sqlOS = " SELECT anss.ansid, anss.scsid, audi.audid
                    FROM  fabrica.ordemservico os 
                    LEFT JOIN fabrica.analisesolicitacao anss
                        ON os.scsid = anss.scsid
                    LEFT JOIN fabrica.auditoria audi
                        ON anss.ansid = audi.ansid
                    WHERE os.odsid	= {$odsid}";

    $dadosOS = $db->pegaLinha($sqlOS);
    $erros = false;

    if (empty($dadosOS['audid'])) {
        $analiseSolicitacao = new AnaliseSolicitacao();
        $auditoria = new Auditoria();
        $auditoriaRepo = new AuditoriaRepositorio();
        $nomeResponsavelFab = $_SESSION['usunome'];

        $analiseSolicitacao->setId($dadosOS['ansid']);
        $auditoria->setAnaliseSolicitacao($analiseSolicitacao);
        $auditoria->setNomeResponsavelFabrica($nomeResponsavelFab);

        if (!$auditoriaRepo->salvar($auditoria)) {
            echo '<script type="text/javascript"> alert(\'Não foi possível criar o documento de auditoria em workflow.\');window.close();</script>';
            $erros = false;
        }
    }
}

function criaOrdemDeServicoDeContagemDetalhada($odsid) 
{
    global $db;
    
    $sql = "select 
					ans.ansid
				FROM fabrica.ordemservico os
				inner join fabrica.analisesolicitacao ans on os.scsid = ans.scsid
				where os.odsid=".$odsid;
    $ansid = $db->pegaUm($sql);
    
    $sigla = recuperaMetrica( $ansid );
    
    if($sigla == 'PF'){
    
	    $erros = false;
	    $erros = verificaAnexoDetalhada($odsid);
	
	    if ($erros) {
	        $sqlOs = "select 
						coalesce( SUM(os.odsqtdpfdetalhada), 0) as detalhada
						, ans.scsid
						, case when ans.ansgarantia = true then 1 else 0 end as garantia
	                    , case when ans.mensuravel  = true then 1 else 0 end as mensuravel
					FROM fabrica.ordemservico os
					inner join fabrica.analisesolicitacao ans 
						on os.scsid = ans.scsid
					where os.odsid = " . $odsid . " 
					group by ans.scsid
						, ansgarantia
	                    , mensuravel";
	        
	        $resultadoOs = $db->pegaLinha($sqlOs);
	
	        if ($resultadoOs['garantia'] == 0  && $resultadoOs['mensuravel'] == 1) {
	            $dias = 0;
	
	            if ($resultadoOs['detalhada'] <= 150) {
	                $dias += 1;//2
	            } elseif ($resultadoOs['detalhada'] <= 600) {
	                $dias += 4;//5
	            } elseif ($resultadoOs['detalhada'] <= 1000) {
	                $dias += 9;//10
	            } elseif ($resultadoOs['detalhada'] > 1000) {
	                $dias += 14;//15
	            }
	            
	            // Adiciona 1 dia casa não atenda as condições acima
	            // $dias = ( empty($dias) ? 1 : $dias );
	            
	                        
	            // Verifica se existe OS Detalhada
	            $sqlOdsidDetalhada = "select odsid
									from
										fabrica.ordemservico
									where
										odsidpai = " . $odsid . "
										and tosid = 3
									limit 1";
	
	            
	            $retornoOdsidDetalhada = $db->pegaLinha($sqlOdsidDetalhada);
	            
	            if(!empty($retornoOdsidDetalhada['odsid'])){
	                return true;
	            }
	            
	            //$dtInicio 	= date('Y-m-d H:i:s', strtotime("+1 days"));
	            //$dtFim 		= date('Y-m-d H:i:s', strtotime("+" . $dias . " days", strtotime($dtInicio)));
	            
	            $dtInicio 	= somar_dias_uteis( date('Y-m-d'), 1);
	            $dtFim 		= somar_dias_uteis( $dtInicio, $dias );
	            
	            //pega vigencia do contrato ativo de contagem de PF
	            $sqlC = "select v.vgcid, c.ctrid  from fabrica.vigenciacontrato v
						inner join fabrica.contrato c on c.ctrid = v.ctrid
						where c.ctrcontagem='t' and c.ctrstatus='A' and v.vgcstatus='A'
						order by vgcid desc limit 1";
				$contrato = $db->pegaLinha($sqlC);
				
	            $dados['odsdtprevinicio']  = formata_data( $dtInicio );
	            $dados['odsdtprevtermino'] = formata_data( $dtFim );
	            $dados['ctrid_disable'] = $contrato['ctrid'];
	            $dados['vgcid'] = $contrato['vgcid'];
	            $dados['mtiid'] = 2; // PF - Contagem de PF
	            $dados['odsdetalhamento'] = 'Realizar contagem detalhada';
	            $dados['odsidpai'] = $odsid;
	            $dados['odsid'] = (!empty($retornoOdsidDetalhada['odsid']) ? $retornoOdsidDetalhada['odsid'] : '' );
	            $dados['tosid'] = TIPO_OS_CONTAGEM_DETALHADA;
	
	            $retorno = salvarOSContagemPF($dados);
	
	            if ( !empty($retorno['odsid']) ) {
	
	                // ID da OS Estimada Criada
	                // $odsid = $retorno['odsid'];
	                // Recupera ID do documento da solicitação
	                $sqlDocId = "select
									docidpf, odsid
								from
									fabrica.ordemservico
								where
									tosid = 3
								and  odsidpai = " . $odsid;
	
	                $retornoSql = $db->pegaLinha($sqlDocId);
	
	                // Recupera dados da tramitação
	                $esdidorigem = wf_pegarEstadoAtual($retornoSql['docidpf']);
	
	                // Ignora condição caso a OS Detalhada já esteja no estado de WF_ESTADO_CPF_AGUARDANDO_CONTAGEM
	                if ($esdidorigem['esdid'] != WF_ESTADO_CPF_AGUARDANDO_CONTAGEM && $esdidorigem['esdid'] != WF_ESTADO_CPF_FINALIZADA) {
	                    $arrAcao = wf_pegarAcao($esdidorigem['esdid'], WF_ESTADO_CPF_AGUARDANDO_CONTAGEM);
	
	                    // Dados da OS a ser tramitada 
	                    $dadosVerificacao = array('odsid' => $retornoSql['odsid']);
	
	                    // Altera estado Pendente para Aguardando Pagamento da OS Detalhada
	                    wf_alterarEstado($retornoSql['docidpf'], $arrAcao['aedid'], '', $dadosVerificacao);
	                }
	            } else {
	                $erros = false;
	            }
	        }
	    }
    }
    else{
    	 return true;
    }
    
    return $erros;
}

function criaOrdemDeServicoEstimada( $scsid ) 
{
    global $db;
    $erros = true;
    
	/*
    $sqlGarantia = "select 
					SUM(os.odsqtdpfestimada) as estimada
					, ans.scsid
					, case when ans.mensuravel  = true then 1 else 0 end as mensuravel
					, case when ans.ansgarantia = true then 1 else 0 end as garantia
					FROM fabrica.ordemservico os
					left join fabrica.analisesolicitacao ans 
					on os.scsid = ans.scsid
				where ans.scsid = " . $scsid . "
				group by ans.scsid
				, mensuravel
				, ansgarantia";

    $tipoGarantia = $db->pegaLinha($sqlGarantia);

    if ($tipoGarantia['garantia'] == 0 && $tipoGarantia['estimada'] >= 50 && $tipoGarantia['mensuravel'] == 1) {
        $dias = 0;

        if ($tipoGarantia['estimada'] <= 150) {

            $dias += 1;//2
        } elseif ($tipoGarantia['estimada'] <= 600) {

            $dias += 4;//5
        } elseif ($tipoGarantia['estimada'] <= 1000) {

            $dias += 9;//10
        } elseif ($tipoGarantia['estimada'] > 1000) {

            $dias += 14;//15
        }
        
        // Adiciona 1 dia casa não atenda as condições acima
        // $dias = ( empty($dias) ? 1 : $dias );

        // Seleciona codigo da Ordem de serviço
        $sqlOdsid = "select odsid 
				from 
					fabrica.ordemservico 
				where 
					scsid =  " . $scsid . "
					and tosid = 1
				limit 1";

        $retornoOdsid = $db->pegaLinha($sqlOdsid);

        // Caso exista OS estimada para OS
        $sqlOdsidEstimada = "select odsid, odsidpai 
							from 
								fabrica.ordemservico 
							where 
								scsid =  " . $scsid . "
								and odsidpai = " . $retornoOdsid['odsid'] . "
								and tosid = 2
							limit 1";
        
        //$tmp = somar_dias_uteis();
        
        $retornoOdsidEstimada = $db->pegaLinha($sqlOdsidEstimada);

        //$dtInicio 	= date('Y-m-d H:i:s', strtotime("+1 days"));
        //$dtFim 		= date('Y-m-d H:i:s', strtotime("+" . $dias . " days", strtotime($dtInicio))); 

        $dtInicio 	= somar_dias_uteis( date('Y-m-d'), 1);
        $dtFim 		= somar_dias_uteis( $dtInicio, $dias );
        

        $dados['odsdtprevinicio'] 	= formata_data($dtInicio);
        $dados['odsdtprevtermino'] 	= formata_data($dtFim);
        $dados['ctrid_disable'] 	= 3;
        $dados['odsdetalhamento'] 	= 'Realizar contagem estimada';
        $dados['odsidpai'] 			= (!empty($retornoOdsidEstimada['odsidpai']) ? $retornoOdsidEstimada['odsidpai'] : $retornoOdsid['odsid'] );
        $dados['odsid'] 			= (!empty($retornoOdsidEstimada['odsid']) ? $retornoOdsidEstimada['odsid'] : '' );
        $dados['tosid'] 			= TIPO_OS_CONTAGEM_ESTIMADA;

        $retorno = salvarOSContagemPF($dados);

        if (!empty($retorno['odsid'])) {
            // Recupera ID do documento da solicitação
            $sqlDocId = "select
							docidpf, odsid
						from
							fabrica.ordemservico 
						where
							tosid = 2
							and  odsidpai = " . $retornoOdsid['odsid'];

            $retornoSql = $db->pegaLinha($sqlDocId);

            // Recupera dados da tramitação
            $esdidorigem = wf_pegarEstadoAtual($retornoSql['docidpf']);

            // Ignora condição caso a OS Estimada já esteja no estado de WF_ESTADO_CPF_AGUARDANDO_CONTAGEM  
            if ($esdidorigem['esdid'] != WF_ESTADO_CPF_AGUARDANDO_CONTAGEM) {

                $arrAcao = wf_pegarAcao($esdidorigem['esdid'], WF_ESTADO_CPF_AGUARDANDO_CONTAGEM);

                $dadosVerificacao = array('odsid' => $retornoSql['odsid']);

                // Altera estado Pendente para Aguardando Pagamento da OS estimada 
                wf_alterarEstado($retornoSql['docidpf'], $arrAcao['aedid'], '', $dadosVerificacao);
            }
        } else {
            $erros = false;
        }
    }
	*/
    
    // Fluxo SS
    enviaEmailFluxoHistorico($scsid);

    return $erros;
}

/**
 * Cria o documento de auditoria
 * ao tramitar a OS de 'Em execucção' para 'Em avaliação'
 * 
 * @global cls_banco $db
 * @param int $odsid - Código da ordem de serviço tramitada
 * @return boolean 
 */
function criaDocumentoGCaposTramitarParaAvaliacao($odsid) {
    global $db;

    $sqlOS = " SELECT anss.ansid, anss.scsid, audi.audid
                    FROM  fabrica.ordemservico os 
                    LEFT JOIN fabrica.analisesolicitacao anss
                        ON os.scsid = anss.scsid
                    LEFT JOIN fabrica.auditoria audi
                        ON anss.ansid = audi.ansid
                    WHERE os.odsid	= {$odsid}";

    $dadosOS = $db->pegaLinha($sqlOS);
    $erros = false;

    if (empty($dadosOS['audid'])) {
        $analiseSolicitacao = new AnaliseSolicitacao();
        $auditoria = new Auditoria();
        $auditoriaRepo = new AuditoriaRepositorio();
        $nomeResponsavelFab = $_SESSION['usunome'];

        $analiseSolicitacao->setId($dadosOS['ansid']);
        $auditoria->setAnaliseSolicitacao($analiseSolicitacao);
        $auditoria->setNomeResponsavelFabrica($nomeResponsavelFab);

        if (!$auditoriaRepo->salvar($auditoria)) {
            echo '<script type="text/javascript"> alert(\'Não foi possível criar o documento de auditoria em workflow.\');window.close();</script>';
            $erros = false;
        }
    }

    enviaEmailFluxoHistoricoOS($odsid);
    return !$erros;
}

function verificaAnexoDetalhada($odsid) {
    global $db;
    
    //se for UST, retorna true
	//recupera sigla do item da metrica
	if($_SESSION['fabrica_var']['ansid']){
		$sigla = recuperaMetrica( $_SESSION['fabrica_var']['ansid'] );
		if($sigla == 'UST'){
			return true;
		}
	}    

    $sqlSS = "SELECT ans.ansgarantia
                    FROM fabrica.ordemservico os
                    INNER JOIN fabrica.analisesolicitacao ans
                        ON os.scsid = ans.scsid
                    WHERE os.odsid = {$odsid}";

    $dadosOS = $db->pegaLinha($sqlSS);

    $sqlAnexo = " SELECT tp.taodsc, os.odsqtdpfdetalhada, os.odssubtotalpfdetalhada
					FROM fabrica.anexoordemservico an
					LEFT JOIN fabrica.tipoanexoordem tp ON an.taoid=tp.taoid
					LEFT JOIN public.arquivo ar ON ar.arqid=an.arqid
					LEFT JOIN fabrica.ordemservico os ON os.odsid=an.odsid                    
				WHERE os.odsid= $odsid AND aosstatus='A' AND tp.taoid = 29 ";

    $dadosAnexo = $db->carregar($sqlAnexo);
    $anexoContagemDetahada = $dadosAnexo[0]['taodsc'];
    $pfDetalhada = $dadosAnexo[0]['odsqtdpfdetalhada'];
    $subTotalPfDetalhada = $dadosAnexo[0]['odssubtotalpfdetalhada'];

    //verifica se a os vinculada é de garantia
    if ($dadosOS['ansgarantia'] == 't') {
        return true;
    }

    if ($anexoContagemDetahada != '') {
        if ($pfDetalhada > 0 and $subTotalPfDetalhada > 0) {
            return true;
        } else {
            return false;
        }
    } else {
        echo '<script type="text/javascript"> alert(\'Favor anexar Relatório de contagem de PF Detalhada.\');</script>';
        return false;
    }
}

function validarPFDetalhadaSS($scsid) {
    return validaDivergencia($scsid, 'SS', 'Detalhada');
}

function validaAguardandoPagamentoSS($scsid) {
    global $db;

    $sql = "SELECT	count(*) as total
			FROM	fabrica.vw_painel_financeiro_empresas
			WHERE id_ss = $scsid
			AND id_status_documento not in (257,277,301,302,303) ";
    $dados = $db->carregar($sql);
    if ($dados[0]['total'] > 0) {
        return false;
    } else {
        return true;
    }
}

function validaAguardandoPagamentoOS($odsid) {
    global $db;
    $OS2_Finaliza_Estimada = true;

    $sql = " SELECT	id_ss, tosid_os
			FROM	fabrica.vw_painel_financeiro_empresas
			WHERE id_os = $odsid ";
    $os = $db->carregar($sql);

    $scsid = $os[0]['id_ss'];

    if ($os[0]['tosid_os'] == 3) {

        $sql_os = "SELECT	count(*) as total
				FROM	fabrica.vw_painel_financeiro_empresas
				WHERE id_ss = $scsid
				AND tosid_os = 2
				AND id_status_documento not in (277,303) ";
        $dados = $db->carregar($sql_os);

        if ($dados[0]['total'] > 0) {
            $OS2_Finaliza_Estimada = false;
        }
    }
    return $OS2_Finaliza_Estimada;
}

/**
 * Finaliza a solicitação de servico caso todas as OS's vinculadas 
 * foram emitidas junto há algum memorando
 * 
 * @param int $odsid
 * @return bool
 */
function finalizaSolicitacaoServico($odsid) {
    $ordemServico = new OrdemServico();
    $ordemServico = $ordemServico->recuperePorId($odsid);
    $solicitacaoServico = new SolicitacaoServico();
    $idSS = $ordemServico->getIdSolicitacaoServico();

    if ($solicitacaoServico->possuiTodosAsOSEmitidasEmMemorando($idSS)) {
        $solicitacaoServico->carregarPorId($idSS);
        $docid = $solicitacaoServico->docid;
        $dadosAcao = wf_pegarAcao(WF_ESTADO_EXECUCAO, WF_ESTADO_FINALIZADA);
        $alterado = wf_alterarEstado($docid, $dadosAcao['aedid'], '', array('scsid' => $idSS));

        if (!$alterado)
            return false;
    }

    return regraFinalizaOS($odsid);
}

/**
 * Verifica se deve ser exibida o trâmite para Em Divergência
 * para as OS da empresa do item 2 ( Detalhada )
 * 
 * @param int $odsid
 * @return boolean 
 */
function exibirDivergencia($odsid) {
    $ordemServico = new OrdemServico();
    $ordemServico = $ordemServico->recuperePorId($odsid);
    

    if ($ordemServico->getIdTipoOrdemServico() != TIPO_OS_CONTAGEM_DETALHADA)
        return false;

    return !validaDivergencia($ordemServico->getIdPai(), 'OS', 'Detalhada');
}

/**
 * Verifica se possui alguma divergência entre os PF da empresa do item 1 
 * e as os detalhada
 * @param int $odsid
 * @return bool
 */
function verificarDivergencia($odsid) {
    $ordemServico = new OrdemServico();
    $ordemServico = $ordemServico->recuperePorId($odsid);
    return !validaDivergencia($ordemServico->getIdPai(), 'OS', 'Detalhada');
}

/**
 * Adiciona o tempo de divergência ao fim de uma OS
 * @global cls_banco $db
 * @param int $odsid
 * @return boolean 
 */
function adicionaTempoDivergencia($odsid) {
    global $db;

    $sqlDocId = "select
                        case when tosid = " . TIPO_OS_GERAL . "
                        then docid
                        else docidpf
                        end AS doc_id
                        , tosid
                from
                        fabrica.ordemservico
                where
                odsid = " . $odsid;

    $retornoSql = $db->pegaLinha($sqlDocId);

    if (!empty($retornoSql['doc_id']) && !empty($retornoSql['tosid'])) {
        
        $diasEmDivergencia = retornaTotalDiasEmDivergencia( $odsid,  $retornoSql['tosid'] );
        
        $sql = "update fabrica.ordemservico set odsdtprevtermino = date(odsdtprevtermino) + {$diasEmDivergencia} where odsid = " . $odsid;

        $db->executar($sql);
        $db->commit();

        enviaEmailFluxoHistoricoOS($odsid);

        return true;
    } else {

        return false;
    }
}

/**
 * Retorna o total de dias que uma OS se encontra em divergência. 
 * Os tipos de OS existentes são:
 * WORKFLOW_ORDEM_SERVICO = 27
 * WORKFLOW_CONTAGEM_PF = 38
 * 
 * @global cls_banco $db
 * @param int $odsId - Código da OS
 * @param int $tosId - Tipo da OS 
 * 
 * @return int
 */
function retornaTotalDiasEmDivergencia( $odsId, $tosId )
{
    global $db;
    $esdidDivergencia   = (TIPO_OS_GERAL == $tosId ? WF_ESTADO_OS_DIVERGENCIA : WF_ESTADO_CPF_DIVERGENCIA);
    $docmentoId         = (TIPO_OS_GERAL == $tosId ? 'os.docid' : 'os.docidpf');
    $tipoOrdemServico   = (TIPO_OS_GERAL == $tosId ? WORKFLOW_ORDEM_SERVICO : WORKFLOW_CONTAGEM_PF);
    
    $sqlDiasEmDivergencia = "SELECT ( ( date( current_date ) - date( hd.htddata ) ) + 1) as dias_divergencia
            FROM fabrica.ordemservico os
            INNER JOIN workflow.documento dc
                    ON dc.docid = {$docmentoId}
            INNER JOIN workflow.historicodocumento hd
                    ON dc.docid = hd.docid
            INNER JOIN workflow.acaoestadodoc a
                    ON a.aedid  = hd.aedid
            INNER JOIN workflow.estadodocumento ed
                    ON ed.esdid = dc.esdid
            WHERE a.esdiddestino    = {$esdidDivergencia}
            AND ed.tpdid            = {$tipoOrdemServico}
            AND os.odsid          = {$odsId}
            ORDER BY os.odsid desc, hd.htddata desc
            LIMIT 1";
            
            
    return $db->pegaUm($sqlDiasEmDivergencia);
}