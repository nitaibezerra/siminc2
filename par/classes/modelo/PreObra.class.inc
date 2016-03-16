	<?php
	
	class PreObra extends Modelo {
	
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
	    protected $arChavePrimaria = array("preid");
	
	    
	    /**
	     * Atributos
	     * @var array
	     * @access protected
	     */
	    protected $arAtributos = array(
	        'preid' => null,
	        'docid' => null,
	        'presistema' => null,
	        'preidsistema' => null,
	        'ptoid' => null,
	        'preobservacao' => null,
	        'prelogradouro' => null,
	        'prenumero' => null,
	        'precomplemento' => null,
	        'prereferencia' => null,
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
	        'preanoselecao' => null,
	        'pretipofundacao' => null,
	        'prestatus' => NULL,
	        'entcodent' => NULL,
	        'preprioridade' => NULL,
	        'tooid' => NULL,
	        'preesfera' => NULL
	    );
	
	    public function recuperarPreObra($preid) {
	        $sql = "SELECT
						pre.preid,
						pre.docid,
						pre.presistema,
						pre.preidsistema,
						pre.ptoid,
						pre.preobservacao,
						pre.prelogradouro,
						pre.precomplemento,
						pre.prereferencia,
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
						pto.ptoclassificacaoobra,
						pto.ptoexisteescola,
						pto.ptodescricao,
						pto.ptoprojetofnde,
						mun.mundescricao,
						pre.preidpai,
						pre.preesfera,
						--COALESCE(oi.obrpercexec, 0) as percexec
						COALESCE(oi.obrpercentultvistoria, 0) as percexec,
						ter.terid
					FROM obras.preobra pre
					LEFT  JOIN obras.pretipoobra 			pto ON pto.ptoid = pre.ptoid
					LEFT  JOIN par.termocompromissopac 		ter ON ter.terid = pre.terid AND terstatus = 'A'
					LEFT  JOIN territorios.municipio 		mun ON mun.muncod = pre.muncod
					LEFT  JOIN obras2.obras 				oi  ON oi.preid = pre.preid
					--LEFT  JOIN obr as.obr ainfraestrutura 	oi ON oi.preid = pre.preid
					WHERE 
						pre.preid = '{$preid}'
						AND pre.prestatus = 'A'";
	
	        return $this->pegaLinha($sql);
	    }
	
	    public function recuperarDescricaoPreObraPorId($preid) {
	        $sql = "SELECT
						predescricao
					FROM obras.preobra
					WHERE preid = '{$preid}'";
	
	        return $this->pegaUm($sql);
	    }
	
	    public function excluiDadosPreObra($preid) {
	        $dados = $this->pegaLinha("SELECT ptocategoria FROM obras.pretipoobra WHERE ptoid = " . $_POST['ptoid']);
	        if ($preid) {
	            $sql = "delete from obras.precronograma where preid = $preid;";
	            $sql.= "delete from obras.preplanilhaorcamentaria where preid = $preid;";
	            if (!$dados['ptocategoria']&&!in_array($_POST['ptoid'], Array(73,74))) {
	                $sql.= "delete from obras.preobraanexo where podid IN (1,4) AND preid = $preid;";
	            }
	            $this->executar($sql);
	            if ($this->commit($sql)) {
	                return true;
	            } else {
	                return false;
	            }
	        } else {
	            return false;
	        }
	    }
	
	    public function verificaTipoObra($icoid, $sisid = null) {
	        $sisid = ($sisid) ? $sisid : $_SESSION['sisid'];
	
	        $where = "WHERE preidsistema = '{$icoid}'";
	        if ($sisid == 23) {
	            $where = "WHERE preid = '{$icoid}'";
	        }
	
	        $sql = "SELECT
						ptoid
					FROM {$this->stNomeTabela}
					{$where}
					AND presistema = '{$sisid}'";
	
	        return $this->pegaUm($sql);
	    }
	
	    public function verificaQtdMesesTipoObra($ptoid) {
	
	        $sql = "SELECT
						ptoqtdmeses
					FROM obras.pretipoobra
					WHERE ptoid = $ptoid";
	
	        return $this->pegaUm($sql);
	    }
	
	    public function verificaObraFNDE($icoid, $sisid = null) {
	        $sisid = ($sisid) ? $sisid : $_SESSION['sisid'];
	
	        $where = "WHERE preidsistema = '{$icoid}'";
	        if ($sisid == 23) {
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
	
	    public function verificaClassificacaoObra($icoid, $sisid = null) {
	        $sisid = ($sisid) ? $sisid : $_SESSION['sisid'];
	
	        $where = "WHERE preidsistema = '{$icoid}'";
	        if ($sisid == 23) {
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
	
	    public function verificaTipoFundacao($preid) {
	
	        $where = "WHERE preid = '{$preid}'";
	
	        $sql = "SELECT
						pretipofundacao
					FROM {$this->stNomeTabela}
					{$where}
					AND preid = '{$preid}'";
	//				ver($sql);
	        return $this->pegaUm($sql);
	    }
	
	    public function verificaCategoriaObra($preid) {
	        $sql = "SELECT
						pretipofundacao
					FROM
						obras.preobra
					WHERE
						preid = {$preid}";
	
	        return $this->pegaUm($sql);
	    }
	
	    public function verificaPlanilhaOrcamentaria($icoid, $sisid = null, $preid = null) {
	
	        $sistema = $sisid != null ? $sisid : $_SESSION['sisid'];
	
	        $stWhere = "WHERE pre.preid = '{$preid}'";
	
	        $ptoid = $this->verificaTipoObra($preid, $sistema);
	
	        if (($ptoid == 2) || ($ptoid == 7)) {
	            $stInner = " AND pre.pretipofundacao = itc.itctipofundacao ";
	        }
	
	        $sql = "SELECT DISTINCT
						CASE WHEN ptoplanilhaporregiao = 'E'
							THEN count(DISTINCT pir.itcid)
							ELSE count(DISTINCT itc.itcid)
						END as itcid,
						CASE WHEN pre.ptoid IN (42,43,44,45)
							THEN count(DISTINCT pmi.itcid)
							ELSE count(DISTINCT ppo.ppoid)
						END as ppoid,
						CASE WHEN ptopreencher IS TRUE
							THEN sum( coalesce( ppo.ppovalorunitario, 0 ) * coalesce( pir.pirqtd, itc.itcquantidade ) )
							ELSE sum( coalesce( pir.pirvalor, coalesce( ppo.ppovalorunitario , 0 ) ) * coalesce( pir.pirqtd, itc.itcquantidade ) )
						END as valor,
						ptovalorminimo as minimo,
						--ptovalormaximo as maximo,
						pre.ptoid
					FROM obras.preobra pre
					INNER JOIN obras.pretipoobra 				pto ON pto.ptoid = pre.ptoid
					INNER JOIN obras.preitenscomposicao 		itc ON itc.ptoid = pto.ptoid AND itc.itcstatus = 'A' $stInner
					LEFT  JOIN obras.preitencomposicao_regiao 	pir ON pir.itcid = itc.itcid AND pir.estuf = pre.estuf
					LEFT  JOIN obras.preplanilhaorcamentaria 	ppo ON itc.itcid = ppo.itcid AND ppo.preid = pre.preid
					LEFT  JOIN obras.preitenscomposicaomi		pmi ON pmi.ptoid = pre.ptoid AND pmi.preid = pre.preid
						$stWhere
						AND itc.itcid NOT IN (7778,8437,20249,20748)
						AND pre.presistema = '$sistema'
						AND ( itc.itcquantidade > 0 OR pir.pirqtd > 0 )
					GROUP BY pre.ptoid, ptovalorminimo, ptovalormaximo, pto.ptopreencher, ptoplanilhaporregiao";
	       
	        return $this->pegaLinha($sql);
	    }
	
	    public function recuperarPreObraPorPreid($preid) {
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
	
	    public function recuperarPreObraPorIcoid($icoid, $boSomentePreid = false) {
	        if ($boSomentePreid) {
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
	
	    public function recuperarPreObraPorMuncodSemQuadraCobertura($campo, $sisid, $preano, $tooid = ORIGEM_OBRA_PAC2) {
	        $tipo = $campo['tipo'] == 'E' || ($_SESSION['par']['estuf'] != 'DF' && $_SESSION['par']['muncod'] == '') ? 'AND pto.ptoesfera in (\'E\',\'T\') --AND pre.preesfera = \'E\''  //existem obras que est�o M na ptoesfera e E na preesfera
	                : 'AND pto.ptoesfera in (\'M\',\'T\') --AND pre.preesfera = \'M\''; //existem obras que est�o M na ptoesfera e E na preesfera
	        $nulMuncod = $campo['campo'] == 'estufpar' && $_SESSION['par']['estuf'] != 'DF' ? ' AND pre.muncod IS NULL' : '';
	        $campo['campo'] = $_SESSION['par']['estuf'] == 'DF' ? 'estuf' : $campo['campo'];
	        if ($tooid == ORIGEM_OBRA_PAC2) {
	            $dado = "P";
	            $sub = "";
	            $InSub = "";
	        } else {
	            $dado = "E";
	            $sub = ",sub.sbadsc,sub.sbaid";
	            $InSub = "LEFT JOIN par.subacaoobra sbo
						  INNER JOIN par.subacao 	sub on sub.sbaid = sbo.sbaid and sub.sbastatus = 'A'
						  ON sbo.preid = pre.preid ";
	        }
	
	        //N�o � permitido a visualiza��o de obras arquivadas para os perfis Estadual e Municipal
	        if (in_array(PAR_PERFIL_EQUIPE_ESTADUAL, pegaPerfilGeral()) || in_array(PAR_PERFIL_EQUIPE_ESTADUAL_APROVACAO, pegaPerfilGeral()) || in_array(PAR_PERFIL_EQUIPE_MUNICIPAL, pegaPerfilGeral()) ||
	                in_array(PAR_PERFIL_EQUIPE_MUNICIPAL_APROVACAO, pegaPerfilGeral()) || in_array(PAR_PERFIL_PREFEITO, pegaPerfilGeral())) {
	            $obraArquivada = "AND esd.esdid NOT IN (229)";
	        } else {
	            $obraArquivada = "";
	        }
	
	        if ($preano) {
	            $anoSelecao = "AND pre.preanoselecao = 2014";
	        } else {
	            $anoSelecao = "AND pre.preanoselecao <> 2014";
	        }
	
	        $sql = "SELECT DISTINCT
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
						pre.preprioridade,
	                    esd.esddsc || 
						CASE WHEN esd.esdid IN (" . WF_PAR_EM_DILIGENCIA . ", " . WF_TIPO_EM_CORRECAO . ") THEN
								'<label style=\"color: red\"> (Inicio: ' || 
								( SELECT to_char(max(hd.htddata),'DD/MM/YYYY')
								FROM workflow.historicodocumento hd 
								WHERE docid = pre.docid AND aedid IN (2022,516,617,1815) )
								 || ' Prazo Final: ' || 
								( 
								SELECT
									to_char(CAST(max(hd.htddata) as date) + COALESCE((SELECT dioqtddia FROM ( SELECT dioqtddia, dioid FROM par.diligenciaobra WHERE preid = pre.preid and dioativo = true ORDER BY dioid DESC LIMIT 1 ) as foo ), 30) ,'DD/MM/YYYY') AS data
								FROM workflow.historicodocumento hd
								WHERE
									docid = pre.docid
									AND aedid IN (2022,516,617,1815) )
								 || ' )'
							ELSE ''
						END as estado1
						" . $sub . "
					FROM {$this->stNomeTabela} pre
					INNER JOIN obras.pretipoobra 		pto ON pre.ptoid = pto.ptoid
					LEFT JOIN workflow.documento 		doc ON doc.docid = pre.docid
					LEFT JOIN workflow.estadodocumento 	esd ON esd.esdid = doc.esdid
					" . $InSub . "
					WHERE
						pre." . $campo['campo'] . " = '" . $campo['valor'] . "'
						$nulMuncod
						$tipo
						AND pre.presistema = '{$sisid}'
						AND pto.ptoclassificacaoobra IN ('" . $dado . "')
						AND pre.prestatus = 'A'
						AND pre.tooid =" . $tooid . "
						AND pre.preidpai IS NULL
						$obraArquivada
						$anoSelecao
					ORDER BY pre.preprioridade";
	
	        return $this->carregar($sql);
	    }
	
	    public function recuperarPreObraPorMuncod($campo, $sisid) {
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
						pre." . $campo['campo'] . " = '" . $campo['valor'] . "'
						AND pre.presistema = '{$sisid}'
						AND pre.prestatus = 'A'
						AND pre.tooid =" . $campo['tooid'] . "
					ORDER BY pre.preprioridade";
	        return $this->carregar($sql);
	    }
	
	    public function recuperarTermosPorMuncod($campo, $sisid) {
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
						pre." . $campo['campo'] . " = '" . $campo['valor'] . "'
						AND pre.presistema = '{$sisid}'
						AND pre.prestatus = 'A'
						AND pre.tooid =" . $campo['tooid'] . "
					ORDER BY pre.preprioridade";
	        return $this->carregar($sql);
	    }
	
	    public function recuperarPreObraPorMuncodComQuadraCobertura($campo, $sisid, $tooid = ORIGEM_OBRA_PAC2) {
	        $tipo = $campo['tipo'] == 'Q' ? 'AND pto.ptoclassificacaoobra = \'Q\' ' : '';
	        $tipo = $campo['tipo'] == 'C' ? 'AND pto.ptoclassificacaoobra = \'C\' ' : $tipo;
	        $tipo = $campo['tipo'] == 'P' ? 'AND pto.ptoclassificacaoobra = \'P\' ' : $tipo;
	        $tipo = $campo['tipo'] == 'M' ? 'AND pto.ptoesfera in (\'M\',\'T\') AND pre.preesfera = \'M\'' : $tipo;
	        $nulMuncod = $campo['campo'] == 'estufpar' && $_SESSION['par']['tipo'] != 'P' ? 'AND pto.ptoesfera in (\'E\',\'T\') AND pre.preesfera = \'E\' AND pre.muncodpar IS NULL' : 'AND pto.ptoesfera in (\'M\',\'T\') AND pre.preesfera = \'M\'';
	
	        //N�o � permitido a visualiza��o de obras arquivadas para os perfis Estadual e Municipal
	        if (in_array(PAR_PERFIL_EQUIPE_ESTADUAL, pegaPerfilGeral()) || in_array(PAR_PERFIL_EQUIPE_ESTADUAL_APROVACAO, pegaPerfilGeral()) || in_array(PAR_PERFIL_EQUIPE_MUNICIPAL, pegaPerfilGeral()) ||
	                in_array(PAR_PERFIL_EQUIPE_MUNICIPAL_APROVACAO, pegaPerfilGeral()) || in_array(PAR_PERFIL_PREFEITO, pegaPerfilGeral())) {
	            $obraArquivada = "AND esd.esdid NOT IN (229)";
	        } else {
	            $obraArquivada = "";
	        }
	
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
						pre.preprioridade,
						esd.esddsc || 
	            		CASE WHEN esd.esdid IN (" . WF_TIPO_EM_CORRECAO . ") THEN
	            				'<label style=\"color: red\"> (Inicio: ' || 
	            				( SELECT to_char(max(hd.htddata),'DD/MM/YYYY')
	            				FROM workflow.historicodocumento hd 
	            				WHERE docid = pre.docid AND aedid IN (2022,516,617,1815) )
	            				 || ' Prazo Final: ' || 
	            				( 
	            				SELECT
	            					to_char(CAST(max(hd.htddata) as date) + COALESCE((SELECT dioqtddia FROM ( SELECT dioqtddia, dioid FROM par.diligenciaobra WHERE preid = pre.preid and dioativo = true ORDER BY dioid DESC LIMIT 1 ) as foo ), 30) ,'DD/MM/YYYY') AS data
	            				FROM workflow.historicodocumento hd
	            				WHERE
	            					docid = pre.docid
	            					AND aedid IN (2022,516,617,1815) )
	            				 || ' )'
	            			ELSE ''
	            		END as estado1
					FROM {$this->stNomeTabela} pre
					INNER JOIN obras.pretipoobra pto ON pre.ptoid = pto.ptoid
					LEFT JOIN workflow.documento doc ON doc.docid = pre.docid
					LEFT JOIN workflow.estadodocumento esd ON esd.esdid = doc.esdid
					WHERE
						pre." . $campo['campo'] . " = '" . $campo['valor'] . "'
						$nulMuncod
						$tipo
						AND pre.presistema = '{$sisid}'
						--AND pre.ptoid = " . OBRA_QUADRA_COBERTURA . "
						AND pto.ptoclassificacaoobra = 'Q'
						AND pre.prestatus = 'A'
						AND pre.tooid =" . $tooid . "
						AND pre.preidpai IS NULL
						$obraArquivada
					ORDER BY pre.preprioridade";
	//ver($sql);
	        return $this->carregar($sql);
	    }
	
	    public function recuperarPreObraPorMuncodCobertura($campo, $sisid, $tooid = ORIGEM_OBRA_PAC2) {
	        if ($campo['tipo'] == 'C' || $campo['tipo'] == 'M' || $campo['tipo'] == 'E' || $campo['tipo'] == '') {
	            if ($campo['campo'] == 'muncodpar') {
	                $v = "(pre.muncodpar = '" . $campo['valor'] . "' OR pre.muncod = '" . $campo['valor'] . "')";
	            } else {
	                $v = "pre." . $campo['campo'] . " = '" . $campo['valor'] . "'";
	            }
	            $tipo = $campo['tipo'] == 'M' ? 'AND pto.ptoesfera in (\'M\',\'T\') AND pre.preesfera = \'M\'' : $tipo;
	            $tipo = $campo['tipo'] == 'P' ? 'AND pto.ptoclassificacaoobra = \'P\' ' : $tipo;
	            $nulMuncod = $campo['campo'] == 'estufpar' ? 'AND pto.ptoesfera in (\'E\',\'T\') AND pre.muncodpar IS NULL AND pre.preesfera = \'E\'' : 'AND pto.ptoesfera in (\'M\',\'T\') AND pre.preesfera = \'M\'';
	
	            //N�o � permitido a visualiza��o de obras arquivadas para os perfis Estadual e Municipal
	            if (in_array(PAR_PERFIL_EQUIPE_ESTADUAL, pegaPerfilGeral()) || in_array(PAR_PERFIL_EQUIPE_ESTADUAL_APROVACAO, pegaPerfilGeral()) || in_array(PAR_PERFIL_EQUIPE_MUNICIPAL, pegaPerfilGeral()) ||
	                    in_array(PAR_PERFIL_EQUIPE_MUNICIPAL_APROVACAO, pegaPerfilGeral()) || in_array(PAR_PERFIL_PREFEITO, pegaPerfilGeral())) {
	                $obraArquivada = "AND esd.esdid NOT IN (229)";
	            } else {
	                $obraArquivada = "";
	            }
	
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
							pre.preprioridade,
							esd.esddsc || 
	                		CASE WHEN esd.esdid IN (" . WF_TIPO_EM_CORRECAO . ") THEN
	                				'<label style=\"color: red\"> (Inicio: ' || 
	                				( SELECT to_char(max(hd.htddata),'DD/MM/YYYY')
	                				FROM workflow.historicodocumento hd 
	                				WHERE docid = pre.docid AND aedid IN (2022,516,617,1815) )
	                				 || ' Prazo Final: ' || 
	                				( 
	                				SELECT
	                					to_char(CAST(max(hd.htddata) as date) + COALESCE((SELECT dioqtddia FROM ( SELECT dioqtddia, dioid FROM par.diligenciaobra WHERE preid = pre.preid and dioativo = true ORDER BY dioid DESC LIMIT 1 ) as foo ), 30) ,'DD/MM/YYYY') AS data
	                				FROM workflow.historicodocumento hd
	                				WHERE
	                					docid = pre.docid
	                					AND aedid IN (2022,516,617,1815) )
	                				 || ' )'
	                			ELSE ''
	                		END as estado1
						FROM {$this->stNomeTabela} pre
						INNER JOIN obras.pretipoobra pto ON pre.ptoid = pto.ptoid
						LEFT JOIN workflow.documento doc ON doc.docid = pre.docid
						LEFT JOIN workflow.estadodocumento esd ON esd.esdid = doc.esdid
						WHERE
							$v
							$nulMuncod
							$tipo
							AND pre.presistema = '{$sisid}'
							--AND pre.ptoid = " . OBRA_QUADRA_COBERTURA . "
							AND pto.ptoclassificacaoobra = 'C'
							AND pre.prestatus = 'A'
							AND pre.tooid =" . $tooid . "
							AND pre.preidpai IS NULL
							$obraArquivada
						ORDER BY pre.preprioridade";
	//				ver($sql);
	            return $this->carregar($sql);
	        }
	    }
	
	    public function verificaFotosObra($preid, $sisid) {
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
	
	    public function verificaDocumentosObra($preid, $sisid, $tipoObra = null, $tipoA = false) {
	        if ($tipoObra) {
	            $stWhere .= " AND tdo.ptoid = '{$tipoObra}' ";
	        }
	
	        if ($tipoObra == 1 && $tipoA == false) {
	            $stWhere .= " AND tdo.podid NOT IN (10,11,12,13,14,15,16,17) ";
	        }
	
	        if ($tipoObra == 1 && $tipoA == true) {
	            $stWhere .= " AND tdo.podid NOT IN (1,3,4,5,6,7,8,9) ";
	        }
	
	        $_SESSION['par']['esfera'] = $_SESSION['par']['esfera'] ? $_SESSION['par']['esfera'] : retornaEsfera($preid);
	
	        if ($_SESSION['par']['esfera'] == 'M') {
	            $stWhere .= " AND doc.podesfera IN ('T','M') ";
	        } else {
	            $stWhere .= " AND doc.podesfera IN ('T','E') ";
	        }
	
	        $sql = "select
						count(oan.arqid) as arqid,
						count(doc.podid) as podid
					from obras.pretipodocumento doc
					inner join obras.pretipodocumentoobra tdo on tdo.podid = doc.podid
					left join obras.preobraanexo oan on oan.podid = doc.podid AND oan.preid = {$preid} AND oan.poasituacao = 'A'
					left join obras.preobra pre on pre.preid = oan.preid AND presistema = {$sisid}
					WHERE doc.podstatus = 'A'
					{$stWhere}";
	
	        return $this->pegaLinha($sql);
	    }
	
	    public function verificaQuestionario($qrpid) {
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
	
	    public function verificaCronograma($preid) {
	        $sql = "select count(*) from obras.precronograma where preid = {$preid}";
	
	        return $this->pegaUm($sql);
	    }
	
	    public function verificaLatitudePreObra($preid) {
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
	
	    public function verificaValoresPlanilha() {
	        $sql = "SELECT
						sum(itc.itcquantidade*ppo.ppovalorunitario) as total
					FROM obras.preobra pre
					INNER JOIN obras.preplanilhaorcamentaria ppo ON pre.preid = ppo.preid
					INNER JOIN obras.preitenscomposicao itc ON itc.itcid = ppo.itcid
					WHERE pre.presistema = '23'
					AND pre.preid = 70
					AND itc.ptoid = 3";
	    }
	
	    public function verificaFotosObras($preid) {
	        $sql = "select arq.arqid from obras.preobrafotos fot
					inner join public.arquivo arq on arq.arqid = fot.arqid
					where fot.preid = '{$preid}'";
	
	        return $this->pegaUm($sql);
	    }
	
	    public function verificaAnexoObras($preid) {
	        $sql = "select arq.arqid from obras.preobraanexo poa
					inner join public.arquivo arq on arq.arqid = poa.arqid
					where poa.preid = '{$preid}'";
	
	        return $this->pegaUm($sql);
	    }
	
	    public function recuperarListaObrasPar($post, $inner = null) {
	        if ($post) {
	            extract($post);
	
	            if ($preid) {
	                $stWhere .= " AND pre.preid = {$preid} ";
	            }
	
	            if ($obrid) {
	                $stWhere .= " AND obr.obrid = {$obrid} ";
	            }
	
	            if ($predescricao) {
	                $stWhere .= " AND pre.predescricao iLIKE '%{$predescricao}%' ";
	            }
	
	            if (trim($municipio) != '' && $bogeratermo == 'true') {
	                $stWhere .= " AND mun.muncod = '{$municipio}' ";
	            } else {
	                if (trim($municipio) != '') {
	                    $stWhere .= " AND UPPER(public.removeacento(mun.mundescricao)) ILIKE '%'||UPPER(public.removeacento('$municipio'))||'%' ";
	                }
	            }
	
	            if ($tooid) {
	                //$stWhere .= " AND pre.tooid = {$tooid} ";
	            }
	
//				$acoes_possui_prorrogacao_prazo = "|| CASE WHEN (SELECT popstatus FROM obras.preobraprorrogacao WHERE preid = pre.preid order by popid desc limit 1) = 'A' THEN '<img width=\"16px\" title=\"Cancelar Prorroga��o de Prazo\" border=\"0\" src=\"../imagens/cancelar_prorrogacao_prazo.png\" id=\"\" onclick=\"cancelarProrrogacaoPrazo(' || pre.preid || ');\" style=\"cursor:pointer\" />' ELSE '' END";
				$acoes_possui_prorrogacao_prazo = "";
				$acoes_possui_reformulacao = "|| CASE WHEN (SELECT COUNT(po.preid) FROM obras.preobra po WHERE po.preidpai = pre.preid AND prestatus = 'A' AND preid NOT IN (SELECT preid FROM obras.preobracancelamentoreformulacao)) > 0 THEN ' <img src=../imagens/mais.gif title=abrir style=cursor:pointer; onclick=\"exibirReformulacao('||pre.preid||','||s.sbaid||','||sbo.sobano||', this);\">' ELSE '' END";
                    
	            if ($reformulacao) {
	                $stWhere .= " AND PRE.PREID IN (SELECT PREIDPAI FROM OBRAS.PREOBRA WHERE PREIDPAI IS NOT NULL AND PREIDPAI = PRE.PREID) ";
	                $stWhereWITHTerritorio .=  " AND PRE.PREID IN (SELECT PREIDPAI FROM OBRAS.PREOBRA WHERE PREIDPAI IS NOT NULL AND PREIDPAI = PRE.PREID) ";
	                $stWhereWITHEntidade  .= " AND PRE.PREID IN (SELECT PREIDPAI FROM OBRAS.PREOBRA WHERE PREIDPAI IS NOT NULL AND PREIDPAI = PRE.PREID) ";
	            }
	
	            if ($estuf) {
	                $stWhere .= " AND mun.estuf = '{$estuf}' ";
	            }
	
	            if ($ptoid[0] != '') {
	                $stWhere .= " AND pre.ptoid IN ('" . implode("', '", $ptoid) . "') ";
	            }
	
	            if ($tpmid[0] != '') {
	                $stWhere .= " AND tpm.tpmid IN ('" . implode("', '", $tpmid) . "') ";
	                $inner = " LEFT  JOIN territorios.muntipomunicipio mtpm ON mtpm.muncod = mun.muncod
							   LEFT  JOIN territorios.tipomunicipio     tpm ON tpm.tpmid   = mtpm.tpmid AND tpmstatus = 'A' --AND gtmid = 7 ";
	            }
	
	            if ($esdid) {
	                $stWhere .= " AND doc.esdid = {$esdid} ";
	            }
	
	            if ($usucpf) {
	                $stWhere .= " AND usu.usucpf = '{$usucpf}' AND doc.esdid = 215";
	            }
	
	            if ($anoatual) {
	                $stWhere .= " AND sbo.sobano = '" . $anoatual . "'";
	            }
	
	            if ($empenhado == "1") {
	                $strNotQuery = "";
	            }
	            if ($empenhado == "2") {
	                $strNotQuery = "NOT";
	            }
	            if ($empenhado == "1" || $empenhado == "2") {
	                $stWhere .= " and pre.preid {$strNotQuery} in ((SELECT DISTINCT preid FROM par.vm_saldo_empenho_por_obra where tipo = 'PAR' and saldo > 0))";
	            }
	
	            if ($poausucpfinclusao) {
	
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
	
	                foreach ($arDocid as $dados) {
	                    $arDocids[] = $dados['docid'];
	                }
	
	                if ($arDocid) {
	                    $filtroAnalista .= " AND doc.docid IN (" . implode(', ', $arDocids) . ")";
	                } else {
	                    $filtroAnalista .= " AND doc.docid IN (0)";
	                }
	            }
	
	            if ($ptoesfera[0]) {
	
	                switch ($ptoesfera[0]) {
	                    case 'M':
	                        //$whereest[] = "1=2";
	                        $stWhere .= ' and iu.muncod IS NOT NULL';
	                        break;
	                    case 'E':
	                        //$wheremun[] = "1=2";
	                        $stWhere .= ' and iu.muncod IS NULL';
	                        break;
	                    case 'T':
	                        //$wheremun[] = "1=2";
	                        $stWhere .= ' and (iu.muncod IS NULL OR iu.muncod IS NOT NULL)';
	                        break;
	                }
	            }
	            $pendenciaObras = $post['pendenciaObras'];
	            if ($pendenciaObras == '1') { // com pendencias
	                $stWhere .= " AND pre.preesfera = 'M' ";
	                $stWhere .= " AND pre.muncod in (select coalesce(muncod, '') from obras2.vm_pendencia_obras where empesfera = 'M')  ";
	            }
	
	            if ($pendenciaObras == '2') { // sem pendencias
	                $stWhere .= " AND pre.preesfera = 'M' ";
	                $stWhere .= " AND pre.muncod not in (select coalesce(muncod, '') from obras2.vm_pendencia_obras where empesfera = 'M')  ";
	            }
	
	            $pendenciaObrasUF = $post['pendenciaObrasUF'];
	            if ($pendenciaObrasUF == '1') { // com pendencias
	                $stWhere .= " AND pre.preesfera = 'E' ";
	                $stWhere .= " AND pre.estuf in (select coalesce(estuf, '') from obras2.vm_pendencia_obras where empesfera = 'E')  ";
	            }
	
	            if ($pendenciaObrasUF == '2') { // sem pendencias
	                $stWhere .= " AND pre.preesfera = 'E' ";
	                $stWhere .= " AND pre.estuf not in (select coalesce(estuf, '') from obras2.vm_pendencia_obras where empesfera = 'E')  ";
	            }
	        }
                
                #Termo Gerado
                if (!empty($termogerado)) {
                    if ($termogerado == '1') {
                        $stWhere .=" AND (SELECT COUNT(dopid) FROM par.documentopar WHERE proid = poc.proid AND dopstatus = 'A') > 0";
                    }else{
                        $stWhere .=" AND (SELECT COUNT(dopid) FROM par.documentopar WHERE proid = poc.proid AND dopstatus = 'A') = 0";
                    }
                }
                
                #Pagamento
                if (!empty($pagamento)) {
                    $strPagamento = "SELECT DISTINCT
                                        e.empnumeroprocesso
                                    FROM par.empenho e
                                    INNER JOIN par.pagamento pg ON e.empid=pg.empid AND pg.pagstatus = 'A' [[replace]]                
                                    INNER JOIN par.v_vrlempenhocancelado vve on vve.empid = e.empid
                                    WHERE e.empstatus = 'A'
                                        AND e.empcodigoespecie NOT IN ('03', '13', '02', '04')
                                        AND e.empnumeroprocesso=poc.pronumeroprocesso
                                        AND vve.vrlempenhocancelado > 0";

                    if ($pagamento == "1") { #Pagamento Solicitado
                        $stWhere .= " AND poc.pronumeroprocesso IN (" . str_replace("[[replace]]", "AND pg.pagsituacaopagamento <> '2 - EFETIVADO'", $strPagamento) . ")";
                    } else if ($pagamento == "2") { #Pagamento N�o Solicitado
                        $stWhere .= " AND (" . str_replace("[[replace]]", "", $strPagamento) . ") IS NULL";
                    } else { #Pagamento Efetivado
                        $stWhere .= " AND poc.pronumeroprocesso IN (" . str_replace("[[replace]]", "AND pg.pagsituacaopagamento = '2 - EFETIVADO'", $strPagamento) . ")";
                    }
                }
                
                #Obras migradas pro Obras
                if (!empty($obrasmigradas)) {
                    if ($obrasmigradas == '1') {
                        $stWhere .= " AND EXISTS(SELECT preid FROM obras2.obras where preid = pre.preid AND obrstatus = 'A') = 't'";
                    } else {
                        $stWhere .= " AND EXISTS(SELECT preid FROM obras2.obras where preid = pre.preid AND obrstatus = 'A') = 'f'";
                    }
                }
	
	        //http://simec-local/par/par.php?modulo=principal/subacao&acao=A&sbaid=2823713
	        if ($exporta != 'true') {
	            $acoes = "'<center><img src=../imagens/alterar.gif title=abrir style=cursor:pointer; onclick=\"exibir('||pre.preid||','||s.sbaid||','||sbo.sobano||');\">
						   <img src=../imagens/consultar.gif title=abrir style=cursor:pointer; onclick=\"window.open(\'par.php?modulo=principal/subacao&acao=A&sbaid='||s.sbaid||'&inuid='||iu.inuid||'&anoatual='||sbo.sobano||'\', \'modelo\', \'height=600,width=800,scrollbars=yes,top=0,left=0\' )\"></center>' $acoes_possui_reformulacao  $acoes_possui_prorrogacao_prazo as acao, ";
	        } else {
	            $acoes = "";
	        }
	
	        $arrConsulta = Array(PAR_PERFIL_CONSULTA_ESTADUAL, PAR_PERFIL_CONTROLE_SOCIAL_ESTADUAL, PAR_PERFIL_CONTROLE_SOCIAL_MUNICIPAL, PAR_PERFIL_CONSULTA_MUNICIPAL);
	
	        if (possui_perfil($arrConsulta)) {
	            $innerResp = "
	                INNER JOIN par.usuarioresponsabilidade urs ON ((urs.estuf = iu.estuf AND urs.pflcod IN (" . PAR_PERFIL_CONSULTA_ESTADUAL . "," . PAR_PERFIL_CONTROLE_SOCIAL_ESTADUAL . ") )
	                        OR (urs.muncod = iu.muncod AND urs.pflcod IN (" . PAR_PERFIL_CONSULTA_MUNICIPAL . ",". PAR_PERFIL_CONTROLE_SOCIAL_MUNICIPAL. ") ))
	                        AND rpustatus = 'A'
	                        AND urs.usucpf = '{$_SESSION['usucpf']}'
	                ";
	        }
	        
	        $nomeEscola = "	                    
						'<img width=\'18px\' height=\'18px\' src=../imagens/icones-escola.png style=cursor:pointer; 
						onmouseover=\"SuperTitleAjax(\'par.php?modulo=principal/listaObrasPar&acao=A&tipoAba=listaObras&preid={$preid}&requisicao=carreganomeescola&codigoEscola=' ||  pre.entcodent || '\')\" 
						onmouseout=\"SuperTitleOff( this );\" >' 
	                    AS nome_escola,";
	
	        $excel = "";
	        $proc = "";
	        if ($_POST['exporta'] == "true") {
	            $excel = "pre.preid, obr.obrid, ";
	            $nomeEscola = "";
	        }
	
	        $sql = "
	            SELECT  DISTINCT
	                    $acoes
	                    $excel
	                    pre.preid as preid,
	                    obr.obrid as obrid,
	                    pre.predescricao,
	                    pto.ptodescricao,
	                    mun.mundescricao,
	                    mun.estuf,
	                    s.sbadsc,
	                    esd.esddsc || 
	                    CASE WHEN esd.esdid = 328 
	                        THEN '<label style=\"color: red\"> (Inicio: ' || 
	                            ( SELECT to_char(max(hd.htddata),'DD/MM/YYYY')
	                            FROM workflow.historicodocumento hd 
	                            WHERE docid = pre.docid AND aedid = 872 )
	                             || ' Prazo Final: ' || 
	                            ( 
	                            SELECT
	                                    to_char(CAST(max(hd.htddata) as date) + COALESCE((SELECT dioqtddia FROM ( SELECT dioqtddia, dioid FROM par.diligenciaobra WHERE preid = pre.preid and dioativo = true ORDER BY dioid DESC LIMIT 1 ) as foo), 30 ) ,'DD/MM/YYYY') AS data
	                            FROM workflow.historicodocumento hd
	                            WHERE docid = pre.docid AND aedid = 872 )
	                             || ' )'
	                        ELSE ''
	                    END as estado1,
	                    --esd2.esddsc as estado2,	                    
	                    str.strdsc as estado2,
	                    $nomeEscola
	                    (SELECT
							usunome
						FROM
							workflow.historicodocumento hst 
						INNER JOIN seguranca.usuario usu ON hst.usucpf = usu.usucpf 
						WHERE hst.hstid = doc.hstid)  as usunome,
	                    sbo.sobano,
	                    (select usunome from (select distinct
	                            max(hd1.htddata) as data,
	                            us1.usunome,
	                            hd1.docid,
	                            ed1.esddsc
	                    from workflow.historicodocumento hd1
	                            inner join workflow.acaoestadodoc ac1 on ac1.aedid = hd1.aedid
	                            inner join workflow.estadodocumento ed1 on ed1.esdid = ac1.esdidorigem /*AND ed1.tpdid = " . WF_FLUXO_OBRAS_PAR . "*/ 
	                            inner join seguranca.usuario us1 on us1.usucpf = hd1.usucpf
	                            left join workflow.comentariodocumento cd1 on cd1.hstid = hd1.hstid
	                    where
	                            ac1.esdiddestino in (" . WF_PAR_EM_DILIGENCIA . "," . WF_PAR_VALIDACAO_DILIGENCIA . "," . WF_PAR_VALIDACAO_INDEFERIMENTO . "," . WF_PAR_VALIDACAO_DEFERIMENTO . ")
	                    and
	                            hd1.docid = pre.docid
	                    group by us1.usunome, hd1.docid, ed1.esddsc, hd1.htddata
	                    order by data desc limit 1) as foo) as nomeanalista,
	                    pre.prevalorobra,
	                    to_char(poc.pronumeroprocesso::bigint, 'FM00000\".\"000000\"/\"0000\"-\"00') as processo
	            FROM par.subacaoobra sbo
	            INNER JOIN par.subacao s ON s.sbaid   = sbo.sbaid AND s.sbastatus = 'A'
	            INNER JOIN obras.preobra pre ON pre.preid = sbo.preid AND pre.prestatus = 'A' AND pre.preidpai IS NULL AND pre.tooid <> 1
	            INNER JOIN workflow.documento doc ON doc.docid = pre.docid
	            INNER JOIN workflow.estadodocumento esd ON esd.esdid = doc.esdid
	            LEFT  JOIN obras.pretipoobra pto ON pto.ptoid = pre.ptoid AND pto.ptostatus = 'A' AND pto.tooid <> 1 
	            INNER JOIN par.acao a   ON a.aciid   = s.aciid   AND a.acistatus = 'A'
	            INNER JOIN par.pontuacao p ON p.ptoid   = a.ptoid   AND p.ptostatus = 'A'
	            INNER JOIN par.instrumentounidade iu  ON iu.inuid  = p.inuid
	            INNER JOIN territorios.municipio mun ON mun.muncod = pre.muncod
	            LEFT  JOIN par.processoobrasparcomposicao ppc ON ppc.preid = sbo.preid and ppc.pocstatus = 'A'
                    LEFT  JOIN par.processoobraspar poc ON poc.proid = ppc.proid and poc.prostatus = 'A'
	            LEFT  JOIN obras2.obras obr 
	           		INNER JOIN workflow.documento doc2 ON doc2.docid = obr.docid /*AND doc2.tpdid = 105*/ AND obr.obrstatus = 'A' AND obr.obridpai IS NULL
	                INNER JOIN workflow.estadodocumento esd2 ON esd2.esdid = doc2.esdid
					LEFT JOIN obras2.situacao_registro_documento srd     ON srd.esdid = doc2.esdid
					LEFT JOIN obras2.situacao_registro str               ON str.strid = srd.strid
	            ON obr.obrid = pre.obrid AND obridpai IS NULL AND obr.obrstatus = 'A'
                    
                    
                    
                /*-- LEFT JOIN obras2.situacao_registro str on str.strid = obr.strid and str.strstatus = 'A' -- comentado para a demanda 1914 (retirar e pegar direto do workflow)*/
	            $inner
	            $innerResp
	            WHERE pre.preidpai IS NULL
	            {$stWhere}
	            {$filtroAnalista}
	            ORDER BY mun.estuf, mun.mundescricao, pre.predescricao
	        ";
	           
// 	ver(simec_htmlentities($sql), d);
	        $arrDados = $this->carregar($sql, null, false);
	        $arrDados = $arrDados ? $arrDados : array();
	        return $arrDados;
	    }
	
	    public function recuperarListaObras($post) {
	        if ($post) {
	
	            extract($post);
	            $stWhereWITHTerritorio = '';
	            $stWhereWITHEntidade   = '';
	
	            if ($preid) {
	                $stWhere .= " AND pre.preid = '{$preid}' ";
	                $stWhereWITHTerritorio .= " AND pre.preid = '{$preid}' ";
	                $stWhereWITHEntidade  .= " AND pre.preid = '{$preid}' ";
	            }
	
	            if ($obrid) {
	                $stWhere .= " AND oi.obrid = '{$obrid}' ";
	                $stWhereWITHTerritorio .= " AND pre.obrid = '{$obrid}' ";
	                $stWhereWITHEntidade  .=  " AND pre.obrid = '{$obrid}' ";
	            }
	
	            //Filtro nome da obra.
	            if ($predescricao) {
	                $stWhere .= " AND UPPER(removeacento(pre.predescricao)) ilike '%'||UPPER(removeacento('{$predescricao}'))||'%' ";
	                $stWhereWITHTerritorio .= " AND UPPER(removeacento(pre.predescricao)) ilike '%'||UPPER(removeacento('{$predescricao}'))||'%' ";
	                $stWhereWITHEntidade  .= " AND UPPER(removeacento(pre.predescricao)) ilike '%'||UPPER(removeacento('{$predescricao}'))||'%' ";
	            }
	
	            //Filtro nome do municipio.
	            if (trim($municipio) != '' && $bogeratermo == 'true') {
	                $stWhere .= " AND mun.muncod = '{$municipio}' ";
	                $stWhereWITHTerritorio .=  " AND mun.muncod = '{$municipio}' ";
	            } else {
	                if (trim($municipio) != '') {
	                    $stWhere .= " AND UPPER(removeacento(mun.mundescricao)) ilike '%'||UPPER(removeacento('{$municipio}'))||'%' ";
	                    $stWhereWITHTerritorio .=  " AND UPPER(removeacento(mun.mundescricao)) ilike '%'||UPPER(removeacento('{$municipio}'))||'%' ";
	                }
	            }
	
	            //Filtro Origem da obra.
	            if ($tooid) {
	                $stWhere .= " AND pre.tooid = {$tooid} ";
	                $stWhereWITHTerritorio .=  " AND pre.tooid = {$tooid} ";
	                $stWhereWITHEntidade  .= " AND pre.tooid = {$tooid} ";
	            }
	
	            if ($sisid) {
	                $stWhere .= " AND presistema = {$sisid} ";
	                $stWhereWITHTerritorio .=  " AND pre.presistema = {$sisid} ";
	                $stWhereWITHEntidade  .= " AND pre.presistema = {$sisid} ";
	            }
	
	            //Filtro esfera - Estadual ou municipal
	            if ($ptoesfera[0]) {
	                $stWhere .= " AND pre.preesfera IN ('" . implode("', '", $ptoesfera) . "') ";
	                $stWhereWITHTerritorio .=  " AND pre.preesfera IN ('" . implode("', '", $ptoesfera) . "') ";
	                $stWhereWITHEntidade  .= " AND pre.preesfera IN ('" . implode("', '", $ptoesfera) . "') ";
	            }
	
	            //Filtro estado.
	            if ($estuf) {
	                $stWhere .= " AND mun.estuf iLIKE '%{$estuf}%' ";
	                $stWhereWITHTerritorio .=  " AND mun.estuf iLIKE '%{$estuf}%' ";
	            }
	
	            if (is_array($ptoid)) {
	                if ($ptoid[0] != '') {
	                    $stWhere .= " AND pre.ptoid IN ('" . implode("', '", $ptoid) . "') ";
	                    $stWhereWITHTerritorio .=  " AND pre.ptoid IN ('" . implode("', '", $ptoid) . "') ";
	                    $stWhereWITHEntidade  .= " AND pre.ptoid IN ('" . implode("', '", $ptoid) . "') ";
	                }
	            }
	
	            if ($tpmid != '' && $tpmid) {
	                //$stWhere .= " AND tpm.tpmid IN ('$tpmid') ";
	                $stWhereWITHTerritorio .=  " AND tpm.tpmid IN ('$tpmid') ";
	            }
	
	            if ($esdid) {
	                $stWhere .= " AND esd.esdid = {$esdid} ";
	            }
	
	            if ($preanoselecao) {
	                $stWhere .= " AND pre.preanoselecao = {$preanoselecao} ";
	                $stWhereWITHTerritorio .=  " AND pre.preanoselecao = {$preanoselecao} ";
	                $stWhereWITHEntidade  .= " AND pre.preanoselecao = {$preanoselecao} ";
	            }
	
	            if ($usucpf) {
	                $stWhere .= " AND usu.usucpf = '{$usucpf}' AND doc.esdid = 215";
	            }
	            if ($corrompido == 0) {
	                $stInner = "";
	                $stInner = "";
	            }
	            if ($corrompido == 1) {
	                $stInner .= "INNER JOIN obras.rel_arquivos arqrel ON arqrel.codigoobra = pre.preid";
	            }
	            if ($corrompido == 2) {
	                $stWhere .= "AND pre.preid IN (SELECT DISTINCT a.codigoobra FROM obras.rel_arquivos a LEFT JOIN public.arquivo_recuperado b ON a.arqid = b.arqid WHERE b.arqid IS NULL AND a.codigoobra = pre.preid )";
	            }
	            if ($corrompido == 3) {
	                $stWhere .= "AND pre.preid NOT IN (SELECT DISTINCT a.codigoobra FROM obras.rel_arquivos a LEFT JOIN public.arquivo_recuperado b ON a.arqid = b.arqid WHERE b.arqid IS NULL AND a.codigoobra = pre.preid)
								 AND pre.preid IN (SELECT DISTINCT a.codigoobra FROM obras.rel_arquivos a INNER JOIN public.arquivo_recuperado b ON a.arqid = b.arqid WHERE b.arqvalidacao = FALSE AND a.codigoobra = pre.preid)";
	            }
	            if ($corrompido == 4) {
	                $stInner .= "INNER JOIN obras.rel_arquivos arqrel ON arqrel.codigoobra = pre.preid";
	                $stWhere .= "AND pre.preid NOT IN (SELECT DISTINCT a.codigoobra FROM obras.rel_arquivos a LEFT JOIN public.arquivo_recuperado b ON a.arqid = b.arqid WHERE (b.arqid IS NULL OR b.arqvalidacao = FALSE) AND a.codigoobra = pre.preid)";
	            }
	            if ($corrompido == 5) {
	                $stWhere .= "AND pre.preid NOT IN (SELECT DISTINCT codigoobra FROM obras.rel_arquivos WHERE codigoobra = pre.preid)";
	            }
	            if ($empenho == true) {
	                $stInner .= "INNER JOIN (SELECT DISTINCT preid FROM par.empenhoobra where eobstatus = 'A') emp ON emp.preid = pre.preid";
	                $stWhereWITHTerritorio .=  " pre.preid in (SELECT DISTINCT preid FROM par.empenhoobra where eobstatus = 'A')  ";
	                $stWhereWITHEntidade  .= " pre.preid in (SELECT DISTINCT preid FROM par.empenhoobra where eobstatus = 'A')  ";
	            }
	            if ($reformulacao) {
	                $stWhere .= " AND PRE.PREID IN (SELECT PREIDPAI FROM OBRAS.PREOBRA WHERE PREIDPAI IS NOT NULL AND PREIDPAI = PRE.PREID) ";
	                $stWhereWITHTerritorio .=  " AND PRE.PREID IN (SELECT PREIDPAI FROM OBRAS.PREOBRA WHERE PREIDPAI IS NOT NULL AND PREIDPAI = PRE.PREID) ";
	                $stWhereWITHEntidade  .= " AND PRE.PREID IN (SELECT PREIDPAI FROM OBRAS.PREOBRA WHERE PREIDPAI IS NOT NULL AND PREIDPAI = PRE.PREID) ";
	            }
	            if ($obrasmi == "1") {
	                $stWhere .= " AND PRE.PREID IN (SELECT preid FROM par.adesaoobraspac WHERE aopstatus = 'A' AND preid = PRE.PREID) ";
	            }
	            if ($obrasmi == "2") {
	                $stWhere .= " AND ( (PRE.PREID NOT IN (SELECT preid FROM par.adesaoobraspac WHERE aopstatus = 'A')) OR (PRE.PREID IN (
	                                    SELECT 
	                                        po.preid
	                                    FROM 
	                                        obras.preobra po
	                                    inner join workflow.documento d on d.docid = po.docid AND d.esdid = 228
	                                    inner join par.empenhoobra eo on eo.preid = po.preid and eo.eobstatus = 'A'
	                                    inner join par.empenho emp on emp.empid = eo.empid and empcodigoespecie not in ('03', '13', '02', '04') and empstatus = 'A'
	                                    inner join par.termoobraspaccomposicao tpc on tpc.preid = po.preid
	                                    inner join par.termocompromissopac ter on ter.terid = tpc.terid  AND ter.terstatus = 'A'
	                                    where
	                                        po.ptoid in ( 42,43,44,45 ) AND
	                                        po.prestatus = 'A' AND
	                                        (ter.terdatainclusao+1) < (select htddata from workflow.historicodocumento  where docid = d.docid and aedid in ( select aedid from workflow.acaoestadodoc where esdiddestino = 228 ) order by htddata desc limit 1))))";
	            }
	
	            if ($empenhado == "1") {
	                $strNotQuery = "";
	            }
	            if ($empenhado == "2") {
	                $strNotQuery = "NOT";
	            }
	            if ($empenhado == "1" || $empenhado == "2") {
	                $stWhere .= " and pre.preid {$strNotQuery} in (SELECT DISTINCT preid FROM par.vm_saldo_empenho_por_obra where tipo = 'PAC' and saldo > 0)";
	                $stWhereWITHTerritorio .=   " and pre.preid {$strNotQuery} in (SELECT DISTINCT preid FROM par.vm_saldo_empenho_por_obra where tipo = 'PAC' and saldo > 0)";
	                $stWhereWITHEntidade  .=  " and pre.preid {$strNotQuery} in (SELECT DISTINCT preid FROM par.vm_saldo_empenho_por_obra where tipo = 'PAC' and saldo > 0)";
	            }
	
	            if ($pendenciaObras == '1') { // com pendencias
	                $stWhere .= " AND pre.preesfera = 'M' ";
	                $stWhere .= " AND pre.muncod in (select coalesce(muncod, '') from obras2.vm_pendencia_obras where empesfera = 'M') ";
	                
	                $stWhereWITHTerritorio .=   " AND pre.preesfera = 'M' AND pre.muncod in (select coalesce(muncod, '') from obras2.vm_pendencia_obras where empesfera = 'M') ";
	                $stWhereWITHEntidade  .=  "  AND pre.preesfera = 'M' AND pre.muncod in (select coalesce(muncod, '') from obras2.vm_pendencia_obras where empesfera = 'M') ";
	                
	            } elseif ($pendenciaObras == '2') { // sem pendencias
	                $stWhere .= " AND pre.preesfera = 'M' ";
	                $stWhere .= " AND pre.muncod not in (select coalesce(muncod, '') from obras2.vm_pendencia_obras where empesfera = 'M') ";
	                
	                $stWhereWITHTerritorio .=   " AND pre.preesfera = 'M' AND pre.muncod not in (select coalesce(muncod, '') from obras2.vm_pendencia_obras where empesfera = 'M') ";
	                $stWhereWITHEntidade  .=  "  AND pre.preesfera = 'M' AND pre.muncod not in (select coalesce(muncod, '') from obras2.vm_pendencia_obras where empesfera = 'M') ";
	            }
	
	            if ($pendenciaObrasUF == '1') { // com pendencias
	                $stWhere .= " AND pre.preesfera = 'E' ";
	                $stWhere .= " AND pre.estuf in (select coalesce(estuf, '') from obras2.vm_pendencia_obras where empesfera = 'E') ";
	                
	                $stWhereWITHTerritorio .=   " AND pre.preesfera = 'E' AND pre.estuf in (select coalesce(estuf, '') from obras2.vm_pendencia_obras where empesfera = 'E')  ";
	                $stWhereWITHEntidade  .=  "   AND pre.preesfera = 'E' AND pre.estuf in (select coalesce(estuf, '') from obras2.vm_pendencia_obras where empesfera = 'E')  ";
	           
	            } elseif ($pendenciaObrasUF == '2') { // sem pendencias
	                $stWhere .= " AND pre.preesfera = 'E' ";
	                $stWhere .= " AND pre.estuf not in (select coalesce(estuf, '') from obras2.vm_pendencia_obras where empesfera = 'E') ";
	                
	                $stWhereWITHTerritorio .=   " AND pre.preesfera = 'E' AND pre.estuf in (select coalesce(estuf, '') from obras2.vm_pendencia_obras where empesfera = 'E')  ";
	                $stWhereWITHEntidade  .=  "   AND pre.preesfera = 'E' AND pre.estuf in (select coalesce(estuf, '') from obras2.vm_pendencia_obras where empesfera = 'E')  ";
	           
	                
	            }
	
	            if ($resid) {
	                $stWhere .= "  AND pre.resid  IN ( " . $resid . "  ) ";
	            }
	
	            if ($poausucpfinclusao) {
	
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
	
	                foreach ($arDocid as $dados) {
	                    $arDocids[] = $dados['docid'];
	                }
	
	                if (is_array($arDocids)) {
	                    $filtroAnalista .= " AND doc.docid IN (" . implode(', ', $arDocids) . ")";
	                } else {
	                    $filtroAnalista .= " AND doc.docid IN (0)";
	                }
	            }
	        }
                
                #Termo Gerado
                if (!empty($termogerado)) {
                    if ($termogerado == '1') {
                        $stWhere .=" AND (SELECT COUNT(terid) FROM par.termocompromissopac WHERE proid = po.proid AND terstatus = 'A') > 0";
                    }else{
                        $stWhere .=" AND (SELECT COUNT(terid) FROM par.termocompromissopac WHERE proid = po.proid AND terstatus = 'A') = 0";
                    }
                }
                
                #Pagamento
                if($pagamento == "1") {
                    $stWhere .= " AND po.pronumeroprocesso IN ( SELECT em.empnumeroprocesso FROM par.empenho em INNER JOIN par.pagamento pg ON em.empid=pg.empid and pagsituacaopagamento not ilike '%CANCELADO%' and empstatus = 'A' AND pg.pagstatus = 'A' )";
                }
                if($pagamento == "2") {
                    $stWhere .= " AND po.pronumeroprocesso NOT IN ( SELECT em.empnumeroprocesso FROM par.empenho em INNER JOIN par.pagamento pg ON em.empid=pg.empid and pagsituacaopagamento not ilike '%CANCELADO%' and empstatus = 'A' AND pg.pagstatus = 'A' )";
                }
                if($pagamento == "3") {
                    $stWhere .= " AND po.pronumeroprocesso IN ( SELECT em.empnumeroprocesso FROM par.empenho em INNER JOIN par.pagamento pg ON em.empid=pg.empid and pagsituacaopagamento ilike '%2 - EFETIVADO%' and empstatus = 'A' AND pg.pagstatus = 'A' )";
                }
                
                #Obras migradas pro Obras
                if (!empty($obrasmigradas)) {
                    if ($obrasmigradas == '1') {
                        $stWhere .= " AND EXISTS(SELECT preid FROM obras2.obras where preid = pre.preid AND obrstatus = 'A') = 't'";
                    } else {
                        $stWhere .= " AND EXISTS(SELECT preid FROM obras2.obras where preid = pre.preid AND obrstatus = 'A') = 'f'";
                    }
                }
	
                $acoes_possui_prorrogacao_prazo = "|| CASE WHEN (SELECT popstatus FROM obras.preobraprorrogacao WHERE preid = pre.preid order by popid desc limit 1) = 'F' OR (SELECT popstatus FROM obras.preobraprorrogacao WHERE preid = pre.preid order by popid desc limit 1) = 'A' THEN '<img width=\"16px\" title=\"Cancelar Prorroga��o de Prazo\" border=\"0\" src=\"../imagens/cancelar_prorrogacao_prazo.png\" id=\"\" onclick=\"cancelarProrrogacaoPrazo(' || pre.preid || ', \''||(SELECT popstatus FROM obras.preobraprorrogacao WHERE preid = pre.preid order by popid desc limit 1)||'\');\" style=\"cursor:pointer\" />' ELSE '' END";
                
                $arrCpfPermissao = array('', '', '', '', '', '');
                $strBtnUnBlockRef = "";
                if (in_array($_SESSION['usucpf'], $arrCpfPermissao)) {
                    $strBtnUnBlockRef = "|| CASE WHEN pre.preblockreformulacao = 'S' THEN '<img title=\'Desbloquear Reformulacao de Obra\' preid=\''||pre.preid||'\' class=\'btnblockreformulacao\' width=\'16px\' height=\'16px\' src=../imagens/par/cadeado-aberto.png style=cursor:pointer; />' ELSE '' END ";
                }
                
	        if ($bogeratermo == 'true') {
	            $acoes = "'<center><input type=\"checkbox\" id=\"preid[]\" name=\"preid[]\" value=\"' || pre.preid || '\" /></center>'";
	            $stWhere .= 'AND esd.esdid = 228';
	        } else {
	
	            $acoes_possui_reformulacao = "|| CASE WHEN (SELECT COUNT(po.preid) FROM obras.preobra po WHERE po.preidpai = pre.preid AND prestatus = 'A' AND preid NOT IN (SELECT preid FROM obras.preobracancelamentoreformulacao)) > 0 THEN ' <img src=../imagens/mais.gif title=abrir style=cursor:pointer; onclick=\"exibirReformulacao('||pre.preid||', this);\">' ELSE '' END";
	
	            if ($this->testa_superuser() || in_array(PAR_PERFIL_COORDENADOR_TECNICO, pegaPerfilGeral()) || in_array(PAR_PERFIL_COORDENADOR_GERAL, pegaPerfilGeral())) {
	                $acoes_reformulacao = "|| CASE WHEN (esd.esdid=" . WF_TIPO_OBRA_APROVADA . ") THEN '<img src=../imagens/restricao_ico.png style=cursor:pointer; onclick=\"confirmarReformulacao('||pre.preid||');\">' ELSE '' END";
	            }
	        }
	        if ($exporta != 'true') {
	            $acoes = "'<img border=\"0\" src=\"../imagens/alterar.gif\" id=\"\" onclick=\"mostrarDadosObras( ' || pre.preid || ', ' || coalesce(mun.muncod,'null') || ', ' || coalesce(doc.esdid,0) || ' );\" style=\"cursor:pointer\" /> '" . $strBtnUnBlockRef . $acoes_reformulacao . $acoes_possui_reformulacao . $acoes_possui_prorrogacao_prazo . " as acao, ";
	        } else {
	            $acoes = "";
	        }
	//		print_r($acoes);
	        if ($premcmv == '1') {
	            $stWhere .= 'AND pre.premcmv = TRUE';
	        } else {
	            //$stWhere .= 'AND pre.premcmv = FALSE';  //Comentei pq se n�o traz tanto o verdadeiro como o falso.
	        }
	
	        $arrConsulta = Array(PAR_PERFIL_CONSULTA_ESTADUAL, PAR_PERFIL_CONTROLE_SOCIAL_ESTADUAL, PAR_PERFIL_CONSULTA_MUNICIPAL, PAR_PERFIL_CONTROLE_SOCIAL_MUNICIPAL);
	
	        if (possui_perfil($arrConsulta)) {
	            $innerResp = "INNER JOIN par.usuarioresponsabilidade urs ON ((urs.estuf = mun.estuf AND urs.pflcod IN (" . PAR_PERFIL_CONSULTA_ESTADUAL . ", ".PAR_PERFIL_CONTROLE_SOCIAL_ESTADUAL.") )
																			OR (urs.muncod = mun.muncod AND urs.pflcod IN (" . PAR_PERFIL_CONSULTA_MUNICIPAL . ",".PAR_PERFIL_CONTROLE_SOCIAL_MUNICIPAL.") ))
																			AND urs.rpustatus = 'A'
																			AND urs.usucpf = '{$_SESSION['usucpf']}'";
	        }
	        
	        $nomeEscola = "
	                    '<img width=\'18px\' height=\'18px\' src=../imagens/icones-escola.png style=cursor:pointer; 
                                onmouseover=\"SuperTitleAjax(\'par.php?modulo=principal/listaObras&acao=A&tipoAba=listaObras&preid={$preid}&requisicao=carreganomeescola&codigoEscola=' ||  pre.entcodent ||'\')\" 
								onmouseout=\"SuperTitleOff( this );\" >' 
	                    AS nome_escola,";
	
	        $excel = "";
	        if ($_POST['exporta'] == "true") {
	            $excel = "pre.preid, oi.obrid, ";
	            $nomeEscola = "";
	        }
                
	        $sql = " 
	        	WITH par_entidade_entidade AS (
				SELECT e.entcodent, e.entnome FROM entidade.entidade e 
				INNER JOIN obras.preobra pre ON pre.entcodent = e.entcodent
				WHERE entstatus = 'A' AND pre.prestatus = 'A' AND pre.entcodent IS NOT NULL AND e.entcodent != ''
						{$stWhereWITHEntidade}
				),
				par_municipio AS (
				SELECT DISTINCT mun.muncod, mun.mundescricao, mun.estuf
				FROM territorios.municipio mun
				INNER JOIN territorios.muntipomunicipio mtpm ON mtpm.muncod = mun.muncod
				INNER JOIN territorios.tipomunicipio tpm ON tpm.tpmid = mtpm.tpmid AND tpmstatus = 'A' AND gtmid = 7
				INNER JOIN obras.preobra pre ON pre.muncod  = mun.muncod AND pre.prestatus = 'A'
				WHERE 1 = 1 {$stWhereWITHTerritorio}
				)	    
	            SELECT  DISTINCT
	                    $acoes 
	                    $excel 
	                    pre.predescricao,
	                    pre.preid as preid,
	    				oi.obrid as obrid,
	                    pto.ptodescricao,
	                    mun.mundescricao,
	                    pre.estuf,
	                    esd.esddsc || 
	                    CASE WHEN esd.esdid = 195 THEN
	                                    '<label style=\"color: red\"> (Inicio: ' || 
	                                    ( SELECT to_char(max(hd.htddata),'DD/MM/YYYY')
	                                    FROM workflow.historicodocumento hd 
	                                    WHERE docid = pre.docid AND aedid IN (2022,516,617,1815) )
	                                     || ' Prazo Final: ' || 
	                                    ( 
	                                    SELECT
	                                            to_char(CAST(max(hd.htddata) as date) + COALESCE((SELECT dioqtddia FROM ( SELECT dioqtddia, dioid FROM par.diligenciaobra WHERE preid = pre.preid and dioativo = true ORDER BY dioid DESC LIMIT 1 ) as foo ), 30) ,'DD/MM/YYYY') AS data
	                                    FROM workflow.historicodocumento hd
	                                    WHERE
	                                            docid = pre.docid
	                                            AND aedid IN (2022,516,617,1815) )
	                                     || ' )'
	                            ELSE ''
	                    END as estado1,
	                    str.strdsc as estado2,
	                    $nomeEscola
	                    usu.usunome,
	                    to_char(hst.htddata,'DD/MM/YYYY HH24:MI:SS') as htddata,
	                    (select usunome from (select distinct
	                                            max(hd1.htddata) as data,
	                                            us1.usunome,
	                                            hd1.docid,
	                                            ed1.esddsc
	                                    from workflow.historicodocumento hd1
	                                            inner join workflow.acaoestadodoc ac1 on
	                                                    ac1.aedid = hd1.aedid
	                                            inner join workflow.estadodocumento ed1 on
	                                                    ed1.esdid = ac1.esdidorigem AND ed1.tpdid = " . WF_FLUXO_PRO_INFANCIA . "
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
	                    resnumero,
	                    ROUND(pre.prevalorobra,2) as prevalorobra,
	                    to_char(po.pronumeroprocesso::bigint, 'FM00000\".\"000000\"/\"0000\"-\"00') as processo
	                    
	            FROM obras.preobra pre
	            -- LEFT JOIN entidade.entidade et ON et.entcodent = pre.entcodent
	            LEFT JOIN par_entidade_entidade et ON et.entcodent = pre.entcodent
	            INNER JOIN workflow.documento doc ON doc.docid   = pre.docid {$filtroAnalista} 
	            INNER JOIN workflow.estadodocumento esd ON esd.esdid = doc.esdid AND esd.tpdid = " . WF_FLUXO_PRO_INFANCIA . "
	            INNER JOIN workflow.historicodocumento hst on doc.hstid=hst.hstid
	            LEFT  JOIN seguranca.usuario usu ON hst.usucpf = usu.usucpf
	            LEFT  JOIN obras.pretipoobra pto ON pre.ptoid = pto.ptoid
	            LEFT JOIN  par_municipio mun ON pre.muncod  = mun.muncod
	            /*
	            LEFT  JOIN territorios.municipio mun
	            	INNER JOIN territorios.muntipomunicipio mtpm
	            		INNER JOIN territorios.tipomunicipio tpm ON tpm.tpmid = mtpm.tpmid AND tpmstatus = 'A' AND gtmid = 7
					ON mtpm.muncod = mun.muncod
	            ON pre.muncod  = mun.muncod
	            */
	            LEFT JOIN par.resolucao res ON res.resid = pre.resid
	            
	            {$stInner}
	            $innerResp
	            " . ($poausucpfinclusao ? " INNER " : " LEFT " ) . " 
	                
	                JOIN (
	                    SELECT  poap.preid,
	                            poap.poausucpfinclusao,
	                            usup.usunome as nomeanalista
	                    FROM obras.preobraanalise poap
	                    LEFT JOIN seguranca.usuario usup ON usup.usucpf = poap.poausucpfinclusao
	                ) poa ON poa.preid = pre.preid
	                
			LEFT  JOIN obras2.obras oi
	                
	                INNER JOIN workflow.documento doc2 ON doc2.docid = oi.docid AND doc2.tpdid = 105 AND oi.obrstatus = 'A' AND oi.obridpai IS NULL
	                INNER JOIN workflow.estadodocumento esd2 ON esd2.esdid = doc2.esdid 
	                LEFT JOIN obras2.situacao_registro_documento srd     ON srd.esdid = doc2.esdid
					LEFT JOIN obras2.situacao_registro str               ON str.strid = srd.strid
	            ON oi.preid = pre.preid AND obridpai IS NULL AND oi.obrstatus = 'A'
	       /*--left join obras2.situacao_registro str on str.strid = oi.strid and str.strstatus = 'A'  -- comentado para a demanda 1914 (retirar e pegar direto do workflow)*/
	                
			LEFT JOIN par.processoobraspaccomposicao popc on popc.preid = pre.preid and popc.pocstatus = 'A'
			LEFT JOIN par.processoobra po on po.proid = popc.proid and po.prostatus = 'A'
	                
			WHERE pre.prestatus = 'A' AND pre.preidpai IS NULL 
		 	{$stWhere}
	        ";
// 		 	ver(simec_htmlentities($sql),d);
	        return $this->carregar($sql, null, false);
	    }
	
	    public function recuperarListaObrasEmpenho($post) {
	    	
	        if ($post) {
	
	            extract($post);
	
	            if ($muncod) {
	                $stWhere .= " AND mun.muncod = '{$municipio}' ";
	            }
	
	            if ($estuf) {
	                $stWhere .= " AND mun.estuf iLIKE '%{$estuf}%' ";
	            }
	
	            if ($ptoid) {
	                $stWhere .= " AND pre.ptoid IN ('" . implode("', '", $ptoid) . "') ";
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
	
	        $sql = "SELECT muncod FROM territorios.muntipomunicipio  WHERE tpmid = " . TIPOMUN_GRUPO1 . " AND muncod='" . $muncod . "'";
	        $muncodGrupo = $this->pegaUm($sql);
	        if ($muncodGrupo)
	            return true;
	        else
	            return false;
	    }
	
	    public function verificaPrazoExpiraAdesao($muncod) {
	        $sql = "SELECT
						count(*)
					FROM obras.preobra pre
					INNER JOIN workflow.documento wkf ON wkf.docid = pre.docid
					WHERE muncod = '{$muncod}'
					AND wkf.esdid IN (194, 195, 210, 211, 212, 213, 214, 215, 217, 218)
					AND pre.prestatus = 'A'";
	
	        return $this->pegaUm($sql);
	    }
	
	    public function verificaQtdObrasEstado($dados) {
	        $sql = "SELECT
						pttquantidadelimite-count(preid) as qtd
					FROM
						obras.preobra pre
					INNER JOIN obras.pretetoobra ptt ON ptt.estuf = pre.estufpar
					WHERE
						estufpar = '" . $dados['estuf'] . "'
						AND muncodpar IS NULL
						AND pttobra = '" . $dados['tipo'] . "'
						AND pre.prestatus = 'A'
					GROUP BY
						pttquantidadelimite";
	        $qtd = $this->pegaUm($sql);
	        $qtd = $qtd ? $qtd : 1;
	
	        return $qtd > 0 ? true : false;
	    }
	
	    function validaPermissaoFinalizarQuestionario($qrpid) {
	        if (!$qrpid) {
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
	
	        $arDados = (array) $this->carregar($sql);
	
	        $return = false;
	        if (count($arDados) < 13) {
	            foreach ($arDados as $dados) {
	                if (substr($dados['pertitulo'], 0, 3) == '1.2' && strpos($dados['itptitulo'], "N�o") > -1) {
	                    $return = true;
	                }
	            }
	        } else {
	            $return = true;
	        }
	
	        return $return;
	    }
	
	    public function pegaMuncodPorPreid($preid) {
	        $sql = "SELECT
						mun.muncod,
						mun.mundescricao
					FROM obras.preobra pre
					INNER JOIN territorios.municipio mun ON mun.muncod = pre.muncod
					WHERE preid = {$preid}";
	
	        return $this->pegaLinha($sql);
	    }
	
	    public function verificaArquivoCorrompido($preid, $arqid = 0) {
	        $where = "";
	        if ($arqid != 0) {
	            $where = "AND arqid = " . $arqid;
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
	
	    public function verificaArquivoValidado($preid, $arqid = 0) {
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
	        return true;
	    }
	
	    public function verificaAlertaArquivoCorrompido($preid) {
	        $texto = true;
	        $sql = "SELECT DISTINCT
						TRUE as teste
					FROM obras.rel_arquivos arqrel
					LEFT JOIN public.arquivo_recuperado arqrec ON arqrec.arqid = arqrel.arqid
					WHERE
						arqrec.arqid IS NULL
						AND codigoobra = {$preid}";
	        return $texto;
	    }
	
	    public function verificaAlertaArquivoSubstituido($preid) {
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
	//			return "<div style=\"text-align: center; font-size: 30px; color: BLUE; border: 4px solid BLUE; background-color: rgb(200,225,157); width: 100% \">Todos os arquivos corrompidos j� foram substitu�dos e aguardam valida��o.</div>";
	//		}
	    }
	
	    function salvarPreObraSubacao($preid, $sobano, $sbaid) {
	        // Essa linha foi comentada pois o preid j� est� diretamente relacionado ao sobano e ao sbaid
	        $sql = "select sobid from par.subacaoobra where preid = $preid --and sobano = '$sobano' and sbaid = '$sbaid'";
	        $sobid = $this->pegaUm($sql);
	        if (!$sobid) {
	            $sql = "insert into par.subacaoobra (sbaid, preid, sobano) values ('$sbaid','$preid','$sobano') returning sobid";
	            $sobid = $this->pegaUm($sql);
	            $this->commit();
	            return $sobid;
	        } else {
	            return $sobid;
	        }
	    }
	
	    function montaLista($sbaid, $sobano = null) {
	
	        $arrWhere = Array("prestatus = 'A'");
	
	        /* Os perfis de Equipe Municipal, EM Aprova��o, Prefeito, Equipe Estadual e Estadual Aprova��o n�o devem ver obras Arquivadas
	         * */
	        $perfil = pegaArrayPerfil($_SESSION['usucpf']);
	
	        $arrNotArquivados = Array(PAR_PERFIL_EQUIPE_MUNICIPAL,
	            PAR_PERFIL_EQUIPE_MUNICIPAL_APROVACAO,
	            PAR_PERFIL_PREFEITO,
	            PAR_PERFIL_EQUIPE_ESTADUAL,
	            PAR_PERFIL_EQUIPE_ESTADUAL_APROVACAO);
	
	        $arrComparacao = array_intersect($perfil, $arrNotArquivados);
	        if ($arrComparacao[0] != '') {
	            $arrWhere[] = "esd.esdid NOT IN (" . WF_PAR_OBRA_ARQUIVADA . ")";
	        }
	        /* FIM - Os perfis de Equipe Municipal, EM Aprova��o, Prefeito, Equipe Estadual e Estadual Aprova��o n�o devem ver obras Arquivadas
	         * */

	        if ($sobano) {
	            $perfil = pegaPerfilGeral();
	            $cabecalho = array("A��o", "Obra", "Tipo", "Status", 'Valor Empenhado', 'Valor Pagamento');
	            if ($sobano >= date('Y') &&
	                    !(
	                    in_array(PAR_PERFIL_CONSULTA, $perfil) ||
	                    in_array(PAR_PERFIL_CONSULTA_ESTADUAL, $perfil) ||
                        in_array(PAR_PERFIL_CONTROLE_SOCIAL_ESTADUAL, $perfil) ||
	                    in_array(PAR_PERFIL_CONSULTA_MUNICIPAL, $perfil) ||
                        in_array(PAR_PERFIL_CONTROLE_SOCIAL_MUNICIPAL, $perfil) ||
	                    in_array(PAR_PERFIL_GERADOR_DOCUMENTO, $perfil) ||
	                    in_array(PAR_PERFIL_PAGADOR, $perfil) ||
	                    in_array(PAR_PERFIL_ENGENHEIRO_FNDE, $perfil) ||
	                    in_array(PAR_PERFIL_ANALISTA_QUESTOES_PONTUAIS, $perfil) ||
	                    in_array(PAR_PERFIL_EMPENHADOR, $perfil) ||
	                    in_array(PAR_PERFIL_ALTA_GESTAO_MEC, $perfil)
	                    )
	            ) {
	                $ano = "and sbo.sobano = '$sobano'";
	                $acao = "'<center><img class=\"middle link\" title=\"Consultar Obra\" src=\"../imagens/consultar.gif\" onclick=\"consultarObra(\'' || pre.preid || '\',\'' || sbo.sbaid || '\',\'' || sbo.sobano || '\')\"  />
							' || CASE
									WHEN esd.esdid = " . WF_PAR_EM_CADASTRAMENTO . "
										THEN
											CASE WHEN ( SELECT DISTINCT preid FROM par.empenhoobra eo INNER JOIN par.empenho e ON e.empid = eo.empid WHERE e.empstatus = 'A' AND preid = pre.preid UNION ALL
														SELECT DISTINCT preid FROM par.empenhoobrapar eop INNER JOIN par.empenho e2 ON e2.empid = eop.empid WHERE e2.empstatus = 'A' AND preid = pre.preid  ) = pre.preid THEN '' ELSE 
													'<img class=\"middle link\" title=\"Excluir Obra\" src=\"../imagens/excluir.gif\" onclick=\"excluirObra(this,\'' || pre.preid || '\')\"  /> ' END 
										ELSE ''
									END
							|| ' </center>' as acao,";
	            } else {
	                $ano = "and sbo.sobano = '$sobano'";
	                $acao = "'<center><img class=\"middle link\" title=\"Consultar Obra\" src=\"../imagens/consultar.gif\" onclick=\"consultarObra(\'' || pre.preid || '\',\'' || sbo.sbaid || '\',\'' || sbo.sobano || '\')\"  /></center>' as acao,";
	            }
	        } else {
	            $cabecalho = array("Obra", "Tipo", "Status", 'Valor Empenhado', 'Valor Pagamento');
	        }
	
	        if (!$sbaid) {
	            $sbaid = $_SESSION['par']['obras']['sbaid'];
	        }
	
	        $sql = "SELECT
						{$acao}
						pre.predescricao,
						pto.ptodescricao,
						esd.esddsc,
						coalesce((select sum(saldo) from par.vm_saldo_empenho_por_obra where preid = pre.preid), '0.00') as valorempenho,
	                    coalesce((SELECT sum(po.popvalorpagamento)
			                      FROM par.pagamentoobrapar po
			                      INNER JOIN par.pagamento p on p.pagid = po.pagid
			                      WHERE po.preid = pre.preid AND p.pagstatus = 'A'
			                      		AND p.pagsituacaopagamento != 'CANCELADO'), '0.00') as valorpagamento
					FROM obras.preobra pre
					INNER JOIN par.subacaoobra 	sbo ON sbo.preid = pre.preid {$ano} and sbo.sbaid = '$sbaid'
					LEFT JOIN territorios.municipio mun ON mun.muncod = pre.muncod
					LEFT JOIN obras.pretipoobra pto ON pto.ptoid = pre.ptoid
					LEFT JOIN workflow.documento doc ON doc.docid = pre.docid
					LEFT JOIN workflow.estadodocumento esd ON esd.esdid = doc.esdid
					WHERE " . implode(' AND ', $arrWhere);
	        return $this->monta_lista($sql, $cabecalho, 2000, 10, "N", "100%", "N");
	    }
	
	    function montaListaSbaEmenda($sbaid, $sobano = null) {
	
	        $arrWhere = Array("prestatus = 'A'");
	
	        /* Os perfis de Equipe Municipal, EM Aprova��o, Prefeito, Equipe Estadual e Estadual Aprova��o n�o devem ver obras Arquivadas
	         * */
	        $perfil = pegaArrayPerfil($_SESSION['usucpf']);
	
	        $arrNotArquivados = Array(PAR_PERFIL_EQUIPE_MUNICIPAL,
	            PAR_PERFIL_EQUIPE_MUNICIPAL_APROVACAO,
	            PAR_PERFIL_PREFEITO,
	            PAR_PERFIL_EQUIPE_ESTADUAL,
	            PAR_PERFIL_EQUIPE_ESTADUAL_APROVACAO);
	
	        $arrComparacao = array_intersect($perfil, $arrNotArquivados);
	        if ($arrComparacao[0] != '') {
	            $arrWhere[] = "esd.esdid NOT IN (" . WF_PAR_OBRA_ARQUIVADA . ")";
	        }
	        /* FIM - Os perfis de Equipe Municipal, EM Aprova��o, Prefeito, Equipe Estadual e Estadual Aprova��o n�o devem ver obras Arquivadas
	         * */
	
	        if ($sobano) {
	            $perfil = pegaPerfilGeral();
	            $cabecalho = array("A��o", "Obra", "Tipo", "Status", 'Valor Empenhado', 'Valor Pagamento', 'Valor Planejado Emenda', 'Valor Aprovado Emenda');
	            if ($sobano >= date('Y') &&
	                    !(
	                    in_array(PAR_PERFIL_CONSULTA, $perfil) ||
	                    in_array(PAR_PERFIL_CONSULTA_ESTADUAL, $perfil) ||
                        in_array(PAR_PERFIL_CONTROLE_SOCIAL_ESTADUAL, $perfil) ||
	                    in_array(PAR_PERFIL_CONSULTA_MUNICIPAL, $perfil) ||
                        in_array(PAR_PERFIL_CONTROLE_SOCIAL_MUNICIPAL, $perfil) ||
	                    in_array(PAR_PERFIL_GERADOR_DOCUMENTO, $perfil) ||
	                    in_array(PAR_PERFIL_PAGADOR, $perfil) ||
	                    in_array(PAR_PERFIL_ENGENHEIRO_FNDE, $perfil) ||
	                    in_array(PAR_PERFIL_ANALISTA_QUESTOES_PONTUAIS, $perfil) ||
	                    in_array(PAR_PERFIL_EMPENHADOR, $perfil) ||
	                    in_array(PAR_PERFIL_ALTA_GESTAO_MEC, $perfil)
	                    )
	            ) {
	                $ano = "and sbo.sobano = '$sobano'";
	                $acao = "'<center><img class=\"middle link\" title=\"Consultar Obra\" src=\"../imagens/consultar.gif\" onclick=\"consultarObra(\'' || pre.preid || '\',\'' || sbo.sbaid || '\',\'' || sbo.sobano || '\')\"  />
							' || CASE
									WHEN esd.esdid = " . WF_PAR_EM_CADASTRAMENTO . "
										THEN '<img class=\"middle link\" title=\"Excluir Obra\" src=\"../imagens/excluir.gif\" onclick=\"excluirObra(this,\'' || pre.preid || '\')\"  /> '
										ELSE ''
									END
							|| ' </center>' as acao,";
	            } else {
	                $ano = "and sbo.sobano = '$sobano'";
	                $acao = "'<center><img class=\"middle link\" title=\"Consultar Obra\" src=\"../imagens/consultar.gif\" onclick=\"consultarObra(\'' || pre.preid || '\',\'' || sbo.sbaid || '\',\'' || sbo.sobano || '\')\"  /></center>' as acao,";
	            }
	        } else {
	            $cabecalho = array("Obra", "Tipo", "Status", 'Valor Empenhado', 'Valor Pagamento', 'Valor Planejado Emenda', 'Valor Aprovado Emenda');
	        }
	
	        if (!$sbaid) {
	            $sbaid = $_SESSION['par']['obras']['sbaid'];
	        }
	
	        $sql = "SELECT
						{$acao}
						pre.predescricao,
						pto.ptodescricao,
						esd.esddsc,
						coalesce((select sum(eo.eobvalorempenho)
	                      from par.empenhoobrapar eo
	                          inner join par.empenho e on e.empid = eo.empid and empstatus = 'A' and eobstatus = 'A'
	                      where eo.preid = pre.preid
	                          and e.empsituacao != 'CANCELADO'), '0.00') as valorempenho,
	                    coalesce((select sum(po.popvalorpagamento)
	                      from par.pagamentoobrapar po
	                          inner join par.pagamento p on p.pagid = po.pagid and pagstatus = 'A'
	                      where po.preid = pre.preid
	                      	and p.pagsituacaopagamento != 'CANCELADO'), '0.00') as valorpagamento,
	                      (SELECT sum(seovlremenda) as valor_planejado FROM par.subacaoemendasdistribuicaoobras where preid = pre.preid)  as valor_planejado,
						  (SELECT sum(seovlremendaaprovado) as valor_aprovado FROM par.subacaoemendasdistribuicaoobras where preid = pre.preid) as valor_aprovado 
					FROM obras.preobra pre
					INNER JOIN par.subacaoobra 	sbo ON sbo.preid = pre.preid {$ano} and sbo.sbaid = '$sbaid'
					LEFT JOIN territorios.municipio mun ON mun.muncod = pre.muncod
					LEFT JOIN obras.pretipoobra pto ON pto.ptoid = pre.ptoid
					LEFT JOIN workflow.documento doc ON doc.docid = pre.docid
					LEFT JOIN workflow.estadodocumento esd ON esd.esdid = doc.esdid
					WHERE " . implode(' AND ', $arrWhere);
	
	        return $this->monta_lista($sql, $cabecalho, 2000, 10, "N", "100%", "N");
	    }
	
	    function montaListaVinculadas($sbaid, $sovano = null) {
	
	        if ($sovano) {
	            $perfil = pegaPerfilGeral();
	            $cabecalho = array("A��o", "Obra", "Tipo");
	            if ($sovano >= date('Y') &&
	                    !(
	                    in_array(PAR_PERFIL_CONSULTA, $perfil) ||
	                    in_array(PAR_PERFIL_CONSULTA_ESTADUAL, $perfil) ||
                        in_array(PAR_PERFIL_CONTROLE_SOCIAL_ESTADUAL, $perfil) ||
	                    in_array(PAR_PERFIL_CONSULTA_MUNICIPAL, $perfil) ||
                        in_array(PAR_PERFIL_CONTROLE_SOCIAL_MUNICIPAL, $perfil) ||
	                    in_array(PAR_PERFIL_GERADOR_DOCUMENTO, $perfil) ||
	                    in_array(PAR_PERFIL_PAGADOR, $perfil) ||
	                    in_array(PAR_PERFIL_ENGENHEIRO_FNDE, $perfil) ||
	                    in_array(PAR_PERFIL_ANALISTA_QUESTOES_PONTUAIS, $perfil) ||
	                    in_array(PAR_PERFIL_EMPENHADOR, $perfil) ||
	                    in_array(PAR_PERFIL_ALTA_GESTAO_MEC, $perfil)
	                    )
	            ) {
	                $ano = "and sov.sovano = '$sovano'";
	                $acao = "CASE WHEN obr.preid IS NOT NULL THEN '<center><img class=\"middle link\" title=\"Consultar Obra\" src=\"../imagens/consultar.gif\" onclick=\"consultarObra(\'' || pre.preid || '\',\'' || sov.sbaid || '\',\'' || sov.sovano || '\')\"  /></center>' ELSE '<center><img class=\"middle link\" title=\"Obra cadastrada fora do PAR\" src=\"../imagens/ico_ajuda.gif\"  /></center>' END as acao,";
	            } else {
	                $ano = "and sov.sovano = '$sovano'";
	                $acao = "CASE WHEN obr.preid IS NOT NULL THEN '<center><img class=\"middle link\" title=\"Consultar Obra\" src=\"../imagens/consultar.gif\" onclick=\"consultarObra(\'' || pre.preid || '\',\'' || sov.sbaid || '\',\'' || sov.sovano || '\')\"  /></center>' ELSE '<center><img class=\"middle link\" title=\"Obra cadastrada fora do PAR\" src=\"../imagens/ico_ajuda.gif\"  /></center>' END as acao,";
	            }
	        } else {
	            $cabecalho = array("Obra", "Tipo", "Status");
	        }
	
	        if (!$sbaid) {
	            $sbaid = $_SESSION['par']['obras']['sbaid'];
	        }
	
	//		$sql = "SELECT
	//					{$acao}
	//					obr.obrdesc,
	//					toba.tobadesc
	//				FROM obr/*as.obra*/infraestrutura obr
	//				INNER JOIN par.subacaoobravinculacao sov ON sov.obrid = obr.obrid {$ano} and sov.sbaid = '$sbaid'
	//				INNER JOIN entidade.endereco ende ON ende.endid = obr.endid
	//				LEFT JOIN obras.preobra pre ON pre.preid = sov.preid
	//				LEFT JOIN territorios.municipio mun ON mun.muncod = ende.muncod
	//				LEFT JOIN obras.tipoobra toba ON toba.tobaid = obr.tobraid
	//				WHERE obsstatus = 'A'";
	
	        $sql = "SELECT
						{$acao}
						obr.obrnome,
						tob.tobdesc
					FROM
						obras2.obras obr
					INNER JOIN par.subacaoobravinculacao 	sov ON sov.obrid = obr.obrid $ano and sov.sbaid = '$sbaid'
					INNER JOIN entidade.endereco 			ende ON ende.endid = obr.endid
					LEFT  JOIN obras.preobra 				pre ON pre.preid = sov.preid
					LEFT  JOIN territorios.municipio 		mun ON mun.muncod = ende.muncod
					LEFT  JOIN obras2.tipoobra 				tob ON tob.tobid = obr.tobid
					WHERE
						obrstatus = 'A'";
	        return $this->monta_lista($sql, $cabecalho, 2000, 10, "N", "100%", "N");
	    }
	
	    function excluirPreObra($preid) {
	        $sql = "update obras.preobra set prestatus = 'I' where preid = $preid";
	        return $this->executar($sql);
	    }
	
	    function recuperarReformulacao($preid) {
	        $sql = " select preid from obras.preobra where preidpai = " . $preid;
	        return $this->pegaUm($sql);
	    }
	
	    /**
	     * Monta o Grafico de Obras Aprovadas Empenho Pac
	     * @return array
	     */
	    public function recuperarPercentualObrasAprovadasEmpenhoPac() {
	        $sql = "
	                SELECT 
	                    count(foo.preid) as valor,
	                    foo.descricao
	                FROM(
	                       SELECT DISTINCT
	                               pre.preid,
	                               CASE WHEN ROUND(coalesce(veo.saldo,0)) >= ROUND(COALESCE(pre.prevalorobra,0)) 
	                                      THEN 'OBRAS 100% EMPENHADAS'
	                                      WHEN ( ROUND(coalesce(veo.saldo,0)) < ROUND(COALESCE(pre.prevalorobra,0)) AND ROUND(coalesce(veo.saldo,0)) > 0  )
	                                              THEN 'OBRAS PARCIALMENTE EMPENHADAS'
	                                      WHEN  ROUND(coalesce(veo.saldo,0)) = 0
	                                              THEN 'OBRAS SEM EMPENHO'
	                                      ELSE 'OBRAS COM SALDO NEGATIVO'
	                               END as descricao
	                       FROM obras.preobra pre
	                       INNER JOIN workflow.documento doc ON doc.docid   = pre.docid  
	                       INNER JOIN workflow.estadodocumento esd ON esd.esdid = doc.esdid AND esd.tpdid = " . WF_FLUXO_PRO_INFANCIA . "
	                       LEFT JOIN par.empenhoobra emp on emp.preid = pre.preid AND eobstatus = 'A'
	                       LEFT JOIN par.v_dadosfinanceiroporobra veo ON veo.preid = emp.preid
	                       WHERE pre.prestatus = 'A' 
	                           AND pre.preidpai IS NULL 
	                           AND pre.tooid = 1
	                           AND presistema = 23  
	                           AND esd.esdid = " . WF_TIPO_OBRA_APROVADA . "
	                ) foo
	                GROUP BY foo.descricao ORDER BY valor DESC";
	        return $this->carregar($sql);
	    }
	
	    /**
	     * Recupera lista de obras Aprovadas
	     * @param array $arrPost
	     * @return array
	     */
	    public function recuperarObrasAprovadasEmpenhoPac($arrPost = array()) {
	        #Retorna lista de filtros
	        $strWhere = $this->getFiltrosPainel($arrPost);
	
	        $strArea = str_replace(' ', '', $arrPost['idArea']);
	        switch ($strArea) {
	            case 'OBRAS100%EMPENHADAS':
	                $strWhere .= " AND (ROUND(coalesce(veo.saldo,0)) >= ROUND(COALESCE(pre.prevalorobra,0)))";
	                break;
	            case 'OBRASPARCIALMENTEEMPENHADAS':
	                $strWhere .= " AND (ROUND(coalesce(veo.saldo,0)) < ROUND(COALESCE(pre.prevalorobra,0)) AND ROUND(coalesce(veo.saldo,0))>0)";
	                break;
	            case 'OBRASSEMEMPENHO':
	                $strWhere .= " AND (ROUND(coalesce(veo.saldo,0)) = 0)";
	                break;
	            case 'OBRASCOMSALDONEGATIVO':
	                $strWhere .= " AND (ROUND(coalesce(veo.saldo,0)) < 0)";
	                break;
	        }
	
	        $sql = "SELECT 
	                        po.pronumeroprocesso as acao,
	                        oi.obrid,
	                        pre.preid,
	                        po.proid,
	                        pre.predescricao,
	                        pto.ptodescricao,
	                        pre.estuf,
	                        mun.mundescricao,
	                        '<span class=\"processo_detalhe\" >'||to_char(po.pronumeroprocesso::bigint, 'FM00000\".\"000000\"/\"0000\"-\"00')||'</span>' as processo,
	                        ROUND(pre.prevalorobra,2) as prevalorobra,
	                        ROUND(pre.prevalorobra,2) as prevalorobra2,
	                        ROUND(pre.prevalorobra,2) as prevalorobra3,
	                        (select usunome from (
	                                select distinct
	                                        max(hd1.htddata) as data,
	                                        us1.usunome,
	                                        hd1.docid,
	                                        ed1.esddsc
	                                from workflow.historicodocumento hd1
	                                        inner join workflow.acaoestadodoc ac1 on
	                                                ac1.aedid = hd1.aedid
	                                        inner join workflow.estadodocumento ed1 on
	                                                ed1.esdid = ac1.esdidorigem AND ed1.tpdid = 37
	                                        inner join seguranca.usuario us1 on
	                                                us1.usucpf = hd1.usucpf
	                                        left join workflow.comentariodocumento cd1 on
	                                                cd1.hstid = hd1.hstid
	                                where
	                                        ac1.esdiddestino in (210,211,212)
	                                and
	                                        hd1.docid = pre.docid
	                                group by us1.usunome, hd1.docid, ed1.esddsc, hd1.htddata
	                                order by data desc limit 1
	                                ) as foo
	                        ) as nomeanalista
	                    FROM obras.preobra pre
	                    LEFT JOIN par.empenhoobra emp on emp.preid = pre.preid AND eobstatus = 'A'
	                    LEFT JOIN par.v_dadosfinanceiroporobra veo ON veo.preid = emp.preid
	                    INNER JOIN workflow.documento doc ON doc.docid   = pre.docid  
	                    LEFT JOIN workflow.estadodocumento esd ON esd.esdid = doc.esdid AND esd.tpdid = " . WF_FLUXO_PRO_INFANCIA . "
	                    LEFT JOIN workflow.historicodocumento hst on doc.hstid=hst.hstid
	                    LEFT JOIN seguranca.usuario usu ON hst.usucpf = usu.usucpf
	                    LEFT JOIN obras.pretipoobra pto ON pre.ptoid = pto.ptoid
	                    LEFT JOIN territorios.municipio mun
	                    INNER JOIN territorios.muntipomunicipio mtpm
	                    INNER JOIN territorios.tipomunicipio tpm ON tpm.tpmid = mtpm.tpmid AND tpmstatus = 'A' AND gtmid = 7
	                            ON mtpm.muncod = mun.muncod
	                            ON pre.muncod  = mun.muncod
	                    LEFT JOIN par.resolucao res ON res.resid = pre.resid
	                    LEFT JOIN (
	                            SELECT
	                                    poap.preid,
	                                    poap.poausucpfinclusao,
	                                    usup.usunome as nomeanalista
	                            FROM obras.preobraanalise poap
	                            LEFT JOIN seguranca.usuario usup ON usup.usucpf = poap.poausucpfinclusao
	                            ) poa ON poa.preid = pre.preid
	                    LEFT  JOIN obras2.obras oi
	                            INNER JOIN workflow.documento doc2 ON doc2.docid = oi.docid AND doc2.tpdid = 105 AND oi.obrstatus = 'A' AND oi.obridpai IS NULL
	                            INNER JOIN workflow.estadodocumento esd2 ON esd2.esdid = doc2.esdid
	                    ON oi.preid = pre.preid AND obridpai IS NULL
	                    LEFT JOIN par.processoobraspaccomposicao popc on popc.preid = pre.preid and popc.pocstatus = 'A'
	                    LEFT JOIN par.processoobra po on po.proid = popc.proid and po.prostatus = 'A'
	                    WHERE pre.prestatus = 'A' AND esd.esdid = " . WF_TIPO_OBRA_APROVADA . "
	                    AND pre.preidpai IS NULL AND 
	                    pre.tooid = 1  
	                    AND presistema = 23
	                    {$strWhere}
	                    GROUP BY 
	                        pre.estuf,
	                        mun.mundescricao,
	                        pre.predescricao,
	                        pto.ptodescricao,
	                        po.pronumeroprocesso,
	                        oi.obrid,
	                        pre.preid,
	                        pre.prevalorobra,
	                        pre.docid,
	                        po.proid
	                    ORDER BY 
	                        po.pronumeroprocesso
	                    ";
	
	        return $sql;
	    }
	
	    /**
	     * Filtro padrao para a primeira versao do Painel
	     * @param array $arrPost
	     * @return string
	     */
	    public function getFiltrosPainel($arrPost) {
	        $strWhere = '';
	        if (is_array($arrPost['estuf'])) {
	            if ($arrPost['estuf'][0] != '') {
	                $strWhere .= " AND mun.estuf IN ('" . implode("', '", $arrPost['estuf']) . "') ";
	            }
	        }
	
	        if (is_array($arrPost['muncod'])) {
	            if ($arrPost['muncod'][0] != '') {
	                $strWhere .= " AND mun.muncod IN ('" . implode("', '", $arrPost['muncod']) . "') ";
	            }
	        }
	
	        if (is_array($arrPost['ptoid'])) {
	            if ($arrPost['ptoid'][0] != '') {
	                $strWhere .= " AND pre.ptoid IN ('" . implode("', '", $arrPost['ptoid']) . "') ";
	            }
	        }
	
	        if (is_array($arrPost['usucpf'])) {
	            if ($arrPost['usucpf'][0] != '') {
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
	                                    hd1.usucpf IN ('" . implode("', '", $arrPost['usucpf']) . "')
	                            group by hd1.docid) as foo";
	                $arDocid = $this->carregar($sql);
	                $arrDoc = array();
	                if ($arDocid) {
	                    foreach ($arDocid as $key => $value) {
	                        $arrDoc[] = $value['docid'];
	                    }
	                }
	                $strWhere .= " AND doc.docid IN (" . implode(', ', $arrDoc) . ")";
	            }
	        }
	        return $strWhere;
	    }
	
	    public function recuperarPercentualObrasAprovadasEmpenhoPar() {
	        $sql = "
	                SELECT 
	                    count(foo.preid) as valor,
	                    foo.descricao
	                FROM(
	                       SELECT DISTINCT
	                               pre.preid,
	                               CASE WHEN ROUND(coalesce(veo.saldo,0)) >= ROUND(COALESCE(pre.prevalorobra,0)) 
	                                      THEN 'OBRAS 100% EMPENHADAS'
	                                      WHEN ( ROUND(coalesce(veo.saldo,0)) < ROUND(COALESCE(pre.prevalorobra,0)) AND ROUND(coalesce(veo.saldo,0)) > 0  )
	                                              THEN 'OBRAS PARCIALMENTE EMPENHADAS'
	                                      WHEN  ROUND(coalesce(veo.saldo,0)) = 0
	                                              THEN 'OBRAS SEM EMPENHO'
	                                      ELSE 'OBRAS COM SALDO NEGATIVO'
	                               END as descricao
	                       FROM obras.preobra pre
	                       INNER JOIN workflow.documento doc ON doc.docid   = pre.docid  
	                       INNER JOIN workflow.estadodocumento esd ON esd.esdid = doc.esdid AND esd.tpdid = " . WF_FLUXO_OBRAS_PAR . "
	                       LEFT JOIN par.empenhoobrapar emp on emp.preid = pre.preid AND eobstatus = 'A'
	                       LEFT JOIN par.v_dadosfinanceiroporobra veo ON veo.preid = emp.preid
	                       WHERE pre.prestatus = 'A' 
	                           AND pre.preidpai IS NULL 
	                           AND presistema = 23
	                           AND esd.esdid = " . WF_PAR_OBRA_APROVADA . "
	                ) foo
	                GROUP BY foo.descricao ORDER BY valor DESC";
	        return $this->carregar($sql);
	    }
	
	    /**
	     * Recupera lista de obras Aprovadas
	     * @param array $arrPost
	     * @return array
	     */
	    public function recuperarObrasAprovadasEmpenhoPar($arrPost = array()) {
	        #Retorna lista de filtros
	        $strWhere = $this->getFiltrosPainel($arrPost);
	
	        $strArea = str_replace(' ', '', $arrPost['idArea']);
	        switch ($strArea) {
	            case 'OBRAS100%EMPENHADAS':
	                $strWhere .= " AND (ROUND(coalesce(veo.saldo,0)) >= ROUND(COALESCE(pre.prevalorobra,0)))";
	                break;
	            case 'OBRASPARCIALMENTEEMPENHADAS':
	                $strWhere .= " AND (ROUND(coalesce(veo.saldo,0)) < ROUND(COALESCE(pre.prevalorobra,0)) AND ROUND(coalesce(veo.saldo,0))>0)";
	                break;
	            case 'OBRASSEMEMPENHO':
	                $strWhere .= " AND (ROUND(coalesce(veo.saldo,0)) = 0)";
	                break;
	            case 'OBRASCOMSALDONEGATIVO':
	                $strWhere .= " AND (ROUND(coalesce(veo.saldo,0)) < 0)";
	                break;
	        }
	
	        $sql = "SELECT 
	                        po.pronumeroprocesso as acao,
	                        oi.obrid,
	                        pre.preid,
	                        po.proid,
	                        pre.predescricao,
	                        pto.ptodescricao,
	                        pre.estuf,
	                        mun.mundescricao,
	                        '<span class=\"processo_detalhe\" >'||to_char(po.pronumeroprocesso::bigint, 'FM00000\".\"000000\"/\"0000\"-\"00')||'</span>' as processo,
	                        ROUND(pre.prevalorobra,2) as prevalorobra,
	                        ROUND(pre.prevalorobra,2) as prevalorobra2,
	                        ROUND(pre.prevalorobra,2) as prevalorobra3,
	                        (select usunome from (
	                                select distinct
	                                        max(hd1.htddata) as data,
	                                        us1.usunome,
	                                        hd1.docid,
	                                        ed1.esddsc
	                                from workflow.historicodocumento hd1
	                                        inner join workflow.acaoestadodoc ac1 on
	                                                ac1.aedid = hd1.aedid
	                                        inner join workflow.estadodocumento ed1 on
	                                                ed1.esdid = ac1.esdidorigem AND ed1.tpdid = 37
	                                        inner join seguranca.usuario us1 on
	                                                us1.usucpf = hd1.usucpf
	                                        left join workflow.comentariodocumento cd1 on
	                                                cd1.hstid = hd1.hstid
	                                where
	                                        ac1.esdiddestino in (210,211,212)
	                                and
	                                        hd1.docid = pre.docid
	                                group by us1.usunome, hd1.docid, ed1.esddsc, hd1.htddata
	                                order by data desc limit 1
	                                ) as foo
	                        ) as nomeanalista
	                    FROM obras.preobra pre
	                    LEFT JOIN par.empenhoobrapar emp on emp.preid = pre.preid AND eobstatus = 'A'
	                    LEFT JOIN par.v_dadosfinanceiroporobra veo ON veo.preid = emp.preid
	                    INNER JOIN workflow.documento doc ON doc.docid   = pre.docid  
	                    LEFT JOIN workflow.estadodocumento esd ON esd.esdid = doc.esdid AND esd.tpdid = 37
	                    LEFT JOIN workflow.historicodocumento hst on doc.hstid=hst.hstid
	                    LEFT JOIN seguranca.usuario usu ON hst.usucpf = usu.usucpf
	                    LEFT JOIN obras.pretipoobra pto ON pre.ptoid = pto.ptoid
	                    LEFT JOIN territorios.municipio mun
	                    INNER JOIN territorios.muntipomunicipio mtpm
	                    INNER JOIN territorios.tipomunicipio tpm ON tpm.tpmid = mtpm.tpmid AND tpmstatus = 'A' AND gtmid = 7
	                            ON mtpm.muncod = mun.muncod
	                            ON pre.muncod  = mun.muncod
	                    LEFT JOIN par.resolucao res ON res.resid = pre.resid
	                    LEFT JOIN (
	                            SELECT
	                                    poap.preid,
	                                    poap.poausucpfinclusao,
	                                    usup.usunome as nomeanalista
	                            FROM obras.preobraanalise poap
	                            LEFT JOIN seguranca.usuario usup ON usup.usucpf = poap.poausucpfinclusao
	                            ) poa ON poa.preid = pre.preid
	                    LEFT  JOIN obras2.obras oi
	                            INNER JOIN workflow.documento doc2 ON doc2.docid = oi.docid AND doc2.tpdid = 105 AND oi.obrstatus = 'A' AND oi.obridpai IS NULL
	                            INNER JOIN workflow.estadodocumento esd2 ON esd2.esdid = doc2.esdid
	                    ON oi.preid = pre.preid AND obridpai IS NULL
	                    LEFT JOIN par.processoobraspaccomposicao popc on popc.preid = pre.preid and popc.pocstatus = 'A'
	                    LEFT JOIN par.processoobra po on po.proid = popc.proid and po.prostatus = 'A'
	                    WHERE pre.prestatus = 'A' AND esd.esdid = 228
	                    AND pre.preidpai IS NULL AND 
	                    pre.tooid = 1  
	                    AND presistema = 23
	                    {$strWhere}
	                    GROUP BY 
	                        pre.estuf,
	                        mun.mundescricao,
	                        pre.predescricao,
	                        pto.ptodescricao,
	                        po.pronumeroprocesso,
	                        oi.obrid,
	                        pre.preid,
	                        pre.prevalorobra,
	                        pre.docid,
	                        po.proid
	                    ORDER BY 
	                        po.pronumeroprocesso
	                    ";
	        return $this->carregar($sql);
	    }
	
	    public function getMaxDataFimVigenciaObra($dopid) {
	        $sql = "SELECT 
	                        TO_CHAR(MAX(to_date(foo.datafimvigencia, 'MM/YYYY')), 'MM/YYYY') 
	                    FROM 
	                    (
	                        SELECT 
	                            po.preid,
	                            CASE WHEN pop.popdataprazoaprovado IS NOT NULL 
	                                THEN TO_CHAR(pop.popdataprazoaprovado, 'mm/YYYY')
	                                ELSE (SELECT DISTINCT TO_CHAR(dpvdatavalidacao, 'mm/YYYY') FROM par.documentoparvalidacao where dopid = {$dopid})
	                            END as datafimvigencia
	                        FROM {$this->stNomeTabela} po
	                        LEFT JOIN obras.preobraprorrogacao pop ON pop.preid = po.preid
	                        WHERE po.preid in ( 
	                            SELECT DISTINCT 
	                                preid
	                            FROM par.documentopar d
	                            INNER JOIN par.termocomposicao tc on tc.dopid = d.dopid
	                            WHERE d.dopid = {$dopid}
	                        )
	                    ) foo";
	        return $this->pegaUm($sql);
	    }
	
	    public function relatorioFinanceiroObra($arrPost = array(), $booSQL = FALSE) {
	//        ver($arrPost, d);
	        #armazena as condicoes da consulta
	        $arrWhere = array("pre.prestatus = 'A'");
	
	        #obrid
	        if (!empty($arrPost['obrid'])) {
	            $arrWhere[] = "pre.obrid = {$arrPost['obrid']}";
	        }
	
	        #preid
	        if (!empty($arrPost['preid'])) {
	            $arrWhere[] = "pre.preid = {$arrPost['preid']}";
	        }
	
	        #processo
	        if (!empty($arrPost['empnumeroprocesso'])) {
	            $arrWhere[] = "vemp.processo = '{$arrPost['empnumeroprocesso']}'";
	        }
	
	        #nome da obra
	        if (!empty($arrPost['predescricao'])) {
	            $arrPost['predescricao'] = strtoupper($arrPost['predescricao']);
	            $arrWhere[] = "UPPER(pre.predescricao) like '%{$arrPost['predescricao']}%'";
	        }
                
                #vigencia de
                if (!empty($arrPost['dtvigenciainicio'])) {
                    $arrWhere[] = "vvo.vigencia >= '{$arrPost['dtvigenciainicio']}'";
                }

                #vigencia ate
                if (!empty($arrPost['dtvigenciafim'])) {
                    $arrWhere[] = "vvo.vigencia <= '{$arrPost['dtvigenciafim']}'";
                }
                
	        #tipo da obra
	        if (!empty($arrPost['ptoid'])) {
	            $arrWhere[] = "pre.ptoid = {$arrPost['ptoid']}";
	        }
	
	        #esfera
	        if (!empty($arrPost['preesfera'])) {
	            if ($arrPost['preesfera'] != 'T') {
	                if ($arrPost['preesfera'] == 'M') {
	                    unset($arrPost['estuf']);
	                }
	                    $arrWhere[] = "pre.preesfera = '{$arrPost['preesfera']}'";
	            }
	        }
	
	        #uf
	        if (!empty($arrPost['estuf'][0])) {
	            $arrWhere[] = "pre.estuf IN ('" . implode("', '", $arrPost['estuf']) . "')";
	        }
	        
	        #municipio
	        if (!empty($arrPost['listaMunicipio'][0])) {
	            $arrWhere[] = "pre.muncod IN ('" . implode("', '", $arrPost['listaMunicipio']) . "')";
	        }
	        
	        #trata o numero do processo para Excel ou Listagem
	        $empnumeroprocesso = "'<span class=\"processo_detalhe\" >' || to_char(foo.processo::bigint, 'FM00000\".\"000000\"/\"0000\"-\"00') || '</span>'";
	        if (isset($arrPost['btnExcel'])) {
	            $empnumeroprocesso = "(TO_CHAR(foo.processo::bigint, 'FM00000\".\"000000\"/\"0000\"-\"00'))";
	        }
	        
	        #possui empenho
	        if (!empty($arrPost['empenho'])) {
	            if ($arrPost['empenho'] != 'T') {
	                $strNot = ($arrPost['empenho'] == 'N') ? 'NOT' : '';
	                $arrWhere[] = "vemp.processo $strNot IN(
	                            SELECT
	                                e.empnumeroprocesso
	                            FROM par.empenho e
	                            INNER JOIN par.v_vrlempenhocancelado vve on vve.empid = e.empid
	                            WHERE e.empstatus = 'A'
	                                AND e.empcodigoespecie NOT IN ('03', '13', '02', '04')
	                                AND e.empnumeroprocesso=vemp.processo
	                                AND vve.vrlempenhocancelado > 0)";
	            }
	        }
	        
	        #possui pagamento
	        if (!empty($arrPost['pagamento'])) {
	            if ($arrPost['pagamento'] != 'T') {
	                $strSinal = ($arrPost['pagamento'] == 'S') ? '>' : '=';
	                $arrWhere[] = "(
	                    SELECT 
	                        SUM(foo.total) 
	                    FROM (
	                        SELECT 
	                            COUNT(preid) as total 
	                        FROM par.pagamentoobra poi 
	                        INNER JOIN par.pagamento pagi ON pagi.pagid = poi.pagid
	                        WHERE pagi.pagsituacaopagamento NOT ilike '%cancelado%' 
	                        AND poi.preid = pre.preid AND pagi.pagstatus = 'A'
	                        UNION ALL
	                        SELECT 
	                                COUNT(preid) 
	                        FROM par.pagamentoobrapar poi 
	                        INNER JOIN par.pagamento pagi ON pagi.pagid = poi.pagid
	                        WHERE pagi.pagsituacaopagamento NOT ilike '%cancelado%' 
	                        AND poi.preid = pre.preid AND pagi.pagstatus = 'A'
	                    ) foo) $strSinal 0";
	            }
	        }
	        
	        $sql = "SELECT DISTINCT
	                    pre.obrid,
	                    pre.preid,
	                    pre.predescricao,
	                    pre.estuf,
	                    (CASE WHEN pre.muncod IS NOT NULL 
	                        THEN (SELECT mun.mundescricao FROM territorios.municipio mun WHERE mun.muncod = pre.muncod) 
	                        ELSE '-'
	                    END) as mundescricao,
	                    (CASE WHEN pre.muncod IS NOT NULL 
				THEN pre.muncod 
				ELSE '-'
			    END) as muncod,
	                    pre.preesfera,
	                    COALESCE(pre.prevalorobra, 0.00) as prevalorobra,
	                    COALESCE(vemp.saldo, 0.00) as saldo,
	                    COALESCE(vobra.totalpago, 0.00) as vlrpago,
	                    COALESCE((vemp.saldo - vobra.totalpago), 0.00) as valorrestante,
	                    (SELECT DISTINCT
	                        $empnumeroprocesso as processo
	                    FROM (
	                        SELECT
	                            po.pronumeroprocesso as processo
	                            FROM par.processoobraspaccomposicao popc
	                            INNER JOIN par.processoobra po ON popc.proid = po.proid 
	                            WHERE popc.preid = pre.preid
	                        UNION 
	                        SELECT
	                            po.pronumeroprocesso as processo
	                            FROM par.processoobrasparcomposicao popc
	                            INNER JOIN par.processoobraspar po ON popc.proid = po.proid 
	                            WHERE popc.preid = pre.preid
	                    ) foo LIMIT 1) as processo,
	                    pto.ptodescricao,
	                    (SELECT DISTINCT
	                        foo.tipo
	                    FROM (
	                        SELECT
	                            'PAC' as tipo
	                            FROM par.processoobraspaccomposicao popc
	                            INNER JOIN par.processoobra po ON popc.proid = po.proid 
	                            WHERE popc.preid = pre.preid
	                        UNION 
	                        SELECT
	                            'PAR' as tipo
	                            FROM par.processoobrasparcomposicao popc
	                            INNER JOIN par.processoobraspar po ON popc.proid = po.proid 
	                            WHERE popc.preid = pre.preid
	                    ) foo LIMIT 1) as tipo
	                FROM {$this->stNomeTabela} pre
	                INNER JOIN obras.pretipoobra pto ON pre.ptoid = pto.ptoid
	                LEFT JOIN par.vm_saldo_empenho_por_obra vemp ON vemp.preid = pre.preid
	                LEFT JOIN par.v_pagamento_total_por_obra vobra ON vobra.preid = pre.preid
                        LEFT JOIN par.v_vigenciaobras vvo ON vvo.preid = pre.preid
	                WHERE " . implode(" AND ", $arrWhere) ."
	                ORDER BY pre.estuf";
	        return ($booSQL) ? $sql : $this->carregar($sql);
	    }
	
		public function recuperaProgramaObra( $preid ){
			
			$sql = "SELECT count(*) FROM par.processoobraspaccomposicao WHERE pocstatus = 'A' AND preid = ".$preid;
			$PAC = $this->pegaUm($sql);
			 
			$sql = "SELECT count(*) FROM par.processoobrasparcomposicao WHERE pocstatus = 'A' AND preid = ".$preid;
			$PAR = $this->pegaUm($sql);
			
			return $PAC > 0 ? 'PAC' : 'PAR';
		}    
	    
	    public function importarPreobraParaObras2( $preid ){
	    	
	    	/*** INICIO - Importa��o dos dados para o sistema de Obras - INICIO ***/
	    	$programa = $this->recuperaProgramaObra( $preid );
	    	include_once APPRAIZ . "www/par/_constantes.php";
	    	
	    	$sql = "SELECT obrid FROM obras.obrainfraestrutura WHERE preid = $preid";
	    	$obrid_1 = $this->pegaUm( $sql );
	    	$obrid_1 = $obrid_1 ? $obrid_1 : 'NULL';

	    	if( $programa == 'PAR' ){
                    /*** S� executa a importa��o caso a obra n�o exista ***/
                    $sql = "SELECT
                                doc.esdid
                            FROM
                                obras.preobra pre
                            INNER JOIN workflow.documento doc ON doc.docid = pre.docid
                            WHERE
                                pre.preid = ".$preid;
                    $esdid = $this->pegaUm( $sql );

//		    	$arrCondicional = Array( WF_SUBACAO_DILIGENCIA_CONDICIONAL, WF_SUBACAO_APROVACAO_CONDICIONAL );
                    $arrCondicional = Array(WF_PAR_OBRA_EM_APROVACAO_CONDICIONAL, WF_PAR_EM_DILIGENCIA, WF_PAR_OBRA_EM_CADASTRAMENTO_CONDICIONAL);

                    // Se for condicional n�o importam os dados da obra (a pedido do Thiago no dia 01/08/2013)
                    if(in_array( $esdid, $arrCondicional )){
                        $sqlEstadoDoc = "SELECT upper(esddsc) as esddsc FROM workflow.estadodocumento WHERE esdid = $esdid";
                        $esddsc = $this->pegaUm( $sqlEstadoDoc ); 

                        echo "ERRO AO INSERIR A OBRA: $preid ENCONTRA-SE EM SITUACAO DE {$esddsc}<br>";
                        return false;
                    }
	    	}
	    	
			$sql = "SELECT count(1) FROM obras.preobra WHERE preid = $preid AND obrid IS NOT NULL";
			$existeObra = $this->pegaUm($sql);

			if( $existeObra < 1 ){
				
				if( $programa == 'PAR' ){
					
					$colunas = "/*CASE WHEN pro.sisid = 23 
		    						THEN 39 --PAR
									ELSE 42 --Emenda Parlamentar
								END as programa,*/
								CASE
									WHEN pto.ptoclassificacaoobra = 'Q' THEN 50 --QUADRA
									WHEN pto.ptoclassificacaoobra = 'P' THEN 41 --PROINFANCIA
									WHEN pto.ptoclassificacaoobra = 'C' THEN 55 --COBERTURA
									ELSE 54 --OUTROS
				                END as programa,
		    					CASE
									WHEN i.indcod in (3,4,7,8) THEN '1'
									WHEN i.indcod in (5,6,10,9) THEN '2'
									ELSE 'NULL'
								END AS modalidade_ensino,
		    					CASE
									WHEN s.ppsid in (652,695,810,896,897,965,966,971,972,977,983,987,989,1015,1016,1091,1093,1099,1104,1105,1115,1116,1119,1120,1122,1124) THEN 1 --RURAL
									WHEN s.ppsid in (494,495,521,555,568,577,624,633,671,676,698,718,783,802,867,882,957,958,961,962,963,964,981,982,1013,1014,1088,1089,1090,1098,1111,1112,1117,1118,1121,1123,1153,1154,1158,1159) THEN 2 --URBANA
									WHEN s.ppsid in (605,655,854,900,901,969,970,975,976,980,986,988,990,1094) THEN 3 --QUILOMBOLA
									WHEN s.ppsid in (542,710,768,801,898,899,967,968,973,974,978,979,984,985) THEN 4 --IND�GENA
									ELSE 2 --URBANA
	    						END AS classificacaoobra,
								CASE
									WHEN sisid = 23 THEN 11 -- TD
									WHEN sisid = 57 THEN 4 -- EMENDAS
									ELSE 11 -- TD
								END AS fonte";
					
					$inner = "	INNER JOIN par.subacaoobra    				so  ON so.preid  = p.preid 
								INNER JOIN par.subacao     					s 	ON s.sbaid   = so.sbaid AND s.sbastatus = 'A'
								INNER JOIN par.acao                			ac  ON ac.aciid  = s.aciid  AND ac.acistatus = 'A'
								INNER JOIN par.pontuacao        			po  ON po.ptoid  = ac.ptoid AND po.ptostatus = 'A'
								INNER JOIN par.instrumentounidade 			iu  ON iu.inuid  = po.inuid 
								INNER JOIN par.criterio             		c   ON c.crtid 	 = po.crtid 
								INNER JOIN par.indicador          			i   ON i.indid 	 = c.indid 
								INNER JOIN par.area                 		a   ON a.areid 	 = i.areid 
								INNER JOIN par.dimensao 					d   ON d.dimid 	 = a.dimid
								INNER JOIN par.processoobrasparcomposicao 	poc ON poc.preid = p.preid   AND poc.pocstatus = 'A'
								INNER JOIN par.processoobraspar 			pro ON pro.proid = poc.proid AND pro.inuid = iu.inuid  AND pro.prostatus = 'A'";
				}else{
					
					$colunas = "CASE
									WHEN pto.ptoclassificacaoobra = 'Q' THEN 50 --QUADRA
									WHEN pto.ptoclassificacaoobra = 'P' THEN 41 --PROINFANCIA
									WHEN pto.ptoclassificacaoobra = 'C' THEN 55 --COBERTURA
									ELSE 54 --OUTROS
				                END as programa,
								CASE WHEN pto.ptoclassificacaoobra = 'Q' THEN 3 ELSE 1 END as modalidade_ensino, -- MODALIDADE DE ENSINO
		    					CASE
									WHEN REPLACE(UPPER(p.predescricao), '�', 'I') ILIKE '%INDIGENA%' THEN 4 -- IND�GENA
									WHEN UPPER(p.predescricao) ILIKE '%RURAL%' THEN 1 -- RURAL
									WHEN UPPER(p.predescricao) ILIKE '%QUILOMBO%' THEN 3 -- QUILOMBO
									ELSE 2 --URBANO
								END AS classificacaoobra,
								p.tooid AS fonte";
					
					$inner = "	INNER JOIN par.processoobraspaccomposicao 	poc ON poc.preid = p.preid
								INNER JOIN par.processoobra 				pro ON pro.proid = poc.proid";
					
				}
				
				/*** Recupera dados da Pre Obra ***/
				$sql = "SELECT DISTINCT
							p.predescricao || ' - ' || mun.mundescricao || ' - ' || mun.estuf as nome_obra,
							p.precep,
							p.prelogradouro,
							p.precomplemento,
							p.prebairro,
							p.muncod,
							p.estuf,
							p.prenumero,
							p.prelatitude,
							p.prelongitude,
							UPPER(p.preesfera) AS preesfera,
							pto.tpoid as tipologiaobra,
							CASE
								WHEN pto.ptodescricao ILIKE '%REFORMA%' THEN 4 --REFORMA
								WHEN pto.ptodescricao ILIKE '%AMPLIA%' THEN 3 --AMPLIA��O
								ELSE 1 --CONSTRU��O
							END AS tipodeobra,
							prevalorobra as valorobra,
							pro.pronumeroprocesso,
							$colunas
						FROM
							obras.preobra         p  
						INNER JOIN obras.pretipoobra 		pto ON pto.ptoid = p.ptoid
						INNER JOIN territorios.municipio 	mun ON mun.muncod = p.muncod
						$inner
						WHERE
							p.preid = ".$preid;

				$dadosPreObra = $this->pegaLinha($sql);
	    				
				//DEFINDO A ENTIDADE
				if($dadosPreObra['preesfera']=='M'){
					$sql = "SELECT ent.entid
							FROM entidade.entidade ent
							INNER JOIN entidade.endereco 		ed  ON ed.entid = ent.entid
							INNER JOIN entidade.funcaoentidade 	fue ON ent.entid = fue.entid
							WHERE 
								ent.entstatus = 'A'
								AND fue.funid IN (1)
								AND fue.fuestatus = 'A'
								AND ed.muncod = '{$dadosPreObra['muncod']}'";
					$unidade_implantadora = $this->pegaUm($sql);
				}else{
					$sql = "SELECT ent.entid
							FROM entidade.entidade ent
							INNER JOIN entidade.endereco 		ed  ON ed.entid = ent.entid
							INNER JOIN entidade.funcaoentidade 	fue ON ent.entid = fue.entid
							WHERE 
								ent.entstatus = 'A'
								AND fue.funid IN (6)
								AND fue.fuestatus = 'A'
								AND ed.estuf = '{$dadosPreObra['estuf']}'";
					$unidade_implantadora = $this->pegaUm($sql);
				}
	    	
				/*** Insere novo endere�o da obra ***/
				$sql = "INSERT INTO entidade.endereco (
							tpeid, 			endcep, 	endlog,
							endcom, 		endbai,
							muncod, 		estuf,
							endnum, 		medlatitude,
							medlongitude, 	endstatus)
						VALUES
							( 4, 									'{$dadosPreObra['precep']}', 		'{$dadosPreObra['prelogradouro']}',
							'{$dadosPreObra['precomplemento']}', 	'".substr(removerEspacoDuplicado($dadosPreObra['prebairro']),0,100)."',
							'{$dadosPreObra['muncod']}', 			'{$dadosPreObra['estuf']}',
							'{$dadosPreObra['prenumero']}', 		'{$dadosPreObra['prelatitude']}',
							'{$dadosPreObra['prelongitude']}', 	'A' ) 
						RETURNING endid";
				    	
				$endid = $this->pegaUm($sql);
				$this->commit();
	    				
				/*** Insere a nova obra ***/
				$sql = "INSERT INTO obras2.empreendimento(
							orgid, 									empesfera,
							tpoid,
							prfid,									tobid,
							tooid, 									cloid,
							moeid, 									entidunidade,
							empdsc,
							empvalorprevisto, 						endid,
							preid,
							obrid_1
							) 
						VALUES (
							3, 										'{$dadosPreObra['preesfera']}',
							".($dadosPreObra['tpoid'] ? $dadosPreObra['tpoid'] : 'NULL').",
							{$dadosPreObra['programa']},			'{$dadosPreObra['tipodeobra']}',
							{$dadosPreObra['fonte']},				'{$dadosPreObra['classificacaoobra']}',
							{$dadosPreObra['modalidade_ensino']}, 	$unidade_implantadora,
							'".(str_ireplace( "'", "", $dadosPreObra['nome_obra']))."',
							'{$dadosPreObra['valorobra']}', 		$endid,
							'$preid', 								$obrid_1) 
						RETURNING empid;";
	    	
				$empid = $this->pegaUm( $sql );
                                $this->commit();
	    	
				/*** Insere a nova obra ***/
				$sql = "INSERT INTO obras2.obras(
							obrnome,
							entid,								tooid,
							preid,								endid,
							tpoid,
							tobid, 								cloid,
							obrvalorprevisto,					empid,
							obrid_1, 							obrnumprocessoconv)
						VALUES(
							'".(str_ireplace( "'", "", $dadosPreObra['nome_obra']))."',
							$unidade_implantadora,				'{$dadosPreObra['fonte']}',
							$preid,								$endid,
							".($dadosPreObra['tipologiaobra'] ? $dadosPreObra['tipologiaobra'] : 'NULL').",
							'{$dadosPreObra['tipodeobra']}',	'{$dadosPreObra['classificacaoobra']}',
							'{$dadosPreObra['valorobra']}',		$empid,
							$obrid_1, 							'{$dadosPreObra['pronumeroprocesso']}')
						RETURNING obrid";
				$obrid = $this->pegaUm($sql);
                                $this->commit();
	    	
				/*
				* Cria Documento WF - In�cio
				*/
				require_once APPRAIZ . 'includes/workflow.php';
				$docdsc = "Fluxo de obra do m�dulo Obras II - obrid " . $obrid;
				$arrMi = Array(OBR_TPOID_MI_TIPO_B, OBR_TPOID_MI_TIPO_C);
	    			
				if( in_array( $dadosPreObra['tipologiaobra'], $arrMi ) ){
	    				
					$docid = wf_cadastrarDocumento(TPDID_OBJETO, $docdsc, OBR_ESDID_AGUARDANDO_ADESAO_DO_MUNICIPIO);
					$sql = "UPDATE obras2.obras SET docid = $docid WHERE obrid = $obrid";
					$this->executar($sql);
				}else{
	    				
	    			$docid = wf_cadastrarDocumento(TPDID_OBJETO, $docdsc);
	    			$sql = "UPDATE obras2.obras SET docid = $docid WHERE obrid = $obrid";
					$this->executar($sql);
				}
				/*
				* Cria Documento WF - Fim
				*/
	    	
				/*** Recupera as fotos e arquivos do terreno no Pr� Obra e Inclue o ID da nova obra na tabela do pre obra ***/
				$sql = "INSERT INTO obras2.obras_arquivos( obrid, tpaid, arqid, oardata, oardtinclusao )
						SELECT DISTINCT
							$obrid as obrid, 
							21 as tpaid,
							arq.arqid,
							now() as oardata,
							now() as oardtinclusao
						FROM
							public.arquivo arq
						INNER JOIN obras.preobrafotos 	pof ON arq.arqid = pof.arqid
						INNER JOIN obras.preobra 		pre ON pre.preid = pof.preid
						WHERE
							pre.preid = $preid
							AND (substring(arqtipo,1,5) = 'image');
							
						INSERT INTO obras2.obras_arquivos( obrid, tpaid, arqid, oardata, oardtinclusao )
						SELECT DISTINCT
	    					$obrid as obrid,
							21 as tpaid,
							arq.arqid,
							NOW() as oardata,
							NOW() as oardtinclusao
						FROM
							obras.preobraanexo p
						INNER JOIN public.arquivo arq ON arq.arqid = p.arqid
						WHERE
							p.preid = $preid; 
								
						UPDATE obras.preobra SET obrid = $obrid WHERE preid = $preid;";
	    	
				$this->executar($sql);
	    		$this->commit();
	    				
			}else{
				$sql = "SELECT obrid FROM obras.preobra WHERE preid = ".$preid." AND obrid IS NOT NULL";
	    		$obrid = $this->pegaUm($sql);
	    				
	    		$sql = "UPDATE obras2.obras SET obrstatus = 'A' WHERE obrid = $obrid";
	    		$this->executar($sql);
	    		$this->commit();
	    	}
	    	
			return $obrid;
	    	/*** FIM - Importa��o dos dados para o sistema de Obras - FIM ***/
		}
		
		public function criarBkp(){
			
			// criar novo preid (replicar obras.preobra)
			$sql = "INSERT INTO obras.preobra(
			            docid, presistema, preidsistema, ptoid, preobservacao,
			            prelogradouro, precomplemento, estuf, muncod, precep, prelatitude,
			            prelongitude, predtinclusao, prebairro, preano, qrpid, predescricao,
			            prenumero, pretipofundacao, prestatus, entcodent, preprioridade,
			            terid, resid, prevalorobra, tooid, muncodpar, estufpar, premcmv,
			            preidpai, predatareformulacao, preusucpfreformulacao)
	            	(SELECT 
						NULL, presistema, preidsistema, ptoid, preobservacao,
			            prelogradouro, precomplemento, estuf, muncod, precep, prelatitude,
			            prelongitude, predtinclusao, prebairro, preano, NULL, predescricao,
			            prenumero, pretipofundacao, prestatus, entcodent, preprioridade,
			            terid, resid, prevalorobra, tooid, muncodpar, estufpar, premcmv,
			            ".$this->arAtributos['preid'].", now(), '{$_SESSION['usucpf']}' 
					FROM obras.preobra WHERE preid=".$this->arAtributos['preid'].") 
					RETURNING preid";
		
			$novopreid = $this->pegaUm($sql);
		
			// buscar o qrpid da preobraanalise antiga
			$sql = "SELECT qrpid FROM obras.preobraanalise WHERE preid='".$this->arAtributos['preid']."'";
			$antigoqrpid = $this->pegaUm($sql);
		
			// buscar o qrpid da preobra2 antiga
			$sql = "SELECT qrpid FROM obras.preobra WHERE preid='".$this->arAtributos['preid']."'";
			$antigoqrpid2 = $this->pegaUm($sql);
		
			// criar novo qrpid (replicar questionario.questionarioresposta)
			if( $antigoqrpid ){
				$sql = "INSERT INTO questionario.questionarioresposta(queid, qrptitulo, qrpdata)
		            	(SELECT queid, 'OBRAS ($novopreid)', qrpdata FROM questionario.questionarioresposta WHERE qrpid = '$antigoqrpid' ) 
						RETURNING qrpid";
				$novoqrpid = $this->pegaUm($sql);
			}
		
			// pegando descricao o municipio
			$sql = "SELECT m.mundescricao FROM obras.preobra p
			     	LEFT JOIN territorios.municipio m ON m.muncod = p.muncod
			     	WHERE preid='".$this->arAtributos['preid']."'";
			$mundescricao = $this->pegaUm( $sql );
		
			// criar novo qrpid (replicar questionario.questionarioresposta)
			$sql = "INSERT INTO questionario.questionarioresposta(queid, qrptitulo, qrpdata)
	            	(SELECT queid, 'OBRAS (".$novopreid." - ".str_replace("'","''",$mundescricao).")', qrpdata FROM questionario.questionarioresposta WHERE qrpid='".$antigoqrpid2."') RETURNING qrpid";
			$novoqrpid2 = $this->pegaUm($sql);
		
			$sql = "UPDATE obras.preobra SET qrpid='".$novoqrpid2."' WHERE preid='".$novopreid."'";
			$this->executar( $sql );
		
			// criar novo panid (replicar)
			$sql = "INSERT INTO obras.preobraanalise(preid, poadataanalise, poastatus, poausucpfinclusao, qrpid, poaindeferido,poajustificativa)
	            	(SELECT '".$novopreid."', poadataanalise, poastatus, poausucpfinclusao, ".(($novoqrpid)?"'".$novoqrpid."'":"NULL").", poaindeferido, poajustificativa 
	            	FROM obras.preobraanalise WHERE preid='".$this->arAtributos['preid']."')";
			$this->executar($sql);
		
			if( $antigoqrpid ){
				$sql = "INSERT INTO questionario.resposta(perid, qrpid, usucpf, itpid, resdsc)
		            	(SELECT perid, '$novoqrpid', usucpf, itpid, resdsc FROM questionario.resposta WHERE qrpid = '$antigoqrpid' )";
				$this->executar($sql);
			}
		
			if( $antigoqrpid2 ){
				$sql = "INSERT INTO questionario.resposta(perid, qrpid, usucpf, itpid, resdsc)
		            	(SELECT perid, '".$novoqrpid2."', usucpf, itpid, resdsc FROM questionario.resposta WHERE qrpid='".$antigoqrpid2."')";
				$this->executar($sql);
			}
		
			$sql = "INSERT INTO obras.preobrafotos(
	            pofdescricao, preid, arqid)
	            (SELECT pofdescricao, '".$novopreid."', arqid FROM obras.preobrafotos WHERE preid='".$this->arAtributos['preid']."')";
			$this->executar($sql);
		
			$sql = "INSERT INTO obras.preobraanexo(preid, poadescricao, arqid, podid, datainclusao, usucpf,poasituacao)
		            (SELECT '".$novopreid."', poadescricao, arqid, podid, datainclusao, usucpf,poasituacao FROM obras.preobraanexo WHERE preid='".$this->arAtributos['preid']."')";
			$this->executar($sql);
		
			$sql = "INSERT INTO obras.preobradocumentosfnde(preid, pdfdescricao, arqid, pdfdatainclusao, pdfstatus)
		            (SELECT '".$novopreid."', pdfdescricao, arqid, pdfdatainclusao, pdfstatus
					FROM obras.preobradocumentosfnde WHERE preid='".$this->arAtributos['preid']."')";
			$this->executar($sql);
		
			$sql = "INSERT INTO obras.precronograma( itcid, preid, prcmes, prcquinzena )
					( SELECT itcid, $novopreid, prcmes, prcquinzena FROM obras.precronograma WHERE preid = ".$this->arAtributos['preid']." )";
			$this->executar($sql);
		
			$sql = "INSERT INTO obras.preplanilhaorcamentaria(preid, itcid, ppovalorunitario)
            		(SELECT '".$novopreid."', itcid, ppovalorunitario FROM obras.preplanilhaorcamentaria WHERE preid='".$this->arAtributos['preid']."')";
			$this->executar($sql);
		
			return $novopreid;
	}
}
