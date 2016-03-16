<?php
	
class Reiteracao extends Modelo{
	
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "gestaodocumentos.reiteracoes";	

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "rtrid");

    /**
     * Atributos
     * @var array
     * @access protected
     */    
    protected $arAtributos = array(
            'rtrid'             => null,
            'taridprincipal'    => null,
            'taridsecundario'   => null
          );	
    
    public function deletarReiteracaoPorTarid( $taridprincipal ){
        #EXCLUIR REITERAÇÃO ANTES DE INSERIR NOVAS, DE ACORDO COM O ID DA TAREFA PRINCIPAL.
        $sql = "DELETE FROM gestaodocumentos.reiteracoes WHERE taridprincipal = {$taridprincipal} RETURNING taridprincipal;";//ver($sql, d);
        $dados = $this->pegaUm($sql);

        if ( $dados > 0 ){
            return true;
        }
    }
    public function deletarReiteracaoPorRtrid( $rtrid ){
        #EXCLUIR REITERAÇÃO ESPECIFICA DE ACORDO COM O rtrid.
        $sql = "DELETE FROM gestaodocumentos.reiteracoes WHERE rtrid = {$rtrid} RETURNING rtrid;";
        $dados = $this->pegaUm($sql);

        if ( $dados > 0 ){
            return true;
        }
    }
}