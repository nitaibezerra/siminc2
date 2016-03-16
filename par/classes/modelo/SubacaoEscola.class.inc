<?php
	
class SubacaoEscola extends Modelo
{
	
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "par.subacaoescolas";	

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "sesid" );

    /**
     * Atributos
     * @var array
     * @access protected
     */    
    protected $arAtributos     = array(
									  	'sesid' => null, 
    									'sbaid' => null,
									  	'sesano' => null, 
									  	'escid' => null,
    									'sesquantidade' => null,
    									'sesstatus' => null,
    									'sesquantidadetecnico' => null,
    									'sesvalidatecnico' => null
									  );

	public function deletaEscolasPorSubacao($sbaid,$sesano)
	{
		$sql = "update $this->stNomeTabela set sesstatus = 'I' where sbaid = $sbaid and sesano = $sesano";
		$this->executar($sql);	
		return $this->commit($sql);
	}
	
	public function recuperaEscolaPorSubacao($sbaid,$sesano,$entid)
	{
		$sql = "select 
					sesid 
				from 
					$this->stNomeTabela ses
				inner join
					par.escolas esc ON esc.escid = ses.escid 
				where 
					sbaid = $sbaid 
				and 
					sesano = $sesano
				and
					entid = $entid";
		return $this->pegaUm($sql);
	}

	public function montaLista($sbaid, $sesano = null, $frmid=null, $ptsid=null){
		
		include_once APPRAIZ . "includes/workflow.php";	
		
		$inuid = $_SESSION['par']['inuid'] ? $_SESSION['par']['inuid'] : $_REQUEST['inuid'];
		$arrayPerfil = pegaArrayPerfil($_SESSION['usucpf']);
		
		$sql = "select docid from par.subacao where sbaid = ".$sbaid;
		$docid = $this->pegaUm($sql);
		$estadoAtual = wf_pegarEstadoAtual( $docid );

		if( $sesano ){
			$whereano = "and ses.sesano = '$sesano'";
			if (in_array(PAR_PERFIL_EQUIPE_FINANCEIRA ,$arrayPerfil) && $subacao['frmid'] == FORMA_EXECUCAO_ASSISTENCIA_FINANCEIRA){
				$acao  = "'<center><img class=\"middle link\" title=\"Excluir Escola\" src=\"../imagens/excluir.gif\" onclick=\"excluirEscola(this,\'' || ses.sesid || '\')\"  /></center>' as acao,";
			}else{
				$acao  = "'' as acao,";
			}
			if( ($estadoAtual['esdid'] == WF_SUBACAO_ANALISE || $estadoAtualSubacao["esdid"] == WF_SUBACAO_DILIGENCIA) ){
				$group1 = "group by	t.entnome, t.entcodent, m.mundescricao, ses.sesid, ses.sesquantidade, ses.sesquantidadetecnico, ses.sesvalidatecnico, ted.num_salas_existentes, ted.num_alunos_existentes ";
			} else {
				$group1 = "group by	t.entnome, t.entcodent, m.mundescricao, ses.sesid, ses.sesquantidade, ted.num_salas_existentes, ted.num_alunos_existentes ";
			}
			if( ($estadoAtual['esdid'] == WF_SUBACAO_ANALISE || $estadoAtualSubacao["esdid"] == WF_SUBACAO_DILIGENCIA) ){
				$group2 = "group by t.entnome, t.entcodent, ses.sesid, ses.sesquantidade, ses.sesquantidadetecnico, ses.sesvalidatecnico, ted.num_salas_existentes, ted.num_alunos_existentes ";
			} else {
				$group2 = "group by t.entnome, t.entcodent, ses.sesid, ses.sesquantidade, ted.num_salas_existentes, ted.num_alunos_existentes ";
			}
		} else {
			$qtd = ",sum(ses.sesquantidade)";
			if( ($estadoAtual['esdid'] == WF_SUBACAO_ANALISE || $estadoAtualSubacao["esdid"] == WF_SUBACAO_DILIGENCIA) || $estadoAtualSubacao["esdid"] == WF_SUBACAO_DILIGENCIA ){
				$group1 = "group by	t.entnome, t.entcodent, m.mundescricao, ses.sesquantidade, ses.sesquantidadetecnico, ses.sesvalidatecnico, ted.num_salas_existentes, ted.num_alunos_existentes ";
			} else {
				$group1 = "group by	t.entnome, t.entcodent, m.mundescricao, ses.sesquantidade, ted.num_salas_existentes, ted.num_alunos_existentes ";
			}
			$group2 = "group by t.entnome, t.entcodent, ted.num_salas_existentes, ted.num_alunos_existentes";
		}

		if($_SESSION['par']['itrid'] == 1){
			if( $sesano ){
				$cabecalho = array("Ação","Município","Escola","Código INEP", "Qtd de Salas", "Qtd de Alunos");
			} else {
				$cabecalho = array("Município","Escola","Código INEP", "Qtd. de Salas", "Qtd de Alunos");
			}

			if( ($frmid == FORMA_EXECUCAO_ASSITENCIA_TECNICA || ($frmid == FORMA_EXECUCAO_EXECUTADA_PELO_MUNICIPIO && $ptsid == FORMA_EXECUCAO_EXECUTADA_PELO_MUNICIPIO_SEM_ITENS) || ( $frmid == FORMA_EXECUCAO_EXECUTADA_PELO_ESTADO && $ptsid == FORMA_EXECUCAO_EXECUTADA_PELO_ESTADO_SEM_ITENS ) ) && $sesano){ 
				array_push($cabecalho,'Quantidade');
				$qtd = ", '<input size=\"10\" id=\"sesquantidade_' || ses.sesid || '\" type=\"text\" onKeyUp=\"this.value=mascaraglobal(\'[#]\',this.value);\" onmouseover=\"MouseOver(this);\" onfocus=\"MouseClick(this);\"	onmouseout=\"MouseOut(this);\" onblur=\"MouseBlur(this);\" onfocus=\"this.select()\" name=\"sesquantidade[{$sesano}]['|| ses.sesid ||']\"";
							if( ($estadoAtual['esdid'] == WF_SUBACAO_ANALISE || $estadoAtualSubacao["esdid"] == WF_SUBACAO_DILIGENCIA) && in_array(PAR_PERFIL_SUPER_USUARIO,$arrayPerfil) || in_array(PAR_PERFIL_ADMINISTRADOR,$arrayPerfil) || in_array(PAR_PERFIL_EQUIPE_TECNICA,$arrayPerfil) || in_array(PAR_PERFIL_EQUIPE_FINANCEIRA,$arrayPerfil) ){
								$qtd .= "' || 'disabled=disabled' || '";
							}
							$qtd .= " value=\"' || 
								case 
						    		when substring( trim(to_char(coalesce(sesquantidade, 0), '9999999990D99')), position(',' in trim(to_char(coalesce(sesquantidade, 0), '9999999990D99'))) +1, 2 ) = '00'
						    			then trim(to_char(coalesce(sesquantidade, 0), '999999999'))
						    		else trim(to_char(coalesce(sesquantidade, 0), '9999999990D99'))
						    	END
							|| '\" />' as sesqtd";
				if( ($estadoAtual['esdid'] == WF_SUBACAO_ANALISE || $estadoAtualSubacao["esdid"] == WF_SUBACAO_DILIGENCIA) ){
						array_push($cabecalho,'Quantidade Técnico');
					if( in_array(PAR_PERFIL_SUPER_USUARIO,$arrayPerfil) || in_array(PAR_PERFIL_ADMINISTRADOR,$arrayPerfil) || in_array(PAR_PERFIL_EQUIPE_TECNICA,$arrayPerfil) || in_array(PAR_PERFIL_EQUIPE_FINANCEIRA,$arrayPerfil)){
						$qtd .= ", '<input size=\"10\" id=\"sesquantidadetecnico_' || ses.sesid || '\" type=\"text\" onKeyUp=\"this.value=mascaraglobal(\'[#]\',this.value);\" onmouseover=\"MouseOver(this);\" onfocus=\"MouseClick(this);\" onmouseout=\"MouseOut(this);\" onblur=\"MouseBlur(this);\"onfocus=\"this.select()\" ' || 
									CASE WHEN sesvalidatecnico = 'S' THEN '' ELSE 'disabled=disabled' END || ' name=\"sesquantidadetecnico[{$sesano}]['|| ses.sesid ||']\" value=\"' || 
										case 
								    		when substring( trim(to_char(coalesce(sesquantidadetecnico, 0), '9999999990D99')), position(',' in trim(to_char(coalesce(sesquantidadetecnico, 0), '9999999990D99'))) +1, 2 ) = '00'
								    			then trim(to_char(coalesce(sesquantidadetecnico, 0), '999999999'))
								    		else trim(to_char(coalesce(sesquantidadetecnico, 0), '9999999990D99'))
								    	END
									|| '\" />' as sesqtdtecnico, 
									'<center><input type=\"radio\" name=\"validaE[{$sesano}][' || ses.sesid || ']\" id=\"validaE[{$sesano}][' || ses.sesid || ']\" value=\"S\" ' || CASE WHEN sesvalidatecnico = 'S' THEN 'checked' ELSE '' END || ' onclick=\"validaEscola(this,$sbaid,$sesano,' || ses.sesid || ')\">Sim  <input type=\"radio\" name=\"validaE[{$sesano}][' || ses.sesid || ']\" id=\"validaE[{$sesano}][' || ses.sesid || ']\" value=\"N\" ' || CASE WHEN sesvalidatecnico = 'N' THEN 'checked' ELSE '' END || ' onclick=\"validaEscola(this,$sbaid,$sesano,' || ses.sesid || ')\">Não</center>' as escolaaprovada ";
					} else {
						$qtd .= ", '<input size=\"10\" id=\"sesquantidadetecnico_' || ses.sesid || '\"
									type=\"text\" onKeyUp=\"this.value=mascaraglobal(\'[#]\',this.value);\" onmouseover=\"MouseOver(this);\" onfocus=\"MouseClick(this);\" onmouseout=\"MouseOut(this);\" onblur=\"MouseBlur(this);\" onfocus=\"this.select()\" ' || 
									CASE WHEN sesvalidatecnico = 'S' THEN '' ELSE 'disabled=disabled' END || ' name=\"sesquantidadetecnico[{$sesano}]['|| ses.sesid ||']\" disabled=disabled value=\"' || 
										case 
								    		when substring( trim(to_char(coalesce(sesquantidadetecnico, 0), '9999999990D99')), position(',' in trim(to_char(coalesce(sesquantidadetecnico, 0), '9999999990D99'))) +1, 2 ) = '00'
								    			then trim(to_char(coalesce(sesquantidadetecnico, 0), '999999999'))
								    		else trim(to_char(coalesce(sesquantidadetecnico, 0), '9999999990D99'))
								    	END
									|| '\" />' as sesqtdtecnico, 
									CASE WHEN sesvalidatecnico = 'S' THEN 'Sim' ELSE 'Não' END as escolaaprovada";
					} 
					array_push($cabecalho,'Escola Aprovada');
				}
			}

			$sql = "	select distinct
							{$acao}
							m.mundescricao,
							t.entnome,
							t.entcodent as entcodent,
							COALESCE(ted.num_salas_existentes,0) as qtd_salas,
							ted.num_alunos_existentes as qtde_alunos
							{$qtd}
						from
							entidade.entidade t
		--				inner join 
		--					entidade.funcaoentidade f on f.entid = t.entid
			            left join
			            	entidade.entidadedetalhe ed on t.entid = ed.entid
			            inner join
			            	entidade.endereco d on t.entid = d.entid
			            left join 
			            	territorios.municipio m on m.muncod = d.muncod
			            inner join 
			            	par.escolas e on e.entid = t.entid
			            inner join 
			            	par.subacaoescolas ses on ses.escid = e.escid
			            and 
			            	ses.sesstatus = 'A'
			            {$whereano}
				        and
				        	ses.sbaid = '$sbaid'
				        left join
							".SCHEMAEDUCACENSO.".tab_dado_escola ted on ted.fk_cod_entidade::character varying = t.entcodent::character varying	
						--left join
						--	".SCHEMAEDUCACENSO.".tab_matricula mat on mat.fk_cod_entidade = ted.fk_cod_entidade
						where
							(t.entescolanova = false or t.entescolanova is null)
						AND 
							t.entstatus = 'A'
		--		        and 
		--		        	f.funid = 3
				        and
				            t.tpcid = 1
				        and
				        	m.estuf = '{$_SESSION['par']['estuf']}'
				        {$group1}	
				        order by
				        	m.mundescricao,
				        	t.entnome";
		}else{
			if( $sesano ){
				$cabecalho = array("Ação","Escola","Código INEP","Qtd. de Salas", "Qtd de Alunos");
			} else {
				$cabecalho = array("Escola","Código INEP","Qtd. de Salas", "Qtd de Alunos");
			}
			
			if( ($frmid == FORMA_EXECUCAO_ASSITENCIA_TECNICA || $frmid == FORMA_EXECUCAO_EXECUTADA_PELO_MUNICIPIO || $frmid == FORMA_EXECUCAO_EXECUTADA_PELO_ESTADO ) && $sesano){
				array_push($cabecalho,'Quantidade');
				$qtd = ", '<input size=\"10\" id=\"sesquantidade_' || ses.sesid || '\"
							type=\"text\"
							onmouseover=\"MouseOver(this);\"
							onfocus=\"MouseClick(this);\"
							onmouseout=\"MouseOut(this);\"
							onblur=\"MouseBlur(this);\"
							onKeyUp=\"this.value=mascaraglobal(\'[#]\',this.value);\"
							onfocus=\"this.select()\"
							name=\"sesquantidade[{$sesano}]['|| ses.sesid ||']\"";
							if( ($estadoAtual['esdid'] == WF_SUBACAO_ANALISE || $estadoAtualSubacao["esdid"] == WF_SUBACAO_DILIGENCIA) && in_array(PAR_PERFIL_SUPER_USUARIO,$arrayPerfil) || in_array(PAR_PERFIL_ADMINISTRADOR,$arrayPerfil) || in_array(PAR_PERFIL_EQUIPE_TECNICA,$arrayPerfil) || in_array(PAR_PERFIL_EQUIPE_FINANCEIRA,$arrayPerfil) ){
								$qtd .= "' || 'disabled=disabled' || '";
							}
							$qtd .= " value=\"' || 
								case 
						    		when substring( trim(to_char(coalesce(sesquantidade, 0), '9999999990D99')), position(',' in trim(to_char(coalesce(sesquantidade, 0), '9999999990D99'))) +1, 2 ) = '00'
						    			then trim(to_char(coalesce(sesquantidade, 0), '999999999'))
						    		else trim(to_char(coalesce(sesquantidade, 0), '9999999990D99'))
						    	END
							|| '\" />' as sesqtd";
				if( ($estadoAtual['esdid'] == WF_SUBACAO_ANALISE || $estadoAtualSubacao["esdid"] == WF_SUBACAO_DILIGENCIA) ){ 
					array_push($cabecalho,'Quantidade Técnico');
					if( in_array(PAR_PERFIL_SUPER_USUARIO,$arrayPerfil) || 
						in_array(PAR_PERFIL_ADMINISTRADOR,$arrayPerfil) || 
						in_array(PAR_PERFIL_EQUIPE_TECNICA,$arrayPerfil) || 
						in_array(PAR_PERFIL_EQUIPE_FINANCEIRA,$arrayPerfil)){
						$qtd .= ", '<input size=\"10\" id=\"sesquantidadetecnico_' || ses.sesid || '\"
									type=\"text\"
									onmouseover=\"MouseOver(this);\"
									onfocus=\"MouseClick(this);\"
									onmouseout=\"MouseOut(this);\"
									onblur=\"MouseBlur(this);\"
									onKeyUp=\"this.value=mascaraglobal(\'[#]\',this.value);\"
									onfocus=\"this.select()\"
									name=\"sesquantidadetecnico[{$sesano}]['|| ses.sesid ||']\"
									' || CASE WHEN sesvalidatecnico = 'S' THEN '' ELSE 'disabled=disabled' END || '
									value=\"' || 
										case 
								    		when substring( trim(to_char(coalesce(sesquantidadetecnico, 0), '9999999990D99')), position(',' in trim(to_char(coalesce(sesquantidadetecnico, 0), '9999999990D99'))) +1, 2 ) = '00'
								    			then trim(to_char(coalesce(sesquantidadetecnico, 0), '999999999'))
								    		else trim(to_char(coalesce(sesquantidadetecnico, 0), '9999999990D99'))
								    	END
									|| '\" />' as sesqtdtecnico,
									'<center><input type=\"radio\" name=\"validaE[{$sesano}][' || ses.sesid || ']\" id=\"validaE[{$sesano}][' || ses.sesid || ']\" value=\"S\" ' || CASE WHEN sesvalidatecnico = 'S' THEN 'checked' ELSE '' END || ' onclick=\"validaEscola(this,$sbaid,$sesano,' || ses.sesid || ')\">Sim  <input type=\"radio\" name=\"validaE[{$sesano}][' || ses.sesid || ']\" id=\"validaE[{$sesano}][' || ses.sesid || ']\" value=\"N\" ' || CASE WHEN sesvalidatecnico = 'N' THEN 'checked' ELSE '' END || ' onclick=\"validaEscola(this,$sbaid,$sesano,' || ses.sesid || ')\">Não</center>' as escolaaprovada ";
					} else {
						$qtd .= ", '<input size=\"10\" id=\"sesquantidadetecnico_' || ses.sesid || '\"
									type=\"text\"
									onmouseover=\"MouseOver(this);\"
									onfocus=\"MouseClick(this);\"
									onmouseout=\"MouseOut(this);\"
									onblur=\"MouseBlur(this);\"
									onKeyUp=\"this.value=mascaraglobal(\'[#]\',this.value);\"
									onfocus=\"this.select()\"
									' || CASE WHEN sesvalidatecnico = 'S' THEN '' ELSE 'disabled=disabled' END || '
									name=\"sesquantidadetecnico[{$sesano}]['|| ses.sesid ||']\"
									disabled=disabled
									value=\"' || 
										case 
								    		when substring( trim(to_char(coalesce(sesquantidadetecnico, 0), '9999999990D99')), position(',' in trim(to_char(coalesce(sesquantidadetecnico, 0), '9999999990D99'))) +1, 2 ) = '00'
								    			then trim(to_char(coalesce(sesquantidadetecnico, 0), '999999999'))
								    		else trim(to_char(coalesce(sesquantidadetecnico, 0), '9999999990D99'))
								    	END
									|| '\" />' as sesqtdtecnico, 
									CASE WHEN sesvalidatecnico = 'S' THEN 'Sim' ELSE 'Não' END as escolaaprovada";
					} 
					array_push($cabecalho,'Escola Aprovada');
				}
			}

			$tcpid = $_SESSION['par']['muncod'] == '5300108' ? 1 : 3;
			$sql = "	select distinct
							{$acao}
							t.entnome,
							'<center><label style=\"color:#999999;\">'||t.entcodent||'</label></center>' as entcodent,
							COALESCE(ted.num_salas_existentes,0) as qtd_salas,
							ted.num_alunos_existentes as qtde_alunos
							{$qtd}
						from
							entidade.entidade t
						inner join 
							entidade.funcaoentidade f on f.entid = t.entid AND f.funid = 3
			            left join
			            	entidade.entidadedetalhe ed on t.entid = ed.entid
			            	and (
				                     entdreg_infantil_creche = '1' or
				                     entdreg_infantil_preescola = '1' or
				                     entdreg_fund_8_anos = '1' or
				                     entdreg_fund_9_anos = '1'
								)
			            inner join
			            	entidade.endereco d on t.entid = d.entid
			            inner join 
			            	par.escolas e on e.entid = t.entid
			            inner join 
			            	par.subacaoescolas ses on ses.escid = e.escid 
			            and 
			            	ses.sesstatus = 'A'
			            {$whereano}
				        and
				        	ses.sbaid = '$sbaid'
				        left join
							".SCHEMAEDUCACENSO.".tab_dado_escola ted on ted.fk_cod_entidade::character varying = t.entcodent::character varying
						--left join
						--	".SCHEMAEDUCACENSO.".tab_matricula mat on mat.fk_cod_entidade = ted.fk_cod_entidade
						where
							(t.entescolanova = false or t.entescolanova is null)
						AND 
							t.entstatus = 'A'
				        and
				            t.tpcid = {$tcpid}
				        and
				        	d.muncod = '{$_SESSION['par']['muncod']}'
				        {$group2}
				        order by
				        	t.entnome";	
		}
		//ver($sql, d);
		return $this->monta_lista_simples($sql,$cabecalho,50000,5,"S","100%",'N',true,false,false,true);
	}
	
	public function conta($sbaid,$sesano = null)
	{
		if( $sesano ){
			$whereano = "and ses.sesano = '$sesano'";
			$qtd = "ses.sesquantidade";
		} else {
			$qtd = "sum(ses.sesquantidade)";
			$group1 = "group by	t.entnome, t.entcodent, m.mundescricao";
			$group2 = "group by t.entnome, t.entcodent";
		}
		
		if($_SESSION['par']['itrid'] == 1){

			$sql = "	select
							count(t.entnome)
						from
							entidade.entidade t
						inner join 
							entidade.funcaoentidade f on f.entid = t.entid
			            left join
			            	entidade.entidadedetalhe ed on t.entid = ed.entid
			            inner join
			            	entidade.endereco d on t.entid = d.entid
			            left join 
			            	territorios.municipio m on m.muncod = d.muncod
			            inner join 
			            	par.escolas e on e.entid = t.entid
			            inner join 
			            	par.subacaoescolas ses on ses.escid = e.escid
			            and 
			            	ses.sesstatus = 'A'
			            {$whereano}
				        and
				        	ses.sbaid = '$sbaid'
						where
							(t.entescolanova = false or t.entescolanova is null)
						AND 
							t.entstatus = 'A'
				        and 
				        	f.funid = 3
				        and
				            t.tpcid = 1
				        and
				        	m.estuf = '{$_SESSION['par']['estuf']}'";
			            
		}else{
			$tcpid = $_SESSION['par']['muncod'] == '5300108' ? 1 : 3;
			$sql = "	select
							count(t.entnome)
						from
							entidade.entidade t
						inner join 
							entidade.funcaoentidade f on f.entid = t.entid
			            left join
			            	entidade.entidadedetalhe ed on t.entid = ed.entid
			            	and (
				                     entdreg_infantil_creche = '1' or
				                     entdreg_infantil_preescola = '1' or
				                     entdreg_fund_8_anos = '1' or
				                     entdreg_fund_9_anos = '1'
								)
			            inner join
			            	entidade.endereco d on t.entid = d.entid
			            inner join 
			            	par.escolas e on e.entid = t.entid
			            inner join 
			            	par.subacaoescolas ses on ses.escid = e.escid 
			            and 
			            	ses.sesstatus = 'A'
			            {$whereano}
				        and
				        	ses.sbaid = '$sbaid'
						where
							(t.entescolanova = false or t.entescolanova is null)
						AND 
							t.entstatus = 'A'
				        and
				            t.tpcid = {$tcpid}
				        and
				        	d.muncod = '{$_SESSION['par']['muncod']}'";	
		}
	//ver($sql);
		return $this->pegaUm($sql);
	}
	
	public function excluirEscola($sesid)
	{
		$sql = "update $this->stNomeTabela set sesstatus = 'I' where sesid = $sesid";
		$this->executar($sql);	
		return $this->commit($sql);
	}

	public function deletaPorPtoid($ptoid)
	{
		$this->executar("delete FROM par.subescolas_subitenscomposicao WHERE sesid IN ( SELECT sesid FROM par.subacaoescolas WHERE sbaid IN ( SELECT sbaid FROM par.subacao WHERE sbastatus = 'A' AND aciid IN (select aciid FROM par.acao WHERE acistatus = 'A' AND ptoid = {$ptoid})))");
		$this->executar("delete FROM par.subacaoescolas WHERE sbaid IN ( SELECT sbaid FROM par.subacao WHERE sbastatus = 'A' AND aciid in(select aciid FROM par.acao WHERE acistatus = 'A' AND ptoid = {$ptoid}))");
	}
	
	public function deletaPorSbaid($sbaid, $ano = array())
	{
		$anos = implode( $ano, "," );
		$this->executar("delete FROM par.subescolas_subitenscomposicao WHERE sesid IN ( SELECT sesid FROM par.subacaoescolas WHERE sbaid = {$sbaid} AND sesano IN ({$anos}) )");
		$this->executar("delete FROM par.subacaoescolas WHERE sbaid = {$sbaid} AND sesano IN ({$anos})");
	}
}									  