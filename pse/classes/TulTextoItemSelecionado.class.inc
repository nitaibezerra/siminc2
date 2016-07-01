<?php
	
class TulTextoItemSelecionado extends Modelo{
	
	public function __construct($pamid,$muncod = null ){
		
		parent::__construct();
		
	}	
	
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "pse.tultextoitemselecionado";	

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "tulid" );

    /**
     * Atributos
     * @var array
     * @access protected
     */    
    protected $arAtributos     = array(
									  	'tulid' => null, 
									  	'isuid' => null, 
									  	'tultexto' => null, 
									  	'tulquantidade' => null,  
									  );
	protected $cneid;
									  
	function carregaListaParcerias($parcerias, $rulid)
	{
		$usucpf = $_SESSION['usucpf'];
		$pflcod = pegaPerfil($usucpf);
				
		$parcerias=($parcerias==''?0:$parcerias);
		$rulid=($rulid==''?0:$rulid);
		$sql = "SELECT ";

		if($pflcod == ESCOLA_ESTADUAL || $pflcod == ESCOLA_MUNICIPAL || $pflcod == SUPER_USUARIO) {
			$sql.= "'<center><img src=\"/imagens/excluir.gif \" style=\"cursor: pointer\" onclick=\"excluiParceria('||tul.tulid||','||tul.isuid||');\" border=0 alt=\"Ir\" title=\"Excluir\"></center>'";
		} else {
			$sql.= "''";
		}
		$sql.= " as acao,
					 tul.tultexto
				FROM 
					pse.tultextoitemselecionado tul
					
				INNER JOIN pse.isuitemselecionado AS item ON tul.isuid = item.isuid

				WHERE
					item.iulid = $parcerias AND
					item.rulid = $rulid";
		$cabecalho = array("Ação","Parcerias");
		$alinha = array("center","left");
		$tamanho = array("5%","95%");
		$this->monta_lista( $sql, $cabecalho, 25, 10, 'N', 'center', '', '',$tamanho,$alinha);
	}							  
							  
}