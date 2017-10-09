<?php
class WSEmpenho {
	
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
	
	public function existePta($ptrid){
		if($ptrid){
			return $this->db->pegaUm("SELECT count(1) as count FROM emenda.planotrabalho WHERE ptrid = ".$ptrid);
		}
	}
	
	private function selectParaEmpenho($ptrid, $where = '', $boCancelar = false, $exfid = ''){
		if($ptrid){
			if (!$where){
				$where = " and exf.exfnumsolempenho is null"; 
			} else {
				$where = " and exf.exfnumsolempenho is not null";
			}
			
			if( !empty( $exfid ) ){
				$where .= " and exf.exfid = $exfid";
			}
			 
			$sql = "SELECT DISTINCT
						fup.fupfuncionalprogramatica, 
						vede.acaid, 
						vede.emecod, 
						vede.gndcod, 
						vede.mapcod, 
						vede.foncod, 
						per.pedid, 
						sum(per.pervalor) as pervalor, 
						pli.pliid, 
						pli.plicod, 
						pli.plititulo, 
						pt.ptres as pliptres, 
						exf.exfid,
						exf.exfcodfontesiafi,
						exf.exfnumsolempenho,
						exf.exfnumempenhooriginal,
						exf.semid,
						esf.esfcod, 
						esf.esfdsc,
						vede.mapcod,
						vede.gndcod,
						ptr.ptrnumprocessoempenho,
						mun.muncodsiafi,
						exf.exfvalor,
						ptr.ptrpropostasiconv,
						ptr.ptrpropostasiconvano,
						(CASE WHEN vede.mapcod = '90' THEN '0'
						  WHEN vede.mapcod = '30' THEN '1'
						  WHEN vede.mapcod = '40' THEN '2'
						  WHEN (vede.mapcod = '50' or vede.mapcod = '60') THEN '2'
						ELSE '' END) as esferaadm,
					
						 (CASE WHEN vede.gndcod = '3' THEN '33'||vede.mapcod||'41'
							   WHEN vede.gndcod = '4' THEN '44'||vede.mapcod||'42'
						  END) as naturezaDesp
					
					FROM emenda.execucaofinanceira exf 
					inner join monitora.pi_planointerno pli on pli.plicod = exf.plicod --and pli.pliano = '".date('Y')."'
					--left join monitora.pi_planointernoptres plpt on pli.pliid = plpt.pliid
					left join monitora.ptres pt on /*pt.ptrid = plpt.ptrid and*/ pt.ptres = exf.ptres
					inner join emenda.planotrabalho ptr on ptr.ptrid = exf.ptrid and exf.exfstatus = 'A'
					inner join emenda.ptemendadetalheentidade ped on ped.pedid = exf.pedid and ped.ptrid = exf.ptrid
					inner join emenda.ptiniesprecurso per on per.pedid = ped.pedid
					inner join emenda.v_emendadetalheentidade vede on vede.edeid = ped.edeid
					inner join emenda.v_funcionalprogramatica fup on fup.acaid = vede.acaid
					inner join public.esfera esf on fup.esfcod = esf.esfcod  
					inner join emenda.entidadebeneficiada ende on vede.entid = ende.enbid
					inner join territorios.municipio mun on ende.muncod = mun.muncod  
					where exf.ptrid = $ptrid 
					--AND vede.emeano = {$_SESSION['exercicio']}	
						$where
						
					GROUP BY 
						fup.fupfuncionalprogramatica, 
						mun.muncodsiafi,
						vede.emecod, 
						vede.gndcod,
						vede.mapcod, 
						vede.foncod, 
						vede.acaid, 
						ptr.ptrnumprocessoempenho,
						per.pedid, 
						pli.pliid, 
						pli.plicod, 
						pli.plititulo, 
						pt.ptres, 
						exf.exfid,
						exf.exfcodfontesiafi,
						exf.exfnumsolempenho,
						exf.exfnumempenhooriginal,
						exf.semid,
						exf.exfvalor,
						esf.esfcod, 
						esf.esfdsc,
						ptr.ptrpropostasiconv,
						ptr.ptrpropostasiconvano
					ORDER BY 
						per.pedid, 
						vede.emecod, 
						vede.gndcod, 
						vede.mapcod, 
						vede.foncod, 
						fup.fupfuncionalprogramatica, 
						vede.acaid";
			
			$arDados = $this->db->carregar($sql);
			$arDados = ($arDados) ? $arDados : array();
			
			$arRetorno = array();
			if(!count($arDados) && !$boCancelar){
				$mensagem = 'Não foi encontrado nenhum registro para empenhar.';
				$arRetorno['boSucesso'] = 0;
				//$arRetorno['mensagem'] = simec_htmlentities( $mensagem );
				$arRetorno['mensagem']   = utf8_encode($mensagem);
				echo simec_json_encode( $arRetorno );
				die;
			}
			return $arDados;
		}
	}
	
