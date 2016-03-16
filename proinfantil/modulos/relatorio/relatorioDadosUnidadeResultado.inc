<?
// Codificação do relatório
header( 'Content-Type: text/html; charset=iso-8859-1' );
// Objeto da classe do relatório dos dados da unidade
$obRelatorio = new relatorioDadosUnidadeResultado();
// Verifica se é um relatório p o browser ou excel
$isXls = ($_REQUEST['requisicao'] == 'relatorio_xls') ? true :false;

// Classe responsável por montar o resultado do relatorio
class relatorioDadosUnidadeResultado
{
	
	// Variáveis da classe
	public $esfera 			= '';
	public $arrTipoEntidade = array();
	public $requisicao 		= '';
	public $arrColunas 		= array();
	public $arrUfs 			= array();
	public $arrMunicipios 	= array();
	public $dados			= array();
	public $db				= '';
	
	// Método construtor
	function __construct()
	{
		// Instancia a classe do BD
		$this->db = new cls_banco();
		
		set_time_limit(0);
		// Seta a esfera escolhida	
		$this->esfera= $_POST['esfera'];
		// Caso estado
		if( $this->esfera == 1)
		{
			$this->arrTipoEntidade = $_POST['tipo_entidade_estado'];
		}
		else 
		{
			$this->arrTipoEntidade = $this->_POST['tipo_entidade_municipio'];
		}
		// Verifica a requisição (relatorio ou relatorio_xls)
	    $this->requisicao = $_POST['requisicao']; // 
	    // Verifica as colunas escolhidas para o relatório
		$this->arrColunas = $this->_POST['coluna'];
	
	}
	
	/**
	 * Monta a string apartir do array e retorna no formato do sql
	 * */
	public function getArrToStrParam( $arrParam )
	{
		if( $arrParam[0] != '')
		{
			foreach( $arrParam as $k => $v )
			{
				$arrParam[$k] = "'{$v}'";
			}
			
			$strReturn = implode(",", $arrParam);
		}
		else
		{
			$strReturn = false;
		}    	
		return $strReturn;		
    }
    
