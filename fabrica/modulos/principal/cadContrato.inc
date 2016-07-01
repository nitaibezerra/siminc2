<?php
$clsFuncoes = new funcoesProjetoFabrica();

// Condição consumida via ajax
if($_REQUEST['requisicaoAjax']){
	header('content-type: text/html; charset=ISO-8859-1');
	$clsFuncoes->$_REQUEST['requisicaoAjax']();
	exit;
}

// Condição consumida via AJAX
if($_REQUEST['requisicaocnpj']){

    $entnumcpfcnpj = str_replace(array('-','/','\\','.',','), '', $_REQUEST['entnumcpfcnpj']);
    $sqlEntidade   = "select entid, entnome from entidade.entidade where entnumcpfcnpj = '" . $entnumcpfcnpj . "' limit 1";

    $resultado = $db->pegaLinha( $sqlEntidade );

    if( !empty($resultado) ){
        $retorno = array( 'status'=>true, 'entid' => $resultado['entid'], 'entnome' => $resultado['entnome'] );
    }else{
        $retorno = array( 'status'=>false );
    }

    echo simec_json_encode( $retorno );
    exit;
}

// Condição consumida via AJAX
if($_REQUEST['periodovigenciavalido']){

    if( !empty($_REQUEST['ctrid']) ){
        $periodoVigencia   = "select count(*) from fabrica.vigenciacontrato 
                                where ctrid = ".$_REQUEST['ctrid']."
                                and ('". formata_data_sql($_REQUEST['vgcdtinicio']) ."' between vgcdtinicio and vgcdtfim
                                or '". formata_data_sql($_REQUEST['vgcdtfim']) ."' between vgcdtinicio and vgcdtfim ) ";
    
        if( !empty($_REQUEST['vgcid']) ){
            $periodoVigencia.= " and vgcid <> " . $_REQUEST['vgcid'];
        }
        
        $resultado = $db->pegaUm( $periodoVigencia );
    }else{
        $resultado = 0;
    }

    if( $resultado == 0 ){
        $retorno = array( 'status'=>'true',  'total' => $resultado );
    }else{
        $retorno = array( 'status'=>'false', 'total' => $resultado );
    }

    echo simec_json_encode( $retorno );
    exit;
}

// Condição consumida via AJAX
if($_REQUEST['periodorepactuacaovalido']){
    
    $periodo = " select count(*)
                from fabrica.repactuacaocontrato 
                where vgcid = ".$_REQUEST['vgcid']."
                    and ( '". formata_data_sql($_REQUEST['rpcdtinicio'] ) ."' between rpcdtinicio and rpcdtfim
                    or '". formata_data_sql($_REQUEST['rpcdtfim']) ."'       between rpcdtinicio and rpcdtfim )
               ";

    if( !empty($_REQUEST['rpcid']) ){
        $periodo .= " and rpcid <> " . $_REQUEST['rpcid'];
    }

    $resultado = $db->pegaUm( $periodo );

    if( $resultado == 0 ){
        $retorno = array( 'status'=>'true',  'total' => $resultado );
    }else{
        $retorno = array( 'status'=>'false', 'total' => $resultado );
    }

    echo simec_json_encode( $retorno );
    exit;
}

// Condição consumida via AJAX
if($_REQUEST['exclusaorepactuacao']){

    if( !empty($_REQUEST['rpcid']) ){
        
        try {
            $sqlDelete = "delete from fabrica.repactuacaocontrato where rpcid = " . $_REQUEST['rpcid'];
            
            if( $db->executar( $sqlDelete ) ){
                $db->commit();
                $retorno = array('retorno' => true);
            }else{
                throw new Exception( "Não foi possível excluir a repactuação." );
            }  
                        
        } catch (Exception $e) {
            $db->rollback();
            $retorno = array('retorno' => false, 'msg' => $e->getMessage() );
        }
    }

    echo simec_json_encode( $retorno );
    exit;
}

// Download do Documento da Repactuação
if( $_REQUEST['download'] == 'S' ){
    
    include_once APPRAIZ . "includes/classes/fileSimec.class.inc";
    $file = new FilesSimec();
    $arqid = $_REQUEST['arqid'];
    $arquivo = $file->getDownloadArquivo($arqid);
    echo"<script>window.location.href = '".$_SERVER['HTTP_REFERER']."';</script>";
    exit;
}

// Condição consumida via ajax
if($_REQUEST['vgcidAjax']){
    header('content-type: text/html; charset=ISO-8859-1');
    
    $sqlRepactuacao = "select distinct
    					case when rpc.arqid is null then
	                        '<center>
	                        <img class=\"middle link\" title=\"Alterar Repactuação\" alt=\"Alterar Vigência\"    onclick=\"editarRepactuacao(\'' || rpc.rpcid || '\')\"   src=\"../imagens/editar_nome.gif\" />
	                        <img class=\"middle link\" title=\"Excluir Repactuação\" alt=\"Excluir Repactuação\" onclick=\"excluirRepactuacao(\'' || rpc.rpcid || '\' )\" src=\"../imagens/excluir.gif\" />
	                        </center>'
	                    else
	                        '<center>
	                        <img class=\"middle link\" title=\"Visualizar Arquivo\"  alt=\"Visualizar Arquivo\"  onclick=\"mostrarAquivo(\'' || rpc.arqid || '\')\"       src=\"../imagens/anexo.gif\" />
	                        <img class=\"middle link\" title=\"Alterar Repactuação\" alt=\"Alterar Vigência\"    onclick=\"editarRepactuacao(\'' || rpc.rpcid || '\')\"   src=\"../imagens/editar_nome.gif\" />
	                        <img class=\"middle link\" title=\"Excluir Repactuação\" alt=\"Excluir Repactuação\" onclick=\"excluirRepactuacao(\'' || rpc.rpcid || '\' )\" src=\"../imagens/excluir.gif\" />
	                        </center>'
	                    end  as acao,
                        rpc.rpcvalor
                        , to_char(rpc.rpcdtinicio,'dd/MM/YYYY') as rpcdtinicio
                        , to_char(rpc.rpcdtfim,'dd/MM/YYYY') as rpcdtfim
                        , rpc.rpcdocumento
                        , tpr.tprdsc
                       from fabrica.repactuacaocontrato rpc
                       inner join fabrica.tiporepactuacao tpr on tpr.tprid = rpc.tprid
                       where rpc.vgcid = " . $_REQUEST['vgcidAjax'];

    explodeSubLista( $sqlRepactuacao );
    exit;
}

function explodeSubLista( $sql ){

    global $db;
    
    $cabecalho     = array("Ações","Valor de Ponto de Função Repactuado","Data de Início","Data de Término", "Nº Documento", "Tipo");
    $tamanho       = array("5%","10%", "10%", "10%", "10%", "10%", "15%", "30%");
    $alinhamento   = array( "center", "center", "center", "center", "center", "center" );
    $db->monta_lista($sql,$cabecalho,100,5,'N','center',$par2,"",$tamanho,$alinhamento);
}

include APPRAIZ."fabrica/classes/Contrato.class.inc";

