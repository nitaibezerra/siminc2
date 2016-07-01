<?php
	
class Tecnico_Empresa extends Modelo{
	
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "obras2.tecnico_empresa";	

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "temid" );

    /**
     * Atributos
     * @var array
     * @access protected
     */    
    protected $arAtributos     = array(
									  	'temid' => null, 
									  	'sgeid' => null, 
									  	'temcpf' => null, 
									  	'temnome' => null, 
    									'temcargo' => null,
									  	'temnumcreacau' => null, 
									  	'temdtcadastro' => null, 
									  	'temdtcapacitacao' => null, 
									  	'temvigente' => null,
									  	'temstatus' => null
									  );

	public function listaSql( array $arWhere )
	{
//		ver($arWhere, d);
		$where = array("temstatus = 'A'");

		if( !possui_perfil(array(PFLCOD_SUPER_USUARIO)) ){
			$usuarioResp = new UsuarioResponsabilidade();
//			$arEntidEmpresa = $usuarioResp->pegaEmpresaPermitida( $_SESSION['usucpf'] );
//			if ( count($arEntidEmpresa) ){
//				$grupoEmpresa  = new Supervisao_Grupo_Empresa();
//				$param = array('sgrid' => $sgrid,
//							   'entid' => $arEntidEmpresa);
//				foreach($grupoEmpresa->pegaSgeid( $param ) as $grupo){
//					$arSgrid[] = $grupo;
//				}
//			}
			$arSgeid = $usuarioResp->pegaSgeidEmpresaPermitida( $_SESSION['usucpf'] );
		}
		if($arSgrid){
			$where[] = "gr.sgrid in (".implode(",",$arSgrid).")";
		}
		if($arSgeid){
			$where[] = "ge.sgeid in (".implode(",",$arSgeid).")";
		}
		if($arWhere['sgrid']){
			$where[] = "gr.sgrid = {$arWhere['sgrid']} ";
		}
        if($arWhere['entid']){
			$where[] = "e.entid = {$arWhere['entid']} ";
		}
		if($arWhere['sgeid']){
			$where[] = "ge.sgeid = {$arWhere['sgeid']} ";
		}
		if($arWhere['temcpf']){
			$where[] = "temcpf::text ilike ('%{$arWhere['temcpf']}%') ";
		}
		if($arWhere['temnome']){
			$where[] = "removeacento(temnome) ilike(removeacento('%{$arWhere['temnome']}%')) ";
		}
		if($arWhere['temcargo']){
			$where[] = "removeacento(temcargo) ilike(removeacento('%{$arWhere['temcargo']}%')) ";
		}
		if($arWhere['temnumcreacau']){
			$where[] = "temnumcreacau ilike ('%{$arWhere['temnumcreacau']}%') ";
		}
		if($arWhere['temdtcapacitacao']){
			$where[] = "temdtcapacitacao = '".formata_data_sql($arWhere['temdtcapacitacao'])."' ";
		}
		if($arWhere['temvigente']){
			$where[] = "temvigente = '{$arWhere['temvigente']}' ";
		}else{
			$where[] = "temvigente = 'A'";
		}
		
		$sql_where = $where ? " AND " . implode(" AND ", $where) : "";
		
		$imgDesativar = <<<EOD
					'<img class=\'link\' title=\'Desativar Técnico\' src=\'../imagens/0_inativo.png\' onclick="inativarTecnico(' || temid || ', \'I\');"  >'
EOD;
		$imgAtivar = <<<EOD
					'<img class=\'link\' title=\'Ativar Técnico\' src=\'../imagens/0_ativo.png\' onclick="inativarTecnico(' || temid || ', \'A\');"  >'
EOD;
		
		$sql = <<<EOD
			SELECT
				DISTINCT
					'<img class=\'link\' src=\'../imagens/alterar.gif\' onclick=\'editarTecnico(' || temid || ')\'  >
					 <img class=\'link\' src=\'../imagens/excluir.gif\' onclick=\'excluirTecnico(' || temid || ')\'  >
					 <img class=\'link\' src=\'../imagens/consultar.gif\' onclick=\'historicoTecnico(' || temid || ')\'  >' as acao,
					gr.sgrnum || ' - ' || gr.sgrnome as grupo,
					to_char(e.entnumcpfcnpj::int8,'00"."000"."000"/"0000"-"00') || ' - ' || e.entnome AS empresa, 
					temcpf,
					temnome,
					temcargo,
					temnumcreacau,
					TO_CHAR(temdtcadastro, 'dd/mm/yyyy') AS temdtcadastro,
					TO_CHAR(temdtcapacitacao, 'dd/mm/yyyy') AS temdtcapacitacao,
					CASE WHEN temvigente = 'A'
						THEN 'Ativo  ' || $imgDesativar
						ELSE 'Inativo  ' || $imgAtivar 
					END AS status
				FROM
					obras2.tecnico_empresa tem
				INNER JOIN 
					obras2.supervisao_grupo_empresa ge ON ge.sgeid = tem.sgeid AND ge.sgestatus != 'I'
				INNER JOIN 
					obras2.supervisao_grupo gr ON gr.sgrid = ge.sgrid AND gr.sgrstatus = 'A'
				INNER JOIN
					entidade.entidade e ON e.entid = ge.entid
				WHERE
					1=1
				$sql_where
				ORDER BY
					grupo,empresa,temnome DESC
EOD;
//dbg($sql, d);
		return $sql;
	}
	
	public function pegaPorCpf( $cpf ){
		$sql = "SELECT 
					count(1) AS existe 
				FROM 
					obras2.tecnico_empresa 
				WHERE
					temstatus = 'A' AND 
					temvigente = 'A' AND 
					temcpf = '$cpf'";
		$dado = $this->pegaUm( $sql );
		return ($dado ? $dado : 0);
	}
	
	public function salvarTecnico()
	{
		$_REQUEST['sgeid'] = $_REQUEST['sgeid_disable'] ? $_REQUEST['sgeid_disable'] : $_REQUEST['sgeid'];
		$this->popularDadosObjeto();
		if($this->salvar()){
			$_SESSION['obras2']['tecnico_empresa']['alert'] = "Operação realizada com sucesso.";
			$historico_tecnico = new Historico_Tecnico_Empresa();
			$arDados = array(
								"usucpf" => $_SESSION['usucpf'],
								"temid" => $this->temid,
								"htedado" => serialize($this->getDados()),
								"htestatus" => $this->temstatus,
			);
			$historico_tecnico->popularDadosObjeto($arDados);
			$historico_tecnico->salvar();
		}else{
			$_SESSION['obras2']['tecnico_empresa']['alert'] = "Não foi possível realizar a operação.";
		}
		header("Location: obras2.php?modulo=principal/tecnicoEmpresa&acao=A");
	}
	
	public function excluirTecnico($temid)
	{
		$this->carregarPorId($temid);
		$this->temstatus = 'I';
		$this->salvar();
			
		$historico_tecnico = new Historico_Tecnico_Empresa();
		$arDados = array(
							"usucpf" => $_SESSION['usucpf'],
							"temid" => $this->temid,
							"htedado" => serialize($this->getDados()),
							"htestatus" => "I",
		);
		$historico_tecnico->popularDadosObjeto($arDados);
		$historico_tecnico->salvar();
	}
	
	public function ativacaoTecnico($temid, $temvigente)
	{
		$this->carregarPorId($temid);
		$this->temvigente = $temvigente;
		$this->salvar();
					
		$historico_tecnico = new Historico_Tecnico_Empresa();
		$arDados = array(
							"usucpf" => $_SESSION['usucpf'],
							"temid" => $this->temid,
							"htedado" => serialize($this->getDados()),
							"htestatus" => "I",
		);
		$historico_tecnico->popularDadosObjeto($arDados);
		$historico_tecnico->salvar();
	}
	
	public function recuperaDados($temid = null)
	{
		if(!$this->getDados()){
			$this->carregarPorId( $temid );
		}
		$sql = "select 
					gr.sgrid
				from 
					obras2.supervisao_grupo gr
				inner join
					obras2.supervisao_grupo_empresa e ON e.sgrid = gr.sgrid
				where
					sgeid = ".$this->sgeid;
		$sgrid = $this->pegaUm($sql);
		$arDados = $this->getDados();
		$arDados['sgrid'] = $sgrid;
		return $arDados;
	}
	
}