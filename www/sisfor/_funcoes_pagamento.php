<?

function processarPagamentoBolsistaSGB($dados) {
	global $db;

	$sql = "SELECT * FROM sisfor.pagamentobolsista WHERE pboid='".$dados->id."'";
	$pagamentobolsista = $db->pegaLinha($sql);

	if($dados->situacao->codigo!='') {
		if($dados->situacao->codigo=='10001' ||
		$dados->situacao->codigo=='00023' ||
		$dados->situacao->codigo=='00025') {
			echo wf_alterarEstado( $pagamentobolsista['docid'], AED_ENVIAR_PAGAMENTO_SGB, $cmddsc = '', array());
		} elseif($dados->situacao->codigo=='10002') {
			echo wf_alterarEstado( $pagamentobolsista['docid'], AED_NAOAUTORIZAR_PAGAMENTO, $cmddsc = 'Erro retornado pelo FNDE: '.$dados->situacao->codigo.' / '.$dados->situacao->descricao, array());
		} else {
			echo wf_alterarEstado( $pagamentobolsista['docid'], AED_RECUSAR_PAGAMENTO, $cmddsc = 'Erro retornado pelo FNDE: '.$dados->situacao->codigo.' / '.$dados->situacao->descricao, array());
			$sql = "UPDATE sisfor.pagamentobolsista SET remid=null WHERE pboid='".$pagamentobolsista['pboid']."'";
			$db->executar($sql);
			$db->commit();
		}
	}

}

