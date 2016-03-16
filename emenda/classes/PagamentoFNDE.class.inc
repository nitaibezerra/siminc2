<?php

/**
 * Modelo de Pagamento FNDE
 * @author Wesley Romualdo
 */
class PagamentoFNDE extends Modelo{	
	
	/**
	 * Nome da tabela especificada
	 * @name $stNomeTabela
	 * @var string
	 * @access protected
	 */
	protected $stNomeTabela = "emenda.ordembancaria";

	/**
	 * Chave primaria.
	 * @name $arChavePrimaria
	 * @var array
	 * @access protected
	 */
	protected $arChavePrimaria = array('orbid');

	/**
	 * Atributos da Tabela
	 * @name $arAtributos
	 * @var array
	 * @access protected
	 */
	protected $arAtributos	= array(
		'orbid' => null,
		'exfid' => null,
		'orbmesparcela' => null,
		'orbanoparcela' => null,
		'orbvalorparcela' => null,
		'orbnumsolicitacao' => null,
		'spgcodigo' => null,
		'orbnumordembancaria' => null,
		'orbvalorpagamento' => null,
		'orbdataemissao' => null,
		'orbdatapagamento' => null,
		'usucpf' => null,
		'orbdatainclusao' => null,
		'orbdataalteracao' => null,
		'orbnumparcela' => null,
	);
	
	private $ptrid = '';
	
	/**
     * Seta o ptrid
     * @name setPta
     * @author Wesley Romualdo
     * @access public
     * @param int $ptrid - id do PTA
     * @return array
     */
	public function setPta($ptrid){
		$this->ptrid = $ptrid;
	}
	
	/**
     * Pega a data atual
     * @name getDataAtual()
     * @author Wesley Romualdo
     * @access public
     * @return array
     */
	private function getDataAtual(){
		return date("c");
	}
	
	/**
     * Carrega os dados do empenho para fazer o pagamento
     * @name carregaDadosEmpenhoPagamento
     * @author Wesley Romualdo
     * @access public
     * @return array
     */
	public function carregaDadosEmpenhoPagamento(){	
		$sql = "SELECT distinct
					vede.acaid, fup.fupfuncionalprogramatica, vede.emecod,
					case when vede.emerelator = 'S' then aut.autnome||' - Relator Geral' else aut.autnome end as autnome,
					vede.gndcod, vede.mapcod, vede.foncod,
					(CASE WHEN vede.emetipo = 'E' THEN 'Emenda'
				       ELSE 'Complemento' END) as tipoemenda,
					per.pedid, tpe.tpedsc, sum(per.pervalor) as pervalor, pli.plicod, pli.plititulo, pt.ptres as pliptres, esf.esfcod, esf.esfdsc,
					(CASE WHEN vede.mapcod = '90' THEN '0 - federal'
				    	  WHEN vede.mapcod = '30' THEN '1 - estadual'
				          WHEN vede.mapcod = '40' THEN '2 - Municipal'
				          WHEN (vede.mapcod = '50' or vede.mapcod = '60') THEN '3 - Particular'
				     ELSE '' END) as esferaadm,
	                 (CASE WHEN vede.gndcod = '3' THEN '3.3.'||vede.mapcod||'.41'
	                 	   WHEN vede.gndcod = '4' THEN '4.4.'||vede.mapcod||'.42'
	                  END) as naturezaDesp,
	                 exf.exfid, exf.exfcodfontesiafi, upf.fondsc, exf.exfnumsolempenho, exf.exfnumempenhooriginal,
	                 exf.exfvalor --, sip.spgcodigo, sip.spgdescricao
				FROM 
					emenda.ptiniciativa pti
					inner join emenda.ptiniciativaespecificacao pte on pte.ptiid = pti.ptiid and pte.ptestatus = 'A'
					inner join emenda.ptiniesprecurso per on per.pteid = pte.pteid
					inner join emenda.ptemendadetalheentidade ped on ped.pedid = per.pedid
					inner join emenda.tipoensino tpe on pti.tpeid = tpe.tpeid
					inner join emenda.v_emendadetalheentidade vede on vede.edeid = ped.edeid
					inner join emenda.autor aut on aut.autid = vede.autid
					inner join emenda.v_funcionalprogramatica fup on fup.acaid = vede.acaid
					inner join emenda.execucaofinanceira exf on pti.ptrid = exf.ptrid and ped.pedid = exf.pedid 			        
					inner join monitora.pi_planointerno pli on pli.plicod = exf.plicod and pli.plisituacao in ('C','A')
					inner join monitora.pi_planointernoptres plpt on pli.pliid = plpt.pliid
					inner join monitora.ptres pt on pt.ptrid = plpt.ptrid and pt.acaid = fup.acaid and pt.ptres = exf.ptres
					inner join public.esfera esf on fup.esfcod = esf.esfcod 
	                left join financeiro.unidadeptresfonte upf on upf.fontesiafi = exf.exfcodfontesiafi
	                /*left join emenda.ordembancaria orb
	                	left join emenda.situacaopagamento sip on sip.spgcodigo = orb.spgcodigo
	                on orb.exfid = exf.exfid and orb.spgcodigo not in ('2','9')*/
				where 
					pti.ptrid = ".$this->ptrid." 
	                and exf.exfstatus = 'A'	                
				GROUP BY 
					per.pedid, tpe.tpedsc, vede.emecod, aut.autnome, vede.gndcod, vede.mapcod, vede.foncod, vede.emerelator,
					tipoemenda, fup.fupfuncionalprogramatica, vede.acaid, pli.plicod, pli.plititulo, pt.ptres, 
					esf.esfcod, esf.esfdsc, exf.exfid, exf.exfcodfontesiafi, exf.exfnumsolempenho, exf.exfnumempenhooriginal,
					exf.exfvalor, upf.fondsc --, sip.spgdescricao, sip.spgcodigo
				ORDER BY
					exf.exfid, per.pedid, tpe.tpedsc, vede.emecod, vede.gndcod, vede.mapcod, 
					vede.foncod, fup.fupfuncionalprogramatica, vede.acaid";
		
		$arDados = $this->carregar($sql);
		$arDados = $arDados ? $arDados : array();
		return $arDados;
	}
	
	/**
     * Carrega as parcelas do pagamento
     * @name carregaParcelas
     * @author Wesley Romualdo
     * @access public
     * @return array
     */
	public function carregaParcelas( $exfid ){
			
		$sql = "SELECT DISTINCT
			  	ord.orbvalorparcela, ord.orbanoparcela, ord.orbmesparcela,
			  	ptr.ptrexercicio, exf.exfid, ord.orbid, ord.orbnumsolicitacao
			FROM
				emenda.ordembancaria ord
			    inner join emenda.execucaofinanceira exf on exf.exfid = ord.exfid
				inner join emenda.planotrabalho ptr on ptr.ptrid = exf.ptrid
			    	and ord.orbvalorparcela is not null
			  		and ord.spgcodigo not in ('9')
			WHERE
			  	ptr.ptrid = ".$this->ptrid."
			  	and exf.exfid = $exfid
			  	and exf.exfstatus = 'A'
			ORDER BY
			    ord.orbanoparcela";

		$arParcela = $this->carregar($sql);
		$arParcela = ($arParcela) ? $arParcela : array();
		
		return $arParcela;
	}
	
