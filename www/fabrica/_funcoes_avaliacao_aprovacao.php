<?php

function carregarMenuAvaliacaoSolicitacao() {
    // monta menu padrão contendo informações sobre as entidades
    $menu = array( 0 => array( "id"        => 1, "descricao" => "Avaliar / Aprovar Solicitação", "link"      => "/fabrica/fabrica.php?modulo=principal/abrirSolicitacao&acao=A&ansid=" . $_SESSION['fabrica_var']['ansid'] . "&scsid=" . $_SESSION['fabrica_var']['scsid'] ),
        1           => array( "id"        => 2, "descricao" => "Observações", "link"      => "/fabrica/fabrica.php?modulo=principal/cadSSObservacao&acao=A&tipoobs=cadAvaliacaoAprovacao" ),
        2           => array( "id"        => 3, "descricao" => "Anexos da ordem de serviço", "link"      => "/fabrica/fabrica.php?modulo=principal/cadDetalhamentoAnexos&acao=C" ),
        3           => array( "id"        => 4, "descricao" => "Providências", "link"      => "/fabrica/fabrica.php?modulo=principal/providencias&acao=A" )
    );
    return $menu;
}

function pegarFasesPorArea( $dados ) {
    global $db;
    $habil = "N";
    if ( $dados['aatid'] ) {
        $habil = "S";
        $sql   = "SELECT e.etpid as codigo, e.etpdsc as descricao FROM fabrica.etapa e
				LEFT JOIN fabrica.etapaareaatuacao a ON e.etpid=a.etpid
				WHERE a.aatid='" . $dados['aatid'] . "'";
    }

    $db->monta_combo( 'etpid', $sql, $habil, 'Selecione', '', '', '', '', 'S', 'etpid' );
}

function inserirMonitoramentoRiscos( $dados ) {
    global $db;
    $sql = "INSERT INTO fabrica.monitorarisco(etpid, scsid, mntitem, mntdsc, mntstatus)
    		VALUES ('" . $dados['etpid'] . "', '" . $_SESSION['fabrica_var']['scsid'] . "', '" . $dados['mntitem'] . "', '" . $dados['mntdsc'] . "', 'A');";

    $db->executar( $sql );

    $db->commit();

    echo "<script>
			alert('Monitoramento / Controle de riscos inserido com sucesso');
			window.location='" . $_SERVER['HTTP_REFERER'] . "';
		  </script>";
}

function atualizarMonitoramentoRiscos( $dados ) {
    global $db;
    $sql = "UPDATE fabrica.monitorarisco
   			SET etpid='" . $dados['etpid'] . "', mntitem='" . $dados['mntitem'] . "', mntdsc='" . $dados['mntdsc'] . "'
 			WHERE mntid='" . $dados['mntid'] . "';";

    $db->executar( $sql );

    $db->commit();

    echo "<script>
			alert('Monitoramento / Controle de riscos atualizado com sucesso');
			window.location='" . $_SERVER['HTTP_REFERER'] . "&mntid=" . $dados['mntid'] . "';
		  </script>";
}

function enviar_execucao( $scsid ) {
    global $db;

    $sql           = "SELECT * FROM fabrica.ordemservico WHERE scsid='" . $scsid . "'";
    $ordemservicos = $db->carregar( $sql );

    if ( $ordemservicos[0] ) {
        foreach ( $ordemservicos as $os ) {
            if ( !$os['docid'] ) {

                $docdsc = "Fluxo da ordem de serviço - ID " . $os['odsid'];
                // cria documento
                $docid  = wf_cadastrarDocumento( WORKFLOW_ORDEM_SERVICO, $docdsc );
                $sql    = "UPDATE fabrica.ordemservico SET docid='" . $docid . "' WHERE odsid='" . $os['odsid'] . "'";
                $db->executar( $sql );
            }
        }
    }
    $db->commit();

    //evia email Solicitação de Serviço
    enviaEmailFluxoHistorico( $scsid );

    return true;
}

