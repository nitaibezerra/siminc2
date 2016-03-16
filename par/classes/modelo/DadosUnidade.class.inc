<?php
	
class DadosUnidade extends Modelo{
	
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "par.dadosunidade";	

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "dunid" );

    /**
     * Atributos
     * @var array
     * @access protected
     */   
    protected $arAtributos     = array(
									  	'dunid' => null, 
									  	'inuid' => null, 
									  	'entid' => null, 
									  	'dundata' => null, 
									  	'usucpf' => null,
    									'dutid' => null,
    									'sgmid' => null,
    									'dunfuncao' => null,
    									'duncpf' => null,    									
    									'dunemail' => null,
    									'dunnome' => null,
    									'dunsegmento' => null, 
									  );

	public function recuperarDadosPorInuid($inuid, $dutid = null)
	{
		if( !$inuid ){
			print "<script>"
				. "    alert('Falta o Inuid!');"
				. "    history.back(-1);"
				. "</script>";
			
			die;
		}
		
		if($dutid){
			$stWhere = " AND dutid = {$dutid}";
		}
		
		$sql = "SELECT 
					dunid, 
					entid,
					dutid,
					sgmid,
					dunfuncao,
					duncpf,
					dunemail,
					dunnome,
					dunsegmento
				FROM {$this->stNomeTabela} 
				WHERE inuid = {$inuid} 
				{$stWhere}";

		return $this->carregar($sql);		
	}
	
	public function recuperarSegmentos($retorno = 'array')
	{
		$sql = "SELECT sgmid as codigo, sgmdsc as descricao FROM par.dadosunidadesegmento ORDER BY sgmdsc ASC";
		if($retorno == 'array'){
			return $this->carregar($sql);
		} else {
			return $sql;
		}
	}
	
	public function deletarDadosPorDutid($inuid, $dutid)
	{
		$sql = "DELETE FROM {$this->stNomeTabela} WHERE inuid = {$inuid} AND dutid = {$dutid}";
		return $this->executar($sql);
	}
	
	public function recuperarFuncaoEntidadePorFunid($funid)
	{
		$sql = "SELECT fundsc FROM entidade.funcao WHERE funid = {$funid}";
		
		return $this->pegaUm($sql);
	}
	
	public function recuperarHistoricoEntidadeDadosUnidade($muncod, $estuf, $itrid, $boAlertas = false)
	{
		if($itrid == 2){
			$prefeitura = DUTID_PREFEITURA;
			$prefeito 	= DUTID_PREFEITO;
			/*
			$prefeitura = '1';
			$prefeito 	= '2';
			*/
		//	$stFueid1 = '2,15';
		//	$stFueid2 = '1,7';
		//	$stWhere = "AND en.muncod = '{$muncod}'";
		}else{
			$prefeitura = DUTID_SECRETARIA_ESTADUAL;
			$prefeito 	= DUTID_SECRETARIO_ESTADUAL;
			/*
			$prefeitura = '6';
			$prefeito 	= '25';
			*/
//			$stFueid1 = '25';
//			$stFueid2 = '6';
		}
		
		if( $muncod ){
		//	$sql = "SELECT estuf FROM territorios.municipio WHERE muncod = '".$muncod."'";
		//	$estuf = $this->pegaUm( $sql );
			$sql = "SELECT inuid FROM par.instrumentounidade WHERE muncod = '".$muncod."'";
			$inuid = $this->pegaUm( $sql );
		} else {
			$sql = "SELECT inuid FROM par.instrumentounidade WHERE estuf = '".$estuf."'";
			$inuid = $this->pegaUm( $sql );
		}
		
		/*
		$sql = "SELECT
					ent.entid,
					ent.entnome AS responsavel,
					max(his.hstdata) AS dthistresponsavel,
					fue.funid as funid1,
					ent2.entid,
					ent2.entnome AS entidade,
					max(his2.hstdata) AS dthistentidade,
					fue2.funid as funid2
				FROM entidade.entidade ent
					INNER JOIN entidade.endereco 		eed ON eed.entid = ent.entid
					INNER JOIN entidade.funcaoentidade 	fue ON fue.entid = ent.entid AND fue.funid IN ({$stFueid1}) AND fue.fuestatus = 'A'
					INNER JOIN entidade.funcao 			fun ON fun.funid = fue.funid
					LEFT JOIN entidade.historico 		his ON his.entid = ent.entid
					LEFT JOIN seguranca.usuario			usu ON usu.usucpf = his.usucpf
					LEFT JOIN entidade.funentassoc 		fea ON fea.fueid = fue.fueid
					LEFT JOIN entidade.entidade         ent2 ON ent2.entid = fea.entid 
					LEFT JOIN entidade.endereco         eed2 ON eed2.entid = ent2.entid 
					LEFT JOIN entidade.funcaoentidade 	fue2 ON fue2.entid = ent2.entid AND fue2.funid IN ({$stFueid2}) AND fue2.fuestatus = 'A'
					LEFT JOIN entidade.funcao 			fun2 ON fun2.funid = fue2.funid
					LEFT JOIN entidade.historico 		his2 ON his2.entid = ent2.entid	
					LEFT JOIN seguranca.usuario			usu2 ON usu2.usucpf = his2.usucpf			
				WHERE ent.entstatus = 'A'
				{$stWhere}
				AND eed2.estuf = '{$estuf}'
				GROUP BY ent.entid,ent.entnome,fue.funid,ent2.entid,ent2.entnome,fue2.funid";
		$arHistoricoDadosUnidade = $this->carregar($sql);
		if(!$boAlertas){
			return $arHistoricoDadosUnidade;
		}
		$arHistoricoDadosUnidade = $arHistoricoDadosUnidade ? $arHistoricoDadosUnidade : array();
		//ver($sql);
		// Verifica a data de atualização das entidades
		foreach($arHistoricoDadosUnidade as $historico){
			
			if($historico['dthistentidade'] < DATA_DADOS_UNIDADE_ATUALIZA && $_SESSION['par']['itrid'] == INSTRUMENTO_DIAGNOSTICO_MUNICIPAL){
				$arMsg[] = 'Atualize os dados cadastrais do(a) '.($historico['funid2'] == FUNID_SECRETARIA_MUNICIPAL_EDUCACAO ? 'Secretaria Municipal de Educação' : 'Prefeitura' ).'.';
			}
			
			if($historico['dthistresponsavel'] < DATA_DADOS_UNIDADE_ATUALIZA && $_SESSION['par']['itrid'] == INSTRUMENTO_DIAGNOSTICO_MUNICIPAL){
				$arMsg[] = 'Atualize os dados cadastrais do(a) '.($historico['funid1'] == FUNID_DIRIGENTE_MUNICIPAL_EDUCACAO ? 'Dirigente Municipal de Educação' : 'Prefeito(a)' ).'.'; 
			}
			
			if($historico['dthistentidade'] < DATA_DADOS_UNIDADE_ATUALIZA && $_SESSION['par']['itrid'] == INSTRUMENTO_DIAGNOSTICO_ESTADUAL){
				$arMsg[] = 'Atualize os dados cadastrais da Secretaria Estadual de Educação.';
			}
			
			if($historico['dthistresponsavel'] < DATA_DADOS_UNIDADE_ATUALIZA && $_SESSION['par']['itrid'] == INSTRUMENTO_DIAGNOSTICO_ESTADUAL){
				$arMsg[] = 'Atualize os dados cadastrais do(a) Dirigente Municipal de Educação.'; 
			}
			
			if($historico['funid1'] > 0){
				$arFunid[] = $historico['funid1'];
			}
			
			if($historico['funid2'] > 0){
				$arFunid[] = $historico['funid2'];
			}
		}
		
		$arFunid = $arFunid ? $arFunid : array();
		
		if(!in_array(FUNID_PREFEITURA, $arFunid) && $_SESSION['par']['itrid'] == INSTRUMENTO_DIAGNOSTICO_MUNICIPAL){
			$arMsg[] = 'Atualize os dados cadastrais da Prefeitura.';
		}
		
		if(!in_array(FUNID_PREFEITO, $arFunid) && $_SESSION['par']['itrid'] == INSTRUMENTO_DIAGNOSTICO_MUNICIPAL){
			$arMsg[] = 'Atualize os dados cadastrais do(a) Prefeito(a).';
		}

		if(!in_array(FUNID_SECRETARIA_MUNICIPAL_EDUCACAO, $arFunid) && $_SESSION['par']['itrid'] == INSTRUMENTO_DIAGNOSTICO_MUNICIPAL){
			$arMsg[] = 'Atualize os dados cadastrais da Secretaria Municipal de Educação.';
		}
		
		if(!in_array(FUNID_DIRIGENTE_MUNICIPAL_EDUCACAO, $arFunid) && $_SESSION['par']['itrid'] == INSTRUMENTO_DIAGNOSTICO_MUNICIPAL){
			$arMsg[] = 'Atualize os dados cadastrais do(a) Dirigente Municipal de Educação.';
		}
		
		if(!in_array(FUNID_SECRETARIA_ESTADUAL_EDUCACAO, $arFunid) && $_SESSION['par']['itrid'] == INSTRUMENTO_DIAGNOSTICO_ESTADUAL){
			$arMsg[] = 'Atualize os dados cadastrais da Secretaria Estadual de Educação.';
		}
		
		if(!in_array(FUNID_SECRETARIO_ESTADUAL_EDUCACAO, $arFunid) && $_SESSION['par']['itrid'] == INSTRUMENTO_DIAGNOSTICO_ESTADUAL){
			$arMsg[] = 'Atualize os dados cadastrais do(a) Dirigente Municipal de Educação.';
		}
		*/
		
		//Prefeitura
		/*
		$sql = "SELECT
					e.entid,
					e.entnome as entidade,
					max(his.hstdata) AS dthistresponsavel,
					e.entnumcpfcnpj,
					e.entnuninsest,
					e.entrazaosocial,
					e.entemail,
					e.entsig,
					e.entnumdddcomercial,
					e.entnumcomercial,
					en.endcep,
					en.endlog,
					en.endnum,
					en.endbai,
					en.estuf,
					en.muncod,
					en.medlatitude,
					en.medlongitude
				FROM
					entidade.entidade e 
				INNER JOIN entidade.funcaoentidade f ON f.entid = e.entid 
				INNER JOIN entidade.endereco en 	 ON en.entid = e.entid 
				LEFT  JOIN entidade.historico his 	 ON his.entid = e.entid
				WHERE
					funid='{$prefeitura}' {$stWhere} AND
					en.estuf = '{$estuf}'
				GROUP BY e.entid, e.entnome,en.endcep, en.endlog, en.endnum, en.endbai, en.estuf,
						en.muncod, en.medlatitude, en.medlongitude, e.entnumcpfcnpj, e.entnuninsest,
						e.entrazaosocial, e.entemail, e.entsig, e.entnumdddcomercial, e.entnumcomercial,
						en.endcep, en.endlog, en.endnum, en.endbai, en.estuf, en.muncod, en.medlatitude, 
						en.medlongitude";
		*/
		$sql = "SELECT
					e.entid,
					e.entnome as entidade,
					e.entdatainclusao AS dthistresponsavel,
					e.entnumcpfcnpj,
					e.entnuninsest,
					e.entrazaosocial,
					e.entemail,
					e.entsig,
					e.entnumdddcomercial,
					e.entnumcomercial,
					e.endcep,
					e.endlog,
					e.endnum,
					e.endbai,
					e.estuf,
					e.muncod,
					e.medlatitude,
					e.medlongitude
				FROM
					par.entidade e
				WHERE
					e.inuid = ".$inuid." AND e.dutid = ".$prefeitura. "ORDER BY e.entid DESC";
		$arrPrefeitura = $this->pegaLinha( $sql );

		if( is_array($arrPrefeitura) && $arrPrefeitura['entid'] ){
			
			//Prefeito
			/*
			$sql = "SELECT
						e.entid,
						e.entnome as responsavel,
						max(his.hstdata) AS dthistresponsavel,
						e.entemail,
						e.entnumrg,
						e.entorgaoexpedidor,
						e.entsexo,
						e.entdatanasc,
						e.entnumdddcomercial,
						e.entnumcomercial
					FROM
						entidade.entidade e 
					INNER JOIN entidade.funcaoentidade f ON f.entid = e.entid 
					INNER JOIN entidade.funentassoc fe 	 ON fe.fueid = f.fueid 
					LEFT  JOIN entidade.historico his 	 ON his.entid = e.entid
					WHERE
						funid='{$prefeito}' AND
						fe.entid = '{$arrPrefeitura['entid']}'
					GROUP BY 
						e.entid, e.entnome, e.entemail, e.entnumrg, e.entorgaoexpedidor,
						e.entsexo, e.entdatanasc, e.entnumdddcomercial, e.entnumcomercial";
			*/
			$sql = "SELECT
						e.entid,
						e.entnome as responsavel,
						e.entdatainclusao AS dthistresponsavel,
						e.entemail,
						e.entnumrg,
						e.entorgaoexpedidor,
						e.entsexo,
						e.entdatanasc,
						e.entnumdddcomercial,
						e.entnumcomercial
					FROM
						par.entidade e
					WHERE
						e.inuid = ".$inuid." AND e.dutid = ".$prefeito." ORDER BY e.entid DESC";
			$arrPrefeito = $this->pegaLinha( $sql );
		} 

		//Secretaria
		/*
		$sql = "SELECT
					e.entid,
					e.entnome as entidade,
					max(his.hstdata) AS dthistresponsavel,
					e.entemail,
					e.entsig,
					e.entnumdddcomercial,
					e.entnumcomercial,
					en.endcep,
					en.endlog,
					en.endnum,
					en.endbai,
					en.estuf,
					en.muncod,
					en.medlatitude,
					en.medlongitude
				FROM
					entidade.entidade e 
				INNER JOIN entidade.funcaoentidade f ON f.entid = e.entid 
				INNER JOIN entidade.endereco en 	 ON en.entid = e.entid 
				LEFT  JOIN entidade.historico his 	 ON his.entid = e.entid
				WHERE
					funid='7' {$stWhere} AND
					en.estuf = '{$estuf}'
				GROUP BY 
					e.entid, e.entnome,en.endcep, en.endlog, en.endnum, en.endbai, en.estuf,
					en.muncod, en.medlatitude, en.medlongitude, e.entemail, e.entsig,
					e.entnumdddcomercial, e.entnumcomercial";
		*/
		$sql = "SELECT
					e.entid,
					e.entnome as entidade,
					e.entdatainclusao AS dthistresponsavel,
					e.entemail,
					e.entsig,
					e.entnumdddcomercial,
					e.entnumcomercial,
					e.endcep,
					e.endlog,
					e.endnum,
					e.endbai,
					e.estuf,
					e.muncod,
					e.medlatitude,
					e.medlongitude
				FROM
					par.entidade e 
				WHERE
					e.inuid = ".$inuid." AND e.dutid = ".DUTID_SECRETARIA_MUNICIPAL. " ORDER BY e.entid DESC";
		$arrSecretaria = $this->pegaLinha( $sql );
		
		
		if( is_array($arrSecretaria) && $arrSecretaria['entid'] ){
			//Secretario
			/*
			$sql = "SELECT
						e.entid,
						e.entnome as responsavel,
						max(his.hstdata) AS dthistresponsavel,
						e.entemail,
						e.entnumrg,
						e.entorgaoexpedidor,
						e.entsexo,
						e.entdatanasc,
						e.entnumdddcomercial,
						e.entnumcomercial
					FROM
						entidade.entidade e 
					INNER JOIN entidade.funcaoentidade f ON f.entid = e.entid 
					INNER JOIN entidade.funentassoc fe 	 ON fe.fueid = f.fueid 
					LEFT  JOIN entidade.historico his 	 ON his.entid = e.entid
					WHERE
						funid='15' AND
						fe.entid = '{$arrSecretaria['entid']}'
					GROUP BY
						e.entid, e.entnome, e.entemail, e.entnumrg, e.entorgaoexpedidor,
						e.entsexo, e.entdatanasc, e.entnumdddcomercial, e.entnumcomercial";
			*/
			$sql = "SELECT
						e.entid,
						e.entnome as responsavel,
						e.entdatainclusao AS dthistresponsavel,
						e.entemail,
						e.entnumrg,
						e.entorgaoexpedidor,
						e.entsexo,
						e.entdatanasc,
						e.entnumdddcomercial,
						e.entnumcomercial
					FROM
						par.entidade e 
					WHERE
						e.inuid = ".$inuid." AND e.dutid = ".DUTID_DIRIGENTE." ORDER BY entid DESC";
			$arrSecretario = $this->pegaLinha( $sql );
		} 
		
		
//		if(!is_array($arrPrefeitura) && $_SESSION['par']['itrid'] == INSTRUMENTO_DIAGNOSTICO_MUNICIPAL){
		if($_SESSION['par']['itrid'] == INSTRUMENTO_DIAGNOSTICO_MUNICIPAL){
			if( !$arrPrefeitura['entidade'] || !$arrPrefeitura['dthistresponsavel'] || !$arrPrefeitura['entnumcpfcnpj'] ||
			!$arrPrefeitura['entnuninsest'] || !$arrPrefeitura['entrazaosocial'] || !$arrPrefeitura['entemail'] || !$arrPrefeitura['entsig'] ||
			!$arrPrefeitura['entnumdddcomercial'] || !$arrPrefeitura['entnumcomercial'] || !$arrPrefeitura['endcep'] || !$arrPrefeitura['endlog'] ||
			!$arrPrefeitura['endnum'] || !$arrPrefeitura['endbai'] || !$arrPrefeitura['estuf'] || !$arrPrefeitura['muncod'] || !$arrPrefeitura['medlatitude'] ||
			!$arrPrefeitura['medlongitude']){
				$arMsg[] = 'Atualize os dados cadastrais da Prefeitura.';
			}
		}
		
		if($_SESSION['par']['itrid'] == INSTRUMENTO_DIAGNOSTICO_MUNICIPAL){
			if( !$arrPrefeito['responsavel'] || !$arrPrefeito['entemail'] || !$arrPrefeito['entnumrg'] || 
			!$arrPrefeito['entorgaoexpedidor'] || !$arrPrefeito['entsexo'] || !$arrPrefeito['entdatanasc'] || !$arrPrefeito['entnumdddcomercial'] || 
			!$arrPrefeito['entnumcomercial'] ){
				$arMsg[] = 'Atualize os dados cadastrais do(a) Prefeito(a).';
			}
		}

		if($_SESSION['par']['itrid'] == INSTRUMENTO_DIAGNOSTICO_MUNICIPAL){
			if( !$arrSecretaria['entidade'] || !$arrSecretaria['entemail'] ||
			!$arrSecretaria['entsig'] || !$arrSecretaria['entnumdddcomercial'] || !$arrSecretaria['entnumcomercial'] || !$arrSecretaria['endcep'] ||
			!$arrSecretaria['endlog'] || !$arrSecretaria['endnum'] || !$arrSecretaria['endbai'] || !$arrSecretaria['estuf'] ||
			!$arrSecretaria['muncod'] || !$arrSecretaria['medlatitude'] || !$arrSecretaria['medlongitude'] ){
				$arMsg[] = 'Atualize os dados cadastrais da Secretaria Municipal de Educação.';
			}
		}
                
		if($_SESSION['par']['itrid'] == INSTRUMENTO_DIAGNOSTICO_MUNICIPAL){
			if(!$arrSecretario['responsavel'] || !$arrSecretario['entemail'] ||
			!$arrSecretario['entnumrg'] || !$arrSecretario['entorgaoexpedidor'] || !$arrSecretario['entsexo'] ||
			!$arrSecretario['entdatanasc'] || !$arrSecretario['entnumdddcomercial'] || !$arrSecretario['entnumcomercial'] ){
				$arMsg[] = 'Atualize os dados cadastrais do(a) Dirigente Municipal de Educação.';
			}
		}

		if($_SESSION['par']['itrid'] == INSTRUMENTO_DIAGNOSTICO_ESTADUAL){
			if( !$arrSecretaria['entidade'] || !$arrSecretaria['dthistresponsavel'] || !$arrSecretaria['entemail'] ||
			!$arrSecretaria['entsig'] || !$arrSecretaria['entnumdddcomercial'] || !$arrSecretaria['entnumcomercial'] || !$arrSecretaria['endcep'] ||
			!$arrSecretaria['endlog'] || !$arrSecretaria['endnum'] || !$arrSecretaria['endbai'] || !$arrSecretaria['estuf'] ||
			!$arrSecretaria['muncod'] || !$arrSecretaria['medlatitude'] || !$arrSecretaria['medlongitude'] ){	
				$arMsg[] = 'Atualize os dados cadastrais da Secretaria Estadual de Educação.';
			}
		}
		
		if($_SESSION['par']['itrid'] == INSTRUMENTO_DIAGNOSTICO_ESTADUAL){
			if(!$arrSecretario['responsavel'] || !$arrSecretario['dthistresponsavel'] || !$arrSecretario['entemail'] ||
			!$arrSecretario['entnumrg'] || !$arrSecretario['entorgaoexpedidor'] || !$arrSecretario['entsexo'] ||
			!$arrSecretario['entdatanasc'] || !$arrSecretario['entnumdddcomercial'] || !$arrSecretario['entnumcomercial'] ){
				$arMsg[] = 'Atualize os dados cadastrais do(a) Secretário Estadual de Educação.';
			}
		}
		

		//Pega os perfis do usuário
		$arrPerfil = pegaPerfilGeral();
		$arrPerfil = is_array($arrPerfil) ? $arrPerfil : Array();
		//Desabilita as opções de salvar para os perfis de consulta
		if(in_array(PAR_PERFIL_CONSULTA,$arrPerfil) || in_array(PAR_PERFIL_CONSULTA_ESTADUAL,$arrPerfil) || in_array(PAR_PERFIL_CONTROLE_SOCIAL_ESTADUAL,$arrPerfil) || in_array(PAR_PERFIL_CONSULTA_MUNICIPAL,$arrPerfil) || in_array(PAR_PERFIL_CONTROLE_SOCIAL_MUNICIPAL,$arrPerfil)){
			
		}else{
			//Mostra mensagens de pendências
			if(count($arMsg) && $_REQUEST['opt'] != 'salvarRegistro' && count($arMsg) != $_SESSION['par']['alerta'.$_SESSION['par']['muncod']] ){
				$msgs = implode('\n', $arMsg);
				echo "<script>
						alert('{$msgs}');
					  </script>";
				$_SESSION['par']['alerta'.$_SESSION['par']['muncod']] = count($arMsg);
			}
		}
	}
	
	public function recuperarNomeFuncaoEntidade($funid)
	{
		$sql = "SELECT fundsc FROM entidade.funcao WHERE funid = {$funid}";
		
		return $this->pegaUm($sql);
	}
	
	public function recuperarHistoricoEntidade($entid, $stFuncaoEntidade = null)
	{
		$sql = "select 
					ent.entnome,
					usu.usunome,
					usu.usucpf,
					--hst.hstdata,
					to_char(hst.hstdata, 'DD/MM/YYYY HH24:MI:SS') as hstdata,
					hst.hsturl,
					--hst.hstacao,
					case when hst.hstacao = 'U' then 'Alteração'
					when hst.hstacao = 'I' then 'Inserção'
					end as hstacao, 
					case when hst.endid is null then '$stFuncaoEntidade'
						else 'Endereço'
					end as alteracao_feita
				from entidade.historico hst 
				inner join entidade.entidade ent on ent.entid = hst.entid
				inner join seguranca.usuario usu on usu.usucpf = hst.usucpf
				where hst.entid = {$entid}
				and hst.sisid = ".SIS_OBRAS."
				order by hstdata desc";
		return $this->carregar($sql);
	}
	
	public function verificaPreenchimentoAbas($inuid, $muncod = null, $estuf = null, $itrid){
		$arConfigMsg = array();
		
		//Pegar os perfis
		$arrPerfil = pegaPerfilGeral();
		//Desabilita as opções de salvar para os perfis de consulta
		if(in_array(PAR_PERFIL_CONSULTA,$arrPerfil) || in_array(PAR_PERFIL_CONSULTA_ESTADUAL,$arrPerfil) || in_array(PAR_PERFIL_CONTROLE_SOCIAL_ESTADUAL,$arrPerfil) || in_array(PAR_PERFIL_CONSULTA_MUNICIPAL,$arrPerfil) || in_array(PAR_PERFIL_CONTROLE_SOCIAL_MUNICIPAL,$arrPerfil)){
			return $arConfigMsg = array();
		}
		
		if($itrid == INSTRUMENTO_DIAGNOSTICO_MUNICIPAL){
			$arConfigMsg[FUNID_PREFEITURA] = 'Favor preencher a aba Prefeitura.';
			$arConfigMsg[FUNID_PREFEITO] = 'Favor preencher a aba Prefeito.';
			$arConfigMsg[FUNID_SECRETARIA_MUNICIPAL_EDUCACAO] = 'Favor preencher a aba Secretaria Municipal de Educação.';
			$arConfigMsg[FUNID_DIRIGENTE_MUNICIPAL_EDUCACAO] = 'Favor preencher a aba Dirigente Municipal de Educação.';
			$sql = "SELECT
						distinct ent.entid, fun.funid
					FROM entidade.entidade ent
						INNER JOIN entidade.endereco 		eed2 ON eed2.entid = ent.entid
						INNER JOIN entidade.funcaoentidade 	fue ON fue.entid = ent.entid AND fue.funid = 1 AND fue.fuestatus = 'A'
						INNER JOIN entidade.funcao 			fun ON fun.funid = fue.funid				
					WHERE (ent.entstatus = 'A' OR ent.entstatus IS NULL) 
					AND eed2.muncod = '{$muncod}'
				union all
					SELECT
						distinct ent.entid, fun.funid								
					FROM entidade.entidade ent
						INNER JOIN entidade.funcaoentidade 	fue ON fue.entid = ent.entid AND fue.funid = 2 AND fue.fuestatus = 'A'
						INNER JOIN entidade.funcao 			fun ON fun.funid = fue.funid
						LEFT JOIN entidade.funentassoc 		fea ON fea.fueid = fue.fueid
						LEFT JOIN entidade.entidade         ent2 ON ent2.entid = fea.entid 
						LEFT JOIN entidade.endereco         eed2 ON eed2.entid = ent2.entid 
						LEFT JOIN entidade.funcaoentidade 	fue2 ON fue2.entid = ent2.entid AND fue2.funid = 1 AND fue2.fuestatus = 'A'
						LEFT JOIN entidade.funcao 			fun2 ON fun2.funid = fue2.funid
					WHERE (ent.entstatus = 'A' OR ent.entstatus IS NULL) 
					AND eed2.muncod = '{$muncod}'
				union all
					SELECT
						distinct ent.entid, fun.funid				
					FROM entidade.entidade ent
						INNER JOIN entidade.endereco 		eed2 ON eed2.entid = ent.entid
						INNER JOIN entidade.funcaoentidade 	fue ON fue.entid = ent.entid AND fue.funid = 7 AND fue.fuestatus = 'A'
						INNER JOIN entidade.funcao 			fun ON fun.funid = fue.funid				
					WHERE (ent.entstatus = 'A' OR ent.entstatus IS NULL) 
					AND eed2.muncod = '{$muncod}'
				union all
					SELECT
						distinct ent.entid, fun.funid							
					FROM entidade.entidade ent
						INNER JOIN entidade.funcaoentidade 	fue ON fue.entid = ent.entid AND fue.funid = 15 AND fue.fuestatus = 'A'
						INNER JOIN entidade.funcao 			fun ON fun.funid = fue.funid
						LEFT JOIN entidade.funentassoc 		fea ON fea.fueid = fue.fueid
						LEFT JOIN entidade.entidade         ent2 ON ent2.entid = fea.entid 
						LEFT JOIN entidade.endereco         eed2 ON eed2.entid = ent2.entid 
						LEFT JOIN entidade.funcaoentidade 	fue2 ON fue2.entid = ent2.entid AND fue2.funid = 7 AND fue2.fuestatus = 'A'
						LEFT JOIN entidade.funcao 			fun2 ON fun2.funid = fue2.funid
					WHERE (ent.entstatus = 'A' OR ent.entstatus IS NULL) 
					AND eed2.muncod = '{$muncod}'";
		} else {
			$arConfigMsg[FUNID_SECRETARIA_ESTADUAL_EDUCACAO] = 'Favor preencher a aba Secretaria Estadual de Educação.';
			$arConfigMsg[FUNID_SECRETARIO_ESTADUAL_EDUCACAO] = 'Favor preencher a aba Secretário(a) Estadual de Educação.';
			$sql = "SELECT
						distinct ent.entid, fun.funid				
					FROM entidade.entidade ent
						INNER JOIN entidade.endereco 		eed2 ON eed2.entid = ent.entid
						INNER JOIN entidade.funcaoentidade 	fue ON fue.entid = ent.entid AND fue.funid = 6 AND fue.fuestatus = 'A'
						INNER JOIN entidade.funcao 			fun ON fun.funid = fue.funid				
					WHERE (ent.entstatus = 'A' OR ent.entstatus IS NULL) 
					and eed2.estuf = '$estuf'
				union all
					SELECT
						distinct ent.entid, fun.funid								
					FROM entidade.entidade ent
						INNER JOIN entidade.funcaoentidade 	fue ON fue.entid = ent.entid AND fue.funid = 25 AND fue.fuestatus = 'A'
						INNER JOIN entidade.funcao 			fun ON fun.funid = fue.funid
						LEFT JOIN entidade.funentassoc 		fea ON fea.fueid = fue.fueid
						LEFT JOIN entidade.entidade         ent2 ON ent2.entid = fea.entid 
						LEFT JOIN entidade.endereco         eed2 ON eed2.entid = ent2.entid 
						LEFT JOIN entidade.funcaoentidade 	fue2 ON fue2.entid = ent2.entid AND fue2.funid = 6 AND fue2.fuestatus = 'A'
						LEFT JOIN entidade.funcao 			fun2 ON fun2.funid = fue2.funid
					WHERE (ent.entstatus = 'A' OR ent.entstatus IS NULL) 
					and eed2.estuf = '$estuf'";
			
		
		}
		
		$arDados = $this->carregar($sql);
		$arDados = ($arDados) ? $arDados : array();
		
		foreach($arDados as $dados){
			if(!in_array($dados['funid'], $arConfigMsg)){
				unset($arConfigMsg[$dados['funid']]);
			}
		}
		
		# VERIFICA SE A ABA EQUIPE LOCAL FOI PREENCHIDA
		$arUnidadeEquipeLocal = self::recuperarDadosPorInuid($_SESSION['par']['inuid'], EQUIPE_LOCAL);
		$arUnidadeEquipeLocal = ($arUnidadeEquipeLocal) ? $arUnidadeEquipeLocal : array();
		if(!count($arUnidadeEquipeLocal) && !$arUnidadeEquipeLocal[0]){
			$arConfigMsg[] = 'Favor preencher a aba Equipe Local.';
		}
		
		# VERIFICA SE A COORDENADOR NA ABA COMITÊ LOCAL FOI PREENCHIDA
		$arUniComiteCoordenador = self::recuperarDadosPorInuid($_SESSION['par']['inuid'], COMITE_LOCAL_COORDENADOR);
		$arUniComiteCoordenador = ($arUniComiteCoordenador) ? $arUniComiteCoordenador : array();
		if(!count($arUniComiteCoordenador) && !$arUniComiteCoordenador[0]){
			$arConfigMsg[] = 'Favor preencher ao menos um coordenador na aba de comitê local em dados da unidade.';
		}
		
		# VERIFICA SE A INTEGRANTES NA ABA COMITÊ LOCAL FOI PREENCHIDA
		$arUniComiteIntegrantes = self::recuperarDadosPorInuid($_SESSION['par']['inuid'], COMITE_LOCAL_INTEGRANTES);
		$arUniComiteIntegrantes = ($arUniComiteIntegrantes) ? $arUniComiteIntegrantes : array();
		if(!$arUniComiteIntegrantes && !$arUniComiteIntegrantes[0]){
			$arConfigMsg[] = 'Favor preencher ao menos um integrante na aba de comitê local em dados da unidade.';
		}
		
		if(count($arConfigMsg)){
			$msgs = implode('\n', $arConfigMsg);
			echo "<script>
					alert('{$msgs}');
					window.location.href = 'par.php?modulo=principal/orgaoEducacao&acao=A';
				  </script>";
			exit;
		}
	}
	
	public function verificaAbasDesatualizada($inuid, $muncod, $estuf, $itrid)
	{
		$arSecretariaDirigente = self::recuperarHistoricoEntidadeDadosUnidade($muncod, $estuf, $itrid);
		$arSecretariaDirigente = $arSecretariaDirigente ? $arSecretariaDirigente : array();
		
		foreach($arSecretariaDirigente as $dados){
			
			if($dados['dthistentidade'] > DATA_DADOS_UNIDADE_ATUALIZA){
				$arFunid[] = $dados['funid2'];
			}
			
			if($dados['dthistresponsavel'] > DATA_DADOS_UNIDADE_ATUALIZA){
				$arFunid[] = $dados['funid1']; 
			}
			
		}
		
		$sql = "SELECT 
					max(dunid) 
				FROM par.dadosunidade 
				WHERE dutid = ".EQUIPE_LOCAL." 
				AND inuid = {$inuid}
				
				UNION ALL
				
				SELECT 
					max(dunid) 
				FROM par.dadosunidade 
				WHERE dutid = ".COMITE_LOCAL_COORDENADOR." 
				AND inuid = {$inuid}
				
				UNION ALL
				
				SELECT 
					max(dunid) 
				FROM par.dadosunidade 
				WHERE dutid = ".COMITE_LOCAL_INTEGRANTES." 
				AND inuid = {$inuid}
				";
		
		$arEquipe = $this->carregar($sql);
		
		foreach($arEquipe as $dados){
			if(!empty($dados['max'])){
				$arFunid[] = $dados['max'];
			}
		}
		
		if(count($arFunid) == 7){
			return true;
		}
		
		return false;
	}
	
	public function recuperarHistorico($inuid, $dutid)
	{
		$sql = "SELECT 
					dun.usucpf,
					dun.dunnome, 
					dun.dundata
				FROM {$this->stNomeTabela} dun
				INNER JOIN seguranca.usuario usu ON usu.usucpf = dun.usucpf 
				WHERE dun.inuid = {$inuid}
				AND dun.dutid = {$dutid}
				ORDER BY dun.dunid DESC";
		return $this->pegaLinha($sql);
	}
	
	public function recuperarCpfDirigente($estuf, $muncod )
	{
		$inuid = $db->pegaUm( "SELECT inuid FROM par.instrumentounidade WHERE muncod = '".$muncod."'" );
		/*
		$sql = "SELECT
					ent.entnumcpfcnpj								
				FROM entidade.entidade ent
					INNER JOIN entidade.endereco 		eed ON eed.entid = ent.entid
					INNER JOIN entidade.funcaoentidade 	fue ON fue.entid = ent.entid AND fue.funid = 15 AND fue.fuestatus = 'A'
					INNER JOIN entidade.funcao 			fun ON fun.funid = fue.funid
					LEFT JOIN entidade.funentassoc 		fea ON fea.fueid = fue.fueid
					LEFT JOIN entidade.entidade         ent2 ON ent2.entid = fea.entid 
					LEFT JOIN entidade.endereco         eed2 ON eed2.entid = ent2.entid 
					LEFT JOIN entidade.funcaoentidade 	fue2 ON fue2.entid = ent2.entid AND fue2.funid = 7 AND fue2.fuestatus = 'A'
					LEFT JOIN entidade.funcao 			fun2 ON fun2.funid = fue2.funid
				WHERE ent.entstatus = 'A'
					AND eed2.estuf = '$estuf'
					AND eed2.muncod = '$muncod' and fue.funid = ".FUNID_DIRIGENTE_MUNICIPAL_EDUCACAO;
		*/
		$sql = "SELECT entnumcpfcnpj FROM par.entidade WHERE inuid = ".$inuid." AND dutid = ".DUTID_DIRIGENTE;
		return $this->pegaUm($sql);			
	}
	
}