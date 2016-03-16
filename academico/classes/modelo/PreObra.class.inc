<?php
	
class PreObra extends Modelo{
	
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "obras.preobra";	

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "preid" );

    /**
     * Atributos
     * @var array
     * @access protected
     */    
    protected $arAtributos     = array(
									  	'preid' => null, 
									  	'docid' => null, 
									  	'presistema' => null,
    									'preidsistema' => null, 
									  	'ptoid' => null,
									  	'preobservacao' => null,
									  	'prelogradouro' => null,
									  	'prenumero' => null,
									  	'precomplemento' => null,
									  	'estuf' => null,
									  	'muncod' => null,
    									'estufpar' => null,
									  	'muncodpar' => null,
									  	'precep' => null,
									  	'prelatitude' => null,
									  	'prelongitude' => null,
									  	'predtinclusao' => null,
									  	'prebairro' => null,
    									'predescricao' => null,
    									'preano' => null,
    									'pretipofundacao' => null,
    									'prestatus' => NULL,
    									'entcodent' => NULL,
    									'preprioridade' => NULL,
    									'tooid' => NULL 
									  );
									  
	public function recuperarPreObra($preid)
	{ 
		$sql = "SELECT 
					pre.preid,
					pre.docid,
					pre.presistema,
					pre.preidsistema,
					pre.ptoid,
					pre.preobservacao,
					pre.prelogradouro,
					pre.precomplemento,
					pre.estuf,
					pre.muncod,
					pre.precep,
					pre.prelatitude,
					pre.prelongitude,
					pre.predtinclusao,
					pre.prebairro,
					pre.preano,
					pre.qrpid,
					pre.predescricao,
					pre.prenumero,
					pre.pretipofundacao,
					pre.entcodent,
					pto.ptodescricao,
					pto.ptoprojetofnde,
					mun.mundescricao,
					pre.preidpai
				FROM obras.preobra pre
				LEFT JOIN territorios.municipio mun ON mun.muncod = pre.muncod
				INNER JOIN obras.pretipoobra pto ON pto.ptoid = pre.ptoid
				WHERE preid = '{$preid}'
				AND prestatus = 'A'";
		
		return $this->pegaLinha($sql);
	}
	
	public function recuperarDescricaoPreObraPorId($preid)
	{
		$sql = "SELECT										
					predescricao					 
				FROM obras.preobra 
				WHERE preid = '{$preid}'";
		
		return $this->pegaUm($sql);
	}
	
	public function excluiDadosPreObra($preid)
	{
		
		if($preid){
			$sql = "delete from obras.precronograma where preid = $preid;";
			$sql.= "delete from obras.preplanilhaorcamentaria where preid = $preid;";
			$this->executar($sql);
			if($this->commit($sql)){
				return true;
			}else{
				return false;
			}
		}else{
			return false;
		}
		
	}
									  
	public function verificaTipoObra($icoid, $sisid = null)
	{
		$sisid = ($sisid) ? $sisid : $_SESSION['sisid'];
		
		$where = "WHERE preidsistema = '{$icoid}'";
		if($sisid == 23){
			$where = "WHERE preid = '{$icoid}'";
		}	
		
		$sql = "SELECT 
					ptoid
				FROM {$this->stNomeTabela} 
				{$where} 
				AND presistema = '{$sisid}'";
				
		return $this->pegaUm($sql);
	}
								  
	public function verificaObraFNDE($icoid, $sisid = null)
	{
		$sisid = ($sisid) ? $sisid : $_SESSION['sisid'];
		
		$where = "WHERE preidsistema = '{$icoid}'";
		if($sisid == 23){
			$where = "WHERE preid = '{$icoid}'";
		}	
		
		$sql = "SELECT 
					pto.ptoprojetofnde 
				FROM {$this->stNomeTabela} po
				INNER JOIN obras.pretipoobra pto ON pto.ptoid = po.ptoid 
				{$where} 
				AND presistema = '{$sisid}'";
				
		return $this->pegaUm($sql);
	}
									  
	public function verificaClassificacaoObra($icoid, $sisid = null)
	{
		$sisid = ($sisid) ? $sisid : $_SESSION['sisid'];
		
		$where = "WHERE preidsistema = '{$icoid}'";
		if($sisid == 23){
			$where = "WHERE preid = '{$icoid}'";
		}	
		
		$sql = "SELECT 
					ptoclassificacaoobra 
				FROM 
					obras.preobra po
				INNER JOIN
					obras.pretipoobra pto ON pto.ptoid = po.ptoid
				{$where} 
				AND presistema = '{$sisid}'";
		return $this->pegaUm($sql);
	}
	
	public function verificaTipoFundacao($preid)
	{
				
		$where = "WHERE preid = '{$preid}'";

		$sql = "SELECT 
					pretipofundacao 
				FROM {$this->stNomeTabela} 
				{$where} 
				AND preid = '{$preid}'";
//				ver($sql);
		return $this->pegaUm($sql);
	}
	
	public function verificaCategoriaObra($preid)
	{
		$sql = "SELECT 
					pretipofundacao 
				FROM 
					obras.preobra
				WHERE 
					preid = {$preid}";
//		$sql = "select 
//					itctipofundacao 
//				from obras.preitenscomposicao itc
//				inner join obras.preplanilhaorcamentaria ppo ON itc.itcid = ppo.itcid
//				where ppo.preid = {$preid}";
		
		return $this->pegaUm($sql);
	}
	
	public function verificaPlanilhaOrcamentaria($icoid, $sisid = null, $preid = null)
	{
		
		$sistema = $sisid != null ? $sisid : $_SESSION['sisid'];
		
		$stWhere = "WHERE pre.preid = '{$preid}'";
//		if($icoid){
//			$stWhere = "WHERE pre.preidsistema = '{$icoid}'";
//		}

		$ptoid = $this->verificaTipoObra($preid, $sistema);
		
		if( ($ptoid == 2) || ($ptoid == 7) ){
			$stInner = " AND pre.pretipofundacao = itc.itctipofundacao ";
		}
		
		$sql = "SELECT 
					count(itc.itcid) as itcid,
					count(ppo.ppoid) as ppoid,
					sum(coalesce(ppo.ppovalorunitario, 0)*itc.itcquantidade) as valor,
					pre.ptoid
				FROM {$this->stNomeTabela} pre
				INNER JOIN obras.preitenscomposicao itc ON pre.ptoid = itc.ptoid AND itcquantidade > 0 {$stInner} 
				LEFT JOIN obras.preplanilhaorcamentaria ppo ON itc.itcid = ppo.itcid AND ppo.preid = {$preid}  
				{$stWhere} 
					AND itc.itcid NOT IN (7778,8437,20249,20748)
					AND pre.presistema = '{$sistema}'
				GROUP BY pre.ptoid";				
//		ver($sql);		
		return $this->pegaLinha($sql);
	}
	
	public function recuperarPreObraPorPreid($preid)
	{
		$sql = "SELECT 
					preid, 
				  	docid, 
				  	presistema,
    				preidsistema, 
				  	ptoid,
				  	preobservacao,
				  	prelogradouro,
				  	prenumero,
				  	precomplemento,
				  	estuf,
				  	muncod,
				  	precep,
				  	prelatitude,
				  	prelongitude,
				  	prebairro,
				  	predtinclusao,
				  	predescricao,
				  	preano,
				  	pretipofundacao
				FROM {$this->stNomeTabela} 
				WHERE preid = '{$preid}'";
		
		return $this->pegaLinha($sql);
		
	}
	
	public function recuperarPreObraPorIcoid($icoid, $boSomentePreid = false)
	{
		if($boSomentePreid){
			$sql = "SELECT preid FROM {$this->stNomeTabela} WHERE preidsistema = '{$icoid}' AND presistema = '{$_SESSION['sisid']}'";
			return $this->pegaUm($sql);
		} else {
			$sql = "SELECT 
						preid, 
					  	docid, 
					  	presistema,
	    				preidsistema, 
					  	ptoid,
					  	preobservacao,
					  	prelogradouro,
					  	prenumero,
					  	precomplemento,
					  	estuf,
					  	muncod,
					  	precep,
					  	prelatitude,
					  	prelongitude,
					  	prebairro,
					  	predtinclusao,
					  	pretipofundacao
					FROM {$this->stNomeTabela} 
					WHERE preidsistema = '{$icoid}' 
					AND presistema = '{$_SESSION['sisid']}'";
			return $this->carregar($sql);
		}
		
	}
	
	public function recuperarPreObraPorMuncodSemQuadraCobertura($campo, $sisid)
	{
		$tipo	   = $campo['tipo']  == 'M'		? 'AND pto.ptoesfera in (\'M\',\'T\')'  : '';
		$nulMuncod = $campo['campo'] == 'estuf' ? 'AND pto.ptoesfera in (\'E\',\'T\') AND pre.muncod IS NULL' : 'AND pto.ptoesfera in (\'M\',\'T\')';
		$sql = "SELECT 
					pre.preid, 
				  	pre.docid, 
				  	pre.presistema,
				  	pre.estuf,
				  	pre.muncod,
				  	pre.predtinclusao,
				  	pre.predescricao,
				  	pre.preano,
				  	pre.pretipofundacao,
				  	esd.esdid,
				  	esd.esddsc,
					pto.ptodescricao,
					pre.preprioridade
				FROM {$this->stNomeTabela} pre
				INNER JOIN obras.pretipoobra pto ON pre.ptoid = pto.ptoid
				LEFT JOIN workflow.documento doc ON doc.docid = pre.docid
				LEFT JOIN workflow.estadodocumento esd ON esd.esdid = doc.esdid 
				WHERE 
					pre.".$campo['campo']." = '".$campo['valor']."'
					$nulMuncod
					$tipo
					AND pre.presistema = '{$sisid}'
					--AND pre.ptoid <> ".OBRA_QUADRA_COBERTURA."
					AND pto.ptoclassificacaoobra = 'P'
					AND pre.prestatus = 'A'
					AND pre.tooid =".ORIGEM_OBRA_PAC2."
				ORDER BY pre.preprioridade";
		return $this->carregar($sql);
	}

	public function recuperarPreObraPorMuncod($campo, $sisid)
	{
		$sql = "SELECT 
					pre.preid, 
				  	pre.docid, 
				  	pre.presistema,
				  	pre.estuf,
				  	pre.muncod,
				  	pre.predtinclusao,
				  	pre.predescricao,
				  	pre.preano,
				  	pre.pretipofundacao,
				  	esd.esdid,
				  	esd.esddsc,
					pto.ptodescricao,
					pre.preprioridade
				FROM {$this->stNomeTabela} pre
				INNER JOIN obras.pretipoobra pto ON pre.ptoid = pto.ptoid
				LEFT JOIN workflow.documento doc ON doc.docid = pre.docid
				LEFT JOIN workflow.estadodocumento esd ON esd.esdid = doc.esdid 
				WHERE 
					pre.".$campo['campo']." = '".$campo['valor']."'
					AND pre.presistema = '{$sisid}'
					AND pre.prestatus = 'A'
					AND pre.tooid =".$campo['tooid']."
				ORDER BY pre.preprioridade";
		return $this->carregar($sql);
	}
	
	public function recuperarTermosPorMuncod($campo, $sisid)
	{
		$sql = "SELECT 
					pre.preid, 
				  	pre.docid, 
				  	pre.presistema,
				  	pre.estuf,
				  	pre.muncod,
				  	pre.predtinclusao,
				  	pre.predescricao,
				  	pre.preano,
				  	pre.pretipofundacao,
				  	esd.esdid,
				  	esd.esddsc,
					pto.ptodescricao,
					pre.preprioridade
				FROM {$this->stNomeTabela} pre
				INNER JOIN obras.pretipoobra pto ON pre.ptoid = pto.ptoid
				LEFT JOIN workflow.documento doc ON doc.docid = pre.docid
				LEFT JOIN workflow.estadodocumento esd ON esd.esdid = doc.esdid 
				WHERE 
					pre.".$campo['campo']." = '".$campo['valor']."'
					AND pre.presistema = '{$sisid}'
					AND pre.prestatus = 'A'
					AND pre.tooid =".$campo['tooid']."
				ORDER BY pre.preprioridade";
		return $this->carregar($sql);
	}
	
	public function recuperarPreObraPorMuncodComQuadraCobertura($campo, $sisid)
	{
		$tipo	   = $campo['tipo']  == 'Q'		? 'AND pto.ptoid in (5,9)' : '';
		$tipo	   = $campo['tipo']  == 'C'		? 'AND pto.ptoid in (4,8,10)'  : $tipo;
		$tipo	   = $campo['tipo']  == 'M'		? 'AND pto.ptoesfera in (\'M\',\'T\')'  : $tipo;
		$nulMuncod = $campo['campo'] == 'estufpar' ? 'AND pto.ptoesfera in (\'E\',\'T\') AND pre.muncodpar IS NULL' : 'AND pto.ptoesfera in (\'M\',\'T\')';
		$sql = "SELECT 
					pre.preid, 
				  	pre.docid, 
				  	pre.presistema,
				  	pre.estuf,
				  	pre.muncod,
				  	pre.predtinclusao,
				  	pre.predescricao,
				  	pre.preano,
				  	pre.pretipofundacao,
				  	esd.esdid,
				  	esd.esddsc,
					pto.ptodescricao,
					pre.preprioridade
				FROM {$this->stNomeTabela} pre
				INNER JOIN obras.pretipoobra pto ON pre.ptoid = pto.ptoid
				LEFT JOIN workflow.documento doc ON doc.docid = pre.docid
				LEFT JOIN workflow.estadodocumento esd ON esd.esdid = doc.esdid 
				WHERE 
					pre.".$campo['campo']." = '".$campo['valor']."'
					$nulMuncod
					$tipo
					AND pre.presistema = '{$sisid}'
					--AND pre.ptoid = ".OBRA_QUADRA_COBERTURA."
					AND pto.ptoclassificacaoobra = 'Q'
					AND pre.prestatus = 'A'
					AND pre.tooid =".ORIGEM_OBRA_PAC2."
				ORDER BY pre.preprioridade";
		
		return $this->carregar($sql);
	}
	
	public function verificaFotosObra($preid, $sisid)
	{
		$sql = "SELECT 
					count(*) 
				FROM 
					obras.preobra pre
				INNER JOIN 
					obras.preobrafotos pof on pof.preid = pre.preid
				INNER JOIN
					public.arquivo arq on arq.arqid = pof.arqid
				WHERE 
					pre.preid = {$preid} 
					AND pre.presistema = {$sisid}
					AND (substring(arq.arqtipo,1,5) = 'image') ";
					
		return $this->pegaUm($sql);
	}
	
	public function verificaDocumentosObra($preid, $sisid, $tipoObra = null, $tipoA = false)
	{
		if($tipoObra){
			$stWhere .= " AND tdo.ptoid = '{$tipoObra}' ";
		}
		
		if($tipoObra == 1 && $tipoA == false){
			$stWhere .= " AND tdo.podid NOT IN (10,11,12,13,14,15,16,17) ";
		}
		
		if($tipoObra == 1 && $tipoA == true){
			$stWhere .= " AND tdo.podid NOT IN (1,3,4,5,6,7,8,9) ";
		}
		
		if($_SESSION['par']['muncod']!=''){
			$stWhere .= " AND doc.podesfera IN ('T','M') ";
		}else{
			$stWhere .= " AND doc.podesfera IN ('T','E') ";
		}
		
		$sql = "select 
					count(oan.arqid) as arqid, 
					count(doc.podid) as podid  
				from obras.pretipodocumento doc
				inner join obras.pretipodocumentoobra tdo on tdo.podid = doc.podid
				left join obras.preobraanexo oan on oan.podid = doc.podid AND oan.preid = {$preid} 
				left join obras.preobra pre on pre.preid = oan.preid AND presistema = {$sisid}
				WHERE doc.podstatus = 'A'
				{$stWhere}";
//				ver($sql, d);
		return $this->pegaLinha($sql);
	}
	
	public function verificaQuestionario($qrpid)
	{
		$sql = "SELECT
					count(resid)
				FROM
					questionario.resposta
				WHERE
					perid in 
						( SELECT
								perid
							FROM
								questionario.pergunta
							WHERE
								grpid in 
									(SELECT
											grpid
										FROM
											questionario.grupopergunta
										WHERE
											queid = 43
									)
						)
					AND qrpid = {$qrpid}";
		
//		ver($sql, d);
		
		return $this->pegaUm($sql);
	}
	
	public function verificaCronograma($preid)
	{
		$sql = "select count(*) from obras.precronograma where preid = {$preid}";
		
		return $this->pegaUm($sql);
	}
	
	public function verificaLatitudePreObra($preid)
	{
		$sql = "select					 
					prelongitude 
				from obras.preobra 
				where preid = {$preid}
				and prelatitude not like '%...%'
				and prelatitude is not null
				and prelongitude not like '%...%'
				and prelongitude is not null";
		
		return $this->pegaUm($sql);
	}
	
	public function verificaValoresPlanilha()
	{
		$sql = "SELECT 
					sum(itc.itcquantidade*ppo.ppovalorunitario) as total	
				FROM obras.preobra pre
				INNER JOIN obras.preplanilhaorcamentaria ppo ON pre.preid = ppo.preid  
				INNER JOIN obras.preitenscomposicao itc ON itc.itcid = ppo.itcid
				WHERE pre.presistema = '23'
				AND pre.preid = 70
				AND itc.ptoid = 3";
	}
	
	public function verificaFotosObras($preid)
	{
		$sql = "select arq.arqid from obras.preobrafotos fot
				inner join public.arquivo arq on arq.arqid = fot.arqid
				where fot.preid = '{$preid}'";
		
		return $this->pegaUm($sql);
	}
	
	public function verificaAnexoObras($preid)
	{
		$sql = "select arq.arqid from obras.preobraanexo poa
				inner join public.arquivo arq on arq.arqid = poa.arqid
				where poa.preid = '{$preid}'";
		
		return $this->pegaUm($sql);
	}
	
	public function recuperarListaObras($post)
	{
		if($post){
			
			extract($post);
			
			if($predescricao){
				$stWhere .= " AND pre.predescricao iLIKE '%{$predescricao}%' ";
			}
			
			if(trim($municipio) != '' && $bogeratermo == 'true'){
				$stWhere .= " AND mun.muncod = '{$municipio}' ";
			} else {
				if(trim($municipio) != '' ){
					$stWhere .= " AND mun.mundescricao iLIKE '%{$municipio}%' ";
				}
			}
		
			if($tooid){
				$stWhere .= " AND pre.tooid = {$tooid} ";
			}
			
			if ($ptoesfera){
				$esfera = Array();
				if(in_array('M',$ptoesfera)){ array_push($esfera , 'M'); array_push($esfera , 'T');}
				if(in_array('E',$ptoesfera)){ array_push($esfera , 'E'); array_push($esfera , 'T'); $stWhere .= " AND pre.muncodpar IS NULL ";}
				$stWhere .= " AND pto.ptoesfera IN ('".implode("','",$esfera)."') ";
			}
			
			if($estuf){
				$stWhere .= " AND mun.estuf iLIKE '%{$estuf}%' ";
			}
			
			if($ptoid[0]!=''){
				$stWhere .= " AND pre.ptoid IN ('".implode("', '", $ptoid)."') ";
			}
			
			if($tpmid){
				$stWhere .= " AND tpm.tpmid IN ('".implode("', '", $tpmid)."') ";
			}
			
			if($esdid){
				$stWhere .= " AND esd.esdid = {$esdid} ";
			}

			if($usucpf){
				$stWhere .= " AND usu.usucpf = '{$usucpf}' AND doc.esdid = 215";
			}
			if($corrompido == 0){
				$stInner = "";
				$stInner = "";
			}
			if($corrompido == 1){
				$stInner .= "INNER JOIN obras.rel_arquivos arqrel ON arqrel.codigoobra = pre.preid";
			}
			if($corrompido == 2){
				$stWhere .= "AND pre.preid IN(SELECT DISTINCT a.codigoobra FROM obras.rel_arquivos a LEFT JOIN public.arquivo_recuperado b ON a.arqid = b.arqid WHERE b.arqid IS NULL)";
			}
			if($corrompido == 3){
				$stWhere .= "AND pre.preid NOT IN (SELECT DISTINCT a.codigoobra FROM obras.rel_arquivos a LEFT JOIN public.arquivo_recuperado b ON a.arqid = b.arqid WHERE b.arqid IS NULL)
							 AND pre.preid IN (SELECT DISTINCT a.codigoobra FROM obras.rel_arquivos a INNER JOIN public.arquivo_recuperado b ON a.arqid = b.arqid WHERE b.arqvalidacao = FALSE)";
			}
			if($corrompido == 4){
				$stInner .= "INNER JOIN obras.rel_arquivos arqrel ON arqrel.codigoobra = pre.preid";
				$stWhere .= "AND pre.preid NOT IN(SELECT DISTINCT a.codigoobra FROM obras.rel_arquivos a LEFT JOIN public.arquivo_recuperado b ON a.arqid = b.arqid WHERE b.arqid IS NULL OR b.arqvalidacao = FALSE)";
			}
			if($corrompido == 5){
				$stWhere .= "AND pre.preid NOT IN(SELECT DISTINCT codigoobra FROM obras.rel_arquivos)";
			}
			if($empenho == true){
				$stInner .= "INNER JOIN (SELECT DISTINCT preid FROM par.empenhoobra where eobstatus = 'A') emp ON emp.preid = pre.preid";
			}
						
			if($poausucpfinclusao){	
										
				$sql = "select docid from (
						select distinct
							max(hd1.htddata) as data,
							hd1.docid
						from workflow.historicodocumento hd1
							inner join workflow.acaoestadodoc ac1 on
								ac1.aedid = hd1.aedid
							inner join workflow.estadodocumento ed1 on
								ed1.esdid = ac1.esdidorigem
							inner join seguranca.usuario us1 on
								us1.usucpf = hd1.usucpf
							left join workflow.comentariodocumento cd1 on
								cd1.hstid = hd1.hstid
						where
							ac1.esdiddestino in (210,211,212)
						and 
							hd1.usucpf = '{$poausucpfinclusao}'
						group by hd1.docid) as foo";
				$arDocid = $this->carregar($sql);
				$arDocid = $arDocid ? $arDocid : array();
				
				foreach($arDocid as $dados){
					$arDocids[] = $dados['docid'];
				}
				
				if($arDocid){
					$filtroAnalista .= " AND doc.docid IN (".implode(', ', $arDocids).")";
				}else{
					$filtroAnalista .= " AND doc.docid IN (0)";
				}
				
			}
		}
		
		if( $bogeratermo == 'true' ){			
			$acoes = "'<center><input type=\"checkbox\" id=\"preid[]\" name=\"preid[]\" value=\"' || pre.preid || '\" /></center>'";
			$stWhere .= 'AND esd.esdid = 228';
		} else {
			
			$acoes_possui_reformulacao = "|| CASE WHEN (SELECT COUNT(po.preid) FROM obras.preobra po WHERE po.preidpai = pre.preid) > 0 THEN ' <img src=../imagens/mais.gif title=abrir style=cursor:pointer; onclick=\"exibirReformulacao('||pre.preid||', this);\">' ELSE '' END";
						
			if($this->testa_superuser() || in_array(PAR_PERFIL_COORDENADOR_TECNICO, pegaPerfilGeral())) {
				$acoes_reformulacao = "|| CASE WHEN esd.esdid=".WF_TIPO_OBRA_APROVADA." THEN '<img src=../imagens/restricao_ico.png style=cursor:pointer; onclick=\"confirmarReformulacao('||pre.preid||');\">' ELSE '' END";
			}
			$acoes = "'<img border=\"0\" src=\"../imagens/alterar.gif\" id=\"' || pre.preid || '_' || mun.muncod || '_' || doc.esdid || '\" class=\"mostra\" style=\"cursor:pointer\" /> '".$acoes_reformulacao.$acoes_possui_reformulacao;
		}
	
		if( $premcmv == '1' ){			
			$stWhere .= 'AND pre.premcmv = TRUE';
		} else {
			$stWhere .= 'AND pre.premcmv = FALSE';
		}
		
		$sql = "SELECT DISTINCT
					$acoes as acao, 
					pre.predescricao,
					pto.ptodescricao,					
					mun.mundescricao,
					--mun.muncod,
					mun.estuf,
					esd.esddsc,					
					usu.usunome,
					to_char(hstu.htddata,'DD/MM/YYYY HH24:MI:SS') as htddata,					
					(select usunome from (select distinct
								max(hd1.htddata) as data,
								us1.usunome,
								hd1.docid,
								ed1.esddsc
							from workflow.historicodocumento hd1
								inner join workflow.acaoestadodoc ac1 on
									ac1.aedid = hd1.aedid
								inner join workflow.estadodocumento ed1 on
									ed1.esdid = ac1.esdidorigem
								inner join seguranca.usuario us1 on
									us1.usucpf = hd1.usucpf
								left join workflow.comentariodocumento cd1 on
									cd1.hstid = hd1.hstid
							where
								ac1.esdiddestino in (210,211,212)
							and 
								hd1.docid = pre.docid
							group by us1.usunome, hd1.docid, ed1.esddsc, hd1.htddata 
							order by data desc limit 1) as foo) as nomeanalista,
					resnumero
				FROM obras.preobra pre
				LEFT  JOIN territorios.municipio 		 mun ON pre.muncod  = mun.muncod
				LEFT  JOIN territorios.muntipomunicipio mtpm ON mtpm.muncod = mun.muncod
				LEFT  JOIN territorios.tipomunicipio     tpm ON tpm.tpmid   = mtpm.tpmid AND tpmstatus = 'A' AND gtmid = 7
				INNER JOIN workflow.documento 			 doc ON doc.docid   = pre.docid {$filtroAnalista}
				INNER JOIN workflow.estadodocumento 	 esd ON esd.esdid   = doc.esdid				
				INNER JOIN obras.pretipoobra 			 pto ON pre.ptoid   = pto.ptoid	
				LEFT  JOIN par.resolucao				 res ON res.resid   = pre.resid			
				{$stInner}
				LEFT JOIN (SELECT h.docid, max(h.hstid) hstid FROM workflow.historicodocumento h GROUP BY docid) hst ON doc.docid=hst.docid
				LEFT JOIN workflow.historicodocumento hstu ON hstu.hstid=hst.hstid
				LEFT JOIN seguranca.usuario usu ON hstu.usucpf=usu.usucpf
				".($poausucpfinclusao ? " INNER " : " LEFT " )." JOIN (SELECT 
									poap.preid,
									poap.poausucpfinclusao,
									usup.usunome as nomeanalista
								FROM obras.preobraanalise poap 
								LEFT JOIN seguranca.usuario usup ON usup.usucpf = poap.poausucpfinclusao
						) poa ON poa.preid = pre.preid
				WHERE pre.prestatus = 'A' AND pre.preidpai IS NULL
				{$stWhere}
				ORDER BY htddata DESC";