$ctrid                  = $_REQUEST['ctrid'];
$vgcid                  = $_REQUEST['vgcid'];
$tpvid                  = $_REQUEST['tpvid'];
$ctrnumero              = $_REQUEST['ctrnumero'];
$ctrobjeto              = $_REQUEST['ctrobjeto'];
$entid                  = $_REQUEST['entid'];
$entnumcpfcnpj          = $_REQUEST['entnumcpfcnpj'];
$entnome                = $_REQUEST['entnome'];
$ctrcontagem            = $_REQUEST['ctrcontagem'];
$ctrtipoempresaitem     = $_REQUEST['ctrtipoempresaitem'];
$gestorcpf              = $_REQUEST['gestorcpf'];
$fiscaladmcpf           = $_REQUEST['fiscaladmcpf'];
$prepostocpf            = $_REQUEST['prepostocpf'];
$ctridvinculado         = $_REQUEST['ctridvinculado'];
$vgcdtinicio            = $_REQUEST['vgcdtinicio'];
$vgcdtfim               = $_REQUEST['vgcdtfim'];
$vgcdtassinatura        = $_REQUEST['vgcdtassinatura'];
$situacao               = $_REQUEST['situacao'];
$vgcvolumepfcontratado  = $_REQUEST['vgcvolumepfcontratado'] ? $_REQUEST['vgcvolumepfcontratado'] : 0;
$vgcvolumepfutilizado   = empty($_REQUEST['vgcvolumepfutilizado']) ? 0 : $_REQUEST['vgcvolumepfutilizado'];
$vgcvalorpfcontrato     = $_REQUEST['vgcvalorpfcontrato'] ? $_REQUEST['vgcvalorpfcontrato'] : 0;
$fiscal                 = $_REQUEST['fiscal'] && $_REQUEST['fiscal'] != "" ? $_REQUEST['fiscal'] : null;
$requisicao             = $_REQUEST['requisicao']; 

$novaVigencia           = empty($ctrid)  && empty($vgcid) ? true : false;
$editarVigencia         = false;

