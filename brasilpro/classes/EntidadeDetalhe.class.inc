<?php
	
class EntidadeDetalhe extends Modelo{
	
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "entidade.entidadedetalhe";	

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "" );

    /**
     * Atributos
     * @var array
     * @access protected
     */    
    protected $arAtributos     = array(
									  	'entcodent' => null, 
									  	'entdreg_infantil_creche' => null, 
									  	'entdreg_infantil_preescola' => null, 
									  	'entdreg_fund_8_anos' => null, 
									  	'entdreg_fund_9_anos' => null, 
									  	'entdreg_medio_medio' => null, 
									  	'entdreg_medio_integrado' => null, 
									  	'entdreg_medio_normal' => null, 
									  	'entdreg_medio_prof' => null, 
									  	'entdesp_infantil_creche' => null, 
									  	'entdesp_infantil_preescola' => null, 
									  	'entdesp_fund_8_anos' => null, 
									  	'entdesp_fund_9_anos' => null, 
									  	'entdesp_medio_medio' => null, 
									  	'entdesp_medio_integrado' => null, 
									  	'entdesp_medio_normal' => null, 
									  	'entdesp_eja_fundamental' => null, 
									  	'entdesp_eja_medio' => null, 
									  	'entdeja_fundamental' => null, 
									  	'entdeja_medio' => null, 
									  	'entdmaterial_esp_etnico' => null, 
									  	'entdeducacao_indigena' => null, 
									  	'entdlingua_portuguesa' => null, 
									  	'entdnum_alunos_ed_comp_escola' => null, 
									  	'entdnum_alunos_ed_comp_outra' => null, 
									  	'entdnum_funcionarios' => null, 
									  	'entdnum_salas_existentes' => null, 
									  	'entdnum_salas_utilizadas' => null, 
									  	'entdnum_comp_administrativos' => null, 
									  	'num_comp_alunos' => null, 
									  	'num_alunos_atend_escola' => null, 
									  	'num_alunos_atend_outra_escola' => null, 
									  	'id_lingua_indigena' => null, 
									  	'id_mod_ens_regular' => null, 
									  	'id_esp_medio_profissional' => null, 
									  	'entid' => null, 
									  	'entdnum_enem_media_total_2006' => null, 
									  	'entdnum_medio_abandono_2005' => null, 
									  	'entdoferta_ept' => null, 
									  	'entdinfept' => null, 
									  	'entpdeescola' => null, 
									  	'entpdevlrpaf' => null, 
									  	'entdstatusept' => null, 
									  	'entddataprevisaoept' => null, 
									  );
									  
									  
	public function recuperarEscolasAtendidas( $inuid ){

		$sql = "select  iue.entid, '[' || e.entcodent || '] ' || e.entnome as entnome, mu.muncod, mu.mundescricao, 
						entdoferta_ept, entdstatusept, entddataprevisaoept
			    from cte.instrumentounidadeescola iue
					inner join entidade.entidade e on e.entid = iue.entid
					INNER JOIN entidade.funcaoentidade fe ON fe.entid = e.entid
			        inner join entidade.endereco ende on ende.entid = e.entid
			        inner join territorios.municipio mu on mu.muncod = ende.muncod
			        left join entidade.entidadedetalhe edd on e.entid = edd.entid
			    where iue.inuid = $inuid
				and entproep = false  
				AND e.tpcid = 1 
				AND fe.funid = 3 
				
			    group by iue.entid, e.entcodent, e.entnome, mu.muncod, mu.mundescricao, entdoferta_ept, entdstatusept, entddataprevisaoept
			    order by mu.mundescricao ASC";

		$resultado = $this->carregar( $sql );
		
		return $resultado ? $resultado : array();
		
	}
	
	public function existePendenciaEPTPorInuid( $inuid ){
		
		$coEntidadeDetalhe = $this->recuperarEscolasAtendidas( $inuid );
		
		$arPendencias = array();
		foreach( $coEntidadeDetalhe as $arEntidadeDetalhe ){
			
			if( $arEntidadeDetalhe["entdoferta_ept"] == "f" ) continue;
			if( $arEntidadeDetalhe["entdoferta_ept"] == "t" && $arEntidadeDetalhe["entdstatusept"] == "F" ) continue;
			if( $arEntidadeDetalhe["entdoferta_ept"] == "t" && $arEntidadeDetalhe["entdstatusept"] == "P" && $arEntidadeDetalhe["entddataprevisaoept"] ) continue;

			return true;
//			$arPendencias[] = $arEntidadeDetalhe["entnome"];

			
		}
		return false;
//		dbg( $arPendencias );
//		dbg( $coEntidadeDetalhe );
	}	
}