function pegarDocidOrdemServico( $dados ) {
    global $db;
    $sql = "SELECT docid FROM fabrica.ordemservico WHERE scsid = '" . $dados['scsid'] . "'";
    return (integer) $db->pegaUm( $sql );
}

function salvarOSContagemPF( $dados ) {
    global $db;

    $odsdtprevinicio  = formata_data_sql( $dados['odsdtprevinicio'] );
    $odsdtprevtermino = formata_data_sql( $dados['odsdtprevtermino'] );
    $ctrid            = $dados['ctrid_disable'];
    $vgcid            = $dados['vgcid'];
    $mtiid            = $dados['mtiid'];

    if ( $dados['odsid'] ) {
        $sql = "UPDATE fabrica.ordemservico
   			SET odsdetalhamento='" . $dados['odsdetalhamento'] . "',
   				odsdtprevinicio='{$odsdtprevinicio}',
   				odsdtprevtermino='{$odsdtprevtermino}',
                                ctrid = $ctrid
 			WHERE odsid='" . $dados['odsid'] . "';";
        $db->executar( $sql );

        $arrReturn['msg']   = "OS #{$dados['odsid']} alterada com sucesso!";
        $arrReturn['odsid'] = $dados['odsid'];
        return $arrReturn;
    } else {

        if ( $dados['odsidpai'] ) {

            $scsid = $db->pegaUm( "select scsid from fabrica.ordemservico where odsid=" . $dados['odsidpai'] );

            $sql            = "INSERT INTO fabrica.ordemservico(
		            scsid, odsdetalhamento, odsdtprevinicio, odsdtprevtermino,
		            odsqtdpfestimada,odscontratada,odsidpai,tosid, ctrid, vgcid, mtiid)
		    		VALUES ({$scsid}, '{$dados['odsdetalhamento']}', '{$odsdtprevinicio}',
							'{$odsdtprevtermino}',null,TRUE,'{$dados['odsidpai']}',{$dados['tosid']}, $ctrid, $vgcid, $mtiid) RETURNING odsid;";
            $dados['odsid'] = $db->pegaUm( $sql );

            $docdsc  = "Fluxo de Contagem de P.F. - ID " . $dados['odsid'];
            // cria documento
            $docidpf = wf_cadastrarDocumento( WORKFLOW_CONTAGEM_PF, $docdsc );
            $sql     = "UPDATE fabrica.ordemservico SET docidpf = $docidpf WHERE odsid = " . $dados['odsid'];
            $db->executar( $sql );
            $db->commit();

            $arrReturn['msg']   = "OS #{$dados['odsid']} inserida com sucesso!";
            $arrReturn['odsid'] = $dados['odsid'];


            //caso o intervalo da Data de previsao de termino e a data de previsão de inicio for menor ou igual a dois dias, deve enviar email para os prepostos
            //da Squadra.
            $timestampInicial   = strtotime( $odsdtprevinicio );
            $timestampTermino   = strtotime( $odsdtprevtermino );
            $timestampDiferenca = $timestampTermino - $timestampInicial;
            $dias               = (int) floor( $timestampDiferenca / (60 * 60 * 24) );

            if ( $dias <= 2 ) {
                $sql = "SELECT tosid, tosdsc
                           FROM fabrica.tipoordemservico
                           WHERE tosid = {$dados['tosid']}";

                $tipoOrdem = $db->pegaLinha( $sql );


                $conteudo = '<p><strong>Listagem de Ordem de Serviço</strong><p>';
                $conteudo .= '<p>Prezado(a) Preposto(a),</p>';
                $conteudo .= '<p>As OS relacionada abaixo, possue data de encerramento previsto para os próximos 2(dois) dias.</p>';
                $conteudo .= "<p>Número da SS: <strong> {$scsid} </strong></p>";
                $conteudo .= "<p>Número da OS: <strong> {$dados['odsid']} </strong></p>";
                $conteudo .= "<p>Número da OS relacionado: <strong> {$dados['odsidpai']} </strong></p>";
                $conteudo .= "<p>Descrição da OS: <strong> {$dados['odsdetalhamento']} </strong></p>";
                $conteudo .= "<p>Previsão de início: <strong> {$dados['odsdtprevinicio']} </strong></p>";
                $conteudo .= "<p>Tipo de Ordem de Serviço: <strong> {$tipoOrdem['tosdsc']} </strong></p>";

                $assunto = "SIMEC - Fábrica - Aviso de criação de Ordem de Serviço";

                $remetente = array( );
                $destinatarios = array( );
                $remetente['email'] = "noreply@mec.gov.br";
                $remetente['nome']  = "SIMEC";

                $sqlPrepostoSquadra = "SELECT usu.usuemail
                    FROM seguranca.usuario usu
                    INNER JOIN seguranca.perfilusuario pu
                        ON usu.usucpf = pu.usucpf	
                    INNER JOIN seguranca.perfil per
                        ON per.pflcod = pu.pflcod
                    WHERE per.pflcod = " . PERFIL_CONTAGEM_PF . "  
                    ORDER BY pu.pflcod;";

                $arrPrepostoEficacia = $db->carregar( $sqlPrepostoSquadra );
                foreach ( $arrPrepostoEficacia as $destinatario ) {
                    $destinatarios[] = $destinatario['usuemail'];
                }

                if ( enviar_email( $remetente, $destinatarios, $assunto, $conteudo ) ) {
                    $arrReturn['msg'] .= "\nE-mail enviado para o preposto(a) responsável";
                } else {
                    $arrReturn['msg'] .= "\nNão foi possível enviar o e-mail para o preposto(a) responsável";
                }
            }
        } else {
            $arrReturn['msg']   = "Erro, Favor entrar em contato com o administrador do sistema!";
            $arrReturn['odsid'] = '';
        }
        return $arrReturn;
    }
}

