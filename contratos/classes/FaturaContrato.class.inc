<?php
	
class FaturaContrato extends Modelo{
	
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "contratos.faturacontrato";	

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "ftcid" );

    /**
     * Atributos
     * @var array
     * @access protected
     */    
    protected $arAtributos     = array(
									  	'ftcid' => null, 
									  	'ctrid' => null, 
// 									  	'epsid' => null, 
									  	'docid' => null, 
									  	'usucpf' => null, 
									  	'ftcdatacriacao' => null, 
									  	'ftcdataalteracao' => null, 
									  	'ftcnumero' => null, 
									  	'ftcdescricao' => null, 
									  	'ftcdataemissao' => null, 
									  	'ftcvalor' => null, 
    									'ftcglosa' => null,
    									'ftcjustificativaglosa' => null,
    									'ftcjustificativaretencao' => null,
							    		'retir'=>null,
										'retcsll'=>null,
										'retcofins'=>null,
										'retpasep'=>null,
										'retoutro'=>null,
										'ftctotalapagar'=>null,
									  	'ftcstatus' => null, 
									  );
    
    public function listaSqlResumoPorCtrid( $ctrid, array $param = array() ){
    	$whereOr = array();
    	$where 	 = array();
    	$join    = array();
    	
        if($param['pesquisar']){
    		$whereOr[] = "fc.ftcnumero ilike ('%{$param['pesquisar']}%')";
    		$whereOr[] = "to_char(ftcdataemissao,'DD/MM/YYYY') ilike ('%{$param['pesquisar']}%')";
    		$whereOr[] = "to_char(ftcdataemissao,'DD-MM-YYYY') ilike ('%{$param['pesquisar']}%')";
    		$whereOr[] = "removeacento(ed.esddsc) ilike removeacento(('%{$param['pesquisar']}%'))";
    		$whereOr[] = "fc.ftcvalor::text ilike ('%".str_replace(array(".",","),array("",""),$param['pesquisar'])."%')";
    	}
    	
    	if ( $param['esdid'] ){
    		$param['esdid']   		 = (array) $param['esdid'];
    		$where[] 		  		 = "ed.esdid IN (" . implode(", ", $param['esdid']) . ")";
//     		$join['documento'] 		 = "JOIN workflow.documento d ON d.docid = fc.docid";
//     		$join['estadodocumento'] = "JOIN workflow.estadodocumento ed ON ed.esdid = d.esdid";
    	}
    	
    	$sql = "SELECT
    				'<span onmouseover=\"return escape(\'Visualizar resumo da nota fiscal: ' || fc.ftcnumero || '\');\">
	    				 <img 
	    					border=\"0\" 
	    					src=\"/imagens/icone_lupa.png\" 
	    					onclick=\"location.href = \'?modulo=principal/popUpFatura&acao=A&requisicao=popUpFatura&ftcid=' || fc.ftcid || '\';\"
	    					style=\"cursor:pointer;\">
    				 </span>' AS acao,
    				ftcnumero,
    				TO_CHAR(ftcdataemissao, 'dd-mm-YYYY') AS ftcdataemissaoformatada,
    				ftcvalor,
    				esddsc
    			FROM
    				contratos.faturacontrato fc
    			JOIN workflow.documento d ON d.docid = fc.docid
    			JOIN workflow.estadodocumento ed ON ed.esdid = d.esdid
    			
    			" . (count($join) ? implode(" ", $join) : "") . "
    			
    			WHERE
    				ftcstatus = 'A' AND
    				ctrid = {$ctrid}

    				" . (count($where) ? " AND " . implode(' AND ',$where) : "") . "
    				" . (count($whereOr) ? " AND (" . implode(' OR ',$whereOr).")" : "") . "
    			
    			ORDER BY
    				ftcdataemissao";
    	
    	return $sql;
    	
//     	$dados = $this->carregar($sql);
    	
//     	return ($dados ? $dados : array());
    }
    
    public function listaDadosPorCtrid( $ctrid, array $param = array() ){
    	$where = array();
    	$join  = array();
    	
    	if ( $param['esdid'] ){
    		$param['esdid']   		 = (array) $param['esdid'];
    		$where[] 		  		 = "ed.esdid IN (" . implode(", ", $param['esdid']) . ")";
    		$join['documento'] 		 = "JOIN workflow.documento d ON d.docid = fc.docid";
    		$join['estadodocumento'] = "JOIN workflow.estadodocumento ed ON ed.esdid = d.esdid";
    	}
    	
    	$sql = "SELECT
    				*
    			FROM
    				contratos.faturacontrato fc
    				" . (count($join) ? implode(" ", $join) : "") . "
    			WHERE
    				ftcstatus = 'A' AND
    				ctrid = {$ctrid}
    				" . (count($where) ? " AND " . implode(' AND ',$where) : "") . "
    			ORDER BY
    				ftcdataemissao";
    	
    	$dados = $this->carregar($sql);
    	
    	return ($dados ? $dados : array());
    }
    
    public function pegaValorTotalPorCtrid( $ctrid, array $param = array() ){
		$where = array();
		$join  = array();

		if ( $param['esdid'] ){
			$param['esdid']   		 = (array) $param['esdid'];
			$where[] 		  		 = "ed.esdid IN (" . implode(", ", $param['esdid']) . ")";
			$join['documento'] 		 = "JOIN workflow.documento d ON d.docid = f.docid";
			$join['estadodocumento'] = "JOIN workflow.estadodocumento ed ON ed.esdid = d.esdid";
		}
		
    	$sql = "SELECT
    				SUM(ftcvalor) AS ftcvalor
    			FROM
    				contratos.faturacontrato f
    				" . (count($join) ? implode(" ", $join) : "") . "
    			WHERE
    				ftcstatus = 'A' AND
    				ctrid = {$ctrid}
    				" . (count($where) ? " AND " . implode(' AND ',$where) : "");
    	
    	$totalValor = $this->pegaUm($sql);
    	
    	return ($totalValor ? $totalValor : 0);
    }
    
    public function listaFaturamento($ctrid)
    {
    	if($_POST['pesquisar'])
    	{
    		$arrWhere[] = "ftc.ftcnumero ilike ('%{$_POST['pesquisar']}%')";
    		$arrWhere[] = "to_char(ftcdataemissao,'DD/MM/YYYY') ilike ('%{$_POST['pesquisar']}%')";
    		$arrWhere[] = "numempenho ilike ('%{$_POST['pesquisar']}%')";
    		$arrWhere[] = "removeacento(esd.esddsc) ilike removeacento(('%{$_POST['pesquisar']}%'))";
    		$arrWhere[] = "removeacento(ftc.ftcdescricao) ilike removeacento(('%{$_POST['pesquisar']}%'))";
    		$arrWhere[] = "ftc.ftcvalor::text ilike ('%".str_replace(array(".",","),array("",""),$_POST['pesquisar'])."%')";
    		$arrWhere[] = "removeacento(esd.esddsc) ilike removeacento(('%{$_POST['pesquisar']}%'))";
    		$arrWhere[] = "removeacento(usu.usunome) ilike removeacento(('%{$_POST['pesquisar']}%'))";
    	}
    	
    	$desabilitado   = false;
    	$somenteLeitura = 'S';
    	 
    	$perfis = arrayPerfil();
    	
    	if(in_array(PERFIL_CONSULTA_UNIDADE, $perfis ) || 
    	   in_array(PERFIL_CONSULTA_GERAL, $perfis ) ||
    	   in_array(PERFIL_EQUIPE_TECNICA_UNIDADE, $perfis)) {
    		$desabilitado = true;
    		$somenteLeitura = 'N';
    	}
    	
    	$sql = "select
    				case when esd.esdid = ".ESTADO_WK_FATURAMENTO_EM_CADASTRAMENTO." AND 'S' = '". $somenteLeitura ."' 
    					then '<img  src=\"../imagens/alterar.gif\" class=\"link\" align=absmiddle onclick=\"editarFaturamento(' || ftc.ftcid || ')\"  /> <img  src=\"../imagens/excluir.gif\" class=\"link\" align=absmiddle onclick=\"excluirFaturamento(' || ftc.ftcid || ')\"  /> <img style=\"cursor: pointer;\" align=absmiddle src=\"../imagens/fluxodoc.gif\" onclick=\"wf_exibirHistorico( \''|| doc.docid ||'\' );\">'
    					else  '<img  src=\"../imagens/consultar.gif\" class=\"link\" align=absmiddle onclick=\"editarFaturamento(' || ftc.ftcid || ')\"  /> <img style=\"cursor: pointer;\" align=absmiddle src=\"../imagens/fluxodoc.gif\" onclick=\"wf_exibirHistorico( \''|| doc.docid ||'\' );\">'
    				end as acao,
    				--nu_empenho,
    				array_to_string( array( SELECT TRIM(obfnumero) from contratos.ordembancariafatura obf WHERE obf.ftcid = ftc.ftcid ), '<br>' ),		
    				ftc.ftcnumero || ' ' AS ftcnumero,
    				to_char(ftcdataemissao,'DD/MM/YYYY') as data,
    				ftc.ftcvalor,
    				usu.usunome,
    				esd.esddsc as esddsc
    			from
    				contratos.faturacontrato ftc
    			--inner join
    			--	contratos.empenho_siafi es ON es.epsid = ftc.epsid
    			inner join
    				seguranca.usuario usu ON usu.usucpf = ftc.usucpf
    			inner join
    				workflow.documento doc ON doc.docid = ftc.docid
    			inner join
    				workflow.estadodocumento esd ON esd.esdid = doc.esdid
    			where
    				ftc.ftcstatus = 'A'
    			".($arrWhere ? " and (".implode(" or ",$arrWhere).")" : "")."
    			and
    				ftc.ctrid = $ctrid
    			order by
    				ftcdataemissao";
    	
    	$arrCab = array("Ação"/*,"Número do Empenho"*/, 'Número OB',"Número da Fatura","Data de Emissão","Valor","Responsável","Situação");
    	$this->monta_lista($sql,$arrCab,30,10,"S","");
    }
    
    public function salvarNotaFiscal()
    {
    	if(!$this->validasalvarNotaFiscal()){
    		return false;
    	}
     	$arCamposNulo=array();
    	$_POST['usucpf'] 			= $_SESSION['usucpf'];
    	$_POST['ctrid'] 			= $_SESSION['ctrid'];
    	$_POST['ftcvalor'] 			= str_replace(array(".",","),array("","."),$_POST['ftcvalor']);
    	$_POST['ftcdataemissao'] 	= formata_data_sql($_POST['ftcdataemissao']);
    	$_POST['ftcdataalteracao'] 	= date("Y-m-d H:i:s");
    	$_POST['docid'] 			= ($_POST['docid'] ? $_POST['docid'] : $this->criaWorkFlowNotaFiscal());
    	if($_POST['ftcglosa']!='')
     		$_POST['ftcglosa']= str_replace(array(".",","),array("","."),$_POST['ftcglosa']);
     	else{
     		$_POST['ftcglosa']= NULL;
     		array_push($arCamposNulo, 'ftcglosa');
     	}
     	if($_POST['ftcaliqoutro']!='')
     		$_POST['ftcaliqoutro']= str_replace(array(".",","),array("","."),$_POST['ftcaliqoutro']);
     	else{
     		$_POST['ftcaliqoutro']= NULL;
     		array_push($arCamposNulo, 'ftcaliqoutro');
     	}  

     	if($_POST['retir']!='')
     		$_POST['retir']= str_replace(array(".",","),array("","."),$_POST['retir']);
     	else{
     		$_POST['retir']= NULL;
     		array_push($arCamposNulo, 'retir');
     	}
     	if($_POST['retcsll']!='')
     		$_POST['retcsll']= str_replace(array(".",","),array("","."),$_POST['retcsll']);
     	else{
     		$_POST['retcsll']= NULL;
     		array_push($arCamposNulo, 'retcsll');
     	}
     	if($_POST['retcofins']!='')
     		$_POST['retcofins']= str_replace(array(".",","),array("","."),$_POST['retcofins']);
     	else{
     		$_POST['retcofins']= NULL;
     		array_push($arCamposNulo, 'retcofins');
     	}
     	if($_POST['retpasep']!='')
     		$_POST['retpasep']= str_replace(array(".",","),array("","."),$_POST['retpasep']);
     	else{
     		$_POST['retpasep']= NULL;
     		array_push($arCamposNulo, 'retpasep');
     	}
     	if($_POST['retoutro']!='')
     		$_POST['retoutro']= str_replace(array(".",","),array("","."),$_POST['retoutro']);
     	else{
     		$_POST['retoutro']= NULL;
     		array_push($arCamposNulo, 'retoutro');
     	}    	
     	if($_POST['ftctotalapagar']!='')
     		$_POST['ftctotalapagar']= str_replace(array(".",","),array("","."),$_POST['ftctotalapagar']);
     	else{
     		$_POST['ftctotalapagar']= NULL;
     		array_push($arCamposNulo, 'ftctotalapagar');
     	}    	
     	//dbg($_POST,d);
     	
     	
    	$this->popularDadosObjeto($_POST);
    	$ftcid = $this->salvar(true,true,$arCamposNulo);
    	
    	if($ftcid){
    		$anexo = new AnexoFatura();
    		$anexo->salvarAnexo($ftcid);
    		
    		$ob = new OrdemBancariaFatura();
    		$ob->salvarOrdemBancaria($ftcid);
    		$_SESSION['administrativo']['contratos']['notafiscal']['alert'] = 'Operação realizada com sucesso.';
    		header("Location: contratos.php?modulo=principal/addNotaContratos&acao=A&ftcid=$ftcid");
    		//$_SESSION['administrativo']['contratos']['notafiscal']['ftcid'] = $ftcid;
    	}else{
    		$_SESSION['administrativo']['contratos']['notafiscal']['alert'] = 'Não foi possível realizar a operação.';
    		$_SESSION['administrativo']['contratos']['notafiscal']['errosalvar'] = 1;
	    	header("Location: contratos.php?modulo=principal/addNotaContratos&acao=A");
    	}
    	exit;
    }
    
    private function validasalvarNotaFiscal()
    {
    	extract($_POST);
//     	if(!$epsid){
//     		$_SESSION['administrativo']['contratos']['notafiscal']['errosalvar'] = 1;
//     		$_SESSION['administrativo']['contratos']['notafiscal']['alert'] = "Favor informar o Número de Empenho.";
//     		return false;
//     	}
    	if(!$ftcnumero){
    		$_SESSION['administrativo']['contratos']['notafiscal']['errosalvar'] = 1;
    		$_SESSION['administrativo']['contratos']['notafiscal']['alert'] = "Favor informar o Número de Fatura.";
    		return false;
    	}
    	if(!$ftcdataemissao){
    		$_SESSION['administrativo']['contratos']['notafiscal']['errosalvar'] = 1;
    		$_SESSION['administrativo']['contratos']['notafiscal']['alert'] = "Favor informar a Data de Emissão.";
    		return false;
    	}
    	if(!$ftcvalor){
    		$_SESSION['administrativo']['contratos']['notafiscal']['errosalvar'] = 1;
    		$_SESSION['administrativo']['contratos']['notafiscal']['alert'] = "Favor informar o Valor.";
    		return false;
    	}
    	return true;
    }
    
    private function criaWorkFlowNotaFiscal()
    {
    	require_once APPRAIZ . 'includes/workflow.php';
    	$docid = wf_cadastrarDocumento( TPDID_FATURAMENTO, "Faturamento de Contrato" );
    	return $docid;
    }
    
    public function excluirFaturamento()
    {
    	$ftcid = $_REQUEST['ftcid'];
    	if($ftcid){
    		$sql = "update contratos.faturacontrato set ftcstatus = 'I' where ftcid = $ftcid;";
    		if($this->executar($sql)){
    			$this->commit();
    			$_SESSION['administrativo']['contratos']['notafiscal']['alert'] = 'Operação realizada com sucesso.';
    		}else{
    			$_SESSION['administrativo']['contratos']['notafiscal']['alert'] = 'Não foi possível realizar a operação.';
    		}
    	}else{
    		$_SESSION['administrativo']['contratos']['notafiscal']['alert'] = 'Não foi possível realizar a operação.';
    	}
    	header("Location: ?modulo=principal/execucaoFinanceiraContratos&acao=A");
    	exit;
    }
    
    function listaExecucaoFinanceira($ctrid)
    {
    	
    	$sql = "SELECT
			    	docid,
			    	ftc.ftcid,
			    	to_char(ftcdataemissao,'DD/MM/YYYY') as ftcdataemissao,
			    	numempenho,
			    	case when length(obf.obfnumero) = 23 then substr(obf.obfnumero, 12, 12) else obf.obfnumero end as obfnumero,
			    	ftcnumero,
			    	obfvalor,
			    	hspcnpj as cnpj,
			    	CASE WHEN ctrvlrtotal IS NOT NULL THEN 
			    			ctrvlrtotal
			    		ELSE 
							(ct.ctrvlrinicial)+(SELECT
											COALESCE(SUM(tdavlr),0) as total
										FROM
											contratos.cttermoaditivo adi
										WHERE
										adi.tdastatus = 'A' AND	adi.ctrid =ct.ctrid)
			    	END AS valorcontrato
			    FROM
			    	contratos.faturacontrato ftc
			    LEFT JOIN
			    	contratos.ordembancariafatura obf ON obf.ftcid = ftc.ftcid
			    LEFT JOIN
			    	contratos.ctcontrato ctr ON ctr.ctrid = ftc.ctrid
			    LEFT JOIN
			    	contratos.hospital h ON h.hspid = ctr.hspid
			    WHERE
			    	ftc.ftcstatus = 'A' AND
			    	ftc.ctrid = $ctrid
			    ORDER BY
			    	ftcdataemissao";
    	
    	/*
    	$sql = "select
    				docid,
    				ftc.ftcid,
    				to_char(ftcdataemissao,'DD/MM/YYYY') as ftcdataemissao,
    				numempenho,
    				case when length(obf.obfnumero) = 23 then substr(obf.obfnumero, 12, 12) else obf.obfnumero end as obfnumero,
    				ftcnumero,
    				obfvalor,
    				--ungcnpj as cnpj,
    				entnumcpfcnpj as cnpj,
    				case when ctrvlrtotal is not null
    					then ctrvlrtotal
    					else ctrvlrinicial
    				end as valorcontrato
    			from
    				contratos.faturacontrato ftc
    			LEFT JOIN
    				contratos.ordembancariafatura obf ON obf.ftcid = ftc.ftcid
    			LEFT JOIN
    				contratos.ctcontrato ctr ON ctr.ctrid = ftc.ctrid
    			--LEFT JOIN
    			--	contratos.contratante ung ON ctr.ungid = ung.ungid
    			LEFT JOIN
					entidade.entidade ent ON ent.entid = ctr.entidcontratada
    			where
    				ftc.ftcstatus = 'A'
    			and
    				ftc.ctrid = $ctrid
    			order by
    				ftcdataemissao";
    	*/
    	return $this->carregar($sql);
	}
	
	function listaExecucaoFinanceira2($ctrid, array $param = array())
	{
		$where 		= array();
		$whereOr 	= array();
		$join  		= array();
		
		if($param['pesquisar'])
		{
			$whereOr[] = "ftc.ftcnumero ilike ('%{$param['pesquisar']}%')";
			$whereOr[] = "to_char(ftcdataemissao,'DD/MM/YYYY') ilike ('%{$param['pesquisar']}%')";
// 			$whereOr[] = "numempenho ilike ('%{$param['pesquisar']}%')";
			$whereOr[] = "removeacento(esd.esddsc) ilike removeacento(('%{$param['pesquisar']}%'))";
			$whereOr[] = "removeacento(ftc.ftcdescricao) ilike removeacento(('%{$param['pesquisar']}%'))";
			$whereOr[] = "ftc.ftcvalor::text ilike ('%".str_replace(array(".",","),array("",""),$param['pesquisar'])."%')";
			$whereOr[] = "removeacento(esd.esddsc) ilike removeacento(('%{$param['pesquisar']}%'))";
// 			$whereOr[] = "removeacento(usu.usunome) ilike removeacento(('%{$param['pesquisar']}%'))";
		}
		
		$sql = "(	SELECT
						NULL::int AS docid,
						NULL::int AS esdid,
						NULL::varchar AS esddsc,
						NULL::int AS ftcid,
						NULL::date AS ftcdataemissao,
						NULL::varchar AS ftcnumero,
						NULL::float AS ftcvalor,
						NULL::float AS ftcglosa,
						--NULL::varchar AS usunome,
						it_co_credor AS cnpj,
						SUM(obs.valor) AS obfvalortotal,
						CASE 
							WHEN ctrvlrtotal IS NOT NULL THEN ctrvlrtotal
							ELSE (ctrvlrinicial) + (SELECT 
														coalesce(SUM(tdavlr),0) AS total
													FROM 
														contratos.cttermoaditivo adi
													WHERE 
														adi.tdastatus = 'A' AND 
														adi.ctrid = c.ctrid)
						END AS valorcontrato
					FROM
						contratos.ctcontrato c
					JOIN entidade.entidade e ON e.entid = c.entidcontratada	
					JOIN contratos.hospital h ON h.hspid = c.hspid AND
								     			 h.hspstatus = 'A'
					JOIN contratos.hospitalug hu ON hu.hspid = h.hspid 
					JOIN contratos.empenhovinculocontrato ec ON ec.ctrid = c.ctrid			     
					JOIN contratos.empenho_siafi es ON es.epsid = ec.epsid AND
									   				   TRIM(es.ungcod) = TRIM(hu.ungcod) AND
									   				   TRIM(es.co_favorecido) = TRIM(e.entnumcpfcnpj)
					JOIN contratos.ob_siafi obs ON TRIM(obs.empenho) = TRIM(es.nu_empenho) AND
								       			   TRIM(obs.unidade) = TRIM(hu.ungcod) AND
								       			   TRIM(obs.it_co_credor) = TRIM(e.entnumcpfcnpj)
					WHERE
						c.ctrid = $ctrid AND
						obs.ob || ec.epsid NOT IN (
													SELECT 
														obf.obfnumero || obf.epsid
													FROM 
														contratos.faturacontrato fc
													JOIN contratos.ordembancariafatura obf ON obf.ftcid = fc.ftcid
													WHERE 
														fc.ftcstatus = 'A' AND
														fc.ctrid = {$_SESSION['ctrid']}
													)
					GROUP BY
						docid,
						ftcid,
						ftcdataemissao,
						ftcnumero,
						ftcvalor,
						cnpj,
						valorcontrato
				)UNION ALL(
					SELECT
						d.docid,
						esd.esdid,
						esd.esddsc,
						ftc.ftcid,
						ftcdataemissao,
						ftcnumero,
						ftcvalor,
						ftcglosa,
						--usu.usunome,
						entnumcpfcnpj as cnpj,
						obf.obfvalortotal,
						CASE 
							WHEN ctrvlrtotal IS NOT NULL THEN ctrvlrtotal
							ELSE (ctrvlrinicial) + (select 
														coalesce(sum(tdavlr),0) as total
													from 
														contratos.cttermoaditivo adi
													where 
														adi.tdastatus = 'A' and 
														adi.ctrid = ctr.ctrid)
						END AS valorcontrato
					FROM
						contratos.faturacontrato ftc
						LEFT JOIN contratos.empenho_siafi eps ON eps.epsid = ftc.epsid
						LEFT JOIN ( SELECT 
										ftcid,
										SUM(obfvalor) AS obfvalortotal
								    FROM
										contratos.ordembancariafatura
								    GROUP BY
										ftcid) obf ON obf.ftcid = ftc.ftcid
						LEFT JOIN contratos.ctcontrato ctr ON ctr.ctrid = ftc.ctrid
						LEFT JOIN entidade.entidade ent ON ent.entid = ctr.entidcontratada
						
						LEFT JOIN workflow.documento d ON d.docid = ftc.docid
						LEFT JOIN workflow.estadodocumento esd ON esd.esdid = d.esdid
						
						--LEFT JOIN seguranca.usuario usu ON usu.usucpf = ftc.usucpf
					WHERE
						ftc.ftcstatus = 'A' and
						ftc.ctrid = {$ctrid}
						" . ( count($where) ? " AND " . implode(" AND ", $where) : "" ) . "
						" . ( count($whereOr) ? " AND (" . implode(" OR ", $whereOr).")" : "" ) . "
					ORDER BY
						ftcdataemissao, ftcnumero
			)";
		
