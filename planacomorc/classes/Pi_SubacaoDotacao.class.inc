<?php
    
class Pi_SubacaoDotacao extends Modelo{
    
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "monitora.pi_subacaodotacao";	
        
    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "sbaid","ptrid" );
        
    /**
     * Atributos
     * @var array
     * @access protected
     */    
    protected $arAtributos     = array(
									  	'sbaid' => null, 
									  	'ptrid' => null, 
									  	'sadvalor' => null, 
									  );
                                                                              
	public function salvar($sbaid, $arPtrid, $arValor) {
        $arPtrid = ($arPtrid) ? $arPtrid : array();
        $arValor = ($arValor) ? $arValor : array();

            if (count($arPtrid) && count($arValor)) {
                foreach ($arPtrid as $key => $value) {
                    //ver($arPtrid,$arValor, $value, $arValor[$key][$value],$arValor[$value], $arValor[$value][$key]);
                    //if(!$arValor[$key][$value]){
                    //    $sadvalor = str_replace(array(".",","), array("","."), $arValor[$value]);
                    //}else{
                    $sadvalor = str_replace(array(".", ","), array("", "."), $arValor[$key][$value]);
                    //}
                   // ver($sadvalor,d);
                    if($sadvalor != '' || $sadvalor != null){
                    $sql = "INSERT INTO monitora.pi_subacaodotacao (sbaid, ptrid, sadvalor) VALUES ('" . $sbaid . "', '" . $value . "', " . $sadvalor . ")";
                    $this->executar($sql);
                    }
                }
            }
    }

}