// Inserção/Edição/Exclusão
if( $requisicao ){

    //Exclui vigência
    if( $requisicao == 'excluirVigencia'){
        
        try {
            $sql     = sprintf("update fabrica.vigenciacontrato set vgcstatus='I' where vgcid = %d",(int)$vgcid);
            $vgcid   = null;
            $msg     = "Vigência excluída com sucesso!";
            
            if( !$db->executar($sql) ){
                throw new Exception( "Não foi possível excluir a vigência." );
            }else{
                $db->commit($sql);
            }
            
        } catch (Exception $e) {
            
            $db->rollback();
            $msg = $e->getMessage();
        }

    // Nova vigência
    }elseif( $requisicao == 'novaVigencia' ){
        
        $novaVigencia = true;
	
    
    // Atualiza vigência
    }elseif( $requisicao == 'editarContrato' ){
    	
    	//Insere itens de contrato  
	    if( $db->pegaUm("SELECT count(1) FROM fabrica.metricaitemcontrato WHERE ctrid = ".$ctrid) > 0 ){
	    	$db->executar("DELETE FROM fabrica.metricaitemcontrato WHERE ctrid = ".$ctrid);
	    }
		
        if( $_REQUEST['item'] ){
        	for($i=0; $i<count($_REQUEST['item']); $i++){
        		
        		$item = $_REQUEST['item'][$i];

            	if($ctrid && $item){
                	$db->executar("INSERT INTO fabrica.metricaitemcontrato(ctrid,mtiid) VALUES(".$ctrid.", '".$item."')");
                }
            }
        }
		
        //atualiza vinculo com empresa do item 2 
		$updateContrato = "UPDATE fabrica.contrato set ctridvinculado = ".($ctridvinculado ? $ctridvinculado : 'null'). " where ctrid = $ctrid";
		$db->executar($updateContrato);		
		$db->commit();
		 
		echo '<script>
				alert("Operação realizada com sucesso!");
				location.href="fabrica.php?modulo=principal/cadContrato&acao=A&ctrid='.$ctrid.'";
				</script>';
		die();
        
				
    // Atualiza vigência
    }elseif( $requisicao == 'editarVigencia' ){
        
    	//var_dump($novaVigencia);
    	
        if( !empty($vgcid) ){
        	
        	// verifica se existem alguma vigência em andamento
        	$executarUpdate = true;
        	
        	if($situacao == 1){

        		$verificaVigenciaEmAndamento = "SELECT COUNT(vgcid) as contador 
        										FROM fabrica.vigenciacontrato
        										WHERE vgcstatus = 'A'
        										AND tsvid = 1
        										AND ctrid =  " . $ctrid ."
        										AND vgcid <> " . $vgcid ;
        		 
        		$msg = "Já existe uma vigência cadastrada para essa situação";
 				
        		$resultado =  $db->pegaLinha( $verificaVigenciaEmAndamento );
        		
        		if( $resultado['contador'] != 0 ){
        			$executarUpdate = false;
        		} 
        	}
        	
        	if( $executarUpdate ){
        		
        		//Insere itens de vigencia  
			    if( $db->pegaUm("SELECT count(1) FROM fabrica.vigenciacontratometricaitem WHERE vgcid = ".$vgcid) > 0 ){
			    	$db->executar("DELETE FROM fabrica.vigenciacontratometricaitem WHERE vgcid = ".$vgcid);
			    }
				 
			    $_REQUEST['item'] = $_REQUEST['xvitem'];
		        if( $_REQUEST['item'] ){
		        	for($i=0; $i<count($_REQUEST['item']); $i++){
		        		
		        		$item = $_REQUEST['item'][$i];
		
		        		$volume = $_REQUEST['volume'][$i];
		        		if($volume) $volume = str_replace(',', '.', str_replace('.', '', $volume)); 
		        		
		            	$valor = $_REQUEST['valor'][$i];
		            	if($valor) $valor = str_replace(',', '.', str_replace('.', '', $valor));
		
		            	if($ctrid && $item && $volume && $valor){
		                	$db->executar("INSERT INTO fabrica.vigenciacontratometricaitem(vgcid,mtiid,vcmvolumecontratado,vcmvalorcontratado) VALUES(".$vgcid.", '".$item."', '".$volume."', '".$valor."')");
		                }
		            }
		        }
		        
        		//Insere fiscais  
                if( $db->pegaUm("SELECT count(1) FROM fabrica.fiscalvigencia WHERE vgcid = ".$vgcid) > 0 ){
                	$db->executar("DELETE FROM fabrica.fiscalvigencia WHERE vgcid = ".$vgcid);
                }
                    
                if( $fiscal ){
                   for($i=0; $i<count($fiscal); $i++){
                       if($vgcid && $fiscal[$i]){
                           $db->executar("INSERT INTO fabrica.fiscalvigencia(vgcid,usucpf) VALUES(".$vgcid.", '".$fiscal[$i]."')");
                	   }
                   }
                }

				$updateContrato = "UPDATE fabrica.contrato
	                                   SET gestorcpf = '".$gestorcpf."'
	                                   , fiscaladmcpf = '".$fiscaladmcpf."'
	                                   , prepostocpf = '".$prepostocpf."'
	                                WHERE ctrid = " . $ctrid;
				$db->executar($updateContrato);
        		
	            //atualiza vigencia
	            $updateVigencia = "UPDATE fabrica.vigenciacontrato
	                                   SET tsvid=".$situacao."
	                                   , gestorcpf = '".$gestorcpf."'
	                                   , fiscaladmcpf = '".$fiscaladmcpf."'
	                                   , prepostocpf = '".$prepostocpf."'
	                                   , vgcdtassinatura = '". formata_data_sql($vgcdtassinatura) ."'
	                                   , vgcdtinicio = '". formata_data_sql($vgcdtinicio) ."'
	                                   , vgcdtfim = '". formata_data_sql($vgcdtfim) ."'
	                                   , vgcvolumepfcontratado = ".str_replace(',', '.', str_replace('.','', $vgcvolumepfcontratado))."
	                                   , vgcvolumepfutilizado  = ".str_replace(',', '.', str_replace('.','', $vgcvolumepfutilizado))."
	                                   , vgcvalorpfcontrato    = ".str_replace(',', '.', str_replace('.','', $vgcvalorpfcontrato)) ."
	                                WHERE vgcid = " . $vgcid;
	            $msg = "Vigência atualizada com sucesso!";
	            
	            try{
	                if( !$db->executar($updateVigencia) ){
	                    throw new Exception( "Não foi possível atualizar a vigência." );
	                }else{
	                    $db->commit( $updateVigencia );
	                }
	            } catch (Exception $e) {
	            
	                $db->rollback();
	                $msg = $e->getMessage();
	            }
        	}
        }
        
    }elseif( $requisicao == 'editar' ){

        $sqlConsutlta = "SELECT 
                            ctrid, vgcdtinicio, vgcdtfim, vgcvolumepfcontratado,
                            tsvid, vgcdtassinatura, vgcvolumepfutilizado, vgcvalorpfcontrato,
                            gestorcpf, fiscaladmcpf, prepostocpf
                         FROM fabrica.vigenciacontrato WHERE vgcid = ". $vgcid;

        $resEditar =  $db->pegaLinha( $sqlConsutlta );

        if( $resEditar ){
            $editarVigencia         = true;
            $ctrid                  = $resEditar['ctrid'];
            $vgcdtinicio            = formata_data( $resEditar['vgcdtinicio'] );
            $vgcdtfim               = formata_data( $resEditar['vgcdtfim'] );
            $vgcvolumepfcontratado  = str_replace( '.', ',', $resEditar['vgcvolumepfcontratado'] );
            $situacao               = $resEditar['tsvid'];
            $vgcdtassinatura        = formata_data( $resEditar['vgcdtassinatura']);
            $vgcvolumepfutilizado   = str_replace('.', ',', $resEditar['vgcvolumepfutilizado']);
            $vgcvalorpfcontrato     = str_replace('.', ',', $resEditar['vgcvalorpfcontrato']);
            $gestorcpf              = $resEditar['gestorcpf'];
			$fiscaladmcpf           = $resEditar['fiscaladmcpf'];
			$prepostocpf            = $resEditar['prepostocpf'];


        }

    }elseif( $requisicao == 'salvar' ){

        $insContrato = "INSERT INTO fabrica.contrato(ctrnumero, ctrobjeto, ctrstatus, entidcontratado, ctrcontagem, ctrtipoempresaitem, gestorcpf, fiscaladmcpf, prepostocpf, ctridvinculado)";
        $insContrato.= " VALUES (";
        $insContrato.= "'" . $ctrnumero . "'";
        $insContrato.= ", '" . $ctrobjeto . "'";
        $insContrato.= ", 'A'";
        $insContrato.= ", " . $entid;
        $insContrato.= ", ";
        $insContrato.= $ctrcontagem == 0 ? 'false' : 'true';
        $insContrato.= ", " . $ctrtipoempresaitem;
        $insContrato.= ", '" . $gestorcpf . "'";
		$insContrato.= ", '" . $fiscaladmcpf . "'";
		$insContrato.= ", '" . $prepostocpf . "'";
        $insContrato.= ", " . ($ctridvinculado ? $ctridvinculado : 'null');
        $insContrato.= ") returning ctrid";
        
        //var_dump( $insContrato );
        
        try {
        	
            $tpvid = empty($ctrid) ? 1 : 2;
            $ctrid = empty($ctrid) ? $db->pegaUm( $insContrato ) : $ctrid;
            
            if( !$ctrid ){
                throw new Exception( "Não foi possível criar novo contrato." );
            }else{
                
            	
            	// verifica se existem alguma vigência em andamento
            	$executarInsert = true;
            	/* 
            	if( $situacao == 1 ){
            		$verificaVigenciaEmAndamento = "SELECT COUNT(vgcid) FROM fabrica.vigenciacontrato
        										WHERE tsvid = 1
        										AND ctrid = ".$ctrid;
            		 
            		$msg = "Já existe uma vigência cadastrada para essa situação";
            	
            		if($db->executar($verificaVigenciaEmAndamento) != 0){
            			$executarInsert = false;
            		}
            	}
            	
            	var_dump( $executarInsert );exit;
				*/
            	
				/*
            	if( $situacao == 1 ){
            		if ( $db->executar( " UPDATE fabrica.vigenciacontrato SET tsvid = 2 WHERE ctrid = " . $ctrid ) === false ){
            			$executarInsert = false;
            		}
            	}
            	*/
            	
            	//insere situação do contrato
            	$sql = "INSERT INTO fabrica.contratosituacao(tscid, ctrid, ctsstatus, ctsdtinclusao, usucpfcadastrador)
    					VALUES (1, $ctrid, 'A', now(), '".$_SESSION['usucpf']."')";
    			$db->executar($sql);
            	
            	
	            //Insere itens de contrato  
			    if( $db->pegaUm("SELECT count(1) FROM fabrica.metricaitemcontrato WHERE ctrid = ".$ctrid) > 0 ){
			    	$db->executar("DELETE FROM fabrica.metricaitemcontrato WHERE ctrid = ".$ctrid);
			    }
				
		        if( $_REQUEST['item'] ){
		        	for($i=0; $i<count($_REQUEST['item']); $i++){
		        		
		        		$item = $_REQUEST['item'][$i];
		
		            	if($ctrid && $item){
		                	$db->executar("INSERT INTO fabrica.metricaitemcontrato(ctrid,mtiid) VALUES(".$ctrid.", '".$item."')");
		                }
		            }
		        }
            	

	            if( $executarInsert ){

	                //Insere vigência
	                $InsVigencia = "INSERT INTO fabrica.vigenciacontrato(ctrid, vgcdtinicio, vgcdtfim, vgcvolumepfcontratado";
	                $InsVigencia.= ",vgcstatus, tsvid, vgcdtassinatura, vgcvolumepfutilizado, vgcvalorpfcontrato, gestorcpf, fiscaladmcpf, prepostocpf, tpvid)";
	                $InsVigencia.= " VALUES (";
	                $InsVigencia.= $ctrid;
	                $InsVigencia.= ", '" . formata_data_sql($vgcdtinicio) . "'";
	                $InsVigencia.= ", '" . formata_data_sql($vgcdtfim) . "'";
	                $InsVigencia.= ", " . str_replace(",", ".", str_replace(".","",$vgcvolumepfcontratado)) ;
	                $InsVigencia.= ", 'A'";
	                $InsVigencia.= ", " . $situacao;
	                $InsVigencia.= ", '" . formata_data_sql($vgcdtassinatura) . "'";
	                $InsVigencia.= ", " . $vgcvolumepfutilizado;
	                $InsVigencia.= ", " . str_replace(",", ".", str_replace(".","",$vgcvalorpfcontrato)) ;
	                $InsVigencia.= ", '" . $gestorcpf . "'";
					$InsVigencia.= ", '" . $fiscaladmcpf . "'";
					$InsVigencia.= ", '" . $prepostocpf . "'";
	                $InsVigencia.= ", "  . $tpvid;
	                $InsVigencia.= ") RETURNING vgcid";
	                
	                $vgcid = $db->pegaUm( $InsVigencia );
	    
	                if( !$vgcid ){
	                    throw new Exception( "Não foi possível criar nova vigência." );
	                }else{
	    
	                    //Insere fiscais  
	                    if( $db->pegaUm("SELECT count(1) FROM fabrica.fiscalvigencia WHERE vgcid = ".$vgcid) > 0 ){
	                        $db->executar("DELETE FROM fabrica.fiscalvigencia WHERE vgcid = ".$vgcid);
	                    }
	                    
	                    if( $fiscal ){
	                        for($i=0; $i<count($fiscal); $i++){
	                            if($vgcid && $fiscal[$i]){
	                                $db->executar("INSERT INTO fabrica.fiscalvigencia(vgcid,usucpf) VALUES(".$vgcid.", '".$fiscal[$i]."')");
	                			}
	                		}
	                	}
	                	
	                	
		                //Insere itens de vigencia  
					    if( $db->pegaUm("SELECT count(1) FROM fabrica.vigenciacontratometricaitem WHERE vgcid = ".$vgcid) > 0 ){
					    	$db->executar("DELETE FROM fabrica.vigenciacontratometricaitem WHERE vgcid = ".$vgcid);
					    }
						 
				        if( $_REQUEST['item'] ){
				        	for($i=0; $i<count($_REQUEST['item']); $i++){
				        		
				        		$item = $_REQUEST['item'][$i];
				
				        		$volume = $_REQUEST['volume'][$i];
				        		if($volume) $volume = str_replace(',', '.', str_replace('.', '', $volume)); 
				        		
				            	$valor = $_REQUEST['valor'][$i];
				            	if($valor) $valor = str_replace(',', '.', str_replace('.', '', $valor));
				
				            	if($ctrid && $item && $volume && $valor){
				                	$db->executar("INSERT INTO fabrica.vigenciacontratometricaitem(vgcid,mtiid,vcmvolumecontratado,vcmvalorcontratado) VALUES(".$vgcid.", '".$item."', '".$volume."', '".$valor."')");
				                }
				            }
				        }
	                	
	                	
	                }
	            }
            }
                    
            $db->commit();
            $msg = "Operação realizada com sucesso!";
            $novaVigencia = false;
            
        } catch ( Exception $e ) {
    
            $db->rollback();
            $msg  = $e->getMessage();
        }
    }
    
    //Atualiza PF Utilizado
    $vlPfCalculado = $db->pegaUm( "SELECT fabrica.fn_calculo_pf_utilizado()");
}