// 		dbg($sql, d);
// 		$sql = "SELECT obs.it_co_credor, obs.it_no_credor, ob,
// 					TO_CHAR(obs.datatransacao, 'DD/MM/YYYY') AS datatransacao,
// 					TO_CHAR(obs.datatransacao, 'YYYY/MM/DD') AS datatransacao_banco,
// 					--			   TO_CHAR(obs.datatransacao, 'DD/MM/YYYY') || ' ' || TO_CHAR(obs.datatransacao, 'HH24:MI') AS datatransacao,
// 					obsob,
// 					unidade,
// 					obs.natureza,
// 					it_co_credor ||' - '|| it_no_credor as favorecido,
// 					obs.valor as valorob
// 				FROM
// 					contratos.ob_siafi obs
// 					JOIN contratos.empenho_siafi eps ON TRIM(eps.nu_empenho) = TRIM(obs.empenho)
// 					WHERE
// 					{$whereUnidade}
// 					AND TRIM(it_co_credor) = TRIM('{$_GET['cnpj']}')
// 					AND eps.epsid = '{$_GET['empenho']}'
// 					AND obs.ob NOT IN ( SELECT
// 											obf.obfnumero
// 										FROM
// 											contratos.faturacontrato fc
// 											JOIN contratos.ordembancariafatura obf ON obf.ftcid = fc.ftcid
// 										WHERE
// 											fc.ftcstatus = 'A' AND
// 											" . ($_GET['ftcid'] ? " fc.ftcid != {$_GET['ftcid']} AND " : "") . "
// 											fc.ctrid = {$_SESSION['ctrid']}
// 										)";
		
		return $this->carregar($sql);
	}
    
    public function listaOrdemBancaria()
    {
	    $epcid = $_POST['epcid'];
	    $sql = "select
	    	'<img src=\"../imagens/mais.gif\" class=\"link\" onclick=\"abreNotaFiscal(this,' || obf.obfid || ')\"   /> ' || obf.obfnumero as acao,
	    	to_char(obfdata,'DD/MM/YYYY') as data,
	    	obf.obfvalor,
	    	count(distinct obf.ftcid) as qtde_notas
	    from
	    	contratos.ordembancariafatura obf
	    inner join
	    	contratos.faturacontrato ftc ON obf.ftcid = ftc.ftcid and ftc.ftcstatus = 'A'
	    where
	    	ftc.ctrid = {$_SESSION['ctrid']}
	    and
	    	ftc.epcid = $epcid
	    group by
	    	obf.obfnumero,obf.obfdata,obf.obfvalor,obf.obfid
	    order by
	    	obf.obfnumero,obf.obfvalor";
	    $arrCab = array("Ordem Bancária","Data","Valor","Qtde. Pagamentos");
		$this->monta_lista_simples($sql,$arrCab,10000,10000,"S","100%");
    }
    
    public function listaNotaFiscal()
    {
    	$obfid = $_POST['obfid'];
    	$sql = "select distinct
    				ftc.ftcnumero,
    				to_char(ftc.ftcdataemissao,'DD/MM/YYYY') as data,
    				ftc.ftcdescricao,
    				ftc.ftcvalor
    			from
    				contratos.faturacontrato ftc
    			inner join
    				contratos.ordembancariafatura obf ON obf.ftcid = ftc.ftcid
    			where
    				ftc.ftcstatus = 'A'
				and
    	    		ftc.ctrid = {$_SESSION['ctrid']}
    	    	and
    	    		obf.obfid = $obfid
    			order by
    				ftc.ftcnumero,data";
    	$arrCab = array("Número","Data","Descrição","Valor");
		$this->monta_lista_simples($sql,$arrCab,10000,10000,"S","100%");
    }

}