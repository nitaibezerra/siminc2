<?php
	
class Supervisao_Grupo_Empresa extends Modelo{
	
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "obras2.supervisao_grupo_empresa";	

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "sgeid" );

    /**
     * Atributos
     * @var array
     * @access protected
     */    
    protected $arAtributos     = array(
									  	'sgeid' => null, 
									  	'sgrid' => null, 
									  	'entid' => null, 
									  	'sgevalorunitario' => null, 
									  	'sgevalora' => null, 
									  	'sgevalorb' => null, 
									  	'sgevalorc' => null, 
									  	'sgestatus' => null, 
									  	'sgevalortotal' => null, 
									  	'sgedtinicio' => null, 
									  	'sgedttermino' => null, 
									  	'sgenumcontrato' => null, 
									  );

	public function pegaSgeid( Array $param = array()){
		$arWhere = array();
		
		if ( !empty( $param['sgrid'] ) ){
			$param['sgrid'] = (array) $param['sgrid'];
			$arWhere[] 		= "sge.sgrid IN(" . implode(", ", $param['sgrid']) . ")";
		}
		if ( !empty( $param['entid'] ) ){
			$param['entid'] = (array) $param['entid'];
			$arWhere[] 		= "sge.entid IN(" . implode(", ", $param['entid']) . ")";
		}
		
		$where = (count($arWhere) ? " AND " . implode(' AND ',$arWhere) : "");
		$sql = <<<EOD
	 			SELECT DISTINCT 
			   		sge.sgeid 
			    FROM 
					obras2.supervisao_grupo_empresa sge
				WHERE
					sge.sgestatus != 'I'
					$where
			    ORDER BY 
					sgeid
EOD;
		$dados = $this->carregarColuna($sql);
		
		return ($dados ? $dados : array());
	}
	
	public function pegaSgrid( Array $param = array()){
		$arWhere = array();
		
		if ( !empty( $param['sgeid'] ) ){
			$param['sgeid'] = (array) $param['sgrid'];
			$arWhere[] 		= "sge.sgeid IN(" . implode(", ", $param['sgeid']) . ")";
		}
		
		if ( !empty( $param['entid'] ) ){
			$param['entid'] = (array) $param['entid'];
			$arWhere[] 		= "sge.entid IN(" . implode(", ", $param['entid']) . ")";
		}
		
		$where = (count($arWhere) ? " AND " . implode(' AND ',$arWhere) : "");
		$sql = <<<EOD
	 			SELECT DISTINCT 
			   		sge.sgrid 
			    FROM 
					obras2.supervisao_grupo_empresa sge
				WHERE
					sge.sgestatus='A'
					$where
			    ORDER BY 
					sgeid
EOD;
		$dados = $this->carregarColuna($sql);
		
		return ($dados ? $dados : array());
	}

    public function listComboEmpresas(){

        if ( possui_perfil(array(PFLCOD_EMPRESA_VISTORIADORA_FISCAL, PFLCOD_EMPRESA_VISTORIADORA_GESTOR)) ){
            $usuarioResp = new UsuarioResponsabilidade();
            $arEntidEmpresa = $usuarioResp->pegaEmpresaPermitida($_SESSION['usucpf']);
            $arWhere[] = "e.entid IN (".implode(',',$arEntidEmpresa).")";
        }
        $where = (count($arWhere) ? " AND " . implode(' AND ',$arWhere) : "");

        $sql = <<<EOD
                SELECT
                  e.entid AS codigo,
                  to_char(e.entnumcpfcnpj::int8,'00"."000"."000"/"0000"-"00') || ' - ' || e.entnome AS descricao
                FROM
                  obras2.supervisao_grupo_empresa sge
                  JOIN entidade.entidade e ON e.entid = sge.entid
                WHERE
                  sgestatus='A'
                  $where
                GROUP BY e.entid,e.entnome,e.entnumcpfcnpj;

EOD;
        $dados = $this->carregar($sql);

        return ($dados ? $dados : array());
    }

	public function listaCombo( Array $param = array()){
		$arWhere = array();
		
		if ( !empty( $param['sgrid'] ) ){
			$arWhere[] = "sge.sgrid = {$param['sgrid']}";
		}

        if ( possui_perfil(array(PFLCOD_EMPRESA_VISTORIADORA_FISCAL, PFLCOD_EMPRESA_VISTORIADORA_GESTOR)) ){
            $usuarioResp = new UsuarioResponsabilidade();
            $arEntidEmpresa = $usuarioResp->pegaEmpresaPermitida($_SESSION['usucpf']);
            $arWhere[] = "e.entid IN (".implode(',',$arEntidEmpresa).")";
        }

		$where = (count($arWhere) ? " AND " . implode(' AND ',$arWhere) : "");
		$sql = <<<EOD
	 			SELECT 
			   		sge.sgeid AS codigo, 
			   		to_char(e.entnumcpfcnpj::int8,'00"."000"."000"/"0000"-"00') || ' - ' || e.entnome || CASE WHEN sgestatus IN ('U') THEN ' (valores antigos)' ELSE ' (valores novo)' END AS descricao
			    FROM 
					obras2.supervisao_grupo_empresa sge
				JOIN entidade.entidade e ON e.entid = sge.entid
				WHERE
					sgestatus IN ('A', 'U')
					$where
			    ORDER BY 
					entnome
EOD;
		$dados = $this->carregar($sql);

		return ($dados ? $dados : array());
	}
	
        public function listaComboEntidades( Array $param = array()){
		$arWhere = array();
		
		if ( !empty( $param['sgrid'] ) ){
			$arWhere[] = "sge.sgrid = {$param['sgrid']}";
		}

        if ( possui_perfil(array(PFLCOD_EMPRESA_VISTORIADORA_FISCAL, PFLCOD_EMPRESA_VISTORIADORA_GESTOR)) ){
            $usuarioResp = new UsuarioResponsabilidade();
            $arEntidEmpresa = $usuarioResp->pegaEmpresaPermitida($_SESSION['usucpf']);
            if(!$arEntidEmpresa)
                return array();

            $arWhere[] = "e.entid IN (".implode(',',$arEntidEmpresa).")";
        }

		$where = (count($arWhere) ? " AND " . implode(' AND ',$arWhere) : "");
		$sql = "
	 			SELECT 
                                    DISTINCT ON (e.entid) e.entid AS codigo, 
			   		to_char(e.entnumcpfcnpj::int8,'00\".\"000\".\"000\"/\"0000\"-\"00') || ' - ' || e.entnome AS descricao
			    FROM 
					obras2.supervisao_grupo_empresa sge
				JOIN entidade.entidade e ON e.entid = sge.entid
				WHERE
					sgestatus='A'
					$where
			    ORDER BY  e.entid, entnome
            ";
		$dados = $this->carregar($sql);

		return ($dados ? $dados : array());
	}
        
	public function pegaGrupoIdPorSgeid( $sgeid ){
		$sql = "SELECT
					sgrid
				FROM
					obras2.supervisao_grupo_empresa
				WHERE
					sgeid = {$sgeid}";
		
		return $this->pegaUm( $sql );
	}

    public function pegaValoroPorSgeid( $sgeid ){
        $sql = "SELECT
					sgevalora, sgevalorb
				FROM
					obras2.supervisao_grupo_empresa
				WHERE
					sgeid = {$sgeid}";

        return $this->pegaLinha( $sql );
    }


    public function pegaValorUnitarioPorSgeid( $sgeid ){
		$sql = "SELECT
					sgevalorunitario
				FROM
					obras2.supervisao_grupo_empresa
				WHERE
					sgeid = {$sgeid}";
		return $this->pegaUm( $sql );
	}
    
	public function pegaValorUnitariobPorSgeid( $sgeid ){
		$sql = "SELECT
					sgevalorb
				FROM
					obras2.supervisao_grupo_empresa
				WHERE
					sgeid = {$sgeid}";
		return $this->pegaUm( $sql );
	}
	
	public function pegaEmpresaPorSgeid( $sgeid ){
		$sql = "SELECT
					to_char(ent.entnumcpfcnpj::int8,'00\".\"000\".\"000\"/\"0000\"-\"00') || ' - ' || ent.entnome as empresa 
				FROM
					obras2.supervisao_grupo_empresa sge
				INNER JOIN
					entidade.entidade ent ON ent.entid = sge.entid					
				WHERE
					sge.sgeid = {$sgeid}";
		
		return $this->pegaUm( $sql );
	}

	public function pegaEmpresaPorSosid( $sosid ){
		$sql = "SELECT
					entid
				FROM
					obras2.supervisao_os os
				JOIN obras2.supervisao_grupo_empresa se ON se.sgeid = os.sgeid AND
														   se.sgestatus = 'A'
				WHERE
					sosstatus = 'A' AND
					sosid = {$sosid}";
		
		$entid = $this->pegaUm( $sql );
		return $entid;
	}
	
	public function pegaNomeEmpresaPorSosid( $sosid ){
		$sql = "SELECT
					entnome
				FROM
					obras2.supervisao_os os
				JOIN obras2.supervisao_grupo_empresa se ON se.sgeid = os.sgeid AND
														   se.sgestatus = 'A'
				JOIN entidade.entidade e ON e.entid = se.entid														   
				WHERE
					sosstatus = 'A' AND
					sosid = {$sosid}";
		
		$entid = $this->pegaUm( $sql );
		return $entid;
	}

    public function getContatosPorSosid($sosid)
    {
        $sql = "
            SELECT
                DISTINCT
                    e.entid,
                    e.entnome,
                    '(' || e.entnumdddcomercial || ') ' || e.entnumcomercial as telefone,
                    ge.sgeid,
                    u.usunome
                FROM
                    obras2.tecnico_empresa tem
                INNER JOIN
                    obras2.supervisao_grupo_empresa ge ON ge.sgeid = tem.sgeid AND ge.sgestatus = 'A'
                INNER JOIN
                    obras2.supervisao_os os ON os.sgeid = ge.sgeid AND os.sosid = $sosid
                INNER JOIN
                    seguranca.usuario u ON u.usucpf = replace(replace(tem.temcpf, '.', ''), '-', '')
                INNER JOIN
                    seguranca.perfilusuario p ON p.usucpf = u.usucpf
                INNER JOIN
                    entidade.entidade e ON e.entid = ge.entid
                WHERE temvigente = 'A' AND p.pflcod IN (910)
        ";

        return $this->carregar($sql);
    }
}