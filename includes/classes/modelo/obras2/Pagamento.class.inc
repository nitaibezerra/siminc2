<?php
	
class Pagamento extends Modelo{
	
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "obras2.pagamento";	

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "pagid" );

    /**
     * Atributos
     * @var array
     * @access protected
     */    
    protected $arAtributos     = array(
                                        'pagid' => null, 
                                        'sgeid' => null, 
                                        'docid' => null,
                                        'arqid' => null,
                                        'pagstatus' => null,
                                        'pagqtdelaudo' => null,
                                        'pagqtdelaudoatrasado' => null,
                                        'pagnumob' => null,
                                        'pagdatasolicitacaopagamento' => null,
                                        'pagobs' => null,
                                        'paganexoarqid' => null,
                                        'pagvalortotal' => null,
                                        'pagnrnotafiscal' => null,
                                       );
	public function carregarSupervisaoPorEmpresa()
	{
		$sgeid = $_POST['sgeid'];
		if($sgeid){
			$sql = "select 
						'<input type=\"checkbox\" name=\"chk_sueid[]\" value=\"' || sue.sueid || '\" />' as acao,
                        o.obrid,
                        emp.empdsc,
                        mr.mesdsc,
                        mi.micdsc,
                        m.estuf,
                        m.mundescricao,

						to_char(sosdtcadastro ,'DD/MM/YYYY') as sosdtcadastro,
						to_char(sosdtinicio,'DD/MM/YYYY') as sosdtinicio,
						to_char(sosdttermino,'DD/MM/YYYY') as sosdttermino,
						sosnum,
						esd2.esddsc,
						ent.entnome,
						sue.suecargovistoriador,
						to_char(suedtsupervisao,'DD/MM/YYYY') as suedtsupervisao
					from
						obras2.supervisaoempresa sue

                    inner join obras2.empreendimento emp on emp.empid = sue.empid
                            
                    inner join entidade.endereco ed ON ed.endid = emp.endid AND ed.endstatus = 'A'

                    inner join territorios.municipio m ON m.muncod = ed.muncod 

                    inner join territorios.mesoregiao mr ON mr.mescod = m.mescod

                    inner join territorios.microregiao mi ON mi.miccod = m.miccod
                        
					inner join entidade.entidade ent ON ent.entid = sue.entidvistoriador
					
					inner join workflow.documento doc ON doc.docid = sue.docid
					
					inner join workflow.estadodocumento esd ON esd.esdid = doc.esdid
					
					inner join obras2.supervisao_os sos ON sos.sosid = sue.sosid
					
					inner join workflow.documento doc2 ON doc2.docid = sos.docid
					
					inner join workflow.estadodocumento esd2 ON esd2.esdid = doc2.esdid

                    inner join obras2.obras o ON o.empid = emp.empid AND o.obridpai IS NULL AND o.obrstatus = 'A'
					where
						suestatus = 'A'
						and esd.esdid = ".WF_ESDID_LAUDO_SUPERVISAO_HOMOLOGADO."
						and sos.sgeid = $sgeid
						and sue.sueid not in ( select pse.sueid from obras2.pagamento_supervisao_empresa pse WHERE pse.psestatus = 'A' ) ";
			// ver($sql,d);
			$arrCab = array("Ação","ID Obra", "Nome da obra", "Mesorregião","Microregião","UF","Município", "Data de Cadastro da OS","Data de Início da OS","Data de Término da OS","Num. OS","Situação da OS","Vistoriador da Supervisão","Cargo do Vistoriador","Data da Supervisão");
			
            $this->monta_lista_simples($sql,$arrCab,1000,10,"N","","N",true);
		}else{
			echo "Empresa não vinculada.";
		}
	}
	
	public function carregarSupervisaoPorPagamento($pagid,$sgeid,$somente_leitura = false)
	{
		if($pagid){
			
			//Carrega as supervisões já inseridas
			$sql = "select pse.sueid from obras2.pagamento_supervisao_empresa pse where pagid = $pagid and pse.psestatus = 'A'";
			$arrSueid = $this->carregarColuna($sql);
			$arrSueid = $arrSueid ? $arrSueid : array();
			
			// Busca estado documento para verificar se mostra tudo ou não
			$sql = "
				select esd.esdid
				from obras2.pagamento_supervisao_empresa pse
				inner join obras2.supervisaoempresa sue on sue.sueid = pse.sueid
				inner join entidade.entidade ent ON ent.entid = sue.entidvistoriador
				inner join workflow.documento doc ON doc.docid = sue.docid
				inner join workflow.estadodocumento esd ON esd.esdid = doc.esdid
				inner join obras2.supervisao_os sos ON sos.sosid = sue.sosid
				where suestatus = 'A'
					and sos.sgeid = $sgeid
					and sue.sueid not in ( select pse.sueid from obras2.pagamento_supervisao_empresa pse where pse.pagid != $pagid and pse.psestatus = 'A') AND pse.psestatus = 'A'";
			$estadoDocumento = $this->pegaUm( $sql );

			//Carrega as supervisoes disponíveis
			$sql = "
				select
				        sue.sueid,
						obr.obrid,
						e.entnumcpfcnpj,
						emp.empdsc,
                        mr.mesdsc,
                        mi.micdsc,
                        m.estuf,
                        m.mundescricao,
						to_char(sosdtcadastro ,'DD/MM/YYYY') as sosdtcadastro,
						to_char(sosdtinicio,'DD/MM/YYYY') as sosdtinicio,
						to_char(sosdttermino,'DD/MM/YYYY') as sosdttermino,
						sosnum,
						esd2.esddsc,
						ent.entnome,
						sue.suecargovistoriador,
						to_char(suedtsupervisao,'DD/MM/YYYY') as suedtsupervisao

					from obras2.pagamento_supervisao_empresa pse

					inner join obras2.supervisaoempresa sue on sue.sueid = pse.sueid

					-- busca do empreendimento
					inner join obras2.empreendimento emp on emp.empid = sue.empid AND emp.empidpai IS NULL
					inner join obras2.obras obr on emp.empid = obr.empid AND obr.obridpai IS NULL AND obr.obrstatus = 'A'
                    inner join entidade.endereco ed ON ed.endid = emp.endid AND ed.endstatus = 'A'
                    inner join territorios.municipio m ON m.muncod = ed.muncod 
                    inner join territorios.mesoregiao mr ON mr.mescod = m.mescod
                    inner join territorios.microregiao mi ON mi.miccod = m.miccod
                    -- 

                    -- busca documento
					inner join entidade.entidade ent ON ent.entid = sue.entidvistoriador
					inner join workflow.documento doc ON doc.docid = sue.docid
					inner join workflow.estadodocumento esd ON esd.esdid = doc.esdid
					-- 

					inner join obras2.supervisao_os sos ON sos.sosid = sue.sosid
					LEFT JOIN obras2.supervisao_grupo_empresa gp ON gp.sgeid = sos.sgeid AND gp.sgestatus != 'I'
					LEFT JOIN entidade.entidade e ON e.entid = gp.entid	
					inner join workflow.documento doc2 ON doc2.docid = sos.docid
					inner join workflow.estadodocumento esd2 ON esd2.esdid = doc2.esdid

					where
						suestatus = 'A'
						and pse.psestatus = 'A'
						--and esd.esdid = ".WF_ESDID_LAUDO_SUPERVISAO_HOMOLOGADO."
						and sos.sgeid = $sgeid
						and sue.sueid not in ( select pse.sueid from obras2.pagamento_supervisao_empresa pse where pse.pagid != $pagid and pse.psestatus = 'A')";

			// se tiver solicitado pagamento não busca nenhuma os além do previsto para pagamento
			if( $estadoDocumento != WF_ESDID_LAUDO_SUPERVISAO_PAGAMENTO_SOLICITADO )
				$sql = "
						select
							sue.sueid,
							obr.obrid,
							ent.entnumcpfcnpj,
							emp.empdsc,
	                        mr.mesdsc,
	                        mi.micdsc,
	                        m.estuf,
	                        m.mundescricao,
							to_char(sosdtcadastro ,'DD/MM/YYYY') as sosdtcadastro,
							to_char(sosdtinicio,'DD/MM/YYYY') as sosdtinicio,
							to_char(sosdttermino,'DD/MM/YYYY') as sosdttermino,
							sosnum,
							esd2.esddsc,
							ent.entnome,
							sue.suecargovistoriador,
							to_char(suedtsupervisao,'DD/MM/YYYY') as suedtsupervisao

						from obras2.supervisaoempresa sue

	                    inner join obras2.empreendimento emp on emp.empid = sue.empid AND emp.empidpai IS NULL
	                    inner join obras2.obras obr ON emp.empid = obr.empid AND obr.obridpai IS NULL AND obr.obrstatus = 'A'

	                    inner join entidade.endereco ed ON ed.endid = emp.endid AND ed.endstatus = 'A'

	                    inner join territorios.municipio m ON m.muncod = ed.muncod 

	                    inner join territorios.mesoregiao mr ON mr.mescod = m.mescod

	                    inner join territorios.microregiao mi ON mi.miccod = m.miccod
	                        
						inner join entidade.entidade ent ON ent.entid = sue.entidvistoriador
						
						inner join workflow.documento doc ON doc.docid = sue.docid
						
						inner join workflow.estadodocumento esd ON esd.esdid = doc.esdid
						
						inner join obras2.supervisao_os sos ON sos.sosid = sue.sosid
						
						inner join workflow.documento doc2 ON doc2.docid = sos.docid
						
						inner join workflow.estadodocumento esd2 ON esd2.esdid = doc2.esdid

						where
							suestatus = 'A'
							and esd.esdid = ".WF_ESDID_LAUDO_SUPERVISAO_HOMOLOGADO."
							and sos.sgeid = $sgeid
							and sue.sueid not in ( select pse.sueid from obras2.pagamento_supervisao_empresa pse where pse.psestatus = 'A')

						UNION

						".$sql;
//			 ver($sql,d);


			$arrCab = array("Ação","ID Obra", "Nome da Obra", "Mesorregião", "Microregião", "Estado", "Município", "Data de Cadastro da OS","Data de Início da OS","Data de Término da OS","Num. OS","Situação da OS","Vistoriador da Supervisão","Cargo do Vistoriador","Data da Supervisão");
			$arrDados = $this->carregar($sql);
			// ver($arrSueid,$arrDados,d);
			$arrDados = $arrDados ? $arrDados : array();
			foreach($arrDados as $chave => $dado){
				if(in_array($dado['sueid'],$arrSueid)){
					$checked = " checked=\"checked\" ";
				}else{
					$checked = " ";
				}
				$arrResultado[$chave]['acao'] = "<input type=\"checkbox\" ".($somente_leitura ? "disabled=\"disabled\"" : "")." $checked name=\"chk_sueid[]\" value=\"{$dado['sueid']}\" />";
				// $arrResultado[$chave]['entnomeempresa'] = formatar_cnpj( $dado['entnumcpfcnpj'] ). ' - ' . $dado['entnomeempresa'];
				$arrResultado[$chave]['obrid'] = $dado['obrid'];
				$arrResultado[$chave]['empdsc'] = $dado['empdsc'];
				$arrResultado[$chave]['mesdsc'] = $dado['mesdsc'];
				$arrResultado[$chave]['micdsc'] = $dado['micdsc'];
				$arrResultado[$chave]['estuf'] = $dado['estuf'];
				$arrResultado[$chave]['mundescricao'] = $dado['mundescricao'];
				$arrResultado[$chave]['sosdtcadastro'] = $dado['sosdtcadastro'];
				$arrResultado[$chave]['sosdtinicio'] = $dado['sosdtinicio'];
				$arrResultado[$chave]['sosdttermino'] = $dado['sosdttermino'];
				$arrResultado[$chave]['sosnum'] = $dado['sosnum'];
				$arrResultado[$chave]['esddsc'] = $dado['esddsc'];
				$arrResultado[$chave]['entnome'] = $dado['entnome'];
				$arrResultado[$chave]['suecargovistoriador'] = $dado['suecargovistoriador'];
				$arrResultado[$chave]['suedtsupervisao'] = $dado['suedtsupervisao'];
			}
			$arrResultado = $arrResultado ? $arrResultado : array();
			$this->monta_lista_simples($arrResultado,$arrCab,1000,10,"N","","N",true);
		}else{
			echo "Não existem supervisões vinculadas.";
		}
	}
	
	public function salvarPagamento()
	{
		$_REQUEST['sgeid'] = $_REQUEST['sgeid_disable'] ? $_REQUEST['sgeid_disable'] : $_REQUEST['sgeid'];
		$arrDados['pagid'] = $_REQUEST['pagid'];
		$arrDados['sgeid'] = $_REQUEST['sgeid'];
		$arrDados['docid'] = $_REQUEST['docid'] ? $_REQUEST['docid'] : $this->retornaDocumentoWorkFlow($arrDados['pagid']);
		$arrDados['arqid'] = $_REQUEST['arquivo'] ? $_REQUEST['arquivo'] : $this->salvarArquivoPagamento();
		$arrDados['pagnrnotafiscal'] = $_REQUEST['pagnrnotafiscal'];
//		 ver($arrDados,d);
		$this->popularDadosObjeto($arrDados);
		$pagid = $this->salvar();
		if($pagid){
			$_SESSION['obras2']['pagamento']['alert'] = "Operação realizada com sucesso.";
			$sql = "delete from obras2.pagamento_supervisao_empresa where pagid = $pagid";
			$this->executar($sql);
			$this->commit();
			if($_REQUEST['chk_sueid']){
                            foreach($_REQUEST['chk_sueid'] as $sueid){
                                $pag = new Pagamento_Supervisao_Empresa();
                                $arrPag = array(
                                                "pagid" => $pagid,
                                                "sueid" => $sueid
                                               );
                                $pag->popularDadosObjeto($arrPag);
                                $pag->salvar();
                                $pag->commit();
                            }
			}
		}else{
			$_SESSION['obras2']['pagamento']['alert'] = "Não foi possível realizar a operação.";
		}
		header("Location: obras2.php?modulo=principal/listaPagamento&acao=A");
		exit;
	}
	
	public function retornaDocumentoWorkFlow($pagid = null)
	{
		if(!$pagid){
			require_once APPRAIZ . 'includes/workflow.php';
			// descrição do documento
			$docdsc = "Fluxo de Pagamento do módulo Obras II ";
			// cria documento do WORKFLOW
			$docid = wf_cadastrarDocumento( WF_TPDID_PAGAMENTO_SUPERVISAO, $docdsc );
			return $docid;
		}else{
			$this->carregarPorId($pagid);
			return $this->docid;
		}
	}
	
	public function salvarArquivoPagamento($pagid = null,$campo = "arqid",$descricao = "Nota Fiscal")
	{
		
		if(!$pagid){
			require_once APPRAIZ . 'includes/classes/fileSimec.class.inc';
			$file = new FilesSimec(null,null,"obras2");
			$file->setUpload($descricao,null,false,false);
			return $file->getIdArquivo();
			dbg($file);
		}else{
			$this->carregarPorId($pagid);
			return $this->$campo;
		}
	}
	
	public function salvarAnexoPagamento($pagid,$campo,$descricao)
	{
		
		if($pagid){
			require_once APPRAIZ . 'includes/classes/fileSimec.class.inc';
			$file = new FilesSimec(null,null,"obras2");
			$file->setUpload($descricao,null,false,false);
			return $file->getIdArquivo();
		}else{
			return false;
		}
	}
	
	public function excluirNotaFiscal()
    {
		$arqid = $_POST['arqid'];
		$campo = $_POST['campo'] ? $_POST['campo'] : "arqid";
		//Fazer exclusão do arquivo físico?
		$sql = "update public.arquivo set arqstatus = 'I' where arqid = $arqid;
				update $this->stNomeTabela set $campo = null where $campo = $arqid";
		$this->executar($sql);
		$this->commit();
	}

	public function listaSql(Array $param = array()){
		
		$where = array();
		
		if ( $param['sgrid'] ){
				$where[] = " gp.sgrid = '".$param['sgrid']."'";
		}
		
		if ( $param['sgeid'] ){
				$where[] = " gp.sgeid = '".$param['sgeid']."'";
		}else if ( $param['sgeid_disable'] ){
				$where[] = " gp.sgeid = '".$param['sgeid_disable']."'";
		}
		
		if ( $param['listapagamentoestuf'] ){
			$param['listapagamentoestuf']   = (array) $param['listapagamentoestuf'];
			$where[]  = " tes.estuf IN('" . implode("', '", $param['listapagamentoestuf']) . "')";
		}
		
		if ( $param['listapagamentomescod'] ){
			$where[] = " mes.mescod = '".$param['listapagamentomescod']."'";
		}
		
		if ( $param['listapagamentomiccod'] ){
			$where[] = " mic.miccod = '".$param['listapagamentomiccod']."'";
		}
		
		if ( $param['listapagamentomuncod'] ){
			$where[] = " m.muncod = '" .$param['listapagamentomuncod']."'";
		}
		if ( $param['entid'] ){
			$where[] = " e.entid = '" .$param['entid']."'";
		}
		
		if ( $param['pagidsituaco'] ){
			$where[] = " esd.esdid = " .$param['pagidsituaco'];
		}
                
		if ( $param['pagnrnotafiscal'] ){
			$where[] = " pg.pagnrnotafiscal = " .$param['pagnrnotafiscal'];
		}
		if ( $param['pagnrnotafiscal'] ){
			$where[] = " pg.pagnrnotafiscal = " .$param['pagnrnotafiscal'];
		}
		
		if ( $param['sosnum'] ){
			$where[] = " array_to_string(
                            array(
                                SELECT DISTINCT sos.sosnum FROM obras2.supervisao_os sos
                                LEFT JOIN obras2.supervisaoempresa sue ON sue.sosid = sos.sosid
                                LEFT JOIN obras2.pagamento_supervisao_empresa pse ON pse.sueid = sue.sueid AND pse.psestatus = 'A'
                                WHERE pse.pagid = pg.pagid
                            ), ', ')::text like('%"  .$param['sosnum']."%')";
		}
		
		if( possui_perfil(array(PFLCOD_SUPER_USUARIO, PFLCOD_GESTOR_MEC, PFLCOD_SUPERVISOR_UNIDADE, PFLCOD_SUPERVISOR_MEC)) == false ){
			$usuarioResp = new UsuarioResponsabilidade();
			$arrEmpresa = $usuarioResp->pegaSgeidEmpresaPermitida( $_SESSION['usucpf'] );
			$where[]  = " gp.sgeid IN('" . implode("', '", $arrEmpresa) . "')";
		}		
		
		$and = (count($where) ? " AND " . implode(' AND ',$where) : "");
		
		$tpdid_em_cadastramento = WF_ESDID_PAGAMENTO_SUPERVISAO_EM_CADASTRAMENTO;
		
		$arrPerfil = pegaPerfilGeral();
		
		//Devem ser colocados aqui os perfis que podem SOLICTAR pagamento (situação Enviado para FNDE) 
		if(in_array(PFLCOD_SUPER_USUARIO,$arrPerfil)){
			$img_solicitar_pagameno = "' ||  (case when esd.esdid = ".WF_ESDID_PAGAMENTO_SUPERVISAO_ENVIADO_FNDE."
                                                            then  
                                                                    '<img
                                                                            align=\"absmiddle\"
                                                                            src=\"/imagens/money.gif\" border=\"0\" /
                                                                            style=\"cursor: pointer; margin-left: 3px;\"
                                                                            onclick=\"javascript:abrirPagamento(' || pg.pagid || ');\"
                                                                            title=\'Solicitar Pagamento\'>'
                                                            else ''
                                                          end) || '";
		}
		
		//Devem ser colocados aqui os perfis que podem REALIZAR pagamento (situação Pagamento Solicitado)
		if(in_array(PFLCOD_SUPER_USUARIO,$arrPerfil)){
			$img_realizar_pagameno = "' ||  (case when esd.esdid = ".WF_ESDID_PAGAMENTO_SUPERVISAO_PAGAMENTO_SOLICITADO."
                                                            then  
                                                                    '<img
                                                                        align=\"absmiddle\"
                                                                        src=\"/imagens/money.gif\" border=\"0\" /
                                                                        style=\"cursor: pointer; margin-left: 3px;\"
                                                                        onclick=\"javascript:realizarPagamento(' || pg.pagid || ');\"
                                                                        title=\'Verificar Pagamento\'>'
                                                            else ''
                                                        end) || '";
		}

        $acao = <<<EOD
		case when esd.esdid = $tpdid_em_cadastramento
							then
                                                            '<center>
                                                                    <img
                                                                            align=\"absmiddle\"
                                                                            src=\"/imagens/clipe1.gif\" border=\"0\" /
                                                                            style=\"cursor: pointer; margin-left: 3px;\"
                                                                            onclick=\"javascript:downloadNotaFiscal(' || arq.arqid || ');\"
                                                                            title= '||arq.arqnome||'>
                                                                    <img
                                                                            align=\"absmiddle\"
                                                                            src=\"/imagens/alterar.gif\" border=\"0\" /
                                                                            style=\"cursor: pointer; margin-left: 3px;\"
                                                                            onclick=\"javascript:abrirPagamento(' || pg.pagid || ');\"
                                                                            />
                                                                    <img
                                                                            align=\"absmiddle\"
                                                                            src=\"/imagens/excluir.gif\" border=\"0\" /
                                                                            style=\"cursor: pointer; margin-left: 3px;\"
                                                                            onclick=\"javascript:excluirPagamento(' || pg.pagid || ');\"
                                                                            />
                                                                    <img
                                                                            align=\"absmiddle\"
                                                                            src=\"/imagens/fluxodoc.gif\" border=\"0\" /
                                                                            style=\"cursor: pointer; margin-left: 3px;\"
                                                                            onclick=\"wf_exibirHistorico( '||pg.docid||' );;\"
                                                                            title=\'Histórico do Pagamento\' />
                                                             </center>'
							else
                                                            '<center>
                                                                    <img
                                                                            align=\"absmiddle\"
                                                                            src=\"/imagens/clipe1.gif\" border=\"0\" /
                                                                            style=\"cursor: pointer; margin-left: 3px;\"
                                                                            onclick=\"javascript:downloadNotaFiscal(' || arq.arqid || ');\"
                                                                            title= '||arq.arqnome||'>
                                                                    <img
                                                                            align=\"absmiddle\"
                                                                            src=\"/imagens/consultar.gif\" border=\"0\" /
                                                                            style=\"cursor: pointer; margin-left: 3px;\"
                                                                            onclick=\"javascript:abrirPagamento(' || pg.pagid || ');\"
                                                                            />
                                                                    <img
                                                                            align=\"absmiddle\"
                                                                            src=\"/imagens/fluxodoc.gif\" border=\"0\" /
                                                                            style=\"cursor: pointer; margin-left: 3px;\"
                                                                            onclick=\"wf_exibirHistorico( '||pg.docid||' );;\"
                                                                            title=\'Histórico do Pagamento\' />
                                                                    $img_solicitar_pagameno
                                                                    $img_realizar_pagameno
                                                             </center>'
						end as acao,
