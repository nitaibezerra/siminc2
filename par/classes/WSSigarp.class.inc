<?php
class WSSigarp {
	
	public $db;
	
	public function __construct(&$db = null){
		if($db){
			$this->db = $db;			
		} else {
			$this->db = new cls_banco(); 
		}
	}
	
	private function getDataAtual(){
		return date("c");
	}
	
	public function listarCategoria( $preid ){
		
		global $db;
		
		$filtroTooid = $_SESSION['par']['tooid'] != '' ? " AND tooid = {$_SESSION['par']['tooid']} " : "";

		try {
			
			$data_created = $this->getDataAtual();
	
			$usuario = USUARIO_SIGARP;
			$senha   = SENHA_SIGARP;

$arqXml = <<<XML
<?xml version="1.0" encoding="ISO-8859-1"?>
<request>
	<header>
		<app>SIGARP</app>
		<version>1.0</version>
		<created>$data_created</created>
	</header>
	<body>
		<auth>
			<usuario>$usuario</usuario>
            <senha>$senha</senha>
		</auth>
		<params>
		</params>
	</body>
</request>
XML;

	/*	
	echo '<pre>';		
	echo simec_htmlentities($arqXml);
	die();
		*/	   	
			if( $_SESSION['baselogin'] == "simec_desenvolvimento" || $_SESSION['baselogin'] == "simec_espelho_producao" ){
				//$urlWS = 'http://hmg.fnde.gov.br/webservices/wssigarp/ws/categoria';
				$urlWS = 'http://172.20.200.115/webservices/wssigarp/ws/categoria';
			} else {
				$urlWS = 'http://www.fnde.gov.br/webservices/wssigarp/ws/categoria';
			}

			$xml = Fnde_Webservice_Client::CreateRequest()
					->setURL($urlWS)
					->setParams( array('xml' => $arqXml, 'method' => 'listar') )
					->execute();
						
			$xmlRetorno = $xml;
			$xml = simplexml_load_string( stripslashes($xml) );

			$result = (integer) $xml->status->result;

			if(!$result) { // Erro
				
				$sql = "INSERT INTO par.historicowspreobra(
				    	preid,
				    	hwpwebservice,
				    	hwpxmlenvio,
				    	hwpxmlretorno,
				    	hwpdataenvio,
				        usucpf)
				    VALUES ('".$preid."',
				    		'listarCategoria - Erro',
				    		'".addslashes($arqXml)."',
				    		'".addslashes($xmlRetorno)."',
				    		NOW(),
				            '".$_SESSION['usucpf']."');";

				$this->db->executar($sql);
				$this->db->commit();
				
				// Carrego as categorias de backup da tabela.
				$sql = "SELECT ptoid as codigo, ptodescricao as descricao FROM obras.pretipoobra WHERE ptocategoria IS NOT NULL $filtroTooid";
				return $this->db->carregar($sql);
				
			} else { // Sucesso
				
				$sql = "INSERT INTO par.historicowspreobra(
				    	preid,
				    	hwpwebservice,
				    	hwpxmlenvio,
				    	hwpxmlretorno,
				    	hwpdataenvio,
				        usucpf)
				    VALUES ('".$preid."',
				    		'listarCategoria - Sucesso',
				    		'".addslashes($arqXml)."',
				    		'".addslashes($xmlRetorno)."',
				    		NOW(),
				            '".$_SESSION['usucpf']."');";

				$this->db->executar($sql);
				$this->db->commit();
				
				$sqlIns = "";
				foreach( $xml->body as $categoria ){
					if( $_SESSION['baselogin'] == "simec_desenvolvimento" ){
						$cat1 = 50;
						$cat2 = 49;
					} else {
						$cat1 = 52;
						$cat2 = 53;
					}
					if( (integer)$categoria->NU_SEQ_CATEGORIA == $cat1 || (integer)$categoria->NU_SEQ_CATEGORIA == $cat2 ){
						$cat = $this->db->pegaUm("SELECT nu_seq_categoria FROM par.s_categoria WHERE nu_seq_categoria = ".(integer)$categoria->NU_SEQ_CATEGORIA);
						$ptoid = $this->db->pegaUm("SELECT ptoid FROM obras.pretipoobra WHERE ptocategoria = ".(integer)$categoria->NU_SEQ_CATEGORIA);
						if(!$cat){
							$sqlIns = "INSERT INTO par.s_categoria ( nu_seq_categoria, no_categoria, st_categoria ) VALUES (".(integer)$categoria->NU_SEQ_CATEGORIA.",'".utf8_decode($categoria->NO_CATEGORIA)."', 'A'); ";
							$this->db->executar($sqlIns);
						}
						if(!$ptoid){
							if( (integer)$categoria->NU_SEQ_CATEGORIA == $cat1) {
								$tpoid = 104;
							} elseif( (integer)$categoria->NU_SEQ_CATEGORIA == $cat2 ){
								$tpoid = 105;
							}
							$sqlIns = "INSERT INTO obras.pretipoobra ( ptodescricao, ptostatus, ptoclassificacaoobra, ptopercentualempenho, tooid, ptoprojetofnde, ptoesfera, ptoexisteescola, ptocategoria, tpoid ) 
										VALUES ( '".utf8_decode($categoria->NO_CATEGORIA)."', 'A', 'P', 100, 1, 't', 'T', 'f', ".(integer)$categoria->NU_SEQ_CATEGORIA.", ".$tpoid." ) RETURNING ptoid; ";
							$ptoid = $this->db->pegaUm($sqlIns);
							$sqlIns = "INSERT INTO obras.pretipodocumentoobra 
										SELECT podid, ".$ptoid." FROM obras.pretipodocumentoobra
										WHERE ptoid IN (38);
										DELETE FROM obras.pretipodocumentoobra WHERE ptoid = ".$ptoid." AND podid = 8; ";
							$this->db->executar($sqlIns);
						}
					}
				}
				if( $sqlIns ){
					$this->db->commit();
				} else {
					$this->db->rollback();
				}
				
				$sql = "SELECT ptoid as codigo, ptodescricao as descricao FROM obras.pretipoobra WHERE ptocategoria IS NOT NULL $filtroTooid";
				return $this->db->carregar($sql);
				
			}
			
		} catch (Exception $e){
	
			# Erro 404 página not found
			if($e->getCode() == 404){
				//echo "Temporariamente indisponível.Favor tente mais tarde.".'\n';
				/* -- ARRUMAR DEPOIS 
				echo '<div style=" border: 1px solid #B7B7B7; font-size: 10px; font-style: normal; font-family: arial; padding: 5px 5px 5px 5px;"> 
							ERRO AO CONSULTAR ITENS DO PREGÃO (SIGARP)
							<div style=" border-top: 1px solid #B7B7B7; padding-top: 5px; " >
								Temporariamente indisponível.Favor tente mais tarde.
							</div>
						</div>
					 	<br>
						';
				*/
				
			}
			$erroMSG = str_replace(array(chr(13),chr(10)), ' ',$e->getMessage());
			$erroMSG = str_replace( "'", '"', $erroMSG );
			/* -- ARRUMAR DEPOIS 
			echo '<div style=" border: 1px solid #B7B7B7; font-size: 10px; font-style: normal; font-family: arial; padding: 5px 5px 5px 5px;"> 
							ERRO AO CONSULTAR ITENS DO PREGÃO (SIGARP)
							<div style=" border-top: 1px solid #B7B7B7; padding-top: 5px; " >
								Erro-WS SIGARP: '.$erroMSG.'
								<br>
								<pre>
								'.print_r($e).'
								</pre>
							</div>
						</div>
					 	<br>
						';
			*/
			
			//echo "Erro-WS SIGARP: $erroMSG";
			//echo "<pre>";
			//print_r($e);
			
		}
	}
	
	public function listarItemCategoria($categoria, $uf, $preid, $atualizaMI = null){
		global $db;
		
		try {
			
			$data_created = $this->getDataAtual();
	
			$usuario = USUARIO_SIGARP;
			$senha   = SENHA_SIGARP;

$arqXml = <<<XML
<?xml version="1.0" encoding="ISO-8859-1"?>
<request>
	<header>
		<app>SIGARP</app>
		<version>1.0</version>
		<created>$data_created</created>
	</header>
	<body>
		<auth>
			<usuario>$usuario</usuario>
            <senha>$senha</senha>
		</auth>
		<params>
			<nu_seq_categoria>$categoria</nu_seq_categoria>
			<uf>$uf</uf>
		</params>
	</body>
</request>
XML;

		/*
	echo '<pre>';		
	echo simec_htmlentities($arqXml);
	die();
			*/   	
			if( $_SESSION['baselogin'] == "simec_desenvolvimento" || $_SESSION['baselogin'] == "simec_espelho_producao" ){
				//$urlWS = 'http://hmg.fnde.gov.br/webservices/wssigarp/ws/categoria';
				$urlWS = 'http://172.20.200.115/webservices/wssigarp/ws/pregao';
			} else {
				$urlWS = 'http://www.fnde.gov.br/webservices/wssigarp/ws/pregao';
			}

			$xml = Fnde_Webservice_Client::CreateRequest()
					->setURL($urlWS)
					->setParams( array('xml' => $arqXml, 'method' => 'listarcategoria') )
					->execute();
						
			$xmlRetorno = $xml;
			$xml = simplexml_load_string( stripslashes($xml) );

			$result = (integer) $xml->status->result;

			if(!$result) { // Erro
				
				$sql = "INSERT INTO par.historicowspreobra(
				    	preid,
				    	hwpwebservice,
				    	hwpxmlenvio,
				    	hwpxmlretorno,
				    	hwpdataenvio,
				        usucpf)
				    VALUES ('".$preid."',
				    		'listarItemCategoria - Erro',
				    		'".addslashes($arqXml)."',
				    		'".addslashes($xmlRetorno)."',
				    		NOW(),
				            '".$_SESSION['usucpf']."');";

				$this->db->executar($sql);
				$this->db->commit();
				
				if( $atualizaMI != '' ){
					$arDados = Array( 'erro' => $xml->status->error->message->text);
					return simec_json_encode($arDados);
				}

				$ptoid = $this->db->pegaUm("SELECT ptoid FROM obras.preobra WHERE preid = ".$preid);
				
				// Carrego os itens de backup da tabela.
				$dados = $this->db->carregar("SELECT * FROM obras.preobra pre
												INNER JOIN obras.preitenscomposicaomi itc ON itc.ptoid = pre.ptoid AND itc.preid = pre.preid
												LEFT JOIN obras.unidademedida umd ON umd.umdid = itc.umdid
												WHERE pre.ptoid = {$ptoid}
												AND itcstatus = 'A'
												AND pre.preid = '{$preid}'
												AND itccodigoitemcodigo <> '' 
												ORDER BY itccodigoitemcodigo");
				
				if( is_array($dados) ){
					foreach( $dados as $c => $dado ){
						$arDados[$c]['itcid'] 		 		= $dado['itcid'];
						$arDados[$c]['boFilho'] 		 	= $this->db->pegaUm("SELECT count(1) FROM obras.preobra pre INNER JOIN obras.preitenscomposicaomi itc ON itc.preid = pre.preid
																WHERE pre.ptoid = {$ptoid} AND pre.preid = '{$preid}' AND itc.itcstatus = 'A' AND itc.itccodigoitemcodigo <> '' AND itc.itcidpai = {$dado['itcid']}");
					  	$arDados[$c]['img'] 			 	= "menos.gif";
					  	$arDados[$c]['itcquantidade'] 	 	= $dado['itcquantidade']?$dado['itcquantidade']:0;
					  	$arDados[$c]['itccodigoitem'] 		= $dado['itccodigoitem'];
					  	$arDados[$c]['itccodigoitemcodigo'] = $dado['itccodigoitemcodigo'];
					  	$arDados[$c]['itcdescricao'] 	 	= utf8_encode($dado['itcdescricao']);
					  	$arDados[$c]['umdeesc'] 		 	= utf8_encode($dado['umdeesc']);
					  	$arDados[$c]['ppovalorunitario'] 	= $dado['itcvalorunitario'];
					  	$arDados[$c]['itcordem'] 			= $dado['itcordem'];
					  	$arDados[$c]['ptoid'] 				= $dado['ptoid'];
					  	$arDados[$c]['itcidpai'] 			= $dado['itcidpai'];
					  	$arDados[$c]['itcvalorunitario'] 	= $dado['itcvalorunitario'];
					  	$arDados[$c]['ppoid'] 				= $dado['ppoid'];
					  	$arDados[$c]['preid'] 				= $dado['preid'];
					  	$arDados[$c]['itctipofundacao'] 	= $dado['itctipofundacao'];

					  	
					  	$arDados[$c]['total'] 			 	= 100;
					}
					//ver($arDados);
					return simec_json_encode($arDados);
				} else {
					// Carrego a casca
					$sqlInsIni = "INSERT INTO obras.preitenscomposicaomi(
								            itcdescricao, itcstatus, itcdtinclusao, ptoid, itccodigoitem, itccodigoitemcodigo, preid )
								    	VALUES ('ITENS', 'A', 'NOW()', ".$ptoid.", '1', '001', ".$preid.") RETURNING itcid";
							
					$itcidPai = $this->db->pegaUm($sqlInsIni);
					$itcidPai = ($itcidPai ? $itcidPai : 'null');
					$this->db->commit();
					
					$preidBackup = $this->db->pegaUm("SELECT preid FROM obras.preitenscomposicaomi WHERE ptoid = ".$ptoid." LIMIT 1");
					
					$sqlIns = "INSERT INTO obras.preitenscomposicaomi(
							            itcdescricao, itcstatus, itcdtinclusao, ptoid, itcvalorunitario, itccodigoitem, itccodigoitemcodigo, itcidpai, itcitemsigarp, umdid, preid )
							            SELECT itcdescricao, itcstatus, itcdtinclusao, ptoid, itcvalorunitario::numeric, itccodigoitem, itccodigoitemcodigo, {$itcidPai}, itcitemsigarp, itc.umdid, {$preid} 
							            FROM obras.preitenscomposicaomi itc
										LEFT JOIN obras.unidademedida umd ON umd.umdid = itc.umdid
										WHERE itc.ptoid = {$ptoid}
										AND itc.preid = {$preidBackup}
										AND itcstatus = 'A'
										AND itccodigoitemcodigo <> ''
										AND itcdescricao <> 'ITENS'";
						
					$this->db->executar($sqlIns);
					$this->db->commit();
					
					$dados = $this->db->carregar("SELECT * FROM obras.preobra pre
												INNER JOIN obras.preitenscomposicaomi itc ON itc.ptoid = pre.ptoid AND pre.preid = itc.preid
												LEFT JOIN obras.unidademedida umd ON umd.umdid = itc.umdid
												WHERE pre.ptoid = {$ptoid}
												AND itcstatus = 'A'
												AND pre.preid = '{$preid}'
												AND itccodigoitemcodigo <> '' 
												ORDER BY itccodigoitemcodigo");
				
					if( is_array($dados) ){
						foreach( $dados as $c => $dado ){
							$arDados[$c]['itcid'] 		 		= $dado['itcid'];
							$arDados[$c]['boFilho'] 		 	= $this->db->pegaUm("SELECT count(1) FROM obras.preobra pre INNER JOIN obras.preitenscomposicaomi itc ON itc.preid = pre.preid
																	WHERE pre.ptoid = {$ptoid} AND pre.preid = '{$preid}' AND itc.itcstatus = 'A' AND itc.itccodigoitemcodigo <> '' AND itc.itcidpai = {$dado['itcid']}");
						  	$arDados[$c]['img'] 			 	= "menos.gif";
						  	$arDados[$c]['itcquantidade'] 	 	= $dado['itcquantidade']?$dado['itcquantidade']:0;
						  	$arDados[$c]['itccodigoitem'] 		= $dado['itccodigoitem'];
						  	$arDados[$c]['itccodigoitemcodigo'] = $dado['itccodigoitemcodigo'];
						  	$arDados[$c]['itcdescricao'] 	 	= utf8_encode($dado['itcdescricao']);
						  	$arDados[$c]['umdeesc'] 		 	= utf8_encode($dado['umdeesc']);
						  	$arDados[$c]['ppovalorunitario'] 	= $dado['itcvalorunitario'];
						  	$arDados[$c]['itcordem'] 			= $dado['itcordem'];
						  	$arDados[$c]['ptoid'] 				= $dado['ptoid'];
						  	$arDados[$c]['itcidpai'] 			= $dado['itcidpai'];
						  	$arDados[$c]['itcvalorunitario'] 	= $dado['itcvalorunitario'];
						  	$arDados[$c]['ppoid'] 				= $dado['ppoid'];
						  	$arDados[$c]['preid'] 				= $dado['preid'];
						  	$arDados[$c]['itctipofundacao'] 	= $dado['itctipofundacao'];
	
						  	
						  	$arDados[$c]['total'] 			 	= 100;
						}
						//ver($arDados);
						return simec_json_encode($arDados);
					}
				}
				
			} else { // Sucesso
				
				$sql = "INSERT INTO par.historicowspreobra(
				    	preid,
				    	hwpwebservice,
				    	hwpxmlenvio,
				    	hwpxmlretorno,
				    	hwpdataenvio,
				        usucpf)
				    VALUES ('".$preid."',
				    		'listarItemCategoria - Sucesso',
				    		'".addslashes($arqXml)."',
				    		'".addslashes($xmlRetorno)."',
				    		NOW(),
				            '".$_SESSION['usucpf']."');";

				$this->db->executar($sql);
				$this->db->commit();
				
				$ptoid = $this->db->pegaUm("SELECT ptoid FROM obras.preobra WHERE preid = ".$preid);
				
				$sqlIns = "";
				$sqlIns2 = "";
				$count = 0;
				foreach( $xml->body as $item ){
					$sql = "SELECT count(itcid) FROM obras.preitenscomposicaomi WHERE ptoid = ".$ptoid." AND itcstatus = 'A' AND preid = ".$preid." AND itcitemsigarp = ".(integer)$item->NU_SEQ_ITEM;
					$teste = $this->db->pegaUm($sql);
					if($teste == 0){
						$count++;
						$conta = $this->db->pegaUm("SELECT count(itcid) FROM obras.preitenscomposicaomi WHERE preid = ".$preid);
						if( $conta = 0 || !$conta ){
							$sqlInsIni = "INSERT INTO obras.preitenscomposicaomi(
								            itcdescricao, itcstatus, itcdtinclusao, ptoid, itccodigoitem, itccodigoitemcodigo, preid )
								    	VALUES ('ITENS', 'A', 'NOW()', ".$ptoid.", '1', '001', ".$preid.") RETURNING itcid";
							
							$itcidPai = $this->db->pegaUm($sqlInsIni);
							$itcidPai = ($itcidPai ? $itcidPai : 'null');
							/*
							$sqlIns2Ini = "INSERT INTO obras.preplanilhaorcamentaria(
											preid, itcid, ppovalorunitario)
										VALUES (".$preid.", ".$itcidPai.", 0)"; 
							
							$this->db->executar($sqlIns2Ini);
							*/
							$this->db->commit();
						} else {
							$sql = "SELECT itcid FROM obras.preitenscomposicaomi WHERE preid = ".$preid." AND itcdescricao = 'ITENS'";
							$itcidPai = $this->db->pegaUm($sql);
							$itcidPai = ($itcidPai ? $itcidPai : 'null');
						}
						
						if( utf8_decode($item->DS_UNIDADE_MEDIDA) == 'm²' ){
							$it = 4;
						}elseif( utf8_decode($item->DS_UNIDADE_MEDIDA) == 'Unidade' ){
							$it = 2;
						}elseif( utf8_decode($item->DS_UNIDADE_MEDIDA) == 't * Km' ){
							$it = 12;
						}elseif( utf8_decode($item->DS_UNIDADE_MEDIDA) == 'm' ){
							$it = 1;
						}elseif( utf8_decode($item->DS_UNIDADE_MEDIDA) == 'Furo' ){
							$it = 11;
						} else {
							$it = null;
						}
						
						$sqlIns = "INSERT INTO obras.preitenscomposicaomi(
							            itcdescricao, itcstatus, itcdtinclusao, ptoid, itcvalorunitario, itccodigoitem, itccodigoitemcodigo, itcidpai, itcitemsigarp, umdid, preid )
							    	VALUES ('".utf8_decode((string)$item->NO_ITEM)."', 'A', 'NOW()', ".$ptoid.", ".$item->VL_ITEM.", '1.".$count."', '".str_pad($count, 6, "00100", STR_PAD_LEFT)."', ".$itcidPai.", ".(integer)$item->NU_SEQ_ITEM.", ".$it.",".$preid.") RETURNING itcid";
						
						$itcid = $this->db->pegaUm($sqlIns);
						/*
						$sqlIns2 = "INSERT INTO obras.preplanilhaorcamentaria(
										preid, itcid, ppovalorunitario)
									VALUES (".$preid.", ".$itcid.", ".retiraPontosBD($item->VL_ITEM).")"; 
						
						$this->db->executar($sqlIns2);
						*/
						$this->db->commit();
					} else {
						if( (integer)$item->NU_SEQ_ITEM ){
							$sql = "SELECT count(itcid) FROM obras.preitenscomposicaomi WHERE ptoid = ".$ptoid." AND preid = ".$preid." AND itcitemsigarp = ".(integer)$item->NU_SEQ_ITEM." AND itcvalorunitario = ".$item->VL_ITEM;
							$teste = $this->db->pegaUm($sql);
							$sql = "SELECT count(*) FROM obras.preobra p 
									INNER JOIN workflow.documento d ON d.docid = p.docid AND d.esdid in (".WF_TIPO_EM_REFORMULACAO_OBRAS_MI.",".WF_TIPO_ANALISE_DE_REFORMULACAO_OBRAS_MI.",".WF_TIPO_EM_CADASTRAMENTO.")
									WHERE p.preid = ".$preid;
							$testeSituacao = $this->db->pegaUm($sql);
							if( $teste == 0 && ($testeSituacao > 0 || $atualizaMI) ){ // Valor está errado e a obra está em reformulação ou em cadastramento ou está forçando a atualização
								//verifico se já tem contrato assinado. Se não tem, atualizo os valores.
								$sql = "SELECT aoscodsituacao FROM par.adesaoobraspacsituacao WHERE preid = ".$preid." AND aoscodsituacao = 110";
								$situacaoObra = $this->db->pegaUm($sql);
								if( !$situacaoObra ){
									$sqlUpdateValorItens .= "UPDATE obras.preitenscomposicaomi SET itcvalorunitario = ".$item->VL_ITEM." WHERE ptoid = ".$ptoid." AND preid = ".$preid." AND itcitemsigarp = ".(integer)$item->NU_SEQ_ITEM."; ";
								}
							}
						}
					}
				}
				
				if($sqlUpdateValorItens){
					$this->db->executar($sqlUpdateValorItens);
					$this->db->commit();
					// Verifico se ele já comecou a preencher a Obra. Se sim, atualizo o valor dela.
					$sql = "SELECT count(itcquantidade) FROM obras.preitenscomposicaomi WHERE ptoid = ".$ptoid." AND preid = ".$preid." AND itcquantidade IS NOT NULL AND itcquantidade <> 0.00";
					$valorItemObra = $this->db->pegaUm($sql);
					if( $valorItemObra > 0 ){
						atualizaValorObra( $preid, 'mi' );
					}
				}

				$dados = $this->db->carregar("SELECT * FROM obras.preobra pre
												INNER JOIN obras.preitenscomposicaomi itc ON itc.ptoid = pre.ptoid AND pre.preid = itc.preid
												LEFT JOIN obras.unidademedida umd ON umd.umdid = itc.umdid
												WHERE pre.ptoid = {$ptoid}
												AND itcstatus = 'A'
												AND pre.preid = '{$preid}'
												AND itccodigoitemcodigo <> '' 
												ORDER BY itccodigoitemcodigo");

				if( is_array($dados) ){
					foreach( $dados as $c => $dado ){
						$arDados[$c]['itcid'] 		 		= $dado['itcid'];
						$arDados[$c]['boFilho'] 		 	= $this->db->pegaUm("SELECT count(1) FROM obras.preobra pre INNER JOIN obras.preitenscomposicaomi itc ON itc.preid = pre.preid
																WHERE pre.ptoid = {$ptoid} AND pre.preid = '{$preid}' AND itc.itcstatus = 'A' AND itc.itccodigoitemcodigo <> '' AND itc.itcidpai = {$dado['itcid']}");
					  	$arDados[$c]['img'] 			 	= "menos.gif";
					  	$arDados[$c]['itcquantidade'] 	 	= $dado['itcquantidade']?$dado['itcquantidade']:0;
					  	$arDados[$c]['itccodigoitem'] 		= $dado['itccodigoitem'];
					  	$arDados[$c]['itccodigoitemcodigo'] = $dado['itccodigoitemcodigo'];
					  	$arDados[$c]['itcdescricao'] 	 	= utf8_encode($dado['itcdescricao']);
					  	$arDados[$c]['umdeesc'] 		 	= utf8_encode($dado['umdeesc']);
					  	$arDados[$c]['ppovalorunitario'] 	= $dado['itcvalorunitario'];
					  	$arDados[$c]['itcordem'] 			= $dado['itcordem'];
					  	$arDados[$c]['ptoid'] 				= $dado['ptoid'];
					  	$arDados[$c]['itcidpai'] 			= $dado['itcidpai'];
					  	$arDados[$c]['itcvalorunitario'] 	= $dado['itcvalorunitario'];
					  	$arDados[$c]['ppoid'] 				= $dado['ppoid'];
					  	$arDados[$c]['preid'] 				= $dado['preid'];
					  	$arDados[$c]['itctipofundacao'] 	= $dado['itctipofundacao'];

					  	
					  	$arDados[$c]['total'] 			 	= 100;
					}
					//ver($arDados, d);
					return simec_json_encode($arDados);
				}
				
			  	
			}
			
		} catch (Exception $e){
	
			# Erro 404 página not found
			if($e->getCode() == 404){
				//echo "Temporariamente indisponível.Favor tente mais tarde.".'\n';
				/* -- ARRUMAR DEPOIS 
				echo '<div style=" border: 1px solid #B7B7B7; font-size: 10px; font-style: normal; font-family: arial; padding: 5px 5px 5px 5px;"> 
							ERRO AO CONSULTAR ITENS DO PREGÃO (SIGARP)
							<div style=" border-top: 1px solid #B7B7B7; padding-top: 5px; " >
								Temporariamente indisponível.Favor tente mais tarde.
							</div>
						</div>
					 	<br>
						';
				*/
				
			}
			$erroMSG = str_replace(array(chr(13),chr(10)), ' ',$e->getMessage());
			$erroMSG = str_replace( "'", '"', $erroMSG );
			/* -- ARRUMAR DEPOIS 
			echo '<div style=" border: 1px solid #B7B7B7; font-size: 10px; font-style: normal; font-family: arial; padding: 5px 5px 5px 5px;"> 
							ERRO AO CONSULTAR ITENS DO PREGÃO (SIGARP)
							<div style=" border-top: 1px solid #B7B7B7; padding-top: 5px; " >
								Erro-WS SIGARP: '.$erroMSG.'
								<br>
								<pre>
								'.print_r($e).'
								</pre>
							</div>
						</div>
					 	<br>
						';
			*/
			
			//echo "Erro-WS SIGARP: $erroMSG";
			//echo "<pre>";
			//print_r($e);
			
		}
	}
	
	public function solicitarItemObra($id, $proid, $tipo = 'pac'){
		global $db;
		
		try {
			
			$data_created = $this->getDataAtual();
	
			$usuario = USUARIO_SIGARP;
			$senha   = SENHA_SIGARP;
			
			if( $tipo == 'pac' ){
				$dados = $this->db->pegaLinha("SELECT pronumeroprocesso, procnpj FROM par.processoobra WHERE prostatus = 'A' and  proid = ".$proid);
				$dadosTemo = $this->db->pegaLinha("SELECT terid as documento FROM par.termocompromissopac WHERE terid = ".$id);
				$arrObras = $this->db->carregarColuna("SELECT DISTINCT
														tpc.preid 
													FROM 
														par.termoobraspaccomposicao tpc 
													INNER JOIN obras.preobra pre ON pre.preid = tpc.preid
													INNER JOIN obras.pretipoobra pto ON pto.ptoid = pre.ptoid AND ptocategoria IS NOT NULL
													WHERE 
														tpc.terid = ".$id);
			} else {
				$dados = $this->db->pegaLinha("SELECT pronumeroprocesso, procnpj FROM par.processoobraspar WHERE prostatus = 'A' and proid = ".$proid);
				$dadosTemo = $this->db->pegaLinha("SELECT dopnumerodocumento as documento FROM par.documentopar WHERE dopid =".$id);
				$arrObras = $this->db->carregarColuna("SELECT DISTINCT
														tpc.preid 
													FROM 
														par.termocomposicao tpc 
													INNER JOIN obras.preobra pre ON pre.preid = tpc.preid
													INNER JOIN obras.pretipoobra pto ON pto.ptoid = pre.ptoid AND ptocategoria IS NOT NULL
													WHERE 
														tpc.dopid = ".$id);
			}
			
			$cnpj = $dados['procnpj'];
			$processo = $dados['pronumeroprocesso'];
			
			
			
$arqXmlSt = 
'<?xml version="1.0" encoding="ISO-8859-1"?>
<request>
	<header>
		<app>SIGARP</app>
		<version>1.0</version>
		<created>'.$data_created.'</created>
	</header>
	<body>
		<auth>
			<usuario>'.$usuario.'</usuario>
			<senha>'.$senha.'</senha>
		</auth>
		<params>
			<cnpj>'.$cnpj.'</cnpj>
			<nu_processo>'.$processo.'</nu_processo>
			<nu_seq_tipo_pagamento>1</nu_seq_tipo_pagamento>
			<nu_termo_compromisso>'.$dadosTemo['documento'].'</nu_termo_compromisso>';
			foreach( $arrObras as $obra ){
				$sql = "select 
							itc.itcitemsigarp as id,
							itc.itcquantidade as qtd,
							itc.itcvalorunitario as valor
						from 
							obras.preitenscomposicaomi itc
						INNER JOIN obras.preobra pre ON pre.preid = itc.preid AND itc.ptoid = pre.ptoid
						where 
							itc.preid = ".$obra." AND
							itc.itcquantidade is not null AND
							itc.itcquantidade > 0";
				$itens = $this->db->carregar($sql);
				$itens = $itens ? $itens : array();
				
				$dadosEndereco = array();
				$sql = "SELECT DISTINCT
							obr.obrid,
							pre.precep,
							pre.prelogradouro,
							pre.prenumero,
							pre.precomplemento,
							pre.prebairro,
							pre.estuf,
							mun.mundescricao,
							pre.muncod,
							pre.prelatitude,
							pre.prelongitude
						FROM
							obras.preobra pre
						INNER JOIN obras2.obras 			obr ON obr.preid = pre.preid AND obrstatus = 'A' AND obridpai IS NULL
						INNER JOIN territorios.municipio 	mun ON mun.muncod = pre.muncod
						WHERE
							pre.preid = ".$obra;
				$dadosEndereco = $this->db->pegaLinha($sql);
				
				if($dadosEndereco['obrid']){
				
					$arqXmlSt .= '<obra>    
						<nu_pre_id>'.$obra.'</nu_pre_id>
						<nu_id_obra>'.$dadosEndereco['obrid'].'</nu_id_obra>
						<endereco>
							<cep>'.$dadosEndereco['precep'].'</cep>
							<logradouro>'.$dadosEndereco['prelogradouro'].'</logradouro>
							<numero>'.$dadosEndereco['prenumero'].'</numero>
							<complemento>'.$dadosEndereco['precomplemento'].'</complemento>
							<bairro>'.$dadosEndereco['prebairro'].'</bairro>
							<uf>'.$dadosEndereco['estuf'].'</uf>
							<municipio>'.$dadosEndereco['mundescricao'].'</municipio>
							<cod_ibge>'.$dadosEndereco['muncod'].'</cod_ibge>
							<latitude>'.$dadosEndereco['prelatitude'].'</latitude>
							<longitude>'.$dadosEndereco['prelongitude'].'</longitude>
						</endereco>';
						foreach( $itens as $item ){
							$pregao = $this->diponibilizaPregoesItensObra($item['id'], 'pregao', $obra);
							if($pregao){
								$it++;
								$arqXmlSt .= '<item>
												<nu_seq_pregao>'.$pregao.'</nu_seq_pregao>
												<nu_seq_item>'.$item['id'].'</nu_seq_item>
												<qt_item>'.$item['qtd'].'</qt_item>
												<vl_pagamento>'.$item['valor'].'</vl_pagamento>
											</item>';
								
							} else {
								$it++;
								$arqXmlSt .= '<item>
												<nu_seq_pregao></nu_seq_pregao>
												<nu_seq_item>'.$item['id'].'</nu_seq_item>
												<qt_item>'.$item['qtd'].'</qt_item>
												<vl_pagamento></vl_pagamento>
											</item>';
								
								$itPerdido[] = array('preid' => $obra,
													'processo' => $processo,
													'item' => $item['id'],
													'qtd' => $item['qtd'],
													'motivo' => 'Falta o pregão');
							}
						}
					$arqXmlSt .= '</obra>';
				} else {
					foreach( $itens as $item ){
						$itPerdido[] = array('preid' => $obra,
											'processo' => $processo,
											'item' => $item['id'],
											'qtd' => $item['qtd'],
											'motivo' => 'Falta a Obra');
					}
				}
			}	
		$arqXmlSt .= '</params>
	</body>
</request>';

$arqXml = <<<XML
{$arqXmlSt}
XML;

		if( $it == 0 ){ //Não tem nenhum item para enviar!
			if( is_array($itPerdido) ){
				foreach( $itPerdido as $it ){
					$sqlInsertItem .= " INSERT INTO par.enviomiitemperdido (
								preid,
								processo,
								emiitem,
								emiqtd,
								emimotivo )
							VALUES
								(".$it['preid'].",
								'".$processo."',
								".$it['item'].",
								".$it['qtd'].",
								'".$it['motivo']."'); ";
				}
				$this->db->executar($sqlInsertItem);
				$this->db->commit();
			}
			return true;
		}

	/*	
	echo '<pre>';		
	echo simec_htmlentities($arqXml);
	die();
		*/	  	
			if( $_SESSION['baselogin'] == "simec_desenvolvimento" || $_SESSION['baselogin'] == "simec_espelho_producao" ){
				//$urlWS = 'http://hmg.fnde.gov.br/webservices/wssigarp/ws/categoria';
				$urlWS = 'http://172.20.200.115/webservices/wssigarp/ws/item';
			//	$urlWS = 'http://www.fnde.gov.br/webservices/wssigarp/ws/item';
			} else {
				$urlWS = 'http://www.fnde.gov.br/webservices/wssigarp/ws/item';
			}

			$xml = Fnde_Webservice_Client::CreateRequest()
					->setURL($urlWS)
					->setParams( array('xml' => $arqXml, 'method' => 'solicitarobra') )
					->execute();
						
			$xmlRetorno = $xml;
			$xml = simplexml_load_string( stripslashes($xml) );

			$result = (integer) $xml->status->result;

			if(!$result) { // Erro
				foreach( $arrObras as $obra ){
					$sql = "INSERT INTO par.historicowspreobra(
					    	preid,
					    	hwpwebservice,
					    	hwpxmlenvio,
					    	hwpxmlretorno,
					    	hwpdataenvio,
					        usucpf)
					    VALUES ('".$obra."',
					    		'solicitarItemObra - Erro',
					    		'".addslashes($arqXml)."',
					    		'".addslashes($xmlRetorno)."',
					    		NOW(),
					            '".$_SESSION['usucpf']."');";
	
					$this->db->executar($sql);
					$this->db->commit();
					
					if( (integer) $xml->status->error->message->code == 25 ){ // Fnde: Já existe processo para esse número de processo. Utilizar o método reformular.
						$this->solicitarItemObraReformulacao($id, $proid, $tipo);
						return;
					}
					
					$sql = "select 
								itc.itcitemsigarp as id,
								itc.itcquantidade as qtd
							from 
								obras.preitenscomposicaomi itc
							where 
								itc.preid = ".$obra." AND
								itc.itcquantidade is not null AND
								itc.itcquantidade > 0";
					
					$itens = $this->db->carregar($sql);
					$itens = $itens ? $itens : array();
					$sqlInsertItem = "";
					foreach($itens as $ite){
						$sqlInsertItem .= " INSERT INTO par.enviomiitemperdido (
												preid,
												processo,
												emiitem,
												emiqtd,
												emimotivo )
											VALUES
												(".$obra.",
												'".$processo."',
												".$ite['id'].",
												".$ite['qtd'].",
												'Erro na conexão'); ";
					}
					$this->db->executar($sqlInsertItem);
					
				}
				$this->db->commit();
				
			} else { // Sucesso
				
				$processo = $xml->body->params->nu_processo;
				$cnpj = $xml->body->params->cnpj;
				foreach( $xml->body->params->adesaopregao as $adesao ){
					$seq_adesao = $adesao->nu_seq_solicitacao_adesao;
					$preid = $adesao->nu_obra;
					foreach( $adesao->item->nu_seq_item as $item ){
						$seq_item = $item;
						$sqlUpdate .= "UPDATE par.adesaoobraspac SET aopstatus = 'I' WHERE preid = ".$preid."; ";
						$sqlInsert .= "INSERT INTO par.adesaoobraspac ( aopadesao, preid, aopitem, aopcnpj, aopprocesso, aopstatus ) VALUES (".$seq_adesao.",".$preid.",".$seq_item.",'".$cnpj."','".$processo."', 'A'); ";
					}
				}
				
				// verifico se tem alguma adesao de obra para ser cancelada
				if($xml->body->params->canceladas){
					foreach( $xml->body->params->canceladas->nu_seq_solicitacao_adesao as $adesaocancelada ){
						$sqlCancelamento .= "UPDATE par.adesaoobraspac SET aopstatus = 'I' WHERE aopadesao = ".$adesaocancelada."; ";
					}
					$this->db->executar($sqlCancelamento);
				}

				$this->db->executar($sqlUpdate);
				$this->db->executar($sqlInsert);
				
				foreach( $arrObras as $obra ){
					$sql = "INSERT INTO par.historicowspreobra(
					    	preid,
					    	hwpwebservice,
					    	hwpxmlenvio,
					    	hwpxmlretorno,
					    	hwpdataenvio,
					        usucpf)
					    VALUES ('".$obra."',
					    		'solicitarItemObra - Sucesso',
					    		'".addslashes($arqXml)."',
					    		'".addslashes($xmlRetorno)."',
					    		NOW(),
					            '".$_SESSION['usucpf']."');";
	
					$this->db->executar($sql);
				}
				
				if( is_array($itPerdido) && $itPerdido[0] ){
					foreach( $itPerdido as $it ){
						$sqlInsert .= " INSERT INTO par.enviomiitemperdido (
									preid,
									processo,
									emiitem,
									emiqtd,
									emimotivo )
								VALUES
									(".$it['preid'].",
									'".$processo."',
									".$it['item'].",
									".$it['qtd'].",
									'".$it['motivo']."'); ";
					}
					$this->db->executar($sqlInsert);
					$this->db->commit();
				}
				
				$this->db->commit();
				
			}
			
		} catch (Exception $e){
	
			# Erro 404 página not found
			if($e->getCode() == 404){
				//echo "Temporariamente indisponível.Favor tente mais tarde.".'\n';
				/* -- ARRUMAR DEPOIS 
				echo '<div style=" border: 1px solid #B7B7B7; font-size: 10px; font-style: normal; font-family: arial; padding: 5px 5px 5px 5px;"> 
							ERRO AO CONSULTAR ITENS DO PREGÃO (SIGARP)
							<div style=" border-top: 1px solid #B7B7B7; padding-top: 5px; " >
								Temporariamente indisponível.Favor tente mais tarde.
							</div>
						</div>
					 	<br>
						';
				*/
				
			}
			$erroMSG = str_replace(array(chr(13),chr(10)), ' ',$e->getMessage());
			$erroMSG = str_replace( "'", '"', $erroMSG );
			/* -- ARRUMAR DEPOIS 
			echo '<div style=" border: 1px solid #B7B7B7; font-size: 10px; font-style: normal; font-family: arial; padding: 5px 5px 5px 5px;"> 
							ERRO AO CONSULTAR ITENS DO PREGÃO (SIGARP)
							<div style=" border-top: 1px solid #B7B7B7; padding-top: 5px; " >
								Erro-WS SIGARP: '.$erroMSG.'
								<br>
								<pre>
								'.print_r($e).'
								</pre>
							</div>
						</div>
					 	<br>
						';
			*/
			
			//echo "Erro-WS SIGARP: $erroMSG";
			//echo "<pre>";
			//print_r($e);
			
		}
	}
	
	public function solicitarItemObraReformulacao($id, $proid, $tipo = 'pac'){
		global $db;
		
		try {
			
			$data_created = $this->getDataAtual();
	
			$usuario = USUARIO_SIGARP;
			$senha   = SENHA_SIGARP;
			
			if( $tipo == 'pac' ){
				$dados = $this->db->pegaLinha("SELECT pronumeroprocesso, procnpj FROM par.processoobra WHERE prostatus = 'A' and proid = ".$proid);
				$dadosTemo = $this->db->pegaLinha("SELECT terid as documento FROM par.termocompromissopac WHERE terid = ".$id);
				$arrObras = $this->db->carregarColuna("SELECT 
														tpc.preid 
													FROM 
														par.termoobraspaccomposicao tpc 
													INNER JOIN obras.preobra pre ON pre.preid = tpc.preid
													INNER JOIN obras.pretipoobra pto ON pto.ptoid = pre.ptoid AND ptocategoria IS NOT NULL
													WHERE 
														tpc.terid = ".$id);
			} else {
				$dados = $this->db->pegaLinha("SELECT pronumeroprocesso, procnpj FROM par.processoobraspar WHERE prostatus = 'A' and proid = ".$proid);
				$dadosTemo = $this->db->pegaLinha("SELECT dopnumerodocumento as documento FROM par.documentopar WHERE dopid =".$id);
				$arrObras = $this->db->carregarColuna("SELECT 
														tpc.preid 
													FROM 
														par.termocomposicao tpc 
													INNER JOIN obras.preobra pre ON pre.preid = tpc.preid
													INNER JOIN obras.pretipoobra pto ON pto.ptoid = pre.ptoid AND ptocategoria IS NOT NULL
													WHERE 
														tpc.dopid = ".$id);
			}
			

			$cnpj = $dados['procnpj'];
			$processo = $dados['pronumeroprocesso'];
			
			
$arqXmlSt = 
'<?xml version="1.0" encoding="ISO-8859-1"?>
<request>
	<header>
		<app>SIGARP</app>
		<version>1.0</version>
		<created>'.$data_created.'</created>
	</header>
	<body>
		<auth>
			<usuario>'.$usuario.'</usuario>
			<senha>'.$senha.'</senha>
		</auth>
		<params>
			<cnpj>'.$cnpj.'</cnpj>
			<nu_processo>'.$processo.'</nu_processo>
			<nu_seq_tipo_pagamento>1</nu_seq_tipo_pagamento>
			<nu_termo_compromisso>'.$dadosTemo['documento'].'</nu_termo_compromisso>';
			foreach( $arrObras as $obra ){
				$sql = "select 
							itc.itcitemsigarp as id,
							itc.itcquantidade as qtd,
							itc.itcvalorunitario as valor
						from 
							obras.preitenscomposicaomi itc
						INNER JOIN obras.preobra pre ON pre.preid = itc.preid AND itc.ptoid = pre.ptoid
						where 
							itc.preid = ".$obra." AND
							itc.itcquantidade is not null AND
							itc.itcquantidade > 0";
				$itens = $this->db->carregar($sql);
				$itens = $itens ? $itens : array();
				
				$dadosEndereco = array();
				$sql = "SELECT DISTINCT
							pre.obrid,
							pre.precep,
							pre.prelogradouro,
							pre.prenumero,
							pre.precomplemento,
							pre.prebairro,
							pre.estuf,
							mun.mundescricao,
							pre.muncod,
							pre.prelatitude,
							pre.prelongitude
						FROM
							obras.preobra pre
						INNER JOIN territorios.municipio mun ON mun.muncod = pre.muncod
						WHERE
							pre.preid = ".$obra;
				$dadosEndereco = $this->db->pegaLinha($sql);
				
				if( $dadosEndereco['obrid'] ){
				
					$arqXmlSt .= '<obra>    
						<nu_pre_id>'.$obra.'</nu_pre_id>
						<nu_id_obra>'.$dadosEndereco['obrid'].'</nu_id_obra>
						<endereco>
							<cep>'.$dadosEndereco['precep'].'</cep>
							<logradouro>'.$dadosEndereco['prelogradouro'].'</logradouro>
							<numero>'.$dadosEndereco['prenumero'].'</numero>
							<complemento>'.$dadosEndereco['precomplemento'].'</complemento>
							<bairro>'.$dadosEndereco['prebairro'].'</bairro>
							<uf>'.$dadosEndereco['estuf'].'</uf>
							<municipio>'.$dadosEndereco['mundescricao'].'</municipio>
							<cod_ibge>'.$dadosEndereco['muncod'].'</cod_ibge>
							<latitude>'.$dadosEndereco['prelatitude'].'</latitude>
							<longitude>'.$dadosEndereco['prelongitude'].'</longitude>
						</endereco>';
						foreach( $itens as $item ){
							$pregao = $this->diponibilizaPregoesItensObra($item['id'], 'pregao', $obra);
							if($pregao){
								$it++;
								$arqXmlSt .= '<item>
												<nu_seq_pregao>'.$pregao.'</nu_seq_pregao>
												<nu_seq_item>'.$item['id'].'</nu_seq_item>
												<qt_item>'.$item['qtd'].'</qt_item>
												<vl_pagamento>'.$item['valor'].'</vl_pagamento>
												<acao>A</acao>
											</item>';
							} else {
								$it++;
								$arqXmlSt .= '<item>
												<nu_seq_pregao></nu_seq_pregao>
												<nu_seq_item>'.$item['id'].'</nu_seq_item>
												<qt_item>'.$item['qtd'].'</qt_item>
												<vl_pagamento></vl_pagamento>
												<acao>A</acao>
											</item>';
								
								$itPerdido[] = array('preid' => $obra,
													'processo' => $processo,
													'item' => $item['id'],
													'qtd' => $item['qtd'],
													'motivo' => 'Falta o pregão');
							}
						}
					$arqXmlSt .= '</obra>';
				} else {
					foreach( $itens as $item ){
						$itPerdido[] = array('preid' => $obra,
											'processo' => $processo,
											'item' => $item['id'],
											'qtd' => $item['qtd'],
											'motivo' => 'Falta a Obra');
					}
				}
			}	
		$arqXmlSt .= '</params>
	</body>
</request>';

$arqXml = <<<XML
{$arqXmlSt}
XML;

		if( $it == 0 ){ //Não tem nenhum item para enviar!
			foreach( $itPerdido as $it ){
				$sqlInsertItem .= " INSERT INTO par.enviomiitemperdido (
							preid,
							processo,
							emiitem,
							emiqtd,
							emimotivo )
						VALUES
							(".$it['preid'].",
							'".$processo."',
							".$it['item'].",
							".$it['qtd'].",
							'".$it['motivo']."'); ";
			}
			$this->db->executar($sqlInsertItem);
			$this->db->commit();
			return true;
		}

	/*	
	echo '<pre>';		
	echo simec_htmlentities($arqXml);
	die();
		*/	  	
			if( $_SESSION['baselogin'] == "simec_desenvolvimento" || $_SESSION['baselogin'] == "simec_espelho_producao" ){
				//$urlWS = 'http://hmg.fnde.gov.br/webservices/wssigarp/ws/categoria';
				$urlWS = 'http://172.20.200.115/webservices/wssigarp/ws/termo';
			//	$urlWS = 'http://www.fnde.gov.br/webservices/wssigarp/ws/item';
			} else {
				$urlWS = 'http://www.fnde.gov.br/webservices/wssigarp/ws/termo';
			}

			$xml = Fnde_Webservice_Client::CreateRequest()
					->setURL($urlWS)
					->setParams( array('xml' => $arqXml, 'method' => 'obra') )
					->execute();
						
			$xmlRetorno = $xml;
			$xml = simplexml_load_string( stripslashes($xml) );

			$result = (integer) $xml->status->result;

			if(!$result) { // Erro
				foreach( $arrObras as $obra ){
					$sql = "INSERT INTO par.historicowspreobra(
					    	preid,
					    	hwpwebservice,
					    	hwpxmlenvio,
					    	hwpxmlretorno,
					    	hwpdataenvio,
					        usucpf)
					    VALUES ('".$obra."',
					    		'solicitarItemObraReformulacao - Erro',
					    		'".addslashes($arqXml)."',
					    		'".addslashes($xmlRetorno)."',
					    		NOW(),
					            '".$_SESSION['usucpf']."');";
	
					$this->db->executar($sql);
					$this->db->commit();
					
					if( (integer) $xml->status->error->message->code == 4 ){ // Fnde: Processo não encontrado.
						$this->solicitarItemObra($id, $proid, $tipo);
						return;
					}
					
					$sql = "select 
							itc.itcitemsigarp as id,
							itc.itcquantidade as qtd
						from 
							obras.preitenscomposicaomi itc
						where 
							itc.preid = ".$obra." AND
							itc.itcquantidade is not null AND
							itc.itcquantidade > 0";
					
					$itens = $this->db->carregar($sql);
					$itens = $itens ? $itens : array();
					$sqlInsertItem = "";
					foreach($itens as $ite){
						$sqlInsertItem .= " INSERT INTO par.enviomiitemperdido (
												preid,
												processo,
												emiitem,
												emiqtd,
												emimotivo )
											VALUES
												(".$obra.",
												'".$processo."',
												".$ite['id'].",
												".$ite['qtd'].",
												'Erro na conexão'); ";
					}
					$this->db->executar($sqlInsertItem);
					
				}
				$this->db->commit();
				
			} else { // Sucesso
				
				$processo = $xml->body->params->nu_processo;
				$cnpj = $xml->body->params->cnpj;
				foreach( $xml->body->params->adesaopregao as $adesao ){
					$seq_adesao = $adesao->nu_seq_solicitacao_adesao;
					$preid = $adesao->nu_obra;
					foreach( $adesao->item->nu_seq_item as $item ){
						$seq_item = $item;
						$sqlUpdate .= "UPDATE par.adesaoobraspac SET aopstatus = 'I' WHERE preid = ".$preid."; ";
						$sqlInsert .= "INSERT INTO par.adesaoobraspac ( aopadesao, preid, aopitem, aopcnpj, aopprocesso, aopstatus ) VALUES (".$seq_adesao.",".$preid.",".$seq_item.",'".$cnpj."','".$processo."', 'A'); ";
					}
				}
				
				// verifico se tem alguma adesao de obra para ser cancelada
				if($xml->body->params->canceladas){
					foreach( $xml->body->params->canceladas->nu_seq_solicitacao_adesao as $adesaocancelada ){
						$sqlCancelamento .= "UPDATE par.adesaoobraspac SET aopstatus = 'I' WHERE aopadesao = ".$adesaocancelada."; ";
					}
					$this->db->executar($sqlCancelamento);
				}

				$this->db->executar($sqlUpdate);
				$this->db->executar($sqlInsert);
				
				foreach( $arrObras as $obra ){
					$sql = "INSERT INTO par.historicowspreobra(
					    	preid,
					    	hwpwebservice,
					    	hwpxmlenvio,
					    	hwpxmlretorno,
					    	hwpdataenvio,
					        usucpf)
					    VALUES ('".$obra."',
					    		'solicitarItemObraReformulacao - Sucesso',
					    		'".addslashes($arqXml)."',
					    		'".addslashes($xmlRetorno)."',
					    		NOW(),
					            '".$_SESSION['usucpf']."');";
	
					$this->db->executar($sql);
				}
				
				if( is_array($itPerdido) && $itPerdido[0] ){
					foreach( $itPerdido as $it ){
						$sqlInsert .= " INSERT INTO par.enviomiitemperdido (
									preid,
									processo,
									emiitem,
									emiqtd,
									emimotivo )
								VALUES
									(".$it['preid'].",
									'".$processo."',
									".$it['item'].",
									".$it['qtd'].",
									'".$it['motivo']."'); ";
					}
					$this->db->executar($sqlInsert);
					$this->db->commit();
				}
				
				$this->db->commit();
				
			}
			
		} catch (Exception $e){
	
			# Erro 404 página not found
			if($e->getCode() == 404){
				//echo "Temporariamente indisponível.Favor tente mais tarde.".'\n';
				/* -- ARRUMAR DEPOIS 
				echo '<div style=" border: 1px solid #B7B7B7; font-size: 10px; font-style: normal; font-family: arial; padding: 5px 5px 5px 5px;"> 
							ERRO AO CONSULTAR ITENS DO PREGÃO (SIGARP)
							<div style=" border-top: 1px solid #B7B7B7; padding-top: 5px; " >
								Temporariamente indisponível.Favor tente mais tarde.
							</div>
						</div>
					 	<br>
						';
				*/
				
			}
			$erroMSG = str_replace(array(chr(13),chr(10)), ' ',$e->getMessage());
			$erroMSG = str_replace( "'", '"', $erroMSG );
			/* -- ARRUMAR DEPOIS 
			echo '<div style=" border: 1px solid #B7B7B7; font-size: 10px; font-style: normal; font-family: arial; padding: 5px 5px 5px 5px;"> 
							ERRO AO CONSULTAR ITENS DO PREGÃO (SIGARP)
							<div style=" border-top: 1px solid #B7B7B7; padding-top: 5px; " >
								Erro-WS SIGARP: '.$erroMSG.'
								<br>
								<pre>
								'.print_r($e).'
								</pre>
							</div>
						</div>
					 	<br>
						';
			*/
			
			//echo "Erro-WS SIGARP: $erroMSG";
			//echo "<pre>";
			//print_r($e);
			
		}
	}
	
	public function solicitarManualmenteItemObra($id, $proid, $tipo = 'pac'){
		global $db;
		
		try {
			
			$data_created = $this->getDataAtual();
	
			$usuario = USUARIO_SIGARP;
			$senha   = SENHA_SIGARP;

			if( $tipo == 'pac' ){
				$dados = $this->db->pegaLinha("SELECT pronumeroprocesso, procnpj FROM par.processoobra WHERE prostatus = 'A' and  proid = ".$proid);
				$dadosTemo = $this->db->pegaLinha("SELECT terid as documento FROM par.termocompromissopac WHERE terid = ".$id);
				$arrObras = $this->db->carregarColuna("SELECT DISTINCT
														tpc.preid 
													FROM 
														par.termoobraspaccomposicao tpc 
													INNER JOIN obras.preobra pre ON pre.preid = tpc.preid
													INNER JOIN obras.pretipoobra pto ON pto.ptoid = pre.ptoid AND ptocategoria IS NOT NULL
													INNER JOIN par.enviomiitemperdido emi ON emi.preid = tpc.preid
													WHERE 
														tpc.terid = ".$id);
			} else {
				$dados = $this->db->pegaLinha("SELECT pronumeroprocesso, procnpj FROM par.processoobraspar WHERE prostatus = 'A' and proid = ".$proid);
				$dadosTemo = $this->db->pegaLinha("SELECT dopnumerodocumento as documento FROM par.documentopar WHERE dopid =".$id);
				$arrObras = $this->db->carregarColuna("SELECT DISTINCT
														tpc.preid 
													FROM 
														par.termocomposicao tpc 
													INNER JOIN obras.preobra pre ON pre.preid = tpc.preid
													INNER JOIN obras.pretipoobra pto ON pto.ptoid = pre.ptoid AND ptocategoria IS NOT NULL
													INNER JOIN par.enviomiitemperdido emi ON emi.preid = tpc.preid
													WHERE 
														tpc.dopid = ".$id);
			}
			
			$cnpj = $dados['procnpj'];
			$processo = $dados['pronumeroprocesso'];
			
			
$arqXmlSt = 
'<?xml version="1.0" encoding="ISO-8859-1"?>
<request>
	<header>
		<app>SIGARP</app>
		<version>1.0</version>
		<created>'.$data_created.'</created>
	</header>
	<body>
		<auth>
			<usuario>'.$usuario.'</usuario>
			<senha>'.$senha.'</senha>
		</auth>
		<params>
			<cnpj>'.$cnpj.'</cnpj>
			<nu_processo>'.$processo.'</nu_processo>
			<nu_seq_tipo_pagamento>1</nu_seq_tipo_pagamento>
			<nu_termo_compromisso>'.$dadosTemo['documento'].'</nu_termo_compromisso>';
			foreach( $arrObras as $obra ){
				$sql = "select distinct
							emi.emiitem as id,
							emi.emiqtd as qtd, 
							itc.itcvalorunitario as valor
						from 
							par.enviomiitemperdido emi
						inner join obras.preitenscomposicaomi itc on itc.itcitemsigarp = emi.emiitem AND emi.preid = itc.preid AND itc.itcquantidade = emi.emiqtd AND itc.itcstatus = 'A'
						where 
							emi.preid = ".$obra;
				$itens = $this->db->carregar($sql);
				$itens = $itens ? $itens : array();
				
				$dadosEndereco = array();
				$sql = "SELECT DISTINCT
							pre.obrid,
							pre.precep,
							pre.prelogradouro,
							pre.prenumero,
							pre.precomplemento,
							pre.prebairro,
							pre.estuf,
							mun.mundescricao,
							pre.muncod,
							pre.prelatitude,
							pre.prelongitude
						FROM
							obras.preobra pre
						INNER JOIN territorios.municipio mun ON mun.muncod = pre.muncod
						WHERE
							pre.preid = ".$obra;
				$dadosEndereco = $this->db->pegaLinha($sql);
				
				if( $dadosEndereco['obrid'] && (is_array( $itens ) && $itens[0])){
				
					$arqXmlSt .= '<obra>
						<nu_pre_id>'.$obra.'</nu_pre_id>
						<nu_id_obra>'.$dadosEndereco['obrid'].'</nu_id_obra>
						<endereco>
							<cep>'.$dadosEndereco['precep'].'</cep>
							<logradouro>'.$dadosEndereco['prelogradouro'].'</logradouro>
							<numero>'.$dadosEndereco['prenumero'].'</numero>
							<complemento>'.$dadosEndereco['precomplemento'].'</complemento>
							<bairro>'.$dadosEndereco['prebairro'].'</bairro>
							<uf>'.$dadosEndereco['estuf'].'</uf>
							<municipio>'.$dadosEndereco['mundescricao'].'</municipio>
							<cod_ibge>'.$dadosEndereco['muncod'].'</cod_ibge>
							<latitude>'.$dadosEndereco['prelatitude'].'</latitude>
							<longitude>'.$dadosEndereco['prelongitude'].'</longitude>
						</endereco>';
						foreach( $itens as $item ){
							$pregao = $this->diponibilizaPregoesItensObra($item['id'], 'pregao', $obra);
							if($pregao){
								$it++;
								$arqXmlSt .= '<item>
												<nu_seq_pregao>'.$pregao.'</nu_seq_pregao>
												<nu_seq_item>'.$item['id'].'</nu_seq_item>
												<qt_item>'.$item['qtd'].'</qt_item>
												<vl_pagamento>'.$item['valor'].'</vl_pagamento>
											</item>';
							} else {
								$it++;
								$arqXmlSt .= '<item>
												<nu_seq_pregao></nu_seq_pregao>
												<nu_seq_item>'.$item['id'].'</nu_seq_item>
												<qt_item>'.$item['qtd'].'</qt_item>
												<vl_pagamento></vl_pagamento>
											</item>';
								
								$itPerdido[] = array('preid' => $obra,
													'processo' => $processo,
													'item' => $item['id'],
													'qtd' => $item['qtd'],
													'motivo' => 'Falta o pregão');
							}
						}
					$arqXmlSt .= '</obra>';
				} else {
					foreach( $itens as $item ){
						$itPerdido[] = array('preid' => $obra,
											'processo' => $processo,
											'item' => $item['id'],
											'qtd' => $item['qtd'],
											'motivo' => 'Falta a Obra');
					}
				}
			}	
		$arqXmlSt .= '</params>
	</body>
</request>';

$arqXml = <<<XML
{$arqXmlSt}
XML;

		if( $it == 0 ){
			return "Nenhum item para enviar.";
		}

	/*	
	echo '<pre>';		
	echo simec_htmlentities($arqXml);
	die();
		*/	  	
			if( $_SESSION['baselogin'] == "simec_desenvolvimento" || $_SESSION['baselogin'] == "simec_espelho_producao" ){
				//$urlWS = 'http://hmg.fnde.gov.br/webservices/wssigarp/ws/categoria';
				$urlWS = 'http://172.20.200.115/webservices/wssigarp/ws/item';
			//	$urlWS = 'http://www.fnde.gov.br/webservices/wssigarp/ws/item';
			} else {
				$urlWS = 'http://www.fnde.gov.br/webservices/wssigarp/ws/item';
			}

			$xml = Fnde_Webservice_Client::CreateRequest()
					->setURL($urlWS)
					->setParams( array('xml' => $arqXml, 'method' => 'solicitarobra') )
					->execute();
						
			$xmlRetorno = $xml;
			$xml = simplexml_load_string( stripslashes($xml) );

			$result = (integer) $xml->status->result;

			if(!$result) { // Erro
				foreach( $arrObras as $obra ){
					$sql = "INSERT INTO par.historicowspreobra(
					    	preid,
					    	hwpwebservice,
					    	hwpxmlenvio,
					    	hwpxmlretorno,
					    	hwpdataenvio,
					        usucpf)
					    VALUES ('".$obra."',
					    		'solicitarItemObraManual - Erro',
					    		'".addslashes($arqXml)."',
					    		'".addslashes($xmlRetorno)."',
					    		NOW(),
					            '".$_SESSION['usucpf']."');";
	
					$this->db->executar($sql);
					
				}
				$this->db->commit();
					
				if( (integer) $xml->status->error->message->code == 25 ){ // Fnde: Já existe processo para esse número de processo. Utilizar o método reformular.
					$this->solicitarManualmenteItemObraReformulacao($id, $proid, $tipo);
					return;
				}
				
				return $xml->status->error->message->text;
				
			} else { // Sucesso
				
				$processo = $xml->body->params->nu_processo;
				$cnpj = $xml->body->params->cnpj;
				foreach( $xml->body->params->adesaopregao as $adesao ){
					$seq_adesao = $adesao->nu_seq_solicitacao_adesao;
					$preid = $adesao->nu_obra;
					foreach( $adesao->item->nu_seq_item as $item ){
						$seq_item = $item;
						$sqlUpdate .= "UPDATE par.adesaoobraspac SET aopstatus = 'I' WHERE preid = ".$preid."; ";
						$sqlInsert .= "INSERT INTO par.adesaoobraspac ( aopadesao, preid, aopitem, aopcnpj, aopprocesso, aopstatus ) VALUES (".$seq_adesao.",".$preid.",".$seq_item.",'".$cnpj."','".$processo."', 'A'); ";
					}
				}
				
				// verifico se tem alguma adesao de obra para ser cancelada
				if($xml->body->params->canceladas){
					foreach( $xml->body->params->canceladas->nu_seq_solicitacao_adesao as $adesaocancelada ){
						$sqlCancelamento .= "UPDATE par.adesaoobraspac SET aopstatus = 'I' WHERE aopadesao = ".$adesaocancelada."; ";
					}
					$this->db->executar($sqlCancelamento);
				}

				$this->db->executar($sqlUpdate);
				$this->db->executar($sqlInsert);
				
				foreach( $arrObras as $obra ){
					$sql = "INSERT INTO par.historicowspreobra(
					    	preid,
					    	hwpwebservice,
					    	hwpxmlenvio,
					    	hwpxmlretorno,
					    	hwpdataenvio,
					        usucpf)
					    VALUES ('".$obra."',
					    		'solicitarItemObraManual - Sucesso',
					    		'".addslashes($arqXml)."',
					    		'".addslashes($xmlRetorno)."',
					    		NOW(),
					            '".$_SESSION['usucpf']."');";
	
					$this->db->executar($sql);
				}
				
				$this->db->executar("DELETE FROM par.enviomiitemperdido WHERE preid IN (".implode(',',$arrObras).")");
				if( $itPerdido[0] ){ //Se tem algum item que não foi!
					foreach( $itPerdido as $it ){
						$sqlInsertItem .= " INSERT INTO par.enviomiitemperdido (
									preid,
									processo,
									emiitem,
									emiqtd,
									emimotivo )
								VALUES
									(".$it['preid'].",
									'".$processo."',
									".$it['item'].",
									".$it['qtd'].",
									'".$it['motivo']."'); ";
					}
					$this->db->executar($sqlInsertItem);
				}
				
				$this->db->commit();
				
				return "Operação realizada com sucesso.";
				
			}
			
		} catch (Exception $e){
	
			# Erro 404 página not found
			if($e->getCode() == 404){
				//echo "Temporariamente indisponível.Favor tente mais tarde.".'\n';
				/* -- ARRUMAR DEPOIS 
				echo '<div style=" border: 1px solid #B7B7B7; font-size: 10px; font-style: normal; font-family: arial; padding: 5px 5px 5px 5px;"> 
							ERRO AO CONSULTAR ITENS DO PREGÃO (SIGARP)
							<div style=" border-top: 1px solid #B7B7B7; padding-top: 5px; " >
								Temporariamente indisponível.Favor tente mais tarde.
							</div>
						</div>
					 	<br>
						';
				*/
				
			}
			$erroMSG = str_replace(array(chr(13),chr(10)), ' ',$e->getMessage());
			$erroMSG = str_replace( "'", '"', $erroMSG );
			/* -- ARRUMAR DEPOIS 
			echo '<div style=" border: 1px solid #B7B7B7; font-size: 10px; font-style: normal; font-family: arial; padding: 5px 5px 5px 5px;"> 
							ERRO AO CONSULTAR ITENS DO PREGÃO (SIGARP)
							<div style=" border-top: 1px solid #B7B7B7; padding-top: 5px; " >
								Erro-WS SIGARP: '.$erroMSG.'
								<br>
								<pre>
								'.print_r($e).'
								</pre>
							</div>
						</div>
					 	<br>
						';
			*/
			
			//echo "Erro-WS SIGARP: $erroMSG";
			//echo "<pre>";
			//print_r($e);
			return "Erro de conexão.";
			
		}
	}
	
	public function solicitarManualmenteItemObraReformulacao($id, $proid, $tipo){
		global $db;
		
		try {
			
			$data_created = $this->getDataAtual();
	
			$usuario = USUARIO_SIGARP;
			$senha   = SENHA_SIGARP;
			
			
			if( $tipo == 'pac' ){
				$dados = $this->db->pegaLinha("SELECT pronumeroprocesso, procnpj FROM par.processoobra WHERE prostatus = 'A' and proid = ".$proid);
				$dadosTemo = $this->db->pegaLinha("SELECT terid as documento FROM par.termocompromissopac WHERE terid = ".$id);
				$arrObras = $this->db->carregarColuna("SELECT DISTINCT
														tpc.preid 
													FROM 
														par.termoobraspaccomposicao tpc 
													INNER JOIN obras.preobra pre ON pre.preid = tpc.preid
													INNER JOIN obras.pretipoobra pto ON pto.ptoid = pre.ptoid AND ptocategoria IS NOT NULL
													--INNER JOIN par.enviomiitemperdido emi ON emi.preid = tpc.preid
													WHERE 
														tpc.terid = ".$id);
			} else {
				$dados = $this->db->pegaLinha("SELECT pronumeroprocesso, procnpj FROM par.processoobraspar  WHERE prostatus = 'A' and proid = ".$proid);
				$dadosTemo = $this->db->pegaLinha("SELECT dopnumerodocumento as documento FROM par.documentopar WHERE dopid =".$id);
				$arrObras = $this->db->carregarColuna("SELECT DISTINCT
														tpc.preid 
													FROM 
														par.termocomposicao tpc 
													INNER JOIN obras.preobra pre ON pre.preid = tpc.preid
													INNER JOIN obras.pretipoobra pto ON pto.ptoid = pre.ptoid AND ptocategoria IS NOT NULL
													--INNER JOIN par.enviomiitemperdido emi ON emi.preid = tpc.preid
													WHERE 
														tpc.dopid = ".$id);
			}
			

			$cnpj = $dados['procnpj'];
			$processo = $dados['pronumeroprocesso'];
			
			
$arqXmlSt = 
'<?xml version="1.0" encoding="ISO-8859-1"?>
<request>
	<header>
		<app>SIGARP</app>
		<version>1.0</version>
		<created>'.$data_created.'</created>
	</header>
	<body>
		<auth>
			<usuario>'.$usuario.'</usuario>
			<senha>'.$senha.'</senha>
		</auth>
		<params>
			<cnpj>'.$cnpj.'</cnpj>
			<nu_processo>'.$processo.'</nu_processo>
			<nu_seq_tipo_pagamento>1</nu_seq_tipo_pagamento>
			<nu_termo_compromisso>'.$dadosTemo['documento'].'</nu_termo_compromisso>';
			foreach( $arrObras as $obra ){
				$sql = "SELECT DISTINCT
							itc.itcitemsigarp as id,
							itc.itcquantidade as qtd, 
							itc.itcvalorunitario as valor
						FROM 
							obras.preitenscomposicaomi itc
						WHERE 
							itc.itcstatus = 'A' AND
							itc.itcitemsigarp IS NOT NULL AND
							itc.itcquantidade <> 0.00 AND
							itc.preid = ".$obra;
				$itens = $this->db->carregar($sql);
				$itens = $itens ? $itens : array();
				
				$dadosEndereco = array();
				$sql = "SELECT DISTINCT
							pre.obrid,
							pre.precep,
							pre.prelogradouro,
							pre.prenumero,
							pre.precomplemento,
							pre.prebairro,
							pre.estuf,
							mun.mundescricao,
							pre.muncod,
							pre.prelatitude,
							pre.prelongitude
						FROM
							obras.preobra pre
						INNER JOIN territorios.municipio mun ON mun.muncod = pre.muncod
						WHERE
							pre.preid = ".$obra;
				$dadosEndereco = $this->db->pegaLinha($sql);
				
				if( $dadosEndereco['obrid'] ){
				
					$arqXmlSt .= '<obra>    
						<nu_pre_id>'.$obra.'</nu_pre_id>
						<nu_id_obra>'.$dadosEndereco['obrid'].'</nu_id_obra>
						<endereco>
							<cep>'.$dadosEndereco['precep'].'</cep>
							<logradouro>'.$dadosEndereco['prelogradouro'].'</logradouro>
							<numero>'.$dadosEndereco['prenumero'].'</numero>
							<complemento>'.$dadosEndereco['precomplemento'].'</complemento>
							<bairro>'.$dadosEndereco['prebairro'].'</bairro>
							<uf>'.$dadosEndereco['estuf'].'</uf>
							<municipio>'.$dadosEndereco['mundescricao'].'</municipio>
							<cod_ibge>'.$dadosEndereco['muncod'].'</cod_ibge>
							<latitude>'.$dadosEndereco['prelatitude'].'</latitude>
							<longitude>'.$dadosEndereco['prelongitude'].'</longitude>
						</endereco>';
						foreach( $itens as $item ){
							$pregao = $this->diponibilizaPregoesItensObra($item['id'], 'pregao', $obra);
							if($pregao){
								$it++;
								$arqXmlSt .= '<item>
												<nu_seq_pregao>'.$pregao.'</nu_seq_pregao>
												<nu_seq_item>'.$item['id'].'</nu_seq_item>
												<qt_item>'.$item['qtd'].'</qt_item>
												<vl_pagamento>'.$item['valor'].'</vl_pagamento>
												<acao>A</acao>
											</item>';
							} else {
								$it++;
								$arqXmlSt .= '<item>
												<nu_seq_pregao></nu_seq_pregao>
												<nu_seq_item>'.$item['id'].'</nu_seq_item>
												<qt_item>'.$item['qtd'].'</qt_item>
												<vl_pagamento></vl_pagamento>
												<acao>A</acao>
											</item>';
								
								$itPerdido[] = array('preid' => $obra,
													'processo' => $processo,
													'item' => $item['id'],
													'qtd' => $item['qtd'],
													'motivo' => 'Falta o pregão');
							}
						}
					$arqXmlSt .= '</obra>';
				} else {
					foreach( $itens as $item ){
						$itPerdido[] = array('preid' => $obra,
											'processo' => $processo,
											'item' => $item['id'],
											'qtd' => $item['qtd'],
											'motivo' => 'Falta a Obra');
					}
				}
			}	
		$arqXmlSt .= '</params>
	</body>
</request>';

$arqXml = <<<XML
{$arqXmlSt}
XML;

		if( $it == 0 ){
			return "Nenhum item para enviar.";
		}

	/*	
	echo '<pre>';		
	echo simec_htmlentities($arqXml);
	die();
		*/	  	
			if( $_SESSION['baselogin'] == "simec_desenvolvimento" || $_SESSION['baselogin'] == "simec_espelho_producao" ){
				//$urlWS = 'http://hmg.fnde.gov.br/webservices/wssigarp/ws/categoria';
				$urlWS = 'http://172.20.200.115/webservices/wssigarp/ws/termo';
			//	$urlWS = 'http://www.fnde.gov.br/webservices/wssigarp/ws/item';
			} else {
				$urlWS = 'http://www.fnde.gov.br/webservices/wssigarp/ws/termo';
			}

			$xml = Fnde_Webservice_Client::CreateRequest()
					->setURL($urlWS)
					->setParams( array('xml' => $arqXml, 'method' => 'obra') )
					->execute();
						
			$xmlRetorno = $xml;
			$xml = simplexml_load_string( stripslashes($xml) );

			$result = (integer) $xml->status->result;

			if(!$result) { // Erro
				foreach( $arrObras as $obra ){
					$sql = "INSERT INTO par.historicowspreobra(
					    	preid,
					    	hwpwebservice,
					    	hwpxmlenvio,
					    	hwpxmlretorno,
					    	hwpdataenvio,
					        usucpf)
					    VALUES ('".$obra."',
					    		'solicitarItemObraManualReformulacao - Erro',
					    		'".addslashes($arqXml)."',
					    		'".addslashes($xmlRetorno)."',
					    		NOW(),
					            '".$_SESSION['usucpf']."');";
	
					$this->db->executar($sql);
					
				}
				$this->db->commit();
					
				if( (integer) $xml->status->error->message->code == 4 ){ // Fnde: Processo não encontrado.
					$this->solicitarManualmenteItemObra($id, $proid, $tipo);
					return;
				}
				
				return $xml->status->error->message->text;
				
			} else { // Sucesso
				
				$processo = $xml->body->params->nu_processo;
				$cnpj = $xml->body->params->cnpj;
				foreach( $xml->body->params->adesaopregao as $adesao ){
					$seq_adesao = $adesao->nu_seq_solicitacao_adesao;
					$preid = $adesao->nu_obra;
					foreach( $adesao->item->nu_seq_item as $item ){
						$seq_item = $item;
						$sqlUpdate .= "UPDATE par.adesaoobraspac SET aopstatus = 'I' WHERE preid = ".$preid."; ";
						$sqlInsert .= "INSERT INTO par.adesaoobraspac ( aopadesao, preid, aopitem, aopcnpj, aopprocesso, aopstatus ) VALUES (".$seq_adesao.",".$preid.",".$seq_item.",'".$cnpj."','".$processo."', 'A'); ";
					}
				}
				
				// verifico se tem alguma adesao de obra para ser cancelada
				if($xml->body->params->canceladas){
					foreach( $xml->body->params->canceladas->nu_seq_solicitacao_adesao as $adesaocancelada ){
						$sqlCancelamento .= "UPDATE par.adesaoobraspac SET aopstatus = 'I' WHERE aopadesao = ".$adesaocancelada."; ";
					}
					$this->db->executar($sqlCancelamento);
				}

				$this->db->executar($sqlUpdate);
				$this->db->executar($sqlInsert);
				
				foreach( $arrObras as $obra ){
					$sql = "INSERT INTO par.historicowspreobra(
					    	preid,
					    	hwpwebservice,
					    	hwpxmlenvio,
					    	hwpxmlretorno,
					    	hwpdataenvio,
					        usucpf)
					    VALUES ('".$obra."',
					    		'solicitarItemObraManualReformulacao - Sucesso',
					    		'".addslashes($arqXml)."',
					    		'".addslashes($xmlRetorno)."',
					    		NOW(),
					            '".$_SESSION['usucpf']."');";
	
					$this->db->executar($sql);
				}
				
				$this->db->executar("DELETE FROM par.enviomiitemperdido WHERE preid IN (".implode(',',$arrObras).")");
				if( $itPerdido[0] ){ //Se tem algum item que não foi!
					foreach( $itPerdido as $it ){
						$sqlInsertItem .= " INSERT INTO par.enviomiitemperdido (
									preid,
									processo,
									emiitem,
									emiqtd,
									emimotivo )
								VALUES
									(".$it['preid'].",
									'".$processo."',
									".$it['item'].",
									".$it['qtd'].",
									'".$it['motivo']."'); ";
					}
					$this->db->executar($sqlInsertItem);
				}
				
				$this->db->commit();
				
				return "Operação realizada com sucesso.";
				
			}
			
		} catch (Exception $e){
	
			# Erro 404 página not found
			if($e->getCode() == 404){
				//echo "Temporariamente indisponível.Favor tente mais tarde.".'\n';
				/* -- ARRUMAR DEPOIS 
				echo '<div style=" border: 1px solid #B7B7B7; font-size: 10px; font-style: normal; font-family: arial; padding: 5px 5px 5px 5px;"> 
							ERRO AO CONSULTAR ITENS DO PREGÃO (SIGARP)
							<div style=" border-top: 1px solid #B7B7B7; padding-top: 5px; " >
								Temporariamente indisponível.Favor tente mais tarde.
							</div>
						</div>
					 	<br>
						';
				*/
				
			}
			$erroMSG = str_replace(array(chr(13),chr(10)), ' ',$e->getMessage());
			$erroMSG = str_replace( "'", '"', $erroMSG );
			/* -- ARRUMAR DEPOIS 
			echo '<div style=" border: 1px solid #B7B7B7; font-size: 10px; font-style: normal; font-family: arial; padding: 5px 5px 5px 5px;"> 
							ERRO AO CONSULTAR ITENS DO PREGÃO (SIGARP)
							<div style=" border-top: 1px solid #B7B7B7; padding-top: 5px; " >
								Erro-WS SIGARP: '.$erroMSG.'
								<br>
								<pre>
								'.print_r($e).'
								</pre>
							</div>
						</div>
					 	<br>
						';
			*/
			
			//echo "Erro-WS SIGARP: $erroMSG";
			//echo "<pre>";
			//print_r($e);
			return "Erro de conexão.";
			
		}
	}
	
	function diponibilizaPregoesItensObra($item, $tipo, $entidade = null){
	
		global $db;
		
		try {
			
			$data_created = date("c");
	
			$usuario = USUARIO_SIGARP;
			$senha   = SENHA_SIGARP;
			
			$inuid = $_SESSION['par']['inuid'] ? $_SESSION['par']['inuid'] : $_SESSION['par_var']['inuid'];
			
			if( !$inuid ){
				echo '<script>alert("Falta o INUID!");history.back(-1);</script>';
			}

			$sql = "SELECT
						CASE WHEN estuf IS NOT NULL 
							THEN estuf 
							ELSE mun_estuf 
						END as estuf 
					FROM
						par.instrumentounidade 
					WHERE 
						inuid = ".$inuid;
			$estuf = $this->db->pegaUm( $sql );

$arqXml = <<<XML
<?xml version="1.0" encoding="ISO-8859-1"?>
<request>
    <header>
        <app>SIGARP</app>
        <version>1.0</version>
        <created>$data_created</created>
    </header>
    <body>
        <auth>
            <usuario>$usuario</usuario>
            <senha>$senha</senha>
        </auth>
        <params>
            <nu_seq_item>{$item}</nu_seq_item>
            <uf>{$estuf}</uf>
        </params>
    </body>
</request>
XML;

		 /*
	echo '<pre>';		
	echo simec_htmlentities($arqXml);
	die();
			*/   		
			if( $_SESSION['baselogin'] == "simec_desenvolvimento" || $_SESSION['baselogin'] == "simec_espelho_producao" ){
				$urlWS = 'http://172.20.200.115/webservices/wssigarp/ws/pregao';
			//	$urlWS = 'http://www.fnde.gov.br/webservices/wssigarp/ws/pregao';
			} else {
				$urlWS = 'http://www.fnde.gov.br/webservices/wssigarp/ws/pregao';
			}
	
			$xml = Fnde_Webservice_Client::CreateRequest()
					->setURL($urlWS)
					->setParams( array('xml' => $arqXml, 'method' => 'listar') )
					->execute();
						
			$xmlRetorno = $xml;
			$xml = simplexml_load_string( stripslashes($xml));
	
			$result = (integer) $xml->status->result;
			
			//print_r ( (string) $xml->body->VL_ITEM );
			//die();
	
			if(!$result) {
				
				$sql = "INSERT INTO par.historicowspreobra(
					    	preid,
					    	hwpwebservice,
					    	hwpxmlenvio,
					    	hwpxmlretorno,
					    	hwpdataenvio,
					        usucpf)
					    VALUES (".$entidade.",
					    		'diponibilizaPregoesItensObra - Erro',
					    		'".addslashes($arqXml)."',
					    		'".addslashes($xmlRetorno)."',
					    		NOW(),
					            '".$_SESSION['usucpf']."');";
	
				$this->db->executar($sql);
				$this->db->commit();
				
				$erros = $xml->status->error->message->text;
				/* -- ARRUMAR DEPOIS 
				echo '<div style=" border: 1px solid #B7B7B7; font-size: 10px; font-style: normal; font-family: arial; padding: 5px 5px 5px 5px;"> 
							ERRO AO CONSULTAR ITENS DO PREGÃO (SIGARP)
							<div style=" border-top: 1px solid #B7B7B7; padding-top: 5px; " >
								'.iconv("UTF-8", "ISO-8859-1", $erros).'
							</div>
						</div>
					 	<br>
						';
				*/
				//echo "*** Descrição do erro ***\n\n";
				//$erros = $xml->status->error->message->text;	
				//echo "* ".iconv("UTF-8", "ISO-8859-1", $erros);
			} else { //sucesso
				
				$sql = "INSERT INTO par.historicowspreobra(
					    	preid,
					    	hwpwebservice,
					    	hwpxmlenvio,
					    	hwpxmlretorno,
					    	hwpdataenvio,
					        usucpf)
					    VALUES (".$entidade.",
					    		'diponibilizaPregoesItensObra - Sucesso',
					    		'".addslashes($arqXml)."',
					    		'".addslashes($xmlRetorno)."',
					    		NOW(),
					            '".$_SESSION['usucpf']."');";
	
				$this->db->executar($sql);
				$this->db->commit();
	
				if( $tipo == 'valor' ){
					return $xml->body->VL_ITEM ? $xml->body->VL_ITEM : null;
				} else {
					return (integer) $xml->body->NU_SEQ_PREGAO ? $xml->body->NU_SEQ_PREGAO : false;
				}
			}
			
		} catch (Exception $e){
	
			# Erro 404 página not found
			if($e->getCode() == 404){
				//echo "Temporariamente indisponível.Favor tente mais tarde.".'\n';
				/* -- ARRUMAR DEPOIS 
				echo '<div style=" border: 1px solid #B7B7B7; font-size: 10px; font-style: normal; font-family: arial; padding: 5px 5px 5px 5px;"> 
							ERRO AO CONSULTAR ITENS DO PREGÃO (SIGARP)
							<div style=" border-top: 1px solid #B7B7B7; padding-top: 5px; " >
								Temporariamente indisponível.Favor tente mais tarde.
							</div>
						</div>
					 	<br>
						';
				*/
				
			}
			$erroMSG = str_replace(array(chr(13),chr(10)), ' ',$e->getMessage());
			$erroMSG = str_replace( "'", '"', $erroMSG );
			/* -- ARRUMAR DEPOIS 
			echo '<div style=" border: 1px solid #B7B7B7; font-size: 10px; font-style: normal; font-family: arial; padding: 5px 5px 5px 5px;"> 
							ERRO AO CONSULTAR ITENS DO PREGÃO (SIGARP)
							<div style=" border-top: 1px solid #B7B7B7; padding-top: 5px; " >
								Erro-WS SIGARP: '.$erroMSG.'
								<br>
								<pre>
								'.print_r($e).'
								</pre>
							</div>
						</div>
					 	<br>
						';
			*/
			
			//echo "Erro-WS SIGARP: $erroMSG";
			//echo "<pre>";
			//print_r($e);
			
		}
	}
	
	function consultarSituacaoObra($preid, $tipo = 1){
	
		global $db;
		
		try {
			
			$data_created = date("c");
	
			$usuario = USUARIO_SIGARP;
			$senha   = SENHA_SIGARP;
			
			if( (!$preid) || ($preid == '') || (empty($preid)) ){
				echo '<script>alert("Falta o ID da obra!");history.back(-1);</script>';
			} else {
				// Inativo se existe alguma situação perdida.
				$sql = "UPDATE par.adesaoobraspacsituacaoperdida SET aopstatus = 'I' WHERE preid = ".$preid;
				$this->db->executar($sql);
				$this->db->commit();
			}


$arqXml = <<<XML
<?xml version="1.0" encoding="ISO-8859-1"?>
<request>
    <header>
        <app>SIGARP</app>
        <version>1.0</version>
        <created>$data_created</created>
    </header>
    <body>
        <auth>
            <usuario>$usuario</usuario>
            <senha>$senha</senha>
        </auth>
        <params>
            <nu_obra>$preid</nu_obra>           
			<historico>$tipo</historico> 
        </params>
    </body>
</request>
XML;

		 /*
	echo '<pre>';		
	echo simec_htmlentities($arqXml);
	die();
			*/   		
			if( $_SESSION['baselogin'] == "simec_desenvolvimento" || $_SESSION['baselogin'] == "simec_espelho_producao" ){
				$urlWS = 'http://172.20.200.115/webservices/wssigarp/ws/solicitacao';
			} else {
				$urlWS = 'http://www.fnde.gov.br/webservices/wssigarp/ws/solicitacao';
			}
	
			$xml = Fnde_Webservice_Client::CreateRequest()
					->setURL($urlWS)
					->setParams( array('xml' => $arqXml, 'method' => 'consultarsituacao') )
					->execute();
						
			$xmlRetorno = $xml;
			$xml = simplexml_load_string( stripslashes($xml));
			
			$result = (integer) $xml->status->result;

			if(!$result) {
				
				$sql = "INSERT INTO par.historicowspreobra(
					    	preid,
					    	hwpwebservice,
					    	hwpxmlenvio,
					    	hwpxmlretorno,
					    	hwpdataenvio,
					        usucpf)
					    VALUES (".$preid.",
					    		'consultarSituacaoObra - Erro',
					    		'".addslashes($arqXml)."',
					    		'".addslashes($xmlRetorno)."',
					    		NOW(),
					            '".$_SESSION['usucpf']."');";
	
				$this->db->executar($sql);
				$this->db->commit();
				
				$sqlInsertPerdido = "INSERT INTO par.adesaoobraspacsituacaoperdida ( preid, aopmotivo, aopdata, aopstatus ) VALUES ( ".$preid.", '".utf8_decode((string)$xml->status->error->message->text)."', 'NOW()', 'A' );";
				$this->db->executar($sqlInsertPerdido);
				$this->db->commit();
				
				return;
				//echo $xml->status->error->message->text;
			
			} else { //sucesso
				
				$sql = "INSERT INTO par.historicowspreobra(
					    	preid,
					    	hwpwebservice,
					    	hwpxmlenvio,
					    	hwpxmlretorno,
					    	hwpdataenvio,
					        usucpf)
					    VALUES (".$preid.",
					    		'consultarSituacaoObra - Sucesso',
					    		'".addslashes($arqXml)."',
					    		'".addslashes($xmlRetorno)."',
					    		NOW(),
					            '".$_SESSION['usucpf']."');";
	
				$this->db->executar($sql);
				$this->db->commit();
				
				$sqlInsert = "";
				foreach( $xml->body->params->situacao as $situacao ){
					$sql = "SELECT aosid FROM par.adesaoobraspacsituacao WHERE preid = ".$preid." AND aoscodsituacao = ".(integer)$situacao->COD_SITUACAO_FASE." AND aosdtalteracao = '".$situacao->DT_ALTERACAO_FASE."' AND aosdescsituacao = '".utf8_decode((string)$situacao->SITUACAO_ADESAO)."'";
					$testaSituacao = $this->db->pegaUm( $sql );
					if( !$testaSituacao ){ // Ainda não temos essa situação no banco
						$sqlInsert .= "INSERT INTO par.adesaoobraspacsituacao ( preid, aoscodsituacao, aosdtalteracao, aosdescsituacao, aoscnpjfornecedor, aosnomefornecedor, aosdata ) VALUES ( ".$preid.", '".(integer)$situacao->COD_SITUACAO_FASE."', '".$situacao->DT_ALTERACAO_FASE."', '".utf8_decode((string)$situacao->SITUACAO_ADESAO)."', '".utf8_decode((string)$situacao->CNPJ_FORNECEDOR)."', '".utf8_decode((string)$situacao->NOME_FORNECEDOR)."', 'NOW()' );";
					}
					$testaSituacao = "";
				}
				if( $sqlInsert ){
					$this->db->executar($sqlInsert);
					$this->db->commit();
				}
				
				return;
	
			}
			
		} catch (Exception $e){
	
			# Erro 404 página not found
			if($e->getCode() == 404){
				//echo "Temporariamente indisponível.Favor tente mais tarde.".'\n';
				/* -- ARRUMAR DEPOIS 
				echo '<div style=" border: 1px solid #B7B7B7; font-size: 10px; font-style: normal; font-family: arial; padding: 5px 5px 5px 5px;"> 
							ERRO AO CONSULTAR ITENS DO PREGÃO (SIGARP)
							<div style=" border-top: 1px solid #B7B7B7; padding-top: 5px; " >
								Temporariamente indisponível.Favor tente mais tarde.
							</div>
						</div>
					 	<br>
						';
				*/
				
			}
			$erroMSG = str_replace(array(chr(13),chr(10)), ' ',$e->getMessage());
			$erroMSG = str_replace( "'", '"', $erroMSG );
			/* -- ARRUMAR DEPOIS 
			echo '<div style=" border: 1px solid #B7B7B7; font-size: 10px; font-style: normal; font-family: arial; padding: 5px 5px 5px 5px;"> 
							ERRO AO CONSULTAR ITENS DO PREGÃO (SIGARP)
							<div style=" border-top: 1px solid #B7B7B7; padding-top: 5px; " >
								Erro-WS SIGARP: '.$erroMSG.'
								<br>
								<pre>
								'.print_r($e).'
								</pre>
							</div>
						</div>
					 	<br>
						';
			*/
			
			//echo "Erro-WS SIGARP: $erroMSG";
			//echo "<pre>";
			//print_r($e);
			
		}
	}
	
	function recuperarContrato($preid){
	
		global $db;
		
		try {
			
			$data_created = date("c");
	
			$usuario = USUARIO_SIGARP;
			$senha   = SENHA_SIGARP;
			
			if( !$preid ){
				echo '<script>alert("Falta o ID da obra!");history.back(-1);</script>';
			}
			
			$licid = LICITACAO_MI;


$arqXml = <<<XML
<?xml version="1.0" encoding="ISO-8859-1"?>
<request>
    <header>
        <app>SIGARP</app>
        <version>1.0</version>
        <created>$data_created</created>
    </header>
    <body>
        <auth>
            <usuario>$usuario</usuario>
            <senha>$senha</senha>
        </auth>
        <params>
            <nu_obra>$preid</nu_obra>           
        </params>
    </body>
</request>
XML;

		 /*
	echo '<pre>';		
	echo simec_htmlentities($arqXml);
	die();
			*/   		
			if( $_SESSION['baselogin'] == "simec_desenvolvimento" || $_SESSION['baselogin'] == "simec_espelho_producao" ){
				$urlWS = 'http://172.20.200.115/webservices/wssigarp/ws/solicitacao';
			} else {
				$urlWS = 'http://www.fnde.gov.br/webservices/wssigarp/ws/solicitacao';
			}
	
			$xml = Fnde_Webservice_Client::CreateRequest()
					->setURL($urlWS)
					->setParams( array('xml' => $arqXml, 'method' => 'consultarcontrato') )
					->execute();
						
			$xmlRetorno = $xml;
			$xml = simplexml_load_string( stripslashes($xml));
			
			$result = (integer) $xml->status->result;
			
			if(!$result) {
				
				$sql = "INSERT INTO par.historicowspreobra(
					    	preid,
					    	hwpwebservice,
					    	hwpxmlenvio,
					    	hwpxmlretorno,
					    	hwpdataenvio,
					        usucpf)
					    VALUES (".$preid.",
					    		'recuperarContrato - Erro',
					    		'".addslashes($arqXml)."',
					    		'".addslashes($xmlRetorno)."',
					    		NOW(),
					            '".$_SESSION['usucpf']."');";
	
				$this->db->executar($sql);
				$this->db->commit();
				
//				echo $xml->status->error->message->text;
				return;
			
			} else { //sucesso
				
				$sql = "INSERT INTO par.historicowspreobra(
					    	preid,
					    	hwpwebservice,
					    	hwpxmlenvio,
					    	hwpxmlretorno,
					    	hwpdataenvio,
					        usucpf)
					    VALUES (".$preid.",
					    		'recuperarContrato - Sucesso',
					    		'".addslashes($arqXml)."',
					    		'XML de contrato - Sucesso',
					    		NOW(),
					            '".$_SESSION['usucpf']."');";
	
				$this->db->executar($sql);
				$this->db->commit();
				
				//recupero o contrato:
				$dataAssinatura  = $xml->body->dataAssinatura;
				$dataVencimento  = $xml->body->dataVencimento;
				$terraplanagem   = $xml->body->terraplanagem;
				$numeroContrato  = $xml->body->numeroContrato;
				$nomeArquivo 	 = $xml->body->nomeArquivo;
				$arquivoRaw  	 = base64_decode($xml->body->arquivoRaw);

				$terraplanagem = (string) $terraplanagem == '1' ? 'true' : 'false';
				
				/*$sqlContrato = "INSERT INTO par.adesaoobraspaccontrato (aocdataassinatura, aocdatavencimento, aocterraplanagem, aocnumerocontrato, aocnomearquivo, preid, aocdatainclusao, aocstatus) 
										VALUES ('".(string) $dataAssinatura."','".(string) $dataVencimento."',".$terraplanagem.",'".(string) $numeroContrato."','".(string) $nomeArquivo."','".$preid."','NOW()', 'A'); ";

				$this->db->executar($sqlContrato);
				$this->db->commit();*/
				
				// Atualizo a terraplanagem.
				$sql = "UPDATE obras.preobra SET preterraplanagem = ".$terraplanagem." WHERE preid = ".$preid;
				$this->db->executar($sql);

				$obrid = $this->db->pegaUm("SELECT o.obrid FROM obras2.obras o WHERE o.preid = ".$preid);
				
				if($obrid){
					$sql = "SELECT 
								c.crtid 
							FROM 
								obras2.obrascontrato oc 
							JOIN obras2.contrato c ON c.crtid = oc.crtid AND c.crtstatus = 'A'
							WHERE 
								oc.obrid = ".$obrid." AND 
								c.crtdtassinatura = '".(string) $dataAssinatura."' AND 
								c.crtnumero = '".(string) $numeroContrato."' AND 
								oc.ocrstatus = 'A'";
					$testaContrato = $this->db->pegaUm( $sql );
					if( !$testaContrato ){ // Ainda não temos esse Contrato no banco, pelo menos não com esses campos, então inativo o que tiver e insiro o novo.
						$fp = fopen(APPRAIZ . 'arquivos/obras2/' . $nomeArquivo, "w");
						if ($fp) {
						  stream_set_write_buffer($fp, 0);
						  fwrite($fp, $arquivoRaw);
						  fclose($fp);
						}
						
						include_once APPRAIZ . "includes/classes/fileSimec.class.inc";
						$arrCampos = array();
						$file = new FilesSimec("obrascontrato", $arrCampos, "obras2");
						$file->setMover(APPRAIZ . 'arquivos/obras2/' . $nomeArquivo, 'pdf', false);
						/*
						$file = new FilesSimec("adesaoobraspaccontrato", $arrCampos, "par");
						$file->setMover(APPRAIZ . 'arquivos/obras2/' . $nomeArquivo, 'pdf', false);
						*/
						$arqid = $file->getIdArquivo();
						
						/*
						 * Bloco do Obras2
						 */
						
						$sql = "SELECT 
									o.obrid
								FROM 
									obras2.obras o
								INNER JOIN obras2.obrascontrato oc ON oc.obrid = o.obrid AND oc.ocrstatus = 'A'
								INNER JOIN obras2.contrato c ON c.crtid = oc.crtid AND c.crtstatus = 'A'
								WHERE 
									o.obrid = ".$obrid;
						$verificacaoContrato = $this->db->pegaUm($sql);
						
						$valor = $this->db->pegaUm("SELECT prevalorobra FROM obras.preobra WHERE preid = ".$preid);
						
						if( $verificacaoContrato ){ // Tem contrato
							// Recupero o contrato
							$crtid = $this->db->pegaUm("SELECT crtid FROM obras2.obrascontrato WHERE obrid = ".$obrid);
							
							// Atualizo o valor
							$sql = "UPDATE obras2.obrascontrato SET ocrvalorexecucao = $valor WHERE obrid = ".$obrid;
							$this->db->executar($sql);
							
							// Atualizo o contrato
							$sql = "UPDATE obras2.contrato SET crtdtassinatura = '".(string) $dataAssinatura."', arqidcontrato= ".$arqid.", crtnumero = '".(string) $numeroContrato."' WHERE crtid = ".$crtid;							
							$this->db->executar($sql);
						} else { // Não tem contrato
							//Relaciono com a licitação
							$sql = "INSERT INTO obras2.obralicitacao (obrid, licid, oblstatus) VALUES (".$obrid.",".$licid.",'A')";
							$this->db->executar($sql);
							
							//Insiro o contrato
							$sql = "INSERT INTO obras2.contrato (crtdtassinatura, arqidcontrato, crtnumero, crtstatus, licid) VALUES ('".(string) $dataAssinatura."',".$arqid.",'".(string) $numeroContrato."','A',".$licid.") RETURNING crtid";
							$crtid = $this->db->pegaUm($sql);
							
							//Insiro no obrascontrato
							$sql = "INSERT INTO obras2.obrascontrato (crtid, obrid, ocrvalorexecucao, ocrstatus) VALUES (".$crtid.", ".$obrid.", ".$valor.", 'A')";
							$this->db->executar($sql);
						}
						$this->db->commit();
						
						/*
						 * Fim do bloco do obras2
						 */
						
						/*
						$sqlContrato = "UPDATE par.adesaoobraspaccontrato SET aocstatus = 'I' WHERE preid = ".$preid."; ";
						$sqlContrato .= "INSERT INTO par.adesaoobraspaccontrato (aocdataassinatura, aocdatavencimento, aocterraplanagem, aocnumerocontrato, aocnomearquivo, aocarquivo, arqid, preid, aocdatainclusao, aocstatus) 
										VALUES ('".(string) $dataAssinatura."','".(string) $dataVencimento."','".(string) $terraplanagem."','".(string) $numeroContrato."','".(string) $nomeArquivo."','".(string) $xml->body->arquivoRaw."',".$arqid.",'".$preid."','NOW()', 'A'); ";
						*/
					}
				}
				/*
				if($sqlContrato){
					$this->db->executar($sqlContrato);
					$this->db->commit();
				}
				*/
		//		$arquivo = $file->getDownloadArquivo($arqid);
					
		//		echo 'Sucesso!';
		//		exit;

				return;
	
			}
			
		} catch (Exception $e){
	
			# Erro 404 página not found
			if($e->getCode() == 404){
				//echo "Temporariamente indisponível.Favor tente mais tarde.".'\n';
				/* -- ARRUMAR DEPOIS 
				echo '<div style=" border: 1px solid #B7B7B7; font-size: 10px; font-style: normal; font-family: arial; padding: 5px 5px 5px 5px;"> 
							ERRO AO CONSULTAR ITENS DO PREGÃO (SIGARP)
							<div style=" border-top: 1px solid #B7B7B7; padding-top: 5px; " >
								Temporariamente indisponível.Favor tente mais tarde.
							</div>
						</div>
					 	<br>
						';
				*/
				
			}
			$erroMSG = str_replace(array(chr(13),chr(10)), ' ',$e->getMessage());
			$erroMSG = str_replace( "'", '"', $erroMSG );
			/* -- ARRUMAR DEPOIS 
			echo '<div style=" border: 1px solid #B7B7B7; font-size: 10px; font-style: normal; font-family: arial; padding: 5px 5px 5px 5px;"> 
							ERRO AO CONSULTAR ITENS DO PREGÃO (SIGARP)
							<div style=" border-top: 1px solid #B7B7B7; padding-top: 5px; " >
								Erro-WS SIGARP: '.$erroMSG.'
								<br>
								<pre>
								'.print_r($e).'
								</pre>
							</div>
						</div>
					 	<br>
						';
			*/
			
			//echo "Erro-WS SIGARP: $erroMSG";
			//echo "<pre>";
			//print_r($e);
			
		}
	}
		
	// Envio Manual - Reformulação
	public function aderirPregaoReformulacao( $dados, $tipo = 1, $nivel = 'subacao' ){

		global $db;
		
		//$item = $db->carregarColuna( "SELECT icoid FROM par.empenhopregaoitemperdido WHERE sbdid = ".$dados['sbdid'] );
		
	try {
		
			if( $nivel == 'subacao' ){
				
				$sql = "SELECT
							sd.sbaid,
							sd.sbdano,
							sd.sbdid
						FROM
							par.subacaodetalhe sd
						INNER JOIN par.subacao s ON s.sbaid = sd.sbaid
						WHERE
							sd.sbdid IN (".$dados['sbdid'].") AND s.sbastatus = 'A'";
		
			} else {
				$sql = "SELECT DISTINCT
							sd.sbaid,
							sd.sbdano,
							sd.sbdid
						FROM
							par.subacaodetalhe sd
						INNER JOIN par.subacao s ON s.sbaid = sd.sbaid
						INNER JOIN par.termocomposicao ter ON ter.sbdid = sd.sbdid
						INNER JOIN par.documentopar dp ON dp.dopid = ter.dopid
						--INNER JOIN par.processoparcomposicao ppc ON ppc.sbdid = sd.sbdid and ppc.ppcstatus = 'A'
						WHERE
							dp.prpid = ".$dados['prpid']." AND s.sbastatus = 'A'";
			}
			
			$sbaids = $this->db->carregar($sql);
			
			$arrItens = array();
			foreach( $sbaids as $sba ){
			
				// Verifico os itens da subação
				$sql = "SELECT 
							s.sbaid,
							sd.sbdano,
							sd.sbdid,
							sic.icoid,
							pic.idsigarp,
							CASE WHEN sbacronograma = 1 --global
							THEN
								CASE WHEN sic.icovalidatecnico IS NULL -- antigos
								THEN coalesce(sic.icoquantidade,0)
								ELSE -- novos
									CASE WHEN sic.icovalidatecnico = 'S' THEN -- validado (caso não o item não é contado)
										sum(coalesce(sic.icoquantidadetecnico,0))
									END
								END
							ELSE -- escolas
								par.recuperaquantidadeitemvalidado(sic.icoid)
							END as quantidade,
							sic.icovalor as valor
						FROM
							par.subacao s
						INNER JOIN par.subacaodetalhe 		     sd  ON sd.sbaid = s.sbaid
					--	INNER JOIN par.processopar 	             pr  ON pr.prpid = sd.prpid and pr.prpstatus = 'A'
						LEFT  JOIN par.subacaoitenscomposicao 	 sic ON sic.sbaid = s.sbaid  AND sic.icoano = sd.sbdano AND sic.icostatus <> 'E'
						LEFT  JOIN par.subacaoescolas 		     se  ON se.sbaid = sic.sbaid AND se.sesano = sic.icoano
						LEFT  JOIN par.subescolas_subitenscomposicao ssi ON ssi.sesid = se.sesid AND ssi.icoid = sic.icoid
						INNER JOIN par.propostaitemcomposicao        pic ON pic.picid = sic.picid AND idsigarp is not null
				--		INNER JOIN par.empenhopregaoitemperdido 	 ip  ON ip.icoid = sic.icoid
						WHERE
							s.sbastatus = 'A' AND sd.sbdid IN ( ".$sba['sbdid']." ) AND 
							
							 CASE WHEN sbacronograma = 1 
		                     THEN sic.icovalidatecnico <> 'N' 
		                     ELSE 
		                     	CASE WHEN (s.frmid = 2) OR ( s.frmid = 4 AND s.ptsid = 42 ) OR ( s.frmid = 12 AND s.ptsid = 46 ) THEN
		                        	se.sesvalidatecnico <> 'N'
		                        ELSE
		                        	sic.icovalidatecnico <> 'N' 
		                        END
		                     END
		                group by
							s.sbaid, sd.sbdano, sd.sbdid, sic.icoid, pic.idsigarp, s.sbacronograma, sic.icovalidatecnico, sic.icoquantidade, s.frmid, s.ptsid, se.sesvalidatecnico, sic.icovalor
						ORDER BY
							s.sbaid";
					//		pr.prpid = ".$_SESSION['par_var']['prpid'];
	
				$arrItens[$sba['sbdid']] = $this->db->carregar( $sql );
				
			}
			
			$cnpj = pegaCnpj($dados['inuid'], $dados['prpid']);
	
			$dadosse = $this->db->pegaLinha("SELECT prpnumeroprocesso, prpbanco, prpagencia, muncod, prptipo, inuid, prptipoexecucao FROM par.processopar WHERE prpid='".$dados['prpid']."' and prpstatus = 'A'");
	
			$dopid = $this->db->pegaUm("SELECT dopid FROM par.documentopar WHERE dopstatus = 'A' AND prpid = ".$dados['prpid']);
			
			$testa = buscaDadosTermo( $dopid );
			
			$dadosTemo = $this->db->pegaLinha("SELECT dopnumerodocumento, dopdatafimvigencia FROM par.documentopar WHERE dopstatus = 'A' AND prpid = ".$dados['prpid']);
			if(strlen($dadosTemo['dopdatafimvigencia']) == 7){
				$mes = substr($dadosTemo['dopdatafimvigencia'], 0, 2);
				$ano = substr($dadosTemo['dopdatafimvigencia'], 3, 4);
				$vigencia = $ano."-".$mes;
			} else {
				$mes = substr($dadosTemo['dopdatafimvigencia'], 3, 2);
				$ano = substr($dadosTemo['dopdatafimvigencia'], 6, 4);
				$vigencia = $ano."-".$mes;
			}
			
			$data_created = date("c");
			$usuario = USUARIO_SIGARP;
			$senha   = SENHA_SIGARP;
			
			$itEnviado = array();
			$itPerdido = array();
			$it = 0;
			$cont = 0;
			$montou = false;
			$ultimaSubacao = 0;
			$itens2 = array();

		
$arqXmlSt = 
'<?xml version="1.0" encoding="ISO-8859-1"?>
<request>
    <header>
        <app>SIGARP</app>
        <version>1.0</version>
        <created>'.$data_created.'</created>
    </header>
    <body>
        <auth>
            <usuario>'.$usuario.'</usuario>
            <senha>'.$senha.'</senha>
        </auth>
        <params>
        	<cnpj>'.$cnpj.'</cnpj>
        	<nu_processo>'.$dadosse['prpnumeroprocesso'].'</nu_processo> 
        	<nu_seq_tipo_pagamento>'.$tipo.'</nu_seq_tipo_pagamento>
        	<nu_termo_compromisso>'.$dadosTemo['dopnumerodocumento'].'</nu_termo_compromisso>
        	<dt_vigencia>'.$vigencia.'</dt_vigencia>';
			foreach( $arrItens as $subacao => $itens ){
        		$monta = 0;
				if( $itens[0] ){
		        	foreach( $itens as $item ){
		        		
		        		$subacao = verificaSubacao( $subacao );
							        	
						$sql = "SELECT 
									CASE WHEN sbacronograma = 1 --global
									THEN
										CASE WHEN sic.icovalidatecnico IS NULL -- antigos
										THEN coalesce(sic.icoquantidade,0)
										ELSE -- novos
											CASE WHEN sic.icovalidatecnico = 'S' THEN -- validado (caso não o item não é contado)
												sum(coalesce(sic.icoquantidadetecnico,0))
											END
										END
									ELSE -- escolas
										par.recuperaquantidadeitemvalidado(sic.icoid)
									END as quantidade,
									sic.icovalor,
									s.sbaid
								FROM 
									par.subacaodetalhe sd
								INNER JOIN par.subacao s 				  ON s.sbaidpai = sd.sbaid
								INNER JOIN par.subacaoitenscomposicao sic ON sic.sbaid = s.sbaid and sic.icoano = sd.sbdano AND sic.icovalidatecnico = 'S'
								LEFT  JOIN par.subacaoescolas 		     se  ON se.sbaid = sic.sbaid AND se.sesano = sic.icoano
								LEFT  JOIN par.subescolas_subitenscomposicao ssi ON ssi.sesid = se.sesid AND ssi.icoid = sic.icoid
								INNER JOIN par.propostaitemcomposicao pic ON pic.picid = sic.picid AND idsigarp = ".$item['idsigarp']."
								WHERE 
									sd.sbdid = ".$subacao."
								GROUP BY
									s.sbacronograma, sic.icovalidatecnico, sic.icoquantidade,s.frmid, s.ptsid, se.sesvalidatecnico, sic.icovalor, s.sbaid, sic.icoid
								ORDER BY
									s.sbaid DESC
								LIMIT 1";
						
						$dadosItemAntigo = $this->db->pegaLinha($sql);
						
						
		        		$itens2[$cont]['sbdid'] = $item['sbdid'];
		        		$itens2[$cont]['icoid'] = $item['icoid'];
		        		$cont++;
		        		if($item['idsigarp']){
			        		$pregao = $this->diponibilizaPregoesItens($item['idsigarp'],'pregao',$dados['prpid']);
			        		if( $item['quantidade'] <> 0 ){
			        			
								if( is_array($dadosItemAntigo) ){
									if( $dadosItemAntigo['quantidade'] == $item['quantidade'] && $dadosItemAntigo['icovalor'] == $item['valor'] ){
										$acao = 'I';
									} else {
										$acao = 'A';
									}
								} else {
									$sql = "SELECT count(sbdreformulacao) FROM par.subacaodetalhe WHERE sbdreformulacao IS NOT NULL AND sbdid = ".$subacao;
									$testaReformulacao = $this->db->pegaUm($sql);
									
									if( $testaReformulacao > 0 ){
										$acao = 'A';
									} else {
										$sql = "SELECT idsigarp FROM par.empenhopregaoitensenviados WHERE prpid = ".$dados['prpid']." AND idsigarp = ".$item['idsigarp'];
										$testaItem = $this->db->pegaUm($sql);
										if( $testaItem ){
											$acao = 'I';
										} else {
											$acao = 'A';
										}
									}
								}
				        		if( $pregao || $acao == 'I' ){
				        		
				        			$pregao = $acao == 'I' ? '' : $pregao;
				        			
				        			if( $acao == 'I' ){
				        				$pregao = $db->pegaUm("SELECT epipregao FROM par.empenhopregaoitensenviados  WHERE idsigarp = ".$item['idsigarp']." AND prpid = ".$dados['prpid']." AND epistatus = 'A'");
				        			}
				        			
				        			$valorAtual = $this->diponibilizaPregoesItens($item['idsigarp'],'valor',$dados['prpid']);
									
				        			$it++;
			        				$montou = true;
				        			if( $subacao != $ultimaSubacao && $ultimaSubacao != 0 ){
				        				$arqXmlSt .= '
				        							</sub_acao>';
				        			}
				        			$ultimaSubacao = $subacao;
				        			if( $monta == 0 ){
				        				$monta++;
				        				
				        				$arqXmlSt .= '
				        							<sub_acao>
				        								<nu_sub_acao>'.$subacao.'</nu_sub_acao>';
				        			}
									$arqXmlSt .= '
										<item>
											<nu_seq_pregao>'.$pregao.'</nu_seq_pregao> 
											<nu_seq_item>'.$item['idsigarp'].'</nu_seq_item> 
											<qt_item>'.$item['quantidade'].'</qt_item>  
											<vl_pagamento>'.$item['valor'].'</vl_pagamento>
											<acao>'.$acao.'</acao>
										</item>';
									
									$arItemEnviado[$item['idsigarp']]['pregao'] = $pregao;
									$arItemEnviado[$item['idsigarp']]['valor'] = $item['valor'];
									$arItemEnviado[$item['idsigarp']]['quantidade'] = $item['quantidade'];
									$arItemEnviado[$item['idsigarp']]['icoid'] = $item['icoid'];
									
									$sqlInsertItemPerd .= "INSERT INTO par.empenhopregaoitemperdido ( prpid, sbdid, icoid ) VALUES (".$dados['prpid'].", ".$item['sbdid'].", ".$item['icoid']."); ";
									
									$itEnviado[] = array('sbdid' => $item['sbdid'],
														'icoid' => $item['icoid']);
				        		}
			        		}
		        		}
		        		$pregao = false;
					}
				}
			}
			if( $montou == true ){
        		$arqXmlSt .= '
        					</sub_acao>';
        	}
		$arqXmlSt .= '</params>
    </body>
</request>';

$arqXml = <<<XML
{$arqXmlSt}
XML;
	
			if( $it == 0 ){ //Não tem nenhum item para enviar!
				echo "WS-Sigarp: Nenhum item para enviar. Verifique os pregões.";
				return true;
			} else {
				if( $sqlInsertItemPerd ){
					$sqlDeleta = "DELETE FROM par.empenhopregaoitemperdido WHERE prpid = ".$dados['prpid'];
					$this->db->executar($sqlDeleta);
					$this->db->executar($sqlInsertItemPerd);
					$this->db->commit();
				}
			}
			
			/*
	echo '<pre>';		
	echo simec_htmlentities($arqXml);
	die();
*/

			
			if( $_SESSION['baselogin'] == "simec_desenvolvimento"  || $_SESSION['baselogin'] == "simec_espelho_producao"){
				$urlWS = 'http://172.20.65.98/webservices/wssigarp/ws/termo';
			} else {
				$urlWS = 'http://www.fnde.gov.br/webservices/wssigarp/ws/termo';
			}
			$xml = Fnde_Webservice_Client::CreateRequest()
					->setURL($urlWS)
					->setParams( array('xml' => $arqXml, 'method' => 'subacao') )
					->execute();
						
			$xmlRetorno = $xml;
								
			$xml = simplexml_load_string( stripslashes($xml));
	
			$result = (integer) $xml->status->result;
			
		//	print_r ($xml);
		//	die();
	
			if(!$result) { //ERRO
				
				$sql = "INSERT INTO par.historicowsprocessopar(
					    	prpid,
					    	hwpwebservice,
					    	hwpxmlenvio,
					    	hwpxmlretorno,
					    	hwpdataenvio,
					        usucpf)
					    VALUES ('".$dados['prpid']."',
					    		'enviarManualmenteItensSigarpReformulacao - Erro',
					    		'".addslashes($arqXml)."',
					    		'".addslashes($xmlRetorno)."',
					    		NOW(),
					            '".$_SESSION['usucpf']."');";
	
				$this->db->executar($sql);
				$this->db->commit();
				
				$erros = $xml->status->error->message->text;
				echo '<div style=" border: 1px solid #B7B7B7; font-size: 10px; font-style: normal; font-family: arial; padding: 5px 5px 5px 5px;"> 
							ERRO AO TENTAR ADERIR AO PREGÃO
							<div style=" border-top: 1px solid #B7B7B7; padding-top: 5px; " >
								'.iconv("UTF-8", "ISO-8859-1", $erros).'
							</div>
						</div>
					 	<br>
						';
				
			} else {
				// Deu certo
				
				$sql = "INSERT INTO par.historicowsprocessopar(
					    	prpid,
					    	hwpwebservice,
					    	hwpxmlenvio,
					    	hwpxmlretorno,
					    	hwpdataenvio,
					        usucpf)
					    VALUES ('".$dados['prpid']."',
					    		'enviarManualmenteItensSigarpReformulacao - Sucesso',
					    		'".addslashes($arqXml)."',
					    		'".addslashes($xmlRetorno)."',
					    		NOW(),
					            '".$_SESSION['usucpf']."')
					    RETURNING hwpid;";
	
				$id = $this->db->pegaUm($sql);
	                        
	            // Realiza importacao dos itens que estao no xml do historico.
	            $controle = new ProcessoItemAdesaoPregaoControle();
	            $result = $controle->transferirHistoricoProcesso($id);
	                        
				$this->db->commit();
				
				$dado = ($xml->body->params);
				
				if( is_array($dado->adesaopregao) || is_object($dado->adesaopregao) ){
					foreach( $dado->adesaopregao as $adesao ){
						$dadosSub = $this->db->pegaLinha( "SELECT sbaid, sbdano FROM par.subacaodetalhe WHERE sbdid = ".(string) $adesao->nu_sub_acao);
						if(!$dadosSub){
							$dadosSub = $this->db->pegaLinha( "SELECT sbaid, sbdano FROM par.subacaodetalhe WHERE prpid = ".$dados['prpid']." AND sbaid = ".(string) $adesao->nu_sub_acao);
						}
						$sub = $dadosSub['sbaid'];
						$ano = $dadosSub['sbdano'];
						$ad = (integer) $adesao->nu_seq_solicitacao_adesao;
						foreach( $adesao->item as $item ){
							foreach( $item->nu_seq_item as $it2 ){
								$it = (integer) $it2;
								
								$pregaoEnviado = $arItemEnviado[$it]['pregao'];
								$quantidadeEnviado = $arItemEnviado[$it]['quantidade'];
								$valorEnviado 	= $arItemEnviado[$it]['valor'];
								$icoId			= $arItemEnviado[$it]['icoid'];
								
								
								$sqlUpdate .= "UPDATE par.empenhopregaoitensenviados SET epistatus = 'I' WHERE idsigarp = ".$it." AND prpid = ".$dados['prpid']." AND sbaid = ".$sub." AND epiano = ".$ano."; ";
								
								if($icoId != "")
								{
									$sqlItensEnviados .= "INSERT INTO par.empenhopregaoitensenviados (prpid, sbaid, epiano, idsigarp, adesao, usucpf, epistatus, epiquantidade, epivalor, epipregao, icoid) VALUES 
													( ".$dados['prpid'].", ".$sub.", ".$ano.", ".$it.", ".$ad.", '".$_SESSION['usucpf']."', 'A', ".$quantidadeEnviado.", ".$valorEnviado.", ".$pregaoEnviado.", {$icoId});";
								}
								else
								{
									$sqlItensEnviados .= "INSERT INTO par.empenhopregaoitensenviados (prpid, sbaid, epiano, idsigarp, adesao, usucpf, epistatus, epiquantidade, epivalor, epipregao) VALUES 
													( ".$dados['prpid'].", ".$sub.", ".$ano.", ".$it.", ".$ad.", '".$_SESSION['usucpf']."', 'A', ".$quantidadeEnviado.", ".$valorEnviado.", ".$pregaoEnviado.");";
									
								}
							}
						}
					}
				}
				
				if($dado->canceladas){
					foreach( $dado->canceladas->nu_seq_solicitacao_adesao as $adesaocancelada ){
						$sqlCancelamento .= "UPDATE par.empenhopregaoitensenviados SET epistatus = 'I' WHERE adesao = ".$adesaocancelada."; ";
					}
					$this->db->executar($sqlCancelamento);
				}
				
				if( $sqlUpdate ){
					$this->db->executar( $sqlUpdate );
				}
				
				if( $sqlItensEnviados ){
					$this->db->executar( $sqlItensEnviados );
				}
				
				$this->db->commit();
				
				if( is_array($itEnviado) && $itEnviado[0] ){
					foreach( $itEnviado as $it ){
						if($it['sbdid'] && $it['icoid'] && $dados['prpid'] ){
							$sqlDelete .= " DELETE FROM par.empenhopregaoitemperdido WHERE
										prpid = ".$dados['prpid']." AND
										sbdid = ".$it['sbdid']." AND
										icoid = ".$it['icoid']."; ";
						}
					}
					$this->db->executar($sqlDelete);
					$this->db->commit();
				}
				
				echo '<div style=" border: 1px solid #B7B7B7; font-size: 10px; font-style: normal; font-family: arial; padding: 5px 5px 5px 5px;"> 
							SOLICITAÇÃO DE ADESÃO REALIZADA COM SUCESSO
						</div>
					 	<br>
						';
			}
			
		} catch (Exception $e){
	
			# Erro 404 página not found
			if($e->getCode() == 404){
				//echo "Erro-Serviço do SIGARP encontra-se temporariamente indisponível. Favor tente mais tarde.".'\n';
				echo '<div style=" border: 1px solid #B7B7B7; font-size: 10px; font-style: normal; font-family: arial; padding: 5px 5px 5px 5px;"> 
							ERRO AO TENTAR ADERIR AO PREGÃO
							<div style=" border-top: 1px solid #B7B7B7; padding-top: 5px; " >
								Erro-Serviço do SIGARP encontra-se temporariamente indisponível. Favor tente mais tarde.
							</div>
						</div>
					 	<br>
						';
				
			}
			$erroMSG = str_replace(array(chr(13),chr(10)), ' ',$e->getMessage());
			$erroMSG = str_replace( "'", '"', $erroMSG );
		
			//echo "Erro-WS Enviar dados para o SIGARP: $erroMSG";
			
			echo '<div style=" border: 1px solid #B7B7B7; font-size: 10px; font-style: normal; font-family: arial; padding: 5px 5px 5px 5px;"> 
							ERRO AO TENTAR ADERIR AO PREGÃO
							<div style=" border-top: 1px solid #B7B7B7; padding-top: 5px; " >
								Erro-WS Enviar dados para o SIGARP: '.$erroMSG.'
							</div>
						</div>
					 	<br>
						';
			
		}
		
		
	}
	
	// Envio Manual - Reformulação
	public function aderirPregaoReformulacaoRotinaNoturna( $dados, $tipo = 1, $nivel = 'subacao' ){

		global $db;
		
		//$item = $db->carregarColuna( "SELECT icoid FROM par.empenhopregaoitemperdido WHERE sbdid = ".$dados['sbdid'] );
		
	try {
		
			if( $nivel == 'subacao' ){
				
				$sql = "SELECT
							sd.sbaid,
							sd.sbdano,
							sd.sbdid
						FROM
							par.subacaodetalhe sd
						INNER JOIN par.subacao s ON s.sbaid = sd.sbaid
						WHERE
							sd.sbdid IN (".$dados['sbdid'].") AND s.sbastatus = 'A'";
		
			} else {
				$sql = "SELECT DISTINCT
							sd.sbaid,
							sd.sbdano,
							sd.sbdid
						FROM
							par.subacaodetalhe sd
						INNER JOIN par.subacao s ON s.sbaid = sd.sbaid
						INNER JOIN par.termocomposicao ter ON ter.sbdid = sd.sbdid
						INNER JOIN par.documentopar dp ON dp.dopid = ter.dopid
						--INNER JOIN par.processoparcomposicao ppc ON ppc.sbdid = sd.sbdid and ppc.ppcstatus = 'A'
						WHERE
							dp.prpid = ".$dados['prpid']." AND s.sbastatus = 'A'";
			}
			
			$sbaids = $this->db->carregar($sql);
			
			$arrItens = array();
			foreach( $sbaids as $sba ){
			
				// Verifico os itens da subação
				$sql = "SELECT 
							s.sbaid,
							sd.sbdano,
							sd.sbdid,
							sic.icoid,
							pic.idsigarp,
							CASE WHEN sbacronograma = 1 --global
							THEN
								CASE WHEN sic.icovalidatecnico IS NULL -- antigos
								THEN coalesce(sic.icoquantidade,0)
								ELSE -- novos
									CASE WHEN sic.icovalidatecnico = 'S' THEN -- validado (caso não o item não é contado)
										sum(coalesce(sic.icoquantidadetecnico,0))
									END
								END
							ELSE -- escolas
								par.recuperaquantidadeitemvalidado(sic.icoid)
							END as quantidade,
							sic.icovalor as valor
						FROM
							par.subacao s
						INNER JOIN par.subacaodetalhe 		     sd  ON sd.sbaid = s.sbaid
					--	INNER JOIN par.processopar 	             pr  ON pr.prpid = sd.prpid and pr.prpstatus = 'A'
						LEFT  JOIN par.subacaoitenscomposicao 	 sic ON sic.sbaid = s.sbaid  AND sic.icoano = sd.sbdano AND sic.icostatus <> 'E'
						LEFT  JOIN par.subacaoescolas 		     se  ON se.sbaid = sic.sbaid AND se.sesano = sic.icoano
						LEFT  JOIN par.subescolas_subitenscomposicao ssi ON ssi.sesid = se.sesid AND ssi.icoid = sic.icoid
						INNER JOIN par.propostaitemcomposicao        pic ON pic.picid = sic.picid AND idsigarp is not null
				--		INNER JOIN par.empenhopregaoitemperdido 	 ip  ON ip.icoid = sic.icoid
						WHERE
							s.sbastatus = 'A' AND sd.sbdid IN ( ".$sba['sbdid']." ) AND 
							
							 CASE WHEN sbacronograma = 1 
		                     THEN sic.icovalidatecnico <> 'N' 
		                     ELSE 
		                     	CASE WHEN (s.frmid = 2) OR ( s.frmid = 4 AND s.ptsid = 42 ) OR ( s.frmid = 12 AND s.ptsid = 46 ) THEN
		                        	se.sesvalidatecnico <> 'N'
		                        ELSE
		                        	sic.icovalidatecnico <> 'N' 
		                        END
		                     END
		                group by
							s.sbaid, sd.sbdano, sd.sbdid, sic.icoid, pic.idsigarp, s.sbacronograma, sic.icovalidatecnico, sic.icoquantidade, s.frmid, s.ptsid, se.sesvalidatecnico, sic.icovalor
						ORDER BY
							s.sbaid";
					//		pr.prpid = ".$_SESSION['par_var']['prpid'];
	
				$arrItens[$sba['sbdid']] = $this->db->carregar( $sql );
				
			}
			
			$cnpj = pegaCnpj($dados['inuid'], $dados['prpid']);
	
			$dadosse = $this->db->pegaLinha("SELECT prpnumeroprocesso, prpbanco, prpagencia, muncod, prptipo, inuid, prptipoexecucao FROM par.processopar WHERE prpid='".$dados['prpid']."' and prpstatus = 'A'");
	
			$dopid = $this->db->pegaUm("SELECT dopid FROM par.documentopar WHERE dopstatus = 'A' AND prpid = ".$dados['prpid']);
			
			$testa = buscaDadosTermo( $dopid );
			
			$dadosTemo = $this->db->pegaLinha("SELECT dopnumerodocumento, dopdatafimvigencia FROM par.documentopar WHERE dopstatus = 'A' AND prpid = ".$dados['prpid']);
			if(strlen($dadosTemo['dopdatafimvigencia']) == 7){
				$mes = substr($dadosTemo['dopdatafimvigencia'], 0, 2);
				$ano = substr($dadosTemo['dopdatafimvigencia'], 3, 4);
				$vigencia = $ano."-".$mes;
			} else {
				$mes = substr($dadosTemo['dopdatafimvigencia'], 3, 2);
				$ano = substr($dadosTemo['dopdatafimvigencia'], 6, 4);
				$vigencia = $ano."-".$mes;
			}
			
			$data_created = date("c");
			$usuario = USUARIO_SIGARP;
			$senha   = SENHA_SIGARP;
			
			$itEnviado = array();
			$itPerdido = array();
			$it = 0;
			$cont = 0;
			$montou = false;
			$ultimaSubacao = 0;
			$itens2 = array();

		
$arqXmlSt = 
'<?xml version="1.0" encoding="ISO-8859-1"?>
<request>
    <header>
        <app>SIGARP</app>
        <version>1.0</version>
        <created>'.$data_created.'</created>
    </header>
    <body>
        <auth>
            <usuario>'.$usuario.'</usuario>
            <senha>'.$senha.'</senha>
        </auth>
        <params>
        	<cnpj>'.$cnpj.'</cnpj>
        	<nu_processo>'.$dadosse['prpnumeroprocesso'].'</nu_processo> 
        	<nu_seq_tipo_pagamento>'.$tipo.'</nu_seq_tipo_pagamento>
        	<nu_termo_compromisso>'.$dadosTemo['dopnumerodocumento'].'</nu_termo_compromisso>
        	<dt_vigencia>'.$vigencia.'</dt_vigencia>';
			foreach( $arrItens as $subacao => $itens ){
        		$monta = 0;
				if( $itens[0] ){
		        	foreach( $itens as $item ){
		        		
		        		$subacao = verificaSubacao( $subacao );
							        	
						$sql = "SELECT 
									CASE WHEN sbacronograma = 1 --global
									THEN
										CASE WHEN sic.icovalidatecnico IS NULL -- antigos
										THEN coalesce(sic.icoquantidade,0)
										ELSE -- novos
											CASE WHEN sic.icovalidatecnico = 'S' THEN -- validado (caso não o item não é contado)
												sum(coalesce(sic.icoquantidadetecnico,0))
											END
										END
									ELSE -- escolas
										par.recuperaquantidadeitemvalidado(sic.icoid)
									END as quantidade,
									sic.icovalor,
									s.sbaid
								FROM 
									par.subacaodetalhe sd
								INNER JOIN par.subacao s 				  ON s.sbaidpai = sd.sbaid
								INNER JOIN par.subacaoitenscomposicao sic ON sic.sbaid = s.sbaid and sic.icoano = sd.sbdano AND sic.icovalidatecnico = 'S'
								LEFT  JOIN par.subacaoescolas 		     se  ON se.sbaid = sic.sbaid AND se.sesano = sic.icoano
								LEFT  JOIN par.subescolas_subitenscomposicao ssi ON ssi.sesid = se.sesid AND ssi.icoid = sic.icoid
								INNER JOIN par.propostaitemcomposicao pic ON pic.picid = sic.picid AND idsigarp = ".$item['idsigarp']."
								WHERE 
									sd.sbdid = ".$subacao."
								GROUP BY
									s.sbacronograma, sic.icovalidatecnico, sic.icoquantidade,s.frmid, s.ptsid, se.sesvalidatecnico, sic.icovalor, s.sbaid, sic.icoid
								ORDER BY
									s.sbaid DESC
								LIMIT 1";
						
						$dadosItemAntigo = $this->db->pegaLinha($sql);
						
						
		        		$itens2[$cont]['sbdid'] = $item['sbdid'];
		        		$itens2[$cont]['icoid'] = $item['icoid'];
		        		$cont++;
		        		if($item['idsigarp']){
		        			
		        			if( $db->pegaLinha("SELECT * FROM par.historicowsprocessoparerrorotina where prpid = {$dados['prpid']} AND idsigarp = {$item['idsigarp']}"))
		        			{
		        				continue;
		        			}
		        			
		        			
			        		$pregao = $this->diponibilizaPregoesItensRotinaNoturna($item['idsigarp'],'pregao',$dados['prpid']);
			        		if( $item['quantidade'] <> 0 ){
				        		if( $pregao ){
				        			
				        			$valorAtual = $this->diponibilizaPregoesItensRotinaNoturna($item['idsigarp'],'valor',$dados['prpid']);
				        			
									if( is_array($dadosItemAntigo) ){
										if( $dadosItemAntigo['quantidade'] == $item['quantidade'] && $dadosItemAntigo['icovalor'] == $item['valor'] ){
											$acao = 'I';
										} else {
											$acao = 'A';
										}
									} else {
										$sql = "SELECT count(sbdreformulacao) FROM par.subacaodetalhe WHERE sbdreformulacao IS NOT NULL AND sbdid = ".$subacao;
										$testaReformulacao = $this->db->pegaUm($sql);
										
										if( $testaReformulacao > 0 ){
											$acao = 'A';
										} else {
											$sql = "SELECT idsigarp FROM par.empenhopregaoitensenviados WHERE prpid = ".$dados['prpid']." AND idsigarp = ".$item['idsigarp'];
											$testaItem = $this->db->pegaUm($sql);
											if( $testaItem ){
												$acao = 'I';
											} else {
												$acao = 'A';
											}
										}
									}
									
									if( $subacao == 3610598 ){
										if( $item['idsigarp'] == 821 || $item['idsigarp'] == 823 || $item['idsigarp'] == 824 || $item['idsigarp'] == 825 ){
											$acao = 'A';
										} else {
											$acao = 'I';
										}
									}
									if( $subacao == 3739181 || $subacao == 3739185 ){
										$acao = 'I';
									}
									
				        			$it++;
			        				$montou = true;
				        			if( $subacao != $ultimaSubacao && $ultimaSubacao != 0 ){
				        				$arqXmlSt .= '
				        							</sub_acao>';
				        			}
				        			$ultimaSubacao = $subacao;
				        			if( $monta == 0 ){
				        				$monta++;
				        				
				        				$arqXmlSt .= '
				        							<sub_acao>
				        								<nu_sub_acao>'.$subacao.'</nu_sub_acao>';
				        			}
									$arqXmlSt .= '
										<item>
											<nu_seq_pregao>'.$pregao.'</nu_seq_pregao> 
											<nu_seq_item>'.$item['idsigarp'].'</nu_seq_item> 
											<qt_item>'.$item['quantidade'].'</qt_item>  
											<vl_pagamento>'.$item['valor'].'</vl_pagamento>
											<acao>'.$acao.'</acao>
										</item>';
									
									$arItemEnviado[$item['idsigarp']]['pregao'] = $pregao;
									$arItemEnviado[$item['idsigarp']]['valor'] = $item['valor'];
									$arItemEnviado[$item['idsigarp']]['quantidade'] = $item['quantidade'];
									$arItemEnviado[$item['idsigarp']]['icoid'] = $item['icoid'];
									
									$sqlInsertItemPerd .= "INSERT INTO par.empenhopregaoitemperdido ( prpid, sbdid, icoid ) VALUES (".$dados['prpid'].", ".$item['sbdid'].", ".$item['icoid']."); ";
									
									$itEnviado[] = array('sbdid' => $item['sbdid'],
														'icoid' => $item['icoid']);
				        		}
			        		}
		        		}
		        		$pregao = false;
					}
				}
			}
			if( $montou == true ){
        		$arqXmlSt .= '
        					</sub_acao>';
        	}
		$arqXmlSt .= '</params>
    </body>
</request>';

$arqXml = <<<XML
{$arqXmlSt}
XML;
	
			if( $it == 0 ){ //Não tem nenhum item para enviar!
				echo "WS-Sigarp: Nenhum item para enviar. Verifique os pregões.";
				return true;
			} else {
				if( $sqlInsertItemPerd ){
					$sqlDeleta = "DELETE FROM par.empenhopregaoitemperdido WHERE prpid = ".$dados['prpid'];
					$this->db->executar($sqlDeleta);
					$this->db->executar($sqlInsertItemPerd);
					$this->db->commit();
				}
			}
			
			/*
	echo '<pre>';		
	echo simec_htmlentities($arqXml);
	die();
*/

			
			if( $_SESSION['baselogin'] == "simec_desenvolvimento"  || $_SESSION['baselogin'] == "simec_espelho_producao"){
				$urlWS = 'http://172.20.65.98/webservices/wssigarp/ws/termo';
			} else {
				$urlWS = 'http://www.fnde.gov.br/webservices/wssigarp/ws/termo';
			}
			$xml = Fnde_Webservice_Client::CreateRequest()
					->setURL($urlWS)
					->setParams( array('xml' => $arqXml, 'method' => 'subacao') )
					->execute();
						
			$xmlRetorno = $xml;								
			$xml = simplexml_load_string( stripslashes($xml));
	
			$result = (integer) $xml->status->result;
			
		//	print_r ($xml);
		//	die();
	
			if(!$result) { //ERRO
				
				$sql = "INSERT INTO par.historicowsprocessopar(
					    	prpid,
					    	hwpwebservice,
					    	hwpxmlenvio,
					    	hwpxmlretorno,
					    	hwpdataenvio,
					        usucpf)
					    VALUES ('".$dados['prpid']."',
					    		'enviarManualmenteItensSigarpReformulacao - Erro',
					    		'".addslashes($arqXml)."',
					    		'".addslashes($xmlRetorno)."',
					    		NOW(),
					            '');";
	
				$this->db->executar($sql);
				$this->db->commit();
				
				$erros = $xml->status->error->message->text;
				echo '<div style=" border: 1px solid #B7B7B7; font-size: 10px; font-style: normal; font-family: arial; padding: 5px 5px 5px 5px;"> 
							ERRO AO TENTAR ADERIR AO PREGÃO
							<div style=" border-top: 1px solid #B7B7B7; padding-top: 5px; " >
								'.iconv("UTF-8", "ISO-8859-1", $erros).'
							</div>
						</div>
					 	<br>
						';
				
			} else {
				// Deu certo
				
				$sql = "INSERT INTO par.historicowsprocessopar(
					    	prpid,
					    	hwpwebservice,
					    	hwpxmlenvio,
					    	hwpxmlretorno,
					    	hwpdataenvio,
					        usucpf)
					    VALUES ('".$dados['prpid']."',
					    		'enviarManualmenteItensSigarpReformulacao - Sucesso',
					    		'".addslashes($arqXml)."',
					    		'".addslashes($xmlRetorno)."',
					    		NOW(),
					            '')
					    RETURNING hwpid;";
	
				$id = $this->db->pegaUm($sql);
	                        
	            // Realiza importacao dos itens que estao no xml do historico.
	            $controle = new ProcessoItemAdesaoPregaoControle();
	            $result = $controle->transferirHistoricoProcesso($id);
	                        
				$this->db->commit();
				
				$dado = ($xml->body->params);
				
				if( is_array($dado->adesaopregao) || is_object($dado->adesaopregao) ){
					foreach( $dado->adesaopregao as $adesao ){
						$dadosSub = $this->db->pegaLinha( "SELECT sbaid, sbdano FROM par.subacaodetalhe WHERE sbdid = ".(string) $adesao->nu_sub_acao);
						if(!$dadosSub){
							$dadosSub = $this->db->pegaLinha( "SELECT sbaid, sbdano FROM par.subacaodetalhe WHERE prpid = ".$dados['prpid']." AND sbaid = ".(string) $adesao->nu_sub_acao);
						}
						$sub = $dadosSub['sbaid'];
						$ano = $dadosSub['sbdano'];
						$ad = (integer) $adesao->nu_seq_solicitacao_adesao;
						foreach( $adesao->item as $item ){
							foreach( $item->nu_seq_item as $it2 ){
								$it = (integer) $it2;
								
								$pregaoEnviado = $arItemEnviado[$it]['pregao'];
								$quantidadeEnviado = $arItemEnviado[$it]['quantidade'];
								$valorEnviado 	= $arItemEnviado[$it]['valor'];
								$icoId		 	= $arItemEnviado[$it]['icoid'];
								
								$sqlUpdate .= "UPDATE par.empenhopregaoitensenviados SET epistatus = 'I' WHERE idsigarp = ".$it." AND prpid = ".$dados['prpid']." AND sbaid = ".$sub." AND epiano = ".$ano."; ";
								
								if($icoId != "")
								{
									$sqlItensEnviados .= "INSERT INTO par.empenhopregaoitensenviados (prpid, sbaid, epiano, idsigarp, adesao, usucpf, epistatus, epiquantidade, epivalor, epipregao, icoid) VALUES
														( ".$dados['prpid'].", ".$sub.", ".$ano.", ".$it.", ".$ad.", '', 'A', ".$quantidadeEnviado.", ".$valorEnviado.", ".$pregaoEnviado.", {$icoId});";
								}
								else	
								{	
									$sqlItensEnviados .= "INSERT INTO par.empenhopregaoitensenviados (prpid, sbaid, epiano, idsigarp, adesao, usucpf, epistatus, epiquantidade, epivalor, epipregao) VALUES 
														( ".$dados['prpid'].", ".$sub.", ".$ano.", ".$it.", ".$ad.", '', 'A', ".$quantidadeEnviado.", ".$valorEnviado.", ".$pregaoEnviado.");";
								}
							}
						}
					}
				}
				
				if($dado->canceladas){
					foreach( $dado->canceladas->nu_seq_solicitacao_adesao as $adesaocancelada ){
						$sqlCancelamento .= "UPDATE par.empenhopregaoitensenviados SET epistatus = 'I' WHERE adesao = ".$adesaocancelada."; ";
					}
					$this->db->executar($sqlCancelamento);
				}
				
				if( $sqlUpdate ){
					$this->db->executar( $sqlUpdate );
				}
				
				if( $sqlItensEnviados ){
					$this->db->executar( $sqlItensEnviados );
				}
				
				$this->db->commit();
				
				if( is_array($itEnviado) && $itEnviado[0] ){
					foreach( $itEnviado as $it ){
						if($it['sbdid'] && $it['icoid'] && $dados['prpid'] ){
							$sqlDelete .= " DELETE FROM par.empenhopregaoitemperdido WHERE
										prpid = ".$dados['prpid']." AND
										sbdid = ".$it['sbdid']." AND
										icoid = ".$it['icoid']."; ";
						}
					}
					$this->db->executar($sqlDelete);
					$this->db->commit();
				}
				
				echo '<div style=" border: 1px solid #B7B7B7; font-size: 10px; font-style: normal; font-family: arial; padding: 5px 5px 5px 5px;"> 
							SOLICITAÇÃO DE ADESÃO REALIZADA COM SUCESSO
						</div>
					 	<br>
						';
			}
			
		} catch (Exception $e){
	
			# Erro 404 página not found
			if($e->getCode() == 404){
				//echo "Erro-Serviço do SIGARP encontra-se temporariamente indisponível. Favor tente mais tarde.".'\n';
				echo '<div style=" border: 1px solid #B7B7B7; font-size: 10px; font-style: normal; font-family: arial; padding: 5px 5px 5px 5px;"> 
							ERRO AO TENTAR ADERIR AO PREGÃO
							<div style=" border-top: 1px solid #B7B7B7; padding-top: 5px; " >
								Erro-Serviço do SIGARP encontra-se temporariamente indisponível. Favor tente mais tarde.
							</div>
						</div>
					 	<br>
						';
				
			}
			$erroMSG = str_replace(array(chr(13),chr(10)), ' ',$e->getMessage());
			$erroMSG = str_replace( "'", '"', $erroMSG );
		
			//echo "Erro-WS Enviar dados para o SIGARP: $erroMSG";
			
			echo '<div style=" border: 1px solid #B7B7B7; font-size: 10px; font-style: normal; font-family: arial; padding: 5px 5px 5px 5px;"> 
							ERRO AO TENTAR ADERIR AO PREGÃO
							<div style=" border-top: 1px solid #B7B7B7; padding-top: 5px; " >
								Erro-WS Enviar dados para o SIGARP: '.$erroMSG.'
							</div>
						</div>
					 	<br>
						';
			
		}
		
		
	}
	
	
	function diponibilizaPregoesItensRotinaNoturna($item, $tipo, $prpid){
	
		global $db;
		$inuid = $db->pegaUm("select inuid from par.processopar  where prpstatus = 'A' and prpid ={$prpid}" );
		
		try {
			
			$data_created = date("c");
	
			$usuario = USUARIO_SIGARP;
			$senha   = SENHA_SIGARP;
			
			$sql = "SELECT
						CASE WHEN estuf IS NOT NULL 
							THEN estuf 
							ELSE mun_estuf 
						END as estuf 
					FROM
						par.instrumentounidade 
					WHERE 
						inuid = {$inuid}";
			$estuf = $this->db->pegaUm( $sql );

$arqXml = <<<XML
<?xml version="1.0" encoding="ISO-8859-1"?>
<request>
    <header>
        <app>SIGARP</app>
        <version>1.0</version>
        <created>$data_created</created>
    </header>
    <body>
        <auth>
            <usuario>$usuario</usuario>
            <senha>$senha</senha>
        </auth>
        <params>
            <nu_seq_item>{$item}</nu_seq_item>
            <uf>{$estuf}</uf>
        </params>
    </body>
</request>
XML;
	
		   /*
	echo '<pre>';		
	echo simec_htmlentities($arqXml);
	die();
			 */  		


			if( $_SESSION['baselogin'] == "simec_desenvolvimento" || $_SESSION['baselogin'] == "simec_espelho_producao" ){
//			if( $_SESSION['baselogin'] == "simec_desenvolvimento" ){
				$urlWS = 'http://172.20.65.98/webservices/wssigarp/ws/pregao';
//				$urlWS = 'http://www.fnde.gov.br/webservices/wssigarp/ws/pregao';
			} else {
				$urlWS = 'http://www.fnde.gov.br/webservices/wssigarp/ws/pregao';
			}
	
			$xml = Fnde_Webservice_Client::CreateRequest()
					->setURL($urlWS)
					->setParams( array('xml' => $arqXml, 'method' => 'listar') )
					->execute();
						
			$xmlRetorno = $xml;
								
			$xml = simplexml_load_string( stripslashes($xml));
	
			$result = (integer) $xml->status->result;
			
			if(!$result) {
				
				$sql = "INSERT INTO par.historicowsprocessopar(
					    	prpid,
					    	hwpwebservice,
					    	hwpxmlenvio,
					    	hwpxmlretorno,
					    	hwpdataenvio,
					        usucpf)
					    VALUES ('".$prpid."',
					    		'diponibilizaPregoesItens - Erro',
					    		'".addslashes($arqXml)."',
					    		'".addslashes($xmlRetorno)."',
					    		NOW(),
					            '');";
	
				$this->db->executar($sql);
				$sql2 = "INSERT INTO par.historicowsprocessoparerrorotina
						(
						 	prpid, 
						  	usucpf, 
						  	hprdata, 
						  	idsigarp 
						 )
						 VALUES
						 (	'".$prpid."',
						 	'',
						 	NOW(),
						 	$item
						 )
				";	
				$this->db->executar($sql2);
				$this->db->commit();
				
				$erros = $xml->status->error->message->text;
	
			} else { //sucesso
				
				$sql = "INSERT INTO par.historicowsprocessopar(
					    	prpid,
					    	hwpwebservice,
					    	hwpxmlenvio,
					    	hwpxmlretorno,
					    	hwpdataenvio,
					        usucpf)
					    VALUES ('".$prpid."',
					    		'diponibilizaPregoesItens - Sucesso',
					    		'".addslashes($arqXml)."',
					    		'".addslashes($xmlRetorno)."',
					    		NOW(),
					            '');";
	
				$this->db->executar($sql);
				$this->db->commit();
	
				if( $tipo == 'valor' ){
					return $xml->body->VL_ITEM ? $xml->body->VL_ITEM : null;
				} else {
					return (integer) $xml->body->NU_SEQ_PREGAO ? $xml->body->NU_SEQ_PREGAO : false;
				}
			}
			
		} catch (Exception $e){
			
			# Erro 404 página not found
			if($e->getCode() == 404){
				//echo "Temporariamente indisponível.Favor tente mais tarde.".'\n';
				/* -- ARRUMAR DEPOIS 
				echo '<div style=" border: 1px solid #B7B7B7; font-size: 10px; font-style: normal; font-family: arial; padding: 5px 5px 5px 5px;"> 
							ERRO AO CONSULTAR ITENS DO PREGÃO (SIGARP)
							<div style=" border-top: 1px solid #B7B7B7; padding-top: 5px; " >
								Temporariamente indisponível.Favor tente mais tarde.
							</div>
						</div>
					 	<br>
						';
				*/
				
			}
			$erroMSG = str_replace(array(chr(13),chr(10)), ' ',$e->getMessage());
			$erroMSG = str_replace( "'", '"', $erroMSG );
			/* -- ARRUMAR DEPOIS 
			echo '<div style=" border: 1px solid #B7B7B7; font-size: 10px; font-style: normal; font-family: arial; padding: 5px 5px 5px 5px;"> 
							ERRO AO CONSULTAR ITENS DO PREGÃO (SIGARP)
							<div style=" border-top: 1px solid #B7B7B7; padding-top: 5px; " >
								Erro-WS SIGARP: '.$erroMSG.'
								<br>
								<pre>
								'.print_r($e).'
								</pre>
							</div>
						</div>
					 	<br>
						';
			*/
			
			//echo "Erro-WS SIGARP: $erroMSG";
			//echo "<pre>";
			//print_r($e);
			
		}
	}
	// Envio Manual
	public function aderirPregaoRotinaNoturna( $dados, $tipo = 1, $nivel = 'subacao' ){

		global $db;
		
	try {
	
			if( $nivel == 'subacao' ){
				
				$sql = "SELECT
							sd.sbaid,
							sd.sbdano,
							sd.sbdid
						FROM
							par.subacaodetalhe sd
						INNER JOIN par.subacao s ON s.sbaid = sd.sbaid
						WHERE
							sd.sbdid IN (".$dados['sbdid'].") AND s.sbastatus = 'A'";
		
			} else {
				$sql = "SELECT DISTINCT
							sd.sbaid,
							sd.sbdano,
							sd.sbdid
						FROM
							par.subacaodetalhe sd
						INNER JOIN par.subacao s ON s.sbaid = sd.sbaid
						INNER JOIN par.termocomposicao ter ON ter.sbdid = sd.sbdid
						INNER JOIN par.documentopar dp ON dp.dopid = ter.dopid
						--INNER JOIN par.processoparcomposicao ppc ON ppc.sbdid = sd.sbdid and ppc.ppcstatus = 'A'
						WHERE
							dp.prpid = ".$dados['prpid']." AND s.sbastatus = 'A'";
			}
			
			$sbaids = $this->db->carregar($sql);
			
			$arrItens = array();
			foreach( $sbaids as $sba ){
			
				// Verifico os itens da subação
				$sql = "SELECT 
							s.sbaid,
							sd.sbdano,
							sd.sbdid,
							sic.icoid,
							pic.idsigarp,
							CASE WHEN sbacronograma = 1 --global
							THEN
								CASE WHEN sic.icovalidatecnico IS NULL -- antigos
								THEN coalesce(sic.icoquantidade,0)
								ELSE -- novos
									CASE WHEN sic.icovalidatecnico = 'S' THEN -- validado (caso não o item não é contado)
										sum(coalesce(sic.icoquantidadetecnico,0))
									END
								END
							ELSE -- escolas
								 par.recuperaquantidadeitemvalidado(sic.icoid)
							END as quantidade,
							sic.icovalor as valor
						FROM
							par.subacao s
						INNER JOIN par.subacaodetalhe 		     sd  ON sd.sbaid = s.sbaid
					--	INNER JOIN par.processopar 	             pr  ON pr.prpid = sd.prpid and pr.prpstatus = 'A'
						LEFT  JOIN par.subacaoitenscomposicao 	 sic ON sic.sbaid = s.sbaid  AND sic.icoano = sd.sbdano and sic.icostatus <> 'E'
						LEFT  JOIN par.subacaoescolas 		     se  ON se.sbaid = sic.sbaid AND se.sesano = sic.icoano
						LEFT  JOIN par.subescolas_subitenscomposicao ssi ON ssi.sesid = se.sesid AND ssi.icoid = sic.icoid
						INNER JOIN par.propostaitemcomposicao        pic ON pic.picid = sic.picid
					--	INNER JOIN par.empenhopregaoitemperdido 	 ip  ON ip.icoid = sic.icoid
						WHERE
							s.sbastatus = 'A' AND sd.sbaid IN ( ".$sba['sbaid']." ) AND sd.sbdano IN ( ".$sba['sbdano']." ) AND 
							
							 CASE WHEN sbacronograma = 1 
		                     THEN sic.icovalidatecnico <> 'N' 
		                     ELSE 
		                     	CASE WHEN (s.frmid = 2) OR ( s.frmid = 4 AND s.ptsid = 42 ) OR ( s.frmid = 12 AND s.ptsid = 46 ) THEN
		                        	se.sesvalidatecnico <> 'N'
		                        ELSE
		                        	sic.icovalidatecnico <> 'N' 
		                        END
		                     END
		                group by
							s.sbaid, sd.sbdano, sd.sbdid, sic.icoid, pic.idsigarp, s.sbacronograma, sic.icovalidatecnico, sic.icoquantidade, s.frmid, s.ptsid, se.sesvalidatecnico, sic.icovalor
						ORDER BY
							s.sbaid";
					//		pr.prpid = ".$_SESSION['par_var']['prpid'];
			
				$arrItens[$sba['sbdid']] = $this->db->carregar( $sql );
				
			}
			
			$cnpj = pegaCnpj($dados['inuid'], $dados['prpid']);
			
			$dadosse = $this->db->pegaLinha("SELECT prpnumeroprocesso, prpbanco, prpagencia, muncod, prptipo, inuid, prptipoexecucao FROM par.processopar WHERE prpid='".$dados['prpid']."' and prpstatus = 'A'");
	
			$dadosTemo = $this->db->pegaLinha("SELECT dopnumerodocumento, dopdatafimvigencia FROM par.documentopar WHERE dopstatus = 'A' AND prpid = ".$dados['prpid']);
			if(strlen($dadosTemo['dopdatafimvigencia']) == 7){
				$mes = substr($dadosTemo['dopdatafimvigencia'], 0, 2);
				$ano = substr($dadosTemo['dopdatafimvigencia'], 3, 4);
				$vigencia = $ano."-".$mes;
			} else {
				$mes = substr($dadosTemo['dopdatafimvigencia'], 3, 2);
				$ano = substr($dadosTemo['dopdatafimvigencia'], 6, 4);
				$vigencia = $ano."-".$mes;
			}
			
			$data_created = date("c");
			$usuario = USUARIO_SIGARP;
			$senha   = SENHA_SIGARP;
			
			$itEnviado = array();
			$itPerdido = array();
			$it = 0;
			$cont = 0;
			$montou = false;
			$ultimaSubacao = 0;
			$itens2 = array();

		
$arqXmlSt = 
'<?xml version="1.0" encoding="ISO-8859-1"?>
<request>
    <header>
        <app>SIGARP</app>
        <version>1.0</version>
        <created>'.$data_created.'</created>
    </header>
    <body>
        <auth>
            <usuario>'.$usuario.'</usuario>
            <senha>'.$senha.'</senha>
        </auth>
        <params>
        	<cnpj>'.$cnpj.'</cnpj>
        	<nu_processo>'.$dadosse['prpnumeroprocesso'].'</nu_processo> 
        	<nu_seq_tipo_pagamento>'.$tipo.'</nu_seq_tipo_pagamento>
        	<nu_termo_compromisso>'.$dadosTemo['dopnumerodocumento'].'</nu_termo_compromisso>
        	<dt_vigencia>'.$vigencia.'</dt_vigencia>';
			foreach( $arrItens as $subacao => $itens ){
        		$monta = 0;
				if( $itens[0] ){
		        	foreach( $itens as $item ){
		        		$itens2[$cont]['sbdid'] = $item['sbdid'];
		        		$itens2[$cont]['icoid'] = $item['icoid'];
		        		$cont++;
		        		if($item['idsigarp']){
		        			
		        			if($db->pegaLinha("SELECT * FROM par.historicowsprocessoparerrorotina where prpid = {$dados['prpid']} AND idsigarp = {$item['idsigarp']}"))
		        			{
		        				continue;
		        				
		        			}
		        			
			        		$pregao = $this->diponibilizaPregoesItensRotinaNoturna($item['idsigarp'],'pregao',$dados['prpid']);
			        		$valor = $this->diponibilizaPregoesItensRotinaNoturna($item['idsigarp'],'valor',$dados['prpid']);
			        		
			        		if( $item['quantidade'] <> 0 ){
				        		if( $pregao ){
				        			$it++;
			        				$montou = true;
				        			if( $subacao != $ultimaSubacao && $ultimaSubacao != 0 ){
				        				$arqXmlSt .= '
				        							</sub_acao>';
				        			}
				        			$ultimaSubacao = $subacao;
				        			if( $monta == 0 ){
				        				$monta++;
				        				$arqXmlSt .= '
				        							<sub_acao>
				        								<nu_sub_acao>'.$subacao.'</nu_sub_acao>';
				        			}
									$arqXmlSt .= '
										<item>
											<nu_seq_pregao>'.$pregao.'</nu_seq_pregao> 
											<nu_seq_item>'.$item['idsigarp'].'</nu_seq_item> 
											<qt_item>'.$item['quantidade'].'</qt_item>  
											<vl_pagamento>'.$item['valor'].'</vl_pagamento>
										</item>';
									$arItemEnviado[$item['idsigarp']]['pregao'] = $pregao;
									$arItemEnviado[$item['idsigarp']]['valor'] = $item['valor'];
									$arItemEnviado[$item['idsigarp']]['quantidade'] = $item['quantidade'];
									$arItemEnviado[$item['idsigarp']]['icoid'] = $item['icoid'];
									
									$sqlInsertItemPerd .= "INSERT INTO par.empenhopregaoitemperdido ( prpid, sbdid, icoid ) VALUES (".$dados['prpid'].", ".$item['sbdid'].", ".$item['icoid']."); ";
									
									$itEnviado[] = array('sbdid' => $item['sbdid'],
														'icoid' => $item['icoid']);
				        		}
			        		}
		        		}
		        		$pregao = false;
					}
				}
			}
			if( $montou == true ){
        		$arqXmlSt .= '
        					</sub_acao>';
        	}
		$arqXmlSt .= '</params>
    </body>
</request>';

$arqXml = <<<XML
{$arqXmlSt}
XML;
	
			if( $it == 0 ){ //Não tem nenhum item para enviar!
				return true;
			} else {
				if( $sqlInsertItemPerd ){
					$sqlDeleta = "DELETE FROM par.empenhopregaoitemperdido WHERE prpid = ".$dados['prpid'];
					$this->db->executar($sqlDeleta);
					$this->db->executar($sqlInsertItemPerd);
					$this->db->commit();
				}
			}
	
	 /*
	echo '<pre>';		
	echo $arqXml;
	die();
		*/  		
			if( $_SESSION['baselogin'] == "simec_desenvolvimento" || $_SESSION['baselogin'] == "simec_espelho_producao" ){
				$urlWS = 'http://172.20.65.98/webservices/wssigarp/ws/item';
			} else {
				$urlWS = 'http://www.fnde.gov.br/webservices/wssigarp/ws/item';
			}
			$xml = Fnde_Webservice_Client::CreateRequest()
					->setURL($urlWS)
					->setParams( array('xml' => $arqXml, 'method' => 'solicitar') )
					->execute();
						
			$xmlRetorno = $xml;
								
			$xml = simplexml_load_string( stripslashes($xml));
	
			$result = (integer) $xml->status->result;
			
		//	print_r ($xml);
		//	die();
	
			if(!$result) { //ERRO
				
				$sql = "INSERT INTO par.historicowsprocessopar(
					    	prpid,
					    	hwpwebservice,
					    	hwpxmlenvio,
					    	hwpxmlretorno,
					    	hwpdataenvio,
					        usucpf)
					    VALUES ('".$dados['prpid']."',
					    		'enviarManualmenteItensSigarp - Erro',
					    		'".addslashes($arqXml)."',
					    		'".addslashes($xmlRetorno)."',
					    		NOW(),
					            '".$_SESSION['usucpf']."');";
	
				$this->db->executar($sql);
				$this->db->commit();
				
				if( (integer) $xml->status->error->message->code == 25 ){ // Fnde: Já existe solicitaçao de adesão para esse processo. Utilizar o método reformular.
					$sigarp = $this->aderirPregaoReformulacaoRotinaNoturna($dados, $tipo, $nivel);
					return;
				}
				
				$erros = $xml->status->error->message->text;
				echo '<div style=" border: 1px solid #B7B7B7; font-size: 10px; font-style: normal; font-family: arial; padding: 5px 5px 5px 5px;"> 
							ERRO AO TENTAR ADERIR AO PREGÃO
							<div style=" border-top: 1px solid #B7B7B7; padding-top: 5px; " >
								'.iconv("UTF-8", "ISO-8859-1", $erros).'
							</div>
						</div>
					 	<br>
						';
				
			} else {
				// Deu certo
				
				$sql = "INSERT INTO par.historicowsprocessopar(
					    	prpid,
					    	hwpwebservice,
					    	hwpxmlenvio,
					    	hwpxmlretorno,
					    	hwpdataenvio,
					        usucpf)
					    VALUES ('".$dados['prpid']."',
					    		'enviarManualmenteItensSigarp - Sucesso',
					    		'".addslashes($arqXml)."',
					    		'".addslashes($xmlRetorno)."',
					    		NOW(),
					            '')
					    RETURNING hwpid;";
	
				$id = $this->db->pegaUm($sql);
	                        
	            // Realiza importacao dos itens que estao no xml do historico.
	            $controle = new ProcessoItemAdesaoPregaoControle();
	            $result = $controle->transferirHistoricoProcesso($id);
	                        
				$this->db->commit();
				
				$dado = ($xml->body->params);
				foreach( $dado->adesaopregao as $adesao ){
					$dadosSub = $this->db->pegaLinha( "SELECT sbaid, sbdano FROM par.subacaodetalhe WHERE sbdid = ".(string) $adesao->nu_sub_acao);
					$sub = $dadosSub['sbaid'];
					$ano = $dadosSub['sbdano'];
					$ad = (integer) $adesao->nu_seq_solicitacao_adesao;
					foreach( $adesao->item as $item ){
						foreach( $item->nu_seq_item as $it2 ){
							$it = (integer) $it2;
							
							$pregaoEnviado = $arItemEnviado[$it]['pregao'];
							$quantidadeEnviado = $arItemEnviado[$it]['quantidade'];
							$valorEnviado = $arItemEnviado[$it]['valor'];
							$icoId = $arItemEnviado[$it]['icoid'];
							
							if($icoId != "")
							{
								$sqlItensEnviados .= "INSERT INTO par.empenhopregaoitensenviados (prpid, sbaid, epiano, idsigarp, adesao, usucpf, epistatus, epiquantidade, epivalor, epipregao, icoid) VALUES
													( ".$dados['prpid'].", ".$sub.", ".$ano.", ".$it.", ".$ad.", '".$_SESSION['usucpf']."', 'A', ".$quantidadeEnviado.", ".$valorEnviado.", ".$pregaoEnviado.", {$icoId});";
							}
							else
							{
								$sqlItensEnviados .= "INSERT INTO par.empenhopregaoitensenviados (prpid, sbaid, epiano, idsigarp, adesao, usucpf, epistatus, epiquantidade, epivalor, epipregao) VALUES
													( ".$dados['prpid'].", ".$sub.", ".$ano.", ".$it.", ".$ad.", '".$_SESSION['usucpf']."', 'A', ".$quantidadeEnviado.", ".$valorEnviado.", ".$pregaoEnviado.");";
							}
						}
					}
				}
				
				if($dado->canceladas){
					foreach( $dado->canceladas->nu_seq_solicitacao_adesao as $adesaocancelada ){
						$sqlCancelamento .= "UPDATE par.empenhopregaoitensenviados SET epistatus = 'I' WHERE adesao = ".$adesaocancelada."; ";
					}
					$this->db->executar($sqlCancelamento);
				}
				
				if( $sqlItensEnviados ){
					$this->db->executar( $sqlItensEnviados );
					$this->db->commit();
				}
				
				if( is_array($itEnviado) && $itEnviado[0] ){
					foreach( $itEnviado as $it ){
						if($it['sbdid'] && $it['icoid'] && $dados['prpid'] ){
							$sqlDelete .= " DELETE FROM par.empenhopregaoitemperdido WHERE
										prpid = ".$dados['prpid']." AND
										sbdid = ".$it['sbdid']." AND
										icoid = ".$it['icoid']."; ";
						}
					}
					$this->db->executar($sqlDelete);
					$this->db->commit();
				}
				
				echo '<div style=" border: 1px solid #B7B7B7; font-size: 10px; font-style: normal; font-family: arial; padding: 5px 5px 5px 5px;"> 
							SOLICITAÇÃO DE ADESÃO REALIZADA COM SUCESSO
						</div>
					 	<br>
						';
			}
			
		} catch (Exception $e){
	
			# Erro 404 página not found
			if($e->getCode() == 404){
				//echo "Erro-Serviço do SIGARP encontra-se temporariamente indisponível. Favor tente mais tarde.".'\n';
				echo '<div style=" border: 1px solid #B7B7B7; font-size: 10px; font-style: normal; font-family: arial; padding: 5px 5px 5px 5px;"> 
							ERRO AO TENTAR ADERIR AO PREGÃO
							<div style=" border-top: 1px solid #B7B7B7; padding-top: 5px; " >
								Erro-Serviço do SIGARP encontra-se temporariamente indisponível. Favor tente mais tarde.
							</div>
						</div>
					 	<br>
						';
				
			}
			$erroMSG = str_replace(array(chr(13),chr(10)), ' ',$e->getMessage());
			$erroMSG = str_replace( "'", '"', $erroMSG );
		
			//echo "Erro-WS Enviar dados para o SIGARP: $erroMSG";
			
			echo '<div style=" border: 1px solid #B7B7B7; font-size: 10px; font-style: normal; font-family: arial; padding: 5px 5px 5px 5px;"> 
							ERRO AO TENTAR ADERIR AO PREGÃO
							<div style=" border-top: 1px solid #B7B7B7; padding-top: 5px; " >
								Erro-WS Enviar dados para o SIGARP: '.$erroMSG.'
							</div>
						</div>
					 	<br>
						';
		}
	}
	
	
	
	// Envio Manual
	public function aderirPregao( $dados, $tipo = 1, $nivel = 'subacao' ){

		global $db;
		
	try {
	
			if( $nivel == 'subacao' ){
				
				$sql = "SELECT
							sd.sbaid,
							sd.sbdano,
							sd.sbdid
						FROM
							par.subacaodetalhe sd
						INNER JOIN par.subacao s ON s.sbaid = sd.sbaid
						WHERE
							sd.sbdid IN (".$dados['sbdid'].") AND s.sbastatus = 'A'";
		
			} else {
				$sql = "SELECT DISTINCT
							sd.sbaid,
							sd.sbdano,
							sd.sbdid
						FROM
							par.subacaodetalhe sd
						INNER JOIN par.subacao s ON s.sbaid = sd.sbaid
						INNER JOIN par.termocomposicao ter ON ter.sbdid = sd.sbdid
						INNER JOIN par.documentopar dp ON dp.dopid = ter.dopid
						--INNER JOIN par.processoparcomposicao ppc ON ppc.sbdid = sd.sbdid and ppc.ppcstatus = 'A'
						WHERE
							dp.prpid = ".$dados['prpid']." AND s.sbastatus = 'A'";
			}
			
			$sbaids = $this->db->carregar($sql);
			
			$arrItens = array();
			foreach( $sbaids as $sba ){
			
				// Verifico os itens da subação
				$sql = "SELECT 
							s.sbaid,
							sd.sbdano,
							sd.sbdid,
							sic.icoid,
							pic.idsigarp,
							CASE WHEN sbacronograma = 1 --global
							THEN
								CASE WHEN sic.icovalidatecnico IS NULL -- antigos
								THEN coalesce(sic.icoquantidade,0)
								ELSE -- novos
									CASE WHEN sic.icovalidatecnico = 'S' THEN -- validado (caso não o item não é contado)
										sum(coalesce(sic.icoquantidadetecnico,0))
									END
								END
							ELSE -- escolas
								 par.recuperaquantidadeitemvalidado(sic.icoid)
							END as quantidade,
							sic.icovalor as valor
						FROM
							par.subacao s
						INNER JOIN par.subacaodetalhe 		     sd  ON sd.sbaid = s.sbaid
					--	INNER JOIN par.processopar 	             pr  ON pr.prpid = sd.prpid and pr.prpstatus = 'A'
						LEFT  JOIN par.subacaoitenscomposicao 	 sic ON sic.sbaid = s.sbaid  AND sic.icoano = sd.sbdano and sic.icostatus <> 'E'
						LEFT  JOIN par.subacaoescolas 		     se  ON se.sbaid = sic.sbaid AND se.sesano = sic.icoano
						LEFT  JOIN par.subescolas_subitenscomposicao ssi ON ssi.sesid = se.sesid AND ssi.icoid = sic.icoid
						INNER JOIN par.propostaitemcomposicao        pic ON pic.picid = sic.picid
					--	INNER JOIN par.empenhopregaoitemperdido 	 ip  ON ip.icoid = sic.icoid
						WHERE
							s.sbastatus = 'A' AND sd.sbaid IN ( ".$sba['sbaid']." ) AND sd.sbdano IN ( ".$sba['sbdano']." ) AND 
							
							 CASE WHEN sbacronograma = 1 
		                     THEN sic.icovalidatecnico <> 'N' 
		                     ELSE 
		                     	CASE WHEN (s.frmid = 2) OR ( s.frmid = 4 AND s.ptsid = 42 ) OR ( s.frmid = 12 AND s.ptsid = 46 ) THEN
		                        	se.sesvalidatecnico <> 'N'
		                        ELSE
		                        	sic.icovalidatecnico <> 'N' 
		                        END
		                     END
		                group by
							s.sbaid, sd.sbdano, sd.sbdid, sic.icoid, pic.idsigarp, s.sbacronograma, sic.icovalidatecnico, sic.icoquantidade, s.frmid, s.ptsid, se.sesvalidatecnico, sic.icovalor
						ORDER BY
							s.sbaid";
					//		pr.prpid = ".$_SESSION['par_var']['prpid'];
	
				$arrItens[$sba['sbdid']] = $this->db->carregar( $sql );
				
			}
	
			$cnpj = pegaCnpj($dados['inuid'], $dados['prpid']);
	
			$dadosse = $this->db->pegaLinha("SELECT prpnumeroprocesso, prpbanco, prpagencia, muncod, prptipo, inuid, prptipoexecucao FROM par.processopar WHERE prpstatus = 'A' and prpid='".$dados['prpid']."'");
	
			$dadosTemo = $this->db->pegaLinha("SELECT dopnumerodocumento, dopdatafimvigencia FROM par.documentopar WHERE dopstatus = 'A' AND prpid = ".$dados['prpid']);
			if(strlen($dadosTemo['dopdatafimvigencia']) == 7){
				$mes = substr($dadosTemo['dopdatafimvigencia'], 0, 2);
				$ano = substr($dadosTemo['dopdatafimvigencia'], 3, 4);
				$vigencia = $ano."-".$mes;
			} else {
				$mes = substr($dadosTemo['dopdatafimvigencia'], 3, 2);
				$ano = substr($dadosTemo['dopdatafimvigencia'], 6, 4);
				$vigencia = $ano."-".$mes;
			}
			
			$data_created = date("c");
			$usuario = USUARIO_SIGARP;
			$senha   = SENHA_SIGARP;
			
			$itEnviado = array();
			$itPerdido = array();
			$it = 0;
			$cont = 0;
			$montou = false;
			$ultimaSubacao = 0;
			$itens2 = array();

		
$arqXmlSt = 
'<?xml version="1.0" encoding="ISO-8859-1"?>
<request>
    <header>
        <app>SIGARP</app>
        <version>1.0</version>
        <created>'.$data_created.'</created>
    </header>
    <body>
        <auth>
            <usuario>'.$usuario.'</usuario>
            <senha>'.$senha.'</senha>
        </auth>
        <params>
        	<cnpj>'.$cnpj.'</cnpj>
        	<nu_processo>'.$dadosse['prpnumeroprocesso'].'</nu_processo> 
        	<nu_seq_tipo_pagamento>'.$tipo.'</nu_seq_tipo_pagamento>
        	<nu_termo_compromisso>'.$dadosTemo['dopnumerodocumento'].'</nu_termo_compromisso>
        	<dt_vigencia>'.$vigencia.'</dt_vigencia>';
			foreach( $arrItens as $subacao => $itens ){
        		$monta = 0;
				if( $itens[0] ){
		        	foreach( $itens as $item ){
		        		$itens2[$cont]['sbdid'] = $item['sbdid'];
		        		$itens2[$cont]['icoid'] = $item['icoid'];
		        		$cont++;
		        		if($item['idsigarp']){
			        		$pregao = $this->diponibilizaPregoesItens($item['idsigarp'],'pregao',$dados['prpid']);
			        		$valor = $this->diponibilizaPregoesItens($item['idsigarp'],'valor',$dados['prpid']);
			        		if( $item['quantidade'] <> 0 ){
				        		if( $pregao ){
				        			$it++;
			        				$montou = true;
				        			if( $subacao != $ultimaSubacao && $ultimaSubacao != 0 ){
				        				$arqXmlSt .= '
				        							</sub_acao>';
				        			}
				        			$ultimaSubacao = $subacao;
				        			if( $monta == 0 ){
				        				$monta++;
				        				$arqXmlSt .= '
				        							<sub_acao>
				        								<nu_sub_acao>'.$subacao.'</nu_sub_acao>';
				        			}
									$arqXmlSt .= '
										<item>
											<nu_seq_pregao>'.$pregao.'</nu_seq_pregao> 
											<nu_seq_item>'.$item['idsigarp'].'</nu_seq_item> 
											<qt_item>'.$item['quantidade'].'</qt_item>  
											<vl_pagamento>'.$item['valor'].'</vl_pagamento>
										</item>';
									$arItemEnviado[$item['idsigarp']]['pregao'] = $pregao;
									$arItemEnviado[$item['idsigarp']]['valor'] = $item['valor'];
									$arItemEnviado[$item['idsigarp']]['quantidade'] = $item['quantidade'];
									$arItemEnviado[$item['idsigarp']]['icoid'] = $item['icoid'];
									
									$sqlInsertItemPerd .= "INSERT INTO par.empenhopregaoitemperdido ( prpid, sbdid, icoid ) VALUES (".$dados['prpid'].", ".$item['sbdid'].", ".$item['icoid']."); ";
									
									$itEnviado[] = array('sbdid' => $item['sbdid'],
														'icoid' => $item['icoid']);
				        		}
			        		}
		        		}
		        		$pregao = false;
					}
				}
			}
			if( $montou == true ){
        		$arqXmlSt .= '
        					</sub_acao>';
        	}
		$arqXmlSt .= '</params>
    </body>
</request>';

$arqXml = <<<XML
{$arqXmlSt}
XML;
	
			if( $it == 0 ){ //Não tem nenhum item para enviar!
				echo "WS-Sigarp: Nenhum item para enviar. Verifique os pregões.";
				return true;
			} else {
				if( $sqlInsertItemPerd ){
					$sqlDeleta = "DELETE FROM par.empenhopregaoitemperdido WHERE prpid = ".$dados['prpid'];
					$this->db->executar($sqlDeleta);
					$this->db->executar($sqlInsertItemPerd);
					$this->db->commit();
				}
			}
	
	 /*
	echo '<pre>';		
	echo $arqXml;
	die();
		*/  		
			if( $_SESSION['baselogin'] == "simec_desenvolvimento" || $_SESSION['baselogin'] == "simec_espelho_producao" ){
				$urlWS = 'http://172.20.65.98/webservices/wssigarp/ws/item';
			} else {
				$urlWS = 'http://www.fnde.gov.br/webservices/wssigarp/ws/item';
			}
			$xml = Fnde_Webservice_Client::CreateRequest()
					->setURL($urlWS)
					->setParams( array('xml' => $arqXml, 'method' => 'solicitar') )
					->execute();
						
			$xmlRetorno = $xml;
								
			$xml = simplexml_load_string( stripslashes($xml));
	
			$result = (integer) $xml->status->result;
			
		//	print_r ($xml);
		//	die();
	
			if(!$result) { //ERRO
				
				$sql = "INSERT INTO par.historicowsprocessopar(
					    	prpid,
					    	hwpwebservice,
					    	hwpxmlenvio,
					    	hwpxmlretorno,
					    	hwpdataenvio,
					        usucpf)
					    VALUES ('".$dados['prpid']."',
					    		'enviarManualmenteItensSigarp - Erro',
					    		'".addslashes($arqXml)."',
					    		'".addslashes($xmlRetorno)."',
					    		NOW(),
					            '".$_SESSION['usucpf']."');";
	
				$this->db->executar($sql);
				$this->db->commit();
				
				if( (integer) $xml->status->error->message->code == 25 ){ // Fnde: Já existe solicitaçao de adesão para esse processo. Utilizar o método reformular.
					$sigarp = $this->aderirPregaoReformulacao($dados, $tipo, $nivel);
					return;
				}
				
				$erros = $xml->status->error->message->text;
				echo '<div style=" border: 1px solid #B7B7B7; font-size: 10px; font-style: normal; font-family: arial; padding: 5px 5px 5px 5px;"> 
							ERRO AO TENTAR ADERIR AO PREGÃO
							<div style=" border-top: 1px solid #B7B7B7; padding-top: 5px; " >
								'.iconv("UTF-8", "ISO-8859-1", $erros).'
							</div>
						</div>
					 	<br>
						';
				
			} else {
				// Deu certo
				
				$sql = "INSERT INTO par.historicowsprocessopar(
					    	prpid,
					    	hwpwebservice,
					    	hwpxmlenvio,
					    	hwpxmlretorno,
					    	hwpdataenvio,
					        usucpf)
					    VALUES ('".$dados['prpid']."',
					    		'enviarManualmenteItensSigarp - Sucesso',
					    		'".addslashes($arqXml)."',
					    		'".addslashes($xmlRetorno)."',
					    		NOW(),
					            '".$_SESSION['usucpf']."')
					    RETURNING hwpid;";
	
				$id = $this->db->pegaUm($sql);
	                        
	            // Realiza importacao dos itens que estao no xml do historico.
	            $controle = new ProcessoItemAdesaoPregaoControle();
	            $result = $controle->transferirHistoricoProcesso($id);
	                        
				$this->db->commit();
				
				$dado = ($xml->body->params);
				foreach( $dado->adesaopregao as $adesao ){
					$dadosSub = $this->db->pegaLinha( "SELECT sbaid, sbdano FROM par.subacaodetalhe WHERE sbdid = ".(string) $adesao->nu_sub_acao);
					$sub = $dadosSub['sbaid'];
					$ano = $dadosSub['sbdano'];
					$ad = (integer) $adesao->nu_seq_solicitacao_adesao;
					foreach( $adesao->item as $item ){
						foreach( $item->nu_seq_item as $it2 ){
							$it = (integer) $it2;
							
							$pregaoEnviado = $arItemEnviado[$it]['pregao'];
							$quantidadeEnviado = $arItemEnviado[$it]['quantidade'];
							$valorEnviado = $arItemEnviado[$it]['valor'];
							$icoId = $arItemEnviado[$it]['icoid'];
							
							if($icoId != "")
							{
								$sqlItensEnviados .= "INSERT INTO par.empenhopregaoitensenviados (prpid, sbaid, epiano, idsigarp, adesao, usucpf, epistatus, epiquantidade, epivalor, epipregao, icoid) VALUES
												( ".$dados['prpid'].", ".$sub.", ".$ano.", ".$it.", ".$ad.", '".$_SESSION['usucpf']."', 'A', ".$quantidadeEnviado.", ".$valorEnviado.", ".$pregaoEnviado.", {$icoId});";
								
							}
							else
							{
								$sqlItensEnviados .= "INSERT INTO par.empenhopregaoitensenviados (prpid, sbaid, epiano, idsigarp, adesao, usucpf, epistatus, epiquantidade, epivalor, epipregao) VALUES
												( ".$dados['prpid'].", ".$sub.", ".$ano.", ".$it.", ".$ad.", '".$_SESSION['usucpf']."', 'A', ".$quantidadeEnviado.", ".$valorEnviado.", ".$pregaoEnviado.");";
							}
						}
					}
				}
				
				if($dado->canceladas){
					foreach( $dado->canceladas->nu_seq_solicitacao_adesao as $adesaocancelada ){
						$sqlCancelamento .= "UPDATE par.empenhopregaoitensenviados SET epistatus = 'I' WHERE adesao = ".$adesaocancelada."; ";
					}
					$this->db->executar($sqlCancelamento);
				}
				
				if( $sqlItensEnviados ){
					$this->db->executar( $sqlItensEnviados );
				}
				
				if( is_array($itEnviado) && $itEnviado[0] ){
					foreach( $itEnviado as $it ){
						if($it['sbdid'] && $it['icoid'] && $dados['prpid'] ){
							$sqlDelete .= " DELETE FROM par.empenhopregaoitemperdido WHERE
										prpid = ".$dados['prpid']." AND
										sbdid = ".$it['sbdid']." AND
										icoid = ".$it['icoid']."; ";
						}
					}
					$this->db->executar($sqlDelete);
					$this->db->commit();
				}
				
				echo '<div style=" border: 1px solid #B7B7B7; font-size: 10px; font-style: normal; font-family: arial; padding: 5px 5px 5px 5px;"> 
							SOLICITAÇÃO DE ADESÃO REALIZADA COM SUCESSO
						</div>
					 	<br>
						';
			}
			
		} catch (Exception $e){
	
			# Erro 404 página not found
			if($e->getCode() == 404){
				//echo "Erro-Serviço do SIGARP encontra-se temporariamente indisponível. Favor tente mais tarde.".'\n';
				echo '<div style=" border: 1px solid #B7B7B7; font-size: 10px; font-style: normal; font-family: arial; padding: 5px 5px 5px 5px;"> 
							ERRO AO TENTAR ADERIR AO PREGÃO
							<div style=" border-top: 1px solid #B7B7B7; padding-top: 5px; " >
								Erro-Serviço do SIGARP encontra-se temporariamente indisponível. Favor tente mais tarde.
							</div>
						</div>
					 	<br>
						';
				
			}
			$erroMSG = str_replace(array(chr(13),chr(10)), ' ',$e->getMessage());
			$erroMSG = str_replace( "'", '"', $erroMSG );
		
			//echo "Erro-WS Enviar dados para o SIGARP: $erroMSG";
			
			echo '<div style=" border: 1px solid #B7B7B7; font-size: 10px; font-style: normal; font-family: arial; padding: 5px 5px 5px 5px;"> 
							ERRO AO TENTAR ADERIR AO PREGÃO
							<div style=" border-top: 1px solid #B7B7B7; padding-top: 5px; " >
								Erro-WS Enviar dados para o SIGARP: '.$erroMSG.'
							</div>
						</div>
					 	<br>
						';
		}
	}
		
	// Envio comum - Validação do Termo - Reformulação
	function enviarItensSigarpReformulacao($dopid){
		global $db;
	
		try {
			
			$sql = "SELECT DISTINCT
						sd.sbaid,
						sd.sbdano,
						sd.sbdid
					FROM 
						par.termocomposicao tc
					INNER JOIN par.subacaodetalhe sd ON sd.sbdid = tc.sbdid
					WHERE
						tc.dopid = {$dopid}";
			
			$sbaids = $this->db->carregar($sql);
			
			$prpid = $this->db->pegaUm("SELECT 
									DISTINCT ppc.prpid
								FROM 
									par.processoparcomposicao ppc
								INNER JOIN par.subacaodetalhe sd ON sd.sbdid = ppc.sbdid
								INNER JOIN par.documentopar dop ON dop.prpid = ppc.prpid and dopstatus = 'A'
								WHERE
									dop.dopid = {$dopid}
									and ppc.ppcstatus = 'A'
									");
			
			$arrItens = array();
			$sbaids = $sbaids ? $sbaids : array();
			foreach( $sbaids as $sba ){
			
				// Verifico os itens da subação
				$sql = "SELECT 
							s.sbaid,
							sd.sbdano,
							sd.sbdid,
							sic.icoid,
							pic.idsigarp,
							CASE WHEN sbacronograma = 1 --global
							THEN
								CASE WHEN sic.icovalidatecnico IS NULL -- antigos
								THEN coalesce(sic.icoquantidade,0)
								ELSE -- novos
									CASE WHEN sic.icovalidatecnico = 'S' THEN -- validado (caso não o item não é contado)
										sum(coalesce(sic.icoquantidadetecnico,0))
									END
								END
							ELSE -- escolas
								par.recuperaquantidadeitemvalidado(sic.icoid)
							END as quantidade,
							sic.icovalor as valor
						FROM
							par.subacao s
						INNER JOIN par.subacaodetalhe 		     sd  ON sd.sbaid = s.sbaid
					--	INNER JOIN par.processopar 	             pr  ON pr.prpid = sd.prpid and pr.prpstatus = 'A'
						LEFT  JOIN par.subacaoitenscomposicao 	 sic ON sic.sbaid = s.sbaid  AND sic.icoano = sd.sbdano AND sic.icostatus = 'A'
						LEFT  JOIN par.subacaoescolas 		     se  ON se.sbaid = sic.sbaid AND se.sesano = sic.icoano
						LEFT  JOIN par.subescolas_subitenscomposicao ssi ON ssi.sesid = se.sesid AND ssi.icoid = sic.icoid
						INNER JOIN par.propostaitemcomposicao        pic ON pic.picid = sic.picid
						WHERE
							s.sbastatus = 'A' AND sd.sbdid IN ( ".$sba['sbdid']." ) AND  
							
							 CASE WHEN sbacronograma = 1 
		                     THEN sic.icovalidatecnico <> 'N' 
		                     ELSE 
		                     	CASE WHEN (s.frmid = 2) OR ( s.frmid = 4 AND s.ptsid = 42 ) OR ( s.frmid = 12 AND s.ptsid = 46 ) THEN
		                        	se.sesvalidatecnico <> 'N'
		                        ELSE
		                        	sic.icovalidatecnico <> 'N' 
		                        END
		                     END
		                group by
							s.sbaid, sd.sbdano, sd.sbdid, sic.icoid, pic.idsigarp, s.sbacronograma, sic.icovalidatecnico, sic.icoquantidade, s.frmid, s.ptsid, se.sesvalidatecnico, sic.icovalor
						ORDER BY
							s.sbaid";
//	ver($sql, d);
				$arrItens[$sba['sbdid']] = $this->db->carregar( $sql );
				$arrAnos[$sba['sbdid']] = $sba['sbdano'];
				
			}
			
//			ver($arrAnos, d);	
			$dadosse = $this->db->pegaLinha("SELECT prpnumeroprocesso, prpbanco, prpagencia, muncod, prptipo, inuid, prptipoexecucao FROM par.processopar WHERE prpstatus = 'A' and prpid='".$prpid."'");
			
			$dadosTemo = $this->db->pegaLinha("SELECT dopnumerodocumento, dopdatafimvigencia FROM par.documentopar WHERE dopid = ".$dopid);
			if(strlen($dadosTemo['dopdatafimvigencia']) == 7){
				$mes = substr($dadosTemo['dopdatafimvigencia'], 0, 2);
				$ano = substr($dadosTemo['dopdatafimvigencia'], 3, 4);
				$vigencia = $ano."-".$mes;
			} else {
				$mes = substr($dadosTemo['dopdatafimvigencia'], 3, 2);
				$ano = substr($dadosTemo['dopdatafimvigencia'], 6, 4);
				$vigencia = $ano."-".$mes;
			}
	
			$cnpj = $this->db->pegaUm("SELECT prpcnpj FROM par.processopar WHERE prpstatus = 'A' and prpid = ".$prpid);
			
			$data_created = date("c");
			$usuario = USUARIO_SIGARP;
			$senha   = SENHA_SIGARP;
			
			$itPerdido = array();
			$it = 0;
			$cont = 0;
			$montou = false;
			$ultimaSubacao = 0;
			$itens2 = array();
			$sqlDeleta = "DELETE FROM par.empenhopregaoitemperdido WHERE prpid = ".$prpid;
			$this->db->executar($sqlDeleta);
			$this->db->commit();
		
$arqXmlSt = 
'<?xml version="1.0" encoding="ISO-8859-1"?>
<request>
    <header>
        <app>SIGARP</app>
        <version>1.0</version>
        <created>'.$data_created.'</created>
    </header>
    <body>
        <auth>
            <usuario>'.$usuario.'</usuario>
            <senha>'.$senha.'</senha>
        </auth>
        <params>
        	<cnpj>'.$cnpj.'</cnpj>
        	<nu_processo>'.$dadosse['prpnumeroprocesso'].'</nu_processo> 
        	<nu_seq_tipo_pagamento>1</nu_seq_tipo_pagamento>
        	<nu_termo_compromisso>'.$dadosTemo['dopnumerodocumento'].'</nu_termo_compromisso>
        	<dt_vigencia>'.$vigencia.'</dt_vigencia>';
			foreach( $arrItens as $subacao => $itens ){
        		$monta = 0;
				if( $itens[0] ){
		        	foreach( $itens as $item ){
		        		if($item['idsigarp']){
		        			$sql = "SELECT 
									CASE WHEN sbacronograma = 1 --global
									THEN
										CASE WHEN sic.icovalidatecnico IS NULL -- antigos
										THEN coalesce(sic.icoquantidade,0)
										ELSE -- novos
											CASE WHEN sic.icovalidatecnico = 'S' THEN -- validado (caso não o item não é contado)
												sum(coalesce(sic.icoquantidadetecnico,0))
											END
										END
									ELSE -- escolas
										par.recuperaquantidadeitemvalidado(sic.icoid)
									END as quantidade,
									sic.icovalor,
									s.sbaid
								FROM 
									par.subacaodetalhe sd
								INNER JOIN par.subacao s 				  ON s.sbaidpai = sd.sbaid
								INNER JOIN par.subacaoitenscomposicao sic ON sic.sbaid = s.sbaid and sic.icoano = sd.sbdano AND sic.icovalidatecnico = 'S'
								LEFT  JOIN par.subacaoescolas 		     se  ON se.sbaid = sic.sbaid AND se.sesano = sic.icoano
								LEFT  JOIN par.subescolas_subitenscomposicao ssi ON ssi.sesid = se.sesid AND ssi.icoid = sic.icoid
								INNER JOIN par.propostaitemcomposicao pic ON pic.picid = sic.picid AND idsigarp = ".$item['idsigarp']."
								WHERE 
									sd.sbdid = ".$subacao." -- AND sd.sbdano = ".$arrAnos[$subacao]."
								GROUP BY
									s.sbacronograma, sic.icovalidatecnico, sic.icoquantidade,s.frmid, s.ptsid, se.sesvalidatecnico, sic.icovalor, s.sbaid, sic.icoid
								ORDER BY
									s.sbaid DESC
								LIMIT 1";

			        		$dadosItemAntigo = $db->pegaLinha($sql);
			        		
			        		$itens2[$cont]['sbdid'] = $item['sbdid'];
			        		$itens2[$cont]['icoid'] = $item['icoid'];
			        		$cont++;
		        		
			        		$pregao = $this->diponibilizaPregoesItens($item['idsigarp'],'pregao',$prpid);
			        		if( $item['quantidade'] <> 0 ){
				        		if( $pregao ){
				        			
				        			$valorAtual = $this->diponibilizaPregoesItens($item['idsigarp'],'valor',$prpid);
									$acao = 'A';
				        			
									if( is_array($dadosItemAntigo) ){
										//if( $dadosItemAntigo['quantidade'] == $item['quantidade'] && number_format($dadosItemAntigo['icovalor'],2,",",".") == number_format( str_replace( array(".", ","), array("", "."), $valorAtual) ,2,",",".") ){
										if( $dadosItemAntigo['quantidade'] == $item['quantidade'] && $dadosItemAntigo['icovalor'] == $item['valor'] ){
											$acao = 'I';
										}
									} else {
										$sql = "SELECT count(sbdreformulacao) FROM par.subacaodetalhe WHERE sbdreformulacao IS NOT NULL AND sbdid = ".$subacao;
										$testaReformulacao = $this->db->pegaUm($sql);
										
										if( $testaReformulacao > 0 ){
											$acao = 'A';
										} else {
											$acao = 'I';
										}
									}
				        			
				        			$it++;
			        				$montou = true;
				        			if( $subacao != $ultimaSubacao && $ultimaSubacao != 0 ){
				        				$arqXmlSt .= '
				        							</sub_acao>';
				        			}
				        			$ultimaSubacao = $subacao;
				        			if( $monta == 0 ){
				        				$monta++;
				        				$arqXmlSt .= '
				        							<sub_acao>
				        								<nu_sub_acao>'.$subacao.'</nu_sub_acao>';
				        			}
									$arqXmlSt .= '
										<item>
											<nu_seq_pregao>'.$pregao.'</nu_seq_pregao> 
											<nu_seq_item>'.$item['idsigarp'].'</nu_seq_item> 
											<qt_item>'.$item['quantidade'].'</qt_item>  
											<vl_pagamento>'.$item['valor'].'</vl_pagamento>
											<acao>'.$acao.'</acao>
										</item>';
									
									$arItemEnviado[$item['idsigarp']]['pregao'] = $pregao;
									$arItemEnviado[$item['idsigarp']]['quantidade'] = $item['quantidade'];
									$arItemEnviado[$item['idsigarp']]['valor'] = $item['valor'];
									$arItemEnviado[$item['idsigarp']]['icoid'] = $item['icoid'];
									
									$sqlInsertItemPerd = "INSERT INTO par.empenhopregaoitemperdido ( prpid, sbdid, icoid ) VALUES (".$prpid.", ".$item['sbdid'].", ".$item['icoid'].")";
									$this->db->executar($sqlInsertItemPerd);
									$this->db->commit();
									
				        		} else {
				        			$itPerdido[] = array('sbdid' => $item['sbdid'],
														'icoid' => $item['icoid']);
				        		}
			        		}
		        		}
		        		$pregao = false;
					}
				}
			}
			if( $montou == true ){
        		$arqXmlSt .= '
        					</sub_acao>';
        	}
		$arqXmlSt .= '</params>
    </body>
</request>';
		/*
ver(simec_htmlentities($arqXmlSt));
die();
*/
		
$arqXml = <<<XML
{$arqXmlSt}
XML;

			if( $it == 0 ){ //Não tem nenhum item para enviar!
				foreach( $itens2 as $item ){ // Salvo todos os itens que estão sendo empenhados!
					if($item['sbdid'] && $item['icoid'] && $prpid ){
						$sqlInsertItem .= " INSERT INTO par.empenhopregaoitemperdido (
									prpid,
									sbdid,
									icoid )
								VALUES
									(".$prpid.",
									".$item['sbdid'].",
									".$item['icoid']."); ";
					}
				}
				if($sqlInsertItem){
					$this->db->executar($sqlInsertItem);
					$this->db->commit();
				}
				return true;
			}
			
			if( $_SESSION['baselogin'] == "simec_desenvolvimento" || $_SESSION['baselogin'] == "simec_espelho_producao" ){
				$urlWS = 'http://172.20.65.98/webservices/wssigarp/ws/termo';
			} else {
	//			$urlWS = 'http://172.20.65.98/webservices/wssigarp/ws/solicitacao';
				$urlWS = 'http://www.fnde.gov.br/webservices/wssigarp/ws/termo';
			}
			
			$xml = Fnde_Webservice_Client::CreateRequest()
					->setURL($urlWS)
					->setParams( array('xml' => $arqXml, 'method' => 'subacao') )
					->execute();
						
			$xmlRetorno = $xml;
								
	//		ver($xml);
			$xml = simplexml_load_string( stripslashes($xml));
	
			$result = (integer) $xml->status->result;
	
	//		ver($xml);
			if(!$result) { //ERRO
				
				
				$sql = "INSERT INTO par.historicowsprocessopar(
					    	prpid,
					    	hwpwebservice,
					    	hwpxmlenvio,
					    	hwpxmlretorno,
					    	hwpdataenvio,
					        usucpf)
					    VALUES ('".$prpid."',
					    		'enviarItensSigarpReformulacao - Erro',
					    		'".addslashes($arqXml)."',
					    		'".addslashes($xmlRetorno)."',
					    		NOW(),
					            '".$_SESSION['usucpf']."');";
	                        
				$this->db->executar($sql);
				$this->db->commit();
				
				if( (integer) $xml->status->error->message->code == 4 ){ // Fnde: Não existe solicitação de adesão para esse processo. Utilize o método solicitar.
					$sigarp = $this->enviarItensSigarp();
					return;
				}
				
			} else {
				// Deu certo
				
				$sql = "INSERT INTO par.historicowsprocessopar(
					    	prpid,
					    	hwpwebservice,
					    	hwpxmlenvio,
					    	hwpxmlretorno,
					    	hwpdataenvio,
					        usucpf)
					    VALUES ('".$prpid."',
					    		'enviarItensSigarpReformulacao - Sucesso',
					    		'".addslashes($arqXml)."',
					    		'".addslashes($xmlRetorno)."',
					    		NOW(),
					            '".$_SESSION['usucpf']."')
	                                RETURNING hwpid;";
	
				$id = $this->db->pegaUm($sql);
	                        
				// Realiza importacao dos itens que estao no xml do historico.
	            $controle = new ProcessoItemAdesaoPregaoControle();
	            $result = $controle->transferirHistoricoProcesso($id);
	                        
	            $this->db->commit();
				
				$sqlDeleta = "DELETE FROM par.empenhopregaoitemperdido WHERE prpid = ".$prpid;
				$this->db->executar($sqlDeleta);
				$this->db->commit();
				
				$dados = ($xml->body->params);
				foreach( $dados->adesaopregao as $adesao ){
					$dadosSub = $this->db->pegaLinha( "SELECT sbaid, sbdano FROM par.subacaodetalhe WHERE sbdid = ".(string) $adesao->nu_sub_acao);
					$sub = $dadosSub['sbaid'];
					$ano = $dadosSub['sbdano'];
					$ad = (integer) $adesao->nu_seq_solicitacao_adesao;
					foreach( $adesao->item as $item ){
						foreach( $item->nu_seq_item as $it2 ){
							$it = (integer) $it2;
							
							$pregaoEnviado = $arItemEnviado[$it]['pregao'];
							$quantidadeEnviado = $arItemEnviado[$it]['quantidade'];
							$valorEnviado = $arItemEnviado[$it]['valor'];
							$icoId = $arItemEnviado[$it]['icoid'];
							$sqlUpdate .= "UPDATE par.empenhopregaoitensenviados SET epistatus = 'I' WHERE idsigarp = ".$it." AND prpid = ".$prpid." AND sbaid = ".$sub." AND epiano = ".$ano."; ";
							
							if( $icoId != "")
							{
								$sqlItensEnviados .= "INSERT INTO par.empenhopregaoitensenviados (prpid, sbaid, epiano, idsigarp, adesao, usucpf, epistatus, epiquantidade, epivalor, epipregao, icoid) VALUES
												( ".$prpid.", ".$sub.", ".$ano.", ".$it.", ".$ad.", '".$_SESSION['usucpf']."', 'A', ".$quantidadeEnviado.", ".$valorEnviado.", ".$pregaoEnviado.", {$icoId});";
							}
							else
							{
								$sqlItensEnviados .= "INSERT INTO par.empenhopregaoitensenviados (prpid, sbaid, epiano, idsigarp, adesao, usucpf, epistatus, epiquantidade, epivalor, epipregao) VALUES
												( ".$prpid.", ".$sub.", ".$ano.", ".$it.", ".$ad.", '".$_SESSION['usucpf']."', 'A', ".$quantidadeEnviado.", ".$valorEnviado.", ".$pregaoEnviado.");";
							}
								
							
//							$sqlUpdate .= "UPDATE par.empenhopregaoitensenviados SET epistatus = 'I' WHERE idsigarp = ".$it." AND prpid = ".$prpid." AND sbaid = ".$sub." AND epiano = ".$ano."; ";
//							$sqlItensEnviados .= "INSERT INTO par.empenhopregaoitensenviados (prpid, sbaid, epiano, idsigarp, adesao, usucpf, epistatus) VALUES ( ".$prpid.", ".$sub.", ".$ano.", ".$it.", ".$ad.", '".$_SESSION['usucpf']."', 'A' );";
						}
					}
				}
				
				if($dados->canceladas){
					foreach( $dados->canceladas->nu_seq_solicitacao_adesao as $adesaocancelada ){
						$sqlCancelamento .= "UPDATE par.empenhopregaoitensenviados SET epistatus = 'I' WHERE adesao = ".$adesaocancelada."; ";
					}
					$this->db->executar($sqlCancelamento);
				}
				
				if( $sqlUpdate ){
					$this->db->executar( $sqlUpdate );
				}
	
				if( $sqlItensEnviados ){
					$this->db->executar( $sqlItensEnviados );
				}
				
				if( is_array($itPerdido) && $itPerdido[0] ){
					foreach( $itPerdido as $it ){
						if($prpid && $it['sbdid'] && $it['icoid'] ){
							$sqlInsert .= " INSERT INTO par.empenhopregaoitemperdido (
										prpid,
										sbdid,
										icoid )
									VALUES
										(".$prpid.",
										".$it['sbdid'].",
										".$it['icoid']."); ";
						}
					}
					$this->db->executar($sqlInsert);
					$this->db->commit();
				}
				
				return true;
			}
			
		} catch (Exception $e){
	
			# Erro 404 página not found
			if($e->getCode() == 404){
				//echo "Erro-Serviço do SIGARP encontra-se temporariamente indisponível. Favor tente mais tarde.".'\n';
				echo '<div style=" border: 1px solid #B7B7B7; font-size: 10px; font-style: normal; font-family: arial; padding: 5px 5px 5px 5px;"> 
							ERRO AO TENTAR ADERIR AO PREGÃO
							<div style=" border-top: 1px solid #B7B7B7; padding-top: 5px; " >
								Erro-Serviço do SIGARP encontra-se temporariamente indisponível. Favor tente mais tarde.
							</div>
						</div>
					 	<br>
						';
				
			}
			$erroMSG = str_replace(array(chr(13),chr(10)), ' ',$e->getMessage());
			$erroMSG = str_replace( "'", '"', $erroMSG );
		
			//echo "Erro-WS Enviar dados para o SIGARP: $erroMSG";
			
			echo '<div style=" border: 1px solid #B7B7B7; font-size: 10px; font-style: normal; font-family: arial; padding: 5px 5px 5px 5px;"> 
							ERRO AO TENTAR ADERIR AO PREGÃO
							<div style=" border-top: 1px solid #B7B7B7; padding-top: 5px; " >
								Erro-WS Enviar dados para o SIGARP: '.$erroMSG.'
							</div>
						</div>
					 	<br>
						';
			
		}
	}
	
	// Envio comum - Validação do Termo
	function enviarItensSigarp($dopid){
		global $db;
	
		try {
			
			$sql = "SELECT DISTINCT
						sd.sbaid,
						sd.sbdano,
						sd.sbdid
					FROM 
						par.termocomposicao tc
					INNER JOIN par.subacaodetalhe sd ON sd.sbdid = tc.sbdid
					WHERE
						tc.dopid = {$dopid}";
			
			$sbaids = $this->db->carregar($sql);
			
			$prpid = $this->db->pegaUm("SELECT 
									DISTINCT ppc.prpid
								FROM 
									par.processoparcomposicao ppc
								INNER JOIN par.subacaodetalhe sd ON sd.sbdid = ppc.sbdid
								INNER JOIN par.documentopar dop ON dop.prpid = ppc.prpid and dopstatus = 'A'
								WHERE
									dop.dopid = {$dopid}
									and ppc.ppcstatus = 'A'
									");
			
			$arrItens = array();
			$sbaids = $sbaids ? $sbaids : array();
			foreach( $sbaids as $sba ){
			
				// Verifico os itens da subação
				$sql = "SELECT 
							s.sbaid,
							sd.sbdano,
							sd.sbdid,
							sic.icoid,
							pic.idsigarp,
							CASE WHEN sbacronograma = 1 --global
							THEN
								CASE WHEN sic.icovalidatecnico IS NULL -- antigos
								THEN coalesce(sic.icoquantidade,0)
								ELSE -- novos
									CASE WHEN sic.icovalidatecnico = 'S' THEN -- validado (caso não o item não é contado)
										sum(coalesce(sic.icoquantidadetecnico,0))
									END
								END
							ELSE -- escolas
								par.recuperaquantidadeitemvalidado(sic.icoid)
							END as quantidade,
							sic.icovalor as valor
						FROM
							par.subacao s
						INNER JOIN par.subacaodetalhe 		     sd  ON sd.sbaid = s.sbaid
					--	INNER JOIN par.processopar 	             pr  ON pr.prpid = sd.prpid and pr.prpstatus = 'A'
						LEFT  JOIN par.subacaoitenscomposicao 	 sic ON sic.sbaid = s.sbaid  AND sic.icoano = sd.sbdano AND sic.icostatus = 'A'
						LEFT  JOIN par.subacaoescolas 		     se  ON se.sbaid = sic.sbaid AND se.sesano = sic.icoano
						LEFT  JOIN par.subescolas_subitenscomposicao ssi ON ssi.sesid = se.sesid AND ssi.icoid = sic.icoid
						INNER JOIN par.propostaitemcomposicao        pic ON pic.picid = sic.picid
						WHERE
							s.sbastatus = 'A' AND sd.sbdid IN ( ".$sba['sbdid']." ) AND -- sd.sbdano IN ( ".$sba['sbdano']." ) AND 
							
							 CASE WHEN sbacronograma = 1 
		                     THEN sic.icovalidatecnico <> 'N' 
		                     ELSE 
		                     	CASE WHEN (s.frmid = 2) OR ( s.frmid = 4 AND s.ptsid = 42 ) OR ( s.frmid = 12 AND s.ptsid = 46 ) THEN
		                        	se.sesvalidatecnico <> 'N'
		                        ELSE
		                        	sic.icovalidatecnico <> 'N' 
		                        END
		                     END
		                group by
							s.sbaid, sd.sbdano, sd.sbdid, sic.icoid, pic.idsigarp, s.sbacronograma, sic.icovalidatecnico, sic.icoquantidade, s.frmid, s.ptsid, se.sesvalidatecnico, sic.icovalor
						ORDER BY
							s.sbaid";
	
				$arrItens[$sba['sbdid']] = $this->db->carregar( $sql );
				$arrAnos[$sba['sbdid']] = $sba['sbdano'];
			}
	//		ver($sql, d);	
			$dadosse = $this->db->pegaLinha("SELECT prpnumeroprocesso, prpbanco, prpagencia, muncod, prptipo, inuid, prptipoexecucao FROM par.processopar WHERE prpid='".$prpid."' and prpstatus = 'A'");
	
			$dadosTemo = $this->db->pegaLinha("SELECT dopnumerodocumento, dopdatafimvigencia FROM par.documentopar WHERE dopid = ".$dopid);
			
			if(strlen($dadosTemo['dopdatafimvigencia']) == 7){
				$mes = substr($dadosTemo['dopdatafimvigencia'], 0, 2);
				$ano = substr($dadosTemo['dopdatafimvigencia'], 3, 4);
				$vigencia = $ano."-".$mes;
			} else {
				$mes = substr($dadosTemo['dopdatafimvigencia'], 3, 2);
				$ano = substr($dadosTemo['dopdatafimvigencia'], 6, 4);
				$vigencia = $ano."-".$mes;
			}
			
			$cnpj = $this->db->pegaUm("SELECT prpcnpj FROM par.processopar WHERE prpstatus = 'A' and prpid = ".$prpid);
			
			$data_created = date("c");
			$usuario = USUARIO_SIGARP;
			$senha   = SENHA_SIGARP;
			
			$itPerdido = array();
			$it = 0;
			$cont = 0;
			$montou = false;
			$ultimaSubacao = 0;
			$itens2 = array();
			$sqlDeleta = "DELETE FROM par.empenhopregaoitemperdido WHERE prpid = ".$prpid;
			$this->db->executar($sqlDeleta);
			$this->db->commit();
		
$arqXmlSt = 
'<?xml version="1.0" encoding="ISO-8859-1"?>
<request>
    <header>
        <app>SIGARP</app>
        <version>1.0</version>
        <created>'.$data_created.'</created>
    </header>
    <body>
        <auth>
            <usuario>'.$usuario.'</usuario>
            <senha>'.$senha.'</senha>
        </auth>
        <params>
        	<cnpj>'.$cnpj.'</cnpj>
        	<nu_processo>'.$dadosse['prpnumeroprocesso'].'</nu_processo> 
        	<nu_seq_tipo_pagamento>1</nu_seq_tipo_pagamento>
        	<nu_termo_compromisso>'.$dadosTemo['dopnumerodocumento'].'</nu_termo_compromisso>
        	<dt_vigencia>'.$vigencia.'</dt_vigencia>';
			foreach( $arrItens as $subacao => $itens ){
        		$monta = 0;
				if( $itens[0] ){
		        	foreach( $itens as $item ){
		        		$itens2[$cont]['sbdid'] = $item['sbdid'];
		        		$itens2[$cont]['icoid'] = $item['icoid'];
		        		$cont++;
		        		if($item['idsigarp']){
			        		$pregao = $this->diponibilizaPregoesItens($item['idsigarp'],'pregao',$prpid);
			        		$valor = $this->diponibilizaPregoesItens($item['idsigarp'],'valor',$prpid);
			        		if( $item['quantidade'] <> 0 ){
				        		if( $pregao ){
				        			$it++;
			        				$montou = true;
				        			if( $subacao != $ultimaSubacao && $ultimaSubacao != 0 ){
				        				$arqXmlSt .= '
				        							</sub_acao>';
				        			}
				        			$ultimaSubacao = $subacao;
				        			if( $monta == 0 ){
				        				$monta++;
				        				$arqXmlSt .= '
				        							<sub_acao>
				        								<nu_sub_acao>'.$subacao.'</nu_sub_acao>';
				        			}
									$arqXmlSt .= '
										<item>
											<nu_seq_pregao>'.$pregao.'</nu_seq_pregao> 
											<nu_seq_item>'.$item['idsigarp'].'</nu_seq_item> 
											<qt_item>'.$item['quantidade'].'</qt_item>  
											<vl_pagamento>'.$item['valor'].'</vl_pagamento>
										</item>';
										$arItemEnviado[$item['idsigarp']]['pregao'] = $pregao;
										$arItemEnviado[$item['idsigarp']]['valor'] = $item['valor'];
										$arItemEnviado[$item['idsigarp']]['quantidade'] = $item['quantidade'];
										$arItemEnviado[$item['idsigarp']]['icoid'] = $item['icoid'];
										
										$sqlInsertItemPerd = "INSERT INTO par.empenhopregaoitemperdido ( prpid, sbdid, icoid ) VALUES (".$prpid.", ".$item['sbdid'].", ".$item['icoid'].")";
										$this->db->executar($sqlInsertItemPerd);
										$this->db->commit();
				        		} else {
				        			$itPerdido[] = array('sbdid' => $item['sbdid'],
														'icoid' => $item['icoid']);
				        		}
			        		}
		        		}
		        		$pregao = false;
					}
				}
			}
			if( $montou == true ){
        		$arqXmlSt .= '
        					</sub_acao>';
        	}
		$arqXmlSt .= '</params>
    </body>
</request>';
		/*
ver(simec_htmlentities($arqXmlSt));
die();
*/
$arqXml = <<<XML
{$arqXmlSt}
XML;

			if( $it == 0 ){ //Não tem nenhum item para enviar!
				foreach( $itens2 as $item ){ // Salvo todos os itens que estão sendo empenhados!
					if($item['sbdid'] && $item['icoid'] && $prpid ){
						$sqlInsertItem .= " INSERT INTO par.empenhopregaoitemperdido (
									prpid,
									sbdid,
									icoid )
								VALUES
									(".$prpid.",
									".$item['sbdid'].",
									".$item['icoid']."); ";
					}
				}
				if( $sqlInsertItem != '' ){
					$this->db->executar($sqlInsertItem);
				}
				$this->db->commit();
				return true;
			}
	
			if( $_SESSION['baselogin'] == "simec_desenvolvimento" || $_SESSION['baselogin'] == "simec_espelho_producao" ){
				$urlWS = 'http://172.20.65.98/webservices/wssigarp/ws/item';
			} else {
	//			$urlWS = 'http://172.20.65.98/webservices/wssigarp/ws/item';
				$urlWS = 'http://www.fnde.gov.br/webservices/wssigarp/ws/item';
			}
			
			$xml = Fnde_Webservice_Client::CreateRequest()
					->setURL($urlWS)
					->setParams( array('xml' => $arqXml, 'method' => 'solicitar') )
					->execute();
						
			$xmlRetorno = $xml;
								
			$xml = simplexml_load_string( stripslashes($xml));
	
			$result = (integer) $xml->status->result;
	
			if(!$result) { //ERRO
				
				$sql = "INSERT INTO par.historicowsprocessopar(
					    	prpid,
					    	hwpwebservice,
					    	hwpxmlenvio,
					    	hwpxmlretorno,
					    	hwpdataenvio,
					        usucpf)
					    VALUES ('".$prpid."',
					    		'enviarItensSigarp - Erro',
					    		'".addslashes($arqXml)."',
					    		'".addslashes($xmlRetorno)."',
					    		NOW(),
					            '".$_SESSION['usucpf']."');";
	                        
				$this->db->executar($sql);
				$this->db->commit();
				
				if( (integer) $xml->status->error->message->code == 25 ){ // Fnde: Já existe solicitaçao de adesão para esse processo. Utilizar o método reformular.
					$sigarp = $this->enviarItensSigarpReformulacao( $dopid );
					return;
				}
				
				if( is_array($itens2) && $itens2[0] ){
					foreach( $itens2 as $item ){
					if($item['sbdid'] && $item['icoid'] && $prpid ){
						$sqlInsert .= " INSERT INTO par.empenhopregaoitemperdido (
									prpid,
									sbdid,
									icoid )
								VALUES
									(".$prpid.",
									".$item['sbdid'].",
									".$item['icoid']."); ";
						}
					}
	                                
	                                
					$this->db->executar($sqlInsert);
					$this->db->commit();
				}
				
			} else {
				// Deu certo

				$sql = "INSERT INTO par.historicowsprocessopar(
					    	prpid,
					    	hwpwebservice,
					    	hwpxmlenvio,
					    	hwpxmlretorno,
					    	hwpdataenvio,
					        usucpf)
					    VALUES ('".$prpid."',
					    		'enviarItensSigarp - Sucesso',
					    		'".addslashes($arqXml)."',
					    		'".addslashes($xmlRetorno)."',
					    		NOW(),
					            '".$_SESSION['usucpf']."')
	                                RETURNING hwpid;";
	
				$id = $this->db->pegaUm($sql);
	                        
				// Realiza importacao dos itens que estao no xml do historico.
	            $controle = new ProcessoItemAdesaoPregaoControle();
	            $result = $controle->transferirHistoricoProcesso($id);
	                        
	            $this->db->commit();
				
				$dados = ($xml->body->params);
				foreach( $dados->adesaopregao as $adesao ){
					$dadosSub = $this->db->pegaLinha( "SELECT sbaid, sbdano FROM par.subacaodetalhe WHERE sbdid = ".(string) $adesao->nu_sub_acao);
					$sub = $dadosSub['sbaid'];
					$ano = $dadosSub['sbdano'];
					$ad = (integer) $adesao->nu_seq_solicitacao_adesao;
					foreach( $adesao->item as $it2 ){
						foreach( $it2->nu_seq_item as $it3){
							$it = (integer) $it3;
							$pregaoEnviado = $arItemEnviado[$it]['pregao'];
							$quantidadeEnviado = $arItemEnviado[$it]['quantidade'];
							$valorEnviado = $arItemEnviado[$it]['valor'];
							$icoId = $arItemEnviado[$it]['icoid'];
							
							if( $icoId != "")
							{
								$sqlItensEnviados .= "INSERT INTO par.empenhopregaoitensenviados (prpid, sbaid, epiano, idsigarp, adesao, usucpf, epistatus, epiquantidade, epivalor, epipregao, icoid) VALUES
													( ".$prpid.", ".$sub.", ".$ano.", ".$it.", ".$ad.", '".$_SESSION['usucpf']."', 'A', ".$quantidadeEnviado.", ".$valorEnviado.", ".$pregaoEnviado.", {$icoId});";
							
							}						
							else 
							{
								$sqlItensEnviados .= "INSERT INTO par.empenhopregaoitensenviados (prpid, sbaid, epiano, idsigarp, adesao, usucpf, epistatus, epiquantidade, epivalor, epipregao) VALUES
													( ".$prpid.", ".$sub.", ".$ano.", ".$it.", ".$ad.", '".$_SESSION['usucpf']."', 'A', ".$quantidadeEnviado.", ".$valorEnviado.", ".$pregaoEnviado.");";
							}
							
						}
					}
				}
				
				if($dados->canceladas){
					foreach( $dados->canceladas->nu_seq_solicitacao_adesao as $adesaocancelada ){
						$sqlCancelamento .= "UPDATE par.empenhopregaoitensenviados SET epistatus = 'I' WHERE adesao = ".$adesaocancelada."; ";
					}
					$this->db->executar($sqlCancelamento);
				}
				
				if( $sqlItensEnviados ){
					$this->db->executar( $sqlItensEnviados );
				}
				
				$sqlDeleta = "DELETE FROM par.empenhopregaoitemperdido WHERE prpid = ".$prpid;
				$this->db->executar($sqlDeleta);
				$this->db->commit();
				
	                        
				if( is_array($itPerdido) && $itPerdido[0] ){
					foreach( $itPerdido as $it ){
						if($prpid && $it['sbdid'] && $it['icoid'] ){
							$sqlInsert .= " INSERT INTO par.empenhopregaoitemperdido (
										prpid,
										sbdid,
										icoid )
									VALUES
										(".$prpid.",
										".$it['sbdid'].",
										".$it['icoid']."); ";
						}
					}
					$this->db->executar($sqlInsert);
					$this->db->commit();
				}
				
				return true;
			}
			
		} catch (Exception $e){
	
			# Erro 404 página not found
			if($e->getCode() == 404){
				//echo "Erro-Serviço do SIGARP encontra-se temporariamente indisponível. Favor tente mais tarde.".'\n';
				echo '<div style=" border: 1px solid #B7B7B7; font-size: 10px; font-style: normal; font-family: arial; padding: 5px 5px 5px 5px;"> 
							ERRO AO TENTAR ADERIR AO PREGÃO
							<div style=" border-top: 1px solid #B7B7B7; padding-top: 5px; " >
								Erro-Serviço do SIGARP encontra-se temporariamente indisponível. Favor tente mais tarde.
							</div>
						</div>
					 	<br>
						';
				
			}
			$erroMSG = str_replace(array(chr(13),chr(10)), ' ',$e->getMessage());
			$erroMSG = str_replace( "'", '"', $erroMSG );
		
			//echo "Erro-WS Enviar dados para o SIGARP: $erroMSG";
			
			echo '<div style=" border: 1px solid #B7B7B7; font-size: 10px; font-style: normal; font-family: arial; padding: 5px 5px 5px 5px;"> 
							ERRO AO TENTAR ADERIR AO PREGÃO
							<div style=" border-top: 1px solid #B7B7B7; padding-top: 5px; " >
								Erro-WS Enviar dados para o SIGARP: '.$erroMSG.'
							</div>
						</div>
					 	<br>
						';
			
		}
	}
		
	function diponibilizaPregoesItens($item, $tipo, $prpid){
	
		global $db;
		
		try {
			
			$data_created = date("c");
	
			$usuario = USUARIO_SIGARP;
			$senha   = SENHA_SIGARP;
			
			$sql = "SELECT inuid FROM par.processopar WHERE prpid = ".$prpid;
			$inuid = $this->db->pegaUm( $sql );
			
			$inuid = $inuid ? $inuid : $_SESSION['par']['inuid'];
			
			$sql = "SELECT
						CASE WHEN estuf IS NOT NULL 
							THEN estuf 
							ELSE mun_estuf 
						END as estuf 
					FROM
						par.instrumentounidade 
					WHERE 
						inuid = ".$inuid;
			$estuf = $this->db->pegaUm( $sql );

$arqXml = <<<XML
<?xml version="1.0" encoding="ISO-8859-1"?>
<request>
    <header>
        <app>SIGARP</app>
        <version>1.0</version>
        <created>$data_created</created>
    </header>
    <body>
        <auth>
            <usuario>$usuario</usuario>
            <senha>$senha</senha>
        </auth>
        <params>
            <nu_seq_item>{$item}</nu_seq_item>
            <uf>{$estuf}</uf>
        </params>
    </body>
</request>
XML;
	
		   /*
	echo '<pre>';		
	echo simec_htmlentities($arqXml);
	die();
			 */  		
			if( $_SESSION['baselogin'] == "simec_desenvolvimento" || $_SESSION['baselogin'] == "simec_espelho_producao" ){
//			if( $_SESSION['baselogin'] == "simec_desenvolvimento" ){
				$urlWS = 'http://172.20.65.98/webservices/wssigarp/ws/pregao';
//				$urlWS = 'http://www.fnde.gov.br/webservices/wssigarp/ws/pregao';
			} else {
				$urlWS = 'http://www.fnde.gov.br/webservices/wssigarp/ws/pregao';
			}
	
			$xml = Fnde_Webservice_Client::CreateRequest()
					->setURL($urlWS)
					->setParams( array('xml' => $arqXml, 'method' => 'listar') )
					->execute();
						
			$xmlRetorno = $xml;
			
			$xml = simplexml_load_string( stripslashes($xml));
			$result = (integer) $xml->status->result;
			
			if(!$result) {
				
				$sql = "INSERT INTO par.historicowsprocessopar(
					    	prpid,
					    	hwpwebservice,
					    	hwpxmlenvio,
					    	hwpxmlretorno,
					    	hwpdataenvio,
					        usucpf)
					    VALUES ('".$prpid."',
					    		'diponibilizaPregoesItens - Erro',
					    		'".addslashes($arqXml)."',
					    		'".addslashes($xmlRetorno)."',
					    		NOW(),
					            '".$_SESSION['usucpf']."');";
	
				$this->db->executar($sql);
				$this->db->commit();
				
				$erros = $xml->status->error->message->text;
	
			} else { //sucesso
				
				$sql = "INSERT INTO par.historicowsprocessopar(
					    	prpid,
					    	hwpwebservice,
					    	hwpxmlenvio,
					    	hwpxmlretorno,
					    	hwpdataenvio,
					        usucpf)
					    VALUES ('".$prpid."',
					    		'diponibilizaPregoesItens - Sucesso',
					    		'".addslashes($arqXml)."',
					    		'".addslashes($xmlRetorno)."',
					    		NOW(),
					            '".$_SESSION['usucpf']."');";
	
				$this->db->executar($sql);
				$this->db->commit();
	
				if( $tipo == 'valor' ){
					return $xml->body->VL_ITEM ? $xml->body->VL_ITEM : null;
				} else {
					return (integer) $xml->body->NU_SEQ_PREGAO ? $xml->body->NU_SEQ_PREGAO : false;
				}
			}
			
		} catch (Exception $e){
			
			# Erro 404 página not found
			if($e->getCode() == 404){
				//echo "Temporariamente indisponível.Favor tente mais tarde.".'\n';
				/* -- ARRUMAR DEPOIS 
				echo '<div style=" border: 1px solid #B7B7B7; font-size: 10px; font-style: normal; font-family: arial; padding: 5px 5px 5px 5px;"> 
							ERRO AO CONSULTAR ITENS DO PREGÃO (SIGARP)
							<div style=" border-top: 1px solid #B7B7B7; padding-top: 5px; " >
								Temporariamente indisponível.Favor tente mais tarde.
							</div>
						</div>
					 	<br>
						';
				*/
				
			}
			$erroMSG = str_replace(array(chr(13),chr(10)), ' ',$e->getMessage());
			$erroMSG = str_replace( "'", '"', $erroMSG );
			/* -- ARRUMAR DEPOIS 
			echo '<div style=" border: 1px solid #B7B7B7; font-size: 10px; font-style: normal; font-family: arial; padding: 5px 5px 5px 5px;"> 
							ERRO AO CONSULTAR ITENS DO PREGÃO (SIGARP)
							<div style=" border-top: 1px solid #B7B7B7; padding-top: 5px; " >
								Erro-WS SIGARP: '.$erroMSG.'
								<br>
								<pre>
								'.print_r($e).'
								</pre>
							</div>
						</div>
					 	<br>
						';
			*/
			
			//echo "Erro-WS SIGARP: $erroMSG";
			//echo "<pre>";
			//print_r($e);
			
		}
	}
	
}

function verificaSubacao($subacao){
	if( $subacao == 5390338 ){
		$subacao = 6205107;
 	}
	if( $subacao == 5361372 ){
		$subacao = 6199373;
 	}
	if( $subacao == 903268 ){
		$subacao = 3321852;
 	}
	if( $subacao == 903369 ){
		$subacao = 3321333;
 	}
	if( $subacao == 883965 ){
		$subacao = 3326736;
 	}
	if( $subacao == 882964 ){
		$subacao = 3322360;
 	}
	if( $subacao == 878255 ){
		$subacao = 3321915;
 	}
	
 	return $subacao;
}
?>