//				ver($sql);
		return $this->carregar($sql);
	}
	
	public function recuperarListaObrasEmpenho($post)
	{
		if($post){
			
			extract($post);
			
			if($muncod){
				$stWhere .= " AND mun.muncod = '{$municipio}' ";
			} 
			
			if($estuf){
				$stWhere .= " AND mun.estuf iLIKE '%{$estuf}%' ";
			}
			
			if($ptoid){
				$stWhere .= " AND pre.ptoid IN ('".implode("', '", $ptoid)."') ";
			}
			
		}
		
		$sql = "SELECT DISTINCT
					' ' as acao, 
					pre.predescricao,
					prevalorobra
				FROM obras.preobra pre
				INNER JOIN territorios.municipio mun ON pre.muncod = mun.muncod
				INNER JOIN par.empenhoobra       eob ON eob.preid  = pre.preid and eobstatus = 'A'
				WHERE 
					pre.prestatus = 'A'
					{$stWhere}";
		
		return $this->carregar($sql);
	}
	
	public function verificaGrupoMunicipioTipoObra_A($muncod) {		
		
		$sql = "SELECT muncod FROM territorios.muntipomunicipio  WHERE tpmid = ".TIPOMUN_GRUPO1." AND muncod='".$muncod."'";
		$muncodGrupo = $this->pegaUm($sql);
		if($muncodGrupo) return true;
		else return false;
		
	}
			
	public function verificaPrazoExpiraAdesao($muncod)
	{
		$sql = "SELECT 
					count(*) 
				FROM obras.preobra pre
				INNER JOIN workflow.documento wkf ON wkf.docid = pre.docid				
				WHERE muncod = '{$muncod}'
				AND wkf.esdid IN (194, 195, 210, 211, 212, 213, 214, 215, 217, 218)
				AND pre.prestatus = 'A'";
		
		return $this->pegaUm($sql);
	}
			
	public function verificaQtdObrasEstado($dados)
	{
		$sql = "SELECT
					pttquantidadelimite-count(preid) as qtd
				FROM
					obras.preobra pre
				INNER JOIN obras.pretetoobra ptt ON ptt.estuf = pre.estufpar
				WHERE
					estufpar = '".$dados['estuf']."'
					AND muncodpar IS NULL
					AND pttobra = '".$dados['tipo']."'
					AND pre.prestatus = 'A'
				GROUP BY
					pttquantidadelimite";
		$qtd = $this->pegaUm($sql);
		$qtd = $qtd ? $qtd : 1;
		
		return $qtd > 0 ? true : false;
	}
	
	function validaPermissaoFinalizarQuestionario( $qrpid ){
			if ( !$qrpid ){
				return false;
			}
		
			$sql = "select 
						itptitulo, pertitulo 
					from 
						questionario.resposta r 
					join questionario.itempergunta it on it.itpid = r.itpid
					join questionario.pergunta p on p.perid = it.perid
					where 
						r.itpid in (
						select itpid from questionario.itempergunta where perid in (
							select perid from questionario.pergunta where grpid in ( 
								select grpid from questionario.grupopergunta where queid = 49 )))
						and qrpid = {$qrpid}";
			
			$arDados = (array) $this->carregar( $sql );
			
			$return = false;
			if ( count($arDados) < 13 ){
				foreach( $arDados as $dados ){
					if ( substr($dados['pertitulo'],0,3) == '1.2' && strpos($dados['itptitulo'], "Não") > -1 ){
						$return = true;
					}
				}
			}else{
				$return = true;
			}
			
		return $return;
	}
	
	public function pegaMuncodPorPreid( $preid )
	{
		$sql = "SELECT 
					mun.muncod, 
					mun.mundescricao 
				FROM obras.preobra pre
				INNER JOIN territorios.municipio mun ON mun.muncod = pre.muncod
				WHERE preid = {$preid}";
		
		return $this->pegaLinha($sql);
	}
	
	public function verificaArquivoCorrompido( $preid, $arqid = 0 )
	{
		$where = "";
		if( $arqid != 0 ){
			$where = "AND arqid = ".$arqid;
		}
		$sql = "SELECT DISTINCT
					TRUE as teste
				FROM obras.rel_arquivos arqrel
				LEFT JOIN public.arquivo_recuperado arqrec ON arqrec.arqid = arqrel.arqid
				WHERE
					arqrec.arqid IS NULL 
					AND codigoobra = {$preid} {$where}";
		return false;
//		return $this->pegaUm($sql);
	}
	
	public function verificaArquivoValidado( $preid, $arqid = 0 )
	{
		$where = "";
//		if( $arqid != 0 ){
//			$where = "AND arqid = ".$arqid;
//		}
//		$sql = "SELECT DISTINCT
//					arqrec.arqvalidacao
//				FROM obras.rel_arquivos arqrel
//				LEFT JOIN public.arquivo_recuperado arqrec ON arqrec.arqid = arqrel.arqid
//				WHERE
//					arqrec.arqid IS NOT NULL 
//					AND codigoobra = {$preid} {$where}";
//		$dados = $this->carregar($sql);
//		if( is_array($dados) ){
//			foreach( $dados as $dado ){
//				if( $dado == false ){
//					return false;
//				}
//			}
//		}
//		return true;
	}
	
	public function verificaAlertaArquivoCorrompido( $preid )
	{
		$sql = "SELECT DISTINCT
					TRUE as teste
				FROM obras.rel_arquivos arqrel
				LEFT JOIN public.arquivo_recuperado arqrec ON arqrec.arqid = arqrel.arqid
				WHERE
					arqrec.arqid IS NULL 
					AND codigoobra = {$preid}";
//		if( $this->pegaUm($sql) ){
//			return "<div style=\"text-align: center; font-size: 30px; color: red; border: 4px solid red; background-color: rgb(250,185,0); width: 100% \"> ALERTA: ESTA OBRA CONTÉM ARQUIVOS CORROMPIDOS!</div>";
////			$texto = "<div style=\"text-align: center; font-size: 30px; color: red; border: 4px solid red; background-color: rgb(250,185,0); width: 100% \"> ALERTA: ESTA OBRA CONTÉM ARQUIVOS CORROMPIDOS!</div>";
//		}
//		return $texto;
	}
	
	public function verificaAlertaArquivoSubstituido( $preid )
	{
		$sql = "SELECT
					arqrel.arqid as corrompido,
					arqrec.arqid as arquivo,
					arqrec.arqvalidacao as validado
				FROM obras.rel_arquivos arqrel
				LEFT JOIN public.arquivo_recuperado arqrec ON arqrec.arqid = arqrel.arqid
				WHERE
					codigoobra = {$preid}";
//		$dados = $this->carregar($sql);
//		$avisa = true;
//		$validado = true;
//		
//		if( is_array($dados) ){
//			foreach( $dados as $dado ){
//				if( $dado['arquivo'] == '' ){
//					$avisa = false;
//				}
//				if( $dado['validado'] != 't' ){
//					$validado = false;
//				}
//			}
//		}
//		if( $validado ){
//			$avisa = false;
//		}
//		if( $avisa ){
//			return "<div style=\"text-align: center; font-size: 30px; color: BLUE; border: 4px solid BLUE; background-color: rgb(200,225,157); width: 100% \">Todos os arquivos corrompidos já foram substituídos e aguardam validação.</div>";
//		}
	}
	
	function salvarPreObraSubacao($preid,$sobano,$sbaid)
	{
		
		$sql = "select sobid from par.subacaoobra where preid = $preid and sobano = '$sobano' and sbaid = '$sbaid'";
		$sobid = $this->pegaUm($sql);
		if(!$sobid){
			$sql = "insert into par.subacaoobra (sbaid, preid, sobano) values ('$sbaid','$preid','$sobano') returning sobid";
			$sobid = $this->pegaUm($sql);
			$this->commit();
			return $sobid;
		}else{
			return $sobid;
		}
	
	}
	
	function montaLista($sbaid,$sobano = null)
	{
		if($sobano){
			$ano = "and sbo.sobano = '$sobano'";
			$acao = "'<center><img class=\"middle link\" title=\"Consultar Obra\" src=\"../imagens/consultar.gif\" onclick=\"consultarObra(\'' || pre.preid || '\',\'' || sbo.sbaid || '\',\'' || sbo.sobano || '\')\"  /> <img class=\"middle link\" title=\"Excluir Obra\" src=\"../imagens/excluir.gif\" onclick=\"excluirObra(this,\'' || pre.preid || '\')\"  /> </center>' as acao,";
			$cabecalho = array("Ação","Obra","Tipo");
		}else{
			$cabecalho = array("Obra","Tipo");
		}
		
		$sql = "SELECT 
					{$acao}
					pre.predescricao,
					pto.ptodescricao
				FROM obras.preobra pre
				INNER JOIN par.subacaoobra 	sbo ON sbo.preid = pre.preid {$ano} and sbo.sbaid = '$sbaid'
				LEFT JOIN territorios.municipio mun ON mun.muncod = pre.muncod
				LEFT JOIN obras.pretipoobra pto ON pto.ptoid = pre.ptoid
				WHERE prestatus = 'A'";


		return $this->monta_lista_simples($sql,$cabecalho,100,5,"S","100%","N");
	}
	
	function excluirPreObra($preid)
	{
		$sql = "update obras.preobra set prestatus = 'I' where preid = $preid";
		return $this->executar($sql);
	}
}