	/**
     * Carrega dados para o pagamento
     * @name selectParaPagamento
     * @author Wesley Romualdo
     * @access public
     * @return array
     */
	public function selectParaPagamento( $exfid = '' ){
		if($this->ptrid){
			$filtro = !empty($exfid) ? "and exe.exfid = ".$exfid : '';
			$sql = "SELECT DISTINCT
						ent.enbcnpj, cc.cocseq_conta, ptr.ptrnumprocessoempenho, ptr.ptranoconvenio, ptr.ptrnumconvenio, ptr.ptrid,
					    ptmc.pmcnumconveniosiafi, to_char(ptp.pubdatapublicacao, 'YYYY-MM-DD') as pubdatapublicacao, ptr.ptrexercicio,
					    exe.exfvalor, exe.exfnumsolempenho, exe.exfnaturezadespesa, exe.exfnumempenhooriginal, exe.exfid, exe.exfanooriginal
					FROM 
						emenda.planotrabalho ptr
					    inner join emenda.entidadebeneficiada ent on ent.enbid = ptr.enbid
					    left join emenda.contacorrente cc on cc.ptrid = ptr.ptrid
					    left join emenda.ptminutaconvenio ptmc on ptmc.ptrid = ptr.ptrid and ptmc.pmcstatus = 'A'
					    inner join emenda.execucaofinanceira exe on exe.ptrid = ptr.ptrid and exe.exfstatus = 'A'
                        left join emenda.ptpublicacao ptp on ptp.pmcid = ptmc.pmcid and ptp.pubstatus = 'A'
					WHERE
						ptr.ptrid = $this->ptrid
						$filtro
					    and ptr.ptrstatus = 'A'
					ORDER BY
						exe.exfid";
				
			$arDados = $this->carregar($sql);
			$arDados = ($arDados) ? $arDados : array();
			return $arDados;
		}
	}
	
	/**
     * lista pagamentos solicitados ao FNDE
     * @name listaPagamentoSolicitado
     * @author Wesley Romualdo
     * @access public
     * @param int $perfil - codigo do perfil usuário
     * @return array
     */
	public function listaPagamentoSolicitado( $arrPerfil ){
		/*$acaoAlterarHabilitada = "'<center><img src=\"/imagens/alterar.gif\" style=\"cursor: pointer\" onclick=\"alterarPagamento( \''||exf.exfid||'\');\" border=0 alt=\"Ir\" title=\"Alterar\"> '";
		$acaoAlterarNaoHabilitada = "'<center><img src=\"/imagens/alterar_01.gif\" style=\"cursor: pointer\" border=0 alt=\"Ir\" title=\"Alterar\"> '";*/
		
		$acaoConsultarHabilitada = "'<center><img src=\"/imagens/refresh2.gif\" style=\"cursor: pointer\" onclick=\"consultarPagamentoPTA( \''||ptr.ptrid||'\', \''||exf.exfid||'\', \''||orb.orbid||'\' );\" border=0 alt=\"Ir\" title=\"Consultar Pagamento\"> '";
		$acaoConsultarNaoHabilitada = "'<center><img src=\"/imagens/refresh2_01.gif\" style=\"cursor: pointer\" border=0 alt=\"Ir\" title=\"Consultar Pagamento\"> '";
		
		$acaoconsultarPagamentoHistorico = "'&nbsp;<img src=\"/imagens/consultar.gif\" style=\"cursor: pointer\" onclick=\"consultarPagamentoHistorico( \''||ptr.ptrid||'\', \''||orb.orbid||'\' );\" border=0 alt=\"Ir\" title=\"Consultar Histórico Pagamento\"></center>'";
		
		$acaoAnulacaoTotalHabilitada = "'<img src=\"/imagens/excluir.gif\" style=\"cursor: pointer\" onclick=\"cancelarPagamento(\''||ptr.ptrid||'\', \''||exf.exfid||'\',\''||orb.orbnumsolicitacao||'\',\''||orb.orbid||'\');\" border=0 alt=\"Ir\" title=\"Anulação Total do Pagamento\">'";
		$acaoAnulacaoTotalNaoHabilitada = "'<img src=\"/imagens/excluir_01.gif\" style=\"cursor: pointer\" border=0 alt=\"Ir\" title=\"Anulação Total do Pagamento\">'";
		
		if( in_array( SUPER_USUARIO, $arrPerfil) || in_array( ADMINISTRADOR_INST, $arrPerfil) ){
			$acaoConsultarNaoHabilitada = $acaoConsultarHabilitada;
		}
		
		/*$acaoAlterarHabilitada || $acaoConsultarHabilitada || $acaoAnulacaoTotalHabilitada else 
					$acaoAlterarNaoHabilitada || $acaoConsultarNaoHabilitada || $acaoAnulacaoTotalNaoHabilitada */
		
		$permissao = verificaPermissaoPerfil( 'analise', 'boolean' );
		
		if( !$permissao ){
			//$acaoAlterarHabilitada = $acaoAlterarNaoHabilitada;
			$acaoConsultarHabilitada = $acaoAlterarNaoHabilitada;
			$acaoConsultarHabilitada = $acaoConsultarNaoHabilitada;
			$acaoAnulacaoTotalHabilitada = $acaoAnulacaoTotalNaoHabilitada;
		}
		
		$sql  = "SELECT DISTINCT
					(CASE WHEN orbnumordembancaria is null THEN 
					$acaoConsultarHabilitada || $acaoAnulacaoTotalHabilitada else 
					$acaoConsultarNaoHabilitada || case when sip.spgcodigo = '6' and sip.spgcodigo <> '9' then $acaoAnulacaoTotalHabilitada else $acaoAnulacaoTotalNaoHabilitada end
					end)||
						$acaoconsultarPagamentoHistorico
					as acao,
				    vede.emecod,
				    sip.spgcodigo,
	                exf.exfnumempenhooriginal,
	                orb.orbmesparcela,
	                orb.orbnumparcela,
	                orb.orbvalorparcela,
	                orb.orbnumsolicitacao,
	                orb.orbnumordembancaria,
	                sip.spgdescricao,
	                to_char(orb.orbdatainclusao, 'DD/MM/YYYY HH24:MI:SS') as datainclusao, orb.orbdatainclusao
				FROM
	            	emenda.planotrabalho ptr
	                inner join emenda.execucaofinanceira exf on exf.ptrid = ptr.ptrid
	                inner join emenda.ordembancaria orb on orb.exfid = exf.exfid
					inner join emenda.ptemendadetalheentidade ped on ped.ptrid = ptr.ptrid
					inner join emenda.v_emendadetalheentidade vede on vede.edeid = ped.edeid and ped.pedid = exf.pedid
	                inner join emenda.situacaopagamento sip on sip.spgcodigo = orb.spgcodigo
				WHERE 
					ptr.ptrid = $this->ptrid 
	                and exf.exfstatus = 'A'
	                and vede.edestatus = 'A'
	                and orb.orbnumsolicitacao is not null
				ORDER BY 
					vede.emecod,
					orb.orbdatainclusao desc";
	
		print '<table border="0" cellspacing="0" cellpadding="3" align="center" bgcolor="#DCDCDC" class="tabela" style="border-top: none; border-bottom: none;">';
		print '<tr><td bgcolor="#e9e9e9" align="center" style="FILTER: progid:DXImageTransform.Microsoft.Gradient(startColorStr=\'#FFFFFF\', endColorStr=\'#dcdcdc\', gradientType=\'1\')" class="subtitulocentro" >Lista de Pagamentos</td></tr></table>';
		
		$arDadosP = $this->carregar( $sql );
		$arDadosP = ( $arDadosP ? $arDadosP : array() );
		
		foreach ($arDadosP as $key => $v) {
			$registro[$key] = array(
								"acao" => $v['acao'],
								"emecod" => $v['emecod'],
								"ne" => $v['exfnumempenhooriginal'],
								"parcela" => $v['orbnumparcela'],
								"orbmesparcela" => $v['orbmesparcela'],
								"orbvalorparcela" => $v['orbvalorparcela'],
								"orbnumsolicitacao" => $v['orbnumsolicitacao'],
								"situacao" => $v['spgdescricao'],
								"datainclusao" => $v['datainclusao'],
								"numordembancária" => $v['orbnumordembancaria'],
								);	
		}
		
		
		$cabecalho 	    = array("Ação","Código", "Nº Empenho", "Parcela", "Mês", "Valor", "Nº solicitacao", "Situaçao", "Data Inclusao", "Nº Ordem bancária");
		$this->monta_lista_array($registro, $cabecalho, 5000, 20, '', 'center', '');
	}
	