	public function cancelarConvenioRAP(&$post){
		$usuario 		 = $post['usuario'];
		$senha   		 = $post['senha'];
		$xmlRetorno = "";
		
		$array = explode( '|', $post['arpmcid'] );
		
		$arPmcid = array();
		foreach ($array as $key => $v) {
			$arCon = explode( '_', $v );
			$arPmcid[] = $arCon[0];
		}
		
		foreach ($array as $key => $v) {
			$arCon = explode( '_', $v );
				
		$sql = "SELECT
				  ent.enbcnpj AS cnpj,
				  ent.muncod,
				  ent.estuf,
				  ptrnumconvenio,
	  			  ptranoconvenio,
	  			  mun.muncodsiafi
				FROM
				  emenda.entidadebeneficiada ent INNER JOIN emenda.planotrabalho ptr
				  ON (ent.enbid = ptr.enbid)  
				  LEFT JOIN territorios.municipio mun
					INNER JOIN territorios.estado est ON (est.estuf = mun.estuf) 
				  ON (mun.muncod = ent.muncod) 
				  INNER JOIN emenda.responsavel res
				  ON (res.resid=ptr.resid)
				  INNER JOIN emenda.ptemendadetalheentidade ped
	  					on ped.ptrid = ptr.ptrid
				WHERE
				  ptr.ptrid = ".$arCon[1]."
			    GROUP BY
				  ent.enbcnpj,
				  ent.muncod,
				  ent.estuf,
				  ptrnumconvenio,
	  			  ptranoconvenio,
	  			  mun.muncodsiafi";
		
		$arPlanoTrabalho = $this->db->pegaLinha($sql);
		$arPlanoTrabalho = ($arPlanoTrabalho) ? $arPlanoTrabalho : array();
		
		$arRetorno = array();
		$arDados = self::selectParaEmpenho( $arCon[1], 'true', true );
		$boSucesso = $count = 0;
		if($arDados){
			$co_especie_empenho = '04';
			foreach($arDados as $dados){
				$exfid							= $dados['exfid'];
				$co_municipio_siafi 			= $dados['muncodsiafi'];
				$nu_cgc_favorecido 				= $arPlanoTrabalho['cnpj'];
				$co_esfera_adm 					= $dados['esferaadm'];
				$vl_empenho 					= $dados['exfvalor'];
				$nu_processo 					= $dados['ptrnumprocessoempenho'];
				$sg_uf 							= $arPlanoTrabalho['estuf'];
				$co_plano_interno_solic 		= $dados['plicod'];
				$co_esfera_orcamentaria_solic 	= substr($dados['esfcod'],0,1);
				$co_ptres_solic 				= $dados['pliptres'];
				$nu_proposta_siconv				= $dados['ptrpropostasiconv'].'/'.$dados['ptrpropostasiconvano'];
				
				$exfcodfontesiafi = str_pad($dados['exfcodfontesiafi'], 10, 0, STR_PAD_LEFT);
				$co_fonte_recurso_solic 		= $exfcodfontesiafi;
				
				$co_natureza_despesa_solic 		= $dados['naturezadesp'];
				
				$an_convenio 					= $arPlanoTrabalho['ptranoconvenio'];
				$nu_convenio 					= $arPlanoTrabalho['ptrnumconvenio'];
					

$arqXml = <<<XML
<?xml version='1.0' encoding='iso-8859-1'?>
<request>
<header>
<app>string</app>
<version>string</version>
<created>{$this->getDataAtual()}</created>
</header>
	<body>
	<params>
		<co_municipio_siafi>$co_municipio_siafi</co_municipio_siafi>
	    <nu_cgc_favorecido>$nu_cgc_favorecido</nu_cgc_favorecido>
	    <co_esfera_adm>$co_esfera_adm</co_esfera_adm>
	    <vl_empenho>$vl_empenho</vl_empenho>
	    <nu_processo>$nu_processo</nu_processo>
	    <sg_uf>$sg_uf</sg_uf>
	    <co_especie_empenho>$co_especie_empenho</co_especie_empenho>
	    <co_plano_interno_solic>$co_plano_interno_solic</co_plano_interno_solic>
	    <co_esfera_orcamentaria_solic>$co_esfera_orcamentaria_solic</co_esfera_orcamentaria_solic>
	    <co_ptres_solic>$co_ptres_solic</co_ptres_solic>
	    <co_fonte_recurso_solic>$co_fonte_recurso_solic</co_fonte_recurso_solic>
	    <co_natureza_despesa_solic>$co_natureza_despesa_solic</co_natureza_despesa_solic>
	    <an_convenio>$an_convenio</an_convenio>
	    <nu_convenio>$nu_convenio</nu_convenio>
	    <nu_proposta_siconv>$nu_proposta_siconv</nu_proposta_siconv>
	</params>
	</body>
</request>
XML;

			if ( $_SESSION['baselogin'] == "simec_desenvolvimento" || $_SESSION['baselogin'] == "simec_espelho_producao" ){
				$urlWS = "http://172.20.200.116/webservices/sigefemendas/integracao/web/dev.php/empenho/solicitar/";						
			} else {
				$urlWS = "http://www.fnde.gov.br/webservices/sigefemendas/index.php/empenho/solicitar/";
			}
				try {
					/*$xsd = APPRAIZ.'emenda/modulos/sistema/ws-sigef/xsd/xml.fnde.ws-sigef.empenho.solicitar.request.xsd';
					$arMessage = validaXML( $arqXml, $xsd );*/

				   // if( empty( $arMessage ) ){
				    	$xml = Fnde_Webservice_Client::CreateRequest()
							->setURL($urlWS)
							->setParams( array('xml' => $arqXml, 'login' => $usuario, 'senha' => $senha) )
							->execute()
						;
				    /*} else {
				    	echo 'Erro-WS: validação xml - Solicitar Emepenho: <br>';
				    	foreach ($arMessage as $msg) {
				    		print_r( $msg['text'] );
				    	}
				    	$sql = "UPDATE emenda.ptminutaconvenio SET 
									pmcjustcancelamento = null,
					  				pmcdatacancelamento = null,
					  				usucpfcancelamento = null
								WHERE 
					  				pmcid in ". "('".implode( "','", $arPmcid )."')";
					
						$this->db->executar( $sql );
						$this->db->commit();
						//echo '3';
				    	die;
				    }	*/				
					
					$xmlRetorno = $xml;
					
					$arrParam = array(
									'ptrid' => $ptrid,
									'exfid' => $exfid,
									'logTipo' => 'CR',
									'xmlEnvio' => $arqXml,
									'xmlRetorno' => $xmlRetorno,
									);
				
					logWsSIGEF($arrParam);
					
					$xml = simplexml_load_string( stripslashes($xml));
					
			        if ( (int) $xml->status->result ){				
			        	
						echo "1";
						
					} else {
						$erroMSG = str_replace(array(chr(13),chr(10)), ' ',utf8_decode($xml->status->error->message->text));
				    	$erroMSG = str_replace( "'", '"', $erroMSG );
				    	
				    	echo "<span style='color: red;'><b>Detalhes da Execução - Cancelar Convenio RAP:</b><br/><br/>".$erroMSG."</span>";
					}
		    		$count++;
				} catch (Exception $e){
					/**
					 * @var LogErroWS
					 * Bloco que grava o erro em nossa base
					 */
					$erroMSG = str_replace(array(chr(13),chr(10)), ' ',$e->getMessage());
					$erroMSG = str_replace( "'", '"', $erroMSG );
					
					$arrParam = array(
									'ptrid' => $ptrid,
									'exfid' => $exfid,
									'logTipo' => 'CR',
									'xmlEnvio' => $arqXml,
									'xmlRetorno' => $xmlRetorno.' - '.$erroMSG,
									);
				
					logWsSIGEF($arrParam);
					
					$sql = "UPDATE emenda.ptminutaconvenio SET 
								pmcjustcancelamento = null,
				  				pmcdatacancelamento = null,
				  				usucpfcancelamento = null
							WHERE 
				  				pmcid in ". "('".implode( "','", $arPmcid )."')";
					
					$this->db->executar( $sql );
					
					$obLogErroWS->commit();
					unset($obLogErroWS);
					
					# Erro 404 página not found
					if($e->getCode() == 404){
						$erro = "Erro-Serviço Cancelamento de Convênio encontra-se temporariamente indisponível.\nFavor tente mais tarde.";
						echo "<span style='color: red;'><b>Detalhes da Execução - Cancelar Convenio RAP:</b><br/><br/>".$erro."</span>";
						return false;
					}
					
					$erro = "Não foi possivel efetivar o cancelamento do convênio do plano de trabalho de nº: ".$arCon[1];
					echo "<span style='color: red;'><b>Detalhes da Execução - Cancelar Convenio RAP:</b><br/><br/>".$erro."</span>";
					return false;
				}
					
			} // Fim do foreach
		}
		}	
			
	} // Fim do método
	
