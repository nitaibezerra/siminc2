<?php


class ModeloDocumentoPar extends Modelo{
	
	// TODO: implementar usando metodos default da Modelo

	/**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "par.documentopar";

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "dopid" );

    /**
     * Atributos
     * @var array
     * @access protected
     */    
    protected $arAtributos     = array(
									'dopid' => null, // not null
									'prpid' => null,
									'doptexto' => null,
									'dopstatus' => null,
									'dopdiasvigencia' => null,
									'dopdatainicio' => null,
									'dopdatafim' => null,
									'mdoid' => null,
									'dopdatainclusao' => null,
									'usucpfinclusao' => null,
									'dopdataalteracao' => null,
									'usucpfalteracao' => null,
									'dopjustificativa' => null,
									'dopdatavalidacaofnde' => null,
									'dopusucpfvalidacaofnde' => null,
									'dopdatavalidacaogestor' => null,
									'dopusucpfvalidacaogestor' => null,
									'dopusucpfstatus' => null,
									'dopdatastatus' => null,
									'dopdatapublicacao' => null,
									'doppaginadou' => null,
									'dopnumportaria' => null,
									'proid' => null,
									'dopreformulacao' => null,
									'dopidpai' => null,
									'dopidaditivo' => null,
									'dopnumerodocumento' => null,
									'dopobservacao' => null,
									'dopdatafimvigencia' => null,
									'dopvalortermo' => null,
									'dopdatainiciovigencia' => null,
									'arqid' => null,
									'dopacompanhamento' => null,
									'dopano' => null,
									'dopdocumentocarregado' => null,

									'programa' => null,
									'nprocesso' => null,
									'prefeitura_nome' => null,
									'prefeitura_cnpj' => null,
									'prefeitura_endereco' => null,
									'prefeitura_municipio' => null,
									'prefeitura_uf' => null,
									'prefeito_nome' => null,
									'prefeito_cpf' => null,
									'mesinicial' => null,
									'mesfinal' => null
									 );
									  
	protected $stOrdem = null;

}
