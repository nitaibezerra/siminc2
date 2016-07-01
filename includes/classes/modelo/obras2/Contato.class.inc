<?php
	
class Contato extends Modelo{
	
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "obras2.contato";	

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "cntid" );

    /**
     * Atributos
     * @var array
     * @access protected
     */    
    protected $arAtributos     = array(
									  	'cntid' => null, 
									  	'orgid' => null, 
									  	'usucpf' => null, 
									  	'estuf' => null, 
									  	'tooid' => null, 
									  	'cntstatus' => null, 
									  );

	public function listaResumoSql( Array $param = array() ){
		$where = array();
//		$join  = array();
    	
		if ( $param['orgid'] ){
			$param['orgid'] = (array) $param['orgid'];
			$where[] = "c.orgid IN(" . implode(", ", $param['orgid']) . ")";
		}		
		if ( $param['estuf'] ){
			$param['estuf'] = (array) $param['estuf'];
			$where[] = "c.estuf IN('" . implode("', '", $param['estuf']) . "')";
		}		
		
		if ( possui_perfil( array(PFLCOD_ADMINISTRADOR, PFLCOD_SUPER_USUARIO) ) ){
			$colNome = "'(' || REPLACE(TO_CHAR(TRIM(u.usucpf)::numeric, '000:000:000-00'),':', '.') || ') ' || u.usunome AS usunome";
		}else{
			$colNome = "u.usunome";
		}
		
		$sql = "SELECT
					DISTINCT
					{$colNome},
					CASE WHEN u.usufoneddd != '' THEN
						'(' || u.usufoneddd || ') ' || COALESCE(u.usufonenum, '')
					ELSE
						COALESCE(u.usufonenum, '')
					END AS usutelefone
				FROM
					obras2.contato c
				LEFT JOIN seguranca.usuario u ON u.usucpf = c.usucpf
				WHERE
					c.cntstatus = 'A'" .
					(count($where) ? " AND " . implode(' AND ',$where) : "");
		return $sql;
	}

	public function listaResumoSqlSemCPF( Array $param = array() ){
		$where = array();
    	
		if ( $param['orgid'] ){
			$param['orgid'] = (array) $param['orgid'];
			$where[] = "c.orgid IN(" . implode(", ", $param['orgid']) . ")";
		}		
		if ( $param['estuf'] ){
			$param['estuf'] = (array) $param['estuf'];
			$where[] = "c.estuf IN('" . implode("', '", $param['estuf']) . "')";
		}		
		
		$sql = "SELECT
					DISTINCT
					 u.usunome AS usunome,
					 u.usuemail as usuemail,
					CASE WHEN u.usufoneddd != '' THEN
						'(' || u.usufoneddd || ') ' || COALESCE(u.usufonenum, '')
					ELSE
						COALESCE(u.usufonenum, '')
					END AS usutelefone
				FROM
					obras2.contato c
				LEFT JOIN seguranca.usuario u ON u.usucpf = c.usucpf
				WHERE
					c.cntstatus = 'A'" .
					(count($where) ? " AND " . implode(' AND ',$where) : "");
		return $sql;
	}
        
        public function listaContatoFNDE(Array $param = array())
        {
		$where = array();
                
		if ( $param['estuf'] ){
			$param['estuf'] = (array) $param['estuf'];
			$where[] = "c.estuf IN('" . implode("', '", $param['estuf']) . "')";
		}		
		
		$sql = "SELECT
                            DISTINCT
                            u.usunome AS usunome, u.usuemail, 
                            CASE WHEN u.usufoneddd != '' THEN
                                   '(' || u.usufoneddd || ') ' || COALESCE(u.usufonenum, '')
                            ELSE
                                   COALESCE(u.usufonenum, '')
                            END AS usutelefone
                            FROM
                                    obras2.contato c
                            LEFT JOIN seguranca.usuario u ON u.usucpf = c.usucpf
                            WHERE
                                    c.cntstatus = 'A'" .
                                    (count($where) ? " AND " . implode(' AND ',$where) : "");
		return $sql;
	}
									  
	public function listaSql( Array $param = array() ){
		$where = array();
//		$join  = array();
    	
		if ( $param['orgid'] ){
			$param['orgid'] = (array) $param['orgid'];
			$where[] = "o.orgid IN(" . implode(", ", $param['orgid']) . ")";
		}

		$acao = "'<center>
					<img
	 					align=\"absmiddle\"
	 					src=\"/imagens/alterar.gif\"
	 					style=\"cursor: pointer\"
	 					onclick=\"javascript: alterarCont(\'' || c.cntid || '\');\"
	 					title=\"Alterar Contato\">
	 				<img
	 					align=\"absmiddle\"
	 					src=\"/imagens/excluir.gif\"
	 					style=\"cursor: pointer; margin-left: 3px;\"
	 					onclick=\"javascript: excluirCont(\'' || c.cntid || '\');\"
	 					title=\"Excluir Contato\">
	 			  </center>'";
		
		
		$sql = "SELECT
					$acao AS acao,
					'(' || REPLACE(TO_CHAR(TRIM(u.usucpf)::numeric, '000:000:000-00'),':', '.') || ') ' || u.usunome,
					o.orgdesc,
					e.estuf,
					t.toodescricao
				FROM
					obras2.contato c
				LEFT JOIN obras2.orgao o ON o.orgid = c.orgid AND
									   o.orgstatus = 'A'
				LEFT JOIN obras2.tipoorigemobra t ON t.tooid = c.tooid AND
												t.toostatus = 'A'
				LEFT JOIN territorios.estado e ON e.estuf = c.estuf
				LEFT JOIN seguranca.usuario u ON u.usucpf = c.usucpf
				WHERE
					c.cntstatus = 'A'" .
					(count($where) ? " AND " . implode(' AND ',$where) : "");
		return $sql;
	}
	
	public function ResponsavelObra($empid){
		
		if ( empty($empid) ){
			return array();
		}
		
		$sql = "select 
					'(' || REPLACE(TO_CHAR(TRIM(u.usucpf)::numeric, '000:000:000-00'),':', '.') || ' ) ' || u.usunome AS usunome, 
					u.usuemail, 
					CASE WHEN u.usufoneddd != '' THEN
						'(' || u.usufoneddd || ') ' || COALESCE(u.usufonenum, '')
					ELSE
						COALESCE(u.usufonenum, '')
					END AS usufonenum, 
					array_to_string(array(select pfldsc from seguranca.perfil where pflcod = ur.pflcod and pflstatus = 'A'),', br') AS usuperfil
				from obras2.usuarioresponsabilidade ur
					inner join seguranca.usuario u on u.usucpf = ur.usucpf
				where 
					ur.empid = {$empid}
					and ur.rpustatus = 'A'";
		
		return $sql;
	}

    public function ReponsavelObraEGestorUnidade($empid)
    {
        if (empty($empid)) {
            return array();
        }

        $sql = "select
					'(' || REPLACE(TO_CHAR(TRIM(u.usucpf)::numeric, '000:000:000-00'),':', '.') || ' ) ' || u.usunome AS usunome,
					u.usuemail,
					CASE WHEN u.usufoneddd != ''    THEN
						'(' || u.usufoneddd || ') ' || COALESCE(u.usufonenum, '')
					ELSE
						COALESCE(u.usufonenum, '')
					END AS usufonenum,
					array_to_string(array(select pfldsc from seguranca.perfil where pflcod = ur.pflcod and pflstatus = 'A'),', br') AS usuperfil,
                                        CASE WHEN us.suscod = 'A' THEN
                                            'Ativo'
                                        WHEN us.suscod = 'B' THEN
                                            'Bloqueado'
                                        WHEN us.suscod = 'P' THEN
                                            'Pendente'
                                        ELSE
                                            '-' END as sit_cadastro
                                        
				from obras2.usuarioresponsabilidade ur
					inner join seguranca.usuario u on u.usucpf = ur.usucpf
					inner join seguranca.usuario_sistema us on us.usucpf = u.usucpf and sisid = 147 and us.susstatus = 'A' and us.suscod = 'A'
					left join obras2.empreendimento e on e.empid = {$empid}
				where
					ur.rpustatus = 'A' AND
					u.suscod = 'A' AND
					--u.usustatus = 'A' AND
					( (ur.empid = {$empid}) or (ur.pflcod = '".PFLCOD_GESTOR_UNIDADE."' and ur.entid = e.entidunidade) )
					";
        return $sql;
    }
    public function ReponsavelObraEGestorUnidadeGestorEmpresaMI($empid)
    {
        if (empty($empid)) {
            return array();
        }

        $sql = "select
					'(' || REPLACE(TO_CHAR(TRIM(u.usucpf)::numeric, '000:000:000-00'),':', '.') || ' ) ' || u.usunome AS usunome,
					u.usuemail,
					CASE WHEN u.usufoneddd != ''    THEN
						'(' || u.usufoneddd || ') ' || COALESCE(u.usufonenum, '')
					ELSE
						COALESCE(u.usufonenum, '')
					END AS usufonenum,
					array_to_string(array(select pfldsc from seguranca.perfil where pflcod = ur.pflcod and pflstatus = 'A'),', br') AS usuperfil,
                                        CASE WHEN us.suscod = 'A' THEN
                                            'Ativo'
                                        WHEN us.suscod = 'B' THEN
                                            'Bloqueado'
                                        WHEN us.suscod = 'P' THEN
                                            'Pendente'
                                        ELSE
                                            '-' END as sit_cadastro
                                        
				from obras2.usuarioresponsabilidade ur
					inner join seguranca.usuario u on u.usucpf = ur.usucpf
					inner join seguranca.usuario_sistema us on us.usucpf = u.usucpf and sisid = 147 and us.susstatus = 'A' and us.suscod = 'A'
					left join obras2.empreendimento e on e.empid = {$empid}
				where
					ur.rpustatus = 'A' AND
					u.suscod = 'A' AND
					--u.usustatus = 'A' AND
					( (ur.empid = {$empid}) or (ur.pflcod IN ('".PFLCOD_GESTOR_UNIDADE."', '".PFLCOD_EMPRESA_MI_GESTOR."') and ur.entid = e.entidunidade) )
					";
        return $sql;
    }

    public function pegaEmailResponsaveisObraCumprimento($empid)
    {
        $sql = "
            SELECT
                u.usuemail
            FROM obras2.usuarioresponsabilidade ur
            INNER JOIN seguranca.usuario u ON u.usucpf = ur.usucpf
            INNER JOIN seguranca.usuario_sistema us ON us.usucpf = u.usucpf AND sisid = 147 AND us.susstatus = 'A' AND us.suscod = 'A'
            LEFT  JOIN obras2.empreendimento e ON e.empid = $empid
            WHERE ur.rpustatus = 'A'
                AND u.suscod = 'A'
                AND ((ur.empid = $empid) OR (ur.pflcod = '".PFLCOD_GESTOR_UNIDADE."' and ur.entid = e.entidunidade));
        ";
        return $this->carregar($sql);
    }


}