function sincronizarDadosUsuarioSGB($dados) {
	global $db;

	set_time_limit( 0 );

	ini_set( 'soap.wsdl_cache_enabled', '0' );
	ini_set( 'soap.wsdl_cache_ttl', 0 );

	$opcoes = Array(
			'exceptions'	=> 0,
			'trace'			=> true,
			//'encoding'		=> 'UTF-8',
			'encoding'		=> 'ISO-8859-1',
			'cache_wsdl'    => WSDL_CACHE_NONE
	);
	 
	$soapClient = new SoapClient( WSDL_CAMINHO_CADASTRO, $opcoes );

	libxml_use_internal_errors( true );


	$sql = "SELECT i.iuscpf, i.nacid, i.iusnome, i.iusdatanascimento, i.iusnomemae, i.iussexo, m.muncod as co_municipio_ibge_nascimento, m.estuf as sg_uf_nascimento,
			   i.eciid, lpad(i.iusagenciasugerida,4,'0') as iusagenciasugerida, m2.muncod as co_municipio_ibge, m2.estuf as sg_uf, ie.ienlogradouro, ie.iencomplemento,
			   ie.iennumero, ie.iencep, ie.ienbairro, it.itdufdoc, it.tdoid, it.itdnumdoc, it.itddataexp, it.itdnoorgaoexp, i.iusemailprincipal
		FROM sisfor.identificacaousuario i
		LEFT JOIN territorios.municipio m ON m.muncod = i.muncod
		LEFT JOIN sisfor.identificaoendereco ie ON ie.iusd = i.iusd
		LEFT JOIN territorios.municipio m2 ON m2.muncod = ie.muncod
		LEFT JOIN sisfor.identusutipodocumento it ON it.iusd = i.iusd
		WHERE i.iusd='".$dados['iusd']."'";

	$dadosusuario = $db->pegaLinha($sql);

	if($dadosusuario) {

		// consultando se cpf existe no SGB
		$xmlRetorno = $soapClient->lerDadosBolsista(
				array('sistema' => SISTEMA_SGB,
						'login'   => USUARIO_SGB,
						'senha'   => SENHA_SGB,
						'nu_cpf'  => $dadosusuario['iuscpf']
				)
		);
		 
		if(!$dados['sincronizacao']) $lnscpf = $db->carregarColuna("SELECT lnscpf FROM sisfor.listanegrasgb");
		else $lnscpf = array();
		 
		if(!in_array($dadosusuario['iuscpf'],$lnscpf)) {
			inserirDadosLog(array('logrequest'=>$soapClient->__getLastRequest(),'logresponse'=>$soapClient->__getLastResponse(),'logcpf'=>$dadosusuario['iuscpf'],'logservico'=>'lerDadosBolsista'));
		} else {
			inserirDadosLog(array('logrequest'=>'Bolsista com problemas de characteres especiais no SGB. Adicionado a lista negra.','logresponse'=>'Bolsista com problemas de characteres especiais no SGB. Adicionado a lista negra.','logcpf'=>$dadosusuario['iuscpf'],'logservico'=>'lerDadosBolsista'));
		}

		preg_match("/<nu_cpf>(.*)<\\/nu_cpf>/si", $xmlRetorno, $match);
		 
		//$xml = new SimpleXMLElement( $xmlRetorno );
		//$existecpf = (string) $xml->nu_cpf;
		$existecpf = (string) $match[1];
		 
		if($existecpf) $ac = 'A';
		else $ac = 'I';

		// gravando dados do bolsista, se existir atualizar senão inserir
		$xmlRetorno_gravarDadosBolsista = $soapClient->gravarDadosBolsista(
				array('sistema'  => SISTEMA_SGB,
						'login'    => USUARIO_SGB,
						'senha'    => SENHA_SGB,
						'acao'     => $ac,
						'dt_envio' => date( 'Y-m-d' ),
						'pessoa'   => array('nu_cpf'                        => $dadosusuario['iuscpf'],
								'no_pessoa'                     => removeAcentos( addslashes($dadosusuario['iusnome']) ),
								'dt_nascimento' 				  => $dadosusuario['iusdatanascimento'],
								'no_pai'        				  => '',
								'no_mae'        				  => removeAcentos( str_replace(array("'"),array(" "),$dadosusuario['iusnomemae']) ),
								'sg_sexo'       				  => $dadosusuario['iussexo'],
								'co_municipio_ibge_nascimento'  => (($dadosusuario['co_municipio_ibge_nascimento'])?$dadosusuario['co_municipio_ibge_nascimento']:$dadosusuario['co_municipio_ibge']),
								'sg_uf_nascimento'              => (($dadosusuario['sg_uf_nascimento'])?$dadosusuario['sg_uf_nascimento']:$dadosusuario['sg_uf']),
								'co_estado_civil'               => $dadosusuario['eciid'],
								'co_nacionalidade'              => $dadosusuario['nacid'],
								'co_situacao_pessoa'            => 1,
								'no_conjuge'                    => $dadosusuario['iusnomeconjuge'],
								'ds_endereco_web'               => '',
								'co_agencia_sugerida'           => $dadosusuario['iusagenciasugerida'],
								'enderecos' 					  => array(array('co_municipio_ibge'       => $dadosusuario['co_municipio_ibge'],
								'sg_uf'                   => $dadosusuario['sg_uf'],
								'ds_endereco'             => removeAcentos( str_replace(array("'"),array(" "),$dadosusuario['ienlogradouro']) ),
								'ds_endereco_complemento' => removeAcentos( str_replace(array("'"),array(" "),$dadosusuario['iencomplemento']) ),
								'nu_endereco'             => removeAcentos( (($dadosusuario['iennumero'])?$dadosusuario['iennumero']:'0') ),
								'nu_cep'                  => $dadosusuario['iencep'],
								'no_bairro'               => removeAcentos( addslashes($dadosusuario['ienbairro']) ),
								'tp_endereco'             => 'R'
										)
						),
						'documentos' 				  	  => array(array('uf_documento'       => $dadosusuario['itdufdoc'],
						'co_tipo_documento'  => $dadosusuario['tdoid'],
						'nu_documento'       => str_replace(array("\'","'"),array(" "," "),$dadosusuario['itdnumdoc']),
						'dt_expedicao'       => $dadosusuario['itddataexp'],
						'no_orgao_expedidor' => removeAcentos(str_replace(array("'"),array(" "),$dadosusuario['itdnoorgaoexp']))
						)
						),
						'emails'                        => array(array('ds_email' => $dadosusuario['iusemailprincipal']
						)
						),
						'formacoes'                     => array( ),
						'experiencias'                  => array( ),
						'telefones'                     => array( ),
						'vinculacoes' 				  => array( )
						)
		)
		);

		$logerro_gravarDadosBolsista = analisaCodXML($xmlRetorno_gravarDadosBolsista,'10001');

		if(!in_array($dadosusuario['iuscpf'],$lnscpf)) {
			inserirDadosLog(array('logerro'=>$logerro_gravarDadosBolsista,'logrequest'=>$soapClient->__getLastRequest(),'logresponse'=>$soapClient->__getLastResponse(),'logcpf'=>$dadosusuario['iuscpf'],'logservico'=>'gravarDadosBolsista'));
		} else {
			inserirDadosLog(array('logerro'=>$logerro_gravarDadosBolsista,'logrequest'=>'Bolsista com problemas de characteres especiais no SGB. Adicionado a lista negra.','logresponse'=>'Bolsista com problemas de characteres especiais no SGB. Adicionado a lista negra.','logcpf'=>$dadosusuario['iuscpf'],'logservico'=>'gravarDadosBolsista'));
		}
		 
		$sql = "UPDATE sisfor.identificacaousuario SET cadastradosgb=".(($logerro_gravarDadosBolsista=='TRUE')?'FALSE':'TRUE')." WHERE iusd='".$dados['iusd']."'";
		$db->executar($sql);
		$db->commit();

	}

}