function verificarPrevisaoOSContagem( $odsid ) {
    global $db;

    $sql  = "select count(*) from fabrica.ordemservico where odsid = $odsid and odsdetalhamento is not null and odsdtprevinicio is not null and odsdtprevtermino is not null";
    $dado = $db->pegaUm( $sql );
    if ( !$dado ) {
        return "Favor informar a previsão de início e término de atendimento!";
    } else {
        return true;
    }
}

function verificarEnvioContagemPF( $odsid ) {
    global $db;

    $sql     = "select docidpf from fabrica.ordemservico where odsidpai = $odsid";
    $docidpf = $db->pegaUm( $sql );
    if ( $docidpf ) {
        $arrEstado = wf_pegarEstadoAtual( $docidpf );
        if ( WF_ESTADO_CPF_PENDENTE != $arrEstado['esdid'] ) {
            return $arrEstado['esddsc'];
        } else {
            return false;
        }
    } else {
        return false;
    }
}

function verificarContagemPF( $odsid = null, $tosid = null ) {
    global $db;

    if ( $tosid == TIPO_OS_CONTAGEM_ESTIMADA ) {
        $campo = "odsqtdpfestimada";
        $taoid = "taoid IN (" . TPANEXO_RELATORIO_PF_ESTIMADA . "," . TPANEXO_RELATORIO_PF . ")";
    } else {
        $campo = "odsqtdpfdetalhada";
        $taoid = "taoid IN (" . TPANEXO_RELATORIO_PF_DETALHADA . "," . TPANEXO_RELATORIO_PF . ")";
    }


    $sql = "select
				count(os.odsid)
			from
				fabrica.ordemservico os
			inner join
				fabrica.anexoordemservico anx ON anx.odsid = os.odsid
			where
				os.odsid = $odsid
			and
				$campo is not null
			and
				arqid is not null
			and
				$taoid";
    //dbg($sql,1);
    $res = $db->pegaUm( $sql );
    if ( $res ) {
        
        $ordemServico        = new OrdemServico();
        $ordemServico        = $ordemServico->recuperePorId( $odsid );
        $isDivergente        = !validaDivergencia( $ordemServico->getIdPai(), 'OS', 'Detalhada' );
        
        if( $isDivergente ) return "Os valores de P.F estão divergentes!";
        
        return true;
    } else {
        return "É necessário incluir o volume de contagem de P.F. e o Relatório de contagem de PF!";
    }
}

