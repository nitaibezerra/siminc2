<?php
	
class MensagemCampo extends Modelo{

    const ID_PFL_PERFIL_AVISADO     = '1';
    const ID_PFL_PERFIL_SOLICITADO  = '2';
    const ID_WKF_ESTADO_ORIGEM      = '3';
    const ID_WKF_ESTADO_DESTINO     = '4';

    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "seguranca.mensagemcampo";	

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "mcaid" );

    /**
     * Atributos
     * @var array
     * @access protected
     */    
    protected $arAtributos     = array(
									  	'mcaid' => null, 
									  	'mreid' => null, 
									  	'mctid' => null, 
									  	'mcavalor' => null, 
									  );
}