	/**
     * Solicita pagamento ao FNDE
     * @name solicitaPagamento
     * @author Wesley Romualdo
     * @access public
     * @param array $post - Dados do formulário
     * @return array
     */	
	public function solicitaPagamento( $post ){
		$usuario 		 = $post['usuario'];
		$senha   		 = $post['senha'];
		$xmlRetorno = "";
		$arParcela = json_decode( urldecode( $post['arparcela'] ) );
		
		$arRetorno = array();
		$arDados = $this->selectParaPagamento( $post['exfid'] );
		$boSucesso = $count = 0;
		
		foreach ($arParcela as $key => $p) {
			if( $p->exfid == $post['exfid'] ) $valor += (float)$p->orbvalorparcela;
		}
		
		$valor = number_format( $valor, 2, ',', '.' );
		$valor = str_replace( ".", '', $valor );
		$valor = str_replace( ",", '.', $valor );
		if($arDados){
			foreach($arDados as $dados){
				$nu_cgc_favorecido 				= $dados['enbcnpj'];
				$nu_seq_conta_corrente_favorec 	= $dados['cocseq_conta'];
				$nu_processo 					= $dados['ptrnumprocessoempenho'];
				$an_convenio_original 			= $dados['ptranoconvenio'];
				$nu_convenio_original 			= $dados['ptrnumconvenio'];
				$nu_convenio_siafi 				= $dados['pmcnumconveniosiafi'];
				//$valor		 					= $dados['exfvalor'];
				// Solicitado pelo Daniel(Mara) - 12/04/2012 $an_referencia	= $dados['ptrexercicio'];
				$an_referencia      			= date("Y");
				$dt_publicacao      			= $dados['pubdatapublicacao'];
				$exfnumempenhooriginal 			= substr($dados['exfnumempenhooriginal'], 6);
				$anoempenho			 			= $dados['exfanooriginal'];
				$sub_tipo_documento				= '01';
				$nu_sistema						= '2';
				$unidade_gestora 				= "153173";
				$gestao 						= "15253";
				$co_programa_fnde				= '03';
				
				#verifica se tipo de valor da natureza
				if( substr($dados['exfnaturezadespesa'],0,1) == '4' ){
					$vl_capital = $valor;
					$vl_custeio = 0;
				}else{
					$vl_capital = 0;
					$vl_custeio = $valor;					
				}
				
				$item = '';
				//$arParcela = self::carregaParcelaPagamento( $exfid );
				foreach ($arParcela as $key => $p) {
					if( $p->exfid == $dados['exfid'] ){
						$parcela 	  = $p->numparcela;
						$exfid	 	  = $p->exfid;
						$vparcela 	  = $p->orbvalorparcela;
						$anoparcela   = $p->orbanoparcela;
						$mesparcela   = $p->orbmesparcela;
						$ptrexercicio = $anoempenho;
						
						$item = "<item>
								  <nu_parcela>$parcela</nu_parcela> 
								  <an_exercicio>$ptrexercicio</an_exercicio> 
								  <vl_parcela>$vparcela</vl_parcela> 
								  <an_parcela>$anoparcela</an_parcela> 
								  <nu_mes>$mesparcela</nu_mes> 
								  <nu_documento_siafi_ne>$exfnumempenhooriginal</nu_documento_siafi_ne> 
						  		</item>";									
				
						$sql = "SELECT distinct l.lwsid FROM par.logws l
								    inner join emenda.logerrows h ON l.lwsid = h.lwsid
								WHERE
								    h.ptrid = {$post['ptrid']}
								    and h.logresposta is null
								    and h.logdatainclusao = (select max(logdatainclusao) from emenda.logerrows where ptrid = {$post['ptrid']})
								    and l.lwstiporequest = '05'";
						$request_id = $this->pegaUm($sql);
						 
						if( empty($request_id) ){
							$arrParam = array(
											'lwstiporequest' 	=> '05',
											'usucpf' 			=> $_SESSION['usucpf']
											);
							$request_id = logWsRequisicao($arrParam, 'lwsid', 'par.logws', 'insert' );
						}
	//<request_id>$request_id</request_id>
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
			<request_id>$request_id</request_id>
			<nu_cgc_favorecido>$nu_cgc_favorecido</nu_cgc_favorecido>
			<nu_seq_conta_corrente_favorec>$nu_seq_conta_corrente_favorec</nu_seq_conta_corrente_favorec>
			<nu_processo>$nu_processo</nu_processo>
			<an_convenio_original>$an_convenio_original</an_convenio_original>
			<nu_convenio_original>$nu_convenio_original</nu_convenio_original>
			<nu_convenio_siafi>$nu_convenio_siafi</nu_convenio_siafi>
			<vl_custeio>$vl_custeio</vl_custeio>
			<vl_capital>$vl_capital</vl_capital>
			<an_referencia>$an_referencia</an_referencia>
			<sub_tipo_documento>$sub_tipo_documento</sub_tipo_documento>
			<nu_sistema>$nu_sistema</nu_sistema>
			<unidade_gestora>$unidade_gestora</unidade_gestora>
			<gestao>$gestao</gestao>
			<co_programa_fnde>$co_programa_fnde</co_programa_fnde>
			<detalhamento_pagamento>
				$item
			</detalhamento_pagamento>
		</params>
	</body>
</request>
XML;
				
					if($_SESSION['baselogin'] == "simec_desenvolvimento" || $_SESSION['baselogin'] == "simec_espelho_producao" ){
						$urlWS = 'http://172.20.200.116/webservices/sigef/integracao/public/index.php/financeiro/ob';
					} else {
						$urlWS = 'http://www.fnde.gov.br/webservices/sigef/index.php/financeiro/ob';
					}
					
	//ver(simec_htmlentities($arqXml), d);
	
					$arrParam = array(
							'lwsrequestdata' 	=> 'now()',
							'lwsurl' 			=> $urlWS,
							'lwsmetodo' 		=> 'solicitar',
							'lwsid' 			=> $request_id
					);
					logWsRequisicao($arrParam, 'lwsid', 'par.logws', 'alter' );
					
					$arrParam = array(
							'ptrid' 			=> $post['ptrid'],
							'exfid' 			=> $exfid,
							'lwsid' 			=> $request_id,
							'logdatainclusao' 	=> 'now()',
							'logtipo' 			=> 'PG',
							'logenvio' 			=> str_replace( "'", '"', $arqXml),
							'usucpf' 			=> $_SESSION['usucpf']
					);
						
					$logid = logWsRequisicao($arrParam, 'logid', 'emenda.logerrows', 'insert' );
					try {						
						$xml = Fnde_Webservice_Client::CreateRequest()
								->setURL($urlWS)
								->setParams( array('xml' => $arqXml, 'method' => 'solicitar') )
								->execute();
								
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
					        	$nu_seq_mov_pag = $xml->body->nu_registro_ob;
					        	
					        	$arrParam = array(
						        			'lwserro' => false,
						        			'lwsid' => $request_id
						        			);
				        		logWsRequisicao($arrParam, 'lwsid', 'par.logws', 'alter' );
					        									
								# Grava dados do pagamento na tabela ordem bancaria
								$sql = "INSERT INTO emenda.ordembancaria(exfid, orbnumparcela, orbmesparcela, orbanoparcela, orbvalorparcela, orbnumsolicitacao, spgcodigo, usucpf, orbdatainclusao) 
										VALUES ( $exfid, '$parcela', '$mesparcela', $anoparcela, $vparcela, '$nu_seq_mov_pag', '0', '{$_SESSION['usucpf']}', 'now()') RETURNING orbid";
				
								$orbid = $this->pegaUm( $sql );
								
								# Grava o Historico
								$obHistOrdemBanc = new Ordembanchistorico();
								$obHistOrdemBanc->orbid = $orbid;
								$obHistOrdemBanc->spgcodigo = '0';
								$obHistOrdemBanc->orbvalorpagamento = $valor;
								$obHistOrdemBanc->usucpf = $_SESSION['usucpf'];
								$obHistOrdemBanc->obhdataalteracao = date('Y-m-d H:i:s');
								$obHistOrdemBanc->salvar();
								
								$boSucesso = $this->commit();
							} else {
								$arrParam = array(
											'lwserro' 	=> true,
											'lwsid' 	=> $request_id
											);
								logWsRequisicao($arrParam, 'lwsid', 'par.logws', 'alter' );
								
								$erroMSG = str_replace(array(chr(13),chr(10)), ' ',utf8_decode($xml->status->error->message->text));
						    	$erroMSG = str_replace( "'", '"', $erroMSG );
						    							
								//echo "Erro-WS Solicitar Pagamento no SIGEF: $erroMSG'";
								
							 	throw new Exception(utf8_decode($xml->status->error->message->text));
							}
			    		$count++;
					} catch (Exception $e){
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
							$erro =  "Erro-Serviço Pagamento encontra-se temporariamente indisponível.\nFavor tente mais tarde.";
							echo "<span style='color: blue;'><b>Detalhes da Execução - Consulta Pagamento SIGEF:</b><br/><br/>".$erro."</span>";
							die;
						}						
						echo "<span style='color: blue;'><b>Detalhes da Execução - Consulta Pagamento SIGEF:</b><br/><br/>".$erroMSG."</span>";
						die;
					} //fim do if que verifica exfid
					}
				}//fim do foreach parcela				
			} // Fim do foreach
			if($boSucesso){
				$erro = "Pagamento solicitado com sucesso.";
				echo "<span style='color: blue;'><b>Detalhes da Execução - Consulta Pagamento SIGEF:</b><br/><br/>".$erro."</span>";
				die;
			} else {
				$erro = "Solicitação de Pagamento não efetuado com sucesso.";
				echo "<span style='color: blue;'><b>Detalhes da Execução - Consulta Pagamento SIGEF:</b><br/><br/>".$erro."</span>";
				die;			
			}
		} else {// Fim do if Array de Dados
			$erro = "Erro-Não há dados para Pagamento.";
			echo "<span style='color: blue;'><b>Detalhes da Execução - Consulta Pagamento SIGEF:</b><br/><br/>".$erro."</span>";
			die;	
		}
	}
	
	/**
     * Cancelar pagamento ao FNDE
     * @name cancelarPagamento
     * @author Wesley Romualdo
     * @access public
     * @param array $post - Dados do formulário
     * @return array
     */
	public function cancelarPagamento(&$post){
		$usuario = $post['usuario'];
		$senha 	 = $post['senha'];
		$xmlRetorno = "";
		$nu_seq_ob = $post['orbnumsolicitacao'];
		
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
        <nu_seq_ob>$nu_seq_ob</nu_seq_ob>
		</params>
	</body>
</request>
XML;

		if($_SESSION['baselogin'] == "simec_desenvolvimento" || $_SESSION['baselogin'] == "simec_espelho_producao" ){
			$urlWS = 'http://172.20.200.116/webservices/sigef/integracao/public/index.php/financeiro/ob';
		} else {
			$urlWS = 'http://www.fnde.gov.br/webservices/sigef/index.php/financeiro/ob';
		}
			$xml = Fnde_Webservice_Client::CreateRequest()
				->setURL($urlWS)
				->setParams( array('xml' => $arqXml, 'method' => 'cancelar') )
				->execute();
			$xmlRetorno = $xml;
			
			$arrParam = array(
						'ptrid' => $post['ptrid'],
						'exfid' => $post['exfid'],
						'logTipo' => 'PX',
						'xmlEnvio' => $arqXml,
						'xmlRetorno' => $xmlRetorno,
						);
		
			logWsSIGEF($arrParam);
			
			$xml = simplexml_load_string( stripslashes($xml));
			
		    if ( (int) $xml->status->result ){
		    					
				$sql = "UPDATE emenda.ordembancaria SET 
						  	spgcodigo = 9,
						  	usucpf = '{$_SESSION['usucpf']}',
						  	orbdataalteracao = now()
						WHERE exfid = ".$post['exfid']." and orbid = ".$post['orbid'];
				
				$this->executar( $sql );
				
				$boSucesso = $this->commit();
				unset($obLogErroWS);		    	
		    	
			    if($boSucesso){
					echo "Pagamento cancelado com sucesso.";
					die;
				} else {
					echo "Pagamento não cancelado com sucesso.";
					die;			
				}
			    
		    } else {
		    	$erroMSG = str_replace(array(chr(13),chr(10)), ' ',utf8_decode($xml->status->error->message->text));
		    	$erroMSG = str_replace( "'", '"', $erroMSG );
		    							
				echo "Erro-WS Cancelamento de Pagamento no SIGEF: $erroMSG";
					
		        //throw new Exception(utf8_decode($xml->status->error->message->text));
		    }
		} catch (Exception $e){
			$erroMSG = str_replace(array(chr(13),chr(10)), ' ',$e->getMessage());
			$erroMSG = str_replace( "'", '"', $erroMSG );
			
			$arrParam = array(
						'ptrid' => $post['ptrid'],
						'exfid' => $post['exfid'],
						'logTipo' => 'PX',
						'xmlEnvio' => $arqXml,
						'xmlRetorno' => $xmlRetorno.' - '.$erroMSG,
						);
		
			logWsSIGEF($arrParam);
			
			# Erro 404 página not found
			if($e->getCode() == 404){
				echo "Erro-Serviço Pagamento encontra-se temporariamente indisponível.\nFavor tente mais tarde.";
				die;
			}	
			echo "Erro-WS Cancelar Pagamento no SIGEF: $erroMSG";
			die;
		}
	}
	
	/**
     * Consulta pagamento ao FNDE
     * @name consultarPagamento
     * @author Wesley Romualdo
     * @access public
     * @param array $post - Dados do formulário
     * @return array
     */
	 # $boConsultaPorPtrid flag para dizer que será consultado somente um pagamento. Só é passado como true, na lista de Pagamento
	public function consultarPagamento(&$post, $boConsultaPorPtrid = false, $boAuto = false){
		$where = "";
		if($boConsultaPorPtrid){
			$where = " and exe.ptrid = {$post['ptrid']} and orb.orbid = {$post['orbid']}";
		}
		$usuario = $post['usuario'];
		$senha 	 = $post['senha'];
		$xmlRetorno = "";
		
		$arErro    = array();
		$arRetorno = array();
		
		$sql = "SELECT 
					orb.orbid, orb.exfid, orb.orbnumsolicitacao, orb.usucpf, 
					orb.orbnumordembancaria, orb.spgcodigo, exe.ptrid
				FROM 
				  emenda.ordembancaria orb
				  inner join emenda.execucaofinanceira exe
				  	on exe.exfid = orb.exfid
				WHERE
					orb.orbnumsolicitacao is not null
				    and exe.exfstatus = 'A' 
					$where";
					
		$arDados = $this->carregar($sql);
		$arDados = ($arDados) ? $arDados : array();
		$count = 0;
		
		foreach($arDados as $dados){
			$exfid 			   	= $dados['exfid'];
			$nu_seq_ob  		= $dados['orbnumsolicitacao'];
			$orbid  			= $dados['orbid'];
			$situacao			= $dados['spgcodigo'];
			
			if(!$post['ptrid']){
				$post['ptrid'] = $dados['ptrid'];
			}
			
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
        <nu_seq_ob>$nu_seq_ob</nu_seq_ob>
		</params>
	</body>
</request>
XML;

    
		/*if($_SESSION['baselogin'] == "simec_desenvolvimento" || $_SESSION['baselogin'] == "simec_espelho_producao" ){
			$urlWS = 'http://172.20.200.116/webservices/sigef/integracao/public/index.php/financeiro/ob';
		} else {*/
			$urlWS = 'http://www.fnde.gov.br/webservices/sigef/index.php/financeiro/ob';
		//}
			$xml = Fnde_Webservice_Client::CreateRequest()
				->setURL($urlWS)
				->setParams( array('xml' => $arqXml, 'method' => 'consultar') )
				->execute();
				/*$xml = Fnde_Webservice_Client::CreateRequest()
						->setURL($urlWS)
						->setParams( array('xml' => $arqXml, 'login' => $usuario, 'senha' => $senha) )
						->execute();*/
				$xmlRetorno = $xml;
				
				$arrParam = array(
							'ptrid' => $post['ptrid'],
							'exfid' => $exfid,
							'logTipo' => 'PC',
							'xmlEnvio' => $arqXml,
							'xmlRetorno' => $xmlRetorno,
							);
			
				logWsSIGEF($arrParam);
				
				$xml = simplexml_load_string( stripslashes($xml));
				//ver($xml,d);
			    if ( (int) $xml->status->result ){
			    	
			        $obPagamento = $xml->body->row->children();
					
			    	$situacao_documento = (string) $obPagamento->situacao_documento;
				    $situacao_documento = trim(substr($situacao_documento,0,strpos($situacao_documento, '-')));
			    	$nu_seq_ob = (string) $obPagamento->nu_seq_ob;
			    	$numero_documento = (string) $obPagamento->numero_documento;
			    	$dt_emissao = (string) $obPagamento->data_documento;
			    	$valor_pagamento = (string) $obPagamento->valor_ob;
			    	$nu_processo = (string) $obPagamento->processo;
			    	$status = (string) $obPagamento->status;
			    	$status = trim(substr($status,0,strpos($status, '-')));
			    	
			    	$dt_emissao = ($dt_emissao ? "'".formata_data_sql($dt_emissao)."'" : 'null');
			    	$orbdatapagamento = $dt_emissao;
			    	$valor_pagamento = ($valor_pagamento ? "'".$valor_pagamento."'" : 'null');
			    	$nu_seq_ob = ($nu_seq_ob ? "'".$nu_seq_ob."'" : 'null');
			    	$numero_documento = ($numero_documento ? "'".$numero_documento."'" : 'null');
			    	
			    	$boSucesso = true;
			    	if( /*$situacao != $situacao_documento &&*/ $status == 1 ){
				    	$sql = "UPDATE 
								  emenda.ordembancaria  
								SET 
								  spgcodigo = '$situacao_documento',
								  orbnumordembancaria = $numero_documento,
								  orbnumsolicitacao = $nu_seq_ob,
								  orbvalorpagamento = $valor_pagamento,
								  orbdataemissao = $dt_emissao,
								  orbdatapagamento = $orbdatapagamento,
								  usucpf = '".$_SESSION['usucpf']."',
								  orbdataalteracao = 'now()'
								 
								WHERE 
								  orbid = $orbid";

				    	$this->executar( $sql );
				    	
				    	$sql = "INSERT INTO emenda.ordembanchistorico(
								  orbid,
								  spgcodigo,
								  usucpf,
								  orbvalorpagamento,
								  obhdataalteracao
								) 
								VALUES (
								  $orbid,
								  '$situacao_documento',
								  '".$_SESSION['usucpf']."',
								  $valor_pagamento,
								  'now()')";
							  
						$this->executar( $sql );			    	
				    	$boSucesso = $this->commit();
				    	
				    	/*if( !empty($nu_ordembancaria) ){
							if( !$boConsultaPorPtrid ){
								# se o ws retornou o numero da ordem bancária, altera o estado do workflow para RECURSO_LIBERADO
								$this->alteraEstado_wf( $dados['ptrid'] ); 
							}
						}*/
			    	}
			    	
			    	
				    if($boSucesso){
				    	$erro = 'Consulta Pagamento no SIGEF: '.$xml->status->message->text.'<br>'.utf8_decode((string) $obPagamento->status);
						echo "<span style='color: blue;'><b>Detalhes da Execução - Consulta Pagamento:</b><br/><br/>".$erro."</span>";
						die;
					} else {
						$erro = 'Consulta Pagamento no SIGEF não efetuada com sucesso.';
						echo "<span style='color: blue;'><b>Detalhes da Execução - Consulta Pagamento:</b><br/><br/>".$erro."</span>";
						die;
					}
				    
			    } else {
			    	$erroMSG = str_replace(array(chr(13),chr(10)), ' ',utf8_decode($xml->status->error->message->text));
			    	$erroMSG = str_replace( "'", '"', $erroMSG );
			    							
					//echo "Erro-WS Consultar Pagamento no SIGEF: $erroMSG";
					
			        throw new Exception(utf8_decode($xml->status->error->message->text));
			    }
			} catch (Exception $e){
				$erroMSG = str_replace(array(chr(13),chr(10)), ' ',$e->getMessage());
				$erroMSG = str_replace( "'", '"', $erroMSG );
				
				$arrParam = array(
							'ptrid' => $post['ptrid'],
							'exfid' => $exfid,
							'logTipo' => 'PC',
							'xmlEnvio' => $arqXml,
							'xmlRetorno' => $xmlRetorno.' - '.$erroMSG,
							);
			
				logWsSIGEF($arrParam);
				
				# Erro 404 página not found
				if($e->getCode() == 404){
					$erro = "Erro-Serviço Pagamento encontra-se temporariamente indisponível.\nFavor tente mais tarde.";
					echo "<span style='color: blue;'><b>Detalhes da Execução - Consulta Pagamento:</b><br/><br/>".$erro."</span>";
					die;
				}
				
				echo "<span style='color: blue;'><b>Detalhes da Execução - Consulta Pagamento no SIGEF:</b><br/><br/>".$erroMSG."</span>";
			}
			$count++;
		} // fim foreach
		
		if(!$boConsultaPorPtrid){
			$sql = "select 
						pt.ptrcod || '/' || pt.ptrexercicio as planotrabalho,  
						exfvalor as valor,
						orb.orbnumsolicitacao, 
						orb.orbnumordembancaria,
						orb.usucpf, 
						se.semcod || ' - ' ||se.semdesc as situacao
					from emenda.planotrabalho pt
						inner join emenda.execucaofinanceira ef on pt.ptrid = ef.ptrid
						inner join emenda.ordembancaria orb on orb.exfid = ef.exfid 
						inner join emenda.situacaoempenho se on ef.semid = se.semid 
					where ef.exfnumsolempenho is not null and orb.orbnumordembancaria is not null";
			$arDados = $this->carregar($sql);			
			$arDados = ($arDados) ? $arDados : array();
			
			$this->cria_aba( $abacod_tela, $url, '' );
			
			if($boAuto){
				ob_start();
			}
					
			monta_titulo( $titulo_modulo, 'Pagamentos Atualizados' );
			
			$cabecalho = array("Plano de Trabalho", "Valor", "Nº Solicitação", "Ordem Bancária", "Usuário", "Situação");
			$this->monta_lista_array($arDados, $cabecalho, 50, 20, '', 'center','');
			
			/*if($boAuto){
				$conteudo = ob_get_contents();
				ob_end_clean();
				
				
				 * ENVIANDO EMAIL CONFIRMANDO O PROCESSAMENTO
				 
				require_once APPRAIZ . 'includes/phpmailer/class.phpmailer.php';
				require_once APPRAIZ . 'includes/phpmailer/class.smtp.php';
				$mensagem = new PHPMailer();
				$mensagem->Host         = "localhost";
				$mensagem->Mailer       = "smtp";
				$mensagem->FromName		= "Empenhos Atualizadas";
				$mensagem->From 		= "simec@mec.gov.br";
				$mensagem->AddAddress( "thiago.barbosa@mec.gov.br", "Wesley Silva" );
				$mensagem->Subject = "Empenhos Atualizadas";
				
				$mensagem->Body = $conteudo;
				$mensagem->IsHTML( true );
				$mensagem->Send();
			}*/
			
			
		}		
		
		# Feito isso Para não parar com a rotina automatica
		if(!$boAuto){
			die;
		}
	}
	
	/**
     * Consulta pagamento geral
     * @name consultarPagamentoGeral
     * @author Wesley Romualdo
     * @access public
     * @param array $post - Dados do formulário
     * @return array
     */
	public function consultarPagamentoGeral(&$post){
		
		/* configurações */
		ini_set("memory_limit", "3000M");
		set_time_limit(0);
		/* FIM configurações */
		
		$usuario = $post['usuario'];
		$senha 	 = $post['senha'];
		$xmlRetorno = "";
		$arErro    = array();
		$arRetorno = array();
		
		$sql = "SELECT 
					orb.orbid, orb.exfid, orb.orbnumsolicitacao, orb.usucpf, 
					orb.orbnumordembancaria, orb.spgcodigo, pl.ptrid, pl.ptrcod||'/'||pl.ptrexercicio as ptrcod
				FROM 
				  emenda.ordembancaria orb
				  inner join emenda.execucaofinanceira exe on exe.exfid = orb.exfid 
				  inner join emenda.planotrabalho pl on pl.ptrid = exe.ptrid and pl.ptrstatus = 'A'
				WHERE
					orb.orbnumsolicitacao is not null
					and orb.spgcodigo <> '9'
				    and exe.exfstatus = 'A' 
				    --AND ptridpai IS NULL
				    AND pl.ptrexercicio='".$_SESSION['exercicio']."'";
					
		$arDados = $this->carregar($sql);
		$arDados = ($arDados) ? $arDados : array();
		
		if(!$arDados) {
			$htmllog .= "<p align=center><a href=emenda.php?modulo=principal/consultarPagamento&acao=A>Clique aqui</a> para solicitar novamente.</p>";
			$htmllog .= "<table class=listagem width=95% align=center bgcolor=#f5f5f5 cellSpacing=1 cellPadding=3>";
			$htmllog .= "<tr><td class=SubTituloCentro>Não existem pagamentos no ano corrente</td></tr>";
			$htmllog .= "</table>";
			die($htmllog);
		}
		
		$count = 0;
		$htmllog .= "<p align=center><a href=emenda.php?modulo=principal/consultarPagamento&acao=A>Clique aqui</a> para solicitar novamente.</p>";
		$htmllog .= "<table class=listagem width=95% align=center bgcolor=#f5f5f5 cellSpacing=1 cellPadding=3>";
		$htmllog .= "<tr>";
		$htmllog .= "<td class=SubTituloCentro>PTA</td>";
		$htmllog .= "<td class=SubTituloCentro>Ordem Bancária</td>";
		$htmllog .= "<td class=SubTituloCentro>Data Emissão</td>";
		$htmllog .= "<td class=SubTituloCentro>Situação</td>";
		$htmllog .= "<td class=SubTituloCentro>Observações</td>";
		$htmllog .= "</tr>";
		
		foreach($arDados as $dados) {
			
			$exfid 			   	= $dados['exfid'];
			$nu_seq_ob			= $dados['orbnumsolicitacao'];
			$orbid  			= $dados['orbid'];
			$situacao			= $dados['spgcodigo'];
			$ptrcod				= $dados['ptrcod'];
			$ptrid				= $dados['ptrid'];
			
			try {
				
/*			    $arqXml = <<<XML
<?xml version='1.0' encoding='iso-8859-1'?><request><header><app>string</app><version>string</version><created>{$this->getDataAtual()}</created>
</header>
	<body>
	<params>
		<pagamento>
			<nu_seq_mov_pag>$orbnumsolicitacao</nu_seq_mov_pag>
		</pagamento>
	</params>
	</body>
</request>
XML;
*/
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
        <nu_seq_ob>$nu_seq_ob</nu_seq_ob>
		</params>
	</body>
</request>
XML;
	    		
				
    
				if ( $_SESSION['baselogin'] == "simec_desenvolvimento" || $_SESSION['baselogin'] == "simec_espelho_producao" ){
					$urlWS = "http://172.20.200.116/webservices/sigefemendas/integracao/web/dev.php/pagamento/consultar/";						
				} else {
					//$urlWS = "http://www.fnde.gov.br/webservices/sigefemendas/index.php/pagamento/consultar/";
					$urlWS = 'http://www.fnde.gov.br/webservices/sigef/index.php/financeiro/ob';
				}
				
				$xsd = APPRAIZ.'emenda/modulos/sistema/ws-sigef/xsd/xml.fnde.ws-sigef.pagamento.consultar.request.xsd';
			
				//$arMessage = validaXML( $arqXml, $xsd );

			   // if( empty( $arMessage ) ){
			    	$xml = Fnde_Webservice_Client::CreateRequest()
						->setURL($urlWS)
						//->setParams( array('xml' => $arqXml, 'login' => $usuario, 'senha' => $senha) )
						->setParams( array('xml' => $arqXml, 'method' => 'consultar') )
						->execute()
					;
			   /* } else {
			    	echo 'Erro-WS: validação xml - Consultar Pagamento: <br>';
			    	foreach ($arMessage as $msg) {
			    		print_r( $msg['text'] );
			    	}
			    	die;
			    }*/
			    
				$xmlRetorno = $xml;
				
				$xml = simplexml_load_string( stripslashes($xml));
				//ver($xml,d);
			    if ( (int) $xml->status->result ){
			    	
			        $obPagamento = $xml->body->row->children();
			    	
			    	$situacao_documento_comp = (string) $obPagamento->situacao_documento;
				    $situacao_documento = trim(substr($situacao_documento_comp,0,strpos($situacao_documento_comp, '-')));
			    	$nu_seq_ob = (string) $obPagamento->nu_seq_ob;
			    	$numero_documento = (string) $obPagamento->numero_documento;
			    	$dt_emissao = (string) $obPagamento->data_documento;
			    	$valor_pagamento = (string) $obPagamento->valor_ob;
			    	$nu_processo = (string) $obPagamento->processo;
			    	$status = (string) $obPagamento->status;
			    	$status = trim(substr($status,0,strpos($status, '-')));
			    	
			    	if( /*$situacao != $situacao_documento &&*/ $status == 1 ){
			    		
						$htmllog .= "<tr>";
						$htmllog .= "<td><center><b>".$ptrcod."</b></center></td>";
						$htmllog .= "<td><center><b>".(($numero_documento)?$numero_documento:'-')."</b></center></td>";
						$htmllog .= "<td><center><b>".(($dt_emissao)?$dt_emissao:'-')."</b></center></td>";
						$htmllog .= "<td><center><b>".$situacao_documento_comp."</b></center></td>";
						$htmllog .= "<td>&nbsp;</td>";
						$htmllog .= "</tr>";
			    		
			    					    	
				    	$sql = "UPDATE 
								  emenda.ordembancaria  
								SET 
								  spgcodigo = '$situacao_documento',
								  orbnumordembancaria = ".(($numero_documento)?"'".$numero_documento."'":"NULL").",
								  orbnumsolicitacao = ".(($nu_seq_ob)?"'".$nu_seq_ob."'":"NULL").",
								  orbvalorpagamento = ".(($valor_pagamento)?"'".$valor_pagamento."'":"NULL").",
								  orbdataemissao = ".(($dt_emissao)?"'".formata_data_sql($dt_emissao)."'":"NULL").",
								  orbdatapagamento = ".(($dt_emissao)?"'".formata_data_sql($dt_emissao)."'":"NULL").",
								  usucpf = '".$_SESSION['usucpf']."',
								  orbdataalteracao = NOW()
								 
								WHERE 
								  orbid = $orbid";

				    	$this->executar( $sql );
				    	
				    	$sql = "INSERT INTO emenda.ordembanchistorico(
								  orbid,
								  spgcodigo,
								  usucpf,
								  orbvalorpagamento,
								  obhdataalteracao
								) 
								VALUES (
								  $orbid,
								  '$situacao_documento',
								  '".$_SESSION['usucpf']."',
								  ".(($valor_pagamento)?"'".$valor_pagamento."'":"NULL").",
								  NOW())";
							  
						$this->executar( $sql );			    	
				    	$this->commit();
				    	
				    	/*if( !empty($nu_ordembancaria) ){
							# se o ws retornou o numero da ordem bancária, altera o estado do workflow para RECURSO_LIBERADO
							$this->alteraEstado_wf( $ptrid ); 
						}*/
						
			    	} else {
			    		
						$htmllog .= "<tr>";
						$htmllog .= "<td><center><b>".$ptrcod."</b></center></td>";
						$htmllog .= "<td><center><b>-</b></center></td>";
						$htmllog .= "<td><center><b>-</b></center></td>";
						$htmllog .= "<td><center><b>-</b></center></td>";
						$htmllog .= "<td>".utf8_decode($obPagamento->status->message->text)."</td>";
						$htmllog .= "</tr>";
			    		
			    	}
			    	
				    
				} else {
					
			    	$erroMSG = str_replace(array(chr(13),chr(10)), ' ',utf8_decode($xml->status->error->message->text));
			    	$erroMSG = str_replace( "'", '"', $erroMSG );
					
					$htmllog .= "<tr>";
					$htmllog .= "<td><center><b>".$ptrcod."</td>";
					$htmllog .= "<td><center><b>-</b></center></td>";
					$htmllog .= "<td><center><b>-</b></center></td>";
					$htmllog .= "<td><center><b>-</b></center></td>";
					$htmllog .= "<td>".$erroMSG."</td>";
					$htmllog .= "</tr>";

			    }
			} catch (Exception $e){
				
				$erroMSG = str_replace(array(chr(13),chr(10)), ' ',$e->getMessage());
				$erroMSG = str_replace( "'", '"', $erroMSG );
				
				$htmllog .= "<tr>";
				$htmllog .= "<td colspan=4 class=SubTituloCentro>Erro no webservice com o FNDE!</td>";
				$htmllog .= "<td>".$erroMSG."</td>";
				$htmllog .= "</tr>";
				
			}
		} // fim foreach
		
		die($htmllog."</table>");
		
	}
	
	/**
     * Carrega as parcelas do pagamento
     * @name carregaParcelaPagamento
     * @author Wesley Romualdo
     * @access public
     * @param int $exfid - id da tabela
     * @return array
     */
	public function carregaParcelaPagamento( $exfid ){
		if( $exfid ){
			
			$sql = "SELECT  
					  	ord.orbvalorparcela as parcela,
					  	ord.orbanoparcela as anoparcela,
					  	ord.orbmesparcela as mesparcela,
					  	exf.exfnumempenhooriginal,
					  	ptr.ptrexercicio,
					  	ord.orbid,
					  	ord.orbnumsolicitacao
					FROM
						emenda.ordembancaria ord
					    inner join emenda.execucaofinanceira exf
					    	on exf.exfid = ord.exfid
						inner join emenda.planotrabalho ptr
					    	on ptr.ptrid = exf.ptrid
					WHERE
					  	ord.exfid = $exfid
					  	and exf.exfstatus = 'A'
					  	and ord.orbvalorparcela is not null
					ORDER BY
					    ord.orbanoparcela";

			$arDados = $this->carregar($sql);
			$arDados = ($arDados) ? $arDados : array();
			
			$arRetorno = array();
			if(!count($arDados)){
				$mensagem = 'Não foi encontrado nenhuma parcela para pagamento.';
				$arRetorno['boSucesso'] = 0;
				//$arRetorno['mensagem'] = simec_htmlentities( $mensagem );
				$arRetorno['mensagem']   = utf8_encode($mensagem);
				echo simec_json_encode( $arRetorno );
				die;
			}
			return $arDados;
		}
	}
	
	/**
     * Carrega os dados da ordem bancária
     * @name carregaOrdemBancariaPagamento
     * @author Wesley Romualdo
     * @access public
     * @param int $orbid - id da tabela
     * @return array
     */
	public function carregaOrdemBancariaPagamento( $orbid ){	
		$sql = "SELECT
				  	ord.orbid,
				  	ord.exfid,
				  	ord.orbmesparcela,
				  	ord.orbanoparcela,
				  	ord.orbvalorparcela,
				  	ord.orbnumsolicitacao,
				  	sip.spgdescricao,
				  	exf.exfnumempenhooriginal,
					ord.orbnumordembancaria,
				  	ord.orbvalorpagamento,
				  	to_char(ord.orbdataemissao, 'DD/MM/YYYY HH24:MI:SS') as orbdataemissao,
				  	to_char(ord.orbdatapagamento, 'DD/MM/YYYY HH24:MI:SS') as orbdatapagamento,
				  	usu.usunome as usucpf,
				  	to_char(ord.orbdatainclusao, 'DD/MM/YYYY HH24:MI:SS') as orbdatainclusao,
				  	to_char(ord.orbdataalteracao, 'DD/MM/YYYY HH24:MI:SS') as orbdataalteracao 
				FROM 
				  	emenda.ordembancaria ord
				    inner join emenda.situacaopagamento sip
				    	on sip.spgcodigo = ord.spgcodigo
				    inner join seguranca.usuario usu
	    				on usu.usucpf = ord.usucpf
	    			inner join emenda.execucaofinanceira exf
	    				on exf.exfid = ord.exfid
				WHERE
					ord.orbid = $orbid";
	
		return $this->pegaLinha( $sql );
	}
	
	public function carregaSituacaoHistorico( $orbid ){
		$sql = "SELECT 
					usu.usunome,
				  	sip.spgdescricao,
				  	orh.orbvalorpagamento,
				  	to_char(orh.obhdataalteracao, 'DD/MM/YYYY HH24:MI:SS') as data
				FROM 
				  	emenda.ordembanchistorico orh
				    inner join emenda.situacaopagamento sip
				    	on sip.spgcodigo = orh.spgcodigo
				    inner join seguranca.usuario usu
				    	on usu.usucpf = orh.usucpf
				WHERE
					orh.orbid = $orbid";
		
		$cabecalho = array( "Usuário", "Situação", "Valor (R$)", "Data Alteração" );
		return $this->monta_lista_simples( $sql, $cabecalho, 100, 30, 'N', '100%');
	}

	public function alteraEstado_wf( $ptrid ){
	
		$sql = "select docid from emenda.planotrabalho where ptrid = ".$ptrid;
		$docid = $this->pegaUm( $sql );
		
		$sql = "SELECT esdid FROM workflow.documento WHERE docid = $docid";								
		$esdid = $this->pegaUm( $sql );
		
		$sql = "select aedid from workflow.acaoestadodoc where esdidorigem = $esdid and esdiddestino = 58";
		$aedid = $this->pegaUm( $sql );
		if( $aedid && $docid ){
			/*echo "<script>
				wf_alterarEstado( $aedid, $docid, 58, 'Empenhado (Em Elaboração da Minuta do Convênio)')
			 </script>";*/
			$arDados = array( "url" => $_SESSION['favurl'],
				  			  "ptrid" => $ptrid );
			include_once APPRAIZ . 'includes/workflow.php';
			//wf_alterarEstado( $docid, $aedid, '', $arDados );
		}
	}
}
?>