	function monta_sql()
	{
		
		// Primeiro verifica se é estadual ou municipal
		$esfera = $_POST['esfera'];
		// Carrega as colunas
		$arrColunas = $_POST['coluna'];
		// Recupera array com as ufs
		$arrUf= $_POST['uf'];
		// Monta o parametro para a clausula where
		$strUf = $this->getArrToStrParam($arrUf);
		// Recupera array com os município
    	$arrMunCod = $_POST['muncod'];
    	// Monta o parametro para a clausula where 		
    	$strMunCod= $this->getArrToStrParam( $arrMunCod );
    	
		// Caso a esfera seja Estadual
		if( $esfera == 1 )
		{
			$sql = "";
			// Verifica tipo de entidade
			$arrTipoEntidade = $_POST['tipo_entidade_estado'];
			
			// Cria um array para cada entidade para guardar as colunas
			$arrayColsSEE = array();
			$arrayColsSECEE = array();
			$arrayColsELE = array();
			$arrayColsCLE = array();
			
			// Verifica os tipos de entidade selecionadas
			$isSEE		= ( in_array('SEE',$arrTipoEntidade )) ? true : false ;
			$isSECEE	= ( in_array('SECEE',$arrTipoEntidade )) ? true : false ; 
			$isELE		= ( in_array('ELE',$arrTipoEntidade )) ? true : false ;
			$isCLE		= ( in_array('CLE',$arrTipoEntidade )) ? true : false ;
			
			$isSecretariaSecretario = false;
			
			if($isSEE || $isSECEE )
			{
				$isSecretariaSecretario = true;
			}
			
			if(is_array($arrColunas) && ( count($arrColunas) > 0 ))
			{
				foreach( $arrColunas as $k => $col )
				{
					//SECRETARIA ESTADUAL DE EDUCAÇÃO
					if($isSEE)
					{
						switch ($col) {
						    case 'CNPJ_SEE':
						        $arrayColsSEE[] = array(
						        	'coluna'	 => 'ent2.entnumcpfcnpj', 
						        	'nomeColuna' => '"CNPJ da Secretaria Estadual de Educação"'
						        );
						        break;
						    case 'NOME_SEE':
						    	$arrayColsSEE[] = array(
						        	'coluna'	 => 'ent2.entnome', 
						        	'nomeColuna' => '"Nome da Secretaria Estadual de Educação"'
						        );
						        break;
							case 'RSOC_SEE':
								$arrayColsSEE[] = array(
						        	'coluna'	 => 'ent2.entrazaosocial', 
						        	'nomeColuna' => '"Razão social da Secretaria Estadual de Educação"'
						        );
							break;
							case 'EMAIL_SEE':
								$arrayColsSEE[] = array(
						        	'coluna'	 => 'ent2.entemail', 
						        	'nomeColuna' => '"Email da Secretaria Estadual de Educação"'
						        );
							break;
							case 'TEL_SEE':
								$arrayColsSEE[] = array(
						        	'coluna'	 => "'('||ent2.entnumdddcomercial|| ') ' ||ent2.entnumcomercial", 
						        	'nomeColuna' => '"Telefone da Secretaria Estadual de Educação"'
						        );
							break;
							case 'CEP_SEE' :
								$arrayColsSEE[] = array(
						        	'coluna'	 => 'ent2.endcep', 
						        	'nomeColuna' => '"CEP da Secretaria Estadual de Educação"'
						        );
								//
							break;
							case 'END_SEE' :
								$arrayColsSEE[] = array(
						        	'coluna'	 => "ent2.endlog 
														||
														CASE WHEN ( ent2.endcom  <> '')
														
															THEN
																 ' - Complemento: ' || ent2.endcom 
															ELSE
																'' 
														END
														
														||
														
														CASE WHEN ( ent2.endnum  <> '')
														
															THEN
																  ' - Número : ' || ent2.endnum
															ELSE
																'' 
														END
														||
														CASE WHEN ( ent2.endbai  <> '')
														
															THEN
																  ' - Bairro: ' || ent.endbai
															ELSE
																'' 
														END
														 ",
						        	'nomeColuna' => '"Endereço da Secretaria Estadual de Educação"'
						        );
								//endlog endcom  endbai
							break;
						}
					}
					// Secretário(a) Estadual de Educação
					if( $isSECEE )
					{
						switch ($col) {
						    case 'CPF_SECEE':
						        $arrayColsSECEE[] = array(
						        	'coluna'	 => 'ent.entnumcpfcnpj', 
						        	'nomeColuna' => '"CPF Secretário(a)"'
						        );
						        break;
						    case 'NOME_SECEE':
						    	$arrayColsSECEE[] = array(
						        	'coluna'	 => 'ent.entnome', 
						        	'nomeColuna' => '"Nome Secretário(a)"'
						        );
						        break;
						    case 'EMAIL_SECEE':
						    	$arrayColsSECEE[] = array(
						        	'coluna'	 => 'ent.entemail', 
						        	'nomeColuna' => '"Email Secretário(a)"'
						        );
						        break;
						    case 'RG_SECEE':
						    	$arrayColsSECEE[] = array(
						        	'coluna'	 => 'ent.entnumrg', 
						        	'nomeColuna' => '"Registro Geral (RG) Secretário(a)"'
						        );
						        break;
							         
						    case 'OE_SECEE':
						    	$arrayColsSECEE[] = array(
						        	'coluna'	 => 'ent.entorgaoexpedidor', 
						        	'nomeColuna' => '"Orgão Expedidor Secretário(a)"'
						        );
						        break;
						    case 'SEX_SECEE':
						    	$arrayColsSECEE[] = array(
						        	'coluna'	 => 'ent.entsexo', 
						        	'nomeColuna' => '"Sexo Secretário(a)"'
						        );
						        break;
						    case 'DTN_SECEE':
						    	$arrayColsSECEE[] = array(
						        	'coluna'	 => 'ent.entdatanasc', 
						        	'nomeColuna' => '"Data de Nascimento Secretário(a)"'
						        );
						        break;
						    case 'TEL_SECEE':
						    	$arrayColsSECEE[] = array(
						        	'coluna'	 => "'('||ent.entnumdddcomercial|| ') ' ||ent.entnumcomercial", 
						        	'nomeColuna' => '"Telefone Secretário(a)"'
						        );
						        break;
						    case 'CEP_SECEE':
						    	$arrayColsSECEE[] = array(
						        	'coluna'	 => 'ent.endcep', 
						        	'nomeColuna' => '"CEP Secretário(a)"'
						        );
						        break;
						    case 'END_SECEE':
						    	$arrayColsSECEE[] = array(
						        	'coluna'	 =>" ent.endlog 
														||
														CASE WHEN ( ent.endcom  <> '')
														
															THEN
																 ' - Complemento: ' || ent.endcom 
															ELSE
																'' 
														END
														
														||
														
														CASE WHEN ( ent.endnum  <> '')
														
															THEN
																  ' - Número : ' || ent.endnum
															ELSE
																'' 
														END
														||
														CASE WHEN ( ent.endbai  <> '')
														
															THEN
																  ' - Bairro: ' || ent.endbai
															ELSE
																'' 
														END
														",
						        	'nomeColuna' => '"Endereco Secretário(a)"'
						        );
						        break;
						}
					}
					if($isELE)
					{
						
				    	switch ($col)
				    	{
						    case 'CPF_ELE':
						        $arrayColsELE[] = array(
						        	'coluna'	 => 'du.duncpf', 
						        	'nomeColuna' => '"CPF Equipe Local"'
						        );
						    break;
						    case 'NOM_ELE':
						    	$arrayColsELE[] = array(
						        	'coluna'	 => 'du.dunnome', 
						        	'nomeColuna' => '"Nome Equipe Local"'
						        );
							break;
						    case 'FUNC_ELE':
						    	$arrayColsELE[] = array(
						        	'coluna'	 => 'du.dunfuncao', 
						        	'nomeColuna' => '"Função/Cargo Equipe Local"'
						        );
							break;
						    case 'SEG_ELE':
						    	$arrayColsELE[] = array(
						        	'coluna'	 => 'sgm.sgmdsc',
						        	'nomeColuna' => '"Segmento Equipe Local"'
						        );
							break;
						    case 'EMAIL_ELE':
						    	$arrayColsELE[] = array(
						        	'coluna'	 => 'du.dunemail', 
						        	'nomeColuna' => '"E-mail Equipe Local"'
						        );
							break;
				    	}
					}
					if($isCLE)
					{
						
						switch ($col)
				    	{
						    case 'CPF_CLE':
						        $arrayColsCLE[] = array(
						        	'coluna'	 => 'du.duncpf', 
						        	'nomeColuna' => '"CPF Comitê Local"'
						        );
						    break;
						    case 'NOME_CLE':
						        $arrayColsCLE[] = array(
						        	'coluna'	 => 'du.dunnome', 
						        	'nomeColuna' => '"Nome Comitê Local"'
						        );
						    break;
						    case 'FUNC_CLE':
						        $arrayColsCLE[] = array(
						        	'coluna'	 => 'du.dunfuncao', 
						        	'nomeColuna' => '"Função/Cargo Comitê Local"'
						        );
						    break;
						    case 'SEG_CLE':
						        $arrayColsCLE[] = array(
						        	'coluna'	 => 'sgm.sgmdsc', 
						        	'nomeColuna' => '"Segmento Comitê Local"'
						        );
						    break;
						    case 'EMAIL_CLE':
						        $arrayColsCLE[] = array(
						        	'coluna'	 => 'du.dunemail', 
						        	'nomeColuna' => '"E-mail Comitê Local"'
						        );
						    break;
				    	}	
					}
					
				}
			}

			if( $isSecretariaSecretario )
			{
				// Secretário(a) Estadual de Educação $arrayColsSECEE
				// SECRETARIA ESTADUAL DE EDUCAÇÃO $arrayColsSEE
				
				$sql = " SELECT
							est.estuf as  \"UF\",
							'Estadual' as \"Esfera\",
				";
				
				$i = 0;
				
				if(is_array($arrayColsSEE) && count($arrayColsSEE))
				{
					
					foreach($arrayColsSEE as $k => $see )
					{
						if( count($arrayColsSECEE) )
						{
							$sql .= " {$see['coluna']} as {$see['nomeColuna']},
							";
						}
						else 
						{
							$i++;
							if( $i == count($arrayColsSEE))
							{
								$sql .= " {$see['coluna']} as {$see['nomeColuna']}
								";
							}
							else 
							{
								$sql .= " {$see['coluna']} as {$see['nomeColuna']},
								";
							}
						}
					}
				}
				
				
				if(is_array($arrayColsSECEE) && count($arrayColsSECEE))
				{
					foreach( $arrayColsSECEE as $k => $secee )
					{
						$i++;
						if( $i == count($arrayColsSECEE))
						{
							$sql .= " {$secee['coluna']} as {$secee['nomeColuna']}
							";
						}
						else 
						{
							$sql .= " {$secee['coluna']} as {$secee['nomeColuna']},
							";
						}
					}
				}
				
				
				
				$sql .="
				 
					FROM
					par.entidade ent
					INNER JOIN par.entidade ent2 ON ent2.muncod = ent.muncod AND ent2.dutid = 9  AND ent2.entstatus = 'A'
					INNER JOIN territorios.estado est on est.estuf = ent2.estuf
				
					WHERE
						ent.entstatus='A' 
						and ent.dutid =  10 
					
				" ;
				
				
				if( $strUf )
				{
					$sql.= "AND
						ent2.estuf in ( {$strUf} )
						
						";	 
				}
				
//				return $sql;
			}
			elseif ($isCLE)
			{
				$sql = "SELECT
						e.estuf as \"UF\",        
						'Estadual' as \"Esfera\",
				 		dut.dutdescricao as \"Tipo dos dados da unidade\",
				 ";
				 	foreach( $arrayColsCLE as $k => $cle )
					{
						$i++;
						if( $i == count($arrayColsCLE))
						{
							$sql .= " {$cle['coluna']} as {$cle['nomeColuna']}
							";
						}
						else 
						{
							$sql .= " {$cle['coluna']} as {$cle['nomeColuna']},
							";
						}
					}
				
				$sql .= " 
					FROM par.dadosunidade du
					INNER JOIN par.instrumentounidade iu on iu.inuid = du.inuid
					INNER JOIN territorios.estado e on e.estuf = iu.estuf
					INNER JOIN par.dadosunidadesegmento sgm on du.sgmid = sgm.sgmid
					INNER JOIN par.dadosunidadetipo dut on dut.dutid = du.dutid
					
					WHERE dut.dutid in (4, 5)
				";
				
				if( $strUf )
				{
					$sql.= "AND
						e.estuf in ( {$strUf} )
						
						";	 
				}
				
//				return $sql;
			}
			elseif ($isELE)
			{
				$sql = "SELECT
						e.estuf as \"UF\",        
						'Estadual' as \"Esfera\",
				 		dut.dutdescricao as \"Tipo dos dados da unidade\",
				 ";
				 	foreach( $arrayColsELE as $k => $ele )
					{
						$i++;
						if( $i == count($arrayColsELE))
						{
							$sql .= " {$ele['coluna']} as {$ele['nomeColuna']}
							";
						}
						else 
						{
							$sql .= " {$ele['coluna']} as {$ele['nomeColuna']},
							";
						}
					}
				
				$sql .= " 
					FROM par.dadosunidade du
					INNER JOIN par.instrumentounidade iu on iu.inuid = du.inuid
					INNER JOIN territorios.estado e on e.estuf = iu.estuf
					INNER JOIN par.dadosunidadesegmento sgm on du.sgmid = sgm.sgmid
					INNER JOIN par.dadosunidadetipo dut on dut.dutid = du.dutid
					
					WHERE dut.dutid = 3 
				";
				
				if( $strUf )
				{
					$sql.= "AND
						e.estuf in ( {$strUf} )
						
						";	 
				}
				
//				return $sql;
				
			}
			if( $sql != "" ){
				$sql .= " ORDER BY \"UF\" ";
			}
		}//Municipal
		else 
		{
			$sql = "";
			// Verifica tipo de entidade
			$arrTipoEntidade = $_POST['tipo_entidade_municipio']; 
			
			// Cria um array para cada entidade para guardar as colunas
			$arrayColsPRF = array();
			$arrayColsPRFTO = array();
			$arrayColsSME = array();
			$arrayColsDME = array();
			$arrayColsELM = array();
			$arrayColsCLM = array();     
			
			// Verifica os tipos de entidade selecionadas
			$isPRF		= ( in_array('PRF',$arrTipoEntidade )) ? true : false ;
			$isPRFTO	= ( in_array('PRFTO',$arrTipoEntidade )) ? true : false ; 
			$isSME		= ( in_array('SME',$arrTipoEntidade )) ? true : false ;
			$isDME		= ( in_array('DME',$arrTipoEntidade )) ? true : false ;
			$isELM		= ( in_array('ELM',$arrTipoEntidade )) ? true : false ;
			$isCLM		= ( in_array('CLM',$arrTipoEntidade )) ? true : false ;
			
			// Trata as combinações dos tipos de entidade escolhidos
			$isDirigenteSecretaria = false;
			$isPrefeitoPrefeitura = false;
			
			// Caso digigente municipal ou secretaria municipal tenham sido escolhidos
			if( $isDME || $isSME )
			{
				$isDirigenteSecretaria = true;
			}	
			
			// Caso prefeito ou prefeitura:
			if( $isPRF || $isPRFTO )
			{
				$isPrefeitoPrefeitura = true;	
			}
			
			
			if(is_array($arrColunas) && ( count($arrColunas) > 0 ))
			{
				foreach( $arrColunas as $k => $col )
				{
					// Prefeitura
					if( $isPRF )
					{
						switch ($col) 
						{
						    case 'CNPJ_PRF':
						        $arrayColsPRF[] = array(
						        	'coluna'	 => 'ent2.entnumcpfcnpj', 
						        	'nomeColuna' => '"CNPJ Prefeitura"'
						        );
							break;
							case 'IE_PRF':
						        $arrayColsPRF[] = array(
						        	'coluna'	 => 'ent2.entnuninsest', 
						        	'nomeColuna' => '"Inscrição Estadual Prefeitura"'
						        );
							break;
							case 'NOME_PRF':
						        $arrayColsPRF[] = array(
						        	'coluna'	 => 'ent2.entnome', 
						        	'nomeColuna' => '"Nome Prefeitura"'
						        );
							break;
							case 'RSOC_PRF':
						        $arrayColsPRF[] = array(
						        	'coluna'	 => 'ent2.entrazaosocial', 
						        	'nomeColuna' => '"Razão Social Prefeitura"'
						        );
							break;
							case 'EMAIL_PRF':
						        $arrayColsPRF[] = array(
						        	'coluna'	 => 'ent2.entemail', 
						        	'nomeColuna' => '"Email Prefeitura"'
						        );
							break;
							case 'SIG_PRF':
						        $arrayColsPRF[] = array(
						        	'coluna'	 => 'ent2.entsig', 
						        	'nomeColuna' => '"Sigla Prefeitura"'
						        );
							break;
							case 'TEL_PRF':
						        $arrayColsPRF[] = array(
						        	'coluna'	 => "'('||ent2.entnumdddcomercial|| ') ' ||ent2.entnumcomercial",  
						        	'nomeColuna' => '"Telefone Prefeitura"'
						        );
							break;
							case 'FAX_PRF':
						        $arrayColsPRF[] = array(
						        	'coluna'	 => "'('||ent2.entnumdddfax|| ') ' ||ent2.entnumfax", 
						        	'nomeColuna' => '"Fax Prefeitura"'
						        );
							break;
							case 'CEP_PRF':
						        $arrayColsPRF[] = array(
						        	'coluna'	 => 'ent2.endcep',
						        	'nomeColuna' => '"CEP Prefeitura"'
						        );
							break;
							
							case 'END_PRF':
						        $arrayColsPRF[] = array(
						        	'coluna'	 => " ent2.endlog 
														||
														CASE WHEN ( ent2.endcom  <> '')
														
															THEN
																 ' - Complemento: ' || ent2.endcom 
															ELSE
																'' 
														END
														
														||
														
														CASE WHEN ( ent2.endnum  <> '')
														
															THEN
																  ' - Número : ' || ent2.endnum
															ELSE
																'' 
														END
														||
														CASE WHEN ( ent2.endbai  <> '')
														
															THEN
																  ' - Bairro: ' || ent2.endbai
															ELSE
																'' 
														END",
						        	'nomeColuna' => '"Endereço Prefeitura"'
						        );
							break;
							
						}
					}
					//  Prefeito(a) 
					if( $isPRFTO )
					{ 	
						switch($col) 
						{
						    case 'CPF_PRFTO':
						        $arrayColsPRFTO[] = array(
						        	'coluna'	 => 'ent.entnumcpfcnpj', 
						        	'nomeColuna' => '"CPF Prefeito(a)"'
						        );
							break;
						    case 'NOME_PRFTO':
						        $arrayColsPRFTO[] = array(
						        	'coluna'	 => 'ent.entnome', 
						        	'nomeColuna' => '"Nome Prefeito(a)"'
						        );
							break;
						    case 'EMAIL_PRFTO':
						        $arrayColsPRFTO[] = array(
						        	'coluna'	 => 'ent.entemail', 
						        	'nomeColuna' => '"Email Prefeito(a)"'
						        );
							break;
						    case 'RG_PRFTO':
						        $arrayColsPRFTO[] = array(
						        	'coluna'	 => 'ent.entnumrg', 
						        	'nomeColuna' => '"Registro Geral (RG) Prefeito(a)"'
						        );
							break;
						    case 'OE_PRFTO':
						        $arrayColsPRFTO[] = array(
						        	'coluna'	 => 'ent.entorgaoexpedidor', 
						        	'nomeColuna' => '"Orgão Expedidor Prefeito(a)"'
						        );
							break;
						    case 'SEX_PRFTO':
						        $arrayColsPRFTO[] = array(
						        	'coluna'	 => 'ent.entsexo', 
						        	'nomeColuna' => '"Sexo Prefeito(a)"'
						        );
							break;
						    case 'DTN_PRFTO':
						        $arrayColsPRFTO[] = array(
						        	'coluna'	 => 'ent.entdatanasc', 
						        	'nomeColuna' => '"Data de Nascimento Prefeito(a)"'
						        );
							break;
						    case 'TEL_PRFTO':
						        $arrayColsPRFTO[] = array(
						        	'coluna'	 => "'('||ent.entnumdddcomercial|| ') ' ||ent.entnumcomercial", 
						        	'nomeColuna' => '"Telefone comercial Prefeito(a)"'
						        );
							break;
						    case 'FAX_PRFTO':
						        $arrayColsPRFTO[] = array(
						        	'coluna'	 => "'('||ent.entnumdddfax|| ') ' ||ent.entnumfax", 
						        	'nomeColuna' => '"Fax Prefeito(a)"'
						        );
							break;
							
						}
					}
					// Secretaria Municipal de Educação
					if( $isSME )
					{ 	
						switch ($col) 
						{                                                             
						    case 'NOME_SME':
						        $arrayColsSME[] = array(
						        	'coluna'	 => 'ent2.entnome', 
						        	'nomeColuna' => '"Nome Secretaria Municipal de Educação"'
						        );
							break;
						    case 'EMAIL_SME':
						        $arrayColsSME[] = array(
						        	'coluna'	 => 'ent2.entemail', 
						        	'nomeColuna' => '"Email Secretaria Municipal de Educação"'
						        );
							break;
						    case 'SIG_SME':
						        $arrayColsSME[] = array(
						        	'coluna'	 => 'ent2.entsig', 
						        	'nomeColuna' => '"Sigla Secretaria Municipal de Educação"'
						        );
							break;
						    case 'TEL_SME':
						        $arrayColsSME[] = array(
						        	'coluna'	 => "'('||ent2.entnumdddcomercial|| ') ' ||ent2.entnumcomercial", 
						        	'nomeColuna' => '"Telefone comercial Secretaria Municipal de Educação"'
						        );
							break;
						    case 'FAX_SME':
						        $arrayColsSME[] = array(
						        	'coluna'	 => "'('||ent2.entnumdddfax|| ') ' ||ent2.entnumfax",  
						        	'nomeColuna' => '"Fax Secretaria Municipal de Educação"'
						        );
							break;
						    case 'CEP_SME':
						        $arrayColsSME[] = array(
						        	'coluna'	 => 'ent2.endcep', 
						        	'nomeColuna' => '"CEP Secretaria Municipal de Educação"'
						        );
							case 'END_SME':
						        $arrayColsSME[] = array(
						        	'coluna'	 => " ent2.endlog 
														||
														CASE WHEN ( ent2.endcom  <> '')
														
															THEN
																 ' - Complemento: ' || ent2.endcom 
															ELSE
																'' 
														END
														
														||
														
														CASE WHEN ( ent2.endnum  <> '')
														
															THEN
																  ' - Número : ' || ent2.endnum
															ELSE
																'' 
														END
														||
														CASE WHEN ( ent2.endbai  <> '')
														
															THEN
																  ' - Bairro: ' || ent2.endbai
															ELSE
																'' 
														END",
						        	'nomeColuna' => '"Endereço Secretaria Municipal de Educação"'
						        );
							break;
						}
					}
					//  Dirigente Municipal de Educação 
					if( $isDME )
					{ 		
						switch ($col) 
						{
						    case 'CPF_DME':
						        $arrayColsDME[] = array(
						        	'coluna'	 => 'ent.entnumcpfcnpj', 
						        	'nomeColuna' => '"CPF Dirigente Municipal de Educação"'
						        );
							break;
						    case 'NOME_DME':
						        $arrayColsDME[] = array(
						        	'coluna'	 => 'ent.entnome', 
						        	'nomeColuna' => '"Nome Dirigente Municipal de Educação"'
						        );
							break;
						    case 'EMAIL_DME':
						        $arrayColsDME[] = array(
						        	'coluna'	 => 'ent.entemail', 
						        	'nomeColuna' => '"Email Dirigente Municipal de Educação"'
						        );
							break;
						    case 'RG_DME':
						        $arrayColsDME[] = array(
						        	'coluna'	 => 'ent.entnumrg', 
						        	'nomeColuna' => '"Registro Geral (RG) Dirigente Municipal de Educação"'
						        );
							break;
						    case 'OE_DME':
						        $arrayColsDME[] = array(
						        	'coluna'	 => 'ent.entorgaoexpedidor', 
						        	'nomeColuna' => '"Orgão Expedidor (RG) Dirigente Municipal de Educação"'
						        );
							break;
						    case 'SEX_DME':
						        $arrayColsDME[] = array(
						        	'coluna'	 => 'ent.entsexo', 
						        	'nomeColuna' => '"Sexo Dirigente Municipal de Educação"'
						        );
							break;
						    case 'DTN_DME':
						        $arrayColsDME[] = array(
						        	'coluna'	 => 'ent.entdatanasc', 
						        	'nomeColuna' => '"Data de Nascimento Dirigente Municipal de Educação"'
						        );
							break;
						    case 'TEL_DME':
						        $arrayColsDME[] = array(
						        	'coluna'	 => "'('||ent.entnumdddcomercial|| ') ' ||ent.entnumcomercial",  
						        	'nomeColuna' => '"Telefone Dirigente Municipal de Educação"'
						        );
							break;
						    case 'FAX_DME':
						        $arrayColsDME[] = array(
						        	'coluna'	 => "'('||ent.entnumdddfax|| ') ' ||ent.entnumfax", 
						        	'nomeColuna' => '"Fax Dirigente Municipal de Educação"'
						        );
							break;
						    
						}
					}
					//  Equipe Local Municipal 
					if( $isELM )
					{ 	
						switch ($col) 
						{
						    case 'CPF_ELM':
						        $arrayColsELM[] = array(
						        	'coluna'	 => 'du.duncpf', 
						        	'nomeColuna' => '"CPF Equipe Local"'
						        );
							break;
						    case 'NOME_ELM':
						        $arrayColsELM[] = array(
						        	'coluna'	 => 'du.dunnome', 
						        	'nomeColuna' => '"Nome Equipe Local"'
						        );
							break;
						    case 'FUNC_ELM':
						        $arrayColsELM[] = array(
						        	'coluna'	 => 'du.dunfuncao', 
						        	'nomeColuna' => '"Função/Cargo Equipe Local"'
						        );
							break;
						    case 'SEG_ELM':
						        $arrayColsELM[] = array(
						        	'coluna'	 => 'sgm.sgmdsc', 
						        	'nomeColuna' => '"Segmento Equipe Local"'
						        );
							break;
						    case 'EMAIL_ELM':
						        $arrayColsELM[] = array(
						        	'coluna'	 => 'du.dunemail', 
						        	'nomeColuna' => '"E-mail Equipe Local"'
						        );
							break;
						
						}
					}
					// Comitê Local MUNICIPAL 
					if( $isCLM )
					{ 	
						switch ($col) 
						{
						    case 'CPF_CLM':
						        $arrayColsCLM[] = array(
						        	'coluna'	 => 'du.duncpf', 
						        	'nomeColuna' => '"CPF Comitê Local"'
						        );
							break;
						    case 'NOME_CLM':
						        $arrayColsCLM[] = array(
						        	'coluna'	 => 'du.dunnome', 
						        	'nomeColuna' => '"Nome Comitê Local"'
						        );
							break;
						    case 'FUNC_CLM':
						        $arrayColsCLM[] = array(
						        	'coluna'	 => 'du.dunfuncao', 
						        	'nomeColuna' => '"Função/Cargo Comitê Local"'
						        );
							break;
						    case 'SEG_CLM':
						        $arrayColsCLM[] = array(
						        	'coluna'	 => 'sgm.sgmdsc', 
						        	'nomeColuna' => '"Segmento Comitê Local"'
						        );
							break;
						    case 'EMAIL_CLM':
						        $arrayColsCLM[] = array(
						        	'coluna'	 => 'du.dunemail', 
						        	'nomeColuna' => '"E-mail Comitê Local"'
						        );
							break;
						
						}
					}
				}
			}
			
		
			if( $isDirigenteSecretaria )
			{
				
				$sql = " SELECT 
						mun.estuf as \"UF\",        
		                mun.muncod as \"IBGE\",
		                mun.mundescricao as \"Local\",
		                'Municipal' as esfera,
		                
				";
				
				$i = 0;
				
				if(is_array($arrayColsSME) && count($arrayColsSME))
				{
					
					foreach($arrayColsSME as $k => $see )
					{
						if( count($arrayColsDME) )
						{
							$sql .= " {$see['coluna']} as {$see['nomeColuna']},
							";
						}
						else 
						{
							$i++;
							if( $i == count($arrayColsSME))
							{
								$sql .= " {$see['coluna']} as {$see['nomeColuna']}
								";
							}
							else 
							{
								$sql .= " {$see['coluna']} as {$see['nomeColuna']},
								";
							}
						}
					}
				}
				
				
				if(is_array($arrayColsDME) && count($arrayColsDME))
				{
					foreach( $arrayColsDME as $k => $secee )
					{
						$i++;
						if( $i == count($arrayColsDME))
						{
							$sql .= " {$secee['coluna']} as {$secee['nomeColuna']}
							";
						}
						else 
						{
							$sql .= " {$secee['coluna']} as {$secee['nomeColuna']},
							";
						}
					}
				}
				
				
				$sql .="
					FROM
						par.entidade ent
						INNER JOIN par.entidade ent2 ON ent2.inuid = ent.inuid AND ent2.dutid = 8 AND ent2.entstatus = 'A'
						INNER JOIN territorios.municipio mun on mun.muncod = ent2.muncod
						
						WHERE
							ent.dutid = 2  
							and ent.entstatus = 'A' 
					
				" ;
				//die($sql);
				
				if( $strMunCod )
				{
					$sql.= "AND
						mun.muncod in ( {$strMunCod} )
						
						";	 
				}
				if( $strUf )
				{
					$sql.= "AND
						mun.estuf in ( {$strUf} )
						
						";	 
				}
//				return $sql;
				
				
			}
			elseif(	$isPrefeitoPrefeitura )
			{
				
				$sql = " SELECT 
						mun.estuf as \"UF\",        
		                mun.muncod as \"IBGE\",
		                mun.mundescricao as \"Local\",
		                'Municipal' as esfera,
		                
				";
				
				$i = 0;
				
				if(is_array($arrayColsPRF) && count($arrayColsPRF))
				{
					
					foreach($arrayColsPRF as $k => $see )
					{
						if( count($arrayColsPRFTO) )
						{
							$sql .= " {$see['coluna']} as {$see['nomeColuna']},
							";
						}
						else 
						{
							$i++;
							if( $i == count($arrayColsPRF))
							{
								$sql .= " {$see['coluna']} as {$see['nomeColuna']}
								";
							}
							else 
							{
								$sql .= " {$see['coluna']} as {$see['nomeColuna']},
								";
							}
						}
					}
				}
				
				
				if(is_array($arrayColsPRFTO) && count($arrayColsPRFTO))
				{
					foreach( $arrayColsPRFTO as $k => $secee )
					{
						$i++;
						if( $i == count($arrayColsPRFTO))
						{
							$sql .= " {$secee['coluna']} as {$secee['nomeColuna']}
							";
						}
						else 
						{
							$sql .= " {$secee['coluna']} as {$secee['nomeColuna']},
							";
						}
					}
				}
				
				
				$sql .="
						FROM
						par.entidade ent
						INNER JOIN par.entidade ent2 ON ent2.inuid = ent.inuid AND ent2.dutid = 6   AND ent2.entstatus = 'A'
						INNER JOIN territorios.municipio mun on mun.muncod = ent2.muncod
						WHERE
							ent.dutid =  7 
							and ent.entstatus = 'A'
				" ;
				
				if( $strMunCod )
				{
					$sql.= "AND
						mun.muncod in ( {$strMunCod} )
						
						";	 
				}
				if( $strUf )
				{
					$sql.= "AND
						mun.estuf in ( {$strUf} )
						
						";	 
				}
//				return $sql;
				
			}
			elseif($isELM)
			{
				$sql = "SELECT
					m.estuf as \"UF\",        
	                m.muncod as \"IBGE\",
	                m.mundescricao as \"Local\",
	                'Municipal' as esfera,
				 	dut.dutdescricao as \"Tipo dos dados da unidade\",
				 ";
				 	foreach( $arrayColsELM as $k => $elm )
					{
						$i++;
						if( $i == count($arrayColsELM))
						{
							$sql .= " {$elm['coluna']} as {$elm['nomeColuna']}
							";
						}
						else 
						{
							$sql .= " {$elm['coluna']} as {$elm['nomeColuna']},
							";
						}
					}
				
				$sql .= " 
					FROM par.dadosunidade du
					INNER JOIN par.instrumentounidade iu on iu.inuid = du.inuid
					INNER JOIN territorios.municipio m on m.muncod = iu.muncod
					INNER JOIN par.dadosunidadesegmento sgm on du.sgmid = sgm.sgmid
					INNER JOIN par.dadosunidadetipo dut on dut.dutid = du.dutid
					
					WHERE dut.dutid = 3 	
				";
				/*
				m.muncod = '5200050'
					AND ""
			*/
				if( $strMunCod )
				{
					$sql.= "AND
						m.muncod in ( {$strMunCod} )
						
						";	 
				}
				if( $strUf )
				{
					$sql.= "AND
						m.estuf in ( {$strUf} )
						
						";	 
				}
				
//				return $sql;
			}
			elseif($isCLM)
			{
				$sql = "SELECT
					m.estuf as \"UF\",        
	                m.muncod as \"IBGE\",
	                m.mundescricao as \"Local\",
	                'Municipal' as esfera,
				 	dut.dutdescricao as \"Tipo dos dados da unidade\",
				 ";
				 	foreach( $arrayColsCLM as $k => $clm )
					{
						$i++;
						if( $i == count($arrayColsCLM))
						{
							$sql .= " {$clm['coluna']} as {$clm['nomeColuna']}
							";
						}
						else 
						{
							$sql .= " {$clm['coluna']} as {$clm['nomeColuna']},
							";
						}
					}
				
				$sql .= " 
					FROM par.dadosunidade du
					INNER JOIN par.instrumentounidade iu on iu.inuid = du.inuid
					INNER JOIN territorios.municipio m on m.muncod = iu.muncod
					INNER JOIN par.dadosunidadesegmento sgm on du.sgmid = sgm.sgmid
					INNER JOIN par.dadosunidadetipo dut on dut.dutid = du.dutid
					
					WHERE dut.dutid in (4, 5)
				";
				
				if( $strMunCod )
				{
					$sql.= "AND
						m.muncod in ( {$strMunCod} )
						
						";	 
				}
				if( $strUf )
				{
					$sql.= "AND
						m.estuf in ( {$strUf} )
						
						";	 
				}
				
//				return $sql;
			}
			if( $sql != "" ){
				$sql .= " ORDER BY \"UF\", \"Local\" ";
			}
		}

		if( $sql != "" ){
			return $sql;
		} else {
			return false;
		}
	}
	