if($ctrid){
	
	$sqlEntidade = "SELECT a.entid, a.entnome, a.entnumcpfcnpj, b.ctrnumero, b.ctrobjeto, b.ctrcontagem, b.ctrtipoempresaitem, b.ctridvinculado";
	$sqlEntidade.= " FROM entidade.entidade a";
	$sqlEntidade.= " INNER JOIN fabrica.contrato b ON b.entidcontratado = a.entid AND b.ctrstatus = 'A' ";
	$sqlEntidade.= " WHERE b.ctrid = " . $ctrid . " LIMIT 1";
	
	$resultado = $db->pegaLinha( $sqlEntidade );
	
	if( !empty($resultado) ){
	    $entid              = $resultado['entid'];
	    $entnome            = $resultado['entnome'];
	    $entnumcpfcnpj      = $resultado['entnumcpfcnpj'];
	    $ctrobjeto          = $resultado['ctrobjeto'];
	    $ctrnumero          = $resultado['ctrnumero'];
	    $ctrcontagem        = $resultado['ctrcontagem'];
	    $ctrtipoempresaitem = $resultado['ctrtipoempresaitem'];
	    $ctridvinculado		= $resultado['ctridvinculado'];
	}
}

// monta cabeçalho 
include APPRAIZ . 'includes/cabecalho.inc';
print '<br/>';

// carregando o menu
$menu[0] = array("descricao" => "Listar Contratos", "link"=> "fabrica.php?modulo=principal/listarContrato&acao=A");
$menu[1] = array("descricao" => "Cadastrar Contrato", "link"=> "fabrica.php?modulo=principal/cadContrato&acao=A");
if($ctrid){
	$menu[2] = array("descricao" => "Penalidades do Contrato", "link"=> "fabrica.php?modulo=principal/penalidadeContrato&acao=A&ctrid=$ctrid");
}
echo montarAbasArray($menu, "fabrica.php?modulo=principal/cadContrato&acao=A");

$titulo = "Cadastrar Contrato";
monta_titulo( $titulo, obrigatorio().'Indica campo obrigatório.' );

