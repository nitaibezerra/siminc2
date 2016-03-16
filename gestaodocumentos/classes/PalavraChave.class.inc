<?php
	
class PalavraChave extends Modelo{
	
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "gestaodocumentos.palavrachave";	

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "plcid" );

    /**
     * Atributos
     * @var array
     * @access protected
     */    
    protected $arAtributos = array(
        'plcid'     => null,
        'tarid'     => null, 
        'plcdsc'    => null
      );	
    
    public function deletarPalavraPorTarid( $tarid ){
        #EXCLUIR PALAVRA CHAVE ANTES DE INSERIR NOVAS, DE ACORDO COM O ID DA TAREFA.
        $sql = "DELETE FROM gestaodocumentos.palavrachave WHERE tarid = {$tarid} RETURNING plcid;";
        $dados = $this->pegaUm($sql);

        if ( $dados > 0 ){
            return true;
        }
    }
}