	public function solicitarEmpenhoReforcoAnulacao(&$post, &$arPlanoTrabalho){
		$usuario 		 = $post['usuario'];
		$senha   		 = $post['senha'];
		$exfid   		 = $post['exfid'];
		$verificadoSiafi = $post['verificadoSiafi'];
		$verificadoCadin = $post['verificadoCadin'];
		$novoValor 		 = $post['novoValor'];
		$ptrid 			 = $post['ptrid'];
		$xmlRetorno 	 = "";
		
		$arRetorno = array();
		$where = " and exf.exfid = $exfid ";
		$arDados = self::selectParaEmpenho($ptrid, $where);
		if($arDados){
			$emetipo = $this->db->pegaUm("select ve.emetipo from
											emenda.execucaofinanceira ex
												inner join emenda.ptemendadetalheentidade pt on pt.pedid = ex.pedid
												inner join emenda.v_emendadetalheentidade ve on ve.edeid = pt.edeid
											where
												ex.exfid = $exfid");
			if( $emetipo == 'X' ){
				$co_centro_gestao_solic			= "61500000000";
			} else {
				$co_centro_gestao_solic			= "61700000000";
			}
				
			$count = 0;
			if($post['especie_empenho'] == 'reforco'){
				$co_especie_empenho = '02';
			} else {
				$co_especie_empenho = '03';			
			}
			foreach($arDados as $dados){
				# verifica se o empenho está efetivado ou não
				if( ($dados['exfnumempenhooriginal'] && $dados['semid'] == 4) || $_SESSION['exercicio'] == '2012' ){
					$exfid	= $dados['exfid'];
					
					$exfid							= $dados['exfid'];
				$co_municipio_siafi 			= $dados['muncodsiafi'];
				$nu_cgc_favorecido 				= $arPlanoTrabalho['cnpj'];
				$co_esfera_adm 					= $dados['esferaadm'];
				$vl_empenho 					= $dados['exfvalor'];
				$nu_processo 					= $dados['ptrnumprocessoempenho'];
				$sg_uf 							= $arPlanoTrabalho['estuf'];
				$co_plano_interno_solic 		= $dados['plicod'];
				$co_esfera_orcamentaria_solic 	= substr($dados['esfcod'],0,1);
				$co_ptres_solic 				= $dados['pliptres'];
				
				$exfcodfontesiafi 				= str_pad($dados['exfcodfontesiafi'], 10, 0, STR_PAD_LEFT);
				$co_fonte_recurso_solic 		= $exfcodfontesiafi;
				
				$co_natureza_despesa_solic 		= $dados['naturezadesp'];
				
				$an_convenio 					= $arPlanoTrabalho['ptranoconvenio'];
				$nu_convenio 					= $arPlanoTrabalho['ptrnumconvenio'];
				
				//$co_centro_gestao_solic			= "61700000000";
				$co_observacao					= "2";
				$co_tipo_empenho				= "5";
				$co_descricao_empenho			= "0010";
				$co_gestao_emitente				= "15253";
				$co_programa_fnde				= "03";
				$co_unidade_gestora_emitente	= "153173";
				$co_unidade_orcamentaria_solic	= "26298";
				$nu_proposta_siconv				= $dados['ptrpropostasiconv'].'/'.$dados['ptrpropostasiconvano'];
				$nu_sistema						= '02';
				$data_created 					= date("c");
					
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
			<nu_cnpj_favorecido>$nu_cgc_favorecido</nu_cnpj_favorecido>
			<nu_empenho_original>$nu_empenho_original</nu_empenho_original>
			<an_exercicio_original>$an_exercicio_original</an_exercicio_original>
			<vl_empenho>$vl_empenho</vl_empenho>
			<nu_processo>$nu_processo</nu_processo>
			<co_especie_empenho>$co_especie_empenho</co_especie_empenho>
			<co_plano_interno_solic>$co_plano_interno_solic</co_plano_interno_solic>
			<co_esfera_orcamentaria_solic>$co_esfera_orcamentaria_solic</co_esfera_orcamentaria_solic>
			<co_ptres_solic>$co_ptres_solic</co_ptres_solic>
			<co_fonte_recurso_solic>$co_fonte_recurso_solic</co_fonte_recurso_solic>
			<co_natureza_despesa_solic>$co_natureza_despesa_solic</co_natureza_despesa_solic>
			<co_centro_gestao_solic>$co_centro_gestao_solic</co_centro_gestao_solic>
			<an_convenio>$an_convenio</an_convenio>
			<nu_convenio>$nu_convenio</nu_convenio>
			<co_observacao>$co_observacao</co_observacao>
			<co_tipo_empenho>$co_tipo_empenho</co_tipo_empenho>
			<co_descricao_empenho>$co_descricao_empenho</co_descricao_empenho>
			<co_gestao_emitente>$co_gestao_emitente</co_gestao_emitente>
			<co_programa_fnde>$co_programa_fnde</co_programa_fnde>
			<co_unidade_gestora_emitente>$co_unidade_gestora_emitente</co_unidade_gestora_emitente>
			<co_unidade_orcamentaria_solic>$co_unidade_orcamentaria_solic</co_unidade_orcamentaria_solic>
			<nu_proposta_siconv>$nu_proposta_siconv</nu_proposta_siconv>
			<nu_sistema>$nu_sistema</nu_sistema>
		</params>
	</body>
</request>
XML;

			if($_SESSION['baselogin'] == "simec_desenvolvimento" || $_SESSION['baselogin'] == "simec_espelho_producao" ){
				$urlWS = 'http://172.20.200.116/webservices/sigef/integracao/public/index.php/orcamento/ne';
			} else {
				$urlWS = 'http://www.fnde.gov.br/webservices/sigef/index.php/orcamento/ne';
			}
				try {
					/*$xsd = APPRAIZ.'emenda/modulos/sistema/ws-sigef/xsd/xml.fnde.ws-sigef.empenho.solicitar.request.xsd';
					$arMessage = validaXML( $arqXml, $xsd );*/

				    //if( empty( $arMessage ) ){
				    	$xml = Fnde_Webservice_Client::CreateRequest()
								->setURL($urlWS)
								->setParams( array('xml' => $arqXml, 'method' => 'solicitar') )
								->execute();
						;
						$xmlRetorno = $xml;
						
						$arrParam = array(
										'ptrid' => $ptrid,
										'exfid' => $exfid,
										'logTipo' => 'E'.substr($co_especie_empenho,1,1),
										'xmlEnvio' => $arqXml,
										'xmlRetorno' => $xmlRetorno,
										);
						
						logWsSIGEF($arrParam);
						
						$xml = simplexml_load_string( stripslashes($xml));
						if ( (int) $xml->status->result ){
							
				        	$obEmpenho = $xml->body->row->children();
				        	$nu_seq_mov_ne = (int) $obEmpenho->nu_seq_ne;
				        	//$nu_seq_mov_ne = 74087;
							
							# Atualiza a tabela de execução financeira.
							$obExecucaoFinanceira = new ExecucaoFinanceira($exfid);
							$obExecucaoFinanceira->exfdataalteracao 	= date('Y-m-d H:i:s');
							$obExecucaoFinanceira->usucpf 				= $_SESSION['usucpf'];
							$obExecucaoFinanceira->exfverifsiafi 	 	= $verificadoSiafi;
							$obExecucaoFinanceira->exfverifcadin 	    = $verificadoCadin;
							$obExecucaoFinanceira->exfcodmunicipiosiafi = $co_municipio_siafi;
							$obExecucaoFinanceira->salvar();
							
							# Grava o Historico
							$obExecFinanceiraHistorico = new ExecFinanceiraHistorico();
							$obExecFinanceiraHistorico->usucpf 				 = $_SESSION['usucpf'];
							$obExecFinanceiraHistorico->exfid 				 = $exfid;
							$obExecFinanceiraHistorico->semid 				 = 1;
							$obExecFinanceiraHistorico->exfvalor 			 = $vl_empenho; // Diferença do valor original
							$obExecFinanceiraHistorico->exfespecieempenho	 = $co_especie_empenho;
							$obExecFinanceiraHistorico->efhiddataalteracao 	 = date('Y-m-d H:i:s');
							$obExecFinanceiraHistorico->exfverifsiafi 	 	 = $verificadoSiafi;
							$obExecFinanceiraHistorico->exfverifcadin 	     = $verificadoCadin;
							$obExecFinanceiraHistorico->salvar();
							
							# Cria Filho na tabela de ExecucaoFinanceira
							$obExecucaoFinanceiraNovo = new ExecucaoFinanceira();
							$obExecucaoFinanceiraNovo->tpeid 				= $obExecucaoFinanceira->tpeid;
							$obExecucaoFinanceiraNovo->pliid 				= $obExecucaoFinanceira->pliid;
							$obExecucaoFinanceiraNovo->ptrid 				= $obExecucaoFinanceira->ptrid;
							$obExecucaoFinanceiraNovo->pedid 				= $obExecucaoFinanceira->pedid;
							$obExecucaoFinanceiraNovo->usucpf 				= $_SESSION['usucpf'];
							$obExecucaoFinanceiraNovo->exfnumsolempenho 	= $nu_seq_mov_ne;
							$obExecucaoFinanceiraNovo->semid 				= 1;
							$obExecucaoFinanceiraNovo->exfdatainclusao 	    = date('Y-m-d H:i:s');
							$obExecucaoFinanceiraNovo->exfdataalteracao 	= date('Y-m-d H:i:s');
							$obExecucaoFinanceiraNovo->exfespecieempenho 	= $co_especie_empenho;
							$obExecucaoFinanceiraNovo->exfvalor  			= $vl_empenho; // Diferença do valor original
							$obExecucaoFinanceiraNovo->exfstatus 			= 'A';
							$obExecucaoFinanceiraNovo->exfidpai 			= $exfid;
							$obExecucaoFinanceiraNovo->exfverifsiafi 	 	= $verificadoSiafi;
							$obExecucaoFinanceiraNovo->exfverifcadin 	    = $verificadoCadin;
							$obExecucaoFinanceiraNovo->exfcodmunicipiosiafi = $co_municipio_siafi;
							$obExecucaoFinanceiraNovo->exfanooriginal 	    = date('Y');
							$obExecucaoFinanceiraNovo->salvar();
							$obExecucaoFinanceiraNovo->commit();
							
							$erro = 'Solicitar Empenho no SIGEF: Realizada com sucesso.';
							echo "<span style='color: blue;'><b>Solicitar Empenho Reforco Anulacao:</b><br/><br/>".$erro."</span>";
							return true;
						} else {
							$erroMSG = str_replace(array(chr(13),chr(10)), ' ',utf8_decode($xml->status->error->message->text));
					    	$erroMSG = str_replace( "'", '"', $erroMSG );
					    							
							$erro = "Erro-WS Solicitar Empenho no SIGEF: $erroMSG";
							echo "<span style='color: red;'><b>Detalhes da Execução - Solicitar Empenho Reforco Anulacao:</b><br/><br/>".$erro."</span>";
							return false;
						}
					} catch (Exception $e){
						/**
						 * @var LogErroWS
						 * Bloco que grava o erro em nossa base
						 */
						$erroMSG = str_replace(array(chr(13),chr(10)), ' ',$e->getMessage());
						$erroMSG = str_replace( "'", '"', $erroMSG );
							
						$arrParam = array(
										'ptrid' => $ptrid,
										'exfid' => $exfid,
										'logTipo' => 'E'.substr($co_especie_empenho,1,1),
										'xmlEnvio' => $arqXml,
										'xmlRetorno' => $xmlRetorno.' - '.$erroMSG,
										);
						
						logWsSIGEF($arrParam);
						
						# Erro 404 página not found
						if($e->getCode() == 404){
							$erro = "Erro-Serviço Empenho encontra-se temporariamente indisponível.\nFavor tente mais tarde.";
							echo "<span style='color: red;'><b>Detalhes da Execução - Solicitar Empenho Reforco Anulacao:</b><br/><br/>".$erro."</span>";
							return false;
						} else {							
							$erro = "Erro-WS Solicitar Empenho no SIGEF: $erroMSG";							
							echo "<span style='color: red;'><b>Detalhes da Execução - Solicitar Empenho Reforco Anulacao:</b><br/><br/>".$erro."</span>";
						}
					}
				} else { // fim do if empenho original
					$erro = "Erro-Empenho aguardando efetivação.";
					echo "<span style='color: red;'><b>Detalhes da Execução - Solicitar Empenho Reforco Anulacao:</b><br/><br/>".$erro."</span>";
					return false;
				} 
				$count++;		
			} // Fim do foreach
			
			/*if($obExecucaoFinanceira->commit()){
				echo "Empenho solicitado com sucesso";
				//die;
			} else {
				echo "Solicitação de Empenho não efetuado com sucesso";
			}*/
			
			unset($obExecucaoFinanceira);
			unset($obExecFinanceiraHistorico);
			unset($obExecucaoFinanceiraNovo);
			die;
		} // Fim do if Array de Dados
	} // Fim do método
		
	# $boConsultaPorPtrid flag para dizer que será consultado somente um empenho. Só é passado como true, na lista de Empenhos na ExcucaoPTA.inc
	public function consultarEmpenho(&$post, $boConsultaPorPtrid = false, $boAuto = false){
		$where = "";
		
		if($boConsultaPorPtrid && $post['solicitarconta'] == 'S'){
			$where = " and ef.exfid = {$post['exfid']} and ef.ptrid = {$post['ptrid']} ";
			$obContaCorrente = new WSContaCorrente($this->db);
			$obContaCorrente->consultarContaCorrente($post);
		}
		$usuario = $post['usuario'];
		$senha 	 = $post['senha'];
		$exfid 	 = $post['exfid'];
		$ptrid 	 = $post['ptrid'];
		$xmlRetorno = "";
		
		$arErro    = array();
		$arRetorno = array();
		
		$arSituacaoEmpenho = array();
		$arSituacaoEmpenho[1] = 8; 
		$arSituacaoEmpenho[2] = 0; 
		$arSituacaoEmpenho[3] = 1; 
		$arSituacaoEmpenho[4] = 2; 
		$arSituacaoEmpenho[5] = 3; 
		$arSituacaoEmpenho[6] = 6; 
		$arSituacaoEmpenho[7] = 4;
		
		$arExecFinanceiraAlterados = array();
		
		$sql = "select ef.exfid, ef.exfnumsolempenho, ef.semid, se.semcod, ef.ptrid from emenda.execucaofinanceira  ef
					inner join emenda.situacaoempenho se on ef.semid = se.semid
					inner join emenda.planotrabalho  p on p.ptrid = ef.ptrid and p.ptrstatus = 'A' 
				where ef.exfnumsolempenho is not null and ef.exfnumsolempenho <> '' 
					and ef.exfid not in(select exfidpai from emenda.execucaofinanceira where exfidpai = ef.exfid and exfstatus = 'A')
					and ef.exfstatus = 'A' 
					$where";
		
		$arDados = $this->db->carregar($sql);
		$arDados = ($arDados) ? $arDados : array();
		$count = 0;
		
		foreach($arDados as $dados){
			$exfid 			   = $dados['exfid'];
			$exfnumsolempenho  = $dados['exfnumsolempenho'];
			$post['ptrid'] 		= ($dados['ptrid'] ? $dados['ptrid'] : $post['ptrid']);
			$ptrid				= $post['ptrid'];
			
			/*if(!$post['ptrid']){
				$post['ptrid'] = $dados['ptrid'];
			}*/
			
			try {

    $arqXml = <<<XML
<?xml version='1.0' encoding='iso-8859-1'?>
<request>
	<header>
		<app>string</app>
		<version>string</version>
		<created>{$this->getDataAtual()}</created>
	</header>
	<body>
		<auth>
			<usuario>$usuario</usuario>
			<senha>$senha</senha>
		</auth>
		<params>
        <nu_seq_ne>$exfnumsolempenho</nu_seq_ne>
		</params>
	</body>
</request>
XML;
			/*if ( $_SESSION['baselogin'] == "simec_desenvolvimento" || $_SESSION['baselogin'] == "simec_espelho_producao" ){
				$urlWS = "http://172.20.200.116/webservices/sigefemendas/integracao/web/dev.php/empenho/consultar/";						
			} else {*/
				$urlWS = "http://www.fnde.gov.br/webservices/sigef/index.php/orcamento/ne";
		//	}
			
		    	$xml = Fnde_Webservice_Client::CreateRequest()
					->setURL($urlWS)
					->setParams( array('xml' => $arqXml, 'method' => 'consultar' ) )
					->execute()
				;
				
				$xmlRetorno = $xml;		
				
				$arrParam = array(
								'ptrid' => $ptrid,
								'exfid' => $exfid,
								'logTipo' => 'EC',
								'xmlEnvio' => $arqXml,
								'xmlRetorno' => $xmlRetorno,
								);
				
				logWsSIGEF($arrParam);
				
				$xml = simplexml_load_string( stripslashes($xml));
				
			    if ( (int) $xml->status->result ){
			    	
			        $obEmpenho = $xml->body->row->children();
					
			    	$situacao_documento = (string) $obEmpenho->situacao_documento;
				    $situacao_documento = trim(substr($situacao_documento,0,strpos($situacao_documento, '-')));
				    
					$nu_empenho = (string) $obEmpenho->numero_documento;
					$obExecucaoFinanceira = new ExecucaoFinanceira($exfid);

					# Verifica se a situação do documento foi alterada
					//if($arSituacaoEmpenho[$dados['semid']] != $situacao_documento){
						# Atualiza a tabela de execução financeira.
						$boSucesso = 0;
						$obExecucaoFinanceira = new ExecucaoFinanceira($exfid);
						$obExecucaoFinanceira->exfdataalteracao = date('Y-m-d H:i:s'); 
						$obExecucaoFinanceira->semid = current(array_keys($arSituacaoEmpenho, $situacao_documento));
						$obExecucaoFinanceira->exfnumempenhooriginal = $nu_empenho;
						$obExecucaoFinanceira->usucpf = $_SESSION['usucpf'];
						
						$obExecFinanceiraHistorico = new ExecFinanceiraHistorico();
						$obExecFinanceiraHistorico->usucpf 				 = $_SESSION['usucpf'];
						$obExecFinanceiraHistorico->exfid 				 = $exfid;
						$obExecFinanceiraHistorico->semid 				 = $obExecucaoFinanceira->semid;
						$obExecFinanceiraHistorico->efhiddataalteracao 	 = date('Y-m-d H:i:s');
						$obExecFinanceiraHistorico->salvar();
						
						/*if( $obExecucaoFinanceira->semid == 4 ){
							if( !$boConsultaPorPtrid ){
								# se o ws retornou a situação do empenho com EMPENHO EFETIVADO, altera o estado do workflow para EM_GERACAO_DA_MINUTA_DE_CONVENIO
								//$this->alteraEstado_wf( $post['ptrid'] );
							}
						}*/
						
						# Verifica se foi efetivado e é filho
						if($obExecucaoFinanceira->semid == 4 && $obExecucaoFinanceira->exfidpai){
							# se foi efetivado, inativamos o filho
							$obExecucaoFinanceira->exfstatus = 'I';
							# alteramos o valor do PAI
							$obExecucaoFinanceiraPai = new ExecucaoFinanceira($obExecucaoFinanceira->exfidpai);
							
							# ANULAÇÂO
							if($obExecucaoFinanceira->exfespecieempenho == 03){
								if($obExecucaoFinanceira->exfvalor == $obExecucaoFinanceiraPai->exfvalor){
									$obExecucaoFinanceiraPai->exfvalor = '0.00';
								} else {
									$obExecucaoFinanceiraPai->exfvalor = $obExecucaoFinanceiraPai->exfvalor - $obExecucaoFinanceira->exfvalor;
								}
							}
							
							# REFORÇO
							if($obExecucaoFinanceira->exfespecieempenho == 02){
								$obExecucaoFinanceiraPai->exfvalor = $obExecucaoFinanceiraPai->exfvalor + $obExecucaoFinanceira->exfvalor;
							}
							$obExecucaoFinanceiraPai->exfvalor = simec_number_format($obExecucaoFinanceiraPai->exfvalor,2, '.','');
							$obExecucaoFinanceiraPai->salvar();
						}
						
						$obExecucaoFinanceira->salvar();
						
						$boSucesso = $this->db->commit();
						unset($obExecucaoFinanceira);
						unset($obExecFinanceiraHistorico);
						if($boSucesso){
							$arExecFinanceiraAlterados[] = $exfid;						
						}
						$erro = 'Consultar Empenho no SIGEF: Realizada com Sucesso';
						echo "<span style='color: blue;'><b>Detalhes da Execução - Consulta Empenho:</b><br/><br/>".$erro."</span>";
				//	}//fim do if
			    } else {
			    	$erroMSG = str_replace(array(chr(13),chr(10)), ' ',utf8_decode($xml->status->error->message->text));
			    	$erroMSG = str_replace( "'", '"', $erroMSG );
			    	
					//$erro = "Erro-WS Consultar Empenho no SIGEF: $erroMSG";
					//echo "<span style='color: red;'><b>Detalhes da Execução - Consulta Empenho:</b><br/><br/>".$erroMSG."</span>";
					throw new Exception(utf8_decode($xml->status->error->message->text));
			    }
			} catch (Exception $e){
				/**
				 * @var LogErroWS
				 * Bloco que grava o erro em nossa base
				 */
				$erroMSG = str_replace(array(chr(13),chr(10)), ' ',$e->getMessage());
				$erroMSG = str_replace( "'", '"', $erroMSG );
				
				$arrParam = array(
								'ptrid' => $ptrid,
								'exfid' => $exfid,
								'logTipo' => 'EC',
								'xmlEnvio' => $arqXml,
								'xmlRetorno' => $xmlRetorno.' - '.$erroMSG,
								);
				
				logWsSIGEF($arrParam);
				
				# Erro 404 página not found
				if($e->getCode() == 404){
					$erro = "Erro-Serviço Consulta Empenho encontra-se temporariamente indisponível.\nFavor tente mais tarde.";
					echo "<span style='color: red;'><b>Detalhes da Execução - Consulta Empenho SIGEF:</b><br/><br/>".$erro."</span>";
					return false;
				}
				
				echo "<span style='color: red;'><b>Detalhes da Execução - Consulta Empenho SIGEF:</b><br/><br/>".$erroMSG."</span>";
				return false;
			}
			$count++;
		} // fim foreach
		
		$sql = "";
		$arDados = "";
		
		$arExecFinanceiraAlterados[] = 1;
		
		if(count($arExecFinanceiraAlterados) && !$boConsultaPorPtrid){
			$exfid    = implode(',',$arExecFinanceiraAlterados);
			$sql = "select 
						pt.ptrid || '/' || pt.ptrexercicio as planotrabalho,  
						exfvalor as valor,
						ef.exfnumsolempenho,
						ef.exfespecieempenho,
						se.semcod || ' - ' ||se.semdesc as situacao
					from emenda.planotrabalho pt
						inner join emenda.execucaofinanceira  ef on pt.ptrid = ef.ptrid 
						inner join emenda.situacaoempenho se on ef.semid = se.semid 
					where exfnumsolempenho is not null and exfnumsolempenho <> '' and exfid in ($exfid)";
			$arDados = $this->db->carregar($sql);
			
		} elseif(count($arExecFinanceiraAlterados) && $boConsultaPorPtrid){
			/*echo 'consultaEmpenho';			
			die; */
		} elseif($boConsultaPorPtrid) {
			$erro = 'Erro-Operação do Consulta Empenho falhou.';
			echo "<span style='color: red;'><b>Detalhes da Execução - Consulta Empenho:</b><br/><br/>".$erro."</span>";
			return false;
		}
		
		if(!$boConsultaPorPtrid){
			$arDados = ($arDados) ? $arDados : array();
			$this->db->cria_aba( $abacod_tela, $url, '' );
			
			if($boAuto){
				ob_start();
			}
					
			monta_titulo( $titulo_modulo, 'Empenhos Atualizados' );
			
			$cabecalho = array("Plano de Trabalho", "Valor", "Empenho", "Tipo Empenho", "Situação");
			$this->db->monta_lista_array($arDados, $cabecalho, 50, 20, '', 'center','');
			
			if($boAuto){
				$conteudo = ob_get_contents();
				ob_end_clean();
				
				/*
				 * ENVIANDO EMAIL CONFIRMANDO O PROCESSAMENTO
				 */
				require_once APPRAIZ . 'includes/phpmailer/class.phpmailer.php';
				require_once APPRAIZ . 'includes/phpmailer/class.smtp.php';
				$mensagem = new PHPMailer();
				$mensagem->Host         = "localhost";
				$mensagem->Mailer       = "smtp";
				$mensagem->FromName		= "Empenhos Atualizadas";
				$mensagem->From 		= $_SESSION['email_sistema'];
				$mensagem->AddAddress( $_SESSION['email_sistema'], SIGLA_SISTEMA );
				$mensagem->Subject = "Empenhos Atualizadas";
				
				$mensagem->Body = $conteudo;
				$mensagem->IsHTML( true );
				$mensagem->Send();
			}
			
			
		}
		
		# Feito isso Para não parar com a rotina automatica
		if(!$boAuto){
			die;
		}
	}
	public function consultarEmpenhoGeral(&$post){
		$where = "";
			
		$usuario = $post['usuario'];
		$senha 	 = $post['senha'];
		$xmlRetorno = "";
		
		$arErro    = array();
		$arRetorno = array();
		
		$arSituacaoEmpenho = array();
		$arSituacaoEmpenho[1] = 8; 
		$arSituacaoEmpenho[2] = 0; 
		$arSituacaoEmpenho[3] = 1; 
		$arSituacaoEmpenho[4] = 2; 
		$arSituacaoEmpenho[5] = 3; 
		$arSituacaoEmpenho[6] = 6; 
		$arSituacaoEmpenho[7] = 4;
		
		$arExecFinanceiraAlterados = array();	
		
		$sql = "SELECT
					pt.ptrcod || '/' || pt.ptrexercicio as planotrabalho,  
					exfvalor as valor, 
					ef.exfid, ef.exfnumsolempenho, ef.semid, se.semcod, ef.ptrid ,
					ef.exfespecieempenho,
					se.semcod || ' - ' ||se.semdesc as situacao,
					cc.cocnu_agencia,
					cc.cocnu_conta_corrente,
					cc.cocds_razao_social,
					cc.cocsituacao_conta,
					cc.cocid,
					ef.exfdataalteracao
				FROM emenda.execucaofinanceira  ef
					inner join emenda.situacaoempenho se on ef.semid = se.semid
					inner join emenda.planotrabalho pt on pt.ptrid = ef.ptrid
					left join emenda.contacorrente cc on cc.ptrid = pt.ptrid
				WHERE 
					ef.exfnumsolempenho is not null and ef.exfnumsolempenho <> ''
					and pt.ptrexercicio = '".$_SESSION['exercicio']."'
					and ef.semid is not null
					and ef.exfid not in(select exfidpai from emenda.execucaofinanceira where exfidpai = ef.exfid and exfstatus = 'A')";
		
		$arDados = $this->db->carregar($sql);
		$arDados = ($arDados) ? $arDados : array();
		if(!$arDados) {
			$htmllog .= "<p align=center><a href=emenda.php?modulo=principal/consultarEmpenho&acao=A>Clique aqui</a> para solicitar novamente.</p>";
			$htmllog .= "<table class=listagem width=95% align=center bgcolor=#f5f5f5 cellSpacing=1 cellPadding=3>";
			$htmllog .= "<tr><td class=SubTituloCentro>Não existem pagamentos no ano corrente</td></tr>";
			$htmllog .= "</table>";
			die($htmllog);
		}
		$count = 0;
		monta_titulo( $titulo_modulo, 'Empenhos Atualizados' );
		$htmllog .= "<p align=center><a href=emenda.php?modulo=principal/consultarEmpenho&acao=A>Clique aqui</a> para solicitar novamente.</p>";
		$htmllog .= "<table class=listagem width=95% align=center bgcolor=#f5f5f5 cellSpacing=1 cellPadding=3>";
		$htmllog .= "<tr>";
		$htmllog .= "<td class=SubTituloCentro>PTA</td>";
		$htmllog .= "<td class=SubTituloCentro>Situação</td>";
		$htmllog .= "<td class=SubTituloCentro>Data de Atualização</td>";
		$htmllog .= "<td class=SubTituloCentro>&nbsp;</td>";
		$htmllog .= "</tr>";
		
		$obContaCorrente = new WSContaCorrente($this->db);
		
		foreach($arDados as $dados){
			$exfid 			   = $dados['exfid'];
			$exfnumsolempenho  = $dados['exfnumsolempenho'];
			$ptrid 			   = $dados['ptrid'];
			$pta 			   = $dados['planotrabalho'];
			$valor 			   = $dados['valor'];
			$tipoempenho 	   = $dados['exfespecieempenho'];
			$situacao 		   = $dados['situacao'];
			$agencia 		   = $dados['cocnu_agencia'];
			$conta_corrente    = $dados['cocnu_conta_corrente'];
			$razao_social 	   = $dados['cocds_razao_social'];
			$situacao_conta    = $dados['cocsituacao_conta'];
			$cocid			   = $dados['cocid'];
			
			if( !empty($cocid) ) $erroCC = $obContaCorrente->consultarContaCorrenteGeral($post, $cocid);
			
			try {
        $arqXml = <<<XML
<?xml version='1.0' encoding='iso-8859-1'?>
<request>
	<header>
		<app>string</app>
		<version>string</version>
		<created>{$this->getDataAtual()}</created>
	</header>
	<body>
		<auth>
			<usuario>$usuario</usuario>
			<senha>$senha</senha>
		</auth>
		<params>
        <nu_seq_ne>$exfnumsolempenho</nu_seq_ne>
		</params>
	</body>
</request>
XML;
			/*if ( $_SESSION['baselogin'] == "simec_desenvolvimento" || $_SESSION['baselogin'] == "simec_espelho_producao" ){
				$urlWS = "http://172.20.200.116/webservices/sigefemendas/integracao/web/dev.php/empenho/consultar/";						
			} else {*/
				$urlWS = "http://www.fnde.gov.br/webservices/sigef/index.php/orcamento/ne";
		//	}
			
		    	$xml = Fnde_Webservice_Client::CreateRequest()
					->setURL($urlWS)
					->setParams( array('xml' => $arqXml, 'method' => 'consultar' ) )
					->execute()
				;
				
				$xmlRetorno = $xml;	
				
				$arrParam = array(
								'ptrid' => $ptrid,
								'exfid' => $exfid,
								'logTipo' => 'EC',
								'xmlEnvio' => $arqXml,
								'xmlRetorno' => $xmlRetorno,
								);
				
				logWsSIGEF($arrParam);
				
				$xml = simplexml_load_string( stripslashes($xml));
			    if ( (int) $xml->status->result ){
			    	
			    	$htmllog .= "<tr>";
					$htmllog .= "<td><center>".$pta."</center></td>";
					$htmllog .= "<td>".$situacao."</td>";
					$htmllog .= "<td>".date('d/m/Y H:i:s')."</td>";
					$htmllog .= "<td colspan='4'><b>".$erroCC."</b></td>";
					$htmllog .= "</tr>";
			    	
					$count++;
					
			        $obEmpenho = $xml->body->row->children();

			    	$situacao_documento = (string) $obEmpenho->situacao_documento;
				    $situacao_documento = trim(substr($situacao_documento,0,strpos($situacao_documento, '-')));
				    
					$nu_empenho = (string) $obEmpenho->nu_empenho;
 
					$obExecucaoFinanceira = new ExecucaoFinanceira($exfid);

					# Verifica se a situação do documento foi alterada
					//if($arSituacaoEmpenho[$dados['semid']] != $situacao_documento){
						# Atualiza a tabela de execução financeira.
						$boSucesso = 0;
						$obExecucaoFinanceira = new ExecucaoFinanceira($exfid);
						$obExecucaoFinanceira->exfdataalteracao = date('Y-m-d H:i:s'); 
						$obExecucaoFinanceira->semid = current(array_keys($arSituacaoEmpenho, $situacao_documento));
						$obExecucaoFinanceira->exfnumempenhooriginal = $nu_empenho;
						$obExecucaoFinanceira->usucpf = $_SESSION['usucpf'];
						
						$obExecFinanceiraHistorico = new ExecFinanceiraHistorico();
						$obExecFinanceiraHistorico->usucpf 				 = $_SESSION['usucpf'];
						$obExecFinanceiraHistorico->exfid 				 = $exfid;
						$obExecFinanceiraHistorico->semid 				 = $obExecucaoFinanceira->semid;
						$obExecFinanceiraHistorico->efhiddataalteracao 	 = date('Y-m-d H:i:s');
						$obExecFinanceiraHistorico->salvar();
												
						# Verifica se foi efetivado e é filho
						if($obExecucaoFinanceira->semid == 4 && $obExecucaoFinanceira->exfidpai){
							# se foi efetivado, inativamos o filho
							$obExecucaoFinanceira->exfstatus = 'I';
							# alteramos o valor do PAI
							$obExecucaoFinanceiraPai = new ExecucaoFinanceira($obExecucaoFinanceira->exfidpai);
							
							
							
							# ANULAÇÂO
							if($obExecucaoFinanceira->exfespecieempenho == 03){
								if($obExecucaoFinanceira->exfvalor == $obExecucaoFinanceiraPai->exfvalor){
									$obExecucaoFinanceiraPai->exfvalor = '0.00';
								} else {
									$obExecucaoFinanceiraPai->exfvalor = $obExecucaoFinanceiraPai->exfvalor - $obExecucaoFinanceira->exfvalor;
								}
							}
							
							# REFORÇO
							if($obExecucaoFinanceira->exfespecieempenho == 02){
								$obExecucaoFinanceiraPai->exfvalor = $obExecucaoFinanceiraPai->exfvalor + $obExecucaoFinanceira->exfvalor;
							}
							$obExecucaoFinanceiraPai->salvar();
						}
						
						$obExecucaoFinanceira->salvar();
						
						$boSucesso = $this->db->commit();
						unset($obExecucaoFinanceira);
						unset($obExecFinanceiraHistorico);
						if($boSucesso){
							$arExecFinanceiraAlterados[] = $exfid;						
						}
				//	}//fim do if
			    } else {
			    	$htmllog .= "<tr>";
					$htmllog .= "<td colspan='8'>".utf8_decode($xml->status->error->message->text)."</td>";
					$htmllog .= "</tr>";
			    }
			} catch (Exception $e){
				$erroMSG = str_replace(array(chr(13),chr(10)), ' ',$e->getMessage());
				$erroMSG = str_replace( "'", '"', $erroMSG );
				
				$arrParam = array(
								'ptrid' => $ptrid,
								'exfid' => $exfid,
								'logTipo' => 'EC',
								'xmlEnvio' => $arqXml,
								'xmlRetorno' => $xmlRetorno.' - '.$erroMSG,
								);
				
				logWsSIGEF($arrParam);
				
				$htmllog .= "<tr>";
				$htmllog .= "<td colspan='8' class=SubTituloCentro>Erro no webservice com o FNDE!</td>";
				$htmllog .= "<td>".$erroMSG."</td>";
				$htmllog .= "</tr>";
			}
		} // fim foreach
		$htmllog1 = '<table class="listagem" width="95%" cellspacing="0" cellpadding="2" border="0" align="center">';
		$htmllog1 .= "<tbody>";
		$htmllog1 .= '<tr bgcolor="#ffffff">';
		$htmllog1 .= "<td><b>Total de Registros: ".$count++."</b></td>";
		$htmllog1 .= "</tr></tbody></table>";
		
		die($htmllog."</table>".$htmllog1);
	}
	
	private function alteraEstado_wf( $ptrid ){
	
		$sql = "select docid from emenda.planotrabalho where ptrid = ".$ptrid." and ptrstatus = 'A'";
		$docid = $this->db->pegaUm( $sql );
		
		$sql = "SELECT esdid FROM workflow.documento WHERE docid = $docid";								
		$esdid = $this->db->pegaUm( $sql );
		
		$sql = "select aedid from workflow.acaoestadodoc where esdidorigem = $esdid and esdiddestino = 58";
		$aedid = $this->db->pegaUm( $sql );
			
		if( $aedid && $docid ){
		/*echo "<script>
				wf_alterarEstado( $aedid, $docid, 58, 'Empenhado (Em Elaboração da Minuta do Convênio)')
			 </script>";*/
			$arDados = array( "url" => $_SESSION['favurl'],
				  			  "ptrid" => $post['ptrid'] );
			include_once APPRAIZ . 'includes/workflow.php';
			wf_alterarEstado( $docid, $aedid, '', $arDados );
		}
	}
		
	public function podeAlterar($exfid){
		$sql = "select exfid, exfnumempenhooriginal, semid from emenda.execucaofinanceira where exfid = $exfid";
		$arDados = $this->db->pegaLinha($sql);
		$arDados = ($arDados) ? $arDados : array();
		if($arDados['semid'] == 4 && $arDados['exfnumempenhooriginal'] != ''){
			$sql = "select count(1) as count from emenda.execucaofinanceira where exfidpai = $exfid and exfstatus = 'A'";
			if($boFilhoAtivo = $this->db->pegaUm($sql)){
				return false;
			}
			return true;
		}
		return false;
	}
	
	public function consultarConvenioWSSape( $numProcesso ){
		
		$arqXml = <<<XML
<?xml version='1.0' encoding='iso-8859-1'?>
<request>
<header>
<app>string</app>
<version>string</version>
<created>{$this->getDataAtual()}</created>
</header>
	<body>
	<params>
		<processo>$numProcesso</processo>
	</params>
	</body>
</request>
XML;
		
	try {
		$xml = Fnde_Webservice_Client::CreateRequest()
				->setURL('http://www.fnde.gov.br/webservices/wssape/index.php')
				->setParams( array('xml' => $arqXml, 'login' => $usuario, 'senha' => $senha) )
				->execute();					
		
			$xmlRetorno = $xml;
			$xml = simplexml_load_string( stripslashes($xml));						
	        if ( (int) $xml->status->result ){						
	        	$obConvenio = $xml->body->children();
	        	//ver( $obConvenio );							
			} else {
				$erroMSG = str_replace(array(chr(13),chr(10)), ' ',utf8_decode($xml->status->error->message->text));
		    	$erroMSG = str_replace( "'", '"', $erroMSG );
		    							
				echo "<script>
						alert('Erro-WS Socilitar Convênio no SIGEF: $erroMSG');
						window.location.href = 'emenda.php?modulo=principal/execucaoPTA&acao=A';
					 </script>";
				
			 	throw new Exception(utf8_decode($xml->status->error->message->text));
			}
		} catch (Exception $e){					
			# Erro 404 página not found
			if($e->getCode() == 404){
				echo "<script>
						alert('Erro-Serviço Convenio encontra-se temporariamente indisponível.\nFavor tente mais tarde.');
						window.location.href = 'emenda.php?modulo=principal/execucaoPTA&acao=A';
					 </script>";
				die;
			}
			$erroMSG = str_replace(array(chr(13),chr(10)), ' ',$e->getMessage());
			$erroMSG = str_replace( "'", '"', $erroMSG );
						
			echo "<script>
					alert('Erro-WS Solicitar Convenio no SIGEF: $erroMSG');
					window.location.href = 'emenda.php?modulo=principal/execucaoPTA&acao=A';
				 </script>";
			die;
		}
	}
	
	public function solicitarEmpenhoOriginal(&$post, &$arPlanoTrabalho){
		$usuario 		 = $post['usuario'];
		$senha   		 = $post['senha'];
		$verificadoSiafi = $post['verificadoSiafi'];
		$verificadoCadin = $post['verificadoCadin'];
		$xmlRetorno = "";
		
		$exfid   		 = $post['exfid'];
		$ptrid   		 = $post['ptrid'];
		
		$emetipo = $this->db->pegaUm("select ve.emetipo from 
											emenda.execucaofinanceira ex
										    inner join emenda.ptemendadetalheentidade pt on pt.pedid = ex.pedid
										    inner join emenda.v_emendadetalheentidade ve on ve.edeid = pt.edeid
										where
											ex.exfid = $exfid");
		
		$arRetorno = array();		
		$arDados = self::selectParaEmpenho($post['ptrid'], '', '', $exfid);
		
		$boSucesso = $count = 0;
		if($arDados){
			$co_especie_empenho = '01';
			//$uniid = $this->db->pegaUm("select COALESCE(count(anaid), '0') from emenda.analise where uniid = 11 and ptrid = {$post['ptrid']}");
			
			if( $emetipo == 'X' ){
				$co_centro_gestao_solic			= "61500000000";
			} else {
				$co_centro_gestao_solic			= "61700000000";
			}
			
			foreach($arDados as $dados){
				$exfid							= $dados['exfid'];
				$co_municipio_siafi 			= $dados['muncodsiafi'];
				$nu_cgc_favorecido 				= $arPlanoTrabalho['cnpj'];
				$co_esfera_adm 					= $dados['esferaadm'];
				$vl_empenho 					= $dados['exfvalor'];
				$nu_processo 					= $dados['ptrnumprocessoempenho'];
				$sg_uf 							= $arPlanoTrabalho['estuf'];
				$co_plano_interno_solic 		= $dados['plicod'];
				$co_esfera_orcamentaria_solic 	= substr($dados['esfcod'],0,1);
				$co_ptres_solic 				= $dados['pliptres'];				
				$exfcodfontesiafi 				= str_pad($dados['exfcodfontesiafi'], 10, 0, STR_PAD_LEFT);
				$co_fonte_recurso_solic 		= $exfcodfontesiafi;				
				$co_natureza_despesa_solic 		= str_pad($dados['naturezadesp'], 8, 0, STR_PAD_RIGHT);				
				$an_convenio 					= $arPlanoTrabalho['ptranoconvenio'];
				$nu_convenio 					= $arPlanoTrabalho['ptrnumconvenio'];
				/*if( $uniid <> 0 ) $co_centro_gestao_solic	= "61500000000";
				else $co_centro_gestao_solic				= "61700000000";*/				
				$co_observacao					= "2";
				$co_tipo_empenho				= "5";
				$co_descricao_empenho			= "0010";
				$co_gestao_emitente				= "15253";
				$co_programa_fnde				= "03";
				$co_unidade_gestora_emitente	= "153173";
				$co_unidade_orcamentaria_solic	= "26298";
				if( !empty($dados['ptrpropostasiconv']) ){
					$nu_proposta_siconv			= $dados['ptrpropostasiconv'].'/'.$dados['ptrpropostasiconvano'];
				}
				$nu_sistema						= '2';
				$data_created 					= date("c");
				
				$sql = "SELECT distinct l.lwsid FROM par.logws l
						    inner join emenda.logerrows h ON l.lwsid = h.lwsid
						WHERE
						    h.ptrid = $ptrid
						    and h.logresposta is null
						    and h.logdatainclusao = (select max(logdatainclusao) from emenda.logerrows where ptrid = $ptrid)
						    and l.lwstiporequest = '$co_especie_empenho'";
				$request_id = $this->db->pegaUm($sql);
				 
				if( empty($request_id) ){
					$arrParam = array(
									'lwstiporequest' 	=> $co_especie_empenho,
									'usucpf' 			=> $_SESSION['usucpf']
									);
					$request_id = logWsRequisicao($arrParam, 'lwsid', 'par.logws', 'insert' );
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
			<request_id>$request_id</request_id>
			<nu_cnpj_favorecido>$nu_cgc_favorecido</nu_cnpj_favorecido>
			<nu_empenho_original>$nu_empenho_original</nu_empenho_original>
			<an_exercicio_original>$an_exercicio_original</an_exercicio_original>
			<vl_empenho>$vl_empenho</vl_empenho>
			<nu_processo>$nu_processo</nu_processo>
			<co_especie_empenho>$co_especie_empenho</co_especie_empenho>
			<co_plano_interno_solic>$co_plano_interno_solic</co_plano_interno_solic>
			<co_esfera_orcamentaria_solic>$co_esfera_orcamentaria_solic</co_esfera_orcamentaria_solic>
			<co_ptres_solic>$co_ptres_solic</co_ptres_solic>
			<co_fonte_recurso_solic>$co_fonte_recurso_solic</co_fonte_recurso_solic>
			<co_natureza_despesa_solic>$co_natureza_despesa_solic</co_natureza_despesa_solic>
			<co_centro_gestao_solic>$co_centro_gestao_solic</co_centro_gestao_solic>
			<an_convenio>$an_convenio</an_convenio>
			<nu_convenio>$nu_convenio</nu_convenio>
			<co_observacao>$co_observacao</co_observacao>
			<co_tipo_empenho>$co_tipo_empenho</co_tipo_empenho>
			<co_descricao_empenho>$co_descricao_empenho</co_descricao_empenho>
			<co_gestao_emitente>$co_gestao_emitente</co_gestao_emitente>
			<co_programa_fnde>$co_programa_fnde</co_programa_fnde>
			<co_unidade_gestora_emitente>$co_unidade_gestora_emitente</co_unidade_gestora_emitente>
			<co_unidade_orcamentaria_solic>$co_unidade_orcamentaria_solic</co_unidade_orcamentaria_solic>
			<nu_proposta_siconv>$nu_proposta_siconv</nu_proposta_siconv>
			<nu_sistema>$nu_sistema</nu_sistema>
		</params>
	</body>
</request>
XML;

			if($_SESSION['baselogin'] == "simec_desenvolvimento" || $_SESSION['baselogin'] == "simec_espelho_producao" ){
				//$urlWS = 'http://172.20.200.116/webservices/sigef/integracao/public/index.php/orcamento/ne';
				$urlWS = 'http://hmg.fnde.gov.br/webservices/sigef/index.php/orcamento/ne';
			} else {
				$urlWS = 'http://www.fnde.gov.br/webservices/sigef/index.php/orcamento/ne';
			}
				try {
					/*$xsd = APPRAIZ.'emenda/modulos/sistema/ws-sigef/xsd/xml.fnde.ws-sigef.empenho.solicitar.request.xsd';
					$arMessage = validaXML( $arqXml, $xsd );*/
					
					$arrParam = array(
									'lwsrequestdata' 	=> 'now()',
									'lwsurl' 			=> $urlWS,
									'lwsmetodo' 		=> 'solicitar',
									'lwsid' 			=> $request_id
									);
					logWsRequisicao($arrParam, 'lwsid', 'par.logws', 'alter' );
										
					$arrParam = array(
							'ptrid' 			=> $ptrid,
							'exfid' 			=> $exfid,
							'lwsid' 			=> $request_id,
							'logdatainclusao' 	=> 'now()',
							'logtipo' 			=> 'E'.substr($co_especie_empenho,1,1),
							'logenvio' 			=> str_replace( "'", '"', $arqXml),
							'usucpf' 			=> $_SESSION['usucpf']
					);
					
					$logid = logWsRequisicao($arrParam, 'logid', 'emenda.logerrows', 'insert' );

				    //if( empty( $arMessage ) ){
				    	$xml = Fnde_Webservice_Client::CreateRequest()
								->setURL($urlWS)
								->setParams( array('xml' => $arqXml, 'method' => 'solicitar') )
								->execute();
						;
				    /*} else {
				    	echo 'Erro-WS: validação xml - Solicitar Emepenho: <br>';
				    	foreach ($arMessage as $msg) {
				    		print_r( $msg['text'] );
				    	}
				    	die;
				    }*/					
					
						$xmlRetorno = $xml;
						
						$arrParam = array(
								'logid'			=> $logid,
								'logresposta' 	=> str_replace( "'", '"', $xmlRetorno)
						);
						logWsRequisicao($arrParam, 'logid', 'emenda.logerrows', 'alter' );
						
						$arrParam = array(
										'lwsresponsedata' => 'now()',
										'lwsid' => $request_id
										);
						logWsRequisicao($arrParam, 'lwsid', 'par.logws', 'alter' );
						
						$xml = simplexml_load_string( stripslashes($xml));
						
				        if ( (int) $xml->status->result ){
				        	
				        	$arrParam = array(
						        			'lwserro' => false,
						        			'lwsid' => $request_id
						        			);
				        	logWsRequisicao($arrParam, 'lwsid', 'par.logws', 'alter' );
						
				        	$obEmpenho = $xml->body->children();
				        	$nu_seq_mov_ne = (int) $obEmpenho->nu_seq_ne;
							//$nu_seq_mov_ne = 74086;
							
				        	# Atualiza a tabela de execução financeira.
							$obExecucaoFinanceira = new ExecucaoFinanceira($exfid);
							$obExecucaoFinanceira->exfnumsolempenho = $nu_seq_mov_ne;
							$obExecucaoFinanceira->semid = 1;
							$obExecucaoFinanceira->exfdataalteracao 	 = date('Y-m-d H:i:s');
							$obExecucaoFinanceira->exfdatainclusao 	     = date('Y-m-d H:i:s');
							$obExecucaoFinanceira->exfespecieempenho 	 = $co_especie_empenho;
							$obExecucaoFinanceira->exfnaturezadespesa 	 = $dados['naturezadesp'];
							$obExecucaoFinanceira->exfverifsiafi 	 	 = $verificadoSiafi;
							$obExecucaoFinanceira->exfverifcadin 	     = $verificadoCadin;
							$obExecucaoFinanceira->exfanooriginal 	     = date('Y');
							$obExecucaoFinanceira->exfcodmunicipiosiafi  = $co_municipio_siafi;
							//alterar campo data
							$obExecucaoFinanceira->usucpf = $_SESSION['usucpf'];
							$obExecucaoFinanceira->salvar();
							
							# Grava Historico
							$obExecFinanceiraHistorico = new ExecFinanceiraHistorico();
							$obExecFinanceiraHistorico->usucpf 				 = $_SESSION['usucpf'];
							$obExecFinanceiraHistorico->exfid 				 = $exfid;
							$obExecFinanceiraHistorico->semid 				 = $obExecucaoFinanceira->semid;
							$obExecFinanceiraHistorico->exfvalor 			 = $obExecucaoFinanceira->exfvalor;
							$obExecFinanceiraHistorico->exfespecieempenho	 = $obExecucaoFinanceira->exfespecieempenho;
							$obExecFinanceiraHistorico->efhiddataalteracao 	 = date('Y-m-d H:i:s');
							$obExecFinanceiraHistorico->exfverifsiafi 	 	 = $verificadoSiafi;
							$obExecFinanceiraHistorico->exfverifcadin 	     = $verificadoCadin;
							$obExecFinanceiraHistorico->salvar();
							
							$boSucesso = $obExecucaoFinanceira->commit();
							unset($obExecucaoFinanceira);
							unset($obExecFinanceiraHistorico);
							$erro = 'Solicitar Empenho no SIGEF: Realizada com sucesso.';
							echo "<span style='color: red;'><b>Solicitar Empenho Original:</b><br/><br/>".$erro."</span>";
							return true;
						} else {
							$arrParam = array(
											'lwserro' 	=> true,
											'lwsid' 	=> $request_id
											);
							logWsRequisicao($arrParam, 'lwsid', 'par.logws', 'alter' );
							
							$erroMSG = str_replace(array(chr(13),chr(10)), ' ',utf8_decode($xml->status->error->message->text));
					    	$erroMSG = str_replace( "'", '"', $erroMSG );
					    							
							$erro = "Erro-WS Solicitar Empenho no SIGEF: $erroMSG";
							echo "<span style='color: red;'><b>Detalhes da Execução - Solicitar Empenho Original:</b><br/><br/>".$erro."</span>";
							return false;
						}
		    		$count++;
				} catch (Exception $e){
					/**
					 * @var LogErroWS
					 * Bloco que grava o erro em nossa base
					 */
					$erroMSG = str_replace(array(chr(13),chr(10)), ' ',$e->getMessage());
					$erroMSG = str_replace( "'", '"', $erroMSG );
					
					$arrParam = array(
									'lwserro' 			=> true,
									'lwsresponsedata' 	=> 'now()',
									'lwsid' 			=> $request_id
									);
					logWsRequisicao($arrParam, 'lwsid', 'par.logws', 'alter' );
					
					$arrParam = array(
							'logid'		  => $logid,
							'logresposta' => str_replace( "'", '"', $xmlRetorno).' - Erro Exception: '.$erroMSG
					);
					logWsRequisicao($arrParam, 'logid', 'emenda.logerrows', 'alter' );
					
					# Erro 404 página not found
					if($e->getCode() == 404){
						$erro = "Erro-Serviço Empenho encontra-se temporariamente indisponível.\nFavor tente mais tarde.";
						echo "<span style='color: red;'><b>Detalhes da Execução - Solicitar Empenho Original:</b><br/><br/>".$erro."</span>";
						return false;
					} else {						
						$erro = "Erro-WS Solicitar Empenho no SIGEF: $erroMSG";
						echo "<span style='color: red;'><b>Detalhes da Execução - Solicitar Empenho Original:</b><br/><br/>".$erro."</span>";
						return false;
					}
				}
					
			} // Fim do foreach
			/*if($boSucesso){
				echo "Empenho solicitado com sucesso";
			} else {
				echo "Solicitação de Empenho não efetuado com sucesso";
			}*/
		} else {// Fim do if Array de Dados
			$erro = "Erro-Não há dados para Empenhar.";
			echo "<span style='color: red;'><b>Detalhes da Execução - solicitar Empenho Original:</b><br/><br/>".$erro."</span>";
			return false;
		}	
			
	}
}
