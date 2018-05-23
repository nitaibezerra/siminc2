<?php
class planoTrabalhoDAO extends cls_banco{
	// Guarda uma instância da classe
    static private $instance;
    private $param;
	
	private function __construct(){
		parent::__construct();
	}
	
	public function __destruct(){
		
	}
	
	/**
	 * metodo set()
	 * executada sempre que a propriedade for atribuída
	 *
	 * @param $propriedade = nome do atributo
	 * @param $valor = valor do atributo
	 */
	function __set($propriedade, $valor){
		$this->param[$propriedade] = $valor;
	}
	
	/**
	 * metodo get()
	 * executada sempre que a propriedade for requerida
	 *
	 * @param $propriedade = nome do atributo
	 * @return valor do atributo
	 */
	function __get($propriedade){
		return $this->param[$propriedade];
	}
	
	/**
	 * metodo getInstance()
	 * cria uma instancia caso ainda não exista.
	 *
	 * @return instancia
	 */
	static public function getInstance(){
		if( !isset( self::$instance ) ){
			$classe = __CLASS__;
			self::$instance = new $classe;
		}
		return self::$instance;
	}
	
	public function getSelect(){
		$sql = 'SELECT ' . $this->distinct;
		
		// monta string com os nomes de colunas
        $sql .= implode(',', $this->colunas);
        
        $sql .= ' FROM ';
        
		$sql .= $this->joins;
        
        if( $this->where ){
        	$sql .= ' WHERE ' . $this->where;
        }        
        if( $this->group ){
        	$sql .= ' GROUP BY ' . implode(',', $this->group);
        }        
		if ($this->order){
			$sql .= ' ORDER BY ' . implode(',', $this->order);
        }
        if ($this->limit){
        	$sql .= ' LIMIT ' . $this->limit;
        }
        if ($this->offset){
        	$sql .= ' OFFSET ' . $this->offset;
        }
        
        return $sql;
        
	}
	
	/**
	 * Monta os filtros da listagem do plano de trabalho
	 *
	 * @param array $post
	 * @return array
	 */
	public function montaFiltrosPTA($obPost, $perfil, $residPossiveis){
		$where = "";
		
		if( $obPost->filtraSituacao ){
			$where = "esd.esdid = {$obPost->esdid} AND enb.estuf = '{$obPost->estuf}'";
		}
		
		if( $obPost->emecod || $obPost->autid ||  in_array( AUTOR_EMENDA, $perfil) ){
			$where1 .= ( is_numeric($obPost->emecod) ) ? " AND e.emecod = '".$obPost->emecod."'" : "";
			$where1 .= ( is_numeric($obPost->autid) ) ? " AND a.autid = '".$obPost->autid."'" : "";
			
			if ( in_array( AUTOR_EMENDA, $perfil) ){
				$where1 .= " AND a.autid in (SELECT autid FROM emenda.usuarioresponsabilidade WHERE usucpf = '{$_SESSION["usucpf"]}' AND rpustatus = 'A')";
			} 
				$where.="and 0 <> (select count(1) from emenda.autor a
							  INNER JOIN emenda.emenda e ON e.autid = a.autid
							  INNER JOIN emenda.emendadetalhe ed ON ed.emeid = e.emeid
							  INNER JOIN emenda.emendadetalheentidade ede ON ed.emdid = ede.emdid
							  INNER JOIN emenda.ptemendadetalheentidade pede ON ede.edeid = pede.edeid
							  where pede.ptrid = ptr.ptrid
							  $where1)";		
		}
		
		if( is_array( $obPost->espid ) ){
			$filtroEspef = " INNER JOIN emenda.iniciativaresponsavel ir on ir.resid = mod.resid 
					INNER JOIN emenda.iniciativa ini on ini.iniid = ir.iniid
					INNER JOIN emenda.iniciativaespecificacao ine on ine.iniid = ini.iniid
					INNER JOIN emenda.especificacao esp on esp.espid = ine.espid
					inner join emenda.especificacao_programacaoexercicio epe on epe.espid = esp.espid and epe.prsano = '".$_SESSION['exercicio']."'
					
					/*INNER JOIN emenda.especificacao esp on res.resid = esp.resid
		            INNER JOIN emenda.iniciativaespecificacao ine on ine.espid = esp.espid*/
		            inner join emenda.ptiniciativaespecificacao ptie 
		            	on ptie.iceid = ine.iceid
		            	and ptie.ptiid = pti1.ptiid
						and ptie.ptestatus = 'A'";
		}
		
		$where .= ($obPost->enbcnpj) ? " AND enb.enbcnpj = '".str_replace( array(".","/","-"), "", $obPost->enbcnpj )."'" : "";
		$where .= ($obPost->enbnome) ? " AND UPPER( removeacento(enb.enbnome) ) like '%".str_to_upper( removeAcentos( trim($obPost->enbnome) ) )."%'" : "";
		$where .= ($obPost->estuf) ? " AND enb.estuf = '".$obPost->estuf."'" : "";
		$where .= ($obPost->mundescricao) ? " AND UPPER( removeacento(mun.mundescricao) ) like '%".str_to_upper(removeAcentos( trim($obPost->mundescricao) ))."%'" : "";
		$where .= ($obPost->resid) ? " AND mod.resid = ".$obPost->resid : "";
		$where .= ($obPost->ptrid) ? " AND ptr.ptrcod = ".str_replace(' ','',$obPost->ptrid) : "";
		$where .= ($residPossiveis) ? " AND mod.resid in (" . implode( ", ", $residPossiveis ) . ")" : "";
		$where .= ($obPost->esdid) ? " AND esd.esdid = ".$obPost->esdid : "";
		$where .= ($obPost->iniid) ? " AND pti1.iniid = ".$obPost->iniid : "";
		$where .= ($obPost->espid[0]) 
								? 
									" AND esp.espid " . (( $obPost->espid_campo_excludente == null || $obPost->espid_campo_excludente == '0') ? ' IN ' : ' NOT IN ') . " (" . implode( ", ", $obPost->espid ) . ")" 
								: 
									"";
		if( in_array( INSTITUICAO_BENEFICIADA, $perfil) ) {
			if($_SESSION["emenda"]["enbid"])
				$where .= " AND ptr.enbid = ".$_SESSION["emenda"]["enbid"];
		}
		$arFiltros = array();
		$arFiltros[] = $where;
		$arFiltros[] = $filtroEspef;
		
