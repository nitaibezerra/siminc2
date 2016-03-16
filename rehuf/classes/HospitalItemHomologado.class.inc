<?php
	
class HospitalItemHomologado extends Modelo{
	
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "rehuf.hospitalitemhomologado";	

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "hitid" );

    /**
     * Atributos
     * @var array
     * @access protected
     */    
    protected $arAtributos     = array(
									  	'hitid' => null,
    									'entid' => null, 
    									'iteid' => null,
    									'tpaid' => null,
    									'usucpf' => null,
    									'hitnomefornecedor' => null,
    									'hitcnpjfornecedor' => null,
    									'hitvalorunitario' => null,
    									'hitpregao' => null,
    									'hitmarca' => null,
    									'hitdatavigencia' => null,
    									'hitobs' => null,
    									'hitdatainclusao' => null,
    									'hitstatus' => null,
									  );
									
	public function salvarItens()
	{
		$preid = $_POST['preid'];
		$arrItens = $_POST['iteid'];
		$sqlI = "delete from rehuf.hospitalitemhomologado hih where hih.entid = {$_SESSION['rehuf_var']['entid']} and iteid in (select ite.iteid from rehuf.itemhomologado ite where ite.preid = $preid);";
		if($arrItens){
			foreach($arrItens as $iteid)
			{
				$tpaid = $_POST['tpaid'][$iteid] ? $_POST['tpaid'][$iteid] : "NULL";
				$hitnomefornecedor = $_POST['hitnomefornecedor'][$iteid] ? "'".$_POST['hitnomefornecedor'][$iteid]."'" : "NULL";
				$hitcnpjfornecedor = $_POST['hitcnpjfornecedor'][$iteid] ? "'".$_POST['hitcnpjfornecedor'][$iteid]."'" : "NULL";
				$hitvalorunitario = $_POST['hitvalorunitario'][$iteid] ? str_replace(array(".",","),array("","."),$_POST['hitvalorunitario'][$iteid]) : "NULL";
				$hitpregao = $_POST['hitpregao'][$iteid] ? "'".$_POST['hitpregao'][$iteid]."'" : "NULL";
				$hitmarca = $_POST['hitmarca'][$iteid] ? "'".$_POST['hitmarca'][$iteid]."'" : "NULL";
				$hitdatavigencia = $_POST['hitdatavigencia'][$iteid] ? "'".formata_data_sql($_POST['hitdatavigencia'][$iteid])."'" : "NULL";
				$hitobs = $_POST['hitobs'][$iteid] ? "'".$_POST['hitobs'][$iteid]."'" : "NULL";
				
				$sqlI.= "insert into 
							rehuf.hospitalitemhomologado 
						(entid,iteid,tpaid,usucpf,hitnomefornecedor,hitcnpjfornecedor,hitvalorunitario,hitpregao,hitmarca,hitdatavigencia,hitobs) 
							values 
						({$_SESSION['rehuf_var']['entid']},$iteid,$tpaid,'{$_SESSION['usucpf']}',$hitnomefornecedor,$hitcnpjfornecedor,$hitvalorunitario,$hitpregao,$hitmarca,$hitdatavigencia,$hitobs);";
			}
		}
		if($this->executar($sqlI)){
			$this->commit();
			$_SESSION['rehuf']['hospitalitemhomologado']['alert'] = "Operação realizada com sucesso.";
		}else{
			$_SESSION['rehuf']['hospitalitemhomologado']['alert'] = "Não foi possível realizar a operação.";
		}
		header("Location: rehuf.php?modulo=pregao/itemPregaoHomologado&acao=A&preid={$_GET['preid']}");
		exit;
	}
	
	public function recuperarItens()
	{
		$sql = "select * from rehuf.hospitalitemhomologado where entid = {$_SESSION['rehuf_var']['entid']}";
		$arrDados = $this->carregar($sql);
		if($arrDados){
			foreach($arrDados as $dado){
				$arrItens[$dado['iteid']] = $dado;
			}
		}
		return $arrItens ? $arrItens : array();
	}
	
	function salvarItensAjax()
	{
		$sqlI = "delete from rehuf.hospitalitemhomologado where entid = {$_SESSION['rehuf_var']['entid']};";
		$arrItens = $_POST['iteid'];
		if($arrItens){
			foreach($arrItens as $iteid)
			{
				$tpaid = $_POST['tpaid'][$iteid] ? $_POST['tpaid'][$iteid] : "NULL";
				$hitnomefornecedor = $_POST['hitnomefornecedor'][$iteid] ? "'".utf8_decode($_POST['hitnomefornecedor'][$iteid])."'" : "NULL";
				$hitcnpjfornecedor = $_POST['hitcnpjfornecedor'][$iteid] ? "'".$_POST['hitcnpjfornecedor'][$iteid]."'" : "NULL";
				$hitvalorunitario = $_POST['hitvalorunitario'][$iteid] ? str_replace(array(".",","),array("","."),$_POST['hitvalorunitario'][$iteid]) : "NULL";
				$hitpregao = $_POST['hitpregao'][$iteid] ? "'".utf8_decode($_POST['hitpregao'][$iteid])."'" : "NULL";
				$hitmarca = $_POST['hitmarca'][$iteid] ? "'".utf8_decode($_POST['hitmarca'][$iteid])."'" : "NULL";
				$hitdatavigencia = $_POST['hitdatavigencia'][$iteid] ? "'".formata_data_sql($_POST['hitdatavigencia'][$iteid])."'" : "NULL";
				$hitobs = $_POST['hitobs'][$iteid] ? "'".utf8_decode($_POST['hitobs'][$iteid])."'" : "NULL";
		
				$sqlI.= "insert into
				rehuf.hospitalitemhomologado
				(entid,iteid,tpaid,usucpf,hitnomefornecedor,hitcnpjfornecedor,hitvalorunitario,hitpregao,hitmarca,hitdatavigencia,hitobs)
				values
				({$_SESSION['rehuf_var']['entid']},$iteid,$tpaid,'{$_SESSION['usucpf']}',$hitnomefornecedor,$hitcnpjfornecedor,$hitvalorunitario,$hitpregao,$hitmarca,$hitdatavigencia,$hitobs);";
			}
		}
		if($this->executar($sqlI)){
			$this->commit();
		}
		exit;
	}
}