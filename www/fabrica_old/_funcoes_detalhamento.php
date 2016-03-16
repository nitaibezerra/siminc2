<?php

function downloadAnexoDetalhamentoServico( $dados ) {
    include_once APPRAIZ . "includes/classes/fileSimec.class.inc";
    $file = new FilesSimec( "anexoordemservico", NULL, "fabrica" );
    $file->getDownloadArquivo( $dados['arqid'] );
}

function carregarMenuDetalhamentoSolicitacao() {
    // monta menu padrão contendo informações sobre as entidades
    $menu = array( 0 => array( "id"        => 1, "descricao" => "Detalhar Solicitação", "link"      => "/fabrica/fabrica.php?modulo=principal/abrirSolicitacao&acao=A&ansid=" . $_SESSION['fabrica_var']['ansid'] . "&scsid=" . $_SESSION['fabrica_var']['scsid'] ),
        1           => array( "id"        => 2, "descricao" => "Observações", "link"      => "/fabrica/fabrica.php?modulo=principal/cadSSObservacao&acao=A&tipoobs=cadDetalhamento" ),
        2           => array( "id"        => 3, "descricao" => "Anexos da Solicitação", "link"      => "/fabrica/fabrica.php?modulo=principal/analiseDemandaAnexos&acao=C" ),
        3           => array( "id"        => 4, "descricao" => "Anexos da Ordem de Serviço", "link"      => "/fabrica/fabrica.php?modulo=principal/cadDetalhamentoAnexos&acao=A" ),
        4           => array( "id"        => 5, "descricao" => "Providências", "link"      => "/fabrica/fabrica.php?modulo=principal/providencias&acao=A" )
    );
    return $menu;
}

