<?php
	
class ContratoSistema extends Modelo{
	
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "fabrica.contratosistema";	

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "ctsid" );
    
    /**
     * Atributos
     * @var array
     * @access protected
     */    
    protected $arAtributos     = array(
									  	'ctsid' => null, 
									  	'ctrid' => null,
									  	'sidid' => null
									  );
	
	protected $arrPesquisa = null;
	
	public function showListaContratoSistema()
	{
				
		$sql = "select
					'<center><img class=\"middle link\" title=\"Alterar Sistema\" alt=\"Alterar Sistema\" onclick=\"alterarSistema(\'' || ctsid || '\')\" src=\"../imagens/alterar.gif\" /> <img class=\"middle link\" title=\"Excluir Sistema\" alt=\"Excluir Sistema\" onclick=\"excluirSistema(\'' || ctsid || '\')\" src=\"../imagens/excluir.gif\" /></center>' as acao,
					(select siddescricao from demandas.sistemadetalhe sid where cts.sidid = sid.sidid) as sistema,
					(select ctrnumero || ' - ' || to_char(ctrdtinicio,'DD/MM/YYY') from fabrica.contrato ctr where cts.ctrid = ctr.ctrid) as contrato
				from
					fabrica.contratosistema cts
				where
					ctsstatus = 'A' 
					".($this->arrPesquisa ? "and " . implode(" and ", $this->arrPesquisa) : "")."
				order by
					ctsid";
		
		$cabecalho = array("Ações","Sistema","Contrato");
		
		$this->monta_lista($sql,$cabecalho,100,50,"N","center","N");
	}
	
	public function excluirContratoSistema()
	{
		$ctsid = $_REQUEST['ctsid'];
		
		$sql = sprintf("update fabrica.contratosistema set ctsstatus = 'I' where ctsid = %d",(int)$ctsid);
		$this->executar($sql);
		$this->commit($sql);
		
	}
	
	public function pesquisarContratoSistema()
	{
		foreach($this->arAtributos as $campos => $valor){
			if($_POST[$campos] && $_POST[$campos] != ""){
				$this->arrPesquisa[] = $campos."::text ilike ('%".$_POST[$campos]."%') ";
			}
		}
		
	}
	
	public function comboSistema($sidid = null)
	{
		$sql = "select
					sidid as codigo,
					sidabrev ||' - '|| siddescricao as descricao
				from
					demandas.sistemadetalhe
				where
					sidid in (select distinct sidid from fabrica.contratosistema where ctsstatus = 'A')
				order by
					2";
		$this->monta_combo("sidid",$sql,"S","Selecione...","","","","","S","","",$sidid);
	}
	
	public function comboContrato($ctrid = null)
	{
		$sql = "select
					ctrid as codigo,
					ctrnumero as descricao
				from
					fabrica.contrato
				where
					ctrid in (select distinct ctrid from fabrica.contratosistema where ctsstatus = 'A')
				order by
					ctrnumero";
		$this->monta_combo("ctrid",$sql,"S","Selecione...","","","","","S","","",$ctrid);
	}

	public function recuperarProdutos($ctsid = null)
	{
		if(!$ctsid){
		return array();
		}
		
		$sql = sprintf("select distinct
							prd.prdid as codigo,
							prd.prddsc as descricao 
						from 
							fabrica.sistemaproduto spr
						inner join 
							fabrica.produto prd ON prd.prdid = spr.prdid
						where 
							spr.ctsid = %d
						and
							prd.prdstatus = 'A'",
				(int)$ctsid);
		
		return $this->carregar($sql);
	}

	public function inserirProdutos($ctsid,$arrDados = null)
	{
		
		if($arrDados && is_array($arrDados)){
			
			foreach($arrDados as $produto){
				$sql .= "insert into fabrica.sistemaproduto (ctsid,prdid) values ($ctsid,$produto);";
			}
			$this->executar($sql);
			$this->commit();
		}
	 
	}
	
	public function deletarProdutos($ctsid)
	{
		$sql = sprintf("delete from fabrica.sistemaproduto where ctsid = %d",(int)$ctsid);
		$this->executar($sql);
		$this->commit(); 
	}
		
}