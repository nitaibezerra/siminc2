<?php

class Categoria extends Modelo{
	
	/**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "monitoraseb.categoria";	

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "catid" );
    
    /**
     * Atributos
     * @var array
     * @access protected
     */    
    protected $arAtributos     = array(
									  	'catid' => null, 
									  	'catnome' => null,
									  	'catvalidacao' => null, 
    									'catbolsista' => null, 
									  	'catnumdoclegal' => null,
    									'catvalunibolsa' => null
									  );
	
	protected $arrPesquisa = null;
	private $arrMessage = array();
	
	private function setMessage($message){
		$this->arrMessage[] = $message;
	}
	
	public function getMessage(){
		if(count($this->arrMessage)){
			foreach($this->arrMessage as $message):
				$html .= "<p style=\"color: blue; font-weight:bold;\">".$message."</p>";
			endforeach;
			return $html;
		}else{
			return false;
		}
	}
	
	public function buscaListaCategoria($campoPesquisa){
		$where = " where 1=1 ";
		if(!empty($campoPesquisa)){
			$where .=" and upper(cat.catnome) like upper('%".$campoPesquisa."%') ";
		} 
		$sql = "SELECT '<center>
					<a style=\"cursor:pointer;\" onclick=\"detalharCategoria(\''||cat.catid||'\');\">
					<img src=\"/imagens/alterar.gif \" border=0 title=\"Alterar\">
					</a>
					<a  style=\"cursor:pointer;\" onclick=\"if(confirm(\'Deseja excluir o registro?\')){removerCategoria(\''||cat.catid||'\');}\">
					<img src=\"/imagens/excluir.gif \" border=0 title=\"Excluir\">
					</a>
					</center>' as acao, 
					cat.catnome, 
					(CASE WHEN cat.catvalidacao is true
						THEN 'Sim'
						ELSE 'Não'
					END) as catvalidacao,
					(CASE WHEN cat.catbolsista is true
						THEN 'Sim'
						ELSE 'Não'
					END) as catbolsista
				FROM monitoraseb.categoria cat ".$where." ORDER BY cat.catnome ASC";
		
		$cabecalho = array("Ação","Nome","Validação","Bolsista");
		$this->monta_lista($sql,$cabecalho,100,50,"N","center","N");
	}
	
	public function salvarCategoria($catid,$catnome, $catvalidacao, $catbolsista, $catnumdoclegal, $catvalunibolsa){
		$valido = $this->validaCategoria($catid,$catnome, $catvalidacao, $catbolsista, $catnumdoclegal, $catvalunibolsa);
		if($valido){
			$sql = "";
			if($catid!=null && $catid!=""){
				$sql.= "update monitoraseb.categoria 
					set catnome='$catnome', 
					    catvalidacao='$catvalidacao', 
					    catbolsista='$catbolsista', 
					    catnumdoclegal='$catnumdoclegal'";
						if($catbolsista=='t') {
					    	$sql.=", catvalunibolsa=".$this->formataMoeda2Numeric($catvalunibolsa);
						}else{
							$sql.=", catvalunibolsa=NULL ";
						}
					$sql.=" where catid=$catid";
				$this->executar($sql);
				$this->commit();
				$this->setMessage("Registro alterado com sucesso!");
			}else{
				$sql.= "insert into monitoraseb.categoria 
							(catnome, catvalidacao, catbolsista, catnumdoclegal, catvalunibolsa) 
						values 
							('$catnome', '$catvalidacao', '$catbolsista', '$catnumdoclegal' ";
						if($catbolsista=='t') {
					    	$sql.=", ".$this->formataMoeda2Numeric($catvalunibolsa);
						}else{
							$sql.=", NULL ";
						}
					$sql.= ")";
				$this->executar($sql);
				$this->commit();
				$this->setMessage("Registro inserido com sucesso!");
			}
			return "";
		}else{
			return "Favor preencher todos os campos obrigatórios!";
		}
	}
	
	private function validaCategoria($catid,$catnome, $catvalidacao, $catbolsista, $catnumdoclegal, $catvalunibolsa){
		if(empty($catnome) || empty($catvalidacao) || empty($catbolsista) || (empty($catnumdoclegal) && $catbolsista=='t') || (empty($catvalunibolsa) && $catbolsista=='t')){
			return false;					
		}else{
			return true;
		}
	}
	
	public function formataNumeric2Moeda($valor){
		$number=number_format($valor,2,',','.');
		return $number;
	}
	
	public function formataMoeda2Numeric($valor){
		$number = str_replace(".","",$valor);
		$number = str_replace(",",".",$number);
		$number=number_format($number,2,'.','');
		return $number; 
	}
	public function excluirCategoria($catid){
		if($this->eof("select cat.* FROM monitoraseb.categoria cat JOIN monitoraseb.equipecursomestre ecm ON (cat.catid = ecm.catid) WHERE cat.catid= $catid")){
	        $this->excluir($catid,false);
	        $this->commit();
	        $this->setMessage("Registro excluído com sucesso!");
	        return "";
        }else{
	        return "Operação não realizada. Categoria vinculada ao Curso Mestre.";
        }
	}

}