function inserirDetalhamentoSolicitacaoServico( $dados ) {
    global $db;

    
    $dadosctr = $db->pegaLinha( "SELECT c.ctrid, c.ctrqtdpfalocado, c.ctrqtdpfcontrato,a.scsid FROM fabrica.analisesolicitacao a 
							  	LEFT JOIN fabrica.contrato c ON c.ctrid = a.ctrid
					  	  		WHERE ansid='" . $_SESSION['fabrica_var']['ansid'] . "' AND ctrcontagem=FALSE" );
	/*
    if ( $dados['odssubtotalpf'] )
        $odssubtotalpf = str_replace( ',', '.', str_replace( '.', '', $dados['odssubtotalpf'] ) );
	
    if ( $dados['odsqtdpfestimada'] )
        $vlEstimada = str_replace( ',', '.', str_replace( '.', '', $dados['odsqtdpfestimada'] ) );

    if ( $dadosctr ) {
        if ( $dadosctr['ctrqtdpfcontrato'] < ($dadosctr['ctrqtdpfalocado'] + (($vlEstimada) ? $vlEstimada : "0")) ) {

            die( "<script>
					alert('O contrato não possui pontos de função suficiente');
					window.location='fabrica.php?modulo=principal/cadDetalhamento&acao=A';
		  		 </script>" );
        }
    }
	*/
    if ( $dados['odssubtotalpf'] ){
        $odssubtotalpf = str_replace( ',', '.', str_replace( '.', '', $dados['odssubtotalpf'] ) );
    }else{
    	$odssubtotalpf = 0;
    }
    
    
    if ( $dados['odsqtdpfestimada'] ){
        $vlEstimada = str_replace( ',', '.', str_replace( '.', '', $dados['odsqtdpfestimada'] ) );
    }else{
    	$vlEstimada = 0;
    }
    //verifica saldo na vigencia do contrato
    $sql = "select distinct
	                vi.vcmid, vi.vcmvolumecontratado, vi.vcmvolumeutilizado
	          from 
	                fabrica.vigenciacontratometricaitem vi
	          inner join fabrica.vigenciacontrato vc on vc.vgcid = vi.vgcid and vc.vgcstatus='A'
	          inner join fabrica.analisesolicitacao an on an.vgcid = vi.vgcid and an.mtiid = vi.mtiid
	          where an.ansid=".$_SESSION['fabrica_var']['ansid'];
    $saldo = $db->pegaLinha($sql);
    $vcmid = $saldo['vcmid'];
	$vcmvolumecontratado = $saldo['vcmvolumecontratado'] ? $saldo['vcmvolumecontratado'] : 0; 
	$vcmvolumeutilizado = $saldo['vcmvolumeutilizado'] ? $saldo['vcmvolumeutilizado'] : 0;
	
	if($vcmid){
	    if ( $vcmvolumecontratado < ($vcmvolumeutilizado + (($vlEstimada) ? $vlEstimada : 0)) ) {
	
		    die( "<script>
				alert('O contrato não possui pontos de função suficiente');
				window.location='fabrica.php?modulo=principal/cadDetalhamento&acao=A';
	 		 </script>" );
	    }
	    
	    //atualiza saldo
	    $db->executar("UPDATE fabrica.vigenciacontratometricaitem SET vcmvolumeutilizado=COALESCE(vcmvolumeutilizado,0)+" . (($vlEstimada) ? $vlEstimada : "0") . " WHERE vcmid='" . $vcmid . "'" );
	}
    
    
    
    $odsdtprevinicio  = formata_data_sql( $dados['odsdtprevinicio'] );
    $odsdtprevtermino = formata_data_sql( $dados['odsdtprevtermino'] );
    $ansprevtermino	  = formata_data_sql( $dados['ansprevtermino'] );
    $ansmensuravel    = $_POST['ansmensuravel'];
    $scsid            = $dadosctr['scsid'];
    
    $sql = "INSERT INTO fabrica.ordemservico(
            scsid,ctrid, odsdetalhamento, odsenderecosvn, odsdtprevinicio, odsdtprevtermino, 
            odssubtotalpf,odsqtdpfestimada,odscontratada)
    		VALUES ({$dados['scsid']},{$dados['ctrid']}, '".addslashes($dados['odsdetalhamento'])."', '{$dados['odsenderecosvn']}', '{$odsdtprevinicio}', 
					'{$odsdtprevtermino}', " . (($odssubtotalpf) ? "'{$odssubtotalpf}'" : "NULL") . ", " . (($vlEstimada) ? "'{$vlEstimada}'" : "NULL") . ",TRUE) RETURNING odsid;";

    $dados['odsid'] = $db->pegaUm( $sql );

    if ( !$dadosctr['ctrid'] ) {
        $dadosctr['ctrid'] = $_POST['ctrid'];
    }
    
    // SS-982 - REQ002 [inicio] Solicitação de Serviço - Detalhamento da Solicitação
    $dataFinalOS   = strtotime( $odsdtprevtermino );
    $dataFinalSS   = strtotime( $ansprevtermino  );

    if( ($dataFinalOS > $dataFinalSS) && !empty($dados['odsid']) )
    {
    	$sqlAns = "update fabrica.analisesolicitacao set ansprevtermino = '$odsdtprevtermino' where scsid = $scsid";
    	$db->executar( $sqlAns );
    	$db->commit();
    }
    // SS-982 - REQ002 [fim]

    enviar_execucao( $scsid );

    //$db->executar( "UPDATE fabrica.contrato SET ctrqtdpfalocado=COALESCE(ctrqtdpfalocado,0)+" . (($vlEstimada) ? $vlEstimada : "0") . " WHERE ctrid='" . $dadosctr['ctrid'] . "'" );
    $db->executar( "UPDATE fabrica.analisesolicitacao SET mensuravel= $ansmensuravel WHERE scsid = $scsid " );
    $db->commit();

    $msgRetorno = 'Detalhamento da solicitação de serviço inserida com sucesso';

    //caso o intervalo da Data de previsao de termino e a data de previsão de inicio for menor ou igual a dois dias, deve enviar email para os prepostos
    //da Squadra.
    $timestampInicial   = strtotime( $odsdtprevinicio );
    $timestampTermino   = strtotime( $odsdtprevtermino );
    $timestampDiferenca = $timestampTermino - $timestampInicial;
    $dias               = (int) floor( $timestampDiferenca / (60 * 60 * 24) );

    if ( $dias <= 2 ) {
        $conteudo = '<p><strong>Listagem de Ordem de Serviço</strong><p>';
        $conteudo .= '<p>Prezado(a) Preposto(a),</p>';
        $conteudo .= '<p>As OS relacionada abaixo, possue data de encerramento previsto para os próximos 2(dois) dias.</p>';
        $conteudo .= "<p>Número da SS: <strong> {{$dados['scsid']} </strong></p>";
        $conteudo .= "<p>Número da OS: <strong> {$dados['odsid']} </strong></p>";
        $conteudo .= "<p>Previsão de início: <strong> {$dados['odsdtprevinicio']} </strong></p>";
        $conteudo .= "<p>Previsão de término: <strong> {$dados['odsdtprevtermino']} </strong></p>";
        
        if($dados['sigla'] == 'PF'){
	        $conteudo .= "<p>Qtd. estimada de PF: <strong> {$dados['odsqtdpfestimada']} </strong></p>";
	        $conteudo .= "<p>Subtotal de PF: <strong> {$odssubtotalpf} </strong></p>";	
        }else{
        	$conteudo .= "<p>Qtd. estimada de UST: <strong> {$dados['odsqtdpfestimada']} </strong></p>";
        }
        
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
                                WHERE per.pflcod = " . PERFIL_PREPOSTO . "  
                                ORDER BY pu.pflcod;";

        $arrPrepostoSquadra = $db->carregar( $sqlPrepostoSquadra );
        foreach ( $arrPrepostoSquadra as $destinatario ) {
            $destinatarios[] = $destinatario['usuemail'];
        }

        if ( enviar_email( $remetente, $destinatarios, $assunto, $conteudo ) ) {
            $msgRetorno .= '\nE-mail enviado para o preposto(a) responsável';
        }
    }

    echo "<script>
			alert('" . $msgRetorno . "');
			window.location='fabrica.php?modulo=principal/cadDetalhamento&acao=A';
		  </script>";
}

function verificaPontosFuncao( $dados ) {
    global $db;

    $limite = 'S';
	/*
    $dadosctr = $db->pegaLinha( "SELECT c.ctrid, c.ctrqtdpfalocado, c.ctrqtdpfcontrato FROM fabrica.analisesolicitacao a 
							  	LEFT JOIN fabrica.contrato c ON c.ctrid = a.ctrid
					  	  		WHERE ansid='" . $_SESSION['fabrica_var']['ansid'] . "'" );

    if ( $dados['odsqtdpfestimada'] )
        $vlEstimada = str_replace( ',', '.', str_replace( '.', '', $dados['odsqtdpfestimada'] ) );

    if ( $dadosctr ) {
        if ( $dadosctr['ctrqtdpfcontrato'] < ($dadosctr['ctrqtdpfalocado'] + (($vlEstimada) ? $vlEstimada : "0")) ) {
            $limite = 'N';
        }
    }
	*/
    
    if ( $dados['odsqtdpfestimada'] )
        	$vlEstimada = str_replace( ',', '.', str_replace( '.', '', $dados['odsqtdpfestimada'] ) );
        	
	//verifica saldo na vigencia do contrato
    $sql = "select distinct
	                vi.vcmvolumecontratado, vi.vcmvolumeutilizado
	          from 
	                fabrica.vigenciacontratometricaitem vi
	          inner join fabrica.vigenciacontrato vc on vc.vgcid = vi.vgcid and vc.vgcstatus='A'
	          inner join fabrica.analisesolicitacao an on an.vgcid = vi.vgcid and an.mtiid = vi.mtiid
	          where an.ansid=".$_SESSION['fabrica_var']['ansid'];
    $saldo = $db->pegaLinha($sql);
	$vcmvolumecontratado = $saldo['vcmvolumecontratado'] ? $saldo['vcmvolumecontratado'] : 0; 
	$vcmvolumeutilizado = $saldo['vcmvolumeutilizado'] ? $saldo['vcmvolumeutilizado'] : 0;
	
	if($vcmid){
		
	    if ( $vcmvolumecontratado < ($vcmvolumeutilizado + (($vlEstimada) ? $vlEstimada : 0)) ) {
		   $limite = 'N';
	    }
	    
	}
    
    echo $limite;
    exit;
}

function inserirAnexoDetalhamento( $dados ) {

    if ( $_FILES['arquivo']['error'] == 0 ) {

        include_once APPRAIZ . "includes/classes/fileSimec.class.inc";

        $campos = array( "taoid"         => "'" . $dados['taoid'] . "'",
            "odsid"         => "'" . $dados['odsid'] . "'",
            "aosdsc"        => "'" . $dados['aosdsc_'] . "'",
            "aosdtinclusao" => "NOW()",
            "aosstatus"     => "'A'" );

        $file = new FilesSimec( "anexoordemservico", $campos, "fabrica" );
        $file->setUpload( (($dados['aosdsc_']) ? $dados['aosdsc_'] : NULL ), $key  = "arquivo" );

        if ( $dados['redirecionamento'] ) {

            echo "<script>
					alert('Arquivo anexado com sucesso');
					window.location='" . $dados['redirecionamento'] . "';
				  </script>";
        }
    }
}

function atualizarDetalhamentoSolicitacaoServico( $dados ) {
    global $db;

    $dadosctr = $db->pegaLinha( "SELECT c.ctrid, c.ctrqtdpfalocado, c.ctrqtdpfcontrato FROM fabrica.analisesolicitacao a 
							  	LEFT JOIN fabrica.contrato c ON c.ctrid = a.ctrid
					  	  		WHERE ansid='" . $_SESSION['fabrica_var']['ansid'] . "' AND ctrcontagem=FALSE" );
	/*
    if ( $dadosctr ) {
        if ( $dadosctr['ctrqtdpfcontrato'] < ($dadosctr['ctrqtdpfalocado'] + ($dados['odsqtdpfestimada'] - $dados['odsqtdpfestimada_'])) ) {

            die( "<script>
					alert('O contrato não possui pontos de função suficiente');
					window.location='fabrica.php?modulo=principal/cadDetalhamento&acao=A';
		  		 </script>" );
        }
    }
	*/
	if ( $dados['odssubtotalpf'] ){
        $odssubtotalpf = str_replace( ',', '.', str_replace( '.', '', $dados['odssubtotalpf'] ) );
    }else{
    	$odssubtotalpf = 0;
    }
    
    if ( $dados['odsqtdpfestimada'] ){
        $vlEstimada = str_replace( ',', '.', str_replace( '.', '', $dados['odsqtdpfestimada'] ) );
    }else{
    	$vlEstimada = 0;
    }
    
    if ( $dados['odsqtdpfestimada_'] ){ //não vem formatada
        $vlEstimada_ = $dados['odsqtdpfestimada_'];
    }else{
    	$vlEstimada_ = 0;
    }
        	
	//verifica saldo na vigencia do contrato
    $sql = "select distinct
	                vi.vcmid, vi.vcmvolumecontratado, vi.vcmvolumeutilizado
	          from 
	                fabrica.vigenciacontratometricaitem vi
	          inner join fabrica.vigenciacontrato vc on vc.vgcid = vi.vgcid and vc.vgcstatus='A'
	          inner join fabrica.analisesolicitacao an on an.vgcid = vi.vgcid and an.mtiid = vi.mtiid
	          where an.ansid=".$_SESSION['fabrica_var']['ansid'];
    $saldo = $db->pegaLinha($sql);
    $vcmid = $saldo['vcmid'];
	$vcmvolumecontratado = $saldo['vcmvolumecontratado'] ? $saldo['vcmvolumecontratado'] : 0; 
	$vcmvolumeutilizado = $saldo['vcmvolumeutilizado'] ? $saldo['vcmvolumeutilizado'] : 0;
	
	if($vcmid){
	    if ( $vcmvolumecontratado < ($vcmvolumeutilizado + ($vlEstimada - $vlEstimada_)) ) {
		    die( "<script>
				alert('O contrato não possui pontos de função suficiente');
				window.location='fabrica.php?modulo=principal/cadDetalhamento&acao=A';
	 		 </script>" );
	    }
	    
	    //atualiza saldo
	    $dif = $odsqtdpfestimada - $odsqtdpfestimada_;
	    $db->executar( "UPDATE fabrica.vigenciacontratometricaitem SET vcmvolumeutilizado=COALESCE(vcmvolumeutilizado,0)" . (($dif >= 0) ? "+" . $dif : "-" . ($dif * -1)) . " WHERE vcmid='" . $vcmid . "'" );
	    
	}    


    $odsdtprevinicio  = formata_data_sql( $dados['odsdtprevinicio'] );
    $odsdtprevtermino = formata_data_sql( $dados['odsdtprevtermino']);
    $ansprevtermino	  = formata_data_sql( $dados['ansprevtermino']  );
	
    $sql = "UPDATE fabrica.ordemservico
   			SET odsdetalhamento='" . $dados['odsdetalhamento'] . "', 
   				odssubtotalpf='" . $odssubtotalpf . "', 
   				odsqtdpfestimada='" . $vlEstimada . "', 
   				odsdtprevinicio='{$odsdtprevinicio}', 
   				odsdtprevtermino='{$odsdtprevtermino}',
   				odsenderecosvn='" . $dados['odsenderecosvn'] . "',
   				odscontratada = TRUE 
 			WHERE odsid='" . $dados['odsid'] . "';";
    $db->executar( $sql );
    
    // SS-982 - REQ002 [inicio] Solicitação de Serviço - Detalhamento da Solicitação
    $dataFinalOS   = strtotime( $odsdtprevtermino );
    $dataFinalSS   = strtotime( $ansprevtermino  );
    
    if( ($dataFinalOS > $dataFinalSS) )
    {
    	$sqlAns = "update fabrica.analisesolicitacao set ansprevtermino = '$odsdtprevtermino' where scsid = $scsid";
    	$db->executar( $sqlAns );
    	$db->commit();
    }
    // SS-982 - REQ002 [fim]

    $scsid         = $_POST['scsid'];
    $ansmensuravel = $_POST['ansmensuravel'];

    $db->executar( "UPDATE fabrica.analisesolicitacao SET mensuravel= $ansmensuravel WHERE scsid = $scsid " );

    $db->commit();

    echo "<script>
			alert('Detalhamento da solicitação de serviço atualizada com sucesso');
			window.location='fabrica.php?modulo=principal/cadDetalhamento&acao=A&odsid=" . $dados['odsid'] . "';
		  </script>";
}

function removerAnexoDetalhamento( $dados ) {
    global $db;
    $sql = "UPDATE fabrica.anexoordemservico SET aosstatus='I' WHERE aosid='" . $dados['aosid'] . "'";
    $db->executar( $sql );
    $db->commit();

    echo "<script>
			alert('Anexo removido com sucesso');
			window.location='" . $_SERVER['HTTP_REFERER'] . "';
		  </script>";
}

function alterarConsultaDisciplina( $dados ) {

    $sql = "SELECT d.dspid as codigo, d.dspdsc||' - '||te.tpedsc as descricao FROM fabrica.disciplina d 
			LEFT JOIN fabrica.servicodisciplina sd ON sd.dspid=d.dspid 
			LEFT JOIN fabrica.tipoexecucao te ON te.tpeid=sd.tpeid
			WHERE ansid='" . $_SESSION['fabrica_var']['ansid'] . "' AND te.tpeid='" . $dados['tpeid'] . "' AND 
				  d.dspid NOT IN ( SELECT od.dspid FROM fabrica.ordemservicodisciplina od 
	  				   			   LEFT JOIN fabrica.ordemservico os ON os.odsid=od.odsid 
	  				   			   WHERE os.scsid='" . $_SESSION['fabrica_var']['scsid'] . "' )";

    $_SESSION['indice_sessao_combo_popup']['dspid']['sql'] = $sql;
}

function salvarOSContratante( $dados ) {
    global $db;

    $odsdtprevinicio  = formata_data_sql( $dados['odsdtprevinicio'] );
    $odsdtprevtermino = formata_data_sql( $dados['odsdtprevtermino'] );
    $dados['odsqtdpfestimada']  = ( $dados['odsqtdpfestimada'] ? str_replace( ',', '.', str_replace( '.', '', $dados['odsqtdpfestimada'] ) ) : 0 );
    $dados['odssubtotalpf']  = ( $dados['odssubtotalpf'] ? str_replace( ',', '.', str_replace( '.', '', $dados['odssubtotalpf'] ) ) : 0 );

    if ( $dados['odsid'] ) {
        $sql = "UPDATE fabrica.ordemservico
   			SET odsdetalhamento='" . $dados['odsdetalhamento'] . "', 
   				odssubtotalpf='" . $dados['odssubtotalpf'] . "',
   				odsqtdpfestimada='" . $dados['odsqtdpfestimada'] . "', 
   				odsdtprevinicio='{$odsdtprevinicio}', 
   				odsdtprevtermino='{$odsdtprevtermino}',
   				odscontratada = FALSE 
 			WHERE odsid='" . $dados['odsid'] . "';";
        $db->executar( $sql );
        return "Nº da OS {$dados['odsid']} alterada com sucesso!";
    } else {
        $sql            = "INSERT INTO fabrica.ordemservico(
	            scsid, odsdetalhamento, odsdtprevinicio, odsdtprevtermino, 
	            odssubtotalpf,odsqtdpfestimada,odscontratada)
	    		VALUES ({$_SESSION['fabrica_var']['scsid']}, '".addslashes($dados['odsdetalhamento'])."', '{$odsdtprevinicio}', 
						'{$odsdtprevtermino}'," . ((!empty($dados['odssubtotalpf'])) ? "'{$dados['odssubtotalpf']}'" : "NULL") . ", " . ((!empty($dados['odsqtdpfestimada'])) ? "'{$dados['odsqtdpfestimada']}'" : "NULL") . ",FALSE) RETURNING odsid;";
        $dados['odsid'] = $db->pegaUm( $sql );
        return "Nº da OS {$dados['odsid']} inserida com sucesso!";
    }
}