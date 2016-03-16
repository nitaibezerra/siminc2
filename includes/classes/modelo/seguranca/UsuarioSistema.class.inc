<?php

class UsuarioSistema extends Modelo{

    /**
    * Nome da tabela especificada
    * @var string
    * @access protected
    */
    protected $stNomeTabela = "seguranca.usuario_sistema";

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "usucpf", "sisid" );

    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributos     = array(
        'usucpf'            => null,
        'sisid'             => null,
        'susstatus'         => null,
        'pflcod'            => null,
        'susdataultacesso'  => null,
        'suscod'            => null
    );

    public function getDadosPorUsucpfSisid($cpf, $sisid){
        // verifica se o usuário já está cadastrado no módulo selecionado
        $sql = sprintf("SELECT usucpf, sisid, suscod FROM usuario_sistema WHERE usucpf = '%s' AND sisid = %d",$cpf,$sisid);

        $usuario_sistema = (object) $this->pegaLinha( $sql );

        return $usuario_sistema;
    }

    public function salvar($boAntesSalvar = true, $boDepoisSalvar = true, $arCamposNulo = array()){
        // vincula o usuário com o módulo
        $sql = "INSERT INTO {$this->stNomeTabela} ( usucpf, sisid, pflcod ) values ( '{$this->usucpf}', {$this->sisid}, {$this->pflcod} )";
        $this->executar( $sql );
    }
}