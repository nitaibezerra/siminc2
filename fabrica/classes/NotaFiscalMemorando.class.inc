<?php

class NotaFiscalMemorando extends Modelo{

	/**
	 * Nome da tabela especificada
	 * @var string
	 * @access protected
	 */
	protected $stNomeTabela = "fabrica.notafiscal";

	/**
	 * Chave primaria.
	 * @var array
	 * @access protected
	 */
	protected $arChavePrimaria = array( "nfsid" );

	/**
	 * Atributos
	 * @var array
	 * @access protected
	*/

	protected $arAtributos = array (
				'nfsid' => null,
				'nfsdsc' => null,
				'nfsdatacadastro' => null,
				'nfsstatus' => null
			);

	protected $arrPesquisa = null;
	
	public function showListaNotaFiscalMemorando()
	{
		$sql = "select
					'<center><img class=\"middle link\" title=\"Alterar Nota\" alt=\"Alterar Nota\" onclick=\"alterarVinculo(\'' || notafiscal.nfsid || '\')\" src=\"../imagens/alterar.gif\" /> <img class=\"middle link\" title=\"Excluir Nota\" alt=\"Excluir Nota\" onclick=\"excluirNota(\'' || notafiscal.nfsid || '\')\" src=\"../imagens/excluir.gif\" /></center>' as acao,
					(select memonumero from fabrica.memorando where memorando.nfsid = notafiscal.nfsid) as memo,	
					nfsdsc as notafiscal
				from
					fabrica.notafiscal
				inner join
					fabrica.memorando
				on
					(notafiscal.nfsid = memorando.nfsid)
				where
					memorando.nfsid is not null
					".($this->arrPesquisa ? "and " . implode(" and ", $this->arrPesquisa) : "")."					
				order by
					notafiscal.nfsid";
		
		$cabecalho = array("Ações","Memorando","Nota Fiscal");

		$this->monta_lista($sql,$cabecalho,100,50,"N","center","N");
	}

	public function excluirVinculo()
	{
		$nfsid = $_REQUEST['nfsid'];

		$sql = sprintf("update fabrica.memorando set nfsid = null where nfsid = %d",(int)$nfsid);
		$this->executar($sql);
		$this->commit($sql);

	}
	
	public function excluirNotaFiscal()
	{
		$nfsid = $_REQUEST['nfsid'];
	
		$this->excluirVinculo();
		$sql = sprintf("delete from fabrica.notafiscal where nfsid = %d",(int)$nfsid);
		$this->executar($sql);
		$this->commit($sql);
	
	}

	public function pesquisarMemorando()
	{

		if($_POST['memoid']){
			$this->arrPesquisa[] = "memorando.memoid = ".$_POST['memoid'];
		}
		
		if ($_POST['nfsdsc']){
			$this->arrPesquisa[] = "notafiscal.nfsdsc::text ilike ('%".$_POST['nfsdsc']."%') ";
		}
		
	}
	
	public function comboMemorando($memoid = null, $obr = 'S')
	{
		$sql = "select
					memoid as codigo,
					memonumero::varchar || '/' || to_char(memodata, 'YYYY')  as descricao
				from
					fabrica.memorando
				where
					nfsid is not null
				order by
					memoid desc";
		$this->monta_combo("memoid",$sql,"S","Selecione...","","","","","$obr","","",$memoid);
	}

	public function comboNotaFiscal($nfsid = null)
	{
		$sql = "select
					nfsid as codigo,
					nfsid as descricao
				from
					fabrica.notafiscal
				order by
					nfsid";
		$this->monta_combo("nfsid",$sql,"S","Selecione...","","","","","S","","",$nfsid);
	}
	
	public function recuperarDataCadastro($nfsid = null)
	{
		
		if(!$nfsid){
			return null;
		}
		
		$sql = sprintf("select
				to_char(nfsdatacadastro, 'DD/MM/YYYY') as datacadastro
				from
					fabrica.notafiscal
				where
					nfsid = %d",(int)$nfsid);
		$data = $this->pegaLinha($sql);
	
		return $data['datacadastro'];
	}
	
	public function recuperarNfDescricao($nfsid = null)
	{
		if(!$nfsid){
			return null;
		}		
		
		$sql = sprintf("select
				nfsdsc
				from
					fabrica.notafiscal
				where
					nfsid = %d",(int)$nfsid);
		
		return $this->pegaLinha($sql);
	}
	
	public function recuperarValorSelecionadoCombo($nfsid = null)
	{
		if(!$nfsid){
			return null;
		}
	
		$sql = sprintf("select
				memoid
				from
					fabrica.memorando
				where
					nfsid = %d",(int)$nfsid);
		
		$memoid = $this->pegaLinha($sql);
	
		return $memoid['memoid'];
	}
	
/*	public function salvarVinculo()
	{
		$this->salvar();
	}*/
	
	public function salvar( $boAntesSalvar = true, $boDepoisSalvar = true, $arCamposNulo = array() ){
		 
		if( $boAntesSalvar ){
			if( !$this->antesSalvar() ){ return false; }
		}
			
		if( count( $this->arChavePrimaria ) > 1 ) trigger_error( "Favor sobreescrever método na classe filha!" );
	
		$stChavePrimaria = $this->arChavePrimaria[0];
	
		if( $this->$stChavePrimaria && !$this->tabelaAssociativa ){
			$this->excluirVinculo($_POST['memoid_disable']);
			
			$this->arAtributos['nfsdatacadastro'] = 'NOW()';
			$this->alterar($arCamposNulo);
			
			$this->atualizarMemorando($_POST['nfsid'], $_POST['memoid_disable']);
			//$this->alterar($arCamposNulo);
			//$resultado = $this->$stChavePrimaria;
			$resultado = true;
		}else{
			$verify = $this->verificarNotaExistente($_POST['nfsdsc']);
			
			if($verify == 0)
			{
				$this->arAtributos['nfsdatacadastro'] = 'NOW()';
				$resultado = $this->inserir();
					
				$this->atualizarMemorando($resultado, $_POST['memoid_disable']);
				
				$resultado = true;
			}else{
				$resultado = false;
			}			
		}
		if( $resultado ){
			if( $boDepoisSalvar ){
				$this->depoisSalvar();
			}
		}
		return $resultado;
	} // Fim salvar()
	
	public function verificarNotaExistente($nfsdsc)
	{
		$sql = "select 
					count(nfsdsc)
				from
					fabrica.notafiscal
				where
					nfsdsc = '$nfsdsc'";
		
		$count = $this->pegaLinha($sql);
		
		return $count['count'];
	}
	
	public function atualizarMemorando($nfsid, $memoid)
	{
		$sql = sprintf("update fabrica.memorando set nfsid = %d where memoid = %d",(int)$nfsid,(int)$memoid);
		$this->executar($sql);
		$this->commit();
	}
}