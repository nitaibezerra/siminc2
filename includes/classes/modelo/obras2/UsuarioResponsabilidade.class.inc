<?php
	
class UsuarioResponsabilidade extends Modelo{
	
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "obras2.usuarioresponsabilidade";	

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "rpuid" );

    /**
     * Atributos
     * @var array
     * @access protected
     */    
    protected $arAtributos     = array(
                                        'rpuid' => null, 
                                        'usucpf' => null, 
                                        'rpustatus' => null, 
                                        'rpudata_inc' => null, 
                                        'pflcod' => null, 
                                        'estuf' => null, 
                                        'entid' => null, 
                                        'orgid' => null, 
                                        'obrid' => null, 
                                        'empid' => null 
                                      );

	public static $arEmpidPermitido;									  
	public static $arOrgidPermitido;									  
	public static $arEntidPermitido;									  

	public function pegaEmpidPermitido( $usucpf, array $arPflcod = array() ){
		$sql = "SELECT
					empid
				FROM
					obras2.usuarioresponsabilidade
				WHERE
					rpustatus = 'A' AND
					empid IS NOT NULL AND
					" . ($arPflcod ? " pflcod IN (" . implode(", ", $arPflcod) . ") AND " : "") . "
					usucpf = '{$usucpf}'";
		
		if ( empty( self::$arEmpidPermitido ) ){
			$dado = $this->carregarColuna( $sql );
			self::$arEmpidPermitido = $dado;
		}else{
			$dado = self::$arEmpidPermitido;
		}
		
		return ($dado ? $dado : array());
	}
	
	public function pegaOrgidPermitido( $usucpf ){
		if ( $this->testa_superuser() || possui_perfil( PFLCOD_GESTOR_CONTRATO_SUPERVISAO_MEC ) ){
			$sql = "SELECT
						orgid			
					FROM
						obras2.orgao
					WHERE
						orgstatus = 'A'
					ORDER BY
						orgid";
		}else{
			
			$arEmpid = $this->pegaEmpidPermitido( $usucpf, array(PFLCOD_SUPERVISOR_UNIDADE, PFLCOD_GESTOR_CONTRATO_SUPERVISAO_MEC) );
			$arEmpid = ($arEmpid ? $arEmpid : array(0));
			
			$sql = "	-- EMPRESA MI
						SELECT
							DISTINCT
							e.orgid
						FROM
							obras2.obras o
						INNER JOIN entidade.endereco             ende ON ende.endid = o.endid    AND ende.endstatus = 'A' AND ende.tpeid = ".TIPO_ENDERECO_OBJETO."
						INNER JOIN obras2.empresami_uf 	          euf ON  euf.estuf = ende.estuf AND euf.eufstatus = 'A' 
						INNER JOIN obras2.usuarioresponsabilidade urs ON  urs.emiid = euf.emiid  AND usucpf = '{$usucpf}' AND urs.rpustatus = 'A'
						INNER JOIN obras2.empreendimento            e ON    e.empid = o.empid    AND e.empstatus = 'A'
					UNION 
						-- PELO ORGÃO
						SELECT
							orgid
						FROM
							obras2.usuarioresponsabilidade
						WHERE
							rpustatus = 'A' AND
							orgid IS NOT NULL AND
							usucpf = '{$usucpf}'
					UNION
						-- PELA ENTIDADE (UNIDADE) SUPERVISOR UNIDADE
						SELECT DISTINCT
							e.orgid
						FROM
							obras2.usuarioresponsabilidade u
						JOIN obras2.empreendimento e ON e.entidunidade = u.entid
						WHERE
							rpustatus = 'A' AND
							u.entid IS NOT NULL AND
							u.pflcod IN (" . PFLCOD_SUPERVISOR_UNIDADE . ") AND
							e.empid IN (" . implode(", ", $arEmpid) . ") AND
							usucpf = '{$usucpf}'
					UNION
						-- PELA ENTIDADE (UNIDADE)
						SELECT DISTINCT
							e.orgid
						FROM
							obras2.usuarioresponsabilidade u
						JOIN obras2.empreendimento e ON e.entidunidade = u.entid
						WHERE
							rpustatus = 'A' AND
							u.entid IS NOT NULL AND
							u.pflcod NOT IN (" . PFLCOD_SUPERVISOR_UNIDADE . ") AND
							usucpf = '{$usucpf}'
					UNION
						-- PELA ENTIDADE (EMPRESA)
						SELECT DISTINCT
							e.orgid
						FROM
							obras2.usuarioresponsabilidade u
						JOIN obras2.supervisao_grupo_empresa sge ON sge.entid = u.entid   AND sge.sgestatus != 'I'
                                                JOIN obras2.supervisao_os             so ON so.sgeid  = sge.sgeid AND so.sosstatus = 'A'
						JOIN obras2.supervisao_os_obra       soo ON soo.sosid = so.sosid  AND soo.soostatus = 'A'
						JOIN obras2.empreendimento             e ON e.empid   = soo.empid
						WHERE
							rpustatus = 'A' AND
							pflcod IN (" . PFLCOD_EMPRESA_VISTORIADORA_FISCAL . ', ' . PFLCOD_EMPRESA_VISTORIADORA_GESTOR . ") AND
							u.entid IS NOT NULL AND
							u.usucpf = '{$usucpf}'
					UNION
						-- PELO EMPREENDIMENTO (EMPREENDIMENTO)
						SELECT DISTINCT
							e.orgid
						FROM
							obras2.usuarioresponsabilidade u
						JOIN obras2.empreendimento e ON e.empid = u.empid
						WHERE
							rpustatus = 'A' AND
							u.empid IS NOT NULL AND
							usucpf = '{$usucpf}'";
		}
		
		if ( empty( self::$arOrgidPermitido ) ){
			$dado = $this->carregarColuna( $sql );
			self::$arOrgidPermitido = $dado;
		}else{
			$dado = self::$arOrgidPermitido;
		}
		
		return ($dado ? $dado : array());
	}
	
	public function pegaEmpresaPermitida( $usucpf ){

		if ( $this->testa_superuser() || possui_perfil(array(PFLCOD_GESTOR_CONTRATO_SUPERVISAO_MEC))  ){
			$sql = "SELECT DISTINCT
						entid			
					FROM
						obras2.usuarioresponsabilidade
					WHERE
						rpustatus = 'A' AND
						pflcod IN (" . PFLCOD_EMPRESA_VISTORIADORA_GESTOR . ", " . PFLCOD_EMPRESA_VISTORIADORA_FISCAL . ")
					ORDER BY
						entid";
		}else{
			$sql = "SELECT
						entid			
					FROM
						obras2.usuarioresponsabilidade
					WHERE
						rpustatus = 'A' AND
						pflcod IN (" . PFLCOD_EMPRESA_VISTORIADORA_GESTOR . ", " . PFLCOD_EMPRESA_VISTORIADORA_FISCAL . ") AND
						usucpf = '{$usucpf}'
					ORDER BY
						entid";
		}

		if ( empty( self::$arEntidPermitido ) ){
			$dado = $this->carregarColuna( $sql );
			self::$arEntidPermitido = $dado;
		}else{
			$dado = self::$arEntidPermitido;
		}

		return ($dado ? $dado : array());
	}
	
	public function pegaSgeidEmpresaPermitida( $usucpf ){
		
		if ( $this->testa_superuser() ){
			$sql = "SELECT DISTINCT
						sge.sgeid			
					FROM
						obras2.supervisao_grupo_empresa sge 
					WHERE
						sge.sgestatus != 'I'
					ORDER BY
						sge.sgeid";
		}else{
			$sql = "SELECT
						sge.sgeid			
					FROM
						obras2.usuarioresponsabilidade usr
					INNER JOIN
						obras2.supervisao_grupo_empresa sge ON sge.entid = usr.entid 
					WHERE
						rpustatus = 'A' AND
						pflcod IN (" . PFLCOD_EMPRESA_VISTORIADORA_GESTOR . ", " . PFLCOD_EMPRESA_VISTORIADORA_FISCAL . ") AND
						usucpf = '{$usucpf}'
					ORDER BY
						sge.sgeid";
		}
		
		$dado = $this->carregarColuna( $sql );
		
		return ($dado ? $dado : array());
	}

	public function pegaUsuarioPorEntidAndPflcod( $entid, $pflcod = null ){
		$arWhere   = array();
		$arWhere[] = "r.rpustatus = 'A'";
		
		$entid 	   = (array) $entid;
		$arWhere[] = "r.entid IN(" . implode(", ", $entid) . ")";
		
		if ( !empty( $pflcod ) ){
			$pflcod    = (array) $pflcod;
			$arWhere[] = "r.pflcod IN (" . implode(", ", $pflcod) . ")";
		}
		
		$where = (count($arWhere) ? implode(' AND ',$arWhere) : "");
		
		$sql = "SELECT
					DISTINCT
					u.*
				FROM
					obras2.usuarioresponsabilidade r	
				JOIN seguranca.usuario u ON u.usucpf = r.usucpf
				WHERE
					{$where}";
		$arUser = $this->carregar( $sql );
		
		return ($arUser ? $arUser : array());
	}

	public function pegaUsuarioPorPerfil( $pflcod ){
		$sql = "SELECT
					DISTINCT
					u.usucpf,
					u.usunome,
					u.usuemail
				FROM
					seguranca.usuario u
				JOIN seguranca.perfilusuario pu ON pu.usucpf = u.usucpf AND
												   pu.pflcod = {$pflcod}";
		
		$dado = $this->carregar( $sql );
		
		return ($dado ? $dado : array());
	}
}