<?php
	
class ComposicaoSubacao extends Modelo{
	
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "cte.composicaosubacao";	

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "cosid" );

    /**
     * Atributos
     * @var array
     * @access protected
     */    
    protected $arAtributos     = array(
									  	'cosid' => null, 
									  	'sbaid' => null, 
									  	'cosord' => null, 
									  	'cosdsc' => null, 
									  	'cosqtd' => null, 
									  	'cosvlruni' => null, 
									  	'unddid' => null, 
									  	'ilbid' => null, 
									  	'cosano' => null, 
									  	'cosnome' => null, 
									  	'coscpf' => null, 
									  	'entid' => null, 
									  	'polid' => null, 
									  );
	
	public function excluirPorSubacao($sbaid, $parametro = NULL){
		$sql = "SELECT cosid FROM ".$this->stNomeTabela." WHERE sbaid =".$sbaid." ".$parametro;
		$itens = $this->carregar($sql);
		if(is_array($itens)){
			foreach($itens as $item){
				if($this->excluir($item['cosid'])){
					$this->commit();
				}else{
					return false;
				}
			}
			return true;
		}else{
			// Se não existe itens de composição.
			return false;
		}
	}
	
	public function carregaPorSubacao($sbaid, $parametro = NULL){
		$sql = "SELECT * FROM ".$this->stNomeTabela." WHERE sbaid =".$sbaid." ".$parametro;
		$itens = $this->carregar($sql);
		return $itens;
	}
}