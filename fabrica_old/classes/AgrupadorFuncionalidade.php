<?php
class AgrupadorFuncionalidade extends Modelo
{
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "inventario.tb_simec_agrupador_funcionalidade";

    /**
     * Chave primaria
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "co_agrupador" );

    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributos = array(
        'co_agrupador'  => null,
        'no_agrupador'  => null,
        'dt_cadastro'   => null,
        'st_agrupador_funcionalidade'   => null
    );
    
    public function verificaSeExisteAgrupadorPorNome($nomeAgrupador){
    	$sql ="SELECT co_agrupador 
    		     FROM inventario.tb_simec_agrupador_funcionalidade
    		     WHERE no_agrupador = '{$nomeAgrupador}'";
    	$result=$this->pegaUm($sql);
    	return $result != false ? $result["co_agrupador"]: false; 
    }
    
    public function salvarNovo( $noAgrupador, $sidid, $dtCadastro )
    {
    	$sql = "insert into $this->stNomeTabela(no_agrupador
    			, dt_cadastro, st_agrupador_funcionalidade, sidid)
    			values('{$noAgrupador}', '{$dtCadastro}', 'A', {$sidid}) returning co_agrupador";

    	return $this->pegaUm( $sql );
    }
    
    public function listarArupadoresPorSistema( $sidid ){
    	
    	$sql = "SELECT
		    		co_agrupador as codigo,
		    		no_agrupador as descricao
		    	FROM inventario.tb_simec_agrupador_funcionalidade
		    	WHERE st_agrupador_funcionalidade = 'A'
		    	AND sidid={$sidid}
		    	ORDER BY no_agrupador";
    	
    	return $this->carregar( $sql );
    }
}
   