function sincronizarDadosEntidadeSGB($dados) {
	global $db;

	set_time_limit( 0 );

	ini_set( 'soap.wsdl_cache_enabled', '0' );
	ini_set( 'soap.wsdl_cache_ttl', 0 );

	$opcoes = Array(
			'exceptions'	=> 0,
			'trace'			=> true,
			//'encoding'		=> 'UTF-8',
			'encoding'		=> 'ISO-8859-1',
			'cache_wsdl'    => WSDL_CACHE_NONE
	);
	 
	$soapClient = new SoapClient( WSDL_CAMINHO_CADASTRO , $opcoes );

	libxml_use_internal_errors( true );

	$sql = "SELECT s.siecnpj as unicnpj, p.unidsc as uninome, s.muncodies as muncod, m.estuf as uniuf
			FROM sisfor.sisfories s 
			INNER JOIN public.unidade p ON p.unicod = s.unicod 
			INNER JOIN territorios.municipio m ON m.muncod = s.muncodies 
			WHERE s.sieid='".$dados['sieid']."'";

	$dadosentidade = $db->pegaLinha($sql);

	$xmlRetornoEntidade = $soapClient->lerDadosEntidade( array('sistema'           => SISTEMA_SGB,
			'login'            => USUARIO_SGB,
			'senha'            => SENHA_SGB,
			'nu_cnpj_entidade' => $dadosentidade['unicnpj']
	) );
	 
	inserirDadosLog(array('logrequest'=>$soapClient->__getLastRequest(),'logresponse'=>$soapClient->__getLastResponse(),'logcnpj'=>$dadosentidade['unicnpj'],'logservico'=>'lerDadosEntidade'));

	preg_match("/<nu_cnpj_entidade>(.*)<\\/nu_cnpj_entidade>/si", $xmlRetornoEntidade, $match);

	$existecnpj = (string) $match[1];

	$dadosEntidade = array( 'sistema'          => SISTEMA_SGB,
			'login'            => USUARIO_SGB,
			'senha'            => SENHA_SGB,
			'nu_cnpj_entidade' => $dadosentidade['unicnpj'],
			'co_tipo_entidade' => '1',
			'no_entidade'      => $dadosentidade['uninome'],
			'sg_entidade'      => '',
			'co_municipio'     => $dadosentidade['muncod'],
			'sg_uf'            => $dadosentidade['uniuf']
	);

	$xmlRetorno_gravaDadosEntidade   = $soapClient->gravaDadosEntidade( $dadosEntidade );

	$logerro_gravaDadosEntidade = analisaCodXML($xmlRetorno_gravaDadosEntidade,'10001');

	inserirDadosLog(array('logrequest'=>$soapClient->__getLastRequest(),'logresponse'=>$soapClient->__getLastResponse(),'logcnpj'=>$dadosentidade['unicnpj'],'logservico'=>'gravaDadosEntidade','logerro' => $logerro_gravaDadosEntidade));

	if($existecnpj) $logerro_gravaDadosEntidade = 'FALSE';
	 
	$sql = "UPDATE sisfor.sisfories SET siecadastrosgb=".(($logerro_gravaDadosEntidade=='TRUE')?'FALSE':'TRUE')." WHERE sieid='".$dados['sieid']."'";
	$db->executar($sql);
	$db->commit();

}

function analisaCodXML($xml,$cod) {
	if(strpos($xml, $cod.':')) {
		return 'FALSE';
	} else {
		return 'TRUE';
	}

}

function inserirDadosLog($dados) {
	global $db;

	$sql = "INSERT INTO sisfor.logsgb(
            pboid, logrequest, logresponse, logcpf, logcnpj, logservico,
            logdata, logerro, remid)
    		VALUES (".(($dados['pboid'])?"'".$dados['pboid']."'":"NULL").",
    				".(($dados['logrequest'])?"'".addslashes($dados['logrequest'])."'":"NULL").",
    				".(($dados['logresponse'])?"'".addslashes($dados['logresponse'])."'":"NULL").",
    				".(($dados['logcpf'])?"'".$dados['logcpf']."'":"NULL").",
    				".(($dados['logcnpj'])?"'".$dados['logcnpj']."'":"NULL").",
    				".(($dados['logservico'])?"'".$dados['logservico']."'":"NULL").",
    				NOW(),
    				".(($dados['logerro'])?$dados['logerro']:"NULL").",
    				".(($dados['remid'])?$dados['remid']:"NULL").");";

	$db->executar($sql);
	$db->commit();
}



?>