?>
<script language="javascript" type="text/javascript" src="../includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-1.4.2.min.js"></script>
<script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/dateFunctions.js"></script>
<script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>
<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>
<style>
	.TituloTabela{background-color: #C5C5C5;font-weight:bold}
	.center{text-align:center}
	.link{cursor: pointer;}
	.middle{vertical-align: middle;}
</style>
<script>
	
	$(document).ready(function() {
        $("[name=ctrtipoempresaitem]").addClass('obrigatorio');

        $('#entnumcpfcnpj').blur(function() {

            if( $(this).val() != ''){
                $params = { entnumcpfcnpj : $('#entnumcpfcnpj').val() };
                $.get('fabrica.php?modulo=principal/cadContrato&acao=A&requisicaocnpj=1', $params, function(data){
                	if(data.status == true){
                		$('#entid').val( data.entid );
                		$('#entnome').val( data.entnome );
                	}else{
                    	alert( "CNPJ não localizado." )
                	}
                }, 'json' );
            }
        });
        
		<?php if(empty($vgcid)) {?>
        $('#situacao option[value=1]').attr({selected : "selected"});
        <?php }?>
        
        
	});

	function limparCampos(){
		$('input:not(:hidden,:button,[readonly=""],[readonly="readonly"]),textarea,select').val("");
		$("[name=entnome],[name=entnumcpfcnpj],[name=ctrnumero],[name=vgcvolumepfcontratado],[name=vgcvolumepfcontratado],[name=vgcvalorpfcontrato]").val("");
	}

	function selecionaTudo(id)
	{
		$('#'+id+' option').each(function(){
			//console.log('fiscal: ' + id);
			$(this).attr('selected', 'selected');
		});
	}

	function salvarContrato()
	{
		validaFormulario();
	}

	function checarDatas(){
		
		if(document.getElementById('vgcdtinicio').value && document.getElementById('vgcdtfim').value) {

			var data_1 = document.getElementById('vgcdtinicio').value;
			var data_2 = document.getElementById('vgcdtfim').value;

			var compara01 = parseInt(data_1.split("/")[2].toString() + data_1.split("/")[1].toString() + data_1.split("/")[0].toString());
			var compara02 = parseInt(data_2.split("/")[2].toString() + data_2.split("/")[1].toString() + data_2.split("/")[0].toString());

			if (compara01 > compara02) {
				alert('Data de início não pode ser maior que a data de término');
			}else{
				var ctrid = $('[name=ctrid]').val();
				var vgcid = $('[name=vgcid]').val();
				
				$.ajax({
					   type: "get",
					   url:  'fabrica.php?modulo=principal/cadContrato&acao=A',
					   data: "periodovigenciavalido=ok&vgcdtinicio=" + data_1 +"&vgcdtfim=" + data_2 + "&ctrid=" + ctrid + "&vgcid=" + vgcid,
					   dataType: 'json',
					   success: function(msg){
					     if( msg.status == 'true' ){
					    	// console.log('envia...');
					    	$('#form_cad_contrato').submit();
					     }else{
					     	alert('Já existe vigência cadastrada para esse perí­odo.');
							return false;
					     }
					   }
					 });
			}
		}	

		return false;
	}
	
	function validaFormulario()
	{
		// Seleciona todos os fiscais, se houver algum incluído
		selecionaTudo('fiscal');
		
		if(!document.form_cad_contrato.ctrid.value){
			selecionaTudo('metrica');
		}
		
		var erro = 0;

		$("[class~=obrigatorio]").each(function() { 
			if(!this.value){
				erro = 1;
				alert('Favor preencher todos os campos obrigatórios!');
				this.focus();
				return false;
			}
		});

		if(erro == 0){
			if( !checarDatas() ){
				return false;
			}
		}else{
			return false;
		}
		
	}
	
	function editarContrato()
	{
		if(document.form_cad_contrato.ctrtipoempresaitem.value=='1'){
			if(document.form_cad_contrato.ctridvinculado.value==''){
				alert('Favor preencher todos os campos obrigatórios!');
				document.form_cad_contrato.ctridvinculado.focus();
				return false;
			}
		}
		
		selecionaTudo('metrica');
		document.form_cad_contrato.submit();

	}
		
	function carregando()
	{
		return "<center><img src=\"../imagens/wait.gif\" class=\"middle\" />Aguarde...Carregando!</center>";
	}
	
	function excluirElemento( id )
	{
		$('#' + id).remove();
		
	}
	
	function validaEmpresaContagemPF()
	{
		var ctrid       = $('[name=ctrid]').val();
		var ctrcontagem = $('[name=ctrcontagem]:checked').val();
		
		if(ctrcontagem == "0"){
			$('#form_cad_contrato').submit();
		}else{
			return $.ajax({
			   type: "POST",
			   url: window.location,
			   data: "requisicaoAjax=verificaEmpresaContagemPF&ctrid=" + ctrid,
			   success: function(msg){
			     if(msg.search("1") >= 0){
			     	alert('Já existe uma empresa responsável atribuída!');
					return false;
			     }else{
			     	$('#form_cad_contrato').submit()
			     }
			   }
			 });
		}
	}

	function excluirVigencia(vgcid,ctrid)
	{
		if(confirm('Deseja realmente excluir a vigência?')){
			window.location.href = 'fabrica.php?modulo=principal/cadContrato&acao=A&requisicao=excluirVigencia&vgcid=' + vgcid +'&ctrid='+ctrid;
		}
	}

	function editarVigencia(vgcid)
	{
		window.location.href = 'fabrica.php?modulo=principal/cadContrato&acao=A&requisicao=editar&vgcid=' + vgcid;
	}

	function mostrarAquivo(arqid)
	{
		if(arqid){
			window.open('fabrica.php?modulo=principal/cadContrato&acao=A&download=S&arqid='+arqid,'Download','scrollbars=yes,height=350,width=520,status=no,toolbar=no,menubar=no,location=no');
		}else{
			alert('Não é possível carregar janela de repactuação!');
		}
	}
	
	function adicionarRepactuacao(vgcid)
	{
		if(vgcid){
			window.open('fabrica.php?modulo=principal/popup/entidadeContrato&acao=A&nova=1&tela=repactuacao&vgcid='+vgcid,'Repactuação','scrollbars=yes,height=350,width=520,status=no,toolbar=no,menubar=no,location=no');
		}else{
			alert('Não é possível carregar janela de repactuação!');
		}
	}
	
	function editarRepactuacao(rpcid){
		if(rpcid){
			window.open('fabrica.php?modulo=principal/popup/entidadeContrato&acao=A&tela=repactuacao&rpcid='+rpcid,'Repactuação','scrollbars=yes,height=350,width=520,status=no,toolbar=no,menubar=no,location=no');
		}else{
			alert('Não é possível carregar janela de repactuação!');
		}
	}

	function novaVigencia( ctrid ){
		window.location.href = 'fabrica.php?modulo=principal/cadContrato&acao=A&requisicao=novaVigencia&ctrid=' + ctrid;
	}

	function montaSubLista( vgcid )
	{
		td 	       = document.getElementById('td_'+vgcid);
		img_mais   = document.getElementById('img_mais_'+vgcid);
		img_menos  = document.getElementById('img_menos_'+vgcid);


		// Faz uma requisição ajax, via POST
		$.ajax({type: "POST",
		   		url: "fabrica.php?modulo=principal/cadContrato&acao=A",
		   		data: "vgcidAjax="+vgcid,
		   		success: function(msg){

					td.style.display = '';
					//td.style.padding = '5px';
					img_mais.style.display = 'none';
					img_menos.style.display = '';
					td.innerHTML = msg;
		   		}
		 });
	}

	function excluirRepactuacao( rpcid )
	{
		if(confirm('Deseja realmente excluir a Repactuação?')){
			
    		$.ajax({type: "POST",
    		   		url: "fabrica.php?modulo=principal/cadContrato&acao=A",
    		   		data: "exclusaorepactuacao=S&rpcid="+rpcid,
    		   		success: function(data){
    			   		if( data ){
    			   		    alert('Registro excluído com sucesso.');
    			   		    window.location.href = window.location;
    			   		}else{
    				   		console.log( data );
    			   		}	
    		   		}
    		 });
		}
	}

	function desmontaSubLista( vgcid )
	{
		td 	       = document.getElementById('td_'+vgcid);
		img_mais   = document.getElementById('img_mais_'+vgcid);
		img_menos  = document.getElementById('img_menos_'+vgcid);

		td.innerHTML = '';
		td.style.display = 'none';
		img_mais.style.display = '';
		img_menos.style.display = 'none';

	}
	
	function filtraEmpresa()
	{
		tr_empresa = document.getElementById('tr_empresa');

		if(document.getElementById('ctrtipoempresaitem').value == 1){
			tr_empresa.style.display = '';
			$("[name=ctridvinculado]").addClass('obrigatorio');
		}else{
			tr_empresa.style.display = 'none';
			$("[name=ctridvinculado]").removeClass('obrigatorio');
		}

	}
	
	function mudaItem(id){
		document.getElementById("vitem_"+id).value = document.getElementById("item_"+id).value; 
	}
	
		
</script>
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
    <tr>
        <td>
            <form name="form_cad_contrato" id="form_cad_contrato" method="post" action="" >
                <fieldset width='95%'>
                    <legend>Contrato</legend>
                	<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
                		<tr>
                			<td class="SubtituloDireita" width="25%">Número do Contrato:</td>
                			<td><?=campo_texto("ctrnumero","S",($ctrnumero ? "N" : "S"),"Número do Contrato","20","20","","") ?></td>
                		</tr>
                		<tr>
                			<td class="SubtituloDireita">CNPJ:</td>
                			<td>
                			<?php  echo campo_texto('entnumcpfcnpj', 'S', ($entnumcpfcnpj ? "N" : "S"), 'CNPJ', 22, 19, "##.###.###/####-##", "", '', '', 0, 'id="entnumcpfcnpj"', '', ( ($entnumcpfcnpj) ? $entnumcpfcnpj :''), '' ); ?>
                			</td>
                		</tr>
                		<tr>
                			<td class="SubtituloDireita">Nome da Empresa:</td>
                			<td>
            				<?php
            				if($entid){ 
            					require_once APPRAIZ . "includes/classes/entidades.class.inc";
            					$entidade = new Entidades();
            					$entidade->carregarPorEntid($entid);
            					$entnome = $entidade->getEntnome();
            				}
            				?>
                			<?=campo_texto("entnome", "S", "N", "Nome empresa", "60", "254","","","","","",'id=entnome','',$entnome) ?>
                			<input type="hidden" name="entid" id="entid" value="<?php echo($entid); ?>" />
                			</td>
                		</tr>
                        <tr>
                            <td class="SubtituloDireita" >Tipo da Empresa</td>
                            <td>
                                <?
                                $ItemTipoEmpresa[] = 'Selecione...';
                                $ItemTipoEmpresa[1] = 'Empresa do Item 1';
                                $ItemTipoEmpresa[2] = 'Empresa do Item 2';
                                
                                if( empty($ctrid) ) {
	                                ?>
	                                <select name="ctrtipoempresaitem" id="ctrtipoempresaitem" onchange="filtraEmpresa();" >
	                                    <?php foreach ($ItemTipoEmpresa as $key => $valor) { ?>
	                                            <option value="<? echo $key ?>"  <? if ($ctrtipoempresaitem == $key or $ctrtipoempresaitem == $key) {
	                                                echo ' selected = true ';
	                                            } else {
	                                                echo 'Selecione...';
	                                            } ?> ><? echo $valor ?></option> 
	                                    <? } ?>
	                                </select>
                                <?php }else{ ?>
	                                <select name="_ctrtipoempresaitem" id="_ctrtipoempresaitem" <?php echo !empty($ctrid) ? " disabled='disabled' " : "" ?>>
	                                    <?php foreach ($ItemTipoEmpresa as $key => $valor) { ?>
	                                            <option value="<? echo $key ?>"  <? if ($ctrtipoempresaitem == $key or $ctrtipoempresaitem == $key) {
	                                                echo ' selected = true ';
	                                            } else {
	                                                echo 'Selecione...';
	                                            } ?> ><? echo $valor ?></option> 
	                                    <? } ?>
	                                </select>
	                                <input type="hidden" name="ctrtipoempresaitem" id="ctrtipoempresaitem" value="<?php echo($ctrtipoempresaitem); ?>" />
								<? } ?>
                            </td>
                        </tr>
                		<tr>
                			<td class="SubtituloDireita">Objeto da Contratação:</td>
                			<td><?=campo_textarea("ctrobjeto","S", (!empty($ctrid) ? "N" : "S") ,"Observações do Projeto","80","3",false) ?></td>
                		</tr>
                		<tr>
                			<td class="SubtituloDireita">Responsável por Contar o Volume de Pontos por Função?</td>
                			<td>
                				<input type="radio" <?php echo $ctrcontagem == "t" ? "checked='checked'" : "" ?><?php echo !empty($ctrid) ? " readonly='readonly' " : "" ?> name="ctrcontagem" value="1" />Sim 
                				<input type="radio" <?php echo !$ctrcontagem || $ctrcontagem == "f" ? "checked='checked'" : "" ?><?php echo !empty($ctrid) ? " readonly='readonly' " : "" ?> name="ctrcontagem" value="0" />Não
                			</td>
                		</tr>
                		<tr>
                			<td class="SubtituloDireita">Itens do Contrato:</td>
                			<td>
                			<?php
                				$flagEditarContrato = true;
                				
                				if($requisicao == 'editar' || $requisicao == 'novaVigencia'){
                					$flagEditarContrato = false;
                				}
                				
                				if($ctrid){
	                				$sql_carregados = "SELECT
		                										mc.mtiid
		                									FROM
		                										fabrica.metricaitemcontrato mc
		                									INNER JOIN
		                										fabrica.metricaitem mi ON mi.mtiid = mc.mtiid
		                									inner join fabrica.metrica mt on mt.mtcid = mi.mtcid
		                									WHERE
		                										mi.mtistatus = 'A' and mc.ctrid = ".$ctrid;
		                			$itens = $db->carregar( $sql_carregados );
		                			$totalItens = count($itens)-1;
                				}
	                			if(!$totalItens) $totalItens = 0;
	                			
	                			$totalItens = $_REQUEST['totalItens'] ? $_REQUEST['totalItens'] : $totalItens;
	                			
                			?>
                				<?if($flagEditarContrato){ ?>
                					Inserir Itens <img src="../imagens/gif_inclui.gif" border="0" align="absmiddle" style="cursor:pointer;" onclick="location.href='fabrica.php?modulo=principal/cadContrato&acao=A&ctrid=<?=$ctrid?>&totalItens=<?=$totalItens+1?>';">
                				<?}?>
                				<table width="50%" class="listagem" id="tabelaItens">
                					<tr>
                						<td>
                							Item
                						</td>
                					</tr>
                					<?
	                				for($i=0; $i<=$totalItens; $i++){
	                					?>
	                					<tr>
	                						<td>
	                							 <?php
						                                $sql_combo = "select 
						                								mi.mtiid as codigo,
						                								mt.mtcsigla ||' - '|| mi.mtinome as descricao
						                							from 
						                								fabrica.metricaitem mi
						                							inner join fabrica.metrica mt on mt.mtcid = mi.mtcid 
						                							where 
						                								mi.mtistatus = 'A'";
						                                $db->monta_combo("item[]",$sql_combo,($flagEditarContrato ? 'S':'N'),"-- Selecione --",'mudaItem('.$i.');mudaItem2','','', '', ($flagEditarContrato ? 'S':'N'), 'item_'.$i,'',$itens[$i]['mtiid']);
					                            ?>
	                						</td>
	                					</tr>
	                				<?}?>
                				</table>
                			</td>
                		</tr>
                		<tr id="tr_empresa" style="display: <?if(!$ctrtipoempresaitem || $ctrtipoempresaitem == '2') echo 'none';?>">
                            <td class="SubtituloDireita">Vinculação Empresa do Item 2</td>
                            <td>
                            <?php
	                                $sql_combo = "select
	                                        c.ctrid as codigo,
	                                        e.entnome as descricao
	                                from
	                                        fabrica.contrato c
	                                inner join
	                                        entidade.entidade e ON e.entid = c.entidcontratado
	                                where
	                                        c.ctrstatus='A' and c.ctrtipoempresaitem = 2";
	                                $db->monta_combo("ctridvinculado",$sql_combo,($flagEditarContrato ? 'S':'N'),"-- Selecione --",'','','', '', ($flagEditarContrato ? 'S':'N'), 'ctridvinculado','',$ctridvinculado);
                            ?>
                            </td>
                        </tr>
                		<?php if( !empty($ctrid) && $novaVigencia == false ) { ?>
                		<tr>
                			<td class="SubtituloDireita"></td>
                			<td>
                				<?if($flagEditarContrato){?>
	                				<input type="hidden" name="requisicao" value="editarContrato" />
	            				    <input type="button" onclick="editarContrato()" value="Salvar" >
            				    <?}?>
                				<input type="button" value="Nova Vigência" onclick="novaVigencia( <?php echo($ctrid); ?>)" />
                			</td>
                		</tr>
                		<?php } ?>		
                	</table>
                </fieldset>
                <?php if( $novaVigencia || $editarVigencia ){ ?>
                <fieldset>
                    <legend>Vigência</legend>
                	<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
                		<tr>
                			<td class="SubtituloDireita" width="25%">Data de Assinatura:</td>
                			<td><?=campo_data2("vgcdtassinatura","S","S","Data de Assinatura","##/##/####"); ?></td>
                		</tr>
                		<tr>
                			<td class="SubtituloDireita">Data de Início:</td>
                			<td><?=campo_data2("vgcdtinicio","S","S","Data de Início","##/##/####"); ?></td>
                		</tr>
                		<tr>
                			<td class="SubtituloDireita">Data de Término:</td>
                			<td><?=campo_data2("vgcdtfim","S","S","Data de Término","##/##/####"); ?></td>
                		</tr>
                		<tr>
                			<td class="SubtituloDireita">Situação:</td>
                			<td>
                				<?php $sql = "	select
                									tsvid as codigo,
                									tsvdsc as descricao
                								from
                									fabrica.tiposituacaovigencia
                								where
                									tsvstatus = 'A'
                								order by
                									tsvdsc"; ?>
                				<?=$db->monta_combo("situacao", $sql, "S", "Selecione...", "", "", "", "", "S", 'situacao', '', $situacao, $situacao); ?>
                			</td>
                		</tr>
                        <tr>
                            <td class="SubtituloDireita">Gestor do Contrato:</td>
                            <td>
                            <?php
                            
                                $sql_combo = "select
                                        u.usucpf as codigo,
                                        u.usunome as descricao
                                from
                                        seguranca.perfilusuario pu
                                inner join
                                        seguranca.usuario u ON u.usucpf = pu.usucpf
                                where
                                        pu.pflcod = " . PERFIL_GESTOR_CONTRATO;

                                $db->monta_combo("gestorcpf",$sql_combo,'S',"-- Selecione --",'','','', '', 'S', 'gestorcpf','',$gestorcpf);
                            ?>
                            </td>
                        </tr>
						<tr>
							<td class="SubtituloDireita">Fiscal Administrativo:</td>
							<td>
								<?php

								$sql_combo = "select
                                        u.usucpf as codigo,
                                        u.usunome as descricao
                                from
                                        seguranca.perfilusuario pu
                                inner join
                                        seguranca.usuario u ON u.usucpf = pu.usucpf
                                where
                                        pu.pflcod = " . PERFIL_FISCAL_CONTRATO;

								$db->monta_combo("fiscaladmcpf",$sql_combo,'S',"-- Selecione --",'','','', '', 'S', 'fiscaladmcpf','',$fiscaladmcpf);
								?>
							</td>
						</tr>
						<tr>
							<td class="SubtituloDireita">Fiscal Preposto:</td>
							<td>
								<?php

								$sql_combo = "select
                                        u.usucpf as codigo,
                                        u.usunome as descricao
                                from
                                        seguranca.perfilusuario pu
                                inner join
                                        seguranca.usuario u ON u.usucpf = pu.usucpf
                                where
                                        pu.pflcod = " . PERFIL_PREPOSTO;

								$db->monta_combo("prepostocpf",$sql_combo,'S',"-- Selecione --",'','','', '', 'S', 'prepostocpf','',$prepostocpf);
								?>
							</td>
						</tr>
						<tr>
							<td class="SubtituloDireita">Fiscal(is) do Contrato:</td>
							<td>
								<?php
								$sql_combo = "select
                								u.usucpf as codigo,
                								u.usunome as descricao
                							from
                								seguranca.perfilusuario pu
                							inner join
                								seguranca.usuario u ON u.usucpf = pu.usucpf
                							where
                								pu.pflcod = ".PERFIL_FISCAL_CONTRATO;

								if( $vgcid ){
									if( $db->pegaUm("SELECT count(1) FROM fabrica.fiscalvigencia WHERE vgcid = ".$vgcid ) > 0 )
									{
										$sql_carregados = "SELECT
                										 		u.usucpf as codigo,
                												u.usunome as descricao
                										   FROM
                										   		fabrica.fiscalvigencia fc
                										   INNER JOIN
                										   		seguranca.usuario u ON u.usucpf = fc.usucpf
                										   WHERE
                										   		fc.vgcid = ".$vgcid;
										$fiscal = $db->carregar( $sql_carregados );
									}
								}

								combo_popup( 'fiscal', $sql_combo, 'Selecione o(s) Fiscal(is)', '400x400', 0, array(), '', 'S', true, true, 6, 400, null, null, null, null, null, true, true, null, true );
								?>
							</td>
						</tr>
                        <tr>
                			<td class="SubtituloDireita">Volume e Valores dos Itens do Contrato:</td>
                			<td>
                				<table width="50%" class="listagem" id="tabelaItens">
                					<tr>
                						<td>
                							Item
                						</td>
                						<td>
                							Volume Total
                						</td>
                						<td>
                							Valor Unitário
                						</td>
                					</tr>
                					<?
	                				for($i=0; $i<=$totalItens; $i++){
	                					
	                					if($vgcid && $itens[$i]['mtiid']){
		                					$sql = "SELECT
			                										vcmvolumecontratado,
			                										vcmvalorcontratado
			                									FROM
			                										fabrica.vigenciacontratometricaitem 
			                									WHERE
			                										vgcid = ".$vgcid."
			                									and
			                										mtiid = ".$itens[$i]['mtiid'];
			                				$vitens = $db->pegaLinha( $sql );
			                				//dbg($sql);
	                					}
	                					
	                					?>
	                					<tr>
	                						<td>
	                							 <?php
						                                $sql_combo = "select 
						                								mi.mtiid as codigo,
						                								mt.mtcsigla ||' - '|| mi.mtinome as descricao
						                							from 
						                								fabrica.metricaitem mi
						                							inner join fabrica.metrica mt on mt.mtcid = mi.mtcid 
						                							where 
						                								mi.mtistatus = 'A'";
						                                $db->monta_combo("vitem[]",$sql_combo, "N","-- Selecione --",'','','', '', "N", 'vitem_'.$i,'',$itens[$i]['mtiid']);
						                                
					                            ?>
					                            <?if($vgcid){?>
					                            	<input type="hidden" name="xvitem[]" value="<?=$itens[$i]['mtiid']?>">
					                            <?}?>
	                						</td>
	                						<td>
	                							<?=campo_texto("volume[]","S", "S","Quantidade de Pontos por Função Contratado","20","120","###.###.###.###,##", "", "right", "", "", "", "", ($vitens['vcmvolumecontratado'] ? number_format($vitens['vcmvolumecontratado'],2,',','.') : '' ) ) ?>
	                						</td>
	                						<td>
	                							<?=campo_texto("valor[]", "S", "S", "Valor de Ponto de Função no contrato", "20", "120", "###.###.###.###,##", "", "right", "", "", "", "", ($vitens['vcmvalorcontratado'] ? number_format($vitens['vcmvalorcontratado'],2,',','.') : '' ) ) ?>
	                						</td>
	                					</tr>
	                				<?}?>
                				</table>
                			</td>
                		</tr>
                		<!-- 
                		<tr>
                			<td class="SubtituloDireita">Volume de Pontos por Função Contratado:</td>
                			<td>
                				<?=campo_texto("vgcvolumepfcontratado","S","S","Quantidade de Pontos por Função Contratado","20","120","##############,##","") ?>
                			</td>
                		</tr>
                		<tr>
                			<td class="SubtituloDireita">Volume de Pontos por Função Utilizado:</td>
                			<td>
                				<?=campo_texto("vgcvolumepfutilizado","N","N","Quantidade de Pontos por Função Utilizado","20","120","","") ?>
                			</td>
                		</tr>
                		<tr>
                			<td class="SubtituloDireita">Valor de Ponto de Função no contrato:</td>
                			<td>
                				<?=campo_texto("vgcvalorpfcontrato", "S", "S", "Valor de Ponto de Função no contrato", "20", "120", "###.###.###,##", ""); ?>
                			</td>
                		</tr>
                		 -->
                        <tr>
            			    <td colspan="2" class="TituloTabela center">
            			        <?php if( $editarVigencia ) { ?>
            			    	    <input type="hidden" name="requisicao" value="editarVigencia" />
            			    	<?php }else{ ?>
            			    	    <input type="hidden" name="requisicao" value="salvar" />
            			    	<?php } ?>
            			    	
            			    	<input type="hidden" name="vgcid" value="<?=$vgcid?>" />
				                <input type="hidden" name="ctrid" value="<?=$ctrid?>" />

            				    <input type="button" onclick="salvarContrato()" value="Salvar" >
            				    <input type="button" onclick="limparCampos()" value="Limpar Campos" >
            			    </td>
            		    </tr>
                    </table>
                </fieldset>
                <?php }elseif( !empty($ctrid) ){
                    
                    echo "<br>";
                    
                    $sqlListaVigencias = "select distinct 
                                        case when rpc.rpcid is not null then
                                        	        '
                                    	        <a href=\"javascript:void(0);\" onclick=\"montaSubLista('|| vgc.vgcid ||')\">
                                    	        <img id=\"img_mais_'|| vgc.vgcid ||'\" src=\"../imagens/mais.gif\" border=0 title=\"Abrir lista repactução\">
                                    	        </a>
                                    	        <a href=\"javascript:void(0);\" onclick=\"desmontaSubLista('|| vgc.vgcid ||')\">
                                    	        <img id=\"img_menos_'|| vgc.vgcid ||'\" src=\"../imagens/menos.gif\" border=0 title=\"Fechar lista repactuação\" style=\"display:none\">
                                    	        </a>

                                    		    <img class=\"middle link\" title=\"Adicionar Repactuação\" alt=\"Adicionar Repactuação\" onclick=\"adicionarRepactuacao(\'' || vgc.vgcid || '\')\" src=\"../imagens/send.png\" /> 
                                    	        <img class=\"middle link\" title=\"Alterar Vigência\" alt=\"Alterar Vigência\" onclick=\"editarVigencia(\'' || vgc.vgcid || '\')\" src=\"../imagens/editar_nome.gif\" /> 
                                    	        <img class=\"middle link\" title=\"Excluir Vigência\" alt=\"Excluir Vigência\" src=\"../imagens/excluir_01.gif\" />
                                        	        ' 
                                        	when totalos._totalos > 0 then
                                        		'
                                        		<img class=\"middle link\" title=\"Adicionar Repactuação\" alt=\"Adicionar Repactuação\" onclick=\"adicionarRepactuacao(\'' || vgc.vgcid || '\')\" src=\"../imagens/send.png\" /> 
                                        	    <img class=\"middle link\" title=\"Alterar Vigência\" alt=\"Alterar Vigência\" onclick=\"editarVigencia(\'' || vgc.vgcid || '\')\" src=\"../imagens/editar_nome.gif\" /> 
                                        	    <img class=\"middle link\" title=\"Excluir Vigência\" alt=\"Excluir Vigência\" src=\"../imagens/excluir_01.gif\" />
                                        		'                                        	
                                        	else
                                        		'
                                        		<img class=\"middle link\" title=\"Adicionar Repactuação\" alt=\"Adicionar Repactuação\" onclick=\"adicionarRepactuacao(\'' || vgc.vgcid || '\')\" src=\"../imagens/send.png\" /> 
                                        	    <img class=\"middle link\" title=\"Alterar Vigência\" alt=\"Alterar Vigência\" onclick=\"editarVigencia(\'' || vgc.vgcid || '\')\" src=\"../imagens/editar_nome.gif\" /> 
                                        	    <img class=\"middle link\" title=\"Excluir Vigência\" alt=\"Excluir Vigência\" onclick=\"excluirVigencia(\'' || vgc.vgcid || '\', \'' || ctr.ctrid || '\')\" src=\"../imagens/excluir.gif\" />
                                        		'
                                        end  as acao,
                                        	(select usunome from seguranca.usuario where usucpf = vgc.gestorcpf) as gestor,
                                        	to_date(to_char(vgc.vgcdtinicio, 'dd/MM/YYYY'),'dd/MM/YYYY') as vgcdtinicio,
                                        	to_date(to_char(vgc.vgcdtfim, 'dd/MM/YYYY'),'dd/MM/YYYY') as vgcdtfim,                                        	
                                        	tsv.tsvdsc,
                                        	tpv.tpvdsc || '</tr><tr style=\"background-color:#F7F7F7\" ><td colspan=10 style=\"padding-left:20px;\" id=\"td_' || vgc.vgcid || '\" ></td></tr>' as tipo
                                        	--coalesce(vgc.vgcvolumepfcontratado, 0) as vgcvolumepfcontratado,
                                        	--coalesce(vgc.vgcvolumepfutilizado, 0) as vgcvolumepfutilizado,
                                        	--coalesce(vgc.vgcvalorpfcontrato, 0) as vgcvalorpfcontrato,
                                        	
                                        from
                                        	fabrica.contrato ctr
                                        	inner join fabrica.vigenciacontrato vgc     ON vgc.ctrid = ctr.ctrid and vgc.vgcstatus = 'A'
                                        	inner join fabrica.tipovigencia tpv         ON tpv.tpvid = vgc.tpvid and tpv.tpvstatus = 'A'
                                        	inner join fabrica.tiposituacaovigencia tsv ON tsv.tsvid = vgc.tsvid and tsv.tsvstatus = 'A'
                                        	left join (select 
                                                        	count(_ods.odsid) as _totalos
                                                        	, _vgc.vgcid
                                                        from fabrica.vigenciacontrato _vgc
                                                        inner join fabrica.ordemservico _ods on _ods.ctrid = _vgc.ctrid
                                                        where _ods.odsdtprevinicio  between _vgc.vgcdtinicio and _vgc.vgcdtfim
                                                        and _ods.odsdtprevinicio between _vgc.vgcdtinicio and _vgc.vgcdtfim
                                                        group by _vgc.vgcid) as totalos
                                                        on totalos.vgcid = vgc.vgcid
                                        	left join fabrica.repactuacaocontrato rpc   ON rpc.vgcid = vgc.vgcid and rpc.rpcstatus = 'A'
                                        where ctr.ctrstatus = 'A'
                                        and ctr.ctrid = $ctrid
                                        order by vgcdtinicio desc";
                    
                    //$cabecalho = array("Ações","Gestor","Data de Início","Data de Término","Situação", "Volume de Pontos por Função Contratado", "Volume de Pontos por Função Utilizado", "Valor de Ponto de Função no Contrato", "Tipo");
                    $cabecalho = array("Ações","Gestor","Data de Início","Data de Término","Situação", "Tipo");
                    //echo'<pre>';die($sql);
                    $db->monta_lista($sqlListaVigencias,$cabecalho,100,50,"N","center","N");
                } ?>
                <?php 
                
                ?>
            </form>
        </td>
    </tr>
</table>
<script type="text/javascript">
$(document).ready(function() {

	// Gatinho para formatação de datas
	$('td[title="Data de Início"], td[title="Data de Término"]').each(function(){

	    var data = $(this).text();
	    var dataArray = data.split("-");
	    var dataFormatada =  dataArray[2] +"/"+ dataArray[1] +"/"+  dataArray[0];
	    $(this).text( dataFormatada ); 
	});

	$('#ctrtipoempresaitem').click(function(){
        return false;
    });
});
</script>
<?php if($msg): ?>
<script>
	alert('<?php echo $msg ?>');
</script>
<?php endif; ?>