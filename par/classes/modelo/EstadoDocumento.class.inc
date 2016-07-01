<?php

class EstadoDocumento extends Modelo {

    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "workflow.estadodocumento";

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array("esdid");

    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributos = array(
        'esdid' => null,
        'tpdid' => null,
        'esdstatus' => null,
        'esdordem' => null,
        'esddsc' => null 
    );
   
    public function carregarEstadoDocumento($esdid, $booSQL){
          $sql = "SELECT 
                    esdid as codigo, 
                    esddsc as descricao
                FROM {$this->stNomeTabela}
                WHERE tpdid = {$esdid}
                AND estadodocumento.esdstatus = 'A'
                ORDER BY descricao";
          return ($booSQL) ? $sql : $this->carregar($sql);
    }
}
