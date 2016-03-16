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
    									'sesstatus' => null
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

	public function montaLista($sbaid, $sesano = null, $frmid=null){
		if( $sesano ){
			$whereano = "and ses.sesano = '$sesano'";
			$acao  = "'<center><img class=\"middle link\" title=\"Excluir Escola\" src=\"../imagens/excluir.gif\" onclick=\"excluirEscola(this,\'' || ses.sesid || '\')\"  /></center>' as acao,";
			$group1 = "group by	t.entnome, t.entcodent, m.mundescricao, ses.sesid, ses.sesquantidade";
			$group2 = "group by t.entnome, t.entcodent, ses.sesid, ses.sesquantidade";
			//$qtd = ",ses.sesquantidade";
		} else {
			$qtd = ",sum(ses.sesquantidade)";
			$group1 = "group by	t.entnome, t.entcodent, m.mundescricao, ses.sesquantidade";
			$group2 = "group by t.entnome, t.entcodent, ses.sesquantidade";
		}

		if($_SESSION['par']['itrid'] == 1){
			if( $sesano ){
				$cabecalho = array("Ação","Município","Escola","Código INEP");
			} else {
				$cabecalho = array("Município","Escola","Código INEP");
			}
			
			if($frmid == FORMA_EXECUCAO_ASSITENCIA_TECNICA){ 
				array_push($cabecalho,'Quantidade');
				$qtd = ", '<input size=\"15\" id=\"sesquantidade_' || ses.sesid || '\"
								type=\"text\"
								onmouseover=\"MouseOver(this);\"
								onfocus=\"MouseClick(this);\"
								onmouseout=\"MouseOut(this);\"
								onblur=\"MouseBlur(this);\"
								onfocus=\"this.select()\"
								name=\"sesquantidade[{$sesano}]['|| ses.sesid ||']\"
								value=\"' || 
									case 
							    		when substring( trim(to_char(coalesce(sesquantidade, 0), '9999999990D99')), position(',' in trim(to_char(coalesce(sesquantidade, 0), '9999999990D99'))) +1, 2 ) = '00'
							    			then trim(to_char(coalesce(sesquantidade, 0), '999999999'))
							    		else trim(to_char(coalesce(sesquantidade, 0), '9999999990D99'))
							    	END
								|| '\" />' as sesqtd";
			}
			
			$sql = "	select distinct
							{$acao}
							m.mundescricao,
							t.entnome,
							t.entcodent
							{$qtd}
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
				        	m.estuf = '{$_SESSION['par']['estuf']}'
				        {$group1}	
				        order by
				        	t.entnome,
				        	m.mundescricao";
		}else{
			if( $sesano ){
				$cabecalho = array("Ação","Escola","Código INEP");
			} else {
				$cabecalho = array("Escola","Código INEP");
			}
			
			if( $frmid == FORMA_EXECUCAO_ASSITENCIA_TECNICA && $sesano ){
				array_push($cabecalho,'Quantidade');
				$qtd = ", '<input size=\"15\" id=\"sesquantidade_' || ses.sesid || '\"
								type=\"text\"
								onmouseover=\"MouseOver(this);\"
								onfocus=\"MouseClick(this);\"
								onmouseout=\"MouseOut(this);\"
								onblur=\"MouseBlur(this);\"
								onfocus=\"this.select()\"
								name=\"sesquantidade[{$sesano}]['|| ses.sesid ||']\"
								value=\"' || 
									case 
							    		when substring( trim(to_char(coalesce(sesquantidade, 0), '9999999990D99')), position(',' in trim(to_char(coalesce(sesquantidade, 0), '9999999990D99'))) +1, 2 ) = '00'
							    			then trim(to_char(coalesce(sesquantidade, 0), '999999999'))
							    		else trim(to_char(coalesce(sesquantidade, 0), '9999999990D99'))
							    	END
								|| '\" />' as sesqtd";
			}
			$tcpid = $_SESSION['par']['muncod'] == '5300108' ? 1 : 3;
			$sql = "	select distinct
							{$acao}
							t.entnome,
							t.entcodent
							{$qtd}
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
				        	d.muncod = '{$_SESSION['par']['muncod']}'
				        {$group2}
				        order by
				        	t.entnome";	
		}
		//ver($sql);
		return $this->monta_lista_simples($sql,$cabecalho,100,5,"N","100%");
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
		$this->executar("delete FROM par.subacaoescolas WHERE sbaid IN ( SELECT sbaid FROM par.subacao WHERE sbastatus = 'A' AND aciid in(select aciid FROM par.acao WHERE acistatus = 'A' AND ptoid = {$ptoid}))");
	}
	
	public function deletaPorSbaid($sbaid, $ano = array())
	{
		$anos = implode( $ano, "," );
		$this->executar("delete FROM par.subacaoescolas WHERE sbaid = {$sbaid} AND sesano IN ({$anos})");
	}
}									  