EOD;

        if($param['xls'])
            $acao = '';

		$sql = <<<EOD
					SELECT
						$acao
						to_char(e.entnumcpfcnpj::int8,'00"."000"."000"/"0000"-"00') || ' - ' || e.entnome AS descricao,
						tes.estdescricao,
						m.mundescricao,
						esd.esddsc,
						' <center> ' || array_to_string(
                            array(
                                SELECT DISTINCT sos.sosnum FROM obras2.supervisao_os sos
                                LEFT JOIN obras2.supervisaoempresa sue ON sue.sosid = sos.sosid
                                LEFT JOIN obras2.pagamento_supervisao_empresa pse ON pse.sueid = sue.sueid AND pse.psestatus = 'A'
                                WHERE pse.pagid = pg.pagid
                            ), ', '
                        ) || ' </center> ' sosnum,
                        pg.pagnrnotafiscal,
                        coalesce(pagnumob,'N/A') as pagnumob,
                        coalesce(to_char(pagdatasolicitacaopagamento,'DD/MM/YYYY'),'N/A') as pagdatasolicitacaopagamento,
                        coalesce(pagvalortotal::text,'N/A') as valor,

                        (SELECT
                            MAX(coalesce(to_char(hstid.htddata,'DD/MM/YYYY'),'N/A'))
                        FROM
                            workflow.historicodocumento hstid WHERE hstid.aedid = 1778 and hstid.docid = pg.docid
                        )  as pagdataenviadofnde
					FROM
						obras2.pagamento pg
					LEFT JOIN 
						obras2.supervisao_grupo_empresa gp ON gp.sgeid = pg.sgeid AND gp.sgestatus != 'I'
					LEFT JOIN
						arquivo arq ON arq.arqid = pg.arqid
					LEFT JOIN
						entidade.entidade e ON gp.entid = e.entid
					LEFT JOIN 
						workflow.documento doc ON doc.docid = pg.docid
					LEFT JOIN 
						workflow.estadodocumento esd ON doc.esdid = esd.esdid
					LEFT JOIN 
						entidade.endereco edr ON edr.entid = e.entid
					LEFT JOIN 
						territorios.estado tes ON tes.estuf = edr.estuf
					LEFT JOIN 
						territorios.municipio m ON m.muncod = edr.muncod
					LEFT JOIN 
						territorios.mesoregiao mes ON mes.mescod = m.mescod
					LEFT JOIN 
						territorios.microregiao mic ON mic.miccod = m.miccod
					WHERE 
						pagstatus = 'A'
					$and