		return $arFiltros;
	}
	/**
	 * Montado o sql da listagem do plano de trabalho
	 *
	 * @param array $post
	 * @return SQL
	 */
	public function montaSQLListaPTA($obPost, $perfil, $residPossiveis){
		$emecod = (is_numeric($obPost->emecod) ? $obPost->emecod : '');
		$autid = (is_numeric($obPost->autid) ? $obPost->autid : '');
		if($obPost->check_expandir){
			$func = "carregaEmendaDetalhePTA(\"img_dimensao_' || ptr.ptrid || '\",\'' || ptr.ptrid || '\',\'".$emecod."\',\'".$autid."\');";
		} else {
			$func = "";
		}
		
		if ($obPost->solicitarReformulacao == 1) $filtroReformulacao = "AND ptm.pmcnumconveniosiafi is not null";
		
		$arFiltros = $this->montaFiltrosPTA($obPost, $perfil, $residPossiveis);
		
		$acoes = "'<img style=\"cursor:pointer\" id=\"img_dimensao_' || ptr.ptrid || '\" src=\"/imagens/mais.gif\" 
			onclick=\"carregaEmendaDetalhePTA(this.id,\'' || ptr.ptrid || '\',\'".$emecod."\',\'".$autid."\');\" 
			border=\"0\" /> <a href=\"#\" onclick=\"alterarPlanoTrabalho(' || ptr.ptrid || ');\">
			<img src=\"../imagens/alterar.gif\" style=\"cursor:pointer;\" border=\"0\"></a>'";
		
		$sql = "SELECT DISTINCT
					$acoes as acoes,
					ptr.ptrid,
				    enb.enbnome,
				    enb.enbcnpj,
				    enb.estuf,
				    mun.mundescricao,
				    --res.resassunto,
				    mod.mdedescricao as resassunto,
				    doc.esdid,
				    --sum(distinct pti.ptivalortotal) as valorTotal,
				    pti.ptivalortotal as valorTotal,
		            esd.esddsc,
		            (SELECT 1 FROM emenda.anexo anx
		             WHERE anx.ptrid = ptr.ptrid LIMIT 1 ) as anxid, 
		            '</td></tr>
		            	<tr style=\"display:none\" id=\"listaEmendaDetalhePTA_' || ptr.ptrid || '\" >
		            		<td id=\"trV_' || ptr.ptrid || '\" colspan=8 ></td>
		            </tr><script>$func</script>' as linha,
		            ptr.ptrdataimpressao,
		            ptr.ptrimprimirpta,
		            ptr.ptridpai,
		            ptr.ptrcod,
		            coalesce(ptr.ptrvalorproponente,0) as ptrvalorproponente,		            
		            (select count(exfid) from emenda.execucaofinanceira where ptrid = ptr.ptrid and exfstatus = 'A' and exfnumsolempenho is not null ) as boempenho,
		            case when ptr.sisid = 23 then 'PAR - Plano de Metas' else 'Emendas' end as sistema
				FROM 
				    emenda.planotrabalho ptr
				    inner join emenda.entidadebeneficiada enb on ptr.enbid = enb.enbid
				    inner join workflow.documento doc on ptr.docid = doc.docid 
				    inner join workflow.estadodocumento esd on esd.esdid = doc.esdid
					inner join emenda.modalidadeensino mod on mod.mdeid = ptr.mdeid and mod.resid = ptr.resid
				    left join territorios.municipio mun on mun.muncod = enb.muncod				    
				    left join emenda.ptminutaconvenio ptm on ptm.ptrid = ptr.ptrid and ptm.pmcstatus = 'A'
				    left join emenda.v_ptiniciativa pti1 on ptr.ptrid = pti1.ptrid
					left join
						(SELECT 
							ptrid,
							sum(ptivalortotal) as ptivalortotal
						 FROM emenda.v_ptiniciativa 
						 GROUP BY ptrid) pti ON pti.ptrid = ptr.ptrid
					{$arFiltros[1]}
				WHERE
				    ptr.ptrstatus = 'A' 
				    AND ptr.ptrexercicio = ".$_SESSION['exercicio']."
				    ".$arFiltros[0]."
				    ".$filtroReformulacao."
				    AND ptr.ptrid NOT IN (SELECT tt.ptridpai FROM emenda.planotrabalho tt WHERE tt.ptridpai = ptr.ptrid and tt.ptrstatus = 'A')
				ORDER BY 
					ptr.ptrcod";
		return $sql;
	}
	
	/**
	 * Função carrega o perfil do usuário por sistema
	 *
	 * @return array
	 */
	public function carregaPerfil(){
		$sql = "SELECT
					pu.pflcod
				FROM 
					seguranca.perfil AS p 
				LEFT JOIN 
					seguranca.perfilusuario AS pu ON pu.pflcod = p.pflcod
				WHERE 
					p.sisid = '".$_SESSION['sisid']."'
				  	AND pu.usucpf = '".$_SESSION['usucpf']."'";
		
		return $this->carregarColuna($sql);
	}
	
	/**
	 * Função monta o sql da emenda detalhe do plano de trabalho
	 *
	 * @param array $post
	 * @return query
	 */
	public function SQLEmendaDetalhePTA_Ajax($post){
		$ptrid = $post['ptrid'];
		$emecod = $post['emecod'];
		$autid = $post['autid'];
		
		$filtro = "";
		
		if($emecod){
			$filtro.= " AND vede.emecod = '".$emecod."'";
		}
		if($autid){
			$filtro.= " AND aut.autid = '".$autid."'";
		}
		
		$sql = "SELECT DISTINCT 
				    vede.emecod,
				    case when vede.emerelator = 'S' then aut.autnome||' - Relator Geral' else aut.autnome end as autnome,
				    fup.fupdsc, 
				    vede.gndcod||' - '||gn.gnddsc as gndcod1, 
				    vede.mapcod||' - '||map.mapdsc as modalidade,
				    vede.foncod||' - '||fon.fondsc as fonte,
				    ped.pedvalor
				FROM emenda.planotrabalho ptr
				    INNER JOIN emenda.ptemendadetalheentidade  ped
				        ON ptr.ptrid = ped.ptrid
				    INNER JOIN emenda.v_emendadetalheentidade vede
				    	ON vede.edeid = ped.edeid
				    left JOIN emenda.v_funcionalprogramatica fup
				        ON (vede.acaid = fup.acaid)
				    inner JOIN emenda.autor aut
				        ON aut.autid = vede.autid
	                left join public.gnd gn
				        on gn.gndcod = vede.gndcod
		                and gn.gndstatus = 'A'
				    left join public.modalidadeaplicacao map
				        on map.mapcod = vede.mapcod
				    left join public.fonterecurso fon
				        on fon.foncod = vede.foncod 
		                and fon.fonstatus = 'A'
				WHERE ptr.ptrid = $ptrid
				    AND vede.edestatus = 'A' " . $filtro;
		
		return $sql;
	}
	
	/**
	 * Função carrega a iniciativa vinculada ao plano de trabalho
	 *
	 * @param integer $ptrid
	 * @param boolean $boMontaLista
	 * @return array ou query
	 */
	public function carregaIniciativaPTADAO($ptrid, $iniid = null, $boMontaLista = false){
		
		$filtro = array();
		
		if( $ptrid )
			$filtro[] = "pti.ptrid = ".$ptrid; 
			
		if( $iniid )
			$filtro[] = "ini.iniid = ".$iniid;
		
		$sql = "SELECT DISTINCT
					ini.iniid,
					ini.ininome
				FROM 
					emenda.ptiniciativa pti
	                inner join emenda.iniciativa ini
	                	on ini.iniid = pti.iniid
	                    --and ini.inistatus = 'A'
				WHERE 
					".implode( ' and ', $filtro );

		if( $boMontaLista ){
			return $sql;
		} else {
			return $this->carregar($sql);
		}
	}
	
	/**
	 * Função carrega os registros cadastrados na pagina definir recursos
	 * 
	 * @param integer $ptrid
	 * @return arrat
	 */
	public function SQLDadosDefinirRecursos( $ptrid ){
		$sql = "SELECT
				  ptr.ptrid,
				  ptr.ptrvalorproponente,
				  ptr.ptrrendimentoaplicacao,
				  res.resid,
				  res.resassunto,
				  enb.enbid,
				  ptr.ptrcod,
				  ptr.ptridpai,
				  ptr.refid,
				  mdeid,
				  ptr.sisid
				FROM
				  emenda.planotrabalho ptr 
                  INNER JOIN emenda.responsavel res ON (ptr.resid = res.resid) 
                  INNER JOIN emenda.entidadebeneficiada enb ON (ptr.enbid = enb.enbid)
				WHERE
				  ptr.ptrstatus = 'A'
				  AND res.resstatus = 'A'
				  AND ptr.ptrid = $ptrid";
		
		return $this->pegaLinha($sql);
	}
	
	public function cabecalhoPlanoTrabalhoDAO( $ptrid ){
		$sql = "SELECT				
				  enb.enbcnpj AS cnpj,
				  enb.enbid,
                  (CASE WHEN vede.emedescentraliza = 'N' THEN
                  '<img src=\"/imagens/consultar.gif\" border=0 align=\"absmiddle\" vspace=\"3\" title=\"Atualizar dados da entidade\" style=\"cursor:pointer;\" onclick=\"atualizaEntidadeBaseFNDE('|| ptr.enbid || ', '|| ptr.ptrid || ');\"/> &nbsp; <a style=\"cursor:pointer;\" onclick=\"atualizaEntidadeBaseFNDE('|| ptr.enbid || ', '|| ptr.ptrid || ');\">' || enb.enbnome || ' (' || enb.enbid || ') </a>'
                  ELSE
				  '<img src=\"/imagens/consultar.gif\" border=0 align=\"absmiddle\" vspace=\"3\" title=\"Editar Entidade\" style=\"cursor:pointer;\" onclick=\"Entidade('|| enb.enbid || ');\"/> &nbsp; <a style=\"cursor:pointer;\" onclick=\"Entidade('|| enb.enbid || ');\">' || enb.enbnome || ' (' || enb.enbid || ') </a>' END) AS orgao_entidade,                  
				  ptr.ptrexercicio AS exercicio,				
				  CASE WHEN enb.muncod IS NULL THEN
					CASE WHEN vede.emedescentraliza = 'N' THEN
						'<span style=\"color: red\">Clique no link acima para atualizar os dados da entidade, caso os dados continuem desatualizados, favor entrar em contato com a Coordenação de Habilitação para Projetos Educacionais (COHAP). Telefones: (61) 2022-4291 / 2022-4296.</span>'
					ELSE 
						'<span style=\"color: red\">Dados desatualizados. Clique no link acima para atualizar o endereço da entidade.</span>' 
					END
				ELSE
					(select mun.mundescricao || ' / ' || mun.estuf 
					from territorios.municipio mun 
					WHERE mun.muncod = enb.muncod)
				END	as endereco,
				  ptr.ptrcod as ptrid, 
				  md.mdedescricao as resassunto,
				  (sum(ped.pedvalor) + coalesce(ptr.ptrvalorproponente,0)) as ptrvalorconcedente,
				  pti.ptivalortotal as valor,
				  ptr.sisid
				  
				FROM
				  emenda.entidadebeneficiada enb 
				  INNER JOIN emenda.planotrabalho ptr ON (enb.enbid = ptr.enbid) 
				  inner join emenda.modalidadeensino md on md.resid = ptr.resid and md.mdeid = ptr.mdeid
				  INNER JOIN emenda.ptemendadetalheentidade ped on ped.ptrid = ptr.ptrid
				  inner join emenda.v_emendadetalheentidade vede on vede.edeid = ped.edeid AND vede.edestatus = 'A'
	  			  LEFT JOIN 
					(SELECT 
						ptrid,
						sum(ptivalortotal) as ptivalortotal
					 FROM emenda.v_ptiniciativa 
					 GROUP BY ptrid) pti ON pti.ptrid = ptr.ptrid
				WHERE
				  ptr.ptrid = $ptrid
				  AND ptr.ptrexercicio = ".$_SESSION["exercicio"]."
				Group by enb.enbcnpj,
				  orgao_entidade,
				  ptr.ptrexercicio,
				  enb.enbid,
				  ptr.ptrid, 
				  ptr.ptrcod,
				  md.mdedescricao,
				  ptr.ptrvalorproponente,
				  pti.ptivalortotal,
				  vede.emedescentraliza,
				  enb.muncod,
				  ptr.sisid";
		
		return $this->carregar($sql);
	}
	
	public function carregaDadosCabecalhoDefinirRecurso( $enbid ){
		$sql = "SELECT 
				  ent.enbcnpj AS cnpj,
				  ent.enbnome AS orgao_entidade,
				  mun.mundescricao AS municipio,
				  ent.estuf AS uf
				FROM
				  emenda.entidadebeneficiada ent 
				  left JOIN territorios.municipio mun ON (mun.muncod = ent.muncod) 
				WHERE
				  ent.enbid = $enbid
				  AND ent.enbstatus = 'A'";
			  
		return $this->carregar($sql);
	}
	
	/**
	 * Função carrega os dados do nivel de ensino beneficiado
	 *
	 * @param integer $ptrid
	 * @param integer $enbid
	 * @return array
	 */
	public function carregaNivelEnsinoBeneficiado($ptrid, $enbid){				
		$sql = "SELECT DISTINCT
					me.resid,
					ed.emdid,
					me.mdedescricao,
					me.mdeid
				FROM
				  emenda.emenda e
				  INNER JOIN emenda.responsavel res ON res.resid = e.resid 
				  INNER JOIN emenda.autor a ON (e.autid = a.autid) 
				  LEFT JOIN emenda.v_funcionalprogramatica funcp ON (e.acaid = funcp.acaid) 
					AND funcp.prgano = ".(($ptrid) ? '(SELECT ptrexercicio::varchar FROM emenda.planotrabalho WHERE ptrid = '.$ptrid.')' : '\''.$_SESSION["exercicio"].'\'')."
					AND funcp.acastatus = 'A'
				  INNER JOIN emenda.emendadetalhe ed ON (e.emeid = ed.emeid) 
				  INNER JOIN emenda.emendadetalheentidade ede ON (ed.emdid = ede.emdid)
				  LEFT JOIN (Select ped.pedid, ped.edeid, ped.ptrid, ptr.mdeid, ptr.resid
														From emenda.ptemendadetalheentidade ped
															inner join emenda.planotrabalho ptr
																ON (ptr.ptrid = ped.ptrid)
														Where ptr.ptrstatus = 'A') ptede
				  ON (ede.edeid = ptede.edeid) 
				  inner join emenda.modalidadeensino me on me.resid = res.resid 
				WHERE
				  (".(($ptrid) ? "ptede.ptrid = ".$ptrid." OR ptede.ptrid is null" : "ptede.ptrid is null").")
				  AND ede.edestatus = 'A'
				  AND ede.ededisponivelpta = 'S'
				  AND ede.enbid = ".(($ptrid) ? '(SELECT enbid FROM emenda.planotrabalho WHERE ptrid = '.$ptrid.')' : $enbid);
		
		return $this->carregar($sql);	
	}
	
	/**
	 * Função Carrega Emendas Disponiveis
	 * Método usado para pesquisar registro no banco
	 * @param integer $resid - Código do Responsavel
	 * @return void 
	 * @access public
	 * @author Wesley Romualdo da Silva
	 * @since 09/09/2009
	 */
	public function carregaEmendasDisponiveisAjaxDAO($ptrid, $mdeid, $enbid){

		$resid = $this->pegaUm("SELECT resid FROM emenda.modalidadeensino WHERE mdeid = $mdeid");
		$sql = "SELECT
					  e.emecod,
					  (CASE WHEN e.emetipo = 'E' THEN 'Emenda'
							ELSE 'Complemento' END) as tipoemenda,
					  case when e.emerelator = 'S' then a.autnome||' - Relator Geral' else a.autnome end as autnome,
					  funcp.fupfuncionalprogramatica,
					  funcp.fupdsc,
					  sum(ede.edevalor) as valor,
					  e.emeid,
					  e.etoid,
					  e.emedescentraliza
				FROM
					  emenda.emenda e INNER JOIN emenda.autor a ON (e.autid = a.autid) 
					  LEFT JOIN emenda.v_funcionalprogramatica funcp ON (e.acaid = funcp.acaid) 
						AND funcp.prgano >= '{$_SESSION['exercicio']}'
						AND funcp.acastatus = 'A'
					  INNER JOIN emenda.emendadetalhe ed ON (e.emeid = ed.emeid) 
					  INNER JOIN emenda.emendadetalheentidade ede ON (ed.emdid = ede.emdid)
					  LEFT JOIN (Select ped.pedid, ped.edeid, ped.ptrid
															From emenda.ptemendadetalheentidade ped
																inner join emenda.planotrabalho ptr
																	ON (ptr.ptrid = ped.ptrid)
															Where ptr.ptrstatus = 'A') ptede
					  ON (ede.edeid = ptede.edeid)  
				WHERE
					  (".(($ptrid) ? "ptede.ptrid = ".$ptrid." OR ptede.ptrid is null" : "ptede.ptrid is null").")
					  AND ede.edestatus = 'A'
					  AND e.resid in (".($resid == 1 || $resid == 3 ? ' 1,3 ' : $resid).")
					  AND ede.enbid = ".(($ptrid) ? '(SELECT enbid FROM emenda.planotrabalho WHERE ptrid = '.$ptrid.')' : $enbid)."
					  AND ede.ededisponivelpta = 'S'
					  AND e.emeano >= {$_SESSION['exercicio']}
					  and e.emetipo = 'E'
					  $where
				GROUP BY
					  e.emecod,
					  a.autnome,
					  e.emerelator,
					  funcp.fupfuncionalprogramatica,
					  funcp.fupdsc,
					  e.emeid,
					  e.emetipo,
					  e.emedescentraliza,
					  e.etoid
				  
				union all
				
				SELECT
					vede.emecod,
				    (CASE WHEN vede.emetipo = 'E' THEN 'Emenda' ELSE 'Complemento' END) as tipoemenda,
				    case when ee.emerelator = 'S' then a.autnome||' - Relator Geral' else a.autnome end as autnome,
				    vfun.fupfuncionalprogramatica,
				    vfun.fupdsc,
				    sum(vede.edevalor) as valor,
				    vede.emeid,
				    ee.etoid,
				    vede.emedescentraliza
				FROM
					emenda.v_emendadetalheentidade vede
				    inner join emenda.autor a ON (vede.autid = a.autid) 
				    inner join emenda.emenda ee ON vede.emeid = ee.emeid 
				    left join emenda.v_funcionalprogramatica vfun on vfun.acaid = vede.acaid and vfun.prgano >= '{$_SESSION['exercicio']}' and vfun.acastatus = 'A'
				    left JOIN emenda.emendadetalhe ed ON (ee.emeid = ed.emeid) 
				    LEFT JOIN (Select ped.pedid, ped.edeid, ped.ptrid
				                                          From emenda.ptemendadetalheentidade ped
				                                              inner join emenda.planotrabalho ptr
				                                                  ON (ptr.ptrid = ped.ptrid)
				                                          Where ptr.ptrstatus = 'A') ptede
				    ON (vede.edeid = ptede.edeid)  
				  WHERE
				    (".(($ptrid) ? "ptede.ptrid = ".$ptrid." OR ptede.ptrid is null" : "ptede.ptrid is null").")
				    AND vede.edestatus = 'A'
				    AND vede.resid in (".($resid == 1 || $resid == 3 ? ' 1,3 ' : $resid).")
				    AND vede.entid in (".(($ptrid) ? "( select enbid from emenda.entidadebeneficiada where enbcnpj = (
									    						SELECT e.enbcnpj FROM emenda.planotrabalho p
									    							inner join emenda.entidadebeneficiada e on e.enbid = p.enbid WHERE ptrid = $ptrid) 
									                            and enbano >= '".$_SESSION['exercicio']."'
									                            and enbstatus = 'A'
									    					)" : $enbid).")
				    AND vede.ededisponivelpta = 'S'
				    AND vede.emeano >= '".$_SESSION['exercicio']."'
				    and vede.emetipo = 'X'
				     $where
				  GROUP BY
				    vede.emecod,
				    a.autnome,
				    ee.emerelator,
				    vfun.fupfuncionalprogramatica,
				    vfun.fupdsc,
				    vede.emeid,
				    vede.emetipo,
				    ee.etoid,
				    vede.emedescentraliza";

		return $this->carregar($sql);
	}
	
	/**
	 * Função carregaDetalheEmenda
	 * Método usado para mostrar os dados da emenda
	 * @param objeto idImg - Objeto do Formulario
	 * @param string div_nome - Nome da Div
	 * @param string tr_nome - Nome da tr
	 * @param int emeid - Codigo da Emenda
	 * @return void 
	 * @access public
	 * @author Wesley Romualdo da Silva
	 * @since 16/09/2009
	 */
	public function carregaDetalheEmendaAjaxDAO($emeid, $ptrid){
			
		$sql = "SELECT distinct
				  ed.emdid,
				  ed.emeid,
				  ed.emdimpositiva,
				  /*ed.gndcod,
				  ed.foncod,
				  ed.mapcod,*/
				  ed.gndcod||' - '||gn.gnddsc as gndcod1, 
			      ed.mapcod||' - '||map.mapdsc as modalidade,
			      ed.foncod||' - '||fon.fondsc as fonte,
				  ede.edevalor as edevalor,
				  e.emetipo,
				  ptede.edeid,
				  ptede.pedid,
				  ede.edeid as coddetalhe				  
				FROM
				  emenda.emenda e 
				  LEFT JOIN emenda.v_funcionalprogramatica funcp ON (funcp.acaid = e.acaid) 
						AND funcp.prgano = ".(($ptrid) ? '(SELECT ptrexercicio::varchar FROM emenda.planotrabalho WHERE ptrid = '.$ptrid.')' : '\''.$_SESSION["exercicio"].'\'')."
				  INNER JOIN emenda.emendadetalhe ed
				  ON (e.emeid = ed.emeid) INNER JOIN emenda.emendadetalheentidade ede
				  ON (ed.emdid = ede.emdid)LEFT JOIN (Select ped.pedid, ped.edeid, ped.ptrid
														From emenda.ptemendadetalheentidade ped
															inner join emenda.planotrabalho ptr
																ON (ptr.ptrid = ped.ptrid)
														Where ptr.ptrstatus = 'A') ptede
				  ON (ede.edeid = ptede.edeid)
			  	left join public.gnd gn on gn.gndcod = ed.gndcod and gn.gndstatus = 'A'
			    left join public.modalidadeaplicacao map on map.mapcod = ed.mapcod
			    left join public.fonterecurso fon on fon.foncod = ed.foncod and fon.fonstatus = 'A'
				WHERE
				  ed.emeid = $emeid
				  AND (".(($ptrid) ? "ptede.ptrid = ".$ptrid." OR ptede.ptrid is null" : "ptede.ptrid is null").")
				  AND ede.edestatus = 'A'
				  AND ede.ededisponivelpta = 'S'
				  and ede.enbid = ".(($ptrid) ? '(SELECT enbid FROM emenda.planotrabalho WHERE ptrid = '.$ptrid.')' : $_SESSION["emenda"]["enbid"])."
				  and e.emetipo = 'E'
			union all
			
			SELECT distinct
		      vede.emdid,
		      vede.emeid,
		      vede.emdimpositiva,
		      vede.gndcod||' - '||gn.gnddsc as gndcod1, 
		      vede.mapcod||' - '||map.mapdsc as modalidade,
		      vede.foncod||' - '||fon.fondsc as fonte,
		      vede.edevalor as edevalor,
		      vede.emetipo,
		      ptede.edeid,
		      ptede.pedid,
		      vede.edeid as coddetalhe
		    FROM
		      emenda.v_emendadetalheentidade vede 
		      left join emenda.v_funcionalprogramatica funcp ON (funcp.acaid = vede.acaid) 
		            AND funcp.prgano >= '".$_SESSION['exercicio']."'
		      left join (Select ped.pedid, ped.edeid, ped.ptrid
		                                              From emenda.ptemendadetalheentidade ped
		                                                  inner join emenda.planotrabalho ptr
		                                                      ON (ptr.ptrid = ped.ptrid)
		                                              Where ptr.ptrstatus = 'A') ptede
		      ON (vede.edeid = ptede.edeid)
		      left join public.gnd gn on gn.gndcod = vede.gndcod and gn.gndstatus = 'A'
		      left join public.modalidadeaplicacao map on map.mapcod = vede.mapcod
		      left join public.fonterecurso fon on fon.foncod = vede.foncod and fon.fonstatus = 'A'
		    WHERE
		      vede.emeid = $emeid
		      AND (".(($ptrid) ? "ptede.ptrid = ".$ptrid." OR ptede.ptrid is null" : "ptede.ptrid is null").")
		      AND vede.edestatus = 'A'
		      AND vede.ededisponivelpta = 'S'
		      AND vede.entid in (".(($ptrid) ? "( select enbid from emenda.entidadebeneficiada where enbcnpj = (
									    						SELECT e.enbcnpj FROM emenda.planotrabalho p
									    							inner join emenda.entidadebeneficiada e on e.enbid = p.enbid WHERE ptrid = $ptrid) 
									                            and enbano >= '".$_SESSION['exercicio']."'
									                            and enbstatus = 'A'
									    					)" : $_SESSION["emenda"]["enbid"]).")
		    AND vede.emeano >= '".$_SESSION['exercicio']."'
		    and vede.emetipo = 'X'";
	
		return $this->carregar($sql);
	}
	
	/**
	 * Função cadastra o plano de trabalho
	 *
	 * @param array de post $post
	 */
	
	public function inserePlanoTrabalhoDAO( $post, $boFilho = false ){
		
		$ptridpai = ( $post['ptrid'] ? $post['ptrid'] : 'null' );
		
		$ptrcod = ($post['ptrcod'] ? $post['ptrcod'] : $this->pegaUm("SELECT max(ptrcod) + 1 FROM emenda.planotrabalho") );
		
		if( $post['porcentagem_contrapartida'] != 'S' && $post['porcentagem_contrapartida'] != 'N' && !empty($post['porcentagem_contrapartida']) ){
			$porcentagem_contrapartida = $this->retiraPontos($post['porcentagem_contrapartida']);
		} else {
			$porcentagem_contrapartida = 'NULL';
		}
		
		/*if( is_numeric( $post['porcentagem_contrapartida'] ) ){
			$porcentagem_contrapartida = $this->retiraPontos($post['porcentagem_contrapartida']);
		} else {
			$porcentagem_contrapartida = 'NULL';
		}*/
		
		if( $boFilho ){
			$sql = "INSERT INTO emenda.planotrabalho( enbid, ptrexercicio, ptrjustificativa, ptrtipodirigente,
					entiddirigente, ptrstatus, resid, ptrvalorproponente, bcoid, ptragenciabancaria, ptrcontrapartida,
				  	docid, ptrnumdocumenta, ptrnumconvenio, ptranoconvenio, ptrnumprocessoempenho, ptrimprimirpta,
					ptrestadoretorno, ptrdataimpressao, ptridpai, refid, ptrcod, carid, ptrsituacao, mdeid, ptrrendimentoaplicacao ) 
				(SELECT {$post['enbid']}, {$_SESSION['exercicio']}, ptrjustificativa, ptrtipodirigente, entiddirigente, 'I',
				  	{$post['resid']}, ".$this->retiraPontos($post['ptrvalorproponente']).", bcoid, ptragenciabancaria, ".$porcentagem_contrapartida.", docid, ptrnumdocumenta,
				  	ptrnumconvenio, ptranoconvenio, ptrnumprocessoempenho, ptrimprimirpta, ptrestadoretorno,
				  	ptrdataimpressao, $ptridpai, refid, $ptrcod, carid, 'P', mdeid, ptrrendimentoaplicacao
				 FROM emenda.planotrabalho 
				 WHERE ptrid = $ptridpai and ptridpai is null and ptrstatus = 'A') RETURNING ptrid";

			$ptrid = $this->pegaUm($sql);
		} else {
			$ptrrendimentoaplicacao = ($post['ptrrendimentoaplicacao'] ? $this->retiraPontos($post['ptrrendimentoaplicacao']): 'null');
			
			$sql = "INSERT INTO emenda.planotrabalho( enbid, ptrexercicio, ptrstatus, resid, mdeid, ptrvalorproponente, 
						ptrcontrapartida, ptridpai, ptrcod, ptrsituacao, ptrrendimentoaplicacao )
					VALUES ( {$post['enbid']}, 
						{$_SESSION['exercicio']}, 'A', 
						{$post['resid']},
						{$post['mdeid']},
					  	".$this->retiraPontos($post['ptrvalorproponente']).",
					  	".$porcentagem_contrapartida.",
					  	".$ptridpai.",
					  	".$ptrcod.",
					  	'E',
					  	".$ptrrendimentoaplicacao."
					)RETURNING ptrid";
	
			$ptrid = $this->pegaUm($sql);
		}
		
		$ptrexercicio		= $_SESSION['exercicio'];
		$resid				= $post['resid'];
		$mdeid				= $post['mdeid'];
		$ptrvalorproponente = $this->retiraPontos($post['ptrvalorproponente']);
		$ptrcontrapartida	= $porcentagem_contrapartida; 
		$ptridpai			= $ptridpai;
		$ptrcod				= $ptrcod;
							
		$xml  = "";
		$xml .= "<ptrexercicio>$ptrexercicio</ptrexercicio>";
		$xml .= "<resid>$resid</resid>";
		$xml .= "<mdeid>$mdeid</mdeid>";
		$xml .= "<ptrvalorproponente>$ptrvalorproponente</ptrvalorproponente>";
		$xml .= "<ptrcontrapartida>$ptrcontrapartida</ptrcontrapartida>";
		$xml .= "<ptridpai>$ptridpai</ptridpai>";
		$xml .= "<ptrcod>$ptrcod</ptrcod>";

		$sql = "INSERT INTO
				emenda.historicogeral
				(
					tabid,
					histidtabela,
					histtxtalteracao,
					histdatainclusao,
					usucpfinclusao,
					ptrid
				)
			VALUES
				(
					'1',
					'{$ptrid}',
					'{$xml}',
					NOW(),
					'{$_SESSION[usucpf]}',
					'{$ptrid}'
				)";		
		$this->executar($sql);		
		//$this->commit();

		return $ptrid;
	}
	
	public function inserePlanoTrabalhoFilhoDAO( $ptridpai ){
		$ptridpai = ( $ptridpai ? $ptridpai : 'ptridpai' );		
		
		$sql = "INSERT INTO emenda.planotrabalho( enbid, ptrexercicio, ptrjustificativa, ptrtipodirigente,
					entiddirigente, ptrstatus, resid, ptrvalorproponente, bcoid, ptragenciabancaria, ptrcontrapartida,
				  	docid, ptrnumdocumenta, ptrnumconvenio, ptranoconvenio, ptrnumprocessoempenho, ptrimprimirpta,
					ptrestadoretorno, ptrdataimpressao, ptridpai, refid, ptrcod, carid, ptrsituacao, ptrdataalteracao, usucpfalteracao, mdeid, ptrrendimentoaplicacao ) 
				(SELECT enbid, ptrexercicio, ptrjustificativa, ptrtipodirigente, entiddirigente, ptrstatus,
				  	resid, ptrvalorproponente, bcoid, ptragenciabancaria, ptrcontrapartida, docid, ptrnumdocumenta,
				  	ptrnumconvenio, ptranoconvenio, ptrnumprocessoempenho, ptrimprimirpta, ptrestadoretorno,
				  	ptrdataimpressao, $ptridpai, refid, ptrcod, carid, 'C', ptrdataalteracao, usucpfalteracao, mdeid, ptrrendimentoaplicacao
				 FROM emenda.planotrabalho 
				 WHERE ptrid = $ptridpai and ptrstatus = 'A') RETURNING ptrid";	
		
		return $this->pegaUm( $sql );
	}
	
	public function inserePtiniciativaFilhoDAO( $ptrid, $ptiidPai, $ptridpai ){
		$sql = "INSERT INTO emenda.ptiniciativa( ptrid, ptidescricao, iniid, tpeid ) 
				(SELECT $ptrid, ptidescricao, iniid, tpeid
				 FROM emenda.ptiniciativa
				 WHERE ptiid = $ptiidPai and ptrid = $ptridpai) RETURNING ptiid";

		return $this->pegaUm( $sql );
	}
	
	public function inserePtiniciativaEspecificacaoFilhoDAO( $ptiid, $pteidPai, $pteid = null ){
		if( $pteid ){
			$sql = "UPDATE emenda.ptiniciativaespecificacao SET ptiid = $ptiid
	 				WHERE pteid = $pteid";		
			$this->executar( $sql );
			$_SESSION['emenda']['ptiid'] = $ptiid;
		} else {
			$sql = "INSERT INTO emenda.ptiniciativaespecificacao(ptiid, iceid, ptequantidade, ptevalorunitario, 
						ptevalorproponente, ptedatainicio, ptedatafim, ptestatus)
					(SELECT $ptiid, iceid, ptequantidade, ptevalorunitario, ptevalorproponente,
	  					max(ptedatainicio), max(ptedatafim), ptestatus
					FROM emenda.ptiniciativaespecificacao WHERE pteid = $pteidPai and ptestatus = 'A' 
					GROUP BY iceid, ptestatus, ptequantidade, ptevalorunitario, ptevalorproponente) RETURNING pteid";
			$pteid = $this->pegaUm( $sql );
		}
		return $pteid;		
	}
	
	public function alteraPtiniciativaEspecificacaoFilhoDAO( $ptiid, $ptiidPai ){
		$sql = "UPDATE emenda.ptiniciativaespecificacao SET ptiid = $ptiid
 				WHERE ptiid = $ptiidPai and ptestatus = 'A'";
		
		$this->executar( $sql );
	}
	
	public function inserePtitemkitFilhoDAO( $pteid, $pteidPai, $ptkid = null ){
		if( $ptkid ){
			$sql = "UPDATE emenda.ptitemkit SET pteid = $pteid
	 				WHERE ptkid = $ptkid";
			
			$this->executar( $sql );
			$sql = "INSERT INTO emenda.ptitemkit(pteid, iteid, itoid, ptkdescricao, ptkunidademedida, ptkquantidade, ptkvalorunitario)
					(SELECT $pteid, iteid, itoid, ptkdescricao, ptkunidademedida, ptkquantidade, ptkvalorunitario
				 	FROM emenda.ptitemkit
				 	WHERE pteid = $pteidPai and ptkid <> ".$_SESSION["emenda"]["ptkid"]." )";

			unset($_SESSION["emenda"]["ptkid"]);
			$this->executar( $sql );
		} else {
			$sql = "INSERT INTO emenda.ptitemkit(pteid, iteid, itoid, ptkdescricao, ptkunidademedida, ptkquantidade, ptkvalorunitario)
					(SELECT $pteid, iteid, itoid, ptkdescricao, ptkunidademedida, ptkquantidade, ptkvalorunitario
					 FROM emenda.ptitemkit
					 WHERE pteid = $pteidPai )";
			
			$this->executar( $sql );
		}
	}
	
	public function inserePtiniesprecursoFilhoDAO( $pteidNovo, $pteidPai){
		$sql = "INSERT INTO emenda.ptiniesprecurso(pteid, pedid, pervalor)
      			(SELECT $pteidNovo, pedid, pervalor
				FROM emenda.ptiniesprecurso
				WHERE pteid = $pteidPai) RETURNING perid";
		
		return $this->pegaUm( $sql );
	}
	
	public function alteraPtiniesprecursoFilhoDAO( $pedid, $pedidPai ){
		
		$pedid = ( $pedid ? $pedid : 'pedid' );
		
		$sql = "UPDATE emenda.ptiniesprecurso SET pedid = $pedid
				WHERE perid = $pedidPai";
		
		$this->executar( $sql );
	}
	
	public function inserePtiniciativabeneficiarioFilhoDAO( $ptiid, $ptiidPai, $ptbid = null ){
		if( $ptbid ){
			$sql = "UPDATE emenda.ptiniciativabeneficiario SET ptiid = $ptiid
	 				WHERE ptbid = $ptbid";		
			$this->executar( $sql );
		} else {
			$sql = "INSERT INTO emenda.ptiniciativabeneficiario( ptiid, icbid, ptbquantidaderural, ptbquantidadeurbana)
					(SELECT $ptiid, icbid, ptbquantidaderural, ptbquantidadeurbana
					 FROM emenda.ptiniciativabeneficiario
					 WHERE ptiid = $ptiidPai)";
			
			$this->executar( $sql );
		}
		
	}
	
	public function inserePtparceladesembolsoFilhoDAO( $ptrid, $ptridpai, $prdidPai, $arPrdid = array() ){
		if( $arPrdid[0] ){
			$sql = "UPDATE emenda.ptparceladesembolso SET ptrid = $ptrid
	 				WHERE prdid in (".implode( ',', $arPrdid ).")";			
			$this->executar( $sql );
		} else {
			$sql = "INSERT INTO emenda.ptparceladesembolso(ptrid, prddata, prdtipo, prdminuta)
					(SELECT $ptrid, prddata, prdtipo, prdminuta 
					FROM emenda.ptparceladesembolso 
					WHERE ptrid = $ptridpai and prdid = $prdidPai) RETURNING prdid";
			
			return $this->pegaUm( $sql );
		}		
	}
	
	public function inserePtparcelainiciativaFilhoDAO( $prdid, $ptiid, $prdidPai, $ptiidPai, $arPrdid = array() ){
		if( $arPrdid[0] ){
			$sql = "UPDATE emenda.ptparcelainiciativa SET ptiid = $ptiid
	 				WHERE prdid in (".implode( ',', $arPrdid ).")";	
		} else {
			$sql = "INSERT INTO emenda.ptparcelainiciativa( prdid, ptiid, privalor )
		      		(SELECT $prdid, $ptiid, privalor
					 FROM emenda.ptparcelainiciativa 
					 WHERE prdid = $prdidPai and ptiid = $ptiidPai)";
		}
		$this->executar( $sql );
	}
	
	public function inserePtescolasbeneficiadasFilhoDAO( $ptrid, $ptridpai, $esbid = null ){
		$filtro = '';
		/*if( $esbid ){
			$sql = "UPDATE emenda.ptescolasbeneficiadas SET ptrid = $ptrid
	 				WHERE esbid = " . $esbid[0];
			$this->executar( $sql );
			$filtro = " and esbid <> " . $esbid[1];
		} */
		$sql = "INSERT INTO emenda.ptescolasbeneficiadas(ptrid, entid, esbquantidadealunos)
				(SELECT $ptrid, entid, esbquantidadealunos
				FROM emenda.ptescolasbeneficiadas
				WHERE ptrid = $ptridpai)";
		
		$this->executar( $sql );
	}
	
	public function inserePtemendadetalheentidadeFilhoDAO( $ptrid, $ptridpai){
		$sql = "INSERT INTO emenda.ptemendadetalheentidade( ptrid, edeid, pedvalor) 
				(SELECT $ptrid, edeid, pedvalor
				FROM emenda.ptemendadetalheentidade
				WHERE ptrid = $ptridpai )";
		
		$this->executar( $sql );
	}
	public function insereExecucaoFinanceiraFilhoDAO( $ptrid, $ptridpai, $pedid, $pedidPai ){
		$sql = "INSERT INTO emenda.execucaofinanceira( tpeid, pliid, ptrid, pedid, exfvalor, exfcodmunicipiosiafi, exfnumsolempenho,
  					exfespecieempenho, exfstatus, exfcodfontesiafi, semid, exfdataalteracao, exfanooriginal, exfnumempenhooriginal,
  					exfidpai, exfnaturezadespesa, exfverifsiafi, exfverifcadin, exfdatainclusao, usucpf, plicod, ptres) 
				(SELECT tpeid, pliid, $ptrid, $pedid, exfvalor, exfcodmunicipiosiafi, exfnumsolempenho, 
					exfespecieempenho, exfstatus, exfcodfontesiafi, semid, exfdataalteracao, exfanooriginal, exfnumempenhooriginal,
  					exfidpai, exfnaturezadespesa, exfverifsiafi, exfverifcadin, exfdatainclusao, usucpf, plicod, ptres
  				 FROM emenda.execucaofinanceira WHERE ptrid = $ptridpai and exfstatus = 'A' and pedid = $pedidPai) RETURNING exfid";
		//$this->insereLogTxt( 'execucaofinanceira', $sql );
		return $this->pegaUm( $sql );
	}
	public function inserePtminutaConvenioFilhoDAO( $ptrid, $ptridpai ){
		$sql = "INSERT INTO emenda.ptminutaconvenio(ptrid, pmctexto, pmcstatus, xpmcdiasvigencia, xpmcdatainicio, xpmcdatafim,
  					pmcobjeto, xpmcdatapublicacao, pmcresolucao, pmcdataconversaosiafi, pmcdataassinatura, pmcnumconveniosiafi,
  					pmcdtatualizaconveniosiafi, xpmctextopublicacao, pmcdatarap, xpmcpagdou, mdoid, pmcdatainclusao,
  					usucpfinclusao, pmcdataalteracao, usucpfalteracao) 
				(SELECT $ptrid, pmctexto, pmcstatus, xpmcdiasvigencia, xpmcdatainicio, xpmcdatafim, 
					pmcobjeto, xpmcdatapublicacao, pmcresolucao, pmcdataconversaosiafi, pmcdataassinatura, pmcnumconveniosiafi,
  					pmcdtatualizaconveniosiafi, xpmctextopublicacao, pmcdatarap, xpmcpagdou, mdoid, pmcdatainclusao,
					usucpfinclusao, pmcdataalteracao, usucpfalteracao 
				 FROM emenda.ptminutaconvenio WHERE ptrid = $ptridpai and pmcstatus = 'A') RETURNING pmcid";
		
		return $this->pegaUm( $sql );
	}
	
	public function insereIntervenienteConvenioFilhoDAO( $pmcid, $ptridpai ){
		$sql = "INSERT INTO emenda.intervenienteconvenio( entidinterveniente, entiddirigente, pmcid, itcstatus) 
				(SELECT entidinterveniente, entiddirigente, $pmcid, itcstatus FROM emenda.intervenienteconvenio WHERE pmcid = (SELECT pmcid FROM emenda.ptminutaconvenio WHERE ptrid = $ptridpai and pmcstatus = 'A') and itcstatus = 'A')";
		
		$this->executar( $sql );
	}
	public function insereContaCorrenteFilhoDAO( $ptrid, $ptridpai ){
		$sql = "INSERT INTO emenda.contacorrente( ptrid, cocnu_seq_mov_conta, cocseq_conta, cocds_razao_social, cocandamento,
  					cocnu_banco, cocnu_agencia, coccnpj, cocsituacao_conta, cocdatainclusao, cocdsproblema, cocnu_conta_corrente) 
				(SELECT $ptrid, cocnu_seq_mov_conta, cocseq_conta, cocds_razao_social, cocandamento,
  					cocnu_banco, cocnu_agencia, coccnpj, cocsituacao_conta, cocdatainclusao, cocdsproblema, cocnu_conta_corrente
				 FROM emenda.contacorrente WHERE ptrid = $ptridpai) RETURNING cocid";
		
		return $this->pegaUm( $sql );
	}
	public function inserePtvigenciaFilhoDAO( $ptrid, $ptridpai, $pmcid = 'pmcid', $arrTiposRef = array() ){
		#pega o ultima vigencia com status ativo
		$sql = "select ptrid from emenda.ptvigencia 
				where ptrid in (SELECT ptrid FROM emenda.planotrabalho WHERE ptrcod = 
									(SELECT ptrcod FROM emenda.planotrabalho WHERE ptrid = $ptridpai))
					and vigstatus = 'A'
				order by ptrid desc limit 1";
		$ptridpaiN = $this->pegaUm($sql);
		
		if( in_array(APOSTILAMENTO, $arrTiposRef) || in_array(RENDIMENTO_DE_APLICACAO, $arrTiposRef) || in_array(ALTERACAO_DE_ITENS_DA_ESPECIFICACAO, $arrTiposRef) ){
			$status = "'I'";
		} else {
			$status = "vigstatus";
		}
		
		$sql = "INSERT INTO emenda.ptvigencia(ptrid, pmcid, vigdatainicio, vigdias, vigdatafim, 
  					vigstatus, vigdatainclusao, usucpfinclusao, refid, vigtipo) 
				(SELECT $ptrid, $pmcid, cast(vigdatafim as date) + interval '1 day' as vigdatainicio, null, null, 
  					$status, vigdatainclusao, usucpfinclusao, null, 'C' FROM emenda.ptvigencia WHERE ptrid = $ptridpaiN and vigstatus = 'A' LIMIT 1)";
		
		$this->executar( $sql );
	}
	public function insereptMinutaReformulacaoFilhoDAO( $ptrid, $ptridpai, $pmcid ){
		$sql = "UPDATE emenda.ptminreformulacao SET ptrid = $ptrid, pmcid = $pmcid WHERE ptrid = $ptridpai and refsituacaoreformulacao = 'C' RETURNING refid";
		$refid = $this->pegaUm( $sql );
		
		$sql = "UPDATE emenda.planotrabalho SET refid = $refid WHERE ptrid = $ptrid";
		$this->executar( $sql );
		
		return $refid;
	}
	public function insereptReformulatiposFilhoDAO( $refid, $ptridpai, $ptridFilho ){
		$sql = "SELECT refid, refsituacaoreformulacao FROM emenda.ptminreformulacao WHERE ptrid = $ptridFilho";
		$arRefid = $this->carregar( $sql );
		
		foreach ($arRefid as $v) {
			$sql = "INSERT INTO emenda.reformulatipos(trefid, refid) 
				(SELECT trefid, ".$v['refid']." FROM emenda.reformulatipos WHERE refid in ( 
							SELECT refid FROM emenda.ptminreformulacao WHERE ptrid = $ptridpai and refstatus = 'A' and refsituacaoreformulacao = '".$v['refsituacaoreformulacao']."' ) )";	
			//$this->insereLogTxt( 'reformulatipos', $sql );
			$this->executar( $sql );
		}
	}
	public function insereptPtpublicacaoFilhoDAO( $refid, $pmcid, $ptridpai ){
		$sql = "INSERT INTO emenda.ptpublicacao(pmcid, refid, pubtxtpublicacao, pubdatapublicacao, pubpagdou,
  					pubdatainclusao, usucpfinclusao, mdoid, pubstatus) 
				(SELECT $pmcid, $refid, pubtxtpublicacao, pubdatapublicacao, pubpagdou,
  					pubdatainclusao, usucpfinclusao, mdoid, pubstatus FROM emenda.ptpublicacao WHERE pmcid = (SELECT pmcid FROM emenda.ptminutaconvenio WHERE ptrid = $ptridpai and pmcstatus = 'A') and pubstatus = 'A')";
		
		$this->executar( $sql );
	}
	public function insereptAnaliseFilhoDAO( $ptrid, $ptridpai, $arrTiposRef ){
		
		if( !in_array(9, $arrTiposRef) ){
			$filtro = " and anatipo <> 'T' ";
		}
		
		$sqlAnalise = "SELECT $ptrid, uniid, anatipo, analote, anasituacaoparecer, anaparecer, usucpf,
  					anadataconclusao, anadatainclusao, anaidpai, anastatus, anaposterior FROM emenda.analise WHERE ptrid = $ptridpai and anastatus = 'A' $filtro";

		$sql = "INSERT INTO emenda.analise(ptrid, uniid, anatipo, analote, anasituacaoparecer, anaparecer, usucpf,
  					anadataconclusao, anadatainclusao, anaidpai, anastatus, anaposterior) 
				($sqlAnalise) RETURNING anaid";
		
		$this->pegaUm( $sql );
	}
	public function insereptExecfinanceiraHistoricoFilhoDAO( $exfid, $ptridpai, $pedidPai ){
		$sql = "INSERT INTO emenda.execfinanceirahistorico( usucpf, exfid, semid, exfvalor,
  					exfespecieempenho, efhiddataalteracao, exfverifsiafi, exfverifcadin) 
				(SELECT usucpf, $exfid, semid, exfvalor, exfespecieempenho, efhiddataalteracao, 
					exfverifsiafi, exfverifcadin FROM emenda.execfinanceirahistorico WHERE exfid in ( SELECT exfid FROM emenda.execucaofinanceira WHERE ptrid = $ptridpai and exfstatus = 'A' and pedid = $pedidPai ) )";
		
		$this->executar( $sql );
	}
	public function insereOrdembancariaFilhoDAO( $exfid, $exfidpai ){
		$sql = "INSERT INTO emenda.ordembancaria( exfid, orbmesparcela, orbanoparcela, orbvalorparcela, orbnumsolicitacao, spgcodigo, orbnumordembancaria,
  												  orbvalorpagamento, orbdataemissao, orbdatapagamento, usucpf, orbdatainclusao, orbdataalteracao) 
				(SELECT $exfid, orbmesparcela, orbanoparcela, orbvalorparcela, orbnumsolicitacao, spgcodigo, orbnumordembancaria,
  												  orbvalorpagamento, orbdataemissao, orbdatapagamento, usucpf, orbdatainclusao, orbdataalteracao
  				 FROM emenda.ordembancaria WHERE exfid = $exfidpai) RETURNING orbid";
		
		return $this->pegaUm( $sql );
	}
	public function insereOrdembanchistoricoFilhoDAO( $orbid, $orbidpai ){
		$sql = "INSERT INTO emenda.ordembanchistorico(orbid, spgcodigo, usucpf, orbvalorpagamento, obhdataalteracao) 
				(SELECT $orbid, spgcodigo, usucpf, orbvalorpagamento, obhdataalteracao FROM emenda.ordembanchistorico WHERE orbid = $orbidpai)";
		
		$this->executar( $sql );
	}
	public function insereptContaCorrenteHistoricoFilhoDAO( $cocid, $ptridpai ){
		$sql = "INSERT INTO emenda.contacorrentehistorico(cocid, cocandamento, cocsituacao_conta,
  					usucpf, ccdatainclusao, cocdsproblema) 
				(SELECT $cocid, cocandamento, cocsituacao_conta,
  					usucpf, ccdatainclusao, cocdsproblema FROM emenda.contacorrentehistorico WHERE cocid in (SELECT cocid FROM emenda.contacorrente WHERE ptrid = $ptridpai) )";
		
		$this->executar( $sql );
	}
	public function insereAnexoFilhoDAO( $ptrid, $anaid, $mailid, $ptridpai, $refid ){
		//$sql = "select anx.anxid, anx.arqid from emenda.anexo anx where ptrid = $ptridpai and anx.anxtipo = 'P' and anx.gdoid is null";
		//$arrAnexos = $this->carregar( $sql );
		//$arrAnexos = $arrAnexos ? $arrAnexos : array();
		//foreach ($arrAnexos as $v) {
			/*$sql = "INSERT INTO public.arquivo(arqnome, arqdescricao, arqextensao, arqtipo, arqtamanho, arqdata, arqhora, arqstatus, usucpf, sisid)
					(SELECT arqnome, arqdescricao, arqextensao, arqtipo, arqtamanho, arqdata, arqhora, arqstatus, usucpf, sisid 
						FROM public.arquivo WHERE arqid = {$v['arqid']}) returning arqid";
			
			$arqid = $this->pegaUm($sql);*/
			
			$sql = "INSERT INTO emenda.anexo(arqid, ptrid, anxdsc, anaid, mailid, anxtipo, anxtermoref, refid, anxprotocolo, anxdata, gdoid)
					(select arqid, $ptrid, anxdsc, $anaid, $mailid, anxtipo, anxtermoref, $refid, anxprotocolo, anxdata, gdoid 
						from emenda.anexo where ptrid = {$ptridpai})";
			$this->executar($sql);
		//}
	}
	public function inserePtmailFilhoDAO( $ptrid, $ptridpai ){
		$sql = "INSERT INTO emenda.ptmail(ptrid, usucpf,
  					mailtxtconteudo, maildataenvio, maildatainclusao) 
				(SELECT $ptrid, usucpf,
  					mailtxtconteudo, maildataenvio, maildatainclusao FROM emenda.ptmail WHERE ptrid = $ptridpai) RETURNING mailid";
		
		return $this->pegaUm( $sql );
	}
	
	public function insereLogTxt( $tabela, $log){
		if( $log ){
			$arquivo = APPRAIZ.'arquivos/emenda/PTA/logInsereFilhos.txt';
			$arq = fopen( $arquivo, 'a+' );			
			fwrite($arq, $tabela.': ' . $log."\n");
			fclose($arq);	
		}
	}
	
	/**
	 * Função verifica se existe emenda para o responsavel
	 *
	 * @param integer $resid
	 */
	public function verificaExistenciaEmendaAjaxDAO($resid){
		$sql = "SELECT
				  count(res.resid) as total
				FROM
				  emenda.responsavel res INNER JOIN emenda.emenda eme
				  ON (res.resid = eme.resid) INNER JOIN emenda.emendadetalhe emd
				  ON (eme.emeid = emd.emeid) INNER JOIN emenda.emendadetalheentidade ede
				  ON (emd.emdid = ede.emdid) INNER JOIN emenda.ptemendadetalheentidade pede
				  ON (ede.edeid = pede.edeid)
				WHERE
				  res.resstatus = 'A'
				  AND ede.edestatus = 'A'
				  AND res.resid = $resid";
		return $this->pegaUm($sql);
	}
	
	/**
	 * Função altera o plano de trabalho
	 *
	 * @param array de post $post
	 */
	public function alteraPlanoTrabalhoDAO($post){
		
		$ptrvalorproponente = $this->retiraPontos($post['ptrvalorproponente']);
		$porcentagem_contrapartida = $this->retiraPontos($post['porcentagem_contrapartida']);
		$ptrrendimentoaplicacao = ($post['ptrrendimentoaplicacao'] ? $this->retiraPontos($post['ptrrendimentoaplicacao']): 'null');
		
		if( is_numeric($porcentagem_contrapartida) ){
			$ptrcontrapartida = $post['porcentagem_contrapartida'] ? $this->retiraPontos($post['porcentagem_contrapartida']) : 'NULL';
		} else {
			$ptrcontrapartida = 'null';
		}
		
		$sql = "UPDATE 
				  emenda.planotrabalho  
				SET
				  ptrvalorproponente = ".$ptrvalorproponente.",
				  ptrcontrapartida = ".$ptrcontrapartida.",
				  ptrrendimentoaplicacao = {$ptrrendimentoaplicacao}
				WHERE 
				  ptrid = {$post['ptrid']}";

		$this->executar($sql);

		$xml  = "";
		$xml .= "<ptrvalorproponente>$ptrvalorproponente</ptrvalorproponente>";
		$xml .= "<ptrcontrapartida>$ptrcontrapartida</ptrcontrapartida>";

		$sql = "INSERT INTO
				emenda.historicogeral
				(
					tabid,
					histidtabela,
					histtxtalteracao,
					histdatainclusao,
					usucpfinclusao,
					ptrid
				)
			VALUES
				(
					'1',
					'{$post['ptrid']}',
					'{$xml}',
					NOW(),
					'{$_SESSION[usucpf]}',
					'{$post['ptrid']}'
				)";		
		$this->executar($sql);
		
		$this->commit();
	}
	
	/**
	 * Função retira pontos de valores monetario
	 *
	 * @param money $valor
	 * @return money
	 */
	public function retiraPontos($valor){
		$valor = str_replace(".","", $valor);
		$valor = str_replace(",",".", $valor);
		
		return $valor;
	}
	
	public function insereEmendasDetalheAjaxDAO($ptrid, $edeid){
		$sql = "INSERT INTO 
				  emenda.ptemendadetalheentidade(
				  ptrid,
				  edeid,
				  pedvalor
				) 
				VALUES (
				  $ptrid,
				  $edeid,
				  (select edevalor from emenda.emendadetalheentidade where edeid = ".$edeid.")
				)";
			return $this->executar($sql);
	}
	
	public function alteraEmendasDetalheAjaxDAO($ptrid, $edeid, $pedid){
		$sql = "UPDATE 
				  emenda.ptemendadetalheentidade  
				SET 
				  ptrid = $ptrid,
				  edeid = $edeid,
			  	  pedvalor = (select edevalor from emenda.emendadetalheentidade where edeid = ".$edeid.")
				WHERE 
				  pedid = $pedid";

		return $this->executar($sql);
	}
	
	/**
	 * Função retorna o codigo do documento vinculado ao plano de trabalho
	 *
	 * @param integer $ptrid
	 * @return docid
	 */
	public function pegarDocid( $ptrid ) {	
		$sql = "SELECT
					docid
				FROM
					emenda.planotrabalho
				WHERE
				 	ptrid = " . (integer) $ptrid;
		
		return (integer) $this->pegaUm( $sql );
	}
	
	/**
	 * Ações do Plano de Trabalho
	 */
	
	/**
	 * Função que deleta as iniciativas vinculadas ao PTA
	 *
	 */
	public function deletaIniciativaPTADAO( $ptiid ){
		$sql = "DELETE FROM 
				  emenda.ptiniesprecurso
				WHERE 
				  pteid in (select pteid from emenda.ptiniciativaespecificacao
						 where ptiid=$ptiid)";
		$this->executar($sql);
		
		$sql = "DELETE FROM 
				  emenda.ptparcelainiciativa 
				WHERE 
				  ptiid = $ptiid";
		$this->executar($sql);
				  
		$sql = "DELETE FROM emenda.ptitemkit
				WHERE
				pteid in (select pteid from emenda.ptiniciativaespecificacao
						 where ptiid=$ptiid)";
		$this->executar($sql);
		$sql = "DELETE FROM emenda.ptiniciativaespecificacao
				WHERE
				ptiid=$ptiid";
		$this->executar($sql);
		$sql = "DELETE FROM emenda.ptiniciativabeneficiario
				WHERE
				ptiid=$ptiid";
		$this->executar($sql);
		$sql = "DELETE FROM emenda.ptiniciativa
				WHERE
				ptiid=$ptiid";
		$this->executar($sql);
		return $this->commit();		
	}
	
	/**
	 * Função pega o estado atual que se encontra o plano de trabalho
	 *
	 * @param integer $ptrid
	 * @return estado do pta
	 */
	public function pegarEstadoAtualDAO( $docid ) {
		 
		$sql = "select
					ed.esdid
				from 
					workflow.documento d
				inner join 
					workflow.estadodocumento ed on ed.esdid = d.esdid
				where
					d.docid = " . $docid;

		return $this->pegaUm( $sql );
	}
	
	/**
	 * Função verifica se a existe iniciativa vinculada ao pta com status ativo
	 *
	 * @param integer $ptrid
	 * @return boolean
	 */	
	public function verificaExclusaoIniciativaPTADAO($ptrid){
		
		$sql = "SELECT
					count(pmcnumconveniosiafi) as total
				FROM 
					emenda.ptminutaconvenio 
				WHERE 
					ptrid = $ptrid
					and pmcstatus = 'A' 
					--and pmcnumconveniosiafi is null";
		
		return $this->pegaUm($sql);
	}
	
	/**
	 * Função carrega registro cadastrado na tabela ptiniciativa
	 *
	 * @param integer $ptrid
	 * @param html $acao
	 * @return array
	 */
	public function carregaDadosIniciativaPTA( $ptrid, $acao ){
		$sql = "SELECT
				  '".$acao."' as acao, 
				  pti.ptiid as codigo,
				  ini.ininome as descricao,
				  pti.ptivalortotal,
				  pti.ptivalorconcedente,
				  pti.ptivalorproponente
				FROM
				  emenda.v_ptiniciativa pti INNER JOIN emenda.iniciativa ini
				  ON (pti.iniid = ini.iniid)
				WHERE
				  pti.ptrid = ".$ptrid."
				  --and ini.inistatus = 'A'";
	
		return $this->carregar($sql);
	}
	
	/**
	 * Função carrega os beneficiarios vinculado a iniciativa e ao plano de trabalho
	 *
	 * @param integer $ptiid
	 * @return array
	 */
	public function ListarbeneficiariosAcaoPlanoTrabalhoDAO($ptiid){
		$sql = "SELECT * FROM(
					SELECT DISTINCT
						ptib.ptbquantidaderural as rural,
					  	ptib.ptbquantidadeurbana as urbana,
					  	ben.bennome as beneficiario,
					  	(ptib.ptbquantidaderural + ptib.ptbquantidadeurbana) as total
					FROM
					  	emenda.ptiniciativa pti 
					  	INNER JOIN emenda.ptiniciativabeneficiario ptib 
					  		ON (pti.ptiid = ptib.ptiid) 
					    RIGHT JOIN emenda.iniciativabeneficiario inb
					  		ON (ptib.icbid = inb.icbid) 
					    INNER JOIN emenda.beneficiario ben
					  		ON (inb.benid = ben.benid)
					WHERE
					  	inb.icbstatus = 'A'
					  	AND ben.benstatus = 'A'
					   	".($ptiid ? " AND pti.ptiid = $ptiid " : "")." 
					
					UNION
					
					SELECT DISTINCT
						ptb.ptbquantidaderural as rural,
					  	ptb.ptbquantidadeurbana as urbana,
					  	b.bennome as beneficiario,
					  	(ptb.ptbquantidaderural + ptb.ptbquantidadeurbana) as total
					FROM
					  	emenda.ptiniciativa pt 
					  	INNER JOIN emenda.ptiniciativabeneficiario ptb 
					  		ON (pt.ptiid = ptb.ptiid) 
					    inner JOIN emenda.iniciativabeneficiario ib
					  		ON (ptb.icbid = ib.icbid) and ib.icbstatus = 'A'
					    INNER JOIN emenda.beneficiario b
					  		ON (ib.benid = b.benid)
					WHERE
						".($ptiid ? " pt.ptiid = $ptiid " : "")."
					) as t
					ORDER BY
						beneficiario";
		
		return $this->carregar($sql);
	}
	
	/**
	 * Função carrega o totalizador do beneficiario vinculado a iniciativa
	 *
	 * @param integer $ptrid
	 * @return html
	*/
	public function totalizaBeneficiariosPlanoTrabalhoDAO( $ptrid ){
		$sql = "SELECT DISTINCT
				  sum(ptib.ptbquantidaderural) as rural,
				  sum(ptib.ptbquantidadeurbana) as urbana, 
				  sum(ptib.ptbquantidaderural + ptib.ptbquantidadeurbana) as total
				FROM
				  emenda.ptiniciativa pti INNER JOIN emenda.ptiniciativabeneficiario ptib 
				  ON (pti.ptiid = ptib.ptiid) RIGHT JOIN emenda.iniciativabeneficiario inb
				  ON (ptib.icbid = inb.icbid)
				WHERE
				  inb.icbstatus = 'A' 
				  AND pti.ptrid = ".$ptrid;
		
		return $this->carregar($sql);
	}
	
	/**
	 * Função carrega tipo de ensino vinculado a iniciativa
	 *
	 * @param integer $iniid
	 * @return array
	 */
	public function carregaIniciativaTipoEnsinoPTADAO( $iniid ){
		$sql = "SELECT 
				  te.tpeid as codigo,
				  te.tpedsc as descricao
				FROM 
				  emenda.tipoensino te INNER JOIN emenda.iniciativatipoensino ite
				  ON (te.tpeid = ite.tpeid)
				WHERE
				  te.tpestatus = 'A'
				  AND ite.iniid = $iniid
				  and te.resid in (SELECT resid FROM emenda.iniciativaresponsavel ir WHERE ir.iniid = $iniid and ir.irestatus = 'A')";
				  
		return $this->carregar($sql);
	}
	
	/**
	 * Função insere vinculo da iniciativa com plano de trabalho
	 *
	 * @param integer $ptrid
	 * @param string $descricao
	 * @param integer $iniid
	 * @param integer $tpeid
	 * @return ptiid
	 */
	public function insereIniciativaPTADAO( $ptrid, $descricao, $iniid, $tpeid ){
		$sql = "INSERT INTO emenda.ptiniciativa( ptrid, ptidescricao, iniid, tpeid) 
				VALUES ( ".$ptrid.", '".pg_escape_string($descricao)."', ".$iniid.", ".$tpeid." )RETURNING ptiid;";
		
		$ptiid = $this->pegaUm($sql);
		return $ptiid;
	}

	/**
	 * Função altera vinculo da iniciativa com plano de trabalho
	 *
	 * @param integer $ptrid
	 * @param string $descricao
	 * @param integer $iniid
	 * @param integer $tpeid
	 * @param integer $ptiid
	 * @return boolean
	 */
	public function alteraIniciativaPTADAO( $ptrid, $descricao, $iniid, $tpeid, $ptiid ){
		$sql = "UPDATE 
				  emenda.ptiniciativa  
				SET 
				  ptrid = ".$ptrid.",
				  ptidescricao = '".pg_escape_string($descricao)."',
				  iniid = ".$iniid.",
				  tpeid = ".$tpeid."
				 
				WHERE 
				  ptiid = ".$ptiid;
  
		$this->executar($sql);
		return $this->commit();
	}
	
	/**
	 * Função carrega os dados da iniciaitva cadastrados
	 *
	 * @param integer $ptiid
	 * @return array
	 */
	public function carregaDadosIniciativaPTADAO( $ptiid ){
		$sql = "SELECT 
				  pti.ptiid,
				  pti.ptrid,
				  pti.ptidescricao,
				  pti.iniid,
				  pti.tpeid
				FROM 
				  emenda.ptiniciativa pti
				WHERE
				  pti.ptiid = {$ptiid}";
		
		return $this->pegaLinha($sql);
	}
	
	/**
	 * Função monta sql de iniciativa vinculada ao plano de trabalho
	 *
	 * @param integer $ptrid
	 * @return query
	 */
	public function montaSQLDadosAcaoPTADAO( $ptrid ){
		$sql = "SELECT DISTINCT
				  ini.iniid as codigo,
				  ini.ininome as descricao
				FROM
				  emenda.iniciativa ini INNER JOIN emenda.ptiniciativa pti
				  ON (ini.iniid = pti.iniid) INNER JOIN emenda.planotrabalho ptr
				  ON (pti.ptrid = ptr.ptrid) INNER JOIN emenda.iniciativaemendadetalhe ied
				  ON (ini.iniid = ied.iniid)
				WHERE
				  ptr.ptrstatus = 'A'
				  --AND ini.inistatus = 'A'
				  AND ied.iedstatus = 'A'
				  AND ptr.ptrid = ".$ptrid;
		
		return $sql;
	}
	
	/**
	 * Função monta sql de iniciativa que não foi vinculada ao plano de trabalho
	 *
	 * @param integer $ptrid
	 * @return query
	 */
	public function montaSQLDadosAcaoDAO( $ptrid ){
		$sql = "SELECT distinct 
					ini.iniid as codigo, 
					ini.ininome as descricao
				FROM 
					emenda.planotrabalho ptr
					INNER JOIN emenda.ptemendadetalheentidade  ped
					    ON ptr.ptrid = ped.ptrid
					INNER JOIN emenda.emendadetalheentidade ede
					    ON ede.edeid = ped.edeid
					INNER JOIN emenda.emendadetalhe emd
					    ON emd.emdid = ede.emdid
					INNER JOIN emenda.emenda eme
					    ON eme.emeid = emd.emeid
					INNER JOIN emenda.iniciativaemendadetalhe ied
					    ON ied.emdid = emd.emdid
					INNER JOIN emenda.iniciativa ini
					    ON ini.iniid = ied.iniid
					LEFT JOIN emenda.ptiniciativa pti
					    ON pti.iniid = ini.iniid
					    AND pti.ptrid = ptr.ptrid
					inner join emenda.iniciativadetalheentidade iede
                       	on iede.edeid = ede.edeid
                        and iede.iniid = ini.iniid
                        and iede.idestatus = 'A'
					WHERE 
						ptr.ptrid = ".$ptrid." 
						AND pti.ptiid is NULL
						AND ied.iedstatus = 'A'
    					AND ede.edestatus = 'A'";
		
		return $sql;
	}
	
	/**
	 * Função carrega cabeçalho da iniciativa especificação
	 *
	 * @param integer $ptiid
	 * @return array
	 */
	public function carregaCabecalhoIniciativaEspecificacaoDAO( $ptiid ){
		$sql = "SELECT 
				  pti.ptiid,
				  ini.iniid,
				  ini.ininome
				FROM 
				  emenda.ptiniciativa pti inner join emenda.iniciativa ini
				  ON (ini.iniid = pti.iniid) 
				WHERE
				  --ini.inistatus = 'A' and
				  pti.ptiid = ".$ptiid;
		
		return $this->pegalinha($sql);	
	}
	
	/**
	 * Função carrega os dados da combo especificação
	 *
	 * @param integer $ptiid
	 * @return array
	 */
	public function montaComboEspecificacaoDAO( $ptiid ){
		$sql = "SELECT
					e.espnome as descricao,
					ie.iceid as codigo
				FROM
					emenda.iniciativa i
				    inner join emenda.iniciativaespecificacao ie on ie.iniid = i.iniid
				    inner join emenda.especificacao e on e.espid = ie.espid
				    inner join emenda.especificacao_programacaoexercicio epe on epe.espid = e.espid and epe.prsano = '".$_SESSION['exercicio']."'
				    inner join emenda.ptiniciativa pti on pti.iniid = i.iniid
				    left join emenda.ptiniciativaespecificacao ptie
				    	on ptie.iceid = ie.iceid
				    	and ptie.ptiid = pti.ptiid
				    	and ptie.ptestatus = 'A'
				WHERE
					pti.ptiid = $ptiid
				    AND ie.icestatus = 'A'
				    AND e.espstatus = 'A'
				    AND i.inistatus = 'A'
				    AND ie.iceid NOT IN (SELECT 
				                            iceid
				                          FROM 
				                            emenda.ptiniciativaespecificacao
				                          WHERE ptestatus = 'A'
				                          	AND ptiid = $ptiid)
				    ORDER BY e.espnome";

		return $this->carregar( $sql );
	}
	
	/**
	 * Função carrega os dados da especificação para listagem
	 *
	 * @param integer $ptiid
	 * @param integer $ptrid
	 * @return array
	 */
	public function listaEspecificacaoCadastradasDAO( $ptiid, $ptrid, $acao ){
		
		$sql = "SELECT 
				  	  ".$acao." as acao,
				      e.espnome,
					  e.espkit,
					  e.espunidademedida,
					  ptie.ptiid,
					  ptie.pteid,
					  ptie.iceid,
					  ptie.ptequantidade,
					  ptie.ptevalorunitario,
					  (ptie.ptequantidade * ptie.ptevalorunitario) as total,
					  ptie.ptevalorproponente,
					  ( CASE WHEN ptie.ptevalorproponente is null THEN (ptie.ptequantidade * ptie.ptevalorunitario)
					         ELSE ((ptie.ptequantidade * ptie.ptevalorunitario) - ptie.ptevalorproponente) END) as concedente,
					  to_char(ptie.ptedatainicio, 'DD/MM/YYYY') as dataini,
					  to_char(ptie.ptedatafim, 'DD/MM/YYYY') as datafim
					FROM 
					  emenda.ptiniciativaespecificacao ptie INNER JOIN emenda.iniciativaespecificacao ie ON (ptie.iceid = ie.iceid) 
					  INNER JOIN emenda.especificacao e ON (ie.espid = e.espid)
					  inner join emenda.especificacao_programacaoexercicio epe on epe.espid = e.espid and epe.prsano = '".$_SESSION['exercicio']."'
					WHERE 
					    --    ie.icestatus = 'A' AND 
						--    e.espstatus = 'A' AND 
					    ptie.ptestatus = 'A' AND 
					    ptie.ptiid = $ptiid
					ORDER BY
						ptie.pteid";

		return $this->carregar($sql);
	}
	
	/**
	 * Função pega a menor e a maior data da especificação
	 *
	 * @param integer $ptiid
	 * @return array
	 */
	public function pegaDataEspecificacao( $ptiid ){
		$sql = "SELECT 
				  to_char(min(ptedatainicio), 'DD/MM/YYYY') as min_dataini, 
  				  to_char(max(ptedatafim), 'DD/MM/YYYY') as max_datafim 
				FROM 
					emenda.ptiniciativaespecificacao
				WHERE 
					ptiid = $ptiid 
					and ptestatus = 'A'";
		
		return $this->pegaLinha( $sql );
	}
	
	/**
	 * Função carrega as iniciativa vinculada a especificação e ao recurso
	 *
	 * @param integer $ptiid
	 * @param integer $pteid
	 * @param integer $ptrid
	 * @return array
	 */
	public function iniciativaEspecificacaoRecursoDAO( $ptiid, $pteid = '', $ptrid=null ){
		if ( !empty($pteid) ){
			$select = " pr.pervalor, pr.perid,";
			
			$join = " LEFT JOIN emenda.ptiniesprecurso pr
							ON pr.pedid = ped.pedid AND pr.pteid = {$pteid}";
		}
		
		/*$ptridpai = pegaPaiPTA( $ptrid );
		$ptridpai =  ( !empty($ptridpai) ? " or pt.ptrid = $ptridpai " : '');*/
	
		$sql = "SELECT DISTINCT 
				    vede.edeid, 
					vede.emecod,
					(CASE WHEN vede.emetipo = 'E' THEN 'Emenda'
						ELSE 'Complemento' END) as tipoemenda, 
				    case when vede.emerelator = 'S' then aut.autnome||' - Relator Geral' else aut.autnome end as autnome,
				    aut.autid, 
				    fup.fupdsc,
					ped.pedvalor,
					vede.gndcod,
	                (SELECT sum(sper.pervalor)
						FROM
							emenda.ptemendadetalheentidade pt 
							inner join emenda.ptiniesprecurso sper
						    	on sper.pedid = pt.pedid
						WHERE (sper.pedid = ped.pedid $ptridpai)) as pervalorutilizado,
				    {$select}
				    ped.pedid
				FROM emenda.planotrabalho ptr
					INNER JOIN emenda.ptemendadetalheentidade  ped
				    	ON ptr.ptrid = ped.ptrid
				    INNER JOIN emenda.v_emendadetalheentidade vede
				    	ON vede.edeid = ped.edeid
				    left JOIN emenda.v_funcionalprogramatica fup
				    	ON (vede.acaid = fup.acaid)
				    INNER JOIN emenda.autor aut
				    	ON aut.autid = vede.autid
				    INNER JOIN emenda.ptiniciativa pti
				    	on pti.ptrid = ptr.ptrid
				    {$join}			    
				WHERE ptr.ptrid = ".$ptrid."
					AND pti.ptiid = $ptiid
				    AND vede.edestatus = 'A'";
		
		return $this->carregar($sql);
	}
	
	/**
	 * Função carrega os registro da especificação para montar a combo de alteração
	 *
	 * @param integer $aceid
	 * @return array
	 */
	public function montaComboUnidadeDAO($aceid){
		$sql = "SELECT DISTINCT
				  e.espunidademedida,
				  e.espkit,
				  e.espvalorunitariopadrao
				FROM 
				  emenda.especificacao e 
				  INNER JOIN emenda.iniciativaespecificacao ie ON (e.espid = ie.espid)
				  inner join emenda.especificacao_programacaoexercicio epe on epe.espid = e.espid and epe.prsano = '".$_SESSION['exercicio']."'
				WHERE e.espstatus = 'A'
				  AND ie.icestatus = 'A'
				  AND ie.iceid = $aceid";

		return $this->pegaLinha($sql);
	}
	
	/**
	 * Função que verifica o valor total do proponente cadastrado para o plano de trabalho
	 *
	 * @param integer $ptrid
	 * @param string $filtro
	 * @return array
	 */
	function verificaTotalProponentePTADAO($ptrid, $filtro){
		
		$sql = "SELECT DISTINCT
					ptr.ptrvalorproponente as totalplanotrabalho,
					sum(coalesce(ptie.ptevalorproponente,0)) as totalespecificacao
				FROM 
					emenda.planotrabalho ptr
					left join emenda.ptiniciativa pti
						on pti.ptrid = ptr.ptrid
					left join emenda.ptiniciativaespecificacao ptie
						on ptie.ptiid = pti.ptiid
						and ptie.ptestatus = 'A'
						".$filtro."
				WHERE
					ptr.ptrid = ".$ptrid."
				GROUP BY
					ptr.ptrid,
					ptr.ptrvalorproponente";
		
		return $this->pegaLinha($sql);
	}
	
	/**
	 * Função alatera a especificação da iniciativa
	 *
	 * @param array $post
	 * @param money $proponente
	 * @param money $valorUnit
	 * @return boolean
	 */
	public function alteraIniciativaEspecificacao($post, $proponente, $valorUnit){
		
		$sqlData = '';
		if( $post['boreformulacao'] != '1' ){
			$sqlData = "ptedatainicio = '".formata_data_sql($post['ptedatainicio'])."',
				  		ptedatafim = '".formata_data_sql($post['ptedatafim'])."',";
		}
		$iceid = $post['aceid1'] ? $post['aceid1'] : $post['iceid'];
		$iceid = $iceid ? $iceid : null;
		
		$sql = "UPDATE 
				  emenda.ptiniciativaespecificacao  
				SET 
				  ptiid = {$post['ptiid']},
				  iceid = ".$iceid.",
				  ptequantidade = ".($post['ptequantidade'] ? $post['ptequantidade'] : $post['ptequantidadeH']).",
				  ptevalorunitario = $valorUnit,
				  ptevalorproponente = ".($proponente ? $proponente : 'NULL').",
				  $sqlData
				  ptestatus = 'A'
				 
				WHERE 
				  pteid = {$post['pteid']}";	
		$this->executar($sql);
		return $this->commit();
	}
	
	/**
	 * Função insere a especificação da iniciativa
	 *
	 * @param array $post
	 * @param money $proponente
	 * @param money $valorUnit
	 * @return integer pteid
	 */
	public function insereIniciativaEspecificacao($post, $proponente, $valorUnit){
		$quantidade = ($post['ptequantidade'] ? $post['ptequantidade'] : $post['ptequantidadeH']);

		$sql = "INSERT INTO 
				  emenda.ptiniciativaespecificacao(
				  ptiid,
				  iceid,
				  ptequantidade,
				  ptevalorunitario,
				  ptevalorproponente,
				  ptedatainicio,
				  ptedatafim,
				  ptestatus
				) 
				VALUES (
				  {$post['ptiid']},
				  ".($post['iceid'] ? $post['iceid'] : $post['aceid1']).",
				  ".$quantidade.",
				  ".$valorUnit.",
				  ".($proponente ? $proponente : 'NULL').",
				  '".formata_data_sql($post['ptedatainicio'])."',
				  '".formata_data_sql($post['ptedatafim'])."',
				  'A'
				)RETURNING pteid";
					  
		$pteid = $this->pegaUm( $sql );
		return $pteid;		
	}
	
	/**
	 * Função se existe recurso vinculado ao plano de trabalho
	 *
	 * @param integer $pteid
	 * @param integer $pedid
	 * @return integer perid
	 */
	public function verificaRecursoCadastradoDAO($pteid, $pedid){
		try{
			$filtro = $pteid ? " AND ptir.pteid = $pteid" : '';
			$sql = "SELECT
						ptir.perid
					FROM
						emenda.ptiniesprecurso ptir
					WHERE
					    ptir.pedid = $pedid
						$filtro";
			
			return $this->pegaUm($sql);
		}catch (Exception $e){
			$this->rollback();
			return 'Erro-'.$e->getMessage();
		}
	}
	
	/**
	 * Função pega o valor da emenda vinculada ao detalhe da entidade
	 *
	 * @param integer $pedid
	 * @return float
	 */
	public function retornaValorEmendaDetalheEntidadeDAO($pedid){	
		$sql = "SELECT 
				  pedvalor
				FROM 
				  emenda.ptemendadetalheentidade
				WHERE pedid = $pedid";
		return $this->pegaUm($sql);
	}
	
	/**
	 * Função verifica o total de recurso utilizado pelo plano de trabalho
	 *
	 * @param integer $pedid
	 * @param string $filtro
	 * @return float
	 */
	public function verificaTotalRecursoJaUtilizadoDAO($pedid, $filtro){
				
		$sql = "SELECT
				  sum(ptir.pervalor) as total
				FROM	
				  emenda.ptemendadetalheentidade pt 
					inner join emenda.ptiniesprecurso ptir
				    	on ptir.pedid = pt.pedid
				WHERE
				    (ptir.pedid = $pedid) ". $filtro;
		
		/*$sql = "SELECT
				  sum(ptir.pervalor) as total
				FROM	
				  emenda.ptiniesprecurso ptir
				WHERE
					ptir.pedid = $pedid
				  	". $filtro;*/
		
		return $this->pegaUm($sql);
	}
	
	/**
	 * Função altera dados da tabela ptinesprecurso
	 *
	 * @param integer $pteid
	 * @param integer $pedid
	 * @param float $pervalor
	 * @return boolean
	 */
	public function alteraInespRecurso( $pteid, $pedid, $pervalor ){
		try{
			$sql = "UPDATE 
					  emenda.ptiniesprecurso  
					SET 
					  pervalor = {$pervalor}
					 
					WHERE 
					  pteid = $pteid
					  AND pedid = {$pedid}";
			
			$this->executar($sql);
			return 1;
		} catch (Exception $e){
			$this->rollback();
			return 'Erro-'.$e->getMessage();
		}
	}
	
	/**
	 * Função insere dados da tabela ptinesprecurso
	 *
	 * @param integer $pteid
	 * @param integer $pedid
	 * @param float $pervalor
	 * @return boolean
	 */
	public function insereInespRecurso( $pteid, $pedid, $pervalor ){
		$sql = "INSERT INTO emenda.ptiniesprecurso( pteid,
													pedid,
													pervalor) 
					VALUES ( $pteid, 
							 {$pedid}, 
							 {$pervalor})";

		$this->executar($sql);
		return 1;
	}
	
	/**
	 * Função carrega os registro da iniciativa recursos vinculados com o plano de trabalho
	 *
	 * @param integer $pteid
	 * @param integer $ptrid
	 * @return array
	 */
	public function carregaIniciativaEmendaDAO($pteid, $ptrid){
		
		$sql = "SELECT DISTINCT 
				    vede.edeid, 
					vede.emecod,
					(CASE WHEN vede.emetipo = 'E' THEN 'Emenda'
						ELSE 'Complemento' END) as tipoemenda, 
				    case when vede.emerelator = 'S' then aut.autnome||' - Relator Geral' else aut.autnome end as autnome,
				    fup.fupdsc, 
				    pr.pervalor, 
				    ped.pedid,
				    vede.gndcod
				FROM emenda.planotrabalho ptr
					INNER JOIN emenda.ptemendadetalheentidade  ped
				    	ON ptr.ptrid = ped.ptrid
				    INNER JOIN emenda.v_emendadetalheentidade vede
				    	ON vede.edeid = ped.edeid
				    LEFT JOIN emenda.v_funcionalprogramatica fup
				    	ON (vede.acaid = fup.acaid)
				    INNER JOIN emenda.autor aut
				    	ON aut.autid = vede.autid
				    INNER JOIN emenda.ptiniciativa pti
				    	on pti.ptrid = ptr.ptrid
				    inner join emenda.iniciativa ini
	                	on ini.iniid = pti.iniid
				    left JOIN emenda.ptiniesprecurso pr
						ON pr.pedid = ped.pedid 
						AND pr.pteid = $pteid
				WHERE ptr.ptrid = ".$ptrid."
				    AND vede.edestatus = 'A'";
	
		return $this->carregar($sql);
	}
	
	/**
	 * Função insere especificação com status P no cadastro de kit
	 *
	 * @param array $post
	 * @return pteid
	 */
	public function insereEspecificacaoPendenteDAO($post){
		$valorUnit = str_replace('.','', $post['ptevalorunitario']);
		$valorUnit = str_replace(',','.', $valorUnit);
		
		$Proponente = str_replace('.','', $post['ptevalorproponente']);
		$Proponente = str_replace(',','.', $Proponente);
	
		$sql = "INSERT INTO 
				  emenda.ptiniciativaespecificacao(
				  ptiid,
				  iceid,
				  ptequantidade,
				  ptevalorunitario,
				  ptevalorproponente,
				  ptedatainicio,
				  ptedatafim,
				  ptestatus
				) 
				VALUES (
				  {$post['ptiid']},
				  ".($post['iceid'] ? $post['iceid'] : $post['iceid1']).",
				  ".($post['ptequantidade'] ? $post['ptequantidade'] : $post['ptequantidadeH']).",".
				  ($valorUnit ? $valorUnit : '0').",".
				  ($Proponente ? $Proponente : '0') .",".
				  ($post['ptedatainicio'] ? "'".formata_data_sql($post['ptedatainicio'])."'" : 'now()').",".
				  ($post['ptedatafim'] ? "'".formata_data_sql($post['ptedatafim'])."'" : 'now()').",
				  'P'
				)RETURNING pteid";

		$pteid = $this->pegaUm($sql);
		$this->commit();
		return $pteid;	
	}
	
	/**
	 * Função exclui inciativas especificações vinculadas ao plano de trabalho
	 *
	 * @param integer $pteid
	 * @return boolean
	 */
	public function excluirPlanoEspecificacaoDAO($pteid){
		$sql = "DELETE FROM 
				  emenda.ptitemkit 
				WHERE 
				  pteid = $pteid";
		
		$this->executar($sql);
		
		$sql = "DELETE FROM 
				  emenda.ptiniesprecurso 
				WHERE 
				  pteid = $pteid";
		
		$this->executar($sql);
		
		$sql = "DELETE FROM 
				  emenda.ptiniciativaespecificacao 
				WHERE 
				  pteid = $pteid";
		  
		$this->executar($sql);
		return $this->commit();		
	}
	
	/**
	 * Função carrega os registro de especificação vinculado ao plano de trabalho para alteração
	 *
	 * @param integer $pteid
	 * @return array
	 */
	public function carregaPlanoEspecificacaoDAO($pteid){
		$sql = "SELECT 
				  ptie.pteid,
				  ptie.ptiid,
				  ptie.iceid,
				  ptie.ptequantidade,
				  ptie.ptevalorunitario,
				  (ptie.ptequantidade * ptie.ptevalorunitario) as total,
				  ptie.ptevalorproponente,
				  ( CASE WHEN ptie.ptevalorproponente is null THEN (ptie.ptequantidade * ptie.ptevalorunitario)
				         ELSE ((ptie.ptequantidade * ptie.ptevalorunitario) - ptie.ptevalorproponente) END) as concedente,
				  to_char(ptie.ptedatainicio, 'DD/MM/YYYY') as dataini,
				  to_char(ptie.ptedatafim, 'DD/MM/YYYY') as datafim,
				  e.espunidademedida,
				  e.espvalorunitariopadrao,
				  e.espkit,
				  ('<select style=\"width: 250px;\" disabled=\"disabled\"><option>'|| e.espnome ||'</option></select>') as espnome
				FROM 
				  emenda.ptiniciativaespecificacao ptie 
				INNER JOIN emenda.iniciativaespecificacao ie ON (ptie.iceid = ie.iceid) 
				INNER JOIN emenda.especificacao e ON (ie.espid = e.espid)
				inner join emenda.especificacao_programacaoexercicio epe on epe.espid = e.espid and epe.prsano = '".$_SESSION['exercicio']."'
				WHERE ptie.pteid = $pteid";
	
		return $this->pegaLinha($sql);		
		//$sql = "SELECT pedid, pervalor FROM emenda.ptiniesprecurso WHERE pteid = {$pteid}";
	}
	
	/**
	 * Função carrega recurso vinculado ao plano de trabalho
	 *
	 * @param integer $pteid
	 * @return array
	 */
	public function carregaPTARecursoDAO($pteid){
		$sql = "SELECT 
				  perid,
				  pteid,
				  pedid,
				  pervalor
				FROM 
				  emenda.ptiniesprecurso
				WHERE 
				  pteid = $pteid";
		
		return $this->pegaLinha($sql);
	}
	
	/**
	 * Função exclui kit da especificação
	 *
	 * @param integer $pteid
	 * @return array
	 */
	public function excluirKitEspecifDAO($pteid){
		$sql = "DELETE FROM 
				  emenda.ptitemkit 
				WHERE 
				  pteid = $pteid";
				  
		$this->executar($sql);
		return $this->commit();
	}
	
	/**
	 * Função verifica se existe kit cadastrado para a especificação
	 *
	 * @param integer $pteid
	 * @return array
	 */
	public function verificaExistenciaKitDAO($pteid){
		$sql = "SELECT 
				  count(ptkid) as total
				FROM 
				  emenda.ptitemkit
				WHERE pteid = $pteid";
		
		return $this->pegaUm($sql);
	}

	/**
	 * Função verifica se existe um especificação vinculada ao plano de trabalho
	 *
	 * @param integer $ptrid
	 * @param integer $pteid
	 * @return integer
	 */
	public function validaEspecificacaoPTADAO($ptrid, $pteid){
		$sql = "SELECT 
					count(ptie.pteid) as total
				FROM 
					emenda.ptiniciativa pti
					inner join emenda.ptiniciativaespecificacao ptie
				  		on ptie.ptiid = pti.ptiid
				WHERE
					pti.ptrid = {$ptrid}
				    AND ptie.pteid = $pteid";
				    
		return $this->pegaUm($sql);
	}
	
	/**
	 * Função carrega dados da especificação para o kit da especificação
	 *
	 * @param integer $pteid
	 * @return array
	 */
	public function carregaDadosEspecificacaoKitDAO( $pteid ){
		$sql = "SELECT e.espnome, 
		    e.espunidademedida,
		    i.ininome,
            sum(ptk.ptkquantidade * ptk.ptkvalorunitario) as total
		FROM emenda.especificacao e 
			INNER JOIN emenda.iniciativaespecificacao ie ON (e.espid = ie.espid) 
			INNER JOIN emenda.iniciativa i ON (ie.iniid = i.iniid) 
			INNER JOIN emenda.ptiniciativaespecificacao ptie ON (ie.iceid = ptie.iceid) 
			LEFT JOIN emenda.ptitemkit ptk ON ptk.pteid = ptie.pteid
			inner join emenda.especificacao_programacaoexercicio epe on epe.espid = e.espid and epe.prsano = '".$_SESSION['exercicio']."'
		WHERE ptie.pteid = $pteid
        GROUP BY
        	e.espnome, 
		    e.espunidademedida,
		    i.ininome";
		    
		return $this->pegaLinha($sql);
	}
	
	/**
	 * Função carrega dados cadastrados na tabela ptitemkit vinculado a especificação
	 *
	 * @param integer $pteid
	 * @param string $acao
	 * @return array
	 */
	public function carregaPtitemkitDAO($pteid, $acao = null){
		
		$acao = $acao ? "$acao as acao," : ''; 
		if( $_SESSION['exercicio'] >= '2012' ){
		$sql = "SELECT ".($acao ? "acao," : '')." ptkid, pteid, iteid, itoid, ptkdescricao, ptkunidademedida, ptkquantidade, ptkvalorunitario, total FROM (
				SELECT
					$acao
								  ptk.ptkid,
								  ptk.pteid,
								  ptk.iteid,
				                  ptk.itoid,
				                  case when pic.picdescricao is not null then pic.picdescricao else ptk.ptkdescricao end as ptkdescricao,
								  ptk.ptkunidademedida,
								  ptk.ptkquantidade,
								  ptk.ptkvalorunitario,
								  (ptk.ptkquantidade * ptk.ptkvalorunitario) as total
								FROM 
								  emenda.ptitemkit ptk
				                  left join emenda.itempar_especificacao ipe on ipe.iteid = ptk.iteid
				                  left join emenda.itempar itp on itp.ipaid = ipe.ipaid
				                  left join par.propostaitemcomposicao pic on pic.picid = itp.picid
				UNION
					SELECT
					$acao
								  ptk.ptkid,
								  ptk.pteid,
								  ptk.iteid,
				                  ptk.itoid,
				                  case when pto.ptodescricao is not null then pto.ptodescricao else ptk.ptkdescricao end as ptkdescricao,
								  ptk.ptkunidademedida,
								  ptk.ptkquantidade,
								  ptk.ptkvalorunitario,
								  (ptk.ptkquantidade * ptk.ptkvalorunitario) as total
								FROM 
								  emenda.ptitemkit ptk
				                  left join emenda.itempar_especificacao ipe on ipe.itoid = ptk.itoid
				                  left join emenda.itemobras io on io.itoid = ipe.itoid
				                  left join obras.pretipoobra pto on pto.ptoid = io.ptoid
				) as foo
				WHERE 
					pteid = $pteid
					and ptkdescricao is not null
				ORDER BY ptkdescricao";
		} else {
			$sql = "SELECT
					$acao
					  ptk.ptkid,
					  ptk.pteid,
					  ptk.iteid,
	                  ptk.itoid,
	                  ptk.ptkdescricao,
					  ptk.ptkunidademedida,
					  ptk.ptkquantidade,
					  ptk.ptkvalorunitario,
					  (ptk.ptkquantidade * ptk.ptkvalorunitario) as total
					FROM 
					  emenda.ptitemkit ptk
					WHERE 
						ptk.pteid = $pteid";
		}
		return $this->carregar($sql);
	} 
	
	/**
	 * Função carrega dados cadastrados na tabela ptitemkit vinculado a especificação
	 *
	 * @param integer $pteid
	 * @param string $acao
	 * @return array
	 */
	public function carregaAlteracaoPtitemkitDAO( $ptkid ){
		if( $_SESSION['exercicio'] >= '2012' ){
		$sql = "SELECT ptkid, pteid, iteid, ptkdescricao, ptkunidademedida, ptkquantidade, ptkvalorunitario, total FROM (
					SELECT
						ptk.ptkid,
						ptk.pteid,
						'P_'||ptk.iteid as iteid,
		                case when pic.picdescricao is not null then pic.picdescricao else ptk.ptkdescricao end as ptkdescricao,
						ptk.ptkunidademedida,
						ptk.ptkquantidade,
						ptk.ptkvalorunitario,
						trim(to_char( (ptk.ptkquantidade * ptk.ptkvalorunitario), '999G999G999D99')) as total
					FROM 
					  emenda.ptitemkit ptk
	                  left join emenda.itempar_especificacao ipe on ipe.iteid = ptk.iteid
	                  left join emenda.itempar itp on itp.ipaid = ipe.ipaid
	                  left join par.propostaitemcomposicao pic on pic.picid = itp.picid
				UNION
					SELECT
						  ptk.ptkid,
						  ptk.pteid,
		                  'O_'||ptk.itoid as iteid,
		                  case when pto.ptodescricao is not null then pto.ptodescricao else ptk.ptkdescricao end as ptkdescricao,
						  ptk.ptkunidademedida,
						  ptk.ptkquantidade,
						  ptk.ptkvalorunitario,
						  trim(to_char( (ptk.ptkquantidade * ptk.ptkvalorunitario), '999G999G999D99')) as total
						FROM 
						  emenda.ptitemkit ptk
		                  left join emenda.itempar_especificacao ipe on ipe.itoid = ptk.itoid
		                  left join emenda.itemobras io on io.itoid = ipe.itoid
		                  left join obras.pretipoobra pto on pto.ptoid = io.ptoid
				) as foo
				WHERE 
					ptkid = $ptkid
					and ptkdescricao is not null
				ORDER BY ptkdescricao";
		} else {
			$sql = "SELECT
					  ptk.ptkid,
					  ptk.pteid,
					  ptk.iteid,
	                  ptk.itoid,
	                  ptk.ptkdescricao,
					  ptk.ptkunidademedida,
					  ptk.ptkquantidade,
					  ptk.ptkvalorunitario,
					  trim(to_char( (ptk.ptkquantidade * ptk.ptkvalorunitario), '999G999G999D99')) as total
					FROM 
					  emenda.ptitemkit ptk
					WHERE 
						ptkid = $ptkid";
		}
		
		return $this->pegaLinha( $sql );
	} 
	
	/**
	 * Função altera os registros cadastrados na tabela ptitemkit
	 *
	 * @param array $post
	 * @return boolean
	 */
	public function alteraPtitemkitDAO( $post ){
		
		if( !empty($post['ptkvalorunitario']) ){
			$valorUnit = str_replace('.','', $post['ptkvalorunitario']);
			$valorUnit = str_replace(',','.', $valorUnit);
			$filtro = ", ptkvalorunitario = $valorUnit ";
		}
		
		if( isset($post['iteid']) && $post['iteid'] != 'null' ){
			$arItens = explode('_', $post['iteid']);
			if( $arItens[0] == 'P' ){
				$item = $arItens[1];
				$obras = 'null'; 
			} else {
				$item = 'null';
				$obras = $arItens[1];
			}
			$ptkdescricao = 'null';
		} else {
			$obras = 'null';
			$item = 'null';
			$ptkdescricao = "'".pg_escape_string(utf8_decode($post['ptkdescricao']))."'";
		}
		
		$sql = "UPDATE 
				  emenda.ptitemkit  
				SET 
				  pteid = {$post['pteid']},
				  iteid = $item,
				  itoid = $obras,
				  ptkdescricao = $ptkdescricao,
				  ptkunidademedida = '".iconv( "UTF-8", "ISO-8859-1", pg_escape_string($post['ptkunidademedida']) )."',
				  ptkquantidade = {$post['ptkquantidade']}
				  $filtro
				 
				WHERE 
				  ptkid = {$post['ptkid']}";	
		
		$this->executar( $sql );
		return $this->commit();
	}
	
	/**
	 * Função insere os registros cadastrados na tabela ptitemkit
	 *
	 * @param array $post
	 * @return boolean
	 */
	public function inserePtitemkitDAO( $post ){
		$valorUnit = str_replace('.','', $post['ptkvalorunitario']);
		$valorUnit = str_replace(',','.', $valorUnit);
		
		if( isset($post['iteid']) ){
			$arItens = explode('_', $post['iteid']);
			if( $arItens[0] == 'P' ){
				$item = $arItens[1];
				$obras = 'null'; 
			} else {
				$item = 'null';
				$obras = $arItens[1];
			}
			$ptkdescricao = 'null';
		} else {
			$obras = 'null';
			$item = 'null';
			$ptkdescricao = "'".pg_escape_string(utf8_decode($post['ptkdescricao']))."'";
		}
		
		$sql = "INSERT INTO 
				  emenda.ptitemkit(
				  pteid,
				  iteid,
				  itoid,
				  ptkdescricao,
				  ptkunidademedida,
				  ptkquantidade,
				  ptkvalorunitario
				) 
				VALUES (
				  {$post['pteid']},
				  $item,
				  $obras,
				  $ptkdescricao,
				  '".iconv( "UTF-8", "ISO-8859-1", pg_escape_string($post['ptkunidademedida']) )."',
				  {$post['ptkquantidade']},
				  $valorUnit
				) RETURNING ptkid";
	
		return $this->pegaUm( $sql );
	}
	
	/**
	 * Função atualiza o valor unitario na tabela especificação
	 *
	 * @param integer $pteid
	 * return boolean
	 */
	public function alteraValorUnitarioEspecificacaoDAO($pteid){
		
		$sql1 = "(SELECT COALESCE(sum(ptkquantidade * ptkvalorunitario),0) as valorUnitario FROM emenda.ptitemkit WHERE pteid = $pteid)";
	
		$sql = "UPDATE 
				  emenda.ptiniciativaespecificacao  
				SET
				  ptevalorunitario = ".$sql1."
				 
				WHERE 
				  pteid = $pteid";
		
		$this->executar($sql);
		return $this->commit();
	}
	
	/**
	 * Função verifica se existe a descrição do kit cadastra na tabela ptitemkit
	 *
	 * @param array $post
	 * @return integer
	 */
	public function verificaExistePtitemkitDAO($post){
		$sql = "SELECT 
				  count(ptkdescricao) as total
				FROM 
				  emenda.ptitemkit
				WHERE lower(ptkdescricao) = lower('".trim($post['ptkdescricao'])."')
				AND pteid = ".$post['pteid'];

		return $this->pegaUm($sql);
	} 
	
	/**
	 * Função exclui registro da tabela ptitemkit
	 *
	 * @param integer $ptkid
	 * @return boolean
	 */
	public function excluirPtitemkitDAO($ptkid){
	
		$sql = "DELETE FROM 
				  emenda.ptitemkit 
				WHERE 
				  ptkid = $ptkid";
				  
		$this->executar($sql);
		return $this->commit();
	
	}
	
	/**
	 * exclui as inciativas vinculadas ao plano de trabalho
	 *
	 * @param integer $ptiid
	 */
	public function excluirPtiniciativabeneficiarioDAO( $ptiid ){
		$sql = "DELETE FROM emenda.ptiniciativabeneficiario
				WHERE ptiid = ".$ptiid;
		$this->executar( $sql );
	}
	
	/**
	 * Função carrega os beneficiarios cadastrados
	 *
	 * @param integer $iniid
	 * @return array
	 */
	public function carregaBeneficiarioDAO( $iniid ){
		$sql = "SELECT b.benid as codigo
				FROM emenda.beneficiario b, emenda.iniciativabeneficiario ib
				WHERE	b.benid=ib.benid AND 
						ib.iniid = $iniid AND
						ib.icbstatus='A'";
		return $this->carregar($sql);
	}
	
	/**
	 * Função insere os dados do beneficiarios
	 *
	 * @param integer $ptiid
	 * @param integer $icbid
	 * @param integer $rural
	 * @param integer $urbana
	 * @return boolean
	 */
	public function inserePtiniciativabeneficiarioDAO( $ptiid, $icbid, $rural, $urbana ){
		$sql = "INSERT INTO emenda.ptiniciativabeneficiario
					(ptiid, icbid, ptbquantidaderural, ptbquantidadeurbana)
					VALUES
					(".$ptiid.", $icbid, ".((is_int($rural))?"'".$rural."'":"'0'").", ".((is_int($urbana))?"'".$urbana."'":"'0'").") RETURNING ptbid";
		return $this->pegaUm($sql);
	}
	
	/**
	 * Função carrega beneficiario vinculado a iniciatia e ao plano de trabalho
	 *
	 * @param integer $ptiid
	 * @return array
	 */
	public function carregaBeneficiarioIniciativaPTADAO( $ptiid ){
		
		$sql = "SELECT 
					max(b.benid)
				FROM emenda.beneficiario b, emenda.iniciativabeneficiario ab, emenda.ptiniciativa pt
				WHERE b.benid=ab.benid AND 
				b.benstatus='A' AND 
				ab.icbstatus='A' AND 
				pt.ptiid=".$ptiid;
		
		return $this->pegaUm($sql);
	}
	
	/**
	 * Função carrega o total de beneficiarios vinculado ao plano de trabalho cadastrados
	 *
	 * @param integer $ptiid
	 * @return total
	 */
	public function carregaTotalBeneficiarioPTADAO( $ptiid ){
		
		$sql = "SELECT count(*) 
				FROM emenda.beneficiario b, emenda.iniciativabeneficiario ab, emenda.ptiniciativabeneficiario ptb, emenda.ptiniciativa pta
				WHERE b.benid=ab.benid AND ptb.icbid = ab.icbid AND pta.ptiid = ptb.ptiid AND pta.ptiid = $ptiid";
		
		return $this->pegaUm($sql);
	}
	
	/**
	 * Função retorna o sql beneficiario sem vinculo a iniciativa
	 *
	 * @param integer $ptiid
	 * @return SQL
	 */
	public function montaSQLBeneficiarioPTADAO( $ptiid, $ptrid='', $iniid='' ){
		
		/*$sql  = "SELECT * FROM(
					SELECT ptb.ptbid, b.benid, ab.icbid, b.bennome, ab.icbobrigatorio as obrigatorio,
											CASE WHEN ptb.ptbquantidaderural=0
											THEN null
											ELSE ptb.ptbquantidaderural
											END as rural,
											
											CASE WHEN ptb.ptbquantidadeurbana=0
											THEN null
											ELSE ptb.ptbquantidadeurbana
											END as urbana,
											
											CASE WHEN (ptb.ptbquantidaderural+ptb.ptbquantidadeurbana)=0
											THEN null
											ELSE (ptb.ptbquantidaderural+ptb.ptbquantidadeurbana)
											END as total
											
									FROM 
										emenda.beneficiario b, 
										emenda.iniciativabeneficiario ab, 
										emenda.ptiniciativabeneficiario ptb, 
										emenda.ptiniciativa pta
									WHERE 
										b.benid=ab.benid 
										AND ptb.icbid = ab.icbid 
										AND pta.ptiid = ptb.ptiid 
										AND b.benstatus='A' 
										AND ab.icbstatus='A' 
										AND pta.ptiid = $ptiid
					UNION
					SELECT ptb.ptbid, b.benid, ab.icbid, b.bennome, ab.icbobrigatorio as obrigatorio,
											CASE WHEN ptb.ptbquantidaderural=0
											THEN null
											ELSE ptb.ptbquantidaderural
											END as rural,
											
											CASE WHEN ptb.ptbquantidadeurbana=0
											THEN null
											ELSE ptb.ptbquantidadeurbana
											END as urbana,
											
											CASE WHEN (ptb.ptbquantidaderural+ptb.ptbquantidadeurbana)=0
											THEN null
											ELSE (ptb.ptbquantidaderural+ptb.ptbquantidadeurbana)
											END as total
											
									FROM 
										emenda.beneficiario b, 
										emenda.iniciativabeneficiario ab, 
										emenda.ptiniciativabeneficiario ptb, 
										emenda.ptiniciativa pta
									WHERE 
										b.benid=ab.benid 
										AND ptb.icbid = ab.icbid 
										AND pta.ptiid = ptb.ptiid 
										AND ab.icbstatus='A' 
										AND pta.ptiid = $ptiid
					) as b
					ORDER BY
						bennome";*/
		$sql = "SELECT ptb.ptbid, b.benid, ab.icbid, b.bennome, ab.icbobrigatorio as obrigatorio,
				        CASE WHEN ptb.ptbquantidaderural=0 THEN null
				        	ELSE ptb.ptbquantidaderural END as rural,											
				        CASE WHEN ptb.ptbquantidadeurbana=0 THEN null
				        	ELSE ptb.ptbquantidadeurbana END as urbana,											
				        CASE WHEN (ptb.ptbquantidaderural+ptb.ptbquantidadeurbana)=0 THEN null
					        ELSE (ptb.ptbquantidaderural+ptb.ptbquantidadeurbana) END as total											
				FROM 
				    emenda.beneficiario b
				    inner join emenda.iniciativabeneficiario ab on ab.benid = b.benid
				    left join emenda.ptiniciativabeneficiario ptb
					    inner join emenda.ptiniciativa pta on pta.ptiid = ptb.ptiid
				        	and pta.ptiid = $ptiid
				    		and pta.ptrid = $ptrid
				    on ptb.icbid = ab.icbid
				WHERE 
					b.benstatus='A' 
				    and ab.icbstatus='A'
				    and ab.iniid = $iniid
				ORDER BY
					b.bennome";
		return $sql;
	}
	
	/**
	 * Função retorna o sql beneficiario com vinculo a iniciativa
	 *
	 * @param integer $ptiid
	 * @param integer $iniid
	 * @return SQL
	 */
	/*public function montaSQLBeneficiarioIniciativaPTADAO( $ptiid, $iniid ){
		
		$sql  = "SELECT 
					'' as ptbid,
					b.benid, 
				    ab.icbid, 
				    b.bennome, 
				    ab.icbobrigatorio as obrigatorio, 
				    null, 
				    null, 
				    null
				FROM
					emenda.ptiniciativa pt
				    inner join emenda.iniciativa ini
				    	on ini.iniid = pt.iniid
				    inner join emenda.iniciativabeneficiario ab
				    	on ab.iniid = ini.iniid
				        AND ab.icbstatus = 'A'
					inner join emenda.beneficiario b
				    	on b.benid = ab.benid
				        AND b.benstatus = 'A'
				WHERE
				    pt.ptiid = $ptiid 
				    AND ab.iniid = $iniid
				ORDER BY 
					b.bennome";
		
		return $sql;
	}*/
	
	/**
	 * Função carrega os totais de beneficiarios
	 *
	 * @param integer $ptiid
	 * @return array
	 */
	public function carregaTotaisBeneficiarios( $ptiid ){
		$sql  = "SELECT 'TOTAL' as titulo,
		
					CASE WHEN sum(ptb.ptbquantidaderural)=0
					THEN null
					ELSE sum(ptb.ptbquantidaderural)
					END as total_rural,
					
					CASE WHEN sum(ptb.ptbquantidadeurbana)=0
					THEN null
					ELSE sum(ptb.ptbquantidadeurbana)
					END as total_urbana,
		
					CASE WHEN (sum(ptb.ptbquantidaderural)+sum(ptb.ptbquantidadeurbana))=0
					THEN null
					ELSE (sum(ptb.ptbquantidaderural)+sum(ptb.ptbquantidadeurbana))
					END as total_geral
	
				FROM emenda.beneficiario b, emenda.iniciativabeneficiario ab, emenda.ptiniciativabeneficiario ptb, emenda.ptiniciativa pta
				WHERE b.benid=ab.benid AND ptb.icbid = ab.icbid AND pta.ptiid = ptb.ptiid AND pta.ptiid = $ptiid AND ab.icbstatus='A'";
		
		return $this->carregar($sql);
	}

	/**
	 * Função lista as escolas beneficiadas
	 *
	 * @param integer $ptrid
	 * @return array
	 */
	public function listaEscolaBeneficiadaDAO( $ptrid ){
		
		$sql = "SELECT DISTINCT
					peb.esbid, 
				    ent.entid,
				    ent.entcodent,
				    ent.entnome, 
				    tpl.tpldesc,
				    peb.esbquantidadealunos
				FROM
					emenda.ptescolasbeneficiadas peb
				    inner join entidade.entidade ent
				    	on ent.entid = peb.entid
				    inner join entidade.funcaoentidade fent
				        ON fent.entid = ent.entid
				    inner join entidade.tipolocalizacao tpl
				    	ON ent.tplid = tpl.tplid
				WHERE
					ent.entstatus = 'A'
				    and fent.funid in(3,4,11,12,16,18,56) 
				    and peb.ptrid = $ptrid";
			
		return $this->carregar($sql);
	}
}
?>