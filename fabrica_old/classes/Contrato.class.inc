<?php
	
class Contrato extends Modelo{
	
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "fabrica.contrato";	

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "ctrid" );
    
    /**
     * Atributos
     * @var array
     * @access protected
     */    
    protected $arAtributos     = array(
										'ctrid'             => null, 
										'ctrnumero'         => null,
										'ctrdtassinatura'   => null, 
    									'ctrdtinicio'       => null, 
									    'ctrdtassinatura'   => null,
    									'ctrdtfim'          => null,
									    'ctrobjeto'         => null, 
									    'ctrqtdpfcontrato'  => null, 
									    'ctrqtdpfutilizado' => null,
    									'ctrstatus'         => null,
    									'entidcontratado'   => null,
    									'gestorcpf'         => null,
    									'entidcogestor'     => null,
    									'ctrcontagem'       => null,
                                        'ctrtipoempresaitem'=>null   
									  );
	
	protected $arrPesquisa = null;
	
	
	public function showListaContratos()
	{
	
	    $sql = "select distinct
                    case when vgcstatus = 'A' then
                    '<center>
                        <img class=\"middle link\" title=\"Alterar Contrato\" alt=\"Alterar Contrato\" onclick=\"alterarContrato(\'' || ctr.ctrid || '\')\" src=\"../imagens/alterar.gif\" />
                        <img class=\"middle link\" title=\"Excluir Contrato\" alt=\"Excluir Contrato\" src=\"../imagens/excluir_01.gif\" /> 
                    </center>' 
                    else
                    '<center>
                        <img class=\"middle link\" title=\"Alterar Contrato\" alt=\"Alterar Contrato\" onclick=\"alterarContrato(\'' || ctr.ctrid || '\')\" src=\"../imagens/alterar.gif\" /> 
                        <img class=\"middle link\" title=\"Excluir Contrato\" alt=\"Excluir Contrato\" onclick=\"excluirContrato(\'' || ctr.ctrid || '\')\" src=\"../imagens/excluir.gif\" />                        
                    </center>' 
                    end as acao,
        	        ctr.ctrid,
        	        ent.entnome,
        			(select usunome from seguranca.usuario where usucpf = ctr.gestorcpf) as gestorcpf,
        			ctrnumero,
        			to_char(vgc.vgcdtassinatura,'DD/MM/YYYY') as vgcdtassinatura,
        			to_char(vgc.vgcdtinicio,'DD/MM/YYYY') as vgcdtinicio,
        			to_char(vgc.vgcdtfim,'DD/MM/YYYY') as vgcdtfim,
        			tscdsc as situacao,
        			ctrobjeto --,vgc.vgcvolumepfcontratado,
        			--vgc.vgcvolumepfutilizado
        	    from
        	        fabrica.contrato ctr
        	        inner join entidade.entidade ent ON ent.entid = ctr.entidcontratado
        	        left join fabrica.vw_vigenciacontrato  vgc 	ON vgc.ctrid = ctr.ctrid and vgc.vgcstatus = 'A'
        	        left  join fabrica.contratosituacao cts ON cts.ctrid = ctr.ctrid and cts.ctsstatus = 'A'
        	        left join fabrica.tiposituacaocontrato tsc ON tsc.tscid = cts.tscid and tsc.tscstatus = 'A'
        	    where ctrstatus = 'A'
        	        ".($this->arrPesquisa ? "and " . implode(" and ", $this->arrPesquisa) : "")."
        	        order by
        	        ctrnumero";
	    //$cabecalho = array("Ações","Código","Empresa","Gestor","Número do Contrato","Data de Assinatura","Data de Início","Data de Término","Situação","Objeto","Volume de Pontos por Função Contratado","Volume de Pontos por Função Utilizado");
	    $cabecalho = array("Ações","Código","Empresa","Gestor","Número do Contrato","Data de Assinatura","Data de Início","Data de Término","Situação","Objeto");
	
	    $this->monta_lista($sql,$cabecalho,100,50,"N","center","N");
	}

	public function showListaContratosTmp()
	{
				
		$sql = "select distinct
					'<center><img class=\"middle link\" title=\"Alterar Contrato\" alt=\"Alterar Contrato\" onclick=\"alterarContrato(\'' || ctr.ctrid || '\')\" src=\"../imagens/alterar.gif\" /> <img class=\"middle link\" title=\"Excluir Contrato\" alt=\"Excluir Contrato\" onclick=\"excluirContrato(\'' || ctr.ctrid || '\')\" src=\"../imagens/excluir.gif\" /></center>' as acao,
					ctr.ctrid,
					(select usunome from seguranca.usuario where usucpf = gestorcpf) as gestorcpf,
					--(select entnome from entidade.entidade where entid = entidcogestor) as entidcogestor,
					ctrnumero,
					to_char(ctrdtassinatura,'DD/MM/YYYY') as ctrdtassinatura,
					to_char(ctrdtinicio,'DD/MM/YYYY') as ctrdtinicio,
					to_char(ctrdtfim,'DD/MM/YYYY') as ctrdtfim,
					tscdsc as situacao,
					ctrobjeto,
					--(CASE WHEN ctrcontagem is true
					--	THEN 'Sim'
					--	ELSE 'Não'
					--END) as respcontagem,
					ctrqtdpfcontrato,
					ctrqtdpfutilizado
                                        --(CASE WHEN ctrtipoempresaitem = 1  THEN 'Empresa do Item 1' 
                                          --    WHEN ctrtipoempresaitem = 2 THEN 'Empresa do Item 2'
                                         --END)as tipoempresa 
				from
					fabrica.contrato ctr
				inner join 
					fabrica.contratosituacao cts ON cts.ctrid = ctr.ctrid
				inner join
					fabrica.tiposituacaocontrato tsc ON tsc.tscid = cts.tscid
				where
					ctrstatus = 'A'
				and
					ctsstatus = 'A'
				and
					tscstatus = 'A'
					".($this->arrPesquisa ? "and " . implode(" and ", $this->arrPesquisa) : "")."
				order by
					ctrnumero";
		$cabecalho = array("Ações","Código","Gestor","Número do Contrato","Data de Assinatura","Data de Início","Data de Término","Situação","Objeto","Volume de Pontos por Função Contratado","Volume de Pontos por Função Utilizado");
		
		$this->monta_lista($sql,$cabecalho,100,50,"N","center","N");
	}
	
	public function recuperaContratosPorEmpresa($entid) {
	
		$sql = sprintf("SELECT 
		                        ctr.ctrid as codigo,
		                        ctr.ctrnumero as descricao
		                FROM
		                        fabrica.contrato ctr
		                INNER JOIN
		                        fabrica.contratosituacao cs
		                        on cs.ctrid=ctr.ctrid and cs.ctsstatus='A'
		                INNER JOIN
		                        fabrica.tiposituacaocontrato tsc
		                        on tsc.tscid=cs.tscid and tsc.tscstatus='A'
						INNER JOIN entidade.entidade ent
					    		on ent.entid = ctr.entidcontratado and ent.entstatus = 'A'
		                        and ctr.ctrstatus = 'A'
						WHERE
							ctr.entidcontratado=%d
		                ORDER BY codigo ASC", $entid);
		
		return $this->carregar($sql);
	}
	
	public function recuperaVigenciaPorContrato($ctrid) {
		$sql = sprintf("SELECT 
							vgcid AS codigo,
							to_char(vgcdtinicio,'DD/MM/YYYY') AS datainicio,
							to_char(vgcdtfim,'DD/MM/YYYY') AS datafim
						FROM
							fabrica.vigenciacontrato
						WHERE
							ctrid=%d
						ORDER BY vgcdtinicio DESC", $ctrid);
		
		return $this->carregar($sql);
	}
	
	public function excluirContrato()
	{
		$ctrid = $_REQUEST['ctrid'];
		
		$sql = sprintf("update fabrica.contrato set ctrstatus = 'I' where ctrid = %d",(int)$ctrid);
		$this->executar($sql);
		$this->commit($sql);
		
	}
	
	public function pesquisarContrato()
	{

            foreach($this->arAtributos as $campos => $valor){
                        
			if($_POST[$campos] && $_POST[$campos] != ""){
				$this->arrPesquisa[] = "ctr.".$campos."::text ilike ('%".$_POST[$campos]."%') ";
			}
			if($_POST['tscid'] && $_POST['tscid'] != ""){
				$this->arrPesquisa[] = "tsc.tscid = ".$_POST['tscid'];
			}
			if($_POST['dtinicio'] && $_POST['dtinicio'] != ""){
				$this->arrPesquisa[] = "ctrdtinicio >= '".$this->trataData($_POST['dtinicio'])."'";
			}
                        if($_POST['dtfim'] && $_POST['dtfim'] != ""){
				$this->arrPesquisa[] = "ctrdtfim >= '".$this->trataData($_POST['dtfim'])."'";
			}
			//if($_POST['tscid'] && $_POST['tscid'] != ""){
			//	$this->arrPesquisa[] = "ctrdtfim <= '".$this->trataData($_POST['dtfim'])."'";
		//	}
		}
		
	}
	
	public function recuperarSituacaoContrato($ctrid)
	{
		$sql = "select
					cts.tscid,
					tscdsc
				from
					fabrica.tiposituacaocontrato tsc
				inner join
					fabrica.contratosituacao cts ON cts.tscid = tsc.tscid
				where
					ctrid = $ctrid
				and
					tsc.tscstatus = 'A'
				and
					cts.ctsstatus = 'A'
				order by
					ctsdtinclusao desc
				limit
					1";
		return $this->pegaLinha($sql);
	}

        /*
	public function recuperarGestorContrato($ctrid)
	{
		$sql = "select
					entnome
				from
					entidade.entidade
				where
					entid = (select entidgestor from fabrica.contrato where ctrid = $ctrid)";
		return $this->pegaUm($sql);
	}
         * 
         */
	
	public function recuperarCoGestorContrato($ctrid)
	{
		$sql = "select
					entnome
				from
					entidade.entidade
				where
					entid = (select entidcogestor from fabrica.contrato where ctrid = $ctrid)";
		return $this->pegaUm($sql);
	}
	
	public function antesSalvar()
	{ 
		$this->ctrdtassinatura	 = $this->trataData($this->ctrdtassinatura);
		$this->ctrdtinicio		 = $this->trataData($this->ctrdtinicio);
		$this->ctrdtfim 		 = $this->trataData($this->ctrdtfim);
		$this->ctrqtdpfcontrato  = $this->trataNumero($this->ctrqtdpfcontrato);
		$this->ctrqtdpfutilizado = $this->trataNumero($this->ctrqtdpfutilizado);
		return true;
	}
	
	public function trataData($data)
	{
		if(strpos($data,"/")){
			$d = explode("/",$data);
			return $d[2]."-".$d[1]."-".$d[0];
		}
		
		if(strpos($data,"-")){		
			$d = explode("-",$data);
			return $d[2]."/".$d[1]."/".$d[0];
		}
		
	}
	
	public function trataNumero($num)
	{
		return str_replace(array(".",","),array("","."),$num);
	}
	
		
	public function salvarSituacaoContrato($ctrid,$situacao)
	{
		$sql = "update fabrica.contratosituacao set ctsstatus = 'I' where ctrid = $ctrid;";
		$sql.= "insert into fabrica.contratosituacao 
					(tscid,ctrid,ctsstatus,ctsdtinclusao,usucpfcadastrador) 
				values 
					($situacao,$ctrid,'A',now(),'".$_SESSION['usucpf']."');";
		$this->executar($sql);
	}
		
}