function mostrarAnexos( $dados ) {
    global $db;



    $sql = "SELECT
				'<div align=center><a style=\"cursor: pointer; color: blue;\" onclick=\"window.location=\'{$_SERVER['REQUEST_URI']}&download=S&arqid=' || ar.arqid || '\';\" /><img src=../imagens/anexo.gif></a></div>' as acao,
				'<div align=center>'||odsid||'</div>' as os,
				tp.taodsc,
				'<a style=\"cursor: pointer; color: blue;\" onclick=\"window.location=\'{$_SERVER['REQUEST_URI']}&download=S&arqid=' || ar.arqid || '\';\" />' || ar.arqnome||'.'||ar.arqextensao ||'</a>',
				'<div align=center>'||ar.arqtamanho||'</div>' as tam,
				an.aosdsc,
				'<div align=center>'||to_char(an.aosdtinclusao,'dd/mm/YYYY HH24:MI') ||'</div>' as data
			FROM fabrica.anexoordemservico an
			LEFT JOIN fabrica.tipoanexoordem tp ON an.taoid=tp.taoid
			LEFT JOIN public.arquivo ar ON ar.arqid=an.arqid
			WHERE odsid=" . $dados['odsid'] . " AND aosstatus='A'";

    $cabecalho = array( "Ação", "Nº OS", "Tipo Arquivo", "Nome arquivo", "Tamanho(bytes)", "Descrição", "Data inclusão" );
    $db->monta_lista_simples( $sql, $cabecalho, 50, 5, 'N', '100%', $par2 );
    exit;
}

// Verifica se existe uma ordem de serviço estimada para a solicitação de serviço, caso exista, deixa aprovar
function regraAprovarSS( $scsid ) {
    global $db;

    $sql = "select
                count(*)

                from fabrica.ordemservico as os
                    inner join fabrica.tipoordemservico as tos
                        on tos.tosid=os.tosid
                where os.scsid = {$scsid} and os.tosid = " . TIPO_OS_CONTAGEM_ESTIMADA;

    $contagemEstimada = $db->pegaUm( $sql );

    if ( $contagemEstimada > 0 ) {
        return true;
    } else {
        return false;
    }
}

/**
 * Envia a os do tipo geral para execução de uma determinada SS
 * Executada no pós - ação ao 'Aprovar' uma SS
 * 
 * @global cls_banco $db
 * @param int $scsid
 * @return bool
 */
function envia_os_pausa_execucao( $scsid )
{
    global $db;
    
    $sql     = "SELECT odsid, docid
                FROM fabrica.ordemservico
                WHERE scsid = {$scsid}
                AND tosid = 1";
                
    $dadosOS        = $db->pegaLinha($sql);
    $esdidorigem    = wf_pegarEstadoAtual(  $dadosOS['docid'] );
    
    if( $esdidorigem['esdid'] != WF_ESTADO_OS_PENDENTE )
    {
        return true;
    }
    
    $rsPegarAcao 	= wf_pegarAcao( $esdidorigem['esdid'], WF_ESTADO_OS_EXECUCAO );
    return wf_alterarEstado( $dadosOS['docid'], $rsPegarAcao['aedid'], '', array( 'odsid' => $dadosOS['odsid'] ));
}

/**
 * Tramita a OS do item 1 vinculado a OS em trâmite atual
 * Tramita a OS do item 1 para o estado de DIVERGÊNCIA
 * Executada quando uma OS do tipo DETALHADA é encaminhada para DIVERGÊNCIA
 * 
 * @param int $odsid
 * @return bool
 */