EOD;
//                VER($sql);
        return $sql;
	}
	
	public function verificaSupervisao($pagid = null)
	{
		$pagid = $pagid ? $pagid : $this->pagid;
		if($pagid){
			$sql = "select count(sueid) from obras2.pagamento_supervisao_empresa where psestatus = 'A' AND pagid = ".$this->pagid;
			return $this->pegaUm($sql);
		}else{
			return false;
		}
	
	}
	
	public function excluirPagamento()
	{
		$pagid = $_POST['pagid'];

		$sql = "delete from obras2.pagamento_supervisao_empresa where pagid = $pagid;
				update obras2.pagamento set pagstatus = 'I' where pagid = $pagid;";
		if($this->executar($sql)){
			$this->commit();
			$_SESSION['obras2']['pagamento']['alert'] = "Operação realizada com sucesso.";
		}else{
			$_SESSION['obras2']['pagamento']['alert'] = "Não foi possível realizar a operação.";	
		}
		header("Location: obras2.php?modulo=principal/listaPagamento&acao=A");
		die;
	}
	
	public function listaSupervisoesPagamento($pagid = null)
	{
		$pagid = $pagid ? $pagid : $this->pagid;
		if($pagid){
			
			$arrGlosadas = $this->recuperarTodasSupervisoesGlosadasPorPagamento($pagid);
			foreach($arrGlosadas as $dado){
				$arrSup[$dado['sueid']] = $dado['pgsglosa'];
			}
			$arrSup = $arrSup ? $arrSup : array(); 

			//Carrega as supervioes
			$sql = "select 
						sue.sueid,
						obrid,
						to_char(sosdtcadastro ,'DD/MM/YYYY') as sosdtcadastro,
						to_char(sosdtinicio,'DD/MM/YYYY') as sosdtinicio,
						to_char(sosdttermino,'DD/MM/YYYY') as sosdttermino,
						sosnum,
						esd2.esddsc,
						sgeid,
						sos.sosid,
						sue.docid,
						'' as data_homologacao,
						'' as dias_execucao,
						'' as dias_fora_prazo
					from
						obras2.supervisaoempresa sue
					inner join
						entidade.entidade ent ON ent.entid = sue.entidvistoriador
					inner join
						obras2.obras obr ON obr.empid = sue.empid and obr.obrstatus = 'A' and obr.obridpai IS NULL
					inner join
						workflow.documento doc ON doc.docid = sue.docid
					inner join
						workflow.estadodocumento esd ON esd.esdid = doc.esdid
					inner join
						obras2.supervisao_os sos ON sos.sosid = sue.sosid
					inner join
						workflow.documento doc2 ON doc2.docid = sos.docid
					inner join
						workflow.estadodocumento esd2 ON esd2.esdid = doc2.esdid
					where
						suestatus = 'A'
					and
						esd.esdid = ".WF_ESDID_LAUDO_SUPERVISAO_PAGAMENTO_SOLICITADO."
					and
						sue.sueid in ( select pse.sueid from obras2.pagamento_supervisao_empresa pse where pse.pagid = $pagid AND pse.psestatus = 'A')-- order by obr.obrid
						";

            //ver($sql,d);

			$arrCab = array("Glosa","Id da obra","Data de Cadastro da OS","Data de Início da OS","Data de Término da OS","Num. OS","Situação da OS","Data de Homologação","Dias em Execução","Dias fora do Prazo","Valor do Laudo");
			$arrDados = $this->carregar($sql);
			$arrDados = $arrDados ? $arrDados : array();
			foreach($arrDados as $chave => $dado){

                $arrData = verificaDataAcaoWorkFlow($dado['docid'],WF_AEDID_LAUDO_SUPERVISAO_HOMOLOGAR);

                if($arrData['data']){
                    $os = new Supervisao_Os( $dado['sosid'] );
                    $dias_fora_prazo = retornaDiasEntreDatas($os->sosdttermino,$arrData['data']);
                }else{
                    $os = new Supervisao_Os( $dado['sosid'] );
                    $dias_fora_prazo = retornaDiasEntreDatas($os->sosdttermino,date("Y-m-d"));
                }

                if($dias_fora_prazo > 0 ){
                    $selected_sim = "checked='checked'";
                    $selected_nao = "";
                }
                else{
                    $selected_nao = "checked='checked'";
                    $selected_sim = "";
                }

				if($arrSup[$dado['sueid']] == "t"){
					$selected_sim = "checked='checked'";
				}else{
				//	$selected_sim = "";
				}
				if($arrSup[$dado['sueid']] == "f"){
					$selected_nao = "checked='checked'";
				}else{
				//	$selected_nao = "";
				}
				$arrResultado[$chave]['acao'] = "<input type=\"hidden\" value=\"{$dado['sueid']}\" name=\"hdn_sueid[{$dado['sueid']}]\" /><input onclick=\"glosaSupervisao('{$dado['sueid']}')\" type=\"radio\" ".($somente_leitura ? "disabled=\"disabled\"" : "")." $selected_sim name=\"rdn_sueid_{$dado['sueid']}\" value=\"sim\" />Sim <input onclick=\"glosaSupervisao('{$dado['sueid']}')\" type=\"radio\" ".($somente_leitura ? "disabled=\"disabled\"" : "")." $selected_nao name=\"rdn_sueid_{$dado['sueid']}\" value=\"nao\" />Não";
                $arrResultado[$chave]['obrid'] = '<span style="text-align: center;display: block">'.$dado['obrid'].'</span>';
				$arrResultado[$chave]['sosdtcadastro'] = '<span style="text-align: center;display: block">'.$dado['sosdtcadastro'].'</span>';

				$arrResultado[$chave]['sosdtinicio'] = '<span style="text-align: center;display: block">'.$dado['sosdtinicio'].'</span>';
				$arrResultado[$chave]['sosdttermino'] = '<span style="text-align: center;display: block">'.$dado['sosdttermino'].'</span>';
				$arrResultado[$chave]['sosnum'] = '<span style="text-align: center;display: block">'.$dado['sosnum'].'</span>';
				$arrResultado[$chave]['esddsc'] = '<span style="text-align: center;display: block">'.$dado['esddsc'].'</span>';

				$arrResultado[$chave]['homologacao'] = '<span style="text-align: center;display: block">'.formata_data($arrData['data']).'</span>';
				$arrResultado[$chave]['dias_execucao'] = '<span style="text-align: center;display: block">'.$this->recuperaDiasExecucaoLaudo($dado['sosid'],$dado['sueid']).'</span>';

				$arrResultado[$chave]['fora_prazo'] = '<span style="text-align: center;display: block">'.$dias_fora_prazo.($dias_fora_prazo > 0 ? "<input type=\"hidden\" name=\"hdn_fora_prazo[{$dado['sueid']}]\" value=\"1\"  />" : "").'</span>';
				if ( $dado['sgeid'] ){
				//	$grupoEmpresa  = new Supervisao_Grupo_Empresa();
				//	$valorUnitario = $grupoEmpresa->pegaValorUnitarioPorSgeid( $dado['sgeid'] );

                   $valorUnitario = $this->recuperaValorDoLaudoComDesconto($dado['sosid']);
				}else{
					$valorUnitario = 0;
				}
				
				$arrResultado[$chave]['valor'] = '<span style="text-align: center;display: block">'.number_format($valorUnitario, 2, ',', '.')."<input type=\"hidden\" name=\"hdn_valor[{$dado['sueid']}]\" value=\"$valorUnitario\"  />".'</span>';
				$arrResultado[$chave]['div'] = "</td></tr><tr style=\"display:".($selected_sim ? "" : "none")."\" id=\"div_glosa_sueid_{$dado['sueid']}\" ><td colspan=10 >".exibeGLosaPorSueid($dado['sueid'],$pagid, $dias_fora_prazo,$valorUnitario);
			}
			$arrResultado = $arrResultado ? $arrResultado : array();
			$this->monta_lista_simples($arrResultado,$arrCab,1000,10,"N","","N",true);
		}else{
			echo "Não existem supervisões vinculadas.";
		}
	}
	
	public function recuperaDiasExecucaoLaudo($sosid,$sueid)
	{
		//é a diferenca entre o dia de inicio da O.S que o laudo faz parte e o dia que ele foi homologado no workflow
		
		$os = new Supervisao_Os( $sosid );
		$os->sosdtinicio;
		$data1 = $os->sosdtinicio;
		
		$se = new SupervisaoEmpresa($sueid);
		$arrAcao = verificaDataAcaoWorkFlow($se->docid,WF_AEDID_LAUDO_SUPERVISAO_HOMOLOGAR);
		$data2 = $arrAcao['data'];
		
		return retornaDiasEntreDatas($data1,$data2);
		
	}
	
	public function salvarRealizacaoPagamento()
	{
		$this->carregarPorId($_POST['pagid']);
		$_POST['pagdatasolicitacaopagamento'] = formata_data_sql( $_POST['pagdatasolicitacaopagamento'] );
		$_POST['pagvalortotal'] = $_POST['pagvalortotal'] ? $_POST['pagvalortotal'] : 0;
		if($_FILES['arquivo']['name']){
			$_POST['paganexoarqid'] = $this->salvarAnexoPagamento($_POST['pagid'],"paganexoarqid","Anexo do Pagamento");
		}
		$this->popularDadosObjeto($_POST);
		$this->salvar();
		$this->commit();
		
		$sql = "delete from obras2.pagamento_glosa_supervisao where pagid = ".$_POST['pagid'];
		$this->executar($sql);
		$this->commit();
		
		if($_POST['hdn_sueid']){
			foreach($_POST['hdn_sueid'] as $sueid){
				if($_POST['rdn_sueid_'.$sueid]){
					//Salva a glosa de pagamentos
					$g = new Pagamento_Glosa_Supervisao();
					$arrGlosa['pgsglosa'] = $_POST['rdn_sueid_'.$sueid] == "nao" ? "false" : "true";
					$arrGlosa['pagid'] = $_POST['pagid'];
					$arrGlosa['tgpid'] = $_POST['tgpid_'.$sueid];
					$arrGlosa['sueid'] = $sueid;
					$arrGlosa['usucpf'] = $_SESSION['usucpf'];
					$arrGlosa['pgsobs'] = $_POST['pgsobs_'.$sueid];
					$arrGlosa['pgsvalor'] = $_POST['pgsvalor_'.$sueid] ? str_replace(array(".",","),array("","."),$_POST['pgsvalor_'.$sueid]) : "0";
            //        ver($_POST['pgsglosavalor_'.$sueid],d);
                    $arrGlosa['pgsglosavalor'] = $_POST['pgsglosavalor_'.$sueid] ? str_replace(array(".",","),array("","."),$_POST['pgsglosavalor_'.$sueid]) : "0";

					$g->popularDadosObjeto($arrGlosa);
					if($arrGlosa['pgsglosa'] == "false"){
						$g->tgpid = null;
						$g->pgsobs = null;
						$g->pgsvalor = null;
                        $g->pgsglosavalor = null;
					}
					$g->salvar();
					$g->commit();
				}
			}
		}
		
		
		$_SESSION['obras2']['pagamento']['alert'] = "Operação realizada com sucesso.";
		header("Location: obras2.php?modulo=principal/realizarPagamento&acao=A&pagid={$_POST['pagid']}");
		die;
	}
	
	public function recuperarTodasSupervisoesGlosadasPorPagamento($pagid)
	{
		$sql = "select * from obras2.pagamento_glosa_supervisao where pagid = $pagid";
		$arrDados = $this->carregar($sql);
		return $arrDados ? $arrDados : array();
	}

    public function recuperaValorDoLaudoComDesconto($sosid)
    {
        $sql = "SELECT sos.sosvalortotal/ (SELECT COUNT(*) FROM obras2.supervisao_os_obra soo WHERE soo.sosid = sos.sosid AND soostatus = 'A') as qt FROM obras2.supervisao_os sos WHERE sos.sosid = $sosid";
        $arrDados = $this->pegaUm($sql);
        return $arrDados;
    }

}