	function retornaDados()
	{
		$sql   = $this->monta_sql();
		if($sql)
		{
			return $this->dados = $this->db->carregar( $sql );	
		}
		else
		{
			return array();
		}
		
		
	}
}

?>
<html>
	<head>
		
<?php 
	if( ! $isXls )
{
?>
		<script type="text/javascript" src="../includes/funcoes.js"></script>
		<link rel='stylesheet' type='text/css' href='../includes/Estilo.css'/>
		<link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>
<?php 
}
?>
		
	</head>
<body marginheight="0" marginwidth="0" leftmargin="0" topmargin="0">	
<?php
// Faz requisição para o método retornaDados que irá retornar o array com os dados do relatório
$dados =  $obRelatorio->retornaDados();

// Caso exista dados ele  monta o relatório, senão exibe mensagem de que não existem registros
if( is_array($dados) &&  count($dados))
{
	// Caso seja uma exportação para excel
	if($isXls)
	{
		$file_name="RelatorioDadosUnidade.xls";
		header("Pragma: public");
		header("Expires: 0");
		header("Cache-Control: must-revalidate, post-check=0, pre-check=0");
		header("Content-Type: application/force-download");
		header("Content-Type: application/octet-stream");
		header("Content-Type: application/download");
		header("Content-Disposition: attachment;filename=".$file_name);
		header("Content-Transfer-Encoding: binary ");
	}
	
	// Monta o cabeçalho com o Brasao
	$cabecalhoBrasao .= "<table width=\"100%\" cellspacing=\"1\" cellpadding=\"5\" border=\"0\" align=\"center\" class=\"tabela\">";
	$cabecalhoBrasao .= "<tr>" .
					    "<td colspan=\"100\">" .			
						monta_cabecalho_relatorio('100') .
					    "</td>" .
				        "</tr>
				        </table>";
	
					
	// Caso não seja excel monta cabeçalho e título						
	if( ! $isXls )
	{
		echo $cabecalhoBrasao;
		monta_titulo('Relatório de Dados das Prefeituras ','');
	}
	// Monta tabela que irá exibir os dados
	echo '<table class="tabela" align="center" border="1" cellpadding="5" cellspacing="1">';
	// Cabeçalho para o arquivo excel
	if( $isXls )
	{
		echo ' <tr style="background-color: #dcdcdc;"> <td colspan="' . count($dados[0]) . '" > Relatório Dados da Unidade </td></tr>';
	}
	
	// Começa linha com os nomes das colunas
	echo '	<tr >' ;
	foreach($dados[0] as $colKey => $colVal )
	{
		echo "		<td style=\"background-color: #e9e9e9;\" align=\'center\'  > <b> {$colKey} </b> </td>";
	}
	echo '</tr>';
	
	// Lista as informações do relatório
	foreach($dados as $linhaKey => $linhaVal )
	{
		echo '<tr>';
		foreach( $linhaVal as $colKey => $colVal )
		{
			echo " <td align=\'center\' width=\'50%\' > <b> {$colVal} </b> </td>";	
		}
		
	}
	// Finaliza o relatório
	echo '</table>';
	
}
else 
{
	// Exibe mensagem de que não existem registros.
	die( '<table cellspacing="1" cellpadding="5" border="0" align="center" class="tabela">'.
										'<TR style="background:#FFF; color:red;">'.
											'<TD colspan="10" align="center">'.
												'Não foram encontrados registros.'.
											'</TD>'.
										'</TR>'.
								   '</table>');
}
 
if($isXls)
{
	// Finaliza processo
	die('<script>window.close();</script>');
}

?>

</body>