function tramitarOSDivergencia( $odsid )
{
    $objOrdemServico     = new OrdemServico();
    $ordemServico        = $objOrdemServico->recuperePorId( $odsid );
    $ordemServicoPai     = $objOrdemServico->recuperePorId( $ordemServico->getIdPai() );
    $arrAcao             = wf_pegarAcao(WF_ESTADO_OS_VERIFICACAO, WF_ESTADO_OS_DIVERGENCIA);
    
    enviaEmailFluxoHistoricoOS( $odsid );
    
    return wf_alterarEstado($ordemServicoPai->getIdDocumento(), $arrAcao['aedid'], '', array( 'odsid' => $ordemServicoPai->getId() ));
}

/**
 * Tramita a OS do tipo GERAL
 * Executada quando uma OS do tipo GERAL é encaminhada para Em Avaliação a partir 
 * do estado de Divergência
 * 
 * @param int $odsid
 * @return bool
 */
function tramitarOSTipoGeralDivergencia( $odsid )
{
    $objOrdemServico        = new OrdemServico();
    $ordemServicoDetalhada  = $objOrdemServico->recuperarOSDetalhadaVinculada( $odsid );        
    $arrAcao                = wf_pegarAcao(WF_ESTADO_CPF_DIVERGENCIA,  WF_ESTADO_CPF_AGUARDANDO_CONTAGEM );
    
    adicionaTempoDivergencia($odsid);
    enviaEmailFluxoHistoricoOS( $odsid );
    
    return wf_alterarEstado($ordemServicoDetalhada->getIdDocumentoPf(), $arrAcao['aedid'], '', array( 'odsid' => $ordemServicoDetalhada->getId() ));
}

function tramitarOSTipoDetalhadaDivergencia( $odsid )
{
    
    global $db;
    
    $ordemServico       = new OrdemServico();
    $diasEmDivergencia  = retornaTotalDiasEmDivergencia( $odsid, TIPO_OS_CONTAGEM_DETALHADA );
    
    $sqlDataPrevisaoTermino    = "SELECT to_char( odsdtprevtermino, 'DD/MM/YYYY') as odsdtprevtermino, 
                                        to_char( (date(odsdtprevtermino) + {$diasEmDivergencia}), 'DD/MM/YYYY') as data_finalizacao                                        
                                        FROM fabrica.ordemservico WHERE odsid = {$odsid}";
                                        
    $dadosPrevistaoTermino      = $db->pegaLinha($sqlDataPrevisaoTermino);
    $oHistoricoPrevisaoTermino  = new HistoricoPrevisaoTermino();
    $dadosPrevTermino           = array(
        'prevtermino'   => $dadosPrevistaoTermino['data_finalizacao'],
        'odsid'         => $odsid,
        'obsdsc'        => 'Redefinição de prazo por tramitação de retorno de divergência',
        'data_tramite'  => $ordemServico->dataEntradaEstado($odsid, WF_ESTADO_CPF_DIVERGENCIA )->format('Y-m-d H:i:s')
    );  
    
    if( ( (bool) $oHistoricoPrevisaoTermino->alterarPrevisaoTerminoEmpresaItem2($dadosPrevTermino) ) === false )
    {
        return false;
    }
    
    $db->commit();
    
    
    return true;
}

/** 
 * Verifica se a OS informado é do tipo Estimada
 * Executada como condição para exibição ou não da ação para enviar para 'Em revisão'
 * a partir do estado 'Aguardando Contagem'
 * @param int $odsid 
 * @return bool
 */

function verificarOrdemServicoEstimada( $odsid )
{
    $objOrdemServico = new OrdemServico();
    $ordemServico = $objOrdemServico->recuperePorId($odsid);
    
    if( $ordemServico->getIdTipoOrdemServico() == TIPO_OS_CONTAGEM_ESTIMADA )
    {
        return true;
    }
    
    return false;
}