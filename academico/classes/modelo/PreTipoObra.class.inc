<?php
	
class PreTipoObra extends Modelo{
	
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "obras.pretipoobra";	

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "ptoid" );

    /**
     * Atributos
     * @var array
     * @access protected
     */    
    protected $arAtributos     = array(
									  	'ptoid' => null, 
									  	'ptodescricao' => null, 
									  	'ptostatus' => null,									  	 
									  );
									  
									  
	public function recuperarTiposObraAtivas( )
	{
		switch ($_SESSION['par']['tipo']){
			case 'Q':
				$filtroTipo = "AND ptoclassificacaoobra = 'Q'";
				//$filtroTipo = 'AND ptoid in (5,9)';
				break;
			case 'C':
				$filtroTipo = "AND ptoclassificacaoobra = 'C'";
				//$filtroTipo = 'AND ptoid in (4,8,10)';
				break;
			default:
				$filtroTipo = '';
				break;
		}
		$esfera = $_SESSION['par']['muncod'] ? 'M' : 'E';
		$retorno = Array('codigo'=>'','descricao'=>'');
		if($tooid){
			$sql = "SELECT 
						ptoid as codigo,
						ptodescricao as descricao
					FROM {$this->stNomeTabela} 
					WHERE 
						ptostatus = 'A'
						$filtroTipo
						AND tooid = 3
						AND ptoesfera in ('T','{$esfera}')
					ORDER BY ptodescricao";
			$retorno = $this->carregar($sql);
		}
		
		return $retorno ? $retorno : Array('codigo'=>'','descricao'=>'');
	}

}